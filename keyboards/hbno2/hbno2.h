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

void init_ergodox(void);
void ergodox_blink_all_leds(void);
uint8_t init_mcp23018(void);

#ifndef LED_BRIGHTNESS_LO
#define LED_BRIGHTNESS_LO       15
#endif
#ifndef LED_BRIGHTNESS_LO
#define LED_BRIGHTNESS_HI       255
#endif


inline void ergodox_board_led_on(void) {
  DDRB |=  (1<<5);
  PORTB |=  (1<<5);
}

inline void ergodox_board_led_off(void) {
  DDRB &= ~(1<<5);
  PORTB &= ~(1<<6);
}

inline void ergodox_led_all_on(void)
{
    ergodox_board_led_on();
}

inline void ergodox_led_all_off(void)
{
    ergodox_board_led_off();
}

#define KEYMAP( \
    k00,k05,k10,k15,k20,k25,   k30,k35,k40,k45,k50,k55, \
    k01,k06,k11,k16,k21,k26,   k31,k36,k41,k46,k51,k56, \
    k02,k07,k12,k17,k22,k27,   k32,k37,k42,k47,k52,k57, \
    k03,k08,k13,k18,k23,k28,   k33,k38,k43,k48,k53,k58, \
    k04,k09,k14,k19,k24,k29,   k34,k39,k44,k49,k54,k59, \
                    k60, k61, k62, k63 ) { \
    { k00, k05, k10, k15, k20, k25, k30, k35, k40, k45, k50, k55, k60 }, \
    { k01, k06, k11, k16, k21, k26, k31, k36, k41, k46, k51, k56, k61 }, \
    { k02, k07, k12, k17, k22, k27, k32, k37, k42, k47, k52, k57, k62 }, \
    { k03, k08, k13, k18, k23, k28, k33, k38, k43, k48, k53, k58, k63 }, \
    { k04, k09, k14, k19, k24, k29, k34, k39, k44, k49, k54, k59, KC_NO }, \
  }

#define LAYOUT_ergodox KEYMAP

#endif
