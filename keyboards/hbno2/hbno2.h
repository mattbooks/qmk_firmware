#ifndef ERGODOX_EZ_H
#define ERGODOX_EZ_H

#include "quantum.h"
#include <stdint.h>
#include <stdbool.h>
#include "i2cmaster.h"
#include <util/delay.h>

#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))
#define CPU_16MHz       0x00

// I2C aliases and register addresses (see "mcp23018.md")
#define I2C_ADDR_WRITE  0b01000000
#define I2C_ADDR_READ   0b01000001
#define IODIRA          0x00            // i/o direction register
#define IODIRB          0x01
#define GPPUA           0x0C            // GPIO pull-up resistor register
#define GPPUB           0x0D
#define GPIOA           0x12            // general purpose i/o port register (write modifies OLAT)
#define GPIOB           0x13
#define OLATA           0x14            // output latch register
#define OLATB           0x15

extern uint8_t mcp23018_status;

uint8_t init_mcp23018(void);

#ifndef LED_BRIGHTNESS_LO
#define LED_BRIGHTNESS_LO       15
#endif
#ifndef LED_BRIGHTNESS_LO
#define LED_BRIGHTNESS_HI       255
#endif

#define MAX_BRIGHTNESS 0x3FF
#define MIN_BRIGHTNESS 0x0

inline void led_on(void) {
  OCR1A = MAX_BRIGHTNESS;
  OCR3A = MAX_BRIGHTNESS;
  OCR1B = MAX_BRIGHTNESS;
}

inline void led_off(void) {
  OCR1A = MIN_BRIGHTNESS;
  OCR3A = MIN_BRIGHTNESS;
  OCR1B = MAX_BRIGHTNESS;
}

inline void led_rgb(uint16_t r, uint16_t g, uint16_t b) {
  OCR1A = r;
  OCR3A = g;
  OCR1B = b;
}

#define KEYMAP( \
    k00,k05,k10,k15,k20,k25, \
    k01,k06,k11,k16,k21,k26, \
    k02,k07,k12,k17,k22,k27, \
    k03,k08,k13,k18,k23,k28, \
    k04,k09,k14,k19,k24,k29, \
\
    k30,k35,k40,k45,k50,k55, \
    k31,k36,k41,k46,k51,k56, \
    k32,k37,k42,k47,k52,k57, \
    k33,k38,k43,k48,k53,k58, \
    k34,k39,k44,k49,k54,k59, \
\
    k60, k61, k63, k62 ) { \
    { k00, k01, k02, k03, k04 }, \
    { k05, k06, k07, k08, k09 }, \
    { k10, k11, k12, k13, k14 }, \
    { k15, k16, k17, k18, k19 }, \
    { k20, k21, k22, k23, k24 }, \
    { k25, k26, k27, k28, k29 }, \
    { k30, k31, k32, k33, k34 }, \
    { k35, k36, k37, k38, k39 }, \
    { k40, k41, k42, k43, k44 }, \
    { k45, k46, k47, k48, k49 }, \
    { k50, k51, k52, k53, k54 }, \
    { k55, k56, k57, k58, k59 }, \
    { k60, k61, k62, k63, KC_NO } \
  }

#endif
