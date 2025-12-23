# Active Context: Current Work Focus

## Current Status
**Date**: December 2024  
**Phase**: Production Ready - Mushroom Chamber Control System Working

## Recent Changes
- ✅ Fan control (GPIO 25) implemented via Blynk virtual pin V0
- ✅ Humidifier control (GPIO 26) implemented via Blynk virtual pin V4
- ✅ Auto mode toggle implemented via Blynk virtual pin V5
- ✅ Automatic control logic: Fan based on CO2 thresholds (950 ppm on, 850 ppm off)
- ✅ Automatic control logic: Humidifier based on humidity thresholds (70% min, 85% max)
- ✅ CO2 sensor data successfully sent to Blynk: Temperature (V1), Humidity (V2), CO2 (V3)
- ✅ Non-blocking timer implementation for CO2 readings (5-second interval for sensor, 60-second interval for Blynk updates)
- ✅ CO2 sensor ASC (Automatic Self-Calibration) disabled for mushroom chamber use
- ✅ Code optimized: Blynk virtual pins only updated when values change
- ✅ **Serial command interface**: Complete command set for control and configuration
- ✅ **CO2 calibration with custom PPM**: `CALIBRATE [ppm]` command accepts optional reference value
- ✅ **Threshold configuration via serial**: All thresholds configurable with persistence
- ✅ **Threshold persistence**: All thresholds saved to flash memory, survive reboots
- ✅ **Built-in help system**: `HELP` or `?` command displays all available commands
- ✅ **System tested and verified**: Device online, all sensors and controls functional

## Current Work Focus

### Immediate Focus
1. **System Operational**: Core functionality tested and working
2. **CO2 Monitoring Active**: Real-time CO2 data transmission confirmed
3. **Serial Calibration**: Users can calibrate CO2 sensor via serial with custom PPM value
4. **Threshold Configuration**: All thresholds configurable via serial with flash persistence
5. **Ready for Enhancement**: System stable, ready for additional features

### Active Decisions
- **Board Configuration**: Using ESP32 Dev Module (`USE_ESP32_DEV_MODULE`)
- **LED Pin**: GPIO 14 (state indication)
- **Fan Pin**: GPIO 25 (output control)
- **Humidifier Pin**: GPIO 26 (output control)
- **Heater Pin**: GPIO 12 (defined, not yet used)
- **Button Pin**: GPIO 0 (board default)
- **Blynk Template**: TMPL4JzPZ45yp (ShroomBox)
- **Firmware Version**: 0.1.2
- **Virtual Pin Assignments**: 
  - V0 = Fan control (manual/auto)
  - V1 = Temperature (read-only)
  - V2 = Humidity (read-only)
  - V3 = CO2 concentration (read-only)
  - V4 = Humidifier control (manual/auto)
  - V5 = Auto mode toggle
- **CO2 Read Interval**: 5 seconds (5000ms, non-blocking timer-based)
- **Blynk Update Interval**: 60 seconds (60000ms, non-blocking timer-based)
- **CO2 Thresholds**: Max 950 ppm (fan on), Variation 100 ppm (fan off at 850 ppm) - *Configurable*
- **Humidity Thresholds**: Min 70% (humidifier on), Variation 15% (humidifier off at 85%) - *Configurable*
- **Threshold Persistence**: All thresholds saved to flash using Preferences API

### Serial Commands Available
| Command | Description |
|---------|-------------|
| `HELP` or `?` | Display all available commands |
| `CALIBRATE` | Calibrate CO2 sensor to 400 ppm (default) |
| `CALIBRATE [ppm]` | Calibrate CO2 sensor to specified PPM (e.g., `CALIBRATE 420`) |
| `f1` | Turn fan ON |
| `f0` | Turn fan OFF |
| `h1` | Turn humidifier ON |
| `h0` | Turn humidifier OFF |
| `a1` | Enable auto mode |
| `a0` | Disable auto mode |
| `SET_CO2_MAX <value>` | Set CO2 max threshold (400-5000 ppm) |
| `SET_CO2_VAR <value>` | Set CO2 variation/hysteresis (50-500 ppm) |
| `SET_HUM_MIN <value>` | Set humidity min threshold (0-100 %) |
| `SET_HUM_VAR <value>` | Set humidity variation (5-50 %) |
| `GET_THRESHOLDS` | Display current threshold values |
| `RESET_THRESHOLDS` | Reset thresholds to defaults |

