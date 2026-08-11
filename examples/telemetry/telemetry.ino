/**
 * Example 1: Telemetry
 *
 * Learn how to send data from the ESP32 to the web page.
 *
 * What it does:
 *   - Connects to Wi-Fi and starts the web controller
 *   - Updates values on the web page every second
 *
 * Hardware needed: Just an ESP32 board
 */

#include "ESP32Robot.h"

WiFiController wifi;

void setup() {
    Serial.begin(115200);

     // Connect to Wi-Fi (change to your network name and password)
    wifi.begin("MyWiFi", "password");
}

void loop() {
     // Always call update() first
    wifi.update();

     // Send data to the web page every second
    if (millis() % 1000 < 20) {
         // Show how long the ESP32 has been running
        wifi.setText("Uptime", String(millis() / 1000) + " s");

         // Show a random number (like a sensor reading)
        wifi.setText("Random", String(random(0, 100)));

         // Show a message if a button is held
        if (wifi.buttonWPressed()) {
            wifi.setText("Button W", "Pressed!");
         } else {
            wifi.setText("Button W", "--");
         }
     }
}
