#include <stdio.h>
#include <stdlib.h>

#include "ultrasonic.h"

int main(void)
{
    ultrasonic_init();
    printf("%lf\n", ultrasonic_ranging());

    return EXIT_SUCCESS;
}
