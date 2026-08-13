/*
 * Smart Dustbin
 * Phase 1 - Development Environment Test
 *
 * Purpose:
 * Verify that the Arduino development environment,
 * serial communication, and basic GPIO operation work.
 */

const int TEST_LED = LED_BUILTIN;

void setup()
{
    pinMode(TEST_LED, OUTPUT);

    Serial.begin(9600);

    Serial.println("=================================");
    Serial.println(" SMART DUSTBIN - PHASE 1 TEST");
    Serial.println("=================================");
    Serial.println("Development environment ready.");
}

void loop()
{
    digitalWrite(TEST_LED, HIGH);

    Serial.println("System Test: LED ON");
    delay(1000);

    digitalWrite(TEST_LED, LOW);

    Serial.println("System Test: LED OFF");
    delay(1000);
}