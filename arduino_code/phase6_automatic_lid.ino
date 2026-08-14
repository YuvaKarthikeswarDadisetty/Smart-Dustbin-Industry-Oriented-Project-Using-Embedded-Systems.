/*
 * ============================================================
 * Smart Dustbin - Phase 6
 * Automatic Lid Control
 * ============================================================
 *
 * HC-SR04 #1:
 * TRIG -> D2
 * ECHO -> D3
 *
 * SG90 Servo:
 * Signal -> D9
 *
 * Logic:
 *
 * Distance <= 20 cm
 *        ↓
 * Hand detected
 *        ↓
 * Servo -> 90°
 *        ↓
 * Lid OPEN
 *        ↓
 * Wait 3 seconds
 *        ↓
 * Servo -> 0°
 *        ↓
 * Lid CLOSED
 *
 * ============================================================
 */

#include <Servo.h>


// ------------------------------------------------------------
// Pin Configuration
// ------------------------------------------------------------

const int HAND_TRIG_PIN = 2;
const int HAND_ECHO_PIN = 3;

const int SERVO_PIN = 9;


// ------------------------------------------------------------
// Configuration
// ------------------------------------------------------------

const float HAND_DETECTION_THRESHOLD = 20.0;

const int CLOSED_ANGLE = 0;
const int OPEN_ANGLE = 90;

const unsigned long OPEN_DURATION = 3000;


// ------------------------------------------------------------
// Servo Object
// ------------------------------------------------------------

Servo lidServo;


// ------------------------------------------------------------
// Read HC-SR04 Distance
// ------------------------------------------------------------

float readDistance()
{
    digitalWrite(HAND_TRIG_PIN, LOW);
    delayMicroseconds(2);

    digitalWrite(HAND_TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(HAND_TRIG_PIN, LOW);

    unsigned long duration =
        pulseIn(
            HAND_ECHO_PIN,
            HIGH,
            30000
        );

    if (duration == 0)
    {
        return -1.0;
    }

    float distance =
        duration * 0.0343 / 2.0;

    if (distance < 2.0 ||
        distance > 400.0)
    {
        return -1.0;
    }

    return distance;
}


// ------------------------------------------------------------
// Open Lid
// ------------------------------------------------------------

void openLid()
{
    Serial.println();
    Serial.println(">>> HAND DETECTED <<<");

    Serial.println(
        "Lid Status : OPENING"
    );

    lidServo.write(OPEN_ANGLE);

    delay(500);

    Serial.println(
        "Lid Status : OPEN"
    );

    Serial.print(
        "Servo Angle : "
    );

    Serial.print(OPEN_ANGLE);

    Serial.println(" degrees");

    Serial.print(
        "Open Duration : "
    );

    Serial.print(
        OPEN_DURATION / 1000
    );

    Serial.println(" seconds");

    delay(OPEN_DURATION);
}


// ------------------------------------------------------------
// Close Lid
// ------------------------------------------------------------

void closeLid()
{
    Serial.println(
        "Lid Status : CLOSING"
    );

    lidServo.write(CLOSED_ANGLE);

    delay(500);

    Serial.println(
        "Lid Status : CLOSED"
    );

    Serial.print(
        "Servo Angle : "
    );

    Serial.print(CLOSED_ANGLE);

    Serial.println(" degrees");
}


// ------------------------------------------------------------
// Setup
// ------------------------------------------------------------

void setup()
{
    Serial.begin(9600);

    pinMode(
        HAND_TRIG_PIN,
        OUTPUT
    );

    pinMode(
        HAND_ECHO_PIN,
        INPUT
    );

    digitalWrite(
        HAND_TRIG_PIN,
        LOW
    );

    lidServo.attach(
        SERVO_PIN
    );

    // Start with lid closed
    lidServo.write(
        CLOSED_ANGLE
    );

    delay(1000);

    Serial.println();
    Serial.println(
        "======================================"
    );

    Serial.println(
        " SMART DUSTBIN - PHASE 6 TEST"
    );

    Serial.println(
        " AUTOMATIC LID CONTROL"
    );

    Serial.println(
        "======================================"
    );

    Serial.println();

    Serial.println(
        "Hand Sensor : HC-SR04"
    );

    Serial.println(
        "TRIG        : D2"
    );

    Serial.println(
        "ECHO        : D3"
    );

    Serial.println();

    Serial.println(
        "Servo       : SG90"
    );

    Serial.println(
        "Signal      : D9"
    );

    Serial.println();

    Serial.print(
        "Detection Threshold : "
    );

    Serial.print(
        HAND_DETECTION_THRESHOLD
    );

    Serial.println(" cm");

    Serial.print(
        "Closed Angle        : "
    );

    Serial.print(
        CLOSED_ANGLE
    );

    Serial.println(" degrees");

    Serial.print(
        "Open Angle          : "
    );

    Serial.print(
        OPEN_ANGLE
    );

    Serial.println(" degrees");

    Serial.println();

    Serial.println(
        "System Ready"
    );

    Serial.println(
        "Lid Status : CLOSED"
    );

    Serial.println();
}


// ------------------------------------------------------------
// Main Loop
// ------------------------------------------------------------

void loop()
{
    float handDistance =
        readDistance();


    // --------------------------------------------------------
    // Invalid Sensor Reading
    // --------------------------------------------------------

    if (handDistance < 0)
    {
        Serial.println(
            "Hand Sensor : INVALID READING"
        );

        delay(500);

        return;
    }


    // --------------------------------------------------------
    // Display Distance
    // --------------------------------------------------------

    Serial.print(
        "Hand Distance : "
    );

    Serial.print(
        handDistance
    );

    Serial.println(" cm");


    // --------------------------------------------------------
    // Hand Detection
    // --------------------------------------------------------

    if (
        handDistance <=
        HAND_DETECTION_THRESHOLD
    )
    {
        openLid();

        closeLid();

        Serial.println();
        Serial.println(
            "Waiting for next hand detection..."
        );

        delay(500);
    }
    else
    {
        Serial.println(
            "Hand Status   : NOT DETECTED"
        );

        Serial.println(
            "Lid Status    : CLOSED"
        );

        lidServo.write(
            CLOSED_ANGLE
        );

        delay(500);
    }
}