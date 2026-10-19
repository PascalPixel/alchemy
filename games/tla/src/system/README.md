# The Lost Age: system

This folder holds what ties The Lost Age's image together, and much of what
☀️ keeps in its runtime folder: the cartridge header and entry, the RAM
layout, the resident IWRAM code, the far-call tables, the overlay streams,
the resource decoders, the task scheduler (`scheduler/`), the link cable
(`link/`) and saving (`save/`). ☀️'s [System](../../../tbs/src/system/README.md)
and [Runtime](../../../tbs/src/runtime/README.md) explain why the image is
linked as it is and how a frame runs; this page says only what differs here.

## What differs from ☀️

- **Grouping.** ⚓️ has no runtime folder: power-up (`system.c`), interrupt
  handlers (`setirq.c`), the scheduler, the link cable and the save records
  are here, beside the resident code they call.
- **Far-call tables.** ⚓️'s are 105 files under `far_call/`, named by the
  family of their exports (`audio_engine.s`, `object_dispatch_3.s`), where ☀️
  has one file per bank; a bank's table is the run of these files its place
  opens with in `games/tla/objects.tsv`. As in ☀️, the banks start at fixed
  addresses (`games/tla/main.ld`), and the stub and its reasons are the same.
- **Overlays.** `overlay/overlays.s` places the packed streams as ☀️'s does,
  but each overlay is linked by its own script under
  `recon/tla/raw/overlays`, not by one `overlay.ld`.
- **Scheduler.** 24 tasks at a time (`SCHEDULER_TLA_TASK_COUNT`), not 20.
- **Compilers.** Thumb files take K2's options on top of ☀️'s (`CFLAGS_tla`
  in the `Makefile`): `-mthumb-split-constants`, `-mthumb-call-via-lr` and
  `-mtune=arm9tdmi`, without `-mthumb-interwork`. The flash library
  (`save/flash.c`, `sst.c`, `atmel.c`, `program_flash_sector.c`) is compiled
  with pret's `old_agbcc`, as ☀️'s is (`FLASH_C`).
- **Fixed addresses.** ⚓️ lists its own in `games/tla/include/ram_buffer.h`,
  `iwram_call.h` and `cartboot.h`, with its own values.

## Status

Functions not yet matched are drafts under `recon/tla/en/main`, by address;
editions whose bytes differ stay on their scaffolds under `recon/tla/<lang>`.
`make progress` gives the counts.

## Evidence and open questions

- XXX: two banks do not open with a far-call file (`rom_main_00` opens with
  a resource decoder, `rom_main_05` with field code); whether they have no
  table or have it elsewhere is not settled.
