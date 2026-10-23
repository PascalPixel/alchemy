# Maps

![Six questions a map answers. The field engine knows a map only through six questions it asks the map's own code. When the party arrives it asks, in order: what happens here (the map events, kept for the whole visit), where the party arrives (the warp destinations), who stands where (the object event templates), and whether anything needs setting up first (the initialization, which can play an arrival cutscene). At every step it asks whether the leader stands in a warp area. As the party leaves it asks where the warp leads. Each answer comes from a function, so a map can answer differently as the story goes on.](https://alchemy.pascalpixel.com/illustrations/scene/entries.svg)

Everything that makes a [map](../../glossary.md#map) itself is in its own code here: who stands
where, what each person says, what happens when the [party](../../glossary.md#the-party) steps on a
doorstep, searches a jar, or uses an item or [Psynergy](../../glossary.md#psynergy-and-pp), and the cutscenes the
map plays. That code is a map [overlay](../../glossary.md#overlay), loaded when the party arrives and
replaced when it leaves. [Running a map](../object/README.md) runs the map
and asks the overlay what to do; the overlay drives the engine back through
its [import table](../../glossary.md#entry-table-and-import-table). Each overlay has a generated page of its own, listed in
[Maps](../field/maps.s) in [story order](../../glossary.md#the-places-in-story-order), with its [layout](../../glossary.md#layout) and its [map events](../../glossary.md#map-event).

## The entry table

The engine knows nothing about any one map. It asks the map's code six
questions, always the same ones, and the answers are the map.

When the party arrives, the engine asks what can happen here (the map
events, kept for the whole visit), where the party arrives (the [warp
destinations](../../glossary.md#warp-destination)), who stands where (the [object event](../../glossary.md#object-event) templates), and whether
anything needs setting up (the initialization). At every step it asks
whether the leader is standing in a [warp](../../glossary.md#warp) area, and as the party leaves,
where the warp leads. Each answer comes from a function rather than a fixed
table, so a map can answer differently as the story goes on
([World](../../world.md#map-entries) gives Vault's example), and an overlay
that serves several maps can answer for each.

The initialization restores what the flags say has happened, starts
callbacks, or plays an arrival cutscene; it may take a warp at once. It
skips arrival cutscenes when `FLAG_KEEP_PARTY_POSITION` says the party is
returning to where it stood, as after a battle.

In the code, the six are `struct MapEntries` in `field_runtime.h`, in a
fixed order at the start of every overlay, `entry/<map>.s`: 48 bytes, each
entry a [veneer](../../glossary.md#veneer), a tiny piece of code that only jumps on: one word holding
two instructions (load the address that follows, jump to it), then the
address of the function. `Field_RunMap` calls
`Map_GetEvents`, then places the party at its warp destination
(`Map_ApplyWarpDest`), creates the object events from the object event
templates and calls `Map_Initialize`. `Field_ProcessStep` reads the regions
at every step, and `Map_TakeWarp` reads the warps as the party leaves. An
overlay that serves several maps chooses by `gGameState.mapId`, as Babi Lighthouse's
`Map_GetWarpRegions` does. Some overlays give the entries names of their own
(`BabiLighthouse_GetWarps`); only the order is fixed.

| Entry, as Vault names it | Offset in `struct MapEntries` | Returns or does |
| --- | --- | --- |
| `Map_Initialize` | `0x04` | the map's setup, run once its object events exist |
| `Map_GetWarpDests` | `0x0c` | `struct MapWarpDest` rows |
| `Map_GetWarps` | `0x14` | the warp table |
| `Map_GetObjectEventTemplates` | `0x1c` | `struct ObjectEventTemplate` rows |
| `Map_GetEvents` | `0x24` | `struct MapEvent` rows |
| `Map_GetWarpRegions` | `0x2c` | `struct MapWarpRegion` boxes, or `NULL` |

## The tables

The answers are tables: where the party can arrive and leave, who stands
where, and what answers each thing the player does. A position in them is
in pixels (x east, y up, z south) and a facing one of eight headings
(`enum Facing`); [Words used here](#words-used-here) gives the units.

- **Warp destinations** (`struct MapWarpDest`): for each warp destination
  number, a flag that must be set (or -1), a position, a facing and the
  camera's limits.
- **Warps**: a header word per map (`MAP_WARPS(Map)`), then rows of
  `MAP_WARP(warp, map, warpDest)` mapping a warp kind to a map and warp
  destination named in `map_warps.h`, ended by `MAP_WARPS_END`.
  `MAP_WARP_IF` adds a flag, and the row is taken only while that flag is
  clear. `WARP_TO_BATTLE` leads into a battle.
- **Object event templates** (`struct ObjectEventTemplate`): a [sprite](../../glossary.md#sprite-and-oam), a
  condition, a behaviour (an `enum MovementType` such as `MOVEMENT_TYPE_STAND`
  or `MOVEMENT_TYPE_WANDER`, or an object [script](../../glossary.md#script)), a position, a facing and
  how the object event turns when spoken to.
- **Regions** (`struct MapWarpRegion`): boxes in which the leader takes a warp
  kind, each with a condition. Most maps have none.
- **Map events** (`struct MapEvent`): a control word (the kind and its
  detail), a [trigger](../../glossary.md#trigger), a condition and a value, ended by `MAP_EVENTS_END`.

### Who stands where

Each row of a map's object event templates puts one person or thing on the
map, an object event, and each row owns an object event slot of its own, a
numbered place for it in the map's state, whether or not it appears.

![Every object event template owns a slot. The blue statue as the game shows it, on its ledge under a jet of water and fallen to the floor below, then the object event templates of Mercury Lighthouse's upper floor, row by row, and the object event slot each row fills before and after the blue statue falls from its ledge. The party's eight slots come first. The first row, the statue fallen to the floor below, appears only once it has fallen; the second, the statue on its ledge, only until then. The rows after them, two pipe sections, three pipe ends and six stone statues, always appear. A row whose condition fails leaves its slot empty rather than handing it to the next row, so every object event keeps the same slot whatever the story has done, and the pipes are always the third and fourth placed object events.](https://alchemy.pascalpixel.com/illustrations/scene/placements.svg)

A row can carry a condition, a [story flag](../../glossary.md#game-flag) that must be set or clear, and a
row whose condition fails puts nobody on the map. Its slot stays empty
rather than going to the next row, so every object event keeps one [local id](../../glossary.md#local-id)
all game and the map's code can name it once: on Mercury Lighthouse's upper floor
the blue statue has one row for its place on the ledge and another for where
it lands after it falls, and only one of the two appears.

The party takes slots 0 to 7, the leader the one the player moves
(`LOCALID_PARTY_LEADER`), and the table's rows take the slots from
`LOCALID_FIRST_PLACED` in order: local id `LOCALID_FIRST_PLACED + n` is always row n
(`FieldScript_SpawnObjectTable`). A row whose sprite is one of the party's
characters (sprites 0 to 7) takes that character's own slot instead. A
condition of 0 is no flag: `Map_AssignViewFlags` replaces it with whether
the object event lies within the view. Each map names its object events in
an enum in row order (`LOCALID_FALLEN_STATUE = LOCALID_FIRST_PLACED`).

### Which map events a cell starts

Some spots on a map do something when the leader reaches them: a doorstep
leads into a house when the leader walks into it, the edge of a village
leads on to the next map, a ladder climbs to a roof, and a chest or a
bookcase answers the A button. The layout marks each spot by the [metatile
behavior](../../glossary.md#metatile-behavior) of its [cell](../../glossary.md#cell), and the map's event table says what that behavior
does. The same behavior means different things on different maps.

On the screen of Vale below, on the day the story begins, each door is a
[coord event](../../glossary.md#coord-event) rather than a warp: walking north into the doorstep runs a
script, `EventScript_EnterIsaacsHouse` for Isaac's house, that opens the door,
walks the leader in and only then asks for the warp, and that can refuse
instead, as Dora does once the party has left Vale. Coord events also climb
the ladder beside Isaac's house (`EventScript_ClimbLadderOntoRoof` up,
`EventScript_ClimbDownFromRoof` down) and put the upper path's cells
back when the leader steps off it (`EventScript_ClearPathCellOverride`). Three
cells of the path at the top carry a behavior no row answers.

![Which cells start map events in Vale. A screen of Vale on the day the story begins, drawn from its layout, with each cell that carries one of the map's own metatile behaviors tinted and numbered. In pink, cells a coord event answers: 1, a door into a house, walking north into it; 2, the door into Isaac's house, walking north; 3, the ladder up onto the roof of Isaac's house, walking north; 4, the ladder down from the roof, walking south; 5, the cell where stepping off the upper path changes its cells back. In grey, 6: three cells of the path at the top with a behavior of the map's own that no row answers.](https://alchemy.pascalpixel.com/illustrations/scene/event-cells.svg)

Stepping on or walking into a cell runs the first coord event whose trigger
is its metatile behavior and whose condition holds, or failing one the first
such warp (`Map_RunCoordEventOrWarp`); pressing A facing it runs the first
such [bg event](../../glossary.md#bg-event). A behavior with no row does nothing. The map's own behaviors
are `MB_EVENT_FIRST` to `MB_EVENT_LAST`; the behaviors above them mean
the same on every map ([Layouts](../layout/README.md#cells)).

### What a map event row says

A map event's kind says what starts it and what its trigger is:

| Kind | Started by | Trigger |
| --- | --- | --- |
| `MAP_EVENT_TALK` | the A button on an object event | the object event's local id |
| `MAP_EVENT_TALK_HALTED` | talking to an object event Halt has stopped | the object event's local id |
| `MAP_EVENT_COORD`, `COORD_EVENT_FACING(f)`, `COORD_EVENT_WALKING_INTO(f)` | stepping onto a cell, facing a way, or pushing against it | the cell's metatile behavior |
| `MAP_EVENT_WARP` | stepping onto a cell no coord event answers | the cell's metatile behavior |
| `BG_EVENT(target)` | the A button on what is ahead, when it is no object event | the cell's metatile behavior |
| `USE_ITEM(item)`, `USE_ITEM_ON_OBJ_EVENT(item)` | using an item | the cell's metatile behavior or the object event's local id |
| `PSYNERGY_ON_OBJ_EVENT(ability)`, `PSYNERGY_IN_AREA(ability, phase)` | using Psynergy on an object event, or anywhere | the object event's local id, or 0 |
| `MAP_EVENT_RAISED` | the map's code raising a value | the value |

A Psynergy event's phase (`enum PsynergyPhase`) says when it runs around the
effect: `PSYNERGY_PHASE_BEFORE_CAST` before it plays,
`PSYNERGY_PHASE_AFTER_CAST` after, and `PSYNERGY_PHASE_EFFECT_ACTS` from inside
the effect, as Force strikes, Frost freezes or Reveal shows what is hidden.
Reveal's maps pair that row with a `PSYNERGY_PHASE_EFFECT_ENDS` one that
hides it all again as the colours come back, and `PSYNERGY_PHASE_AFTER_END`
runs when a Psynergy that runs out by itself, such as Cloak, has ended.
[Running a map](../object/README.md#how-field-psynergy-reaches-its-effect)
follows a cast through these and lists the phases' values.

A map event's value is a message (`MSGNO(name)`), a function
(`EVENT_SCRIPT(function)`), the warp kind for a warp, or for a bg event what
is found: `FIND_ITEM`, `FIND_COINS`, `FIND_PSYNERGY_STONE`, `FIND_MESSAGE` or
`FIND_MIMIC`. Functions live above `0x10000` and messages below, which is how
one word holds either.

The formats are in `games/tbs/include/map_tables.h`, the warp names in
`map_warps.h`.

| Name | Value | Meaning |
| --- | --- | --- |
| `LOCALID_PARTY_LEADER` | 0 | the object event the player moves |
| `LOCALID_FIRST_PLACED` | 8 | the slot of the first object event template |
| `CONDITION_ALWAYS` | -1 | a row that always holds |
| `CONDITION_IN_VIEW` | 0 | an object event shown only while within the view (`Map_AssignViewFlags`) |
| `CONDITION_FLAG_SET` | `0x1000` | with a flag (`IF_FLAG_SET`): the flag must be set, not clear |
| `MAP_WARPS_END` | `0x1ff` | ends a warp table |
| `MAP_EVENTS_END` | `0xffffffff` | ends a map event table |
| `MAP_EVENT_TALK` | 0 | map event kind: talking to an object event |
| `MAP_EVENT_WARP` | 1 | map event kind: a warp cell |
| `MAP_EVENT_COORD` | 2 | map event kind: stepping onto or pushing against a cell |
| `MAP_EVENT_BG` | 3 | map event kind: a search |
| `MAP_EVENT_ITEM` | 4 | map event kind: an item used on a cell or an object event |
| `MAP_EVENT_PSYNERGY` | 5 | map event kind: Psynergy |
| `MAP_EVENT_RAISED` | 6 | map event kind: raised by code |
| `MAP_EVENT_TALK_HALTED` | 7 | map event kind: talking to an object event Halt has stopped |
| `MAP_EVENT_TRIGGER_OWN_EFFECT` | `0x400` | trigger bit: a Psynergy event runs in place of the effect, a bg event shows no checking message |
| `FIND_ITEM(item)` | `0x00100000` + item | a bg event finds an item |
| `FIND_COINS(coins)` | `0x00200000` + coins | a bg event finds coins |
| `FIND_PSYNERGY_STONE` | `0x00300000` | a bg event finds a Psynergy Stone |
| `FIND_MESSAGE(message)` | `0x00400000` + message | a bg event shows a message |
| `FIND_MIMIC` | `0x00500000` | a bg event starts a mimic's battle |

## How a map event is chosen

When the player talks to someone, the game reads the map's event table
from the top and runs the first row that fits. That one rule is how a person
says something new as the story moves on.

![The first row that fits answers. Four talk rows from Vault's houses once the party is back from Kalay, in table order. The mayor's first row holds until he has welcomed Isaac back, and it runs the first time the player talks to him. His second row holds always, and it answers the later talks on that visit: he warns of the thieves' revenge. The cook's first row holds always and answers every talk with "We don't get a lot of travelers in winter." The cook's second row, in which he would offer his leftover bones, is never reached, because the row above it always fits first.](https://alchemy.pascalpixel.com/illustrations/scene/first-row.svg)

A map lists its rows from the latest point in the story down to the
default. In Vault's houses after Kalay, the mayor has a row that holds until
he has welcomed Isaac back, and below it one that always holds: the first
talk welcomes Isaac, and every later one warns of the thieves. A row behind
one that always fits is never reached, as the cook's second row shows.

`Map_FindEvent` returns the first row whose kind, trigger and condition
fit. A bg event on a named target (everything but `BG_EVENT_UNNAMED`) is
returned whatever its condition, unless it finds a mimic, and
`Field_SearchTile` tests the condition afterwards, so that an opened chest
still says it is empty. Rows these rules make unreachable keep a
[`// Unused:`](../../glossary.md#unused) tag in their table, and [Unused](../../unused.md) describes every
such case.

| Name | Value | Meaning |
| --- | --- | --- |
| `BG_EVENT_UNNAMED` | 0 | a bg event that shows nothing as the leader checks |
| `BG_EVENT_CHEST` | 1 | the first named target; target n shows `MsgTileChecked` + n |

## Scripts

A cutscene is written as a list of what happens, one step to a line, and the
game plays it from top to bottom while the party waits.

![A cutscene runs from top to bottom. At the top, the scene as the game plays it, each message answered: the view shakes, she cries out, turns to Isaac and speaks again. The script a woman in Vale runs when Isaac talks to her after Kalay, one step to a line with how long each step holds the scene. The player loses control. The view starts to shake, and the script waits ten frames. The shake is set to die away. She says "Aghh! I felt another one!", which holds until the player answers it and then twenty frames more. She turns to face Isaac and the scene holds twenty frames. She says the next line, which holds until the player answers it. Then the player has control again.](https://alchemy.pascalpixel.com/illustrations/scene/script-steps.svg)

A step that takes time, such as a wait, a line of dialogue or someone
walking somewhere, waits inside itself, so the next line starts only when it
is done, and the walk resumes only after the last line. Steps that take no
time, such as starting the shake, follow each other within one [frame](../../glossary.md#frame).
Whatever has to keep moving during a cutscene, like a villager walking past,
runs on its own: as an object script (the steps an object event runs each
frame), a callback (a function the map adds to run once a frame) or an
update hook (a function an object event runs each frame with itself).

A function a map event names is a script: straight-line code written in the
commands of `script.h` over the engine calls of `script_layer.h`.

```c
/// LunpaFortressUpperFloor, raised when the patrolling pair spots the leader:
/// the first stops and stares, and the second throws the party out.
static void EventScript_PatrolPairThrowsPartyOut(void) {
  EV_START();
  Engine_ObjectEventStop(UPPER_FLOOR_FIRST_PATROL);
  Engine_ObjectEventSetDestinationOffset(UPPER_FLOOR_FIRST_PATROL, 0, 0);
  OBJECT_MODE(UPPER_FLOOR_FIRST_PATROL, 0);
  OBJECT_FACE(UPPER_FLOOR_FIRST_PATROL, LOCALID_PARTY_LEADER, 0);
  Engine_ObjectEventShowEmote(UPPER_FLOOR_FIRST_PATROL,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  LunpaFortress_GuardThrowsPartyOut(UPPER_FLOOR_SECOND_PATROL);
  EV_END();
}
```

`EV_START` stops the leader and makes it the speaker, and `EV_END` returns the
camera to it. `WAIT(n)` waits `n` frames, about 59.73 a second.
`MESSAGE_SET` chooses a message and `MESSAGE_PUT` shows it spoken by an object event.
`OBJECT_POSITION_SET`, `OBJECT_WALK`, `OBJECT_SPEED`, `OBJECT_DIR`, `OBJECT_FACE` and `OBJECT_MODE`
move, turn and animate object events by local id (`OBJECT_MODE` takes an `enum
ObjectEventAnimation` such as `ANIMATION_NOD`). `FLAG_CHECK` and `FLAG_SET` test and set
game flags, and `SE(n)` plays a sound from `songs.h`. Callbacks are added with
`FieldScheduler_AddCallback` at `TASK_PRIORITY_MAP`.

| Name | Value | Meaning |
| --- | --- | --- |
| `TASK_PRIORITY_MAP` | 3200 | the [task](../../glossary.md#task) priority a map's callbacks run at |

## State that cleans itself up

A map never undoes what it changed. Instead the game keeps two sets of
flags for maps to use, and forgets them for it as the party moves on.

![What a map forgets, and when. Map temp flags are forgotten every time the party enters a map, even the one it just left: the plant Growth raised on Crossbone Isle is a sprout again, as the picture of it on its cliff ledge shows. Map-section temp flags are kept while the party moves between the maps of one map section and forgotten when it enters another map section: Vault's mayor remembers having welcomed Isaac back while the party stays in Vault, and welcomes him again on the next visit. Coming back from a battle forgets neither, so the room is as the party left it.](https://alchemy.pascalpixel.com/illustrations/scene/flag-reset.svg)

Map temp flags last until the party next enters a map, so a map keeps a
pushed stone pillar or an opened door in one and finds it back in place next
time. [Map-section](../../glossary.md#map-section) temp flags last while the party stays anywhere in the map
section, so they hold what must survive moving between its rooms, as Fuchin
Falls Cave keeps where its logs float. Coming back from a battle forgets
neither.

`Map_ResetFlagsOnEnter` clears the map's 256 flags on every ordinary
arrival and the map section's 256 when the party enters another map section
([the front page](../../README.md#game-flags) explains the ranges). Because
the numbers are reused everywhere, each map says near its top (or beside a
single use) what it keeps in them, and names those it gives a meaning in its own file
(`FLAG_FIRST_PLANT_GROWN = FLAG_MAP_TEMP_40` in Crossbone Isle,
`FLAG_WELCOMED_BACK = FLAG_MAPSEC_TEMP_00` in Vault's houses).

The flags are in `games/tbs/include/flags.h`.

| Name | Value | Meaning |
| --- | --- | --- |
| `FLAG_MAP_TEMP_00` to `FLAG_MAP_TEMP_FF` | `0x200` to `0x2ff` | cleared on every ordinary arrival on a map |
| `FLAG_MAPSEC_TEMP_00` to `FLAG_MAPSEC_TEMP_FF` | `0x300` to `0x3ff` | cleared on entering another map section |

## Code shared between overlays

Two maps that need the same piece of code each carry their own copy of
it, because a map's code can call only the main program, never another
map's.

![Code two maps share is copied into both. Two overlays side by side: Vault's town and Vault's houses. Each holds its own code, and each also holds a copy of the same two pieces: showing a villager's thought read with Mind Read, and Ivan handing back Isaac's Djinni before he leaves the party. Below them is the main program, which is always loaded and which both call. Only one map's code is loaded at a time, and it can call only the main program, so shared code is written once and copied in.](https://alchemy.pascalpixel.com/illustrations/scene/shared-code.svg)

Only one overlay is loaded at a time, so the code several maps need is
written once in `common/` and compiled into each of them. The ROM holds a
copy in each overlay, alike but for the addresses they hold (their calls
into the overlay's import table and their pointers to its data). Each map
includes the header at the point its bytes take in that overlay. A header in
`common/` is code, not declarations: including it twice in one map would
place it twice.

| Headers | Included by | What they hold |
| --- | --- | --- |
| `colosso_*.h`, `item_equip.h` | the three Colosso final matches (`colosseum_*_match.c`); `item_equip.h` also Goma Cave's entrance | the friends' cheering spots, the stage guides, the race gauge, the opponent's recorded path, the start lettering |
| `vault_*.h` | Vault and its interiors | the walking villagers, the Bone, the doorway, Ivan's farewell |
| `mercury_lighthouse_*.h` | Mercury Lighthouse and its entrance | the pool's lights that carry the leader over it, the passages between floors, the pool's shimmer |
| `kolima_*.h` | Kolima Forest's maps | the cursed forest's colours, the dust of Tret's curse, the hop onto the river's object event |
| `sol_sanctum_*.h` | Sol Sanctum's rooms | the Elemental Star room's floating lights, the corridor's and maze's entry |
| `vale.h`, `world_map_linked_effects.h` | Vale's gate and storm; Vale and the world map | the storm's door warps; the particles around a glowing object event |
| `effect_*.h` | many | `FieldEffect_Spawn` and the small effects it places, cutscene Psynergy |
| `object_*.h` | many | object events: staged object events, lookups by cell and distance, turning and waiting |
| `map_grid_panel.h`, `scroll_initialize_hblank_dma.h` | Crossbone Isle's stepping stones and Kolima Forest's floodgate; the Lemurian ship | a stretch of the layout drawn straight into the background's tile map; the water's ripple |

A header named after one map may be shared further: `colosso_item_icon.h`
also draws Crossbone Isle's Red Key. A helper that some of the maps including
its header never call is `UNUSED`, so that a current GCC does not warn there.
Renaming a function in `common/` renames it in every map that includes the
header.

## How it fits with the rest of the game

`Game_Run` unpacks the overlay into `gOverlayArea` (`Map_LoadOverlay`) and
`Field_RunMap` calls its entries; [Engine](../../engine.md#overlays)
describes the entry and import tables and how the overlay is loaded. The map
table and how maps depend on the story are on [the front
page](../../README.md#the-map-table-and-map-sections). The layouts the maps are
built on are in [Layouts](../layout/README.md), and the debug rooms, built the
same way, in [Debug rooms](../debug/README.md).

To find the code for a map, find its row in `field/maps.s`: the overlay's
resource number begins the rows of `objects.tsv` that name its files. To find
what someone says, search `games/tbs/text/en.po` for the line; its `msgctxt`
is the name the map writes (`MsgVaultWelcomeToVaultTheTownAt`).

## Limits

- An overlay's code, tables and RAM variables must fit in `gOverlayArea`,
  32 KB of [EWRAM](../../glossary.md#iwram-and-ewram).
- An overlay calls the [main image](../../glossary.md#main-image) only through its import table, never another
  overlay.
- 66 object event slots: the party's 8, then the placed object events.
- 256 map temp flags and 256 map-section temp flags, reused by every map and
  map section.

## Files

| File | What it holds |
| --- | --- |
| `<map>.c` | one map overlay: its code, then its tables |
| `<map>.h` | the overlay's own names (object events, flags, structures), where it has any |
| `entry/<map>.s` | the entry table: six veneers the engine calls |
| `import/<map>.s` | the import table: the main-image functions the overlay calls |
| `common/*.h` | code that several overlays carry, each in its own copy |

Each overlay is three objects in `games/tbs/objects.tsv`, linked in this order
into the overlay area: `entry/<map>`, `<map>` and `import/<map>`; the
rows' first column is the overlay's resource number. The map table,
`gField_MapTable` in [`field/maps.s`](../field/maps.s), gives each
map its overlay, map section, map type and layout, and names it `MapId_<Map>`;
several maps may share one overlay, and then share its file. A map overlay
is never split into parts: its code is laid out by kind, entry points,
then handlers, then tables, not by map.

The table formats are in `games/tbs/include/map_tables.h`, the script
commands in `script.h`, and the engine calls they are written over in
`script_layer.h`, `field_service.h` and `engine_call.h`. Prototypes other files
see are in `games/tbs/include/unit/map/`.

## Words used here

- **object event**: a person or thing on the map, which a map addresses by its
  local id. 0 to 7 are the party (`LOCALID_PARTY_LEADER` is 0, the one the
  player moves), and the placed object events start at `LOCALID_FIRST_PLACED`
  (8). Each map names its own in an enum.
- **position**: pixels, x east, y up and z south; the engine keeps them in
  16.16 fixed point (`PIXELS(n)`), and a cell is 16 by 16 pixels.
- **facing**: `enum Facing`, eight headings in 1/65536 turns from `FACING_EAST`
  through `FACING_SOUTH` to `FACING_NORTHEAST`.
- **condition**: a table row's test: `CONDITION_ALWAYS`, a flag that must be
  clear, or `IF_FLAG_SET(flag)`.
- **map temp flags** and **map-section temp flags**: `FLAG_MAP_TEMP_xx` and
  `FLAG_MAPSEC_TEMP_xx`, the two ranges cleared for map code
  ([above](#state-that-cleans-itself-up)). A map-section temp flag with a
  proven meaning takes a name in the map's own file, never in `flags.h`.
- **messages**: linker symbols written by bare name, `MSGNO(name)` their
  number. A message's `msgctxt` in `games/tbs/text/en.po` is its name;
  `MESSAGE_PUT` chooses the next message after showing one, so a line that
  follows a named one has no name of its own.
- **callback**: a function a map adds with `FieldScheduler_AddCallback` at
  `TASK_PRIORITY_MAP`, run once a frame until removed.
- **update hook**: an object event's `update`, run each frame with the object
  event.

## Function prefixes

A map's functions take its name (`Vault_`, `VaultRevisit_`,
`ColosseumNavampaMatch_`) and say what the player sees; most functions a
map event table names are `EventScript_` instead. Shared helpers take their family's
prefix (`Colosso_`, `MercuryLighthouse_`, `KolimaPalette_`, `StagedObjectEvent_`, and
`FieldEffect_` for the field effects). Some keep older descriptive names
(`MapObjectEvent_`, `OverlayObject_`, `Map_`) until the maps that call
them are humanized. The spellings some sites keep to match (fields reached
through `offsetof`, another struct's view of an object event, update hooks returning
`s32`) are in [Matching](../MATCHING.md).

A run of functions inside one overlay that is about one thing takes that
thing's name, and a party member's or the leader's part in a script takes
that character's (`Garet_`, `Mia_`, `Ivan_`, `Leader_`):

| Prefix | Overlay | What it acts on |
| --- | --- | --- |
| `Pillar_` | `alpine_crossing_lama_temple.c` | the pillar drawn in front of or behind the leader |
| `Village_`, `Houses_` | `altin.c` | Altin's village outside, and its houses and inn inside |
| `Beast_`, `Spray_` | `altin.c` | the hopping water beast and its spray |
| `Water_` | `altin_peak.c` | the water layer flickering away |
| `Entrance_` | `babi_lighthouse.c` | an unused division |
| `MapLocalState_` | `colosseum_satrage_match.c`, `crossbone_isle_keys.c` | storing a raised map event's trigger, and an empty hook |
| `InnWorker_` | `lalivero_interiors.c` | the inn worker's sigh |
| `Reveal_`, `HeatGauge_`, `Manticore_` | `lamakan_desert.c` | the secrets Reveal shows, the heat gauge and the Manticore |
| `Ship_`, `ShipGlow_`, `OrbGlow_`, `Ripple_`, `Wave_`, `Curtain_` | `lemurian_ship.c` | the ship's swell and glows, the water's ripple and the closing curtains |
| `TretPalette_` | `tret_tree_interiors.c` | the map's tint |
| `FloatingPillar_`, `Rubble_` | `venus_lighthouse.c` | the floating pillars and the rubble particles |
| `Reunion_`, `WagonChoice_`, `Bunza_` | `west_lunpa_cave.c` | the reunion and the choice to ride Bunza's wagon |

## Evidence and open questions

A map's name is its map section's area name from `gMapSec_NameRules`, or for a
room the game leaves unnamed, what the player sees there; its functions
are named for what the player sees; its messages are linker symbols made from
their English text. Overlay numbers are their rows of the resource directory.

Many functions in `common/` keep older descriptive names (`MapObjectEvent_`,
`OverlayObject_`, `Map_`); each takes its name with the maps that
call it, as those maps are humanized. A sound without a proven name keeps
its number (`SE_188`), as `songs.h` says.

The animated warps of `enum WarpStyle` play the leader's leaving and arrival
animations, named in `enum ObjectEventAnimation` from Isaac's sprite
(`spr_000.tsv`, animations 23 to 26): an arrival grows from a small figure
seen side-on to full size facing the player, and a leaving turns from his
back to side-on as he shrinks and is then hidden. The alternate pair, which
goes with `SE_STAIRS_DOWN`, has its own drawings of the same turns.
