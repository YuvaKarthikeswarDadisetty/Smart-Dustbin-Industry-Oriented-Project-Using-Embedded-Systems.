/*
 * ============================================================
 * Smart Dustbin - Phase 10
 * Fault Handling & System Reliability
 * ============================================================
 *
 * Purpose:
 * - Detect invalid ultrasonic sensor readings
 * - Handle sensor timeout conditions safely
 * - Prevent false lid activation
 * - Maintain safe LED and buzzer states
 * - Keep the system running after sensor errors
 *
 * Hardware:
 * Arduino UNO
 * HC-SR04 #1 -> Hand Detection
 * HC-SR04 #2 -> Waste-Level Detection
 * SG90 Servo
 * Green LED
 * Red LED
 * Buzzer
 *
 * Pin Configuration:
 *
 * Hand Sensor:
 * TRIG -> D2
 * ECHO -> D3
 *
 * Waste Sensor:
 * TRIG -> D4
 * ECHO -> D5
 *
 * Green LED -> D6
 * Red LED   -> D7
 * Buzzer    -> D8
 * Servo     -> D9
 * ============================================================
 */

#include <Servo.h>

// ============================================================
// PIN CONFIGURATION
// ============================================================

const int HAND_TRIG_PIN  = 2;
const int HAND_ECHO_PIN  = 3;

const int LEVEL_TRIG_PIN = 4;
const int LEVEL_ECHO_PIN = 5;

const int GREEN_LED_PIN  = 6;
const int RED_LED_PIN    = 7;
const int BUZZER_PIN     = 8;

const int SERVO_PIN      = 9;

// ============================================================
// SYSTEM PARAMETERS
// ============================================================

const int CLOSED_ANGLE = 0;
const int OPEN_ANGLE   = 90;

const int HAND_THRESHOLD = 20;

const float BIN_DEPTH_CM = 30.0;

const int MEDIUM_THRESHOLD   = 50;
const int HIGH_THRESHOLD     = 75;
const int CRITICAL_THRESHOLD = 90;

const unsigned long SENSOR_TIMEOUT = 30000UL;

const unsigned long LID_OPEN_TIME = 3000UL;

// ============================================================
// SERVO
// ============================================================

Servo lidServo;

// ============================================================
// SYSTEM STATE
// ============================================================

bool lidOpen = false;

unsigned long lidOpenedAt = 0;

// ============================================================
// SENSOR RESULT STRUCTURE
// ============================================================

struct SensorReading
{
    float distance;
    bool valid;
};

// ============================================================
// READ ULTRASONIC SENSOR
// ============================================================

SensorReading readDistance(int trigPin, int echoPin)
{
    SensorReading result;

    result.distance = -1;
    result.valid = false;

    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);

    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    unsigned long duration =
        pulseIn(echoPin, HIGH, SENSOR_TIMEOUT);

    if (duration == 0)
    {
        return result;
    }

    float distance = duration * 0.0343 / 2.0;

    if (distance <= 0 || distance > 400)
    {
        return result;
    }

    result.distance = distance;
    result.valid = true;

    return result;
}

// ============================================================
// SAFE SYSTEM STATE
// ============================================================

void applySafeState()
{
    digitalWrite(GREEN_LED_PIN, LOW);
    digitalWrite(RED_LED_PIN, HIGH);

    digitalWrite(BUZZER_PIN, LOW);

    if (lidOpen)
    {
        lidServo.write(CLOSED_ANGLE);

        lidOpen = false;

        Serial.println("SAFE ACTION: Lid forced CLOSED.");
    }
}

// ============================================================
// OPEN LID
// ============================================================

void openLid()
{
    if (lidOpen)
    {
        return;
    }

    Serial.println();
    Serial.println(">>> HAND DETECTED <<<");

    Serial.println("Lid Status    : OPENING");

    lidServo.write(OPEN_ANGLE);

    lidOpen = true;
    lidOpenedAt = millis();

    Serial.println("Lid Status    : OPEN");
    Serial.println("Servo Angle   : 90 degrees");
}

// ============================================================
// CLOSE LID
// ============================================================

void closeLid()
{
    if (!lidOpen)
    {
        return;
    }

    Serial.println("Lid Status    : CLOSING");

    lidServo.write(CLOSED_ANGLE);

    lidOpen = false;

    Serial.println("Lid Status    : CLOSED");
    Serial.println("Servo Angle   : 0 degrees");
}

// ============================================================
// UPDATE LID TIMER
// ============================================================

void updateLid()
{
    if (!lidOpen)
    {
        return;
    }

    if (millis() - lidOpenedAt >= LID_OPEN_TIME)
    {
        closeLid();
    }
}

// ============================================================
// DISPLAY SENSOR FAULT
// ============================================================

void reportSensorFault(const char* sensorName)
{
    Serial.println();
    Serial.println("!!! SENSOR FAULT !!!");

    Serial.print("Sensor        : ");
    Serial.println(sensorName);

    Serial.println("Reading       : INVALID / TIMEOUT");

    Serial.println("Action        : SAFE MODE");

    Serial.println("-------------------------------");
}

// ============================================================
// DISPLAY SYSTEM STATUS
// ============================================================

