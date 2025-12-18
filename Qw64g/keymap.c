#include QMK_KEYBOARD_H
#include "version.h"
#define MOON_LED_LEVEL LED_LEVEL
#ifndef ZSA_SAFE_RANGE
#define ZSA_SAFE_RANGE SAFE_RANGE
#endif

enum custom_keycodes {
  RGB_SLD = ZSA_SAFE_RANGE,
  DRAG_SCROLL,
  TOGGLE_SCROLL,
  NAVIGATOR_INC_CPI,
  NAVIGATOR_DEC_CPI,
  NAVIGATOR_TURBO,
  NAVIGATOR_AIM,
  OS_SYM,
  OS_NAV,
};

// Layer states
typedef enum {
    LAYER_OFF,        // Layer inactive
    LAYER_HELD,       // Layer key is being held, no other key pressed yet
    LAYER_HELD_USED,  // Layer key is being held, another key was pressed
    LAYER_STICKY,     // Layer was tapped, waiting for a key press
    LAYER_STICKY_USED,// Layer is sticky and a key was pressed, waiting for release
} layer_mode_t;

static layer_mode_t sym_mode = LAYER_OFF;
static layer_mode_t nav_mode = LAYER_OFF;

// Timeout tracking
static uint16_t sym_timer = 0;
static uint16_t nav_timer = 0;


