# The Lost Age: window

The Lost Age's window bank: the [windows](../../../tbs/glossary.md#window), the
[message](../../../tbs/glossary.md#message) decoder and text, glyphs, the
selection lists, the debug warp, the saving screens and the bank's tables
(`window_tables.s`, `keyboard.s`, `scales.s`). [Windows](../../../tbs/src/window/README.md)
explains the window system for The Broken Seal.

## What differs from The Broken Seal

- `window.c` is The Broken Seal's window object, written as parts under
  `window/` cut where The Broken Seal's are, with one part more: `debug.c`,
  the debug menus, which The Broken Seal keeps in its screens object.
  `screens.c` is its screens object as far as the battle Psynergy menu
  (`battle_psynergy_menu.c`), written the same way. `menus.c` holds the
  rest of that object, from the battle target selection to the pause menu.
- `cell_picture.c` is ARM code, compiled with GCC 2.95.2 like The Broken
  Seal's ARM files (`ARM_C`).
- `screens/save.c` holds `FileList_UpdatePulse`, which pulses four colours
  of background palette 14 along a sine. Its twin in The Broken Seal is
  `FileList_UpdatePulse` in
  [screens/save.c](../../../tbs/src/window/screens/save.c).
