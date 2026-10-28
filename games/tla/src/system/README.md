# The Lost Age: system

What ties The Lost Age's image together, with much of what The Broken Seal
keeps in its runtime folder: the cartridge header and entry, the RAM layout,
the resident IWRAM code, the [far-call](../../../tbs/glossary.md#far-call) tables, the
[overlay](../../../tbs/glossary.md#overlay) streams, the resource decoders, the
[task](../../../tbs/glossary.md#task) table (`task/`), the link cable (`link/`) and saving
(`save/`). [Cartridge and memory](../../../tbs/src/cartridge/README.md) and
[Runtime](../../../tbs/src/runtime/README.md) explain, for The Broken Seal,
why the image is linked as it is and how a [frame](../../../tbs/glossary.md#frame) runs.

## What differs from The Broken Seal

- There is no runtime folder: power-on (`Entry` in `entry.s`, then
  `Func_0801319c` in `f01319c.c`, which enters the
  [main routine](../../../tbs/glossary.md#main-routine)), `wait()` (`system.c`), the interrupt
  table (`runtime_irq_table.s`) and `Irq_SetHandler` (`setirq.c`), the task
  table, the link cable and saving sit here, beside the resident code they
  call. The main routine itself is in the field folder
  ([The Lost Age: field](../field/README.md)).
- The far-call tables are 105 files under `far_call/`, named by the family of
  their exports (`sound_driver.s`, `object_dispatch_3.s`), where The Broken
  Seal has one file per [bank](../../../tbs/glossary.md#bank); in `games/tla/objects.tsv` most banks
  open with the run of these files that makes up their table. The banks start
  at fixed addresses (`games/tla/main.ld`), and the far-call stub works as it
  does in The Broken Seal.
- `overlay/overlays.s` places the packed overlay streams as The Broken Seal's
  does; how each overlay is linked is in
  [The Lost Age: scene](../scene/README.md).
- The task table holds 24 tasks (`TASK_COUNT`), not 20.
- Every Thumb file is compiled with `-mthumb-split-constants`,
  `-mthumb-call-via-lr`, `-mthumb-slow-loads` and `-mtune=arm9tdmi` added to
  The Broken Seal's options, and without `-mthumb-interwork` (`CFLAGS_tla` in
  the `Makefile`). The flash library (`save/flash.c`, `sst.c`, `atmel.c`,
  `program_flash_sector.c`) is compiled with `old_agbcc`, as The Broken Seal's
  is (`FLASH_C`).
- This game's fixed addresses are its own, listed in
  `games/tla/include/ram_buffer.h`, `iwram_call.h` and `cartboot.h`.
- Each game numbers its own scenes, items, abilities, formations, resources
  and sounds, so an id from one does not name anything in the other.
- Functions that do what a twin does mostly take its name, so a search finds
  both. The folder pages name the exceptions.
