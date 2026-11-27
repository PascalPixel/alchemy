# The development kit's code

Both games link the same two libraries from the console's development kit:
the SDK's flash library, which identifies, erases, writes and reads the save
chip, and its sound driver, MusicPlayer2000, which pret calls m4a. This
folder holds the one copy of them that both games build. It is the kit's
code, not Camelot's: what the studio wrote over the driver and around the
save stays with each game.

- `flash/` is the flash library: `flash.c` finds the chip and times the
  waits, `sst.c`, `atmel.c` and `program_flash_sector.c` erase and program
  each kind of chip, and `flashsst.c`, `flashmx.c`, `flashat.c` and
  `flashtab.c` are the chips' settings and the list of them.
- `sound/` is the driver: `sound.c` starts it and controls the music
  players, `driver.c` is its core, the four CGB channels and the track
  controls, `tables.c` its jump table and pitch tables, and the assembly is
  its handwritten part, the song commands (`clear_buffer.s`), the mixer
  that runs in IWRAM (`mixer.s`) and SoundMain's entry (`umul_high.s`).
- `flash.h`, `flash_data.h`, `sound_driver_types.h` and `sound_driver.h`
  declare them. The build searches `games/common` after the game's own
  `include/`, so a file of either game includes them as
  `#include "sdk/flash.h"`.

## Each game builds its own copy

Every edition compiles these files into its own objects, under
`out/<edition>/obj/games/common/sdk/`, and links them where its linker
script names them. `flash.c`, `sst.c`, `atmel.c`, `program_flash_sector.c`,
`sound.c`, `driver.c` and `tables.c` go through the kit's compiler, pret's
old_agbcc (`FLASH_C` and `SOUND_C` in the Makefile); the chip tables and
`empty_callback.c` go through the game's own compiler, as the rest of its C.
Either way the objects come out the same in both games but for the three
numbers below.

## What each game sets

`games/<game>/include/sdk_config.h` has the same name in both games and
holds what differs between them:

| Setting              | The Broken Seal | The Lost Age | What it is                                         |
| -------------------- | --------------- | ------------ | -------------------------------------------------- |
| `Ram_MPlayJumpTable` | `0x02004000`    | `0x02006800` | where the driver's jump table sits in EWRAM        |
| `Ram_SoundMixer`     | `0x03007000`    | `0x03006000` | where `m4aSoundInit` copies the mixer in IWRAM     |
| `SOUND_GAME_MODE`    | `0x0097F800`    | `0x0099FA00` | the sound mode the driver starts in: notes, rate   |

They stay numbers. Written as a linked name, the jump table's address makes
old_agbcc build different code for `driver.c`, and then neither game
matches. The mixer's address has to agree with the `.iwram_mixer` address
in the game's linker script, and a wrong value anywhere shows up only as a
ROM that no longer matches.

The rest comes from each game as it does for any of its code: `types.h`,
the hardware registers in `system/definitions.h`, `Bios_CpuSet`, the music
player and song tables in `games/<game>/sound`, and the symbols its linker
script defines, `sySndMainRAMRom` and `Mixer_CallViaR3Rom`.

## What stays with each game

- `include/sound/sound_driver_symbols.h` is the game's layer over the
  driver: the ranges of its sound numbers, its music players by row, its
  reverb codes and the music's ramps. It is Camelot's, and different in
  each game.
- `src/system/channel_update_entry.s`, `resident_mixer.s` and
  `resident_mixer_interpolate.s` are Camelot's own mixer, resident in IWRAM
  beside `mixer_render.s`. The two games' code differs: The Lost Age mixes
  528 samples a VBlank to The Broken Seal's 352, with other instructions.
- `games/tla/src/sdk/sound/` holds kit bytes only The Lost Age's cartridge
  has: its live `MPlayMain` and `ply_note` in `mplay_main.s`, older copies
  of driver code (`leftover_driver.s`, `player_work_copy.c`,
  `dummy_func_work_copy.c`) and two unused BIOS calls
  (`bios_sound_driver_mode.s`).

Because of that last folder, `MPlayMain` and `ply_note` in `clear_buffer.s`
are weak names. The Broken Seal's driver runs them from there; The Lost Age
links its own pair from `mplay_main.s`, which take the names, and keeps
these bytes in the cartridge unreferenced.
