# Scenes

A scene overlay is the code and tables of one place: who stands where, what
happens when the party talks to someone, touches a cell, searches, uses an
item or Psynergy, and the cutscenes the place plays. [Field
objects](../object/README.md) runs the scene and asks the overlay for these
through six fixed entry points; the overlay drives the engine back through
its import table. Each overlay has a generated page of its own, listed in
[Places](../field/scenes.s) in story order, with its map and its events.

## Files

| File | What it holds |
| --- | --- |
| `<place>.c` | one scene overlay: its code, then its tables |
| `<place>.h` | the overlay's own names (actors, flags, structures), where it has any |
| `entry/<place>.s` | the entry table: six veneers the engine calls |
| `import/<place>.s` | the import table: the main-image functions the overlay calls |
| `common/*.h` | code that several overlays carry, each in its own copy (below) |

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

## Words and local names

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
  `FLAG_PLACE_TEMP_xx`, the two ranges cleared for scene code (below). A place
  flag with a proven meaning takes a name in the scene's own file, never in
  `flags.h`.
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
`FieldFx_` for the field effects). Some keep older descriptive names
(`SceneActor_`, `OverlayObject_`, `FieldScene_`) until the scenes that call
them are humanized. The spellings some sites keep to match (fields reached
through `offsetof`, another struct's view of an actor, update hooks returning
`s32`) are in [Matching](../MATCHING.md).

## Design

### The entry table

The engine knows a scene only through six veneers at the start of its overlay,
always in this order (`struct SceneEntries` in `field_runtime.h`):

| Entry, as Vault names it | Returns or does |
| --- | --- |
| `Scene_Initialize` | the scene's setup, run once its actors exist |
| `Scene_GetEntrances` | `struct SceneEntrance` rows |
| `Scene_GetExits` | the exit table |
| `Scene_GetPlacements` | `struct ScenePlacement` rows |
| `Scene_GetEvents` | `struct SceneEvent` rows |
| `Scene_GetRegions` | `struct SceneRegion` boxes, or `NULL` |

