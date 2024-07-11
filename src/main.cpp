#include <Arduino.h>
#include <Wire.h>

#include <WiFi.h>
const char* ssid = "ESP32";
const char* password = "kloudtech";

#include <WebServer.h>
#include <PageIndex.h>
WebServer server(80);  // Web server on port 80

// Sleep Factors
#define uS_TO_S_FACTOR 1000000ULL /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP 60          /* Time ESP32 will go to sleep (in seconds) */

// BME
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#define BME_SDA 21
#define BME_SCL 22
Adafruit_BME280 bme;
float t1 = 0;
float h1 = 0;
float p1 = 0;

// BMP
#include <Adafruit_BMP085.h>
Adafruit_BMP085 bmp;
float t2;
float p2;

// DHT22 Library
#include <DHT.h>
#define DHTPIN 04
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
float h2;

// AS5600 Library
int magnetStatus = 0; // value of the status register (MD, ML, MH)

int lowbyte;    // raw angle 7:0
word highbyte;  // raw angle 7:0 and 11:8
int rawAngle;   // final raw angle
float degAngle; // raw angle in degrees (360/4096 * [value between 0-4095])

int quadrantNumber, previousquadrantNumber; // quadrant IDs
float numberofTurns = 0;                    // number of turns
float correctedAngle = 0;                   // tared angle - based on the startup value
float startAngle = 0;                       // starting angle
float totalAngle = 0;                       // total absolute angular displacement
float previoustotalAngle = 0;               // for the display printing

// BH1750 Library
#include <BH1750.h>
BH1750 lightMeter;
float lux = 0;
float irradiance = 0;

// UV Library
#define UVPIN 32
float sensorVoltage;
float sensorValue;
int UV_index;

// Slave Library
#define SLAVE 0x03
#define countof(a) (sizeof(a) / sizeof(a[0]))

// Precipitation
float tipValue = 0.1099, rain;
uint16_t receivedRainCount;
int currentRainCount;
RTC_DATA_ATTR int prevRainCount;

// Wind Speed
float windspeed;
int REV, radius = 100, period = TIME_TO_SLEEP;
uint16_t receivedWindCount;
int currentWindCount;
RTC_DATA_ATTR int prevWindCount;

void handleRoot() {
  String s = MAIN_page; // Read HTML contents
  server.send(200, "text/html", s); // Send web page
}

// Sensor readings
void handleBMETemperature() {
    t1 = bme.readTemperature();
    String Temperature1_Value = String(t1);
    server.send(200, "text/plain", Temperature1_Value); // Send Temperature value only to client ajax request

    Serial.print("Temperature 1: ");
    Serial.print(t1);
    Serial.println(" °C");
}

void handleBMEHumidity() {
    h1 = bme.readHumidity();
    String Humidity1_Value = String(h1);
    server.send(200, "text/plain", Humidity1_Value); // Send Humidity value only to client ajax request

    Serial.print("Humidity 1: ");
    Serial.print(h1);
    Serial.println(" %");
}

void handleBMEPressure() {
    p1 = bme.readPressure() / 100;
    String Pressure1_Value = String(p1);
    server.send(200, "text/plain", Pressure1_Value);


    Serial.print("Pressure 1: ");
    Serial.print(p1);
    Serial.println(" mb");
}

void handleBMPTemperature() {
    t2 = bmp.readTemperature();
    String Temperature2_Value = String(t2);
    server.send(200, "text/plain", Temperature2_Value);
    
    Serial.print("Temperature 2: ");
    Serial.print(t2);
    Serial.println(" °C");
}

void handleBMPPressure() {
    p2 = bmp.readPressure() / 100;
    String Pressure2_Value = String(p2);
    server.send(200, "text/plain", Pressure2_Value);

    Serial.print("Pressure 2: ");
    Serial.print(p2);
    Serial.println(" mb");
}

void handleDHTHumidity() {
    h2 = dht.readHumidity();
    String Humidity2_Value = String(h2);
    server.send(200, "text/plain", Humidity2_Value);

    Serial.print("Humidity 2: ");
    Serial.print(h2);
    Serial.println(" %");
}

void handleWindDirection() {
    Wire.beginTransmission(0x36); // connect to the sensor
    Wire.write(0x0D);             // figure 21 - register map: Raw angle (7:0)
    Wire.endTransmission();       // end transmission
    Wire.requestFrom(0x36, 1);    // request from the sensor

    while (Wire.available() == 0)
        ;                    // wait until it becomes available
    lowbyte = Wire.read(); // Reading the data after the request

    // 11:8 - 4 bits
    Wire.beginTransmission(0x36);
    Wire.write(0x0C); // figure 21 - register map: Raw angle (11:8)
    Wire.endTransmission();
    Wire.requestFrom(0x36, 1);

    while (Wire.available() == 0)
        ;
    highbyte = Wire.read();

    // 4 bits have to be shifted to its proper place as we want to build a 12-bit number
    highbyte = highbyte << 8;      // shifting to left
    rawAngle = highbyte | lowbyte; // int is 16 bits (as well as the word)
    degAngle = rawAngle * 0.087890625;

    // recalculate angle
    correctedAngle = degAngle - startAngle; // this tares the position

    if (correctedAngle < 0) // if the calculated angle is negative, we need to "normalize" it
    {
        correctedAngle = correctedAngle + 360; // correction for negative numbers (i.e. -15 becomes +345)
    }

    String WindDirection_Value = String(correctedAngle);
    server.send(200, "text/plain", WindDirection_Value);

    Serial.print("Wind Direction: ");
    Serial.print(correctedAngle);
    Serial.println(" °");
}

