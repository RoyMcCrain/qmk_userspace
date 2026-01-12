# CLAUDE.md

QMK userspace for Naginata-style (薙刀式) Japanese input method on custom keyboards.

## Build Commands

```bash
# Build firmware
make eswai/<keyboard>:<keymap>

# Build and flash
make eswai/<keyboard>:<keymap>:flash

# Examples
make eswai/742:naginata_v17
make eswai/x60:naginata_v15y:flash
```

Requires: `qmk config user.qmk_home` must be configured.

## Keyboards (keyboards/eswai/)

| Keyboard | Type | MCU |
|----------|------|-----|
| 742 | Split, OLED | ATmega32u4 |
| x60 | 60% | ATmega32u4 |
| vpico | 4x10 ortho | RP2040 |
| kunai16 | Macropad | ATmega32u4/STM32 |
| anhedral67 | 67-key | ATmega32u4 |
| mouse | 3-key trackball | RP2040 |
| ssol56 | 56-key split | ATmega32u4 |
| qol56 | 56-key | ATmega32u4 |
| x68 | 68-key | ATmega32u4 |
| shuriken16/19 | Macropad | ATmega32u4 |
| str20 | 60% | STM32F103 |
| numattack16 | Keypad | ATmega32u4 |

## Naginata Versions (users/)

| Version | Status | Notes |
|---------|--------|-------|
| **naginata_v17** | Latest | Recommended for new projects |
| naginata_v16 | Fast mode | Quick confirmation |
| naginata_v15y | Stable | Early confirmation |
| naginata_v15/x/xx | Legacy | Older implementations |
| roy | Personal | Wrapper for external keyboards |

## Directory Structure

```
keyboards/eswai/<keyboard>/
  ├── info.json       # QMK metadata
  ├── config.h        # Hardware config
  ├── rules.mk        # Build flags
  └── keymaps/<name>/ # Keymap directories

users/naginata_v*/
  ├── naginata.h      # Core engine header
  ├── naginata_v*.c   # Implementation
  ├── twpair_on_jis.* # JIS two-stroke pairs
  ├── nglist.*        # Candidate list
  └── *.rb            # Table generator
```

## Key Build Flags

```makefile
UNICODE_ENABLE = yes    # Required for Japanese output
NKRO_ENABLE = yes       # N-Key Rollover
LTO_ENABLE = yes        # Smaller firmware
COMBO_ENABLE = yes      # Key combinations
CONSOLE_ENABLE = yes    # Debug output
```

## Development

1. Copy existing keymap to create new one
2. Build and flash to test
3. Enable `CONSOLE_ENABLE` for debugging

Ruby scripts (`*.rb`) generate conversion tables from dictionary files.

## Notes

- Each Naginata version is independent; do not mix
- OS detection handles macOS/Windows/Linux differences
- No automated tests configured