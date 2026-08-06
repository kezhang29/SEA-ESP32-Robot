/**
 * webpage.h
 *
 * Holds the entire front-end (HTML + CSS + JavaScript) served by
 * WiFiController as a single PROGMEM string. Keeping it here keeps
 * WiFiController.cpp focused purely on server/button logic.
 */

#ifndef WIFICONTROLLER_WEBPAGE_H
#define WIFICONTROLLER_WEBPAGE_H

#include <Arduino.h>

const char WIFICONTROLLER_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no">
<title>WiFiController</title>
<style>
  :root {
    --bg: #0f1115;
    --panel: #171a21;
    --panel-2: #1e222b;
    --accent: #4fd1c5;
    --accent-2: #7c6cf6;
    --text: #eef1f5;
    --text-dim: #8a91a0;
    --btn: #232733;
    --btn-active: linear-gradient(145deg, #4fd1c5, #38b2ac);
    --danger: #f56565;
  }

  * { box-sizing: border-box; -webkit-tap-highlight-color: transparent; }

  html, body {
    margin: 0;
    padding: 0;
    height: 100%;
    background: var(--bg);
    color: var(--text);
    font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Helvetica, Arial, sans-serif;
    overscroll-behavior: none;
    touch-action: none;
    user-select: none;
  }

  .wrap {
    display: flex;
    flex-direction: column;
    align-items: center;
    min-height: 100%;
    padding: 16px;
    gap: 16px;
  }

  header {
    display: flex;
    align-items: center;
    gap: 10px;
    width: 100%;
    max-width: 480px;
    justify-content: space-between;
  }

  header h1 {
    font-size: 18px;
    margin: 0;
    letter-spacing: 0.5px;
    font-weight: 600;
  }

  .status {
    display: flex;
    align-items: center;
    gap: 6px;
    font-size: 12px;
    color: var(--text-dim);
  }

  .dot {
    width: 9px;
    height: 9px;
    border-radius: 50%;
    background: var(--accent);
    box-shadow: 0 0 8px var(--accent);
  }

  .dot.offline {
    background: var(--danger);
    box-shadow: 0 0 8px var(--danger);
  }

  .panel {
    width: 100%;
    max-width: 480px;
    background: var(--panel);
    border-radius: 18px;
    padding: 18px;
    box-shadow: 0 10px 30px rgba(0,0,0,0.35);
  }

  .telemetry {
    display: grid;
    grid-template-columns: repeat(auto-fill, minmax(120px, 1fr));
    gap: 10px;
    margin-bottom: 6px;
  }

  .telemetry .item {
    background: var(--panel-2);
    border-radius: 12px;
    padding: 10px 12px;
  }

  .telemetry .label {
    font-size: 11px;
    color: var(--text-dim);
    text-transform: uppercase;
    letter-spacing: 0.5px;
    margin-bottom: 4px;
  }

  .telemetry .value {
    font-size: 16px;
    font-weight: 600;
  }

  .telemetry.empty::after {
    content: "No telemetry yet";
    color: var(--text-dim);
    font-size: 13px;
    grid-column: 1 / -1;
  }

  .pad {
    width: 100%;
    max-width: 480px;
    display: grid;
    gap: 10px;
  }

  .row {
    display: grid;
    gap: 10px;
  }

  .row.three { grid-template-columns: repeat(3, 1fr); }
  .row.one { grid-template-columns: 1fr; }

  button.btn {
    appearance: none;
    border: none;
    outline: none;
    border-radius: 16px;
    background: var(--btn);
    color: var(--text);
    font-size: 18px;
    font-weight: 700;
    padding: 20px 0;
    letter-spacing: 1px;
    box-shadow: 0 4px 0 rgba(0,0,0,0.35), 0 6px 14px rgba(0,0,0,0.25);
    transition: transform 0.05s ease, background 0.08s ease, box-shadow 0.08s ease;
  }

  button.btn:active,
  button.btn.active {
    background: var(--btn-active);
    color: #06231f;
    transform: translateY(3px);
    box-shadow: 0 1px 0 rgba(0,0,0,0.35), 0 2px 6px rgba(0,0,0,0.25);
  }

  .btn-space {
    background: var(--panel-2);
    color: var(--accent-2);
    letter-spacing: 4px;
  }

  .btn-space.active {
    background: linear-gradient(145deg, #7c6cf6, #5c4bd6);
    color: #f4f2ff;
  }

  .btn-num {
    font-size: 15px;
    padding: 14px 0;
    color: var(--text-dim);
  }

  footer {
    color: var(--text-dim);
    font-size: 11px;
    margin-top: 4px;
    text-align: center;
  }

  @media (min-width: 520px) {
    button.btn { font-size: 20px; }
  }
</style>
</head>
<body>
  <div class="wrap">
    <header>
      <h1>WiFiController</h1>
      <div class="status">
        <span class="dot" id="statusDot"></span>
        <span id="statusText">connecting</span>
      </div>
    </header>

    <div class="panel">
      <div class="telemetry" id="telemetry"></div>
    </div>

    <div class="pad">
      <div class="row one">
        <button class="btn" data-btn="w">W</button>
      </div>
      <div class="row three">
        <button class="btn" data-btn="a">A</button>
        <button class="btn" data-btn="s">S</button>
        <button class="btn" data-btn="d">D</button>
      </div>
      <div class="row one">
        <button class="btn btn-space" data-btn="space">SPACE</button>
      </div>
      <div class="row three">
        <button class="btn" data-btn="b">B</button>
        <button class="btn" data-btn="x">X</button>
        <button class="btn" data-btn="y">Y</button>
      </div>
      <div class="row three">
        <button class="btn btn-num" data-btn="k1">1</button>
        <button class="btn btn-num" data-btn="k2">2</button>
        <button class="btn btn-num" data-btn="k3">3</button>
      </div>
    </div>

    <footer>Keyboard: W A S D &middot; Space &middot; B X Y &middot; 1 2 3</footer>
  </div>

<script>
(function () {
  "use strict";

  var STATE_KEYS = ["w", "a", "s", "d", "b", "x", "y", "space", "k1", "k2", "k3"];
  var state = { w:0, a:0, s:0, d:0, b:0, x:0, y:0, space:0, k1:0, k2:0, k3:0 };

  var KEY_TO_BTN = {
    "KeyW": "w", "KeyA": "a", "KeyS": "s", "KeyD": "d",
    "KeyB": "b", "KeyX": "x", "KeyY": "y",
    "Space": "space",
    "Digit1": "k1", "Digit2": "k2", "Digit3": "k3"
  };

  var statusDot = document.getElementById("statusDot");
  var statusText = document.getElementById("statusText");
  var telemetryEl = document.getElementById("telemetry");
  var buttons = {};

  document.querySelectorAll(".btn").forEach(function (el) {
    buttons[el.getAttribute("data-btn")] = el;
  });

  var sendTimer = null;
  var pendingSend = false;

  function setButtonState(name, pressed) {
    if (!(name in state)) return;
    if (state[name] === (pressed ? 1 : 0)) return;
    state[name] = pressed ? 1 : 0;
    var el = buttons[name];
    if (el) el.classList.toggle("active", !!pressed);
    sendState();
  }

  function buildQuery() {
    var parts = [];
    for (var i = 0; i < STATE_KEYS.length; i++) {
      var k = STATE_KEYS[i];
      parts.push(k + "=" + state[k]);
    }
    return parts.join("&");
  }

  function sendState() {
    // Coalesce rapid-fire changes into a single in-flight request.
    if (pendingSend) return;
    pendingSend = true;
    fetch("/state?" + buildQuery(), { method: "GET", cache: "no-store" })
      .then(function (res) {
        setOnline(res.ok);
      })
      .catch(function () {
        setOnline(false);
      })
      .finally(function () {
        pendingSend = false;
      });
  }

  function setOnline(online) {
    statusDot.classList.toggle("offline", !online);
    statusText.textContent = online ? "connected" : "offline";
  }

  // --- Keyboard input -----------------------------------------------
  window.addEventListener("keydown", function (e) {
    if (e.code === "Space") e.preventDefault();
    var name = KEY_TO_BTN[e.code];
    if (!name) return;
    if (e.repeat) return;
    setButtonState(name, true);
  }, { passive: false });

  window.addEventListener("keyup", function (e) {
    if (e.code === "Space") e.preventDefault();
    var name = KEY_TO_BTN[e.code];
    if (!name) return;
    setButtonState(name, false);
  }, { passive: false });

  // --- Mouse / touch input -------------------------------------------
  Object.keys(buttons).forEach(function (name) {
    var el = buttons[name];

    var press = function (ev) { ev.preventDefault(); setButtonState(name, true); };
    var release = function (ev) { ev.preventDefault(); setButtonState(name, false); };

    el.addEventListener("mousedown", press);
    el.addEventListener("mouseup", release);
    el.addEventListener("mouseleave", release);

    el.addEventListener("touchstart", press, { passive: false });
    el.addEventListener("touchend", release, { passive: false });
    el.addEventListener("touchcancel", release, { passive: false });
  });

  // Safety net: if the tab loses focus while a key is held, release
  // everything so a button doesn't get "stuck" pressed.
  window.addEventListener("blur", function () {
    Object.keys(state).forEach(function (name) { setButtonState(name, false); });
  });

  // --- Telemetry polling ----------------------------------------------
  function renderTelemetry(items) {
    telemetryEl.classList.toggle("empty", items.length === 0);
    telemetryEl.innerHTML = items.map(function (it) {
      return '<div class="item"><div class="label"></div><div class="value"></div></div>';
    }).join("");
    var nodes = telemetryEl.querySelectorAll(".item");
    items.forEach(function (it, i) {
      nodes[i].querySelector(".label").textContent = it.label;
      nodes[i].querySelector(".value").textContent = it.value;
    });
  }

  function pollTelemetry() {
    fetch("/telemetry", { cache: "no-store" })
      .then(function (res) { return res.json(); })
      .then(function (data) {
        renderTelemetry(data);
        setOnline(true);
      })
      .catch(function () {
        setOnline(false);
      });
  }

  pollTelemetry();
  setInterval(pollTelemetry, 500);

  // Keep a periodic heartbeat so the "connected" indicator and any
  // dropped state changes recover automatically on flaky Wi-Fi.
  setInterval(sendState, 2000);
})();
</script>
</body>
</html>
)rawliteral";

#endif // WIFICONTROLLER_WEBPAGE_H
