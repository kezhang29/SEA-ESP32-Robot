/**
 * Example 3: WiFi Drive
 *
 * Learn how to control a robot using the web controller.
 *
 * What it does:
 *   - Connects to Wi-Fi and starts the web controller
 *   - W = forward, S = backward, A = left, D = right
 *   - SPACE = stop
 *
 * Hardware needed: ESP32 + motor driver + 4 motors
 *
 * IMPORTANT: Change the pin numbers and Wi-Fi credentials to match your setup!
 */

#include "ESP32Robot.h"

// --- Create the web controller ---
WiFiController wifi;

// --- Create 4 motors ---
// Format: Motor(IN1, IN2, PWM, channel, standby)
Motor leftFront(12, 13, 14, 0, 15);
Motor leftBack(16, 17, 18, 1, 19);
Motor rightFront(20, 21, 22, 2, 23);
Motor rightBack(2, 3, 4, 3, 5);

// --- Group them into a drivetrain ---
Drivetrain robot(leftFront, leftBack, rightFront, rightBack);

// --- How fast the robot drives (0 = stopped, 255 = full speed) ---
const int SPEED = 150;

void setup() {
    Serial.begin(115200);

     // Set up all motor pins
    robot.configure();

     // Connect to Wi-Fi (change to your network name and password)
    wifi.begin("MyWiFi", "password");
}

void loop() {
     // Always call update() first — it checks for button presses
    wifi.update();

     // Emergency stop
    if (wifi.buttonSpacePressed()) {
        robot.stop();
     }
     // Forward
    else if (wifi.buttonWPressed()) {
        robot.forward(SPEED);
     }
     // Backward
    else if (wifi.buttonSPressed()) {
        robot.backward(SPEED);
     }
     // Turn left
    else if (wifi.buttonAPressed()) {
        robot.left(SPEED);
     }
     // Turn right
    else if (wifi.buttonDPressed()) {
        robot.right(SPEED);
     }
     // No button pressed — stop
    else {
        robot.stop();
     }
}
