/*

Note for ErgoDox EZ customizers: Here be dragons!
This is not a file you want to be messing with.
All of the interesting stuff for you is under keymaps/ :)
Love, Erez

Copyright 2013 Oleg Kostyuk <cub.uanic@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 * scan matrix
 */
#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include "wait.h"
#include "action_layer.h"
#include "print.h"
#include "debug.h"
#include "util.h"
#include "matrix.h"
#include QMK_KEYBOARD_H
#include "i2cmaster.h"
#ifdef DEBUG_MATRIX_SCAN_RATE
#include  "timer.h"
#endif

#define DEBUG_MATRIX_SCAN_RATE 1

/*
 * This constant define not debouncing time in msecs, but amount of matrix
 * scan loops which should be made to get stable debounced results.
 *
 * On Ergodox matrix scan rate is relatively low, because of slow I2C.
 * Now it's only 317 scans/second, or about 3.15 msec/scan.
 * According to Cherry specs, debouncing time is 5 msec.
 *
 * However, some switches seem to have higher debouncing requirements, or
 * something else might be wrong. (Also, the scan speed has improved since
 * that comment was written.)
 */

#ifndef DEBOUNCE
#   define DEBOUNCE	5
#endif

/* matrix state(1:on, 0:off) */
static matrix_row_t matrix[MATRIX_ROWS];

// Debouncing: store for each key the number of scans until it's eligible to
// change.  When scanning the matrix, ignore any changes in keys that have
// already changed in the last DEBOUNCE scans.
static uint8_t debounce_matrix[MATRIX_ROWS * MATRIX_COLS];

static uint8_t read_rows(uint8_t col);
static void unselect_columns(void);
static void select_column(uint8_t col);

static uint8_t mcp23018_reset_loop;

#ifdef DEBUG_MATRIX_SCAN_RATE
uint32_t matrix_timer;
uint32_t matrix_scan_count;
#endif


__attribute__ ((weak))
void matrix_init_user(void) {}

__attribute__ ((weak))
void matrix_scan_user(void) {}

__attribute__ ((weak))
void matrix_init_kb(void) {
  matrix_init_user();
}

__attribute__ ((weak))
void matrix_scan_kb(void) {
  matrix_scan_user();
}

inline
uint8_t matrix_rows(void)
{
    return MATRIX_COLS;
}

inline
uint8_t matrix_cols(void)
{
    return MATRIX_ROWS;
}

void matrix_init(void)
{
    // initialize row and col
    mcp23018_status = init_mcp23018();

    unselect_columns();

    // initialize matrix state: all keys off
    for (uint8_t i=0; i < MATRIX_ROWS; i++) {
        matrix[i] = 0;
        for (uint8_t j=0; j < MATRIX_COLS; ++j) {
            debounce_matrix[i * MATRIX_COLS + j] = 0;
        }
    }

/* #ifdef DEBUG_MATRIX_SCAN_RATE */
/*     matrix_timer = timer_read32(); */
/*     matrix_scan_count = 0; */
/* #endif */

    matrix_init_quantum();

}

void matrix_power_up(void) {
    mcp23018_status = init_mcp23018();

    unselect_columns();

    // initialize matrix state: all keys off
    for (uint8_t i=0; i < MATRIX_COLS; i++) {
        matrix[i] = 0;
    }

/* #ifdef DEBUG_MATRIX_SCAN_RATE */
    /* matrix_timer = timer_read32(); */
    /* matrix_scan_count = 0; */
/* #endif */
}

// Returns a matrix_row_t whose bits are set if the corresponding key should be
// eligible to change in this scan.
matrix_row_t debounce_mask(uint8_t col) {
  matrix_row_t result = 0;
  for (uint8_t j=0; j < MATRIX_COLS; ++j) {
    if (debounce_matrix[col * MATRIX_COLS + j]) {
      --debounce_matrix[col * MATRIX_COLS + j];
    } else {
      result |= (1 << j);
    }
  }
  return result;
}

// Report changed keys in the given row.  Resets the debounce countdowns
// corresponding to each set bit in 'change' to DEBOUNCE.
void debounce_report(matrix_row_t change, uint8_t col) {
  for (uint8_t i = 0; i < MATRIX_COLS; ++i) {
    if (change & (1 << i)) {
      debounce_matrix[col * MATRIX_COLS + i] = DEBOUNCE;
    }
  }
}

