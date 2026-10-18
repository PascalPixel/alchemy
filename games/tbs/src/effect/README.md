# Battle effects

This folder holds The Broken Seal's battle effects: everything drawn over the
battle while a Psynergy, a Djinni, a summon, an item or an enemy attack plays
out. Each effect is a function that takes the effect's `BattleEffectArgument`
(the user, the targets and the effect's own values), plays a fixed number of
frames and returns when the effect is over.

## Files

| File | What it holds |
| --- | --- |
| `effect.c` | the guide, the includes and the macros every effect shares; it includes the parts below at its end |
| `parts/*.c` | the effects themselves, in 66 parts (index below); never compiled on their own |
| `effect_table.c` | the effects' tables: picture cells, sizes, offsets and frame lists, in the order the code uses them, and the handler table |
| `effect_bits.c` | the bit operands the blitter builder adds to its widths |
| `battle_effect_composite.s`, `battle_effect_composite_color.s` | the ARM fragments blitters are built from |

The headers are in `games/tbs/include/unit/effect/`: `effect.h` for the
functions and `effect_table.h` for the tables.

A part opened on its own lacks the macros and declarations `effect.c` gives
it before its include, so an editor may flag names in it; read and check a
part as part of `effect.c`.

In the ROM the effect code is one object: 150 KB of compiled code, one small
block of constants, then the tables. Camelot's other C files are each followed
by their own data, so the effects were very likely one large file, with the
tables kept in a separate data file in step with it. `effect.c` stays that one
object; the split into parts is ours, cut where one family of effects ends and
the next begins, and the order of its `#include` lines is the order of the
code in the ROM. The tables are not `const`: as `const`, 52 of them change the
generated code, and without it all of them match. Each function's small
constant vectors are `static` variables inside it, in the ROM in the order of
the functions that use them.

## Finding an effect

Each ordinary effect kind is named after the move that uses it, such as
`BattleFx_RunQuake` or `BattleFx_RunCure`. To find a move's code, search for
its English name: its handler's comment begins `// Abilities:` and lists every
ability that reaches it. The handler table, `gBattleFx_EffectHandlers` in
`effect_table.c`, names each kind's part and first ability:

```c
BattleFx_RunQuake, // 1, quake.c: Quake
```

Kinds that no ability uses keep descriptive names. Many kinds are one family
drawn by a shared runner with a mode, written as
`DEFINE_EFFECT_KIND(BattleFx_RunCure, BattleFx_RunHealingMotes, 2)`: the move's
name over the runner and the mode it passes. The largest families:

| Runner | Part | What it draws |
| --- | --- | --- |
| `BattleFx_RunTargetStrike` | `target_strike.c` | repeated slashes or columns of light on the first target |
| `BattleFx_RunProjectileVolley` | `projectiles.c` | shots that fly from the user and burst on each target |
| `BattleFx_RunLobbedShells` | `lobbed_shells.c` | shells lobbed at each target in turn, bursting into smoke |
| `BattleFx_RunBurstShower` | `burst_shower.c` | a shower of bursts at random points near the targets |
| `BattleFx_RunTargetMotes` | `target_motes.c` | smoke, hearts or orbs turning around each target |
| `BattleFx_RunStatBoost` | `stat_change.c` | flares that swirl into each target and burst |
| `BattleFx_RunStatReduction` | `stat_change.c` | a dagger or shield over each target that shatters |
| `BattleFx_RunHealingMotes` | `healing.c` | healing sparkles swirling in to each target |
| `BattleFx_RunStreakVolley` | `streaks.c` | streaks fired from above the user at each target |
| `BattleFx_RunRushingStrike` | `rushing_strike.c` | the user rushes at the first target and strikes |
| `BattleFx_RunBurstScene` | `burst_scene.c` | a flurry of strikes on the first target |
| `BattleFx_RunGrowingColumns` | `growth.c` | vines or green pillars growing from the ground |
| `BattleFx_RunDjinnBeams` | `djinn_beams.c` | a Djinni appears, then columns of light fall on the targets |

## Words used here

- **kind**: an effect's number in the handler table, which the ability's
  entry in `gBattle_ActionFlags` selects.
- **runner** and **mode**: a runner draws a family of kinds; the mode it is
  passed picks the family member.
- **variant**: `effect->variant`, the size of the ability (Quake, Earthquake or
  Quake Sphere play the same kind at three variants). A particle's `variant`
  field is something else: a spare word each effect uses as it needs.
- **side**: `effect->side`, 1 when the targets are the party and 0 when they
  are enemies; effects mirror on it.
- **sheet** and **cell**: a sheet is a decoded set of pictures in
  `work->sheet`; a cell is a byte offset into it (the `*_CELL` and `*_CELLS`
  constants).
- **level**: a pixel value, an index into a palette ramp. Fades work on levels.

## Entry points

The battle starts an effect through one of three functions, each of which
allocates the effect's three heap blocks (sprite sheet, work block and
canvas), runs the effect and releases them:

