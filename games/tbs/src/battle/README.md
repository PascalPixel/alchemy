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
| `advarrow.s` | the arrow that bobs beside a battle message while it waits for the player, built from `advarrow.png`, which holds sixteen 8 by 16 frames; the code cycles through the first eight, one every four frames |
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
- **ability kind**: the part of an ability's row that chooses the formula its
  targets go through: an attack, Psynergy damage, healing and so on
  ([below](#what-is-an-ability-kind)). It's the last hex digit of the
  ability's `flags` as `actions.tsv` writes them, the byte's low four bits
  (`ABILITY_KIND_MASK`, `enum AbilityKind`). The **effect kind** is a
  different number, in `actflags.tsv`, that picks the animation
  (`enum BattleFxKind`, in [Battle effects](../effect/README.md)).
- **power**: how strong an ability is, the `power` column of `actions.tsv`:
  Quake's is 12 and Avalanche's 160. What it counts depends on the ability
  kind ([below](#what-is-an-abilitys-power)). A unit's **Power** and
  **Resist**, capitalized as the status screen writes them, are a different
  pair of numbers, one pair for each element (`BattleUnit.element`).
- **added effect**: what an ability does besides its damage or healing, such
  as Sleep's sleep or Impact's raised Attack: the `effect` column of
  `actions.tsv` (`enum AbilityEffect`,
  [below](#what-does-an-added-effect-do)).
- **element level**: the Lv the status screen shows under each element icon
  (`Unit_GetElementLevel`). A character's Power and Resist start from it, and
  the gap between two units' levels moves the chance of an added effect.
- **place** and **place code**: where a unit stands in its side's line-up
  (`BattleWork.partyUnits` or `enemyIds`). A command aims at a place code: the
  place plus `PLACE_PARTY` for the party or `PLACE_ENEMY` for the enemies.
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
- **pose**: how a battle object stands: standing, leaping, casting, taking
  damage, held or down (`POSE_*`, set with `ObjectDispatch_SelectAnimationFar`).

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
| `BattlePresentation_` | what the battle shows: the staging, the opening transition, the camera and the palettes |
| `BattleCommand_` | choosing an action and planning it |
| `BattleTarget_` | choosing targets |
| `BattleEvent_`, `BattleEv_`, `BattleEventRuntime_` | the event queue and its playback |
| `BattleUnit_` | one unit's battle state: its guard, its weapon sprite, its status icons and the counters that tick at the end of its turn |
| `BattleFormation_`, `BattleEnemy_` | the enemy groups, and each enemy's level, name letter, defeat and coins |
| `BattleClass_` | an enemy class's row of the battle sprite table |
| `BattleEscape_` | running away |
| `ElementGlow_` | a unit's elemental glow: the star field and the other glows (`star_field.c`) |
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

A battle is a series of rounds. At the start of each the player chooses a command for
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

![Isaac runs back and forth in Goma Cave until the screen flashes white and goes black, and the battle opens on the party facing a group of monsters with `Wild Mushroom 1 appeared!`](https://alchemy.pascalpixel.com/guides/object/random-battle.gif)

An enemy picks one of the eight abilities in its row of
`games/tbs/src/game/enemies.tsv` by its `action_strategy`
(`enum EnemyActionStrategy`). The first three weight the eight evenly,
falling by 6 a place, or falling steeply from 113 to 5
(`gBattleCommand_SelectionWeights`); `ACTION_STRATEGY_RANDOM_START` and
`ACTION_STRATEGY_IN_ORDER` remember their place in the list from one turn to
the next. A choice it can't make (too little PP, a seal, nobody worth aiming
at) is drawn again, and after 18 tries the enemy defends.

An enemy that picks an ability whose added effect is Defend, Total Defense or
Flee never uses that ability at all. `BattleCommand_SelectAutomatic` turns the
choice into a command of its own, `CMD_DEFEND`, `CMD_DEFEND2` or `CMD_FLEE`,
aimed at the enemy itself. A guard is chosen as the round is built and raised
before anyone acts (`round.c`), and a Flee waits for the enemy's turn, when
the player reads `<unit> runs!`.

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
works in a battle the party can't run from (`BATTLE_START_NO_ESCAPE`).

## How an ability resolves

[Battle](../../battle.md) follows Isaac's Quake through a turn. Underneath,
whatever the player picks becomes an ability id (Attack is ability 1, Quake 3,
Cure 90, Flint 300), and the id picks a row in each of three tables:
`games/tbs/src/game/actions.tsv` (`struct BattleAct`) says what the ability
does, `actflags.tsv` (`gBattle_ActionFlags`) picks the animation, and
`actstat.tsv` (`gBattle_ActionStatus`) can choose how the battle shows it.

In `actions.tsv`, Quake's row reads `target_mode` 1 (`TARGET_MODE_FOES`),
`element` 0 (`ELEMENT_VENUS`), `range` 2, `pp_cost` 4 and `power` 12. Its
`flags` are written `0x85`, which is `0x80` plus 5: the `0x80`
(`ACTION_USABLE_IN_BATTLE`) lets it be used in battle, and the 5 is its
ability kind. Those two numbers, the power and the ability kind, decide what
each target takes.

### What is an ability's power?

It's the number in the ability's row that says how strong it is. It's only
where the arithmetic starts, though: the caster's and the target's own numbers
move the result from there. Those are Power and Resist. Every unit has one of
each in every element, and the status screen lists them under the four
element icons (`BattleUnit.element`, drawn by `../menu/menu/status.c`, and in
battle by `../window/battle_command/status.c`). It's an even match when the
caster's Power in the ability's element equals the target's Resist in it.

What the power counts depends on the ability kind. For damage, it's what the
aimed target takes at an even match, before a random 0 to 3 is added: Quake's
12 is 12 to 15 damage. For healing, it's the HP restored by a healer with 100
Power in the element, where healing is neither boosted nor cut. For Psynergy
that strikes like an Attack, such as Ragnarok, it's damage added to a plain
blow, and for an ability that multiplies the blow, such as the Djinni Flint,
it's the multiplier in tenths: Flint's 16 makes the blow 1.6 times as hard.

### What is an ability kind?

It's the number that tells the battle which formula to use: whether the
ability hurts, heals, strikes like an Attack or takes PP. `BattleTargetExec`
([`battle/target_exec.c`](battle/target_exec.c)) switches on it for each
target, once the added effect has been rolled. Quake's ability kind,
`ABILITY_KIND_DAMAGE`, is plain damage from the power and the element, and
most of the Psynergy that hurts has it, Avalanche among them.

| Ability kind | What the target takes | What the power is |
| --- | --- | --- |
| `ABILITY_KIND_HEAL_HP`: Cure, Ply, Herb | HP back, from the power and the healer's Power in the element (`BattleRecoveryCalc`) | the HP healed at Power 100 |
| `ABILITY_KIND_EFFECT`: Sleep, Impair, Restore, Drain | its added effect; most have a power of 0 and do nothing else, and Drain and a few others also do damage from the power and the element, but only when the effect lands | the damage at an even match |
| `ABILITY_KIND_ATTACK`: Attack, Ragnarok | a blow, the actor's Attack against the target's Defense, with the power added (`BattleAtkCalc`) | damage added to a plain blow |
| `ABILITY_KIND_ATTACK_MULTIPLIED`: Flint, Quick Strike | a plain blow (`BattleAtkCalc`) times the power, in tenths | the blow's multiplier, in tenths |
| `ABILITY_KIND_DAMAGE`: Quake, Avalanche | damage from the power, scaled by the caster's Power against the target's Resist (`BattlePowerCalc`) | the damage at an even match |
| `ABILITY_KIND_UNLEASHED_DAMAGE`: Fire Blessing, War Cry | as for `ABILITY_KIND_DAMAGE`, falling off faster with distance: 100, 50, then 30 percent | the damage at an even match |
| `ABILITY_KIND_SUMMON`: Ramses, Judgment | as for `ABILITY_KIND_DAMAGE`, once a share of the target's maximum HP is added to the power (3 percent for each Djinni the summon uses, so 3 to 12), falling off by its own table: 100, 70, then 40 percent | the damage at an even match, before that share |
| `ABILITY_KIND_PP_DAMAGE`: Crazy Voice | as for `ABILITY_KIND_DAMAGE`, taken from PP, falling off as `ABILITY_KIND_UNLEASHED_DAMAGE` does and with no random added | the PP lost at an even match |
| `ABILITY_KIND_HEAL_PP`: Psy Crystal, Adept Ring | PP back, as for `ABILITY_KIND_HEAL_HP` (Ember has this ability kind too, but its power is 0: its PP comes from its added effect) | the PP restored at Power 100 |

HP healing, attacks and HP damage then add a random 0 to 3; an added effect's
damage, PP damage and PP healing don't. Three ability kinds apply no power at
all: `ABILITY_KIND_BLANK`, which only the 128 unused rows named `?` have (row
0, Nothing, is `ABILITY_KIND_DAMAGE`); `ABILITY_KIND_EFFECT_ONLY`, for Granite,
Flash and Kite, whose added effect is all they do; and `ABILITY_KIND_NO_POWER`,
for the field Psynergy, Defend, Total Defense, Stand Ready, Flee, the enemies'
commands such as Threaten and Ally Search, and the stat-raising items: Power
Bread, Cookie, Apple, Hard Nut, Mint and Lucky Pepper.

The other ability kinds have nothing to apply when the power is 0, either.
`BattleTargetExec` skips the damage or healing of an ability of any other
kind but the two attacks when it has no power, so all it does is its added
effect. An attack only adds its power to the blow, so Attack itself, whose
power is 0, still strikes.

`ABILITY_KIND_UNLEASHED_DAMAGE` belongs to the enemies' Blessings, Deadly Gas,
Sonic Wave, Shriek, War Cry, howls and Wing Beats, and it's why most of them
are announced as `<unit> unleashes <ability>!`
([below](#which-effect-plays-and-how-is-it-staged)). A weapon's unleash is
something else: it replaces Attack with an ability of the weapon's own, and
nearly all of those are attacks of `ABILITY_KIND_ATTACK`.

### How does a command become an ability id?

`BattleCommand_BuildPlan` ([`battle/plan.c`](battle/plan.c)) works it out
from the command the player or the enemy chose:

- **Attack** (`CMD_ATTACK`) is `ABILITY_ATTACK`, unless the equipped weapon
  unleashes (`RollWeaponUnleashFar`). Then the weapon's row of `items.tsv`
  gives the ability in `unleash_action`, at a chance of `unleash_chance`
  times 5 percent plus the equipment's unleash bonus.
- **Psynergy** (`CMD_PSYNERGY`) is the ability chosen in the menu. Its
  `pp_cost` is paid here, and only when the cast goes ahead: a sealed caster,
  or one short of PP, pays nothing.
- **Djinn** (`CMD_DJINN`): `Djinn_GetAbilityIdFar(element, index)` reads the
  Djinni's row of `games/tbs/src/game/djinn.tsv` (20 rows to an element,
  Venus first) and returns its `ability` column, which is
  300 + 20 × element + index. Only a set Djinni is unleashed, and it then
  goes on standby. One on standby is set again instead, and one still
  recovering says so.
- **Items** (`CMD_ITEM`): the item's row of `games/tbs/src/game/items.tsv`
  gives the ability in `action_id`. An item with none (0), or a broken one,
  makes the user defend. Its `use_type` says what using it costs: 1
  (`ITEM_USE_CONSUMED`) uses one up, 2 (`ITEM_USE_MAY_BREAK`) breaks it one
  time in eight, and 4 (`ITEM_USE_EMPTIES`) leaves an Empty Bottle. 3
  (`ITEM_USE_TEACHES`) marks equipment that teaches its ability while it's
  worn (`../game/game/levels.c`).
- **Summons** (`CMD_SUMMON`): the summon's row of
  `games/tbs/src/game/summons.tsv` gives the ability, once enough Djinn of
  each element are on standby. The plan then marks the Djinn it uses.
- **Enemies** (`CMD_ABILITY`) use one of `action0` to `action7` in their row
  of `games/tbs/src/game/enemies.tsv`, chosen by `enemy_ai.c` as described
  above.

The plan takes the element an ability strikes with from its row's `element`
(4, `ELEMENT_NONE`, is none), except that a plain Attack strikes with the
weapon's element, and an enemy's attack with its own.

### How far does an ability reach?

When the player aims an ability, a cursor sits on the aimed unit and smaller
markers on the others its range covers (`BattleTarget_RunSelection`,
`../window/battle_command/targets.c`). The range counts places in the side's
line-up: 1 is the aimed unit alone, each step more adds a place on either
side, and 255 (`ABILITY_RANGE_ALL`) is the whole side, with every target
treated as the aimed one. `BattlePlan_SetTargets` (`plan.c`) walks those
places in order, passing over a place whose unit has left the battle and a
downed unit unless the ability revives, and keeps each target's distance
from the aimed place. A downed unit still holds its place, so it still counts
as a step. If nobody in range is left, the actor defends instead
(`<unit> is defending!`).

Avalanche has a range of 3: the aimed foe and two places on either side,
five in all, where Quake's range of 2 reaches three.

![Isaac casts Avalanche at Ivan, Garet and Mia: the ground swells under them, a narrow wall of rock rises with rocks tumbling up it and widens until it fills the screen, and Ivan takes 203 damage, Garet 160 and Mia 120.](https://alchemy.pascalpixel.com/abilities/avalanche.gif)

Like the Quake recording in [Battle](../../battle.md), this one turns the
ability on Isaac's own companions. Isaac aims at Ivan, with nobody on Ivan's
other side, so Avalanche catches Garet one place along and Mia two, where
Quake stopped at Garet. Ivan takes 203 damage, Garet 160 and Mia 120, and
160 and 120 are 80 and 60 percent of about 200, as they'd be if the three had
much the same Venus Resist. Everything but an attack
falls off like that with the target's distance from the aimed one, by the
tables in `battle_calc.s`. The figures are percentages:

| Steps from the aimed unit | 0 | 1 | 2 | 3 | 4 | 5 |
| --- | --- | --- | --- | --- | --- | --- |
| Psynergy damage and HP and PP healing (`ABILITY_KIND_DAMAGE`, `ABILITY_KIND_HEAL_HP`, `ABILITY_KIND_HEAL_PP`) | 100 | 80 | 60 | 40 | 20 | 10 |
| Summon damage (`ABILITY_KIND_SUMMON`) | 100 | 70 | 40 | 30 | 20 | 10 |
| An enemy's unleashed damage, an added effect's damage and PP damage (`ABILITY_KIND_UNLEASHED_DAMAGE`, `ABILITY_KIND_EFFECT`, `ABILITY_KIND_PP_DAMAGE`) | 100 | 50 | 30 | 10 | 10 | 10 |
| The added effect's chance | 100 | 60 | 30 | 30 | 30 | 30 |

### How does the element change the amount?

The element bonus is the actor's Power in the ability's element less the
target's Resist in it. An item's user always counts as Power 100, whatever
the item (the `CMD_ITEM` test in `BattleTargetExec`). The element multiplier
(`Battle_GetElementMultiplier`, read from the five points of
`gElement_PowerResistCurve` in `../game/element_curve.s`) turns the bonus into
the number the damage is multiplied by. It's 1 at an even match and moves half
a percent for every point of bonus, from 0 at 200 points behind to 2 at 200
ahead. Attacks feel the element at half strength, from 0.5 to 1.5. An ability
with no element (`ELEMENT_NONE`) gets no bonus at all, so its multiplier stays
1, whether it's an attack or not. Healing goes by the healer's own Power
instead, with no Resist in it: one percent more or less for each point above
or below 100, from nothing at 0 to double at 200.
An ability with no element heals as if at 100. The game counts these
multipliers in 256ths, so 1 is 256 (`FULL_RATE`), and every result rounds
down.

A character's Power and Resist start from its element level in each element
(`Unit_CalcBaseElements`, `../game/game/classes.c`):
`gElement_PowerResistByLevel` runs from Power 75 and Resist 85 at level 0 to
150 and 165 at level 15. An enemy's come from its element preset
(`gEnemy_ElementPresetTable`). `UnitCalc` adds the equipment's Power and
Resist, 20 Resist in every element for each stage of Resistance that Ward or
Resist has raised (or Weaken or Enfeeble lowered), and a summon's
boost: at the end of the summoner's turn, the element it spent the most Djinn
of gains 10, 30, 60 or 100 Power for one to four Djinn, and the player reads
`<unit>'s Earth Power rises by <amount>!` or its Water, Fire or Wind twin
(`BattleUnit_ProcessTurnEnd`). Everything stays between 0 and 200.

For an attack and for `ABILITY_KIND_DAMAGE`, `ABILITY_KIND_UNLEASHED_DAMAGE`
and `ABILITY_KIND_SUMMON`, the damage message tells the
player how the element fared. It goes by the target's base Resist, before
equipment and stages (`baseElement`): `<unit> takes <damage> damage.` when
the ability's element is the one the target resists best, `damage!!!` when
it's the one it resists worst, and `damage!` otherwise, and a target that
resists all four alike gets `!!!`. Garet's line ends `!!!` in the Avalanche
recording, so there's no element he resists less than Venus.

A Djinni that strikes the element a target resists worst may also earn the
Djinn bonus: at a chance of the user's Power less the target's Resist, plus
30 percent, an enemy it fells gives more coins and experience and a better
chance of its item (`BattleEnemy_RecordDefeat`).

### How much do Avalanche, Cure and Ragnarok do?

Avalanche has a power of 160 and `ABILITY_KIND_DAMAGE`. At an even match the
multiplier is 1, so `BattlePowerCalc` gives 160: the aimed foe takes 160 to
163 once the random 0 to 3 is added, a foe one place away 128 to 131, and one
two places away 96 to 99. With a 20-point lead, Isaac's Venus Power at 120
against a Venus Resist of 100, the multiplier is about 1.1 (281 in 256ths:
256, plus 1.28 for each of the 20 points, rounded down), and the aimed foe
takes 175 to 178, then 140 to 143 and 105 to 108. A
foe that's defending takes half of that after the random, 87 to 89 for the
aimed one (`REDUCE_BY_GUARD`). Ivan's 203 in the recording is about a quarter
more than the even match's 160, which puts Isaac's Venus Power there 50 to 55
ahead of Ivan's Venus Resist.

Cure has a power of 70 and `ABILITY_KIND_HEAL_HP`. A healer with 100
Venus Power restores 70 to 73 HP, and one with 120 restores 83 to 86, the
multiplier then being 307 in 256ths. Neither goes past the target's maximum
HP, where the message becomes `<unit>'s HP is fully restored!`. The Healing
Ring uses the same row (its `action_id` in `items.tsv` is 90), and from the
ring it's always 70 to 73, whoever holds it, because an item's user counts as
Power 100.

![Isaac casts Cure on Ivan beside him: a ring of light rises from the ground round Ivan and closes in, Ivan flashes white, and he recovers 99 HP.](https://alchemy.pascalpixel.com/abilities/cure.gif)

Ivan recovers 99 HP in the recording, so Isaac's Venus Power there is
between 138 and 142.

Ragnarok is Psynergy that strikes as an attack: `ABILITY_KIND_ATTACK` with a
power of 35. Take an actor with Attack 100 against a Defense of 60, at
an even match. A plain attack, whose power is 0, deals half of Attack less
Defense, 20, so 20 to 23 with the random. Ragnarok adds twice its power
before the halving, so it adds its 35 in full: 55 to 58. The element works at
half strength in an attack, so a 20-point lead in Venus adds about 5 percent,
where it adds 10 percent to Avalanche. Because it costs PP, it's staged as a cast
(`Ability_IsPsynergy`), not as a run-up and a blow.

![Isaac casts Ragnarok at Ivan: a huge flaming sword swings down onto him and bursts into a blaze of light, and Ivan takes 127 damage.](https://alchemy.pascalpixel.com/abilities/ragnarok.gif)

Flint, Venus's first Djinni, multiplies the same blow instead:
`ABILITY_KIND_ATTACK_MULTIPLIED` with a power of 16, so the plain 20 becomes 32, and 32 to 35
with the random.

However weak the actor and however strong the Defense or the guard, a blow
always does at least 1 damage, a plain one or one that multiplies it.
Psynergy damage has no such floor and can come to 0.

A plain Attack has rolls of its own, made as the plan is built. It misses one
time in 32, and a deluded attacker misses about three times in five as well
(153 in 256); the player then reads `<unit> nimbly dodges the blow!`. A target
that can't act (asleep, stunned, charmed, held by Ground, or down) is never
missed. A critical hit comes one time in 32, or failing that at half the
percentage the equipment adds to its unleash chance, and adds a quarter to the
damage, then the target's level divided by 5, plus 6.

An ability's added effect can make it strike twice or three times instead, on
every target at once: about three times in five (153 in 256) for Gust, Rapid
Smash, Slice and Sonic Smash, and one time in four for Flying Attack, Double
Fang, Twin Beaks and Swift Strike. Sonic Smash and Swift Strike are the ones
that strike three times. The code also has added effects for a double or
triple blow one time in eight, and for an ability's own critical hit, half as
much again, but no ability has any of them.

### Which effect plays, and how is it staged?

`actflags.tsv` picks the animation. Avalanche's `effect` there is `0x2003`:
the last three digits, `003`, are the effect kind, `BATTLE_FX_GAIA`, and
the `2` in front of them is the variant. `BattleFx_RunGaia`
([`../effect/parts/gaia.c`](../effect/parts/gaia.c)) plays variant 2 as its
widest wall of rock, the one Grand Gaia has too. Cure's effect kind is
`BATTLE_FX_CURE` and Ragnarok's `BATTLE_FX_RAGNAROK`.
[Battle effects](../effect/README.md#how-a-move-picks-its-effect) explains
the rest of that number, the variants and every effect family.

Before the effect plays, the battle sets the stage: where the camera turns,
who stays on the field, and whether the actor runs up or stands and casts.
`actstat.tsv` can choose that staging (`enum BattlePlanStaging` in
`battle_internal.h`). Avalanche's row is `PLAN_SHOW_FULL_SCREEN`, which only seven
abilities have: Gaia, Mother Gaia, Grand Gaia, Rockfall, Rockslide,
Avalanche and the summon Procne. `BattlePresentation_RunUnitTransition`
([`battle/staging.c`](battle/staging.c)) stages them. The camera turns, and
the actor raises its hand and glows in its element while the units of the
side that isn't targeted fade out. Then only the actor and the target side's
units stay on the field while the effect plays and the events run, and at the
end everyone fades back in.

Flint's row is `PLAN_SHOW_ATTACK`, as are most weapon unleashes' and
several Djinn's: the actor runs up and strikes. Seven weapon unleashes, such
as Blizzard and Titan Blade, have `PLAN_SHOW_ENEMY_CAST`, despite its name.
Most rows give no staging, and then the plan decides. An ability that costs PP, or Psy
Drain, is a cast (`PLAN_SHOW_CAST`), as Quake, Cure and Ragnarok are. Another
with an effect to draw is a cast too, or `PLAN_SHOW_ENEMY_CAST` for an enemy,
and one with nothing to draw shows only its messages (`PLAN_SHOW_MESSAGES`).
Using an item has staging of its own (`PLAN_SHOW_ITEM`), unless the item's
ability is a summon or full-screen.

The announcement comes from the command, before any of this:
`<unit> attacks!` for a plain Attack (`MsgActorAttacks`),
`<unit> casts <ability>!` for Psynergy (`MsgActorCasts`),
`<unit> unleashes <ability>!` for a Djinni (`MsgActorUnleashesDjinn`),
`<unit> summons <ability>!` for a summon (`MsgActorSummons`),
`<unit> uses <item>!` for an item, or `<unit> raises the <item>!` for one the
user keeps, such as a weapon or a ring, and
`<unit>'s <weapon> lets out a howl!` followed by the ability's name for a
weapon's unleash. An enemy's ability is `<unit> uses <ability>!`, or
`<unit> unleashes <ability>!` for `ABILITY_KIND_UNLEASHED_DAMAGE`, and a few
moves have
wordings of their own, such as Sonic Wave's `<unit> emits ultrasonic waves!`
and the howls' `<unit> lets out a <ability>!`.

### Where are the formulas?

The battle code calls the formulas by their far-call names, which
`games/tbs/src/system/far_call/GAME.S` exports for the functions in
[`../game/game/battle_calc.c`](../game/game/battle_calc.c):

| Called as | Function | What it works out |
| --- | --- | --- |
| `BattleAtkCalc` | `Battle_CalcAttackDamage` | Attack less Defense (never below 0) plus twice the ability's power, the whole sum halved, then scaled by the element multiplier at half strength |
| `BattlePowerCalc` | `Battle_CalcPsynergyPower` | the ability's power times the element multiplier |
| `BattleRecoveryCalc` | `Battle_CalcRecovery` | the ability's power times the multiplier for the healer's Power |
| `BattleHitCheck` | `BattleFx_RollSuccess` | whether an added effect takes hold: its base rate, the element levels, Luck and weakness |
| `RollWeaponUnleashFar` | `RollWeaponUnleash` | whether the weapon unleashes |

## What does an added effect do?

Besides its damage or healing, an ability can carry one added effect: Sleep
puts its target to sleep, Impact raises an ally's Attack, Drain hands the HP
it took back to its caster. It's the `effect` column of `actions.tsv`
(`enum AbilityEffect` in
[`battle_unit.h`](../../include/battle/battle_unit.h), whose notes name the
abilities that have each one). `BattleTargetExec` handles it in four steps for
each target, and its case comments give each effect's detail.

### Does it land?

First `BattleHitCheck` (`BattleFx_RollSuccess`, in
[`../game/game/battle_calc.c`](../game/game/battle_calc.c)) rolls whether the
effect takes hold. It fails at once when there's nothing for it to do: a
revival on a unit still standing, Cure Poison on one that isn't poisoned,
Restore or Tonic on a unit with nothing to cure, a curse on a unit whose
countdown is already at its last turn. Otherwise the chance starts from the
effect's base rate (Sleep 45 percent, Taint 55, Condemn's instant death 20
and so on, in `BattleFx_GetBaseSuccessRate`), gains 3 for each element level
the caster is ahead of the target (nothing for an effect with no element,
since `Unit_GetElementLevel` gives every unit level 0 there), loses 3 for
every two points of the target's Luck, and gains 25 when the effect is one of
the three weaknesses the target's class lists, or an enemy's `weak_effect0`
to `weak_effect2` in `enemies.tsv`.
The distance table above then scales it, so a Sleep with a 60 percent chance
on the aimed foe has 36 on its neighbours. Quartz and Dew revive at a fixed 60
and 90 percent, and an effect with no rate of its own, such as a raised stat
or no effect at all, always works on the aimed target, though the distance
table still scales it for its neighbours; levels, Luck and weaknesses don't
change those. Luff gets three tries.

When the effect of an `ABILITY_KIND_EFFECT` ability fails, its target takes no damage either, and the player reads
`But it has no effect on <unit>!` (`MsgNoEffect`).

Three effects are rolled earlier, as the plan is built (`plan.c`): a double
or triple blow, an ability's critical hit, and Annihilation's instant death,
which rolls at full rate whatever the distance so that its effect can show
whether it landed.

### Which effects change the blow?

Once an effect has landed, a few of them change the damage step before it
runs. Torch's attack counts only half the target's Defense, and Severe Blow
and Thrash leave the target 1 HP at most. Condemn and the other instant
deaths skip the damage altogether, Dynamite and Mortal Blow take their user
out of the battle, and Psy Drain becomes PP damage (`ABILITY_KIND_PP_DAMAGE`), or
misses a target with no PP. Ground holds a target only while it still has an
action waiting in the round's list: `BattlePresentation_DispatchAction` clears
each entry as it plays it, so a target that has already acted is left alone,
and the player reads `But it has no effect on <unit>!`.

An enemy's Ally Search, Search for Help and Speed Surge are handled before the
damage, too. Ally Search and Search for Help bring a new enemy into an empty place when a
free unit id and room among the enemy sprites allow it (`<unit> appears!!`),
and otherwise the player reads `But no one came!`. Speed Surge splits the
enemy instead (`It split off into <unit>!`), or reads `It failed to divide!`.

### What does it do once it lands?

After the damage or healing, the effect applies if it landed and the target is
still standing. A revival needs the target down instead, and a drain still
works on a target its own damage has just felled (`BattleEffectDeadCheck`).

Stat changes move Attack, Defense or Resistance a stage or two, between -4 and
4, for seven turns. A raised stat always lasts its seven turns, but a lowered
one may recover sooner: once five turns or fewer are left, the end of each
turn rolls for it, more often for a lucky unit and as the turns run down
(`BattleUnit_RollCounterEarlyEnd`). A stage is an eighth of Attack or Defense,
or 20 Resist in every element, and the number in the message is how far the
stat really moved, so a stat already at its limit shows 0. Zephyr and Vine set
Agility outright instead, doubled or halved, for five turns, which never end
early.

The statuses that Delude, Confuse, Charm, Paralyze and Sleep cause last seven
turns and may wear off sooner, and so does Bind's seal on Psynergy. Luff's
seal lasts two turns that never end early. Taint's poison does nothing to a
unit that's already poisoned, and the deadly poison of the ability Poison
replaces plain poison but does nothing to a unit that has it already. Curse
calls the Grim Reaper, who takes the target when the seven-turn countdown
runs out, and each later curse brings it a turn closer.

![Ivan casts Paralyze at Isaac: purple rings and streaks burst around Ivan, a crackle of yellow light wraps Isaac, and the message reads `Isaac has been stunned!`](https://alchemy.pascalpixel.com/abilities/paralyze.gif)

![Ivan casts Confuse at Isaac, Garet and Mia: a pale blue mist rolls over the three, and the messages read `Isaac is confused!` and `But it has no effect on Garet!`](https://alchemy.pascalpixel.com/abilities/confuse.gif)

Cure Poison and Antidote clear poison. Restore and Elixir clear delusion,
stun, sleep, the Psynergy seal and the Grim Reaper's countdown, but not poison
or an evil spirit. Tonic clears all of those, poison and the evil spirit
included, but it doesn't clear confusion or charm any more than Restore does:
those wear off on their own or end when the unit goes down. XXX: likely an
oversight, because the roll that decides whether Restore or Tonic lands
counts confusion and charm, so either can land on a unit whose only trouble is
one of those and change nothing. Revive and the Water of Life bring a downed
unit back with all its HP, Quartz with half and Dew with 80 percent. Fizz and
Spritz heal 60 and 30 percent of the maximum HP, and Ember 7 percent of the
maximum PP.

Drain gives its caster all the HP the target lost, Sap half, Psy Drain the PP
it took, and Psy Leech a tenth of the damage as PP, as far as the target has
it. Defend and Granite halve the damage the target takes for the rest of the
round, and Total Defense and Flash cut it to a tenth; the message
(`A protective aura encircles <unit>!`) shows either way, but a guard is never
lowered. Break undoes every raised stat stage, and the summon boost too, and
the player reads `<unit>'s strength returns to normal!`. Kite lets the target
act twice next turn, Ground costs it its next action, and Contain, Threaten
and Tremble do nothing beyond their announcement. Haunt sets an evil spirit
on the target (`An evil spirit grips <unit>!`), which comes into play when the
haunted unit acts.

### What happens after each target is hit?

Two more rolls follow. Damage wakes a sleeping target one time in four,
unless it fell asleep this turn (`<unit> wakes from slumber!`). And when the
actor is haunted, one time in four the evil spirit takes a quarter of the
damage the actor has just dealt that target, at least 1. The shares add up
across the targets (`BattlePlan.hauntDamage`), and at the end of its turn the
actor takes them all, as `A curse does <damage> damage to <unit>!`
(`BattleUnit_ProcessTurnEnd`).

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
the order the player sees them, and a message's arguments always come before it,
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
- Party unit ids run to `ALLY_MAX` and enemy unit ids start at
  `FIRST_ENEMY_UNIT`. The party fights four at a time and six enemies at most,
  three on each side in a link battle (`PARTY_BATTLE_SIZE`, `ENEMY_BATTLE_SIZE`,
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
