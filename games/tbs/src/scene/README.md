# Scenes

![Six questions a place answers. The field engine knows a place only through six questions it asks the place's own code. When the party arrives it asks, in order: what happens here (the events, kept for the whole visit), where the party arrives (the entrances), who stands where (the placements), and whether anything needs setting up first (the initialization, which can play an arrival cutscene). At every step it asks whether the leader stands in an exit area. As the party leaves it asks where the exit leads. Each answer comes from a function, so a place can answer differently as the story goes on.](https://alchemy.pascalpixel.com/illustrations/scene/entries.svg)

Everything that makes a place itself is in its own code here: who stands
where, what each person says, what happens when the party steps on a
doorstep, searches a jar, or uses an item or Psynergy, and the cutscenes the
place plays. That code is a scene overlay, loaded when the party arrives and
replaced when it leaves. [Running a place](../object/README.md) runs the place
and asks the overlay what to do; the overlay drives the engine back through
its import table. Each overlay has a generated page of its own, listed in
[Places](../field/scenes.s) in story order, with its map and its events.

## The entry table

The engine knows nothing about any one place. It asks the place's code six
questions, always the same ones, and the answers are the place.

When the party arrives, the engine asks what can happen here (the events,
kept for the whole visit), where the party arrives (the entrances), who
stands where (the placements), and whether anything needs setting up
(the initialization). At every step it asks whether the leader is standing
in an exit area, and as the party leaves, where the exit leads. Each answer
comes from a function rather than a fixed table, so a place can answer
differently as the story goes on ([World](../../world.md#scene-entries) gives
Vault's example), and an overlay that serves several scenes can answer for
each.

The initialization restores what the flags say has happened, starts
callbacks, or plays an arrival cutscene; it may take an exit at once. It
skips arrival cutscenes when `FLAG_KEEP_PARTY_POSITION` says the party is
returning to where it stood, as after a battle.

In the code, the six are `struct SceneEntries` in `field_runtime.h`, in a
fixed order at the start of every overlay, `entry/<place>.s`: 48 bytes, a
veneer word before each function pointer. `Field_RunScene` calls
`Scene_GetEvents`, then places the party at its entrance
(`Scene_ApplyEntrance`), creates the actors from the placements and calls
`Scene_Initialize`. `Field_ProcessStep` reads the regions at every step, and
`Scene_TakeExit` reads the exits as the party leaves. An overlay that serves
several scenes chooses by `gGameState.scene`, as Babi Lighthouse's
`Scene_GetRegions` does. Some overlays give the entries names of their own
(`BabiLighthouse_GetExits`); only the order is fixed.

| Entry, as Vault names it | Offset in `struct SceneEntries` | Returns or does |
| --- | --- | --- |
| `Scene_Initialize` | `0x04` | the scene's setup, run once its actors exist |
| `Scene_GetEntrances` | `0x0c` | `struct SceneEntrance` rows |
| `Scene_GetExits` | `0x14` | the exit table |
| `Scene_GetPlacements` | `0x1c` | `struct ScenePlacement` rows |
| `Scene_GetEvents` | `0x24` | `struct SceneEvent` rows |
| `Scene_GetRegions` | `0x2c` | `struct SceneRegion` boxes, or `NULL` |

## The tables

The answers are tables: where the party can arrive and leave, who stands
where, and what answers each thing the player does.

- **Entrances** (`struct SceneEntrance`): for each entrance number, a flag
  that must be set (or -1), a position, a facing and the camera's limits.
- **Exits**: a header word per scene (`SCENE_EXITS(Place)`), then rows of
  `SCENE_EXIT(exit, scene, entrance)` mapping an exit kind to a scene and
  entrance named in `scene_links.h`, ended by `SCENE_EXITS_END`.
  `SCENE_EXIT_IF` adds a flag, and the row is taken only while that flag is
  clear. `SCENE_EXIT_TO_BATTLE` leads into a battle.
- **Placements** (`struct ScenePlacement`): a sprite, a condition, a behaviour
  (an `enum ActorBehavior` such as `ACTOR_STAND` or `ACTOR_WANDER`, or an
  object script), a position, a facing and how the actor turns when spoken to.
- **Regions** (`struct SceneRegion`): boxes in which the leader takes an exit
  kind, each with a condition. Most scenes have none.
- **Events** (`struct SceneEvent`): a control word (the kind and its detail), a
  trigger, a condition and a value, ended by `SCENE_EVENTS_END`.

### Who stands where

Each row of a place's placement table puts one person or thing on the map,
and each row owns an actor slot of its own, whether or not it appears.

![Every placement row owns an actor slot. The placement table of Mercury Lighthouse's upper floor, row by row, and the actor slot each row fills before and after the blue statue falls from its ledge. The party's eight slots come first. The first row, the statue fallen to the floor below, appears only once it has fallen; the second, the statue on its ledge, only until then. The rows after them, two pipe sections, three pipe ends and six stone statues, always appear. A row whose condition fails leaves its slot empty rather than handing it to the next row, so every actor keeps the same slot whatever the story has done, and the pipes are always the third and fourth placed actors.](https://alchemy.pascalpixel.com/illustrations/scene/placements.svg)

A row can carry a condition, a story flag that must be set or clear, and a
row whose condition fails puts nobody on the map. Its slot stays empty
rather than going to the next row, so every actor keeps one number all game
and the scene's code can name it once: on Mercury Lighthouse's upper floor
the blue statue has one row for its place on the ledge and another for where
it lands after it falls, and only one of the two appears.

The party takes slots 0 to 7, the leader the one the player moves
(`ACTOR_PARTY_LEADER`), and the table's rows take the slots from
`ACTOR_FIRST_PLACED` in order: actor `ACTOR_FIRST_PLACED + n` is always row n
(`Event_SpawnObjectTable`). A row whose sprite is one of the party's
characters (sprites 0 to 7) takes that character's own slot instead. A
condition of 0 is no flag: `Scene_AssignViewFlags` replaces it with whether
the actor lies within the view. Each scene names its actors in an enum in
row order (`ACTOR_FALLEN_STATUE = ACTOR_FIRST_PLACED`).

### Which events a cell starts

Some spots on a map do something when the leader reaches them: a doorstep
leads into a house when the leader walks into it, the edge of a village
leads on to the next scene, a ladder climbs to a roof, and a chest or a
bookcase answers the A button. The map marks each spot by the collision code
of its cell, and the scene's event table says what that code does. The same
code means different things in different places.

On the screen of Vale below, on the day the story begins, each door is a
touch row rather than an exit row: walking north into the doorstep runs a
script, `Event_EnterIsaacsHouse` for Isaac's house, that opens the door,
walks the leader in and only then asks for the exit, and that can refuse
instead, as Dora does once the party has left Vale. Touch rows also climb the
ladder beside Isaac's house (`Event_ClimbLadderOntoRoof` up,
`Event_ClimbDownFromRoof` down) and put the upper path's cells
back when the leader steps off it (`Event_ClearPathCellOverride`). Three
cells of the path at the top carry a code no row answers.

![A screen of Vale on the day the story begins, drawn from its map at twice the GBA's size, with each cell that carries one of the scene's own codes tinted and joined by a line to a label beside the picture saying what it does: in magenta, for touch rows, a door into a house and the door into Isaac's house (walk north into them), the ladder up onto the roof of Isaac's house (walk north into it) and down from the roof (walk south into it), and the cell where stepping off the upper path changes its cells back; in grey, three cells on the path at the top labelled no event. A key under it says what each colour stands for.](https://alchemy.pascalpixel.com/guides/scene/event-cells.png)

Stepping on or walking into a cell runs the first touch row whose trigger is
its code and whose condition holds, or failing one the first such exit row
(`Scene_RunTouchOrExit`); pressing A facing it runs the first such search
row. A code with no row does nothing. The scene's own codes are
`MAP_CELL_EVENT_FIRST` to `MAP_CELL_EVENT_LAST`; the codes above them mean
the same on every map ([Maps](../map/README.md#cells)).

### What an event row says

An event's kind says what starts it and what its trigger is:

| Kind | Started by | Trigger |
| --- | --- | --- |
| `EVENT_TALK` | the A button on an actor | the actor's number |
| `EVENT_TALK_HALTED` | talking to an actor Halt has stopped | the actor's number |
| `EVENT_TOUCH`, `TOUCH_FACING(f)`, `TOUCH_WALKING_INTO(f)` | stepping onto a cell, facing a way, or pushing against it | the cell's collision code |
| `EVENT_EXIT` | stepping onto a cell no touch event answers | the cell's collision code |
| `SEARCH(target)` | the A button on what is ahead, when it is no actor | the cell's collision code |
| `USE_ITEM(item)`, `USE_ITEM_ON_ACTOR(item)` | using an item | the cell's code or the actor's number |
| `PSYNERGY_ON_ACTOR(ability)`, `PSYNERGY_IN_AREA(ability, phase)` | using Psynergy on an actor, or anywhere | the actor's number, or 0 |
| `EVENT_RAISED` | the scene's code raising a value | the value |

A Psynergy event's phase (`enum PsynergyPhase`) says when it runs around the
effect: `PSYNERGY_PHASE_BEFORE_CAST` before it plays,
`PSYNERGY_PHASE_AFTER_CAST` after, and `PSYNERGY_PHASE_EFFECT_ACTS` from inside
the effect, as Force strikes, Frost freezes or Reveal shows what is hidden.
Reveal's scenes pair that row with a `PSYNERGY_PHASE_EFFECT_ENDS` one that
hides it all again as the colours come back, and `PSYNERGY_PHASE_AFTER_END`
runs when a Psynergy that runs out by itself, such as Cloak, has ended. [Field
objects](../object/README.md#how-field-psynergy-reaches-its-effect) follows a
cast through these and lists the phases' values.

An event's value is a message (`MSGNO(name)`), a function
(`EVENT_SCRIPT(function)`), the exit kind for an exit, or for a search what is
found: `FIND_ITEM`, `FIND_COINS`, `FIND_PSYNERGY_STONE`, `FIND_MESSAGE` or
`FIND_MIMIC`. Functions live above `0x10000` and messages below, which is how
one word holds either.

The formats are in `games/tbs/include/field_scene.h`, the door names in
`scene_links.h`.

| Name | Value | Meaning |
| --- | --- | --- |
| `ACTOR_PARTY_LEADER` | 0 | the actor the player moves |
| `ACTOR_FIRST_PLACED` | 8 | the slot of a placement table's first row |
| `CONDITION_ALWAYS` | -1 | a row that always holds |
| `CONDITION_IN_VIEW` | 0 | a placement shown only while within the view (`Scene_AssignViewFlags`) |
| `CONDITION_FLAG_SET` | `0x1000` | with a flag (`IF_FLAG_SET`): the flag must be set, not clear |
| `SCENE_EXITS_END` | `0x1ff` | ends an exit table |
| `SCENE_EVENTS_END` | `0xffffffff` | ends an event table |
| `EVENT_TALK` | 0 | event kind: talking to an actor |
| `EVENT_EXIT` | 1 | event kind: an exit cell |
| `EVENT_TOUCH` | 2 | event kind: stepping onto or pushing against a cell |
| `EVENT_SEARCH` | 3 | event kind: a search |
| `EVENT_ITEM` | 4 | event kind: an item used on a cell or an actor |
| `EVENT_PSYNERGY` | 5 | event kind: Psynergy |
| `EVENT_RAISED` | 6 | event kind: raised by code |
| `EVENT_TALK_HALTED` | 7 | event kind: talking to an actor Halt has stopped |
| `EVENT_TRIGGER_OWN_EFFECT` | `0x400` | trigger bit: a Psynergy event runs in place of the effect, a search shows no checking message |
| `FIND_ITEM(item)` | `0x00100000` + item | a search finds an item |
| `FIND_COINS(coins)` | `0x00200000` + coins | a search finds coins |
| `FIND_PSYNERGY_STONE` | `0x00300000` | a search finds a Psynergy Stone |
| `FIND_MESSAGE(message)` | `0x00400000` + message | a search shows a message |
| `FIND_MIMIC` | `0x00500000` | a search starts a mimic's battle |

## How an event is chosen

When the player talks to someone, the game reads the place's event table
from the top and runs the first row that fits. That one rule is how a person
says something new as the story moves on.

![The first row that fits answers. Four talk rows from Vault's houses once the party is back from Kalay, in table order. The mayor's first row holds until he has welcomed Isaac back, and it runs the first time the player talks to him. His second row holds always, and it answers the later talks on that visit: he warns of the thieves' revenge. The cook's first row holds always and answers every talk with "We don't get a lot of travelers in winter." The cook's second row, in which he would offer his leftover bones, is never reached, because the row above it always fits first.](https://alchemy.pascalpixel.com/illustrations/scene/first-row.svg)

A place lists its rows from the latest point in the story down to the
default. In Vault's houses after Kalay, the mayor has a row that holds until
he has welcomed Isaac back, and below it one that always holds: the first
talk welcomes Isaac, and every later one warns of the thieves. A row behind
one that always fits is never reached, as the cook's second row shows.

`Scene_FindEvent` returns the first row whose kind, trigger and condition
fit. A search on a named target (everything but `SEARCH_UNNAMED`) is
returned whatever its condition, unless it finds a mimic, and
`Field_SearchTile` tests the condition afterwards, so that an opened chest
still says it is empty. Rows these rules make unreachable keep a
`// Unused:` tag in their table, and [Unused](../../unused.md) describes every
such case.

| Name | Value | Meaning |
| --- | --- | --- |
| `SEARCH_UNNAMED` | 0 | a search that shows nothing as the leader checks |
| `SEARCH_CHEST` | 1 | the first named target; target n shows `MsgTileChecked` + n |

## Scripts

A cutscene is written as a list of what happens, one step to a line, and the
game plays it from top to bottom while the party waits.

![A cutscene runs from top to bottom. The script a woman in Vale runs when Isaac talks to her after Kalay, one step to a line with how long each step holds the scene. The player loses control. The view starts to shake, and the script waits ten frames. The shake is set to die away. She says "Aghh! I felt another one!", which holds until the player answers it and then twenty frames more. She turns to face Isaac and the scene holds twenty frames. She says the next line, which holds until the player answers it. Then the player has control again.](https://alchemy.pascalpixel.com/illustrations/scene/script-steps.svg)

A step that takes time, such as a wait, a line of dialogue or someone
walking somewhere, waits inside itself, so the next line starts only when it
is done, and the walk resumes only after the last line. Steps that take no
time, such as starting the shake, follow each other within one frame.
Whatever has to keep moving during a cutscene, like a villager walking past,
runs on its own as an object script, a callback or an update hook.

A function an event names is a script: straight-line code written in the
commands of `event.h` over the engine calls of `field_event.h`.

```c
/// LunpaFortressUpperFloor, raised when the patrolling pair spots the leader:
/// the first stops and stares, and the second throws the party out.
static void Event_PatrolPairThrowsPartyOut(void) {
  EV_START();
  Engine_ActorStop(UPPER_FLOOR_FIRST_PATROL);
  Engine_ActorSetDestinationOffset(UPPER_FLOOR_FIRST_PATROL, 0, 0);
  OBJECT_MODE(UPPER_FLOOR_FIRST_PATROL, 0);
  OBJECT_FACE(UPPER_FLOOR_FIRST_PATROL, ACTOR_PARTY_LEADER, 0);
  Engine_ActorShowEmote(UPPER_FLOOR_FIRST_PATROL,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  LunpaFortress_GuardThrowsPartyOut(UPPER_FLOOR_SECOND_PATROL);
  EV_END();
}
```

`EV_START` stops the leader and makes it the speaker, and `EV_END` returns the
camera to it. `WAIT(n)` waits `n` frames, about 59.73 a second.
`MESSAGE_SET` chooses a message and `MESSAGE_PUT` shows it spoken by an actor.
`OBJECT_POSITION_SET`, `OBJECT_WALK`, `OBJECT_SPEED`, `OBJECT_DIR`, `OBJECT_FACE` and `OBJECT_MODE`
move, turn and animate actors by number (`OBJECT_MODE` takes an `enum
ActorAnimation` such as `ANIMATION_NOD`). `FLAG_CHECK` and `FLAG_SET` test and set
game flags, and `SE(n)` plays a sound from `songs.h`. Callbacks are added with
`FieldScheduler_AddCallback` at `TASK_PRIORITY_SCENE`.

| Name | Value | Meaning |
| --- | --- | --- |
| `TASK_PRIORITY_SCENE` | 3200 | the task priority a scene's callbacks run at |

## State that cleans itself up

A scene never undoes what it changed. Instead the game keeps two sets of
flags for scenes to use, and forgets them for it as the party moves on.

![What a place forgets, and when. Scene flags are forgotten every time the party enters a scene, even the one it just left: the plant Growth raised on Crossbone Isle is a sprout again. Place flags are kept while the party moves between the scenes of one place and forgotten when it enters another place: Vault's mayor remembers having welcomed Isaac back while the party stays in Vault, and welcomes him again on the next visit. Coming back from a battle forgets neither, so the room is as the party left it.](https://alchemy.pascalpixel.com/illustrations/scene/flag-reset.svg)

Scene flags last until the party next enters a scene, so a scene keeps a
pushed stone pillar or an opened door in one and finds it back in place next
time. Place flags last while the party stays anywhere in the place, so they
hold what must survive moving between its rooms, as Fuchin Falls Cave keeps
where its logs float. Coming back from a battle forgets neither.

`Scene_ResetFlagsOnEnter` clears the scene's 256 flags on every ordinary
entrance and the place's 256 when the party enters another group ([the front
page](../../README.md#game-flags) explains the ranges). Because the numbers
are reused everywhere, each scene says near its top (or beside a single use)
what it keeps in them, and names those it gives a meaning in its own file
(`FLAG_FIRST_PLANT_GROWN = FLAG_SCENE_TEMP_40` in Crossbone Isle,
`FLAG_WELCOMED_BACK = FLAG_PLACE_TEMP_00` in Vault's houses).

The flags are in `games/tbs/include/flags.h`.

| Name | Value | Meaning |
| --- | --- | --- |
| `FLAG_SCENE_TEMP_00` to `FLAG_SCENE_TEMP_FF` | `0x200` to `0x2ff` | cleared on every ordinary entrance to a scene |
| `FLAG_PLACE_TEMP_00` to `FLAG_PLACE_TEMP_FF` | `0x300` to `0x3ff` | cleared on entering another group |

## Code shared between overlays

Two places that need the same piece of code each carry their own copy of
it, because a place's code can call only the main program, never another
place's.

![Code two places share is copied into both. Two overlays side by side: Vault's town and Vault's houses. Each holds its own code, and each also holds a copy of the same two pieces: showing a villager's thought read with Mind Read, and Ivan handing back Isaac's Djinni before he leaves the party. Below them is the main program, which is always loaded and which both call. Only one place's code is loaded at a time, and it can call only the main program, so shared code is written once and copied in.](https://alchemy.pascalpixel.com/illustrations/scene/shared-code.svg)

Only one overlay is loaded at a time, so the code several places need is
written once in `common/` and compiled into each of them. The ROM holds a
copy in each overlay, alike but for the addresses they hold (their calls
into the overlay's import table and their pointers to its data). Each scene
includes the header at the point its bytes take in that overlay. A header in
`common/` is code, not declarations: including it twice in one scene would
place it twice.

| Headers | Included by | What they hold |
| --- | --- | --- |
| `colosso_*.h`, `item_equip.h` | the three Colosso final matches (`colosseum_*_match.c`); `item_equip.h` also Goma Cave's entrance | the friends' cheering spots, the stage guides, the race gauge, the opponent's recorded path, the start lettering |
| `vault_*.h` | Vault and its interiors | the walking villagers, the Bone, the doorway, Ivan's farewell |
| `mercury_lighthouse_*.h` | Mercury Lighthouse and its entrance | the pool's lights that carry the leader over it, the passages between floors, the pool's shimmer |
| `kolima_*.h` | Kolima Forest's scenes | the cursed forest's colours, the dust of Tret's curse, the hop onto the river's actor |
| `sol_sanctum_*.h` | Sol Sanctum's rooms | the Elemental Star room's floating lights, the corridor's and maze's entry |
| `vale.h`, `world_map_linked_effects.h` | Vale's gate and storm; Vale and the world map | the storm's door exits; the particles around a glowing actor |
| `effect_*.h` | many | `FieldEffect_Spawn` and the small effects it places, cutscene Psynergy |
| `object_*.h` | many | actors: staged actors, lookups by cell and distance, turning and waiting |
| `map_panel.h`, `scroll_initialize_hblank_dma.h` | Crossbone Isle's stepping stones and Kolima Forest's floodgate; the Lemurian ship | a stretch of map drawn into the tile map; the water's ripple |

A header named after one place may be shared further: `colosso_item_icon.h`
also draws Crossbone Isle's Red Key. A helper that some of the scenes including
its header never call is `UNUSED`, so that a current GCC does not warn there.
Renaming a function in `common/` renames it in every scene that includes the
header.

## How it fits with the rest of the game

`Game_Run` unpacks the overlay into `gOverlayArea` (`Scene_LoadOverlay`) and
`Field_RunScene` calls its entries; [Engine](../../engine.md#overlays)
describes the entry and import tables and how the overlay is loaded. The scene
table and how scenes depend on the story are on [the front
page](../../README.md#the-scene-table-and-places). The maps the scenes are set
on are in [Maps](../map/README.md), and the debug rooms, built the same way,
in [Debug rooms](../debug/README.md).

To find the code for a place, find its row in `field/scenes.s`: the overlay's
resource number begins the rows of `objects.tsv` that name its files. To find
what someone says, search `games/tbs/text/en.po` for the line; its `msgctxt`
is the name the scene writes (`MsgVaultWelcomeToVaultTheTownAt`).

## Limits

- An overlay's code, tables and RAM variables must fit in `gOverlayArea`,
  32 KB of EWRAM.
- An overlay calls the main image only through its import table, never another
  overlay.
- 66 actor slots: the party's 8, then the placed actors.
- 256 scene flags and 256 place flags, reused by every scene and place.

## Files

| File | What it holds |
| --- | --- |
| `<place>.c` | one scene overlay: its code, then its tables |
| `<place>.h` | the overlay's own names (actors, flags, structures), where it has any |
| `entry/<place>.s` | the entry table: six veneers the engine calls |
| `import/<place>.s` | the import table: the main-image functions the overlay calls |
| `common/*.h` | code that several overlays carry, each in its own copy |

Each overlay is three objects in `games/tbs/objects.tsv`, linked in this order
into the overlay area: `entry/<place>`, `<place>` and `import/<place>`; the
rows' first column is the overlay's resource number. The scene table,
`gField_SceneTable` in [`field/scenes.s`](../field/scenes.s), gives each
scene its overlay, group, variant and map, and names it `SceneId_<Place>`;
several scenes may share one overlay, and then share its file. A scene overlay
is never split into parts (S3): its code is laid out by kind, entry points,
then handlers, then tables, not by place.

The table formats are in `games/tbs/include/field_scene.h`, the script
commands in `event.h`, and the engine calls they are written over in
`field_event.h`, `field_service.h` and `engine_call.h`. Prototypes other files
see are in `games/tbs/include/unit/scene/`.

## Words used here

- **actor**: a field object a scene addresses by number. 0 to 7 are the party
  (`ACTOR_PARTY_LEADER` is 0, the one the player moves), and the placed actors
  start at `ACTOR_FIRST_PLACED` (8). Each scene names its own in an enum.
- **position**: pixels, x east, y up and z south; the engine keeps them in
  16.16 fixed point (`PIXELS(n)`), and a cell is 16 by 16 pixels.
- **facing**: `enum Facing`, eight headings in 1/65536 turns from `FACING_EAST`
  through `FACING_SOUTH` to `FACING_NORTHEAST`.
- **condition**: a table row's test: `CONDITION_ALWAYS`, a flag that must be
  clear, or `IF_FLAG_SET(flag)`.
- **scene flags** and **place flags**: `FLAG_SCENE_TEMP_xx` and
  `FLAG_PLACE_TEMP_xx`, the two ranges cleared for scene code
  ([above](#state-that-cleans-itself-up)). A place flag with a proven meaning
  takes a name in the scene's own file, never in `flags.h`.
- **messages**: linker symbols written by bare name, `MSGNO(name)` their
  number. A message's `msgctxt` in `games/tbs/text/en.po` is its name;
  `MESSAGE_PUT` chooses the next message after showing one, so a line that
  follows a named one has no name of its own.
- **callback**: a function a scene adds with `FieldScheduler_AddCallback` at
  `TASK_PRIORITY_SCENE`, run once a frame until removed.
- **update hook**: an actor's `update`, run each frame with the actor.

## Function prefixes

A scene's functions take its place's name (`Vault_`, `VaultRevisit_`,
`ColosseumNavampaMatch_`) and say what the player sees; most functions an
event table names are `Event_` instead. Shared helpers take their family's
prefix (`Colosso_`, `MercuryLighthouse_`, `KolimaPalette_`, `StagedActor_`, and
`FieldEffect_` for the field effects). Some keep older descriptive names
(`SceneActor_`, `OverlayObject_`, `FieldScene_`) until the scenes that call
them are humanized. The spellings some sites keep to match (fields reached
through `offsetof`, another struct's view of an actor, update hooks returning
`s32`) are in [Matching](../MATCHING.md).

A run of functions inside one overlay that is about one thing takes that
thing's name, and a party member's or the leader's part in an event takes
that character's (`Garet_`, `Mia_`, `Ivan_`, `Leader_`):

| Prefix | Overlay | What it acts on |
| --- | --- | --- |
| `Pillar_` | `alpine_crossing_lama_temple.c` | the pillar drawn in front of or behind the leader |
| `Village_`, `Houses_` | `altin.c` | Altin's village outside, and its houses and inn inside |
| `Beast_`, `Spray_` | `altin.c` | the hopping water beast and its spray |
| `Water_` | `altin_peak.c` | the water layer flickering away |
| `Entrance_` | `babi_lighthouse.c` | an unused division |
| `SceneState_` | `colosseum_satrage_match.c`, `crossbone_isle_keys.c` | storing a raised-event trigger, and an empty hook |
| `InnWorker_` | `lalivero_interiors.c` | the inn worker's sigh |
| `Reveal_`, `HeatGauge_`, `Manticore_` | `lamakan_desert.c` | the secrets Reveal shows, the heat gauge and the Manticore |
| `Ship_`, `ShipGlow_`, `OrbGlow_`, `Ripple_`, `Wave_`, `Curtain_` | `lemurian_ship.c` | the ship's swell and glows, the water's ripple and the closing curtains |
| `TretPalette_` | `tret_tree_interiors.c` | the scene's tint |
| `FloatingPillar_`, `Rubble_` | `venus_lighthouse.c` | the floating pillars and the rubble particles |
| `Reunion_`, `WagonChoice_`, `Bunza_` | `west_lunpa_cave.c` | the reunion and the choice to ride Bunza's wagon |

## Evidence and open questions

A scene's name is its place's area name from `gScene_AreaNameRules`, or for a
room the game leaves unnamed, what the player sees there (H3); its functions
are named for what the player sees; its messages are linker symbols made from
their English text. Overlay numbers are their rows of the resource directory.

Many functions in `common/` keep older descriptive names (`SceneActor_`,
`OverlayObject_`, `FieldScene_`); each takes its name with the places that
call it, as those places are humanized. A sound without a proven name keeps
its number (`SE_188`), as `songs.h` says.

The animated exits of `enum ExitStyle` play the leader's leaving and arrival
animations, named in `enum ActorAnimation` from Isaac's sprite
(`spr_000.tsv`, animations 23 to 26): an arrival grows from a small figure
seen side-on to full size facing the player, and a leaving turns from his
back to side-on as he shrinks and is then hidden. The alternate pair, which
goes with `SE_STAIRS_DOWN`, has its own drawings of the same turns.
