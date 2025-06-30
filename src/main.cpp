#include <Arduino.h>
#include <SoftwareSerial.h>
#include "Config.h"
#include "MotorController.h"
#include "RCChannel.h"
#include "BuzzerController.h"
#include "Melodies.h"
#include <EEPROM.h>

// Create instances of our classes
MotorController motorController(
    LEFT_MOTOR_PWM_PIN, LEFT_MOTOR_DIR1_PIN, LEFT_MOTOR_DIR2_PIN,
    RIGHT_MOTOR_PWM_PIN, RIGHT_MOTOR_DIR1_PIN, RIGHT_MOTOR_DIR2_PIN);

// Create RCChannel instances (unified class handles both interrupt types)
RCChannel turnChannel(TURN_CHANNEL_PIN);      // External Interrupt
RCChannel moveChannel(MOVE_CHANNEL_PIN);      // External Interrupt
RCChannel throttleChannel(THROTTLE_CHANNEL_PIN);  // Pin Change Interrupt
RCChannel onTuneChannel(ON_TUNE_CHANNEL_PIN);    // Pin Change Interrupt
RCChannel selectTuneChannel(SELECT_TUNE_CHANNEL_PIN); // Pin Change Interrupt

// Create BuzzerController instance
BuzzerController buzzerController(BUZZER_PIN);

// Melody control state
struct MelodyState {
    BuzzerMode currentMode = BuzzerMode::OFF;
    BuzzerMode previousMode = BuzzerMode::OFF;
    uint8_t currentMelodyIndex = 0;
    bool melodyActive = false;
    bool melodyInitialized = false;
    bool readyToSelectNext = false;
    uint16_t prevSelectPulse = RC_PULSE_CENTER;
} melodyState;

// Signal validation state
struct SignalState {
    bool allSignalsValid = false;
    unsigned long lastValidSignalTime = 0;
} signalState;

// Helper functions
int mapRCToSpeed(uint16_t pulse) {
    pulse = constrain(pulse, 1000, 2000);
    return map(pulse, 1000, 2000, MOTOR_SPEED_MIN, MOTOR_SPEED_MAX);
}

float mapThrottleToScale(uint16_t pulse) {
    pulse = constrain(pulse, 1000, 2000);
    return THROTTLE_SCALE_MIN + (pulse - 1000) * ((THROTTLE_SCALE_MAX - THROTTLE_SCALE_MIN) / 1000.0f);
}

bool isInRange(uint16_t value, uint16_t min, uint16_t max) {
    return value >= min && value <= max;
}

bool isNeutral(uint16_t pulse) {
    return isInRange(pulse, RC_PULSE_NEUTRAL_MIN, RC_PULSE_NEUTRAL_MAX);
}

bool validateAllSignals() {
    bool turnValid = turnChannel.isSignalValid();
    bool moveValid = moveChannel.isSignalValid();
    bool throttleValid = throttleChannel.isSignalValid();
    bool onTuneValid = onTuneChannel.isSignalValid();
    bool selectTuneValid = selectTuneChannel.isSignalValid();
    
    signalState.allSignalsValid = turnValid && moveValid && throttleValid && onTuneValid && selectTuneValid;
    
    if (signalState.allSignalsValid) {
        signalState.lastValidSignalTime = millis();
    }
    
    return signalState.allSignalsValid;
}

