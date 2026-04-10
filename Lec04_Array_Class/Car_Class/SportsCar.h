#pragma once
#include "Car.h"

class SportsCar : public Car {
public:
    bool bTurbo;

    SportsCar() : Car(0, "SportsCar", 1) {
        bTurbo = false;
    }

    void setTurbo(bool bTur) { bTurbo = bTur; }

    void speedUp() {
        if (bTurbo) speed += 20;
        else Car::speedUp();
    }
};