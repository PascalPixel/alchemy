# Field objects

This folder holds The Broken Seal's code for a field scene and what plays in
it: the main loop that moves between scenes, random encounters, a scene's
actors and events, the leader's steps, using items and Psynergy on the
field, the commands scene scripts run, Djinn joining the party, the world
map screen, and the slot machine.

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
| `encounter.tsv` | the encounter zones: rate, level, enemy groups and their weights |

The prototypes are in `games/tbs/include/unit/object/`. Each part opens with
a line saying what it holds: `head -2 object/*.c psynergy_effects/*.c` lists
them all. The link order of the folder's objects is in
`games/tbs/objects.tsv`.

## Words used here

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
`DisplayTransition_`, `EffectSlot_`, `ReelGame_`, and the prefixes of the
engine's other families. Field Psynergy effects are `FieldFx_`, a kind's
runner named for its Psynergy (`FieldFx_RunCatch`). `BattleFx_`,
`BattleEffect_` and `RunBattleEffectNN` here are older names for field
Psynergy code that other folders or ⚓️'s twins also use; they keep them
until a rename across folders.

## Spellings

One idea is written in several ways, and the sites have not all been
measured; each keeps its spelling until it is:

- the field work: `(struct FieldWork *)gEventWork`,
  `WORK_SLOT(HEAP_SLOT_EVENT)`, or `Runtime_AllocateBlock(HEAP_SLOT_EVENT,
  sizeof(struct FieldWork))`, which returns the existing block;
- the effect scene: `gEffectWork` or `*(gWorkSlot + HEAP_SLOT_FX)`;
- the map's view: `gMapWork[0]` or `WORK_SLOT(8)`.

Measured ones:

- `((16 - blend) << 8) | blend` in `world_map.c` is `BLEND_ALPHA(blend,
  16 - blend)` with its operands swapped; the macro changes the registers.

`/* FAKEMATCH(kind): ... */` marks a device that steers the compiler, with
the instructions that differ without it (AGENTS.md, S2).

## Finding a field Psynergy

`Field_UsePsynergy` (`object/psynergy.c`) pays the PP, runs the scene's
Psynergy events and plays the effect of the ability's field kind:
`psynergy_effects/dispatch.c` runs the kind's handler, cast with no target
(`BattleFx_Run`) or on one (`BattleFx_DispatchRequestKind`). Each kind's
effect is the part named for it, such as `psynergy_effects/reveal.c`, and
each of its entry points opens with the abilities that reach it:

```c
// Abilities: Catch (field kind 13), cast on a target.
```
