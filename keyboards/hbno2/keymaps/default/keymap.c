#include QMK_KEYBOARD_H
#include "debug.h"
#include "action_layer.h"
#include "version.h"
#include "hbno2.h"

#define BASE 0 // default layer
#define SYMB 1 // symbols
#define MDIA 2 // media keys

enum custom_keycodes {
  PLACEHOLDER = SAFE_RANGE, // can always be here
  RGB_RUP,
  RGB_GUP,
  RGB_BUP,
  RGB_RDN,
  RGB_GDN,
  RGB_BDN
};

#define RGB_MAX = 0x3FF

#define _______ KC_TRNS

uint16_t rgb_r = 0x0;
uint16_t rgb_g = 0x0;
uint16_t rgb_b = 0x0;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[BASE] = KEYMAP(  // layer 0 : default
  // left hand
  KC_GRV,               KC_1,    KC_2,    KC_3,    KC_4,    KC_5,
  MT(MOD_LGUI, KC_TAB), KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,
  CTL_T(KC_ESC),        KC_A,    KC_S,    KC_D,    KC_F,    KC_G,
  MT(MOD_LSFT, KC_NO),  KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,
  LT(MDIA, KC_NO),      KC_LOCK, KC_LSFT, KC_LCTL, KC_LALT, LT(SYMB, KC_NO),

  KC_6,                 KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC,
  KC_Y,                 KC_U,    KC_I,    KC_O,    KC_P,    MT(MOD_RGUI, KC_PLUS),
  KC_H,                 KC_J,    KC_K,    KC_L,    KC_SCLN, MT(MOD_RCTL, KC_MINS),
  KC_N,                 KC_M,    KC_COMM, KC_DOT,  KC_SLSH, MT(MOD_RSFT, KC_BSLS),
  LT(SYMB, KC_TAB),     KC_RGUI, KC_RCTL, KC_RSFT, KC_RALT, LT(MDIA, KC_NO),

  KC_LGUI, KC_BSPC, KC_ENT, KC_SPC),


[SYMB] = KEYMAP(
  _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,
  _______, _______, _______, _______, _______, _______,
  _______, KC_LCBR, KC_LBRC, KC_LPRN, KC_QUOT, _______,
  _______, _______, _______, _______, _______, _______,
  _______, _______, _______, _______, _______, _______,

  KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_SLEP,
  _______, _______, _______, _______, _______, _______,
  _______, KC_DQUO, KC_RPRN, KC_RBRC, KC_RCBR, _______,
  _______, _______, _______, _______, KC_UP,   _______,
  _______, _______, _______, KC_LEFT, KC_DOWN, KC_RGHT,

  _______, _______, _______, _______),


[MDIA] = KEYMAP(
  KC_PAUS, KC_VOLU, KC_MNXT, _______, _______, _______,
  KC_SLCK, KC_VOLD, KC_MPRV, _______, _______, _______,
  _______, KC_MUTE, KC_MPLY, _______, _______, _______,
  _______, _______, _______, _______, _______, _______,
  _______, _______, _______, _______, _______, _______,

  _______, _______, _______, RGB_RUP, RGB_GUP, RGB_BUP,
  _______, _______, _______, RGB_RDN, RGB_GDN, RGB_BDN,
  _______, _______, _______, _______, _______, _______,
  _______, _______, _______, _______, _______, _______,
  _______, _______, _______, _______, _______, _______,

  _______, _______, _______, _______),
};

const uint16_t PROGMEM fn_actions[] = {
    [1] = ACTION_LAYER_TAP_TOGGLE(SYMB)                // FN1 - Momentary Layer 1 (Symbols)
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  /* if (record->event.pressed) { */
  /*   uint8_t set_rgb = 0; */

  /*   switch (keycode) { */
  /*     case RGB_RUP: */
  /*       set_rgb = 1; */
  /*       rgb_r = 0x3FF & (1 | rgb_r << 1); */
  /*       break; */
  /*     case RGB_GUP: */
  /*       set_rgb = 1; */
  /*       rgb_g = 0x3FF & (1 | rgb_g << 1); */
  /*       break; */
  /*     case RGB_BUP: */
  /*       set_rgb = 1; */
  /*       rgb_b = 0x3FF & (1 | rgb_b << 1); */
  /*       break; */
  /*     case RGB_RDN: */
  /*       set_rgb = 1; */
  /*       rgb_r = rgb_r >> 1; */
  /*       break; */
  /*     case RGB_GDN: */
  /*       set_rgb = 1; */
  /*       rgb_g = rgb_g >> 1; */
  /*       break; */
  /*     case RGB_BDN: */
  /*       set_rgb = 1; */
  /*       rgb_b = rgb_b >> 1; */
  /*       break; */
  /*   } */

  /*   if (set_rgb) { */
  /*     led_rgb(rgb_r, rgb_g, rgb_b); */
  /*   } */
  /* } */

  return true;
}

// Runs constantly in the background, in a loop.
void matrix_scan_user(void) {
};

extern void change_led(led_value_t);

const led_value_t LED_BASE = { 0x0, 0x1FF, 0x0 };
const led_value_t LED_SYMB = { 0x1FF, 0x0, 0x0 };
const led_value_t LED_MDIA = { 0x0, 0x0, 0x1FF };

// Runs whenever there is a layer state change.
uint32_t layer_state_set_user(uint32_t state) {
  switch (biton32(state)) {
    case BASE:
      change_led(LED_BASE);
      break;
    case SYMB:
      change_led(LED_SYMB);
      break;
    case MDIA:
      change_led(LED_MDIA);
      break;
  }

  return state;
};

// Runs just one time when the keyboard initializes.
void matrix_init_user(void) {
  change_led(LED_BASE);
};

