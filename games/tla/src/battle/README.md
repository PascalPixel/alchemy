# Battles

This folder is the battle sequence: one fight from its opening curtain to
its spoils. [Battle](../../battle.md) gives the shape. This page follows a
fight in order and says which routine does each part.

## Starting a battle

A battle is handed one number, a [formation](../../glossary.md#formation),
and `Battle_RunEncounter` plays the whole fight from it.

### The enemies of a formation

A formation lists up to five kinds of enemy, each with a fewest and a most.
Every kind comes its fewest number of times, then each gains a random
number more while there is room: six places on the screen, a large enemy
taking two. So one formation is a family of line-ups, not one. Each enemy
becomes a unit filled in from its row of the enemy table. It is
`BattleFormation_BuildEnemyList`.

The Lost Age has 660 formations. Here they are still assembly rows in
[`battle_tables.s`](battle_tables.s), `gBattleFormation_Records`; The Broken
Seal writes its formations as a C array. With one game flag set, the
battle passes over the number it was given: it picks among the formations
whose enemies are near the party's average level, and moves the enemies'
levels by the difference.

## Turns

A turn is written down before it is played. Each action is brief: who
acts, the command, where it is aimed, and a priority to sort by. Then the
list is sorted and played from the top (`Battle_GatherActions`).

### The party's commands

The Lost Age's party can be eight strong, and four fight at once: the
first four in the party's order, with the rest waiting behind. A turn opens
on the party's row: Fight, Switch, Flee and Status. Switch is there while
more than four travel together. It picks a place in the line, then the one
from behind who takes it, and B on the party's row puts the last switch
back. Status shows the ones behind as well.

Then the menu asks each party member in front and still standing for an
action (`Battle_CollectPartyCommands`). One who is asleep or stunned is not
asked, but an action is still written for them with no command in it, and
their place in the order is when the player reads why they did nothing.

When everyone in front is down and someone behind still stands, the ones
behind step into the fallen places at the start of the next turn
(`BattleParty_SwapInReserves`). The battle is lost only when all of them
are down. A link battle is three against three, with nobody behind.

Enemies choose by dice from a list of eight moves in their row of the
enemy table, weighted by a habit, such as favouring the start of the list
(`BattleAI_ChooseAction`). An enemy may act more than once a turn, each
later action a little slower than the one before.

Priority is Agility, with a little luck added for party members. A few
things jump the queue by having their priority raised past any Agility,
which is how a guarding Djinni always goes first (`BattleActionOrder_Sort`).

## Building an action

Just before a unit acts, its brief action is turned into a full
description, the action run: the ability, everyone it reaches, and every
roll that affects how it will look (`BattleActionRun_Build`).

### Commands and ability ids

Everything a unit can do is an ability, a row of the ability table, known
by its row number. The command decides which ability the action carries:
Attack is the ability Attack unless the weapon unleashes, Psynergy is the
one chosen, an item is the item's ability, and so on. After this point the
battle no longer cares which menu the action came from.

Reach is counted in places along a side's line. A range of 1 is the unit
aimed at, and each point more adds a place on either side.

### Staging and announcement

How an action is shown is its [staging](../../glossary.md#staging). Most
abilities are staged by what they are: an Attack is a run and a blow,
anything that costs PP is a cast, an item is raised. The rest name a
staging of their own in a table, and beside it a second names each
ability's battle effect. Both are assembly rows in `battle_tables.s` here
too.

## Staging an action

Staging is the part the player watches, in `BattleStaging_DispatchAction`:
the attacker's run and leap, the caster's raised hand and glow, a summon
taking the whole screen while the rest of its side fades out. It starts the
ability's battle effect at the right moment, so that a leap and its blow
meet.

## How outcomes reach the screen

Working out what an ability does to a target takes no time. Its HP drops
in a single frame. What takes time is the showing: the flinch, the number,
the line of text waiting for a button.

So the two are split. Resolving changes the numbers at once and leaves
behind a list of things to show (`BattlePlayback_Push`). A task plays that
list a little each frame, while the battle effect goes on drawing
(`BattlePlayback_Run`). That is how lightning and text share the screen.

## How an ability resolves

For each target in turn, the rules decide the damage or healing and
whether the ability's added effect, such as sleep or poison, takes hold
(`BattleTarget_Execute`).

### The ability kind

Every ability has a power, and its kind says what the power means. For an
attack kind it is added to an ordinary blow, so the user's Attack counts.
For a damage kind the Attack does not come into it, only the user's Power
in the ability's element. Healing and the rest have kinds of their own.
The sums are in [Game rules](../game/README.md#battle-arithmetic).

### The element multiplier

Each unit has a Power and a Resist in each element, and the gap between
the user's Power and the target's Resist scales the result. The damage
line tells the player: it ends in three exclamation marks when the element
is the one the target resists worst, and a full stop when it is the one it
resists best.

An ability that reaches several targets also falls off with distance from
the one aimed at, by tables the two games share.

## The end of an action and of a turn

After each action the things it leaves behind are settled and everyone
returns to their spot (`BattleUnit_ProcessActionEnd`). After each turn the
lasting effects are counted down (`Battle_ProcessTurnEnd`).

## The end of the battle

A win gives experience to every party member still standing, in full to
the ones in front and half to the ones behind, then coins, then items,
each level gained shown as it happens (`Battle_AwardSpoils`). Then the
battle writes its result, and the main routine decides where the party
goes.

## The zoomed battlefield

The battlefield is one flat picture on the layer the console can zoom, and
the camera is that layer's zoom and slide. The picture is larger than the
layer can hold, so it is kept as an upper and a lower half, and the layer
is switched from one to the other partway down the screen as it is drawn.
`BattleCamera_SetRange` writes where the switch falls on each line, and
`BattleBackground_ShowZoomedPage` starts it every frame.

## The other files

- `BattleTransition_Run` is the opening: the curtain of tiles closing in,
  or the mosaic fade to black.
- `AutoBattle_BuildActions` and `AutoBattle_RunAction` build and play
  actions without the command menu.
- `LinkBattle_SyncTurn` and its neighbours are a battle against another
  console: the parties exchanged and the handshake after every action.
- The bank's C is two objects, written as parts cut where The Broken
  Seal's are. Its tables are the assembly rows of `battle_tables.s`, the
  debug battles' `gDebug_ParameterSets` among them.
