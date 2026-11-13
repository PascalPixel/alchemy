# The Lost Age: Game Pak

The Lost Age's Game Pak: the ROM header (`rom_header.s`), the
[far-call](../../../tbs/glossary.md#far-call) tables (`far_call/`), the
resource directory (`resource_directory.s`) and the flash chip that holds the
saves (`flash/`). [The Game Pak](../../../tbs/src/gamepak/README.md)
explains them for The Broken Seal.

## What differs from The Broken Seal

- The far-call tables are one file per [bank](../../../tbs/glossary.md#bank)
  under `far_call/` (`FIELD.S`, `SOUND.S`), as in The Broken Seal, apart
  from `resource_f.s`, the table of a bank The Broken Seal does not have; in
  the linker script (`games/tla/ld_script.ld`) each bank opens with its
  table. The banks start at fixed addresses,
  written in the script where each begins; the four that differ by edition
  (the game, field sequence, menu and shop banks) are named at its top, one
  macro each. The far-call stub (`far_call/veneer.s`) works as it does in The
  Broken Seal.
- The flash library (`flash/flash.c`, `sst.c`, `atmel.c`,
  `program_flash_sector.c`) is compiled with `old_agbcc`, as The Broken Seal's
  is (`FLASH_C`).