void handleLight() {
    lux = lightMeter.readLightLevel();
    String Light_Value = String(lux);
    server.send(200, "text/plain", Light_Value);

    Serial.print("Light Intensity: ");
    Serial.print(lux);
    Serial.println(" lux");
}

void handleUV() {
    // sensorValue = analogRead(UVPIN);
    // sensorVoltage = sensorValue * (3.3 / 4095);
    // UV_index = sensorVoltage / 0.1;
    // String UV_Value = String(UV_index);
    // server.send(200, "text/plain", UV_Value);

    UV_index = 0;
    String UV_Value = String(UV_index);
    server.send(200, "text/plain", UV_Value);

    Serial.print("UV Intensity: ");
    Serial.print(UV_index);
    Serial.println(" mW/cm2");
}

void handlePrecipitation() {
    while (2 < Wire.available()) {
        byte msb = Wire.read();
        byte lsb = Wire.read();
        receivedRainCount = (msb << 8) | lsb;
    }
    currentRainCount = receivedRainCount;
    if ((currentRainCount - prevRainCount) > -1) {
        rain = (currentRainCount - prevRainCount) * tipValue;
    }
    else {
        rain = (65535 + currentRainCount - prevRainCount) * tipValue;
    }
    prevRainCount = currentRainCount;
    String Precipitation_Value = String(rain);
    server.send(200, "text/plain", Precipitation_Value);

    Serial.print("Precipitation: ");
    Serial.print(rain);
    Serial.println(" mm");
}

void handleWindSpeed() {
    while (Wire.available()) {
        byte msb = Wire.read();
        byte lsb = Wire.read();
        receivedWindCount = (msb << 8) | lsb;
    }
    currentWindCount = receivedWindCount;
    if ((currentWindCount - prevWindCount) > -1) {
        REV = (currentWindCount - prevWindCount);
    }
    else {
        REV = (65355 + currentWindCount - prevWindCount);
    }
    windspeed = (2 * PI * radius * REV * 3.6) / (period * 1000);
    prevWindCount = currentWindCount;
    String WindSpeed_Value = String(windspeed);
    server.send(200, "text/plain", WindSpeed_Value);

    Serial.print("Wind Speed: ");
    Serial.print(windspeed);
    Serial.println(" kph");
}

//----------------------------------------Setup
void setup(void) {
  // Initialize Serial Monitor
    Serial.begin(115200);
    Serial.println("");

    // Set-up Access Point
    Serial.println("Setting AP...");
    WiFi.softAP(ssid, password);
    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);

    // Initialize Wire I2C
    Wire.begin(21, 22);

    // Initialize BME
    if (!bme.begin(0x76)) {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        while (1);
    }
    else {
        Serial.println("BME Initiation Complete");
    }
    delay(10);

    // Initialize BMP
    if (!bmp.begin()) {
        Serial.println("Could not find a valid BMP180 sensor, check wiring!");
        while (1);
    }
    else {
        Serial.println("BMP Initiation Complete");
    }
    delay(10);

    // Initialize DHT
    dht.begin();
    if (isnan(h2)) {
        Serial.println("Could not find a valid DHT22 sensor, check wiring!");
    }
    else {
        Serial.println("DHT Initiation Complete");
    }
    delay(10);

    // Initialize Light sensor
    if (!lightMeter.begin()) {
        Serial.println("Could not find a valid BH1750 sensor, check wiring!");
    }
    else {
        Serial.println("BH1750 Initiation Complete");
    }

    // // Initialize UV
    // if (isnan(sensorValue)) {
    //     Serial.println("Could not find a valid UV sensor, check wiring!");
    // }
    // else {
    //     Serial.println("UV Initiation Complete");
    // }

    // Initialize Slave
    if (!Wire.begin()) {
        Serial.println("Could not find a valid Slave Device, check wiring!");
    }
    else {
        Serial.println("Slave Initiation Complete");
    }
    Wire.requestFrom(SLAVE, 4);

    // Connect to WiFi
    // WiFi.begin(ssid, password);
    // Serial.println("");
    // Serial.print("Connecting");
    // while (WiFi.status() != WL_CONNECTED) {
    //     Serial.print(".");
    //     delay(250);
    // }
    // Serial.println("");
    // Serial.print("Successfully connected to : ");
    // Serial.println(ssid);
    // Serial.print("IP address: ");
    // Serial.println(WiFi.localIP());

    // Setup Web server routes
    server.on("/", handleRoot);
    server.on("/readBMETemperature", handleBMETemperature);
    server.on("/readBMEHumidity", handleBMEHumidity);
    server.on("/readBMEPressure", handleBMEPressure);
    server.on("/readBMPTemperature", handleBMPTemperature);
    server.on("/readBMPPressure", handleBMPPressure);
    server.on("/readDHTHumidity", handleDHTHumidity);
    server.on("/readWindDirection", handleWindDirection);
    server.on("/readLight", handleLight);
    server.on("/readUV", handleUV);
    server.on("/readPrecipitation", handlePrecipitation);
    server.on("/readWindSpeed", handleWindSpeed);

    // Start server
    server.begin();
    Serial.println("HTTP server started");
}

//----------------------------------------Loop
void loop() {
  server.handleClient();  // Handle client requests
}
