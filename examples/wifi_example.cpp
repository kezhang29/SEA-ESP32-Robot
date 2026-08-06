/**
 * BasicExample
 *
 * Demonstrates the WiFiController library:
 *   - Connects to Wi-Fi and hosts the on-screen controller page.
 *   - Blinks the built-in LED each time B is pressed.
 *   - Prints "Driving Forward" to Serial while W is held.
 *   - Prints "Turning Left" to Serial while A is held.
 *   - Logs button press/release events to the Serial Monitor.
 *   - Publishes a couple of example telemetry values.
 *
 * Update WIFI_SSID / WIFI_PASSWORD below before uploading.
 */

#include <Arduino.h>
#include "WiFiController.h"

// ----- Wi-Fi credentials -------------------------------------------------
static const char* WIFI_SSID = "MyWiFi";
static const char* WIFI_PASSWORD = "password";

#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

WiFiController wifi;

bool ledState = false;

void driveForward() {
    Serial.println("Driving Forward");
}

void turnLeft() {
    Serial.println("Turning Left");
}

void toggleLED() {
    ledState = !ledState;
    digitalWrite(LED_BUILTIN, ledState ? HIGH : LOW);
    Serial.println(ledState ? "LED ON" : "LED OFF");
}

void setup() {
    Serial.begin(115200);
    delay(200);

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    wifi.begin(WIFI_SSID, WIFI_PASSWORD);

    wifi.setText("Battery", "12.4 V");
    wifi.setText("Mode", "Manual");
}

void loop() {
    wifi.update();

    // Continuous driving while held down.
    if (wifi.buttonWPressed()) {
        driveForward();
    }

    if (wifi.buttonAPressed()) {
        turnLeft();
    }

    // One-shot event: toggle the LED exactly once per press.
    if (wifi.buttonBJustPressed()) {
        toggleLED();
    }

    // Example event logging for the rest of the buttons.
    if (wifi.buttonDJustPressed())               Serial.println("D pressed (turn right)");
    if (wifi.buttonSPressedJustPressed())         Serial.println("S pressed (drive backward)");
    if (wifi.buttonSpacePressed())                Serial.println("SPACE held");
    if (wifi.buttonXPressed())                    Serial.println("X held");
    if (wifi.buttonYPressed())                    Serial.println("Y held");
    if (wifi.button1Pressed())                    Serial.println("1 held");
    if (wifi.button2Pressed())                    Serial.println("2 held");
    if (wifi.button3Pressed())                    Serial.println("3 held");

    if (wifi.buttonWReleased()) Serial.println("W released");
    if (wifi.buttonBReleased()) Serial.println("B released");

    // Keep an example telemetry value changing so students can see the
    // web page update live.
    static unsigned long lastTelemetryUpdate = 0;
    if (millis() - lastTelemetryUpdate > 1000) {
        lastTelemetryUpdate = millis();
        wifi.setText("Uptime", String(millis() / 1000) + " s");
    }
}
