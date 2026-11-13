# The Game Pak

The cartridge holds two chips. A 16 MB ROM has the whole game: the code and
every picture, map, tune and line of text. A small flash chip, 64 KB, can be
written to and keeps the three save files.

This page is about how things on the ROM are found, and how the code on it
is laid out. Both rest on one idea: a thing is found by its position in a
table, so tables only ever grow at the end.

## The ROM header

The first 192 bytes are the same layout on every Game Boy Advance game: a
jump to the game's first instruction, the Nintendo logo, a title, a game
code and a checksum. The console checks them before it lets the game run.

Each language is a cartridge of its own, an
[edition](../../glossary.md#editions), and the last letter of its game code
names the language: `AGFJ`, `AGFE`, `AGFD` and so on.

## The resource directory and table order

Every stored thing on the ROM has a number: a map, a sprite sheet, a
battle background, a scene's code. The
[resource directory](../../glossary.md#resource-directory) is one table of
2,048 rows, and row *n* holds the address of resource *n*. Whoever wants a
resource holds its number, reads that row, and goes there. One read, no
searching.

Walking through a door is two lookups. The scene table says which resource
is the new scene's overlay. The directory says where that resource is. The
game unpacks it into memory and runs it.

A number is nothing but a row's position, so the table can never be
reordered. Put a new picture in at row 500 and every later number would
point one row off. Anything new goes at the end, and 325 empty rows are
waiting there.

Roughly, reading down the rows: the windows' pieces and fonts, the title,
the battle backgrounds, the minigames, the summons and battle effects, the
world map, the enemies, the maps with their tiles and colours, then the 114
overlays.

One of the first rows points at the build stamp, the date and time the
edition was built. A save file records it, so the game can tell a file
written by another build when it loads one.

## The flash chip

Writing a save is slow because flash is awkward. The chip is 16 sectors of
4 KB, and a sector cannot be changed in place: it is erased whole and
written again from the start, a byte at a time, each byte sent as a short
command and then read back until the chip agrees.

Cartridges were made with chips from four makers that take different
commands and need different patience. So before every save or load the
game asks the chip who made it and picks the matching routines, and every
wait has a time limit so a dead chip cannot hang the game.

How the 16 sectors are used, so that a power cut never costs a save, is in
[Saving](../system/README.md#saving).

## Why the main image is cut into banks

The code that is always loaded, the
[main image](../../glossary.md#main-image), is cut into fifteen
[banks](../../glossary.md#bank), one for each large part of the game: the
battle, the sound, the windows, the menus, the shops, even Lucky Dice,
Lucky Wheels and Super Lucky Dice, one bank each. Each bank starts at a
fixed address. For four of them, the game, field sequence, menu and shop
banks, that address depends on the edition.

Code inside a bank moves whenever that bank is rebuilt. So one bank never
calls another's function directly. Each bank opens with a table of
[veneers](../../glossary.md#veneer), one for each function other banks may
use, and a call from outside goes to a position in that table. That is a
[far call](../../glossary.md#far-call).

It is the resource directory's idea again, for code. The table is the one
thing that does not move, callers know a function only by its place in it,
and so a table only ever grows at the end.

### A veneer

A veneer is 8 bytes: "jump to the address that follows", and the address.
An overlay carries a small table of its own veneers, each pointing at a
veneer in a bank. So a call from a scene into the main image takes two
hops.

The first bank is the exception. It holds the System, and the other banks
call its functions directly. Only the overlays go through its table.

## Where the files are

- `rom_header.s` is the header, and `resource_directory.s` the 2,048
  rows.
- `flash/` is the flash chip's routines.
- `far_call/` holds each bank's table.
