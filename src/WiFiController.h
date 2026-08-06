/**
 * WiFiController.h
 *
 * A beginner-friendly library that turns an ESP32 into a Wi-Fi controlled
 * robot. It hosts a responsive, modern web page (a virtual gamepad) that
 * students can use from a phone, tablet, or laptop -- either by tapping
 * on-screen buttons or by using the keyboard. All networking, HTTP
 * handling, HTML, CSS, and JavaScript are hidden inside this library.
 *
 * Typical usage:
 *
 *     #include "WiFiController.h"
 *
 *     WiFiController wifi;
 *
 *     void setup() {
 *         wifi.begin("MyWiFi", "password");
 *     }
 *
 *     void loop() {
 *         wifi.update();
 *
 *         if (wifi.buttonWPressed()) {
 *             driveForward();
 *         }
 *     }
 *
 * License: MIT
 */

#ifndef WIFICONTROLLER_H
#define WIFICONTROLLER_H

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

/// Maximum number of telemetry key/value pairs that can be displayed
/// on the web page at once (see setText()).
static const uint8_t WIFICONTROLLER_MAX_TELEMETRY = 12;

/// Maximum length (characters) stored for a telemetry label or value.
static const uint8_t WIFICONTROLLER_MAX_TELEMETRY_LEN = 24;

class WiFiController {
public:
    WiFiController();

    // ---------------------------------------------------------------
    // Setup / lifecycle
    // ---------------------------------------------------------------

    /**
     * Connects to the given Wi-Fi network and starts the built-in web
     * server that hosts the controller web page. Blocks (with a
     * progress message on Serial) until the connection succeeds.
     *
     * @param ssid     Wi-Fi network name to connect to.
     * @param password Wi-Fi network password.
     */
    void begin(const char* ssid, const char* password);

    /**
     * Services incoming web requests and refreshes all button state
     * (current / previous / just-pressed / just-released). Call this
     * once, near the top of loop(). No other bookkeeping is required.
     */
    void update();

    /**
     * @return true if the ESP32 is currently connected to Wi-Fi.
     */
    bool connected();

    // ---------------------------------------------------------------
    // "Currently held down" queries
    // ---------------------------------------------------------------

    bool buttonWPressed();
    bool buttonAPressed();
    bool buttonSPressed();
    bool buttonDPressed();

    bool buttonBPressed();
    bool buttonXPressed();
    bool buttonYPressed();

    bool buttonSpacePressed();

    bool button1Pressed();
    bool button2Pressed();
    bool button3Pressed();

    // ---------------------------------------------------------------
    // "Pressed this frame" queries (true only on the update() call
    // during which the button transitioned from released -> pressed)
    // ---------------------------------------------------------------

    bool buttonWJustPressed();
    bool buttonAJustPressed();
    bool buttonSPressedJustPressed();
    bool buttonDJustPressed();

    bool buttonBJustPressed();

    // ---------------------------------------------------------------
    // "Released this frame" queries (true only on the update() call
    // during which the button transitioned from pressed -> released)
    // ---------------------------------------------------------------

    bool buttonWReleased();
    bool buttonBReleased();

    // ---------------------------------------------------------------
    // Telemetry
    // ---------------------------------------------------------------

    /**
     * Shows (or updates) a label/value pair on the web page, e.g.
     *     wifi.setText("Battery", "12.4 V");
     * Calling this again with the same label updates its value in
     * place; the web page reflects the change automatically.
     *
     * @param label Name of the telemetry field (e.g. "Battery").
     * @param value Text to display for that field (e.g. "12.4 V").
     */
    void setText(const String& label, const String& value);

    /**
     * Removes all telemetry fields previously set with setText(),
     * clearing the telemetry panel on the web page.
     */
    void clearTelemetry();

private:
    // ---- Button identifiers -----------------------------------------
    enum ButtonId {
        BTN_W = 0,
        BTN_A,
        BTN_S,
        BTN_D,
        BTN_B,
        BTN_X,
        BTN_Y,
        BTN_SPACE,
        BTN_1,
        BTN_2,
        BTN_3,
        BTN_COUNT
    };

    struct TelemetryItem {
        char label[WIFICONTROLLER_MAX_TELEMETRY_LEN];
        char value[WIFICONTROLLER_MAX_TELEMETRY_LEN];
        bool used;
    };

    // ---- Web server ---------------------------------------------------
    WebServer _server;

    // ---- Button state ---------------------------------------------------
    bool _current[BTN_COUNT];
    bool _previous[BTN_COUNT];
    bool _justPressed[BTN_COUNT];
    bool _justReleased[BTN_COUNT];

    // ---- Telemetry storage ---------------------------------------------
    TelemetryItem _telemetry[WIFICONTROLLER_MAX_TELEMETRY];

    // ---- Internal helpers ------------------------------------------------
    void setupRoutes();
    void handleRoot();
    void handleState();
    void handleTelemetry();
    void handleNotFound();

    bool isPressed(ButtonId id) const;
    bool isJustPressed(ButtonId id) const;
    bool isJustReleased(ButtonId id) const;

    static String jsonEscape(const String& text);
};

#endif // WIFICONTROLLER_H
