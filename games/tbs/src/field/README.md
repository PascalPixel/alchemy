# Field

The field engine runs everything outside a battle that moves or is drawn: it
keeps track of the [party](../../glossary.md#the-party), the townspeople, the camera and even footprints,
moves each of them a little every [frame](../../glossary.md#frame), decides
whether the ground ahead can be walked on, and draws the
[map](../../glossary.md#map), a town as three flat layers or the world map
tilted into perspective. It decides nothing about any one map.

Three other folders hold the rest. The loop that drives the engine is in
`overworld`, whose page is [Running a map](../overworld/README.md); the folder
is named after the [bank](../../glossary.md#bank) it fills. Each map's own
code, its [scripts](../../glossary.md#script) included, is an
[overlay](../../glossary.md#overlay): [Maps](../map/README.md). What a town or
dungeon shows and stands on is its [layout](../../glossary.md#layout):
[Layouts](../layout/README.md). [World](../../world.md) gives the whole layer
in brief.

## The walk and what interrupts it

![Isaac walks up to a young man in Vault and talks to him; the walk stops while "Welcome to Vault, the town at the heart of Angara." is shown, and once the window closes Isaac walks on](https://alchemy.pascalpixel.com/guides/field/talk-walk.gif)

A field map switches between two states. While walking, the player has
control and the game watches, once a frame, for anything that should
interrupt. While handling, the player has none and the game works through
whatever came up, one thing at a time, then hands control back.

Three kinds of thing interrupt the walk: the ground the leader steps onto (a
doorway, a spot that starts a script, a ledge, ground that brings a random
battle), the buttons (A to talk or search, the menu, Start and the two
shortcut buttons), and a running script asking for another script or a
battle. Each raises a flag of its own, a request, and the game handles them in
a fixed order:

1. leaving the map;
2. a script something asked for;
3. a battle;
4. the cell the leader stepped onto;
5. a ledge or climbing spot the leader pushes against;
6. the A button, which becomes talking, searching or the menu;
7. then the menu, talking, searching, [Psynergy](../../glossary.md#psynergy-and-pp),
   an item and Start.

Leaving comes first, so once a doorway or a battle has been asked for the map
ends before anything raised with it is looked at. Because the walk stays
stopped while a request is handled, a cutscene is one script that plays from
start to end while the party waits; whatever must keep moving meanwhile moves
on its own, as an object script ([below](#everything-that-moves-runs-a-script)).

`Map_Run` ([`overworld/run_map.c`](../overworld/overworld/run_map.c)) waits a
frame and calls `Field_ProcessStep`, which checks the ground under the leader,
until `Field_CountRequests` finds a request; the leader's control reads the
buttons through `Field_CheckConfiguredKeys`. The requests are fields of
`struct FieldWork` ([Running a
map](../overworld/README.md#the-map-loop-handles-requests-not-scripts)). A
[warp](../../glossary.md#warp) is `Map_Run`'s result, which `Game_Run` passes
to `Map_TakeWarp`.

## Everything that moves runs a script

![Isaac walks through Imil's snow; each footprint he leaves is an object of its own and disappears after a moment](https://alchemy.pascalpixel.com/guides/field/snow.gif)

Everything on the field that moves or changes by itself is an
[object](../../glossary.md#object), and each one runs a small script. The
party, the people and things a map places (its
[object events](../../glossary.md#object-event)), the camera and each
footprint share 64 slots. Every frame each object runs the steps of its script
until one says to wait. The leader's script is "move by the pad, wait a frame,
repeat", the camera's "follow the leader, wait a frame, repeat", and a
footprint's "wait 120 frames, disappear"; the leader leaves one every 18
frames of walking or 12 of running on the maps that show them.

The objects are the 64 slots of `gObjects`, 112 bytes each, in a block the
[heap](../../glossary.md#heap-and-pools) lends under the
[heap slot](../../glossary.md#heap-slot) `HEAP_SLOT_OBJECTS`.
`Object_UpdateAllThumb` (`field/object_motion.c`) gives each its turn, then
moves each object that isn't frozen: free flight towards its target, following
the ground, floating, and solid objects refusing to overlap. In a battle
`Object_UpdateAllMotion` replaces it, moving the first 14 objects and running
no scripts.

### How a script runs

![Two children in Vault run round the well, each following the same eight points half a lap apart, over and over](https://alchemy.pascalpixel.com/guides/field/well-route.gif)

A script is a list of 32-bit words: a command, then the values it needs. The
two children round Vault's well each run one of these, the boy's eight points
in turn and the girl's the same points half a lap behind. A step that only
sets, tests or jumps takes no time; `SC_SYNC` waits until the child reaches
the point.

```c
s32 gVault_BoyRoundTheWellRoute[] = {
    SC_ANIMATION(2),
    SC_SET(SW_SPEED, PIXELS(4) / 5),
    SC_SET(SW_ACCELERATION, PIXELS(1) / 5),
    SC_LABEL(1),
    SC_MOVE(PIXELS(136), 0, PIXELS(376)),
    SC_SYNC,
    SC_MOVE(PIXELS(120), 0, PIXELS(376)),
    SC_SYNC,
    /* ... six more points round the well ... */
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};
```

The commands are the `SC_` macros of [`objcmd.h`](../../include/objcmd.h);
the script is in [`map/vault.c`](../map/vault.c). A command word indexes
`gObjectScript_CommandTable`, whose handler returns 1 when it has finished and
the next command runs in the same frame, or 0 to end the object's turn.
`SC_WAIT(n)` moves past itself and returns 0 with n - 1 frames left, since the
current frame counts. Before the script, the update calls the object's hook if
it has one, a function a map can give one object to run every frame; the hook
runs even while the object is paused, and a paused object does nothing else.

A word above 63 is no command, and the update steps over it, which is how a
label sits in the list: `SC_LABEL(n)` has its top bit set and n in bits 16
up, and a jump's target `SC_TO(n)` is the same word with bit 30 set as well.
`ObjectScript_FindLabel` clears bit 30 and searches the first 1,024 words for
the label; a label it cannot find, and `SC_TOP`, restart the script. `SC_END`
leaves the object idling on `gObjectScript_EndScript`; only `SC_FREE` frees
the slot.

Most handlers are in `field/script_commands.c`; the player controls
(`SC_PLAYER`, `SC_KEYS`, `SC_WORLD`, `SC_KEY_COUNT`) are in
`field/player_control.c`.

| Name | Value | Meaning |
| --- | --- | --- |
| `OBJECT_COUNT` | 64 | objects, the slots of `gObjects` |
| `SC_WAIT(n)` | `0x00`, n | end the turn and wait n frames |
| `SC_SYNC` | `0x01` | wait until the object reaches its target |
| `SC_MOVE(x, y, z)` | `0x03`, x, y, z | set a target to move towards |
| `SC_CAMERA` | `0x0b` | follow the leader |
| `SC_LOOP(n, to)` | `0x0c`, n, to | jump to label `to` n - 1 times, so what lies between runs n times |
| `SC_END` | `0x10` | idle on `gObjectScript_EndScript` |
| `SC_PLAYER` | `0x18` | the player's control on a town or dungeon map |
| `SC_KEYS` | `0x19` | move 8 pixels a step by the direction pad |
| `SC_WORLD` | `0x1a` | the player's control on the world map |
| `SC_FREE` | `0x1b` | free the object's slot |
| `SC_ANIMATION(v)` | `0x1c`, v | switch the sprite to animation v |
| `SC_KEY_COUNT` | `0x23` | check the configured buttons; the last command |
| `SC_LABEL(n)` | `0x80000000 \| n << 16` | a label: a word the update steps over |
| `SC_TO(n)` | `0xc0000000 \| n << 16` | a jump's target, naming label n |
| `SC_TOP` | 0 | the script's start |
| `SC_FOREVER` | `0xffff` | an `SC_LOOP` count that never runs out |

## Sprites and their animation

![In a Goma Cave battle Isaac leaps at a sleeping Skeleton and swings his sword, then Garet swings his axe; each figure's body and weapon change pose together](https://alchemy.pascalpixel.com/guides/field/battle-swing.gif)

What the player sees of an object is its
[sprite](../../glossary.md#sprite-and-oam), built from up to four pictures
laid over each other, each playing its own animation. In a battle Isaac's
body, his weapon and the shadow under his feet are three anims of one sprite,
so as he swings the body and the weapon change frame together, and the whole
picture is redrawn each time.

Each anim plays an animation script of byte pairs: a frame and how long to
show it, or from `ANIMCMD_END` (239) up a command (`enum AnimationCommand` in
`field_internal.h`). The timer counts sixteenths of a frame and loses
`SpriteAnim.step` each frame, `ANIM_NORMAL_STEP` at normal speed. A script
names frames as if the sprite always faced one way; a facing table (`facing.c`)
adds a number to the frame and says whether to mirror it for each eighth of a
turn. When any anim's frame changes, `Sprite_ComposeFrame` decodes each anim's
frame into one buffer in priority order and uploads it to the sprite's tiles.

`BattleObject_SpawnObjectsForList` (`../battle/battle/objects.c`) gives
Isaac's battle sprite his body (`BattleObject.resource`) and weapon
(`BattleObject.overlay`) at priority 1, drawn in that order so the weapon lies
over the body, and the shadow (`BattleObject.effect`) at priority 0, drawn
first. The party is seen from behind, so the facing table adds 1 to each frame
his swing names: frames 12, 14 and 16 draw the sheets' 13, 15 and 17
(`spr_300.png`, `spr_320.png` and `spr_510.png` in
[`graphics/sprite`](../graphics/sprite)). An anim can also be recoloured as it
is drawn (`SpriteAnim.colorMode`), through one of the nine 256-byte tables of
`gSpriteColorTables` and `gSpriteColorRemapTables` or in one flat colour.

| Name | Value | Meaning |
| --- | --- | --- |
| `ANIM_OBJECT_COUNT` | 64 | sprites, and the sprite anims they share |
| `ANIM_LAYER_COUNT` | 4 | anims a sprite composes, `Sprite.entries` |
| `ANIM_NORMAL_STEP` | 16 | sixteenths of a frame off an anim's timer each frame: normal speed |
| `ANIMCMD_END` | 239 | blank the anim and stop its script |
| `ANIMCMD_SET_KIND` | 240 | use facing table `arg` |
| `ANIMCMD_HOLD` | 241 | stay on this command and frame for good |
| `ANIMCMD_WAIT` | 245 | keep the current frame `arg` frames longer |
| `ANIMCMD_JUMP` | 253 | continue at script position `arg` |
| `ANIMCMD_PLAY` | 254 | switch to animation `arg` |
| `ANIMCMD_BLANK` | 255 | show nothing for `arg` |

## Objects move first, then the camera draws them

Every frame all the objects move first, and only then does the camera work out
where each appears on the screen and what it is in front of. Because that
order never changes, each sprite is drawn where its object has just moved to.

The two steps are two [tasks](../../glossary.md#task), jobs the game runs once
a frame. `ObjectSystem_Initialize` adds the object update at priority 3210
and a camera at 3200; the runner runs higher priorities first. The camera
places each sprite and queues it by depth, and the frame's sprite list, the
OAM, is built from that queue. While the item, Psynergy, Status,
[Djinn](../../glossary.md#djinn) or debug menu is open,
`ObjectSystemWork.suspended` hides the sprites that are not held.

| Mode | Started by | Object update | Camera |
| --- | --- | --- | --- |
| `OBJECT_SYSTEM_LAYOUT` (2) | the field map runner, the debug sprite viewer | `Object_UpdateAllThumb` | `ObjectSystem_PlaceSprites`, which scrolls with the map |
| `OBJECT_SYSTEM_WORLD_MAP` (3) | the field map runner | `Object_UpdateAllThumb` | `ObjectSystem_ProjectSprites`, which projects 3D points onto the screen |
| `OBJECT_SYSTEM_BATTLE` (4) | the encounter (`battle/battle/encounter.c`) | `Object_UpdateAllMotion` | `ObjectSystem_ProjectSprites` |

## Walking and running

![Isaac walks right in front of the Sanctum in Vale, then runs back left with B held, half as fast again](https://alchemy.pascalpixel.com/guides/field/walk-run.gif)

The leader walks at one pixel a frame and runs at one and a half while B is
held. Before each step the game tests the ground 8 pixels ahead, and against a
wall it tries headings further and further to either side, so the leader
slides along the wall instead of stopping dead.

When the map loop hands control back, it starts one of the engine's controls
on the leader: `SC_PLAYER` on a town or dungeon map, `SC_WORLD` on the world
map, `SC_KEYS` (8-pixel steps) while climbing, or only `SC_KEY_COUNT` while
the party is carried through the air. On a town or dungeon map
`Object_UpdateLeaderControl` tests the pad's heading and 22.5 and 45 degrees
either side, then headings up to 67.5 degrees off. With `FLAG_FAST_WALK` set,
B moves at `FIELD_FAST_WALK_SPEED`; nothing in
[The Broken Seal](../../glossary.md#the-two-games) sets that flag. On the world
map `Object_MoveOnWorldMap` walks at half a pixel a frame, and L raises the
limit while it repeats.

The Japanese edition and [the international
editions](../../glossary.md#editions) differ on the world map, chosen in a
block of `#if`s at the top of `field/player_control.c`:

| | Japanese | International |
| --- | --- | --- |
| world map probe | 8 pixels; the step and 30 degrees either side, then up to 67.5 degrees off | 7 pixels; the step and 22.5 and 45 degrees either side, then up to 90 degrees off |
| R and L on the world map | raise their events while held | raise them once a press |

Speeds are 16.16 fixed-point pixels a frame: the top 16 bits are whole pixels
and the low 16 a fraction, so `0x10000` is one pixel.

| Name | Value | Meaning |
| --- | --- | --- |
| `FIELD_WALK_SPEED` | `0x10000` | 1 pixel a frame |
| `FIELD_RUN_SPEED` | `0x18000` | 1.5 pixels a frame, with B held |
| `FIELD_ACCELERATION` | `0x4000` | a quarter pixel a frame faster each frame |
| `FIELD_FAST_WALK_SPEED` | `0x40000` | 4 pixels a frame, B held with `FLAG_FAST_WALK` |
| `FIELD_FAST_WALK_ACCELERATION` | `0x10000` | 1 pixel a frame faster each frame |
| `WORLD_MAP_WALK_SPEED` | `0x8000` | half a pixel a frame |
| `WORLD_MAP_RUN_SPEED` | `0x10000` | 1 pixel a frame, with B held |
| `WORLD_MAP_ACCELERATION` | `0x4000` | a quarter pixel a frame faster each frame, walking |
| `WORLD_MAP_RUN_ACCELERATION` | `0x14000` | 1.25 pixels a frame faster each frame, running |
| `WORLD_MAP_FAST_SPEED` | `0x40000` | 4 pixels a frame, while L repeats |
| `WORLD_MAP_PROBE_RADIUS` | `0x80000`, `0x70000` | 8 pixels in Japanese, 7 in the international editions |

## How high a step can go

![Isaac walks north across the shallow pond in Goma Cave and stops at its edge: a rim of rock 32 pixels high runs along it](https://alchemy.pascalpixel.com/guides/field/step-height.gif)

Every spot on a map has a height. The leader can step up onto ground at most
8 pixels higher, or down onto ground at most 12 pixels lower, and anything
steeper blocks the way as surely as a wall. Most of the game's walls are made
this way rather than marked: Goma Cave has no wall marks at all, only rims of
ground 32 pixels high like the one at this pond's edge, though the floor
beyond it is no higher than the water.

Each [cell](../../glossary.md#cell) has a shape: a terrain kind (flat, a
slope, a step and so on) in the low four bits of its first byte and three
height samples in steps of 8 pixels. `MapGrid_GetTerrainHeight` picks one of
the 16 functions of `gMapGrid_TerrainHeightFunctions` (`curve_data.c`) by the
kind and works the height out from the samples. An object's `layer` names the
map layer whose ground it follows, so walls and heights can differ from one
layer to the next. `MapGrid_CheckCollision` (`field/terrain.c`) decides each
test; on the world map `WorldMap_IsWalkable` reads a 4-bit collision value
instead.

| Name | Value | Meaning |
| --- | --- | --- |
| `TERRAIN_KIND_COUNT` | 16 | ground height functions a shape can pick |
| `COLLISION_TOO_LOW` | -1 | the ground there is more than 12 pixels lower |
| `COLLISION_NONE` | 0 | the object can step there |
| `COLLISION_TOO_HIGH` | 1 | the ground there is more than 8 pixels higher |
| `COLLISION_IMPASSABLE` | 2 | the cell there is a wall (`MB_WALL`) |

## Towns and dungeons on screen

![Isaac walks east behind a house in Vale: its roof is drawn over him as he passes, then he walks back](https://alchemy.pascalpixel.com/guides/world/behind-roof.gif)

A town or dungeon is drawn as three flat layers stacked on top of each other,
the ground at the back, a middle layer, and in front an overhead layer of roofs
and treetops. On most maps the characters are drawn between the middle and
overhead layers, which is how Isaac can walk behind a roof. Which layer is in
front comes from the console's priorities, not from code.

`MapGrid_LoadLayout` loads a layout from its row of `gLayouts` (`layouts.c`)
and puts the three map layers on three of the console's four
[background layers](../../glossary.md#background-layer): `MAPGRID_LAYER_GROUND`
on `BG3`, `MAPGRID_LAYER_MIDDLE` on `BG2` and `MAPGRID_LAYER_OVERHEAD` on
`BG1`, leaving `BG0` for messages and menus. Each layer's priority comes from
the layout's header, and a lower number is nearer the front; the field draws
sprites at priority 2, and a sprite wins a tie. Of the 182 layouts, 101 give
the layers 3, 2 and 1, 49 give 3, 1 and 2, 15 give 3, 1 and 1, and the rest
other orders; a priority of 0 leaves a layer off, and nine layouts show only
their ground.

Each layer scrolls by its own parallax and drift from the header, and
`MapGrid_UpdateLayerScroll` redraws the rows and columns of the BG maps that
come into view from the cells' [metatiles](../../glossary.md#metatile). Three
timed effects run as tasks (`field/tile_animation.c`): up to 16 tileset
animations, the blend script, and palette cycles; only `PaletteCycle_Start`
schedules a palette cycle, and nothing calls it.
[Layouts](../layout/README.md#tileset-animation-and-blend-scripts) gives the
scripts' format.

| Name | Value | Background | Meaning |
| --- | --- | --- | --- |
| `MAPGRID_LAYER_GROUND` | 0 | `BG3` | the ground, at the back |
| `MAPGRID_LAYER_MIDDLE` | 1 | `BG2` | walls and edges, under the object events |
| `MAPGRID_LAYER_OVERHEAD` | 2 | `BG1` | roofs and tree tops, over the object events |

## The world map in perspective

![Isaac walks south out of Vale on the world map; while L is held the view pulls back to show far more land, and it returns when L is let go](https://alchemy.pascalpixel.com/guides/field/world-map-zoom.gif)

The world map is a flat map, like a town's, that the console tilts so it seems
to stretch away towards the horizon. Each of the screen's 160 lines shows one
straight strip of the flat map, and lines near the top show strips farther
away and wider, squeezed to fit, so distant ground looks smaller. When the
player holds L, the lines spread so far apart that some rows of the ground are
never drawn.

The console can scale and rotate `BG2` and `BG3`, and reads their scale and
position afresh for every line if the game changes them between lines. As the
party leaves Vale each line's scale falls steadily from 0.61 on the top line
to 0.44 on the bottom one. The lower ground is on `BG3` and the mountains,
woods and cliffs on `BG2`; the two share a priority, so `BG2` is drawn in
front, and the sprites in front of both.

`WorldMap_BuildScanlineTable` gives each screen line a width scale and a
ground distance when the world map is set up, and again only if the camera's
pitch changes, which nothing in the game does. Each frame
`Transform_UpdateVertices`, one of the ARM routines run from RAM
([below](#routines-run-from-ram)), turns them into a page of affine values
for every line, and `WorldMap_ApplyAffineFrame` starts a DMA that runs in each
HBlank, the pause after a line is drawn, to feed the next line its own. There
are two pages, used by alternate frames. The ground is unpacked a square at a
time around the party (`WorldMap_UpdateCurrentTileBlock`,
`WorldMap_PreloadCurrentTileBlock`), and `WorldMap_LoadGraphics` takes the
second graphics set of `gWorldMap_GraphicsResources` where the ground is
`WORLD_MAP_ENCOUNTER_REGION_SNOWFIELD`. When a menu or script starts,
`WorldMap_StartStillView` draws the current frame into ordinary tiles on
`BG1` and switches to `DISPCNT_MODE_0`; `WorldMap_LoadAreaGraphics` returns to
the moving view. The world map's data is in
[Layouts](../layout/README.md#the-world-maps-data).

| Name | Value | Meaning |
| --- | --- | --- |
| `WORLD_MAP_ENCOUNTER_REGION_SNOWFIELD` | 21 | the region whose ground takes the world map's second graphics set |

## Design

### One object, a few views

All 64 objects are the same 112 bytes, and the engine reads them as
`struct Object` (`include/object.h`): the script commands, the motion loop,
the cameras, the battle and the field effects all take an `Object *`. Two
halfwords near its end mean different things to different code, so `Object`
names each use in an anonymous union. Maps read the same object as
`struct ObjectEvent` (`include/object_event.h`), or, for an effect, as
`struct FieldEffect` (`include/system/field_effect.h`); their fields sit at
`Object`'s offsets, and an update hook takes a `union ObjectEventOrEffect *`.
A few views remain where [matching](../../glossary.md#matching-decompilation)
needs them, each a field-for-field copy that differs only where its reader
does, as [Matching](../MATCHING.md#views-of-one-struct) explains:

| View | Reader | What differs |
| --- | --- | --- |
| `ScriptOperands` (`include/field/action_script.h`) | `SC_SET`, `SC_ADD`, `SC_CMP` | every field at the width its operand handler reads, unsigned |
| `ObjectArmView` (`include/script_motion.h`) | `Object_UpdateAll` (`update.c`) | ARM code built with GCC 2.95.2, which cannot read `Object`'s anonymous unions |
| `BattleSpriteObject` (`include/battle/motobj.h`) | `BattleObject_SpawnObjectsForList` | the sprite word untyped |

[Camelot](../../glossary.md#camelot)'s source likely had one struct.

### Scripts without addresses

A jump stores a label number, not an offset, and `ObjectScript_FindLabel`
searches for it each time. A script is therefore a plain array of words that C
initializers can write, with no assembler to fix up its branches, at the cost
of a linear search on every jump and the 1,024-word reach.

### Routines run from RAM

The work done per scanline or per pixel is ARM code copied into RAM before it
runs: `Transform_UpdateVertices`, `TileMap_DrawRows`, the sprite frame decoder
(`Render_DecodeFrame`) and `Tile_ConvertMapGrid`. The cartridge's bus is 16
bits wide, so 32-bit ARM instructions run slowly from ROM, and the rest of the
game is Thumb. `Object_UpdateAll` is an ARM copy of the object update for the
first eight objects, but nothing runs it: it has none of the field's extras
(free flight, following the ground, floating, solid objects), so it could not
stand in for the Thumb update.

`field.c` is one object in the ROM; its parts are our cut of it, made where
one family of functions ends and the next begins.

## How the field fits with the rest of the game

[Running a map](../overworld/README.md) owns the map loop: it starts the
object system, loads the map and the overlay, places the object events and
handles requests, and this engine supplies the objects, scripts and ground
checks it uses. Overlays reach the engine through their
[import tables](../../glossary.md#entry-table-and-import-table)
(`src/map/import/`): nine import `MapGrid_GetTerrainHeight` and six
`MapGrid_SetMetatileBehaviorAt`, with which a map rewrites a cell's
[metatile behavior](../../glossary.md#metatile-behavior). The battle uses the
object system in `OBJECT_SYSTEM_BATTLE` for its own sprites. The Lost Age's
field code is described by [its own guide](../../../tla/src/field/README.md),
as differences from this one.

## Limits

- 64 objects; 64 sprites, sharing 64 sprite anims, up to four each.
- 8 sprite frame caches, 16 tileset animations and 4 palette cycles.
- `gObjectScript_CommandTable` has 47 entries: 36 commands, then 11 that act
  as `SC_END`. A label must lie within the first 1,024 words of its script.
- A step can climb 8 pixels and drop 12.

## Words used here

- An **object** is one of the 64 slots of `gObjects`, 112 bytes each, as the
  [glossary](../../glossary.md#object) defines it; a map knows its object
  events among them by [local id](../../glossary.md#local-id). A slot with no
  script is free.
- A **sprite** is one of the 64 `struct Sprite`s: an image and a shadow, two
  entries of OAM (the hardware's sprite list), and up to four **sprite anims**
  (`struct SpriteAnim`), animations composed into one picture.
- An **object script** is a list of 32-bit words: `SC_` commands of
  [`objcmd.h`](../../include/objcmd.h) and their operands. An **animation
  script** is a list of byte pairs that a sprite anim plays.
- A map **layer** is one of a town's three backgrounds
  ([Layouts](../layout/README.md#one-grid-three-layers)).
- Positions are 16.16 fixed-point pixels: x across the map, y up, z down the
  map. Angles are 16-bit fractions of a turn (`FACING_EAST` is 0,
  `FACING_SOUTH` a quarter turn).

In the code, `object` and `interpreter` are the object a command runs on,
`sprite` and `layer` a sprite and one of its anims, `work` or `gridWork` the
map work (`gMapGridWork[0]`), and `perspective` the world map's. Because the
parts compile as one file, a part's own types, macros and static helpers carry
its family's name (`ObjectCamera`, `Layout`, `WorldMap`), and a part undefines
its macros at its end. [Matching](../MATCHING.md) explains why some sites keep
an unusual spelling.

## Evidence

The command names (`SC_WAIT`, `SC_SYNC` and the rest) are the macros of
`objcmd.h`, named from what each handler does; the struct views are named from
the fields each family reads, and the layout words from the loader. None is
recovered from the original.

The update accepts command words up to 63 but the table has 47 entries, so a
word from 47 to 63 would call through whatever follows the table. No script
does: every script is written in `objcmd.h`'s macros, whose last command is
`SC_KEY_COUNT` (`0x23`), and none holds a bare word from `0x24` to 63.

The field maps sampled for a sprite with more than one drawn anim (Vale,
Bilibin's item shop and inn, Imil, Goma Cave, Xian, Tolbi and the world map)
have none: the leader on the world map has a second anim, `SPRITE_BLANK_32`,
which draws nothing. The recording of a sprite's anims is therefore of a
battle, whose units are the same `struct Sprite` composed by the same
`Sprite_ComposeFrame`.

Three halfwords of the map work between the next message and the camera
object, `unusedAfterMessage` and the pair `unusedBeforeCamera`, are reset by
`FieldScript_Start` at every script (to `0xffff`, -1 and -1) and read by
nothing.
