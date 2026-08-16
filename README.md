# 🗑️ Smart Dustbin – Industry-Oriented Project Using Embedded Systems

An Arduino-based smart dustbin that detects a user's hand, automatically controls the lid, monitors waste level, provides LED indication, activates a critical-level buzzer, and handles invalid sensor readings through safe-mode logic.

## 🎯 Objectives

- Contactless lid operation
- Automatic waste-level monitoring
- Fill-percentage estimation
- Bin-status classification
- LED and buzzer indication
- Sensor-fault detection
- Safe-mode handling
- Wokwi-based validation

## ⚙️ Features

### Contactless Lid

```text
Hand Detected → Servo 90° → Lid Open
```

### Automatic Closing

```text
Lid Open → Timer → Closing → Servo 0° → Lid Closed
```

### Waste Levels

| Fill | Status | Indicator |
|---|---|---|
| 0% – <50% | NORMAL | Green |
| 50% – <75% | MEDIUM | Green |
| 75% – <90% | HIGH | Red |
| ≥90% | CRITICAL / FULL | Red + Buzzer |

### Buzzer

```cpp
tone(BUZZER_PIN, 2000);
```

Non-critical states use:

```cpp
noTone(BUZZER_PIN);
```

### Fault Handling

```text
!!! SENSOR FAULT !!!

Sensor  : HAND SENSOR
Reading : INVALID / TIMEOUT
Action  : SAFE MODE
```

## 🧩 System Architecture

```text
                 ┌──────────────────┐
                 │    Arduino UNO   │
                 │  Main Controller │
                 └────────┬─────────┘
                          │
             ┌────────────┼────────────┐
             ▼            ▼            ▼
        Hand HC-SR04  Waste HC-SR04  SG90 Servo
             │            │            │
             └────────────┼────────────┘
                          ▼
                    Control Logic
                          │
              ┌───────────┼───────────┐
              ▼           ▼           ▼
          Green LED    Red LED     Buzzer
```

## 🔌 Hardware

| Component | Qty | Purpose |
|---|---:|---|
| Arduino UNO | 1 | Controller |
| HC-SR04 | 2 | Hand + waste sensing |
| SG90 Servo | 1 | Lid control |
| Green LED | 1 | Normal status |
| Red LED | 1 | High/critical status |
| Buzzer | 1 | Critical alert |

## 💻 Tools

- Arduino C/C++
- Wokwi
- Visual Studio Code
- Git
- GitHub

## 📁 Project Structure

```text
Smart Dustbin – Industry-Oriented Project Using Embedded Systems/
├── arduino_code/
├── docs/
├── screenshots/
├── simulation/
│   └── final_wokwi_link.txt
├── .gitignore
└── README.md
```

## 🧪 Testing

| Test | Result |
|---|---|
| Normal bin | ✅ PASS |
| Medium fill | ✅ PASS |
| High fill | ✅ PASS |
| Critical/full | ✅ PASS |
| Green LED | ✅ PASS |
| Red LED | ✅ PASS |
| Buzzer | ✅ PASS |
| Hand detection | ✅ PASS |
| Lid opening | ✅ PASS |
| Servo 90° | ✅ PASS |
| Lid closing | ✅ PASS |
| Servo 0° | ✅ PASS |
| Sensor fault | ✅ PASS |
| Safe mode | ✅ PASS |
| Integrated operation | ✅ PASS |

## 🚀 Development Phases

| Phase | Status |
|---|---|
| 1–12 | ✅ COMPLETED |
| 13 – Documentation & Release | ✅ COMPLETED |

## 🌐 Wokwi Simulation

**Open the complete Smart Dustbin simulation:**

https://wokwi.com/projects/472201675787684865

## 🐙 GitHub

https://github.com/YuvaKarthikeswarDadisetty/Smart-Dustbin-Industry-Oriented-Project-Using-Embedded-Systems.

## 🔮 Future Improvements

- ESP32 / IoT connectivity
- Cloud monitoring
- Mobile application
- Remote notifications
- Waste-level history
- Data analytics
- ML-based waste prediction
- Real-world hardware deployment

## 🎓 Skills Demonstrated

Embedded Systems • Arduino • C/C++ • Ultrasonic Sensors • Servo Control • Digital I/O • PWM • Automation • Fault Handling • Wokwi • Git • GitHub • Testing • Debugging • Technical Documentation

## 👨‍💻 Project Status

### ✅ FUNCTIONALLY COMPLETED

The Smart Dustbin has been validated through Wokwi simulation and all major functional requirements have passed testing.

## 📄 License

This project is intended for educational, portfolio, and demonstration purposes.