### Current Implementation State
- ✅ Fan control (GPIO 25) via Blynk virtual pin V0 - manual and automatic modes
- ✅ Humidifier control (GPIO 26) via Blynk virtual pin V4 - manual and automatic modes
- ✅ Auto mode toggle via Blynk virtual pin V5 and serial commands (a1, a0)
- ✅ CO2 sensor (SCD30) integration - initialization and reading working
- ✅ CO2 sensor ASC disabled for mushroom chamber use
- ✅ CO2 sensor calibration via serial with custom PPM value
- ✅ Environmental data sent to Blynk: Temperature (V1), Humidity (V2), CO2 (V3) every 60 seconds
- ✅ Automatic control logic: Fan based on CO2 thresholds (configurable)
- ✅ Automatic control logic: Humidifier based on humidity thresholds (configurable, only when fan is off)
- ✅ Non-blocking loop implementation - responds immediately to Blynk commands
- ✅ Serial command interface for manual control, calibration, and configuration
- ✅ Threshold configuration via serial with validation and persistence
- ✅ Threshold persistence to flash memory (survives reboots)
- ✅ Built-in help system for serial commands
- ✅ WiFi provisioning (Blynk.Inject)
- ✅ Blynk cloud connectivity
- ✅ State machine implementation
- ✅ LED state indication (GPIO 14)
- ✅ Configuration reset via button (GPIO 0)
- ✅ OTA update capability (framework support)
- ✅ Configuration persistence

## Next Steps

### Potential Enhancements (Not Started)
1. **Heater Control**: Implement heater control via GPIO 12 for temperature management
2. **Blynk Threshold Configuration**: Make thresholds configurable via Blynk virtual pins
3. **Data Logging**: Historical tracking of environmental conditions
4. **Alerts/Notifications**: Blynk notifications when thresholds are exceeded
5. **Web Portal Enhancement**: Improve configuration portal UI (requires filesystem)
6. **Error Handling**: Enhanced error recovery and user feedback
7. **Power Management**: Deep sleep for battery operation
8. **Multi-Chamber Support**: Support multiple fruiting chambers from same device

### Maintenance Tasks
- Monitor for Blynk library updates
- Test OTA update functionality
- Verify compatibility with different ESP32 boards
- Document any board-specific issues

## Active Considerations

### Code Quality
- Code follows Blynk.Edgent framework patterns
- Modular header file structure is clean
- State machine is well-defined
- Error handling is present but could be enhanced
- Serial command parsing is straightforward and extensible
- Threshold persistence properly implemented with Preferences API

### User Experience
- LED indication provides good visual feedback
- Button reset is functional but requires 10-second hold
- Web portal is basic but functional
- Serial output provides good debugging information
- Serial commands allow manual control without Blynk app
- Built-in help system makes commands discoverable
- Threshold configuration is intuitive with validation

### Technical Debt
- Heater pin (GPIO 12) defined but not yet implemented
- Threshold values are configurable via serial (not yet via Blynk)
- CO2 sensor blocking delay (1.5s) inside readCO2() - acceptable since it runs every 5s
- Project is in early stage (v0.1.2)
- Code structure is clean and maintainable

## Questions to Resolve
- What additional features are planned for ShroomBox?
- Are there specific sensors or actuators to integrate?
- What is the target deployment scenario?
- Are there specific board requirements beyond ESP32 Dev Module?

## Notes
- Project is a sandbox/learning project
- Currently focused on basic functionality
- Framework provides solid foundation for expansion
- Good starting point for IoT development
- Threshold configuration and persistence fully implemented
