# Battle

When a random battle starts, the field gives way to a battlefield where the
[party](glossary.md#the-party) faces the enemies. From then on the battle runs in
rounds. At the start of a round the player picks a command for each party
member and every enemy picks one of its own. The game lines the actions up,
fastest first, and plays them one after another: an announcement such as
`Isaac casts Quake!`, the animation, and the damage and messages for each
target. When one side has nobody left standing, or the party runs away, the
battle ends and the field comes back.

Two kinds of code share the work. The rules decide what happens: who acts
when, who is hit and for how much, which statuses take hold. The animations
draw it: the rock that rises, the flash of a critical hit, a [summon](glossary.md#summons)'s whole
scene. They are kept apart so cleanly that a later target's damage is worked
out while the animation is still on screen. This page explains the battle as
a whole; [Battles](src/battle/README.md) goes through the rules in detail,
[Game rules](src/game/README.md) holds the tables and the arithmetic they
use, and [Battle animations](src/battle_anim/README.md) draws them.

## Starting a battle

To the rest of the game a battle is one more [map](glossary.md#map): the field leaves for it
as it would leave through a doorway, and comes back to the same place
afterwards.

In the code, a random encounter raises a field [warp](glossary.md#warp) like any other, with
`RUN_BATTLE` as the next map and the formation in
`gSaveBlock.warpDest`. `Game_Run` (`src/overworld/overworld/map.c`) clears what
the last map left running, as it does between any two maps: the [task](glossary.md#task)
table, the list of small jobs the game runs once a [frame](glossary.md#frame), and the [heap](glossary.md#heap-and-pools), the
pool of memory a map borrows from. Then, instead of loading a map's
[overlay](glossary.md#overlay) (the code for one map, loaded when the party arrives there), it
calls the battle, which fights the whole battle before it returns.

The battle's code is not an overlay: it is always present, in the `battle`
[bank](glossary.md#bank) of the [main image](glossary.md#main-image). Code
in another bank calls it through a [far call](glossary.md#far-call), so the
field calls `Battle_RunFar(warpDest)`, the entry that leads to
`Battle_Run`. Afterwards `Map_ReturnFromRun` reloads the field
map the battle started from, with the party where it was, or after a defeat
goes to the defeat point, the map the defeat rules in
`src/overworld/map_rules.s` give for the place the party fell.
The debug battle menu (`DebugBattle_RunMenu`) is the only other way in.

`Battle_Run` (`src/battle/battle/encounter.c`) allocates the
battle's [heap slots](glossary.md#heap-slot), `gBattleWork` among them, and sets `FLAG_IN_BATTLE`
while it runs. Its result is the number of enemies defeated,
`BATTLE_OUTCOME_FLED` or `BATTLE_OUTCOME_LOST`. The formation is a row of
`src/battle/formations.c`, the enemy formations, which
`src/battle/battle/formation.c` turns into enemy units; the encounter zones
in `src/overworld/encounter_zones.c` choose the row.

| Name | Value | Meaning |
| --- | --- | --- |
| `RUN_BATTLE` | `0x1fe` | the next map that runs a battle instead of an overlay |
| `FLAG_IN_BATTLE` | `0x103` | set while a battle runs |
| `BATTLE_OUTCOME_FLED` | 999 | `Battle_Run`'s result when the party got away |
| `BATTLE_OUTCOME_LOST` | -1 | its result when the party was defeated |

[Running a map](src/overworld/README.md#random-battles) covers how the field
rolls the encounter, and [Battles](src/battle/README.md#rounds) what
`Battle_Run` does from the opening to the spoils.

## Units and abilities

Everyone in a fight, party member or enemy, is a unit, and everything a unit
can do on its turn is an ability: Attack, Quake, a [Djinni](glossary.md#djinn), a summon, using an
item, an enemy's special move.

Party members keep their character numbers as unit ids, and enemies take ids
from `FIRST_ENEMY_UNIT`. `Unit_GetFar` returns a unit's `struct Unit`,
with its HP, [PP](glossary.md#psynergy-and-pp), stats, statuses and a [Power](glossary.md#power-and-resist) and a Resist in each [element](glossary.md#the-four-elements).

An ability's id is the same row in three tables, each saying one thing about
it:

| Table | Read through | What its row says |
| --- | --- | --- |
| `src/game/abilities.c` | `struct AbilityDefinition`, `Ability_Get` | what the ability does: its targets, element, range, PP cost, power, added effect and ability kind |
| `src/battle/ability_staging.c` | `gAbilityAnimations` | the animation that draws it (its animation kind) and how strong a version |
| `src/battle/ability_staging.c` | `gAbilityStaging` | what happens around the animation, where the default won't do |

Quake is row 3. Its row of `abilities.c` has element `ELEMENT_VENUS`, range 2,
a PP cost of 4 and a power of 12. Its `flags` column holds two things at
once:

```
0x85 = ABILITY_USABLE_IN_BATTLE (0x80) | ABILITY_FORMULA_DAMAGE (5)
```

The ability kind, the low four bits of `flags` (`ABILITY_FORMULA_MASK`),
chooses the formula the targets go through. The animation kind in
`gAbilityAnimations` is an unrelated number that chooses only the animation, so
the arithmetic and the picture can be changed independently.

`gAbilityStaging` is 0 for most abilities, and the battle then picks the
staging itself: a cast for Psynergy or anything else with an animation, and
only the messages for an ability with none. Its 86 other rows choose the
staging: Terra Strike's, `STAGING_ATTACK`, has the actor run up and
strike, as a plain Attack does, and Avalanche's, `STAGING_FULL_SCREEN`,
turns the camera and fades out the side that isn't targeted while the rock
falls. Thrash, the last ability, has no row, so it always takes the default.

| Name | Value | Meaning |
| --- | --- | --- |
| `FIRST_ENEMY_UNIT` | 128 | the first enemy's unit id |
| `PARTY_BATTLE_SIZE` | 4 | party members who fight at once |
| `ENEMY_BATTLE_SIZE` | 6 | enemies on the field at most |
| `ABILITY_FORMULA_MASK` | 15 | the bits of an ability's `flags` that are its kind |
| `ABILITY_USABLE_IN_BATTLE` | `0x80` | the bit of `flags` that lets the ability be chosen in battle |

[Game rules](src/game/README.md#how-one-number-finds-a-row-and-its-name)
has the tables and [the unit stats they feed](src/game/README.md#why-stats-are-always-worked-out-from-scratch), and [Battles](src/battle/README.md#the-ability-kind) every
ability kind.

## Rounds and turns

A battle is rounds of choosing, sorting and playing actions, until one side
is beaten.

The work runs at two levels: once a round, and once for each action in it.
Once a round, `round.c` gathers every unit's action and sorts them:

- The party's come from the command menu, or from `auto_battle.c` under
  `FLAG_AUTO_BATTLE`.
- The enemies' come from `BattleAI_ChooseAction` (`enemy_ai.c`), or
  from the other console in a link battle.
- `BattleTurnOrder_Sort` sorts them by Agility, fastest first, except
  that three Djinn that guard or hold an enemy, Granite, Flash and Ground,
  go before everyone.

Then, for each action in turn, `BattleStaging_DispatchAction`
(`staging.c`):

1. builds its turn: `BattleTurn_Build` (`turn.c`) turns the action into a
   `struct BattleTurn`, with the ability id, the targets, double blows and
   critical hits rolled in advance, and the staging;
2. stages the turn and runs its animation, during which
   `BattleAbility_ApplyToTarget` (`target_exec.c`) applies the ability to each
   target;
3. ends the actor's turn (`BattleUnit_ProcessTurnEnd`, `turn_end.c`): its
   summon's boost, poison, and the Grim Reaper's countdown, the turns left
   to a unit that Curse has struck.

After every action the battle checks both sides, so it can end in the middle
of a round. When the round is over, `Battle_ProcessRoundEnd` (`turn_end.c`)
recovers Djinn and counts down stat changes and statuses. In the code, the
loop in `Battle_Run` (`encounter.c`) reads, simplified:

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
[who acts first](src/battle/README.md#rounds),
[enemy action selection](src/battle/README.md#enemy-actions),
[first strikes and fleeing](src/battle/README.md#first-strikes-and-fleeing)
and [building a turn](src/battle/README.md#building-a-turn).

## Resolving an ability

When Avalanche hits three enemies, the one aimed at takes the most and the
ones beside it less, and an enemy weak to Venus takes more than one that
resists it. Every damaging Psynergy works out its number the same way, in a
few plain steps.

An ability's power is the damage the aimed target takes at an even match,
when the caster's Power in the ability's element equals the target's Resist
in it; Avalanche's is 160. Two things scale it, a little luck is added, and
a guard can cut the result:

- The element. Each point the caster's Power is ahead of the target's Resist
  adds half a percent, up to double; each point behind takes half a percent
  off, down to nothing. The multiplier is counted in 256ths, so Isaac's
  Venus Power 20 ahead of each target's Venus Resist makes 281 in 256, and
  Avalanche's 160 becomes 160 × 281 / 256 = 175, rounded down.
- The distance. A range counts places in the target side's line-up, and each
  step from the aimed place keeps a smaller share: 80 percent one step away
  and 60 two steps away for Psynergy damage, so 175, 140 and 105. Summons and
  the enemies' unleashed moves have tables of their own.
- The luck. A random 0 to 3 is added, so the three take 175 to 178, 140 to
  143 and 105 to 108.
- The guard. A target guarding this round, by Defend or the Djinni Granite,
  takes half of that, so the aimed enemy, defending, would take 87 to 89. A
  target guarded by Total Defense, an enemy's stronger guard, or by the
  Djinni Flash takes a tenth.

Every step rounds down.

In the code, `BattleAbility_ApplyToTarget` rolls the added effect (the extra a
move may bring besides damage, such as Sleep; `Battle_RollEffectSuccessFar`),
then switches on the ability kind to choose the formula, then applies the
added effect if it landed. The formulas are in
`src/game/game/battle_calc.c`, in the `game` bank, so the battle calls them
by their far-call names: `Battle_CalculateAttackDamageFar` for attacks,
`Battle_CalculatePsynergyPowerFar` for Psynergy damage,
`Battle_CalculateRecoveryFar` for healing. `Battle_GetElementMultiplier`
gives the element's share in 256ths (`FULL_RATE`). The fall-off tables are in
`src/battle/battle_calc.s`, and the guard is `REDUCE_BY_GUARD`.

| Name | Value | Meaning |
| --- | --- | --- |
| `ABILITY_FORMULA_DAMAGE` | 5 | HP damage from the power: Psynergy |
| `ABILITY_FORMULA_SUMMON` | 8 | a summon's damage |
| `FULL_RATE` | 256 | a multiplier of 1, in 256ths |

[Battles](src/battle/README.md#how-an-ability-resolves) has every ability
kind's formula, the fall-off tables and the added effects, worked through for
Avalanche, Cure and Ragnarok.

## The battle's picture

The battlefield the player sees stretches away into the distance and slides
sideways as the camera swings round, while the status window and the
messages stay flat in front. That takes three of the console's [background layers](glossary.md#background-layer) and its
[sprites](glossary.md#sprite-and-oam), each with its own job.

| Layer | What it holds |
| --- | --- |
| `BG0` | the windows: status and messages |
| `BG1` | the battlefield, at a fixed size (`BATTLE_BACKGROUND_FIXED`) |
| `BG2` | the battlefield, zoomed with the camera (`BATTLE_BACKGROUND_ZOOMED`), or an animation's canvas |
| sprites | the units |

The battlefield is one picture of the place, 256 by 120 pixels, and its
depth is drawn into it. The hardware only zooms and pans it. It sits on
`BG2`, the layer the hardware can scale and turn (an affine layer), and
`BattleCamera_SetRange` sets that layer's zoom from the camera's distance
and its sideways scroll from the camera's turn, once a frame. The code calls
this the perspective plane, though nothing in it is drawn in perspective by
the hardware.

The console draws the screen one line at a time and can change a layer's
settings in the short pause after each line, the horizontal blank. The
battle uses that pause for a different reason: an affine layer's map names
each tile with one byte, so it can show only 256 tiles, and the picture has
480. The map therefore numbers the top 64 pixels' tiles from character block
2 and the rest again from character block 3, and an HBlank DMA (a copy the
hardware makes by itself after each line) writes each line's `BG2CNT`, the
layer's control register, to say which block that line reads:

| Lines | `BG2CNT` |
| --- | --- |
| 0 to 15 | off, except while commands are chosen (`FLAG_CHOOSING_COMMANDS`) |
| from there to the line where the picture's lower part starts at this zoom | the upper part, character block 2 |
| from there to 135 | the lower part, character block 3 |
| 136 to 159 | off: the status window covers them |

`BattleCamera_SetRange` builds these 160 values into one page while the other
is shown, and the two pages swap each frame.

`BG1` holds the same picture at a fixed size. An animation that needs `BG2` for
its canvas switches the background to it (`BATTLE_BACKGROUND_FIXED`), and its
horizontal scroll still follows the camera.

[Battle animations](src/battle_anim/README.md#the-canvas) describes how an animation
takes `BG2` for its canvas.

## Animations

Every ability's animation, from a sword swing to Judgment rising out of the
clouds, is one function that draws it frame by frame and returns
when it is over, while the battle waits.

`BattleStaging_DispatchAction` starts the animation through one of three
entry points, chosen by bits of the turn's `animation`, which are the
ability's `gAbilityAnimations` row:

| Entry point | Chosen by | What it runs |
| --- | --- | --- |
| `BattleAnim_RunTableEffect` | neither bit | an ordinary animation kind, an index into `gBattleAnim_Handlers` |
| `BattleAnim_DispatchByIdRange` | `ANIM_BY_ID_RANGE` (`0x4000`) | by the animation kind's value: 100 to 135 a casting impact, 200 and up a ring and spark scene, any other a beam sequence |
| `BattleAnim_RunSummonEffect` | `ANIM_SUMMON` (`0x8000`) | the summons' scenes |

No row sets both bits.

Each entry point borrows three blocks of memory from the heap (the sprite
sheet, the work block and the canvas), runs the animation to its end and gives
them back in reverse order, since giving a heap block back also frees
everything borrowed after it. An animation must therefore remove every task
that reads those blocks before it returns.

Most animations draw into a canvas, a hidden picture of 128 by 128 pixels, one
byte each, which `BG2` shows blended over the battle and stretched to twice
its width (`BattleAnim_StartCanvasLayer`). An animation stamps pictures onto
the canvas with blitters, small drawing routines that
`BattleAnim_BuildBlitter` writes while the game runs, each for one
combination of size, mirroring, clipping and blend mode, and moves particles
(sparks, embers, chips of rock) over it. It never writes video memory
itself: it sets `transferPending` and waits, and the callback
`BattleAnim_ProcessPendingGraphicsTransfer` copies the canvas and then fades,
clears or keeps the pixels left in it as the work block's transfer mode says.
A canvas pixel holds not a colour but a level, a step on a ramp of colours
from nothing to the brightest, so lowering every level a little each frame
makes trails die away without keeping track of each particle.

![Isaac unleashes Flint at Ivan, and sparks close in on a point above Isaac while he hangs in mid-leap, a streak of light shoots down at Ivan, and a ring of light and a spray of sparks burst over him.](https://alchemy.pascalpixel.com/abilities/flint.gif)

Flint is a casting impact, reached through `BattleAnim_DispatchByIdRange`
(`src/battle_anim/parts/casting_impact.c`): 64 sparks gather on the caster, `BG2`
is moved so the impact sits over the target, and 32 scattering sparks follow
the impact.

Every animation runs a fixed number of frames, and within a frame its steps run
in a fixed order. The order is part of the animation's output: later steps draw
over earlier ones, and many steps draw random numbers, so moving one changes
every random value after it.

The twelve summons that need two or more Djinn are scenes, enumerated by
`enum SummonAnimId`, and take the same three blocks. The four summons
that need one Djinni, Venus, Mercury, Mars and Jupiter, are ordinary animation
kinds.

![Isaac summons Judgment: the screen goes dark, Judgment rises out of the clouds and strikes, and the scene clears to a wide landscape](https://alchemy.pascalpixel.com/abilities/judgment.gif)

`BattleAnim_RunJudgment` (`src/battle_anim/parts/judgment.c`) replaces the battle's
picture with backgrounds of its own and runs in two parts, each with its own
frame clock (`JUDGMENT_GATHER_FRAMES`, `JUDGMENT_SWEEP_FRAMES`). The battle
comes back near the end of the second part, and the damage is shown after
it has.

[Battle animations](src/battle_anim/README.md#how-a-move-picks-its-animation) describes
how the entry point is chosen, then [the canvas](src/battle_anim/README.md#the-canvas),
[the blitters](src/battle_anim/README.md#blitters),
[the particle pools](src/battle_anim/README.md#particles),
[casting impacts](src/battle_anim/README.md#casting-impacts),
[Judgment](src/battle_anim/README.md#judgment) and every
[animation family](src/battle_anim/README.md#animation-families).

## Rules and animations run together

In a battle the damage messages appear while the rock is still rising: the
animation does not have to finish before the numbers show. That is because
the animation and the damage run side by side, and only meet at the moment
the blow lands.

An animation is an ordinary function with a loop of its own: it calls
`WaitFrames` whenever time has to pass and returns only when the animation is
over. The battle code that called it cannot run until then, so the damage
has to come from somewhere else: a [task](glossary.md#task), which
`WaitFrames` itself runs once a frame while the animation waits
([Engine](engine.md#waiting-and-tasks) explains how).

The staging runners in `src/battle/battle/staging.c` all follow the same
pattern; `BattleStaging_RunPsynergyCast`, simplified:

```c
Task_Add(BattlePlayback_Run, 0xc80); /* idle task */
if (plan->animation & ANIM_BY_ID_RANGE)
  BattleAnim_DispatchByIdRangeFar(&work.effect); /* the animation's loop */
else
  BattleAnim_RunTableEffectFar(&work.effect);
BattlePlayback_WaitForReady(); /* until every target has played */
```

Playback (`src/battle/battle/playback.c`) stays idle until the animation calls
`ANIM_SHOW_DAMAGE(hitSound)`, which most animations do on the frame the blow
lands. From then on, once a frame, the task runs `BattleAbility_ApplyToTarget` for
the next target and plays that target's entries over the following frames
before it moves on. An animation that never calls it, such as Atalanta's or
Torch's, leaves playback idle until it returns, and
`BattlePlayback_WaitForReady` then starts playback itself, so its damage shows
after the animation.

![Isaac casts Quake: the rock rises under the party, and the damage messages appear while it's still on screen, the aimed member taking more than the one beside him](https://alchemy.pascalpixel.com/abilities/quake.gif)

The two sides meet at the playback queue (`struct BattlePlaybackQueue`,
`BATTLE_PLAYBACK_CAPACITY` entries). `BattleAbility_ApplyToTarget` never draws or waits:
it changes the target's HP and statuses and pushes entries with
`BattlePlayback_Push` in the order the player sees them. A message's blanks,
such as the name and the number in `Garet takes 14 damage!!!`, are filled
from values pushed before it, since the message reads them as it is written
out. `BattlePlayback_RunAll` plays the same queue straight through, for
outcomes shown outside an animation, such as a cast that is sealed or short of
PP.

The invariant is that the animation decides only when the damage appears, and
the rules alone decide how much it is. A later target's arithmetic runs while
the animation is still drawing.

| Name | Value | Meaning |
| --- | --- | --- |
| `BATTLE_PLAYBACK_CAPACITY` | 64 | entries the playback queue holds |

[Battles](src/battle/README.md#rules-and-pictures-meet-at-the-playback-queue)
takes one target's queue apart, entry by entry.

## Limits

- There are 519 abilities, ids 0 to 518; `gAbilityStaging` is one row shorter.
- A turn reaches at most `TARGET_MAX` (14) targets.
- The party fights four at a time and the enemies six at most
  (`PARTY_BATTLE_SIZE`, `ENEMY_BATTLE_SIZE`).
- `formations.c` has 380 formations, each of up to five kinds of enemy.
- The playback queue holds `BATTLE_PLAYBACK_CAPACITY` (64) entries; `BattlePlayback_Push`
  doesn't check it.
- The canvas is 128 by 128 pixels, the work block holds 64 particles, and
  there are two blitter slots.
