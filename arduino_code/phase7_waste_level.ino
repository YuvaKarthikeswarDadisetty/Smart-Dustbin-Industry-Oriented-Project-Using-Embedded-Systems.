/*
 * ============================================================
 * Smart Dustbin - Phase 7
 * Waste-Level Monitoring
 * ============================================================
 *
 * HC-SR04 #2:
 *
 * TRIG -> D4
 * ECHO -> D5
 *
 * Purpose:
 * Detect the amount of waste inside the dustbin.
 *
 * Bin Depth:
 * 30 cm
 *
 * Fill Calculation:
 *
 * Fill % = ((30 - distance) / 30) * 100
 *
 * Status:
 *
 * 0% - 49%   -> NORMAL
 * 50% - 74%  -> MEDIUM
 * 75% - 89%  -> HIGH
 * 90% - 100% -> CRITICAL / FULL
 *
 * ============================================================
 */

const int LEVEL_TRIG_PIN = 4;
const int LEVEL_ECHO_PIN = 5;


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
// Read Ultrasonic Distance
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


    // No echo received
    if (duration == 0)
    {
        return -1.0;
    }


    float distance =
        duration * 0.0343 / 2.0;


    // Invalid distance
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


    // Limit to 0 - 100
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


    digitalWrite(
        LEVEL_TRIG_PIN,
        LOW
    );


    Serial.println();

    Serial.println(
        "======================================"
    );

    Serial.println(
        " SMART DUSTBIN - PHASE 7 TEST"
    );

    Serial.println(
        " WASTE-LEVEL MONITORING"
    );

    Serial.println(
        "======================================"
    );


    Serial.println();

    Serial.println(
        "Sensor : HC-SR04 #2"
    );

    Serial.println(
        "TRIG   : D4"
    );

    Serial.println(
        "ECHO   : D5"
    );


    Serial.println();

    Serial.print(
        "Bin Depth : "
    );

    Serial.print(
        BIN_DEPTH_CM
    );

    Serial.println(" cm");


    Serial.println();

    Serial.println(
        "Fill Thresholds:"
    );

    Serial.println(
        "NORMAL   : < 50%"
    );

    Serial.println(
        "MEDIUM   : 50% - 74%"
    );

    Serial.println(
        "HIGH     : 75% - 89%"
    );

    Serial.println(
        "CRITICAL : >= 90%"
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
    // Display Results
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

    Serial.println(" cm");


    Serial.print(
        "Estimated Fill : "
    );

    Serial.print(
        fillPercentage
    );

    Serial.println("%");


    Serial.print(
        "Bin Status     : "
    );

    Serial.println(
        binStatus
    );


    Serial.println(
        "--------------------------------------"
    );

    Serial.println();


    delay(1000);
}