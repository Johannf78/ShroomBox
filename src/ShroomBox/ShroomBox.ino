/*************************************************************
  Blynk is a platform with iOS and Android apps to control
  ESP32, Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build mobile and web interfaces for any
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: https://www.blynk.io
    Sketch generator:           https://examples.blynk.cc
    Blynk community:            https://community.blynk.cc
    Follow us:                  https://www.fb.com/blynkapp
                                https://twitter.com/blynk_app

  Blynk library is licensed under MIT license
 *************************************************************
  Blynk.Edgent implements:
  - Blynk.Inject - Dynamic WiFi credentials provisioning
  - Blynk.Air    - Over The Air firmware updates
  - Device state indication using a physical LED
  - Credentials reset using a physical Button
 *************************************************************/

/* Fill in information from your Blynk Template here */
/* Read more: https://bit.ly/BlynkInject */
#define BLYNK_TEMPLATE_ID "TMPL4JzPZ45yp"
#define BLYNK_TEMPLATE_NAME "ShroomBox"

#define BLYNK_FIRMWARE_VERSION        "0.1.2"

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG

#define APP_DEBUG

#define USE_ESP32_DEV_MODULE


#include "WiFi.h"
#include <Wire.h>
#include "BlynkEdgent.h"
#include <Preferences.h>

#define PIN_FAN 25
#define PIN_HUMIDIFIER 26
#define PIN_LED 14
#define PIN_HEATER 12

// SCD30 CO2, Humidity and temperature sensor
//https://sensirion.com/products/catalog/SCD30
#include <SensirionI2cScd30.h>
#include <Wire.h>

//I2C CO2 sensor communication:                         ESP32DevKitC
//SCL I2C: Serial clock input           Brown         	GPIO 22
//SDA I2C: Serial data input / output   BrownWhite      GPIO 21

// macro definitions
// make sure that we use the proper definition of NO_ERROR
#ifdef NO_ERROR
  #undef NO_ERROR
#endif
#define NO_ERROR 0

SensirionI2cScd30 sensor;

static char errorMessage[64];
static int16_t error;

// Non-blocking timer for CO2 sensor readings
unsigned long lastCO2Read = 0;
const unsigned long CO2_READ_INTERVAL = 5000; // 1000 milliseconds in a second
unsigned long lastBlynkUpdate = 0;
const unsigned long BLYNK_UPDATE_INTERVAL = 60000; // 1000 milliseconds in a second


float co2Concentration = 0.0;
float temperature = 0.0;
float humidity = 0.0;

//The maximum CO2 allowed before the fan turns on.
float co2Max = 950; //2850; //2300; //900.0; //1250
float co2Variation = 100.0;

//The minimum humidity allowed before the humidifier turns on.
float humidityMin = 70.0;
float humidityVariation = 15.0;

// Preferences for threshold persistence
Preferences thresholdPrefs;
const char* THRESHOLD_NAMESPACE = "thresholds";

//Automatic or manual mode, Default to AutoOn =1
bool autoOn = 1;

BLYNK_WRITE(V0)
{
  int value = param.asInt();

  if (value == 1) {
    digitalWrite(PIN_FAN, HIGH);
    Serial.print("PIN_FAN =");
    Serial.println(value);
  } else {
    digitalWrite(PIN_FAN, LOW);
    Serial.print("PIN_FAN = ");
    Serial.println(value);
  }
}
BLYNK_WRITE(V4)
{
  int value = param.asInt();

  if (value == 1) {
    digitalWrite(PIN_HUMIDIFIER, HIGH);
    Serial.print("PIN_HUMIDIFIER =");
    Serial.println(value);
  } else {
    digitalWrite(PIN_HUMIDIFIER, LOW);
    Serial.print("PIN_HUMIDIFIER = ");
    Serial.println(value);
  }
}

BLYNK_WRITE(V5)
{
  int value = param.asInt();

  if (value == 1) {
    autoOn = true;
    Serial.print("autoOn =");
    Serial.println(value);
  } else {
    autoOn = false;
    Serial.print("autoOn = ");
    Serial.println(value);
  }
}

