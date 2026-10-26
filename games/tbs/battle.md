# Battle

When a random battle starts, the field gives way to a battlefield where the
[party](glossary.md#the-party) faces the enemies. The battle then runs in
[rounds](glossary.md#rounds-and-turns): the player picks a command for each
party member, every enemy picks one of its own, and the game plays the
actions one after another, fastest first. When one side has nobody left
standing, or the party runs away, the field comes back.

Two kinds of code share the work. The rules decide what happens: who acts
when, who is hit and for how much, which statuses take hold. The
[animations](glossary.md#battle-animation) draw it. [Battles](src/battle/README.md)
goes through the rules in detail, [Game rules](src/game/README.md) holds the
tables and arithmetic they use, and [Battle animations](src/battle_anim/README.md)
draws them.

## Starting a battle

![A battle in Goma Cave ends: Ivan's Magic Rod puts a Skeleton to sleep, Isaac and Garet strike it down, the spoils are announced, and the screen fades back to the cave with Isaac standing where the battle found him.](https://alchemy.pascalpixel.com/guides/battle/won.gif)

To the rest of the game a battle is one more [sequence](glossary.md#sequence). The
field leaves for it as it would leave through a doorway, and when the battle
is won it comes back to the same spot. After a defeat it goes to the place the
game sends a beaten party instead.

A random encounter raises a field [exit](glossary.md#exit) like any other,
whose destination is the battle and whose arrival point names the enemies'
formation. The game clears its [tasks](glossary.md#task) and its
[heap](glossary.md#heap-and-pools), as it does between any two scenes, and
then, instead of loading the next scene's [overlay](glossary.md#overlay), runs
the battle from start to finish. The battle's code sits in its own
[bank](glossary.md#bank) of the [main image](glossary.md#main-image), reached
by a [far call](glossary.md#far-call). When it returns, the field scene is
loaded again, or after a defeat the scene the defeat rules name. The debug
battle menu is the only other way in.

`SequenceManager_Run` (`src/overworld/overworld/map.c`) sees the next scene
`SEQUENCE_BATTLE` and calls `Battle_RunFar`, which leads to `Battle_Run`
(`src/battle/battle/encounter.c`). The formations are the rows of
`src/battle/formations.c`, chosen by the encounter zones in
`src/overworld/encounter_zones.c`.

[Running a scene](src/overworld/README.md#random-battles) covers how the field
rolls the encounter, and [Battles](src/battle/README.md#rounds) what the
battle does from the opening to the spoils.

## Units and abilities

![At the start of a round Isaac opens his Psynergy, picks Quake and aims it at the Harridan; Garet looks through his Djinn and backs out to attack; Ivan defends and Mia attacks. The round begins with `Ivan is defending!` and `Isaac casts Quake!`.](https://alchemy.pascalpixel.com/guides/battle/commands.gif)

Everyone in a fight, party member or enemy, is a unit, and everything a unit
can do on its turn is an ability: Attack, Quake, a [Djinni](glossary.md#djinn),
a [summon](glossary.md#summons), using an item, an enemy's special move. The
command menu is how the player chooses one for each party member.

A unit has its HP, [PP](glossary.md#psynergy-and-pp), stats and statuses, and
a [Power and a Resist](glossary.md#power-and-resist) in each
[element](glossary.md#the-four-elements). Up to four party members and six
enemies fight at once.

An ability is a number, and three tables say what that number means: what the
ability does (its targets, element, range, PP cost, power and added effect),
which animation draws it and how strongly, and anything special about how it
is staged. The rules and the picture are chosen separately, so the arithmetic
and the animation can change independently: Quake, Earthquake and Quake
Sphere share one animation at three strengths.

A unit is a `struct Unit` (`Unit_GetFar`); the ability tables are
`src/game/abilities.c` (`struct AbilityDefinition`, read by `Ability_Get`),
then `gAbilityAnimations` and `gAbilityStaging` in
`src/battle/ability_staging.c`.

[Game rules](src/game/README.md#how-one-number-finds-a-row-and-its-name)
has the tables, and [Battles](src/battle/README.md#the-ability-kind) every
kind of ability.

## Rounds and turns

![Isaac and Garet against two Gryphons: both attack, then the first Gryphon casts Impact and its partner's Attack rises, and the second casts Tornado on the pair; the next round's menu opens.](https://alchemy.pascalpixel.com/guides/battle/round.gif)

A round is one pass of choosing, sorting and playing actions. The faster a
unit, the earlier it acts, and after every action the game checks whether a
side has been beaten, so a battle can end in the middle of a round.

The party's actions come from the command menu (or are picked automatically
in the auto battle), the enemies' from each enemy's habits. They are sorted
by Agility, except that the Djinn Granite, Flash and Ground go before
everyone. Then each action in turn is announced, staged and animated, and its
targets take its effect; the actor's turn ends; and when every action has
played, the round ends with statuses counting down. The loop in `Battle_Run`,
simplified:

```c
for (;;) {                                     /* one round */
  actionCount = Battle_GatherActions(battle->actions);
  for (i = 0; i < actionCount; i++) {          /* one action */
    if (BattleStaging_DispatchAction(&battle->actions[i], ...) == 1)
      goto fled;
    if (BattleUnit_ListLiving(BATTLE_SIDE_MASK_PARTY, 0) == 0)
      goto party_lost;
    if (BattleUnit_ListLiving(BATTLE_SIDE_MASK_ENEMIES, 0) == 0)
      goto won;
  }
  Battle_ProcessRoundEnd();
}
```

[Battles](src/battle/README.md#rounds) goes through each step, with
[enemy actions](src/battle/README.md#enemy-actions),
[first strikes and fleeing](src/battle/README.md#first-strikes-and-fleeing)
and [building a turn](src/battle/README.md#building-a-turn).

## Resolving an ability

![Isaac casts Avalanche at Ivan, Garet and Mia: a wall of rock rises among them, and Ivan, the one aimed at, takes 203 damage, Garet beside him 160 and Mia 120.](https://alchemy.pascalpixel.com/abilities/avalanche.gif)

When Avalanche hits three enemies, the one aimed at takes the most and the
ones beside it less, and an enemy weak to Venus takes more than one that
resists it. Every damaging Psynergy works its number out the same way: the
ability's power, scaled by the element and by the distance, plus a little
luck.

An ability's power is the damage the aimed target takes at an even match,
when the caster's Power in the element equals the target's Resist; Avalanche's
is 160.

- The element: each point of Power ahead of the Resist adds half a percent, up
  to double, and each point behind takes half a percent off. Twenty points
  ahead gives 281 in 256, so 160 becomes 175.
- The distance, counted in places along the target side's line: one place
  away keeps 80 percent and two places 60, so 175, 140 and 105.
- The luck: a random 0 to 3 is added.
- The guard: a defending target takes half; one behind Total Defense or the
  Djinni Flash a tenth.

Every step rounds down. Before the damage the game rolls whether the
ability's added effect, such as Sleep, takes hold, and afterwards applies it.

`BattleAbility_ApplyToTarget` (`src/battle/battle/target_exec.c`) does this
for each target; the formulas are in `src/game/game/battle_calc.c`.

[Battles](src/battle/README.md#how-an-ability-resolves) has every kind of
ability's formula, the fall-off tables and the added effects.

## The battle's picture

![A battle opens on the Lizard King, a Harridan and two Stone Soldiers: the camera sweeps in from close beside the enemies to a wide view as each is announced, then swings round behind the party as the first menu opens, the battlefield sliding and growing with it.](https://alchemy.pascalpixel.com/guides/battle/opening.gif)

The battlefield seems to stretch into the distance and slides sideways as the
camera swings round, while the status window and the messages stay flat in
front. The battlefield is one flat picture of the place, 256 by 120 pixels,
with the depth already painted in; the hardware only zooms and slides it.

The screen is built from layers: the windows on one, the battlefield on
another that the hardware can scale and turn, and the units as
[sprites](glossary.md#sprite-and-oam) over them. Once a frame the camera's
distance sets the zoom and its turn the sideways scroll. The zooming layer can
only name 256 small tiles and the battlefield picture has 480, so the picture
is kept in two halves, and on every screen line the game tells the hardware
which half to read next, switching at the line where the lower half starts at
this zoom. The lines under the status window show no battlefield at all.

An animation that needs the zooming layer for its own picture moves the
battlefield onto a fixed-size layer for a while
([the canvas](src/battle_anim/README.md#the-canvas)).

| Layer | What it holds |
| --- | --- |
| `BG0` | the windows: status and messages |
| `BG1` | the battlefield at a fixed size, while an animation borrows `BG2` |
| `BG2` | the battlefield zoomed with the camera, or an animation's canvas |
| sprites | the units |

`BattleCamera_SetRange` (`src/battle/battle/background.c`) sets the zoom and
builds the per-line table that an HBlank DMA writes into `BG2CNT`.

## Animations

![Isaac summons Ramses: the screen goes black, Ramses rises over a desert under a purple sky and brings down his fist in a burst of fire, the battle comes back with `Garet takes 151 damage!!!`, and `Isaac's Earth Power rises by 30!`.](https://alchemy.pascalpixel.com/abilities/ramses.gif)

Every ability's animation, from a sword swing to a summon's whole animation, is one
function that draws it frame by frame and returns when it is over, while the
battle waits. Most draw into a small hidden picture, the canvas, which the
screen shows stretched and blended over the battle.

There are three ways in: the ordinary animations from one table, a range of
numbered animations (the impacts of Djinn and weapon unleashes, the plain attacks'
beams and critical hits), and the summons' animations. Each borrows three blocks
of memory for the animation (its pictures, its working space and the canvas)
and gives them back afterwards, so an animation must stop everything that
reads them before it returns. Within a frame its steps run in a fixed order,
and many draw random numbers, so moving one step changes everything drawn
after it.

The three entry points are `BattleAnim_RunTableEffect`,
`BattleAnim_DispatchByIdRange` and `BattleAnim_RunSummonEffect`.

[Battle animations](src/battle_anim/README.md#how-a-move-picks-its-animation)
describes how the animation is chosen, then
[the canvas](src/battle_anim/README.md#the-canvas),
[the blitters](src/battle_anim/README.md#blitters),
[the particles](src/battle_anim/README.md#particles) and every
[animation family](src/battle_anim/README.md#animation-families).

## Rules and animations run together

![Isaac casts Quake: rock pillars rise under Garet, Ivan and Mia, and the damage messages appear while the rock is still on screen.](https://alchemy.pascalpixel.com/abilities/quake.gif)

The damage messages appear while the rock is still rising: the animation does
not have to finish first. The animation decides only when the damage appears;
the rules alone decide how much it is.

An animation is an ordinary function with its own loop: it waits a frame
whenever time has to pass and returns only when it is over. The damage comes
from a [task](glossary.md#task), which the waiting runs once a frame
([Engine](engine.md#waiting-and-tasks)). That task, playback, stays idle until
the animation signals that the blow has landed. From then on it applies the
ability to one target at a time and plays what that target's rules left
behind: the flinch, the message, the falling HP. An animation that never
signals, such as Torch's, leaves playback to start once it returns, so its
damage shows afterwards.

The staging runners in `src/battle/battle/staging.c` all follow this pattern;
`BattleStaging_RunPsynergyCast`, simplified:

```c
Task_Add(BattlePlayback_Run, TASK_PRIORITY_MAIN_LOOP); /* idle task */
if (plan->animation & ANIM_BY_ID_RANGE)
  BattleAnim_DispatchByIdRangeFar(&work.effect); /* the animation's loop */
else
  BattleAnim_RunTableEffectFar(&work.effect);
BattlePlayback_WaitForReady(); /* until every target has played */
```

The signal is `ANIM_SHOW_DAMAGE`, and playback is in
`src/battle/battle/playback.c`.

[Battles](src/battle/README.md#rules-and-pictures-meet-at-the-playback-queue)
takes one target's queue apart.

## Limits

- There are 519 abilities.
- A turn reaches at most 14 targets.
- The party fights four at a time and the enemies six at most.
- There are 380 formations, each of up to five kinds of enemy.
- The playback queue holds 64 entries, and nothing checks that it does not
  overflow.
- The canvas is 128 by 128 pixels, an animation has room for 64 particles of
  its own, and it can draw with two blitters at once.
