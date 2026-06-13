#pragma once

// 薙刀式のかな出力(send_string)の取りこぼし対策。
// QMKのsend_stringはTAP_CODE_DELAYをキー保持時間と文字間隔の両方に使うため、
// 0(デフォルト)だとエクスプローラー検索窓など処理の重い入力欄でかなが欠ける。
#define TAP_CODE_DELAY 5
