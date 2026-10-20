# Field

Whenever you're walking around in The Broken Seal, in a town, a dungeon or
out on the world map, the field engine is what you're looking at. It draws
the sprites of people and things and keeps up to 64 field objects moving,
each under a small script of its own. It turns the direction pad into steps,
loads and scrolls the layered maps of towns and dungeons, and tilts the
world map into perspective. Whenever other code needs to know about the
ground at some point, how high it is or whether anyone can stand there, it
asks the engine.

The engine doesn't decide what happens anywhere. That's the scene code's
job: [Field objects](../object/README.md) runs a field scene on this engine,
each place's [Scenes](../scene/README.md) overlay gives it its events, and
the map data the engine loads lives in [Maps](../map/README.md).

A little about the Game Boy Advance helps here. The console builds its
picture from up to four background layers, BG0 to BG3, with the hardware
sprites drawn among them. Each background is a grid of 8 by 8 pixel tiles
held in video memory (VRAM), and two of them, BG2 and BG3, can be affine:
the hardware scales and rotates them as it draws. The processor speaks two
instruction sets, compact 16-bit Thumb, in which almost all of the game is
written, and 32-bit ARM, which runs fastest from the small internal RAM, so
the game keeps a few busy routines in ARM and copies them there. DMA is the
console's hardware copier, which moves memory without the processor doing
the work. And some of the game's data is stored compressed with LZ, a scheme
that replaces repeated runs of bytes with references back to earlier ones.

## The files

| File | What it holds |
| --- | --- |
| `field.c` | the field object itself: a short guide that includes `field_internal.h`, then the 14 parts under `field/` in the order their code has in the ROM |
| `field_internal.h` | the parts' includes, and the few names more than one part uses; it places no bytes |
| `field/*.c` | the 14 parts, each compiled only through `field.c` |
| `update.c` | `Object_UpdateAll`, an ARM version of the object update for the first eight objects, written to be copied to RAM and run there; only the unused `Object_RunUpdateAllFromHeap` would run it |
| `vertices.c` | `Transform_UpdateVertices`, the ARM routine that turns the world map's ground plane into BG2's affine values, one set per scanline |
| `object_script.c`, `operand.c` | the object scripts the engine starts by itself, the script command table, and the handlers of the work values |
| `facing.c` | the facing tables, then small constant tables the parts read: the order a sprite's tile dissolves in, the fixed camera's matrix, the wave a floating object bobs on, the idle script, the direction pad's headings, the footprint and dust scripts, and how far the world map camera leans for each pad direction |
| `curve_data.c`, `mapdata.c` | the terrain height tables; the height function of each terrain kind, the world map's terrain behaviour table, and two small tables nothing in The Broken Seal reads |
| `world_map_resources.c`, `griddata.c` | the world map's two graphics sets, and the sprite viewer's scale |
| `lucky_dice.c` | the Lucky Dice minigame, with the camera defaults and 8.8 fixed-point helpers it uses |
| `layered.tsv` | the layered map table, `gMap_LayeredScenes`: each map's header, palette, three tile sheets and layer data, as resource numbers counted from `ResourceId_Map001` |
| `remap0.tsv` to `remap3.tsv` | `gRuntime_ByteRemapTable`, four 256-entry byte remap tables that some sprite colour modes, and a command icon's colours, are drawn through |
| `scenes.s` | the scene table, `gField_SceneTable`: each scene's overlay, its place group, whether it's a town, and its map; each row also defines the scene's `SceneId_` number |
| `common/effect/` | the ground particle and sparkle pictures of the field's weather effects ([`weather.c`](../object/psynergy_effects/weather.c)), which [`scripts.s`](../object/scripts.s) places |

The headers are in `games/tbs/include/unit/field/`, one for each C file
except `world_map_resources.c`, whose table `field.h` declares; the field
object's functions are declared in
[`unit/field/field.h`](../../include/unit/field/field.h). The order of all
these objects in the ROM is their rows of `games/tbs/objects.tsv`.

