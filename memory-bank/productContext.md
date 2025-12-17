# Product Context: ShroomBox

## Why This Project Exists
ShroomBox is an IoT device designed for environmental monitoring and control of a mushroom fruiting chamber. It enables remote control and monitoring of environmental conditions (CO2, temperature, humidity) and automated control of fans and humidifiers through the Blynk platform, ensuring optimal growing conditions for mushrooms.

## Problems It Solves

### 1. Easy Device Provisioning
- **Problem**: Traditional IoT devices require manual WiFi configuration and complex setup
- **Solution**: Blynk.Inject provides dynamic WiFi credentials provisioning through a simple web portal or mobile app

### 2. Remote Device Control
- **Problem**: Need to control hardware remotely without direct physical access
- **Solution**: Blynk platform provides mobile and web interfaces for remote control

### 3. Local Device Control
- **Problem**: Sometimes need to control device without network/app access
- **Solution**: Serial command interface allows direct control via USB connection

### 4. CO2 Sensor Calibration
- **Problem**: CO2 sensors drift over time and need recalibration
- **Solution**: Serial command `CALIBRATE [ppm]` allows easy recalibration to any reference value

### 5. Firmware Updates
- **Problem**: Updating firmware on deployed devices is difficult and time-consuming
- **Solution**: Blynk.Air enables Over-The-Air firmware updates without physical access

### 6. Device State Visibility
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
2. Device reads CO2 sensor every 5 seconds and sends data to Blynk every 60 seconds (V1=temp, V2=humidity, V3=CO2)
3. User can control fan manually via Blynk virtual pin V0
4. User can control humidifier manually via Blynk virtual pin V4
5. User can enable/disable auto mode via Blynk virtual pin V5
6. In auto mode:
   - Fan turns on when CO2 > 900 ppm, off when CO2 < 750 ppm
   - Humidifier turns on when humidity < 70% (only when fan is off), off when humidity > 85%
7. Device responds to commands from Blynk app immediately (non-blocking loop)
8. LED indicates device state (breathing animation when running)

### Serial Command Flow
1. User connects via Serial Monitor at 115200 baud
2. User can send commands:
   - `CALIBRATE` - Calibrate CO2 sensor to 400 ppm (default)
   - `CALIBRATE 420` - Calibrate CO2 sensor to 420 ppm
   - `f1` / `f0` - Turn fan ON/OFF
   - `h1` / `h0` - Turn humidifier ON/OFF
3. Device responds with confirmation messages

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
- **Local Control**: Serial commands work without network access
- **Easy Calibration**: Simple command to recalibrate CO2 sensor

### For Developers
- **Extensible**: Easy to add new features and sensors
- **Maintainable**: Clean code structure following Blynk.Edgent patterns
- **Debuggable**: Serial output and state indicators help troubleshoot issues
- **Documented**: Clear understanding of device behavior and state transitions

## Target Use Cases
1. **Mushroom Growing**: Automated environmental control for mushroom fruiting chambers
2. **Indoor Agriculture**: Monitor and control CO2, temperature, and humidity for optimal growing conditions
3. **Home Automation**: Control fans and humidifiers remotely
4. **Environmental Monitoring**: Real-time monitoring of air quality and environmental conditions

## Current Implementation Status
- Fan control (GPIO 25) via Blynk virtual pin V0 - manual and automatic modes
- Humidifier control (GPIO 26) via Blynk virtual pin V4 - manual and automatic modes
- Auto mode toggle via Blynk virtual pin V5
- CO2 sensor (SCD30) integrated - reads CO2, temperature, humidity
- Environmental data sent to Blynk: Temperature (V1), Humidity (V2), CO2 (V3) every 60 seconds
- Automatic control logic: Fan based on CO2 thresholds (900 ppm max), Humidifier based on humidity thresholds (70% min)
- CO2 sensor ASC (Automatic Self-Calibration) disabled for mushroom chamber use
- CO2 sensor calibration via serial with custom PPM value (`CALIBRATE [ppm]`)
- Serial command interface for local control (f0, f1, h0, h1)
- Non-blocking loop implementation for responsive Blynk command processing
- WiFi provisioning working
- Blynk cloud connectivity working
- State machine and LED indication (GPIO 14) working
- Configuration reset via button (GPIO 0) working
- OTA update capability present
