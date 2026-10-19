# Scenes

This folder holds The Broken Seal's scenes: the code and tables of every place
the party can walk around in, one overlay each. A scene places the people and
things of a map, says what happens when the party talks to someone, touches a
spot, searches or uses Psynergy, and plays its cutscenes. The field engine (in
`games/tbs/src/field` and `games/tbs/src/object`) runs the scene; the scene
drives it through the engine's services.

## Files

| File | What it holds |
| --- | --- |
| `<place>.c` | one scene overlay: its code, then its tables |
| `<place>.h` | the scene's own names (actors, flags, structures), where it has any |
| `entry/<place>.s` | the overlay's entry table: six veneers the engine calls |
| `import/<place>.s` | the overlay's import table: the main-image functions it calls |
| `common/*.h` | code that several overlays carry, each in its own copy |

The prototypes other files see are in `games/tbs/include/unit/scene/`. The
command macros are in `games/tbs/include/event.h`, the engine's services in
`field_event.h` and `field_service.h`, and the table formats in
`field_scene.h`.

Each overlay is three objects in `games/tbs/objects.tsv`, linked in that order
at 0x2008000: `entry/<place>`, `<place>` and `import/<place>`. Its first
column is the overlay's resource number. The scene table,
`gField_SceneTable` in `games/tbs/src/field/scenes.s`, gives each scene its
overlay, its group (the place it belongs to), its variant and its map, and names
it `SceneId_<Place>`; several scenes may share one overlay (Bilibin's three,
for one). A scene is one object: it has no parts.

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
entry: `Vault_SelectEvents` returns `gVault_EventsAfterThieves` once the
thieves are in jail.

Then the party walks until something happens: an event runs, or the party
takes an exit and the next scene is loaded.

## The tables

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

## Finding a scene and an event in it

1. Find the place in `scenes.s`: `scene Vault, 0x382, ...` runs overlay
   0x382, and the rows of `objects.tsv` that begin `382` name its files
   (`vault.c`). Scenes that share an overlay (`GomaCaveWaterway1` and
   `GomaCaveWaterway2`) share one file.
2. To find what someone says, search `games/tbs/text/en.po` for the line; its
   `msgctxt` is the message's linker name (`MsgVaultWelcomeToVaultTheTownAt`),
   which the scene uses as written.
3. The scene's event table names the function or message for each actor and
   spot; its actors are numbered from 8 in the order of the placement table.

## Writing a scene

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

Each site keeps the spelling it matches with: a command and the service it
names can compile differently.

## Words and conventions

- **actor**: a field object a scene addresses by number. 0 to 7 are the
  party's characters (`ACTOR_PARTY_LEADER` is 0, the one the player moves),
  8 on are the placed actors (`ACTOR_FIRST_PLACED`).
- **position**: 16.16 fixed point pixels, x across, y up and z down the map;
  scenes write pixels (`PIXELS(n)`, or plain pixels to a command). A **cell**
  is 16 by 16 pixels (`x >> 20`).
- **facing**: a 16-bit turn, `FACING_EAST` 0 to `FACING_NORTHEAST` 0xe000,
  clockwise as seen on the screen.
- **flags**: game flags, from `flags.h` where a name is proven. 0x200 to 0x2ff
  belong to the scene and are cleared on every ordinary entrance
  (`FLAG_SCENE_TEMP_xx`, with what each scene uses it for); 0x300 to 0x3ff
  belong to the group of scenes of one place. `FLAG_KEEP_PARTY_POSITION` is
  set when the party returns to where it was, as after a battle: a scene's
  initialization skips its arrival cutscenes then.
- **messages** are linker symbols written by bare name; `MSGNO(name)` is the
  number, and a message's successor is the next line said.
- **sounds** come from `songs.h`: `SE_` and `MUS_` names where the sound is
  known, its number otherwise (`SE_188`).
- **callbacks**: functions the scene adds with `FieldScheduler_AddCallback`
  (priority 0xc80, `TASK_PRIORITY_SCENE`), run once a frame until removed.
- **update hooks**: an actor's `update`, run each frame with the actor.

Names follow the rest of the project (AGENTS.md, S3): a scene's functions are
`<Place>_` (`Vault_`, `VaultRevisit_`, `ColossoLog_`) and say what the player
sees, from the English release's names and text; an unknown keeps a
placeholder. The shared helpers take their family's prefix (`Colosso_`,
`MercuryLighthouse_`, `KolimaPalette_`, `StagedActor_`, `Effect_`); some still
carry older descriptive names (`SceneActor_`, `SceneEffect_`, `OverlayObject_`,
`FieldScene_`) until the scenes that call them are renamed with them.

## Code that several scenes share

An overlay can only call the main image, so code that several overlays need
is in each of them. Where the same code appears in several overlays it is
written once, in `common/`, and each scene includes it at the place its bytes
are, under the line `// Shared source is included here in ROM order.` Each
header opens by saying which scenes include it. A header in `common/` is code,
not declarations: including it twice in one scene would place it twice.

The headers are named after the place or the job they share:

| Headers | Scenes | What they hold |
| --- | --- | --- |
| `colosso_*.h`, `item_equip.h` | the Colosso finals' courses (Colosso Log, River and Wall) | the friends' cheering spots, the stage guides, the race gauge, the rival's recorded path, the guide's pointer and the start lettering |
| `vault_*.h` | Vault and its interiors | the walking villagers, the Bone, the doorway, Ivan's farewell |
| `mercury_lighthouse_*.h` | Mercury Lighthouse and its entrance | the pool's lights that carry the leader over it, the passages between floors, the pool's shimmer |
| `kolima_*.h` | Kolima and its forest | the cursed forest's colours, the dust of Tret's curse, the hop onto the river's actor |
| `sol_sanctum_*.h` | Sol Sanctum's rooms | the Elemental Star room's floating lights, the corridor's and maze's entry |
| `vale.h`, `world_map_linked_effects.h` | Vale's scenes, the world map | the storm's door exits, the particles around a glowing actor |
| `effect_*.h` | many | Effect_Spawn and the small effects it places, cutscene Psynergy |
| `object_*.h` | many | actors: pushed blocks and rolling logs, the floating Nut, lookups by cell and distance, turning and waiting |
| `map_panel.h`, `scroll_initialize_hblank_dma.h` | Crossbone Isle and Kolima; the Lemurian ship | a stretch of map drawn into the tile map; the water's ripple |

A header named after one place may be shared further: `colosso_item_icon.h`
also draws Crossbone Isle's Red Key.

Renaming a function in `common/` renames it in every scene that includes the
header, and in the scene's entry table or event table when it is listed
there.

## Spellings that match at different sites

Some sites match only with a plain-C equivalent of the usual spelling, and
keep it without a FAKEMATCH tag; each says so in a comment:

- A field read or written through its offset, `*(T *)((u8 *)p +
  offsetof(struct FieldActor, field))`, where `p->field` goes through
  FieldActor's single/group union and moves a load (`effect_spawn.h`,
  `mercury_lighthouse_pillar.h`).
- Another struct's view of the same object (`struct SolSanctumRingObject`
  for a FieldActor) in `sol_sanctum_elemental_star_ring_setup.h`.
- A byte store through a `u8 *` and `offsetof` in place of a struct field
  (`Colosso_RestoreCompetitor`).
- `gMapWork[0]` read as a `u32` (`SceneActor_MoveAndRedraw`).

An update hook that the bytes show returning a value keeps its `s32` return,
with the `void` form the hook type wants behind `UBFIX` (`// UB:` above it).
