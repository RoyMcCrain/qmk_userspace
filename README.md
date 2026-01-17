# QMK Userspace

薙刀式v17を使用したキーマップ

## 対応キーボード

- planck/rev6
- planck/rev7
- zsa/planck_ez/base
- boardsource/technik_o
- salicylic_acid3/goforty_ortho

## ビルド

```bash
qmk config user.overlay_dir="$(realpath .)"
qmk compile -kb planck/rev6 -km roy
```

## 書き込み方法

ファームウェアは [Releases](../../releases) からダウンロードできます。

### planck/rev6, rev7, zsa/planck_ez (STM32 DFU)

1. QMK Toolboxを起動
2. MCUで `STM32F303` を選択
3. `.bin` ファイルを選択
4. キーボードをリセット（RESETキー or 裏面のリセットボタン）
5. `STM32 DFU device connected` と表示されたら `Flash` をクリック

### boardsource/technik_o (ATmega32u4)

1. QMK Toolboxを起動
2. MCUで `ATmega32u4` を選択
3. `.hex` ファイルを選択
4. キーボードをリセット
5. `Caterina device connected` と表示されたら `Flash` をクリック

### salicylic_acid3/goforty_ortho (RP2040)

QMK本体に存在しないため、CIで独自にビルドしています。
ファームウェアは [GitHub Actions](../../actions) の Artifacts からダウンロードできます。

1. BOOTSELボタンを押しながらUSB接続
2. `RPI-RP2` USBドライブが表示される
3. `.uf2` ファイルをドラッグ&ドロップ

#### ローカルでビルドする場合

シンボリックリンクを作成してビルドします。

```bash
# 初回のみ
mkdir -p ~/qmk_firmware/keyboards/salicylic_acid3
ln -s ~/ghq/github.com/RoyMcCrain/qmk_userspace/keyboards/salicylic_acid3/goforty_ortho ~/qmk_firmware/keyboards/salicylic_acid3/goforty_ortho

# ビルド
qmk compile -kb salicylic_acid3/goforty_ortho -km roy
```

## 謝辞

- [大岡俊彦氏](http://oookaworks.seesaa.net/) - 薙刀式の考案者
- [eswai](https://github.com/eswai) - QMK薙刀式の実装
