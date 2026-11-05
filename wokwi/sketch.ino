
#include <WiFi.h>
#include <Wire.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <Adafruit_BMP085.h>
#include <DHTesp.h>

// ======================================
// WIFI CONFIGURATION
// ======================================

const char* WIFI_SSID = "Wokwi-GUEST";
const char* WIFI_PASSWORD = "";

// ======================================
// THINGSPEAK CONFIGURATION
// ======================================

const char* THINGSPEAK_URL =
  "https://api.thingspeak.com/update";

// Enter your actual ThingSpeak WRITE API KEY
const char* WRITE_API_KEY = "E7PJT1OKAOP4VA7B";

// ======================================
// SENSOR CONFIGURATION
// ======================================

#define SDA_PIN 21
#define SCL_PIN 22
#define DHT_PIN 15

Adafruit_BMP085 bmp;
DHTesp dhtSensor;

// ======================================
// TIMING CONFIGURATION
// ======================================

const unsigned long UPLOAD_INTERVAL = 16000;

unsigned long previousUpload = 0;

float previousPressure = -1.0;

// ======================================
// WIFI CONNECTION
// ======================================

bool connectWiFi() {

  if (WiFi.status() == WL_CONNECTED) {
    return true;
  }

  Serial.println("\nConnecting to WiFi...");

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD, 6);

  unsigned long startTime = millis();

  while (WiFi.status() != WL_CONNECTED &&
         millis() - startTime < 20000) {

    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {

    Serial.println("\nWiFi Connected!");

    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    return true;
  }

  Serial.println("\nWiFi Connection Failed!");

  Serial.print("WiFi Status: ");
  Serial.println(WiFi.status());

  return false;
}

// ======================================
// SENSOR INITIALIZATION
// ======================================

void initializeSensors() {

  Serial.println("\nInitializing Sensors...");

  // Initialize I2C communication
  Wire.begin(SDA_PIN, SCL_PIN);

  // Initialize BMP180
  if (!bmp.begin()) {

    Serial.println("ERROR: BMP180 Not Found!");

    while (true) {
      delay(1000);
    }
  }

  Serial.println("BMP180 Initialized!");

  // Initialize DHT22 on GPIO 15
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);

  Serial.println("DHT22 Configured!");

  // Allow sensor to stabilize
  delay(2000);
}

// ======================================
// READ SENSOR DATA
// ======================================

bool readSensors(
  float &temperature,
  float &humidity,
  float &pressure
) {

  // Read DHT22
  TempAndHumidity data =
    dhtSensor.getTempAndHumidity();

  temperature = data.temperature;
  humidity = data.humidity;

  // Read BMP180
  // Convert Pascals into hPa
  pressure = bmp.readPressure() / 100.0F;

  // Print sensor diagnostics
  Serial.println("\n--- SENSOR DIAGNOSTICS ---");

  Serial.print("DHT22 Status: ");
  Serial.println(dhtSensor.getStatusString());

  Serial.print("Temperature: ");
  Serial.println(temperature);

  Serial.print("Humidity: ");
  Serial.println(humidity);

  Serial.print("BMP180 Pressure: ");
  Serial.print(pressure);
  Serial.println(" hPa");

  // Check DHT22 communication
  if (dhtSensor.getStatus() != 0) {

    Serial.println("ERROR: DHT22 reading failed!");

    return false;
  }

  // Check invalid numerical values
  if (!isfinite(temperature) ||
      !isfinite(humidity) ||
      !isfinite(pressure)) {

    Serial.println("ERROR: Invalid sensor value!");

    return false;
  }

  // Validate sensor ranges
  if (temperature < -40 || temperature > 80 ||
      humidity < 0 || humidity > 100 ||
      pressure < 300 || pressure > 1100) {

    Serial.println("ERROR: Sensor value out of range!");

    return false;
  }

  Serial.println("All sensor readings valid!");

  return true;
}

// ======================================
// PRESSURE TREND ANALYSIS
// ======================================

