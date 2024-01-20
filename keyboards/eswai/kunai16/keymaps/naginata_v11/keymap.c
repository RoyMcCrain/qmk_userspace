/* Copyright 2018-2019 eswai <@eswai>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H
#include "action_layer.h"
#include "eeconfig.h"

#include "keymap_japanese.h"

extern keymap_config_t keymap_config;

// 薙刀式
#include "naginata.h"
NGKEYS naginata_keys;
// 薙刀式

// Defines the keycodes used by our macros in process_record_user
enum custom_keycodes {
  QWERTY = NG_SAFE_RANGE,
  EUCALYN,
  WORKMAN,
  QGMLWY,
  RAISE,
  LOWER,
  NUMBER,
  ADJUST,
  RGBRST,
  EISU,
  KANA2,
  EURO,
  SSCLN,
  SEQL,
  DELA,
  DELE,
  ALPH,
  SALPH,
};

// Layers
enum kepmap_layers {
  _QGMLWY,
  _SHIFT,
// 薙刀式
  _NAGINATA,
// 薙刀式
  _LOWER,
  _RAISE,
  _ADJUST,
};

#define _____   KC_TRNS
#define XXXXX   KC_NO
#define KFUNC   TD(FUNC)
#define SFTSPC  SFT_T(KC_SPC)
#define ALTENT  ALT_T(KC_ENT)
#define ALTSPC  ALT_T(KC_SPC)
#define ALTBS   ALT_T(KC_BSPC)
#define CMDSPC  CMD_T(KC_SPC)
#define CMDBSP  CMD_T(KC_BSPC)
#define CTLENT  CTL_T(KC_ENT)
#define CTLSPC  CTL_T(KC_SPC)
#define CTLBSP  CTL_T(KC_BSPC)
#define CMDENT  CMD_T(KC_ENT)
#define CTLBS   CTL_T(KC_BSPC)
#define LSHFT LM(_SHIFT,MOD_LSFT)
// 薙刀式
// 編集モードを追加する場合
#define KC(A) C(KC_##A)
#define KS(A) S(KC_##A)
#define KG(A) G(KC_##A)
#define KA(B) A(KC_##B)
// 薙刀式

// 薙刀式
enum combo_events {
  NAGINATA_ON_CMB,
  NAGINATA_OFF_CMB,
  ENTER_CMB,
};

#if defined(DQGMLWY)
const uint16_t PROGMEM ngon_combo[] = {KC_I, KC_A, COMBO_END};
const uint16_t PROGMEM ngoff_combo[] = {KC_N, KC_R, COMBO_END};
const uint16_t PROGMEM enter_combo[] = {KC_V, KC_H, COMBO_END};
#endif

combo_t key_combos[COMBO_COUNT] = {
  [NAGINATA_ON_CMB] = COMBO_ACTION(ngon_combo),
  [NAGINATA_OFF_CMB] = COMBO_ACTION(ngoff_combo),
  [ENTER_CMB] = COMBO(enter_combo, KC_ENT),
};
// 薙刀式

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* _QGMLWY
  +-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+
  |        Q        |        G        |        M        |        L        |        W        |        Y        |        F        |        U        |        B        |      BSPC       |
  +-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+
  |        D        |        S        |        T        |        N        |        R        |        I        |        A        |        E        |        O        |       ENT       |
  +-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+
  |        Z        |        X        |        C        |        V        |        J        |        K        |        H        |        P        |        ,        |        .        |
  +-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+
  |                 |                 |      LCMD       |      LOWER      |LT(_SHIFT,KC_SPC)|LT(_SHIFT,KC_ENT)|      RAISE      |      RCTL       |                 |                 |
  +-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+
*/
  [_QGMLWY] = LAYOUT(
    KC_Q             ,KC_G             ,KC_M             ,KC_L             ,KC_W             ,KC_Y             ,KC_F             ,KC_U             ,KC_B             ,KC_BSPC          , \
    KC_D             ,KC_S             ,KC_T             ,KC_N             ,KC_R             ,KC_I             ,KC_A             ,KC_E             ,KC_O             ,KC_ENT           , \
    KC_Z             ,KC_X             ,KC_C             ,KC_V             ,KC_J             ,KC_K             ,KC_H             ,KC_P             ,KC_COMM          ,KC_DOT           , \
                                        KC_LCMD          ,LOWER            ,LT(_SHIFT,KC_SPC),LT(_SHIFT,KC_ENT),RAISE            ,KC_RCTL
  ),

