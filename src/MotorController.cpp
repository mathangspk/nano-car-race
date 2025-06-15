#include "MotorController.h"

MotorController::MotorController(uint8_t leftPWM, uint8_t leftD1, uint8_t leftD2,
                               uint8_t rightPWM, uint8_t rightD1, uint8_t rightD2)
    : leftMotorPWM(leftPWM), leftMotorDir1(leftD1), leftMotorDir2(leftD2),
      rightMotorPWM(rightPWM), rightMotorDir1(rightD1), rightMotorDir2(rightD2),
      leftSpeed(0), rightSpeed(0) {
}

void MotorController::begin() {
    pinMode(leftMotorPWM, OUTPUT);
    pinMode(leftMotorDir1, OUTPUT);
    pinMode(leftMotorDir2, OUTPUT);
    pinMode(rightMotorPWM, OUTPUT);
    pinMode(rightMotorDir1, OUTPUT);
    pinMode(rightMotorDir2, OUTPUT);
    stop();
}

void MotorController::setMotorSpeed(uint8_t pwmPin, uint8_t dir1, uint8_t dir2, int speed) {
    if (speed > 255) speed = 255;
    if (speed < -255) speed = -255;
    
    if (speed > 0) {
        digitalWrite(dir1, HIGH);
        digitalWrite(dir2, LOW);
        analogWrite(pwmPin, speed);
    } else if (speed < 0) {
        digitalWrite(dir1, LOW);
        digitalWrite(dir2, HIGH);
        analogWrite(pwmPin, -speed);
    } else {
        digitalWrite(dir1, LOW);
        digitalWrite(dir2, LOW);
        analogWrite(pwmPin, 0);
    }
}

void MotorController::setSpeed(int left, int right) {
    leftSpeed = left;
    rightSpeed = right;
    setMotorSpeed(leftMotorPWM, leftMotorDir1, leftMotorDir2, left);
    setMotorSpeed(rightMotorPWM, rightMotorDir1, rightMotorDir2, right);
}

void MotorController::stop() {
    setSpeed(0, 0);
}

void MotorController::moveForward(int speed) {
    setSpeed(speed, speed);
}

void MotorController::moveBackward(int speed) {
    setSpeed(-speed, -speed);
}

void MotorController::turnLeft(int speed) {
    setSpeed(speed/2, speed);
}

void MotorController::turnRight(int speed) {
    setSpeed(speed, speed/2);
} 