#ifndef MOTOR_CONTROLLER_H
#define MOTOR_CONTROLLER_H

#include <Arduino.h>

class MotorController {
private:
    uint8_t leftMotorPWM;    // Left motor PWM pin
    uint8_t leftMotorDir1;   // Left motor direction pin 1
    uint8_t leftMotorDir2;   // Left motor direction pin 2
    uint8_t rightMotorPWM;   // Right motor PWM pin
    uint8_t rightMotorDir1;  // Right motor direction pin 1
    uint8_t rightMotorDir2;  // Right motor direction pin 2
    
    int leftSpeed;           // Current left motor speed (-255 to 255)
    int rightSpeed;          // Current right motor speed (-255 to 255)

    void setMotorSpeed(uint8_t pwmPin, uint8_t dir1, uint8_t dir2, int speed);

public:
    MotorController(uint8_t leftPWM, uint8_t leftD1, uint8_t leftD2,
                   uint8_t rightPWM, uint8_t rightD1, uint8_t rightD2);
    
    void begin();
    void setSpeed(int left, int right);  // Set speed for both motors (-255 to 255)
    void stop();                         // Stop both motors
    
    // High-level movement controls
    void moveForward(int speed);
    void moveBackward(int speed);
    void turnLeft(int speed);
    void turnRight(int speed);
};

#endif 