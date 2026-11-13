# The Lost Age: system

The Lost Age's System code: power-on and the entry, the RAM layout, the
resident IWRAM code, the interrupt table, the heap, the
[task](../../../tbs/glossary.md#task) table, the two routines copied to RAM
to run (`resource/`), the save format (`savedata.c`), the link cable
(`system/serial.c`), the game flags (`system/flags.c`), the maths, the
[overlay](../../../tbs/glossary.md#overlay) streams (`overlay/`) and the
[main routine](../../../tbs/glossary.md#main-routine).
[System code](../../../tbs/src/system/README.md) and
[The Game Pak](../../../tbs/src/gamepak/README.md) explain, for The Broken
Seal, how a [frame](../../../tbs/glossary.md#frame) runs and why the image is
linked as it is.
The ROM header, the far-call tables, the resource directory and the flash chip
are in [The Lost Age: Game Pak](../gamepak/README.md).

## What differs from The Broken Seal

- The code from power-on to the game flags is one object,
  `system.c`, written as parts under `system/` and cut where The Broken
  Seal's parts are: power-on (`Entry` in `entry.s`, then `System_Initialize` in
  `system/start_up.c`, which enters the main routine), `wait()` and
  `Irq_SetHandler` (`system/frame.c`), the task table (`system/task.c`) and
  saving (`system/save.c`). The link cable (`system/serial.c`) and the
  game flags (`system/flags.c`) are its last parts, and the interrupt
  table (`irq_table.s`) is assembly beside the resident code it calls.
- The main routine, `MainRoutine_Run`, and the scene changes around it in the
  ROM, from `Scene_TakeExit` to `Scene_PlayMusic`, are one object,
  `main_routine.c`, as in The Broken Seal. It adds four debug starts and keeps
  background palettes 14 and 15 across three of its sequences.
- The game flags are kept by ten functions here (`system/flags.c`), where
  The Broken Seal keeps them with the game rules.
- The trigonometry (`trig.s`, `sine_table.s`), the tangent table the
  arctangent searches (`mathtan.s`), `math.c`, the long multiplies and the
  16.16 dot product are here, in the first bank, as their twins are.
- `overlay/overlays.s` places the packed overlay streams as The Broken Seal's
  does; how each overlay is linked is in
  [The Lost Age: scene](../scene/README.md).
- The task table holds 24 tasks (`TASK_COUNT`), not 20.
- Every Thumb file is compiled with The Broken Seal's options without
  `-mthumb-interwork`, plus `-mtune=gba` (`CFLAGS_tla` in the `Makefile`).
  This game's functions return with `pop {pc}`, which interworking code never
  does. The tune times instructions as the GBA's own processor runs them,
  calls through a register with `mov lr` and half a `bl` instead of a helper
  routine, and builds large constants in a register instead of reading them
  from the cartridge, as `alchemy-gcc`'s README describes.
- This game's fixed addresses are its own, listed in
  `games/tla/include/system/ram_buffer.h`, `iwram_call.h` and
  `gamepak/rom_header.h`.
- Each game numbers its own scenes, items, abilities, formations, resources
  and sounds, so an id from one does not name anything in the other.
- Functions that do what a twin does mostly take its name, so a search finds
  both. The folder pages name the exceptions.
