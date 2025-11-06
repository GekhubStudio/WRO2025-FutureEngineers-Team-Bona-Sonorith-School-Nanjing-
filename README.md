# WRO2025-FutureEngineers-Team-Bona-Sonorith-School-Nanjing-
# WRO 2025 Future Engineers - Self-Driving Car Challenge
## Team Information
- **Team Number**: Team[XXX]  // 替换为你的团队编号
- **Team Members**: [Name 1, Student ID 1], [Name 2, Student ID 2], [Name 3, Student ID 3]  // 若2人则删除1人
- **Coach Name**: [Coach's Full Name]
- **Country/Region**: [Country/Region Name]
- **School/Institution**: [School/Institution Name] (Optional)
- **Competition Category**: WRO 2025 Future Engineers - Self-Driving Cars
- **Repository Creation Date**: [YYYY-MM-DD]

## Project Overview
This repository contains all technical materials for Team[XXX]'s participation in the WRO 2025 Future Engineers Self-Driving Car Challenge. Our self-driving vehicle is designed to complete two core challenges: the **Open Challenge** (3 laps on a track with random inner walls) and the **Obstacle Challenge** (3 laps with random traffic signs + parallel parking). 

The vehicle integrates computer vision, sensor fusion, and autonomous control to navigate the track without human intervention. The design focuses on reliability, efficiency, and compliance with WRO 2025 rules. All materials here are consistent with the hard copy documentation submitted for the competition.

## Core Modules & Electromechanical Connections
### 1. Sensor Module
- **Camera**: [Camera Model, e.g., Raspberry Pi Camera V2 with Wide-Angle Lens]  
  - Function: Lane detection, traffic sign (red/green pillars) recognition.  
  - Connection: Connected to [Controller Model, e.g., Raspberry Pi 4B] via CSI interface; data processed by `camera_detection.py` to output lane offset and sign type.  
- **Ultrasonic Sensors**: [Sensor Model, e.g., HC-SR04 x 2]  
  - Function: Measure distance to track walls/obstacles to avoid collisions.  
  - Connection: Linked to [Controller Model, e.g., Arduino UNO] via GPIO pins; data read by `ultrasonic_sensor.cpp` and sent to the main controller for real-time adjustment.  
- **IMU Sensor**: [Sensor Model, e.g., MPU6050]  
  - Function: Provide vehicle posture (angle, acceleration) to optimize steering stability.  
  - Connection: Connected to Arduino UNO via I2C; data fused with camera/ultrasonic data in `main_controller.py`.

### 2. Control Module
- **Steering Control**: [Servo Model, e.g., SG90 Servo Motor]  
  - Function: Adjust vehicle direction based on lane offset and path planning.  
  - Connection: Controlled by Arduino UNO via PWM signal; `steering_control.py` calculates the required servo angle (0-180°) and sends commands.  
- **Speed Control**: [DC Motor Model, e.g., 12V DC Motor with Gearbox]  
  - Function: Regulate vehicle speed (0-50 cm/s) to balance efficiency and stability.  
  - Connection: Driven by [Motor Driver, e.g., L298N] connected to Arduino UNO; `speed_control.cpp` adjusts motor voltage based on lap progress and obstacle status.  
- **Parking Control**:  
  - Function: Complete parallel parking in the Obstacle Challenge.  
  - Logic: After 3 laps, the vehicle uses ultrasonic sensors to locate the parking lot (magenta markers); `parking_control.ino` controls steering and speed to align with the parking area (20cm width, 1.5x vehicle length).

### 3. Decision Module
- **Path Planning**: `path_planning.py` uses a "waypoint following" algorithm. It generates real-time waypoints based on lane detection and adjusts the path when traffic signs are recognized (red pillar → stay right; green pillar → stay left).  
- **Obstacle Handling**: If ultrasonic sensors detect a collision risk (distance < 10cm), the module sends a "slow down" command to the speed control module; if traffic signs are passed incorrectly, it triggers a corrective maneuver (within 2 allowed reverse sections).

## Compilation & Upload Guide
### 1. Hardware Preparation
- **Controllers**: [List all controllers, e.g., Raspberry Pi 4B (SBC), Arduino UNO (SBM)]  
- **Software Tools**: [List tools, e.g., Python 3.9 (for Raspberry Pi), Arduino IDE 2.2.1 (for Arduino), Git 2.40.1 (for version control)]  
- **Wiring Check**: Refer to `Documentation/wiring_diagram.pdf` to ensure all components are connected correctly (e.g., servo PWM pin → Arduino D9, ultrasonic trig pin → Arduino D12).

### 2. Code Compilation (Arduino Part)
1. Open Arduino IDE and load the code in `/Code/Control/steering_control.py` (convert to .ino if needed) or `/Code/Main/main_controller.py` (for integrated control).  
2. Select the correct board: "Tools" → "Board" → "Arduino AVR Boards" → "Arduino UNO".  
3. Select the correct port: "Tools" → "Port" → [Your Port, e.g., COM3 (Windows) or /dev/ttyUSB0 (Linux)].  
4. Click "Verify" to compile the code. If no errors (0 warnings), proceed to upload.

### 3. Code Upload (Raspberry Pi Part)
1. Connect Raspberry Pi to the computer via SSH (IP address: [Raspberry Pi IP, e.g., 192.168.1.100]) or use a monitor/keyboard.  
2. Clone this repository to Raspberry Pi:  
   ```bash
   git clone https://github.com/[Your GitHub Username]/WRO2025-FutureEngineers-SelfDrivingCar-Team[XXX].git
