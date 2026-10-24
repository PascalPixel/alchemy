# System

Every [map](../../glossary.md#map) the [party](../../glossary.md#the-party)
visits needs its own code, [layout](../../glossary.md#layout) and pictures,
and a Game Boy Advance has 288 KB of working memory against a cartridge that
holds the whole game. The system folder is what makes that fit: it decides
which part of memory holds what, splits the program into
[banks](../../glossary.md#bank) that call each other through fixed tables,
swaps each map's code in and out of one small area, and finds every picture,
layout and piece of code on the cartridge by number. Nearly all of it is
assembly: tables the linker lays out, and code no C
[compiler](../../glossary.md#the-compilers) emits, each file naming in its
header the instruction that shows it.
[Runtime](../runtime/README.md) builds
directly on it, and [Maps](../map/README.md) are the main users of its
[overlays](../../glossary.md#overlay).

## Memory map

The console has two kinds of working memory. EWRAM, 256 KB, is large but
slower and holds most of the game's data: its variables, the current map's
code, the map grid and the [heap](../../glossary.md#heap-and-pools). IWRAM,
32 KB, is fast and holds what must run or be read quickly: routines copied
from the cartridge at power-on, the busiest variables, a second heap and the
stacks. Video memory (VRAM) holds the tiles and maps the screen is drawn
from, and the processor runs the program straight from the cartridge.

`main.ld` declares the areas. EWRAM's bytes are numbered from `0x02000000`
and IWRAM's from `0x03000000`, written in hexadecimal as the link map writes
them, so the tables also give each part's start in KB. The addresses are the
English edition's (`out/tbs-en/tbs-en.map`); the other
[editions](../../glossary.md#editions) agree except that German moves three
IWRAM variables ([below](#the-addresses-that-never-move)).

**EWRAM**, laid out by `ewram.s`:

| Starts at | Address | Size | What | Named in |
| --- | --- | --- | --- | --- |
| 0 KB | `0x02000000` | 32 KB | the main image's variables: the save buffer, the [game flags](../../glossary.md#game-flag) (`gGameFlagBytes`), the save block (`gSaveBlock`), the sound driver's and flash library's work | `ewram.s` |
| 32 KB | `0x02008000` | 32 KB | `gOverlayArea`, the loaded map or screen overlay | `ewram.s`, `overlay/overlay.ld` |
| 64 KB | `0x02010000` | 32 KB | `gMapGridCellBuffer`; battle animations also use it for particles | `Ram_MapGridCellBuffer` |
| 96 KB | `0x02018000` | 16 KB | `gActorSpriteSlots` | `Ram_EnemySpriteSlots` |
| 112 KB | `0x0201c000` | 16 KB | `gDecodeBuffer` | `Ram_DecodeBuffer` |
| 128 KB | `0x02020000` | 32 KB | `gMetatiles`, the layout's [metatiles](../../glossary.md#metatile) | `Ram_Metatiles` |
| 160 KB | `0x02028000` | 16 KB | `gMapGridAnimTiles` | `ewram.s` |
| 176 KB | `0x0202c000` | 16 KB | `gMapGridShapes` | `Ram_MapGridShapes` |
| 192 KB | `0x02030000` | 32 KB | `gEwramHeap`, which `heap.c` allocates from up to `0x02040000`, so past its 32 KB it runs into `gBgTileBuffer` | `ewram.s`, `heap.c` |
| 224 KB | `0x02038000` | 32 KB, to the end | `gBgTileBuffer` | `Ram_BgTileBuffer` |

**IWRAM**:

| Starts at | Address | What | Named in |
| --- | --- | --- | --- |
| 0 KB | `0x03000000` | the resident runtime, 5 KB copied from the ROM: the dispatcher `IwramIrqMain`, the handler table `gIrqHandlers`, the resident sound mixing code and small routines the rest of the game calls | `main.ld` (`.iwram_runtime`), `runtime*.s` |
| 5 KB | `0x03001400` | the main image's IWRAM variables, among them the [heap slots](../../glossary.md#heap-slot) `gHeapSlots` (`0x03001e50`) | `iwram.s`, `Ram_HeapSlots` |
| 8 KB | `0x03002000` | `gIwramHeap`, which `heap.c` allocates from below `Ram_IwramHeapEnd` | `iwram.s` |
| 28 KB | `0x03007000` | the sound driver's mixer, `SoundMainRAM`, 932 bytes copied by `m4aSoundInit` | `Ram_SoundMixer`, `main.ld` (`.iwram_mixer`) |
| 30 KB | `0x03007800` | `Ram_IwramHeapEnd`, the end of the IWRAM heap, where `gCartridgeResetMarker` sits | `iwram_hi.s` |
| 30.5 KB | `0x03007a00` | `Ram_FrameWaitStackTop`, the stack top `WaitFrames` runs from | `iwram_hi.s` |
| the last 256 and 96 bytes | `0x03007f00`, `0x03007fa0` | the system-mode and IRQ-mode stack tops `System_Entry` sets | `entry.s` |
| the last 4 bytes | `0x03007ffc` | `gIrqVector`, where the BIOS jumps on an interrupt | `entry.s`, `iwram_hi.s` |

### Video memory

The screen is built from 8 by 8 pixel tiles. A
[background layer](../../glossary.md#background-layer) is a sheet of tiles plus
a screen map saying which tile goes where; a
[sprite](../../glossary.md#sprite-and-oam) draws its own tiles wherever it is
put. On a map, video memory holds the windows' tiles first, then the windows'
screen map and the three map grid layers' screen maps, then the layout's three
sheets of tiles, and in the last 32 KB the sprites' tiles.

The hardware addresses it in fixed blocks (`games/tbs/include/system/io_reg.h`):
background tiles in four 16 KB character blocks (`BG_CHAR_BLOCK(n)`) and
background maps in 2 KB screen blocks (`BG_SCREEN_BLOCK(n)`), in the same
64 KB. `Window_LoadGraphics` puts the window tiles at the start of character
block 0; the window tilemap is screen block 4 (`WINDOW_TILEMAP_VRAM`), with the
text canvas tiles after it (`TEXT_CANVAS_VRAM`). `MapGrid_LoadLayout` puts the
three layers' maps in screen blocks 5 to 7 and the tilesets in character blocks
1 to 3 ([Layouts](../layout/README.md)). The credits split the background in
two halves of 128 colours each ([Graphics](../graphics/README.md)). Sprite
tiles take the last 32 KB (`OBJ_VRAM0`), which the sprite VRAM cache hands out
as 512 blocks of 64 bytes (`runtime/vram_cache.c`).

## Why the program is cut into banks

The main image, the program the processor runs from the cartridge, is linked
in banks: the field, the windows, the battles, the sound and the rest. Each
starts at a fixed address with room to spare and opens with a table of short
jumps, one for each function other banks call. A call from another bank goes
to the jump, which goes on to the function wherever it is, so when a bank's
code grows or shrinks no other bank has to change.

`main.ld` puts each bank at an address written per edition. The banks are
`rom_main_00` to `rom_main_13` in `objects.tsv` and the linker scripts
([Source](../README.md#banks-and-folders) lists them), and the resource bank's
table opens `rom_tail_01`, after the sound code and the music. A caller uses
the jump's `...Far` name: `Sound_PlaySongFar` reaches `Sound_PlaySong`. The
runtime bank is the exception: other banks call its functions directly, and
its table, `FarCall_RuntimeTable` (`WaitFramesFar` for `WaitFrames`), right
after the cartridge header, serves the overlays. Why is an open question in
[Engine](../../engine.md#open-questions).

A table's entries keep their order. A row added in the middle moves every
later entry, and the bank's code, by 8 bytes: callers follow through their
symbols, but every caller of a later entry changes its bytes, and code that
reads a table by place goes wrong. `Transform_UpdateVertices`
(`games/tbs/src/field/vertices.c`) calls `ArcTan2`, `Trig_Sin` and `Trig_Cos`
as members of `FarCall_RuntimeTable` at byte offsets 64, 88 and 96, which its
`STATIC_ASSERT`s check.

Each jump is the `overlay_veneer` macro in `overlay.s`: `ldr r4` of the target
from the word after it, then `bx r4`. It may use r4 freely because every game
C file is compiled with `-fcall-used-r4`, so no caller expects r4 to survive a
call, and `bx` switches to ARM for the IWRAM routines. Most of German's bank
starts fall on 256-byte steps, the other editions' on 2 or 4 KB ones.

| Name | Where | Meaning |
| --- | --- | --- |
| `overlay_veneer` | `overlay.s` | the 8-byte stub: `ldr r4` of the target, `bx r4` |
| `far_call/*.S` | first row of each `rom_main_NN` from 01 on | a bank's far-call table, one stub per export (`stub.inc`) |
| `FarCall_RuntimeTable` | after the cartridge header | the runtime bank's table |
| `...Far` | each table entry | the name another bank or an overlay calls |

## How an overlay reaches the rest of the game

A town's people, cutscenes and puzzles matter only while the party is there,
and all the maps' code together would not fit in RAM. So each map's code is
packed on the cartridge as an overlay and unpacked, when the party arrives,
into `gOverlayArea`, 32 KB of EWRAM, over the map before. Every overlay is
built to run at that one address, so only one is ever loaded and an overlay
never calls another; code several maps need is copied into each
(`games/tbs/src/map/common/`).

There are 96 overlays, placed by `overlay/overlays.s` and named in the map
table (`field/map_headers.s`); several maps can share one (the three
`VaultCave` maps do), and the title screen and a few screens that are mostly
menus are overlays too. Each is linked on its own at the overlay area
(`overlay/overlay.ld`) against the main image's symbols, and is three objects:

- The [entry table](../../glossary.md#entry-table-and-import-table)
  (`map/entry/<map>.s`): six 8-byte stubs for the map's start-up and the
  functions that return its [warp destinations](../../glossary.md#warp-destination),
  [warps](../../glossary.md#warp), [object event](../../glossary.md#object-event)
  templates, [map events](../../glossary.md#map-event) and warp regions (boxes
  on the map that take a warp). The field calls them through
  `struct MapEntries` (`include/map_header.h`), as
  `gOverlayArea.initialize()`, `gOverlayArea.events()` and so on.
- The overlay's code and data.
- The import table (`map/import/<map>.s`). EWRAM is out of `bl` range of the
  cartridge, so the overlay makes a short `bl` to a local stub whose word holds
  the main-image function's far-call entry: `WaitFrames` goes to
  `WaitFramesFar`, which goes to `WaitFrames`.

The rows an overlay may import, and their order, are fixed in `overlay.inc`:
the four IWRAM divide routines first (`IwramSignedDivide` and the rest), which
the stubs reach directly, then the far-call entries in main-image order. A
stub takes the name of what it reaches, except a few whose callers read the
result differently (`Import_RandomNext` reaches `Random16`, but the maps read
its result as an `int`); `script_layer.h` lists them.

`Overlay_Load` (`../runtime/runtime/system.c`) unpacks an overlay and runs
`Resource_PatchThumbBranch` over it: in the packed stream each `bl` holds its
target's offset from the start of the overlay, and the patch turns that into
the offset from the instruction, which is what the processor expects. The
packer, `tools/ags-overlays`, does the reverse.

`struct MapEntries` reads each entry's [veneer](../../glossary.md#veneer) as
the address word in its last four bytes, so member `i` sits at `8 * i + 4`:

| Member | Offset | What it returns |
| --- | --- | --- |
| `initialize` | `0x04` | runs the map's start-up |
| `warpDests` | `0x0c` | the map's warp destinations (`struct MapWarpDest`) |
| `warps` | `0x14` | its warps |
| `objectEventTemplates` | `0x1c` | its object event templates (`struct ObjectEventTemplate`) |
| `events` | `0x24` | its events (`struct MapEvent`) |
| `regions` | `0x2c` | its warp regions (`struct MapWarpRegion`) |

## The resource directory and table order

When the party enters Bilibin, the map table doesn't say where Bilibin's code
is; it gives a number, which picks a row of the resource directory, and the
row says where the packed data starts. Pictures, layouts and tables are found
the same way. A resource's number is its place in the list, which keeps the
lookup to one step but means a row added in the middle renumbers everything
after it.

Every resource is a row of `Resource_DirectoryTable` (`resource/directory.s`),
asked for by number (`Resource_GetTableEntry`); the `resource` macro defines
`ResourceId_NAME` as the row's place. Moving a row still builds, since code
naming a resource by symbol follows, but the ROM no longer matches, and
numbers travel without symbols in several places. The directory ends with 25
unused rows (`unused 25`), where a row fits without renumbering any other.

| Where a number travels without its symbol | Why its rows keep their order |
| --- | --- |
| the map table, `field/map_headers.s` | each map names its overlay as a resource number |
| `objects.tsv`'s first column | each overlay's rows are numbered by resource |
| `field/layouts.c` | numbers counted from `ResourceId_Layout001` (`MapGrid_LoadLayout`) |
| `window/window/options.c` | the option menu's marker steps through eight [frames](../../glossary.md#frame) from `ResourceId_MenuCursorFrames` |

## The addresses that never move

A fixed address is the code saying "the overlay area is at this address"
rather than "wherever the linker put `gOverlayArea`". [The
original](../../glossary.md#the-original) program holds 27 of them, so the
matching build compiles the same numbers and the memory map cannot move what
they point at. `include/ram_buffer.h`, `include/iwram_call.h` and
`include/cartboot.h` list each once with `FIXED_ADDRESS(type, address,
linked)`: the matching build compiles the number, and `make modern`, the check
build with a current GCC, the linked name (`FIXED_LINKED_` in
`include/types.h`).

| What | Where | Why it can't move |
| --- | --- | --- |
| The cartridge restart entry, `Cart_Restart` | `0x08000000`, the start of the ROM | The soft reset in `WaitFrames` jumps there, and `SerialTest_Run`, a link cable test nothing reaches, sends the first 640 bytes from there. |
| The resident runtime, `.iwram_runtime` (5 KB, copied by `System_InstallIwramAndIrqs`) | `0x03000000` | C calls its routines at their addresses (`IwramSqrt` and the rest), and ARM code running from RAM reaches them as offsets from `IwramIrqMain` (`IWRAM_ENTRY`). |
| The overlay area, `gOverlayArea` | `0x02008000`, 32 KB | Every overlay is linked to run there; its first 48 bytes are the entry table. |
| The map and decode buffers, from `Ram_MapGridCellBuffer` to `Ram_BgTileBuffer` | after the overlay area | Code reaches across their boundaries by arithmetic: the map grid's [cells](../../glossary.md#cell) fill 64 KB from `Ram_MapGridCellBuffer`, through the sprite slots and the decode buffer. |
| The ends of the two heaps | `0x03007800` and `0x02040000` | The allocators test against these numbers (`heap.c`). |
| The sound driver's mixer, `SoundMainRAM` (932 bytes) | `0x03007000` | `m4aSoundInit` copies 1 KB there at once (`SOUND_MIXER_CODE_SIZE`), so the 92 bytes after the mixer in the ROM land just after it. |
| The frame wait's stack top, `Ram_FrameWaitStackTop` | `0x03007a00` | The 512 bytes below it are what the frame loop, its [tasks](../../glossary.md#task) and the VBlank work always have. |
| The stacks and `gIrqVector` | the top of IWRAM | `System_Entry` sets the stacks there, and the BIOS jumps through `gIrqVector`. |
| `gBgScroll`, `gVramCacheEntries` and `gHeapSlots` | among the IWRAM variables | The code reaches them as constants; German moves all three 16 bytes up, so `ram_buffer.h` chooses them per edition. |

## How far the heaps run past their space

The allocators test only against each heap's end, so nothing stops a heap
running into the buffers above its own space, and in play both do, without
harm anyone has seen. These marks come from the English edition in libmgba,
with the heap's next address read at every frame and every release: a Goma
Cave battle, the field menu, the Item screen, field
[Psynergy](../../glossary.md#psynergy-and-pp), a shop, an inn, a talk, the
world map and its still picture, the credits and sleep mode.

- In battle the IWRAM heap runs into the sound driver's mixer: with the heap
  at `0x0300667c`, `Sprite_ComposeFrame` takes a 4 KB pixel buffer, 1,660 bytes
  of it over `SoundMainRAM`, and clearing it overwrites the mixer. Nothing
  needs the mixer there: the VBlank handler mixes through `Mixer_Enter` and
  the resident code at `IwramSoundMixEntry`, and through a whole battle the
  code at `0x03007000` never runs.
- The EWRAM heap runs into `gBgTileBuffer`, to `0x02038e60` in the field and
  `0x0203d960` in battle. Only the world map uses that buffer, and there the
  heap reaches one word into it while the still map picture is open.

## Design

The banks and their tables buy independence, not reach: a Thumb `bl` reaches
4 MB either way, and the main image is a little over 1.5 MB from the start of
the ROM to the resource bank's table. Another bank knows only where a bank
starts and the place of each export in its table, so a bank can grow inside
its gap without moving any address other code holds.

Overlays depend on the main image through the same tables: an import stub
holds a far-call entry's address, which moves only when a table changes, at
the price of two hops per call. In the other direction, the engine knows an
overlay only through the entry table's six members. The fixed addresses are
the cost of code that holds numbers rather than symbols; each is defined once
where its bytes are, so `make modern` builds from the names while the matching
build keeps the numbers.

## How it fits with the rest of the game

[Runtime](../runtime/README.md) copies the resident code into IWRAM at
power-on and allocates from the two heaps laid out here. The field loads each
map's overlay by the number the map table gives
([Running a map](../overworld/README.md)), and every overlay in
[Maps](../map/README.md), [Menus](../menu/README.md) and
[Debug rooms](../debug/README.md) is built from the entry and import tables
described here. Code everywhere loads its pictures and layouts through the
resource directory, and [saving](../runtime/README.md#saving) writes through
the flash library in `save/`.

## Limits

- An overlay has the 32 KB of the overlay area, and a bank the space up to
  the next bank's fixed start.
- The IWRAM heap ends just below the frame wait's stack and the EWRAM heap at
  the end of EWRAM; `AllocHeapSlot` takes IWRAM first.
- Nothing keeps either heap out of the buffers above it
  ([above](#how-far-the-heaps-run-past-their-space)).
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
- **resource**: a row of the directory, loaded by its id
  (`Resource_GetTableEntry`); the id is the `ResourceId_` symbol's value.
- **resident code**: code, mostly ARM, copied into IWRAM at power-up
  (`.iwram_runtime`) or when sound starts (`.iwram_mixer`).

## Evidence and open questions

The names follow what each piece does in the link: the far-call tables are
named for the bank they open, the stubs for their target with `Far` added, and
the overlays by their directory number. `IwramIrqMain` and the other resident
routines are named from their code, and the fixed addresses come from the
literals in the ROM's code. No build step compares a `FIXED_ADDRESS` literal
with its symbol's linked address; a one-off comparison of all 27 uses with the
six Broken Seal linker maps found every literal equal to its symbol in each
edition its `#if` selects.

- [XXX](../../glossary.md#xxx) (no evidence): why the overlay packer stores
  each call's target as an offset from the overlay's start; likely so the
  overlays compress better under LZ, since every call to one routine then has
  the same bytes, but the ROM holds only the packed result, so only Camelot's
  tools could say.
- XXX (no evidence): whether Camelot linked the banks separately; the fixed
  bank starts and the tables are what a separate link needs, but one link with
  fixed starts leaves the same bytes, so only Camelot's build could say.
- XXX (no evidence): whether the 25 unused directory rows were left for later
  resources; they are empty in the ROM and nothing reads them, so they show
  only that they are spare.
- XXX (no evidence): why the directory's first 19 rows point at pieces of the
  program: the start of the ROM (`Rom_Start`, row 0), the directory itself
  (row 1), the build stamp (row 2), and from row 3 every bank's far-call table
  but the runtime's, the sound and resource tables twice each. The build stamp
  is read through its row (`System_CopyBuildStamp`), but the far-call rows are
  only passed to stubs that do nothing (`Stub_NoOpHook`, `Stub_ReturnZero`),
  from the title screen, Lucky Dice, Lucky Wheels and the sprite viewer. Those
  calls look like a leftover of loading banks as resources, but The Lost Age's
  stubs are empty too and nothing in either ROM loads a bank through its row,
  so only Camelot's sources could say.
