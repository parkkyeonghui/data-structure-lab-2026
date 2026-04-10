#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

class Car {
protected:
    int speed;
    char name[40];

public:
    int gear;

    Car() {}
    ~Car() {}

    Car(int s, const char* n, int g)
        : speed(s), gear(g) {
        strcpy(name, n);
    }

    void changeGear(int g = 4) {
        gear = g;
    }

    void speedUp() {
        speed += 5;
    }

    void display() {
        //printf("[%s] : 기어=%d단 속도=%dkmph\n", name, gear, speed);
        printf("[%s] : Gear=%d Speed%dkmph\n", name, gear, speed);
    }

    void whereAmI() {
        //printf("객체 주소 = %p\n", this);
        printf("Object address = %p\n", this);
    }
};
