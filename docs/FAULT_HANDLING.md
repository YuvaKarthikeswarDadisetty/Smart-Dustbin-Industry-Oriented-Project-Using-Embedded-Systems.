# Smart Dustbin - Fault Handling & System Reliability

## Phase 10

Phase 10 introduces fault detection and safe-state handling for ultrasonic sensor failures.

---

## 1. Objectives

- Detect invalid ultrasonic readings.
- Detect sensor timeout conditions.
- Prevent false lid activation.
- Force the lid to a safe closed position during faults.
- Prevent false buzzer activation.
- Maintain a known LED state.
- Continue system operation after sensor recovery.

---

## 2. Fault Conditions

### Hand Sensor Fault

If the hand-detection HC-SR04 does not return a valid echo:

```text
Hand Distance : INVALID
Hand Status   : SENSOR FAULT