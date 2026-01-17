# CLAUDE.md

QMK userspace for Naginata-style (薙刀式) Japanese input method on custom keyboards.

## Build Commands

```bash
# Setup overlay
qmk config user.overlay_dir="$(realpath .)"

# Build firmware
qmk compile -kb <keyboard> -km roy

# Examples
qmk compile -kb planck/rev6 -km roy
qmk compile -kb zsa/planck_ez/base -km roy
```

## Keyboards (roy keymap)

| Keyboard | MCU | File | Bootloader |
|----------|-----|------|------------|
| planck/rev6 | STM32F303 | .bin | STM32 DFU |
| planck/rev7 | STM32F303 | .bin | STM32 DFU |
| zsa/planck_ez/base | STM32F303 | .bin | STM32 DFU |
| boardsource/technik_o | ATmega32u4 | .hex | Caterina |
| salicylic_acid3/goforty_ortho | RP2040 | .uf2 | RP2040 UF2 |

### goforty_ortho (Local Build Only)

Not in upstream QMK. Requires symlink:

```bash
mkdir -p ~/qmk_firmware/keyboards/salicylic_acid3
ln -s <userspace>/keyboards/salicylic_acid3/goforty_ortho ~/qmk_firmware/keyboards/salicylic_acid3/goforty_ortho
qmk compile -kb salicylic_acid3/goforty_ortho -km roy
```

CI builds this automatically and uploads to GitHub Actions Artifacts.

## Flashing

### STM32 DFU (planck, planck_ez)
1. QMK Toolbox → MCU: `STM32F303`
2. Load `.bin` file
3. Reset keyboard → Flash

### Caterina (technik_o)
1. QMK Toolbox → MCU: `ATmega32u4`
2. Load `.hex` file
3. Reset keyboard → Flash

### RP2040 (goforty_ortho)
1. Hold BOOTSEL + connect USB
2. Drag `.uf2` to `RPI-RP2` drive

## Naginata Versions (users/)

| Version | Status | Notes |
|---------|--------|-------|
| **naginata_v17** | Latest | Recommended for new projects |
| naginata_v16 | Fast mode | Quick confirmation |
| naginata_v15y | Stable | Early confirmation |
| roy | Personal | Wrapper using naginata_v17 |

## Directory Structure

```
keyboards/<vendor>/<keyboard>/keymaps/roy/
  ├── keymap.c      # Keymap definition
  ├── config.h      # Keymap config
  └── rules.mk      # Build flags

users/roy/
  └── rules.mk      # Includes naginata_v17

users/naginata_v17/
  ├── naginata.h    # Core engine header
  ├── naginata_v17.c
  └── *.rb          # Table generators
```

## Key Build Flags

```makefile
UNICODE_ENABLE = yes    # Required for Japanese output
COMBO_ENABLE = yes      # Key combinations
OS_DETECTION_ENABLE = yes # Auto-detect OS
MOUSEKEY_ENABLE = yes   # Mouse keys
AUTO_SHIFT_ENABLE = yes # Auto shift
```

## Notes

- Firmware downloads: [Releases](../../releases) or [Actions](../../actions)
- Each Naginata version is independent; do not mix
- OS detection handles macOS/Windows/Linux differences
