# Field

This folder holds The Broken Seal's field engine: the sprites of people and
things, the 64 field objects and the scripts that move them, the player's
control by the keys, the layered maps of towns and dungeons, the world map
in perspective, and what the ground is like under a point. Scenes (in
`games/tbs/src/scene`) drive all of it through the engine's services.

See also: [Field objects](../object/README.md), which runs a field scene on
this engine; [Scenes](../scene/README.md), which drive it; and
[Maps](../map/README.md), the data its map code loads.

## Files

| File | What it holds |
| --- | --- |
| `field.c` | the guide; it includes `field_internal.h`, then the parts under `field/` in ROM order |
| `field_internal.h` | the includes and the few names more than one part uses; it places no bytes |
| `field/*.c` | the field object's code in 14 parts; never compiled on their own |
| `update.c` | `Object_UpdateAll`, the ARM update of the eight script objects, run from RAM |
| `vertices.c` | `Transform_UpdateVertices`, the ARM routine that turns the world map's ground plane into BG2's per-scanline affine values |
| `object_script.c`, `operand.c` | the object scripts the engine starts itself, the command table, and the work value handlers |
| `facing.c` | the facing tables, then the parts' small constant tables: the tile dissolve order, the fixed camera's matrix, the float wave, the idle script, the world map's pad headings, footprint scripts and turn rates |
| `curve_data.c`, `mapdata.c` | the terrain height tables, and the height function table with the world map's terrain behaviour and steps |
| `world_map_resources.c`, `griddata.c` | the world map's graphics sets, and the sprite viewer's scale |
| `lucky_dice.c` | the Lucky Dice minigame, with the camera defaults and 8.8 helpers it uses |
| `layered.tsv`, `remap*.tsv`, `scenes.s` | the layered map table, the byte remap table and the scene table |

The headers are in `games/tbs/include/unit/field/`, one for each file; the
field object's functions are declared in
[`unit/field/field.h`](../../include/unit/field/field.h). The objects' link order
is their rows of `games/tbs/objects.tsv`. In the ROM `field.c` is one
object; its split into parts is ours, cut where one family ends and the next
begins, and `head -3 field/*.c` says what each part holds.

## Words

- **object**: one of the 64 field objects in `gObjectSlots` (`struct
  ObjectRuntime`). The same memory is also read as a `DispatchObject`,
  `ScriptInterpreter`, `ScriptMotionObject` or `FieldActor`, each naming the
  fields its code uses (see the design below).
- **sprite**: a `struct AnimationObject`, one of 64: an image and a shadow
  (two OAM parts) and up to four **layers** (`struct AnimationEntry`), each
  playing an animation script over its frames. A sprite is drawn into one
  pixel buffer from its layers in priority order.
- **animation script**: pairs of bytes, a frame or an `ANIM_CMD_` command and
  its argument; the layer timer counts sixteenths of a frame.
- **object script**: `s32` words, each an `SC_` command of `objcmd.h` and its
  operands, run by `gScriptObject_CommandTable`. **Work values** (`SW_`) are
  an object's fields as `SC_SET`, `SC_ADD` and `SC_CMP` see them.
