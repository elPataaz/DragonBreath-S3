# DragonBreath-S3
An experimental, high-suspense breath analysis gag built on the ESP32-S3. Features a 15-second psychological 'sensing' phase, a ramping RGB 'thermal' mesh glow, and a high-volume 'Busted' siren trigger. Designed for open-frame 'Roll Cage' hardware builds.

### 🛠 Hardware Wiring (The Roll-Cage Map)


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
