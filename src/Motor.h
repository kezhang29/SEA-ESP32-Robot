#pragma once

#include <Arduino.h>

class Motor {
public:
    Motor(
        int in1,
        int in2,
        int pwm,
        int channel
    );

    void begin();

    void setSpeed(int speed);

private:
    int m_in1;
    int m_in2;
    int m_pwm;
    int m_channel;
};