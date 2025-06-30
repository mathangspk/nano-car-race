# Nano RC Car Controller

A sophisticated Arduino Nano-based RC car controller with motor control, buzzer melodies, and unified RC signal handling.

## 🚗 Features

- **Dual Motor Control**: Independent control of left and right motors with PWM and direction control
- **Unified RC Signal Handling**: Automatic detection and use of appropriate interrupt types (External Interrupt or Pin Change Interrupt)
- **Melody System**: Multiple melodies with selection and EEPROM storage
- **Signal Validation**: Robust signal validation with timeout and fail-safe mechanisms
- **Throttle Scaling**: Intelligent throttle control with configurable scaling
- **Modular Design**: Clean, maintainable code structure with separate modules

## 📋 Requirements

### Hardware
- Arduino Nano (ATmega328P)
- 2x DC Motors with motor driver (L298N or similar)
- RC Receiver (compatible with PWM signals)
- Buzzer/Speaker
- Power supply (7-12V recommended)

### Software
- PlatformIO IDE or Arduino IDE
- Arduino framework

## 🔧 Pin Configuration

### Motor Control
| Function | Pin | Description |
|----------|-----|-------------|
| Left Motor PWM | 9 | PWM speed control |
| Left Motor DIR1 | 7 | Direction control 1 |
| Left Motor DIR2 | 8 | Direction control 2 |
| Right Motor PWM | 10 | PWM speed control |
| Right Motor DIR1 | 11 | Direction control 1 |
| Right Motor DIR2 | 12 | Direction control 2 |

### RC Channels
| Channel | Pin | Interrupt Type | Function |
|---------|-----|----------------|----------|
| Turn | 2 | External (INT0) | Steering control |
| Move | 3 | External (INT1) | Forward/backward |
| Throttle | 4 | Pin Change (PCINT16) | Speed scaling |
| On Tune | 5 | Pin Change (PCINT17) | Melody on/off |
| Select Tune | 6 | Pin Change (PCINT18) | Melody selection |

### Other
| Function | Pin | Description |
|----------|-----|-------------|
| Buzzer | A0 | Audio output |

## 🏗️ Project Structure

```
nano-race-sbus/
├── include/                 # Header files
│   ├── Config.h            # Centralized configuration
│   ├── MotorController.h   # Motor control interface
│   ├── RCChannel.h         # Unified RC signal handling
│   ├── BuzzerController.h  # Buzzer and melody control
│   ├── Melodies.h          # Melody definitions
│   └── Melody.h            # Melody data structure
├── src/                    # Source files
│   ├── main.cpp            # Main application logic
│   ├── MotorController.cpp # Motor control implementation
│   ├── RCChannel.cpp       # RC signal handling implementation
│   ├── BuzzerController.cpp # Buzzer control implementation
│   └── Melodies.cpp        # Melody implementations
├── lib/                    # External libraries
│   └── FUTABA_SBUS/        # SBUS protocol library
├── platformio.ini          # PlatformIO configuration
└── README.md               # This file
```

## 🚀 Setup Instructions

### 1. Hardware Setup

1. **Motor Connection**:
   - Connect motor driver to Arduino pins as defined in `Config.h`
   - Ensure proper power supply for motors (separate from Arduino)

2. **RC Receiver**:
   - Connect RC receiver channels to specified pins
   - Ensure receiver is powered and bound to transmitter

3. **Buzzer**:
   - Connect buzzer to pin A0 (or modify `BUZZER_PIN` in `Config.h`)

### 2. Software Setup

1. **Install PlatformIO** (recommended):
   ```bash
   # Clone or download the project
   cd nano-race-sbus
   
   # Build the project
   pio run
   
   # Upload to Arduino Nano
   pio run --target upload
   
   # Monitor serial output
   pio device monitor
   ```

2. **Arduino IDE**:
   - Open `src/main.cpp` in Arduino IDE
   - Install required libraries
   - Select Arduino Nano board
   - Upload the sketch

## 🎮 Usage

### RC Control
- **Turn Channel**: Controls left/right steering
- **Move Channel**: Controls forward/backward movement
- **Throttle Channel**: Scales overall speed (35% - 100%)
- **On Tune Channel**: Controls melody system
- **Select Tune Channel**: Selects different melodies

### Melody System

#### Melody Modes
1. **OFF Mode** (1200-1700): No melody playing
2. **Default Mode** (950-1200): Plays default melody
3. **Selection Mode** (1500-2100): Allows melody selection

#### Melody Selection
1. Set On Tune channel to Selection Mode
2. Move Select Tune channel above 1500 to enable selection
3. Return Select Tune to center (1400-1600) to change melody
4. Selected melody is automatically saved to EEPROM

### Signal Validation
- All RC signals are validated for proper range (900-2500μs)
- System stops motors if signals are invalid or timeout
- Error melody plays when signal issues are detected

## ⚙️ Configuration

All configuration is centralized in `include/Config.h`:

### Pin Configuration
```cpp
#define LEFT_MOTOR_PWM_PIN      9
#define LEFT_MOTOR_DIR1_PIN     7
// ... more pin definitions
```

### RC Signal Ranges
```cpp
#define RC_PULSE_MIN            900
#define RC_PULSE_MAX            2500
#define RC_PULSE_NEUTRAL_MIN    1460
#define RC_PULSE_NEUTRAL_MAX    1540
```

### Motor Control
```cpp
#define THROTTLE_SCALE_MIN      0.35f
#define THROTTLE_SCALE_MAX      1.0f
#define MOTOR_SPEED_MIN         -255
#define MOTOR_SPEED_MAX         255
```

### Debug Mode
Uncomment in `Config.h` to enable debug output:
```cpp
#define DEBUG_ENABLED
```

## 🔍 Technical Details

### Interrupt Handling
The system automatically detects and uses the appropriate interrupt type:
- **External Interrupts**: Pins 2, 3, 21 (higher precision)
- **Pin Change Interrupts**: Pins 0-7 (more channels available)

### Signal Processing
- RC pulse width measurement with microsecond precision
- Automatic signal validation and timeout detection
- Fail-safe motor stopping on signal loss

### Motor Control Algorithm
```
Left Speed = (Turn Speed - Move Speed) × Throttle Scale
Right Speed = (Turn Speed + Move Speed) × Throttle Scale
```

## 🐛 Troubleshooting

### Common Issues

1. **Motors not responding**:
   - Check motor driver connections
   - Verify power supply voltage
   - Check RC signal validity

2. **RC signals not detected**:
   - Verify receiver connections
   - Check transmitter binding
   - Ensure proper pin assignments

3. **Buzzer not working**:
   - Check buzzer connection to pin A0
   - Verify buzzer polarity

4. **Compilation errors**:
   - Ensure all required libraries are installed
   - Check PlatformIO configuration

### Debug Output
Enable debug mode in `Config.h` to see detailed serial output:
```
Nano RC Car Controller Starting...
RC channels initialized.
System initialized and ready!
>> Play default melody
Selected melody index: 2
```

## 📝 License

This project is open source. Feel free to modify and distribute.

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly
5. Submit a pull request

## 📞 Support

For issues and questions:
1. Check the troubleshooting section
2. Review the code comments
3. Open an issue on the repository

---

**Happy RC Car Driving! 🏎️** 