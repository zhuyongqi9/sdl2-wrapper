#include "utils.h"
#include <iostream>

int main() {
    Timer timer;
    timer.start();
    int sum;
    for (int i = 0 ; i<10000000; i++) sum += i;
    std::cout << timer.get_ticks() << std::endl;
}