# ☀️ The Broken Seal

[The Broken Seal](glossary.md#the-two-games) plays one [map](glossary.md#map)
at a time: a room, a town, a dungeon floor, the world map. When the
[party](glossary.md#the-party) walks through a door, the game ends one map and
starts the next. Each map brings its own code, loaded only while the party is
there, and its own [layout](glossary.md#layout), the picture it shows and the
ground under it. What a map knows about how far the story has gone, it reads
from the [game flags](glossary.md#game-flag), a long list of yes-or-no facts
saved with the game. The rest of the program is always there, cut into
[banks](glossary.md#bank) ([How the code is organised](#how-the-code-is-organised)).

## The main loop

![Isaac walks up into the Sanctum's doorway in Vale; the village fades to black and the Sanctum's hall fades in around him, its people standing round the altar](https://alchemy.pascalpixel.com/guides/front/sanctum-door.gif)

Walking through the Sanctum's door ends Vale and starts the Sanctum: the game
throws away everything Vale had set up, loads the Sanctum's code and runs it
until the party leaves again. The whole game is that loop, once for each map,
not once a [frame](glossary.md#frame). A map can run for minutes; while it
does, its code waits frame by frame and the engine keeps the screen moving
([Engine](engine.md#waiting-and-tasks)). A battle or a minigame takes a map's
place in the loop, and when it ends the game goes back to the map it started
from, with everyone where they stood.

`Game_Run` ([overworld/map.c](src/overworld/overworld/map.c)) is the loop, and
it never returns. Each pass first clears what the last map left: DMA channel 0
(a map may leave it rewriting a display setting after every line, for a wave
or a tilt), every [task](glossary.md#task), the HBlank and VCount handlers and
the [heap](glossary.md#heap-and-pools). Then, for a field map:

1. `Map_ResetFlagsOnEnter` clears the flags that last one map or one
   [map section](glossary.md#map-section) ([Game flags](#game-flags)).
2. The map's music starts, unless `FLAG_KEEP_MUSIC` or `FLAG_KEEP_MUSIC_HELD`
   keeps what is playing.
3. `Map_LoadOverlay` unpacks the map's code, its [overlay](glossary.md#overlay).
4. `Map_Run` runs the map until the party takes a [warp](glossary.md#warp)
   (`VALE_TO_SANCTUM`, say) and returns it.
5. `Map_TakeWarp` looks the warp up to find the next map and the
   [warp destination](glossary.md#warp-destination) there.

A battle or minigame runs under a map number past the end of the map table
(`RUN_BATTLE` and the rest below), with no overlay, since its code is in the
[main image](glossary.md#main-image). It returns a result instead of a warp,
and `Map_ReturnFromRun` picks where to go: after a defeat, the point a
[script](glossary.md#script) set for losing that battle, or else the last
save; otherwise the destination a script set, or else the map the battle
started from, with `FLAG_KEEP_PARTY_POSITION` set so the party and the
[object events](glossary.md#object-event) stand where they were. A random
battle is a warp like any other: the map sets the next map to `RUN_BATTLE` and
leaves. [Running a map](src/overworld/README.md) follows a field map from
`Map_Run` to its warp.

`System_Initialize` starts the loop at power-on with `GAME_RUN_TITLE`, so the
first map is the title screen. The title and the main menu are the map table's
first two rows, run like maps though the party never walks there.

| Name | Value | Meaning |
| --- | --- | --- |
| `GAME_RUN_TITLE` | 0 | start at the title, with a fresh save block |
| `GAME_RUN_DEBUG_VALE` | 1 | in debug mode, start at `MapId_ValePlaza` |
| `GAME_RUN_DEBUG_MAIN_MENU` | 2 | in debug mode, start at the main menu |
| `RUN_NONE` | 507 | the first map number with no row in the map table (rows 0 to 200) |
| `RUN_LUCKY_WHEELS` | 508 | Lucky Wheels at Tolbi's spring |
| `RUN_LUCKY_DICE` | 509 | Lucky Dice at Tolbi's spring |
| `RUN_BATTLE` | 510 | a battle |

## Maps and overlays

Only one map's code is in memory at a time. Most of what a map needs (who
stands there, what they say, its cutscenes) matters nowhere else, so each
map's code is packed on the cartridge as an overlay and unpacked into the same
32 KB of [EWRAM](glossary.md#iwram-and-ewram), `gOverlayArea`, over the map
before it.

The engine knows a map only through six entry points at the start of its
overlay (`struct MapEntries`): its initialization, and the functions that
return its warp destinations, warps, object event templates,
[map events](glossary.md#map-event) and warp regions. They are functions, not
pointers to tables, so a map can return a different table as the story moves
on. The overlay ends in an
[import table](glossary.md#entry-table-and-import-table) for the main-image
functions it calls. Overlays never call each other; code several maps need is
compiled into each from `map/common/`. [Engine](engine.md#overlays) has the
two tables and [Maps](src/map/README.md) how a map event is found.

## The map table and map sections

![Isaac steps from the world map onto Vale's marker; the village's gate fades in with the name Vale in a small window at the top of the screen, and he walks up through the gate](https://alchemy.pascalpixel.com/guides/front/area-name.gif)

Arriving at Vale from the world map shows the name "Vale" for a moment. The
name belongs to Vale's map section, the group of maps the village is made of:
walking between Vale's houses shows nothing, because the party stays in the
same section. A map's code says what happens there; its map section chooses
the music, the area name and which flags are kept while the party stays
inside. The two are chosen separately: the Bilibin Barricade runs on
Bilibin's code but is a map section of its own.

The map table, `gMapHeaders` in [field/map_headers.s](src/field/map_headers.s),
has one row (`struct MapHeader`) for each of the 201 maps: the overlay to
load, the map section (`mapSec`), the map type (`mapType`) and the layout.
Each row defines its map's id, `MapId_<Place>`, as its position, so code names
a map by a symbol rather than a number. On a change of map section
`Map_ResetFlagsOnEnter` sets `FLAG_SHOW_LOCATION_NAME`, and `Map_Run` then
calls `MapNamePopup_Show` for `MAP_NAME_POPUP_FRAMES`. A few maps clear the
flag first so that no name shows (`ValeCave_Initialize`).

A `MAP_TYPE_TOWN` map, a town or one of its interiors, restores no
[PP](glossary.md#psynergy-and-pp), recovers no [Djinn](glossary.md#djinn) and
does no poison damage as the party walks. Retreat, which takes the party back
to where it entered the map section, is blocked there, and Avoid doesn't wear
off.

| Name | Value | Meaning |
| --- | --- | --- |
| maps | 201 | rows of `gMapHeaders` |
| overlays | 96 | the maps' code, some shared by several maps |
| map sections | 71 | groups of maps: their music, area name and kept flags |
| `MAP_NAME_POPUP_FRAMES` | 90 | frames the area name shows |
| `MAP_TYPE_FIELD` | 1 | every other map: dungeons, the world map |
| `MAP_TYPE_TOWN` | 2 | a town or one of its interiors |

## Warps and warp destinations

![Isaac walks down the path from Sol Sanctum's entrance and arrives at the foot of the steps in Vale; then the same walk again, and this time the warp leads to Vale erupting, the villagers crowded beside the bridge while the river and the cliffs glow red](https://alchemy.pascalpixel.com/guides/front/sol-entrance-warp.gif)

The path down from Sol Sanctum's entrance is one warp, but where it leads
depends on the story: to Vale, or, on the night Mt. Aleph erupts, to the
village in flames. A warp names where it goes; a row in the map's warp table
turns it into a map and a warp destination, and a row can be made to count
only while a flag is clear. The warp destination is where the party appears
and which way it faces.

[map_warps.h](include/map_warps.h) names every way between maps. Warp
destinations are named for where the party comes from
(`VALE_PLAZA_FROM_SANCTUM`) and warps for where they lead
(`VALE_PLAZA_TO_WORLD_MAP`), so a door has the same name from both sides.
`Map_TakeWarp` takes the first row for the warp taken; a `MAP_WARP_IF` row
counts only while its flag is clear. These are the rows of the path in the
recording ([sol_sanctum_entrance.c](src/map/sol_sanctum_entrance.c)):

```c
u32 gSolSanctumEntrance_Warps[] = {
    MAP_WARPS(SolEntrance),
    MAP_WARP_IF(SOL_ENTRANCE_TO_VALE, Vale, VALE_FROM_SOL_ENTRANCE,
                FLAG_SOL_SANCTUM_ERUPTED),
    MAP_WARP_IF(SOL_ENTRANCE_TO_VALE, ValeEruption,
                VALE_ERUPTION_FROM_SOL_ENTRANCE, FLAG_VALE_PARTY_LEFT),
    MAP_WARP_IF(SOL_ENTRANCE_TO_VALE, Vale, VALE_FROM_SOL_ENTRANCE,
                WARP_FLAG_NONE),
    ...
```

So the path leads to Vale until the sanctum erupts, to Vale erupting until
the party leaves Vale, and to Vale again for the rest of the game. A map's
initialization often tests the warp destination too, to play the part of a
sequence the last map sent the party to.

## Game flags

![In a small room of Sol Sanctum, Isaac searches the chest in front of him: a green Small Jewel rises out of it, he holds it over his head and a window reads "Isaac got a Small Jewel."](https://alchemy.pascalpixel.com/guides/front/chest.gif)

The chest gives its Small Jewel once. Opening it sets a game flag,
`FLAG_SOL_SANCTUM_FIRST_SMALL_JEWEL_FOUND`, and the chest's row in the map's
event table counts only while that flag is clear; from then on the chest is
drawn open and searching it finds it empty. A map keeps nothing of its own
between visits: whatever the game must remember, a chest or the end of a
chapter, is a flag saved with the game.

The game flags are 4,096 bits in `gGameFlagBytes`, named in
[flags.h](include/flags.h): the party and the Djinn first, then the flags the
engine shares, then the story, then most chests and finds, each map section's
together. Two ranges are scratch space a map never has to undo.
`Map_ResetFlagsOnEnter` clears them on every arrival without
`FLAG_KEEP_PARTY_POSITION`, which a return from battle sets:

- the map's own flags (`FLAG_MAP_TEMP_FIRST` to `FLAG_MAP_TEMP_LAST`) on every
  such arrival, so a pillar kept in one is back in place next time;
- the map section's flags (`FLAG_MAPSEC_TEMP_FIRST` to `FLAG_MAPSEC_TEMP_LAST`)
  only on entering another map section.

Entering a map section also sets its `FLAG_MAPSEC_VISITED` flag, which puts
its marker on the world map screen. Eight flags starting at a multiple of 8
can be read as a byte, which is how counters live among the flags. Because
rows and flags interact, some code can never run; its comment opens with
[`Unused:`](glossary.md#unused), and [Unused](https://alchemy.pascalpixel.com/tbs/unused)
collects every case. [Game rules](src/game/README.md#the-game-flags) has the
layout and the calls.

| Name | Value | Meaning |
| --- | --- | --- |
| `FLAG_DJINN_FIRST` | 48 | Flint's flag, the first Djinn flag |
| `FLAG_KEEP_PARTY_POSITION` | 265 | keeps both scratch ranges, and the party where it was |
| `FLAG_KEEP_MUSIC` | 282 | keeps the music for one map change |
| `FLAG_KEEP_MUSIC_HELD` | 283 | keeps the music for as long as it is set |
| `FLAG_MAPSEC_VISITED(group)` | 384 + group | the map section's marker on the world map screen |
| `FLAG_MAP_TEMP_FIRST` to `_LAST` | 512 to 767 | the map's 256 flags |
| `FLAG_MAPSEC_TEMP_FIRST` to `_LAST` | 768 to 1,023 | the map section's 256 flags |

## How maps depend on the story

![Vale's gate three times: on the stormy night, rain falling and the villagers gathered at the top of the screen; after the party has left, a guard standing by the path; and later in the journey, more villagers about](https://alchemy.pascalpixel.com/guides/front/vale-plaza-story.gif)

The same gate is one map for the whole game, yet different people stand there
at different points in the story. Each time the party arrives, the map asks
the flags which version of itself to be and returns that version's tables.
The story is the order the flags are set in, and these maps show the forms
the dependence takes, in [story order](glossary.md#the-places-in-story-order).

- **One map that reads the flags.** Vale's gate,
  [vale_plaza.c](src/map/vale_plaza.c), in the recording. `ValePlaza_GetEvents`
  asks in order:

  ```c
  const struct MapEvent *ValePlaza_GetEvents(void) {
    if (GameFlag_Get(FLAG_KALAY_REACHED) != 0) {
      return gValePlaza_EventsAfterKalay;
    }
    if (GameFlag_Get(FLAG_VALE_PARTY_LEFT) != 0) {
      return gValePlaza_EventsAfterLeaving;
    }
    if (gSaveBlock.warpDest == VALE_PLAZA_FROM_SANCTUM) {
      return gValePlaza_DepartureEvents;
    }
    if (GameFlag_Get(FLAG_VALE_STORM) != 0) {
      return gValePlaza_StormEvents;
    }
    return gValePlaza_Events;
  }
  ```

  Its warps and object event templates are chosen the same way.
- **One map per point in the story.** The prologue's stormy night elsewhere
  in Vale is a map of its own, [vale_stormy_night.c](src/map/vale_stormy_night.c),
  on its own layout, and it sets `FLAG_VALE_STORM` when it starts. A place that
  changes completely gets a map; a place whose people change reads the flags.
- **A map with no map events.** [vale_eruption.c](src/map/vale_eruption.c),
  Vale while Mt. Aleph erupts, plays its whole sequence from its
  initialization and leaves by its one warp.
- **A row that lasts until its flag is set.** Each pedestal of the Elemental
  Star room,
  [sol_sanctum_elemental_star_room.c](src/map/sol_sanctum_elemental_star_room.c),
  is a [bg event](glossary.md#bg-event) row that lasts until its star's flag
  is set, so each star is taken once, as the chest above is opened once.
- **A flag set by a warp destination.** The World Map's initialization sets
  `FLAG_VALE_PARTY_LEFT` the first time the party arrives by
  `WORLD_MAP_FROM_VALE_PLAZA`, and the gate's tables change from then on.
- **One sequence cut into parts by warp destinations.**
  [venus_lighthouse_aerie.c](src/map/venus_lighthouse_aerie.c) is one scripted
  sequence broken by two battles and two trips to the World Map; each part
  starts from the warp destination the part before sent the party to.
- **The last map.** [lemurian_ship.c](src/map/lemurian_ship.c), at the edge of
  Lalivero, ends the game by leaving for the title screen by
  `TITLE_FROM_LEMURIAN_SHIP`.

[Places](src/field/map_headers.s) lists every map in story order with what
happens there.

## The world map

![The whole world map of The Broken Seal laid out flat: the land of mountains, rivers and forest that the story crosses, two small islands in the sea, and plain blocks where the map's data leaves squares blank](https://alchemy.pascalpixel.com/story/world.png)

The World Map, [world_map.c](src/map/world_map.c), is a map like the others,
drawn in perspective on the two background layers the hardware can scale and
rotate. The picture above is the whole of it laid flat. The world is stored as
squares 512 pixels on a side, and only the 2 by 2 squares around the party
are unpacked at a time (`WorldMap_UpdateCurrentTileBlock`).

Its code does three kinds of work. Each town or dungeon is an object event on
the map, and touching one raises the trigger its map event turns into that
place's warp. It holds what the party meets on the map itself, three Djinn
and a storm. And its initialization plays the story's moments between places,
chosen by the warp destination the last map sent the party to.
[World](world.md#the-world-map) explains how the tilt is drawn.

## How the code is organised

The main image, the program the processor runs straight from the cartridge,
is laid out by `main.ld` in fourteen banks, about 1 MB of the English
cartridge, followed by the sound code, the music and the packed data. A caller
in another bank goes through the bank's far-call table, by the function's name
with `Far` added: `Game_Run` starts a battle with `Battle_RunFar`, which leads
to `Battle_Run` in the `battle` bank. The [glossary](glossary.md#far-call) says
how a far call works, and
[System](src/system/README.md#why-the-program-is-cut-into-banks) why the
program is cut into banks.

The source's folders follow the cartridge:

- The nine large banks each have a folder, in cartridge order: `runtime`,
  `field`, `window`, `game`, `overworld`, `menu`, `shop`, `battle` and
  `battle_anim`. `overworld` runs a map from arrival to warp; `field` moves
  and draws the objects.
- The five small banks' code sits in the folder it belongs with.
- `graphics`, `layout`, `sound` and `system` are grouped by topic and linked
  into whichever bank or image needs them.
- `map` and `debug` hold overlays.

[`objects.tsv`](objects.tsv) lists every object in link order with the image
and bank it goes in, and [Source](src/README.md) goes through every folder.

## Section pages

- [World](world.md): the field engine, the layouts, the request loop a map
  runs and field Psynergy.
- [Battle](battle.md): how a battle runs from the first menu to the spoils,
  how damage and healing are worked out, and how its animations are drawn.
- [Menus](menus.md): the field menu and its screens, the shops and inns, and
  the windows text is drawn in.
- [Engine](engine.md): start-up, frame timing and tasks, banks and far calls,
  overlays and songs.
- [Places](src/field/map_headers.s): every map in story order, with its object
  event templates and map events.
- [Game rules](src/game/README.md): stats, items, Djinn, classes, levels and
  the flags.
- [Unused](https://alchemy.pascalpixel.com/tbs/unused): the maps, scripts and
  lines the game keeps but never plays, and why each never runs.
- [Source](src/README.md): every file of the code, with its comments.
- [Glossary](glossary.md): the words every page uses, from Psynergy and Djinn
  to banks, overlays and bg events.
