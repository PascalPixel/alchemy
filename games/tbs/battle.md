# Battle

![A battle, round by round. At the start of each round everyone chooses an action: the party in the command menu, each enemy from its own list. The actions line up fastest first, by Agility. Then each action plays in turn: it is announced, its animation plays and each target is hit, and the unit's turn ends. After any action, if one side is down or the party ran, the battle ends, with coins, experience and items after a win. When everyone has acted, the next round begins.](https://alchemy.pascalpixel.com/illustrations/battle/round.svg)

When a random battle starts, the field gives way to a battlefield where the
party faces the enemies. From then on the battle runs in
rounds. At the start of a round the player picks a command for each party
member and every enemy picks one of its own. The game lines the actions up,
fastest first, and plays them one after another: an announcement such as
`Isaac casts Quake!`, the animation, and the damage and messages for each
target. When one side has nobody left standing, or the party runs away, the
battle ends and the field comes back.

Two kinds of code share the work. The rules decide what happens: who acts
when, who is hit and for how much, which statuses take hold. The effects
draw it: the rock that rises, the flash of a critical hit, a summon's whole
scene. They are kept apart so cleanly that a later target's damage is worked
out while the animation is still on screen. This page explains the battle as
a whole; [Battles](src/battle/README.md) goes through the rules in detail,
[Game rules](src/game/README.md) holds the tables and the arithmetic they
use, and [Battle effects](src/effect/README.md) the animations.

## Starting a battle

To the rest of the game a battle is one more map: the field leaves for it
as it would leave through a doorway, and comes back to the same place
afterwards.

In the code, a random encounter raises a field warp like any other, with
`MAP_BATTLE` as the next map and the enemy group in
`gGameState.warpDest`. `Game_Run` (`src/object/object/map.c`) clears what
the last map left running, as it does between any two maps: the task
table, the list of small jobs the game runs once a frame, and the heap, the
pool of memory a map borrows from. Then, instead of loading a map's
overlay (the code for one map, loaded when the party arrives there), it
calls `Battle_RunEncounterFar(warpDest)`, which fights the whole battle
before it returns; a `Far` name is the one other parts of the program use
to call into the battle's part. Afterwards `Map_ReturnFromBattle` reloads the field map the
battle started from, with the party where it was, or after a defeat goes to
the defeat point. The debug battle menu (`DebugBattle_RunMenu`) is the only
other way in. The battle's code is always present, in the `battle` bank of
the main program, rather than loaded as an overlay.

`Battle_RunEncounter` (`src/battle/battle/encounter.c`) allocates the
battle's work slots, `gBattleWork` among them, and sets `FLAG_IN_BATTLE`
while it runs. Its result is the number of enemies defeated,
`ENCOUNTER_FLED` or `ENCOUNTER_PARTY_LOST`. The enemy group is a row of
`formatn.tsv`, which `formation.c` turns into enemy units; the encounter
zones in `src/object/encounter.tsv` choose the row.

| Name | Value | Meaning |
| --- | --- | --- |
| `MAP_BATTLE` | `0x1fe` | the next map that runs a battle instead of an overlay |
| `FLAG_IN_BATTLE` | `0x103` | set while a battle runs |
| `ENCOUNTER_FLED` | 999 | `Battle_RunEncounter`'s result when the party got away |
| `ENCOUNTER_PARTY_LOST` | -1 | its result when the party was defeated |

[Running a map](src/object/README.md#random-battles) covers how the field
rolls the encounter, and [Battles](src/battle/README.md#rounds) what
`Battle_RunEncounter` does from the opening to the spoils.

## Units and abilities

Everyone in a fight, party member or enemy, is a unit, and everything a unit
can do on its turn is an ability: Attack, Quake, a Djinni, a summon, using an
item, an enemy's special move.

Party members keep their character numbers as unit ids, and enemies take ids
from `FIRST_ENEMY_UNIT`. `Unit_GetFar` returns a unit's `struct BattleUnit`,
with its HP, PP, stats, statuses and a Power and a Resist in each element.

An ability's id is the same row in three tables, each saying one thing about
it:

| Table | Read through | What its row says |
| --- | --- | --- |
| `src/game/actions.tsv` | `struct BattleAct`, `BattleAction_Get` | what the ability does: its targets, element, range, PP cost, power, added effect and ability kind |
| `src/battle/actflags.tsv` | `gBattle_ActionFlags` | the effect that draws it, and how it is staged |
| `src/battle/actstat.tsv` | `gBattle_ActionStatus` | how the battle shows it, where it overrides the default |

Quake is row 3. Its row of `actions.tsv` has element `ELEMENT_VENUS`, range 2,
a PP cost of 4 and a power of 12. Its `flags` column holds two things at
once: `0x85` is `0x80`, `ACTION_USABLE_IN_BATTLE`, plus 5,
`ABILITY_KIND_DAMAGE`. The ability kind, the low four bits of `flags`
(`ABILITY_KIND_MASK`), chooses the formula the targets go through. The effect kind in `actflags.tsv` is an unrelated number
that chooses only the animation, so the arithmetic and the picture can be
changed independently.

| Name | Value | Meaning |
| --- | --- | --- |
| `FIRST_ENEMY_UNIT` | 128 | the first enemy's unit id |
| `PARTY_BATTLE_SIZE` | 4 | party members who fight at once |
| `ENEMY_BATTLE_SIZE` | 6 | enemies on the field at most |
| `ABILITY_KIND_MASK` | 15 | the bits of an ability's `flags` that are its kind |

[Game rules](src/game/README.md#how-one-number-finds-a-row-and-its-name)
has the tables and [the unit stats they feed](src/game/README.md#why-stats-are-always-worked-out-from-scratch), and [Battles](src/battle/README.md#the-ability-kind) every
ability kind.

## Rounds and turns

The picture at the top of this page is one battle: rounds of choosing,
sorting and playing actions, until one side is beaten.

A round has five steps, each in its own part of `src/battle/battle/`:

1. `round.c` gathers every unit's action: the party's from the command menu,
   the enemies' from `BattleCommand_SelectAutomatic` (`enemy_ai.c`), or from
   the other console in a link battle, and from `auto_battle.c` under
   `FLAG_AUTO_BATTLE`. `BattleQueue_SortByPriority` sorts them by Agility,
   fastest first, except that Granite, Flash and Ground go before everyone.
2. `BattleCommand_BuildPlan` (`plan.c`) turns each action into a
   `struct BattlePlan`: the ability id, the targets, double blows and
   critical hits rolled in advance, and the staging.
3. `BattlePresentation_DispatchAction` (`staging.c`) stages the plan and runs
   its effect.
4. `BattleTarget_Execute` (`target_exec.c`) applies the ability to each target.
5. `turn_end.c` counts down stat changes and statuses, recovers Djinn and
   applies poison and the Grim Reaper's countdown.

After every action the battle checks both sides, so it can end in the middle
of a round. A plan reaches at most `TARGET_MAX` (14) targets.
[Battles](src/battle/README.md#rounds) goes through each step, with
[who acts first](src/battle/README.md#rounds),
[enemy action selection](src/battle/README.md#enemy-actions),
[first strikes and escape](src/battle/README.md#first-strikes-and-escape)
and [planning an action](src/battle/README.md#planning-an-action).

## Resolving an ability

When Avalanche hits three enemies, the one aimed at takes the most and the
ones beside it less, and an enemy weak to Venus takes more than one that
resists it. Every damaging Psynergy works out its number the same way, in a
few plain steps.

![How Psynergy damage is worked out, for Isaac's Avalanche at three enemies in a row, aimed at the first. Avalanche's power is 160. Isaac's Venus Power is 20 ahead of each target's Venus Resist, which multiplies it by about 1.1, to 175. The distance keeps 100 percent for the aimed enemy, 80 one place away and 60 two away: 175, 140 and 105. Last, 0 to 3 is added at random: 175 to 178, 140 to 143 and 105 to 108. A defending target takes half, one behind Total Defense a tenth. Every step rounds down.](https://alchemy.pascalpixel.com/illustrations/battle/damage.svg)

An ability's power is the damage the aimed target takes at an even match,
when the caster's Power in the ability's element equals the target's Resist
in it; Avalanche's is 160. Two things scale it, a little luck is added, and
a guard can cut the result:

- The element. Each point the caster's Power is ahead of the target's Resist
  adds half a percent, up to double; each point behind takes half a percent
  off, down to nothing. Isaac's Venus Power 20 ahead of each target's Venus
  Resist makes Avalanche's 160 about 1.1 times as much, 175.
- The distance. A range counts places in the target side's line-up, and each
  step from the aimed place keeps a smaller share: 80 percent one step away
  and 60 two steps away for Psynergy damage, so 175, 140 and 105. Summons and
  the enemies' unleashed moves have tables of their own.
- The luck. A random 0 to 3 is added, so the three take 175 to 178, 140 to
  143 and 105 to 108.
- The guard. A target guarding this round, by Defend or the Djinni Granite,
  takes half of that, and one behind Total Defense or Flash a tenth, so the
  aimed enemy, defending, would take 87 to 89.

Every step rounds down.

In the code, `BattleTarget_Execute` rolls the added effect (the extra a
move may bring besides damage, such as Sleep; `Battle_RollEffectSuccessFar`),
then switches on the ability kind to choose the formula, then applies the
added effect if it landed. The formulas are in
`src/game/game/battle_calc.c`, and the battle calls them by their `Far`
names:
`Battle_CalculateAttackDamageFar` for attacks,
`Battle_CalculatePsynergyPowerFar` for Psynergy damage,
`Battle_CalculateRecoveryFar` for healing. `Battle_GetElementMultiplier`
gives the element's share, counted in 256ths (`FULL_RATE`): Avalanche's 20
points ahead give 281 in 256ths. The fall-off tables are in
`src/battle/battle_calc.s`, and the guard is `REDUCE_BY_GUARD`.

| Name | Value | Meaning |
| --- | --- | --- |
| `ABILITY_KIND_DAMAGE` | 5 | HP damage from the power: Psynergy |
| `ABILITY_KIND_SUMMON` | 8 | a summon's damage |
| `FULL_RATE` | 256 | a multiplier of 1, in 256ths |

[Battles](src/battle/README.md#how-an-ability-resolves) has every ability
kind's formula, the fall-off tables and the added effects, worked through for
Avalanche, Cure and Ragnarok.

## The battle's picture

The battlefield the player sees tilts away into the distance and turns as
the camera swings round, while the status window and the messages stay flat
in front. That takes three of the console's background layers and its
sprites, each with its own job.

![A battle pulled apart into its layers. A battle in Goma Cave as it opens comes apart into the sheets the console stacks to draw it, then closes again. From the back: the cave tilted into a floor on `BG2`; the party and the enemies, sprites; the status window and the message on `BG0`; and in front the arrow that waits for a button, a sprite. `BG1` draws nothing here.](https://alchemy.pascalpixel.com/illustrations/battle/battle-layers.svg)

The console draws the screen one line at a time, top to bottom, and can
change a layer's settings in the short pause after each line. The battle
uses that pause to tilt a flat picture into a floor. The battlefield is
`BG2`, the layer the hardware can stretch and turn (an affine layer), shown
in perspective (`BATTLE_BACKGROUND_PLANE`): `BattleCamera_SetRange` works out
a page of settings, one for each line, and an HBlank DMA (a copy the
hardware makes by itself in the pause after each line) gives every line from
0 to 135 its own `BG2CNT` and scale, so the plane stops above the status
window and turns as the camera turns. Outside the time commands are chosen
(`FLAG_CHOOSING_COMMANDS`), its top 16 lines are left off. `BG1` holds the
same picture flat. An effect that needs `BG2` for its canvas switches the
background to that flat picture (`BATTLE_BACKGROUND_FLAT`), whose horizontal
scroll follows the camera. `BG0` holds the windows, and the units are
sprites. The picture above takes these layers apart.

| Layer | What it holds |
| --- | --- |
| `BG0` | the windows: status and messages |
| `BG1` | the battlefield flat (`BATTLE_BACKGROUND_FLAT`) |
| `BG2` | the battlefield in perspective (`BATTLE_BACKGROUND_PLANE`), or an effect's canvas |
| sprites | the units |

[Battle effects](src/effect/README.md#the-canvas) describes how an effect
takes `BG2` for its canvas.

## Effects

Every ability's animation, from a sword swing to Judgment rising out of the
clouds, is an effect: one function that draws it frame by frame and returns
when it is over, while the battle waits.

`BattlePresentation_DispatchAction` starts the effect through one of three
entry points, chosen by bits of the plan's `presentationFlags`, which are the
ability's `actflags.tsv` row:

| Entry point | Chosen by | What it runs |
| --- | --- | --- |
| `BattleEffect_RunTableEffect` | neither bit | an ordinary effect kind, an index into `gBattleEffect_EffectHandlers` |
| `BattleEffect_DispatchByIdRange` | `STAGING_BY_ID_RANGE` (`0x4000`) | casting impacts, beam sequences and ring and spark scenes, by the effect kind's range |
| `BattleEffect_RunSummonEffect` | `STAGING_SUMMON` (`0x8000`) | the summons' scenes |

Each entry point borrows three blocks of memory from the heap (the sprite
sheet, the work block and the canvas), runs the effect to its end and gives
them back in reverse order, since giving a heap block back also frees
everything borrowed after it. An effect must therefore stop every callback
(a function the scheduler calls once a frame) that reads those blocks
before it returns.

Most effects draw into a canvas, a hidden picture of 128 by 128 pixels, one
byte each, which `BG2` shows blended over the battle and stretched to twice
its width (`BattleEffect_StartCanvasLayer`). An effect stamps pictures onto
the canvas with blitters, small drawing routines that
`BattleEffect_BuildBlitter` writes while the game runs, each for one
combination of size, mirroring, clipping and blend mode, and moves particles
(sparks, embers, chips of rock) over it. It never writes video memory
itself: it sets `transferPending` and waits, and the callback
`BattleEffect_ProcessPendingGraphicsTransfer` copies the canvas and then fades,
clears or keeps the pixels left in it as the work block's transfer mode says.
A canvas pixel holds not a colour but a level, a step on a ramp of colours
from nothing to the brightest, so lowering every level a little each frame
makes trails die away without keeping track of each particle.

![Isaac unleashes Flint at Ivan, and sparks close in on a point above Isaac while he hangs in mid-leap, a streak of light shoots down at Ivan, and a ring of light and a spray of sparks burst over him.](https://alchemy.pascalpixel.com/abilities/flint.gif)

Flint is a casting impact, reached through `BattleEffect_DispatchByIdRange`
(`src/effect/parts/casting_impact.c`): 64 sparks gather on the caster, `BG2`
is moved so the impact sits over the target, and 32 scattering sparks follow
the impact.

Every effect runs a fixed number of frames, and within a frame its steps run
in a fixed order. The order is part of the effect's output: later steps draw
over earlier ones, and many steps draw random numbers, so moving one changes
every random value after it.

The twelve summons that need two or more Djinn are scenes, enumerated by
`enum SummonEffectKind`. They take the same three blocks; unlike
`BattleEffect_RunTableEffect`, `BattleEffect_RunSummonEffect` leaves setting
`work->effect` to each scene. The four summons that need one Djinni, Venus,
Mercury, Mars and Jupiter, are ordinary effect kinds. A summon's damage
(`ABILITY_KIND_SUMMON`) also adds 3 percent of the target's maximum HP to the
power for each Djinni it uses.

![Isaac summons Judgment: the screen goes dark, Judgment rises out of the clouds and strikes, and the scene clears to a wide landscape](https://alchemy.pascalpixel.com/abilities/judgment.gif)

`BattleEffect_RunJudgment` (`src/effect/parts/judgment.c`) replaces the battle's
display with backdrops of its own and runs in two parts, each with its own
frame clock (`JUDGMENT_GATHER_FRAMES`, `JUDGMENT_SWEEP_FRAMES`). The battle
comes back near the end of the second part, and the damage is shown after
it has.

[Battle effects](src/effect/README.md#how-a-move-picks-its-effect) describes
how the entry point is chosen, then [the canvas](src/effect/README.md#the-canvas),
[the blitters](src/effect/README.md#blitters),
[the particle pools](src/effect/README.md#particles),
[casting impacts](src/effect/README.md#casting-impacts),
[Judgment](src/effect/README.md#judgment) and every
[effect family](src/effect/README.md#effect-families).

## Rules and effects run together

In a battle the damage messages appear while the rock is still rising: the
animation does not have to finish before the numbers show. That is because
the animation and the damage run side by side, and only meet at the moment
the blow lands.

![The animation and the damage, side by side, with time running down the page. On the left the animation runs in its own loop: Quake's rock rises frame after frame. On the right the damage task waits. When the blow lands the animation says so, and from then on both run at once: the animation keeps drawing to its last frame while the task works out the first target's damage and shows its messages, then the next target's, then the last. The battle moves on only when both are done. The animation picks when the damage appears; the rules alone decide how much.](https://alchemy.pascalpixel.com/illustrations/battle/side-by-side.svg)

An effect is an ordinary function with a loop of its own: it calls
`WaitFrames` whenever time has to pass and returns only when the animation is
over. The battle code that called it cannot run until then, so the damage
has to come from somewhere else: a task, a small job that the scheduler,
the engine's list of jobs, runs once a frame alongside the effect.

Before it starts the effect, `BattlePresentation_DispatchAction` schedules
`BattleEvent_Playback` (`src/battle/battle/playback.c`) with
`Scheduler_AddOrUpdateCallback`, and after the effect returns it waits in
`BattleEvent_WaitForReady` until playback is done. Playback stays idle until
the effect calls `EFFECT_SHOW_DAMAGE(hitSound)`, which most effects do on the
frame the blow lands; if an effect never calls it, the wait after the effect
starts playback instead. From then on, once a frame, the task runs
`BattleTarget_Execute` for the next target and plays that target's events
over the following frames before it moves on.

![Isaac casts Quake: the rock rises under the party, and the damage messages appear while it's still on screen, the aimed member taking more than the one beside him](https://alchemy.pascalpixel.com/abilities/quake.gif)

The two sides meet at the battle event queue (`struct BattleEventQueue`,
`BATTLE_EVENT_CAPACITY` events). `BattleTarget_Execute` never draws or waits: it
changes the target's HP and statuses and pushes events with `BattleEvent_Push` in
the order the player sees them. The name and number a message fills its
blanks with go before the message, since the message takes them from the
window system's argument table as it is written out. `BattleEvent_DispatchQueued` plays the same queue straight
through, for outcomes shown outside an effect, such as a cast that is sealed
or short of PP.

The invariant is that the effect decides only when the damage appears, and
the rules alone decide how much it is. A later target's arithmetic runs while
the effect is still drawing. XXX: what starts playback for the effect parts
that never call `EFFECT_SHOW_DAMAGE`, beyond the wait after the effect, is not
known.

| Name | Value | Meaning |
| --- | --- | --- |
| `BATTLE_EVENT_CAPACITY` | 64 | events the battle event queue holds |

[Battles](src/battle/README.md#rules-and-pictures-meet-at-the-event-queue)
takes one target's queue apart, event by event.

## Limits

- There are 519 abilities, ids 0 to 518; `actstat.tsv` is one row shorter.
- A plan reaches at most `TARGET_MAX` (14) targets.
- The party fights four at a time and the enemies six at most
  (`PARTY_BATTLE_SIZE`, `ENEMY_BATTLE_SIZE`).
- `formatn.tsv` has 380 enemy groups, each of up to five kinds of enemy.
- The event queue holds `BATTLE_EVENT_CAPACITY` (64) events.
- The canvas is 128 by 128 pixels, the work block holds 64 particles, and
  there are two blitter slots.
