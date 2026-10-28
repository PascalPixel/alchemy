# The Lost Age: graphics

The Lost Age's pictures and the code that draws with them, and most of what
The Broken Seal's window folder holds: the [windows](../../../tbs/glossary.md#window)
(`window/`), the [message](../../../tbs/glossary.md#message) decoder and text (`text/`), glyphs,
palettes and the display. [Graphics](../../../tbs/src/graphics/README.md) and
[Windows](../../../tbs/src/window/README.md) explain how these work.

## What differs from The Broken Seal

- The window system and the message decoder live under `graphics/window/` and
  `graphics/text/`, which have a folder to themselves in The Broken Seal;
  this game's [window folder](../window/README.md) holds one file. The
  pictures are grouped by what they show, as in The Broken Seal
  (`battle_fx/`, `monster/`, `world_map/` and the rest), each built from its
  PNG.
- `tile/cell_picture.c` is ARM code, compiled with GCC 2.95.2 like The Broken
  Seal's ARM files (`ARM_C`).
- This game's copies of pictures The Broken Seal also has are its own files
  under its own resource numbers.
