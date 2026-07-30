#pragma once

#include "Motor.h"


class Drivetrain {

public:
    Drivetrain(
        Motor& leftFront,
        Motor& leftBack,
        Motor& rightFront,
        Motor& rightBack,
        int stby1,
        int stby2
    );

    void begin();

    void arcadeDrive(
        int leftSpeed,
        int rightSpeed
    );

    void forward(int speed);

    void backward(int speed);

    void left(int speed);

    void right(int speed);

    void stop();

private:
    Motor& m_leftFront;
    Motor& m_leftBack;

    Motor& m_rightFront;
    Motor& m_rightBack;

    int m_stby1;
    int m_stby2;
};