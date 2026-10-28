# Cartridge and memory

A Game Boy Advance has 288 KB of working memory against an 8 MB cartridge
that holds the whole game, and every [scene](../../glossary.md#scene) the
[party](../../glossary.md#the-party) visits needs its own code,
[map](../../glossary.md#map) and pictures. The tables and assembly here make
that fit: they decide which part of memory holds what, split the
[main image](../../glossary.md#main-image) into
[banks](../../glossary.md#bank) that call each other through fixed tables,
swap each scene's code in and out of one small area, and find every picture,
map and piece of code on the cartridge by number. Nearly all of it is
assembly: tables the linker lays out, and code no C
[compiler](../../glossary.md#the-compilers) emits; each file's header names
the instruction or pattern that rules out C (`svc`, `mrs SPSR`, a literal
pool two functions share). [Runtime](../runtime/README.md) builds
directly on it, and [Scenes](../scene/README.md) are the main users of its
[overlays](../../glossary.md#overlay).

## Memory map

The console has two kinds of working memory,
[IWRAM and EWRAM](../../glossary.md#iwram-and-ewram). EWRAM, 256 KB, is large but
slower and holds most of the game's data: its variables, the current scene's
code, the map's grid and pictures being unpacked, and the larger
[heap](../../glossary.md#heap-and-pools). IWRAM, 32 KB, is fast and holds
what must run or be read quickly: routines copied from the cartridge at
power-on, the busiest variables, a second heap and the stacks. Video memory
holds the tiles and tile maps the screen is drawn from, and the processor runs
the main image straight from the cartridge.

Each area has a fixed place, the same in every
[edition](../../glossary.md#editions), except that German reserves 16 bytes
before the IWRAM variables, so all of them sit 16 bytes higher (three of them
are fixed addresses, below):

| EWRAM, from | Size | What |
| --- | --- | --- |
| 0 KB | 32 KB | the game's variables: the [main routine's work](../../glossary.md#main-routines-work), the [game flags](../../glossary.md#game-flag), the sound driver's work |
| 32 KB | 32 KB | the current scene's code, `gOverlayArea` |
| 64 KB | 112 KB | the map grid's [cells](../../glossary.md#cell), the battle figures' pictures, unpacking space, the map's [cell pictures](../../glossary.md#cell-picture) and its animated tiles |
| 176 KB | 16 KB | the shapes of the ground |
| 192 KB | 32 KB | the EWRAM heap |
| 224 KB | 32 KB | background tiles being prepared, used only by the world map |

| IWRAM, from | What |
| --- | --- |
| 0 KB | 5 KB of routines copied from the cartridge: the interrupt code, the sound mixing and small helpers the rest of the game calls |
| 5 KB | the busiest variables, among them the [heap slots](../../glossary.md#heap-slot) |
| 8 KB | the IWRAM heap |
| 28 KB | the sound driver's own mixer, copied in when sound starts |
| 30 KB | the build stamp and the start of the stacks |
| the last 256 bytes | the stack tops and the address the console jumps to on an interrupt |

The allocators test only against the end of EWRAM and against the IWRAM
heap's end at 30 KB, so each heap can run into the buffer above it, and in
play both do: the EWRAM heap reaches into the world map's tile buffer in
battle, and in battle the IWRAM heap runs over the sound mixer's copy at
28 KB, which never runs during a battle (the measurements are in the comments
on `gEwramHeap` and `gIwramHeap`, `include/system/mem.h`).
`main.ld` declares the areas, and the English link map,
`out/tbs-en/tbs-en.map`, has every address.

### Video memory

The screen is built from 8 by 8 pixel tiles. A
[background layer](../../glossary.md#background-layer) is a sheet of tiles plus
a screen map saying which tile goes where; a
[sprite](../../glossary.md#sprite-and-oam) draws its own tiles wherever it is
put. On a scene, video memory holds the windows' tiles first, then the
windows' screen map and the three map layers' screen maps, then the map's
three sheets of tiles, and in its last 32 KB the sprites' tiles, which a cache
hands out in 512 blocks of 64 bytes. The credits screen divides the background
memory into two halves instead
([Graphics](../graphics/README.md#two-battle-backgrounds-in-one-background-memory)).

The window tiles are placed by `Window_LoadGraphics`, the map's by
`MapGrid_LoadMap` ([Maps](../map/README.md)), and the sprite tiles by the
cache's `VramCache_Load` and `VramCache_AllocateBlocks`.

## Why the main image is cut into banks

Each bank starts at a fixed address with room to spare and opens with a
table of [veneers](../../glossary.md#veneer), one for each function other banks
call, so a bank's code can grow or shrink without moving an address another
bank holds ([System](../../system.md#banks-and-far-calls)). A
[far call](../../glossary.md#far-call) goes through the veneer, named for its
function with `Far` added: `Sound_PlayFar` reaches `Sound_Play`. The runtime
bank is the exception: other banks call its functions directly, and only the
overlays go through its table.

A veneer is the 8-byte `overlay_veneer` macro in `overlay.s`: `ldr r4` loads
the address held in the word after it, `bx r4` jumps there, and the word is
the target's address. It can overwrite r4 without saving it because every game
file is compiled with `-fcall-used-r4`, so no caller expects r4 to survive a
call. `bx` also switches the processor to ARM when the target is ARM code, so
the same veneer reaches the ARM routines copied into IWRAM.

A table's entries keep their order, since an entry added in the middle would
move every veneer after it. An entry no code calls by name stays in its place
as a bare `overlay_veneer` row with no `Far` name. Each bank's table is a
file in `far_call/`, and [Source](../README.md#banks-and-folders) lists the
banks.

## How an overlay reaches the rest of the game

Since all the scenes' code together would not fit in memory, each scene's code
is packed on the cartridge as an overlay and unpacked, when the party arrives,
into the 32 KB overlay area, over the scene before. Every overlay is built to
run at that one address, so only one is ever loaded and an overlay never calls
another; code several scenes need is copied into each. There are 96 overlays:
several scenes can share one, and the Title and a few screens that are mostly
menus are overlays too.

An overlay meets the rest of the game at two tables:

- At its start, the
  [entry table](../../glossary.md#entry-table-and-import-table): the functions
  the main image calls to ask the scene about itself
  ([Scenes](../scene/README.md#the-entry-table) explains each).
- At its end, the import table: one entry for each function of the main image
  the overlay calls. The overlay area is too far from the cartridge for a
  direct call, so each call goes through a veneer holding the address of the
  function's veneer in its bank's table, which stays put while the banks' code
  moves.

Every overlay keeps the same list of possible imports in the same order, and
takes from it only what it uses. As the game unpacks an overlay it also fixes
up the overlay's calls: on the cartridge each call is stored relative to the
start of the overlay, and the processor needs it relative to the call itself.

The entry table is `struct SceneEntryPoints`; the list of imports is
`overlay.inc`. Before each scene the main routine calls `Scene_LoadOverlay`,
whose `Overlay_Load` unpacks the overlay the scene table names; the packer is
`tools/ags-overlays`.

## The resource directory and table order

When the party enters Bilibin, the scene table doesn't say where Bilibin's code
is; it gives a number, which picks a row of the
[resource directory](../../glossary.md#resource-directory), and the row says
where the packed data starts. Pictures, maps and tables are found the same
way. A resource's number is its place in the list, which keeps the lookup to
one step but means a row added in the middle renumbers everything after it.

Code names a resource by a symbol, which follows a moved row, but in a few
places the numbers travel bare: the scene table names each scene's code by
number, and some code counts on from one resource to the next (the maps, and
the eight pictures of the settings menu's marker). The directory ends with 25
empty rows nothing reads, where a row fits without renumbering any other.

The directory is `Resource_DirectoryTable` in `resource/directory.s`, read by
`Resource_GetTableEntry`; each row defines its `ResourceId_` symbol.

## The addresses that never move

Some code says "the sound mixer is at this address" rather than "wherever the
linker put the sound mixer". [The original](../../glossary.md#the-original)
holds 24 such fixed addresses, so the matching build compiles the same
numbers, and the memory map cannot move what they point at: the cartridge's
first instruction, eleven routines copied into IWRAM, the map and decode
buffers that follow the overlay area, the sound mixer, the end of the IWRAM
heap, the stack [`wait()`](../../glossary.md#wait) runs its
[frames](../../glossary.md#frame) on, and three IWRAM variables whose German
address differs. Each is defined once, with why it cannot move, and the check
build with a current GCC (`make modern`) reaches each by its name instead.

They are the `FIXED_ADDRESS` lines of `include/ram_buffer.h`,
`include/iwram_call.h` and `include/cartboot.h`.

## Design

Banks are not needed for reach: a Thumb `bl` reaches 4 MB either way and the
main image is 991 KB (just under 1 MB, in the English link map). They exist so
a bank can grow without moving an address another bank holds. Overlays pay
for the same independence with two veneers per call.

## How it fits with the rest of the game

[Runtime](../runtime/README.md) copies the resident code into IWRAM at
power-on and lends memory from the two heaps laid out here. The main routine
loads each scene's overlay by the number the scene table gives
([Running a scene](../overworld/README.md#the-main-routine)), and every overlay in
[Scenes](../scene/README.md), [Menus](../menu/README.md) and
[Debug scenes](../debug/README.md) is built from the entry and import tables
described here. Code everywhere loads its pictures and maps through the
resource directory, and [saving](../runtime/README.md#saving) writes through
the flash library in `save/`.

## Limits

- An overlay has the 32 KB of the overlay area, and a bank the space up to
  the next bank's fixed start.
- Nothing keeps either heap out of the buffers above it.
- A far-call table's entries keep their order: an entry added in the middle
  would move every veneer after it.
- The resource directory ends with 25 empty rows nothing reads; a row added
  anywhere else renumbers every row after it.

## Words used here

- **resident code**: code copied into IWRAM at power-on or when sound starts.

## Evidence

The names follow what each piece does in the link: the far-call tables are
named for the bank they open, the veneers for their target with `Far` added, and
the overlays by their directory number. `IwramIrqMain` and the other resident
routines are named from their code, and the fixed addresses come from the
literals in the ROM's code. No build step compares a `FIXED_ADDRESS` literal
with its symbol's linked address; a one-off comparison of every one of them with
the six Broken Seal linker maps found every literal equal to its symbol in
each edition its `#if` selects.

The heaps' high points (how far each heap reached) were measured in the
English edition in libmgba, with the heap's
next address read at every frame and every release: a Goma Cave battle, the
field menu, the Item screen, field [Psynergy](../../glossary.md#psynergy-and-pp),
a shop, an inn, a talk, the world map and its still picture, the credits and
sleep mode.
