# DragonBreath-S3
An experimental, high-suspense breath analysis gag built on the ESP32-S3. Features a 15-second psychological 'sensing' phase, a ramping RGB 'thermal' mesh glow, and a high-volume 'Busted' siren trigger. Designed for open-frame 'Roll Cage' hardware builds.
- Audio Files: Copyright is real. Don't host Jaws/Bad Boys on GitHub or you'll get a DMCA. Source them yourself!
1. **Power Up:** The OLED shows `warming up:` (5s) while the sensor mesh glows **Solid Green**.
2. **The Stop:** When the screen says `system ready:`, tell the target you need a sample.
3. **Click 1 (The Trigger):** before they blow,which could be a minute , click the trigger. 
   - **0002.mp3 (Jaws)** starts with a +6 volume boost.
   - OLED flashes `Sensing:` with a 10s **Progress Bar**.
   - The **Green LED** blinks faster and faster as the bar hits 100%.
4. **The Sweat:** The bar hits 100% and displays `Analysis:`. The Jaws music keeps looping. Wait at least 4 seconds while looking concerned at the screen.
5. **Click 2 (The Bust):** When the tension is maxed, click again.
   - Jaws music **STOPS**. **0003.mp3 (Bad Boys/Siren)** blasts.
   - OLED screams `BUSTED! u holding out?`.
   - **Red & Blue Strobes** go into full "Patrol Car" mode.
6. **Click 3 (Reset):** Click to clear the screen and LEDs for the next "mark."
 

### 🎭 The Performance Guide: How to "Bust" a Dragon 🎭

This device isn't just a prop; it's a social engineering tool. To get the best reaction at a party or event, follow the **Slow-Burn** method:

1. **The Setup**  
   Approach your target with the device powered on. The OLED should display `system ready:` and the green mesh should be glowing steady.  
   *Lines to use:* "Excuse me, we've had reports of 'Dragon Breath' in this immediate area. I'm going to need a sample, please."

2. **The Blow**  
   Position the "thick straw" near their mouth and have them blow. **Click the trigger.**  
   The **Jaws theme** will start low. As the progress bar fills over the 10-second window, keep your eyes glued to the screen. Act like you are reading serious, fluctuating data.

3. **The 'Nervous' Look**  
   When the bar hits 100% and the screen changes to `Analysis:`, **DO NOT CLICK YET.**  
   Let the music loop and the yellow light build tension. Squint at the screen, look up at the target with a concerned face, then look back at the device.  
   *Lines to use:* "Uh oh... the sulfuric levels are spiking. I've never seen a reading this high."

4. **The Drop**  
   Wait for them to get nervous or ask, "What does it say?!"  
   **Click the trigger again.** The suspense music cuts out, the **Bad Boys** siren blasts, and the **Red/Blue strobes** turn the entire room into a high-stakes crime scene!

-------------
 ## 📂 Stealth Mode & Troubleshooting

### **The "Dual-Mode" Feature**
1. **Party Mode (Solid Onboard LED):** Use the DFPlayer Pro's 128MB space for extra MP3s (0004.mp3, etc.). Use the **physical buttons** on the player board to play music. While the LED is **Solid**, you’re just a guy with a glowing speaker.
2. **Gag Mode (Flashing Onboard LED):** The moment you pull the **Grip Trigger**, the ESP32-S3 hijacks the audio via Serial. The onboard LED will **Flash**, signaling the S3 has taken control for the Jaws sequence.
------------------------------
## 🛠️ The "Mock" MQ-3 Hardware Hack
⚠️**WARNING: THE SENSOR IS FAKE.** Do not connect the heater pins!⚠️

- **The Look:** Pull the silver metal mesh hood off a cheap MQ-3 sensor.
- **The LED:** Place your **4-pin RGB LED** inside that mesh hood. This provides the "Thermal Glow" and the "Police Strobe."
- **The Wiring:** 
  -⚠️ **DO NOT** connect the **H-H (Heater)** pins to power (it will melt the plastic!).
  - ⚠️**DO NOT** connect the **A-B (Sensor)** pins to the ESP32. 

---------------------------------

    ### 🛠 Hardware Wiring:  ###


| Component       | ESP32-S3 Pin | Notes                          |
|-----------------|--------------|--------------------------------|
| **OLED SDA**    | GPIO 8       | I2C Data                       |
| **OLED SCL**    | GPIO 9       | I2C Clock                      |
| **Trigger**     | GPIO 4       | Button to GND (Internal Pullup)|
| **Volume Pot**  | GPIO 6       | Center Pin (Wiper)             |
| **RGB Red**     | GPIO 7       | Mesh Glow (220Ω Resistor)      |
| **RGB Green**   | GPIO 5       | Mesh Glow (220Ω Resistor)      |
| **RGB Blue**    | GPIO 40      | Mesh Glow (220Ω Resistor)      |
| **Blue LED**    | GPIO 1       | External Strobe                |
| **Red LED**     | GPIO 2       | External Strobe                |
| **Player TX**   | GPIO 17      | Connect to DFPlayer RX         |
| **Player RX**   | GPIO 18      | Connect to DFPlayer TX         |
-----------------------------------------------------------------
![101](https://github.com/user-attachments/assets/e265b91a-d49e-492a-8d20-d897ed05ba1f)
![102](https://github.com/user-attachments/assets/7025f21a-c6b0-46c9-9ce0-24d9c3da5f6b)
------------------------------------------------------------------------
### 📂 DFPlayer Pro Audio Setup
1. Connect the DFPlayer Pro to your PC via **USB-C**.
2. Format the onboard 128MB storage to **FAT32** if you have issues with "ghost" files.
3. Drag and drop your audio files into the **root** folder.
4. Name them exactly `/0002.mp3` (Suspense) and `/0003.mp3` (Siren).
5. 📂 Audio Upload (The "Pain-Free" Way)
1. Plug the **DFPlayer Pro** into your PC via **USB-C** (appears as a 128MB drive).
2. **Mac Users:** Run `dot_clean /Volumes/DFPlayer` in Terminal after copying to prevent "clicking" bugs.
3. Name files: `/0002.mp3` (Jaws) and `/0003.mp3` (Bad Boys)

--------------------------------------------------------------------------
## ⚠️ Lessons Learned (So you don't have to)
This project was a massive pain to develop. Here’s what I learned the hard way:
- **Serial Speed:** The DFPlayer PRO version needs 115200 baud, not 9600 like the Mini.
- **S3 Pins:** Don't use GPIO 10 for the button; it's unstable. Use GPIO 4.
- **Compiler Errors:** If you get "jump to case label," wrap your switch-case logic in curly braces `{}`.
- **Audio Files:** Copyright is real. Don't host Jaws/Bad Boys on GitHub or you'll get a DMCA. Source them yourself!

⚠️ "Final Warning: If you start smelling actual sulfur, the device has either achieved sentience or you've found a real dragon. Either way, you won't have to worry about breathalyzing anyone ever again. DragonBreath-S3: Use at your own risk."

