# Battles

Every fight in The Broken Seal, from the opening transition to the coins and
experience at the end, is played out by the battle code.

Each round the battle works out who acts and in what order, what the enemies
choose, what each attack, Psynergy, Djinni, summon or item does to the units
it reaches, and how all of that is shown. Three other folders share the work.
The animations drawn over a battle are in
[Battle effects](../effect/README.md), the command menu and the party's status
window are in `../window`, and the arithmetic of damage and success rates,
along with the tables every ability reads, is in
[Game rules](../game/README.md).

## Files

| File | What it holds |
| --- | --- |
| `battle.c` | the battle object itself: it includes `battle_internal.h`, then the parts in `battle/` in the order their code has in the ROM |
| `battle_internal.h` | what the parts share: the includes and the names two or more parts use; each part includes it too |
| `battle/*.c` | the code, in 24 parts, each compiled only through `battle.c` |
| `battle_data.s`, `battle_vars.s` | the battle's smaller tables and pictures in their ROM order, including where the `.tsv` tables below land in the ROM |
| `advarrow.s` | the arrow that bobs beside a battle message while it waits for you, built from `advarrow.png`, which holds sixteen 8 by 16 frames; the code cycles through the first eight, one every four frames |
| `presentation/` | the opening curtain's tiles and their variants, and the brightness steps of a unit's highlight (`highlight_ramp.s`) |
| `effect/` | two 32 by 16 sparks the field Psynergy effects draw, which `../object/arcspark.s` places |
| `battle_calc.s` | the fall-off tables: how much of a damage, healing or effect chance a target gets by its distance from the aimed one |
| `actflags.tsv` | each ability's effect and how it's staged |
| `actstat.tsv` | how each ability's outcome is shown |
| `formatn.tsv` | the 380 enemy groups (formations) |
| [`sprites.tsv`](sprites.tsv) | each enemy class's battle sprite |
| `presets.tsv` | the debug party presets |
| [`../game/game/battle_calc.c`](../game/game/battle_calc.c) | damage and success rates: the attack, Psynergy and healing formulas, the element multiplier, the effect rolls and the weapon unleash |

The ROM shows the battle as a single object, so that's how it's built. The
split into 24 parts is ours, made where one family of functions ends and the
next begins. Each part opens with `/// @file` and a summary of what it holds
on the lines after it, so the first few lines of the parts read together as
a table of contents. The functions are declared in
[`unit/battle/battle.h`](../../include/unit/battle/battle.h), and the
object's place in the link order is in `games/tbs/objects.tsv`, from the
`battle` row.

## Words used here

- **unit** and **unit id**: everyone in a fight is a unit, a party member or
  an enemy. Party members keep their character numbers as ids, 0 to 7, and
  enemies take ids from 128 (`FIRST_ENEMY_UNIT`). `UnitGetFar` returns a
  unit's `struct BattleUnit`, with its HP, PP, stats and statuses.
- **ability** and **ability id**: everything a unit can do on its turn is an
  ability, from a plain Attack to Quake, a Djinni, a summon, an item's use or
  an enemy's special move. Each is a row of `games/tbs/src/game/actions.tsv`
  (`struct BattleAct`, read with `BattleActGet`) and the same row of
  `actflags.tsv`; both name the row in their `ability` column.
- **kind**: a number from 0 to 15 that picks the formula an ability's targets
  take: an attack, Psynergy damage, healing and so on. A byte is eight bits,
  eight on-or-off switches, and the kind is the low four bits of the
  ability's `flags` byte (`ABILITY_KIND_MASK`, `enum AbilityKind`).
- **place** and **place code**: where a unit stands in its side's line-up
  (`BattleWork.partyUnits` or `enemyIds`). A command aims at a place code: the
  place plus 256 for the party or 384 for the enemies, which the code writes
  in hexadecimal as 0x100 and 0x180 (`PLACE_PARTY`, `PLACE_ENEMY`).
- **battle object**: a unit's figure on the battlefield, its sprite and how
  it moves (`struct BattleObj`, `BattleObjGet`). Its **records** are its
  sprites; the Kraken and the Fusion Dragon are drawn in two parts, so they
  have two.
