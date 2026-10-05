Hardware Connections

The prototype uses an ESP32-WROOM-32E as the main controller. The BMP280 temperature/pressure sensor and SSD1306 OLED display communicate with the ESP32 through the I²C interface.

ESP32-WROOM-32E Connections

Component| Pin| ESP32 Pin
BMP280| VCC| 3.3V
BMP280| GND| GND
BMP280| SDA| GPIO 21
BMP280| SCL| GPIO 22
OLED (SSD1306)| VCC| 3.3V
OLED (SSD1306)| GND| GND
OLED (SSD1306)| SDA| GPIO 21
OLED (SSD1306)| SCL| GPIO 22
Relay Module| IN| GPIO 18
Relay Module| GND| GND
Relay Module| VCC| 5V

I²C Configuration

The BMP280 and OLED share the same I²C bus:

- SDA → GPIO 21
- SCL → GPIO 22
- BMP280 I²C Address → 0x77
- OLED I²C Address → 0x3C

Relay Configuration

The relay module used in the prototype is active-LOW:

- GPIO 18 LOW → Relay ON → Heater ON
- GPIO 18 HIGH → Relay OFF → Heater OFF

Temperature Control

The heater is controlled using a hysteresis-based temperature control system:

- Temperature ≤ 13°C → Heater ON
- Temperature ≥ 15°C → Heater OFF
- 13°C–15°C → Previous heater state is maintained

This hysteresis prevents rapid ON/OFF switching of the heater around the threshold temperature.