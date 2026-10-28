# Field code

The [Field](../../glossary.md#field), the division of Camelot's credits that
runs everywhere the [party](../../glossary.md#the-party) walks, keeps the
actors (the party, the townspeople, the camera, each footprint), moves them a
little every [frame](../../glossary.md#frame), draws their sprites, in battles
too, decides whether the ground ahead can be walked on, and draws the
[map](../../glossary.md#map), a town as three flat layers or the World Map
tilted into perspective. Above that, the
[field sequence](../../glossary.md#field-sequence) runs one
[scene](../../glossary.md#scene) from the moment it loads until the party
leaves: it sets the scene up, hands the leader to the player, and deals with
whatever stops the walk, such as a doorway, a random battle, the A button or
the menu, before handing the next destination back to the
[main routine](../../glossary.md#main-routine)
([System code](../system/README.md#the-main-routine)).

The folder holds the Field's code in four [banks](../../glossary.md#bank):

- Bank 01: `field.c`, one object written as parts under `field/` (the actors
  and their scripts, sprites, walking, the map grid, the cameras and the World
  Map), with the data it reads and the ARM routines it copies into RAM
  (`actor_update_arm.c`, `world_map_lines.c`, `picture_decode.s`,
  `map_grid_convert.s`).
- Bank 04: `field_sequence.c`, written as parts under `field_sequence/`
  (random encounters, the scene's actors, walking and the requests it raises,
  `FieldSequence_Run` in `run.c`, the triggers, items and Psynergy on the
  field, treasure spots, the screen transitions and the palette fade), and
  `event_commands.c`, written as parts under `event_commands/` (the
  [event commands](../../glossary.md#event-command), field Psynergy, the
  weather and line scrolls, and the world picture). Beside them are the bank's
  tables: the encounter zones and rules, the area names and music rules
  (`scene_rules.s`), the battle music, the field Psynergy tables
  (`field_sequence_tables.c`), the actor scripts they play (`scripts.s`) and
  the scene table, `scenes.s`.
- Bank 11: `lucky_dice.c`, the dice sequence, Lucky Dice in Tolbi's game room.
- Bank 12: `lucky_wheels.c`, the slots sequence, Lucky Wheels in the same room.

Each scene's own code, its [events](../../glossary.md#event) included, is an
[overlay](../../glossary.md#overlay): [Scenes](../scene/README.md). What a town
or dungeon shows and stands on is its map: [Maps](../map/README.md).
[Field](../../field.md) gives the whole division in brief.

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
[`actor_script.h`](../../include/field/actor_script.h), each a row of
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

## The field sequence serves requests

While the player has control, the game watches once a frame for anything
that should stop the walk: the ground the leader steps onto, the buttons, or
a running event asking for another event or a battle. Each raises a
[field request](../../glossary.md#field-request), and the walk stays stopped
while the requests are handled one at a time, so an event is one routine that
plays from start to end while the party waits; whatever must keep moving
meanwhile moves on its own, as an actor script
([above](#everything-that-moves-runs-an-actor-script)). The field sequence is
one loop, `FieldSequence_Run`, that goes round once a frame while the scene
runs.

Before the loop starts, the scene is set up: its
[map](../../glossary.md#map) is loaded, its random battles chosen
([below](#random-battles)), the party placed at its
[entrance](../../glossary.md#entrance), its people placed, and its own setup
run, which may send the party on at once. Then the scene fades in, and the
area's name is shown if the party has come into a new area.

The walk stops only when something asks it to, checked once a
[frame](../../glossary.md#frame), and the loop then deals with one thing at a
time until nothing is left. Each thing that can stop the walk has its own
field in the field work, a request; stepping onto a doorway, pressing A or an
event asking for a battle only sets it. The loop works down the requests in a
fixed order, deals with the first one set, and starts again from the top. A
request an event sets waits until the event has finished, so a battle an
event asks for starts after its last line.

`FieldSequence_Run` runs one scene; its state, requests included, is
`struct FieldWork`, reached through `gFieldWork`. As it starts it writes the
return point and empties the after-battle and defeat destinations. Before a
battle, `Actor_Snapshot` remembers where everyone stood in `gActorSnapshot`,
outside the heap; when the scene is loaded again with
`FLAG_KEEP_PARTY_POSITION` set, `Actor_Restore` puts them back. The requests,
in the order the loop takes them:

| Order | Field | What it asks for |
| --- | --- | --- |
| 1 | `exit` | leave the scene |
| 2 | `raisedTrigger` | run a trigger the scene's code raised, or one of the codes below |
| 3 | `encounter` | a random or requested battle |
| 4 | `stepTrigger` | a [step trigger](../../glossary.md#step-trigger) or exit on the [cell](../../glossary.md#cell) stepped onto |
| 5 | `pushCell` | a ledge, climbing spot or jump the leader pushes against |
| 6 | `aButton` | the A button: talking if someone is ahead, else searching, else the menu |
| 7 | `fieldMenu` | the field menu |
| 8 | `talk` | talk to someone |
| 9 | `examineCell` | search a cell |
| 10 | `psynergy` | use Psynergy ([below](#how-field-psynergy-reaches-its-target)) |
| 11 | `item` | use an item |
| 12 | `pauseMenu` | the Pause menu |

| Name | Value | Meaning |
| --- | --- | --- |
| `FIELD_RAISED_TRIGGER_PARTY_FELL` | -1 | characters fell to poison or hazardous ground; if no one is left standing, the party goes to its restart point, with the leader at 1 HP |
| `FIELD_RAISED_TRIGGER_WORLD_PICTURE` | -888 | R on the World Map: the picture of the world |
| `FIELD_RAISED_TRIGGER_WORLD_MAP_ZOOM` | -889 | L on the World Map: the view pulls back |
| `FIELD_RAISED_TRIGGER_MENUS` | 250 | the field menu or Pause menu, when `FLAG_MENUS_RAISE_TRIGGER` hands them to the scene |

| Name | Value | Meaning |
| --- | --- | --- |
| `FLAG_KEEP_PARTY_POSITION` | `0x109` | the next scene starts where the party was, everyone put back |
| `ACTOR_SNAPSHOT_CAPACITY` | 32 | people a battle's snapshot keeps |

## Random battles

![Isaac walks up and down a narrow passage of Goma Cave until a random battle starts: the screen flashes and goes black, and the battle opens on Isaac, Garet and Ivan facing a Ghost, with "Ghost appeared!"](https://alchemy.pascalpixel.com/guides/object/random-battle.gif)

As the party walks through a dungeon or across the World Map, a counter fills
as the leader moves, and a battle starts when it is full. The counter grows
with the leader's speed, so it measures distance walked rather than time, and
a random bias, drawn again after each battle, makes the gaps vary. Each
kind of ground belongs to a zone, which says how far apart its battles are,
how strong its enemies are, and which groups of enemies can appear and how
often. When the party's average level is above the zone's, each level of the
difference, up to five, spaces its battles further apart, and with Avoid such
a zone has none at all. With no difference and no bias, a zone of rate r
brings a battle after about 16 × r pixels walked, 16 × r frames at
`FIELD_WALK_SPEED`. There are none at all
while Cloak is up, in a zone with no rate, or where the scene or its area
turns them off.

One more rule holds only in the Sol Sanctum: Jenna is in the party only from
Vale to the sanctum's eruption, joining at level 1, and random battles stop
while she has more than 130 experience points, just short of level 4, so the
party cannot train there before the journey begins.

A town or dungeon scene has three kinds of ground: ordinary ground and two
kinds of marked cells. The scene's rule picks a first zone. The first kind of
marked cell takes it and the second kind takes the next zone, while ordinary
ground has no battles, unless the rule moves the first zone onto ordinary
ground instead. A scene no rule matches has no battles. The World Map takes its zone from the
region and the kind of ground under the party. Some zones are never rolled on
at all: event battles, mimic chests and Djinn fights name them directly.

`Encounter_CountStepsAndSelectFormation` fills the counter and draws the
enemies, `Encounter_SelectMapZones` picks a scene's zones from
`gScene_EncounterRules`, the World Map's come from
`gWorldMap_EncounterRules`, and each zone is a row of `gEncounterZones`
(`struct EncounterZone`).

| Name | Value | Meaning |
| --- | --- | --- |
| `ENCOUNTER_ZONE_NONE` | 0 | no battles |
| `ENCOUNTER_ZONE_MIMIC` | 99 | mimic chests: the chest's slot picks the formation (`Encounter_GetFormationAtSlot`) |
| `ENCOUNTER_ZONE_VENUS_DJINN` to `ENCOUNTER_ZONE_JUPITER_DJINN` | 100 to 103 | Djinni battles |
| `FLAG_NO_ENCOUNTERS` | `0x160` | no random battles |
| `FLAG_NO_ENCOUNTERS_IN_AREA` | `0x161` | no random battles until the next ordinary entry to a scene |
| `FLAG_LURE_CAP` | `0x167` | the counter fills twice as fast |

## How field Psynergy reaches its target

Move shows a giant hand that pushes what is ahead: on a stone pillar the
pillar slides; where the scene has no event for it, nothing moves. What Psynergy does to a scene is decided by
that scene's own events, and the cast only draws it. A scene can have an event
for a Psynergy at three moments: before the cast plays, after it, and while it
acts. The cast aims at what is ahead only when the scene has an event for
this Psynergy there. In Vale, the event after Move notes which column the
pillar now stands in and makes its cell solid.

A cast runs in this order. No field Psynergy works in the Battle Arena, Reveal
does nothing on the World Map, and Retreat refuses where the scene forbids it.
Otherwise the PP is paid, the scene's event for before the cast runs (it can
still stop the cast or turn it away from its target), the cast plays, and the
scene's event for after it runs. An event for after the cast can play the
cast itself, as two of Altmiller Cave's Mind Read events and one at Suhalla
Gate do; then the usual cast does not play. The
kinds that act on a target, Force, Frost and Reveal, run the scene's event for
that moment themselves as they act. Halt holds one person still at a time, so
casting it again lets go of the one held before.

`Field_UsePsynergy` runs a cast, `FieldPsynergy_Run` plays it, and each
Psynergy's cast is the part of `event_commands/` named for it, whose comment
names the abilities that reach it. A trigger's moment is
`enum PsynergyPhase`, and what an ability does on the field is
`enum FieldPsynergy`, the `fieldPsynergy` field of each ability in
[`ability_table.c`](../game/ability_table.c).

| Name | Value | Meaning |
| --- | --- | --- |
| `PSYNERGY_PHASE_AFTER_CAST` | 0 | after the cast |
| `PSYNERGY_PHASE_BEFORE_CAST` | 1 | once the PP is paid, before the cast |
| `PSYNERGY_PHASE_AFTER_END` | 2 | after a Psynergy that runs out by itself has ended, as Cloak fades or Reveal runs out |
| `PSYNERGY_PHASE_BEFORE_END` | 3 | as `PSYNERGY_PHASE_AFTER_END`, before it |
| `PSYNERGY_PHASE_FADES` | 4 | as Reveal's colours come back |
| `PSYNERGY_PHASE_ACTS` | 5 | as Force, Frost and Reveal act |
| `PSYNERGY_PHASE_ANY` | 7 | every moment |
| `FLAG_RETREAT_BLOCKED` | `0x144` | Retreat refuses |
| `FLAG_IN_BATTLE_ARENA` | `0x17e` | all field Psynergy refuses |

## The palette fade

When Reveal is cast the backgrounds turn grey in one frame, and most field
Psynergy tints the screen over 8 frames as it starts and fades back after it.
Both are the [palette fade](../../glossary.md#palette-fade): a copy of the
colours on screen that moves toward a set of target colours by an equal amount
each frame, so that every colour arrives together.

`PaletteFade_Initialize` sets it up as each scene loads, copying palettes 0 to
13 of the backgrounds and of the sprites (so the window colours in palette 15
never fade) into `gPaletteFadeWork`, and adds `PaletteFade_Update` as a task
of the frame pass. Each frame that task moves every colour channel, and on the
last frame it copies the target itself, so that rounding never leaves a colour
short. `FLAG_PALETTE_BLEND_HELD` holds it still while a menu is open. A cast's
colour is one of eight in `gFieldEffect_TargetColors`
(`FieldPsynergy_FadeToCastColor`), picked by the frame counter unless one of
the `FLAG_PSYNERGY_TINT(n)` flags forces one. Of the screen transitions the
palette fade serves only `TRANSITION_BACKDROP_FADE`; the fade from black is
`ScreenFade_FromBlack` in [System code](../system/README.md).

## The world picture

![On the World Map, R brings up the picture of the whole world, a hand on the party's place beside a "Present location" window; the hand moves right and up across the map, sits back on the party's place while L is held, and A closes the picture](https://alchemy.pascalpixel.com/guides/object/world-map-picture.gif)

On the World Map, R shows a picture of the whole world with a marker on every
place the party has visited, and L pulls the moving view back. A hand moves
with the d-pad, faster the longer it is held, and returns to the party's place
while L or R is held; the place nearest it blinks and its name shows in a
window. A or B closes the screen. The screen borrows the memory the map's
cells normally use, so it needs none of its own.

Each place's marker is drawn where the place stands on the World Map, so the
picture needs no positions of its own. In the German, French, Spanish and
Italian [editions](../../glossary.md#editions) the hand also glides onto the
nearest place when the d-pad is let go, and a place's name shows from closer
by.

`WorldPicture_Show` draws the picture, the screen's own work is in
[`event_commands/world_picture.c`](event_commands/world_picture.c), and
`gWorldPicture_AreaMarkers` lists each place's visited flag, marker and name.

| Name | Value | Meaning |
| --- | --- | --- |
| `WORLD_PICTURE_CURSOR_SNAPS` | 1 in `TBS_PATCH_2` editions, else 0 | the hand glides onto the nearest place when the d-pad is let go |
| `WORLD_PICTURE_NAME_RANGE` | 64 in `TBS_PATCH_2` editions, else 100 | how near the hand must be, squared, for a name to show |

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

### A battle does not run inside the scene

The heap is emptied between a scene and a battle, so the actors' places live
in `gActorSnapshot`, a fixed EWRAM buffer, and the return point in the main
routine's work.

### Spellings kept for matching

Several structures are reached by more than one spelling (the field work, the
field effect, the map's view, the palette fade), each site keeping the one it
matches with; [Matching](../MATCHING.md#heap-slots) explains why.

## How it fits with the rest of the game

The main routine runs the field sequence (`FieldSequence_Run`) for every
scene: it sets up the actors (`ActorSystem_Initialize`), loads the map and the
overlay, places the actors and handles field requests, with the actors, actor
scripts and ground checks of bank 01. Each scene's overlay supplies its tables
and setup, and calls back into this folder through its
[import table](../../glossary.md#entry-table-and-import-table) for the event
commands its events are written in: nine overlays ask for the ground's
height, and six rewrite a cell's
[cell behavior](../../glossary.md#cell-behavior). Battles and the minigames
return through the main routine's work, and the screen transitions close the
field before each battle as well as between scenes. The battle uses the actors
in its own mode for its sprites. The Lost Age's field code is described by
[its own guide](../../../tla/src/field/README.md), as differences from this
one.

## Limits

- 64 actors; 64 sprites, sharing 64 sprite animations, up to four each.
- 8 sprite picture caches and 16 tileset animations.
- `gActorScript_CommandTable` has 47 entries: 36 commands, then 11 that act
  as `SC_END`. A label must lie within the first 1,024 words of its script.
- A step can climb 8 pixels and drop 12.
- 104 encounter zones of eight formations each.
- 66 actor ids in the field work, each pointing at one of the 64 actors, and
  ten treasure spots; 24 field effect sprites in the field effect work.
- A battle snapshot keeps up to 32 actors.
- One Halt target at a time.
- The palette fade covers palettes 0 to 13 of the backgrounds and of the
  sprites.

## Words used here

- An **actor** is one of the 64 slots of `gActors`, 112 bytes each, as the
  [glossary](../../glossary.md#actor) defines it; a scene knows its own
  among them by [actor id](../../glossary.md#actor-id). A slot with no
  script has been released.
- A **sprite** is one of the 64 `struct Sprite`s: an image and a shadow, two
  entries of OAM (the hardware's sprite list), and up to four **sprite animations**
  (`struct SpriteAnimation`), animations composed into one picture.
- An **actor script** is a list of 32-bit words: `SC_` commands of
  [`actor_script.h`](../../include/field/actor_script.h) and their operands. An **animation
  script** is a list of byte pairs that a sprite animation plays.
- **field work**: `struct FieldWork`, the running scene's state
  ([above](#the-field-sequence-serves-requests)). Local name `work`.
- **request**: a field of the field work that something sets to interrupt the
  walk (above).
- **field kind**: what an ability does on the field, the `fieldPsynergy` field
  of each ability in [`ability_table.c`](../game/ability_table.c), named by
  `enum FieldPsynergy`
  (`module/field/event_commands.h`). Whirlwind and Gale share
  `FIELD_PSYNERGY_WHIRLWIND`.
- **field effect**: `struct FieldEffectWork` (`field/field_psynergy_work.h`), the work of
  the field Psynergy playing: the ability, its field kind (`fieldPsynergy`), its
  caster and target, and 24 field effect sprites. It is
  `HEAP_SLOT_FIELD_EFFECT`, reached as `gFieldEffectWork`; the Djinn joining
  uses it too. Local name `effectWork`.
- **field effect sprite**: `struct FieldParticle`, one particle a field Psynergy
  moves and draws.
- **treasure spot**: a chest or Psynergy Stone the map places at one of
  its cells; the field work holds up to ten, apart from the actors.
- **mode**: `work->mode` is `ACTOR_SYSTEM_MAP` on a town or dungeon scene
  and `ACTOR_SYSTEM_WORLD_MAP` on the World Map.
- A map **layer** is one of a town's three backgrounds
  ([Maps](../map/README.md#one-grid-three-layers)).
- Positions are 16.16 fixed-point pixels: x across the map, y up, z down the
  map. Angles are 16-bit fractions of a full circle (`FACING_EAST` is 0,
  `FACING_SOUTH` a quarter of one).

In the code, `actor`, `object` and `interpreter` are the actor a command runs on,
`sprite` and `layer` a sprite and one of its animations, `work` the map work
(`gMapGridWork[0]`), and `perspective` the World Map's. `field.c`,
`field_sequence.c` and `event_commands.c` are each one object written as
parts in a folder of its name
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

The field kinds are named from the abilities whose `fieldPsynergy` field holds
each kind, and the Psynergy the English release gives them; the requests from what the loop does
with each; the encounter columns from what
`Encounter_CountStepsAndSelectFormation` computes with them; the zone names in
`encounter_zones.c` from the scenes whose rules select them.

`Field_CountRequests` adds the requests together (`talk` twice) and tests the
sum, so a negative raised trigger with a positive request can hide both.
It can happen: `Field_UpdateWalking` raises `FIELD_RAISED_TRIGGER_PARTY_FELL` (-1)
on the frame a party member falls to poison, and the `stepTrigger` request is the cell's
[cell behavior](../../glossary.md#cell-behavior), 1 on the cells of
step trigger 1, which 154 scenes have. Both then wait until another request makes
the sum non-zero, by which time a later cell's behavior may have replaced the
step trigger. It is marked [`// BUG:`](../../glossary.md#bug-bugfix-and-ubfix), and
`BUGFIX` tests each request instead.

`Event_ShowSpeakerMessage`, `Event_ShowMessageAtPosition` and
`Event_ForwardShowMessageAtPosition` are unused: each is reached only
through a bare [veneer](../../glossary.md#veneer) in
`gamepak/far_call/FIELD_SEQUENCE.S`, which has no symbol for code to call
([`event_commands/messages.c`](event_commands/messages.c)).
