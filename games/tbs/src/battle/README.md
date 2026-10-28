# Battles

Everything the rules of a fight decide is worked out here: who acts in what
order, which enemy does what, who an ability reaches, whether a blow misses or
lands a critical hit, how much damage each target takes, and which statuses
take hold. The answers come from tables and dice rolls, and each shows up on
screen as a message such as `Ivan takes 15 damage!`.

[Battle](../../battle.md) explains a battle as a whole and is the place to
start. [Battle effects](../battle_effect/README.md) draws them,
[Windows](../window/README.md) holds the command menu and the
[party](../../glossary.md#the-party)'s status window, and
[Game rules](../game/README.md) holds the ability, item and enemy tables and
the arithmetic the rules call.

## Turns

![Isaac, Garet, Ivan and Mia against the Lizard King, a Harridan and two Stone Soldiers: Ivan, Isaac and Mia attack the Harridan until it falls; Garet, whose target is gone, defends; then one Stone Soldier casts Blast, the other tries Mad Blast without enough PP, and the Lizard King casts High Impact.](https://alchemy.pascalpixel.com/guides/battle/turn-order.gif)

Each turn, every unit (everyone in the fight, party member or enemy)
chooses an action, and the actions play fastest first. Here the party,
at level 35, is faster than all four enemies, so it acts first; Garet,
slowest of the four, finds the Harridan already felled and defends instead.

Who acts first is decided by Agility, highest first, with a few twists:

- The Djinn that guard or hold an enemy, Granite, Flash and Ground, go before
  everyone ([Djinni](../../glossary.md#djinn)).
- A party member's Agility gains up to a sixteenth more at random each turn,
  so two party members of nearly equal speed may swap places; an enemy's counts as
  it is.
- On a tie the party goes first, and enemies of equal Agility go in a
  shuffled order.
- A unit that acts twice in a turn has its second action at half its
  Agility.
- Defend and Total Defense are raised as soon as the actions are sorted, so a
  defending unit is guarded against everything that turn.

Before the first turn the formation becomes a set of enemies, each with its
level and, when two of a kind meet, a letter to tell them apart, and the
opening plays. Then each turn goes the same way: the actions are gathered
and sorted; each action is announced, staged and animated, while the rules
apply it to each target; the unit's action ends, which is when poison bites
(a tenth of the maximum HP, deadly poison a fifth), the Grim Reaper takes a
unit whose countdown has run out, and a
[summon](../../glossary.md#summons)'s boost takes hold; and at the turn's
end stat changes and statuses count down and the Djinn recover. When one side
has nobody left, the spoils (coins, experience, items and level ups) are
handed out.

`Battle_Run` ([`battle/encounter.c`](battle/encounter.c)) runs the whole
battle. The turn's steps are `Battle_GatherActions` and
`BattleActionOrder_Sort` (`turn.c`), then for each action `BattleActionRun_Build`
(`action.c`) and `BattleStaging_DispatchAction` (`staging.c`), and last
`Battle_ProcessTurnEnd` (`turn_end.c`); `spoils.c` closes the battle.

### Enemy actions

![While the party defends, the first Gryphon uses Wing Flutter, a volley of light across the party, and the second casts Tornado.](https://alchemy.pascalpixel.com/guides/battle/gryphons.gif)

Every enemy has a list of eight actions and a habit for choosing among them,
and the dice do the rest. A Gryphon's list runs Wing Flutter, Tornado, Twin
Beaks, Impact, Attack, Sonic Slash, Bind and Ward, and it favours the start of
the list, so it uses Wing Flutter most and Ward least: each pick rolls a
number from 0 to 255, and Wing Flutter takes rolls 0 to 52, Tornado 53 to 99,
Twin Beaks 100 to 140, and so on down to Ward's 245 to 255.

An enemy chooses twice each turn. While the turn is put together it keeps
its pick only if it is a guard, so that its guard goes up with the party's;
when it acts it picks again. A pick it cannot use (too little
[PP](../../glossary.md#psynergy-and-pp), a seal, no target worth it) is
drawn again, and after 18 failed picks the enemy defends.

The habits are these:

| Habit | How the entry is picked |
| --- | --- |
| even | at random, each entry as likely |
| falling | at random, from 53 in 256 for the first down to 11 for the last |
| first favoured | at random, the first 113 in 256, the last 5 |
| random start | a random entry the first time, then each in turn |
| in order | each in turn, from where it left off |
| next pick | the next entry at each new pick, from the first |
| same | the entry it picked last time |

`BattleAI_ChooseAction` ([`battle/enemy_ai.c`](battle/enemy_ai.c)) makes the
choice from the enemy's row of `../game/enemies.c`; the habits are
`enum EnemyActionStrategy` and the random ones read `gBattleAI_ActionWeights`.

### First strikes and fleeing

![A random battle with two Gryphons opens with `Isaac's party attacks first!`, and Isaac and Garet attack while the Gryphons stand still.](https://alchemy.pascalpixel.com/guides/battle/first-strike.gif)

Now and then a random battle opens with `Isaac's party attacks first!` and the
enemies stand still for a turn; more rarely it opens with
`Isaac's party was caught by surprise!`, and some enemies act before the party
can choose anything.

Only random battles roll for it: the party strikes first one time in 16, and
otherwise the enemies one time in 32. The Djinn tutorial always gives the
party the first strike. When the enemies strike first, the party chooses no
commands and two to five enemies, drawn at random, act once each. Everything
is back to normal at the turn's end.

![The party, at level 20, chooses Flee against two Gryphons: `Isaac and friends run!` is followed by `But there's no escape!`, and the Gryphons use Wing Flutter and Twin Beaks. The next turn the party tries again and gets away.](https://alchemy.pascalpixel.com/guides/battle/flee.gif)

Running away is a chance that grows with every failed try: 50 percent, plus
20 for each failed try in the battle, plus 5 for each level the party's
average is above the enemies' (or less, below). The party at level 20 against
Gryphons at level 23 starts at 35 percent. A first strike makes running
certain, and some battles, such as a Mimic's, cannot be fled at all. A failed
try makes the rest of the turn one the enemies began.

`BattleWork.firstStrike` records which side has the jump, and
`BattleFlee_CheckSuccess` (`turn.c`) rolls the escape. What kind of battle it
is, `gAdventure.battleType`, decides the rest:

| Kind | What it changes |
| --- | --- |
| `BATTLE_TYPE_NORMAL` | a random encounter: the first-strike rolls are made |
| `BATTLE_TYPE_NO_ESCAPE` | the party cannot run, as from a Mimic |
| `BATTLE_TYPE_SCRIPTED` | a battle an [event](../../glossary.md#event) starts; the command menu refuses to run |
| `BATTLE_TYPE_LINK` | a link battle |
| `BATTLE_TYPE_DEBUG` | the debug battle menu's battle; the party may run |

## Building an action

![Isaac casts Spire at Ivan: the announcement `Isaac casts Spire!` comes first, then a spire of rock falls on Ivan and shatters, and `Ivan takes 52 damage!`.](https://alchemy.pascalpixel.com/abilities/spire.gif)

Just before a unit acts, the player reads its announcement, such as
`Isaac casts Spire!`. By then the game has built the chosen command into an
action run: which ability it is, who it reaches, and the rolls the battle effect must
show. An action that fails at this point (an item already used up, no one
left in range) shows its own message instead and the action ends there.

`BattleActionRun_Build` ([`battle/action.c`](battle/action.c)) builds the action's run, a
`struct BattleActionRun`, and shows the announcement.

### Commands and ability ids

![Isaac attacks Ivan with the Swift Sword: `Isaac's Swift Sword lets out a howl! Sonic Smash!`, and the sword strikes three times for 281 damage.](https://alchemy.pascalpixel.com/guides/battle/unleash.gif)

The command the player chose decides which ability the action carries. An
Attack is usually just Attack, but a weapon can unleash its own ability in its
place, as the Swift Sword unleashes Sonic Smash.

- Attack is Attack, unless the weapon unleashes, at five times its unleash
  chance in percent plus what the rest of the equipment adds; the player then
  reads `<unit>'s <weapon> lets out a howl!` and the ability's name.
- Psynergy is the ability chosen in the menu. Its PP are paid here, only when
  the cast goes ahead.
- Djinn reads the Djinni's ability from `../game/djinn_table.c`. Only a set
  Djinni is unleashed, and it then goes on standby; one on standby is set
  again instead.
- Item takes the item's ability; an item with none, or a broken one, makes
  the user defend.
- Summon takes the summon's ability from `../game/summons.c` and marks the
  standby Djinn it uses.

The action takes its [element](../../glossary.md#the-four-elements) from the
ability, except that a plain Attack strikes with the weapon's element, or an
enemy's with its own. The commands are `enum BattleCommand`.

### Targets and range

![Isaac aims Quake: the cursor moves along the Harridan, the Lizard King and the two Stone Soldiers, the enemies beside the aimed one lit too, and from the last it wraps round to the first.](https://alchemy.pascalpixel.com/guides/battle/aim.gif)

Each side stands in a line, and a unit's position in it is its place. An
ability reaches the places within its range of the aimed one, counted in
places rather than in distance on the screen; the player sees the reach as the
lit enemies around the cursor.

A range of 1 is the aimed unit alone, and each step more adds a place on
either side; the widest range covers the whole side, with every target
treated as the aimed one. The reach passes over a unit that has left the
battle, and a downed unit unless the ability revives, but a downed unit still
holds its place. If no one in range is left, the unit defends
(`<unit> is defending!`). The reach stops at the ends of the line, though the
cursor wraps round. Where the units stand on screen changes none of this.

`BattleActionRun_SetTargets` walks the places; the cursor is
`BattleTarget_RunSelection` (`../window/screens/targets.c`).

| Name | Value | Meaning |
| --- | --- | --- |
| `ABILITY_RANGE_ALL` | 255 | the whole side, every target treated as the aimed one |
| `TARGET_MAX` | 14 | targets an action holds at most |

### Rolls made in advance

![Isaac attacks Ivan twice: the first time `Ivan nimbly dodges the blow!`, the second a critical hit bursts in light and `Ivan takes 110 damage!`.](https://alchemy.pascalpixel.com/guides/battle/rolls.gif)

A dodge and a critical hit look different from a plain blow, so the battle effect
has to know which it is before it starts. The action therefore rolls them while
it is built, before anything is drawn.

A plain Attack never misses a target that cannot act (down, asleep, stunned,
charmed or held by Ground). Otherwise it misses one time in 32, and a deluded
attacker also misses 153 times in 256. A critical hit, a quarter more damage,
lands one time in 32, or else at half the percentage the attacker's equipment
adds to its unleash chance.

An ability with a double or triple blow makes one roll for all its targets:
153 in 256 for Gust, Rapid Smash, Slice and Sonic Smash, one time in four for
Flying Attack, Double Fang, Twin Beaks and Swift Strike; Sonic Smash and Swift
Strike strike three times. A success raises each target's blows and the
battle effect's strength, so the extra blows show. Annihilation rolls its instant
death here too; every other added effect is rolled for each target as it is
hit.

### Staging and announcement

![Isaac casts Rockslide at Garet, Ivan and Mia: a column of light opens into a wall of fire with rocks tumbling through it.](https://alchemy.pascalpixel.com/abilities/rockslide.gif)

The staging is what happens around a battle effect: whether the unit runs up and
strikes, raises a hand to cast, or nothing moves. Rockslide is staged full
screen: the camera turns and the side that isn't targeted fades out.

Most abilities are staged by kind: anything that costs PP is a cast, and an
ability with nothing to draw shows only its messages. A few have their own
staging in `gAbilityStaging`: Gaia, Mother Gaia, Grand Gaia, Rockfall,
Rockslide, Avalanche and Procne are full screen, and Slice and seven weapon
unleashes are staged as an enemy's cast, with the element's streaks in place
of the star field.

The announcement follows the command: `<unit> attacks!`,
`<unit> casts <ability>!`, `<unit> unleashes <ability>!` for a Djinni,
`<unit> summons <ability>!`, `<unit> uses <item>!`, or
`<unit> raises the <item>!` for an item the user keeps. An enemy's ability is
`<unit> uses <ability>!`, or `<unit> unleashes <ability>!` for its special
attacks.

The stagings are `enum BattleStaging`:

| Name | What the player sees |
| --- | --- |
| `STAGING_MESSAGES` | no battle effect, only each target's messages |
| `STAGING_ATTACK` | the acting unit runs up and strikes |
| `STAGING_CAST` | a cast: Psynergy, and Venus, Mercury, Mars and Jupiter |
| `STAGING_ITEM` | the user raises an item |
| `STAGING_SUMMON` | the larger summons, the acting unit hidden |
| `STAGING_DJINNI` | a Djinni with no attack |
| `STAGING_FLEE` | a unit runs |
| `STAGING_ENEMY_CAST` | the element's streaks in place of the star field |
| `STAGING_FULL_SCREEN` | the camera turns and the side not targeted fades out |

## How an ability resolves

![Mia casts Ply on Garet: motes of light swirl in to him and `Garet recovers 150 HP!`.](https://alchemy.pascalpixel.com/abilities/ply.gif)

Once the battle effect shows the blow land, or the healing reach its target, each
target in turn learns what the ability does to it: how much damage it takes or
how much it heals, and whether the ability's added effect (Sleep, a lowered
Defense, poison) takes hold.

For one target, the game first notes how far it stands from the aimed place
and how strong the user is in the ability's element; rolls whether the added
effect lands; brings in a new enemy if the ability calls one; deals the damage
or healing; applies the added effect; and last rolls whether the hit woke the
target. Nothing is drawn and nothing waits while this happens: the target's
numbers change at once, and what the player should see is put in a queue to
be shown afterwards ([below](#rules-and-pictures-meet-at-the-playback-queue)).

`BattleAbility_ApplyToTarget`
([`battle/target_exec.c`](battle/target_exec.c)) does this, its steps marked
in its body.

### The ability kind

![Isaac casts Ragnarok at Ivan: a blazing sword plunges down on him and bursts into light, and `Ivan takes 127 damage!`.](https://alchemy.pascalpixel.com/abilities/ragnarok.gif)

The ability kind decides what the power means. Ragnarok is an attack kind:
its power is added to a plain blow, so it hits harder the stronger Isaac's
Attack is, where Quake's damage follows his Power in Venus.

| Ability kind | What the target takes | What the power is |
| --- | --- | --- |
| healing (Cure, Ply, Herb) | HP back, by the healer's Power in the element | the HP restored at Power 100 |
| effect (Sleep, Impair, Drain) | damage only when the added effect lands | the damage at an even match |
| attack (Attack, Ragnarok) | a blow, the user's Attack against the target's Defense | damage added to a plain blow |
| multiplied attack (Flint, Quick Strike) | a plain blow times the power | the multiplier, in tenths |
| damage (Quake, Avalanche) | damage scaled by the user's Power against the target's Resist | the damage at an even match |
| enemy damage (Fire Blessing, War Cry) | as damage, falling off faster | the damage at an even match |
| summon (Ramses, Judgment) | as damage, with a share of the target's maximum HP added | the damage at an even match, before the share |
| PP damage (Crazy Voice) | PP lost, as damage | the PP lost at an even match |
| PP healing (Psy Crystal, Adept Ring) | PP back, as healing | the PP restored at Power 100 |

An even match is the user's [Power](../../glossary.md#power-and-resist) in
the element equalling the target's Resist in it. Granite, Flash, Kite, the
field Psynergy, Defend and Flee apply no power at all, and every other kind
but the two attacks does nothing when its power is 0. Healing, attacks and
damage add a random 0 to 3. A summon adds 3 percent of the target's maximum HP
for each Djinni it uses. A blow always does at least 1 damage; a critical hit
adds a quarter, then the target's level divided by 5, plus 6.

The kinds are `enum AbilityFormula`, the low four bits of an ability's
`flags`. The formulas are in [`../game/game/battle_calc.c`](../game/game/battle_calc.c),
called from here through their far-call names, such as
`Battle_CalculateRecoveryFar` for `Battle_CalculateRecovery`
([far call](../../glossary.md#far-call)).

### The element multiplier

![Mia casts Froth Sphere at Isaac, Garet and Ivan: bubbles stream across and burst over them, and the messages read `Isaac takes 69 damage!`, `Garet takes 85 damage!` and `Ivan takes 68 damage!!!`.](https://alchemy.pascalpixel.com/abilities/froth-sphere.gif)

Each unit has a Power and a Resist in each of the four elements, and the gap
between the attacker's Power and the target's Resist decides how much the
element counts. The message hints at it: three exclamation marks mean the
element is the one the target resists worst, a full stop the one it resists
best.

Each point of Power ahead of the Resist adds half a percent, up to double at
200 points ahead, and each point behind takes half a percent off, down to
nothing at 200 behind. A blow feels the element at half strength. Healing
uses the healer's Power alone: each point above or below 100 moves it one
percent.

A character's Power and Resist come from the element levels the
Status screen shows, an enemy's from its element preset, and equipment adds to both.
A Resistance stage adds 20 Resist in every element. A summon's boost raises
the element it spent most Djinn of by 10, 30, 60 or 100 Power for one to four
Djinn. A Djinni striking the element the target resists worst may also earn
the Djinn bonus: an enemy it fells gives more coins and experience.

`Battle_GetElementMultiplier` reads the multiplier off
`gElement_PowerResistCurve` (`../game/element_table.c`), in 256ths.

### Distance

![Isaac casts Avalanche at Ivan, Garet and Mia: the ground swells under them, a wall of rock rises with rocks tumbling up it, and Ivan takes 203 damage, Garet 160 and Mia 120.](https://alchemy.pascalpixel.com/abilities/avalanche.gif)

An ability that reaches several enemies hits the one aimed at hardest and the
ones beside it less. Isaac aims at Ivan, with no one on Ivan's other side, so
Garet is one place away and Mia two.

Every kind but the attacks falls off with the target's distance from the aimed
place. The figures are percentages:

| Places from the aimed one | 0 | 1 | 2 | 3 | 4 | 5 |
| --- | --- | --- | --- | --- | --- | --- |
| damage and healing | 100 | 80 | 60 | 40 | 20 | 10 |
| summons | 100 | 70 | 40 | 30 | 20 | 10 |
| enemy damage, effect and PP damage | 100 | 50 | 30 | 10 | 10 | 10 |
| an added effect's chance | 100 | 60 | 30 | 30 | 30 | 30 |

Avalanche is a damage ability with a power of 160 and a range of 3. At an even
match the aimed foe takes 160 to 163, one place away 128 to 131 and two places
away 96 to 99; with the user's Power 20 ahead, 175 to 178, 140 to 143 and 105
to 108, and a defending aimed foe 87 to 89. Blows work differently: with
Attack 100 against Defense 60, a plain Attack deals half the difference, 20
to 23; Ragnarok (power 35) adds twice its power before halving, 55 to 58;
Flint (power 16) multiplies the blow by 1.6, 32 to 35. Cure (power 70)
restores 70 to 73 HP at Power 100 and 83 to 86 at 120; the Healing Ring, which
uses Cure's row, always restores 70 to 73, since an item's user counts as
Power 100.

The tables are in `battle_calc.s`.

### Whether the added effect lands

![Ivan casts Confuse at Isaac, Garet and Mia: a pale blue mist rolls over the three, and the messages read `Isaac is confused!` and `But it has no effect on Garet!`](https://alchemy.pascalpixel.com/abilities/confuse.gif)

When Ivan casts Confuse at a group, some fall to it and the player reads
`But it has no effect on <unit>!` for the others. Each target gets its own
roll, and the chance depends on the effect, the two units' element levels, the
target's Luck and weaknesses, and its distance from the aimed place.

The roll fails at once when there is nothing to do (a revival on a unit still
standing, a cure on a unit without the status). Otherwise the chance starts
from the effect's own rate (Sleep 45 percent, Taint 55, Condemn 20 and so on),
gains 3 for each element level the user is ahead, loses 3 for every two
points of the target's Luck, gains 25 when the target is weak to the effect,
and is scaled by the distance. A few effects have a fixed chance instead:
Quartz 60 percent, Dew 90, and 100 for an effect with no rate of its own. When
the added effect of an effect-kind ability fails, the target takes no damage
either.

`Battle_RollEffectSuccess` (`../game/game/battle_calc.c`) makes the roll.

### Calling an ally, and effects that change the damage step

![A Fighter Bee uses Ally Search: `Fighter Bee looks for allies!`, and a second Fighter Bee flies in beside it, `Fighter Bee2 appears!!`.](https://alchemy.pascalpixel.com/guides/battle/ally-search.gif)

Some added effects stand in for the damage or change how it is dealt, so they
are applied before it. The most visible brings in a new enemy.

Ally Search, Search for Help and Speed Surge bring one in when the roll
succeeds and there is room for another enemy and its
[sprite](../../glossary.md#sprite-and-oam): it takes an empty place or joins
the end of the line, the enemies are lined up again, and the player reads
`<unit> appears!!` or `It split off into <unit>!`; otherwise
`But no one came!` or `It failed to divide!`.

Other effects that landed change the damage itself: Torch's blow counts half
the target's Defense; Severe Blow and Thrash leave the target 1 HP at most;
Condemn and the other instant deaths skip the damage; Dynamite and Mortal Blow
remove their user; Psy Drain takes PP instead of HP; and Ground holds a
target only if it has not acted yet this turn.

### Applying the added effect

![Garet casts Impair at Mia: a shield appears over her and shatters, and `Mia's Defense drops by 54!`.](https://alchemy.pascalpixel.com/abilities/impair.gif)

Statuses and stat changes are counters on the unit that the end of each turn
counts down. Impair lowers Mia's Defense for seven turns, and the number in the
message is how far it fell.

The effect is applied when it landed and the target is still standing; a
revival needs the target down instead, and a drain still works on a target its
own damage has just felled. The rules the effects share are these.

- A stat change moves Attack, Defense or Resistance one or two stages, at
  most four either way, for seven turns. A stage is an eighth of Attack or
  Defense, and the message gives how far the stat moved, so a stat at its
  limit shows 0. Zephyr and Vine set Agility outright, to double or half, for
  five turns.
- A status (Delude, Confuse, Charm, Paralyze, Sleep) and Bind's seal last
  seven turns. Luff's seal lasts two turns that never end early. Curse starts
  the Grim Reaper's countdown of seven turns.
- A lowered stat or a status may end early: once five turns or fewer are left,
  each turn's end rolls for it, more often for a lucky unit, and the player
  reads, for a delusion, `<unit> sees clearly once again!`. A raised stat
  always lasts its turns.
- Restore and Elixir clear delusion, stun, sleep, the seal and the Grim
  Reaper's countdown; Tonic clears those, poison and the evil spirit. Neither
  clears confusion or charm.
- Drain gives the user all the HP the target lost, Sap half, Psy Drain the PP
  it took, and Psy Leech a tenth of the damage as PP.
- Defend and Granite halve the damage the target takes this turn, Total
  Defense and Flash cut it to a tenth. Break returns every raised stage to 0
  and removes a summon's boost. Kite lets the target act twice next turn,
  Ground costs it its next action, and Haunt sets an evil spirit on it.

Each effect's details are in the case comments of
`BattleAbility_ApplyToTarget` and the notes on `enum AbilityEffect`
([`battle_unit.h`](../../include/battle/battle_unit.h)).

### After each target

![Isaac casts Quake at Garet, Ivan and Mia while Ivan is asleep: Quake's blow wakes him, `Ivan wakes from slumber!`.](https://alchemy.pascalpixel.com/guides/battle/wake.gif)

A sleeping unit that is hit may wake up, and a unit haunted by an evil spirit
pays for its own attacks.

Losing HP or PP wakes a sleeping target one time in four, unless it fell
asleep this turn. A haunted unit, one time in four for each target it took
HP or PP from, keeps aside a quarter of that loss (at least 1), and loses the
total from its own HP at the end of its action. In the Djinn tutorial a
Djinni's blow always leaves its target 1 HP, and a summon always fells it.

## Rules and pictures meet at the playback queue

![Ivan casts Plasma at Garet, Isaac and Mia: lightning bursts over them, and the damage messages follow one target at a time, `Garet takes 46 damage!`, `Isaac takes 56 damage!`, `Mia takes 44 damage!!!`.](https://alchemy.pascalpixel.com/abilities/plasma.gif)

Each target's HP drops at once, but the player sees it happen in steps: the
target flinches, its message appears and waits, and only then does the next
target come. The rules leave a list of what to show, in order, and the
screen plays the list.

That list, the playback queue, is the one way the rules reach the screen: the
rules never draw or wait, and a battle effect never works out an outcome.
[Battle](../../battle.md#rules-and-battle-effects-run-together) shows how
playback runs alongside the battle effect and starts on the frame the blow lands.
Playback takes the targets in their order along the battlefield, not the aimed
one first. Only the flinch (nine [frames](../../glossary.md#frame) of
flashing) and the message (until the player presses A) make it wait; the names
and numbers a message needs play within one frame, just before it. An
outcome shown with no battle effect, such as a cast that is sealed or short of
PP, plays the same queue straight through.

The queue is `struct BattlePlaybackQueue`, filled by `BattlePlayback_Push`
and played by `BattlePlayback_Run` ([`battle/playback.c`](battle/playback.c)),
or by `BattlePlayback_RunAll` when there is no battle effect.

## Design

The rules and the pictures are kept apart on purpose. The rules decide
everything before or while the battle effect plays, and leave only a list of what
to show; the battle effect decides only when the damage appears. So the same
arithmetic serves every battle effect, and a battle effect can be replaced or
skipped without changing an outcome.

Actions are worked out in two stages for the same reason: what the battle effect
must show (a miss, a critical hit, a double blow) is rolled while the action is
built, before anything is drawn, and everything else is rolled target by
target as the blow lands.

Some of the original's functions promise to return a value and end without
one, and no caller reads it; the source keeps the promised type where writing
`void` would change the compiled code, with the `void` form behind
[`UBFIX`](../../glossary.md#bug-bugfix-and-ubfix) ([Matching](../MATCHING.md)).

The battle is one object in the ROM and one in the build; its parts are cut
where one family of functions ends and the next begins.

## How it fits with the rest of the game

To the rest of the game a battle is one more sequence: the field leaves for it
through an exit and comes back to the same spot
([Battle](../../battle.md#starting-a-battle)). While it runs the battle
borrows the system's [tasks](../../glossary.md#task) and heap, the field's actors, the
window code for its menus and status window, and the
[game rules](../game/README.md) for its tables and arithmetic, which sit in
another [bank](../../glossary.md#bank) and are reached by far calls. The
battle effects ([Battle effects](../battle_effect/README.md)) are called by the
staging and hand back control when they finish.

## Limits

- There are 519 abilities. The action reads `gAbilityBattleEffects` only up to
  Thrash, and `gAbilityStaging`, a row shorter, up to Severe Blow.
- An action reaches at most `TARGET_MAX` targets.
- The party fights four at a time and the enemies six at most; in a link
  battle each console sends three.
- `formations.c` has 380 formations, each of up to five kinds of enemy with
  the least and most of each. The encounter zones in
  `../overworld/encounter_zones.c` choose among them.
- The playback queue holds `BATTLE_PLAYBACK_CAPACITY` (64) events.

## Words used here

- **unit**, **unit id**: everyone in a fight. Party members keep their
  character numbers as unit ids, and enemies take ids from 128
  (`FIRST_ENEMY_UNIT`). A unit is a `struct Unit` (`Unit_GetFar`).
- **ability**, **ability id**: anything a unit does when it acts, from Attack
  to Quake, a Djinni, a summon, an item's use or an enemy's move. The id is a
  row number in three tables: `../game/abilities.c`, `gAbilityBattleEffects` and
  `gAbilityStaging`.
- **ability kind**: which formula an ability's targets go through. The
  battle effect an ability plays is chosen separately.
- **power**: what an ability's power counts depends on its kind. A unit's
  **Power** and **Resist**, capitalized as the Status screen writes them,
  are its stats in each element.
- **added effect**: what an ability does besides damage or healing
  (`enum AbilityEffect`).
- **element level**: the level the Status screen shows under each element.
  A character's Power and Resist start from it, and it moves the chance of an
  added effect.
- **place**: a unit's position in its side's line-up.
- **side**: the party or the enemies.
- **turn**: one pass in which every unit acts once, in Agility order; the
  game's word, as in `HP recovery / turn`. Stat changes and statuses last a
  number of turns.
- **action**: one unit's go within a turn: a `struct BattleAction` while the
  turn is gathered, then, once built, the action under way
  (`struct BattleActionRun`): the acting unit, the
  ability, its element, its targets with their distances and rolls, its
  staging and its battle effect.
- **battle object**: a unit's figure on the battlefield
  (`struct BattleObject`), with its shadow and status icon. The Kraken and the
  Fusion Dragon are drawn in two parts.
- **event**: one thing the battle shows while an action plays, waiting in the
  playback queue: a message, a name or number the next message needs, a
  flinch, a defeat, a redraw or a sound.
- **pose**: how a battle object stands (`POSE_*`).

The parts give the same locals the same names: `battle` or `work` for
`gBattleWork`, `unitId`, `unit` for its `struct Unit`, `battleObject` for its
`struct BattleObject`, `actor` for that object's `struct Actor`, `record`
for one of its sprites, and `plan` for the action under way.

## Evidence

Abilities and enemies are named from the label columns of their tables,
`ability` and `enemy`, which follow the English release's text. The formulas
are named from what `battle_calc.c` computes, and the event kinds from what
playback does with each. The shadow under each unit is named from what the
battle's recordings show.

The chances and counts on this page were checked in a tbs-en build with
`alchemy-devtools render`, answering `Battle_Random` with chosen numbers.
Replaying a Gryphon's action with each of the 256 rolls gave its actions in the
spans above. One encounter (the Lizard King, a Harridan and two Stone
Soldiers) opened both as a first strike, where only the four party members
acted, and as a surprise, where three of the enemies did. Isaac's attacks on a
copy of Ivan gave a miss (`Ivan nimbly dodges the blow!`), a critical hit of
110 and, with the Swift Sword unleashing Sonic Smash, a triple blow of 281.
Real calls of `Battle_GetElementMultiplier` fell on the element line: for
Quake, Ivan at 52 points ahead and 1.26 times, Mia at 51 and 1.25, Isaac's
copy at 30 behind and 0.85, and for an attack on Ivan, 8 behind and 0.98 on
the blow's line. Against a Vermin, its delusion ended early with 5 turns left
while both Defense changes ran their 7 turns. Aiming Quake along four large
enemies moved the cursor from the last place round to the first. The party at
level 20 ran from two Gryphons on its second try, the first forced to fail.

Bits 0 and 1 of the `flags` column of `gAbilityBattleEffects` (`EFFECT_ON_DOWNED`,
`EFFECT_OTHER_SIDE`) are read, but no row in either game sets them. Building
the action adds the first itself for the revive effects; nothing adds the second, so the
side flip it asks for is unused. Only the Mars Lighthouse bosses (Saturos's
second fight, Menardi and the Fusion Dragon) set the class bit that makes a
beam blow land with `SE_HEAVY_IMPACT` (`BattleSprite_HasHeavyImpact`).

The battle camera has a second point it can follow besides `followPosition`:
`followEye`, which `ActorSystem_ProjectSprites` would use in place of the eye
when it turns the sprites to face the camera. Every camera setup (the
battle's, the World Map's and Lucky Dice's) clears it and nothing sets it, so
the sprites always face the real eye. The last two words of
`BattleBackgroundWork`, `unusedAfterFlag`, are cleared when a battle starts;
no code in the battle, its battle effects or the field reads them.