/* _SHIFT
  +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
  |S(Q) |S(G) |S(M) |S(L) |S(W) |S(Y) |S(F) |S(U) |S(B) | DEL |
  +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
  |S(D) |S(S) |S(T) |S(N) |S(R) |S(I) |S(A) |S(E) |S(O) |  :  |
  +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
  |S(Z) |S(X) |S(C) |S(V) |S(J) |S(K) |S(H) |S(P) |  /  |  ?  |
  +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
  |     |     | __  | __  | __  | __  | __  | __  |     |     |
  +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
*/
  [_SHIFT] = LAYOUT(
    S(KC_Q),S(KC_G),S(KC_M),S(KC_L),S(KC_W),S(KC_Y),S(KC_F),S(KC_U),S(KC_B),KC_DEL , \
    S(KC_D),S(KC_S),S(KC_T),S(KC_N),S(KC_R),S(KC_I),S(KC_A),S(KC_E),S(KC_O),KC_COLN, \
    S(KC_Z),S(KC_X),S(KC_C),S(KC_V),S(KC_J),S(KC_K),S(KC_H),S(KC_P),KC_SLSH,KC_QUES, \
                    _______,_______,_______,_______,_______,_______
  ),

/* _LOWER
  +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
  | ESC |     |     |  :  |  ;  |  /  |  7  |  8  |  9  |  -  |
  +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
  | TAB |  {  |  [  |  (  |  <  |  *  |  4  |  5  |  6  |  +  |
  +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
  |     |  }  |  ]  |  )  |  >  |  0  |  1  |  2  |  3  |  =  |
  +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
  |     |     | __  | __  | __  | __  | __  | __  |     |     |
  +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
*/
  [_LOWER] = LAYOUT(
    KC_ESC ,XXXXXXX,XXXXXXX,KC_COLN,KC_SCLN,KC_SLSH,KC_7   ,KC_8   ,KC_9   ,KC_MINS, \
    KC_TAB ,KC_LCBR,KC_LBRC,KC_LPRN,KC_LT  ,KC_ASTR,KC_4   ,KC_5   ,KC_6   ,KC_PLUS, \
    XXXXXXX,KC_RCBR,KC_RBRC,KC_RPRN,KC_GT  ,KC_0   ,KC_1   ,KC_2   ,KC_3   ,KC_EQL , \
                    _______,_______,_______,_______,_______,_______
  ),

/* "
  +-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
  |   ~   |   @   |   #   |   $   |   %   | HOME  | S(UP) |  UP   |       | BSPC  |
  +-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
  |   ^   |   &   |   !   |   ?   |   ¥   |  END  | LEFT  | DOWN  | RGHT  |       |
  +-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
  |   |   |   `   |   '   |   "   |   _   |       |S(LEFT)|S(DOWN)|S(RGHT)|       |
  +-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
  |ndefine|ndefine|ndefine|ndefine|ndefine|ndefine|ndefine|ndefine|ndefine|ndefine|
  +-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
  |       |       |  __   |  __   |  __   |  __   |  __   |  __   |       |       |
  +-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
*/
  [_RAISE] = LAYOUT(
    KC_TILD   ,KC_AT     ,KC_HASH   ,KC_DLR    ,KC_PERC   ,KC_HOME   ,S(KC_UP)  ,KC_UP     ,XXXXXXX   ,KC_BSPC   , \
    KC_CIRC   ,KC_AMPR   ,KC_EXLM   ,KC_QUES   ,KC_JYEN   ,KC_END    ,KC_LEFT   ,KC_DOWN   ,KC_RGHT   ,XXXXXXX   , \
    KC_PIPE   ,KC_GRV    ,KC_QUOT   ,KC_DQT    ,KC_UNDS   ,XXXXXXX   ,S(KC_LEFT),S(KC_DOWN),S(KC_RGHT),XXXXXXX   , \
                          _______   ,_______   ,_______   ,_______   ,_______   ,_______
  ),

/* _ADJUST
  +------+------+------+------+------+------+------+------+------+------+
  |      |      | MAIL |      | WAKE |      |      |      |      |QWERTY|
  +------+------+------+------+------+------+------+------+------+------+
  |      | SLEP |      |      |RESET |      |      | MYCM |      |QGMLWY|
  +------+------+------+------+------+------+------+------+------+------+
  |      |      | CALC |      |      |      | PWR  |      |      |      |
  +------+------+------+------+------+------+------+------+------+------+
  |      |      |  __  |  __  |  __  |  __  |  __  |  __  |      |      |
  +------+------+------+------+------+------+------+------+------+------+
*/
  [_ADJUST] = LAYOUT(
    XXXXXXX,XXXXXXX,KC_MAIL,XXXXXXX,KC_WAKE,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,QWERTY , \
    XXXXXXX,KC_SLEP,XXXXXXX,XXXXXXX,RESET  ,XXXXXXX,XXXXXXX,KC_MYCM,XXXXXXX,QGMLWY , \
    XXXXXXX,XXXXXXX,KC_CALC,XXXXXXX,XXXXXXX,XXXXXXX,KC_PWR ,XXXXXXX,XXXXXXX,XXXXXXX, \
                    _______,_______,_______,_______,_______,_______
  ),