- **side**: the party or the enemies. A list of units can be asked for one
  side or both (`BATTLE_SIDE_PARTY`, `BATTLE_SIDE_ENEMIES`), and a unit's own
  `side` field says whether it's in the battle at all, and on which side.
- **plan**: the action under way, who acts, with which ability and at which
  targets (`struct BattlePlan`). Its **outcome** is how it's staged
  (`enum BattlePlanStaging`: messages only, an attack run, a cast, an item, a
  summon, a flight and so on), and its **presentation flags** say which effect
  plays and how (`STAGING_*`, from `actflags.tsv`).
- **event**: one thing the battle shows as an action plays out, waiting its
  turn in the battle event queue (`BattleEvPush`). It may be a message, a
  name or number the message needs, a unit flinching or going down, or a
  sound.
- **pose**: how a battle object stands: standing, taking damage, held or down
  (`POSE_*`, set with `ObjectDispatch_SelectAnimationFar`).

The same locals carry the same names in every part: `battle` or `work` for
`gBattleWork`, `unitId`, `unit` for its `struct BattleUnit`, `battleObject`
for its `struct BattleObj`, `object` for that object's `struct MotionObject`,
`record` for one of its sprites, and `plan` for the `struct BattlePlan`.

## Function prefixes

| Prefix | What it acts on |
| --- | --- |
| `Battle_` | the battle as a whole: `Battle_RunEncounter`, the round's end, the spoils |
| `BattleParty_` | the unit lists: who's in the battle and who's still standing |
| `BattleObj`, `BattleActor_` | the battle objects |
| `BattleMotion_` | their movements |
| `BattlePres_`, `BattlePresentation_` | what the battle shows: the staging, the opening transition, the camera and the palettes |
| `BattleCommand_` | choosing an action and planning it |
| `BattleTarget_` | choosing targets |
| `BattleEvent_`, `BattleEv_`, `BattleEventRuntime_` | the event queue and its playback |
| `BattleUnit_` | one unit's battle state: its guard, its weapon sprite, its status icons and the counters that tick at the end of its turn |
| `BattleFormation_`, `BattleEnemy_` | the enemy groups, and each enemy's level, name letter, defeat and coins |
| `BattleClass_` | an enemy class's row of the battle sprite table |
| `BattleEscape_` | running away |
| `BattleFx_` | the star field and a unit's elemental glow, as in `../effect` |
| `LinkBattle_` | link battles |
| `BattleSpoils_` | the items the enemies leave, ranked so the best are kept and handed out first |
| `BattleSummon_` | which summons the party's standby Djinn make available |
| `BattlePlacement_` | where each place stands on the battlefield |
| `BattleStatusIcon_` | the status icons that cycle over a unit |
| `BattleIntro_` | the message that announces the encounter |
| `DebugBattle_`, `DebugParty_` | the debug battle menu, its message viewer and the party presets |

A function that acts on one thing the engine names elsewhere keeps that
thing's prefix: `Camera_`, `Palette_`, `Graphics_`, `Runtime_`, `Party_`,
`DjinnRecovery_`, `Ability_`, `Dma_`, `Text_`.

A few older names say too little or the wrong thing; they're listed under
the open questions at the end.

[Matching](../MATCHING.md) explains the odd spellings and the `FAKEMATCH` tags.

## How a battle runs

A battle is a series of rounds. At the start of each you choose a command for
everyone in the party, the enemies choose theirs, and then everybody acts in
turn, fastest first. Each turn and each round end with their own
bookkeeping, and it all goes on until one side is gone or the party gets
away.

`Battle_RunEncounter` ([`battle/encounter.c`](battle/encounter.c)) fights the
whole battle and returns how it ended: the number of enemies defeated when
the party wins, or that the party ran (`ENCOUNTER_FLED`) or fell
(`ENCOUNTER_PARTY_LOST`). Along the way `formation.c` turns the enemy group
into units, how many of each kind with their levels and the letters that tell
two of a kind apart, `transition.c` plays the opening (the curtain, or the
Venus Lighthouse storm), and `spoils.c` hands out the coins, experience,
items and level ups at the end. Each round goes like this:

