/* Copyright eswai <@eswai> / Satoru NAKAYA <@tor-nky>
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
#include "nglist.h"
#include "nglistarray.h"
#include "naginata.h"

#include <string.h>

user_config_t naginata_config; // EEPROMに保存する設定の実体

static bool is_naginata = false; // 薙刀式がオンかオフか
static uint8_t naginata_layer = 0; // NG_*を配置しているレイヤー番号
static uint16_t ngon_keys[2]; // 薙刀式をオンにするキー(通常HJ)
static uint16_t ngoff_keys[2]; // 薙刀式をオフにするキー(通常FG)
static NGListArray nginput;
static uint32_t pressed_keys; // 押しているキーのビットをたてる
static int8_t n_pressed_keys; // 押しているキーの数

// 31キーを32bitの各ビットに割り当てる
#define B_Q    (1UL<<0)
#define B_W    (1UL<<1)
#define B_E    (1UL<<2)
#define B_R    (1UL<<3)
#define B_T    (1UL<<4)

#define B_Y    (1UL<<5)
#define B_U    (1UL<<6)
#define B_I    (1UL<<7)
#define B_O    (1UL<<8)
#define B_P    (1UL<<9)

#define B_A    (1UL<<10)
#define B_S    (1UL<<11)
#define B_D    (1UL<<12)
#define B_F    (1UL<<13)
#define B_G    (1UL<<14)

#define B_H    (1UL<<15)
#define B_J    (1UL<<16)
#define B_K    (1UL<<17)
#define B_L    (1UL<<18)
#define B_SCLN (1UL<<19)

#define B_Z    (1UL<<20)
#define B_X    (1UL<<21)
#define B_C    (1UL<<22)
#define B_V    (1UL<<23)
#define B_B    (1UL<<24)

#define B_N    (1UL<<25)
#define B_M    (1UL<<26)
#define B_COMM (1UL<<27)
#define B_DOT  (1UL<<28)
#define B_SLSH (1UL<<29)

#define B_SHFT (1UL<<30)

// キーコードとキービットの対応
// メモリ削減のため配列はNG_Qを0にしている
const uint32_t ng_key[] = {
  [NG_Q    - NG_Q]  = B_Q,
  [NG_W    - NG_Q]  = B_W,
  [NG_E    - NG_Q]  = B_E,
  [NG_R    - NG_Q]  = B_R,
  [NG_T    - NG_Q]  = B_T,

  [NG_Y    - NG_Q]  = B_Y,
  [NG_U    - NG_Q]  = B_U,
  [NG_I    - NG_Q]  = B_I,
  [NG_O    - NG_Q]  = B_O,
  [NG_P    - NG_Q]  = B_P,

  [NG_A    - NG_Q]  = B_A,
  [NG_S    - NG_Q]  = B_S,
  [NG_D    - NG_Q]  = B_D,
  [NG_F    - NG_Q]  = B_F,
  [NG_G    - NG_Q]  = B_G,

  [NG_H    - NG_Q]  = B_H,
  [NG_J    - NG_Q]  = B_J,
  [NG_K    - NG_Q]  = B_K,
  [NG_L    - NG_Q]  = B_L,
  [NG_SCLN - NG_Q]  = B_SCLN,

  [NG_Z    - NG_Q]  = B_Z,
  [NG_X    - NG_Q]  = B_X,
  [NG_C    - NG_Q]  = B_C,
  [NG_V    - NG_Q]  = B_V,
  [NG_B    - NG_Q]  = B_B,

  [NG_N    - NG_Q]  = B_N,
  [NG_M    - NG_Q]  = B_M,
  [NG_COMM - NG_Q]  = B_COMM,
  [NG_DOT  - NG_Q]  = B_DOT,
  [NG_SLSH - NG_Q]  = B_SLSH,

  [NG_SHFT - NG_Q]  = B_SHFT,
  [NG_SHFT2 - NG_Q] = B_SHFT,
};

// カナ変換テーブル
typedef struct {
  uint32_t shift;
  uint32_t douji;
  const char *kana; // flash上の文字列リテラルへのポインタ。ASCII=send_string, マルチバイト=Unicode入力
  void (*func)(void);
} naginata_kanamap;

const PROGMEM naginata_kanamap ngdickana[] = {
  // 清音
  {.shift = 0UL        , .douji = B_J             , .kana = "a"      , .func = nofunc }, // あ
  {.shift = 0UL        , .douji = B_K             , .kana = "i"      , .func = nofunc }, // い
  {.shift = 0UL        , .douji = B_L             , .kana = "u"      , .func = nofunc }, // う
  {.shift = B_SHFT     , .douji = B_O             , .kana = "e"      , .func = nofunc }, // え
  {.shift = B_SHFT     , .douji = B_N             , .kana = "o"      , .func = nofunc }, // お
  {.shift = 0UL        , .douji = B_F             , .kana = "ka"     , .func = nofunc }, // か
  {.shift = 0UL        , .douji = B_W             , .kana = "ki"     , .func = nofunc }, // き
  {.shift = 0UL        , .douji = B_H             , .kana = "ku"     , .func = nofunc }, // く
  {.shift = 0UL        , .douji = B_S             , .kana = "ke"     , .func = nofunc }, // け
  {.shift = 0UL        , .douji = B_V             , .kana = "ko"     , .func = nofunc }, // こ
  {.shift = B_SHFT     , .douji = B_U             , .kana = "sa"     , .func = nofunc }, // さ
  {.shift = 0UL        , .douji = B_R             , .kana = "si"     , .func = nofunc }, // し
  {.shift = 0UL        , .douji = B_O             , .kana = "su"     , .func = nofunc }, // す
  {.shift = B_SHFT     , .douji = B_A             , .kana = "se"     , .func = nofunc }, // せ
  {.shift = 0UL        , .douji = B_B             , .kana = "so"     , .func = nofunc }, // そ
  {.shift = 0UL        , .douji = B_N             , .kana = "ta"     , .func = nofunc }, // た
  {.shift = B_SHFT     , .douji = B_G             , .kana = "ti"     , .func = nofunc }, // ち
  {.shift = B_SHFT     , .douji = B_L             , .kana = "tu"     , .func = nofunc }, // つ
  {.shift = 0UL        , .douji = B_E             , .kana = "te"     , .func = nofunc }, // て
  {.shift = 0UL        , .douji = B_D             , .kana = "to"     , .func = nofunc }, // と
  {.shift = 0UL        , .douji = B_M             , .kana = "na"     , .func = nofunc }, // な
  {.shift = B_SHFT     , .douji = B_D             , .kana = "ni"     , .func = nofunc }, // に
  {.shift = B_SHFT     , .douji = B_B             , .kana = "nu"     , .func = nofunc }, // ぬ
  {.shift = B_SHFT     , .douji = B_R             , .kana = "ne"     , .func = nofunc }, // ね
  {.shift = B_SHFT     , .douji = B_J             , .kana = "no"     , .func = nofunc }, // の
  {.shift = 0UL        , .douji = B_C             , .kana = "ha"     , .func = nofunc }, // は
  {.shift = 0UL        , .douji = B_X             , .kana = "hi"     , .func = nofunc }, // ひ
  {.shift = B_SHFT     , .douji = B_X             , .kana = "hi"     , .func = nofunc }, // ひ
  {.shift = B_SHFT     , .douji = B_SCLN          , .kana = "hu"     , .func = nofunc }, // ふ
  {.shift = 0UL        , .douji = B_P             , .kana = "he"     , .func = nofunc }, // へ
  {.shift = 0UL        , .douji = B_Z             , .kana = "ho"     , .func = nofunc }, // ほ
  {.shift = B_SHFT     , .douji = B_Z             , .kana = "ho"     , .func = nofunc }, // ほ
  {.shift = B_SHFT     , .douji = B_F             , .kana = "ma"     , .func = nofunc }, // ま
  {.shift = B_SHFT     , .douji = B_S             , .kana = "mi"     , .func = nofunc }, // み
  {.shift = B_SHFT     , .douji = B_COMM          , .kana = "mu"     , .func = nofunc }, // む
  {.shift = B_SHFT     , .douji = B_W             , .kana = "me"     , .func = nofunc }, // め
  {.shift = B_SHFT     , .douji = B_K             , .kana = "mo"     , .func = nofunc }, // も
  {.shift = B_SHFT     , .douji = B_H             , .kana = "ya"     , .func = nofunc }, // や
  {.shift = B_SHFT     , .douji = B_P             , .kana = "yu"     , .func = nofunc }, // ゆ
  {.shift = B_SHFT     , .douji = B_I             , .kana = "yo"     , .func = nofunc }, // よ
  {.shift = 0UL        , .douji = B_DOT           , .kana = "ra"     , .func = nofunc }, // ら
  {.shift = B_SHFT     , .douji = B_E             , .kana = "ri"     , .func = nofunc }, // り
  {.shift = 0UL        , .douji = B_I             , .kana = "ru"     , .func = nofunc }, // る
  {.shift = 0UL        , .douji = B_SLSH          , .kana = "re"     , .func = nofunc }, // れ
  {.shift = B_SHFT     , .douji = B_SLSH          , .kana = "re"     , .func = nofunc }, // れ
  {.shift = 0UL        , .douji = B_A             , .kana = "ro"     , .func = nofunc }, // ろ
  {.shift = B_SHFT     , .douji = B_DOT           , .kana = "wa"     , .func = nofunc }, // わ
  {.shift = B_SHFT     , .douji = B_C             , .kana = "wo"     , .func = nofunc }, // を
  {.shift = 0UL        , .douji = B_COMM          , .kana = "nn"     , .func = nofunc }, // ん
  {.shift = 0UL        , .douji = B_SCLN          , .kana = "-"      , .func = nofunc }, // ー

  // 濁音
  {.shift = 0UL        , .douji = B_J|B_F         , .kana = "ga"     , .func = nofunc }, // が
  {.shift = 0UL        , .douji = B_J|B_W         , .kana = "gi"     , .func = nofunc }, // ぎ
  {.shift = 0UL        , .douji = B_F|B_H         , .kana = "gu"     , .func = nofunc }, // ぐ
  {.shift = 0UL        , .douji = B_J|B_S         , .kana = "ge"     , .func = nofunc }, // げ
  {.shift = 0UL        , .douji = B_J|B_V         , .kana = "go"     , .func = nofunc }, // ご
  {.shift = 0UL        , .douji = B_F|B_U         , .kana = "za"     , .func = nofunc }, // ざ
  {.shift = 0UL        , .douji = B_J|B_R         , .kana = "zi"     , .func = nofunc }, // じ
  {.shift = 0UL        , .douji = B_F|B_O         , .kana = "zu"     , .func = nofunc }, // ず
  {.shift = 0UL        , .douji = B_J|B_A         , .kana = "ze"     , .func = nofunc }, // ぜ
  {.shift = 0UL        , .douji = B_J|B_B         , .kana = "zo"     , .func = nofunc }, // ぞ
  {.shift = 0UL        , .douji = B_F|B_N         , .kana = "da"     , .func = nofunc }, // だ
  {.shift = 0UL        , .douji = B_J|B_G         , .kana = "di"     , .func = nofunc }, // ぢ
  {.shift = 0UL        , .douji = B_F|B_L         , .kana = "du"     , .func = nofunc }, // づ
  {.shift = 0UL        , .douji = B_J|B_E         , .kana = "de"     , .func = nofunc }, // で
  {.shift = 0UL        , .douji = B_J|B_D         , .kana = "do"     , .func = nofunc }, // ど
  {.shift = 0UL        , .douji = B_J|B_C         , .kana = "ba"     , .func = nofunc }, // ば
  {.shift = 0UL        , .douji = B_J|B_X         , .kana = "bi"     , .func = nofunc }, // び
  {.shift = 0UL        , .douji = B_F|B_SCLN      , .kana = "bu"     , .func = nofunc }, // ぶ
  {.shift = 0UL        , .douji = B_F|B_P         , .kana = "be"     , .func = nofunc }, // べ
  {.shift = 0UL        , .douji = B_J|B_Z         , .kana = "bo"     , .func = nofunc }, // ぼ
  {.shift = 0UL        , .douji = B_F|B_L|B_SCLN  , .kana = "vu"     , .func = nofunc }, // ゔ

  // 半濁音
  {.shift = 0UL        , .douji = B_M|B_C         , .kana = "pa"     , .func = nofunc }, // ぱ
  {.shift = 0UL        , .douji = B_M|B_X         , .kana = "pi"     , .func = nofunc }, // ぴ
  {.shift = 0UL        , .douji = B_V|B_SCLN      , .kana = "pu"     , .func = nofunc }, // ぷ
  {.shift = 0UL        , .douji = B_V|B_P         , .kana = "pe"     , .func = nofunc }, // ぺ
  {.shift = 0UL        , .douji = B_M|B_Z         , .kana = "po"     , .func = nofunc }, // ぽ

  // 小書き
  {.shift = 0UL        , .douji = B_Q|B_H         , .kana = "xya"    , .func = nofunc }, // ゃ
  {.shift = 0UL        , .douji = B_Q|B_P         , .kana = "xyu"    , .func = nofunc }, // ゅ
  {.shift = 0UL        , .douji = B_Q|B_I         , .kana = "xyo"    , .func = nofunc }, // ょ
  {.shift = 0UL        , .douji = B_Q|B_J         , .kana = "xa"     , .func = nofunc }, // ぁ
  {.shift = 0UL        , .douji = B_Q|B_K         , .kana = "xi"     , .func = nofunc }, // ぃ
  {.shift = 0UL        , .douji = B_Q|B_L         , .kana = "xu"     , .func = nofunc }, // ぅ
  {.shift = 0UL        , .douji = B_Q|B_O         , .kana = "xe"     , .func = nofunc }, // ぇ
  {.shift = 0UL        , .douji = B_Q|B_N         , .kana = "xo"     , .func = nofunc }, // ぉ
  {.shift = 0UL        , .douji = B_Q|B_DOT       , .kana = "xwa"    , .func = nofunc }, // ゎ
  {.shift = 0UL        , .douji = B_G             , .kana = "xtu"    , .func = nofunc }, // っ
  {.shift = 0UL        , .douji = B_Q|B_S         , .kana = "xke"    , .func = nofunc }, // ヶ
  {.shift = 0UL        , .douji = B_Q|B_F         , .kana = "xka"    , .func = nofunc }, // ヵ

  // 清音拗音 濁音拗音 半濁拗音
  {.shift = 0UL        , .douji = B_R|B_H         , .kana = "sya"    , .func = nofunc }, // しゃ
  {.shift = 0UL        , .douji = B_R|B_P         , .kana = "syu"    , .func = nofunc }, // しゅ
  {.shift = 0UL        , .douji = B_R|B_I         , .kana = "syo"    , .func = nofunc }, // しょ
  {.shift = 0UL        , .douji = B_J|B_R|B_H     , .kana = "zya"    , .func = nofunc }, // じゃ
  {.shift = 0UL        , .douji = B_J|B_R|B_P     , .kana = "zyu"    , .func = nofunc }, // じゅ
  {.shift = 0UL        , .douji = B_J|B_R|B_I     , .kana = "zyo"    , .func = nofunc }, // じょ
  {.shift = 0UL        , .douji = B_W|B_H         , .kana = "kya"    , .func = nofunc }, // きゃ
  {.shift = 0UL        , .douji = B_W|B_P         , .kana = "kyu"    , .func = nofunc }, // きゅ
  {.shift = 0UL        , .douji = B_W|B_I         , .kana = "kyo"    , .func = nofunc }, // きょ
  {.shift = 0UL        , .douji = B_J|B_W|B_H     , .kana = "gya"    , .func = nofunc }, // ぎゃ
  {.shift = 0UL        , .douji = B_J|B_W|B_P     , .kana = "gyu"    , .func = nofunc }, // ぎゅ
  {.shift = 0UL        , .douji = B_J|B_W|B_I     , .kana = "gyo"    , .func = nofunc }, // ぎょ
  {.shift = 0UL        , .douji = B_G|B_H         , .kana = "tya"    , .func = nofunc }, // ちゃ
  {.shift = 0UL        , .douji = B_G|B_P         , .kana = "tyu"    , .func = nofunc }, // ちゅ
  {.shift = 0UL        , .douji = B_G|B_I         , .kana = "tyo"    , .func = nofunc }, // ちょ
  {.shift = 0UL        , .douji = B_J|B_G|B_H     , .kana = "dya"    , .func = nofunc }, // ぢゃ
  {.shift = 0UL        , .douji = B_J|B_G|B_P     , .kana = "dyu"    , .func = nofunc }, // ぢゅ
  {.shift = 0UL        , .douji = B_J|B_G|B_I     , .kana = "dyo"    , .func = nofunc }, // ぢょ
  {.shift = 0UL        , .douji = B_D|B_H         , .kana = "nya"    , .func = nofunc }, // にゃ
  {.shift = 0UL        , .douji = B_D|B_P         , .kana = "nyu"    , .func = nofunc }, // にゅ
  {.shift = 0UL        , .douji = B_D|B_I         , .kana = "nyo"    , .func = nofunc }, // にょ
  {.shift = 0UL        , .douji = B_X|B_H         , .kana = "hya"    , .func = nofunc }, // ひゃ
  {.shift = 0UL        , .douji = B_X|B_P         , .kana = "hyu"    , .func = nofunc }, // ひゅ
  {.shift = 0UL        , .douji = B_X|B_I         , .kana = "hyo"    , .func = nofunc }, // ひょ
  {.shift = 0UL        , .douji = B_J|B_X|B_H     , .kana = "bya"    , .func = nofunc }, // びゃ
  {.shift = 0UL        , .douji = B_J|B_X|B_P     , .kana = "byu"    , .func = nofunc }, // びゅ
  {.shift = 0UL        , .douji = B_J|B_X|B_I     , .kana = "byo"    , .func = nofunc }, // びょ
  {.shift = 0UL        , .douji = B_M|B_X|B_H     , .kana = "pya"    , .func = nofunc }, // ぴゃ
  {.shift = 0UL        , .douji = B_M|B_X|B_P     , .kana = "pyu"    , .func = nofunc }, // ぴゅ
  {.shift = 0UL        , .douji = B_M|B_X|B_I     , .kana = "pyo"    , .func = nofunc }, // ぴょ
  {.shift = 0UL        , .douji = B_S|B_H         , .kana = "mya"    , .func = nofunc }, // みゃ
  {.shift = 0UL        , .douji = B_S|B_P         , .kana = "myu"    , .func = nofunc }, // みゅ
  {.shift = 0UL        , .douji = B_S|B_I         , .kana = "myo"    , .func = nofunc }, // みょ
  {.shift = 0UL        , .douji = B_E|B_H         , .kana = "rya"    , .func = nofunc }, // りゃ
  {.shift = 0UL        , .douji = B_E|B_P         , .kana = "ryu"    , .func = nofunc }, // りゅ
  {.shift = 0UL        , .douji = B_E|B_I         , .kana = "ryo"    , .func = nofunc }, // りょ

  // 清音外来音 濁音外来音
  {.shift = 0UL        , .douji = B_M|B_E|B_K     , .kana = "thi"    , .func = nofunc }, // てぃ
  {.shift = 0UL        , .douji = B_M|B_E|B_P     , .kana = "texyu"  , .func = nofunc }, // てゅ
  {.shift = 0UL        , .douji = B_J|B_E|B_K     , .kana = "dhi"    , .func = nofunc }, // でぃ
  {.shift = 0UL        , .douji = B_J|B_E|B_P     , .kana = "dhu"    , .func = nofunc }, // でゅ
  {.shift = 0UL        , .douji = B_M|B_D|B_L     , .kana = "toxu"   , .func = nofunc }, // とぅ
  {.shift = 0UL        , .douji = B_J|B_D|B_L     , .kana = "doxu"   , .func = nofunc }, // どぅ
  {.shift = 0UL        , .douji = B_M|B_R|B_O     , .kana = "sye"    , .func = nofunc }, // しぇ
  {.shift = 0UL        , .douji = B_M|B_G|B_O     , .kana = "tye"    , .func = nofunc }, // ちぇ
  {.shift = 0UL        , .douji = B_J|B_R|B_O     , .kana = "zye"    , .func = nofunc }, // じぇ
  {.shift = 0UL        , .douji = B_J|B_G|B_O     , .kana = "dye"    , .func = nofunc }, // ぢぇ
  {.shift = 0UL        , .douji = B_V|B_SCLN|B_J  , .kana = "fa"     , .func = nofunc }, // ふぁ
  {.shift = 0UL        , .douji = B_V|B_SCLN|B_K  , .kana = "fi"     , .func = nofunc }, // ふぃ
  {.shift = 0UL        , .douji = B_V|B_SCLN|B_O  , .kana = "fe"     , .func = nofunc }, // ふぇ
  {.shift = 0UL        , .douji = B_V|B_SCLN|B_N  , .kana = "fo"     , .func = nofunc }, // ふぉ
  {.shift = 0UL        , .douji = B_V|B_SCLN|B_P  , .kana = "fyu"    , .func = nofunc }, // ふゅ
  {.shift = 0UL        , .douji = B_V|B_K|B_O     , .kana = "ixe"    , .func = nofunc }, // いぇ
  {.shift = 0UL        , .douji = B_V|B_L|B_K     , .kana = "wi"     , .func = nofunc }, // うぃ
  {.shift = 0UL        , .douji = B_V|B_L|B_O     , .kana = "we"     , .func = nofunc }, // うぇ
  {.shift = 0UL        , .douji = B_V|B_L|B_N     , .kana = "uxo"    , .func = nofunc }, // うぉ
  {.shift = 0UL        , .douji = B_F|B_L|B_J     , .kana = "va"     , .func = nofunc }, // ゔぁ
  {.shift = 0UL        , .douji = B_F|B_L|B_K     , .kana = "vi"     , .func = nofunc }, // ゔぃ
  {.shift = 0UL        , .douji = B_F|B_L|B_O     , .kana = "ve"     , .func = nofunc }, // ゔぇ
  {.shift = 0UL        , .douji = B_F|B_L|B_N     , .kana = "vo"     , .func = nofunc }, // ゔぉ
  {.shift = 0UL        , .douji = B_F|B_L|B_P     , .kana = "vuxyu"  , .func = nofunc }, // ゔゅ
  {.shift = 0UL        , .douji = B_V|B_H|B_J     , .kana = "kuxa"   , .func = nofunc }, // くぁ
  {.shift = 0UL        , .douji = B_V|B_H|B_K     , .kana = "kuxi"   , .func = nofunc }, // くぃ
  {.shift = 0UL        , .douji = B_V|B_H|B_O     , .kana = "kuxe"   , .func = nofunc }, // くぇ
  {.shift = 0UL        , .douji = B_V|B_H|B_N     , .kana = "kuxo"   , .func = nofunc }, // くぉ
  {.shift = 0UL        , .douji = B_V|B_H|B_DOT   , .kana = "kuxwa"  , .func = nofunc }, // くゎ
  {.shift = 0UL        , .douji = B_F|B_H|B_J     , .kana = "guxa"   , .func = nofunc }, // ぐぁ
  {.shift = 0UL        , .douji = B_F|B_H|B_K     , .kana = "guxi"   , .func = nofunc }, // ぐぃ
  {.shift = 0UL        , .douji = B_F|B_H|B_O     , .kana = "guxe"   , .func = nofunc }, // ぐぇ
  {.shift = 0UL        , .douji = B_F|B_H|B_N     , .kana = "guxo"   , .func = nofunc }, // ぐぉ
  {.shift = 0UL        , .douji = B_F|B_H|B_DOT   , .kana = "guxwa"  , .func = nofunc }, // ぐゎ
  {.shift = 0UL        , .douji = B_V|B_L|B_J     , .kana = "tsa"    , .func = nofunc }, // つぁ

  // 追加
  {.shift = 0UL        , .douji = B_SHFT          , .kana = " "      , .func = nofunc },
  {.shift = 0UL        , .douji = B_Q             , .kana = ""       , .func = nofunc },
  {.shift = B_SHFT     , .douji = B_V             , .kana = ","SS_TAP(X_ENTER)  , .func = nofunc },
  {.shift = B_SHFT     , .douji = B_M             , .kana = "."SS_TAP(X_ENTER)  , .func = nofunc },
  {.shift = 0UL        , .douji = B_U             , .kana = SS_TAP(X_BACKSPACE) , .func = nofunc },

  {.shift = 0UL        , .douji = B_V|B_M         , .kana = SS_TAP(X_ENTER)  , .func = nofunc }, // enter
  // {.shift = B_SHFT     , .douji = B_V|B_M         , .kana = SS_TAP(X_ENTER)  , .func = nofunc }, // enter+シフト(連続シフト)
  // 冗長シフトのせいで句読点の確定が遅れる。

  {.shift = 0UL        , .douji = B_T             , .kana = ""       , .func = ng_T }, //
  {.shift = 0UL        , .douji = B_Y             , .kana = ""       , .func = ng_Y }, //
  {.shift = B_SHFT     , .douji = B_T             , .kana = ""       , .func = ng_ST }, //
  {.shift = B_SHFT     , .douji = B_Y             , .kana = ""       , .func = ng_SY }, //
  {.shift = 0UL        , .douji = B_H|B_J         , .kana = ""       , .func = naginata_on }, //　かなオン
  {.shift = 0UL        , .douji = B_F|B_G         , .kana = ""       , .func = naginata_off }, //　かなオフ

};

// 薙刀式のレイヤー、オンオフするキー
void set_naginata(uint8_t layer, uint16_t *onk, uint16_t *offk) {
  naginata_layer = layer;
  ngon_keys[0] = *onk;
  ngon_keys[1] = *(onk+1);
  ngoff_keys[0] = *offk;
  ngoff_keys[1] = *(offk+1);

  initializeListArray(&nginput);

  naginata_config.raw = eeconfig_read_user();
  if (naginata_config.os != NG_WIN && naginata_config.os != NG_MAC && naginata_config.os != NG_LINUX) {
    naginata_config.os = NG_WIN;
    naginata_config.tategaki = 1;
    eeconfig_update_user(naginata_config.raw);
  }
  ng_set_unicode_mode(naginata_config.os);
}

// 薙刀式をオン
void naginata_on(void) {
  is_naginata = true;
  naginata_clear();
  layer_on(naginata_layer);

  tap_code(KC_LANGUAGE_1); // Mac
  tap_code(KC_INTERNATIONAL_4); // Win
}

// 薙刀式をオフ
void naginata_off(void) {
  is_naginata = false;
  naginata_clear();
  layer_off(naginata_layer);

  tap_code(KC_LANGUAGE_2); // Mac
  tap_code(KC_INTERNATIONAL_5); // Win
}

// 薙刀式のon/off状態を返す
bool naginata_state(void) {
  return is_naginata;
}

void switchOS(uint8_t os) {
  naginata_config.os = os;
  eeconfig_update_user(naginata_config.raw);
  ng_set_unicode_mode(naginata_config.os);
}

void ng_set_unicode_mode(uint8_t os) {
  switch (os) {
    case NG_WIN:
      set_unicode_input_mode(UNICODE_MODE_WINCOMPOSE);
      break;
    case NG_MAC:
      set_unicode_input_mode(UNICODE_MODE_MACOS);
      break;
    case NG_LINUX:
      set_unicode_input_mode(UNICODE_MODE_LINUX);
      break;
  }
}

void tategaki_toggle() {
  naginata_config.tategaki ^= 1;
  eeconfig_update_user(naginata_config.raw);
}

void ng_show_os(void) {
  switch (naginata_config.os) {
    case NG_WIN:
      send_string("win");
      break;
    case NG_MAC:
      send_string("mac");
      break;
    case NG_LINUX:
      send_string("linux");
      break;
  }
  if (naginata_config.tategaki) {
    send_string("/tate");
  } else {
    send_string("/yoko");
  }
}

#define MAX_STRLEN 40
void ng_send_unicode_string_P(const char *pstr) {
  if (strlen_P(pstr) > MAX_STRLEN) return;
  char str[MAX_STRLEN];
  strcpy_P(str, pstr);

  switch (naginata_config.os) {
    case NG_LINUX:
      tap_code(KC_INTERNATIONAL_5);
      send_unicode_string(str);
      tap_code(KC_INTERNATIONAL_4);
      break;
    case NG_WIN:
      send_unicode_string(str);
      tap_code(KC_ENT);
      break;
    case NG_MAC: // Karabiner-Elementsが必要
      tap_code(KC_LANGUAGE_2); // 未確定文字を確定する
      wait_ms(50);
      register_code(KC_LCTL); // Unicode HEX Inputへ切り替え
      wait_ms(50);
      tap_code(KC_F20);
      wait_ms(50);
      unregister_code(KC_LCTL);
      wait_ms(50);
      send_unicode_string(str);
      wait_ms(50);
      register_code(KC_LSFT); // 日本語入力へ切り替え。再変換にならないように「shift+かな」「かな」の2打にする。
      wait_ms(50);
      tap_code(KC_LANGUAGE_1);
      wait_ms(50);
      unregister_code(KC_LSFT);
      wait_ms(50);
      tap_code(KC_LANGUAGE_1);
      break;
  }
}

// modifierが押されたら薙刀式レイヤーをオフしてベースレイヤーに戻す
// get_mods()がうまく動かない
static int n_modifier = 0;

bool process_modifier(uint16_t keycode, keyrecord_t *record) {
  if (IS_MODIFIER_KEYCODE(keycode) || IS_QK_MOD_TAP(keycode)) {
    if (record->event.pressed) {
      n_modifier++;
      layer_off(naginata_layer);
    } else {
      n_modifier--;
      if (n_modifier <= 0) {
        n_modifier = 0;
        layer_on(naginata_layer);
      }
    }
    return true;
  }
  return false;
}

static uint16_t fghj_buf = 0; // 押しているJかKのキーコード
static uint8_t nkeypress = 0; // 同時にキーを押している数

// 薙刀式の起動処理(容量が大きいCOMBOを使わない)
bool enable_naginata(uint16_t keycode, keyrecord_t *record) {
  // キープレス
  if (record->event.pressed) {
    nkeypress++;
    // 1キー目、JKの前に他のキーを押していないこと
    if (fghj_buf == 0 && nkeypress == 1) {
      // かなオンキーの場合
      if (keycode == ngon_keys[0] || keycode == ngon_keys[1] || keycode == ngoff_keys[0] || keycode == ngoff_keys[1]) {
        fghj_buf = keycode;
        return false;
      }
    // ２キー目
    } else {
      // ２キー目、１キー目、両方ともかなオンキー
      if ((keycode == ngon_keys[0] && fghj_buf == ngon_keys[1]) ||
          (keycode == ngon_keys[1] && fghj_buf == ngon_keys[0])) {
        naginata_on();
        fghj_buf = 0;
        nkeypress = 0;
        return false;
      } else if ((keycode == ngoff_keys[0] && fghj_buf == ngoff_keys[1]) ||
          (keycode == ngoff_keys[1] && fghj_buf == ngoff_keys[0])) {
        naginata_off();
        fghj_buf = 0;
        nkeypress = 0;
        return false;
      // ２キー目はかなオンキーではない
      } else {
        tap_code(fghj_buf); // 1キー目を出力
        fghj_buf = 0;
        nkeypress = 0;
        return true; // 2キー目はQMKにまかせる
      }
    }
  } else {
    nkeypress = 0;
    // J/K単押しだった
    if (fghj_buf > 0) {
      tap_code(fghj_buf);
      fghj_buf = 0;

      // Shift + Jで、先にShiftを外した場合にShiftがリリースされない不具合対策
      if (IS_MODIFIER_KEYCODE(keycode)) {
        unregister_code(keycode);
      } else if (IS_QK_MOD_TAP(keycode)) {
        if (keycode & (MOD_LCTL << 8))
          unregister_code(KC_LEFT_CTRL);
        if (keycode & (MOD_LSFT << 8))
          unregister_code(KC_LEFT_SHIFT);
        if (keycode & (MOD_LALT << 8))
          unregister_code(KC_LEFT_ALT);
        if (keycode & (MOD_LGUI << 8))
          unregister_code(KC_LEFT_GUI);
        if (keycode & (MOD_RCTL << 8))
          unregister_code(KC_RIGHT_CTRL);
        if (keycode & (MOD_RSFT << 8))
          unregister_code(KC_RIGHT_SHIFT);
        if (keycode & (MOD_RALT << 8))
          unregister_code(KC_RIGHT_ALT);
        if (keycode & (MOD_RGUI << 8))
          unregister_code(KC_RIGHT_GUI);
      }
      return false;
    }
  }

  fghj_buf = 0;
  return true;
}

// バッファをクリアする
void naginata_clear(void) {
  initializeListArray(&nginput);
  n_modifier = 0;
  nkeypress = 0;
  fghj_buf = 0;
  pressed_keys = 0;
  n_pressed_keys = 0;
}

// 薙刀式の入力処理
bool process_naginata(uint16_t keycode, keyrecord_t *record) {

  if (record->event.pressed) {
    n_pressed_keys++;
  } else {
    if (n_pressed_keys > 0) {
      n_pressed_keys--;
    }
  }
  if (n_pressed_keys == 0)
    pressed_keys = 0;

  #if defined(CONSOLE_ENABLE)
      uprintf(">process_naginata pressed_keys=%lu, %d\n", pressed_keys, n_pressed_keys);
  #endif

  // まれに薙刀モードオンのまま、レイヤーがオフになることがあるので、対策
  if (n_modifier == 0 && is_naginata && !layer_state_is(naginata_layer))
    layer_on(naginata_layer);
  if (n_modifier == 0 && !is_naginata && layer_state_is(naginata_layer))
    layer_off(naginata_layer);
  if (n_modifier > 0 && layer_state_is(naginata_layer))
    layer_off(naginata_layer);

  // OS切り替え(UNICODE出力)
  if (record->event.pressed) {
    switch (keycode) {
      case NGSW_WIN:
        switchOS(NG_WIN);
        return false;
        break;
      case NGSW_MAC:
        switchOS(NG_MAC);
        return false;
        break;
      case NGSW_LNX:
        switchOS(NG_LINUX);
        return false;
        break;
      case NG_SHOS:
        ng_show_os();
        return false;
        break;
      case NG_TAYO:
        tategaki_toggle();
        return false;
        break;
    }
  }

  if (!is_naginata)
    // return true;
    return enable_naginata(keycode, record);

  if (process_modifier(keycode, record))
    return true;

  if (keycode >= NG_Q && keycode <= NG_SHFT2) {
    if (record->event.pressed) {
      #if defined(CONSOLE_ENABLE)
          uprintf(">process_naginata pressed=%u nginput.size=%u\n", keycode, nginput.size);
      #endif

      pressed_keys |= ng_key[keycode - NG_Q]; // キーの重ね合わせ

      if (keycode == NG_SHFT || keycode == NG_SHFT2) {
        NGList a;
        initializeList(&a);
        addToList(&a, keycode);
        addToListArray(&nginput, &a);
      } else {
        NGList a;
        NGList b;
        uint32_t bkeys = 0UL; // 結合候補bの全キービット
        if (nginput.size > 0) {
          copyList(&(nginput.elements[nginput.size - 1]), &a);
          copyList(&a, &b);
          addToList(&b, keycode);
          for (int i = 0; i < b.size; i++) {
            bkeys |= ng_key[b.elements[i] - NG_Q];
          }
        }

        // 前のキーとの同時押し: 候補があり、かつ全キーが今この瞬間 物理的に押下中のときだけ結合
        // (指を離して打てば清音、重ねて打てば濁音。すがず・ますがまずになる誤爆を防ぐ)
        // 同じキー連打を除外
        if (nginput.size > 0 && a.elements[a.size - 1] != keycode && number_of_candidates(&b) > 0
            && (bkeys & pressed_keys) == bkeys) {
          removeFromListArrayAt(&nginput, nginput.size - 1);
          addToListArray(&nginput, &b);
        // 前のキーと同時押しはない
        } else {
          // 連続シフトではない
          NGList e;
          initializeList(&e);
          addToList(&e, keycode);
          addToListArray(&nginput, &e);
        }
      }
      
      // 連続シフト
      static const uint16_t rs[10][2] = {{NG_D, NG_F}, {NG_C, NG_V}, {NG_J, NG_K}, {NG_M, NG_COMM}, {NG_SHFT, 0}, {NG_SHFT2, 0}, {NG_F, 0}, {NG_V, 0}, {NG_J, 0}, {NG_M, 0}};
      uint32_t keyset = 0UL;
      for (int i = 0; i < nginput.elements[0].size; i++) {
        keyset |= ng_key[nginput.elements[0].elements[i] - NG_Q];
      }
      for (int i = 0; i < 10; i++) {
        NGList rskc;
        initializeList(&rskc);
        addToList(&rskc, rs[i][0]);
        if (rs[i][1] > 0) {
          addToList(&rskc, rs[i][1]);
        }

        int c = includeList(&rskc, keycode);
        uint32_t brs = 0UL;
        for (int j = 0; j < rskc.size; j++) {
          brs |=  ng_key[rskc.elements[j] - NG_Q];
        }

        NGList l = nginput.elements[nginput.size - 1];
        for (int j = 0; j < l.size; j++) {
          addToList(&rskc, l.elements[j]);
        }

        if (c <  0 && ((brs & pressed_keys) == brs) &&  (keyset & brs) != brs && number_of_matches(&rskc) >  0) {
          nginput.elements[nginput.size - 1] = rskc;
          break;
        }
      }

      if (nginput.size > 1 || number_of_candidates(&(nginput.elements[0])) == 1) {
        ng_type(&(nginput.elements[0]));
        removeFromListArrayAt(&nginput, 0);
      }

      #if defined(CONSOLE_ENABLE)
        uprintf("<process_naginata pressed=%u nginput.size=%u\n", keycode, nginput.size);
      #endif

    } else { // key release
      #if defined(CONSOLE_ENABLE)
          uprintf(">process_naginata released=%u nginput.size=%u\n", keycode, nginput.size);
      #endif
      pressed_keys &= ~ng_key[keycode - NG_Q]; // キーの重ね合わせ

      if (pressed_keys == 0UL) {
        while (nginput.size > 0) {
          ng_type(&(nginput.elements[0]));
          removeFromListArrayAt(&nginput, 0);
        }
      } else {
        if (nginput.size > 0 && number_of_candidates(&(nginput.elements[0])) == 1) {
          ng_type(&(nginput.elements[0]));
          removeFromListArrayAt(&nginput, 0);
        }
      }

      #if defined(CONSOLE_ENABLE)
          uprintf("<process_naginata released=%u nginput.size=%u\n", keycode, nginput.size);
      #endif
    }
    return false;
  }

  return true;
}

// UTF-8マルチバイト(日本語記号など)を含むか。ASCII/SS_TAP制御コードは全て<0x80
static bool ng_has_multibyte(const char *s) {
  for (; *s; s++) {
    if ((uint8_t)*s >= 0x80) return true;
  }
  return false;
}

// キー入力を文字に変換して出力する
void ng_type(NGList *keys) {
  #if defined(CONSOLE_ENABLE)
    uprintf(">ng_type size=%u\n", keys->size);
  #endif
  if (keys->size == 0) return;

  naginata_kanamap bngdickana;

  if (keys->size == 1 && keys->elements[0] == NG_SHFT2) {
    tap_code16(KC_ENT);
    return;
  }

  bool ftype = false;
  uint32_t keyset = 0UL;
  for (int i = 0; i < keys->size; i++) {
    keyset |= ng_key[keys->elements[i] - NG_Q];
  }
  for (int i = 0; i < sizeof ngdickana / sizeof bngdickana; i++) {
    memcpy_P(&bngdickana, &ngdickana[i], sizeof(bngdickana));
    if ((bngdickana.shift | bngdickana.douji) == keyset) {
      if (bngdickana.kana[0] != '\0') {
        if (ng_has_multibyte(bngdickana.kana)) {
          ng_send_unicode_string_P(bngdickana.kana);
        } else {
          send_string(bngdickana.kana);
        }
      } else {
        bngdickana.func();
      }
      ftype = true;
      break;
    }
  }
  // JIみたいにJIを含む同時押しはたくさんあるが、JIのみの同時押しがないとき
  // 最後の１キーを別に分けて変換する
  if (!ftype) {
    NGList a, b;
    initializeList(&a);
    initializeList(&b);
    for (int i = 0; i < keys->size - 1; i++) {
      addToList(&a, keys->elements[i]);
    }
    addToList(&b, keys->elements[keys->size - 1]);
    ng_type(&a);
    ng_type(&b);
  }

  #if defined(CONSOLE_ENABLE)
    uprintf("<ng_type\n");
  #endif
}

// Helper function for counting matches/candidates
static int count_kana_entries(NGList *keys, bool exact_match) {
  if (keys->size == 0) return 0;

  naginata_kanamap bngdickana;
  int count = 0;
  uint32_t keyset0 = 0UL, keyset1 = 0UL, keyset2 = 0UL;
  
  // keysetを配列にしたらバイナリサイズが増えた
  switch (keys->size) {
    case 1:
      keyset0 = ng_key[keys->elements[0] - NG_Q];
      break;
    case 2:
      keyset0 = ng_key[keys->elements[0] - NG_Q];
      keyset1 = ng_key[keys->elements[1] - NG_Q];
      break;
    default:
      keyset0 = ng_key[keys->elements[0] - NG_Q];
      keyset1 = ng_key[keys->elements[1] - NG_Q];
      keyset2 = ng_key[keys->elements[2] - NG_Q];
      break;
  }

  for (int i = 0; i < sizeof ngdickana / sizeof bngdickana; i++) {
    memcpy_P(&bngdickana, &ngdickana[i], sizeof(bngdickana));
    bool matches = false;

    switch (keys->size) {
      case 1:
        if (exact_match) {
          matches = (bngdickana.shift == keyset0) || 
                   (bngdickana.shift == 0UL && bngdickana.douji == keyset0);
        } else {
          matches = ((bngdickana.shift & keyset0) == keyset0) ||
                   (bngdickana.shift == 0UL && (bngdickana.douji & keyset0) == keyset0);
        }
        break;
      case 2:
        if (exact_match) {
          matches = (bngdickana.shift == (keyset0 | keyset1)) ||
                   (bngdickana.shift == keyset0 && bngdickana.douji == keyset1) ||
                   (bngdickana.shift == 0UL && bngdickana.douji == (keyset0 | keyset1));
        } else {
          matches = (bngdickana.shift == (keyset0 | keyset1)) ||
                   (bngdickana.shift == keyset0 && (bngdickana.douji & keyset1) == keyset1) ||
                   (bngdickana.shift == 0UL && (bngdickana.douji & (keyset0 | keyset1)) == (keyset0 | keyset1));
          // しぇ、ちぇ、など2キーで確定してはいけない
          if (matches && (bngdickana.shift | bngdickana.douji) != (keyset0 | keyset1)) {
            count = 2;
          }
        }
        break;
      default:
        if (exact_match) {
          matches = (bngdickana.shift == (keyset0 | keyset1) && bngdickana.douji == keyset2) ||
                   (bngdickana.shift == keyset0 && bngdickana.douji == (keyset1 | keyset2)) ||
                   (bngdickana.shift == 0UL && bngdickana.douji == (keyset0 | keyset1 | keyset2));
        } else {
          matches = (bngdickana.shift == (keyset0 | keyset1) && (bngdickana.douji & keyset2) == keyset2) ||
                   (bngdickana.shift == keyset0 && (bngdickana.douji & (keyset1 | keyset2)) == (keyset1 | keyset2)) ||
                   (bngdickana.shift == 0UL && (bngdickana.douji & (keyset0 | keyset1 | keyset2)) == (keyset0 | keyset1 | keyset2));
        }
        break;
    }

    if (matches) {
      count++;
      if (count > 1) break;
    }
  }

  return count;
}

int number_of_matches(NGList *keys) {
  #if defined(CONSOLE_ENABLE)
    uprintf(">number_of_matches\n");
  #endif
  
  int result = count_kana_entries(keys, true);
  
  #if defined(CONSOLE_ENABLE)
    uprintf("<number_of_matches nom=%u\n", result);
  #endif
  return result;
}

int number_of_candidates(NGList *keys) {
  #if defined(CONSOLE_ENABLE)
    uprintf(">number_of_candidates\n");
  #endif
  
  int result = count_kana_entries(keys, false);
  
  #if defined(CONSOLE_ENABLE)
    uprintf("<number_of_candidates noc=%u\n", result);
  #endif
  return result;
}

void nofunc() {}

void ng_T() {
  ng_left(1);
}

void ng_Y() {
  ng_right(1);
}

void ng_ST() {
  register_code(KC_LSFT);
  ng_left(1);
  unregister_code(KC_LSFT);
}

void ng_SY() {
  register_code(KC_LSFT);
  ng_right(1);
  unregister_code(KC_LSFT);
}

void ng_left(uint8_t c) {
  for (uint8_t i = 0; i < c; i++) {
    tap_code(KC_LEFT);
  }
}

void ng_right(uint8_t c) {
  for (uint8_t i = 0; i < c; i++) {
    tap_code(KC_RIGHT);
  }
}