# System

This folder holds what ties The Broken Seal's image together: the cartridge
header and entry, the code resident in IWRAM, the BIOS calls, the far-call
tables between the banks of the main image, the overlays and the resource
directory. Nearly all of it is assembly: handwritten code, each file proven in
its header (AGENTS.md, S1), and the tables the linker lays out.

| File | What it holds |
| --- | --- |
| `rom_head.s`, `entry.s` | the cartridge header and the entry that sets up the CPU modes and stacks |
| `ewram.s`, `iwram.s`, `iwram_hi.s` | the main image's RAM variables, in their order in EWRAM and IWRAM |
| `runtime.s`, `runtime_irq_table.s`, `runtime_mix*.s`, `runtime_mul_q16_ip.s` | the code copied to IWRAM: the interrupt dispatcher, its handler table, the sound mixer and the fixed-point multiply |
| `iwram_entry.s` | the stubs that call the IWRAM code from the ROM |
| `cpu_set.s`, `sound_get_jump_list.s` | BIOS calls |
| `heapfree.s` | `Runtime_ReleaseHeapBlock` |
| `updfar.s` | three ARM routines that share one literal pool |
| `syspal.s` | the window palette and the basic sprite palettes |
| `far_call/*.S` | each bank's far-call table: one 8-byte jump per export, in a fixed order (`stub.inc`) |
| `overlay/` | the overlays: `overlays.s` places each one's packed stream as a directory resource, `overlay.ld` links each at 0x2008000, and the `ovl_*.s` hold the RAM variables of the overlays that have them |
| `overlay.inc` | the stub tables every overlay and far-call bank opens with |
| `resource/directory.s` | the resource directory: one entry per picture, table or overlay, each defining its `ResourceId_` name |
| `resource/*.s`, `resource/metadata_get_record.c` | the resource decoders, the colour conversion tables and the animation metadata lookup |

The unit headers are in `games/tbs/include/unit/system/`, the far-call
prototypes in `games/tbs/include/farcall/`. Link order, overlays included, is
in `games/tbs/objects.tsv`; `games/tbs/overlays.tsv` gives each overlay's
`.bss` header.

## Words used here

- **bank** and **far call**: the main image is linked in banks (`runtime`,
  `field`, `window`, `game` and the rest); a function another bank calls is
  exported through that bank's far-call table, and called by its `...Far`
  name. An entry with no name keeps its place in the table as a bare veneer.
- **overlay**: code and data loaded at 0x2008000 when a scene or a screen
  needs it, from one compressed resource. An overlay is its entry table, its
  code and its import table, the main-image functions it calls by name.
- **resource**: an entry of the directory, loaded by its id
  (`Resource_GetTableEntry`); the id is the `ResourceId_` symbol's value.

## Changing things

- A resource's id is its row's place in `directory.s` (`ResourceId_NAME` is
  the row number), so a row added in the middle renumbers every row after it;
  code names a resource by its symbol.
