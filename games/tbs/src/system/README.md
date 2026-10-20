# System

When you switch Golden Sun on, the first of the game's own code to run is
here, and everything after it stands on what it sets up. `System_Entry` puts
the processor's stacks in place and hands over to the runtime. From then on,
what's here decides where everything lives in memory, which routines sit in
the Game Boy Advance's fast internal RAM, how one part of the program calls
another, and how the code for a town or a dungeon is packed so that it can be
unpacked into RAM when the party walks in. It also holds the directory that
every picture, table and overlay is found through, and the library that
writes your save to the cartridge's flash chip.

Nearly all of it is assembly. Some of that is code no C compiler would
produce, and each such file says in its header what gives it away; the rest
is tables the linker lays out. [Runtime](../runtime/README.md) builds directly
on it, and [Scenes](../scene/README.md) are the main users of its overlays.

## Where everything lives

The Game Boy Advance gives a game a few memory areas, each at a fixed
address. Addresses are written in hexadecimal, as the code, the link map and
any debugger show them: 0x02000000 is a base-16 number, and each hex digit
stands for four bits, so its eight digits make up the 32 bits of an address.
Sizes are counted in bytes of eight bits. The areas this game uses, as
`main.ld` declares them, are 256 KB of
work RAM (EWRAM) from 0x02000000; 32 KB of the small, fast internal RAM
(IWRAM) from 0x03000000; video RAM (VRAM) from 0x06000000, which holds the
tiles and maps the screen is drawn from; and the cartridge ROM itself, up to
32 MB from 0x08000000.

The addresses below are tbs-en's, from its link map (`out/tbs-en/tbs-en.map`).
German moves a few IWRAM variables (`ram_buffer.h`).

**EWRAM**, laid out by `ewram.s`:

| Address | Size | What | Named in |
| --- | --- | --- | --- |
| 0x02000000 | 32 KB | the main image's variables: the save buffer, the game flags (`gGameFlagBytes`), the game state (`gGameState`), the sound driver's work and the flash library's | `ewram.s` |
| 0x02008000 | 32 KB | the overlay area, `gOverlayArea`: the scene or screen overlay that's loaded | `ewram.s`, `overlay/overlay.ld` |
| 0x02010000 | 32 KB | `gMapCellBuffer`; battle effects also use it for particles | `Ram_MapCellBuffer`, `ram_buffer.h` |
| 0x02018000 | 16 KB | `gActorSpriteSlots` | `Ram_ActorSpriteSlots` |
| 0x0201c000 | 16 KB | `gDecodeBuffer` | `Ram_DecodeBuffer` |
| 0x02020000 | 32 KB | `gMapBlocks`, the map's blocks (`games/tbs/src/field/field/map_layers.c`) | `Ram_MapBlocks` |
| 0x02028000 | 16 KB | `gMapLayerData` | `ewram.s` |
| 0x0202c000 | 16 KB | `gMapCollision` | `Ram_MapCollision` |
| 0x02030000 | 32 KB | `gEwramHeap`, the EWRAM heap; `heap.c` allocates from it up to 0x02040000, the end of EWRAM, so past its 32 KB it would run into `gBgTileBuffer` | `ewram.s`, `games/tbs/src/runtime/runtime/heap.c` |
| 0x02038000 | 32 KB, to the end | `gBgTileBuffer` | `Ram_BgTileBuffer` |

**IWRAM**:

| Address | What | Named in |
| --- | --- | --- |
| 0x03000000 | the resident runtime, 0x1400 bytes copied from the ROM: the interrupt dispatcher `IwramIrqMain`, the handler table `gIrqHandlers`, the resident sound mixing code and the small fast routines the rest of the game calls (multiply, divide, square root, copying and filling, the 3D transform) | `main.ld` (`.iwram_runtime`), `runtime*.s` |
| 0x03001400 | the main image's IWRAM variables, among them the heap slots `gWorkSlot` (0x03001e50) | `iwram.s`, `Ram_WorkSlot` |
| 0x03002000 | `gIwramHeap`, the IWRAM heap, which `heap.c` allocates from below `Ram_IwramHeapEnd` | `iwram.s` |
| 0x03007000 | the sound driver's mixer, `SoundDriver_Mixer`, 0x3a4 bytes copied from the ROM by `Audio_Initialize` | `Ram_SoundMixer`, `main.ld` (`.iwram_mixer`), `games/tbs/src/sound/audio.c` |
| 0x03007800 | `Ram_IwramHeapEnd`: the end of the IWRAM heap, where `gCartridgeResetMarker` sits | `Ram_IwramHeapEnd`, `iwram_hi.s` |
| 0x03007a00 | `Ram_FrameWaitStackTop`: the stack `WaitFrames` runs on | `Ram_FrameWaitStackTop`, `iwram_hi.s` |
| 0x03007f00, 0x03007fa0 | the system-mode and IRQ-mode stack tops `System_Entry` sets | `entry.s` |
| 0x03007ffc | `gIrqVector`, where the BIOS jumps on an interrupt | `entry.s`, `iwram_hi.s` |

**VRAM** (`games/tbs/include/system/io_reg.h`) holds the tiles every picture
on screen is built from, and the maps that say which tile goes where:

- Background tiles go in four 16 KB character blocks (`BG_CHAR_BLOCK(n)`) and
  background maps in 2 KB screen blocks (`BG_SCREEN_BLOCK(n)`), sharing the
  same 64 KB. The window system's tilemap is at 0x06002000
  (`UI_TILEMAP_VRAM`, screen block 4) and its text canvas tiles at 0x06002500
  (`UI_CANVAS_VRAM`,
  [`unit/window/window.h`](../../include/unit/window/window.h)).
- Screens that load two pictures in turn split the background in halves: VRAM
  from 0x06000000 with background colours 0 to 127 is one half, and VRAM from
  0x06008000 with colours 128 to 255 the other
  (`Graphics_LoadCharacterBlockAndPalette`,
  `games/tbs/src/graphics/credits/credits.c`).
- Sprite tiles take the 32 KB from 0x06010000 (`OBJ_VRAM0`), which the sprite
  VRAM cache hands out as 512 blocks of 64 bytes
  (`games/tbs/src/runtime/runtime/vram_cache.c`).

## Why the program is cut into banks

The main image, the part of the program the processor runs straight from the
cartridge, isn't one continuous run of code. `main.ld` cuts it into banks
(`runtime`, `field`, `window`, `game` and the rest), puts each one at an
address of its own, written per edition (the `. = ABSOLUTE(...)` before each
`rom_main_NN` from 01 on, and before the later `rom_tail` places), and leaves
the space up to that address empty.

Each bank opens with its far-call table: a list of 8-byte jumps, one for each
of its functions that another bank calls. A caller in another bank never
calls such a function at its own address. It calls the table's entry instead,
by the function's `...Far` name, so `WaitFramesFar` reaches `WaitFrames`. In
`objects.tsv` the table is the first row of every `rom_main_NN` place from 01
on (a `far_call/*.S` file). The runtime bank's table, `FarCall_RuntimeTable`,
sits right after the cartridge header at 0x080000c0, and the resource bank's
opens `rom_tail_01`.

Distance doesn't explain this. A Thumb `bl`, the call instruction most of the
game uses, reaches 4 MB either way, and the whole main image, from 0x08000000
to the last bank's table below 0x08190000, is within reach of every bank. What
the arrangement buys is independence: another bank knows only where a bank
starts and the place of each export in its table, so a bank can grow or
shrink inside its gap without moving a single address that other code holds.

The starts differ by edition because each edition's banks hold a different
amount of code. Most of German's starts fall on 256-byte steps (0x08013d00,
0x0807ae00), where the other editions' sit on 2 or 4 KB ones.
XXX: likely Camelot linked each bank on its own against the others' tables,
because fixed starts and tables are what that would need; but the bytes show
only the starts and the tables, not how the game was built.

