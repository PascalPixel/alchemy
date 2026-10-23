# Field

![The field's loop. The party walks, and every frame the game checks the ground under the leader and the buttons. When something comes up (a doorway, a random battle, the A button, the menu, Psynergy or an item), walking stops, the game handles one thing at a time until nothing is left, and walking resumes. Leaving the map, through a doorway or into a battle, is always handled first and ends the loop.](https://alchemy.pascalpixel.com/illustrations/field/scene-loop.svg)

While the [party](../../glossary.md#the-party) walks around a town, a dungeon or the world map, the game
asks once a [frame](../../glossary.md#frame) whether something should happen. Has the leader stepped
into a doorway? Has a random battle started? Has the player pressed A beside
someone, or opened the menu? When something has, the walking stops, the game
deals with that one thing, and the walking starts again. Nearly everything
that happens outside a battle passes through that loop.

This folder is the field engine that runs under that loop. It keeps track
of everything that moves (the party, the townspeople, the camera, even
footprints), moves each of them a little every frame, decides whether the
ground ahead can be walked on, and draws the [map](../../glossary.md#map): a town as three flat layers,
or the world map tilted into perspective. It decides nothing about any one
map.

Three other folders hold the rest:

- The loop that drives the engine is in `src/object`, whose page is
  [Running a map](../object/README.md). The folder is named after the
  [bank](../../glossary.md#bank) of the program it fills, not after the
  objects described here.
- Each map's own code, its [scripts](../../glossary.md#script) included, is
  an [overlay](../../glossary.md#overlay), a piece of code loaded into RAM
  for that map alone: [Maps](../map/README.md).
- What a town or dungeon shows and stands on is its
  [layout](../../glossary.md#layout): [Layouts](../layout/README.md).

[World](../../world.md) gives the whole layer in brief.

## The walk and what interrupts it

A field map switches between two states: walking, when the player has
control and the game watches for anything that should interrupt, and
handling, when the player has none and the game works through whatever came
up, one thing at a time.

Three kinds of thing can interrupt the walk:

- The ground the leader steps onto: a doorway, a spot that starts a script, a
  ledge to jump down, or ground that adds to the count towards a random
  battle until one starts.
- The buttons: A to talk to someone or search something ahead (with nothing
  there, A opens the menu), the menu button, Start, and the two shortcut
  buttons, which use the [Psynergy](../../glossary.md#psynergy-and-pp) or item the player assigned to them.
- A script that is already running, which can ask for another script or a
  battle of its own.

Each of these raises a flag of its own, a request. The game handles the
requests in a fixed order:

1. leaving the map;
2. a script something asked for;
3. a battle;
4. the cell the leader stepped onto;
5. a ledge or climbing spot the leader pushes against;
6. the A button, which becomes talking, searching or the menu;
7. then the menu, talking, searching, Psynergy, an item and Start.

Leaving comes first: once a doorway or a battle has been asked for, the map
ends before anything raised with it is looked at. After each request the
game starts again from the top of the list, and when no flag is left the
player gets control back.

That is why a cutscene is written as one script that runs from start to end.
When the leader steps on a spot that starts one, the walking stops, the
script plays to its last line while the party waits, and only then does the
walking resume. Whatever has to keep moving during the cutscene (someone
walking across the screen, the camera) moves on its own, as an object script
(below).

A random battle is handled as a way out of the map, and the map is loaded
again afterwards with everyone put back ([Running a
map](../object/README.md#the-map-loop-handles-requests-not-scripts)).

In the code, `Map_Run` ([`object/run_map.c`](../object/object/run_map.c))
runs one map. Its inner loop is the walk: it waits a frame and calls
`Field_ProcessStep`, which checks the ground under the leader, until
`Field_CountRequests` finds a request. The leader's control script reads the
buttons through `Field_CheckConfiguredKeys`. The requests are fields of
`struct FieldWork`; [Running a map](../object/README.md#the-map-loop-handles-requests-not-scripts)
gives each field and how it is handled. A
[warp](../../glossary.md#warp) is `Map_Run`'s result, which `Game_Run` passes to `Map_TakeWarp`
to load the next map.

## Everything that moves runs a script

Everything on the field that moves or changes by itself is a [field object](../../glossary.md#field-object),
and each one runs a small script of steps.

![Three field objects and their scripts. The leader moves by the direction pad, waits one frame and repeats, for good. The camera follows the leader, waits one frame and repeats. A footprint waits 120 frames and then disappears. Each frame every object runs its steps in order until one says to wait, which ends its turn until the next frame. Below, Isaac in Imil's snow as the game draws him, with the footprints his steps left beside him.](https://alchemy.pascalpixel.com/illustrations/field/object-scripts.svg)

The field objects are the party, the people and things a map places (its
[object events](../../glossary.md#object-event)), the camera, even the footprints the leader leaves, and there
are 64 slots for them. Every frame each object takes a turn: it runs its steps in order until
one says to wait, which ends its turn until a later frame. A step that only
tests something or jumps to another part of the script takes no time, so a
script can make a decision within a single frame.

The player's control is itself one of these steps. The leader's script is
"move by the pad, wait a frame, repeat"; the camera's is "follow the leader,
wait a frame, repeat"; a footprint's is "wait 120 frames, disappear". The
leader leaves a footprint every 18 frames of walking or 12 of running, on the
maps that show them.

### How a script runs

A script is a list of 32-bit words: each step is a command word, followed by
the values it needs. The commands are the `SC_` macros of
[`objcmd.h`](../../include/objcmd.h). This is the start of the walk a woman
in Vault takes while she looks for the young man who should be fixing her
roof (`gVault_WomanWantingTheRoofFixedRoute` in `map/vault.c`, shortened):

```c
s32 gVault_WomanWantingTheRoofFixedRoute[] = {
    SC_SET(SW_SPEED, PIXELS(2) / 5),
    SC_SET(SW_ACCELERATION, PIXELS(1) / 5),
    SC_LABEL(1),
    SC_SET(SW_FACING, FACING_WEST),
    SC_ANIMATION(2),                      // walking
    SC_MOVE(PIXELS(168), 0, PIXELS(312)), // set a target
    SC_SYNC,                              // wait until she reaches it
    SC_ANIMATION(1),                      // standing
    SC_WAIT(20),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(20),
    /* ... the same to the other end of her path ... */
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};
```

`SC_SET` writes one of the object's work values, the fields `SW_` numbers
(its speed, its facing), which `SC_ADD` adds to and `SC_CMP` tests. Everything up to `SC_SYNC` happens in one frame;
`SC_SYNC` and each `SC_WAIT` end her turn.

`Object_UpdateAllThumb` (`field/object_motion.c`) gives every object with a
script its turn. It first calls the object's hook, if it has one: a function
the object runs every frame with itself, which a map can set to give one
object extra behaviour. The hook runs even when the object is paused, a flag
a map sets to hold an object where it is; a paused object does nothing else
that frame, neither its script nor its motion. Otherwise, if the object is
still waiting, the update counts its wait down; if not, the update runs
commands until one yields. A command word is an index into
`gObjectScript_CommandTable`, whose entry takes the object and returns 1 or
0:

- 1 means the command has finished and moved past its values, and the next
  command runs in the same frame. Tests, jumps, flags and arithmetic return
  1, so a script branches without spending a frame.
- 0 ends the object's turn. A command that waits for something returns 0
  without moving on until it is satisfied: `SC_SYNC` waits until the object
  reaches its target, or until a solid object has blocked it for 60 frames.
  `SC_WAIT(n)` moves past itself, sets the wait to n - 1, since the current
  frame counts as the first, and returns 0.

A word above 63 is not a command, and the update steps over it. That is what
lets a label sit in the list: `SC_LABEL(n)` is a word with its top bit set and
n in bits 16 up. A jump's target, `SC_TO(n)`, is the same word with bit 30
also set. `ObjectScript_FindLabel` clears bit 30 and searches the first 1,024
words for the result:

```text
SC_LABEL(1)               0x80010000
SC_TO(1)                  0xc0010000
SC_TO(1) without bit 30   0x80010000   equals the label, not the jump's own word
```

The script continues after the label. A label it cannot find, and `SC_TOP`
(0), restart the script.

`SC_END` does not free the object: it points the script at
`gScript_MainScript`, a lone `SC_END`, so the object idles until something
gives it another script. `SC_FREE` frees the slot. A footprint is an object of
`FIELD_SPRITE_FOOTPRINT` that the player's control starts on
`gWorldMap_FootprintActions`, `SC_WAIT(120), SC_FREE`.

The scripts the engine starts by itself (`object_script.c`) each repeat one
command: the command, `SC_WAIT(1)`, `SC_LOOP(SC_FOREVER, SC_TOP)`. Most
command handlers are in `field/script_commands.c`; `SC_BASE`, `SC_HIDE_SPRITES`
and `SC_SHOW_SPRITE` are at the top of `field/object_motion.c`, and the controls
(`SC_PLAYER`, `SC_KEYS`, `SC_WORLD`, `SC_KEY_COUNT`) are in
`field/player_control.c`.

After the scripts, the same loop moves each object that isn't frozen (a
frozen object still runs its script but is not moved): free
flight in 3D towards its target, following the ground, floating, and solid
objects refusing to overlap. In a battle, `Object_UpdateAllMotion` replaces
it; it moves the first 14 objects and runs no scripts.

The objects are the 64 slots of `gObjects`, 112 bytes each, in a block
of memory borrowed from the [heap](../../glossary.md#heap-and-pools) under the [heap slot](../../glossary.md#heap-slot)
`HEAP_SLOT_OBJECTS`, and the
handlers are the rows of
`gObjectScript_CommandTable` (`object_script.c`).

| Name | Value | Meaning |
| --- | --- | --- |
| `OBJECT_COUNT` | 64 | field objects, the slots of `gObjects` |
| `SC_WAIT(n)` | `0x00`, n | end the turn and wait n frames |
| `SC_SYNC` | `0x01` | wait until the object reaches its target |
| `SC_CAMERA` | `0x0b` | follow the leader |
| `SC_LOOP(n, to)` | `0x0c`, n, to | jump to label `to` n - 1 times, so what lies between runs n times |
| `SC_END` | `0x10` | return to `gScript_MainScript` and idle |
| `SC_PLAYER` | `0x18` | the player's control on a town or dungeon map |
| `SC_KEYS` | `0x19` | move 8 pixels a step by the direction pad |
| `SC_WORLD` | `0x1a` | the player's control on the world map |
| `SC_FREE` | `0x1b` | free the object's slot |
| `SC_KEY_COUNT` | `0x23` | check the configured buttons; the last command |
| `SC_LABEL(n)` | `0x80000000 \| n << 16` | a label: a word the update steps over |
| `SC_TO(n)` | `0xc0000000 \| n << 16` | a jump's target, naming label n |
| `SC_TOP` | 0 | the script's start |
| `SC_FOREVER` | `0xffff` | an `SC_LOOP` count that never runs out |

## Sprites and their animation

What the player sees of an object is its [sprite](../../glossary.md#sprite-and-oam), built from up to four
pictures laid over each other, each playing its own animation.

![Isaac's sprite in a battle is three pictures laid over each other: a dark shadow, drawn first, then his body, then his sword on top. The game combines them into one picture and redraws it whenever any layer moves on to another frame. As he attacks, his body and his sword change frame together, so each pose shows the sword in his hand, while the shadow stays the same.](https://alchemy.pascalpixel.com/illustrations/field/sprite-layers.svg)

The game redraws a sprite whenever one of its layers changes frame. In a
battle, Isaac's body, his sword and the shadow under his feet are three layers
of one sprite, and as he swings, the body and the sword change frame together.

Each layer of a sprite plays an animation script of byte pairs. A first byte
below `ANIMCMD_END` (239) is a frame to show and the second byte how
long. The layer's timer counts sixteenths of a frame and loses
`SpriteAnim.step` each frame, `ANIMATION_NORMAL_STEP` (16) at normal
speed, so a layer can be made to play slower or faster. From 239 up, the
first byte is a command (`enum AnimationCommand` in `field_internal.h`):
`ANIMCMD_END`, `ANIMCMD_SET_KIND` (choose a facing
table, below), `ANIMCMD_HOLD`, `ANIMCMD_WAIT`,
`ANIMCMD_JUMP`, `ANIMCMD_PLAY` (switch animation) and
`ANIMCMD_BLANK`. The drawing code skips 242 to 244, 246 to 250 and
252 as unused, and shows 251 as an ordinary frame.

When any layer's frame changes, `Sprite_ComposeFrame` redraws the
sprite: it decodes each layer's frame into one pixel buffer in priority order,
adds an outline if asked, and uploads the result to the sprite's tiles in
video memory. A layer's frames come from the sprite's data on the cartridge,
or from one of eight resource slots, buffers that hold an animation's frames
once they are loaded.

A script names frames as if the sprite always faced one way. A facing table
turns that into the frame to draw: for each eighth of a turn the sprite can
face, it gives a number to add to the script's frame and whether to mirror
the picture (`facing.c`).

Isaac's battle sprite shows how the layers combine.
`BattleObject_SpawnObjectsForList` (`../battle/battle/objects.c`) gives it
three layers: his body (`BattleObject.resource`) and his weapon
(`BattleObject.overlay`) at priority 1, and the 64 by 64 sprite 510
(`BattleObject.effect`) at priority 0. The two at priority 1 are drawn in layer
order, so the weapon lies over the body; the layer at priority 0 is drawn
first, so its shadow lies under his feet. The body and the weapon play scripts
with the same frame numbers, so each change of pose changes both and redraws
the whole buffer. The party is seen from behind in a battle, so the facing
table adds 1 to each frame a script names: the script of his swing names
frames 12, 14 and 16, and the sheets' frames 13, 15 and 17 are drawn. The
picture composes those frames from the sprite sheets themselves (`spr_300.png`,
`spr_320.png` and `spr_510.png` in [`graphics/sprite`](../graphics/sprite))
in the order `Sprite_ComposeFrame` uses.

A layer can also be recoloured as it is drawn: its colour mode
(`SpriteAnim.colorMode`, `enum SpriteColorMode`) either picks one of nine
256-byte tables that map each pixel to another colour (the five
`gSpriteColorTables`, then the four of `gRuntime_ByteRemapTable`,
which lie one after another), or paints every drawn pixel one colour. The
first remap table also recolours the menus' command icons
(`Runtime_RemapBytesByTable`).

A sprite is a `struct Sprite`, and each of its layers a
`struct SpriteAnim`; the four remap tables are `remap0.tsv` to
`remap3.tsv`.

| Name | Value | Meaning |
| --- | --- | --- |
| `ANIMATION_OBJECT_COUNT` | 64 | sprites, and the layer records they share |
| `ANIMATION_LAYER_COUNT` | 4 | layers a sprite composes, `Sprite.entries` |
| `ANIMATION_NORMAL_STEP` | 16 | sixteenths of a frame off a layer's timer each frame: normal speed |
| `ANIMCMD_END` | 239 | blank the layer and stop its script |
| `ANIMCMD_SET_KIND` | 240 | use facing table `arg` |
| `ANIMCMD_HOLD` | 241 | stay on this command and frame for good |
| `ANIMCMD_WAIT` | 245 | keep the current frame `arg` frames longer |
| `ANIMCMD_JUMP` | 253 | continue at script position `arg` |
| `ANIMCMD_PLAY` | 254 | switch to animation `arg` |
| `ANIMCMD_BLANK` | 255 | show nothing for `arg` |

## Objects move first, then the camera draws them

Every frame, all the objects move first, and then the camera works out where
each one appears on the screen and what it is in front of or behind. Because
that order never changes, each sprite is drawn where its object has just
moved to.

The two steps are two [tasks](../../glossary.md#task) of the scheduler, the list of jobs the game runs
once a frame. `ObjectSystem_Initialize` sets the engine up for one kind of
map and schedules both: the object update at order 3210 (`0xc8a`) and a camera at
3200 (`0xc80`). The scheduler runs higher orders first, so objects move before the
camera places them.

| Mode | Started by | Object update | Camera |
| --- | --- | --- | --- |
| `OBJECT_SYSTEM_LAYOUT` (2) | the field map runner (`object/object/run_map.c`), the debug sprite viewer | `Object_UpdateAllThumb` | `ObjectSystem_PlaceSprites`, which scrolls with the map |
| `OBJECT_SYSTEM_WORLD_MAP` (3) | the field map runner | `Object_UpdateAllThumb` | `ObjectSystem_ProjectSprites`, which projects 3D points onto the screen |
| `OBJECT_SYSTEM_BATTLE` (4) | the encounter (`battle/battle/encounter.c`) | `Object_UpdateAllMotion` | `ObjectSystem_ProjectSprites` |

The camera places each object's sprite and queues it by depth, and the
frame's sprite list, the list of every sprite's position and picture that
the console keeps in its OAM (object attribute memory), is built from that
queue. The battle effects, the
Psynergy effects and the menus place their own sprites through the same
routine, `Object_ApplyProjectedPlacementFar`. While the item, Psynergy,
Status, [Djinn](../../glossary.md#djinn) or debug menu is open, `ObjectSystemWork.suspended` is set, so
sprites that are not held are not drawn and give up their video memory.

## Walking and running

Before each step the leader takes, the game tests the ground ahead, and
against a wall it tries other directions so the leader slides along it.

![The leader seen from above, with the direction pad held to the right. Before each step the game checks the ground 8 pixels away in five directions: the way the pad points, and 22.5 and 45 degrees to either side. On open ground all five are clear, and the leader steps the way the pad points. Walking into a slanting wall, three of the five land beyond the wall, so the game tries other headings, nearest first: 22.5, then 45, then 67.5 degrees to either side, each needing five clear checks of its own. Here the first to pass is 67.5 degrees towards the open side, so the leader slides along the wall instead of stopping.](https://alchemy.pascalpixel.com/illustrations/world/probe-fan.svg)

The leader walks at one pixel a frame, and runs at one and a half while the
dash button, B, is held. Before each step the game tests the ground 8 pixels
ahead. If the leader is walking into a wall, it tries directions further and
further to either side, and takes the first one that is open, so the leader
slides along the wall instead of stopping dead.

When the map loop gives control back to the player, it starts one of the
engine's scripts on the leader: `SC_PLAYER` on a town or dungeon map,
`SC_WORLD` on the world map, `SC_KEYS` (8-pixel steps by the pad) while
climbing, or only `SC_KEY_COUNT`, which checks the configured buttons, while
the party is carried through the air.

Each control sets the leader's `speedLimit` and `acceleration` from the
speeds named at the top of `field/player_control.c`. On a town or dungeon
map, `Object_UpdateLeaderControl` walks at `FIELD_WALK_SPEED` (1 pixel a
frame) and runs at `FIELD_RUN_SPEED` (1.5) while a key of
`gGameState.dashKeys` (B) is held,
gaining `FIELD_ACCELERATION` (a quarter pixel) a frame. With `FLAG_FAST_WALK`
set, holding B moves at `FIELD_FAST_WALK_SPEED` (4); no code in [The Broken
Seal](../../glossary.md#the-two-games) sets that flag. On the world map, `Object_MoveOnWorldMap` walks at
`WORLD_MAP_WALK_SPEED` (half a pixel a frame) and runs at
`WORLD_MAP_RUN_SPEED` (1), and L raises the limit to `WORLD_MAP_FAST_SPEED`
(4) while it repeats.

Before each step the control tests the ground ahead with the checks below. On
a town or dungeon map it tries 8 pixels the way the pad points and the
headings 22.5 and 45 degrees either side, then headings up to 67.5 degrees
off. On the world map the test reaches `WORLD_MAP_PROBE_RADIUS`. The Japanese
edition and [the international editions](../../glossary.md#editions)
(English, German, Spanish, French and Italian) differ here, and a block of
`#if`s at the top of `field/player_control.c` chooses the values:

| | Japanese | International |
| --- | --- | --- |
| world map probe | 8 pixels; the step and 30 degrees either side, then up to 67.5 degrees off | 7 pixels; the step and 22.5 and 45 degrees either side, then up to 90 degrees off |
| R and L on the world map | raise their events while held | raise them once a press |

R raises `FIELD_RAISED_EVENT_REGION_MAP`, which opens [the world map
screen](../object/README.md#the-world-map-screen), and L
`FIELD_RAISED_EVENT_WORLD_MAP_ZOOM`, which zooms the view out.

Speeds are 16.16 fixed-point pixels a frame: the top 16 bits are whole
pixels and the low 16 bits a fraction, so `0x10000` is one pixel.

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

Every spot on a map has a height. The leader can step up onto ground at most
8 pixels higher, or down onto ground at most 12 pixels lower; anything
steeper blocks the way as surely as a wall. Most of the game's walls are made
this way, by height, rather than by marking a spot as a wall: Goma Cave has no
wall marks at all, only ridges of ground 32 pixels high at the foot of each
cliff. [Layouts](../layout/README.md#cells) draws the rule from the side.

A map is a grid of 16-pixel squares, its [cells](../../glossary.md#cell), and each cell has a shape
that gives the height of every point in it. A shape is four bytes: a terrain
kind (flat, a slope, a step and so on) in the low four bits of the first,
and three height samples in steps of 8 pixels. Code that asks about the
ground at a point reads the cell there and looks up its shape; the kind picks
one of `TERRAIN_KIND_COUNT` (16) functions in
`gMapGrid_TerrainHeightFunctions`, which works the height out from the three
samples (`MapGrid_GetTerrainHeight`).

A town or dungeon has three layers ([below](#towns-and-dungeons-on-screen)),
each with ground of its own. An object's `layer` names the layer whose
ground it follows, so walls and heights can differ from one layer to the
next.

`MapGrid_CheckCollision` (`field/terrain.c`) decides each test: `COLLISION_IMPASSABLE`
for a cell marked `MB_WALL`, `COLLISION_TOO_HIGH` for ground more than 8
pixels above the object's, `COLLISION_TOO_LOW` for ground more than 12 pixels
below, and `COLLISION_NONE` otherwise. On the world map,
`WorldMap_IsWalkable` asks `WorldMap_GetCollision` instead, which reads a
4-bit collision value from the world map's upper collision layer where it
has one, else the lower.

The height functions are `gMapGrid_TerrainHeightFunctions` (`curve_data.c`); the
cells and their shapes are the layout's, as [Layouts](../layout/README.md#cells)
describes.

| Name | Value | Meaning |
| --- | --- | --- |
| `TERRAIN_KIND_COUNT` | 16 | ground height functions a shape can pick |
| `COLLISION_TOO_LOW` | -1 | the ground there is more than 12 pixels lower |
| `COLLISION_NONE` | 0 | the object can step there |
| `COLLISION_TOO_HIGH` | 1 | the ground there is more than 8 pixels higher |
| `COLLISION_IMPASSABLE` | 2 | the cell there is a wall (`MB_WALL`) |

## Towns and dungeons on screen

A town or dungeon is drawn as three flat layers stacked on top of each other,
with the characters between them.

![Vale pulled apart into its layers. A screen of Vale at the Sanctum's door comes apart into the sheets the console stacks to draw it, then closes again. From the back: the ground layer on `BG3`, with the grass, paths, water and steps; the middle layer on `BG2`, with the Sanctum's walls and door and the cliff edges; Isaac, a sprite; and the overhead layer on `BG1`, with the Sanctum's roof and the tree top. Isaac is drawn in front of the middle layer and behind the overhead one.](https://alchemy.pascalpixel.com/illustrations/field/layer-stack.svg)

The ground is at the back, then a middle layer, and in front an overhead
layer of roofs and treetops. On most maps the characters are drawn between
the middle and overhead layers, which is how Isaac can walk behind a roof.

`MapGrid_LoadLayout` loads a layout from its row of `gLayouts`;
[Layouts](../layout/README.md) describes the data and where each part is decoded.
The console has four [background layers](../../glossary.md#background-layer), and the three map layers are drawn on
three of them: `MAPGRID_LAYER_GROUND` on `BG3`, `MAPGRID_LAYER_MIDDLE` on `BG2` and
`MAPGRID_LAYER_OVERHEAD` on `BG1`. `BG0` is left for the windows, where messages
and menus are drawn.

Which layer is in front comes from the console's priorities, not from code.
Each layer's priority comes from the layout's header, and a lower number is
drawn nearer the front. The field draws sprites at priority 2, and a sprite
wins a tie with a background. Of the 182 layouts, 101 give the layers 3, 2
and 1, so the ground is at the back, the middle layer under the object events
and the overhead layer over them, as with the Sanctum's roof and the tree top
in front of Isaac above. Another 49 give 3, 1 and 2, which puts the middle
layer over the object events and the overhead layer under them, 15 give 3, 1
and 1, and the rest use other orders. A priority of 0 leaves the layer off;
nine layouts show only their ground.

Each layer scrolls by its own parallax and drift from the header, so a layer
can move at its own rate as the view moves or drift while it stands still.
`MapGrid_UpdateLayerScroll` redraws the rows and columns of the BG maps that come
into view from the cells and their [metatiles](../../glossary.md#metatile).

Three timed effects run as tasks while a map is shown
(`field/tile_animation.c`): the tile animation copies tiles into video memory
on up to 16 channels, the blend script writes the blend registers over time,
and palette cycles rotate colours. The scripts for the first two come from the
layout; [Layouts](../layout/README.md#tile-animation-and-blend-scripts) gives their
format. Only `PaletteCycle_Start` schedules palette cycles, and nothing calls
it, so no palette cycle runs in the game.

The layouts are the rows of `gLayouts` (`layouts.tsv`).

| Name | Value | Background | Meaning |
| --- | --- | --- | --- |
| `MAPGRID_LAYER_GROUND` | 0 | `BG3` | the ground, at the back |
| `MAPGRID_LAYER_MIDDLE` | 1 | `BG2` | walls and edges, under the object events |
| `MAPGRID_LAYER_OVERHEAD` | 2 | `BG1` | roofs and tree tops, over the object events |

## The world map in perspective

The world map is a flat map, like a town's, that the console tilts so that it
seems to stretch away towards the horizon.

![Above, the world map's screen as Isaac leaves Vale, tilted into perspective, with five of its lines picked out in colour from the top of the screen to the bottom. Below, the same part of the world laid flat, with Vale and Vault marked. A trapezoid around Isaac shows the part of the flat map the screen reads: each coloured screen line reads the straight strip of the same colour. The lines near the top of the screen read strips that are farther away and wider, so the area read is a trapezoid, wide at the far end.](https://alchemy.pascalpixel.com/illustrations/field/world-map-footprint.svg)

It tilts the map one screen line at a time: each of the screen's 160 lines
shows one straight strip of the flat map, and lines near the top show strips
that are farther away and wider, squeezed to fit, so distant ground looks
smaller. Together the strips cover a trapezoid of the flat map around the
party.

The console can scale and rotate two of its background layers as it draws
them, `BG2` and `BG3`, and it reads their scale and position afresh for every
line if the game changes them between lines. The field gives every line its
own scale: as the party leaves Vale it falls steadily from 0.61 on the top
line to 0.44 on the bottom one, so a line near the top covers more of the map.

![The world map pulled apart into its layers. The world map as Isaac leaves Vale comes apart into the sheets the console stacks to draw it, then closes again. From the back: the lower ground on `BG3`, with the grass and paths; the upper ground on `BG2`, with the mountains, woods and cliffs; and the sprites, Isaac and Vale. Both ground layers are tilted line by line and share a priority, so `BG2` is drawn in front of `BG3`, and the sprites in front of both. Where the upper ground covers it, the lower ground is left unfinished.](https://alchemy.pascalpixel.com/illustrations/field/world-map-layers.svg)

`WorldMap_BuildScanlineTable` gives each screen line a width scale and a
ground distance, or zeros for a line above the horizon. It runs when the world
map is set up and again only if the camera's pitch changes, which nothing in
the game does, and at that one pitch every line meets the ground. Each frame
`Transform_UpdateVertices` turns them into a page of `BG2`'s and `BG3`'s
affine values (scale, rotation and position) for every line. It is one of
the routines in the processor's 32-bit ARM instructions that run from RAM
([below](#routines-run-from-ram)). `WorldMap_ApplyAffineFrame` sets the first
line's values and starts a DMA, a copy the console makes by itself, that runs
in each HBlank, the pause after a line is drawn, to feed the next line its
own. There are two pages, and alternate frames use them. When the player
holds L to zoom out, the lines spread so far apart that some rows of the
ground are never drawn.

The ground is unpacked a square at a time around the party
(`WorldMap_UpdateCurrentTileBlock`), and `WorldMap_PreloadCurrentTileBlock` loads at
most one missing square a frame as the party walks. `WorldMap_LoadGraphics`
chooses between the two graphics sets of `gWorldMap_GraphicsResources` by the
32-pixel region under the point it is given: the second set where it is
`WORLD_MAP_ENCOUNTER_REGION_SNOWFIELD`.

When a menu or script starts, `WorldMap_StartStillView` draws the current
frame into ordinary 256-colour tiles on `BG1` with `TileMap_DrawRows`, an ARM
routine run from RAM (through `TileMap_DrawRowsFromRam`), and switches the
display to `DISPCNT_MODE_0`, in which no background is scaled.
`WorldMap_LoadAreaGraphics` reloads the area's palette and tiles afterwards and
returns to the moving view.

The world map's graphics sets are `gWorldMap_GraphicsResources`
(`world_map_resources.c`); its own data is described in
[Layouts](../layout/README.md#the-world-maps-data).

| Name | Value | Meaning |
| --- | --- | --- |
| `WORLD_MAP_ENCOUNTER_REGION_SNOWFIELD` | 21 | the region whose ground takes the world map's second graphics set |

## Design

### One object, many views

All 64 objects are the same 112 bytes, but each family of code reads them
through a struct of its own: `Object` (`include/object_runtime.h`),
`DispatchObject` (`include/object_dispatch.h`), `ObjectScriptContext` and
`ScriptOperands` (`include/field/action_script.h`), `ScriptMotionObject`
(`include/script_motion.h`), `ScriptObjectRuntime`
(`include/field/script_object_runtime.h`), `ScriptObjectEntry`
(`include/script_object_entry.h`) and `ObjectEvent` (`include/object_event.h`).
Other folders add more, such as the battle's `MotionObject` and the field
effects' `FieldEffect`. Each view was written from the code that reads it and
names what that code uses; the bytes it leaves alone keep ObjectEvent's
names under a "not named here" note. `ObjectScriptContext` is 100 bytes, since
no command reads the last 12.

The views agree on the layout (the script first, then position, velocity and
target) and differ in names and types where their readers differ. The wait is
unsigned in `Object` and `ObjectEvent` and signed in
`ScriptMotionObject`; the routine run each frame at offset 0x6c is `hook` to
the scripts, `update` to maps and `Object`. Even
`gObjects` is declared as an `Object *` in `include/field/field.h`
and as a `ScriptMotionObject *` in `update.c`.

The several views come from the [matching](../../glossary.md#matching-decompilation):
the code must compile to [the original](../../glossary.md#the-original)'s
instructions, and a function sometimes matches only through a view of its
own, as [Matching](../MATCHING.md#field) lists. [Camelot](../../glossary.md#camelot)'s source likely had one
struct, since every view describes the same 112 bytes.

### Scripts without addresses

A jump stores a label number, not an offset, and `ObjectScript_FindLabel` searches
for it each time. A script is therefore a plain array of words that C
initializers can write, with no assembler to fix up its branches, at the cost
of a linear search on every jump and the 1,024-word reach.

### Routines run from RAM

The work done per scanline or per pixel is ARM code copied into RAM before it
runs: `Transform_UpdateVertices`, `TileMap_DrawRows`, the sprite frame
decoder (`Render_DecodeFrame`) and `Tile_ConvertMapGrid`. The cartridge's bus is
16 bits wide, so 32-bit ARM instructions run slowly from ROM, and the rest of
the game is Thumb. `Object_UpdateAll` is an ARM copy of the object update for
the first eight objects, written for the same treatment, but nothing runs it.
It is a smaller update than the field needs: it covers eight objects of the
64, and has none of the field's extras that `Object_UpdateAllThumb` adds (free
flight, terrain following, floating, and solid objects that refuse to overlap),
so it could not stand in for the Thumb update.

## How the field fits with the rest of the game

[Running a map](../object/README.md) owns the map loop: it starts the
object system, loads the map and the overlay, places the object events and
handles requests; the engine supplies the objects, scripts and ground checks it
uses. Overlays reach the engine through their [import tables](../../glossary.md#entry-table-and-import-table)
(`src/map/import/`): nine import `MapGrid_GetTerrainHeight` and six
`MapGrid_SetMetatileBehaviorAt`, with which a map rewrites a cell's [metatile behavior](../../glossary.md#metatile-behavior).
The battle uses the object system in `OBJECT_SYSTEM_BATTLE` for its own sprites. The
Lost Age's field code is described by [its own
guide](../../../tla/src/field/README.md), as differences from this one.

## Limits

- 64 field objects; 64 sprites, sharing 64 animation layer records, up to
  four each.
- 8 resource slots for a sprite's frames, 16 tile animation channels and 4
  palette cycles.
- `gObjectScript_CommandTable` has 47 entries: 36 commands, then 11 that act
  as `SC_END`. A label must lie within the first 1,024 words of its script.
- A step can climb 8 pixels and drop 12.

## Files

| File | What it holds |
| --- | --- |
| `field.c` | the field object: a guide comment, then `field_internal.h` and the 14 parts under `field/` in ROM order |
| `field_internal.h` | the parts' includes, and the names more than one part uses; it places no bytes |
| `field/*.c` | the 14 parts, each one family of functions; `head -3 field/*.c` prints what each holds |
| `update.c` | `Object_UpdateAll`, an ARM copy of the object update for the first eight objects, which only the unused `Object_RunUpdateAllFromHeap` would run |
| `vertices.c` | `Transform_UpdateVertices`, the ARM routine that turns the world map's scanline table into `BG2`'s affine values |
| `object_script.c`, `operand.c` | the scripts the engine starts by itself, `gObjectScript_CommandTable`, and the handlers of the work values |
| `facing.c` | the facing tables and the small tables the parts read: the order a sprite's tiles dissolve in, the fixed camera's matrix, the wave a floating object bobs on, the idle script, the direction pad's headings, the footprint and dust scripts, and the world map camera's lean for each pad direction |
| `curve_data.c`, `map_grid_data.c` | the ground height tables, `gMapGrid_TerrainHeightFunctions`, the world map's terrain behaviour table, and two tables nothing reads (`gWorldMap_DirectionOffsets`, `gWorldMap_UnitScale`) |
| `world_map_resources.c` | `gWorldMap_GraphicsResources`, the world map's two graphics sets |
| `griddata.c` | the scale the debug sprite viewer draws at |
| `lucky_dice.c` | the Lucky Dice minigame, with its camera defaults and 8.8 fixed-point helpers |
| `layouts.tsv` | `gLayouts`: each layout's header, palette, three tile sheets and layer data, as resource numbers counted from `ResourceId_Layout001` |
| `remap0.tsv` to `remap3.tsv` | `gRuntime_ByteRemapTable`, four 256-byte colour remap tables |
| `maps.s` | `gMapHeaders`: each map's overlay, [map section](../../glossary.md#map-section), map type (town or field) and layout; each row also defines the map's `MapId_` |
| `common/effect/` | the ground particle and sparkle pictures of the field's weather, which [`scripts.s`](../object/scripts.s) places |

The headers are in `games/tbs/include/unit/field/`, one for each C file but
`world_map_resources.c`, whose table `field.h` declares. Link order is in
`games/tbs/objects.tsv`.

`field.c` is one object in the ROM; the parts are our cut of it, made where
one family of functions ends and the next begins. They are compiled only
through `field.c`, and each also includes `field_internal.h` so that it reads
on its own.

## Words used here

- A **field object** is one of the 64 slots of `gObjects`, 112 bytes
  each, as the [glossary](../../glossary.md#field-object) defines it; a map
  knows its object events among them by [local id](../../glossary.md#local-id). A slot with no script is free.
- A **sprite** is one of the 64 `struct Sprite`s: an image and a
  shadow, two entries of OAM (the hardware's sprite list), and up to four
  animation **layers** (`struct SpriteAnim`) composed into one picture.
- An **object script** is a list of 32-bit words: `SC_` commands of
  [`objcmd.h`](../../include/objcmd.h) and their operands. An **animation
  script** is a list of byte pairs that a sprite layer plays.
- A map **layer** is one of a town's three backgrounds
  ([Layouts](../layout/README.md#one-grid-three-layers)); a map's layers
  have nothing to do with a sprite's.
- Positions are 16.16 fixed-point pixels: x across the map, y up, z down the
  map. Angles are 16-bit fractions of a turn (`FACING_EAST` is 0,
  `FACING_SOUTH` a quarter turn).

In the code, `object` and `interpreter` are the object a command runs on,
`sprite` and `layer` a sprite and one of its layers, `work` or `gridWork` the
map work (`gMapGridWork[0]`), and `perspective` the world map's.

Because the parts compile as one file, a part's own types, macros and static
helpers carry its family's name (`ObjectCamera`, `Layout`, `WorldMap`),
and a part undefines its macros at its end.
[Matching](../MATCHING.md) lists the sites that keep an unusual spelling.

## Function prefixes

| Prefix | What it covers |
| --- | --- |
| `Sprite_`, `Render_` | drawing sprites |
| `SpriteDissolve_` | dissolving a sprite into or out of view, pixel by pixel |
| `Animation_`, `AnimationObject_`, `SpriteAnim_`, `Sprite_`, `SpriteFrameCache_` | sprites, their layers and the resource slots they draw from |
| `ObjectSystem_`, `ObjectDispatch_`, `FieldObject_`, `Object_` | keeping and moving objects |
| `ActionScript_` | object script commands |
| `Field_` | the configured keys |
| `MapGrid_`, `TileMap_` | layered maps, their cells and the BG maps built from them |
| `WorldMap_` | the world map |
| `GetWorldMap`, `CheckWorldMap` | the ground under a point on the world map; `MapGrid_GetMetatileBehaviorAt`, `MapGrid_SetMetatileBehaviorAt` and `MapGrid_IsWallAt` ask the same of a layered map |
| `TileAnimation_`, `DisplayBlend_`, `PaletteCycle_` | tile animation, the blend script and palette cycles |
| `Curve_` | ground height functions |
| `Ui_`, `SpriteViewer_` | the debug sprite viewer |
| `LuckyDice_` | the Lucky Dice minigame |
| `ScriptObject_` | the overlap and wandering tests object scripts run |
| `ObjectCamera_` | a camera hook that returns 1 and nothing calls (`object_camera.c`) |

A few functions take the prefix of the one thing they act on
(`Decompress_Lz`, `FieldCamera_FollowLeaderInBounds`). Names other
folders or The Lost Age's copy also use change only in a rename across all of
them.

## Evidence and open questions

The command names (`SC_WAIT`, `SC_SYNC` and the rest) are the macros of
`objcmd.h`, named from what each handler does; the struct views are named
from the fields each family reads, and the layout words from the loader. None is
recovered from the original.

The update accepts command words up to 63 but the table has 47 entries, so a
word from 47 to 63 would call through whatever follows the table. No script
does: every script in the game is written in `objcmd.h`'s macros, whose last
command is `SC_KEY_COUNT` (`0x23`), and no script holds a bare word from
`0x24` to 63, so neither the eleven entries that act as `SC_END` nor the words
past the table are ever called.

The field maps sampled for a sprite with more than one drawn layer (Vale,
Bilibin's item shop and inn, Imil, Goma Cave, Xian, Tolbi and the world map)
have none: the leader on the world map has a second layer,
`SPRITE_BLANK_32`, which draws nothing even in a forest, where the player
control gives it priority 1. The picture of a sprite's layers is therefore of
a battle unit, whose sprite is the same `struct Sprite`
composed by the same `Sprite_ComposeFrame`.


- XXX: what `FieldWork`'s `unk1da`, `unk1dc` and `unk1de` hold;
  `FieldScript_Start` sets them to 0xffff, -1 and -1 at each script and no
  C reads them, so a read in a map overlay's assembly or a watch on them over
  a script would settle it.
