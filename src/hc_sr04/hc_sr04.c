#include <stdio.h>
#include <unistd.h>
#include <inttypes.h>

#include "hc_sr04.h"

#ifdef __USE_BCM2835__
#include <bcm2835.h>

#define PIN_TRIGGER RPI_GPIO_P1_19
#define PIN_ECHO_BACK RPI_GPIO_P1_21

#define HC_SR04_READ_TIMEOUT 100000 //us -> 100ms

static uint8_t hc_sr04_isTimeout(uint16_t);

/**
 * capture and return time in ms
 *
 * so rude... need kernel module...
 */
double hc_sr04_capture(void)
{
    double time = 0;

    // void start_capture
    {
        bcm2835_gpio_write(PIN_TRIGGER, HIGH);
        usleep(10);
        bcm2835_gpio_write(PIN_TRIGGER, LOW);
    }

    // double read_distance
    {
        int status = HIGH;
        while(HIGH != bcm2835_gpio_lev(PIN_ECHO_BACK) || hc_sr04_isTimeout(5)) {
            usleep(5);
        }

        while(LOW != bcm2835_gpio_lev(PIN_ECHO_BACK) || hc_sr04_isTimeout(100)) {
            usleep(100);
            time += 0.1;
        }

    }

    return time;
}

void hc_sr04_prepare(void)
{
    if (!bcm2835_init()) {
        exit(1);
    }

    bcm2835_gpio_fsel(PIN_ECHO_BACK, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_fsel(PIN_TRIGGER, BCM2835_GPIO_FSEL_OUTP);

    bcm2835_gpio_write(PIN_TRIGGER, LOW);
}

static uint8_t hc_sr04_isTimeout(uint16_t sleep_us)
{
    static spent = 0;

    spent += sleep_us;

    if (spent >= HC_SR04_READ_TIMEOUT) {
        return 1;
    }

    return 0;
}

#endif

void hc_sr04_init(void)
{
    hc_sr04_prepare();
}

double hc_sr04_ranging(void)
{
    double time = hc_sr04_capture();

    double distance_m = 5;
    if (time < 38) {
        distance_m = time / 58 * 100;
    }

    return distance_m;
}

