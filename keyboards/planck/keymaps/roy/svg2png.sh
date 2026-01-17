#!/bin/bash
# YAMLからSVG/PNGを生成するスクリプト

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"

# YAML → SVG (keymap-drawer)
keymap draw "$SCRIPT_DIR/keymap.yaml" -o "$SCRIPT_DIR/my_keymap.svg"

# SVG → PNG (resvg)
npx resvg-cli "$SCRIPT_DIR/my_keymap.svg" "$SCRIPT_DIR/keymap.png"
