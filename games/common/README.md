# What both games share

Only the development kit's code lives here, in [`sdk/`](sdk/README.md): the
flash library and the sound driver. Everything else, even a file both games
have byte for byte, stays in each game's own folder.

Each edition builds its own objects from these files, under
`out/<edition>/obj/games/common/`, and the game's linker script places each
one where that game's cartridge has it. The build searches `games/common`
after the game's own `include/`, so a game's file includes a kit header by its
path under here (`#include "sdk/flash.h"`).