In the ROM, `field.c` is a single object. Splitting it into parts is our
doing: each cut falls where one family of functions ends and the next
begins. `field.c` includes the parts in ROM order, so they're compiled as
that one object and never on their own, and each part includes
`field_internal.h` as well, so it still makes sense when you open it by
itself. A part's opening lines say what it holds (`head -3 field/*.c`
prints them all).

## Words you'll see

A **field object** is one of the 64 slots of `gObjectSlots`, each 0x70 bytes
long (112 in decimal; a number written with `0x` is hexadecimal). The
party's characters, the people and things a scene places, the camera and the
footprints the leader leaves are all field objects. An object's main struct
is `struct ObjectRuntime`, but the same memory is also read as a
`DispatchObject`, `ScriptInterpreter`, `ScriptMotionObject`, `FieldActor`
and more, each naming only the fields its code uses;
[One object, many views](#one-object-many-views) explains why.

A **sprite** is a `struct AnimationObject`, one of 64. It has an image and a
shadow, which are two entries in OAM (the GBA's list of hardware sprites to
draw), and up to four **layers** (`struct AnimationEntry`), each playing its
own animation script over its frames. The engine composes a sprite's layers
into one pixel buffer in priority order and uploads that to video memory.

An **animation script** is a run of byte pairs, a byte being a number from 0
to 255. The first byte of a pair is usually a frame to show, with the second
saying for how many frames. A few values from 239 up are commands instead,
with the second byte as their argument: the `ANIM_CMD_` names of
`field_internal.h` cover 239 (end), 240 (pick a facing table), 241 (hold),
245 (wait longer), 253 (jump), 254 (switch animation) and 255 (show
nothing). The drawing code skips 242 to 244, 246 to 250 and 252 as unused,
and 251 shows as an ordinary frame. A layer's timer counts sixteenths of a
frame and normally loses 16 each frame, which is how a layer can be made to
play slower or faster.

An **object script** is a list of 32-bit words: an `SC_` command of
[`objcmd.h`](../../include/objcmd.h), then its operands, run through
`gScriptObject_CommandTable`. A script reads and changes its own object
through **work values** (`SW_`), which are the object's fields numbered as
`SC_SET`, `SC_ADD` and `SC_CMP` see them.

A **cell** is one 16 by 16 pixel square of a layered map, kept as a 32-bit
word in a grid 128 cells wide. Its low halfword (16 bits) holds the
**block** in its low 12 bits, a 2 by 2 group of 8-pixel tiles (a metatile),
and above that four flag bits, the layer and priority an object standing
there takes. Its third byte is the collision code that walls, exits and
events are keyed on, and its fourth picks its ground shape. A layered map
has three map **layers**, on BG3, BG2 and BG1, which have nothing to do with
a sprite's layers.

Positions are in 16.16 fixed-point pixels: the top 16 bits count whole
pixels and the low 16 bits a fraction, so one pixel is 0x10000. x runs
across the map, y up, and z down the map. Angles are 16-bit fractions of a
turn, so 0x4000 is a quarter turn.

Reading the code, you'll meet the same few local names again and again.
`object` and `interpreter` are the object a command is running on, `sprite`
and `layer` a sprite and one of its layers, `work` or `mapWork` the map's
working state (`gMapWork[0]`), and `perspective` the world map's.

## Function prefixes

The folder's functions keep the prefixes of the engine family they belong
to, since that's how other folders call them:

| Prefix | What it covers |
| --- | --- |
| `Sprite_`, `Render_` | drawing sprites |
| `Animation_`, `AnimationObjects_`, `ResourceMetadata_`, `ResourceObject_`, `ResourceSlot_` | keeping sprites, their layers and the resource slots they draw from |
| `ObjectSystem_`, `ObjectDispatch_`, `FieldObject_`, `Object_` | keeping and moving objects |
| `Script_` | object script commands |
| `Field_` | the configured keys |
| `Map_`, `Tilemap_` | the layered maps and their cells; `TileMap_DrawRowsFromRam` is spelled after `TileMap_DrawRows`, the routine it runs |
| `WorldMap_` | the world map |
| `GetWorldMap`, `CheckWorldMap` | the ground under a point on the world map; `GetMapCellCollision`, `SetMapCellCollision` and `CheckMapPositionCellOccupied` ask the same of a layered map |
| `MapAnimation_`, `DisplayBlend_`, `PaletteQueue_` | the timed map effects: tile animation, the blend script and palette cycles |
| `Curve_` | the ground height functions |
| `Ui_` | the debug sprite viewer |
| `LuckyDice_` | the Lucky Dice minigame |

A few functions take the prefix of the one thing they act on instead, such
as `Resource_DecompressLz` or `Camera_FollowLeaderInBounds`. Some names no
longer say quite what their code does, but other folders or The Lost Age's
copy of this code use them too, so they change only in a rename across all
of them.

Since all 14 parts are compiled together as one file, a part's own types,
macros and static helpers carry the name of the part or its family
(`ObjectCamera`, `LayeredScene`, `WorldMap`), so they can't collide with
another part's and you can tell where each comes from. For the same reason a
part undefines its macros at its end, before the next part begins.

A few sites spell something in an unusual way because that's what compiles
to the original bytes. [Matching](../MATCHING.md) lists those spellings and
explains the markers you'll see in comments.

## How it works

### How an object's script runs

Every field object in use runs a script, even if it's only the idle one;
an object with no script is a free slot. The camera trailing the leader, an
actor wandering near its home and a footprint waiting to disappear are all
scripts, and so is the leader, since the player's control is itself a
script command run once a frame.

![Isaac walking through the snow in Imil, leaving footprints that fade behind him](https://alchemy.pascalpixel.com/guides/field/snow.gif)

In Imil's snow you can watch that happen. Every 18 frames of walking, or 12
of running, the player's control creates a footprint, field object kind 25,
and starts it on a script two words long, `SC_WAIT(120), SC_FREE`
(`gWorldMap_FootprintActions`, in `facing.c`). Each footprint you see is one
of the 64 objects, waiting out its four seconds before it frees itself.

A script is an array of 32-bit words, and its cursor is a word index into
it. Each frame, for each live object, the update first runs the object's
hook (even for a paused object, which does nothing else that frame). Then it
counts down the object's wait if it has one, and otherwise runs commands
until one of them yields. A command is a function in
`gScriptObject_CommandTable` that takes the object and returns 1 or 0.

A command that returns 1 has done its work and moved the cursor past itself
and its operands, and the next command runs in the same frame. Tests,
jumps, flags and work-value arithmetic all return 1, so a script can decide
and branch without losing a frame.

A command that returns 0 ends the object's frame, and the same cursor runs
again next frame. A command that's waiting for something returns 0 without
moving the cursor and returns 1 once it's satisfied: `SC_SYNC` waits until
the object reaches its target, or until a solid object has blocked it for
60 frames. `SC_WAIT(n)` moves past itself, sets the wait count to n - 1
(the current frame counts as the first) and returns 0.

Words above 63 aren't commands, and the loop steps over them. That's what
lets a label sit in the stream. `SC_LABEL(n)` is a word with its top bit set
and the label's number in the bits from 16 up, which execution skips. A
jump's operand is `SC_TO(n)`, the same word with bit 30 also set.
`Script_FindLabel` clears bit 30 from the operand and searches the first
1,024 words for the result, so it finds the label and never the jump's own
operand, and the script carries on just past the label. A label it can't
find, and `SC_TOP` (0), send the script back to its start. Because every
jump searches for its label rather than storing an offset, a script needs no
assembler to fix up its branches.

`SC_END` doesn't free the object. It points the script at
`gScript_MainScript`, a lone `SC_END`, and returns 0, so the object idles,
running that one command each frame until something gives it a new script.
`SC_FREE` is what frees it.

The scripts the engine starts by itself are in `object_script.c`, and each
repeats one command every frame: the command, `SC_WAIT(1)`, then
`SC_LOOP(SC_FOREVER, SC_TOP)`. Each command is a function with a row in
`gScriptObject_CommandTable` and a macro in `objcmd.h`. Most of the
functions are in `script_commands.c`; `SC_BASE`, `SC_B54_OFF` and
`SC_B54_ON` sit at the top of `object_motion.c`, and the controls
(`SC_PLAYER`, `SC_KEYS`, `SC_WORLD` and `SC_KEY_COUNT`) live in
`player_control.c`.

The loop that runs scripts is `Object_UpdateAllThumb`
(`field/object_motion.c`). It covers all 64 objects and adds the field's
extras: free flight in 3D, following the terrain, floating, and solid
objects that refuse to overlap. In battle, objects move with
`Object_UpdateAllMotion`, which runs no scripts. `Object_UpdateAll`
(`update.c`) is an ARM copy of the same loop for the first eight objects,
written to be copied to RAM. The only routine that copies and runs it,
`Object_RunUpdateAllFromHeap`, is called by nothing, so the ROM keeps it
unused.

### How the player moves

When the scene loop hands control back to you, it starts one of the
engine's own scripts on the leader: `SC_PLAYER` on a layered map, `SC_WORLD`
on the world map, `SC_KEYS` (8-pixel steps by the direction pad) while
climbing, or only `SC_KEY_COUNT`, a check of the configured keys, while the
party is carried through the air.

The speeds are set at the top of each control function, as a `speedLimit`
and an `acceleration` in 16.16 pixels per frame. On a layered map,
`FieldObject_UpdatePlayerControl` (`player_control.c`) walks the leader at 1
pixel a frame (0x10000) and runs at 1.5 (0x18000) while a dash key is held,
both with an acceleration of a quarter pixel (0x4000); with game flag 383
(0x17f) set, holding B moves at 4. On the world map, `Object_MoveOnWorldMap` walks at
half a pixel a frame and 1 with a dash key, and L raises the limit to 4
pixels with no debug test or flag guarding it.

![Isaac walking right through Vale, then running back the other way](https://alchemy.pascalpixel.com/guides/field/walk-run.gif)

Here Isaac walks right for two seconds and then runs back with B held: the
same two seconds take him about half as far again.

Before each step the control probes ahead. On a layered map it tries 8
pixels the way the pad points and the headings 22.5 and 45 degrees either
side, then headings up to 67.5 degrees off, which is how the leader slides
along walls. On the world map the probe reaches
`WORLD_MAP_PROBE_RADIUS`, set at the top of `player_control.c`: 8 pixels in
the Japanese release and 7 from the English one on. The English release
also tests 22.5 and 45 degrees either side and then headings up to 90
degrees off, where the Japanese one tests 30 degrees either side and then up
to 67.5. On the world map, R and L raise fixed events instead of their
shortcuts: R brings up
[the world map screen](../object/README.md#the-world-map-screen) and L
zooms the view out. They do it for as long as they're held in the
Japanese release, and once per press from the English one on.

Whether a step is allowed comes from the ground. `Map_CheckObjectStep`
refuses a wall (collision code 0xff), ground more than 8 pixels higher than
the object's and ground more than 12 pixels lower.

### One object, many views

Every field object is the same 0x70-byte block of memory, but different
families of code read it through different structs. The engine's code
alone uses eight: `ObjectRuntime` (`include/object_runtime.h`), `DispatchObject`
(`include/object_dispatch.h`), `ScriptInterpreter` and `ScriptOperands`
(`include/field/script.h`), `ScriptMotionObject`
(`include/script_motion.h`), `ScriptObjectRuntime`
(`include/field/script_object_runtime.h`), `ScriptObjectEntry`
(`include/script_object_entry.h`) and `FieldActor` (`include/field_obj.h`).
Other folders add their own, such as the battle's `MotionObject`, the field
effects' `FieldEffect` and `EffectObj` in `object/object/field_fx.c`.

Each was written from the code that reads it and names only the fields that
code uses; `ScriptInterpreter` covers only the first 0x64 bytes, which is
all the commands read. They agree on most of the block (the script at offset
0, the position at 8, the velocity at 0x24, the target at 0x38) and
disagree where their readers do:

- The halfword at 0x5e is `wait` in the actor, runtime, interpreter, motion
  and operand views, and the dispatcher keeps it among its unnamed bytes.
  The interpreter and motion views read it signed, the actor and runtime
  views unsigned. `SC_WAIT` writes the frame count less one, and both update
  loops count it down.
- The word at 0x6c is the per-frame routine: `hook` in
  `ScriptMotionObject`, `update` in `FieldActor`, `unk6c` in
  `ObjectRuntime`. Both update loops call it, and the work value `SW_HOOK`
  addresses it. `ObjectRuntime` calls the word at 0x60 `callback`, but no
  code calls through it: its bytes are the refused count and the frozen flag
  (`ScriptMotionObject`), then two bytes only scene code uses.
- 0x18 is a scale pair in `FieldActor` and `ScriptOperands` and unknown
  bytes in `ObjectRuntime`, and 0x44 is the bounce in `FieldActor`,
  `ScriptMotionObject` and `ScriptOperands` but unknown in `ObjectRuntime`
  and the dispatcher.

They're kept apart, rather than merged into one struct, because the views
are also what the code matches with. The compiler addresses a field
differently depending on its declared type and on whether it's reached
through a union (the `FieldActor` union of one sprite or a group of them
moves a load, as [Matching](../MATCHING.md) records among the scene
spellings), so one merged struct would change the bytes at sites each view
now matches. Even `gObjectSlots` is declared as an `ObjectRuntime *` in
`include/field/field.h` and as a `ScriptMotionObject *` in `update.c`. XXX:
likely one struct in Camelot's source, because every view describes the
same 0x70 bytes; merging them waits until every field has one name and one
type that every site matches with.

### The layered map

Towns and dungeons are drawn on three of the GBA's four background layers
(BG0 to BG3), all of them windows onto one shared grid of cells. Each layer
has its own parallax, drift and wrap, so it can scroll at its own rate as
the view moves, or drift by itself.

![Vale pulled apart into its layers: BG3 at the back, then BG2, the sprites and BG1 in front](https://alchemy.pascalpixel.com/guides/field/layers-3d.gif)

Here is Vale pulled apart into its layers and put back together. At the back
is BG3, the ground. In front of it, BG2 holds the Sanctum's walls and door and
the cliff edges. Then come the sprites, Isaac among them, and BG1 goes in front
of everything with the tree top, the Sanctum's roof and the tops of the stair
posts. That's how Isaac can walk under a roof or behind a tree: he's drawn
between two layers of the same map.

The grid is 128 by 128 cells, 64 KB, decoded into `Ram_MapCellBuffer`. Each
layer's window starts at the cell its header gives (`Map_LoadLayeredScene`
sets `layers[n].cells` to the cell at half the header's 8-pixel tile
position, since a cell is 2 by 2 tiles).

Layer 0 shows on BG3, layer 1 on BG2 and layer 2 on BG1, which leaves BG0
for the windows. A layer's priority byte in the header doubles as its
switch: `Map_LoadLayeredScene` turns a layer on only when its priority isn't
zero, so map layers use priorities 1 to 3.

Collision and height work per layer too. An object's `terrainId` names the
layer whose ground it follows, and code that asks about the ground reads
the cell at that point in that layer's window, then looks up its shape. The
map format, and how the build makes it, are in [Maps](../map/README.md).

### The world map

The world map is drawn in perspective, with the ground tilting away towards
the horizon. The map itself is flat: BG2 and BG3 are affine backgrounds,
and their scaling changes on every scanline of the screen to make the ground
recede. When the camera's pitch
changes, `WorldMap_BuildScanlineTable` gives each line of the screen a width
scale and a ground distance, or zeros above the horizon. Each frame
`Transform_UpdateVertices`, an ARM routine run from RAM, turns them into the
frame's page of affine values. `MapAnimation_ApplyAffineFrame` then sets the
first scanline's values and starts a DMA transfer that runs in the short
pause after each scanline (the HBlank), feeding the rest of the lines theirs
as the screen is drawn. There are two pages, and the frames alternate
between them.

![Isaac walking and running on the world map, the ground tilting away towards the horizon](https://alchemy.pascalpixel.com/guides/field/world-map-walk.gif)

The map under Isaac here is flat. Only the scaling changes from one scanline
to the next: a line near the horizon covers far more of the map than a line
at the bottom of the screen, and that is all it takes for the ground to tilt
away.

The map is made of 32-pixel cells, and the tiles around the player are
loaded one a frame as you walk (`Map_PreloadCurrentTileBlock`). There are
two graphics sets, `gWorldMap_GraphicsResources`, and
`WorldMap_LoadGraphics` takes the second where the ground is terrain kind
21.

When a menu or event starts on the world map, `WorldMap_BeginStillView`
freezes the perspective view as a still picture on BG1: it draws the current
affine frame into pixels with the ARM routine `TileMap_DrawRows`
(through `TileMap_DrawRowsFromRam`) and shows them as 256-colour tiles.
`Map_LoadAreaGraphics` reloads the area's palette and tiles afterwards and
returns to the moving view.

## How the field fits with the rest of the game

The object system is set up by whoever needs it. The field scene runner
(`object/object/run_scene.c`) starts it in mode 2 for a layered map and in
`MODE_WORLD_MAP` (3) for the world map, the battle's encounter in mode 4,
and the debug sprite viewer in mode 2. Layered maps use the scrolling camera
(`ObjectSystem_UpdateCamera`), and modes 3 and 4 the fixed camera
(`ObjectSystem_UpdateCameraFixed`), which projects 3D points onto the
screen. The scheduler runs the object update before the camera, so objects
move before the camera follows them.

The battle effects, the Psynergy effects and the menus place their own
sprites through the engine (`Object_ApplyProjectedPlacementFar`). A menu
can suspend sprite projection (`ObjectSystemWork.suspended`) and the tile
animations (`Menu_SuspendFieldTasks`) while it runs. Scenes reach the
engine through its far calls, such as `Map_GetTerrainHeightFar`, which about
ten scene overlays use, and `SetMapCellCollisionFar`, with which a scene
rewrites a cell's collision code. The Lost Age's field code is described by
[its own guide](../../../tla/src/field/README.md), which explains how it
differs from this one.

## Limits

- 64 field objects, and 64 sprites that share 64 animation layer records
  between them, up to four each.
- 8 resource slots, 16 tile animation channels and 4 palette cycles. Only
  `PaletteQueue_Start` schedules the palette cycles, and nothing calls it,
  so no palette cycle runs in the game.
- The script command table has 47 entries. The 36 from 0x00 to 0x23 are the
  commands, and the 11 from 0x24 on all act as `SC_END`. A label must be
  within the first 1,024 words of its script.

## Evidence and open questions

The command names (`SC_WAIT`, `SC_SYNC` and the rest) are the macros of
`objcmd.h`, named from what each handler does; the struct views are named
from the fields each family reads, and the map's words from the loader.
None of them is recovered from the original.

Still unknown:

- XXX: the update accepts command words up to 63, but the table has 47
  entries, so a word from 47 to 63 would call through whatever follows the
  table.
- XXX: why placeholder blocks (0xfff) are given numbers of their own as the
  map is converted (`Tile_ConvertMap`, and the unused
  `Tilemap_NumberPlaceholderBlocks` that does the same in C).
