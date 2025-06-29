#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// EEPROM addresses
#define EEPROM_MELODY_ADDR 0

// Debug settings
#define DEBUG_MODE 1  // Set to 0 to disable debug output

// Pin definitions for motor control (Arduino Nano)
namespace Config {
    // Motor pins
    const uint8_t PROPELLER_PWM = 9;
    const uint8_t PROPELLER_DIR1 = 7;
    const uint8_t PROPELLER_DIR2 = 8;
    const uint8_t RIGHT_MOTOR_PWM = 10;
    const uint8_t RIGHT_MOTOR_DIR1 = 11;
    const uint8_t RIGHT_MOTOR_DIR2 = 12;
    
    // RC Channel pins
    const uint8_t TURN_CHANNEL_PIN = 2;
    const uint8_t MOVE_CHANNEL_PIN = 3;
    const uint8_t THROTTLE_CHANNEL_PIN = 4;
    const uint8_t ON_TUNE_CHANNEL_PIN = 5;
    const uint8_t SELECT_TUNE_CHANNEL_PIN = 6;
    
    // Buzzer pin
    const uint8_t BUZZER_PIN = A0;
    
    // RC signal ranges
    const uint16_t RC_MIN_PULSE = 1000;
    const uint16_t RC_MAX_PULSE = 2000;
    const uint16_t RC_CENTER_PULSE = 1500;
    const uint16_t RC_ERROR_MIN = 900;
    const uint16_t RC_ERROR_MAX = 2100;
    
    // Deadband settings
    const uint16_t MOVE_DEADBAND_MIN = 1460;
    const uint16_t MOVE_DEADBAND_MAX = 1540;
    
    // Throttle scaling
    const float THROTTLE_MIN_SCALE = 0.35f;
    const float THROTTLE_MAX_SCALE = 1.0f;
    
    // Buzzer mode thresholds
    const uint16_t ON_TUNE_OFF_MIN = 1200;
    const uint16_t ON_TUNE_OFF_MAX = 1700;
    const uint16_t ON_TUNE_DEFAULT_MIN = 950;
    const uint16_t ON_TUNE_DEFAULT_MAX = 1200;
    const uint16_t ON_TUNE_SELECT_MIN = 1500;
    const uint16_t ON_TUNE_SELECT_MAX = 2100;
    
    // Melody selection thresholds
    const uint16_t SELECT_TUNE_THRESHOLD = 1500;
    const uint16_t SELECT_TUNE_EXECUTE_MIN = 1400;
    const uint16_t SELECT_TUNE_EXECUTE_MAX = 1600;
}

#endif 