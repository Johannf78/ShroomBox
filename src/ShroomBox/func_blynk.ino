
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
