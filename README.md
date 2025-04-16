# BagTrack IoT Tracker (ESP8266 + GPS + Telegram)

**BagTrack** is an affordable smart backpack solution using **NodeMCU (ESP8266)** and **GPS module**, integrated with **Telegram Bot API** for real-time location alerts and anti-theft triggers.

---

## Features

- Real-time GPS Location via Telegram
- Instant Alerts on Physical Trigger (e.g., bag zipper open detection)
- Internet connectivity via Wi-Fi
- Simple bot commands: `/status` for location
- Fallback default location if GPS is unavailable

---

## Hardware Used

| Component             | Description                 | Estimated Cost |
|----------------------|-----------------------------|----------------|
| NodeMCU ESP8266      | Microcontroller (Wi-Fi)     | ₹300           |
| NEO-6M GPS Module     | GPS module (UART)           | ₹600           |
| RF Receiver/Switch    | Physical trigger input       | ₹300           |
| Jumper Wires & Breadboard | For circuit assembly    | ₹300           |

> **Total Cost:** ~₹1,500

---

## Software & Tools

- Arduino IDE
- Telegram Bot API
- Libraries:
  - `TinyGPSPlus`
  - `SoftwareSerial`
  - `ESP8266WiFi`
  - `WiFiClientSecure`
  - `UniversalTelegramBot`

---


## Telegram Commands

| Command     | Action                              |
|-------------|-------------------------------------|
| `/status`   | Sends current GPS location (or fallback) |
| `/start`    | Start message (optional)            |
| `/stop`     | Stop message (optional)             |

---

## How It Works

1. **ESP8266** connects to your Wi-Fi network.
2. **GPS module** provides real-time coordinates.
3. If someone physically triggers the bag (RF trigger on D1), ESP8266 sends your live location to your **Telegram chat**.
4. Users can request the location anytime using `/status`.

---