/* _NAGINATA
  +-------+-------+-------+-------+-------+-------+-------+-------+
  | NG_Q  | NG_W  | NG_E  | NG_R  | NG_T  | NG_Y  | NG_U  | NG_I  |NG_O|NG_P|
  +-------+-------+-------+-------+-------+-------+-------+-------+
  | NG_A  | NG_S  | NG_D  | NG_F  | NG_G  | NG_H  | NG_J  | NG_K  |NG_L|NG_SCLN|
  +-------+-------+-------+-------+-------+-------+-------+-------+
  | NG_Z  | NG_X  | NG_C  | NG_V  | NG_B  | NG_N  | NG_M  |NG_COMM|NG_DOT|NG_SLSH|
  +-------+-------+-------+-------+-------+-------+-------+-------+
  |       |       |  __   |  __   |NG_SHFT|NG_SHFT|  __   |  __   |  |  |
  +-------+-------+-------+-------+-------+-------+-------+-------+
*/
  [_NAGINATA] = LAYOUT(
    NG_Q   ,NG_W   ,NG_E   ,NG_R   ,NG_T   ,NG_Y   ,NG_U   ,NG_I   ,NG_O,NG_P, \
    NG_A   ,NG_S   ,NG_D   ,NG_F   ,NG_G   ,NG_H   ,NG_J   ,NG_K   ,NG_L,NG_SCLN, \
    NG_Z   ,NG_X   ,NG_C   ,NG_V   ,NG_B   ,NG_N   ,NG_M   ,NG_COMM,NG_DOT,NG_SLSH, \
                    _______,_______,NG_SHFT,NG_SHFT,_______,_______
  ),

};

void persistent_default_layer_set(uint16_t default_layer) {
  eeconfig_update_default_layer(default_layer);
  default_layer_set(default_layer);
}

static bool nstate = false;

uint16_t timer;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

  // if (record->event.pressed) {
  //   uint16_t nowt = timer_read();
  //   if (nowt - timer > 5000) {
  //     makesure_mode();
  //   }
  //   timer = nowt;
  // }

  switch (keycode) {
    case ADJUST:
      if (record->event.pressed) {
        layer_on(_ADJUST);
      } else {
        layer_off(_ADJUST);
      }
      return false;
      break;
    case LOWER:
      if (record->event.pressed) {
        layer_on(_LOWER);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_LOWER);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      }
      return false;
      break;
    case RAISE:
      if (record->event.pressed) {
        layer_on(_RAISE);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_RAISE);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      }
      return false;
      break;
    case KANA2:
      if (record->event.pressed) {
        naginata_on();
      }
      return false;
      break;
    case EISU:
      if (record->event.pressed) {
        naginata_off();
      }
      return false;
      break;
    case QGMLWY:
      if (record->event.pressed) {
        persistent_default_layer_set(1UL<<_QGMLWY);
      }
      return false;
      break;
    case EURO:
      if (record->event.pressed) {
        send_string("(e)");
        return false;
      }
      break;
    case SSCLN:
      if (record->event.pressed) {
        unregister_code(KC_LSFT);
        tap_code(KC_SCLN);
        return false;
      }
      break;
    case SEQL:
      if (record->event.pressed) {
        unregister_code(KC_LSFT);
        tap_code(KC_EQL);
        return false;
      }
      break;
    case DELA:
      if (record->event.pressed) {
        register_code(KC_LSFT);
        tap_code(KC_HOME);
        unregister_code(KC_LSFT);
        register_code(KC_LCTRL);
        tap_code(KC_X);
        unregister_code(KC_LCTRL);
        return false;
      }
      break;
    case DELE:
      if (record->event.pressed) {
        register_code(KC_LSFT);
        tap_code(KC_END);
        unregister_code(KC_LSFT);
        register_code(KC_LCTRL);
        tap_code(KC_X);
        unregister_code(KC_LCTRL);
      }
      break;
    case ALPH:
      if (record->event.pressed) {
        nstate = naginata_state();
        if (nstate) naginata_off();
      } else {
        if (nstate) naginata_on();
      }
      break;
    case SALPH:
      if (record->event.pressed) {
        nstate = naginata_state();
        if (nstate) naginata_off();
        register_code(KC_LSFT);
      } else {
        unregister_code(KC_LSFT);
        if (nstate) naginata_on();
      }
      break;
    }

  // 薙刀式
    if (!process_naginata(keycode, record))
      return false;
  // 薙刀式

  return true;
}

// 薙刀式
// IME ONのcombo
void process_combo_event(uint8_t combo_index, bool pressed) {
  switch(combo_index) {
    case NAGINATA_ON_CMB:
      if (pressed) {
        naginata_on();
      }
      break;
    case NAGINATA_OFF_CMB:
      if (pressed) {
        naginata_off();
      }
      break;
  }
}
// 薙刀式

void matrix_init_user(void) {
  // 薙刀式
  set_naginata(_NAGINATA);
  #ifdef NAGINATA_EDIT_MAC
  set_unicode_input_mode(UC_OSX);
  #endif
  #ifdef NAGINATA_EDIT_WIN
  set_unicode_input_mode(UC_WINC);
  #endif
  // 薙刀式
  timer = timer_read();
}

void matrix_scan_user(void) {

}

void led_set_user(uint8_t usb_led) {

}
