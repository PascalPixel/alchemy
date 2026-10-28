# The Lost Age: system

The Lost Age's System code: power-on and the entry, the RAM layout, the
resident IWRAM code, the interrupt table, the heap, the
[task](../../../tbs/glossary.md#task) table (`task/`), the resource loader
(`resource/`), the save format (`save/`), the link cable (`serial/`), the game
flags (`flags/`), the maths, the [overlay](../../../tbs/glossary.md#overlay)
streams (`overlay/`) and the [main routine](../../../tbs/glossary.md#main-routine).
[Cartridge and memory](../../../tbs/src/cartridge/README.md) and
[Runtime](../../../tbs/src/runtime/README.md) explain, for The Broken Seal,
why the image is linked as it is and how a [frame](../../../tbs/glossary.md#frame) runs.
The ROM header, the far-call tables, the resource directory and the flash chip
are in [The Lost Age: Game Pak](../gamepak/README.md).

## What differs from The Broken Seal

- The code is not gathered into one object with parts: power-on (`Entry` in
  `entry.s`, then `Func_0801319c` in `f01319c.c`, which enters the main
  routine), `wait()` (`system.c`), the interrupt table (`irq_table.s`) and
  `Irq_SetHandler` (`setirq.c`), the task table, the link cable and saving
  are each their own files, beside the resident code they call.
- The main routine, `MainRoutine_Run` (`main_routine.c`), sits here with the
  run of files around it in the ROM (`take_exit.c`, `return_from_sequence.c`,
  `data_get_resource_group.c`, `data_load_resource_group.c`,
  `reset_flags_on_enter.c`, `play_saved_music.c`), as The Broken Seal's
  `main_routine.c` does. It adds four debug starts and keeps background
  palettes 14 and 15 across three of its sequences.
- The game flags are read by five functions here (`flags/`), where The Broken
  Seal keeps them with the game rules.
- The trigonometry (`trig.s`, `sine_table.s`), the tangent table the
  arctangent searches (`mathtan.s`), `math.c`, the long multiplies and the
  16.16 dot product are here, in the first bank, as their twins are.
- `overlay/overlays.s` places the packed overlay streams as The Broken Seal's
  does; how each overlay is linked is in
  [The Lost Age: scene](../scene/README.md).
- The task table holds 24 tasks (`TASK_COUNT`), not 20.
- Every Thumb file is compiled with `-mthumb-split-constants`,
  `-mthumb-call-via-lr`, `-mthumb-slow-loads` and `-mtune=arm9tdmi` added to
  The Broken Seal's options, and without `-mthumb-interwork` (`CFLAGS_tla` in
  the `Makefile`).
- This game's fixed addresses are its own, listed in
  `games/tla/include/system/ram_buffer.h`, `iwram_call.h` and
  `gamepak/rom_header.h`.
- Each game numbers its own scenes, items, abilities, formations, resources
  and sounds, so an id from one does not name anything in the other.
- Functions that do what a twin does mostly take its name, so a search finds
  both. The folder pages name the exceptions.
