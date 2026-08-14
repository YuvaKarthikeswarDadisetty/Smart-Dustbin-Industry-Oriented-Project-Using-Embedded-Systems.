# Smart Dustbin – System Specification

## 1. Project Information

### Project Title

**Smart Dustbin – Industry-Oriented Embedded System**

### Project Type

Embedded Systems / Automation Project

### Project Version

Version 1.0 – Student Prototype

### Development Approach

The project will be developed in phases using both:

* Virtual simulation
* Real hardware implementation

### Primary Simulation Platform

Wokwi

---

# 2. System Objective

The objective of this project is to develop an automated smart dustbin capable of:

1. Detecting a hand or object approaching the dustbin.
2. Automatically opening the dustbin lid.
3. Automatically closing the lid after a predefined time.
4. Measuring the amount of waste inside the dustbin.
5. Calculating the approximate bin fill percentage.
6. Indicating the bin status using LEDs.
7. Activating an audible alert when the bin reaches a critical fill level.
8. Demonstrating embedded-system concepts including sensing, processing, actuation, GPIO control, PWM, threshold logic, and sensor-based automation.

---

# 3. Controller

## Arduino UNO

The Arduino UNO is the main microcontroller used in Version 1 of the project.

### Responsibilities

The Arduino UNO will:

* Read the hand-detection ultrasonic sensor.
* Read the waste-level ultrasonic sensor.
* Calculate distances.
* Detect an approaching hand/object.
* Control the servo motor.
* Calculate the bin fill percentage.
* Control the green LED.
* Control the red LED.
* Control the buzzer.
* Send system information to the Serial Monitor.

### Reason for Selection

Arduino UNO was selected because it:

* Is beginner-friendly.
* Provides sufficient GPIO pins.
* Supports servo control.
* Supports HC-SR04 interfacing.
* Is easy to debug.
* Is supported by Wokwi.
* Is suitable for educational embedded-system development.
* Allows easy migration to more advanced platforms later.

---

# 4. Sensors

The system uses two HC-SR04 ultrasonic sensors.

## Sensor 1 – Hand/Object Detection

### Purpose

Detects whether a person's hand or another object is approaching the dustbin.

### Function

The sensor measures the distance between the sensor and the approaching object.

### Decision

If the measured distance is less than or equal to the configured hand-detection threshold:

```text
Distance <= 20 cm
```

the system considers an object/hand to be detected.

### Result

The Arduino commands the servo motor to open the lid.

---

## Sensor 2 – Waste-Level Detection

### Purpose

Measures the distance between the top of the dustbin and the waste.

### Function

The sensor is mounted at the upper portion of the dustbin and points downward toward the waste.

### Measurement

A larger measured distance indicates less waste.

A smaller measured distance indicates more waste.

### Result

The Arduino uses the measured distance to calculate the approximate bin fill percentage.

---

# 5. Servo Motor

## SG90 Micro Servo

The SG90 servo motor is used to control the dustbin lid.

### Purpose

Provide automatic mechanical lid opening and closing.

### Initial Servo Positions

```text
0°  → Lid Closed

90° → Lid Open
```

These values are initial prototype values and may be calibrated during the physical hardware phase depending on the mechanical construction of the lid.

### Initial Lid Timing

```text
Lid Open Duration = 3 seconds
```

### Operating Sequence

```text
Hand detected
      ↓
Servo moves to 90°
      ↓
Lid opens
      ↓
Approximately 3 seconds
      ↓
Servo moves to 0°
      ↓
Lid closes
```

---

# 6. LEDs

The project uses two status LEDs.

## Green LED

### Purpose

Indicates normal bin operation.

### Condition

```text
Bin Fill < 90%
```

### Output

```text
Green LED = ON
Red LED = OFF
Buzzer = OFF
```

---

## Red LED

### Purpose

Indicates a critical/full bin condition.

### Condition

```text
Bin Fill >= 90%
```

### Output

```text
Green LED = OFF
Red LED = ON
Buzzer = ON
```

---