Because each is a function, it can choose its table by the story flags
([World](../../world.md#scene-entries) gives Vault's example) and, in an
overlay that serves several scenes, by `gGameState.scene`, as Babi
Lighthouse's `Scene_GetRegions` does. Some overlays give the entries names of
their own (`BabiLighthouse_GetExits`); only the order is fixed.

`Field_RunScene` asks for the events first and keeps them for the whole scene,
then places the party at its entrance, creates the actors, and calls
`Scene_Initialize`. The initialization restores what the flags say has
happened, starts callbacks, or plays an arrival cutscene; it may take an exit
at once. It skips arrival cutscenes when `FLAG_KEEP_PARTY_POSITION` says the
party is returning to where it stood, as after a battle. The exits are read
only as the party leaves (`Scene_TakeExit`), and the regions at every step.

### The tables

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
  The rows whose condition holds become actors from `ACTOR_FIRST_PLACED` on, in
  order. A condition of 0 is no flag: `Scene_AssignViewFlags` replaces it with
  whether the actor lies within the view.
- **Regions** (`struct SceneRegion`): boxes in which the leader takes an exit
  kind, each with a condition. Most scenes have none.
- **Events** (`struct SceneEvent`): a control word (the kind and its detail), a
  trigger, a condition and a value, ended by `SCENE_EVENTS_END`.

In Vale, where Garet's sister stands, the leader is actor 0 and the actors
the placement rows made are numbered from `ACTOR_FIRST_PLACED`, in the order
their rows' conditions held.

![Vale at twice the GBA's size where Garet's sister stands, each actor on screen labelled with its actor number: Isaac 0, Garet's sister 18, the stone pillar 20 and two other actors, 10 and 19](https://alchemy.pascalpixel.com/guides/scene/actors.png)

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

The touch, exit and search rows meet the map through its collision codes: a
cell whose code is one of the scene's own, 1 to 239, is answered by the first
row of a matching kind with that code as its trigger. On the screen of Vale
below, the codes at the doorsteps and beside the second house are touch
rows', and the path's 19 has no row in the event table.

![A screen of Vale by day drawn from its map at twice the GBA's size, each cell with one of the scene's own collision codes tinted and labelled with the code: magenta for the cells a touch row names, such as 7 and 8 at the doorsteps of two houses and 100 and 101 beside the second, and grey for 19, three cells on a path that no row names](https://alchemy.pascalpixel.com/guides/scene/event-cells.png)

A Psynergy event's phase (`enum PsynergyPhase`) says when it runs around the
effect: `PSYNERGY_PHASE_BEFORE_CAST` before it plays,
`PSYNERGY_PHASE_AFTER_CAST` after, and `PSYNERGY_PHASE_EFFECT_ACTS` from inside
the effect, as Force strikes, Frost freezes or Reveal shows what is hidden.
Reveal's scenes pair that row with a `PSYNERGY_PHASE_EFFECT_ENDS` one that
hides it all again as the colours come back, and `PSYNERGY_PHASE_AFTER_END`
runs when a Psynergy that runs out by itself, such as Cloak, has ended. [Field
objects](../object/README.md#how-field-psynergy-reaches-its-effect) follows a
cast through these.

An event's value is a message (`MSGNO(name)`), a function
(`EVENT_SCRIPT(function)`), the exit kind for an exit, or for a search what is
found: `FIND_ITEM`, `FIND_COINS`, `FIND_PSYNERGY_STONE`, `FIND_MESSAGE` or
`FIND_MIMIC`. Functions live above `0x10000` and messages below, which is how
one word holds either.

### How an event is chosen

`Scene_FindEvent` takes the first row that fits in kind, trigger and condition,
so a scene lists the rows for one actor or cell from the latest story state to
the default, and a row below one for the same actor or cell with no condition
is never reached. A search on a
named target (everything but `SEARCH_UNNAMED`) is returned whatever its
condition, unless it finds a mimic, and `Field_SearchTile` tests the condition
afterwards, so that an opened chest still says it is empty. Rows these rules
make unreachable keep a `// Unused:` tag in their table, and
[Unused](../../unused.md) describes every such case.

### Scripts

A function an event names is a script written in the commands of `event.h`
over the engine calls of `field_event.h`:

```c
/// LunpaFortressUpperFloor, raised when the patrolling pair spots the leader:
/// the first stops and stares, and the second throws the party out.
static void Event_PatrolPairThrowsPartyOut(void) {
  EV_BEGIN();
  Engine_ActorStop(UPPER_FLOOR_FIRST_PATROL);
  Engine_ActorSetDestinationOffset(UPPER_FLOOR_FIRST_PATROL, 0, 0);
  OBJ_MODE(UPPER_FLOOR_FIRST_PATROL, 0);
  OBJ_FACE(UPPER_FLOOR_FIRST_PATROL, ACTOR_PARTY_LEADER, 0);
  Engine_ActorShowEmote(UPPER_FLOOR_FIRST_PATROL,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  LunpaFortress_GuardThrowsPartyOut(UPPER_FLOOR_SECOND_PATROL);
  EV_END();
}
```

`EV_BEGIN` stops the leader and makes it the speaker, and `EV_END` returns the
camera to it. `WAIT(n)` waits `n` frames, about 59.73 a second.
`MESSAGE_SET` chooses a message and `MESSAGE_PUT` shows it spoken by an actor.
`OBJ_POS_SET`, `OBJ_WALK`, `OBJ_SPEED`, `OBJ_DIR`, `OBJ_FACE` and `OBJ_MODE`
move, turn and animate actors by number (`OBJ_MODE` takes an `enum
ActorAnimation` such as `ANIM_NOD`). `FLAG_CHECK` and `FLAG_SET` test and set
game flags, and `SE(n)` plays a sound from `songs.h`.

The script runs to its end before the scene loop looks at anything else, so a
cutscene is straight-line code: each command that takes time waits inside
itself. What must move meanwhile runs as object scripts, callbacks or update
hooks.

### State that cleans itself up

A scene never undoes what it changed. `Scene_ResetFlagsOnEnter` clears the
scene's 256 flags on every ordinary entrance and the place's 256 when the
party enters another group ([the front page](../../README.md#game-flags)
explains the ranges). So a scene keeps a pushed stone pillar or an opened door in a
`FLAG_SCENE_TEMP_xx` flag and finds it back in place next time, and keeps what must
survive moving between the place's scenes in a `FLAG_PLACE_TEMP_xx` flag, as
Fuchin Falls Cave keeps where its logs float.
Because the numbers are reused everywhere, each scene says near its top (or
beside a single use) what it keeps in them.

### Code shared between overlays

Only one overlay is loaded at a time and it can call only the main image, so
code several overlays need is compiled into each of them: the ROM holds
byte-identical copies of the same functions. Such code is written once, in
`common/`, and each scene includes the header at the point its bytes take in
that overlay. A header in `common/` is code, not declarations: including it
twice in one scene would place it twice.

| Headers | Included by | What they hold |
| --- | --- | --- |
| `colosso_*.h`, `item_equip.h` | the three Colosso final matches (`colosseum_*_match.c`); `item_equip.h` also Goma Cave's entrance | the friends' cheering spots, the stage guides, the race gauge, the rival's recorded path, the start lettering |
| `vault_*.h` | Vault and its interiors | the walking villagers, the Bone, the doorway, Ivan's farewell |
| `mercury_lighthouse_*.h` | Mercury Lighthouse and its entrance | the pool's lights that carry the leader over it, the passages between floors, the pool's shimmer |
| `kolima_*.h` | Kolima Forest's scenes | the cursed forest's colours, the dust of Tret's curse, the hop onto the river's actor |
| `sol_sanctum_*.h` | Sol Sanctum's rooms | the Elemental Star room's floating lights, the corridor's and maze's entry |
| `vale.h`, `world_map_linked_effects.h` | Vale's gate and storm; Vale and the world map | the storm's door exits; the particles around a glowing actor |
| `effect_*.h` | many | `FieldFx_Spawn` and the small effects it places, cutscene Psynergy |
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
