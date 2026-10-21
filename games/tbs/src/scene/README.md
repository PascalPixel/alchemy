# Scenes

This folder holds The Broken Seal's scenes: the code and tables of every place
the party can walk around in, one overlay each. A scene places the people and
things of a map, says what happens when the party talks to someone, touches a
spot, searches or uses Psynergy, and plays its cutscenes. The field engine (in
`games/tbs/src/field` and `games/tbs/src/object`) runs the scene; the scene
drives it through the engine's services.

See also: [Field objects](../object/README.md), which runs a scene and its
events; [Field](../field/README.md), the engine underneath; and
[Maps](../map/README.md), the maps the scenes are set on.

## Files

| File | What it holds |
| --- | --- |
| `<place>.c` | one scene overlay: its code, then its tables |
| `<place>.h` | the scene's own names (actors, flags, structures), where it has any |
| `entry/<place>.s` | the overlay's entry table: six veneers the engine calls |
| `import/<place>.s` | the overlay's import table: the main-image functions it calls |
| `common/*.h` | code that several overlays carry, each in its own copy (below) |

The prototypes other files see are in `games/tbs/include/unit/scene/`. The
command macros are in `games/tbs/include/event.h`, the engine's services in
`field_event.h` and `field_service.h`, and the table formats in
`field_scene.h`. The game's text is in `games/tbs/text`, one PO catalog per
language.

Each overlay is three objects in `games/tbs/objects.tsv`, linked in that order
to run in the overlay area: `entry/<place>`, `<place>` and `import/<place>`. Its first
column is the overlay's resource number. The scene table,
`gField_SceneTable` in `games/tbs/src/field/scenes.s`, gives each scene its
overlay, its group (the place it belongs to), its variant and its map, and names
it `SceneId_<Place>`; several scenes may share one overlay (Bilibin's three,
for one). A scene is one object: it has no parts.

## Words

- **actor**: a field object a scene addresses by number. 0 to 7 are the
  party's characters (`ACTOR_PARTY_LEADER` is 0, the one the player moves),
  8 on are the placed actors (`ACTOR_FIRST_PLACED`).
- **position**: 16.16 fixed point pixels, x across, y up and z down the map;
  scenes write pixels (`PIXELS(n)`, or plain pixels to a command). A **cell**
  is 16 by 16 pixels (`x >> 20`).
- **facing**: one of eight headings, `FACING_EAST` through `FACING_NORTHEAST`,
  clockwise as seen on the screen; underneath it's a 16-bit fraction of a turn.
- **flags**: game flags, from `flags.h` where a name is proven: the party
  and the Djinn first, then the system flags the engine shares, then the
  story flags, and most chest and find flags last, each place's in its own
  section. The scene's own flags (`FLAG_SCENE_FIRST` to `FLAG_SCENE_LAST`) are
  cleared on every ordinary entrance: they are written `FLAG_SCENE_TEMP_xx`,
  and each scene says near its top (or beside a single use) what it keeps in
  them. A place's flags (`FLAG_PLACE_FIRST` to `FLAG_PLACE_LAST`) belong to
  the group of scenes of one place, which reuses the numbers: they are written
  `FLAG_PLACE_TEMP_xx`, or by a name of the scene's own in its file where
  one is proven (never in `flags.h`), and each scene says what it keeps in
  them. A placement whose condition is 0 is placed only within the view
  (`Scene_AssignViewFlags`); that 0 is no flag. `FLAG_KEEP_PARTY_POSITION` is
  set when the party returns to where it was, as after a battle: a scene's
  initialization skips its arrival cutscenes then.
- **messages** are linker symbols written by bare name; `MSGNO(name)` is the
  number, and a message's successor is the next line said.
- **sounds** come from `songs.h`: `SE_` and `MUS_` names where the sound is
  known, its number otherwise (`SE_188`).
- **callbacks**: functions the scene adds with `FieldScheduler_AddCallback`
  (at `TASK_PRIORITY_SCENE`), run once a frame until removed.