The jump itself is the `overlay_veneer` macro in `overlay.s`:
`ldr r4, =target; bx r4`, two instructions of two bytes each and then the
target's address in a word, the four bytes the processor reads as one
number. It can
use the register r4 freely because every game C file is compiled with
`-fcall-used-r4` (the `Makefile`'s `CFLAGS_tbs`), so no caller expects r4 to
survive a call. The processor has two instruction sets, the compact Thumb that
most of the game's C is compiled to and the full ARM that the IWRAM routines
use, and `bx` switches to ARM when the target is ARM code. That's why the IWRAM
routines can be reached through the same stubs.

## How an overlay reaches the rest of the game

The code for each place the party walks around in, a town, a dungeon or the
world map, isn't in the main image. It's an overlay: one compressed resource
in the cartridge, unpacked into the 32 KB at 0x02008000 when its scene loads.
There are 96 of them, numbered 0x36F to 0x3CE in the resource directory; some
places share one (the three `VaultCave` scenes all load 0x3a7), and the title
screen and a few other screens that are mostly menus are overlays too.

An overlay runs from EWRAM, 96 MB below the ROM, out of any `bl`'s reach, so
every call it makes into the main image has to load an address into a
register. It does that through its import table, the last of its three
objects (`import/<place>.s`): the overlay's code makes a short `bl` to a local
stub, and the stub's word holds the main-image address. The overlay is linked
on its own at 0x2008000 (`overlay/overlay.ld`), after the main image and
against its symbols (`-R` in the `Makefile`'s overlay rule), so the import
table is where its calls into the main image are written down.

The stub's word isn't the function itself but its far-call entry. Vault
imports `Engine_TaskWait`, whose stub goes to `WaitFramesFar`, which goes to
`WaitFrames`. So an overlay holds only the places of far-call entries, which
move only when a table changes, and the two hops are the price. The rows an
overlay may import, and their order, are fixed in `overlay.inc`: Camelot's
four divide routines first, which the stubs reach directly in IWRAM
(`IwramSignedDivide` and the rest), then the far-call entries in the order of
the main image. Each import table sets `import.NAME` for the stubs its overlay
calls and includes that list, so every overlay that imports a function keeps
it in the same relative order. Several local names can lead to one target
(`ArcTan2`, `Math_Atan2`, `CalculateFacingAngle` and
`CalculateAngleFromCoordinateDelta` all reach `ArcTan2Far`), because different
overlays' code calls the same routine by different names.

In the other direction, an overlay opens with its entry table
(`entry/<place>.s`): six 8-byte stubs whose words hold the scene's start-up
function and the functions that return its entrances, exits, placements,
events and regions. The field code reads them through `struct SceneEntries`
(`include/field_runtime.h`), calling `gOverlayArea.initialize()`,
`gOverlayArea.events()` and so on. The engine and an overlay meet only at
those two tables, and both have a known order.

Only one overlay is ever loaded, because they're all linked to the same 32 KB,
so an overlay never calls another. Code that several scenes need is copied
into each of them instead, which is what `games/tbs/src/scene/common/` holds.

When `Resource_LoadCode` (`runtime/runtime/system.c`) unpacks an overlay into
the overlay area, it runs `Resource_PatchThumbBranch` over it. In the packed
stream, each `bl` holds its target's position counted from the start of the
overlay, and the patch turns it back into a distance from the instruction
itself, which is what the processor expects. The packer,
`tools/ags-overlays`, does the reverse, so in the stream every call to the
same routine has the same bytes wherever it's made from. XXX: likely done so
the overlays compress better under LZ, because that's what the filter
achieves; nothing in the ROM says so.

## Why the order of a table matters

Every picture, map, table and overlay the game loads is a resource: a row of
the directory, `Resource_DirectoryTable` in `resource/directory.s`, which
holds where that resource's data starts. Code asks for a resource by its
number (`Resource_GetTableEntry`), and the number is simply the row's place:
the `resource` macro defines `ResourceId_NAME` as that place. The far-call
tables work the same way, as lists of places. Callers in this tree use names
the linker resolves, so a moved row still builds. But the bytes change, and
some code reads a list by position, which makes the order part of the design.

A row added in the middle of a far-call table moves every entry after it by 8
bytes, and the bank's own code after the table by 8 bytes too. Callers use
the `...Far` symbols, so that's only a relink, but the bank's code moves
towards the next bank's fixed start, and once the gap is gone the link fails.
Code that reads a table by place goes wrong: `Transform_UpdateVertices`
(`games/tbs/src/field/vertices.c`) calls `ArcTan2`, `Trig_Sin` and `Trig_Cos`
as members of `FarCall_RuntimeTable` at byte offsets 64, 88 and 96, which its
`STATIC_ASSERT`s check. And every caller of a later entry, in every bank and
overlay, changes its bytes, so no edition compares OK. A row at the end of a
table moves no other entry, though the bank's code after it still shifts.

In the directory, a row added in the middle renumbers every row after it.
Code that names a resource by its symbol follows a renumbering, but numbers
travel without symbols in several places, and those would silently point at
the wrong row. The scene table
(`games/tbs/src/field/scenes.s`) gives each scene its overlay as a number
(`scene Vault, 0x382, ...`), and the first column of `objects.tsv` numbers
each overlay's rows the same way. The layered map table
(`games/tbs/src/field/layered.tsv`) holds row numbers counted from
`ResourceId_Map001` (`Map_LoadLayeredScene`), so the rows after that one keep
their order. And code adds to an id to reach the rows after it: the option
menu's marker steps through eight frames at `ResourceId_ShopCursorFrames` plus
the frame (`window/window/options.c`), so a run like that stays together.

The directory ends with 25 unused rows (`unused 25`), where a row fits
without renumbering any other. XXX: whether they were left for that isn't
known.

## The addresses that never move

Some places in memory are fixed: the code reaches them as plain numbers, not
through the linker. `include/ram_buffer.h`, `include/iwram_call.h` and
`include/cartboot.h` list them, each once, with
`FIXED_ADDRESS(type, address, linked)`. The matching build compiles the
number, and `make modern` compiles the linked name instead (`FIXED_LINKED_` in
`include/types.h`). Because the code holds these numbers itself, the layout
has to keep every one of them:

1. **The cartridge restart entry is 0x08000000** (`Cart_Restart`). The soft
   reset in `WaitFrames` jumps there, and `SerialTest_Run`, a link cable test
   left in the game that nothing reaches, sends the first 0x280 bytes from
   there over the cable. So the header and `System_Entry` stay at the start
   of the ROM.
2. **The resident runtime is at 0x03000000** (`.iwram_runtime`, 0x1400
   bytes), copied from the ROM by `Runtime_InstallIwramAndIrqs`. C calls its
   routines at their addresses (`IwramSqrt` at 0x030001d8 and the rest), and
   the ARM code that runs from RAM reaches them as offsets from
   `IwramIrqMain` (`IWRAM_ENTRY`, `games/tbs/src/field/update.c`). Nothing in
   that section can move.
3. **Every overlay runs at 0x02008000** (`gOverlayArea`) and has to fit in the
   32 KB below 0x02010000. Its first 48 bytes are its entry table, which the
   field object calls by member.
4. **The map and decode buffers sit at fixed places in EWRAM**
   (`Ram_MapCellBuffer` 0x02010000, `Ram_ActorSpriteSlots` 0x02018000,
   `Ram_DecodeBuffer` 0x0201c000, `Ram_MapBlocks` 0x02020000,
   `Ram_MapCollision` 0x0202c000, `Ram_BgTileBuffer` 0x02038000). Code reaches
   across their boundaries by arithmetic: the map's cells fill 64 KB from
   0x02010000, through the sprite slots and the decode buffer (see
   [Maps](../map/README.md)), and the tile animation reads tiles from 0x600
   on at `Ram_DecodeBuffer + n * 32`, which lands in `gMapLayerData`. So
   `ewram.s` can't reorder or resize them.
5. **The heaps end at 0x03007800 and 0x02040000.** The allocators test
   against these numbers (`heap.c`); `gCartridgeResetMarker` sits at the
   first, and the system stack lives above it.
6. **The sound driver's mixer is at 0x03007000** (`.iwram_mixer`, 0x3a4
   bytes). `Audio_Initialize` copies it there, a whole 1 KB at once
   (`SOUND_MIXER_CODE_SIZE`), so the 0x5c bytes that follow the mixer in the
   ROM land just after it. The per-song mixer records go somewhere else:
   `Sound_LoadPresetParameters` copies its 152-byte presets into the resident
   code at `IwramSoundPresetCode` (0x03000bd8), not into this mixer. The mixer
   lies inside the IWRAM heap's range, which runs from `gIwramHeap`
   (0x03002000) to 0x03007800; see the open questions.
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

## Limits

An overlay has the 32 KB from 0x02008000 to 0x02010000, and a bank has the
space up to the next bank's fixed start, and no more. The IWRAM heap ends at
0x03007800 and the EWRAM heap at 0x02040000; `Runtime_AllocateHeapBlock`
takes IWRAM first and EWRAM when IWRAM is full. A far-call table's entries
keep their order, because code in other banks calls them by place, and the
resource directory has 25 spare rows at its end.

## What's in the folder

| File | What it holds |
| --- | --- |
| `rom_head.s`, `entry.s` | the cartridge header, the entry that sets up the CPU modes and stacks, and the five sound preset records (`IwramCodeRecords`) |
| `ewram.s`, `iwram.s`, `iwram_hi.s` | the main image's RAM variables, in their order in EWRAM and IWRAM (the memory map above) |
| `runtime.s`, `runtime_irq_table.s`, `runtime_mix*.s`, `runtime_mul_q16_ip.s` | the code copied to IWRAM: the interrupt dispatcher, its handler table, the resident sound mixing code, and the fixed-point multiply with the divide and square root routines |
| `iwram_entry.s` | the stubs that call the IWRAM code from the ROM |
| `cpu_set.s`, `sound_get_jump_list.s` | BIOS calls |
| `heapfree.s` | `Runtime_ReleaseHeapBlock` and `MemFree` |
| `updfar.s` | three ARM routines that share one literal pool (`ArcTan2FarArm`, `IwramSqrtFarArm`, `IwramRatioMulQ14FarArm`) |
| `syspal.s`, `memory/*.png` | the window palette and the basic sprite palettes, built from the PNGs |
| `overlay.s` | the far-call stub macro: 8 bytes that load a routine's address and jump to it |
| `far_call/*.S` | each bank's far-call table: one 8-byte jump per export, in a fixed order (`stub.inc`) |
| `overlay/` | the overlays: `overlays.s` places each one's packed stream as a directory resource, `overlay.ld` links each at 0x2008000, and the `ovl_*.s` hold the RAM variables of the overlays that have them |
| `overlay.inc` | the stub tables every overlay and far-call bank opens with |
| `resource/directory.s` | the resource directory: one entry per picture, table or overlay, each defining its `ResourceId_` name |
| `resource/*.s`, `resource/metadata_get_record.c`, `resource/colconv*.tsv` | the resource decoders, beside them the sprite list builder and colour-buffer kernels that other code copies into RAM to run, the colour conversion tables, the sprites' shadow and the animation metadata lookup |
| `save/` | the flash save library: the chip drivers for Atmel, Macronix and SST flash (`FLASH_C`, compiled with pret's `old_agbcc`), and the save record signature, the letters `CAMELOT` (`savedata.c`) |

The unit headers are in `games/tbs/include/unit/system/`, the far-call
prototypes in `games/tbs/include/farcall/`. Link order, overlays included, is
in `games/tbs/objects.tsv`; `games/tbs/overlays.tsv` gives each overlay's
`.bss` header. `games/tbs/main.ld` places the main image's sections, and
`games/tbs/include/ram_buffer.h` names the fixed RAM addresses the code uses
as constants. How the build is judged, and what the markers in the code mean,
is in [Matching](../MATCHING.md).

## Words used here

- **Bank** and **far call**: the main image is linked in banks (`runtime`,
  `field`, `window`, `game` and the rest). A function another bank calls is
  exported through its bank's far-call table and called by its `...Far` name.
  An entry no code calls by name still keeps its place in the table, as a
  jump with no `...Far` name of its own.
- **Overlay**: code and data loaded at 0x2008000 when a scene or a screen
  needs it, from one compressed resource. An overlay is its entry table, its
  code, and its import table of the main-image functions it calls by name.
- **Resource**: an entry of the directory, loaded by its id
  (`Resource_GetTableEntry`); the id is the `ResourceId_` symbol's value.
- **Resident code**: code, mostly ARM, copied from the ROM into IWRAM at
  power-up (`.iwram_runtime`) or when sound starts (`.iwram_mixer`).

## Function prefixes

| Prefix | What it acts on |
| --- | --- |
| `System_` | the cartridge entry |
| `Iwram` | the resident IWRAM code and its entry stubs (`IwramIrqMain`, `IwramHalt`, `IwramMulQ16`) |
| `Bios_` | the BIOS calls |
| `Resource_`, `ResourceId_` | the resource decoders and the directory's ids |
| `Flash_`, `EraseFlash`, `ProgramFlash` | the flash library, named as pret names it |
| `Runtime_`, `Sound_`, `ColorBuffer_`, `Render_` | things the engine names elsewhere |

## Evidence and open questions

The names here follow what each piece does in the link: the far-call tables
are named for the bank they open, the stubs for their target with `Far`
added, and the overlays by their directory number. `IwramIrqMain` and the
other resident routines are named from their code. The fixed addresses come
from the literals in the ROM's code; the matching build compiles those
numbers and `make modern` uses the linked names, so both forms build, though
nothing in the build compares one with the other (below).

Still unknown:

- XXX: whether Camelot linked the banks separately, which the fixed bank
  starts suggest but don't prove.
- XXX: whether the IWRAM heap ever grows past 0x03007000, where it would
  overwrite the sound mixer (0x3a4 bytes) and the 0x45c unnamed bytes after
  it. Nothing in `heap.c` keeps it below: the allocators test against
  0x03007800, and even `Runtime_GetRemainingIwram`, which `Battle_RunEncounter`
  calls around every action and then ignores, measures up to 0x03007800, so
  the game's own measure counts the mixer as free. The heap has 0x5000 bytes
  (20 KB) below the mixer. The EWRAM heap has the same shape: `ewram.s` gives
  it 32 KB, and the allocators let it run on through `gBgTileBuffer` to
  0x02040000. Settling either needs the heaps' high-water marks from play.
- XXX: the project's rules ask for each fixed address to be checked against
  the link, but no build step compares a `FIXED_ADDRESS` literal with its
  symbol's linked address (`make modern` only builds with the name). A one-off
  comparison of all 27 `FIXED_ADDRESS` uses with the six ☀️ linker maps found
  every literal equal to its symbol in each edition its `#if` selects (the
  German `Ram_BgScroll`, `Ram_VramBlockCache` and `Ram_WorkSlot` in tbs-de,
  the others' in the other five).
- XXX: why the directory's first 19 rows point at pieces of the program
  itself: the start of the ROM (`Rom_Start`, row 0), the directory's
  own address (row 1), the build stamp (row 2), and from row 3 on every bank's
  far-call table but the runtime's, the sound and resource tables twice each.
  The build stamp is read through its row (`game/game/units.c`), but the named
  far-call rows are only passed to stubs that do nothing
  (`RuntimeDispatch_NoOpHook`, `RuntimeDispatch_ReturnZero`), from the title
  screen, Lucky Dice, the slot machine and the sprite viewer. Those calls look
  like a leftover of loading banks as resources (the comment in
  `menu/title.c` suggests as much), but nothing proves it.
