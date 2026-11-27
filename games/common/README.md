# What both games share

The Broken Seal and The Lost Age are two games built by one studio on one
engine, and many of their files are the same byte for byte. Each file here
is one copy that both games build. Nothing here is linked once for both: every
edition builds its own objects from these files, under
`out/<edition>/obj/games/common/`, and the game's linker script places each
one where that game's cartridge has it. A file moves here only when both
games have it unchanged and its place in either ROM stays the same.

- [`sdk/`](sdk/README.md) is the development kit's code: the flash library
  and the sound driver.
- `features.h` and `stddef.h` are headers both games include unchanged: how
  the code asks for an edition's features and languages, and `NULL`,
  `size_t` and `offsetof`. The build searches `games/common` after the
  game's own `include/`, so a game's file includes them as it always did
  (`#include "features.h"`).
- `sound/songs/` holds the songs and sound effects both games have, one MIDI
  file (or, for the empty songs, one assembly file) each, and
  `sound/direct_sound_samples/` the recorded samples both play. Each game's
  `sound/song_table.s` and `sound/direct_sound_samples.s` still list every
  sound in that game's order. `sound/programmable_wave_samples.s` is the wave
  channel's patterns.
- `text/messages.s` places the edition's messages. The messages themselves
  are each game's own, in its `text/*.po`.
- `graphics/font/` holds the six font sheets both games have, one for each
  language, with their tables of widths; each game's `src/graphics/font/text.s`
  places its edition's sheet.
- `graphics/ui/marks/` and `graphics/ui/menu_titles/` hold the icon marks and
  the menu titles both games draw; each game's `src/graphics/ui/marks.s` and
  `src/graphics/ui/menu_titles.s` place them, beside the ones that are its
  own. `graphics/ui/` also holds the settings menu's slider and its colours
  and the sprite cursors, each with its list.
- `window/` holds the Djinn menu's blinking tile, and `battle/transition/`
  the battle curtain's pictures, each with its list.

A picture listed in a game's own list beside other pictures of that game
stays in that game's folder even when the other game has the same file:
a list names the files beside it.