//Function prototypes, it needs to be here because it is used in the setup function.
//one needs to add a forward declaration for these functions, as it is defined in a seperate .ino file:func_co2_sensor.ino
void initCO2();
void readCO2();
void calibrateCO2(uint16_t refPpm = 400);
void handleSerialCommands();  // Forward declaration for serial command handler
void loadThresholds();  // Load thresholds from flash
void saveThresholds();  // Save thresholds to flash

void setup()
{
  pinMode(PIN_FAN, OUTPUT);
  digitalWrite(PIN_FAN, HIGH);
  
  pinMode(PIN_HUMIDIFIER, OUTPUT);
  digitalWrite(PIN_HUMIDIFIER, HIGH);

  Serial.begin(115200);
  delay(100);

  // Load saved thresholds from flash
  loadThresholds();

  BlynkEdgent.begin();


  initCO2();
}



int fanValueCurrently = -1;    // Read your fan value 
int fanValueLast = -1;            // Some value that's NOT possible for PIN_FAN at start
int humidifierValueLast = -1; 
int humidifierValueCurrently = -1;

// Threshold persistence functions
void loadThresholds() {
  thresholdPrefs.begin(THRESHOLD_NAMESPACE, true);  // Read-only mode
  
  // Load thresholds, use defaults if not found
  co2Max = thresholdPrefs.getFloat("co2Max", 950.0);
  co2Variation = thresholdPrefs.getFloat("co2Var", 100.0);
  humidityMin = thresholdPrefs.getFloat("humMin", 70.0);
  humidityVariation = thresholdPrefs.getFloat("humVar", 15.0);
  
  thresholdPrefs.end();
  
  Serial.println("=== Loaded Thresholds from Flash ===");
  Serial.print("CO2 Max: ");
  Serial.print(co2Max);
  Serial.println(" ppm");
  Serial.print("CO2 Variation: ");
  Serial.print(co2Variation);
  Serial.println(" ppm");
  Serial.print("Humidity Min: ");
  Serial.print(humidityMin);
  Serial.println(" %");
  Serial.print("Humidity Variation: ");
  Serial.print(humidityVariation);
  Serial.println(" %");
  Serial.println("===================================");
}

void saveThresholds() {
  thresholdPrefs.begin(THRESHOLD_NAMESPACE, false);  // Read-write mode
  
  thresholdPrefs.putFloat("co2Max", co2Max);
  thresholdPrefs.putFloat("co2Var", co2Variation);
  thresholdPrefs.putFloat("humMin", humidityMin);
  thresholdPrefs.putFloat("humVar", humidityVariation);
  
  thresholdPrefs.end();
  
  Serial.println("Thresholds saved to flash memory");
}  


