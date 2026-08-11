/**
 * Example 2: Motors
 *
 * Learn how to control motors and the drivetrain directly (no Wi-Fi).
 *
 * What it does:
 *   - Drives forward for 2 seconds
 *   - Turns right for 1 second
 *   - Drives backward for 2 seconds
 *   - Turns left for 1 second
 *   - Repeats forever
 *
 * Hardware needed: ESP32 + motor driver + 4 motors
 *
 * IMPORTANT: Change the pin numbers below to match your wiring!
 */

#include "ESP32Robot.h"

// --- Create 4 motors ---
// Format: Motor(IN1, IN2, PWM, channel, standby)
Motor leftFront(12, 13, 14, 0, 15);
Motor leftBack(16, 17, 18, 1, 19);
Motor rightFront(20, 21, 22, 2, 23);
Motor rightBack(2, 3, 4, 3, 5);

// --- Group them into a drivetrain ---
Drivetrain robot(leftFront, leftBack, rightFront, rightBack);

void setup() {
    Serial.begin(115200);

     // Set up all motor pins
    robot.configure();
}

void loop() {
     // Drive forward
    Serial.println("Forward!");
    robot.forward(150);
    delay(2000);

     // Turn right
    Serial.println("Turn Right!");
    robot.right(150);
    delay(1000);

     // Drive backward
    Serial.println("Backward!");
    robot.backward(150);
    delay(2000);

     // Turn left
    Serial.println("Turn Left!");
    robot.left(150);
    delay(1000);

     // Stop briefly before repeating
    robot.stop();
    delay(500);
}
