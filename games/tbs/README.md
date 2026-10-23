# ☀️ The Broken Seal

![The game runs one scene at a time, in a loop that goes round once for each scene, not once a frame. At power-on the first scene is the title. Each time round, the game first drops everything the last scene left behind. Then it runs the next scene. A place, such as a town, a dungeon or the world map, has its code loaded and runs until the party takes an exit, and the exit names the next scene. A battle or a minigame runs and then sends the party back, most often to where it stood before. Then the loop goes round again. A random battle is a scene like any other: the place ends, and is loaded again after.](https://alchemy.pascalpixel.com/illustrations/front/main-loop.svg)

The Broken Seal plays one scene at a time: a room, a town, a dungeon floor,
the world map, a battle. When the party walks through a door, the game ends
one scene and starts the next. Each place has code of its own, loaded only
while the party is there, and a map. Everything a place knows about how far
the story has gone comes from the game flags, a long list of yes-or-no facts
saved with the game. The program itself is cut into banks, and the source has
one folder for each large bank.

## The main loop

The game runs one scene, then the next, for as long as it is on. This loop
goes round once for each scene, not once a frame: a scene can last for
minutes, and while it runs, its own code waits frame by frame and the
engine keeps the screen moving, as [Engine](engine.md#waiting-and-tasks)
explains.

Each time round, the game first clears away everything the last scene left
behind, so nothing one scene set up can leak into the next. Then it runs the
next scene. A place runs until the party takes an exit, and the exit says
which scene comes next and where the party arrives. A battle or a minigame is
a scene too: it runs, and then the game picks the scene to go back to, most
often the place the battle started from, with everyone where they stood.

`Game_Run`, in [object/scene.c](src/object/object/map.c), is this loop of
scenes, and it never returns. `System_Initialize` calls it at power-on
with `GAME_RUN_TITLE`, which makes the title (`MapId_Title`) the first
scene; the debug start modes begin at Vale's gate or the clear screen
instead, and only while `gDebugMode` is set. The current scene and the
entrance the party arrives by are `gGameState.scene` and
`gGameState.entrance`.

Each pass of the loop first clears away what the last scene left running:

- DMA channel 0, if a scene left it running. A DMA channel is a part of the
  console that copies memory by itself, and a scene can set one to change a
  display setting during every HBlank, the short pause after each line of
  the screen is drawn, for a wave or a tilt.
- Every task, the functions the engine calls once a frame for a scene (one
  types out text, another scrolls the map).
- The HBlank and VCount handlers, functions the hardware calls after each
  line, or when it reaches one chosen line.
- The heap, the memory from which scenes borrow blocks for their state; it
  is emptied, so every block the last scene held is free again.

Then it does one of two things:

- A field scene has its flags updated (`Map_ResetFlagsOnEnter`), its music
  started unless `FLAG_KEEP_MUSIC` or `FLAG_KEEP_MUSIC_HELD` keeps what is
  playing, and its overlay unpacked (`Map_LoadOverlay`). `Field_RunMap`
  runs it until the party takes an exit and returns the exit's kind, and
  `Map_TakeWarp` turns that into the next scene and entrance.
- A battle or a minigame is a scene number from `MAP_BLANK` up
  (`MAP_BATTLE`, `MAP_LUCKY_DICE`, `MAP_REEL_GAME`), with no row in the
  scene table and no overlay, since its code is in the main image. It returns
  a result rather than an exit, and `Map_ReturnFromBattle` picks the scene
  to go back to: the defeat point after a defeat, the destination an event
  set, or else the field scene the battle started from, with
  `FLAG_KEEP_PARTY_POSITION` set so that the party and the actors come back
  where they were.

A random battle is therefore an exit like any other: the field scene sets
the next scene to `MAP_BATTLE` and leaves, and the loop loads it again
afterwards. [Running a place](src/object/README.md) follows a field scene
from `Field_RunMap` to its exit, including how a battle is chosen from the
encounter zones in `encounter.tsv`.

The tables on this page give each value as an ordinary number, with the
hexadecimal form the source writes it in beside it.

| Name | Value | Meaning |
| --- | --- | --- |
| `GAME_RUN_TITLE` | 0 | start at the title, with a fresh game state |
| `GAME_RUN_DEBUG_VALE` | 1 | in debug mode, start at `MapId_ValeGate` |
| `GAME_RUN_DEBUG_CLEAR` | 2 | in debug mode, start at the clear screen |
| `MAP_BLANK` | 507 (`0x1fb`) | the first scene number with no row in the scene table, whose rows are 0 to 200 |
| `MAP_REEL_GAME` | 508 (`0x1fc`) | Lucky Wheels at Tolbi's spring |
| `MAP_LUCKY_DICE` | 509 (`0x1fd`) | Lucky Dice at Tolbi's spring |
| `MAP_BATTLE` | 510 (`0x1fe`) | a battle |

## Scenes and overlays

Only one place's code is in memory at a time. When the party enters a place,
its code is unpacked from the cartridge over the last place's.

![One place's code in memory at a time. On the cartridge, each place's code is stored packed, 96 pieces in all, such as Vale's, Vault's, Bilibin's and Goma Cave's. In memory there is one area of 32 KB for a place's code. When the party enters Vault, Vault's code is unpacked into that area, over whatever place's code was there before. Beside it the main program is always loaded; the place's code calls it, and the main program asks it about the place.](https://alchemy.pascalpixel.com/illustrations/front/overlay-swap.svg)

The console has little memory, and most of what a place needs (who is there,
what they say, its cutscenes) matters nowhere else. So each place's code is
an overlay: a packed piece of code that `Resource_LoadCode` unpacks into
`gOverlayArea`, 32 KB of EWRAM (the console's larger, slower work memory,
256 KB in all), when the scene loads, over the one before.

The field engine knows a scene only through six entry points, short jumps
at the start of its overlay (`struct MapEntries`): its initialization, and the functions
that return its entrances, exits, placements, events and exit regions. They
are functions rather than pointers to tables so that a scene can return a
different table as the story moves on. Each overlay also ends in an import
table of the main program's functions it calls, each reached through the
jump table at the start of the bank that holds it ([How the code is
organised](#how-the-code-is-organised) explains banks); overlays never call
each other, and code several scenes need is
compiled into each of them from `scene/common/`.
[Scenes](src/map/README.md) describes the tables and how to find an event,
and [Engine](engine.md#overlays) the overlay's two tables.

The other half of a scene is its map: three background layers with the
actors drawn between them, and a grid of cells under them that says how high
the ground is, where it can be walked and which cells raise events.
[World](world.md) explains both.

## The scene table and places

One table lists every scene the game has, and for each says which code it
runs, which place it belongs to and which map it shows.

![Each scene's code and place. Four scenes and what the scene table gives each. Bilibin's town, its treasury and the Bilibin Barricade all run on Bilibin's code; Bilibin's houses have code of their own. Bilibin's town, houses and treasury are one place, Bilibin, which chooses their music, the area name shown on arrival and the flags kept while the party is inside. The Bilibin Barricade shares Bilibin's code but is a place of its own. Which code a scene runs and which place it belongs to are chosen separately.](https://alchemy.pascalpixel.com/illustrations/front/scene-parts.svg)

A scene's code says what happens there, and its place, a group of scenes,
chooses the music, the area name shown on arrival and the flags kept while
the party stays inside. The two are chosen separately: the Bilibin
Barricade runs on Bilibin's code but is a place of its own, and Bilibin's
houses have their own code but are part of Bilibin.

The scene table, `gField_MapTable` in [field/maps.s](src/field/maps.s),
has a row for each of the 201 scenes, `struct MapRecord`: the overlay's
resource, the group, the variant and the map. Each row also defines the
scene's id, `MapId_<Place>`, as the row's position, so code names a scene
by a symbol the linker resolves rather than by a number. The 201 scenes
share 96 overlays and 71 groups. Vale is eight scenes in one group, and the
World Map is a group of one.

The variant marks towns. A `MAP_TYPE_TOWN` scene, a town or one of its
interiors, restores no PP, recovers no Djinn and does no poison damage as
the party walks, Retreat is blocked there and Avoid doesn't wear off.

| Name | Value | Meaning |
| --- | --- | --- |
| scenes | 201 | rows of `gField_MapTable` |
| overlays | 96 | the scenes' code, some shared by several scenes |
| groups | 71 | places: their music, area name and kept flags |
| `MAP_TYPE_FIELD` | 1 | every other scene: dungeons, the world map |
| `MAP_TYPE_TOWN` | 2 | a town or one of its interiors |

## Entrances and exits

Each way out of a scene leads to another scene and an entrance there, and the
entrance decides where the party appears. One way out can lead to different
places at different points in the story.

![One way out, three destinations. The way down from Sol Sanctum's entrance has three rows in the exit table, read from the top. The first leads to Vale, and is taken until the sanctum erupts. The second leads to Vale during the eruption, and is taken until the party leaves Vale. The third leads to Vale, always. The first row whose condition still holds is taken, so the same path leads to Vale, then to Vale erupting, then to Vale again for the rest of the game.](https://alchemy.pascalpixel.com/illustrations/front/exits.svg)

[scene_links.h](include/map_warps.h) names every way between scenes. Each
scene's entrances are named by where the party arrives from
(`VALE_GATE_FROM_SANCTUM`) and its exits by where they lead
(`VALE_GATE_TO_WORLD_MAP`), and a door is named the same from both sides.

An exit row, `MAP_WARP(exit, scene, entrance)`, maps an exit kind to a
scene and an entrance. `Map_TakeWarp` looks up the current scene's rows in
the overlay's exit table and takes the first one whose kind matches; a row
written `MAP_WARP_IF` is taken only while its flag is clear, and the
search otherwise goes on to the next row. Sol Sanctum's entrance
(`gSolSanctumEntrance_Warps`) has three rows for `SOL_ENTRANCE_TO_VALE`,
with `FLAG_SOL_SANCTUM_ERUPTED`, then `FLAG_VALE_PARTY_LEFT`, then no flag.
The entrance then decides where the party appears, and a scene's
initialization often tests it as well, to play the part of a sequence that
the previous scene sent the party to.

## Game flags

The game flags are the long list of yes-or-no facts saved with the game:
which Djinn the party has, which chests are open, how far the story has
gone. Two ranges of them are scratch space for scenes, forgotten as the
party moves on.

![What a place forgets, and when. Scene flags are forgotten every time the party enters a scene, even the one it just left: the plant Growth raised on Crossbone Isle is a sprout again. Place flags are kept while the party moves between the scenes of one place and forgotten when it enters another place: Vault's mayor remembers having welcomed Isaac back while the party stays in Vault, and welcomes him again on the next visit. Coming back from a battle forgets neither, so the room is as the party left it.](https://alchemy.pascalpixel.com/illustrations/scene/flag-reset.svg)

The game flags are 4,096 bits in `gGameFlagBytes`, saved with the game and
named in [flags.h](include/flags.h): the party and the Djinn first, then the
system flags the engine shares, then the story flags, and most chest and
find flags last, each place's in a section of its own. Eight flags starting
at a multiple of 8 can also be read and written as a byte, which is how
counters live among the flags.

The two ranges cleared for the scene code mean a scene never has to undo
what it changed. `Map_ResetFlagsOnEnter` clears the scene's own flags
(`FLAG_MAP_TEMP_FIRST` to `FLAG_MAP_TEMP_LAST`) on every ordinary entrance, and the
place's flags (`FLAG_MAPSEC_TEMP_FIRST` to `FLAG_MAPSEC_TEMP_LAST`) when the party enters
another group. `FLAG_KEEP_PARTY_POSITION` keeps both ranges, which is what
lets a scene come back from a battle unchanged. Entering a group also sets
its `FLAG_MAPSEC_VISITED` flag, which puts the place's marker on the world
map screen.

A scene reads the story flags in three places. Its entry functions choose
which table to return; its event, placement and exit rows each carry a
condition, and `Map_FindEvent` runs the first event row whose kind,
trigger and condition fit; and its initialization restores what the flags
say has already happened. Because rows and flags interact this way, some
code can never run, and its comment opens with `Unused:` and says why.
[Unused](unused.md) collects every such case.

| Name | Value | Meaning |
| --- | --- | --- |
| `FLAG_DJINNI_0` | 48 | Flint's flag; each Djinni's is this plus its `DjinniId` |
| `FLAG_KEEP_PARTY_POSITION` | 265 (`0x109`) | keeps both cleared ranges, and the party where it was |
| `FLAG_KEEP_MUSIC` | 282 (`0x11a`) | keeps the music for one scene change |
| `FLAG_KEEP_MUSIC_HELD` | 283 (`0x11b`) | keeps the music for as long as it is set |
| `FLAG_MAPSEC_VISITED(group)` | 384 (`0x180`) + group | the place's marker on the world map screen |
| `FLAG_MAP_TEMP_FIRST` to `FLAG_MAP_TEMP_LAST` | 512 to 767 (`0x200` to `0x2ff`) | the scene's 256 flags, cleared on every ordinary entrance |
| `FLAG_MAPSEC_TEMP_FIRST` to `FLAG_MAPSEC_TEMP_LAST` | 768 to 1,023 (`0x300` to `0x3ff`) | the place's 256 flags, cleared on entering another group |

[Game rules](src/game/README.md#the-game-flags) gives the layout and the
calls.

## How scenes depend on the story

The same town looks different at different points in the story: other people
stand in it and say other things. A scene does this by asking the flags
which version of itself to be.

![One scene, five moments in the story. Vale's town gate is one scene for the whole game, and each time the party arrives it asks the story which set of events to use, testing in order. Has the party reached Kalay? Then the events after Kalay. Has the party left Vale? Then the events after leaving. Has the party come from the Sanctum? Then the farewell. Is it the stormy night? Then the storm. Otherwise, the events of the day the story begins.](https://alchemy.pascalpixel.com/illustrations/front/vale-gate.svg)

The story is the order the flags are set in, and these scenes show the forms
that dependence takes, in story order.

- **One scene per point in the story.** The prologue's stormy night in Vale
  is a scene of its own, [vale_storm.c](src/map/vale_storm.c), on its own
  map, and it sets `FLAG_VALE_STORM` when it starts. A place that changes
  completely gets a scene; a place whose people change reads the flags.
- **One scene that reads the flags.** Vale's town gate,
  [vale_gate.c](src/map/vale_gate.c), is one scene for the whole game.
  `ValeGate_GetEvents` returns one of five event tables, tested in order:
  after `FLAG_KALAY_REACHED`, after `FLAG_VALE_PARTY_LEFT`, the farewell
  when the party arrives by `VALE_GATE_FROM_SANCTUM`, the storm while
  `FLAG_VALE_STORM` is set, and otherwise the day the story begins. Its exits
  and placements are chosen the same way. The storm ends here: the battle
  against the two strangers returns to `VALE_GATE_AFTER_TRAVELERS_BATTLE`,
  which clears `FLAG_VALE_STORM` and leaves by `VALE_GATE_TO_TITLE`.
- **A scene with no events.** [vale_eruption.c](src/map/vale_eruption.c),
  the ground before the sanctum while Mt. Aleph erupts, plays its whole
  sequence from its initialization on arrival and leaves by its one exit.
- **A row that lasts until its flag is set.** In the Elemental Star room,
  [sol_sanctum_elemental_star_room.c](src/map/sol_sanctum_elemental_star_room.c),
  each pedestal's search row lasts until that star's flag is set, so each
  star is taken once.
- **A flag set by an entrance.** The World Map's initialization sets
  `FLAG_VALE_PARTY_LEFT` the first time the party arrives by
  `WORLD_MAP_FROM_VALE_GATE`, and the gate's tables change from then on.
- **One sequence cut into parts by entrances.**
  [venus_lighthouse_aerie.c](src/map/venus_lighthouse_aerie.c), the
  Lighthouse Aerie, is one scripted sequence interrupted by two battles and
  two trips to the World Map. Each part starts in its initialization from
  the entrance the part before sent the party to; arriving at
  `WORLD_MAP_AFTER_VENUS_LIGHTHOUSE_LIT`, `WorldMap_Initialize` plays the
  lighthouse transition and sends the party straight back to
  `VENUS_LIGHTHOUSE_AERIE_AFTER_BEACON_LIT`. Mercury Lighthouse uses the same
  transition from `WORLD_MAP_AFTER_MERCURY_LIGHTHOUSE_LIT`.
- **The last scene.** [lemurian_ship.c](src/map/lemurian_ship.c), at the
  edge of Lalivero, ends the game by leaving for the title scene by
  `TITLE_FROM_LEMURIAN_SHIP` with `FLAG_KEEP_MUSIC` set.

[Places](src/field/maps.s) lists every scene in story order with what
happens there.

## The world map

The World Map, [world_map.c](src/map/world_map.c), is a scene like the
others but drawn in perspective, on the two background layers the hardware
can scale and rotate. The world is stored as a grid of squares, 512 pixels
on a side, and only the 2 by 2 squares around the party are unpacked at a
time; `WorldMap_UpdateCurrentTileBlock` unpacks them into both layers. The
picture below was made by moving that point across the whole map and laying
each square out flat, one pixel for each pixel of the
map, so it also shows the squares the map's data leaves blank.

![The whole world map of The Broken Seal, laid out flat](https://alchemy.pascalpixel.com/story/world.png)

Its code does three kinds of work. Each place is an actor on the map, and
touching one raises the trigger that the event table turns into that place's
exit. It holds what the party meets on the map itself, three Djinn and a
storm. And its initialization plays the story's moments between places,
chosen by the entrance the last scene sent the party to.
[World](world.md#the-world-map) explains how the tilt is drawn.

## How the code is organised

The program on the cartridge is cut into banks, and the source has one
folder for each large bank, in the order the cartridge holds them.

![The main program, bank by bank. The main program's code as it lies on the English cartridge, about 1 MB, drawn to scale from start to end and cut into fourteen banks. Each large bank has a folder of the source: runtime (start-up, frames and memory), field (moving and drawing), window (windows, fonts and all the game's text, the largest), game (the rules: stats, items), object (running a place), menu, shop, battle, and effect (battle animations). Five small banks follow, the last for sound. Each bank opens with a table of jumps, and a call from another bank, a far call, goes to a jump in that table, which leads on to the function: the object bank starts a battle by calling the battle bank's table.](https://alchemy.pascalpixel.com/illustrations/front/banks.svg)

The main image is the program the processor runs from the cartridge.
`main.ld` lays out its code in fourteen banks, each starting at an address
fixed per edition; together they take about 1 MB of the English cartridge,
and the sound code, the music and the game's packed data follow them. A
call from one bank into another is a far call. Each bank begins with a
far-call table, one 8-byte jump for each of its functions that another bank
calls, and a caller in another bank calls that jump, written as the
function's name with `Far` added: `Game_Run`, in the `object` bank, starts
a battle with `Battle_RunEncounterFar`, the jump at the start of the
`battle` bank that leads to `Battle_RunEncounter`. The nine large banks each
have a folder, in the order the cartridge holds them: `runtime`, `field`, `window`, `game`,
`object`, `menu`, `shop`, `battle` and `effect`.
[System](src/system/README.md#why-the-program-is-cut-into-banks) explains
why the program is cut into banks. The five small banks' code sits in the
folder it belongs with. `graphics`, `map`, `sound` and `system` gather one
kind of thing wherever it is linked, and `scene` and `debug` hold overlays.
[`objects.tsv`](objects.tsv) lists every object in link order with the image
and bank it goes in, and [Source](src/README.md) goes through every folder.

## Section pages

- [World](world.md): the field engine, the maps, the request loop a scene
  runs and field Psynergy.
- [Battle](battle.md): how a battle runs from the first menu to the spoils,
  how an ability's damage and healing are worked out, and how its effects
  are drawn.
- [Menus](menus.md): the field menu and its screens, the shops and inns, and
  the windows text is drawn in.
- [Engine](engine.md): start-up, frame timing and tasks, banks and far
  calls, overlays and sound cues.
- [Places](src/field/maps.s): every scene in story order, with its
  placements and events.
- [Game rules](src/game/README.md): stats, items, Djinn, classes, levels and
  the flags.
- [Unused](unused.md): the scenes, events and lines the game keeps but never
  plays, and why each never runs.
- [Source](src/README.md): every file of the code, with its comments.