1. **The actions are gathered** (`round.c`). The party's come from the
   command menu (`BattlePresentation_BuildActions`), the enemies' from
   `BattleCommand_SelectAutomatic` (`enemy_ai.c`), or from the other console
   in a link battle. Under `FLAG_AUTO_BATTLE` they come from `auto_battle.c`
   instead. Then they're sorted into turn order by Agility, fastest first,
   except that Granite, Flash and Ground, the Djinn that guard the party or
   cost an enemy its turn, go before everyone (`BattleQueue_SortByPriority`).
2. **Each action is announced and planned** (`BattleCommand_BuildPlan`,
   `plan.c`): its targets, whether each is struck twice, three times or
   critically, and how it's staged.
3. **It's staged** (`BattlePresentation_DispatchAction`, `staging.c`): an
   attack, a cast, an item, a summon or an escape, each with its effect from
   `../effect`.
4. **Each target takes it** (`BattleTargetExec`, `target_exec.c`). Nothing is
   shown there. It queues battle events (the message's arguments, the
   message, the hit, the downed unit), which `BattleEvent_Playback`
   (`playback.c`) plays over frames while the effect runs, or
   `BattleEv_DispatchQueued` ([`battle/events.c`](battle/events.c)) plays at
   once.
5. **The turn ends, and then the round** (`turn_end.c`): stat changes and
   statuses count down, Djinn recover, poison takes a tenth of a unit's
   maximum HP at the end of its turn (deadly poison a fifth), and the Grim
   Reaper takes a cursed unit when its seven-turn countdown runs out.

An enemy picks one of the eight abilities in its row of
`games/tbs/src/game/enemies.tsv` by its `action_strategy`. Strategies 0 to 2
weight the eight evenly, falling by 6 a place, or falling steeply from 113
to 5 (`gBattleCommand_SelectionWeights`); 3 and 4 remember their place in
the list from one turn to the next. A choice it can't make (too little PP, a
seal, nobody worth aiming at) is drawn again, and after 18 tries the enemy
defends.

Sometimes one side gets the jump on the other. Unless a battle sets its own
start, the party strikes first one time in 16, and always in the Djinn
tutorial: no enemy acts in the first round, and running away can't fail.
Otherwise the enemies strike first one time in 32: the party chooses no
commands in the first round, and at most two to five enemies, drawn at
random, act once each. A failed escape gives the enemies that advantage for
the rest of its round (`BattleWork.encounterMode`).

Running away (`BattleEscape_CheckSuccess`, `round.c`) starts at a 50 percent
chance, gains 20 for every failed try in the same battle, and moves 5 for
each level the party's average is above or below the enemies'. It never
works in a battle you can't run from (`BATTLE_START_NO_ESCAPE`).

## How an ability resolves

Whatever you pick in the menu, the battle turns it into an ability id: Attack
is ability 1, Quake 3, Flint 300. The id is a row number, the same row in
every ability table, so any move can be followed from the menu to the damage
it does by reading its rows. Quake, for example, is row 3 of
`games/tbs/src/game/actions.tsv`. It aims at foes (`target_mode` 1), strikes
with Venus (`element` 0), reaches the aimed enemy and one either side
(`range` 2), costs 4 PP and has a `power` of 12. Its `flags` are 0x85, which
says it can be used in battle (bit 7, the top one) and, in the low four bits,
that it's kind 5, Psynergy damage.

