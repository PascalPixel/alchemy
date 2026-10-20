# Field objects

This folder holds The Broken Seal's code for a field scene and what plays in
it: the main loop that moves between scenes, random encounters, a scene's
actors and events, the leader's steps, using items and Psynergy on the
field, the commands scene scripts run, Djinn joining the party, the world
map screen, and the slot machine.

See also: [Field](../field/README.md), the engine these scenes run on;
[Scenes](../scene/README.md), the overlays that give each place its events;
and [Maps](../map/README.md).

## Files

| File | What it holds |
| --- | --- |
| `object.c` | the guide to the field object; it includes `object_internal.h`, then the parts under `object/` in ROM order |
| `psynergy_effects.c` | the guide to the field Psynergy and event object; it includes `psynergy_effects_internal.h`, then the parts under `psynergy_effects/` in ROM order |
| `*_internal.h` | each object's includes, and the macros, types and inline helpers two of its parts use; they place no bytes |
| `colorfx.c` | the slot machine (the reel game) and an LZ packer, linked in another bank |
| `fxdata.c` | the objects' constant tables: object scripts, curves and lookups |
| `battle_cue.c`, `zonedata.c` | the battle music of each encounter, and the world map's encounter zones and Djinni battles |
| `location_rules.s`, `scene_rules.s` | the encounter zones, music and place names of field scenes |
| `scripts.s`, `arcspark.s`, `swatch.s` | object scripts and pictures |
| `encounter.tsv` | the 104 encounter zones: rate, level, enemy groups and their weights, each row named by its place in the `zone` column |

The prototypes are in `games/tbs/include/unit/object/`. Each part opens with
a line saying what it holds: `head -2 object/*.c psynergy_effects/*.c` lists
them all. The link order of the folder's objects is in
`games/tbs/objects.tsv`.

## Words

- **field work**: `struct FieldWork`, the field's state in `gEventWork`
  (heap slot `HEAP_SLOT_EVENT`): the actor slots, the scene's events and
  the pending requests.
- **actor**: an object in one of the field work's 66 actor slots; 0 to 7
  are the party, 8 on the scene's placements. An actor id is a slot.
