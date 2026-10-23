# Battles

![Who acts first in a round, from first to last: three Djinn that guard or hold an enemy, then everyone by Agility, highest first, a second action late; Defend is raised before anyone acts.](https://alchemy.pascalpixel.com/illustrations/battle/turn-order.svg)

Everything the rules of a fight decide is worked out here: who acts in what
order, which enemy does what, who an ability reaches, whether a blow misses
or lands a critical hit, how much damage each target takes, and which
statuses take hold and for how long. The answers come from tables and dice
rolls, and each one shows up on screen as a message such as
`Ivan takes 15 damage!`.

The code runs a fight from its opening transition to the spoils: it builds
the units, gathers each round's actions and sorts them into turns, plans each
action, stages it, and applies the ability to every target. Three other
folders share the work. [Battle effects](../effect/README.md) draws the
animations, [Windows](../window/README.md) holds the command menu and the
[party](../../glossary.md#the-party)'s status window, and [Game rules](../game/README.md) holds the ability, item and enemy
tables and the arithmetic in `game/battle_calc.c`. [Battle](../../battle.md)
explains a battle as a whole: how it is entered from the field, the loop of
rounds and actions in code, and how the rules and the effects run at the
same time. Read it first; this page explains the rules underneath.

Every rule here is the [original](../../glossary.md#the-original)'s,
rebuilt so that it compiles to the same bytes
([matching decompilation](../../glossary.md#matching-decompilation)), so a
few forms below are kept for the bytes rather than for the reader; Design
names them.

## Rounds

A battle is a loop of rounds. In each, every unit (everyone in the fight,
party member or enemy) chooses an action, the
actions line up fastest first, and they play one after another, until one
side has no unit standing or the party gets away.
[Battle](../../battle.md) opens with a picture of that loop; the picture at
the top of this page is the order inside one round.

Who acts first is decided by Agility, highest first, with three twists. A
[Djinni](../../glossary.md#djinn) that guards or holds an enemy (Granite, Flash and Ground) goes before
everyone. A party member's Agility gains up to a sixteenth more at random
each round, so two members of nearly equal speed may swap places from one
round to the next; an enemy's Agility counts as it is. On a tie the party goes first, and
enemies of equal Agility go in a shuffled order. And a unit that acts twice in a round has its second action
at half its Agility, so it usually comes late. Defend is raised as soon as
the actions are sorted, before anyone acts, so a defending unit is guarded
against everything that round, even the fastest enemy.

`Battle_Run` ([`battle/encounter.c`](battle/encounter.c)) fights the
whole battle and returns the number of enemies defeated, `BATTLE_OUTCOME_FLED` or
`BATTLE_OUTCOME_LOST`. Before the first round `formation.c` turns the enemy
group into units, with their levels and the letters that tell two of a kind
apart, and `transition.c` plays the opening: the curtain, or the Venus
Lighthouse storm. After the last, `spoils.c` hands out coins, experience,
items and level ups.

Each round runs five steps, and the battle ends when one side has no unit
standing or the party gets away. Steps 1 and 5 run once a round, steps 2 to
4 once for each action:

1. `round.c` gathers the actions: the party's from the command menu
   (`Battle_GatherActions`), or from `auto_battle.c` under
   `FLAG_AUTO_BATTLE`; the enemies' from `BattleAI_ChooseAction`
   (`enemy_ai.c`), or from the other console in a link battle. Each
   action's priority is worked out as above (`Battle_CollectPartyCommands`),
   and `BattleTurnOrder_Sort` sorts them, highest first, keeping the
   order of equal priorities; the three Djinn have `DJINN_FIRST_PRIORITY`
   added. Defend and Total Defense are raised here.
2. `BattleTurn_Build` (`plan.c`) announces the action and fills the
   plan.
3. `BattleStaging_DispatchAction` (`staging.c`) stages the plan and runs
   its effect.
4. While the effect plays, the battle's playback task calls
   `BattleAbility_ApplyToTarget` (`target_exec.c`) once for each target, which
   applies the ability to it; then `turn_end.c` closes the actor's turn:
   poison takes a tenth of the maximum HP (deadly poison a fifth), the Grim
   Reaper takes a unit whose countdown has run out, and a
   [summon](../../glossary.md#summons)'s boost is applied.
5. `turn_end.c` closes the round: stat changes and statuses count down and
   Djinn recover.

The rules never draw. `BattleAbility_ApplyToTarget` changes the units at once and
leaves a list of what to show, the battle event queue, which playback shows
in step with the animation;
[Rules and pictures meet at the event queue](#rules-and-pictures-meet-at-the-event-queue)
takes it apart.

| Name | Value | Meaning |
| --- | --- | --- |
| `BATTLE_OUTCOME_LOST` | -1 | `Battle_Run`'s result when the party is beaten |
| `BATTLE_OUTCOME_FLED` | 999 | its result when the party ran away |
| `PARTY_BATTLE_SIZE` | 4 | party members who fight at a time |
| `ENEMY_BATTLE_SIZE` | 6 | enemies in a battle at most |
| `LINK_PARTY_SIZE` | 3 | party members each console sends to a link battle |

### Enemy actions

A Gryphon in battle uses Wing Flutter more often than anything else and
Ward least often. Every enemy has a list of eight
actions and a habit for choosing among them, and the dice do the rest.

![How an enemy picks its action. A Gryphon's eight actions, each with its chance under its strategy, which favours the top of the list: Wing Flutter 53 in 256, Tornado 47, Twin Beaks 41, Impact 35, Attack 29, Sonic Slash 23, Bind 17 and Ward 11. As the round starts the enemy draws once: a guard it draws, Defend or Total Defense, is kept and raised at once, and anything else waits for its turn. At its turn it draws again until it can use the pick: too little PP, a seal, or nobody worth aiming at means another draw, and 18 failed draws make it defend. Other enemies draw at 32 in 256 each, favour the first from 113 down to 5, or go down the list in order.](https://alchemy.pascalpixel.com/illustrations/battle/enemy-action.svg)

An enemy picks one of the eight abilities in its row of `../game/enemies.tsv`
by its action strategy (`enum EnemyActionStrategy`). Three strategies draw at
random by a row of `gBattleAI_ActionWeights`: evenly, falling by 6 an
entry, or falling steeply from 113 in 256 to 5. The other four take the list
in order, from a random entry or the first, or repeat the last pick, and
remember their entry between turns.

`BattleAI_ChooseAction` runs twice for each enemy. As the round is
built (`retry` 0) it draws once and keeps the pick only if it is a guard, so
that the guard is raised with the party's before anyone acts. At the enemy's
turn (`retry` 1) it draws again for anything else, and a guard drawn then is
refused. A pick the enemy cannot use (too little [PP](../../glossary.md#psynergy-and-pp), a seal, no target worth
it) is drawn again, and after 18 failed picks the enemy defends. A roll is a
number from 0 to 255, so a chance of 53 in 256 is the 53 rolls from 0 to 52:
the Gryphon's first action, Wing Flutter, takes rolls 0 to 52, Tornado 53 to
99, Twin Beaks 100 to 140, and so on to Ward's 245 to 255.

| Strategy | Value | How the entry is picked |
| --- | --- | --- |
| `ACTION_STRATEGY_EVEN` | 0 | at random, 32 in 256 each |
| `ACTION_STRATEGY_FALLING` | 1 | at random, from 53 in 256 down to 11 |
| `ACTION_STRATEGY_FIRST_FAVOURED` | 2 | at random, the first 113 in 256, down to 5 |
| `ACTION_STRATEGY_RANDOM_START` | 3 | a random entry the first time, then each in turn |
| `ACTION_STRATEGY_IN_ORDER` | 4 | each in turn from the remembered entry |
| `ACTION_STRATEGY_NEXT_PICK` | 5 | the next entry at each new pick, from 0 |
| `ACTION_STRATEGY_SAME` | 6 | the entry last picked |

An ability whose added effect (what it does besides damage or healing, the
`effect` column of `actions.tsv`) is Defend, Total Defense or Flee is never used
as an ability. `BattleAI_ChooseAction` turns it into the command
`COMMAND_DEFEND`, `COMMAND_TOTAL_DEFENSE` or `COMMAND_FLEE`, aimed at the enemy itself, so the
guard is raised with the party's before anyone acts.

### First strikes and escape

Now and then a random battle opens with `Isaac's party attacks first!`, and
the enemies stand still for a round; more rarely it opens with
`Isaac's party was caught by surprise!`, and some of the enemies act before
the party can choose anything. Running away works the same kind of way: a
chance that grows with every failed try.

![Who gets the jump. A random battle opens with two rolls. One time in 16 the party strikes first: no enemy acts in the first round, and running away cannot fail. Otherwise, one time in 32 the party is caught by surprise: it chooses nothing, and two to five enemies act, once each. Otherwise the round is a normal one. A bar shows the shares: about 6 percent, about 3 percent and 91 percent. Only random encounters roll; the Djinn lesson always gives the party the first strike. Running away succeeds 50 percent of the time, plus 20 percent for each failed try and 5 percent for each level the party is ahead; a failed try gives the rest of the round to the enemies, as in a surprise.](https://alchemy.pascalpixel.com/illustrations/battle/first-strike.svg)

`BattleWork.firstStrike` records which side has the jump. In a random
encounter (`BATTLE_TYPE_NORMAL`) the party strikes first one time in 16, and
otherwise the enemies one time in 32; the Djinn tutorial (`FLAG_DJINN_LESSON`)
always gives the party the first strike. When the party strikes first, no
enemy acts in that round and running away cannot fail. When the enemies do,
the party chooses no commands, and two to five enemies, drawn at random, act
once each. The mode returns to normal at the round's end.

`BattleFlee_CheckSuccess` (`round.c`) gives a 50 percent chance, plus 20
for each failed try in the battle, plus 5 for each level the party's average
is above the enemies' (or less, below). It always fails when the battle
cannot be escaped (`BATTLE_TYPE_NO_ESCAPE`). A failed escape turns the rest
of the round into one the enemies began.

| Name | Value | Meaning |
| --- | --- | --- |
| `FIRST_STRIKE_NONE` | 0 | neither side has the jump |
| `FIRST_STRIKE_PARTY` | 1 | the party strikes first |
| `FIRST_STRIKE_ENEMIES` | 2 | the enemies strike first |
| `BATTLE_TYPE_NORMAL` | 0 | a random encounter: the first-strike rolls are made |
| `BATTLE_TYPE_NO_ESCAPE` | 2 | a battle the party cannot run from, such as a Mimic's |
| `BATTLE_TYPE_SCRIPTED` | 3 | a battle a [script](../../glossary.md#script) starts; the command menu refuses to run |
| `BATTLE_TYPE_LINK` | 4 | a link battle |
| `BATTLE_TYPE_DEBUG` | 5 | the debug battle menu's battle; the party may run |

## Planning an action

Just before a unit acts, the player reads its announcement, such as
`Isaac casts Quake!`. By then the game has turned the chosen command into a
plan: which ability it is, who it reaches, and the rolls the animation must
show.

`BattleTurn_Build` ([`battle/plan.c`](battle/plan.c)) turns the chosen
command into an ability id, shows its announcement, picks its targets and
makes the rolls that must be known before the effect plays. It returns 0 to
stage the plan, or `TURN_REPORTED` when the action failed and its message
has been shown. The plan is `struct BattleTurn` (`battle_type.h`), here
with its comments shortened:

```c
struct BattleTurn {
  u8 actorId;
  s8 targetCount;
  u8 targetIds[BATTLE_TARGET_CAPACITY];          /* in battlefield order */
  s8 targetOffset[BATTLE_TARGET_CAPACITY];       /* places from the aimed one */
  s8 targetMultiplier[BATTLE_TARGET_CAPACITY];   /* 0 miss, 1, 2 or 3 blows */
  s8 targetCritical[BATTLE_TARGET_CAPACITY];     /* enum BattleCritical */
  s8 targetHit[BATTLE_TARGET_CAPACITY];          /* added effect landed */
  s16 command;                                   /* enum BattleCommand */
  u8 padding[2];
  s32 abilityId;
  s32 element;
  s32 outcome;               /* enum BattleStaging */
  u32 animation;     /* actflags.tsv's row: STAGING_ */
  s32 failure;
  u32 hauntDamage;
};
```

| Name | Value | Meaning |
| --- | --- | --- |
| `TURN_REPORTED` | -1 | the action failed and its message has been shown |

### Commands and ability ids

The command the actor chose decides which ability id the plan carries.

- `COMMAND_ATTACK` is `ABILITY_ATTACK`, unless the equipped weapon unleashes
  (`RollWeaponUnleashFar`): then the weapon's `unleash_action` replaces it,
  at a chance of five times its `unleash_chance` percent plus what the
  equipment adds. The player reads `<unit>'s <weapon> lets out a howl!` and
  then the ability's name.
- `COMMAND_PSYNERGY` is the ability chosen in the menu. Its PP cost is paid here,
  only when the cast goes ahead: a sealed caster, or one short of PP, pays
  nothing.
- `COMMAND_DJINN` reads the Djinni's ability from `../game/djinn.tsv`
  (`Djinn_GetAbilityIdFar`). Only a set Djinni is unleashed, and it then goes
  on standby; one on standby is set again instead, and one still recovering
  says so.
- `COMMAND_ITEM` takes the item's `action_id`. An item with none, or a broken
  one, makes the user defend. What using it costs is the item's `use_type`
  ([Game rules](../game/README.md)), spent in `staging.c` after the effect.
- `COMMAND_SUMMON` takes the summon's ability from `../game/summons.tsv` and
  marks the standby Djinn it uses.
- `COMMAND_ENEMY_ABILITY` is an enemy's ability, chosen as above.

The plan takes its [element](../../glossary.md#the-four-elements) from the ability's row, except that a plain
Attack strikes with the weapon's element, or an enemy's with its own
(`Item_GetEquippedElement`).

| Command | Value | Ability |
| --- | --- | --- |
| `COMMAND_ATTACK` | 0 | `ABILITY_ATTACK`, or the weapon's unleash |
| `COMMAND_PSYNERGY` | 1 | the ability chosen in the menu |
| `COMMAND_ITEM` | 2 | the item's `action_id` |
| `COMMAND_DEFEND` | 3 | Defend |
| `COMMAND_ENEMY_ABILITY` | 4 | an enemy's ability |
| `COMMAND_DJINN` | 5 | the Djinni's ability |
| `COMMAND_SUMMON` | 6 | the summon's ability |
| `COMMAND_TOTAL_DEFENSE` | 7 | an enemy's Total Defense |
| `COMMAND_NONE` | 8 | nothing this turn |
| `COMMAND_FLEE` | 99 | running away |

### Targets and range

Each side stands in a line, and a unit's position in that line is its place.
An ability reaches the places within its range of the aimed one, counted in
places rather than in distance on the screen.

![Who an ability reaches. Reach is counted in places along the enemies' line, not in distance on the screen; Quake reaches the aimed enemy and one place either side. Aimed in the middle of five enemies it hits three. Aimed at the end of the line it hits two: the reach does not go round to the other end. A downed enemy beside the aim is passed over, but its place still counts, so the enemy beyond it is missed. While the player aims, the cursor steps along the line and wraps from the last place to the first.](https://alchemy.pascalpixel.com/illustrations/battle/aim.svg)

`BattleTurn_SetTargets` walks the target side's places outward from the
aimed one, as far as the ability's `range`: 1 is the aimed unit alone, each
step more adds a place on either side, and `ABILITY_RANGE_ALL` is the whole
side, with every target treated as the aimed one. It passes over a place
whose unit has left the battle, and a downed unit unless the ability revives,
but a downed unit still holds its place and counts as a step. It keeps each
target's distance from the aimed place, at most `TARGET_MAX` targets. If no
one in range is left, the actor defends (`<unit> is defending!`). The player
sees the same range as a cursor and smaller markers while aiming
(`BattleTarget_RunSelection`, `../window/battle_command/targets.c`).

| Name | Value | Meaning |
| --- | --- | --- |
| `ABILITY_RANGE_ALL` | 255 | the whole side, every target treated as the aimed one |
| `TARGET_MAX` | 14 | targets a plan holds at most |
| `PLACE_PARTY` | `0x100` | added to a party place to make its place code |
| `PLACE_ENEMY` | `0x180` | added to an enemy place to make its place code |

### Where each place stands

Seen from above, the battlefield is a plane in pixels, with x between the
two sides and z along each line. `BattleUnit_RefreshPlacement` stands the party at x 80, each
member at the z of their place in `gBattlePlacement_StepPairs`, and
`BattlePlacement_LayoutEnemies` lines the enemies up at x -80, each taking
38 pixels of the line if large and 27 if small. Four large enemies therefore
stand 38 pixels apart, from z 45 to z -69, off centre because the line
starts from an estimate of 30 pixels an enemy. None of this changes who an
ability reaches.

### Rolls made in advance

When an enemy dodges Isaac's attack, the player reads
`Ivan nimbly dodges the blow!` and no damage follows; when Isaac lands a
critical hit, the blow bursts in light. The animation has to know the outcome before it starts, so a few rolls
are made while the action is planned, before anything is drawn.

![Rolls made before the animation plays. A plain Attack lands 31 times in 32 and misses once; a deluded attacker also misses 153 times in 256, and a target that can't act is never missed. Separately, 1 blow in 32 is a critical hit, which deals a quarter more and then adds the target's level divided by 5, plus 6. An ability that strikes more than once makes one roll for all its targets: 153 in 256 for Gust, Rapid Smash, Slice and Sonic Smash, 1 in 4 for Flying Attack, Double Fang, Twin Beaks and Swift Strike. Sonic Smash and Swift Strike strike three times, the others twice, and the animation shows each blow.](https://alchemy.pascalpixel.com/illustrations/battle/rolls.svg)

The plan makes the rolls the effect must know before it plays: whether a plain
Attack misses or lands a critical hit, and how many blows a multiple-blow
ability strikes.

A plain Attack never misses a target that cannot act (down, asleep, stunned,
charmed or held by Ground). Otherwise it misses one time in 32, and a deluded
attacker also misses 153 times in 256; the Djinn tutorial's attack always
misses. A miss sets the target's multiplier to 0, and the player reads
`<unit> nimbly dodges the blow!`.

A critical hit (`CRITICAL_HIT`) takes two rolls. The first lands one time in
32. If it misses, the second lands at half the percentage the attacker's
equipment adds to its weapon's unleash chance
(`Equipment_GetUnleashRateBonus`): equipment adding 10 percent gives a 5
percent second chance, and none gives none.

An ability with a double or triple blow makes one roll, and a success
applies to every target: 153 in 256 for Gust, Rapid Smash, Slice and Sonic Smash, and one time in four for
Flying Attack, Double Fang, Twin Beaks and Swift Strike. Sonic Smash and Swift
Strike strike three times. A success raises the targets' multiplier, which
`BattleAbility_ApplyToTarget` multiplies the damage by, and the effect's variant, so the
animation shows the extra blows. `enum AbilityEffect` also has blows one time
in eight and an ability's own critical hit (`CRITICAL_STRONG`, half as much
again), but no ability uses them.

Annihilation rolls its instant death here, at full rate whatever the
distance, so that its effect can show whether it landed. Every other added
effect is rolled per target in `BattleAbility_ApplyToTarget`.

| Name | Meaning |
| --- | --- |
| `CRITICAL_HIT` | an attack's critical hit: a quarter more damage |
| `CRITICAL_STRONG` | an ability's own critical hit, half as much again; no ability uses it |

### Staging and announcement

The plan records how its action is staged, from `actflags.tsv` and
`actstat.tsv`, and the announcement the player reads before it plays. Three
words name the parts of how it looks:

- the **outcome** (`BattleTurn.staging`) is the staging around the
  animation: whether the actor runs up, casts, hides, or nothing moves;
- the **effect kind** is which animation plays;
- the **variant** is how strong a version of it.

![A test recording: Isaac casts Quake at Garet, Ivan and Mia seven times, frame for frame, with the plan's outcome forced to a different value each time, from `STAGING_CAST`, Quake's own, to `STAGING_FULL_SCREEN`. As a cast, a summon or a full-screen move, rings of light gather round Isaac before the rock pillars rise. As an item or a Djinni, the pillars rise with no gathering. As an enemy's cast, yellow streaks burst round Isaac instead of the rings. As an escape, Isaac turns and runs and the battle ends. The six that cast deal the same damage: `Garet takes 14 damage!!!`, `Ivan takes 15 damage!` and `Mia takes 12 damage!`.](https://alchemy.pascalpixel.com/guides/battle/staging.gif)

`actflags.tsv` gives the plan's presentation flags: the effect kind in the
low twelve bits (`ANIM_ID_MASK`), the variant above them, and the bits
that choose the effect's entry point (`ANIM_BY_ID_RANGE`,
`ANIM_SUMMON`). [Battle effects](../effect/README.md#how-a-move-picks-its-effect)
gives the whole bit layout.

The outcome comes from `actstat.tsv` where its row is not 0. Otherwise an
ability that costs PP, or Psy Drain (`Ability_IsPsynergy`), is a cast
(`STAGING_CAST`); another ability with an effect to draw is a cast too, or
`STAGING_ENEMY_CAST` for an enemy; and one with nothing to draw shows only
its messages (`STAGING_MESSAGES`). An item's use is staged as
`STAGING_ITEM` unless its outcome is a summon or full screen.
`actstat.tsv` gives `STAGING_FULL_SCREEN` to Gaia, Mother Gaia, Grand Gaia,
Rockfall, Rockslide, Avalanche and Procne: the camera turns, the side that
isn't targeted fades out, and only the actor and the target side stay on the
field while the effect plays (`BattleStaging_HideField`). It
gives `STAGING_ENEMY_CAST` to Slice and seven weapon unleashes, so they play
with the element's streaks in place of the gathering star field.

The announcement follows the command: `<unit> attacks!`,
`<unit> casts <ability>!`, `<unit> unleashes <ability>!` for a Djinni,
`<unit> summons <ability>!`, and `<unit> uses <item>!`, or
`<unit> raises the <item>!` for an item the user keeps. An enemy's ability is
`<unit> uses <ability>!`, or `<unit> unleashes <ability>!` for
`ABILITY_FORMULA_ENEMY_DAMAGE`, and some enemy moves have wordings of their
own, chosen by ability id in `BattleTurn_Build`.

| Name | Value | Meaning |
| --- | --- | --- |
| `STAGING_MESSAGES` | 1 | no animation: each target's events only |
| `STAGING_ATTACK` | 2 | the actor runs up and strikes |
| `STAGING_CAST` | 3 | Psynergy, and Venus, Mercury, Mars and Jupiter |
| `STAGING_ITEM` | 4 | an item |
| `STAGING_SUMMON` | 5 | the larger summons, the actor hidden |
| `STAGING_DJINNI` | 6 | a Djinni with no attack |
| `STAGING_FLEE` | 7 | a unit runs |
| `STAGING_ENEMY_CAST` | 8 | the element's streaks in place of the star field |
| `STAGING_FULL_SCREEN` | 9 | the camera turns and the side not targeted fades out |
| `ANIM_ID_MASK` | `0xfff` | the effect kind |
| `ANIM_VARIANT_MASK` | `0x3000` | the effect's variant, from bit 12 (`ANIM_VARIANT_SHIFT`) |
| `ANIM_BY_ID_RANGE` | `0x4000` | the effect is chosen by ability id range |
| `ANIM_SUMMON` | `0x8000` | the effect is a scene seen from the actor |

## How an ability resolves

Once the animation shows the blow landing, each target in turn learns what
the ability does to it: how much damage it takes or how much it heals, and
whether the ability's added effect (Sleep, a lowered Defense, a poison) takes
hold. For damage the game takes the ability's power, scales it by the
element and by how far the target stands from the one aimed at, and adds a
little luck.

![How Psynergy damage is worked out, for Isaac's Avalanche at three enemies: its power of 160, times 281 in 256 for Isaac's Venus Power 20 ahead, is 175; the distance keeps 100, 80 and 60 percent, 175, 140 and 105; 0 to 3 is added at random; a guard halves it or cuts it to a tenth. Every step rounds down.](https://alchemy.pascalpixel.com/illustrations/battle/damage.svg)

`BattleAbility_ApplyToTarget` ([`battle/target_exec.c`](battle/target_exec.c)) turns the
plan into what happens to one target: its damage or healing, its added
effect, and the events that show them. It draws nothing and never waits: it changes the
units at once and pushes the events that show the change, in the order the
player sees them. It runs in seven phases, marked in its body:

1. It copies the target, to measure stat changes later; finds the target's
   distance from the aimed place; finds the element's affinity; and takes the
   actor's [Power](../../glossary.md#power-and-resist) in the ability's element, or 100 for an item's user.
2. It rolls the Djinn bonus and whether the added effect lands.
3. It handles an enemy calling an ally.
4. It applies the added effects that change the damage step.
5. It deals the damage or healing, by the ability kind.
6. It applies the added effect.
7. It rolls waking from damage, recalculates the target's stats and gathers
   the evil spirit's share for a haunted actor.

| Name | Value | Meaning |
| --- | --- | --- |
| `FULL_RATE` | 256 | 1 in the multipliers, which are counted in 256ths |
| `ABILITY_FORMULA_MASK` | 15 | the bits of an ability's `flags` that are its kind |

### The ability kind

The ability kind chooses the formula in phase 5. The formulas are in
[`../game/game/battle_calc.c`](../game/game/battle_calc.c), in the `game`
[bank](../../glossary.md#bank), so the battle calls them by their
[far-call](../../glossary.md#far-call) names (`../system/far_call/GAME.S`):
`Battle_CalculateRecoveryFar` leads to `Battle_CalculateRecovery`.

| Ability kind | What the target takes | What the power is |
| --- | --- | --- |
| `ABILITY_FORMULA_HEAL_HP` (Cure, Ply, Herb) | HP back, by the healer's Power in the element (`Battle_CalculateRecoveryFar`) | the HP restored at Power 100 |
| `ABILITY_FORMULA_EFFECT` (Sleep, Impair, Drain) | damage only when the added effect lands, and otherwise `But it has no effect on <unit>!` | the damage at an even match |
| `ABILITY_FORMULA_ATTACK` (Attack, Ragnarok) | a blow, the actor's Attack against the target's Defense (`Battle_CalculateAttackDamageFar`) | damage added to a plain blow |
| `ABILITY_FORMULA_ATTACK_MULTIPLIED` (Flint, Quick Strike) | a plain blow times the power | the multiplier, in tenths |
| `ABILITY_FORMULA_DAMAGE` (Quake, Avalanche) | damage scaled by the actor's Power against the target's Resist (`Battle_CalculatePsynergyPowerFar`) | the damage at an even match |
| `ABILITY_FORMULA_ENEMY_DAMAGE` (Fire Blessing, War Cry) | as `ABILITY_FORMULA_DAMAGE`, falling off faster | the damage at an even match |
| `ABILITY_FORMULA_SUMMON` (Ramses, Judgment) | as `ABILITY_FORMULA_DAMAGE`, with a share of the target's maximum HP added to the power, falling off by its own table | the damage at an even match, before the share |
| `ABILITY_FORMULA_PP_DAMAGE` (Crazy Voice) | PP lost, as `ABILITY_FORMULA_DAMAGE` | the PP lost at an even match |
| `ABILITY_FORMULA_HEAL_PP` (Psy Crystal, Adept Ring) | PP back, as `ABILITY_FORMULA_HEAL_HP` | the PP restored at Power 100 |

An even match is the actor's Power in the element equalling the target's
Resist in it. `ABILITY_FORMULA_BLANK` (only the unused rows named `?`),
`ABILITY_FORMULA_EFFECT_ONLY` (Granite, Flash, Kite) and `ABILITY_FORMULA_NO_POWER`
(the field Psynergy, Defend, Flee, the enemies' commands and the stat-raising
items) apply no power. Every other kind except the two attacks skips its
formula when the power is 0, so such an ability does only its added effect;
an attack's power is only added to the blow, so Attack, whose power is 0,
still strikes. Ember is `ABILITY_FORMULA_HEAL_PP` with a power of 0: its PP
comes from its added effect.

A summon called with `COMMAND_SUMMON` adds 3 percent of the target's maximum HP
to the power for each Djinni it uses, so 3 to 12 percent. HP healing,
attacks and HP damage then add a random 0 to 3; an added effect's damage, PP
damage and PP healing do not. A guarding target takes half the damage, or a
tenth behind Total Defense (`REDUCE_BY_GUARD`). A blow always does at least
1 damage; Psynergy damage can come to 0. A critical hit adds a quarter to a
blow, then the target's level divided by 5, plus 6.

`ABILITY_FORMULA_ENEMY_DAMAGE` is the enemies' Blessings, gases, howls and
similar moves, which is why most of them are announced as
`<unit> unleashes <ability>!`. A weapon's unleash is unrelated: it replaces
Attack with the weapon's own ability, nearly always `ABILITY_FORMULA_ATTACK`.

### The element multiplier

Garet's Mars Psynergy hurts an enemy that resists Mars poorly far more than
one that resists it well. Each unit has a Power and a Resist in each of the
four elements, and the gap between the attacker's Power and the target's
Resist decides how much the element counts.

![The element multiplier, as a graph against how far the attacker's Power in the ability's element is ahead of the target's Resist in it, from 200 points behind to 200 ahead. For Psynergy it is a straight line from nothing at 200 behind, through 1 at an even match, to double at 200 ahead: half a percent a point. A blow feels the element half as much, from 0.5 to 1.5. Past 200 points either way it stops changing. Avalanche's 20 points ahead in the worked example gives about 1.1. An ability with no element is always at 1.](https://alchemy.pascalpixel.com/illustrations/battle/element-multiplier.svg)

The element bonus is the actor's Power in the ability's element less the
target's Resist in it. `Battle_GetElementMultiplier` turns it into a
multiplier read from the five points of `gElement_PowerResistCurve`
(`../game/element_curve.s`), which lie on one line: 1 at an even match,
moving half a percent for each point, from 0 at 200 points behind to 2 at
200 ahead. A blow feels the element at half strength, from 0.5 to 1.5. An
ability with no element (`ELEMENT_NONE`) gets no bonus. Healing uses the
healer's Power alone: each point above or below 100 moves it one percent,
from nothing at 0 to double at 200, and an ability with no element heals as
if at 100. The multipliers are counted in 256ths (`FULL_RATE`), and every
result rounds down.

A unit's Power and Resist come from its element levels
(`gElement_PowerResistByLevel`) or, for an enemy, its element preset, with
the equipment's added by `Unit_Calculate`. A Resistance stage adds 20 Resist in
every element (`RESISTANCE_PER_STAGE`). A summon's boost, applied at the end
of the summoner's turn, raises the element it spent most Djinn of by 10, 30,
60 or 100 Power for one to four Djinn (`BattleUnit_ProcessTurnEnd`).

The damage message says how the element fared against the target's base
Resist, before equipment and stages (`baseElement`): `<unit> takes <damage>
damage.` when the ability's element is the one the target resists best,
`damage!!!` when it is the one it resists worst, and `damage!` otherwise. A
target that resists all four alike gets `damage!!!`.

A Djinni that strikes the element the target resists worst may also earn the
Djinn bonus, at a chance of the actor's Power less the target's Resist, plus
30 percent: an enemy it fells then gives more coins and experience and a
better chance of its item (`BattleEnemy_RecordDefeat`).

| Name | Value | Meaning |
| --- | --- | --- |
| `RESISTANCE_PER_STAGE` | 20 | Resist added in every element by one Resistance stage |

### Distance

An ability that reaches several enemies hits the one aimed at hardest and
the ones beside it less. Range and distance are both counted in places in
the line-up, not in pixels, and every kind but the attacks falls off with
the target's distance from the aimed place.

![The fall-off with distance. Five enemies in a line, the middle one aimed at. A range of 1 reaches the aimed enemy alone, 2 one place either side, 3 two places either side. Each place away keeps a smaller share: Psynergy damage and healing 100, 80 and 60 percent; a summon 100, 70 and 40; the enemies' unleashed moves 100, 50 and 30; an added effect's chance 100, 60 and 30. A downed enemy still holds its place, and an ability that hits a whole side counts everyone as aimed.](https://alchemy.pascalpixel.com/illustrations/battle/distance.svg)

![Isaac casts Avalanche at Ivan, Garet and Mia: the ground swells under them, a narrow wall of rock rises with rocks tumbling up it and widens until it fills the screen, and Ivan takes 203 damage, Garet 160 and Mia 120.](https://alchemy.pascalpixel.com/abilities/avalanche.gif)

In the recording Isaac aims at Ivan, with no one on Ivan's other side, so
Garet is one step away and Mia two.

The share each step keeps comes from the tables in `battle_calc.s`. The
figures are percentages:

| Steps from the aimed place | 0 | 1 | 2 | 3 | 4 | 5 |
| --- | --- | --- | --- | --- | --- | --- |
| `ABILITY_FORMULA_DAMAGE`, `ABILITY_FORMULA_HEAL_HP`, `ABILITY_FORMULA_HEAL_PP` | 100 | 80 | 60 | 40 | 20 | 10 |
| `ABILITY_FORMULA_SUMMON` | 100 | 70 | 40 | 30 | 20 | 10 |
| `ABILITY_FORMULA_ENEMY_DAMAGE`, `ABILITY_FORMULA_EFFECT`, `ABILITY_FORMULA_PP_DAMAGE` | 100 | 50 | 30 | 10 | 10 | 10 |
| an added effect's chance (`gRangeFalloff_EffectChance`) | 100 | 60 | 30 | 30 | 30 | 30 |

Avalanche is `ABILITY_FORMULA_DAMAGE` with a power of 160 and a range of 3, the
aimed foe and two places on either side. At an even match `Battle_CalculatePsynergyPowerFar`
gives 160, so the aimed foe takes 160 to 163 with the random, a foe one place
away 128 to 131 and one two places away 96 to 99. With the actor's Power 20
ahead, the multiplier is 281 in 256ths, and the three take 175 to 178, 140 to
143 and 105 to 108; a defending aimed foe takes 87 to 89.

The blows work differently. With Attack 100 against Defense 60 at an even
match, a plain Attack deals half of Attack less Defense, 20 to 23 with the
random. Ragnarok (`ABILITY_FORMULA_ATTACK`, power 35) adds twice its power
before the halving, 55 to 58. Flint (`ABILITY_FORMULA_ATTACK_MULTIPLIED`, power
16) multiplies the plain blow by 1.6, 32 to 35. Cure (`ABILITY_FORMULA_HEAL_HP`,
power 70) restores 70 to 73 HP from a healer with Power 100 and 83 to 86 from
one with 120, and the Healing Ring, whose `action_id` is Cure's row, always
restores 70 to 73 because an item's user counts as Power 100.

### Whether the added effect lands

When Ivan casts Sleep at a group, some enemies fall asleep and the player
reads `But it has no effect on <unit>!` for the others. Each target gets its
own roll, and the chance depends on the effect, on the two units' element
levels, on the target's Luck and weaknesses, and on its distance from the
aimed place.

![Will the added effect land? Ivan casts Sleep. The chance starts from the effect's own rate, 45 percent for Sleep, 40 for Stun. It gains 3 percent for each level Ivan's Jupiter is above the target's, loses 3 percent for every 2 points of the target's Luck, and gains 25 percent if Sleep is one of the target's three weaknesses. The distance then scales it: all of it for the aimed target, 60 percent next to it, 30 percent further. Some chances are fixed and changed only by distance: the Djinni Quartz 60 percent, Dew 90, and an effect with no rate of its own 100.](https://alchemy.pascalpixel.com/illustrations/battle/effect-chance.svg)

![Ivan casts Confuse at Isaac, Garet and Mia: a pale blue mist rolls over the three, and the messages read `Isaac is confused!` and `But it has no effect on Garet!`](https://alchemy.pascalpixel.com/abilities/confuse.gif)

`Battle_RollEffectSuccess`, reached through its far-call entry
`Battle_RollEffectSuccessFar`, rolls it, unless the plan rolled it already. It fails at once when there is nothing to do: a revival on
a unit still standing, a cure on a unit without the ailment, Curse on a unit
whose countdown is at its last turn. Otherwise the chance starts from the
effect's base rate (`Battle_GetEffectBaseSuccessRate`: Sleep 45 percent,
Taint 55, Condemn 20 and so on), gains 3 for each element level the actor is
ahead of the target, loses 3 for every two points of the target's Luck, and
gains 25 when the effect is one of the three the target is weak to. The
distance table then scales it. Some effects have a fixed chance instead,
which levels, Luck and weakness don't change, stored as a negative base rate:
Quartz 60 percent, Dew 90, and 100 for an effect with no rate of its own.
Luff's seal rolls up to three times and lands if any roll does.

When the added effect of an `ABILITY_FORMULA_EFFECT` ability fails, the target
takes no damage either.

### Calling an ally, and effects that change the damage step

Some added effects stand in for the damage or change how it is dealt, and
are applied before it, in phases 3 and 4.

Phase 3 handles Ally Search, Search for Help and Speed Surge. When the roll
succeeds, a free unit id exists and the enemy [sprite](../../glossary.md#sprite-and-oam) slots have room
(`Enemy_HasSpriteRoom`), the new enemy fills an empty place or is appended, the
enemies are laid out again, and the player reads `<unit> appears!!` or
`It split off into <unit>!`; otherwise `But no one came!` or
`It failed to divide!`.

Phase 4 runs only for an effect that landed:

- Torch's blow counts half the target's Defense.
- Severe Blow and Thrash leave the target 1 HP at most.
- Condemn and the other instant deaths skip the damage.
- Dynamite and Mortal Blow remove their user from the battle.
- Psy Drain becomes `ABILITY_FORMULA_PP_DAMAGE`, or misses a target with no PP.
- Ground, which costs its target the next action, is decided here: it holds
  a target only while it still has an entry in the round's action list.
  `BattleStaging_DispatchAction` clears each entry as it plays it, so a
  target that has already acted is left alone.

### Applying the added effect

A deluded enemy stays deluded for a while and then, sometimes early, the
player reads that it `sees clearly once again!`. Statuses and stat changes
are counters on the unit that the end of each round counts down.

![How long a status lasts, over the seven round ends after it lands. A status such as Delude or Sleep, or a lowered stat, lasts 7 turns; after the first round end it is sure to last, and from 5 turns left each round end may end it early, more often for a lucky unit and as the turns run out, until the seventh ends it anyway. A raised stat always lasts all 7 turns. One stage moves Attack or Defense by an eighth, four stages at most.](https://alchemy.pascalpixel.com/illustrations/battle/status-turns.svg)

Phase 6 applies the effect when it landed and the target is still standing.
A revival needs the target down instead, and a drain still works on a target
its own damage has just felled (`Ability_EffectAppliesToDowned`). The case comments
in `BattleAbility_ApplyToTarget` and the notes on `enum AbilityEffect`
([`battle_unit.h`](../../include/battle/battle_unit.h)) give each effect's
detail; the rules they share are these.

- A stat change moves Attack, Defense or Resistance one or two stages,
  between `STAT_STAGE_MIN` and `STAT_STAGE_MAX`, for `STAT_CHANGE_TURNS`.
  A stage is an eighth of Attack or Defense. The number in the message is
  how far the stat moved, measured against the copy from phase 1, so a stat
  at its limit shows 0. Zephyr and Vine set the Agility stage outright, to
  double or half, for `AGILITY_CHANGE_TURNS`.
- A status (Delude, Confuse, Charm, Paralyze, Sleep) and Bind's seal last
  `STATUS_TURNS`, counted down at the end of each round. Luff's seal
  (`PSY_SEAL_LUFF`) lasts two turns that never end early. Taint does nothing
  to a poisoned unit, and Poison's deadly poison replaces plain poison. Curse
  starts the Grim Reaper's countdown of `CURSE_TURNS`, and each later curse
  brings it a turn closer.
- A lowered stat and a status may end early: once five turns or fewer are
  left, the end of each round rolls for it, more often for a lucky unit and
  as the turns run down (`BattleUnit_RollCounterEarlyEnd`). A raised stat and
  the Agility change always last their turns.
- Restore and Elixir clear delusion, stun, sleep, the seal and the Grim
  Reaper's countdown. Tonic clears those, poison and the evil spirit. Neither
  clears confusion or charm, which wear off or end when the unit goes down.
- Drain gives the actor all the HP the target lost, Sap half, Psy Drain the
  PP it took, and Psy Leech a tenth of the damage as PP.
- Defend and Granite set `GUARD_DEFEND`, Total Defense and Flash
  `GUARD_TOTAL_DEFENSE`; the message shows either way, but a guard is never
  lowered. Break returns every raised stage to 0 and removes a summon's
  boost. Kite lets the target act twice next turn, Ground costs it its next
  action, and Haunt sets an evil spirit on it.

| Name | Value | Meaning |
| --- | --- | --- |
| `STAT_STAGE_MIN` | -4 | the lowest stat stage: half the stat |
| `STAT_STAGE_MAX` | 4 | the highest: one and a half times the stat |
| `AGILITY_STAGE_DOUBLED` | 8 | Zephyr's Agility stage: twice the Agility |
| `STAT_CHANGE_TURNS` | 7 | turns a stat change lasts |
| `AGILITY_CHANGE_TURNS` | 5 | turns an Agility change lasts |
| `STATUS_TURNS` | 7 | turns a status or Bind's seal lasts |
| `CURSE_TURNS` | 7 | the Grim Reaper's countdown |
| `PSY_SEAL_BIND` | 7 | Bind's seal: seven turns, in the low three bits |
| `PSY_SEAL_LUFF` | 16 | Luff's seal: two turns of 8, never ending early |
| `GUARD_DEFEND` | 1 | Defend and Granite: half the damage |
| `GUARD_TOTAL_DEFENSE` | 2 | Total Defense and Flash: a tenth of the damage |

### After each target

A sleeping unit that is hit may wake up, and a unit haunted by an evil
spirit pays for its own attacks: once a target has been dealt with, the
damage it took may wake it and may feed the haunted actor's evil spirit,
which takes its share from the actor at the end of its turn.

![After each target. Isaac, haunted by an evil spirit, casts Quake at Garet, Ivan and Mia, and Ivan is asleep; here every roll comes up. A sleeping target that loses HP wakes one time in four, unless it fell asleep this round. One time in four for each target, the evil spirit keeps a quarter of what the target lost, at least 1. Garet loses 14 HP and is awake; the spirit's total is 3 HP. Ivan loses 15 HP and wakes up; the total is 6 HP. Mia loses 12 HP; the total is 9 HP. At the end of Isaac's turn the message reads `A curse does 9 damage to Isaac!`, and his HP falls from 625 to 616.](https://alchemy.pascalpixel.com/illustrations/battle/after-target.svg)

Losing HP or PP wakes a sleeping target one time in four, unless it fell
asleep this round. A haunted actor, one time in four for each target it took
HP or PP from, adds a quarter of that loss (at least 1) to
`BattleTurn.hauntDamage`, and takes the total from its own HP at the end of
its turn (`BattleUnit_ProcessTurnEnd`).

In the Djinn tutorial a Djinni's blow always leaves its target 1 HP, and a
summon always fells it.

## Rules and pictures meet at the event queue

When Quake hits Garet, his HP drops at once, but the player sees it happen
in steps: Garet flinches, the message `Garet takes 14 damage!!!` appears and
waits for A, and only then does the next target's turn come. The rules leave
a list of what to show, in order, and the screen plays the list.

![One target's events, played in order. Quake reaches Garet. The rules work out his damage at once, take 14 HP off, and queue what the player should see: the hit, which makes him flinch and flash for 9 frames; the number 14 and the name Garet, which take no time and fill in the message's blanks; the message, which waits until A is pressed; and a refresh that redraws him. A few more events that take no time follow, then the next target. The number and name go first because the message fills its blanks from them as it is written.](https://alchemy.pascalpixel.com/illustrations/battle/event-queue.svg)

The battle event queue (`struct BattlePlaybackQueue`, `BATTLE_PLAYBACK_CAPACITY`
events) is the one channel from the rules, which change the units at once,
to the screen, which shows the changes one by one while the effect plays.
`BattleAbility_ApplyToTarget` never draws or waits, and an effect never computes an
outcome. [Battle](../../battle.md#rules-and-effects-run-together) shows the
code that runs `BattlePlayback_Run` as a [task](../../glossary.md#task)
while the effect's own loop waits: playback runs each target's rules once
the effect calls `ANIM_SHOW_DAMAGE`, and a few effects that never call it
(`atalanta.c`, `djinn_heal.c` and `torch.c`) have playback started by
`BattlePlayback_WaitForReady` once they return.

In the recording of Quake that [Battle](../../battle.md#rules-and-effects-run-together)
shows, the rock plays for about two seconds before its blow lands
and starts playback. Playback then resolves the three targets in the plan's
order, the battlefield's (Garet, Ivan, Mia), not the aimed Ivan first, and
each target's `BattleAbility_ApplyToTarget` queues the same eight events. Only the hit
(nine [frames](../../glossary.md#frame) of flashing) and the message (until the player presses A) make
playback wait; the number and name it pushes before the message, and the
name pushed for an added effect Quake doesn't have, the clear and the
refreshes after it, all play within one frame.

A message takes its names and numbers from the window system's list of
message arguments as it is written out, so they are pushed before it, and a
message uses them up, so each later message pushes the name again.
`BattlePlayback_RunAll` plays the same queue straight through for an
outcome shown outside an effect, such as a cast that is sealed or short of
PP.

| Name | Value | Meaning |
| --- | --- | --- |
| `BATTLE_PLAYBACK_CAPACITY` | 64 | events the queue holds |

## Design

### Results nobody reads

Some of the original's functions promise to return a value and end without
one, and no caller reads it. The source keeps the promised type where
writing `void` would change the compiled code at the function's end, and
[`UBFIX`](../../glossary.md#bug-bugfix-and-ubfix) gives the `void` form; [Matching](../MATCHING.md) explains the rule. `BattleAbility_ApplyToTarget`,
`BattlePlayback_RunAll` and `BattlePlayback_WaitForReady` are among them.

### One sprite, several structs

A unit's figure is one of the object slots `Object_CreateFar` hands out. The
battle reads it as a `struct MotionObject`, while the field's [far calls](../../glossary.md#far-call) take
it as a `struct DispatchObject` or a `struct Object`. They describe the
same record, with fields at the same offsets, and a call into the field casts
to the view its prototype names. [XXX](../../glossary.md#xxx): whether these could be one struct
depends on naming the fields only one view names so far.

## Limits

- There are 519 abilities. The plan reads `actflags.tsv` only up to
  `ABILITY_THRASH`, and `actstat.tsv`, a row shorter, up to
  `ABILITY_SEVERE_BLOW`.
- A plan reaches at most `TARGET_MAX` targets.
- The party fights `PARTY_BATTLE_SIZE` at a time and the enemies
  `ENEMY_BATTLE_SIZE` at most; in a link battle each console sends
  `LINK_PARTY_SIZE`.
- `formatn.tsv` has 380 enemy groups, each of up to five kinds of enemy with
  the least and most of each (`member0` to `member4`, `minimum0` to
  `minimum4`, `maximum0` to `maximum4`). The encounter zones in
  `../object/encounter.tsv` choose among them.
- The event queue holds `BATTLE_PLAYBACK_CAPACITY` events.

## Files

| File | What it holds |
| --- | --- |
| `battle.c` | the battle object: it includes `battle_internal.h`, then the parts in `battle/` in ROM order |
| `battle_internal.h` | the private header every part includes: the names two or more parts use |
| `battle/*.c` | the code, in 24 parts |
| `battle_data.s`, `battle_vars.s` | the smaller tables and pictures in ROM order, including where the `.tsv` tables below are placed |
| `battle_calc.s` | the fall-off tables: the share of damage, healing or an added effect's chance a target gets by its distance from the aimed place |
| `actflags.tsv` | each ability's effect kind and staging bits (`gAbilityAnimations`) |
| `actstat.tsv` | each ability's staging, where it overrides the plan's choice (`gAbilityStaging`) |
| `formatn.tsv` | the enemy groups (formations) |
| [`sprites.tsv`](sprites.tsv) | each enemy class's battle sprite |
| `presets.tsv` | the debug party presets |
| `advarrow.s` | the arrow beside a battle message that waits for the player, built from `advarrow.png`: sixteen 8 by 16 frames, of which the code cycles through the first eight, one every four frames |
| `presentation/` | the opening curtain's tiles and their variants, and the brightness steps of a unit's highlight (`highlight_ramp.s`) |
| `effect/` | two 32 by 16 sparks that `../object/arcspark.s` places for the field Psynergy effects |

The ROM holds the battle as one object, and it is built as one. The split
into parts is ours, made where one family of functions ends and the next
begins; the folder's [guide](../README.md) explains how parts are compiled.
The first lines of the parts, read in the order `battle.c` includes them,
form a table of contents. The functions are declared in
[`unit/battle/battle.h`](../../include/unit/battle/battle.h).

## Words used here

- **unit**, **unit id**: everyone in a fight. Party members keep their
  character numbers as unit ids, up to `ALLY_MAX`, and enemies take ids from
  `FIRST_ENEMY_UNIT`. `Unit_GetFar` returns a unit's `struct Unit`: its
  HP, PP, stats, statuses, stat stages and a Power and a Resist in each
  element (`Unit.element`).
- **ability**, **ability id**: anything a unit does on its turn, from Attack
  to Quake, a Djinni, a summon, an item's use or an enemy's move. The id is a
  row number in three tables: `../game/actions.tsv` (`struct AbilityDefinition`, read
  with `Ability_Get`), `actflags.tsv` and `actstat.tsv`.
- **ability kind**: the low four bits of the ability's `flags`
  (`ABILITY_FORMULA_MASK`, `enum AbilityFormula`). It chooses the formula each
  target goes through. The **effect kind** in `actflags.tsv` is unrelated and
  chooses only the animation (`enum BattleAnimId`).
- **power**: the `power` column of `actions.tsv`. What it counts depends on
  the ability kind. A unit's **Power** and **Resist**, capitalized as the
  status screen writes them, are its per-element stats.
- **added effect**: what an ability does besides damage or healing, the
  `effect` column of `actions.tsv` (`enum AbilityEffect`).
- **element level**: the level the status screen shows under each element
  (`Unit_GetElementLevel`). A character's Power and Resist start from it, and
  it moves the chance of an added effect.
- **place**, **place code**: a unit's position in its side's line-up
  (`BattleWork.partyUnits`, `BattleWork.enemyIds`). A command aims at a place
  code, the place plus `PLACE_PARTY` or `PLACE_ENEMY`.
- **side**: the party or the enemies (`BATTLE_SIDES_PARTY`,
  `BATTLE_SIDES_ENEMIES`). A unit's own `side` field says which side it is on,
  or that it has left the battle.
- **plan**: the action under way (`struct BattleTurn`): the actor, the
  ability, its element, its targets with their distances and rolls, its
  **outcome** (`enum BattleStaging`, how it is staged) and its
  **presentation flags** (the `actflags.tsv` row: `STAGING_*`).
- **battle object**: a unit's figure on the battlefield (`struct BattleObject`,
  `BattleObject_Get`). Its **records** are its sprites; the Kraken and the Fusion
  Dragon are drawn in two parts and have two (`SPRITE_WITH_SECOND_PART_A`,
  `SPRITE_WITH_SECOND_PART_B`).
- **event**: one thing the battle shows while an action plays, waiting in the
  battle event queue (`BattlePlayback_Push`, `enum BattlePlaybackCommand` in
  `battle_event.h`): a message, a name or number the next message needs, a
  flinch, a defeat, a redraw or a sound.
- **pose**: how a battle object stands (`POSE_*`, set with
  `ObjectDispatch_SelectAnimationFar`).

The parts give the same locals the same names: `battle` or `work` for
`gBattleWork`, `unitId`, `unit` for its `struct Unit`, `battleObject`
for its `struct BattleObject`, `object` for that object's
`struct MotionObject`, `record` for one of its sprites, and `plan`.

## Function prefixes

| Prefix | What it acts on |
| --- | --- |
| `Battle_` | the battle as a whole: `Battle_Run`, the round's end |
| `BattleParty_` | the unit lists: who is in the battle and who is still standing |
| `BattleQueue_` | the round's action list and its turn order |
| `BattleCommand_` | an action chosen and planned |
| `BattlePlan_` | the plan's targets |
| `BattleTarget_` | targets an enemy or a confused unit picks |
| `BattlePresentation_` | what the battle shows: the staging, the opening transition, the camera, the palettes |
| `BattleEvent_` | the event queue and its playback |
| `BattleObject_` | the battle objects |
| `BattleMotion_` | their movements |
| `BattlePlacement_` | where each place stands on the battlefield |
| `BattleUnit_` | one unit's battle state: its guard, weapon sprite, status flags and turn-end counters |
| `BattleStatusIcon_` | the status icons that cycle over a unit |
| `BattleFormation_`, `BattleEnemy_` | the enemy groups, and each enemy's level, name letter, defeat and coins |
| `BattleClass_` | an enemy class's row of the battle sprite table |
| `BattleEscape_` | running away |
| `BattleSpoils_` | the items the enemies leave |
| `BattleSummon_` | which summons the party's standby Djinn make available |
| `BattleIntro_` | the message that announces the encounter |
| `BattleBackground_`, `BattleCamera_` | the battle background and the perspective plane's zoom |
| `SummonSlot_` | the enemy sprite slots a unit's battle sprite is loaded into |
| `ElementGlow_` | a unit's elemental glow: the star field and the other glows |
| `LinkBattle_` | link battles |
| `DebugBattle_`, `DebugParty_` | the debug battle menu, its message viewer and the party presets |

A function that acts on one thing the engine names elsewhere keeps that
thing's prefix (`Camera_`, `Palette_`, `Party_`, `Ability_`, `Text_` and
others). [Matching](../MATCHING.md) explains the unusual spellings and the
[`FAKEMATCH`](../../glossary.md#fakematch) tags.

## Evidence and open questions

Abilities and enemies are named from the label columns of their tables,
`ability` and `enemy`, which follow the English release's text. The formulas
are named from what `battle_calc.c` computes, and the event kinds from what
playback does with each.

The chances and counts in the illustrations were checked in a tbs-en build
with `alchemy-devtools render`, answering `Battle_Random` with chosen
numbers. Replaying a Gryphon's turn with each of the 256 rolls gave its
actions in the spans above. One encounter (the Lizard King, a Harridan and
two Stone Soldiers) opened both as a first strike, where only the four party
members acted, and as a surprise, where three of the enemies did. Isaac's
attacks on a copy of Ivan gave a miss (`Ivan nimbly dodges the blow!`), a
critical hit of 110 and, with the Swift Sword unleashing Sonic Smash, a triple
blow of 281. Real calls of `Battle_GetElementMultiplier` fell on the line
drawn: for Quake, Ivan at 52 points ahead and 1.26 times, Mia at 51 and 1.25,
Isaac's copy at 30 behind and 0.85, and for an attack on Ivan, 8 behind and
0.98 on the blow's line. Against a Vermin, its delusion ended early with 5
turns left while both Defense changes ran their 7 turns. The haunt example
is one such run, with both rolls answered 0, and aiming Quake along four
large enemies moved the cursor from the last place round to the first.

Bits 0 and 1 of the `flags` column of `actflags.tsv` (`ANIM_ON_DOWNED`,
`ANIM_OTHER_SIDE`) are read, but no row in either game sets them. The plan
adds the first itself for the revive effects; nothing adds the second, so the
side flip it asks for is unused. `BattlePlacement_LayoutEnemies` lays out the
enemies' column, and `BattleSprite_HasHeavyImpact` reads the class bit that
makes a beam blow land with `SE_HEAVY_IMPACT`, which only the Mars Lighthouse
bosses set (Saturos's second fight, Menardi and the Fusion Dragon).

- XXX: what `BattleCamera.unk18` and `BattlePresentationTransition`'s
  `unk18` and `unk1c` hold; the battle's setup clears them and no C reads
  them, so a reader in the effect overlays or a watch on them through a
  battle would settle it.