- **update hooks**: an actor's `update`, run each frame with the actor.

## Prefixes

A scene's functions are `<Place>_` (`Vault_`, `VaultRevisit_`,
`ColossoLog_`) and say what the player sees, from the English release's names
and text; an unknown keeps a placeholder. The shared helpers take their
family's prefix (`Colosso_`, `MercuryLighthouse_`, `KolimaPalette_`,
`StagedActor_`, `Effect_`); some still carry older descriptive names
(`SceneActor_`, `SceneEffect_`, `OverlayObject_`, `FieldScene_`) until the
scenes that call them are renamed with them.

The spellings some sites keep to match (fields reached through `offsetof`,
another struct's view of an actor, update hooks that return `s32`), and the
comment markers, are in [Matching](../MATCHING.md).

## Design

### A scene is an overlay that answers six questions

Every scene is its own overlay, loaded into the same 32 KB overlay area when
the party enters, so the game keeps only one place's code in memory at a
time. The field engine knows nothing about a scene except its entry table:
six veneers at the overlay's start, always in the same order, which it calls
as functions (`struct SceneEntries`). Each returns one of the scene's tables,
or runs its setup.

They are functions rather than pointers to tables so that a scene can answer
differently as the story moves on: Vault's `Scene_GetEvents` returns
`gVault_EventsAfterThieves` once the thieves are in jail, and the engine
needs no notion of story state at all. The same goes for placements, whose
rows carry conditions, and for exits, whose rows can depend on a flag.

### State that cleans itself up

A scene does not undo what it changed when the party leaves. Instead, two
ranges of game flags are cleared for it on entry (`Scene_ResetFlagsOnEnter`,
`games/tbs/src/object/object/scene.c`): the scene's own flags on every
ordinary entrance, and the place's flags whenever the party enters a
different group of scenes, the scenes of one place. So a scene keeps a
moved crate or an opened door in a `FLAG_SCENE_TEMP_xx` flag, and it is put
back the next time the party comes in; a place keeps what should last while
the party is anywhere inside it in `FLAG_PLACE_TEMP_xx` flags, as Fuchin
Falls Cave keeps where its logs float. When the party returns to where it
was, as after a battle, `FLAG_KEEP_PARTY_POSITION` keeps both ranges, and a scene's
initialization skips its arrival cutscenes.

### Why shared code is copied into each scene

An overlay can only call the main image, through its import table, because
only one overlay is ever loaded (see [System](../system/README.md)). So code
that several overlays need is in each of them, and the ROM holds several
byte-for-byte copies of the same functions. Where the same code appears in
several overlays it is written once, in `common/`, and each scene includes
it at the place its bytes are, under the line `// Shared source is included
here in ROM order.` Each header opens by saying which scenes include it. A
header in `common/` is code, not declarations: including it twice in one
scene would place it twice.

The headers are named after the place or the job they share:

| Headers | Scenes | What they hold |
| --- | --- | --- |
| `colosso_*.h`, `item_equip.h` | the sites of the Colosso finals matches (Colosseum Final 1, 2 and 3) | the friends' cheering spots, the stage guides, the race gauge, the rival's recorded path, the guide's pointer and the start lettering |
| `vault_*.h` | Vault and its interiors | the walking villagers, the Bone, the doorway, Ivan's farewell |
| `mercury_lighthouse_*.h` | Mercury Lighthouse and its entrance | the pool's lights that carry the leader over it, the passages between floors, the pool's shimmer |
| `kolima_*.h` | Kolima and its forest | the cursed forest's colours, the dust of Tret's curse, the hop onto the river's actor |
| `sol_sanctum_*.h` | Sol Sanctum's rooms | the Elemental Star room's floating lights, the corridor's and maze's entry |
| [`common/vale.h`](common/vale.h), [`common/world_map_linked_effects.h`](common/world_map_linked_effects.h) | Vale's scenes, the world map | the storm's door exits, the particles around a glowing actor |
| `effect_*.h` | many | Effect_Spawn and the small effects it places, cutscene Psynergy |
| `object_*.h` | many | actors: pushed blocks and rolling logs, the floating Nut, lookups by cell and distance, turning and waiting |
| `map_panel.h`, [`common/scroll_initialize_hblank_dma.h`](common/scroll_initialize_hblank_dma.h) | Crossbone Isle and Kolima; the Lemurian ship | a stretch of map drawn into the tile map; the water's ripple |

A helper that some of the scenes including its header never call is
`UNUSED`, so that a current GCC does not warn in those scenes.

A header named after one place may be shared further:
[`common/colosso_item_icon.h`](common/colosso_item_icon.h) also draws
Crossbone Isle's Red Key.

Renaming a function in `common/` renames it in every scene that includes the
header, and in the scene's entry table or event table when it is listed
there.

## How a scene runs

`Field_RunScene` (`games/tbs/src/object/object/run_scene.c`) loads the map,
then calls the overlay through its entry table, in this order:

1. **events**: the event table, kept for the whole scene;
2. **entrances**: where the party appears, by entrance number;
3. **placements**: the actors to create;
4. **initialize**: the scene's own setup, run once the actors exist: restore
   what the flags say has happened, start callbacks, or play a cutscene and
   take an exit straight away;
5. **exits** are read when the party leaves (`Scene_TakeExit`), and the
   sixth entry, **regions**, at every step: boxes in which the leader takes
   an exit. Most scenes have none and return `NULL`.

A scene that changes with the story returns another table from the same
entry (see the design above).

Then the party walks until something happens: an event runs, or the party
takes an exit and the next scene is loaded.

### The tables

- **Entrances** (`struct SceneEntrance`): a position, a facing and the
  camera's limits for each entrance number.
- **Exits** (`SCENE_EXIT(exit, scene, entrance)`): the scene and entrance each
  exit leads to; `SCENE_EXIT_IF` adds a flag.
- **Placements** (`struct ScenePlacement`): a sprite, a condition, a
  behaviour (`ACTOR_STAND`, `ACTOR_WANDER` or an object script), a position in
  pixels, a facing and how the actor turns when spoken to. The rows whose
  condition holds become actors 8, 9 and so on, in order.
- **Events** (`struct SceneEvent`): a kind, a trigger, a condition and what
  happens, ending with `SCENE_EVENTS_END`.

An event's kind says what starts it and what its trigger is:

| Kind | Started by | Trigger |
| --- | --- | --- |
| `EVENT_TALK` | talking to an actor | the actor's number |
| `EVENT_TOUCH`, `TOUCH_WALKING_INTO(facing)` | stepping on, or walking into, a map cell | the cell's collision code |
| `EVENT_EXIT` | walking off through an exit | the cell's collision code |
| `SEARCH(target)` | the A button on a cell | the cell's collision code |
| `PSYNERGY_ON_ACTOR(ability)` | Psynergy on an actor | the actor's number |
| `PSYNERGY_IN_AREA(ability, phase)` | Psynergy anywhere in the scene | 0 |
| `EVENT_RAISED` | the scene's code raising a value | the value |

What happens is a message (`MSGNO(MsgName)`), a function
(`EVENT_SCRIPT(Function)`), or for a search what is found (`FIND_ITEM`,
`FIND_COINS`, `FIND_PSYNERGY_STONE`, `FIND_MESSAGE`). A condition is
`CONDITION_ALWAYS`, a flag that must be clear, or `IF_FLAG_SET(flag)`.

## How to find a scene and an event in it

1. Find the place in `scenes.s`: its row gives the overlay's resource
   number, and the rows of `objects.tsv` that begin with that number name its
   files (`vault.c` for Vault). Scenes that share an overlay (`GomaCaveWaterway1` and
   `GomaCaveWaterway2`) share one file.
2. To find what someone says, search `games/tbs/text/en.po` for the line; its
   `msgctxt` is the message's linker name (`MsgVaultWelcomeToVaultTheTownAt`),
   which the scene uses as written.
3. The scene's event table names the function or message for each actor and
   spot; its actors are numbered from 8 in the order of the placement table.

## How to change a line of dialogue

Edit the line's `msgstr` in `games/tbs/text/en.po`; the build encodes the
catalog into the English edition, so nothing in the scene changes. A message
the code names, such as `MSGNO(MsgVaultWelcomeToVaultTheTownAt)`, is the
entry whose `msgctxt` is that name. A message the code reaches as the one
after a named one (`MsgName + n`, or the next line a `MESSAGE_PUT` says) has
the `msgctxt` `message` and is found by its `msgid`: the named message's
`msgid` plus n. Keep the control codes in braces and the `\n` line breaks the
window needs (`{page_break}` starts a new page). The other five languages,
`ja.po`, `de.po`, `es.po`, `fr.po` and `it.po`, are separate catalogs for
their own editions: each has the same named messages under the same
`msgctxt`, with its own `msgid` numbers, so change each language's
`msgstr` by the `msgctxt`, or leave it and only English changes. Any text
change makes that edition differ from the original cartridge, so it no longer
compares OK.

## How to write a scene

Scenes are written in the commands of `event.h` and the services of
`field_event.h`:

```c
// Stop first guard, face leader, then second guard expels party.
static void LunpaFortress_ConfrontCaughtPairedPatrol(void) {
  EV_BEGIN();
  Engine_ActorStop(9);
  Engine_ActorSetDestinationOffset(9, 0, 0);
  OBJ_MODE(9, 0);
  OBJ_FACE(9, ACTOR_PARTY_LEADER, 0);
  Engine_ActorShowEmote(9, EMOTE_IN_FRONT, 0);
  LunpaFortress_ExpelPartyWithGuard(10);
  EV_END();
}
```

- `EV_BEGIN` and `EV_END` bracket a cutscene, in which the player does not
  control the party. `WAIT(n)` waits n frames (60 a second).
- `MESSAGE_SET` chooses a message and `MESSAGE_PUT` shows it, spoken by an
  actor; each `MESSAGE_PUT` moves on to the message after it.
- Actors are moved by number: `OBJ_POS_SET`, `OBJ_WALK`, `OBJ_SPEED`,
  `OBJ_DIR`, `OBJ_FACE`, `OBJ_MODE` (an animation: `ANIM_STAND`, `ANIM_WALK`,
  `ANIM_NOD`, `ANIM_SHAKE_HEAD`).
- `FLAG_CHECK` and `FLAG_SET` test and set game flags; `SE(n)` plays a sound.

Names follow the rest of the project: readable C in English, as pret's
decompilations write it, with pret's naming (`PascalCase` functions,
`gName` globals).

## Limits

- An overlay is loaded into `gOverlayArea`, 32 KB of EWRAM
  (`games/tbs/src/system/ewram.s`): its code, tables and RAM variables must
  fit there.
- An overlay calls the main image only through its import table, never
  another overlay.
- The field work holds 66 actor slots (`games/tbs/include/field_runtime.h`):
  the party's 8, then the placed actors.
- A scene has 256 flags of its own (`FLAG_SCENE_FIRST` to `FLAG_SCENE_LAST`),
  cleared on every ordinary entrance.

## Evidence and open questions

A scene's name is its place's name in the English release, from the text its
people say and the place names the game shows; its functions are named for
what the player sees there; its messages are linker symbols made from their
English text. Overlay numbers are their rows of the resource directory.

Still unknown:

- XXX: many functions in `common/` keep older descriptive names
  (`SceneActor_`, `SceneEffect_`, `OverlayObject_`, `FieldScene_`) until the
  scenes that call them are renamed with them.
- XXX: sounds without a proven name keep their numbers (`SE_188`).