# 7. LED Resistors

Each LED must use a current-limiting resistor.

### Resistor Value

```text
220Ω
```

### Connection

```text
Arduino GPIO
     ↓
  220Ω Resistor
     ↓
     LED
     ↓
    GND
```

A resistor must not be omitted when connecting the physical LEDs.

---

# 8. Buzzer

## Active Buzzer

The active buzzer provides an audible warning when the dustbin reaches the critical fill level.

### Normal Condition

```text
Buzzer = OFF
```

### Critical Condition

```text
Bin Fill >= 90%

Buzzer = ON
```

The buzzer provides an additional alert alongside the red LED.

---

# 9. Final Pin Mapping

The following Arduino UNO pin mapping is the official Version 1 pin assignment.

| Arduino UNO Pin | Component         | Function             |
| --------------- | ----------------- | -------------------- |
| D2              | HC-SR04 #1 TRIG   | Hand sensor trigger  |
| D3              | HC-SR04 #1 ECHO   | Hand sensor echo     |
| D4              | HC-SR04 #2 TRIG   | Level sensor trigger |
| D5              | HC-SR04 #2 ECHO   | Level sensor echo    |
| D6              | Green LED         | Normal status        |
| D7              | Red LED           | Critical/full status |
| D8              | Buzzer            | Full-bin alert       |
| D9              | SG90 Servo Signal | Lid control          |

### Final Pin Map

```text
D2 → Hand TRIG
D3 → Hand ECHO

D4 → Level TRIG
D5 → Level ECHO

D6 → Green LED
D7 → Red LED
D8 → Buzzer
D9 → Servo
```

This pin mapping should be maintained throughout the project unless a hardware or simulation issue requires a documented change.

---

# 10. Bin Dimensions

For the initial prototype and simulation, the following bin dimension is assumed:

```text
Prototype Bin Height = 30 cm
```

This value is used for the initial fill-percentage calculation.

### Important

For real hardware implementation, the actual usable bin height should be measured and calibrated.

The 30 cm value is therefore a prototype reference rather than a fixed production specification.

---

# 11. Hand Detection Threshold

The initial hand/object detection threshold is:

```text
20 cm
```

### Logic

```text
Distance > 20 cm
    ↓
No hand/object detected
    ↓
Lid remains closed
```

```text
Distance <= 20 cm
    ↓
Hand/object detected
    ↓
Open lid
```

This threshold can be adjusted during testing if false detections occur.

---

# 12. Bin Fill Thresholds

Using the 30 cm prototype bin height:

| Approximate Fill |    Sensor Distance | Status          |
| ---------------: | -----------------: | --------------- |
|               0% |              30 cm | Empty / Normal  |
|              25% |            22.5 cm | Normal          |
|              50% |              15 cm | Medium          |
|              75% |             7.5 cm | High            |
|              90% |               3 cm | Critical / Full |
|             100% | Approximately 0 cm | Full            |

These are theoretical prototype reference values.

Actual physical readings will be calibrated during the testing phase.

---

# 13. Full-Bin Threshold

The initial critical threshold is:

```text
90%
```

### Normal State

```text
Fill < 90%
```

Output:

```text
Green LED = ON
Red LED = OFF
Buzzer = OFF
```

### Critical State

```text
Fill >= 90%
```

Output:

```text
Green LED = OFF
Red LED = ON
Buzzer = ON
```

---

# 14. Bin-Level Calculation

The initial fill-level calculation is:

```text
Fill Level = Bin Height - Measured Distance
```

The fill percentage is:

```text
Fill Percentage =
(Fill Level / Bin Height) × 100
```

Therefore:

```text
Fill Percentage =
((Bin Height - Measured Distance) / Bin Height) × 100
```

### Example

Given:

```text
Bin Height = 30 cm
Measured Distance = 15 cm
```

Then:

```text
Fill Level = 30 - 15
           = 15 cm
```

Therefore:

```text
Fill Percentage = (15 / 30) × 100
                = 50%
```

---

# 15. Power Requirements

