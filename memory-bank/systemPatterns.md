# System Patterns: ShroomBox Architecture

## System Architecture

### High-Level Architecture
```
┌─────────────────┐
│   Blynk Cloud   │
└────────┬────────┘
         │ HTTPS/SSL
         │
┌────────▼────────┐
│  ESP32 Device   │
│  (ShroomBox)    │
│                 │
│  ┌───────────┐  │
│  │  WiFi     │  │◄─── WiFi Network
│  └───────────┘  │
│  ┌───────────┐  │
│  │  Blynk    │  │◄─── Blynk Library
│  │  Client   │  │
│  └───────────┘  │
│  ┌───────────┐  │
│  │  State    │  │◄─── State Machine
│  │  Machine  │  │
│  └───────────┘  │
│  ┌───────────┐  │
│  │  Hardware │  │◄─── LED, Button, Fan, Humidifier, CO2 Sensor
│  │  Control  │  │
│  └───────────┘  │
└─────────────────┘
```

## Key Technical Decisions

### 1. State Machine Pattern
The device uses a state machine to manage different operational modes:
- **MODE_WAIT_CONFIG**: Waiting for initial configuration
- **MODE_CONFIGURING**: User is configuring device via web portal
- **MODE_CONNECTING_NET**: Connecting to WiFi network
- **MODE_CONNECTING_CLOUD**: Connecting to Blynk cloud
- **MODE_RUNNING**: Normal operation, connected and running
- **MODE_OTA_UPGRADE**: Performing firmware update
- **MODE_SWITCH_TO_STA**: Switching from AP to Station mode
- **MODE_RESET_CONFIG**: Resetting configuration
- **MODE_ERROR**: Error state, will restart after timeout

**Rationale**: State machine provides clear separation of concerns and predictable behavior transitions.

### 2. Configuration Storage
- Uses ESP32 Preferences API for persistent storage
- Stores WiFi credentials, Blynk token, cloud host/port
- Supports static IP configuration
- Includes error tracking for provisioning failures

**Rationale**: Preferences API provides reliable non-volatile storage without filesystem overhead.

### 3. Modular Header File Structure
Each major component is in its own header file:
- `BlynkEdgent.h`: Main framework integration
- `BlynkState.h`: State machine definitions
- `ConfigStore.h`: Configuration management
- `ConfigMode.h`: Web portal and WiFi provisioning
- `Indicator.h`: LED state indication
- `ResetButton.h`: Button handling
- `Settings.h`: Board-specific configuration
- `OTA.h`: Over-the-air update handling
- `Console.h`: Serial console interface
- `func_co2_sensor.ino`: CO2 sensor functions (initCO2, readCO2)

**Rationale**: Modular structure makes code maintainable and allows easy extension.

### 4. LED Indication System
- Different colors/patterns for each state
- Uses pthread for non-blocking LED updates
- Supports RGB LED, single-color LED, and WS2812 addressable LEDs
- Visual feedback for button actions

**Rationale**: Visual feedback is essential for user experience and debugging.

## Design Patterns in Use

### 1. Singleton Pattern
- `BlynkEdgent`: Single global instance manages device lifecycle
- `indicator`: Single global Indicator instance
- `configStore`: Single global configuration store

### 2. State Pattern
- State machine implemented via enum and switch statements
- Each state has dedicated handler function
- State transitions are explicit and logged

### 3. Template Method Pattern
- `BlynkEdgent.begin()`: Initialization template
- `BlynkEdgent.run()`: Main loop template with state-specific behavior

### 4. Observer Pattern
- Button interrupts trigger state changes
- LED indicator observes state changes and updates display

## Component Relationships

### Core Dependencies
```
Edgent_ESP32.ino
    │
    ├──► BlynkEdgent.h
    │       │
    │       ├──► BlynkState.h
    │       ├──► ConfigStore.h
    │       ├──► ConfigMode.h
    │       ├──► ResetButton.h
    │       ├──► Indicator.h
    │       ├──► OTA.h
    │       ├──► Console.h
    │       └──► Settings.h
    │
    └──► User Code (BLYNK_WRITE handlers)
```

### Data Flow
1. **Configuration Flow**:
   - User input → Web portal → ConfigStore → Preferences → Flash
   
2. **Control Flow**:
   - Blynk app → Blynk cloud → Blynk library → BLYNK_WRITE handler → Hardware
   
3. **Sensor Flow** (Current):
   - CO2 Sensor (I2C) → readCO2() → Serial output + Blynk virtual pins V1, V2, V3 → Blynk cloud
   - Timer-based non-blocking reading (every 60 seconds)
   - Temperature (V1), Humidity (V2), and CO2 (V3) all sent to Blynk
   - Automatic control logic uses sensor data to control fan and humidifier
   
4. **State Flow**:
   - Events → State machine → State handlers → LED indicator → Visual feedback

## Key Abstractions

### 1. BlynkEdgent Class
- Encapsulates all Blynk.Edgent functionality
- Provides `begin()` and `run()` interface
- Manages state transitions and mode switching

### 2. ConfigStore Structure
- Packed struct for efficient storage
- Flag-based configuration options
- Default configuration fallback

### 3. Indicator Class
- Abstract LED control interface
- Supports multiple LED types (RGB, single, WS2812)
- Animation system (beat, wave patterns)

## Configuration Management

### Board Configuration
- Defined in `Settings.h`
- Supports multiple ESP32 board types
- Configurable pins for button and LED
- Board-specific settings via preprocessor defines

### Runtime Configuration
- WiFi SSID/Password
- Blynk Auth Token
- Cloud host/port
- Static IP settings (optional)
- Stored in Preferences, loaded on boot

## Error Handling

### Error Codes
- `BLYNK_PROV_ERR_NONE`: Success
- `BLYNK_PROV_ERR_CONFIG`: Invalid configuration
- `BLYNK_PROV_ERR_NETWORK`: WiFi connection failed
- `BLYNK_PROV_ERR_CLOUD`: Blynk cloud connection failed
- `BLYNK_PROV_ERR_TOKEN`: Invalid auth token
- `BLYNK_PROV_ERR_INTERNAL`: Other errors

### Error Recovery
- Errors stored in config for debugging
- Device enters ERROR state on failure
- Auto-restart after timeout
- Button can trigger reset during error

## Threading Model

### Main Thread
- Runs `BlynkEdgent.run()` loop continuously (every 10ms)
- Handles state machine transitions
- Processes Blynk events immediately (non-blocking)
- Timer-based CO2 sensor reading (every 60 seconds) using millis()
- Automatic control logic checks sensor values and updates fan/humidifier states
- Updates Blynk virtual pins only when values change (optimization)

### Indicator Thread (pthread)
- Separate thread for LED animations
- Non-blocking LED updates
- Independent timing

### Interrupt Handlers
- Button interrupt for reset functionality
- Non-blocking, sets flags for main thread

