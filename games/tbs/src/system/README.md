# System

This folder holds what ties The Broken Seal's image together: the cartridge
header and entry, the RAM layout, the code resident in IWRAM, the BIOS calls,
the far-call tables between the banks of the main image, the overlays, the
resource directory and the flash save library. Nearly all of it is assembly:
handwritten code that no C compiler would produce, each file showing why in
its header, and the tables the linker lays out.

See also: [Runtime](../runtime/README.md), which stands on this folder, and
[Scenes](../scene/README.md), the overlays' main users.

## Files

| File | What it holds |
| --- | --- |
| `rom_head.s`, `entry.s` | the cartridge header and the entry that sets up the CPU modes and stacks |
| `ewram.s`, `iwram.s`, `iwram_hi.s` | the main image's RAM variables, in their order in EWRAM and IWRAM (the memory map below) |
| `runtime.s`, `runtime_irq_table.s`, `runtime_mix*.s`, `runtime_mul_q16_ip.s` | the code copied to IWRAM: the interrupt dispatcher, its handler table, the sound mixer and the fixed-point multiply |
| `iwram_entry.s` | the stubs that call the IWRAM code from the ROM |
| `cpu_set.s`, `sound_get_jump_list.s` | BIOS calls |
| `heapfree.s` | `Runtime_ReleaseHeapBlock` and `MemFree` |
| `updfar.s` | three ARM routines that share one literal pool |
| `syspal.s`, `memory/*.png` | the window palette and the basic sprite palettes, built from the PNGs |
| `overlay.s` | the far-call stub macro: 8 bytes that load a routine's address and jump to it |
| `far_call/*.S` | each bank's far-call table: one 8-byte jump per export, in a fixed order (`stub.inc`) |
| `overlay/` | the overlays: `overlays.s` places each one's packed stream as a directory resource, `overlay.ld` links each at 0x2008000, and the `ovl_*.s` hold the RAM variables of the overlays that have them |
| `overlay.inc` | the stub tables every overlay and far-call bank opens with |
| `resource/directory.s` | the resource directory: one entry per picture, table or overlay, each defining its `ResourceId_` name |
| `resource/*.s`, `resource/metadata_get_record.c`, `resource/colconv*.tsv` | the resource decoders, the colour conversion tables and the animation metadata lookup |
| `save/` | the flash save library: the chip drivers for Atmel, Macronix and SST flash (`FLASH_C`, compiled with pret's `old_agbcc`), and the save record signature (`savedata.c`) |

The unit headers are in `games/tbs/include/unit/system/`, the far-call
prototypes in `games/tbs/include/farcall/`. Link order, overlays included, is
in `games/tbs/objects.tsv`; `games/tbs/overlays.tsv` gives each overlay's
`.bss` header. `games/tbs/main.ld` places the main image's sections, and
`games/tbs/include/ram_buffer.h` names the fixed RAM addresses the code uses
as constants.

## Words

- **bank** and **far call**: the main image is linked in banks (`runtime`,
  `field`, `window`, `game` and the rest); a function another bank calls is
  exported through that bank's far-call table, and called by its `...Far`
  name. An entry with no name keeps its place in the table as a bare veneer.
- **overlay**: code and data loaded at 0x2008000 when a scene or a screen
  needs it, from one compressed resource. An overlay is its entry table, its
  code and its import table, the main-image functions it calls by name.
- **resource**: an entry of the directory, loaded by its id
  (`Resource_GetTableEntry`); the id is the `ResourceId_` symbol's value.
- **resident code**: ARM routines copied from the ROM into IWRAM at power-up
  (`.iwram_runtime`) or when sound starts (`.iwram_mixer`).

## Prefixes

| Prefix | What it acts on |
| --- | --- |
| `System_` | the cartridge entry |
| `Iwram` | the resident IWRAM code and its entry stubs (`IwramIrqMain`, `IwramHalt`, `IwramMulQ16`) |
| `Bios_` | the BIOS calls |
| `Resource_`, `ResourceId_` | the resource decoders and the directory's ids |
| `Flash_`, `EraseFlash`, `ProgramFlash` | the flash library, named as pret names it |
| `Runtime_`, `Sound_` | things the engine names elsewhere |

## Design

### Why the image is split into banks

`main.ld` does not let the main image's code flow from one object to the
next. It places each bank at an address of its own, written per edition
(`. = ABSOLUTE(...)` before each `rom_main_NN`), and leaves the space up to
that address empty. Each bank then opens with its far-call table: the first
row of every `rom_main_NN` place in `objects.tsv` is a `far_call/*.S` file.
A function that another bank calls is reached through its 8-byte entry in
that table, never at its own address.

Distance does not explain this. A Thumb `bl` reaches 4 MB either way, and
the whole main image, from 0x08000000 to the last bank's table below
0x08190000, is within reach of every bank. What the arrangement does buy is
independence: another bank knows only where a bank starts and the place of
each export in its table, so a bank can grow or shrink inside its gap
without moving a single address that other code holds. The gaps are of
different sizes in each edition, as each edition's code grew differently,
German most. XXX: the bytes show the fixed starts and the tables, not how
Camelot built the game; linking each bank on its own against the others'
tables is the likeliest reason, but not a proven one.

The stub itself (`overlay_veneer` in `overlay.s`) is `ldr r4, =target; bx
r4`. It can use r4 freely because every game C file is compiled with
`-fcall-used-r4` (the `Makefile`'s `CFLAGS_tbs`), so no caller expects r4 to
survive a call. `bx` also switches to ARM state when the target is ARM code,
which is why the IWRAM routines can be reached through the same stubs.

### Why an overlay calls only through its import table

An overlay runs from EWRAM at 0x2008000, 96 MB below the ROM, out of any
`bl`'s reach; every call it makes into the main image has to load an address
into a register. It does so through its import table, the last of its three
objects (`import/<place>.s`): the overlay's code makes a short `bl` to a
local stub, and the stub's word holds the main-image address. The overlay is
linked on its own at 0x2008000 (`overlay/overlay.ld`), after the main image
and against its symbols (`-R` in the `Makefile`'s overlay rule), so the
import table is where its calls into the main image are recorded.

The stub's word is not the function but its far-call entry: Vault imports
`Engine_TaskWait`, whose stub goes to `WaitFramesFar`, which goes to
`WaitFrames`. So an overlay holds only the places of far-call entries, which
move only when a table changes, and the two hops are the price. The rows an
overlay may import, and their order, are fixed in `overlay.inc`: Camelot's
four divide routines first, then the far-call entries in the order of the
main image. Each import table sets `import.NAME` for the stubs its overlay
calls and includes that list, so every overlay that imports a function keeps
it in the same relative order. Several local names may lead to one target
(`ArcTan2`, `Math_Atan2` and `CalculateFacingAngle` all reach `ArcTan2Far`),
because different overlays' code calls the same routine by different names.

The other direction is the entry table at the overlay's start
(`entry/<place>.s`): six veneers that the field object calls as
`gOverlayArea.initialize()`, `gOverlayArea.events()` and so on
(`struct SceneEntries`, `include/field_runtime.h`). The engine and an overlay
meet only at those two tables, each of known order.

An overlay never calls another overlay, because only one is ever loaded: all
of them are linked to the same 32 KB at 0x2008000. Code that several scenes
need is therefore copied into each of them, which is what
`games/tbs/src/scene/common/` holds.

When an overlay is loaded, `Resource_LoadCode` (`runtime/runtime/system.c`)
decompresses it into the overlay area and then runs
`Resource_PatchThumbBranch` over it, which turns each `bl` pair from a
stored position back into a displacement from the instruction. The packer,
`tools/ags-overlays`, does the reverse: in the stream, every call to the same
routine has the same bytes, wherever it is made from. XXX: that this was
done so that the overlays compress better under LZ is an inference from what
the filter does; nothing in the ROM says so.

### What breaks when a row is inserted

A far-call table is a list of places. Inserting a row moves every entry after
it by 8 bytes and the bank's own code after the table by 8 bytes too. In this
tree that is a relink, since callers use the `...Far` symbols, so the build
still works; but:

- the bank's code moves towards the next bank's fixed start, and if the gap
  is gone the link fails;
- code that reads a table by place stops working: `Transform_UpdateVertices`
  (`games/tbs/src/field/vertices.c`) calls `ArcTan2`, `Trig_Sin` and
  `Trig_Cos` as members of `FarCall_RuntimeTable` at offsets 64, 88 and 96,
  which its `STATIC_ASSERT`s check;
- every caller of a later entry, in every bank and overlay, changes its
  bytes, so no edition compares OK.

A resource id is a row number: `resource` in `directory.s` defines
`ResourceId_NAME` as the row's place. Code that names a resource by its
symbol follows a renumbering, but numbers travel without symbols in several
places, and those silently point at the wrong row:

- the scene table (`games/tbs/src/field/scenes.s`) gives each scene its
  overlay as a number (`scene Vault, 0x382, ...`), and the first column of
  `objects.tsv` numbers each overlay's rows the same way;
- the layered map table (`games/tbs/src/field/layered.tsv`) holds row
  numbers counted from `ResourceId_Map001` (`Map_LoadLayeredScene`), so the
  rows after that one must keep their order;
- code adds to an id to reach the rows after it (the window cursor's frames
  are `ResourceId_ShopCursorFrames` plus the frame, in
  `window/window/options.c`), so such runs must stay together.

The directory ends with 25 unused rows (`unused 25`), where a row can be added
without renumbering any other. XXX: whether they were left for that is not
known.

### The fixed addresses

Some places in memory are fixed: code reaches them as numbers, not through
the linker. `include/ram_buffer.h`, `include/iwram_call.h` and
`include/cartboot.h` list them, each once, with `FIXED_ADDRESS(type, address,
linked)`: the matching build compiles the number, and `make modern` compiles
the linked name instead (`FIXED_LINKED_` in `include/types.h`). These are the
invariants, and what follows from each:

1. **The cartridge restart entry is 0x08000000** (`Cart_Restart`). The soft
   reset in `WaitFrames` jumps there, and `SerialTest_Run` sends the first
   0x280 bytes from there over the link cable. The header and
   `System_Entry` stay at the start of the ROM.
2. **The resident runtime is at 0x03000000** (`.iwram_runtime`, 0x1400
   bytes), copied from the ROM by `Runtime_InstallIwramAndIrqs`. C calls its
   routines at their addresses (`IwramSqrt` at 0x030001d8 and the rest), and
   the ARM code that runs from RAM reaches them as offsets from
   `IwramIrqMain` (`IWRAM_ENTRY`, `games/tbs/src/field/update.c`). Nothing in
   that section may move.
3. **Every overlay runs at 0x02008000** (`gOverlayArea`) and must fit in the
   32 KB below 0x02010000. Its first 48 bytes are its entry table, which the
   field object calls by member.
4. **The map and decode buffers are at fixed places in EWRAM**
   (`Ram_MapCellBuffer` 0x02010000, `Ram_ActorSpriteSlots` 0x02018000,
   `Ram_DecodeBuffer` 0x0201c000, `Ram_MapBlocks` 0x02020000,
   `Ram_MapCollision` 0x0202c000, `Ram_BgTileBuffer` 0x02038000). Code
   reaches across their boundaries by arithmetic: the map's cells fill 64 KB
   from 0x02010000, through the sprite slots and the decode buffer (see
   [Maps](../map/README.md)), and the tile animation reads tiles from 0x600 on
   at `Ram_DecodeBuffer + n * 32`, which lands in `gMapLayerData`. So
   `ewram.s` may not reorder or resize them.
5. **The heaps end at 0x03007800 and 0x02040000.** The allocators test
   against these numbers (`heap.c`); `gCartridgeResetMarker` sits at the
   first, and the system stack lives above it.
6. **The sound mixer is at 0x03007000** (`.iwram_mixer`, 0x3a4 bytes),
   copied by `Audio_Initialize`; `Sound_LoadPresetParameters` copies its
   152-byte presets into the resident code at `IwramSoundPresetCode`
   (0x03000bd8), not into the mixer. The mixer lies inside the IWRAM heap's
   range, which runs from `gIwramHeap` (0x03002000) to 0x03007800; see the
   open questions.
7. **The frame wait's stack top is 0x03007a00** (`Ram_FrameWaitStackTop`):
   the 512 bytes between it and the heap's end are what the frame loop, its
   tasks and the VBlank work always have (see
   [Runtime](../runtime/README.md)).
8. **The stacks and the interrupt vector are at the top of IWRAM.**
   `System_Entry` sets the system stack to 0x03007f00 and the IRQ stack to
   0x03007fa0, and the BIOS jumps through 0x03007ffc (`gIrqVector`) on an
   interrupt.
9. **A few IWRAM variables are reached as constants**: `gBgScroll`,
   `gVramBlockCache` and `gWorkSlot` (`Ram_WorkSlot`, 0x03001e50). German
   moves all three 16 bytes up, so `ram_buffer.h` chooses them per edition.

## How memory is laid out

The addresses are tbs-en's, from its link map (`out/tbs-en/tbs-en.map`);
German moves a few IWRAM variables (`ram_buffer.h`).

**EWRAM**, 256 KB from 0x02000000 (`main.ld`), laid out by `ewram.s`:

| Address | Size | What | Named in |
| --- | --- | --- | --- |
| 0x02000000 | 32 KB | the main image's variables: the save buffer, the game flags (`gGameFlagBytes`), the game state (`gGameState`), the sound driver's work and the flash library's | `ewram.s` |
| 0x02008000 | 32 KB | the overlay area, `gOverlayArea`: the loaded scene or screen overlay | `ewram.s`, `overlay/overlay.ld` |
| 0x02010000 | 32 KB | `gMapCellBuffer`; battle effects also use it for particles | `Ram_MapCellBuffer`, `ram_buffer.h` |
| 0x02018000 | 16 KB | `gActorSpriteSlots` | `Ram_ActorSpriteSlots` |
| 0x0201c000 | 16 KB | `gDecodeBuffer` | `Ram_DecodeBuffer` |
| 0x02020000 | 32 KB | `gMapBlocks`, the map's blocks (`games/tbs/src/field/field/map_layers.c`) | `Ram_MapBlocks` |
| 0x02028000 | 16 KB | `gMapLayerData` | `ewram.s` |
| 0x0202c000 | 16 KB | `gMapCollision` | `Ram_MapCollision` |
| 0x02030000 | 32 KB | `gEwramHeap`, the EWRAM heap; `heap.c` allocates from it up to 0x02040000 | `ewram.s`, `games/tbs/src/runtime/runtime/heap.c` |
| 0x02038000 | 32 KB, to the end | `gBgTileBuffer` | `Ram_BgTileBuffer` |

**IWRAM**, 32 KB from 0x03000000 (`main.ld`):

| Address | What | Named in |
| --- | --- | --- |
| 0x03000000 | the resident runtime, 0x1400 bytes copied from the ROM: the interrupt dispatcher `IwramIrqMain`, the handler table `gIrqHandlers`, the fixed-point multiply and the mixer's helpers | `main.ld` (`.iwram_runtime`), `runtime*.s` |
| 0x03001400 | the main image's IWRAM variables, among them the heap slots `gWorkSlot` (0x03001e50) | `iwram.s`, `Ram_WorkSlot` |
| 0x03002000 | `gIwramHeap`, the IWRAM heap, which `heap.c` allocates from below `Ram_IwramHeapEnd` | `iwram.s` |
| 0x03007000 | the sound mixer, `SoundDriver_Mixer`, 0x3a4 bytes copied from the ROM by `Audio_Initialize` | `Ram_SoundMixer`, `main.ld` (`.iwram_mixer`), `games/tbs/src/sound/audio.c` |
| 0x03007800 | `Ram_IwramHeapEnd`: the end of the IWRAM heap, where `gCartridgeResetMarker` sits | `Ram_IwramHeapEnd`, `iwram_hi.s` |
| 0x03007a00 | `Ram_FrameWaitStackTop`: the stack `WaitFrames` runs on | `Ram_FrameWaitStackTop`, `iwram_hi.s` |
| 0x03007f00, 0x03007fa0 | the system-mode and IRQ-mode stack tops `System_Entry` sets | `entry.s` |
| 0x03007ffc | `gIrqVector`, where the BIOS jumps on an interrupt | `entry.s`, `iwram_hi.s` |

**VRAM**, from 0x06000000 (`games/tbs/include/system/io_reg.h`):

- BG tiles are four 16 KB character blocks (`BG_CHAR_BLOCK(n)`) and BG maps
  2 KB screen blocks (`BG_SCREEN_BLOCK(n)`) in the same 64 KB. The window
  system's tilemap is at 0x06002000 (`UI_TILEMAP_VRAM`, screen block 4) and
  its text canvas tiles at 0x06002500 (`UI_CANVAS_VRAM`,
  [`unit/window/window.h`](../../include/unit/window/window.h)).
- The BG halves: screens that load two pictures in turn use VRAM from
  0x06000000 with BG colours 0 to 127 as one half, and VRAM from 0x06008000
  with colours 128 to 255 as the other (`Graphics_LoadCharacterBlockAndPalette`,
  `games/tbs/src/graphics/credits/credits.c`).
- Sprite tiles are the 32 KB from 0x06010000 (`OBJ_VRAM0`), which the sprite
  VRAM cache hands out in 512 blocks of 64 bytes
  (`games/tbs/src/runtime/runtime/vram_cache.c`).

## How to change things

- A resource's id is its row's place in `directory.s`, so a row added in the
  middle renumbers every row after it (see what breaks, above). Add a row in
  place of one of the unused ones at the end, and name the resource by its
  symbol.
- A far-call export is added at the end of its bank's table, where it moves
  nothing.
- Matching: the markers and the build's check are in
  [Matching](../MATCHING.md).

## Limits

- An overlay has the 32 KB from 0x02008000 to 0x02010000.
- The IWRAM heap ends at 0x03007800 and the EWRAM heap at 0x02040000;
  `Runtime_AllocateHeapBlock` takes IWRAM first and EWRAM when IWRAM is
  full.
- A far-call table's entries keep their order: code in other banks calls
  them by place.

## Evidence and open questions

The names here follow what each piece does in the link: the far-call tables
are named by the bank they open, the stubs by their target with `Far` added,
and the overlays by their directory number. `IwramIrqMain` and the other
resident routines are named from their code. The fixed addresses come from
the literals in the ROM's code and are checked by building: the matching build
compiles them, `make modern` uses the linked names.

Still unknown:

- XXX: whether Camelot linked the banks separately, which the fixed bank
  starts suggest but do not prove.
- XXX: whether the IWRAM heap ever grows past 0x03007000, where it would
  overwrite the sound mixer (0x3a4 bytes) and the 0x45c unnamed bytes after
  it. Nothing in `heap.c` keeps it below: the allocators test against
  0x03007800, and even `Runtime_GetRemainingIwram`, which `Battle_RunEncounter`
  calls around every action and then ignores, measures up to 0x03007800, so
  the game's own measure counts the mixer as free. The heap has 0x5000 bytes
  below the mixer. Settling it needs the heap's high-water mark from play.
- XXX: AGENTS says each fixed address is checked against the link; no build
  step compares a `FIXED_ADDRESS` literal with the symbol's linked address
  (`make modern` only builds with the name). A one-off comparison of all 27
  `FIXED_ADDRESS` uses with the six ☀️ linker maps found every literal equal
  to its symbol in each edition its `#if` selects (the German
  `Ram_BgScroll`, `Ram_VramBlockCache` and `Ram_WorkSlot` in tbs-de, the
  others' in the other five).
- XXX: why the directory keeps its own address and `FarCall_FieldTable` as
  resources (its rows 1 and 3).
