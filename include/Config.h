#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// ============================================================================
// PIN CONFIGURATION
// ============================================================================

// Motor Control Pins (Arduino Nano)
#define LEFT_MOTOR_PWM_PIN      9
#define LEFT_MOTOR_DIR1_PIN     7
#define LEFT_MOTOR_DIR2_PIN     8
#define RIGHT_MOTOR_PWM_PIN     10
#define RIGHT_MOTOR_DIR1_PIN    11
#define RIGHT_MOTOR_DIR2_PIN    12

// Buzzer Pin
#define BUZZER_PIN              A0

// RC Channel Pins
#define TURN_CHANNEL_PIN        2   // External Interrupt
#define MOVE_CHANNEL_PIN        3   // External Interrupt
#define THROTTLE_CHANNEL_PIN    4   // Pin Change Interrupt
#define ON_TUNE_CHANNEL_PIN     5   // Pin Change Interrupt
#define SELECT_TUNE_CHANNEL_PIN 6   // Pin Change Interrupt

// ============================================================================
// RC SIGNAL CONFIGURATION
// ============================================================================

// RC Pulse Width Ranges
#define RC_PULSE_MIN            900
#define RC_PULSE_MAX            2500
#define RC_PULSE_NEUTRAL_MIN    1460
#define RC_PULSE_NEUTRAL_MAX    1540
#define RC_PULSE_CENTER         1500

// RC Signal Validation
#define SIGNAL_TIMEOUT_MS       1000
#define RC_PULSE_VALID_MIN      900
#define RC_PULSE_VALID_MAX      2500

// ============================================================================
// MOTOR CONTROL CONFIGURATION
// ============================================================================

// Throttle Scaling
#define THROTTLE_SCALE_MIN      0.35f
#define THROTTLE_SCALE_MAX      1.0f

// Motor Speed Limits
#define MOTOR_SPEED_MIN         -255
#define MOTOR_SPEED_MAX         255

// ============================================================================
// MELODY CONTROL CONFIGURATION
// ============================================================================

// EEPROM Address
#define EEPROM_MELODY_ADDR      0

// Melody Control Ranges
#define MELODY_OFF_MIN          1200
#define MELODY_OFF_MAX          1700
#define MELODY_DEFAULT_MIN      950
#define MELODY_DEFAULT_MAX      1200
#define MELODY_SELECT_MIN       1500
#define MELODY_SELECT_MAX       2100
#define SELECT_TRIGGER_MIN      1400
#define SELECT_TRIGGER_MAX      1600

// ============================================================================
// SYSTEM CONFIGURATION
// ============================================================================

// Serial Communication
#define SERIAL_BAUD_RATE        115200

// Timing
#define LOOP_DELAY_MS           10
#define CHANNEL_STABILIZE_MS    1000

// ============================================================================
// DEBUG CONFIGURATION
// ============================================================================

// Uncomment to enable debug output
// #define DEBUG_ENABLED

#ifdef DEBUG_ENABLED
    #define DEBUG_PRINT(x) Serial.print(x)
    #define DEBUG_PRINTLN(x) Serial.println(x)
#else
    #define DEBUG_PRINT(x)
    #define DEBUG_PRINTLN(x)
#endif

#endif // CONFIG_H 