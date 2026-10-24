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

To the rest of the game a battle is one more [map](glossary.md#map). The
field leaves for it as it would leave through a doorway, and when the battle
is won it comes back to the same spot. After a defeat it goes to the place the
game sends a beaten party instead.

A random encounter raises a field [warp](glossary.md#warp) like any other,
with `RUN_BATTLE` as the next map and the formation in `gSaveBlock.warpDest`.
`Game_Run` (`src/overworld/overworld/map.c`) clears the
[task](glossary.md#task) table and the [heap](glossary.md#heap-and-pools), as
it does between any two maps, and then, instead of loading a map's
[overlay](glossary.md#overlay), calls `Battle_RunFar(warpDest)`, a
[far call](glossary.md#far-call) into the `battle`
[bank](glossary.md#bank) of the [main image](glossary.md#main-image). That
leads to `Battle_Run` (`src/battle/battle/encounter.c`), which fights the whole
battle before it returns. `Map_ReturnFromRun` then reloads the field map, or
after a defeat the map the defeat rules in `src/overworld/map_rules.s` give.
The debug battle menu (`DebugBattle_RunMenu`) is the only other way in.

`Battle_Run` takes the battle's [heap slots](glossary.md#heap-slot),
`gBattleWork` among them, and sets `FLAG_IN_BATTLE` while it runs. The
formation is a row of `src/battle/formations.c`, chosen by the encounter
zones in `src/overworld/encounter_zones.c`.

| Name | Value | Meaning |
| --- | --- | --- |
| `RUN_BATTLE` | `0x1fe` | the next map that runs a battle instead of an overlay |
| `FLAG_IN_BATTLE` | `0x103` | set while a battle runs |
| `BATTLE_OUTCOME_FLED` | 999 | `Battle_Run`'s result when the party got away |
| `BATTLE_OUTCOME_LOST` | -1 | its result when the party was defeated; otherwise the enemies defeated |

[Running a map](src/overworld/README.md#random-battles) covers how the field
rolls the encounter, and [Battles](src/battle/README.md#rounds) what
`Battle_Run` does from the opening to the spoils.

## Units and abilities

![At the start of a round Isaac opens his Psynergy, picks Quake and aims it at the Harridan; Garet looks through his Djinn and backs out to attack; Ivan defends and Mia attacks. The round begins with `Ivan is defending!` and `Isaac casts Quake!`.](https://alchemy.pascalpixel.com/guides/battle/commands.gif)

Everyone in a fight, party member or enemy, is a unit, and everything a unit
can do on its turn is an ability: Attack, Quake, a [Djinni](glossary.md#djinn),
a [summon](glossary.md#summons), using an item, an enemy's special move. The
command menu is how the player chooses one for each party member.

A unit is a `struct Unit` (`Unit_GetFar`), with its HP,
[PP](glossary.md#psynergy-and-pp), stats, statuses and a
[Power and a Resist](glossary.md#power-and-resist) in each
[element](glossary.md#the-four-elements). Party members keep their character
numbers as unit ids; enemies take ids from `FIRST_ENEMY_UNIT`.

An ability's id is a row in three tables, each saying one thing about it:

| Table | Read through | What its row says |
| --- | --- | --- |
| `src/game/abilities.c` | `struct AbilityDefinition`, `Ability_Get` | its targets, element, range, PP cost, power, added effect and ability kind |
| `src/battle/ability_staging.c` | `gAbilityAnimations` | the animation that draws it and how strong a version |
| `src/battle/ability_staging.c` | `gAbilityStaging` | what happens around the animation, where the default won't do |

The ability kind, the low four bits of the `flags` column
(`ABILITY_FORMULA_MASK`), chooses the formula the targets go through. Quake's
`flags` are `ABILITY_USABLE_IN_BATTLE | ABILITY_FORMULA_DAMAGE`. The animation
in `gAbilityAnimations` is an unrelated number, so the arithmetic and the
picture can change independently. `gAbilityStaging` is 0 for most abilities,
and the battle then stages a cast, or only the messages for an ability with
nothing to draw.

| Name | Value | Meaning |
| --- | --- | --- |
| `FIRST_ENEMY_UNIT` | 128 | the first enemy's unit id |
| `PARTY_BATTLE_SIZE` | 4 | party members who fight at once |
| `ENEMY_BATTLE_SIZE` | 6 | enemies on the field at most |

[Game rules](src/game/README.md#how-one-number-finds-a-row-and-its-name)
has the tables, and [Battles](src/battle/README.md#the-ability-kind) every
ability kind.

## Rounds and turns

![Isaac and Garet against two Gryphons: both attack, then the first Gryphon casts Impact and its partner's Attack rises, and the second casts Tornado on the pair; the next round's menu opens.](https://alchemy.pascalpixel.com/guides/battle/round.gif)

A round is one pass of choosing, sorting and playing actions. The faster a
unit, the earlier it acts, and after every action the game checks whether a
side has been beaten, so a battle can end in the middle of a round.

`Battle_GatherActions` (`round.c`) collects the round's actions: the party's
from the command menu, or from `auto_battle.c` under `FLAG_AUTO_BATTLE`, and
the enemies' from `BattleAI_ChooseAction` (`enemy_ai.c`). `BattleTurnOrder_Sort`
sorts them by Agility, except that the Djinn Granite, Flash and Ground go
before everyone. Then, for each action, `BattleStaging_DispatchAction`
(`staging.c`) builds its turn (`BattleTurn_Build`, `turn.c`), stages it and
runs its animation, during which `BattleAbility_ApplyToTarget`
(`target_exec.c`) applies the ability to each target, and ends the actor's
turn (`BattleUnit_ProcessTurnEnd`, `turn_end.c`). `Battle_ProcessRoundEnd`
closes the round. The loop in `Battle_Run`, simplified:

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

Every step rounds down.

`BattleAbility_ApplyToTarget` rolls the added effect (an extra such as Sleep,
`Battle_RollEffectSuccessFar`), switches on the ability kind to choose the
formula, then applies the effect if it landed. The formulas are in
`src/game/game/battle_calc.c`, called by their far-call names:
`Battle_CalculatePsynergyPowerFar` for Psynergy damage,
`Battle_CalculateAttackDamageFar` for attacks and `Battle_CalculateRecoveryFar`
for healing. `Battle_GetElementMultiplier` gives the element's share in 256ths
(`FULL_RATE`), the fall-off tables are in `src/battle/battle_calc.s`, and the
guard is `REDUCE_BY_GUARD`.

[Battles](src/battle/README.md#how-an-ability-resolves) has every ability
kind's formula, the fall-off tables and the added effects.

## The battle's picture

![A battle opens on the Lizard King, a Harridan and two Stone Soldiers: the camera sweeps in from close beside the enemies to a wide view as each is announced, then swings round behind the party as the first menu opens, the battlefield sliding and growing with it.](https://alchemy.pascalpixel.com/guides/battle/opening.gif)

The battlefield seems to stretch into the distance and slides sideways as the
camera swings round, while the status window and the messages stay flat in
front. The battlefield is one flat picture of the place, 256 by 120 pixels,
with the depth already painted in; the hardware only zooms and slides it.

| Layer | What it holds |
| --- | --- |
| `BG0` | the windows: status and messages |
| `BG1` | the battlefield at a fixed size (`BATTLE_BACKGROUND_FIXED`) |
| `BG2` | the battlefield zoomed with the camera (`BATTLE_BACKGROUND_ZOOMED`), or an animation's canvas |
| sprites | the units ([sprites](glossary.md#sprite-and-oam)) |

`BG2` is the [background layer](glossary.md#background-layer) the hardware can
scale and turn, and `BattleCamera_SetRange` sets its zoom from the camera's
distance and its sideways scroll from the camera's turn, once a frame. Such a
layer can name only 256 tiles and the picture has 480, so the top 64 pixels
are numbered from character block 2 and the rest from character block 3, and
an HBlank DMA rewrites `BG2CNT` after each screen line to say which block the
next line reads. `BattleCamera_SetRange` builds those 160 values into one page
while the other is shown:

| Lines | `BG2CNT` |
| --- | --- |
| 0 to 15 | off, except while commands are chosen (`FLAG_CHOOSING_COMMANDS`) |
| to where the lower part starts at this zoom | the upper part, character block 2 |
| from there to 135 | the lower part, character block 3 |
| 136 to 159 | off: the status window covers them |

An animation that needs `BG2` for its canvas switches the battlefield to
`BG1`, whose scroll still follows the camera
([the canvas](src/battle_anim/README.md#the-canvas)).

## Animations

![Isaac summons Ramses: the screen goes black, Ramses rises over a desert under a purple sky and brings down his fist in a burst of fire, the battle comes back with `Garet takes 151 damage!!!`, and `Isaac's Earth Power rises by 30!`.](https://alchemy.pascalpixel.com/abilities/ramses.gif)

Every ability's animation, from a sword swing to a summon's whole scene, is one
function that draws it frame by frame and returns when it is over, while the
battle waits. Most draw into a small hidden picture, the canvas, which the
screen shows stretched and blended over the battle.

`BattleStaging_DispatchAction` starts the animation through one of three entry
points, chosen by bits of the turn's `animation`, the ability's
`gAbilityAnimations` row:

| Entry point | Chosen by | What it runs |
| --- | --- | --- |
| `BattleAnim_RunTableEffect` | neither bit | an animation from `gBattleAnim_Handlers` |
| `BattleAnim_DispatchByIdRange` | `ANIM_BY_ID_RANGE` | a casting impact (100 to 135), a ring and spark scene (200 and up) or a beam sequence |
| `BattleAnim_RunSummonEffect` | `ANIM_SUMMON` | a summon's scene, such as Ramses' |

Each entry point borrows three blocks from the heap (the sprite sheet, the
work block and the canvas), runs the animation and gives them back in reverse
order, so an animation must remove every task that reads them before it
returns. Within a frame its steps run in a fixed order, and many draw random
numbers, so moving one changes everything drawn after it.

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

An animation is an ordinary function with its own loop: it calls `WaitFrames`
whenever time has to pass and returns only when it is over. The damage comes
from a [task](glossary.md#task), which `WaitFrames` runs once a frame while the
animation waits ([Engine](engine.md#waiting-and-tasks)). The staging runners in
`src/battle/battle/staging.c` all follow this pattern;
`BattleStaging_RunPsynergyCast`, simplified:

```c
Task_Add(BattlePlayback_Run, 0xc80); /* idle task */
if (plan->animation & ANIM_BY_ID_RANGE)
  BattleAnim_DispatchByIdRangeFar(&work.effect); /* the animation's loop */
else
  BattleAnim_RunTableEffectFar(&work.effect);
BattlePlayback_WaitForReady(); /* until every target has played */
```

Playback (`src/battle/battle/playback.c`) stays idle until the animation calls
`ANIM_SHOW_DAMAGE`, usually on the frame the blow lands. From then on the task
runs `BattleAbility_ApplyToTarget` for one target at a time and plays what it
queued. `BattleAbility_ApplyToTarget` never draws or waits: it changes the
target's HP and statuses and pushes entries onto the playback queue
(`struct BattlePlaybackQueue`) in the order the player sees them. An
animation that never calls `ANIM_SHOW_DAMAGE`, such as Torch's, leaves
playback to `BattlePlayback_WaitForReady`, so its damage shows afterwards.

| Name | Value | Meaning |
| --- | --- | --- |
| `BATTLE_PLAYBACK_CAPACITY` | 64 | entries the playback queue holds |

[Battles](src/battle/README.md#rules-and-pictures-meet-at-the-playback-queue)
takes one target's queue apart.

## Limits

- There are 519 abilities, ids 0 to 518; `gAbilityStaging` is one row shorter.
- A turn reaches at most `TARGET_MAX` (14) targets.
- The party fights four at a time and the enemies six at most
  (`PARTY_BATTLE_SIZE`, `ENEMY_BATTLE_SIZE`).
- `formations.c` has 380 formations, each of up to five kinds of enemy.
- The playback queue holds `BATTLE_PLAYBACK_CAPACITY` (64) entries, and
  `BattlePlayback_Push` doesn't check it.
- The canvas is 128 by 128 pixels, the work block holds 64 particles, and
  there are two blitter slots.
