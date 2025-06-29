#include "MotorController.h"

MotorController::MotorController(uint8_t pwm, uint8_t d1, uint8_t d2)
    : propellerPWM(pwm), propellerDir1(d1), propellerDir2(d2), propellerSpeed(0) {}

void MotorController::begin()
{
    pinMode(propellerPWM, OUTPUT);
    pinMode(propellerDir1, OUTPUT);
    pinMode(propellerDir2, OUTPUT);
    stop();
}

void MotorController::setMotorSpeed(uint8_t pwmPin, uint8_t dir1, uint8_t dir2, int speed)
{
    speed = constrain(speed, -255, 255);

    if (speed > 0)
    {
        digitalWrite(dir1, HIGH);
        digitalWrite(dir2, LOW);
        analogWrite(pwmPin, speed);
    }
    else if (speed < 0)
    {
        digitalWrite(dir1, LOW);
        digitalWrite(dir2, HIGH);
        analogWrite(pwmPin, -speed);
    }
    else
    {
        digitalWrite(dir1, LOW);
        digitalWrite(dir2, LOW);
        analogWrite(pwmPin, 0);
    }
}

void MotorController::setSpeed(int speed)
{
    propellerSpeed = speed;
    setMotorSpeed(propellerPWM, propellerDir1, propellerDir2, speed);
}

void MotorController::stop()
{
    setSpeed(0);
}

void MotorController::moveForward(int speed)
{
    setSpeed(speed);
}

void MotorController::moveBackward(int speed)
{
    setSpeed(-speed);
}
