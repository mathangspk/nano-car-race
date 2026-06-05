#include "CarApplication.h"
#include "Config.h"

CarApplication::CarApplication()
    : _motorController(LEFT_MOTOR_PWM_PIN, LEFT_MOTOR_DIR1_PIN, LEFT_MOTOR_DIR2_PIN,
                       RIGHT_MOTOR_PWM_PIN, RIGHT_MOTOR_DIR1_PIN, RIGHT_MOTOR_DIR2_PIN),
      _turnChannel(TURN_CHANNEL_PIN),
      _moveChannel(MOVE_CHANNEL_PIN),
      _throttleChannel(THROTTLE_CHANNEL_PIN),
      _onTuneChannel(ON_TUNE_CHANNEL_PIN),
      _selectTuneChannel(SELECT_TUNE_CHANNEL_PIN),
      _buzzerController(BUZZER_PIN),
      _melodyManager(_buzzerController),
      _allSignalsValid(false) {}

void CarApplication::begin() {
    _turnChannel.begin();
    _moveChannel.begin();
    _throttleChannel.begin();
    _onTuneChannel.begin();
    _selectTuneChannel.begin();
    delay(CHANNEL_STABILIZE_MS);

    _motorController.begin();
    _buzzerController.begin();
    _melodyManager.begin();
}

void CarApplication::update() {
    uint16_t turnPulse = _turnChannel.getPulseWidth();
    uint16_t movePulse = _moveChannel.getPulseWidth();
    uint16_t throttlePulse = _throttleChannel.getPulseWidth();
    uint16_t onTunePulse = _onTuneChannel.getPulseWidth();
    uint16_t selectTunePulse = _selectTuneChannel.getPulseWidth();

    _allSignalsValid = validateAllSignals();

    _melodyManager.update(onTunePulse, selectTunePulse, _allSignalsValid);
    _buzzerController.update();

    int turnSpeed = 0;
    int moveSpeed = 0;
    int leftSpeed = 0;
    int rightSpeed = 0;
    float throttleScale = 1.0f;

    if (_allSignalsValid) {
        turnSpeed = isNeutral(turnPulse) ? 0 : mapRCToSpeed(turnPulse, RC_TURN_MIN, RC_TURN_MAX);
        moveSpeed = isNeutral(movePulse) ? 0 : mapRCToSpeed(movePulse, RC_MOVE_MIN, RC_MOVE_MAX);
        throttleScale = mapThrottleToScale(throttlePulse);

        turnSpeed = turnSpeed * throttleScale;
        moveSpeed = moveSpeed * throttleScale;

        leftSpeed = constrain(moveSpeed + turnSpeed, MOTOR_SPEED_MIN, MOTOR_SPEED_MAX);
        rightSpeed = constrain(moveSpeed - turnSpeed, MOTOR_SPEED_MIN, MOTOR_SPEED_MAX);

        _motorController.setSpeed(leftSpeed, rightSpeed);
    } else {
        _motorController.stop();
    }

    // Log values every 500ms for calibration
    static uint32_t lastLogTime = 0;
    uint32_t currentTime = millis();
    if (currentTime - lastLogTime >= 500) {
        lastLogTime = currentTime;
        Serial.print("Signals: ");
        Serial.print(_allSignalsValid ? "VALID" : "INVALID");
        Serial.print(" | RC Pulses - Turn: ");
        Serial.print(turnPulse);
        Serial.print(", Move: ");
        Serial.print(movePulse);
        Serial.print(", Throttle: ");
        Serial.print(throttlePulse);
        Serial.print(" | Speeds - Turn: ");
        Serial.print(turnSpeed);
        Serial.print(", Move: ");
        Serial.print(moveSpeed);
        Serial.print(" | Outputs L/R: ");
        Serial.print(leftSpeed);
        Serial.print("/");
        Serial.println(rightSpeed);
    }
}

bool CarApplication::validateAllSignals() {
    return _turnChannel.isSignalValid() &&
           _moveChannel.isSignalValid() &&
           _throttleChannel.isSignalValid() &&
           _onTuneChannel.isSignalValid() &&
           _selectTuneChannel.isSignalValid();
}

int CarApplication::mapRCToSpeed(uint16_t pulse, uint16_t rcMin, uint16_t rcMax) {
    pulse = constrain(pulse, rcMin, rcMax);
    return map(pulse, rcMin, rcMax, MOTOR_SPEED_MIN, MOTOR_SPEED_MAX);
}

float CarApplication::mapThrottleToScale(uint16_t pulse) {
    pulse = constrain(pulse, RC_THROTTLE_MIN, RC_THROTTLE_MAX);
    return THROTTLE_SCALE_MIN + (pulse - RC_THROTTLE_MIN) * ((THROTTLE_SCALE_MAX - THROTTLE_SCALE_MIN) / (float)(RC_THROTTLE_MAX - RC_THROTTLE_MIN));
}

bool CarApplication::isNeutral(uint16_t pulse) {
    return pulse >= RC_PULSE_NEUTRAL_MIN && pulse <= RC_PULSE_NEUTRAL_MAX;
}
