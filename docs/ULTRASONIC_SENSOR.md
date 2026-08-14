# Smart Dustbin – Ultrasonic Sensor Interfacing

## Phase 4

Phase 4 implements ultrasonic sensor interfacing for:

1. Hand/object detection
2. Waste-level detection

## Sensor 1

HC-SR04

TRIG → D2
ECHO → D3

Purpose:
Hand/object detection.

Threshold:
20 cm

## Sensor 2

HC-SR04

TRIG → D4
ECHO → D5

Purpose:
Waste-level detection.

Prototype bin height:
30 cm

## Distance Formula

Distance = Duration × 0.0343 / 2

## Fill Formula

Fill Percentage =
((Bin Height - Measured Distance) / Bin Height) × 100

## Test Results

| Test | Input | Expected |
|---|---:|---|
| Hand absent | 30 cm | Not detected |
| Hand detected | 15 cm | Detected |
| Empty bin | 30 cm | 0% |
| 25% bin | 22.5 cm | 25% |
| 50% bin | 15 cm | 50% |
| 75% bin | 7.5 cm | 75% |
| Critical | 3 cm | 90% |

## Fault Handling

Invalid or timeout sensor readings are reported through the Serial Monitor.

## Phase 4 Status

Completed after successful simulation and sensor testing.