#define DUAL_FUNC_0 LT(14, KC_7)
#define DUAL_FUNC_1 LT(1, KC_F1)
#define DUAL_FUNC_2 LT(6, KC_F2)
#define DUAL_FUNC_3 LT(12, KC_F16)
#define DUAL_FUNC_4 LT(11, KC_F5)
#define DUAL_FUNC_5 LT(5, KC_F15)
#define DUAL_FUNC_6 LT(15, KC_I)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT_voyager(
    KC_TRANSPARENT, KC_1,           KC_2,           KC_3,           KC_4,           KC_5,                                           KC_6,           KC_7,           KC_8,           KC_9,           KC_0,           KC_TRANSPARENT, 
    KC_TRANSPARENT, KC_Q,           KC_W,           KC_E,           KC_R,           KC_T,                                           KC_Y,           KC_U,           KC_I,           KC_O,           QK_REPEAT_KEY,  KC_TRANSPARENT,
    KC_TRANSPARENT, KC_A,           KC_S,           KC_D,           KC_F,           KC_G,                                           KC_H,           KC_J,           KC_K,           KC_L,           KC_P,           KC_TRANSPARENT, 
    KC_TRANSPARENT, KC_Z,           KC_X,           KC_C,           KC_V,           KC_B,                                           KC_N,           KC_M,           KC_COMMA,       KC_DOT,         KC_QUOTE,       KC_TRANSPARENT, 
                                                    OSM(MOD_LSFT),  OS_SYM,                                         OS_NAV,         KC_SPACE
  ),
  [1] = LAYOUT_voyager(
    KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,                                 KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, 
    KC_TRANSPARENT, KC_EXLM,        KC_AT,          KC_HASH,        KC_DLR,         KC_LBRC,                                        KC_RBRC,        KC_AMPR,        KC_QUES,        KC_PIPE,        KC_BSLS,        KC_TRANSPARENT, 
    KC_TRANSPARENT, KC_GRAVE,       KC_SLASH,       KC_UNDS,        KC_MINUS,       KC_LPRN,                                        KC_RPRN,        OSM(MOD_LGUI),  OSM(MOD_LALT),  OSM(MOD_LCTL),  OSM(MOD_LSFT),  KC_TRANSPARENT, 
    KC_TRANSPARENT, KC_ASTR,        KC_PLUS,        KC_EQUAL,       KC_COLN,        KC_LCBR,                                        KC_RCBR,        KC_PERC,        KC_CIRC,        KC_TILD,        KC_SCLN,        KC_TRANSPARENT, 
                                                    KC_LEFT_SHIFT,  KC_TRANSPARENT,                                 KC_TRANSPARENT, KC_TRANSPARENT
  ),
  [2] = LAYOUT_voyager(
    KC_NO,          KC_NO,          KC_NO,          KC_NO,          KC_NO,          KC_NO,                                          KC_NO,          KC_NO,          KC_NO,          KC_NO,          KC_NO,          KC_NO,          
    KC_NO,          KC_NO,          KC_NO,          KC_NO,          KC_NO,          KC_NO,                                          LCTL(LSFT(KC_TAB)),KC_PGDN,        KC_PAGE_UP,     LCTL(KC_TAB),   KC_DELETE,      KC_NO,          
    KC_NO,          OSM(MOD_LSFT),  OSM(MOD_LALT),  OSM(MOD_LALT),  OSM(MOD_LGUI),  CW_TOGG,                                        KC_LEFT,        KC_DOWN,        KC_UP,          KC_RIGHT,       KC_BSPC,        KC_NO,          
    KC_NO,          KC_NO,          KC_NO,          KC_NO,          KC_NO,          KC_NO,                                          KC_HOME,        KC_TAB,         KC_ESCAPE,      KC_END,         KC_ENTER,       KC_NO,          
                                                    KC_LEFT_SHIFT,  KC_TRANSPARENT,                                 KC_TRANSPARENT, KC_TRANSPARENT
  ),
  [3] = LAYOUT_voyager(
    KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,                                 KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, 
    KC_TRANSPARENT, KC_1,           KC_2,           KC_3,           KC_4,           KC_5,                                           KC_6,           KC_7,           KC_8,           KC_9,           KC_0,           KC_TRANSPARENT, 
    KC_TRANSPARENT, KC_LEFT_SHIFT,  KC_LEFT_CTRL,   KC_LEFT_ALT,    KC_LEFT_GUI,    KC_F11,                                         KC_F12,         KC_LEFT_GUI,    KC_LEFT_ALT,    KC_LEFT_CTRL,   KC_LEFT_SHIFT,  KC_TRANSPARENT, 
    KC_TRANSPARENT, KC_F1,          KC_F2,          KC_F3,          KC_F4,          KC_F5,                                          KC_F6,          KC_F7,          KC_F8,          KC_F9,          KC_F10,         KC_TRANSPARENT, 
                                                    KC_TRANSPARENT, KC_TRANSPARENT,                                 KC_TRANSPARENT, KC_TRANSPARENT
  ),
  [4] = LAYOUT_voyager(
    KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,                                 KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, 
    KC_TRANSPARENT, DUAL_FUNC_0,    DUAL_FUNC_1,    DUAL_FUNC_2,    OSM(MOD_LGUI),  KC_TRANSPARENT,                                 KC_TRANSPARENT, DUAL_FUNC_3,    DUAL_FUNC_4,    DUAL_FUNC_5,    DUAL_FUNC_6,    KC_TRANSPARENT, 
    KC_TRANSPARENT, NAVIGATOR_AIM,  KC_MS_BTN2,     KC_MS_BTN3,     KC_MS_BTN1,     DRAG_SCROLL,                                    KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, 
    KC_TRANSPARENT, LGUI(KC_Z),     LGUI(KC_X),     LGUI(KC_C),     LGUI(KC_V),     TO(0),                                          TOGGLE_SCROLL,  KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,
                                                    KC_TRANSPARENT, KC_TRANSPARENT,                                 KC_TRANSPARENT, KC_TRANSPARENT
  ),
  [5] = LAYOUT_voyager(
    KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,                                 KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, 
    KC_TRANSPARENT, KC_NO,          KC_AUDIO_VOL_DOWN,KC_AUDIO_MUTE,  KC_AUDIO_VOL_UP,KC_BRIGHTNESS_UP,                                RGB_VAI,        RGB_TOG,        TOGGLE_LAYER_COLOR,RGB_MODE_FORWARD,RGB_HUI,        KC_TRANSPARENT, 
    KC_TRANSPARENT, KC_NO,          KC_MEDIA_PREV_TRACK,KC_MEDIA_PLAY_PAUSE,KC_MEDIA_NEXT_TRACK,KC_BRIGHTNESS_DOWN,                                RGB_VAD,        KC_NO,          KC_NO,          KC_NO,          RGB_HUD,        KC_TRANSPARENT, 
    KC_TRANSPARENT, KC_NO,          KC_NO,          KC_NO,          KC_NO,          TO(0),                                          KC_NO,          KC_NO,          KC_NO,          KC_NO,          QK_BOOT,        KC_TRANSPARENT, 
                                                    KC_TRANSPARENT, KC_TRANSPARENT,                                 KC_TRANSPARENT, KC_TRANSPARENT
  ),
};


const uint16_t PROGMEM combo0[] = { OSL(2), KC_SPACE, COMBO_END};
const uint16_t PROGMEM combo1[] = { OSL(1), OSM(MOD_LSFT), COMBO_END};

