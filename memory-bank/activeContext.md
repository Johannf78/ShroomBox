# Active Context: Current Work Focus

## Current Status
**Date**: System Operational & Tested Successfully  
**Phase**: Production Ready - Mushroom Chamber Control System Working

## Recent Changes
- ✅ Fan control (GPIO 25) implemented via Blynk virtual pin V0
- ✅ Humidifier control (GPIO 26) implemented via Blynk virtual pin V4
- ✅ Auto mode toggle implemented via Blynk virtual pin V5
- ✅ Automatic control logic: Fan based on CO2 thresholds (1250 ppm on, 1100 ppm off)
- ✅ Automatic control logic: Humidifier based on humidity thresholds (70% min, 85% max)
- ✅ CO2 sensor data successfully sent to Blynk: Temperature (V1), Humidity (V2), CO2 (V3)
- ✅ Non-blocking timer implementation for CO2 readings (60-second interval)
- ✅ CO2 sensor ASC (Automatic Self-Calibration) disabled for mushroom chamber use
- ✅ Code optimized: Blynk virtual pins only updated when values change
- ✅ **System tested and verified**: Device online, all sensors and controls functional

## Current Work Focus

### Immediate Focus
1. **System Operational**: Core functionality tested and working
2. **CO2 Monitoring Active**: Real-time CO2 data transmission confirmed
3. **Ready for Enhancement**: System stable, ready for additional features

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
- **CO2 Read Interval**: 60 seconds (60000ms, non-blocking timer-based)
- **CO2 Thresholds**: Max 1250 ppm (fan on), Variation 150 ppm (fan off at 1100 ppm)
- **Humidity Thresholds**: Min 70% (humidifier on), Variation 15% (humidifier off at 85%)

### Current Implementation State
- ✅ Fan control (GPIO 25) via Blynk virtual pin V0 - manual and automatic modes
- ✅ Humidifier control (GPIO 26) via Blynk virtual pin V4 - manual and automatic modes
- ✅ Auto mode toggle via Blynk virtual pin V5
- ✅ CO2 sensor (SCD30) integration - initialization and reading working
- ✅ CO2 sensor ASC disabled for mushroom chamber use
- ✅ Environmental data sent to Blynk: Temperature (V1), Humidity (V2), CO2 (V3) every 60 seconds
- ✅ Automatic control logic: Fan based on CO2 thresholds
- ✅ Automatic control logic: Humidifier based on humidity thresholds (only when fan is off)
- ✅ Non-blocking loop implementation - responds immediately to Blynk commands
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
2. **Advanced Thresholds**: User-configurable CO2 and humidity thresholds via Blynk
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

### User Experience
- LED indication provides good visual feedback
- Button reset is functional but requires 10-second hold
- Web portal is basic but functional
- Serial output provides good debugging information

### Technical Debt
- Heater pin (GPIO 12) defined but not yet implemented
- Threshold values are hardcoded - could be made configurable via Blynk
- CO2 sensor blocking delay (1.5s) inside readCO2() - acceptable since it runs every 60s
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

