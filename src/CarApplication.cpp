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

    if (_allSignalsValid) {
        int turnSpeed = isNeutral(turnPulse) ? 0 : mapRCToSpeed(turnPulse);
        int moveSpeed = isNeutral(movePulse) ? 0 : mapRCToSpeed(movePulse);
        float throttleScale = mapThrottleToScale(throttlePulse);

        turnSpeed = turnSpeed * throttleScale;
        moveSpeed = -moveSpeed * throttleScale;

        int leftSpeed = constrain(moveSpeed + turnSpeed, MOTOR_SPEED_MIN, MOTOR_SPEED_MAX);
        int rightSpeed = constrain(moveSpeed - turnSpeed, MOTOR_SPEED_MIN, MOTOR_SPEED_MAX);

        _motorController.setSpeed(leftSpeed, rightSpeed);
    } else {
        _motorController.stop();
    }
}

bool CarApplication::validateAllSignals() {
    return _turnChannel.isSignalValid() &&
           _moveChannel.isSignalValid() &&
           _throttleChannel.isSignalValid() &&
           _onTuneChannel.isSignalValid() &&
           _selectTuneChannel.isSignalValid();
}

int CarApplication::mapRCToSpeed(uint16_t pulse) {
    pulse = constrain(pulse, 1000, 2000);
    return map(pulse, 1000, 2000, MOTOR_SPEED_MIN, MOTOR_SPEED_MAX);
}

float CarApplication::mapThrottleToScale(uint16_t pulse) {
    pulse = constrain(pulse, 1000, 2000);
    return THROTTLE_SCALE_MIN + (pulse - 1000) * ((THROTTLE_SCALE_MAX - THROTTLE_SCALE_MIN) / 1000.0f);
}

bool CarApplication::isNeutral(uint16_t pulse) {
    return pulse >= RC_PULSE_NEUTRAL_MIN && pulse <= RC_PULSE_NEUTRAL_MAX;
}
