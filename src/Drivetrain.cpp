#include "Drivetrain.h"

Drivetrain::Drivetrain(Motor& leftFront, Motor& leftBack, Motor& rightFront, Motor& rightBack):
    m_leftFront(leftFront),
    m_leftBack(leftBack),
    m_rightFront(rightFront),
    m_rightBack(rightBack) {}

void Drivetrain::configure() {
    m_leftFront.configure();
    m_leftBack.configure();

    m_rightFront.configure();
    m_rightBack.configure();
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