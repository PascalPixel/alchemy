# Battle effects

This folder holds The Broken Seal's battle effects: everything drawn over the
battle while Psynergy, a Djinni, a summon, an item or an enemy attack plays
out. Each effect is a function that takes the effect's `BattleEffectArgument`
(the user, the targets and the effect's own values), plays a fixed number of
frames and returns when the effect is over.

See also: [Battles](../battle/README.md), which starts each effect and plays
its messages and hits while the effect runs.

## Files

| File | What it holds |
| --- | --- |
| `effect.c` | the guide; it includes `effect_internal.h`, then the parts below in ROM order |
| `effect_internal.h` | the includes, macros and helpers every part shares; each part includes it too |
| `parts/*.c` | the effects themselves, in 66 parts; never compiled on their own |
| `effect_table.c` | the effects' tables: picture cells, sizes, offsets and frame lists, in the order the code uses them, and the handler table |
| `effect_bits.c` | the bit operands the blitter builder adds to its widths |
| `battle_effect_composite.s`, `battle_effect_composite_color.s` | the ARM fragments blitters are built from |

The headers are in `games/tbs/include/unit/effect/`:
[`effect.h`](../../include/unit/effect/effect.h) for the functions and
`effect_table.h` for the tables.

`effect.c` includes the parts in ROM order, and each part's first lines say
what it holds: `head -3 parts/*.c` lists them all. Each part includes
`effect_internal.h`, so a part opened on its own resolves in an editor; it is
still compiled only as part of `effect.c`.

In the ROM the effect code is one object of about 150 KB; its rows of
`games/tbs/objects.tsv` (from `effect`) give it, the blitter fragments, the
tables and the bit operands in link order.

## Words

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
- **element**: 0 Venus, 1 Mercury, 2 Mars, 3 Jupiter, where a function takes
  one.

The same locals carry the same names in every handler:

- `effect`: the `struct BattleEffectArgument *` a handler is passed.
- `slots` and `cursor`: `gBattleFxWork`, and the pointer that walks it to
  fetch `work` and `canvas` (`EFFECT_GET_SLOTS`).
- `work`: the effect's `struct BattleEffectWork`; `canvas`: the 128x128 canvas.
- `sheet`: the decoded pictures, usually `work->sheet`.
- `draw`: the `BlitterPair`, a rectangle blitter for each facing
  (`BattleFx_FetchRectangleBlitters`).
- `frame`: the frame counter; `cell`: a picture's index or offset.
- `unk_<offset>`: a field whose meaning is not known yet.

## Prefixes

Functions are `BattleFx_` unless they act on one thing the engine names
elsewhere, which keep its prefix as their Lost Age twins do: `Camera_`,
`Palette_`, `Graphics_`, `Resource_`, `EffectStep_`, `EffectPosition_`,
`ObjectGroup_`.

The spellings some sites keep to match (a work slot is read five ways), and
the `DEAD` and `FAKEMATCH` markers, are in [Matching](../MATCHING.md).

## How to find an effect

Each ordinary effect kind is named after the move that uses it, such as
`BattleFx_RunQuake` or `BattleFx_RunCure`. To find a move's code, search for
its English name: its handler's comment has a line `/// Abilities:` that lists
every ability that reaches it. The handler table, `gBattleFx_EffectHandlers` in
`effect_table.c`, names each kind's part and first ability:

```c
[BATTLE_FX_QUAKE - 1] = BattleFx_RunQuake, // quake.c: Quake
```

Kinds that no ability uses keep descriptive names. Many kinds are one family
drawn by a shared runner with a mode: the kind's handler is a two-line
function named for the move that calls the runner with its mode, such as
`BattleFx_RunCure`, which calls `BattleFx_RunHealingMotes(effect, 2)`.
The largest families:

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
| `BattleFx_RunGrowingColumns` | [`parts/growth.c`](parts/growth.c) | vines or green pillars growing from the ground |
| `BattleFx_RunDjinnBeams` | `djinn_beams.c` | a Djinni appears, then columns of light fall on the targets |

## How to write a handler's header

Every handler opens the same way, with a documentation comment:

```c
/// Rock wall: one sentence saying what the effect does.
///
/// Abilities: Quake, Earthquake, Quake Sphere (kind 1).
///
/// What the player sees, frame by frame where the effect is long.
///
/// effect: the user and targets, if the handler reads more than usual.
/// mode  : for a runner, what each mode changes.
```

A kind no ability uses says `Abilities: none (kind N).`, and a scene `(scene
kind N)`. The description does not repeat the kind or the frame count a
`*_FRAMES` constant already gives.

## How the battle starts an effect

The battle starts an effect through one of three functions, each of which
allocates the effect's three heap blocks (sprite sheet, work block and
canvas), runs the effect and releases them:

