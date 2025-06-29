#ifndef MOTOR_CONTROLLER_H
#define MOTOR_CONTROLLER_H

#include <Arduino.h>

class MotorController
{
private:
    uint8_t propellerPWM;
    uint8_t propellerDir1;
    uint8_t propellerDir2;
    int propellerSpeed;

    void setMotorSpeed(uint8_t pwmPin, uint8_t dir1, uint8_t dir2, int speed);

public:
    MotorController(uint8_t pwm, uint8_t d1, uint8_t d2);

    void begin();
    void setSpeed(int speed); // Set propeller speed (-255 to 255)
    void stop();
    void moveForward(int speed);
    void moveBackward(int speed);
};

#endif
