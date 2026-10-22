# Battle

![A battle in Goma Cave pulled apart into its layers: at the back the cave as the perspective plane on BG2, then the same cave flat on BG1, then the status window and message on BG0, and the party and the enemy as sprites in front](https://alchemy.pascalpixel.com/guides/battle/layers-3d.gif)

The battle layer is the code that runs a fight from its opening transition to
the spoils: the rounds and turn order, the ability tables, the arithmetic each
target goes through, and the effects drawn over the battlefield while it
resolves.

## Starting a battle

A battle is a scene of its own that runs from the main image rather than an
overlay, entered from the field and returning to it.

```mermaid
flowchart LR
    field["Field scene:<br/>an encounter raises an exit<br/>to SCENE_BATTLE"] --> run["Game_Run:<br/>reset tasks and heap"]
    run --> enc["Battle_RunEncounterFar(entrance)"]
    enc --> back["Scene_ReturnFromBattle:<br/>the field scene again,<br/>or the defeat point"]
```

Its code is in the main image's `battle` bank. A random encounter raises a
field exit like any other, with `SCENE_BATTLE` as the next scene and the enemy
group in `gGameState.entrance`. `Game_Run` (`src/object/object/scene.c`)
resets the task table and the heap as it does between any two scenes, then
calls `Battle_RunEncounterFar(entrance)` instead of loading an overlay. The
debug battle menu (`DebugBattle_RunMenu`) is the only other caller.

`Battle_RunEncounter` (`src/battle/battle/encounter.c`) allocates the
battle's work slots, `gBattleWork` among them, sets `FLAG_IN_BATTLE`, and
fights the whole battle before it returns. The result is the number of
enemies defeated, `ENCOUNTER_FLED` or `ENCOUNTER_PARTY_LOST`.
`Scene_ReturnFromBattle` then reloads the field scene the battle started
from, with the party where it was, or after a defeat goes to the defeat
point. The enemy group is a row of `formatn.tsv`, which `formation.c` turns
into enemy units; the encounter zones in `src/object/encounter.tsv` choose
the row.

| Name | Value | Meaning |
| --- | --- | --- |
| `SCENE_BATTLE` | `0x1fe` | the next scene that runs a battle instead of an overlay |
| `FLAG_IN_BATTLE` | `0x103` | set while a battle runs |
| `ENCOUNTER_FLED` | 999 | `Battle_RunEncounter`'s result when the party got away |
| `ENCOUNTER_PARTY_LOST` | -1 | its result when the party was defeated |

[Field objects](src/object/README.md#random-battles) covers how the field
rolls the encounter, and [Battles](src/battle/README.md#rounds) what
`Battle_RunEncounter` does from the opening to the spoils.

## Units and abilities

Everyone in a fight is a unit, and everything a unit can do on its turn is an
ability, one row number across three tables.

Party members keep their character numbers as unit ids, and enemies take ids
from `FIRST_ENEMY_UNIT`. `Unit_GetFar` returns a unit's `struct BattleUnit`,
with its HP, PP, stats, statuses and a Power and a Resist in each element.

An ability runs from Attack to Quake, a Djinni, a summon, an item's use or an
enemy's special move. Its id is the same row in three tables:

| Table | Read through | What its row says |
| --- | --- | --- |
| `src/game/actions.tsv` | `struct BattleAct`, `BattleAction_Get` | what the ability does: its targets, element, range, PP cost, power, added effect and ability kind |
| `src/battle/actflags.tsv` | `gBattle_ActionFlags` | the effect that draws it, and how it is staged |
| `src/battle/actstat.tsv` | `gBattle_ActionStatus` | how the battle shows it, where it overrides the default |

Quake is row 3. Its row of `actions.tsv` has element `ELEMENT_VENUS`, range 2,
a PP cost of 4 and a power of 12, and its `flags` are `0x85`:
`ACTION_USABLE_IN_BATTLE` plus `ABILITY_KIND_DAMAGE`. The ability kind, the
low four bits of `flags` (`ABILITY_KIND_MASK`), chooses the formula the
targets go through. The effect kind in `actflags.tsv` is an unrelated number
that chooses only the animation, so the formula and the picture can be
changed independently.

| Name | Value | Meaning |
| --- | --- | --- |
| `FIRST_ENEMY_UNIT` | 128 | the first enemy's unit id |
| `PARTY_BATTLE_SIZE` | 4 | party members who fight at once |
| `ENEMY_BATTLE_SIZE` | 6 | enemies on the field at most |
| `ABILITY_KIND_MASK` | 15 | the bits of an ability's `flags` that are its kind |

[Game rules](src/game/README.md#a-rows-id-is-its-position-and-its-name-is-a-message-at-the-same-position)
has the tables and [the unit stats they feed](src/game/README.md#stats-are-recomputed-whole), and [Battles](src/battle/README.md#the-ability-kind) every
ability kind.

## Rounds and turns

A round gathers every unit's action, sorts them into turn order and then
plans, stages, applies and closes each one in turn.

A round has five steps, each in its own part of `src/battle/battle/`:

1. `round.c` gathers every unit's action: the party's from the command menu,
   the enemies' from `BattleCommand_SelectAutomatic` (`enemy_ai.c`), or from
   the other console in a link battle, and from `auto_battle.c` under
   `FLAG_AUTO_BATTLE`. `BattleQueue_SortByPriority` sorts them by Agility,
   fastest first, except that Granite, Flash and Ground go before everyone.
2. `BattleCommand_BuildPlan` (`plan.c`) turns each action into a
   `struct BattlePlan`: the ability id, the targets, double and critical blows
   rolled in advance, and the staging.
3. `BattlePresentation_DispatchAction` (`staging.c`) stages the plan and runs
   its effect.
4. `BattleTarget_Execute` (`target_exec.c`) applies the ability to each target.
5. `turn_end.c` counts down stat changes and statuses, recovers Djinn and
   applies poison and the Grim Reaper's countdown.

The battle ends when one side has no unit left standing or the party gets
away. A plan reaches at most `TARGET_MAX` (14) targets.
[Battles](src/battle/README.md#rounds) goes through each step, with
[enemy action selection](src/battle/README.md#enemy-actions),
[first strikes and escape](src/battle/README.md#first-strikes-and-escape)
and [planning an action](src/battle/README.md#planning-an-action).

## Resolving an ability

`BattleTarget_Execute` puts each target in turn through the formula its ability
kind chooses, scaled by distance and element.

It rolls the added effect (`Battle_RollEffectSuccessFar`), then switches on the ability
kind to choose the formula, then applies the added effect if it landed. The
formulas are in `src/game/game/battle_calc.c` and are called by their
far-call names: `Battle_CalculateAttackDamageFar` for attacks, `Battle_CalculatePsynergyPowerFar` for Psynergy
damage, `Battle_CalculateRecoveryFar` for healing.

For `ABILITY_KIND_DAMAGE` the power is the damage the aimed target takes when
the caster's Power in the ability's element equals the target's Resist in it;
a random 0 to 3 is then added. Quake's 12 is therefore 12 to 15. Two factors
scale it:

- Distance. A range counts places in the target side's line-up, and each step
  from the aimed place takes a lower share from the fall-off tables in
  `src/battle/battle_calc.s`: 80 percent one step away and 60 two steps away
  for Psynergy damage. Summons and enemy unleashed damage have tables of
  their own.
- Element. Each point the caster's Power is ahead of the target's Resist adds
  half a percent, up to double, and each point behind takes half a percent
  off, down to nothing (`Battle_GetElementMultiplier`). The multiplier is
  counted in 256ths (`FULL_RATE`), and every result rounds down.

| Name | Value | Meaning |
| --- | --- | --- |
| `ABILITY_KIND_DAMAGE` | 5 | HP damage from the power: Psynergy |
| `ABILITY_KIND_SUMMON` | 8 | a summon's damage |
| `FULL_RATE` | 256 | a multiplier of 1, in 256ths |

[Battles](src/battle/README.md#how-an-ability-resolves) has every ability
kind's formula, the fall-off tables and the added effects, worked through for
Avalanche, Cure and Ragnarok.

## The battle's picture

A battle is drawn on three backgrounds and the sprites, with the battlefield
as a perspective plane that an effect may trade for a flat picture.

The battlefield is `BG2`, an affine plane in perspective
(`BATTLE_BACKGROUND_PLANE`): `BattleCamera_SetRange` builds a page of
per-line values, and an HBlank DMA gives every line from 0 to 135 its own
`BG2CNT` and scale, so the plane stops above the status window and turns with
the camera's yaw. Outside the time commands are chosen
(`FLAG_CHOOSING_COMMANDS`), its top 16 lines are left off. `BG1` holds the
same picture flat. An effect that needs `BG2` for its canvas switches the
background to that flat picture (`BATTLE_BACKGROUND_FLAT`), whose horizontal
scroll follows the camera. `BG0` holds the windows, and the units are
sprites. The picture at the top of this page takes these layers apart.

| Layer | What it holds |
| --- | --- |
| `BG0` | the windows: status and messages |
| `BG1` | the battlefield flat (`BATTLE_BACKGROUND_FLAT`) |
| `BG2` | the battlefield in perspective (`BATTLE_BACKGROUND_PLANE`), or an effect's canvas |
| sprites | the units |

[Battle effects](src/effect/README.md#the-canvas) describes how an effect
takes `BG2` for its canvas.

## Effects

An effect is the function that draws an ability, started through one of
three entry points and run to its end while the battle waits.

`BattlePresentation_DispatchAction` starts the effect through one of three
entry points, chosen by bits of the plan's `presentationFlags`, which are the
ability's `actflags.tsv` row:

| Entry point | Chosen by | What it runs |
| --- | --- | --- |
| `BattleEffect_RunTableEffect` | neither bit | an ordinary effect kind, an index into `gBattleEffect_EffectHandlers` |
| `BattleEffect_DispatchByIdRange` | `STAGING_BY_ID_RANGE` (`0x4000`) | casting impacts, beam sequences and ring and spark scenes, by the effect kind's range |
| `BattleEffect_RunSceneEffect` | `STAGING_SCENE` (`0x8000`) | the summons' scenes |

Each entry point takes three heap blocks (the sprite sheet, the work block
and the canvas), runs the effect to its end and releases them in reverse
order, since releasing a heap block also releases everything allocated after
it. An effect must therefore remove every callback that reads those blocks
before it returns.

Most effects draw into a canvas of 128 by 128 pixels, one byte each, which
`BG2` shows blended over the battle and stretched to twice its width
(`BattleEffect_BeginCanvasLayer`). An effect copies pictures onto the canvas
through blitters, ARM routines `BattleEffect_BuildBlitter` assembles at run time
for one combination of size, mirroring, clipping and blend mode, and moves
particles over it. It never writes video memory itself: it sets
`transferPending` and waits, and the callback
`BattleEffect_ProcessPendingGraphicsTransfer` copies the canvas and then fades,
clears or keeps the pixels left in it as the work block's transfer mode says.
Canvas pixels are levels in a palette ramp, so fading them makes trails die
away without per-particle bookkeeping.

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
`enum SceneEffectKind`. They take the same three blocks; unlike
`BattleEffect_RunTableEffect`, `BattleEffect_RunSceneEffect` leaves setting
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

The damage messages appear while the effect is still drawing because a task
plays the rules' results from the frame the blow lands.

![Isaac casts Quake: the rock rises under the party, and the damage messages appear while it's still on screen, the aimed member taking more than the one beside him](https://alchemy.pascalpixel.com/abilities/quake.gif)

An effect is an ordinary function with a loop of its own: it calls
`WaitFrames` whenever time has to pass and returns only when the animation is
over. The battle code that called it cannot run until then.

The messages are shown from a task. Before it starts the effect,
`BattlePresentation_DispatchAction` schedules `BattleEvent_Playback`
(`src/battle/battle/playback.c`) with `Scheduler_AddOrUpdateCallback`, and
after the effect returns it waits in `BattleEvent_WaitForReady` until
playback is done. Playback stays idle until the effect calls
`EFFECT_SHOW_DAMAGE(hitSound)`, which most effects do on the frame the blow
lands; if an effect never calls it, the wait after the effect starts playback
instead. From then on, once a frame, the task runs `BattleTarget_Execute` for the
next target and plays that target's events over the following frames before
it moves on.

```mermaid
sequenceDiagram
    participant D as BattlePresentation_DispatchAction
    participant F as Effect
    participant P as BattleEvent_Playback (task)
    participant T as BattleTarget_Execute
    participant Q as Event queue
    D->>P: Scheduler_AddOrUpdateCallback
    D->>F: start the effect
    loop until the blow lands
        F->>F: WaitFrames
    end
    F->>P: EFFECT_SHOW_DAMAGE(hitSound)
    par the effect keeps drawing
        F->>F: WaitFrames
    and playback runs the rules
        loop each target, once a frame
            P->>T: BattleTarget_Execute(target)
            T->>Q: BattleEvent_Push (arguments, then message)
            P->>Q: play that target's events
        end
    end
    F-->>D: effect returns
    D->>P: BattleEvent_WaitForReady until playback is done
```

The two sides meet at the battle event queue (`struct BattleEventQueue`,
`BATTLE_EVENT_CAPACITY` events). `BattleTarget_Execute` never draws or waits: it
changes the target's HP and statuses and pushes events with `BattleEvent_Push` in
the order the player sees them, a message's arguments before the message,
since the message takes them from the window system's argument table when it
is decoded. `BattleEvent_DispatchQueued` plays the same queue straight
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
takes one round's queue apart, event by event.

## Limits

- There are 519 abilities, ids 0 to 518; `actstat.tsv` is one row shorter.
- A plan reaches at most `TARGET_MAX` (14) targets.
- The party fights four at a time and the enemies six at most
  (`PARTY_BATTLE_SIZE`, `ENEMY_BATTLE_SIZE`).
- `formatn.tsv` has 380 enemy groups, each of up to five kinds of enemy.
- The event queue holds `BATTLE_EVENT_CAPACITY` (64) events.
- The canvas is 128 by 128 pixels, the work block holds 64 particles, and
  there are two blitter slots.