## Arduino UNO

```text
Operating Supply:
5V logic/system supply
```

---

## HC-SR04 Sensor #1

```text
VCC → Arduino 5V
GND → Arduino GND
```

---

## HC-SR04 Sensor #2

```text
VCC → Arduino 5V
GND → Arduino GND
```

---

## Green LED

```text
Arduino D6
    ↓
220Ω resistor
    ↓
Green LED
    ↓
GND
```

---

## Red LED

```text
Arduino D7
    ↓
220Ω resistor
    ↓
Red LED
    ↓
GND
```

---

## Buzzer

```text
Arduino D8 → Buzzer Positive
Arduino GND → Buzzer Negative
```

---

## Servo

```text
Servo Signal → Arduino D9
Servo VCC    → 5V supply
Servo GND    → Common GND
```

### Physical Hardware Recommendation

For the real hardware implementation, the servo should preferably use a suitable external 5V supply because servo current demand can cause voltage drops or Arduino resets.

The external servo supply and Arduino must share a common ground.

```text
External 5V Supply
       │
       ├── Servo VCC
       │
       └── GND ───── Arduino GND
```

---

# 16. System Inputs

The system has two primary sensor inputs.

## Input 1 – Hand/Object Distance

```text
Source:
HC-SR04 Sensor #1

Input Type:
Distance measurement

Purpose:
Detect an approaching hand/object.

Threshold:
20 cm
```

---

## Input 2 – Waste Distance

```text
Source:
HC-SR04 Sensor #2

Input Type:
Distance measurement

Purpose:
Determine approximate waste level.

Reference:
30 cm prototype bin height.
```

---

# 17. System Outputs

The system has five primary outputs.

## Output 1 – Servo Motor

```text
Device:
SG90 Servo

Purpose:
Automatic lid control.

0°:
Closed

90°:
Open
```

---

## Output 2 – Green LED

```text
Device:
Green LED

Purpose:
Normal bin status.
```

---

## Output 3 – Red LED

```text
Device:
Red LED

Purpose:
Critical/full bin status.
```

---

## Output 4 – Buzzer

```text
Device:
Active Buzzer

Purpose:
Audible critical/full-bin alert.
```

---

## Output 5 – Serial Monitor

```text
Interface:
Arduino Serial

Baud Rate:
9600

Purpose:
Debugging and system diagnostics.
```

The Serial Monitor will display:

* Hand distance
* Waste distance
* Fill percentage
* Bin status
* Lid status
* Invalid sensor readings

---

# 18. System Workflow

The complete system workflow is:

```text
START
  ↓
Initialize Arduino
  ↓
Initialize Sensors
  ↓
Initialize Servo
  ↓
Initialize LEDs
  ↓
Initialize Buzzer
  ↓
Read Hand Sensor
  ↓
Hand/Object Detected?
  │
  ├── NO ────────────────┐
  │                      │
  └── YES                │
       ↓                 │
   Open Lid              │
       ↓                 │
   Wait ~3 seconds        │
       ↓                 │
   Close Lid              │
       ↓                 │
       └─────────┬────────┘
                 ↓
        Read Level Sensor
                 ↓
        Calculate Fill %
                 ↓
          Fill >= 90%?
             │
       ┌─────┴─────┐
       │           │
      NO          YES
       │           │
       ▼           ▼
 Green LED       Red LED
    ON             ON
       │           │
 Buzzer OFF     Buzzer ON
       │           │
       └─────┬─────┘
             ↓
            LOOP
```

---

# 19. Control Logic

## Lid Control

```text
IF hand distance <= 20 cm
    Open lid
    Wait approximately 3 seconds
    Close lid
ELSE
    Keep lid closed
```

---

## Bin-Level Control

```text
Read waste distance

Calculate:

Fill % =
((30 - distance) / 30) × 100

IF Fill % >= 90
    Red LED ON
    Green LED OFF
    Buzzer ON
ELSE
    Green LED ON
    Red LED OFF
    Buzzer OFF
```