- **cell**: a map word, 128 to a row, each 16x16 pixels: its low 12 bits are
  the **block** (a 2x2 group of tiles, a metatile) and the rest attributes.
  A layered map has three map **layers** on BG3, BG2 and BG1 (not to be
  confused with a sprite's layers).
- Positions are 16.16 fixed point pixels, x across, y up and z down the map;
  angles are 16-bit turns (`0x4000` is a quarter turn).

The same locals carry the same names: `object` and `interpreter` for the
object a command runs on, `sprite` and `layer` for a sprite and its layer,
`work` or `mapWork` for the map's work (`gMapWork[0]`), and `perspective` for
the world map's.

## Prefixes

The folder's functions keep the prefixes of the engine family they belong to,
as other folders call them: `Sprite_` and `Render_` draw sprites,
`Animation_`, `AnimationObjects_`, `ResourceMetadata_`, `ResourceObject_`
and `ResourceSlot_` keep sprites and layers, `ObjectSystem_`,
`ObjectDispatch_`, `FieldObject_` and `Object_` keep objects, `Script_` are
object script commands, `Field_` the configured keys, `Map_` and `Tilemap_`
the layered maps and cells (`TileMap_DrawRowsFromRam` after the routine it
runs), `WorldMap_` the world map, `GetWorldMap` and `CheckWorldMap` the
ground under a point on it, `MapAnimation_`, `DisplayBlend_` and
`PaletteQueue_` the timed map effects, `Curve_` the ground height functions,
`Ui_` the debug sprite viewer and `LuckyDice_` the minigame. A name other
folders or The Lost Age's twin also use changes only in a rename across
folders, even where it no longer says what its code does.

A part's own types, macros and static helpers start with its part's or
family's name (`ObjectCamera`, `LayeredScene`, `WorldMap`), and its macros
are undefined at its end.

The spellings some sites keep to match, and the comment markers, are in
[Matching](../MATCHING.md).

## Design

### The object script protocol

An object's script is an array of `s32` words, and its cursor is a word
index. Each frame, for each live object, the update runs the object's hook,
then counts down its wait if it has one, and otherwise runs commands until
one of them yields. A command is a function in `gScriptObject_CommandTable`
that takes the object and returns 1 or 0:

- **1 continues**: the command has done its work and moved the cursor past
  itself and its operands, and the next command runs in the same frame.
  Tests, jumps, flags and work-value arithmetic return 1, so a script can
  decide and branch without losing a frame.
- **0 waits**: the frame is over for this object, and the same cursor runs
  again next frame. A command that is waiting for something (`SC_SYNC`
  until the object reaches its target) returns 0 without moving the cursor,
  and returns 1 once it is satisfied; `SC_WAIT` sets the wait count, moves
  past itself and returns 0.

So a command's return value decides time, and its cursor decides place; a
command that returned 0 without moving would be run again forever, which is
what waiting is.

Words above 63 are not commands, and the loop steps over them. That is what
lets a label sit in the stream: `SC_LABEL(n)` is a word with its top bit set,
which execution skips, and a jump's operand is `SC_TO(n)`, the same word with
bit 30 also set. `Script_FindLabel` clears bit 30 from the operand and
searches the first 1,024 words for the result, so it finds the label and
never the jump's own operand. Jumps are found by searching on every jump, not
by offsets, so a script needs no assembler to fix up its branches.

`SC_END` does not free the object: it points the script at
`gScript_MainScript`, a lone `SC_END`, and returns 0, so the object idles,
re-running that one command each frame until something gives it a new
script. `SC_FREE` frees it.

There are two loops that run scripts. `Object_UpdateAll` (`update.c`) is ARM
code, copied to RAM to run, for the eight script objects; `Object_UpdateAllThumb`
(`field/object_motion.c`) does the same for all 64 objects in Thumb, with the
field's extras (free flight, terrain, floating and solid objects). Both
follow the protocol above.

### One block, five views

A field object is 0x70 bytes, and five structs describe it:
`ObjectRuntime` (`include/object_runtime.h`), `DispatchObject`
(`include/object_dispatch.h`), `ScriptInterpreter` (`include/field/script.h`),
`ScriptMotionObject` (`include/script_motion.h`) and `FieldActor`
(`include/field_obj.h`). Each was written from the code that reads it, and
each names only the fields that code uses. They agree on most of the block
(the script at 0, the position at 8, the velocity at 0x24, the target at
0x38) and disagree where their readers do:

- the halfword at 0x5e is `wait` to the motion loop, `lookupResult` to the
  command handlers (`SC_WAIT` writes its count there) and `unk5e` elsewhere;
- the per-frame routine is `callback` at 0x60 in `ObjectRuntime`, but `hook`
  and `update` at 0x6c in `ScriptMotionObject` and `FieldActor`;
- 0x18 is a scale pair in `FieldActor` and unknown bytes in `ObjectRuntime`;
  0x44 is the bounce in two views and unknown in a third.

They are kept apart, rather than merged into one struct, because the views
are also what the code matches with. agscc addresses a field differently by
its declared type and by whether it is reached through a union (the
`FieldActor` single or group sprite union moves a load, as
[Matching](../MATCHING.md) records under Scenes), so one merged struct would change the bytes of the sites
that each view now matches. Even `gObjectSlots` is declared as
`ObjectRuntime *` in `include/field/field.h` and as `ScriptMotionObject *` in
`update.c`. XXX: Camelot surely had one struct; merging the five waits until
every field has one name and one type that every site matches with.

### The layered map

A town or dungeon map is three background layers that share one grid of
cells. The grid is 128 by 128 cells, 64 KB, decoded into
`Ram_MapCellBuffer`; each layer is a window into it, starting at the cell its
header gives (`Map_LoadLayeredScene` sets `layers[n].cells` to the cell at
half the header's 8-pixel tile position). Layer 0 shows on BG3, layer 1 on
BG2 and layer 2 on BG1, leaving BG0 to the windows; each has its own
parallax, drift and wrap. A layer's priority byte in the header is also its
switch: `Map_LoadLayeredScene` turns a layer on only when its priority is not
zero, so map layers use priorities 1 to 3. The data, its format and how the
build makes it are in [Maps](../map/README.md).

## How to change things

- The player's walking and running speeds are set early in
  `FieldObject_UpdatePlayerControl` and `Object_MoveOnWorldMap`
  (`player_control.c`), as `speedLimit` and `acceleration` per frame in
  16.16 pixels; the world map's step probe is `WORLD_MAP_PROBE_RADIUS` at the
  top of that part.
- An object script command is a function in `script_commands.c` (or
  `object_motion.c` for the first three) and a row of
  `gScriptObject_CommandTable` in `object_script.c`, with its macro in
  `objcmd.h`. It returns 1 to go on and 0 to wait, and moves the cursor past
  its operands when it is done.
- Build and check with `make tbs-en`, and `make compare-editions` for a
  change meant to match: all twelve editions must stay OK.

## Limits

- 64 objects, 64 sprites of up to four layers, 8 resource slots, 16 tile
  animation channels and 4 palette cycles.
- 47 script commands, 0x24 onwards all `SC_END`; a label must be within the
  first 1,024 words of its script.

## Evidence and open questions

The command names (`SC_WAIT`, `SC_SYNC` and the rest) are the macros of
`objcmd.h`, named from what each handler does; the struct views from the
fields each family reads; the map's words from the loader. None of them is
recovered from the original.

Still unknown:

- XXX: the update accepts command words up to 63, but the table has 47
  entries; a word from 47 to 63 would call through whatever follows the table.
- XXX: which of 0x60 and 0x6c is the object's per-frame routine in each use,
  and what the other holds.
- XXX: why placeholder blocks (0xfff) are given numbers of their own as the
  map is converted (`Tile_ConvertMap`, and the unused
  `Tilemap_NumberPlaceholderBlocks` that does the same in C).