void handleSerialCommands()
{
  // --- Serial command handling ---
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();  // remove \r and spaces

    // Help command - display all available commands
    if (cmd.equalsIgnoreCase("help") || cmd.equalsIgnoreCase("?")) {
      Serial.println("\n========================================");
      Serial.println("     ShroomBox Serial Commands");
      Serial.println("========================================");
      Serial.println("\n--- CO2 Sensor ---");
      Serial.println("  CALIBRATE [ppm]  - Calibrate CO2 sensor (default: 400 ppm)");
      Serial.println("                    Example: CALIBRATE 420");
      Serial.println("\n--- Manual Control ---");
      Serial.println("  f1               - Turn fan ON");
      Serial.println("  f0               - Turn fan OFF");
      Serial.println("  h1               - Turn humidifier ON");
      Serial.println("  h0               - Turn humidifier OFF");
      Serial.println("  a1               - Enable auto mode");
      Serial.println("  a0               - Disable auto mode");
      Serial.println("\n--- Threshold Configuration ---");
      Serial.println("  SET_CO2_MAX <val> - Set CO2 max threshold (400-5000 ppm)");
      Serial.println("                      Example: SET_CO2_MAX 1000");
      Serial.println("  SET_CO2_VAR <val> - Set CO2 variation/hysteresis (50-500 ppm)");
      Serial.println("                      Example: SET_CO2_VAR 150");
      Serial.println("  SET_HUM_MIN <val> - Set humidity min threshold (0-100 %)");
      Serial.println("                      Example: SET_HUM_MIN 75");
      Serial.println("  SET_HUM_VAR <val> - Set humidity variation (5-50 %)");
      Serial.println("                      Example: SET_HUM_VAR 20");
      Serial.println("  GET_THRESHOLDS    - Display current threshold values");
      Serial.println("  RESET_THRESHOLDS  - Reset thresholds to defaults");
      Serial.println("\n--- Information ---");
      Serial.println("  HELP or ?        - Display this help message");
      Serial.println("\n========================================\n");
      return;  // Exit early after displaying help
    }

    if (cmd.startsWith("CALIBRATE") || cmd.startsWith("calibrate")) {
      uint16_t refPpm = 400;  // default value
      int spaceIndex = cmd.indexOf(' ');
      if (spaceIndex > 0) {
        refPpm = cmd.substring(spaceIndex + 1).toInt();
      }
      Serial.print("Calibrating to: ");
      Serial.print(refPpm);
      Serial.println(" ppm");
      calibrateCO2(refPpm);
    }
    if (cmd.equalsIgnoreCase("a1")) {
      autoOn = true;
      Serial.print("autoOn = ");
      Serial.println(autoOn);
      if (Blynk.connected()) {
        Blynk.virtualWrite(V5, 1);
      }
    }
    if (cmd.equalsIgnoreCase("a0")) {
      autoOn = false;
      Serial.print("autoOn = ");
      Serial.println(autoOn);
      if (Blynk.connected()) {
        Blynk.virtualWrite(V5, 0);
      }
    }
    if (cmd.equalsIgnoreCase("f1")) {
      fanValueCurrently = HIGH;
      digitalWrite(PIN_FAN, fanValueCurrently);
      Serial.print("fanValueCurrently = ");
      Serial.println(fanValueCurrently);
      if (Blynk.connected()) {
        Blynk.virtualWrite(V0, fanValueCurrently);
      }
    }
    if (cmd.equalsIgnoreCase("f0")) {
      fanValueCurrently = LOW;
      digitalWrite(PIN_FAN, fanValueCurrently);
      Serial.print("fanValueCurrently = ");
      Serial.println(fanValueCurrently);
      if (Blynk.connected()) {
        Blynk.virtualWrite(V0, fanValueCurrently);
      }
    }
    if (cmd.equalsIgnoreCase("h1")) {
      humidifierValueCurrently = HIGH;
      digitalWrite(PIN_HUMIDIFIER, humidifierValueCurrently);
      Serial.print("humidifierValueCurrently = ");
      Serial.println(humidifierValueCurrently);
      if (Blynk.connected()) {
        Blynk.virtualWrite(V4, humidifierValueCurrently);
      }
    }
    if (cmd.equalsIgnoreCase("h0")) {
      humidifierValueCurrently = LOW;
      digitalWrite(PIN_HUMIDIFIER, humidifierValueCurrently);
      Serial.print("humidifierValueCurrently = ");
      Serial.println(humidifierValueCurrently);
      if (Blynk.connected()) {
        Blynk.virtualWrite(V4, humidifierValueCurrently);
      }
    }
    
    // Threshold configuration commands
    if (cmd.startsWith("SET_CO2_MAX") || cmd.startsWith("set_co2_max")) {
      int spaceIndex = cmd.indexOf(' ');
      if (spaceIndex > 0) {
        float newValue = cmd.substring(spaceIndex + 1).toFloat();
        if (newValue >= 400 && newValue <= 5000) {
          co2Max = newValue;
          saveThresholds();  // Save to flash
          Serial.print("CO2 Max threshold set to: ");
          Serial.print(co2Max);
          Serial.println(" ppm (saved)");
        } else {
          Serial.println("Error: CO2 Max must be between 400 and 5000 ppm");
        }
      } else {
        Serial.println("Usage: SET_CO2_MAX <value>");
      }
    }
    
    if (cmd.startsWith("SET_CO2_VAR") || cmd.startsWith("set_co2_var")) {
      int spaceIndex = cmd.indexOf(' ');
      if (spaceIndex > 0) {
        float newValue = cmd.substring(spaceIndex + 1).toFloat();
        if (newValue >= 50 && newValue <= 500) {
          co2Variation = newValue;
          saveThresholds();  // Save to flash
          Serial.print("CO2 Variation set to: ");
          Serial.print(co2Variation);
          Serial.println(" ppm (saved)");
        } else {
          Serial.println("Error: CO2 Variation must be between 50 and 500 ppm");
        }
      } else {
        Serial.println("Usage: SET_CO2_VAR <value>");
      }
    }
    
    if (cmd.startsWith("SET_HUM_MIN") || cmd.startsWith("set_hum_min")) {
      int spaceIndex = cmd.indexOf(' ');
      if (spaceIndex > 0) {
        float newValue = cmd.substring(spaceIndex + 1).toFloat();
        if (newValue >= 0 && newValue <= 100) {
          humidityMin = newValue;
          saveThresholds();  // Save to flash
          Serial.print("Humidity Min threshold set to: ");
          Serial.print(humidityMin);
          Serial.println(" % (saved)");
        } else {
          Serial.println("Error: Humidity Min must be between 0 and 100 %");
        }
      } else {
        Serial.println("Usage: SET_HUM_MIN <value>");
      }
    }
    
    if (cmd.startsWith("SET_HUM_VAR") || cmd.startsWith("set_hum_var")) {
      int spaceIndex = cmd.indexOf(' ');
      if (spaceIndex > 0) {
        float newValue = cmd.substring(spaceIndex + 1).toFloat();
        if (newValue >= 5 && newValue <= 50) {
          humidityVariation = newValue;
          saveThresholds();  // Save to flash
          Serial.print("Humidity Variation set to: ");
          Serial.print(humidityVariation);
          Serial.println(" % (saved)");
        } else {
          Serial.println("Error: Humidity Variation must be between 5 and 50 %");
        }
      } else {
        Serial.println("Usage: SET_HUM_VAR <value>");
      }
    }
    
    if (cmd.equalsIgnoreCase("GET_THRESHOLDS") || cmd.equalsIgnoreCase("THRESHOLDS")) {
      Serial.println("=== Current Thresholds ===");
      Serial.print("CO2 Max: ");
      Serial.print(co2Max);
      Serial.println(" ppm (Fan turns ON above this)");
      Serial.print("CO2 Variation: ");
      Serial.print(co2Variation);
      Serial.print(" ppm (Fan turns OFF at ");
      Serial.print(co2Max - co2Variation);
      Serial.println(" ppm)");
      Serial.print("Humidity Min: ");
      Serial.print(humidityMin);
      Serial.println(" % (Humidifier turns ON below this)");
      Serial.print("Humidity Variation: ");
      Serial.print(humidityVariation);
      Serial.print(" % (Humidifier turns OFF at ");
      Serial.print(humidityMin + humidityVariation);
      Serial.println(" %)");
      Serial.println("========================");
    }
    
    if (cmd.equalsIgnoreCase("RESET_THRESHOLDS")) {
      co2Max = 950.0;
      co2Variation = 100.0;
      humidityMin = 70.0;
      humidityVariation = 15.0;
      saveThresholds();  // Save defaults to flash
      Serial.println("Thresholds reset to defaults (saved):");
      Serial.println("CO2 Max: 950 ppm, CO2 Var: 100 ppm");
      Serial.println("Humidity Min: 70 %, Humidity Var: 15 %");
    }
  }
}


