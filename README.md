# QMK Userspace

薙刀式v17を使用したキーマップ

## 対応キーボード

- planck/rev6
- planck/rev7
- zsa/planck_ez/base
- boardsource/technik_o
- salicylic_acid3/goforty_ortho (ローカルビルドのみ)

## ビルド

```bash
qmk config user.overlay_dir="$(realpath .)"
qmk compile -kb planck/rev6 -km roy
```

### salicylic_acid3/goforty_ortho (ローカルビルド)

QMK本体に存在しないため、シンボリックリンクを作成してビルドします。

```bash
# 初回のみ
mkdir -p ~/qmk_firmware/keyboards/salicylic_acid3
ln -s ~/ghq/github.com/RoyMcCrain/qmk_userspace/keyboards/salicylic_acid3/goforty_ortho ~/qmk_firmware/keyboards/salicylic_acid3/goforty_ortho

# ビルド
qmk compile -kb salicylic_acid3/goforty_ortho -km roy
```

#### 書き込み方法 (RP2040)

1. BOOTSELボタンを押しながらUSB接続
2. `RPI-RP2` USBドライブが表示される
3. `salicylic_acid3_goforty_ortho_roy.uf2` をドラッグ&ドロップ

## 謝辞

- [大岡俊彦氏](http://oookaworks.seesaa.net/) - 薙刀式の考案者
- [eswai](https://github.com/eswai) - QMK薙刀式の実装
