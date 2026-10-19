# System

This folder holds what ties The Broken Seal's image together: the cartridge
header and entry, the RAM layout, the code resident in IWRAM, the BIOS calls,
the far-call tables between the banks of the main image, the overlays, the
resource directory and the flash save library. Nearly all of it is assembly:
handwritten code that no C compiler would produce, each file showing why in
its header, and the tables the linker lays out.

## Files

| File | What it holds |
| --- | --- |
| `rom_head.s`, `entry.s` | the cartridge header and the entry that sets up the CPU modes and stacks |
| `ewram.s`, `iwram.s`, `iwram_hi.s` | the main image's RAM variables, in their order in EWRAM and IWRAM (the memory map below) |
| `runtime.s`, `runtime_irq_table.s`, `runtime_mix*.s`, `runtime_mul_q16_ip.s` | the code copied to IWRAM: the interrupt dispatcher, its handler table, the sound mixer and the fixed-point multiply |
| `iwram_entry.s` | the stubs that call the IWRAM code from the ROM |
| `cpu_set.s`, `sound_get_jump_list.s` | BIOS calls |
| `heapfree.s` | `Runtime_ReleaseHeapBlock` |
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

- A resource's id is its row's place in `directory.s` (`ResourceId_NAME` is
  the row number), so a row added in the middle renumbers every row after it;
  code names a resource by its symbol.

## Limits

- An overlay has the 32 KB from 0x02008000 to 0x02010000.
- The IWRAM heap ends at 0x03007800 and the EWRAM heap at 0x02040000;
  `Runtime_AllocateHeapBlock` takes IWRAM first and EWRAM when IWRAM is
  full.
- A far-call table's entries keep their order: code in other banks calls
  them by place.