1. **The command picks the id** (`BattleCommand_BuildPlan`, `plan.c`):
   - **Attack** (`CMD_ATTACK`) is ability 1, unless the equipped weapon
     unleashes (`RollWeaponUnleashFar`). Then the weapon's row of `items.tsv`
     gives the ability in `unleash_action`, at a chance of `unleash_chance`
     times 5 percent plus the equipment's unleash bonus.
   - **Psynergy** (`CMD_PSYNERGY`) is the ability chosen in the menu. Its
     `pp_cost` is paid here, and only when the cast goes ahead: a sealed
     caster, or one short of PP, pays nothing.
   - **Djinn** (`CMD_DJINN`): `Djinn_GetAbilityIdFar(element, index)` reads
     the Djinni's row of `games/tbs/src/game/djinn.tsv` (20 rows to an
     element, Venus first) and returns its `ability` column, which is
     300 + 20 × element + index. Only a set Djinni is unleashed, and it then
     goes on standby. One on standby is set again instead, and one still
     recovering says so.
   - **Items** (`CMD_ITEM`): the item's row of
     `games/tbs/src/game/items.tsv` gives the ability in `action_id`. An item
     with none (0), or a broken one, makes the user defend. Its `use_type`
     says what using it costs: 1 one is used up, 2 it may break, 4 it leaves
     an Empty Bottle (`ITEM_USE_` in `battle_internal.h`); 3 is an item that
     teaches its ability while it's equipped (`../game/game/levels.c`).
   - **Summons** (`CMD_SUMMON`): the summon's row of
     `games/tbs/src/game/summons.tsv` gives the ability, once enough Djinn of
     each element are on standby. The plan then marks the Djinn it uses.
   - **Enemies** (`CMD_ABILITY`) use one of `action0` to `action7` in their
     row of `games/tbs/src/game/enemies.tsv`, chosen by `enemy_ai.c` as
     described above.
2. **The plan reads the ability's rows.** `BattleActGet` returns its row of
   `actions.tsv` as a `struct BattleAct`: the `element` it strikes with (4
   for none; Attack takes the weapon's, and an enemy's attack its own), the
   `effect` that may add a second or third blow or a critical hit, its
   `range` and its `power`. A `range` of 1 is the aimed unit alone, each step
   more adds one unit on either side, and 255 reaches the whole side. The
   ability's row of `actflags.tsv` (`gBattle_ActionFlags`) chooses the effect
   drawn (`../effect`), and `actstat.tsv` (`gBattle_ActionStatus`) how the
   outcome is staged.
3. **Each target takes it** in `BattleTargetExec` (`target_exec.c`). First
   the ability's added `effect` is rolled (`BattleHitCheck`), and then its
   kind picks the formula:

   | Kind | Amount |
   | --- | --- |
   | 3 attack | `BattleAtkCalc(attack, defense, power, element bonus)` |
   | 4 multiplied attack | `BattleAtkCalc(attack, defense, 0, element bonus) * power / 10` |
   | 5 damage, 6 unleashed damage, 8 summon | `BattlePowerCalc(power, element bonus, 256)`; a summon first adds 3 percent of the target's maximum HP to the power for each Djinni it costs, so 3 to 12 percent |
   | 2 effect | `BattlePowerCalc` as above, only when the effect lands |
   | 10 PP damage | `BattlePowerCalc` as above, taken from PP |
   | 1 heal HP, 11 heal PP | `BattleRecoveryCalc` from the healer's element power |

   Kinds 0, 7 and 9 apply no power at all. Kind 0 is only the 128 unused
   rows named `?` (row 0, Nothing, is kind 5). Kind 7 is Granite, Flash and
   Kite, whose effect is all they do. Kind 9 covers the field Psynergy,
   Defend, Total Defense, Stand Ready, Flee, the enemies' commands, and the
   stat-raising items: Power Bread, Cookie, Apple, Hard Nut, Mint and Lucky
   Pepper.

The element bonus is the actor's power in the ability's element less the
target's resistance to it, with an item's user always counted at power 100.
A miss makes the blow's multiplier 0, and a double or triple blow makes it 2
or 3. Attacks, HP healing and Psynergy, unleash and summon damage then add a
random 0 to 3, and a guarding target takes half the damage (Defend, Granite)
or a tenth (Total Defense, Flash).

Everything except an attack falls off with the target's distance from the
aimed one, in steps of one unit, using the tables in `battle_calc.s`. An
ability that reaches the whole side treats every target as the aimed one.
The figures are percentages:

