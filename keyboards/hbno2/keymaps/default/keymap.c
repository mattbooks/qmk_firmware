#include QMK_KEYBOARD_H
#include "debug.h"
#include "action_layer.h"
#include "version.h"

#define BASE 0 // default layer
#define SYMB 1 // symbols
#define MDIA 2 // media keys

enum custom_keycodes {
  PLACEHOLDER = SAFE_RANGE, // can always be here
  EPRM,
  VRSN,
  RGB_SLD
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[BASE] = KEYMAP(  // layer 0 : default
        // left hand
        CTL_T(KC_ESC), KC_1,    KC_2,      KC_3,    KC_4,     KC_5,      KC_6,         KC_7,  KC_8,     KC_9,      KC_0,      CTL_T(KC_ESC),
        KC_LGUI,       KC_Q,    KC_W,      KC_E,    KC_R,     KC_T,      KC_Y,         KC_U,  KC_I,     KC_O,      KC_P,      KC_RGUI,
        KC_LCTRL,      KC_A,    KC_S,      KC_D,    KC_F,     KC_G,      KC_H,         KC_J,  KC_K,     KC_L,      KC_SCOLON, KC_RCTRL,
        KC_LSHIFT,     KC_Z,    KC_X,      KC_C,    KC_V,     KC_B,      KC_N,         KC_M,  KC_COMMA, KC_DOT,    KC_SLASH,  KC_RSHIFT,
        KC_NO,         KC_LALT, KC_LSHIFT, KC_LGUI, KC_LCTRL, KC_BSPACE, KC_TAB,   KC_RCTRL,  KC_RGUI,  KC_RSHIFT, KC_RALT,   KC_NO,

                       KC_SPACE, KC_NO, KC_NO, KC_ENTER)
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
  switch (keycode) {
    // dynamically generate these.
    case EPRM:
      if (record->event.pressed) {
        eeconfig_init();
      }
      return false;
      break;
    case VRSN:
      if (record->event.pressed) {
        SEND_STRING (QMK_KEYBOARD "/" QMK_KEYMAP " @ " QMK_VERSION);
      }
      return false;
      break;
    case RGB_SLD:
      if (record->event.pressed) {
        #ifdef RGBLIGHT_ENABLE
          rgblight_mode(1);
        #endif
      }
      return false;
      break;
  }
  return true;
}

// Runs just one time when the keyboard initializes.
void matrix_init_user(void) {
#ifdef RGBLIGHT_COLOR_LAYER_0
  rgblight_setrgb(RGBLIGHT_COLOR_LAYER_0);
#endif
};

// Runs constantly in the background, in a loop.
void matrix_scan_user(void) {
  print("LOOP");

};

// Runs whenever there is a layer state change.
uint32_t layer_state_set_user(uint32_t state) {
  return state;
};