void loop() {
  BlynkEdgent.run();
  delay(10);

  // Non-blocking CO2 sensor reading - only read every 5 seconds
  unsigned long currentMillis = millis();
  if (currentMillis - lastCO2Read >= CO2_READ_INTERVAL) {
    lastCO2Read = currentMillis;
    Serial.println("Reading CO2 sensor...");
    readCO2();
    
    // Automatic control logic (only runs when auto mode is enabled)
    if (autoOn) {
      Serial.println("Auto mode: ON");
      
      // Fan control based on CO2 with hysteresis
      float co2TurnOffThreshold = co2Max - co2Variation;
      
      if (co2Concentration > co2Max) {
        // CO2 above max threshold - turn fan ON
        Serial.print("CO2 > max threshold (");
        Serial.print(co2Max);
        Serial.println(" ppm) - Fan ON");
        fanValueCurrently = HIGH;
        digitalWrite(PIN_FAN, fanValueCurrently);
      }
      else if (co2Concentration <= co2TurnOffThreshold) {
        // CO2 at or below turn-off threshold - turn fan OFF
        Serial.print("CO2 <= turn-off threshold (");
        Serial.print(co2TurnOffThreshold);
        Serial.println(" ppm) - Fan OFF");
        fanValueCurrently = LOW;
        digitalWrite(PIN_FAN, LOW);
        
        // Only control humidifier when fan is OFF
        if (humidity < humidityMin) {
          Serial.println("Humidity < min - Humidifier ON");
          humidifierValueCurrently = HIGH;
          digitalWrite(PIN_HUMIDIFIER, humidifierValueCurrently);
        }
        else if (humidity > (humidityMin + humidityVariation)) {
          Serial.println("Humidity > max - Humidifier OFF");
          humidifierValueCurrently = LOW;
          digitalWrite(PIN_HUMIDIFIER, LOW);
        }
      }
      else {
        // CO2 is in hysteresis zone (between turn-off and max threshold)
        // Maintain current fan state to prevent rapid switching
        // However, if CO2 drops below co2Max and fan is ON, turn it OFF
        // (This handles threshold changes while maintaining hysteresis)
        if (fanValueCurrently == HIGH && co2Concentration < co2Max) {
          // Fan was ON but CO2 dropped below max - turn OFF
          Serial.print("CO2 dropped below max (");
          Serial.print(co2Max);
          Serial.println(" ppm) - Fan OFF");
          fanValueCurrently = LOW;
          digitalWrite(PIN_FAN, LOW);
          
          // Only control humidifier when fan is OFF
          if (humidity < humidityMin) {
            Serial.println("Humidity < min - Humidifier ON");
            humidifierValueCurrently = HIGH;
            digitalWrite(PIN_HUMIDIFIER, humidifierValueCurrently);
          }
          else if (humidity > (humidityMin + humidityVariation)) {
            Serial.println("Humidity > max - Humidifier OFF");
            humidifierValueCurrently = LOW;
            digitalWrite(PIN_HUMIDIFIER, LOW);
          }
        }
        else {
          // Maintain current state in hysteresis zone
          Serial.print("CO2 in hysteresis zone (");
          Serial.print(co2TurnOffThreshold);
          Serial.print(" - ");
          Serial.print(co2Max);
          Serial.print(" ppm, current: ");
          Serial.print(co2Concentration);
          Serial.print(") - Fan remains ");
          Serial.println(fanValueCurrently == HIGH ? "ON" : "OFF");
        }
      }
    }
  }

  if (currentMillis - lastBlynkUpdate >= BLYNK_UPDATE_INTERVAL) {
    lastBlynkUpdate = currentMillis;
    
    // Send CO2 concentration to Blynk if connected
    if (Blynk.connected()) {
        Blynk.virtualWrite(V1, temperature);
        Blynk.virtualWrite(V2, humidity);
        Blynk.virtualWrite(V3, co2Concentration);
        Serial.println("Blynk Updated");
    }
  }
  
  handleSerialCommands();
  
  

  //If value changed, update Blynk. So, only update Blynk if the value has changed.
  if (fanValueCurrently != fanValueLast) {
    Blynk.virtualWrite(V0, fanValueCurrently);
    fanValueLast = fanValueCurrently;
  }

  if (humidifierValueCurrently != humidifierValueLast) {
    Blynk.virtualWrite(V4, humidifierValueCurrently);
    humidifierValueLast = humidifierValueCurrently;
  }

}
