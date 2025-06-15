#ifndef RC_CHANNEL_H
#define RC_CHANNEL_H

#include <Arduino.h>

class RCChannel
{
public:
    RCChannel(uint8_t pin);
    void begin();
    uint16_t getPulseWidth() const;

private:
    static void handleInterrupt0();
    static void handleInterrupt1();
    static void handleInterrupt2();

    void handleInterrupt();

    volatile uint32_t pulseStart = 0;
    volatile uint16_t pulseWidth = 1500; // giá trị mặc định trung lập
    uint8_t pin;

    static RCChannel *instances[3]; // tối đa 3 kênh với INT0, INT1, INT2
};

#endif
