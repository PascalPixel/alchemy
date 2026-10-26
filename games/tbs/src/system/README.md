# System

Every [scene](../../glossary.md#scene) the [party](../../glossary.md#the-party)
visits needs its own code, [map](../../glossary.md#map) and pictures,
and a Game Boy Advance has 288 KB of working memory against a cartridge that
holds the whole game. The system folder is what makes that fit: it decides
which part of memory holds what, splits the program into
[banks](../../glossary.md#bank) that call each other through fixed tables,
swaps each scene's code in and out of one small area, and finds every picture,
map and piece of code on the cartridge by number. Nearly all of it is
assembly: tables the linker lays out, and code no C
[compiler](../../glossary.md#the-compilers) emits, each file naming in its
header the instruction that shows it.
[Runtime](../runtime/README.md) builds
directly on it, and [Scenes](../scene/README.md) are the main users of its
[overlays](../../glossary.md#overlay).

## Memory map

The console has two kinds of working memory. EWRAM, 256 KB, is large but
slower and holds most of the game's data: its variables, the current scene's
code, the map's grid and pictures being unpacked, and the larger
[heap](../../glossary.md#heap-and-pools). IWRAM, 32 KB, is fast and holds
what must run or be read quickly: routines copied from the cartridge at
power-on, the busiest variables, a second heap and the stacks. Video memory
holds the tiles and tile maps the screen is drawn from, and the processor runs the
program straight from the cartridge.

Each area has a fixed place, the same in every
[edition](../../glossary.md#editions) except for three IWRAM variables German
moves:

| EWRAM, from | Size | What |
| --- | --- | --- |
| 0 KB | 32 KB | the game's variables: the save block, the [game flags](../../glossary.md#game-flag), the sound driver's work |
| 32 KB | 32 KB | the current scene's code, `gOverlayArea` |
| 64 KB | 96 KB | the map grid's [cells](../../glossary.md#cell), the battle figures' pictures, unpacking space, the map's [metatiles](../../glossary.md#metatile) and its animated tiles |
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

Both heaps can run past their space into the buffers above them, since
nothing checks; in play they do, without harm, because nothing is using those
buffers at the time (the measurements are on `gEwramHeap` and `gIwramHeap`).
`main.ld` declares the areas, and the English link map,
`out/tbs-en/tbs-en.map`, has every address.

### Video memory

The screen is built from 8 by 8 pixel tiles. A
[background layer](../../glossary.md#background-layer) is a sheet of tiles plus
a screen map saying which tile goes where; a
[sprite](../../glossary.md#sprite-and-oam) draws its own tiles wherever it is
put. On a scene, video memory holds the windows' tiles first, then the windows'
screen map and the three map layers' screen maps, then the map's three
sheets of tiles, and in its last 32 KB the sprites' tiles, which a cache
hands out in blocks of 64 bytes. The credits divide the background memory in
two halves instead ([Graphics](../graphics/README.md)).

The window tiles are placed by `Window_LoadGraphics`, the map's by
`MapGrid_LoadMap` ([Maps](../map/README.md)), and the sprite tiles
by the cache in `runtime/vram_cache.c`.

## Why the program is cut into banks

The main image, the program the processor runs from the cartridge, is linked
in banks: the field, the windows, the battles, the sound and the rest. Each
starts at a fixed address with room to spare and opens with a table of short
jumps, one for each function other banks call. A call from another bank goes
to the jump, which goes on to the function wherever it is, so when a bank's
code grows or shrinks no other bank has to change.

A table's entries keep their order. Adding one in the middle would move every
later entry, and code elsewhere that reads the table by position would go
wrong. The runtime bank is the exception to the rule: other banks call its
functions directly, and its table serves only the scenes' code.

A caller uses the jump's name, the function's with `Far` added:
`Sound_PlaySongFar` reaches `Sound_PlaySong`. Each jump is the
`overlay_veneer` macro in `overlay.s`; each bank's table is a file in
`far_call/`, and [Source](../README.md#banks-and-folders) lists the banks.

## How an overlay reaches the rest of the game

A town's people, events and puzzles matter only while the party is there,
and all the scenes' code together would not fit in memory. So each scene's code
is packed on the cartridge as an overlay and unpacked, when the party
arrives, into the 32 KB overlay area, over the scene before. Every overlay is
built to run at that one address, so only one is ever loaded and an overlay
never calls another; code several scenes need is copied into each. There are
96 overlays: several scenes can share one, and the title screen and a few
screens that are mostly menus are overlays too.

An overlay meets the rest of the game at two tables:

- At its start, the
  [entry table](../../glossary.md#entry-table-and-import-table): the six
  questions the field asks a scene, for its set-up and for its
  [entrances](../../glossary.md#entrance),
  [exits](../../glossary.md#exit), [actors](../../glossary.md#actor),
  [map events](../../glossary.md#map-event) and exit regions (the areas of the
  scene that lead out).
- At its end, the import table: one entrance for each function of the main
  image the overlay calls. The overlay area is too far from the cartridge for
  a direct call, so each call goes through an entrance holding the function's
  full address in the bank tables, which stays put while the banks' code
  moves.

Every overlay keeps the same list of possible imports in the same order, and
takes from it only what it uses. As the game unpacks an overlay it also fixes
up the overlay's calls: on the cartridge each call is stored relative to the
start of the overlay, and the processor needs it relative to the call itself.

The entry table is `struct SceneEntries`; the list of imports is `overlay.inc`;
`Overlay_Load` unpacks an overlay, and the packer is `tools/ags-overlays`.

## The resource directory and table order

When the party enters Bilibin, the scene table doesn't say where Bilibin's code
is; it gives a number, which picks a row of the resource directory, and the
row says where the packed data starts. Pictures, maps and tables are found
the same way. A resource's number is its place in the list, which keeps the
lookup to one step but means a row added in the middle renumbers everything
after it.

Code names a resource by a symbol, which follows a moved row, but in a few
places the numbers travel bare, without a symbol: the scene table names each
scene's code by number, and some code counts on from one resource to the next
(the maps, and the frames of the option menu's marker). The directory ends
with 25 spare rows, where a row fits without renumbering any other.

The directory is `Resource_DirectoryTable` in `resource/directory.s`, read by
`Resource_GetTableEntry`; each row defines its `ResourceId_` symbol.

## The addresses that never move

Some code says "the overlay area is at this address" rather than "wherever
the linker put the overlay area". [The original](../../glossary.md#the-original)
program holds 27 such fixed addresses, so the matching build compiles the same
numbers, and the memory map cannot move what they point at: the cartridge's
first instruction, the routines copied into IWRAM, the overlay area, the scene
and decode buffers after it, the ends of the two heaps, the sound mixer, the
frame wait's stack and the interrupt address. Each is listed once, with why
it cannot move, and the check build with a current GCC (`make modern`)
reaches each by its name instead.

They are the `FIXED_ADDRESS` lines of `include/ram_buffer.h`,
`include/iwram_call.h` and `include/cartboot.h`.

## Design

The banks and their tables buy independence, not reach: a call can reach 4 MB
either way, and the main image is a little over 1.5 MB. Another bank knows
only where a bank starts and the place of each export in its table, so a bank
can grow inside its gap without moving any address other code holds.

Overlays depend on the main image through the same tables: an import holds a
bank table entry's address, which moves only when a table changes, at the
price of two jumps per call. In the other direction, the engine knows an
overlay only through the entry table's six questions. The fixed addresses are
the cost of code that holds numbers rather than symbols; each is defined once,
so `make modern` builds from the names while the matching build keeps the
numbers.

## How it fits with the rest of the game

[Runtime](../runtime/README.md) copies the resident code into IWRAM at
power-on and lends memory from the two heaps laid out here. The field loads
each scene's overlay by the number the scene table gives
([Running a scene](../overworld/README.md)), and every overlay in
[Scenes](../scene/README.md), [Menus](../menu/README.md) and
[Debug rooms](../debug/README.md) is built from the entry and import tables
described here. Code everywhere loads its pictures and maps through the
resource directory, and [saving](../runtime/README.md#saving) writes through
the flash library in `save/`.

## Limits

- An overlay has the 32 KB of the overlay area, and a bank the space up to
  the next bank's fixed start.
- Nothing keeps either heap out of the buffers above it.
- A far-call table's entries keep their order, because code in other banks
  and overlays calls them by place.
- The resource directory has 25 spare rows at its end.

## Words used here

- **bank** and **far call**: the main image is linked in banks. A function
  another bank calls is exported through its bank's far-call table and called
  by its `...Far` name; an entry no code calls by name keeps its place as a
  stub with no `...Far` name.
- **overlay**: code and data unpacked into the overlay area from one
  compressed resource: its entry table, its code, and its import table.
- **resource**: a row of the directory, loaded by its number.
- **resident code**: code copied into IWRAM at power-on or when sound starts.

## Evidence

The names follow what each piece does in the link: the far-call tables are
named for the bank they open, the stubs for their target with `Far` added, and
the overlays by their directory number. `IwramIrqMain` and the other resident
routines are named from their code, and the fixed addresses come from the
literals in the ROM's code. No build step compares a `FIXED_ADDRESS` literal
with its symbol's linked address; a one-off comparison of all 27 uses with the
six Broken Seal linker maps found every literal equal to its symbol in each
edition its `#if` selects.

The heaps' marks come from the English edition in libmgba, with the heap's
next address read at every frame and every release: a Goma Cave battle, the
field menu, the Item screen, field [Psynergy](../../glossary.md#psynergy-and-pp),
a shop, an inn, a talk, the world map and its still picture, the credits and
sleep mode.
