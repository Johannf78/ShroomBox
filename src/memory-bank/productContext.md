# Product Context: ShroomBox

## Why This Project Exists
ShroomBox is an IoT device that enables remote control and monitoring of hardware through the Blynk platform. The project aims to provide a foundation for building connected devices with minimal configuration overhead.

## Problems It Solves

### 1. Easy Device Provisioning
- **Problem**: Traditional IoT devices require manual WiFi configuration and complex setup
- **Solution**: Blynk.Inject provides dynamic WiFi credentials provisioning through a simple web portal or mobile app

### 2. Remote Device Control
- **Problem**: Need to control hardware remotely without direct physical access
- **Solution**: Blynk platform provides mobile and web interfaces for remote control

### 3. Firmware Updates
- **Problem**: Updating firmware on deployed devices is difficult and time-consuming
- **Solution**: Blynk.Air enables Over-The-Air firmware updates without physical access

### 4. Device State Visibility
- **Problem**: Difficult to know device status without physical inspection
- **Solution**: LED indicator provides visual feedback of device state (configuring, connecting, running, error)

## How It Should Work

### Initial Setup Flow
1. Device boots and checks for stored configuration
2. If no configuration exists, enters configuration mode (AP mode)
3. User connects to device's WiFi AP
4. User accesses web portal or uses Blynk app to configure WiFi and Blynk credentials
5. Device saves configuration and attempts to connect
6. Device connects to WiFi, then to Blynk cloud
7. Device enters running state

### Normal Operation Flow
1. Device maintains connection to WiFi and Blynk cloud
2. User can control LED via Blynk virtual pin V0 (immediate response)
3. Device reads CO2 sensor every 5 seconds and sends data to Blynk virtual pin V3
4. Device responds to commands from Blynk app immediately (non-blocking loop)
5. LED indicates device state (breathing animation when running)

### Configuration Reset Flow
1. User holds physical button for 10 seconds
2. Device resets configuration to defaults
3. Device enters configuration mode
4. User can reconfigure device

### OTA Update Flow
1. User uploads new firmware via Blynk app or web portal
2. Device receives update and installs it
3. Device reboots with new firmware

## User Experience Goals

### For End Users
- **Simple Setup**: Minimal steps to get device online
- **Reliable Operation**: Device maintains connection and responds quickly
- **Clear Feedback**: LED clearly indicates what device is doing
- **Easy Recovery**: Simple way to reset and reconfigure if needed

### For Developers
- **Extensible**: Easy to add new features and sensors
- **Maintainable**: Clean code structure following Blynk.Edgent patterns
- **Debuggable**: Serial output and state indicators help troubleshoot issues
- **Documented**: Clear understanding of device behavior and state transitions

## Target Use Cases
1. **Home Automation**: Control lights, fans, or other devices remotely
2. **Monitoring**: Monitor sensors and receive alerts
3. **Prototyping**: Quick prototype of IoT concepts
4. **Education**: Learn IoT development with ESP32 and Blynk

## Current Implementation Status
- Basic LED control via Blynk virtual pin V0 (immediate response)
- CO2 sensor (SCD30) integrated - reads CO2, temperature, humidity
- CO2 concentration sent to Blynk virtual pin V3 in real-time
- Non-blocking loop implementation for responsive Blynk command processing
- WiFi provisioning working
- Blynk cloud connectivity working
- State machine and LED indication working
- Configuration reset via button working
- OTA update capability present