- **placement**: a row of a scene's placement table (`struct
  ScenePlacement`), from which its actors are made.
- **scene event**: a row of the scene's event table (`struct SceneEvent`):
  talking, touching, searching, items and Psynergy, by kind.
- **field kind**: the field effect of Psynergy, the `type` column of
  `games/tbs/src/game/actions.tsv`: 1 Move, 2 Mind Read, 3 Ply, 4 Force,
  5 Douse, 6 Frost, 7 Lift, 8 Reveal, 9 Halt, 10 Cloak, 11 Carry, 12 Growth,
  13 Catch, 14 Whirlwind and Gale, 15 Retreat, 16 Avoid; `enum FieldKind`
  (`FIELD_KIND_CATCH`) in `unit/object/psynergy_effects.h` names them, and
  the effect scene keeps it as its `animation`.
- **effect scene**: `struct BattleFxScene` in `gEffectWork` (heap slot
  `HEAP_SLOT_FX`, 56): the Psynergy being used, its caster and target, and
  its 24 effect slots.
- **effect slot**: a `struct EffectSlot`, a particle an effect moves and
  draws (`psynergy_effects/effect_slot.c`).
- **marker**: a chest or Psynergy stone the map places at a marked cell.
- **world map**: `work->mode == MODE_WORLD_MAP`.

The same locals carry the same names: `work` for the field work (or a
part's own work block), `scene` for the effect scene, `object` and
`actor` for an actor, `leader` for the party's leader.

## Prefixes

Functions are named for what they act on: `Scene_`, `Encounter_`, `Field_`,
`Event_` and `FieldEvent_` (scene script commands), `Object_`,
`ObjectMotion_`, `ObjectTable_`, `Marker_`, `Camera_`, `Djinn_`,
`DisplayTransition_`, `WorldMap_` (entering and leaving world map events),
`EffectSlot_`, `ReelGame_`, and the prefixes of the
engine's other families. Field Psynergy effects are `FieldFx_`, a kind's
runner named for its Psynergy (`FieldFx_RunCatch`). `BattleFx_`,
`BattleEffect_` and `RunBattleEffectNN` here are older names for field
Psynergy code that other folders or ⚓️'s twins also use; they keep them
until a rename across folders.

The spellings some sites keep to match (the field work, the effect scene and
the map's view are each reached three ways), and the comment markers, are in
[Matching](../MATCHING.md).

## Design

### A scene runs as a loop of requests

`Field_RunScene` ([`object/run_scene.c`](object/run_scene.c)) sets a scene
up, runs its initialization, fades it in, and then alternates between two
states. While nothing is pending, it hands the leader to the player and
calls `Field_ProcessStep` once a frame. Anything that should interrupt
walking (the leader stepping on a cell with an event, a random battle, the
A button, the menu, an exit) does not act at once: it raises a request,
which is a field of the field work (`work->exit`, `work->event`,
`work->battle` and the rest), and walking stops as soon as
`Field_CountRequests` is not zero.

The loop then handles the requests one at a time, in a fixed order: the exit
(`exit`), a raised event (`event`), a random battle (`battle`), a touched or
exit cell (`action`), a pushed tile (`input`), the A button (`examine`), the
menu (`menu`), talking to an actor (`descriptor`), searching a tile
(`tile`), Psynergy (`command`), an item (`item`) and the pause menu
(`start`). After each one it starts again from the top, so an exit always
wins: when an event's script takes an exit, the scene ends before any other
request that was raised with it is handled. The exit's kind is what
`Field_RunScene` returns, and `Scene_TakeExit` loads the next scene from it.

A random battle is an exit too. Its handler snapshots the actors
(`ObjectTable_Snapshot`), sets the next scene to `SCENE_BATTLE` and raises
the exit `EXIT_NONE` (999), so the next pass of the loop leaves the scene;
the battle runs as a scene of its own, and the field scene is loaded again
afterwards, with the party where it was.

Because requests are fields, not a queue, each kind holds at most one, and
raising one twice before it is handled is the same as raising it once. An
event's value is the event to run, or a negative code: -1 when characters
fell to poison or hazardous ground, -888 to show the world map picture
(`Map_ShowWorldMap`) and -889 for `BattleFx_RunVisibilityTransition`.

This shape is why scene code can be written as straight-line cutscenes: a
handler runs a script to its end, waiting with `WaitFrames` as it goes, and
the walking loop resumes only when every request is done. What must move
during a cutscene runs as tasks and object scripts (see
[Runtime](../runtime/README.md) and [Field](../field/README.md)).

## How to find a field Psynergy

`Field_UsePsynergy` ([`object/psynergy.c`](object/psynergy.c)) pays the PP,
runs the scene's Psynergy events and plays the effect of the ability's field
kind: [`psynergy_effects/dispatch.c`](psynergy_effects/dispatch.c) runs the
kind's handler, cast with no target (`BattleFx_Run`) or on one
(`BattleFx_DispatchRequestKind`). Each kind's
effect is the part named for it, such as `psynergy_effects/reveal.c`, and
each of its entry points opens with the abilities that reach it:

```c
// Abilities: Catch (field kind 13), cast on a target.
```

## How to change a place's random battles

A field scene's zones come from `gBattle_LocationRules`
(`location_rules.s`): the first rule matching the scene and entrance gives
the zone of ground 1, and grounds 2 to 7 take the zones after it. The world
map's come from `gWorldMap_EncounterRules` (`zonedata.c`), by the terrain
under the leader. The zone is a row of `encounter.tsv`, whose `zone` column
names the place:

- `rate` is how often battles come (0 for none), and `level` the party
  level the zone is made for: each level the party's average is above it
  raises the rate by 5, up to five levels, and with Avoid active such a
  zone has no battles (`Encounter_SelectEnemyGroup`,
  [`object/encounter.c`](object/encounter.c)).
- `group0` to `group7` are enemy groups, rows of
  `games/tbs/src/battle/formatn.tsv`, and `weight0` to `weight7` each
  group's relative chance.

Zones with rate 0 are fought only by slot: the scripted battles a scene
asks for (`Field_RequestBattleFromEncounterTable`), the mimic chests (zone
99) and the Djinni battles (zones 100 to 103, `gEncounter_AreaEntryTable`).

## Limits

- 104 encounter zones of eight groups each.
- The field work holds 66 actor slots; the effect scene 24 effect slots.

## Evidence and open questions

The field kinds are named from the `type` column of `actions.tsv` and the
Psynergy the English release gives each; the requests from what the loop
does with each; the encounter columns from what `Encounter_SelectEnemyGroup`
computes with them. The zone names in `encounter.tsv` are the places whose
rules select them.

Still unknown:

- XXX: `Field_CountRequests` adds the requests together (`descriptor`
  twice) and tests the sum. A negative event code raised together with a
  positive request could sum to zero and stop the loop from seeing either;
  whether that ever happens is not known.
- XXX: the request fields' names (`descriptor` for talking, `command` for
  Psynergy, `input` for a pushed tile) say less than their handlers do; they
  are shared with other folders and wait for a rename.
- XXX: the `BattleFx_`, `BattleEffect_` and `RunBattleEffectNN` names on field
  Psynergy code are older than the `FieldFx_` prefix and wait for a rename
  across folders.
