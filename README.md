🛰️ Arduino-Based Interactive Radar System
A real-time embedded system project that utilizes an ultrasonic sensor, a servo motor, and an OLED display to simulate a radar scanning system. Developed as part of a Microprocessors / Embedded Systems course.

🛠️ Hardware Components
Microcontroller: 
Arduino UNO R4 WiFi

Sensors: HC-SR04 Ultrasonic Sensor

Actuators: SG90 Micro Servo Motor

Output: 0.96" SSD1306 OLED Display (I2C)

Alert System: Active Buzzer

🚀 Features
Real-Time Data Acquisition: Scans distance data within a calibrated range.

Dynamic Visualization: Live radar sweep and object detection points on OLED.

Audible Warnings: Proximity-based alert frequency via buzzer.

Optimized Scanning: Calibrated between 45° and 135° to maintain mechanical stability and prevent wire entanglement.

🔧 Installation & Usage
Connect the hardware according to the circuit diagram (available in /docs).

Install the necessary libraries: Adafruit_GFX, Adafruit_SSD1306, and Servo.

Upload Radar_Master_Code.ino to your Arduino.

Open the Serial Monitor (9600 baud) or Serial Plotter to view live data streams.

💡 Engineering Decisions & Challenges
Jumper Wire Management: Used Female-to-Male extensions to provide enough "slack" for the moving sensor, preventing mechanical resistance.

Sweep Optimization: Reduced the sweep angle from 180° to 90° (45°-135°) to avoid hardware collisions and increase data density in the primary field of view.

Noise Filtering: Implemented a software timeout for the pulseIn() function to ignore ghost signals and out-of-range data.
