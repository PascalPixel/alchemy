# The Lost Age: Game Pak

The Lost Age's Game Pak: the ROM header (`rom_header.s`), the
[far-call](../../../tbs/glossary.md#far-call) tables (`far_call/`), the
resource directory (`resource_directory.s`) and the flash chip that holds the
saves (`flash/`). [Cartridge and memory](../../../tbs/src/cartridge/README.md)
explains them for The Broken Seal.

## What differs from The Broken Seal

- The far-call tables are 105 files under `far_call/`, named by the family of
  their exports (`sound_driver.s`, `object_dispatch_3.s`), where The Broken
  Seal has one file per [bank](../../../tbs/glossary.md#bank); in
  `games/tla/objects.tsv` most banks open with the run of these files that
  makes up their table. The banks start at fixed addresses
  (`games/tla/main.ld`), and the far-call stub (`far_call/veneer.s`) works as
  it does in The Broken Seal.
- The flash library (`flash/flash.c`, `sst.c`, `atmel.c`,
  `program_flash_sector.c`) is compiled with `old_agbcc`, as The Broken Seal's
  is (`FLASH_C`).
