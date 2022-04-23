#include <dimmed-led-lib.h>

int main(int argc, char const *argv[])
{
    dimLed(0, 1, 5000);

    fadeInLed(2, 5000);

    fadeOutLed(1, 5000);

    return 0;
}
