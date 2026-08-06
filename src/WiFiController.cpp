/**
 * WiFiController.cpp
 *
 * Implementation of the WiFiController class. See WiFiController.h for
 * the public API and usage example.
 */

#include "WiFiController.h"
#include "webpage.h"

// ---------------------------------------------------------------------
// Construction
// ---------------------------------------------------------------------

WiFiController::WiFiController() : _server(80) {
    for (uint8_t i = 0; i < BTN_COUNT; i++) {
        _current[i] = false;
        _previous[i] = false;
        _justPressed[i] = false;
        _justReleased[i] = false;
    }

    for (uint8_t i = 0; i < WIFICONTROLLER_MAX_TELEMETRY; i++) {
        _telemetry[i].used = false;
        _telemetry[i].label[0] = '\0';
        _telemetry[i].value[0] = '\0';
    }
}

// ---------------------------------------------------------------------
// Setup / lifecycle
// ---------------------------------------------------------------------

void WiFiController::begin(const char* ssid, const char* password) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    Serial.print("[WiFiController] Connecting to \"");
    Serial.print(ssid);
    Serial.print("\"");

    while (WiFi.status() != WL_CONNECTED) {
        delay(250);
        Serial.print(".");
    }

    Serial.println();
    Serial.print("[WiFiController] Connected! Open http://");
    Serial.print(WiFi.localIP());
    Serial.println("/ in a browser to drive the robot.");

    setupRoutes();
    _server.begin();

    Serial.println("[WiFiController] Web server started.");
}

void WiFiController::update() {
    // Handle any pending HTTP requests. This is what actually updates
    // _current[] via handleState().
    _server.handleClient();

    // Compute just-pressed / just-released transitions relative to the
    // state captured at the end of the previous update() call, then
    // snapshot the current state for next time.
    for (uint8_t i = 0; i < BTN_COUNT; i++) {
        _justPressed[i] = _current[i] && !_previous[i];
        _justReleased[i] = !_current[i] && _previous[i];
        _previous[i] = _current[i];
    }
}

bool WiFiController::connected() {
    return WiFi.status() == WL_CONNECTED;
}

// ---------------------------------------------------------------------
// Button queries - "currently held"
// ---------------------------------------------------------------------

bool WiFiController::buttonWPressed() { return isPressed(BTN_W); }
bool WiFiController::buttonAPressed() { return isPressed(BTN_A); }
bool WiFiController::buttonSPressed() { return isPressed(BTN_S); }
bool WiFiController::buttonDPressed() { return isPressed(BTN_D); }

bool WiFiController::buttonBPressed() { return isPressed(BTN_B); }
bool WiFiController::buttonXPressed() { return isPressed(BTN_X); }
bool WiFiController::buttonYPressed() { return isPressed(BTN_Y); }

bool WiFiController::buttonSpacePressed() { return isPressed(BTN_SPACE); }

bool WiFiController::button1Pressed() { return isPressed(BTN_1); }
bool WiFiController::button2Pressed() { return isPressed(BTN_2); }
bool WiFiController::button3Pressed() { return isPressed(BTN_3); }

// ---------------------------------------------------------------------
// Button queries - "just pressed this update()"
// ---------------------------------------------------------------------

bool WiFiController::buttonWJustPressed() { return isJustPressed(BTN_W); }
bool WiFiController::buttonAJustPressed() { return isJustPressed(BTN_A); }
bool WiFiController::buttonSPressedJustPressed() { return isJustPressed(BTN_S); }
bool WiFiController::buttonDJustPressed() { return isJustPressed(BTN_D); }

bool WiFiController::buttonBJustPressed() { return isJustPressed(BTN_B); }

// ---------------------------------------------------------------------
// Button queries - "just released this update()"
// ---------------------------------------------------------------------

bool WiFiController::buttonWReleased() { return isJustReleased(BTN_W); }
bool WiFiController::buttonBReleased() { return isJustReleased(BTN_B); }

// ---------------------------------------------------------------------
// Telemetry
// ---------------------------------------------------------------------

