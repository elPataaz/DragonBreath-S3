#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DFRobot_DF1201S.h>

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

enum GagState { WELCOME, WARMUP, READY_WAIT, SENSING, ANALIZING, ANALYSIS_HOLD, BUSTED, SHUTDOWN };
GagState currentState = WELCOME;

unsigned long timer = 0, volTimer = 0, volDisplayTimer = 0, lastDebounceTime = 0;
bool lastButtonState = HIGH, buttonStableState = HIGH, showVolBriefly = false;
int currentVol = -1;

void setup() {
  pinMode(BLUE_LED, OUTPUT); pinMode(RED_LED, OUTPUT);
  pinMode(RGB_RED, OUTPUT); pinMode(RGB_GREEN, OUTPUT); pinMode(RGB_BLUE, OUTPUT);
  pinMode(START_BUTTON, INPUT_PULLUP);

  Wire.begin(8, 9);
  Wire.setClock(400000); // FIX: Set I2C to 400kHz for cleaner data transfer
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextColor(WHITE, BLACK); // FIX: Opaque text erases artifacts behind it
  
  dfSerial.begin(115200, SERIAL_8N1, 18, 17);
  if (dfplayer.begin(dfSerial)) {
    dfplayer.setPrompt(false);
    dfplayer.switchFunction(dfplayer.MUSIC);
    dfplayer.setPlayMode(dfplayer.SINGLECYCLE);
  }
  randomSeed(analogRead(0)); 
  timer = millis();
}

void loop() {
  bool clicked = checkButtonClick();
  handleVolume();

  if (showVolBriefly && (millis() - volDisplayTimer < 1500)) {
    displayVolume();
  } else {
    runStateMachine(clicked);
  }
}

void runStateMachine(bool clicked) {
  unsigned long elapsed = millis() - timer;

  switch (currentState) {
    case WELCOME:
      display.clearDisplay();
      display.setCursor(10, 15); display.setTextSize(1); display.print("DRAGON BREATH 5000");
      display.setCursor(22, 35); display.print("patent pending");
      display.display();
      if (elapsed > 5000) { timer = millis(); currentState = WARMUP; }
      break;

    case WARMUP:
      setRGB(0, 50, 0); 
      display.clearDisplay();
      display.setCursor(20, 10); display.setTextSize(1); display.print("WARMING UP....");
      display.setCursor(12, 45); display.print("please be patient");
      display.display();
      if (elapsed > 5000) { timer = millis(); currentState = READY_WAIT; }
      break;

    case READY_WAIT:
      setRGB(0, 255, 0);
      display.clearDisplay();
      display.setTextSize(3);
      display.setCursor(10, 0);  display.print("SYSTEM");
      display.setCursor(15, 40); display.print("READY");
      display.display();
      if (clicked) {
        dfplayer.playSpecFile("/0002.mp3"); 
        timer = millis();
        currentState = SENSING;
      }
      break;

    case SENSING: {
      int currentBlinkRate = map(elapsed, 0, 14000, 400, 50); 
      setRGB(0, (millis() / currentBlinkRate) % 2 * 255, 0); 
      display.clearDisplay();
      if ((millis() / 300) % 2 == 0) { 
        display.setTextSize(3); display.setCursor(0, 5); display.print("SENSING");
      }
      display.drawRect(10, 45, 108, 12, WHITE);
      display.fillRect(10, 45, map(elapsed, 0, 14000, 0, 108), 12, WHITE);
      display.display();
      if (elapsed > 14000) { timer = millis(); currentState = ANALIZING; }
    } break;

    case ANALIZING:
      setRGB(255, 100, 0);
      display.clearDisplay();
      display.setCursor(30, 10); display.setTextSize(1); display.print("ANALYSIS....");
      display.setCursor(18, 45); display.print("results may vary");
      display.display();
      if (elapsed > 4000) { timer = millis(); currentState = ANALYSIS_HOLD; }
      break;

    case ANALYSIS_HOLD: {
      static bool barsVisible = false;
      if (elapsed < 5000) {
        display.clearDisplay();
        display.setCursor(0, 5);  display.setTextSize(3); display.print("RESULTS....");
        display.setCursor(20, 45); display.setTextSize(1); display.print("not looking great");
        display.display();
        barsVisible = false;
      } else {
        if (!barsVisible) {
          drawDrugTestBars(); 
          barsVisible = true;
        }
        if (clicked) {
          barsVisible = false; 
          triggerBustedSequence();
        }
      }
    } break;

    case BUSTED:
      runBustedStrobe();
      if (clicked) { currentState = SHUTDOWN; timer = millis(); }
      break;

    case SHUTDOWN:
      dfplayer.pause();
      digitalWrite(BLUE_LED, LOW); digitalWrite(RED_LED, LOW);
      setRGB(0, 0, 0);
      display.clearDisplay();
      display.setCursor(35, 25); display.setTextSize(2); display.print("RESET");
      display.display();
      if (elapsed > 2000) currentState = READY_WAIT;
      break;
  }
}

void drawDrugTestBars() {
  display.clearDisplay();
  const char* labels[] = {"EtOH", "ThC", "CoKe", "MetH"};
  int barX[] = {8, 41, 75, 108};
  for (int i = 0; i < 4; i++) {
    int val = (i == 0) ? 46 : random(5, 30); 
    display.drawRect(barX[i], 0, 10, 50, WHITE);
    display.fillRect(barX[i], 50 - val, 10, val, WHITE);
    display.setTextSize(1); 
    display.setCursor(barX[i] - 2, 55);
    display.print(labels[i]);
  }
  display.display();
}

void triggerBustedSequence() {
  dfplayer.pause();
  display.clearDisplay(); display.display();
  delay(500); // Snappy 0.5s wait
  dfplayer.setVol(15); 
  dfplayer.playSpecFile("/0003.mp3"); 
  currentState = BUSTED;
}

void runBustedStrobe() {
  bool toggle = (millis() / 100) % 2;
  digitalWrite(RED_LED, toggle);
  digitalWrite(BLUE_LED, !toggle);
  setRGB(toggle ? 255 : 0, 0, toggle ? 0 : 255);
  display.clearDisplay();
  display.setCursor(0, 5);  display.setTextSize(3); display.print("BUSTED!");
  display.setCursor(10, 42); display.setTextSize(1); display.print("you holding out...");
  display.setCursor(45, 54); display.print("again!"); 
  display.display();
}

void handleVolume() {
  if (millis() - volTimer > 200) {
    int newVol = map(analogRead(POT_PIN), 0, 4095, 0, 30);
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
  display.setCursor(45, 10); display.setTextSize(1); display.print("VOLUME");
  display.setCursor(50, 30); display.setTextSize(3); display.print(currentVol);
  display.display();
}

bool checkButtonClick() {
  bool reading = digitalRead(START_BUTTON);
  if (reading != lastButtonState) lastDebounceTime = millis();
  if ((millis() - lastDebounceTime) > 50) {
    if (buttonStableState == HIGH && reading == LOW) {
      buttonStableState = LOW; lastButtonState = reading;
      return true;
    }
    buttonStableState = reading;
  }
  lastButtonState = reading;
  return false;
}

void setRGB(int r, int g, int b) { 
  analogWrite(RGB_RED, r); analogWrite(RGB_GREEN, g); analogWrite(RGB_BLUE, b); 
}
