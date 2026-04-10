#include "SportsCar.h"

int main() {
    Car a(50, "NormalCar", 3);
    SportsCar b;

    b.changeGear(2);
    b.setTurbo(true);

    a.display();

    b.speedUp();
    b.display();

    return 0;
}