void WiFiController::setText(const String& label, const String& value) {
    int freeSlot = -1;

    for (uint8_t i = 0; i < WIFICONTROLLER_MAX_TELEMETRY; i++) {
        if (_telemetry[i].used && label.equals(_telemetry[i].label)) {
            strncpy(_telemetry[i].value, value.c_str(), WIFICONTROLLER_MAX_TELEMETRY_LEN - 1);
            _telemetry[i].value[WIFICONTROLLER_MAX_TELEMETRY_LEN - 1] = '\0';
            return;
        }
        if (!_telemetry[i].used && freeSlot == -1) {
            freeSlot = i;
        }
    }

    if (freeSlot == -1) {
        // Telemetry panel is full; silently ignore rather than crash.
        return;
    }

    strncpy(_telemetry[freeSlot].label, label.c_str(), WIFICONTROLLER_MAX_TELEMETRY_LEN - 1);
    _telemetry[freeSlot].label[WIFICONTROLLER_MAX_TELEMETRY_LEN - 1] = '\0';

    strncpy(_telemetry[freeSlot].value, value.c_str(), WIFICONTROLLER_MAX_TELEMETRY_LEN - 1);
    _telemetry[freeSlot].value[WIFICONTROLLER_MAX_TELEMETRY_LEN - 1] = '\0';

    _telemetry[freeSlot].used = true;
}

void WiFiController::clearTelemetry() {
    for (uint8_t i = 0; i < WIFICONTROLLER_MAX_TELEMETRY; i++) {
        _telemetry[i].used = false;
    }
}

// ---------------------------------------------------------------------
// Private helpers - button state
// ---------------------------------------------------------------------

bool WiFiController::isPressed(ButtonId id) const {
    return _current[id];
}

bool WiFiController::isJustPressed(ButtonId id) const {
    return _justPressed[id];
}

bool WiFiController::isJustReleased(ButtonId id) const {
    return _justReleased[id];
}

// ---------------------------------------------------------------------
// Private helpers - web server
// ---------------------------------------------------------------------

void WiFiController::setupRoutes() {
    _server.on("/", HTTP_GET, [this]() { handleRoot(); });
    _server.on("/state", HTTP_GET, [this]() { handleState(); });
    _server.on("/telemetry", HTTP_GET, [this]() { handleTelemetry(); });
    _server.onNotFound([this]() { handleNotFound(); });
}

void WiFiController::handleRoot() {
    _server.send_P(200, "text/html", WIFICONTROLLER_HTML);
}

void WiFiController::handleState() {
    // The web page always sends the full button state on every
    // request, so any argument that is present simply overwrites the
    // corresponding entry. Arguments that are missing are left
    // untouched, which keeps this endpoint tolerant of future changes
    // to the front end.
    struct { const char* key; ButtonId id; } mapping[] = {
        { "w", BTN_W }, { "a", BTN_A }, { "s", BTN_S }, { "d", BTN_D },
        { "b", BTN_B }, { "x", BTN_X }, { "y", BTN_Y },
        { "space", BTN_SPACE },
        { "k1", BTN_1 }, { "k2", BTN_2 }, { "k3", BTN_3 }
    };

    for (auto& m : mapping) {
        if (_server.hasArg(m.key)) {
            _current[m.id] = _server.arg(m.key).toInt() != 0;
        }
    }

    _server.send(200, "text/plain", "OK");
}

void WiFiController::handleTelemetry() {
    String json = "[";
    bool first = true;

    for (uint8_t i = 0; i < WIFICONTROLLER_MAX_TELEMETRY; i++) {
        if (!_telemetry[i].used) continue;

        if (!first) json += ",";
        first = false;

        json += "{\"label\":\"";
        json += jsonEscape(_telemetry[i].label);
        json += "\",\"value\":\"";
        json += jsonEscape(_telemetry[i].value);
        json += "\"}";
    }

    json += "]";

    _server.send(200, "application/json", json);
}

void WiFiController::handleNotFound() {
    _server.send(404, "text/plain", "Not found");
}

String WiFiController::jsonEscape(const String& text) {
    String out;
    out.reserve(text.length());

    for (size_t i = 0; i < text.length(); i++) {
        char c = text.charAt(i);
        switch (c) {
            case '\"': out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\n': out += "\\n"; break;
            case '\r': out += "\\r"; break;
            case '\t': out += "\\t"; break;
            default:
                out += c;
                break;
        }
    }

    return out;
}
