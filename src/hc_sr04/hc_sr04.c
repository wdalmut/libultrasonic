#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>

#include "hc_sr04.h"

#ifdef __USE_BCM2835__
#include <bcm2835.h>

#ifndef PIN_TRIGGER
#define PIN_TRIGGER RPI_GPIO_P1_19
#endif

#ifndef PIN_ECHO_BACK
#define PIN_ECHO_BACK RPI_GPIO_P1_21
#endif

#define HC_SR04_READ_TIMEOUT 100000 //us -> 100ms

uint32_t time_spent = 0;

static void hc_sr04_clear_timeout(void)
{
    time_spent = 0;
}

static uint8_t hc_sr04_is_timeout(uint16_t sleep_us)
{
    time_spent += sleep_us;

    uint8_t is_timeout = 0;
    if (time_spent >= HC_SR04_READ_TIMEOUT) {
        is_timeout = 1;
    }

    return is_timeout;
}

/**
 * capture and return time in us
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
        hc_sr04_clear_timeout();
        while(HIGH != bcm2835_gpio_lev(PIN_ECHO_BACK) || hc_sr04_is_timeout(5)) {
            usleep(5);
        }

        hc_sr04_clear_timeout();
        while(LOW != bcm2835_gpio_lev(PIN_ECHO_BACK) || hc_sr04_is_timeout(100)) {
            usleep(100);
            time += 100;
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

#endif

void hc_sr04_init(void)
{
    hc_sr04_prepare();
}

double hc_sr04_ranging(void)
{
    double time_us = hc_sr04_capture();

    double distance_m = 5;
    if (time_us < 38000) {
        distance_m = time_us / 58 / 100;
    }

    return distance_m;
}

