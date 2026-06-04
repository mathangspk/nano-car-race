#include "MelodyManager.h"
#include "Config.h"
#include "Melodies.h"
#include <EEPROM.h>

MelodyManager::MelodyManager(BuzzerController& buzzer)
    : _buzzer(buzzer), _currentMode(BuzzerMode::OFF), _currentMelodyIndex(0),
      _melodyActive(false), _melodyInitialized(false), _readyToSelectNext(false),
      _prevSelectPulse(RC_PULSE_CENTER) {}

void MelodyManager::begin() {
    _currentMelodyIndex = EEPROM.read(EEPROM_MELODY_ADDR);
    if (_currentMelodyIndex >= melodyCount) {
        _currentMelodyIndex = 0;
    }
}

void MelodyManager::update(uint16_t onTunePulse, uint16_t selectTunePulse, bool signalValid) {
    if (!signalValid) {
        _currentMode = BuzzerMode::ERROR;
        _buzzer.setMode(_currentMode);
        return;
    }

    if (onTunePulse >= MELODY_OFF_MIN && onTunePulse <= MELODY_OFF_MAX) {
        _currentMode = BuzzerMode::OFF;
        _melodyActive = false;
        _melodyInitialized = false;
        _buzzer.setMode(_currentMode);
        return;
    }

    if (onTunePulse >= MELODY_DEFAULT_MIN && onTunePulse <= MELODY_DEFAULT_MAX) {
        if (!_melodyActive) {
            _currentMelodyIndex = 1;
            _buzzer.setCustomMelody(melodies[_currentMelodyIndex]);
            _currentMode = BuzzerMode::CUSTOM;
            _buzzer.setMode(_currentMode);
            _melodyActive = true;
        }
        _melodyInitialized = false;
        return;
    }

    if (onTunePulse >= MELODY_SELECT_MIN && onTunePulse <= MELODY_SELECT_MAX) {
        _melodyActive = false;

        if (!_melodyInitialized) {
            _melodyInitialized = true;
            _currentMelodyIndex = EEPROM.read(EEPROM_MELODY_ADDR);
            if (_currentMelodyIndex >= melodyCount) {
                _currentMelodyIndex = 0;
            }
            _buzzer.setCustomMelody(melodies[_currentMelodyIndex]);
            _currentMode = BuzzerMode::CUSTOM;
            _buzzer.setMode(_currentMode);
        }

        if (selectTunePulse > RC_PULSE_CENTER && _prevSelectPulse <= RC_PULSE_CENTER) {
            _readyToSelectNext = true;
        }

        if (selectTunePulse >= SELECT_TRIGGER_MIN && selectTunePulse <= SELECT_TRIGGER_MAX && _readyToSelectNext) {
            _readyToSelectNext = false;
            _currentMelodyIndex++;
            if (_currentMelodyIndex >= melodyCount) {
                _currentMelodyIndex = 0;
            }

            EEPROM.write(EEPROM_MELODY_ADDR, _currentMelodyIndex);
            _buzzer.setCustomMelody(melodies[_currentMelodyIndex]);
            _currentMode = BuzzerMode::CUSTOM;
            _buzzer.setMode(_currentMode);
        }

        _prevSelectPulse = selectTunePulse;
        return;
    }

    if (_melodyActive || _melodyInitialized) {
        _melodyActive = false;
        _melodyInitialized = false;
        _currentMode = BuzzerMode::OFF;
        _buzzer.setMode(_currentMode);
    }
}
