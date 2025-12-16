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
const unsigned long CO2_READ_INTERVAL = 60000; // 1000 milliseconds in a second

float co2Concentration = 0.0;
float temperature = 0.0;
float humidity = 0.0;

//The maximum CO2 allowed before the fan turns on.
float co2Max = 900.0; //1250
float co2Variation = 150.0;

//The minimum humidity allowed before the humidifier turns on.
float humidityMin = 70.0;
float humidityVariation = 15.0;

//Automatic or manual mode
bool autoOn = 0;

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
void calibrateCO2();

void setup()
{
  pinMode(PIN_FAN, OUTPUT);
  digitalWrite(PIN_FAN, HIGH);
  
  pinMode(PIN_HUMIDIFIER, OUTPUT);
  digitalWrite(PIN_HUMIDIFIER, HIGH);

  Serial.begin(115200);
  delay(100);

  BlynkEdgent.begin();


  initCO2();
}


int fanValueCurrently = -1;    // Read your fan value 
int fanValueLast = -1;            // Some value that's NOT possible for PIN_FAN at start
int humidifierValueLast = -1; 
int humidifierValueCurrently = -1;  

void loop() {
  BlynkEdgent.run();
  delay(10);

  // Non-blocking CO2 sensor reading - only read every 5 seconds
  unsigned long currentMillis = millis();
  if (currentMillis - lastCO2Read >= CO2_READ_INTERVAL) {
    lastCO2Read = currentMillis;
    readCO2();
  }

    // --- Serial command handling ---
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();  // remove \r and spaces

    if (cmd.equalsIgnoreCase("CALIBRATE")) {
      calibrateCO2();
    }
    // you can keep / add other commands here (FAN_ON, etc.)
  }

  if (autoOn){
    if (co2Concentration > co2Max) {
      fanValueCurrently = HIGH;
      digitalWrite(PIN_FAN, fanValueCurrently);
    }
    else
    {
      if (co2Concentration < co2Max- co2Variation){
        fanValueCurrently = LOW;
        digitalWrite(PIN_FAN, LOW);
        //Only turn on the humidifier if the fan is turned of.
        if (humidity < humidityMin){
          humidifierValueCurrently = HIGH;
          digitalWrite(PIN_HUMIDIFIER, humidifierValueCurrently);
        }
        else
        {
          if (humidity > humidityMin + humidityVariation){
            humidifierValueCurrently = LOW;
            digitalWrite(PIN_HUMIDIFIER, LOW);
          }
        }
      }
    }
  }


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
