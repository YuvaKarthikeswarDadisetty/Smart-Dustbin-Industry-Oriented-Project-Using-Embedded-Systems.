/*
 * ============================================================
 * Smart Dustbin - Phase 5
 * SG90 Servo Motor Interfacing
 * ============================================================
 *
 * Servo:
 * SG90
 *
 * Signal -> D9
 * VCC    -> 5V
 * GND    -> GND
 *
 * Initial Positions:
 *
 * 0°  -> Lid CLOSED
 * 90° -> Lid OPEN
 *
 * ============================================================
 */

#include <Servo.h>

const int SERVO_PIN = 9;

const int CLOSED_ANGLE = 0;
const int OPEN_ANGLE = 90;

const unsigned long OPEN_DURATION = 3000;

Servo lidServo;


/*
 * ------------------------------------------------------------
 * Function: moveLid()
 *
 * Moves the servo to the requested position.
 * ------------------------------------------------------------
 */
void moveLid(int angle)
{
    lidServo.write(angle);

    Serial.print("Servo Angle : ");
    Serial.print(angle);
    Serial.println(" degrees");
}


void setup()
{
    Serial.begin(9600);

    lidServo.attach(SERVO_PIN);

    Serial.println();
    Serial.println("======================================");
    Serial.println(" SMART DUSTBIN - PHASE 5 TEST");
    Serial.println(" SG90 SERVO MOTOR INTERFACING");
    Serial.println("======================================");

    Serial.println();
    Serial.println("Servo Signal : D9");
    Serial.println("Closed Angle : 0 degrees");
    Serial.println("Open Angle   : 90 degrees");

    Serial.println();
    Serial.println("Initializing servo...");

    // Start with lid closed
    moveLid(CLOSED_ANGLE);

    delay(1000);

    Serial.println("System Ready");
    Serial.println();
}


void loop()
{
    // --------------------------------------------------------
    // CLOSE LID
    // --------------------------------------------------------

    Serial.println("--------------------------------------");
    Serial.println("Lid Status : CLOSED");

    moveLid(CLOSED_ANGLE);

    delay(2000);


    // --------------------------------------------------------
    // OPEN LID
    // --------------------------------------------------------

    Serial.println("--------------------------------------");
    Serial.println("Lid Status : OPEN");

    moveLid(OPEN_ANGLE);

    delay(OPEN_DURATION);


    // --------------------------------------------------------
    // CLOSE LID AGAIN
    // --------------------------------------------------------

    Serial.println("--------------------------------------");
    Serial.println("Lid Status : CLOSING");

    moveLid(CLOSED_ANGLE);

    delay(2000);
}