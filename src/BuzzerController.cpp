#include "BuzzerController.h"

BuzzerController::BuzzerController(uint8_t pin)
    : _pin(pin), _mode(BuzzerMode::OFF), _lastMode(BuzzerMode::OFF),
      _lastToneTime(0), _toneState(false), _melodyIndex(0), _customMelody(nullptr) {}

void BuzzerController::begin()
{
    pinMode(_pin, OUTPUT);
    noTone(_pin);
}

void BuzzerController::setMode(BuzzerMode mode)
{
    if (_mode != mode)
    {
        _mode = mode;
        _melodyIndex = 0;
        _lastToneTime = millis();
    }
}

void BuzzerController::update()
{
    if (_mode != _lastMode)
    {
        _lastMode = _mode;
        _melodyIndex = 0;
        _lastToneTime = millis();
        noTone(_pin);
    }

    switch (_mode)
    {
    case BuzzerMode::READY:
        playReadyTone();
        break;
    case BuzzerMode::ERROR:
        playErrorTone();
        break;
    case BuzzerMode::REVERSING:
        playReversingTone();
        break;
    case BuzzerMode::CUSTOM:
        playCustomMelody();
        break;
    default:
        noTone(_pin);
        break;
    }
}

void BuzzerController::playReadyTone()
{
    if (millis() - _lastToneTime >= 1000)
    {
        tone(_pin, 1000, 100);
        _lastToneTime = millis();
    }
}

void BuzzerController::playErrorTone()
{
    if (millis() - _lastToneTime >= 300)
    {
        _toneState = !_toneState;
        _toneState ? tone(_pin, 400) : noTone(_pin);
        _lastToneTime = millis();
    }
}

void BuzzerController::playReversingTone()
{
    if (millis() - _lastToneTime >= 500)
    {
        tone(_pin, 600, 100);
        _lastToneTime = millis();
    }
}

void BuzzerController::setCustomMelody(const Melody *melody)
{
    _customMelody = melody;
    _melodyIndex = 0;
    _lastToneTime = millis();
}

void BuzzerController::playCustomMelody()
{
    if (!_customMelody)
        return;

    const Note *notes = _customMelody->getNotes();
    size_t length = _customMelody->getLength();

    if (_melodyIndex < length)
    {
        if (millis() - _lastToneTime >= notes[_melodyIndex].duration)
        {
            _lastToneTime = millis();
            if (notes[_melodyIndex].frequency > 0)
            {
                tone(_pin, notes[_melodyIndex].frequency);
                Serial.print("Play Note: ");
                Serial.print(notes[_melodyIndex].frequency);
                Serial.print(" Hz, duration: ");
                Serial.print(notes[_melodyIndex].duration);
                Serial.println(" ms");
            }
            else
            {
                noTone(_pin);
                Serial.println("Pause");
            }
            _melodyIndex++;
        }
    }
    else
    {
        noTone(_pin);
        if (_customMelody->shouldLoop())
        {
            Serial.println("Melody loop restart.");
            _melodyIndex = 0;
        }
    }
}
