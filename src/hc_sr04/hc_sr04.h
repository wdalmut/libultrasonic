#ifndef __HC_SR04_H__
#define __HC_SR04_H__

// Use Raspberry Pi (BCM2835)
#define __USE_BCM2835__ 1

void hc_sr04_init(void);
double hc_sr04_ranging(void);

//*********************************//
//       PRIVATE FUNCTIONS         //
//*********************************//
void hc_sr04_prepare(void);
double hc_sr04_capture(void);

#endif
