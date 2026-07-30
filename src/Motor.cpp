#include "Motor.h"

Motor::Motor(int in1, int in2, int pwm, int channel):
    m_in1(in1),
    m_in2(in2),
    m_pwm(pwm),
    m_channel(channel) {}

void Motor::begin() {
    pinMode(m_in1, OUTPUT);
    pinMode(m_in2, OUTPUT);

    ledcSetup(m_channel, 1000, 8);

    ledcAttachPin(m_pwm, m_channel);
}

void Motor::setSpeed(int speed) {
    speed = constrain(speed, 0, 255);

    if (speed > 0) {
        digitalWrite(m_in1, HIGH);
        digitalWrite(m_in2, LOW);

        ledcWrite(m_channel, speed);
    } else if (speed < 0) {
        digitalWrite(m_in1, LOW);
        digitalWrite(m_in2, HIGH);

        ledcWrite(m_channel, speed);
    } else { 
        ledcWrite(m_channel, 0); 
    }
}