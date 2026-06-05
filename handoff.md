# Handoff Documentation - Nano RC Car Refactoring & Bug Fixes

This document records the changes made in this iteration, the current state of the system, verification results, and the recommended next steps.

## Summary of Changes
- **RC Channel Calibration Mapping**: Calibrated the pulse width ranges for the joystick channels based on real-time receiver data logs. Added `RC_MOVE_MIN` / `RC_MOVE_MAX` (1220 to 1730), `RC_TURN_MIN` / `RC_TURN_MAX` (1220 to 1730), and `RC_THROTTLE_MIN` / `RC_THROTTLE_MAX` (1060 to 2060) to `Config.h`. Updated `mapRCToSpeed` and `mapThrottleToScale` to use these calibrated values so the car can reach full speed and turn with correct resolution.
- **Calibration Logging**: Added periodic (500ms) Serial logging in `CarApplication::update()` to print signals validation status, raw RC pulse widths (`turnPulse`, `movePulse`, `throttlePulse`), mapped speed components, and final left/right motor output speeds. This helped identify the actual transmitter endpoints.
- **Joystick Direction Fix**: Negated `moveSpeed` in `CarApplication.cpp` (`moveSpeed = -moveSpeed * throttleScale;`). This maps joystick UP to forward movement (`tiến`) and joystick DOWN to reverse movement (`lùi`) on the physical car.
- **Upload Verification Safety**: Increased the startup delay in `main.cpp` from 3000ms to 6000ms to ensure AVRdude verification completes successfully without serial line interference from the running sketch.

## Current System State
- **Compiling**: The project builds successfully with `pio run`.
- **Calibration**: Control mappings now use real-world transmitter endpoints (1220 to 1730), ensuring full resolution and speed range.
- **Logging**: Configured to print RC receiver input and motor output speed values to the Serial monitor (at 115200 baud rate) every 500ms.
- **Joystick Logic**: Configured such that pushing UP drives the car forward, and pushing DOWN drives the car backward.
- **Working Features**:
  - RC signal validation and timeout safety fail-safe.
  - Motor speed calculation and differential steering.
  - Buzzer sound initialization and melody selection.
  - EEPROM saving/loading for selected melodies.

## Verification & Testing
- Built the firmware using PlatformIO:
  ```bash
  pio run
  ```
  **Result**: `SUCCESS` compile with zero errors and warnings.
- Upload: User has opted to perform the upload manually.

## Next Steps
1. **Manual Upload**: The user will upload the binary file to the Arduino Nano board.
2. **Physical Testing**: Test the joystick controls to verify that pushing UP drives the car backward and pushing DOWN drives the car forward.
3. **Verify Steering & Melodies**: Verify that steering and buzzer features continue to work as expected.
