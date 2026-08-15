/*
 * ============================================================
 * SMART DUSTBIN - PHASE 9
 * INTEGRATED SMART DUSTBIN CONTROL
 * ============================================================
 *
 * SYSTEM COMPONENTS
 *
 * Hand Detection Sensor:
 * HC-SR04 #1
 * TRIG -> D2
 * ECHO -> D3
 *
 * Waste-Level Sensor:
 * HC-SR04 #2
 * TRIG -> D4
 * ECHO -> D5
 *
 * Indicators:
 * Green LED -> D6
 * Red LED   -> D7
 * Buzzer    -> D8
 *
 * Lid Servo:
 * SG90
 * Signal -> D9
 *
 * ============================================================
 *
 * HAND DETECTION
 *
 * Distance <= 20 cm
 *        ↓
 * Hand detected
 *        ↓
 * Servo opens to 90 degrees
 *        ↓
 * Lid remains open for 3 seconds
 *        ↓
 * Servo returns to 0 degrees
 *
 * ============================================================
 *
 * WASTE LEVEL
 *
 * Bin depth = 30 cm
 *
 * 0% - 49%   -> NORMAL
 * 50% - 74%  -> MEDIUM
 * 75% - 89%  -> HIGH
 * 90% - 100% -> CRITICAL / FULL
 *
 * ============================================================
 *
 * INDICATORS
 *
 * NORMAL:
 * Green ON
 * Red OFF
 * Buzzer OFF
 *
 * MEDIUM:
 * Green ON
 * Red OFF
 * Buzzer OFF
 *
 * HIGH:
 * Green OFF
 * Red ON
 * Buzzer OFF
 *
 * CRITICAL / FULL:
 * Green OFF
 * Red ON
 * Buzzer ON
 *
 * ============================================================
 */

#include <Servo.h>


// ============================================================
// PIN CONFIGURATION
// ============================================================

// Hand detection sensor
const int HAND_TRIG_PIN = 2;
const int HAND_ECHO_PIN = 3;

// Waste-level sensor
const int LEVEL_TRIG_PIN = 4;
const int LEVEL_ECHO_PIN = 5;

// Status indicators
const int GREEN_LED_PIN = 6;
const int RED_LED_PIN = 7;
const int BUZZER_PIN = 8;

// Servo
const int SERVO_PIN = 9;


// ============================================================
// SYSTEM CONFIGURATION
// ============================================================

const float BIN_DEPTH_CM = 30.0;

const float HAND_DETECTION_DISTANCE = 20.0;

const int CLOSED_ANGLE = 0;
const int OPEN_ANGLE = 90;

const unsigned long LID_OPEN_DURATION = 3000;


// ============================================================
// WASTE THRESHOLDS
// ============================================================

const int MEDIUM_THRESHOLD = 50;
const int HIGH_THRESHOLD = 75;
const int CRITICAL_THRESHOLD = 90;


// ============================================================
// SERVO OBJECT
// ============================================================

Servo lidServo;


// ============================================================
// LID STATE
// ============================================================

bool lidOpen = false;

unsigned long lidOpenedAt = 0;


// ============================================================
// SENSOR READING FUNCTION
// ============================================================

float readDistance(
    int trigPin,
    int echoPin
)
{
    digitalWrite(
        trigPin,
        LOW
    );

    delayMicroseconds(2);


    digitalWrite(
        trigPin,
        HIGH
    );

    delayMicroseconds(10);


    digitalWrite(
        trigPin,
        LOW
    );


    unsigned long duration =
        pulseIn(
            echoPin,
            HIGH,
            30000
        );


    if (duration == 0)
    {
        return -1.0;
    }


    float distance =
        duration * 0.0343 / 2.0;


    if (
        distance < 2.0 ||
        distance > 400.0
    )
    {
        return -1.0;
    }


    return distance;
}


// ============================================================
// CALCULATE FILL PERCENTAGE
// ============================================================

int calculateFillPercentage(
    float distance
)
{
    float fillPercentage =
        (
            (BIN_DEPTH_CM - distance)
            / BIN_DEPTH_CM
        ) * 100.0;


    if (fillPercentage < 0)
    {
        fillPercentage = 0;
    }


    if (fillPercentage > 100)
    {
        fillPercentage = 100;
    }


    return (int)fillPercentage;
}


// ============================================================
// GET BIN STATUS
// ============================================================

