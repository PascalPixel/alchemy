# Battles

This folder holds The Broken Seal's battle code: a battle from its opening
transition to its spoils, the commands the enemies choose, what each action
does to its targets, and how the battle shows it. The effects drawn over a
battle are in `../effect`; the command menu and the party's status window are
in `../window`.

## Files

| File | What it holds |
| --- | --- |
| `battle.c` | the guide; it includes `battle_internal.h`, then the parts below in ROM order |
| `battle_internal.h` | the includes and the names two or more parts use; each part includes it too |
| `battle/*.c` | the code, in 24 parts; never compiled on their own |
| `battle_data.s`, `battle_vars.s`, `advarrow.s` | the battle's tables and pictures |
| `*.tsv` | tables the build encodes: `actflags.tsv` each ability's effect and staging, `actstat.tsv` how each ability is shown, `formatn.tsv` the 380 enemy formations, `sprites.tsv` each enemy class's battle sprite, `presets.tsv` the debug party presets |

The functions are declared in `games/tbs/include/unit/battle/battle.h`. The
object's link order is in `games/tbs/objects.tsv` (from `battle`); the order
of the parts is in `battle.c`, and each part's first lines say what it holds:
`head -3 battle/*.c` lists them.

## How a battle runs

`Battle_RunEncounter` (`encounter.c`) fights a whole battle. Each round:

1. The actions are gathered: the party's from the command menu
   (`BattlePresentation_BuildActions`, `round.c`), the enemies' from
   `BattleCommand_SelectAutomatic` (`enemy_ai.c`) or the link opponent's, then
   sorted into turn order. Under `FLAG_AUTO_BATTLE` the actions come from
   `auto_battle.c` instead.
2. Each action is announced and planned (`BattleCommand_BuildPlan`,
   `plan.c`): its targets, hits, critical hits and how it is staged.
3. It is staged (`BattlePresentation_DispatchAction`, `staging.c`): an
   attack, a cast, an item, a summon or an escape, each with its effect from
   `../effect`.
4. Each target takes it (`BattleTargetExec`, `target_exec.c`). Nothing is
   shown there: it queues battle events (the message's arguments, the
   message, the hit, the downed unit), which `BattleEvent_Playback`
   (`playback.c`) plays over frames while the effect runs, or
   `BattleEv_DispatchQueued` (`events.c`) plays at once.
5. The turn's end and the round's (`turn_end.c`): stat changes and statuses
   count down, Djinn recover, poison and the Grim Reaper strike.

## Words used here

- **unit** and **unit id**: a fighter, party (ids 0 to 7) or enemy (ids from
  `FIRST_ENEMY_UNIT`, 128); `UnitGetFar` returns its `struct BattleUnit`.
- **place**: a unit's position in `BattleWork.partyUnits` or `enemyIds`. A
  **place code** is a place with 0x100 for the party or 0x180 for the
  enemies (`PLACE_PARTY`, `PLACE_ENEMY`), as a command's target.
- **battle object**: a unit's `struct BattleObj`, its sprite and motion on the
  battlefield (`BattleObjGet`); its **records** are its sprites.
- **side**: `BATTLE_SIDE_PARTY`, `BATTLE_SIDE_ENEMIES` or both, for the unit
  lists; a unit's own `side` field says whether it is present.
- **plan**: `struct BattlePlan`, the action under way; its **outcome** is how
  it is staged (`enum BattlePlanStaging`) and its **presentation flags** the
  effect and how it plays (`STAGING_*`, from `actflags.tsv`).
- **event**: one entry of the battle event queue (`BattleEvPush`).
- **pose**: what `ObjectDispatch_ApplyArgumentToChildrenFar` sets on a battle
  object (`POSE_*`).

The same locals carry the same names in every part: `battle` or `work` for
`gBattleWork`, `unitId`, `unit` for its `struct BattleUnit`, `battleObject`
for its `struct BattleObj`, `object` for that object's `struct MotionObject`,
`record` for one of its sprites, and `plan` for the `struct BattlePlan`.

## Prefixes

Functions are `Battle_` for the battle as a whole and take a family's prefix
otherwise: `BattleParty_` (the unit lists), `BattleObj`/`BattleActor_` (the
battle objects), `BattleMotion_` (their movements), `BattlePres_` and
`BattlePresentation_` (what the battle shows), `BattleTarget_`,
`BattleCommand_`, `BattleEvent_`/`BattleEv_`, `BattleUnit_`,
`BattleFormation_`, `BattleEnemy_`, `BattleEscape_`, `BattleFx_` (the star
field, as in `../effect`). Functions that act on one thing the engine names
elsewhere keep its prefix: `Camera_`, `Palette_`, `Graphics_`, `Runtime_`,
`Resource_`, `Item_`, `Party_`. Several names predate these rules and say
too little or the wrong thing (the `Summon_` getters read the enemy class
table); renaming them crosses folders, so each is its own commit.

## Plain-C spellings

Each site keeps the spelling it matches with:

- An action slot's first halfword is cleared through a cast,
  `*(u16 *)&member->actionSlots[i] = 0`, where the field store loads its
  zero differently (`debug.c`).
- `BLEND_ALPHA(16, 16) - step` and `step + BLEND_ALPHA(0, 16)` where the
  macro's `|` form changes the code (`background.c`).
- Work slots are read as `WORK_SLOT(n)`, as `gWorkSlot[n]`, or through
  `(void **)&gCameraWork` and `(void **)&gBattleWork` indexed past them,
  which is undefined behaviour with its defined form behind `UBFIX`.

`FAKEMATCH(kind)` tags mark S2's devices, each with its measured difference
in tbs-en: the instructions of the function that the plain form changes.
