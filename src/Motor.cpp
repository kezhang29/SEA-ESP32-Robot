#include "Motor.h"

Motor::Motor(int in1, int in2, int pwm, int channel, int stby):
    m_in1(in1),
    m_in2(in2),
    m_pwm(pwm),
    m_channel(channel),
    m_stby(stby){}

void Motor::configure() {
    pinMode(m_in1, OUTPUT);
    pinMode(m_in2, OUTPUT);
    pinMode(m_stby, OUTPUT);

    digitalWrite(m_stby, HIGH);

    #if ESP_ARDUINO_VERSION_MAJOR >= 3
        ledcAttach(m_pwm, 1000, 8);
    #else
        ledcSetup(m_channel, 1000, 8);
        ledcAttachPin(m_pwm, m_channel);
    #endif
}

void Motor::setSpeed(int speed) {
    speed = constrain(speed, -255, 255);

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

    #if ESP_ARDUINO_VERSION_MAJOR >= 3
        ledcWrite(m_pwm, speed);
    #else
        ledcWrite(m_channel, speed);
    #endif
}