| Steps from the aimed unit | 0 | 1 | 2 | 3 | 4 | 5 |
| --- | --- | --- | --- | --- | --- | --- |
| Psynergy damage (kind 5), HP and PP healing (1, 11) | 100 | 80 | 60 | 40 | 20 | 10 |
| Summon damage (8) | 100 | 70 | 40 | 30 | 20 | 10 |
| Unleashed damage (6), effect damage (2), PP damage (10) | 100 | 50 | 30 | 10 | 10 | 10 |
| The added effect's chance | 100 | 60 | 30 | 30 | 30 | 30 |

The battle code calls the formulas by their far-call names, which
`games/tbs/src/system/far_call/GAME.S` exports for the functions in
[`../game/game/battle_calc.c`](../game/game/battle_calc.c):

| Called as | Function | What it works out |
| --- | --- | --- |
| `BattleAtkCalc` | `Battle_CalcAttackDamage` | Attack less Defense (never below 0) plus twice the power, the whole sum halved, then scaled by the element multiplier at half strength |
| `BattlePowerCalc` | `Battle_CalcPsynergyPower` | the power times the element multiplier |
| `BattleRecoveryCalc` | `Battle_CalcRecovery` | the power times the multiplier for the healer's element power |
| `BattleHitCheck` | `BattleFx_RollSuccess` | whether an effect takes hold: its base rate, the element lead, luck and weakness |
| `RollWeaponUnleashFar` | `RollWeaponUnleash` | whether the weapon unleashes |

The element multiplier (`Battle_GetElementMultiplier`) is where elements
matter. An even match is 1. Each point the actor's power is ahead of the
target's resistance adds half a percent, up to 2 at 200 points ahead, and
each point behind takes half a percent off, down to 0 at 200 behind. Attacks
feel the element at half strength, from 0.5 to 1.5. Healing goes by the
healer's own element power instead: one percent more or less for each point
above or below 100, from nothing at 0 to double at 200. An item, or an
ability with no element, heals as if at 100.

An added effect's chance starts from its base rate (Sleep 45 percent, Poison
55, Condemn's instant death 20 and so on, in `BattleFx_GetBaseSuccessRate`),
gains 3 for each element level the caster is ahead of the target, loses 3
for every two points of the target's luck, and gains 25 against a weakness.
The distance table above then scales it. Quartz and Dew revive at a fixed 60
and 90 percent, and an effect with no rate of its own always works; levels,
luck and weaknesses don't change those. Luff gets three tries.

A plain attack can miss: one time in 32, and a deluded attacker also misses
153 times in 256. A target that can't act (asleep, stunned, charmed, held by
Ground, or down) is never missed. A critical hit comes one time in 32, or
else at the equipment's unleash bonus in 200ths, and adds a quarter to the
damage, then the target's level divided by 5, plus 6. An ability whose effect
is a critical hit lands one somewhere between 1 time in 64 and 1 in 4, for
half as much again before the same addition. An ability's double or triple
blow comes 153, 32 or 64 times in 256, depending on its effect.

## Why it works this way

### The rules run while the effect plays

When a blow lands, the target flinches and its message appears while the
effect is still on screen. That takes some arranging. An effect is an
ordinary function with its own loop: it calls `WaitFrames` whenever time has
to pass and returns only when the animation is over (see
[Battle effects](../effect/README.md)). The battle's own code can't show a
hit or a message while it waits for that, so it shows them from a task, a
routine the scheduler runs every frame however deep the waiting goes (see
[Runtime](../runtime/README.md)). `BattlePresentation_DispatchAction`
([`battle/staging.c`](battle/staging.c)) schedules `BattleEvent_Playback`
([`battle/playback.c`](battle/playback.c)), runs the effect, then waits in
`BattleEventRuntime_WaitForReady` until playback is done.

Playback stays idle until the effect says the blow has landed. Most effects
call `EFFECT_SHOW_DAMAGE(hitSound)` on that frame (56 of the 66 effect parts
do), which starts playback with the hit's sound; a sound of 0 keeps the one
already set, `SE_HEAVY_IMPACT` after `BattleEventRuntime_Reset`. If an effect
never calls it, the wait after the effect starts playback instead. From then on,
once a frame, the task works through the targets one at a time. It runs
`BattleTargetExec` for the next target, which changes the target's HP,
statuses and the rest and queues what should be shown, and then plays that
target's queue over the following frames before it moves on.

