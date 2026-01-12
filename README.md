# QMK Userspace

薙刀式v17を使用したキーマップ

## 対応キーボード

- planck/rev6
- planck/rev7
- zsa/planck_ez/base
- boardsource/technik_o

## ビルド

```bash
qmk config user.overlay_dir="$(realpath .)"
qmk compile -kb planck/rev6 -km roy
```

## 謝辞

- [大岡俊彦氏](http://oookaworks.seesaa.net/) - 薙刀式の考案者
- [eswai](https://github.com/eswai) - QMK薙刀式の実装
