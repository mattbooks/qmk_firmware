#include QMK_KEYBOARD_H
#include "i2cmaster.h"

bool i2c_initialized = 0;
uint8_t mcp23018_status = 0x20;

void matrix_init_kb(void) {
  // Set timer pins to outputs
  DDRB |= (1<<5) | (1<<6);
  DDRC |= (1<<6);

  // Configure PWM for timers 1A, 1B and 3A
  TCCR1A = (1 << WGM11) | (1 << WGM10) | // Fast PWM, 10 bit, phase correct
    (1 << COM1A1) | (1 << COM1A0) | // set on up counting match, clear on down
    (1 << COM1B1) | (1 << COM1B0); // set on up counting match, clear on down
  TCCR3A = (1 << WGM31) | (1 << WGM30) | // Fast PWM, 10 bit, phase correct
    (1 << COM3A1) | (1 << COM3A0); // set on up counting match, clear on down

  // Set clock source without scaler
  TCCR1B = (1<<CS10);
  TCCR3B = (1<<CS30);
}

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