- `BattleFx_RunTableEffect` (`dispatch.c`) looks the effect's kind up in
  `gBattleFx_EffectHandlers`, the table of ordinary effects.
- `BattleFx_RunSceneEffect` (`dispatch.c`) runs one of the large effects, those
  whose presentation flags have bit 15 set, chosen by its kind.
- `BattleFx_DispatchByIdRange` (`positions.c`) runs the casting impacts, beam
  sequences and ring scenes, which need a sprite sheet twice the usual size.

An effect may schedule callbacks that use those blocks, but it must remove
them before it returns, because the blocks are gone afterwards.

## The canvas

Most effects draw into one canvas: 128x128 pixels of 8-bit colour, stored as
16x16 tiles of 8x8 pixels. BG2 shows it, stretched to twice the width so that
it spans the screen; `BattleFx_BeginCanvasLayer` (`canvas.c`) sets that up.

An effect never writes the canvas to video memory itself. It draws a frame,
sets the work block's `transferPending` and waits a frame. The scheduled
callback `BattleFx_ProcessPendingGraphicsTransfer` copies the canvas to video
memory and then treats the pixels it keeps according to the work block's
transfer mode (`enum CanvasTransferMode`): leave them, clear them, keep 50% or
75% of each, darken them or brighten them. Canvas pixels are levels in a
palette ramp, so fading the levels makes trails decay without any bookkeeping
per particle.

Two other paths use BG1. `BattleFx_SetupCanvasTileMap` shows the canvas
on BG1 without stretching it (the beam scenes). Effects that draw a
full-screen picture into `gMapCellBuffer` show it on BG1 too, flushed by
`BattleFx_FlushPendingGraphicsTransfer` (the particle streams).

## Blitters

Drawing a picture onto the canvas goes through a blitter, a small ARM routine
that `BattleFx_BuildBlitter` (`blitter.c`) assembles at run time from the
fragments in `battle_effect_composite.s`, for one cell size, one mirroring, one
clip and one way of combining pixels (copy, additive, brightest wins and so
on). A general routine that tested all of these for every pixel would be far
too slow. The routines live in two heap slots, and an effect usually takes
them as a `BlitterPair`: `[0]` is the picture as drawn and `[1]` another mode
or its mirror, so one picture can be drawn on both sides of the screen.

## The frame loop

Every effect has the same shape. Its setup opens the canvas, builds the
blitters, loads the pictures and places the particles. Then it runs a fixed
number of frames, and finally releases what the setup took; the transfer
callback must be gone before the heap blocks are freed.

Within a frame, steps each run over a range of frames, in a fixed order,
ending with the transfer. The order matters for two reasons: later steps draw
over earlier ones, and many steps draw random numbers, so moving a step
changes every random value after it. Cues that fire on one frame, such as a
sound, a screen shake or a palette step, are tests on the frame number, and the
comments in a long effect mark where each of its phases begins.

## Particles

A particle is an `EffectStep`: a position and a velocity in three axes, 16.16
fixed point unless a comment says otherwise, and one spare word (`variant`)
that each effect uses as it needs: a life, a frame or a picture. There are two
pools. The work block holds 64 (`work->particles`); larger effects use
`gMapCellBuffer`, 48 KB of map storage the battle does not need while an
effect plays, room for about 1,750 (`PARTICLES` and `Ram_MapCellBuffer` name
the same buffer). Effects reserve ranges by index, and a slot whose particle
has finished is reused by the next one launched, so an effect's pool size is
fixed, and its comments say which indices hold what.
`EffectStep_AdvanceWithGravity2D` and its kin (`positions.c`) move particles
under gravity, and `BattleUnit_ProjectToScreen` projects battle positions onto
the screen.

Several families break something apart: boulders crumble into chips, shells
burst into shards, an impact throws out fragments and Judgment lifts pieces
of the field into the air. Each piece is a particle, drawn by its top-left
corner, centred on its position or standing on it, as each family chooses.
Spawning stays with each runner: every family draws its own random ranges,
and the order of those draws is part of the effect.

## Pictures and colours

An effect loads its pictures by resource id: `ResourceId_FirePillarSheetB` is
an entry in the resource directory
(`games/tbs/src/system/resource/directory.s`) that points at
`BattleFx_FirePillarSheetB`. That label is defined in
`games/tbs/src/graphics/battle_fx/battlefx.s`, or in a file of its own under
`games/common/src/graphics/battle_fx/` when both games use it, and its data is
built from an indexed PNG beside it (`fire_pillar_b.png`, with a `.tsv` part
list where it has one). Edit the PNG and its palette; the build packs them.

Each runner decodes its pictures at fixed offsets in `work->sheet`. Pixel
values are levels: a picture drawn with additive or max compositing needs its
levels in the same ramp as the canvas palette the effect loads. A fade draws
from darker copies of a picture built before the first frame, and a symmetric
picture is drawn left of its centre line and then mirrored to the right.

## Changing an effect

1. Find the effect (above) and read its header: the frame timeline, the
   particle ranges and the pictures it loads.
