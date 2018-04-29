#include QMK_KEYBOARD_H
#include "debug.h"
#include "action_layer.h"
#include "version.h"
#include "hbno2.h"

#define BASE 0 // default layer
#define SYMB 1 // symbols
#define MDIA 2 // media keys

extern void led_rgb(uint16_t, uint16_t, uint16_t);

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

uint16_t rgb_r = 0x0;
uint16_t rgb_g = 0x0;
uint16_t rgb_b = 0x0;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[BASE] = KEYMAP(  // layer 0 : default
  // left hand
  CTL_T(KC_ESC),   KC_1,    KC_2,      KC_3,    KC_4,     KC_5,
  KC_LGUI,         KC_Q,    KC_W,      KC_E,    KC_R,     KC_T,
  KC_LCTRL,        KC_A,    KC_S,      KC_D,    KC_F,     KC_G,
  KC_LSHIFT,       KC_Z,    KC_X,      KC_C,    KC_V,     KC_B,
  LT(MDIA, KC_NO), KC_NO,   KC_LSHIFT, KC_LCTRL, KC_LALT, LT(SYMB, KC_NO),

  KC_6,         KC_7,  KC_8,     KC_9,      KC_0,      KC_BSPACE,
  KC_Y,         KC_U,  KC_I,     KC_O,      KC_P,      KC_RGUI,
  KC_H,         KC_J,  KC_K,     KC_L,      KC_SCOLON, KC_RCTRL,
  KC_N,         KC_M,  KC_COMMA, KC_DOT,    KC_SLASH,  KC_RSHIFT,
  LT(SYMB, KC_TAB),   KC_RGUI,  KC_RCTRL,  KC_RSHIFT, KC_RALT, LT(MDIA, KC_NO),

  KC_LGUI, KC_BSPACE, KC_ENTER, KC_SPACE),
[SYMB] = KEYMAP(
 CTL_T(KC_ESC), KC_F1,    KC_F2, KC_F3, KC_F4, KC_F5,
 KC_LGUI,       KC_GRAVE, KC_NO, KC_NO, KC_NO, KC_NO,
 KC_LCTRL,      KC_LEFT_CURLY_BRACE, KC_LBRACKET, KC_LEFT_PAREN, KC_QUOTE, KC_MINUS,
 KC_LSHIFT,     KC_NO,    KC_NO, KC_GRAVE, KC_EQUAL, KC_PIPE,
 KC_NO,         KC_LALT,  KC_LSHIFT, KC_LGUI, KC_LCTRL, KC_LGUI,

 KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_SYSTEM_SLEEP,
 KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,  KC_RGUI,
 KC_UNDERSCORE, KC_DOUBLE_QUOTE, KC_RIGHT_PAREN, KC_RBRACKET, KC_RIGHT_CURLY_BRACE, KC_RCTRL,
 KC_BSLASH, KC_PLUS, KC_NO, KC_NO, KC_UP, KC_RSHIFT,
 KC_RGUI, KC_RCTRL, KC_RGUI, KC_LEFT, KC_DOWN, KC_RIGHT,

  KC_LGUI, KC_BSPACE, KC_ENTER, KC_SPACE),
[MDIA] = KEYMAP(
 KC_PAUS, KC_VOLU, KC_MNXT, KC_NO, KC_NO, KC_NO,
 KC_SLCK, KC_VOLD, KC_MPRV, KC_NO, KC_NO, KC_NO,
 KC_NO,   KC_MUTE, KC_MPLY, KC_NO, KC_NO, KC_NO,
 KC_NO,   KC_NO,   KC_NO,   KC_NO, KC_NO, KC_NO,
 KC_NO,   KC_NO,   KC_NO,   KC_NO, KC_NO, KC_NO,

 KC_NO,   KC_NO,   KC_NO,   RGB_RUP, RGB_GUP, RGB_BUP,
 KC_NO,   KC_NO,   KC_NO,   RGB_RDN, RGB_GDN, RGB_BDN,
 KC_NO,   KC_NO,   KC_NO,   KC_NO, KC_NO, KC_NO,
 KC_NO,   KC_NO,   KC_NO,   KC_NO, KC_NO, KC_NO,
 KC_NO,   KC_NO,   KC_NO,   KC_NO, KC_NO, KC_NO,

  KC_NO, KC_NO, KC_NO, KC_NO),
};

const uint16_t PROGMEM fn_actions[] = {
    [1] = ACTION_LAYER_TAP_TOGGLE(SYMB)                // FN1 - Momentary Layer 1 (Symbols)
};

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
  // MACRODOWN only works in this function
  switch(id) {
    case 0:
      if (record->event.pressed) {
        SEND_STRING (QMK_KEYBOARD "/" QMK_KEYMAP " @ " QMK_VERSION);
      }
      break;
    case 1:
      if (record->event.pressed) { // For resetting EEPROM
        eeconfig_init();
      }
      break;
  }
  return MACRO_NONE;
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

// Runs whenever there is a layer state change.
uint32_t layer_state_set_user(uint32_t state) {
  switch (state) {
    case BASE:
      rgb_r = 0x00;
      rgb_g = 0x3F;
      rgb_b = 0x00;
      break;
    case SYMB:
      rgb_r = 0x3F;
      rgb_g = 0x00;
      rgb_b = 0x00;
      break;
    case MDIA:
      rgb_r = 0x00;
      rgb_g = 0x00;
      rgb_b = 0x3F;
      break;
  }

  led_rgb(rgb_r, rgb_g, rgb_b);

  return state;
};

// Runs just one time when the keyboard initializes.
void matrix_init_user(void) {
  layer_state_set_user(BASE);
};

