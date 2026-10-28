# The Lost Age: window

The Lost Age's window bank: the [windows](../../../tbs/glossary.md#window), the
[message](../../../tbs/glossary.md#message) decoder and text, glyphs, the
selection lists, the debug warp, the saving screens and the bank's tables
(`window_tables.s`, `keyboard.s`, `scales.s`). [Windows](../../../tbs/src/window/README.md)
explains the window system for The Broken Seal.

## What differs from The Broken Seal

- The code is not gathered into objects with parts: each file holds one
  function or a small family, some still named by address (`f03975c.c`).
- `cell_picture.c` is ARM code, compiled with GCC 2.95.2 like The Broken
  Seal's ARM files (`ARM_C`).
- `glow_update_sine.c` holds `PaletteGlow_UpdateSine`, which pulses four
  colours of background palette 14 along a sine. Its twin in The Broken Seal
  is `FileList_UpdatePulse` in
  [screens/save.c](../../../tbs/src/window/screens/save.c).
