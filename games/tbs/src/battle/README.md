# Battles

This folder holds The Broken Seal's battle code: a battle from its opening
transition to its spoils, the commands the enemies choose, what each action
does to its targets, and how the battle shows it. The effects drawn over a
battle are in `../effect`; the command menu and the party's status window are
in `../window`; the arithmetic of damage and success rates is in `../game`.

See also: [Battle effects](../effect/README.md), what is drawn while an
action plays, and [Game rules](../game/README.md), the tables abilities read.

## Files

| File | What it holds |
| --- | --- |
| `battle.c` | the guide; it includes `battle_internal.h`, then the parts below in ROM order |
| `battle_internal.h` | the includes and the names two or more parts use; each part includes it too |
| `battle/*.c` | the code, in 24 parts; never compiled on their own |
| `battle_data.s`, `battle_vars.s`, `advarrow.s` | the battle's tables and pictures |
| `battle_calc.s` | the fall-off tables: each damage and healing amount by the target's distance from the aimed one |
| `*.tsv` | tables the build encodes: `actflags.tsv` each ability's effect and staging, `actstat.tsv` how each ability's outcome is shown, `formatn.tsv` the 380 enemy groups (formations), [`sprites.tsv`](sprites.tsv) each enemy class's battle sprite, `presets.tsv` the debug party presets |
| [`../game/game/battle_calc.c`](../game/game/battle_calc.c) | damage and success rates: the attack, Psynergy and healing formulas, the element multiplier, the effect rolls and the weapon unleash |

The functions are declared in
[`unit/battle/battle.h`](../../include/unit/battle/battle.h). The object's
link order is in `games/tbs/objects.tsv` (from `battle`); the order of the
parts is in `battle.c`, and each part's first lines say what it holds:
`head -3 battle/*.c` lists them.

## Words

- **unit** and **unit id**: a fighter, party (ids 0 to 7) or enemy (ids from
  `FIRST_ENEMY_UNIT`, 128); `UnitGetFar` returns its `struct BattleUnit`.
- **ability** and **ability id**: everything a unit can do on its turn is an
  ability, a row of `games/tbs/src/game/actions.tsv` (`struct BattleAct`,
  `BattleActGet`) and the same row of `actflags.tsv`; both name the row in
  their `ability` column.
- **kind**: the low four bits of an ability's `flags` (`ABILITY_KIND_MASK`,
  `enum AbilityKind`), which pick the formula its targets take.
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
- **pose**: what `ObjectDispatch_SelectAnimationFar` sets on a battle
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

The spellings some sites keep to match, and the `FAKEMATCH` tags, are in
[Matching](../MATCHING.md).

## Design

### Outcomes are worked out while the effect plays

An effect is a function that keeps the CPU until it is over (see
[Battle effects](../effect/README.md)), so the battle cannot show a hit or a
message from its own code while the effect runs. It does that from a task
instead. `BattlePresentation_DispatchAction` ([`battle/staging.c`](battle/staging.c))
schedules `BattleEvent_Playback` ([`battle/playback.c`](battle/playback.c)),
then runs the effect, then waits in `BattleEventRuntime_WaitForReady` until
playback is done.

Playback is idle until the effect says the blow lands: most effects call
`EFFECT_SHOW_DAMAGE(hitSound)` at that frame (56 of the 66 parts do), which
starts it with the hit's sound. If an effect never does, the wait after it
starts playback instead. From then on, once a frame, the task works through
the targets one at a time: it runs `BattleTargetExec` for the next target,
which changes the target's HP, statuses and the rest and queues what should
be shown, and then plays that target's queue over the following frames
before it moves to the next.

The queue (`struct BattleEventQueue`, up to 64 events) is the boundary
between the rules and the show. `BattleTargetExec` never draws or waits; it
pushes events with `BattleEvPush`: the names and numbers a message needs,
then the message, then the flinch, the defeat, the status redraw, a sound.
The order is the order the player sees them, and a message's arguments
always come before it, because the message takes them from the window
system's argument table when it is decoded. The same queue can also be
played at once, without frames between events, by `BattleEv_DispatchQueued`,
which is how outcomes outside an effect are shown: a cast that is blocked or
lacks PP, for example (`staging.c`).

So the effect decides when damage appears, the rules decide what it is, and
neither waits for the other: the arithmetic of a later target happens while
the effect is still drawing, and only once its turn in the queue comes.

### Functions that return a value no caller reads

Some native helpers are declared to return an integer or pointer and end
without a `return`, or with a bare one; their callers use their side effects.
Falling off the end is undefined only when a caller uses the missing result.
The three helpers that formerly forwarded such a result now just call the
helper, so none reads a value that was never produced.

Changing the return type can change the return-address register: agscc's
`pop {r1}; bx r1` preserves r0, while `pop {r0}; bx r0` does not. Matching
keeps the existing type only where the `void` form changes the assembly;
`BattleEventRuntime_Reset` is `void` in every build. `UBFIX`, enabled by the
modern build, gives the remaining side-effect helpers `void` contracts.

### One object, seen through several structs

A battle unit's sprite on the field is one of the object slots the field's
object code hands out (`Object_CreateFar`). The battle reads it as a
`struct MotionObject`, while the field's far calls take it as a
`struct DispatchObject` or `struct ObjectRuntime`, three views of the same
record at the same offsets. Calls into the field cast to the view the
prototype names.

## How a battle runs

`Battle_RunEncounter` ([`battle/encounter.c`](battle/encounter.c)) fights a
whole battle. Each round:

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
   `BattleEv_DispatchQueued` ([`battle/events.c`](battle/events.c)) plays at
   once.
