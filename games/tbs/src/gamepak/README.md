# The Game Pak

The [Game Pak](../../glossary.md#game-pak) is the cartridge as hardware: the
ROM the console runs the game from and the flash chip the saves are written
to. A Game Boy Advance has 288 KB of working memory against an 8 MB cartridge
that holds the whole game, and every [scene](../../glossary.md#scene) the
[party](../../glossary.md#the-party) visits needs its own code,
[map](../../glossary.md#map) and pictures. The files here make the ROM fit
together: the header the console starts from (`rom_header.s`), the
[far-call](../../glossary.md#far-call) tables that open each
[bank](../../glossary.md#bank) of the
[main image](../../glossary.md#main-image) (`far_call/`), the
[resource directory](../../glossary.md#resource-directory) through which every
picture, map and piece of scene code is found by number
(`resource_directory.s`), and the library that drives the
[flash chip](../../glossary.md#flash-chip) (`flash/`). What the game keeps in
RAM, and how a scene's code is unpacked there, is
[System code](../system/README.md).

## The ROM header

The console starts the game at the cartridge's first word, a branch to
`System_Entry`, inside a block of fixed data the console checks: the
logo, the game's title, its code `AGS` with a letter for the edition's
language, the maker code and a complement check over them. The build leaves
the logo and the complement check empty, and `tools/ags-header` writes them
into the finished image.
The header is `Rom_Start` in `rom_header.s`; the edition's letter is
`gRom_LanguageCode`.

## Why the main image is cut into banks

Each bank starts at a fixed address with room to spare and opens with a
table of [veneers](../../glossary.md#veneer), one for each function other banks
call, so a bank's code can grow or shrink without moving an address another
bank holds ([System](../../system.md#banks-and-far-calls)). A far call goes
through the veneer, named for its function with `Far` added: `Sound_PlayFar`
reaches `Sound_Play`. The System's bank 00 is the exception: other banks call
its functions directly, and only the overlays go through its table.

A veneer is the 8-byte `overlay_veneer` macro in `far_call/veneer.s`: `ldr r4`
loads the address held in the word after it, `bx r4` jumps there, and the word
is the target's address. It can overwrite r4 without saving it because every
game file is compiled with `-fcall-used-r4`, so no caller expects r4 to
survive a call. `bx` also switches the processor to ARM when the target is ARM
code, so the same veneer reaches the ARM routines copied into
[IWRAM](../../glossary.md#iwram-and-ewram).

A table's entries keep their order, since an entry added in the middle would
move every veneer after it. An entry no code calls by name stays in its place
as a bare `overlay_veneer` row with no `Far` name. Each bank's table is a
file in `far_call/` named for the bank in capitals (`FIELD_SEQUENCE.S`), with
`far_call/table.inc` as every table's head, and
[Source](../README.md#banks-and-folders) lists the banks. The C names of each
table's entries are in `include/gamepak/far_call/`. An
[overlay](../../glossary.md#overlay) reaches these tables through its import
table ([System code](../system/README.md#how-an-overlay-reaches-the-rest-of-the-game)).

## The resource directory and table order

When the party enters Bilibin, the scene table doesn't say where Bilibin's code
is; it gives a number, which picks a row of the resource directory, and the
row says where the packed data starts. Pictures, maps and tables are found the
same way. A resource's number is its place in the list, which keeps the lookup
to one step but means a row added in the middle renumbers everything after it.

Code names a resource by a symbol, which follows a moved row, but in a few
places the numbers travel bare: the scene table names each scene's code by
number, and some code counts on from one resource to the next (the maps, and
the eight pictures of the settings menu's marker). The directory ends with 25
empty rows nothing reads, where a row fits without renumbering any other.

The directory is `Resource_DirectoryTable` in `resource_directory.s`, read by
`Resource_GetTableEntry`; each row defines its `ResourceId_` symbol.

## The flash chip

The saves live in a 64 KB flash chip on the cartridge, written and erased in
16 sectors of 4 KB. Cartridges were built with chips from different makers,
each wanting its own commands, so at start-up the library asks the chip for
its maker and device codes and picks the routines for that chip from
`gFlash_Chips`: SST, Atmel, Macronix or Panasonic, with a default for any
other. How a save is spread over the sectors so that a power cut never loses
one is [Saving](../system/README.md#saving).

`IdentifyFlash` reads the chip's codes through `ReadFlashId`; the library is
`flash.c`, `sst.c`, `atmel.c` and `program_flash_sector.c` with their tables
in the `flash*.s` files, compiled with `old_agbcc` (`FLASH_C` in the
`Makefile`). Its version string is `FLASH_V123`.

## Design

Banks are not needed for reach: a Thumb `bl` reaches 4 MB either way and the
main image is 991 KB (just under 1 MB, in the English link map). They exist so
a bank can grow without moving an address another bank holds. Overlays pay
for the same independence with two veneers per call.

## How it fits with the rest of the game

The header hands the console to the [System](../system/README.md#start-up),
whose bank 00 the other banks call directly; every other bank is reached
through its table here. Code everywhere loads its pictures and maps through
the resource directory, and [saving](../system/README.md#saving) writes
through the flash library.

## Limits

- A bank has the space up to the next bank's fixed start.
- A far-call table's entries keep their order: an entry added in the middle
  would move every veneer after it.
- The resource directory ends with 25 empty rows nothing reads; a row added
  anywhere else renumbers every row after it.
- 16 flash sectors of 4 KB.

## Evidence

The folder is named for Nintendo's word for the cartridge, the Game Pak, as
GBATEK writes "GamePak ROM", "GamePak SRAM" and "GamePak Waitstates". The
far-call tables are named for the bank they open, the veneers for their target
with `Far` added, and the overlays by their directory number. The flash
library's files are named for the chip makers (`flashat`, `flashmx`,
`flashsst`) or in plain English (`flash`, `program_flash_sector`).