void handleMelodyControl(uint16_t onTunePulse, uint16_t selectTunePulse) {
    // Check for signal timeout
    if (millis() - signalState.lastValidSignalTime > SIGNAL_TIMEOUT_MS) {
        melodyState.currentMode = BuzzerMode::ERROR;
        return;
    }

    // Melody OFF mode
    if (isInRange(onTunePulse, MELODY_OFF_MIN, MELODY_OFF_MAX)) {
        melodyState.currentMode = BuzzerMode::OFF;
        melodyState.melodyActive = false;
        melodyState.melodyInitialized = false;
        return;
    }

    // Default melody mode
    if (isInRange(onTunePulse, MELODY_DEFAULT_MIN, MELODY_DEFAULT_MAX)) {
        if (!melodyState.melodyActive) {
            DEBUG_PRINTLN(">> Play default melody");
            melodyState.currentMelodyIndex = 1;
            buzzerController.setCustomMelody(melodies[melodyState.currentMelodyIndex]);
            melodyState.currentMode = BuzzerMode::CUSTOM;
            melodyState.melodyActive = true;
        }
        melodyState.melodyInitialized = false;
        return;
    }

    // Melody selection mode
    if (isInRange(onTunePulse, MELODY_SELECT_MIN, MELODY_SELECT_MAX)) {
        melodyState.melodyActive = false;

        // Initialize selection mode
        if (!melodyState.melodyInitialized) {
            melodyState.melodyInitialized = true;
            melodyState.currentMelodyIndex = EEPROM.read(EEPROM_MELODY_ADDR);
            if (melodyState.currentMelodyIndex >= melodyCount) {
                melodyState.currentMelodyIndex = 0;
            }

            DEBUG_PRINT(">> Enter selection mode, playing saved melody index: ");
            DEBUG_PRINTLN(melodyState.currentMelodyIndex);
            buzzerController.setCustomMelody(melodies[melodyState.currentMelodyIndex]);
            melodyState.currentMode = BuzzerMode::CUSTOM;
        }

        // Handle melody selection
        if (selectTunePulse > RC_PULSE_CENTER && melodyState.prevSelectPulse <= RC_PULSE_CENTER) {
            melodyState.readyToSelectNext = true;
        }

        if (isInRange(selectTunePulse, SELECT_TRIGGER_MIN, SELECT_TRIGGER_MAX) && melodyState.readyToSelectNext) {
            melodyState.readyToSelectNext = false;
            melodyState.currentMelodyIndex++;
            if (melodyState.currentMelodyIndex >= melodyCount) {
                melodyState.currentMelodyIndex = 0;
            }

            EEPROM.write(EEPROM_MELODY_ADDR, melodyState.currentMelodyIndex);
            buzzerController.setCustomMelody(melodies[melodyState.currentMelodyIndex]);
            melodyState.currentMode = BuzzerMode::CUSTOM;

            DEBUG_PRINT("Selected melody index: ");
            DEBUG_PRINTLN(melodyState.currentMelodyIndex);
        }

        melodyState.prevSelectPulse = selectTunePulse;
        return;
    }

    // No melody mode
    if (melodyState.melodyActive || melodyState.melodyInitialized) {
        DEBUG_PRINTLN(">> Stop playing melody");
        melodyState.melodyActive = false;
        melodyState.melodyInitialized = false;
        melodyState.currentMode = BuzzerMode::OFF;
    }
}

void setup() {
    Serial.begin(SERIAL_BAUD_RATE);
    Serial.println("\nNano RC Car Controller Starting...");

    // Initialize RC channels
    turnChannel.begin();
    moveChannel.begin();
    throttleChannel.begin();
    onTuneChannel.begin();
    selectTuneChannel.begin();
    delay(CHANNEL_STABILIZE_MS); // Wait for channels to stabilize
    Serial.println("RC channels initialized.");

    // Initialize components
    motorController.begin();
    Serial.println("System initialized and ready!");
}

void loop() {
    // Read RC channel values
    uint16_t turnPulse = turnChannel.getPulseWidth();
    uint16_t movePulse = moveChannel.getPulseWidth();
    uint16_t throttlePulse = throttleChannel.getPulseWidth();
    uint16_t onTunePulse = onTuneChannel.getPulseWidth();
    uint16_t selectTunePulse = selectTuneChannel.getPulseWidth();

    // Validate all signals
    validateAllSignals();

    // Handle melody control
    handleMelodyControl(onTunePulse, selectTunePulse);

    // Update buzzer if mode changed
    if (melodyState.currentMode != melodyState.previousMode) {
        buzzerController.setMode(melodyState.currentMode);
        melodyState.previousMode = melodyState.currentMode;
    }

    buzzerController.update();

    // Only control motors if signals are valid
    if (signalState.allSignalsValid) {
        // Calculate motor speeds
        int turnSpeed = mapRCToSpeed(turnPulse);
        if (isNeutral(turnPulse)) {
            turnSpeed = 0;
        }

        int moveSpeed = mapRCToSpeed(movePulse);
        if (isNeutral(movePulse)) {
            moveSpeed = 0;
        }

        float throttleScale = mapThrottleToScale(throttlePulse);

        // Apply throttle scaling
        turnSpeed = turnSpeed * throttleScale;
        moveSpeed = moveSpeed * throttleScale;

        // Calculate individual wheel speeds
        int leftSpeed = constrain(turnSpeed - moveSpeed, MOTOR_SPEED_MIN, MOTOR_SPEED_MAX);
        int rightSpeed = constrain(turnSpeed + moveSpeed, MOTOR_SPEED_MIN, MOTOR_SPEED_MAX);

        // Set motor speeds
        motorController.setSpeed(leftSpeed, rightSpeed);
    } else {
        // Stop motors if signals are invalid
        motorController.stop();
    }

    delay(LOOP_DELAY_MS);
}
