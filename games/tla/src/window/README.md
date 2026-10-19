# The Lost Age: window

This folder holds one file of The Lost Age's window code,
`glow_update_sine.c`: `PaletteGlow_UpdateSine`, the sine-driven glow of four
colours of BG palette 14. Its twin is in ☀️'s
[Windows](../../../tbs/src/window/README.md) (`window/battle_command/save.c`),
which explains the window system it belongs to.

## What differs from ☀️

- **Grouping.** The rest of ⚓️'s window system, the windows, channels and
  message decoder, is under `games/tla/src/graphics/window/` and
  `games/tla/src/graphics/text/` (see
  [The Lost Age: graphics](../graphics/README.md)).
- **Compiler.** K2's options on top of ☀️'s (`CFLAGS_tla` in the
  `Makefile`): `-mthumb-split-constants`, `-mthumb-call-via-lr` and
  `-mtune=arm9tdmi`, without `-mthumb-interwork`.

## Evidence and open questions

- XXX: why this one file sits here and the rest of the window code under
  `graphics/`, and whether the rest moves here once its object boundaries
  are known.
