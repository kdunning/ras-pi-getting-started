#include <wiringPi.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* GPIO mappings for LED pins */
typedef enum LedMapping
{
    RED_GPIO = 0,
    GRN_GPIO = 1,
    BLU_GPIO = 2,
    LED_COUNT
} LedMapping_t;

/* Main entry point */
int main(int argc, char **argv)
{
    /* WiringPi set up */
    wiringPiSetup();
    pinMode(RED_GPIO, OUTPUT);
    pinMode(GRN_GPIO, OUTPUT);
    pinMode(BLU_GPIO, OUTPUT);

    /* Check input arguments */
    int gpio = -1;
    if (argc >= 2)
    {
        if (strcmp(argv[1], "red") == 0)
        {
            printf("Turning the red LED on.\n");
            gpio = RED_GPIO;
        }
        else if (strcmp(argv[1], "green") == 0)
        {
            printf("Turning the green LED on.\n");
            gpio = GRN_GPIO;
        }
        else if (strcmp(argv[1], "blue") == 0)
        {
            printf("Turning the blue LED on.\n");
            gpio = BLU_GPIO;
        }
    }
    else
    {
        printf("Usage: ./%s <colour> {red|green|blue}\n", __FILE__);
    }

    /* Loop through available LEDs and turn on relevant GPIOs */
    static const int AVAILABLE_GPIOS[LED_COUNT] =
    {
        RED_GPIO,
        GRN_GPIO,
        BLU_GPIO
    };
    for (int i = 0; i < LED_COUNT; ++i)
    {
        digitalWrite(AVAILABLE_GPIOS[i], AVAILABLE_GPIOS[i] == gpio);
    }
    return EXIT_SUCCESS;
}