- `BattleFx_RunTableEffect` ([`parts/dispatch.c`](parts/dispatch.c)) looks
  the effect's kind up in `gBattleFx_EffectHandlers`, the table of ordinary
  effects.
- `BattleFx_RunSceneEffect` ([`parts/dispatch.c`](parts/dispatch.c)) runs one
  of the large effects, those whose presentation flags have bit 15 set,
  chosen by its kind.
- `BattleFx_DispatchByIdRange` (`positions.c`) runs the casting impacts, beam
  sequences and ring scenes, which need a sprite sheet twice the usual size.

An effect may schedule callbacks that use those blocks, but it must remove
them before it returns, because the blocks are gone afterwards: releasing a
heap block also releases everything allocated after it (see
[Runtime](../runtime/README.md)), so the three are released in the reverse
of the order they were taken.

## Design

### One object, and tables that are not `const`

Camelot's other C files are each followed by their own data, so the effects
were very likely one large file, with the tables kept in a separate data file
in step with it. `effect.c` stays that one object; the split into parts is
ours, cut where one family of effects ends and the next begins, and the order
of its `#include` lines is the order of the code in the ROM. The tables are
not `const`: as `const`, 52 of them change the generated code, and without it
all of them match. Each function's small constant vectors are `static`
variables inside it, in the ROM in the order of the functions that use them.

### The canvas

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
full-screen picture into `Ram_MapCellBuffer` show it on BG1 too, flushed by
`BattleFx_FlushPendingGraphicsTransfer` (the particle streams).

### Blitters

Drawing a picture onto the canvas goes through a blitter, a small ARM routine
that `BattleFx_BuildBlitter` (`blitter.c`) assembles at run time from the
fragments in `battle_effect_composite.s`, for one cell size, one mirroring, one
clip and one way of combining pixels (copy, additive, brightest wins and so
on). A general routine that tested all of these for every pixel would be far
too slow. The routines live in two heap slots, and an effect usually takes
them as a `BlitterPair`: `[0]` is the picture as drawn and `[1]` another mode
or its mirror, so one picture can be drawn on both sides of the screen.

The builder walks a template word by word and splices in the pixel fragments
of the mode asked for. Some template words are copied as they are, some are
chosen by the flags, and some have an immediate or a branch displacement added
before they are written. The routine it writes has five blocks, in order:

1. **Prologue**: three entry words, the three-word mask for additive
   compositing, then one of four alternatives that set up the walk for the
   mirroring asked for (X only 3 words, Y only 4, both 3, neither 1).
2. **Vertical clip**, for vertically clipped cells only: eight words, the
   third chosen by the vertical mirroring and the last two carrying the cell
   height.
3. **Columns**: a test and a forward branch that skips the whole routine when
   no rows remain, then either a width mask (unclipped) or twelve clip words
   whose shifts carry the cell width as an ARM rotated immediate
   (`gBattleFx_BitOperands`, in `effect_bits.c`). A second test and branch
   skip everything when no columns remain. Six words, then one or two, then
   five more finish the set-up.
4. **Row loop**: four loop-head words carrying the row count and the width's
   shift, and a fifth when unclipped. Each row then draws a leading partial
   run, pixel by pixel up to the first 8-pixel boundary; the full-cell loop,
   eight pixels a pass from the mode's 8-pixel fragment; and a trailing
   partial run like the leading one. A forward branch skips each of the three
   when it is empty. Three words advance to the next row and a fourth branches
   back to the loop head.
5. **Exit**: two final words, where the two skip branches of block 3 land.

Where the template offers alternatives, the builder's cursor moves past all
of them, whichever it writes, so the walk ends on the template's last word for
every combination; `BattleFx_BuildBlitter` checks that it does. In each
partial run and in the full-cell loop the template keeps four words where the
routine gets its mode's fragment, and those four are skipped too.

