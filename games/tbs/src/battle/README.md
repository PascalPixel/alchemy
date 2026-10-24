# Battles

Everything the rules of a fight decide is worked out here: who acts in what
order, which enemy does what, who an ability reaches, whether a blow misses or
lands a critical hit, how much damage each target takes, and which statuses
take hold. The answers come from tables and dice rolls, and each shows up on
screen as a message such as `Ivan takes 15 damage!`.

[Battle](../../battle.md) explains a battle as a whole and is the place to
start. [Battle animations](../battle_anim/README.md) draws the animations,
[Windows](../window/README.md) holds the command menu and the
[party](../../glossary.md#the-party)'s status window, and
[Game rules](../game/README.md) holds the ability, item and enemy tables and
the arithmetic in `game/battle_calc.c`.

## Rounds

![Isaac, Garet, Ivan and Mia against the Lizard King, a Harridan and two Stone Soldiers: Ivan, Isaac and Mia attack the Harridan until it falls; Garet, whose target is gone, defends; then one Stone Soldier casts Blast, the other tries Mad Blast without enough PP, and the Lizard King casts High Impact.](https://alchemy.pascalpixel.com/guides/battle/turn-order.gif)

Each round, every unit (everyone in the fight, party member or enemy)
chooses an action, and the actions play fastest first. Here the party,
at level 35, is faster than all four enemies, so it acts first; Garet,
slowest of the four, finds the Harridan already felled and defends instead.

Who acts first is decided by Agility, highest first, with a few twists:

- The Djinn that guard or hold an enemy, Granite, Flash and Ground, go before
  everyone ([Djinni](../../glossary.md#djinn), `DJINN_FIRST_PRIORITY`).
- A party member's Agility gains up to a sixteenth more at random each round,
  so two members of nearly equal speed may swap places; an enemy's counts as
  it is.
- On a tie the party goes first, and enemies of equal Agility go in a
  shuffled order.
- A unit that acts twice in a round has its second action at half its
  Agility.
- Defend and Total Defense are raised as soon as the actions are sorted, so a
  defending unit is guarded against everything that round.

`Battle_Run` ([`battle/encounter.c`](battle/encounter.c)) fights the whole
battle. Before the first round `formation.c` turns the formation into units,
with their levels and the letters that tell two of a kind apart, and
`transition.c` plays the opening. Each round then runs these steps:

1. `round.c` gathers the actions (`Battle_GatherActions`): the party's from
   the command menu or `auto_battle.c`, the enemies' from
   `BattleAI_ChooseAction` (`enemy_ai.c`), or from the other console in a
   link battle. `Battle_CollectPartyCommands` works out each action's
   priority and `BattleTurnOrder_Sort` sorts them, keeping the order of equal
   priorities.
2. For each action, `BattleTurn_Build` (`turn.c`) announces it and fills in
   the turn, and `BattleStaging_DispatchAction` (`staging.c`) stages it and
   runs its animation, while `BattleAbility_ApplyToTarget` (`target_exec.c`)
   applies it to each target.
3. `turn_end.c` closes the actor's turn: poison takes a tenth of the maximum
   HP (deadly poison a fifth), the Grim Reaper takes a unit whose countdown
   has run out, and a [summon](../../glossary.md#summons)'s boost is applied.
4. `turn_end.c` closes the round: stat changes and statuses count down and
   Djinn recover.

After the last round `spoils.c` hands out coins, experience, items and level
ups.

| Name | Value | Meaning |
| --- | --- | --- |
| `BATTLE_OUTCOME_LOST` | -1 | `Battle_Run`'s result when the party is beaten |
| `BATTLE_OUTCOME_FLED` | 999 | its result when the party ran away |
| `PARTY_BATTLE_SIZE` | 4 | party members who fight at a time |
| `ENEMY_BATTLE_SIZE` | 6 | enemies in a battle at most |
| `LINK_PARTY_SIZE` | 3 | party members each console sends to a link battle |

### Enemy actions

![While the party defends, the first Gryphon uses Wing Flutter, a volley of light across the party, and the second casts Tornado.](https://alchemy.pascalpixel.com/guides/battle/gryphons.gif)

Every enemy has a list of eight actions and a habit for choosing among them,
and the dice do the rest. A Gryphon's list runs Wing Flutter, Tornado, Twin
Beaks, Impact, Attack, Sonic Slash, Bind and Ward, and it favours the start of
the list, so it uses Wing Flutter most and Ward least.

An enemy picks one of the eight abilities in its row of `../game/enemies.c`
by its action strategy (`enum EnemyActionStrategy`). Three strategies draw at
random by a row of `gBattleAI_ActionWeights`; the other four take the list in
order, from a random entry or the first, or repeat the last pick. A roll is a
number from 0 to 255: the Gryphon's Wing Flutter takes rolls 0 to 52, Tornado
53 to 99, Twin Beaks 100 to 140, and so on to Ward's 245 to 255.

`BattleAI_ChooseAction` runs twice for each enemy. As the round is built
(`retry` 0) it keeps its pick only if it is a guard, so the guard is raised
with the party's; at the enemy's turn (`retry` 1) it draws again for anything
else. A pick the enemy cannot use (too little
[PP](../../glossary.md#psynergy-and-pp), a seal, no target worth it) is drawn
again, and after 18 failed picks the enemy defends. An ability whose added
effect is Defend, Total Defense or Flee becomes the command `COMMAND_DEFEND`,
`COMMAND_TOTAL_DEFENSE` or `COMMAND_FLEE`.

| Strategy | Value | How the entry is picked |
| --- | --- | --- |
| `ACTION_STRATEGY_EVEN` | 0 | at random, 32 in 256 each |
| `ACTION_STRATEGY_FALLING` | 1 | at random, from 53 in 256 down to 11 |
| `ACTION_STRATEGY_FIRST_FAVOURED` | 2 | at random, the first 113 in 256, down to 5 |
| `ACTION_STRATEGY_RANDOM_START` | 3 | a random entry the first time, then each in turn |
| `ACTION_STRATEGY_IN_ORDER` | 4 | each in turn from the remembered entry |
| `ACTION_STRATEGY_NEXT_PICK` | 5 | the next entry at each new pick, from 0 |
| `ACTION_STRATEGY_SAME` | 6 | the entry last picked |

### First strikes and fleeing

![A random battle with two Gryphons opens with `Isaac's party attacks first!`, and Isaac and Garet attack while the Gryphons stand still.](https://alchemy.pascalpixel.com/guides/battle/first-strike.gif)

Now and then a random battle opens with `Isaac's party attacks first!` and the
enemies stand still for a round; more rarely it opens with
`Isaac's party was caught by surprise!`, and some enemies act before the party
can choose anything.

`BattleWork.firstStrike` records which side has the jump. In a random
encounter (`BATTLE_TYPE_NORMAL`) the party strikes first one time in 16, and
otherwise the enemies one time in 32; the Djinn tutorial (`FLAG_DJINN_LESSON`)
always gives the party the first strike. When the enemies strike first, the
party chooses no commands and two to five enemies, drawn at random, act once
each. The mode returns to normal at the round's end.

![The party, at level 20, chooses Flee against two Gryphons: `Isaac and friends run!` is followed by `But there's no escape!`, and the Gryphons use Wing Flutter and Twin Beaks. The next round the party tries again and gets away.](https://alchemy.pascalpixel.com/guides/battle/flee.gif)

Running away is a chance that grows with every failed try.
`BattleFlee_CheckSuccess` (`round.c`) gives 50 percent, plus 20 for each
failed try in the battle, plus 5 for each level the party's average is above
the enemies' (or less, below); the party at level 20 against Gryphons at level
23 starts at 35 percent. A first strike makes running certain, and it always
fails when the battle cannot be fled (`BATTLE_TYPE_NO_ESCAPE`). A failed try
turns the rest of the round into one the enemies began.

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

## Building a turn

![Isaac casts Spire at Ivan: the announcement `Isaac casts Spire!` comes first, then a spire of rock falls on Ivan and shatters, and `Ivan takes 52 damage!`.](https://alchemy.pascalpixel.com/abilities/spire.gif)

Just before a unit acts, the player reads its announcement, such as
`Isaac casts Spire!`. By then the game has turned the chosen command into a
turn: which ability it is, who it reaches, and the rolls the animation must
show.

`BattleTurn_Build` ([`battle/turn.c`](battle/turn.c)) does that and shows the
announcement. It returns 0 to stage the turn, or `TURN_REPORTED` (-1) when the
action failed and its message has been shown. The turn is
`struct BattleTurn` (`battle_type.h`), shortened:

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
  u32 animation;     /* gAbilityAnimations' row: STAGING_ */
  s32 failure;
  u32 hauntDamage;
};
```

### Commands and ability ids

![Isaac attacks Ivan with the Swift Sword: `Isaac's Swift Sword lets out a howl! Sonic Smash!`, and the sword strikes three times for 281 damage.](https://alchemy.pascalpixel.com/guides/battle/unleash.gif)

The command the player chose decides which ability the turn carries. An
Attack is usually just Attack, but a weapon can unleash its own ability in its
place, as the Swift Sword unleashes Sonic Smash.

- `COMMAND_ATTACK` is `ABILITY_ATTACK`, unless the weapon unleashes
  (`RollWeaponUnleashFar`) at five times its `unleash_chance` percent plus
  what the equipment adds; the player then reads
  `<unit>'s <weapon> lets out a howl!` and the ability's name.
- `COMMAND_PSYNERGY` is the ability chosen in the menu. Its PP cost is paid
  here, only when the cast goes ahead.
- `COMMAND_DJINN` reads the Djinni's ability from `../game/djinn_table.c`
  (`Djinn_GetAbilityIdFar`). Only a set Djinni is unleashed, and it then goes
  on standby; one on standby is set again instead.
- `COMMAND_ITEM` takes the item's `action_id`; an item with none, or a broken
  one, makes the user defend. What using it costs is spent in `staging.c`.
- `COMMAND_SUMMON` takes the summon's ability from `../game/summons.c` and
  marks the standby Djinn it uses.

The turn takes its [element](../../glossary.md#the-four-elements) from the
ability's row, except that a plain Attack strikes with the weapon's element,
or an enemy's with its own (`Item_GetEquippedElement`).

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

![Isaac aims Quake: the cursor moves along the Harridan, the Lizard King and the two Stone Soldiers, the enemies beside the aimed one lit too, and from the last it wraps round to the first.](https://alchemy.pascalpixel.com/guides/battle/aim.gif)

Each side stands in a line, and a unit's position in it is its place. An
ability reaches the places within its range of the aimed one, counted in
places rather than in distance on the screen; the player sees the reach as the
lit enemies around the cursor.

`BattleTurn_SetTargets` walks the target side's places outward from the aimed
one, as far as the ability's `range`: 1 is the aimed unit alone, each step
more adds a place on either side, and `ABILITY_RANGE_ALL` is the whole side,
every target treated as the aimed one. It passes over a place whose unit has
left the battle, and a downed unit unless the ability revives, but a downed
unit still holds its place. If no one in range is left, the actor defends
(`<unit> is defending!`). The reach stops at the ends of the line, though the
cursor (`BattleTarget_RunSelection`, `../window/screens/targets.c`) wraps.

Where the units stand on screen changes none of this.
`BattleUnit_RefreshPlacement` stands the party at x 80 and
`BattlePlacement_LayoutEnemies` lines the enemies up at x -80, each taking 38
pixels of the line if large and 27 if small.

| Name | Value | Meaning |
| --- | --- | --- |
| `ABILITY_RANGE_ALL` | 255 | the whole side, every target treated as the aimed one |
| `TARGET_MAX` | 14 | targets a turn holds at most |
| `PLACE_PARTY` | `0x100` | added to a party place to make its place code |
| `PLACE_ENEMY` | `0x180` | added to an enemy place to make its place code |

### Rolls made in advance

![Isaac attacks Ivan twice: the first time `Ivan nimbly dodges the blow!`, the second a critical hit bursts in light and `Ivan takes 110 damage!`.](https://alchemy.pascalpixel.com/guides/battle/rolls.gif)

A dodge and a critical hit look different from a plain blow, so the animation
has to know which it is before it starts. The turn therefore rolls them while
it is built, before anything is drawn.

A plain Attack never misses a target that cannot act (down, asleep, stunned,
charmed or held by Ground). Otherwise it misses one time in 32, and a deluded
attacker also misses 153 times in 256. A critical hit (`CRITICAL_HIT`, a
quarter more damage) lands one time in 32, or else at half the percentage the
attacker's equipment adds to its unleash chance
(`Equipment_GetUnleashRateBonus`).

An ability with a double or triple blow makes one roll for all its targets:
153 in 256 for Gust, Rapid Smash, Slice and Sonic Smash, one time in four for
Flying Attack, Double Fang, Twin Beaks and Swift Strike; Sonic Smash and Swift
Strike strike three times. A success raises each target's multiplier and the
animation's variant, so the extra blows show. Annihilation rolls its instant
death here too; every other added effect is rolled per target in
`BattleAbility_ApplyToTarget`.

### Staging and announcement

![Isaac casts Rockslide at Garet, Ivan and Mia: a column of light opens into a wall of fire with rocks tumbling through it.](https://alchemy.pascalpixel.com/abilities/rockslide.gif)

The staging is what happens around an animation: whether the actor runs up and
strikes, raises a hand to cast, or nothing moves. Rockslide is staged full
screen: the camera turns and the side that isn't targeted fades out.

The turn's outcome comes from `gAbilityStaging` where its row is not 0.
Otherwise an ability that costs PP, or Psy Drain (`Ability_IsPsynergy`), is a
cast (`STAGING_CAST`); another with an animation is a cast too, or
`STAGING_ENEMY_CAST` for an enemy; and one with nothing to draw shows only its
messages. `gAbilityStaging` gives `STAGING_FULL_SCREEN` to Gaia, Mother Gaia,
Grand Gaia, Rockfall, Rockslide, Avalanche and Procne
(`BattleStaging_HideField`), and `STAGING_ENEMY_CAST` to Slice and seven
weapon unleashes. `gAbilityAnimations` gives the animation word
([Battle animations](../battle_anim/README.md#the-animation-word)).

The announcement follows the command: `<unit> attacks!`,
`<unit> casts <ability>!`, `<unit> unleashes <ability>!` for a Djinni,
`<unit> summons <ability>!`, `<unit> uses <item>!`, or
`<unit> raises the <item>!` for an item the user keeps. An enemy's ability is
`<unit> uses <ability>!`, or `<unit> unleashes <ability>!` for
`ABILITY_FORMULA_ENEMY_DAMAGE`.

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

## How an ability resolves

![Mia casts Ply on Garet: motes of light swirl in to him and `Garet recovers 150 HP!`.](https://alchemy.pascalpixel.com/abilities/ply.gif)

Once the animation shows the blow land, or the healing reach its target, each
target in turn learns what the ability does to it: how much damage it takes or
how much it heals, and whether the ability's added effect (Sleep, a lowered
Defense, poison) takes hold.

`BattleAbility_ApplyToTarget` ([`battle/target_exec.c`](battle/target_exec.c))
works that out for one target. It draws nothing and never waits: it changes
the units at once and pushes the events that show the change. It runs in seven
phases, marked in its body:

1. It copies the target, to measure stat changes later; finds the target's
   distance from the aimed place; and takes the actor's
   [Power](../../glossary.md#power-and-resist) in the ability's element, or
   100 for an item's user.
2. It rolls the Djinn bonus and whether the added effect lands.
3. It handles an enemy calling an ally.
4. It applies the added effects that change the damage step.
5. It deals the damage or healing, by the ability kind.
6. It applies the added effect.
7. It rolls waking from damage, recalculates the target's stats and gathers
   the evil spirit's share for a haunted actor.

### The ability kind

![Isaac casts Ragnarok at Ivan: a blazing sword plunges down on him and bursts into light, and `Ivan takes 127 damage!`.](https://alchemy.pascalpixel.com/abilities/ragnarok.gif)

The ability kind decides what the power means. Ragnarok is an attack kind:
its power is added to a plain blow, so it hits harder the stronger Isaac's
Attack is, where Quake's damage follows his Power in Venus.

The formulas are in [`../game/game/battle_calc.c`](../game/game/battle_calc.c),
in the `game` [bank](../../glossary.md#bank), called by their
[far-call](../../glossary.md#far-call) names: `Battle_CalculateRecoveryFar`
leads to `Battle_CalculateRecovery`.

| Ability kind | What the target takes | What the power is |
| --- | --- | --- |
| `ABILITY_FORMULA_HEAL_HP` (Cure, Ply, Herb) | HP back, by the healer's Power in the element | the HP restored at Power 100 |
| `ABILITY_FORMULA_EFFECT` (Sleep, Impair, Drain) | damage only when the added effect lands | the damage at an even match |
| `ABILITY_FORMULA_ATTACK` (Attack, Ragnarok) | a blow, the actor's Attack against the target's Defense | damage added to a plain blow |
| `ABILITY_FORMULA_ATTACK_MULTIPLIED` (Flint, Quick Strike) | a plain blow times the power | the multiplier, in tenths |
| `ABILITY_FORMULA_DAMAGE` (Quake, Avalanche) | damage scaled by the actor's Power against the target's Resist | the damage at an even match |
| `ABILITY_FORMULA_ENEMY_DAMAGE` (Fire Blessing, War Cry) | as `ABILITY_FORMULA_DAMAGE`, falling off faster | the damage at an even match |
| `ABILITY_FORMULA_SUMMON` (Ramses, Judgment) | as `ABILITY_FORMULA_DAMAGE`, with a share of the target's maximum HP added | the damage at an even match, before the share |
| `ABILITY_FORMULA_PP_DAMAGE` (Crazy Voice) | PP lost, as `ABILITY_FORMULA_DAMAGE` | the PP lost at an even match |
| `ABILITY_FORMULA_HEAL_PP` (Psy Crystal, Adept Ring) | PP back, as `ABILITY_FORMULA_HEAL_HP` | the PP restored at Power 100 |

An even match is the actor's Power in the element equalling the target's
Resist in it. `ABILITY_FORMULA_BLANK`, `ABILITY_FORMULA_EFFECT_ONLY` (Granite,
Flash, Kite) and `ABILITY_FORMULA_NO_POWER` (the field Psynergy, Defend, Flee)
apply no power, and every other kind but the two attacks skips its formula
when the power is 0. HP healing, attacks and HP damage add a random 0 to 3. A
summon called with `COMMAND_SUMMON` adds 3 percent of the target's maximum HP
for each Djinni it uses. A blow always does at least 1 damage; a critical hit
adds a quarter, then the target's level divided by 5, plus 6.

| Name | Value | Meaning |
| --- | --- | --- |
| `FULL_RATE` | 256 | 1 in the multipliers, which are counted in 256ths |
| `ABILITY_FORMULA_MASK` | 15 | the bits of an ability's `flags` that are its kind |

### The element multiplier

![Mia casts Froth Sphere at Isaac, Garet and Ivan: bubbles stream across and burst over them, and the messages read `Isaac takes 69 damage!`, `Garet takes 85 damage!` and `Ivan takes 68 damage!!!`.](https://alchemy.pascalpixel.com/abilities/froth-sphere.gif)

Each unit has a Power and a Resist in each of the four elements, and the gap
between the attacker's Power and the target's Resist decides how much the
element counts. The message hints at it: three exclamation marks mean the
element is the one the target resists worst, a full stop the one it resists
best.

The element bonus is the actor's Power in the ability's element less the
target's Resist in it. `Battle_GetElementMultiplier` reads it off the five
points of `gElement_PowerResistCurve` (`../game/element_table.c`), which lie on
one line: 1 at an even match, moving half a percent for each point, from 0 at
200 points behind to 2 at 200 ahead. A blow feels the element at half
strength. Healing uses the healer's Power alone: each point above or below 100
moves it one percent.

A unit's Power and Resist come from its element levels
(`gElement_PowerResistByLevel`) or an enemy's element preset, with the
equipment's added by `Unit_Calculate`. A Resistance stage adds 20 Resist in
every element (`RESISTANCE_PER_STAGE`). A summon's boost raises the element it
spent most Djinn of by 10, 30, 60 or 100 Power for one to four Djinn.

The message compares the element with the target's base Resist (`baseElement`):
`damage.` for the element it resists best, `damage!!!` for the one it resists
worst, `damage!` otherwise. A Djinni striking the element the target resists
worst may also earn the Djinn bonus: an enemy it fells gives more coins and
experience (`BattleEnemy_RecordDefeat`).

| Name | Value | Meaning |
| --- | --- | --- |
| `RESISTANCE_PER_STAGE` | 20 | Resist added in every element by one Resistance stage |

### Distance

![Isaac casts Avalanche at Ivan, Garet and Mia: the ground swells under them, a wall of rock rises with rocks tumbling up it, and Ivan takes 203 damage, Garet 160 and Mia 120.](https://alchemy.pascalpixel.com/abilities/avalanche.gif)

An ability that reaches several enemies hits the one aimed at hardest and the
ones beside it less. Isaac aims at Ivan, with no one on Ivan's other side, so
Garet is one place away and Mia two.

Every kind but the attacks falls off with the target's distance from the aimed
place, by the tables in `battle_calc.s`. The figures are percentages:

| Places from the aimed one | 0 | 1 | 2 | 3 | 4 | 5 |
| --- | --- | --- | --- | --- | --- | --- |
| `ABILITY_FORMULA_DAMAGE`, `ABILITY_FORMULA_HEAL_HP`, `ABILITY_FORMULA_HEAL_PP` | 100 | 80 | 60 | 40 | 20 | 10 |
| `ABILITY_FORMULA_SUMMON` | 100 | 70 | 40 | 30 | 20 | 10 |
| `ABILITY_FORMULA_ENEMY_DAMAGE`, `ABILITY_FORMULA_EFFECT`, `ABILITY_FORMULA_PP_DAMAGE` | 100 | 50 | 30 | 10 | 10 | 10 |
| an added effect's chance (`gRangeFalloff_EffectChance`) | 100 | 60 | 30 | 30 | 30 | 30 |

Avalanche is `ABILITY_FORMULA_DAMAGE` with a power of 160 and a range of 3. At
an even match the aimed foe takes 160 to 163, one place away 128 to 131 and two
places away 96 to 99; with the actor's Power 20 ahead, 175 to 178, 140 to 143
and 105 to 108, and a defending aimed foe 87 to 89. Blows work differently:
with Attack 100 against Defense 60, a plain Attack deals half the difference,
20 to 23; Ragnarok (power 35) adds twice its power before halving, 55 to 58;
Flint (power 16) multiplies the blow by 1.6, 32 to 35. Cure (power 70) restores
70 to 73 HP at Power 100 and 83 to 86 at 120; the Healing Ring, which uses
Cure's row, always restores 70 to 73, since an item's user counts as Power 100.

### Whether the added effect lands

![Ivan casts Confuse at Isaac, Garet and Mia: a pale blue mist rolls over the three, and the messages read `Isaac is confused!` and `But it has no effect on Garet!`](https://alchemy.pascalpixel.com/abilities/confuse.gif)

When Ivan casts Confuse at a group, some fall to it and the player reads
`But it has no effect on <unit>!` for the others. Each target gets its own
roll, and the chance depends on the effect, the two units' element levels, the
target's Luck and weaknesses, and its distance from the aimed place.

`Battle_RollEffectSuccess`, through `Battle_RollEffectSuccessFar`, fails at
once when there is nothing to do (a revival on a unit still standing, a cure
on a unit without the status). Otherwise the chance starts from the effect's
base rate (`Battle_GetEffectBaseSuccessRate`: Sleep 45 percent, Taint 55,
Condemn 20 and so on), gains 3 for each element level the actor is ahead,
loses 3 for every two points of the target's Luck, gains 25 when the target is
weak to the effect, and is scaled by the distance table. Some effects have a
fixed chance instead, stored as a negative base rate: Quartz 60 percent, Dew
90, and 100 for an effect with no rate of its own. When the added effect of an
`ABILITY_FORMULA_EFFECT` ability fails, the target takes no damage either.

### Calling an ally, and effects that change the damage step

![A Fighter Bee uses Ally Search: `Fighter Bee looks for allies!`, and a second Fighter Bee flies in beside it, `Fighter Bee2 appears!!`.](https://alchemy.pascalpixel.com/guides/battle/ally-search.gif)

Some added effects stand in for the damage or change how it is dealt, so they
are applied before it. The most visible brings in a new enemy.

Phase 3 handles Ally Search, Search for Help and Speed Surge. When the roll
succeeds, a free unit id exists and the enemy [sprite](../../glossary.md#sprite-and-oam)
slots have room (`Enemy_HasSpriteRoom`), the new enemy fills an empty place or
is appended, the enemies are laid out again, and the player reads
`<unit> appears!!` or `It split off into <unit>!`; otherwise `But no one came!`
or `It failed to divide!`.

Phase 4 runs only for an effect that landed: Torch's blow counts half the
target's Defense; Severe Blow and Thrash leave the target 1 HP at most;
Condemn and the other instant deaths skip the damage; Dynamite and Mortal Blow
remove their user; Psy Drain becomes `ABILITY_FORMULA_PP_DAMAGE`; and Ground
holds a target only while it still has an entry in the round's action list,
which `BattleStaging_DispatchAction` clears as each action plays, so a target
that has already acted is left alone.

### Applying the added effect

![Garet casts Impair at Mia: a shield appears over her and shatters, and `Mia's Defense drops by 54!`.](https://alchemy.pascalpixel.com/abilities/impair.gif)

Statuses and stat changes are counters on the unit that the end of each round
counts down. Impair lowers Mia's Defense for seven turns, and the number in the
message is how far it fell.

Phase 6 applies the effect when it landed and the target is still standing; a
revival needs the target down instead, and a drain still works on a target its
own damage has just felled (`Ability_EffectAppliesToDowned`). The case comments
in `BattleAbility_ApplyToTarget` and the notes on `enum AbilityEffect`
([`battle_unit.h`](../../include/battle/battle_unit.h)) give each effect's
detail; the rules they share are these.

- A stat change moves Attack, Defense or Resistance one or two stages, between
  `STAT_STAGE_MIN` and `STAT_STAGE_MAX`, for `STAT_CHANGE_TURNS`. A stage is
  an eighth of Attack or Defense, and the message gives how far the stat
  moved, so a stat at its limit shows 0. Zephyr and Vine set the Agility stage
  outright, to double or half, for `AGILITY_CHANGE_TURNS`.
- A status (Delude, Confuse, Charm, Paralyze, Sleep) and Bind's seal last
  `STATUS_TURNS`. Luff's seal lasts two turns that never end early. Curse
  starts the Grim Reaper's countdown of `CURSE_TURNS`.
- A lowered stat or a status may end early: once five turns or fewer are left,
  each round's end rolls for it, more often for a lucky unit
  (`BattleUnit_RollCounterEarlyEnd`), and the player reads, for a delusion,
  `<unit> sees clearly once again!`. A raised stat always lasts its turns.
- Restore and Elixir clear delusion, stun, sleep, the seal and the Grim
  Reaper's countdown; Tonic clears those, poison and the evil spirit. Neither
  clears confusion or charm.
- Drain gives the actor all the HP the target lost, Sap half, Psy Drain the PP
  it took, and Psy Leech a tenth of the damage as PP.
- Defend and Granite set `GUARD_DEFEND`, Total Defense and Flash
  `GUARD_TOTAL_DEFENSE`. Break returns every raised stage to 0 and removes a
  summon's boost. Kite lets the target act twice next turn, Ground costs it
  its next action, and Haunt sets an evil spirit on it.

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

![Isaac casts Quake at Garet, Ivan and Mia while Ivan is asleep: Quake's blow wakes him, `Ivan wakes from slumber!`.](https://alchemy.pascalpixel.com/guides/battle/wake.gif)

A sleeping unit that is hit may wake up, and a unit haunted by an evil spirit
pays for its own attacks.

Losing HP or PP wakes a sleeping target one time in four, unless it fell
asleep this round. A haunted actor, one time in four for each target it took
HP or PP from, adds a quarter of that loss (at least 1) to
`BattleTurn.hauntDamage`, and takes the total from its own HP at the end of
its turn (`BattleUnit_ProcessTurnEnd`). In the Djinn tutorial a Djinni's blow
always leaves its target 1 HP, and a summon always fells it.

## Rules and pictures meet at the playback queue

![Ivan casts Plasma at Garet, Isaac and Mia: lightning bursts over them, and the damage messages follow one target at a time, `Garet takes 46 damage!`, `Isaac takes 56 damage!`, `Mia takes 44 damage!!!`.](https://alchemy.pascalpixel.com/abilities/plasma.gif)

Each target's HP drops at once, but the player sees it happen in steps: the
target flinches, its message appears and waits, and only then does the next
target's turn come. The rules leave a list of what to show, in order, and the
screen plays the list.

The playback queue (`struct BattlePlaybackQueue`) is the one channel from the
rules to the screen. `BattleAbility_ApplyToTarget` never draws or waits, and an
animation never computes an outcome. [Battle](../../battle.md#rules-and-animations-run-together)
shows how `BattlePlayback_Run` runs as a [task](../../glossary.md#task) while
the animation's own loop waits: playback runs each target's rules once the
animation calls `ANIM_SHOW_DAMAGE`, and for the few animations that never call
it (`atalanta.c`, `djinn_heal.c` and `torch.c`) `BattlePlayback_WaitForReady`
starts playback once they return.

Playback resolves the targets in the turn's order, the battlefield's, not the
aimed one first. Only the hit (nine [frames](../../glossary.md#frame) of
flashing) and the message (until the player presses A) make it wait; the
names and numbers pushed before a message, which fills its blanks from them as
it is written, play within one frame. A message uses its arguments up, so each
later message pushes the name again. `BattlePlayback_RunAll` plays the same
queue straight through for an outcome shown outside an animation, such as a
cast that is sealed or short of PP.

| Name | Value | Meaning |
| --- | --- | --- |
| `BATTLE_PLAYBACK_CAPACITY` | 64 | events the queue holds |

## Design

### Results nobody reads

Some of the original's functions promise to return a value and end without
one, and no caller reads it. The source keeps the promised type where writing
`void` would change the compiled code at the function's end, and
[`UBFIX`](../../glossary.md#bug-bugfix-and-ubfix) gives the `void` form;
[Matching](../MATCHING.md) explains the rule. `BattleAbility_ApplyToTarget`,
`BattlePlayback_RunAll` and `BattlePlayback_WaitForReady` are among them.

### One sprite, one struct

A unit's figure is one of the object slots `Object_CreateFar` hands out, a
`struct Object`, which the battle and the field's
[far calls](../../glossary.md#far-call) both take. Only
`BattleObject_SpawnObjectsForList` reads it through a copy of the layout,
`BattleSpriteObject`, whose sprite word is untyped, since only that form
compiles to the original's instructions ([Matching](../MATCHING.md)).

The ROM holds the battle as one object, and the build compiles it as one; the
parts are our cut of it, made where one family of functions ends and the next
begins. A function that acts on one thing the engine names elsewhere keeps
that thing's prefix (`Transform_`, `Palette_`, `Party_`, `Ability_`, `Text_`
and others) rather than taking `Battle_`.

## Limits

- There are 519 abilities. The turn reads `gAbilityAnimations` only up to
  `ABILITY_THRASH`, and `gAbilityStaging`, a row shorter, up to
  `ABILITY_SEVERE_BLOW`.
- A turn reaches at most `TARGET_MAX` targets.
- The party fights `PARTY_BATTLE_SIZE` at a time and the enemies
  `ENEMY_BATTLE_SIZE` at most; in a link battle each console sends
  `LINK_PARTY_SIZE`.
- `formations.c` has 380 formations, each of up to five kinds of enemy with
  the least and most of each (`member0` to `member4`, `minimum0` to
  `minimum4`, `maximum0` to `maximum4`). The encounter zones in
  `../overworld/encounter_zones.c` choose among them.
- The playback queue holds `BATTLE_PLAYBACK_CAPACITY` events.

## Words used here

- **unit**, **unit id**: everyone in a fight. Party members keep their
  character numbers as unit ids, up to `ALLY_MAX`, and enemies take ids from
  `FIRST_ENEMY_UNIT`. `Unit_GetFar` returns a unit's `struct Unit`: its HP,
  PP, stats, statuses, stat stages and a Power and a Resist in each element
  (`Unit.element`).
- **ability**, **ability id**: anything a unit does on its turn, from Attack
  to Quake, a Djinni, a summon, an item's use or an enemy's move. The id is a
  row number in three tables: `../game/abilities.c` (`struct AbilityDefinition`,
  read with `Ability_Get`), `gAbilityAnimations` and `gAbilityStaging`.
- **ability kind**: the low four bits of the ability's `flags`
  (`ABILITY_FORMULA_MASK`, `enum AbilityFormula`). It chooses the formula each
  target goes through. The **animation id** in `gAbilityAnimations` is
  unrelated and chooses only the animation (`enum BattleAnimId`).
- **power**: the `power` field in `abilities.c`. What it counts depends on
  the ability kind. A unit's **Power** and **Resist**, capitalized as the
  summary screen writes them, are its per-element stats.
- **added effect**: what an ability does besides damage or healing, the
  `effect` field in `abilities.c` (`enum AbilityEffect`).
- **element level**: the level the summary screen shows under each element
  (`Unit_GetElementLevel`). A character's Power and Resist start from it, and
  it moves the chance of an added effect.
- **place**, **place code**: a unit's position in its side's line-up
  (`BattleWork.partyUnits`, `BattleWork.enemyIds`). A command aims at a place
  code, the place plus `PLACE_PARTY` or `PLACE_ENEMY`.
- **side**: the party or the enemies (`BATTLE_SIDE_MASK_PARTY`,
  `BATTLE_SIDE_MASK_ENEMIES`). A unit's own `side` field says which side it is
  on, or that it has left the battle.
- **turn**: the action under way (`struct BattleTurn`): the actor, the
  ability, its element, its targets with their distances and rolls, its
  **outcome** (`enum BattleStaging`, how it is staged) and its
  **animation word** (the `gAbilityAnimations` row: `STAGING_*`).
- **battle object**: a unit's figure on the battlefield
  (`struct BattleObject`, `BattleObject_Get`). It has one sprite; the Kraken
  and the Fusion Dragon are drawn in two parts and have two
  (`SPRITE_WITH_SECOND_PART_A`, `SPRITE_WITH_SECOND_PART_B`).
- **event**: one thing the battle shows while an action plays, waiting in the
  playback queue (`BattlePlayback_Push`, `enum BattlePlaybackCommand` in
  `battle_event.h`): a message, a name or number the next message needs, a
  flinch, a defeat, a redraw or a sound.
- **pose**: how a battle object stands (`POSE_*`, set with
  `Object_SelectAnimationFar`).

The parts give the same locals the same names: `battle` or `work` for
`gBattleWork`, `unitId`, `unit` for its `struct Unit`, `battleObject` for its
`struct BattleObject`, `object` for that object's `struct Object`, `record`
for one of its sprites, and `plan`.

## Evidence and open questions

Abilities and enemies are named from the label columns of their tables,
`ability` and `enemy`, which follow the English release's text. The formulas
are named from what `battle_calc.c` computes, and the event kinds from what
playback does with each.

The chances and counts on this page were checked in a tbs-en build with
`alchemy-devtools render`, answering `Battle_Random` with chosen numbers.
Replaying a Gryphon's turn with each of the 256 rolls gave its actions in the
spans above. One encounter (the Lizard King, a Harridan and two Stone
Soldiers) opened both as a first strike, where only the four party members
acted, and as a surprise, where three of the enemies did. Isaac's attacks on a
copy of Ivan gave a miss (`Ivan nimbly dodges the blow!`), a critical hit of
110 and, with the Swift Sword unleashing Sonic Smash, a triple blow of 281.
Real calls of `Battle_GetElementMultiplier` fell on that line: for Quake, Ivan
at 52 points ahead and 1.26 times, Mia at 51 and 1.25, Isaac's copy at 30
behind and 0.85, and for an attack on Ivan, 8 behind and 0.98 on the blow's
line. Against a Vermin, its delusion ended early with 5 turns left while both
Defense changes ran their 7 turns. Aiming Quake along four large enemies moved
the cursor from the last place round to the first. The party at level 20 ran
from two Gryphons on its second try, the first forced to fail.

Bits 0 and 1 of the `flags` column of `gAbilityAnimations` (`ANIM_ON_DOWNED`,
`ANIM_OTHER_SIDE`) are read, but no row in either game sets them. The turn
adds the first itself for the revive effects; nothing adds the second, so the
side flip it asks for is unused. `BattlePlacement_LayoutEnemies` lays out the
enemies' column, and `BattleSprite_HasHeavyImpact` reads the class bit that
makes a beam blow land with `SE_HEAVY_IMPACT`, which only the Mars Lighthouse
bosses set (Saturos's second fight, Menardi and the Fusion Dragon).

The battle camera has a second point it can follow besides `followPosition`:
`followEye`, which `ObjectSystem_ProjectSprites` would use in place of the eye
when it turns the sprites to face the camera. Every camera setup (the
battle's, the world map's and Lucky Dice's) clears it and nothing sets it, so
the sprites always face the real eye. The last two words of
`BattleBackgroundWork`, `unusedAfterFlag`, are cleared when a battle starts;
no code in the battle, its animations or the field reads them, since every
reader of the slot goes through the struct's named fields.
