# ESP8266 XOX Game (Tic-Tac-Toe)

A two-player Tic-Tac-Toe game built using an ESP8266 NodeMCU board, 5 physical push buttons, and a Wi-Fi-based web interface — no need for external hosting or filesystem upload!

## 🔧 Features

- 🕹️ **Real-time interaction** with physical buttons:
  - `UP`, `LEFT`, `RIGHT`, `SELECT`, and `DOWN` (optional)
- 🌐 **Access via browser** — ESP8266 runs in AP (Access Point) mode
- 🧠 Smart in-browser UI:
  - Grid highlighting with cursor
  - Tracks turns for `X` and `O`
  - Win/draw detection
  - Scoreboard
  - "Next Game" and "Reset Score" buttons
- 🪛 Simple, detachable hardware via header pins
- 🖥️ No LittleFS or SPIFFS required

---

## 📷 Screenshot
![Screenshot1](https://github.com/user-attachments/assets/1f43cd47-1747-493a-b590-d42e7eed33f6)

![Screenshot2](https://github.com/user-attachments/assets/d0e79774-9f99-4424-af77-95da7b71d8a2)

![Screenshot3](https://github.com/user-attachments/assets/69a96739-7fec-447d-9bbc-b853a9678548)

---

## 🖥️ Web Interface

Once powered on:
- Connect to Wi-Fi: `XOX_Game` (password: `12345678`)
- Open browser and go to: [http://192.168.4.1](http://192.168.4.1)

---

## 🔌 Hardware Setup

| Function | NodeMCU Pin | GPIO | Status |
|----------|-------------|------|--------|
| Up       | D1          | 5    | ✅ Connected |
| Left     | D2          | 4    | ✅ Connected |
| Right    | D5          | 14   | ✅ Connected |
| Select   | D6          | 12   | ✅ Connected |
| Down     | D7          | 13   | ✅ Connected |

**ESP8266 Dev Board:** NodeMCU v1.0 (ESP-12E module)  
**Buttons:** Tactile push buttons
**Hardware simulation :
![Circuit demo](https://github.com/user-attachments/assets/d023299a-a0e7-45b1-98bb-8563fd79c228)
!!Down button is not placed due to certain technical difficulties

---

## 🛠️ Schematic (KiCad)

The PCB includes:
- Female headers to plug/unplug NodeMCU
- 5 push buttons connected to GPIOs
- All traces on the bottom side

---

## 🚀 Getting Started

1. Open `src/xox_game.ino` in Arduino IDE
2. Select board: `NodeMCU 1.0 (ESP-12E Module)`
3. Upload the sketch
4. Connect power, join Wi-Fi, and play!

## 🧠 TODO / Improvements

- Add buzzer feedback or LED indicators
- Expand to multiplayer over web (ESP8266 WebSocket)
- Add OLED support