Forward branches are written with a zero displacement, their word index
remembered, and patched when the walk reaches their target; backward branches
are computed at once. An ARM branch displacement counts words from the branch
plus two, as the processor reads two words ahead. The walk runs twice: first
with no output buffer, to count the words, then into a heap block of exactly
that size. Branch locations are word indices, so the counting pass needs no
buffer. The fragments' registers and modes are described at the top of
`battle_effect_composite.s`.

### The frame loop

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

### Particles

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

### Picture format and units

- A picture in a sheet is linear: one byte a pixel, row after row, `width`
  bytes to a row. The canvas is tiled instead: 16x16 tiles of 8x8 pixels.
- A pixel value is a level in the palette ramp the effect loads; level 0 is
  transparent to the transparent blitters, as colour 0 is to the hardware.
- Sizes given to `Iwram_CopyWords`, `Iwram_ClearWords` and `Iwram_FillWords`
  are in bytes.
- Positions and velocities are 16.16 fixed point; BG2's affine scale
  (`REG_BG2PA`) is 8.8 and its reference point (`REG_BG2X`) 20.8. On the wide
  canvas, which BG2 stretches to twice its width, an x on the screen is half
  that x on the canvas.

### Pictures and colours

An effect loads its pictures by resource id: `ResourceId_FirePillarSheetB` is
an entry in the resource directory
(`games/tbs/src/system/resource/directory.s`) that points at
`BattleFx_FirePillarSheetB`. That label is defined in
`games/tbs/src/graphics/battle_fx/battlefx.s` or in a file of its own beside
it, and its data is built from an indexed PNG beside it (`fire_pillar_b.png`,
with a `.tsv` part list where it has one). Edit the PNG and its palette; the
build packs them.

Each runner decodes its pictures at fixed offsets in `work->sheet`. Pixel
values are levels: a picture drawn with additive or max compositing needs its
levels in the same ramp as the canvas palette the effect loads. A fade draws
from darker copies of a picture built before the first frame, and a symmetric
picture is drawn left of its centre line and then mirrored to the right.

### Effect families

The largest parts each hold a family of effects that share their particles,
pictures and frame loop. What the code cannot say in one place, the order of a
family's phases and which particle ranges hold what, is said here once; each
part points here.

**Casting impacts** (`casting_impact.c`). A casting impact is chosen by its
number in `enum CastingImpact`, the ability's effect kind less 100, and runs
in four phases. While the caster gathers, 64 sparks close in on it over 32
frames, four more each frame, each in its own random rotation; a Djinni
impact (Bane to Sap, impacts 0 to 7) shows its Djinni 80 pixels over the
caster once the first sparks have gone out. Then BG2 is placed so the impact
sits over the target; impacts 15, 17, 24, 26 and 35 (Severe Blow) stop here
and hand the canvas to another effect. The impact itself lasts 20 to 80
frames, depending on the impact: the caster's overlay, an orb, sparks and the
impact's own animation are drawn while the background palette steps towards
the impact's palette, a Djinni flies to the target over the first six frames,
and the target is hit on frame 6. Last, the canvas and its callbacks are
released, or impact 21 (Meltdown) hands them to the stat reduction. The
particles are used twice: `work->particles` holds the gathering sparks and
then, from the impact on, 32 scattering sparks whose `variant` is their
lifetime; the 64 records at `Ram_MapCellBuffer + 0x4000` belong to the
impact's own animation. An impact that hands off to another runner first
removes its prelude callbacks and releases the blitters that runner will
replace.

**Djinn attacks** (`djinn_attacks.c`). Each attack draws one of the small
Djinn on the canvas and throws its element at the targets. The Mercury Djinni
spirals through the centre of the field and fires five blue flames to the
left, each shattering into motes. The Jupiter Djinni spirals the other way
while eight flameballs fall from the top right and shatter on the ground. The
Venus Djinni sways while ten flame columns rise from the ground, each throwing
motes upwards. The Mars Djinni flares from a glowing orb, dives to the ground
and sets nine flames erupting in an arc while embers rise. The motes the
flames shatter into live in `PARTICLES`, 32 slots a flame; the flames, rocks
and chips themselves live in `work->particles`. The appearance prelude leaves
the canvas ready for the attack that called it, and each attack removes its
callbacks before it returns.

