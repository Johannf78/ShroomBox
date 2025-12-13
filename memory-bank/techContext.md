# Technical Context: ShroomBox

## Technologies Used

### Hardware Platform
- **ESP32 Development Module** (`USE_ESP32_DEV_MODULE`)
- GPIO 14 for LED state indication
- GPIO 0 for reset button (board default)
- GPIO 25 for fan control (output)
- GPIO 26 for humidifier control (output)
- GPIO 12 for heater control (defined, not yet used)
- GPIO 21 (SDA) and GPIO 22 (SCL) for I2C communication with CO2 sensor
- Built-in WiFi and Bluetooth capabilities

### Core Libraries & Frameworks
- **Blynk.Edgent**: IoT device management framework
  - Version: Latest (new library required)
  - Provides: WiFi provisioning, OTA updates, cloud connectivity
- **BlynkSimpleEsp32_SSL**: Blynk library for ESP32 with SSL support
- **WiFi.h**: ESP32 WiFi library
- **Preferences.h**: ESP32 non-volatile storage
- **WebServer.h**: HTTP server for configuration portal
- **DNSServer.h**: DNS server for captive portal
- **Update.h**: OTA firmware update support

### Optional Libraries
- **Adafruit_NeoPixel**: For WS2812 RGB LED support (if configured)
- **pthread**: POSIX threads for LED indicator (default on ESP32)
- **SensirionI2cScd30**: Sensirion SCD30 CO2 sensor library (I2C communication)
- **Wire.h**: I2C communication library (ESP32 built-in)

## Development Setup

### Arduino IDE Configuration
- **Board**: ESP32 Dev Module
- **Upload Speed**: Default
- **CPU Frequency**: Default
- **Flash Frequency**: Default
- **Flash Size**: Default
- **Partition Scheme**: Default
- **Core Debug Level**: None (or Info for debugging)

### Required Board Manager URLs
- ESP32 board support: https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json

### Library Dependencies
1. **Blynk Library** (new version)
   - Install via Arduino Library Manager
   - Must support Blynk.Edgent features
   - Requires SSL support for ESP32

2. **ESP32 Board Support Package**
   - Install via Board Manager
   - Includes WiFi, Preferences, WebServer, etc.

3. **SensirionI2cScd30 Library**
   - Install via Arduino Library Manager
   - Required for SCD30 CO2 sensor communication
   - Provides I2C interface for sensor initialization and data reading

### Build Configuration

#### Preprocessor Defines
- `BLYNK_TEMPLATE_ID`: "TMPL4JzPZ45yp"
- `BLYNK_TEMPLATE_NAME`: "ShroomBox"
- `BLYNK_FIRMWARE_VERSION`: "0.1.2"
- `USE_ESP32_DEV_MODULE`: Board type
- `APP_DEBUG`: Enable debug output
- `BLYNK_PRINT`: Serial output for Blynk logs

#### Optional Defines
- `BLYNK_DEBUG`: Enable Blynk library debug output
- `WIFI_CAPTIVE_PORTAL_ENABLE`: Enable captive portal
- `BLYNK_USE_LITTLEFS` or `BLYNK_USE_SPIFFS`: Filesystem for web portal assets

### Serial Configuration
- **Baud Rate**: 115200
- **Output**: Debug messages, state transitions, connection status
- **Format**: Plain text with timestamps (via Blynk logging)

## Technical Constraints

### Memory Constraints
- **Flash**: Varies by ESP32 model (typically 4MB+)
- **RAM**: Limited heap for WiFi and Blynk operations
- **Preferences**: Limited to ~64KB (ESP32 limitation)

### Network Constraints
- **WiFi**: 2.4GHz only (ESP32 limitation)
- **Security**: WEP minimum (configurable, WPA2+ recommended)
- **Connection Timeout**: 50 seconds for WiFi, 50 seconds for Blynk cloud
- **Retry Logic**: 500 retries maximum for connections

