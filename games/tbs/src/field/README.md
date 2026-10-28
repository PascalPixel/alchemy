# The field bank

The field keeps the actors (the [party](../../glossary.md#the-party), the
townspeople, the camera, each footprint), moves them a little every
[frame](../../glossary.md#frame), draws their sprites, in battles too, decides
whether the ground ahead can be walked on, and draws the
[map](../../glossary.md#map), a town as three flat layers or the World Map
tilted into perspective. It decides nothing about any one
[scene](../../glossary.md#scene).

Three other folders hold the rest. This folder is named after the
[bank](../../glossary.md#bank) it fills; the
[field sequence](../../glossary.md#field-sequence) that drives it is in
`field_sequence.c` ([Running a scene](../overworld/README.md)). Each scene's own
code, its [events](../../glossary.md#event) included, is an
[overlay](../../glossary.md#overlay): [Scenes](../scene/README.md). What a town or
dungeon shows and stands on is its map:
[Maps](../map/README.md). [Field](../../field.md) gives the whole division
in brief.

## The walk and what interrupts it

While the player has control, the game watches once a frame for anything
that should stop the walk: the ground the leader steps onto, the buttons, or
a running event asking for another event or a battle. Each raises a
[field request](../../glossary.md#field-request), and the walk stays stopped
while the requests are handled one at a time, so an event is one routine that
plays from start to end while the party waits; whatever must keep moving
meanwhile moves on its own, as an [actor script](../../glossary.md#actor-script)
([below](#everything-that-moves-runs-an-actor-script)).

`FieldSequence_Run` is the field sequence, and `Field_UpdateWalking` checks the
ground under the leader once a frame; [Running a
scene](../overworld/README.md#the-field-sequence-serves-requests) gives the
requests and the order they are handled in.

## Everything that moves runs an actor script

![Isaac walks through Imil's snow; each footprint he leaves is an actor of its own and disappears after a moment](https://alchemy.pascalpixel.com/guides/field/snow.gif)

Everything on the field that moves or changes by itself is an
[actor](../../glossary.md#actor), and each one runs a small script. The
party, the people and things a scene places, the camera and each
footprint share 64 slots. Every frame each actor runs the commands of its
script until one says to wait. The leader's script is "move by the pad, wait
a frame, repeat", the camera's "follow the leader, wait a frame, repeat", and
a footprint's "wait 120 frames, disappear"; the leader leaves one every 18
frames of walking or 12 of running on the maps that show them.

After its script, the field moves each actor that isn't paused: towards
the point its script aims it at, following the ground up and down, floating,
and refusing to let solid actors overlap. In a battle a simpler update
replaces it, which moves the first 14 actors and runs no scripts.

The actors are `gActors`, read as `struct Actor`, and
`Actor_UpdateAllThumb` runs each one's script and motion for the frame;
`Actor_UpdateAllMotion` is the battle's update.

### How an actor script runs

A script is a list of 32-bit words: a command, then the values it needs. The two
children who run round Vault's well each run one of these, the boy's eight
points one after another and the girl's the same points half a lap behind. A
command that only sets, tests or jumps takes no time, and the next one runs in
the same frame; a command like `SC_SYNC` stops the script for this frame and
each frame after until the child reaches the point.

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

A loop jumps back to a numbered label; `ActorScript_FindLabel` searches the
script's first 1,024 words for it. `SC_END` leaves the actor standing idle,
and only `SC_RELEASE` releases its slot. A scene can also give one actor a
function of its own, an update routine, that runs every frame before its script, even
while the actor is paused.

The commands are the `SC_` macros of
[`actor_script.h`](../../include/actor_script.h), each a row of
`gActorScript_CommandTable`.

| Name | Meaning |
| --- | --- |
| `SC_ANIMATION(n)` | play [sprite animation](../../glossary.md#sprite-animation) n |
| `SC_SET(w, v)` | set the actor's value w, such as `SW_SPEED`, to v |
| `SC_WAIT(n)` | stop for this frame, and run again n frames later |
| `SC_SYNC` | wait until the actor reaches its target |
| `SC_MOVE(x, y, z)` | set a target to move towards |
| `SC_LOOP(n, to)` | jump to label `to` n - 1 times, so what lies between runs n times |
| `SC_LABEL(n)`, `SC_TO(n)` | a label, and a jump's target naming it |
| `SC_FOREVER` | an `SC_LOOP` count that never runs out |
| `SC_END` | stand idle for good |
| `SC_RELEASE` | release the actor's slot |

## Sprites and their animation

What the player sees of an actor is its
[sprite](../../glossary.md#sprite-and-oam), built from up to four pictures
laid over each other, each playing its own animation. In a battle Isaac's
body, his weapon and the shadow under his feet are three animations of one
sprite, so as he swings, the body and the weapon change pose together, and
whenever any layer changes picture the sprite's whole picture is drawn again
(`Sprite_ComposePicture`).

An animation is a list of pairs: which picture to show, and for how long.
It names pictures as if the figure always faced one way; a facing table then
adds a number to each picture and says whether to mirror it, for each of the
eight ways a figure can face. The party is seen from behind in a battle, so
the table adds 1 to every picture Isaac's swing names: pictures 12, 14 and 16
of his swing are the sheets' 13, 15 and 17 (`spr_300.png` and `spr_320.png` in
[`graphics/sprite`](../graphics/sprite)); the shadow (`spr_510.png`) holds one
picture throughout. An animation can also be recoloured
as it is drawn: its colours swapped round, turned grey or tinted, or filled
with one flat colour (`enum SpriteColorMode`).

A sprite is `struct Sprite` and each of its animations a
`struct SpriteAnimation`; `Sprite_ComposePicture` draws them into one picture, the
commands of an animation are `enum AnimationCommand`, and `facing.c` holds
the facing tables.

| Name | Value | Meaning |
| --- | --- | --- |
| `ANIM_OBJECT_COUNT` | 64 | sprites, and the sprite animations they share |
| `ANIM_LAYER_COUNT` | 4 | animations one sprite lays over each other |
| `SPRITE_ANIMATION_NORMAL_STEP` | 16 | sixteenths of a frame off a sprite animation's timer each frame: normal speed |

## Actors move first, then the camera draws them

Every frame all the actors move first, and only then does the camera work out
where each appears on the screen and what it is in front of. Because that
order never changes, each sprite is drawn where its actor has just moved to.
While a menu is open over the field (the item, Psynergy, Status,
[Djinn](../../glossary.md#djinn) or debug menu), the sprites that are not held
are hidden.

The actor update and the camera are two [tasks](../../glossary.md#task), routines
the game runs once a frame, and the camera is one of two kinds: on a town or
dungeon map it scrolls with the map, and on the World Map and in a battle it projects each
actor's place in 3D onto the screen.

`ActorSystem_Initialize` adds both tasks, with the actor
update at a higher priority so that it runs first;
`ActorSystem_PlaceSprites` is the scrolling camera and
`ActorSystem_ProjectSprites` the projecting one.

| Mode | Started by | Actor update | Camera |
| --- | --- | --- | --- |
| `ACTOR_SYSTEM_MAP` (2) | the field sequence, the debug sprite viewer | `Actor_UpdateAllThumb` | `ActorSystem_PlaceSprites` |
| `ACTOR_SYSTEM_WORLD_MAP` (3) | the field sequence | `Actor_UpdateAllThumb` | `ActorSystem_ProjectSprites` |
| `ACTOR_SYSTEM_BATTLE` (4) | the battle | `Actor_UpdateAllMotion` | `ActorSystem_ProjectSprites` |

## Walking and running

![Isaac walks right in front of the Sanctum in Vale, then runs back left with B held, half as fast again](https://alchemy.pascalpixel.com/guides/field/walk-run.gif)

The leader walks at one pixel a frame and runs at one and a half while B is
held. Before each frame's move the game tests the ground 8 pixels ahead, and against a
wall it tries headings further and further to either side, up to 67.5 degrees
off, so the leader slides along the wall instead of stopping dead. On the
World Map the party walks at half a pixel a frame, and holding L pulls the view
back while the party stops ([below](#the-world-map-in-perspective)). Climbing, the leader moves 8 pixels at a time instead. A faster walk
of 4 pixels a frame waits behind `FLAG_FAST_WALK`, which nothing in
[The Broken Seal](../../glossary.md#the-two-games) sets.

The Japanese edition and [the international
editions](../../glossary.md#editions) differ on the World Map:

| | Japanese | International |
| --- | --- | --- |
| how far ahead the ground is tested | 8 pixels, straight ahead and 30 degrees either side, then up to 67.5 degrees off | 7 pixels, straight ahead and 22.5 and 45 degrees either side, then up to 90 degrees off |
| R and L on the World Map | act again and again while held | act once a press |

`Actor_UpdatePlayerControl` steers the leader on a town or
dungeon map and `Actor_MoveOnWorldMap` on the World Map; the edition's
values are chosen in a block of `#if`s at the top of `field/player_control.c`.
Speeds are in pixels a frame with a fraction: `0x10000` is one pixel.

| Name | Value | Meaning |
| --- | --- | --- |
| `FIELD_WALK_SPEED` | `0x10000` | 1 pixel a frame |
| `FIELD_RUN_SPEED` | `0x18000` | 1.5 pixels a frame, with B held |
| `FIELD_ACCELERATION` | `0x4000` | a quarter pixel a frame faster each frame |
| `FIELD_FAST_WALK_SPEED` | `0x40000` | 4 pixels a frame, B held with `FLAG_FAST_WALK` |
| `WORLD_MAP_WALK_SPEED` | `0x8000` | half a pixel a frame |
| `WORLD_MAP_RUN_SPEED` | `0x10000` | 1 pixel a frame, with B held |
| `WORLD_MAP_FAST_SPEED` | `0x40000` | 4 pixels a frame, the limit `Actor_MoveOnWorldMap` sets while L repeats |

## How high a step can go

Every spot on a map has a height. The leader can step up onto ground at most
8 pixels higher, or down onto ground at most 12 pixels lower, and anything
steeper blocks the way as surely as a wall. Most of the game's walls are made
this way rather than marked. Goma Cave has no wall cells at all: its walls are
rims of ground 32 pixels high, too steep to step onto.

The height anywhere in a [cell](../../glossary.md#cell) is worked out from its
shape and three heights ([Maps](../map/README.md#cells)). Each actor follows
the ground of one of the map's three layers, so walls and heights can differ
from one layer to the next. The World Map has no heights: each of its cells says only whether it
can be walked on.

`MapGrid_GetTerrainHeight` works out a height through one of the
16 functions of `gMapGrid_TerrainHeightFunctions`, `MapGrid_CheckCollision`
decides each test, and `WorldMap_IsWalkable` the World Map's.

| Name | Value | Meaning |
| --- | --- | --- |
| `COLLISION_TOO_LOW` | -1 | the ground there is more than 12 pixels lower |
| `COLLISION_NONE` | 0 | the actor can step there |
| `COLLISION_TOO_HIGH` | 1 | the ground there is more than 8 pixels higher |
| `COLLISION_IMPASSABLE` | 2 | the cell there is a wall (`CELL_BEHAVIOR_WALL`) |

## Towns and dungeons on screen

![Vale in front of the Sanctum taken apart into the ground, the middle layer, the sprites and the overhead layer, spread one behind another, then put back together into the picture the player sees](https://alchemy.pascalpixel.com/guides/field/layers-3d.gif)

A town or dungeon is drawn as three flat layers stacked on top of each other,
the ground at the back, a middle layer, and in front an overhead layer of roofs
and treetops. On most maps the characters are drawn between the middle and
overhead layers, which is how Isaac can walk behind a roof. Which layer is in
front comes from the console's priorities: each layer's priority comes from
the map, and a character takes 2 on open ground or 3 where its cell puts it
behind a building (`ActorSystem_PlaceSprites`); a character wins a tie. Of the
182 maps, counting each one's first layout, 101 order their layers ground,
middle, overhead from back to front, 49 put the overhead layer behind the
middle one, 16 give both the same priority, and the rest other orders; a
priority of 0 leaves a layer off, and nine maps show only their ground.

Each layer can follow the camera at its own pace and drift by itself, and as
the view moves the game draws the row or column of the map that comes into
view. Two timed changes run on their own: tileset animations, which swap
the pictures of a few tiles (a waterfall, a flickering torch), and a blend
script that changes how the layers mix; code for up to four palette cycles is
there too, but nothing calls it.

`MapGrid_LoadMap` loads a map from its row of `gMaps`,
`MapGrid_UpdateLayerScroll` scrolls the layers, and the timed changes are in
`field/tile_animation.c`. [Maps](../map/README.md#tileset-animation-and-blend-scripts)
gives the blend scripts' format.

| Name | Value | Background | Meaning |
| --- | --- | --- | --- |
| `MAPGRID_LAYER_GROUND` | 0 | `BG3` | the ground, at the back |
| `MAPGRID_LAYER_MIDDLE` | 1 | `BG2` | walls and edges, under the actors |
| `MAPGRID_LAYER_OVERHEAD` | 2 | `BG1` | roofs and tree tops, over the actors |

## The World Map in perspective

![The World Map just south of Vale taken apart into its two tilted backgrounds and the sprites, spread one behind another, then put back together](https://alchemy.pascalpixel.com/guides/field/world-map-layers-3d.gif)

The World Map is a flat map, like a town's, that the console tilts so it seems
to stretch away towards the horizon. Each of the screen's 160 lines shows one
straight strip of the flat map, and lines near the top show strips farther
away and wider, squeezed to fit, so distant ground looks smaller. While L is
held the view pulls back to 1.6 times as far, and the lines spread so far
apart that some rows of the ground are never drawn
(`FieldEffect_RunVisibilityTransition`).

The console can scale and rotate two of its background layers, and reads
their scale afresh for every line if the game changes it between lines. The
lower ground is on one and the mountains, woods and cliffs on the other, in
front, with the sprites in front of both. Each frame the game works out every
line's values and has them fed to the console one line at a time, in the
pause after each line is drawn. The ground is far too big to keep unpacked,
so the game unpacks only the squares around the party, and the ground of the
snowfield region takes graphics of its own. When a menu opens or an event starts, the game
draws the current view into an ordinary still picture, and the moving view
comes back when it closes.

`WorldMap_BuildScanlineTable` gives each line its scale and
distance, `Transform_UpdateVertices` turns them into the console's values
each frame, and `WorldMap_ApplyAffinePage` feeds them line by line;
`WorldMap_StartStillPicture` makes the still picture and
`WorldMap_LoadAreaGraphics` brings the moving view back. The World Map's data
is in [Maps](../map/README.md#the-world-maps-data).

## Design

### One actor, a few views

All 64 actors are the same 112-byte `struct Actor`, which the script
commands, the motion loop, the cameras, the battle and the
[field effects](../../glossary.md#field-effect) (`struct FieldEffect`, fields in
the same places) all take. Three field-for-field copies remain only for
matching ([Matching](../MATCHING.md#views-of-one-struct)).

### Actor scripts without addresses

A jump names a label rather than a place in the list, so a script is a plain
C array of words with nothing to fix up when it is built, at the cost of a
search on every jump.

### Routines run from RAM

The work done for every screen line or every pixel is written in ARM, the
processor's 32-bit instructions ([Source](../README.md#c-and-assembly)), and
copied onto the [heap](../../glossary.md#heap-and-pools) before it runs, into
the console's fast internal RAM while there is room: the World Map's line
values (`Transform_UpdateVertices`), drawing rows of the map, decoding sprite
pictures and putting a map's cells together. The cartridge's bus is 16 bits
wide, so ARM runs slowly from it, and the rest of the game is Thumb, the
16-bit ones. There is also an ARM copy of the actor update,
`Actor_UpdateAll`, for the first eight actors, but only the unused
`Actor_RunUpdateAllFromHeap` would copy and run it: it lacks the field's
extras (free flight in 3D, following the ground, floating, solid actors), so it
could not stand in for the usual update.

## How the field fits with the rest of the game

[Running a scene](../overworld/README.md) owns the field sequence
(`FieldSequence_Run`): it sets up the actors (`ActorSystem_Initialize`), loads
the map and the overlay, places the actors and handles field requests, and the
field supplies the actors, actor scripts and ground checks it uses. Overlays
reach the field through their
[import tables](../../glossary.md#entry-table-and-import-table): nine overlays
ask for the ground's height, and six rewrite a cell's
[cell behavior](../../glossary.md#cell-behavior). The battle uses the
actors in its own mode for its sprites. The Lost Age's field code is
described by [its own guide](../../../tla/src/field/README.md), as
differences from this one.

## Limits

- 64 actors; 64 sprites, sharing 64 sprite animations, up to four each.
- 8 sprite picture caches and 16 tileset animations.
- `gActorScript_CommandTable` has 47 entries: 36 commands, then 11 that act
  as `SC_END`. A label must lie within the first 1,024 words of its script.
- A step can climb 8 pixels and drop 12.

## Words used here

- An **actor** is one of the 64 slots of `gActors`, 112 bytes each, as the
  [glossary](../../glossary.md#actor) defines it; a scene knows its own
  among them by [actor id](../../glossary.md#actor-id). A slot with no
  script has been released.
- A **sprite** is one of the 64 `struct Sprite`s: an image and a shadow, two
  entries of OAM (the hardware's sprite list), and up to four **sprite animations**
  (`struct SpriteAnimation`), animations composed into one picture.
- An **actor script** is a list of 32-bit words: `SC_` commands of
  [`actor_script.h`](../../include/actor_script.h) and their operands. An **animation
  script** is a list of byte pairs that a sprite animation plays.
- A map **layer** is one of a town's three backgrounds
  ([Maps](../map/README.md#one-grid-three-layers)).
- Positions are 16.16 fixed-point pixels: x across the map, y up, z down the
  map. Angles are 16-bit fractions of a full circle (`FACING_EAST` is 0,
  `FACING_SOUTH` a quarter of one).

In the code, `actor`, `object` and `interpreter` are the actor a command runs on,
`sprite` and `layer` a sprite and one of its animations, `work` the map work
(`gMapGridWork[0]`), and `perspective` the World Map's. `field.c` is
one object written as parts under `field/`
([Source](../README.md#objects-and-parts) says how they are compiled), so a
part's own types and macros carry its family's name (`ActorCamera`,
`MapGrid`, `WorldMap`). [Matching](../MATCHING.md) explains why some sites
keep an unusual spelling.

## Evidence

The command names (`SC_WAIT`, `SC_SYNC` and the rest) are the macros of
`actor_script.h`, named from what each handler does; the struct views are named from
the fields each family reads, and the map words from the loader. None is
recovered from the original.

The update accepts command words up to 63, but the table has 47 rows, the
last 11 acting as `SC_END`, so a word from 47 to 63 would call through
whatever follows it. No script holds one: the last command macro is
`SC_KEY_COUNT` (35).

On the field each sprite draws one sprite animation: the World Map leader's
second, `SPRITE_BLANK_32`, draws nothing (Vale, Bilibin, Imil, Goma Cave,
Xian, Tolbi). A battle unit lays its body, weapon and shadow in one
`struct Sprite`.
