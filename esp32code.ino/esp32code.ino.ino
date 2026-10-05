#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// =========================
// PINS
// =========================
#define SDA_PIN 21
#define SCL_PIN 22
#define RELAY_PIN 18

// =========================
// I2C ADDRESSES
// =========================
#define BMP280_ADDRESS 0x76
#define OLED_ADDRESS 0x3C

// =========================
// OLED
// =========================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

// =========================
// HEATER THRESHOLDS
// =========================
#define HEATER_ON_TEMP  35.0
#define HEATER_OFF_TEMP 40.0

Adafruit_BMP280 bmp;

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  OLED_RESET
);

bool heaterState = false;

void setup() {

  Serial.begin(115200);

  Wire.begin(SDA_PIN, SCL_PIN);

  // Relay active LOW
  pinMode(RELAY_PIN, OUTPUT);

  // Heater OFF at startup
  digitalWrite(RELAY_PIN, HIGH);


  // BMP280
  if (!bmp.begin(BMP280_ADDRESS)) {

    Serial.println("BMP280 NOT FOUND!");

    while (1) {
      digitalWrite(RELAY_PIN, HIGH);
      delay(1000);
    }
  }


  // OLED
  if (!display.begin(
        SSD1306_SWITCHCAPVCC,
        OLED_ADDRESS
      )) {

    Serial.println("OLED NOT FOUND!");

    while (1) {
      digitalWrite(RELAY_PIN, HIGH);
      delay(1000);
    }
  }


  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(2);
  display.setCursor(10, 10);
  display.println("READY");

  display.setTextSize(1);
  display.setCursor(15, 40);
  display.println("Sensor connected");

  display.display();

  delay(1500);
}


void loop() {

  // ==================================
  // SENSOR VALUES
  // ==================================

  float temperature = bmp.readTemperature();

  float pressure = bmp.readPressure() / 100.0F;


  // ==================================
  // SENSOR ERROR
  // ==================================

  if (isnan(temperature) || isnan(pressure)) {

    heaterState = false;

    // Safety: heater OFF
    digitalWrite(RELAY_PIN, HIGH);

    display.clearDisplay();

    display.setTextColor(SSD1306_WHITE);

    display.setTextSize(1);

    display.setCursor(20, 20);
    display.println("SENSOR ERROR!");

    display.setCursor(20, 40);
    display.println("HEATER OFF");

    display.display();

    delay(1000);

    return;
  }


  // ==================================
  // HEATER CONTROL
  // ==================================

  if (temperature <= HEATER_ON_TEMP) {

    heaterState = true;
  }

  if (temperature >= HEATER_OFF_TEMP) {

    heaterState = false;
  }


  // Active LOW relay

  if (heaterState) {

    digitalWrite(RELAY_PIN, LOW);

  } else {

    digitalWrite(RELAY_PIN, HIGH);
  }


  // ==================================
  // OLED
  // ==================================

  display.clearDisplay();

  display.setTextColor(SSD1306_WHITE);


  // Header
  display.setTextSize(1);

  display.setCursor(0, 0);
  display.println("SENSOR DATA");

  display.drawLine(
    0, 10,
    127, 10,
    SSD1306_WHITE
  );


  // ==================================
  // ACTUAL TEMPERATURE
  // ==================================

  display.setTextSize(2);

  display.setCursor(0, 17);

  display.print(temperature, 1);
  display.print(" C");


  // ==================================
  // ACTUAL PRESSURE
  // ==================================

  display.setTextSize(1);

  display.setCursor(0, 40);

  display.print("Pressure: ");

  display.print(pressure, 1);

  display.println(" hPa");


  // ==================================
  // RELAY STATUS
  // ==================================

  display.setCursor(0, 54);

  display.print("Heater: ");

  if (heaterState) {
    display.print("ON");
  } else {
    display.print("OFF");
  }


  display.display();


  // ==================================
  // SERIAL MONITOR
  // ==================================

  Serial.print("Temperature = ");
  Serial.print(temperature, 2);
  Serial.print(" C");

  Serial.print(" | Pressure = ");
  Serial.print(pressure, 2);
  Serial.print(" hPa");

  Serial.print(" | Heater = ");

  if (heaterState) {
    Serial.println("ON");
  } else {
    Serial.println("OFF");
  }


  delay(1000);
}