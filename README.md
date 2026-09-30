# Ladakh Monitoring System

An ESP32-based monitoring and control system built for high-altitude / cold-climate conditions (Ladakh). It watches the temperature of a battery / distribution unit (DU), automatically drives a heater relay to keep it from freezing, tracks the surrounding environment, and pushes live data to Firebase for remote monitoring.

## Features

- Dual temperature sensing using W1209 NTC probes
- Automatic heater control with hysteresis (no relay chatter)
- External environment monitoring (temperature, humidity, pressure) via BME280
- 3-page OLED display (internal readings, external readings, system status)
- WiFi connectivity with automatic reconnect
- Live data upload to Firebase Realtime Database
- Historical logging for trend analysis

## Hardware

| Component | Role |
|---|---|
| ESP32 Dev Board | Main controller |
| W1209 NTC probe #1 | Internal environment temperature (monitoring only) |
| W1209 NTC probe #2 | DU / battery temperature (controls the heater) |
| BME280 | External temperature, humidity, pressure |
| SSD1306 OLED (128x64, I2C) | Local display |
| Relay module (active LOW) | Switches the heater |
| Buck converter | Steps down field power (e.g. 12V) to 5V for the ESP32 and relay |

## Pin Connections

| Signal | ESP32 Pin |
|---|---|
| Internal NTC probe | GPIO32 |
| DU / battery NTC probe | GPIO33 |
| Relay IN | GPIO23 |
| I2C SDA (OLED + BME280) | GPIO22 |
| I2C SCL (OLED + BME280) | GPIO21 |

**Power**
- OLED and BME280 → 3.3V
- Relay module → 5V (from buck converter output)
- All GND lines tied to a common ground

**NTC probes** (voltage divider, one per probe)
```
Probe ---- GND
Probe ---- GPIO (32 or 33) ---- 10kΩ resistor ---- 3.3V
```

## Heater (Relay) Logic

The relay is controlled only by the DU / battery probe (GPIO33):

- DU temperature **≤ 20°C** → heater **ON**
- DU temperature **≥ 22°C** → heater **OFF**
- Between 20°C and 22°C → relay holds its previous state (hysteresis, prevents rapid on/off switching)

If the DU sensor reading is invalid (probe disconnected), the heater is forced OFF as a safety default.

## OLED Pages

1. **Internal / DU** – internal temperature, DU temperature, relay state, heater state
2. **External** – external temperature, humidity, pressure, BME280 status
3. **System** – WiFi status, cloud upload status, relay state, time since last successful upload

Pages rotate automatically every few seconds.

## Firebase

Data is pushed to a Firebase Realtime Database:

- `/ladakhMonitoringSystem/latest` – overwritten every 10 seconds with the current reading
- `/ladakhMonitoringSystem/history` – a new timestamped entry appended every 60 seconds

Each payload includes internal temp, DU temp, external temp/humidity/pressure, relay state, sensor health flags, WiFi status, and uptime.

## Setup

1. Install the required Arduino libraries:
   - `Adafruit SSD1306`
   - `Adafruit GFX`
   - `Adafruit BME280`
   - `Adafruit Unified Sensor`
2. In the sketch, set your network and Firebase credentials:
   ```cpp
   const char* WIFI_SSID     = "YOUR_WIFI_NAME";
   const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";
   const char* FIREBASE_DATABASE_URL = "https://YOUR_PROJECT-default-rtdb.firebaseio.com";
   const char* FIREBASE_AUTH = "YOUR_DATABASE_SECRET";
   ```
3. Select **ESP32 Dev Module** under Tools → Board, choose the correct COM port, and upload.
4. Open the Serial Monitor at 115200 baud to confirm WiFi, sensor, and Firebase status on boot.

## Safety Notes

- If the heater runs on mains (AC) voltage, keep all high-voltage wiring isolated to the relay's dry contacts (COM/NO). No ESP32 pin should ever touch the AC side.
- The relay defaults to OFF at boot and stays OFF if the DU sensor fails, to avoid uncontrolled heating.

## License

Add your preferred license here (e.g. MIT).