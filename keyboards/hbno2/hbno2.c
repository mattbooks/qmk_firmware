#include QMK_KEYBOARD_H
#include "i2cmaster.h"


extern inline void ergodox_board_led_on(void);

extern inline void ergodox_board_led_off(void);

extern inline void ergodox_led_all_on(void);
extern inline void ergodox_led_all_off(void);

bool i2c_initialized = 0;
uint8_t mcp23018_status = 0x20;

/* void matrix_init_kb(void) { */
/*    /1* // keyboard LEDs (see "PWM on ports OC1(A|B|C)" in "teensy-2-0.md") *1/ */
/*    /1*  TCCR1A = 0b10101001;  // set and configure fast PWM *1/ */
/*    /1*  TCCR1B = 0b00001001;  // set and configure fast PWM *1/ */
/*    /1*  TCCR4D = 0b00001001;  // set and configure fast PWM *1/ */

/*    /1*  ergodox_board_led_on(); *1/ */
/*    /1*  _delay_ms(500); *1/ */
/*    /1*  ergodox_board_led_off(); *1/ */

/*    /1*  matrix_init_user(); *1/ */

/*   init_mcp23018(); */
/* } */

uint8_t init_mcp23018(void) {
    mcp23018_status = 0x20;

    // I2C subsystem

    // uint8_t sreg_prev;
    // sreg_prev=SREG;
    // cli();
    i2c_init();  // on pins D(1,0)
    i2c_initialized = true;
    _delay_ms(1000);

/*     // set pin direction */
/*     // - unused  : input  : 1 */
/*     // - input   : input  : 1 */
/*     // - driving : output : 0 */
    mcp23018_status = i2c_start(I2C_ADDR_WRITE);    if (mcp23018_status) goto out;
    mcp23018_status = i2c_write(IODIRA);            if (mcp23018_status) goto out;
    mcp23018_status = i2c_write(0b10000000);        if (mcp23018_status) goto out;
    mcp23018_status = i2c_write(0b11000000);        if (mcp23018_status) goto out;
    i2c_stop();

/*     // set pull-up */
/*     // - unused  : on  : 1 */
/*     // - input   : on  : 1 */
/*     // - driving : off : 0 */
    mcp23018_status = i2c_start(I2C_ADDR_WRITE);    if (mcp23018_status) goto out;
    mcp23018_status = i2c_write(GPPUA);             if (mcp23018_status) goto out;
    mcp23018_status = i2c_write(0b01111111);        if (mcp23018_status) goto out;
    mcp23018_status = i2c_write(0b00111111);        if (mcp23018_status) goto out;

out:
    i2c_stop();

    // SREG=sreg_prev;

    return mcp23018_status;
}