---

# 20. Sensor Validation

The firmware should validate ultrasonic sensor readings.

An invalid measurement may occur because of:

* Sensor timeout
* No echo
* Out-of-range measurement
* Temporary interference

The system should not blindly treat an invalid reading as a full bin.

The Serial Monitor should report an invalid sensor condition when appropriate.

---

# 21. Initial System States

At startup:

```text
Lid:
CLOSED

Green LED:
ON

Red LED:
OFF

Buzzer:
OFF

Fill Status:
NORMAL
```

The system then continuously monitors both sensors.

---

# 22. Version 1 Feature Scope

The following features are included in Version 1:

* Automatic lid opening
* Automatic lid closing
* Hand/object detection
* Waste-level detection
* Fill percentage calculation
* Green status LED
* Red status LED
* Buzzer alert
* Serial Monitor diagnostics
* Sensor validation
* Wokwi simulation
* Real hardware implementation

---

# 23. Features Not Included in Version 1

The following features are intentionally excluded from the initial implementation:

* Wi-Fi
* Cloud dashboard
* Mobile application
* MQTT
* GPS
* Multiple-bin networking
* Machine learning
* Predictive waste collection
* OLED/LCD

These may be considered future enhancements after the basic embedded system is completed and tested.

---

# 24. Future Version – ESP32/IoT Extension

A future Version 2 can use an ESP32.

Possible architecture:

```text
Smart Dustbin
      ↓
    ESP32
      ↓
    Wi-Fi
      ↓
 Cloud / MQTT
      ↓
 Dashboard
      ↓
Waste Collection Team
```

Possible Version 2 features:

* Remote bin monitoring
* Cloud dashboard
* Mobile notifications
* Multiple smart bins
* Collection scheduling
* Historical fill-level data
* IoT-based waste management

---

# 25. Final Version 1 Specification

| Parameter         | Final Specification  |
| ----------------- | -------------------- |
| Controller        | Arduino UNO          |
| Hand Sensor       | HC-SR04              |
| Level Sensor      | HC-SR04              |
| Servo             | SG90                 |
| Green LED         | Normal status        |
| Red LED           | Critical/full status |
| Buzzer            | Active buzzer        |
| Bin Height        | 30 cm prototype      |
| Hand Threshold    | 20 cm                |
| Full Threshold    | 90%                  |
| Lid Closed        | 0°                   |
| Lid Open          | 90°                  |
| Lid Open Duration | ~3 seconds           |
| Serial Baud Rate  | 9600                 |
| Simulation        | Wokwi                |
| Programming       | Arduino C/C++        |
| Version           | 1.0                  |

---

# 26. Final Pin Configuration

```text
Arduino UNO
│
├── D2 → HC-SR04 #1 TRIG
├── D3 → HC-SR04 #1 ECHO
│
├── D4 → HC-SR04 #2 TRIG
├── D5 → HC-SR04 #2 ECHO
│
├── D6 → Green LED
├── D7 → Red LED
├── D8 → Buzzer
└── D9 → SG90 Servo Signal
```

---

# 27. Phase 2 Completion Criteria

Phase 2 is considered complete when:

* Arduino UNO is selected.
* Two HC-SR04 sensors are selected.
* SG90 servo is selected.
* Green and red LEDs are selected.
* Buzzer is selected.
* Pin mapping is finalized.
* Bin dimensions are defined.
* Hand detection threshold is defined.
* Fill-level thresholds are defined.
* Servo positions are defined.
* Power requirements are documented.
* System inputs are documented.
* System outputs are documented.
* Version 1 architecture is finalized.

---

# 28. Phase 2 Status

**STATUS: COMPLETED**

The specifications in this document are the baseline specifications for the remaining development phases.

Any changes to the hardware, pins, thresholds, or architecture should be documented as a project change rather than being made silently.

---

## Next Phase

**Phase 3 – Circuit Design & Wiring**

The next phase will use this exact specification to create the complete Wokwi circuit and verify every connection before we begin individual sensor programming.
