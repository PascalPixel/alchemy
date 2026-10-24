# World

Everywhere the [party](glossary.md#the-party) walks, a town, a dungeon or the
world map, is run by the same four parts. The loop in `src/overworld` lets the
party walk and deals with whatever interrupts it. The engine in `src/field`
moves everything that moves and draws the [map](glossary.md#map). The
[layout](glossary.md#layout), in `src/layout`, says what the ground is like.
And each map's own code, its [overlay](glossary.md#overlay) in `src/map`, says
who is there and what happens; only that part knows anything about one map.
The two engine folders take the names of the [banks](glossary.md#bank) they
fill, not of what they do. How maps and overlays fit into the whole game is on
[the front page](README.md).

## The field loop and its requests

![Isaac walks north into the door of Jenna's grandparents' house in Vale; the door opens, he steps inside, the screen fades and the room inside appears with him at its door](https://alchemy.pascalpixel.com/guides/world/door.gif)

While the party walks, the game asks once a [frame](glossary.md#frame)
whether something should happen. Stepping into a doorway, pressing A beside
someone or a random battle starting only raises a flag of its own, a request.
The walk then stops, the game deals with the requests one at a time in a fixed
order, and the walk starts again. Leaving the map comes first, so a cutscene
that ends by taking a [warp](glossary.md#warp), like this door, simply ends
the map.

A random battle is a way out of the map too: the game remembers where
everyone stood, loads the battle as the next map, and afterwards loads the map
again with everyone put back.

`Map_Run` runs one field map: it loads the layout, calls the overlay's
entries, then calls `Field_ProcessStep` once a frame until
`Field_CountRequests` finds a request. Each kind of request is one member of
`struct FieldWork`, so at most one of each can wait. A battle request saves
the [object events](glossary.md#object-event) with `ObjectEvent_Snapshot`
and sets the next map to `RUN_BATTLE`; `Map_ReturnFromRun` then reloads the
map with `FLAG_KEEP_PARTY_POSITION` set, and `ObjectEvent_Restore` puts the
object events back.

[Running a map](src/overworld/README.md#the-map-loop-handles-requests-not-scripts)
lists all twelve requests in the order they are handled.

## Map entries

![Isaac walks up to a young man in Vault, who says "Welcome to Vault, the town at the heart of Angara.", then walks on](https://alchemy.pascalpixel.com/guides/field/talk-walk.gif)

The engine knows nothing about any one map. It asks the map's code six
questions: what can happen here, where the party arrives, who stands where,
whether anything needs setting up, whether the leader stands in a warp area,
and where a warp leads. Each answer comes from a function, so a map can answer
differently as the story moves on without the engine knowing any story.

The six functions are `struct MapEntries` at the start of every overlay
([map_header.h](include/map_header.h)). Vault's `Map_GetEvents` returns
`gVault_EventsAfterThieves` once `FLAG_VAULT_THIEVES_JAILED` is set, and
`gVault_Events` before. A [map event](glossary.md#map-event) row names its
kind, the [trigger](glossary.md#trigger) it answers, a condition and what it
runs; this is the row for the young man above ([vault.c](src/map/vault.c)):

```c
{ MAP_EVENT_TALK, LOCALID_WELCOMING_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgVaultWelcomeToVaultTheTownAt) },
```

`Map_FindEvent` runs the first row whose kind, trigger and condition fit.

[Maps](src/map/README.md#the-entry-table) describes the
[entry table](glossary.md#entry-table-and-import-table) and its tables, and
[Engine](engine.md#overlays) how an overlay is loaded. The front page
describes [how maps depend on the story](README.md#how-maps-depend-on-the-story),
and [Places](src/field/map_headers.s) lists every map in
[story order](glossary.md#the-places-in-story-order).

## Map and map section flags

A map never undoes what it changed. The game keeps two ranges of
[game flags](glossary.md#game-flag) for maps to use and clears them as the
party moves on: map flags whenever the party enters a map,
[map section](glossary.md#map-section) flags when it enters another map
section, and neither on a return from battle. [The front
page](README.md#game-flags) gives the ranges, and
[Maps](src/map/README.md#state-that-cleans-itself-up) says how a map uses
them.

## Objects and their scripts

![Isaac walks through Imil's snow, and each footprint he leaves fades after a moment](https://alchemy.pascalpixel.com/guides/field/snow.gif)

Everything on the field that moves or changes by itself, the party, the
townspeople, the camera, even each footprint in the snow, is an
[object](glossary.md#object) running a small script. Every frame each object
runs the steps of its script until one says to wait. A footprint's whole
script is "wait 120 frames, disappear".

The objects are the 64 slots of `gObjects`. A script is an array of 32-bit
words, `SC_` commands from `objcmd.h` and their operands, run by
`Object_UpdateAllThumb` through `gObjectScript_CommandTable`. The player's
control is itself a command, run once a frame on the leader: `SC_PLAYER` on a
town or dungeon map, `SC_WORLD` on the world map. A footprint is started on
`gWorldMap_FootprintActions`, `SC_WAIT(120), SC_FREE`.

[Field](src/field/README.md#everything-that-moves-runs-a-script) covers the
commands, the sprites and [the views of one
object](src/field/README.md#one-object-a-few-views).

## Movement

![Isaac holds up and right below the Sanctum in Vale: he slides right along its front wall, then up along the cliff beside it, instead of stopping](https://alchemy.pascalpixel.com/guides/world/wall-slide.gif)

The leader walks at one pixel a frame and runs at one and a half with B held.
Before every step the game tests the ground just ahead. When the way is
blocked it tries headings a little to either side and takes the first that is
open, so the leader slides along a wall instead of stopping dead.

`Object_UpdateLeaderControl` tests five points 8 pixels away, the pad's
heading and 22.5 and 45 degrees either side, then headings 22.5, 45 and 67.5
degrees off; `MapGrid_CheckCollision` decides each point from the
[cell](glossary.md#cell) there.

| Name | Value | Meaning |
| --- | --- | --- |
| `FIELD_WALK_SPEED` | `0x10000` | 1 pixel a frame |
| `FIELD_RUN_SPEED` | `0x18000` | 1.5 pixels a frame, with B held |
| `COLLISION_TOO_HIGH` | 1 | the ground there is more than 8 pixels higher |
| `COLLISION_TOO_LOW` | -1 | the ground there is more than 12 pixels lower |
| `COLLISION_IMPASSABLE` | 2 | the cell is a wall (`MB_WALL`) |

[Field](src/field/README.md#walking-and-running) has the speeds on each kind
of map and [how high a step can go](src/field/README.md#how-high-a-step-can-go).

## Cells and metatile behaviors

![Isaac faces the stack of wooden boxes by a house in Vale and presses A: "Isaac checked the wooden box..." and "but didn't find anything."](https://alchemy.pascalpixel.com/guides/world/wooden-box.gif)

A layout is a grid of 16-pixel squares, cells, and each cell says three
things: what is drawn there, how high its ground is, and what standing there
does. That last part, its [metatile behavior](glossary.md#metatile-behavior),
is what walls and map events are keyed on. Most behaviors belong to the map,
whose event table says what each one does, so the same behavior is a door in
one town and a chest in another. A few mean the same on every map: the jars,
barrels and wooden boxes anyone can search, ledges, the desert's sand and
walls.

A loaded cell is one 32-bit word (`struct MapGridCell`): 12 bits pick the
[metatile](glossary.md#metatile), one byte is the behavior and one the ground
shape that `MapGrid_GetTerrainHeight` reads.

| Name | Value | Meaning |
| --- | --- | --- |
| `MB_EVENT_FIRST` to `MB_EVENT_LAST` | 1 to 239 | the map's own events and warps |
| `MB_ENCOUNTER_GROUND_1`, `MB_ENCOUNTER_GROUND_2` | 240, 241 | ground with battles of its own |
| `MB_JAR` to `MB_WELL` | 242 to 247 | search spots with lines every map shares |
| `MB_DESERT_SAND` | 250 | sand that hurts the party as it walks |
| `MB_LEDGE`, `MB_CLIMB`, `MB_JUMP_FORWARD` | 252 to 254 | jumped down, climbed, or jumped across |
| `MB_WALL` | 255 | a wall, whatever the heights |

[Layouts](src/layout/README.md#cells) gives every behavior and [how the build
makes each layout](src/layout/README.md#how-the-build-makes-a-layout).

## Map layers

![Isaac walks east along the path above a house in Vale; as he passes behind it the roof is drawn over him, and he comes out the other side and walks back](https://alchemy.pascalpixel.com/guides/world/behind-roof.gif)

A town or dungeon is drawn as flat sheets stacked on top of each other: the
ground at the back, then walls and edges, then the characters, then roofs and
treetops in front. That is how Isaac can walk behind a roof. No code works out
who is hidden: the console draws its
[background layers](glossary.md#background-layer) and sprites in the order of
their priorities, and the characters are simply drawn between two of the
map's layers.

The three map layers are `BG3`, `BG2` and `BG1`; `BG0` holds messages and
menus. Each layer's priority comes from the layout's header. Object events
placed from a table carry `OBJ_EVENT_PRIORITY_AUTOMATIC`, so each frame their
sprites take the priority the cell under them sets (`ObjectSystem_PlaceSprites`):
the cells behind a house put whoever stands there behind its walls as well as
its roof.

| Name | Value | Background | What it holds |
| --- | --- | --- | --- |
| `MAPGRID_LAYER_GROUND` | 0 | `BG3` | the ground, at the back |
| `MAPGRID_LAYER_MIDDLE` | 1 | `BG2` | walls and edges, most often under the characters |
| `MAPGRID_LAYER_OVERHEAD` | 2 | `BG1` | roofs and tree tops, over the characters |

[Field](src/field/README.md#towns-and-dungeons-on-screen) shows how the layers
are put on screen, and [Layouts](src/layout/README.md#one-grid-three-layers)
how they share one grid.

## The world map

![Isaac walks and runs on the world map near Vale, the land tilting away towards the top of the screen](https://alchemy.pascalpixel.com/guides/field/world-map-walk.gif)

The world map is a flat map tilted into perspective, so the land near the
party looks close and the land towards the top of the screen far away. The
console can scale two of its background layers, and the game gives every line
of the screen its own scale: each line shows one straight strip of the flat
map, and lines higher up show strips that are farther away and wider.

`WorldMap_BuildScanlineTable` gives each screen line a width scale and a
ground distance. Each frame `Transform_UpdateVertices`, ARM code run from
[IWRAM](glossary.md#iwram-and-ewram), turns them into a page of affine values
for `BG2` and `BG3`, and `WorldMap_ApplyAffineFrame` starts an HBlank DMA that
feeds each line its own. When a menu or script opens, `WorldMap_StartStillView`
draws the view into ordinary tiles so the window layer can come back.

[Field](src/field/README.md#the-world-map-in-perspective) works through the
scanline table, [Layouts](src/layout/README.md#the-world-maps-data) the world
map's data, and [Running a map](src/overworld/README.md#the-world-map-screen)
the still picture R brings up. The world map's own places and events are on
[the front page](README.md#the-world-map).

## Random encounters

![Isaac walks back and forth on the world map south of Vale until the screen flashes and a battle against Wild Mushrooms opens](https://alchemy.pascalpixel.com/guides/world/world-map-battle.gif)

As the party walks, a counter fills, faster when running and faster on some
ground than on other, and a battle starts when it is full. Each kind of ground
belongs to a zone, which decides how far apart its battles are and which
enemies can appear. A party that has outgrown a zone meets its battles less
often there, and with Avoid none at all.

`Encounter_CountStepsAndSelectFormation` adds to `gSaveBlock.encounterSteps`
every frame the leader moves and, once the counter passes 16 in 16.16 fixed
point, draws one of the zone's eight formations by weight. A field map's zones
come from `gBattle_LocationRules`, the world map's from
`gWorldMap_EncounterRules`; each zone is a row of
`src/overworld/encounter_zones.c`.

[Running a map](src/overworld/README.md#random-battles) has the roll step by
step, and [Battle](battle.md#starting-a-battle) what happens once it starts.

## Field Psynergy

![Isaac casts Move in Vale: the screen tints, a hand appears over the stone pillar and pushes it, and the colours come back](https://alchemy.pascalpixel.com/abilities/move.gif)

Using Move on a stone pillar slides it; using it where nothing answers shows
the same hand and moves nothing. What [Psynergy](glossary.md#psynergy-and-pp)
does to a map is decided by the map's own events, and the effect only draws
it.

Choosing Psynergy raises the `psynergy` request, and `Field_UsePsynergy`
handles it: it pays the PP, looks up the map's Psynergy events for what the
leader faces, runs the `PSYNERGY_PHASE_BEFORE_CAST` event, plays the effect
through `FieldEffect_Run`, and runs the `PSYNERGY_PHASE_AFTER_CAST` event. In
Vale the event after Move notes where the pillar now stands and makes its new
cell solid.

[Running a map](src/overworld/README.md#how-field-psynergy-reaches-its-effect)
covers every phase and each Psynergy's effect.

## Limits

- 64 objects, and 64 sprites sharing 64 sprite anims, up to four each.
- 66 object event pointers in the field work, the party's 8 and then 58 for
  the map's object event templates, though only 64 objects exist to fill them.
- One overlay loaded at a time, in the 32 KB of `gOverlayArea`.
- 256 map flags and 256 map section flags, each range reused by every map or
  map section.
- 104 encounter zones of eight formations each.
- 47 script commands, of which the last 11 act as `SC_END`; a jump finds only
  a label within the first 1,024 words of its script.
- A battle snapshot keeps up to 32 object events.