5. The turn's end and the round's (`turn_end.c`): stat changes and statuses
   count down, Djinn recover, poison and the Grim Reaper strike.

## How an ability resolves

Every command becomes an ability id, and the id is a row number in the
ability tables. To change what an ability does, find its name in the
`ability` column of `games/tbs/src/game/actions.tsv` and follow the steps
below.

1. **The command chooses the id** (`BattleCommand_BuildPlan`, `plan.c`):
   - `CMD_ATTACK`: ability 1, Attack, unless the equipped weapon unleashes
     (`RollWeaponUnleashFar`): then its row of `items.tsv` gives the
     ability, `unleash_action`, at `unleash_chance` times 5 percent plus the
     equipment's unleash bonus.
   - `CMD_PSYNERGY`: the ability chosen in the menu; its `pp_cost` is paid
     here.
   - `CMD_DJINN`: `Djinn_GetAbilityIdFar(element, index)` reads the Djinni's
     row of `games/tbs/src/game/djinn.tsv` (20 rows to an element, Venus
     first) and returns its `ability` column: 300 + 20 × element + index.
   - `CMD_ITEM`: the item's row of `games/tbs/src/game/items.tsv` gives the
     ability in `action_id` (0, or a broken item, makes the user defend).
     Its `use_type` says what using it costs: 1 one is used up, 2 it may
     break, 4 it leaves an Empty Bottle (`ITEM_USE_` in
     `battle_internal.h`); 3 is an item that teaches its ability while
     equipped (`../game/game/levels.c`).
   - `CMD_SUMMON`: the summon's row of `games/tbs/src/game/summons.tsv`
     gives the ability, once enough Djinn of each element are on standby.
   - `CMD_ABILITY`: an enemy's, one of `action0` to `action7` in its row of
     `games/tbs/src/game/enemies.tsv`, chosen by `enemy_ai.c`.
2. **The plan reads the ability's rows.** `BattleActGet` returns its row of
   `actions.tsv` as a `struct BattleAct`: the `element` it strikes with (4
   for none; Attack takes the weapon's), the `effect` that may add a second
   or third blow or a critical hit, its `range` and `power`. Its row of
   `actflags.tsv` (`gBattle_ActionFlags`) chooses the effect drawn
   (`../effect`), and `actstat.tsv` (`gBattle_ActionStatus`) how the outcome
   is staged.
3. **Each target takes it** in `BattleTargetExec` (`target_exec.c`). First
   the ability's `effect` is rolled (`BattleHitCheck`); then its kind picks
   the formula:

   | Kind (`flags & 15`) | Amount |
   | --- | --- |
   | 3 attack | `BattleAtkCalc(attack, defense, power, element bonus)` |
   | 4 multiplied attack | `BattleAtkCalc(attack, defense, 0, element bonus) * power / 10` |
   | 5 damage, 6 unleashed damage, 8 summon | `BattlePowerCalc(power, element bonus, 256)`; a summon adds 3 to 12 percent of the target's maximum HP |
   | 2 effect | `BattlePowerCalc` as above, only when the effect lands |
   | 10 PP damage | `BattlePowerCalc` as above, taken from PP |
   | 1 heal HP, 11 heal PP | `BattleRecoveryCalc` from the healer's element power |

   The element bonus is the actor's power in the ability's element less the
   target's resistance to it. Each amount then falls off with the target's
   distance from the aimed one (the tables in `battle_calc.s`), and Defend
   reduces damage.

The battle code calls the formulas by their far-call names, which
`games/tbs/src/system/far_call/GAME.S` exports for the functions in
[`../game/game/battle_calc.c`](../game/game/battle_calc.c):

| Called as | Function | What it computes |
| --- | --- | --- |
| `BattleAtkCalc` | `Battle_CalcAttackDamage` | attack less defense, plus twice the power, times half the element multiplier |
| `BattlePowerCalc` | `Battle_CalcPsynergyPower` | power times the element multiplier |
| `BattleRecoveryCalc` | `Battle_CalcRecovery` | power times the multiplier for the healer's element power |
| `BattleHitCheck` | `BattleFx_RollSuccess` | whether an effect takes hold: its base rate, the element lead, luck and weakness |
| `RollWeaponUnleashFar` | `RollWeaponUnleash` | whether the weapon unleashes |

## Limits

- 519 abilities, ids 0 to 518: the plan reads `actflags.tsv` only for those
  ids, and `actstat.tsv` up to 517.
- A plan reaches at most 14 targets (`TARGET_MAX`, `plan.c`).
- Party unit ids are 0 to 7 and enemy unit ids start at 128.
- 380 enemy groups in `formatn.tsv`; the encounter zones in
  `games/tbs/src/object/encounter.tsv` choose among them.

## Evidence and open questions

The abilities, enemies and groups are named from the `ability`, `enemy` and
label columns of their tables, which follow the English release's text; the
formulas from what `battle_calc.c` computes; the event opcodes from what
playback does with each.

Still unknown:

- XXX: several older names say too little or the wrong thing (the `Summon_`
  getters read the enemy class table); renaming them crosses folders.
- XXX: bits 0 and 1 of an ability's staging flags (`STAGING_ON_DOWNED`,
  `STAGING_OTHER_SIDE`) are read but no row of `actflags.tsv` sets them.
- XXX: what reaches `BattleEvent_Playback` for the ten effect parts that never
  call `EFFECT_SHOW_DAMAGE`, beyond the wait after the effect.
