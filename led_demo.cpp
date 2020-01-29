//------------------------------------------------------------------------------
/// @file   led_demos.cpp
/// @brief  Provides a demo for toggling LEDs using wiringPi to control GPIO
///         lines on the Raspberry Pi.
///
/// @details    This application makes the assumption that an RGB LED is
///             connected as such: Red to pin 11 (GPIO 0), Green to pin 12
///             (GPIO 1) and Blue to pin 13 (GPIO 2). This application is
///             designed to be used with the Raspberry Pi only. Attempts to
///             run on any other platform will fail to work.
//------------------------------------------------------------------------------
/// @author Kris Dunning (kris.dunning@itdev.co.uk)
//  http://www.itdev.co.uk
//------------------------------------------------------------------------------
#include <cstdio>
#include <cctype>
#include <cstdlib>
#include <wiringPi.h>
#include <softPwm.h>
#include <unistd.h>
#include <cmath>

// LED Pin mappings
/// @brief  Red LED pin mapping.
#define RED_LED     (0)
/// @brief  Green LED pin mapping.
#define GRN_LED     (1)
/// @brief  Blue LED pin mapping.
#define BLU_LED     (2)

/// @brief  The number of microseconds in a second.
#define US_PER_S    (1000000U)

/// @brief  Provides the available colour list. By mapping Red Green and Blue
///         to different bits, we can use masks to encode a mixture of colours.
enum LedColours
{
    Off = 0,
    Red = 1,
    Green = 2,
    Blue = 4,
    Cyan = Blue | Green,
    Magenta = Blue | Red,
    Yellow = Red | Green,
    White = Red | Green | Blue,
};

/// @brief  Provides a sequence to flash/fade.
static const LedColours LED_COL_SEQUENCE[] =
{
    Red,
    Blue,
    Green,
    Blue,
    Cyan,
    Magenta,
    Yellow,
    White
};

/// @brief  The size of the sequence list.
static const size_t SEQUENCE_COUNT = sizeof(LED_COL_SEQUENCE) / sizeof(LED_COL_SEQUENCE[0]);

/// @brief  Macro to update the LED and print the action to screen.
#define UPDATE_LED(pin, col, val)    do {                           \
    printf("Turning %s %s\n", #col, (val & col) ? "on" : "off");    \
    digitalWrite(pin, (val & col));                                 \
} while(0);

//******************************************************************************
/// @brief  Sets the relevant LED pins.
void set_colour
(
    const LedColours col ///<IN The colour to set
)
{
    UPDATE_LED(RED_LED, Red, col);
    UPDATE_LED(GRN_LED, Green, col);
    UPDATE_LED(BLU_LED, Blue, col);
}

//******************************************************************************
/// @brief  Runs a sequence of colours with a 500ms time delay between each
///         change.
void free_run()
{
    printf("Free running! Use CTRL+C to exit.\n");
    while (true)
    {
        for (size_t i = 0; i < SEQUENCE_COUNT; ++i)
        {
            set_colour(LED_COL_SEQUENCE[i]);
            usleep(500000);
        }
    }
}

//******************************************************************************
/// @brief  Uses hardware PWM to control the Green LED (pin 12).
/// @note   There is only  one PWM pin on the Raspberry Pi.
/// @details    This function calculates the PWM such that the brightness
///             appears to increase and decrease as linearly as possible.
///             The hardware PWM pin accepts values from 0 (off) to 1024 (full).
void hardware_pwm()
{
    printf("Green LED pulse! Use CTRL+C to exit.\n");
    static const int LED_STEPS = 256;
    static const int FLASH_FREQ = 2;
    static const int SLEEP_TIME_US = US_PER_S / (FLASH_FREQ * LED_STEPS);
    // Turn off all LEDs
    set_colour(Off);
    // Change the pin mode from output to PWM
    pinMode(GRN_LED, PWM_OUTPUT);
    const float r = (LED_STEPS * log10(2)) / (log10(1024));
    while (true)
    {
        // Ramp up the brightness
        for (int i = 0; i < LED_STEPS; ++i)
        {
            const int pwm = pow(2, (i / r)) - 1;
            pwmWrite(GRN_LED, pwm);
            usleep(SLEEP_TIME_US);
        }
        // Ramp down the brightness
        for (int i = LED_STEPS; i > 0; --i)
        {
            const int pwm = pow(2, (i / r)) - 1;
            pwmWrite(GRN_LED, pwm);
            usleep(SLEEP_TIME_US);
        }
    }
}