**Judgment** (`judgment.c`). The shockwave scene runs in two parts, each with
its own frame clock. The gathering, on the violet sky backdrop, lasts
`JUDGMENT_GATHER_FRAMES`: a projected formation of sixteen objects moves
forward while rotating motes shrink in on its centre, then a burst grows
there, shards fly from it and a ring of dimming motes spreads outward. The
sweep lasts `JUDGMENT_SWEEP_FRAMES`: the formation gives way to debris objects
and the scene cuts to an aerial backdrop, then to beach and forest ones,
swapping palettes, backdrop and debris on set frames. Meanwhile expanding
elliptical rings are drawn into a wave picture that persists in the work
block, explosion cells fan out from the impact, and the debris rises and is
recycled behind a scrolling explosion front. A or B skips ahead in both parts,
by different rules: the gathering simply ends after frame 4, while the sweep
jumps to its next transition frame (150, 214 or 280). The gathering's motes
are `PARTICLES` 0 to 127 and its sparks 128 to 255 (`JUDGMENT_SPARKS`); the
sweep's debris are particles 0 to 15, one for each debris object. The work
block is laid out as `struct JudgmentWork`: the sheet's 10,000 bytes of pixels,
then eight dimmed copies of the mote sheet. The sweep draws its 120x120 wave
picture from the start of the sheet, so it runs on over the dimmed copies,
which only the gathering needs.

**Spark bursts** (`plasma.c`). A spark burst is a large flash, a ring of small
flashes that fade, and a spray of sparks that fall and bounce on the ground.
The spark groups scene strikes one to three such bursts, 8 frames apart, and
the sword effects build their strikes from the same parts. A flash is an
`EffectStep` whose x and y are an offset from its burst's centre and whose
`variant` counts down its life; it is drawn only for its last 18 frames, from
a 32x64 flash picture chosen by its age. A spark moves in 16.16 fixed point
under its own gravity, and its `variant` is its remaining life; below the
floor it bounces back at half speed instead of being drawn, and it shrinks as
its life runs out. A strike makes every target flash and recoil at once. The
spark groups scene keeps its flashes in `work->particles`, sixteen a burst
(only the first twelve are drawn), and its sparks in `PARTICLES`, one run a
burst.

**Projectile volleys and rushing strikes** (`projectiles.c`). A projectile
volley fires a row of shots from the user at each target. The mode (0 to 11)
picks the shot picture, how the shots are aimed and which sounds play; the
mode's row gives its number of shots a target, their flight time and the
stagger between shots and targets. The mode rows keep their table indices,
and the wrappers name the mode of each ability. Every frame runs four steps in
order: each launched shot moves one step along its aim and is drawn on the
half-width canvas; a shot that passes its target's x on the target's side of
the field hits once, starting a hit flash, throwing sparks, shaking the camera
and making the target reel; comet volleys draw each hit flash for 12 frames,
as half a picture drawn again flipped for the other half; and the sparks
thrown by hits fall and bounce while those left behind fireballs rise. All of
these particles share `PARTICLES`: hit flashes from 0, sparks from 100 and
fireball trails from 512. A particle's `variant` is its frames left (0 when
free), and a flash's z counts the frames it has shown. The shots themselves
live in `work->particles`, one for each target and shot, whose `variant` is
set once the shot has hit. The rushing strike sends the user at the first
target; on frame 10 it strikes, a ring flares and 64 sparks scatter, over 60
frames in all.

**Stat reductions and the scenes after them** (`stat_change.c`). A stat
reduction shows the dagger or the shield over each target, 4 frames apart,
and breaks it into 24 shards that fly off and darken through seven dimmed
copies of the shard picture. Its shards live in `PARTICLES`, 64 for each
target, and are drawn with trails: each transfer of the canvas fades or
darkens what is already there. The scenes after it each throw a pattern of
particles and play it out over a fixed number of frames. Flare raises nine
flames one after another along a half circle, optionally stacked two or three
high. Quake raises three rock pillars that throw the targets into the air.
Fire's homing embers fly out of the user, hang, and then home in on the
targets, striking with a flare where they reach the ground. Frost's rising
motes spiral up around the target while ice columns spring up, then burst:
the motes spread and fall, chips fly and smoke rises over the heights they
span. The blast (Blast, Smoke Bomb) bursts a fountain of motes and puffs of
smoke up beside the targets, which fall back under gravity. A free or waiting
particle is usually marked by a negative `variant`, which otherwise counts the
frames of its animation, and `effect->variant` picks each scene's counts and
timing from the named columns of its table.

