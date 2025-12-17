# Project Brief: ShroomBox ESP32 IoT Device

## Project Overview
ShroomBox is an ESP32-based IoT device for environmental monitoring and control of a mushroom fruiting chamber, using the Blynk.Edgent framework. The device provides remote control and monitoring capabilities through the Blynk platform, enabling mobile and web interfaces for environmental management.

## Core Requirements

### Primary Goals
1. **IoT Device Control**: Enable remote control of ESP32 hardware via Blynk platform
2. **Easy Provisioning**: Implement Blynk.Inject for dynamic WiFi credentials provisioning
3. **OTA Updates**: Support Over-The-Air firmware updates via Blynk.Air
4. **Device Indication**: Visual state indication using physical LED
5. **Configuration Management**: Credentials reset using physical button
6. **Local Control**: Serial command interface for local control and calibration

### Key Features
- WiFi connectivity with dynamic provisioning
- Blynk cloud integration
- Fan control (GPIO 25) via Blynk virtual pin V0 - manual and automatic modes
- Humidifier control (GPIO 26) via Blynk virtual pin V4 - manual and automatic modes
- Auto mode toggle via Blynk virtual pin V5
- CO2 sensor integration (SCD30) - reads CO2, temperature, and humidity
- Environmental data transmission to Blynk: Temperature (V1), Humidity (V2), CO2 (V3)
- Automatic environmental control based on CO2 and humidity thresholds
- CO2 sensor calibration via serial with custom PPM value
- Serial command interface for local control (CALIBRATE, f0, f1, h0, h1)
- Non-blocking loop implementation for responsive operation
- State machine-based operation
- Configuration persistence using ESP32 Preferences
- Web-based configuration portal
- OTA firmware update capability
- LED state indication (GPIO 14)

## Project Scope

### In Scope
- ESP32 device firmware development
- Blynk.Edgent framework integration
- Fan and humidifier control for mushroom chamber
- Automatic environmental control based on sensor thresholds
- CO2 sensor (SCD30) integration via I2C
- CO2 sensor calibration via serial interface
- Temperature and humidity monitoring
- WiFi provisioning and management
- Cloud connectivity
- Device state management
- Serial command interface

### Out of Scope (Currently)
- Additional sensor integrations (future expansion)
- Custom mobile app development (uses Blynk app)
- Local web server beyond configuration portal

## Success Criteria
- Device successfully connects to WiFi
- Device connects to Blynk cloud
- Fan can be controlled remotely via Blynk app (virtual pin V0)
- Humidifier can be controlled remotely via Blynk app (virtual pin V4)
- Auto mode can be toggled via Blynk app (virtual pin V5)
- Environmental sensor data (CO2, temperature, humidity) is transmitted to Blynk cloud (V1, V2, V3)
- Automatic control maintains optimal CO2 and humidity levels
- CO2 sensor can be calibrated via serial command with custom PPM value
- Fan and humidifier can be controlled via serial commands
- Configuration can be reset via physical button
- OTA updates work correctly
- Device state is clearly indicated via LED

## Constraints
- Must use ESP32 development board
- Must use Blynk.Edgent framework
- Template ID: TMPL4JzPZ45yp
- Template Name: ShroomBox
- Firmware Version: 0.1.2
