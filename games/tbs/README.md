# ☀️ The Broken Seal

[The Broken Seal](glossary.md#the-two-games) plays one [map](glossary.md#map) at a time: a room, a town, a dungeon floor,
the world map. When the [party](glossary.md#the-party) walks through a door, the game ends one map and starts the next. Each map has code of its own, loaded only
while the party is there, and a [layout](glossary.md#layout). Everything a map knows about how far
the story has gone comes from the [game flags](glossary.md#game-flag), a long list of yes-or-no facts
saved with the game. The rest of the program is always there, cut into
[banks](glossary.md#bank), stretches of the cartridge that call each other
only through a table of jumps at the start of each; the source has one
folder for each large bank ([How the code is
organised](#how-the-code-is-organised)).

## The main loop

The game runs one map, then the next, for as long as it is on. This loop
goes round once for each map, not once a [frame](glossary.md#frame): a map can last for
minutes, and while it runs, its own code waits frame by frame and the
engine keeps the screen moving, as [Engine](engine.md#waiting-and-tasks)
explains.

Each time round, the game first clears away everything the last map left
behind, so nothing one map set up can leak into the next. Then it runs the
next map. A field map runs until the party takes a [warp](glossary.md#warp), and the warp says
which map comes next and the [warp destination](glossary.md#warp-destination) where the party arrives. A
battle or a minigame runs in place of a map, under a map number past the
end of the map table: it runs, and then the game picks the map to go back
to, most often the map the battle started from, with everyone where they
stood.

`Game_Run`, in [overworld/map.c](src/overworld/overworld/map.c), is this loop of
maps, and it never returns. `System_Initialize` calls it at power-on
with `GAME_RUN_TITLE`, which makes the title screen (`MapId_Title`) the
first map. The current map and the warp destination the party arrives by
are `gGameState.mapId` and `gGameState.warpDest`.

The title screen and the main menu are the map table's first two
rows, `MapId_Title` and `MapId_MainMenu`, so the loop runs them with code of
their own the way it runs a map, though the party never walks there.
The main menu is where the player chooses a file; the debug map list calls it
"Clear data". The game also goes there after the credits. Two debug start modes, taken only while
`gDebugMode` is set, begin at Vale's gate or at the main menu
instead of the title.

Each pass of the loop first clears away what the last map left running:

- DMA channel 0, if a map left it running. A DMA channel is a part of the
  console that copies memory by itself, and a map can set one to change a
  display setting during every HBlank, the short pause after each line of
  the screen is drawn, for a wave or a tilt.
- Every [task](glossary.md#task), the functions the engine calls once a frame for a map (one
  types out text, another scrolls the map grid).
- The HBlank and VCount handlers, functions the hardware calls after each
  line, or when it reaches one chosen line.
- The [heap](glossary.md#heap-and-pools), the memory from which maps borrow blocks for their state; it
  is emptied, so every block the last map held is free again.

Then it does one of two things:

- A field map is prepared and run:
  1. `Map_ResetFlagsOnEnter` clears the flags that last for one map or one
     map section ([Game flags](#game-flags)).
  2. Its music starts, unless `FLAG_KEEP_MUSIC` or `FLAG_KEEP_MUSIC_HELD`
     keeps what is playing.
  3. `Map_LoadOverlay` unpacks its code, its [overlay](glossary.md#overlay).
  4. `Map_Run` runs it until the party takes a warp, and returns that
     warp (`VALE_GATE_TO_WORLD_MAP`, say).
  5. `Map_TakeWarp` looks the warp up in the map's warp table to find the
     next map and warp destination ([Warps](#warps-and-warp-destinations)).
- A battle or a minigame runs in place of a map, under a map number from
  `RUN_NONE` up
  (`RUN_BATTLE`, `RUN_LUCKY_DICE`, `RUN_LUCKY_WHEELS`), with no row in the
  map table and no overlay, since its code is in the [main image](glossary.md#main-image). It returns
  a result rather than a warp, and `Map_ReturnFromRun` picks the map
  to go back to: after a defeat, the point a [script](glossary.md#script) set for losing
  that battle, or else where the game was last saved; otherwise the
  destination a script set, or else the field map the battle started from, with
  `FLAG_KEEP_PARTY_POSITION` set so that the party and the [object events](glossary.md#object-event)
  come back where they were.

A random battle is therefore a warp like any other: the field map sets
the next map to `RUN_BATTLE` and leaves, and the loop loads it again
afterwards. [Running a map](src/overworld/README.md), the guide to the
`overworld` folder, follows a field map
from `Map_Run` to its warp, including how a battle is chosen from the
encounter zones in `encounter.tsv`.

The tables on this page give each value as an ordinary number, with the
hexadecimal form the source writes it in beside it.

| Name | Value | Meaning |
| --- | --- | --- |
| `GAME_RUN_TITLE` | 0 | start at the title, with a fresh game state |
| `GAME_RUN_DEBUG_VALE` | 1 | in debug mode, start at `MapId_ValeGate` |
| `GAME_RUN_DEBUG_MAIN_MENU` | 2 | in debug mode, start at the main menu (`MapId_MainMenu`) |
| `RUN_NONE` | 507 (`0x1fb`) | the first map number with no row in the map table, whose rows are 0 to 200 |
| `RUN_LUCKY_WHEELS` | 508 (`0x1fc`) | Lucky Wheels at Tolbi's spring |
| `RUN_LUCKY_DICE` | 509 (`0x1fd`) | Lucky Dice at Tolbi's spring |
| `RUN_BATTLE` | 510 (`0x1fe`) | a battle |

## Maps and overlays

Only one map's code is in memory at a time. When the party enters a map,
its code is unpacked from the cartridge over the last map's.

The console has little memory, and most of what a map needs (who is there,
what they say, its cutscenes) matters nowhere else. So each map's code is
an overlay: a packed piece of code that `Overlay_Load` unpacks into
`gOverlayArea`, 32 KB of [EWRAM](glossary.md#iwram-and-ewram) (the console's larger, slower work memory,
256 KB in all), when the map loads, over the one before.

The field engine knows a map only through six entry points, short jumps
at the start of its overlay (`struct MapEntries`): its initialization, and the functions
that return its warp destinations, warps, object event templates, [map events](glossary.md#map-event)
and warp regions. They are functions rather than pointers to tables so that
a map can return a different table as the story moves on. Each overlay also
ends in an [import table](glossary.md#entry-table-and-import-table), one
short jump for each main-program function it calls. Overlays never call
each other, and code several maps need is compiled into each of them from
`map/common/`.
[Maps](src/map/README.md) describes the tables and how to find a map event,
and [Engine](engine.md#overlays) the overlay's two tables.

The other half of a map is its layout: three [background layers](glossary.md#background-layer) with the
object events drawn between them, and a grid of [cells](glossary.md#cell) under them that says
how high the ground is, where it can be walked and which cells raise map
events. [World](world.md) explains both.

## The map table and map sections

One table lists every map the game has, and for each says which code it
runs, which [map section](glossary.md#map-section) it belongs to and which layout it shows.

A map's code says what happens there, and its map section, a group of maps,
chooses the music, the area name shown on arrival and the flags kept while
the party stays inside. The two are chosen separately: the Bilibin
Barricade runs on Bilibin's code but is a map section of its own, and
Bilibin's houses have their own code but are part of Bilibin.

The map table, `gMapHeaders` in [field/map_headers.s](src/field/map_headers.s),
has a row for each of the 201 maps, `struct MapHeader`: the overlay to
load, the map section (`mapSec`), the map type (`mapType`) and the layout. Each row also defines the
map's id, `MapId_<Place>`, as the row's position, so code names a map
by a symbol the linker resolves rather than by a number. The 201 maps
share 96 overlays and 71 map sections. Vale is eight maps in one map
section, and the World Map is a map section of one.

The map type marks towns. A `MAP_TYPE_TOWN` map, a town or one of its
interiors, restores no [PP](glossary.md#psynergy-and-pp), recovers no [Djinn](glossary.md#djinn) and does no poison damage as
the party walks. Retreat, the Psynergy that takes the party back to where
it entered the map section, is blocked there, and Avoid, which keeps away
the battles of zones the party has outgrown, doesn't wear off.

| Name | Value | Meaning |
| --- | --- | --- |
| maps | 201 | rows of `gMapHeaders` |
| overlays | 96 | the maps' code, some shared by several maps |
| map sections | 71 | groups of maps: their music, area name and kept flags |
| `MAP_TYPE_FIELD` | 1 | every other map: dungeons, the world map |
| `MAP_TYPE_TOWN` | 2 | a town or one of its interiors |

## Warps and warp destinations

Each warp out of a map leads to another map and a warp destination there,
and the warp destination decides where the party appears. One warp can lead
to different maps at different points in the story.

[map_warps.h](include/map_warps.h) names every way between maps. Each
map's warp destinations are named by where the party arrives from
(`VALE_GATE_FROM_SANCTUM`) and its warps by where they lead
(`VALE_GATE_TO_WORLD_MAP`), and a door is named the same from both sides.

A warp row, `MAP_WARP(warp, map, warpDest)`, sends one warp to a map and a
warp destination. `Map_TakeWarp` looks up the current map's rows in the
overlay's warp table and takes the first one for the warp taken; a row
written `MAP_WARP_IF` is taken only while its flag is clear, and the search
otherwise goes on to the next row. These are the three rows of the way down
from Sol Sanctum's entrance
([sol_sanctum_entrance.c](src/map/sol_sanctum_entrance.c)):

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

So the same path leads to Vale until the sanctum erupts, to Vale erupting
until the party leaves Vale, and to Vale again for the rest of the game.

A map's initialization often tests the warp destination as well, to play the part of a sequence that
the previous map sent the party to.

## Game flags

A map keeps nothing of its own between visits: what the game must remember,
from a chest someone opened to the end of a chapter, is a game flag, saved
with the game. Two ranges of flags are set aside for maps to use and are
cleared as the party moves on.

The game flags are 4,096 bits in `gGameFlagBytes`, saved with the game and
named in [flags.h](include/flags.h): the party and the Djinn first, then the
system flags the engine shares, then the story flags, and most chest and
find flags last, each map section's in a section of its own.

The two ranges cleared for the map code mean a map never has to undo
what it changed. `Map_ResetFlagsOnEnter` clears them on every arrival
without `FLAG_KEEP_PARTY_POSITION`, which a return from battle sets:

- the map's own flags (`FLAG_MAP_TEMP_FIRST` to `FLAG_MAP_TEMP_LAST`) on
  every such arrival, so a pillar the party pushed, kept in a
  `FLAG_MAP_TEMP_xx` flag, is back in place the next time it comes in;
- the map section's flags (`FLAG_MAPSEC_TEMP_FIRST` to
  `FLAG_MAPSEC_TEMP_LAST`) only when the party enters another map section,
  so they hold what should last while the party is anywhere inside it.

Keeping both ranges is what lets a map come back from a battle unchanged.
Entering a map section also sets its `FLAG_MAPSEC_VISITED` flag, which puts
the map section's marker on the world map screen. Eight flags starting at a
multiple of 8 can also be read and written as a byte, which is how counters
live among the flags.

A map reads the story flags in three places. Its entry functions choose
which table to return; its map event, object event template and warp rows
each carry a condition, and `Map_FindEvent` runs the first map event row
whose kind, [trigger](glossary.md#trigger) and condition fit; and its initialization restores what
the flags say has already happened. Because rows and flags interact this
way, some code can never run, and its comment opens with [`Unused:`](glossary.md#unused) and says
why. [Unused](unused.md) collects every such case.

| Name | Value | Meaning |
| --- | --- | --- |
| `FLAG_DJINN_FIRST` | 48 | Flint's flag, the first Djinn flag; `FLAG_DJINNI(djinni)` is this plus its `DjinniId` |
| `FLAG_KEEP_PARTY_POSITION` | 265 (`0x109`) | keeps both cleared ranges, and the party where it was |
| `FLAG_KEEP_MUSIC` | 282 (`0x11a`) | keeps the music for one map change |
| `FLAG_KEEP_MUSIC_HELD` | 283 (`0x11b`) | keeps the music for as long as it is set |
| `FLAG_MAPSEC_VISITED(group)` | 384 (`0x180`) + group | the map section's marker on the world map screen |
| `FLAG_MAP_TEMP_FIRST` to `FLAG_MAP_TEMP_LAST` | 512 to 767 (`0x200` to `0x2ff`) | the map's 256 flags, cleared on every arrival without `FLAG_KEEP_PARTY_POSITION` |
| `FLAG_MAPSEC_TEMP_FIRST` to `FLAG_MAPSEC_TEMP_LAST` | 768 to 1,023 (`0x300` to `0x3ff`) | the map section's 256 flags, cleared on entering another map section |

[Game rules](src/game/README.md#the-game-flags) gives the layout and the
calls.

## How maps depend on the story

The same town looks different at different points in the story: other people
stand in it and say other things. A map does this by asking the flags
which version of itself to be.

The story is the order the flags are set in, and these maps show the forms
that dependence takes, in [story order](glossary.md#the-places-in-story-order).

- **One map per point in the story.** The prologue's stormy night in Vale
  is a map of its own, [vale_storm.c](src/map/vale_storm.c), on its own
  layout, and it sets `FLAG_VALE_STORM` when it starts. A place that changes
  completely gets a map; a place whose people change reads the flags.
- **One map that reads the flags.** Vale's town gate,
  [vale_gate.c](src/map/vale_gate.c), is one map for the whole game.
  `ValeGate_GetEvents` asks five questions in order:

  ```c
  const struct MapEvent *ValeGate_GetEvents(void) {
    if (GameFlag_Test(FLAG_KALAY_REACHED) != 0) {
      return gValeGate_EventsAfterKalay;
    }
    if (GameFlag_Test(FLAG_VALE_PARTY_LEFT) != 0) {
      return gValeGate_EventsAfterLeaving;
    }
    if (gGameState.warpDest == VALE_GATE_FROM_SANCTUM) {
      return gValeGate_DepartureEvents;
    }
    if (GameFlag_Test(FLAG_VALE_STORM) != 0) {
      return gValeGate_StormEvents;
    }
    return gValeGate_Events;
  }
  ```

  Its warps
  and object event templates are chosen the same way. The storm ends here:
  the battle against the two strangers returns to
  `VALE_GATE_AFTER_TRAVELERS_BATTLE`, which clears `FLAG_VALE_STORM` and
  leaves by `VALE_GATE_TO_TITLE`.
- **A map with no map events.** [vale_eruption.c](src/map/vale_eruption.c),
  the ground before the sanctum while Mt. Aleph erupts, plays its whole
  sequence from its initialization on arrival and leaves by its one warp.
- **A row that lasts until its flag is set.** In the Elemental Star room,
  [sol_sanctum_elemental_star_room.c](src/map/sol_sanctum_elemental_star_room.c),
  each pedestal's [bg event](glossary.md#bg-event) row lasts until that star's flag is set, so each
  star is taken once.
- **A flag set by a warp destination.** The World Map's initialization sets
  `FLAG_VALE_PARTY_LEFT` the first time the party arrives by
  `WORLD_MAP_FROM_VALE_GATE`, and the gate's tables change from then on.
- **One sequence cut into parts by warp destinations.**
  [venus_lighthouse_aerie.c](src/map/venus_lighthouse_aerie.c), the
  Lighthouse Aerie, is one scripted sequence interrupted by two battles and
  two trips to the World Map. Each part starts in its initialization from
  the warp destination the part before sent the party to; arriving at
  `WORLD_MAP_AFTER_VENUS_LIGHTHOUSE_LIT`, `WorldMap_Initialize` plays the
  lighthouse transition and sends the party straight back to
  `VENUS_LIGHTHOUSE_AERIE_AFTER_BEACON_LIT`. Mercury Lighthouse uses the same
  transition from `WORLD_MAP_AFTER_MERCURY_LIGHTHOUSE_LIT`.
- **The last map.** [lemurian_ship.c](src/map/lemurian_ship.c), at the
  edge of Lalivero, ends the game by leaving for the title screen by
  `TITLE_FROM_LEMURIAN_SHIP` with `FLAG_KEEP_MUSIC` set.

[Places](src/field/map_headers.s) lists every map in story order with what
happens there.

## The world map

The World Map, [world_map.c](src/map/world_map.c), is a map like the
others but drawn in perspective, on the two background layers the hardware
can scale and rotate. The world is stored as a grid of squares, 512 pixels
on a side, and only the 2 by 2 squares around the party are unpacked at a
time; `WorldMap_UpdateCurrentTileBlock` unpacks them into both layers. The
picture below was made by moving the party's position across the whole map
and laying each square it unpacked out flat, one pixel for each pixel of the
map, so it also shows the squares the map's data leaves blank.

![The whole world map of The Broken Seal laid out flat: the land of mountains, rivers and forest that the story crosses, two small islands in the sea, and plain blocks where the map's data leaves squares blank](https://alchemy.pascalpixel.com/story/world.png)

Its code does three kinds of work. Each place is an object event on the
map, and touching one raises the trigger that the map event table
turns into that place's warp. It holds what the party meets on the map
itself, three Djinn and a storm. And its initialization plays the story's
moments between places, chosen by the warp destination the last map sent
the party to. [World](world.md#the-world-map) explains how the tilt is drawn.

## How the code is organised

The program on the cartridge is cut into banks, and the source has one
folder for each large bank, in the order the cartridge holds them.

The main image is the program the processor runs from the cartridge.
`main.ld` lays out its code in fourteen banks; together they take about
1 MB of the English cartridge, and the sound code, the music and the game's packed
data follow them. A caller in another bank goes through the bank's
far-call table, by the function's name with `Far` added: `Game_Run`, in the
`overworld` bank, starts a battle with `Battle_RunFar`, which leads to
`Battle_Run` in the `battle` bank. The [glossary](glossary.md#far-call)
says how a [far call](glossary.md#far-call) works, and
[System](src/system/README.md#why-the-program-is-cut-into-banks) why the
program is cut into banks.

The source's folders follow the cartridge:

- The nine large banks each have a folder, in the order the cartridge holds
  them: `runtime`, `field`, `window`, `game`, `overworld`, `menu`, `shop`,
  `battle` and `battle_anim`. A folder takes its bank's name: `overworld`
  runs a map from arrival to warp, while `field` moves and draws the objects.
- The five small banks' code sits in the folder it belongs with.
- `graphics`, `layout`, `sound` and `system` are grouped by topic, and their
  files are linked into whichever bank or image needs them.
- `map` and `debug` hold overlays.

[`objects.tsv`](objects.tsv) lists every object, each file the build
compiles or assembles, in link order with the image and bank it goes in, and
[Source](src/README.md) goes through every folder.

## Section pages

- [World](world.md): the field engine, the layouts, the request loop a map
  runs and field Psynergy.
- [Battle](battle.md): how a battle runs from the first menu to the spoils,
  how an ability's damage and healing are worked out, and how its animations
  are drawn.
- [Menus](menus.md): the field menu and its screens, the shops and inns, and
  the windows text is drawn in.
- [Engine](engine.md): start-up, frame timing and tasks, banks and far
  calls, overlays and songs.
- [Places](src/field/map_headers.s): every map in story order, with its
  object event templates and map events.
- [Game rules](src/game/README.md): stats, items, Djinn, classes, levels and
  the flags.
- [Unused](unused.md): the maps, scripts and lines the game keeps but never
  plays, and why each never runs.
- [Source](src/README.md): every file of the code, with its comments.
- [Glossary](glossary.md): the words every page uses, from Psynergy and
  Djinn to banks, overlays and bg events.