**Target strikes** (`target_strike.c`). One runner, `BattleFx_RunTargetStrike`,
plays all twelve styles of repeated slashes or columns of light on the first
target. Each style's row gives its kind (0 for a sideways slash, 1 for a
column of light), its number of hits, its extras (smoke, an absorbing drain,
glints, sparks or one heavy hit), its palette, the frames each picture is
held, the column's composite mode and the scene's duration; the wrappers above
the runner name the style of each effect. The setup loads the pictures and the
style's palette, then parks every particle, idle, where it will start: 64
smoke puffs at the target's feet, and 128 motes and 512 sparks 20 pixels up
its body, each with a random velocity. A particle is idle while its `variant`
is -1. Each frame first draws the strike (a slash sweep or a column) and, on a
hit frame, shakes and flashes the target and lights the next group of
particles; glints are then scattered around the target, and the lit smoke,
motes and sparks are drawn and moved. The finish releases the canvas layer.

## How to change an effect

1. Find the effect (above) and read its header: the frame timeline, the
   particle ranges and the pictures it loads.
2. Change it, then build and check the edition you changed:

   ```sh
   make tbs-en            # build the English ROM, out/tbs-en/tbs-en.gba
   make compare-editions  # for a matching change: all twelve must stay OK
   ```

   A mod changes the bytes on purpose, so it will not compare OK; keep the
   matching tree separate from your mod.
3. Keep within the limits below. An effect's particle counts are named where
   it sets them up: raise one and its loops follow. Steps that draw random
   numbers keep their order, or every later random value changes.

## How to add an effect

An ability reaches its effect through its row of
`games/tbs/src/battle/actflags.tsv`: the row number is the ability id, and
the `ability` column names it (a label the build skips). The battle reads the
row as one word, `effect` in its low half and `flags` above it
(`STAGING_*` in `battle/battle.c`):

- `effect` bits 0 to 11: the kind, an index into `gBattleFx_EffectHandlers`
  (`enum BattleFxKind` names them).
- `effect` bits 12 and 13: the variant, the ability's strength (Earthquake is
  `0x1001`, kind 1 variant 1). A double or triple blow adds one or two.
- `effect` bit 14: dispatched by id range (`BattleFx_DispatchByIdRange`);
  bit 15: a scene effect (`BattleFx_RunSceneEffect`), not a table kind.
- `flags`: the high half. Its bits 0 and 1 are `STAGING_ON_DOWNED` and
  `STAGING_OTHER_SIDE`, which no row sets; the rows use only bits 2 and 3
  (24 abilities), read outside the effects.

To give an ability a new effect, write its handler in a part, name a free kind
for it in `enum BattleFxKind`
([`effect.h`](../../include/unit/effect/effect.h); kinds without a handler are
free), put the handler in that kind's row of `gBattleFx_EffectHandlers`, and
set the ability's `effect` column to the kind. To reuse an effect, point the
column at an existing kind and variant.

## Limits

- The canvas is 128x128 pixels.
- `work->sheet` holds 0x6980 bytes, and an effect that does not use the
  scanline table after it (`bg2xLines`) may use up to 0x7080
  (`imageStorage`).
- The work block has 64 particles and `PARTICLES` about 1,750.
- There are two blitter slots, and each new picture size or mode needs its
  own blitter.

## Evidence and open questions

A handler's name comes from the abilities that reach its kind, read from
`actflags.tsv` and named as the English release names them; the families'
runners from what they draw; the blitter's words from the fragments it
joins. Kinds that no ability uses (kinds 34, 67 and 387, in `planet_diver.c`,
`bind.c` and `thrash.c`) are named from what they draw.

Still unknown:

- XXX: what reaches the three kinds that no ability uses.
- XXX: the `unk_` fields of the work block and the particles' spare words
  where an effect's use of them is not yet read.
- XXX: why `work->sheet` may grow into the scanline table only for effects
  that do not use it (0x7080 against 0x6980), and which effects do.
