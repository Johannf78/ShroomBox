
void initCO2()
{

 Wire.begin();
    sensor.begin(Wire, SCD30_I2C_ADDR_61);

    sensor.stopPeriodicMeasurement();
    sensor.softReset();
    delay(2000);
    uint8_t major = 0;
    uint8_t minor = 0;
    error = sensor.readFirmwareVersion(major, minor);
    if (error != NO_ERROR) {
        Serial.print("Error trying to execute readFirmwareVersion(): ");
        errorToString(error, errorMessage, sizeof errorMessage);
        Serial.println(errorMessage);
        return;
    }
    Serial.print("major: ");
    Serial.print(major);
    Serial.print("\t");
    Serial.print("minor: ");
    Serial.print(minor);
    Serial.println();
    error = sensor.startPeriodicMeasurement(0);
    if (error != NO_ERROR) {
        Serial.print("Error trying to execute startPeriodicMeasurement(): ");
        errorToString(error, errorMessage, sizeof errorMessage);
        Serial.println(errorMessage);
        return;
    }

    // DISABLE Automatic Self-Calibration
    disableASC();
}

void disableASC() {
  Serial.println("Disabling Automatic Self-Calibration...");
  
  // Stop continuous measurement first
  error = sensor.stopPeriodicMeasurement();
  if (error != NO_ERROR) {
    Serial.print("Error stopping measurement: ");
    errorToString(error, errorMessage, sizeof errorMessage);
    Serial.println(errorMessage);
    return;
  }
  
  delay(500); // Give sensor time to stop
  
  // Disable ASC - 0 = disabled, 1 = enabled
  error = sensor.activateAutoCalibration(0);
  if (error != NO_ERROR) {
    Serial.print("Error disabling ASC: ");
    errorToString(error, errorMessage, sizeof errorMessage);
    Serial.println(errorMessage);
    return;
  }
  
  delay(100);
  
  // Restart measurement with ambient pressure (0 = auto)
  error = sensor.startPeriodicMeasurement(0);
  if (error != NO_ERROR) {
    Serial.print("Error restarting measurement: ");
    errorToString(error, errorMessage, sizeof errorMessage);
    Serial.println(errorMessage);
    return;
  }
  
  Serial.println("✓ ASC successfully DISABLED!");
  Serial.println("Sensor configured for mushroom chamber use");
}

void readCO2() {
  

    delay(1500);
    error = sensor.blockingReadMeasurementData(co2Concentration, temperature,
                                               humidity);
    if (error != NO_ERROR) {
        Serial.print("Error trying to execute blockingReadMeasurementData(): ");
        errorToString(error, errorMessage, sizeof errorMessage);
        Serial.println(errorMessage);
        return;
    }
    Serial.print("co2Concentration: ");
    Serial.print(co2Concentration);
    Serial.print("\t");
    Serial.print("temperature: ");
    Serial.print(temperature);
    Serial.print("\t");
    Serial.print("humidity: ");
    Serial.print(humidity);
    Serial.println();

    // Send CO2 concentration to Blynk if connected
    if (Blynk.connected()) {
        Blynk.virtualWrite(V1, temperature);
        Blynk.virtualWrite(V2, humidity);
        Blynk.virtualWrite(V3, co2Concentration);
    }

}