void showPressureTrend(float pressure) {

  if (previousPressure < 0) {

    Serial.println("Pressure Trend: Initial Reading");

  } else {

    float difference =
      pressure - previousPressure;

    Serial.print("Pressure Change: ");
    Serial.print(difference, 2);
    Serial.println(" hPa");

    if (difference > 0.10) {

      Serial.println("Pressure is Rising");

    } else if (difference < -0.10) {

      Serial.println("Pressure is Falling");

    } else {

      Serial.println("Pressure is Stable");
    }
  }

  previousPressure = pressure;
}

// ======================================
// UPLOAD DATA TO THINGSPEAK
// ======================================

void uploadToCloud(
  float temperature,
  float humidity,
  float pressure
) {

  if (WiFi.status() != WL_CONNECTED) {

    Serial.println("WiFi Disconnected. Upload Skipped!");

    return;
  }

  WiFiClientSecure secureClient;

  // For Wokwi simulation only.
  // Production systems should validate server certificates.
  secureClient.setInsecure();

  HTTPClient http;

  if (!http.begin(secureClient, THINGSPEAK_URL)) {

    Serial.println("HTTPS Initialization Failed!");

    return;
  }

  http.setTimeout(15000);

  http.addHeader(
    "Content-Type",
    "application/x-www-form-urlencoded"
  );

  // ThingSpeak channel fields:
  // Field 1 = Temperature
  // Field 2 = Humidity
  // Field 3 = Atmospheric Pressure

  String payload =
    "api_key=" + String(WRITE_API_KEY) +
    "&field1=" + String(temperature, 2) +
    "&field2=" + String(humidity, 2) +
    "&field3=" + String(pressure, 2);

  Serial.println("\nUploading to ThingSpeak...");

  int responseCode = http.POST(payload);

  if (responseCode == HTTP_CODE_OK) {

    String response = http.getString();

    response.trim();

    long entryID = response.toInt();

    if (entryID > 0) {

      Serial.println("CLOUD UPLOAD SUCCESSFUL!");

      Serial.print("Entry ID: ");
      Serial.println(entryID);

    } else {

      Serial.println("ThingSpeak Rejected Update!");

      Serial.println("Response: " + response);
    }

  } else {

    Serial.print("HTTP Error: ");
    Serial.println(responseCode);

    if (responseCode < 0) {

      Serial.println(
        http.errorToString(responseCode)
      );
    }
  }

  http.end();
}

// ======================================
// SETUP
// ======================================

void setup() {

  Serial.begin(115200);

  delay(1000);

  Serial.println("\n================================");
  Serial.println(" LOW-COST WEATHER STATION");
  Serial.println(" ESP32 + BMP180 + DHT22");
  Serial.println(" THINGSPEAK CLOUD MONITORING");
  Serial.println("================================");

  // Initialize sensors
  initializeSensors();

  // Connect to WiFi
  connectWiFi();

  Serial.println("\nSystem Ready!");
}

// ======================================
// MAIN LOOP
// ======================================

void loop() {

  // Reconnect if WiFi disconnects
  if (WiFi.status() != WL_CONNECTED) {

    connectWiFi();
  }

  // Upload data every 16 seconds
  if (millis() - previousUpload >= UPLOAD_INTERVAL) {

    float temperature;
    float humidity;
    float pressure;

    Serial.println("\n--------------------------------");

    // Read sensor values
    if (readSensors(
      temperature,
      humidity,
      pressure
    )) {

      Serial.println("\nWEATHER MONITORING DATA");

      Serial.print("Temperature: ");
      Serial.print(temperature, 2);
      Serial.println(" °C");

      Serial.print("Humidity: ");
      Serial.print(humidity, 2);
      Serial.println(" %");

      Serial.print("Atmospheric Pressure: ");
      Serial.print(pressure, 2);
      Serial.println(" hPa");

      // Analyze pressure trend
      showPressureTrend(pressure);

      // Upload readings to ThingSpeak
      uploadToCloud(
        temperature,
        humidity,
        pressure
      );

    } else {

      Serial.println("Sensor reading failed!");

      Serial.println("Cloud upload skipped.");
    }

    previousUpload = millis();

    Serial.println("--------------------------------");
  }

  delay(100);
}
