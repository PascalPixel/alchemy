# Battle

A battle is where the [party](glossary.md#the-party) faces a group of enemies
on a battlefield of its own. It runs in
[turns](glossary.md#turns-and-actions): the player picks a command for each
party member, every enemy picks one of its own, and the game plays the
actions one after another, fastest first, until one side has nobody left
standing or the party runs away. Two kinds of code share the work: the rules
decide who acts when, who is hit and for how much, and the
[battle effects](glossary.md#battle-effect) draw it.
[Battles](src/battle/README.md) goes through the rules,
[Game rules](glossary.md#game-rules) holds their tables and arithmetic, and
[Battle effects](src/battle_effect/README.md) draws what they decide.

## Starting a battle

![Isaac walks up and down a narrow passage of Goma Cave until a random battle starts: the screen flashes and goes black, and the battle opens on Isaac, Garet and Ivan facing a Ghost, with "Ghost appeared!"](https://alchemy.pascalpixel.com/guides/object/random-battle.gif)

A battle is a [sequence](glossary.md#sequence) like any other, which the
[main routine](glossary.md#main-routine) runs between two
[scenes](glossary.md#scene).

Every way into a battle in play leaves the same request in the field's
[work](glossary.md#work): the number of a
[formation](glossary.md#formation), the enemies to fight. A random encounter
sets it after the party has walked far enough
([Random encounters](field.md#random-encounters)), and a chest that
turns out to be a mimic sets it from its
[examine trigger](glossary.md#examine-trigger). An
[event](glossary.md#event) sets it with
`Field_RequestBattleFromEncounterTable` (or `Field_RequestDjinnBattle` for a
[Djinni](glossary.md#djinn)), and may first mark the battle
`BATTLE_TYPE_SCRIPTED`, so the party cannot run, and set where the party goes
if it wins and if it loses, as the thieves in Vault's inn loft do. The battle
starts once the event has returned.

The [field sequence](glossary.md#field-sequence)'s loop (`FieldSequence_Run`)
answers the request: it saves the scene's [actors](glossary.md#actor)
(`Actor_Snapshot`), writes `SEQUENCE_BATTLE` as the next sequence and the
formation as its entrance into `gMainRoutineWork`, and ends the scene. The
main routine's next pass tears the scene down, so the battle starts cold
([System](system.md#the-main-routine)), and calls `Battle_RunFar`
([far call](glossary.md#far-call)) with the formation. `Battle_Run` (`src/battle/battle/run.c`) plays the whole
battle and returns its outcome. A loss goes to the
[destination](glossary.md#destination) an event set for a defeat, or else to
the restart point, which is most often the last Sanctum; a win or a flight
goes back to the scene the battle started from, unless an event set another,
and the saved actors are put back (`FLAG_KEEP_PARTY_POSITION`).

## Units and abilities

Everyone in a fight, party member or enemy, is a
[unit](glossary.md#unit), and everything a unit can do when it acts is an
ability: Attack, Quake, a Djinni, a [summon](glossary.md#summons), using an
item, an enemy's special move. The command menu is how the player chooses one
for each party member.

A unit has its HP, [PP](glossary.md#psynergy-and-pp), stats and statuses, and
for every [element](glossary.md#the-four-elements) its own
[Power and Resist](glossary.md#power-and-resist).

An ability is a number, and three tables say what that number means: what the
ability does (its targets, element, range, PP cost, power and added effect),
which battle effect draws it and how strongly, and anything special about how it
is staged. The rules and the picture are chosen separately, so abilities with
different arithmetic can share one battle effect: Quake, Earthquake and Quake
Sphere share one at three strengths.

A unit is a `struct Unit`, read by `Unit_Get` (`src/game/game/units.c`); the
ability tables are `src/game/ability_table.c` (`struct AbilityDefinition`, read by
`Ability_Get`), then `gAbilityBattleEffects` and `gAbilityStaging` in
`src/battle/ability_staging.c`.
[Game rules](src/game/README.md) holds the units and every table of items,
abilities, Djinn, enemies, summons and classes.

## Turns and actions

A turn is one pass of choosing, sorting and playing actions. The faster a
unit, the earlier it acts, and after every action the game checks whether a
side has been beaten, so a battle can end in the middle of a turn.

The party's actions come from the command menu, the enemies' from each
enemy's habits. They are sorted by Agility, except that the Djinn Granite,
Flash and Ground go before everyone. Then each action is announced,
[staged](glossary.md#staging) and drawn, and its targets take its effect;
when every action has played, the turn ends with statuses counting down. The
loop in `Battle_Run`, simplified:

```c
for (;;) {                                     /* one turn */
  actionCount = Battle_GatherActions(battle->actions);
  for (i = 0; i < actionCount; i++) {          /* one action */
    if (BattleStaging_DispatchAction(&battle->actions[i], ...) == 1)
      goto fled;
    if (BattleUnit_ListLiving(BATTLE_SIDE_MASK_PARTY, 0) == 0)
      goto party_lost;
    if (BattleUnit_ListLiving(BATTLE_SIDE_MASK_ENEMIES, 0) == 0)
      goto won;
  }
  Battle_ProcessTurnEnd();
}
```

[Battles](src/battle/README.md#turns) follows a turn through.

## Resolving an ability

![Isaac casts Avalanche at Ivan, Garet and Mia: a wall of rock rises among them, and Ivan, the one aimed at, takes 203 damage, Garet beside him 160 and Mia 120.](https://alchemy.pascalpixel.com/abilities/avalanche.gif)

The ability recordings are made from Camelot's battle test menu, with copies
of the party's characters on the enemy side, placed as a link battle places
them, and their HP raised so that every cast leaves them standing.

When Avalanche hits three targets, the one aimed at takes the most and the
ones beside it less, and a target weak to Venus takes more than one that
resists it. Every damaging Psynergy works its number out the same way: the
ability's power, scaled by the element and by the distance, plus a little
luck.

An ability's power is the damage the aimed target takes at an even match,
when the caster's Power in the element equals the target's Resist; Avalanche's
is 160.

- The element: Power above the Resist raises the damage and Power below it
  lowers it, by half a percent a point. Twenty points ahead gives 281 in 256,
  so 160 becomes 175.
- The distance, counted in places along the target side's line: one place
  away keeps 80 percent and two places 60, so 175, 140 and 105.
- The luck: a random 0 to 3 is added.
- The guard: a defending target takes half; one behind Total Defense or the
  Djinni Flash a tenth.

Each of these rounds down. In the recording Ivan takes 203 rather than 160
because Isaac's Venus Power is above Ivan's Venus Resist, and Garet and Mia,
further along the line, take less. Before the damage the game rolls whether the
ability's added effect, such as Sleep, takes hold, and afterwards applies it.

`BattleAbility_ApplyToTarget` (`src/battle/battle/ability.c`) does this
for each target; the formulas are in `src/game/game/battle_arithmetic.c`.
[Battles](src/battle/README.md#how-an-ability-resolves) has every kind of
ability's formula, the fall-off tables and the added effects.

## The battlefield on screen

![A random battle in Goma Cave with its background layers and sprites pulled apart and spread one behind another, in the order the hardware stacks them.](https://alchemy.pascalpixel.com/guides/battle/layers-3d.gif)

The battlefield is one flat picture of the place, 256 by 120 pixels, with its
depth already painted in. Once a [frame](glossary.md#frame) the camera's
distance sets the hardware's zoom of it and the camera's swing its sideways
scroll, while the windows stay flat in front. The units are
[sprites](glossary.md#sprite-and-oam) over it.

| [Layer](glossary.md#background-layer) | What it holds |
| --- | --- |
| `BG0` | the windows: status and messages |
| `BG1` | the battlefield at a fixed size, while a battle effect borrows `BG2` |
| `BG2` | the battlefield zoomed with the camera, or a battle effect's canvas |
| sprites | the units |

`BattleCamera_SetRange` (`src/battle/battle/background.c`) sets the zoom;
[Battles](src/battle/README.md#the-zoomed-battlefield) explains how one
picture too large for the zooming layer is shown through it.

## Battle effects

![Isaac summons Ramses: the screen goes black, Ramses rises over a desert under a purple sky and brings down his fist in a burst of fire, the battle comes back with `Garet takes 151 damage!!!`, and `Isaac's Earth Power rises by 30!`.](https://alchemy.pascalpixel.com/abilities/ramses.gif)

Every ability's battle effect, from a sword swing to a summon's arrival, is one
function that draws it frame by frame and returns when it is over; the staging
runner that called it waits for it to return. Most draw into the
[canvas](src/battle_effect/README.md#the-canvas), a small picture kept off screen and
shown stretched and blended over the battle.

There are three ways in: the ordinary battle effects from one table
(`BattleEffect_RunFromTable`), a range of numbered battle effects, which are
the impacts of Djinn and weapon unleashes and the plain attacks' beams and
critical hits (`BattleEffect_RunByIdRange`), and the summons'
(`BattleEffect_RunSummon`). Each borrows three blocks of memory for the battle
effect (its pictures, its working space and the canvas) and gives them back
afterwards, so a battle effect must remove any task that still reads them
before it returns.
[Battle effects](src/battle_effect/README.md#how-an-ability-picks-its-effect)
describes how the battle effect is chosen and how it draws.

## Rules and battle effects run together

![Isaac casts Quake: rock pillars rise under Garet, Ivan and Mia, and the damage messages appear while the rock is still on screen.](https://alchemy.pascalpixel.com/abilities/quake.gif)

The damage messages appear while the rock is still rising: the battle effect
does not have to finish first. How much damage is the rules' business alone;
the battle effect only times when it shows.

A battle effect is an ordinary function with its own loop: it waits a frame
whenever time has to pass and returns only when it is over. The damage comes
from a [task](glossary.md#task), which the waiting runs once a frame
([System](system.md#waiting-and-tasks)). That task, playback, stays idle until
the battle effect signals that the blow has landed. From then on it applies the
ability to one target at a time and plays the
[playback queue](glossary.md#playback-queue) entries that target's rules left
behind: the flinch, the message, the falling HP. A battle effect that never
signals, such as Torch's, leaves playback to start once it returns, so its
damage shows afterwards.

The staging runners that play a battle effect (`staging.c`) follow this
pattern; `BattleStaging_RunPsynergyCast`, simplified:

```c
Task_Add(BattlePlayback_Run, TASK_PASS_FRAME); /* idle task */
if (plan->battleEffect & EFFECT_BY_ID_RANGE)
  BattleEffect_RunByIdRangeFar(&work.effect); /* the battle effect's loop */
else
  BattleEffect_RunFromTableFar(&work.effect);
BattlePlayback_WaitForReady(); /* until every target has played */
```

The signal is `EFFECT_SHOW_DAMAGE`, and playback is in
`src/battle/battle/playback.c`.
[Battles](src/battle/README.md#how-outcomes-reach-the-screen)
takes one target's queue apart.

## Limits

- There are 519 abilities.
- Four party members fight at once, against at most `BATTLE_ENEMY_CAPACITY`
  (6) enemies.
- `FORMATION_COUNT` (380) formations, each with up to five kinds of enemy.
- The playback queue holds `BATTLE_PLAYBACK_CAPACITY` (64) entries, and
  `BattlePlayback_Push` never checks that it does not overflow.
- A battle effect draws into a 128 by 128 pixel canvas and has room for 64
  particles of its own.
