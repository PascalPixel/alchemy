# Battle effects

This folder holds The Broken Seal's battle effects: everything drawn over the
battle while a Psynergy, a Djinni, a summon, an item or an enemy attack plays
out. Each effect is a function that takes the effect's `BattleEffectArgument`
(the caster, its targets and the effect's own values), plays a fixed number of
frames and returns when the effect is over.

## Files

The layout follows what the ROM shows of Camelot's own:

| File | What it holds |
| --- | --- |
| `effect.c` | all of the effect code: the entry points, every effect and the helpers they share |
| `effect_table.c` | the effects' tables: picture cells, sizes, offsets, frame lists, in the order the code uses them |
| `effect_bits.c` | the bit operands the compositing routines add to their widths |
| `battle_effect_composite.s`, `battle_effect_composite_color.s` | the hand-written ARM routines blitters are assembled from |
| `battle_effect_dot_product.s`, `battle_effect_expand.s` | vector maths and image expansion in ARM |

In the ROM, the effect bank is 150 KB of compiled code followed by one small
block of constants, then the tables. Camelot's other C files are each
followed by their own data, so the effects were very likely one large file,
with its tables kept in a separate data file in step with it. The effects
code also wrote its data without `const`: as `const`, 52 of the tables
change the generated code, and without it all of them match. Each function's
small constant vectors are `static` variables inside it, in the ROM in the
order of the functions that use them.

## Entry points

The battle starts an effect through one of three functions, each of which
allocates the effect's three heap blocks (sprite sheet, work block and
canvas), runs the effect and releases them:

- `BattleFx_RunTableEffect` looks the effect's kind up in
  `gBattleFx_EffectHandlers`, the table of ordinary effects.
- `BattleFx_RunSceneEffect` runs one of the large effects, those whose
  presentation flags have bit 15 set, chosen by its kind.
- `BattleFx_DispatchByIdRange` runs the casting impacts, beam sequences and
  ring scenes, which need a sprite sheet twice the usual size.

An effect may schedule callbacks that use those blocks, but it must remove
them before it returns, because the blocks are gone afterwards.

## The canvas

Most effects draw into one canvas: 128x128 pixels of 8-bit colour, stored as
16x16 tiles of 8x8 pixels. BG2 shows it, stretched to twice the width so that
it spans the screen; `BattleFx_BeginCanvasLayer` sets that up.

An effect never writes the canvas to video memory itself. It draws a frame,
sets the work block's `transferPending` and waits a frame. The scheduled
callback `BattleFx_ProcessPendingGraphicsTransfer` copies the canvas
to video memory and then treats the pixels it keeps according to the work
block's transfer mode (`enum CanvasTransferMode`): leave them, clear them,
fade them, darken or brighten them. Canvas pixels are indices into a palette
ramp, so fading the indices makes trails decay without any bookkeeping per
particle. Effects that draw a full-screen picture in `gMapCellBuffer` use BG1
instead, flushed by `BattleFx_FlushPendingGraphicsTransfer`.

## Blitters

Drawing a picture onto the canvas goes through a blitter, a small ARM routine
that `BattleFx_LoadWork` assembles at run time from the fragments in
`battle_effect_composite.s`, for one cell size, one mirroring, one clip and one
way of combining pixels (copy, additive, brightest wins and so on). A general
routine that tested all of these for every pixel would be far too slow. The
routines live in two heap slots, and an effect usually takes them as a
`BlitterPair`: `[0]` is the picture as drawn and `[1]` another mode or its
mirror, so one picture can be drawn on both sides of the screen.

## The frame loop

Every effect has the same shape. Its setup opens the canvas, builds the
blitters, loads the pictures and places the particles. Then it runs a fixed
number of frames, and finally releases what the setup took, the transfer
callback first.

Within a frame, steps each run over a range of frames, in a fixed order,
ending with the transfer. The order matters for two reasons: later steps draw
over earlier ones, and many steps draw random numbers, so moving a step
changes every random value after it.

## Particles

A particle is an `EffectStep`: a position and a velocity in three axes, 16.16
fixed point unless a comment says otherwise, and one spare word (`variant`)
that each effect uses as it needs: a life, a frame or a picture. The
particles live in `gMapCellBuffer`. Effects reserve ranges of it by index, and
a slot whose particle has finished is reused by the next one launched, so an
effect's pool size is fixed. `EffectStep_AdvanceWithGravity2D` and its kin
move particles under gravity, and `BattleUnit_ProjectToScreen` projects
battle positions onto the screen.