### Storage Constraints
- **ConfigStore**: Packed struct, ~200 bytes
- **WiFi SSID**: Max 33 characters
- **WiFi Password**: Max 63 characters
- **Blynk Token**: 32 characters (alphanumeric + dash/underscore)
- **Cloud Host**: Max 33 characters

### Timing Constraints
- **Button Hold Time (Indication)**: 3 seconds
- **Button Hold Time (Action)**: 10 seconds
- **Button Press Time (Action)**: 50ms minimum
- **State Transition**: Immediate (no delays)
- **Main Loop Delay**: 10ms between iterations (for BlynkEdgent.run())
- **CO2 Sensor Read Interval**: 60 seconds (60000ms, non-blocking timer-based)
- **CO2 Sensor Blocking Delay**: 1.5 seconds (inside readCO2(), only runs every 60s)

## Dependencies

### Required ESP32 Features
- WiFi (Station and Access Point modes)
- Preferences (non-volatile storage)
- WebServer (HTTP server)
- DNSServer (DNS for captive portal)
- Update (OTA firmware updates)
- pthread (threading support)

### Blynk Platform Requirements
- **Blynk Account**: Required for device registration
- **Blynk Template**: Template ID must match configured template
- **Blynk App**: Mobile app for device control (iOS/Android)
- **Internet Connection**: Required for cloud connectivity
- **SSL/TLS**: Required for secure communication (port 443)

## Development Workflow

### Local Development
1. Open project in Arduino IDE
2. Select ESP32 Dev Module board
3. Configure serial port
4. Upload sketch
5. Monitor serial output at 115200 baud

### Testing Workflow
1. **First Boot**: Device should enter config mode (AP mode)
2. **Connect to AP**: Device creates "Blynk ShroomBox-XXXX" network
3. **Configure**: Access web portal or use Blynk app
4. **Verify Connection**: Check serial output for connection status
5. **Test Control**: Use Blynk app to control fan (V0), humidifier (V4), and auto mode (V5)
6. **Test Sensors**: Verify CO2 (V3), temperature (V1), and humidity (V2) data in Blynk app
7. **Test Auto Mode**: Enable auto mode and verify automatic control based on thresholds
8. **Test Reset**: Hold button 10 seconds to reset config

### Debugging
- **Serial Monitor**: Primary debugging tool
- **LED Indicator**: Visual state feedback
- **State Logging**: State transitions logged to serial
- **Error Codes**: Stored in config for troubleshooting

## Build Output
- **Firmware Binary**: `.bin` file for OTA updates
- **Partition Table**: Default (app + OTA)
- **Flash Layout**: App partition + OTA partition + NVS

## Known Technical Limitations
1. **WiFi AP Mode**: Limited to 4 connected stations
2. **Web Portal**: Basic HTML form (no filesystem assets by default)
3. **OTA Updates**: Requires stable connection, may fail on poor WiFi
4. **Button**: GPIO 0 may conflict with boot mode (hold during boot)
5. **LED**: GPIO 14 used for state indication
6. **Fan/Humidifier**: GPIO 25 and 26 are outputs, ensure proper relay/transistor drive
7. **CO2 Sensor**: ASC disabled for mushroom chamber - requires manual calibration if needed

## Performance Characteristics
- **Boot Time**: ~2-3 seconds to serial output
- **WiFi Connect**: 5-30 seconds depending on network
- **Blynk Connect**: 2-10 seconds after WiFi connected
- **State Transitions**: <100ms
- **LED Updates**: 20ms-5000ms depending on animation
- **Main Loop**: ~10ms per iteration

## Security Considerations
- **WiFi**: Uses WPA2+ encryption (user-configured)
- **Blynk**: SSL/TLS encrypted communication
- **Auth Token**: 32-character token for device authentication
- **Web Portal**: No authentication (local network only)
- **OTA Updates**: No signature verification (Blynk handles this)

## Future Technical Considerations
- **Filesystem**: Consider LittleFS/SPIFFS for web portal assets
- **MQTT**: Alternative to Blynk for local control
- **Additional Sensors**: GPIO pins available for expansion
- **Power Management**: Deep sleep for battery operation
- **Multi-Device**: Support for multiple devices from same codebase

