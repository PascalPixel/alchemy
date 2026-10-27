# World

Everywhere the [party](glossary.md#the-party) walks, a town, a dungeon or the
world map, is run by the same four parts. The loop in `src/overworld` lets the
party walk and deals with whatever interrupts it. The engine in `src/field`
moves everything that moves and draws the [map](glossary.md#map), and the
map's own data, in `src/map`, says what the ground is like. And each
[scene](glossary.md#scene)'s own code, its [overlay](glossary.md#overlay) in `src/scene`, says
who is there and what happens; only that part knows anything about one scene.
The two engine folders take the names of the [banks](glossary.md#bank) they
fill, not of what they do. How scenes and overlays fit into the whole game is on
[the front page](README.md).

## The field loop and its requests

![Isaac walks north into the door of Jenna's grandparents' house in Vale; the door opens, he steps inside, the screen fades and the room inside appears with him at its door](https://alchemy.pascalpixel.com/guides/world/door.gif)

While the party walks, the game asks once a [frame](glossary.md#frame)
whether something should happen. Stepping into a doorway, pressing A beside
someone or a random battle starting does not act at once: it only raises a
flag of its own, a request. The walk then stops, the game deals with the
requests one at a time in a fixed order, and the walk starts again. Each kind
of request has one flag, so at most one of each can wait. Leaving the scene
comes first, so an event that ends by taking an [exit](glossary.md#exit),
like this door, simply ends the scene.

A random battle is a way out of the scene too: the game remembers where
everyone stood, loads the battle as if it were the next scene, and afterwards
loads the scene again with everyone put back where they were.

`SceneManager_Run` runs one scene and `struct FieldWork` holds the
requests. [Running a
scene](src/overworld/README.md#the-scene-loop-handles-requests-not-scripts) lists
all twelve requests in the order they are handled.

## Scene entries

![Isaac walks up to a young man in Vault, who says "Welcome to Vault, the town at the heart of Angara.", then walks on](https://alchemy.pascalpixel.com/guides/field/talk-walk.gif)

The engine knows nothing about any one scene. It asks the scene's code six
questions: what can happen here, where the party arrives, who stands where,
whether anything needs setting up, whether the leader stands in an exit area,
and where an exit leads. Each answer comes from a function, so a scene can answer
differently as the story moves on without the engine knowing any story: Vault
gives one list of what can happen before its thieves are jailed and another
after.

What can happen is a table of [triggers](glossary.md#trigger). Each row
says what starts it (talking to someone, stepping on a spot, searching,
using Psynergy), whom or where it answers, when it holds, and what it runs.
This is the row for the young man above:

```c
{ TRIGGER_TALK, ACTOR_WELCOMING_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgVaultWelcomeToVaultTheTownAt) },
```

The game runs the first row that fits, so a scene lists its rows from the
latest point in the story down to the default.

The six questions are `struct SceneEntryPoints` at the start of every
overlay, and the rows are `struct Trigger`. [Scenes](src/scene/README.md#the-entry-table)
describes the [entry table](glossary.md#entry-table-and-import-table) and its
tables, and [Engine](engine.md#overlays) how an overlay is loaded. The front
page describes [how scenes depend on the story](README.md#how-scenes-depend-on-the-story),
and [Places](src/field/scene_headers.s) lists every scene in
[story order](glossary.md#the-places-in-story-order).

## Scene and area flags

A scene never undoes what it changed. The game keeps two ranges of
[game flags](glossary.md#game-flag) for scenes to use and clears them as the
party moves on: scene flags whenever the party enters a scene, and
[area](glossary.md#area) flags when it enters another
area. Neither is cleared on a return from battle. A pushed stone pillar
kept in a scene flag is back in place the next time the party comes in; logs
floated in one room of a cave, kept in area flags, stay floated while
the party moves between the cave's rooms. [The front
page](README.md#game-flags) gives the ranges, and
[Scenes](src/scene/README.md#state-that-cleans-itself-up) says how a scene uses
them.

## Objects and their scripts

![Isaac walks through Imil's snow, and each footprint he leaves fades after a moment](https://alchemy.pascalpixel.com/guides/field/snow.gif)

Everything on the field that moves or changes by itself, the party, the
townspeople, the camera, even each footprint in the snow, is an
[object](glossary.md#object) running a small script. Every frame each object
runs the steps of its script until one says to wait. A footprint's whole
script is "wait 120 frames, disappear". The player's control is itself a step
of the leader's script, run once a frame: "move by the pad, wait a frame,
repeat".

The objects are the 64 places of `gObjects`, and the steps are
the `SC_` commands of `objcmd.h`, run by `Object_UpdateAllThumb`.
[Field](src/field/README.md#everything-that-moves-runs-a-script) covers the
commands, the sprites and [the views of one
object](src/field/README.md#one-object-a-few-views).

## Movement

![Isaac holds up and right below the Sanctum in Vale: he slides right along its front wall, then up along the cliff beside it, instead of stopping](https://alchemy.pascalpixel.com/guides/world/wall-slide.gif)

The leader walks at one pixel a frame and runs at one and a half with B held.
Before every step the game tests the ground just ahead. When the way is
blocked it tries headings a little to either side and takes the first that is
open, so the leader slides along a wall instead of stopping dead. Ground more
than 8 pixels higher, or more than 12 lower, blocks the way as surely as a
wall.

`Object_UpdateLeaderControl` steers the leader and
`MapGrid_CheckCollision` judges each point it tests.

| Name | Value | Meaning |
| --- | --- | --- |
| `FIELD_WALK_SPEED` | `0x10000` | 1 pixel a frame |
| `FIELD_RUN_SPEED` | `0x18000` | 1.5 pixels a frame, with B held |
| `COLLISION_TOO_HIGH` | 1 | the ground there is more than 8 pixels higher |
| `COLLISION_TOO_LOW` | -1 | the ground there is more than 12 pixels lower |
| `COLLISION_IMPASSABLE` | 2 | the cell is a wall |

[Field](src/field/README.md#walking-and-running) has the speeds on each kind
of ground and [how high a step can go](src/field/README.md#how-high-a-step-can-go).

## Cells and metatile behaviors

![Isaac faces the stack of wooden boxes by a house in Vale and presses A: "Isaac checked the wooden box..." and "but didn't find anything."](https://alchemy.pascalpixel.com/guides/world/wooden-box.gif)

A map is a grid of 16-pixel squares, [cells](glossary.md#cell), and each
cell says three things: what is drawn there, how high its ground is, and what
standing there does. That last part, its [metatile
behavior](glossary.md#metatile-behavior), is a number that walls and
triggers are keyed on. Most numbers belong to the scene, whose trigger table says
what each one does, so the same number is a door in one town and a chest in
another. A few mean the same in every scene: the jars, barrels and wooden boxes
anyone can search, as in the recording, ledges, the desert's sand and walls.

A loaded cell is `struct MapGridCell`, and the behaviors are
`enum MetatileBehavior`. [Maps](src/map/README.md#cells) gives every
behavior and [how the build makes each
map](src/map/README.md#how-the-build-makes-a-map).

## Map layers

![Isaac walks east along the path above a house in Vale; as he passes behind it the roof is drawn over him, and he comes out the other side and walks back](https://alchemy.pascalpixel.com/guides/world/behind-roof.gif)

A town or dungeon is drawn as flat sheets stacked on top of each other: the
ground at the back, then walls and edges, then the characters, then roofs and
treetops in front. That is how Isaac can walk behind a roof. No code works out
who is hidden: the console draws its
[background layers](glossary.md#background-layer) and sprites in the order of
their priorities, and the characters are simply drawn between two of the
map's layers. Where a character stands can move it between the sheets: the
cells behind a house put whoever stands there behind its walls as well as its
roof.

The three map layers are `BG3`, `BG2` and `BG1` (`BG0` holds
messages and menus), and `ObjectSystem_PlaceSprites` gives each sprite the
priority of the cell under it.

| Name | Value | Background | What it holds |
| --- | --- | --- | --- |
| `MAPGRID_LAYER_GROUND` | 0 | `BG3` | the ground, at the back |
| `MAPGRID_LAYER_MIDDLE` | 1 | `BG2` | walls and edges, most often under the characters |
| `MAPGRID_LAYER_OVERHEAD` | 2 | `BG1` | roofs and tree tops, over the characters |

[Field](src/field/README.md#towns-and-dungeons-on-screen) shows how the layers
are put on screen, and [Maps](src/map/README.md#one-grid-three-layers)
how they share one grid.

## The world map

![Isaac walks and runs on the world map near Vale, the land tilting away towards the top of the screen](https://alchemy.pascalpixel.com/guides/field/world-map-walk.gif)

The world map is a flat map tilted into perspective, so the land near the
party looks close and the land towards the top of the screen far away. The
console can scale two of its background layers, and the game gives every line
of the screen its own scale: each line shows one straight strip of the flat
map, and lines higher up show strips that are farther away and wider. When a
menu or a script opens, the game draws the current view into an ordinary
still picture, so that the windows can use the layer the tilt was using.

`WorldMap_BuildScanlineTable` works out each line's scale and
`WorldMap_StartStillView` makes the still picture.
[Field](src/field/README.md#the-world-map-in-perspective) works through the
lines, [Maps](src/map/README.md#the-world-maps-data) the world map's
data, and [Running a scene](src/overworld/README.md#the-world-map-screen) the
picture of the whole world R brings up. The world map's own places and events
are on [the front page](README.md#the-world-map).

## Random encounters

![Isaac walks back and forth on the world map south of Vale until the screen flashes and a battle against Wild Mushrooms opens](https://alchemy.pascalpixel.com/guides/world/world-map-battle.gif)

As the party walks, a counter fills, faster when running and faster on some
ground than on other, and a battle starts when it is full. Each kind of ground
belongs to a zone, which decides how far apart its battles are and which
enemies can appear. A party that has outgrown a zone meets its battles less
often there, and with Avoid none at all.

`Encounter_CountStepsAndSelectFormation` fills the counter and
picks the enemies, and each zone is a row of `gEncounterZones`. [Running a
scene](src/overworld/README.md#random-battles) has the roll step by step, and
[Battle](battle.md#starting-a-battle) what happens once it starts.

## Field Psynergy

![Isaac casts Move in Vale: the screen tints, a hand appears over the stone pillar and pushes it, and the colours come back](https://alchemy.pascalpixel.com/abilities/move.gif)

Using Move on a stone pillar slides it; using it where nothing answers shows
the same hand and moves nothing. What [Psynergy](glossary.md#psynergy-and-pp)
does to a scene is decided by the scene's own events, and the effect only draws
it. A scene can answer a Psynergy just before its effect plays and just after:
in Vale, the event after Move notes where the pillar now stands and makes the
ground under it solid.

`Field_UsePsynergy` runs a cast and `FieldEffect_Run` plays its
effect. [Running a scene](src/overworld/README.md#how-field-psynergy-reaches-its-effect)
covers every moment a scene can answer and each Psynergy's effect.

## Limits

- 64 objects, and 64 sprites sharing 64 sprite anims, up to four each.
- 66 actor places in the field work, the party's 8 and then 58 for
  the scene's actor templates, though only 64 objects exist to fill them.
- One overlay loaded at a time, in the 32 KB of `gOverlayArea`.
- 256 scene flags and 256 area flags, each range reused by every scene or
  area.
- 104 encounter zones of eight formations each.
- 47 script commands, of which the last 11 act as `SC_END`; a jump finds only
  a label within the first 1,024 words of its script.
- A battle snapshot keeps up to 32 actors.
