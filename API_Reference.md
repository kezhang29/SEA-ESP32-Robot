# ESP32Robot — API Reference

> A beginner-friendly library for building Wi-Fi-controlled robots with the ESP32.

---

## Table of Contents

1. [Getting Started](#getting-started)
2. [WiFiController](#wificontroller)
3. [Motor](#motor)
4. [Drivetrain](#drivetrain)
5. [Quick Reference](#quick-reference)

---

## Getting Started

### Include the Library

```cpp
#include "ESP32Robot.h"   // Includes everything: WiFi, Motors, Drivetrain
```

Or include only what you need:

```cpp
#include "WiFiController.h"   // Just the web controller
#include "Motor.h"             // Just motor control
#include "Drivetrain.h"       // Just the 4-motor drivetrain
```

### Basic Setup Pattern

Every sketch follows this pattern:

```cpp
#include "ESP32Robot.h"

WiFiController wifi;            // Create the controller

void setup() {
    Serial.begin(115200);
    wifi.begin("YourWiFiName", "YourWiFiPassword");
}

void loop() {
    wifi.update();              // Always call this first in loop()

    if (wifi.buttonWPressed()) {
        // Do something while W is held down
    }
}
```

After uploading, check the **Serial Monitor** for the IP address (e.g., `192.168.1.42`). Open that address in any browser to see the controller.

---

## WiFiController

The `WiFiController` connects your ESP32 to Wi-Fi and hosts a web page that acts as a virtual gamepad.

### Constructor

```cpp
WiFiController wifi;
```

Creates a new controller. No arguments needed.

---

### `begin(ssid, password)`

Connects to a Wi-Fi network and starts the web server. **Blocks** until connected.

```cpp
wifi.begin("MyWiFi", "mypassword");
```

| Parameter | Type | Description |
|---|---|---|
| `ssid` | `const char*` | Your Wi-Fi network name |
| `password` | `const char*` | Your Wi-Fi network password |

**Example:**
```cpp
void setup() {
    wifi.begin("SchoolWiFi", "password123");
}
```

> **Note:** The ESP32 must be on the **same Wi-Fi network** as your phone or computer for the controller to work.

---

### `update()`

Processes incoming web requests and refreshes button state. **Call this once at the top of `loop()`** — every sketch needs it.

```cpp
void loop() {
    wifi.update();   // Always first!

    // Now you can check buttons...
}
```

| Parameter | Type | Description |
|---|---|---|
| (none) | — | — |

**Returns:** Nothing (`void`)

---

### `connected()`

Checks if the ESP32 is still connected to Wi-Fi.

```cpp
if (wifi.connected()) {
    Serial.println("We are online!");
}
```

| Parameter | Type | Description |
|---|---|---|
| (none) | — | — |

**Returns:** `true` if connected, `false` if not

---

### Button Queries — "Currently Held Down"

These return `true` as long as the button is **held down** on the web page or keyboard.

| Method | Button | Keyboard Key |
|---|---|---|
| `buttonWPressed()` | W | W |
| `buttonAPressed()` | A | A |
| `buttonSPressed()` | S | S |
| `buttonDPressed()` | D | D |
| `buttonBPressed()` | B | B |
| `buttonXPressed()` | X | X |
| `buttonYPressed()` | Y | Y |
| `buttonSpacePressed()` | SPACE | Spacebar |
| `button1Pressed()` | 1 | 1 |
| `button2Pressed()` | 2 | 2 |
| `button3Pressed()` | 3 | 3 |

**Example — robot moves forward while W is held:**
```cpp
if (wifi.buttonWPressed()) {
    myRobot.forward(200);
}
```

---

### Button Queries — "Just Pressed This Frame"

These return `true` **only once** — the exact moment the button transitions from released to pressed. Use these for actions that should happen **one time per press** (like toggling an LED).

> **Note:** Not every button has a "just pressed" variant yet. The available ones are listed below.

| Method | Button |
|---|---|
| `buttonWJustPressed()` | W |
| `buttonAJustPressed()` | A |
| `buttonSPressedJustPressed()` | S |
| `buttonDJustPressed()` | D |
| `buttonBJustPressed()` | B |

**Example — toggle an LED each time B is pressed:**
```cpp
bool ledOn = false;

void loop() {
    wifi.update();

    if (wifi.buttonBJustPressed()) {
        ledOn = !ledOn;                    // Flip the state
        digitalWrite(LED_BUILTIN, ledOn ? HIGH : LOW);
    }
}
```

---

### Button Queries — "Just Released This Frame"

These return `true` **only once** — the exact moment the button transitions from pressed to released.

| Method | Button |
|---|---|
| `buttonWReleased()` | W |
| `buttonBReleased()` | B |

**Example — log when the user lets go:**
```cpp
if (wifi.buttonWReleased()) {
    Serial.println("W released — stopping!");
    myRobot.stop();
}
```

---

### `setText(label, value)`

Displays (or updates) a label/value pair on the web page's **telemetry panel**. The web page updates automatically — no extra code needed.

```cpp
wifi.setText("Battery", "12.4 V");
wifi.setText("Speed", "200");
wifi.setText("Mode", "Auto");
```

| Parameter | Type | Description |
|---|---|---|
| `label` | `String` | The name to display (e.g., `"Battery"`) |
| `value` | `String` | The value to display (e.g., `"12.4 V"`) |

**Returns:** Nothing (`void`)

**Examples:**
```cpp
// Show the robot's uptime
wifi.setText("Uptime", String(millis() / 1000) + " s");

// Show a sensor reading
int temperature = readTemperature();
wifi.setText("Temp", String(temperature) + " C");

// Update an existing label (same name, new value)
wifi.setText("Speed", "0");
delay(1000);
wifi.setText("Speed", "150");   // Updates in place
```

> **Limits:** Maximum **12** telemetry items at once. Each label and value can be up to **24 characters** long.

---

### `clearTelemetry()`

Removes all telemetry items from the web page.

```cpp
wifi.clearTelemetry();
```

| Parameter | Type | Description |
|---|---|---|
| (none) | — | — |

**Returns:** Nothing (`void`)

---

## Motor

The `Motor` class controls a single DC motor connected through a motor driver (like an L298N or TB6612FNG).

### Constructor

```cpp
Motor myMotor(in1_pin, in2_pin, pwm_pin, ledc_channel, standby_pin);
```

| Parameter | Type | Description |
|---|---|---|
| `in1` | `int` | GPIO pin connected to IN1 on the motor driver |
| `in2` | `int` | GPIO pin connected to IN2 on the motor driver |
| `pwm` | `int` | GPIO pin for PWM (speed control) |
| `channel` | `int` | LEDC PWM channel (0-15, pick a unique number per motor) |
| `stby` | `int` | GPIO pin for standby/enable (optional; use `-1` if not used) |

**Example:**
```cpp
// Motor connected to GPIO pins 12, 13, 14, using LEDC channel 0, standby on pin 15
Motor leftMotor(12, 13, 14, 0, 15);
```

---

### `configure()`

Sets up the motor's GPIO pins. Call this **once** in `setup()`.

```cpp
void setup() {
    leftMotor.configure();
    rightMotor.configure();
}
```

| Parameter | Type | Description |
|---|---|---|
| (none) | — | — |

**Returns:** Nothing (`void`)

---

### `setSpeed(speed)`

Sets the motor's speed and direction.

```cpp
leftMotor.setSpeed(200);     // Forward at medium speed
leftMotor.setSpeed(-150);    // Reverse at medium speed
leftMotor.setSpeed(0);        // Stop
```

| Parameter | Type | Description |
|---|---|---|
| `speed` | `int` | Speed from **-255** (full reverse) to **255** (full forward). **0** = stop. |

**Returns:** Nothing (`void`)

**Speed Guide:**

| Value | Meaning |
|---|---|
| `-255` | Full speed reverse |
| `-128` | Half speed reverse |
| `0` | Stop |
| `128` | Half speed forward |
| `255` | Full speed forward |

---

## Drivetrain

The `Drivetrain` class groups **4 motors** (2 left, 2 right) and provides easy-to-use movement commands.

### Constructor

```cpp
Drivetrain robot(leftFront, leftBack, rightFront, rightBack);
```

| Parameter | Type | Description |
|---|---|---|
| `leftFront` | `Motor&` | Reference to the front-left motor |
| `leftBack` | `Motor&` | Reference to the back-left motor |
| `rightFront` | `Motor&` | Reference to the front-right motor |
| `rightBack` | `Motor&` | Reference to the back-right motor |

**Example:**
```cpp
Motor lf(12, 13, 14, 0, 15);
Motor lb(16, 17, 18, 1, 19);
Motor rf(20, 21, 22, 2, 23);
Motor rb(2, 3, 4, 3, 5);

Drivetrain robot(lf, lb, rf, rb);
```

---

### `configure()`

Configures all 4 motors. Call once in `setup()`.

```cpp
void setup() {
    robot.configure();
}
```

---

### Movement Commands

All movement commands take a `speed` parameter from **0** (stopped) to **255** (full speed).

#### `forward(speed)`

```cpp
robot.forward(200);    // Move forward
```

#### `backward(speed)`

```cpp
robot.backward(150);    // Move backward
```

#### `left(speed)`

```cpp
robot.left(180);        // Turn left (spin in place)
```

#### `right(speed)`

```cpp
robot.right(180);       // Turn right (spin in place)
```

#### `stop()`

```cpp
robot.stop();           // Stop all motors
```

#### `arcadeDrive(leftSpeed, rightSpeed)`

Independent control of left and right wheels — for advanced maneuvers.

```cpp
robot.arcadeDrive(200, 100);   // Left wheels faster than right = gentle right curve
robot.arcadeDrive(-200, 200);  // Left reverse, right forward = sharp left spin
```

| Parameter | Type | Description |
|---|---|---|
| `leftSpeed` | `int` | Speed for left wheels: -255 (reverse) to 255 (forward) |
| `rightSpeed` | `int` | Speed for right wheels: -255 (reverse) to 255 (forward) |

---

## Quick Reference

### Button Layout on the Web Page

```
        [ W ]

    [ A ] [ S ] [ D ]

        [ SPACE ]

    [ B ] [ X ] [ Y ]

    [ 1 ] [ 2 ] [ 3 ]
```

### Typical Movement Mapping

| Button | Common Use |
|---|---|
| W | Move forward |
| S | Move backward |
| A | Turn left |
| D | Turn right |
| SPACE | Emergency stop |
| B, X, Y | Custom actions (toggle lights, play sounds, etc.) |
| 1, 2, 3 | Mode switching or custom actions |

### Minimal Working Sketch

```cpp
#include "ESP32Robot.h"

WiFiController wifi;

void setup() {
    Serial.begin(115200);
    wifi.begin("MyWiFi", "password");
}

void loop() {
    wifi.update();

    if (wifi.buttonWPressed()) {
        Serial.println("Going forward!");
    }
}
```

### Common Patterns

**Hold to do something:**
```cpp
if (wifi.buttonWPressed()) {
    robot.forward(200);
} else {
    robot.stop();
}
```

**Press once to toggle:**
```cpp
if (wifi.buttonBJustPressed()) {
    lightsOn = !lightsOn;
}
```

**Show data on the web page:**
```cpp
wifi.setText("Battery", String(batteryVoltage) + " V");
```