String getBinStatus(
    int fillPercentage
)
{
    if (
        fillPercentage >=
        CRITICAL_THRESHOLD
    )
    {
        return "CRITICAL / FULL";
    }


    if (
        fillPercentage >=
        HIGH_THRESHOLD
    )
    {
        return "HIGH";
    }


    if (
        fillPercentage >=
        MEDIUM_THRESHOLD
    )
    {
        return "MEDIUM";
    }


    return "NORMAL";
}


// ============================================================
// UPDATE STATUS INDICATORS
// ============================================================

void updateIndicators(
    int fillPercentage
)
{
    // --------------------------------------------------------
    // NORMAL / MEDIUM
    // --------------------------------------------------------

    if (
        fillPercentage <
        HIGH_THRESHOLD
    )
    {
        digitalWrite(
            GREEN_LED_PIN,
            HIGH
        );

        digitalWrite(
            RED_LED_PIN,
            LOW
        );

        noTone(
            BUZZER_PIN
        );

        return;
    }


    // --------------------------------------------------------
    // HIGH
    // --------------------------------------------------------

    if (
        fillPercentage <
        CRITICAL_THRESHOLD
    )
    {
        digitalWrite(
            GREEN_LED_PIN,
            LOW
        );

        digitalWrite(
            RED_LED_PIN,
            HIGH
        );

        noTone(
            BUZZER_PIN
        );

        return;
    }


    // --------------------------------------------------------
    // CRITICAL / FULL
    // --------------------------------------------------------

    digitalWrite(
        GREEN_LED_PIN,
        LOW
    );

    digitalWrite(
        RED_LED_PIN,
        HIGH
    );

    tone(
        BUZZER_PIN,
        1000
    );
}


// ============================================================
// OPEN LID
// ============================================================

void openLid()
{
    lidServo.write(
        OPEN_ANGLE
    );

    lidOpen = true;

    lidOpenedAt = millis();


    Serial.println();
    Serial.println(
        ">>> HAND DETECTED <<<"
    );

    Serial.println(
        "Lid Status : OPENING"
    );


    delay(300);


    Serial.println(
        "Lid Status : OPEN"
    );

    Serial.println(
        "Servo Angle : 90 degrees"
    );

    Serial.println(
        "Open Duration : 3 seconds"
    );
}


// ============================================================
// CLOSE LID
// ============================================================

void closeLid()
{
    lidServo.write(
        CLOSED_ANGLE
    );

    lidOpen = false;


    Serial.println(
        "Lid Status : CLOSING"
    );


    delay(300);


    Serial.println(
        "Lid Status : CLOSED"
    );

    Serial.println(
        "Servo Angle : 0 degrees"
    );

    Serial.println();
}


// ============================================================
// CHECK AUTOMATIC LID
// ============================================================

void updateLidControl(
    float handDistance
)
{
    // --------------------------------------------------------
    // If lid is closed and hand is detected
    // --------------------------------------------------------

    if (
        !lidOpen &&
        handDistance > 0 &&
        handDistance <=
        HAND_DETECTION_DISTANCE
    )
    {
        openLid();

        return;
    }


    // --------------------------------------------------------
    // If lid is open, wait 3 seconds
    // --------------------------------------------------------

    if (lidOpen)
    {
        unsigned long elapsed =
            millis() -
            lidOpenedAt;


        if (
            elapsed >=
            LID_OPEN_DURATION
        )
        {
            closeLid();
        }
    }
}


// ============================================================
// SETUP
// ============================================================