uint8_t matrix_scan(void)
{
    /* matrix_print(); */

    /* uint8_t data = 0; */

    /* mcp23018_status = i2c_start(I2C_ADDR_READ); */
    /* if (mcp23018_status) { */
    /*   goto out; */
    /* } */

    /* data = i2c_readNak(); */

/* out: */
    /* i2c_stop(); */

    if (mcp23018_status) { // if there was an error
        if (++mcp23018_reset_loop == 0) {
            // since mcp23018_reset_loop is 8 bit - we'll try to reset once in 255 matrix scans
            // this will be approx bit more frequent than once per second
            print("trying to reset mcp23018\n");
            mcp23018_status = init_mcp23018();
            if (mcp23018_status) {
                print("left side not responding\n");
            } else {
                print("left side attached\n");
            }
        }
    }

/* #ifdef DEBUG_MATRIX_SCAN_RATE */
/*     matrix_scan_count++; */

/*     uint32_t timer_now = timer_read32(); */
/*     if (TIMER_DIFF_32(timer_now, matrix_timer)>1000) { */
/*         print("matrix scan frequency: "); */
/*         pdec(matrix_scan_count); */
/*         print("\n"); */

/*         matrix_timer = timer_now; */
/*         matrix_scan_count = 0; */
/*     } */
/* #endif */

    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {

        select_column(i);
        matrix_row_t mask = debounce_mask(i);
        matrix_row_t rows = (read_rows(i) & mask) | (matrix[i] & ~mask);
        debounce_report(rows ^ matrix[i], i);
        matrix[i] = rows;

        unselect_columns();
    }

    matrix_scan_quantum();

    return 1;
}

inline
bool matrix_is_on(uint8_t row, uint8_t col)
{
    return (matrix[col] & ((matrix_row_t)1<<row));
}

inline
matrix_row_t matrix_get_row(uint8_t col)
{
    return matrix[col];
}

void matrix_print(void)
{
    print("\nc/r 01234\n");
    for (uint8_t col = 0; col < MATRIX_ROWS; col++) {
        phex(col); print(": ");
        xprintf("%05b\n", matrix[col]);
    }
    _delay_ms(1000);
}

uint8_t matrix_key_count(void)
{
    uint8_t count = 0;
    for (uint8_t i = 0; i < MATRIX_COLS; i++) {
        count += bitpop16(matrix[i]);
    }
    return count;
}

static uint8_t read_rows(uint8_t column)
{
  DDRB &= ~((1<<7) | (1<<3) | (1<<2) | (1<<1) | (1<<0));
  PORTB |= (1<<7) | (1<<3) | (1<<2) | (1<<1) | (1<<0);
  uint8_t data = PINB;
  /* xprintf("PINB:             %08b\n", data); */
  /* xprintf("data & 0xF:       %08b\n", data & 0x0F); */
  /* xprintf("data & 0x80 >> 3: %08b\n", (data & 0x80) >> 3); */
  /* xprintf("0xE0:             %08b\n", 0xE0); */
  return ~(0xE0 | ((data & 0x80) >> 3) | (data & 0x0F));
}

static void unselect_columns(void)
{
  mcp23018_status = i2c_start(I2C_ADDR_WRITE);    if (mcp23018_status) goto out;
  mcp23018_status = i2c_write(GPIOA);             if (mcp23018_status) goto out;
  mcp23018_status = i2c_write(0b11111111);        if (mcp23018_status) goto out;
  mcp23018_status = i2c_write(0b11111111);        if (mcp23018_status) goto out;

out:
  i2c_stop();
}

static void select_column(uint8_t col)
{
  uint8_t port = GPIOB;
  uint8_t data = ~(1<<col);
  if (col >= 6) {
    data = ~(1<<(col - 6));
    port = GPIOA;
  }

  // set col to low
  mcp23018_status = i2c_start(I2C_ADDR_WRITE);    if (mcp23018_status) goto out;
  mcp23018_status = i2c_write(port);              if (mcp23018_status) goto out;
  mcp23018_status = i2c_write(data);

out:
  i2c_stop();
}