2. Change it, then build and check the edition you changed:

   ```sh
   make tbs-en            # build the English ROM, out/tbs-en/tbs-en.gba
   make compare-editions  # for a matching change: all twelve must stay OK
   ```

   A mod changes the bytes on purpose, so it will not compare OK; keep the
   matching tree separate from your mod.
3. Keep within the limits: the canvas is 128x128 pixels; `work->sheet`
   holds 0x6980 bytes; the work block has 64 particles and `gMapCellBuffer`
   about 1,750; there are two blitter slots, and each new picture size or
   mode needs its own blitter. Steps that draw random numbers keep their
   order, or every later random value changes.

## Parts of effect.c, in ROM order

| Part | What it holds |
| --- | --- |
| `display.c` | Display, scroll and window HBlank DMA helpers. |
| `falling.c` | Ice and Douse: shards and bolts falling on the targets' side. |
| `streaks.c` | Streak volleys (Sonic Wave, Shriek, War Cry ...) and mote fountains (Sleep Star, Soothing Star). |
| `target_strike.c` | Monster bites, fangs and slashes on one target. |
| `death_plunge.c` | Death Plunge and the scene helpers it uses. |
| `spider_web.c` | Poison Beat, Spinning Beat and Spider Web. |
| `canvas.c` | The canvas layer: transfers, opening and closing, tile maps, lines. |
| `planet_diver.c` | Planet Diver, Haunt and the orbiting spirals. |
| `gathering_lights.c` | Demon Night, Rumble and Haunting. |
| `mirrored_wall.c` | The walls of Berserk, Spasm and Recovery. |
| `djinn_beams.c` | Revive, Water of Life, Quartz and Dew. |
| `lobbed_shells.c` | Lobbed shells (Mad Spatter, Decompose, Bone Chiller ...). |
| `condemn.c` | Condemn. |
| `bind.c` | Bind and Triangle Strike. |
| `stars.c` | Astral Blast and Shining Star. |
| `procne.c` | The Procne summon. |
| `tiamat.c` | Dragon Driver and the Tiamat summon. |
| `nereid.c` | The Nereid summon. |
| `vortices.c` | Froth and Whirlwind. |
| `prism.c` | Prism. |
| `plasma.c` | Plasma, and the spark groups of Blast and Dynamite. |
| `volcano.c` | Volcano. |
| `target_motes.c` | Status motes: Taint, Delude, Paralyze, Sleep, Curse .... |
| `djinn_heal.c` | Djinn healing. |
| `dispatch.c` | Running scene and table effects, and shared target helpers. |
| `cybele.c` | The Cybele summon. |
| `boreas.c` | The Boreas summon. |
| `drain.c` | Break and Drain. |
| `healing.c` | Healing motes: Ply, Wish, Cure, Cure Poison. |
| `stat_change.c` | Stat boosts and reductions (Impact, Guard, Ward, Dull, Weaken ...). |
| `flare.c` | Flare. |
| `quake.c` | Quake. |
| `fire.c` | Fire. |
| `frost.c` | Frost. |
| `ray.c` | Ray. |
| `eruption.c` | Smoke Bomb and Blast. |
| `burst_shower.c` | The blessings' burst showers. |
| `orbiting.c` | Swirling stars, orbiting skulls and target bursts. |
| `atalanta.c` | The Atalanta summon. |
| `growth.c` | Growth, Punji, Vine and Thorn. |
| `bolt.c` | Bolt. |
| `projectiles.c` | Projectile volleys (Slash, Shuriken, Wing Flutter, Blizzard ...). |
| `rushing_strike.c` | Rushing strikes (Bone Charge, Brute Force, Onslaught, Headbutt). |
| `djinn_attacks.c` | The Mercury, Jupiter, Venus and Mars Djinn attacks. |
| `hail.c` | Hail and Ground. |
| `thor.c` | The Thor summon. |
| `spire.c` | Spire. |
| `burst_scene.c` | Gust, Flying Attack and the rush bursts. |
| `gaia.c` | Gaia. |
| `positions.c` | Effect positions, projection and id dispatch helpers. |
| `beams.c` | Beam sequences and the ring and spark scene. |
| `casting_impact.c` | The casting impact scene and its image helpers. |
| `torch.c` | Torch and Kite. |
| `helm_splitter.c` | Helm Splitter, formations and the impact burst. |
| `particle_streams.c` | Meteor and Outer Space. |
| `ramses.c` | The Ramses summon. |
| `dragon_cloud.c` | Dragon Cloud. |
| `annihilation.c` | Annihilation. |
| `ragnarok.c` | Ragnarok. |
| `titan_blade.c` | Titan Blade. |
| `judgment.c` | The Judgment summon. |
| `kirin.c` | The Kirin summon. |
| `neptune.c` | The Neptune summon. |
| `thrash.c` | Thrash, Quick Strike and Vorpal Slash. |
| `undead_sword.c` | Undead Sword. |
| `blitter.c` | The blitter builder, BattleFx_BuildBlitter. |
