# Project Brief: ShroomBox ESP32 IoT Device

## Project Overview
ShroomBox is an ESP32-based IoT device project using the Blynk.Edgent framework. The device provides remote control capabilities through the Blynk platform, enabling mobile and web interfaces for device management.

## Core Requirements

### Primary Goals
1. **IoT Device Control**: Enable remote control of ESP32 hardware via Blynk platform
2. **Easy Provisioning**: Implement Blynk.Inject for dynamic WiFi credentials provisioning
3. **OTA Updates**: Support Over-The-Air firmware updates via Blynk.Air
4. **Device Indication**: Visual state indication using physical LED
5. **Configuration Management**: Credentials reset using physical button

### Key Features
- WiFi connectivity with dynamic provisioning
- Blynk cloud integration
- LED control via Blynk virtual pin (V0) - immediate response
- CO2 sensor integration (SCD30) - reads CO2, temperature, and humidity
- CO2 data transmission to Blynk virtual pin (V3) in real-time
- Non-blocking loop implementation for responsive operation
- State machine-based operation
- Configuration persistence using ESP32 Preferences
- Web-based configuration portal
- OTA firmware update capability

## Project Scope

### In Scope
- ESP32 device firmware development
- Blynk.Edgent framework integration
- LED control functionality
- CO2 sensor (SCD30) integration via I2C
- WiFi provisioning and management
- Cloud connectivity
- Device state management

### Out of Scope (Currently)
- Additional sensor integrations (future expansion)
- Custom mobile app development (uses Blynk app)
- Local web server beyond configuration portal

## Success Criteria
- Device successfully connects to WiFi
- Device connects to Blynk cloud
- LED can be controlled remotely via Blynk app (immediate response)
- CO2 sensor data is transmitted to Blynk cloud (virtual pin V3)
- Configuration can be reset via physical button
- OTA updates work correctly
- Device state is clearly indicated via LED

## Constraints
- Must use ESP32 development board
- Must use Blynk.Edgent framework
- Template ID: TMPL4JzPZ45yp
- Template Name: ShroomBox
- Firmware Version: 0.1.0

