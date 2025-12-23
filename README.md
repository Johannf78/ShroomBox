# ShroomBox

An ESP32-based IoT device for environmental monitoring and automated control of mushroom fruiting chambers. ShroomBox uses the Blynk.Edgent framework to provide remote monitoring and control via mobile and web interfaces.

![Version](https://img.shields.io/badge/version-0.1.2-blue)
![License](https://img.shields.io/badge/license-MIT-green)
![Platform](https://img.shields.io/badge/platform-ESP32-orange)

## 🌟 Features

### Environmental Monitoring
- **CO2 Monitoring**: Real-time CO2 concentration measurement using Sensirion SCD30 sensor
- **Temperature & Humidity**: Continuous monitoring of environmental conditions
- **Data Transmission**: All sensor data sent to Blynk cloud every 60 seconds

### Automated Control
- **Fan Control**: Automatic fan activation when CO2 exceeds 900 ppm, deactivation below 750 ppm
- **Humidifier Control**: Automatic humidification when humidity drops below 70%, stops above 85%
- **Smart Logic**: Humidifier only operates when fan is off to prevent conflicts
- **Manual Override**: Full manual control available via Blynk app

### Serial Command Interface
- **CO2 Calibration**: Calibrate sensor via serial with custom PPM value (`CALIBRATE 420`)
- **Manual Control**: Direct fan/humidifier/auto mode control via serial commands
- **Threshold Configuration**: Configure all control thresholds via serial with persistence
- **Help System**: Built-in help command to display all available commands
- **Local Access**: Control device without network access via USB connection

### IoT Capabilities
- **Blynk.Edgent Integration**: Easy WiFi provisioning and OTA updates
- **Remote Control**: Control fan, humidifier, and auto mode from anywhere
- **Real-time Monitoring**: View environmental data on mobile or web dashboard
- **State Indication**: Visual LED feedback for device status
- **Configuration Reset**: Physical button for easy reset and reconfiguration

## 📋 Hardware Requirements

### Required Components
- **ESP32 Development Module** (ESP32 DevKit recommended)
- **Sensirion SCD30 CO2 Sensor** (I2C interface)
- **Fan Control Relay/Transistor** (for GPIO 25)
- **Humidifier Control Relay/Transistor** (for GPIO 26)
- **LED** (for state indication on GPIO 14)
- **Reset Button** (connected to GPIO 0)
- **I2C Pull-up Resistors** (typically 4.7kΩ)

### Pin Configuration
| Component | GPIO Pin | Type | Description |
|------------|----------|------|-------------|
| Fan | 25 | Output | Fan control relay/transistor |
| Humidifier | 26 | Output | Humidifier control relay/transistor |
| LED | 14 | Output | State indication LED |
| Heater | 12 | Output | Reserved for future use |
| Reset Button | 0 | Input | Configuration reset button |
| SCD30 SDA | 21 | I2C | CO2 sensor data line |
| SCD30 SCL | 22 | I2C | CO2 sensor clock line |

## 🚀 Installation

### Prerequisites
1. **Arduino IDE** (1.8.x or 2.x)
2. **ESP32 Board Support**
   - Add board manager URL: `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`
   - Install "ESP32" board package
3. **Required Libraries** (install via Library Manager):
   - **Blynk Library** (latest version with Edgent support) Search Arduino IDE Library Manager for Blynk, install the one by Volodymyr
   - **SensirionI2cScd30** (for CO2 sensor) Search Arduino IDE Library Manager for "Sensirion core", install the one by Sensirion. https://github.com/Sensirion/arduino-core/ 

### Setup Steps

1. **Clone the Repository**
   ```bash
   git clone https://github.com/yourusername/ShroomBox.git
   cd ShroomBox
   ```

2. **Open in Arduino IDE**
   - Navigate to `src/ShroomBox/ShroomBox.ino`
   - Select board: **ESP32 Dev Module**
   - Select appropriate COM port

3. **Configure Blynk Template** (if using custom template)
   - Update `BLYNK_TEMPLATE_ID` in `ShroomBox.ino` if needed
   - Default template ID: `TMPL4JzPZ45yp`

4. **Upload Firmware**
   - Click Upload button in Arduino IDE
   - Wait for upload to complete

5. **Monitor Serial Output**
   - Open Serial Monitor at 115200 baud
   - Device will enter configuration mode on first boot

## ⚙️ Configuration

### Initial Setup (Blynk.Inject)

1. **First Boot**
   - Device creates WiFi access point: `Blynk ShroomBox-XXXX`
   - Connect to this network with your phone/computer

2. **Configure via Web Portal**
   - Open browser, navigate to `http://192.168.4.1` or `http://blynk.cc`
   - Enter your WiFi credentials
   - Enter Blynk authentication token (from Blynk app)

3. **Configure via Blynk App**
   - Open Blynk mobile app
   - Scan QR code or enter device details
   - Follow on-screen instructions

4. **Device Connection**
   - Device will connect to WiFi and Blynk cloud
   - LED will indicate connection status
   - Check Serial Monitor for connection confirmation

### Configuration Reset

- **Hold reset button (GPIO 0) for 10 seconds**
- Device will reset all configuration (WiFi, Blynk credentials)
- **Note**: Threshold settings are NOT reset by button (use `RESET_THRESHOLDS` serial command)
- Returns to configuration mode

### Threshold Configuration

All control thresholds can be configured via serial commands and are automatically saved to flash memory:

- **CO2 Thresholds**: Set via `SET_CO2_MAX` and `SET_CO2_VAR` commands
- **Humidity Thresholds**: Set via `SET_HUM_MIN` and `SET_HUM_VAR` commands
- **View Current**: Use `GET_THRESHOLDS` command
- **Reset to Defaults**: Use `RESET_THRESHOLDS` command
- **Persistence**: All changes are automatically saved and persist across reboots

## 📱 Blynk Dashboard Setup

### Virtual Pin Assignments

| Virtual Pin | Type | Description | Widget Recommendation |
|-------------|------|-------------|----------------------|
| V0 | Integer | Fan Control (0=OFF, 1=ON) | Button/Switch |
| V1 | Double | Temperature (°C) | Gauge/Value Display |
| V2 | Double | Humidity (%) | Gauge/Value Display |
| V3 | Double | CO2 Concentration (ppm) | Gauge/Value Display |
| V4 | Integer | Humidifier Control (0=OFF, 1=ON) | Button/Switch |
| V5 | Integer | Auto Mode Toggle (0=Manual, 1=Auto) | Button/Switch |

### Recommended Dashboard Layout

1. **Control Section**
   - Switch widget for Fan (V0)
   - Switch widget for Humidifier (V4)
   - Switch widget for Auto Mode (V5)

2. **Monitoring Section**
   - Gauge widget for CO2 (V3) - Range: 400-2000 ppm
   - Gauge widget for Temperature (V1) - Range: 0-50°C
   - Gauge widget for Humidity (V2) - Range: 0-100%

3. **Data Visualization** (Optional)
   - SuperChart widget for historical data
   - Value Display widgets for current readings

## 🎮 Usage

### Manual Mode
1. Set Auto Mode (V5) to **OFF** in Blynk app
2. Manually control Fan (V0) and Humidifier (V4) as needed
3. Monitor environmental data (V1, V2, V3)

### Automatic Mode
1. Set Auto Mode (V5) to **ON** in Blynk app (or use serial command `a1`)
2. Device automatically controls fan and humidifier:
   - **Fan ON**: When CO2 > threshold (default: 950 ppm, configurable)
   - **Fan OFF**: When CO2 < (threshold - variation) (default: 850 ppm, configurable)
   - **Humidifier ON**: When humidity < threshold (default: 70%, configurable, only when fan is off)
   - **Humidifier OFF**: When humidity > (threshold + variation) (default: 85%, configurable)
3. Monitor real-time sensor data
4. Manual override still available (temporarily overrides auto mode)
5. Use `GET_THRESHOLDS` serial command to view current threshold values

### Serial Commands
Connect via Serial Monitor at **115200 baud** to use these commands:

#### Control Commands
| Command | Description |
|---------|-------------|
| `f1` | Turn fan ON |
| `f0` | Turn fan OFF |
| `h1` | Turn humidifier ON |
| `h0` | Turn humidifier OFF |
| `a1` | Enable auto mode |
| `a0` | Disable auto mode |

#### CO2 Sensor Commands
| Command | Description |
|---------|-------------|
| `CALIBRATE` | Calibrate CO2 sensor to 400 ppm (default) |
| `CALIBRATE 420` | Calibrate CO2 sensor to specified PPM value |

**CO2 Calibration Tips:**
- Expose sensor to fresh outdoor air (~400-420 ppm) for at least 2 minutes
- Send calibration command: `CALIBRATE 420`
- Wait for confirmation message in Serial Monitor

#### Threshold Configuration Commands
| Command | Description | Range |
|---------|-------------|-------|
| `SET_CO2_MAX <value>` | Set CO2 max threshold (fan ON) | 400-5000 ppm |
| `SET_CO2_VAR <value>` | Set CO2 variation/hysteresis | 50-500 ppm |
| `SET_HUM_MIN <value>` | Set humidity min threshold | 0-100 % |
| `SET_HUM_VAR <value>` | Set humidity variation | 5-50 % |
| `GET_THRESHOLDS` | Display current threshold values | - |
| `RESET_THRESHOLDS` | Reset all thresholds to defaults | - |

**Threshold Configuration Examples:**
```
SET_CO2_MAX 1000      # Fan turns ON when CO2 > 1000 ppm
SET_CO2_VAR 150       # Fan turns OFF when CO2 < 850 ppm (1000-150)
SET_HUM_MIN 75        # Humidifier turns ON when humidity < 75%
SET_HUM_VAR 20        # Humidifier turns OFF when humidity > 95% (75+20)
```

**Note**: All threshold changes are automatically saved to flash memory and persist across reboots.

#### Information Commands
| Command | Description |
|---------|-------------|
| `HELP` or `?` | Display all available commands with descriptions |

Type `HELP` or `?` in Serial Monitor to see a complete list of all commands with examples.

### LED Status Indication
- **Breathing Animation**: Device running normally
- **Fast Blink**: Configuration mode
- **Slow Blink**: Connecting to WiFi/Cloud
- **Solid**: Error state

## 🔧 Technical Details

### Sensor Specifications
- **CO2 Sensor**: Sensirion SCD30
  - Range: 400-10000 ppm
  - Accuracy: ±(30 ppm + 3% of reading)
  - Automatic Self-Calibration: **Disabled** (for mushroom chamber use)
  - Manual Calibration: Via serial command `CALIBRATE [ppm]`
  - Sensor Read Interval: 5 seconds
  - Blynk Update Interval: 60 seconds

### Automatic Control Thresholds
- **CO2 Maximum**: 950 ppm (fan activates) - *Configurable via serial*
- **CO2 Variation**: 100 ppm (hysteresis - fan deactivates at 850 ppm) - *Configurable via serial*
- **Humidity Minimum**: 70% (humidifier activates) - *Configurable via serial*
- **Humidity Variation**: 15% (hysteresis - humidifier deactivates at 85%) - *Configurable via serial*
- **Persistence**: All thresholds are saved to flash memory and survive reboots

### Firmware Details
- **Framework**: Blynk.Edgent
- **Version**: 0.1.2
- **Template ID**: TMPL4JzPZ45yp
- **Template Name**: ShroomBox
- **Main Loop Delay**: 10ms (non-blocking)
- **Sensor Read Interval**: 5 seconds
- **Blynk Update Interval**: 60 seconds
- **Serial Baud Rate**: 115200
- **Configuration Storage**: ESP32 Preferences API (flash memory)
- **Threshold Persistence**: Automatic save to flash on change

### OTA Updates
- Over-the-air firmware updates supported via Blynk.Air
- Upload new firmware through Blynk app or web portal
- No physical access required for updates

## 🐛 Troubleshooting

### Device Won't Connect to WiFi
- Check WiFi credentials are correct
- Ensure 2.4GHz network (ESP32 doesn't support 5GHz)
- Check signal strength
- Reset configuration and try again

### Sensor Not Reading
- Verify I2C connections (SDA=GPIO 21, SCL=GPIO 22)
- Check pull-up resistors (4.7kΩ recommended)
- Verify sensor power supply
- Check Serial Monitor for error messages

### Blynk Not Receiving Data
- Verify device is connected to Blynk cloud (check Serial Monitor)
- Ensure virtual pins are correctly configured in Blynk dashboard
- Check internet connection
- Verify Blynk authentication token

### Fan/Humidifier Not Responding
- Check relay/transistor connections
- Verify GPIO pins are correct (Fan=25, Humidifier=26)
- Test with manual control first
- Check Serial Monitor for control commands

## 📚 Project Structure

```
ShroomBox/
├── src/
│   └── ShroomBox/
│       ├── ShroomBox.ino          # Main sketch
│       ├── func_co2_sensor.ino    # CO2 sensor functions
│       ├── BlynkEdgent.h          # Blynk framework integration
│       ├── BlynkState.h           # State machine definitions
│       ├── ConfigStore.h          # Configuration management
│       ├── ConfigMode.h           # WiFi provisioning
│       ├── Indicator.h            # LED state indication
│       ├── ResetButton.h          # Button handling
│       ├── Settings.h             # Board configuration
│       ├── OTA.h                  # OTA update handling
│       └── Console.h              # Serial console
├── memory-bank/                   # Project documentation
├── LICENSE
└── README.md
```

## ✅ Recent Updates

- [x] Serial command interface for local control
- [x] CO2 calibration via serial with custom PPM value (`CALIBRATE [ppm]`)
- [x] Manual fan/humidifier/auto mode control via serial (`f0`, `f1`, `h0`, `h1`, `a0`, `a1`)
- [x] **Threshold configuration via serial** - Set all control thresholds remotely
- [x] **Threshold persistence** - All thresholds saved to flash memory, survive reboots
- [x] **Built-in help system** - Type `HELP` or `?` to see all available commands
- [x] Updated CO2 thresholds (950 ppm max, 100 ppm variation)
- [x] Separate sensor read (5s) and Blynk update (60s) intervals

## 🔮 Future Enhancements

- [ ] Heater control implementation (GPIO 12)
- [ ] User-configurable thresholds via Blynk (currently available via serial)
- [ ] Historical data logging
- [ ] Blynk notifications/alerts
- [ ] Multi-chamber support
- [ ] Enhanced web portal UI
- [ ] Threshold configuration via Blynk virtual pins

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- **Blynk** - IoT platform and framework
- **Sensirion** - SCD30 CO2 sensor
- **ESP32** - Hardware platform

## 📞 Support

For issues, questions, or contributions:
- Open an issue on GitHub
- Check the [memory-bank documentation](memory-bank/) for detailed technical information

---

**Note**: This project is designed specifically for mushroom fruiting chamber environmental control. The CO2 sensor's Automatic Self-Calibration (ASC) is disabled to ensure accurate readings in elevated CO2 environments typical of mushroom growing.