void displayStatus(
    SensorReading hand,
    SensorReading level
)
{
    Serial.println();
    Serial.println("===============================");

    if (hand.valid)
    {
        Serial.print("Hand Distance : ");
        Serial.print(hand.distance);
        Serial.println(" cm");

        if (hand.distance <= HAND_THRESHOLD)
        {
            Serial.println("Hand Status   : DETECTED");
        }
        else
        {
            Serial.println("Hand Status   : NOT DETECTED");
        }
    }
    else
    {
        Serial.println("Hand Distance : INVALID");
        Serial.println("Hand Status   : SENSOR FAULT");
    }

    if (level.valid)
    {
        Serial.print("Waste Distance: ");
        Serial.print(level.distance);
        Serial.println(" cm");

        float fillPercentage =
            ((BIN_DEPTH_CM - level.distance)
            / BIN_DEPTH_CM) * 100.0;

        fillPercentage = constrain(
            fillPercentage,
            0,
            100
        );

        Serial.print("Estimated Fill: ");
        Serial.print(fillPercentage);
        Serial.println("%");

        if (fillPercentage < MEDIUM_THRESHOLD)
        {
            Serial.println("Bin Status    : NORMAL");
        }
        else if (fillPercentage < HIGH_THRESHOLD)
        {
            Serial.println("Bin Status    : MEDIUM");
        }
        else if (fillPercentage < CRITICAL_THRESHOLD)
        {
            Serial.println("Bin Status    : HIGH");
        }
        else
        {
            Serial.println("Bin Status    : CRITICAL / FULL");
        }
    }
    else
    {
        Serial.println("Waste Distance: INVALID");
        Serial.println("Bin Status    : SENSOR FAULT");
    }

    Serial.println("===============================");
}

// ============================================================
// SETUP
// ============================================================

void setup()
{
    pinMode(HAND_TRIG_PIN, OUTPUT);
    pinMode(HAND_ECHO_PIN, INPUT);

    pinMode(LEVEL_TRIG_PIN, OUTPUT);
    pinMode(LEVEL_ECHO_PIN, INPUT);

    pinMode(GREEN_LED_PIN, OUTPUT);
    pinMode(RED_LED_PIN, OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT);

    Serial.begin(9600);

    lidServo.attach(SERVO_PIN);

    lidServo.write(CLOSED_ANGLE);

    digitalWrite(GREEN_LED_PIN, LOW);
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);

    Serial.println();
    Serial.println("================================");
    Serial.println(" SMART DUSTBIN - PHASE 10");
    Serial.println(" FAULT HANDLING & RELIABILITY");
    Serial.println("================================");

    Serial.println("System initialized.");
    Serial.println("Initial safety state: LID CLOSED");

    delay(1000);
}

// ============================================================
// MAIN LOOP
// ============================================================

void loop()
{
    SensorReading hand =
        readDistance(
            HAND_TRIG_PIN,
            HAND_ECHO_PIN
        );

    SensorReading level =
        readDistance(
            LEVEL_TRIG_PIN,
            LEVEL_ECHO_PIN
        );

    // ========================================================
    // FAULT DETECTION
    // ========================================================

    bool sensorFault = false;

    if (!hand.valid)
    {
        reportSensorFault("HAND SENSOR");

        sensorFault = true;
    }

    if (!level.valid)
    {
        reportSensorFault("WASTE-LEVEL SENSOR");

        sensorFault = true;
    }

    // ========================================================
    // SAFE MODE
    // ========================================================

    if (sensorFault)
    {
        applySafeState();

        displayStatus(hand, level);

        delay(1000);

        return;
    }

    // ========================================================
    // NORMAL OPERATION
    // ========================================================

    float fillPercentage =
        ((BIN_DEPTH_CM - level.distance)
        / BIN_DEPTH_CM) * 100.0;

    fillPercentage = constrain(
        fillPercentage,
        0,
        100
    );

    // ========================================================
    // WASTE STATUS INDICATORS
    // ========================================================

    if (fillPercentage < MEDIUM_THRESHOLD)
    {
        digitalWrite(GREEN_LED_PIN, HIGH);
        digitalWrite(RED_LED_PIN, LOW);
        digitalWrite(BUZZER_PIN, LOW);
    }
    else if (fillPercentage < HIGH_THRESHOLD)
    {
        digitalWrite(GREEN_LED_PIN, HIGH);
        digitalWrite(RED_LED_PIN, LOW);
        digitalWrite(BUZZER_PIN, LOW);
    }
    else if (fillPercentage < CRITICAL_THRESHOLD)
    {
        digitalWrite(GREEN_LED_PIN, LOW);
        digitalWrite(RED_LED_PIN, HIGH);
        digitalWrite(BUZZER_PIN, LOW);
    }
    else
    {
        digitalWrite(GREEN_LED_PIN, LOW);
        digitalWrite(RED_LED_PIN, HIGH);
        digitalWrite(BUZZER_PIN, HIGH);
    }

    // ========================================================
    // HAND DETECTION
    // ========================================================

    if (
        hand.distance <= HAND_THRESHOLD &&
        !lidOpen
    )
    {
        openLid();
    }

    // ========================================================
    // LID TIMER
    // ========================================================

    updateLid();

    // ========================================================
    // STATUS
    // ========================================================

    displayStatus(hand, level);

    delay(1000);
}