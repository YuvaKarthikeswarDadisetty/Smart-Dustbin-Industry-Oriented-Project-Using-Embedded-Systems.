# Smart Dustbin – Circuit Design

## Phase 3 – Circuit Design & Wiring

## 1. Objective

The objective of Phase 3 is to create and verify the complete Smart Dustbin circuit using Arduino UNO, two HC-SR04 ultrasonic sensors, an SG90 servo motor, two LEDs, and a buzzer.

The circuit is first implemented virtually using Wokwi.

---

## 2. Controller

Arduino UNO

---

## 3. Components

* Arduino UNO
* HC-SR04 Ultrasonic Sensor #1
* HC-SR04 Ultrasonic Sensor #2
* SG90 Servo Motor
* Green LED
* Red LED
* 220Ω resistor × 2
* Active Buzzer

---

## 4. Pin Configuration

| Arduino Pin | Component       | Function               |
| ----------- | --------------- | ---------------------- |
| D2          | HC-SR04 #1 TRIG | Hand detection trigger |
| D3          | HC-SR04 #1 ECHO | Hand detection echo    |
| D4          | HC-SR04 #2 TRIG | Waste-level trigger    |
| D5          | HC-SR04 #2 ECHO | Waste-level echo       |
| D6          | Green LED       | Normal status          |
| D7          | Red LED         | Full status            |
| D8          | Buzzer          | Full-bin alert         |
| D9          | SG90 Servo      | Lid control            |

---

## 5. Sensor 1 – Hand Detection

### HC-SR04 #1

```text
VCC  → Arduino 5V
GND  → Arduino GND
TRIG → Arduino D2
ECHO → Arduino D3
```

Purpose:

Detect an approaching hand or object.

Initial detection threshold:

```text
20 cm
```

---

## 6. Sensor 2 – Waste-Level Detection

### HC-SR04 #2

```text
VCC  → Arduino 5V
GND  → Arduino GND
TRIG → Arduino D4
ECHO → Arduino D5
```

Purpose:

Measure the distance from the top of the bin to the waste.

Prototype bin height:

```text
30 cm
```

---

## 7. Servo Motor

### SG90

```text
Signal → Arduino D9
VCC    → 5V
GND    → GND
```

Initial positions:

```text
0°  → Lid Closed
90° → Lid Open
```

Initial open duration:

```text
3 seconds
```

---

## 8. Green LED

```text
D6
 ↓
220Ω resistor
 ↓
Green LED
 ↓
GND
```

Purpose:

Indicate normal bin status.

---

## 9. Red LED

```text
D7
 ↓
220Ω resistor
 ↓
Red LED
 ↓
GND
```

Purpose:

Indicate critical/full bin status.

---

## 10. Buzzer

```text
D8 → Buzzer Positive
GND → Buzzer Negative
```

Purpose:

Provide an audible alert when the bin reaches the critical threshold.

---

## 11. Power

The ultrasonic sensors operate from the Arduino 5V supply in the prototype.

The servo uses a 5V supply.

For physical hardware, an external suitable 5V servo supply is recommended, with common ground between the servo supply and Arduino.

---

## 12. Circuit Architecture

```text
HC-SR04 #1
Hand Detection
      │
      ▼
    D2/D3
      │
      ▼
┌───────────────┐
│   Arduino     │
│      UNO      │
└───────────────┘
      ▲
      │
    D4/D5
      │
      ▲
HC-SR04 #2
Waste Level


Arduino D9
    │
    ▼
  Servo
    │
    ▼
Automatic Lid


Arduino D6
    │
    ▼
Green LED


Arduino D7
    │
    ▼
Red LED


Arduino D8
    │
    ▼
Buzzer
```

---

## 13. Phase 3 Verification

The following connections must be verified before proceeding:

* [ ] HC-SR04 #1 VCC connected
* [ ] HC-SR04 #1 GND connected
* [ ] HC-SR04 #1 TRIG connected to D2
* [ ] HC-SR04 #1 ECHO connected to D3
* [ ] HC-SR04 #2 VCC connected
* [ ] HC-SR04 #2 GND connected
* [ ] HC-SR04 #2 TRIG connected to D4
* [ ] HC-SR04 #2 ECHO connected to D5
* [ ] Green LED connected through 220Ω resistor
* [ ] Green LED connected to D6
* [ ] Red LED connected through 220Ω resistor
* [ ] Red LED connected to D7
* [ ] Buzzer connected to D8
* [ ] Servo signal connected to D9
* [ ] Servo VCC connected
* [ ] Servo GND connected

---

## 14. Phase 3 Status

Phase 3 is complete when all components are correctly wired in Wokwi and the circuit screenshot has been captured.

### Next Phase

Phase 4 – Ultrasonic Sensor Interfacing
