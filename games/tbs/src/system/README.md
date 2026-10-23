# System

Every [map](../../glossary.md#map) the [party](../../glossary.md#the-party) visits needs its own code, its own [layout](../../glossary.md#layout) and its own
pictures, and a Game Boy Advance has little room for any of it: 288 KB of
working memory, against a cartridge that holds the whole game. The system
folder holds what makes that fit. It lays out which part of memory holds
what, splits the program into [banks](../../glossary.md#bank) that can call one another through fixed
tables, swaps each map's code in and out of one small area as the party
moves, and finds every picture, layout and piece of code on the cartridge by
number.

In more detail, the folder holds the cartridge header and `System_Entry`, the
RAM variables in their fixed order, the resident code copied into [IWRAM](../../glossary.md#iwram-and-ewram), the
[far-call](../../glossary.md#far-call) tables that join the banks of the [main image](../../glossary.md#main-image), the [overlays](../../glossary.md#overlay) that hold
each map's code, the resource directory every picture, table and overlay is
found through, and the flash library that writes saves. Nearly all of it is
assembly: tables the linker lays out, and code no C [compiler](../../glossary.md#the-compilers) emits. Since
the source must compile back into the cartridge's exact bytes (a [matching
decompilation](../../glossary.md#matching-decompilation)), each assembly file names in its header the instruction or
pattern that shows no compiler wrote it. [Runtime](../runtime/README.md) builds
directly on it, and [Maps](../map/README.md) are the main users of its
overlays.

## Memory map

The memory map says where each part of the game's state lives.

The console has two kinds of working memory. EWRAM, 256 KB, is large but
slower, and holds most of the game's data: its variables, the current map's
code, the map grid and the [heap](../../glossary.md#heap-and-pools). IWRAM, 32 KB, is fast, and holds what has to run
or be read quickly: routines copied from the cartridge at power-on, the
engine's busiest variables, a second heap and the stacks. Separately, video
memory (VRAM) holds the tiles and maps the screen is drawn from, and the
cartridge itself holds the program, which the CPU runs straight from it, and
everything packed that the game loads.

`main.ld` declares these areas. Each byte of memory has an address, a number
the processor uses to name it; EWRAM's bytes are numbered from `0x02000000`
and IWRAM's from `0x03000000`. Addresses are written in hexadecimal (base
16, marked `0x`), as the link map and the source write them, so the tables
also say how far into the area each part starts, in KB. The addresses are
the English edition's, from its link map (`out/tbs-en/tbs-en.map`); the
other [editions](../../glossary.md#editions) agree except that German moves three IWRAM variables
([below](#the-addresses-that-never-move)).

**EWRAM**, laid out by `ewram.s`:

| Starts at | Address | Size | What | Named in |
| --- | --- | --- | --- | --- |
| 0 KB | `0x02000000` | 32 KB | the main image's variables: the save buffer, the [game flags](../../glossary.md#game-flag) (`gGameFlagBytes`), the save block (`gSaveBlock`), the sound driver's work and the flash library's | `ewram.s` |
| 32 KB | `0x02008000` | 32 KB | `gOverlayArea`, the loaded map or screen overlay | `ewram.s`, `overlay/overlay.ld` |
| 64 KB | `0x02010000` | 32 KB | `gMapGridCellBuffer`; battle animations also use it for particles | `Ram_MapGridCellBuffer` |
| 96 KB | `0x02018000` | 16 KB | `gActorSpriteSlots` | `Ram_EnemySpriteSlots` |
| 112 KB | `0x0201c000` | 16 KB | `gDecodeBuffer` | `Ram_DecodeBuffer` |
| 128 KB | `0x02020000` | 32 KB | `gMetatiles`, the layout's [metatiles](../../glossary.md#metatile) (`games/tbs/src/field/field/map_grid.c`) | `Ram_Metatiles` |
| 160 KB | `0x02028000` | 16 KB | `gMapGridLayerData` | `ewram.s` |
| 176 KB | `0x0202c000` | 16 KB | `gMapGridShapes` | `Ram_MapGridShapes` |
| 192 KB | `0x02030000` | 32 KB | `gEwramHeap`; `heap.c` allocates from it up to `0x02040000`, the end of EWRAM, so past its 32 KB it runs into `gBgTileBuffer` | `ewram.s`, `games/tbs/src/runtime/runtime/heap.c` |
| 224 KB | `0x02038000` | 32 KB, to the end | `gBgTileBuffer` | `Ram_BgTileBuffer` |

**IWRAM**:

| Starts at | Address | What | Named in |
| --- | --- | --- | --- |
| 0 KB | `0x03000000` | the resident runtime, 5 KB copied from the ROM: the dispatcher `IwramIrqMain`, the handler table `gIrqHandlers`, the resident sound mixing code and the small routines the rest of the game calls | `main.ld` (`.iwram_runtime`), `runtime*.s` |
| 5 KB | `0x03001400` | the main image's IWRAM variables, among them the [heap slots](../../glossary.md#heap-slot) `gHeapSlots` (`0x03001e50`) | `iwram.s`, `Ram_HeapSlots` |
| 8 KB | `0x03002000` | `gIwramHeap`, which `heap.c` allocates from below `Ram_IwramHeapEnd` | `iwram.s` |
| 28 KB | `0x03007000` | the sound driver's mixer, `SoundMainRAM`, 932 bytes copied from the ROM by `m4aSoundInit` | `Ram_SoundMixer`, `main.ld` (`.iwram_mixer`), `games/tbs/src/sound/audio.c` |
| 30 KB | `0x03007800` | `Ram_IwramHeapEnd`, the end of the IWRAM heap, where `gCartridgeResetMarker` sits | `iwram_hi.s` |
| 30.5 KB | `0x03007a00` | `Ram_FrameWaitStackTop`, the stack top `WaitFrames` runs from | `iwram_hi.s` |
| the last 256 and 96 bytes | `0x03007f00`, `0x03007fa0` | the system-mode and IRQ-mode stack tops `System_Entry` sets | `entry.s` |
| the last 4 bytes | `0x03007ffc` | `gIrqVector`, where the BIOS jumps on an interrupt | `entry.s`, `iwram_hi.s` |

### Video memory

Video memory holds the pictures the screen is drawn from, and a map fills
it with its own.

The screen is built from tiles, small 8-by-8 pictures. A [background layer](../../glossary.md#background-layer) is
a sheet of tiles plus a screen map that says which tile goes in each place on
screen; a [sprite](../../glossary.md#sprite-and-oam) draws its own tiles wherever it is put. Video memory is
split accordingly. On a map, the windows' tiles come first, then the
windows' screen map and the three map grid layers' screen maps, then the
layout's three sheets of tiles, and the last 32 KB holds the sprites' tiles.

The hardware addresses video memory in fixed blocks (`games/tbs/include/system/io_reg.h`):

- Background tiles go in four 16 KB character blocks (`BG_CHAR_BLOCK(n)`) and
  background maps in 2 KB screen blocks (`BG_SCREEN_BLOCK(n)`), sharing the
  same 64 KB. `Window_LoadGraphics` copies the window tiles to the start of
  character block 0. The window system's tilemap is screen block 4
  (`WINDOW_TILEMAP_VRAM`) and its text canvas tiles follow it (`TEXT_CANVAS_VRAM`,
  [`unit/window/window.h`](../../include/unit/window/window.h)).
  `MapGrid_LoadLayout` puts the ground, middle and overhead layers' maps in
  screen blocks 5, 6 and 7 and the layout's three tilesets in character
  blocks 1 to 3 ([Layouts](../layout/README.md)).
- Screens that load two pictures in turn split the background in halves: the
  first 32 KB with background colours 0 to 127, the second with colours 128 to
  255 (`Credits_LoadBackground`,
  `games/tbs/src/graphics/credits/credits.c`).
- Sprite tiles take the last 32 KB (`OBJ_VRAM0`), which the sprite VRAM cache
  hands out as 512 blocks of 64 bytes
  (`games/tbs/src/runtime/runtime/vram_cache.c`).

## Why the program is cut into banks

The program is linked in banks, each at a fixed place in the cartridge, so
that one bank's code can change size without disturbing any other.

The program the CPU runs straight from the cartridge, the main image, is
divided into banks: the field, the windows, the battles, the sound and the
rest. Each starts at a fixed address with room to spare after it, and opens
with a table of short entries, one for each of its functions that other banks
call. A call from another bank goes to the entry, which jumps on to the
function. When a bank's code grows or shrinks, its functions move, but the
table and every entry in it stay where they were, so no other bank has to
change.

`main.ld` puts each bank at an address of its own, written per edition,
and leaves the space up to that address empty. Each bank is a place in
`objects.tsv` and the linker scripts, `rom_main_00` to `rom_main_13`
([Source](../README.md#banks-and-folders) lists them), and the resource
bank's table opens `rom_tail_01`, after the sound code and the music.

Each bank opens with its far-call table, one 8-byte stub for each function
another bank calls. A caller in another bank calls the table's entry, by the
function's `...Far` name, rather than the function's own address:
`Sound_PlaySongFar` reaches `Sound_PlaySong`. The runtime bank is the
exception: the other banks call its functions directly, and its table,
`FarCall_RuntimeTable` (`WaitFramesFar` for `WaitFrames`), which follows the
cartridge header, serves the overlays. Why the runtime alone is called
directly is an open question in [Engine](../../engine.md#open-questions).

A table's entries keep their order. A row added in the middle moves every
later entry, and the bank's code, by 8 bytes. Callers follow through their
`...Far` symbols, but the bank's code moves towards the next bank's fixed
start, and once the gap is gone the link fails. Code that reads a table by
place goes wrong: `Transform_UpdateVertices` (`games/tbs/src/field/vertices.c`)
calls `ArcTan2`, `Trig_Sin` and `Trig_Cos` as members of `FarCall_RuntimeTable`
at byte offsets 64, 88 and 96, which its `STATIC_ASSERT`s check. And every
caller of a later entry, in every bank and overlay, changes its bytes, so no
edition compares OK. A row at the end of a table moves no other entry, though
the bank's code after it still shifts.

The stub is the `overlay_veneer` macro in `overlay.s`: `ldr r4` of the
target's address from the word after it, then `bx r4`. It may use r4 freely
because every game C file is compiled with `-fcall-used-r4` (the `Makefile`'s
`CFLAGS_tbs`), so no caller expects r4 to survive a call. `bx` switches to ARM
when the target is ARM code, so the IWRAM routines are reached through the
same stubs.

The bank starts differ by edition because each edition's banks hold different
amounts of code. Most of German's starts fall on 256-byte steps, where the
other editions' fall on 2 or 4 KB ones.

| Name | Where | Meaning |
| --- | --- | --- |
| `overlay_veneer` | `overlay.s` | the 8-byte stub: `ldr r4` of the target, `bx r4` |
| `far_call/*.S` | first row of each `rom_main_NN` from 01 on | a bank's far-call table, one stub per export (`stub.inc`) |
| `FarCall_RuntimeTable` | after the cartridge header | the runtime bank's table |
| `...Far` | each table entry | the name another bank or an overlay calls (`Sound_PlaySongFar`) |

## How an overlay reaches the rest of the game

An overlay is one map's code, unpacked into one fixed 32 KB area of EWRAM
while the party is there, and it meets the rest of the program only through
two tables of stubs.

The code for a town's people, its cutscenes and its puzzles is needed only
while the party is in that town, and all the maps' code together would not
fit in RAM. So each map's code is packed on the cartridge as an overlay, and
when the party enters a map, its overlay is unpacked into the overlay area
over whatever map's code was there before. Every overlay is built to run at that
one address, so only one is ever loaded.

There are 96 overlays, placed by `overlay/overlays.s` and listed in the map
table (`field/map_headers.s`), each a compressed resource unpacked into
`gOverlayArea`. Some maps share one (the three `VaultCave` maps load the
same overlay), and the title screen and a few other screens that are mostly
menus are overlays too.

An overlay is linked on its own at the overlay area (`overlay/overlay.ld`),
after the main image and against its symbols (`-R` in the `Makefile`'s overlay
rule). It is three objects:

- The [entry table](../../glossary.md#entry-table-and-import-table) (`map/entry/<map>.s`) opens it: six 8-byte stubs whose
  words hold the map's start-up function and the functions that return its
  [warp destinations](../../glossary.md#warp-destination), [warps](../../glossary.md#warp), [object event](../../glossary.md#object-event) templates, [map events](../../glossary.md#map-event) and warp
  regions (boxes on the map that take a warp). The field code reads them
  through `struct MapEntries` (`include/map_header.h`), calling
  `gOverlayArea.initialize()`, `gOverlayArea.events()` and so on.
- The overlay's code and data.
- The import table (`map/import/<map>.s`). EWRAM is out of `bl` range of
  the ROM, so the overlay's code makes a short `bl` to a local stub, and the
  stub's word holds the address of the main-image function's far-call entry,
  never the function itself: `WaitFrames` goes to `WaitFramesFar`, which
  goes to `WaitFrames`.

The rows an overlay may import, and their order, are fixed in `overlay.inc`:
the four IWRAM divide routines first (`IwramSignedDivide` and the rest), which
the stubs reach directly, then the far-call entries in the order of the main
image. Each import table sets `import.NAME` for the stubs its overlay calls
and includes that list, so every overlay keeps its imports in the same
relative order. A stub takes the name of the routine it reaches, so an
overlay calls `WaitFrames` or `ArcTan2` as the main image does. A few keep a
name of their own, where the maps' calls differ from the routine's prototype
(`Engine_RandomNext` reaches `Random16`, but the maps read its result as an
`int`); `script_layer.h` lists them.

Only one overlay is loaded at a time, since all are linked to the same 32 KB,
so an overlay never calls another. Code several maps need is copied into
each of them (`games/tbs/src/map/common/`).

`Overlay_Load` (`../runtime/runtime/system.c`) unpacks an overlay and
runs `Resource_PatchThumbBranch` over it. In the packed stream each `bl` holds
its target's offset from the start of the overlay, and the patch turns it into
the offset from the instruction itself, which is what the CPU expects. The
packer, `tools/ags-overlays`, does the reverse, so in the stream every call to
the same routine has the same bytes wherever it is made from. Why is an open
question ([below](#evidence-and-open-questions)).

`struct MapEntries` reads each entry's [veneer](../../glossary.md#veneer) as the address word in its
last four bytes, so member `i` sits at `8 * i + 4`:

| Member | Offset | What it returns |
| --- | --- | --- |
| `initialize` | `0x04` | runs the map's start-up |
| `warpDests` | `0x0c` | the map's warp destinations (`struct MapWarpDest`) |
| `warps` | `0x14` | its warps |
| `objectEventTemplates` | `0x1c` | its object event templates (`struct ObjectEventTemplate`) |
| `events` | `0x24` | its events (`struct MapEvent`) |
| `regions` | `0x2c` | its warp regions (`struct MapWarpRegion`) |

## The resource directory and table order

Every picture, layout, table and overlay on the cartridge is found by its number
in one list, the resource directory, so the list has to keep its order.

When the party enters Bilibin, the map table does not say where Bilibin's
code is; it gives a number. The number picks a row of the resource
directory, and the row holds where the packed data starts on the cartridge.
Pictures, layouts and tables are found the same way. A resource's number is
simply its place in the list, which keeps the lookup to one step, but it also
means a row added in the middle renumbers everything after it. The far-call
tables work the same way: an entry is found by its place.

Every resource is a row of `Resource_DirectoryTable` (`resource/directory.s`).
Code asks for a resource by its number (`Resource_GetTableEntry`), and the
`resource` macro defines `ResourceId_NAME` as that row's place.

Moving a row in the middle still builds, since code that names a resource by
its symbol follows, but every later row is renumbered, so the ROM no longer
matches, and numbers travel without symbols in several places: the map table (`games/tbs/src/field/map_headers.s`)
gives each map its overlay as a resource number, and the first column of
`objects.tsv` numbers each overlay's rows the same way. The layout table
(`games/tbs/src/field/layouts.c`) holds numbers counted from
`ResourceId_Layout001` (`MapGrid_LoadLayout`), so the rows after that one keep
their order. Code also adds to an id to reach the rows after it: the option
menu's marker steps through eight [frames](../../glossary.md#frame) from `ResourceId_MenuCursorFrames`
(`window/window/options.c`), so a run like that stays together. The directory
ends with 25 unused rows (`unused 25`), where a row fits without renumbering
any other.

| Where a number travels without its symbol | Why its rows keep their order |
| --- | --- |
| the map table, `field/map_headers.s` | each map names its overlay as a resource number |
| `objects.tsv`'s first column | each overlay's rows are numbered by resource |
| `field/layouts.c` | numbers counted from `ResourceId_Layout001` |
| `window/window/options.c` | eight frames counted from `ResourceId_MenuCursorFrames` |

## The addresses that never move

Some places in memory are reached as plain numbers written into the code, not
through names the linker fills in, so the layout must keep each one exactly
where the code expects it.

A fixed address is the code saying "the overlay area is at this address",
rather than "the overlay area is wherever the linker put `gOverlayArea`". [The
original](../../glossary.md#the-original) program holds 27 of them, and the matching build has to
compile the same numbers, so the memory map cannot move anything they point
at. Each is written down once, beside the name it stands for.

`include/ram_buffer.h`, `include/iwram_call.h` and `include/cartboot.h` list
them, each once, with `FIXED_ADDRESS(type, address, linked)`. The matching
build compiles the number, and `make modern`, the check build with a current GCC, compiles the linked name instead
(`FIXED_LINKED_` in `include/types.h`). Because the code holds these numbers
itself, the layout has to keep every one:

| What | Where | Why it can't move |
| --- | --- | --- |
| The cartridge restart entry, `Cart_Restart` | `0x08000000`, the start of the ROM | The soft reset in `WaitFrames` jumps there, and `SerialTest_Run`, a link cable test nothing reaches, sends the first 640 bytes from there over the cable. So the header and `System_Entry` stay at the start of the ROM. |
| The resident runtime, `.iwram_runtime` (5 KB, copied by `Runtime_InstallIwramAndIrqs`) | `0x03000000`, the start of IWRAM | C calls its routines at their addresses (`IwramSqrt` and the rest), and the ARM code that runs from RAM reaches them as offsets from `IwramIrqMain` (`IWRAM_ENTRY`, `games/tbs/src/field/update.c`). |
| The overlay area, `gOverlayArea` | `0x02008000`, 32 KB of EWRAM | Every overlay is linked to run there and has to fit. Its first 48 bytes are the entry table, which the field's code calls by member. |
| The map and decode buffers: `Ram_MapGridCellBuffer`, `Ram_EnemySpriteSlots`, `Ram_DecodeBuffer`, `Ram_Metatiles`, `Ram_MapGridShapes`, `Ram_BgTileBuffer` | after the overlay area, as the EWRAM table lists | Code reaches across their boundaries by arithmetic: the map grid's [cells](../../glossary.md#cell) fill 64 KB from the start of `Ram_MapGridCellBuffer`, through the sprite slots and the decode buffer, and the tileset animation reads tiles from `Ram_DecodeBuffer` that land in `gMapGridLayerData` ([Layouts](../layout/README.md)). So `ewram.s` can't reorder or resize them. |
| The ends of the two heaps | `0x03007800` in IWRAM and `0x02040000`, the end of EWRAM | The allocators test against these numbers (`heap.c`). `gCartridgeResetMarker` sits at the first, and the system stack lives above it. |
| The sound driver's mixer, `SoundMainRAM` (932 bytes) | `0x03007000`, inside the IWRAM heap's range | `m4aSoundInit` copies the mixer to that number (`Ram_SoundMixer`), and the driver runs it there. The copy is 1 KB at once (`SOUND_MIXER_CODE_SIZE`), so the 92 bytes after the mixer in the ROM land just after it. |
| The frame wait's stack top, `Ram_FrameWaitStackTop` | `0x03007a00` | The 512 bytes between it and the heap's end are what the frame loop, its [tasks](../../glossary.md#task) and the VBlank work always have ([Runtime](../runtime/README.md)). |
| The stacks and the interrupt vector, `gIrqVector` | the top of IWRAM | `System_Entry` sets the system and IRQ stacks there, and the BIOS jumps through `gIrqVector` on an interrupt. |
| `gBgScroll`, `gVramCacheEntries` and `gHeapSlots` (`Ram_HeapSlots`) | among the main image's IWRAM variables | The code reaches them as constants. German moves all three 16 bytes up, so `ram_buffer.h` chooses them per edition. |

## How far the heaps run past their space

Both heaps can grow into the memory above them, and in play both do, without
harm that anyone has seen.

The allocators test only against the heap's end, and so does
`Runtime_GetRemainingIwram`, which `Battle_Run` calls around every
action and then ignores. So nothing stops a heap running into the buffers
above its own space. The marks below come from the English edition played in
libmgba, an emulator library, with the heap's next address read at every
frame and every release: a Goma Cave battle, and the field menu, the Item
screen, field [Psynergy](../../glossary.md#psynergy-and-pp) cast from the menu and directly, a shop, an inn, a
talk, the world map and its still picture, the credits and sleep mode.

- In battle the IWRAM heap runs into the sound driver's mixer. With the
  heap's next address at `0x0300667c`, `Sprite_ComposeFrame` takes
  the 4 KB pixel buffer of a 64 by 64 sprite from `Alloc`, 1,660 bytes of
  it over `SoundMainRAM`, and clearing the buffer overwrites the mixer
  from its first word. Nothing copies the mixer back, and nothing needs it:
  the VBlank handler mixes through `Mixer_Enter` and the resident
  code at `IwramSoundMixEntry`, and through a whole battle neither
  `SoundMain` nor the code at `0x03007000` runs once.
- The EWRAM heap runs into `gBgTileBuffer`, to `0x02038e60` in the field and
  `0x0203d960` in battle. Only the world map uses that buffer, and there the
  heap reaches `0x02038004`, one word into it, while the still map picture is
  open.

## Design

The banks and their tables buy independence. Distance is not the reason: a
Thumb `bl` reaches 4 MB either way, and the main image, a little over 1.5 MB
from the start of the ROM to the resource bank's table, is within reach of
every bank. Another bank knows only where a bank starts and the place of each export
in its table, so a bank can grow or shrink inside its gap without moving any
address other code holds. How [Camelot](../../glossary.md#camelot) built them is an open question (at
the end of this page).

Overlays depend on the main image through the same tables. An import stub
holds a far-call entry's address, which moves only when a table changes, so an
overlay built against one layout of a bank's code still works after that code
moves; the price is two hops per call. In the other direction, the engine
knows an overlay only through the entry table's six fixed members. The engine
and an overlay meet only at those two tables, and both have a known order.

The fixed addresses are the cost of code that holds numbers rather than
symbols. Each is defined once and named where its bytes are, so `make modern`
can build from the names while the matching build keeps the numbers.

## How it fits with the rest of the game

The system folder is the ground the rest stands on. [Runtime](../runtime/README.md)
copies the resident code into IWRAM at power-on and allocates from the two
heaps laid out here. The field code loads each map's overlay by the number
the map table gives it ([Running a map](../overworld/README.md)), and every
overlay in [Maps](../map/README.md), [Menus](../menu/README.md) and
[Debug rooms](../debug/README.md) is built from the entry and import tables
described here. Code everywhere loads its pictures and layouts through the
resource directory, and [saving](../runtime/README.md#saving) writes through
the flash library in `save/`.

## Limits

- An overlay has the 32 KB of the overlay area, and a bank the space up to
  the next bank's fixed start.
- The IWRAM heap ends just below the frame wait's stack and the EWRAM heap at
  the end of EWRAM; `AllocHeapSlot` takes IWRAM first and EWRAM
  when IWRAM is full.
- Nothing keeps either heap out of the buffers above its own space; both
  run past it in play ([below](#how-far-the-heaps-run-past-their-space)).
- A far-call table's entries keep their order, because code in other banks
  and overlays calls them by place.
- The resource directory has 25 spare rows at its end.

## Words used here

- **bank** and **far call**: the main image is linked in banks (`runtime`,
  `field`, `window`, `game` and the rest). A function another bank calls is
  exported through its bank's far-call table and called by its `...Far` name.
  An entry no code calls by name keeps its place in the table, as a stub with
  no `...Far` name.
- **overlay**: code and data unpacked into the overlay area from one
  compressed resource when a map or screen needs it: its entry table, its
  code, and its import table.
- **resource**: a row of the directory, loaded by its id
  (`Resource_GetTableEntry`); the id is the `ResourceId_` symbol's value.
- **resident code**: code, mostly ARM, copied from the ROM into IWRAM at
  power-up (`.iwram_runtime`) or when sound starts (`.iwram_mixer`).

## Evidence and open questions

The names follow what each piece does in the link: the far-call tables are
named for the bank they open, the stubs for their target with `Far` added, and
the overlays by their directory number. `IwramIrqMain` and the other resident
routines are named from their code. The fixed addresses come from the literals
in the ROM's code.

No build step compares a `FIXED_ADDRESS` literal with its symbol's linked
address; `make modern` only builds with the name. A one-off comparison of all
27 `FIXED_ADDRESS` uses with the six Broken Seal linker maps found every literal equal
to its symbol in each edition its `#if` selects.

- [XXX](../../glossary.md#xxx) (no evidence): why the overlay packer stores each call's target as an
  offset from the overlay's start; likely so the overlays compress better
  under LZ, since every call to one routine then has the same bytes, but the
  ROM holds only the packed result, so only Camelot's tools could say.
- XXX (no evidence): whether Camelot linked the banks separately; the fixed
  bank starts and the tables are what a separate link needs, but one link
  with fixed starts leaves the same bytes, so only Camelot's build could say.
- XXX (no evidence): whether the 25 unused directory rows were left for later
  resources; they are empty in the ROM and nothing reads them, so they show
  only that they are spare.
- XXX (no evidence): why the directory's first 19 rows point at pieces of the program: the
  start of the ROM (`Rom_Start`, row 0), the directory itself (row 1), the
  build stamp (row 2), and from row 3 every bank's far-call table but the
  runtime's, the sound and resource tables twice each. The build stamp is read
  through its row (`System_CopyBuildStamp`, `game/game/units.c`), but the named far-call rows are
  only passed to stubs that do nothing (`RuntimeDispatch_NoOpHook`,
  `RuntimeDispatch_ReturnZero`), from the title screen, Lucky Dice, Lucky
  Wheels and the sprite viewer. Those calls look like a leftover of loading
  banks as resources (the comment in `menu/title.c` suggests as much), but
  The Lost Age's stubs are empty too, and nothing in either ROM loads a bank
  through its row, so only Camelot's sources could say.