The queue (`struct BattleEventQueue`, up to 64 events) is where the rules
hand over to the show. `BattleTargetExec` never draws or waits. It pushes
events with `BattleEvPush`: the names and numbers a message needs, then the
message, then the flinch, the defeat, the status redraw, a sound. They go in
the order you see them, and a message's arguments always come before it,
because the message takes them from the window system's argument table when
it's decoded. The same queue can also be played straight through, without
frames between events, by `BattleEv_DispatchQueued`. That's how the battle
shows outcomes outside an effect, such as a cast that's sealed or short of PP
(`staging.c`).

So the effect only decides when the damage appears, and the rules alone
decide how much it is. A later target's arithmetic runs while the effect is
still drawing, once that target's turn comes round.

### Functions with a result nobody reads

Some of the original's helpers are declared to return an integer or a
pointer, and end without a `return` or with a bare one. Their callers only
want what they do. In C, falling off the end like that is only undefined when
a caller uses the missing value, and none does. The three wrappers around
such helpers just call the helper and return nothing themselves.

Making them all `void` isn't free, because the return type can change which
register the return address comes back through. A function's result travels
in r0, and alchemy-gcc's `pop {r1}; bx r1` leaves r0 alone where
`pop {r0}; bx r0` doesn't. So the matching build keeps the declared type
wherever the `void` form changes the assembly; `BattleEventRuntime_Reset` is
`void` in every build. With `UBFIX`, which the modern build turns on, the
rest are declared `void` too, `BattleTargetExec`, `BattleEv_DispatchQueued`
and `BattleEventRuntime_WaitForReady` among them.

### One sprite, three structs

A battle unit's sprite on the battlefield is one of the object slots the
field's object code hands out (`Object_CreateFar`). The battle reads it as a
`struct MotionObject`, while the field's far calls take it as a
`struct DispatchObject` or a `struct ObjectRuntime`. They're three views of
the same record, with their fields at the same offsets, and a call into the
field casts to the view its prototype names.

## Limits

- There are 519 abilities, ids 0 to 518. The plan reads `actflags.tsv` only
  for those ids, and `actstat.tsv`, a row shorter, up to 517.
- A plan reaches at most 14 targets (`TARGET_MAX`, `plan.c`).
- Party unit ids are 0 to 7 and enemy unit ids start at 128. The party fights
  four at a time and six enemies at most (128 to 133), three on each side in
  a link battle (`PARTY_BATTLE_SIZE`, `ENEMY_BATTLE_SIZE`,
  `LINK_PARTY_SIZE`).
- `formatn.tsv` has 380 enemy groups, each up to five kinds of enemy with the
  least and most of each (`member0` to `member4`, `minimum0` to `minimum4`,
  `maximum0` to `maximum4`). The encounter zones in
  `games/tbs/src/object/encounter.tsv` choose among them.

## Evidence and open questions

The abilities and enemies are named from the label columns of their tables,
`ability` and `enemy`, which follow the English release's text. The formulas
are named from what `battle_calc.c` computes, and the event opcodes from what
playback does with each.

Still open:

- XXX: bits 0 and 1 of the `flags` column of `actflags.tsv`
  (`STAGING_ON_DOWNED`, `STAGING_OTHER_SIDE`) are read, but no row sets
  them. The plan sets the first itself for the revive effects; nothing sets
  the second.
- XXX: what reaches `BattleEvent_Playback` for the ten effect parts that never
  call `EFFECT_SHOW_DAMAGE`, beyond the wait after the effect.
- XXX: a few older names say too little or the wrong thing.
  `Summon_LayoutPositions` places the enemies, and
  `Summon_IsEntrySecondaryFlagged` reads a flag in the enemy class table that
  the effect code checks too (`../effect/parts/beams.c`); neither has
  anything to do with summons. A better name for the second needs to know
  what that flag means.
