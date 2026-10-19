# Field

This folder holds The Broken Seal's field engine: the sprites of people and
things, the 64 field objects and the scripts that move them, the player's
control by the keys, the layered maps of towns and dungeons, the world map
in perspective, and what the ground is like under a point. Scenes (in
`games/tbs/src/scene`) drive all of it through the engine's services.

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
field object's functions are declared in `field.h`. The objects' link order
is their rows of `games/tbs/objects.tsv`. In the ROM `field.c` is one
object; its split into parts is ours, cut where one family ends and the next
begins, and `head -3 field/*.c` says what each part holds.

## Words used here

- **object**: one of the 64 field objects in `gObjectSlots` (`struct
  ObjectRuntime`). The same memory is also read as a `DispatchObject`,
  `ScriptInterpreter`, `ScriptMotionObject` or `FieldActor`, each naming the
  fields its code uses.
- **sprite**: a `struct AnimationObject`, one of 64: an image and a shadow
  (two OAM parts) and up to four **layers** (`struct AnimationEntry`), each
  playing an animation script over its frames. A sprite is drawn into one
  pixel buffer from its layers in priority order.
- **animation script**: pairs of bytes, a frame or an `ANIM_CMD_` command and
  its argument; the layer timer counts sixteenths of a frame.
- **object script**: `s32` words, each an `SC_` command of `objcmd.h` and its
  operands, run by `gScriptObject_CommandTable`. A command returns 1 to run
  the next one this frame and 0 to wait for the next frame. **Work values**
  (`SW_`) are an object's fields as `SC_SET`, `SC_ADD` and `SC_CMP` see them.
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

## Spellings

Each idea has one spelling, except where a site matches only with a plain-C
equivalent; those sites keep it, untagged, with their measure in a comment:

- `*&gProjection.centerX` for `gProjection.centerX` (`vertices.c`), and
  `*(s32 *)&metadata->frames` for the frames pointer
  (`AnimationObject_Allocate`).
- `(mapState->anim + channelIndex)->` for `mapState->anim[channelIndex].`
  (`MapAnimation_Update`).
- `Scheduler_AddOrUpdateCallback` called through a cast to a `void`
  function (`Map_LoadLayeredScene`, with the declared call behind `UBFIX`).
- `(u16)~(...)` for a halfword mask (`Map_SetShownLayers`).
- Three loops written with `goto` (`Sprite_ComposeAnimationFrame`,
  `AnimationObjects_SelectAnimation`), with their loop forms measured in the
  comments beside them. The other `goto`s (the wander retries, the step
  searches, the map effects' script loops and the debug loops) stay until
  their loops are found.

Two comment markers are fixed: `// DEAD: why` for code the ROM keeps that
cannot change the result, and `/* FAKEMATCH(kind): ... */` for a device that
steers the compiler (AGENTS.md, S2).

## Changing things

- The player's walking and running speeds are set early in
  `FieldObject_UpdatePlayerControl` and `Object_MoveOnWorldMap`
  (`player_control.c`), as `speedLimit` and `acceleration` per frame in
  16.16 pixels; the world map's step probe is `WORLD_MAP_PROBE_RADIUS` at the
  top of that part.
- An object script command is a function in `script_commands.c` (or
  `object_motion.c` for the first three) and a row of
  `gScriptObject_CommandTable` in `object_script.c`, with its macro in
  `objcmd.h`.
- Limits: 64 objects, 64 sprites of up to four layers, 8 resource slots,
  16 tile animation channels and 4 palette cycles.
- Build and check with `make tbs-en`, and `make compare-editions` for a
  change meant to match: all twelve editions must stay OK.
