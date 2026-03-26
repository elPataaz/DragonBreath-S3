#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DFRobot_DF1201S.h>

// Pin Mapping
#define BLUE_LED 1
#define RED_LED 2
#define RGB_RED 7
#define RGB_GREEN 5
#define RGB_BLUE 40
#define START_BUTTON 4 
#define POT_PIN 6     

Adafruit_SSD1306 display(128, 64, &Wire, -1);
HardwareSerial dfSerial(1); 
DFRobot_DF1201S dfplayer;

enum GagState { WARMUP, READY_WAIT, SENSING, ANALYSIS_HOLD, BUSTED, SHUTDOWN };
GagState currentState = WARMUP;

unsigned long timer = 0, volTimer = 0, volDisplayTimer = 0, lastDebounceTime = 0;
const unsigned long debounceDelay = 50; 
bool lastButtonState = HIGH, buttonStableState = HIGH, showVolBriefly = false;
int currentVol = -1;

void setup() {
  pinMode(BLUE_LED, OUTPUT); pinMode(RED_LED, OUTPUT);
  pinMode(RGB_RED, OUTPUT); pinMode(RGB_GREEN, OUTPUT); pinMode(RGB_BLUE, OUTPUT);
  pinMode(START_BUTTON, INPUT_PULLUP);

  Wire.begin(8, 9);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextColor(WHITE);
  
  dfSerial.begin(115200, SERIAL_8N1, 18, 17);
  if (dfplayer.begin(dfSerial)) {
    dfplayer.setPrompt(false);
    dfplayer.switchFunction(dfplayer.MUSIC);
    dfplayer.setPlayMode(dfplayer.SINGLECYCLE);
  }
  timer = millis();
}

void loop() {
  bool clicked = checkButtonClick();
  handleVolume();

  if (showVolBriefly && (millis() - volDisplayTimer < 2000)) {
    displayVolume();
  } else {
    runStateMachine(clicked);
  }
}

void runStateMachine(bool clicked) {
  unsigned long elapsed = millis() - timer;

  switch (currentState) {
    case WARMUP:
      setRGB(0, 255, 0); // Solid Green
      drawCenteredText("warming up:", 1);
      if (elapsed > 5000) {
        timer = millis();
        currentState = READY_WAIT;
      }
      break;

    case READY_WAIT:
      setRGB(0, 255, 0);
      drawCenteredText("system ready:", 1);
      if (clicked) {
        dfplayer.playSpecFile("/0002.mp3"); // Start Jaws
        // Apply Volume Boost (+6) for the quiet Jaws track
        dfplayer.setVol(constrain(currentVol + 6, 0, 30)); 
        timer = millis();
        currentState = SENSING;
      }
      break;

    case SENSING: {
      // RAMPING BLINK: Starts slow (400ms), ends fast (50ms)
      int blinkRate = map(elapsed, 0, 10000, 400, 50);
      setRGB(0, (millis() / blinkRate) % 2 * 255, 0); 

      display.clearDisplay();
      if ((millis() / 300) % 2 == 0) {
        display.setTextSize(2);
        display.setCursor(15, 15);
        display.print("Sensing:");
      }
      display.drawRect(10, 45, 108, 10, WHITE);
      display.fillRect(10, 45, map(elapsed, 0, 10000, 0, 108), 10, WHITE);
      display.display();

      if (elapsed > 10000) {
        timer = millis();
        currentState = ANALYSIS_HOLD;
      }
    } break;

    case ANALYSIS_HOLD:
      setRGB(255, 255, 0); // Solid Yellow (Tension)
      display.clearDisplay();
      if ((millis() / 300) % 2 == 0) {
        display.setTextSize(2);
        display.setCursor(10, 25);
        display.print("Analysis:");
      }
      display.display();

      if (clicked && elapsed > 4000) {
        dfplayer.pause();
        dfplayer.setVol(currentVol); // Reset volume to actual knob level
        dfplayer.playSpecFile("/0003.mp3"); // Start Police
        currentState = BUSTED;
      }
      break;

    case BUSTED: { 
      display.clearDisplay();
      display.setTextSize(2); display.setCursor(20, 10); display.println("BUSTED!");
      display.setTextSize(1); display.setCursor(25, 35); display.println("u holding out?");
      display.display();

      int strobe = (millis() / 100) % 2;
      if (strobe == 0) {
        digitalWrite(RED_LED, HIGH); digitalWrite(BLUE_LED, LOW);
        setRGB(255, 0, 0);
      } else {
        digitalWrite(RED_LED, LOW); digitalWrite(BLUE_LED, HIGH);
        setRGB(0, 0, 255);
      }
      if (clicked) currentState = SHUTDOWN;
    } break;

    case SHUTDOWN:
      dfplayer.pause();
      digitalWrite(BLUE_LED, LOW); digitalWrite(RED_LED, LOW);
      setRGB(0, 0, 0);
      display.clearDisplay(); display.display();
      break;
  }
}

void setRGB(int r, int g, int b) {
  analogWrite(RGB_RED, r);
  analogWrite(RGB_GREEN, g);
  analogWrite(RGB_BLUE, b);
}

void handleVolume() {
  if (millis() - volTimer > 150) {
    int potVal = analogRead(POT_PIN);
    int newVol = map(potVal, 0, 4095, 0, 30);
    if (abs(newVol - currentVol) > 1) { 
      dfplayer.setVol(newVol);
      currentVol = newVol;
      showVolBriefly = true;
      volDisplayTimer = millis();
    }
    volTimer = millis();
  }
}

void displayVolume() {
  display.clearDisplay();
  display.setTextSize(1); display.setCursor(45, 10); display.print("VOLUME");
  display.setTextSize(3); display.setCursor(50, 30); display.print(currentVol);
  display.display();
}

void drawCenteredText(String msg, int sz) {
  display.clearDisplay();
  display.setTextSize(sz);
  display.setCursor(10, 25);
  display.println(msg);
  display.display();
}

bool checkButtonClick() {
  bool reading = digitalRead(START_BUTTON);
  bool clickDetected = false;
  if (reading != lastButtonState) lastDebounceTime = millis();
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (buttonStableState == LOW && reading == HIGH) clickDetected = true;
    buttonStableState = reading;
  }
  lastButtonState = reading;
  return clickDetected;
}