void setup()
{
    Serial.begin(9600);


    // --------------------------------------------------------
    // Sensor pins
    // --------------------------------------------------------

    pinMode(
        HAND_TRIG_PIN,
        OUTPUT
    );

    pinMode(
        HAND_ECHO_PIN,
        INPUT
    );


    pinMode(
        LEVEL_TRIG_PIN,
        OUTPUT
    );

    pinMode(
        LEVEL_ECHO_PIN,
        INPUT
    );


    // --------------------------------------------------------
    // Indicator pins
    // --------------------------------------------------------

    pinMode(
        GREEN_LED_PIN,
        OUTPUT
    );

    pinMode(
        RED_LED_PIN,
        OUTPUT
    );

    pinMode(
        BUZZER_PIN,
        OUTPUT
    );


    // --------------------------------------------------------
    // Initial indicator state
    // --------------------------------------------------------

    digitalWrite(
        HAND_TRIG_PIN,
        LOW
    );

    digitalWrite(
        LEVEL_TRIG_PIN,
        LOW
    );

    digitalWrite(
        GREEN_LED_PIN,
        LOW
    );

    digitalWrite(
        RED_LED_PIN,
        LOW
    );

    noTone(
        BUZZER_PIN
    );


    // --------------------------------------------------------
    // Servo initialization
    // --------------------------------------------------------

    lidServo.attach(
        SERVO_PIN
    );

    lidServo.write(
        CLOSED_ANGLE
    );


    // --------------------------------------------------------
    // Startup message
    // --------------------------------------------------------

    Serial.println();

    Serial.println(
        "================================================"
    );

    Serial.println(
        "       SMART DUSTBIN - PHASE 9"
    );

    Serial.println(
        "       INTEGRATED SYSTEM TEST"
    );

    Serial.println(
        "================================================"
    );

    Serial.println();


    Serial.println(
        "SYSTEM COMPONENTS"
    );

    Serial.println(
        "Hand Sensor  : D2 / D3"
    );

    Serial.println(
        "Waste Sensor : D4 / D5"
    );

    Serial.println(
        "Green LED    : D6"
    );

    Serial.println(
        "Red LED      : D7"
    );

    Serial.println(
        "Buzzer       : D8"
    );

    Serial.println(
        "Servo        : D9"
    );


    Serial.println();

    Serial.println(
        "Hand Threshold : 20 cm"
    );

    Serial.println(
        "Bin Depth      : 30 cm"
    );

    Serial.println(
        "Lid Open Time  : 3 seconds"
    );


    Serial.println();

    Serial.println(
        "System Status : READY"
    );

    Serial.println(
        "Lid Status    : CLOSED"
    );

    Serial.println();
}


// ============================================================
// MAIN LOOP
// ============================================================

void loop()
{
    // ========================================================
    // 1. READ HAND SENSOR
    // ========================================================

    float handDistance =
        readDistance(
            HAND_TRIG_PIN,
            HAND_ECHO_PIN
        );


    // ========================================================
    // 2. READ WASTE SENSOR
    // ========================================================

    float wasteDistance =
        readDistance(
            LEVEL_TRIG_PIN,
            LEVEL_ECHO_PIN
        );


    // ========================================================
    // 3. HANDLE HAND DETECTION
    // ========================================================

    updateLidControl(
        handDistance
    );


    // ========================================================
    // 4. HANDLE WASTE LEVEL
    // ========================================================

    if (wasteDistance < 0)
    {
        digitalWrite(
            GREEN_LED_PIN,
            LOW
        );

        digitalWrite(
            RED_LED_PIN,
            LOW
        );

        noTone(
            BUZZER_PIN
        );


        Serial.println(
            "Waste Sensor : INVALID READING"
        );

        Serial.println(
            "Fill Status  : UNKNOWN"
        );
    }
    else
    {
        int fillPercentage =
            calculateFillPercentage(
                wasteDistance
            );


        String binStatus =
            getBinStatus(
                fillPercentage
            );


        updateIndicators(
            fillPercentage
        );


        // ----------------------------------------------------
        // Display system data
        // ----------------------------------------------------

        Serial.println(
            "------------------------------------------------"
        );


        Serial.print(
            "Hand Distance : "
        );

        if (handDistance > 0)
        {
            Serial.print(
                handDistance
            );

            Serial.println(
                " cm"
            );
        }
        else
        {
            Serial.println(
                "INVALID"
            );
        }


        Serial.print(
            "Hand Status   : "
        );

        if (
            handDistance > 0 &&
            handDistance <=
            HAND_DETECTION_DISTANCE
        )
        {
            Serial.println(
                "DETECTED"
            );
        }
        else
        {
            Serial.println(
                "NOT DETECTED"
            );
        }


        Serial.print(
            "Waste Distance: "
        );

        Serial.print(
            wasteDistance
        );

        Serial.println(
            " cm"
        );


        Serial.print(
            "Estimated Fill: "
        );

        Serial.print(
            fillPercentage
        );

        Serial.println(
            "%"
        );


        Serial.print(
            "Bin Status    : "
        );

        Serial.println(
            binStatus
        );


        Serial.print(
            "Lid Status    : "
        );

        if (lidOpen)
        {
            Serial.println(
                "OPEN"
            );
        }
        else
        {
            Serial.println(
                "CLOSED"
            );
        }


        if (
            fillPercentage <
            HIGH_THRESHOLD
        )
        {
            Serial.println(
                "Indicator     : GREEN"
            );
        }
        else if (
            fillPercentage <
            CRITICAL_THRESHOLD
        )
        {
            Serial.println(
                "Indicator     : RED"
            );
        }
        else
        {
            Serial.println(
                "Indicator     : RED + BUZZER"
            );
        }


        Serial.println(
            "------------------------------------------------"
        );
    }


    Serial.println();


    delay(500);
}