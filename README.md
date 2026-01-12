# QMK Userspace - 薙刀式実装

このリポジトリは、カスタムメカニカルキーボード用の薙刀式日本語入力メソッドの[QMK Userspace](https://docs.qmk.fm/#/feature_userspace)実装を含んでいます。

## 概要

薙刀式はメカニカルキーボード向けに設計された効率的な日本語入力方式です。このuserspaceでは複数のバージョンを提供しています：

- **v15y** - 早期確定機能を含む現在の安定版（推奨版）
- **v16** - 高速モード対応の開発版
- **v15** - 旧安定版

## 対応キーボード

[eswai](https://github.com/eswai)製の各種カスタムキーボードに対応しています：

- `eswai/742` - OLED搭載分割キーボード
- `eswai/x60` - 60%キーボードレイアウト
- `eswai/vpico` - RP2040ベースキーボード
- `eswai/kunai16` - 16キーマクロパッド
- その他のキーボードは `keyboards/eswai/` に収録

## はじめに

### 1. QMKのセットアップ

まず、QMKファームウェアをセットアップします：

```bash
# QMK CLIのインストール
python3 -m pip install --user qmk

# QMKセットアップの実行
qmk setup
```

### 2. フォークとクローン

1. このリポジトリをGitHubアカウントにフォーク
2. フォークをローカルにクローン：

```bash
git clone https://github.com/{あなたのユーザー名}/qmk_userspace.git
cd qmk_userspace
```

### 3. QMK Userspaceの設定

QMK設定でuserspaceを有効化：

```bash
qmk config user.overlay_dir="$(realpath .)"
```

### 4. ファームウェアのビルド

特定のキーボードとキーマップ用のファームウェアをビルド：

```bash
# eswai/742でnaginata_v15yをビルド
qmk compile -kb eswai/742 -km naginata_v15y

# またはmakeコマンドを使用
make eswai/742:naginata_v15y

# キーボードに直接書き込み
make eswai/742:naginata_v15y:flash
```

## 薙刀式実装の詳細

### ディレクトリ構造

```
.
├── keyboards/eswai/       # キーボード固有の設定
│   ├── 742/
│   ├── x60/
│   └── ...
└── users/                 # 共有薙刀式実装
    ├── naginata_v15y/     # 早期確定付き安定版
    ├── naginata_v16/      # 開発版
    └── naginata_v15/      # 旧版
```

### 主な機能

- **日本語入力**: Unicodeによる完全なひらがな・カタカナサポート
- **2ストローク組み合わせ**: `twpair_on_jis`による効率的な文字入力
- **早期確定**: v15yではタイピングフローを改善する早期文字確定機能を搭載
- **OS検出**: macOS、Windows、Linux向けの自動検出と適応
- **OLEDサポート**: 対応キーボードで現在のモードとステータスを表示

### ビルド設定

薙刀式サポートに必要なビルドフラグ：

```makefile
UNICODE_ENABLE = yes    # 日本語文字出力に必須
NKRO_ENABLE = yes      # Nキーロールオーバーサポート
LTO_ENABLE = yes       # ファームウェアサイズ削減用のリンク時最適化
```

## GitHub Actionsによるビルド

このリポジトリには自動ビルド用のGitHub Actionsワークフローが含まれています：

1. フォークでGitHub Actionsを有効化
2. リポジトリに変更をプッシュ
3. Actionsタブでビルドステータスを確認
4. Releasesセクションからファームウェアをダウンロード

ビルドターゲットをカスタマイズするには、`.github/workflows/build_binaries.yml`を編集してください。

## 開発

### 新しいキーマップの作成

1. 既存のキーマップディレクトリをコピー：
```bash
cp -r keyboards/eswai/742/keymaps/naginata_v15y keyboards/eswai/742/keymaps/my_keymap
```

2. 好みに合わせてキーマップを修正
3. 変更をビルドしてテスト

### デバッグ

デバッグ用のコンソール出力を有効化：

```makefile
# キーマップのrules.mkに追加
CONSOLE_ENABLE = yes
```

## リソース

- [QMKドキュメント](https://docs.qmk.fm)
- [薙刀式公式ドキュメント](https://github.com/eswai/naginata-style)
- [オリジナル薙刀式実装](https://github.com/eswai/qmk_userspace)

## ライセンス

このプロジェクトはQMKファームウェアのライセンスに従います。詳細はQMKリポジトリを参照してください。

## 謝辞

- [大岡俊彦氏](http://oookaworks.seesaa.net/) - 薙刀式の考案者
- [eswai](https://github.com/eswai) - 薙刀式の実装とキーボード設計
- QMKコミュニティ - 優れたファームウェアフレームワークの提供