#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include "Life.h"

int main() {
    Life life;

    while (life.running())
    {
        life.update();


        life.render();
    }

    // testPath();

    return 0;
}