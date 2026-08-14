/*
 * ============================================================
 * Smart Dustbin - Phase 4
 * Ultrasonic Sensor Interfacing
 * ============================================================
 *
 * Sensor 1:
 * HC-SR04
 * TRIG -> D2
 * ECHO -> D3
 * Purpose -> Hand/Object Detection
 *
 * Sensor 2:
 * HC-SR04
 * TRIG -> D4
 * ECHO -> D5
 * Purpose -> Waste-Level Detection
 *
 * ============================================================
 */

const int HAND_TRIG_PIN = 2;
const int HAND_ECHO_PIN = 3;

const int LEVEL_TRIG_PIN = 4;
const int LEVEL_ECHO_PIN = 5;

// Hand detection threshold
const float HAND_DETECTION_THRESHOLD = 20.0;

// Prototype bin height
const float BIN_HEIGHT_CM = 30.0;

// Minimum and maximum valid HC-SR04 range
const float MIN_DISTANCE_CM = 2.0;
const float MAX_DISTANCE_CM = 400.0;


/*
 * ------------------------------------------------------------
 * Function: readDistance()
 *
 * Reads an HC-SR04 sensor and returns distance in centimeters.
 *
 * Return value:
 *   Positive value -> valid distance
 *   -1             -> invalid/timeout reading
 * ------------------------------------------------------------
 */
float readDistance(int trigPin, int echoPin)
{
    // Make sure trigger is LOW
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);

    // Send 10 microsecond trigger pulse
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Measure echo pulse duration
    unsigned long duration =
        pulseIn(echoPin, HIGH, 30000);

    // No echo received
    if (duration == 0)
    {
        return -1.0;
    }

    // Convert time to distance
    float distance =
        duration * 0.0343 / 2.0;

    // Validate sensor range
    if (distance < MIN_DISTANCE_CM ||
        distance > MAX_DISTANCE_CM)
    {
        return -1.0;
    }

    return distance;
}


/*
 * ------------------------------------------------------------
 * Function: calculateFillPercentage()
 *
 * Calculates approximate bin fill percentage.
 *
 * Formula:
 *
 * Fill Level = Bin Height - Sensor Distance
 *
 * Fill % = (Fill Level / Bin Height) * 100
 * ------------------------------------------------------------
 */
int calculateFillPercentage(float distance)
{
    if (distance < 0)
    {
        return -1;
    }

    float fillLevel =
        BIN_HEIGHT_CM - distance;

    // Prevent negative fill level
    if (fillLevel < 0)
    {
        fillLevel = 0;
    }

    // Prevent fill level above bin height
    if (fillLevel > BIN_HEIGHT_CM)
    {
        fillLevel = BIN_HEIGHT_CM;
    }

    float percentage =
        (fillLevel / BIN_HEIGHT_CM) * 100.0;

    return (int)percentage;
}


void setup()
{
    Serial.begin(9600);

    // Sensor 1
    pinMode(HAND_TRIG_PIN, OUTPUT);
    pinMode(HAND_ECHO_PIN, INPUT);

    // Sensor 2
    pinMode(LEVEL_TRIG_PIN, OUTPUT);
    pinMode(LEVEL_ECHO_PIN, INPUT);

    // Initial trigger states
    digitalWrite(HAND_TRIG_PIN, LOW);
    digitalWrite(LEVEL_TRIG_PIN, LOW);

    Serial.println();
    Serial.println("======================================");
    Serial.println(" SMART DUSTBIN - PHASE 4 TEST");
    Serial.println(" ULTRASONIC SENSOR INTERFACING");
    Serial.println("======================================");

    Serial.println();
    Serial.println("Sensor 1 : Hand/Object Detection");
    Serial.println("TRIG     : D2");
    Serial.println("ECHO     : D3");

    Serial.println();

    Serial.println("Sensor 2 : Waste-Level Detection");
    Serial.println("TRIG     : D4");
    Serial.println("ECHO     : D5");

    Serial.println();
    Serial.println("System Ready");
    Serial.println();
}


void loop()
{
    // --------------------------------------------------------
    // Read Sensor 1
    // --------------------------------------------------------

    float handDistance =
        readDistance(
            HAND_TRIG_PIN,
            HAND_ECHO_PIN
        );


    // --------------------------------------------------------
    // Read Sensor 2
    // --------------------------------------------------------

    float wasteDistance =
        readDistance(
            LEVEL_TRIG_PIN,
            LEVEL_ECHO_PIN
        );


    // --------------------------------------------------------
    // Display Hand Sensor
    // --------------------------------------------------------

    Serial.println("--------------------------------------");

    if (handDistance < 0)
    {
        Serial.println(
            "Hand Sensor : INVALID READING"
        );
    }
    else
    {
        Serial.print(
            "Hand Distance : "
        );

        Serial.print(handDistance);

        Serial.println(" cm");


        if (handDistance <=
            HAND_DETECTION_THRESHOLD)
        {
            Serial.println(
                "Hand Status   : DETECTED"
            );
        }
        else
        {
            Serial.println(
                "Hand Status   : NOT DETECTED"
            );
        }
    }


    // --------------------------------------------------------
    // Display Waste Sensor
    // --------------------------------------------------------

    if (wasteDistance < 0)
    {
        Serial.println(
            "Level Sensor : INVALID READING"
        );
    }
    else
    {
        Serial.print(
            "Waste Distance : "
        );

        Serial.print(wasteDistance);

        Serial.println(" cm");


        int fillPercentage =
            calculateFillPercentage(
                wasteDistance
            );


        Serial.print(
            "Estimated Fill : "
        );

        Serial.print(fillPercentage);

        Serial.println("%");


        if (fillPercentage >= 90)
        {
            Serial.println(
                "Bin Status     : CRITICAL / FULL"
            );
        }
        else if (fillPercentage >= 75)
        {
            Serial.println(
                "Bin Status     : HIGH"
            );
        }
        else if (fillPercentage >= 50)
        {
            Serial.println(
                "Bin Status     : MEDIUM"
            );
        }
        else
        {
            Serial.println(
                "Bin Status     : NORMAL"
            );
        }
    }


    Serial.println("--------------------------------------");

    delay(1000);
}