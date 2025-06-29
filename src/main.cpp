#include <Arduino.h>
#include <SoftwareSerial.h>
#include "Config.h"
#include "MotorController.h"
#include "RCChannel.h"
#include "RCChannelPCINT.h"
#include "BuzzerController.h"
#include "BuzzerStateManager.h"
#include "Melodies.h"
#include <EEPROM.h>
// include <Servo.h>

#define EEPROM_MELODY_ADDR 0
#define DEBUG_MODE 1  // Set to 0 to disable debug output

// Create instances of our classes
MotorController motorController(
    Config::PROPELLER_PWM, Config::PROPELLER_DIR1, Config::PROPELLER_DIR2);

// Create RCChannel instances
RCChannel turnChannel(Config::TURN_CHANNEL_PIN);
RCChannel moveChannel(Config::MOVE_CHANNEL_PIN);
RCChannelPCINT throttleChannel(Config::THROTTLE_CHANNEL_PIN); // Using PCINT for pin 21 (INT2 on Nano)
RCChannelPCINT onTuneChannel(Config::ON_TUNE_CHANNEL_PIN);
RCChannelPCINT selectTuneChannel(Config::SELECT_TUNE_CHANNEL_PIN);

// Create BuzzerController instance
BuzzerController buzzerController(Config::BUZZER_PIN);

// Create buzzer state manager instance
BuzzerStateManager buzzerManager(buzzerController);

// Helper functions
int mapRCToSpeed(uint16_t pulse) {
    pulse = constrain(pulse, Config::RC_MIN_PULSE, Config::RC_MAX_PULSE);
    return map(pulse, Config::RC_MIN_PULSE, Config::RC_MAX_PULSE, -255, 255);
}

float mapThrottleToScale(uint16_t pulse) {
    pulse = constrain(pulse, Config::RC_MIN_PULSE, Config::RC_MAX_PULSE);
    return Config::THROTTLE_MIN_SCALE + (pulse - Config::RC_MIN_PULSE) * 
           ((Config::THROTTLE_MAX_SCALE - Config::THROTTLE_MIN_SCALE) / 
            (Config::RC_MAX_PULSE - Config::RC_MIN_PULSE));
}

void printDebugInfo(uint16_t turn, uint16_t move, uint16_t throttle, uint16_t onTune, uint16_t selectTune) {
#if DEBUG_MODE
    Serial.print("Channel 1 Pulse Width: ");
    Serial.println(turn);
    Serial.print("Channel 2 Pulse Width: ");
    Serial.println(move);
    Serial.print("Channel 3 Pulse Width: ");
    Serial.println(throttle);
    Serial.print("Channel 4 Pulse Width: ");
    Serial.println(onTune);
    Serial.print("Channel 5 Pulse Width: ");
    Serial.println(selectTune);
#endif
}

void handleMotorControl(uint16_t move, uint16_t throttle) {
    // Check if RC is connected
    if (!buzzerManager.isRCConnected(throttle)) {
        motorController.stop();
        return;
    }

    int moveSpeed = mapRCToSpeed(move);
    
    // Add deadband for move signal (forward/backward control channel)
    if (move >= Config::MOVE_DEADBAND_MIN && move <= Config::MOVE_DEADBAND_MAX) {
        moveSpeed = 0;
    }
    
    float throttleScale = mapThrottleToScale(throttle);
    
#if DEBUG_MODE
    Serial.print("Throttle Scale: ");
    Serial.println(throttleScale);
    Serial.print("Propeller Speed: ");
    Serial.println(moveSpeed * throttleScale);
#endif
    
    int propellerSpeed = moveSpeed * throttleScale;
    motorController.setSpeed(propellerSpeed);
}

void setup() {
    // Initialize debug serial port
    Serial.begin(115200);
    Serial.println("\nNano RC Car Controller Starting...");
    
    // Initialize RC channels
    turnChannel.begin();
    moveChannel.begin();
    throttleChannel.begin();
    onTuneChannel.begin();
    selectTuneChannel.begin();
    delay(1000); // Wait for channels to stabilize
    Serial.println("RC channels initialized.");
    
    // Initialize components
    motorController.begin();
    Serial.println("System initialized and ready!");
}

void loop() {
    // Read RC channel values
    uint16_t turn = turnChannel.getPulseWidth();
    uint16_t move = moveChannel.getPulseWidth();
    uint16_t throttle = throttleChannel.getPulseWidth();
    uint16_t onTune = onTuneChannel.getPulseWidth();
    uint16_t selectTune = selectTuneChannel.getPulseWidth();

    // Print debug information
    printDebugInfo(turn, move, throttle, onTune, selectTune);

    // Handle buzzer mode and melody selection
    buzzerManager.handleBuzzerMode(onTune, selectTune);

    // Handle motor control
    handleMotorControl(move, throttle);

    // Small delay to avoid flooding the serial output
    delay(10);
}
