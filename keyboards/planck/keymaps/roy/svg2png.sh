#!/bin/bash
# SVGをPNGに変換するスクリプト

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
npx resvg-cli "$SCRIPT_DIR/keymap.svg" "$SCRIPT_DIR/keymap.png"
