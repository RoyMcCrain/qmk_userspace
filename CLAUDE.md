# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Overview

This is a QMK userspace repository implementing the Naginata-style (薙刀式) Japanese input method for custom mechanical keyboards. The codebase manages multiple versions of the Naginata input system and supports various custom keyboards manufactured by eswai.

## Common Commands

### Building Firmware
```bash
# Build firmware for specific keyboard and keymap
make eswai/742:naginata_v15y
make eswai/x60:naginata_v15y
make eswai/vpico:naginata_v15y

# Build and flash directly to keyboard
make eswai/742:naginata_v15y:flash
```

### Available Keyboards
- `eswai/742` - Split keyboard with OLED display
- `eswai/x60` - 60% keyboard
- `eswai/vpico` - RP2040-based keyboard
- `eswai/kunai16` - 16-key macropad
- Additional keyboards in `keyboards/eswai/`

### Naginata Versions
- `naginata_v15y` - Current stable version with early confirmation feature
- `naginata_v16` - Development version (fast mode)
- `naginata_v15` - Legacy stable version

## Architecture

### Directory Structure
- `keyboards/eswai/*/` - Keyboard-specific configurations and keymaps
- `users/naginata_*/` - Shared Naginata input method implementations
- Each keyboard has its own `rules.mk`, `config.h`, and keymap directories

### Key Components

**Naginata Input System**:
- `naginata.h/c` - Core input processing engine
- `twpair_on_jis.h/c` - Two-stroke combination handling for JIS layout
- `nglist.h/c` - Conversion candidate list management (v15y+)
- `naginata_qmk_v*.rb` - Ruby scripts for generating conversion tables

**OS Detection**:
- Automatic OS detection via `os_detection.h`
- Separate handling for macOS, Windows, and Linux

**Build Configuration**:
- `UNICODE_ENABLE = yes` - Required for Japanese character output
- `NKRO_ENABLE = yes` - N-Key Rollover support
- `LTO_ENABLE = yes` - Link Time Optimization for smaller firmware

### Development Workflow

1. **Creating New Keymaps**: Copy existing keymap directory and modify
2. **Testing Changes**: Build and flash to test on actual hardware
3. **Debugging**: Enable console output with `CONSOLE_ENABLE = yes` in rules.mk

### Important Notes

- QMK firmware must be installed separately with `qmk config user.qmk_home` configured
- The v15y version is the recommended stable implementation
- Each Naginata version is independently implemented; mixing versions is not supported
- No automated tests or linting tools are configured