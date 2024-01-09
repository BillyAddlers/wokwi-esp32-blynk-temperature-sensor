#define BLYNK_TEMPLATE_ID           "TMPL6zGpSggSY"
#define BLYNK_TEMPLATE_NAME         "ESP32 Temperature and Humidity Sensor"
#define BLYNK_AUTH_TOKEN            "mYiIDM66DuCmGTzzK0iD0DZosQPl9l0B"
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHTesp.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int DHT_PIN = 15;

DHTesp Sensor;

char ssid[] = "Wokwi-GUEST";
char pass[] = "";

BlynkTimer timer;

BLYNK_WRITE(V0)
{
  int value = param.asInt();
  Blynk.virtualWrite(V1, value);
}

BLYNK_CONNECTED()
{
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

void myTimerEvent()
{
  Blynk.virtualWrite(V2, millis() / 1000);
}

void setup()
{
  lcd.init();
  lcd.backlight();
  lcd.setCursor(5, 0); lcd.print("Arduino");
  lcd.setCursor(2, 1); lcd.print("DHT22 Sensor!");
  Serial.begin(115200);
  Sensor.setup(DHT_PIN, DHTesp::DHT22);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(1000L, myTimerEvent);
}

void loop()
{
  Blynk.run();
  timer.run();
  TempAndHumidity data = Sensor.getTempAndHumidity();
  float h = data.humidity;
  float t = data.temperature;
  Serial.println("Temp: " + String(t, 2) + "°C");
  Serial.println("Humidity: " + String(h, 1) + "%");
  Serial.println("---");
  lcd.clear();
  lcd.setCursor(0, 0); lcd.print("T:");  lcd.print(String(t, 2));
  lcd.print(" C");
  lcd.setCursor(0, 1); lcd.print("H:");  lcd.print(String(h, 1));
  lcd.print(" %");
  delay(2000);
}
