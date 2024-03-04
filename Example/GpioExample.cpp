#include <iostream>
#include <wiringPi.h>

using namespace std;

int main(void)
{
    //set up GPIO
    wiringPiSetupGpio();

    //pin 17 as input
    pinMode(17, input);

    //pull up/down mode
    pullUpDnControl(17, PUD_DOWN);

    if (HIGH == value)
    {
        printf("bruh");
    }
}