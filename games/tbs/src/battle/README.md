# Battles

This folder is the battle sequence: one fight from its opening curtain to
its spoils. [Battle](../../battle.md) gives the shape. This page follows a
fight in order and says which file does each part.

## Starting a battle

![A battle opens: the curtain parts on a Lizard King, a Harridan and two Stone Soldiers, each announced one after another, `Lizard King appeared!`, `Harridan appeared!`, `Stone Soldier1 appeared!`, `Stone Soldier2 appeared!`, and the command menu comes up on Fight](https://alchemy.pascalpixel.com/guides/battle/opening.gif)

A battle is handed one number, a [formation](../../glossary.md#formation).

### The enemies of a formation

A formation lists up to five kinds of enemy, each with a fewest and a most.
Every kind comes its fewest number of times, then each gains a random
number more while there is room. So one formation is a family of line-ups,
not one. Each enemy becomes a unit filled in from its row of the enemy
table. It is `formation.c` and `setup.c`.

## Turns

![A turn against a Lizard King, a Harridan and two Stone Soldiers: Ivan, Isaac and Mia attack the Harridan one after another, Garet defends, the Stone Soldiers cast Blast and Mad Blast, and the Lizard King casts High Impact, and its Attack rises](https://alchemy.pascalpixel.com/guides/battle/turn-order.gif)

A turn is written down before it is played. Each action is brief: who
acts, the command, where it is aimed, and a priority to sort by. Then the
list is sorted and played from the top (`turn.c`).

### The party's commands

The menu asks each party member still standing for an action. One who is
asleep or stunned is not asked, but an action is still written for them
with no command in it, and their place in the order is when the player
reads why they did nothing.

Enemies choose by dice from a list of eight moves in their row of the
enemy table, weighted by a habit, such as favouring the start of the list
(`enemy_ai.c`).

A move marked for an item uses the first item the enemy holds. An enemy
that holds none is given the ability Can't Use, but an item command reads
that number as an inventory slot, slot 509, far past the unit's own
twelve. What lies there is never a usable item: for the first three
enemies it is an empty slot of another enemy, and for the others it is
memory past the enemy units that held nothing or the Claymore whenever it
was measured. Either way the enemy defends. Of the enemies that come here,
only a Rat Fighter stands that far along, fourth or fifth in its formations
with Apes, Grubs or Orcs.

Priority is Agility, with a little luck added for party members. A few
things jump the queue by having their priority raised past any Agility,
which is how a guarding Djinni always goes first.

## Building an action

Just before a unit acts, its brief action is turned into a full
description, the action run: the ability, everyone it reaches, and every
roll that affects how it will look (`action.c`, `targeting.c`).

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
staging of their own in a table.

## Staging an action

![Isaac attacks and his sword unleashes: `Isaac's Swift Sword lets out a howl! Sonic Smash!`, and a burst of light strikes Ivan](https://alchemy.pascalpixel.com/guides/battle/unleash.gif)

Staging is the part the player watches, in `staging.c`: the attacker's
run and leap, the caster's raised hand and glow, a summon taking the whole
screen while the rest of its side fades out. It starts the ability's
battle effect at the right moment, so that a leap and its blow meet.

## How outcomes reach the screen

![Ivan casts Plasma at Garet, Isaac and Mia: lightning bursts over them, and the damage lines follow one target at a time, first Garet's, then Isaac's, then Mia's.](https://alchemy.pascalpixel.com/abilities/plasma.gif)

Working out what an ability does to a target takes no time. Its HP drops
in a single frame. What takes time is the showing: the flinch, the number,
the line of text waiting for a button.

So the two are split. Resolving changes the numbers at once and leaves
behind a list of things to show. A task plays that list a little each
frame, while the battle effect goes on drawing. That is how lightning and
text share the screen, and it is `playback.c` and `playback_queue.c`.

## How an ability resolves

For each target in turn, the rules decide the damage or healing and
whether the ability's added effect, such as sleep or poison, takes hold
(`ability.c`). Nothing is shown while this happens: each step queues the
names, numbers, messages and reactions that playback then shows in order,
while the units themselves change at once.

A target is settled in seven steps:

1. A copy of the target as it was, its distance from the place aimed at,
   how well it resists the element, and the user's Power in it.
2. For a Djinni striking a weakness, the roll for the Djinn bonus, which
   gives more coins and experience for an enemy it fells; then whether
   the effect lands at all.
3. Calling an ally: an enemy calls another into an empty place.
4. The effects that change how damage works: Ground, Torch, Severe Blow,
   instant death, a self-destruct, Psy Drain.
5. The damage or healing, by the ability kind.
6. The added effect: cures, healing, stat changes, revival, statuses,
   drains, Break, the curse, guards.
7. Waking a sleeping target that took damage, one time in four, the
   stats worked out again, and a haunted user's share of what it took.

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
the one aimed at.

## The end of an action and of a turn

After each action the things it leaves behind are settled and everyone
returns to their spot. After each turn the lasting effects are counted
down (`turn_end.c`).

## The end of the battle

![In Goma Cave, Ivan's Magic Rod unleashes Murk on a Skeleton, which falls asleep; Isaac and Garet attack it until `Ghost's party goes down!`, and the experience and coins are handed out](https://alchemy.pascalpixel.com/guides/battle/won.gif)

A win gives experience to every party member still standing, then coins,
then items, each level gained shown as it happens (`spoils.c`). Then the
battle writes its result, and the main routine decides where the party
goes.

## The zoomed battlefield

![A random battle in Goma Cave with its background layers and sprites pulled apart and spread one behind another, in the order the hardware stacks them.](https://alchemy.pascalpixel.com/guides/battle/layers-3d.gif)

The battlefield is one flat picture on the layer the console can zoom, and
the camera is that layer's zoom and slide. The picture is larger than the
layer can hold, so it is kept as an upper and a lower half, and the layer
is switched from one to the other partway down the screen as it is drawn
(`background.c`).

## The other files

- `transition.c` is the opening: the curtain, or the storm on Venus
  Lighthouse.
- `auto_battle.c` builds and plays actions without the command menu.
- `link.c` is a battle against another console: the parties exchanged and
  the handshake each turn.
