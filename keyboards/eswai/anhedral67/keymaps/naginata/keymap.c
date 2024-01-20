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
#include "keymap_japanese.h"

// 薙刀式
#include "naginata.h"
NGKEYS naginata_keys;
// 薙刀式

// Defines the keycodes used by our macros in process_record_user
enum custom_keycodes {
  QWERTY = NG_SAFE_RANGE,
  QGMLWY,
  RAISE, LOWER, ADJUST,
  NUMLOC,
  EISU,
  KANA2,
  ALPH,
  SALPH,
};

// Layers
enum kepmap_layers {
  _QGMLWY,
  _QWERTY,
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
// 薙刀式

#define DEG   UC(0x00B0)
#define DELTA UC(0x0394)
#define NTEQ  UC(0x2260)
#define NREQ  UC(0x2260)
#define MICRO UC(0x00B5)
#define EURO  UC(0x20AC)
#define LTE   UC(0x2264)
#define GTE   UC(0x2265)

// 薙刀式
enum combo_events {
  NAGINATA_ON_CMB,
  NAGINATA_OFF_CMB,
  LOGIN_CMB,
};

#if defined(DQGMLWY)
const uint16_t PROGMEM ngon_combo[] = {KC_I, KC_A, COMBO_END};
const uint16_t PROGMEM ngoff_combo[] = {KC_N, KC_R, COMBO_END};
const uint16_t PROGMEM login_combo[] = {KC_Q, KC_G, KC_M, COMBO_END};
#endif

combo_t key_combos[COMBO_COUNT] = {
  [NAGINATA_ON_CMB] = COMBO_ACTION(ngon_combo),
  [NAGINATA_OFF_CMB] = COMBO_ACTION(ngoff_combo),
  [LOGIN_CMB] = COMBO_ACTION(login_combo),
};
// 薙刀式

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* _QGMLWY
  +-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+
  |       F2        |        `        |        1        |        2        |        3        |        4        |        5        |        6        |        7        |        8        |        9        |        0        |        -        |        +        |        =        |
  +-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+
  |       ESC       |        Q        |        G        |        M        |        L        |        W        |        :        |                 |        Y        |        F        |        U        |        B        |        ,        |      BSPC       |                 |
  +-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+
  |       TAB       |        D        |        S        |        T        |        N        |        R        |        \        |                 |        I        |        A        |        E        |        O        |        '        |       ENT       |                 |
  +-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+
  |      LCTL       |        Z        |        X        |        C        |        V        |        J        |        (        |        )        |        K        |        H        |        P        |        .        |        ?        |       UP        |       DEL       |
  +-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+
  |      LSHFT      |                 |                 |      ALPH       |      LOWER      |LT(_SHIFT,KC_SPC)|       ENT       |                 |LT(_SHIFT,KC_SPC)|      RAISE      |      LALT       |                 |      LEFT       |      DOWN       |      RGHT       |
  +-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+
*/
  [_QGMLWY] = LAYOUT(
    KC_F2            ,JP_GRV           ,KC_1             ,KC_2             ,KC_3             ,KC_4             ,KC_5             ,KC_6             ,KC_7             ,KC_8             ,KC_9             ,KC_0             ,JP_MINS          ,JP_PLUS          ,JP_EQL           , \
    KC_ESC           ,KC_Q             ,KC_G             ,KC_M             ,KC_L             ,KC_W             ,JP_COLN          ,                  KC_Y             ,KC_F             ,KC_U             ,KC_B             ,JP_COMM          ,KC_BSPC                         , \
    KC_TAB           ,KC_D             ,KC_S             ,KC_T             ,KC_N             ,KC_R             ,JP_BSLS          ,                  KC_I             ,KC_A             ,KC_E             ,KC_O             ,JP_QUOT          ,KC_ENT                          , \
    KC_LCTL          ,KC_Z             ,KC_X             ,KC_C             ,KC_V             ,KC_J             ,JP_LPRN          ,JP_RPRN          ,KC_K             ,KC_H             ,KC_P             ,JP_DOT           ,JP_QUES          ,KC_UP            ,KC_DEL           , \
    LSHFT            ,                                    ALPH             ,LOWER            ,LT(_SHIFT,KC_SPC),KC_BSPC          ,                  LT(_SHIFT,KC_ENT),RAISE            ,KC_LALT          ,                  KC_LEFT          ,KC_DOWN          ,KC_RGHT
  ),

/* _NAGINATA
  +-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
  |  __   |  __   |  __   |  __   |  __   |  __   |  __   |  __   |  __   |  __   |  __   |  __   |  __   |  __   |  __   |
  +-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
  |  __   | NG_Q  | NG_W  | NG_E  | NG_R  | NG_T  |  __   |       | NG_Y  | NG_U  | NG_I  | NG_O  | NG_P  |  __   |       |
  +-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
  |  __   | NG_A  | NG_S  | NG_D  | NG_F  | NG_G  |  __   |       | NG_H  | NG_J  | NG_K  | NG_L  |NG_SCLN|  __   |       |
  +-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
  |  __   | NG_Z  | NG_X  | NG_C  | NG_V  | NG_B  |  __   |  __   | NG_N  | NG_M  |NG_COMM|NG_DOT |NG_SLSH|  __   |  __   |
  +-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
  |  __   |       |       |  __   |  __   |NG_SHFT|  __   |       |NG_SHFT|  __   |  __   |       |  __   |  __   |  __   |
  +-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
*/
  [_NAGINATA] = LAYOUT(
    _______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______, \
    _______,NG_Q   ,NG_W   ,NG_E   ,NG_R   ,NG_T   ,_______,        NG_Y   ,NG_U   ,NG_I   ,NG_O   ,NG_P   ,_______     , \
    _______,NG_A   ,NG_S   ,NG_D   ,NG_F   ,NG_G   ,_______,        NG_H   ,NG_J   ,NG_K   ,NG_L   ,NG_SCLN,_______     , \
    _______,NG_Z   ,NG_X   ,NG_C   ,NG_V   ,NG_B   ,_______,_______,NG_N   ,NG_M   ,NG_COMM,NG_DOT ,NG_SLSH,_______,_______, \
    _______,                _______,_______,NG_SHFT,_______,        NG_SHFT,_______,_______,        _______,_______,_______
  ),

/* _LOWER
  +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
  | __  | __  | F1  | F2  | F3  | F4  | F5  | F6  | F7  | F8  | F9  | F10 | F11 | F12 | __  |
  +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
  |     | XX  | XX  | XX  | XX  | XX  | XX  |     |  /  |  7  |  8  |  9  |  +  | __  |     |
  +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
  |     | XX  | XX  | XX  | XX  | XX  | XX  |     |  *  |  4  |  5  |  6  |  -  | __  |     |
  +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
  |     | XX  | XX  | XX  | XX  | XX  |  [  |  ]  |  0  |  1  |  2  |  3  |  =  |PGUP | __  |
  +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
  | __  |     |     |SALPH| __  | __  | __  |     | __  | __  | __  |     |HOME |PGDN | END |
  +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
*/
  [_LOWER] = LAYOUT(
    _______,_______,KC_F1  ,KC_F2  ,KC_F3  ,KC_F4  ,KC_F5  ,KC_F6  ,KC_F7  ,KC_F8  ,KC_F9  ,KC_F10 ,KC_F11 ,KC_F12 ,_______, \
    XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,        JP_SLSH,KC_7   ,KC_8   ,KC_9   ,JP_PLUS,_______     , \
    XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,        JP_ASTR,KC_4   ,KC_5   ,KC_6   ,JP_MINS,_______     , \
    XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,JP_LBRC,JP_RBRC,KC_0   ,KC_1   ,KC_2   ,KC_3   ,JP_EQL ,KC_PGUP,_______, \
    _______,                SALPH  ,_______,_______,_______,        _______,_______,_______,        KC_HOME,KC_PGDN,KC_END
  ),

/* _RAISE
  +-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
  |  __   |  __   |  __   |  __   |  __   |  __   |  __   |  __   |  __   |  __   |  __   |  __   |  __   |  __   |  __   |
  +-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
  |  __   |  XX   |   ¥   | MICRO | DELTA |  XX   |  XX   |       |  XX   | S(UP) |  UP   |  XX   |  XX   |  __   |       |
  +-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
  |  __   |  XX   | EURO  |  DEG  | NTEQ  |  XX   |  XX   |       |  XX   | LEFT  | DOWN  | RGHT  |  XX   |  __   |       |
  +-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
  |  __   |  XX   |  XX   |  XX   |  XX   |  XX   |   {   |   }   |  XX   |S(LEFT)|S(DOWN)|S(RGHT)|  XX   |  __   |  __   |
  +-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
  |  __   |       |       |  __   |  __   |  __   |  __   |       |  __   |  __   |  __   |       |  __   |  __   |  __   |
  +-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
*/
  [_RAISE] = LAYOUT(
    _______   ,_______   ,_______   ,_______   ,_______   ,_______   ,_______   ,_______   ,_______   ,_______   ,_______   ,_______   ,_______   ,_______   ,_______   , \
    _______   ,XXXXXXX   ,JP_YEN    ,MICRO     ,DELTA     ,XXXXXXX   ,XXXXXXX   ,           XXXXXXX   ,S(KC_UP)  ,KC_UP     ,XXXXXXX   ,XXXXXXX   ,_______           , \
    _______   ,XXXXXXX   ,EURO      ,DEG       ,NTEQ      ,XXXXXXX   ,XXXXXXX   ,           XXXXXXX   ,KC_LEFT   ,KC_DOWN   ,KC_RGHT   ,XXXXXXX   ,_______           , \
    _______   ,XXXXXXX   ,XXXXXXX   ,XXXXXXX   ,XXXXXXX   ,XXXXXXX   ,JP_LCBR   ,JP_RCBR   ,XXXXXXX   ,S(KC_LEFT),S(KC_DOWN),S(KC_RGHT),XXXXXXX   ,_______   ,_______   , \
    _______   ,                      _______   ,_______   ,_______   ,_______   ,           _______   ,_______   ,_______   ,           _______   ,_______   ,_______
  ),

/* _ADJUST
  +-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
  |  XX   |  XX   |  XX   |  XX   |  XX   |  XX   |  XX   |  XX   |  XX   |  XX   |  XX   |  XX   |  XX   |  XX   |  XX   |
  +-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
  |EEP_RST|QWERTY |  XX   | MAIL  |  XX   | WAKE  |  XX   |       |  XX   |  XX   |  XX   |  XX   |  XX   |  __   |       |
  +-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
  |  __   |QGMLWY | SLEP  |  XX   |  XX   | RESET |  XX   |       |  XX   |  XX   |  XX   |  XX   |  XX   |  __   |       |
  +-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
  |  __   |  XX   |  XX   | CALC  |  XX   |  XX   |  XX   |  XX   |  XX   |  XX   |  XX   |  XX   |  XX   |  __   |  __   |
  +-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
  |  __   |       |       |  __   |  __   |  __   |  __   |       |  __   |  __   |  __   |       |  __   |  __   |  __   |
  +-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
*/
  [_ADJUST] = LAYOUT(
    XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX, \
    EEP_RST,QWERTY ,XXXXXXX,KC_MAIL,XXXXXXX,KC_WAKE,XXXXXXX,        XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,_______     , \
    _______,QGMLWY ,KC_SLEP,XXXXXXX,XXXXXXX,RESET  ,XXXXXXX,        XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,_______     , \
    _______,XXXXXXX,XXXXXXX,KC_CALC,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,_______,_______, \
    _______,                _______,_______,_______,_______,        _______,_______,_______,        _______,_______,_______
  ),

/* _SHIFT
  +-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
  |  __   |   ~   |   !   |   @   |   #   |   $   |   %   |   ^   |   &   |   *   |   (   |   )   |   /   |   *   |   _   |
  +-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
  |S(ESC) | S(Q)  | S(G)  | S(M)  | S(L)  | S(W)  |   ;   |       | S(Y)  | S(F)  | S(U)  | S(B)  |  __   |S(BSPC)|       |
  +-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
  |S(TAB) | S(D)  | S(S)  | S(T)  | S(N)  | S(R)  |   |   |       | S(I)  | S(A)  | S(E)  | S(O)  |   "   |S(ENT) |       |
  +-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
  |  __   | S(Z)  | S(X)  | S(C)  | S(V)  | S(J)  |   <   |   >   | S(K)  | S(H)  | S(P)  |  __   |   /   | S(UP) |S(DEL) |
  +-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
  |  __   |       |       | LGUI  |  __   |  __   |  __   |       |  __   |  __   |  __   |       |S(LEFT)|S(DOWN)|S(RGHT)|
  +-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
*/
  [_SHIFT] = LAYOUT(
    _______   ,JP_TILD   ,JP_EXLM   ,JP_AT     ,JP_HASH   ,JP_DLR    ,JP_PERC   ,JP_CIRC   ,JP_AMPR   ,JP_ASTR   ,JP_LPRN   ,JP_RPRN   ,JP_SLSH   ,JP_ASTR   ,JP_UNDS   , \
    S(KC_ESC) ,S(KC_Q)   ,S(KC_G)   ,S(KC_M)   ,S(KC_L)   ,S(KC_W)   ,JP_SCLN   ,           S(KC_Y)   ,S(KC_F)   ,S(KC_U)   ,S(KC_B)   ,_______   ,S(KC_BSPC)        , \
    S(KC_TAB) ,S(KC_D)   ,S(KC_S)   ,S(KC_T)   ,S(KC_N)   ,S(KC_R)   ,JP_PIPE   ,           S(KC_I)   ,S(KC_A)   ,S(KC_E)   ,S(KC_O)   ,JP_DQUO    ,S(KC_ENT)         , \
    _______   ,S(KC_Z)   ,S(KC_X)   ,S(KC_C)   ,S(KC_V)   ,S(KC_J)   ,KC_LT     ,KC_GT     ,S(KC_K)   ,S(KC_H)   ,S(KC_P)   ,_______   ,JP_SLSH   ,S(KC_UP)  ,S(KC_DEL) , \
    _______   ,                      KC_LGUI   ,_______   ,_______   ,_______   ,           _______   ,_______   ,_______   ,           S(KC_LEFT),S(KC_DOWN),S(KC_RGHT)
  ),

/* _QWERTY
  +-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+
  |      HOME       |        `        |        1        |        2        |        3        |        4        |        5        |        6        |        7        |        8        |        9        |        0        |        [        |        ]        |                 |
  +-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+
  |       ESC       |        Q        |        W        |        E        |        R        |        T        |        :        |                 |        Y        |        U        |        I        |        O        |        P        |      BSPC       |                 |
  +-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+
  |       TAB       |        A        |        S        |        D        |        F        |        G        |        |        |                 |        H        |        J        |        K        |        L        |        ;        |       ENT       |                 |
  +-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+
  |      LCTL       |        Z        |        X        |        C        |        V        |        B        |        -        |        +        |        N        |        M        |        ,        |        .        |        /        |       UP        |       DEL       |
  +-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+
  |      LSHFT      |                 |                 |      ALPH       |      LOWER      |LT(_SHIFT,KC_SPC)|       ENT       |                 |LT(_SHIFT,KC_SPC)|      RAISE      |      LALT       |                 |      LEFT       |      DOWN       |      RGHT       |
  +-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+-----------------+
*/
  [_QWERTY] = LAYOUT(
    KC_HOME          ,JP_GRV           ,KC_1             ,KC_2             ,KC_3             ,KC_4             ,KC_5             ,KC_6             ,KC_7             ,KC_8             ,KC_9             ,KC_0             ,JP_LBRC          ,JP_RBRC          ,XXXXXXX          , \
    KC_ESC           ,KC_Q             ,KC_W             ,KC_E             ,KC_R             ,KC_T             ,JP_COLN          ,                  KC_Y             ,KC_U             ,KC_I             ,KC_O             ,KC_P             ,KC_BSPC                         , \
    KC_TAB           ,KC_A             ,KC_S             ,KC_D             ,KC_F             ,KC_G             ,JP_PIPE          ,                  KC_H             ,KC_J             ,KC_K             ,KC_L             ,JP_SCLN          ,KC_ENT                          , \
    KC_LCTL          ,KC_Z             ,KC_X             ,KC_C             ,KC_V             ,KC_B             ,JP_MINS          ,JP_PLUS          ,KC_N             ,KC_M             ,JP_COMM          ,JP_DOT           ,JP_SLSH          ,KC_UP            ,KC_DEL           , \
    LSHFT            ,                                    ALPH             ,LOWER            ,LT(_SHIFT,KC_SPC),KC_ENT           ,                  LT(_SHIFT,KC_SPC),RAISE            ,KC_LALT          ,                  KC_LEFT          ,KC_DOWN          ,KC_RGHT
  ),

};

void persistent_default_layer_set(uint16_t default_layer) {
  eeconfig_update_default_layer(default_layer);
  default_layer_set(default_layer);
}

static bool num_toggle = false;
static bool nstate = false;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

  // tap=toggle layer, hold=momentally layer on
  if (keycode == NUMLOC) {
    if (record->event.pressed) {
      num_toggle = true;
      layer_invert(_LOWER);
    } else {
      if (layer_state_is(_LOWER) && num_toggle == false) {
        layer_invert(_LOWER);
      }
    }
    return false;
  } else {
    num_toggle = false;
  }

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
    case LOGIN_CMB:
      if (pressed && !naginata_state()) {
        register_code (KC_LCTRL);
        register_code (KC_LALT);
        register_code (KC_DEL);
        unregister_code (KC_DEL);
        unregister_code (KC_LALT);
        unregister_code (KC_LCTRL);
      }
      break;
  }
}
// 薙刀式

void matrix_init_user(void) {
  // 薙刀式
  set_naginata(_NAGINATA);
  // 薙刀式
  set_unicode_input_mode(UC_WINC);
}

void matrix_scan_user(void) {

}

void led_set_user(uint8_t usb_led) {

}