combo_t key_combos[COMBO_COUNT] = {
    COMBO(combo0, OSL(5)),
    COMBO(combo1, KC_LEFT_GUI),
};

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case KC_SPACE:
            return TAPPING_TERM -25;
        default:
            return TAPPING_TERM;
    }
}


extern rgb_config_t rgb_matrix_config;

RGB hsv_to_rgb_with_value(HSV hsv) {
  RGB rgb = hsv_to_rgb( hsv );
  float f = (float)rgb_matrix_config.hsv.v / UINT8_MAX;
  return (RGB){ f * rgb.r, f * rgb.g, f * rgb.b };
}

void keyboard_post_init_user(void) {
  rgb_matrix_enable();
}

const uint8_t PROGMEM ledmap[][RGB_MATRIX_LED_COUNT][3] = {
    [0] = { {80,178,177}, {80,178,177}, {80,178,177}, {80,178,177}, {80,178,177}, {80,178,177}, {80,178,177}, {80,178,177}, {80,178,177}, {80,178,177}, {80,178,177}, {80,178,177}, {80,178,177}, {86,158,169}, {93,161,161}, {101,164,152}, {109,168,144}, {118,172,136}, {118,172,136}, {118,172,136}, {118,172,136}, {118,172,136}, {118,172,136}, {118,172,136}, {118,172,136}, {118,172,136}, {80,178,177}, {80,178,177}, {80,178,177}, {80,178,177}, {80,178,177}, {80,178,177}, {80,178,177}, {80,178,177}, {80,178,177}, {80,178,177}, {80,178,177}, {80,178,177}, {118,172,136}, {109,168,144}, {101,164,152}, {93,161,161}, {86,158,169}, {80,178,177}, {118,172,136}, {118,172,136}, {118,172,136}, {118,172,136}, {118,172,136}, {118,172,136}, {118,172,136}, {118,172,136} },

    [1] = { {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {94,218,139}, {94,218,139}, {94,218,139}, {94,218,139}, {152,255,255}, {0,0,0}, {94,218,139}, {94,218,139}, {238,254,255}, {238,254,255}, {188,255,255}, {0,0,0}, {94,218,139}, {215,218,204}, {215,218,204}, {41,255,255}, {131,255,255}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {152,255,255}, {74,89,255}, {121,218,204}, {0,218,204}, {0,218,204}, {0,0,0}, {188,255,255}, {238,254,255}, {21,255,255}, {21,255,255}, {121,218,204}, {0,0,0}, {131,255,255}, {41,255,255}, {185,157,204}, {185,157,204}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} },

    [2] = { {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {161,198,230}, {161,198,230}, {161,198,230}, {161,198,230}, {80,178,177}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {193,255,255}, {41,255,255}, {41,255,255}, {193,255,255}, {161,198,230}, {0,0,0}, {90,218,204}, {90,218,204}, {90,218,204}, {90,218,204}, {161,198,230}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {161,198,230}, {0,0,0}, {0,0,0}, {0,0,0} },

    [3] = { {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {25,255,255}, {25,255,255}, {25,255,255}, {25,255,255}, {25,255,255}, {0,0,0}, {77,255,255}, {77,255,255}, {77,255,255}, {77,255,255}, {215,218,204}, {0,0,0}, {215,218,204}, {215,218,204}, {215,218,204}, {215,218,204}, {215,218,204}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {25,255,255}, {25,255,255}, {25,255,255}, {25,255,255}, {25,255,255}, {0,0,0}, {215,218,204}, {77,255,255}, {77,255,255}, {77,255,255}, {77,255,255}, {0,0,0}, {215,218,204}, {215,218,204}, {215,218,204}, {215,218,204}, {215,218,204}, {0,0,0}, {0,0,0}, {0,0,0} },

    [5] = { {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {41,255,255}, {0,245,245}, {74,255,255}, {27,239,227}, {0,0,0}, {0,0,0}, {193,255,255}, {193,255,255}, {193,255,255}, {27,239,227}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {74,89,255}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {27,239,227}, {161,198,230}, {41,255,255}, {41,255,255}, {41,255,255}, {0,0,0}, {27,239,227}, {0,0,0}, {0,0,0}, {0,0,0}, {41,255,255}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,245,245}, {0,0,0}, {0,0,0}, {0,0,0} },

};

void set_layer_color(int layer) {
  for (int i = 0; i < RGB_MATRIX_LED_COUNT; i++) {
    HSV hsv = {
      .h = pgm_read_byte(&ledmap[layer][i][0]),
      .s = pgm_read_byte(&ledmap[layer][i][1]),
      .v = pgm_read_byte(&ledmap[layer][i][2]),
    };
    if (!hsv.h && !hsv.s && !hsv.v) {
        rgb_matrix_set_color( i, 0, 0, 0 );
    } else {
        RGB rgb = hsv_to_rgb_with_value(hsv);
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
  }
}

bool rgb_matrix_indicators_user(void) {
  if (rawhid_state.rgb_control) {
      return false;
  }
  if (!keyboard_config.disable_layer_led) { 
    switch (biton32(layer_state)) {
      case 0:
        set_layer_color(0);
        break;
      case 1:
        set_layer_color(1);
        break;
      case 2:
        set_layer_color(2);
        break;
      case 3:
        set_layer_color(3);
        break;
      case 5:
        set_layer_color(5);
        break;
     default:
        if (rgb_matrix_get_flags() == LED_FLAG_NONE) {
          rgb_matrix_set_color_all(0, 0, 0);
        }
    }
  } else {
    if (rgb_matrix_get_flags() == LED_FLAG_NONE) {
      rgb_matrix_set_color_all(0, 0, 0);
    }
  }

  return true;
}

extern bool set_scrolling;
extern bool navigator_turbo;
extern bool navigator_aim;
void pointing_device_init_user(void) {
  set_auto_mouse_enable(true);
}

bool is_mouse_record_user(uint16_t keycode, keyrecord_t* record) {
  // All keys are not mouse keys when one shot auto mouse is enabled.
  return false;
}




bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
  case QK_MODS ... QK_MODS_MAX: 
    // Mouse keys with modifiers work inconsistently across operating systems, this makes sure that modifiers are always
    // applied to the mouse key that was pressed.
    if (IS_MOUSE_KEYCODE(QK_MODS_GET_BASIC_KEYCODE(keycode))) {
    if (record->event.pressed) {
        add_mods(QK_MODS_GET_MODS(keycode));
        send_keyboard_report();
        wait_ms(2);
        register_code(QK_MODS_GET_BASIC_KEYCODE(keycode));
        return false;
      } else {
        wait_ms(2);
        del_mods(QK_MODS_GET_MODS(keycode));
      }
    }
    break;

    case OS_SYM:
      if (record->event.pressed) {
          layer_on(_SYM);
          sym_mode = LAYER_HELD;
      } else {
          if (sym_mode == LAYER_HELD) {
              // Tapped without using - make sticky and start timeout
              sym_mode = LAYER_STICKY;
              sym_timer = timer_read();  // Only start timer when becoming sticky
          } else {
              // Was held+used or sticky+used - turn off
              sym_mode = LAYER_OFF;
              layer_off(_SYM);
          }
      }
      return false;

  case OS_NAV:
      if (record->event.pressed) {
          layer_on(_NAV);
          nav_mode = LAYER_HELD;
      } else {
          if (nav_mode == LAYER_HELD) {
              // Tapped without using - make sticky and start timeout
              nav_mode = LAYER_STICKY;
              nav_timer = timer_read();  // Only start timer when becoming sticky
          } else {
              // Was held+used or sticky+used - turn off
              nav_mode = LAYER_OFF;
              layer_off(_NAV);
          }
      }
      return false;

    case DUAL_FUNC_0:
      if (record->tap.count > 0) {
        if (record->event.pressed) {
          register_code16(LSFT(KC_MS_BTN1));
        } else {
          unregister_code16(LSFT(KC_MS_BTN1));
        }
      } else {
        if (record->event.pressed) {
          register_code16(KC_LEFT_SHIFT);
        } else {
          unregister_code16(KC_LEFT_SHIFT);
        }  
      }  
      return false;
    case DUAL_FUNC_1:
      if (record->tap.count > 0) {
        if (record->event.pressed) {
          register_code16(LCTL(KC_MS_BTN1));
        } else {
          unregister_code16(LCTL(KC_MS_BTN1));
        }
      } else {
        if (record->event.pressed) {
          register_code16(KC_LEFT_CTRL);
        } else {
          unregister_code16(KC_LEFT_CTRL);
        }  
      }  
      return false;
    case DUAL_FUNC_2:
      if (record->tap.count > 0) {
        if (record->event.pressed) {
          register_code16(LALT(KC_MS_BTN1));
        } else {
          unregister_code16(LALT(KC_MS_BTN1));
        }
      } else {
        if (record->event.pressed) {
          register_code16(KC_LEFT_ALT);
        } else {
          unregister_code16(KC_LEFT_ALT);
        }  
      }  
      return false;
    case DUAL_FUNC_3:
      if (record->tap.count > 0) {
        if (record->event.pressed) {
          register_code16(LGUI(KC_LBRC));
        } else {
          unregister_code16(LGUI(KC_LBRC));
        }
      } else {
        if (record->event.pressed) {
          register_code16(KC_LEFT_GUI);
        } else {
          unregister_code16(KC_LEFT_GUI);
        }  
      }  
      return false;
    case DUAL_FUNC_4:
      if (record->tap.count > 0) {
        if (record->event.pressed) {
          register_code16(LGUI(KC_RBRC));
        } else {
          unregister_code16(LGUI(KC_RBRC));
        }
      } else {
        if (record->event.pressed) {
          register_code16(KC_LEFT_ALT);
        } else {
          unregister_code16(KC_LEFT_ALT);
        }  
      }  
      return false;
    case DUAL_FUNC_5:
      if (record->tap.count > 0) {
        if (record->event.pressed) {
          register_code16(LCTL(LSFT(KC_TAB)));
        } else {
          unregister_code16(LCTL(LSFT(KC_TAB)));
        }
      } else {
        if (record->event.pressed) {
          register_code16(KC_LEFT_CTRL);
        } else {
          unregister_code16(KC_LEFT_CTRL);
        }  
      }  
      return false;
    case DUAL_FUNC_6:
      if (record->tap.count > 0) {
        if (record->event.pressed) {
          register_code16(LCTL(KC_TAB));
        } else {
          unregister_code16(LCTL(KC_TAB));
        }
      } else {
        if (record->event.pressed) {
          register_code16(KC_LEFT_SHIFT);
        } else {
          unregister_code16(KC_LEFT_SHIFT);
        }  
      }  
      return false;
    case DRAG_SCROLL:
      if (record->event.pressed) {
        set_scrolling = true;
      } else {
        set_scrolling = false;
      }
      return false;
    case TOGGLE_SCROLL:
      if (record->event.pressed) {
        set_scrolling = !set_scrolling;
      }
      return false;
    break;
  case NAVIGATOR_TURBO:
    if (record->event.pressed) {
      navigator_turbo = true;
    } else {
      navigator_turbo = false;
    }
    return false;
  case NAVIGATOR_AIM:
    if (record->event.pressed) {
      navigator_aim = true;
    } else {
      navigator_aim = false;
    }
    return false;
  case NAVIGATOR_INC_CPI:
    if (record->event.pressed) {
        pointing_device_set_cpi(1);
    }
    return false;
  case NAVIGATOR_DEC_CPI:
    if (record->event.pressed) {
        pointing_device_set_cpi(0);
    }
    return false;
    case RGB_SLD:
      if (record->event.pressed) {
        rgblight_mode(1);
      }
      return false;
  }

  // For non-layer keys (including OSMs)
  if (keycode != OS_SYM && keycode != OS_NAV) {
      if (record->event.pressed) {
          // Mark held layers as "used"
          if (sym_mode == LAYER_HELD) sym_mode = LAYER_HELD_USED;
          if (nav_mode == LAYER_HELD) nav_mode = LAYER_HELD_USED;
          // Mark sticky layers as "used" - ready to deactivate on release
          if (sym_mode == LAYER_STICKY) sym_mode = LAYER_STICKY_USED;
          if (nav_mode == LAYER_STICKY) nav_mode = LAYER_STICKY_USED;
      } else {
          // On release, clear if we pressed a key while sticky
          if (sym_mode == LAYER_STICKY_USED) {
              sym_mode = LAYER_OFF;
              layer_off(_SYM);
          }
          if (nav_mode == LAYER_STICKY_USED) {
              nav_mode = LAYER_OFF;
              layer_off(_NAV);
          }
      }
  }

  return true;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, _SYM, _NAV, _NUM);
}

// Matrix scan function to check for timeouts
void matrix_scan_user(void) {
    // Check SYM layer timeout
    if (sym_mode == LAYER_STICKY || sym_mode == LAYER_STICKY_USED) {
        if (timer_elapsed(sym_timer) > ONESHOT_LAYER_TIMEOUT) {
            sym_mode = LAYER_OFF;
            layer_off(_SYM);
        }
    }

    // Check NAV layer timeout
    if (nav_mode == LAYER_STICKY || nav_mode == LAYER_STICKY_USED) {
        if (timer_elapsed(nav_timer) > ONESHOT_LAYER_TIMEOUT) {
            nav_mode = LAYER_OFF;
            layer_off(_NAV);
        }
    }
}