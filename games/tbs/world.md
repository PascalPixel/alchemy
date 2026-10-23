# World

Everywhere the [party](glossary.md#the-party) walks, a town, a dungeon or the world map, is run by
the same four parts. The loop in `src/overworld` lets the party walk and deals
with whatever interrupts it. The engine in `src/field` moves everything that
moves and draws the [map](glossary.md#map). The two folders take the names
of the [banks](glossary.md#bank) they fill, not of what they do. The [layout](glossary.md#layout), in `src/layout`, says what the ground is like.
And each map's own code, its [overlay](glossary.md#overlay) in `src/map`, says who is there and
what happens. Only that last part knows anything about one map. How maps
and overlays fit into the whole game is on [the front page](README.md).

## The field loop and its requests

While the party walks, the game asks once a [frame](glossary.md#frame) whether something should
happen. When something should, the walk stops, the game deals with it, and
the walk starts again.

Stepping into a doorway, pressing A beside someone, a random battle
starting: each only raises a flag of its own, a request. When the walk
stops, the game deals with the raised requests one at a time, always in the
same order, and leaving comes first, so a cutscene that ends by taking a
[warp](glossary.md#warp) simply ends the map. While a request is dealt
with, the walk stays stopped, so a cutscene can be written as one
[script](glossary.md#script) that runs from start to end.

A random battle is a way out of the map too. The game remembers where
everyone stood, loads the battle as the next map (under a map number past
the end of the map table, as [the front page](README.md#the-main-loop)
explains), and afterwards loads the map again with everyone put back.

In the code, `Map_Run` runs one field map: it loads the layout, calls
the overlay's entries, then lets the leader walk, calling `Field_ProcessStep`
once a frame, until `Field_CountRequests` finds a request. The running
map's state is one block of memory, `struct FieldWork`, and each kind of
request is one member of it, so at most one of each kind can be waiting. A
battle request saves the [object events](glossary.md#object-event) with
`ObjectEvent_Snapshot`, sets the next map to `RUN_BATTLE` and sets the warp
the map returns to `WARP_NONE`, which tells the loop the next map is
already chosen; afterwards `Map_ReturnFromRun`
loads the field map again with `FLAG_KEEP_PARTY_POSITION` set, and
`ObjectEvent_Restore` puts the object events back.

| Name | Value | Meaning |
| --- | --- | --- |
| `RUN_BATTLE` | 510 (`0x1fe`) | the next map a battle request writes |
| `WARP_NONE` | 999 | the warp result that means the next map is already set |
| `FLAG_KEEP_PARTY_POSITION` | 265 (`0x109`) | the next field map starts where the party was |

[Field](src/field/README.md#the-walk-and-what-interrupts-it) explains the
walk, and [Running a map](src/overworld/README.md#the-map-loop-handles-requests-not-scripts)
lists all twelve requests in the order they are handled and goes through
each one.

## Map entries

The engine knows a map only by asking its code six questions: what can
happen here, where the party arrives, who stands where, whether anything
needs setting up, whether the leader is in a warp region, and where a warp
leads.

Each answer comes from a function rather than a fixed table, so a map can
answer differently as the story moves on, and the engine needs no idea of
the story at all. Vault's `Map_GetEvents` returns
`gVault_EventsAfterThieves` once `FLAG_VAULT_THIEVES_JAILED` is set, and
`gVault_Events` before.

The six functions are `struct MapEntries`, at the start of every overlay,
each behind a [veneer](glossary.md#veneer) word the engine calls through
([map_header.h](include/map_header.h)):

```c
struct MapEntries {
  u32 initializeVeneer;
  s32 (*initialize)(void);
  u32 warpDestsVeneer;
  const struct MapWarpDest *(*warpDests)(void);
  u32 warpsVeneer;
  const u32 *(*warps)(void);
  u32 objectEventTemplatesVeneer;
  const struct ObjectEventTemplate *(*objectEventTemplates)(void);
  u32 eventsVeneer;
  const struct MapEvent *(*events)(void);
  u32 regionsVeneer;
  const struct MapWarpRegion *(*regions)(void);
};
```

A [map event](glossary.md#map-event) table is an array of `struct MapEvent`
rows, each naming its kind, the [trigger](glossary.md#trigger) it answers, a condition and the
message or script it runs.
These are four of the rows of `gVault_Events`
([vault.c](src/map/vault.c)):

```c
struct MapEvent gVault_Events[] = {
    { MAP_EVENT_COORD, MB_VAULT_SOUTH_GATE, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_IvanStaysBehind) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), MB_VAULT_INN_DOOR, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterInn) },
    { MAP_EVENT_TALK, LOCALID_WELCOMING_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgVaultWelcomeToVaultTheTownAt) },
    { MAP_EVENT_TALK, LOCALID_ERUPTION_WITNESS, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_AskAboutEruption) },
    ...
```

The first runs a script when the leader steps on the [cell](glossary.md#cell)
marked as the south gate, the second when the leader walks north into the
inn's door, and the last two when the player talks to a townsperson, one
with a single line of dialogue, the other with a script. A condition is a
game flag that must be set or clear, or `CONDITION_ALWAYS`.
`Map_FindEvent` runs the first row whose kind, trigger and condition fit,
so a later row for the same person or cell runs only where every earlier
row's condition fails.

How an overlay is loaded and how it calls back into the [main image](glossary.md#main-image) is in
[Engine](engine.md#overlays). The front page describes [how maps depend on
the story](README.md#how-maps-depend-on-the-story) across the game.
[Maps](src/map/README.md#the-entry-table) describes the [entry table](glossary.md#entry-table-and-import-table), the
tables it returns and [how a map event is
chosen](src/map/README.md#how-a-map-event-is-chosen), and
[Places](src/field/map_headers.s) lists every map in [story order](glossary.md#the-places-in-story-order).

## Map and map section flags

A map never undoes what it changed. The game keeps two ranges of
[game flags](glossary.md#game-flag) for maps to use and forgets them as the
party moves on: map flags whenever the party enters a map,
[map section](glossary.md#map-section) flags when it enters another map
section, and neither on a return from battle. [The front
page](README.md#game-flags) gives the ranges, and
[Maps](src/map/README.md#state-that-cleans-itself-up) says how a map uses
them.

## Objects and their scripts

Everything on the field that moves or changes by itself, the party, the
townspeople, the camera, even the footprints the leader leaves in snow, is a
[object](glossary.md#object), and each one runs a small script.

![Isaac walking through Imil's snow, leaving footprints behind him](https://alchemy.pascalpixel.com/guides/field/snow.gif)

Every frame each object takes a turn: it runs the steps of its script in
order until one says to wait. A step that only tests something or jumps
takes no time, so a script can decide within a frame. The player's control
is itself one of these steps, run once a frame on the leader: `SC_PLAYER` on
a town or dungeon map, `SC_WORLD` on the world map, `SC_KEYS` while
climbing. A footprint shows an object's whole life: every 18 frames of
walking, or 12 of running, the leader's control creates one and starts it
on `gWorldMap_FootprintActions`, the two words `SC_WAIT(120), SC_FREE`.

The objects are the 64 slots of `gObjects`, each 112 bytes. A script is
an array of 32-bit words, an `SC_` command from `objcmd.h` and then its
operands, run through `gObjectScript_CommandTable` by
`Object_UpdateAllThumb`. A command that returns 1 has finished and the next
runs in the same frame; one that returns 0 ends the object's turn. A jump
stores no offset: `ObjectScript_FindLabel` searches the script for the matching
`SC_LABEL` word, looking through at most the first 1,024 words
(`SCRIPT_LABEL_SEARCH_WORDS`) and starting the script again from the top
when it finds none. `SC_END` leaves the object idling on
`gScript_MainScript`; only `SC_FREE` frees the slot.

Different parts of the code read an object's 112 bytes through different
structs (`Object`, `ScriptMotionObject`, `ObjectEvent` and others),
each naming the fields that part uses. They stay apart because the build
must reproduce the [original](glossary.md#the-original)'s instructions, and
reading the same bytes through one merged struct makes the compiler choose
others; [Matching](src/MATCHING.md#views-of-one-struct) explains the rule.

| Name | Value | Meaning |
| --- | --- | --- |
| `OBJECT_COUNT` | 64 | the slots of `gObjects` |
| `FIELD_SPRITE_FOOTPRINT` | 25 | a footprint's [sprite](glossary.md#sprite-and-oam), on maps that show them |

[Field](src/field/README.md#everything-that-moves-runs-a-script) covers the
scripts and their commands, the sprites and [the views of one
object](src/field/README.md#one-object-a-few-views).

## Movement

The leader walks at one pixel a frame and runs at one and a half. Before
every step the game checks the ground just ahead, and when the leader walks
into a wall it tries other directions, so the leader slides along the wall
instead of stopping dead.

![Isaac walking and then running in Vale](https://alchemy.pascalpixel.com/guides/field/walk-run.gif)

On a town or dungeon map the leader walks at `FIELD_WALK_SPEED` and runs at
`FIELD_RUN_SPEED` while a dash button is held. Each step needs five clear
checks 8 pixels away: the way the pad points and 22.5 and 45 degrees either
side. When one fails, `Object_UpdateLeaderControl` tries headings 22.5,
45 and 67.5 degrees off, nearest first, each with five checks of its own, and
takes the first that passes. `MapGrid_CheckCollision` decides each check from
the cell there. Speeds are in 16.16 fixed point, the engine's way of
holding fractions of a pixel in a whole number: the upper 16 bits count
whole pixels and the lower 16 bits sixty-five-thousandths of one, so
`0x10000` is one pixel and `0x18000` one and a half.

| Name | Value | Meaning |
| --- | --- | --- |
| `COLLISION_TOO_LOW` | -1 | the ground there is more than 12 pixels lower |
| `COLLISION_NONE` | 0 | the object can step there |
| `COLLISION_TOO_HIGH` | 1 | the ground there is more than 8 pixels higher |
| `COLLISION_IMPASSABLE` | 2 | the cell there is a wall (`MB_WALL`) |
| `FIELD_WALK_SPEED` | `0x10000` | 1 pixel a frame |
| `FIELD_RUN_SPEED` | `0x18000` | 1.5 pixels a frame, with a dash button held |

[Field](src/field/README.md#walking-and-running) has the speeds on each kind of
map and [the ground under an
object](src/field/README.md#how-high-a-step-can-go).

## Cells and metatile behaviors

A layout is a grid of 16-pixel squares, cells, and each cell says three things:
what is drawn there, how high its ground is, and what standing there does.

What standing on a cell does is its [metatile behavior](glossary.md#metatile-behavior), and it is what walls
and map events are keyed on. The behaviors from `MB_EVENT_FIRST` to
`MB_EVENT_LAST` belong to the map, and its map event table says what each one
does, so the same behavior is a door in one town and a chest in another. The
ones above them mean the same on every map: ground with battles of its own,
the jars and barrels anyone can search, the desert's sand, ledges, climbing
spots and walls. A wall need not be marked, either: the leader can step at most 8
pixels up or 12 down, so ground that rises sharply stops him as surely as a
wall. Goma Cave has no wall-coded cells at all; a rim of ground 32 pixels
high runs along the foot of each cliff.

The three layers of a layout are windows onto one grid of 128 by 128 cells,
decoded into the 64 KB at `Ram_MapGridCellBuffer`. A cell is one 32-bit word
(`struct MapGridCell`):

```
 31        24 23        16 15  14 13  12 11                    0
+------------+------------+------+------+-----------------------+
| ground     | metatile   | prio-| layer| metatile              |
| shape      | behavior   | rity | + 1  |                       |
+------------+------------+------+------+-----------------------+
```

The low 12 bits pick the [metatile](glossary.md#metatile). The two fields
above it, where they are not 0, give the map layer and the sprite priority
an object standing on the cell takes, so the cells behind a house can put
whoever walks there behind it (see [Map layers](#map-layers)). The third
byte is the metatile behavior, and the top byte the ground shape, from which
`MapGrid_GetTerrainHeight` computes the ground height.

| Name | Value | Meaning |
| --- | --- | --- |
| `MB_EVENT_FIRST` | 1 | the first metatile behavior the map's events name |
| `MB_EVENT_LAST` | 239 | the last of them |
| `MB_ENCOUNTER_GROUND_1`, `MB_ENCOUNTER_GROUND_2` | 240, 241 | ground with battles of its own |
| `MB_JAR` to `MB_WELL` | 242 to 247 | jars, barrels, boxes, graves, books and wells, searched on every map |
| `MB_DESERT_SAND` | 250 | sand that hurts the party as it walks |
| `MB_LEDGE`, `MB_CLIMB`, `MB_JUMP_FORWARD` | 252 to 254 | jumped down, climbed, or jumped across |
| `MB_WALL` | 255 | a wall, whatever the heights |

[Layouts](src/layout/README.md#cells) gives every metatile behavior, and goes on to the
layout's tables and [how the build makes each
layout](src/layout/README.md#how-the-build-makes-a-layout) from editable tables.

## Map layers

A town or dungeon is drawn as flat sheets stacked on top of each other: the
ground at the back, then walls and edges, then the characters, then roofs
and treetops in front. That is how Isaac can walk behind a roof.

No code works out whether someone is hidden behind a roof or a tree. The
console draws its [background layers](glossary.md#background-layer) and sprites in an order set by their
priorities, and the characters are simply drawn between two layers of the
same map.

The three map layers are three of the console's four background layers:
`MAPGRID_LAYER_GROUND` on `BG3`, `MAPGRID_LAYER_MIDDLE` on `BG2` and
`MAPGRID_LAYER_OVERHEAD` on `BG1`; `BG0` is the window layer, where messages and
menus are drawn. Each layer's priority comes from the map's header, and most
maps give the three layers 3, 2 and 1. A sprite wins a tie with a
background. The object events placed from a table, the leader's included,
carry `OBJ_EVENT_PRIORITY_AUTOMATIC`, so each frame their sprites take the
priority the cell under them sets, and keep their last one where it sets
none (`ObjectSystem_PlaceSprites`). In Vale:

- on open grass Isaac's sprite is at 2, so he stands in front of the middle
  layer and behind the overhead one;
- behind a house the cells set 3, so he is behind the house's walls as well
  as its roof.

A priority of 0 leaves a layer off: `MapGrid_LoadLayout` enables only the
layers whose priority isn't zero.

| Name | Value | Background | What it holds |
| --- | --- | --- | --- |
| `MAPGRID_LAYER_GROUND` | 0 | `BG3` | the ground, at the back |
| `MAPGRID_LAYER_MIDDLE` | 1 | `BG2` | walls and edges, most often under the characters |
| `MAPGRID_LAYER_OVERHEAD` | 2 | `BG1` | roofs and tree tops, over the characters |

[Field](src/field/README.md#towns-and-dungeons-on-screen) shows how
the layers are put on screen, and
[Layouts](src/layout/README.md#one-grid-three-layers) how they share one grid.

## The world map

The world map is a flat map tilted into perspective, so the land near the
party looks close and the land towards the top of the screen far away.

![Isaac walking on the world map, the land tilting away towards the top of the screen](https://alchemy.pascalpixel.com/guides/field/world-map-walk.gif)

The console can scale and rotate two of its background layers, and the game
changes the scale for every line of the screen. Each screen line shows one
straight strip of the flat map; lines higher on the screen show strips that
are farther away and wider, so together they show a trapezoid around the
party, and the land seems to tilt away. When the player holds L to zoom out,
the lines spread so far apart that some strips of the map are never drawn.

In the code, a background the console can scale and rotate is an affine
background: for each screen line it takes a starting point on the map and a
step to move by for each pixel, its affine values, and draws what it finds
along that line. In display mode 2, the screen setting the world map uses,
`BG2` and `BG3` are both affine: the ground is `BG3`, the middle map layer of
mountains and forest is `BG2`, and the towns and the party are sprites drawn
flat over both.

`WorldMap_BuildScanlineTable` gives each screen line (scanline) a width
scale and a ground distance when the world map is set up, and again only if
the camera's pitch changes, which nothing in the game does. Each frame
`Transform_UpdateVertices` turns them into a page of affine values. It is ARM
code, the processor's full 32-bit instructions, run from [IWRAM](glossary.md#iwram-and-ewram), its fast
internal memory, since that is where such code runs quickest; the rest of
the game is in the compact 16-bit Thumb form. `WorldMap_ApplyAffineFrame`
sets the first line's values and starts an HBlank DMA: a copy the console
makes by itself in the HBlank, the pause after each line is drawn, here
feeding each following line its own values. There are two pages, and
alternate frames use them: the DMA reads one page throughout a frame while
`Transform_UpdateVertices` builds the other.

When a menu or script starts on the world map, `WorldMap_StartStillView` draws
the current affine frame into ordinary tiles on `BG1` and switches the display
to `DISPCNT_MODE_0`, in which no background is affine. Mode 2 shows only the
two affine layers, so the switch also brings back `BG0`, the window layer the
menu or script's messages are drawn on. `WorldMap_LoadAreaGraphics` undoes
it. The world map's own code, its places and map events, is described on
[the front page](README.md#the-world-map).

[Field](src/field/README.md#the-world-map-in-perspective) works through the
scanline table, [Layouts](src/layout/README.md#the-world-maps-data) the world map's
data, and [Running a map](src/overworld/README.md#the-world-map-screen) the world
map screen the R button brings up.

## Random encounters

As the party walks, a counter fills, faster on some ground than on other,
and a battle starts when it is full. A party that has outgrown a place meets
its battles less often there.

![A cave giving way to a random battle that opens with `Ghost appeared!`](https://alchemy.pascalpixel.com/guides/object/random-battle.gif)

Each kind of ground belongs to a zone, and the zone decides how far apart
its battles are and which enemies can appear. Each frame the leader walks,
`Encounter_CountStepsAndSelectFormation` adds to the counter, `gSaveBlock.encounterSteps`;
despite its name it does both jobs, filling the counter every frame and
choosing the formation once it is full. What it adds:

- grows with the leader's top speed at the moment, so running fills the
  counter faster than walking;
- shrinks as the zone's rate grows, so a higher rate means a longer walk
  between battles;
- varies by a random bias, drawn again after each battle.

Each level, up to five, by which the party's average level is above the
zone's adds 5 to the rate, and with Avoid active (the Psynergy that keeps
such battles away) a zone the party has outgrown has no battles at all. There
are no battles while Cloak is active, on ground with no zone, or while a flag
turns them off. A
battle starts when the counter reaches `work->encounterThreshold`, 16 in
16.16 fixed point, and one of the zone's eight formations is drawn by
weight. A field map's zones come from the
first matching rule in `gBattle_LocationRules`, which assigns zones to the
encounter grounds its cells mark; the world map's come from
`gWorldMap_EncounterRules`, by the encounter region under the leader. Each zone is a
row of `src/overworld/encounter_zones.c`.

[Running a map](src/overworld/README.md#random-battles) has the roll step by
step, and [Battle](battle.md#starting-a-battle) what happens once it starts.

## Field Psynergy

Using Move on a stone pillar slides it; using it on a blank wall shows the
same hand and moves nothing. What [Psynergy](glossary.md#psynergy-and-pp) does to a map is decided by the
map's own map events, and the effect only draws it.

![Isaac casts Move in Vale: the screen tints, a hand appears over the stone pillar and pushes it, and the colours come back](https://alchemy.pascalpixel.com/abilities/move.gif)

Choosing Psynergy from the menu or a shortcut raises the `psynergy` request,
and `Field_UsePsynergy` handles it:

1. It checks the Psynergy can be used there, pays the PP and looks up the
   map's Psynergy events for what the leader faces.
2. It runs the `PSYNERGY_PHASE_BEFORE_CAST` event.
3. It plays the effect for the ability's field kind (`enum FieldPsynergy`, from
   the `type` column of `src/game/abilities.tsv`, so Move's hand or Frost's
   ice) through `FieldEffect_Run`, or `FieldEffect_DispatchRequestKind` when
   an event names the target. An event in the `PSYNERGY_PHASE_EFFECT_ACTS`
   phase runs from inside the effect, at the moment Force strikes or Frost
   freezes.
4. It runs the `PSYNERGY_PHASE_AFTER_CAST` event. In Vale, the one after Move
   notes where the pillar now stands and makes its new cell solid.

| Name | Value | Meaning |
| --- | --- | --- |
| `PSYNERGY_PHASE_AFTER_CAST` | 0 | after the effect |
| `PSYNERGY_PHASE_BEFORE_CAST` | 1 | once the PP is paid, before the effect |
| `PSYNERGY_PHASE_EFFECT_ACTS` | 5 | from inside the effect, where it acts on the map |

[Running a map](src/overworld/README.md#how-field-psynergy-reaches-its-effect)
covers the dispatch, every phase and each field kind's effect.

## Limits

- 64 objects, and 64 sprites sharing 64 sprite anims, up to
  four each.
- 66 object event pointers in the field work, the party's 8 and then 58
  for the ones the map's object event templates place, though only 64
  objects exist to fill them.
- One overlay loaded at a time, in the 32 KB of `gOverlayArea`.
- 256 map flags and 256 map section flags, each range reused by every map or
  map section.
- 104 encounter zones of eight formations each.
- 47 script commands, of which the last 11 act as `SC_END`; a jump finds
  only a label within the first 1,024 words of its script.
- A battle snapshot keeps up to 32 object events.
