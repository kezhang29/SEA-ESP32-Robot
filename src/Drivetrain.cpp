#include "Drivetrain.h"

Drivetrain::Drivetrain(Motor& leftFront, Motor& leftBack, Motor& rightFront, Motor& rightBack, int stby1, int stby2):
    m_leftFront(leftFront),
    m_leftBack(leftBack),
    m_rightFront(rightFront),
    m_rightBack(rightBack),
    m_stby1(stby1),
    m_stby2(stby2) {}

void Drivetrain::begin() {

    m_leftFront.begin();
    m_leftBack.begin();

    m_rightFront.begin();
    m_rightBack.begin();

    pinMode(m_stby1, OUTPUT);
    pinMode(m_stby2, OUTPUT);

    digitalWrite(m_stby1, HIGH);
    digitalWrite(m_stby2, HIGH);
}

void Drivetrain::arcadeDrive(int leftSpeed, int rightSpeed) {
    m_leftFront.setSpeed(leftSpeed);
    m_leftBack.setSpeed(leftSpeed);

    m_rightFront.setSpeed(rightSpeed);
    m_rightBack.setSpeed(rightSpeed);
}

void Drivetrain::forward(int speed) {
    arcadeDrive(speed, speed);
}

void Drivetrain::backward(int speed) {
    arcadeDrive(-speed, -speed);
}

void Drivetrain::left(int speed) {
    arcadeDrive(-speed, speed);
}

void Drivetrain::right(int speed){
    arcadeDrive(speed, -speed);
}

void Drivetrain::stop() {
    arcadeDrive(0, 0);
}