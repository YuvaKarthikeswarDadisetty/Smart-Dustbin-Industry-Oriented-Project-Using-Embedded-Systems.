/*
 * ============================================================
 * Smart Dustbin - Phase 8
 * LED + Buzzer Status Indicators
 * ============================================================
 *
 * Waste-Level Sensor:
 *
 * HC-SR04 #2
 * TRIG -> D4
 * ECHO -> D5
 *
 * Indicators:
 *
 * Green LED -> D6
 * Red LED   -> D7
 * Buzzer    -> D8
 *
 * Bin Depth:
 * 30 cm
 *
 * Status:
 *
 * 0% - 49%   -> NORMAL
 * 50% - 74%  -> MEDIUM
 * 75% - 89%  -> HIGH
 * 90% - 100% -> CRITICAL / FULL
 *
 * Indicator Logic:
 *
 * NORMAL
 *   Green LED ON
 *   Red LED OFF
 *   Buzzer OFF
 *
 * MEDIUM
 *   Green LED ON
 *   Red LED OFF
 *   Buzzer OFF
 *
 * HIGH
 *   Green LED OFF
 *   Red LED ON
 *   Buzzer OFF
 *
 * CRITICAL / FULL
 *   Green LED OFF
 *   Red LED ON
 *   Buzzer ON
 *
 * ============================================================
 */


// ------------------------------------------------------------
// Pin Configuration
// ------------------------------------------------------------

const int LEVEL_TRIG_PIN = 4;
const int LEVEL_ECHO_PIN = 5;

const int GREEN_LED_PIN = 6;
const int RED_LED_PIN = 7;
const int BUZZER_PIN = 8;


// ------------------------------------------------------------
// Bin Configuration
// ------------------------------------------------------------

const float BIN_DEPTH_CM = 30.0;


// ------------------------------------------------------------
// Fill Thresholds
// ------------------------------------------------------------

const int MEDIUM_THRESHOLD = 50;
const int HIGH_THRESHOLD = 75;
const int CRITICAL_THRESHOLD = 90;


// ------------------------------------------------------------
// Read Waste Distance
// ------------------------------------------------------------

float readWasteDistance()
{
    digitalWrite(
        LEVEL_TRIG_PIN,
        LOW
    );

    delayMicroseconds(2);

    digitalWrite(
        LEVEL_TRIG_PIN,
        HIGH
    );

    delayMicroseconds(10);

    digitalWrite(
        LEVEL_TRIG_PIN,
        LOW
    );


    unsigned long duration =
        pulseIn(
            LEVEL_ECHO_PIN,
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


// ------------------------------------------------------------
// Calculate Fill Percentage
// ------------------------------------------------------------

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


// ------------------------------------------------------------
// Determine Bin Status
// ------------------------------------------------------------

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


// ------------------------------------------------------------
// Control Status Indicators
// ------------------------------------------------------------

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


// ------------------------------------------------------------
// Setup
// ------------------------------------------------------------

void setup()
{
    Serial.begin(9600);


    pinMode(
        LEVEL_TRIG_PIN,
        OUTPUT
    );

    pinMode(
        LEVEL_ECHO_PIN,
        INPUT
    );


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


    Serial.println();

    Serial.println(
        "======================================"
    );

    Serial.println(
        " SMART DUSTBIN - PHASE 8 TEST"
    );

    Serial.println(
        " LED + BUZZER STATUS INDICATORS"
    );

    Serial.println(
        "======================================"
    );


    Serial.println();

    Serial.println(
        "Waste Sensor:"
    );

    Serial.println(
        "TRIG : D4"
    );

    Serial.println(
        "ECHO : D5"
    );


    Serial.println();

    Serial.println(
        "Indicators:"
    );

    Serial.println(
        "Green LED : D6"
    );

    Serial.println(
        "Red LED   : D7"
    );

    Serial.println(
        "Buzzer    : D8"
    );


    Serial.println();

    Serial.println(
        "System Ready"
    );

    Serial.println();
}


// ------------------------------------------------------------
// Main Loop
// ------------------------------------------------------------

void loop()
{
    float wasteDistance =
        readWasteDistance();


    // --------------------------------------------------------
    // Invalid Reading
    // --------------------------------------------------------

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

        Serial.println();


        delay(1000);

        return;
    }


    // --------------------------------------------------------
    // Calculate Fill
    // --------------------------------------------------------

    int fillPercentage =
        calculateFillPercentage(
            wasteDistance
        );


    String binStatus =
        getBinStatus(
            fillPercentage
        );


    // --------------------------------------------------------
    // Update Indicators
    // --------------------------------------------------------

    updateIndicators(
        fillPercentage
    );


    // --------------------------------------------------------
    // Serial Output
    // --------------------------------------------------------

    Serial.println(
        "--------------------------------------"
    );


    Serial.print(
        "Waste Distance : "
    );

    Serial.print(
        wasteDistance
    );

    Serial.println(
        " cm"
    );


    Serial.print(
        "Estimated Fill : "
    );

    Serial.print(
        fillPercentage
    );

    Serial.println(
        "%"
    );


    Serial.print(
        "Bin Status     : "
    );

    Serial.println(
        binStatus
    );


    if (
        fillPercentage <
        HIGH_THRESHOLD
    )
    {
        Serial.println(
            "Indicator      : GREEN"
        );
    }
    else if (
        fillPercentage <
        CRITICAL_THRESHOLD
    )
    {
        Serial.println(
            "Indicator      : RED"
        );
    }
    else
    {
        Serial.println(
            "Indicator      : RED + BUZZER"
        );
    }


    Serial.println(
        "--------------------------------------"
    );

    Serial.println();


    delay(1000);
}