//******************************************************************************
/// @brief  Uses software PWM to control all three LEDs.
/// @note   Any GPIO can provide a PWM signal controlled by a background thread.
/// @details    This function does not attempt to control the brightness of the
///             LED and just fades at a regular interval.
void software_pwm()
{
    printf("Free running with software PWM. Use CTRL+C to exit.\n");
    static const int LED_STEPS = 100;
    static const int FLASH_FREQ = 1;
    static const int SLEEP_TIME_US = US_PER_S / (FLASH_FREQ * LED_STEPS);

    // Variables to store the last and next colour indices
    int last = 0;
    int next = 1;

    // Calculate the current PWM values for Red, Green and Blue.
    int r = (LED_STEPS * (LED_COL_SEQUENCE[last] & Red)) / Red;
    int g = (LED_STEPS * (LED_COL_SEQUENCE[last] & Green)) / Green;
    int b = (LED_STEPS * (LED_COL_SEQUENCE[last] & Blue)) / Blue;

    // Initialise Software PWM values for the first colour
    softPwmCreate(RED_LED, r, LED_STEPS);
    softPwmCreate(GRN_LED, g, LED_STEPS);
    softPwmCreate(BLU_LED, b, LED_STEPS);

    while (true)
    {
        // Calculate the offsets for each step.
        // This will be either 1 (gets brighter), 0 (no change) or -1 (gets dimmer)
        const int r_o = ((LED_COL_SEQUENCE[next] & Red) - (LED_COL_SEQUENCE[last] & Red)) / Red;
        const int g_o = ((LED_COL_SEQUENCE[next] & Green) - (LED_COL_SEQUENCE[last] & Green)) / Green;
        const int b_o = ((LED_COL_SEQUENCE[next] & Blue) - (LED_COL_SEQUENCE[last] & Blue)) / Blue;
        // Run through and change colour to the next
        for (int i = 0; i < LED_STEPS; ++i)
        {
            softPwmWrite(RED_LED, (r += r_o));
            softPwmWrite(GRN_LED, (g += g_o));
            softPwmWrite(BLU_LED, (b += b_o));
            usleep(SLEEP_TIME_US);
        }
        last = next;
        next = (last + 1) % SEQUENCE_COUNT;
    }
}

//******************************************************************************
/// @brief  Application entry point.
/// @return Always returns EXIT_SUCCESS.
int main(int argc, char **argv)
{
    printf("Enter a letter from the following:\n");
    printf("* R (Red)\n");
    printf("* G (Green)\n");
    printf("* B (Blue)\n");
    printf("* C (Cyan)\n");
    printf("* M (Magenta)\n");
    printf("* Y (Yellow)\n");
    printf("* W (White)\n");
    printf("* F  Free run - changes colour forever.\n");
    printf("* P  Pulse - pulses the green LED forever (hardware PWM).\n");
    printf("* S  Software PWM pulse - changes colour forever with software PWM fades.\n");
    printf("* X  Exit\n");

    wiringPiSetup();
    pinMode(RED_LED, OUTPUT);
    pinMode(GRN_LED, OUTPUT);
    pinMode(BLU_LED, OUTPUT);
    bool running = true;
    while (running)
    {
        const int opt = toupper(getchar());
        switch (static_cast<char>(opt))
        {
            case 'R':
                set_colour(Red);
                break;
            case 'G':
                set_colour(Green);
                break;
            case 'B':
                set_colour(Blue);
                break;
            case 'C':
                set_colour(Cyan);
                break;
            case 'M':
                set_colour(Magenta);
                break;
            case 'Y':
                set_colour(Yellow);
                break;
            case 'W':
                set_colour(White);
                break;
            case 'X':
                running = false;
                break;
            case 'F':
                free_run();
                break;
            case 'P':
                hardware_pwm();
                break;
            case 'S':
                software_pwm();
                break;
            case '\n':
                break;
            default:
                set_colour(Off);
                break;
        }
    }
    printf("Exiting.\n");
    return EXIT_SUCCESS;
}
