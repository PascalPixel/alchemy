// Battle effects combine a software canvas, sprite reactions and scanline
// window or scroll tables. Each runner prepares its resources, advances the
// effect one frame at a time, then releases its callbacks and drawing buffers.
//
// The effect families below share one set of work slots. Ram_MapCellBuffer
// is scratch storage: different families give it different particle or image
// layouts. The individual runners document those layouts and remove their
// drawing callbacks before releasing the buffers.
//
// Pictures: the effects draw from decoded sheets, byte-per-pixel pictures
// whose pixel values are colour levels, indices into a palette ramp. A cell
// offset (the *_CELL and *_CELLS constants) is a byte offset into a decoded
// sheet, and each runner decodes its picture sets at fixed offsets in
// work->sheet. A fade draws from darker copies of a picture built before the
// first frame, and a symmetric picture is drawn left of its centre line and
// then mirrored to the right.
//
// Debris: several families break something apart. Boulders crumble into
// chips, shells burst into shards, an impact throws out fragments and
// Judgment lifts pieces of the field into the air. Each piece is an
// EffectStep: a 16.16 position, a 16.16 velocity and a variant the family
// uses as a lifetime, an image phase or an "alive" mark. Pieces move with
// EffectStep_AdvanceWithGravity2D or 3D and are drawn by their top-left
// corner, centred on their position or standing on it, as each family
// chooses. Spawning stays with each runner: every family draws its own
// random ranges, and the order of those draws is part of the effect.

#include "unit/effect/effect_table.h"
#include "unit/effect/effect.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/runtime/runtime.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "canvas.h"
#include "map_scroll.h"
#include "heap_state.h"
#include "menu_list.h"
#include "types.h"
#include "io_write_queue.h"
#include "io_reg.h"
#include "scene.h"
#include "global_cells.h"
#include "dma.h"
#include "resource_ids.h"
#include "battle_effect.h"
#include "fixed_math.h"
#include "callback_scheduler.h"
#include "effect_step.h"
#include "battle_effect_work.h"
#include "battle_display.h"
#include "ram_buffer.h"
#include "resource.h"
#include "b5_context.h"
#include "motobj.h"
#include "iwram_call.h"
#include "transform.h"
#include "battle_work.h"
#include "project.h"
#include "field_event.h"
#include "keystate.h"
#include "battle_type.h"
#include "sound_ids.h"
#include "battle_system.h"
#include "battle_session.h"
#include "animspr.h"
#include "battle_unit.h"
#include "fixed_point_position.h"
#include "irq.h"
#include "unit/effect/effect_bits.h"
#include "call.h"

// Display, palette and scanline effects.
//
// Effects change the screen around their drawing in three ways:
//
// - Display state: the windows that frame the battle view (lines 16 to
//   135), the blend and display registers, and the set-up every effect
//   starts and ends with (BattleFx_ConfigureEffectDisplay, below).
// - Per-frame callbacks the runner schedules: background scrolling, the
//   camera shake and the backdrop palette fades that make the canvas stand
//   out.
// - Per-scanline work: HBlank DMA that feeds a window edge, BG2's affine
//   reference or a palette row to the hardware line by line
//   (BattleFx_ArmWin0HBlankDma, BattleFx_ArmBg2AffineHBlankDma and
//   BattleFx_ArmPaletteHBlankDma), and the tables those transfers read.
//
// The callbacks read the current effect's work block, so the runner that
// schedules them must remove them before that block is released.

// The battle screen's display mode while an effect plays: background mode 1
// with BG0 to BG2, sprites and both windows on.
#define EFFECT_DISPCNT                                                         \
  (DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG1_ON |     \
   DISPCNT_BG2_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON | DISPCNT_WIN1_ON)

// Window 0, the battle view, shows BG0 to BG2, sprites and blending. Effects
// pick window 1's layers themselves.
#define EFFECT_WIN0_LAYERS                                                     \
  (WININ_WIN0_BG0 | WININ_WIN0_BG1 | WININ_WIN0_BG2 | WININ_WIN0_OBJ |         \
   WININ_WIN0_CLR)

// Outside the battle view: BG0 and blending only.
#define EFFECT_WINOUT (WINOUT_WIN01_BG0 | WINOUT_WIN01_CLR | WINOUT_WINOBJ_ALL)

// A work slot read through a pointer to the effect's own slot,
// gWorkSlot[HEAP_SLOT_BATTLE_EFFECT], or to the canvas slot just after it.
#define EFFECT_SLOT(slots, slot) ((slots)[(slot) - HEAP_SLOT_BATTLE_EFFECT])
#define CANVAS_SLOT(slots, slot) ((slots)[(slot) - HEAP_SLOT_BATTLE_CANVAS])

// Defines the battle effect kind `name` as a call of a shared runner with one
// fixed argument, its style or variant. The strike, volley, shower, mote and
// stat-change families each have one runner and a kind like this for each
// ability that uses it.
#define DEFINE_EFFECT_KIND(name, runner, arg)                                  \
  void name(struct BattleEffectArgument *effect) { runner(effect, arg); }

// Index of DMAxCNT_H, the control half, in a channel's 16-bit registers.
#define DMA_CONTROL_HALF 5

// DMA control word that copies one halfword per HBlank, repeating, with the
// destination reloaded each line: the scanline table that drives a window
// edge or a scroll register.
#define DMA_HBLANK_HALFWORD_PER_LINE 0xa2600001

// The same a word at a time: BG2's affine reference point.
#define DMA_HBLANK_WORD_PER_LINE 0xa6600001

// DMA control word that fills n words from one fixed source word.
#define DMA_FILL_WORDS(n) (0x85000000 | (n))

// Stops a DMA channel: clears its repeat and start timing bits, then its
// enable bit, and reads the control register back so the stop takes effect
// before the channel is set up again.
static __inline__ void Dma_Stop(volatile u16 *channel) {
  channel[DMA_CONTROL_HALF] &= 0xc5ff;
  channel[DMA_CONTROL_HALF] &= 0x7fff;
  (void)channel[DMA_CONTROL_HALF];
}

// How BattleFx_ProcessPendingGraphicsTransfer moves the effect's
// 128x128 canvas into VRAM once a frame (work->transferMode); the canvas
// pixels are indices into a palette ramp, so scaling them fades a trail.
enum CanvasTransferMode {
  CANVAS_COPY,             // copy as drawn
  CANVAS_COPY_AND_CLEAR,   // copy, then fill the canvas with transferValue
  CANVAS_COPY_AND_FADE,    // copy, keeping 50% or 75% (transferValue) of each
                           // pixel in the canvas for the next frame
  CANVAS_COPY_AND_DARKEN,  // copy, then darken the canvas by transferValue
  CANVAS_COPY_AND_BRIGHTEN // copy, then brighten the canvas by transferValue
};

// gBattleFxWork points at the effect's first work slot,
// HEAP_SLOT_BATTLE_EFFECT, which holds its work block; the canvas is in the
// slot after it. Effects read both by walking a cursor along the slots.
#define EFFECT_GET_WORK_AND_CANVAS(cursor, work, canvas)                       \
  ((work) = *(cursor)++, (canvas) = *(cursor))

// The same from the start: `slots` keeps gBattleFxWork for the other slots
// (EFFECT_SLOT) while `cursor` walks.
#define EFFECT_GET_SLOTS(slots, cursor, work, canvas)                          \
  ((slots) = gBattleFxWork, (cursor) = (slots),                                \
   EFFECT_GET_WORK_AND_CANVAS(cursor, work, canvas))

// Loops `i` over the effect's targets, effect->actors[0] to
// effect->actors[effect->count - 1]: the loops that flash, shake, strike or
// aim at each target in turn.
#define EFFECT_FOR_EACH_TARGET(i, effect)                                      \
  for ((i) = 0; (i) != (effect)->count; (i)++)

// Marks particles[0] to particles[count - 1] free: a particle whose variant
// is -1 is not drawn or moved until an effect gives it a life.
#define EFFECT_FREE_PARTICLES(i, particles, count)                             \
  for ((i) = 0; (i) != (count); (i)++)                                         \
  (particles)[i].variant = -1

// Starts a frame's 3D projection from the battle camera: resets the
// transform, then looks from camera->eye towards camera->pos. Effects that
// place pictures at battle positions do this before projecting them.
#define EFFECT_VIEW_FROM_CAMERA(camera)                                        \
  (Render_ResetTransformState(),                                               \
   Transform_SetLookAt((camera)->eye, (camera)->pos))

// Sets the x, y (up) and z of a three-part vector: a battle position about
// to be projected, a per-frame step or a scale.
#define EFFECT_SET_VECTOR(v, x, y, z) ((v)[0] = (x), (v)[1] = (y), (v)[2] = (z))

// Set the position or the velocity of a particle, a piece of debris or a
// battle object in one statement, as effects do when they launch one.
#define EFFECT_SET_POSITION(p, px, py, pz)                                     \
  ((p)->x = (px), (p)->y = (py), (p)->z = (pz))
#define EFFECT_SET_VELOCITY(p, vx, vy, vz)                                     \
  ((p)->velocityX = (vx), (p)->velocityY = (vy), (p)->velocityZ = (vz))

// The canvas life cycle, shared by every canvas effect. The statement macros
// are comma expressions, not do { } while (0) blocks: agscc compiles a
// do-while (0) in an effect's frame loop differently at some sites, and the
// comma form matches at all of them.

// Builds a blitter for the 128x128 canvas (1 << 7 pixels each way) in work
// slot `slot`: BattleFx_LoadWork's drawing routine for one clip, mirror and
// blend mode. Every canvas effect draws through one or two of these.
#define EFFECT_LOAD_BLITTER(slot, flags, mode)                                 \
  BattleFx_LoadWork(slot, 7, 7, flags, mode)

// Sets how the canvas transfer treats what is left in the canvas after each
// copy: `mode` with `value` (see enum CanvasTransferMode).
#define EFFECT_SET_TRANSFER(work, mode, value)                                 \
  ((work)->transferMode = (mode), (work)->transferValue = (value))

// Sets the transfer, then starts copying the canvas to VRAM once a frame.
#define EFFECT_START_TRANSFER(work, mode, value)                               \
  (EFFECT_SET_TRANSFER(work, mode, value),                                     \
   Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer,      \
                                 0x480))

// Ends a frame of a canvas effect: marks the canvas ready for the transfer
// callback, then waits for the next frame. Nearly every effect loop ends so.
#define EFFECT_PRESENT_FRAME(work) ((work)->transferPending = 1, WaitFrames(1))

// The usual end of an effect frame: counts down the targets' flash timers
// (ObjectGroup_TickMemberTimers), then presents the canvas.
#define EFFECT_END_FRAME(work)                                                 \
  (ObjectGroup_TickMemberTimers(), EFFECT_PRESENT_FRAME(work))

// Frees the two blitters, the alternate first.
#define EFFECT_RELEASE_BLITTERS()                                              \
  (Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE),                      \
   Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER))

// The usual end of a canvas effect: stops the transfer callback before
// freeing what it reads, then the blitters, then the canvas layer.
#define EFFECT_FINISH_CANVAS()                                                 \
  (Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer),          \
   EFFECT_RELEASE_BLITTERS(), BattleFx_EndCanvasLayer())

// The same for an effect that drew through one blitter.
#define EFFECT_FINISH_CANVAS_ONE_BLITTER()                                     \
  (Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer),          \
   Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER), BattleFx_EndCanvasLayer())

// Particle sheets hold ten mote cells, 1x2 to 10x20 pixels. Draws mote cell
// `size` (size pixels wide, twice that tall) of `sheet` through blitter
// `draw`, centred across x and standing on y: the motes, sparks and dust of
// 32 effects, from the mote fountain to the summons.
#define EFFECT_DRAW_MOTE(draw, canvas, sheet, x, y, size)                      \
  (draw)(canvas, (sheet) + gBattleFx_MoteCellOffsets[(size) - 1],              \
         (x) - (size) / 2, (y) - (size), size, (size) * 2)

// Draws flare cell `size` of `sheet`, a square `size` pixels from its centre
// to each edge, centred on (x, y): Douse's sparks and the glowing points of
// the ice fleck and gathering bursts, stat boosts, beams and rings.
#define EFFECT_DRAW_FLARE(draw, canvas, sheet, x, y, size)                     \
  (draw)(canvas, (sheet) + gBattleFx_FlareCells[(size) - 1], (x) - (size),     \
         (y) - (size), (size) * 2, (size) * 2)

// Shows the canvas: the effect display mode, BG2 stretched to twice its width
// (the 128-pixel canvas shown 256 pixels wide) and blended over everything
// below with `top` sixteenths of its colour.
#define EFFECT_SHOW_WIDE_CANVAS(top)                                           \
  (REG_DISPCNT = EFFECT_DISPCNT, REG_BG2PA = Q_8_8(0.5),                       \
   REG_BLDALPHA = BLEND_ALPHA(top, 16),                                        \
   REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL)

// The two halves of a WIN0H or WIN1H value: the window's left edge and its
// right edge (exclusive) on one scanline.
#define WINDOW_SPAN(left, right) (((left) << 8) | (right))

// A WIN0H value whose left edge is past its right edge: nothing of the line
// is inside the window.
#define WINDOW_SPAN_EMPTY WINDOW_SPAN(1, 0)

// Keeps `v` within low to high, the low bound tested first: a screen depth,
// a colour level or a clipped coordinate. A plain block, as a do-while (0)
// compiles differently at some of its sites.
#define EFFECT_CLAMP(v, low, high)                                             \
  {                                                                            \
    if ((v) < (low))                                                           \
      (v) = (low);                                                             \
    if ((v) > (high))                                                          \
      (v) = (high);                                                            \
  }

// Used for clipping canvas lines and comparing projectile travel distances.
#define EFFECT_ABS(v) ((v) < 0 ? -(v) : (v))

// Closing bolts and lightning columns use the same 24x120 picture cells.
#define LIGHTNING_BOLT_CELL_BYTES 2880

// Windows 0 and 1 both cover the battle view: the full width, lines 16 to
// 135.
#define EFFECT_SET_BATTLE_WINDOWS()                                            \
  (REG_WIN0H = WIN_RANGE(0, 240), REG_WIN0V = WIN_RANGE(16, 136),              \
   REG_WIN1H = WIN_RANGE(0, 240), REG_WIN1V = WIN_RANGE(16, 136))

// BG2 shows the canvas from screen line 16 down (BG2Y is -16.0), stretched to
// twice its width: the 128x128 canvas across the battle view.
#define EFFECT_SET_CANVAS_AFFINE()                                             \
  (REG_BG2X = 0, REG_BG2Y = -0x1000, REG_BG2PA = Q_8_8(0.5), REG_BG2PB = 0,    \
   REG_BG2PC = 0, REG_BG2PD = Q_8_8(1))

// Writes one row of BG2's map for the canvas: its 16 tiles of row `row` in
// order. An affine map has one byte per tile, so they are written two at a
// time, eight halfwords from `offset` on.
#define EFFECT_WRITE_CANVAS_MAP_ROW(row, column, offset)                       \
  for ((column) = 0; (column) != 8; (column)++, (offset) += 2) {               \
    s32 tileIndex;                                                             \
    s16 pair;                                                                  \
                                                                               \
    tileIndex = (row) * 16 + (column) * 2;                                     \
    pair = ((tileIndex + 1) << 8) | tileIndex;                                 \
    *(volatile s16 *)((u8 *)BG_CANVAS_MAP + (offset)) = pair;                  \
  }

// Puts the battle screen back into the display state every effect starts
// from and ends with: no blending, window 0 and window 1 both covering the
// battle view (lines 16 to 135), and the effect display mode queued for the
// next frame. Waits one frame.
void BattleFx_ConfigureEffectDisplay(void) {
  // Restore the battle viewport before the next effect changes its layers.
  REG_BLDCNT = 0;
  REG_BLDALPHA =
      BLEND_ALPHA(14, 16); // 14/16 of the top layer over all of the bottom
  EFFECT_SET_BATTLE_WINDOWS();
  // Window 1 starts as window 0 without BG1.
  REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG2 |
              WININ_WIN1_OBJ | WININ_WIN1_CLR;
  REG_WINOUT = EFFECT_WINOUT;
  // Queues the effect display mode, 2 frames ahead.
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, EFFECT_DISPCNT, 2 << 16);
  WaitFrames(1);
}

// Scheduler callback: every scrollInterval frames, scrolls background 1 by
// (scrollStepX, scrollStepY).
void BattleFx_AdvanceScrollOnInterval(void) {
  struct BattleEffectWork *work = WORK_SLOT(HEAP_SLOT_BATTLE_EFFECT);
  u32 *timer = (u32 *)&work->scrollTimer;

  (*timer)++;
  if (*timer == work->scrollInterval) {
    gBgScroll[1].x += work->scrollStepX;
    gBgScroll[1].y += work->scrollStepY;
    *timer = 0;
  }
}

// Scheduler callback: every scrollInterval frames, writes the effect's
// background 2 reference point to the hardware and then moves it by
// (scrollStepX, scrollStepY), so the affine layer slides in steps.
void Camera_AdvanceBg2Reference(void) {
  s32 timer;
  struct BattleEffectWork *work;

  work = WORK_SLOT(HEAP_SLOT_BATTLE_EFFECT);
  timer = work->scrollTimer + 1;
  work->scrollTimer = timer;
  if (timer == work->scrollInterval) {
    // Display the current reference before advancing to the next one.
    REG_BG2X = work->bg2x;
    REG_BG2Y = work->bg2y;
    work->bg2x += work->scrollStepX;
    work->bg2y += work->scrollStepY;
    work->scrollTimer = 0;
  }
}

// Scheduler callback: restarts the HBlank DMA that copies one WIN0H value per
// scanline from gMapCellBuffer, so window 0's edges follow the per-line table
// the effect drew there (a slanted or shaped window).
void BattleFx_ArmWin0HBlankDma(void) {
  volatile u16 *channel = REG_DMA0;
  // Stop the preceding scanline transfer before replacing its source.
  Dma_Stop(channel);
  Dma_Set(gMapCellBuffer, (void *)&REG_WIN0H, DMA_HBLANK_HALFWORD_PER_LINE,
          (volatile u32 *)channel);
}

// Fills window 0's per-scanline edges for the falling shards and bolts:
// lines 8 to `last` take `slant`, an expression of the line `i`; the rest of
// the battle view, down to line 135, takes the fixed `span`; and the lines
// below it are empty.
#define EFFECT_BUILD_SLANTED_WINDOW(line, i, last, slant, span)                \
  for ((i) = 0; (i) != 160; (i)++) {                                           \
    if ((i) >= 8 && (i) <= (last))                                             \
      (line)[i] = (slant);                                                     \
    else if ((i) <= 135)                                                       \
      (line)[i] = (span);                                                      \
    else                                                                       \
      (line)[i] = WINDOW_SPAN_EMPTY;                                           \
  }

// Falling shards and bolts, streak volleys and the mote fountain: four effects
// that throw many small pictures at the targets.
//
// - The falling shards (Ice) and bolts (Douse) rain diagonally onto the
//   targets' side. Both show the canvas only inside a slanted band of
//   window 0, whose edges an HBlank DMA feeds line by line from a table at
//   the start of gMapCellBuffer. Each particle falls until it passes y 80,
//   then lands: shards shatter and kick up sparkle puffs, bolts flash and
//   throw a fountain of sparks. A small timing table per effect picks the
//   count, spacing and duration from effect->variant.
// - The streak volley fires three streaks of light from the user at each
//   target in turn; a settings row per style says whether the streak
//   animates and how a hit looks.
// - The mote fountain bursts motes out of the user that float upwards and
//   wobble while each target reacts.
//
// Particle slots: the shards use work->particles, 0 to 31 for the shards and
// 32 to 63 for their puffs; the bolts use work->particles for the bolts and
// gMapCellBuffer, after the window table, for their sparks; the streaks use
// work->particles, one per target and streak; the motes use PARTICLES. A
// falling particle's variant is -1 until it lands, then counts the frames
// since. Shards and bolts are positioned in eighths of a pixel; sparks,
// streaks and motes in 16.16 fixed point.
enum { FALLING_SHARD_COUNT = 32, FALLING_SHARD_PUFF_COUNT = 32 };

// gIce_Counts has two entries for each variant.
#define ICE_SHARD_COUNT(variant) gIce_Counts[(variant) * 2]
#define ICE_FRAMES(variant) gIce_Counts[(variant) * 2 + 1]
// A falling shard or bolt keeps falling while its y (in eighths of a pixel)
// is at most this, just above 80 pixels.
#define FALLING_LAND_Y 0x27f

// Abilities: Ice, Ice Horn, Ice Missile (kind 7).
//
// Battle effect 7: ice shards rain down diagonally onto the targets' side of
// the field and shatter into puffs of sparkle, flashing and shaking each
// target as they land. The canvas shows only inside a slanted band of
// window 0, so the shards stream out of the sky above the targets.
// effect->variant picks the size of the volley from gIce_Counts
// (shards and frames: 16 for 46 frames, 20 for 52, 32 for 80); variant 1
// leaves longer trails and variant 2 draws additively.
// effect->side is 1 when the targets are the party, 0 when they are enemies.
void BattleFx_RunIce(struct BattleEffectArgument *effect) {
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  u8 *sparkles;
  BlitterPair draw;
  struct EffectStep *shard;
  struct EffectStep *puff;
  u16 *line;
  s32 i;
  s32 k;

  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  sparkles = EFFECT_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  work->effect = effect;
  // Load the shard and puff images before shaping their visible canvas.
  BattleFx_BeginCanvasLayer(0x2001);
  REG_BG2PA = Q_8_8(1);
  Resource_LoadAndDecompress((s32)&ResourceId_IceShardSheet, work->sheet, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_SparkleDots, sparkles, 0, 0);
  BattleFx_ConfigureEffectDisplay();
  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  // window 1 hides BG2
  REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG1 |
              WININ_WIN1_OBJ | WININ_WIN1_CLR;

  // Line the shards up above the screen, each one 8 pixels higher and
  // further back along the slant than the one before.
  {
    s32 y;
    s32 slant;

    for (i = 0, y = -128, slant = -16; i != FALLING_SHARD_COUNT;
         y -= 64, i++, slant -= 8) {
      s32 x;

      shard = &work->particles[i];
      x = (Random16() & 63) + (Random16() & 7) + 24;
      if (work->effect->side == 1)
        x = x + slant + 24;
      else
        x = x - slant + 80;
      shard->y = y;
      shard->x = x << 3;
      shard->variant = -1; // still falling
    }
  }
  for (i = 0; i != FALLING_SHARD_PUFF_COUNT; i++)
    work->particles[FALLING_SHARD_COUNT + i].variant = -1; // puff slot free

  // The shards fall towards the targets' side, so the blitters mirror them
  // when the targets are the party.
  if (work->effect->side == 0) {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, COMPOSITE_CLIP_Y, COMPOSITE_MAX);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE, COMPOSITE_CLIP_Y,
                        COMPOSITE_ADDITIVE);
  } else {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X,
                        COMPOSITE_MAX);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                        COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X,
                        COMPOSITE_ADDITIVE);
  }
  draw[0] =
      (DrawRectangle)((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BLITTER];
  draw[1] = (DrawRectangle)((union HeapState *)gWorkSlot)
                ->slots[HEAP_SLOT_BLITTER_ALTERNATE];

  // Window 0's edges, one per scanline: a band slanting the way the shards
  // fall, then a fixed span down to line 135, and nothing below that.
  if (work->effect->side == 0) {
    line = (u16 *)gMapCellBuffer;
    EFFECT_BUILD_SLANTED_WINDOW(line, i, 103, (0xf0 - i) | ((0x70 - i) << 8),
                                WINDOW_SPAN(8, 136));
  } else {
    line = (u16 *)gMapCellBuffer;
    EFFECT_BUILD_SLANTED_WINDOW(line, i, 95, (i - 8 + 160) | ((i + 0x18) << 8),
                                WINDOW_SPAN(120, 248));
  }

  Scheduler_AddOrUpdateCallback(BattleFx_ArmWin0HBlankDma, 0x480);
  work->transferMode = CANVAS_COPY_AND_FADE;
  if (work->effect->variant == 1)
    work->transferValue = 75;
  else
    work->transferValue = 50;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);

  for (frame = 0; frame != ICE_FRAMES(work->effect->variant); frame++) {
    // 16 frames before the end, the battle shows the damage, with sound
    // 132 as each target is hit.
    if (frame == ICE_FRAMES(work->effect->variant) - 16)
      BattleEventRuntime_BeginPhaseFar(132);

    // Shards: fall 8 pixels a frame down the slant; on landing, shatter
    // through five images, kicking up puffs in the middle of it.
    for (i = 0; i != ICE_SHARD_COUNT(work->effect->variant); i++) {
      shard = &work->particles[i];
      if (shard->variant == -1) {
        s32 x = shard->x / 8;
        s32 y = shard->y / 8;

        draw[work->effect->variant == 2 ? 1 : 0](canvas, work, x, y, 32, 32);
        if (shard->y <= FALLING_LAND_Y) { // above y 80
          if (work->effect->side == 0)
            shard->x -= 64;
          else
            shard->x += 64;
          shard->y += 64;
        } else {
          // Landed: every fourth shard plays the impact sound, and every
          // target flashes blue in its damage pose for 8 frames.
          if ((i & 3) == 0)
            AudioCommand_PlayFar(115);
          work->shakeFrames = 2;
          shard->variant = 0;
          EFFECT_FOR_EACH_TARGET(k, work->effect)
            ObjectGroup_UpdateMembers(work->effect->actors[k],
                                      SPRITE_COLOR_BLUE_TINT, POSE_DAMAGE, k,
                                      8);
        }
      }
      if (shard->variant != -1) {
        s32 x = shard->x / 8;
        s32 y = shard->y / 8;

        // Shattering: frames 1 to 14 step through the sheet's 32x32 images
        // 1 to 5.
        if (shard->variant >= 1 && shard->variant <= 14) {
          draw[work->effect->variant == 2 ? 1 : 0](
              canvas, work->sheet + 0x400 + ((shard->variant / 3) << 10), x, y,
              32, 32);
        }
        // Frames 9 to 11 each start a puff in the first free slot, near
        // the shard.
        if (shard->variant >= 9 && shard->variant <= 11) {
          for (k = 0; k != FALLING_SHARD_PUFF_COUNT; k++) {
            puff = &work->particles[FALLING_SHARD_COUNT + k];
            if (puff->variant == -1) {
              puff->variant = 18;
              puff->x = (((Random16() & 31) + shard->x / 8) << 3) + 8;
              puff->y = ((Random16() & 15) + shard->y / 8 - 15) << 3;
              break;
            }
          }
        }
        if (shard->variant <= 14)
          shard->variant++;
      }
    }

    // Puffs: count down from 18, two frames on each of the nine sparkle
    // images (8 down to 0), centred on the puff's position.
    for (i = 0; i != FALLING_SHARD_PUFF_COUNT; i++) {
      puff = &work->particles[FALLING_SHARD_COUNT + i];
      if (puff->variant != -1) {
        if (puff->variant <= 17) {
          s32 image = puff->variant / 2;
          s32 x = puff->x / 8 - (gBattleFx_PuffSizes[image] >> 1);
          s32 y = puff->y / 8 - (gBattleFx_PuffSizes[image] >> 1);

          draw[work->effect->variant == 2 ? 1 : 0](
              canvas, sparkles + gBattleFx_PuffCells[image], x, y,
              gBattleFx_PuffSizes[image], gBattleFx_PuffSizes[image]);
        }
        if (puff->variant > -1)
          puff->variant--;
      }
    }
    ObjectGroup_TickMemberTimers();
    Camera_ApplyShake(4, 4);
    EFFECT_PRESENT_FRAME(work);
  }

  // Tear down in the reverse order.
  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  Scheduler_RemoveCallback(BattleFx_ArmWin0HBlankDma);
  EFFECT_RELEASE_BLITTERS();
  BattleFx_EndCanvasLayer();
  BattleFx_ConfigureEffectDisplay();
}

// The falling bolts keep their sparks after window 0's scanline table in
// gMapCellBuffer.
enum { FALLING_BOLT_COUNT = 64, FALLING_BOLT_SPARK_COUNT = 512 };

struct BoltSparkBuffer {
  u16 lines[160];
  struct EffectStep sparks[FALLING_BOLT_SPARK_COUNT];
};

#define FALLING_BOLT_SPARKS ((struct BoltSparkBuffer *)Ram_MapCellBuffer)

// gFallingBolts_Counts has four entries for each variant.
#define FALLING_BOLT_WAVE_COUNT(variant) gFallingBolts_Counts[(variant) * 4]
#define FALLING_BOLT_SPARKS_PER_BOLT(variant)                                  \
  gFallingBolts_Counts[(variant) * 4 + 1]
#define FALLING_BOLT_SPACING(variant) gFallingBolts_Counts[(variant) * 4 + 2]
#define FALLING_BOLT_DURATION(variant) gFallingBolts_Counts[(variant) * 4 + 3]
#define DOUSE_FALLING_CELL 0x604 // the falling bolt picture in the sheet
#define DOUSE_FLASH_CELL 0x844   // the landing flash picture in the sheet
#define DOUSE_BURST_CELL 0xa84   // the 42x42 burst picture in the sheet

// Abilities: Douse, Drench, Deluge (kind 9).
//
// Battle effect 9: blue bolts streak down diagonally onto the targets' side
// of the field, each bursting into a fountain of falling sparks where it
// lands, while the targets flash. Like the falling shards, the canvas shows
// only inside a slanted band of window 0.
// effect->variant picks the volley from gFallingBolts_Counts: 6 bolts of 8
// sparks, 8 of 16 or 32 of 4, spaced 32, 24 or 8 pixels apart, over 80
// frames; it also picks how the canvas fades (see below).
// effect->side is 1 when the targets are the party, 0 when they are enemies.
void BattleFx_RunDouse(struct BattleEffectArgument *effect) {
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle drawSpark;
  DrawRectangle drawBolt;
  struct EffectStep *bolt;
  struct EffectStep *spark;
  u16 *line;
  s32 i;
  s32 k;

  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  work->effect = effect;
  // The beam and its sparks use separate images but the same clipped canvas.
  BattleFx_BeginCanvasLayer(0x2001);
  REG_BG2PA = Q_8_8(1);
  Resource_LoadAndDecompress((s32)&ResourceId_BlueBeamSheet,
                             work->sheet + DOUSE_FALLING_CELL, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD, work->sheet, 0,
                             0);
  BattleFx_ConfigureEffectDisplay();
  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  // window 1 hides BG2
  REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG1 |
              WININ_WIN1_OBJ | WININ_WIN1_CLR;

  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, COMPOSITE_CLIP_Y, COMPOSITE_MAX);
  drawBolt = EFFECT_SLOT(slots, HEAP_SLOT_BLITTER);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE, COMPOSITE_CLIP_Y,
                      COMPOSITE_ADDITIVE);
  drawSpark = EFFECT_SLOT(slots, HEAP_SLOT_BLITTER_ALTERNATE);

  EFFECT_FREE_PARTICLES(i, FALLING_BOLT_SPARKS->sparks,
                        FALLING_BOLT_SPARK_COUNT);

  // Line the bolts up above the screen along the slant, FALLING_BOLT_SPACING
  // apart.
  for (i = 0; i != FALLING_BOLT_COUNT; i++) {
    s32 x;
    s32 y;

    bolt = &work->particles[i];
    x = Random16() & 63;
    y = -(i * FALLING_BOLT_SPACING(work->effect->variant) + 16);
    if (work->effect->side == 1)
      x = x + y / 2 - 48;
    else
      x = x - y / 2 + 72;
    bolt->x = x << 3;
    bolt->y = y << 3;
    bolt->variant = -1; // still falling
  }

  // Window 0's edges, one per scanline: a band slanting the way the bolts
  // fall, then a fixed span down to line 135, and nothing below that.

  if (work->effect->side == 0) {
    line = (u16 *)gMapCellBuffer;
    EFFECT_BUILD_SLANTED_WINDOW(line, i, 103,
                                ((0x34 - i / 2) << 8) | (0xb4 - i / 2),
                                WINDOW_SPAN(0, 128));
  } else {
    line = (u16 *)gMapCellBuffer;
    EFFECT_BUILD_SLANTED_WINDOW(line, i, 103,
                                ((i / 2 + 60) << 8) | (i / 2 + 188),
                                WINDOW_SPAN(112, 240));
  }

  Scheduler_AddOrUpdateCallback(BattleFx_ArmWin0HBlankDma, 0x480);
  // Variant 0 clears the canvas every frame; the others leave trails.
  if (work->effect->variant == 0) {
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  } else if (work->effect->variant == 1) {
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, 50);
  } else {
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);
  }
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);

  for (frame = 0; frame != FALLING_BOLT_DURATION(work->effect->variant);
       frame++) {
    // 64 frames before the end, the battle shows the damage, with sound
    // 132 as each target is hit.
    if (frame == FALLING_BOLT_DURATION(work->effect->variant) - 64)
      BattleEventRuntime_BeginPhaseFar(132);

    // Bolts: fall 8 pixels a frame (4 across) until they pass y 80, then
    // burst and play a two-image flash.
    for (i = 0; i != FALLING_BOLT_WAVE_COUNT(work->effect->variant); i++) {
      s32 x;
      s32 y;

      bolt = &work->particles[i];
      x = bolt->x / 8;
      y = bolt->y / 8;
      if (bolt->variant == -1) {
        drawBolt(canvas, work->sheet + DOUSE_FALLING_CELL, x, y, 24, 24);
        if (bolt->y <= FALLING_LAND_Y) {
          if (work->effect->side == 0)
            bolt->x -= 32;
          else
            bolt->x += 32;
          bolt->y += 64;
        } else {
          // Landed: throw this bolt's sparks out from its tip, mostly
          // upwards, each living 16 to 31 frames.
          bolt->variant = 0;
          for (k = 0; k != FALLING_BOLT_SPARKS_PER_BOLT(work->effect->variant);
               k++) {
            spark =
                &FALLING_BOLT_SPARKS->sparks[i * FALLING_BOLT_SPARKS_PER_BOLT(
                                                     work->effect->variant) +
                                             k];
            spark->x = (x + 12) << 16;
            spark->y = y << 16;
            spark->velocityX = ((Random16() & 255) - 128) << 9;
            if (work->effect->variant == 2)
              spark->velocityY = ((Random16() & 0x1ff) - 0x180) << 10;
            else
              spark->velocityY = ((Random16() & 255) - 255) << 10;
            spark->variant = (Random16() & 15) + 16;
          }
          // Every fourth bolt plays the hit sound, and every target
          // flashes white in its damage pose for 2 frames.
          if ((i & 3) == 0)
            AudioCommand_PlayFar(132);
          EFFECT_FOR_EACH_TARGET(k, work->effect)
            ObjectGroup_UpdateMembers(work->effect->actors[k],
                                      SPRITE_COLOR_WHITE, POSE_DAMAGE, k, 2);
        }
      } else {
        if (bolt->variant >= 0 && bolt->variant <= 3)
          drawBolt(canvas, work->sheet + DOUSE_FLASH_CELL, x, y, 24, 24);
        else if (bolt->variant <= 7)
          drawBolt(canvas, work->sheet + DOUSE_BURST_CELL, x - 9, y - 9, 42,
                   42);
        if (bolt->variant <= 14)
          bolt->variant++;
      }
    }
    // Sparks: a dot that shrinks as its life runs out, falling under
    // gravity.
    for (i = 0; i != FALLING_BOLT_SPARK_COUNT; i++) {
      spark = &FALLING_BOLT_SPARKS->sparks[i];
      if (spark->variant != -1) {
        s32 size = spark->variant + 1;

        if (size > 6)
          size = 6;
        EFFECT_DRAW_FLARE(drawSpark, canvas, work->sheet, HI(spark->x),
                          HI(spark->y), size);
        EffectStep_AdvanceWithGravity2D(spark, 60, 0x2000);
        spark->variant--;
      }
    }
    EFFECT_END_FRAME(work);
  }

  // Stop both transfers before releasing the blitters and canvas.
  Scheduler_RemoveCallback(BattleFx_ArmWin0HBlankDma);
  EFFECT_FINISH_CANVAS();
  BattleFx_ConfigureEffectDisplay();
}

// Streak volleys: three streaks of light fly from the user to each target in
// turn, in one of five colours.

// gStreakVolley_Records has four flags for each style.
#define STREAK_VALUE(mode, n) gStreakVolley_Records[(mode) * 4 + (n)]
#define STREAK_ANIMATE 0 // nonzero: the streak cycles through three images
#define STREAK_HIT 1     // nonzero: play the hit sound and flash the target
#define STREAK_MOTION 2  // the target's recoil motion on the hit (-1: none)
#define STREAK_SHAKE 3 // nonzero: the struck target trembles from side to side

#define STREAK_MAX 3 // streaks fired at each target

enum StreakStyle {
  STREAK_STYLE_EMBER,
  STREAK_STYLE_LIME,
  STREAK_STYLE_VIOLET,
  STREAK_STYLE_RED,
  STREAK_STYLE_CYAN
};

// Abilities: Sonic Wave (kind 300).
//
// Battle effect 300: ember streaks.
DEFINE_EFFECT_KIND(BattleFx_RunSonicWave, BattleFx_RunStreakVolley,
                   STREAK_STYLE_EMBER)

// Abilities: Shriek (kind 301).
//
// Battle effect 301: lime streaks.
DEFINE_EFFECT_KIND(BattleFx_RunShriek, BattleFx_RunStreakVolley,
                   STREAK_STYLE_LIME)

// Abilities: Banshee Howl (kind 302).
//
// Battle effect 302: violet streaks.
DEFINE_EFFECT_KIND(BattleFx_RunBansheeHowl, BattleFx_RunStreakVolley,
                   STREAK_STYLE_VIOLET)

// Abilities: War Cry (kind 304).
//
// Battle effect 304: cyan streaks.
DEFINE_EFFECT_KIND(BattleFx_RunWarCry, BattleFx_RunStreakVolley,
                   STREAK_STYLE_CYAN)

// Abilities: Wicked Howl (kind 305).
//
// Battle effect 305: red streaks.
DEFINE_EFFECT_KIND(BattleFx_RunWickedHowl, BattleFx_RunStreakVolley,
                   STREAK_STYLE_RED)

// The streak volley in lime. UNUSED: no effect table entry points here.
UNUSED void BattleFx_RunLimeStreaksAgain(struct BattleEffectArgument *effect) {
  BattleFx_RunStreakVolley(effect, STREAK_STYLE_LIME);
}

#define STREAK_SPACING 6       // frames between streaks
#define STREAK_FLIGHT 24       // frames from user to target
#define STREAK_TARGET_DELAY 32 // frames between targets

// Fires STREAK_MAX streaks at each target from 20 pixels above the user,
// one every 6 frames, each reaching 20 pixels above its target 24 frames
// later; each target's volley starts 32 frames after the previous one's.
// A streak image is 20x40 and drawn twice, mirrored upwards, as a 20x80
// streak that wavers from side to side and leaves a fading trail.
// mode : the style, 0 to 4: the streak colour (ember, lime, violet, red,
//        cyan) and the row of gStreakVolley_Records that says how a hit looks.
void BattleFx_RunStreakVolley(struct BattleEffectArgument *effect, s32 mode) {
  struct EffectPosition position;
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 k;
  DrawRectangle drawLower;
  DrawRectangle drawUpper;
  struct BattleCamera *camera;
  struct MotionObject *source;
  struct MotionObject *target;
  s32 resource;
  s32 frame;
  s32 streakIndex;
  s32 streaks = STREAK_MAX;
  s32 lift = 20 << 16; // 20 pixels, 16.16

  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  camera = EFFECT_SLOT(slots, HEAP_SLOT_CAMERA);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(1);

  // The upper half of a streak is its image flipped vertically; on the
  // party's side both halves are mirrored as well.
  if (work->effect->side == 0) {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y, COMPOSITE_MAX);
    drawUpper = EFFECT_SLOT(slots, HEAP_SLOT_BLITTER);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE, EFFECT_CLIP_BOUNDS,
                        COMPOSITE_MAX);
    drawLower = EFFECT_SLOT(slots, HEAP_SLOT_BLITTER_ALTERNATE);
  } else {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP,
                        COMPOSITE_MAX);
    drawUpper = EFFECT_SLOT(slots, HEAP_SLOT_BLITTER);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_MAX);
    drawLower = EFFECT_SLOT(slots, HEAP_SLOT_BLITTER_ALTERNATE);
  }

  // The streak images, and the background palette for the style's colour.
  Resource_LoadAndDecompress((s32)&ResourceId_FiveModeImage, work->sheet, 0, 0);
  switch (mode) {
  case STREAK_STYLE_EMBER:
    resource = (s32)&ResourceId_EmberStreakSheet;
    break;
  case STREAK_STYLE_LIME:
    resource = (s32)&ResourceId_LimePalette;
    break;
  case STREAK_STYLE_VIOLET:
    resource = (s32)&ResourceId_VioletPaletteD;
    break;
  case STREAK_STYLE_RED:
    resource = (s32)&ResourceId_RedPaletteB;
    break;
  case STREAK_STYLE_CYAN:
  default:
    resource = (s32)&ResourceId_CyanPalette;
    break;
  }
  Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(resource), 128);
  work->transferMode = CANVAS_COPY_AND_FADE; // halve the canvas each frame
  work->transferValue = 50;                  // so streaks leave a trail
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);

  source = GetBattleObjectSlotFar(work->effect->actor)->object;
  {
    s32 totalFrames = streaks * work->effect->count * STREAK_SPACING + 48;

    // Every streak starts 20 pixels above the user and moves in a straight
    // line to 20 pixels above its target over 24 frames.
    EFFECT_FOR_EACH_TARGET(k, work->effect) {
      target = GetBattleObjectSlotFar(work->effect->actors[k])->object;
      for (streakIndex = 0; streakIndex != streaks; streakIndex++) {
        struct EffectStep *streak = &work->particles[k * streaks + streakIndex];

        EFFECT_SET_POSITION(streak, source->x, source->y + lift, source->z);
        EFFECT_SET_VELOCITY(streak, (target->x - streak->x) / STREAK_FLIGHT,
                            (target->y + lift - streak->y) / STREAK_FLIGHT,
                            (target->z - streak->z) / STREAK_FLIGHT);
        streak->variant = 0; // frames since the streak set off
      }
    }

    for (frame = 0; frame != totalFrames; frame++) {
      s32 *row = work->bg2X;

      // A sine wave of BG2X offsets, one per scanline, for the wave
      // HBlank DMA; this effect does not start that DMA itself.
      for (k = 0; k != 160; k++)
        *row++ = (0x80000 - Trig_Sin((frame + k) << 12) * 2) >> 10;
      // Fade the canvas out over the last 16 frames.
      if (frame > totalFrames - 16)
        REG_BLDALPHA = (totalFrames - frame) | 0x1000;
      EFFECT_VIEW_FROM_CAMERA(camera);
      EFFECT_FOR_EACH_TARGET(k, work->effect) {
        if (frame >= k * STREAK_TARGET_DELAY) {
          for (streakIndex = 0; streakIndex != streaks; streakIndex++) {
            if (frame >=
                k * STREAK_TARGET_DELAY + streakIndex * STREAK_SPACING) {
              struct EffectStep *streak =
                  &work->particles[k * streaks + streakIndex];
              s32 step;

              // Project the streak onto the half-width canvas; its image
              // changes every 8 frames and stays on the sixth.
              EffectPosition_ApplyBaseAndYOffset(streak, &position);
              position.x >>= 1;
              step = streak->variant / 8;
              if (step > 5)
                step = 5;
              if (STREAK_VALUE(mode, STREAK_ANIMATE)) {
                u8 *cell = &work->sheet[step * 800 + frame / 2 % 3 * 4800];

                drawUpper(canvas, cell, position.x - 10, position.y - 40, 20,
                          40);
                drawLower(canvas, cell, position.x - 10, position.y, 20, 40);
              } else {
                u8 *cell = &work->sheet[step * 800 + 2 * 4800];

                drawUpper(canvas, cell, position.x - 10, position.y - 40, 20,
                          40);
                drawLower(canvas, cell, position.x - 10, position.y, 20, 40);
              }
              EffectStep_AdvanceWithGravity3D(streak, 64, 0);
              streak->variant++;
            }
          }

          // From 6 frames after the first streak hits, for 32 frames, the
          // target trembles from side to side.
          if (STREAK_VALUE(mode, STREAK_SHAKE) &&
              frame >= k * STREAK_TARGET_DELAY + 30 &&
              frame < k * STREAK_TARGET_DELAY + 62) {
            struct MotionObject *struck =
                GetBattleObjectSlotFar(work->effect->actors[k])->object;

            struck->x +=
                gStreakVolley_Tremble[(frame - k * STREAK_TARGET_DELAY - 30) %
                                      8]
                << 16;
            if (struck->x > 0)
              struck->x += 0x8000;
            else
              struck->x -= 0x8000;
            ObjectGroup_UpdateMembers(work->effect->actors[k], -1, POSE_DAMAGE,
                                      -1, 0);
          }

          // The hit: the first streak plays the hit sound and flashes the
          // target white in its damage pose for 8 frames, and it
          // flashes again 16 frames later.
          if (STREAK_VALUE(mode, STREAK_HIT)) {
            if (frame == k * STREAK_TARGET_DELAY + 24) {
              AudioCommand_PlayFar(133);
              if (k == 0) // the first hit lets the battle show the damage
                BattleEventRuntime_BeginPhaseFar(-1);
              ObjectGroup_UpdateMembers(work->effect->actors[k],
                                        SPRITE_COLOR_WHITE, POSE_DAMAGE, k, 8);
            }
            if (frame == k * STREAK_TARGET_DELAY + 40)
              ObjectGroup_UpdateMembers(work->effect->actors[k],
                                        SPRITE_COLOR_WHITE, POSE_DAMAGE, k, 8);
          }
          // The style's recoil motion, with a camera shake.
          if (STREAK_VALUE(mode, STREAK_MOTION) != -1 &&
              frame == k * STREAK_TARGET_DELAY + 24) {
            work->shakeFrames = 4;
            BattleMotion_ApplyVariantMotionFar(
                work->effect->actors[k], STREAK_VALUE(mode, STREAK_MOTION));
          }
        }
      }
      Camera_ApplyShake(8, 8);
      EFFECT_END_FRAME(work);
    }
  }

  EFFECT_FINISH_CANVAS();
}

enum MoteFountainMode { MOTE_FOUNTAIN_HEALING, MOTE_FOUNTAIN_DRIFTING };
enum { MOTE_INITIALIZED_COUNT = 256, MOTE_VISIBLE_COUNT = 128 };

// Abilities: Sleep Star (kind 349).
//
// Battle effect 349: rising motes that drift onto the targets and strike.
DEFINE_EFFECT_KIND(BattleFx_RunSleepStar, BattleFx_RunMoteFountain,
                   MOTE_FOUNTAIN_DRIFTING)

// Abilities: Soothing Star (kind 344).
//
// Battle effect 344: rising healing motes.
DEFINE_EFFECT_KIND(BattleFx_RunSoothingStar, BattleFx_RunMoteFountain,
                   MOTE_FOUNTAIN_HEALING)

#define MOTE_FOUNTAIN_FRAMES 128 // frames in the effect

// Motes burst out of the user in every direction and float upwards,
// wobbling, for 128 frames; a new group of 32 joins every 8 frames. Each
// target in turn reacts from frame 48, 8 frames apart.
// mode 0: orange motes that stay close to the user; each target plays the
//         recovery sound and glows (a healing effect).
// mode 1: motes on the earth palette that drift across towards the far side
//         of the field; each target flinches (an attack).
void BattleFx_RunMoteFountain(struct BattleEffectArgument *effect, s32 mode) {
  s32 worldPoint[3];
  struct EffectPosition position;
  struct BattleEffectWork *work;
  void *canvas;
  BlitterPair draw;
  struct BattleCamera *camera;
  u8 *sheet;
  struct MotionObject *source;
  struct EffectStep *mote;
  s32 resource;
  s32 frame;
  s32 i;

  work = *(gWorkSlot + HEAP_SLOT_BATTLE_EFFECT);
  canvas = *(gWorkSlot + HEAP_SLOT_BATTLE_CANVAS);
  camera = *(gWorkSlot + HEAP_SLOT_CAMERA);
  sheet = *(gWorkSlot + HEAP_SLOT_BATTLE_SHEET);
  work->effect = effect;
  if (mode == MOTE_FOUNTAIN_HEALING)
    BattleFx_BeginCanvasLayer(0);
  else
    BattleFx_BeginCanvasLayer(1);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS, COMPOSITE_MAX);
  draw[0] =
      (DrawRectangle)((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BLITTER];
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y, COMPOSITE_MAX);
  draw[1] = (DrawRectangle)((union HeapState *)gWorkSlot)
                ->slots[HEAP_SLOT_BLITTER_ALTERNATE];
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  if (mode == MOTE_FOUNTAIN_HEALING)
    resource = (s32)&ResourceId_OrangePaletteA;
  else
    resource = (s32)&ResourceId_EarthWallSheet;
  Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(resource), 128);
  work->transferMode = CANVAS_COPY_AND_FADE; // trails keep 3/4 a frame
  work->transferValue = 75;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);

  // Every mote starts 5 pixels above the user with a random direction and
  // speed, mostly upwards, and a life of 48 to 79 frames. Only the first 128
  // are ever drawn.
  source = GetBattleObjectSlotFar(work->effect->actor)->object;
  for (i = 0; i != MOTE_INITIALIZED_COUNT; i++) {
    s32 speed;
    s32 angle;

    mote = &PARTICLES[i];

    speed = 0x3ff;
    speed &= Random16();
    angle = Random16() & 0xffff;
    EFFECT_SET_POSITION(mote, source->x, source->y + 0x50000, source->z);
    EFFECT_SET_VELOCITY(mote, Trig_Sin(angle) * (speed + 32) >> 8,
                        ((Random16() & 255) - 32) << 9,
                        -(Trig_Cos(angle) * (speed + 32) << 1) >> 8);
    mote->variant = (Random16() & 31) + 48;
    if (mode == MOTE_FOUNTAIN_HEALING) {
      mote->velocityX /= 2;
      mote->velocityZ /= 2;
    }
  }

  for (frame = 0; frame != MOTE_FOUNTAIN_FRAMES; frame++) {
    EFFECT_VIEW_FROM_CAMERA(camera);
    for (i = 0; i != MOTE_VISIBLE_COUNT; i++) {
      mote = &PARTICLES[i];
      if (frame >= i / 32 * 8 && mote->variant >= 0) {
        s32 size;

        // Wobble sideways, project onto the half-width canvas and draw a
        // 1x2 to 6x12 mote, larger the nearer it is to the camera.
        EFFECT_SET_VECTOR(
            worldPoint, mote->x + Trig_Sin((i * 4 + mote->variant) << 10) * 16,
            mote->y, mote->z);
        EffectPosition_ApplyBaseAndYOffset(worldPoint, &position);
        position.x >>= 1;
        if (position.depth <= 313)
          position.depth = 314;
        if (position.depth > 634)
          position.depth = 634;
        size = 6 - (position.depth - 314) / 64;
        EFFECT_DRAW_MOTE(draw[0], canvas, sheet, position.x, position.y, size);
        EffectStep_AdvanceWithGravity3D(mote, 62, 0x400);
        // Mode 1 pushes every mote towards the other side of the field.
        if (mode == MOTE_FOUNTAIN_DRIFTING) {
          if (source->x < 0)
            mote->velocityX += 0x2000;
          else
            mote->velocityX -= 0x2000;
        }
        mote->variant--;
      }
    }
    // Each target reacts in turn, and the battle shows its result.
    if (mode == MOTE_FOUNTAIN_DRIFTING) {
      EFFECT_FOR_EACH_TARGET(i, work->effect) {
        if (frame == i * 8 + 48) {
          BattleEventRuntime_BeginPhaseFar(-1);
          ObjectGroup_UpdateMembers(work->effect->actors[i], SPRITE_COLOR_WHITE,
                                    POSE_DAMAGE, i, 8);
        }
      }
    } else {
      EFFECT_FOR_EACH_TARGET(i, work->effect) {
        if (frame == i * 8 + 48) {
          AudioCommand_PlayFar(SOUND_RECOVERY);
          BattleEventRuntime_BeginPhaseFar(-1);
          ObjectGroup_UpdateMembers(work->effect->actors[i], SPRITE_COLOR_WHITE,
                                    -1, i, 8);
        }
      }
    }
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS();
}

// Target strikes, actor attacks and tiled scene transitions.
// The runners coordinate their own drawing callbacks and release them
// before returning their drawing buffers.

// Target strikes: repeated slashes or columns of light on the first target.
//
// One runner, BattleFx_RunTargetStrike, plays all twelve strike styles. Each
// style's row gives its kind (0 for a sideways slash, 1 for a column of
// light), its number of hits, its extras (smoke, an absorbing drain, glints,
// sparks or one heavy hit), its palette, the frames each picture is held, the
// column's composite mode and the scene's duration. The wrappers above it
// name the style of each effect.
//
// The scene runs in three phases:
//
// - Setup loads the pictures and the style's palette, then parks every
//   particle, idle, where it will start: 64 smoke puffs at the target's feet,
//   128 motes and 512 sparks 20 pixels up its body, each with a random
//   velocity. A particle is idle while its variant is -1.
// - Each frame first draws the strike (a slash sweep or a column) and, on a
//   hit frame, shakes and flashes the target and lights the next group of
//   particles. Glints are then scattered around the target, and the lit
//   smoke, motes and sparks are drawn and moved.
// - The finish releases the canvas layer.
//
// Several steps consume random numbers, so their order within a frame is part
// of the effect.

// The strike sparks live after the motes in gMapCellBuffer.
#define TARGET_STRIKE_SPARKS ((struct EffectStep *)(Ram_MapCellBuffer + 0x3800))

// gTargetStrike_Records has seven entries for each style.
#define STRIKE_VALUE(mode, field) gTargetStrike_Records[(mode) * 7 + (field)]
enum {
  STRIKE_KIND,         // STRIKE_KIND_SLASH or STRIKE_KIND_COLUMN
  STRIKE_HITS,         // how many times the target is struck
  STRIKE_FLAGS,        // STRIKE_SMOKE and so on
  STRIKE_COLOUR,       // background palette, 0 to 5 (see below)
  STRIKE_IMAGE_FRAMES, // frames each strike image is shown
  STRIKE_COMPOSITE,    // how the columns are drawn (COMPOSITE_MAX or _ADDITIVE)
  STRIKE_FRAMES        // length of the effect in frames
};
// STRIKE_KIND_* and the STRIKE_SMOKE flags are in effect_table.h.

// Rows of gTargetStrike_Records used by these effect entry points.
enum {
  STRIKE_STYLE_BLUE_SPARK_COLUMNS = 0,
  STRIKE_STYLE_LIME_SMOKE_COLUMN = 1,
  STRIKE_STYLE_EMBER_DRAIN_COLUMNS = 3,
  STRIKE_STYLE_MARS_DRAIN_COLUMNS = 4,
  STRIKE_STYLE_LIGHTNING_GLINT_COLUMNS = 5,
  STRIKE_STYLE_LIGHTNING_GLINT_SLASH = 6,
  STRIKE_STYLE_LIME_SMOKE_SLASH = 9,
  STRIKE_STYLE_MARS_SMOKE_COLUMN = 10
};

// Abilities: Numbing Sting (kind 313).
//
// Battle effect 313: one slash with glints, lightning colours.
DEFINE_EFFECT_KIND(BattleFx_RunNumbingSting, BattleFx_RunTargetStrike,
                   STRIKE_STYLE_LIGHTNING_GLINT_SLASH)

// Abilities: Cannibal Fang (kind 316).
//
// Battle effect 316: three ember columns that drain the target into the user.
DEFINE_EFFECT_KIND(BattleFx_RunCannibalFang, BattleFx_RunTargetStrike,
                   STRIKE_STYLE_EMBER_DRAIN_COLUMNS)

// Abilities: Poisonous Bite (kind 318); Poison Fang (kind 323).
//
// Battle effects 318 and 323: one lime column with smoke.
DEFINE_EFFECT_KIND(BattleFx_RunPoisonousBite, BattleFx_RunTargetStrike,
                   STRIKE_STYLE_LIME_SMOKE_COLUMN)

// Abilities: Rabid Fang (kind 336).
//
// Battle effect 336: one column with smoke, Mars Djinn colours.
DEFINE_EFFECT_KIND(BattleFx_RunRabidFang, BattleFx_RunTargetStrike,
                   STRIKE_STYLE_MARS_SMOKE_COLUMN)

// Abilities: Electric Bite (kind 324).
//
// Battle effect 324: three lightning columns with glints.
DEFINE_EFFECT_KIND(BattleFx_RunElectricBite, BattleFx_RunTargetStrike,
                   STRIKE_STYLE_LIGHTNING_GLINT_COLUMNS)

// Abilities: Poison Tail (kind 325).
//
// Battle effect 325: one lime slash with smoke.
DEFINE_EFFECT_KIND(BattleFx_RunPoisonTail, BattleFx_RunTargetStrike,
                   STRIKE_STYLE_LIME_SMOKE_SLASH)

// Abilities: Vampiric Fang (kind 327).
//
// Battle effect 327: three columns that drain the target, Mars Djinn colours.
DEFINE_EFFECT_KIND(BattleFx_RunVampiricFang, BattleFx_RunTargetStrike,
                   STRIKE_STYLE_MARS_DRAIN_COLUMNS)

// Abilities: Double Fang (kind 332).
//
// Battle effect 332: two blue columns with sparks.
DEFINE_EFFECT_KIND(BattleFx_RunDoubleFang, BattleFx_RunTargetStrike,
                   STRIKE_STYLE_BLUE_SPARK_COLUMNS)

// Abilities: Mortal Blow (kind 333).
//
// Battle effect 333: one heavy slash with sparks, Mars Djinn colours.
DEFINE_EFFECT_KIND(BattleFx_RunMortalBlow, BattleFx_RunTargetStrike, 8)

// Abilities: Twin Beaks (kind 335).
//
// Battle effect 335: two slashes with sparks, lightning colours.
DEFINE_EFFECT_KIND(BattleFx_RunTwinBeaks, BattleFx_RunTargetStrike, 7)

// Abilities: Acid Bite (kind 337).
//
// Battle effect 337: three pink columns with smoke.
DEFINE_EFFECT_KIND(BattleFx_RunAcidBite, BattleFx_RunTargetStrike, 2)

// Abilities: Drain Fang (kind 386).
//
// Battle effect 386: three additive columns that drain the target with large
// motes, Mars Djinn colours.
DEFINE_EFFECT_KIND(BattleFx_RunDrainFang, BattleFx_RunTargetStrike, 0xB)

// Strikes the first target STRIKE_HITS times with a sideways slash or a
// column of light, with the extras its flags choose, and lets the battle
// show the damage on the hits.
// mode : the style, 0 to 11, a row of gTargetStrike_Records.
void BattleFx_RunTargetStrike(struct BattleEffectArgument *effect, s32 mode) {
  struct EffectPosition ground;
  struct EffectPosition targetPosition;
  struct EffectPosition alternate;
  struct EffectPosition position;
  BlitterPair draw;
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 k;
  u8 *particleSheet;
  struct BattleCamera *camera;
  struct MotionObject *target;
  struct MotionObject *source;
  s32 resource;
  s32 i;
  s32 wispSheet = 0x3200;
  DrawRectangle *lower = &gBlitters[HEAP_SLOT_BLITTER_ALTERNATE];

  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  particleSheet = EFFECT_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  camera = EFFECT_SLOT(slots, HEAP_SLOT_CAMERA);
  work->effect = effect;
  if (mode == 8)
    BattleFx_BeginCanvasLayer(0);
  else
    BattleFx_BeginCanvasLayer(1);
  REG_BLDALPHA = BLEND_ALPHA(16, 16); // an even mix

  // The images: glints at the start of the sheet, the slashes or the
  // columns at 0xc80, smoke at 0x3200, and the style's palette.
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, particleSheet,
                             0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, work->sheet, 1, 0);
  if (STRIKE_VALUE(mode, STRIKE_KIND) == STRIKE_KIND_SLASH)
    Resource_LoadAndDecompress((s32)&ResourceId_BeamSequenceImage,
                               work->sheet + 0xc80, 0, 0);
  else
    Resource_LoadAndDecompress((s32)&ResourceId_TwelveModeImage,
                               work->sheet + 0xc80, 0, 0);
  switch (STRIKE_VALUE(mode, STRIKE_COLOUR)) {
  case 0:
    resource = (s32)&ResourceId_BlueBeamSheet;
    break;
  case 1:
    resource = (s32)&ResourceId_LimePalette;
    break;
  case 2:
    resource = (s32)&ResourceId_PinkPalette;
    break;
  case 3:
    resource = (s32)&ResourceId_EmberStreakSheet;
    break;
  case 4:
    resource = (s32)&ResourceId_MarsDjinnSheet;
    break;
  case 5:
  default:
    resource = (s32)&ResourceId_LightningBoltSheet;
    break;
  }
  Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(resource), 128);
  Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet, work->sheet + 0x3200,
                             1, 0);

  // Park every particle, idle, where it will start: smoke at the target's
  // feet, motes and sparks 20 pixels up its body, each with a random
  // velocity.
  for (k = 0; k != 1; k++) {
    target = GetBattleObjectSlotFar(work->effect->actors[k])->object;
    for (i = 0; i != 64; i++) {
      struct EffectStep *step = &work->particles[i];

      EffectPosition_ApplyAlternateStepAndYOffset(work->effect->actors[k],
                                                  &ground);
      EFFECT_SET_POSITION(step, (ground.x / 2) << 16, 0x500000, 0);
      EFFECT_SET_VELOCITY(step, ((Random16() & 255) - 128) << 9,
                          ((Random16() & 255) - 128) << 9, 0);
      step->variant = -1;
    }
    for (i = 0; i != 128; i++) {
      struct EffectStep *step = &PARTICLES[k * 128 + i];

      EFFECT_SET_POSITION(step, target->x, 0x140000, target->z);
      EFFECT_SET_VELOCITY(step, ((Random16() & 255) - 128) << 11,
                          ((Random16() & 255) - 128) << 11,
                          ((Random16() & 255) - 128) << 11);
      step->variant = -1;
    }
    for (i = 0; i != 512; i++) {
      TARGET_STRIKE_SPARKS[i].x = target->x;
      TARGET_STRIKE_SPARKS[i].y = 0x140000;
      TARGET_STRIKE_SPARKS[i].z = target->z;
      if (STRIKE_VALUE(mode, STRIKE_KIND) == STRIKE_KIND_COLUMN) {
        TARGET_STRIKE_SPARKS[i].velocityX = ((Random16() & 255) - 128) << 11;
        TARGET_STRIKE_SPARKS[i].velocityY = ((Random16() & 255) - 128) << 11;
        TARGET_STRIKE_SPARKS[i].velocityZ = ((Random16() & 255) - 128) << 11;
      } else {
        if (STRIKE_VALUE(mode, STRIKE_FLAGS) & 16) {
          TARGET_STRIKE_SPARKS[i].velocityX = (Random16() & 0x1ff) << 11;
          TARGET_STRIKE_SPARKS[i].velocityY = ((Random16() & 0x1ff) - 256)
                                              << 11;
          TARGET_STRIKE_SPARKS[i].velocityZ = ((Random16() & 0x1ff) - 256)
                                              << 11;
        } else {
          TARGET_STRIKE_SPARKS[i].velocityX = (Random16() & 255) << 11;
          TARGET_STRIKE_SPARKS[i].velocityY = ((Random16() & 255) - 128) << 11;
          TARGET_STRIKE_SPARKS[i].velocityZ = ((Random16() & 255) - 128) << 11;
        }
        if (TARGET_STRIKE_SPARKS[i].x >
            0) // slashes throw sparks towards the middle
          TARGET_STRIKE_SPARKS[i].velocityX =
              -TARGET_STRIKE_SPARKS[i].velocityX;
      }
      TARGET_STRIKE_SPARKS[i].variant = -1;
    }
  }

  work->transferMode = CANVAS_COPY_AND_FADE; // trails keep 3/4 a frame
  work->transferValue = 75;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);

  // Strike halfway between the target's middle and its feet.
  EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &targetPosition);
  EffectPosition_ApplyAlternateStepAndYOffset(work->effect->actors[0],
                                              &alternate);
  targetPosition.y += (alternate.y - targetPosition.y) / 2;

  for (frame = 0; frame != STRIKE_VALUE(mode, STRIKE_FRAMES); frame++) {
    s32 span = STRIKE_VALUE(mode, STRIKE_HITS) *
               STRIKE_VALUE(mode, STRIKE_IMAGE_FRAMES);
    s32 length = span * 4;
    s32 glint = 0;

    if (STRIKE_VALUE(mode, STRIKE_KIND) == STRIKE_KIND_SLASH) {
      // Slashes: six images sweep across the target from the user's side,
      // once per hit; the fifth image of each sweep is the hit.
      if (frame < span * 6) {
        s32 picture = frame / STRIKE_VALUE(mode, STRIKE_IMAGE_FRAMES) % 6;

        if (work->effect->side == 1) {
          EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER,
                              EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                              COMPOSITE_MAX);
          gBlitters[HEAP_SLOT_BLITTER](
              canvas, work->sheet + gTargetStrike_Offsets[picture],
              targetPosition.x / 2 - (gTargetStrike_Reach[picture] / 2) -
                  gTargetStrike_Widths[picture] + 8,
              targetPosition.y - (gTargetStrike_Heights[picture] / 2),
              gTargetStrike_Widths[picture], gTargetStrike_Heights[picture]);
        } else {
          EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS,
                              COMPOSITE_MAX);
          gBlitters[HEAP_SLOT_BLITTER](
              canvas, work->sheet + gTargetStrike_Offsets[picture],
              targetPosition.x / 2 + (gTargetStrike_Reach[picture] / 2) - 8,
              targetPosition.y - (gTargetStrike_Heights[picture] / 2),
              gTargetStrike_Widths[picture], gTargetStrike_Heights[picture]);
        }
        Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
        if (frame % (STRIKE_VALUE(mode, STRIKE_IMAGE_FRAMES) * 6) ==
            STRIKE_VALUE(mode, STRIKE_IMAGE_FRAMES) * 4) {
          if (mode == 8) {
            BattleEventRuntime_BeginPhaseFar(SOUND_HEAVY_IMPACT);
          } else {
            AudioCommand_PlayFar(133);
            BattleEventRuntime_BeginPhaseFar(133);
          }
          // A heavy hit lights every spark at once; otherwise the hit
          // lights the next 32. Each hit also starts the next 8 smoke
          // puffs.
          if (STRIKE_VALUE(mode, STRIKE_FLAGS) & STRIKE_HEAVY) {
            work->shakeFrames = 8;
            ObjectGroup_UpdateMembers(work->effect->actors[0],
                                      SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 12);
            BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 4);
            for (i = 0; i != 512; i++)
              TARGET_STRIKE_SPARKS[i].variant = (Random16() & 15) + 15;
          } else {
            work->shakeFrames = 4;
            ObjectGroup_UpdateMembers(work->effect->actors[0],
                                      SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 8);
            for (i = 0; i != 32; i++) {
              struct EffectStep *step =
                  &TARGET_STRIKE_SPARKS
                      [frame / (STRIKE_VALUE(mode, STRIKE_IMAGE_FRAMES) * 6) *
                           32 +
                       i];

              step->variant = (Random16() & 15) + 7;
            }
          }
          for (i = 0; i != 8; i++) {
            struct EffectStep *step =
                &work->particles[frame /
                                     (STRIKE_VALUE(mode, STRIKE_IMAGE_FRAMES) *
                                      6) *
                                     16 +
                                 i];

            step->variant = 0;
          }
        }
      }
      if (frame >= 12 && frame <= 31)
        glint = 1;
    } else if (frame < length + 4) {
      // Columns: a column of light, drawn as two mirrored halves, rises
      // through its stages on the target once per hit, then holds stage 3
      // for 4 frames.
      s32 stage;

      if (frame < length) {
        s32 picture = frame / STRIKE_VALUE(mode, STRIKE_IMAGE_FRAMES);

        while (picture > 4)
          picture -= 4;
        stage = gTargetStrike_ColumnStages[picture];
      } else {
        stage = 3;
      }
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS,
                          STRIKE_VALUE(mode, STRIKE_COMPOSITE));
      gBlitters[HEAP_SLOT_BLITTER](canvas, work->sheet + stage * 864 + 0xc80,
                                   targetPosition.x / 2 - 18, 56, 18, 48);
      Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                          STRIKE_VALUE(mode, STRIKE_COMPOSITE));
      gBlitters[HEAP_SLOT_BLITTER](canvas, work->sheet + stage * 864 + 0xc80,
                                   targetPosition.x / 2, 56, 18, 48);
      Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
      if (frame % (STRIKE_VALUE(mode, STRIKE_IMAGE_FRAMES) * 4) ==
          STRIKE_VALUE(mode, STRIKE_IMAGE_FRAMES) * 3) {
        // A hit: the target flashes; the last hit lets the battle show the
        // damage, the others only play the hit sound. Each hit lights the
        // next sparks, smoke puffs and motes.
        ObjectGroup_UpdateMembers(work->effect->actors[0], SPRITE_COLOR_WHITE,
                                  POSE_DAMAGE, 0, 8);
        work->shakeFrames = 4;
        if (frame > (STRIKE_VALUE(mode, STRIKE_HITS) * 4 - 4) *
                        STRIKE_VALUE(mode, STRIKE_IMAGE_FRAMES))
          BattleEventRuntime_BeginPhaseFar(133);
        else
          AudioCommand_PlayFar(133);
        for (i = 0; i != 64; i++) {
          struct EffectStep *step =
              &TARGET_STRIKE_SPARKS
                  [frame / (STRIKE_VALUE(mode, STRIKE_IMAGE_FRAMES) * 6) * 64 +
                   i];

          step->variant = (Random16() & 15) + 7;
        }
        for (i = 0; i != 8; i++) {
          struct EffectStep *step =
              &work->particles
                   [frame / (STRIKE_VALUE(mode, STRIKE_IMAGE_FRAMES) * 6) * 16 +
                    i];

          step->variant = 0;
        }
        for (i = 0; i != 16; i++)
          PARTICLES[frame / (STRIKE_VALUE(mode, STRIKE_IMAGE_FRAMES) * 6) * 16 +
                    i]
              .variant = 0;
      }
      if (stage == 3)
        glint = 1;
    }

    // Glints: three a frame, scattered at random around the target in
    // random flips.
    if ((STRIKE_VALUE(mode, STRIKE_FLAGS) & STRIKE_GLINTS) && glint) {
      for (i = 0; i != 3; i++) {
        s32 image = frame & 3;
        s32 angle = Random16() & 0xffff;
        s32 range = 31;
        s32 radius = Random16() & range;
        s32 x;
        s32 y;

        x = targetPosition.x / 2 + (Trig_Sin(angle) * (radius + 4) >> 17) -
            (gBattleFx_GlintCellWidths[image] / 2);
        y = targetPosition.y - (Trig_Cos(angle) * (radius + 4) >> 17) -
            (gBattleFx_GlintCellHeights[image] / 2);
        EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                            gTargetStrike_GlintDrawFlags[Random16() & 3] |
                                EFFECT_CLIP_BOUNDS,
                            COMPOSITE_ADDITIVE);
        (*lower)(canvas, work->sheet + gBattleFx_GlintCellOffsets[image], x, y,
                 gBattleFx_GlintCellWidths[image],
                 gBattleFx_GlintCellHeights[image]);
        Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
      }
    }

    EFFECT_VIEW_FROM_CAMERA(camera);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS, COMPOSITE_MAX);
    draw[0] = gBlitters[HEAP_SLOT_BLITTER];
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE, EFFECT_CLIP_BOUNDS,
                        COMPOSITE_ADDITIVE);
    draw[1] = *lower;
    for (k = 0; k != 1; k++) {
      source = GetBattleObjectSlotFar(work->effect->actor)->object;
      // Smoke puffs rise and spread through six images.
      if (STRIKE_VALUE(mode, STRIKE_FLAGS) & STRIKE_SMOKE) {
        for (i = 0; i != 64; i++) {
          struct EffectStep *step = &work->particles[i];

          if (step->variant >= 0 && step->variant <= 23) {
            s32 picture = step->variant / 4;

            draw[i & 1](canvas, work->sheet + picture * 1152 + wispSheet,
                        HI(step->x) - 12, HI(step->y) - 24, 24, 48);
            EffectStep_AdvanceWithGravity2D(step, 62, -0x400);
            step->variant++;
          }
        }
      }
      // Motes burst out of the target, then after 10 frames are pulled
      // into the user, who glows from frame 55 to 90; a mote goes out when
      // it reaches the user.
      if (STRIKE_VALUE(mode, STRIKE_FLAGS) & STRIKE_ABSORB) {
        s32 size = 3;

        if (mode == 11)
          size = 8;
        if (frame == 55)
          ObjectGroup_UpdateMembers(work->effect->actor, SPRITE_COLOR_WHITE, -1,
                                    -1, 0);
        if (frame == 90)
          ObjectGroup_UpdateMembers(work->effect->actor, 0, -1, -1, 0);
        for (i = 0; i != 64; i++) {
          struct EffectStep *step = &PARTICLES[i];

          if (step->variant >= 0) {
            EffectPosition_ApplyBaseAndYOffset(step, &position);
            position.x >>= 1;
            draw[1](canvas, particleSheet + gBattleFx_MoteCellOffsets[size - 1],
                    position.x - ((u32)size >> 1), position.y - size, size,
                    size * 2);
            EffectStep_AdvanceWithGravity3D(step, 60, 0);
            step->variant++;
            if (step->variant > 10) {
              s32 dx = (source->x - step->x) >> 8;
              s32 dy = (source->y - step->y + 0x140000) >> 8;
              s32 dz = (source->z - step->z) >> 8;

              step->velocityX += dx;
              step->velocityY += dy;
              step->velocityZ += dz;
              if ((dx >= -0xfff && dx <= 0xfff) &&
                  (dz >= -0xfff && dz <= 0xfff))
                step->variant = -1;
            }
          }
        }
      }
      // Sparks fly out and shrink as their lives run out.
      if (STRIKE_VALUE(mode, STRIKE_FLAGS) & STRIKE_SPARKS) {
        for (i = 0; i != 128; i++) {
          struct EffectStep *step = &TARGET_STRIKE_SPARKS[i];
          s32 size = step->variant;

          if (size >= 0) {
            EffectPosition_ApplyBaseAndYOffset(step, &position);
            position.x >>= 1;
            EffectStep_AdvanceWithGravity3D(step, 60, 0);
            size >>= 3;
            size++;
            EFFECT_DRAW_MOTE(draw[1], canvas, particleSheet, position.x,
                             position.y, size);
            step->variant--;
          }
        }
      }
    }
    EFFECT_RELEASE_BLITTERS();
    if (STRIKE_VALUE(mode, STRIKE_FLAGS) & STRIKE_HEAVY)
      Camera_ApplyShake(8, 8);
    else
      Camera_ApplyShake(2, 4);
    EFFECT_END_FRAME(work);
  }

  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  BattleFx_EndCanvasLayer();
}

// Somersault strikes, falling blades, element pillars and bursts, swirling
// swipes, the spider web and a picture that shatters.
//
// - Death Plunge runs the user up to the first target; both leap, turn upside
//   down, and on the hit bounce apart while a ring of flashes rises from the
//   user.
// - The blade rain drops 16 swords, one every 4 frames, slanting in on the
//   first target and sticking in the ground. No effect table entry points at
//   it.
// - The element pillar (BattleFx_RunIceFleckBurst) stands a mirrored pillar
//   of light at a unit's feet while motes spiral up around it.
// - The gathering burst flares sparks at four corners around a unit, draws
//   motes spiralling in, then plays a burst of light over it.
// - The swirl swipe (Poison Beat, Spinning Beat) sweeps a swirl across the
//   first target; its two modes differ in colour, length and how the target
//   reacts.
// - The spider web spreads four mirrored quarters of a web over the targets
//   and fades it in and out.
//
// Every runner sets up its canvas layer, pictures and particles, then runs a
// fixed number of frames, each ending with a canvas present (or a plain frame
// wait), and finally releases what the setup took. The two element runners
// allocate their own work, canvas and sheet instead of using the shared ones.
// Within a frame the order of the steps matters: later steps draw over
// earlier ones and some consume random numbers.

#define DEATH_PLUNGE_FRAMES 96 // frames in the effect

// Abilities: Death Plunge (kind 53).
//
// Battle effect 53: the user runs 9/10 of the way to the first target, then
// both leap and turn upside down in the air; on frame 54 the target is hit
// and both bounce back, while flashes burst from the user and rise in a
// widening ring.
void BattleFx_RunDeathPlunge(struct BattleEffectArgument *effect) {
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  /* FAKEMATCH: frame local: an unused second blitter word fills the
     frame's 8-byte draw slot, as this file's two-blitter effects declare
     the pair. Measured (2026-10-13): with draw[1] the frame is 80 bytes
     for 84 (differ 14, stack offsets only). */
  BlitterPair draw;
  struct BattleObj *first;
  struct BattleObj *second;
  struct MotionObject *user;
  struct MotionObject *target;
  s32 x;
  s32 z;
  s32 userHeight;
  s32 targetHeight;
  s32 point[3];
  struct EffectPosition screen;
  s32 frame;
  s32 percent;

  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, work->sheet, 1,
                             1);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                      COMPOSITE_MAX);
  work->transferMode = CANVAS_COPY_AND_FADE; // trails keep 3/4 a frame
  work->transferValue = 75;
  draw[0] = EFFECT_SLOT(slots, HEAP_SLOT_BLITTER);
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);
  // Run the user to 90% of the way to the target, then wait for it.
  first = GetBattleObjectSlotFar(work->effect->actor);
  second = GetBattleObjectSlotFar(work->effect->actors[0]);
  user = first->object;
  target = second->object;
  percent = 90;
  x = user->x + percent * (target->x - user->x) / 100;
  z = user->z + percent * (target->z - user->z) / 100;
  userHeight = Battle_GetObjectTableValueFar(work->effect->actor);
  targetHeight = Battle_GetObjectTableValueFar(work->effect->actors[0]);
  ObjectDispatch_InitFromTable6Far(user);
  Object_SetMoveTargetFar(user, x, 0, z);
  ObjectDispatch_ApplyArgumentToChildrenFar(user, 2);
  user->snapToTarget = 1;
  user->autoFaceMotion = 1;
  user->acceleration = 2 << 16;
  user->speedLimit = 8 << 16;
  WaitFrames(20);
  for (frame = 0; frame != DEATH_PLUNGE_FRAMES; frame++) {
    struct BattleCamera *camera;

    camera = gCameraWork;
    EFFECT_VIEW_FROM_CAMERA(camera);

    // Frame 0: both leap. Frame 11: both turn upside down, raised by their
    // own heights so they flip about their middles.
    if (frame == 0) {
      target->velocityY = 15 << 16;
      target->verticalMotionStrength = 0x91eb;
      user->velocityY = 15 << 16;
      user->verticalMotionStrength = 0x91eb;
    }
    if (frame == 11) {
      target->scaleY = -target->scaleY;
      user->scaleY = -user->scaleY;
      user->y += userHeight;
      target->y += targetHeight;
    }

    // Frame 54: the target flashes and both bounce; the user heads back
    // towards the middle of the field.
    if (frame == 54) {
      ObjectGroup_UpdateMembers(work->effect->actors[0], SPRITE_COLOR_WHITE,
                                POSE_DAMAGE, 0, 10);
      target->velocityY = 8 << 16;
      target->verticalMotionStrength = 0xab85;
      user->velocityY = 5 << 16;
      user->verticalMotionStrength = 0x7851;
      user->acceleration = 1 << 16;
      user->speedLimit = 2 << 16;
      user->autoFaceMotion = 0;
      ObjectDispatch_InitFromTable6Far(user);
      Object_SetMoveTargetFar(user, 0, 0, user->z);
    }
    EFFECT_VIEW_FROM_CAMERA(camera);
    EFFECT_SET_VECTOR(point, user->x, user->y, user->z);
    EffectPosition_ApplyBaseAndYOffset(point, &screen);
    screen.x >>= 1;

    // Frames 54 and 55: one big flash on the user. Frames 56 to 67: 16
    // flashes in a ring that widens as it rises, through six images.
    if (frame == 54 || frame == 55)
      draw[0](canvas, work, screen.x - 16, screen.y - 16, 32, 64);
    if (frame >= 56 && frame <= 67) {
      s32 i;

      for (i = 0; i != 16; i++) {
        s32 angle;
        s32 rx;
        s32 ry;

        angle = i << 12; // 22.5 degrees apart
        rx = screen.x + ((Trig_Sin(angle) * (frame - 46)) >> 16);
        ry = ((Trig_Cos(angle) * (frame - 46)) >> 16) - frame + 100;
        draw[0](canvas, work->sheet + (((frame - 56) / 2) << 11), rx - 16, ry,
                32, 64);
      }
    }

    // Frame 64: both turn right side up again.
    if (frame == 64) {
      target->scaleY = -target->scaleY;
      user->scaleY = -user->scaleY;
      user->y -= userHeight;
      target->y -= targetHeight;
      ObjectDispatch_ApplyArgumentToChildrenFar(user, 0);
    }
    if (frame == 54) // the battle shows the damage, with a heavy impact
      BattleEventRuntime_BeginPhaseFar(SOUND_HEAVY_IMPACT);
    if (frame == 0) {
      AudioCommand_PlayFar(136);
      work->shakeFrames = 6;
    }
    if (frame == 53)
      work->shakeFrames = 6;
    Camera_ApplyShake(16, 16);
    EFFECT_PRESENT_FRAME(work);
  }
  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

#define BLADE_RAIN_FRAMES 120 // frames in the effect

// Drop 16 swords, one every 4 frames, slanting in towards the first target
// (the middle of the canvas, x 64) and sticking in the ground, where their
// bottom 4 pixels are hidden.
//
// Sword i falls from frame 4i + 16 and sticks at frame 4i + 25. Every 4
// frames from 24 to 84 the target flashes white in its damage pose for 2
// frames and the screen shakes; every 8 frames the hit sound plays.
UNUSED void BattleFx_RunBladeRain(struct BattleEffectArgument *effect) {
  struct EffectPosition screenPosition;
  BlitterPair draw;
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle *blit;
  struct EffectStep *blade;
  s32 frame;
  s32 i;

  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(1);
  REG_BG2PA = Q_8_8(1);
  REG_BLDALPHA = BLEND_ALPHA(0, 16); // invisible to start with

  // [0] draws as is, [1] mirrored; both skip colour 0.
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                      COMPOSITE_TRANSPARENT);
  draw[0] = (DrawRectangle)EFFECT_SLOT(slots, HEAP_SLOT_BLITTER);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                      COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X,
                      COMPOSITE_TRANSPARENT);
  draw[1] = (DrawRectangle)EFFECT_SLOT(slots, HEAP_SLOT_BLITTER_ALTERNATE);
  blit = draw;
  Resource_LoadAndDecompress((s32)&ResourceId_SwordSlashSheet, work->sheet, 1,
                             1);
  work->transferMode = CANVAS_COPY_AND_CLEAR; // redraw from blank each frame
  work->transferValue = 0;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);
  // Scroll the canvas so its middle column sits on the first target.
  EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &screenPosition);
  REG_BG2X = (64 - screenPosition.x) << 8;

  // Each sword starts at a random x from 16 to 111, above the canvas, and
  // drifts towards the middle faster (in steps of 2 pixels a frame) the
  // further out it starts.
  for (i = 0; i != 64; i++) {
    s32 column;

    blade = &work->particles[i];
    column = Random16() % 96 + 16;
    blade->x = column;
    blade->y = (24 - i / 4) << 16;
    if (column <= 43)
      blade->velocityX = 3;
    else if (column <= 51)
      blade->velocityX = 2;
    else if (column <= 59)
      blade->velocityX = 1;
    else if (column <= 67)
      blade->velocityX = 0;
    else if (column <= 75)
      blade->velocityX = -1;
    else if (column <= 83)
      blade->velocityX = -2;
    else
      blade->velocityX = -3;
    blade->velocityX <<= 17;
    blade->velocityY = 8 << 16;
    blade->x <<= 16;
  }
  AudioCommand_PlayFar(212);
  for (frame = 0; frame != BLADE_RAIN_FRAMES; frame++) {
    // Fade in over the first 16 frames and out over the last 16.
    if (frame <= 16) {
      REG_BLDALPHA = frame | 0x1000;
      if (frame == 16)
        REG_BLDCNT = 0;
    }
    if (frame > 103) {
      REG_BLDALPHA = (120 - frame) | 0x1000;
      if (frame == 104)
        REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
    }

    // The image depends on how steeply the sword falls, and a sword
    // drifting left is drawn mirrored. Sword i falls from frame 4i + 16
    // and sticks in the ground at frame 4i + 25.
    for (i = 15; i != -1; i--) {
      s32 drift;
      s32 speed;
      s32 cell;

      blade = &work->particles[i];
      drift = blade->velocityX;
      speed = drift;
      if (drift < 0)
        speed = -drift;
      cell = speed >> 17;
      if (frame < i * 4 + 25) {
        u32 width;
        u32 height;

// Sequence dimensions before the call; assigning and reading them in
// separate arguments was UB. Plain English trial: 0x30c to 0x320 bytes,
// with different instructions. Preserve the original native calls.
#ifdef UBFIX
        width = gBladeRain_CellWidths[cell];
        height = gBladeRain_CellHeights[cell];
        blit[(u32)drift >> 31](canvas,
                               work->sheet + gBladeRain_CellSourceOffsets[cell],
                               (blade->x >> 16) - (width >> 1),
                               (blade->y >> 16) - (height >> 1), width, height);
#else
        blit[(u32)drift >> 31](
            canvas, work->sheet + gBladeRain_CellSourceOffsets[cell],
            (blade->x >> 16) - ((width = gBladeRain_CellWidths[cell]) >> 1),
            (blade->y >> 16) - ((height = gBladeRain_CellHeights[cell]) >> 1),
            width, height);
#endif
        if (frame >= i * 4 + 16) {
          blade->x += blade->velocityX;
          blade->y += blade->velocityY;
        }
      } else {
        u32 width;
        u32 height;

#ifdef UBFIX
        width = gBladeRain_CellWidths[cell];
        height = gBladeRain_CellHeights[cell];
        blit[(u32)drift >> 31](
            canvas, work->sheet + gBladeRain_CellSourceOffsets[cell],
            (blade->x >> 16) - (width >> 1), (blade->y >> 16) - (height >> 1),
            width, height - 4);
#else
        blit[(u32)drift >> 31](
            canvas, work->sheet + gBladeRain_CellSourceOffsets[cell],
            (blade->x >> 16) - ((width = gBladeRain_CellWidths[cell]) >> 1),
            (blade->y >> 16) - ((height = gBladeRain_CellHeights[cell]) >> 1),
            width, height - 4);
#endif
      }
    }

    // Every 4 frames from 24 to 84 the target flashes white in its damage
    // pose for 2 frames and the screen shakes; every 8 frames the hit
    // sound plays.
    if (frame >= 23 && frame <= 87 && (frame & 3) == 0) {
      ObjectGroup_UpdateMembers(work->effect->actors[0], SPRITE_COLOR_WHITE,
                                POSE_DAMAGE, 0, 2);
      work->shakeFrames = 1;
      if ((frame & 7) == 0)
        AudioCommand_PlayFar(133);
    }
    Camera_ApplyShake(8, 8);
    EFFECT_END_FRAME(work);
  }
  EFFECT_FINISH_CANVAS();
}

#define ICE_FLECK_BURST_FRAMES 56 // frames in the effect

// The glow of an element around a unit (BattleFx_PlayUnitElementEffect's
// mode 2): a symmetrical pillar of light stands at the unit's feet while
// motes spiral up around it, for 56 frames, and the first target flashes.
// Unlike the other effects it allocates its own work, canvas and sheet.
// effect->kind    : the element, which picks the palette: 0 Venus yellow,
//                   1 Mercury blue, 2 Mars red, others Jupiter violet.
// effect->variant : the mote size less one, kept to 1 to 4 after adding 1.
void BattleFx_RunIceFleckBurst(struct BattleEffectArgument *effect) {
  struct EffectPosition ground;
  struct EffectPosition screenPosition;
  s32 point[3];
  struct BattleEffectWork *work;
  void *canvas;
  BlitterPair draw;
  u8 *sheet;
  struct BattleCamera *camera;
  s32 frame;
  void *palette;
  s32 i;

  work = (struct BattleEffectWork *)Runtime_AllocateHeapBlock(
      HEAP_SLOT_BATTLE_EFFECT, sizeof *work);
  canvas = (void *)Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_CANVAS, 0x4000);
  sheet = (u8 *)Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_SHEET, 0x60e);
  camera = gCameraWork;
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);
  work->fadeFrames = 24;
  work->fadeStep = 0;
  REG_BLDALPHA = BLEND_ALPHA(12, 16); // 12/16 over all of the background
  REG_BG2PA = Q_8_8(1);
  Resource_LoadAndDecompress((s32)&ResourceId_IceTileSheet, work->sheet, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD, sheet, 0, 0);
  switch (work->effect->kind) {
  case 0:
    palette = Resource_GetTableEntry((s32)&ResourceId_YellowPaletteA);
    break;
  case 1:
    palette = Resource_GetTableEntry((s32)&ResourceId_IceTileSheet);
    break;
  case 2:
    palette = Resource_GetTableEntry((s32)&ResourceId_RedPaletteA);
    break;
  default:
    palette = Resource_GetTableEntry((s32)&ResourceId_VioletPaletteA);
    break;
  }
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);

  // The spiral's motes: x holds the angle and z the radius in 1/16
  // pixels; mote i waits i frames before it starts to rise.
  for (i = 0; i != 128; i++) {
    PARTICLES[i].y = 0;
    PARTICLES[i].x = Random16() & 0xffff;
    PARTICLES[i].z = (Random16() & 0x1ff) + i * 2;
    PARTICLES[i].variant = -i;
  }
  /* FAKEMATCH: the cancelling pointer updates retain work's allocation
   * priority without operand asm. Without them, work and the cell index
   * exchange r9/r11 and two spill slots swap (420 instructions, 50 rows).
   * The adjacent scheduling asm is unnecessary with these updates. */
  work++;
  work--;
  work->transferMode = CANVAS_COPY_AND_FADE; // trails keep 3/4 a frame
  work->transferValue = 75;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                      COMPOSITE_ADDITIVE);
  draw[0] = gWorkSlot[HEAP_SLOT_BLITTER];
  work->effect->variant++;
  if (work->effect->variant <= 0)
    work->effect->variant = 1;
  if (work->effect->variant > 4)
    work->effect->variant = 4;
  AudioCommand_PlayFar(212);

  for (frame = 0; frame != ICE_FLECK_BURST_FRAMES; frame++) {
    // Keep the canvas centred on the unit's feet, fade out over the last 6
    // frames, and flash the first target from frame 16 for 20 frames.
    EffectPosition_ApplyAlternateStepAndYOffset(work->effect->actor, &ground);
    REG_BG2X = (64 - ground.x) << 8;
    if (frame > 49)
      REG_BLDALPHA = (112 - frame * 2) | 0x1000;
    if (frame == 16)
      ObjectGroup_UpdateMembers(work->effect->actors[0], SPRITE_COLOR_WHITE, -1,
                                0, 20);

    // The pillar: its left half at x 47 and its mirrored right half at x
    // 64, keeping the brighter pixel. The pillar (17x64) cycles through four
    // images, two frames each; the glow at its foot (24x43) through three,
    // four frames each.
    if (frame <= 55) {
      s32 n;
      u8 *glow;

      i = frame / 2;
      n = i % 4;
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                          COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, COMPOSITE_MAX);
      draw[1] = ((DrawRectangle *)gWorkSlot)[HEAP_SLOT_BLITTER_ALTERNATE];
      draw[1](canvas, work->sheet + n * 1088, 47, ground.y - 64, 17, 64);
      n = frame / 4 % 3;
      glow = work->sheet + n * 1032 + 0x1100;
      draw[1](canvas, glow, 40, ground.y - 36, 24, 43);
      Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                          COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                              COMPOSITE_FLIP_X,
                          COMPOSITE_MAX);
      draw[1] = ((DrawRectangle *)gWorkSlot)[HEAP_SLOT_BLITTER_ALTERNATE];
      n = i % 4;
      draw[1](canvas, work->sheet + n * 1088, 64, ground.y - 64, 17, 64);
      draw[1](canvas, glow, 64, ground.y - 36, 24, 43);
      Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
    }
    GetBattleObjectSlotFar(work->effect->actor);
    EFFECT_VIEW_FROM_CAMERA(camera);

    // The first 32 motes: each frame a mote turns 0x400 (about 5.6
    // degrees), rises 5 pixels and moves 4 pixels further out.
    for (i = 0; i != 32; i++) {
      struct EffectStep *fleck = &PARTICLES[i];

      if (fleck->variant >= 0) {
        s32 size;

        point[0] = fleck->z * Trig_Sin(fleck->x) >> 4;
        point[2] = -(fleck->z * Trig_Cos(fleck->x) >> 4);
        point[1] = fleck->y;
        fleck->x += 0x400;
        fleck->y += 5 << 16;
        fleck->z += 64;
        EffectPosition_ApplyBaseAndYOffset(point, &screenPosition);
        screenPosition.x /= 2;
        size = (i & 1) + work->effect->variant;
        EFFECT_DRAW_FLARE(draw[0], canvas, sheet, screenPosition.x,
                          screenPosition.y, size);
      }
      fleck->variant++;
    }
    EFFECT_END_FRAME(work);
  }

  Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  BattleFx_EndCanvasLayer();
  Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_SHEET);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_CANVAS);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_EFFECT);
  /* FAKEMATCH: cancelling frame updates preserve the allocation lifetimes
   * without the two trailing asm statements. Deleting both swaps the
   * ground and camera-position pointers at sp+8/sp+12 (420 instructions,
   * 18 differing rows); one dead increment leaves the same swap. */
  frame ^= 1;
  frame ^= 1;
}

// Sets the pixel at (x, y) of the 256x120 8bpp tile picture in
// gMapCellBuffer (32 tiles across) to colour 2.
#define PLOT_SHATTER_MASK_PIXEL(x, y)                                          \
  (*(Ram_MapCellBuffer + ((x) & 7) + (x) / 8 * 64 + ((y) & 7) * 8 +            \
     (y) / 8 * 2048) = 2)

#define OPENING_FRAMES 128 // frames in the effect
#define OPENING_FLASHES 32 // flashes thrown as the picture breaks
#define OPENING_PIECES 33  // pieces the picture breaks into
#define OPENING_DISC_X 96  // the centre of the opening disc
#define OPENING_DISC_Y 60  // the centre of the opening disc

// The shattering opening of the battle on the Venus aerie storm backdrop:
// the screen turns white and a grey disc grows from (96, 60) by 32 pixels a
// frame while the TileAnimationMask picture breaks into 33 pieces that fly
// apart and fall; background palettes 6 to 13 then ease from white back to
// the battle backdrop's colours, and from frame 51 32 flashes rise. Runs
// for 128 frames and allocates its own work, canvas and sheet.
// effect : only effect->actors[0], the unit that flashes, is read.
void BattleFx_RunTileAndPaletteAnimation(struct BattleEffectArgument *effect) {
  /* FAKEMATCH: needed to match: never read: the native frame keeps 12 unused
   * bytes at sp+44..55, exactly this struct 2026-10-11: without it only the
   * frame shrinks (differ 4). */
  struct EffectPosition position;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  BlitterPair draw;
  struct BattleWork *battle;
  struct BattleBackgroundView *view;
  void **slots;
  void **slotTable;
  s32 targetRadius;
  struct EffectStep *piece;
  s32 radius;
  s32 i;
  s32 octantX;
  s32 j;
  s32 k;
  s32 error;
  s32 octantY;
  s32 x1;
  s32 size;
  s32 leftX;
  s32 innerLeftX;
  s32 y1;
  s32 x2;
  s32 y2;
  u8 *startY;
  u8 *startX;
  s32 centreX;
  s32 centreY;

  work = Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_EFFECT, sizeof(*work));
  size = 0x4000;
  canvas = Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_CANVAS, size);
  Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_SHEET, 0x302);
  slots = gWorkSlot + HEAP_SLOT_BATTLE;
  battle = slots[0];
  view = slots[HEAP_SLOT_BATTLE_BACKGROUND - HEAP_SLOT_BATTLE];
  work->effect = effect;

  // Make BG2 the 128x128 canvas, as BattleFx_BeginCanvasLayer does but
  // without its waits.
  BattleFx_ClearActorTimers();
  view->secondMode = 1;
  gBgScroll[1].y = 32;
  BattlePresentation_ConfigurePaletteFadeFar(1, battle->background, 0);
  REG_BG2CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7);
  BattlePres_SetupTransitionSceneFar(0, 0, 0, 100);
  view->secondMode = 0;
  EFFECT_SET_CANVAS_AFFINE();
  EFFECT_SET_BATTLE_WINDOWS();
  REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG2 |
              WININ_WIN1_OBJ | WININ_WIN1_CLR;
  REG_WINOUT = EFFECT_WINOUT;

  i = 0;
  for (k = 0; k != 16; k++) {
    for (j = 0; j != 8; j++, i += 2) {
      s16 entry;

      entry = ((k * 16 + j * 2 + 1) << 8) | ((k * 8 + j) * 2);
      *(volatile s16 *)(0x06003800 + i) = entry;
    }
  }

  Iwram_ClearWords(canvas, size);
  Iwram_ClearWords((void *)BG_CHAR_BLOCK(1), size);
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, EFFECT_DISPCNT, 0x20000);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  REG_BLDCNT = 0;
  Resource_LoadAndDecompress((s32)&ResourceId_TileAnimationMask,
                             work->imageStorage, 1, 1);
  work->transferMode = CANVAS_COPY_AND_CLEAR; // redraw from blank each frame
  work->transferValue = 0;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                      COMPOSITE_TRANSPARENT);
  slotTable = gWorkSlot;
  draw[0] = slotTable[HEAP_SLOT_BLITTER];
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                      COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, COMPOSITE_MAX);
  draw[1] = slotTable[HEAP_SLOT_BLITTER_ALTERNATE];

  // Each piece flies out from (32, 60) at a quarter of its distance from
  // there a frame.
  startY = gBattlePieceStartY;
  startX = gBattlePieceStartX;
  radius = 0;
  piece = work->particles;
  for (i = 0; i != OPENING_PIECES; i++) {
    s32 x, y, vx, vy;

    x = *startX++ << 16;
    y = *startY++ << 16;
    piece->x = x;
    piece->y = y;
    vx = (x - 0x200000) >> 2;
    vy = (y - 0x3c0000) >> 2;
    piece->velocityX = vx;
    piece->velocityY = vy;
    piece++;
  }

  // Save the picture at VRAM 0x06008000, fill it with colour 1, and turn
  // background palettes 6 to 13 white.
  Iwram_CopyWords(Ram_MapCellBuffer, (void *)BG_CHAR_BLOCK(2), 0x7800);
  Iwram_FillWords(Ram_MapCellBuffer, 0x7800, 0x01010101);
  view->busy = 1;
  work->savedBg1X = gBgScroll[1].x;
  work->savedBg1Y = gBgScroll[1].y;
  gBgScroll[1].x = 0;
  IO_WRITE_QUEUE_PUSH((u32)&REG_BG1CNT,
                      BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(0) |
                          BGCNT_256_COLORS | BGCNT_SCREEN_BASE(31),
                      0x20000);
  Iwram_FillWords((void *)&BG_PLTT_COLOR(6, 0), 0x100, 0x7fff7fff);
  AudioCommand_PlayFar(212);
  ObjectGroup_UpdateMembers(work->effect->actors[0], SPRITE_COLOR_WHITE, 3, 0,
                            30);

  for (frame = 0; frame != OPENING_FRAMES; frame++) {
    if (frame == 2)
      AudioCommand_PlayFar(212);
    if (frame == 3)
      AudioCommand_PlayFar(212);
    if (frame == 28)
      ObjectGroup_UpdateMembers(work->effect->actors[0], -1, 3, -1, 0);
    if (frame == 32)
      AudioCommand_PlayFar(149);
    if (frame == 5)
      AudioCommand_PlayFar(145);
    if (frame == 5) {
      gBgScroll[1].x = work->savedBg1X;
      BattleBackground_LoadFar(1, battle->background, -1);
    }

    // From frame 8, move every colour of palettes 6 to 13 one step towards
    // the battle backdrop's colour.
    if (frame > 7) {
      u16 *color;

      color = (u16 *)&BG_PLTT_COLOR(6, 0);
      for (i = 0; i != 128; i++) {
        s32 red = *color & 0x1f;
        s32 green = (*color >> 5) & 0x1f;
        s32 blue = (*color >> 10) & 0x1f;
        s32 goalRed = battle->palette[i] & 0x1f;
        s32 goalGreen = (battle->palette[i] >> 5) & 0x1f;
        s32 goalBlue = (battle->palette[i] >> 10) & 0x1f;

        if (red < goalRed)
          red++;
        else if (red > goalRed)
          red--;
        if (green < goalGreen)
          green++;
        else if (green > goalGreen)
          green--;
        if (blue < goalBlue)
          blue++;
        else if (blue > goalBlue)
          blue--;
        *color++ = (blue << 10) | (green << 5) | red;
      }
    }

    if (frame == 4)
      Iwram_FillWords((void *)BG_CHAR_BLOCK(2), 0x7800, 0x02020202);

    // Frames 0 to 3: grow the disc by 32 pixels of radius and brighten its
    // grey (colour 2).
    if (frame <= 3) {
      s32 v;

      v = frame * 4 + 8;
      targetRadius = frame * 32;
      BG_PLTT[2] = (v << 10) | (v << 5) | v;
      centreX = OPENING_DISC_X;
      centreY = OPENING_DISC_Y;
      for (; radius != targetRadius; radius++) {
        // The midpoint circle algorithm: one point plots all eight
        // octants, and each is drawn twice, one pixel apart, to fill gaps.
        octantX = radius;
        octantY = 0;
        error = radius;
        while (octantX >= octantY) {
          x2 = octantX + centreX;
          x1 = centreX - octantX;
          leftX = x1;
          y1 = centreY - octantY;
          y2 = octantY + centreY;
          if (y1 < 0)
            y1 = 0;
          if (y2 > 119)
            y2 = 119;
          if (x1 < 0)
            x1 = 0;
          if (x2 > 255)
            x2 = 255;
          PLOT_SHATTER_MASK_PIXEL(x2, y2);
          PLOT_SHATTER_MASK_PIXEL(x2, y1);
          PLOT_SHATTER_MASK_PIXEL(x1, y2);
          PLOT_SHATTER_MASK_PIXEL(x1, y1);
          x1 = leftX + 1;
          x2 = octantX + 97;
          if (x1 < 0)
            x1 = 0;
          if (x2 > 255)
            x2 = 255;
          PLOT_SHATTER_MASK_PIXEL(x2, y2);
          PLOT_SHATTER_MASK_PIXEL(x2, y1);
          PLOT_SHATTER_MASK_PIXEL(x1, y2);
          PLOT_SHATTER_MASK_PIXEL(x1, y1);

          x2 = octantY + centreX;
          x1 = centreX - octantY;
          innerLeftX = x1;
          y2 = octantX + centreY;
          y1 = centreY - octantX;
          if (x1 < 0)
            x1 = 0;
          if (x2 > 255)
            x2 = 255;
          if (y1 < 0)
            y1 = 0;
          if (y2 > 119)
            y2 = 119;
          PLOT_SHATTER_MASK_PIXEL(x2, y2);
          PLOT_SHATTER_MASK_PIXEL(x2, y1);
          PLOT_SHATTER_MASK_PIXEL(x1, y2);
          PLOT_SHATTER_MASK_PIXEL(x1, y1);
          x1 = innerLeftX + 1;
          x2 = octantY + 97;
          if (x1 < 0)
            x1 = 0;
          if (x2 > 255)
            x2 = 255;
          PLOT_SHATTER_MASK_PIXEL(x2, y2);
          PLOT_SHATTER_MASK_PIXEL(x2, y1);
          PLOT_SHATTER_MASK_PIXEL(x1, y2);
          PLOT_SHATTER_MASK_PIXEL(x1, y1);

          error -= octantY * 2 + 1;
          if (error < 0) {
            error += octantX * 2 - 2;
            octantX--;
          }
          octantY++;
        }
      }
      Iwram_CopyWords((void *)BG_CHAR_BLOCK(2), Ram_MapCellBuffer, 0x7800);
    }

    // Draw the pieces up to frame 50; they start to fall on frame 4.
    if (frame <= 50) {
      for (i = 0; i != OPENING_PIECES; i++) {
        piece = &work->particles[i];
        draw[0](canvas, work->imageStorage + gBattlePieceOffset[i],
                HI(piece->x), HI(piece->y), gBattlePieceWidth[i],
                gBattlePieceHeight[i]);
        if (frame > 3)
          EffectStep_AdvanceWithGravity2D(piece, 64, 0x4000);
      }
    }

    // Frames 8 to 50: colour 1 goes from black to white.
    if (frame >= 8 && frame <= 50) {
      s32 v;

      v = frame - 8;
      if (v > 31)
        v = 31;
      BG_PLTT[1] = (v << 10) | (v << 5) | v;
    }

    // Frame 51: load the flash images and a ramp of colours, and place 32
    // flashes at random in x 32 to 63, y 80 to 111.
    if (frame == 51) {
      Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, work, 1, 0);
      for (i = 1; i != 64; i++) {
        s32 v;

        v = i / 2;
        if (v < 0)
          v = 0;
        BG_PLTT[i] = (v << 10) | ((v / 2) << 5) | v;
      }
      REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
      for (i = 0; i != OPENING_FLASHES; i++) {
        piece = &work->particles[i];
        piece->x = ((Random16() & 31) + 32) << 16;
        piece->y = ((Random16() & 31) + 80) << 16;
        piece->velocityX = ((Random16() & 0x1ff) - 0x100) << 12;
        piece->velocityY = 0;
        piece->variant = 0;
      }
      work->transferMode = CANVAS_COPY_AND_FADE; // trails keep half a frame
      work->transferValue = 50;
    }

    // Flashes: flash i appears at frame 52 + i / 4 and rises, changing
    // image every 4 frames, for 40 frames.
    if (frame > 52) {
      for (i = 0; i != OPENING_FLASHES; i++) {
        piece = &work->particles[i];
        if (frame >= i / 4 + 52 && piece->variant <= 39) {
          s32 cell;

          cell = piece->variant / 4;
          if (cell > 5)
            cell = 5;
          draw[1](canvas, work->sheet + (cell << 11), HI(piece->x) - 16,
                  HI(piece->y) - 32, 32, 64);
          EffectStep_AdvanceWithGravity2D(piece, 60, -0x1000);
          piece->variant++;
        }
      }
    }

    EFFECT_END_FRAME(work);
  }

  // Restore the background and the display.
  EFFECT_RELEASE_BLITTERS();
  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  ObjectGroup_UpdateMembers(work->effect->actors[0], -1, POSE_STANDING, -1, 0);
  gBgScroll[1].x = work->savedBg1X;
  gBgScroll[1].y = 32;
  BattlePresentation_ConfigurePaletteFadeFar(2, battle->background, 0);
  WaitFrames(1);
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                          DISPCNT_BG2_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON |
                          DISPCNT_WIN1_ON,
                      0x20000);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_SHEET);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_CANVAS);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_EFFECT);
}

// One streak of light of the element streak burst, laid over the same
// 28-byte records as PARTICLES: a line along a randomly turned axis from
// tail to head, in distance from the centre.
struct Streak {
  s32 head;
  s32 tail;
  s32 unused08;
  s32 pitch;
  s32 yaw;
  s32 roll;
  s32 unused18;
};

// The burst of an element over a unit (BattleFx_PlayUnitElementEffect's
// mode 3), for 56 frames: sparks flare at the four corners around the
// unit, motes spiral in, and from frame 28 a burst of light plays over
// the unit while the first target flashes. Allocates its own work, canvas
// and sheet.
// effect->kind : the element, which picks the palette: 0 Venus yellow,
//                1 Mercury blue, 2 Mars red, others Jupiter violet.
void BattleFx_RunGatheringBurst(struct BattleEffectArgument *effect) {
  struct EffectPosition ground;
  struct BattleEffectWork *work;
  void *canvas;
  BlitterPair draw;
  u8 *sheet;
  void *palette;
  s32 frame;
  s32 i;

  work = Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_EFFECT, sizeof *work);
  canvas = Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_CANVAS, 0x4000);
  sheet = Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_SHEET, 0x60e);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);
  work->fadeFrames = 24;
  work->fadeStep = 0;
  REG_BLDALPHA = BLEND_ALPHA(12, 16); // 12/16 over all of the background
  REG_BG2PA = Q_8_8(1);
  Resource_LoadAndDecompress((s32)&ResourceId_CyanSparkSheet, work->sheet, 1,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD, sheet, 0, 0);
  switch (work->effect->kind) {
  case 0:
    palette = Resource_GetTableEntry((s32)&ResourceId_YellowPaletteA);
    break;
  case 1:
    palette = Resource_GetTableEntry((s32)&ResourceId_IceTileSheet);
    break;
  case 2:
    palette = Resource_GetTableEntry((s32)&ResourceId_RedPaletteA);
    break;
  default:
    palette = Resource_GetTableEntry((s32)&ResourceId_VioletPaletteA);
    break;
  }
  {
    Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  }

  // PARTICLES are set up as for the element pillar but never drawn here.
  // The motes that are drawn start 16 to 47 pixels out at a random angle
  // (x) and spiral in 2 pixels a frame for 16 to 31 frames.
  for (i = 0; i != 128; i++) {
    PARTICLES[i].y = 0x800000;
    PARTICLES[i].x = Random16() & 0xffff;
    PARTICLES[i].z = (Random16() & 0x1ff) + 0x400;
    PARTICLES[i].variant = -i;
  }
  for (i = 0; i != 64; i++) {
    work->particles[i].x = Random16() & 0xffff;
    work->particles[i].y = (Random16() & 31) + 16;
    work->particles[i].variant = (i & 15) + 16;
  }
  work->transferMode = CANVAS_COPY_AND_FADE; // trails keep 3/4 a frame
  work->transferValue = 75;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                      COMPOSITE_ADDITIVE);
  draw[0] =
      (DrawRectangle)((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BLITTER];
  AudioCommand_PlayFar(140);

  for (frame = 0; frame != 56; frame++) {
    // Follow the unit, fade out over the last 6 frames, and on frame 26
    // flash the first target for 20 frames.
    EffectPosition_ApplyStepAndYOffset(effect->actor, &ground);
    REG_BG2X = (64 - ground.x) << 8;
    if (frame > 49)
      REG_BLDALPHA = (0x70 - frame * 2) | 0x1000;
    if (frame == 26) {
      AudioCommand_PlayFar(212);
      ObjectGroup_UpdateMembers(work->effect->actors[0], SPRITE_COLOR_WHITE, -1,
                                0, 20);
    }
    // Frames 28 to 48: the burst of light (48x48), seven images.
    if (frame >= 28 && frame <= 48)
      draw[0](canvas, work->sheet + (frame - 28) / 3 * 2304 + 0x1400, 40,
              ground.y - 24, 48, 48);
    // Frames 0 to 14: a spark (32x32) at each corner, flipped to face
    // outwards, five images.
    if (frame >= 0 && frame <= 14) {
      s32 picture = frame / 3 % 5;
      DrawRectangle *slot =
          &((DrawRectangle *)gWorkSlot)[HEAP_SLOT_BLITTER_ALTERNATE];

      for (i = 0; i != 4; i++) {
        EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                            gCornerSparks_DrawFlags[i] | EFFECT_CLIP_BOUNDS,
                            COMPOSITE_MAX);
        draw[1] = *slot;
        draw[1](canvas, work->sheet + (picture << 10), gCornerSparks_X[i] + 32,
                ground.y + gCornerSparks_Y[i] - 32, 32, 32);
        Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
      }
    }
    // The motes, shrinking as they close in.
    if (frame >= 0) {
      for (i = 0; i != 64; i++) {
        struct EffectStep *mote = &work->particles[i];

        if (mote->variant >= 0 && mote->y > 0) {
          s32 size = (mote->variant >> 3) + 1;
          s32 x = (mote->y * Trig_Sin(mote->x) >> 16) + 64;
          s32 y = (mote->y * Trig_Cos(mote->x) >> 16) + ground.y;

          if (size <= 0)
            size = 1;
          EFFECT_DRAW_FLARE(draw[0], canvas, sheet, x, y, size);
          mote->y -= 2;
          mote->variant--;
        }
      }
    }
    EFFECT_END_FRAME(work);
  }

  Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  BattleFx_EndCanvasLayer();
  Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_SHEET);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_CANVAS);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_EFFECT);
}

// Scheduler callback of the element streak burst: on its first frame seeds
// 256 streaks with random axes, then each frame draws the first 64 (streak
// i from frame i / 4) as lines three pixels thick that shrink 4 pixels a
// frame towards the centre of the canvas, a little below its middle.
void BattleFx_DrawStreaks(void) {
  struct BattleEffectWork *work = WORK_SLOT(HEAP_SLOT_BATTLE_EFFECT);
  s32 frame;
  s32 i;
  struct Streak *streak;
  struct Vector3 point;
  struct EffectPosition head;
  struct EffectPosition tail;

  frame = (work->frame)++;
  if (frame == 0) {
    for (i = 0; i != 256; i++) {
      struct Streak *seed = &((struct Streak *)Ram_MapCellBuffer)[i];
      s32 r = Random16() & 15;

      seed->head = r + 48;
      seed->tail = r + 40;
      seed->pitch = Random16() & 0xffff;
      seed->yaw = Random16() & 0xffff;
      seed->roll = Random16() & 0xffff;
    }
  }
  point.y = 0;
  point.z = 0;
  i = 0;
  for (; i != 64; i++) {
    streak = &((struct Streak *)Ram_MapCellBuffer)[i];
    if (frame > i / 4 && streak->head > 0) {
      s32 fade;

      Render_ResetTransformState();
      SceneTransform_ApplyRoll(streak->roll);
      SceneTransform_ApplyPitch(streak->pitch);
      SceneTransform_ApplyYaw(streak->yaw);
      point.x = streak->head;
      EffectPosition_ApplyBaseAndYOffset(&point, &head);
      head.x += 64;
      head.y += 80;
      point.x = streak->tail;
      EffectPosition_ApplyBaseAndYOffset(&point, &tail);
      tail.x += 64;
      tail.y += 80;
      streak->tail -= 4;
      streak->head -= 4;
      if (streak->tail < 0)
        streak->tail = 0;
      // The tail stops at the centre; the line brightens as its tail
      // closes in.
      fade = -streak->tail / 2;
      BattleFx_DrawClippedCanvasLine(tail.x - 1, tail.y, head.x - 1, head.y,
                                     fade + 48);
      BattleFx_DrawClippedCanvasLine(tail.x, tail.y - 1, head.x, head.y - 1,
                                     fade + 48);
      BattleFx_DrawClippedCanvasLine(tail.x, tail.y, head.x, head.y, fade + 56);
    }
  }
  work->transferPending = 1;
}

// Starts the element streak burst (BattleFx_PlayUnitElementEffect's mode 1)
// and returns at once: opens the canvas and schedules
// BattleFx_DrawStreaks with a canvas that darkens each frame.
// The caller keeps the canvas on its unit and ends the burst with
// BattleFx_ScheduleCallbacksAndReleaseBlocks.
// kind : the element, which picks the palette: 0 Venus yellow, 1 Mercury
//        blue, 2 Mars red, 3 Jupiter violet, others a dusty grey.
void BattleFx_PrepareScene(s32 kind) {
  struct BattleEffectWork *work;
  void *palette;
  s32 id;

  work = Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_EFFECT, sizeof *work);
  Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_CANVAS, 0x4000);
  BattleFx_BeginCanvasLayer(0);
  work->fadeFrames = 24;
  REG_BG2PA = Q_8_8(1);
  REG_BLDALPHA = BLEND_ALPHA(16, 16); // an even mix
  switch (kind) {
  case 0:
    id = (s32)&ResourceId_YellowPaletteB;
    break;
  case 1:
    id = (s32)&ResourceId_CrystalSheet;
    break;
  case 2:
    id = (s32)&ResourceId_EmberStreakSheet;
    break;
  case 3:
    id = (s32)&ResourceId_VioletPaletteD;
    break;
  case 4:
  default:
    id = (s32)&ResourceId_DustPuffSheet;
    break;
  }
  palette = Resource_GetTableEntry(id);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  work->frame = 0;
  work->transferMode = CANVAS_COPY_AND_DARKEN;
  work->transferValue = 0x06060606; // 6 off every pixel each frame
  Scheduler_AddOrUpdateCallback(BattleFx_DrawStreaks, 0xC80);
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);
}

// Ends the element streak burst that BattleFx_PrepareScene
// started: stops its callbacks and the background palette fade, clears the
// canvas in VRAM and frees the work and the canvas.
void BattleFx_ScheduleCallbacksAndReleaseBlocks(void) {
  Scheduler_RemoveCallback(BattleFx_DrawStreaks);
  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  {
    Iwram_ClearWords((void *)BG_CHAR_BLOCK(1), 0x4000);
  }
  Scheduler_RemoveCallback(Palette_StepFadeTransfer);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_CANVAS);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_EFFECT);
}

// Abilities: Poison Beat (kind 382).
//
// Battle effect 382: a lime swipe that knocks the target back.
DEFINE_EFFECT_KIND(BattleFx_RunPoisonBeat, BattleFx_RunSwirlSwipe, 0)

// Abilities: Spinning Beat (kind 383).
//
// Battle effect 383: a magenta swipe that throws the target into the air.
DEFINE_EFFECT_KIND(BattleFx_RunSpinningBeat, BattleFx_RunSwirlSwipe, 1)

// A swirling swipe across the first target, six images four frames each,
// from the user's side: the first four from the swirl sheet, the last two
// from the tail sheet. The swipe lands on frame 8.
// mode 0 : lime colours; the battle shows the damage at once and the target
//          recoils. 48 frames.
// mode 1 : the target flashes, is thrown up on frame 13 and the battle shows
//          the damage as it lands on frame 65. 74 frames.
void BattleFx_RunSwirlSwipe(struct BattleEffectArgument *effect, s32 mode) {
  struct BattleEffectWork *work;
  void *canvas;
  struct MotionObject *object;
  struct EffectPosition position;
  /* FAKEMATCH: needed to match: only draw[0] is used: the second word fills
   * four bytes of the frame */
  BlitterPair draw;
  s32 frames;
  s32 frame;
  s32 cell;
  void **slots;
  void **cursor;

  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);
  REG_BG2PA = Q_8_8(1);
  if (work->effect->side == 1) {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS,
                        COMPOSITE_TRANSPARENT);
    draw[0] = (DrawRectangle)EFFECT_SLOT(slots, HEAP_SLOT_BLITTER);
  } else {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                        COMPOSITE_TRANSPARENT);
    draw[0] = (DrawRectangle)EFFECT_SLOT(slots, HEAP_SLOT_BLITTER);
  }
  Resource_LoadAndDecompress((s32)&ResourceId_MagentaSwirlSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_MagentaTailSheet,
                             Ram_MapCellBuffer, 1, 0);
  if (mode == 0) {
    void *palette = Resource_GetTableEntry((s32)&ResourceId_LimePalette);
    Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  }
  work->transferMode = CANVAS_COPY_AND_FADE; // trails keep 3/4 a frame
  work->transferValue = 75;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);
  object = GetBattleObjectSlotFar(work->effect->actors[0])->object;
  EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &position);
  // Line the canvas up so the swipe crosses the target.
  if (work->effect->side == 0)
    REG_BG2X = (16 - position.x) << 8;
  else
    REG_BG2X = (112 - position.x) << 8;
  frames = 74;
  if (mode != 1)
    frames = 48;
  for (frame = 0; frame != frames; frame++) {
    cell = frame / 4;
    if (cell <= 5) {
      if (cell <= 3)
        draw[0](canvas, work->sheet + gSwirlSwipe_CellSourceOffsets[cell],
                gSwirlSwipe_CellX[cell + work->effect->side * 6],
                gSwirlSwipe_CellBiasY[cell] + 32, gSwirlSwipe_CellWidths[cell],
                gSwirlSwipe_CellHeights[cell]);
      else
        draw[0](canvas, Ram_MapCellBuffer + gSwirlSwipe_CellSourceOffsets[cell],
                gSwirlSwipe_CellX[cell + work->effect->side * 6],
                gSwirlSwipe_CellBiasY[cell] + 32, gSwirlSwipe_CellWidths[cell],
                gSwirlSwipe_CellHeights[cell]);
    }
    if (frame == 8) {
      if (mode == 0) {
        BattleEventRuntime_BeginPhaseFar(133);
        BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 1);
      } else {
        AudioCommand_PlayFar(SOUND_HEAVY_IMPACT);
        ObjectGroup_UpdateMembers(work->effect->actors[0], SPRITE_COLOR_WHITE,
                                  POSE_DAMAGE, 0, 4);
      }
      work->shakeFrames = 8;
    }
    if (mode == 1) {
      if (frame == 13) {
        object->velocityY = 0xc0000;
        object->verticalMotionStrength = 0x7851;
        object->verticalMotionPhase = 0x4000;
      }
      if (frame == 65) {
        work->shakeFrames = 4;
        BattleEventRuntime_BeginPhaseFar(SOUND_HEAVY_IMPACT);
      }
    }
    Camera_ApplyShake(8, 8);
    EFFECT_END_FRAME(work);
  }
  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

#define SPIDER_WEB_FRAMES 63 // frames in the effect

// Abilities: Spider Web (kind 345).
//
// Battle effect 345: a spider web spreads over the targets, drawn as four
// mirrored 32x32 quarters around the point between the first and last
// target. It fades in over 8 frames; the targets flash on frame 10, the
// battle shows the damage on frame 32, and it fades out from frame 54.
void BattleFx_RunSpiderWeb(struct BattleEffectArgument *effect) {
  struct EffectPosition first;
  struct EffectPosition last;
  void *canvas;
  struct BattleEffectWork *work;
  s32 size;
  s32 transparent;

  // Plain, the counter's priority (20 refs over 232 insns) ties size and
  // outranks transparent (13 over 240).
  /* FAKEMATCH: pins the frame counter to r9, as native; plain it takes sl and
   * transparent r9 (differ 30), and no plain shape ranks transparent above it
   * (for/while/u32, declaration order, a bound variable, early init, argument
   * use, 19k permuter candidates). */
  register s32 frame __asm__("r9");
  s32 i;

  /* FAKEMATCH: needed to match: size and transparent first hold the slot
   * table and blend register addresses, sharing their registers 2026-10-11:
   * plain slot reads and REG_BLDALPHA differ 56; the reads alone 40, the
   * register alone 50. */
  size = (s32)&Ram_WorkSlot[HEAP_SLOT_BATTLE_CANVAS];
  canvas = *(void **)size;
  work = *(struct BattleEffectWork **)(size - 4);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(2);
  transparent = (s32)&REG_BLDALPHA;
  REG_BG2PA = Q_8_8(1);
  *(u16 *)transparent = 0x1000; // REG_BLDALPHA: invisible to start with
  EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &first);
  EffectPosition_ApplyStepAndYOffset(
      work->effect->actors[work->effect->count - 1], &last);
  first.x += (last.x - first.x) / 2; // centre the canvas between them
  REG_BG2X = (64 - first.x) << 8;
  Resource_LoadAndDecompress((s32)&ResourceId_SpiderWebSheet, work->sheet, 1,
                             1);
  work->transferMode = CANVAS_COPY_AND_CLEAR; // redraw from blank each frame
  work->transferValue = 0;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);
  AudioCommand_PlayFar(143);
  // transparent is COMPOSITE_TRANSPARENT, and also the 1 that marks each
  // frame's canvas for transfer.
  frame = 0;
  transparent = 1;
  size = 32;
  do {
    if (frame <= 8)
      REG_BLDALPHA = (frame << 1) | 0x1000;
    if (frame > 53)
      REG_BLDALPHA = (0x7c - (frame << 1)) | 0x1000;
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS, transparent);
    ((DrawRectangle)Ram_WorkSlot[HEAP_SLOT_BLITTER])(canvas, work, 33, 41, size,
                                                     size);
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, transparent);
    ((DrawRectangle)Ram_WorkSlot[HEAP_SLOT_BLITTER])(canvas, work, 64, 41, size,
                                                     size);
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y, transparent);
    ((DrawRectangle)Ram_WorkSlot[HEAP_SLOT_BLITTER])(canvas, work, 33, 72, size,
                                                     size);
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP,
                        transparent);
    ((DrawRectangle)Ram_WorkSlot[HEAP_SLOT_BLITTER])(canvas, work, 64, 72, size,
                                                     size);
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
    if (frame == 32)
      BattleEventRuntime_BeginPhaseFar(143);
    EFFECT_FOR_EACH_TARGET(i, work->effect) {
      if (frame == 10)
        ObjectGroup_UpdateMembers(work->effect->actors[i], SPRITE_COLOR_WHITE,
                                  -1, i, 8);
    }
    ObjectGroup_TickMemberTimers();
    work->transferPending = transparent;
    WaitFrames(1);
    frame++;
  } while (frame != SPIDER_WEB_FRAMES);
  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  BattleFx_EndCanvasLayer();
}

// Shared canvas transfers, palette fades, clipping and target flash timers.
//
// Most effects draw into a 128x128 canvas of 8-bit colour in the canvas heap
// block, stored as 16x16 tiles of 8x8 pixels. Each frame a runner draws, then
// sets transferPending. The scheduled callback
// BattleFx_ProcessPendingGraphicsTransfer copies the canvas to video memory
// before the next frame and, by transferMode, clears, fades, darkens or
// brightens the pixels it keeps, so that trails decay. Effects that draw a
// full-screen picture in Ram_MapCellBuffer use BG1 instead
// (BattleFx_SetupCanvasTileMap) and the matching flush callback. A runner
// must remove its transfer callback before the buffers are released.
//
// A reaction may start one of the eight actor timers in the effect's work
// block; ObjectGroup_TickMemberTimers counts them down each frame and gives
// the battler its own palette back when one runs out. The timers are indexed
// like effect->actors, so timer n belongs to actor n.

// A shared pixel-address helper changed registers at unchanged English
// size 0x15c. Merging the nested bounds tests also changed native code;
// keep the original address expressions and two tests.
// Wipes the whole canvas to one colour with a ragged edge: each row (or
// column) lags a random 0 to 63 pixels behind the front, which speeds up as
// it goes. Returns when the wipe is done, a frame at a time.
// mode  : 1 sweeps left to right; anything else sweeps top to bottom.
// value : 0 wipes to colour 1, 1 wipes to colour 0 (clears the canvas).
void BattleFx_WipeCanvas(s32 mode, s32 value) {
  struct BattleEffectWork *work;
  u8 *canvas;
  u8 delay[128];
  s32 x;
  s32 y;
  s32 wipePosition;

  canvas = *(gWorkSlot + HEAP_SLOT_BATTLE_CANVAS);
  work = *(gWorkSlot + HEAP_SLOT_BATTLE_EFFECT);
  for (x = 0; x != 128; x++)
    delay[x] = Random16() & 0x3f;

  // The canvas is 16x16 tiles of 8x8 pixels; (x, y) is byte
  // ((y / 8 * 16 + x / 8) * 8 + y % 8) * 8 + x % 8.
  if (mode == 1) {
    s32 front;
    s32 speed;

    front = 0;
    speed = 1;
    x = 0;
    do {
      front += speed;
      speed++;
      for (; x != front; x++) {
        for (y = 0; y != 128; y++) {
          wipePosition = x - delay[y];
          if (wipePosition >= 0)
            if (wipePosition <= 127)
              canvas[(((y / 8) * 16 + wipePosition / 8) * 8 + (y & 7)) * 8 +
                     (wipePosition & 7)] = 1 - value;
        }
      }
      EFFECT_PRESENT_FRAME(work);
    } while (front <= 256);
  } else {
    s32 front;
    s32 speed;

    front = 0;
    speed = 1;
    y = 0;
    do {
      front += speed / 2;
      speed += 4;
      for (; y != front; y++) {
        for (x = 0; x != 128; x++) {
          wipePosition = y - delay[x];
          if (wipePosition >= 0)
            if (wipePosition <= 127)
              canvas[(((wipePosition / 8) * 16 + x / 8) * 8 +
                      (wipePosition & 7)) *
                         8 +
                     (x & 7)] = 1 - value;
        }
      }
      EFFECT_PRESENT_FRAME(work);
    } while (front <= 191);
  }
}

// Scheduler callback of nearly every effect: when the effect has finished
// drawing a frame (transferPending), copies the 128x128 canvas into BG2's
// character block 1 the way transferMode says, and counts the frames since
// the last copy.
void BattleFx_ProcessPendingGraphicsTransfer(void) {
  void **slots = gWorkSlot + HEAP_SLOT_BATTLE_EFFECT;
  struct BattleEffectWork *work = slots[0];
  void *source;
  s32 *counter;
  s32 nextCounter;

  if (work->transferPending == 1) {
    source = EFFECT_SLOT(slots, HEAP_SLOT_BATTLE_CANVAS);
    switch (work->transferMode) {
    case CANVAS_COPY:
      Iwram_CopyWords((void *)BG_CHAR_BLOCK(1), source, 0x4000);
      break;
    case CANVAS_COPY_AND_CLEAR:
      Iwram_CopyWords((void *)BG_CHAR_BLOCK(1), source, 0x4000);
      Iwram_FillWords(source, 0x4000, work->transferValue);
      break;
    case CANVAS_COPY_AND_FADE:
      if (work->transferValue == 50) {
        ColorBuffer_BackupAndHalve(source, (void *)BG_CHAR_BLOCK(1), 0x4000);
      } else {
        ColorBuffer_BackupAndScaleThreeQuarters(
            source, (void *)BG_CHAR_BLOCK(1), 0x4000);
      }
      break;
    case CANVAS_COPY_AND_DARKEN:
      ColorBuffer_BackupAndDarken(source, work->transferValue,
                                  (void *)BG_CHAR_BLOCK(1), 0x4000);
      break;
    case CANVAS_COPY_AND_BRIGHTEN:
      ColorBuffer_BackupAndBrighten(source, work->transferValue,
                                    (void *)BG_CHAR_BLOCK(1), 0x4000);
      break;
    }
    work->transferPending = 0;
    counter = &work->framesSinceTransfer;
    nextCounter = 1;
  } else {
    counter = &work->framesSinceTransfer;
    nextCounter = *counter + 1;
  }
  *counter = nextCounter;
}

// Sharing the five-mode switch grows native English from 0xc0 to 0xd0 bytes.
// Inline fade helpers (with either work or value parameters) keep the sizes
// but move the source setup ahead of the destination and size setup.
// The same for the effects that draw a full-screen picture in
// gMapCellBuffer instead of the canvas: copies its 0x7800 bytes to VRAM
// 0x06008000 the way transferMode says (modes 0 to 3).
void BattleFx_FlushPendingGraphicsTransfer(void) {
  void **slots;
  struct BattleEffectWork *work;
  void *source;
  s32 transferMode;

  slots = gWorkSlot + HEAP_SLOT_BATTLE_EFFECT;
  work = slots[0];
  source = Ram_MapCellBuffer;
  if (work->transferPending != 1)
    return;

  transferMode = work->transferMode;
  switch (transferMode) {
  case CANVAS_COPY:
    Iwram_CopyWords((void *)BG_CHAR_BLOCK(2), source, 0x7800);
    break;
  case CANVAS_COPY_AND_CLEAR:
    Iwram_CopyWords((void *)BG_CHAR_BLOCK(2), source, 0x7800);
    Iwram_FillWords(source, 0x7800, work->transferValue);
    break;
  case CANVAS_COPY_AND_FADE:
    if (work->transferValue == 50) {
      ColorBuffer_BackupAndHalve(source, (void *)BG_CHAR_BLOCK(2), 0x7800);
    } else {
      ColorBuffer_BackupAndScaleThreeQuarters(source, (void *)BG_CHAR_BLOCK(2),
                                              0x7800);
    }
    break;
  case CANVAS_COPY_AND_DARKEN:
    ColorBuffer_BackupAndDarken(source, work->transferValue,
                                (void *)BG_CHAR_BLOCK(2), 0x7800);
    break;
  }

  work->transferPending = 0;
}

// One step of the background palette fade, in 1/65536ths of full
// brightness: 24 steps take it from full to 60%.
#define CANVAS_FADE_STEP 1092

// Writes the window, display and blend registers the effect keeps in its
// work, all at once.
void Graphics_ApplyWindowBlendRegisters(void) {
  struct BattleEffectWork *work = WORK_SLOT(HEAP_SLOT_BATTLE_EFFECT);

  REG_WIN0H = work->win0h;
  REG_WIN0V = work->win0v;
  REG_WIN1H = work->win1h;
  REG_WIN1V = work->win1v;
  REG_WININ = work->winin;
  REG_WINOUT = work->winout;
  REG_DISPCNT = work->dispcnt;
  REG_BLDCNT = work->bldcnt;
  REG_BLDALPHA = work->bldalpha;
}

// Writes the effect's background 2 reference point to the hardware.
void Display_ApplyBg2Reference(void) {
  struct BattleEffectWork *work = WORK_SLOT(HEAP_SLOT_BATTLE_EFFECT);

  REG_BG2X = work->bg2x;
  REG_BG2Y = work->bg2y;
}

// Scheduler callback while the canvas is open: for fadeFrames frames,
// darkens the battle backdrop's colours one CANVAS_FADE_STEP more each frame
// into background palettes 6 to 13, so the effect stands out.
void Palette_StepFadeTransfer(void) {
  struct BattleWork *battle = WORK_SLOT(HEAP_SLOT_BATTLE);
  struct BattleEffectWork *work = WORK_SLOT(HEAP_SLOT_BATTLE_EFFECT);

  if (work->fadeFrames > 0) {
    s32 step = ++work->fadeStep;

    Graphics_ScaleRgb555ClampedFar(battle->palette,
                                   (void *)&BG_PLTT_COLOR(6, 0),
                                   0x10000 - step * CANVAS_FADE_STEP, 128);
    work->fadeFrames--;
  }
}

// Clears every target's flash timer.
void BattleFx_ClearActorTimers(void) {
  struct BattleEffectWork *work = WORK_SLOT(HEAP_SLOT_BATTLE_EFFECT);

  Iwram_ClearWords(work->actorTimers, sizeof work->actorTimers);
}

// Counts down each target's flash timer, which ObjectGroup_UpdateMembers
// starts; when one runs out, the target gets its own palette back.
void ObjectGroup_TickMemberTimers(void) {
  struct BattleEffectWork *work;
  s32 actorIndex;

  work = WORK_SLOT(HEAP_SLOT_BATTLE_EFFECT);
  actorIndex = 0;
  do {
    if (work->actorTimers[actorIndex] != 0) {
      if ((work->actorTimers[actorIndex] = work->actorTimers[actorIndex] - 1) ==
          0) {
        ObjectGroup_UpdateMembers(work->effect->actors[actorIndex], 0, -1, -1,
                                  0);
      }
    }
    actorIndex += 1;
  } while (actorIndex != 8);
}

// The canvas layer: BG2 shows the effect's 128x128 8bpp canvas (0x4000
// bytes), its tiles from character block 1, stretched to twice the width
// so it spans 256 pixels.
//
// REG_DISPCNT values used here (mode 1, 1D sprites):
//   0x7741 : BG0 to BG2, sprites, windows 0 and 1
//   0x7341 : BG0, BG1, sprites, windows 0 and 1 (BG2 hidden)
//   0x7541 : BG0, BG2, sprites, windows 0 and 1 (BG1 hidden)
//   0x1741 : BG0 to BG2, sprites
//   0x1341 : BG0, BG1, sprites
//   0x1541 : BG0, BG2, sprites

// Sharing the tilemap loop shortened BeginCanvasLayer and OpenCanvasLayer
// by four bytes each in the English trial; retain their original loop scopes.
// Opens the canvas layer: starts the background palette fade, makes BG2 the
// canvas, alpha-blended over the battle, inside the effect windows, and
// clears the canvas.
// bgControl : added to BG2CNT, the layer's priority (0 to 3) and so on.
void BattleFx_BeginCanvasLayer(s32 bgControl) {
  struct BattleEffectWork *work = *(gWorkSlot + HEAP_SLOT_BATTLE_EFFECT);
  struct BattleWork *battle = *(gWorkSlot + HEAP_SLOT_BATTLE);
  void *canvas = *(gWorkSlot + HEAP_SLOT_BATTLE_CANVAS);
  struct BattleBackgroundView *display =
      *(gWorkSlot + HEAP_SLOT_BATTLE_BACKGROUND);
  s32 row;
  s32 column;
  s32 offset;

  BattleFx_ClearActorTimers();
  display->secondMode = 1;
  WaitFrames(1);
  REG_BLDCNT = 0;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, EFFECT_DISPCNT, 0x20000);
  gBgScroll[1].y = 32;
  WaitFrames(1);

  // Start the background palette fade.
  BattlePresentation_ConfigurePaletteFadeFar(1, battle->background, 0);
  work->fadeFrames = 0;
  work->fadeStep = 0;
  Scheduler_AddOrUpdateCallback(Palette_StepFadeTransfer, 0xc80);

  // Change BG2's settings while it is hidden.
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                          DISPCNT_BG1_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON |
                          DISPCNT_WIN1_ON,
                      0x20000);
  WaitFrames(1);
  REG_BG2CNT = bgControl | 0x784;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                          DISPCNT_BG1_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON |
                          DISPCNT_WIN1_ON,
                      0x20000);
  BattlePres_SetupTransitionSceneFar(0, 0, 0, 100);
  display->secondMode = 0;
  WaitFrames(1);
  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BLDALPHA = BLEND_ALPHA(14, 16);
  EFFECT_SET_CANVAS_AFFINE();
  EFFECT_SET_BATTLE_WINDOWS();
  REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG2 |
              WININ_WIN1_OBJ | WININ_WIN1_CLR;
  REG_WINOUT = EFFECT_WINOUT;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, EFFECT_DISPCNT, 0x20000);

  // The screen map: the 16x16 tiles in order. An affine map has one byte
  // per tile, so they are written two at a time.
  offset = 0;
  for (row = 0; row != 16; row++) {
    EFFECT_WRITE_CANVAS_MAP_ROW(row, column, offset);
  }

  Iwram_ClearWords(canvas, 0x4000);
  Iwram_ClearWords(BG_CHAR_BLOCK(1), 0x4000);
  work->shakeFrames = 0;
  work->savedBg1X = gBgScroll[1].x;
  work->savedBg1Y = gBgScroll[1].y;
  WaitFrames(1);
}

// Opens the canvas layer as BattleFx_BeginCanvasLayer does, but with the
// windows off until the end, the backdrop palette starting half faded, and
// a 24-frame fade.
// bgControl : added to BG2CNT, the layer's priority (0 to 3) and so on.
void BattleFx_OpenCanvasLayer(s32 bgControl) {
  struct BattleEffectWork *work = *(gWorkSlot + HEAP_SLOT_BATTLE_EFFECT);
  struct BattleWork *battle = *(gWorkSlot + HEAP_SLOT_BATTLE);
  void *canvas = *(gWorkSlot + HEAP_SLOT_BATTLE_CANVAS);
  struct BattleBackgroundView *display =
      *(gWorkSlot + HEAP_SLOT_BATTLE_BACKGROUND);
  s32 row;
  s32 column;
  s32 offset;

  BattleFx_ClearActorTimers();
  display->secondMode = 1;
  WaitFrames(1);
  REG_BLDCNT = 0;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                          DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_OBJ_ON,
                      0x20000);
  gBgScroll[1].y = 32;
  WaitFrames(1);
  BattlePresentation_ConfigurePaletteFadeFar(1, battle->background, 128);
  work->fadeFrames = 24;
  work->fadeStep = 0;
  Scheduler_AddOrUpdateCallback(Palette_StepFadeTransfer, 0xc80);
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                          DISPCNT_BG1_ON | DISPCNT_OBJ_ON,
                      0x20000);
  WaitFrames(1);
  REG_BG2CNT = bgControl | 0x784;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                          DISPCNT_BG1_ON | DISPCNT_OBJ_ON,
                      0x20000);
  BattlePres_SetupTransitionSceneFar(0, 0, 0, 100);
  display->secondMode = 0;
  WaitFrames(1);

  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BLDALPHA = BLEND_ALPHA(14, 16);
  EFFECT_SET_CANVAS_AFFINE();
  EFFECT_SET_BATTLE_WINDOWS();
  REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG2 |
              WININ_WIN1_OBJ | WININ_WIN1_CLR;
  REG_WINOUT = EFFECT_WINOUT;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, EFFECT_DISPCNT, 0x20000);
  WaitFrames(1);

  offset = 0;
  for (row = 0; row != 16; row++) {
    EFFECT_WRITE_CANVAS_MAP_ROW(row, column, offset);
  }

  Iwram_ClearWords(canvas, 0x4000);
  Iwram_ClearWords(BG_CHAR_BLOCK(1), 0x4000);
  WaitFrames(1);
}

// Opens the canvas layer with BG2 a wrapping 256x256 map: each row of the
// map is the canvas's 16 tiles followed by 16 of tile 0, so the canvas
// repeats across the screen with gaps.
// bg_control : added to BG2CNT, the layer's priority (0 to 3) and so on.
void BattleFx_BeginTiledCanvas(s32 bgControl) {
  s32 row;
  s32 column;
  s32 offset;

  BattleFx_BeginCanvasLayer(bgControl);
  REG_BG2CNT = bgControl | 0x6784;
  offset = 0;
  for (row = 0; row != 16; row++) {
    EFFECT_WRITE_CANVAS_MAP_ROW(row, column, offset);
    for (column = 0; column != 8; column++, offset += 2) {
      *(volatile u16 *)((u8 *)BG_CANVAS_MAP + offset) = 0;
    }
  }
}

// Closes the canvas layer: plays the closing sound, restores BG1's scroll
// and the projection centre, clears the canvas tiles, stops the palette
// fade and brings the backdrop's palette back from level 21 to 0 over 8
// frames, then shows BG2 again and hides BG1.
void BattleFx_EndCanvasLayer(void) {
  struct BattleEffectWork *work = *(gWorkSlot + HEAP_SLOT_BATTLE_EFFECT);
  struct BattleWork *battle = *(gWorkSlot + HEAP_SLOT_BATTLE);
  s32 i;

  AudioCommand_PlayFar(289);
  gBgScroll[1].x = work->savedBg1X;
  gBgScroll[1].y = work->savedBg1Y;
  gProjection.centerX = 120;
  gProjection.centerY = 120;
  REG_BG2CNT = BGCNT_PRIORITY(3) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7);
  Iwram_ClearWords(BG_CHAR_BLOCK(1), 0x4000);
  Scheduler_RemoveCallback(Palette_StepFadeTransfer);
  gBgScroll[1].y = 32;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                          DISPCNT_BG1_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON |
                          DISPCNT_WIN1_ON,
                      0x20000);
  REG_BLDCNT = 0;
  WaitFrames(1);

  BattlePresentation_ConfigurePaletteFadeFar(2, battle->background, 7);
  WaitFrames(1);
  for (i = 0; i != 8; i++) {
    BattlePresentation_SetPaletteLevelFar(battle->background, 21 - i * 3);
    WaitFrames(1);
  }

  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                          DISPCNT_BG2_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON |
                          DISPCNT_WIN1_ON,
                      0x20000);
  WaitFrames(1);
}

// Hides BG1 and the windows and starts bringing the backdrop's palette back.
void BattleFx_SetTransitionFlagAndDisplay(void) {
  struct BattleWork *battle;
  s32 *flag;

  flag = &((struct BattleBackgroundView *)
               Ram_WorkSlot[HEAP_SLOT_BATTLE_BACKGROUND])
              ->secondMode;
  battle = Ram_WorkSlot[HEAP_SLOT_BATTLE];
  *flag = 1;
  QueueIoWriteDelay2((u32)&REG_DISPCNT, 0x1541);
  WaitFrames(1);
  BattlePresentation_ConfigurePaletteFadeFar(2, battle->background, 0);
  WaitFrames(1);
}

// Sets up BG1, instead of BG2, as a 256-colour picture layer for the
// effects that draw full screen into gMapCellBuffer: a 16-row map of tiles
// 0x100 onwards beside blank tiles, BG1 blended over the rest, and the
// effect windows. Also clears the canvas.
void BattleFx_SetupCanvasTileMap(void) {
  // UB: indexing past scalar gBattleWork reaches other work slots. Named
  // slots add four bytes in the native EN trial (0x136 to 0x13a).
#ifdef UBFIX
  void *canvas = WORK_SLOT(HEAP_SLOT_BATTLE_CANVAS);
  struct BattleWork *screen = gBattleWork;
  struct BattleBackgroundView *layer = WORK_SLOT(HEAP_SLOT_BATTLE_BACKGROUND);
#else
  u8 **cells = (u8 **)&gBattleWork;
  void *canvas = cells[31];
  struct BattleWork *screen = (struct BattleWork *)cells[0];
  struct BattleBackgroundView *layer = (struct BattleBackgroundView *)cells[35];
#endif
  s32 row;
  s32 column;
  s32 byteOffset;

  BattleFx_ClearActorTimers();
  BattlePresentation_ConfigurePaletteFadeFar(2, screen->background, 0);
  gBgScroll[1].y = 32;
  layer->secondMode = 0;
  Iwram_ClearWords((void *)0x06003fc0, 64);
  Iwram_FillWords((void *)0x0600f900, 0x200, -1);
  byteOffset = 0;
  for (row = 0; row != 16; row++) {
    for (column = 0; column != 32; column++, byteOffset += 2) {
      if (column > 15)
        *(volatile u16 *)(0x0600fb00 + byteOffset) = 0xff;
      else
        *(volatile u16 *)(0x0600fb00 + byteOffset) = row * 16 + 0x100 + column;
    }
  }
  REG_DISPCNT = EFFECT_DISPCNT;
  REG_BG1CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(0) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(31);
  REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  EFFECT_SET_BATTLE_WINDOWS();
  REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG2 |
              WININ_WIN1_OBJ | WININ_WIN1_CLR;
  REG_WINOUT = EFFECT_WINOUT;
  REG_BLDALPHA = BLEND_ALPHA(14, 16);
  Iwram_ClearWords(canvas, 0x4000);
  Iwram_ClearWords(BG_CHAR_BLOCK(1), 0x4000);
  WaitFrames(1);
}

// Draws a line on the 128x128 8bpp canvas (16 tiles across), keeping the
// brighter colour where it crosses what is already drawn. The ends' y is
// kept on the canvas; x is not.
// x0, y0 : the start, in pixels
// x1, y1 : the end, in pixels
// color  : the colour index
void BattleFx_DrawClippedCanvasLine(s32 x0, s32 y0, s32 x1, s32 y1, s32 color) {
  s32 dx = x1 - x0;
  s32 dy = y1 - y0;
  s32 frac = 0x80; // 0.5 in 8-bit fixed point
  u8 *canvas = ((u8 **)gBattleFxWork)[1];
  s32 step;
  s32 i;
  s32 x;
  s32 y;
  s32 tmp;
  u32 offset;

  EFFECT_CLAMP(y0, 0, 127);
  EFFECT_CLAMP(y1, 0, 127);

  if (EFFECT_ABS(dx) < EFFECT_ABS(dy)) {
    // Steep: step y a pixel at a time and follow x in fixed point.
    if (dy < 0) {
      tmp = x0;
      x0 = x1;
      x1 = tmp;
      tmp = y0;
      y0 = y1;
      y1 = tmp;
      dx = x1 - x0;
      dy = y1 - y0;
    }
    step = (EFFECT_ABS(x1 - x0) << 8) / EFFECT_ABS(y1 - y0);
    x = x0;
    for (i = y0; i != y1; i++) {
      offset = (((((u32)i >> 3) << 4) + ((u32)x >> 3)) << 3) + (i & 7);
      offset = (offset << 3) + (x & 7);
      if (canvas[offset] < color)
        canvas[offset] = color;
      frac += step;
      if (frac & 0x100) {
        if (dx > 0)
          x++;
        else
          x--;
        frac &= ~0x100;
      }
    }
  } else {
    // Shallow: step x a pixel at a time and follow y in fixed point.
    if (dx < 0) {
      tmp = x0;
      x0 = x1;
      x1 = tmp;
      tmp = y0;
      y0 = y1;
      y1 = tmp;
      dx = x1 - x0;
      dy = y1 - y0;
    }
    step = (EFFECT_ABS(y1 - y0) << 8) / EFFECT_ABS(x1 - x0);
    y = y0;
    for (i = x0; i != x1; i++) {
      offset = (((((u32)y >> 3) << 4) + ((u32)i >> 3)) << 3) + (y & 7);
      offset = (offset << 3) + (i & 7);
      if (canvas[offset] < color)
        canvas[offset] = color;
      frac += step;
      if (frac & 0x100) {
        if (dy > 0)
          y++;
        else
          y--;
        frac &= ~0x100;
      }
    }
  }
}

// Things thrown or flown at the targets: the leaping fireball, closing skulls,
// orbiting spirals, gathering lights and lobbed shells; and the mirrored
// walls.
//
// Each runner loads its pictures and blitters, places its particles, then
// plays a fixed number of frames. A frame resets the 3D transform to the
// camera, moves and draws every live particle, hits the targets on their
// scheduled frames and presents the canvas. The order of the steps within a
// frame matters: later steps draw over earlier ones, and several of them draw
// random numbers.
//
// - Planet Diver flashes the user, throws it into the air, turns it into a
//   ball of fire that falls on the target and bursts into 256 chips that
//   bounce on the ground.
// - The closing skulls (Haunt) gather around each target in turn, eight
//   skulls scattered in 3D that step towards a point above the target while
//   the group spins.
// - The orbiting spirals circle above each target in turn, four of them a
//   quarter turn apart.
// - The gathering lights burst out of the user and are pulled into each
//   target until they reach it.
// - The lobbed shells fly from the user to each target, tumbling, and burst
//   into smoke or shards where they land.
//
// Particle slots: the skulls take 64 slots of PARTICLES per target, of which
// 8 are used; the lights 128 per target; the shells 16 per target, of which
// the first flies. The impact-chip and shell-burst views below reuse the map
// buffer that other effects address as an array of EffectStep records. The
// skulls, spirals and lights ripple the effect layer throughout, through the
// HBlank DMA of BG2's affine origin.

// The leap that falls as a ball of fire.

// One chip of the impact, laid over the same 28-byte records as PARTICLES
// but with its velocities in other places than struct EffectStep's.
struct ImpactChip {
  s32 x; // canvas position, 16.16 fixed point
  s32 y;
  s32 velocityX;
  s32 unused0c;
  s32 velocityY;
  s32 unused14;
  s32 life; // frames left
};

#define IMPACT_CHIPS ((struct ImpactChip *)Ram_MapCellBuffer)

#define PLANET_DIVER_FRAMES 88          // frames in the effect
#define PLANET_DIVER_CLEARED_CHIPS 1024 // map-buffer chip slots cleared first
#define PLANET_DIVER_CHIPS 256          // chips the fireball bursts into

// Abilities: Planet Diver (kind 60).
//
// Battle effect 60: the user flashes and leaps into the air, turns into a
// ball of fire, falls onto the first target and bursts into chips that
// bounce on the ground. 88 frames.
// effect->side says which way the ball falls; actors[0] is where it lands.
void BattleFx_RunPlanetDiver(struct BattleEffectArgument *effect) {
  struct EffectPosition screenPosition;
  BlitterPair draw;
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  u8 *sprites;
  struct MotionObject *target;
  s32 dx;
  struct MotionObject *actor;
  u8 *palette;
  s32 frame;
  s32 i;

  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  sprites = EFFECT_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);
  Resource_DecodeType01(
      Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesA), sprites);
  // A sheet resource is its palette followed by its packed images.
  palette = Resource_GetTableEntry((s32)&ResourceId_FlashBurstSheet);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  palette += 128;
  Resource_DecodeType01(palette, work);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS, COMPOSITE_MAX);
  draw[0] = EFFECT_SLOT(slots, HEAP_SLOT_BLITTER);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_MAX);
  draw[1] = EFFECT_SLOT(slots, HEAP_SLOT_BLITTER_ALTERNATE);
  work->transferMode = CANVAS_COPY_AND_FADE; // trails keep 3/4 a frame
  work->transferValue = 75;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);

  for (i = 0; i != PLANET_DIVER_CLEARED_CHIPS; i++)
    IMPACT_CHIPS[i].life = 0;
  actor = GetBattleObjectSlotFar(work->effect->actor)->object;
  target = GetBattleObjectSlotFar(work->effect->actors[0])->object;
  // The ball falls 15 pixels a frame towards the middle of the field.
  if (actor->x > 0)
    dx = -(15 << 16);
  else
    dx = 15 << 16;

  for (frame = 0; frame != PLANET_DIVER_FRAMES; frame++) {
    struct BattleCamera *camera = gCameraWork;

    EFFECT_VIEW_FROM_CAMERA(camera);
    if (frame > 17 || frame == 0) {
      EffectPosition_ApplyAlternateStepAndYOffset(work->effect->actor,
                                                  &screenPosition);
      screenPosition.x /= 2;
    }

    // The flash: frames 2 and 3 on the user, frames 4 to 15 spreading in
    // 16 directions.
    if (frame >= 2 && frame <= 3)
      draw[0](canvas, work, screenPosition.x - 16, screenPosition.y - 64, 32,
              64);
    if (frame >= 4 && frame <= 15) {
      for (i = 0; i != 16; i++) {
        s32 x = screenPosition.x + (frame * Trig_Sin(i << 12) >> 16);
        s32 y = screenPosition.y + (frame * Trig_Cos(i << 12) >> 16) - frame;

        draw[0](canvas, work->sheet + ((frame - 4) / 2 << 11), x - 16, y - 64,
                32, 64);
      }
    }
    // Frame 4: the user leaps, heading out past its own position.
    if (frame == 4) {
      actor->velocityY = 20 << 16;
      actor->acceleration = 1 << 16;
      actor->speedLimit = 3 << 16;
      actor->verticalMotionStrength = 0xab85;
      actor->autoFaceMotion = 0;
      actor->snapToTarget = 0;
      Object_SetMoveTargetFar(actor, actor->x * 3, 0, actor->z);
      ObjectDispatch_ApplyArgumentToChildrenFar(actor, 2);
      work->shakeFrames = frame;
      AudioCommand_PlayFar(136);
    }
    // Frame 16: switch to the fireball images and line the user up with
    // the target's depth.
    if (frame == 16) {
      palette = Resource_GetTableEntry((s32)&ResourceId_FlameballSheet);
      Iwram_CopyWords((void *)BG_PLTT, palette, 128);
      palette += 128;
      Resource_DecodeType01(palette, work);
      actor->verticalMotionStrength = 0;
      actor->velocityX = 0;
      actor->velocityY = 0;
      actor->z = target->z;
      ObjectDispatch_InitFromTable6Far(actor);
    }
    // From frame 18: fall 15 pixels across and 8 down a frame; on reaching
    // the ground, throw out 256 chips, and the target recoils and flashes
    // as the battle shows the damage.
    if (frame > 17) {
      if (actor->y > 0) {
        actor->x += dx;
        actor->y -= 8 << 16;
        if (work->effect->side == 0) {
          draw[0](canvas, work, screenPosition.x - 20, screenPosition.y - 52,
                  40, 64);
          screenPosition.x -= 8;
        } else {
          draw[1](canvas, work, screenPosition.x - 26, screenPosition.y - 52,
                  40, 64);
          screenPosition.y += 8;
        }
      }
      if (actor->y < 0) {
        actor->y = 0;
        for (i = 0; i != PLANET_DIVER_CHIPS; i++) {
          struct ImpactChip *chip = &IMPACT_CHIPS[i];
          s32 speed;
          s32 angle;

          speed = 0x3ff;
          speed &= Random16();
          angle = Random16() & 0xffff;
          chip->x = screenPosition.x << 16;
          chip->y = (screenPosition.y - 24) << 16;
          chip->velocityX = Trig_Sin(angle) * (speed + 32) >> 6;
          chip->velocityY = -(Trig_Cos(angle) * (speed + 32) << 1) >> 6;
          chip->life = (Random16() & 7) + 32;
        }
        work->shakeFrames = 8;
        BattleEventRuntime_BeginPhaseFar(145);
        BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 4);
        ObjectGroup_UpdateMembers(work->effect->actors[0], SPRITE_COLOR_WHITE,
                                  POSE_DAMAGE, 0, 8);
      }
    }

    // Chips: slow to 56/64 of their speed each frame, fall, and bounce at
    // half speed off y 112; only those on the canvas are drawn.
    for (i = 0; i != PLANET_DIVER_CHIPS; i++) {
      struct ImpactChip *chip = &IMPACT_CHIPS[i];
      s32 y;

      if (chip->life > 0) {
        chip->x += chip->velocityX;
        y = chip->y + chip->velocityY;
        chip->life--;
        chip->y = y;
        chip->velocityX = chip->velocityX * 56 / 64;
        chip->velocityY = chip->velocityY * 56 / 64 + 0x2000;
        if (chip->y > 112 << 16) {
          chip->velocityY = -chip->velocityY / 2;
        } else if ((chip->x >= 0 && chip->x < 127 << 16) && chip->y >= 0) {
          s32 size = chip->life / 8 + 1;

          EFFECT_DRAW_MOTE(draw[i & 1], canvas, sprites, (chip->x >> 16),
                           (chip->y >> 16), size);
        }
      }
    }
    Camera_ApplyShake(16, 16);
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS();
}

// Closing skulls.

#define HAUNT_SCATTERED 512 // skull slots scattered at the start
#define HAUNT_SKULLS 8      // skulls drawn around each target
#define HAUNT_SLOTS 64      // particles reserved for each target

// Abilities: Haunt (kind 39).
//
// Battle effect 39: around each target in turn, 32 frames apart, eight
// skulls appear one every 4 frames, scattered up to 64 pixels away, and
// close in on a point 20 pixels above the target while the group spins; the
// effect layer ripples throughout. PARTICLES holds 64 skulls per target, of
// which the first 8 are used.
void BattleFx_RunHaunt(struct BattleEffectArgument *effect) {
  struct EffectPosition screenPosition;
  s32 point[3];
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  BlitterPair draw;
  struct BattleCamera *camera;
  u8 *palette;
  s32 i;
  s32 k;

  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);
  palette = Resource_GetTableEntry((s32)&ResourceId_SkullSheet);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  palette += 128;
  Resource_DecodeType01(palette, work);
  palette = Resource_GetTableEntry((s32)&ResourceId_PinkBurstSheet);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS, COMPOSITE_MAX);
  draw[0] = EFFECT_SLOT(slots, HEAP_SLOT_BLITTER);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_ADDITIVE);
  draw[1] = EFFECT_SLOT(slots, HEAP_SLOT_BLITTER_ALTERNATE);
  Scheduler_AddOrUpdateCallback(BattleFx_ArmBg2AffineHBlankDma, 0x480);
  work->transferMode = CANVAS_COPY_AND_DARKEN;
  work->transferValue = 0x04040404; // 4 off every pixel each frame
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);

  for (i = 0; i != HAUNT_SCATTERED; i++) {
    PARTICLES[i].x = ((Random16() & 255) - 127) << 15;
    PARTICLES[i].y = ((Random16() & 255) - 127) << 15;
    PARTICLES[i].z = ((Random16() & 255) - 127) << 15;
  }
  AudioCommand_PlayFar(142);

  for (frame = 0; frame != work->effect->count * 32 + 96; frame++) {
    s32 *row;

    camera = gCameraWork;
    if (frame == 96) // the battle shows the damage
      BattleEventRuntime_BeginPhaseFar(0);

    // Ripple the canvas sideways, line by line.
    row = work->bg2X;
    if (work->effect->side == 0) {
      for (i = 0; i != 160; i++)
        *row++ = (0x60000 - Trig_Sin((frame + i) << 11) * 6) >> 10;
    } else {
      for (i = 0; i != 160; i++)
        *row++ = Trig_Sin((frame + i) << 11) * 6 >> 10;
    }

    EFFECT_FOR_EACH_TARGET(k, work->effect) {
      struct MotionObject *target =
          GetBattleObjectSlotFar(work->effect->actors[k])->object;

      // Centre on the point 20 pixels above the target and spin the group
      // about it.
      EFFECT_VIEW_FROM_CAMERA(camera);
      EFFECT_SET_VECTOR(point, target->x, 20 << 16, target->z);
      SceneTransform_ApplyPosition(point);
      if (frame > k * 32) {
        SceneTransform_ApplyPitch(frame << 9);
        if (frame == k * 32 + 32)
          ObjectGroup_UpdateMembers(work->effect->actors[k], SPRITE_COLOR_WHITE,
                                    POSE_DAMAGE, k, 32);

        // Each skull steps one pixel nearer the centre a frame; skulls take
        // one of three images in turn.
        for (i = 0; i != HAUNT_SKULLS; i++) {
          struct EffectStep *skull = &PARTICLES[k * HAUNT_SLOTS + i];

          if (frame > (k * 8 + i) * 4) {
            s32 x = (skull->x >> 8) * (skull->x >> 8);
            s32 y = (skull->y >> 8) * (skull->y >> 8);
            s32 z = (skull->z >> 8) * (skull->z >> 8);
            s32 dist = Iwram_Sqrt(x + y + z) >> 8;

            if (dist != 0) {
              EffectPosition_ApplyBaseAndYOffset(skull, &screenPosition);
              screenPosition.x >>= 1;
              draw[0](canvas, work->sheet + i % 3 * 576, screenPosition.x - 12,
                      screenPosition.y - 12, 24, 24);
              skull->x -= skull->x / dist;
              skull->y -= skull->y / dist;
              skull->z -= skull->z / dist;
              skull->variant++;
            }
          }
        }
      }
    }
    EFFECT_END_FRAME(work);
  }

  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  Scheduler_RemoveCallback(BattleFx_ArmBg2AffineHBlankDma);
  EFFECT_RELEASE_BLITTERS();
  BattleFx_EndCanvasLayer();
}

#define ORBITING_SPIRALS_LIFE 60 // frames each target's spirals turn

// Battle effect 34: four spirals circle 40 pixels above each target in turn,
// 16 frames apart, for 60 frames each, 16 pixels out from the centre; each
// target takes its damage pose halfway through. The effect layer ripples
// throughout.
// Signed sine/cosine results are scaled by multiplication: negative left
// shifts were undefined, while these products stay within signed 32 bits.
void BattleFx_RunOrbitingSpirals(struct BattleEffectArgument *effect) {
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  void *palette;
  BlitterPair draw;
  s32 point[3];
  struct EffectPosition screen;
  s32 member;
  s32 offsetX;
  s32 frame;

  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);
  REG_BG2PA = Q_8_8(1);
  palette = Resource_GetTableEntry((s32)&ResourceId_SpiralSheet);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  Resource_DecodeType01((u8 *)palette + 128, work);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS, COMPOSITE_MAX);
  draw[0] = EFFECT_SLOT(slots, HEAP_SLOT_BLITTER);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP, COMPOSITE_MAX);
  draw[1] = EFFECT_SLOT(slots, HEAP_SLOT_BLITTER_ALTERNATE);
  Scheduler_AddOrUpdateCallback(BattleFx_ArmBg2AffineHBlankDma, 0x480);
  work->transferMode = CANVAS_COPY_AND_FADE; // trails keep half a frame
  work->transferValue = 50;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);
  // On the party's side the canvas is scrolled 104 pixels left.
  if (work->effect->side == 1) {
    REG_BG2X = -0x6800;
    offsetX = -112;
  } else {
    offsetX = 0;
  }
  for (frame = 0; frame != (work->effect->count * 16) + 48; frame++) {
    struct BattleCamera *camera;
    s32 *row;
    s32 i;

    // Ripple the canvas sideways, line by line.
    camera = gCameraWork;
    row = work->bg2X;
    if (work->effect->side == 0) {
      s32 angle;
      s32 base;

      for (i = 0, base = 0x80000, angle = frame << 10; i != 160; i++) {
        *row++ = (base - ((Trig_Sin(angle) * 8))) >> 10;
        angle += 1024;
      }
    } else {
      s32 angle;

      for (i = 0, angle = frame << 10; i != 160; i++) {
        *row++ = (((Trig_Sin(angle) * 8)) >> 10) - 0x7000;
        angle += 1024;
      }
    }
    EFFECT_VIEW_FROM_CAMERA(camera);
    member = 0;
    if (work->effect->count != 0) {
      while (member != work->effect->count) {
        struct MotionObject *obj;

        obj = GetBattleObjectSlotFar(work->effect->actors[member])->object;
        if (frame > member * 16 &&
            frame < (member * 16) + ORBITING_SPIRALS_LIFE) {
          s32 spin;

          if (frame == (member * 16) + 32) {
            ObjectGroup_UpdateMembers(work->effect->actors[member], 0,
                                      POSE_DAMAGE, -1, 0);
          }
          EFFECT_SET_VECTOR(point, obj->x, 40 << 16, obj->z);
          EffectPosition_ApplyBaseAndYOffset(point, &screen);

          // The spirals (32x32) a quarter turn apart, through four images,
          // turned upside down every other 16 frames.
          for (i = 0; i != 4; i++) {
            s32 x;
            s32 y;
            s32 half;

            spin = (frame << 9) + (i << 14);
            x = (screen.x + (((Trig_Sin(spin) * 16)) >> 16)) + offsetX;
            y = screen.y + (((Trig_Cos(spin) * 16)) >> 16);
            half = frame / 16;
            draw[half & 1](canvas,
                           work->sheet + (((frame / 4) - (half * 4)) << 10),
                           x - 16, y - 16, 32, 32);
          }
        }
        member++;
      }
    }
    EFFECT_PRESENT_FRAME(work);
  }
  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  Scheduler_RemoveCallback(BattleFx_ArmBg2AffineHBlankDma);
  EFFECT_RELEASE_BLITTERS();
  BattleFx_EndCanvasLayer();
}

// Lights that scatter from the user and gather into each target.

// Abilities: Demon Night (kind 50).
//
// Battle effect 50: pink lights.
DEFINE_EFFECT_KIND(BattleFx_RunDemonNight, BattleFx_RunGatheringLights, 0)

// Abilities: Rumble (kind 308).
//
// Battle effect 308: lights in Mars Djinn colours.
DEFINE_EFFECT_KIND(BattleFx_RunRumble, BattleFx_RunGatheringLights, 1)

// Abilities: Haunting (kind 351).
//
// Battle effect 351: lights in Mercury Djinn colours.
DEFINE_EFFECT_KIND(BattleFx_RunHaunting, BattleFx_RunGatheringLights, 2)

// gMemberBurst_Counts has two entries for each mode.
#define GATHERING_LIGHTS_COUNT(mode) gMemberBurst_Counts[(mode) * 2]
#define GATHERING_LIGHTS_FRAMES(mode) gMemberBurst_Counts[(mode) * 2 + 1]
#define GATHERING_LIGHTS_SLOTS 128 // particles reserved for each target

// For each target in turn, 20 frames apart, lights burst out of the user;
// 30 frames after each one appears it is pulled towards the middle of the
// target's body and goes out when it is within 32 pixels across. Each
// target is struck 70 frames after its turn starts, and the battle shows the
// damage at frame 80. The effect layer ripples throughout.
// mode : the colours (0 pink, 1 Mars Djinn, 2 Mercury Djinn) and the row of
//        gMemberBurst_Counts: lights per target and frames.
void BattleFx_RunGatheringLights(struct BattleEffectArgument *effect,
                                 s32 mode) {
  struct EffectPosition screenPosition;
  BlitterPair draw;
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 k;
  struct BattleCamera *camera;
  s32 paletteId;
  s32 i;

  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  camera = EFFECT_SLOT(slots, HEAP_SLOT_CAMERA);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(1);
  Resource_DecodeType01(
      Resource_GetTableEntry((s32)&ResourceId_MemberBurstImage), work);
  if (mode == 0)
    paletteId = (s32)&ResourceId_PinkBurstSheet;
  else if (mode == 1)
    paletteId = (s32)&ResourceId_MarsDjinnSheet;
  else
    paletteId = (s32)&ResourceId_MercuryDjinnSheet;
  Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(paletteId), 128);
  BattleFx_FetchRectangleBlitters(work->effect->side, draw);

  // The lights start at the top of the user's body and fly off in any
  // direction at up to 2 pixels a frame.
  EFFECT_FREE_PARTICLES(i, PARTICLES, 1024);
  EFFECT_FOR_EACH_TARGET(k, work->effect) {
    struct MotionObject *src =
        GetBattleObjectSlotFar(work->effect->actor)->object;
    s32 height = Battle_GetObjectTableValueFar(work->effect->actor);

    for (i = 0; i != GATHERING_LIGHTS_SLOTS; i++) {
      struct EffectStep *mote = &PARTICLES[k * GATHERING_LIGHTS_SLOTS + i];

      EFFECT_SET_POSITION(mote, src->x, height, src->z);
      EFFECT_SET_VELOCITY(mote, ((Random16() & 255) - 128) << 10,
                          ((Random16() & 255) - 128) << 10,
                          ((Random16() & 255) - 128) << 10);
      mote->variant = 0;
    }
  }

  Scheduler_AddOrUpdateCallback(BattleFx_ArmBg2AffineHBlankDma, 0x480);
  work->transferMode = CANVAS_COPY_AND_FADE; // trails keep 3/4 a frame
  work->transferValue = 75;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);
  AudioCommand_PlayFar(146);

  for (frame = 0;
       frame != GATHERING_LIGHTS_FRAMES(mode) + work->effect->count * 20;
       frame++) {
    s32 *row;

    if (frame == 80) {
      if (mode == 0)
        BattleEventRuntime_BeginPhaseFar(SOUND_HEAVY_IMPACT);
      else
        BattleEventRuntime_BeginPhaseFar(133);
    }
    EFFECT_VIEW_FROM_CAMERA(camera);

    // Ripple the canvas sideways, line by line.
    row = work->bg2X;
    for (i = 0; i != 160; i++)
      *row++ = (0x100000 - Trig_Sin((frame + i) << 10) * 16) >> 10;

    EFFECT_FOR_EACH_TARGET(k, work->effect) {
      struct MotionObject *target =
          GetBattleObjectSlotFar(work->effect->actors[k])->object;
      s32 half = Battle_GetObjectTableValueFar(work->effect->actors[k]) / 2;

      if (frame == k * 20 + 71) {
        if (mode == 0)
          AudioCommand_PlayFar(SOUND_HEAVY_IMPACT);
        else
          AudioCommand_PlayFar(133);
      }
      if (frame == k * 20 + 70)
        ObjectGroup_UpdateMembers(work->effect->actors[k], SPRITE_COLOR_WHITE,
                                  POSE_DAMAGE, k, 26);
      if (frame > k * 20) {
        for (i = 0; i != GATHERING_LIGHTS_COUNT(mode); i++) {
          struct EffectStep *mote = &PARTICLES[k * GATHERING_LIGHTS_SLOTS + i];

          if (frame > (k * 10 + i) * 2 && mote->variant >= 0) {
            EffectPosition_ApplyBaseAndYOffset(mote, &screenPosition);
            screenPosition.x >>= 1;
            draw[0](canvas, work->sheet + i % 3 * 640, screenPosition.x - 10,
                    screenPosition.y - 16, 20, 32);
            EffectStep_AdvanceWithGravity3D(mote, 62, 0);
            if (frame > k * 20 + i + 30) {
              s32 dx = (target->x - mote->x) >> 9;
              s32 dy = (target->y + half - mote->y) >> 9;
              s32 dz = (target->z - mote->z) >> 9;

              mote->velocityX += dx;
              mote->velocityY += dy;
              mote->velocityZ += dz;
              if ((dx >= -0xfff && dx <= 0xfff) &&
                  (dz >= -0xfff && dz <= 0xfff))
                mote->variant = -1;
            }
          }
        }
      }
    }
    EFFECT_END_FRAME(work);
  }

  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  EFFECT_RELEASE_BLITTERS();
  Scheduler_RemoveCallback(BattleFx_ArmBg2AffineHBlankDma);
  BattleFx_EndCanvasLayer();
}

// The canvas blitters, and the 48-frame wall effects.

// Builds the two usual routines that draw onto the 128x128 canvas.
// flip : nonzero draws mirrored.
// out  : [0] keeps the higher colour index, [1] adds the colours (up to 63).
void BattleFx_FetchRectangleBlitters(s32 flip, BlitterPair out) {
  if (flip == 0) {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                        COMPOSITE_MAX);
    out[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                        COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                        COMPOSITE_ADDITIVE);
    out[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
  } else {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER,
                        COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X,
                        COMPOSITE_MAX);
    out[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                        COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X,
                        COMPOSITE_ADDITIVE);
    out[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
  }
}

// Abilities: Berserk (kind 353).
//
// Battle effect 353: the wall rises wide and shakes the screen hard.
DEFINE_EFFECT_KIND(BattleFx_RunBerserk, BattleFx_RunMirroredWall, 1)

// Abilities: Spasm (kind 348).
//
// Battle effect 348: the wall rises with a long gentle shake and the
// recovery sound.
DEFINE_EFFECT_KIND(BattleFx_RunSpasm, BattleFx_RunMirroredWall, 0)

// Abilities: Recovery (kind 389).
//
// Battle effect 389: a wide wall of fire, without a shake.
DEFINE_EFFECT_KIND(BattleFx_RunRecovery, BattleFx_RunMirroredWall, 2)

// Raises a symmetrical wall (the EarthWall images) at the first target's
// feet for 48 frames; the target glows and the battle shows the result on
// frame 24.
// mode : 0 actual size, a gentle shake for 32 frames
//        1 1.25 times as wide, a hard shake for 8 frames
//        2 1.25 times as wide, no shake, in the Fireball colours
void BattleFx_RunMirroredWall(struct BattleEffectArgument *effect, s32 mode) {
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  u8 *palette;
  s32 x;
  struct EffectPosition screenPosition;
  BlitterPair draw;
  void **slots;
  s32 frame;
  s32 cell;

  cursor = gBattleFxWork;
  EFFECT_GET_WORK_AND_CANVAS(cursor, work, canvas);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);

  // The EarthWall images, coloured with the Mars Djinn palette (the Fireball
  // palette in mode 2).
  palette = Resource_GetTableEntry((s32)&ResourceId_EarthWallSheet);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  palette += 128;
  Resource_DecodeType01(palette, work);
  palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  if (mode == 2) {
    palette = Resource_GetTableEntry((s32)&ResourceId_FireballSheet);
    Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  }

  // Scroll BG2 so the canvas's x 64 is on the target.
  EffectPosition_ApplyAlternateStepAndYOffset(work->effect->actors[0],
                                              &screenPosition);
  if (mode == 0) {
    REG_BG2PA = Q_8_8(1);
    x = 64 - screenPosition.x;
    REG_BG2X = x << 8;
  } else {
    REG_BG2PA = Q_8_8(0.8); // shown 1.25 times as wide
    x = (-screenPosition.x * 4) / 5 + 64;
    REG_BG2X = x << 8;
  }

  // The left half as drawn, the right half mirrored, keeping the higher
  // colour.
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                      COMPOSITE_MAX);
  slots = gWorkSlot;
  draw[0] = slots[HEAP_SLOT_BLITTER];
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                      COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X,
                      COMPOSITE_MAX);
  draw[1] = slots[HEAP_SLOT_BLITTER_ALTERNATE];
  work->transferMode = CANVAS_COPY_AND_FADE; // trails keep half a frame
  work->transferValue = 50;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);

  if (mode == 2) {
    work->shakeFrames = 0;
    AudioCommand_PlayFar(212);
  } else if (mode == 1) {
    work->shakeFrames = 8;
    AudioCommand_PlayFar(212);
  } else {
    work->shakeFrames = 32;
  }

  for (frame = 0; frame != 48; frame++) {
    if (frame == 0) {
      if (mode == 2)
        ObjectGroup_UpdateMembers(work->effect->actors[0], SPRITE_COLOR_WHITE,
                                  -1, 0, 32);
      else
        ObjectGroup_UpdateMembers(work->effect->actors[0],
                                  SPRITE_COLOR_RED_TINT, -1, 0, 32);
    }
    if (frame == 24)
      BattleEventRuntime_BeginPhaseFar(0);
    if (frame == 8 && mode == 0)
      AudioCommand_PlayFar(SOUND_RECOVERY);

    // Images 0, 1 and 2, four frames each, then 1 and 2 by turns, up to
    // frame 27.
    if (frame <= 31) {
      cell = frame / 4;
      if (cell > 2)
        cell = (cell & 1) + 1;
      if (frame <= 27) {
        draw[0](canvas, work->sheet + gEarthWall_CellSourceOffsets[cell],
                64 - gEarthWall_CellWidths[cell],
                (screenPosition.y - gEarthWall_CellHeights[cell]) + 8,
                gEarthWall_CellWidths[cell], gEarthWall_CellHeights[cell]);
        draw[1](canvas, work->sheet + gEarthWall_CellSourceOffsets[cell], 64,
                (screenPosition.y - gEarthWall_CellHeights[cell]) + 8,
                gEarthWall_CellWidths[cell], gEarthWall_CellHeights[cell]);
      }
    }
    if (mode == 0)
      Camera_ApplyShake(2, 2);
    else
      Camera_ApplyShake(16, 16);
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS();
}

// Beams of light: a Djinni appears, motes drift down, and a column of
// light falls from the sky onto each target.

// Images in work->sheet.
#define DJINN_BEAM_MOTE_CELLS 0x2710   // the motes (CrescentMoonSheet, 24x24)
#define DJINN_BEAM_DJINNI_CELLS 0x65c0 // the Djinni (40x40)

// Abilities: Revive, Water of Life (kind 25); Quartz (kind 201).
//
// Battle effects 25 and 201: with a Venus Djinni.
DEFINE_EFFECT_KIND(BattleFx_RunRevive, BattleFx_RunDjinnBeams, 0)

// Abilities: Dew (kind 209).
//
// Battle effect 209: with a Mercury Djinni.
DEFINE_EFFECT_KIND(BattleFx_RunDew, BattleFx_RunDjinnBeams, 1)

// When effect->prelude is 1, a Djinni appears, sways and then rises out of
// sight. Over each target in turn, 16 frames apart, eight motes drift down,
// and from 72 frames after its turn starts a column of light falls on it
// and narrows away; the battle shows the result on frame 80.
// mode : 0 a Venus Djinni, 1 a Mercury Djinni.
void BattleFx_RunDjinnBeams(struct BattleEffectArgument *effect, s32 mode) {
  struct EffectPosition one;
  struct EffectPosition screenPosition;
  s32 ground[3];
  s32 point[3];
  s32 djinnX;
  s32 djinnY;
  BlitterPair draw;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 offsetX;
  u8 *palette;
  s32 i;
  s32 k;

  cursor = gBattleFxWork;
  EFFECT_GET_WORK_AND_CANVAS(cursor, work, canvas);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);
  if (work->effect->prelude == 1)
    BattleFx_PrepareCanvasEffect(effect, mode,
                                 work->effect->side ^ work->effect->prelude, 0,
                                 &djinnX, &djinnY);
  palette = Resource_GetTableEntry((s32)&ResourceId_EarthWallSheet);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  palette += 128;
  Resource_DecodeType01(palette, work);
  palette = Resource_GetTableEntry((s32)&ResourceId_CrescentMoonSheet);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  palette += 128;
  Resource_DecodeType01(palette, work->sheet + DJINN_BEAM_MOTE_CELLS);
  if (mode == 0)
    palette = Resource_GetTableEntry((s32)&ResourceId_VenusDjinnSheet);
  else
    palette = Resource_GetTableEntry((s32)&ResourceId_MercuryDjinnSheet);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  palette += 128;
  Resource_DecodeType01(palette, work->sheet + DJINN_BEAM_DJINNI_CELLS);

  // The motes start 80 pixels up, each at a random point of its sway.
  for (i = 0; i != 512; i++) {
    PARTICLES[i].x = 0;
    PARTICLES[i].y = 80 << 16;
    PARTICLES[i].z = (Random16() | -32) << 14;
    PARTICLES[i].variant = Random16() & 255;
  }

  // With one target, scroll BG2 so the target is at the canvas's x 64.
  REG_BG2PA = Q_8_8(1);
  if (work->effect->count == 1) {
    EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &one);
    offsetX = 64 - one.x;
  } else {
    if (work->effect->side == 1)
      offsetX = -112;
    else
      offsetX = 0;
  }
  REG_BG2X = offsetX << 8;
  work->transferMode = CANVAS_COPY_AND_FADE; // trails keep half a frame
  work->transferValue = 50;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);

  for (frame = 0; frame != work->effect->count * 16 + 116; frame++) {
    struct BattleCamera *camera = gCameraWork;

    if (frame == 64)
      AudioCommand_PlayFar(212);
    if (frame == 80)
      BattleEventRuntime_BeginPhaseFar(0);

    // The Djinni sways 20 pixels either way and from frame 32 rises 2
    // pixels a frame.
    k = work->effect->prelude;
    if (k == 1) {
      s32 angle = frame << 11;
      s32 x = (-Trig_Sin(angle) * 20 >> 16) + djinnX + offsetX - 20;
      s32 y = (Trig_Cos(angle) * 4 >> 16) + djinnY - 24;

      BattleFx_FetchRectangleBlitters(work->effect->side ^ k, draw);
      if (frame > 32)
        y = y - frame * 2 + 64;
      draw[1](canvas, work->sheet + DJINN_BEAM_DJINNI_CELLS, x, y, 40, 40);
      if (frame <= 3)
        draw[1](canvas, work->sheet + DJINN_BEAM_DJINNI_CELLS, x, y, 40, 40);
      EFFECT_RELEASE_BLITTERS();
    }
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS, COMPOSITE_MAX);
    draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_MAX);
    draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];

    // Motes: one every 8 frames, swaying up to 16 pixels either way as
    // they sink a pixel a frame; they go out 8 pixels above the ground.
    EFFECT_FOR_EACH_TARGET(k, work->effect) {
      struct MotionObject *target =
          GetBattleObjectSlotFar(work->effect->actors[k])->object;
      s32 start = k * 16;

      EFFECT_VIEW_FROM_CAMERA(camera);
      EFFECT_SET_VECTOR(point, target->x, 0, target->z);
      SceneTransform_ApplyPosition(point);
      if (frame > start) {
        for (i = 0; i != 8; i++) {
          struct EffectStep *mote = &PARTICLES[k * 64 + i];

          if (frame > i * 8 + start && mote->y > 8 << 16) {
            s32 sway;

            EffectPosition_ApplyBaseAndYOffset(mote, &screenPosition);
            screenPosition.x += offsetX;
            sway = Trig_Sin(mote->variant << 10) * 16 >> 16;
            if (i & 1)
              screenPosition.x -= sway;
            else
              screenPosition.x += sway;
            draw[i & 1](canvas,
                        work->sheet +
                            gMemberBeam_MotePictures[mote->variant / 8 & 7] *
                                576 +
                            DJINN_BEAM_MOTE_CELLS,
                        screenPosition.x - 12, screenPosition.y - 12, 24, 24);
            mote->y -= 1 << 16;
            mote->variant++;
          }
        }
      }
    }

    // The column: drawn in two mirrored halves from the top of the canvas
    // down to the target's feet, 6 pixels each, narrowing by a pixel every
    // 3 frames from 16 frames after it falls. The target glows meanwhile.
    EFFECT_FOR_EACH_TARGET(i, work->effect) {
      s32 start = i * 16;

      if (frame >= start + 72) {
        struct MotionObject *target =
            GetBattleObjectSlotFar(work->effect->actors[i])->object;

        EFFECT_VIEW_FROM_CAMERA(camera);
        if (frame == start + 72)
          ObjectGroup_UpdateMembers(work->effect->actors[i], SPRITE_COLOR_GRB,
                                    -1, -1, 0);
        if (frame == start + 88)
          ObjectGroup_UpdateMembers(work->effect->actors[i], 0, -1, -1, 0);
        EFFECT_SET_VECTOR(ground, target->x, 0, target->z);
        EffectPosition_ApplyBaseAndYOffset(ground, &screenPosition);
        screenPosition.x += offsetX;
        if (frame < start + 104) {
          s32 pic = frame / 4;
          s32 width = 6;

          if (frame > start + 88)
            width = 6 - (frame - start - 88) / 3;
          if (pic > 2)
            pic = (pic & 1) + 1;
          // The glow at the target's feet.
          if (frame < start + 100) {
            draw[0](canvas, work->sheet + gMemberBeam_HeadOffsets[pic],
                    screenPosition.x - gMemberBeam_HeadWidths[pic],
                    screenPosition.y - gMemberBeam_HeadHeights[pic] + 8,
                    gMemberBeam_HeadWidths[pic], gMemberBeam_HeadHeights[pic]);
            draw[1](canvas, work->sheet + gMemberBeam_HeadOffsets[pic],
                    screenPosition.x,
                    screenPosition.y - gMemberBeam_HeadHeights[pic] + 8,
                    gMemberBeam_HeadWidths[pic], gMemberBeam_HeadHeights[pic]);
          }
          // The column, a row at a time.
          for (k = 0; k != screenPosition.y; k++) {
            draw[0](canvas, work->sheet + 5, screenPosition.x - width, k, width,
                    1);
            draw[1](canvas, work->sheet + 5, screenPosition.x, k, width, 1);
          }
        }
      }
    }
    EFFECT_RELEASE_BLITTERS();
    EFFECT_PRESENT_FRAME(work);
  }

  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  BattleFx_EndCanvasLayer();
}

// Lobbed shells: a shell arcs from the user onto each target and bursts
// into smoke or shards, in seven colourings.

// Abilities: Mad Spatter (kind 347).
//
// Battle effect 347: gold shells that burst into shards.
DEFINE_EFFECT_KIND(BattleFx_RunMadSpatter, BattleFx_RunLobbedShells, 4)

// Abilities: Decompose (kind 350).
//
// Battle effect 350: green shells that burst into smoke.
DEFINE_EFFECT_KIND(BattleFx_RunDecompose, BattleFx_RunLobbedShells, 5)

// Abilities: Bone Chiller (kind 309).
//
// Battle effect 309: blue shells that burst into smoke.
DEFINE_EFFECT_KIND(BattleFx_RunBoneChiller, BattleFx_RunLobbedShells, 0)

// Abilities: Sticky Goo (kind 315).
//
// Battle effect 315: shells in the spider web's colours that burst into
// smoke.
DEFINE_EFFECT_KIND(BattleFx_RunStickyGoo, BattleFx_RunLobbedShells, 1)

// Abilities: Poison Ink (kind 340).
//
// Battle effect 340: shells in Mars Djinn colours that burst into smoke.
DEFINE_EFFECT_KIND(BattleFx_RunPoisonInk, BattleFx_RunLobbedShells, 6)

// Abilities: Sticky Poison (kind 322).
//
// Battle effect 322: lime shells that burst into smoke.
DEFINE_EFFECT_KIND(BattleFx_RunStickyPoison, BattleFx_RunLobbedShells, 2)

// Abilities: Rotten Blood (kind 330).
//
// Battle effect 330: green shells that burst into shards.
DEFINE_EFFECT_KIND(BattleFx_RunRottenBlood, BattleFx_RunLobbedShells, 3)

// gLobbedShells_Records has two entries per style; the first is nonzero when
// the shells burst into shards instead of smoke.
extern s8 gLobbedShells_Records[];
extern u8 gLobbedShells_ShardWidths[];
extern u8 gLobbedShells_ShardHeights[];
extern u16 gLobbedShells_ShardOffsets[];

// The bursts live after the shells in gMapCellBuffer.
#define SHELL_BURSTS ((struct EffectStep *)(Ram_MapCellBuffer + 0x1c00))
#define LOBBED_SHELLS_SLOTS 16           // particles reserved for each target
#define LOBBED_SHELLS_BURST_SLOTS 256    // burst particles after the shells
#define LOBBED_SHELLS_SMOKE_CELLS 0x3e80 // the smoke pictures in the sheet

// Nonzero when the style's shells burst into shards instead of smoke.
#define LOBBED_SHELLS_SHATTER(mode) gLobbedShells_Records[(mode) * 2]

// Lobs a shell from the user at each target in turn, 8 frames apart. Each
// tumbles through ten images as it flies and, when it lands, the target
// flashes and the battle shows the damage; the shell bursts into 4 smoke
// puffs that drift up or 16 shards that fly up and fall.
// mode : the style, 0 to 6: the colours and the row of gLobbedShells_Records.
void BattleFx_RunLobbedShells(struct BattleEffectArgument *effect, s32 mode) {
  struct EffectPosition position;
  BlitterPair draw;
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 k;
  s32 total;
  struct MotionObject *source;
  struct BattleCamera *camera;
  u8 *palette;
  s32 i;
  s32 height;

  slots = gWorkSlot + HEAP_SLOT_BATTLE_EFFECT;
  cursor = slots;
  EFFECT_GET_WORK_AND_CANVAS(cursor, work, canvas);
  camera = EFFECT_SLOT(slots, HEAP_SLOT_CAMERA);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(1);
  REG_BLDALPHA = BLEND_ALPHA(16, 16); // an even mix

  // The shell images, the smoke at 0x3e80 and the style's palette.
  palette = Resource_GetTableEntry((s32)&ResourceId_GoldShellSheet);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  palette += 128;
  Resource_DecodeType01(palette, work);
  palette = Resource_GetTableEntry((s32)&ResourceId_SmokeSheet);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  palette += 128;
  Resource_DecodeType01(palette, work->sheet + LOBBED_SHELLS_SMOKE_CELLS);
  if (mode == 0)
    palette = Resource_GetTableEntry((s32)&ResourceId_BluePalette);
  else if (mode == 1)
    palette = Resource_GetTableEntry((s32)&ResourceId_SpiderWebSheet);
  else if (mode == 2)
    palette = Resource_GetTableEntry((s32)&ResourceId_LimePalette);
  else if (mode == 3)
    palette = Resource_GetTableEntry((s32)&ResourceId_GreenPalette);
  else if (mode == 4)
    palette = Resource_GetTableEntry((s32)&ResourceId_GoldShellSheet);
  else if (mode == 6)
    palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
  else
    palette = Resource_GetTableEntry((s32)&ResourceId_GreenPalette);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);

  // Each target's shells start 4 pixels up at the user, with a 16-frame
  // flight to the target and an upward throw.
  EFFECT_FREE_PARTICLES(i, PARTICLES, 1024);
  source = GetBattleObjectSlotFar(work->effect->actor)->object;
  height = 0x40000;
  EFFECT_FOR_EACH_TARGET(k, work->effect) {
    struct MotionObject *target =
        GetBattleObjectSlotFar(work->effect->actors[k])->object;

    for (i = 0; i != LOBBED_SHELLS_SLOTS; i++) {
      struct EffectStep *shot = &PARTICLES[k * LOBBED_SHELLS_SLOTS + i];

      EFFECT_SET_POSITION(shot, source->x, height, source->z);
      EFFECT_SET_VELOCITY(shot, (target->x - source->x) >> 4, 0x40000,
                          (target->z - source->z) >> 4);
      shot->variant = 0;
    }
  }
  EFFECT_FREE_PARTICLES(i, SHELL_BURSTS, LOBBED_SHELLS_BURST_SLOTS);

  // draw[0] draws the shells facing their flight; draw[1] draws the smoke
  // additively, or the shards mirrored the other way from draw[0].
  if (effect->side == 0) {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS, COMPOSITE_MAX);
    draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
    if (LOBBED_SHELLS_SHATTER(mode) == 0) {
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE, EFFECT_CLIP_BOUNDS,
                          COMPOSITE_ADDITIVE);
      draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
    } else {
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_MAX);
      draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
    }
  } else {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_MAX);
    draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
    if (LOBBED_SHELLS_SHATTER(mode) == 0) {
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE, EFFECT_CLIP_BOUNDS,
                          COMPOSITE_ADDITIVE);
      draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
    } else {
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE, EFFECT_CLIP_BOUNDS,
                          COMPOSITE_MAX);
      draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
    }
  }
  work->transferMode = CANVAS_COPY_AND_FADE; // trails keep 3/4 a frame
  work->transferValue = 75;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);
  if (LOBBED_SHELLS_SHATTER(mode) == 0)
    total = work->effect->count * 8 + 72;
  else
    total = work->effect->count * 8 + 56;
  AudioCommand_PlayFar(103);

  for (frame = 0; frame != total; frame++) {
    struct EffectStep *burst;

    EFFECT_VIEW_FROM_CAMERA(camera);
    // The shells: target k's shell flies from frame 8k, and the target is
    // hit on frame 8k + 17.
    k = 0;
    if (work->effect->count != 0) {
      do {
        if (frame >= k * 8) {
          struct EffectStep *shot = &PARTICLES[k * LOBBED_SHELLS_SLOTS];

          if (frame == k * 8 + 17) {
            ObjectGroup_UpdateMembers(work->effect->actors[k],
                                      SPRITE_COLOR_WHITE, POSE_DAMAGE, k, 16);
            BattleEventRuntime_BeginPhaseFar(133);
          }
          /* FAKEMATCH: needed to match: restores the CSE block boundary after
           * the hit block (k << 3 is recomputed). Without the block: differ
           * 449; without both of this function's empty blocks: differ 337. */
          do {
          } while (0);
          if (shot->variant >= 0) {
            s32 step = (frame - k * 8) / 3;

            if (step > 9)
              step = 9;
            EffectPosition_ApplyBaseAndYOffset(shot, &position);
            position.x >>= 1;
            if (step > 4)
              draw[0](canvas, work->sheet + step * 768, position.x - 16,
                      position.y - 12, 32, 24);
            else
              draw[0](canvas, work->sheet + step * 768, position.x - 12,
                      position.y - 16, 24, 32);
            if (shot->variant == 0)
              EffectStep_AdvanceWithGravity3D(shot, 63, -0x8000);
            // On the ground: the shell stops and bursts.
            if (shot->y < 0) {
              s32 count;
              s8 *records = gLobbedShells_Records;
              s32 row;

              shot->y = 0;
              shot->variant = 1;
              /* FAKEMATCH: needed to match: native loads the table address
               * before the row (r0, r1) 2026-10-11: gLobbedShells_Records[mode
               * * 2] in the test differs 135, as a conditional expression too.
               */
              row = mode * 2;
              if (records[row] == 0)
                count = 4;
              else
                count = 16;
              for (i = 0; i != count; i++) {
                struct EffectStep *burst;
                s32 group = k * 4;

                /* FAKEMATCH: needed to match: restores the CSE block boundary
                 * between group's set and group * 8. Without the block: differ
                 * 171; as SHELL_BURSTS + group * 8 + i without it: differ 177.
                 */
                do {
                } while (0);
                burst = &SHELL_BURSTS[group * 8 + i];

                EFFECT_SET_POSITION(burst, shot->x, shot->y, shot->z);
                if (LOBBED_SHELLS_SHATTER(mode) == 0) {
                  EFFECT_SET_VELOCITY(burst, ((Random16() & 63) - 32) << 11, 0,
                                      ((Random16() & 63) - 32) << 11);
                } else {
                  EFFECT_SET_VELOCITY(burst, ((Random16() & 63) - 32) << 13,
                                      ((Random16() & 31) + 32) << 12,
                                      ((Random16() & 63) - 32) << 13);
                }
                burst->variant = 0;
              }
            }
          }
        }
        k++;
      } while (k != work->effect->count);
    }
    // The bursts, for 45 frames: smoke through six images, or shards that
    // tumble through nine images, half of them from a second set.
    for (i = 0, burst = SHELL_BURSTS; i != LOBBED_SHELLS_BURST_SLOTS;
         burst++, i++) {
      if ((u32)burst->variant <= 44 && burst->y >= 0) {
        EffectPosition_ApplyBaseAndYOffset(burst, &position);
        position.x >>= 1;
        if (LOBBED_SHELLS_SHATTER(mode) == 0) {
          draw[1](canvas,
                  work->sheet + burst->variant / 8 * 1152 +
                      LOBBED_SHELLS_SMOKE_CELLS,
                  position.x - 12, position.y - 24, 24, 48);
        } else {
          s32 picture = burst->variant / 5;
          s32 flip;

          if (i & 1)
            picture += 9;
          flip = effect->side;
          draw[burst->velocityX > 0 ? flip ^ 1 : flip](
              canvas,
              work->sheet + gLobbedShells_ShardOffsets[picture] + 0x1e00,
              position.x - gLobbedShells_ShardWidths[picture] / 2,
              position.y - gLobbedShells_ShardHeights[picture] / 2,
              gLobbedShells_ShardWidths[picture],
              gLobbedShells_ShardHeights[picture]);
        }
        if (LOBBED_SHELLS_SHATTER(mode) == 0)
          EffectStep_AdvanceWithGravity3D(burst, 62, 0x800);
        else
          EffectStep_AdvanceWithGravity3D(burst, 62, -0x8000);
        burst->variant++;
      }
    }
    Camera_ApplyShake(2, 2);
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS();
}

// Shattering panels, rotating shapes and whirlwind formations.
//
// The triangle, ring and star effects all draw an outline the same way: a
// handful of corner points are projected onto the canvas and stored in the
// velocity fields of EffectStep records, then each edge between neighbouring
// corners is sampled at a fixed number of evenly spaced points and a mote or
// flare is stamped at each sample.
//
// - The triangle effects (Bind, Luff and BattleFx_RunTriangleStrike) spin a
//   shrinking triangle onto the first target for 64 frames and then stand a
//   rune there.
// - Astral Blast flies three ten-pointed rings at the target, knocking it
//   back; Shining Star drops eight ten-pointed stars that bounce near the
//   target.
// - Procne's whirlwind of comets is a 400-frame scene in several phases: a
//   whirlwind lifts and spins the targets while dust spirals up; at frame 160
//   the sky turns violet and a block of nine objects appears and sways; motes
//   gather into the block; four comets shoot out of it with rings of sparks;
//   finally the targets fall and are put back.
//
// The blue-ramp helper also converts a full-screen tiled picture in place.

// The full-screen picture the blue ramp works on.
#define BLUE_RAMP_PIXELS Ram_MapCellBuffer

#define CONDEMN_FRAMES 132       // frames in the effect
#define CONDEMN_BREAK_FRAME 88   // the panel breaks in a white flash
#define CONDEMN_BOUNCE_FRAME 120 // the falling target bounces

// gCondemn_PanelX and _PanelY have seven stages for each column and row.
#define CONDEMN_PANEL_X(column, stage) gCondemn_PanelX[(column) * 7 + (stage)]
#define CONDEMN_PANEL_Y(row, stage) gCondemn_PanelY[(row) * 7 + (stage)]

// Abilities: Condemn (kind 40).
//
// Battle effect 40: a tall panel (57x98) swings across the screen three
// times, each pass fading out through a ripple, the last over the first
// target; on frame 88 it shatters in a white flash into a burst of larger
// pieces and the target is knocked back, bouncing on frame 120. The effect
// layer ripples throughout. 132 frames.
void BattleFx_RunCondemn(struct BattleEffectArgument *object) {
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  u8 *sheet;
  void *cells;
  BlitterPair draw;
  struct MotionObject *target;
  /* FAKEMATCH: needed to match: the second position is never used: it gives the
   * frame its native 76 bytes 2026-10-11: a single position shrinks only the
   * frame (differ 4). */
  struct EffectPosition pos[2];
  s32 swing[2];
  s32 drawPanel;
  s32 panelColumn;
  s32 panelRow;
  s32 zero;
  s32 frame;

  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  /* FAKEMATCH: needed to match: the zero is held in sl through the setup calls.
   * Storing the literal 0 without the local: differ 66. */
  zero = 0;
  work->effect = object;
  BattleFx_BeginTiledCanvas(0);
  REG_BG2PA = Q_8_8(1);
  REG_BLDALPHA = BLEND_ALPHA(16, 16); // an even mix

  // The panel in work, the pieces of the burst in gMapCellBuffer.
  sheet = Resource_GetTableEntry((s32)&ResourceId_CounterRevealSheet);
  Iwram_CopyWords((void *)BG_PLTT, sheet, 128);
  sheet = sheet + 128;
  Resource_DecodeType01(sheet, work);
  cells = Ram_MapCellBuffer;
  sheet = Resource_GetTableEntry((s32)&ResourceId_RedCrescentSheetA);
  sheet = sheet + 128;
  Resource_DecodeType01(sheet, cells);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_TRANSPARENT);
  draw[0] = (DrawRectangle)EFFECT_SLOT(slots, HEAP_SLOT_BLITTER);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                      COMPOSITE_TRANSPARENT);
  draw[1] = (DrawRectangle)EFFECT_SLOT(slots, HEAP_SLOT_BLITTER_ALTERNATE);
  Scheduler_AddOrUpdateCallback(BattleFx_ArmBg2AffineHBlankDma, 0x480);
  work->transferMode = CANVAS_COPY_AND_CLEAR; // redraw from blank each frame
  work->transferValue = zero;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);
  // curve is where the panel swings about, in 16.16 pixels: the first pass
  // high up on the near side.
  drawPanel = 1;
  if (work->effect->side == 1) {
    swing[0] = -0x500000;
  } else {
    swing[0] = 0x700000;
  }
  swing[1] = -0x200000;
  for (frame = 0; frame != CONDEMN_FRAMES; frame++) {
    s32 spin;
    s32 screenX;
    s32 screenY;
    s32 ripple;
    s32 lineOffset;
    s32 angle;
    s32 i;
    s32 *scanline;

    spin = frame << 9;
    screenX = (swing[0] >> 16) + (((Trig_Sin(spin) * 16)) >> 16) + 48;
    screenY = (swing[1] >> 16) + (((Trig_Cos(spin) * 4)) >> 16) + 16;
    if (frame == CONDEMN_BREAK_FRAME) {
      AudioCommand_PlayFar(SOUND_HEAVY_IMPACT);
    }
    // Frame 32: the second pass, lower and further across; the panel skips
    // a frame while it moves.
    if (frame == 32) {
      if (work->effect->side == 1) {
        swing[0] = -0x200000;
      } else {
        swing[0] = 0x480000;
      }
      swing[1] = 0x180000;
      drawPanel = 0;
    }
    if (frame == 33) {
      REG_BLDALPHA = BLEND_ALPHA(16, 16);
      drawPanel = 1;
    }
    // Frame 64: the last pass, on the first target.
    if (frame == 64) {
      EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], pos);
      if (work->effect->side == 1) {
        swing[0] = (pos[0].x - 128) << 16;
      } else {
        swing[0] = (pos[0].x - 64) << 16;
      }
      swing[1] = 0;
      drawPanel = 0;
    }
    if (frame == 65) {
      REG_BLDALPHA = BLEND_ALPHA(16, 16);
      drawPanel = 1;
    }
    // The last 16 frames of each of the first two passes ripple harder and
    // harder as the panel fades out.
    scanline = work->bg2X;
    ripple = 0;
    if (frame <= 31) {
      if (frame > 15) {
        ripple = (frame * 2) - 32;
        REG_BLDALPHA = (31 - frame) | 0x1000;
      }
    } else if (frame <= 63) {
      if (frame > 47) {
        ripple = (frame * 2) - 96;
        REG_BLDALPHA = (63 - frame) | 0x1000;
      }
    }
    if (ripple < 0) {
      ripple = 0;
    }

    i = 0;
    lineOffset = (6 - screenX) << 8;
    angle = frame << 11;
    for (; i != 160; i++) {
      *scanline++ = lineOffset - ((Trig_Sin(angle) * ripple) >> 10);
      angle += 0x800;
    }
    if (drawPanel != 0) {
      if (work->effect->side == 0) {
        panelColumn = 0;
        panelRow = 0;
      } else {
        panelColumn = 1;
        /* FAKEMATCH: needed to match: i is 160 here, so this is 0: a literal 0
         * drops a stack slot and shrinks the frame (differ 452). */
        panelRow = i >> 31;
      }
      // The panel until frame 91; from frame 88 the burst: a white flash
      // over the whole canvas, then larger pieces two frames each.
      if (frame <= 87) {
        draw[work->effect->side](canvas, work, CONDEMN_PANEL_X(panelColumn, 0),
                                 CONDEMN_PANEL_Y(panelRow, 0) + screenY, 57,
                                 98);
      } else {
        if (frame <= 91) {
          draw[work->effect->side](
              canvas, work, CONDEMN_PANEL_X(panelColumn, 0),
              CONDEMN_PANEL_Y(panelRow, 0) + screenY, 57, 98);
        }
        draw[work->effect->side](
            canvas, work->sheet + 0x15d2, CONDEMN_PANEL_X(panelColumn, 1),
            CONDEMN_PANEL_Y(panelRow, 1) + screenY, 99, 69);
        if ((frame - 88) <= 1U) {
          Iwram_FillWords(canvas, 0x4000, 0x3F3F3F3F);
        }
        if ((frame - 90) <= 1U) {
          draw[work->effect->side](
              canvas, work->sheet + 0x3081, CONDEMN_PANEL_X(panelColumn, 2),
              CONDEMN_PANEL_Y(panelRow, 2) + screenY, 128, 91);
        }
        if ((frame - 92) <= 1U) {
          draw[work->effect->side](
              canvas, (void *)gMapCellBuffer, CONDEMN_PANEL_X(panelColumn, 3),
              CONDEMN_PANEL_Y(panelRow, 3) + screenY, 128, 91);
        }
        if ((frame - 94) <= 1U) {
          draw[work->effect->side](canvas, (u8 *)gMapCellBuffer + 0x2d80,
                                   CONDEMN_PANEL_X(panelColumn, 4),
                                   CONDEMN_PANEL_Y(panelRow, 4) + screenY, 128,
                                   59);
        }
        if ((frame - 96) <= 1U) {
          draw[work->effect->side](canvas, (u8 *)gMapCellBuffer + 0x4b00,
                                   CONDEMN_PANEL_X(panelColumn, 5),
                                   CONDEMN_PANEL_Y(panelRow, 5) + screenY, 122,
                                   29);
        }
        if ((frame - 98) <= 1U) {
          draw[work->effect->side](canvas, (u8 *)gMapCellBuffer + 0x58d2,
                                   CONDEMN_PANEL_X(panelColumn, 6),
                                   CONDEMN_PANEL_Y(panelRow, 6) + screenY, 76,
                                   25);
        }
      }
    }
    // Frame 88: knock the target back, away from the middle of the field.
    if (frame == CONDEMN_BREAK_FRAME) {
      target = GetBattleObjectSlotFar(work->effect->actors[0])->object;
      target->velocityY = 0x10000;
      target->acceleration = 0x20000;
      target->speedLimit = 0x20000;
      target->verticalMotionStrength = 0;
      target->autoFaceMotion = 0;
      target->snapToTarget = 0;
      Object_SetMoveTargetFar(target, target->x << 1, 0, target->z);
      ObjectGroup_UpdateMembers(work->effect->actors[0], -1, POSE_DAMAGE, -1,
                                0);
    }
    if (frame == CONDEMN_BOUNCE_FRAME) {
      GetBattleObjectSlotFar(work->effect->actors[0])
          ->object->verticalMotionStrength = 0xAB85;
    }
    EFFECT_PRESENT_FRAME(work);
  }
  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  Scheduler_RemoveCallback(BattleFx_ArmBg2AffineHBlankDma);
  BattleBackground_LoadFar(1, gBattleWork->background, 24);
  EFFECT_RELEASE_BLITTERS();
  BattleFx_EndCanvasLayer();
}

// Turn the screen into shades of blue, top down, with a ragged edge. Works
// on a copy of the screen in gMapCellBuffer. Each column starts up to 63
// lines late, and the edge speeds up a little every frame; after at most 27
// frames the picture moves onto a second blue ramp at colours 96 to 127.
// Takes 256 random numbers, one a column. Nothing calls it.
UNUSED void Graphics_ConvertBackgroundToBlueRamp(void) {
  s32 i, x, y, row, end, speed;
  s32 red, green, blue;
  u16 color;
  u8 *delays, *p, *pixel;
  u16 *palette;
  u32 delayMask;
  palette = (u16 *)&BG_PLTT_COLOR(2, 0);
  i = 0;
  do {
    s32 half;
    half = i / 2;
    *palette++ = (i << 10) | (half << 5) | half;
    i++;
  } while (i != 32);
  delays = *(u8 **)&gWorkSlot[HEAP_SLOT_BATTLE_SHEET];
  end = 0;
  speed = 16;
  Iwram_CopyWords(BLUE_RAMP_PIXELS, (void *)BG_CHAR_BLOCK(2), 0x7800);
  delayMask = 63;
  p = delays;
  do {
    *p++ = Random16() & delayMask;
  } while (p != delays + 256);
  row = 0;
  i = 0;
  do {
    end += speed / 4;
    speed++;
    while (row != end) {
      x = 0;
      do {
        y = row - delays[x];
        if (y >= 0) {
          if (y <= 119) {
            pixel = BLUE_RAMP_PIXELS + ((x & 7) + ((x / 8) << 6) +
                                        ((y & 7) << 3) + ((y / 8) << 11));
            color = ((s16 *)BG_PLTT)[*pixel];
            red = color & 31;
            green = (color >> 5) & 31;
            blue = (color >> 10) & 31;
            if (red < green)
              red = green;
            if (red < blue)
              red = blue;
            *pixel = 63 - red;
          }
        }
        x++;
      } while (x != 256);
      row++;
    }
    Iwram_CopyWords((void *)BG_CHAR_BLOCK(2), BLUE_RAMP_PIXELS, 0x7800);
    WaitFrames(1);
    if (end > 248)
      break;
    i++;
  } while (i != 27);
  palette = (u16 *)&BG_PLTT_COLOR(6, 0);
  i = 0;
  do {
    s32 half;
    half = i / 2;
    *palette++ = (i << 10) | (half << 5) | half;
    i++;
  } while (i != 32);
  i = 0;
  do {
    BLUE_RAMP_PIXELS[i] += 64;
    i++;
  } while (i != 0x7800);
  Iwram_CopyWords((void *)BG_CHAR_BLOCK(2), BLUE_RAMP_PIXELS, 0x7800);
  WaitFrames(1);
}

// Abilities: Bind (kind 38); Luff (kind 218).
//
// Battle effects 38 and 218: a triangle of light spins and shrinks onto the
// first target for 64 frames, then a rune stands on the target; the battle
// shows the result on frame 64. When effect->prelude is 1 a Jupiter Djinni
// appears first, sways and rises away. The canvas darkens a little faster
// every 16 frames.
void BattleFx_RunBind(struct BattleEffectArgument *effect) {
  struct EffectPosition anchor;
  // World input and projected vertices reuse one coordinate workspace.
  union {
    struct Vector3 world;
    struct EffectPosition projected;
  } position;
  struct EffectPosition screen;
  struct Vector3 scale;
  s32 originX;
  s32 originY;
  BlitterPair draw;
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 member;
  struct BattleCamera *camera;
  s32 shift;
  u8 *graphics;
  u8 *palette;
  struct MotionObject *object;
  s32 tick;
  struct EffectStep *point;
  s32 size;
  s32 j;
  s32 k;

  slots = &((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BATTLE_EFFECT];
  cursor = slots;
  EFFECT_GET_WORK_AND_CANVAS(cursor, work, canvas);
  graphics = EFFECT_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  camera = *(gWorkSlot + HEAP_SLOT_CAMERA);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(1);
  if (work->effect->prelude == 1)
    BattleFx_PrepareCanvasEffect(effect, 3, work->effect->side, 0, &originX,
                                 &originY);
  REG_BG2PA = Q_8_8(1);

  // The rune, the motes at 0x1000 and the Djinni at 0x2000.
  palette = Resource_GetTableEntry((s32)&ResourceId_RuneSheet);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  palette += 128;
  Resource_DecodeType01(palette, work);
  Resource_DecodeType01(
      Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesA), graphics);
  Resource_DecodeType01(
      Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesD),
      work->sheet + 0x1000);
  palette = Resource_GetTableEntry((s32)&ResourceId_JupiterDjinnSheet);
  palette += 128;
  Resource_DecodeType01(palette, work->sheet + 0x2000);
  work->transferMode = CANVAS_COPY_AND_DARKEN;
  work->transferValue = 0x04040404; // 4 off every pixel each frame
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);
  EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &anchor);
  shift = 64 - anchor.x;
  REG_BG2X = shift << 8;
  AudioCommand_PlayFar(142);

  for (frame = 0; frame != work->effect->count * 20 + 72; frame++) {
    if (frame == 64)
      BattleEventRuntime_BeginPhaseFar(0);
    Graphics_UpdatePhasePalette(frame, 0xaaab, 0x5555, 0);

    // The Djinni sways 20 pixels either way and from frame 32 rises 2
    // pixels a frame.
    if (work->effect->prelude == 1) {
      s32 x;
      s32 y;

      x = ((Trig_Sin(frame << 11) * 20) >> 16) + originX + shift - 20;
      y = (((Trig_Cos(frame << 11) * 4)) >> 16) + originY - 24;
      BattleFx_FetchRectangleBlitters(work->effect->side, draw);
      if (frame > 32)
        y = y - frame * 2 + 64;
      draw[0](canvas, work->sheet + 0x2000, x, y, 40, 40);
      if (frame <= 3)
        draw[1](canvas, work->sheet + 0x2000, x, y, 40, 40);
      EFFECT_RELEASE_BLITTERS();
    }
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS, COMPOSITE_MAX);
    draw[0] =
        (DrawRectangle)((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BLITTER];
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_MAX);
    draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
    if (frame > 16 && (frame & 15) == 0)
      work->transferValue += 0x01010101;

    // Only the first target is drawn.
    for (member = 0; member != 1; member++) {
      tick = frame - member * 8;
      object = GetBattleObjectSlotFar(work->effect->actors[member])->object;
      if (tick >= 0 && tick < 96) {
        EFFECT_VIEW_FROM_CAMERA(camera);
        position.world.x = object->x;
        position.world.y = object->y;
        position.world.z = object->z;
        EffectPosition_ApplyBaseAndYOffset(&position.world, &screen);
        screen.x = anchor.x + shift;
        screen.y -= 24;
        // The triangle: its three corners, turned a third of a circle
        // apart and shrinking as they spin, are projected into the
        // particles' velocity fields; each side is 24 motes.
        if (tick <= 67) {
          for (j = 0; j != 3; j++) {
            point = &work->particles[member * 32 + j];
            Render_ResetTransformState();
            if (tick <= 63) {
              scale.x = 0x2a000 - (tick * 3 << 9);
              scale.y = 0x2a000 - (tick * 3 << 9);
              scale.z = 0x2a000 - (tick * 3 << 9);
              SceneTransform_ApplyScale(Vector3_Coordinates(&scale));
              SceneTransform_ApplyRoll((64 - tick) << 9);
              SceneTransform_ApplyYaw((64 - tick) << 9);
            }
            SceneTransform_ApplyRoll(j * 0x5555);
            EffectPosition_ApplyBaseAndYOffset(gBind_Vertex,
                                               &position.projected);
            point->velocityX = position.projected.x + screen.x;
            point->velocityY = position.projected.y + screen.y + 16;
          }
          for (j = 0; j != 3; j++) {
            struct EffectStep *from = &work->particles[j + member * 32];
            struct EffectStep *to = &work->particles[(j + 1) % 3 + member * 32];

            size = 5 - tick / 16;
            for (k = 0; k != 24; k++) {
              s32 x =
                  from->velocityX + k * (to->velocityX - from->velocityX) / 24;
              s32 y =
                  from->velocityY + k * (to->velocityY - from->velocityY) / 24;

              draw[0](canvas,
                      work->sheet + gBattleFx_FlareCells[size - 1] + 0x1000,
                      x - size, y - size, size * 2, size * 2);
            }
          }
        }
        // Then the rune, in two mirrored halves.
        if (tick > 63) {
          draw[0](canvas, work, screen.x - 24, screen.y - 24, 24, 48);
          draw[1](canvas, work, screen.x, screen.y - 24, 24, 48);
        }
      }
    }
    EFFECT_RELEASE_BLITTERS();
    EFFECT_PRESENT_FRAME(work);
  }

  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  BattleFx_EndCanvasLayer();
}

// Battle effect 67: the spinning triangle and rune of
// BattleFx_RunBind without the Djinni; on frame 46 the user
// steps up to the first target.
void BattleFx_RunTriangleStrike(struct BattleEffectArgument *effect) {
  struct EffectPosition anchor;
  // World input and projected vertices reuse one coordinate workspace.
  union {
    struct Vector3 world;
    struct EffectPosition projected;
  } position;
  struct EffectPosition screen;
  struct Vector3 scale;
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  BlitterPair draw;
  s32 member;
  struct BattleCamera *camera;
  s32 shift;
  u8 *graphics;
  u8 *palette;
  struct MotionObject *object;
  s32 tick;
  struct EffectStep *point;
  s32 size;
  s32 j;
  s32 k;

  slots = &((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BATTLE_EFFECT];
  cursor = slots;
  EFFECT_GET_WORK_AND_CANVAS(cursor, work, canvas);
  graphics = EFFECT_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  camera = *(gWorkSlot + HEAP_SLOT_CAMERA);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);
  REG_BG2PA = Q_8_8(1);

  // The rune, and the motes at 0x1000.
  palette = Resource_GetTableEntry((s32)&ResourceId_RuneSheet);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  palette += 128;
  Resource_DecodeType01(palette, work);
  Resource_DecodeType01(
      Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesA), graphics);
  palette = Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesD);
  Resource_DecodeType01(palette, work->sheet + 0x1000);
  work->transferMode = CANVAS_COPY_AND_DARKEN;
  work->transferValue = 0x04040404; // 4 off every pixel each frame
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);
  EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &anchor);
  shift = 64 - anchor.x;
  REG_BG2X = shift << 8;
  AudioCommand_PlayFar(142);

  for (frame = 0; frame != work->effect->count * 20 + 72; frame++) {
    if (frame == 64)
      BattleEventRuntime_BeginPhaseFar(0);
    if (frame == 46)
      BattleMotion_ApproachTargetFar(work->effect->actor,
                                     work->effect->actors[0], 16, 0);
    Graphics_UpdatePhasePalette(frame, 0xaaab, 0x5555, 0);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS, COMPOSITE_MAX);
    draw[0] =
        (DrawRectangle)((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BLITTER];
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_MAX);
    draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
    if (frame > 16 && (frame & 15) == 0)
      work->transferValue += 0x01010101;

    // Only the first target is drawn.
    for (member = 0; member != 1; member++) {
      tick = frame - member * 8;
      object = GetBattleObjectSlotFar(work->effect->actors[member])->object;
      if (tick >= 0 && tick < 96) {
        EFFECT_VIEW_FROM_CAMERA(camera);
        position.world.x = object->x;
        position.world.y = object->y;
        position.world.z = object->z;
        EffectPosition_ApplyBaseAndYOffset(&position.world, &screen);
        screen.x = anchor.x + shift;
        screen.y -= 24;
        // The triangle: its three corners, turned a third of a circle
        // apart and shrinking as they spin, are projected into the
        // particles' velocity fields; each side is 24 motes.
        if (tick <= 67) {
          for (j = 0; j != 3; j++) {
            point = &work->particles[member * 32 + j];
            Render_ResetTransformState();
            if (tick <= 63) {
              scale.x = 0x2a000 - (tick * 3 << 9);
              scale.y = 0x2a000 - (tick * 3 << 9);
              scale.z = 0x2a000 - (tick * 3 << 9);
              SceneTransform_ApplyScale(Vector3_Coordinates(&scale));
              SceneTransform_ApplyRoll((64 - tick) << 9);
              SceneTransform_ApplyYaw((64 - tick) << 9);
            }
            SceneTransform_ApplyRoll(j * 0x5555);
            EffectPosition_ApplyBaseAndYOffset(gTriangleStrike_Vertex,
                                               &position.projected);
            point->velocityX = position.projected.x + screen.x;
            point->velocityY = position.projected.y + screen.y + 16;
          }
          for (j = 0; j != 3; j++) {
            struct EffectStep *from = &work->particles[j + member * 32];
            struct EffectStep *to = &work->particles[(j + 1) % 3 + member * 32];

            size = 5 - tick / 16;
            for (k = 0; k != 24; k++) {
              s32 x =
                  from->velocityX + k * (to->velocityX - from->velocityX) / 24;
              s32 y =
                  from->velocityY + k * (to->velocityY - from->velocityY) / 24;

              draw[0](canvas,
                      work->sheet + gBattleFx_FlareCells[size - 1] + 0x1000,
                      x - size, y - size, size * 2, size * 2);
            }
          }
        }
        // Then the rune, in two mirrored halves.
        if (tick > 63) {
          draw[0](canvas, work, screen.x - 24, screen.y - 24, 24, 48);
          draw[1](canvas, work, screen.x, screen.y - 24, 24, 48);
        }
      }
    }
    EFFECT_RELEASE_BLITTERS();
    EFFECT_PRESENT_FRAME(work);
  }

  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  BattleFx_EndCanvasLayer();
}

// Abilities: Astral Blast (kind 68).
//
// Battle effect 68: three star-shaped rings of light fly from 40 pixels
// above the user at the first target, 12 frames apart, growing and
// spinning as they go, while the camera swings round; each knocks the
// target back on arrival, and the third lets the battle show the damage.
void BattleFx_RunAstralBlast(struct BattleEffectArgument *effect) {
  struct EffectPosition screenPosition;
  struct EffectPosition base;
  struct Vector3 scale;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  /* FAKEMATCH: frame local: only draw[0] is used; the unused second blitter
     word fills four bytes of the frame. Measured (2026-10-14): declared as one
     DrawRectangle, the frame is 112 bytes for 116 (differ 48, frame offsets
     only). */
  BlitterPair draw;
  s32 i;
  u8 *graphics;
  struct BattleCamera *camera;
  struct MotionObject *caster;
  struct MotionObject *source;
  struct MotionObject *target;
  struct EffectStep *point;
  struct EffectStep *step;
  s32 start;
  s32 peak;
  s32 size;
  s32 j;
  s32 k;
  struct EffectStep *trails;

  work = *(gWorkSlot + HEAP_SLOT_BATTLE_EFFECT);
  canvas = *(gWorkSlot + HEAP_SLOT_BATTLE_CANVAS);
  graphics = *(gWorkSlot + HEAP_SLOT_BATTLE_SHEET);
  camera = *(gWorkSlot + HEAP_SLOT_CAMERA);
  caster = GetBattleObjectSlotFar(effect->actor)->object;
  work->effect = effect;
  BattleFx_BeginCanvasLayer(1);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_RuneSheet), 128);
  Resource_DecodeType01(
      Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesA), graphics);
  ObjectDispatch_ApplyArgumentToChildrenFar(caster, 2);
  ObjectDispatch_ApplyValueToChildrenFar(caster, 48);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS, COMPOSITE_MAX);
  work->transferMode = CANVAS_COPY_AND_FADE; // trails keep 3/4 a frame
  work->transferValue = 75;
  draw[0] = (DrawRectangle) * (gWorkSlot + HEAP_SLOT_BLITTER);
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);

  source = GetBattleObjectSlotFar(work->effect->actor)->object;
  target = GetBattleObjectSlotFar(work->effect->actors[0])->object;
  // Each ring reaches the target in 12 frames; the second and third aim
  // twice as far across, past it.
  for (i = 0, point = work->particles; i != 3; i++) {
    EFFECT_SET_POSITION(point, source->x, source->y + 0x280000, source->z);
    if (i == 0)
      point->velocityX = (target->x - point->x) / 12;
    else
      point->velocityX = (target->x * 2 - point->x) / 12;
    point->velocityY = (target->y - point->y + 0x280000) / 12;
    point->velocityZ = (target->z - point->z) / 12;
    point->variant = 0;
    point++;
  }

  trails = gMapCellBuffer;
  for (frame = 0; frame != 60; frame++) {
    // Swing the camera round, slowing over frames 40 to 47.
    if (frame <= 47) {
      struct BattleCamera *view = *(gWorkSlot + HEAP_SLOT_CAMERA);
      s32 speed;

      if (frame <= 39)
        speed = 128;
      else
        speed = 0x300 - frame * 16;
      if (work->effect->side == 0)
        view->yaw -= speed;
      else
        view->yaw += speed;
    }
    for (i = 0; i != 3; i++) {
      start = i * 12;
      if (frame >= start) {
        step = &work->particles[i];
        size = (frame - start) / 4 + 2;
        if (size > 10)
          size = 10;
        EFFECT_VIEW_FROM_CAMERA(camera);
        SceneTransform_ApplyPosition(EffectStep_Coordinates(step));
        // The ring's ten points, alternately near and far from its centre,
        // projected onto the canvas; peak is the farthest depth.
        peak = 0;
        for (j = 0; j != 10; j++) {
          s32 depth;

          Transform_Push();
          SceneTransform_ApplyRoll((frame - start) << 10);
          SceneTransform_ApplyYaw(0x4000);
          scale.x = ((frame - start) << 12) + 0x1000;
          if (scale.x > 0x10000)
            scale.x = 0x10000;
          scale.y = scale.x;
          scale.z = scale.x;
          SceneTransform_ApplyScale(Vector3_Coordinates(&scale));
          SceneTransform_ApplyRoll(j * 0x199a);
          depth = EffectPosition_ApplyBaseAndYOffset(
              &gAstralBlast_Points[j & 1], &screenPosition);
          if (peak < depth)
            peak = depth;
          screenPosition.x >>= 1;
          // These first stores read base, which is never set; the next
          // two overwrite them.
          trails[i * 10 + j].velocityX = screenPosition.x + base.x;
          trails[i * 10 + j].velocityY = screenPosition.y + base.y;
          trails[i * 10 + j].velocityX = screenPosition.x;
          trails[i * 10 + j].velocityY = screenPosition.y;
          Transform_Pop();
        }
        // Join the points with 16 motes each, unless the ring is too far
        // away.
        if (peak <= 399999) {
          for (j = 0; j != 10; j++) {
            struct EffectStep *from =
                &((struct EffectStep *)gMapCellBuffer)[j + i * 10];
            struct EffectStep *to =
                &((struct EffectStep *)gMapCellBuffer)[(j + 1) % 10 + i * 10];

            for (k = 0; k != 16; k++) {
              s32 x =
                  from->velocityX + (to->velocityX - from->velocityX) * k / 16;
              s32 y =
                  from->velocityY + (to->velocityY - from->velocityY) * k / 16;

              EFFECT_DRAW_MOTE(draw[0], canvas, graphics, x, y, size);
            }
          }
        }
        step->x += step->velocityX;
        step->y += step->velocityY;
        step->z += step->velocityZ;
        // The hit: knock the target back, away from the middle.
        if (frame == start + i + 10) {
          target->acceleration = 0x20000;
          target->speedLimit = 0x80000;
          target->velocityY = 0x50000;
          target->verticalMotionStrength = 0xab85;
          target->autoFaceMotion = 0;
          ObjectDispatch_InitFromTable6Far(target);
          if (target->x < 0)
            Object_SetMoveTargetFar(target, target->x - 0x280000, 0, target->z);
          else
            Object_SetMoveTargetFar(target, target->x + 0x280000, 0, target->z);
          if (i == 2) {
            BattleEventRuntime_BeginPhaseFar(SOUND_HEAVY_IMPACT);
          } else {
            AudioCommand_PlayFar(SOUND_HEAVY_IMPACT);
            ObjectGroup_UpdateMembers(work->effect->actors[0],
                                      SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 8);
          }
          work->shakeFrames = 4;
        }
      }
    }
    Camera_ApplyShake(8, 8);
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS();
}

// Abilities: Shining Star (kind 62).
//
// Battle effect 62: eight spinning star outlines fall from 120 pixels above
// the user towards the first target, a few frames apart, and bounce off the
// ground near it, each bounce shaking the screen and flashing the targets;
// the battle shows the damage on frame 96. 128 frames.
void BattleFx_RunShiningStar(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  /* FAKEMATCH: frame local: only draw[0] is used; the unused second blitter
     word fills four bytes of the frame. Measured (2026-10-14): declared as one
     DrawRectangle, the frame is 60 bytes for 64 (differ 40, frame offsets
     only). */
  BlitterPair draw;
  s32 i;
  u8 *graphics;
  struct BattleCamera *camera;
  struct MotionObject *source;
  struct MotionObject *target;
  struct EffectStep *point;
  struct EffectStep *step;
  struct EffectPosition screenPosition;
  s32 size;
  s32 j;
  s32 k;
  struct EffectStep *trails;

  work = *(gWorkSlot + HEAP_SLOT_BATTLE_EFFECT);
  canvas = *(gWorkSlot + HEAP_SLOT_BATTLE_CANVAS);
  graphics = *(gWorkSlot + HEAP_SLOT_BATTLE_SHEET);
  camera = *(gWorkSlot + HEAP_SLOT_CAMERA);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(1);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_RuneSheet), 128);
  Resource_DecodeType01(
      Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesA), graphics);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS, COMPOSITE_MAX);
  work->transferMode = CANVAS_COPY_AND_FADE; // trails keep half a frame
  work->transferValue = 50;
  draw[0] = (DrawRectangle) * (gWorkSlot + HEAP_SLOT_BLITTER);
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);

  source = GetBattleObjectSlotFar(work->effect->actor)->object;
  target = GetBattleObjectSlotFar(work->effect->actors[0])->object;
  // Each star aims at a random spot up to 64 pixels either side of the
  // target, 20 pixels up, over 12 frames, and starts 8i to 8i + 15 frames
  // in (its variant).
  for (i = 0, point = work->particles; i != 8; i++) {
    s32 delay = i * 8;

    EFFECT_SET_POSITION(point, source->x / 2, source->y + 0x780000, source->z);
    // The signed -64..63 pixel offset scales without a negative left shift.
    point->velocityX =
        (target->x + ((s32)((Random16() & 0x7f) - 64) * 65536) - point->x) / 12;
    point->velocityY = (target->y - point->y + 0x140000) / 12;
    point->velocityZ = (target->z - point->z) / 12;
    point->variant = (Random16() & 15) + delay;
    point++;
  }

  size = 2;
  trails = gMapCellBuffer;
  for (frame = 0; frame != 128; frame++) {
    Graphics_UpdatePhasePalette(frame, 0xaaab, 0x5555, 0);
    if (frame == 96)
      BattleEventRuntime_BeginPhaseFar(SOUND_HEAVY_IMPACT);
    for (i = 0; i != 8; i++) {
      step = &work->particles[i];
      if (frame >= step->variant) {
        EFFECT_VIEW_FROM_CAMERA(camera);
        EffectPosition_ApplyBaseAndYOffset(step, &screenPosition);
        screenPosition.x >>= 1;
        // On the canvas, draw the star: ten points, alternately near and
        // far, turning as it falls, joined by 12 motes each.
        if (screenPosition.x >= -8 && screenPosition.x <= 127) {
          if (screenPosition.y <= 127) {
            if (screenPosition.y >= -8) {
              for (j = 0; j != 10; j++) {
                trails[i * 10 + j].velocityX =
                    screenPosition.x +
                    ((Trig_Sin(j * 0x199a - ((frame - step->variant) << 11)) *
                      gShiningStar_Radii[j & 1]) /
                         2 >>
                     16);
                trails[i * 10 + j].velocityY =
                    screenPosition.y -
                    (Trig_Cos(j * 0x199a - ((frame - step->variant) << 11)) *
                         gShiningStar_Radii[j & 1] >>
                     16);
              }
              for (j = 0; j != 10; j++) {
                struct EffectStep *from = &trails[j + i * 10];
                struct EffectStep *to = &trails[i * 10 + (j + 1) % 10];

                for (k = 0; k != 12; k++) {
                  s32 x = from->velocityX +
                          (to->velocityX - from->velocityX) * k / 12;
                  s32 y = from->velocityY +
                          (to->velocityY - from->velocityY) * k / 12;

                  EFFECT_DRAW_MOTE(draw[0], canvas, graphics, x, y, size);
                }
              }
            }
          }
        }
        // Bounce below 30 pixels up, losing half the speed across.
        if (step->y <= 0x1dffff) {
          step->velocityY = -step->velocityY;
          step->velocityX /= 2;
          step->velocityZ /= 2;
          work->shakeFrames = 4;
          AudioCommand_PlayFar(SOUND_HEAVY_IMPACT);
          EFFECT_FOR_EACH_TARGET(j, work->effect)
            ObjectGroup_UpdateMembers(work->effect->actors[j],
                                      SPRITE_COLOR_WHITE, POSE_DAMAGE, j, 8);
        }
        step->x += step->velocityX;
        step->y += step->velocityY;
        step->z += step->velocityZ;
      }
    }
    Camera_ApplyShake(4, 4);
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS();
}

extern u8 gWhirlwindComets_GridX[];
extern u8 gWhirlwindComets_GridY[];
extern u16 gWhirlwindComets_Frames[];
extern u8 gWhirlwindComets_CellWidths[];
extern u8 gWhirlwindComets_CellHeights[];
extern u8 gWhirlwindComets_CellBiasY[];
extern u16 gWhirlwindComets_CellOffsets[];

// Abilities: Procne (scene kind 8).
//
// Large battle effect 8 (BattleFx_RunSceneEffect), 400 frames:
//   0 to 159   : a whirlwind rises over the field and lifts the targets,
//                spinning them round it and drawing them in, while dust
//                spirals up;
//   160        : the backdrop becomes the violet sky, and a 3x3 block of
//                nine objects appears and sways;
//   256 to 329 : motes gather into the middle of the block;
//   330 to 372 : four comets shoot out of it, 12 frames apart, each with a
//                ring of sparks, striking the targets on frames 334, 347,
//                359 and 372 (gWhirlwindComets_Frames);
//   395        : the targets fall back to the ground.
// Pressing A or B skips to frame 160, or from there to frame 395. At the
// end the battle shows the damage and the targets go back to their places.
void BattleFx_RunProcne(struct BattleEffectArgument *argument) {
  static struct SpriteScale unitScale = {{{0x10000, 0x10000}}};
  void **slots;
  void *canvas;
  struct BattleCamera *camera;
  struct BattleEffectWork *work;
  BlitterPair draw;
  s32 frame;
  u8 *graphics;
  struct Point2D wave;
  struct Point2D drift;
  struct MotionObject *object;
  struct BattleObj *battleObject;
  struct EffectStep *point;
  struct EffectStep *spark;
  struct EffectStep *mark;
  u8 *resource;
  u8 *image;
  s32 index;
  s32 slot;
  s32 size;
  s32 span;
  s32 dist;
  s32 alive;
  s32 width;
  s32 heading[8];
  s32 radius[8];
  s32 spin[8];
  struct Point2D home[8];
  s32 baseAngle[8];
  struct Vector3 world;
  struct EffectPosition screen;
  struct Vector3 shake;
  struct SpritePlacement place;
  struct SpriteScale scale;

  slots = &gBattleFxWork[1];
  canvas = slots[0];
  camera = *(struct BattleCamera **)((u8 *)slots - 112);
  work = slots[-1];
  graphics = slots[1];
  work->effect = argument;

  BattleFx_BeginCanvasLayer(0);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS, COMPOSITE_MAX);
  draw[0] = (DrawRectangle)slots[6];
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_ADDITIVE);
  draw[1] = (DrawRectangle)slots[7];

  resource = Resource_GetTableEntry((s32)&ResourceId_WhirlwindSheet);
  Iwram_CopyWords((void *)BG_PLTT, resource, 0x80);
  resource += 0x80;
  Resource_DecodeType01(resource, work);
  Resource_DecodeType01(
      Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesA), graphics);

  work->transferMode = CANVAS_COPY_AND_FADE; // trails keep half a frame
  work->transferValue = 50;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);

  // wave is where the block of objects sways, drift a push from each comet.
  wave.x = 0x800000;
  wave.y = 0x280000;
  drift.x = 0;
  drift.y = 0;

  // Remember each target's place, and its heading and distance from the
  // middle of the field for the whirlwind to turn.
  EFFECT_FOR_EACH_TARGET(index, work->effect) {
    battleObject = GetBattleObjectSlotFar(work->effect->actors[index]);
    object = battleObject->object;
    home[index].x = object->x;
    home[index].y = object->z;
    baseAngle[index] = object->angle;
    heading[index] = (u16)ArcTan2(object->x, object->z);
    radius[index] = Iwram_Sqrt((object->x >> 8) * (object->x >> 8) +
                               (object->z >> 8) * (object->z >> 8)) >>
                    7;
    spin[index] = 0;
    object->verticalMotionStrength = 0;
  }

  // The dust: each mote starts 120 out, after a random delay (variant).
  index = 0;
  do {
    point = &work->particles[index];
    point->z = 120;
    point->y = 0;
    Random16();
    point->velocityY = 0;
    point->velocityX = 0;
    point->variant = Random16() & 63;
    index++;
  } while (index != 64);

  width = 48;
  frame = 0;
  do {
    // A or B skips ahead.
    if ((gKeysRepeat & (KEY_A | KEY_B)) != 0) {
      if (frame <= 159)
        frame = 160;
      else if (frame <= 394)
        frame = 395;
    }

    EFFECT_VIEW_FROM_CAMERA(camera);

    if (frame == 16)
      AudioCommand_PlayFar(141);
    if (frame == 256)
      AudioCommand_PlayFar(140);
    if (frame == 334)
      AudioCommand_PlayFar(212);
    if (frame == 347)
      AudioCommand_PlayFar(212);
    if (frame == 359)
      AudioCommand_PlayFar(212);
    if (frame == 372)
      AudioCommand_PlayFar(212);

    // The targets, from 16i frames: spin faster round the middle and close
    // in, rising quickly once near it, until frame 160; they keep turning
    // until frame 490 (past the end), and flash on each comet strike.
    EFFECT_FOR_EACH_TARGET(index, work->effect) {
      if (frame > index * 16) {
        battleObject = GetBattleObjectSlotFar(work->effect->actors[index]);
        object = battleObject->object;
        object->x = (radius[index] * Trig_Sin(heading[index])) >> 1;
        object->z = (radius[index] * Trig_Cos(heading[index])) >> 1;

        if (frame <= 159) {
          if (frame > index * 16 + 16)
            spin[index] += 48;
          if (radius[index] <= 31)
            object->y += 0x60000;
          else
            object->y += 0x8000;
          if (object->y > 0x7c0000)
            object->y = 0x7c0000;
        }

        if (frame <= 490) {
          heading[index] += spin[index];
          if (heading[index] > 0x10000)
            heading[index] -= 0x10000;
          object->angle += spin[index] / 4;
        }

        if (frame == 395)
          object->verticalMotionStrength = 0xab85;

        slot = 0;
        do {
          if (frame == gWhirlwindComets_Frames[slot]) {
            object->velocityY = 0;
            ObjectGroup_UpdateMembers(work->effect->actors[index],
                                      SPRITE_COLOR_WHITE, POSE_DAMAGE, index,
                                      8);
          }
          slot++;
        } while (slot != 4);

        if (radius[index] > 16)
          radius[index] -= 2;
      }
    }

    // The whirlwind: three images in turn, growing up from the ground over
    // frames 16 to 159, a second section above it from frame 48, and both
    // at full height from frame 160 to 399.
    if ((u32)(frame - 16) <= 143) {
      size = frame * 2 - 32;
      if (size > 48)
        size = 48;
      draw[0](canvas, &work->sheet[frame / 4 % 3 * 3072 + (width - size) * 48],
              32, 112 - size, width, size);
    }

    if ((u32)(frame - 48) <= 111) {
      size = frame * 2 - 96;
      if (size > 64)
        size = 64;
      draw[0](canvas, &work->sheet[frame / 4 % 3 * 3072 + (64 - size) * 48], 32,
              64 - size, width, size);
    }

    if ((u32)(frame - 160) <= 239) {
      image = work->sheet + frame / 4 % 3 * 3072;
      draw[0](canvas, image, 8, 0, width, 64);
      draw[0](canvas, image, 8, 64, width, 64);
    }

    // The dust (24 motes): spiral in and up, starting again from the
    // bottom when it passes 48 pixels up.
    if (frame <= 159) {
      index = 0;
      point = work->particles;
      do {
        if (point[index].variant == 0) {
          world.x = point[index].z * Trig_Sin(point[index].velocityY);
          world.y = point[index].y;
          world.z = point[index].z * Trig_Cos(point[index].velocityY);
          EffectPosition_ApplyBaseAndYOffset(&world.x, &screen);
          screen.x >>= 1;
          if (point[index].y <= 0x3fffff) {
            draw[0](canvas, work->sheet + 0x2400, screen.x - 12, screen.y - 12,
                    16, 16);
          }
          if (point[index].z > 24)
            point[index].z -= 4;
          point[index].velocityY += point[index].velocityX * 2;
          if (point[index].velocityY > 0x10000)
            point[index].velocityY -= 0x10000;
          point[index].velocityX += 50;
          if (point[index].velocityX > 4096)
            point[index].velocityX = 4096;
          point[index].velocityY += 2048;
          point[index].y += point[index].velocityY * 2;
          if (point[index].y > 0x300000) {
            point[index].y = 0;
            point[index].z = 100;
            point[index].velocityY = 0;
            point[index].velocityX = 0;
          }
        } else {
          point[index].variant--;
        }
        index++;
      } while (index != 24);
    }

    // Frame 160: the violet sky, the nine objects and the comet images;
    // the targets hang at random heights and the motes that will gather
    // are scattered round the block.
    if (frame == 160) {
      s32 speed;
      s32 angle;

      BattleBackground_LoadFar(1, (s32)&ResourceId_VioletSkyBackdrop, 0);
      BattlePresentation_ConfigurePaletteFadeFar(
          1, (s32)&ResourceId_VioletSkyBackdrop, 8);
      BattleFx_SelectLivingTargets(work->effect);
      BattleFx_SpawnObjects(9, 376, 2);

      resource = Resource_GetTableEntry((s32)&ResourceId_VioletCometSheet);
      Iwram_CopyWords((void *)BG_PLTT, resource, 0x80);
      resource += 0x80;
      Resource_DecodeType01(resource, work->sheet + 0x3600);

      index = 0;
      do {
        PARTICLES[index].variant = 0;
        index++;
      } while (index != 512);

      EFFECT_FOR_EACH_TARGET(index, work->effect) {
        battleObject = GetBattleObjectSlotFar(work->effect->actors[index]);
        object = battleObject->object;
        object->y = ((Random16() & 15) + 8) << 16;
      }

      gProjection.centerX = 72;

      index = 0;
      do {
        speed = Random16() & 127;
        angle = (Random16() & 0x7fff) + 0x9fff;
        work->particles[index].x = Trig_Sin(angle) * speed;
        work->particles[index].y = Trig_Cos(angle) * speed;
        work->particles[index].z = (Random16() % 200 - 100) << 16;
        work->particles[index].variant = 0;
        index++;
      } while (index != 32);

      EFFECT_FOR_EACH_TARGET(index, work->effect) {
        radius[index] = 16;
        spin[index] = 6000;
      }
    }

    // From frame 160: sway the block of objects, faster for its first 64
    // frames, plus what is left of each comet's push.
    if (frame > 159) {
      scale = unitScale;
      size = 16;
      if (frame - 160 <= 64)
        size = 32;

      wave.x -= (Trig_Sin((frame - 160) << 7) * 32) >> 6;
      wave.y += (Trig_Sin((frame - 160) << 9) * size) >> 6;
      wave.x += drift.x;
      wave.y += drift.y;
      drift.x /= 2;
      drift.y /= 2;

      scale.x = 0x10000;
      scale.y = 0x10000;
      place.groundHeight = 0;
      index = 0;
      do {
        place.x = (gWhirlwindComets_GridX[index] << 16) + wave.x + 0x280000;
        place.height = 0xff0000 + index * 0x400000;
        place.y = ((gWhirlwindComets_GridY[index] + index * 64) << 16) + wave.y;
        Object_ApplyProjectedPlacementFar(work->objects[index], &place, &scale,
                                          0);
        index++;
      } while (index != 9);

      if (frame > 255) {
        s32 threshold;

        shake.z = 0x1000000;
        alive = 0;
        shake.x = 0;
        shake.y = 0;
        Render_ResetTransformState();
        SceneTransform_ApplyPosition(Vector3_Coordinates(&shake));

        // Frames 256 to 329: the motes close in on the middle of the
        // block a pixel a frame; those that arrive swell one mote there.
        if (frame <= 329) {
          index = 0;
          do {
            s32 xx;
            s32 yy;
            s32 zz;

            point = &work->particles[index];
            xx = (point->x >> 8) * (point->x >> 8);
            yy = (point->y >> 8) * (point->y >> 8);
            zz = (point->z >> 8) * (point->z >> 8);
            dist = Iwram_Sqrt(xx + yy + zz) >> 9;
            if (dist != 0) {
              EffectPosition_ApplyBaseAndYOffset(&point->x, &screen);
              screen.x = (screen.x >> 17) + (wave.x >> 17) + 32;
              screen.y = (screen.y >> 16) + (wave.y >> 16) - 4;
              screen.depth >>= 16;
              if (screen.depth <= 169)
                screen.depth = 170;
              if (screen.depth > 350)
                screen.depth = 350;
              span = 6 - (screen.depth - 170) / 36;
              EFFECT_DRAW_MOTE(draw[1], canvas, graphics, screen.x, screen.y,
                               span);
              point->x -= point->x / dist;
              point->y -= point->y / dist;
              point->z -= point->z / dist;
            } else {
              alive++;
            }
            index++;
          } while (index != 32);

          if (alive > 0) {
            span = alive / 10 + 1;
            draw[1](canvas, graphics + gBattleFx_MoteCellOffsets[span - 1],
                    (wave.x >> 17) - span / 2 + 32, (wave.y >> 16) - span - 4,
                    span, span * 2);
          }
        }

        // Each strike throws out 96 sparks.
        slot = 0;
        do {
          if (frame == gWhirlwindComets_Frames[slot]) {
            s32 speed;
            s32 angle;

            index = 0;
            do {
              speed = (Random16() & 127) + 16;
              angle = Random16() & 0xffff;
              spark = &PARTICLES[slot * 128 + index];
              spark->velocityX = (Trig_Sin(angle) * speed) >> 6;
              spark->velocityY = -(Trig_Cos(angle) * speed) >> 6;
              spark->velocityY = ((Random16() & 255) - 128) << 10;
              EFFECT_SET_POSITION(spark, 0, 0, 0);
              spark->variant = (Random16() & 15) + 64;
              index++;
            } while (index != 96);
          }

          if (frame >= gWhirlwindComets_Frames[0]) {
            spark = PARTICLES;
            index = 0;
            do {
              if (spark[index].variant > 0) {
                EffectPosition_ApplyBaseAndYOffset(&spark[index].x, &screen);
                screen.x = (screen.x >> 17) + 32;
                screen.y = (screen.y >> 16) + 56;
                screen.depth >>= 16;
                if (screen.depth <= 169)
                  screen.depth = 170;
                if (screen.depth > 350)
                  screen.depth = 350;
                span = 3 - (screen.depth - 170) / 90;
                EFFECT_DRAW_MOTE(draw[0], canvas, graphics, screen.x, screen.y,
                                 span);
                spark[index].x += spark[index].velocityX;
                spark[index].y += spark[index].velocityY;
                spark[index].z += spark[index].velocityY;
                spark[index].variant--;
              }
              index++;
            } while (index != 512);
          }
          slot++;
        } while (slot != 4);

        // The four comets, from frame 330 + 12i: each leaves the middle of
        // the block, flies down to the left growing through three images,
        // and spreads a ring of 28 motes for 8 frames; it pushes the block
        // and shakes the screen.
        index = 0;
        do {
          mark = &work->particles[index + 60];
          threshold = index * 12 + 330;
          if (frame == threshold) {
            mark->x = (wave.x >> 17) + 32;
            mark->y = (wave.y >> 16) - 4;
            mark->velocityX = (wave.x >> 17) + 32;
            mark->velocityY = (wave.y >> 16) - 4;

            slot = 0;
            do {
              work->particles[slot + 32].variant = 4;
              slot++;
            } while (slot != 28);

            drift.x = 0x80000;
            drift.y = -0x20000;
            work->shakeFrames = 8;
          }

          if (frame >= threshold) {
            s32 size;

            size = (frame - threshold) / 2;
            if (size > 2)
              size = 2;
            draw[1](canvas, work->sheet + gWhirlwindComets_CellOffsets[size],
                    mark->x, mark->y - gWhirlwindComets_CellBiasY[size],
                    gWhirlwindComets_CellWidths[size],
                    gWhirlwindComets_CellHeights[size]);
            mark->x -= 8;
            mark->y += 2;

            if (frame < threshold + 8) {
              SceneTransform_ApplyRoll(-2048);
              SceneTransform_ApplyYaw(-4096);

              slot = 0;
              spark = &work->particles[32];
              do {
                struct EffectStep *step;

                step = &spark[slot];
                world.x = 0;
                world.y = step->variant * Trig_Cos(slot * 2340);
                world.z = step->variant * Trig_Sin(slot * 2340);
                step->variant += 2;
                EffectPosition_ApplyBaseAndYOffset(&world.x, &screen);
                screen.x =
                    (screen.x >> 17) + work->particles[index + 60].velocityX;
                screen.y =
                    (screen.y >> 16) + work->particles[index + 60].velocityY;
                screen.depth >>= 16;
                if (screen.depth <= 169)
                  screen.depth = 170;
                if (screen.depth > 350)
                  screen.depth = 350;
                span = 3 - (screen.depth - 170) / 90;
                EFFECT_DRAW_MOTE(draw[0], canvas, graphics, screen.x, screen.y,
                                 span);
                slot++;
              } while (slot != 28);
            }
          }
          index++;
        } while (index != 4);
      }
    }

    EFFECT_END_FRAME(work);
    frame++;
  } while (frame != 400);

  BattleEventRuntime_BeginPhaseFar(SOUND_HEAVY_IMPACT);

  // Put the targets back.
  EFFECT_FOR_EACH_TARGET(index, work->effect) {
    struct MotionObject *obj;

    battleObject = GetBattleObjectSlotFar(work->effect->actors[index]);
    obj = battleObject->object;
    obj->x = home[index].x;
    obj->z = home[index].y;
    obj->angle = baseAngle[index];
  }

  gProjection.centerX = 120;

  BattleFx_SetupBlendedDisplay();

  index = 0;
  do {
    ResourceObject_ReleaseFar(work->objects[index]);
    index++;
  } while (index != 9);

  EFFECT_FINISH_CANVAS();
}

// Embers, vortices, crystals and columns of lightning or fire.
// Each family reads its counts and animation geometry from effect_table.c.
//
// Two scenes raise embers, small motes that drift upwards:
//
// - The palette particles scene raises embers from the user's side while
//   flashes, sparks and falling streaks fly; in mode 0 a formation of eight
//   objects sweeps in with them and explodes at the end. gMapCellBuffer holds
//   512 embers, and work->particles hold 24 flashes, 16 streaks and 24
//   sparks, in that order. An idle particle's variant is -1; a live one's
//   counts its age up until it goes out.
// - The ember columns scene draws three wrapping columns of fire over a band
//   rising along the canvas floor. Its embers burst from one point, fall back
//   under gravity, and from frame 80 rise away; one that falls below the band
//   goes out and its slot is lit again.
//
// The ember columns' embers are shared with the falling sword (Titan Blade).
//
// The pillars of lightning (Plasma) and of fire (Volcano) strike columns one
// after another, each on its own schedule, and throw sparks up from each
// column's foot into one pool of 1024 in gMapCellBuffer. A spark's variant is
// its remaining life, 0 when idle.
//
// - The lightning pillars strike every 8 frames with a white flash, showing
//   two pictures over three frames. Their sparks move in 16.16 fixed point
//   and bounce off the ground at y = 104.
// - The fire pillars stand at random distances on the targets' side and
//   scroll a column of fire up with a flickering flame at its foot. Each
//   erupts with a fountain of 200 sparks and keeps spitting 4 a frame. Their
//   sparks move in 24.8 fixed point, with z holding the speed across, and
//   bounce off y = 120.
//
// These sparks differ from the shared spark bursts in their fixed point,
// their bounds and how fast they shrink, so they keep their own loops.

// Rising embers, and embers with three columns of fire.

// Abilities: Dragon Driver (kind 385).
//
// Battle effect 385: the rising embers from the user's side, in the
// LightningBolt colours.
DEFINE_EFFECT_KIND(BattleFx_RunDragonDriver, BattleFx_RunPaletteParticles, 1)

// Abilities: Tiamat (scene kind 7).
//
// Large battle effect 7 (BattleFx_RunSceneEffect): the rising embers with a
// formation of eight objects that sweeps in and explodes at the end.
DEFINE_EFFECT_KIND(BattleFx_RunTiamat, BattleFx_RunPaletteParticles, 0)

#define PALETTE_PARTICLES_FRAMES 208 // frames in the effect
// Falling streaks, work->particles 24 to 39.
#define PALETTE_PARTICLES_STREAKS 16
#define PALETTE_PARTICLES_FIRST_STREAK 24 // the first streak in work->particles
#define PALETTE_PARTICLES_FIRST_SPARK 40  // the first spark in work->particles
#define PALETTE_PARTICLES_EMBERS 512      // embers in PARTICLES
#define PALETTE_PARTICLES_FLASHES 24      // flashes, work->particles 0 to 23
#define PALETTE_PARTICLES_SPARKS 24       // sparks, work->particles 40 to 63
// The streak pictures in the sheet.
#define PALETTE_PARTICLES_STREAK_IMAGES 0x3000

// A 208-frame effect of embers rising from the ground; pressing A or B
// during frames 49 to 159 skips to frame 160. The targets reel on frame
// 138, the screen flares white over frames 160 to 175 and fades out after,
// and the battle shows the damage on frame 178.
// mode : 0 also brings in a formation of eight objects that sweeps in and,
//          at the end, explodes with BattleFx_RunImpactBurst;
//        1 throws the sparks and flashes from the user's side
//          (effect->side), in the LightningBolt colours.
// Four kinds of particle, in canvas coordinates with y growing downwards:
//   PARTICLES[0..511]  embers: rise from y 112, speeding up; one a frame
//                      over frames 33 to 103, eight a frame over 104 to 175
//   particles[40..63]  sparks: one on each odd frame from 41 to 127,
//                      thrown up at a slant
//   particles[0..23]   flashes: one a frame over 129 to 175, thrown across
//   particles[24..39]  streaks: fall at a slant from the top and, below
//                      y 55, play their images over and over
void BattleFx_RunPaletteParticles(struct BattleEffectArgument *effect,
                                  s32 mode) {
  struct EffectPosition origin;
  void **cursor;
  void *canvas;
  struct BattleCamera *camera;
  struct BattleEffectWork *work;
  BlitterPair draw;
  s32 frame;
  u8 *sheet;
  struct Point2D focus;
  s32 dir;
  u8 *palette;
  s32 i;

  // cursor points at gWorkSlot[HEAP_SLOT_BATTLE_CANVAS], the canvas.
  cursor = &gWorkSlot[HEAP_SLOT_BATTLE_CANVAS];
  canvas = cursor[0];
  camera = cursor[-28]; // [12] the camera
  work = cursor[-1];    // [39] the effect work
  sheet = cursor[HEAP_SLOT_BATTLE_SHEET -
                 HEAP_SLOT_BATTLE_CANVAS]; // [41] the particle images
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);
  REG_BLDALPHA = BLEND_ALPHA(16, 16); // an even mix
  REG_BG2CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                      COMPOSITE_MAX);
  draw[0] = CANVAS_SLOT(cursor, HEAP_SLOT_BLITTER);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                      COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, COMPOSITE_ADDITIVE);
  draw[1] = CANVAS_SLOT(cursor, HEAP_SLOT_BLITTER_ALTERNATE);

  // The flashes from FlashBurst, the streaks from EmberStreak (at sheet +
  // 0x3000).
  palette = Resource_GetTableEntry((s32)&ResourceId_FlashBurstSheet);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  palette += 128;
  Resource_DecodeType01(palette, work);
  palette = Resource_GetTableEntry((s32)&ResourceId_EmberStreakSheet);
  palette += 128;
  Resource_DecodeType01(palette, work->sheet + PALETTE_PARTICLES_STREAK_IMAGES);
  Resource_DecodeType01(
      Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesA), sheet);
  if (mode == 1)
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_LightningBoltSheet),
                    128);
  work->transferMode = CANVAS_COPY_AND_FADE; // trails keep 3/4 a frame
  work->transferValue = 75;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);

  // Mode 1 mirrors everything when the user is on the right (side 1).
  if (mode == 0)
    dir = 1;
  else if (work->effect->side == 1)
    dir = -1;
  else
    dir = 1;
  if (mode == 1) {
    EffectPosition_ApplyStepAndYOffset(work->effect->actor, &origin);
    origin.x /= 2;
    origin.y = 66;
    if (work->effect->side == 1)
      origin.x = 76;
    else
      origin.x = 44;
  }

  // The formation's centre starts at (212, -60).
  focus.x = 212 << 16;
  focus.y = -(60 << 16);
  EFFECT_FREE_PARTICLES(i, work->particles, 64);
  for (i = 0; i != PALETTE_PARTICLES_STREAKS; i++) {
    struct EffectStep *streak =
        &work->particles[PALETTE_PARTICLES_FIRST_STREAK + i];

    if (dir == 1)
      streak->x = (Random16() & 127) + 128;
    else
      streak->x = (Random16() & 127) - 128;
    streak->y = (Random16() & 7) - 72;
    streak->variant = -(Random16() & 31); // frames to wait before falling
  }
  EFFECT_FREE_PARTICLES(i, PARTICLES, PALETTE_PARTICLES_EMBERS);
  if (mode == 0) {
    BattleFx_SelectLivingTargets(work->effect);
    WaitFrames(1);
    BattleFx_SpawnObjects(8, 377, 2);
  }

  for (frame = 0; frame != PALETTE_PARTICLES_FRAMES; frame++) {
    // A or B skips to frame 160.
    if ((gKeysRepeat & (KEY_A | KEY_B)) && frame > 48 && frame <= 159) {
      if (mode == 0) {
        AnimationObjects_SelectAnimationFar(work->objects[0], 8);
        AnimationObjects_SelectAnimationFar(work->objects[1], 9);
        AnimationObjects_SelectAnimationFar(work->objects[3], 10);
        AnimationObjects_SelectAnimationFar(work->objects[4], 11);
      }
      EFFECT_FOR_EACH_TARGET(i, work->effect) {
        ObjectGroup_UpdateMembers(work->effect->actors[i],
                                  SPRITE_COLOR_RED_TINT, POSE_DAMAGE, -1, 0);
        BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 4);
      }
      frame = 160;
    }
    EFFECT_VIEW_FROM_CAMERA(camera);
    if (frame == 178)
      BattleEventRuntime_BeginPhaseFar(134);

    // From frame 128 the trails keep only half; over frames 160 to 175 the
    // canvas is filled each frame and the palette brightens; from 176 the
    // pixels fade 2 a frame.
    if (frame == 128)
      work->transferValue = 50;
    if (frame == 176) {
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_DARKEN, 0x02020202);
      palette = Resource_GetTableEntry((s32)&ResourceId_EmberStreakSheet);
      Iwram_CopyWords((void *)BG_PLTT, palette, 128);
    } else if (frame >= 160 && frame <= 175) {
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0x10101010);
      if (frame > 173)
        work->transferValue = 0x3f3f3f3f;
      Palette_BrightenBgEntries(2, 2, 2);
    }

    // Start new particles.

    // Embers: from y 112, moving up.
    if (frame >= 33 && frame <= 175) {
      s32 spawned = 0;
      s32 limit = 1;

      if (frame > 103)
        limit = 8;
      for (i = 0; i != PALETTE_PARTICLES_EMBERS; i++) {
        struct EffectStep *grain = &PARTICLES[i];

        if (grain->variant == -1) {
          grain->x = ((Random16() & 255) - 32) << 16;
          grain->y = 112 << 16;
          grain->velocityX =
              ((Random16() & 127) + gPaletteParticles_DropSpeeds[i & 3]) << 9;
          grain->velocityY =
              -((Random16() & 127) + gPaletteParticles_DropSpeeds[i & 3]) << 11;
          grain->variant = 0;
          spawned++;
          if (spawned == limit)
            break;
        }
      }
    }
    // Sparks: one on each odd frame, thrown up at a slant.
    if (frame >= 41 && frame <= 127) {
      s32 spawned = 0;

      if (frame & 1) {
        for (i = 0; i != PALETTE_PARTICLES_SPARKS; i++) {
          struct EffectStep *spark =
              &work->particles[PALETTE_PARTICLES_FIRST_SPARK + i];

          if (spark->variant == -1) {
            s32 speed = (Random16() & 255) + 128;
            s32 angle = (Random16() & 0x1fff) + 20000;

            if (mode == 0) {
              spark->x = ((Random16() & 7) + 78) << 16;
              spark->y = 70 << 16;
            } else {
              spark->x = ((Random16() & 7) + origin.x - 8) << 16;
              spark->y = origin.y << 16;
            }
            spark->velocityX = speed * Trig_Sin(angle) >> 9;
            spark->velocityY = speed * Trig_Cos(angle) >> 9;
            spawned++;
            spark->variant = 0;
            if (spawned == 1)
              break;
          }
        }
      }
    }
    if (frame == 48)
      AudioCommand_PlayFar(141);
    if (frame == 128)
      AudioCommand_PlayFar(145);
    // Flashes: one a frame, thrown across.
    if (frame >= 129 && frame <= 175) {
      s32 spawned = 0;

      for (i = 0; i != PALETTE_PARTICLES_FLASHES; i++) {
        struct EffectStep *flash = &work->particles[i];

        if (flash->variant == -1) {
          s32 speed = (Random16() & 255) + 128;
          s32 angle = (Random16() & 0x1fff) - 20000;

          if (mode == 0) {
            flash->x = 68 << 16;
            flash->y = 64 << 16;
          } else {
            flash->x = origin.x << 16;
            flash->y = origin.y << 16;
          }
          flash->velocityX = speed * Trig_Sin(angle) >> 6;
          flash->velocityY = speed * Trig_Cos(angle) >> 6;
          flash->variant = 0;
          spawned++;
          if (spawned == 1)
            break;
        }
      }
    }

    // Draw and move the particles.

    // Flashes: a new image every 4 frames; gone after 24 frames.
    if (frame <= 175) {
      for (i = 0; i != PALETTE_PARTICLES_FLASHES; i++) {
        struct EffectStep *flash = &work->particles[i];

        if (flash->variant >= 0) {
          draw[0](canvas, work->sheet + ((flash->variant >> 2) << 11),
                  HI(flash->x) - 16, HI(flash->y) - 32, 32, 64);
          flash->x += flash->velocityX * dir;
          flash->y += flash->velocityY;
          flash->variant++;
          if (flash->variant == PALETTE_PARTICLES_FLASHES)
            flash->variant = -1;
        }
      }
    }

    // Sparks: 1x2 dots that speed upwards; gone after 48 frames.
    for (i = 0; i != PALETTE_PARTICLES_SPARKS; i++) {
      struct EffectStep *spark =
          &work->particles[PALETTE_PARTICLES_FIRST_SPARK + i];
      s32 size = 1;

      if (spark->variant >= 0) {
        draw[1](canvas, gPaletteParticles_SparkPixels, HI(spark->x),
                HI(spark->y) - size, size, size * 2);
        spark->x += spark->velocityX * dir;
        spark->y += spark->velocityY;
        spark->velocityY += -0x400;
        spark->variant++;
        if (spark->variant == 48)
          spark->variant = -1;
      }
    }

    // Embers: speed upwards until frame 128, then keep their climb and curve
    // away to the left (to the right when dir is -1), the odd ones harder;
    // gone after 256 frames.
    if (frame <= 175) {
      for (i = 0; i != PALETTE_PARTICLES_EMBERS; i++) {
        struct EffectStep *grain = &PARTICLES[i];

        if (grain->variant >= 0) {
          u8 size = gPaletteParticles_DropSizes[i & 3];
          s32 height = size * 2;

          draw[0](canvas, sheet + gBattleFx_MoteCellOffsets[size - 1],
                  HI(grain->x) - size / 2, HI(grain->y) - size, size, height);
          grain->x += grain->velocityX * dir;
          grain->y += grain->velocityY;
          if (frame > 128) {
            if (i & 1)
              grain->velocityX += -0x8000;
            else
              grain->velocityX += -0x2000;
          } else {
            grain->velocityY += -0x400;
          }
          grain->variant++;
          if (grain->variant == 256)
            grain->variant = -1;
        }
      }
    }

    // The shake and the formation.
    if (frame == 128)
      work->shakeFrames = 48;
    if (mode == 0 && frame == 48)
      work->shakeFrames = 8;

    // Over frames 40 to 47 the formation moves 8 pixels left and 16 down a
    // frame; from 128 to 175 four of its objects play animations 8 to 11.
    if (frame >= 40 && frame <= 47) {
      focus.x += -(8 << 16);
      focus.y += 16 << 16;
    }
    if (mode == 0) {
      if (frame == 128) {
        AnimationObjects_SelectAnimationFar(work->objects[0], 8);
        AnimationObjects_SelectAnimationFar(work->objects[1], 9);
        AnimationObjects_SelectAnimationFar(work->objects[3], 10);
        AnimationObjects_SelectAnimationFar(work->objects[4], 11);
      }
      if (frame == 176) {
        AnimationObjects_SelectAnimationFar(work->objects[0], 0);
        AnimationObjects_SelectAnimationFar(work->objects[1], 1);
        AnimationObjects_SelectAnimationFar(work->objects[3], 3);
        AnimationObjects_SelectAnimationFar(work->objects[4], 4);
      }
      BattleFx_PlaceFormationObjects(3, focus.x, focus.y);
    }
    if (frame == 138) {
      EFFECT_FOR_EACH_TARGET(i, work->effect) {
        ObjectGroup_UpdateMembers(work->effect->actors[i],
                                  SPRITE_COLOR_RED_TINT, POSE_DAMAGE, -1, 0);
        BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 4);
      }
    }

    // Streaks: after their wait, fall 6 pixels across and 6 down a frame;
    // below y 55 they play six images, two frames each, over and over.
    if (frame <= 175) {
      for (i = 0; i != PALETTE_PARTICLES_STREAKS; i++) {
        struct EffectStep *streak =
            &work->particles[PALETTE_PARTICLES_FIRST_STREAK + i];

        if (streak->y > 55) {
          if (streak->variant >= 0 && streak->variant <= 11) {
            s32 cel = streak->variant / 2;

            draw[0](canvas,
                    work->sheet + gPaletteParticles_StreakOffsets[cel] +
                        PALETTE_PARTICLES_STREAK_IMAGES,
                    streak->x - gPaletteParticles_StreakWidths[cel] / 2,
                    streak->y + gPaletteParticles_StreakDrops[cel],
                    gPaletteParticles_StreakWidths[cel],
                    gPaletteParticles_StreakHeights[cel]);
          }
          streak->variant++;
          if (streak->variant == 12)
            streak->variant = 0;
        } else if (streak->variant == 0) {
          s32 size = 10;

          streak->x -= dir * 6;
          streak->y += 6;
          if (frame <= 47 && streak->y > 55)
            AudioCommand_PlayFar(136);
          draw[0](canvas, sheet + gBattleFx_MoteCellOffsets[size - 1],
                  streak->x - size / 2, streak->y + 30, size, size * 2);
        } else {
          streak->variant++;
        }
      }
    }
    Camera_ApplyShake(8, 8);
    EFFECT_END_FRAME(work);
  }

  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  EFFECT_RELEASE_BLITTERS();
  if (mode == 0) {
    BattleFx_RunImpactBurst(3, focus.x, focus.y);
    for (i = 0; i != 8; i++)
      ResourceObject_ReleaseFar(work->objects[i]);
  }
  BattleFx_EndCanvasLayer();
}

#define NEREID_FRAMES 124 // frames in the effect

// gNereid_Columns has two entries for each column.
#define NEREID_COLUMN_X(column) gNereid_Columns[(column) * 2]
#define NEREID_COLUMN_Y(column) gNereid_Columns[(column) * 2 + 1]
#define NEREID_OBJECTS 12         // objects in the sweeping formation
#define NEREID_COLUMNS 3          // wrapping columns of fire
#define NEREID_EMBERS 1024        // embers in PARTICLES
#define NEREID_COLUMN_IMAGE 0x6e4 // the column of fire in the sheet

// Abilities: Nereid (scene kind 2).
//
// Large battle effect 2 (BattleFx_RunSceneEffect), 124 frames; pressing A
// or B during frames 33 to 97 skips to frame 98. A formation of twelve
// objects sweeps in from the right; embers burst up on frame 28 and over
// frames 32 to 79, fall back under gravity and from frame 80 rise away,
// while three columns of fire stream upwards over frames 32 to 79 and the
// targets bounce. At the end the formation explodes with
// BattleFx_RunImpactBurst; the battle shows the damage on frame 120.
void BattleFx_RunNereid(struct BattleEffectArgument *effect) {
  BlitterPair draw;
  void **cursor;
  void *canvas;
  struct BattleEffectWork *work;
  u8 *graphics;
  u8 *palette;

  // cursor points at gWorkSlot[HEAP_SLOT_BATTLE_CANVAS], the canvas.
  cursor = &gWorkSlot[HEAP_SLOT_BATTLE_CANVAS];
  canvas = cursor[0];
  work = cursor[-1]; // [39] the effect work
  graphics = cursor[HEAP_SLOT_BATTLE_SHEET -
                    HEAP_SLOT_BATTLE_CANVAS]; // the particle images
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);
  REG_BLDALPHA = BLEND_ALPHA(16, 16); // an even mix
  BattleFx_FetchRectangleBlitters(0, draw);

  // The band along the bottom from WaterSpray, the columns from FirePillarA
  // (at sheet + 0x6e4).
  palette = Resource_GetTableEntry((s32)&ResourceId_WaterSpraySheet);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  palette += 128;
  Resource_DecodeType01(palette, work);
  palette = Resource_GetTableEntry((s32)&ResourceId_FirePillarSheetA);
  palette += 128;
  Resource_DecodeType01(palette, work->sheet + NEREID_COLUMN_IMAGE);
  Resource_DecodeType01(
      Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesA), graphics);
  work->transferMode = CANVAS_COPY_AND_FADE; // trails keep 3/4 a frame
  work->transferValue = 75;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);
  {
    struct Point2D focus;
    struct Point2D velocity;
    s32 rise;
    s32 frame;
    s32 k;
    struct EffectStep *point;
    struct EffectStep *spark;
    struct EffectStep *sparks;
    s32 *life;

    // The formation comes in from (256, 88), heading left.
    rise = 0;
    focus.x = 256 << 16;
    focus.y = 88 << 16;
    velocity.x = -(16 << 16);
    velocity.y = -(4 << 16);
    EFFECT_FREE_PARTICLES(k, work->particles, 64);
    for (k = 0; k != 16; k++) {
      point = &work->particles[24 + k];
      point->x = Random16() & 127;
      point->y = (Random16() & 7) + 56;
      point->variant = -(Random16() & 15);
    }
    life = &gMapCellBuffer->variant;
    for (k = 0; k != NEREID_EMBERS; k++) {
      *life = -1;
      life += 7; // sizeof(struct EffectStep) / 4
    }
    sparks = gMapCellBuffer;
    BattleFx_SelectLivingTargets(work->effect);
    WaitFrames(1);
    BattleFx_SpawnObjects(NEREID_OBJECTS, 380, 2);

    for (frame = 0; frame != NEREID_FRAMES; frame++) {
      if ((gKeysRepeat & (KEY_A | KEY_B)) && frame > 32 && frame <= 97)
        frame = 98;
      if (frame == 120)
        BattleEventRuntime_BeginPhaseFar(SOUND_HEAVY_IMPACT);
      if (frame <= 15)
        rise += 2;

      // The formation slows to 58/64 of its speed across and 56/64 down
      // each frame, and is pushed back right when left of x 120.
      if (frame <= 99) {
        focus.x += velocity.x;
        focus.y += velocity.y;
        velocity.x = velocity.x * 58 / 64;
        velocity.y = velocity.y * 56 / 64;
        if (focus.x < 120 << 16)
          velocity.x += 0x8000;
      }
      BattleFx_PlaceFormationObjects(1, focus.x, focus.y);

      // Embers burst up from around (32, 96): 256 on frame 28, then 16 a
      // frame over frames 32 to 79.
      if (frame == 28) {
        for (k = 0; k != 256; k++) {
          spark = &sparks[k];
          if (spark->variant == -1) {
            s32 radius = Random16() & 63;
            s32 angle = Random16() & 0xffff;

            spark->x = ((Trig_Sin(angle) * radius) >> 3) + (32 << 16);
            spark->y = ((Trig_Cos(angle) * radius) >> 2) + (96 << 16);
            spark->velocityX = ((Random16() & 63) - 32) << 14;
            spark->velocityY = (-(Random16() & 63) - 8) << 13;
            spark->variant = 0;
          }
        }
      }
      if (frame >= 32 && frame < 80) {
        s32 spawned = 0;

        for (k = 0; k != NEREID_EMBERS; k++) {
          spark = &sparks[k];
          if (spark->variant == -1) {
            s32 radius = Random16() & 63;
            s32 angle = Random16() & 0xffff;

            spark->x = ((Trig_Sin(angle) * radius) >> 3) + (32 << 16);
            spark->y = ((Trig_Cos(angle) * radius) >> 2) + (96 << 16);
            spark->velocityX = ((Random16() & 63) - 32) << 14;
            spark->velocityY = (-(Random16() & 63) - 8) << 13;
            spark->variant = 0;
            spawned++;
            if (spawned == 16)
              break;
          }
        }
      }
      if (frame == 0)
        AudioCommand_PlayFar(164);
      if (frame == 32)
        AudioCommand_PlayFar(145);
      if (frame == 80)
        AudioCommand_PlayFar(144);

      // The columns: a 104-pixel image scrolling up 16 pixels a frame,
      // drawn twice to wrap round.
      if (frame >= 32 && frame < 80) {
        for (k = 0; k != NEREID_COLUMNS; k++) {
          s32 span = (frame * 16 - 256 + k * 25) % 104;

          draw[0](canvas, work->sheet + NEREID_COLUMN_IMAGE,
                  NEREID_COLUMN_X(k) - 17, NEREID_COLUMN_Y(k) - span - 104, 34,
                  104);
          draw[0](canvas, work->sheet + NEREID_COLUMN_IMAGE,
                  NEREID_COLUMN_X(k) - 17, NEREID_COLUMN_Y(k) - span, 34, span);
        }
      }

      // The band along the bottom: five 32x32 images that rise 32 pixels
      // over the first 16 frames and drift right.
      if (frame <= 95) {
        for (k = 0; k != 5; k++)
          draw[0](canvas, work, k * 32 + ((frame / 4) & 31) - 32, 120 - rise,
                  32, 32);
      }

      // Embers: grow from frame 68; those falling are drawn additively. Until
      // frame 80 they fall under gNereid_Gravity, then speed upwards;
      // they slow to 62/64 a frame, and go out when they fall below y 104.
      for (k = 0; k != NEREID_EMBERS; k++) {
        spark = &sparks[k];
        if (spark->variant >= 0) {
          s32 size = k % 3 + 2;

          if (spark->velocityY > 0)
            size += 2;
          if (frame > 68 && size <= 5)
            size = 6;
          if (frame > 70 && size <= 6)
            size = 7;
          if (frame > 72 && size <= 7)
            size = 8;
          if (frame > 74 && size <= 8)
            size = 9;
          if (frame > 76)
            size = 10;
          EFFECT_DRAW_MOTE(draw[spark->velocityY > 0], canvas, graphics,
                           HI(spark->x), HI(spark->y), size);
          spark->x += spark->velocityX;
          spark->y += spark->velocityY;
          if (frame > 80)
            spark->velocityY += -0x8000;
          else
            spark->velocityY += gNereid_Gravity[k & 3];
          spark->velocityX = spark->velocityX * 62 / 64;
          spark->velocityY = spark->velocityY * 62 / 64;
          spark->variant++;
          if (spark->velocityY > 0 && HI(spark->y) > 104)
            spark->variant = -1;
        }
      }

      // The targets bounce and flash white on frames 36, 48, 60 and 72,
      // getting their colours back 6 frames later.
      if (frame <= 79) {
        EFFECT_FOR_EACH_TARGET(k, work->effect) {
          if (frame > 29) {
            s32 phase = frame % 12;

            if (phase == 0) {
              struct MotionObject *obj =
                  GetBattleObjectSlotFar(work->effect->actors[k])->object;

              ObjectGroup_UpdateMembers(work->effect->actors[k],
                                        SPRITE_COLOR_WHITE, POSE_DAMAGE, -1, 0);
              obj->velocityY = 0x48000;
              obj->verticalMotionStrength = 0xab85;
            }
            if (phase == 6)
              ObjectGroup_UpdateMembers(work->effect->actors[k], 0, POSE_DAMAGE,
                                        -1, 0);
          }
        }
      }
      EFFECT_PRESENT_FRAME(work);
    }

    Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
    EFFECT_RELEASE_BLITTERS();
    BattleFx_RunImpactBurst(1, focus.x, focus.y);
    for (k = 0; k != NEREID_OBJECTS; k++)
      ResourceObject_ReleaseFar(work->objects[k]);
  }
  BattleFx_EndCanvasLayer();
}

// gFroth_Counts has two entries for each variant.
#define FROTH_MOTE_COUNT(variant) gFroth_Counts[(variant) * 2]
#define FROTH_FRAMES(variant) gFroth_Counts[(variant) * 2 + 1]
#define FROTH_MOTE_CELL 0xc00 // the mote picture in the sheet
#define FROTH_FIRST_TARGET 32 // the first target seat in work->particles

// Abilities: Froth, Froth Sphere, Froth Spiral (kind 10).
//
// Battle effect 10: motes fly out of the top of the user, hang, and from 48
// frames after each appears home in on the targets in turn; each that
// reaches the ground strikes in a burst, flashing and knocking back its
// target, while the camera swings round over frames 17 to 63.
// effect->variant picks the volley from gFroth_Counts (motes and
// frames) and how hard the screen shakes.
void BattleFx_RunFroth(struct BattleEffectArgument *effect) {
  struct EffectPosition screenPosition;
  struct BattleEffectWork *work;
  void *canvas;
  BlitterPair draw;
  u8 *palette;
  struct MotionObject *source;
  struct MotionObject *object;
  struct EffectStep *point;
  struct EffectStep *seat;
  struct EffectStep *target;
  struct BattleCamera *camera;
  s32 frame;
  s32 y;
  s32 i;
  s8 *flags;

  flags = (s8 *)gMapCellBuffer; // nonzero once a mote has struck
  work = *(gWorkSlot + HEAP_SLOT_BATTLE_EFFECT);
  canvas = *(gWorkSlot + HEAP_SLOT_BATTLE_CANVAS);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(1);
  palette = Resource_GetTableEntry((s32)&ResourceId_VortexSheet);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  palette += 128;
  Resource_DecodeType01(palette, work);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS, COMPOSITE_MAX);
  draw[0] = (DrawRectangle) * (gWorkSlot + HEAP_SLOT_BLITTER);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP, COMPOSITE_MAX);
  REG_BLDALPHA = BLEND_ALPHA(15, 15); // 15/16 of each
  draw[1] = (DrawRectangle) * (gWorkSlot + HEAP_SLOT_BLITTER_ALTERNATE);
  source = GetBattleObjectSlotFar(work->effect->actor)->object;
  y = source->y + Battle_GetObjectTableValueFar(work->effect->actor);
  for (i = 0; i != 30; i++) {
    point = &work->particles[i];
    EFFECT_SET_POSITION(point, source->x, y, source->z);
    EFFECT_SET_VELOCITY(point, ((s32)((Random16() & 255) - 127) << 16) >> 5,
                        ((s32)((Random16() & 127) - 16) << 16) >> 6,
                        ((s32)((Random16() & 255) - 127) << 16) >> 5);
    point->variant = -1;
    flags[i] = 0;
  }
  // Where each target stands, on the ground.
  EFFECT_FOR_EACH_TARGET(i, work->effect) {
    seat = &work->particles[FROTH_FIRST_TARGET + i];
    object = GetBattleObjectSlotFar(work->effect->actors[i])->object;
    EFFECT_SET_POSITION(seat, object->x, 0, object->z);
  }
  work->cameraDelta = 0;
  work->cameraPhase = 0;
  Scheduler_AddOrUpdateCallback(Camera_ApplyPhasedDelta, 0x480);
  work->transferMode = CANVAS_COPY_AND_FADE; // trails keep 3/4 a frame
  work->transferValue = 75;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);
  AudioCommand_PlayFar(164);

  for (frame = 0; frame != FROTH_FRAMES(work->effect->variant); frame++) {
    camera = *(gWorkSlot + HEAP_SLOT_CAMERA);
    if (frame >= 17 && frame < 64)
      work->cameraDelta = 0x180;
    else
      work->cameraDelta = 0;
    // 16 frames before the end the battle shows the damage, with sound 132
    // on each hit.
    if (frame == FROTH_FRAMES(work->effect->variant) - 16)
      BattleEventRuntime_BeginPhaseFar(132);
    EFFECT_VIEW_FROM_CAMERA(camera);
    for (i = 0; i != FROTH_MOTE_COUNT(work->effect->variant); i++) {
      point = &work->particles[i];
      if (frame > i * 2 && flags[i] == 0) {
        EffectPosition_ApplyBaseAndYOffset(point, &screenPosition);
        screenPosition.x >>= 1;
        EFFECT_CLAMP(screenPosition.depth, 160, 0x31f);
        draw[0](canvas, work->sheet + FROTH_MOTE_CELL, screenPosition.x - 6,
                screenPosition.y - 12, 12, 24);
        point->x += point->velocityX;
        point->y += point->velocityY;
        point->z += point->velocityZ;
      }
      // Mote i appears on frame 2i; 48 frames later it starts to home in on
      // its target, damped until frame 2i + 85, and strikes on reaching
      // the ground.
      if (frame > i * 2 + 48 && flags[i] == 0) {
        target = &work->particles[FROTH_FIRST_TARGET + i % work->effect->count];
        point->velocityX += (target->x - point->x) >> 9;
        point->velocityY += (target->y - point->y) >> 9;
        point->velocityZ += (target->z - point->z) >> 9;
        if (frame < i * 2 + 85) {
          EFFECT_SET_VELOCITY(point, point->velocityX * 60 / 64,
                              point->velocityY * 60 / 64,
                              point->velocityZ * 60 / 64);
        }
        if (point->y < 0) {
          flags[i] = 1;
          point->variant = 0;
          point->x = screenPosition.x;
          point->y = screenPosition.y + (Random16() & 31) - 16;
          ObjectGroup_UpdateMembers(
              work->effect->actors[i % work->effect->count], SPRITE_COLOR_WHITE,
              POSE_DAMAGE, i % work->effect->count, 4);
          BattleMotion_ApplyVariantMotionFar(
              work->effect->actors[i % work->effect->count], 0);
          work->shakeFrames = 4;
          AudioCommand_PlayFar(132);
        }
      }
      // The burst where a mote struck: two mirrored halves, three images.
      if (point->variant >= 0 && point->variant < 16) {
        draw[0](canvas, work->sheet + ((point->variant / 2) % 3 << 10),
                point->x - 16, point->y - 56, 16, 64);
        draw[1](canvas, work->sheet + ((point->variant / 2) % 3 << 10),
                point->x, point->y - 56, 16, 64);
        point->variant++;
      }
    }
    Camera_ApplyShake(work->effect->variant * 2 + 2,
                      work->effect->variant * 2 + 2);
    if (work->cameraPhase == 0)
      work->cameraPhase = 1;
    EFFECT_END_FRAME(work);
  }

  Scheduler_RemoveCallback(Camera_ApplyPhasedDelta);
  EFFECT_FINISH_CANVAS();
}

// gWhirlwind_Shapes has three entries for each variant.
#define WHIRLWIND_TORNADOES(variant) gWhirlwind_Shapes[(variant) * 3]
#define WHIRLWIND_SWAY(variant) gWhirlwind_Shapes[(variant) * 3 + 1]
#define WHIRLWIND_SHAKE(variant) gWhirlwind_Shapes[(variant) * 3 + 2]

// Abilities: Whirlwind, Tornado, Tempest, Gale, Typhoon, Hurricane (kind 20).
//
// Battle effect 20: tornadoes sweep across the targets for 80 frames,
// sparkling with glints; the canvas flashes and the targets reel seven
// times, 8 frames apart, from frame 16, and the battle shows the damage on
// frame 32.
// effect->variant picks the shape from gWhirlwind_Shapes: how many
// tornadoes, how far they sway and how hard the screen shakes.
void BattleFx_RunWhirlwind(struct BattleEffectArgument *effect) {
  BlitterPair draw;
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 wave;
  u8 *palette;
  struct EffectStep *point;
  s32 frame;
  s32 x;
  s32 y;
  s32 segment;
  s32 cell;
  s32 i;
  s32 j;

  slots = &((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BATTLE_EFFECT];
  cursor = slots;
  EFFECT_GET_WORK_AND_CANVAS(cursor, work, canvas);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(1);
  REG_BLDALPHA = BLEND_ALPHA(16, 16); // an even mix
  palette = Resource_GetTableEntry((s32)&ResourceId_TornadoSheet);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  palette += 128;
  Resource_DecodeType01(palette, work);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS, COMPOSITE_MAX);
  draw[0] = (DrawRectangle)EFFECT_SLOT(slots, HEAP_SLOT_BLITTER);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_MAX);
  draw[1] = (DrawRectangle)EFFECT_SLOT(slots, HEAP_SLOT_BLITTER_ALTERNATE);

  // The glints: each waits up to 15 frames, then plays at a random spot on
  // its tornado.
  for (j = 0; j != 16; j++) {
    point = &work->particles[j];
    point->x = Random16() & 31;
    point->y = (Random16() & 63) + 16;
    point->variant = -(Random16() & 15);
  }
  work->transferMode = CANVAS_COPY_AND_FADE; // trails keep half a frame
  work->transferValue = 50;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);
  REG_BLDALPHA = BLEND_ALPHA(0, 16); // invisible to start with
  WaitFrames(1);
  AudioCommand_PlayFar(141);

  for (frame = 0; frame != 80; frame++) {
    wave = (Trig_Sin(frame << 10) * 16);
    if (frame == 32)
      BattleEventRuntime_BeginPhaseFar(133);
    // Seven flashes of the canvas, 8 frames apart.
    for (i = 0; i != 7; i++) {
      if (frame == i * 8 + 16)
        Iwram_FillWords(canvas, 0x4000, 0x08080808);
    }
    if (work->effect->side == 1)
      wave += 0x200000;
    else
      wave -= 0x200000;
    // Fade in over 16 frames and out over the last 16.
    if (frame <= 16)
      REG_BLDALPHA = frame | 0x1000;
    if (frame > 63)
      REG_BLDALPHA = (0x4f - frame) | 0x1000;
    // Each tornado: three sections stacked, swaying about the sweep, a
    // quarter turn apart, with four glints.
    for (i = 0; i != WHIRLWIND_TORNADOES(work->effect->variant); i++) {
      x = ((WHIRLWIND_SWAY(work->effect->variant) *
                Trig_Sin((frame << 11) + i * 0x4000) +
            wave) >>
           16) +
          40;
      y = (Trig_Cos((frame << 11) + i * 0x4000) * 2) >> 16;
      segment = (frame / 2) % 3;
      draw[0](canvas, work->sheet + segment * 0xa00 + 0xc56, x, y + 16, 40, 32);
      draw[0](canvas, work->sheet + segment * 0x500 + 0x2a56, x, y + 48, 40,
              32);
      draw[0](canvas, work->sheet + segment * 0xa00 + 0x1156, x, y + 80, 40,
              32);
      for (j = 0; j != 4; j++) {
        point = &work->particles[i * 4 + j];
        if (point->variant >= 0) {
          cell = point->variant / 2 + (j / 2) * 3;
          draw[j & 1](canvas, work->sheet + gBattleFx_GlintCellOffsets[cell],
                      point->x + x, point->y + y,
                      gBattleFx_GlintCellWidths[cell],
                      gBattleFx_GlintCellHeights[cell]);
        }
        point->variant++;
        if (point->variant == 6) {
          point->x = Random16() & 31;
          point->y = (Random16() & 63) + 16;
          point->variant = 0;
        }
      }
    }
    // Target j reels 3j frames after each flash.
    EFFECT_FOR_EACH_TARGET(j, work->effect) {
      for (i = 0; i != 7; i++) {
        if (frame == i * 8 + j * 3 + 16) {
          ObjectGroup_UpdateMembers(work->effect->actors[j], SPRITE_COLOR_WHITE,
                                    POSE_DAMAGE, j, 4);
          BattleMotion_ApplyVariantMotionFar(work->effect->actors[j], 6);
        }
      }
    }
    work->shakeFrames = 1;
    Camera_ApplyShake(WHIRLWIND_SHAKE(work->effect->variant),
                      WHIRLWIND_SHAKE(work->effect->variant) * 2);
    EFFECT_END_FRAME(work);
  }

  EFFECT_RELEASE_BLITTERS();
  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  BattleFx_EndCanvasLayer();
}

// Falling crystals.

// gPrism_Counts has two entries for each variant.
#define PRISM_CRYSTAL_COUNT(variant) gPrism_Counts[(variant) * 2]
#define PRISM_FRAMES(variant) gPrism_Counts[(variant) * 2 + 1]

// gPrism_ShardStarts has an x and a y for each shard.
#define PRISM_SHARD_START_X(shard) gPrism_ShardStarts[(shard) * 2]
#define PRISM_SHARD_START_Y(shard) gPrism_ShardStarts[(shard) * 2 + 1]

// Abilities: Prism, Hail Prism, Freeze Prism (kind 8).
//
// Battle effect 8: crystals drop out of the sky onto the targets' side, 8
// frames apart, and shatter on the ground into 16 spinning shards each,
// flashing the targets and shaking the screen; the battle shows the damage
// 80 frames before the end, and the canvas darkens away over the last 8.
// effect->variant picks the volley from gPrism_Counts (crystals and
// frames: 1 in 110, 4 in 134, 12 in 166); with variant 2 the camera also
// swings round for the first 104 frames.
// The crystals are particles[]: z is 1 once shattered, variant is the frame
// it starts to fall. The shards are 16 per crystal in gMapCellBuffer: z is
// how fast it spins.
void BattleFx_RunPrism(struct BattleEffectArgument *effect) {
  BlitterPair draw;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  struct EffectStep *seed;
  struct EffectStep *crystal;
  struct EffectStep *shards;
  struct EffectStep *shard;
  s32 x;
  s32 cell;
  s32 i;
  s32 j;

  shards = gMapCellBuffer;
  cursor = &gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];
  work = *cursor++;
  canvas = *cursor;
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);
  REG_BLDALPHA = BLEND_ALPHA(16, 16); // an even mix
  Resource_LoadAndDecompress((s32)&ResourceId_CrystalSheet, work->sheet, 1, 1);
  BattleFx_FetchRectangleBlitters(work->effect->side, draw);
  work->transferMode = CANVAS_COPY_AND_FADE; // trails keep half a frame
  work->transferValue = 50;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);

  // Line the crystals up above the screen, each starting 8 frames after
  // the last and drifting towards the middle as it falls.
  for (j = 0; j != PRISM_CRYSTAL_COUNT(work->effect->variant); j++) {
    seed = &work->particles[j];
    x = Random16() & 31;
    seed->y = -0x400000;
    if (work->effect->side == 1) {
      x = ((Random16() & 31) + 80) << 16;
      seed->velocityX = (Random16() & 63) << 12;
    } else {
      x = ((Random16() & 31) + 8) << 16;
      seed->velocityX = -(Random16() & 63) << 12;
    }
    seed->x = x - seed->velocityX * 18;
    seed->velocityY = 0;
    seed->z = 0;
    seed->variant = j * 8;
  }

  for (frame = 0; frame != PRISM_FRAMES(work->effect->variant); frame++) {
    if (work->effect->variant == 2 && frame <= 103) {
      // Swing the camera round, slowing from frame 96.
      struct BattleCamera *camera = gCameraWork;
      s32 speed;

      if (frame <= 95)
        speed = 192;
      else
        speed = 2496 - frame * 24;
      if (work->effect->side == 0)
        camera->yaw -= speed;
      else
        camera->yaw += speed;
    }
    if (frame == PRISM_FRAMES(work->effect->variant) - 80)
      BattleEventRuntime_BeginPhaseFar(SOUND_HEAVY_IMPACT);
    if (frame == PRISM_FRAMES(work->effect->variant) - 8) {
      // Darken the canvas away over the last 8 frames.
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_DARKEN, 0x06060606);
    }
    if (frame <= PRISM_FRAMES(work->effect->variant) - 8) {
      for (i = 0; i != PRISM_CRYSTAL_COUNT(work->effect->variant); i++) {
        crystal = &work->particles[i];
        if (crystal->z == 1) {
          // A shattered crystal: its shards fly and spin, slowing down.
          for (j = 0; j != 16; j++) {
            cell = (j % 5) * 3 + (shards[i * 16 + j].variant / 96) % 3;
            shard = &shards[i * 16 + j];
            draw[j <= 2](canvas,
                         work->sheet + gPrism_ShardOffsets[cell] + 0x800,
                         HI(shard->x) - gPrism_ShardWidths[cell] / 2,
                         HI(shard->y) - gPrism_ShardHeights[cell] / 2,
                         gPrism_ShardWidths[cell], gPrism_ShardHeights[cell]);
            EffectStep_AdvanceWithGravity2D(shard, 64, 0x2000);
            shard->variant += shard->z;
            if (shard->z > 1 && (frame & 1))
              shard->z--;
          }
        } else if (frame >= crystal->variant) {
          // A falling crystal (32x64).
          draw[i & 1](canvas, work, HI(crystal->x) - 16, HI(crystal->y), 32,
                      64);
          EffectStep_AdvanceWithGravity2D(crystal, 64, 0x10000);
          if (crystal->y > 0x380000) {
            // Shatter it on reaching the ground (y 56).
            crystal->z = 1;
            crystal->y = 0x380000;
            for (j = 0; j != 16; j++) {
              shard = &shards[i * 16 + j];
              shard->x = ((PRISM_SHARD_START_X(j) - 40) << 16) + crystal->x;
              shard->y = PRISM_SHARD_START_Y(j) << 16;
              shard->velocityX = ((Random16() & 127) - 64) << 11;
              shard->velocityY = -(Random16() & 127) << 11;
              if (i & 1) {
                shard->velocityX *= 2;
                shard->velocityY *= 2;
              }
              shard->z = 32;
              shard->variant = 0;
            }
            work->shakeFrames = 8;
            AudioCommand_PlayFar(144);
            EFFECT_FOR_EACH_TARGET(j, work->effect)
              ObjectGroup_UpdateMembers(work->effect->actors[j],
                                        SPRITE_COLOR_WHITE, POSE_DAMAGE, j, 4);
          }
        }
      }
    }
    Camera_ApplyShake(work->effect->variant * 2 + 4,
                      work->effect->variant * 4 + 8);
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS();
}

// Rebuilds background colours 1 to 63 as a grey ramp whose red, green and
// blue each sway with the frame, out of step by their phases (1/65536ths
// of a turn), so the effect shimmers through colours.
// frame      : the effect's frame; the sway turns 1/64 of a circle a frame
// redPhase, greenPhase, bluePhase : each channel's offset in the sway
void Graphics_UpdatePhasePalette(s32 frame, s32 redPhase, s32 greenPhase,
                                 s32 bluePhase) {
  u16 palette[64];
  u16 *output;
  s32 phase;
  s32 redOffset;
  s32 greenOffset;
  s32 blueOffset;
  s32 index;

  phase = frame * 0x400;
  redOffset = (Trig_Sin(phase + redPhase) * 16) >> 15;
  greenOffset = (Trig_Sin(phase + greenPhase) * 16) >> 15;
  blueOffset = (Trig_Sin(phase + bluePhase) * 16) >> 15;

  palette[0] = 0;
  index = 1;
  output = &palette[1];
  do {
    s32 red;
    s32 green;
    s32 blue;

    red = (index + redOffset) / 2;
    green = (index + greenOffset) / 2;
    blue = (index + blueOffset) / 2;

    EFFECT_CLAMP(red, 0, 31);
    EFFECT_CLAMP(green, 0, 31);
    EFFECT_CLAMP(blue, 0, 31);

    *output++ = (blue << 10) | (green << 5) | red;
    index++;
  } while (index != 64);

  Iwram_CopyWords((void *)&BG_PLTT[1], palette, sizeof(palette));
}

// Pillars of lightning and the sparks they throw up.

#define LIGHTNING_PILLAR_SPARK_COUNT 1024 // sparks

// gPlasma_Sparks has two entries for each variant.
#define PLASMA_SPARKS(variant) gPlasma_Sparks[(variant) * 2]
#define PLASMA_SHAKE_FRAMES(variant) gPlasma_Sparks[(variant) * 2 + 1]
#define PLASMA_SPACING 8           // frames between pillars
#define PLASMA_SECOND_IMAGE 0x1500 // the second picture in the sheet

// gPlasma_Columns has seven pillars for each side.
#define PLASMA_COLUMN_X(side, pillar) gPlasma_Columns[(pillar) + (side) * 7]

// Abilities: Plasma, Shine Plasma, Spark Plasma, Thunderclap, Thunderbolt,
// Thunderstorm (kind 18).
//
// Battle effect 18: pillars of lightning strike one every 8 frames, each
// with a white flash, throwing sparks up from the ground and knocking the
// targets back; the battle shows the damage on frame 32.
// effect->variant picks how many pillars (gPlasma_Counts), how many
// sparks each throws and how long the screen shakes (gPlasma_Sparks);
// with variant 2 the camera also swings round for the first 64 frames.
// effect->side picks the row of pillar positions.
void BattleFx_RunPlasma(struct BattleEffectArgument *effect) {
  BlitterPair draw;
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 i;
  u8 *sprites;
  s32 count;
  s32 frame;
  s32 k;
  s32 start;
  s32 *life;
  struct EffectStep *spark;
  struct EffectStep *sparks;

  slots = &gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];
  cursor = slots;
  EFFECT_GET_WORK_AND_CANVAS(cursor, work, canvas);
  sprites = EFFECT_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(1);
  REG_BLDALPHA = BLEND_ALPHA(16, 16); // an even mix
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_ADDITIVE);
  draw[0] = (DrawRectangle)EFFECT_SLOT(slots, HEAP_SLOT_BLITTER);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_MAX);
  draw[1] = (DrawRectangle)EFFECT_SLOT(slots, HEAP_SLOT_BLITTER_ALTERNATE);
  Resource_LoadAndDecompress((s32)&ResourceId_LightningPillarSheet, work->sheet,
                             1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sprites, 0, 0);
  if (work->effect->variant != 2) {
    void *palette = Resource_GetTableEntry((s32)&ResourceId_VioletPaletteB);
    Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  }

  // Every spark starts idle.
  life = &((struct EffectStep *)gMapCellBuffer)->variant;
  for (k = 0; k != LIGHTNING_PILLAR_SPARK_COUNT; k++) {
    *life = 0;
    life += 7;
  }
  sparks = gMapCellBuffer;
  work->transferMode = CANVAS_COPY_AND_FADE; // trails keep half a frame
  work->transferValue = 50;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);
  count = gPlasma_Counts[work->effect->variant];

  for (frame = 0; frame != count * 8 - count + 48; frame++) {
    // Variant 2 swings the camera round for the first 64 frames.
    if (work->effect->variant == 2 && frame <= 63) {
      struct CameraWork *camera = (struct CameraWork *)gCameraWork;
      s32 speed;

      if (frame <= 55)
        speed = 256;
      else
        speed = 704 - frame * 8;
      if (work->effect->side == 1)
        camera->scroll -= speed;
      else
        camera->scroll += speed;
    }
    if (frame == 32)
      BattleEventRuntime_BeginPhaseFar(SOUND_HEAVY_IMPACT);

    // Pillar i strikes on frame 8i with a flash, shows its first image the
    // frame after and its second for two frames after that.
    for (i = 0; i != count; i++) {
      start = i * PLASMA_SPACING;
      if (frame == start) {
        AudioCommand_PlayFar(SOUND_HEAVY_IMPACT);
        Iwram_FillWords(canvas, 0x4000, 0x10101010);
      }
      if (frame >= start && frame < start + 9) {
        if (frame >= start + 1 && frame < start + 2)
          draw[0](canvas, work, PLASMA_COLUMN_X(work->effect->side, i) - 24, 0,
                  48, 112);
        if (frame >= start + 2 && frame < start + 4)
          draw[0](canvas, work->sheet + PLASMA_SECOND_IMAGE,
                  PLASMA_COLUMN_X(work->effect->side, i) - 24, 0, 48, 112);

        // Throw sparks from the pillar's foot (y 104), anywhere in the
        // upper half circle, and shake the screen.
        if (frame == start + 2) {
          s32 spawned = 0;

          for (k = 0; k != LIGHTNING_PILLAR_SPARK_COUNT; k++) {
            spark = &sparks[k];
            if (spark->variant == 0) {
              s32 speed;
              s32 angle;

              speed = (Random16() & 0x3ff) + 32;
              angle = (Random16() & 0x7fff) - 0x4000;

              spark->x = PLASMA_COLUMN_X(work->effect->side, i) << 16;
              spark->y = 104 << 16;
              spark->velocityX = (Trig_Sin(angle) * speed) >> 7;
              spark->velocityY = -((Trig_Cos(angle) * speed) << 1) >> 7;
              spark->variant = (Random16() & 7) + 32;
              spawned++;
              if (spawned == PLASMA_SPARKS(work->effect->variant))
                break;
            }
          }
          work->shakeFrames = PLASMA_SHAKE_FRAMES(work->effect->variant);
        }
      }
      if (frame == start + 4) {
        EFFECT_FOR_EACH_TARGET(k, work->effect) {
          BattleMotion_ApplyVariantMotionFar(work->effect->actors[k], 1);
          ObjectGroup_UpdateMembers(work->effect->actors[k], SPRITE_COLOR_WHITE,
                                    POSE_DAMAGE, k, 8);
        }
      }
    }

    // Sparks: bounce off the ground at half speed, and shrink as their lives
    // run out; only those on the canvas are drawn.
    for (k = 0; k != LIGHTNING_PILLAR_SPARK_COUNT; k++) {
      spark = &sparks[k];
      if (spark->variant > 0) {
        spark->variant--;
        EffectStep_AdvanceWithGravity2D(spark, 60, 0x1000);
        if (spark->y > 104 << 16) {
          spark->velocityY = -spark->velocityY / 2;
        } else {
          s32 x = spark->x >> 16;
          s32 y;
          s32 size;

          if (spark->x >= 0 && x <= 119 && spark->y >= 0) {
            y = spark->y >> 16;
            size = spark->variant / 8 + 1;
            EFFECT_DRAW_MOTE(draw[k & 1], canvas, sprites, x, y, size);
          }
        }
      }
    }
    Camera_ApplyShake(8, 16);
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS();
}

// Spark bursts.
//
// A spark burst is a large flash, a ring of small flashes that fade, and a
// spray of sparks that fall and bounce on the ground. The spark groups scene
// strikes one to three such bursts, 8 frames apart, and the sword effects
// build their strikes from the same parts:
//
// - A flash is an EffectStep whose x and y are an offset from its burst's
//   centre and whose variant counts down its life. It is drawn only for its
//   last 18 frames, from a 32x64 flash picture chosen by its age.
// - A spark moves in 16.16 fixed point under its own gravity, and its variant
//   is its remaining life. Below the floor it bounces back at half speed
//   instead of being drawn; it shrinks as its life runs out.
// - A strike makes every target flash and recoil at once.
//
// The spark groups scene keeps its flashes in work->particles, sixteen per
// burst (only the first twelve are drawn), and its sparks in gMapCellBuffer,
// one run per burst.

// Abilities: Blast, Nova, Supernova, Fire Bomb, Cluster Bomb, Carpet Bomb (kind
// 15).
//
// Battle effect 15: bursts at (60, 48), the canvas behind the battle's
// first layer (priority 1).
DEFINE_EFFECT_KIND(BattleFx_RunNova, BattleFx_RunSparkGroups, 0)

// Abilities: Dynamite (kind 338).
//
// Battle effect 338: orange bursts at (60, 64), the canvas in front
// (priority 0).
DEFINE_EFFECT_KIND(BattleFx_RunDynamite, BattleFx_RunSparkGroups, 1)

// gSparkGroups_Shapes has five entries for each variant.
#define SPARK_GROUPS_SPARKS(variant) gSparkGroups_Shapes[(variant) * 5]
#define SPARK_GROUPS_BURSTS(variant) gSparkGroups_Shapes[(variant) * 5 + 1]
#define SPARK_GROUPS_OFFSET(variant, burst)                                    \
  gSparkGroups_Shapes[(variant) * 5 + (burst) + 2]

#define SPARK_GROUPS_FLASHES 16 // flashes kept per burst
#define SPARK_GROUPS_SPACING 8  // frames between bursts
#define SPARK_GROUPS_RING 12    // of which drawn as the ring
#define SPARK_FLASH_DRAWN 18    // a flash is drawn for its last 18 frames

// One to three bursts, 8 frames apart: each flashes, rings itself with
// twelve smaller flashes that fade, and throws out sparks that bounce on
// the ground; every burst knocks the targets back.
// effect->variant picks the shape from gSparkGroups_Shapes: sparks per
// burst, how many bursts, and each burst's offset across. Variant 2 also
// scrolls the camera, and variant 3 flashes the whole canvas on frame 4.
// kind : 0 at (60, 48), the battle shows the damage on frame 24;
//        1 at (60, 64), orange; the damage shows on frame 2;
//        2 below the user, in the LightningBolt colours, with the sparks
//          rising instead of falling; the damage shows on frame 2.
void BattleFx_RunSparkGroups(struct BattleEffectArgument *effect, s32 kind) {
  struct EffectPosition screenPosition;
  BlitterPair draw;
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 i;
  s32 frame;
  u8 *graphics;
  s32 originX;
  s32 originY;
  s32 j;
  s32 start;
  struct EffectStep *point;
  struct EffectStep *spark;
  struct EffectStep *sparks;

  slots = gWorkSlot + HEAP_SLOT_BATTLE_EFFECT;
  cursor = slots;
  EFFECT_GET_WORK_AND_CANVAS(cursor, work, canvas);
  graphics = EFFECT_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  work->effect = effect;
  if (kind == 0) {
    BattleFx_BeginCanvasLayer(1);
    originX = 60;
    originY = 48;
  } else if (kind == 1) {
    BattleFx_BeginCanvasLayer(0);
    originX = 60;
    originY = 64;
  } else {
    BattleFx_BeginCanvasLayer(0);
    EffectPosition_ApplyStepAndYOffset(work->effect->actor, &screenPosition);
    originX = screenPosition.x / 2;
    originY = screenPosition.y + 48;
  }
  REG_BLDALPHA = BLEND_ALPHA(16, 16); // an even mix
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS, COMPOSITE_MAX);
  draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_ADDITIVE);
  draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, graphics, 0, 0);
  if (kind == 1) {
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB),
                    128);
  } else if (kind == 2) {
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_LightningBoltSheet),
                    128);
  }
  sparks = gMapCellBuffer;

  // For each burst: sixteen ring flashes (the first twelve are drawn) at
  // random angles, further out and shorter-lived the later they come, and
  // its sparks, thrown in every direction, mostly up.
  for (i = 0; i != SPARK_GROUPS_BURSTS(work->effect->variant); i++) {
    for (j = 0; j != SPARK_GROUPS_FLASHES; j++) {
      s32 radius;
      s32 angle;

      point = &work->particles[i * SPARK_GROUPS_FLASHES + j];
      radius = j * 2;
      angle = Random16() & 0xffff;
      point->x = Trig_Sin(angle) * radius;
      point->y = -(Trig_Cos(angle) * radius);
      point->variant = j / 2 + 25;
    }
    for (j = 0; j != SPARK_GROUPS_SPARKS(work->effect->variant); j++) {
      s32 speed;
      s32 angle;

      spark = &sparks[i * SPARK_GROUPS_SPARKS(work->effect->variant)] + j;
      speed = (Random16() & 0x3ff) + 32;
      angle = Random16() & 0xffff;
      if (work->effect->side == 1)
        spark->x =
            (originX - SPARK_GROUPS_OFFSET(work->effect->variant, i) + 28)
            << 16;
      else
        spark->x =
            (originX + SPARK_GROUPS_OFFSET(work->effect->variant, i) - 28)
            << 16;
      spark->y = originY << 16;
      spark->velocityX = (Trig_Sin(angle) * speed) >> 6;
      spark->velocityY = -((Trig_Cos(angle) * speed) << 1) >> 6;
      spark->variant = (Random16() & 7) + 32;
    }
  }
  work->transferMode = CANVAS_COPY_AND_FADE; // trails keep 3/4 a frame
  work->transferValue = 75;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);

  for (frame = 0;
       frame !=
       SPARK_GROUPS_BURSTS(work->effect->variant) * SPARK_GROUPS_SPACING + 56;
       frame++) {
    struct CameraWork *camera =
        *(struct CameraWork **)(gWorkSlot + HEAP_SLOT_CAMERA);

    if (work->effect->variant == 2 && frame <= 51) {
      if (work->effect->side == 0)
        camera->scroll += 256;
      else
        camera->scroll -= 256;
    }
    /* FAKEMATCH: needed to match: this fill calls the fixed entry itself;
     * through Iwram_FillWords a spilled invariant ahead of the frame loop is
     * built in r2 where the ROM uses r1 (3 of 776 instructions differ). */
    if (work->effect->variant == 3 && frame == 4)
      Iwram_FillWordsEntry(canvas, 0x4000, 0x3f3f3f3f);
    if (kind == 1 || kind == 2) {
      if (frame == 2)
        BattleEventRuntime_BeginPhaseFar(145);
    } else {
      if (frame == 2)
        AudioCommand_PlayFar(145);
      if (frame == 24)
        BattleEventRuntime_BeginPhaseFar(SOUND_HEAVY_IMPACT);
    }

    // Burst i starts on frame 8i with a two-frame flash, then its ring and
    // sparks; the targets reel 6 frames in.
    for (i = 0; i != SPARK_GROUPS_BURSTS(work->effect->variant); i++) {
      start = i * SPARK_GROUPS_SPACING;
      if (frame == start)
        work->shakeFrames = 12;
      if (frame >= start && frame < start + 2) {
        if (work->effect->side == 1)
          draw[0](canvas, work,
                  originX - SPARK_GROUPS_OFFSET(work->effect->variant, i) + 12,
                  originY - 32, 32, 64);
        else
          draw[0](canvas, work,
                  originX + SPARK_GROUPS_OFFSET(work->effect->variant, i) - 44,
                  originY - 32, 32, 64);
      }
      if (frame >= start) {
        for (j = 0; j != SPARK_GROUPS_RING; j++) {
          s32 x;
          s32 y;

          point = &work->particles[i * SPARK_GROUPS_FLASHES] + j;
          y = HI(point->y) + originY;
          if (work->effect->side == 1)
            x = HI(point->x) + originX -
                SPARK_GROUPS_OFFSET(work->effect->variant, i) + 28;
          else
            x = HI(point->x) + originX +
                SPARK_GROUPS_OFFSET(work->effect->variant, i) - 28;
          if (point->variant >= 0 && point->variant < SPARK_FLASH_DRAWN)
            draw[0](canvas,
                    work->sheet +
                        (gSparkGroups_FlashCells[point->variant / 3] << 11),
                    x - 16, y - 32, 32, 64);
          if (point->variant > 0)
            point->variant--;
          else
            point->variant = -1;
        }
      }
      if (frame > start + 5) {
        s32 gravity;

        if (kind == 2)
          gravity = -0x1000;
        else
          gravity = 0x1000;
        for (j = 0; j != SPARK_GROUPS_SPARKS(work->effect->variant); j++) {
          spark = &sparks[i * SPARK_GROUPS_SPARKS(work->effect->variant)] + j;
          if (spark->variant > 0) {
            EffectStep_AdvanceWithGravity2D(spark, 60, gravity);
            spark->variant--;
            if (spark->y > 0x6c0000) {
              spark->velocityY = -spark->velocityY / 2;
            } else if (spark->x >= 0 && spark->x < 0x7f0000 && spark->y >= 0) {
              s32 y = spark->y >> 16;
              s32 x = spark->x >> 16;
              s32 size = spark->variant / 5 + 1;

              EFFECT_DRAW_MOTE(draw[j & 1], canvas, graphics, x, y, size);
            }
          }
        }
      }
      EFFECT_FOR_EACH_TARGET(j, work->effect) {
        if (frame == start + 6) {
          ObjectGroup_UpdateMembers(work->effect->actors[j], SPRITE_COLOR_WHITE,
                                    POSE_DAMAGE, j, 10);
          BattleMotion_ApplyVariantMotionFar(work->effect->actors[j], 4);
        }
      }
    }
    Camera_ApplyShake(16, 16);
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS();
}

#define VOLCANO_FRAMES 96               // frames in the effect
#define VOLCANO_ERUPTION 200            // sparks thrown as a pillar starts
#define VOLCANO_TRICKLE 4               // and each frame after
#define VOLCANO_SPARK_POOL 1024         // sparks in PARTICLES
#define VOLCANO_FLAME_IMAGE_ODD 0xdd0   // the flame on odd frames
#define VOLCANO_FLAME_IMAGE_EVEN 0x10b4 // the flame on even frames

// Abilities: Volcano, Eruption, Pyroclasm, Lava Shower, Molten Bath, Magma
// Storm (kind 13).
//
// Battle effect 13: one, two or four columns of fire (gVolcano_Counts by
// effect->variant) rise near the far edge on the targets' side, at the
// depths of gVolcano_Depths, starting at gVolcano_StartFrames; each
// erupts with a fountain of 200 sparks and keeps spitting more, and each
// eruption knocks the targets back. The battle shows the damage on frame
// 16. Variant 0 is red, 2 orange; variant 2 also swings the camera round.
void BattleFx_RunVolcano(struct BattleEffectArgument *effect) {
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 pillar;
  u8 *sheet;
  struct EffectPosition screen;
  BlitterPair draw;
  struct MotionObject *object;
  struct BattleCamera *camera;
  s32 i;

  slots = &gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];
  cursor = slots;
  EFFECT_GET_WORK_AND_CANVAS(cursor, work, canvas);
  sheet = EFFECT_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(1);
  REG_BLDALPHA = BLEND_ALPHA(16, 16); // an even mix
  Resource_LoadAndDecompress((s32)&ResourceId_FirePillarSheetA, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  if (work->effect->variant == 0)
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_RedPaletteB), 128);
  else if (work->effect->variant == 2)
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB),
                    128);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS, COMPOSITE_MAX);
  draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_MAX);
  draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];

  for (i = 0; i != VOLCANO_SPARK_POOL; i++)
    PARTICLES[i].variant = 0;

  // The pillars stand 72 to 87 pixels out on the first target's side.
  object = GetBattleObjectSlotFar(work->effect->actors[0])->object;
  for (i = 0; i != 4; i++) {
    struct EffectStep *base = &work->particles[i];
    s32 x;

    x = ((Random16() & 15) + 72) << 16;
    base->y = 0;
    base->x = x;
    base->z = gVolcano_Depths[work->effect->variant][i] << 16;
    if (object->x < 0)
      base->x = -x;
  }

  work->transferMode = CANVAS_COPY_AND_FADE; // trails keep half a frame
  work->transferValue = 50;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);

  for (frame = 0; frame != VOLCANO_FRAMES; frame++) {
    camera = gCameraWork;
    if (work->effect->variant == 2 && frame < 64) {
      if (work->effect->side == 0)
        camera->yaw = camera->yaw + 192;
      else
        camera->yaw = (s16)camera->yaw - 192;
    }
    if (frame == 16)
      BattleEventRuntime_BeginPhaseFar(SOUND_HEAVY_IMPACT);
    EFFECT_VIEW_FROM_CAMERA(camera);

    if (frame < 64) {
      for (pillar = 0; pillar != gVolcano_Counts[work->effect->variant];
           pillar++) {
        EffectPosition_ApplyBaseAndYOffset(&work->particles[pillar], &screen);
        screen.x /= 2;
        screen.y -= 8;
        if (frame == gVolcano_StartFrames[pillar])
          AudioCommand_PlayFar(145);
        // From 4 frames after it starts: the column, scrolling up 16
        // pixels a frame, and a flickering flame at its foot.
        if (frame >= gVolcano_StartFrames[pillar] + 4) {
          s32 scroll = (frame * 16 + pillar * 25) % 104;

          draw[pillar & 1](canvas, work, screen.x - 17, screen.y - scroll - 104,
                           34, 104);
          draw[pillar & 1](canvas, work, screen.x - 17, screen.y - scroll, 34,
                           scroll);
          if (frame & 1) {
            draw[0](canvas, work->sheet + VOLCANO_FLAME_IMAGE_ODD,
                    screen.x - 20, screen.y - 24, 20, 37);
            draw[1](canvas, work->sheet + VOLCANO_FLAME_IMAGE_ODD, screen.x,
                    screen.y - 24, 20, 37);
          } else {
            draw[0](canvas, work->sheet + VOLCANO_FLAME_IMAGE_EVEN,
                    screen.x - 20, screen.y - 24, 20, 37);
            draw[1](canvas, work->sheet + VOLCANO_FLAME_IMAGE_EVEN, screen.x,
                    screen.y - 24, 20, 37);
          }
        }
        // Sparks: 200 at the start, then 4 a frame from 16 frames on.
        if (frame == gVolcano_StartFrames[pillar] ||
            frame >= gVolcano_StartFrames[pillar] + 16) {
          s32 spawned = 0;

          for (i = 0; i != VOLCANO_SPARK_POOL; i++) {
            struct EffectStep *spark = &PARTICLES[i];

            if (spark->variant == 0) {
              s32 speed;
              s32 angle;

              speed = (Random16() & 0x3ff) + 32;
              angle = (Random16() & 0x7fff) - 0x4000;
              EFFECT_SET_POSITION(spark, screen.x << 8,
                                  (screen.y << 8) + 0x1000,
                                  (Trig_Sin(angle) * speed) >> 15);
              spark->velocityY = -(Trig_Cos(angle) * speed * 2) >> 15;
              spawned++;
              if (frame == gVolcano_StartFrames[pillar]) {
                spark->variant = (Random16() & 7) + 48;
                if (spawned == VOLCANO_ERUPTION)
                  break;
              } else {
                spark->variant = (Random16() & 7) + 24;
                if (spawned == VOLCANO_TRICKLE)
                  break;
              }
            }
          }
        }
        if (frame == gVolcano_StartFrames[pillar]) {
          work->shakeFrames = 2;
          EFFECT_FOR_EACH_TARGET(i, work->effect) {
            ObjectGroup_UpdateMembers(work->effect->actors[i],
                                      SPRITE_COLOR_RED_TINT, POSE_DAMAGE, i, 8);
            BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 1);
          }
        }
      }
    }

    // The sparks, in 24.8 fixed point (z holds the speed across): slow to
    // 60/64 a frame, fall, and bounce off y 120 at half speed.
    for (i = 0; i != VOLCANO_SPARK_POOL; i++) {
      struct EffectStep *spark = &PARTICLES[i];
      s32 life = spark->variant;

      if (life > 0) {
        s32 x;
        s32 y;
        s32 py;

        spark->variant = life - 1;
        x = spark->x += spark->z;
        y = spark->y += spark->velocityY;
        spark->z = spark->z * 60 / 64;
        spark->velocityY = spark->velocityY * 60 / 64 - 16;
        py = y / 256;
        if (py > 120) {
          spark->velocityY = -spark->velocityY / 2;
        } else if (x >= 0 && x >> 8 <= 126 && y >= 0) {
          s32 px = x >> 8;
          s32 size = (life - 17) / 8;

          if (size <= 0)
            size = 1;
          EFFECT_DRAW_MOTE(draw[i & 1], canvas, sheet, px, py, size);
        }
      }
    }
    Camera_ApplyShake(16, 16);
    EFFECT_END_FRAME(work);
  }

  EFFECT_RELEASE_BLITTERS();
  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  BattleFx_EndCanvasLayer();
}

// Smoke, glints and status pictures centered on battle targets.
// Mote modes choose how the particles gather or spread around each target.

// Swirling motes around each target, in eight styles.
//
// Around each target in turn, 8 frames apart, motes turn about a point 20
// pixels up, either closing in on it or rising, while the effect layer
// ripples line by line and the target flashes. Each mote cycles through six
// images, 4 frames each, and then starts again: a closing mote where it is, a
// rising one at a new random point below.
//
// The motes live in PARTICLES, 64 slots for each target, of which the first
// 16, 24 or 32 are drawn; their variant counts the frames of the current
// image cycle. The bouncing sheep (Sleep) and the target markers (Curse) that
// follow keep theirs in work->particles.

// Abilities: Taint, Poison (kind 32).
//
// Battle effect 32: lime smoke (effect->variant 0) or pink smoke.
void BattleFx_RunTaint(struct BattleEffectArgument *effect) {
  if (effect->variant == 0) {
    BattleFx_RunTargetMotes(effect, 0);
    return;
  }
  BattleFx_RunTargetMotes(effect, 1);
}

// Pink smoke; no ☀️ effect table entry points here.
UNUSED void BattleFx_RunTargetMotesMode1(struct BattleEffectArgument *effect) {
  BattleFx_RunTargetMotes(effect, 1);
}

// Abilities: Delude, Confuse, Mist (kind 33).
//
// Battle effect 33: cyan smoke that rises.
DEFINE_EFFECT_KIND(BattleFx_RunDelude, BattleFx_RunTargetMotes, 2)

// Abilities: Paralyze (kind 36).
//
// Battle effect 36: rising smoke with glints.
DEFINE_EFFECT_KIND(BattleFx_RunParalyze, BattleFx_RunTargetMotes, 3)

// Abilities: Heartrender (kind 346); kind 35 is used by no ability.
//
// Battle effects 35 and 346: hearts.
DEFINE_EFFECT_KIND(BattleFx_RunHeartrender, BattleFx_RunTargetMotes, 4)

// Abilities: Mystic Flame (kind 312).
//
// Battle effect 312: glowing orbs.
DEFINE_EFFECT_KIND(BattleFx_RunMysticFlame, BattleFx_RunTargetMotes, 5)

// Abilities: Demon Fire (kind 64).
//
// Battle effect 64: glowing orbs in Mars Djinn colours, and the user
// strikes.
DEFINE_EFFECT_KIND(BattleFx_RunDemonFire, BattleFx_RunTargetMotes, 7)

// Abilities: Drown (kind 66).
//
// Battle effect 66: vortex motes.
DEFINE_EFFECT_KIND(BattleFx_RunDrown, BattleFx_RunTargetMotes, 6)

// Styles whose motes start in a wide ball and last 64 frames more.
#define TARGET_MOTES_START_WIDE(mode)                                          \
  ((mode) <= 1 || (mode) == 4 || (mode) == 5 || (mode) == 6 || (mode) == 7)
// Styles whose motes close in on the target; the others rise.
#define TARGET_MOTES_CLOSE_IN(mode)                                            \
  ((mode) <= 1 || (mode) == 4 || (mode) == 5 || (mode) == 6)

// Where mode 3 keeps its glint images in work->sheet.
enum { TARGET_MOTE_GLINT_CELLS = 0x2580 };

#define TARGET_MOTES 512     // motes scattered at the start
#define TARGET_MOTE_SLOTS 64 // particles reserved for each target
#define TARGET_MOTE_CYCLE 24 // frames of a mote's image cycle

// Around each target in turn, 8 frames apart, motes (smoke, hearts, orbs)
// turn about a point 20 pixels up, either closing in on it or rising, and
// the target flashes until the end; the battle shows the damage 32 frames
// before the end. The effect layer ripples, except in modes 0, 1 and 3.
// mode : 0 lime smoke, 1 pink smoke, 2 cyan smoke, 3 smoke with glints,
//        4 hearts, 5 glowing orbs, 6 vortex motes, 7 orbs in Mars Djinn
//        colours, with the user stepping up to strike the first target.
void BattleFx_RunTargetMotes(struct BattleEffectArgument *effect, u32 mode) {
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 total;
  s32 member;
  s32 count;
  struct EffectPosition screen;
  s32 point[3];
  struct EffectPosition base;
  BlitterPair draw;
  void *palette;
  s32 *row;
  s32 i;

  cursor = gBattleFxWork;
  EFFECT_GET_WORK_AND_CANVAS(cursor, work, canvas);
  count = 16;
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);
  Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet, work->sheet, 1, 1);
  if (mode == 0) {
    palette = Resource_GetTableEntry((s32)&ResourceId_LimePalette);
  } else if (mode == 1) {
    palette = Resource_GetTableEntry((s32)&ResourceId_PinkBurstSheet);
  } else if (mode == 2) {
    palette = Resource_GetTableEntry((s32)&ResourceId_CyanPalette);
  } else if (mode == 3) {
    palette = Resource_GetTableEntry((s32)&ResourceId_BlastSheet);
  } else if (mode == 4) {
    palette = Resource_GetTableEntry((s32)&ResourceId_PinkBurstSheet);
  } else if (mode == 5) {
    Resource_LoadAndDecompress((s32)&ResourceId_GlowOrbSheet, work->sheet, 1,
                               0);
    palette = Resource_GetTableEntry((s32)&ResourceId_GlowOrbSheet);
  } else if (mode == 7) {
    count = 24;
    Resource_LoadAndDecompress((s32)&ResourceId_GlowOrbSheet, work->sheet, 1,
                               0);
    palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
  } else {
    count = 32;
    Resource_LoadAndDecompress((s32)&ResourceId_VortexSheet, work->sheet, 1, 0);
    palette = Resource_GetTableEntry((s32)&ResourceId_VortexSheet);
  }
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  if (mode == 4)
    Resource_LoadAndDecompress((s32)&ResourceId_HeartSheet, work->sheet, 1, 1);
  if (mode == 3)
    Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet,
                               work->sheet + TARGET_MOTE_GLINT_CELLS, 1, 0);

  for (i = 0; i != TARGET_MOTES; i++) {
    struct EffectStep *mote = &PARTICLES[i];

    if (TARGET_MOTES_START_WIDE(mode)) {
      EFFECT_SET_POSITION(mote, ((Random16() & 0xff) - 127) << 15,
                          ((Random16() & 0xff) - 127) << 14,
                          ((Random16() & 0xff) - 127) << 15);
    } else {
      EFFECT_SET_POSITION(mote, ((Random16() & 0xff) - 127) << 13,
                          ((Random16() & 0xff) - 255) << 13,
                          ((Random16() & 0xff) - 127) << 13);
    }
    mote->variant = 0;
  }

  if (TARGET_MOTES_START_WIDE(mode))
    total = work->effect->count * 8 + 64;
  else
    total = work->effect->count * 8 + 32;
  if (!(mode <= 1 || mode == 3))
    Scheduler_AddOrUpdateCallback(BattleFx_ArmBg2AffineHBlankDma, 0x480);
  work->transferMode = CANVAS_COPY_AND_FADE; // trails keep 3/4 a frame
  work->transferValue = 75;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);
  AudioCommand_PlayFar(142);

  for (frame = 0; frame != total; frame++) {
    struct BattleCamera *camera = gCameraWork;

    if (mode == 7) {
      if (frame == total - 46)
        BattleMotion_ApproachTargetFar(work->effect->actor,
                                       work->effect->actors[0], 16, 0);
      if (frame == total - 32) {
        BattleEventRuntime_BeginPhaseFar(SOUND_HEAVY_IMPACT);
        BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 4);
        work->shakeFrames = 8;
      }
    } else if (frame == total - 32) {
      BattleEventRuntime_BeginPhaseFar(133);
    }
    // Ripple the canvas sideways, line by line.
    row = work->bg2X;
    for (i = 0; i != 160; i++)
      *row++ = (0x40000 - ((Trig_Sin((frame << 12) + i * 0x800) * 4))) >> 10;

    EFFECT_FOR_EACH_TARGET(member, work->effect) {
      struct MotionObject *object =
          GetBattleObjectSlotFar(work->effect->actors[member])->object;

      // Mode 3: two glints a frame, scattered round the target.
      if (mode == 3 && frame > member * 8 && frame < member * 8 + 32) {
        EffectPosition_ApplyStepAndYOffset(work->effect->actors[member], &base);
        for (i = 0; i != 2; i++) {
          s32 angle;
          s32 radius;
          s32 x;
          s32 y;

          angle = Random16() & 0xffff;
          radius = (Random16() & 31) + 4;
          x = base.x / 2 + ((Trig_Sin(angle) * radius) >> 17) -
              (gBattleFx_GlintCellWidths[frame & 3] >> 1);
          y = base.y - ((Trig_Cos(angle) * radius) >> 16) -
              (gBattleFx_GlintCellHeights[frame & 3] >> 1);
          EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                              EFFECT_CLIP_BOUNDS |
                                  gRenderMode_GlintFlips[Random16() & 3],
                              COMPOSITE_MAX);
          ((DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE])(
              canvas,
              work->sheet + gBattleFx_GlintCellOffsets[frame & 3] +
                  TARGET_MOTE_GLINT_CELLS,
              x, y + 16, gBattleFx_GlintCellWidths[frame & 3],
              gBattleFx_GlintCellHeights[frame & 3]);
          Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
        }
      }
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS,
                          COMPOSITE_ADDITIVE);
      draw[0] = gWorkSlot[HEAP_SLOT_BLITTER];
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE, EFFECT_CLIP_BOUNDS,
                          COMPOSITE_MAX);
      draw[1] = gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
      EFFECT_VIEW_FROM_CAMERA(camera);
      EFFECT_SET_VECTOR(point, object->x, 160 << 13, object->z);
      SceneTransform_ApplyPosition(point);
      // Turn the group; mote i joins i frames after the target's turn
      // starts and takes a new image every 4 frames.
      if (frame > member * 8) {
        SceneTransform_ApplyYaw(frame << 9);
        if (mode <= 1 || mode == 4)
          SceneTransform_ApplyPitch(frame << 9);
        for (i = 0; i != count; i++) {
          struct EffectStep *mote = &PARTICLES[member * TARGET_MOTE_SLOTS + i];
          s32 distance;

          if (frame > member * 8 + i) {
            s32 xx = (mote->x >> 8) * (mote->x >> 8);
            s32 yy = (mote->y >> 8) * (mote->y >> 8);
            s32 zz = (mote->z >> 8) * (mote->z >> 8);

            distance = Iwram_Sqrt(xx + yy + zz) >> 9;
            if (distance != 0 && mote->variant <= 23) {
              s32 cel = mote->variant / 4;

              EffectPosition_ApplyBaseAndYOffset(mote, &screen);
              screen.x >>= 1;
              if (mode == 5 || mode == 7)
                draw[1](canvas, work->sheet + cel * 1600, screen.x - 20,
                        screen.y - 20, 40, 40);
              else if (mode == 6)
                draw[1](canvas, work->sheet + 0xc00, screen.x - 6,
                        screen.y - 12, 12, 24);
              else if (mode == 4)
                draw[1](canvas, work->sheet, screen.x - 11, screen.y - 21, 22,
                        42);
              else
                draw[(i & 3) != 0](canvas, work->sheet + cel * 1152,
                                   screen.x - 12, screen.y - 24, 24, 48);
              if (TARGET_MOTES_CLOSE_IN(mode)) {
                mote->x -= mote->x / distance;
                mote->y -= mote->y / distance;
                mote->z -= mote->z / distance;
              } else {
                mote->y += 0x10000;
              }
              mote->variant++;
              if (mote->variant == TARGET_MOTE_CYCLE) {
                if (TARGET_MOTES_CLOSE_IN(mode)) {
                  mote->variant = 0;
                } else {
                  EFFECT_SET_POSITION(mote, ((Random16() & 0xff) - 127) << 13,
                                      ((Random16() & 0xff) - 255) << 12,
                                      ((Random16() & 0xff) - 127) << 13);
                }
              }
            }
          }
        }
      }
      EFFECT_RELEASE_BLITTERS();
      if (frame == member * 8 + 16) {
        s32 delay = total - frame;

        if (delay > 31)
          delay = 31;
        ObjectGroup_UpdateMembers(work->effect->actors[member],
                                  SPRITE_COLOR_WHITE, POSE_DAMAGE, member,
                                  delay);
      }
    }
    Camera_ApplyShake(16, 16);
    EFFECT_END_FRAME(work);
  }

  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  if (!(mode <= 1 || mode == 3))
    Scheduler_RemoveCallback(BattleFx_ArmBg2AffineHBlankDma);
  BattleFx_EndCanvasLayer();
}

#define SLEEP_FRAMES 148 // frames in the effect
#define SLEEP_SEEDED 32  // particles cleared at the start
#define SLEEP_SHEEP 12   // sheep dropped

// Abilities: Sleep (kind 37).
//
// Battle effect 37: twelve sheep drop onto the targets' side, one every 4
// frames, tumbling through eight images (the last four mirrored both
// ways); each target takes its damage pose in turn from frame 64, and the
// battle shows the result on frame 80. 148 frames.
void BattleFx_RunSleep(struct BattleEffectArgument *effect) {
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 anchor[3];
  struct EffectStep *current;
  struct EffectStep *sheep;
  s32 particleIndex;
  s32 frame;
  DrawRectangle drawFlipped;
  DrawRectangle drawUpright;
  s32 targetIndex;

  sheep = PARTICLES;
  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);
  REG_BG2PA = Q_8_8(1);
  REG_BLDCNT = 0;    // drawn solid
  Resource_LoadAndDecompress((s32)&ResourceId_SheepSheet, work->sheet, 1, 1);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_TRANSPARENT);
  drawUpright = (DrawRectangle)EFFECT_SLOT(slots, HEAP_SLOT_BLITTER);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP,
                      COMPOSITE_TRANSPARENT);
  drawFlipped = (DrawRectangle)EFFECT_SLOT(slots, HEAP_SLOT_BLITTER_ALTERNATE);

  // Each sheep starts above the canvas at a random x, with a random tumble
  // speed (z) and starting image.
  for (particleIndex = 0; particleIndex != SLEEP_SEEDED; particleIndex++) {
    current = &sheep[particleIndex];
    current->x = ((Random16() & 63) + 32) << 16;
    current->y = -0x200000;
    current->velocityY = Random16() & 0;
    current->z = Random16() & 3;
    current->variant = Random16() & 255;
  }
  if (work->effect->side == 1)
    REG_BG2X = -0x7000;                       // over the party's side
  work->transferMode = CANVAS_COPY_AND_CLEAR; // redraw from blank each frame
  work->transferValue = 0;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);
  AudioCommand_PlayFar(142);
  for (frame = 0; frame != SLEEP_FRAMES; frame++) {
    struct BattleCamera *camera;

    camera = gCameraWork;
    if (frame == 80)
      BattleEventRuntime_BeginPhaseFar(0);
    EFFECT_FOR_EACH_TARGET(targetIndex, work->effect) {
      struct MotionObject *target;

      target =
          GetBattleObjectSlotFar(work->effect->actors[targetIndex])->object;
      EFFECT_VIEW_FROM_CAMERA(camera);
      EFFECT_SET_VECTOR(anchor, target->x, 160 << 14, target->z);
      SceneTransform_ApplyPosition(anchor);
      if (frame == targetIndex * 16 + 64)
        ObjectGroup_UpdateMembers(work->effect->actors[targetIndex], 0,
                                  POSE_DAMAGE, -1, 0);
    }
    current = sheep;
    for (particleIndex = 0; particleIndex != SLEEP_SHEEP;
         particleIndex++, current++) {
      if (frame > particleIndex * 4 && current->y <= 0x7fffff) {
        s32 cel;

        cel = (current->variant / 16) & 7;
        if (cel < 4)
          drawUpright(canvas, work->sheet + (cel << 10),
                      (current->x >> 16) - 16, (current->y >> 16) - 16, 32, 32);
        else
          drawFlipped(canvas, work->sheet + (cel << 10) - 0x1000,
                      (current->x >> 16) - 16, (current->y >> 16) - 16, 32, 32);
        current->y += current->velocityY;
        current->velocityY += 0x2000;
        current->variant += current->z;
        // Meant to bounce a sheep off the ground (y 92), tumbling faster;
        // but velocityY has just grown, so it is never 0 here and the
        // sheep fall on through.
        if (current->y > 0x5c0000 && current->velocityY == 0) {
          s32 speed = current->velocityY + 1;
          current->y = 0x5c0000;
          current->z += 4;
          current->velocityY = -speed / 2;
        }
      }
    }
    EFFECT_PRESENT_FRAME(work);
  }
  EFFECT_FINISH_CANVAS();
}

#define CURSE_MARKER_CELLS 1728 // the marker picture in the sheet

// Abilities: Curse (kind 41); Lucid Prophecy (kind 354).
//
// Battle effects 41 and 354: a small flame burns above each target in
// turn, 16 frames apart, for 72 frames each, over a marker at its feet;
// the battle shows the result on frame 32.
void BattleFx_RunCurse(struct BattleEffectArgument *effect) {
  void **slot;
  struct BattleEffectWork *work;
  void *canvas;
  /* FAKEMATCH: needed to match: the family's blitter pair: this effect loads
   * only one. A single blitter, as draw[1] or a plain pointer: differ 202. */
  BlitterPair draw;
  struct EffectStep *marker;
  s32 i;
  s32 frame;
  s32 start;
  s32 cel;
  s32 y;
  struct EffectPosition screenPosition;

  slot = &gWorkSlot[HEAP_SLOT_BATTLE_CANVAS];
  canvas = slot[0];
  work = slot[-1];
  work->effect = effect;
  BattleFx_BeginCanvasLayer(1);
  REG_BG2PA = Q_8_8(1);
  REG_BLDCNT = 0;    // drawn solid
  Resource_LoadAndDecompress((s32)&ResourceId_FlameColumnSheet, work->sheet, 1,
                             1);
  if (work->effect->side == 1) {
    REG_BG2X = -0x7000; // over the party's side
  }
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_TRANSPARENT);
  work->transferMode = CANVAS_COPY_AND_CLEAR; // redraw from blank each frame
  work->transferValue = 0;
  draw[0] = (DrawRectangle)CANVAS_SLOT(slot, HEAP_SLOT_BLITTER);
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);
  // Each flame starts at a random point of its animation.
  EFFECT_FOR_EACH_TARGET(i, work->effect) {
    work->particles[i].variant = Random16() & 63;
  }
  frame = 0;
  while (frame != (work->effect->count << 5) + 32) {
    if (frame == 32) {
      BattleEventRuntime_BeginPhaseFar(0);
    }
    i = 0;
    if (work->effect->count != 0) {
      marker = work->particles;
      do {
        start = i << 4;
        if (frame == start) {
          AudioCommand_PlayFar(143);
        }
        if (frame >= start && frame < start + 72) {
          EffectPosition_ApplyStepAndYOffset(work->effect->actors[i],
                                             &screenPosition);
          if (work->effect->side == 1) {
            screenPosition.x -= 112;
          }
          y = screenPosition.y;
          screenPosition.y = y - 16;
          // The marker (16x20), then the flame (16x12) above it, cycling
          // through nine images six frames each.
          draw[0](canvas, work->sheet + CURSE_MARKER_CELLS,
                  screenPosition.x - 8, y - 20, 16, 20);
          if (frame >= start) {
            cel = ((frame - start) + marker->variant) / 6 % 9;
            draw[0](canvas, work->sheet + cel * 192, screenPosition.x - 8,
                    screenPosition.y - 16, 16, 12);
          }
        }
        marker++;
        i++;
      } while (i != work->effect->count);
    }
    EFFECT_PRESENT_FRAME(work);
    frame += 1;
  }
  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

#define DJINN_HEAL_SWIRL_FRAMES 98 // frames of the swirl
#define DJINN_HEAL_SWIRL_MOTES 32  // motes swirled in

// A small Mercury Djinni healing the party: the user glows while motes
// swirl round a point that rises from the user up to above the first
// target (frames 30 to 69); the Djinni appears there on frame 83. Then,
// for each target in turn, 8 frames apart, sparkles spiral in on it with
// the recovery sound and a glow, while the Djinni sways and finally flies
// off. No effect table entry points here.
UNUSED void BattleFx_RunDjinnHeal(struct BattleEffectArgument *effect) {
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 member;
  u8 *sheet;
  DrawRectangle *draw;
  struct MotionObject *actor;
  struct MotionObject *goal;
  s32 frame;
  s32 i;
  s32 position[3];
  s32 target[3];
  s32 delta[3];
  s32 djinniPoint[3];
  struct EffectPosition screen;
  s32 point[3];
  struct EffectPosition view;
  BlitterPair callbacks;

  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  sheet = EFFECT_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);
  Resource_LoadAndDecompress((s32)&ResourceId_MercuryDjinnSmallSheet,
                             work->sheet, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  BattleFx_FetchRectangleBlitters(work->effect->side ^ 1, draw = callbacks);
  actor = GetBattleObjectSlotFar(work->effect->actor)->object;
  goal = GetBattleObjectSlotFar(work->effect->actors[0])->object;
  for (i = 0; i != 64; i++) {
    struct EffectStep *spark = &PARTICLES[i];
    s32 heading = Random16() & 0xffff;
    s32 speed = (Random16() & 0xff) + 128;

    EFFECT_SET_POSITION(spark, 0, ((Random16() & 31) + 20) << 16, 0);
    EFFECT_SET_VELOCITY(spark, (Trig_Sin(heading) * speed) >> 5, 0,
                        (Trig_Cos(heading) * speed) >> 5);
    spark->variant = 0;
  }
  work->transferMode = CANVAS_COPY_AND_FADE; // trails keep 3/4 a frame
  work->transferValue = 75;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);

  // The swirl travels from the user's feet to 90 pixels above the first
  // target over frames 30 to 69.
  EFFECT_SET_VECTOR(position, actor->x, 0, actor->z);
  EFFECT_SET_VECTOR(target, goal->x, 180 << 15, 0);
  EFFECT_SET_VECTOR(delta, (target[0] - position[0]) / 40,
                    (target[1] - position[1]) / 40,
                    (target[2] - position[2]) / 40);
  for (frame = 0; frame != DJINN_HEAL_SWIRL_FRAMES; frame++) {
    struct BattleCamera *camera = gCameraWork;
    if (frame == 8)
      AudioCommand_PlayFar(212);
    if (frame == 80)
      AudioCommand_PlayFar(142);
    EFFECT_VIEW_FROM_CAMERA(camera);
    if (frame >= 30 && frame < 70) {
      position[0] += delta[0];
      position[1] += delta[1];
      position[2] += delta[2];
    }
    SceneTransform_ApplyPosition(position);
    if (frame == 0)
      ObjectGroup_UpdateMembers(work->effect->actor, SPRITE_COLOR_WHITE, -1, -1,
                                0);
    if (frame == 24)
      ObjectGroup_UpdateMembers(work->effect->actor, 0, -1, -1, 0);
    // The swirl: 32 motes, each turning about its own axis and, from 30
    // frames after it appears, pulled back into the centre.
    for (i = 0; i != DJINN_HEAL_SWIRL_MOTES; i++) {
      struct EffectStep *spark = &PARTICLES[i];
      if (frame > i && spark->variant == 0) {
        s32 size;
        Transform_Push();
        switch (i & 3) {
        case 0:
          SceneTransform_ApplyYaw(frame * (i * 32 + 256));
          break;
        case 1:
          SceneTransform_ApplyPitch(-frame * (i * 32 + 256));
          break;
        case 2:
          SceneTransform_ApplyRoll(-frame * (i * 32 + 256));
          break;
        case 3:
          SceneTransform_ApplyPitch(-frame * (i * 32 + 256));
          SceneTransform_ApplyRoll(-frame * (i * 32 + 256));
          break;
        }
        EffectPosition_ApplyBaseAndYOffset(spark, &screen);
        screen.x >>= 1;
        Transform_Pop();
        EFFECT_CLAMP(screen.depth, 250, 634);
        size = 9 - (screen.depth - 250) / 64;
        EFFECT_DRAW_MOTE(callbacks[0], canvas, sheet, screen.x, screen.y, size);
        EffectStep_AdvanceWithGravity3D(spark, 60, 0);
        if (frame > i + 30) {
          s32 dx = -spark->x >> 8;
          s32 dy = -spark->y >> 8;
          s32 dz = -spark->z >> 8;

          spark->velocityX += dx;
          spark->velocityY += dy;
          spark->velocityZ += dz;
        }
      }
    }
    // The Djinni appears at the swirl's centre, bobbing.
    if (frame > 82) {
      EFFECT_SET_VECTOR(djinniPoint, 0, (Trig_Sin(frame << 10) * 4), 0);
      EffectPosition_ApplyBaseAndYOffset(djinniPoint, &screen);
      screen.x >>= 1;
      draw[1](canvas, work, screen.x - 10, screen.y - 17, 20, 34);
    }
    EFFECT_PRESENT_FRAME(work);
  }
  // The healing: sparkles scattered round each target.
  for (i = 0; i != 512; i++) {
    struct EffectStep *spark = &PARTICLES[i];
    EFFECT_SET_POSITION(spark, ((Random16() & 0xff) - 127) << 15,
                        ((Random16() & 0x7f) + 64) << 15,
                        ((Random16() & 0xff) - 127) << 15);
    spark->variant = 0;
  }
  Resource_LoadAndDecompress((s32)&ResourceId_SparkleDots, sheet, 0, 0);
  for (frame = 0; frame != work->effect->count * 8 + 72; frame++) {
    struct BattleCamera *camera = gCameraWork;
    EFFECT_VIEW_FROM_CAMERA(camera);
    // The Djinni sways, and flies up and away for the last 32 frames.
    if (frame >= work->effect->count * 8 + 40)
      target[1] += 0x40000;
    EFFECT_SET_VECTOR(point, target[0], target[1],
                      target[2] + Trig_Sin(frame << 11) * 40);
    EffectPosition_ApplyBaseAndYOffset(point, &view);
    view.x >>= 1;
    callbacks[0](canvas, work, view.x - 10, view.y - 17, 20, 34);
    EFFECT_FOR_EACH_TARGET(member, work->effect) {
      struct MotionObject *object =
          GetBattleObjectSlotFar(work->effect->actors[member])->object;
      s32 start = member * 8;
      EFFECT_VIEW_FROM_CAMERA(camera);
      EFFECT_SET_VECTOR(position, object->x, 160 << 14, object->z);
      SceneTransform_ApplyPosition(position);
      if (frame == start + 30)
        AudioCommand_PlayFar(SOUND_RECOVERY);
      if (frame == start + 40)
        ObjectGroup_UpdateMembers(work->effect->actors[member],
                                  SPRITE_COLOR_WHITE, -1, -1, 0);
      if (frame == start + 64)
        ObjectGroup_UpdateMembers(work->effect->actors[member], 0, -1, -1, 0);
      if (frame > start) {
        SceneTransform_ApplyYaw((frame - start) << 9);
        for (i = 0; i != 64; i++) {
          struct EffectStep *spark = &PARTICLES[member * 64 + i];

          if (frame > start + i / 2) {
            s32 xx = (spark->x >> 8) * (spark->x >> 8);
            s32 yy = (spark->y >> 8) * (spark->y >> 8);
            s32 zz = (spark->z >> 8) * (spark->z >> 8);
            s32 distance = Iwram_Sqrt(xx + yy + zz) >> 9;
            if (distance != 0) {
              s32 cel;

              EffectPosition_ApplyBaseAndYOffset(spark, &view);
              view.x >>= 1;
              EFFECT_CLAMP(view.depth, 314, 634);
              /* FAKEMATCH: needed to match: the size series A draws its sparks
               * at, kept from that effect and never used here. Without the dead
               * store: differ 479. */
              cel = 3 - (view.depth - 314) / 128;
              cel = (i * 4 + frame) % 9;
              {
                u8 *cell = sheet + gBattleFx_PuffCells[cel];
                u32 edge = gBattleFx_PuffSizes[cel];
                u32 half = edge >> 1;

                draw[1](canvas, cell, view.x - half, view.y - half, edge, edge);
              }
              spark->x -= spark->x / distance;
              spark->y -= spark->y / distance;
              spark->z -= spark->z / distance;
            }
          }
        }
      }
    }
    EFFECT_PRESENT_FRAME(work);
  }
  EFFECT_FINISH_CANVAS();
}

// Scheduler callback: turns the battle camera by cameraDelta. The effect
// sets cameraPhase to 1 after each of its frames; the callback then turns
// the whole delta once, and half of it on the frames in between, so the
// turn stays smooth whatever the effect's frame rate.
void Camera_ApplyPhasedDelta(void) {
  // Mixing the slot-table work read with a separate camera base grows native
  // English from 0x58 to 0x5c bytes. Capture both owners from the same table.
  void **slots = gWorkSlot + HEAP_SLOT_BATTLE_EFFECT;
  struct BattleEffectWork *work = slots[0];
  struct BattleCamera *camera = EFFECT_SLOT(slots, HEAP_SLOT_CAMERA);
  s32 *phase = &work->cameraPhase;

  if (*phase == 1) {
    camera->yaw += work->cameraDelta;
    *phase = 0;
  } else {
    camera->yaw += work->cameraDelta / 2;
    if (*phase == 2)
      *phase = 0;
    else
      *phase = 2;
  }
}

// Calls the empty BattleFx_RunNoEffect count times, without waiting for a
// frame.
// count : how many calls
void BattleFx_RunNoEffectFrames(s32 count) {
  s32 callIndex;

  callIndex = 0;
  if (count != 0) {
    do {
      callIndex += 1;
      BattleFx_RunNoEffect();
    } while (callIndex != count);
  }
}

// Entry points for scene effects and table-dispatched effects.
//
// The battle asks for an effect by kind through one of three entry points:
//
// - BattleFx_RunTableEffect looks the kind up in gBattleFx_EffectHandlers,
//   the table of ordinary effects.
// - BattleFx_RunSceneEffect picks one of the large scene effects, the ones
//   whose presentation flags have bit 15 set, by a switch.
// - BattleFx_DispatchByIdRange runs the casting impacts, beam sequences and
//   ring scenes, which need a sprite sheet twice the usual size.
//
// Each entry point allocates the effect's three heap blocks (sprite sheet,
// work block and canvas), runs the effect to its end and releases them. A
// runner may schedule callbacks that use those blocks, but it must remove
// them before it returns to the dispatcher, since the blocks are gone
// afterwards. Shared helpers below manage the scanline window and the
// targets' animation objects.

// Runs one of the large battle effects, the ones whose presentation flags
// have bit 15 set: takes the effect's sheet, work and canvas, runs the
// effect chosen by its kind to the end, and frees them again.
// Each scene is named for the summon whose gBattle_ActionFlags entry selects
// its kind, as the handlers of gBattleFx_EffectHandlers are; kind 0, which no
// ability selects, runs Meteor's scene too.
void BattleFx_RunSceneEffect(struct BattleEffectArgument *effect) {
  Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_SHEET, 0x302);
  Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_EFFECT,
                            sizeof(struct BattleEffectWork));
  Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_CANVAS, 0x4000);

  if (effect->kind == 0) {
    BattleFx_RunMeteor(effect);
  } else {
    switch (effect->kind) {
    case 1:
      BattleFx_RunRamses(effect);
      break;
    case 2:
      BattleFx_RunNereid(effect);
      break;
    case 3:
      BattleFx_RunKirin(effect);
      break;
    case 4:
      BattleFx_RunAtalanta(effect);
      break;
    case 5:
      BattleFx_RunCybele(effect);
      break;
    case 6:
      BattleFx_RunNeptune(effect);
      break;
    case 7:
      BattleFx_RunTiamat(effect);
      break;
    case 8:
      BattleFx_RunProcne(effect);
      break;
    case 9:
      BattleFx_RunJudgment(effect);
      break;
    case 10:
      BattleFx_RunBoreas(effect);
      break;
    case 11:
      BattleFx_RunMeteor(effect);
      break;
    case 12:
      BattleFx_RunThor(effect);
      break;
    }
  }

  Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_CANVAS);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_EFFECT);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_SHEET);
}

// Runs an ordinary battle effect: takes the effect's sheet, work and
// canvas, runs the entry of gBattleFx_EffectHandlers for its kind,
// and frees them again. Kind 0 is no effect and only clears the variant
// in the shared command.
void BattleFx_RunTableEffect(struct BattleEffectArgument *effect) {
  s32 kind;
  struct BattleEffectWork *work;

  Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_SHEET, 0x302);
  Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_EFFECT,
                            sizeof(struct BattleEffectWork));
  Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_CANVAS, 0x4000);

  work = WORK_SLOT(HEAP_SLOT_BATTLE_EFFECT);
  work->effect = effect;
  kind = effect->kind;
  if (kind == 0)
    effect->variant = 0;
  else
    gBattleFx_EffectHandlers[kind - 1](effect);

  Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_CANVAS);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_EFFECT);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_SHEET);
}

// Scheduler callback: builds window 0's right edge for every scanline and
// restarts the HBlank DMA that writes it to WIN0H each line.
// gMapCellBuffer + 0    : the base right edge (u16)
// gMapCellBuffer + 2    : how far each of lines 8 to 135 pulls the edge in
//                         (u8 x 128)
// gMapCellBuffer + 0x82 : the WIN0H table built here (u16 x 160)
// Lines outside 8 to 135 get 0xfff1, a left edge past the right, so nothing
// of them is in the window.
void BattleFx_BuildWindowEdgeTable(void) {
  u16 *baseRightEdge;
  u16 *scanlineEdges;
  u8 *edgeOffsets;
  s32 scanline;
  s32 rightEdge;

  baseRightEdge = (u16 *)gMapCellBuffer;
  scanlineEdges = (u16 *)(Ram_MapCellBuffer + 0x82);
  edgeOffsets = Ram_MapCellBuffer + 2;
  for (scanline = 0; scanline != 160; scanline++) {
    if (scanline >= 8 && scanline <= 135) {
      rightEdge = *baseRightEdge - edgeOffsets[scanline - 8];
      EFFECT_CLAMP(rightEdge, 0, 240);
      scanlineEdges[scanline] = rightEdge;
    } else {
      scanlineEdges[scanline] = 0xfff1;
    }
  }
  {
    volatile u16 *channel = REG_DMA0;

    Dma_Stop(channel);
    Dma_Set((u8 *)gMapCellBuffer + 0x82, (void *)&REG_WIN0H,
            DMA_HBLANK_HALFWORD_PER_LINE, (volatile u32 *)channel);
  }
}

// Shows the battle objects of every unit with HP left on the targets' side:
// the enemies (units 128 to 133) when the first target is one, otherwise
// the party (units 0 to 7).
void BattleFx_SelectLivingTargets(struct BattleEffectArgument *arg) {
  s16 list[14];
  s32 count;
  s32 i;

  count = 0;
  if (arg->actors[0] > 127) {
    for (i = 0; i != 6; i++) {
      s32 id = i + 128;

      if (UnitGetFar(id)->hp > 0)
        list[count++] = id;
    }
  } else {
    for (i = 0; i != 8; i++) {
      if (UnitGetFar(i)->hp > 0)
        list[count++] = i;
    }
  }
  list[count] = 0xff; // the end of the list
  BattleActor_SpawnObjectsForListFar(list, 0);
}

// Shows the BG1 battle background behind a cleared canvas: clears the canvas
// buffer and the 16 KiB of VRAM it is copied to, then blends BG2 over the
// rest at 14/16 and moves the projection centre down to line 120.
void BattleFx_SetupBlendedDisplay(void) {
  void **slots = &gWorkSlot[HEAP_SLOT_BATTLE];
  u8 *canvas = slots[HEAP_SLOT_BATTLE_CANVAS - HEAP_SLOT_BATTLE];
  struct BattleWork *battle = slots[0];
  volatile u32 fill;

  REG_DISPCNT = DISPCNT_MODE_1;
  gBgScroll[1].y = 32;
  BattleBackground_LoadFar(1, battle->background, 24);
  Dma_Fill32Cell(fill, 0, canvas, 0x4000);
  Dma_Fill32Cell(fill, 0, BG_CHAR_BLOCK(1), 0x4000);
  WaitFrames(1);
  REG_BLDALPHA = BLEND_ALPHA(14, 16);
  REG_BLDCNT =
      BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_DISPCNT = EFFECT_DISPCNT;
  gProjection.centerY = 120;
}

// Updates every animation group of one battler's object.
//
// actorId: the battler whose object is updated.
// palette: the colour mode each sprite part takes (enum SpriteColorMode), 0
//          for the battler's own, or -1 to leave the parts alone. A part
//          that changes colours also restarts its frame.
// animation: the pose each group switches to (enum BattlePose), or -1 for
//            none.
// timerSlot: the actor timer (gBattleFxWork's actorTimers) to set, or -1;
//            ObjectGroup_TickMemberTimers restores the battler's palette
//            when it runs out.
// timerFrames: the frames that timer counts.
// The parts and animations of a battler that is fading out stay as they are.
void ObjectGroup_UpdateMembers(s32 actorId, s32 palette, s32 animation,
                               s32 timerSlot, s32 timerFrames) {
  struct BattleObj *slot;
  struct AnimationObject *group;
  struct BattleEffectWork *work;
  s32 groupIndex;

  slot = GetBattleObjectSlotFar(actorId);
  work = WORK_SLOT(HEAP_SLOT_BATTLE_EFFECT);
  groupIndex = 0;

  while ((group = GetMotionRecordFar(slot->object, groupIndex)) != NULL) {
    // The timer's low byte, read through memory as the original does.
    if (timerSlot != -1) {
      work->actorTimers[timerSlot] = ((u8 *)&timerFrames)[0];
    }
    if (slot->fading == 0) {
      if (palette != -1) {
        s32 partIndex;

        partIndex = 0;
        if (group->count != 0) {
          struct AnimationEntry **parts;

          parts = group->entries;
          do {
            struct AnimationEntry *part;

            // Every part but the battler's effect and animation entries.
            part = *parts++;
            if (part != NULL &&
                part != (struct AnimationEntry *)slot->effectEntry &&
                part != (struct AnimationEntry *)slot->animationEntry) {
              if (palette == 0)
                part->param = BattleMotion_GetSlotPaletteFar(actorId);
              else
                part->param = palette;
              part->frame = 0xff;
            }
            partIndex++;
          } while (partIndex != group->count);
        }
      }
      if (animation != -1)
        AnimationObjects_SelectAnimationFar(group, animation);
    }
    groupIndex++;
  }
}

// Writes the tiled canvas tilemap for BG2 (priority 1), then closes the
// canvas layer again, which fades the background palette back in.
void BattleFx_InitTilemapAndFlushQueue(void) {
  BattleFx_BeginTiledCanvas(1);
  BattleFx_EndCanvasLayer();
}

// Vines, falling rock and ice, and particles drawn from the targets.
// The family-specific scratch views describe trails, shards and gathering
// motes in the shared particle buffer.

#define CYBELE_FRAMES 366     // frames in the effect
#define CYBELE_CHIP_COUNT 48  // chips thrown by the thorns
#define CYBELE_VINES 10       // vines drawn
#define CYBELE_FIRST_SPARK 16 // the first spark in work->particles
#define CYBELE_FIRST_THORN 32 // the first thorn in work->particles
#define CYBELE_FIRST_CHIP 16  // the first chip in work->particles
#define CYBELE_EMBER_CELL 6   // the ember picture in gCybele_EmberOffsets

// gCybele_ObjectPoses has the animations of objects 3 and 4 for each pose.
#define CYBELE_POSE_OBJECT3(pose) gCybele_ObjectPoses[(pose) * 2]
#define CYBELE_POSE_OBJECT4(pose) gCybele_ObjectPoses[(pose) * 2 + 1]

// Abilities: Cybele (scene kind 5).
//
// Thorn vines: thorns fly in from the right, vines grow out of the ground
// and lift the targets while sparks flash around them, the picture
// dissolves pixel by pixel, and the targets drop and explode.
//
// effect: the targets.
void BattleFx_RunCybele(struct BattleEffectArgument *effect) {
  void **cursor;
  struct BattleEffectWork *work;
  u8 *canvas;
  s32 frame;
  s32 height;
  s32 speed;
  s32 phase;
  s32 rise;
  s32 i;
  s32 j;
  u8 *order;
  s32 point[3];
  struct EffectPosition screen;
  BlitterPair draw;

  cursor = gBattleFxWork;
  EFFECT_GET_WORK_AND_CANVAS(cursor, work, canvas);
  work->effect = effect;
  BattleFx_SpawnObjects(8, 0x177, 1);

  // The dissolve order: eight shuffled runs of the columns 0 to 127.
  order = Ram_MapCellBuffer;
  for (i = 0; i != 1024; i++)
    order[i] = i & 127;
  for (i = 0; i != 8; i++) {
    for (j = 0; j != 128; j++) {
      s32 a = Random16() & 127;
      s32 b = Random16() & 127;
      u8 *to = &order[i * 128 + b];
      u8 *from = &order[i * 128 + a];
      u8 t = *to;

      *to = *from;
      *from = t;
    }
  }

  BattleFx_BeginCanvasLayer(0);
  REG_BG2PA = Q_8_8(1);
  REG_BLDCNT = 0;
  Resource_LoadAndDecompress((s32)&ResourceId_ThornVineSheet, work->sheet, 1,
                             1);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                      COMPOSITE_TRANSPARENT);
  draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                      COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X,
                      COMPOSITE_TRANSPARENT);
  draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  REG_BG2X = -0x2000; // 32 pixels left (20.8 fixed point)

  // Vines (0 to 15): x, the y of the root, the growth speed and the frames
  // before the vine starts to grow.
  for (i = 0; i != 16; i++) {
    struct EffectStep *vine = &work->particles[i];

    if (i <= 4) {
      vine->x = i * 20;
      vine->y = (Random16() & 7) + 104;
    } else {
      vine->x = i * 20 - 90;
      vine->y = (Random16() & 7) + 108;
    }
    vine->velocityY = (Random16() & 7) + 4;
    vine->variant = (Random16() & 15) + 16;
  }
  // Sparks (16 to 31): a negative variant is the wait before one appears.
  for (i = 0; i != 16; i++) {
    struct EffectStep *spark = &work->particles[CYBELE_FIRST_SPARK + i];

    spark->x = (Random16() & 63) + 32;
    spark->y = (Random16() & 31) + 64;
    spark->variant = -(Random16() & 7) - 8;
  }
  // Thorns (32 to 47): fly left from the top right.
  for (i = 0; i != 16; i++) {
    struct EffectStep *thorn = &work->particles[CYBELE_FIRST_THORN + i];

    thorn->x = 128 << 16;
    thorn->y = 64 << 16;
    thorn->velocityX = -((Random16() & 255) + 200) << 9;
    thorn->velocityY = 0;
    thorn->variant = 0;
  }
  BattleFx_SelectLivingTargets(work->effect);

  height = -(64 << 16);
  speed = 0;
  for (frame = 0; frame != CYBELE_FRAMES; frame++) {
    struct BattleCamera *camera = gCameraWork;

    // A or B skips the vines straight to the drop.
    if ((gKeysRepeat & (KEY_A | KEY_B)) != 0 && frame > 190 && frame <= 285) {
      Iwram_ClearWords(canvas, 0x4000);
      frame = 286;
    }
    if (frame == 224)
      work->transferMode = CANVAS_COPY;
    EFFECT_VIEW_FROM_CAMERA(camera);
    if (frame == 31) {
      work->shakeFrames = 8;
      AudioCommand_PlayFar(157);
      EFFECT_FOR_EACH_TARGET(i, work->effect)
        BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 6);
    }
    if (frame == 72)
      AudioCommand_PlayFar(136);
    if (frame == 140)
      AudioCommand_PlayFar(156);

    // Drop the formation objects to a height of 64 pixels.
    speed += 0x4000;
    height += speed;
    if (height > 64 << 16)
      height = 64 << 16;
    BattleFx_PlaceFormationObjects(2, 128 << 16, height);
    if (frame >= 48 && frame <= 96) {
      s32 pose = (frame - 48) / 24 % 3;

      AnimationObjects_SelectAnimationFar(work->objects[3],
                                          CYBELE_POSE_OBJECT3(pose));
      AnimationObjects_SelectAnimationFar(work->objects[4],
                                          CYBELE_POSE_OBJECT4(pose));
    }

    // Launch one thorn per frame.
    if (frame >= 72 && frame <= 127) {
      for (i = 0; i != 16; i++) {
        struct EffectStep *thorn = &work->particles[CYBELE_FIRST_THORN + i];

        if (frame >= i + 72 && thorn->y < (104 << 16)) {
          s32 cel = (frame + i) / 4 % 5;
          s32 x = HI(thorn->x);
          s32 y = thorn->y >> 16;
          u8 *cell = work->sheet + gCybele_ThornOffsets[cel];
          u32 width = gCybele_ThornWidths[cel];
          u32 tall;

          x -= width >> 1;
          tall = gCybele_ThornHeights[cel];
          y -= tall >> 1;
          draw[0](canvas, cell, x, y, width, tall);
          EffectStep_AdvanceWithGravity2D(thorn, 64, 0x1000);
        }
      }
    }
    // Throw the chips (16 to 63) up from the ground.
    if (frame == 128) {
      for (i = 0; i != CYBELE_CHIP_COUNT; i++) {
        struct EffectStep *chip = &work->particles[CYBELE_FIRST_CHIP + i];

        chip->x = (Random16() % 96) << 16;
        chip->y = ((Random16() & 7) + 88) << 16;
        chip->velocityX = ((Random16() & 255) - 128) << 11;
        chip->velocityY = -(Random16() & 255) << 11;
        chip->variant = -(Random16() & 15) - 16;
      }
      REG_BG2X = 0;
    }

    // Grow the vines, shaking the screen while they grow.
    phase = frame - 128;
    if (phase >= 0 && phase <= 96) {
      rise = phase;
      if (rise > 80)
        rise = 80;
      else
        work->shakeFrames = 2;
      for (i = 0; i != CYBELE_VINES; i++) {
        struct EffectStep *vine = &work->particles[i];

        if (rise > vine->variant) {
          u8 *sheet = work->sheet;
          s32 top;
          s32 wrap;
          s32 k;

          if (i > 5)
            sheet = work->sheet + 0x6c0;
          top = (rise - vine->variant) * vine->velocityY;
          for (wrap = top; wrap > 184; wrap -= 64)
            ;
          // The tip (24x8).
          if (wrap <= 119)
            draw[i & 1](canvas, sheet, vine->x, vine->y - wrap - 8, 24, 8);
          // The stem: three 24x64 pieces, clipped at the top of the screen and
          // at the root.
          for (k = 0; k != 3; k++) {
            s32 y = vine->y - wrap + k * 64;
            s32 skip = 0;
            s32 rows = 64;

            if (y >= -64) {
              if (y < 0) {
                skip = -y * 24;
                rows = y + 64;
                y = 0;
              }
              if (y + rows > vine->y)
                rows -= y + rows - vine->y;
              draw[i & 1](canvas, sheet + skip + 192, vine->x, y, 24, rows);
            }
          }
          // A leaf, on the odd vines only.
          if ((i & 1) != 0) {
            s32 y = ((vine->y - top) & 127) - 16;
            s32 kind = i % 3;
            s32 full = gCybele_LeafSizes[kind];
            s32 rows = full;

            if (y + rows > vine->y)
              rows -= y + rows - vine->y;
            if (rows > 0)
              draw[i & 1](canvas, work->sheet + gCybele_LeafOffsets[kind],
                          vine->x + 8, y, full, rows);
          }
        }
      }
    }
    // Chips: until frame 159, one that leaves the screen is thrown up again.
    if (phase >= 0 && phase <= 95) {
      for (i = 0; i != 32; i++) {
        struct EffectStep *chip = &work->particles[32 + i];

        if (chip->variant >= 0) {
          s32 cel = i % 5;

          draw[0](canvas, work->sheet + gCybele_ChipOffsets[cel], HI(chip->x),
                  HI(chip->y), gCybele_ChipWidths[cel],
                  gCybele_ChipHeights[cel]);
          chip->x += chip->velocityX;
          chip->y += chip->velocityY;
          chip->velocityY += 0x4000;
          if ((chip->y < 0 || chip->y > 120 << 16) && frame <= 159) {
            chip->x = (Random16() % 96) << 16;
            chip->y = ((Random16() & 7) + 88) << 16;
            chip->velocityX = ((Random16() & 255) - 128) << 11;
            chip->velocityY = -(Random16() & 255) << 11;
          }
        }
        chip->variant++;
      }
    }

    // Every 4 frames, move the 64 background colours one step towards grey.
    if (frame >= 224 && frame <= 247 && (frame & 3) == 0) {
      u16 *color = (u16 *)BG_PLTT;

      for (i = 0; i != 64; i++) {
        s32 red = *color & 31;
        s32 green = (*color >> 5) & 31;
        s32 blue = (*color >> 10) & 31;
        s32 grey = (red + green + blue) / 3;

        if (red > grey)
          red--;
        if (red < grey)
          red++;
        if (green > grey)
          green--;
        if (green < grey)
          green++;
        if (blue > grey)
          blue--;
        if (blue < grey)
          blue++;
        *color++ = (blue << 10) | (green << 5) | red;
      }
    }

    // Six sparks keep flashing around each target's body.
    if (phase >= 0 && phase <= 172) {
      EFFECT_FOR_EACH_TARGET(i, work->effect) {
        struct MotionObject *object =
            GetBattleObjectSlotFar(work->effect->actors[i])->object;

        EFFECT_SET_VECTOR(point, object->x, object->y, object->z);
        EffectPosition_ApplyBaseAndYOffset(point, &screen);
        for (j = 0; j != 6; j++) {
          struct EffectStep *spark =
              &work->particles[CYBELE_FIRST_SPARK + i * 6 + j];

          if (spark->variant == 0) {
            spark->x = screen.x + (Random16() & 15) - 8;
            spark->y = screen.y + (Random16() & 15) - 40;
          }
          if (spark->variant >= 0 && spark->variant <= 4) {
            u8 *cell = work->sheet + gCybele_SparkOffsets[spark->variant];
            u32 size = gCybele_SparkSizes[spark->variant];
            u32 half = size >> 1;

            draw[0](canvas, cell, spark->x - half, spark->y - half, size, size);
          }
          spark->variant++;
          if (frame <= 199 && spark->variant == 5)
            spark->variant = -(Random16() & 7);
        }
      }
    }

    // Dissolve the 128x128 canvas from the top, two rows a frame, clearing
    // the pixels in the shuffled order.
    if (frame > 232) {
      s32 row = frame * 2 - 496;

      for (i = 0; i != 32; i++) {
        for (j = 0; j != 4; j++) {
          s32 y = row + i;

          if (y >= 0 && y <= 127) {
            s32 x = order[((y & 7) * 32 + i) * 4 + j];

            canvas[((y / 8 * 16 + x / 8) * 8 + (y & 7)) * 8 + (x & 7)] = 0;
          }
        }
      }
      row++;
      for (i = 0; i != 32; i++) {
        for (j = 0; j != 4; j++) {
          s32 y = row + i;

          if (y >= 0 && y <= 127) {
            s32 x = order[((y & 7) * 32 + i) * 4 + j];

            canvas[((y / 8 * 16 + x / 8) * 8 + (y & 7)) * 8 + (x & 7)] = 0;
          }
        }
      }
    }

    // Lift the targets to a height of 128 pixels, 8 frames apart.
    if (frame >= 161 && frame <= 223) {
      EFFECT_FOR_EACH_TARGET(i, work->effect) {
        if (frame > i * 8 + 160) {
          struct BattleObj *slot =
              GetBattleObjectSlotFar(work->effect->actors[i]);
          struct MotionObject *object = slot->object;
          void *record;
          s32 n;

          object->y += 8 << 16;
          if (object->y > 128 << 16)
            object->y = 128 << 16;
          object->verticalMotionStrength = 0;
          for (n = 0; (record = GetMotionRecordFar(slot->object, n)) != 0; n++)
            AnimationObjects_SelectAnimationFar(record, 5);
        }
      }
    }
    // Drop the targets from 96 pixels, 5 frames apart, and hit each one 16
    // frames later.
    EFFECT_FOR_EACH_TARGET(i, work->effect) {
      if (frame == i * 5 + 286) {
        struct MotionObject *object =
            GetBattleObjectSlotFar(work->effect->actors[i])->object;

        object->y = 96 << 16;
        object->verticalMotionStrength = 0xab85;
      }
      if (frame == i * 5 + 302) {
        ObjectGroup_UpdateMembers(work->effect->actors[i], SPRITE_COLOR_WHITE,
                                  -1, i, 8);
        AudioCommand_PlayFar(134);
        work->shakeFrames = 8;
      }
    }

    // Switch to the blast sheet and give each target ten embers, 72 degrees
    // apart (two rings of five).
    if (frame == 302) {
      EFFECT_RELEASE_BLITTERS();
      Resource_LoadAndDecompress((s32)&ResourceId_YellowRingSheet, work->sheet,
                                 1, 0);
      Resource_LoadAndDecompress((s32)&ResourceId_BlastSheet,
                                 work->sheet + 0x1680, 1, 1);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER,
                          COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, COMPOSITE_MAX);
      draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                          COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                              COMPOSITE_FLIP_X,
                          COMPOSITE_MAX);
      draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
      REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND |
                   BLDCNT_TGT2_ALL;
      REG_BG2PA = Q_8_8(0.5);
      REG_BG2X = 0;
      EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);
      EFFECT_FOR_EACH_TARGET(i, work->effect) {
        struct MotionObject *object =
            GetBattleObjectSlotFar(work->effect->actors[i])->object;

        for (j = 0; j != 10; j++) {
          struct EffectStep *ember = &work->particles[i * 10 + j];

          EFFECT_SET_POSITION(ember, object->x, 20 << 16, object->z);
          EFFECT_SET_VELOCITY(ember, (Trig_Sin(j * 0x3334) * 4),
                              (Random16() & 0x7fff) + (1 << 16),
                              (Trig_Cos(j * 0x3334) * 4));
          ember->variant = 0;
        }
      }
    }
    // The blast (six 40x24 frames, drawn as two halves), then five embers.
    if (frame > 301) {
      EFFECT_FOR_EACH_TARGET(i, work->effect) {
        s32 start = i * 4 + 302;

        if (frame >= start && frame < i * 4 + 314) {
          s32 cel = (frame - start) / 2;
          struct MotionObject *object =
              GetBattleObjectSlotFar(work->effect->actors[i])->object;

          EFFECT_SET_VECTOR(point, object->x, 0, object->z);
          EffectPosition_ApplyBaseAndYOffset(point, &screen);
          screen.x >>= 1;
          draw[0](canvas, work->sheet + cel * 480, screen.x - 20, screen.y - 24,
                  20, 24);
          draw[1](canvas, work->sheet + cel * 480, screen.x, screen.y - 24, 20,
                  24);
        }
        if (frame >= start + 6) {
          for (j = 0; j != 5; j++) {
            struct EffectStep *ember = &work->particles[i * 10 + j];

            EffectPosition_ApplyBaseAndYOffset(&ember->x, &screen);
            screen.x >>= 1;
            if (ember->variant >= 0 && ember->variant <= 26) {
              s32 cel = CYBELE_EMBER_CELL;
              s32 offset = gCybele_EmberOffsets[cel];
              u32 size = gCybele_EmberSizes[cel];

              draw[0](canvas, work->sheet + offset, screen.x - (size >> 1),
                      screen.y - (size >> 1), size, size);
            }
            EffectStep_AdvanceWithGravity2D(ember, 60, 0x1000);
            ember->variant++;
          }
        }
      }
    }

    if (frame <= 127)
      Camera_ApplyShake(4, 16);
    else if (frame <= 301)
      Camera_ApplyShake(2, 2);
    else
      Camera_ApplyShake(4, 8);
    EFFECT_END_FRAME(work);
  }

  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  EFFECT_RELEASE_BLITTERS();
  BattleEventRuntime_BeginPhaseFar(134);
  BattleFx_RunImpactBurst(2, 128 << 16, height);
  for (i = 0; i != 8; i++)
    ResourceObject_ReleaseFar(work->objects[i]);
  BattleFx_EndCanvasLayer();
}

// Rock, ice and wall effects that raise or break the battlefield.
//
// Boreas, Break, Quake, Spire, Gaia and the mirrored wall
// (BattleFx_RunMirroredWall) all draw onto the 128x128 effect canvas, which
// the canvas layer shows on BG2, and run a fixed number of frames, presenting
// the canvas at the end of each. Most throw pieces of debris about.
//
// Boreas, the rock and ice scene, is the longest. Its first half opens a
// window on a field of rocks that crumble in three waves, throwing up chips,
// while a big ice block drops in and bursts into spray trailed by smoke. Its
// second half rains ice blocks on the targets while boulders rise and crumble
// into chunks that bounce on the ground. Each half is a loop over frames
// whose steps run in the order the original drew and moved things, since the
// order of the random draws is part of the effect.
//
// Gaia, the rising wall, rebuilds its pictures from one decoded sheet: nine
// one-line rows of the growing wall stretched to 32 lines, the top and the
// foot of the standing wall and three frames of swelling ground. The ground
// swells, the wall grows row by row, stands while rocks (animation objects)
// tumble up its face, and sinks.
//
// The others are simpler: Break's ice shards burst from each target, the
// mirrored wall rises at a target's feet, Quake's three rock pillars throw
// the targets into the air, and Spire's boulders drop and shatter.

// The smoke trails behind the spray, after the 128 particles in PARTICLES.
#define ICE_BLOCK_TRAILS ((struct EffectStep *)((u8 *)gMapCellBuffer + 0xe00))
#define BOREAS_BIG_BLOCK_CELLS 0xc46      // the dropping 24x48 ice block
#define BOREAS_FALLING_BLOCK_CELLS 0x14f9 // the diagonal 40x80 ice block

// gBoreas_Points has an x and a y for each rock.
#define BOREAS_POINT_X(rock) gBoreas_Points[rock][0]
#define BOREAS_POINT_Y(rock) gBoreas_Points[rock][1]

// Abilities: Boreas (scene kind 10).
//
// Rocks and ice blocks: a window opens on rocks that crumble, a big ice
// block drops and throws up spray; then ice blocks rain down on the
// targets, and boulders crumble into bouncing chips.
//
// effect: the targets.
void BattleFx_RunBoreas(struct BattleEffectArgument *effect) {
  static struct SpriteScale unitScale = {{{0x10000, 0x10000}}};
  s32 rockPoints[32][2]; // a copy of gBoreas_Points that nothing reads
  struct SpritePlacement position;
  void *canvas;
  struct BattleEffectWork *work;
  s32 frame;
  DrawRectangle draw;
  DrawRectangle drawAlt;
  u8 *puffSheet;
  s32 edgeSpeed;
  s32 row;
  u32 *slots;
  s32 count;
  s32 i;
  s32 j;
  s32 shakeX;
  s32 shakeY;
  s32 y;
  s32 dropFrame;
  struct EffectStep *spray;
  struct EffectStep *trail;
  struct SpriteScale scale;

  slots = (u32 *)&gWorkSlot[HEAP_SLOT_BATTLE_CANVAS];
  canvas = (void *)slots[0];
  work = (struct BattleEffectWork *)CANVAS_SLOT(slots, HEAP_SLOT_BATTLE_EFFECT);
  puffSheet = (u8 *)CANVAS_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  edgeSpeed = 1;
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);
  BattleFx_ConfigureEffectDisplay();
  PLTT_BACKDROP = 0;
  *(u16 *)&BG_PLTT[1] = 0; // background palette 0, colour 1
  work->transferMode = CANVAS_COPY;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);
  REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_CLR;
  BattleFx_WipeCanvas(1, 0);
  REG_WIN0H = WIN_RANGE(240, 240); // an empty window
  Resource_LoadAndDecompress((s32)&ResourceId_IceBlockSheet, work->sheet, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_SparkleDots, puffSheet, 0, 0);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                      COMPOSITE_MAX);
  draw = (DrawRectangle)CANVAS_SLOT(slots, HEAP_SLOT_BLITTER);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                      COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                      COMPOSITE_TRANSPARENT);
  drawAlt = (DrawRectangle)CANVAS_SLOT(slots, HEAP_SLOT_BLITTER_ALTERNATE);
  gProjection.centerY = 240;
  BattleFx_SelectLivingTargets(work->effect);
  WaitFrames(1);
  BattleBackground_LoadFar(1, (s32)&ResourceId_VioletSkyBackdrop, 0);
  BattleFx_SpawnObjects(9, 0x174, 1);
  EFFECT_SHOW_WIDE_CANVAS(16);
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, 50);

  for (i = 0; i != 64; i++) {
    PARTICLES[i].x = (Random16() & 63) + 32;
    PARTICLES[i].y = (Random16() & 31) + 120;
    PARTICLES[i].variant = -1;
  }

  for (i = 0; i != 32; i++) {
    rockPoints[i][0] = BOREAS_POINT_X(i);
    rockPoints[i][1] = BOREAS_POINT_Y(i);
  }

  AudioCommand_PlayFar(141);

  //------------------------------------------------------------------
  // First half: the rocks, the big ice block and the spray (A or B skips
  // to the second half).
  //------------------------------------------------------------------
  for (frame = 0; frame != 288; frame++) {
    count = 16;
    if ((gKeysRepeat & (KEY_A | KEY_B)) && frame > 16) {
      break;
    }

    // Open the window from the left: its right edge speeds up, and each line
    // lags 0 to 63 pixels behind it.
    if (frame >= 0 && frame < 16) {
      u16 *edge = (u16 *)gMapCellBuffer;
      if (frame == 1) {
        u8 *inset = Ram_MapCellBuffer + 2;
        s32 line;
        for (line = 0; line != 128; line++) {
          inset[line] = Random16() & 63;
        }
        *edge = 0;
        Scheduler_AddOrUpdateCallback(BattleFx_BuildWindowEdgeTable, 0x480);
      }
      *edge += edgeSpeed;
      edgeSpeed += 3;
      if (frame == 15) {
        Scheduler_RemoveCallback(BattleFx_BuildWindowEdgeTable);
      }
    }

    // The rocks still standing.
    if (frame > 103) {
      count = 0;
    } else if (frame > 63) {
      count = 6;
    } else if (frame > 31) {
      count = 10;
    }

    // Shake: random until frame 167, from the table on frames 176 to 179.
    if (frame <= 167) {
      shakeX = (Random16() & 3) - 1;
      shakeY = (Random16() & 3) - 1;
      gBgScroll[1].x = shakeX;
      gBgScroll[1].y = shakeY + 32;
    } else {
      shakeX = 0;
      shakeY = 0;
      gBgScroll[1].x = 0;
      gBgScroll[1].y = 32;
    }
    if (frame >= 176 && frame < 180) {
      shakeX = -gBoreas_ShakeOffsets[frame - 176];
      shakeY = gBoreas_ShakeOffsets[frame - 176];
      gBgScroll[1].x = shakeX;
      gBgScroll[1].y = shakeY + 32;
    }

    for (i = 0; i != count; i++) {
      s32 cell = i % 3;
      draw(canvas, work->sheet + gBoreas_RockCells[cell],
           BOREAS_POINT_X(i) - shakeX,
           BOREAS_POINT_Y(i) - gBoreas_RockHeights[cell] - shakeY,
           gBoreas_RockWidths[cell], gBoreas_RockHeights[cell]);
    }

    // Lay out the 3x3 objects 32 pixels apart.
    scale = unitScale;
    if (frame == 174) {
      for (i = 0; i != 9; i++) {
        work->objects[i]->image.priority = 0;
      }
    }
    if (frame > 208) {
      AnimationObjects_SelectAnimationFar(work->objects[2],
                                          gBoreas_Animations[(frame / 4) & 3]);
    }

    scale.x = 0x10000;
    scale.y = 0x10000;
    position.groundHeight = 0;
    position.height = 255 << 16;
    for (row = 0; row != 3; row++) {
      for (j = 0; j != 3; j++) {
        position.x = (j * 32 + 144 - shakeX) << 16;
        position.y = (row * 32 + 76 - shakeY) << 16;
        Object_ApplyProjectedPlacementFar(work->objects[row * 3 + j], &position,
                                          &scale, 0);
      }
    }

    // The big ice block (24x48) falls from the top right on frames 160 to
    // 175 and sinks slowly from frame 208.
    dropFrame = frame - 160;
    if (dropFrame >= 0 && dropFrame < 158) {
      s32 sinkFrame = frame - 208;
      s32 blockX = 80;
      s32 blockY = 8;
      if (frame <= 175) {
        blockX = 96 - dropFrame;
        blockY = dropFrame * 4 - 56;
      } else if (frame > 208) {
        blockY = sinkFrame / 4 + 8;
      }
      drawAlt(canvas, work->sheet + BOREAS_BIG_BLOCK_CELLS, blockX, blockY, 24,
              48);
    }

    if (frame == 32) {
      AudioCommand_PlayFar(134);
    }
    if (frame == 64) {
      AudioCommand_PlayFar(134);
    }
    if (frame == 104) {
      AudioCommand_PlayFar(134);
    }
    if (frame == 176) {
      AudioCommand_PlayFar(134);
    }
    if (frame == 226) {
      AudioCommand_PlayFar(145);
    }
    Render_ResetTransformState();

    // Each time rocks crumble, throw up 32 chips.
    if (frame == 32) {
      for (i = 0; i != 32; i++) {
        PARTICLES[i].x = ((Random16() & 31) + 68) << 16;
        PARTICLES[i].y = ((Random16() & 31) + 8) << 16;
        PARTICLES[i].velocityX = ((Random16() & 127) - 63) << 11;
        PARTICLES[i].velocityY = ((-Random16() & 127) - 64) << 11;
        PARTICLES[i].variant = (Random16() & 15) + 32;
      }
    }
    if (frame == 64) {
      for (i = 0; i != 32; i++) {
        PARTICLES[i + 32].x = (Random16() % 48 + 60) << 16;
        PARTICLES[i + 32].y = ((Random16() & 31) + 52) << 16;
        PARTICLES[i + 32].velocityX = ((Random16() & 127) - 63) << 12;
        PARTICLES[i + 32].velocityY = ((-Random16() & 31) - 32) << 13;
        PARTICLES[i + 32].variant = (Random16() & 15) + 32;
      }
    }
    if (frame == 104) {
      for (i = 0; i != 32; i++) {
        PARTICLES[i].x = ((Random16() & 63) + 52) << 16;
        PARTICLES[i].y = ((Random16() & 31) + 72) << 16;
        PARTICLES[i].velocityX = ((Random16() & 127) - 63) << 11;
        PARTICLES[i].velocityY = ((-Random16() & 31) - 32) << 13;
        PARTICLES[i].variant = (Random16() & 15) + 32;
      }
    }

    if (frame >= 32 && frame < 208) {
      for (i = 0; i != 64; i++) {
        if (PARTICLES[i].variant >= 0) {
          s32 cell;
          if (frame > 191) {
            cell = i % 7 + 4;
          } else {
            cell = i & 3;
          }
          drawAlt(canvas, work->sheet + gBoreas_DebrisCells[cell],
                  HI(PARTICLES[i].x), HI(PARTICLES[i].y),
                  gBoreas_DebrisWidths[cell], gBoreas_DebrisHeights[cell]);
          PARTICLES[i].x += PARTICLES[i].velocityX;
          PARTICLES[i].y += PARTICLES[i].velocityY;
          PARTICLES[i].velocityY += 0x2000;
        }
      }
    }

    // Spray: 128 drops from (72, 56) towards the bottom left, leaving smoke
    // behind them from frame 228.
    if (frame > 223) {
      if (frame == 224) {
        for (i = 0; i != 128; i++) {
          PARTICLES[i].x = 72 << 16;
          PARTICLES[i].y = 56 << 16;
          PARTICLES[i].velocityX = (-(Random16() & 127) - 64) << 11;
          PARTICLES[i].velocityY = ((Random16() & 127) + 16) << 11;
          PARTICLES[i].variant = Random16();
        }
      }
      for (i = 0; i != 128; i++) {
        if (frame >= i / 4 + 224) {
          s32 cell = i % 3;
          if ((i & 1) == 0) {
            draw(canvas, work->sheet + gBoreas_SprayCells[cell],
                 HI(PARTICLES[i].x), HI(PARTICLES[i].y),
                 gBoreas_SprayWidths[cell], gBoreas_SprayHeights[cell]);
          }
          PARTICLES[i].x += PARTICLES[i].velocityX;
          PARTICLES[i].y += PARTICLES[i].velocityY;
          if ((PARTICLES[i].x >> 16) < -16 || (PARTICLES[i].y >> 16) > 120) {
            PARTICLES[i].x = 72 << 16;
            PARTICLES[i].y = 56 << 16;
          }
          PARTICLES[i].variant++;
        }
      }
      if (frame == 228) {
        for (i = 0; i != 128; i++) {
          spray = &PARTICLES[i];
          trail = &ICE_BLOCK_TRAILS[i];
          trail->x = spray->x;
          trail->y = spray->y;
          trail->variant = 0;
        }
      }
      for (i = 0, trail = ICE_BLOCK_TRAILS, spray = PARTICLES; i != 128;
           i++, trail++, spray++) {
        if (frame >= i + 228) {
          s32 cell = trail->variant / 2 % 9;
          s32 size = gBattleFx_PuffSizes[cell];
          draw(canvas, puffSheet + gBattleFx_PuffCells[cell],
               HI(trail->x) - size / 2, HI(trail->y) - size / 2, size, size);
          if (++trail->variant == 18) {
            trail->x = spray->x;
            trail->y = spray->y;
            trail->variant = 0;
          }
        }
      }
    }

    EFFECT_PRESENT_FRAME(work);
  }

  //------------------------------------------------------------------
  // Second half: ice blocks rain down on the targets, and boulders crumble
  // into bouncing chips.
  //------------------------------------------------------------------
  for (i = 0; i != 9; i++) {
    ResourceObject_ReleaseFar(work->objects[i]);
  }
  BattleFx_SetupBlendedDisplay();
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  AudioCommand_PlayFar(289);

  for (i = 0; i != 64; i++) {
    work->particles[i].x = ((Random16() & 127) + 64) << 16;
    work->particles[i].z = -(16 << 16);
    work->particles[i].y = (-(Random16() & 127) - 64) << 16;
    work->particles[i].velocityX = (-(Random16() & 63) - 127) << 12;
    work->particles[i].velocityY = ((Random16() & 63) + 127) << 12;
    work->particles[i].variant = 0;
  }
  for (i = 0; i != 64; i++) {
    PARTICLES[i].x = Random16() & 127;
    PARTICLES[i].y = (Random16() & 63) + i / 2;
    PARTICLES[i].variant = -i / 2;
  }

  for (frame = 0; frame != 146; frame++) {
    count = 0;
    if (frame == 96) {
      BattleEventRuntime_BeginPhaseFar(134);
    }
    if (frame == 16) {
      work->shakeFrames = 32;
    }
    // Raise one boulder every 2 frames, up to 16.
    if (frame > 16) {
      count = (frame - 16) / 2;
      if (count > 16) {
        count = 16;
      }
    }
    if (frame >= 9 && frame < 72 && (frame & 3) == 0) {
      AudioCommand_PlayFar(132);
    }
    if (frame == 72) {
      AudioCommand_PlayFar(145);
    }
    // A 40x80 block falls diagonally from the top right.
    if (frame > 64) {
      draw(canvas, work->sheet + BOREAS_FALLING_BLOCK_CELLS,
           (64 - frame) * 7 + 88, frame * 14 - 992, 40, 80);
    }
    if (frame <= 71) {
      for (i = 0; i != count; i++) {
        s32 cell = i % 3;
        draw(canvas, work->sheet + gBoreas_BoulderCells[cell],
             BOREAS_POINT_X(i) - 56,
             BOREAS_POINT_Y(i) - gBoreas_BoulderHeights[cell],
             gBoreas_BoulderWidths[cell], gBoreas_BoulderHeights[cell]);
      }
    }
    // The boulders crumble, throwing up 64 chips from where they stood.
    if (frame == 72) {
      for (i = 0; i != 64; i++) {
        work->particles[i].x = (BOREAS_POINT_X(i & 15) - 56) << 16;
        work->particles[i].y = BOREAS_POINT_Y(i & 15) << 16;
        work->particles[i].velocityX = ((Random16() & 127) - 63) << 13;
        work->particles[i].velocityY = (-(Random16() & 31) - 16) << 14;
      }
      if (frame == 72) {
        work->shakeFrames = 4;
      }
    }
    // Chips: one that lands fast at y = 120 bounces back at a quarter of its
    // speed.
    if (frame > 71) {
      count = 32;
      if (frame != 72) {
        count = 64;
      }
      for (i = 0; i != count; i++) {
        struct EffectStep *chip = &work->particles[i];
        y = HI(chip->y);
        if (y <= 135) {
          s32 cell = i % 3;
          draw(canvas, work->sheet + gBoreas_FallCells[cell], HI(chip->x),
               y - gBoreas_FallHeights[cell], gBoreas_FallWidths[cell],
               gBoreas_FallHeights[cell]);
          EffectStep_AdvanceWithGravity2D(chip, 64, 1 << 16);
          if (HI(chip->y) > 120 && chip->velocityY > 8 << 16) {
            chip->velocityY = -chip->velocityY / 4;
            chip->y = 120 << 16;
            work->shakeFrames = 1;
          }
        }
      }
    }
    // The falling ice blocks (24x48): until frame 47, one that lands starts
    // again at the top.
    if (frame <= 71) {
      for (i = 0; i != 64; i++) {
        draw(canvas, work->sheet, HI(work->particles[i].x) - 12,
             HI(work->particles[i].y) - 24, 24, 48);
        work->particles[i].x += work->particles[i].velocityX;
        work->particles[i].y += work->particles[i].velocityY;
        if ((work->particles[i].y >> 16) > 120 && frame <= 47) {
          work->particles[i].x = ((Random16() & 127) + 64) << 16;
          work->particles[i].y = -(16 << 16);
        }
      }
    }

    EFFECT_FOR_EACH_TARGET(i, work->effect) {
      if (frame == i * 8 + 32) {
        ObjectGroup_UpdateMembers(work->effect->actors[i],
                                  SPRITE_COLOR_BLUE_TINT, POSE_DAMAGE, -1, 0);
      }
    }

    // Smoke: until frame 127, each puff that ends starts again.
    if (frame > 72) {
      for (i = 0; i != 64; i++) {
        if (PARTICLES[i].variant >= 0 && PARTICLES[i].variant < 18) {
          s32 cell = PARTICLES[i].variant / 2 % 9;
          s32 size = gBattleFx_PuffSizes[cell];
          draw(canvas, puffSheet + gBattleFx_PuffCells[cell],
               PARTICLES[i].x - size / 2, PARTICLES[i].y - size / 2, size,
               size);
        }
        if (++PARTICLES[i].variant == 18 && frame <= 127) {
          PARTICLES[i].x = Random16() & 127;
          PARTICLES[i].y = (Random16() & 63) + (frame - 54) / 2;
          PARTICLES[i].variant = 0;
        }
      }
    }

    if (frame >= 72 && frame < 80) {
      Camera_ApplyShake(8, 8);
    } else {
      Camera_ApplyShake(2, 2);
    }
    EFFECT_PRESENT_FRAME(work);
  }

  EFFECT_RELEASE_BLITTERS();
  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  BattleFx_EndCanvasLayer();
}

// Ice shards.
#define ICE_SHARDS_PER_TARGET 128 // shards for each target

// Abilities: Break (kind 43).
//
// Ice shards burst up from each target in turn, 20 frames apart, tinting
// the target as they go, and fall away below line 112.
// effect: the targets.
void BattleFx_RunBreak(struct BattleEffectArgument *effect) {
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle draw;
  u8 *sheet;
  struct BattleCamera *camera;
  s32 point[3];
  struct EffectPosition screen;
  s32 member;
  s32 frame;
  s32 i;

  slots = &gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];
  cursor = slots;
  EFFECT_GET_WORK_AND_CANVAS(cursor, work, canvas);
  sheet = EFFECT_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  camera = EFFECT_SLOT(slots, HEAP_SLOT_CAMERA);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_IceBlockSheet), 128);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                      COMPOSITE_MAX);
  draw = (DrawRectangle)EFFECT_SLOT(slots, HEAP_SLOT_BLITTER);

  EFFECT_FREE_PARTICLES(i, PARTICLES, 8 * ICE_SHARDS_PER_TARGET);

  EFFECT_VIEW_FROM_CAMERA(camera);

  // Scatter the shards around the middle of each body; they spread sideways
  // and jump up.
  EFFECT_FOR_EACH_TARGET(member, work->effect) {
    struct MotionObject *object;
    s32 offset;
    s32 height;
    struct EffectStep *shard;

    offset = member * ICE_SHARDS_PER_TARGET * sizeof(struct EffectStep);
    object = GetBattleObjectSlotFar(work->effect->actors[member])->object;
    height = Battle_GetObjectTableValueFar(work->effect->actors[member]) / 2;
    EFFECT_SET_VECTOR(point, object->x, height, object->z);
    EffectPosition_ApplyBaseAndYOffset(point, &screen);
    screen.x >>= 1;
    for (i = 0, shard = (struct EffectStep *)(Ram_MapCellBuffer + offset);
         i != ICE_SHARDS_PER_TARGET; i++) {
      s32 radius;
      s32 angle;
      s32 lift;

      radius = Random16() & 0xff;
      angle = Random16() & 0xffff;
      shard->x = ((Trig_Sin(angle) * radius) >> 7) + (screen.x << 16);
      shard->y = ((Trig_Cos(angle) * radius) >> 3) + (screen.y << 16);
      shard->velocityX = (128 - (Random16() & 0xff)) << 9;
      lift = Random16() & 0xff;
      shard->variant = 0;
      shard->velocityY = (-lift - 128) << 10;
      shard++;
    }
  }

  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 50);

  for (frame = 0; frame != work->effect->count * 20 + 56; frame++) {
    if (frame == 32)
      BattleEventRuntime_BeginPhaseFar(0);
    EFFECT_FOR_EACH_TARGET(member, work->effect) {
      s32 offset;

      offset = member * ICE_SHARDS_PER_TARGET * sizeof(struct EffectStep);
      if (frame == member * 20) {
        AudioCommand_PlayFar(143);
        ObjectGroup_UpdateMembers(work->effect->actors[member],
                                  SPRITE_COLOR_WHITE, -1, member, 20);
      }
      // A shard that falls back past y = 112 goes out.
      if (frame > member * 20) {
        struct EffectStep *shard;

        for (i = 0, shard = (struct EffectStep *)(Ram_MapCellBuffer + offset);
             i != ICE_SHARDS_PER_TARGET; i++) {
          if (shard->variant >= 0) {
            s32 size;

            size = i % 3 + 1;
            EFFECT_DRAW_MOTE(draw, canvas, sheet, HI(shard->x), HI(shard->y),
                             size);
            EffectStep_AdvanceWithGravity2D(shard, 62, gBreak_Gravities[i & 3]);
            shard->variant++;
            if (shard->velocityY > 0 && HI(shard->y) > 112)
              shard->variant = -1;
          }
          shard++;
        }
      }
    }
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

// Gathering motes: motes of light leave the targets and are drawn into the
// user.

#define GATHERING_MOTES_PER_TARGET 128 // motes for each target
#define DRAIN_DRAWN 32                 // motes drawn for each target

// Abilities: Drain, Psy Drain (kind 42).
//
// Motes burst from each target in turn, 20 frames apart, and are pulled
// into the centre of the user's body.
//
// effect: the targets; variant 0 takes the ice block palette, any other
//         the blast palette.
void BattleFx_RunDrain(struct BattleEffectArgument *effect) {
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle draw;
  u8 *sheet;
  struct BattleCamera *camera;
  struct EffectPosition screen;
  s32 alternate;
  s32 member;
  s32 frame;
  s32 i;

  slots = &gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];
  cursor = slots;
  EFFECT_GET_WORK_AND_CANVAS(cursor, work, canvas);
  sheet = EFFECT_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  camera = *(struct BattleCamera **)((u8 *)gWorkSlot + HEAP_SLOT_CAMERA * 4);
  if (effect->variant == 0)
    alternate = 0;
  else
    alternate = 1;
  work->effect = effect;
  BattleFx_BeginCanvasLayer(1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry(alternate == 0
                                             ? (s32)&ResourceId_IceBlockSheet
                                             : (s32)&ResourceId_BlastSheet),
                  128);

  // The motes fly out of the centre of each target's body at random speeds
  // of up to 2 pixels a frame on each axis.
  EFFECT_FREE_PARTICLES(i, PARTICLES, 8 * GATHERING_MOTES_PER_TARGET);

  EFFECT_FOR_EACH_TARGET(member, work->effect) {
    struct MotionObject *object;
    s32 halfHeight;

    object = GetBattleObjectSlotFar(work->effect->actors[member])->object;
    halfHeight =
        Battle_GetObjectTableValueFar(work->effect->actors[member]) / 2;
    for (i = 0; i != GATHERING_MOTES_PER_TARGET; i++) {
      struct EffectStep *mote =
          &PARTICLES[member * GATHERING_MOTES_PER_TARGET + i];

      EFFECT_SET_POSITION(mote, object->x, object->y + halfHeight, object->z);
      EFFECT_SET_VELOCITY(mote, ((Random16() & 0xff) - 128) << 10,
                          ((Random16() & 0xff) - 128) << 10,
                          ((Random16() & 0xff) - 128) << 10);
      mote->variant = 0;
    }
  }

  // Draw with the maximum blend, and lower every canvas pixel by 4 on each
  // transfer so the motes leave fading trails.
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                      COMPOSITE_MAX);
  draw = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_DARKEN, 0x04040404);
  AudioCommand_PlayFar(142);

  for (frame = 0; frame != work->effect->count * 20 + 72; frame++) {
    struct MotionObject *user;
    s32 halfHeight;

    user = GetBattleObjectSlotFar(work->effect->actor)->object;
    halfHeight = Battle_GetObjectTableValueFar(work->effect->actor) / 2;
    if (frame == 64)
      BattleEventRuntime_BeginPhaseFar(133);
    EFFECT_VIEW_FROM_CAMERA(camera);
    if (frame == 40)
      ObjectGroup_UpdateMembers(work->effect->actor, SPRITE_COLOR_WHITE, -1, -1,
                                0);
    if (frame == work->effect->count * 20 + 52)
      ObjectGroup_UpdateMembers(work->effect->actor, 0, -1, -1, 0);
    EFFECT_FOR_EACH_TARGET(member, work->effect) {
      if (frame == member * 20)
        ObjectGroup_UpdateMembers(work->effect->actors[member],
                                  SPRITE_COLOR_WHITE, POSE_DAMAGE, member, 42);
      if (frame > member * 20) {
        for (i = 0; i != DRAIN_DRAWN; i++) {
          struct EffectStep *mote =
              &PARTICLES[member * GATHERING_MOTES_PER_TARGET + i];

          if (mote->variant >= 0) {
            s32 size;

            // The canvas is shown at double width, so halve x.
            EffectPosition_ApplyBaseAndYOffset(mote, &screen);
            screen.x >>= 1;
            size = 6;
            EFFECT_DRAW_MOTE(draw, canvas, sheet, screen.x, screen.y, size);
            EffectStep_AdvanceWithGravity3D(mote, 62, 0);

            // 10 frames after it appears (one frame later for each mote), a
            // mote accelerates towards the user by 1/256 of the distance, and
            // goes out once it is within 16 pixels horizontally.
            if (frame > member * 20 + i + 10) {
              s32 dx;
              s32 dy;
              s32 dz;

              dx = (user->x - mote->x) >> 8;
              dy = (user->y + halfHeight - mote->y) >> 8;
              dz = (user->z - mote->z) >> 8;
              mote->velocityX += dx;
              mote->velocityY += dy;
              mote->velocityZ += dz;
              if ((dx > -0x1000 && dx < 0x1000) &&
                  (dz > -0x1000 && dz < 0x1000))
                mote->variant = -1;
            }
          }
        }
      }
    }
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

// Healing and stat-boost presentations.
//
// Healing sparkles come in seven modes. Motes of light scattered around each
// target swirl in towards it, one target every 8 frames, with the recovery
// sound on each target's 20th frame and a tint on its 36th. The modes differ
// in how the motes scatter and turn and in which resource they draw; the
// wrappers name the mode of each effect.
//
// PARTICLES holds the motes: 512 are scattered, and target n draws from the
// 64 at n * 64 (32 of them, or 128 running into the next target's, by the
// effect's variant). Each mote keeps its position relative to the target's
// centre, so swirling the motes is a turn of the whole frame of reference,
// and drawing one in is a step of 2 pixels a frame along the line to the
// centre.
//
// A stat boost brings out a Djinni, which sways from side to side and, after
// 32 frames, rises 2 pixels a frame; then it swirls flares of light in to
// each target in turn, 8 frames apart, and a burst of light and the mode's
// picture (a dagger, a shield, a streak or a wisp) play over it.

// Healing sparkles, in seven modes: motes of light swirl in around each
// target. PARTICLES holds the motes, 64 for each target.

#define HEALING_DJINNI_CELLS 0x1000 // the djinni picture in the sheet

// Abilities: Ply, Ply Well, Pure Ply, Herb, Nut, Vial, Potion, Soothing Water
// (kind 22); Fizz (kind 204).
//
// One entry for each mode.
DEFINE_EFFECT_KIND(BattleFx_RunPly, BattleFx_RunHealingMotes, 0)

// Abilities: Wish, Wish Well, Pure Wish (kind 23); Regenerate (kind 44);
// Reflect (kind 45); Spritz (kind 206).
//
// Gather healing motes around each target with scattering style 1.
DEFINE_EFFECT_KIND(BattleFx_RunWish, BattleFx_RunHealingMotes, 1)

// Abilities: Cure, Cure Well, Potent Cure, Corn (kind 21); Worms (kind 352).
//
// Gather healing motes using the blast sheet.
DEFINE_EFFECT_KIND(BattleFx_RunCure, BattleFx_RunHealingMotes, 2)

// Abilities: Cure Poison, Restore, Antidote, Elixir (kind 24).
//
// Mode 3 when effect->variant is 0, mode 4 otherwise.
void BattleFx_RunCurePoison(struct BattleEffectArgument *effect) {
  if (effect->variant == 0) {
    BattleFx_RunHealingMotes(effect, 3);
    return;
  }
  BattleFx_RunHealingMotes(effect, 4);
}

// Send healing sparkles out from each target with scattering style 4.
DEFINE_EFFECT_KIND(BattleFx_RunHealingMotesMode4, BattleFx_RunHealingMotes, 4)

// Abilities: Tonic (kind 208).
//
// Send healing sparkles out from each target with scattering style 5.
DEFINE_EFFECT_KIND(BattleFx_RunTonic, BattleFx_RunHealingMotes, 5)

// Abilities: Psy Crystal, Adept Ring, Ember (kind 212).
//
// Gather healing motes in the Mars Djinni palette.
DEFINE_EFFECT_KIND(BattleFx_RunPsyCrystal, BattleFx_RunHealingMotes, 6)

#define HEALING_MOTES 512 // motes in PARTICLES
#define HEALING_SLOTS 64  // particles reserved for each target

// Healing sparkles: motes of light scattered around each target swirl in
// towards it, one target every 8 frames, with the recovery sound on each
// target's 20th frame and a tint on its 36th.
//
// effect: the targets. With prelude 1, a djinni (Mars in mode 6, Mercury
//         otherwise) appears first, sways and rises. variant picks the
//         motes per target (0: 32, 2: 128, others: 64) and, in modes 0,
//         1, 3, 4 and 5, the resource (0: ice shard, others: ice block).
// mode: how the motes scatter and turn. Only mode 0 shows the canvas at
//       its own width, centred on the first target. Modes 3, 4 and 5 put
//       the motes out one by one instead of drawing them in; modes 0, 3,
//       4 and 5 draw sparkle dots. Mode 6 takes the Mars resource, mode 2
//       the blast resource.
void BattleFx_RunHealingMotes(struct BattleEffectArgument *effect, u32 mode) {
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frameCount;
  s32 member;
  u8 *sprites;
  s32 motesPerTarget;
  s32 canvasShiftX;
  s32 doubleWidth;
  DrawRectangle *draw;
  s32 frame;
  s32 i;
  u8 *resource;
  struct EffectPosition base;
  struct EffectPosition screen;
  s32 point[3];
  s32 originX;
  s32 originY;
  BlitterPair blit;

  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  sprites = EFFECT_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  canvasShiftX = 0;
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);

  // Bring out the djinni; origin receives where it appears.
  if (work->effect->prelude == 1) {
    s32 kind = mode == 6 ? 2 : 1;

    if (mode == 6 || mode == 0)
      BattleFx_PrepareCanvasEffect(effect, kind, work->effect->side, 0,
                                   &originX, &originY);
    else
      BattleFx_PrepareCanvasEffect(effect, kind, work->effect->side, 1,
                                   &originX, &originY);
    work->effect->variant = 0;
  }

  // Mode 0 shows the canvas at its own width, shifted so the first target
  // is at its centre (64); the others keep it at double width.
  if (mode == 0) {
    EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &base);
    canvasShiftX = 64 - base.x;
    REG_BG2X = canvasShiftX << 8;
    REG_BG2PA = Q_8_8(1);
    doubleWidth = 0;
  } else {
    doubleWidth = 1;
  }

  // Pictures and resource.
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sprites, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_SparkleDots, work->sheet, 0, 0);
  if (mode <= 1 || mode == 3 || mode == 4 || mode == 5) {
    if (work->effect->variant == 0)
      resource = Resource_GetTableEntry((s32)&ResourceId_IceShardSheet);
    else
      resource = Resource_GetTableEntry((s32)&ResourceId_IceBlockSheet);
  } else if (mode == 6) {
    resource = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
  } else {
    resource = Resource_GetTableEntry((s32)&ResourceId_BlastSheet);
  }
  Iwram_CopyWords((void *)BG_PLTT, resource, 128);

  // The djinni: 40x40 at the canvas's own width, the small 20x40 picture at
  // double width.
  if (doubleWidth == 0) {
    if (mode == 6)
      resource = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
    else
      resource = Resource_GetTableEntry((s32)&ResourceId_MercuryDjinnSheet);
  } else {
    if (mode == 6)
      resource = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSmallSheet);
    else
      resource =
          Resource_GetTableEntry((s32)&ResourceId_MercuryDjinnSmallSheet);
  }
  resource += 128;
  Resource_DecodeType01(resource, work->sheet + HEALING_DJINNI_CELLS);
  BattleFx_FetchRectangleBlitters(work->effect->side,
                                  draw = blit); // [0] maximum, [1] additive

  // Scatter the motes around the centre.
  if (mode == 0 || mode == 6) {
    // x and z within 64 pixels, y 32 to 95 pixels up.
    for (i = 0; i != HEALING_MOTES; i++) {
      struct EffectStep *spark = &PARTICLES[i];

      EFFECT_SET_POSITION(spark, ((Random16() & 0xff) - 127) << 15,
                          ((Random16() & 0x7f) + 64) << 15,
                          ((Random16() & 0xff) - 127) << 15);
      spark->variant = 0;
    }
    frameCount = work->effect->count * 8 + 88;
  } else if (mode == 1) {
    // All three within 64 pixels.
    for (i = 0; i != HEALING_MOTES; i++) {
      struct EffectStep *spark = &PARTICLES[i];

      EFFECT_SET_POSITION(spark, ((Random16() & 0xff) - 127) << 15,
                          ((Random16() & 0xff) - 127) << 15,
                          ((Random16() & 0xff) - 127) << 15);
      spark->variant = 0;
    }
    frameCount = work->effect->count * 8 + 88;
  } else if (mode == 2) {
    // A ring of radius 32 to 95 pixels, 50 pixels down.
    for (i = 0; i != HEALING_MOTES; i++) {
      struct EffectStep *spark = &PARTICLES[i];
      s32 heading = Random16() & 0xffff;
      s32 radius = (Random16() & 63) + 32;

      EFFECT_SET_POSITION(spark, Trig_Sin(heading) * radius, -50 << 16,
                          Trig_Cos(heading) * radius);
      spark->velocityY = ((Random16() & 31) + 32) << 13;
      spark->variant = 0;
    }
    frameCount = work->effect->count * 8 + 88;
  } else if (mode == 3) {
    // x and z within 64 pixels, y within 32.
    for (i = 0; i != HEALING_MOTES; i++) {
      struct EffectStep *spark = &PARTICLES[i];

      EFFECT_SET_POSITION(spark, ((Random16() & 0xff) - 127) << 15,
                          ((Random16() & 0xff) - 127) << 14,
                          ((Random16() & 0xff) - 127) << 15);
      spark->variant = 0;
    }
    frameCount = work->effect->count * 8 + 72;
  } else {
    // All three within 64 pixels.
    for (i = 0; i != HEALING_MOTES; i++) {
      struct EffectStep *spark = &PARTICLES[i];

      EFFECT_SET_POSITION(spark, ((Random16() & 0xff) - 127) << 15,
                          ((Random16() & 0xff) - 127) << 15,
                          ((Random16() & 0xff) - 127) << 15);
      spark->variant = 0;
    }
    frameCount = work->effect->count * 8 + 72;
  }

  // Motes for each target: 32 for variant 0, 128 for variant 2, else 64.
  motesPerTarget = 64;
  if (work->effect->variant == 0)
    motesPerTarget = 32;
  else if (work->effect->variant == 2)
    motesPerTarget = 128;
  work->transferMode =
      CANVAS_COPY_AND_FADE; // each transfer fades the canvas to 3/4, for trails
  work->transferValue = 75; // percent
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);

  for (frame = 0; frame != frameCount; frame++) {
    struct BattleCamera *camera = gCameraWork;

    if (frame == 40)
      BattleEventRuntime_BeginPhaseFar(0);

    // The djinni sways from side to side and, after 32 frames, rises 2 pixels
    // a frame. For the first 4 frames it is also added on top.
    if (work->effect->prelude == 1) {
      if (doubleWidth == 0) {
        s32 x =
            ((Trig_Sin(frame << 11) * 20) >> 16) + originX + canvasShiftX - 20;
        s32 y = ((Trig_Cos(frame << 11) * 4) >> 16) + originY - 24;

        if (frame > 32)
          y = y - frame * 2 + 64;
        blit[0](canvas, work->sheet + HEALING_DJINNI_CELLS, x, y, 40, 40);
        if (frame <= 3)
          draw[1](canvas, work->sheet + HEALING_DJINNI_CELLS, x, y, 40, 40);
      } else {
        s32 x = ((Trig_Sin(frame << 11) * 10) >> 16) + originX / 2 - 10;
        s32 y = ((Trig_Cos(frame << 11) * 4) >> 16) + originY - 24;

        if (frame > 32)
          y = y - frame * 2 + 64;
        blit[0](canvas, work->sheet + HEALING_DJINNI_CELLS, x, y, 20, 40);
        if (frame <= 3)
          draw[1](canvas, work->sheet + HEALING_DJINNI_CELLS, x, y, 20, 40);
      }
    }

    EFFECT_FOR_EACH_TARGET(member, work->effect) {
      struct MotionObject *object =
          GetBattleObjectSlotFar(work->effect->actors[member])->object;
      s32 start = member * 8;

      // Centre on the point 40 pixels above the target.
      EFFECT_VIEW_FROM_CAMERA(camera);
      EFFECT_SET_VECTOR(point, object->x, 40 << 16, object->z);
      SceneTransform_ApplyPosition(point);
      if (frame == start + 20)
        AudioCommand_PlayFar(SOUND_RECOVERY);
      if (frame == start + 36)
        ObjectGroup_UpdateMembers(work->effect->actors[member],
                                  SPRITE_COLOR_WHITE, -1, member, 28);
      if (frame > start) {
        if (mode == 0 || mode == 6) {
          SceneTransform_ApplyYaw((frame - member * 8) << 9);
        } else if (mode == 1) {
          SceneTransform_ApplyPitch(frame << 9);
          SceneTransform_ApplyRoll(frame << 9);
        } else if (mode == 2) {
          SceneTransform_ApplyYaw((frame - member * 40) << 9);
        } else if (mode == 3) {
          SceneTransform_ApplyYaw((frame - member * 8) << 9);
        } else {
          SceneTransform_ApplyYaw((frame - member * 8) << 9);
          SceneTransform_ApplyPitch((frame - member * 8) << 9);
        }

        // Four motes appear each frame; in modes 3, 4 and 5 mote i goes out at
        // frame start + i / 2 + 32.
        for (i = 0; i != motesPerTarget; i++) {
          struct EffectStep *spark = &PARTICLES[member * HEALING_SLOTS + i];
          s32 endFrame;

          if (mode == 3 || mode == 4 || mode == 5)
            endFrame = start + i / 2 + 32;
          else
            endFrame = 0x10000;
          if (frame > start + i / 4 && frame < endFrame) {
            s32 xx = (spark->x >> 8) * (spark->x >> 8);
            s32 yy = (spark->y >> 8) * (spark->y >> 8);
            s32 zz = (spark->z >> 8) * (spark->z >> 8);
            s32 distance = Iwram_Sqrt(xx + yy + zz) >> 9;

            if (distance != 0) {
              s32 size;

              EffectPosition_ApplyBaseAndYOffset(spark, &screen);
              if (mode == 0)
                screen.x += canvasShiftX;
              else
                screen.x >>= 1;
              screen.y += 16;

              // Size 3 to 1 over depths 314 to 634 (mode 0 cycles through 9
              // sizes).
              EFFECT_CLAMP(screen.depth, 314, 634);
              size = 3 - (screen.depth - 314) / 128;
              if (mode == 0)
                size = (i * 4 + frame) % 9;
              if (mode == 0 || mode == 3 || mode == 4 || mode == 5) {
                u8 *cell = work->sheet + gBattleFx_PuffCells[size];
                u32 edge = gBattleFx_PuffSizes[size];
                u32 half = edge >> 1;

                draw[1](canvas, cell, screen.x - half, screen.y - half, edge,
                        edge);
              } else {
                EFFECT_DRAW_MOTE(draw[1], canvas, sprites, screen.x, screen.y,
                                 size);
              }

              // Move the mote 2 pixels a frame towards the centre.
              if (mode <= 2 || mode == 6) {
                spark->x -= spark->x / distance;
                spark->y -= spark->y / distance;
                spark->z -= spark->z / distance;
              }
            }
          }
        }
      }
    }
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS();
}

// Abilities: Impact, High Impact, Demon Spear, Angel Spear (kind 26); Forge
// (kind 210); kind 46 is used by no ability.
//
// Stat boosts, in six modes: light gathers on each target and bursts, and
// a picture (a dagger, a shield, a streak or a wisp) plays over it.
// PARTICLES holds the gathering flares, 64 for each target.
// One entry for each mode.
DEFINE_EFFECT_KIND(BattleFx_RunImpact, BattleFx_RunStatBoost, 0)

// Abilities: Guard, Protect, Guardian, Protector (kind 28); Corona (kind 211).
//
// Gather stat-boost light and display a shield in Mars colours.
DEFINE_EFFECT_KIND(BattleFx_RunGuard, BattleFx_RunStatBoost, 1)

// Abilities: Ward, Resist, Magic Shell, Magic Shield (kind 30); Breeze (kind
// 215).
//
// Gather stat-boost light and display a shield in Jupiter colours.
DEFINE_EFFECT_KIND(BattleFx_RunWard, BattleFx_RunStatBoost, 2)

// Abilities: Granite (kind 200).
//
// Gather stat-boost light and display a violet streak in Venus colours.
DEFINE_EFFECT_KIND(BattleFx_RunGranite, BattleFx_RunStatBoost, 3)

// Abilities: Flash (kind 213).
//
// Gather stat-boost light and display a violet streak in Mars colours.
DEFINE_EFFECT_KIND(BattleFx_RunFlash, BattleFx_RunStatBoost, 5)

// Abilities: Zephyr (kind 216).
//
// Gather stat-boost light and display a wisp without replacing its palette.
DEFINE_EFFECT_KIND(BattleFx_RunZephyr, BattleFx_RunStatBoost, 4)

// Where each picture sits in the sheet.
enum StatBoostSheet {
  STAT_BOOST_FLARES = 0x0000,  // flares (ParticleSpritesD)
  STAT_BOOST_BURST = 0x060e,   // six 40x40 bursts of light (GlowOrb)
  STAT_BOOST_PICTURE = 0x2b8e, // the picture the mode picks
  STAT_BOOST_DJINNI = 0x65c0   // the 40x40 djinni
};

#define STAT_BOOST_SCATTERED 512 // flares scattered at the start
#define STAT_BOOST_DRAWN 32      // flares drawn for each target
#define STAT_BOOST_SLOTS 64      // particles reserved for each target

// Flares of light swirl in to the centre of each target, one target every
// 8 frames, burst, and the mode's picture plays. The canvas is shown at
// 1.25 times its width (canvas x is 4/5 of screen x).
//
// effect: the targets. With prelude 1, a djinni appears first and sways
//         in place.
// mode: the picture: 0 a dagger, 1 and 2 a shield, 3 and 5 a violet
//       streak, 4 a wisp. The palette comes from the Venus djinni in mode
//       3, the Jupiter djinni in mode 2, the Mars djinni in modes 0, 1 and
//       5; mode 4 keeps the palette. The djinni is Venus in mode 3,
//       Jupiter in modes 2 and 4, Mars otherwise.
void BattleFx_RunStatBoost(struct BattleEffectArgument *effect, u32 mode) {
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  BlitterPair draw;
  s32 member;
  s32 canvasShiftX;
  s32 drawFlags;
  s32 frame;
  s32 i;
  struct EffectPosition base;
  s32 center[3];
  struct EffectPosition screen;
  s32 point[3];
  s32 originX;
  s32 originY;

  cursor = gBattleFxWork;
  EFFECT_GET_WORK_AND_CANVAS(cursor, work, canvas);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);

  // Bring out the djinni (Venus is 0, Jupiter 3, Mars 2); origin receives
  // where it appears.
  if (work->effect->prelude == 1) {
    if (mode == 3)
      BattleFx_PrepareCanvasEffect(effect, 0, work->effect->side ^ 1, 1,
                                   &originX, &originY);
    else if (mode == 2 || mode == 4)
      BattleFx_PrepareCanvasEffect(effect, 3, work->effect->side ^ 1, 1,
                                   &originX, &originY);
    else
      BattleFx_PrepareCanvasEffect(effect, 2, work->effect->side ^ 1, 1,
                                   &originX, &originY);
    originX = originX * 4 / 5;
  }
  REG_BG2PA = Q_8_8(0.8); // 1.25 times the width

  // Pictures and palette.
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD,
                             work->sheet + STAT_BOOST_FLARES, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_GlowOrbSheet,
                             work->sheet + STAT_BOOST_BURST, 1, 1);
  if (mode == 3 || mode == 5) {
    Resource_LoadAndDecompress((s32)&ResourceId_VioletStreakSheet,
                               work->sheet + STAT_BOOST_PICTURE, 1, 1);
    if (mode == 3)
      Iwram_CopyWords((void *)BG_PLTT,
                      Resource_GetTableEntry((s32)&ResourceId_VenusDjinnSheet),
                      128);
    else
      Iwram_CopyWords((void *)BG_PLTT,
                      Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet),
                      128);
  } else if (mode == 4) {
    Resource_LoadAndDecompress((s32)&ResourceId_WispSheet,
                               work->sheet + STAT_BOOST_PICTURE, 1, 1);
  } else {
    void *palette;

    if (mode == 0)
      Resource_LoadAndDecompress((s32)&ResourceId_DaggerSheet,
                                 work->sheet + STAT_BOOST_PICTURE, 1, 0);
    else
      Resource_LoadAndDecompress((s32)&ResourceId_ShieldSheet,
                                 work->sheet + STAT_BOOST_PICTURE, 1, 0);
    if (mode == 0)
      palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
    else if (mode == 2 || mode == 4)
      palette = Resource_GetTableEntry((s32)&ResourceId_JupiterDjinnSheet);
    else if (mode == 1)
      palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
    else
      palette = Resource_GetTableEntry((s32)&ResourceId_PinkBurstSheet);
    Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  }
  if (mode == 3)
    Resource_LoadAndDecompress((s32)&ResourceId_VenusDjinnSheet,
                               work->sheet + STAT_BOOST_DJINNI, 1, 0);
  else if (mode == 2 || mode == 4)
    Resource_LoadAndDecompress((s32)&ResourceId_JupiterDjinnSheet,
                               work->sheet + STAT_BOOST_DJINNI, 1, 0);
  else
    Resource_LoadAndDecompress((s32)&ResourceId_MarsDjinnSheet,
                               work->sheet + STAT_BOOST_DJINNI, 1, 0);

  // Scatter the flares around the centre: x and z within 25 pixels, y
  // within 50.
  for (i = 0; i != STAT_BOOST_SCATTERED; i++) {
    struct EffectStep *flare = &PARTICLES[i];

    EFFECT_SET_POSITION(flare, (Random16() % 200 - 100) << 14,
                        (Random16() % 200 - 100) << 15,
                        (Random16() % 200 - 100) << 14);
    flare->variant = 0;
  }

  // With one target, canvasShiftX the canvas so that target is at its centre
  // (64).
  if (work->effect->count == 1) {
    EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &base);
    canvasShiftX = -base.x * 4 / 5 + 64;
  } else {
    if (work->effect->side == 1)
      canvasShiftX = -64;
    else
      canvasShiftX = 0;
  }
  REG_BG2X = canvasShiftX << 8;
  work->transferMode =
      CANVAS_COPY_AND_FADE; // each transfer halves the canvas, for trails
  work->transferValue = 50; // percent
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);
  if (work->effect->side == 1)
    drawFlags = COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X;
  else
    drawFlags = COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y;
  AudioCommand_PlayFar(142);

  for (frame = 0; frame != work->effect->count * 8 + 108; frame++) {
    struct BattleCamera *camera = WORK_SLOT(HEAP_SLOT_CAMERA);

    if (frame == 80)
      BattleEventRuntime_BeginPhaseFar(0);

    // The djinni sways from side to side and, after 32 frames, rises 2 pixels
    // a frame. For the first 4 frames it is also added on top.
    if (work->effect->prelude == 1) {
      s32 x =
          ((Trig_Sin(frame << 11) * 20) >> 16) + originX + canvasShiftX - 20;
      s32 y = ((Trig_Cos(frame << 11) * 4) >> 16) + originY - 24;

      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, drawFlags ^ COMPOSITE_FLIP_X,
                          COMPOSITE_MAX);
      draw[0] = gWorkSlot[HEAP_SLOT_BLITTER];
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                          drawFlags ^ COMPOSITE_FLIP_X, COMPOSITE_ADDITIVE);
      draw[1] = gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
      if (frame > 32)
        y = y - frame * 2 + 64;
      draw[0](canvas, work->sheet + STAT_BOOST_DJINNI, x, y, 40, 40);
      if (frame <= 3)
        draw[1](canvas, work->sheet + STAT_BOOST_DJINNI, x, y, 40, 40);
      EFFECT_RELEASE_BLITTERS();
    }

    EFFECT_FOR_EACH_TARGET(member, work->effect) {
      struct MotionObject *object =
          GetBattleObjectSlotFar(work->effect->actors[member])->object;
      s32 height =
          Battle_GetObjectTableValueFar(work->effect->actors[member]) * 2 / 3;

      if (frame == member * 8 + 80)
        AudioCommand_PlayFar(212);
      EFFECT_VIEW_FROM_CAMERA(camera);
      // Centre on two thirds of the target's height.
      EFFECT_SET_VECTOR(point, object->x, height, object->z);
      SceneTransform_ApplyPosition(point);
      if (frame == member * 8 + 48)
        ObjectGroup_UpdateMembers(work->effect->actors[member],
                                  SPRITE_COLOR_WHITE, -1, member, 16);

      // Turn the flares and move them 2 pixels a frame towards the centre.
      if (frame > member * 8) {
        EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, drawFlags, COMPOSITE_MAX);
        draw[0] = gWorkSlot[HEAP_SLOT_BLITTER];
        EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE, drawFlags,
                            COMPOSITE_ADDITIVE);
        draw[1] = gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
        if (mode == 0) {
          SceneTransform_ApplyPitch(-frame << 10);
        } else if (mode == 1) {
        } else if (mode == 2) {
          SceneTransform_ApplyYaw(frame << 10);
        } else {
          SceneTransform_ApplyYaw(frame << 10);
          SceneTransform_ApplyRoll(frame << 10);
        }
        for (i = 0; i != STAT_BOOST_DRAWN; i++) {
          struct EffectStep *flare = &PARTICLES[member * STAT_BOOST_SLOTS + i];

          if (frame > member * 8 + i) {
            s32 xx = (flare->x >> 8) * (flare->x >> 8);
            s32 yy = (flare->y >> 8) * (flare->y >> 8);
            s32 zz = (flare->z >> 8) * (flare->z >> 8);
            s32 distance = Iwram_Sqrt(xx + yy + zz) >> 9;

            if (distance != 0) {
              s32 size;

              // The size (half the width) runs 6 to 1 over depths 314 to 634.
              EffectPosition_ApplyBaseAndYOffset(flare, &screen);
              screen.x = screen.x * 4 / 5 + canvasShiftX;
              EFFECT_CLAMP(screen.depth, 314, 634);
              size = 6 - (screen.depth - 314) / 64;
              EFFECT_DRAW_FLARE(draw[1], canvas, work->sheet, screen.x,
                                screen.y, size);
              flare->x -= flare->x / distance;
              flare->y -= flare->y / distance;
              flare->z -= flare->z / distance;
            }
          }
        }
        EFFECT_RELEASE_BLITTERS();
      }

      // The centre's position on screen.
      EFFECT_SET_VECTOR(center, 0, 0, 0);
      EffectPosition_ApplyBaseAndYOffset(center, &screen);
      screen.x = screen.x * 4 / 5 + canvasShiftX;

      // The burst: six frames of 4 frames each, from frame 52.
      if (frame >= member * 8 + 52 && frame < member * 8 + 76) {
        s32 cel = (frame - member * 8 - 52) / 4 % 6;

        EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, drawFlags, COMPOSITE_MAX);
        draw[0] = gWorkSlot[HEAP_SLOT_BLITTER];
        draw[0](canvas, work->sheet + cel * 1600 + STAT_BOOST_BURST,
                screen.x - 20, screen.y - 20, 40, 40);
        Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
      }

      // The mode's picture, from frame 80.
      if (mode == 0) {
        // Seven 24x40 frames, 4 frames each.
        if (frame >= member * 8 + 80 && frame < member * 8 + 108) {
          s32 cel = (frame - member * 8 - 80) / 4 % 7;

          EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, drawFlags, COMPOSITE_MAX);
          draw[0] = gWorkSlot[HEAP_SLOT_BLITTER];
          draw[0](canvas, work->sheet + cel * 960 + STAT_BOOST_PICTURE,
                  screen.x - 12, screen.y - 20, 24, 40);
          Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
        }
      } else if (mode == 3 || mode == 5) {
        // Six 32x64 frames, 4 frames each.
        if (frame >= member * 8 + 80 && frame < member * 8 + 104) {
          s32 cel = (frame - member * 8 - 80) / 4 % 6;

          EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, drawFlags, COMPOSITE_MAX);
          draw[0] = gWorkSlot[HEAP_SLOT_BLITTER];
          draw[0](canvas, work->sheet + cel * 2048 + STAT_BOOST_PICTURE,
                  screen.x - 16, screen.y - 32, 32, 64);
          Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
        }
      } else if (mode == 4) {
        // 64x32 frames, 2 frames each, mirrored below into 64x64.
        if (frame >= member * 8 + 80 && frame < member * 8 + 104) {
          s32 cel = (frame - member * 8 - 80) / 2 % 6;

          EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, drawFlags, COMPOSITE_MAX);
          draw[0] = gWorkSlot[HEAP_SLOT_BLITTER];
          EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                              drawFlags | COMPOSITE_FLIP_Y, COMPOSITE_MAX);
          draw[1] = gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
          draw[0](canvas, work->sheet + cel * 2048 + STAT_BOOST_PICTURE,
                  screen.x - 32, screen.y - 24, 64, 32);
          draw[1](canvas, work->sheet + cel * 2048 + STAT_BOOST_PICTURE,
                  screen.x - 32, screen.y + 8, 64, 32);
          EFFECT_RELEASE_BLITTERS();
        }
      } else {
        // Six 40x40 frames, 4 frames each, added.
        if (frame >= member * 8 + 80 && frame < member * 8 + 104) {
          s32 cel = (frame - member * 8 - 80) / 4 % 6;

          EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, drawFlags, COMPOSITE_ADDITIVE);
          draw[0] = gWorkSlot[HEAP_SLOT_BLITTER];
          draw[0](canvas, work->sheet + cel * 1600 + STAT_BOOST_PICTURE,
                  screen.x - 20, screen.y - 20, 40, 40);
          Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
        }
      }
    }
    EFFECT_END_FRAME(work);
  }

  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  BattleFx_EndCanvasLayer();
}

// Stat-reduction shards, flame arcs, rock pillars and erupting particles.
// Named table fields describe the counts and timing values used by each
// family's variant rows.
//
// A stat reduction shows the dagger or the shield over each target, 4 frames
// apart, and breaks it into 24 shards that fly off and darken through seven
// dimmed copies of the shard picture. Its shards live in PARTICLES, 64 for
// each target, and are drawn with trails: each transfer of the canvas fades
// or darkens what is already there.
//
// The scenes after it each throw a pattern of particles and play it out over
// a fixed number of frames:
//
// - Flare raises nine flames one after another along a half circle,
//   optionally stacked two or three high.
// - Quake raises three rock pillars that throw the targets into the air.
// - Fire's homing embers fly out of the user, hang, and then home in on the
//   targets, striking with a flare where they reach the ground.
// - Frost's rising motes spiral up around the target while ice columns spring
//   up, then burst: the motes spread and fall, chips fly and smoke rises over
//   the heights they span.
// - The blast (Blast, Smoke Bomb) bursts a fountain of motes and puffs of
//   smoke up beside the targets, which fall back under gravity.
//
// Each scene takes over the canvas layer, loads its sheet, seeds its
// particles and then runs its frames, each ending with a canvas present. A
// free or waiting particle is usually marked by a negative variant, which
// otherwise counts the frames of its animation. effect->variant picks each
// scene's counts from its timing table. Several steps consume random numbers,
// so their order within a frame is part of the effect.

// Abilities: Dull, Blunt (kind 27).
//
// Break the dagger picture into shards that fade through darker copies.
DEFINE_EFFECT_KIND(BattleFx_RunDull, BattleFx_RunStatReduction, 0)

// Abilities: Impair, Debilitate (kind 29); kind 47 is used by no ability.
//
// Break the shield picture into fading shards with style 1.
DEFINE_EFFECT_KIND(BattleFx_RunImpair, BattleFx_RunStatReduction, 1)

// Abilities: Weaken, Enfeeble (kind 31).
//
// Break the shield picture into fading shards with style 2.
DEFINE_EFFECT_KIND(BattleFx_RunWeaken, BattleFx_RunStatReduction, 2)

// Break the shield picture into fading shards with style 3.
DEFINE_EFFECT_KIND(BattleFx_RunStatReductionMode3, BattleFx_RunStatReduction, 3)

// The shard picture in the sheet, followed by seven copies of it, each
// with its colour levels capped 7 lower than the one before.
#define STAT_REDUCTION_SHARD_CELLS 0x2580
#define STAT_REDUCTION_SHARD_BYTES 696 // bytes in each copy
#define STAT_REDUCTION_SHARDS 24       // shards drawn for each target
#define STAT_REDUCTION_SLOTS 32        // particles reserved for each target

// Stat cuts: a dagger (mode 0) or a shield (other modes) appears over each
// target, 4 frames apart, and then breaks into 24 shards that fly off and
// fade out through the darker copies.
//
// effect: the targets.
// mode: 0 the dagger and the pink burst palette, 1 the shield and the
//       glow orb palette, 2 and 3 the shield and the pink burst palette.
void BattleFx_RunStatReduction(struct BattleEffectArgument *effect, s32 mode) {
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 j;
  s32 xOffset;
  DrawRectangle *draw;
  struct BattleCamera *camera;
  s32 origin[3];
  struct EffectPosition screen;
  s32 point[3];
  BlitterPair blitters;
  s32 i;
  s32 paletteId;

  cursor = gBattleFxWork;
  EFFECT_GET_WORK_AND_CANVAS(cursor, work, canvas);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);
  REG_BG2PA = Q_8_8(1);
  if (mode == 0)
    Resource_LoadAndDecompress((s32)&ResourceId_DaggerSheet, work->sheet, 1, 1);
  else
    Resource_LoadAndDecompress((s32)&ResourceId_ShieldSheet, work->sheet, 1, 1);
  if (mode == 0)
    paletteId = (s32)&ResourceId_PinkBurstSheet;
  else if (mode == 1)
    paletteId = (s32)&ResourceId_GlowOrbSheet;
  else
    paletteId = (s32)&ResourceId_PinkBurstSheet;
  Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(paletteId), 128);
  Resource_LoadAndDecompress((s32)&ResourceId_PaletteRampImage,
                             work->sheet + STAT_REDUCTION_SHARD_CELLS, 0, 0);

  // Build the seven darker copies of the shard picture.
  for (i = 1; i != 8; i++) {
    for (j = 0; j != STAT_REDUCTION_SHARD_BYTES; j++) {
      s32 level = work->sheet[STAT_REDUCTION_SHARD_CELLS + j];

      if (level > 64 - i * 7)
        level = 64 - i * 7;
      if (level < 0)
        level = 0;
      work->sheet[STAT_REDUCTION_SHARD_CELLS + i * STAT_REDUCTION_SHARD_BYTES +
                  j] = level;
    }
  }

  // On side 1 the canvas moves 112 pixels right, so everything is drawn
  // 112 pixels further left on it.
  if (work->effect->side == 1) {
    REG_BG2X = -0x7000;
    xOffset = -112;
  } else {
    REG_BG2X = 0;
    xOffset = 0;
  }

  // The shards start in a grid of eight rows and four columns and fly
  // away from the user's side, scattering in a random direction.
  for (i = 0; i != 512; i++) {
    struct EffectStep *shard = &PARTICLES[i];
    s32 radius = 192;
    s32 angle;

    angle = Random16() & 0xffff;
    shard->x = 0;
    if (mode == 0) {
      shard->y = ((i & 31) / 4 * 6 - 10) << 16;
      shard->z = (i % 4 * 2 - 2) << 16;
    } else {
      shard->y = ((i & 31) / 4 * 6 - 10) << 16;
      shard->z = (i % 4 * 8 - 16) << 16;
    }
    if (work->effect->side == 1)
      shard->velocityX = 0x20000;
    else
      shard->velocityX = -0x20000;
    shard->velocityY = ((Trig_Cos(angle) * radius) >> 6) + 0x10000;
    shard->velocityZ = (Trig_Sin(angle) * radius) >> 6;
    shard->variant = Random16() & 0xff;
  }

  BattleFx_FetchRectangleBlitters(work->effect->side, draw = blitters);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 50);

  for (frame = 0; frame != work->effect->count * 4 + 64; frame++) {
    camera = gCameraWork;
    if (frame == 72)
      BattleEventRuntime_BeginPhaseFar(0);
    EFFECT_FOR_EACH_TARGET(j, work->effect) {
      struct MotionObject *object;
      s32 targetFrame;
      s32 x;
      s32 y;

      targetFrame = frame - j * 4;
      object = GetBattleObjectSlotFar(work->effect->actors[j])->object;
      if (targetFrame > 0) {
        EFFECT_VIEW_FROM_CAMERA(camera);
        EFFECT_SET_VECTOR(point, object->x, 160 << 13, object->z);
        EFFECT_VIEW_FROM_CAMERA(camera);
        SceneTransform_ApplyPosition(point);
        EFFECT_SET_VECTOR(origin, 0, 0, 0);
        EffectPosition_ApplyBaseAndYOffset(origin, &screen);
        x = screen.x + xOffset;
        y = screen.y;
        if (mode == 0) {
          if (targetFrame <= 26)
            blitters[0](canvas, work->sheet + targetFrame / 4 % 7 * 960, x - 12,
                        y - 20, 24, 40);
        } else {
          if (targetFrame <= 23)
            draw[1](canvas, work->sheet + targetFrame / 4 % 6 * 1600, x - 20,
                    y - 20, 40, 40);
        }
        if (targetFrame == 24)
          AudioCommand_PlayFar(143);
        // From frame 24 the picture breaks; the shards darken one copy
        // every 4 frames after frame 28.
        if (targetFrame >= 24 && targetFrame <= 60) {
          s32 level;

          level = 0;
          if (targetFrame > 28) {
            level = (targetFrame - 24) / 4;
            if (level > 7)
              level = 7;
          }
          for (i = 0; i != STAT_REDUCTION_SHARDS; i++) {
            struct EffectStep *shard = &PARTICLES[j * STAT_REDUCTION_SLOTS + i];
            s32 cell;

            cell = i % 4 * 3 + (shard->variant + targetFrame) / 8 % 3;
            EffectPosition_ApplyBaseAndYOffset(shard, &screen);
            x = screen.x + xOffset;
            y = screen.y;
            blitters[0](canvas,
                        work->sheet +
                            (level * STAT_REDUCTION_SHARD_BYTES +
                             gPaletteRamp_ShardCells[cell]) +
                            STAT_REDUCTION_SHARD_CELLS,
                        x, y, gPaletteRamp_ShardWidths[cell],
                        gPaletteRamp_ShardHeights[cell]);
            EffectStep_AdvanceWithGravity3D(shard, 60, 0);
          }
        }
      }
    }
    EFFECT_PRESENT_FRAME(work);
  }

  EFFECT_FINISH_CANVAS();
}

#define FLAME_ARC_COUNT 9 // flames on the arc

// Abilities: Flare, Flare Wall, Flare Storm (kind 11).
//
// Flame arc: nine flames rise one after another, 4 frames apart, along a
// half circle, over 80 frames. Each target is hit in turn, 8 frames apart
// from frame 16.
//
// effect: the targets. variant 1 adds a second flame 16 pixels above each
//         one, variant 2 a third 32 pixels above (and longer trails).
void BattleFx_RunFlare(struct BattleEffectArgument *effect) {
  u32 *slots;
  u32 *cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle draw;
  s32 i;
  s32 cell;
  u32 width;
  s32 direction;
  s32 originX;
  s32 angle;
  s32 delay;
  struct EffectStep *arcFlame;
  struct EffectStep *flame;

  // The work, the canvas, and BG1 and BG2 blended at 14:16.
  slots = (u32 *)&gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];
  cursor = slots;
  work = (struct BattleEffectWork *)*cursor++;
  canvas = (void *)*cursor;
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);
  REG_BLDCNT =
      BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BLDALPHA = BLEND_ALPHA(14, 16);
  Resource_LoadAndDecompress((s32)&ResourceId_EmberStreakSheet, work->sheet, 1,
                             1);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                      COMPOSITE_ADDITIVE);
  draw = (DrawRectangle)EFFECT_SLOT(slots, HEAP_SLOT_BLITTER);

  // The arc: from -90 degrees in steps of 22.5, 32 pixels across and 16
  // high. Against enemies (ids from 128) it runs from the left, otherwise
  // from the right.
  if (work->effect->actors[0] > 127) {
    originX = 0;
    direction = 1;
  } else {
    originX = 64;
    direction = -1;
  }
  angle = -0x4000;
  i = 0;
  delay = 0;
  arcFlame = work->particles;
  do {
    arcFlame->x =
        ((direction * (((Trig_Sin(angle) * 32)) >> 16)) + originX) + 20;
    arcFlame->y = (((Trig_Cos(angle) * 16)) >> 16) + 40;
    arcFlame->variant = delay;
    angle += 0x1000;
    delay -= 4;
    i += 1;
    arcFlame++;
  } while (i != FLAME_ARC_COUNT);

  work->transferMode = CANVAS_COPY_AND_FADE;
  if (work->effect->variant == 2) {
    work->transferValue = 75;
  } else {
    work->transferValue = 50;
  }
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);
  AudioCommand_PlayFar(136);
  frame = 0;
  do {
    if (frame == 24) {
      BattleEventRuntime_BeginPhaseFar(133);
    }

    // Each flame shows six pictures, 4 frames each, and goes out.
    i = 0;
    flame = work->particles;
    do {
      if ((flame->variant >= 0) && (flame->variant <= 23)) {
        cell = flame->variant / 4;
// Sequence dimensions before the call; assigning and reading them in
// separate arguments was UB. Plain English trial: unchanged 0x284 bytes,
// with different instructions. Preserve the original native calls.
#ifdef UBFIX
        width = gBattleFx_PuffArcCellWidths[cell];
        draw(canvas, work->sheet + gBattleFx_PuffArcCellSourceOffsets[cell],
             flame->x - (width >> 1),
             flame->y + gBattleFx_PuffArcCellBiasY[cell], width,
             gBattleFx_PuffArcCellHeights[cell]);
#else
        draw(canvas, work->sheet + gBattleFx_PuffArcCellSourceOffsets[cell],
             flame->x - ((width = gBattleFx_PuffArcCellWidths[cell]) >> 1),
             flame->y + gBattleFx_PuffArcCellBiasY[cell], width,
             gBattleFx_PuffArcCellHeights[cell]);
#endif
        if (work->effect->variant != 0) {
#ifdef UBFIX
          width = gBattleFx_PuffArcCellWidths[cell];
          draw(canvas, work->sheet + gBattleFx_PuffArcCellSourceOffsets[cell],
               flame->x - (width >> 1),
               (flame->y + gBattleFx_PuffArcCellBiasY[cell]) - 16, width,
               gBattleFx_PuffArcCellHeights[cell]);
#else
          draw(canvas, work->sheet + gBattleFx_PuffArcCellSourceOffsets[cell],
               flame->x - ((width = gBattleFx_PuffArcCellWidths[cell]) >> 1),
               (flame->y + gBattleFx_PuffArcCellBiasY[cell]) - 16, width,
               gBattleFx_PuffArcCellHeights[cell]);
#endif
        }
        if (work->effect->variant == 2) {
#ifdef UBFIX
          width = gBattleFx_PuffArcCellWidths[cell];
          draw(canvas, work->sheet + gBattleFx_PuffArcCellSourceOffsets[cell],
               flame->x - (width >> 1),
               (flame->y + gBattleFx_PuffArcCellBiasY[cell]) - 32, width,
               gBattleFx_PuffArcCellHeights[cell]);
#else
          draw(canvas, work->sheet + gBattleFx_PuffArcCellSourceOffsets[cell],
               flame->x - ((width = gBattleFx_PuffArcCellWidths[cell]) >> 1),
               (flame->y + gBattleFx_PuffArcCellBiasY[cell]) - 32, width,
               gBattleFx_PuffArcCellHeights[cell]);
#endif
        }
      }
      i += 1;
      flame->variant += 1;
      flame++;
    } while (i != FLAME_ARC_COUNT);

    i = 0;
    while (i != work->effect->count) {
      if (frame == (i * 8) + 16) {
        ObjectGroup_UpdateMembers(work->effect->actors[i],
                                  SPRITE_COLOR_RED_TINT, POSE_DAMAGE, i, 12);
      }
      i += 1;
    }
    EFFECT_END_FRAME(work);
    frame += 1;
  } while (frame != 80);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  BattleFx_EndCanvasLayer();
}

// List the indices within indexRadius of where currentValue sits in values,
// keeping only those from 0 to entryLimit - 1. Writes the indices to output
// and returns how many there are.
UNUSED s32 BattleFx_BuildCenteredIndexList(BattleEffectIndexWindow *window,
                                           s16 *output) {
  s32 windowLength = window->indexRadius * 2 + 1;
  s32 outputCount = 0;
  s32 centerIndex = 0;
  s32 windowIndex;

  for (windowIndex = 0; windowIndex != windowLength; windowIndex++) {
    if (window->currentValue == window->values[windowIndex]) {
      centerIndex = windowIndex;
      break;
    }
  }
  for (windowIndex = 0; windowIndex != windowLength; windowIndex++) {
    s32 index = centerIndex + windowIndex - window->indexRadius;

    if (index >= 0 && index < window->entryLimit) {
      output[outputCount++] = index;
    }
  }
  return outputCount;
}

// The columns of a gQuake_Timings row.
enum {
  TIMING_FRAMES,      // frames in the whole effect
  TIMING_RISE_FRAMES, // frames each pillar stands
  TIMING_SHAKE        // how far the screen shakes
};

struct RockWallHeights {
  s32 column[4]; // each pillar's height in pixels
};

#define ROCK_PILLAR_WIDTH 40 // a pillar's width in pixels
// The rock picture in the sheet; the sheet starts with the 40x16 strip of
// ground cut from the background.
#define ROCK_WALL_CELLS 0x1e00

// The offset of pixel (x, y) in 8bpp tiles laid out 32 tiles across.
#define ROCK_WALL_PIXEL_OFFSET(x, y)                                           \
  (((x) & 7) + (((x) / 8) << 6) + (((y) & 7) << 3) + (((y) / 8) << 11))

// Abilities: Quake, Earthquake, Quake Sphere (kind 1).
//
// Rock wall: three rock pillars rise from the left, 4 frames apart, and
// sink again, throwing the targets standing on them into the air. Each
// pillar is capped with a strip of ground cut from the background.
//
// effect: the targets; variant picks the row of gQuake_Timings.
void BattleFx_RunQuake(struct BattleEffectArgument *effect) {
  static s32 wallHeights[4] = {
      -32,
      -32,
      -32,
      -32,
  };
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 column;
  DrawRectangle draw;
  struct BattleCamera *camera;
  s32 xOffset;
  struct RockWallHeights heights;
  s32 point[3];
  struct EffectPosition screen;
  s32 member;
  s32 row;
  s32 col;
  s32 y;

  slots = &gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];
  cursor = slots;
  EFFECT_GET_WORK_AND_CANVAS(cursor, work, canvas);
  camera = *(struct BattleCamera **)((u8 *)gWorkSlot + HEAP_SLOT_CAMERA * 4);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(1);
  REG_BG2PA = Q_8_8(1); // the canvas at its own width
  REG_BLDCNT = 0;    // no blending
  Resource_LoadAndDecompress((s32)&ResourceId_RockWallSheet,
                             work->sheet + ROCK_WALL_CELLS, 1, 1);

  // Copy the background tiles (0x8000 bytes from VRAM 0x06008000) and cut
  // the strip at x 32 to 71, y 96 to 111 into the riseFrame of the sheet.
  Iwram_CopyWords(Ram_MapCellBuffer, BG_CHAR_BLOCK(2), 0x8000);
  for (row = 0; row != 16; row++) {
    y = row + 96;
    for (col = 0; col != ROCK_PILLAR_WIDTH; col++) {
      s32 pixel = ROCK_WALL_PIXEL_OFFSET(col + 32, y);

      work->sheet[row * ROCK_PILLAR_WIDTH + col] = Ram_MapCellBuffer[pixel];
    }
  }

  // On side 1 the canvas moves 112 pixels right, and the targets' positions
  // with it.
  if (work->effect->side == 1) {
    REG_BG2X = -(112 << 8);
    xOffset = -112;
  } else {
    xOffset = 0;
  }
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_TRANSPARENT);
  draw = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
  EFFECT_FOR_EACH_TARGET(member, work->effect)
    work->particles[member].variant = 0;      // 1 once the target has landed
  work->transferMode = CANVAS_COPY_AND_CLEAR; // each transfer clears the canvas
  work->transferValue = 0;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);
  heights = (*(const struct RockWallHeights *)&wallHeights);
  work->shakeFrames = 128;
  AudioCommand_PlayFar(141);

  for (frame = 0; frame != gQuake_Timings[work->effect->variant][TIMING_FRAMES];
       frame++) {
    EFFECT_VIEW_FROM_CAMERA(camera);
    if (frame == gQuake_Timings[work->effect->variant][TIMING_FRAMES] - 16)
      BattleEventRuntime_BeginPhaseFar(
          133); // 16 frames before the end, move on to the results

    // The pillars rise from frames 16, 20 and 24.
    for (column = 0; column != 3; column++) {
      s32 riseFrame = column * 4 + 16;

      if ((frame & 31) + 32 ==
          riseFrame) // never true: the left side is 32 or more
        heights.column[column] += 32;
      if (frame >= riseFrame &&
          frame <
              riseFrame +
                  gQuake_Timings[work->effect->variant][TIMING_RISE_FRAMES]) {
        s32 height;
        s32 top;

        // The height rises and falls along a sine, 1/64 of a turn a frame.
        height =
            (heights.column[column] * Trig_Sin((frame - riseFrame) << 10)) >>
            16;
        if (height < 0)
          height = -height;
        top = 112 - height;
        draw(canvas, work->sheet + ROCK_WALL_CELLS,
             column * ROCK_PILLAR_WIDTH + 8, top, ROCK_PILLAR_WIDTH, height);
        draw(canvas, work->sheet, column * ROCK_PILLAR_WIDTH + 8, 96 - height,
             ROCK_PILLAR_WIDTH, 16);

        // Throw up the targets standing on the pillar.
        EFFECT_FOR_EACH_TARGET(member, work->effect) {
          struct MotionObject *object;

          object = GetBattleObjectSlotFar(work->effect->actors[member])->object;
          EFFECT_SET_VECTOR(point, object->x, object->y, object->z);
          EffectPosition_ApplyBaseAndYOffset(point, &screen);
          screen.x += xOffset;
          if (screen.x >= column * ROCK_PILLAR_WIDTH + 8 &&
              screen.x <= column * ROCK_PILLAR_WIDTH + 8 + ROCK_PILLAR_WIDTH &&
              screen.y >= top) {
            object->velocityY = 12 << 16; // 12 pixels a frame upwards
            object->verticalMotionStrength = 0xab85;
          }
          if (object->y < 0)
            ObjectGroup_UpdateMembers(work->effect->actors[member], 0,
                                      POSE_DAMAGE, -1, 0);
        }
      }
    }

    // A target that lands flashes white for 5 frames.
    EFFECT_FOR_EACH_TARGET(member, work->effect) {
      struct MotionObject *object;
      struct EffectStep *state;

      object = GetBattleObjectSlotFar(work->effect->actors[member])->object;
      state = &work->particles[member];
      if (state->variant == 0 && object->y <= 0 && object->velocityY < 0) {
        state->variant = 1;
        ObjectGroup_UpdateMembers(work->effect->actors[member],
                                  SPRITE_COLOR_WHITE, POSE_DAMAGE, member, 5);
      }
    }
    Camera_ApplyShake(gQuake_Timings[work->effect->variant][TIMING_SHAKE],
                      gQuake_Timings[work->effect->variant][TIMING_SHAKE]);
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

// Abilities: Fire, Fireball, Inferno (kind 12).
//
// Homing embers: embers burst from the user two a frame, turn towards the
// targets 48 frames after they appear, and each one that hits the ground
// shows an impact flare for 12 frames.
//
// effect: the targets; variant picks the number of embers
//         (gFire_Counts).
void BattleFx_RunFire(struct BattleEffectArgument *effect) {
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  struct BattleCamera *camera;
  u8 *sheet;
  s32 bodyTop;
  struct EffectPosition screen;
  BlitterPair draw;
  struct MotionObject *user;
  s32 i;

  slots = &((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BATTLE_EFFECT];
  cursor = slots;
  EFFECT_GET_WORK_AND_CANVAS(cursor, work, canvas);
  camera = *(struct BattleCamera **)((u8 *)gWorkSlot + HEAP_SLOT_CAMERA * 4);
  sheet = EFFECT_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  work->effect = effect;
  if (effect->side == 1)
    BattleFx_BeginCanvasLayer(1);
  else
    BattleFx_BeginCanvasLayer(0);
  Resource_LoadAndDecompress((s32)&ResourceId_EmberStreakSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                      COMPOSITE_ADDITIVE);
  draw[0] =
      (DrawRectangle)((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BLITTER];
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                      COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, COMPOSITE_MAX);
  draw[1] = (DrawRectangle)((union HeapState *)gWorkSlot)
                ->slots[HEAP_SLOT_BLITTER_ALTERNATE];
  REG_BLDALPHA = BLEND_ALPHA(16, 16);

  // The embers leave the bodyTop of the user's body in a random direction and
  // speed, rising at -16 to 111 sixty-fourths of a pixel a frame.
  user = GetBattleObjectSlotFar(work->effect->actor)->object;
  bodyTop = user->y + Battle_GetObjectTableValueFar(work->effect->actor);
  for (i = 0; i != 64; i++) {
    struct EffectStep *ember = &PARTICLES[i];
    s32 angle;
    s32 speed;

    angle = Random16();
    speed = (Random16() & 127) + 127;
    EFFECT_SET_VELOCITY(ember, (Trig_Sin(angle) * speed) >> 6,
                        (s32)(((Random16() & 127) - 16) << 16) >> 6,
                        (Trig_Cos(angle) * speed) >> 6);
    EFFECT_SET_POSITION(ember, user->x, bodyTop, user->z);
    ember->variant = -1; // -1 in flight, then the frames of the impact flare
  }

  work->cameraDelta = 0;
  work->cameraPhase = 0;
  Scheduler_AddOrUpdateCallback(Camera_ApplyPhasedDelta, 0x480);
  work->transferMode = CANVAS_COPY_AND_FADE;
  work->transferValue = 75; // each transfer fades the canvas to 3/4
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);

  for (frame = 0; frame != (gFire_Counts[work->effect->variant] >> 1) + 132;
       frame++) {
    if (frame > 16 && frame < 80)
      work->cameraDelta = 0x100;
    else
      work->cameraDelta = 0;
    if (frame == (gFire_Counts[work->effect->variant] >> 1) + 108)
      BattleEventRuntime_BeginPhaseFar(133);
    BattlePres_SetupTransitionSceneFar(0, 0, 0, 100);
    EFFECT_VIEW_FROM_CAMERA(camera);

    // The embers in flight.
    for (i = 0; i != gFire_Counts[work->effect->variant]; i++) {
      struct EffectStep *ember = &PARTICLES[i];
      s32 launchFrame;

      launchFrame = i / 2;
      if (frame > launchFrame && ember->variant == -1) {
        s32 size;

        // Size 10 to 1 over depths 160 to 799; maximum blend for the first 48
        // frames, additive after.
        EffectPosition_ApplyBaseAndYOffset(ember, &screen);
        screen.x >>= 1;
        EFFECT_CLAMP(screen.depth, 160, 799);
        size = 10 - (screen.depth - 160) / 64;
        draw[frame < launchFrame + 48 ? 1 : 0](
            canvas, sheet + gBattleFx_MoteCellOffsets[size - 1],
            screen.x - size / 2, screen.y - size, size, size * 2);
        ember->x += ember->velocityX;
        ember->y += ember->velocityY;
        ember->z += ember->velocityZ;
      }

      // Accelerate towards the target by 1/512 of the distance, damping the
      // speed to 60/64 until 85 frames after the ember appeared.
      if (frame > launchFrame + 48 && ember->variant == -1) {
        struct MotionObject *target;
        s32 vx;
        s32 vy;
        s32 vz;

        target = GetBattleObjectSlotFar(
                     work->effect->actors[i % work->effect->count])
                     ->object;
        vx = ember->velocityX += (target->x - ember->x) >> 9;
        vy = ember->velocityY += (target->y - ember->y) >> 9;
        vz = ember->velocityZ += (target->z - ember->z) >> 9;
        if (frame < launchFrame + 85) {
          EFFECT_SET_VELOCITY(ember, vx * 60 / 64, vy * 60 / 64, vz * 60 / 64);
        }

        // Below the ground it hits: keep its screen position for the impact
        // flare, tint the target red in its damage pose for 4 frames, and
        // shake the screen for 2 frames.
        if (ember->y < 0) {
          ember->variant = 0;
          ember->x = screen.x;
          ember->y = screen.y;
          AudioCommand_PlayFar(136);
          ObjectGroup_UpdateMembers(
              work->effect->actors[i % work->effect->count],
              SPRITE_COLOR_RED_TINT, POSE_DAMAGE, i % work->effect->count, 4);
          work->shakeFrames = 2;
        }
      }
    }

    // The impact flares: six pictures, 2 frames each.
    for (i = 0; i != gFire_Counts[work->effect->variant]; i++) {
      struct EffectStep *ember = &PARTICLES[i];

      if (ember->variant >= 0 && ember->variant < 12) {
        s32 cel;
        u32 width;

        cel = ember->variant / 2;
// Sequence dimensions before the call; assigning and reading them in
// separate arguments was UB. Plain English trial: unchanged 0x4a8 bytes,
// with different instructions. Preserve the original native calls.
#ifdef UBFIX
        width = gFire_FlareWidths[cel];
        draw[1](canvas, work->sheet + gFire_FlareCells[cel],
                ember->x - (width >> 1), ember->y + gFire_FlareBiasY[cel] - 56,
                width, gFire_FlareHeights[cel]);
#else
        draw[1](canvas, work->sheet + gFire_FlareCells[cel],
                ember->x - ((width = gFire_FlareWidths[cel]) >> 1),
                ember->y + gFire_FlareBiasY[cel] - 56, width,
                gFire_FlareHeights[cel]);
#endif
        ember->variant++;
      }
    }

    if (work->cameraPhase == 0)
      work->cameraPhase = 1;
    Camera_ApplyShake(8, 8);
    EFFECT_END_FRAME(work);
  }

  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  Scheduler_RemoveCallback(Camera_ApplyPhasedDelta);
  EFFECT_RELEASE_BLITTERS();
  BattleFx_EndCanvasLayer();
}

// One row of gRisingMotes_Timings, for each variant.
enum {
  RISE_MOTES,   // the rising motes
  RISE_COLUMNS, // the ice columns
  RISE_FRAME    // the frame the motes burst
};

// The value of one column of this effect's gRisingMotes_Timings row.
#define RISING_MOTE_TIMING(field)                                              \
  (gRisingMotes_Timings[work->effect->variant * 3 + (field)])

// gRisingMotes_ColumnSpots has an x and a foot y for each column.
#define RISING_MOTES_COLUMN_X(column) gRisingMotes_ColumnSpots[(column) * 2]
#define RISING_MOTES_COLUMN_FOOT(column)                                       \
  gRisingMotes_ColumnSpots[(column) * 2 + 1]

// Abilities: Frost, Tundra, Glacier (kind 6).
//
// Rising motes: motes spiral upwards while ice columns spring up one every
// 3 frames; at the burst frame the motes spread and fall, chips fly and
// smoke rises.
//
// effect: the targets; variant picks the number of motes and columns and
//         the burst frame (gRisingMotes_Timings).
void BattleFx_RunFrost(struct BattleEffectArgument *effect) {
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  u8 *puffSheet;
  DrawRectangle *draw;
  s32 count;
  s32 lowest;
  s32 highest;
  s32 xyz[3];
  struct EffectPosition screen;
  s32 point[3];
  BlitterPair pair;
  struct EffectStep *mote;
  s32 i;

  slots = &gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];
  cursor = slots;
  EFFECT_GET_WORK_AND_CANVAS(cursor, work, canvas);
  puffSheet = EFFECT_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(1);
  REG_BG2PA = Q_8_8(1);
  Resource_LoadAndDecompress((s32)&ResourceId_IceChipSheet, work->sheet, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_SparkleDots, puffSheet, 0, 0);
  BattleFx_FetchRectangleBlitters(0, draw = pair);

  // Motes: x holds the angle, z the radius (56 to 119), y the height (-64
  // to -33 pixels).
  for (i = 0; i != 64; i++) {
    struct EffectStep *seed = &work->particles[i];

    seed->x = Random16() & 0xffff;
    seed->z = (Random16() & 63) + 56;
    seed->y = ((Random16() & 31) - 64) << 16;
  }

  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 50);
  if (work->effect->side == 1)
    REG_BG2X = -0x7000;

  for (frame = 0; frame != RISING_MOTE_TIMING(RISE_FRAME) + 75; frame++) {
    lowest = 120 << 16;
    highest = 0;
    if (frame == RISING_MOTE_TIMING(RISE_FRAME) + 11)
      BattleEventRuntime_BeginPhaseFar(132);
    EFFECT_SET_VECTOR(point, 0, 0, 512 << 16);
    Render_ResetTransformState();
    SceneTransform_ApplyPosition(point);
    if (frame >= 36 && frame < 64 && (frame & 3) == 0)
      AudioCommand_PlayFar(115);
    if (frame == 85)
      AudioCommand_PlayFar(136);
    EFFECT_FOR_EACH_TARGET(i, work->effect) {
      if (frame == i * 4 + 40)
        ObjectGroup_UpdateMembers(work->effect->actors[i],
                                  SPRITE_COLOR_BLUE_TINT, POSE_DAMAGE, -1, 0);
    }

    // Motes: until the burst they turn, closing in and rising 5 pixels a
    // frame up to 48 pixels; after it they spread out and fall, and the range
    // of their heights is kept.
    if (frame < RISING_MOTE_TIMING(RISE_FRAME))
      count = RISING_MOTE_TIMING(RISE_MOTES);
    else
      count = 16;
    if (frame < RISING_MOTE_TIMING(RISE_FRAME) + 35) {
      for (i = 0; i != count; i++) {
        if (frame > i) {
          s32 cel = i % 8;

          mote = &work->particles[i];
          if (mote->y < (48 - i / 2) << 16 && mote->y > -(48 << 16)) {
            struct EffectPosition *position;
            u32 width;
            u32 height;

            EFFECT_SET_VECTOR(xyz, mote->z * Trig_Sin(mote->x), mote->y,
                              mote->z * Trig_Cos(mote->x));
            EffectPosition_ApplyBaseAndYOffset(xyz, position = &screen);
            position->x = (position->x >> 17) + 64;
            position->y = HI(position->y) + 60;
// Sequence dimensions before the call; assigning and reading them in
// separate arguments was UB. Plain English trial: 0x6f0 to 0x6d4 bytes,
// with different instructions. Preserve the original native calls.
#ifdef UBFIX
            width = gRisingMotes_MoteWidths[cel];
            height = gRisingMotes_MoteHeights[cel];
            draw[1](canvas, work->sheet + gRisingMotes_MoteCells[cel],
                    position->x - (width >> 1), position->y - (height >> 1),
                    width, height);
#else
            draw[1](canvas, work->sheet + gRisingMotes_MoteCells[cel],
                    position->x - ((width = gRisingMotes_MoteWidths[cel]) >> 1),
                    position->y -
                        ((height = gRisingMotes_MoteHeights[cel]) >> 1),
                    width, height);
#endif
          }
          if (frame < RISING_MOTE_TIMING(RISE_FRAME)) {
            if (frame > i + 16) {
              if (mote->z > 4)
                mote->z -= 2;
              if (mote->y < 48 << 16)
                mote->y += 5 << 16;
              mote->x += 0x200;
            }
          } else {
            mote->z += 8;
            mote->y -= (i % 5 + 2) << 16;
            if (lowest > mote->y)
              lowest = mote->y;
            if (highest < mote->y)
              highest = mote->y;
          }
        }
      }
    }
    lowest += 64 << 16;
    highest += 64 << 16;

    // Columns (32 wide): one more every 3 frames from frame 36.
    if (frame < RISING_MOTE_TIMING(RISE_FRAME)) {
      for (i = 0; i != RISING_MOTE_TIMING(RISE_COLUMNS); i++) {
        if (i < (frame - 36) / 3) {
          s32 cel = i % 3;
          u32 height;

          if (frame >= RISING_MOTE_TIMING(RISE_FRAME) - 7) {
#ifdef UBFIX
            height = gRisingMotes_ColumnHeights[cel];
            draw[1](canvas, work->sheet + gRisingMotes_ColumnCells[cel],
                    RISING_MOTES_COLUMN_X(i),
                    RISING_MOTES_COLUMN_FOOT(i) - height, 32, height);
#else
            draw[1](canvas, work->sheet + gRisingMotes_ColumnCells[cel],
                    RISING_MOTES_COLUMN_X(i),
                    RISING_MOTES_COLUMN_FOOT(i) -
                        (height = gRisingMotes_ColumnHeights[cel]),
                    32, height);
#endif
          } else {
#ifdef UBFIX
            height = gRisingMotes_ColumnHeights[cel];
            pair[0](canvas, work->sheet + gRisingMotes_ColumnCells[cel],
                    RISING_MOTES_COLUMN_X(i),
                    RISING_MOTES_COLUMN_FOOT(i) - height, 32, height);
#else
            pair[0](canvas, work->sheet + gRisingMotes_ColumnCells[cel],
                    RISING_MOTES_COLUMN_X(i),
                    RISING_MOTES_COLUMN_FOOT(i) -
                        (height = gRisingMotes_ColumnHeights[cel]),
                    32, height);
#endif
          }
        }
      }
    }

    // Chips: at the burst, 32 fly up from low down (24 of them are drawn).
    if (frame == RISING_MOTE_TIMING(RISE_FRAME)) {
      for (i = 0; i != 32; i++) {
        struct EffectStep *chip = &PARTICLES[i];

        EFFECT_SET_POSITION(chip, (Random16() & 127) << 16,
                            ((Random16() & 15) + 80) << 16,
                            ((Random16() & 63) - 32) << 12);
        chip->velocityY = ((-Random16() & 15) - 16) << 13;
        chip->variant = (Random16() & 15) + 16;
      }
    }
    if (frame >= RISING_MOTE_TIMING(RISE_FRAME)) {
      for (i = 0; i != 24; i++) {
        struct EffectStep *chip = &PARTICLES[i];

        if (chip->variant >= 0) {
          s32 cel = i % 8;

          draw[1](canvas, work->sheet + gRisingMotes_EmberCells[cel],
                  HI(chip->x), HI(chip->y), gRisingMotes_EmberWidths[cel],
                  gRisingMotes_EmberHeights[cel]);
          chip->x += chip->velocityX;
          chip->y += chip->velocityY;
          chip->variant--;
        }
        if (lowest > chip->y)
          lowest = chip->y;
        if (highest < chip->y)
          highest = chip->y;
      }
    }
    lowest >>= 16;
    highest >>= 16;
    if (highest <= lowest)
      highest = lowest + 1;

    // Smoke: puffs appear over the heights of the motes and chips and rise
    // half a pixel a frame.
    if (frame == RISING_MOTE_TIMING(RISE_FRAME)) {
      for (i = 0; i != 32; i++) {
        struct EffectStep *puff = &work->particles[i];

        puff->velocityX = (Random16() & 127) << 16;
        if (highest == lowest)
          puff->velocityY = lowest << 16;
        else
          puff->velocityY = (Random16() % (highest - lowest) + lowest) << 16;
        puff->variant = (Random16() & 15) + 20;
      }
    }
    if (frame >= RISING_MOTE_TIMING(RISE_FRAME)) {
      s32 rise = (frame - RISING_MOTE_TIMING(RISE_FRAME)) / 2;

      for (i = 0; i != 32; i++) {
        struct EffectStep *puff = &work->particles[i];

        if (puff->variant >= 0 && puff->variant <= 17) {
          s32 cel = (17 - puff->variant) / 2;
          u32 size;

#ifdef UBFIX
          size = gBattleFx_PuffSizes[cel];
          draw[1](canvas, puffSheet + gBattleFx_PuffCells[cel],
                  HI(puff->velocityX) - (size >> 1),
                  HI(puff->velocityY) - (size >> 1) - rise, size, size);
#else
          draw[1](canvas, puffSheet + gBattleFx_PuffCells[cel],
                  HI(puff->velocityX) -
                      ((size = gBattleFx_PuffSizes[cel]) >> 1),
                  HI(puff->velocityY) - (size >> 1) - rise, size, size);
#endif
        }
        puff->variant--;
        if ((puff->variant == -1 || puff->variant == 17) &&
            frame < RISING_MOTE_TIMING(RISE_FRAME) + 35) {
          puff->variant = 17;
          puff->velocityX = (Random16() & 127) << 16;
          puff->velocityY = (Random16() % (highest - lowest) + lowest) << 16;
        }
      }
    }
    EFFECT_PRESENT_FRAME(work);
  }

  EFFECT_RELEASE_BLITTERS();
  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  BattleFx_EndCanvasLayer();
}

// Closing bolts: four lightning bolts that close in from around the
// target, and the sparks they throw.

#define CLOSING_BOLT_SPARK_COUNT 1024

#define CLOSING_BOLT_CELLS 0x60e // the bolt pictures in the sheet
#define CLOSING_BOLT_COUNT 4

// Abilities: Ray, Storm Ray, Destruct Ray (kind 17).
//
// Four bolts turn in towards the centre for 16 frames, scattering sparks
// from their roots; the sparks fly on until frame 64.
//
// effect: the targets. variant 2 shows the canvas at double width;
//         variant 0 draws every bolt with the maximum blend, the others
//         alternate it with additive. The variant also sets the sparks
//         each bolt throws a frame (gLightningBolts_Sparks).
void BattleFx_RunRay(struct BattleEffectArgument *effect) {
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  u8 *sheet;
  s32 bolt;
  s32 thrown;
  BlitterPair draw;
  s32 i;

  // The work, the canvas, and BG2's scale and position.
  slots = &gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];
  cursor = slots;
  EFFECT_GET_WORK_AND_CANVAS(cursor, work, canvas);
  sheet = EFFECT_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  work->effect = effect;
  BattleFx_BeginTiledCanvas(1);
  if (work->effect->variant == 2)
    REG_BG2PA = Q_8_8(0.5); // double width
  else
    REG_BG2PA = Q_8_8(1);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS, COMPOSITE_MAX);
  draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                      COMPOSITE_ADDITIVE);
  draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
  Resource_LoadAndDecompress((s32)&ResourceId_LightningBoltSheet,
                             work->sheet + CLOSING_BOLT_CELLS, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  if (work->effect->variant == 2) {
    if (work->effect->side == 1)
      REG_BG2X = -0x1000;
    else
      REG_BG2X = 0x1000;
  } else {
    if (work->effect->side == 1)
      REG_BG2X = -0x8000;
  }

  // Set up the sparks.
  for (i = 0; i != CLOSING_BOLT_SPARK_COUNT; i++) {
    struct EffectStep *spark = &PARTICLES[i];
    s32 speed;
    s32 angle;

    speed = (Random16() & 0x3ff) + 256;
    angle = (Random16() & 0x7fff) - 0x4000;
    EFFECT_SET_POSITION(spark, 0x4000, 0x7000, (Trig_Sin(angle) * speed) >> 16);
    spark->velocityZ = -(Trig_Cos(angle) * speed * 2) >> 16;
    spark->variant = 0;
  }

  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);
  AudioCommand_PlayFar(138);

  for (frame = 0; frame != 64; frame++) {
    if (frame == 20)
      BattleEventRuntime_BeginPhaseFar(133); // start playing the results
    if (frame < 16) {
      // Clear the canvas every 5 frames.
      if (frame % 5 == 2)
        Iwram_FillWords(canvas, 0x4000, 0x10101010);
      for (bolt = 0; bolt != CLOSING_BOLT_COUNT; bolt++) {
        s32 angle;
        s32 x;
        s32 y;

        // The radius across shrinks from 32 by a pixel a frame.
        thrown = 0;
        angle = ((frame << 11) + 0x4000) * bolt;
        x = (((32 - frame) * Trig_Sin(angle)) >> 16) + 64;
        y = -(((Trig_Cos(angle) * 8)) >> 16) - 8;
        if (work->effect->variant == 0)
          draw[0](canvas,
                  work->sheet + (Random16() & 3) * LIGHTNING_BOLT_CELL_BYTES +
                      CLOSING_BOLT_CELLS,
                  x + (Random16() & 7) - 16, y, 24, 120);
        else
          draw[bolt & 1](canvas,
                         work->sheet +
                             (Random16() & 3) * LIGHTNING_BOLT_CELL_BYTES +
                             CLOSING_BOLT_CELLS,
                         x + (Random16() & 7) - 16, y, 24, 120);

        // Throw sparks from the bolt's root.
        for (i = 0; i != CLOSING_BOLT_SPARK_COUNT; i++) {
          struct EffectStep *spark = &PARTICLES[i];

          if (spark->variant == 0) {
            s32 speed;
            s32 spread;

            speed = (Random16() & 0x1ff) + 128;
            spread = (Random16() & 0x7fff) - 0x4000;
            spark->x = x << 16;
            spark->y = (y + 112) << 16;
            spark->velocityX = (Trig_Sin(spread) * speed) >> 9;
            spark->velocityY = -(Trig_Cos(spread) * speed * 2) >> 7;
            spark->variant = (Random16() & 7) + 32;
            thrown++;
            if (thrown == gLightningBolts_Sparks[work->effect->variant][1])
              break;
          }
        }
      }
      work->shakeFrames = 1;
    }

    // Move the sparks, bouncing them off the bottom (y 120).
    for (i = 0; i != CLOSING_BOLT_SPARK_COUNT; i++) {
      struct EffectStep *spark = &PARTICLES[i];

      if (spark->variant > 0) {
        spark->variant--;
        EffectStep_AdvanceWithGravity2D(spark, 60, -0x800);
        if (spark->y > 0x780000) {
          spark->velocityY = -spark->velocityY / 2;
        } else if (spark->x >= 0 && spark->x < 0x7f0000 && spark->y >= 0) {
          s32 x = spark->x >> 16;
          s32 y = spark->y >> 16;
          s32 size = spark->variant / 8 + 1;

          EFFECT_DRAW_MOTE(draw[0], canvas, sheet, x, y, size);
        }
      }
    }

    // The targets are hit 4 frames apart.
    if (frame >= 4 && frame < 96) {
      EFFECT_FOR_EACH_TARGET(i, work->effect) {
        if (frame == i * 4 + 4)
          ObjectGroup_UpdateMembers(work->effect->actors[i], SPRITE_COLOR_WHITE,
                                    POSE_DAMAGE, i, 10);
      }
    }
    Camera_ApplyShake(2, 4);
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS();
}

// Abilities: Smoke Bomb, Sleep Bomb (kind 380).
//
// Erupt grey motes and smoke using the second count-table row.
DEFINE_EFFECT_KIND(BattleFx_RunSmokeBomb, BattleFx_RunBlastAndSmokeBomb, 1)

// Abilities: Blast, Mad Blast, Fiery Blast (kind 14).
//
// Erupt fire-coloured motes and smoke and recoil the targets.
DEFINE_EFFECT_KIND(BattleFx_RunBlast, BattleFx_RunBlastAndSmokeBomb, 0)

// The columns of a gBlastAndSmokeBomb_Counts row.
enum {
  FIELD_MOTES, // motes thrown
  FIELD_PUFFS, // puffs of smoke thrown
  FIELD_FRAMES // frames in the whole effect
};

// The mote pictures in the sheet: two sets of sizes, 770 bytes apart.
#define FIELD_MOTE_CELLS 0x3200

// Shared by BattleFx_RunBlast (mode 0) and BattleFx_RunSmokeBomb (mode 1).
//
// A fountain of motes and puffs of smoke bursts up beside the
// targets and falls back, hitting the targets one frame apart.
//
// effect: the targets.
// mode: 0 burns (the fire streak palette, the effect's variant picks the
//       counts, and the targets recoil); 1 is grey (a ramp of greys,
//       variant 1's counts).
void BattleFx_RunBlastAndSmokeBomb(struct BattleEffectArgument *effect,
                                   s32 mode) {
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  /* FAKEMATCH: frame local: only draw[0] is used; the unused second blitter
     word fills four bytes of the frame. Measured (2026-10-14): declared as one
     DrawRectangle, the frame is 52 bytes for 56 (differ 20, frame offsets
     only). */
  BlitterPair draw;
  s32 variant;
  struct EffectPosition screen;
  s32 frame;
  s32 i;

  cursor = gBattleFxWork;
  EFFECT_GET_WORK_AND_CANVAS(cursor, work, canvas);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(1);
  Resource_LoadAndDecompress((s32)&ResourceId_BlastSheet, work->sheet, 1, 0);
  if (mode == 1) {
    for (i = 0; i != 64; i++) {
      s32 level = i / 2;

      BG_PLTT[i] = (level << 10) | (level << 5) | level;
    }
    variant = 1;
  } else {
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_FireStreakSheet),
                    128);
    variant = work->effect->variant;
  }

  // The puffs and the motes start 50 pixels to the side and scatter up.
  for (i = 0; i != 32; i++) {
    struct EffectStep *puff = &work->particles[i];

    if (work->effect->side == 1)
      puff->x = 200 << 14;
    else
      puff->x = -(200 << 14);
    puff->y = 0;
    puff->z = 0;
    EFFECT_SET_VELOCITY(puff, ((Random16() & 63) - 32) << 13,
                        ((Random16() & 63) + 16) << 12,
                        ((Random16() & 63) - 32) << 13);
    puff->variant = 0;
  }
  for (i = 0; i != 1024; i++) {
    struct EffectStep *mote = &PARTICLES[i];

    if (work->effect->side == 1)
      mote->x = 200 << 14;
    else
      mote->x = -(200 << 14);
    mote->y = 0;
    mote->z = 0;
    EFFECT_SET_VELOCITY(mote, ((Random16() & 63) - 32) << 13,
                        ((Random16() & 31) + 8) << 13,
                        ((Random16() & 63) - 32) << 13);
    mote->variant = 0;
  }

  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                      COMPOSITE_MAX);
  draw[0] =
      (DrawRectangle)((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BLITTER];
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);

  for (frame = 0; frame != gBlastAndSmokeBomb_Counts[variant][FIELD_FRAMES];
       frame++) {
    struct BattleCamera *camera;

    camera = gCameraWork;
    EFFECT_VIEW_FROM_CAMERA(camera);
    if (frame == 2)
      AudioCommand_PlayFar(144);
    if (frame == gBlastAndSmokeBomb_Counts[variant][FIELD_FRAMES] - 48)
      BattleEventRuntime_BeginPhaseFar(133);

    for (i = 0; i != gBlastAndSmokeBomb_Counts[variant][FIELD_MOTES]; i++) {
      struct EffectStep *mote = &PARTICLES[i];

      if (mote->y >= 0) {
        s32 size;

        EffectPosition_ApplyBaseAndYOffset(mote, &screen);
        screen.x >>= 1;
        screen.x += work->effect->side * 32 - 16;
        EFFECT_CLAMP(screen.depth, 160, 799);
        size = 9 - (screen.depth - 160) / 64;
        draw[0](canvas,
                work->sheet +
                    (gBattleFx_MoteCellOffsets[size - 1] + (i & 1) * 770) +
                    FIELD_MOTE_CELLS,
                screen.x - size / 2, screen.y - size, size, size * 2);
        EffectStep_AdvanceWithGravity3D(mote, 64, -0x2000);
      }
    }

    if (frame > 2) {
      for (i = 0; i != gBlastAndSmokeBomb_Counts[variant][FIELD_PUFFS]; i++) {
        struct EffectStep *puff = &work->particles[i];

        if (i < frame && puff->y >= 0) {
          EffectPosition_ApplyBaseAndYOffset(puff, &screen);
          screen.x >>= 1;
          screen.x += work->effect->side * 32 - 16;
          if (puff->variant >= 0 && puff->variant <= 20) {
            s32 cel = puff->variant / 3;
            u32 width;

// Sequence dimensions before the call; assigning and reading them in
// separate arguments was UB. Plain English trial: unchanged 0x444 bytes,
// with different instructions. Preserve the original native calls.
#ifdef UBFIX
            width = gBlastAndSmokeBomb_PuffSizes[cel];
            draw[0](canvas, work->sheet + gBlastAndSmokeBomb_PuffCells[cel],
                    screen.x - (width >> 1), screen.y - (width >> 1), width,
                    width);
#else
            draw[0](canvas, work->sheet + gBlastAndSmokeBomb_PuffCells[cel],
                    screen.x -
                        ((width = gBlastAndSmokeBomb_PuffSizes[cel]) >> 1),
                    screen.y - (width >> 1), width, width);
#endif
          }
          if (puff->variant <= 20)
            puff->variant++;
          EffectStep_AdvanceWithGravity3D(puff, 64, -0x2000);
        }
      }
    }

    if (mode == 0) {
      EFFECT_FOR_EACH_TARGET(i, work->effect) {
        if (frame == i + 6) {
          ObjectGroup_UpdateMembers(work->effect->actors[i], SPRITE_COLOR_WHITE,
                                    POSE_DAMAGE, i, 10);
          BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 2);
        }
      }
    } else {
      EFFECT_FOR_EACH_TARGET(i, work->effect) {
        if (frame == i + 6)
          ObjectGroup_UpdateMembers(work->effect->actors[i], SPRITE_COLOR_WHITE,
                                    POSE_DAMAGE, i, 10);
      }
    }
    if (frame == 2)
      work->shakeFrames = 6;
    Camera_ApplyShake(16, 16);
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

// Creates the effect's animation objects in gBattleFxWork's objects.
//
// entryCount: how many objects to create.
// kind: the resource object every one of them is made from.
// variant: the sprite priority they take.
// Object n plays animation n.
void BattleFx_SpawnObjects(s32 entryCount, s32 kind, u32 variant) {
  struct BattleEffectWork *work = WORK_SLOT(HEAP_SLOT_BATTLE_EFFECT);
  s32 entryIndex = 0;

  if (entryCount == 0) {
    return;
  }
  do {
    struct AnimationObject *object = ResourceObject_CreateFar(kind);

    work->objects[entryIndex] = object;
    if (object != 0) {
      object->flags = 0;
      AnimationObjects_SelectAnimationFar(object, entryIndex);
      work->objects[entryIndex]->image.priority = variant;
    }
    entryIndex++;
  } while (entryIndex != entryCount);
}

// An effect that does nothing.
void BattleFx_RunNoEffect(void) {}

// Burst showers, swirling stars, orbiting sparks, target bursts and staged
// slashes.
//
// Each effect here is a frame loop over the shared effect canvas: it loads its
// sheet, seeds its particles in work->particles (or in the larger shared pool,
// PARTICLES), then draws and advances them every frame before presenting the
// canvas.
//
// - The burst shower fires one burst a frame from the user at random points
//   near the targets, and hits each target every 8 frames. One mode draws a
//   lightning bolt instead. Each burst is one particle slot, its variant
//   counting the 24 frames it lives (-1 when free).
// - The swirling stars spin 64 stars about a point above the field, each on
//   its own axis, then pull them back to the centre.
// - The orbiting skulls (BattleFx_RunOrbitingSparks) circle each target in
//   turn, three to a target, their angles kept in the particles' velocityX.
// - The target bursts flare up on each target while five puffs of smoke
//   scatter around each flare; six particle slots per target.
// - Atalanta's staged slashes run in two parts. In the gathering part a 3x3
//   formation follows a stored path while sixteen blades draw their trail
//   particles in, flash and fly off; each trail particle's random rotation
//   matrix is stored after the particle records in the shared scratch buffer.
//   In the falling part 24 blades drop onto the targets and burst into flame.
//
// The order of the steps within a frame matters: later steps draw over
// earlier ones, and several of them consume random numbers.

// The burst shower's second picture in the sheet, after the tornado.
#define BURST_SHOWER_IMAGE (work->sheet + 0xC56)

// The burst shower's styles, its mode argument.
enum BurstShowerMode {
  BURST_SHOWER_MODE_FLASH,     // flash bursts
  BURST_SHOWER_MODE_ICE,       // in the ice block palette
  BURST_SHOWER_MODE_SPRAY,     // in the water spray palette
  BURST_SHOWER_MODE_PINK,      // pink, fired from 16 pixels lower
  BURST_SHOWER_MODE_DJINNI,    // Mars djinni palette, 28 pixels in front
  BURST_SHOWER_MODE_LIGHTNING, // no bursts: a violet lightning bolt
  BURST_SHOWER_MODE_GREY,      // grey; the targets stay solid black
  BURST_SHOWER_MODE_FACES,     // demon faces from 16 pixels in front
};

// Arms DMA 0 to copy one word of gBattleFxWork's bg2X table into BG2X at
// each horizontal blank, bending BG2 line by line.
void BattleFx_ArmBg2AffineHBlankDma(void) {
  struct BattleEffectWork *work = WORK_SLOT(HEAP_SLOT_BATTLE_EFFECT);
  volatile u16 *channel = REG_DMA0;
  Dma_Stop(channel);
  Dma_Set(work->bg2X, (void *)&REG_BG2X, DMA_HBLANK_WORD_PER_LINE,
          (volatile u32 *)channel);
}

// Abilities: Fire Blessing (kind 400).
//
// Fire flash bursts from the user towards randomly chosen targets.
DEFINE_EFFECT_KIND(BattleFx_RunFireBlessing, BattleFx_RunBurstShower,
                   BURST_SHOWER_MODE_FLASH)

// Abilities: Water Blessing (kind 401).
//
// Fire the burst shower in the water-spray palette.
DEFINE_EFFECT_KIND(BattleFx_RunWaterBlessing, BattleFx_RunBurstShower,
                   BURST_SHOWER_MODE_SPRAY)

// Abilities: Dark Blessing (kind 403).
//
// Fire grey bursts and leave the targets solid black.
DEFINE_EFFECT_KIND(BattleFx_RunDarkBlessing, BattleFx_RunBurstShower,
                   BURST_SHOWER_MODE_GREY)

// Abilities: Dark Blessing, Acid Blessing (kind 404).
//
// Fire pink bursts from 16 pixels below the normal origin.
DEFINE_EFFECT_KIND(BattleFx_RunAcidBlessing, BattleFx_RunBurstShower,
                   BURST_SHOWER_MODE_PINK)

// Abilities: Storm Blessing (kind 405).
//
// Show violet lightning between the user and targets instead of bursts.
DEFINE_EFFECT_KIND(BattleFx_RunStormBlessing, BattleFx_RunBurstShower,
                   BURST_SHOWER_MODE_LIGHTNING)

// Abilities: Evil Blessing (kind 406).
//
// Fire demon-face bursts from 16 pixels in front of the user.
DEFINE_EFFECT_KIND(BattleFx_RunEvilBlessing, BattleFx_RunBurstShower,
                   BURST_SHOWER_MODE_FACES)

// Abilities: Deadly Gas (kind 407).
//
// Fire Mars-coloured bursts from 28 pixels in front of the user.
DEFINE_EFFECT_KIND(BattleFx_RunDeadlyGas, BattleFx_RunBurstShower,
                   BURST_SHOWER_MODE_DJINNI)

#define BURST_SHOWER_FRAMES 64 // frames in the effect

// Burst shower: for 48 frames the user fires one burst a frame at a random
// point near one of the targets, and each target is hit every 8 frames.
//
// effect: the user and the targets.
// mode: the burst and its palette: 0 flash bursts, 1 in the ice block
//       palette, 2 in the water spray palette, 3 pink and fired from 16
//       pixels lower, 4 in the Mars djinni palette from 28 pixels in
//       front, 6 grey (the targets turn solid black and stay so), 7 demon
//       faces from 16 pixels in front. Mode 5 fires no bursts: a violet
//       lightning bolt stands between the user and the targets, on a
//       canvas shown at three times its width.
void BattleFx_RunBurstShower(struct BattleEffectArgument *effect, s32 mode) {
  u32 *slots;
  struct BattleEffectWork *work;
  void *canvas;
  u8 *sprites;
  BlitterPair blit;
  struct EffectPosition origin;
  struct EffectPosition targetSpots[8];
  s32 aim[3];
  u16 *color;
  s32 frame;
  s32 i;
  s32 target;
  s32 level;
  s32 paletteId;
  s32 width;
  s32 height;

  slots = (u32 *)&gWorkSlot[HEAP_SLOT_BATTLE_CANVAS];
  canvas = (void *)slots[0];
  work = (struct BattleEffectWork *)CANVAS_SLOT(slots, HEAP_SLOT_BATTLE_EFFECT);
  sprites = (u8 *)CANVAS_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);

  if (mode == BURST_SHOWER_MODE_FACES) {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                        COMPOSITE_MAX);
    blit[0] = (DrawRectangle)CANVAS_SLOT(slots, HEAP_SLOT_BLITTER);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                        COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X,
                        COMPOSITE_MAX);
    blit[1] = (DrawRectangle)CANVAS_SLOT(slots, HEAP_SLOT_BLITTER_ALTERNATE);
  } else {
    BattleFx_FetchRectangleBlitters(work->effect->side, blit);
  }

  // The pictures and the palette.
  Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, work->sheet, 1, 0);
  if (mode == BURST_SHOWER_MODE_LIGHTNING) {
    Resource_LoadAndDecompress((s32)&ResourceId_VioletLightningSheetA,
                               BURST_SHOWER_IMAGE, 1, 1);
  } else if (mode == BURST_SHOWER_MODE_FACES) {
    Resource_LoadAndDecompress((s32)&ResourceId_DemonFaceSheet,
                               BURST_SHOWER_IMAGE, 1, 1);
  } else {
    Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                               BURST_SHOWER_IMAGE, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sprites, 0,
                               0);
    if (mode == BURST_SHOWER_MODE_GREY) {
      for (i = 0, color = (u16 *)BG_PLTT; i != 64; i++) {
        level = i / 4;
        *color = ((level << 10) | (level << 5)) | level;
        color++;
      }
      REG_BLDCNT = 0;
    } else {
      switch (mode) {
      case BURST_SHOWER_MODE_FLASH:
        paletteId = (s32)&ResourceId_FlashBurstSheet;
        break;
      case BURST_SHOWER_MODE_ICE:
        paletteId = (s32)&ResourceId_IceBlockSheet;
        break;
      case BURST_SHOWER_MODE_SPRAY:
        paletteId = (s32)&ResourceId_WaterSpraySheet;
        break;
      case BURST_SHOWER_MODE_PINK:
        paletteId = (s32)&ResourceId_PinkPalette;
        break;
      case BURST_SHOWER_MODE_DJINNI:
      default:
        paletteId = (s32)&ResourceId_MarsDjinnSheet;
        break;
      }
      Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(paletteId), 128);
    }
  }

  if (mode == BURST_SHOWER_MODE_FACES) {
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, 50);
  } else {
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);
  }
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);

  // Where the bursts start: the user, moved for some modes.
  for (i = 0; i != 64; i++) {
    work->particles[i].variant = -1;
  }

  EffectPosition_ApplyAnimationAndYOffset(work->effect->actor, &origin);
  if (mode == BURST_SHOWER_MODE_PINK) {
    origin.y -= 16;
  }
  if (mode == BURST_SHOWER_MODE_DJINNI) {
    if (work->effect->side == 1) {
      origin.x += 28;
    } else {
      origin.x -= 28;
    }
  }
  if (mode == BURST_SHOWER_MODE_FACES) {
    if (work->effect->side == 1) {
      origin.x += 16;
    } else {
      origin.x -= 16;
    }
  }
  if (mode == BURST_SHOWER_MODE_LIGHTNING) {
    origin.x = origin.x / 3;
    REG_BG2PA = Q_8_8(1.0 / 3);
  }

  // Where each target stands.
  i = 0;
  while (i != work->effect->count) {
    EffectPosition_ApplyStepAndYOffset(work->effect->actors[i],
                                       &targetSpots[i]);
    i += 1;
  }

  frame = 0;
  do {
    target = frame % work->effect->count;
    if (frame == 4) {
      AudioCommand_PlayFar(136);
    }
    if (mode != BURST_SHOWER_MODE_GREY) {
      if (frame == 24) {
        BattleEventRuntime_BeginPhaseFar(134);
      }
    } else {
      if (frame == 60) {
        BattleEventRuntime_BeginPhaseFar(134);
      }
    }

    // Mode 5 draws the lightning from the user; the others fire one burst
    // a frame for 48 frames at a random point near a target.
    if (mode == BURST_SHOWER_MODE_LIGHTNING) {
      if (work->effect->side == 1) {
        blit[0](canvas, BURST_SHOWER_IMAGE + ((frame / 3 % 3 * 9) << 9),
                origin.x - 2, origin.y - 32, 72, 62);
      } else {
        blit[0](canvas, BURST_SHOWER_IMAGE + ((frame / 3 % 3 * 9) << 9),
                origin.x - 70, origin.y - 32, 72, 62);
      }
    } else {
      aim[0] = (targetSpots[target].x + (Random16() & 31)) - 16;
      aim[1] = (targetSpots[target].y + (Random16() & 63)) - 16;
      if (frame <= 47) {
        work->particles[frame].x = origin.x << 15;
        work->particles[frame].y = origin.y << 16;
        work->particles[frame].velocityX = (aim[0] - origin.x) << 11;
        work->particles[frame].velocityY = (aim[1] - origin.y) << 11;
        work->particles[frame].variant = 0;
      }
    }

    // Move and draw the bursts; each lasts 24 frames.
    i = 0;
    width = 32;
    height = 64;
    do {
      struct EffectStep *spark = &work->particles[i];
      if (spark->variant >= 0) {
        if (mode == BURST_SHOWER_MODE_FACES) {
          if (spark->variant > 5) {
            blit[work->effect->side](canvas, BURST_SHOWER_IMAGE,
                                     HI(spark->x) - 16, HI(spark->y) - 32,
                                     width, height);
          }
        } else if (mode == BURST_SHOWER_MODE_DJINNI) {
          if (spark->variant > 5) {
            blit[0](canvas, BURST_SHOWER_IMAGE + ((spark->variant / 4) << 11),
                    HI(spark->x) - 16, HI(spark->y) - 32, width, height);
          }
        } else if (mode != BURST_SHOWER_MODE_LIGHTNING) {
          if (spark->variant > 1) {
            blit[0](canvas, BURST_SHOWER_IMAGE + ((spark->variant / 4) << 11),
                    HI(spark->x) - 16, HI(spark->y) - 32, width, height);
          }
        }
        spark->x += spark->velocityX;
        spark->y += spark->velocityY;
        spark->variant += 1;
        if (spark->variant == 24) {
          spark->variant = -1;
        }
      }
      i += 1;
    } while (i != 64);

    // Hit each target every 8 frames, once its turn has come.
    if (mode == BURST_SHOWER_MODE_LIGHTNING) {
      i = 0;
      while (i != work->effect->count) {
        if ((frame >= (i * 4) + 2) && ((frame & 7) == i)) {
          work->shakeFrames = 8;
          ObjectGroup_UpdateMembers(work->effect->actors[i], SPRITE_COLOR_WHITE,
                                    POSE_DAMAGE, i, 4);
        }
        i += 1;
      }
    } else {
      i = 0;
      while (i != work->effect->count) {
        if ((frame >= (i * 4) + 16) && ((frame & 7) == i)) {
          work->shakeFrames = 8;
          if (mode == BURST_SHOWER_MODE_GREY) {
            ObjectGroup_UpdateMembers(work->effect->actors[i],
                                      SPRITE_COLOR_BLACK, POSE_DAMAGE, i, 4);
          } else {
            ObjectGroup_UpdateMembers(work->effect->actors[i],
                                      SPRITE_COLOR_WHITE, POSE_DAMAGE, i, 4);
          }
          BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 4);
        }
        i += 1;
      }
    }

    Camera_ApplyShake(4, 4);
    if (mode != BURST_SHOWER_MODE_GREY) {
      ObjectGroup_TickMemberTimers();
    }
    EFFECT_PRESENT_FRAME(work);
    frame += 1;
  } while (frame != BURST_SHOWER_FRAMES);

  EFFECT_FINISH_CANVAS();
}

#define SWIRLING_STARS_FRAMES 160 // frames in the effect
#define SWIRLING_STARS_COUNT 64   // stars that swirl

// Swirling stars: 64 stars scattered around a point 80 pixels up spin
// about it, each on its own axis, appearing four a frame; 30 frames after
// it appears, each star is drawn back towards the centre.
//
// object: the effect argument.
void BattleFx_RunSwirlingStars(void *object) {
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 center[3];
  struct EffectPosition screen;
  BlitterPair draw;
  struct EffectStep *star;
  s32 i;
  s32 frame;

  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  work->effect = object;
  BattleFx_BeginCanvasLayer(0);
  Resource_LoadAndDecompress((s32)&ResourceId_StarDotSheet, work->sheet, 1, 1);
  BattleFx_FetchRectangleBlitters(work->effect->side ^ 1, draw);
  for (i = 0; i != 256; i++) {
    star = &PARTICLES[i];
    EFFECT_SET_POSITION(star, ((Random16() & 0xff) - 127) << 16,
                        ((Random16() & 0xff) - 127) << 16,
                        ((Random16() & 0xff) - 127) << 16);
    EFFECT_SET_VELOCITY(star, 0, 0, 0);
    star->variant = 0;
  }
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  EFFECT_SET_VECTOR(center, 0, 160 << 15, 0);
  for (frame = 0; frame != SWIRLING_STARS_FRAMES; frame++) {
    struct BattleCamera *camera;

    camera = gCameraWork;
    EFFECT_VIEW_FROM_CAMERA(camera);
    SceneTransform_ApplyPosition(center);
    star = PARTICLES;
    for (i = 0; i != SWIRLING_STARS_COUNT; i++, star++) {
      if (frame > i / 4 && star->variant == 0) {
        s32 size;

        Transform_Push();
        switch (i & 3) {
        case 0:
          SceneTransform_ApplyYaw(frame * (i + 32) * 8);
          break;
        case 1:
          SceneTransform_ApplyPitch(-frame * (i + 32) * 8);
          break;
        case 2:
          SceneTransform_ApplyRoll(-frame * (i + 32) * 8);
          break;
        case 3:
          SceneTransform_ApplyPitch(-frame * (i + 32) * 8);
          SceneTransform_ApplyRoll(-frame * (i + 32) * 8);
          break;
        }
        EffectPosition_ApplyBaseAndYOffset(star, &screen);
        screen.x >>= 1;
        Transform_Pop();
        EFFECT_CLAMP(screen.depth, 250, 0x27a);
        size = 9 - (screen.depth - 250) / 64;
        draw[0](canvas,
                work->sheet +
                    ((i & 3) * 770 + gBattleFx_MoteCellOffsets[size - 1]),
                screen.x - size / 2, screen.y - size, size, size * 2);
        EffectStep_AdvanceWithGravity3D(star, 60, 0);
        if (frame > i / 4 + 30) {
          s32 dx = -star->x >> 8;
          s32 dy = -star->y >> 8;
          s32 dz = -star->z >> 8;

          star->velocityX += dx;
          star->velocityY += dy;
          star->velocityZ += dz;
        }
      }
    }
    EFFECT_PRESENT_FRAME(work);
  }
  EFFECT_FINISH_CANVAS();
}

// Orbiting skulls: three skulls circle each target in turn, 16 frames
// apart, for 64 frames each. Runs with the effect already in
// gBattleFxWork.
void BattleFx_RunOrbitingSparks(void) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 point[3];
  struct EffectPosition center;
  struct EffectPosition screen;
  s32 frame;
  /* FAKEMATCH: needed to match: only draw[0] is used: the second word fills
   * four bytes of the frame */
  BlitterPair draw;
  s32 member;
  struct BattleCamera *camera;
  u8 *skulls;
  s32 xOffset;
  s32 i;

  work = WORK_SLOT(HEAP_SLOT_BATTLE_EFFECT);
  canvas = WORK_SLOT(HEAP_SLOT_BATTLE_CANVAS);
  camera = WORK_SLOT(HEAP_SLOT_CAMERA);
  BattleFx_BeginCanvasLayer(1);
  REG_BG2PA = Q_8_8(1);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  skulls = Resource_GetTableEntry((s32)&ResourceId_SkullSheet);
  Iwram_CopyWords((void *)BG_PLTT, skulls, 128);
  Resource_DecodeType01(skulls + 128, work);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                      COMPOSITE_MAX);
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);
  draw[0] = (DrawRectangle)WORK_SLOT(HEAP_SLOT_BLITTER);
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);
  if (work->effect->side == 0)
    xOffset = 0;
  else
    xOffset = -112;
  /* FAKEMATCH: needed to match: a one-pass loop is a sched2 barrier, so the
   * seed counter is set before its pointer 2026-10-11: without it differ 2
   * (movs r4, #0 falls behind the particle offset); i = 0 ahead of the store
   * differs 170. */
  do {
    REG_BG2X = xOffset << 8;
  } while (0);
  // velocityX holds each skull's angle on its orbit.
  for (i = 0; i != 64; i++) {
    struct EffectStep *step = &work->particles[i];

    step->x = 0;
    step->y = 0;
    step->velocityX = 0;
    step->z = 4;
  }
  for (frame = 0; frame != work->effect->count * 16 + 64; frame++) {
    EFFECT_FOR_EACH_TARGET(member, work->effect) {
      struct MotionObject *object;
      u32 memberFrame;

      object = GetBattleObjectSlotFar(work->effect->actors[member])->object;
      EffectPosition_ApplyStepAndYOffset(work->effect->actors[member], &screen);
      memberFrame = frame - member * 16;
      screen.x += xOffset;
      if (memberFrame < 64) {
        /* Ordinary pointer differs 118 in EN (265 native, 269 built).
         * A generic output barrier before filling differs 155 (272
         * built); after filling it differs 118 (267 built). Keeping
         * rec live after the call differs 179 (270 built). None keeps
         * the native loop layout. loop.c's second pass hoists the
         * invariant &point when 15 x savings x life 8 reaches the
         * loop's insn count (about 100 after giv reduction). */
        /* FAKEMATCH: pins rec to r0 so the point is filled through r0 and
         * loop.c cannot hoist its address into fp, as native; plain differs 118
         * (hoisted into fp, frame 92), loads before stores 12, one-pass blocks
         * around the fill and the apply 2 (add r5, sp, #64 after the last
         * store). */
        register s32 *rec asm("r0");

        EFFECT_VIEW_FROM_CAMERA(camera);
        rec = point;
        EFFECT_SET_VECTOR(rec, object->x, object->y, object->z);
        EffectPosition_ApplyBaseAndYOffset(rec, &center);
        /* FAKEMATCH: needed to match: sched2 barrier, so this store precedes
         * the spark counter as in the reference 2026-10-11: without it differ 2
         * (movs r4, #0 moves ahead of the store). */
        do {
          center.x += xOffset;
        } while (0);
        for (i = 0; i != 3; i++) {
          struct EffectStep *spark = &work->particles[member * 3 + i];
          s32 x;
          s32 y;

          x = screen.x +
              (((Trig_Sin(spark->velocityX + i * 0x5555) * 8)) >> 16);
          y = screen.y +
              (((Trig_Cos(spark->velocityX + i * 0x5555) * 8)) >> 16);
          spark->velocityX += 0x200;
          draw[0](canvas, work->sheet + i * 0x240, x - 12, y - 28, 24, 24);
        }
      }
    }
    EFFECT_PRESENT_FRAME(work);
  }
  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

// The burst picture in the sheet, after the smoke.
#define TARGET_BURST_CELLS 0x1b00

#define TARGET_BURSTS_FRAMES 96 // frames in the effect

// Target bursts: on each target in turn, 8 frames apart, a burst flares up
// in the middle while five puffs of smoke scatter around it, for 40
// frames.
//
// object: the effect argument.
void BattleFx_RunTargetBursts(void *object) {
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  void *palette;
  s32 frame;
  DrawRectangle drawSmoke;
  DrawRectangle drawBurst;
  struct EffectStep *puff;
  s32 i;
  struct EffectPosition screen;
  s32 point[3];

  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  work->effect = object;
  BattleFx_BeginCanvasLayer(0);
  Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet, work->sheet, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_TargetBurstImage,
                             work->sheet + TARGET_BURST_CELLS, 0, 0);
  palette = Resource_GetTableEntry((s32)&ResourceId_PinkBurstSheet);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                      COMPOSITE_ADDITIVE);
  drawBurst = EFFECT_SLOT(slots, HEAP_SLOT_BLITTER);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                      COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, COMPOSITE_MAX);
  drawSmoke = EFFECT_SLOT(slots, HEAP_SLOT_BLITTER_ALTERNATE);

  // Six puffs for each target: five scatter at random, the sixth (the
  // burst) stays in the middle.
  for (i = 0; i != 64; i++) {
    s32 angle;
    s32 speed;

    puff = &work->particles[i];
    angle = Random16() & 0xffff;
    speed = Random16() & 0xff;
    EFFECT_SET_POSITION(puff, 0, 0, 0);
    if (i % 6 == 5) {
      puff->velocityX = 0;
      puff->velocityY = 0;
    } else {
      puff->velocityX = (Trig_Sin(angle) * speed) >> 7;
      puff->velocityY = (Trig_Cos(angle) * speed) >> 9;
    }
    puff->velocityZ = 0;
    puff->variant = 0;
  }

  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);

  for (frame = 0; frame != TARGET_BURSTS_FRAMES; frame++) {
    s32 member;
    struct BattleCamera *camera;

    camera = gCameraWork;
    EFFECT_FOR_EACH_TARGET(member, work->effect) {
      struct MotionObject *object;

      object = GetBattleObjectSlotFar(work->effect->actors[member])->object;
      EFFECT_VIEW_FROM_CAMERA(camera);
      EFFECT_SET_VECTOR(point, object->x, 160 << 13, object->z);
      SceneTransform_ApplyPosition(point);
      if (frame >= member * 8 && frame < member * 8 + 40) {
        struct EffectStep *puff;

        for (i = 0; i != 6; i++) {
          s32 cel;

          puff = &work->particles[member * 6 + i];
          cel = puff->variant / 6;
          if (cel > 5)
            cel = 5;
          EffectPosition_ApplyBaseAndYOffset(puff, &screen);
          screen.x >>= 1;
          if (i == 5) {
            EffectStep_AdvanceWithGravity3D(puff, 62, 0x800);
            drawBurst(canvas, work->sheet + cel * 0x480 + TARGET_BURST_CELLS,
                      screen.x - 12, screen.y - 36, 24, 48);
          } else {
            EffectStep_AdvanceWithGravity3D(puff, 60, 0x200);
            drawSmoke(canvas, work->sheet + cel * 0x480, screen.x - 12,
                      screen.y - 36, 24, 48);
          }
          puff->variant += 1;
        }
      }
    }
    EFFECT_PRESENT_FRAME(work);
  }
  EFFECT_FINISH_CANVAS();
}

extern struct SpriteScale gStagedParticles_UnitScale;

// The trail particles' random rotations, 48 bytes each, after the 384
// particles in PARTICLES.
#define STAGED_PARTICLE_MATRICES (Ram_MapCellBuffer + 0x3800)

// The pictures in the slash sheet: five flash frames, then the slash in
// three parts.
enum SlashSheetCell {
  SLASH_FLASH_1 = 0x0000,
  SLASH_FLASH_2 = 0x0188,
  SLASH_FLASH_3_TOP = 0x057c,
  SLASH_FLASH_3_BOTTOM = 0x07d4,
  SLASH_FLASH_4_TOP = 0x0aaa,
  SLASH_FLASH_4_MIDDLE = 0x0c90,
  SLASH_FLASH_4_BOTTOM = 0x0e74,
  SLASH_FLASH_5_TOP = 0x1088,
  SLASH_FLASH_5_MIDDLE = 0x11f8,
  SLASH_FLASH_5_BOTTOM = 0x147c,
  SLASH_TOP = 0x16ac,
  SLASH_MIDDLE = 0x17fc,
  SLASH_BOTTOM = 0x1bf3
};

// Abilities: Atalanta (scene kind 4).
//
// Rain of blades: under a violet sky, a 3x3 formation of objects flies
// along a stored path while sixteen blades gather trails of light, flash
// and fly off; A or B skips ahead. Then 24 blades fall diagonally on the
// targets and burst into flame, hitting each target as they land.
//
// effect: the targets.
void BattleFx_RunAtalanta(struct BattleEffectArgument *effect) {
  s32 point[3];
  struct EffectPosition screen;
  struct SpritePlacement position;
  struct EffectPosition target;
  BlitterPair draw;
  void **slots;
  struct BattleBackgroundView *view;
  void *canvas;
  struct BattleEffectWork *work;
  s8 *path;
  s32 i;
  s32 j;
  s32 frame;
  u8 *sheet;
  s32 scrollX;
  s32 pathX;
  s32 pathY;
  s32 row;
  s32 column;

  // The background is work slot 44: the slots before it hold the effect
  // work (-5), the canvas (-4) and the sheet (-3), the slots after it the
  // blitters (2 and 3).
  slots = &gWorkSlot[HEAP_SLOT_BATTLE_BACKGROUND];
  view = slots[0];
  canvas = slots[-4];
  work = slots[-5];
  sheet = slots[-3];
  scrollX = gBgScroll[1].x;
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0x2000);
  REG_BG2PA = Q_8_8(1);
  BattleFx_ConfigureEffectDisplay();
  BG_PLTT[0] = 0;
  BG_PLTT[1] = 0;
  work->transferMode = CANVAS_COPY;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);
  BattleFx_WipeCanvas(0, 0);
  BattleFx_SelectLivingTargets(work->effect);
  BattleFx_SpawnObjects(9, 370, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_SlashSheet, work->sheet, 1, 1);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_LimePalette), 128);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  path = Resource_GetTableEntry((s32)&ResourceId_StagedParticlePath);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                      COMPOSITE_MAX);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                      COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, COMPOSITE_ADDITIVE);
  draw[0] = slots[HEAP_SLOT_BLITTER - HEAP_SLOT_BATTLE_BACKGROUND];
  /* FAKEMATCH: needed to match: the one-pass block keeps the table address
   * after the second read 2026-10-11: without the block differ 2 (mov r2, sp
   * moves ahead of the first store). */
  do {
    draw[1] = slots[HEAP_SLOT_BLITTER_ALTERNATE - HEAP_SLOT_BATTLE_BACKGROUND];
  } while (0);
  gProjection.centerY = 240;
  WaitFrames(1);
  BattleBackground_LoadFar(1, (s32)&ResourceId_VioletSkyBackdrop, 0);
  work->scrollTimer = 0;
  work->scrollInterval = 4;
  work->scrollStepX = -1;
  work->scrollStepY = 0;
  Scheduler_AddOrUpdateCallback(BattleFx_AdvanceScrollOnInterval, 0x480);
  view->busy = 1;
  BattleFx_WipeCanvas(0, 1);
  EFFECT_SHOW_WIDE_CANVAS(16);
  pathX = 0;
  pathY = 0;
  // Each blade starts at a random spot with 24 trail particles, each on a
  // random rotation kept in STAGED_PARTICLE_MATRICES.
  for (i = 0; i != 16; i++) {
    struct EffectStep *slash = &work->particles[i];

    slash->x = (Random16() % 96 + 12) << 16;
    slash->y = ((Random16() & 63) + 32) << 16;
    slash->velocityX = 0;
    slash->velocityY = 0;
    slash->variant = 0;
    for (j = 0; j != 24; j++) {
      PARTICLES[i * 24 + j].x = (Random16() & 15) + 48;
      Render_ResetTransformState();
      SceneTransform_ApplyRoll(Random16() & 0xffff);
      SceneTransform_ApplyPitch(Random16() & 0xffff);
      SceneTransform_ApplyYaw(Random16() & 0xffff);
      Transform_Store(STAGED_PARTICLE_MATRICES + (i * 24 + j) * 48);
    }
  }
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, 50);
  REG_BG2CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7);
  for (frame = 0; frame != 220 && !(gKeysRepeat & (KEY_A | KEY_B)); frame++) {
    // The formation follows the path: two 16-bit coordinates, then a
    // signed byte step for each axis every frame.
    if (frame <= 209) {
      if (frame == 0) {
        pathX = (path[0] << 8) + (u8)path[1];
        pathY = (path[2] << 8) + (u8)path[3];
        path += 4;
      } else {
        pathX += path[0];
        pathY += path[1];
        path += 2;
      }
      position.groundHeight = 0;
      position.height = 0xff0000;
      for (row = 0; row != 3; row++) {
        for (column = 0; column != 3; column++) {
          position.x = (pathX + 80 + column * 32) << 16;
          position.y = (64 - pathY + row * 32) << 16;
          Object_ApplyProjectedPlacementFar(work->objects[row * 3 + column],
                                            &position,
                                            &gStagedParticles_UnitScale, 0);
        }
      }
    }
    point[1] = 0;
    point[2] = 0;
    if (frame == 48) {
      work->fadeFrames = 24;
      work->fadeStep = 0;
    }
    // Blade i appears at frame i * 8 + 64: its trails close in for 16
    // frames, it flashes for 5, and from frame 85 it flies off.
    for (i = 0; i != 16; i++) {
      s32 start = i * 8 + 64;

      if (frame >= start) {
        struct EffectStep *slash = &work->particles[i];
        s32 x = HI(slash->x);
        s32 y = HI(slash->y);

        if (frame == i * 8 + 84)
          AudioCommand_PlayFar(212);
        if (frame >= i * 8 + 85) {
          slash->x += slash->velocityX;
          slash->y += slash->velocityY;
          slash->velocityX -= 0x10000;
          slash->velocityY += 0x20000;
          draw[0](canvas, work->sheet + SLASH_TOP, x + 4, y - 40, 16, 21);
          draw[0](canvas, work->sheet + SLASH_MIDDLE, x - 16, y - 19, 29, 35);
          draw[0](canvas, work->sheet + SLASH_BOTTOM, x - 20, y + 16, 21, 24);
        } else if (frame >= i * 8 + 80) {
          switch (frame - start - 16) {
          case 0:
            draw[0](canvas, work->sheet + SLASH_FLASH_1, x - 7, y - 14, 14, 28);
            break;
          case 1:
            draw[0](canvas, work->sheet + SLASH_FLASH_2, x - 11, y - 22, 23,
                    44);
            break;
          case 2:
            draw[0](canvas, work->sheet + SLASH_FLASH_3_TOP, x - 4, y - 31, 20,
                    30);
            draw[0](canvas, work->sheet + SLASH_FLASH_3_BOTTOM, x - 16, y - 1,
                    22, 33);
            break;
          case 3:
            draw[0](canvas, work->sheet + SLASH_FLASH_4_TOP, x + 1, y - 38, 18,
                    27);
            draw[0](canvas, work->sheet + SLASH_FLASH_4_MIDDLE, x - 11, y - 11,
                    22, 22);
            draw[0](canvas, work->sheet + SLASH_FLASH_4_BOTTOM, x - 19, y + 11,
                    19, 28);
            break;
          case 4:
            draw[0](canvas, work->sheet + SLASH_FLASH_5_TOP, x + 4, y - 40, 16,
                    23);
            draw[0](canvas, work->sheet + SLASH_FLASH_5_MIDDLE, x - 10, y - 17,
                    23, 28);
            draw[0](canvas, work->sheet + SLASH_FLASH_5_BOTTOM, x - 20, y + 11,
                    20, 28);
            break;
          }
        } else {
          for (j = 0; j != 24; j++) {
            struct EffectStep *trail = &PARTICLES[i * 24 + j];

            if (trail->x > 0) {
              Transform_Load(STAGED_PARTICLE_MATRICES + (i * 24 + j) * 48);
              point[0] = trail->x;
#if !TBS_PATCH_1
              {
                /* FAKEMATCH: the named result pointer is a pseudo of its own,
                 * which gives the Japanese frame its 124 bytes and keeps the
                 * trail counter j in a stack slot, as native. 2026-10-13: the
                 * plain &screen argument, as the other editions pass it,
                 * gives a 120-byte frame and j in r9 (tbs-ja differ 372). */
                struct EffectPosition *out = &screen;

                EffectPosition_ApplyBaseAndYOffset(point, out);
              }
              screen.x = (screen.x >> 1) + x;
              screen.y = screen.y + y;
#else
              EffectPosition_ApplyBaseAndYOffset(point, &screen);
              screen.x = (screen.x >> 1) + x;
              screen.y = screen.y + y + 16;
#endif
              trail->x -= 4;
              draw[1](canvas, sheet + gBattleFx_MoteCellOffsets[4],
                      screen.x - 2, screen.y - 5, 5, 10);
            }
          }
        }
      }
    }
    EFFECT_PRESENT_FRAME(work);
  }
  Scheduler_RemoveCallback(BattleFx_AdvanceScrollOnInterval);
  view->busy = 0;
  gBgScroll[1].x = scrollX;
  BattleFx_SetupBlendedDisplay();
  for (i = 0; i != 9; i++)
    ResourceObject_ReleaseFar(work->objects[i]);
  REG_BG2PA = Q_8_8(0.5);
  REG_DISPCNT = EFFECT_DISPCNT;
  Resource_LoadAndDecompress((s32)&ResourceId_EmberStreakSheet,
                             Ram_MapCellBuffer, 1, 0);
  // Second part: blades fall diagonally, each on a target where there is
  // one, and burst into flame where they land.
  for (i = 0; i != 32; i++) {
    struct EffectStep *slash = &work->particles[i];
    s32 member = i % 6;

    if (member < work->effect->count) {
      EffectPosition_ApplyStepAndYOffset(work->effect->actors[member], &target);
      slash->y = -((Random16() & 31) + 40);
      slash->x = target.x / 2 + (80 - slash->y) / 2;
    } else {
      slash->x = (Random16() & 63) + 80;
      slash->y = -((Random16() & 31) + 40);
    }
    slash->variant = -1;
  }
  for (frame = 0; frame != 88; frame++) {
    for (i = 0; i != 24; i++) {
      struct EffectStep *slash = &work->particles[i];

      if (frame >= i * 2 || frame > 40) {
        if (slash->variant >= 0) {
          if (slash->variant <= 23) {
            s32 cell = slash->variant / 4;
            u32 width;

// Sequence dimensions before the call; assigning and reading them in
// separate arguments was UB. Plain English trial: unchanged 0x944 bytes,
// with different instructions. Preserve the original native calls.
#ifdef UBFIX
            width = gBattleFx_PuffArcCellWidths[cell];
            draw[i & 1](canvas,
                        Ram_MapCellBuffer +
                            gBattleFx_PuffArcCellSourceOffsets[cell],
                        slash->x - (width >> 1) - 8,
                        slash->y + gBattleFx_PuffArcCellBiasY[cell] - 40, width,
                        gBattleFx_PuffArcCellHeights[cell]);
#else
            draw[i & 1](
                canvas,
                Ram_MapCellBuffer + gBattleFx_PuffArcCellSourceOffsets[cell],
                slash->x - ((width = gBattleFx_PuffArcCellWidths[cell]) >> 1) -
                    8,
                slash->y + gBattleFx_PuffArcCellBiasY[cell] - 40, width,
                gBattleFx_PuffArcCellHeights[cell]);
#endif
            if (slash->variant <= 11) {
              draw[0](canvas, work->sheet + SLASH_TOP, slash->x + 4,
                      slash->y - 40, 16, 21);
              draw[0](canvas, work->sheet + SLASH_MIDDLE, slash->x - 16,
                      slash->y - 19, 29, 35);
            }
          }
          slash->variant++;
        } else {
          s32 height = 24;

          if (slash->y > 56)
            height = height - slash->y + 56;
          draw[0](canvas, work->sheet + SLASH_TOP, slash->x + 4, slash->y - 40,
                  16, 21);
          draw[0](canvas, work->sheet + SLASH_MIDDLE, slash->x - 16,
                  slash->y - 19, 29, 35);
          if (height > 0)
            draw[0](canvas, work->sheet + SLASH_BOTTOM, slash->x - 20,
                    slash->y + 16, 21, height);
          slash->x -= 6;
          slash->y += 12;
          if (slash->y > 79) {
            s32 member;

            slash->variant = 0;
            work->shakeFrames = 2;
            AudioCommand_PlayFar(134);
            member = i % 6;
            if (member < work->effect->count) {
              ObjectGroup_UpdateMembers(work->effect->actors[member],
                                        SPRITE_COLOR_WHITE, POSE_DAMAGE, member,
                                        8);
              BattleMotion_ApplyVariantMotionFar(work->effect->actors[member],
                                                 1);
            }
          }
        }
      }
    }
    Camera_ApplyShake(4, 8);
    EFFECT_END_FRAME(work);
  }
  EFFECT_FINISH_CANVAS();
}

// Growing vines and pillars, lightning strikes and Djinni entrances.
//
// Growing columns and thorns spring up one after another on a fixed schedule,
// each shaking the screen as it appears, throwing up puffs of dust from its
// root and making the targets react a few frames later. The dust is a pool
// of EffectSteps in PARTICLES, a free one marked by variant -1; each puff
// plays seven pictures, drawn mirrored about its x. Both scenes fade out over
// their last 16 frames.
//
// Lightning bolts (Bolt) strike the targets in turn, each throwing sparks and
// glints from its foot. Sparks fall under gravity and bounce off the ground
// at half speed.
//
// Variant rows supply the counts and sprite geometry. Every scene ends by
// finishing the canvas it began. A Djinni entrance leaves the canvas cleared
// for the effect that follows it.

// Growing columns: vines or pillars spring from the ground one after
// another, throwing up dust. PARTICLES holds the dust.
// One growing column, laid over a struct EffectStep.
struct DualColumn {
  s32 x; // the root, in canvas pixels
  s32 y;
  s32 z; // the EffectStep's z and velocityX, unused here
  s32 velocityX;
  s32 height;    // the full length
  s32 velocityZ; // the EffectStep's, unused here
  s32 start;     // the frame it starts to grow
};

// Abilities: Growth, Mad Growth, Wild Growth (kind 4).
//
// Grow vines from the ground, scattering dust from each root.
DEFINE_EFFECT_KIND(BattleFx_RunGrowth, BattleFx_RunGrowingColumns, 0)

// Abilities: Punji, Punji Trap, Punji Strike (kind 56).
//
// Grow green pillars from the ground, scattering dust from each root.
DEFINE_EFFECT_KIND(BattleFx_RunPunji, BattleFx_RunGrowingColumns, 1)

// Vines (mode 0) or green pillars (mode 1) grow one every 4 frames,
// alternately mirrored, each throwing up dust from its root.
//
// effect: the targets; variant picks the number of columns
//         (gDualTable_ColumnCounts).
void BattleFx_RunGrowingColumns(struct BattleEffectArgument *effect, s32 mode) {
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  void *rectangle[2];
  void *secondSlot;

  s32 frame;
  s32 duration;
  void **draw;
  s32 cueFrame;
  s32 fadeStart;
  s32 fadeEnd;
  s32 start;

  u8 *variantCursor;

  struct DualColumn *setup;
  s32 x;
  s32 height;
  s32 limit;

  s32 puffCells;
  struct DualColumn *column;
  s32 length;
  s32 reach;
  s8 pillarHeight;

  s32 n;
  s32 reactFrame;

  s32 i;
  struct EffectStep *puff;
  s32 puffAge;
  s32 image;

  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(1);
  REG_BG2PA = Q_8_8(1); // the canvas at its own width
  REG_BLDCNT = 0;
  if (mode == 1) {
    Resource_LoadAndDecompress((s32)&ResourceId_GreenPillarSheet, work->sheet,
                               1, 1);
  } else {
    Resource_LoadAndDecompress((s32)&ResourceId_VineSheet, work->sheet, 1, 1);
  }
  if (work->effect->side == 1) {
    REG_BG2X = -0x7000; // 112 pixels right
  }
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                      COMPOSITE_TRANSPARENT);
  rectangle[0] = gWorkSlot[HEAP_SLOT_BLITTER];
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                      COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X,
                      COMPOSITE_TRANSPARENT);
  secondSlot = gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
  /* FAKEMATCH: needed to match: the columns draw through a second pointer to
   * the pair, whose address the frame keeps in a stack slot 2026-10-11:
   * rectangle[side] at the columns differs 22 (the address slot of the pair
   * moves from sp+28 to sp+16). */
  draw = rectangle;
  draw[1] = secondSlot;

  duration = gDualTable_ColumnCounts[work->effect->variant] * 4 + 56;

  // Mark all the dust free (variant -1).
  variantCursor = (u8 *)gMapCellBuffer + offsetof(struct EffectStep, variant);
  i = 0;
  do {
    i++;
    *(s32 *)variantCursor = -1;
    variantCursor += sizeof(struct EffectStep);
  } while (i != 1024);

  // The 16 columns' roots and lengths.
  i = 0;
  setup = (struct DualColumn *)work->particles;
  do {
    x = (gDualTable_ColumnX[i] + (7 & Random16())) - 4;
    setup->y = i / 2 + 108;
    setup->x = x;
    height = (63 & Random16()) + 55;
    setup->height = height;
    limit = gDualTable_HeightLimits[i % 3];
    if (limit < height) {
      setup->height = limit;
    }
    limit = i * 4 + 8;
    setup->start = limit;
    i++;
    setup++;
  } while (i != 16);

  work->transferMode = CANVAS_COPY_AND_CLEAR; // clear the canvas every frame
  work->transferValue = 0;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);

  frame = 0;
  if (duration != 0) {
    cueFrame = duration - 64;
    fadeStart = duration - 20;
    fadeEnd = duration - 4;
    do {
      if (frame == cueFrame) {
        BattleEventRuntime_BeginPhaseFar(132);
      }
      if (frame >= fadeStart && frame < fadeEnd) {
        // Fade out over the last 16 frames.
        REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
        REG_BLDALPHA = ((duration - frame) - 5) | 0x1000;
      }
      if (frame < fadeEnd) {
        i = 0;
        if (gDualTable_ColumnCounts[work->effect->variant] != 0) {
          do {
            column = (struct DualColumn *)((u8 *)work->particles +
                                           i * sizeof(struct DualColumn));
            start = i * 4 + 8;
            if (frame == i * 4 + 9) {
              work->shakeFrames = 2;
            }

            // Grow 8 pixels a frame (odd columns mirrored).
            if (frame > start) {
              u32 shape = i % 3;

              length = (frame - start) * 8;
              reach = column->height;
              if (length > reach) {
                length = reach;
              }
              if (mode == 0) {
                {
                  s32 side = 1 & i;
                  s32 cell = gDualTable_VineCells[shape];
                  shape = gDualTable_VineWidths[shape];
                  ((DrawRectangle)draw[side])(
                      canvas, work->sheet + cell, column->x - (shape >> 1),
                      column->y - length, shape, length);
                }
              } else {
                pillarHeight = gDualTable_PillarHeights[7 & i];
                if (length > pillarHeight) {
                  length = pillarHeight;
                }
                {
                  s32 side = 1 & i;
                  s32 cell = gDualTable_PillarCells[shape];
                  shape = gDualTable_PillarWidths[shape];
                  ((DrawRectangle)draw[side])(
                      canvas, work->sheet + cell, column->x - (shape >> 1),
                      column->y - length, shape, length);
                }
              }
            }

            // The targets react 4 frames after the column starts to grow.
            n = 0;
            if (work->effect->count != 0) {
              reactFrame = start + 4;
              do {
                if (frame == reactFrame) {
                  if (!(i & 1)) {
                    AudioCommand_PlayFar(133);
                  }
                  ObjectGroup_UpdateMembers(work->effect->actors[n],
                                            SPRITE_COLOR_WHITE, POSE_DAMAGE, n,
                                            3);
                }
                n++;
              } while (n != work->effect->count);
            } else {
              reactFrame = start + 4;
            }

            // On frames 4 and 8, put a free puff of dust at the root.
            if ((frame == reactFrame) || (frame == (start + 8))) {
              /* FAKEMATCH: needed to match: the scan reuses length as its puff
               * pointer 2026-10-11: a block pointer of its own differs 48 (r5
               * and r6 swap); the puff pointer of the function 271. */
              length = (u32)gMapCellBuffer;
              n = 0;
              while (n != 512) {
                if (((struct EffectStep *)length)->variant == -1) {
                  ((struct EffectStep *)length)->x =
                      ((Random16() & 0xf) + column->x) - 8;
                  ((struct EffectStep *)length)->y = (Random16() & 0xf) + 80;
                  ((struct EffectStep *)length)->variant = 0;
                  break;
                }
                length += sizeof(struct EffectStep);
                n++;
              }
            }
            i++;
          } while (i != gDualTable_ColumnCounts[work->effect->variant]);
        }
      }

      // Dust: seven pictures, 2 frames each, drawn mirrored about the middle.
      i = 0;
      puff = gMapCellBuffer;
      do {
        puffAge = puff->variant;
        if (puffAge >= 0) {
          image = puffAge / 2;
          puffCells = 0x1e59;
          if (mode != 0) {
            puffCells = 0xaff;
          }
          ((DrawRectangle)rectangle[0])(
              canvas, work->sheet + (gDualTable_PuffCells[image] + puffCells),
              puff->x - gDualTable_PuffWidths[image],
              puff->y - (s8)gDualTable_PuffHeights[image] / 2,
              gDualTable_PuffWidths[image], (s8)gDualTable_PuffHeights[image]);
          ((DrawRectangle)rectangle[1])(
              canvas, work->sheet + (gDualTable_PuffCells[image] + puffCells),
              puff->x, puff->y - (s8)gDualTable_PuffHeights[image] / 2,
              gDualTable_PuffWidths[image], (s8)gDualTable_PuffHeights[image]);
          puff->variant++;
          if (puff->variant == 14) {
            puff->variant = -1;
          }
        }
        puff++;
        i++;
      } while (i != 512);

      Camera_ApplyShake(4, 4);
      EFFECT_END_FRAME(work);
      frame++;
    } while (frame != duration);
  }
  EFFECT_FINISH_CANVAS();
}

#define VINE_COLUMNS 16 // columns that rise
#define VINE_FRAMES 70  // frames in the effect

// Abilities: Vine (kind 202).
//
// Rising vines: sixteen green vines grow from the middle of the row of
// targets, 2 frames apart, and shrink back.
//
// effect: the targets.
void BattleFx_RunVine(struct BattleEffectArgument *effect) {
  u32 *slots, *cursor;
  struct BattleEffectWork *work;
  void *canvas;
  BlitterPair draw;
  s32 originX, originY;
  struct EffectPosition first, last;
  s32 i, frame, cell, height, offset, middle;
  struct EffectStep *column;

  slots = (u32 *)(gWorkSlot + HEAP_SLOT_BATTLE_EFFECT);
  cursor = slots;
  work = (struct BattleEffectWork *)*cursor++;
  canvas = (void *)*cursor;
  work->effect = effect;
  BattleFx_PrepareCanvasEffect(effect, 4, effect->side, 4, &originX, &originY);
  BattleFx_BeginCanvasLayer(1);
  REG_BG2PA = Q_8_8(1); // the canvas at its own width
  REG_BLDCNT = 0;
  Resource_LoadAndDecompress((s32)&ResourceId_GreenVineSheet, work->sheet, 1,
                             1);

  // Put the point halfway between the first and last targets at canvas x
  // 64.
  EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &first);
  EffectPosition_ApplyStepAndYOffset(
      work->effect->actors[work->effect->count - 1], &last);
  middle = first.x;
  middle += (last.x - middle) / 2;
  first.x = middle;
  REG_BG2X = (64 - first.x) << 8;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                      COMPOSITE_TRANSPARENT);
  draw[0] = (DrawRectangle)EFFECT_SLOT(slots, HEAP_SLOT_BLITTER);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                      COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X,
                      COMPOSITE_TRANSPARENT);
  draw[1] = (DrawRectangle)EFFECT_SLOT(slots, HEAP_SLOT_BLITTER_ALTERNATE);
  column = work->particles;
  i = 0;
  do {
    column[i].x = gVine_ColumnOffsets[i] + 64;
    i++;
  } while (i != VINE_COLUMNS);
  work->transferMode = CANVAS_COPY_AND_CLEAR; // clear the canvas every frame
  work->transferValue = 0;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);
  frame = 0;
  do {
    if (frame == 32) {
      AudioCommand_PlayFar(143);
      EFFECT_FOR_EACH_TARGET(i, work->effect)
        ObjectGroup_UpdateMembers(work->effect->actors[i], SPRITE_COLOR_WHITE,
                                  POSE_DAMAGE, i, 16);
    }
    i = 0;
    column = work->particles;
    do {
      if (frame == i * 4 + 5)
        work->shakeFrames = 2;

      // Grow 4 pixels a frame up to 32, and shrink from frame i * 2 + 32.
      offset = i * 2;
      if (frame > offset + 4) {
        cell = (frame / 4 + i) % 5; // five 32x32 pictures
        if (frame < offset + 32) {
          height = (frame - offset) * 4 - 16;
          if (height > 32)
            height = 32;
        } else {
          height = 160 - (frame - offset) * 4;
        }
        if (height > 0)
          draw[i & 1](canvas, work->sheet + (cell << 10), column->x - 16,
                      (i & 7) - height + 104, 32, height);
      }
      i++;
      column++;
    } while (i != VINE_COLUMNS);
    Camera_ApplyShake(4, 4);
    EFFECT_END_FRAME(work);
    frame++;
  } while (frame != VINE_FRAMES);
  EFFECT_FINISH_CANVAS();
}

// Abilities: Thorn, Briar, Nettle (kind 5).
//
// Rising thorns: thorns burst from the ground one every 8 frames, throwing
// up dust from their roots.
//
// effect: the targets; variant picks the number of thorns
//         (gThorn_Counts). gThorn_Kinds gives each
//         thorn's size and direction.
void BattleFx_RunThorn(struct BattleEffectArgument *effect) {
  void *canvas;
  s32 frame;
  DrawRectangle drawFlip;
  DrawRectangle draw;
  s32 frameCount;
  u32 *slots;
  u32 *cursor;
  struct BattleEffectWork *work;
  s32 i;
  s32 j;
  s32 age;
  s32 start;

  slots = (u32 *)(gWorkSlot + HEAP_SLOT_BATTLE_EFFECT);
  cursor = slots;
  work = (struct BattleEffectWork *)*cursor++;
  canvas = (void *)*cursor;
  work->effect = effect;
  BattleFx_BeginCanvasLayer(1);
  REG_BG2PA = Q_8_8(1); // the canvas at its own width
  REG_BLDCNT = 0;
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  Resource_LoadAndDecompress((s32)&ResourceId_ThornSheet, work->sheet, 1, 1);
  if (work->effect->side == 1) {
    REG_BG2X = -0x7000; // 112 pixels right
  }
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                      COMPOSITE_TRANSPARENT);
  draw = ((DrawRectangle *)slots)[HEAP_SLOT_BLITTER - HEAP_SLOT_BATTLE_EFFECT];
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                      COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X,
                      COMPOSITE_TRANSPARENT);
  work->transferMode = CANVAS_COPY_AND_CLEAR; // clear the canvas every frame
  work->transferValue = 0;
  drawFlip = ((DrawRectangle *)
                  slots)[HEAP_SLOT_BLITTER_ALTERNATE - HEAP_SLOT_BATTLE_EFFECT];
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);
  frameCount = gThorn_Counts[work->effect->variant] * 8 + 56;

  for (i = 0; i != 1024; i++) {
    PARTICLES[i].variant = -1; // free
  }

  for (frame = 0; frame != frameCount; frame++) {
    if (frame == frameCount - 64) {
      BattleEventRuntime_BeginPhaseFar(132);
    }
    if (frame >= frameCount - 16) {
      // Fade out over the last 16 frames.
      REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
      REG_BLDALPHA = (frameCount - frame - 1) | 0x1000;
    }

    for (i = 0; i != gThorn_Counts[work->effect->variant]; i++) {
      start = i * 8 + 8;
      age = frame - (i * 8 + 8);
      if (frame > start) {
        s32 height;
        s32 lean;

        // Kinds 0 and 1 are big thorns (48 wide), 2 and 3 small ones (32 wide);
        // odd kinds are mirrored. A thorn leans sideways as it grows.
        if (gThorn_Kinds[i] < 2) {
          height = age * 16;
          lean = age * 6;
          if (height > 80) {
            height = 80;
          }
          if (lean > 30) {
            lean = 30;
          }
          if (gThorn_Kinds[i] & 1) {
            drawFlip(canvas, work->sheet, gThorn_X[i] - lean, 108 - height, 48,
                     height);
          } else {
            draw(canvas, work->sheet, gThorn_X[i] + lean, 108 - height, 48,
                 height);
          }
        } else {
          height = age * 8;
          lean = age;
          if (height > 64) {
            height = 64;
          }
          if (age > 8) {
            lean = 8;
          }
          if (gThorn_Kinds[i] & 1) {
            drawFlip(canvas, work->sheet + 0xf00, gThorn_X[i] - lean,
                     108 - height, 32, height);
          } else {
            draw(canvas, work->sheet + 0xf00, gThorn_X[i] + lean, 108 - height,
                 32, height);
          }
        }
        if (frame == start + 1) {
          work->shakeFrames = 3;
        }

        // For 2 frames after the thorn appears, add a puff of dust each frame.
        if (frame < start + 3) {
          s32 y = (Random16() & 31) + 72;
          struct EffectStep *puff;
          for (j = 0; j != 64; j++) {
            puff = &PARTICLES[j];
            if (puff->variant == -1) {
              puff->x = gThorn_X[i] + (Random16() & 31) + 32;
              if (puff->x > 96) {
                puff->x = 96;
              }
              puff->y = y;
              puff->variant = 0;
              break;
            }
          }
        }
      }
      EFFECT_FOR_EACH_TARGET(j, work->effect) {
        if (frame == start + 4) {
          AudioCommand_PlayFar(132);
          ObjectGroup_UpdateMembers(work->effect->actors[j], SPRITE_COLOR_WHITE,
                                    POSE_DAMAGE, j, 3);
        }
      }
    }

    // Dust: seven pictures, 2 frames each, drawn mirrored about the middle.
    for (i = 0; i != 64; i++) {
      if (PARTICLES[i].variant >= 0) {
        s32 cel = PARTICLES[i].variant / 2;
        draw(canvas, work->sheet + gThorn_PuffCells[cel],
             PARTICLES[i].x - gThorn_PuffWidths[cel],
             PARTICLES[i].y - (s8)gThorn_PuffHeights[cel] / 2,
             gThorn_PuffWidths[cel], (s8)gThorn_PuffHeights[cel]);
        drawFlip(canvas, work->sheet + gThorn_PuffCells[cel], PARTICLES[i].x,
                 PARTICLES[i].y - (s8)gThorn_PuffHeights[cel] / 2,
                 gThorn_PuffWidths[cel], (s8)gThorn_PuffHeights[cel]);
        if (++PARTICLES[i].variant == 14) {
          PARTICLES[i].variant = -1;
        }
      }
    }
    Camera_ApplyShake(8, 8);
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS();
}

// Lightning bolts: a bolt strikes each target, with sparks and glints.
// PARTICLES holds the sparks, 128 for each target.
// The second blitter, made anew for each glint.
#define LIGHTNING_GLINT_BLITTER                                                \
  ((DrawRectangle *)                                                           \
       gBattleFxWork)[HEAP_SLOT_BLITTER_ALTERNATE - HEAP_SLOT_BATTLE_EFFECT]

// The columns of a gLightningBolts_Counts row.
enum {
  LIGHTNING_BOLT_SPARKS,       // sparks thrown from each bolt's foot
  LIGHTNING_BOLT_GLINTS,       // glints a frame around each target
  LIGHTNING_BOLT_GLINT_SPREAD, // how high and wide the glints spread
  LIGHTNING_BOLT_STRIKES       // bolts drawn over each other
};

// The value of one column of this effect's gLightningBolts_Counts row.
#define LIGHTNING_BOLT_VALUE(field)                                            \
  (gLightningBolts_Counts[work->effect->variant * 4 + (field)])

// The bolt pictures in the sheet, after the tornado.
#define LIGHTNING_BOLT_CELLS 0xc56

// Abilities: Bolt, Flash Bolt, Blue Bolt (kind 16).
//
// A bolt of lightning strikes each target in turn, 8 frames apart.
//
// effect: the targets; variant picks the row of gLightningBolts_Counts
//         and the glints' blend.
void BattleFx_RunBolt(struct BattleEffectArgument *effect) {
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 i;
  s32 frame;
  /* FAKEMATCH: frame local: only draw[0] is used; the unused second blitter
     word fills four bytes of the frame. Measured (2026-10-14): declared as one
     DrawRectangle, the frame is 60 bytes for 64 (differ 66, frame offsets
     only). */
  BlitterPair draw;
  u8 *sprites;
  struct EffectPosition screen;
  s32 j;

  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  sprites = EFFECT_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(1);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                      COMPOSITE_MAX);
  draw[0] = EFFECT_SLOT(slots, HEAP_SLOT_BLITTER);
  Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, work->sheet, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_LightningBoltSheet,
                             work->sheet + LIGHTNING_BOLT_CELLS, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sprites, 0, 0);
  for (j = 0; j != 1024; j++)
    PARTICLES[j].variant = 0;
  EFFECT_FREE_PARTICLES(j, work->particles, 64);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);
  AudioCommand_PlayFar(138);
  for (frame = 0; frame != work->effect->count * 8 + 40; frame++) {
    if (frame == 24)
      BattleEventRuntime_BeginPhaseFar(133);
    EFFECT_FOR_EACH_TARGET(i, work->effect) {
      if (frame == i * 8)
        Iwram_FillWords(canvas, 0x4000, 0x10101010);
    }
    EFFECT_FOR_EACH_TARGET(i, work->effect) {
      s32 start = i * 8;

      EffectPosition_ApplyAlternateStepAndYOffset(work->effect->actors[i],
                                                  &screen);
      screen.x /= 2;
      if (frame == start + 1)
        work->shakeFrames = 4;
      if (frame == start + 4) {
        ObjectGroup_UpdateMembers(work->effect->actors[i], SPRITE_COLOR_WHITE,
                                  POSE_DAMAGE, i, 6);
        BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 6);
      }

      // The bolt (24 wide) grows 64 pixels a frame and stops at 104.
      if (frame >= start && frame < start + 16) {
        s32 height = (frame - start) << 6;

        if (height > 104)
          height = 104;
        for (j = 0; j != LIGHTNING_BOLT_VALUE(LIGHTNING_BOLT_STRIKES); j++) {
          draw[0](canvas,
                  work->sheet +
                      (((i + frame + j) / 2) & 3) * LIGHTNING_BOLT_CELL_BYTES +
                      LIGHTNING_BOLT_CELLS,
                  screen.x - 12, 0, 24, height);
        }

        // Throw sparks upwards, within 90 degrees, from the foot (y = 104).
        if (frame == start + 2) {
          for (j = 0; j != LIGHTNING_BOLT_VALUE(LIGHTNING_BOLT_SPARKS); j++) {
            struct EffectStep *spark = &PARTICLES[i * 128 + j];
            s32 speed = (Random16() & 0x1ff) + 64;
            s32 angle = (Random16() & 0x7fff) - 0x4000;

            spark->x = screen.x << 16;
            spark->y = 104 << 16;
            spark->velocityX = (Trig_Sin(angle) * speed) >> 5;
            spark->velocityY = -(Trig_Cos(angle) * speed) >> 6;
            spark->variant = (Random16() & 7) + 32;
          }
        }
      }

      // Glints: at random spots above the foot, flipped at random.
      if (frame >= start + 2 && frame < start + 24) {
        for (j = 0; j != LIGHTNING_BOLT_VALUE(LIGHTNING_BOLT_GLINTS); j++) {
          s32 kind = j & 3;
          s32 drop =
              Random16() % LIGHTNING_BOLT_VALUE(LIGHTNING_BOLT_GLINT_SPREAD);
          s32 spread =
              LIGHTNING_BOLT_VALUE(LIGHTNING_BOLT_GLINT_SPREAD) - drop + 1;
          s32 y = screen.y - drop - (gBattleFx_GlintCellHeights[kind] >> 1) + 8;
          u32 random = Random16();
          s32 x = screen.x;

          x += random % spread;
          x -= spread / 2;
          x -= (gBattleFx_GlintCellWidths[kind] >> 1);

          EFFECT_LOAD_BLITTER(
              HEAP_SLOT_BLITTER_ALTERNATE,
              gLightningBolts_GlintPalettes[Random16() & 3] | COMPOSITE_CLIP_X |
                  COMPOSITE_CLIP_Y,
              gLightningBolts_GlintModes[work->effect->variant]);
          LIGHTNING_GLINT_BLITTER(
              canvas, work->sheet + gBattleFx_GlintCellOffsets[kind], x, y,
              gBattleFx_GlintCellWidths[kind],
              gBattleFx_GlintCellHeights[kind]);
          Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
        }
      }
    }

    // Sparks bounce off the ground at half speed and shrink as they die out.
    for (j = 0; j != 1024; j++) {
      struct EffectStep *spark = &PARTICLES[j];

      if (spark->variant > 0) {
        spark->variant--;
        EffectStep_AdvanceWithGravity2D(spark, 60, 0x1000);
        if (spark->y > 104 << 16) {
          spark->velocityY = -spark->velocityY / 2;
        } else if ((spark->x >= 0 && spark->x < 127 << 16) && spark->y >= 0) {
          s32 size = spark->variant / 16 + 1;
          s32 x = spark->x >> 16;
          s32 y = spark->y >> 16;

          EFFECT_DRAW_MOTE(draw[0], canvas, sprites, x, y, size);
        }
      }
    }
    Camera_ApplyShake(2, 8);
    EFFECT_END_FRAME(work);
  }
  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

// Djinni entrance: sparks swirl around a point that travels from the user
// to the goal and gather into the djinni, which sways for 16 frames and
// bursts into motes. Leaves the canvas cleared for the effect that
// follows.
//
// effect: the user and the targets.
// kind: the djinni: 0 Venus, 1 Mercury, 2 Mars, 3 Jupiter; 4 to 7 the same
//       over 84 frames instead of 64.
// side: 1 mirrors the pictures.
// anchor: where the djinni appears: 0 over the first target, 1 at the
//         first target's x at depth 0, 2 over the user, 3 at the user's x
//         at depth 0, 4 in the centre of the field.
// outX, outY: receive the djinni's last screen position.
void BattleFx_PrepareCanvasEffect(struct BattleEffectArgument *effect, s32 kind,
                                  s32 side, s32 anchor, s32 *outX, s32 *outY) {
  s32 center[3];
  s32 goal[3];
  s32 step[3];
  s32 point[3];
  struct EffectPosition position;
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  BlitterPair draw;
  u8 *sheet;
  s32 frameCount;
  struct MotionObject *target;
  struct MotionObject *user;
  u8 *palette;
  s32 sheetId;
  s32 frame;
  s32 i;

  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  sheet = EFFECT_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);
  work->fadeFrames = 24;
  work->fadeStep = 0;
  if (kind > 3) {
    kind -= 4;
    frameCount = 84;
  } else {
    frameCount = 64;
  }
  switch (kind) {
  case 0:
    sheetId = (s32)&ResourceId_VenusDjinnSmallSheet;
    break;
  case 1:
    sheetId = (s32)&ResourceId_MercuryDjinnSmallSheet;
    break;
  case 2:
    sheetId = (s32)&ResourceId_MarsDjinnSmallSheet;
    break;
  default:
    sheetId = (s32)&ResourceId_JupiterDjinnSmallSheet;
    break;
  }
  palette = Resource_GetTableEntry(sheetId);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  Resource_DecodeType01(palette + 128, work);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  if (side == 1) {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER,
                        COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X,
                        COMPOSITE_ADDITIVE);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                        COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X,
                        COMPOSITE_MAX);
  } else {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                        COMPOSITE_ADDITIVE);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                        COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, COMPOSITE_MAX);
  }
  draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
  draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
  user = GetBattleObjectSlotFar(work->effect->actor)->object;
  target = GetBattleObjectSlotFar(work->effect->actors[0])->object;
  // Sparks spread out flat from the centre, 20 to 51 pixels up.
  for (i = 0; i != 64; i++) {
    struct EffectStep *spark = &PARTICLES[i];
    s32 angle = Random16() & 0xffff;
    s32 speed = (Random16() & 255) + 128;

    EFFECT_SET_POSITION(spark, 0, ((Random16() & 31) + 20) << 16, 0);
    EFFECT_SET_VELOCITY(spark, (Trig_Sin(angle) * speed) >> 5, 0,
                        (Trig_Cos(angle) * speed) >> 5);
    spark->variant = 0;
  }
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);
  // The centre moves from the user to the goal, 60 pixels up, over frames
  // 6 to 45.
  EFFECT_SET_VECTOR(center, user->x, 0, user->z);
  switch (anchor) {
  case 0:
    EFFECT_SET_VECTOR(goal, target->x, 0x3c0000, target->z);
    break;
  case 1:
    EFFECT_SET_VECTOR(goal, target->x, 0x3c0000, 0);
    break;
  case 2:
    EFFECT_SET_VECTOR(goal, user->x, 0x3c0000, user->z);
    break;
  case 3:
    EFFECT_SET_VECTOR(goal, user->x, 0x3c0000, 0);
    break;
  case 4:
    EFFECT_SET_VECTOR(goal, 0, 0x3c0000, 0);
    break;
  }
  EFFECT_SET_VECTOR(step, (goal[0] - center[0]) / 40,
                    (goal[1] - center[1]) / 40, (goal[2] - center[2]) / 40);
  for (frame = 0; frame != frameCount; frame++) {
    struct BattleCamera *camera = gCameraWork;

    if (frame > 75)
      REG_BLDALPHA = (168 - frame * 2) | 0x1000;
    if (frame == 8)
      AudioCommand_PlayFar(212);
    EFFECT_VIEW_FROM_CAMERA(camera);
    if (frame >= 6 && frame <= 45) {
      center[0] += step[0];
      center[1] += step[1];
      center[2] += step[2];
    }
    SceneTransform_ApplyPosition(center);
    if (frame == 0)
      ObjectGroup_UpdateMembers(work->effect->actor, SPRITE_COLOR_WHITE, -1, -1,
                                0);
    if (frame == 24)
      ObjectGroup_UpdateMembers(work->effect->actor, 0, -1, -1, 0);
    // The sparks spin about the centre, each on its own axis; from 24
    // frames after appearing they are pulled in and go out at the centre.
    for (i = 0; i != 32; i++) {
      struct EffectStep *spark = &PARTICLES[i];

      if (frame >= i / 8 && spark->variant == 0) {
        s32 size;

        Transform_Push();
        switch (i & 3) {
        case 0:
          SceneTransform_ApplyYaw(frame * (i * 32 + 256));
          break;
        case 1:
          SceneTransform_ApplyPitch(-frame * (i * 32 + 256));
          break;
        case 2:
          SceneTransform_ApplyRoll(-frame * (i * 32 + 256));
          break;
        case 3:
          SceneTransform_ApplyPitch(-frame * (i * 32 + 256));
          SceneTransform_ApplyRoll(-frame * (i * 32 + 256));
          break;
        }
        EffectPosition_ApplyBaseAndYOffset(&spark->x, &position);
        position.x >>= 1;
        Transform_Pop();
        EFFECT_CLAMP(position.depth, 250, 634);
        size = 8 - (position.depth - 250) / 64;
        EFFECT_DRAW_MOTE(draw[1], canvas, sheet, position.x, position.y, size);
        EffectStep_AdvanceWithGravity3D(spark, 60, 0);
        if (frame >= i / 8 + 24) {
          s32 pullX = -spark->x >> 7;
          s32 pullY = -spark->y >> 7;
          s32 pullZ = -spark->z >> 7;

          spark->velocityX += pullX;
          spark->velocityY += pullY;
          spark->velocityZ += pullZ;
          EFFECT_SET_VELOCITY(spark, spark->velocityX * 62 / 64,
                              spark->velocityY * 62 / 64,
                              spark->velocityZ * 62 / 64);
          if ((pullX > -2048 && pullX < 2048) &&
              (pullZ > -2048 && pullZ < 2048))
            spark->variant = -1;
        }
      }
    }
    // The djinni sways at the centre on frames 54 to 69.
    if (frame >= 54 && frame <= 69) {
      EFFECT_SET_VECTOR(point, (Trig_Sin(frame << 10) * 4), 0, 0);
      EffectPosition_ApplyBaseAndYOffset(point, &position);
      *outX = position.x;
      *outY = position.y;
      position.x >>= 1;
      draw[0](canvas, work->sheet, position.x - 10, position.y - 20, 20, 40);
    }
    // At frame 64 motes burst from the djinni.
    if (frame == 64) {
      for (i = 0; i != 64; i++) {
        struct EffectStep *mote = &work->particles[i];
        s32 angle = Random16() & 0xffff;
        s32 speed = (Random16() & 255) + 128;

        mote->x = *outX << 15;
        mote->y = *outY << 16;
        mote->velocityX = (Trig_Sin(angle) * speed) >> 6;
        mote->velocityY = (Trig_Cos(angle) * speed) >> 5;
        mote->variant = (Random16() & 15) + 8;
      }
    }
    if (frame > 63) {
      for (i = 0; i != 64; i++) {
        struct EffectStep *mote = &work->particles[i];

        if (mote->variant >= 0) {
          s32 size = (mote->variant >> 3) + 2;

          EFFECT_DRAW_MOTE(draw[0], canvas, sheet, HI(mote->x), HI(mote->y),
                           size);
          EffectStep_AdvanceWithGravity2D(mote, 60, 0);
          mote->variant--;
        }
      }
    }
    EFFECT_PRESENT_FRAME(work);
  }
  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  EFFECT_RELEASE_BLITTERS();
  Scheduler_RemoveCallback(Palette_StepFadeTransfer);
  Iwram_ClearWords(BG_CHAR_BLOCK(1), 0x4000);
  Iwram_ClearWords(canvas, 0x4000);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
}

// Projectile volleys and rushing strikes.
//
// A projectile volley fires a row of shots from the user at each target. The
// mode (0 to 11) picks the shot picture, how the shots are aimed and which
// sounds play; the mode's row gives its number of shots per target, their
// flight time and the stagger between shots and targets. Mode entries retain
// their table indices; the wrappers name the mode of each ability.
//
// The volley runs in four parts every frame:
//
//   flight:     each launched shot moves one step along its aim and is drawn
//               on the half-width canvas;
//   collision:  a shot that passes its target's x on the target's side of the
//               field hits once: it starts a hit flash, throws sparks, shakes
//               the camera and makes the target reel;
//   flashes:    comet volleys draw each hit flash for 12 frames, as half a
//               picture drawn again flipped for the other half;
//   sparks and trails: the particles thrown by hits fall and bounce, and
//               those left behind fireballs rise.
//
// All particles share PARTICLES: hit flashes from 0, sparks from 100 and
// fireball trails from 512. A particle's variant is its frames left (0 when
// free); a flash's z counts the frames it has shown. The shots themselves
// live in work->particles, one per target and shot, whose variant is set once
// the shot has hit.
//
// The rushing strike sends the user at the first target; on frame 10 it
// strikes, a ring flares and 64 sparks scatter, over 60 frames in all.

// Actor animation steps while firing and after the volley finishes.
#define VOLLEY_ANIMATION_STEP_FAST 0x30
#define VOLLEY_ANIMATION_STEP_NORMAL 0x10

// Abilities: Slash, Wind Slash, Sonic Slash (kind 19).
//
// Fire the projectile volley using its first table row.
DEFINE_EFFECT_KIND(BattleFx_RunSlash, BattleFx_RunProjectileVolley, 0)

// Abilities: Shuriken (kind 54).
//
// Fire the projectile volley using its second table row.
DEFINE_EFFECT_KIND(BattleFx_RunShuriken, BattleFx_RunProjectileVolley, 1)

// Abilities: Crazy Voice (kind 303).
//
// Fire the projectile volley whose shots bounce on impact.
DEFINE_EFFECT_KIND(BattleFx_RunCrazyVoice, BattleFx_RunProjectileVolley, 2)

// Abilities: Wing Flutter (kind 307).
//
// Fire the projectile volley using table row 3.
DEFINE_EFFECT_KIND(BattleFx_RunWingFlutter, BattleFx_RunProjectileVolley, 3)

// Abilities: Wing Stroke (kind 381).
//
// Fire the projectile volley using table row 11.
DEFINE_EFFECT_KIND(BattleFx_RunWingStroke, BattleFx_RunProjectileVolley, 11)

// Abilities: Wing Beat (kind 306).
//
// Fire the projectile volley using table row 4.
DEFINE_EFFECT_KIND(BattleFx_RunWingBeat, BattleFx_RunProjectileVolley, 4)

// Puts the user into its action pose (2) and fires the volley in this mode
// while its animation runs fast.
static __inline__ void
BattleFx_RunVolleyWithAction(struct BattleEffectArgument *effect, s32 mode) {
  s32 user;

  user = (s32)GetBattleObjectSlotFar(effect->actor)->object;
  ObjectDispatch_ApplyArgumentToChildrenFar(user, 2);
  ObjectDispatch_ApplyValueToChildrenFar(user, VOLLEY_ANIMATION_STEP_FAST);
  BattleFx_RunProjectileVolley(effect, mode);
  ObjectDispatch_ApplyValueToChildrenFar(user, VOLLEY_ANIMATION_STEP_NORMAL);
}

// Abilities: Cutting Edge (kind 57).
//
// Volley mode 5, fired from the user's action pose.
DEFINE_EFFECT_KIND(BattleFx_RunCuttingEdge, BattleFx_RunVolleyWithAction, 5)

// Abilities: Heat Wave (kind 59).
//
// Volley mode 6, fired from the user's action pose.
DEFINE_EFFECT_KIND(BattleFx_RunHeatWave, BattleFx_RunVolleyWithAction, 6)

// Abilities: Sleet (kind 205).
//
// Fire the projectile volley with the delayed frame-48 hit cue.
DEFINE_EFFECT_KIND(BattleFx_RunSleet, BattleFx_RunProjectileVolley, 7)

// Abilities: Ice Blessing (kind 402).
//
// Fire the projectile volley using table row 10.
DEFINE_EFFECT_KIND(BattleFx_RunIceBlessing, BattleFx_RunProjectileVolley, 10)

// Abilities: Blizzard (kind 63).
//
// Volley mode 9, fired from the user's action pose.
DEFINE_EFFECT_KIND(BattleFx_RunBlizzard, BattleFx_RunVolleyWithAction, 9)

// Projectile volleys, and moving battlers about.
extern u8 gVolley_CometHeights[];
// Five bytes for each mode; see VolleyField.
extern u8 gVolley_Modes[];
extern u8 gVolley_FlashWidths[];
extern u8 gVolley_FlashHeights[];
extern u8 gVolley_FlashSteps[];
extern u8 gVolley_FlashRises[];
extern u16 gVolley_FlashOffsets[];
extern u16 gVolley_PelletOffsets[];

// PARTICLES holds the hit flashes from 0, the sparks from 100 and the
// fireball trails from 512.
#define VOLLEY_PARTICLE_COUNT 1024

// The bytes of a gVolley_Modes row.
enum VolleyField {
  VOLLEY_SHOTS,   // shots at each target
  VOLLEY_STEPS,   // frames a shot takes to reach its target
  VOLLEY_STAGGER, // frames between shots
  VOLLEY_DELAY,   // frames between targets
  VOLLEY_SPARKS   // sparks thrown on each hit
};

// The value of one field of this volley's gVolley_Modes row.
#define VOLLEY_VALUE(field) (gVolley_Modes[mode * 5 + (field)])

// Where the volley's pictures sit in the sheet.
#define VOLLEY_COMET_CELLS 0x1000  // comets: the top half of each
#define VOLLEY_STREAK_CELLS 0x2000 // wind streaks
#define VOLLEY_FLASH_TURNED 0x128a // the hit flashes turned on their side
#define VOLLEY_DJINNI_CELLS 0x65c0 // the small Mercury djinni
#define VOLLEY_FIRST_SPARK 100     // the first spark in PARTICLES
#define VOLLEY_FIRST_TRAIL 512     // the first trail in PARTICLES

// Projectile volley: shots fly from the user to each target and burst
// into a flash and sparks when they pass it.
//
// effect: the user and the targets. variant picks the comet in modes 0
//         and 8; with prelude 1 a Mercury djinni enters first.
// mode: the shot and its flight (0 to 11): 0, 5 and 8 comets (5 in the
//       ice palette), 1 shurikens, 2 bouncing pellets, 3, 4 and 11
//       yellow sparks, 6 fireballs with trails, 7, 9 and 10 ice chips.
void BattleFx_RunProjectileVolley(struct BattleEffectArgument *effect,
                                  s32 mode) {
  struct EffectPosition position;
  s32 screenX;
  s32 screenY;
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 i;
  s32 j;
  s32 frame;
  BlitterPair draw;
  struct BattleCamera *camera;
  u8 *sheet;
  s32 soundCooldown;
  s32 total;
  s32 kind;
  s32 shots;
  s32 steps;
  struct MotionObject *user;
  s32 k;
  // The trail particles placed, and later the sparks' speed shift: one
  // local for both, as separate locals change the register allocation.
  s32 trailsOrShift;
  s32 hit;
  s32 x;

  // The work, the canvas and the camera.
  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  camera = *(struct BattleCamera **)((u8 *)slots - (HEAP_SLOT_BATTLE_EFFECT -
                                                    HEAP_SLOT_CAMERA) *
                                                       sizeof(void *));
  sheet = EFFECT_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  soundCooldown = 0;
  work->effect = effect;
  kind = effect->variant;
  if (mode == 10)
    BattleFx_BeginCanvasLayer(0);
  else
    BattleFx_BeginCanvasLayer(1);
  if (work->effect->prelude == 1)
    BattleFx_PrepareCanvasEffect(effect, 1, work->effect->side, 2, &screenX,
                                 &screenY);

  // 128x128 drawing, mirrored on side 1.
  if (mode == 5) {
    if (work->effect->side == 1)
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_MAX);
    else
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS, COMPOSITE_MAX);
  } else {
    if (work->effect->side == 1)
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                          COMPOSITE_ADDITIVE);
    else
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS,
                          COMPOSITE_ADDITIVE);
  }
  draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];

  // The shot pictures.
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  if (mode == 0 || mode == 5 || mode == 8) {
    if (mode == 5)
      kind = 2;
    if (mode == 8)
      kind = 0;
    if (kind == 0)
      Resource_LoadAndDecompress((s32)&ResourceId_CometSheetA,
                                 work->sheet + VOLLEY_COMET_CELLS, 1, 1);
    else if (kind == 1)
      Resource_LoadAndDecompress((s32)&ResourceId_CometSheetB,
                                 work->sheet + VOLLEY_COMET_CELLS, 1, 1);
    else
      Resource_LoadAndDecompress((s32)&ResourceId_CometSheetC,
                                 work->sheet + VOLLEY_COMET_CELLS, 1, 1);
    if (mode == 5) {
      const void *pal = Resource_GetTableEntry((s32)&ResourceId_IceBlockSheet);
      {
        Iwram_CopyWords((void *)BG_PLTT, pal, 128);
      }
    }
    Resource_LoadAndDecompress((s32)&ResourceId_WindStreakSheet,
                               work->sheet + VOLLEY_STREAK_CELLS, 1, 0);
    if (mode == 5) {
      const void *pal = Resource_GetTableEntry((s32)&ResourceId_IceBlockSheet);
      {
        Iwram_CopyWords((void *)BG_PLTT, pal, 128);
      }
    }
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);
  } else if (mode == 1) {
    Resource_LoadAndDecompress((s32)&ResourceId_ShurikenSheet, work, 1, 1);
    REG_BLDCNT = 0;
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  } else if (mode == 2) {
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_CometSheetA), 128);
    Resource_LoadAndDecompress((s32)&ResourceId_ProjectileVolleyImage, work, 0,
                               0);
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, 50);
  } else if (mode == 3 || mode == 4 || mode == 11) {
    Resource_LoadAndDecompress((s32)&ResourceId_YellowSparkSheet, work, 1, 1);
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, 50);
  } else if (mode == 6) {
    Resource_LoadAndDecompress((s32)&ResourceId_FireballSheet, work, 1, 1);
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, 50);
  } else {
    Resource_LoadAndDecompress((s32)&ResourceId_IceChipSheet, work, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_MercuryDjinnSmallSheet,
                               work->sheet + VOLLEY_DJINNI_CELLS, 1, 0);
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, 50);
  }
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);

  // Where the shots start and how fast they fly.
  user = GetBattleObjectSlotFar(work->effect->actor)->object;
  shots = VOLLEY_VALUE(VOLLEY_SHOTS);
  steps = VOLLEY_VALUE(VOLLEY_STEPS);
  {
    s32 stagger = VOLLEY_VALUE(VOLLEY_STAGGER);
    s32 delay = VOLLEY_VALUE(VOLLEY_DELAY);
    s32 height = Battle_GetObjectTableValueFar(work->effect->actor);

    if (work->effect->count * shots > 63) // keep within particles[64]
      work->effect->count = 1;

    EFFECT_FOR_EACH_TARGET(i, work->effect) {
      struct MotionObject *target =
          GetBattleObjectSlotFar(work->effect->actors[i])->object;
      s32 targetHeight = Battle_GetObjectTableValueFar(work->effect->actors[i]);

      for (j = 0; j != shots; j++) {
        struct EffectStep *shot = &work->particles[i * shots + j];

        shot->x = user->x;
        if (mode == 7)
          shot->y = user->y + ((Random16() & 15) << 16) + 0x3a0000;
        else if (mode == 10)
          shot->y = user->y + height / 2;
        else if (mode == 6)
          shot->y = user->y + height / 2;
        else if (mode == 9)
          shot->y = user->y + height / 2 + ((16 - (Random16() & 31)) << 16);
        else if ((u32)(mode - 3) <= 1)
          shot->y = user->y + height / 2 + ((16 - (Random16() & 31)) << 16);
        else if (mode == 11)
          shot->y = user->y + height / 2 + ((32 - (Random16() & 63)) << 16);
        else if (mode == 5)
          shot->y = user->y + height / 2;
        else
          shot->y = user->y + height;
        shot->z = user->z;
        shot->velocityX = (target->x - shot->x) / steps;
        if (mode == 7)
          shot->velocityY =
              (s32)(target->y + ((Random16() & 63) << 16) - shot->y - 0xc0000) /
              steps;
        else if (mode == 8)
          shot->velocityY =
              (s32)(target->y + ((Random16() & 7) << 16) - shot->y + 0x160000) /
              steps;
        else if (mode == 9)
          shot->velocityY =
              (s32)(target->y + ((64 - (Random16() & 63)) << 16) - shot->y) /
              steps;
        else if (mode == 10)
          shot->velocityY =
              (s32)(target->y + ((Random16() & 31) << 16) - shot->y + 0x40000) /
              steps;
        else if ((u32)(mode - 3) <= 1 || mode == 11 || mode == 5)
          shot->velocityY = 0;
        else if (mode == 6)
          shot->velocityY = (s32)(target->y + targetHeight / 2 +
                                  ((Random16() & 15) << 16) - shot->y) /
                            steps;
        else
          shot->velocityY = (s32)(target->y + targetHeight -
                                  ((Random16() & 15) << 16) - shot->y) /
                            steps;
        shot->velocityZ = (target->z - shot->z) / steps;
        shot->variant = 0;
      }
    }
    for (i = 0; i != VOLLEY_PARTICLE_COUNT; i++)
      PARTICLES[i].variant = 0;
    if (mode == 6)
      total = work->effect->count * delay + shots * stagger + 32;
    else
      total = work->effect->count * delay + shots * stagger + 16;

    // Each frame: move and draw.
    for (frame = 0; frame != total; frame++) {
      s32 index;

      // Start playing the results.
      if (soundCooldown > 0)
        soundCooldown--;
      if (mode == 6) {
        if (frame == 4)
          AudioCommand_PlayFar(136);
        if (frame == 32)
          BattleEventRuntime_BeginPhaseFar(134);
      } else if (mode == 7) {
        if (frame == 48)
          BattleEventRuntime_BeginPhaseFar(133);
      } else if (mode != 5) {
        if (frame == 16)
          BattleEventRuntime_BeginPhaseFar(133);
      }

      // Prelude: the small djinni circles, and from frame 70 rises away.
      if (work->effect->prelude == 1) {
        s32 x = ((-Trig_Sin(frame << 11) * 4) >> 16) + screenX / 2 - 10;
        s32 y = ((Trig_Cos(frame << 11) * 2) >> 16) + screenY - 24;

        if (frame > 69)
          y = y - frame * 2 + 138;
        if (work->effect->side == 1)
          EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                              EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                              COMPOSITE_ADDITIVE);
        else
          EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE, EFFECT_CLIP_BOUNDS,
                              COMPOSITE_ADDITIVE);
        draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
        if (frame <= 3)
          draw[1](canvas, work->sheet + VOLLEY_DJINNI_CELLS, x, y, 20, 40);
        draw[1](canvas, work->sheet + VOLLEY_DJINNI_CELLS, x, y, 20, 40);
        Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
      }

      // The shots.
      EFFECT_VIEW_FROM_CAMERA(camera);
      index = 0;
      EFFECT_FOR_EACH_TARGET(i, work->effect) {
        struct MotionObject *target =
            GetBattleObjectSlotFar(work->effect->actors[i])->object;

        for (j = 0; j != shots; j++, index++) {
          if (j * stagger + i * delay < frame) {
            struct EffectStep *shot = &work->particles[i * shots + j];
            struct EffectPosition *screenPosition = &position;

            // The screen position (half across, on the canvas).
            EffectPosition_ApplyBaseAndYOffset(shot, screenPosition);
            screenPosition->x >>= 1;
            shot->x += shot->velocityX;
            shot->y += shot->velocityY;
            shot->z += shot->velocityZ;
            if (mode == 6) {
              // Leave two trail particles behind the fireball.
              trailsOrShift = 0;
              for (k = VOLLEY_FIRST_TRAIL; k != VOLLEY_PARTICLE_COUNT; k++) {
                struct EffectStep *particle = &PARTICLES[k];

                if (particle->variant == 0) {
                  particle->x = position.x << 16;
                  particle->y = position.y << 16;
                  particle->velocityX = ((Random16() & 255) - 128) << 11;
                  particle->velocityY = ((Random16() & 255) - 128) << 11;
                  particle->variant = (Random16() & 7) + 32;
                  trailsOrShift++;
                  if (trailsOrShift == 2)
                    break;
                }
              }
            }
            if (shot->variant == 0) {
              // A shot hits once it is on the target's side of the field and
              // past the target's x.
              x = shot->x;
              if (target->x < 0) {
                hit = x < 0;
                /* FAKEMATCH: cancelling updates under if (target) end CSE
                 * before the hit test and emit nothing, keeping the native sign
                 * result and duplicated side check (per-side test, goto
                 * native); unguarded 77 (3 English insns lost), one test after
                 * an if/else 65, ?: of both sign tests 65 (77 tested whole),
                 * hit preset 0 then nested ifs 53, tests on shot->x 65, these
                 * guards plus one test after the if/else 55. */
                if (target) {
                  hit ^= x;
                  hit ^= x;
                }
                if (!hit)
                  goto drawShot;
              } else {
                hit = x >= 0;
                /* FAKEMATCH: the same CSE block end keeps the
                 * opposite sign result before its side check;
                 * the cancelling updates emit no instructions. */
                if (target) {
                  hit ^= x;
                  hit ^= x;
                }
                if (!hit)
                  goto drawShot;
              }
              if (EFFECT_ABS(x) >= EFFECT_ABS(target->x)) {
                s32 spawned = 0;
                struct EffectStep *flash = &PARTICLES[index];

                shot->variant = 1;
                if (mode == 5) {
                  BattleEventRuntime_BeginPhaseFar(134);
                } else if (mode != 6 && soundCooldown == 0) {
                  soundCooldown = 8;
                  AudioCommand_PlayFar(132);
                }
                if (mode == 2) {
                  // Bounce off.
                  s32 kick = ((Random16() & 31) + 32) << 12;

                  if (Random16() & 1)
                    shot->velocityY += kick;
                  else
                    shot->velocityY -= kick;
                  shot->velocityX = -shot->velocityX;
                }
                flash->variant = 1;
                EFFECT_SET_POSITION(flash, screenPosition->x, screenPosition->y,
                                    0);
                if (mode != 7)
                  work->shakeFrames = 2;
                ObjectGroup_UpdateMembers(work->effect->actors[i],
                                          SPRITE_COLOR_WHITE, POSE_DAMAGE, i,
                                          8);
                if (mode != 7 && mode != 9 && mode != 10) {
                  if (mode == 5)
                    BattleMotion_ApplyVariantMotionFar(work->effect->actors[i],
                                                       4);
                  else
                    BattleMotion_ApplyVariantMotionFar(work->effect->actors[i],
                                                       5);
                }
                // Sparks.
                trailsOrShift = mode == 5 ? 12 : 11;
                for (k = VOLLEY_FIRST_SPARK; k != VOLLEY_FIRST_TRAIL; k++) {
                  struct EffectStep *particle = &PARTICLES[k];

                  if (particle->variant == 0) {
                    particle->x = position.x << 16;
                    particle->y = position.y << 16;
                    particle->velocityX = ((Random16() & 255) - 128)
                                          << trailsOrShift;
                    particle->velocityY = ((Random16() & 255) - 128)
                                          << trailsOrShift;
                    particle->variant = (Random16() & 7) + 16;
                    spawned++;
                    if (spawned == VOLLEY_VALUE(VOLLEY_SPARKS))
                      break;
                  }
                }
              }
            }
          drawShot:
            if (mode == 0 || mode == 5 || mode == 8) {
              // The picture holds the top half; the bottom half is drawn
              // flipped.
              if (work->effect->side == 1)
                EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                                    EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                                    COMPOSITE_MAX);
              else
                EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                                    EFFECT_CLIP_BOUNDS, COMPOSITE_MAX);
              ((DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE])(
                  canvas, work->sheet + VOLLEY_COMET_CELLS,
                  screenPosition->x - 16,
                  screenPosition->y - gVolley_CometHeights[kind], 32,
                  gVolley_CometHeights[kind]);
              Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
              if (work->effect->side == 1)
                EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                                    EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP,
                                    COMPOSITE_MAX);
              else
                EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                                    EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y,
                                    COMPOSITE_MAX);
              ((DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE])(
                  canvas, work->sheet + VOLLEY_COMET_CELLS,
                  screenPosition->x - 16, screenPosition->y, 32,
                  gVolley_CometHeights[kind]);
              Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
            } else if (mode == 1) {
              if (EFFECT_ABS(shot->x) <= EFFECT_ABS(target->x))
                draw[0](canvas, work->sheet + frame % 6 * 768,
                        screenPosition->x - 16, screenPosition->y - 12, 32, 24);
            } else if (mode == 7 || mode == 9 || mode == 10) {
              s32 cell = j & 3;
              draw[0](canvas, work->sheet + gVolley_PelletOffsets[cell],
                      screenPosition->x - 4, screenPosition->y - 4, 8, 8);
            } else if (mode == 2) {
              draw[0](canvas, work->sheet + j % 6 * 128, screenPosition->x - 4,
                      screenPosition->y - 8, 8, 16);
            } else if (mode == 3) {
              draw[0](canvas, work->sheet + 96, screenPosition->x - 9,
                      screenPosition->y - 7, 18, 13);
            } else if (mode == 4) {
              draw[0](canvas, work->sheet, screenPosition->x - 6,
                      screenPosition->y - 4, 12, 8);
            } else if (mode == 11) {
              draw[0](canvas, work->sheet + 330, screenPosition->x - 15,
                      screenPosition->y - 12, 29, 23);
            } else {
              draw[0](canvas, work->sheet, screenPosition->x - 20,
                      screenPosition->y - 32, 40, 64);
            }
          }
        }
      }

      // Hit flashes, 12 frames: half a picture, drawn again flipped for the
      // other half (odd flashes use the pictures turned on their side).
      if (mode == 0 || mode == 5 || mode == 8) {
        for (i = 0; i != work->effect->count * shots; i++) {
          struct EffectStep *flash = &PARTICLES[i];

          if (flash->variant == 1) {
            s32 step = flash->z / 2;

            if (!(i & 1)) {
              EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                                  EFFECT_CLIP_BOUNDS, COMPOSITE_MAX);
              ((DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE])(
                  canvas, work->sheet + gVolley_FlashOffsets[step],
                  flash->x - gVolley_FlashWidths[step],
                  flash->y - gVolley_FlashRises[step],
                  gVolley_FlashWidths[step], gVolley_FlashHeights[step]);
              Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
              EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                                  EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP,
                                  COMPOSITE_MAX);
              ((DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE])(
                  canvas, work->sheet + gVolley_FlashOffsets[step], flash->x,
                  flash->y + gVolley_FlashSteps[step] -
                      gVolley_FlashRises[step],
                  gVolley_FlashWidths[step], gVolley_FlashHeights[step]);
              Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
            } else {
              EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                                  EFFECT_CLIP_BOUNDS, COMPOSITE_MAX);
              ((DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE])(
                  canvas,
                  work->sheet + gVolley_FlashOffsets[step] +
                      VOLLEY_FLASH_TURNED,
                  flash->x - gVolley_FlashRises[step],
                  flash->y - gVolley_FlashWidths[step],
                  gVolley_FlashHeights[step], gVolley_FlashWidths[step]);
              Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
              EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                                  EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP,
                                  COMPOSITE_MAX);
              ((DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE])(
                  canvas,
                  work->sheet + gVolley_FlashOffsets[step] +
                      VOLLEY_FLASH_TURNED,
                  flash->x + gVolley_FlashSteps[step] -
                      gVolley_FlashRises[step],
                  flash->y, gVolley_FlashHeights[step],
                  gVolley_FlashWidths[step]);
              Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
            }
            flash->z++;
            if (flash->z == 12)
              flash->variant = 0;
          }
        }
      }

      // Sparks fall and bounce off the bottom (y 112).
      for (i = VOLLEY_FIRST_SPARK; i != VOLLEY_FIRST_TRAIL; i++) {
        struct EffectStep *particle = &PARTICLES[i];

        if (particle->variant > 0) {
          s32 size = (particle->variant >> 3) + 1;

          EFFECT_DRAW_MOTE(draw[0], canvas, sheet, HI(particle->x),
                           HI(particle->y), size);
          EffectStep_AdvanceWithGravity2D(particle, 60, 0x1000);
          if (particle->y > 0x700000)
            particle->velocityY = -particle->velocityY / 2;
          particle->variant--;
        }
      }

      // Fireball trails rise.
      for (i = VOLLEY_FIRST_TRAIL; i != VOLLEY_PARTICLE_COUNT; i++) {
        struct EffectStep *particle = &PARTICLES[i];

        if (particle->variant > 0) {
          s32 size = (particle->variant >> 4) + 1;

          EFFECT_DRAW_MOTE(draw[0], canvas, sheet, HI(particle->x),
                           HI(particle->y), size);
          EffectStep_AdvanceWithGravity2D(particle, 60, -0x4000);
          particle->variant--;
        }
      }
      Camera_ApplyShake(4, 4);
      EFFECT_END_FRAME(work);
    }
  }

  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

// Sends a battler back to its place (its anchor) with a hop.
//
// id: the battler.
void BattleFx_InitializeRisingObject(s32 id) {
  struct BattleObj *slot = GetBattleObjectSlotFar(id);
  struct MotionObject *object = slot->object;

  object->acceleration = 0x20000;
  object->speedLimit = 0x80000; // 8 pixels a frame
  object->velocityY = 0x40000;
  object->verticalMotionStrength = 0xab85;
  object->verticalMotionPhase = 0;
  object->autoFaceMotion = 0;
  object->snapToTarget = 1;
  ObjectDispatch_InitFromTable6Far(object);
  Object_SetMoveTargetFar(object, slot->anchorX, 0, slot->anchorZ);
  ObjectDispatch_ApplyArgumentToChildrenFar(object, 1);
}

// Moves a battler 80% of the way to another one over a number of frames,
// in its action pose.
//
// id: the battler that moves.
// targetId: the battler it moves towards.
// frames: how long the move takes.
void BattleFx_SetApproachMotion(s32 id, s32 targetId, s32 frames) {
  struct BattleObj *slot = GetBattleObjectSlotFar(id);
  struct BattleObj *targetSlot = GetBattleObjectSlotFar(targetId);
  struct MotionObject *object = slot->object;
  struct MotionObject *target = targetSlot->object;
  s32 rate = 80; // percent of the way
  s32 diffX = target->x - object->x;
  s32 startX = object->x;
  s32 dx = rate * diffX / 100;
  s32 diffZ = target->z - object->z;
  s32 startZ = object->z;
  s32 dz = rate * diffZ / 100;
  s32 x = startX + dx;
  s32 z = startZ + dz;
  s32 stepX = dx >> 8; // the distance is measured in 1/256 pixels
  s32 stepZ = dz >> 8;
  s32 speed;

  speed = Iwram_Sqrt(stepX * stepX + stepZ * stepZ);
  speed = (speed << 8) / frames;
  object->acceleration = speed;
  object->speedLimit = speed;
  object->snapToTarget = 1;
  object->verticalMotionStrength = 0xab85;
  object->velocityY = 0;
  object->verticalMotionPhase = 0;
  object->autoFaceMotion = 1;
  ObjectDispatch_InitFromTable6Far(object);
  Object_SetMoveTargetFar(object, x, 0, z);
  ObjectDispatch_ApplyArgumentToChildrenFar(object, 2);
}

// Packs 288 rows of 40 bytes to half width, keeping every second byte
// (the bytes of the odd columns win).
//
// src: the rows to pack.
// dst: receives the packed rows, half the stride apart.
// stride: the bytes in one row of src.
// rowCount: ignored; 288 rows are always packed.
void Graphics_PackTileRows(u8 *src, u8 *dst, s32 stride, s32 rowCount) {
  s32 row = 0;
  s32 rows = 288;
  s32 offset = 0;

  do {
    s32 x;

    for (x = 0; x != 40; x++) {
      dst[offset / 2 + x / 2] = src[offset + x];
    }
    offset += stride;
    row++;
  } while (row != rows);
}

#define RUSHING_STRIKE_SPARK_COUNT 64   // sparks
#define RUSHING_STRIKE_RING_BYTES 0x3c0 // one 20x48 ring picture

// Abilities: Bone Charge (kind 311).
//
// The rushing strike in each of its four palettes.
DEFINE_EFFECT_KIND(BattleFx_RunBoneCharge, BattleFx_RunRushingStrike, 0)

// Abilities: Brute Force (kind 314).
//
// Rush the target and scatter sparks in the ice-block palette.
DEFINE_EFFECT_KIND(BattleFx_RunBruteForce, BattleFx_RunRushingStrike, 1)

// Abilities: Onslaught (kind 326).
//
// Rush the target and scatter sparks in the pink-burst palette.
DEFINE_EFFECT_KIND(BattleFx_RunOnslaught, BattleFx_RunRushingStrike, 2)

// Abilities: Headbutt (kind 339).
//
// Rush the target and scatter sparks in the blast palette.
DEFINE_EFFECT_KIND(BattleFx_RunHeadbutt, BattleFx_RunRushingStrike, 3)

#define RUSHING_STRIKE_FRAMES 60 // frames in the effect
#define RUSHING_STRIKE_FRAME 10  // the user strikes

// Rushing strike: the user rushes at the first target and strikes it on
// frame 10; a ring flares where it hits and 64 sparks scatter, over 60
// frames in all.
//
// effect: the user and the targets.
// variant: the palette: 0 fire pillar, 1 ice block, 2 pink burst, 3 blast.
void BattleFx_RunRushingStrike(struct BattleEffectArgument *effect,
                               s32 variant) {
  /* FAKEMATCH: needed to match: the blitter/camera declarations and independent
   * store order preserve spill slots and loop preheader order 2026-10-11: the
   * camera read is the plain slot now; Ram_MapCellBuffer at the two uses of
   * cells differs 2 (its load moves ahead of lsls r3 at the packing call). */
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  BlitterPair draw;
  struct BattleCamera *camera;
  u8 *sheet;
  struct EffectStep *spark;
  struct EffectStep *newSpark;
  struct EffectPosition origin;
  struct EffectPosition screen;
  struct EffectPosition sparkScreen;
  struct MotionObject *target;
  s32 frame;
  s32 i;
  s32 size;
  s32 ringFrame;
  s32 paletteId;
  u8 *cells;

  // The work, the canvas and the camera.
  cells = Ram_MapCellBuffer;
  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  camera = EFFECT_SLOT(slots, HEAP_SLOT_CAMERA);
  sheet = EFFECT_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);

  // The top half, and the bottom half flipped; mirrored on side 1.
  if (work->effect->side == 0) {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS, COMPOSITE_MAX);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y, COMPOSITE_MAX);
  } else {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_MAX);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP, COMPOSITE_MAX);
  }
  draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
  draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];

  // Pictures and palette: the ring is the yellow orb packed to half width.
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_YellowOrbSheet, work->sheet, 1,
                             0);
  Graphics_PackTileRows(work->sheet, (u8 *)cells, 40, 288);
  Resource_LoadAndDecompress((s32)&ResourceId_BlueBurstSheet, work->sheet, 1,
                             1);
  switch (variant) {
  case 0:
    paletteId = (s32)&ResourceId_FirePillarSheetB;
    break;
  case 1:
    paletteId = (s32)&ResourceId_IceBlockSheet;
    break;
  case 2:
    paletteId = (s32)&ResourceId_PinkBurstSheet;
    break;
  default:
    paletteId = (s32)&ResourceId_BlastSheet;
    break;
  }
  Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(paletteId), 128);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);

  // Send the user in and ready the sparks at the target (they fly towards
  // the middle of the field).
  BattleFx_SetApproachMotion(work->effect->actor, work->effect->actors[0], 10);
  target = GetBattleObjectSlotFar(work->effect->actors[0])->object;
  for (i = 0; i != RUSHING_STRIKE_SPARK_COUNT; i++) {
    newSpark = &work->particles[i];
    newSpark->x = target->x;
    newSpark->y = target->y + 0xa0000; // 10 pixels up
    newSpark->z = target->z;
    EFFECT_SET_VELOCITY(newSpark, (Random16() & 0x1ff) << 11,
                        ((Random16() & 255) - 64) << 11,
                        ((Random16() & 255) - 128) << 11);
    if (newSpark->x > 0)
      newSpark->velocityX = -newSpark->velocityX;
    newSpark->variant = i / 2 + 16; // frames left
  }
  EffectPosition_ApplyAlternateStepAndYOffset(work->effect->actors[0], &origin);

  for (frame = 0; frame != RUSHING_STRIKE_FRAMES; frame++) {
    // For 15 frames, draw the burst at the user in two halves.
    if (frame <= 14) {
      EffectPosition_ApplyAlternateStepAndYOffset(work->effect->actor, &screen);
      draw[0](canvas, work->sheet, screen.x / 2 - 16, screen.y - 48, 40, 32);
      draw[1](canvas, work->sheet, screen.x / 2 - 16, screen.y - 16, 40, 32);
    }
    // The strike.
    if (frame == RUSHING_STRIKE_FRAME) {
      ObjectGroup_UpdateMembers(work->effect->actors[0], SPRITE_COLOR_WHITE,
                                POSE_DAMAGE, 0, 8);
      BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 4);
      BattleEventRuntime_BeginPhaseFar(134);
      work->shakeFrames = 8;
    }
    // The ring: six pictures, 2 frames each, from frame 8.
    ringFrame = frame - 8;
    if (ringFrame >= 0 && ringFrame <= 11) {
      s32 ring = ringFrame / 2;
      draw[0](canvas, cells + ring * RUSHING_STRIKE_RING_BYTES,
              origin.x / 2 - 16, screen.y - 40, 20, 48);
    }
    // The sparks.
    if (ringFrame >= 0 && ringFrame <= 55) {
      EFFECT_VIEW_FROM_CAMERA(camera);
      for (i = 0; i != RUSHING_STRIKE_SPARK_COUNT; i++) {
        spark = &work->particles[i];
        size = spark->variant;
        if (size > 0) {
          EffectPosition_ApplyBaseAndYOffset(&spark->x, &sparkScreen);
          size >>= 4;
          size += 2;
          sparkScreen.x >>= 1;
          EFFECT_DRAW_MOTE(draw[0], canvas, sheet, sparkScreen.x, sparkScreen.y,
                           size);
          EffectStep_AdvanceWithGravity3D(spark, 60, -0x200);
          spark->variant--;
        }
      }
    }
    Camera_ApplyShake(8, 8);
    EFFECT_END_FRAME(work);
  }
  EFFECT_FINISH_CANVAS();
}

// Copies a w x h byte picture into an h x w one turned 90 degrees
// clockwise: the top row of src becomes the right column of dst.
void Graphics_TransposeCopy(const u8 *src, u8 *dst, s32 w, s32 h) {
  s32 y;
  s32 x;
  s32 column;
  s32 offset;
  u8 *to;
  const u8 *from;

  y = 0;
  if (h != 0) {
    offset = 0;
    column = h;
    do {
      x = 0;
      if (w != 0) {
        to = (u8 *)(column + (u32)dst - 1);
        from = (const u8 *)(offset + (u32)src);
        do {
          x++;
          *to = *from;
          from++;
          to += h;
        } while (x != w);
      }
      y++;
      offset += w;
      column--;
    } while (y != h);
  }
}

// Djinni attacks, reveal particles and camera shake.
//
// Each attack draws one of the small Djinn on the effect canvas and throws its
// element at the targets:
//
// - The Mercury Djinni spirals through the centre of the field and fires five
//   blue flames to the left, each shattering into motes.
// - The Jupiter Djinni spirals the other way while eight flameballs fall from
//   the top right and shatter on the ground.
// - The Venus Djinni sways while ten flame columns rise from the ground, each
//   throwing motes upwards.
// - The Mars Djinni flares from a glowing orb, dives to the ground and sets
//   nine flames erupting in an arc while embers rise.
// - The Venus Djinni of the rock toss (Ground) throws six rocks from the
//   user, shedding chips, which swirl and home in on the first target.
//
// Every attack sets up the canvas, its blitters and its particles, then runs
// a fixed number of frames, each a list of steps run in a fixed order and
// ending with a canvas present, which waits for the next frame. The order
// matters: later steps draw over earlier ones and several consume random
// numbers.
//
// The motes the flames shatter into live in PARTICLES, 32 slots per flame;
// the flames, rocks and chips themselves live in work->particles. The
// appearance prelude leaves the canvas ready for the calling attack; each
// attack removes its callbacks before returning. Sheet cell layouts select
// the images.

#define MERCURY_FRAMES 96          // frames in the effect
#define MERCURY_FLAMES 5           // flames thrown
#define MERCURY_MOTES_PER_FLAME 32 // motes in each flame

// Abilities: Mercury (kind 251).
//
// Mercury djinni flames: the small Mercury djinni spirals around (96, 60)
// and fires five blue flames to the left; each flame shatters into 32
// motes after 5 frames, hitting every target.
//
// effect: the targets.
void BattleFx_RunMercury(struct BattleEffectArgument *effect) {
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  u8 *sheet;
  BlitterPair blit;
  DrawRectangle *draw;
  struct EffectStep *flame;
  struct EffectStep *mote;
  u8 *cells;
  s32 i;
  s32 j;

  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  sheet = EFFECT_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);
  draw = blit;
  BattleFx_FetchRectangleBlitters(0, draw);

  // The flame pictures are the blue flame sheet turned on its side.
  cells = Ram_MapCellBuffer;
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_MercuryDjinnSmallSheet,
                             work->sheet, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_BlueFlameSheet, cells, 1, 1);
  Graphics_TransposeCopy(cells, work->sheet + 0x2a8, 17, 104);
  Graphics_TransposeCopy(cells + 0x6e8, work->sheet + 0x990, 34, 65);
  work->transferMode = CANVAS_COPY_AND_FADE;
  work->transferValue = 75; // each transfer fades the canvas to 3/4
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);
  EFFECT_FREE_PARTICLES(i, work->particles, 8);
  EFFECT_FREE_PARTICLES(i, PARTICLES, 512);
  AudioCommand_PlayFar(162);
  frame = 0;
  do {
    if (frame == 56)
      BattleEventRuntime_BeginPhaseFar(133);

    // Flames move 12 pixels left a frame and shatter on their 5th frame.
    for (i = 0; i != MERCURY_FLAMES; i++) {
      flame = &work->particles[i];
      if (flame->variant != -1) {
        blit[0](canvas, work->sheet + 0x990, flame->x - 16, flame->y - 17, 65,
                34);
        flame->x -= 12;
        flame->variant++;
        if (flame->variant == 5) {
          AudioCommand_PlayFar(133);
          work->shakeFrames = 4;
          for (j = 0; j != MERCURY_MOTES_PER_FLAME; j++) {
            s32 angle;
            s32 speed;

            mote = &PARTICLES[i * MERCURY_MOTES_PER_FLAME + j];
            angle = Random16() & 0xffff;
            speed = (Random16() & 0x1ff) + 256;
            mote->x = flame->x << 16;
            mote->y = flame->y << 16;
            mote->velocityX = (Trig_Sin(angle) * speed) >> 8;
            mote->velocityY = (Trig_Cos(angle) * speed) >> 7;
            mote->variant = (Random16() & 15) + 32; // lives 32 to 47 frames
          }
        }
      }
    }

    // The djinni turns once every 32 frames. Its radius, 64 - 2 * frame
    // (half that across), reaches 0 at frame 32, passing through the
    // centre, and then grows on the other side.
    if (frame <= 95) {
      s32 x = ((Trig_Sin(frame << 11) * (64 - frame * 2)) >> 17) + 96;
      s32 y = ((Trig_Cos(frame << 11) * (64 - frame * 2)) >> 16) + 60;

      draw[1](canvas, work->sheet, x - 10, y - 17, 20, 34);
      for (i = 0; i != MERCURY_FLAMES; i++) {
        // Fire a flame on each launch frame; 6 frames later the targets flash
        // white in their damage pose for 6 frames, and motion 6.
        if (frame == gMercury_LaunchFrames[i]) {
          struct EffectStep *shot = &work->particles[i];

          if (shot->variant == -1) {
            shot->x = x - 8;
            shot->velocityX = x;
            shot->y = y;
            shot->variant = 0;
            break;
          }
        }
        if (frame == gMercury_LaunchFrames[i] + 6) {
          EFFECT_FOR_EACH_TARGET(j, work->effect) {
            ObjectGroup_UpdateMembers(work->effect->actors[j],
                                      SPRITE_COLOR_WHITE, POSE_DAMAGE, j, 6);
            BattleMotion_ApplyVariantMotionFar(work->effect->actors[j], 6);
          }
        }
      }
    }

    // Motes shrink as they age and slow as they fall.
    for (i = 0; i != 256; i++) {
      mote = &PARTICLES[i];
      if (mote->variant != -1) {
        s32 size = mote->variant / 16 + 2;

        EFFECT_DRAW_MOTE(draw[1], canvas, sheet, HI(mote->x), HI(mote->y),
                         size);
        EffectStep_AdvanceWithGravity2D(mote, 62, 0x2000);
        mote->variant--;
      }
    }
    Camera_ApplyShake(4, 4);
    EFFECT_END_FRAME(work);
    frame++;
  } while (frame != MERCURY_FRAMES);
  EFFECT_FINISH_CANVAS();
}

#define JUPITER_FRAMES 96    // frames in the effect
#define JUPITER_FLAMEBALLS 8 // flameballs that fall
#define JUPITER_GROUND_Y 95  // where the flameballs land

// Abilities: Jupiter (kind 253).
//
// Jupiter djinni fall: the small Jupiter djinni spirals while eight
// flameballs fall from the top right to the bottom left, 4 frames apart;
// each shatters into 32 motes below line 95, hitting every target.
//
// effect: the targets.
void BattleFx_RunJupiter(struct BattleEffectArgument *effect) {
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  u8 *sheet;
  BlitterPair blit;
  DrawRectangle *draw;
  struct EffectStep *ball;
  struct EffectStep *mote;
  s32 frame;
  s32 i;
  s32 j;
  s32 angle;

  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  sheet = EFFECT_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  draw = blit;
  BattleFx_FetchRectangleBlitters(0, draw);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_JupiterDjinnSmallSheet,
                             work->sheet, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_FlameballSheet,
                             work->sheet + 0x320, 1, 0);
  work->transferMode = CANVAS_COPY_AND_FADE;
  work->transferValue = 75; // each transfer fades the canvas to 3/4
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);

  // The flameballs start at x 64 to 127, y -80 to -17.
  for (i = 0; i != 32; i++) {
    ball = &work->particles[i];
    ball->x = (Random16() & 63) + 64;
    ball->y = (Random16() & 63) - 80;
  }
  EFFECT_FREE_PARTICLES(i, PARTICLES, 512);
  AudioCommand_PlayFar(171);
  frame = 0;
  angle = 0x8000; // 180 degrees
  do {
    if (frame == 56)
      BattleEventRuntime_BeginPhaseFar(133);

    // The djinni turns 11.25 degrees a frame. Its radius, 64 - 2 * frame
    // (half that across), reaches 0 at frame 32, passing through the
    // centre, and then grows on the other side.
    if (frame <= 95) {
      s32 x = (Trig_Sin(angle) * (64 - frame * 2)) >> 17;
      s32 y = (Trig_Cos(angle) * (64 - frame * 2)) >> 16;

      blit[0](canvas, work->sheet, x + 86, y + 28, 20, 40);
    }

    // Flameballs: one every 4 frames, moving 6 pixels left and 12 down a
    // frame.
    for (i = 0; i != JUPITER_FLAMEBALLS; i++) {
      ball = &work->particles[i];
      if (frame >= i * 4 + 8 && ball->y <= JUPITER_GROUND_Y) {
        blit[0](canvas, work->sheet + 0x320, ball->x - 20, ball->y - 32, 40,
                64);
        ball->x -= 6;
        ball->y += 12;
        if (ball->y > JUPITER_GROUND_Y) {
          for (j = 0; j != 32; j++) {
            s32 direction;
            s32 speed;

            mote = &PARTICLES[i * 32 + j];
            direction = Random16() & 0xffff;
            speed = (Random16() & 0x1ff) + 256;
            mote->x = ball->x << 16;
            mote->y = ball->y << 16;
            mote->velocityX = (Trig_Sin(direction) * speed) >> 7;
            mote->velocityY = (Trig_Cos(direction) * speed) >> 6;
            mote->variant = (Random16() & 15) + 32; // lives 32 to 47 frames
          }
          AudioCommand_PlayFar(133);
          work->shakeFrames = 4;
          EFFECT_FOR_EACH_TARGET(j, work->effect) {
            ObjectGroup_UpdateMembers(work->effect->actors[j],
                                      SPRITE_COLOR_WHITE, POSE_DAMAGE, j, 6);
            BattleMotion_ApplyVariantMotionFar(work->effect->actors[j], 6);
          }
        }
      }
    }

    // Motes shrink as they age and slow as they fall.
    for (i = 0; i != 512; i++) {
      mote = &PARTICLES[i];
      if (mote->variant != -1) {
        s32 size = mote->variant / 16 + 1;

        EFFECT_DRAW_MOTE(draw[1], canvas, sheet, HI(mote->x), HI(mote->y),
                         size);
        EffectStep_AdvanceWithGravity2D(mote, 62, 0x2000);
        mote->variant--;
      }
    }
    Camera_ApplyShake(4, 4);
    EFFECT_END_FRAME(work);
    angle -= 0x800; // 11.25 degrees
    frame++;
  } while (frame != JUPITER_FRAMES);
  EFFECT_FINISH_CANVAS();
}

// Decompresses a resource whose first 128 bytes may be a palette.
//
// id: the resource.
// dst: receives the decompressed data.
// skipPalette: the resource starts with a palette to skip.
// copyPalette: copy that palette to the start of background palette RAM.
void Resource_LoadAndDecompress(s32 id, void *dst, s32 skipPalette,
                                s32 copyPalette) {
  u8 *data = Resource_GetTableEntry(id);

  if (copyPalette != 0)
    Iwram_CopyWords((void *)BG_PLTT, data, 128);
  if (skipPalette != 0)
    data += 128;
  Resource_DecodeType01(data, dst);
}

#define VENUS_FRAMES 96  // frames in the effect
#define VENUS_COLUMNS 10 // columns of flame that rise

// Abilities: Venus (kind 250).
//
// Venus djinni columns: the small Venus djinni sways while ten columns of
// flame in its colours rise from the ground one after another, each
// bursting into motes and hitting every target.
//
// effect: the targets.
void BattleFx_RunVenus(struct BattleEffectArgument *effect) {
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  BlitterPair draw;
  u8 *sheet;
  struct EffectStep *column;
  struct EffectStep *mote;
  s32 i;
  s32 j;
  s32 angle;

  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  sheet = EFFECT_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER,
                      COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP_Y,
                      COMPOSITE_MAX);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                      COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, COMPOSITE_ADDITIVE);
  draw[0] = EFFECT_SLOT(slots, HEAP_SLOT_BLITTER);
  draw[1] = EFFECT_SLOT(slots, HEAP_SLOT_BLITTER_ALTERNATE);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_VenusDjinnSmallSheet, work->sheet,
                             1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_BlueFlameSheet,
                             work->sheet + 0x2f8, 1, 0);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);
  // The flame columns start at the bottom, at random x.
  for (i = 0; i != 32; i++) {
    column = &work->particles[i];
    column->x = Random16() & 63;
    column->y = 104;
  }
  EFFECT_FREE_PARTICLES(i, PARTICLES, 512);
  AudioCommand_PlayFar(141);
  frame = 0;
  angle = 0x8000;
  do {
    // The djinni sways from side to side as it rises and falls.
    if (frame <= 79) {
      s32 x = (Trig_Sin(angle) * 24) >> 16;
      s32 y = (Trig_Cos(angle) * (64 - frame * 2)) >> 16;

      draw[1](canvas, work->sheet, x + 22, y + 29, 20, 38);
    }
    if (frame == 56)
      BattleEventRuntime_BeginPhaseFar(133);
    // Ten flame columns rise 12 pixels a frame, one every 4 frames from
    // frame 16; each throws 16 motes and hits every target as it appears.
    for (i = 0; i != VENUS_COLUMNS; i++) {
      column = &work->particles[i];
      if (frame >= i * 4 + 16) {
        draw[0](canvas, work->sheet + 0x9e0, column->x - 17, column->y - 32, 34,
                65);
        if (frame == i * 4 + 16) {
          for (j = 0; j != 16; j++) {
            s32 direction;
            s32 speed;

            mote = &PARTICLES[i * 32 + j];
            direction = (Random16() & 0x7fff) + 0x4000;
            speed = (Random16() & 0x1ff) + 256;
            mote->x = column->x << 16;
            mote->y = (column->y + 16) << 16;
            mote->velocityX = (Trig_Sin(direction) * speed) >> 7;
            mote->velocityY = (Trig_Cos(direction) * speed) >> 6;
            mote->variant = (Random16() & 15) + 32;
          }
          if (i & 1)
            AudioCommand_PlayFar(133);
          work->shakeFrames = 4;
          EFFECT_FOR_EACH_TARGET(j, work->effect) {
            ObjectGroup_UpdateMembers(work->effect->actors[j],
                                      SPRITE_COLOR_WHITE, POSE_DAMAGE, j, 6);
            BattleMotion_ApplyVariantMotionFar(work->effect->actors[j], 6);
          }
        }
        column->y -= 12;
      }
    }
    // Motes shrink as they age and slow as they fall.
    for (i = 0; i != 512; i++) {
      mote = &PARTICLES[i];
      if (mote->variant != -1) {
        s32 size = mote->variant / 16 + 2;

        EFFECT_DRAW_MOTE(draw[1], canvas, sheet, HI(mote->x), HI(mote->y),
                         size);
        EffectStep_AdvanceWithGravity2D(mote, 62, 0x2000);
        mote->variant--;
      }
    }
    Camera_ApplyShake(4, 4);
    EFFECT_END_FRAME(work);
    angle -= 0x800;
    frame++;
  } while (frame != VENUS_FRAMES);
  EFFECT_FINISH_CANVAS();
}

#define MARS_FRAMES 112          // frames in the effect
#define MARS_STRIKE_FRAME 32     // the Djinni strikes the ground
#define MARS_FLAMES 9            // flames along the arc
#define MARS_EMBERS_PER_FLAME 16 // embers in each flame

// Abilities: Mars (kind 252).
//
// Mars Djinni flames: a glowing orb flares, the small Mars Djinni dives to
// the ground, and nine flames erupt in a row while embers rise. The scene
// runs for 112 frames:
//
//   0 to 23    the orb flares
//   20 to 31   the Djinni appears, then dives from frame 24
//   32         it strikes: the screen shakes and the flames start erupting
//   38         every target is hit
//   40 on      the embers rise, 16 more every 2 frames
//
// effect: the targets.
void BattleFx_RunMars(struct BattleEffectArgument *effect) {
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  u8 *sheet;
  BlitterPair blitters;
  DrawRectangle *draw;
  struct EffectStep *step;
  struct EffectStep *mote;
  s32 index;
  s32 frame;
  s32 i;
  s32 diveY;

  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  sheet = EFFECT_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);
  draw = blitters;
  BattleFx_FetchRectangleBlitters(0, draw);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_MarsDjinnSmallSheet, work->sheet,
                             1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_GlowOrbSheet, work->sheet + 0x320,
                             1, 1);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);

  // Nine flames along a flattened arc, 2 frames apart, each with 16
  // embers that rise wobbling from the ground beneath it.
  index = 0;
  step = work->particles;
  do {
    s32 angle = index << 11;

    step->x = (Trig_Sin(angle) * 24) >> 16;
    step->y = ((Trig_Cos(angle) * 4) >> 16) + 52;
    if (index & 1)
      step->x = 32 - step->x;
    else
      step->x += 32;
    step->variant = -(index * 2);
    for (i = 0; i != MARS_EMBERS_PER_FLAME; i++) {
      mote = &PARTICLES[index * MARS_EMBERS_PER_FLAME + i];
      mote->x = (((Random16() & 15) + step->x) - 8) << 16;
      mote->y = ((Random16() & 7) + 96) << 16;
      mote->velocityX = ((Random16() & 127) - 64) << 11;
      mote->velocityY = ((Random16() & 127) - 64) << 10;
      mote->z = Random16() & 0xffff;
      mote->velocityZ = Random16() & 0xffff;
    }
    step++;
    index++;
  } while (index != MARS_FLAMES);

  AudioCommand_PlayFar(136);
  frame = 0;
  diveY = -172;
  do {
    if (frame == 56)
      BattleEventRuntime_BeginPhaseFar(133);
    // A glowing orb flares, then the djinni dives down to the left.
    if (frame <= 23)
      blitters[0](canvas, work->sheet + 0x320 + (frame / 4) * 0x640, 40, 20, 40,
                  40);
    if (frame == 20)
      Iwram_CopyWords(
          (void *)BG_PLTT,
          Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSmallSheet), 128);
    if (frame >= 20 && frame <= 31) {
      if (frame > 23)
        draw[1](canvas, work->sheet, 146 - frame * 4, diveY, 20, 40);
      else
        blitters[0](canvas, work->sheet, 50, 20, 20, 40);
    }
    // At frame 32 it strikes: the screen shakes and the flames erupt.
    if (frame == MARS_STRIKE_FRAME) {
      AudioCommand_PlayFar(145);
      work->shakeFrames = 8;
      Resource_LoadAndDecompress((s32)&ResourceId_EmberStreakSheet, work->sheet,
                                 1, 1);
    }
    if (frame > 31) {
      index = 0;
      step = work->particles;
      do {
        if (step->variant >= 0 && step->variant <= 47) {
          s32 cell = step->variant / 8;
          u32 width;

// Sequence dimensions before the call; assigning and reading them in
// separate arguments was UB. Plain English trial: unchanged 0x3c4 bytes,
// with different instructions. Preserve the original native calls.
#ifdef UBFIX
          width = gBattleFx_PuffArcCellWidths[cell];
          blitters[0](canvas,
                      work->sheet + gBattleFx_PuffArcCellSourceOffsets[cell],
                      step->x - (width >> 1),
                      step->y + gBattleFx_PuffArcCellBiasY[cell], width,
                      gBattleFx_PuffArcCellHeights[cell]);
#else
          blitters[0](
              canvas, work->sheet + gBattleFx_PuffArcCellSourceOffsets[cell],
              step->x - ((width = gBattleFx_PuffArcCellWidths[cell]) >> 1),
              step->y + gBattleFx_PuffArcCellBiasY[cell], width,
              gBattleFx_PuffArcCellHeights[cell]);
#endif
        }
        index++;
        step->variant++;
        step++;
      } while (index != MARS_FLAMES);
    }
    // The embers rise, swaying, from frame 40.
    step = PARTICLES;
    index = 0;
    do {
      if (frame >= (index / MARS_EMBERS_PER_FLAME) * 2 + 40) {
        s32 size;
        s32 x;
        s32 angle;

        size = (index & 1) + 3;
        x = HI(step->x) + ((Trig_Sin(step->z) * 4) >> 16);
        draw[1](canvas, sheet + gBattleFx_MoteCellOffsets[size - 1],
                x - ((u32)size >> 1), HI(step->y) - size, size, size * 2);
        EffectStep_AdvanceWithGravity2D(step, 64, -0x2000);
        angle = step->z;
        step->z = angle + 0x800;
        if (step->z > 0xffff)
          step->z = angle + 0x800 - 0xffff;
      }
      index++;
      step++;
    } while (index != 144);

    if (frame == 38) {
      EFFECT_FOR_EACH_TARGET(index, work->effect) {
        ObjectGroup_UpdateMembers(work->effect->actors[index],
                                  SPRITE_COLOR_WHITE, POSE_DAMAGE, index, 16);
        BattleMotion_ApplyVariantMotionFar(work->effect->actors[index], 6);
      }
    }
    Camera_ApplyShake(8, 8);
    EFFECT_END_FRAME(work);
    diveY += 8;
    frame++;
  } while (frame != MARS_FRAMES);
  EFFECT_FINISH_CANVAS();
}
#define REVEAL_CHIP_COUNT 64 // ice chips

// Where the water burst's pictures sit in the sheet, after the waves.
#define REVEAL_CHIP_CELLS 0x400
#define REVEAL_DJINNI_CELLS 0x65c0

#define HAIL_FRAMES 64 // frames in the effect

// Abilities: Hail (kind 207).
//
// Water burst: after a Mercury djinni enters, ice chips burst out of the
// first target and waves rise along the bottom of the screen; the canvas
// fades out at the end.
//
// effect: the user and the targets; prelude 1 keeps the djinni circling.
void BattleFx_RunHail(struct BattleEffectArgument *effect) {
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  struct EffectPosition origin;
  s32 screenY;
  s32 screenX;
  BlitterPair draw;
  struct EffectStep *chip;
  s32 i;
  s32 frame;
  s32 rise;

  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);
  BattleFx_PrepareCanvasEffect(effect, 1, work->effect->side, 2, &screenX,
                               &screenY);
  BattleFx_FetchRectangleBlitters(work->effect->side, draw);
  Resource_LoadAndDecompress((s32)&ResourceId_WaterSpraySheet, work->sheet, 1,
                             1);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);
  EffectPosition_ApplyAlternateStepAndYOffset(work->effect->actors[0], &origin);

  // The chips burst up from the first target.
  for (i = 0; i != REVEAL_CHIP_COUNT; i++) {
    s32 angle;
    s32 speed;

    angle = (Random16() & 0x7fff) + 0x4000;
    speed = (Random16() & 0x1ff) + 0x80;
    gMapCellBuffer[i].x = ((origin.x / 2 + (Random16() & 0xf)) - 8) << 16;
    gMapCellBuffer[i].y = (origin.y + 8) << 16;
    gMapCellBuffer[i].velocityX = (Trig_Sin(angle) * speed) >> 9;
    gMapCellBuffer[i].velocityY = (Trig_Cos(angle) * speed) >> 6;
    gMapCellBuffer[i].z = Random16() & 0x7f;
    gMapCellBuffer[i].velocityZ = Random16() & 0x7f;
    gMapCellBuffer[i].variant = (Random16() & 0xf) + 32;
  }

  for (frame = 0; frame != HAIL_FRAMES; frame++) {
    // Fade out over the last 16 frames.
    if (frame > 47)
      REG_BLDALPHA = (64 - frame) | 0x1000;
    if (frame == 1) {
      Resource_LoadAndDecompress((s32)&ResourceId_IceChipSheet,
                                 work->sheet + REVEAL_CHIP_CELLS, 1, 1);
      Resource_LoadAndDecompress((s32)&ResourceId_MercuryDjinnSmallSheet,
                                 work->sheet + REVEAL_DJINNI_CELLS, 1, 0);
    }

    // Prelude: the small djinni circles.
    if (work->effect->prelude == 1) {
      s32 orbit;
      s32 x;
      s32 y;

      orbit = frame << 11;
      x = (((-Trig_Sin(orbit) * 4)) >> 16) + screenX / 2 - 10;
      y = (((Trig_Cos(orbit) * 2)) >> 16) + screenY - 22;
      if (frame > 69)
        y = (y - frame * 2) + 138;
      draw[1](canvas, work->sheet + REVEAL_DJINNI_CELLS, x, y, 20, 40);
      if (frame <= 3)
        draw[1](canvas, work->sheet + REVEAL_DJINNI_CELLS, x, y, 20, 40);
    }

    for (i = 0, chip = gMapCellBuffer; i != REVEAL_CHIP_COUNT; i++, chip++) {
      if (frame >= i / 4 + 4) {
        s32 cell;
        s32 width;
        s32 height;

        cell = (chip->z / 128) & 3;
// Sequence dimensions before the call; assigning and reading them in
// separate arguments was UB. Plain English trial: unchanged 0x3bc bytes,
// with different instructions. Preserve the original native calls.
#ifdef UBFIX
        width = gHail_CellWidths[cell];
        height = gHail_CellHeights[cell];
        draw[i & 1](
            canvas,
            work->sheet + REVEAL_CHIP_CELLS + gHail_CellSourceOffsets[cell],
            HI(chip->x) - width / 2, HI(chip->y) - height / 2, width, height);
#else
        draw[i & 1](canvas,
                    work->sheet + REVEAL_CHIP_CELLS +
                        gHail_CellSourceOffsets[cell],
                    HI(chip->x) - (width = gHail_CellWidths[cell]) / 2,
                    HI(chip->y) - (height = gHail_CellHeights[cell]) / 2, width,
                    height);
#endif
        EffectStep_AdvanceWithGravity3D(chip, 63, 0x1000);
      }
    }

    if (frame == 8) {
      work->shakeFrames = frame;
      BattleEventRuntime_BeginPhaseFar(134);
      ObjectGroup_UpdateMembers(work->effect->actors[0], SPRITE_COLOR_WHITE,
                                POSE_DAMAGE, 0, 16);
      BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 3);
    }

    // Waves rise along the bottom, 4 pixels a frame up to 32, drifting
    // away from the user.
    rise = frame * 4;
    if (rise > 32)
      rise = 32;
    if (work->effect->side == 0) {
      for (i = 0; i != 5; i++)
        draw[0](canvas, work->sheet, (i << 5) - (frame / 4 & 31), 120 - rise,
                32, 32);
    } else {
      for (i = 0; i != 5; i++)
        draw[0](canvas, work->sheet, ((i << 5) + (frame / 4 & 31)) - 32,
                120 - rise, 32, 32);
    }

    Camera_ApplyShake(4, 8);
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS();
}

// The small Venus djinni in the rock toss's sheet.
#define ROCK_TOSS_DJINNI_CELLS 0x65c0

#define GROUND_FRAMES 96        // frames in the effect
#define GROUND_LAUNCHED 8       // rocks launched
#define GROUND_CHIPS 32         // chip slots
#define GROUND_ROCKS 6          // rocks thrown
#define GROUND_CHIPS_PER_ROCK 4 // chips each rock sheds

// Abilities: Ground (kind 203).
//
// Venus djinni rock toss: after a Venus djinni enters, six rocks rise from
// the user shedding chips, swirl, and home in on the first target, each
// hitting it as it lands.
//
// effect: the user and the targets; prelude 1 keeps the djinni circling.
void BattleFx_RunGround(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 i;
  s32 frame;
  BlitterPair draw;
  struct BattleCamera *camera;
  struct MotionObject *target;
  struct MotionObject *user;
  struct EffectPosition position;
  s32 screenX;
  s32 screenY;
  s32 j;

  work = *(gWorkSlot + HEAP_SLOT_BATTLE_EFFECT);
  canvas = *(gWorkSlot + HEAP_SLOT_BATTLE_CANVAS);
  camera = *(gWorkSlot + HEAP_SLOT_CAMERA);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);
  BattleFx_PrepareCanvasEffect(effect, 0, work->effect->side, 2, &screenX,
                               &screenY);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  if (work->effect->side == 1)
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER,
                        COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X,
                        COMPOSITE_MAX);
  else
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                        COMPOSITE_MAX);
  draw[0] =
      (DrawRectangle)((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BLITTER];
  Resource_LoadAndDecompress((s32)&ResourceId_PortalSheet, work->sheet, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_VenusDjinnSmallSheet,
                             work->sheet + ROCK_TOSS_DJINNI_CELLS, 1, 1);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);
  user = GetBattleObjectSlotFar(work->effect->actor)->object;
  target = GetBattleObjectSlotFar(work->effect->actors[0])->object;
  // The rocks leave the user 66 pixels up, fanning out.
  for (i = 0; i != GROUND_LAUNCHED; i++) {
    struct EffectStep *rock = &work->particles[i];

    EFFECT_SET_POSITION(rock, user->x, 0x420000, user->z);
    EFFECT_SET_VELOCITY(rock, (i * 0x500000) >> 5,
                        (s32)(((Random16() & 127) - 64) << 16) >> 6,
                        (s32)(((Random16() & 255) - 127) << 16) >> 5);
    if (rock->x > 0)
      rock->velocityX = -rock->velocityX;
    rock->variant = 1;
  }
  for (frame = 0; frame != GROUND_FRAMES; frame++) {
    if (frame > 16)
      BattleFx_StepPaletteToResource((s32)&ResourceId_PortalSheet);
    if (work->effect->prelude == 1) {
      s32 angle = frame << 11;
      s32 x = (((-Trig_Sin(angle) * 4)) >> 16) + screenX / 2 - 10;
      s32 y = (((Trig_Cos(angle) * 2)) >> 16) + screenY - 22;

      if (frame > 16)
        y = y - frame * 2 + 32;
      if (work->effect->side == 1)
        EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                            COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                                COMPOSITE_FLIP_X,
                            COMPOSITE_ADDITIVE);
      else
        EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                            COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                            COMPOSITE_ADDITIVE);
      if (frame <= 3)
        ((DrawRectangle)((union HeapState *)gWorkSlot)
             ->slots[HEAP_SLOT_BLITTER_ALTERNATE])(
            canvas, work->sheet + ROCK_TOSS_DJINNI_CELLS, x, y, 20, 40);
      Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
      draw[0](canvas, work->sheet + ROCK_TOSS_DJINNI_CELLS, x, y, 20, 40);
    }
    // Every 2 frames each chip takes a random picture (velocityX) and
    // flip (velocityY).
    if ((frame & 1) == 0) {
      for (i = 0; i != GROUND_CHIPS; i++) {
        struct EffectStep *chip = &work->particles[32 + i];

        chip->velocityX = Random16() % 6 + 3;
        chip->velocityY = gGround_ChipFlips[Random16() & 3];
      }
    }
    EFFECT_VIEW_FROM_CAMERA(camera);
    for (i = 0; i != GROUND_ROCKS; i++) {
      struct EffectStep *rock = &work->particles[i];

      if (rock->variant == 1) {
        if (frame > i * 2) {
          s32 x;
          s32 y;

          EffectPosition_ApplyBaseAndYOffset(&rock->x, &position);
          position.x >>= 1;
          x = position.x - 12;
          y = position.y - 24;
          draw[0](canvas, work->sheet, x, y, 24, 48);
          if ((frame & 3) <= 1)
            draw[0](canvas, work->sheet + gGround_ChipCells[1],
                    x + gGround_ChipX[1], y + gGround_ChipY[1],
                    gGround_ChipWidths[1], gGround_ChipHeights[1]);
          else
            draw[0](canvas, work->sheet + gGround_ChipCells[2],
                    x + gGround_ChipX[2], y + gGround_ChipY[2],
                    gGround_ChipWidths[2], gGround_ChipHeights[2]);
          for (j = 0; j != GROUND_CHIPS_PER_ROCK; j++) {
            struct EffectStep *chip = &work->particles[32 + i * 4 + j];
            s32 chipX;
            s32 chipY;

            EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE, chip->velocityY,
                                COMPOSITE_MAX);
            draw[1] = (DrawRectangle)((union HeapState *)gWorkSlot)
                          ->slots[HEAP_SLOT_BLITTER_ALTERNATE];
            if (chip->velocityY & 4)
              chipX = x - gGround_ChipWidths[chip->velocityX] -
                      gGround_ChipX[chip->velocityX] + 24;
            else
              chipX = x + gGround_ChipX[chip->velocityX];
            if (chip->velocityY & 8)
              chipY = y - gGround_ChipHeights[chip->velocityX] -
                      gGround_ChipY[chip->velocityX] + 48;
            else
              chipY = y + gGround_ChipY[chip->velocityX];
            draw[1](canvas, work->sheet + gGround_ChipCells[chip->velocityX],
                    chipX, chipY, gGround_ChipWidths[chip->velocityX],
                    gGround_ChipHeights[chip->velocityX]);
            Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
          }
          rock->x += rock->velocityX;
          rock->y += rock->velocityY;
          rock->z += rock->velocityZ;
        }
        // After 16 frames the rock homes in on the target, damped until
        // frame 85, and hits it below 20 pixels.
        if (frame > i * 2 + 16) {
          rock->velocityX += (target->x - rock->x) >> 8;
          rock->velocityY += (0x140000 - rock->y) >> 8;
          rock->velocityZ += (target->z - rock->z) >> 8;
          if (frame < i * 2 + 85) {
            EFFECT_SET_VELOCITY(rock, rock->velocityX * 60 / 64,
                                rock->velocityY * 60 / 64,
                                rock->velocityZ * 60 / 64);
          }
          if (rock->y < 0x140000) {
            work->shakeFrames = 8;
            rock->variant = 0;
            AudioCommand_PlayFar(134);
            ObjectGroup_UpdateMembers(work->effect->actors[0],
                                      SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 4);
            BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 4);
          }
        }
      }
    }
    Camera_ApplyShake(16, 16);
    EFFECT_END_FRAME(work);
  }
  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

// Shakes the battle view while gBattleFxWork's shakeFrames lasts, moving
// BG1 and the projection centre by a random offset; once the shake runs
// out, BG1 returns to its saved position and the centre to (120, 120).
//
// randomMask: the horizontal range, a power of two.
// shakeRange: the vertical range, a power of two; both offsets are
//             centred on half of it.
void Camera_ApplyShake(s32 randomMask, u32 shakeRange) {
  s32 displayY;
  s32 offsetX;
  s32 *remainingFrames;
  s32 restoredPosition;
  struct BattleEffectWork *work;
  s32 offsetY;
  s32 halfRange;
  s32 randomX;
  struct BgScroll *displayPosition;
  struct Projection *displayCenter;
  struct BgScroll *restoredScroll;
  struct Projection *restoredCenter;

  work = WORK_SLOT(HEAP_SLOT_BATTLE_EFFECT);
  remainingFrames = &work->shakeFrames;
  if (*remainingFrames > 0) {
    randomX = (randomMask - 1) & Random16();
    halfRange = (s32)(shakeRange + (shakeRange >> 0x1F)) >> 1;
    offsetY = ((shakeRange - 1) & Random16()) - halfRange;
    displayPosition = gBgScroll;
    offsetX = randomX - halfRange;
    displayY = offsetY + 0x20;
    displayPosition[1].x = offsetX;
    displayPosition[1].y = displayY;
    displayCenter = &gProjection;
    displayCenter->centerX = 0x78 - offsetX;
    displayCenter->centerY = 0x78 - offsetY;
    *remainingFrames -= 1;
    return;
  }
  restoredPosition = work->savedBg1X;
  restoredScroll = gBgScroll;
  restoredScroll[1].x = restoredPosition;
  restoredPosition = work->savedBg1Y;
  restoredScroll[1].y = restoredPosition;
  restoredCenter = &gProjection;
  restoredCenter->centerX = 0x78;
  restoredCenter->centerY = 0x78;
}

// Storm and rock scenes, staged bursts and rising walls.
// The final helpers advance particles and project battle units to the screen.

extern u8 gThor_TargetPalettes[];
extern u8 gThor_SmokeFlips[];

// 910 sparks from Ram_MapCellBuffer + 0xc58; the smoke pictures come
// before them.
#define STORM_PARTICLES ((struct EffectStep *)(Ram_MapCellBuffer + 0xc58))

// The violet mote in the second part's sheet.
#define STORM_MOTE_CELLS 0x6000

// Storm summons: a circle of light forms in the dusk sky and sends down a
// beam; then a pillar of lightning and a storm of violet bolts strike the
// targets, pushing them back, and the effect ends in an impact burst.
//
// effect: the targets.

#define STORM_PARTICLE_COUNT 910     // particles in STORM_PARTICLES
#define STORM_DIMMED_DOTS 10000      // the darker copies of the dot pictures
#define STORM_DIMMED_DOT_BYTES 0x302 // bytes in each darker copy
#define STORM_DUSK_FRAMES 170        // frames, at most, of the dusk part
#define STORM_LIGHTNING_FRAMES 192   // frames of the lightning part
#define STORM_BOLTS 30               // violet bolts after the pillar

// Abilities: Thor (scene kind 12).
//
// The scene plays in two parts on the effect canvas. In the dusk part
// (STORM_DUSK_FRAMES, which A or B cuts short) a circle of 128 dots and three
// lines across it form over scrolling dusk clouds; the circle turns, sinks
// into the sky and a beam of light falls from it. In the lightning part
// (STORM_LIGHTNING_FRAMES) a pillar of lightning strikes the formation and
// throws sparks, STORM_BOLTS violet bolts follow while a violet mote sways
// above, and the formation is pushed back in smoke until an impact burst ends
// the effect. The sparks live in STORM_PARTICLES; a spark whose variant is 0
// is free, and a positive variant counts the frames it has left.
void BattleFx_RunThor(struct BattleEffectArgument *effect) {
  static s16 points[6][2] = {
      {8, 135}, {184, 135}, {128, 135}, {96, 32}, {96, 32}, {64, 135},
  };
  u8 ramp[32];
  s32 point[3];
  struct EffectPosition screen[11];
  s32 position[3];
  s32 scale[3];
  void **slots;
  struct BattleBackgroundView *view;
  struct BattleEffectWork *work;
  void *canvas;
  u8 *sprites;
  s32 frame;
  /* FAKEMATCH: frame local: only blit[0] is used; the unused second blitter
     word fills four bytes of the frame. Measured (2026-10-14): declared as one
     DrawRectangle, the frame is 332 bytes for 336 (differ 202, frame offsets
     only). */
  BlitterPair blit;
  s32 size;
  s32 count;
  s32 i;
  s32 j;
  s32 dotCount;
  struct EffectStep *spark;
  DrawRectangle *blitters; // the work slots read as blitters
  s32 rings = STORM_DIMMED_DOTS;

  slots = &gWorkSlot[HEAP_SLOT_BATTLE_BACKGROUND];
  view = slots[0];
  work = slots[-5];
  canvas = slots[-4];
  sprites = slots[-3];
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0x2000);
  REG_BG2PA = Q_8_8(1);
  Resource_LoadAndDecompress((s32)&ResourceId_LightFanSheet, work->sheet, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_LightFanSheet, work->sheet, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesC,
                             work->sheet + 0x1800, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sprites, 0, 0);

  // Eight copies of the dot pictures, each with its colour levels capped 7
  // lower than the one before.
  {
    s32 offset;
    for (i = 0, offset = 0; i != 8; i++) {
      u8 *src;
      for (j = 0, src = sprites; j != STORM_DIMMED_DOT_BYTES; j++) {
        s32 v = *src++;

        if (v > 64 - i * 7) {
          v = 64 - i * 7;
        }
        if (v < 0) {
          v = 0;
        }
        (work->sheet + STORM_DIMMED_DOTS)[offset + j] = v;
      }
      offset += j;
    }
  }

  BattleFx_ConfigureEffectDisplay();
  PLTT_BACKDROP = 0;
  *(u16 *)&BG_PLTT[1] = 0; // background palette 0, colour 1
  REG_BG2CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7) | BGCNT_WRAP;
  work->scrollTimer = 0;
  work->scrollInterval = 2;
  work->scrollStepX = 1;
  work->scrollStepY = 0;
  view->busy = 1;
  Scheduler_AddOrUpdateCallback(BattleFx_AdvanceScrollOnInterval, 0x480);
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);
  work->transferMode = CANVAS_COPY;
  BattleFx_WipeCanvas(0, 0);
  BattleBackground_LoadFar(1, (s32)&ResourceId_DuskCloudsBackdrop, 0);
  gProjection.centerY = 240;
  BattleFx_WipeCanvas(0, 1);
  REG_BG2PA = Q_8_8(0.5);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                      COMPOSITE_ADDITIVE);
  blitters = (DrawRectangle *)gWorkSlot;
  blit[0] = blitters[HEAP_SLOT_BLITTER];
  EFFECT_SHOW_WIDE_CANVAS(15);
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);

  // One row of the beam: brightest in the middle, darker to each side.
  for (i = 0; i != 16; i++) {
    ramp[i] = i * 8 + 3;
    ramp[31 - i] = i * 8 + 3;
    if (ramp[i] > 63) {
      ramp[i] = 63;
    }
    if (ramp[31 - i] > 63) {
      ramp[31 - i] = 63;
    }
  }
  REG_BG2CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7);

  // First part, 170 frames (A or B skips it): a circle of dots and
  // three lines across it form and turn over dusk clouds, the circle shrinks
  // into the sky, and a beam of light falls from it.
  for (frame = 0; frame != STORM_DUSK_FRAMES; frame++) {
    size = 2;
    count = frame * 2;
    if (frame == 16) {
      AudioCommand_PlayFar(140);
    }
    if (frame == 132) {
      AudioCommand_PlayFar(131);
    }
    if (frame == 151) {
      AudioCommand_PlayFar(145);
    }
    if (gKeysRepeat & (KEY_A | KEY_B)) {
      break;
    }
    Graphics_UpdatePhasePalette(frame * 2, 0xaaab, 0x5555, 0);
    if (frame == 150) {
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0x1a1a1a1a);
    } else {
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);
    }
    Projection_Set(0x1fe0000, Iwram_RatioMulQ14(0x1fe0000, 0xc000), 0x7fff0000);
    Render_ResetTransformState();
    if (frame > 128) {
      s32 sinkFrame = frame - 128;
      if (sinkFrame > 22) {
        sinkFrame = 20;
      }
      EFFECT_SET_VECTOR(position, 0, -sinkFrame << 17, 0x2000000);
      size = (sinkFrame >> 2) + 2;
      if (size > 8) {
        size = 8;
      }
      SceneTransform_ApplyPosition(position);
      SceneTransform_ApplyPitch(-0x1000);
      SceneTransform_ApplyRoll(0x8000);
      SceneTransform_ApplyPitch(0x8000);
      SceneTransform_ApplyYaw(sinkFrame << 12);
      if (frame > 150) {
        EFFECT_SET_VECTOR(scale, frame * 0x1800 - 0xd9400, scale[0], scale[0]);
        size = 5;
      } else {
        EFFECT_SET_VECTOR(scale, 0x10000 - sinkFrame * 3 * 0x400, scale[0],
                          scale[0]);
      }
      SceneTransform_ApplyScale(scale);
    } else {
      EFFECT_SET_VECTOR(position, 0, 0, 0x1000000);
      SceneTransform_ApplyPosition(position);
      SceneTransform_ApplyPitch(-0x1000);
      SceneTransform_ApplyRoll(frame << 8);
      SceneTransform_ApplyPitch(frame << 8);
    }

    // The three lines, drawn dot by dot between pairs of points.
    if (frame <= 149) {
      for (i = 0; i != 6; i++) {
        EFFECT_SET_VECTOR(point, (points[i][0] - 96) << 16, 0,
                          (points[i][1] - 96) << 16);
        EffectPosition_ApplyBaseAndYOffset(point, &screen[i]);
        screen[i].x = (screen[i].x >> 17) + 64;
        screen[i].y = (screen[i].y >> 16) + 60;
      }
      for (i = 0; i != 3; i++) {
        dotCount = count * 2 - i * 48 - 256;
        if (dotCount > 48) {
          dotCount = 48;
        }
        if (dotCount >= 0) {
          for (j = 0; j != dotCount; j++) {
            s32 x = screen[i * 2].x +
                    Iwram_MulQ16(j * (screen[i * 2 + 1].x - screen[i * 2].x),
                                 0x555);
            s32 y = screen[i * 2].y +
                    Iwram_MulQ16(j * (screen[i * 2 + 1].y - screen[i * 2].y),
                                 0x555);
            EFFECT_DRAW_MOTE(blit[0], canvas, sprites, x, y, size);
          }
        }
      }
    }

    // The circle of 128 dots, darkening once the beam has fallen.
    if (frame <= 179) {
      DrawRectangle drawAlt;
      s32 level = 0;

      if (frame > 155) {
        level = frame - 156;
      }
      if (level > 7) {
        level = 7;
      }
      if (frame <= 139) {
        EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                            COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                            COMPOSITE_ADDITIVE);
        drawAlt = gBlitters[HEAP_SLOT_BLITTER_ALTERNATE];
      } else {
        EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                            COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, COMPOSITE_MAX);
        drawAlt = gBlitters[HEAP_SLOT_BLITTER_ALTERNATE];
      }
      dotCount = count;
      if (dotCount > 128) {
        dotCount = 128;
      }
      for (i = 0; i != dotCount; i++) {
        s32 dotSize = size + 1;
        point[0] = Trig_Sin(i << 9) * 96;
        point[2] = -(Trig_Cos(i << 9) * 96);
        EffectPosition_ApplyBaseAndYOffset(point, &screen[0]);
        screen[0].x = (screen[0].x >> 17) + 64;
        screen[0].y = (screen[0].y >> 16) + 60;
        drawAlt(canvas,
                work->sheet +
                    (level * STORM_DIMMED_DOT_BYTES +
                     gBattleFx_MoteCellOffsets[dotSize - 1]) +
                    rings,
                screen[0].x - dotSize / 2, screen[0].y - dotSize, dotSize,
                dotSize * 2);
      }
      Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
    }

    // The beam: rows of the ramp narrowing at its top, and the light fan
    // falling down it.
    if ((u32)(frame - 151) <= 16) {
      s32 inset = 0;
      s32 beamHeight = (frame - 151) * 20 + 44;
      if (frame > 151) {
        inset = frame - 152;
      }
      if (inset > 15) {
        inset = 15;
      }
      for (i = 1; i != 10; i++) {
        if (inset + i <= 15) {
          blit[0](canvas, &ramp[inset + i], inset + 48 + i, 16 - i,
                  32 - inset * 2 - i * 2, 1);
        }
      }
      for (i = 0; i != beamHeight; i++) {
        s32 fullWidth = 32;
        // loop.c matches this 32 with the smoke calls' 32 and hoists it out of
        // the frame loop, where it loses its register and later reloads rotate:
        // after an earlier move its threshold is 12, and savings 2 x life 19 x
        // 12 = 456 reaches the loop'dotSize 448 insns.
        /* FAKEMATCH: the barrier keeps fullWidth'dotSize 32 in this
         * loop'dotSize preheader, as native, and its two codeless preheader
         * insns lengthen i'dotSize life (1292), so dotCount (28 refs/106) ranks
         * before i (195 refs) and takes r9; sinkFrame 96 local before the ramp
         * loops also defeats the hoist (432 < 448) but lacks those insns, so i
         * takes r9 and sinkFrame slot appears at [sp, #60] (differ 699). */
        asm("" : "+r"(fullWidth));
        blit[0](canvas, ramp + inset, inset + 48, i + 16, fullWidth - inset * 2,
                1);
      }
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                          COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, COMPOSITE_MAX);
      gBlitters[HEAP_SLOT_BLITTER_ALTERNATE](canvas, work->sheet, 32,
                                             (frame - 151) * 20 - 12, 32, 96);
      Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                          COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                              COMPOSITE_FLIP_X,
                          COMPOSITE_MAX);
      gBlitters[HEAP_SLOT_BLITTER_ALTERNATE](canvas, work->sheet, 64,
                                             (frame - 151) * 20 - 12, 32, 96);
      Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
    }
    EFFECT_PRESENT_FRAME(work);
  }

  // Second part, 192 frames: a pillar of lightning strikes the
  // formation, throwing sparks, then violet bolts strike again and again while
  // the formation is pushed back in smoke, ending in an impact burst.
  view->busy = 0;
  Scheduler_RemoveCallback(BattleFx_AdvanceScrollOnInterval);
  BattleFx_SetupBlendedDisplay();
  BattleFx_SelectLivingTargets(work->effect);
  BattleFx_SpawnObjects(9, 0x173, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, Ram_MapCellBuffer,
                             1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_LightningPillarSheet, work->sheet,
                             1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_VioletMoteSheet,
                             work->sheet + STORM_MOTE_CELLS, 1, 0);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_CrystalSheet), 128);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesB, sprites, 0, 0);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  {
    s32 formationX;
    s32 formationY;
    s32 formationSpeedX;
    s32 formationSpeedY;
    s32 pillarX;
    s32 pillarY;

    formationX = 0x500000;
    formationY = 0x400000;
    formationSpeedX = 0;
    formationSpeedY = 0;
    for (i = 0; i != STORM_PARTICLE_COUNT; i++) {
      STORM_PARTICLES[i].variant = 0;
    }
    for (i = 0; i != 16; i++) {
      work->particles[i].x = (Random16() & 31) - 16;
      work->particles[i].y = Random16() & 63;
      work->particles[i].variant = -(Random16() & 15);
    }
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, 50);
    AudioCommand_PlayFar(145);
    pillarX = 60;
    pillarY = 112;

    for (frame = 0; frame != STORM_LIGHTNING_FRAMES; frame++) {
      s32 stormFrame;
      if (frame == 20) {
        Resource_LoadAndDecompress((s32)&ResourceId_VioletLightningSheetB, work,
                                   1, 0);
      }
      stormFrame = frame - 80;
      if ((u32)stormFrame <= 59 && !(frame & 7)) {
        AudioCommand_PlayFar(134);
      }
      if (frame == 140) {
        BattleEventRuntime_BeginPhaseFar(134);
      }
      for (j = 0; j != 1; j++) {
        s32 start = j * 128 + 8;
        if (frame >= start && frame < j * 128 + 17) {
          if (frame >= j * 128 + 9 && frame < j * 128 + 12) {
            blit[0](canvas, work->sheet, 36, 0, 48, pillarY);
          }
          if (frame >= j * 128 + 12 && frame < start + 8) {
            blit[0](canvas, work->sheet + 0x1500, 36, 0, 48, 112);
          }
          if (frame == start + 2) {
            s32 spawned = 0;
            for (i = 0, spark = STORM_PARTICLES; i != STORM_PARTICLE_COUNT;
                 i++, spark++) {
              if (spark->variant == 0) {
                s32 speed = 0x3ff;
                s32 angle;
                speed &= Random16();
                angle = (Random16() & 0x7fff) - 0x4000;
                spark->x = pillarX << 16;
                spark->y = pillarY << 16;
                speed += 32;
                spark->velocityX = (Trig_Sin(angle) * speed) >> 7;
                spark->velocityY = -((Trig_Cos(angle) * speed) << 1) >> 7;
                spark->variant = (Random16() & 7) + 32;
                if (++spawned == 512) {
                  break;
                }
              }
            }
            work->shakeFrames = 8;
          }
        }
      }
      if (frame == 10) {
        EFFECT_FOR_EACH_TARGET(i, work->effect) {
          ObjectGroup_UpdateMembers(work->effect->actors[i], SPRITE_COLOR_WHITE,
                                    POSE_DAMAGE, i, 8);
          BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 4);
        }
      }
      // Frames 80 to 143: thirty violet bolts, 2 frames apart, and the
      // violet mote swaying in four mirrored copies.
      if ((u32)stormFrame <= 63) {
        s32 moteX;
        s32 moteY;
        s32 mirrorX;
        for (i = 0; i != 6; i++) {
          if (frame == i * 8 + 80) {
            Iwram_FillWords(canvas, 0x4000, 0x10101010);
          }
        }
        for (i = 0; i != STORM_BOLTS; i++) {
          if (frame >= i * 2 + 80) {
            if (frame < i * 2 + 82) {
              s32 boltX = (Random16() & 3) - 3;
              s32 boltY = (Random16() & 3) + 32;
              EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                                  COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                                  COMPOSITE_MAX);
              gBlitters[HEAP_SLOT_BLITTER_ALTERNATE](
                  canvas, work->sheet + i % 3 * 0x1440, boltX, boltY, 72, 72);
              Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
            }
            if (frame == i * 2 + 80) {
              s32 spawned = 0;
              for (j = 0, spark = STORM_PARTICLES; j != STORM_PARTICLE_COUNT;
                   j++, spark++) {
                if (spark->variant == 0) {
                  s32 speed = 0x3ff;
                  s32 angle;
                  speed &= Random16();
                  angle = Random16() & 0xffff;
                  spark->x = 0x100000;
                  spark->y = 0x500000;
                  speed += 32;
                  spark->velocityX = (Trig_Sin(angle) * speed) >> 7;
                  spark->velocityY = -((Trig_Cos(angle) * speed) << 1) >> 7;
                  spark->variant = (Random16() & 7) + 32;
                  if (++spawned == 16) {
                    break;
                  }
                }
              }
              work->shakeFrames = 8;
              EFFECT_FOR_EACH_TARGET(j, work->effect) {
                ObjectGroup_UpdateMembers(work->effect->actors[j],
                                          gThor_TargetPalettes[Random16() & 3],
                                          5, j, 4);
                BattleMotion_ApplyVariantMotionFar(work->effect->actors[j], 4);
              }
            }
          }
        }
        {
          s32 radius = 7;
          s32 angle;
          radius &= Random16();
          angle = Random16() & 0xffff;
          moteX = (Trig_Sin(angle) * (radius + 8)) >> 16;
          mirrorX = moteX + 72;
          moteY = 32 - ((Trig_Cos(angle) * (radius + 8)) >> 16);
        }
        EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                            COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, COMPOSITE_MAX);
        gBlitters[HEAP_SLOT_BLITTER_ALTERNATE](canvas,
                                               work->sheet + STORM_MOTE_CELLS,
                                               moteX + 60, moteY - 24, 12, 24);
        Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
        EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                            COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                                COMPOSITE_FLIP_X,
                            COMPOSITE_MAX);
        gBlitters[HEAP_SLOT_BLITTER_ALTERNATE](canvas,
                                               work->sheet + STORM_MOTE_CELLS,
                                               mirrorX, moteY - 24, 12, 24);
        Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
        EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                            COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                                COMPOSITE_FLIP_Y,
                            COMPOSITE_MAX);
        gBlitters[HEAP_SLOT_BLITTER_ALTERNATE](
            canvas, work->sheet + STORM_MOTE_CELLS, moteX + 60, moteY, 12, 24);
        Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
        EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                            COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                                COMPOSITE_FLIP_X | COMPOSITE_FLIP_Y,
                            COMPOSITE_MAX);
        gBlitters[HEAP_SLOT_BLITTER_ALTERNATE](
            canvas, work->sheet + STORM_MOTE_CELLS, mirrorX, moteY, 12, 24);
        Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
        j = 0;
        for (i = 0, spark = STORM_PARTICLES; i != STORM_PARTICLE_COUNT;
             i++, spark++) {
          if (spark->variant == 0) {
            s32 speed = 63;
            s32 angle;
            speed &= Random16();
            angle = Random16() & 0xffff;
            spark->x = mirrorX << 16;
            spark->y = moteY << 16;
            speed += 64;
            spark->velocityX = (Trig_Sin(angle) * speed) >> 7;
            spark->velocityY = -((Trig_Cos(angle) * speed) << 1) >> 7;
            spark->variant = (Random16() & 7) + 16;
            if (++j == 4) {
              break;
            }
          }
        }
      }

      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                          COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                              COMPOSITE_FLIP_X | COMPOSITE_FLIP_Y,
                          COMPOSITE_MAX);
      {
        DrawRectangle draw = gBlitters[HEAP_SLOT_BLITTER_ALTERNATE];
        struct EffectStep *spark;
        for (i = 0, spark = STORM_PARTICLES; i != STORM_PARTICLE_COUNT;
             i++, spark++) {
          if (spark->variant > 0) {
            spark->variant--;
            EffectStep_AdvanceWithGravity2D(spark, 60, 0);
            if (spark->y > 0x780000) {
              spark->velocityY = -spark->velocityY / 2;
            } else if ((u32)spark->x <= 0x7effff && spark->y >= 0) {
              s32 x = spark->x >> 16;
              s32 y = spark->y >> 16;
              s32 dotSize = spark->variant / 16 + 1;
              EFFECT_DRAW_MOTE(draw, canvas, sprites, x, y, dotSize);
            }
          }
        }
      }
      Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);

      // The formation is pushed back from frame 32.
      if (frame > 15) {
        if (frame == 32) {
          formationSpeedX = 0x40000;
          formationSpeedY = -0x4000;
        }
        if (frame > 31) {
          formationX += formationSpeedX;
          formationY += formationSpeedY;
          formationSpeedX = formationSpeedX * 60 / 64;
          formationSpeedY = formationSpeedY * 60 / 64;
        }
        BattleFx_PlaceFormationObjects(0, formationX, formationY);
      }
      // Smoke around the formation.
      if ((u32)(frame - 16) <= 127) {
        s32 formationScreenX;
        for (i = 0, formationScreenX = formationX >> 17; i != 3; i++) {
          s32 fullWidth = i & 3;
          s32 angle = Random16() & 0xffff;
          s32 x = (((Trig_Sin(angle) * 8)) >> 16) + formationScreenX -
                  (gBattleFx_GlintCellWidths[fullWidth] >> 1);
          s32 y = ((Trig_Cos(angle) * 40) >> 16) -
                  (gBattleFx_GlintCellHeights[fullWidth] >> 1);
          EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                              3 | gThor_SmokeFlips[Random16() & 3], 2);
          gBlitters[HEAP_SLOT_BLITTER_ALTERNATE](
              canvas, Ram_MapCellBuffer + gBattleFx_GlintCellOffsets[fullWidth],
              x, y + 56, gBattleFx_GlintCellWidths[fullWidth],
              gBattleFx_GlintCellHeights[fullWidth]);
          Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
        }
      }
      if (frame <= 31) {
        Camera_ApplyShake(8, 16);
      } else {
        Camera_ApplyShake(4, 4);
      }
      EFFECT_END_FRAME(work);
    }

    Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
    Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
    BattleFx_RunImpactBurst(0, formationX, formationY);
  }
  for (i = 0; i != 9; i++) {
    ResourceObject_ReleaseFar(work->objects[i]);
  }
  BattleFx_EndCanvasLayer();
}

// The shard pictures in the sheet, after the boulder.
#define SHATTER_SHARD_CELLS 0x83c
#define SPIRE_SHARDS 21 // shards per rock

// gSpire_ShardOffsets has an x and a y for each shard.
#define SPIRE_SHARD_X(shard) gSpire_ShardOffsets[(shard) * 2]
#define SPIRE_SHARD_Y(shard) gSpire_ShardOffsets[(shard) * 2 + 1]

// Abilities: Spire, Clay Spire, Stone Spire (kind 2).
//
// Shattering rocks: boulders drop along the row of targets, centred on
// it, and burst into tumbling shards, hitting every target as each lands.
//
// object: the targets; variant picks the number of boulders
//         (gSpire_RockCounts).
void BattleFx_RunSpire(struct BattleEffectArgument *object) {
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle blit;
  s32 rockCount;
  struct EffectPosition first;
  struct EffectPosition last;
  struct EffectStep *rock;
  struct EffectStep *shard;
  s32 frame;
  s32 i;
  s32 j;
  s32 cell;
  u32 width;
  u32 height;

  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  work->effect = object;
  BattleFx_BeginCanvasLayer(1);
  REG_BG2PA = Q_8_8(1);
  REG_BLDCNT = 0;
  Resource_LoadAndDecompress((s32)&ResourceId_BoulderSheet, work->sheet, 1, 1);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                      COMPOSITE_TRANSPARENT);
  blit = EFFECT_SLOT(slots, HEAP_SLOT_BLITTER);
  EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &first);
  EffectPosition_ApplyStepAndYOffset(
      work->effect->actors[work->effect->count - 1], &last);
  first.x += (last.x - first.x) / 2;
  REG_BG2X = (64 - first.x) << 8;
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  rockCount = gSpire_RockCounts[work->effect->variant];
  for (i = 0; i != rockCount; i++) {
    rock = &work->particles[i];
    rock->y = -0x400000;
    rock->velocityY = 0;
  }
  // Each rock breaks into 21 shards, thrown up from where it lands.
  for (i = 0; i != rockCount; i++) {
    for (j = 0; j != SPIRE_SHARDS; j++) {
      struct EffectStep *shard = &PARTICLES[i * SPIRE_SHARDS + j];

      shard->x = (SPIRE_SHARD_X(j) + gSpire_RockX[i]) << 16;
      shard->y = SPIRE_SHARD_Y(j) << 16;
      shard->velocityX = ((Random16() % 96) - 48) << 10;
      shard->velocityY = -((Random16() & 127) + 32) << 11;
      shard->z = 32;
      shard->variant = 0;
    }
  }
  for (frame = 0; frame != gSpire_DropFrames[rockCount - 1] + 80; frame++) {
    if (frame == gSpire_DropFrames[rockCount - 1] + 48)
      BattleEventRuntime_BeginPhaseFar(132);
    for (i = 0; i != rockCount; i++) {
      rock = &work->particles[i];
      if (frame == gSpire_DropFrames[i] + 18) {
        AudioCommand_PlayFar(134);
        work->shakeFrames = 4;
      }
      if (frame >= gSpire_DropFrames[i] + 18) {
        for (j = 0; j != SPIRE_SHARDS; j++) {
          shard = &PARTICLES[i * SPIRE_SHARDS + j];
          cell = (j % 5) * 3 + shard->variant / 96 % 3;
// Sequence dimensions before the call; assigning and reading them in
// separate arguments was UB. Plain English trial: unchanged 0x3bc bytes,
// with different instructions. Preserve the original native calls.
#ifdef UBFIX
          width = gSpire_ShardWidths[cell];
          height = gSpire_ShardHeights[cell];
          blit(canvas,
               work->sheet + gSpire_ShardCells[cell] + SHATTER_SHARD_CELLS,
               HI(shard->x) - (width >> 1), HI(shard->y) - (height >> 1), width,
               height);
#else
          blit(canvas,
               work->sheet + gSpire_ShardCells[cell] + SHATTER_SHARD_CELLS,
               HI(shard->x) - ((width = gSpire_ShardWidths[cell]) >> 1),
               HI(shard->y) - ((height = gSpire_ShardHeights[cell]) >> 1),
               width, height);
#endif
          EffectStep_AdvanceWithGravity2D(shard, 64, 0x4000);
          shard->variant += shard->z;
          if (shard->z > 1 && (frame & 1))
            shard->z--;
        }
      }
      // The rock falls from its drop frame and lands 18 frames later.
      if (frame < gSpire_DropFrames[i] + 18) {
        if (frame >= gSpire_DropFrames[i])
          blit(canvas, work->sheet, gSpire_RockX[i] + 47, HI(rock->y), 34, 62);
        rock->y += rock->velocityY;
        if (frame > gSpire_DropFrames[i])
          rock->velocityY += 0x10000;
        if (rock->y > 0x320000)
          rock->y = 0x320000;
      }
      if (frame == gSpire_DropFrames[i] + 18) {
        EFFECT_FOR_EACH_TARGET(j, work->effect)
          ObjectGroup_UpdateMembers(work->effect->actors[j], SPRITE_COLOR_WHITE,
                                    POSE_DAMAGE, j, 8);
      }
    }
    Camera_ApplyShake(2, 4);
    EFFECT_END_FRAME(work);
  }
  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

// Abilities: Gust (kind 219); Rapid Smash, Sonic Smash, Slice (kind 310).
//
// The flurry's entries: each passes its kind of flurry to
// BattleFx_RunBurstScene. The dispatch table supplies actual effect
// arguments; their variant selects the flurry when an entry covers several
// modes.
void BattleFx_RunGust(struct BattleEffectArgument *effect) {
  if (effect->variant == 0) {
    BattleFx_RunBurstScene(effect, 6);
    return;
  }
  if (effect->variant == 1) {
    BattleFx_RunBurstScene(effect, 7);
    return;
  }
  BattleFx_RunBurstScene(effect, 8);
}

// Abilities: Bear Claw (kind 317); Counterstrike (kind 342).
//
// Play the first flurry style selected by the effect table.
DEFINE_EFFECT_KIND(BattleFx_RunBearClaw, BattleFx_RunBurstScene, 0)

// Abilities: Ransack (kind 321); Mad Dash (kind 343).
//
// Use the same first flurry style for this alternate effect-table entry.
DEFINE_EFFECT_KIND(BattleFx_RunRansack, BattleFx_RunBurstScene, 0)

// Abilities: Bacteria Rush (kind 328).
//
// Play the second flurry style selected by the effect table.
DEFINE_EFFECT_KIND(BattleFx_RunBacteriaRush, BattleFx_RunBurstScene, 1)

// Abilities: Freebite Rush (kind 334).
//
// Play the third flurry style selected by the effect table.
DEFINE_EFFECT_KIND(BattleFx_RunFreebiteRush, BattleFx_RunBurstScene, 2)

// Abilities: Flying Attack (kind 319); Swift Strike (kind 329).
//
// Choose flurry styles 3, 4 or 5 from the effect variant.
void BattleFx_RunFlyingAttack(struct BattleEffectArgument *effect) {
  if (effect->variant == 0) {
    BattleFx_RunBurstScene(effect, 3);
    return;
  }
  if (effect->variant == 1) {
    BattleFx_RunBurstScene(effect, 4);
    return;
  }
  BattleFx_RunBurstScene(effect, 5);
}

// Flurries of strikes, and the rising rock wall.
// The bytes of a gBurstScene_Records row.
enum BurstField {
  BURST_ARC_SHEET, // 0: blue arc A, else B; 1 also draws every strike as an arc
  BURST_PALETTE,   // the palette's resource (0 to 3)
  BURST_STRIKES,   // the strikes
  BURST_MOTES,     // motes thrown by each strike
  BURST_INTERVAL,  // frames between strikes
  BURST_FRAMES,    // frames in the whole effect
  BURST_FLASH      // 1 fills the canvas on each hit
};

// The value of one field of this flurry's gBurstScene_Records row.
#define BURST_RECORD(field) (gBurstScene_Records[variant * 7 + (field)])

// The packed flash pictures and the waves.
#define BURST_FLASH_CELLS 0x5100
#define BURST_WAVE_CELLS (Ram_MapCellBuffer + 0x5e00)

// Flurry: the user closes on the first target and strikes it again and
// again; each strike shows an arc (a wave for the third and fourth of
// every four), flashes and throws motes, and the last one ends the attack.
//
// effect: the user and the targets; prelude 1 brings in a Jupiter djinni
//         first.
// variant: the row of gBurstScene_Records.
void BattleFx_RunBurstScene(struct BattleEffectArgument *effect, s32 variant) {
  struct EffectPosition actorPos;
  struct EffectPosition targetPos;
  struct EffectPosition screen;
  s32 screenX;
  s32 screenY;
  BlitterPair draw;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  u8 *sprites;
  struct BattleCamera *camera;
  struct MotionObject *target;
  u8 *palette;
  s32 paletteId;
  s32 row;
  s32 col;
  s32 i;
  s32 j;

  work = *(gWorkSlot + HEAP_SLOT_BATTLE_EFFECT);
  canvas = *(gWorkSlot + HEAP_SLOT_BATTLE_CANVAS);
  sprites = *(gWorkSlot + HEAP_SLOT_BATTLE_SHEET);
  camera = *(gWorkSlot + HEAP_SLOT_CAMERA);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  if (work->effect->prelude == 1)
    BattleFx_PrepareCanvasEffect(effect, 7, work->effect->side, 2, &screenX,
                                 &screenY);

  // Pictures and palette.
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sprites, 0, 0);

  // Pack the flash (40x288) to half width at BURST_FLASH_CELLS, as the
  // canvas is shown at double width.
  Resource_LoadAndDecompress((s32)&ResourceId_YellowOrbSheet, work->sheet, 1,
                             0);
  for (row = 0; row != 288; row++) {
    for (col = 0; col != 40; col++) {
      s32 src = row * 40 + col;
      s32 dst = row * 20 + col / 2 + BURST_FLASH_CELLS;

      work->sheet[dst] = work->sheet[src];
    }
  }
  if (BURST_RECORD(BURST_ARC_SHEET) == 0)
    Resource_LoadAndDecompress((s32)&ResourceId_BlueArcSheetA, work->sheet, 1,
                               1);
  else
    Resource_LoadAndDecompress((s32)&ResourceId_BlueArcSheetB, work->sheet, 1,
                               1);
  Resource_LoadAndDecompress((s32)&ResourceId_WaveSheet, BURST_WAVE_CELLS, 1,
                             0);
  switch (BURST_RECORD(BURST_PALETTE)) {
  case 0:
    paletteId = (s32)&ResourceId_MarsDjinnSheet;
    break;
  case 1:
    paletteId = (s32)&ResourceId_LimePalette;
    break;
  case 2:
    paletteId = (s32)&ResourceId_BlueArcSheetB;
    break;
  default:
    paletteId = (s32)&ResourceId_EmberStreakSheet;
    break;
  }
  palette = Resource_GetTableEntry(paletteId);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  WaitFrames(1);

  EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &targetPos);
  work->transferMode = CANVAS_COPY_AND_FADE; // trails, fading to 3/4
  work->transferValue = 75;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);

  // Motes wait 25 pixels above the target, set to fly towards the middle
  // of the field.
  target = GetBattleObjectSlotFar(work->effect->actors[0])->object;
  for (i = 0; i != 768; i++) {
    struct EffectStep *mote = &PARTICLES[i];

    EFFECT_SET_POSITION(mote, target->x, target->y + 0x190000, target->z);
    EFFECT_SET_VELOCITY(mote, (Random16() & 255) << 12,
                        ((Random16() & 255) - 127) << 12,
                        ((Random16() & 255) - 127) << 12);
    if (mote->x > 0)
      mote->velocityX = -mote->velocityX;
    mote->variant = -1; // frames left (negative: not out yet)
  }
  BattleMotion_ApproachTargetFar(work->effect->actor, work->effect->actors[0],
                                 4, 0);

  for (frame = 0; frame != BURST_RECORD(BURST_FRAMES); frame++) {
    s32 bursts = BURST_RECORD(BURST_STRIKES);

    EffectPosition_ApplyStepAndYOffset(work->effect->actor, &actorPos);
    actorPos.x /= 2;

    // Arcs and waves; odd strikes are drawn flipped.
    if (work->effect->side == 0) {
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER,
                          COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, COMPOSITE_MAX);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                          COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                              COMPOSITE_FLIP_Y,
                          COMPOSITE_MAX);
    } else {
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER,
                          COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                              COMPOSITE_FLIP_X,
                          COMPOSITE_MAX);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                          COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP,
                          COMPOSITE_MAX);
    }
    draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
    draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
    for (i = 0; i != bursts; i++) {
      s32 start = i * BURST_RECORD(BURST_INTERVAL);

      if (frame >= start && frame < start + 6) {
        s32 step = frame - start;

        if ((i & 3) <= 1 || BURST_RECORD(BURST_ARC_SHEET) == 1) {
          // The arc: six 48x72 frames.
          if (work->effect->side == 0)
            draw[i & 1](canvas, work->sheet + step * 3456, targetPos.x / 2 - 16,
                        targetPos.y - 40, 48, 72);
          else
            draw[i & 1](canvas, work->sheet + step * 3456, targetPos.x / 2 - 32,
                        targetPos.y - 40, 48, 72);
        } else {
          // The wave: six 48x16 frames.
          if (work->effect->side == 0)
            draw[i & 1](canvas, BURST_WAVE_CELLS + step * 768,
                        targetPos.x / 2 - 16, actorPos.y - 8, 48, 16);
          else
            draw[i & 1](canvas, BURST_WAVE_CELLS + step * 768,
                        targetPos.x / 2 - 32, actorPos.y - 8, 48, 16);
        }
      }
      if (frame == start + 2) {
        // The hit; the last strike ends the attack.
        if (BURST_RECORD(BURST_FLASH) == 1)
          Iwram_FillWords(canvas, 0x4000, 0x2f2f2f2f);
        ObjectGroup_UpdateMembers(work->effect->actors[0], SPRITE_COLOR_WHITE,
                                  POSE_DAMAGE, 0, 4);
        if (i == bursts - 1) {
          BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 4);
          work->shakeFrames = 8;
          BattleEventRuntime_BeginPhaseFar(134);
        } else {
          if (i & 1)
            BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 7);
          work->shakeFrames = 4;
          AudioCommand_PlayFar(134);
        }
        for (j = 0; j != BURST_RECORD(BURST_MOTES); j++)
          PARTICLES[i * 32 + j].variant = (Random16() & 7) + 15;
      }
      // The flash: six 20x48 frames, 2 frames each.
      if (frame >= start + 2 && frame < start + 14)
        draw[0](canvas,
                work->sheet + ((frame - start - 2) / 2) * 960 +
                    BURST_FLASH_CELLS,
                targetPos.x / 2 - 10, targetPos.y - 24, 20, 48);
    }
    EFFECT_RELEASE_BLITTERS();

    // Motes: drawn until they die out, bouncing off the ground.
    EFFECT_VIEW_FROM_CAMERA(camera);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                        COMPOSITE_ADDITIVE);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                        COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, COMPOSITE_MAX);
    draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
    draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
    for (i = 0; i != 512; i++) {
      struct EffectStep *mote = &PARTICLES[i];

      s32 size = mote->variant;

      if (size > 0) {
        EffectPosition_ApplyBaseAndYOffset(&mote->x, &screen);
        screen.x /= 2;
        size >>= 3;
        size++;
        draw[(i / 2) & 1](canvas, sprites + gBattleFx_MoteCellOffsets[size - 1],
                          screen.x - size / 2, screen.y - size, size, size * 2);
        EffectStep_AdvanceWithGravity3D(mote, 60, -0x400);
        if (mote->y < 0x80000)
          mote->velocityY = -mote->velocityY / 2;
        mote->variant--;
      }
    }
    EFFECT_RELEASE_BLITTERS();
    Camera_ApplyShake(8, 8);
    EFFECT_END_FRAME(work);
  }
  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  BattleFx_EndCanvasLayer();
}

// The standing wall's top (48x96, stretched from 48x32) and foot (48x21)
// in the rebuilt sheet.
#define WALL_TOP_CELLS 0x13c0
#define WALL_FOOT_CELLS 0x25c0
#define GAIA_ROCKS 11 // rocks that tumble up the wall

// Abilities: Gaia, Mother Gaia, Grand Gaia, Rockfall, Rockslide, Avalanche
// (kind 3).
//
// Rising wall: the ground swells, a wall of rock rises from it, stands
// while rocks tumble up its face and the targets are battered, and sinks
// again. The wall is symmetric: its left half is drawn, then mirrored for
// the right. Its centre is at canvas x 64 + wallX. The rocks are 11
// animation objects, moved by particles[10] to particles[20].
//
// effect: the targets; variant 0 to 2 widens the wall.
void BattleFx_RunGaia(struct BattleEffectArgument *effect) {
  static struct SpriteScale unitScale = {{{0x10000, 0x10000}}};
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  BlitterPair draw;
  u8 *sprites;
  s32 wallX;
  s32 rockX;
  struct SpriteScale scale;
  struct SpritePlacement position;
  u8 *src;
  u16 *color;
  void *palette = (void *)BG_PLTT;
  s32 sheetIndex;
  s32 frame;
  s32 i;
  s32 j;
  s32 k;

  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  sprites = EFFECT_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);

  // The higher the variant, the wider the wall.
  if (work->effect->variant == 0)
    REG_BG2PA = Q_8_8(0.8); // 1.25 times the width
  else if (work->effect->variant == 1)
    REG_BG2PA = Q_8_8(1.0 / 1.5); // 1.5 times the width
  if (work->effect->side == 1) {
    wallX = 8;
    if (work->effect->variant == 0)
      rockX = 40;
    else if (work->effect->variant == 1)
      rockX = 36;
    else
      rockX = 40;
  } else {
    wallX = -16;
    rockX = -12;
  }
  REG_BG2X = gGaia_Bg2Shifts[work->effect->side][work->effect->variant] << 8;

  // Palette: background colours 1 to 63, each channel 8 steps darker.
  Resource_LoadAndDecompress((s32)&ResourceId_EarthWallSheet, Ram_MapCellBuffer,
                             1, 0);
  Iwram_CopyWords(palette,
                  Resource_GetTableEntry((s32)&ResourceId_OrangePaletteA), 128);
  color = (u16 *)&BG_PLTT[1];
  for (i = 0; i != 63; i++) {
    s32 entry = *color;
    s32 blue = ((u16)entry >> 10) & 31;
    s32 green = ((u16)entry >> 5) & 31;
    s32 red = 31;

    red &= entry;
    blue -= 8;
    green -= 8;
    red -= 8;

    if (blue < 0)
      blue = 0;
    if (green < 0)
      green = 0;
    if (red < 0)
      red = 0;
    *color++ = (blue << 10) | (green << 5) | red;
  }

  // Rebuild the pictures in work->sheet:
  //   the growing wall: nine one-line rows, each stretched to 32 lines;
  //   the top of the wall (WALL_TOP_CELLS): 48x32 stretched to 48x96;
  //   the foot of the wall (WALL_FOOT_CELLS): 48x21;
  //   the swelling ground: three frames.
  src = Ram_MapCellBuffer;
  sheetIndex = 0;
  for (j = 0; j != 9; j++) {
    for (i = 0; i != 32; i++) {
      for (k = 0; k != gGaia_RowWidths[j]; k++, sheetIndex++)
        work->sheet[sheetIndex] = src[k];
    }
    src += gGaia_RowWidths[j];
  }
  for (j = 0; j != 32; j++) {
    for (i = 0; i != 3; i++) {
      for (k = 0; k != 48; k++, sheetIndex++)
        work->sheet[sheetIndex] = src[k];
    }
    src += 48;
  }
  for (i = 0; i != 1008; i++)
    work->sheet[sheetIndex++] = *src++;
  for (i = 0; i != 3; i++) {
    for (k = 0; k != gGaia_MoundWidths[i] * gGaia_MoundHeights[i]; k++)
      work->sheet[sheetIndex++] = *src++;
  }
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sprites, 0, 0);

  // The rock objects.
  for (i = 0; i != GAIA_ROCKS; i++) {
    struct AnimationObject *rock = ResourceObject_CreateFar(390);

    work->objects[i] = rock;
    if (rock != 0) {
      rock->flags = 0;
      AnimationObjects_SelectAnimationFar(rock, i / 4);
      work->objects[i]->image.priority = 1;
    }
  }
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                      COMPOSITE_MAX);
  draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                      COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X,
                      COMPOSITE_MAX);
  draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
  REG_BLDCNT =
      BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  work->transferMode = CANVAS_COPY_AND_FADE; // trails, fading to 3/4
  work->transferValue = 75;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);

  // Rocks: x is the column, y counts down from 128 by z, velocityX cycles
  // through 1 to 12, and variant is the wait before the rock appears.
  for (i = 0; i != GAIA_ROCKS; i++) {
    struct EffectStep *rock = &work->particles[10 + i];

    EFFECT_SET_POSITION(rock, (Random16() & 15) + 88, 128,
                        (Random16() & 15) + 2);
    EFFECT_SET_VELOCITY(rock, i, 1, 0x8000);
    rock->variant = 44 - i * 4;
  }
  for (i = 0; i != 6; i++)
    work->particles[16 + i].x = gGaia_LateRockColumns[i];

  // Dust rises from the foot of the wall.
  for (i = 0; i != 256; i++) {
    struct EffectStep *dust = &PARTICLES[i];

    dust->x = ((Random16() & 63) + wallX + 32) << 16;
    dust->y = ((Random16() & 7) + 96) << 16;
    dust->velocityY = ((Random16() & 63) + 32) << 13;
    dust->variant = Random16() & 31;
  }
  REG_BG2CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7);
  work->shakeFrames = 250;
  for (frame = 0; frame != 192; frame++) {
    if (frame == 0)
      AudioCommand_PlayFar(212);
    if (frame == 40)
      AudioCommand_PlayFar(141);
    if (frame == 96)
      AudioCommand_PlayFar(145);
    if (frame == 120)
      BattleEventRuntime_BeginPhaseFar(134);

    // The swelling ground (frames 0 to 81).
    if (frame <= 81) {
      s32 cell = frame / 4;

      if (cell > 2)
        cell = (cell & 1) + 1;
      draw[0](canvas, work->sheet + gGaia_MoundSheetOffsets[cell],
              wallX - gGaia_MoundWidths[cell] + 64,
              116 - gGaia_MoundHeights[cell], gGaia_MoundWidths[cell],
              gGaia_MoundHeights[cell]);
      draw[1](canvas, work->sheet + gGaia_MoundSheetOffsets[cell], wallX + 64,
              116 - gGaia_MoundHeights[cell], gGaia_MoundWidths[cell],
              gGaia_MoundHeights[cell]);
    }

    // The rising wall (frames 12 to 87).
    if (frame >= 12 && frame <= 87) {
      s32 row = (frame - 64) / 3;

      EFFECT_CLAMP(row, 0, 7);
      for (i = 0; i != 4; i++) {
        draw[0](canvas, work->sheet + gGaia_RowSheetOffsets[row],
                wallX - gGaia_RowWidths[row] + 64, i * 32 - 12,
                gGaia_RowWidths[row], 32);
        draw[1](canvas, work->sheet + gGaia_RowSheetOffsets[row], wallX + 64,
                i * 32 - 12, gGaia_RowWidths[row], 32);
      }
    }

    // The sinking wall (frames 160 to 183).
    if (frame >= 160 && frame <= 183) {
      s32 row = 7 - (frame - 160) / 3;

      EFFECT_CLAMP(row, 0, 7);
      for (i = 0; i != 4; i++) {
        draw[0](canvas, work->sheet + gGaia_RowSheetOffsets[row],
                wallX - gGaia_RowWidths[row] + 64, i * 32 - 12,
                gGaia_RowWidths[row], 32);
        draw[1](canvas, work->sheet + gGaia_RowSheetOffsets[row], wallX + 64,
                i * 32 - 12, gGaia_RowWidths[row], 32);
      }
    }

    // The standing wall (frames 88 to 159).
    if (frame >= 88 && frame <= 159) {
      s32 width = 48;

      draw[0](canvas, &work->sheet[WALL_TOP_CELLS], wallX + 16, 0, width, 96);
      draw[1](canvas, &work->sheet[WALL_TOP_CELLS], wallX + 64, 0, width, 96);
      draw[0](canvas, &work->sheet[WALL_FOOT_CELLS], wallX + 16, 96, width, 21);
      draw[1](canvas, &work->sheet[WALL_FOOT_CELLS], wallX + 64, 96, width, 21);
    }

    // Dust (from frame 88): until frame 159, dust that leaves the top
    // returns to the foot.
    if (frame > 87) {
      for (i = 0; i != 64; i++) {
        struct EffectStep *dust = &PARTICLES[i];

        if (dust->variant == 0) {
          s32 size = (i & 3) + 5;

          EFFECT_DRAW_MOTE(draw[0], canvas, sprites, HI(dust->x), HI(dust->y),
                           size);
          dust->y -= dust->velocityY;
          if (dust->y < 0 && frame <= 159)
            dust->y = 0x600000;
        } else {
          dust->variant--;
        }
      }
    }

    // Rocks (from frame 5): from frame 72 there are 11 of them, larger.
    if (frame > 4) {
      s32 rocks = 6;

      if (frame > 71)
        rocks = GAIA_ROCKS;
      for (k = 0; k != rocks; k++) {
        struct EffectStep *rock = &work->particles[10 + k];
        s32 slot;

        if (rock->variant == 0) {
          scale = unitScale;
          if (frame > 71) {
            scale.x = (k << 12) + 0x8000;
            scale.x += work->effect->variant << 14;
          } else {
            scale.x = 0x8000;
          }
          scale.y = scale.x;
          position.groundHeight = 0;
          position.x = (rock->x + rockX * 2) << 16;
          position.height = 0x2000000 - (rock->y << 16);
          position.y = 0x2000000;
          slot = (frame / 2 + k) % GAIA_ROCKS;
          if (slot != -1)
            Object_ApplyProjectedPlacementFar(work->objects[slot], &position,
                                              &scale, 0);
          rock->y -= rock->z;
          rock->velocityX += rock->velocityY;
          if (rock->velocityX > 12)
            rock->velocityX -= 12;
          if (rock->y < 0) {
            // Below 0 the rock starts again at 128 (from frame 88 in a new
            // column, at a new speed).
            if (frame > 159) {
              rock->variant = -1;
            } else {
              if (frame > 87) {
                rock->z = (Random16() & 7) + 8;
                if (work->effect->variant == 0)
                  rock->x = Random16() % 96 + 42;
                else if (work->effect->variant == 1)
                  rock->x = Random16() % 112 + 34;
                else
                  rock->x = Random16() % 160 + 10;
              }
              rock->y = 128;
              rock->variant = 8;
            }
          }
        } else {
          rock->variant--;
        }
      }
    }

    // The targets (frames 86 to 158).
    if (frame <= 158) {
      EFFECT_FOR_EACH_TARGET(i, work->effect) {
        if (frame > 85) {
          if (frame % 12 == 0)
            ObjectGroup_UpdateMembers(work->effect->actors[i],
                                      SPRITE_COLOR_WHITE, POSE_DAMAGE, i, 6);
          if ((frame & 3) == 0)
            BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 5);
        }
      }
    }
    if (frame < 90 || frame > 160)
      Camera_ApplyShake(2, 2);
    else
      Camera_ApplyShake(8, 8);
    EFFECT_END_FRAME(work);
  }
  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  EFFECT_RELEASE_BLITTERS();
  for (i = 0; i != GAIA_ROCKS; i++)
    ResourceObject_ReleaseFar(work->objects[i]);
  BattleFx_EndCanvasLayer();
}

// Particle motion and projection into the battle view.
//
// Effect particles are EffectStep records in 16.16 fixed point. The two
// integrators move a particle one frame: they add its velocity to its
// position, add gravity to its vertical speed and then damp every speed by a
// factor out of 64. The 3D one works in field space, before projection; the
// 2D one works directly on the screen.
//
// The EffectPosition_Apply*AndYOffset helpers turn a point or a battler into
// an EffectPosition on the screen through the current transform. Each returns
// a point 16 pixels higher than the projection, which is where effects anchor
// on a battler. They differ only in how the battler is projected: at its
// sprite's scaled height, scaled for the battle view, or plainly.

// Moves a particle one frame in three dimensions: adds its velocity, then
// gravity to its vertical speed, then damps every speed by damping / 64.
void EffectStep_AdvanceWithGravity3D(struct EffectStep *step, s32 damping,
                                     s32 gravity) {
  step->x += step->velocityX;
  step->y += step->velocityY;
  step->z += step->velocityZ;
  step->velocityY += gravity;
  EFFECT_SET_VELOCITY(step, step->velocityX * damping / 64,
                      step->velocityY * damping / 64,
                      step->velocityZ * damping / 64);
}

// Moves a particle one frame on the screen: adds its velocity, then
// gravity to its vertical speed, then damps both speeds by damping / 64.
void EffectStep_AdvanceWithGravity2D(struct EffectStep *step, s32 damping,
                                     s32 gravity) {
  step->x += step->velocityX;
  step->y += step->velocityY;
  step->velocityY += gravity;
  step->velocityX = step->velocityX * damping / 64;
  step->velocityY = step->velocityY * damping / 64;
}

// Projects a point in the current transform onto the screen, 16 pixels
// higher, and returns the projection's scale.
s32 EffectPosition_ApplyBaseAndYOffset(const void *point,
                                       struct EffectPosition *position) {
  s32 result = Render_ProjectPoint(point, EffectPosition_Coordinates(position));
  position->y -= 16;
  return result;
}

// The screen position of a battler's sprite, at its scaled height, 16
// pixels higher.
void EffectPosition_ApplyAnimationAndYOffset(s32 id,
                                             struct EffectPosition *position) {
  BattleUnit_ProjectToScreen(id, EffectPosition_Coordinates(position));
  position->y -= 16;
}

// The screen position of a battler, scaled for the battle view, 16 pixels
// higher.
void EffectPosition_ApplyStepAndYOffset(s32 id,
                                        struct EffectPosition *position) {
  BattleMotion_ProjectScaledPositionFar(id,
                                        EffectPosition_Coordinates(position));
  position->y -= 16;
}

// The screen position of a battler, 16 pixels higher.
void EffectPosition_ApplyAlternateStepAndYOffset(
    s32 id, struct EffectPosition *position) {
  BattleMotion_ProjectPositionFar(id, EffectPosition_Coordinates(position));
  position->y -= 16;
}

// Projects a battler onto the screen, raised by its table height at the
// sprite's scale.
//
// unit: the battler.
// screen: receives x and y.
// Returns 0.
s32 BattleUnit_ProjectToScreen(s32 unit, s32 *screen) {
  struct BattleCamera *camera;
  struct MotionObject *object;
  struct AnimationObject *info;
  s32 scale;
  /* FAKEMATCH: needed to match: the unused 12-byte vector preserves both stack
   * adjustments and camera-load order. */
  s32 unused[3];

  camera = gCameraWork;
  object = GetBattleObjectSlotFar(unit)->object;
  info = GetMotionRecordFar(object, 0);
  EFFECT_VIEW_FROM_CAMERA(camera);
  scale = Iwram_MulQ16(Render_ProjectPoint(&object->x, screen), info->scale);
  screen[1] -= Iwram_MulQ16(scale, Battle_GetObjectTableValueFar(unit) >> 17);
  return 0;
}

// Reads the held keys when the first target is an enemy (id 128 or more)
// and does nothing with them.
void BattleFx_ProbeKeysForExtendedActor(void) {
  struct BattleEffectWork *work = WORK_SLOT(HEAP_SLOT_BATTLE_EFFECT);
  struct BattleEffectArgument *effect = work->effect;

  if (effect->actors[0] > 0x7f)
    (void)gKeysHeld;
}

// Casting impacts, beam sequences and ring-and-spark scenes.
//
// A casting impact (enum CastingImpact, the ability's effect kind less 100)
// runs in four phases:
//
// - Gathering: for 32 frames 64 sparks close in on the caster, four more each
//   frame, each in its own random rotation. Djinni impacts (Bane to Sap, 0 to
//   7) show their Djinni 80 pixels over the caster once the first sparks have
//   gone out.
// - Background: BG2 is placed so the impact sits over the target. Impacts 15,
//   17, 24, 26 and 35 stop here and hand the canvas to another effect.
// - Impact: for 20 to 80 frames, depending on the impact, the caster's
//   overlay, an orb, sparks and the impact's own animation are drawn while
//   the background palette steps toward the impact's palette. A Djinni flies
//   to the target over the first six frames, and the target is hit on
//   frame 6.
// - Finish: the canvas and its callbacks are released, or impact 21 hands
//   them to the stat reduction.
//
// The particles are used twice: work->particles holds the gathering sparks
// and then, from the impact, 32 scattering sparks (variant is their
// lifetime); the 64 records at gMapCellBuffer + 0x4000 belong to the impact's
// own animation.
//
// The beam sequence draws a growing beam picture and its projected sparks
// over the first target. The ring-and-spark scene plays a strike of rings and
// sparks, restoring the user's motion afterwards. A casting impact may hand
// off to another runner after removing its prelude callbacks and releasing
// the blitters that runner will replace.

// Runs a battle effect from the second range of kinds: 100 to 135 are the
// casting impacts (kind - 100), 136 to 199 the beam sequences, 200 and up
// the ring and spark scenes. Allocates the sheet, the work and the canvas
// for the effect and frees them after.
//
// effect: the effect argument.
void BattleFx_DispatchByIdRange(struct BattleEffectArgument *effect) {
  s32 kind;
  s32 value;

  // One local holds the sheet size, then the kind, then the impact.
  value = 0x60e;
  Runtime_AllocateBlock(HEAP_SLOT_BATTLE_SHEET, value);
  Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_EFFECT,
                            sizeof(struct BattleEffectWork));
  Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_CANVAS, 0x4000);
  value = effect->kind;
  kind = value;
  value = kind - 100;
  if ((u32)value <= 35) {
    BattleFx_RunCastingImpact(effect, value);
  } else if (kind > 199) {
    BattleFx_RunRingAndSparkScene(effect);
  } else {
    BattleFx_RunBeamSequence(effect);
  }
  Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_CANVAS);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_EFFECT);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_SHEET);
}
// Draw the selected beam and its projected sparks over the first target.
void BattleFx_RunBeamSequence(struct BattleEffectArgument *effect) {
  struct EffectPosition screenPosition;
  struct EffectPosition anchor;
  struct EffectPosition point;
  void **heapCache;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 shake;
  u8 *sheet;
  struct BattleCamera *camera;
  s32 kind;
  struct BattleUnit *unit;
  BlitterPair draw;
  DrawRectangle *slots;
  struct MotionObject *target;
  void *colors = (void *)BG_PLTT;
  s32 half;
  s32 frame;
  s32 i;

  heapCache = &gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];
  cursor = heapCache;
  EFFECT_GET_WORK_AND_CANVAS(cursor, work, canvas);
  sheet = EFFECT_SLOT(heapCache, HEAP_SLOT_BATTLE_SHEET);
  camera = EFFECT_SLOT(heapCache, HEAP_SLOT_CAMERA);
  kind = effect->kind;
  work->effect = effect;
  unit = UnitGetFar(effect->actor);
  WaitFrames(1);
  BattleFx_ConfigureEffectDisplay();
  BattleFx_SetupCanvasTileMap();
  REG_BG1CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(0) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(31);
  WaitFrames(1);
  if (kind == 5) {
    if (work->effect->side == 0) {
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER,
                          COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                              COMPOSITE_FLIP_Y,
                          COMPOSITE_ADDITIVE);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                          COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                              COMPOSITE_FLIP_Y,
                          COMPOSITE_MAX);
    } else {
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER,
                          COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                              COMPOSITE_FLIP_X | COMPOSITE_FLIP_Y,
                          COMPOSITE_ADDITIVE);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                          COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                              COMPOSITE_FLIP_X | COMPOSITE_FLIP_Y,
                          COMPOSITE_MAX);
    }
  } else {
    if (work->effect->side == 0) {
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER,
                          COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                          COMPOSITE_ADDITIVE);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                          COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, COMPOSITE_MAX);
    } else {
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER,
                          COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                              COMPOSITE_FLIP_X,
                          COMPOSITE_ADDITIVE);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                          COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                              COMPOSITE_FLIP_X,
                          COMPOSITE_MAX);
    }
  }
  slots = (DrawRectangle *)gWorkSlot;
  draw[0] = slots[HEAP_SLOT_BLITTER];
  draw[1] = slots[HEAP_SLOT_BLITTER_ALTERNATE];
  WaitFrames(1);
  if (kind == 4) {
    Resource_LoadAndDecompress((s32)&ResourceId_WaveSheet, work, 1, 1);
  } else if (kind == 3) {
    Resource_LoadAndDecompress((s32)&ResourceId_BeamSequenceImage, work, 0, 0);
  } else {
    switch (kind) {
    case 0:
    case 1:
    case 5:
      Resource_LoadAndDecompress((s32)&ResourceId_BlueArcSheetA, work, 1, 1);
      break;
    case 2:
      Resource_LoadAndDecompress((s32)&ResourceId_BlueArcSheetB, work, 1, 1);
      break;
    }
  }
  if (work->effect->actor > 7)
    Iwram_CopyWords(
        colors, Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSmallSheet),
        128);
  else
    Iwram_CopyWords(
        colors, Resource_GetTableEntry((s32)&ResourceId_CrescentSheet), 128);
  WaitFrames(1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD, sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_YellowOrbSheet, Ram_MapCellBuffer,
                             1, 0);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 50);
  REG_BG1CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(0) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(31);
  EffectPosition_ApplyAnimationAndYOffset(work->effect->actors[0], &anchor);
  if (work->effect->side == 0)
    shake = 96 - anchor.x;
  else
    shake = 32 - anchor.x;
  if (shake > 0)
    shake = 0;
  if (shake < -128)
    shake = -128;
  anchor.x += shake;
  gBgScroll[1].x = shake;
  gBgScroll[1].y = 80;
  WaitFrames(1);
  target = GetBattleObjectSlotFar(work->effect->actors[0])->object;
  half = Battle_GetObjectTableValueFar(work->effect->actors[0]) / 2;
  for (i = 0; i != 64; i++) {
    struct EffectStep *spark = &work->particles[i];

    EFFECT_SET_POSITION(spark, target->x, target->y + half, target->z);
    EFFECT_SET_VELOCITY(spark, (Random16() & 255) << 10,
                        ((Random16() & 255) - 32) << 10,
                        ((Random16() & 255) - 127) << 10);
    if (spark->x > 0)
      spark->velocityX = -spark->velocityX;
    spark->velocityX = -spark->velocityX;
    spark->variant = i + 16;
  }

  for (frame = 0; frame != 32; frame++) {
    if (frame == 5) {
      if (Summon_IsEntrySecondaryFlaggedFar(unit->classId))
        BattleEventRuntime_BeginPhaseFar(134);
      else
        BattleEventRuntime_BeginPhaseFar(133);
    }
    if (frame == 4)
      BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 0);
    EffectPosition_ApplyStepAndYOffset(work->effect->actor, &screenPosition);
    screenPosition.y += 16;
    if (kind == 4) {
      if (frame <= 11) {
        if (work->effect->side == 0)
          draw[1](canvas, work->sheet + (5 - frame / 2) * 0x300,
                  screenPosition.x + shake - 48, screenPosition.y - 8, 48, 16);
        else
          draw[1](canvas, work->sheet + (5 - frame / 2) * 0x300,
                  screenPosition.x + shake, screenPosition.y - 8, 48, 16);
      }
    } else if ((u32)kind <= 2 || kind == 5) {
      if (frame <= 11) {
        if (work->effect->side == 0)
          draw[1](canvas, work->sheet + frame / 2 * 0xd80,
                  screenPosition.x + shake - 48, screenPosition.y - 40, 48, 72);
        else
          draw[1](canvas, work->sheet + frame / 2 * 0xd80,
                  screenPosition.x + shake, screenPosition.y - 40, 48, 72);
      }
    } else {
      if (frame <= 17) {
        s32 cell = frame / 3;

        if (work->effect->side == 0)
          draw[1](
              canvas, work->sheet + gBeamSequence_CellSheetOffsets[cell],
              screenPosition.x + gBeamSequence_CellReaches[cell] + shake - 58,
              screenPosition.y - gBeamSequence_CellHeights[cell] / 2,
              gBeamSequence_CellWidths[cell], gBeamSequence_CellHeights[cell]);
        else
          draw[1](canvas, work->sheet + gBeamSequence_CellSheetOffsets[cell],
                  screenPosition.x - gBeamSequence_CellReaches[cell] + shake -
                      gBeamSequence_CellWidths[cell] + 58,
                  screenPosition.y - gBeamSequence_CellHeights[cell] / 2,
                  gBeamSequence_CellWidths[cell],
                  gBeamSequence_CellHeights[cell]);
      }
    }
    if (frame >= 4 && frame <= 15)
      draw[0](canvas, Ram_MapCellBuffer + (frame - 4) / 2 * 0x780,
              anchor.x - 16, anchor.y - 24, 40, 48);
    EFFECT_VIEW_FROM_CAMERA(camera);
    if (frame >= 4 && frame <= 31) {
      for (i = 0; i != 64; i++) {
        s32 pair = i / 2;
        struct EffectStep *spark = &work->particles[pair];
        s32 size = spark->variant;

        if (size > 0) {
          Render_ProjectPoint(&spark->x, EffectPosition_Coordinates(&point));
          point.x += shake;
          size >>= 3;
          size += 2;
          EFFECT_DRAW_FLARE(draw[pair & 1], canvas, sheet, point.x, point.y,
                            size);
          EffectStep_AdvanceWithGravity3D(spark, 60, -0x400);
          spark->variant--;
        }
      }
    }
    EFFECT_PRESENT_FRAME(work);
  }

  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  EFFECT_RELEASE_BLITTERS();
  gBgScroll[1].y = 32;
  BattleFx_SetTransitionFlagAndDisplay();
}

// Play the selected ring-and-spark strike, restoring the user motion after it.
void BattleFx_RunRingAndSparkScene(struct BattleEffectArgument *effect) {
  struct EffectPosition screenPosition;
  struct EffectPosition anchor;
  struct EffectPosition point;
  struct EffectPosition spot;
  void **heapCache;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 shake;
  u8 *sheet;
  struct BattleCamera *camera;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 acceleration;
  s32 strength;
  BlitterPair draw;
  DrawRectangle *slots;
  struct MotionObject *object;
  struct MotionObject *target;
  u16 *background;
  void *colors = (void *)BG_PLTT;
  s32 words = 0x4000;
  s32 big;
  s32 half;
  s32 frame;
  s32 i;

  heapCache = &gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];
  cursor = heapCache;
  EFFECT_GET_WORK_AND_CANVAS(cursor, work, canvas);
  sheet = EFFECT_SLOT(heapCache, HEAP_SLOT_BATTLE_SHEET);
  camera = EFFECT_SLOT(heapCache, HEAP_SLOT_CAMERA);
  big = 1;
  if (effect->kind <= 199)
    big = 0;
  work->effect = effect;
  BattlePres_SetupTransitionAtPairMidpointFar(effect->actor,
                                              effect->firstTarget, 130);
  WaitFrames(1);
  BattleFx_SetupCanvasTileMap();
  REG_BG1CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(0) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(31);
  if (work->effect->side == 0) {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                        COMPOSITE_ADDITIVE);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                        COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, COMPOSITE_MAX);
  } else {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER,
                        COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X,
                        COMPOSITE_ADDITIVE);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                        COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X,
                        COMPOSITE_MAX);
  }
  slots = (DrawRectangle *)gWorkSlot;
  draw[0] = slots[HEAP_SLOT_BLITTER];
  draw[1] = slots[HEAP_SLOT_BLITTER_ALTERNATE];
  BattlePres_SetupTransitionAtPairMidpointFar(work->effect->actor,
                                              work->effect->firstTarget, 130);
  WaitFrames(1);
  Resource_LoadAndDecompress((s32)&ResourceId_StarBurstSheet,
                             work->imageStorage, 1, 0);
  BattlePres_SetupTransitionAtPairMidpointFar(work->effect->actor,
                                              work->effect->firstTarget, 130);
  WaitFrames(1);
  Resource_LoadAndDecompress((s32)&ResourceId_CrescentSheet, Ram_MapCellBuffer,
                             1, 1);
  if (work->effect->actor > 7)
    Iwram_CopyWords(
        colors, Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSmallSheet),
        128);
  BattlePres_SetupTransitionAtPairMidpointFar(work->effect->actor,
                                              work->effect->firstTarget, 130);
  WaitFrames(1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD, sheet, 0, 0);
  BattlePres_SetupTransitionAtPairMidpointFar(work->effect->actor,
                                              work->effect->firstTarget, 130);
  WaitFrames(1);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);

  if (big == 1) {
    object = GetBattleObjectSlotFar(work->effect->actor)->object;
    for (i = 0; i != 64; i++) {
      struct EffectStep *mote = &work->particles[i];

      EFFECT_SET_POSITION(mote, (Random16() & 63) + 16, 0, 0);
      EFFECT_SET_VELOCITY(mote, Random16() & 0xffff, Random16() & 0xffff,
                          Random16() & 0xffff);
    }
    ObjectDispatch_ApplyValueToChildrenFar(object, 0);
    velocityX = object->velocityX;
    velocityY = object->velocityY;
    velocityZ = object->velocityZ;
    strength = object->verticalMotionStrength;
    acceleration = object->acceleration;
    EFFECT_SET_VELOCITY(object, 0, 0, 0);
    object->acceleration = 0;
    object->verticalMotionStrength = 0;
    EffectPosition_ApplyStepAndYOffset(work->effect->actor, &screenPosition);
    shake = 64 - screenPosition.x;
    gBgScroll[1].x = shake;
    gBgScroll[1].y = 80;
    work->fadeFrames = 24;
    work->fadeStep = 0;
    Scheduler_AddOrUpdateCallback(Palette_StepFadeTransfer, 0xc80);
    AudioCommand_PlayFar(212);
    for (frame = 0; frame != 32; frame++) {
      BattlePres_SetupTransitionAtPairMidpointFar(
          work->effect->actor, work->effect->firstTarget, 130);
      for (i = 0; i != 64; i++) {
        struct EffectStep *mote = &work->particles[i];

        if (mote->x >= 0 && frame >= i / 4) {
          s32 size = (i & 1) + 5;

          Render_ResetTransformState();
          SceneTransform_ApplyRoll(mote->velocityZ);
          SceneTransform_ApplyPitch(mote->velocityX);
          SceneTransform_ApplyYaw(mote->velocityY);
          EffectPosition_ApplyBaseAndYOffset(&mote->x, &point);
          point.x += 64;
          point.y = point.y + screenPosition.y + 24;
          EFFECT_CLAMP(point.depth, -60, 60);
          point.depth += 60;
          EFFECT_DRAW_FLARE(draw[0], canvas, sheet, point.x, point.y, size);
          mote->x -= 4;
        }
      }
      EFFECT_PRESENT_FRAME(work);
    }
    Scheduler_RemoveCallback(Palette_StepFadeTransfer);
    ObjectDispatch_ApplyValueToChildrenFar(object, 16);
    EFFECT_SET_VELOCITY(object, velocityX, velocityY, velocityZ);
    object->acceleration = acceleration;
    object->verticalMotionStrength = strength;
  }

  Iwram_ClearWords(canvas, words);
  Iwram_ClearWords((void *)BG_CHAR_BLOCK(1), words);
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);
  REG_BG1CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(0) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(31);
  EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &anchor);
  if (work->effect->side == 0)
    shake = 32 - anchor.x;
  else
    shake = 96 - anchor.x;
  if (shake > 0)
    shake = 0;
  if (shake < -128)
    shake = -128;
  anchor.x += shake;
  gBgScroll[1].y = 80;
  gBgScroll[1].x = shake;
  target = GetBattleObjectSlotFar(work->effect->actors[0])->object;
  half = Battle_GetObjectTableValueFar(work->effect->actors[0]) / 2;
  for (i = 0; i != 64; i++) {
    struct EffectStep *spark = &work->particles[i];

    EFFECT_SET_POSITION(spark, target->x, target->y + half, target->z);
    EFFECT_SET_VELOCITY(spark, (Random16() & 255) << 10,
                        (Random16() & 255) << 10,
                        ((Random16() & 255) - 127) << 10);
    if (spark->x > 0)
      spark->velocityX = -spark->velocityX;
    spark->variant = i + 16;
  }

  for (frame = 0; frame != 32; frame++) {
    if (frame == 5)
      BattleEventRuntime_BeginPhaseFar(134);
    if (frame == 4)
      BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 0);
    EffectPosition_ApplyStepAndYOffset(work->effect->actor, &screenPosition);
    screenPosition.y += 16;
    if (frame <= 1)
      draw[0](canvas, work->imageStorage, 0, 0, 120, 120);
    else if (frame <= 3)
      draw[0](canvas, work->imageStorage + 0x3840, 0, 0, 120, 120);
    else if (frame <= 5)
      draw[0](canvas, Ram_MapCellBuffer, 0, 0, 120, 120);
    else if (frame <= 7)
      draw[0](canvas, Ram_MapCellBuffer + 0x3840, 0, 0, 120, 120);
    EFFECT_VIEW_FROM_CAMERA(camera);
    if (frame >= 4 && frame <= 31) {
      for (i = 0; i != 64; i++) {
        s32 pair = i / 2;
        struct EffectStep *spark = &work->particles[pair];
        s32 size = spark->variant;

        if (size > 0) {
          EffectPosition_ApplyBaseAndYOffset(&spark->x, &spot);
          spot.x += shake;
          size >>= 3;
          size += 2;
          spot.y += 16;
          EFFECT_DRAW_FLARE(draw[pair & 1], canvas, sheet, spot.x, spot.y,
                            size);
          EffectStep_AdvanceWithGravity3D(spark, 60, -0x400);
          spark->variant--;
        }
      }
    }
    EFFECT_PRESENT_FRAME(work);
  }

  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  EFFECT_RELEASE_BLITTERS();
  gBgScroll[1].y = 32;
  for (frame = 0, background = &gBattleWork->background; frame != 7; frame++) {
    BattlePresentation_SetPaletteLevelFar(*background, 6 - frame);
    WaitFrames(1);
  }
  BattleFx_SetTransitionFlagAndDisplay();
}

// Move each RGB555 channel one level towards the resource palette.
void BattleFx_StepPaletteToResource(s32 resourceId) {
  u16 target[64];
  u16 *color = (u16 *)BG_PLTT;
  s32 i;

  Iwram_CopyWords(target, Resource_GetTableEntry(resourceId), 128);
  target[0] = 0;
  for (i = 0; i != 64; i++, color++) {
    s32 red = *color & 0x1f;
    s32 green = (*color >> 5) & 0x1f;
    s32 blue = (*color >> 10) & 0x1f;
    s32 targetRed = target[i] & 0x1f;
    s32 targetGreen = (target[i] >> 5) & 0x1f;
    s32 targetBlue = (target[i] >> 10) & 0x1f;

    if (red < targetRed)
      red++;
    else if (red > targetRed)
      red--;
    if (green < targetGreen)
      green++;
    else if (green > targetGreen)
      green--;
    if (blue < targetBlue)
      blue++;
    else if (blue > targetBlue)
      blue--;
    target[i] = (blue << 10) | (green << 5) | red;
  }
  Iwram_CopyWords((u16 *)BG_PLTT, target, 128);
}

// Battle effects: Djinn abilities (effects 100-135)

static __inline__ void CastingImpact_LoadResource(const void *id, void *dest,
                                                  s32 skip, s32 copy) {
  /* FAKEMATCH: needed to match: inline scope keeps each resource call's
   * arguments local; only BattleFx_RunCastingImpact loads through it: with its
   * 24 loads called directly it differs throughout and is 2 instructions longer
   * (tbs-en, 2026-10-14), and the same inline as a layer for the whole effect
   * family breaks BattleFx_RunProjectileVolley and BattleFx_RunBurstScene (L6
   * review) */
  Resource_LoadAndDecompress((s32)id, dest, skip, copy);
}

// Pack the casting-impact picture rows through their measured scope.
static __inline__ void CastingImpact_PackRows(void *src, void *dest, s32 width,
                                              s32 height) {
  /* FAKEMATCH: needed to match: keep packing arguments within their own inline
   * scope */
  Graphics_PackTileRows(src, dest, width, height);
}

// Draw one casting-impact image through its measured argument scope.
static __inline__ void CastingImpact_DrawImage(void *canvas, const void *pixels,
                                               s32 x, s32 y, s32 width,
                                               s32 height,
                                               DrawRectangle *draw) {
  /* FAKEMATCH: needed to match: inline scope reloads each image address at its
   * drawing call */
  (*draw)(canvas, pixels, x, y, width, height);
}

// Draw a casting-impact image whose height is twice its width.
static __inline__ void CastingImpact_DrawTallImage(void *canvas,
                                                   const void *pixels, s32 x,
                                                   s32 y, s32 width,
                                                   DrawRectangle *draw) {
  /* FAKEMATCH: needed to match: derive height inside the inline scope so the
   * dimension stores reuse one register instead of keeping two argument
   * temporaries alive */
  (*draw)(canvas, pixels, x, y, width, width * 2);
}

// Draw only the unscrolled height of a casting-impact image.
static __inline__ void CastingImpact_DrawScrolledImage(void *canvas,
                                                       const void *pixels,
                                                       s32 x, s32 y, s32 width,
                                                       s32 height, s32 scroll,
                                                       DrawRectangle *draw) {
  /* FAKEMATCH: needed to match: subtract the scrolled part inside the drawing
   * scope */
  (*draw)(canvas, pixels, x, y, width, height - scroll);
}

// Draw a casting-impact image cropped to 91 pixels high.
static __inline__ void CastingImpact_DrawCroppedImage(void *canvas,
                                                      const void *pixels, s32 x,
                                                      s32 y, s32 width,
                                                      DrawRectangle *draw) {
  /* FAKEMATCH: forwarding inline; the cropped height stays inside its scope.
   * Both calls made through the blitter directly with 91 differ 16 on tbs-en:
   * the stack arguments 128 and 91 go through r0 instead of r1 and the pixels
   * pool load moves ahead of them. */
  (*draw)(canvas, pixels, x, y, width, 91);
}

// Clear the prelude image, stop its callbacks and release its blitters before
// the next effect takes over.
static inline void CastingImpact_EndPrelude(struct BattleEffectWork *work,
                                            void *canvas) {
  Iwram_ClearWords(BG_CHAR_BLOCK(1), 0x4000);
  Iwram_ClearWords(canvas, 0x4000);
  work->effect->prelude = 0;
  Scheduler_RemoveCallback(Palette_StepFadeTransfer);
  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  EFFECT_RELEASE_BLITTERS();
}

// The casting impacts, by kind - 100: each is named for the first ability
// whose gBattle_ActionFlags entry selects it; the four no ability selects
// keep their numbers.
//
// The trailing comments give each impact's own animation, drawn over the
// target after the orb and sparks. Most animations use the 64 particles at
// gMapCellBuffer + 0x4000, which start at the target; their pictures are in
// gMapCellBuffer. "Smoke" is puffs of smoke rising from the target; "hands
// off" impacts stop after gathering and hand the canvas to another effect.
enum CastingImpact {
  CASTING_IMPACT_BANE = 0,       // smoke
  CASTING_IMPACT_SMOG,           // smoke
  CASTING_IMPACT_MIST,           // smoke
  CASTING_IMPACT_FEVER,          // smoke
  CASTING_IMPACT_FLINT,          // nothing
  CASTING_IMPACT_SCORCH,         // flash bursts rise from the target
  CASTING_IMPACT_SQUALL,         // two bolts strike among glints
  CASTING_IMPACT_SAP,            // motes fly back to the caster
  CASTING_IMPACT_TERRA_STRIKE,   // a rock spire grows and sinks
  CASTING_IMPACT_POISON_CLOUD,   // smoke
  CASTING_IMPACT_DEADLY_POISON,  // smoke
  CASTING_IMPACT_MORTAL_DANGER,  // also Death Size; a large picture rises and
                                 // breaks
  CASTING_IMPACT_BAD_OMEN,       // skulls rise from the target
  CASTING_IMPACT_LIFE_NOURISH,   // motes fly back to the caster
  CASTING_IMPACT_AQUA_SOCK,      // a blue flame column falls onto the target
  CASTING_IMPACT_15,             // hands off
  CASTING_IMPACT_FROST_BITE,     // ice chips converge on the target
  CASTING_IMPACT_17,             // hands off
  CASTING_IMPACT_LIFE_LEECH,     // motes fly back to the caster
  CASTING_IMPACT_PSY_LEECH,      // motes fly back to the caster
  CASTING_IMPACT_BROIL,          // ember streaks spread sideways
  CASTING_IMPACT_MELTDOWN,       // nothing; hands to the stat reduction
  CASTING_IMPACT_HEAT_MIRAGE,    // also Heat Flash; smoke
  CASTING_IMPACT_BARRAGE,        // flash bursts rise from the target
  CASTING_IMPACT_24,             // hands off
  CASTING_IMPACT_ACID_BATH,      // smoke
  CASTING_IMPACT_26,             // hands off
  CASTING_IMPACT_STUN_VOLTAGE,   // two bolts strike among glints
  CASTING_IMPACT_BLINDING_SMOG,  // smoke
  CASTING_IMPACT_MURK,           // smoke
  CASTING_IMPACT_CYCLONE_SLASH,  // a tornado stands over the target
  CASTING_IMPACT_PSYPHON_SEAL,   // a rune opens over the target
  CASTING_IMPACT_ASURA,          // a beast scrolls across and breaks
  CASTING_IMPACT_FORCIBLE_ARM,   // a flame falls diagonally
  CASTING_IMPACT_TRUNCHEON_FIST, // a flame falls diagonally
  CASTING_IMPACT_SEVERE_BLOW,    // hands off
};

#define CASTING_ORB_CELLS 0x5100    // the impact orb, packed at half width
#define CASTING_DJINNI_CELLS 0x3c56 // the Djinni pictures, in gMapCellBuffer
#define CASTING_GATHER_FRAMES 32    // frames the sparks gather
#define CASTING_SPARKS 64           // sparks in work->particles
// The impact pictures' particles, in gMapCellBuffer after the Djinni.
#define CASTING_IMPACT_PARTICLES                                               \
  ((struct EffectStep *)(Ram_MapCellBuffer + 0x4000))
#define CASTING_IMPACT_PARTICLE_COUNT 64

// gCastingImpact_ImageX has seven pictures for each side.
#define CASTING_IMAGE_X(side, image) gCastingImpact_ImageX[(side) * 7 + (image)]

// Djinn abilities
// Gather sparks around the acting unit, launch the Djinni and draw its impact.
// Kinds 15, 17, 24, 26 and 35 hand off to another effect after gathering.
// command: effect arguments
// kind: ability (effect number minus 100)
void BattleFx_RunCastingImpact(struct BattleEffectArgument *command, s32 kind) {
  void **slots;
  void **slot;
  s32 alive;
  s32 cellOffset;
  struct EffectPosition casterPos;
  struct EffectPosition targetPos;
  struct FixedPointPosition djinnPos;
  s32 phase;
  s32 i;
  s32 *scanline;
  s32 n;
  s32 djinnX;
  DrawRectangle *slotDraw;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  struct BattleCamera *camera;
  u8 *sprites;
  s32 scrollPos;
  s32 scrollSpeed;
  s32 duration;
  struct EffectPosition *targetScreen;
  struct EffectPosition *casterScreen;
  DrawRectangle *drawPair;
  s32 savedVelocityX;
  s32 savedVelocityY;
  s32 savedVelocityZ;
  s32 savedAcceleration;
  s32 savedVerticalStrength;
  struct MotionObject *targetActor;
  struct FixedPointPosition *motion;
  s32 kindFromTwo;
  s32 kindFromFour;
  struct FixedPointPosition *position;
  struct MotionObject *casterActor;
  struct EffectStep *particle;
  s32 pairX;
  s32 stripX;
  struct FixedPointPosition velocity;
  struct EffectPosition sparkScreen;
  BlitterPair blitters;
  struct EffectPosition projected;

  slots = gWorkSlot + HEAP_SLOT_BATTLE_EFFECT;
  slot = slots;
  work = (struct BattleEffectWork *)*slot++;
  canvas = *slot;
  camera = *(gWorkSlot + HEAP_SLOT_CAMERA);
  sprites = (u8 *)EFFECT_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  work->effect = command;
  if (kind == CASTING_IMPACT_MORTAL_DANGER ||
      kind == CASTING_IMPACT_TERRA_STRIKE || kind == CASTING_IMPACT_ASURA) {
    BattleFx_BeginTiledCanvas(0);
  } else {
    BattleFx_BeginCanvasLayer(0);
  }
  REG_BLDALPHA = BLEND_ALPHA(16, 16);

  //------------------------------------------------------------------
  // Graphics
  //   work->sheet: six 48x72 frames over the acting unit
  //   work->sheet + 0x5100: 20x48 impact orb, packed at half width
  //   gMapCellBuffer: ability-specific impact graphics
  //   gMapCellBuffer + 0x3c56: 20x40 Djinni graphics for each element
  //------------------------------------------------------------------
  CastingImpact_LoadResource(&ResourceId_ParticleSpritesA, sprites, 0, 0);
  CastingImpact_LoadResource(&ResourceId_FireStreakSheet, work, 1, 0);
  CastingImpact_LoadResource(&ResourceId_YellowOrbSheet, Ram_MapCellBuffer, 1,
                             0);
  /* FAKEMATCH: needed to match: local dimensions preserve the packing argument
   * order 2026-10-11: literal 40 and 288 differ 4 (movs r2, #40 and the buffer
   * load move ahead of lsls r3). */
  {
    s32 width = 40;
    s32 height = 288;
    CastingImpact_PackRows(Ram_MapCellBuffer, work->sheet + CASTING_ORB_CELLS,
                           width, height);
  }
  if (kind == CASTING_IMPACT_SCORCH || kind == CASTING_IMPACT_BARRAGE) {
    CastingImpact_LoadResource(&ResourceId_FlashBurstSheet, Ram_MapCellBuffer,
                               1, 0);
  } else if (kind == CASTING_IMPACT_BAD_OMEN) {
    CastingImpact_LoadResource(&ResourceId_SkullSheet, Ram_MapCellBuffer, 1, 0);
  } else if (kind == CASTING_IMPACT_SQUALL ||
             kind == CASTING_IMPACT_STUN_VOLTAGE) {
    CastingImpact_LoadResource(&ResourceId_TornadoSheet, Ram_MapCellBuffer, 1,
                               0);
    CastingImpact_LoadResource(&ResourceId_LightningBoltSheet,
                               Ram_MapCellBuffer + 0xc56, 1, 0);
  } else if (kind == CASTING_IMPACT_PSYPHON_SEAL) {
    CastingImpact_LoadResource(&ResourceId_RuneSheet, Ram_MapCellBuffer, 1, 1);
  } else if (kind == CASTING_IMPACT_TERRA_STRIKE) {
    CastingImpact_LoadResource(&ResourceId_RockSpireSheet, Ram_MapCellBuffer, 1,
                               1);
  } else if (kind == CASTING_IMPACT_AQUA_SOCK) {
    CastingImpact_LoadResource(&ResourceId_BlueFlameSheet, Ram_MapCellBuffer, 1,
                               0);
  } else if (kind == CASTING_IMPACT_CYCLONE_SLASH) {
    CastingImpact_LoadResource(&ResourceId_TornadoSheet, Ram_MapCellBuffer, 1,
                               0);
  } else if (kind == CASTING_IMPACT_FROST_BITE) {
    CastingImpact_LoadResource(&ResourceId_IceChipSheet, Ram_MapCellBuffer, 1,
                               0);
  } else if (kind == CASTING_IMPACT_BROIL) {
    CastingImpact_LoadResource(&ResourceId_EmberStreakSheet, Ram_MapCellBuffer,
                               1, 0);
  } else if (kind == CASTING_IMPACT_FORCIBLE_ARM ||
             kind == CASTING_IMPACT_TRUNCHEON_FIST) {
    CastingImpact_LoadResource(&ResourceId_FlameSheetA, Ram_MapCellBuffer, 1,
                               0);
  } else if (kind != CASTING_IMPACT_MORTAL_DANGER &&
             kind != CASTING_IMPACT_ASURA) {
    CastingImpact_LoadResource(&ResourceId_SmokeSheet, Ram_MapCellBuffer, 1, 0);
  }
  switch (kind) // Djinni element
  {
  case CASTING_IMPACT_BANE: // Earth
  case CASTING_IMPACT_FLINT:
  case CASTING_IMPACT_SAP:
  case CASTING_IMPACT_TERRA_STRIKE:
  case CASTING_IMPACT_POISON_CLOUD:
  case CASTING_IMPACT_DEADLY_POISON:
  case CASTING_IMPACT_MORTAL_DANGER:
  case CASTING_IMPACT_BAD_OMEN:
  case CASTING_IMPACT_LIFE_NOURISH:
  case CASTING_IMPACT_FORCIBLE_ARM:
    CastingImpact_LoadResource(&ResourceId_VenusDjinnSmallSheet,
                               Ram_MapCellBuffer + CASTING_DJINNI_CELLS, 1, 1);
    break;
  case CASTING_IMPACT_MIST: // Water
  case CASTING_IMPACT_AQUA_SOCK:
  case CASTING_IMPACT_15:
  case CASTING_IMPACT_FROST_BITE:
  case CASTING_IMPACT_17:
  case CASTING_IMPACT_LIFE_LEECH:
  case CASTING_IMPACT_PSY_LEECH:
    CastingImpact_LoadResource(&ResourceId_MercuryDjinnSmallSheet,
                               Ram_MapCellBuffer + CASTING_DJINNI_CELLS, 1, 1);
    break;
  case CASTING_IMPACT_FEVER: // Fire
  case CASTING_IMPACT_SCORCH:
  case CASTING_IMPACT_BROIL:
  case CASTING_IMPACT_MELTDOWN:
  case CASTING_IMPACT_HEAT_MIRAGE:
  case CASTING_IMPACT_BARRAGE:
  case CASTING_IMPACT_24:
  case CASTING_IMPACT_ACID_BATH:
  case CASTING_IMPACT_TRUNCHEON_FIST:
  case CASTING_IMPACT_SEVERE_BLOW:
    CastingImpact_LoadResource(&ResourceId_MarsDjinnSmallSheet,
                               Ram_MapCellBuffer + CASTING_DJINNI_CELLS, 1, 1);
    break;
  case CASTING_IMPACT_SMOG: // Wind
  case CASTING_IMPACT_SQUALL:
  case CASTING_IMPACT_26:
  case CASTING_IMPACT_STUN_VOLTAGE:
  case CASTING_IMPACT_BLINDING_SMOG:
  case CASTING_IMPACT_MURK:
  case CASTING_IMPACT_CYCLONE_SLASH:
  case CASTING_IMPACT_PSYPHON_SEAL:
  case CASTING_IMPACT_ASURA:
    CastingImpact_LoadResource(&ResourceId_JupiterDjinnSmallSheet,
                               Ram_MapCellBuffer + CASTING_DJINNI_CELLS, 1, 1);
    break;
  case 100:
    CastingImpact_LoadResource(&ResourceId_MercuryDjinnSmallSheet,
                               Ram_MapCellBuffer + CASTING_DJINNI_CELLS, 1, 1);
    break;
  }
  work->transferMode =
      CANVAS_COPY_AND_FADE; // Afterimage: 3/4 for kind 12, otherwise 1/2.
  if (kind == CASTING_IMPACT_BAD_OMEN) {
    work->transferValue = 75;
  } else {
    work->transferValue = 50;
  }
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);
  /* FAKEMATCH: needed to match: retain each output pointer while preparing its
   * call 2026-10-11: the pointers set in statements before the calls differ 38,
   * after the calls 142. */
  EffectPosition_ApplyStepAndYOffset(work->effect->actors[0],
                                     targetScreen = &targetPos);
  EffectPosition_ApplyStepAndYOffset(work->effect->actor,
                                     casterScreen = &casterPos);
  BattleFx_FetchRectangleBlitters(work->effect->side, drawPair = blitters);
  work->fadeFrames = 24;
  work->fadeStep = 0;
  casterActor = GetBattleObjectSlotFar(work->effect->actor)->object;

  // Gathering sparks: x is radius (32-95), velocity is the rotation angle.
  {
    for (i = 0; i != CASTING_SPARKS; i++) {
      particle = &work->particles[i];
      EFFECT_SET_POSITION(particle, (Random16() & 63) + 32, 0, 0);
      EFFECT_SET_VELOCITY(particle, Random16() & 0xffff, Random16() & 0xffff,
                          Random16() & 0xffff);
    }
  }
  // Stop the acting unit and place the Djinni 80 pixels overhead.
  ObjectDispatch_ApplyValueToChildrenFar(casterActor, 0);
  position = &djinnPos;
  EFFECT_SET_POSITION(position, casterActor->x, casterActor->y + 0x500000,
                      casterActor->z);
  savedVelocityX = casterActor->velocityX;
  savedVelocityY = casterActor->velocityY;
  savedVelocityZ = casterActor->velocityZ;
  savedAcceleration = casterActor->acceleration;
  savedVerticalStrength = casterActor->verticalMotionStrength;
  EFFECT_SET_VELOCITY(casterActor, 0, 0, 0);
  casterActor->acceleration = 0;
  casterActor->verticalMotionStrength = 0;
  EffectPosition_ApplyStepAndYOffset(work->effect->actor, casterScreen);
  casterScreen->x = casterScreen->x / 2;
  AudioCommand_PlayFar(212);

  //------------------------------------------------------------------
  // Gather for 32 frames.
  // Start four sparks each frame; shrink their radii by 4 toward the acting
  // unit. Kinds 0-7 reveal the Djinni as the sparks begin to disappear.
  //------------------------------------------------------------------
  frame = 0;
  do {
    n = 0;
    for (i = 0; i != CASTING_SPARKS; i++) {
      struct EffectStep *step = &work->particles[i];
      if (step->x >= 0) {
        if (frame >= i / 4) {
          {
            s32 size = 5;
            Render_ResetTransformState();
            SceneTransform_ApplyRoll(step->velocityZ);
            SceneTransform_ApplyPitch(step->velocityX);
            SceneTransform_ApplyYaw(step->velocityY);
            EffectPosition_ApplyBaseAndYOffset(step, &sparkScreen);
            sparkScreen.x = sparkScreen.x / 2 + casterScreen->x;
            if (kind <= 7)
              sparkScreen.y = sparkScreen.y + casterScreen->y - 8;
            else if (kind == CASTING_IMPACT_SEVERE_BLOW)
              sparkScreen.y = sparkScreen.y + casterScreen->y + 44;
            else
              sparkScreen.y = sparkScreen.y + casterScreen->y + 12;
            EFFECT_CLAMP(sparkScreen.depth, -60, 60);
            sparkScreen.depth += 60; // 0〜120
            EFFECT_DRAW_MOTE(blitters[1], canvas, sprites, sparkScreen.x,
                             sparkScreen.y, size);
          }
          step->x -= 4;
        }
        n++;
      }
    }
    if (kind <= 7) {
      alive = n;
      if (alive <= 63) {
        EFFECT_VIEW_FROM_CAMERA(camera);
        EffectPosition_ApplyBaseAndYOffset(&djinnPos, &sparkScreen);
        sparkScreen.x /= 2;
        djinnX = sparkScreen.x;
        blitters[0](canvas, Ram_MapCellBuffer + CASTING_DJINNI_CELLS,
                    djinnX - 10, sparkScreen.y - 4, 20, 40);
      }
    }
    EFFECT_PRESENT_FRAME(work);
    frame++;
  } while (frame != CASTING_GATHER_FRAMES);
  /* FAKEMATCH: needed to match: restart the pointer's lifetime after gathering
   * so the projection above uses the stack position directly 2026-10-11: direct
   * djinnPos stores above, with this as the only assignment, differ 114, as
   * does removing it. */
  position = &djinnPos;

  //------------------------------------------------------------------
  // Prepare BG2 for each ability: normal horizontal scale, impact at the
  // target.
  //------------------------------------------------------------------
  if (kind == CASTING_IMPACT_MORTAL_DANGER) {
    REG_BG2PA = Q_8_8(1);
    if (work->effect->side == 0) {
      REG_BG2X = (frame - targetScreen->x) << 8; // frame is 32 here.
    } else {
      REG_BG2X = (96 - targetScreen->x) << 8;
    }
  } else {
    if (kind == CASTING_IMPACT_ASURA) {
      // Scroll the image horizontally; scrollPos is in 16.16 pixels.
      REG_BG2PA = Q_8_8(1);
      if (work->effect->side == 0) {
        scrollPos = -0x800000;
        scrollSpeed = 0xc0000;
      } else {
        scrollPos = 0x80000;
        scrollSpeed = -0xc0000;
      }
      REG_BG2X = (scrollPos >> 16) << 8;
    }
  }
  if (kind == CASTING_IMPACT_TERRA_STRIKE) {
    REG_BG2PA = Q_8_8(1);
    REG_BG2X = (64 - targetScreen->x) << 8;
    work->transferMode = CANVAS_COPY_AND_CLEAR; // Clear to 0 each frame.
    work->transferValue = 0;
    Iwram_ClearWords(BG_CHAR_BLOCK(1), 0x4000);
    Iwram_ClearWords(canvas, 0x4000);
    REG_BLDCNT = 0;
  }
  if (kind == CASTING_IMPACT_PSYPHON_SEAL) {
    REG_BG2PA = Q_8_8(1);
    if (work->effect->side == 0) {
      REG_BG2X = (32 - targetScreen->x) << 8;
    } else {
      REG_BG2X = (96 - targetScreen->x) << 8;
    }
  }
  // Kinds 15, 17, 24 and 26 clear the canvas and hand off to another effect.
  if (kind == CASTING_IMPACT_15 || kind == CASTING_IMPACT_17 ||
      kind == CASTING_IMPACT_24 || kind == CASTING_IMPACT_26) {
    CastingImpact_EndPrelude(work, canvas);
    ObjectDispatch_ApplyArgumentToChildrenFar(casterActor, 3);
    if (kind == CASTING_IMPACT_15) {
      BattleFx_RunProjectileVolley(command, 9);
    }
    if (kind == CASTING_IMPACT_24) {
      BattleFx_RunMysticFlame(command);
    }
    if (kind != CASTING_IMPACT_26) {
      return;
    }
    BattleFx_RunProjectileVolley(command, 8);
    return;
  }
  ObjectDispatch_ApplyValueToChildrenFar(casterActor, 16);
  EFFECT_SET_VELOCITY(casterActor, savedVelocityX, savedVelocityY,
                      savedVelocityZ);
  casterActor->acceleration = savedAcceleration;
  casterActor->verticalMotionStrength = savedVerticalStrength;
  // Kind 35 clears the canvas and hands off to BattleFx_RunSparkGroups.
  if (kind == CASTING_IMPACT_SEVERE_BLOW) {
    CastingImpact_EndPrelude(work, canvas);
    command->variant = 3;
    BattleFx_RunSparkGroups(command, 2);
    return;
  }

  // Djinni step: reach 30 pixels above the target in six frames.
  targetActor = GetBattleObjectSlotFar(work->effect->actors[0])->object;
  motion = &velocity;
  EFFECT_SET_POSITION(motion, (targetActor->x - position->x) / 6,
                      (targetActor->y - position->y + 0x1e0000) / 6,
                      (targetActor->z - position->z) / 6);
  for (i = 0; i != CASTING_SPARKS; i++) {
    particle = &work->particles[i];
    particle->variant = 0;
  }
  // Impact particles scatter from half the target height; variant is lifetime.
  if (kind != CASTING_IMPACT_AQUA_SOCK) {
    s32 height = Battle_GetObjectTableValueFar(work->effect->actors[0]) / 2;
    for (i = 0; i != 32; i++) {
      particle = &work->particles[i];
      EFFECT_SET_POSITION(particle, targetActor->x, height, targetActor->z);
      if (kind == CASTING_IMPACT_PSYPHON_SEAL) {
        EFFECT_SET_VELOCITY(particle, ((Random16() & 255) - 127) << 12,
                            ((Random16() & 255) - 64) << 10,
                            ((Random16() & 255) - 127) << 12);
      } else {
        EFFECT_SET_VELOCITY(particle, ((Random16() & 255) - 127) << 12,
                            ((Random16() & 255) - 64) << 12,
                            ((Random16() & 255) - 127) << 12);
      }
      particle->variant = i / 2 + 32;
    }
  }
  // Kinds 11 and 32 replace the acting-unit overlay and impact graphics.
  if (kind == CASTING_IMPACT_MORTAL_DANGER) {
    CastingImpact_LoadResource(&ResourceId_CounterRevealSheet, work, 1, 1);
    CastingImpact_LoadResource(&ResourceId_RedCrescentSheetA, Ram_MapCellBuffer,
                               1, 0);
    REG_BLDALPHA = BLEND_ALPHA(16, 14);
  }
  if (kind == CASTING_IMPACT_ASURA) {
    CastingImpact_LoadResource(&ResourceId_BlueBeastSheet, work->imageStorage,
                               1, 1);
    CastingImpact_LoadResource(&ResourceId_RedCrescentSheetB, Ram_MapCellBuffer,
                               1, 0);
    REG_BLDALPHA = BLEND_ALPHA(16, 14);
  }
  // 64 impact-image particles at gMapCellBuffer + 0x4000.
  if (kind != CASTING_IMPACT_SAP && kind != CASTING_IMPACT_LIFE_NOURISH &&
      kind != CASTING_IMPACT_LIFE_LEECH &&
      kind != CASTING_IMPACT_MORTAL_DANGER && kind != CASTING_IMPACT_ASURA &&
      kind != CASTING_IMPACT_PSY_LEECH) {
    s32 height = 0;
    if (kind != CASTING_IMPACT_BAD_OMEN)
      height = 0x140000;
    for (i = 0; i != CASTING_IMPACT_PARTICLE_COUNT; i++) {
      particle = &CASTING_IMPACT_PARTICLES[i];
      EFFECT_SET_POSITION(particle, targetActor->x, height, targetActor->z);
      if (kind == CASTING_IMPACT_SCORCH || kind == CASTING_IMPACT_BARRAGE) {
        EFFECT_SET_VELOCITY(particle, ((Random16() & 255) - 127) << 11,
                            (Random16() & 255) << 11,
                            ((Random16() & 255) - 127) << 11);
      } else if (kind == CASTING_IMPACT_ACID_BATH) {
        EFFECT_SET_VELOCITY(particle, ((Random16() & 255) - 127) << 11,
                            (Random16() & 127) << 10,
                            ((Random16() & 255) - 127) << 11);
      } else {
        EFFECT_SET_VELOCITY(particle, ((Random16() & 255) - 127) << 10,
                            (Random16() & 127) << 10,
                            ((Random16() & 255) - 127) << 10);
      }
      particle->variant = 0;
    }
  }
  // Kinds 2, 3, 12, 22, 28 and 29 wave BG2 by scanline.
  kindFromTwo = kind - 2;
  if ((kindFromTwo >= 0 && kindFromTwo <= 1) ||
      kind == CASTING_IMPACT_BAD_OMEN || kind == CASTING_IMPACT_HEAT_MIRAGE ||
      kind == CASTING_IMPACT_MURK || kind == CASTING_IMPACT_BLINDING_SMOG) {
    Scheduler_AddOrUpdateCallback(BattleFx_ArmBg2AffineHBlankDma, 0x480);
  }

  // Duration in frames
  kindFromFour = kind - 4;
  if ((kindFromFour >= 0 && kindFromFour <= 2) ||
      kind == CASTING_IMPACT_BARRAGE || kind == CASTING_IMPACT_CYCLONE_SLASH ||
      kind == CASTING_IMPACT_STUN_VOLTAGE ||
      kind == CASTING_IMPACT_FORCIBLE_ARM ||
      kind == CASTING_IMPACT_TRUNCHEON_FIST || kind == 100) {
    duration = 32;
  } else if (kind >= 0 && kind <= 3 || kind == CASTING_IMPACT_TERRA_STRIKE ||
             kind == CASTING_IMPACT_POISON_CLOUD ||
             kind == CASTING_IMPACT_DEADLY_POISON ||
             kind == CASTING_IMPACT_HEAT_MIRAGE ||
             kind == CASTING_IMPACT_ACID_BATH || kind == CASTING_IMPACT_MURK ||
             kind == CASTING_IMPACT_PSYPHON_SEAL ||
             kind == CASTING_IMPACT_AQUA_SOCK) {
    duration = 48;
  } else if (kind == CASTING_IMPACT_MELTDOWN) {
    duration = 20;
  } else if (kind == CASTING_IMPACT_MORTAL_DANGER ||
             kind == CASTING_IMPACT_ASURA || kind == CASTING_IMPACT_BROIL) {
    duration = 40;
  } else if (kind == CASTING_IMPACT_BLINDING_SMOG ||
             kind == CASTING_IMPACT_BAD_OMEN) {
    duration = 64;
  } else {
    duration = 80;
  }
  //------------------------------------------------------------------
  // Impact
  //------------------------------------------------------------------
  frame = 0;
  while (frame != duration) {
    // BG2 wave table: BG2X for each scanline.
    if (kind != CASTING_IMPACT_MORTAL_DANGER) {
      if (kind != CASTING_IMPACT_ASURA) {
        s32 base;
        scanline = work->bg2X;
        i = 0;
        base = 0x40000;
        phase = frame << 12;
        for (; i != 160; i++) {
          s32 offset = (base - ((Trig_Sin(phase) * 4))) >>
                       10; // 1 - sin pixels, in 8.8 format
          *scanline++ = offset;
          phase += 0x800;
        }
      }
    }
    // Refresh the acting unit position for the first three frames, 16 pixels
    // below.
    if (frame <= 2) {
      EffectPosition_ApplyStepAndYOffset(work->effect->actor, casterScreen);
      casterScreen->x = casterScreen->x / 2;
      casterScreen->y += 16;
    }
    // Acting-unit overlay: six frames, two ticks each.
    if (kind != CASTING_IMPACT_MORTAL_DANGER &&
        kind != CASTING_IMPACT_TERRA_STRIKE && kind != CASTING_IMPACT_ASURA &&
        kind != CASTING_IMPACT_FORCIBLE_ARM &&
        kind != CASTING_IMPACT_TRUNCHEON_FIST) {
      if (frame <= 11) {
        if (work->effect->side == 0) {
          blitters[0](canvas, work->sheet + frame / 2 * 3456,
                      casterScreen->x - 32, casterScreen->y - 40, 48, 72);
        } else {
          blitters[0](canvas, work->sheet + frame / 2 * 3456, casterScreen->x,
                      casterScreen->y - 40, 48, 72);
        }
      }
    }
    // Move the BG palette one step toward the ability's resource palette.
    switch (kind) {
    case CASTING_IMPACT_FORCIBLE_ARM:
      BattleFx_StepPaletteToResource((s32)&ResourceId_FlameSheetA);
      break;
    case CASTING_IMPACT_AQUA_SOCK:
      BattleFx_StepPaletteToResource((s32)&ResourceId_BlueFlameSheet);
      break;
    case CASTING_IMPACT_PSYPHON_SEAL:
      BattleFx_StepPaletteToResource((s32)&ResourceId_RuneSheet);
      break;
    case CASTING_IMPACT_TERRA_STRIKE:
      BattleFx_StepPaletteToResource((s32)&ResourceId_RockSpireSheet);
      break;
    case CASTING_IMPACT_BANE:
    case CASTING_IMPACT_DEADLY_POISON:
      BattleFx_StepPaletteToResource((s32)&ResourceId_MarsDjinnSheet);
      break;
    case CASTING_IMPACT_BAD_OMEN:
    case CASTING_IMPACT_LIFE_NOURISH:
    case CASTING_IMPACT_ACID_BATH:
      BattleFx_StepPaletteToResource((s32)&ResourceId_PinkBurstSheet);
      break;
    case CASTING_IMPACT_LIFE_LEECH:
      BattleFx_StepPaletteToResource((s32)&ResourceId_IceBlockSheet);
      break;
    case CASTING_IMPACT_PSY_LEECH:
      BattleFx_StepPaletteToResource((s32)&ResourceId_BlastSheet);
      break;
    case CASTING_IMPACT_MIST:
    case CASTING_IMPACT_MURK:
      BattleFx_StepPaletteToResource((s32)&ResourceId_TanPalette);
      break;
    case CASTING_IMPACT_SMOG:
    case CASTING_IMPACT_BLINDING_SMOG:
      BattleFx_StepPaletteToResource((s32)&ResourceId_CyanPalette);
      break;
    case CASTING_IMPACT_FEVER:
    case CASTING_IMPACT_BROIL:
    case CASTING_IMPACT_HEAT_MIRAGE:
      BattleFx_StepPaletteToResource((s32)&ResourceId_EmberStreakSheet);
      break;
    case CASTING_IMPACT_POISON_CLOUD:
      BattleFx_StepPaletteToResource((s32)&ResourceId_LimePalette);
      break;
    case CASTING_IMPACT_SCORCH:
    case CASTING_IMPACT_BARRAGE:
      BattleFx_StepPaletteToResource((s32)&ResourceId_FlashBurstSheet);
      break;
    }
    // Impact orb on frames 4-15; impact particles from frame 4.
    if (kind != CASTING_IMPACT_MORTAL_DANGER &&
        kind != CASTING_IMPACT_TERRA_STRIKE && kind != CASTING_IMPACT_ASURA) {
      if (frame >= 4 && frame <= 15) {
        drawPair[1](canvas,
                    work->sheet + (frame - 4) / 2 * 960 + CASTING_ORB_CELLS,
                    targetScreen->x / 2 - 8, casterScreen->y - 24, 20, 48);
      }
      EFFECT_VIEW_FROM_CAMERA(camera);
      if (frame > 3) {
        for (i = 0; i != 128; i++) {
          s32 index = i / 2;
          struct EffectStep *step = &work->particles[index];
          s32 size = step->variant;
          if (size > 0) {
            EffectPosition_ApplyBaseAndYOffset(step, &projected);
            size >>= 4;
            size++;
            projected.x /= 2;
            EFFECT_DRAW_MOTE(blitters[index & 1], canvas, sprites, projected.x,
                             projected.y, size);
            EffectStep_AdvanceWithGravity3D(step, 60, -0x1000);
            step->variant--;
          }
        }
      }
    }
    //------------------------------------------------------------------
    // Ability-specific impact
    //------------------------------------------------------------------
    if (kind == CASTING_IMPACT_SAP || kind == CASTING_IMPACT_LIFE_NOURISH ||
        kind == CASTING_IMPACT_LIFE_LEECH || kind == CASTING_IMPACT_PSY_LEECH) {
      // From frame 12, draw target particles toward the acting unit.
      if (frame == 50) {
        ObjectGroup_UpdateMembers(work->effect->actor, SPRITE_COLOR_WHITE, -1,
                                  -1, 0);
      }
      if (frame == 79) {
        ObjectGroup_UpdateMembers(work->effect->actor, 0, -1, -1, 0);
      }
      if (frame == 12) {
        particle = CASTING_IMPACT_PARTICLES;
        for (i = 0; i != CASTING_IMPACT_PARTICLE_COUNT; i++, particle++) {
          EFFECT_SET_POSITION(particle, targetActor->x, 0x140000,
                              targetActor->z);
          EFFECT_SET_VELOCITY(particle, ((Random16() & 255) - 128) << 10,
                              ((Random16() & 255) - 128) << 10,
                              ((Random16() & 255) - 128) << 10);
          particle->variant = 0;
        }
      }
      if (frame > 11) {
        struct EffectStep *step = CASTING_IMPACT_PARTICLES;
        struct MotionObject *caster;
        s32 height;
        caster = GetBattleObjectSlotFar(work->effect->actor)->object;
        height = Battle_GetObjectTableValueFar(work->effect->actor) / 2;
        for (i = 0; i != 32; i++, step++) {
          if (step->variant >= 0) {
            s32 size = (i & 1) + 6;
            EffectPosition_ApplyBaseAndYOffset(step, &projected);
            projected.x >>= 1;
            blitters[0](canvas, sprites + gBattleFx_MoteCellOffsets[size - 1],
                        projected.x - (u32)size / 2, projected.y - size, size,
                        size * 2);
            EffectStep_AdvanceWithGravity3D(step, 62, 0);
            if (frame > i + 22) {
              s32 dx = (caster->x - step->x) >> 8;
              s32 dy = (caster->y + height - step->y) >> 8;
              s32 dz = (caster->z - step->z) >> 8;
              step->velocityX += dx;
              step->velocityY += dy;
              step->velocityZ += dz;
              if ((dx >= -0xfff && dx <= 0xfff) &&
                  (dz >= -0xfff && dz <= 0xfff))
                step->variant = -1;
            }
          }
        }
      }
    } else if (kind == CASTING_IMPACT_MELTDOWN) {
      // Draw nothing; finish with BattleFx_RunImpair.
    } else if (kind == CASTING_IMPACT_SQUALL ||
               kind == CASTING_IMPACT_STUN_VOLTAGE) {
      // Two tall images (four 24x104 frames) and fragments around the target.
      if (frame >= 6 && frame <= 19) {
        phase = frame;
        i = 0;
        do {
          blitters[0](canvas,
                      ((phase / 2) & 3) * 2880 + (Ram_MapCellBuffer + 0xc56),
                      targetScreen->x / 2 - 8, 0, 24, 104);
          i++;
          phase += 3;
        } while (i != 2);
      }
      if (frame >= 8 && frame <= 23) {
        for (i = 0; i != 3; i++) {
          s32 image = i & 3;
          s32 angle = Random16() & 0xffff;
          s32 imageX;
          s32 imageY;
          s32 drawFlags;
          imageX = Trig_Sin(angle);
          imageX <<= 3;
          imageX >>= 16;
          imageX += targetScreen->x / 2;
          imageX -= gBattleFx_GlintCellWidths[image] / 2;
          imageY = Trig_Cos(angle);
          imageY <<= 5;
          imageY >>= 16;
          imageY -= gBattleFx_GlintCellHeights[image] / 2;
          EFFECT_RELEASE_BLITTERS();
          drawFlags = Random16(); // Random flips
          EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                              gCastingImpact_GlintDrawFlags[drawFlags & 3] |
                                  COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                              COMPOSITE_MAX);
          ((DrawRectangle *)gWorkSlot)[HEAP_SLOT_BLITTER_ALTERNATE](
              canvas, Ram_MapCellBuffer + gBattleFx_GlintCellOffsets[image],
              imageX, imageY + 56, gBattleFx_GlintCellWidths[image],
              gBattleFx_GlintCellHeights[image]);
          Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
          BattleFx_FetchRectangleBlitters(work->effect->side, blitters);
        }
      }
    } else if (kind == CASTING_IMPACT_AQUA_SOCK) {
      // Descending column (17-pixel image scrolling down) and particles from
      // the target.
      s32 fall;
      s32 scroll;
      EFFECT_RELEASE_BLITTERS();
      if (frame >= 0 && frame <= 23) {
        DrawRectangle *draw;
        s32 width;
        s32 tileHeight;
        s32 columnX;
        s32 columnY;
        s32 top;
        s32 originX;

        originX = targetScreen->x / 2;
        fall = frame * 32 - 232;
        scroll = frame * 16 - 48;
        if (fall > 0)
          fall = 0;
        while (scroll > 104)
          scroll -= 104;
        EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                            COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, COMPOSITE_MAX);
        draw = (DrawRectangle *)gWorkSlot;
        draw += HEAP_SLOT_BLITTER_ALTERNATE;
        columnX = originX - 8;
        columnY = fall + scroll;
        top = columnY - 104;
        width = 17;
        tileHeight = 104;
        CastingImpact_DrawImage(canvas, Ram_MapCellBuffer, columnX, top, width,
                                tileHeight, draw);
        CastingImpact_DrawScrolledImage(canvas, Ram_MapCellBuffer, columnX,
                                        columnY, width, tileHeight, scroll,
                                        draw);
        (*draw)(canvas, Ram_MapCellBuffer + 0x6e8, originX - width, fall + 47,
                width * 2, 65);
        Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
        if (frame == 8) {
          work->shakeFrames = frame;
        }
        if (frame > 1) {
          s32 emitted = 0;
          struct EffectStep *step;
          for (i = 0; i != CASTING_SPARKS; i++) {
            if ((step = &work->particles[i])->variant == 0) {
              EFFECT_SET_POSITION(step, targetActor->x, 0x140000,
                                  targetActor->z);
              EFFECT_SET_VELOCITY(step, ((Random16() & 255) - 127) << 12,
                                  ((Random16() & 255) - 64) << 10,
                                  ((Random16() & 255) - 127) << 12);
              step->variant = i / 2 + 32;
              emitted++;
              if (emitted == 4)
                break;
            }
          }
        }
      }
      BattleFx_FetchRectangleBlitters(work->effect->side, blitters);
    } else if (kind == CASTING_IMPACT_PSYPHON_SEAL) {
      // Mirrored 24x48 images
      s32 height;
      EFFECT_RELEASE_BLITTERS();
      if (frame >= 4 && frame <= 23) {
        pairX = targetScreen->x / 2;
        height = 48;
        EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                            COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, COMPOSITE_MAX);
        slotDraw = (DrawRectangle *)gWorkSlot;
        CastingImpact_DrawImage(canvas, Ram_MapCellBuffer, pairX - 24, 48, 24,
                                height, slotDraw + HEAP_SLOT_BLITTER_ALTERNATE);
        Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
        EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                            COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                                COMPOSITE_FLIP_X,
                            COMPOSITE_MAX);
        CastingImpact_DrawImage(canvas, Ram_MapCellBuffer, pairX, 48, 24,
                                height, slotDraw + HEAP_SLOT_BLITTER_ALTERNATE);
        Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
      }
      BattleFx_FetchRectangleBlitters(work->effect->side, blitters);
    } else if (kind == CASTING_IMPACT_CYCLONE_SLASH) {
      // Three 40x32 frames, two ticks each; fade from frame 16.
      s32 image;
      if (frame > 15) {
        REG_BLDALPHA = (32 - frame) | 0x1000;
      }
      if (frame > 5) {
        stripX = targetScreen->x / 2 - 20;
        image = frame / 2 % 3;
        cellOffset = image * 2560;
        blitters[0](canvas, Ram_MapCellBuffer + 0xc56 + cellOffset, stripX, 16,
                    40, 32);
        blitters[0](canvas, image * 1280 + (Ram_MapCellBuffer + 0x2a56), stripX,
                    48, 40, 32);
        blitters[0](canvas, cellOffset + (Ram_MapCellBuffer + 0x1156), stripX,
                    80, 40, 32);
      }
    } else if (kind == CASTING_IMPACT_SCORCH ||
               kind == CASTING_IMPACT_BARRAGE) {
      // Six 32x64 frames per particle, two ticks each.
      struct EffectStep *step = CASTING_IMPACT_PARTICLES;
      for (i = 0; i != 16; i++, step++) {
        if (frame >= i / 2 + 4) {
          s32 age = step->variant;
          if (age <= 11) {
            s32 image = age / 2;
            EffectPosition_ApplyBaseAndYOffset(step, &projected);
            projected.x /= 2;
            blitters[0](canvas, Ram_MapCellBuffer + (image << 11),
                        projected.x - 16, projected.y - 32, 32, 64);
            EffectStep_AdvanceWithGravity3D(step, 60, 0x1000);
            step->variant++;
          }
        }
      }
    } else if (kind == CASTING_IMPACT_FLINT) {
      // Draw nothing.
    } else if (kind == CASTING_IMPACT_MORTAL_DANGER) {
      // Large target overlay: seven frames, two ticks each, bobbing vertically.
      // Fill the canvas on frames 4 and 5.
      s32 imageY;
      s32 sway;
      Trig_Sin(frame << 9); // Result unused
      sway = Trig_Cos(frame << 9);
      sway <<= 2;
      sway >>= 16;
      imageY = (targetScreen->y >> 16) + sway + 16;
      if (frame <= 3) {
        blitters[0](canvas, work, CASTING_IMAGE_X(work->effect->side, 0),
                    imageY + gCastingImpact_ImageY[0], 57, 98);
      } else {
        if (frame <= 7) {
          blitters[0](canvas, work, CASTING_IMAGE_X(work->effect->side, 0),
                      imageY + gCastingImpact_ImageY[0], 57, 98);
        }
        blitters[0](canvas, work->sheet + 0x15d2,
                    CASTING_IMAGE_X(work->effect->side, 1),
                    imageY + gCastingImpact_ImageY[1], 99, 69);
        if (frame == 4 || frame == 5) {
          Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
        }
        if (frame == 6 || frame == 7) {
          blitters[0](canvas, work->sheet + 0x3081,
                      CASTING_IMAGE_X(work->effect->side, 2),
                      imageY + gCastingImpact_ImageY[2], 128, 91);
        }
        if (frame == 8 || frame == 9) {
          CastingImpact_DrawCroppedImage(
              canvas, Ram_MapCellBuffer, CASTING_IMAGE_X(work->effect->side, 3),
              imageY + gCastingImpact_ImageY[3], 128, &blitters[0]);
        }
        if (frame == 10 || frame == 11) {
          blitters[0](canvas, Ram_MapCellBuffer + 0x2d80,
                      CASTING_IMAGE_X(work->effect->side, 4),
                      imageY + gCastingImpact_ImageY[4], 128, 59);
        }
        if (frame == 12 || frame == 13) {
          blitters[0](canvas, Ram_MapCellBuffer + 0x4b00,
                      CASTING_IMAGE_X(work->effect->side, 5),
                      imageY + gCastingImpact_ImageY[5], 122, 29);
        }
        if (frame == 14 || frame == 15) {
          blitters[0](canvas, Ram_MapCellBuffer + 0x58d2,
                      CASTING_IMAGE_X(work->effect->side, 6),
                      imageY + gCastingImpact_ImageY[6], 76, 25);
        }
      }
    } else if (kind == CASTING_IMPACT_ASURA) {
      // Image that scrolls and stops, plus large images lasting two frames
      // each. Fade from frame 16.
      scrollPos += scrollSpeed;
      if (frame > 6) {
        scrollSpeed = (scrollSpeed * 48) / 64;
      }
      REG_BG2X = (scrollPos >> 16) << 8;
      {
        s32 fade;
        fade = frame - 16;
        if (fade >= 0 && fade <= 15) {
          REG_BLDALPHA = (16 - fade) | 0x1000;
        }
      }
      if (frame == 4 || frame == 5) {
        Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
      }
      if (frame <= 3) {
        if (work->effect->side == 1) {
          blitters[0](canvas, work->imageStorage, 0, 24, 80, 104);
        } else {
          blitters[0](canvas, work->imageStorage, 48, 24, 80, 104);
        }
      } else {
        if (frame <= 7) {
          if (work->effect->side == 1) {
            blitters[0](canvas, work->imageStorage, 0, 24, 80, 104);
          } else {
            blitters[0](canvas, work->imageStorage, 48, 24, 80, 104);
          }
        }
        if (work->effect->side == 1) {
          blitters[0](canvas, work->imageStorage + 0x1e00, 16, 16, 80, 104);
        } else {
          blitters[0](canvas, work->imageStorage + 0x1e00, 32, 16, 80, 104);
        }
        if (frame == 6 || frame == 7) {
          blitters[0](canvas, work->imageStorage + 0x3e80, 0, 16, 128, 91);
        }
        if (frame == 8 || frame == 9) {
          CastingImpact_DrawCroppedImage(canvas, Ram_MapCellBuffer, 0, 16, 128,
                                         &blitters[0]);
        }
        if (frame == 10 || frame == 11) {
          blitters[0](canvas, Ram_MapCellBuffer + 0x2d80, 0, 16, 128, 59);
        }
        if (frame == 12 || frame == 13) {
          blitters[0](canvas, Ram_MapCellBuffer + 0x4b00, 0, 16, 128, 29);
        }
        if (frame == 14 || frame == 15) {
          blitters[0](canvas, Ram_MapCellBuffer + 0x5980, 0, 16, 128, 26);
        }
      }
    } else if (kind == CASTING_IMPACT_BROIL) {
      // Twelve particles every other frame, spreading sideways over six
      // two-tick frames.
      for (i = 0; i != 12; i++) {
        if (frame >= i + 6 && frame < i + 18) {
          s32 image = (frame - i - 6) / 2;
          s32 imageX =
              targetScreen->x / 2 - gBattleFx_PuffArcCellWidths[image] / 2;
          s32 mirrored;
          if (i & 1)
            imageX += (1 + i) / 2 * 3;
          else
            imageX -= (i + 1) / 2 * 3;
          mirrored = 1;
          if (i != 0) {
            mirrored = 0;
            if (((i - 1) & 3) > 1)
              mirrored = 1;
          }
          blitters[mirrored](canvas,
                             Ram_MapCellBuffer +
                                 gBattleFx_PuffArcCellSourceOffsets[image],
                             imageX, gBattleFx_PuffArcCellBiasY[image] + 48,
                             gBattleFx_PuffArcCellWidths[image],
                             gBattleFx_PuffArcCellHeights[image]);
        }
      }
    } else if (kind == CASTING_IMPACT_FROST_BITE) {
      // 64 fragments converge in straight lines from around the target.
      // The target reacts whenever another group of eight arrives.
      if (frame == 0) {
        for (i = 0; i != CASTING_IMPACT_PARTICLE_COUNT; i++) {
          particle = &CASTING_IMPACT_PARTICLES[i];
          EFFECT_SET_POSITION(particle, (Random16() & 127) + 32, 0, 0);
          EFFECT_SET_VELOCITY(particle, Random16() & 0xffff,
                              Random16() & 0xffff, Random16() & 0xffff);
        }
        CASTING_IMPACT_PARTICLES[63].y = 159;
      }
      {
        struct EffectStep *step = CASTING_IMPACT_PARTICLES;
        struct EffectPosition *origin;
        /* FAKEMATCH: a zero offset known only after gcse keeps the origin copy,
         * hoisted after the projection address. origin = targetScreen, or
         * targetScreen->x and ->y read directly, differ 64 on tbs-en, two
         * instructions fewer. */
        s32 bias = 0;
        for (i = 0; i != CASTING_IMPACT_PARTICLE_COUNT; i++, step++) {
          if (step->x >= 0 && frame >= i / 2) {
            s32 image = i & 3;
            Render_ResetTransformState();
            SceneTransform_ApplyPitch(step->velocityX);
            SceneTransform_ApplyYaw(step->velocityY);
            EffectPosition_ApplyBaseAndYOffset(step, &projected);
            /* FAKEMATCH: the zero bias above; see there. */
            origin = (struct EffectPosition *)((u8 *)targetScreen + bias);
            projected.x = projected.x / 2 + origin->x / 2;
            projected.y += origin->y + 32;
            blitters[1](canvas,
                        Ram_MapCellBuffer + gCastingImpact_OrbitCells[image],
                        projected.x - 4, projected.y - 4, 8, 8);
            step->x -= 6;
            if (step->x < 0 && ((i & 7) == 0 || i == 63)) {
              AudioCommand_PlayFar(133);
              ObjectGroup_UpdateMembers(work->effect->actors[0],
                                        SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 4);
            }
          }
        }
      }
    } else if (kind == CASTING_IMPACT_TERRA_STRIKE) {
      // 32-pixel-wide image grows from the ground to 96 pixels, then shrinks.
      s32 height;
      if (frame >= 5 && frame <= 49) {
        if (frame > 25) {
          height = 196 - (frame << 2);
        } else {
          height = (frame << 4) - 64;
        }
        if (height > 96) {
          height = 96;
        }
        CastingImpact_DrawImage(canvas, Ram_MapCellBuffer, 48, 104 - height, 32,
                                height, &blitters[0]);
      }
    } else if (kind == CASTING_IMPACT_FORCIBLE_ARM ||
               kind == CASTING_IMPACT_TRUNCHEON_FIST) {
      // 32x64 image falls diagonally onto the target over six frames.
      s32 offset;
      s32 imageX;
      s32 imageY;
      if (frame <= 5) {
        /* FAKEMATCH: needed to match: calculate each center before the shared
         * centered draw 2026-10-11: targetScreen->x / 2 in each arm differs
         * 147; with offset set first 129; hoisted out of the arms 146. */
        if (work->effect->side == 0) {
          imageX = targetScreen->x;
          imageX += (u32)imageX >> 31;
          imageX >>= 1;
          offset = (6 - frame) * 3;
          imageX += offset * 2;
          imageY = targetScreen->y - offset * 4 + 24;
        } else {
          imageX = targetScreen->x;
          imageX += (u32)imageX >> 31;
          imageX >>= 1;
          offset = (6 - frame) * 3;
          imageX -= offset * 2;
          imageY = targetScreen->y - offset * 4 + 24;
        }
        CastingImpact_DrawTallImage(canvas, Ram_MapCellBuffer, imageX - 16,
                                    imageY - 32, 32, &blitters[1]);
      }
    } else if (kind == CASTING_IMPACT_BAD_OMEN) {
      // 16 particles with three 24x24 images; fade from frame 48.
      if (frame > 47) {
        REG_BLDALPHA = (64 - frame) | 0x1000;
      }
      {
        struct EffectStep *step = CASTING_IMPACT_PARTICLES;
        for (i = 0; i != 16; i++, step++) {
          s32 image = i % 3;
          EffectPosition_ApplyBaseAndYOffset(step, &projected);
          projected.x /= 2;
          blitters[i & 1](canvas, Ram_MapCellBuffer + image * 576,
                          projected.x - 12, projected.y - 12, 24, 24);
          EffectStep_AdvanceWithGravity3D(step, 60, 1 << ((i & 3) + 11));
          step->variant++;
        }
      }
    } else if (kind != 100) {
      // Other kinds: six 24x48 frames per particle, four ticks each.
      struct EffectStep *step = CASTING_IMPACT_PARTICLES;
      for (i = 0; i != 16; i++, step++) {
        if (frame >= i + 4) {
          s32 age = step->variant;
          if (age <= 23) {
            s32 image = age / 4;
            EffectPosition_ApplyBaseAndYOffset(step, &projected);
            projected.x /= 2;
            blitters[i & 1](canvas, Ram_MapCellBuffer + image * 1152,
                            projected.x - 12, projected.y - 24, 24, 48);
            if (kind == CASTING_IMPACT_ACID_BATH)
              EffectStep_AdvanceWithGravity3D(step, 60, 0x400);
            else
              EffectStep_AdvanceWithGravity3D(step, 60, 0x1000);
            step->variant++;
          }
        }
      }
    }
    // Djinni kinds 0-7: fly to the target in six frames.
    if (kind <= 7) {
      if (frame <= 5) {
        EffectPosition_ApplyBaseAndYOffset(position, &projected);
        projected.x /= 2;
        djinnX = projected.x;
        drawPair[1](canvas, Ram_MapCellBuffer + CASTING_DJINNI_CELLS,
                    djinnX - 10, projected.y - 4, 20, 40);
        position->x += motion->x;
        position->y += motion->y;
        position->z += motion->z;
      }
    }
    // Impact: ability-specific shake at frame 6; target reacts at frames 6
    // and 14.
    if (frame == 3) {
      BattleEventRuntime_BeginPhaseFar(-1);
    }
    if (frame == 4) {
      AudioCommand_PlayFar(134);
    }
    if (frame == 6) {
      if (kind == CASTING_IMPACT_FLINT || kind == CASTING_IMPACT_SCORCH ||
          kind == CASTING_IMPACT_SAP || kind == CASTING_IMPACT_LIFE_NOURISH ||
          kind == CASTING_IMPACT_LIFE_LEECH ||
          kind == CASTING_IMPACT_PSY_LEECH || kind == CASTING_IMPACT_BARRAGE ||
          kind == CASTING_IMPACT_TRUNCHEON_FIST || kind == 100) {
        BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 4);
        work->shakeFrames = 8;
      } else if (kind == CASTING_IMPACT_BROIL ||
                 kind == CASTING_IMPACT_AQUA_SOCK ||
                 kind == CASTING_IMPACT_FORCIBLE_ARM) {
        BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 1);
        work->shakeFrames = 2;
      } else if (kind == CASTING_IMPACT_CYCLONE_SLASH ||
                 kind == CASTING_IMPACT_TERRA_STRIKE) {
        BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 3);
        work->shakeFrames = 8;
      }
    }
    if (frame == 6) {
      ObjectGroup_UpdateMembers(work->effect->actors[0], SPRITE_COLOR_WHITE,
                                POSE_DAMAGE, 0, 4);
    }
    if (frame == 14) {
      ObjectGroup_UpdateMembers(work->effect->actors[0], SPRITE_COLOR_WHITE,
                                POSE_DAMAGE, 0, 4);
    }
    Camera_ApplyShake(8, 8);
    EFFECT_END_FRAME(work);
    frame++;
  }
  if (kind == CASTING_IMPACT_MELTDOWN) {
    Iwram_ClearWords(BG_CHAR_BLOCK(1), 0x4000);
    Iwram_ClearWords(canvas, 0x4000);
    work->effect->prelude = 0;
    Scheduler_RemoveCallback(Palette_StepFadeTransfer);
    Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
    EFFECT_RELEASE_BLITTERS();
    BattleFx_RunImpair(command);
  } else {
    if (kind == CASTING_IMPACT_MIST || kind == CASTING_IMPACT_FEVER ||
        kind == CASTING_IMPACT_BAD_OMEN || kind == CASTING_IMPACT_HEAT_MIRAGE ||
        kind == CASTING_IMPACT_BLINDING_SMOG || kind == CASTING_IMPACT_MURK) {
      Scheduler_RemoveCallback(BattleFx_ArmBg2AffineHBlankDma);
    }
    EFFECT_FINISH_CANVAS();
  }
}

// Formation placement, radial ripples, fire charges and impact bursts.
//
// Three canvas effects whose pictures are described the same way: each sheet
// is a list of cells, given as widths, heights and byte offsets, and an
// animation picks a cell by frame.
//
// - Torch's rippling disc builds a radial picture by computing one quadrant
//   and mirroring it into the other three, then bends it line by line through
//   BG2's affine reference while the palette cycles. Each pixel holds its
//   distance from the middle, so rotating the palette makes rings run
//   outward.
// - Helm Splitter's fire swirl charge leaps the user at the first target,
//   plays the swirl over it and scatters bouncing sparks. The sparks live in
//   gMapCellBuffer, whose 1024 slots are cleared first.
// - The impact burst plays a flash with large and small sparks and three
//   fragments, while the effect's objects settle into one of the formations
//   of BattleFx_PlaceFormationObjects. Large sparks and the fragments use
//   work->particles (0 to 63 and 61 to 63, the fragments overwriting the last
//   large sparks), small sparks gMapCellBuffer.

#define TORCH_FRAMES 96 // frames in the effect

// Abilities: Torch (kind 214).
//
// Build a mirrored radial picture, then fade it in and out through scanline
// ripples while its rings run outward. The scene runs for 96 frames: it
// fades in over the first 9, ripples throughout and fades out over the
// last 7.
void BattleFx_RunTorch(struct BattleEffectArgument *effect) {
  void **heapCache;
  struct BattleEffectWork *work;
  void *canvas;
  s32 screenY;
  s32 screenX;
  DrawRectangle draw[2];
  s32 x;
  s32 y;
  s32 dx;
  s32 dy;
  s32 cy;
  s32 d;
  s32 i;
  s32 t;
  s32 r;
  s32 g;
  s32 b;
  s32 frame;
  s32 wave;
  s32 level;
  u16 *pal;
  s32 angle;
  s32 *line;

  heapCache = gBattleFxWork;
  EFFECT_GET_WORK_AND_CANVAS(heapCache, work, canvas);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0x2000);
  BattleFx_PrepareCanvasEffect(effect, 6, work->effect->side, 2, &screenX,
                               &screenY);
  REG_BG2CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7) | BGCNT_WRAP;
  REG_BLDALPHA = BLEND_ALPHA(0, 16);
  REG_BG2PA = Q_8_8(1.0 / 1.5);
  BattleFx_FetchRectangleBlitters(work->effect->side, draw);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);

  // Build one quadrant and mirror it into the other three.
  for (y = 0; y != 64; y++) {
    for (x = 0; x != 64; x++) {
      cy = y / 8 + 64;
      dy = y - cy;
      dx = x - 64;
      d = Iwram_Sqrt(dx * dx + dy * dy);
      d /= 2;
      if (d == 0)
        d = 1;
      if (d > 63)
        d = 63;
      work->sheet[y * 128 + x] = d;
      work->sheet[y * 128 + 127 - x] = d;
      work->sheet[(127 - y) * 128 + x] = d;
      work->sheet[(127 - y) * 128 + 127 - x] = d;
    }
  }

  pal = (u16 *)Ram_MapCellBuffer;
  for (i = 1; i != 64; i++) {
    if (i > 31)
      t = 64 - i;
    else
      t = i;
    r = t * 9;
    g = t * 7 - 42;
    b = t * 7 - 56;
    if (r < 0)
      r = 0;
    if (g < 0)
      g = 0;
    if (b < 0)
      b = 0;
    if (r > 255)
      r = 255;
    if (g > 255)
      g = 255;
    if (b > 250)
      b = 250;
    r >>= 3;
    g >>= 3;
    b >>= 3;
    pal[i] = ((u16 *)BG_PLTT)[i] = b << 10 | g << 5 | r;
  }

  draw[0](canvas, work, 0, 0, 128, 128);
  work->transferPending = 1;
  Scheduler_AddOrUpdateCallback(BattleFx_ArmBg2AffineHBlankDma, 0x480);

  // Fade the radial picture in, ripple each scanline, then fade it out.
  for (frame = 0; frame != TORCH_FRAMES; frame++) {
    if (frame <= 8) {
      level = frame * 2;
      wave = level;
      REG_BLDALPHA = level | 0x1000;
    } else {
      wave = frame * 2;
    }
    if (frame > 88)
      REG_BLDALPHA = (0xc0 - frame * 2) | 0x1000;

    line = work->bg2X;
    for (i = 0, angle = -(wave << 9); i != 160; i++) {
      *line++ = ((i << 18) - ((Trig_Sin(angle) * 128)) + 0x40000) >> 10;
      angle += 0x200;
    }

    if (frame > 127) {
      work->transferPending = 1;
    } else {
      pal[1] = pal[63];
      Dma_Set(pal + 62, pal + 63, 0x80a0003e, REG_DMA3);
      IO_WRITE_QUEUE_PUSH((u32)&BG_PLTT[1], (u32)(pal + 1), 0x8000003f);
    }
    WaitFrames(1);
  }

  Scheduler_RemoveCallback(BattleFx_ArmBg2AffineHBlankDma);
  EFFECT_FINISH_CANVAS();
}

// Abilities: Kite (kind 217).
//
// Show the extended Jupiter Djinni prelude on the side opposite the effect.
void BattleFx_RunKite(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  s32 originX;
  s32 originY;

  work = WORK_SLOT(HEAP_SLOT_BATTLE_EFFECT);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);
  BattleFx_PrepareCanvasEffect(effect, 7, work->effect->side ^ 1, 0, &originX,
                               &originY);
  BattleFx_EndCanvasLayer();
}

#define HELM_SPLITTER_FRAMES 70       // frames in the effect
#define HELM_SPLITTER_STRIKE_FRAME 18 // the swirl strikes the target
// The pink star animation, after the swirl.
#define HELM_SPLITTER_STAR_CELLS 0x3e80
#define HELM_SPLITTER_CLEARED_SLOTS 1024 // map-buffer spark slots cleared first
#define HELM_SPLITTER_SPARKS 16          // sparks scattered by the strike
#define HELM_SPLITTER_SPARK_SLOTS 128    // spark slots moved and drawn

// Abilities: Helm Splitter (kind 51).
//
// Leap towards the first target, strike through a fire swirl and scatter
// sparks.
void BattleFx_RunHelmSplitter(struct BattleEffectArgument *effect) {
  void **heapCache;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  u8 *graphics;
  DrawRectangle draw[2];
  struct EffectPosition position;

  struct EffectStep *sparks;
  struct EffectStep *spark;
  s32 *particleLife;
  s32 frame;
  s32 i;

  EFFECT_GET_SLOTS(heapCache, cursor, work, canvas);
  graphics = EFFECT_SLOT(heapCache, HEAP_SLOT_BATTLE_SHEET);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);
  REG_BG2PA = Q_8_8(1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, graphics, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_FireSwirlSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_PinkStarSheet,
                             work->sheet + HELM_SPLITTER_STAR_CELLS, 1, 0);
  BattleFx_FetchRectangleBlitters(work->effect->side, draw);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 50);

  // Clear the reusable spark pool before moving the user.
  particleLife = &gMapCellBuffer->variant;
  for (i = 0; i != HELM_SPLITTER_CLEARED_SLOTS; i++) {
    *particleLife = 0;
    particleLife += 7;
  }
  sparks = gMapCellBuffer;

  {
    struct MotionObject *object =
        GetBattleObjectSlotFar(work->effect->actor)->object;
    struct MotionObject *target =
        GetBattleObjectSlotFar(work->effect->actors[0])->object;
    s32 diffX = target->x - object->x;
    s32 startX = object->x;
    s32 deltaX = diffX * 80 / 100;
    s32 diffZ = target->z - object->z;
    s32 startZ = object->z;
    s32 deltaZ = diffZ * 80 / 100;
    s32 x = startX + deltaX;
    s32 z = startZ + deltaZ;
    s32 shortX = deltaX >> 8;
    s32 shortZ = deltaZ >> 8;
    s32 distance;

    distance = Iwram_Sqrt(shortX * shortX + shortZ * shortZ);
    distance = (distance << 8) / 20;
    object->acceleration = distance;
    object->speedLimit = distance;
    object->snapToTarget = 1;
    object->velocityY = 0x70000;
    object->verticalMotionStrength = 0xdeb8;
    object->verticalMotionPhase = 0;
    object->autoFaceMotion = 1;
    ObjectDispatch_InitFromTable6Far(object);
    Object_SetMoveTargetFar(object, x, 0, z);
    ObjectDispatch_ApplyArgumentToChildrenFar(object, 2);
  }

  // Play the swirl and impact, then let the sparks bounce and fade.
  for (frame = 0; frame != HELM_SPLITTER_FRAMES; frame++) {
    EffectPosition_ApplyAlternateStepAndYOffset(work->effect->actor, &position);
    REG_BG2X = (80 - position.x) << 8;
    if (frame >= 8 && frame <= 23) {
      s32 cell = (frame - 8) / 2;

      if (cell > 6)
        cell = 6;
      if (work->effect->side == 0) {
        draw[0](canvas, work->sheet + gHelmSplitter_CellSourceOffsets[cell],
                gHelmSplitter_CellBiasX[cell] + 30,
                gHelmSplitter_CellBiasY[cell] + position.y - 60,
                gHelmSplitter_CellWidths[cell],
                gHelmSplitter_CellHeights[cell]);
      } else {
        draw[0](canvas, work->sheet + gHelmSplitter_CellSourceOffsets[cell],
                -gHelmSplitter_CellBiasX[cell] -
                    gHelmSplitter_CellWidths[cell] + 108,
                gHelmSplitter_CellBiasY[cell] + position.y - 60,
                gHelmSplitter_CellWidths[cell],
                gHelmSplitter_CellHeights[cell]);
      }
    }
    if (frame == HELM_SPLITTER_STRIKE_FRAME) {
      BattleEventRuntime_BeginPhaseFar(134);
      ObjectGroup_UpdateMembers(work->effect->actors[0], SPRITE_COLOR_WHITE,
                                POSE_DAMAGE, 0, 8);
      BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 6);
      work->shakeFrames = 4;
      for (i = 0; i != HELM_SPLITTER_SPARKS; i++) {
        s32 force = (Random16() & 63) + 256;
        s32 angle = Random16() & 0xffff;

        spark = &sparks[i];
        spark->x = 0x400000;
        spark->y = 0x500000;
        spark->velocityX = (Trig_Sin(angle) * force) >> 7;
        spark->velocityY = -(Trig_Cos(angle) * force) >> 6;
        spark->variant = (Random16() & 15) + 16;
      }
    }
    for (i = 0; i != HELM_SPLITTER_SPARK_SLOTS; i++) {
      spark = &sparks[i];
      if (spark->variant > 0) {
        spark->variant--;
        EffectStep_AdvanceWithGravity2D(spark, 60, 0);
        if (spark->y > 0x680000) {
          spark->velocityY = -spark->velocityY / 2;
        } else if ((spark->x >= 0 && spark->x <= 0x7effff) && spark->y >= 0) {
          s32 sx = spark->x >> 16;
          s32 sy = spark->y >> 16;

          draw[0](canvas,
                  work->sheet + HELM_SPLITTER_STAR_CELLS +
                      (((frame + i) / 4 % 6) << 8),
                  sx - 8, sy - 8, 16, 16);
        }
      }
    }
    Camera_ApplyShake(8, 8);
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS();
}

// Arrange work->objects in the requested formation.
// formation: 0 = 3x3 at 32-pixel spacing; 1 = 12 in rows of 1, 3, 4, 4;
//            2 = 8 in rows of 3, 2, 3; 3 = 8 scaled by 1.25.
// x, z: formation origin in 16.16 pixels
void BattleFx_PlaceFormationObjects(s32 formation, s32 x, s32 z) {
  static struct SpriteScale normalScale = {{{0x10000, 0x10000}}};
  static struct SpriteScale smallScale = {{{0x14000, 0x14000}}};
  struct BattleEffectWork *work = WORK_SLOT(HEAP_SLOT_BATTLE_EFFECT);
  struct SpriteScale normal = normalScale;
  struct SpriteScale big = smallScale;
  struct SpritePlacement pos;
  s32 i;

  pos.groundHeight = 0;
  pos.height = 255 << 16;
  switch (formation) {
  case 0:
    for (i = 0; i != 9; i++) {
      pos.x = (i % 3 << 21) + x; // 32-pixel spacing
      pos.y = (i / 3 << 21) + z;
      Object_ApplyProjectedPlacementFar(work->objects[i], &pos, &normal, 0);
    }
    break;
  case 1:
    for (i = 0; i != 12; i++) {
      pos.x = (gFormation_TwelveX[i] << 16) + x - (16 << 16);
      pos.y = (gFormation_TwelveZ[i] << 16) + z - (32 << 16);
      Object_ApplyProjectedPlacementFar(work->objects[i], &pos, &normal, 0);
    }
    break;
  case 2:
    for (i = 0; i != 8; i++) {
      pos.x = (gFormation_RightEightX[i] << 16) + x + (16 << 16);
      pos.y = (gFormation_RightEightZ[i] << 16) + z;
      Object_ApplyProjectedPlacementFar(work->objects[i], &pos, &normal, 0);
    }
    break;
  case 3:
    for (i = 0; i != 8; i++) {
      pos.x = (gFormation_SmallEightX[i] << 16) + x;
      pos.y = (gFormation_SmallEightZ[i] << 16) + z;
      Object_ApplyProjectedPlacementFar(work->objects[i], &pos, &big, 0);
    }
    break;
  }
}

#define IMPACT_SHARD_CELLS 0x59d8 // Fragment graphics: four 12x24 images

#define IMPACT_BURST_FRAMES 72         // frames in the effect
#define IMPACT_BURST_FRAGMENTS 3       // fragments, work->particles 61 to 63
#define IMPACT_BURST_DRAWN_LARGE 30    // large sparks drawn
#define IMPACT_BURST_DRAWN_SMALL 60    // small sparks drawn
#define IMPACT_BURST_LARGE_SPARKS 64   // large sparks thrown
#define IMPACT_BURST_SMALL_SPARKS 64   // small sparks thrown
#define IMPACT_BURST_FIRST_FRAGMENT 61 // the first fragment in work->particles

// Open an impact flash and scatter sparks and fragments over 72 frames.
// Also rearrange the objects into formation during the first 36 frames.
// formation: layout used by BattleFx_PlaceFormationObjects
// x, y: impact position in 16.16 pixels
void BattleFx_RunImpactBurst(s32 formation, s32 x, s32 y) {
  void **heap;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle additive;
  DrawRectangle maxBlend;
  u8 *sheet;
  s32 orgX;
  struct EffectStep *particle;
  s32 frame;
  s32 i;

  // Set up work and canvas; double BG2 horizontally and blend BG1 with BG2.
  EFFECT_GET_SLOTS(heap, cursor, work, canvas);
  sheet = heap[2];
  orgX = x;
  x += 40 << 16;
  x /= 2; // Canvas uses half the horizontal width.
  REG_BG2PA = Q_8_8(0.5);
  REG_BG2X = 0;
  REG_BLDCNT =
      BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS, COMPOSITE_MAX);
  maxBlend = heap[7];
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_ADDITIVE);
  additive = heap[8];
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_FlareSheet, work->sheet, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_FlareImage,
                             work->sheet + IMPACT_SHARD_CELLS, 0, 0);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 50);

  // Large sparks: speed 256-511, lifetime 16-31 frames; use the first 30.
  for (i = 0; i != IMPACT_BURST_LARGE_SPARKS; i++) {
    s32 speed = (Random16() & 255) + 256;
    s32 angle = Random16() & 0xffff;

    work->particles[i].x = x;
    work->particles[i].y = y;
    work->particles[i].velocityX = (Trig_Sin(angle) * speed) >> 7;
    work->particles[i].velocityY = -((Trig_Cos(angle) * speed) >> 6);
    work->particles[i].variant = (Random16() & 15) + 16;
  }

  // Three fragments, 120 degrees apart.
  for (i = 0; i != IMPACT_BURST_FRAGMENTS; i++) {
    work->particles[IMPACT_BURST_FIRST_FRAGMENT + i].x = x;
    work->particles[IMPACT_BURST_FIRST_FRAGMENT + i].y = y;
    work->particles[IMPACT_BURST_FIRST_FRAGMENT + i].velocityX =
        (Trig_Sin(i * 0x5555) * 32) >> 6;
    work->particles[IMPACT_BURST_FIRST_FRAGMENT + i].velocityY =
        -((Trig_Cos(i * 0x5555) * 32) >> 5);
  }

  // Small sparks: speed 32-287, lifetime 20-35 frames.
  for (i = 0; i != IMPACT_BURST_SMALL_SPARKS; i++) {
    s32 speed = (Random16() & 255) + 32;
    s32 angle = Random16() & 0xffff;

    gMapCellBuffer[i].x = x;
    gMapCellBuffer[i].y = y;
    gMapCellBuffer[i].velocityX = (Trig_Sin(angle) * speed) >> 6;
    gMapCellBuffer[i].velocityY = -((Trig_Cos(angle) * speed) >> 5);
    gMapCellBuffer[i].variant = (Random16() & 15) + 20;
  }

  for (frame = 0; frame != IMPACT_BURST_FRAMES; frame++) {
    if (frame == 4)
      AudioCommand_PlayFar(154);
    if (frame == 32)
      AudioCommand_PlayFar(212);

    // Flash: advance the image every five frames starting at frame 8.
    if (frame <= 47) {
      s32 cell = (frame - 8) / 5;
      u32 width;
      u32 height;

      if (cell < 0)
        cell = 0;
// Sequence dimensions before the call; assigning and reading them in
// separate arguments was UB. Plain English trial: unchanged 0x3d0 bytes,
// with different instructions. Preserve the original native calls.
#ifdef UBFIX
      width = gImpactBurst_CellWidths[cell];
      height = gImpactBurst_CellHeights[cell];
      maxBlend(canvas, work->sheet + gImpactBurst_CellSourceOffsets[cell],
               (x >> 16) - (width >> 1), (y >> 16) - (height >> 1), width,
               height);
#else
      maxBlend(canvas, work->sheet + gImpactBurst_CellSourceOffsets[cell],
               (x >> 16) - ((width = gImpactBurst_CellWidths[cell]) >> 1),
               (y >> 16) - ((height = gImpactBurst_CellHeights[cell]) >> 1),
               width, height);
#endif
    }

    // Emit two large sparks every other frame.
    for (i = 0, particle = work->particles; i != IMPACT_BURST_DRAWN_LARGE;
         i++, particle++) {
      if (frame > i / 2 && particle->variant > 0) {
        s32 size;
        s32 px;
        s32 py;

        particle->variant--;
        EffectStep_AdvanceWithGravity2D(particle, 60, 0);
        size = particle->variant / 16 + 3;
        px = HI(particle->x);
        py = HI(particle->y);
        EFFECT_DRAW_MOTE(additive, canvas, sheet, px, py, size);
      }
    }

    // Small sparks start at frame 36.
    for (i = 0, particle = gMapCellBuffer; i != IMPACT_BURST_DRAWN_SMALL;
         i++, particle++) {
      if (frame > 35 && particle->variant > 0) {
        s32 size;
        s32 px;
        s32 py;

        particle->variant--;
        EffectStep_AdvanceWithGravity2D(particle, 60, 0);
        size = particle->variant / 16 + 1;
        px = HI(particle->x);
        py = HI(particle->y);
        EFFECT_DRAW_MOTE(additive, canvas, sheet, px, py, size);
      }
    }

    // Fragments run on frames 36-63; advance the image every seven frames.
    for (i = 0; i != IMPACT_BURST_FRAGMENTS; i++) {
      struct EffectStep *shard =
          &work->particles[IMPACT_BURST_FIRST_FRAGMENT + i];

      if (frame >= 36 && frame <= 63) {
        s32 cell;
        s32 px;
        s32 py;

        EffectStep_AdvanceWithGravity2D(shard, 64, 0);
        cell = (frame - 36) / 7;
        px = HI(shard->x);
        py = HI(shard->y);
        maxBlend(canvas, work->sheet + cell * 0x120 + IMPACT_SHARD_CELLS,
                 px - 6, py - 12, 12, 24);
      }
    }

    if (frame <= 35)
      BattleFx_PlaceFormationObjects(formation, orgX, y);
    EFFECT_PRESENT_FRAME(work);
  }

  // Finish by clearing background character block 1.
  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  Iwram_ClearWords(BG_CHAR_BLOCK(1), 0x4000);
  EFFECT_RELEASE_BLITTERS();
}

// Add per-channel steps to background colours 1 to 63, capped at 31.
void Palette_BrightenBgEntries(s32 blueStep, s32 greenStep, s32 redStep) {
  u16 *color = (u16 *)&BG_PLTT[1];
  s32 i;

  for (i = 0; i != 63; i++) {
    s32 blue = (*color >> 10) & 0x1f;
    s32 green = (*color >> 5) & 0x1f;
    s32 red = *color & 0x1f;

    blue += blueStep;
    green += greenStep;
    red += redStep;
    if (blue > 31)
      blue = 31;
    if (green > 31)
      green = 31;
    if (red > 31)
      red = 31;
    *color++ = (blue << 10) | (green << 5) | red;
  }
}

// Stream the effect sheet palette row into background palette RAM each HBlank.
void BattleFx_ArmPaletteHBlankDma(void) {
  struct BattleEffectWork *work = WORK_SLOT(HEAP_SLOT_BATTLE_EFFECT);
  volatile u16 *channel = REG_DMA0;
  Dma_Stop(channel);
  Dma_Set(work->sheet + 0x1f80, (void *)BG_PLTT, DMA_HBLANK_HALFWORD_PER_LINE,
          (volatile u32 *)channel);
}

// Multi-stage particle streams and circling object scenes.
//
// The particle stream runs in two halves. The rise dissolves the battle map
// into a starfield, then for up to 192 frames lifts a formation of objects
// while motes, orbiting sparks and falling drops are drawn into the map
// buffer over a heat-tinted scanline palette. The burst then switches to a
// blended half-scale canvas and throws embers, screen motes and growing
// flashes from the origin for 54 shaking frames. The burst's flashes use the
// first 128 shared particle records and its embers the 512 records after
// them; the work's own particle slots hold the rising motes (0 to 31) and the
// flames (32 to 47).
//
// The other three effects are one frame loop each:
//
// - The circling scene (Ramses), over the desert backdrop: a formation
//   stands while an orb circles, drops three times throwing puffs, and flies
//   off; six rocks fall and bounce, and the orb's flight ends in a burst of
//   512 sparks from the shared particle records.
// - The dual stream (Dragon Cloud): a flame leaves the user along a stored
//   path with a fading trail of 19 copies behind it, then strikes the first
//   target and bursts into embers and flashes.
// - The fire burst shards (Annihilation): the user closes on the first
//   target, a fire streak flares and a burst of 64 shards scatters from the
//   target.
//
// Within a frame the order of the steps matters: later steps draw over
// earlier ones, and several of them consume random numbers.

// Abilities: Meteor (scene kind 11).
//
// Run the particle stream in mode 0.
DEFINE_EFFECT_KIND(BattleFx_RunMeteor, BattleFx_RunParticleStreams, 0)

// Abilities: Outer Space (kind 384).
//
// Run the particle stream in mode 1.
DEFINE_EFFECT_KIND(BattleFx_RunOuterSpace, BattleFx_RunParticleStreams, 1)

extern s16 gParticleStreams_OrbitPoints[][3];
extern s16 gParticleStreams_DropPoints[][2];
extern u16 gParticleStreams_FlashSheetOffsets[];
extern u16 gParticleStreams_FlashSizes[];

#define STREAM_EMBERS (&PARTICLES[128])

#define PARTICLE_STREAMS_RISE_FRAMES 192 // frames, at most, of the rise
#define PARTICLE_STREAMS_BURST_FRAMES 54 // frames of the burst

// The stream runs in two halves. The rise dissolves the battle map into a
// starfield, then for up to PARTICLE_STREAMS_RISE_FRAMES lifts a formation of
// objects while motes, orbiting sparks and falling drops are drawn into the
// map buffer over a heat-tinted scanline palette. The burst then switches to
// a blended half-scale canvas and throws embers, screen motes and growing
// flashes from the origin for PARTICLE_STREAMS_BURST_FRAMES shaking frames.
//
// Particle slots: the burst's flashes use PARTICLES 0 to 127 and its embers
// the 512 after them (STREAM_EMBERS); work->particles holds the rising motes
// (0 to 31) and the flames (32 to 47).
void BattleFx_RunParticleStreams(struct BattleEffectArgument *effect,
                                 s32 mode) {
  volatile u32 fill;
  u8 offsets[128];
  void **cache;
  void *canvas;
  DrawRectangle draw;
  s32 direction;
  struct BattleEffectWork *work;
  void *colors = (void *)BG_PLTT;

  // Start a flame in the first free particle slot from 32 to 47.
  void ParticleStreams_SpawnFlame(s32 x, s32 y, s32 velocityX) {
    s32 i;

    for (i = 0; i != 16; i++) {
      struct EffectStep *flame = &work->particles[32 + i];

      if (flame->variant == -1) {
        flame->variant = 0;
        flame->x = x;
        flame->y = y;
        flame->velocityX = velocityX;
        break;
      }
    }
  }

  // Start a mote in the first free particle slot from 0 to 31.
  UNUSED void ParticleStreams_SpawnMote(s32 x, s32 y) {
    s32 i;

    for (i = 0; i != 32; i++) {
      struct EffectStep *mote = &work->particles[i];

      if (mote->variant == -1) {
        mote->variant = 0;
        mote->x = x;
        mote->y = y;
        break;
      }
    }
  }

  struct SpritePlacement position;
  s32 vector[3];
  struct EffectPosition spot;
  struct EffectPosition *screenPosition;
  struct EffectPosition point;
  struct SpriteScale scale;
  s32 frame;
  s32 i;

  cache = &gBattleFxWork[1];
  canvas = cache[0];
  work = cache[-1];

  work->effect = effect;
  BattleFx_BeginCanvasLayer(0x2000);
  REG_BG2PA = Q_8_8(1);
  /* FAKEMATCH: test through direction so CSE retains the mode reload
   * in this arm. Testing mode directly replaces ldr r4,[sp,#76] with
   * movs r4,#1 (1532 instructions, two differing rows). 2026-10-13:
   * a switch on mode, side tested by if/else or `side != 1 ? -1 : mode`
   * differ the same 2; direction = -1 first and set in the arm 58. */
  direction = mode;
  if (direction == 1) {
    struct MotionObject *object =
        GetBattleObjectSlotFar(work->effect->actor)->object;

    object->velocityY = 0xa0000;
    object->verticalMotionStrength = 0x91eb;
    ObjectGroup_UpdateMembers(work->effect->actor, -1, 2, -1, 0);
    AudioCommand_PlayFar(145);
    direction = work->effect->side == 1 ? mode : -1;
  } else {
    direction = -1;
  }
  BattleFx_ConfigureEffectDisplay();
  PLTT_BACKDROP = 0;
  *(u16 *)&BG_PLTT[1] = 0;
  work->transferMode = CANVAS_COPY;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);
  BattleFx_WipeCanvas(0, 0);
  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  if (mode == 1) {
    for (i = 0; i != 2; i++) {
      struct AnimationObject *object =
          ResourceObject_CreateFar(483 + i * 0x2001);

      work->objects[i] = object;
      if (object != 0) {
        object->flags = 0;
        AnimationObjects_SelectAnimationFar(object, 2);
        work->objects[i]->image.priority = 3;
      }
    }
  } else {
    BattleFx_SpawnObjects(1, 381, 3);
  }
  Resource_LoadAndDecompress((s32)&ResourceId_FlameSheetB, work, 1, 1);
  if (mode == 1)
    Iwram_CopyWords(colors,
                    Resource_GetTableEntry((s32)&ResourceId_LightningBoltSheet),
                    128);
  fill = 0x01010101;
  Dma_Set((void *)&fill, Ram_MapCellBuffer, DMA_FILL_WORDS(0x2000), REG_DMA3);
  Iwram_CopyWords((void *)BG_CHAR_BLOCK(2), Ram_MapCellBuffer, 0x7800);
  WaitFrames(1);
  REG_BLDCNT = 0;
  REG_BG2PA = Q_8_8(1);
  REG_BG1CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(0) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(31);
  REG_BG2CNT = BGCNT_PRIORITY(3) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7) | BGCNT_WRAP;
  for (i = 0; i != 63; i++) {
    s32 red = (Random16() & 15) + 16;
    s32 green = (Random16() & 15) + 16;
    s32 blue = (Random16() & 15) + 16;

    ((u16 *)&BG_PLTT_COLOR(8, 0))[i] = (blue << 10) | (green << 5) | red;
  }
  fill = 0;
  Dma_Set((void *)&fill, canvas, DMA_FILL_WORDS(0x1000), REG_DMA3);
  for (i = 0; i != 256; i++) {
    s32 x = Random16() & 127;
    s32 y = Random16() & 127;
    s32 shade = (Random16() & 63) + 64;

    ((u8 *)canvas)[((y / 8 * 16 + x / 8) * 8 + (y & 7)) * 8 + (x & 7)] = shade;
  }
  Iwram_CopyWords((void *)BG_CHAR_BLOCK(1), canvas, 0x4000);
  gProjection.centerY = 240;
  BattleFx_SelectLivingTargets(work->effect);
  work->bg2x = 0;
  work->bg2y = 0;
  work->scrollTimer = 0;
  work->scrollInterval = 2;
  work->scrollStepX = direction << 7;
  work->scrollStepY = 256;
  Scheduler_AddOrUpdateCallback(Camera_AdvanceBg2Reference, 0x4ff);
  Scheduler_AddOrUpdateCallback(BattleFx_FlushPendingGraphicsTransfer, 0x480);
  for (i = 0; i != 128; i++)
    offsets[i] = Random16() & 63;
  {
    s32 limit = 0;
    s32 step = 1;
    s32 row = 0;

    do {
      limit += step / 2;
      step += 4;
      for (; row != limit; row++) {
        s32 x;

        for (x = 0; x != 256; x++) {
          s32 y = row - offsets[x & 127];

          if (y >= 0)
            if (y <= 127)
              Ram_MapCellBuffer[((y / 8 * 32 + x / 8) * 8 + (y & 7)) * 8 +
                                (x & 7)] = 0;
        }
      }
      EFFECT_PRESENT_FRAME(work);
    } while (limit <= 191);
  }
  REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  {
    s32 savedX = gBgScroll[1].x;
    s32 savedY = gBgScroll[1].y;
    // Two independent absolute slot reads grow native EN from 0xe38 to 0xe48.
    void **slots = &gWorkSlot[HEAP_SLOT_BATTLE_BACKGROUND];
    struct BattlePresentationTransition *presentation = slots[0];

    gBgScroll[1].x = 0;
    gBgScroll[1].y = 32;
    BattleFx_LoadWork(HEAP_SLOT_BLITTER, 8, 7,
                      COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, COMPOSITE_MAX);
    draw = slots[HEAP_SLOT_BLITTER - HEAP_SLOT_BATTLE_BACKGROUND];
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_DARKEN,
                        (s32)(Ram_MapBlocks + 0x202));
    Scheduler_AddOrUpdateCallback(BattleFx_ArmPaletteHBlankDma, 0x4fe);
    EFFECT_FREE_PARTICLES(i, work->particles, 64);
    presentation->active = 1;
    work->frame = 0;
    frame = 0;
    do {
      s32 angle = frame << 8;
      /* FAKEMATCH: count fixed in r2, as in the reference; as a hard register
       * it leaves the division temp canonical in cse, so the compare and +3 use
       * the temp as in the reference. */
      register s32 count asm("r2") = work->frame;
      /* An ordinary base alone leaves 8 aligned differences in 1532
       * instructions. Integer/word/halfword carriers, a typed inline
       * line helper and general or early-clobber barrier outputs keep
       * that residual. A two-minute exact-stack/branch search (seed
       * 353, nested helpers ignored) found no improvement in 1241
       * compiled candidates. Changing count alone differs 10, both
       * count and base differ 74, and level alone differs 42. */
      /* FAKEMATCH: work's copy in r0; else it and the 0x778c reload swap r0/r1.
       * 2026-10-11: plain `heat = work->frame / 4` with a plain line pointer
       * differs 69: combine merges count's copy, the 0x778c reload and work
       * swap r0/r1, and the halfword zero becomes a literal-pool load once its
       * reload scratch moves. */
      register u8 *base asm("r0") = (u8 *)work;
      s32 heat = count / 4;
      u16 *line;
      s32 size;
      s32 x;
      s32 y;

      /* FAKEMATCH: count read and base passed through; reading count after the
       * division keeps its load apart from the division temp (no combine);
       * through base the asm is not volatile, so no sched2 barrier, and the
       * mode reload lands after movs r2, #252 as in the reference (38 -> 2). */
      asm("" : "=r"(base) : "0"(base), "r"(count));
      line = (u16 *)(base + 0x1f80);
      if (mode == 1) {
        if ((gKeysRepeat & 3) && frame > 16)
          break;
      } else {
        if ((gKeysRepeat & 3) && frame > 4)
          break;
      }
      {
        s32 hold;

        /* FAKEMATCH: hold takes r7 across the call; with line in r5, hold in r7
         * and r6 clobbered, heat takes r4 and its sp+8 caller-save as in the
         * reference (222 -> 89); global.c ignores hard conflicts with r7, the
         * frame pointer, so r7 needs a pseudo. 2026-10-11: a plain call differs
         * 354, heat in r6 with no caller-save and the frame 280 instead of 284.
         */
        asm("" : "=r"(hold));
        /* FAKEMATCH: r6 is clobbered across the call. */
        asm("" : : : "r6");
        if (frame == 0)
          AudioCommand_PlayFar(141);
        /* FAKEMATCH: hold's use after the call. */
        asm("" : : "r"(hold));
      }
      for (i = 0; i != 15; i++)
        *line++ = 0;
      for (; i != 135; i++) {
        s32 green = i - 16;
        /* FAKEMATCH: reuse red for the biased signed quotient. A
         * separate quotient coalesces with level; the explicit bias
         * keeps the native r3 copy and shift (four register rows). */
        s32 red = green;
        /* Ordinary level differs 42 in EN (1532 native/built).
         * A generic output boundary after its sum reduces this to 26,
         * retaining the quotient/level r2/r3 exchange. A red input
         * retains 26; also unbinding count differs 36, with either an
         * input or output count operand on the existing base boundary.
         * Reversing the sum or deriving green as red - 48 retains 42. */
        /* FAKEMATCH: level in r2; unbinding it changes ten register/order rows.
         * 2026-10-11: the natural `level = green / 4 + heat` differs 18, a
         * level/red r3/r2 swap only (local-alloc hands level r3 first); a use
         * of level after the first red clamp matches, and moving green's
         * computation there differs 4; no plain statement survives to
         * allocation there. */
        register s32 level asm("r2");

        if (green < 0)
          red += 3;
        red >>= 2;
        level = red + heat;
        red = level - 32;
        green = level - 80;

        EFFECT_CLAMP(red, 0, 31);
        EFFECT_CLAMP(green, 0, 31);
        *line++ = (red << 10) | (green << 5) | (green >> 1);
      }
      for (; i != 160; i++)
        *line++ = 0;
      if (direction == 1)
        x = frame / 4;
      else
        x = 64 - frame / 4;
      y = 96 - frame;
      position.groundHeight = 0;
      position.height = 0xff0000;
      if (mode == 1) {
        size = angle + 0xa000;
        scale.x = size;
        scale.y = size;
        position.x = (x << 16) + 0x500000;
        position.y = (64 - y) << 16;
        Object_ApplyProjectedPlacementFar(work->objects[0], &position, &scale,
                                          0);
        Object_ApplyProjectedPlacementFar(work->objects[1], &position, &scale,
                                          0);
      } else {
        size = angle + 0x10000;
        scale.x = size;
        scale.y = size;
        position.x = (x << 16) + 0x600000;
        position.y = (96 - y) << 16;
        Object_ApplyProjectedPlacementFar(work->objects[0], &position, &scale,
                                          0);
      }
      for (i = 0, y = 32 - y; i != 32; i++) {
        struct EffectStep *mote = &work->particles[i];

        if (mote->variant == -1) {
          s32 angle = (Random16() & 0x7fff) + 0x4000;

          mote->variant = 0;
          mote->x = ((x + 96) << 16) + Trig_Sin(angle) * 30 / 65536 * size;
          mote->y = (y << 16) - Trig_Cos(angle) * 30 / 65536 * size;
          break;
        }
      }
      point.x = 0;
      point.y = 0;
      point.depth = 0x2000000;
      Render_ResetTransformState();
      SceneTransform_ApplyPosition(EffectPosition_Coordinates(&point));
      SceneTransform_ApplyRoll(0x800);
      SceneTransform_ApplyYaw(frame << 8);
      for (i = 0; i != 7; i++) {
        EFFECT_SET_VECTOR(vector,
                          (gParticleStreams_OrbitPoints[i][0] / 2) << 16,
                          (gParticleStreams_OrbitPoints[i][1] + frame) << 16,
                          (gParticleStreams_OrbitPoints[i][2] / 2) << 16);
        EffectPosition_ApplyBaseAndYOffset(vector, screenPosition = &spot);
        screenPosition->x = HI(screenPosition->x) + 128;
        screenPosition->y = HI(screenPosition->y) + 60;
        draw(Ram_MapCellBuffer, work->sheet + 0x1f40, screenPosition->x - 4,
             screenPosition->y - 4, 8, 8);
      }
      if (direction == 1)
        x = frame / 4 - 16;
      else
        x = 16 - frame / 4;
      y = frame - 96;
      for (i = 0; i != 7; i++) {
        s32 drop = gParticleStreams_DropPoints[i][1] + y;

        if (drop <= 93)
          draw(Ram_MapCellBuffer, work->sheet + 0x1c80,
               gParticleStreams_DropPoints[i][0] + x - 12, drop - 12, 24, 24);
        else if (drop <= 95)
          ParticleStreams_SpawnFlame(
              (gParticleStreams_DropPoints[i][0] + x) << 16, drop << 16, 1);
      }
      for (i = 0; i != 32; i++) {
        struct EffectStep *mote = &work->particles[i];

        if (mote->variant >= 0) {
          draw(Ram_MapCellBuffer, work->sheet + (mote->variant << 10),
               HI(mote->x) - 16, HI(mote->y) - 16, 32, 32);
          mote->x -= direction * 0x14000;
          mote->y -= 0x50000;
          mote->variant++;
          if (mote->variant == 6)
            mote->variant = -1;
        }
      }
      EFFECT_PRESENT_FRAME(work);
      frame++;
      work->frame++;
    } while (frame != PARTICLE_STREAMS_RISE_FRAMES);
    WaitFrames(1);
    presentation->active = 0;
    Scheduler_RemoveCallback(Camera_AdvanceBg2Reference);
    Scheduler_RemoveCallback(BattleFx_ArmPaletteHBlankDma);
    Scheduler_RemoveCallback(BattleFx_FlushPendingGraphicsTransfer);
    gBgScroll[1].x = savedX;
    gBgScroll[1].y = savedY;
  }
  Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
  BattleFx_SetupBlendedDisplay();
  REG_BG2PA = Q_8_8(0.5);
  REG_BG2X = 0;
  REG_BG2Y = 0xfffff000;
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  REG_BG2CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7) | BGCNT_WRAP;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                      COMPOSITE_MAX);
  draw = gWorkSlot[HEAP_SLOT_BLITTER];
  Resource_LoadAndDecompress((s32)&ResourceId_BlastSheet, work, 1, 0);
  for (i = 0; i != 32; i++) {
    struct EffectStep *mote = &work->particles[i];

    mote->x = Random16() & 127;
    mote->y = (Random16() & 127) + 127;
  }
  for (i = 0; i != 128; i++) {
    struct EffectStep *flash = &PARTICLES[i];

    EFFECT_SET_POSITION(flash, 0, 0, 0);
    EFFECT_SET_VELOCITY(flash, ((Random16() & 255) - 127) << 12,
                        (Random16() & 255) << 11,
                        ((Random16() & 255) - 127) << 12);
    flash->variant = 0;
  }
  for (i = 0; i != 512; i++) {
    struct EffectStep *ember = &STREAM_EMBERS[i];

    EFFECT_SET_POSITION(ember, 0, 0, 0);
    EFFECT_SET_VELOCITY(ember, ((Random16() & 255) - 128) << 13,
                        (Random16() & 255) << 11,
                        ((Random16() & 255) - 128) << 13);
    ember->variant = 0;
  }
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0x10101010);

  for (frame = 0; frame != PARTICLE_STREAMS_BURST_FRAMES; frame++) {
    s32 size = (frame + 0x1d0) << 8;
    struct BattleCamera *camera = gCameraWork;
    s32 elapsed = frame - 16;
    struct SpriteScale scale;
    s32 x;
    s32 y;
    s32 grow;

    if (elapsed > 19)
      Palette_BrightenBgEntries(2, 2, 2);
    if (frame == 0)
      AudioCommand_PlayFar(156);
    if (frame == 40)
      AudioCommand_PlayFar(145);
    if (frame == 48) {
      if (mode == 1) {
        ResourceObject_ReleaseFar(work->objects[0]);
        ResourceObject_ReleaseFar(work->objects[1]);
        BattleActor_CommitPlacementFar();
      }
      BattleEventRuntime_BeginPhaseFar(134);
    }
    EFFECT_VIEW_FROM_CAMERA(camera);
    for (i = 0; i != 64; i++) {
      struct EffectStep *ember = &STREAM_EMBERS[i];

      if (ember->y >= 0) {
        s32 size;

        EffectPosition_ApplyBaseAndYOffset(&ember->x, &point);
        point.x >>= 1;
        if (point.depth <= 159)
          point.depth = 160;
        if (point.depth > 799)
          point.depth = 799;
        size = 9 - (point.depth - 160) / 64;
        draw(canvas,
             work->sheet +
                 (gBattleFx_MoteCellOffsets[size - 1] + (i & 1) * 770) + 0x3200,
             point.x - size / 2, point.y - size, size, size * 2);
        EffectStep_AdvanceWithGravity3D(ember, 64, -0x2000);
        if (ember->y <= 0x140000) {
          ember->x = 0;
          ember->z = 0;
          ember->y = 0x140000;
          EFFECT_SET_VELOCITY(ember, ((Random16() & 63) - 32) << 15,
                              (Random16() & 63) << 13,
                              ((Random16() & 63) - 32) << 15);
        }
      }
    }
    for (i = 0; i != 64; i++) {
      struct EffectStep *mote = &work->particles[i];
      u32 rise = i & 7;
      s32 size = rise + 3;

      draw(canvas,
           work->sheet + (gBattleFx_MoteCellOffsets[size - 1] + (i & 1) * 770) +
               0x3200,
           mote->x - size / 2, mote->y - size, size, size * 2);
      mote->y = mote->y - rise - 8;
      if (mote->y < -10)
        mote->y = 128;
    }
    for (i = 0; i != 64; i++) {
      struct EffectStep *flash = &PARTICLES[i];

      if (i / 3 < elapsed && flash->y >= 0) {
        EffectPosition_ApplyBaseAndYOffset(&flash->x, &point);
        point.x >>= 1;
        if ((u32)flash->variant <= 13) {
          s32 cell = flash->variant / 2;

          draw(canvas, work->sheet + gParticleStreams_FlashSheetOffsets[cell],
               point.x - gParticleStreams_FlashSizes[cell] / 2,
               point.y - gParticleStreams_FlashSizes[cell] / 2,
               gParticleStreams_FlashSizes[cell],
               gParticleStreams_FlashSizes[cell]);
        }
        flash->variant++;
        if (flash->variant == 14) {
          flash->y = 0x140000;
          flash->x = 0;
          flash->z = ((Random16() & 255) - 127) << 16;
          EFFECT_SET_VELOCITY(flash, 0, (Random16() & 255) << 11, 0);
          flash->variant = 0;
        } else {
          EffectStep_AdvanceWithGravity3D(flash, 64, 1);
        }
      }
    }
    if (direction == 1)
      x = frame / 2 + 24;
    else
      x = 56 - frame / 2;
    y = 64 - frame * 2;
    grow = (frame << 8) + 0x20000;
    position.groundHeight = 0;
    position.height = 0xff0000;
    if (mode == 1) {
      scale.x = size;
      scale.y = size;
      position.x = (x << 16) + 0x600000;
      position.y = (96 - y) << 16;
      Object_ApplyProjectedPlacementFar(work->objects[0], &position, &scale, 0);
      Object_ApplyProjectedPlacementFar(work->objects[1], &position, &scale, 0);
    } else {
      scale.x = grow;
      scale.y = grow;
      position.x = (x << 16) + 0x600000;
      position.y = (96 - y) << 16;
      Object_ApplyProjectedPlacementFar(work->objects[0], &position, &scale, 0);
    }
    work->shakeFrames = 1;
    Camera_ApplyShake(8, 8);
    EFFECT_PRESENT_FRAME(work);
  }
  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
  if (mode == 0)
    ResourceObject_ReleaseFar(work->objects[0]);
  BattleFx_EndCanvasLayer();
}

#define RAMSES_FRAMES 320         // frames in the effect
#define RAMSES_STRIKE_FRAME 260   // the orb's burst strikes the targets
#define RAMSES_ROCKS 6            // rocks that fall and bounce
#define RAMSES_FIRST_PUFF 6       // the first puff in work->particles
#define RAMSES_FIRST_ROCK_PUFF 30 // the first rock puff in work->particles
#define RAMSES_SPARKS 512         // sparks of the orb's burst

// Abilities: Ramses (scene kind 1).
//
// Over the desert backdrop a formation stands while an orb circles, drops
// three times throwing puffs, and flies off; RAMSES_ROCKS rocks fall and
// bounce, and the orb's flight ends in a burst of RAMSES_SPARKS sparks from
// PARTICLES. Within a frame the order of the steps matters: later steps draw
// over earlier ones, and several of them consume random numbers.
void BattleFx_RunRamses(struct BattleEffectArgument *effect) {
  static struct SpriteScale unitScale = {{{0x10000, 0x10000}}};
  struct SpritePlacement position;
  DrawRectangle draw[2];
  struct SpriteScale scale;
  void **cache;
  void *canvas;
  struct BattleEffectWork *work;
  s32 frame;
  u8 *sheet;
  struct Point2D base;
  struct Point2D orb;
  s32 i;
  s32 k;

  cache = &gBattleFxWork[1];
  canvas = cache[0];
  work = cache[-1];
  sheet = cache[1];
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);
  BattleFx_ConfigureEffectDisplay();
  PLTT_BACKDROP = 0;
  *(u16 *)&BG_PLTT[1] = 0;
  work->transferMode = CANVAS_COPY;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);
  BattleFx_WipeCanvas(1, 0);
  BattleFx_SelectLivingTargets(work->effect);
  BattleFx_SpawnObjects(9, 379, 2);
  // Add the six falling rocks after creating the nine-object formation.
  for (i = 0; i != RAMSES_ROCKS; i++) {
    struct AnimationObject *object = ResourceObject_CreateFar(390);

    work->objects[9 + i] = object;
    if (object != 0) {
      object->flags = 0;
      AnimationObjects_SelectAnimationFar(object, i % 3);
      work->objects[9 + i]->image.priority = 1;
    }
  }
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                      COMPOSITE_MAX);
  draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                      COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, COMPOSITE_ADDITIVE);
  draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
  REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG1 |
              WININ_WIN1_BG2 | WININ_WIN1_CLR;
  REG_WIN0H = WIN_RANGE(0, 240);
  REG_WIN1V = WIN_RANGE(16, 136);
  WaitFrames(1);
  BattleBackground_LoadFar(1, (s32)&ResourceId_DesertBackdrop, 0);
  BattleFx_WipeCanvas(1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_BlastSheet, work->sheet, 1, 1);
  EFFECT_SHOW_WIDE_CANVAS(16);
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, 50);
  base.x = 0xbc0000;
  base.y = 0x5c0000;
  orb.x = 0xa00000;
  orb.y = 0x5c0000;
  for (i = 0; i != RAMSES_ROCKS; i++) {
    struct EffectStep *rock = &work->particles[i];

    rock->x = (Random16() & 127) << 16;
    rock->y = i * -0x100000;
    rock->velocityX = 0;
    rock->velocityY = 0;
    rock->variant = 0;
  }
  for (i = 0; i != 58; i++)
    work->particles[RAMSES_FIRST_PUFF + i].variant = 24;
  EFFECT_FREE_PARTICLES(i, PARTICLES, 1024);
  work->fadeFrames = 24;
  work->fadeStep = 0;

  for (frame = 0; frame != RAMSES_FRAMES && !(gKeysRepeat & (KEY_A | KEY_B));
       frame++) {
    if (frame == 94)
      AudioCommand_PlayFar(156);
    if (frame == 136)
      AudioCommand_PlayFar(156);
    if (frame == 178)
      AudioCommand_PlayFar(156);
    if (frame == RAMSES_STRIKE_FRAME)
      AudioCommand_PlayFar(145);
    scale = unitScale;
    if (frame >= 96 && frame <= 251)
      work->shakeFrames = 1;
    else if (frame >= RAMSES_STRIKE_FRAME && frame <= 263)
      work->shakeFrames = 1;
    position.groundHeight = 0;
    position.height = 0;
    for (i = 0; i != 7; i++) {
      position.x = (gRamses_ObjectColumns[i] << 16) + base.x - 0x200000;
      position.y = (gRamses_ObjectRows[i] << 16) + base.y - 0x200000;
      Object_ApplyProjectedPlacementFar(work->objects[i], &position, &scale, 0);
    }
    if (frame <= 90) {
      orb.x = Trig_Sin(frame << 9) * 16 + 0x9c0000;
      orb.y = Trig_Cos(frame << 9) * 16 + 0x5c0000;
    }
    if (frame <= 196) {
      s32 start;

      for (i = 0, start = 91; i != 3; start += 40, i++) {
        if (frame >= start && frame < start + 4)
          orb.y += 0x80000;
        if (frame == start + 3) {
          for (k = 0; k != 4; k++) {
            struct EffectStep *puff =
                &work->particles[RAMSES_FIRST_PUFF + i * 8 + k];

            puff->x = 0x400000;
            puff->y = 0x600000;
            puff->velocityX = ((Random16() & 255) - 127) << 10;
            puff->velocityY = ((Random16() & 255) - 127) << 10;
            puff->variant = Random16() & 15;
          }
        }
        if (frame >= start + 20 && frame < start + 36)
          orb.y -= 0x20000;
      }
    }
    if (frame >= 244 && frame <= 251)
      orb.x -= 0x10000;
    if (frame >= 252 && frame <= 275)
      orb.x -= (frame - 250) << 16;
    if (frame <= 259) {
      position.x = orb.x;
      position.height = -0x1000000;
      position.y = orb.y - 0x1000000;
      Object_ApplyProjectedPlacementFar(work->objects[7], &position, &scale, 0);
      position.x = orb.x + 0x200000;
      Object_ApplyProjectedPlacementFar(work->objects[8], &position, &scale, 0);
    }
    position.height = 0;
    for (i = 0; i != RAMSES_ROCKS; i++) {
      struct EffectStep *rock = &work->particles[i];

      if (rock->variant != 2) {
        position.x = rock->x;
        position.y = rock->y;
        Object_ApplyProjectedPlacementFar(work->objects[9 + i], &position,
                                          &scale, 0);
        rock->x += rock->velocityX;
        rock->y += rock->velocityY;
        if (frame > 96)
          rock->velocityY += 0x4000;
        if (rock->y > 0x780000) {
          rock->variant++;
          if (rock->variant == 1) {
            rock->velocityY = -rock->velocityY / 2;
            for (k = 0; k != 2; k++) {
              struct EffectStep *puff =
                  &work->particles[RAMSES_FIRST_ROCK_PUFF + i * 2 + k];

              puff->x = rock->x / 2;
              puff->y = rock->y - 0x200000;
              puff->velocityX = ((Random16() & 255) - 127) << 10;
              puff->velocityY = ((Random16() & 255) - 127) << 10;
              puff->variant = Random16() & 15;
            }
          } else if (frame <= 199) {
            rock->y = 0;
            rock->velocityY = 0;
            rock->variant = 0;
          }
        }
      }
    }
    for (i = 0; i != 56; i++) {
      struct EffectStep *puff = &work->particles[RAMSES_FIRST_PUFF + i];

      if (puff->variant >= 0) {
        if (puff->variant >= 0 && puff->variant <= 23) {
          s32 cell = puff->variant / 6 + 3;

          draw[0](canvas, work->sheet + gRamses_PuffSheetOffsets[cell],
                  HI(puff->x) - gRamses_PuffSizes[cell] / 2,
                  HI(puff->y) - gRamses_PuffSizes[cell] / 2,
                  gRamses_PuffSizes[cell], gRamses_PuffSizes[cell]);
        }
        EffectStep_AdvanceWithGravity2D(puff, 60, -0x4000);
        puff->variant++;
      }
    }
    if (frame == RAMSES_STRIKE_FRAME) {
      EFFECT_FOR_EACH_TARGET(i, work->effect) {
        BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 4);
        ObjectGroup_UpdateMembers(work->effect->actors[i], SPRITE_COLOR_WHITE,
                                  -1, i, 8);
      }
      work->shakeFrames = 8;
    }
    if (frame == RAMSES_STRIKE_FRAME) {
      for (i = 0; i != RAMSES_SPARKS; i++) {
        struct EffectStep *spark = &PARTICLES[i];
        s32 speed;
        s32 angle;

        speed = 0x3ff;
        speed &= Random16();
        angle = Random16() & 0xffff;
        spark->x = 0x200000;
        spark->y = 0x5c0000;
        spark->velocityX = Trig_Sin(angle) * (speed + 32) >> 7;
        spark->velocityY = -(Trig_Cos(angle) * (speed + 32) << 1) >> 7;
        spark->variant = (Random16() & 15) + 32;
      }
    }
    for (i = 0; i != RAMSES_SPARKS; i++) {
      struct EffectStep *spark = &PARTICLES[i];

      if (spark->variant >= 0) {
        s32 size = (spark->variant >> 3) + 1;

        EFFECT_DRAW_MOTE(draw[i & 1], canvas, sheet, HI(spark->x), HI(spark->y),
                         size);
        EffectStep_AdvanceWithGravity2D(spark, 62, 0x1000);
        spark->variant--;
      }
    }
    Camera_ApplyShake(8, 8);
    EFFECT_END_FRAME(work);
  }

  BattleEventRuntime_BeginPhaseFar(134);
  BattleFx_SetupBlendedDisplay();
  for (i = 0; i != 15; i++)
    ResourceObject_ReleaseFar(work->objects[i]);
  EFFECT_FINISH_CANVAS();
}

#define DRAGON_CLOUD_FRAMES 150           // frames in the effect
#define DRAGON_CLOUD_STRIKE_FRAME 83      // the flame strikes the first target
#define DRAGON_CLOUD_TRAIL_CELL_BYTES 936 // bytes in each trail copy
#define DRAGON_CLOUD_FIRST_EMBER 8        // the first ember in work->particles
#define DRAGON_CLOUD_FIRST_FLASH 48       // the first flash in work->particles
#define DRAGON_CLOUD_TRAIL_CELLS 0x1680   // the trail picture in the sheet
#define DRAGON_CLOUD_PUFF_CELLS 0x3c00    // the puff pictures in the sheet

// Abilities: Dragon Cloud (kind 49).
//
// A flame leaves the user along a stored path with a fading trail of copies
// behind it, then strikes the first target on DRAGON_CLOUD_STRIKE_FRAME and
// bursts into embers and flashes. The trail's copies are darker versions of
// the trail picture, each DRAGON_CLOUD_TRAIL_CELL_BYTES long.
void BattleFx_RunDragonCloud(struct BattleEffectArgument *effect) {
  struct Point2D trail[20];
  struct EffectPosition position;
  DrawRectangle draw[2];
  void **heapCache;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 drift;
  s8 *path;
  s32 x;
  s32 y;
  u8 *sheet;
  struct BattleCamera *camera;
  s32 i;
  s32 j;

  EFFECT_GET_SLOTS(heapCache, cursor, work, canvas);
  drift = 0;
  x = 0;
  y = 0;
  sheet = EFFECT_SLOT(heapCache, HEAP_SLOT_BATTLE_SHEET);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  Resource_LoadAndDecompress((s32)&ResourceId_FirePillarSheetB, work->sheet, 1,
                             1);
  for (i = 1; i != 20; i++) {
    for (j = 0; j != DRAGON_CLOUD_TRAIL_CELL_BYTES; j++) {
      s32 shade = work->sheet[DRAGON_CLOUD_TRAIL_CELLS + j];

      if (i > 10) {
        shade = shade - i * 4 + 40;
        if (shade < 0)
          shade = 0;
        work->sheet[DRAGON_CLOUD_TRAIL_CELLS +
                    (i - 10) * DRAGON_CLOUD_TRAIL_CELL_BYTES + j] = shade;
      }
    }
  }
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_EmberStreakSheet,
                             &work->sheet[DRAGON_CLOUD_PUFF_CELLS], 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                             Ram_MapCellBuffer, 1, 0);
  BattleFx_FetchRectangleBlitters(work->effect->side, draw);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);

  for (j = 0; j != 40; j++) {
    struct EffectStep *ember = &work->particles[DRAGON_CLOUD_FIRST_EMBER + j];

    if (work->effect->side == 0)
      ember->x = -0x380000;
    else
      ember->x = 0x380000;
    ember->y = 0;
    ember->z = 0;
    EFFECT_SET_VELOCITY(ember, ((Random16() & 63) - 32) << 14,
                        (Random16() & 63) << 13,
                        ((Random16() & 63) - 32) << 14);
    ember->variant = 1;
  }
  for (j = 0; j != 16; j++) {
    struct EffectStep *flash = &work->particles[DRAGON_CLOUD_FIRST_FLASH + j];

    if (work->effect->side == 0)
      flash->x = -0x380000;
    else
      flash->x = 0x380000;
    flash->y = 0x140000;
    flash->z = 0;
    EFFECT_SET_VELOCITY(flash, ((Random16() & 63) - 32) << 14,
                        (Random16() & 63) << 12,
                        ((Random16() & 63) - 32) << 14);
    flash->variant = 0;
  }
  for (i = 0, j = -0x4000; i != 8; i++, j += 0x1000) {
    struct EffectStep *puff = &work->particles[i];

    if (work->effect->side == 1)
      puff->x = (Trig_Sin(j) * 24 >> 16) + 88;
    else
      puff->x = (-(Trig_Sin(j) * 24) >> 16) + 16;
    puff->y = (Trig_Cos(j) * 16 >> 16) + 40;
    puff->variant = -(i * 2);
  }
  path = Resource_GetTableEntry((s32)&ResourceId_DualStreamPath);

  for (frame = 0; frame != DRAGON_CLOUD_FRAMES; frame++) {
    camera = gCameraWork;
    if (frame == DRAGON_CLOUD_STRIKE_FRAME)
      BattleEventRuntime_BeginPhaseFar(134);
    if (frame == 0)
      AudioCommand_PlayFar(136);
    if (frame == 50)
      AudioCommand_PlayFar(136);
    if (work->effect->side == 0) {
      if (frame <= 63)
        camera->yaw -= 0x100;
    } else {
      if (frame <= 63)
        camera->yaw += 0x100;
    }
    BattlePres_SetupTransitionSceneFar(0, 0, 0, 100);
    if (frame <= 17) {
      s32 cell = frame / 3;

      draw[0](canvas,
              &work->sheet[DRAGON_CLOUD_PUFF_CELLS] +
                  gBattleFx_PuffArcCellSourceOffsets[cell],
              48, gBattleFx_PuffArcCellBiasY[cell] + 60,
              gBattleFx_PuffArcCellWidths[cell],
              gBattleFx_PuffArcCellHeights[cell]);
      draw[1](canvas,
              &work->sheet[DRAGON_CLOUD_PUFF_CELLS] +
                  gBattleFx_PuffArcCellSourceOffsets[cell],
              56, gBattleFx_PuffArcCellBiasY[cell] + 60,
              gBattleFx_PuffArcCellWidths[cell],
              gBattleFx_PuffArcCellHeights[cell]);
    }
    if (frame >= 18 && frame <= 58) {
      if (frame == 18) {
        x = (path[0] << 8) + (u8)path[1];
        y = (path[2] << 8) + (u8)path[3] + 16;
        path += 4;
      } else {
        x += path[0];
        y += path[1];
        path += 2;
      }
    }
    if (frame >= 78 && frame <= 118) {
      if (frame == 78) {
        x = -56;
        y = 48;
      } else {
        y -= 16;
      }
    }
    for (j = 19; j != 0; j--) {
      if (frame > j + 18 && frame <= j + 83) {
        trail[j].x = trail[j - 1].x;
        trail[j].y = trail[j - 1].y;
        if (j > 10)
          draw[0](canvas,
                  &work->sheet[DRAGON_CLOUD_TRAIL_CELLS +
                               (j - 10) * DRAGON_CLOUD_TRAIL_CELL_BYTES],
                  trail[j].x, trail[j].y, 24, 39);
        else
          draw[0](canvas, &work->sheet[DRAGON_CLOUD_TRAIL_CELLS], trail[j].x,
                  trail[j].y, 24, 39);
      }
    }
    EFFECT_VIEW_FROM_CAMERA(camera);
    if (frame >= 18 && frame <= DRAGON_CLOUD_STRIKE_FRAME) {
      s32 lift;

      if (work->effect->side == 1)
        position.x = 64 - x / 2;
      else
        position.x = x / 2 + 64;
      position.y = 60 - y;
      lift = (position.y - trail[0].y - 24) / 2;
      if (lift > 2)
        lift = 2;
      if (lift < -2)
        lift = -2;
      drift += lift;
      if (drift > 8)
        drift = 8;
      if (drift < -8)
        drift = -8;
      lift = drift / 4 + 2;
      trail[0].x = position.x - 12;
      trail[0].y = position.y - 20;
      draw[0](canvas, &work->sheet[lift * 0x480], trail[0].x - 6,
              trail[0].y - 2, 24, 48);
    }
    if (frame == DRAGON_CLOUD_STRIKE_FRAME) {
      work->shakeFrames = 8;
      ObjectGroup_UpdateMembers(work->effect->actors[0], SPRITE_COLOR_WHITE,
                                POSE_DAMAGE, 0, 8);
      BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 1);
    }
    if (frame > DRAGON_CLOUD_STRIKE_FRAME) {
      for (i = 0; i != 56; i++) {
        struct EffectStep *ember =
            &work->particles[DRAGON_CLOUD_FIRST_EMBER + i];

        if (ember->y >= 0) {
          s32 size;

          EffectPosition_ApplyBaseAndYOffset(&ember->x, &position);
          position.x >>= 1;
          if (position.depth <= 159)
            position.depth = 160;
          if (position.depth > 799)
            position.depth = 799;
          size = 9 - (position.depth - 160) / 64;
          if (i > 47) {
            if (ember->variant <= 11) {
              draw[0](canvas, Ram_MapCellBuffer + ember->variant / 2 * 0x800,
                      position.x - 16, position.y - 32, 32, 64);
              ember->variant++;
            }
          } else {
            EFFECT_DRAW_MOTE(draw[0], canvas, sheet, position.x, position.y,
                             size);
          }
          ember->x += ember->velocityX;
          ember->y += ember->velocityY;
          ember->z += ember->velocityZ;
          ember->velocityY -= 0x2000;
        }
      }
    }
    if (frame == 50) {
      work->shakeFrames = 12;
      ObjectGroup_UpdateMembers(work->effect->actors[0], SPRITE_COLOR_WHITE,
                                POSE_DAMAGE, 0, 8);
    }
    if (frame > 49) {
      for (i = 0; i != 8; i++) {
        struct EffectStep *puff = &work->particles[i];

        if (puff->variant >= 0 && puff->variant <= 11) {
          s32 cell = puff->variant / 2;

          draw[1](canvas,
                  &work->sheet[DRAGON_CLOUD_PUFF_CELLS] +
                      gBattleFx_PuffArcCellSourceOffsets[cell],
                  puff->x - gBattleFx_PuffArcCellWidths[cell] / 2,
                  puff->y + gBattleFx_PuffArcCellBiasY[cell],
                  gBattleFx_PuffArcCellWidths[cell],
                  gBattleFx_PuffArcCellHeights[cell]);
        }
        puff->variant++;
      }
    }
    Camera_ApplyShake(8, 8);
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS();
}

#define ANNIHILATION_FRAMES 64 // frames in the effect

// Abilities: Annihilation (kind 55).
//
// The user closes on the first target and fire bursts from it in a shower of
// shards. The scene runs for 64 frames:
//
//   4 on       64 shards scatter from the target and fall
//   6 to 11    a fire streak flares in front of the user
//   8          the target is struck; the canvas is filled
//   16 to 47   a fire burst grows over the target in three steps
//
// effect: the user and the target; a nonzero variant gives the burst its
// second picture and a sound.
void BattleFx_RunAnnihilation(struct BattleEffectArgument *effect) {
  void **heapCache;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  u8 *particleSheet;
  struct BattleCamera *camera;
  struct MotionObject *targetObject;
  struct EffectStep *shard;
  struct EffectPosition position;
  struct EffectPosition origin;
  struct EffectPosition out;
  DrawRectangle blitters[2];
  s32 i;
  s32 frame;
  s32 size;
  u8 *burstPixels;

  heapCache = &gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];
  cursor = heapCache;
  EFFECT_GET_WORK_AND_CANVAS(cursor, work, canvas);
  particleSheet = EFFECT_SLOT(heapCache, HEAP_SLOT_BATTLE_SHEET);
  camera = EFFECT_SLOT(heapCache, HEAP_SLOT_CAMERA);
  burstPixels = Ram_MapCellBuffer;
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);
  Resource_LoadAndDecompress((s32)&ResourceId_FireStreakSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_FireBurstSheet, burstPixels, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, particleSheet,
                             0, 0);
  BattleMotion_ApproachTargetFar(work->effect->actor, work->effect->actors[0],
                                 4, 0);
  WaitFrames(1);
  targetObject = GetBattleObjectSlotFar(work->effect->actors[0])->object;

  i = 0;
  do {
    struct EffectStep *seed = &work->particles[i];

    EFFECT_SET_POSITION(seed, targetObject->x, targetObject->y,
                        targetObject->z);
    EFFECT_SET_VELOCITY(seed, (Random16() & 255) << 11,
                        ((Random16() & 255) - 127) << 12,
                        ((Random16() & 255) - 127) << 12);
    if (seed->x > 0)
      seed->velocityX = -seed->velocityX;
    seed->variant = i / 4 * 2 + 16;
    i++;
  } while (i != 64);

  EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &origin);
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);

  for (frame = 0; frame != ANNIHILATION_FRAMES; frame++) {
    if (frame == 8)
      BattleEventRuntime_BeginPhaseFar(0x86);
    if (work->effect->variant != 0 && frame == 8)
      AudioCommand_PlayFar(0xd4);
    EffectPosition_ApplyStepAndYOffset(work->effect->actor, &position);
    if (frame >= 6 && frame <= 11) {
      if (work->effect->side == 0)
        EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER,
                            COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                            COMPOSITE_ADDITIVE);
      else
        EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER,
                            COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                                COMPOSITE_FLIP_X,
                            COMPOSITE_ADDITIVE);
      blitters[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
      if (work->effect->side == 0)
        blitters[0](canvas, work->sheet + (frame - 6) * 0xd80,
                    position.x / 2 - 24, position.y - 24, 48, 72);
      else
        blitters[0](canvas, work->sheet + (frame - 6) * 0xd80, position.x / 2,
                    position.y - 24, 48, 72);
      Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
    }
    if (frame >= 16 && frame <= 47) {
      s32 step = (frame - 16) / 2;
      s32 strip;

      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER,
                          COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, COMPOSITE_MAX);
      blitters[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
      if (step > 2)
        step = 2;
      if (work->effect->variant == 0)
        strip = 0;
      else
        strip = 0x2580;
      blitters[0](canvas, Ram_MapCellBuffer + strip + step * 0xc80,
                  origin.x / 2 - 20, origin.y - 48, 40, 80);
      BattleFx_RunNoEffectFrames(10000);
      Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
    }
    if (frame == 8) {
      Dma_Fill32(0x3f3f3f3f, canvas, 0x4000);
    }
    EFFECT_VIEW_FROM_CAMERA(camera);
    if (frame > 3) {
      BattleFx_FetchRectangleBlitters(work->effect->side, blitters);
      for (i = 0; i != 64; i++) {
        s32 life;

        shard = &work->particles[i];
        life = shard->variant;
        if (life > 0) {
          EffectPosition_ApplyBaseAndYOffset(shard, &out);
          out.x >>= 1;
          out.y = out.y + origin.y - 112;
          size = (life >> 3) + 2;
          blitters[(i / 2) & 1](
              canvas, particleSheet + gBattleFx_MoteCellOffsets[size - 1],
              out.x - size / 2, out.y - size, size, size * 2);
          EffectStep_AdvanceWithGravity3D(shard, 60, -0x400);
          shard->variant--;
        }
      }
      EFFECT_RELEASE_BLITTERS();
    }
    if (frame == 8) {
      BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 4);
      work->shakeFrames = 4;
    }
    if (frame == 6)
      ObjectGroup_UpdateMembers(work->effect->actors[0], SPRITE_COLOR_RED_TINT,
                                -1, -1, 0);
    if (frame == 14)
      ObjectGroup_UpdateMembers(work->effect->actors[0], SPRITE_COLOR_RED_TINT,
                                -1, -1, 0);
    Camera_ApplyShake(16, 16);
    EFFECT_END_FRAME(work);
  }
  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  BattleFx_EndCanvasLayer();
}

// The flame blade (Ragnarok) and the falling sword (Titan Blade): a huge
// sword cuts or drops onto the targets amid flashes, sparks and dust.
//
// Both scenes draw a large sword picture onto the effect canvas and time
// everything else by frame number: sounds, screen shakes, palette fades and
// the targets' reactions each happen on a fixed frame.
//
// - Ragnarok swings a 64x128 blade down over 10 frames, pauses until frame
//   20 and then creeps on until frame 39. Two columns of light strike at
//   frames 16 and 24: each is a large flash, a ring of 12 small flashes and
//   256 sparks that bounce on the ground. The targets recoil as each
//   column's sparks fly.
// - Titan Blade raises embers from frame 28 while a pillar of fire scrolls
//   over the target. The sword then drops onto the target, a ring of
//   flashes bursts from frame 56, sparks fly from frame 60 and the embers
//   swell as they clear. The canvas is wiped at frames 9 and 60.
//
// Flashes live in work->particles and the sword pictures in the work sheet.
// Sparks and embers share PARTICLES: Ragnarok keeps 340 sparks per column
// there, Titan Blade 684 embers and then its sparks.

#define FLAME_BLADE_FLASH_CELLS                                                \
  (64 * 128) // Place flash graphics after the 64x128 sword image.

#define RAGNAROK_FRAMES 80        // frames in the effect
#define RAGNAROK_COLUMNS 2        // columns that strike
#define RAGNAROK_SEEDED 3         // columns given particles
#define RAGNAROK_FLASHES 16       // flashes kept per column
#define RAGNAROK_RING 12          // of which drawn as the ring
#define RAGNAROK_SPARKS 340       // sparks kept per column
#define RAGNAROK_DRAWN_SPARKS 256 // of which flown

// gRagnarok_StrikeColumns has three columns for each side.
#define RAGNAROK_COLUMN_X(side, column)                                        \
  gRagnarok_StrikeColumns[(column) + (side) * 3]

// Abilities: Ragnarok (kind 58).
//
// Draw the sword for 40 frames, swinging down over 10 frames. Flash the
// two columns at frames 16 and 24; scatter sparks six frames later.
// Column x comes from gRagnarok_StrikeColumns: first two of three per side.
// PARTICLES holds 340 sparks per column.
void BattleFx_RunRagnarok(struct BattleEffectArgument *effect) {
  void **heapCache;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 col;
  u8 *sheet;
  struct MotionObject *user;
  DrawRectangle blit[2];
  s32 frame;
  s32 i;

  EFFECT_GET_SLOTS(heapCache, cursor, work, canvas);
  sheet = EFFECT_SLOT(heapCache, HEAP_SLOT_BATTLE_SHEET);

  user = GetBattleObjectSlotFar(effect->actor)->object;
  effect->variant = 1;
  work->effect = effect;
  BattleFx_BeginCanvasLayer(1);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  BattleFx_FetchRectangleBlitters(work->effect->side, blit);
  ObjectDispatch_ApplyArgumentToChildrenFar(user, 2);
  ObjectDispatch_ApplyValueToChildrenFar(user, 48);

  Resource_LoadAndDecompress((s32)&ResourceId_FlameBladeSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                             work->sheet + FLAME_BLADE_FLASH_CELLS, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);

  //------------------------------------------------------------------
  // Prepare 16 flashes and 340 sparks per column.
  //------------------------------------------------------------------
  col = 0;
  do {
    // Flashes: i * 2 pixels from the center, lifetime 25-32.
    i = 0;
    do {
      struct EffectStep *flash = &work->particles[col * RAGNAROK_FLASHES + i];

      s32 radius = i * 2;
      s32 angle = Random16() & 0xffff;

      flash->x = Trig_Sin(angle) * radius;
      flash->y = -(Trig_Cos(angle) * radius);
      flash->variant = i / 2 + 25;
      i++;
    } while (i != RAGNAROK_FLASHES);

    // Sparks: random direction and speed from column y = 88, lifetime 32-39.
    i = 0;
    do {
      struct EffectStep *spark = &PARTICLES[col * RAGNAROK_SPARKS + i];

      s32 speed;
      s32 angle;

      speed = (Random16() & 0x1ff) + 32;
      angle = Random16() & 0xffff;
      spark->x = RAGNAROK_COLUMN_X(work->effect->side, col) << 16;
      spark->y = 88 << 16;
      spark->velocityX = (Trig_Sin(angle) * speed) >> 6;
      spark->velocityY = -(Trig_Cos(angle) * speed * 2) >> 6;
      spark->variant = (Random16() & 7) + 32;
      i++;
    } while (i != RAGNAROK_SPARKS);
    col++;
  } while (col != RAGNAROK_SEEDED);

  work->transferMode = CANVAS_COPY_AND_FADE;
  work->transferValue = 75; // Reduce to 3/4 on each transfer for an afterimage.
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);

  frame = 0;
  do {
    if (frame == 4) {
      AudioCommand_PlayFar(212);
    }
    if (frame == 8) {
      work->shakeFrames = frame;
    }
    if (frame == 18) {
      AudioCommand_PlayFar(145);
    }
    if (frame == 40) {
      BattleEventRuntime_BeginPhaseFar(134);
    }

    //------------------------------------------------------------------
    // Swing down over 10 frames, pause until frame 20, then advance slowly.
    // Do not draw below y = 104.
    //------------------------------------------------------------------
    if (frame <= 39) {
      s32 height;
      s32 x;
      s32 top;

      height = 128;
      if (work->effect->side == 1) {
        if (frame <= 9) {
          x = frame * 10 - 8;
          top = frame * 16 - 128;
        } else if (frame > 20) {
          x = frame + 62;
          top = frame * 2 - 24;
        } else {
          x = 82;
          top = 16;
        }
      } else {
        if (frame <= 9) {
          x = 128 - frame * 10;
          top = frame * 16 - 128;
        } else if (frame > 20) {
          x = 58 - frame;
          top = frame * 2 - 24;
        } else {
          x = 38;
          top = 16;
        }
      }
      if (top + 128 > 104) {
        height = height - top - 24;
      }
      if (height > 0) {
        blit[0](canvas, work, x - 32, top, 64, height);
      }
    }

    if (frame > 16) {
      BattleFx_StepPaletteToResource((s32)&ResourceId_BlastSheet);
    }

    //------------------------------------------------------------------
    // Two columns: flashes at start, sparks at start + 6.
    //------------------------------------------------------------------
    col = 0;
    do {
      s32 start = col * 8 + 16;

      if (frame == start) {
        work->shakeFrames = 12;
      }
      if (frame >= start) {
        // Large flash for the first two frames, then 12 small flashes.
        if (frame < start + 2) {
          blit[0](canvas, work->sheet + FLAME_BLADE_FLASH_CELLS,
                  RAGNAROK_COLUMN_X(work->effect->side, col) - 16, 56, 32, 64);
        }
        i = 0;
        do {
          struct EffectStep *flash =
              &work->particles[col * RAGNAROK_FLASHES + i];

          s32 x;
          s32 life;
          s32 y;

          y = HI(flash->y);
          x = HI(flash->x) + RAGNAROK_COLUMN_X(work->effect->side, col);
          life = flash->variant;
          if (life >= 0 && life <= 17) {
            blit[0](canvas,
                    work->sheet + (gRagnarok_FlashCells[life / 3] << 11) +
                        FLAME_BLADE_FLASH_CELLS,
                    x - 16, y + 56, 32, 64);
          }
          life = flash->variant;
          flash->variant = life > 0 ? life - 1 : -1;
          i++;
        } while (i != RAGNAROK_RING);
      }

      // Sparks bounce below y = 108 at half speed; draw only within x = 0-126.
      if (frame > start + 5) {
        i = 0;
        do {
          struct EffectStep *spark = &PARTICLES[col * RAGNAROK_SPARKS + i];

          if (spark->variant > 0) {
            s32 x;
            s32 y;
            s32 size;

            EffectStep_AdvanceWithGravity2D(spark, 64, 0x1000);
            spark->variant--;
            y = spark->y;
            if (y > (108 << 16)) {
              spark->velocityY = -spark->velocityY / 2;
            } else {
              x = spark->x;
              if ((x >= 0 && x <= 0x7effff) && y >= 0) {
                u8 *cel;
                s32 lane;
                s32 py = y >> 16;
                s32 px = x >> 16;

                // Size follows lifetime; alternate comparison and additive
                // compositing.
                size = spark->variant / 5 + 1;
                lane = i & 1;
                cel = sheet + gBattleFx_MoteCellOffsets[size - 1];
                px -= size / 2;
                py -= size;
                blit[lane](canvas, cel, px, py, size, size * 2);
              }
            }
          }
          i++;
        } while (i != RAGNAROK_DRAWN_SPARKS);
      }

      EFFECT_FOR_EACH_TARGET(i, work->effect) {
        if (frame == start + 6) {
          ObjectGroup_UpdateMembers(work->effect->actors[i], SPRITE_COLOR_WHITE,
                                    POSE_DAMAGE, i, 10);
          BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 4);
        }
      }
      col++;
    } while (col != RAGNAROK_COLUMNS);

    Camera_ApplyShake(16, 16);
    EFFECT_END_FRAME(work);
    frame++;
  } while (frame != RAGNAROK_FRAMES);

  ObjectDispatch_ApplyValueToChildrenFar(user, 16);
  EFFECT_FINISH_CANVAS();
}

#define FALLING_SWORD_SPARKS                                                   \
  ((struct EffectStep *)(Ram_MapCellBuffer +                                   \
                         TITAN_BLADE_DUST * sizeof(struct EffectStep)))

#define TITAN_BLADE_FRAMES 102        // frames in the effect
#define TITAN_BLADE_SWORD_IMAGE 20000 // the sword picture in the sheet
#define TITAN_BLADE_FLASHES 32        // flashes scattered
#define TITAN_BLADE_RING 16           // of which drawn as the ring
#define TITAN_BLADE_DUST 684       // dust slots in PARTICLES, before the sparks
#define TITAN_BLADE_FIRST_DUST 256 // dust lit at once on frame 28
#define TITAN_BLADE_DRAWN_DUST 342 // dust moved and drawn
#define TITAN_BLADE_SPARKS 340     // sparks after the dust

// Abilities: Titan Blade (kind 61).
//
// Drop a sword onto the first target amid charging flashes, dust and sparks.
void BattleFx_RunTitanBlade(struct BattleEffectArgument *effect) {
  void **heap;
  void **slotCursor;
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle blitters[2];
  u8 *sheet;
  struct EffectPosition screenPosition;
  s32 frame;
  s32 canvasCenterX;
  s32 step;
  s32 sparkOriginX;
  s32 i;

  EFFECT_GET_SLOTS(heap, slotCursor, work, canvas);
  sheet = heap[2];

  work->effect = effect;
  BattleFx_BeginCanvasLayer(1);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  EffectPosition_ApplyAlternateStepAndYOffset(work->effect->actors[0],
                                              &screenPosition);
  canvasCenterX = screenPosition.x / 2;

  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                      COMPOSITE_MAX);
  blitters[0] = heap[7];
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                      COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                      COMPOSITE_TRANSPARENT);
  blitters[1] = heap[8];

  Resource_LoadAndDecompress((s32)&ResourceId_SwordSheet,
                             work->sheet + TITAN_BLADE_SWORD_IMAGE, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_FirePillarSheetA, work->sheet, 1,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                             work->sheet + (221 << 4), 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);

  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);

  i = 0;
  do {
    struct EffectStep *flash = &work->particles[i];

    s32 scale = i * 2;
    s32 ang = Random16() & 0xFFFF;

    flash->x = Trig_Sin(ang) * scale;
    flash->y = -(Trig_Cos(ang) * scale);
    flash->variant = i / 2 + 25;
    i++;
  } while (i != TITAN_BLADE_FLASHES);

  i = 0;
  do {
    PARTICLES[i].variant = -1;
    i++;
  } while (i != TITAN_BLADE_DUST);

  i = 0;
  sparkOriginX = canvasCenterX << 16;
  do {
    struct EffectStep *spark = &FALLING_SWORD_SPARKS[i];

    s32 mag;
    s32 ang;

    /* FAKEMATCH: the mask is loaded into the magnitude before the random word
     * is taken in, as the native code does (ldr r6, =0x1ff; ands r6, r0).
     * 2026-10-13: Random16() & 0x1FF, 0x1FF & Random16() and mag = Random16()
     * followed by mag &= 0x1FF all differ 41 of 779, 780 instructions: the and
     * takes the random word as its destination, copied into r6, with the mask
     * in r3, and the pool offsets shift. */
    mag = 0x1FF;
    mag &= Random16();
    ang = Random16() & 0xFFFF;
    spark->x = sparkOriginX;
    spark->y = 176 << 15;
    spark->velocityX = (Trig_Sin(ang) * (mag + 32)) >> 5;
    spark->velocityY = -(Trig_Cos(ang) * (mag + 32)) >> 6;
    spark->variant = (Random16() & 7) + 32;
    i++;
  } while (i != TITAN_BLADE_SPARKS);

  frame = 0;
  do {
    if (frame >= 25 && frame <= 47) {
      BattleFx_StepPaletteToResource((s32)&ResourceId_BlastSheet);
    }
    if (frame > 56) {
      BattleFx_StepPaletteToResource((s32)&ResourceId_LightningBoltSheet);
    }

    if (frame == 8) {
      work->shakeFrames = frame;
    }
    if (frame == 48) {
      work->shakeFrames = 8;
    }
    if (frame == 60) {
      work->shakeFrames = 16;
    }

    if (frame == 4) {
      AudioCommand_PlayFar(212);
    }
    if (frame == 32) {
      AudioCommand_PlayFar(164);
    }
    if (frame == 60) {
      AudioCommand_PlayFar(145);
      BattleEventRuntime_BeginPhaseFar(134);
    }

    if (frame > 55) {
      i = 0;
      do {
        struct EffectStep *flash = &work->particles[i];

        s32 x;
        s32 life;
        s32 y;

        y = HI(flash->y);
        x = HI(flash->x) + canvasCenterX;
        life = flash->variant;
        if (life >= 0 && life <= 17) {
          blitters[0](canvas,
                      work->sheet + (gTitanBlade_FlashCells[life / 3] << 11) +
                          (221 << 4),
                      x - 16, y + 48, 32, 64);
        }
        life = flash->variant;
        flash->variant = life > 0 ? life - 1 : -1;
        i++;
      } while (i != TITAN_BLADE_RING);
    }

    if (frame == 28) {
      i = 0;
      do {
        struct EffectStep *dust = &PARTICLES[i];

        if (dust->variant == -1) {
          s32 mag;
          s32 ang;

          mag = Random16() & 63;
          ang = Random16() & 0xFFFF;
          dust->x = ((Trig_Sin(ang) * mag) >> 3) + sparkOriginX;
          dust->y = ((Trig_Cos(ang) * mag) >> 2) + (192 << 15);
          dust->velocityX = ((Random16() & 63) - 32) << 14;
          dust->velocityY = (-(Random16() & 63) - 8) << 13;
          dust->variant = 0;
        }
        i++;
      } while (i != TITAN_BLADE_FIRST_DUST);
    }

    step = frame - 32;
    if (step >= 0 && step <= 31) {
      s32 cnt;

      cnt = 0;
      i = 0;
      do {
        struct EffectStep *dust = &PARTICLES[i];

        if (dust->variant == -1) {
          s32 mag;
          s32 ang;

          mag = Random16() & 63;
          ang = Random16() & 0xFFFF;
          dust->x = ((Trig_Sin(ang) * mag) >> 3) + sparkOriginX;
          dust->y = ((Trig_Cos(ang) * mag) >> 2) + (192 << 15);
          dust->velocityX = ((Random16() & 63) - 32) << 14;
          dust->velocityY = (-(Random16() & 63) - 8) << 13;
          dust->variant = 0;
          cnt++;
          if (cnt == 16) {
            break;
          }
        }
        i++;
      } while (i != TITAN_BLADE_DUST);
    }

    if (step >= 0 && step <= 31) {
      s32 phase = frame * 16;
      s32 x = canvasCenterX - 17;
      s32 width = 34;
      DrawRectangle blit = blitters[0];
      s32 high = 104;
      s32 size = (phase - 256) % high;

      blit(canvas, work, x, 4 - size, width, high);
      blit(canvas, work, x, 108 - size, width, size);
    }

    if (frame <= 71) {
      i = 0;
      do {
        struct EffectStep *dust = &PARTICLES[i];

        if (dust->variant >= 0) {
          s32 size;

          size = i % 3 + 2;
          if (dust->velocityY > 0) {
            size += 2;
          }
          if (frame > 68 && size <= 5) {
            size = 6;
          }
          if (frame > 70 && size <= 6) {
            size = 7;
          }
          if (frame > 72 && size <= 7) {
            size = 8;
          }
          if (frame > 74 && size <= 8) {
            size = 9;
          }
          if (frame > 76) {
            size = 10;
          }
          EFFECT_DRAW_MOTE(blitters[0], canvas, sheet, HI(dust->x), HI(dust->y),
                           size);

          dust->x += dust->velocityX;
          dust->y += dust->velocityY;
          if (frame > 80) {
            dust->velocityY += -32768;
          } else {
            dust->velocityY += gTitanBlade_DustGravity[i & 3];
          }
          dust->velocityX = dust->velocityX * 62 / 64;
          dust->velocityY = dust->velocityY * 62 / 64;
          dust->variant++;
          if (dust->velocityY > 0 && HI(dust->y) > 108) {
            dust->variant = -1;
          }
        }
        i++;
      } while (i != TITAN_BLADE_DRAWN_DUST);
    }

    if (frame <= 95) {
      s32 x;
      s32 high;
      s32 top;

      x = canvasCenterX - 18;
      high = 120;
      if (frame > 60) {
        top = frame * 8 - 450;
      } else if (frame > 32) {
        top = step / 2 + 16;
      } else if (frame <= 9) {
        top = frame * 16 - 128;
      } else {
        top = 16;
      }
      if (top + 120 > 108) {
        high = high - top - 12;
      }
      if (high > 0) {
        blitters[1](canvas, work->sheet + TITAN_BLADE_SWORD_IMAGE, x, top, 36,
                    high);
      }
    }

    if (frame > 59) {
      i = 0;
      do {
        struct EffectStep *spark = &FALLING_SWORD_SPARKS[i];

        if (spark->variant > 0) {
          s32 x;
          s32 y;
          s32 size;

          EffectStep_AdvanceWithGravity2D(spark, 64, 128 << 6);
          y = spark->y;
          spark->variant--;
          if (y > (216 << 15)) {
            spark->velocityY = -spark->velocityY / 2;
          } else {
            x = spark->x;
            if ((x >= 0 && x <= 0x7effff) && y >= 0) {
              u8 *cell;

              size = spark->variant / 5 + 1;
              cell = sheet + gBattleFx_MoteCellOffsets[size - 1];
              x >>= 16;
              y >>= 16;
              x -= size / 2;
              y -= size;
              blitters[0](canvas, cell, x, y, size, size * 2);
            }
          }
        }
        i++;
      } while (i != TITAN_BLADE_SPARKS);
    }

    if (frame == 68) {
      EFFECT_FOR_EACH_TARGET(i, work->effect) {
        ObjectGroup_UpdateMembers(work->effect->actors[i], SPRITE_COLOR_WHITE,
                                  POSE_DAMAGE, i, 16);
        BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 7);
      }
    }

    if (frame == 9) {
      Iwram_FillWords(canvas, 128 << 7, 0x3F3F3F3F);
    }
    if (frame == 60) {
      Iwram_FillWords(canvas, 128 << 7, 0x3F3F3F3F);
    }

    Camera_ApplyShake(16, 16);
    EFFECT_END_FRAME(work);
    frame++;
  } while (frame != TITAN_BLADE_FRAMES);

  EFFECT_FINISH_CANVAS();
}

// The shockwave scene (Judgment): a gathering formation, its burst, and a
// sweep of debris across three backdrops.
//
// The scene runs in two parts, each with its own frame clock.
//
// - The gathering, on the violet sky backdrop: a projected formation of
//   sixteen objects moves forward while rotating motes shrink in on its
//   centre. A burst then grows there, shards fly from it, and a ring of
//   dimming motes spreads outward. It runs SHOCKWAVE_GATHER_FRAMES frames.
// - The sweep, SHOCKWAVE_SWEEP_FRAMES frames: the formation gives way to
//   debris objects and the scene cuts to an aerial backdrop, then to beach
//   and forest ones, swapping palettes, backdrop and debris on set frames.
//   Meanwhile expanding elliptical rings are drawn into a wave picture that
//   persists in work, explosion cells fan out from the impact, and the
//   debris objects rise and are recycled behind a scrolling explosion front.
//
// Both parts skip ahead on A or B, by different rules: the gathering simply
// ends after frame 4, while the sweep jumps to its next transition frame
// (150, 214 or 280).
//
// Particle slots: the gathering's motes use PARTICLES 0 to 127 and its
// sparks 128 to 255 (SHOCKWAVE_SPARKS). The sweep's debris use particles 0
// to 15, one for each debris object.
//
// The work block is laid out as ShockwaveWork: the sheet's pixels first (the
// sweep's wave picture is its first 120x120 bytes), then eight dimmed copies
// of the mote sheet.

struct ShockwaveWork {
  u8 sheet[0x2710];
  u8 ramp[8][770];
};

extern u16 gShockwaveScene_ShardCells[];
extern u8 gShockwaveScene_ShardWidths[];
extern u8 gShockwaveScene_ShardHeights[];

#define SHOCKWAVE_SPARKS (&PARTICLES[128])

#define SHOCKWAVE_GATHER_FRAMES 160    // frames of the gathering
#define SHOCKWAVE_SWEEP_FRAMES 320     // frames of the sweep
#define SHOCKWAVE_SHEET_FLARE_B 0x4240 // the second flare picture in the sheet
#define SHOCKWAVE_FORMATION_OBJECTS 16 // objects in the formation
#define SHOCKWAVE_DEBRIS_OBJECTS 16    // debris objects of the sweep
#define SHOCKWAVE_SHEET_BURST 0x600    // the burst pictures in the sheet
#define SHOCKWAVE_SHEET_EXPLOSION 0x4000 // the explosion pictures in the sheet
#define SHOCKWAVE_SHEET_FLARE_A 0x4000   // the first flare picture in the sheet
// The small flash picture in the sheet.
#define SHOCKWAVE_SHEET_FLASH_SMALL 0x6000
// The large flash picture in the sheet.
#define SHOCKWAVE_SHEET_FLASH_LARGE 0x6110
// The flash streak picture in the sheet.
#define SHOCKWAVE_SHEET_FLASH_STREAK 0x64e8
#define SHOCKWAVE_EXPLOSION_CELL_BYTES 1728 // bytes in each explosion picture
#define SHOCKWAVE_WAVE_SIZE 120             // the wave picture is 120x120
#define SHOCKWAVE_DMA_FILL_CANVAS                                              \
  DMA_FILL_WORDS(0x1000) // DMA word that clears the canvas
// DMA word that clears the wave picture.
#define SHOCKWAVE_DMA_FILL_WAVE DMA_FILL_WORDS(0xe10)

// Abilities: Judgment (scene kind 9).
//
// Stage a formation above the violet backdrop, then pulse and scatter its
// images.
void BattleFx_RunJudgment(struct BattleEffectArgument *effect) {
  static struct SpriteScale unitScale = {{{0x10000, 0x10000}}};
  s32 anchor[3];
  struct EffectPosition projectedPosition;
  struct SpritePlacement place;
  volatile u32 fill;
  struct SpritePlacement debrisPlace;
  struct SpriteScale pair;
  struct SpriteScale scale;
  void *canvas;
  struct BattleEffectWork *work;
  struct BattlePresentationTransition *ctrl;
  DrawRectangle draw[2];
  s32 frame;
  u8 *ramp;
  struct BattleCamera *camera;
  s32 depth;
  s32 zoom;
  s32 minimumDepth;
  s32 maximumDepth;
  void **cache;
  DrawRectangle *slots;
  s32 cx;
  s32 cy;
  s32 squash;
  s32 sx;
  s32 ring;
  s32 rings;
  s32 shift;
  s32 i;
  s32 j;
  struct EffectStep *particle;
  DrawRectangle blit;
  volatile u16 *ime;

  cache = &gWorkSlot[HEAP_SLOT_BATTLE_CANVAS];
  ime = (u16 *)&REG_IME;
  canvas = cache[0];
  work = CANVAS_SLOT(cache, HEAP_SLOT_BATTLE_EFFECT);
  ctrl = CANVAS_SLOT(cache, HEAP_SLOT_BATTLE_BACKGROUND);
  ramp = CANVAS_SLOT(cache, HEAP_SLOT_BATTLE_SHEET);
  camera = CANVAS_SLOT(cache, HEAP_SLOT_CAMERA);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0x2000);
  REG_BG2PA = Q_8_8(1);
  BattleFx_ConfigureEffectDisplay();
  PLTT_BACKDROP = 0;
  *(u16 *)&BG_PLTT[1] = 0;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                      COMPOSITE_ADDITIVE);
  draw[0] = (DrawRectangle)CANVAS_SLOT(cache, HEAP_SLOT_BLITTER);
  work->transferMode = CANVAS_COPY;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);
  BattleFx_WipeCanvas(0, 0);
  BattleFx_SelectLivingTargets(work->effect);
  BattleFx_SpawnObjects(SHOCKWAVE_FORMATION_OBJECTS, 0x17e, 1);
  gProjection.centerY = 240;
  WaitFrames(1);
  BattleBackground_LoadFar(1, (s32)&ResourceId_VioletSkyBackdrop, 0);
  ctrl->active = 1;
  gBgScroll[1].x = 0;
  BattleFx_WipeCanvas(0, 1);
  EFFECT_SHOW_WIDE_CANVAS(16);
  depth = 0xffc00000;
  zoom = 0x80000;
  minimumDepth = 0xffff;
  maximumDepth = 0;
  Resource_LoadAndDecompress((s32)&ResourceId_PinkBurstSheet, work, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_OverlayImageB,
                             work->sheet + SHOCKWAVE_SHEET_BURST, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, work->sheet + 0x95c,
                             1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, ramp, 0, 0);

  for (i = 0; i != 8; i++) {
    u8 *src = ramp;
    for (j = 0; j != 770; j++) {
      s32 v = *src++;
      if (v > 64 - i * 7) {
        v = 64 - i * 7;
      }
      if (v < 0) {
        v = 0;
      }
      ((struct ShockwaveWork *)work)->ramp[i][j] = v;
    }
  }

  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_BlueCrescentSheet),
                  128);

  for (i = 0; i != 128; i++) {
    PARTICLES[i].x = (Random16() & 15) + 72;
    PARTICLES[i].velocityX = Random16() & 0xffff;
    PARTICLES[i].velocityY = Random16() & 0xffff;
    PARTICLES[i].velocityY = Random16() & 0xffff;
  }

  for (i = 0; i != 128; i++) {
    s32 angle = Random16() & 0xffff;
    /* FAKEMATCH: a form the bytes prove: the amplitude is a value, not a
     * literal: the native hoists 255 into sl and multiplies with muls.
     * 2026-10-13: 255 written at both multiplies differs 142 of 2455; amp and
     * speed set before the loop differ 2 (i = 0 is scheduled after them), set
     * after i = 0 or in the for initializer 0, the same bytes as here. */
    s32 amp = 255;
    /* FAKEMATCH: register choice: the velocities' zero is a value set at the
     * top of the body, which loop.c hoists into r8 as the native's. 2026-10-13:
     * 0 stored directly differs 138 of 2455; see amp for the forms set before
     * the loop. */
    s32 speed = 0;
    SHOCKWAVE_SPARKS[i].x = (Trig_Sin(angle) * amp) >> 2;
    SHOCKWAVE_SPARKS[i].z = (Trig_Cos(angle) * amp) >> 2;
    SHOCKWAVE_SPARKS[i].y = ((Random16() & 63) + 32) << 16;
    SHOCKWAVE_SPARKS[i].velocityX = speed;
    SHOCKWAVE_SPARKS[i].velocityY = speed;
    SHOCKWAVE_SPARKS[i].variant = angle;
  }

  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, 50);
  REG_BG2CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7);

  for (frame = 0; frame != SHOCKWAVE_GATHER_FRAMES &&
                  (frame <= 4 || !(gKeysRepeat & (KEY_A | KEY_B)));
       frame++) {
    /* FAKEMATCH: needed to match: the frame > 143 block calls through a pointer
     * to the slot table (base kept at sp+56) 2026-10-11: set once before the
     * frame loop differs 22 (the preheader reload registers rotate); as a local
     * of the frame > 143 block 579; the slot table named at the uses 581. */
    slots = (DrawRectangle *)gWorkSlot;
    if (frame == 143) {
      Iwram_FillWords(canvas, 0x4000, 0x2a2a2a2a);
      AudioCommand_PlayFar(145);
    }
    if (frame == 80) {
      AudioCommand_PlayFar(142);
    }

    // Retain the disabled particle stage: this loop starts at frame zero.
    if (frame < 0) {
      EFFECT_VIEW_FROM_CAMERA(camera);
      for (i = 0; i != 128; i++) {
        s32 z;
        s32 size;

        EffectStep_AdvanceWithGravity3D(&SHOCKWAVE_SPARKS[i], 60, -0x400);
        EffectPosition_ApplyBaseAndYOffset(&SHOCKWAVE_SPARKS[i],
                                           &projectedPosition);
        projectedPosition.x >>= 1;
        projectedPosition.y -= 120;
        if (projectedPosition.depth < 100) {
          projectedPosition.depth = 100;
        }
        if (projectedPosition.depth > 900) {
          projectedPosition.depth = 900;
        }
        z = projectedPosition.depth - 100;
        projectedPosition.depth = z;
        size = (z / 100) + 1;
        if (minimumDepth > z) {
          minimumDepth = z;
        }
        if (maximumDepth < z) {
          maximumDepth = z;
        }
        EFFECT_DRAW_MOTE(draw[0], canvas, ramp, projectedPosition.x,
                         projectedPosition.y, size);
        if (frame == 64) {
          SHOCKWAVE_SPARKS[i].velocityX +=
              (Trig_Sin(SHOCKWAVE_SPARKS[i].variant) * 255) >> 3;
          SHOCKWAVE_SPARKS[i].velocityY +=
              (Trig_Cos(SHOCKWAVE_SPARKS[i].variant) * 255) >> 3;
        }
      }
    }

    if (frame == 72) {
      work->fadeFrames = 24;
      work->fadeStep = 0;
    }

    anchor[2] = 0;
    anchor[1] = 0;
    for (i = 0; i != 64; i++) {
      if (frame > i / 4 + 80 && PARTICLES[i].x > 0) {
        s32 size;

        Render_ResetTransformState();
        SceneTransform_ApplyRoll(PARTICLES[i].velocityY);
        SceneTransform_ApplyPitch(PARTICLES[i].velocityX);
        SceneTransform_ApplyYaw(PARTICLES[i].velocityY + (frame << 9));
        anchor[0] = PARTICLES[i].x;
        PARTICLES[i].x -= 2;
        if (PARTICLES[i].x < 0) {
          PARTICLES[i].x = 0;
        }
        EffectPosition_ApplyBaseAndYOffset(anchor, &projectedPosition);
        if (projectedPosition.depth < -60) {
          projectedPosition.depth = -60;
        }
        if (projectedPosition.depth > 60) {
          projectedPosition.depth = 60;
        }
        projectedPosition.depth += 60;
        size = projectedPosition.depth / 20 + 2;
        projectedPosition.x += 60;
        projectedPosition.y += 80;
        EFFECT_DRAW_MOTE(draw[0], canvas, ramp, projectedPosition.x,
                         projectedPosition.y, size);
      }
    }

    pair = unitScale;
    place.groundHeight = 0;
    place.height = 0;
    for (i = 0; i != SHOCKWAVE_FORMATION_OBJECTS; i++) {
      place.x = ((i % 4) << 21) + (152 << 15);
      place.y = ((i / 4) << 21) + depth;
      Object_ApplyProjectedPlacementFar(work->objects[i], &place, &pair, 0);
    }

    depth += zoom;
    if (frame <= 47) {
      zoom -= 0x4000;
    }
    if (frame > 32) {
      zoom = zoom * 60 / 64;
    }
    if (frame == 144) {
      zoom = -0x80000;
    }
    if (frame == 146) {
      zoom = 0x10000;
    }

    if (frame == 72) {
      for (i = 0; i != SHOCKWAVE_FORMATION_OBJECTS; i++) {
        AnimationObjects_SelectAnimationFar(work->objects[i], i + 16);
      }
    }
    if (frame == 76) {
      for (i = 0; i != SHOCKWAVE_FORMATION_OBJECTS; i++) {
        AnimationObjects_SelectAnimationFar(work->objects[i], i + 32);
      }
    }

    if (frame >= 116 && frame < 144) {
      s32 high;
      s32 cell;
      s32 n = (frame - 116) / 4;
      if (n > 6) {
        n = 6;
      }
      // A shared load/draw/release helper grew this runner by eight bytes
      // in English (0x167c to0x1684); keep these four explicit draws.
      high = gShockwaveScene_BurstHeights[n];
      cell = gShockwaveScene_BurstCells[n] + SHOCKWAVE_SHEET_BURST;

      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                          COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, COMPOSITE_MAX);
      (((DrawRectangle *)gWorkSlot)[HEAP_SLOT_BLITTER_ALTERNATE])(
          canvas, work->sheet + cell, 60 - high, 80 - high * 2, high, high * 2);
      Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                          COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                              COMPOSITE_FLIP_X,
                          COMPOSITE_MAX);
      (((DrawRectangle *)gWorkSlot)[HEAP_SLOT_BLITTER_ALTERNATE])(
          canvas, work->sheet + cell, 60, 80 - high * 2, high, high * 2);
      Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                          COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                              COMPOSITE_FLIP_Y,
                          COMPOSITE_MAX);
      (((DrawRectangle *)gWorkSlot)[HEAP_SLOT_BLITTER_ALTERNATE])(
          canvas, work->sheet + cell, 60 - high, 80, high, high * 2);
      Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                          COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                              COMPOSITE_FLIP_X | COMPOSITE_FLIP_Y,
                          COMPOSITE_MAX);
      (((DrawRectangle *)gWorkSlot)[HEAP_SLOT_BLITTER_ALTERNATE])(
          canvas, work->sheet + cell, 60, 80, high, high * 2);
      Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);

      for (i = 0; i != 6; i++) {
        s32 k = i & 3;
        s32 angle = Random16() & 0xffff;
        s32 x =
            ((Trig_Sin(angle) * 16) >> 16) - gShockwaveScene_ShardWidths[k] / 2;
        s32 y = ((Trig_Cos(angle) * 16) >> 16) -
                gShockwaveScene_ShardHeights[k] / 2;
        draw[0](canvas, work->sheet + gShockwaveScene_ShardCells[k], x + 60,
                y + 80, gShockwaveScene_ShardWidths[k],
                gShockwaveScene_ShardHeights[k]);
      }
    }

    if (frame > 143) {
      s32 top = (frame << 4) - 2272;
      s32 n;
      s32 r;
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                          COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                          COMPOSITE_ADDITIVE);
      (slots[HEAP_SLOT_BLITTER_ALTERNATE])(canvas, work, 36, top, 24, 64);
      Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                          COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                              COMPOSITE_FLIP_X,
                          COMPOSITE_ADDITIVE);
      (slots[HEAP_SLOT_BLITTER_ALTERNATE])(canvas, work, 60, top, 24, 64);
      Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);

      n = (frame - 144) / 2;
      r = frame * 10 - 1424;
      work->transferValue = 75;
      if (n <= 6) {
        s32 cell = n * 770 + gBattleFx_MoteCellOffsets[7] + 0x2710;
        /* FAKEMATCH: the height is a value, so high / 2 is hoisted into fp.
         * 2026-10-13: the literals (4, 8 and 16) differ 690 of 2455. */
        s32 high = 8;
        EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                            COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, COMPOSITE_MAX);
        blit = slots[HEAP_SLOT_BLITTER_ALTERNATE];
        for (i = 0; i != 128; i++) {
          s32 angle = i << 9;
          s32 x = ((r * Trig_Sin(angle)) >> 16) - high / 2;
          s32 y = (r * Trig_Cos(angle)) >> 17;
          blit(canvas, work->sheet + cell, x + 60, y + 72, high, high * 2);
        }
        Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
      }
    }

    EFFECT_PRESENT_FRAME(work);
  }

  Iwram_FillWords(canvas, 0x4000, 0);

  for (i = 0; i != SHOCKWAVE_FORMATION_OBJECTS; i++) {
    ResourceObject_ReleaseFar(work->objects[i]);
  }

  for (i = 0; i != SHOCKWAVE_DEBRIS_OBJECTS; i++) {
    struct AnimationObject *obj = ResourceObject_CreateFar(0x186);
    work->objects[i] = obj;
    if (obj != 0) {
      obj->flags = 0;
      AnimationObjects_SelectAnimationFar(obj, (i % 3));
      work->objects[i]->image.priority = 0;
    }
  }

  Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                      COMPOSITE_MAX);
  draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
  Resource_LoadAndDecompress((s32)&ResourceId_BlueCrescentSheet,
                             work->sheet + 0x4000, 1, 1);

  {
    u8 *c = ramp;
    for (i = 0; i != 770; i++, c++) {
      if (*c > 32) {
        *c += 224;
      } else if (*c != 0) {
        *c = 1;
      }
    }
  }

  work->fadeFrames = 0;
  work->fadeStep = 0;
  BattleBackground_LoadFar(1, (s32)&ResourceId_AerialViewBackdrop, 0);
  REG_BG2PA = Q_8_8(1);
  REG_BG2X = -0x3c00;
  REG_BG2CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7);

  fill = 0;
  Dma_Set(&fill, canvas, SHOCKWAVE_DMA_FILL_CANVAS, REG_DMA3);

  cx = 60;
  cy = 44;
  squash = 6;
  shift = 0;
  ring = 0;
  sx = 8;
  rings = 2;
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);

  fill = shift;
  Dma_Set(&fill, work, SHOCKWAVE_DMA_FILL_WAVE, REG_DMA3);

  for (frame = 0; frame != SHOCKWAVE_SWEEP_FRAMES; frame++) {
    if (frame == 66) {
      AudioCommand_PlayFar(145);
      AudioCommand_PlayFar(141);
    }
    if (frame == 155) {
      AudioCommand_PlayFar(162);
    }
    if (frame == 217) {
      AudioCommand_PlayFar(156);
    }
    if (frame == 280) {
      AudioCommand_PlayFar(157);
    }
    if (frame == 300) {
      BattleEventRuntime_BeginPhaseFar(145);
    }

    if (gKeysRepeat & (KEY_A | KEY_B)) {
      if (frame >= 5 && frame < 150) {
        frame = 150;
        IO_WRITE_QUEUE_PUSH_IME(ime, (u32)&REG_BG2PA, Q_8_8(0.5), 0x20000);
        IO_WRITE_QUEUE_PUSH_IME(ime, (u32)&REG_BG2X, 0, 0x30000);
        Resource_LoadAndDecompress((s32)&ResourceId_ExplosionSheet,
                                   work->sheet + SHOCKWAVE_SHEET_EXPLOSION, 1,
                                   0);
      } else if (frame >= 155 && frame < 214) {
        frame = 214;
      } else if (frame >= 219 && frame < 280) {
        frame = 280;
      }
    }

    if (frame == 64) {
      IO_WRITE_QUEUE_PUSH_IME(ime, (u32)&REG_BG2PA, Q_8_8(0.5), 0x20000);
      IO_WRITE_QUEUE_PUSH_IME(ime, (u32)&REG_BG2X, 0, 0x30000);
      fill = 0;
      Dma_Set(&fill, canvas, SHOCKWAVE_DMA_FILL_CANVAS, REG_DMA3);
      Resource_LoadAndDecompress((s32)&ResourceId_ExplosionSheet,
                                 work->sheet + SHOCKWAVE_SHEET_EXPLOSION, 1, 0);
      Resource_LoadAndDecompress((s32)&ResourceId_OverlayImageA,
                                 work->sheet + SHOCKWAVE_SHEET_FLASH_SMALL, 0,
                                 0);
    }

    if (frame == 66) {
      u16 *pal = (u16 *)&BG_PLTT_COLOR(6, 0);
      for (i = 0; i != 128; i++, pal++) {
        *pal ^= 0x7fff;
      }
    }

    if (frame == 69) {
      fill = 0x3f3f3f3f;
      Dma_Set(&fill, canvas, SHOCKWAVE_DMA_FILL_CANVAS, REG_DMA3);
    }

    if (frame == 70) {
      BattlePresentation_ConfigurePaletteFadeFar(
          1, (s32)&ResourceId_AerialViewBackdrop, 7);
    }

    if (frame == 150) {
      cx = 112;
      cy = 32;
      ring = 0;
      fill = 0;
      rings = 4;
      squash = 8;
      Dma_Set(&fill, work, SHOCKWAVE_DMA_FILL_WAVE, REG_DMA3);
      fill = 0x3f3f3f3f;
      Dma_Set(&fill, canvas, SHOCKWAVE_DMA_FILL_CANVAS, REG_DMA3);
      BattleBackground_LoadFar(1, (s32)&ResourceId_BeachBackdrop, 0);
    }

    if (frame == 214) {
      fill = 0;
      Dma_Set(&fill, work, SHOCKWAVE_DMA_FILL_WAVE, REG_DMA3);
      fill = 0x3f3f3f3f;
      Dma_Set(&fill, canvas, SHOCKWAVE_DMA_FILL_CANVAS, REG_DMA3);
      BattleBackground_LoadFar(1, (s32)&ResourceId_ForestBackdrop, 0);
      for (i = 0; i != SHOCKWAVE_DEBRIS_OBJECTS; i++) {
        particle = &work->particles[i];
        particle->x = ((Random16() & 63) + 32) << 16;
        particle->y = ((Random16() & 31) + 96) << 16;
        particle->velocityY = ((Random16() & 15) + 16) << 15;
        particle->z = 0x8000 - (particle->x >> 7);
        particle->variant = 1;
      }
    }

    if (frame == 280) {
      BattleFx_SetupBlendedDisplay();
      fill = 0;
      Dma_Set(&fill, work, SHOCKWAVE_DMA_FILL_WAVE, REG_DMA3);
      fill = 0x01010101;
      Dma_Set(&fill, canvas, SHOCKWAVE_DMA_FILL_CANVAS, REG_DMA3);
      ctrl->active = 0;
      REG_BLDALPHA = BLEND_ALPHA(16, 16);
      for (i = 0; i != SHOCKWAVE_DEBRIS_OBJECTS; i++) {
        particle = &work->particles[i];
        particle->x = ((Random16() & 63) + 256) << 16;
        particle->y = ((Random16() & 31) + 96) << 16;
        particle->velocityY = ((Random16() & 15) + 16) << 15;
        particle->z = 0;
      }
      shift = 272;
      EFFECT_SET_TRANSFER(work, CANVAS_COPY, 0);
    }

    if (frame > 279) {
      /* FAKEMATCH: palette RAM through a volatile pointer: the native keeps
         red's mask 31 in the loop rather than hoisting it; a plain pointer
         differs in 52 of 2455 rows. */
      volatile u16 *pal = (volatile u16 *)0x05000002;
      for (i = 0; i != 63; i++, pal++) {
        s32 c = *pal;
        s32 b = ((u16)c >> 10) & 31;
        s32 g = ((u16)c >> 5) & 31;
        s32 r = c & 31;
        b++;
        g++;
        r++;
        if (b > 31) {
          b = 31;
        }
        if (g > 31) {
          g = 31;
        }
        if (r > 31) {
          r = 31;
        }
        *pal = (b << 10) | (g << 5) | r;
      }
    }

    if (frame == 182) {
      fill = 0x3f3f3f3f;
      Dma_Set(&fill, canvas, SHOCKWAVE_DMA_FILL_CANVAS, REG_DMA3);
    }

    if (frame <= 63) {
      u8 *sheet;
      s32 top = frame - 4;
      if ((frame & 7) < 4) {
        sheet = work->sheet + SHOCKWAVE_SHEET_FLARE_A;
      } else {
        sheet = work->sheet + SHOCKWAVE_SHEET_FLARE_B;
      }
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                          COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, COMPOSITE_MAX);
      (((DrawRectangle *)gWorkSlot)[HEAP_SLOT_BLITTER_ALTERNATE])(
          canvas, sheet, 36, top - 24, 24, 24);
      Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                          COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                              COMPOSITE_FLIP_X,
                          COMPOSITE_MAX);
      (((DrawRectangle *)gWorkSlot)[HEAP_SLOT_BLITTER_ALTERNATE])(
          canvas, sheet, 59, top - 24, 24, 24);
      Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                          COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                              COMPOSITE_FLIP_Y,
                          COMPOSITE_MAX);
      (((DrawRectangle *)gWorkSlot)[HEAP_SLOT_BLITTER_ALTERNATE])(
          canvas, sheet, 36, top - 1, 24, 24);
      Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                          COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                              COMPOSITE_FLIP_X | COMPOSITE_FLIP_Y,
                          COMPOSITE_MAX);
      (((DrawRectangle *)gWorkSlot)[HEAP_SLOT_BLITTER_ALTERNATE])(
          canvas, sheet, 59, top - 1, 24, 24);
      Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
    }

    if (frame >= 64 && frame < 66) {
      draw[0](canvas, work->sheet + SHOCKWAVE_SHEET_FLASH_SMALL, 52, 51, 16,
              17);
    }
    if (frame >= 66 && frame < 68) {
      draw[0](canvas, work->sheet + SHOCKWAVE_SHEET_FLASH_LARGE, 48, 40, 24,
              41);
    }
    if (frame >= 68 && frame < 76) {
      s32 d = 76 - frame;
      draw[0](canvas, work->sheet + SHOCKWAVE_SHEET_FLASH_STREAK, 49 - d / 2,
              38 - d, 22, 44);
      draw[0](canvas, work->sheet + SHOCKWAVE_SHEET_FLASH_STREAK, 49 - d / 2,
              d + 38, 22, 44);
      draw[0](canvas, work->sheet + SHOCKWAVE_SHEET_FLASH_STREAK, d / 2 + 49,
              38 - d, 22, 44);
      draw[0](canvas, work->sheet + SHOCKWAVE_SHEET_FLASH_STREAK, d / 2 + 49,
              d + 38, 22, 44);
    }
    if (frame >= 78 && frame < 80) {
      draw[0](canvas, work->sheet + SHOCKWAVE_SHEET_FLASH_SMALL, 52, 51, 16,
              17);
    }
    if (frame >= 80 && frame < 82) {
      draw[0](canvas, work->sheet + SHOCKWAVE_SHEET_FLASH_LARGE, 48, 40, 24,
              41);
    }
    if (frame >= 82 && frame < 86) {
      s32 d = frame * 2 - 164;
      draw[0](canvas, work->sheet + SHOCKWAVE_SHEET_FLASH_STREAK, 49 - d / 4,
              38 - d, 22, 44);
      draw[0](canvas, work->sheet + SHOCKWAVE_SHEET_FLASH_STREAK, 49 - d / 4,
              d + 38, 22, 44);
      draw[0](canvas, work->sheet + SHOCKWAVE_SHEET_FLASH_STREAK, d / 4 + 49,
              38 - d, 22, 44);
      draw[0](canvas, work->sheet + SHOCKWAVE_SHEET_FLASH_STREAK, d / 4 + 49,
              d + 38, 22, 44);
    }

    if (frame >= 72 && frame < 76) {
      s32 r2 = (frame << 4) - 1136;
      /* FAKEMATCH: the size is a value, so 5 and 20 are hoisted into fp and sl.
       * 2026-10-13: the literals (5, [9], 10 and 20) differ 100 of 2455. */
      s32 size = 10;
      for (i = 0; i != 256; i++) {
        s32 angle = i << 8;
        s32 x = ((Trig_Sin(angle) * r2) >> 16) - (size >> 1);
        s32 y = (Trig_Cos(angle) * r2) >> 17;
        draw[0](canvas, ramp + gBattleFx_MoteCellOffsets[size - 1], x + 60,
                y + 50, size, size * 2);
      }
    }

    if (frame > 85) {
      if (frame <= 213) {
        for (i = 0; i != rings; i++, ring++) {
          s32 radius = ring;
          s32 slope = 0;
          s32 err = ring;
          while (radius >= slope) {
            s32 half;
            s32 halfSlope;
            s32 left;
            s32 right;
            s32 top;
            s32 bottom;
            s32 dx;

            half = radius / 2;
            dx = (half * sx) / 8;
            right = cx + dx;
            /* FAKEMATCH: a dead branch restores the label CSE and reload saw
             * between the edges. 2026-10-13: without it 67 of 2455 differ; as
             * if (dx > cx) left = 0; else left = cx - dx; 173. */
            if (dx > cx) {
              left = 0;
            }
            left = cx - dx;
            bottom = cy + (slope * squash) / 8;
            top = cy - (slope * sx) / 8;
            if (top < 0) {
              top = 0;
            }
            if (bottom > 119) {
              bottom = 119;
            }
            if (left < 0) {
              left = 0;
            }
            if (right > 119) {
              right = 119;
            }
            work->sheet[bottom * SHOCKWAVE_WAVE_SIZE + right] = 20;
            work->sheet[top * SHOCKWAVE_WAVE_SIZE + right] = 20;
            work->sheet[bottom * SHOCKWAVE_WAVE_SIZE + left] = 20;
            work->sheet[top * SHOCKWAVE_WAVE_SIZE + left] = 20;
            right = cx + half + 1;
            left = cx - half + 1;
            if (left < 0) {
              left = 0;
            }
            if (right > 119) {
              right = 119;
            }
            work->sheet[bottom * SHOCKWAVE_WAVE_SIZE + right] = 20;
            work->sheet[top * SHOCKWAVE_WAVE_SIZE + right] = 20;
            work->sheet[bottom * SHOCKWAVE_WAVE_SIZE + left] = 20;
            work->sheet[top * SHOCKWAVE_WAVE_SIZE + left] = 20;

            halfSlope = slope / 2;
            dx = (halfSlope * sx) / 8;
            right = cx + dx;
            /* FAKEMATCH: a dead branch restores the label CSE and reload saw
             * between the edges. 2026-10-13: without it 1954 of 2455 differ,
             * without both dead branches 1949. */
            if (dx > cx) {
              left = 0;
            }
            left = cx - dx;
            bottom = cy + (radius * squash) / 8;
            top = cy - (radius * sx) / 8;
            if (left < 0) {
              left = 0;
            }
            if (right > 119) {
              right = 119;
            }
            if (top < 0) {
              top = 0;
            }
            if (bottom > 119) {
              bottom = 119;
            }
            work->sheet[bottom * SHOCKWAVE_WAVE_SIZE + right] = 20;
            work->sheet[top * SHOCKWAVE_WAVE_SIZE + right] = 20;
            work->sheet[bottom * SHOCKWAVE_WAVE_SIZE + left] = 20;
            work->sheet[top * SHOCKWAVE_WAVE_SIZE + left] = 20;
            right = cx + halfSlope + 1;
            left = cx - halfSlope + 1;
            if (left < 0) {
              left = 0;
            }
            if (right > 119) {
              right = 119;
            }
            work->sheet[bottom * SHOCKWAVE_WAVE_SIZE + right] = 20;
            work->sheet[top * SHOCKWAVE_WAVE_SIZE + right] = 20;
            work->sheet[bottom * SHOCKWAVE_WAVE_SIZE + left] = 20;
            work->sheet[top * SHOCKWAVE_WAVE_SIZE + left] = 20;

            err = err - slope * 2 - 1;
            if (err < 0) {
              err = err + radius * 2 - 2;
              radius--;
            }
            slope++;
          }
        }
      }
    }

    if (frame >= 86 && frame < 150) {
      s32 angle = -0x4000;
      for (i = 0; i != 9; i++, angle += 0x1000) {
        s32 x = ((frame - 86) * Trig_Sin(angle)) >> 16;
        s32 y = ((frame - 70) * Trig_Cos(angle)) >> 16;
        draw[0](canvas,
                work->sheet +
                    ((frame / 2 + i) % 4) * SHOCKWAVE_EXPLOSION_CELL_BYTES +
                    SHOCKWAVE_SHEET_EXPLOSION,
                x + 44, y + 17, 32, 54);
      }
    }

    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                        COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                        COMPOSITE_ADDITIVE);
    draw[1] = gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
    if (frame > 85) {
      draw[1](canvas, work, 0, 0, SHOCKWAVE_WAVE_SIZE, SHOCKWAVE_WAVE_SIZE);
    }

    if (frame > 279) {
      shift -= 8;
    }

    if (frame > 319) {
      EFFECT_FOR_EACH_TARGET(i, work->effect) {
        ObjectGroup_UpdateMembers(work->effect->actors[i], SPRITE_COLOR_BLACK,
                                  POSE_DAMAGE, -1, 0);
      }
    }

    if (frame == 238) {
      fill = 0x3f3f3f3f;
      Dma_Set(&fill, canvas, SHOCKWAVE_DMA_FILL_CANVAS, REG_DMA3);
    }

    if (frame >= 214 && frame < 280) {
      gBgScroll[1].x += 8;
    }

    if (frame >= 246 && frame < 280) {
      shift += 8;
      EFFECT_SET_TRANSFER(work, CANVAS_COPY, 75);
    }

    if (frame > 213) {
      debrisPlace.groundHeight = 0;
      debrisPlace.height = 0;
      for (i = 0; i != SHOCKWAVE_DEBRIS_OBJECTS; i++) {
        particle = &work->particles[i];
        if (particle->variant != 0) {
          scale.x = particle->z + (particle->y >> 7);
          if (scale.x < 0x800) {
            scale.x = 0x800;
          }
          scale.y = scale.x;
          debrisPlace.x = particle->x;
          debrisPlace.height = 0xff0000;
          debrisPlace.y = particle->y + 0xff0000;
          Object_ApplyProjectedPlacementFar(work->objects[i], &debrisPlace,
                                            &scale, 0);
          particle->y -= particle->velocityY;
          if (particle->y < 0x100000) {
            if (frame <= 279) {
              particle->x = ((Random16() & 63) + shift + 32) << 16;
            } else {
              particle->x = ((Random16() & 63) + shift - 32) << 16;
            }
            if (frame > 301) {
              particle->variant = 0;
            } else if (frame > 245) {
              particle->y = 0x600000;
            } else {
              particle->y = 0xc00000 - particle->x;
            }
          }
        }
      }

      if (frame <= 279) {
        for (i = 0; i != 8; i++) {
          s32 x =
              (i & 1) * 20 - (Random16() & 15) + (i / 2) * 4 + shift / 2 - 16;
          s32 y = (i / 2) * 32 - (Random16() & 15);
          draw[1](canvas,
                  work->sheet +
                      ((frame / 2 + i) % 4) * SHOCKWAVE_EXPLOSION_CELL_BYTES +
                      SHOCKWAVE_SHEET_EXPLOSION,
                  x, y, 32, 54);
        }
      } else {
        for (i = 0; i != 8; i++) {
          s32 x =
              (i & 1) * 20 + (Random16() & 15) - (i / 2) * 4 + shift / 2 - 16;
          s32 y = (i / 2) * 32 - (Random16() & 15);
          draw[1](canvas,
                  work->sheet +
                      ((frame / 2 + i) % 4) * SHOCKWAVE_EXPLOSION_CELL_BYTES +
                      SHOCKWAVE_SHEET_EXPLOSION,
                  x, y, 32, 54);
        }
      }
    }

    Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);

    if (frame > 63) {
      /* FAKEMATCH: forced temporary: the shake value through an s32 keeps the
       * AND constant an immediate and the instruction count native's.
       * 2026-10-13: gBgScroll[1].y = (Random16() & 3) + 30 directly differs 17
       * of 2455, 2456 instructions. */
      s32 shake = (Random16() & 3) + 30;

      gBgScroll[1].y = shake;
    }

    EFFECT_PRESENT_FRAME(work);
  }

  for (i = 0; i != SHOCKWAVE_DEBRIS_OBJECTS; i++) {
    ResourceObject_ReleaseFar(work->objects[i]);
  }

  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

// Object formations that sweep, rise or break into flares.
//
// These effects move a formation of the effect's animation objects
// (work->objects, created by BattleFx_SpawnObjects) across the battlefield
// while the canvas layer (BG2) draws particles behind and around them.
//
// The object row runs in two stages. In the sweep stage nine objects ride in
// on a sine wave while the backdrop scrolls, a ring of motes opens around
// them and a trail of fire follows. In the rise stage the row slides across
// the targets; each target the row passes throws a burst of 32 motes and
// plays its hit reaction, and the fire sheet slides off behind it.
//
// The formation sweep also runs in two stages. The first places eight objects
// at one and a half times their size, opens the window with an edge table
// built every frame, sprays particles under gravity, converges a ring of
// flares, then breaks into a burst of light. During it a frame callback and
// an H-blank interrupt change BG2's horizontal scale. The second stage drops
// five blue flame columns onto the targets; each column throws sparks when it
// reaches the ground.

#define KIRIN_OBJECTS 9 // objects in the rows

// Abilities: Kirin (scene kind 3).
//
// The scene runs in two stages. In the sweep, KIRIN_OBJECTS objects ride in
// on a sine wave while the backdrop scrolls, a ring of motes opens around
// them and a trail of fire follows. In the rise the row slides across the
// targets; each target the row passes throws a burst of 32 motes and plays its
// hit reaction, and the fire sheet slides off behind it.
void BattleFx_RunKirin(struct BattleEffectArgument *effect) {
  static struct SpriteScale sweepScale = {{{0x10000, 0x10000}}};
  static struct SpriteScale riseScale = {{{0x10000, 0x10000}}};
  void **cursor;
  struct SpritePlacement place;
  struct EffectPosition unit;
  u8 jitterRow[16];
  u8 hitRow[14];
  struct SpritePlacement risePlace;
  void *canvas;
  struct BattleEffectWork *work;
  DrawRectangle blit;
  u8 *sheet;
  s32 baseX;
  s32 grow;
  s32 savedX;
  struct BattleBackgroundView *control;
  s32 shift;
  s32 wave;
  s32 frame;
  s32 i;

  cursor = &gBattleFxWork[1];
  canvas = cursor[0];
  work = cursor[-1];
  sheet = cursor[1];
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);
  BattleFx_ConfigureEffectDisplay();
  REG_BG2CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7);
  PLTT_BACKDROP = 0;
  BG_PLTT_COLOR(0, 1) = 0;
  work->transferMode = CANVAS_COPY;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);
  BattleFx_WipeCanvas(1, 0);
  BattleFx_SpawnObjects(KIRIN_OBJECTS, 0x175, 1);
  gProjection.centerY = 240;
  BattleFx_SelectLivingTargets(work->effect);
  REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG1 |
              WININ_WIN1_BG2 | WININ_WIN1_CLR;
  REG_WIN0H = WIN_RANGE(0, 202);
  WaitFrames(1);
  BattleBackground_LoadFar(1, (s32)&ResourceId_ForestBackdrop, 0);
  BattleFx_WipeCanvas(1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_FireBlobSheet, work->sheet, 1, 1);
  EFFECT_SHOW_WIDE_CANVAS(14);

  baseX = 0;
  grow = 0;
  savedX = gBgScroll[1].x;
  control = cursor[4];
  shift = 0;
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, baseX);
  control->busy = 1;

  i = 0;
  do {
    struct EffectStep *fire = &work->particles[i];

    fire->x = (Random16() & 31) + 16;
    fire->y = ((Random16() & 31) + 48) << 16;
    fire->velocityY = ((Random16() & 31) - 16) << 16;
    fire->variant = Random16() % 48 + 2;
    i++;
  } while (i != 64);

  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                      COMPOSITE_ADDITIVE);
  blit =
      (DrawRectangle)((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BLITTER];
  REG_BG2CNT = BGCNT_PRIORITY(2) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7);

  for (frame = 0; frame != 120; frame++) {
    wave = 0;
    if (frame == 0) {
      AudioCommand_PlayFar(136);
    }
    if (frame == 26) {
      AudioCommand_PlayFar(141);
    }
    if (frame == 40) {
      AudioCommand_PlayFar(154);
    }
    if (frame == 72) {
      AudioCommand_PlayFar(154);
    }
    if (frame == 104) {
      AudioCommand_PlayFar(154);
    }
    if ((gKeysRepeat & (KEY_A | KEY_B)) != 0 && frame > 16) {
      break;
    }

    if (frame >= 24 && frame <= 55) {
      shift++;
    }
    if (shift > 24) {
      shift = 24;
    }
    if (frame <= 135) {
      gBgScroll[1].x -= shift;
      grow += shift;
    }

    if (frame <= 149) {
      struct SpriteScale pair = sweepScale;
      s32 lift;

      lift = 0;
      if (frame > 103) {
        lift = frame * 16 - 1664;
      }
      if (frame >= 8 && frame <= 31) {
        baseX = baseX + shift - 8;
      }
      if (frame > 7) {
        s32 amp;
        s32 ang;

        amp = 96;
        if (frame <= 104) {
          amp = 32;
        }
        ang = (frame * 1024 - 0x2000) & 0xFFFF;
        if (ang > 0x8000) {
          ang -= 0x8000;
        }
        wave = (Trig_Sin(ang) * amp) >> 16;
        if ((frame & 31) == 8) {
          work->shakeFrames = 4;
        }
      }
      place.groundHeight = 0;
      place.height = 255 << 16;
      i = 0;
      do {
        place.x = ((baseX + gKirin_Columns[i] - lift) << 16) + (224 << 16);
        place.y = ((gKirin_Rows[i] - wave) << 16) + (144 << 15);
        Object_ApplyProjectedPlacementFar(work->objects[i], &place, &pair, 0);
        i++;
      } while (i != KIRIN_OBJECTS);
    }

    if (frame <= 26) {
      s32 size = grow + 4;
      s32 count = frame * 8;

      if (size > 10) {
        size = 10;
      }
      if (count > 64) {
        count = 64;
      }
      for (i = 0; i != count; i++) {
        s32 ang = i << 10;
        s32 x = ((Trig_Sin(ang) * (grow * 2 + 8)) >> 16) + grow + 96;
        s32 y = ((Trig_Cos(ang) * (grow * 12 + 48)) >> 16) + 64;

        EFFECT_DRAW_MOTE(blit, canvas, sheet, x, y, size);
      }
    }

    if (frame == 24) {
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, 50);
    }
    if (frame == 28) {
      REG_BG2CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
                   BGCNT_SCREEN_BASE(7);
    }

    if (frame > 17) {
      i = 0;
      do {
        struct EffectStep *fire = &work->particles[i];

        if (fire->variant == 0) {
          s32 size = i % 3 + 1;

          blit(canvas, sheet + gBattleFx_MoteCellOffsets[size - 1], fire->x,
               HI(fire->y) - size, size, size * 2);
          fire->x += 2;
          fire->y += fire->velocityY;
          fire->velocityY = fire->velocityY * 48 / 64;
        } else {
          fire->variant--;
        }
        if (fire->x > 128 || fire->variant == 1) {
          fire->x = (Random16() & 31) + baseX + 172;
          fire->y = ((Random16() & 31) - wave + 56) << 16;
          fire->velocityY = ((Random16() & 31) - 16) << 15;
        }
        i++;
      } while (i != 48);
    }

    if (frame > 31) {
      s32 rise = (frame - 32) / 2;
      s32 y;

      if (rise > 40) {
        rise = 40;
      }
      i = 0;
      y = 0;
      do {
        blit(canvas, work->sheet + (Random16() & 3) * 0x600, 120 - rise, y, 48,
             32);
        i++;
        y += 18;
      } while (i != 6);
    }

    if (work->shakeFrames > 0) {
      work->shakeFrames--;
      gBgScroll[1].y = (Random16() & 7) + 28;
    } else {
      gBgScroll[1].y = 32;
    }
    EFFECT_PRESENT_FRAME(work);
  }

  gBgScroll[1].x = savedX;
  control->busy = 0;
  BattleFx_SetupBlendedDisplay();
  REG_WIN0H = WIN_RANGE(0, 240);

  i = 0;
  do {
    struct AnimationObject *object = work->objects[i];

    object->image.priority = 3;
    i++;
  } while (i != KIRIN_OBJECTS);

  {
    s32 rowX;
    u8 *hit;
    u8 *jitter;

    rowX = 224;
    hit = hitRow;
    i = 0;
    do {
      hit[i] = 0;
      i++;
    } while (i != 14);
    jitter = jitterRow;
    i = 0;
    do {
      jitter[i] = Random16() & 31;
      i++;
    } while (i != 16);
    i = 0;
    do {
      PARTICLES[i].variant = 0;
      i++;
    } while (i != 160 << 1);

    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);
    REG_BG2CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
                 BGCNT_SCREEN_BASE(7);
    REG_BLDALPHA = BLEND_ALPHA(16, 16);

    frame = 0;
    do {
      if (frame <= 23) {
        struct SpriteScale riseScaleCopy = riseScale;
        s32 ang;
        s32 rise;

        rowX -= 16;
        if (frame <= 8) {
          ang = frame * 2048 + 0x4000;
          if (ang > 0x8000) {
            ang -= 0x8000;
          }
          rise = ((Trig_Sin(ang) * 64)) >> 16;
        } else {
          ang = frame * 2048 + 0x4000;
          if (ang > 0x8000) {
            ang -= 0x8000;
          }
          rise = ((Trig_Sin(ang) * 32)) >> 16;
        }
        risePlace.groundHeight = 0;
        risePlace.height = 255 << 16;
        i = 0;
        do {
          risePlace.x = (rowX + gKirin_Columns[i]) << 16;
          risePlace.y = ((gKirin_Rows[i] - rise) << 16) + (144 << 15);
          Object_ApplyProjectedPlacementFar(work->objects[i], &risePlace,
                                            &riseScaleCopy, 0);
          i++;
        } while (i != KIRIN_OBJECTS);
      }

      if (frame == 8) {
        work->shakeFrames = frame;
        AudioCommand_PlayFar(145);
      }
      if (frame == 11) {
        AudioCommand_PlayFar(145);
      }
      if (frame == 46) {
        AudioCommand_PlayFar(137);
      }

      EFFECT_FOR_EACH_TARGET(i, work->effect) {
        if (hit[i] == 0) {
          EffectPosition_ApplyAlternateStepAndYOffset(work->effect->actors[i],
                                                      &unit);
          if (unit.x > rowX) {
            s32 k;

            hit[i] = 1;
            k = 0;
            do {
              struct EffectStep *burst = &PARTICLES[i * 32 + k];
              s32 up;

              burst->x = unit.x << 15;
              burst->y = (unit.y - 16) << 16;
              burst->velocityX = ((Random16() & 255) - 128) << 10;
              up = (Random16() & 255) - 192;
              burst->velocityY = up << 11;
              burst->x += burst->velocityX * 4;
              burst->y += up << 13;
              burst->variant = (Random16() & 15) + 8;
              k++;
            } while (k != 32);
            BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 1);
            AudioCommand_PlayFar(134);
          }
        }
      }

      i = 0;
      do {
        struct EffectStep *burst = &PARTICLES[i];

        if (burst->variant > 0) {
          s32 size = 3;

          EFFECT_DRAW_MOTE(blit, canvas, sheet, HI(burst->x), HI(burst->y),
                           size);
          burst->x += burst->velocityX;
          burst->y += burst->velocityY;
          burst->variant--;
        }
        i++;
      } while (i != 192);

      if (frame == 48) {
        AudioCommand_PlayFar(136);
      }
      if (frame > 40) {
        s32 slide = (frame - 40) * 12;
        s32 y;

        EFFECT_SET_TRANSFER(work, CANVAS_COPY, 75);
        i = 0;
        y = -8;
        do {
          blit(canvas, work->sheet + (Random16() & 3) * 0x600,
               jitter[i] - slide + 120, y, 48, 32);
          i++;
          y += 8;
        } while (i != 16);
      }
      if (frame > 64) {
        work->transferMode = CANVAS_COPY_AND_FADE;
      }
      if (frame == 58) {
        EFFECT_FOR_EACH_TARGET(i, work->effect) {
          ObjectGroup_UpdateMembers(work->effect->actors[i], SPRITE_COLOR_BLACK,
                                    POSE_DAMAGE, -1, 0);
        }
      }

      Camera_ApplyShake(8, 8);
      EFFECT_END_FRAME(work);
      frame++;
    } while (frame != 96);
  }

  BattleEventRuntime_BeginPhaseFar(134);
  i = 0;
  do {
    ResourceObject_ReleaseFar(work->objects[i]);
    i++;
  } while (i != KIRIN_OBJECTS);
  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

// Restore the background 2 horizontal scale to one screen pixel per source
// pixel.
void Graphics_ResetBg2Pa(void) { REG_BG2PA = Q_8_8(1); }

// Stretch background 2 to twice its source width.
void Graphics_SetBg2AffineScaleHalf(void) { REG_BG2PA = Q_8_8(0.5); }

#define NEPTUNE_FLAME_GROUND 112   // where the blue flames throw sparks
#define NEPTUNE_OBJECTS 8          // objects in the formation
#define NEPTUNE_PARTICLES 1024     // map-buffer particle slots
#define NEPTUNE_FLARE_SHEET 0x4e20 // the flare pictures in the sheet

// Abilities: Neptune (scene kind 6).
//
// The scene runs in two stages. The first places NEPTUNE_OBJECTS objects at
// one and a half times their size, opens the window with an edge table built
// every frame, sprays particles under gravity, converges a ring of flares,
// then breaks into a burst of light; meanwhile a frame callback and an HBlank
// interrupt change BG2's horizontal scale. The second stage drops five blue
// flame columns onto the targets; each throws sparks when it reaches the
// ground (NEPTUNE_FLAME_GROUND).
void BattleFx_RunNeptune(struct BattleEffectArgument *effect) {
  static struct SpriteScale unitScale = {{{0x18000, 0x18000}}};
  struct SpritePlacement pos;
  struct EffectPosition targetPosition;
  void *canvas;
  u8 *particleSheet;
  s32 windowEdgeStep;
  s32 formationX;
  s32 formationZ;
  s32 formationVelocityX;
  s32 formationVelocityZ;
  s32 savedBg1ScrollX;
  struct BattleBackgroundView *background;
  s32 j;
  s32 emittedSparks;
  s32 sparkOriginX;
  u32 *cache;
  struct BattleEffectWork *work;
  s32 frame;
  s32 t;
  s32 i;
  s32 n;
  s32 s;
  struct EffectStep *flareSeed;
  struct EffectStep *flare;
  struct EffectStep *burstSeed;
  struct EffectStep *flameColumn;
  DrawRectangle blit[2];
  struct SpriteScale scale;
  s32 ground = NEPTUNE_FLAME_GROUND;

  cache =
      (u32 *)&((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BATTLE_CANVAS];
  canvas = (void *)cache[0];
  work = (struct BattleEffectWork *)CANVAS_SLOT(cache, HEAP_SLOT_BATTLE_EFFECT);
  particleSheet = (u8 *)CANVAS_SLOT(cache, HEAP_SLOT_BATTLE_SHEET);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);
  BattleFx_ConfigureEffectDisplay();
  PLTT_BACKDROP = 0;
  *(u16 *)&BG_PLTT[1] = 0;
  Scheduler_AddOrUpdateCallback(Graphics_ResetBg2Pa, 0x480);
  Runtime_SetIrqHandler(2, 0, (s32)Graphics_SetBg2AffineScaleHalf);
  work->transferMode = CANVAS_COPY;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);
  REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_CLR;
  BattleFx_WipeCanvas(1, 0);
  REG_WIN0H = WIN_RANGE(240, 240);
  BattleFx_SelectLivingTargets(work->effect);
  BattleFx_SpawnObjects(NEPTUNE_OBJECTS, 0x17a, 1);
  gProjection.centerY = 240;
  WaitFrames(1);
  BattleBackground_LoadFar(1, (s32)&ResourceId_DuskCloudsBackdrop, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, particleSheet,
                             0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_WaterSpraySheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD,
                             work->sheet + NEPTUNE_FLARE_SHEET, 0, 0);
  EFFECT_SHOW_WIDE_CANVAS(16);
  windowEdgeStep = 1;
  formationX = 140 << 17;
  formationZ = 200 << 16;
  formationVelocityX = -0x40000;
  formationVelocityZ = -0x80000;
  savedBg1ScrollX = gBgScroll[1].x;
  background = (struct BattleBackgroundView *)CANVAS_SLOT(
      cache, HEAP_SLOT_BATTLE_BACKGROUND);
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);
  background->busy = 1;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                      COMPOSITE_ADDITIVE);
  blit[0] = (DrawRectangle)CANVAS_SLOT(cache, HEAP_SLOT_BLITTER);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                      COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, COMPOSITE_MAX);
  blit[1] = (DrawRectangle)CANVAS_SLOT(cache, HEAP_SLOT_BLITTER_ALTERNATE);
  REG_BG2CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7);

  for (i = 0; i != NEPTUNE_PARTICLES; i++) {
    PARTICLES[i].variant = -1;
  }
  // Seed the flares that converge during the formation burst.
  for (i = 0, flareSeed = work->particles; i != 32; i++, flareSeed++) {
    s32 a = (Random16() & 0x3fff) + 0x8000;
    s32 r = (Random16() & 127) + 255;
    flareSeed->x = (Trig_Sin(a) * r) >> 2;
    flareSeed->y = (Trig_Cos(a) * r) >> 2;
    flareSeed->velocityX = -flareSeed->x / 32;
    flareSeed->velocityY = -flareSeed->y / 32;
    flareSeed->variant = 0;
  }
  for (i = 0; i != 712; i++) {
    s32 r = Random16() & 255;
    s32 a = Random16() & 0xffff;
    PARTICLES[i].x = ((Trig_Sin(a) * r) >> 3) + 0x800000;
    PARTICLES[i].y = ((Trig_Cos(a) * r) >> 4) + (192 << 15);
    PARTICLES[i].velocityX = (-(Random16() & 0x1fff) - 0x800) << 5;
    PARTICLES[i].velocityY = (-(Random16() & 0xfff) - 0x1800) << 5;
    PARTICLES[i].variant = 0;
  }

  frame = 0;
  do {
    if (frame == 24) {
      AudioCommand_PlayFar(162);
    }
    if (frame == 76) {
      AudioCommand_PlayFar(164);
    }
    if (frame == 154) {
      AudioCommand_PlayFar(142);
    }
    if (frame == 222) {
      AudioCommand_PlayFar(145);
    }
    if (frame >= 0 && frame < 16) {
      u16 *phase = (u16 *)Ram_MapCellBuffer;
      if (frame == 1) {
        u8 *noise = (u8 *)gMapCellBuffer + 2;
        s32 k;
        for (k = 0; k != 128; k++) {
          noise[k] = Random16() & 63;
        }
        *phase = 0;
        Scheduler_AddOrUpdateCallback(BattleFx_BuildWindowEdgeTable, 0x480);
      }
      *phase += windowEdgeStep;
      windowEdgeStep += 3;
      if (frame == 15) {
        Scheduler_RemoveCallback(BattleFx_BuildWindowEdgeTable);
      }
    }

    if (frame > 21) {
      scale = unitScale;
      if (frame <= 83) {
        if (frame > 48) {
          formationVelocityX = formationVelocityX * 60 / 64;
        }
        formationVelocityZ = formationVelocityZ * 60 / 64;
        if (formationZ > 0x840000) {
          formationVelocityZ -= 0x2000;
        } else {
          formationVelocityZ += 0x2000;
        }
        formationX += formationVelocityX;
        formationZ += formationVelocityZ;
      } else if (frame <= 170) {
        if (frame <= 107) {
          formationVelocityX += 0x2000;
        }
        formationVelocityX = formationVelocityX * 60 / 64;
        formationX += formationVelocityX;
        if (frame <= 117) {
          if (formationZ > 0x8a0000) {
            formationVelocityZ -= 0x2000;
          } else {
            formationVelocityZ += 0x2000;
          }
          formationVelocityZ = formationVelocityZ * 56 / 64;
          formationZ += formationVelocityZ;
        }
      }
      pos.groundHeight = 0;
      pos.height = 0;
      for (i = 0; i != NEPTUNE_OBJECTS; i++) {
        pos.x =
            (((gNeptune_ObjectX[i] * 3) >> 1) << 16) + formationX - 0x200000;
        pos.y =
            (((gNeptune_ObjectY[i] * 3) >> 1) << 16) + formationZ - 0x180000;
        Object_ApplyProjectedPlacementFar(work->objects[i], &pos, &scale, 0);
      }
    }

    if (frame == 76) {
      for (i = 0; i != NEPTUNE_PARTICLES; i++) {
        if (PARTICLES[i].variant == -1) {
          s32 r = Random16() & 255;
          s32 a = Random16() & 0xffff;
          PARTICLES[i].x = ((Trig_Sin(a) * r) >> 3) + 0x400000;
          PARTICLES[i].y = ((Trig_Cos(a) * r) >> 4) + (192 << 15);
          PARTICLES[i].velocityX = (32 - (Random16() & 63)) << 12;
          PARTICLES[i].velocityY = (-(Random16() & 31) - 16) << 13;
          PARTICLES[i].variant = 0;
        }
      }
    }
    if (frame == 152) {
      Runtime_SetIrqHandler(2, 96, (s32)Graphics_SetBg2AffineScaleHalf);
      work->fadeFrames = 24;
      work->fadeStep = 0;
    }

    t = frame - 152;
    if (t >= 0 && t < 88) {
      for (i = 0, flare = work->particles; i != 32; i++, flare++) {
        if (frame >= i / 4 + 152 && frame < i / 4 + 152 + 32) {
          s32 fx = HI(flare->x) + 112;
          s32 fy = HI(flare->y) + 62;
          s32 r = (i & 3) + 5;
          s = r * 2;
          blit[0](canvas,
                  work->sheet + gBattleFx_FlareCells[r - 1] +
                      NEPTUNE_FLARE_SHEET,
                  fx - r, fy - r, s, s);
          flare->x += flare->velocityX;
          flare->y += flare->velocityY;
        }
      }
    }

    if (frame == 222) {
      for (i = 0; i != 64; i++) {
        burstSeed = &work->particles[i];
        burstSeed->x = (Random16() & 15) - 8;
        burstSeed->y = (Random16() & 15) - 8;
        if (burstSeed->x < 0) {
          burstSeed->x -= 4;
        } else {
          burstSeed->x += 4;
        }
        if (burstSeed->y < 0) {
          burstSeed->y -= 4;
        } else {
          burstSeed->y += 4;
        }
        burstSeed->x += 100;
        burstSeed->y += 52;
        burstSeed->velocityX = -6;
        burstSeed->z = burstSeed->x;
        burstSeed->velocityY = burstSeed->y;
        burstSeed->variant = 0;
      }
    }
    if (frame > 221) {
      for (i = 0; i != 64; i++) {
        u16 *cells = gBattleFx_FlareCells;
        s32 r = 1;
        if (frame >= i / 2 + 222) {
          blit[0](canvas, work->sheet + cells[r - 1] + NEPTUNE_FLARE_SHEET,
                  work->particles[i].x - r, work->particles[i].y - r, r * 2,
                  r * 2);
          work->particles[i].x += work->particles[i].velocityX;
          work->particles[i].y += work->particles[i].velocityY;
          if (work->particles[i].y < 0) {
            work->particles[i].y = work->particles[i].velocityY;
            work->particles[i].x = work->particles[i].z;
          }
        }
      }
    }

    if (frame > 175) {
      n = 5;
      if (frame > 221) {
        n = 60;
        if (frame != 223 && frame != 224) {
          n = 40;
        }
      }
      if (n > 60) {
        n = 60;
      }
      for (i = 0; i != n; i++) {
        s32 w;
        s32 d = (i + 1) / 2;
        w = n * 2 - d * 4;
        if (w < 12) {
          w = 12;
        }
        if (w > 63) {
          w = 63;
        }
        if (i & 1) {
          d = -d;
        }
        BattleFx_DrawClippedCanvasLine(d + 48, 0, d / 2 + 111, 63 - (d + 1) / 2,
                                       w);
      }
    }

    if (t >= 0 && t < 40) {
      s32 z;
      z = t / 2 + 1;
      if (z > 4) {
        z = 4;
      }
      blit[1](canvas,
              work->sheet + gBattleFx_FlareCells[z - 1] + NEPTUNE_FLARE_SHEET,
              108 - z, 60 - z, z * 2, z * 2);
    }
    if (frame >= 192 && frame < 200) {
      s32 z;
      z = (frame - 192) / 2 + 3;
      if (z > 6) {
        z = 6;
      }
      blit[1](canvas,
              work->sheet + gBattleFx_FlareCells[z - 1] + NEPTUNE_FLARE_SHEET,
              108 - z, 60 - z, z * 2, z * 2);
    }
    if (frame > 199) {
      s32 z;
      s32 u = frame - 200;
      z = u / 4 + 5;
      if (z > 8) {
        z = 8;
      }
      blit[1](canvas,
              work->sheet + gBattleFx_FlareCells[z - 1] + NEPTUNE_FLARE_SHEET,
              108 - z, 60 - z, z * 2, z * 2);
      if (frame <= 213) {
        z = u / 4 + 1;
        if (z > 4) {
          z = 4;
        }
        blit[1](canvas,
                work->sheet + gBattleFx_FlareCells[z - 1] + NEPTUNE_FLARE_SHEET,
                100 - z, 52 - z, z * 2, z * 2);
      }
    }
    if (frame >= 214 && frame < 222) {
      s32 z;
      z = (frame - 214) / 4;
      if (z > 2) {
        z = 2;
      }
      blit[1](canvas, work->sheet + z * 9 * 256 + 0x400, 63, 18, 48, 48);
    }
    if (frame > 221) {
      s32 z;
      z = (frame - 222) / 2 % 4 + 3;
      blit[0](canvas, work->sheet + z * 9 * 256 + 0x400, 72, 30, 48, 48);
      if (z == 5) {
        blit[0](canvas, work->sheet + 0x1600, 66, 22, 48, 48);
      }
    }

    if (frame > 27) {
      DrawRectangle draw = blit[1];
      for (i = 0; i != NEPTUNE_PARTICLES; i++) {
        s32 m = 3;
        if (PARTICLES[i].variant >= 0) {
          s32 k = i % 3 + 2;
          EFFECT_DRAW_MOTE(draw, canvas, particleSheet, HI(PARTICLES[i].x),
                           HI(PARTICLES[i].y), k);
          EffectStep_AdvanceWithGravity2D(&PARTICLES[i], 62,
                                          gNeptune_Gravity[i & m]);
          PARTICLES[i].variant++;
          if (PARTICLES[i].velocityY > 0 && HI(PARTICLES[i].y) > 104) {
            PARTICLES[i].variant = -1;
          }
        }
      }
    }
    for (i = 0; i != 5; i++) {
      blit[1](canvas, work->sheet, i * 32 + ((frame / 4) & 31) - 32, 88, 32,
              32);
    }
    EFFECT_PRESENT_FRAME(work);
  } while (++frame != 244 && (frame <= 16 || !(gKeysRepeat & (KEY_A | KEY_B))));

  gBgScroll[1].x = savedBg1ScrollX;
  background->busy = 0;
  Runtime_SetIrqHandler(2, 0, 0);
  BattleFx_SetupBlendedDisplay();
  Scheduler_RemoveCallback(Graphics_ResetBg2Pa);
  REG_WIN0H = WIN_RANGE(0, 240);
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);
  for (i = 0; i != NEPTUNE_PARTICLES; i++) {
    PARTICLES[i].variant = 0;
  }
  for (i = 0; i != 16; i++) {
    work->particles[i].x = (Random16() & 31) + 32;
    work->particles[i].y = 0;
    work->particles[i].variant = 0;
  }
  EFFECT_FOR_EACH_TARGET(i, work->effect) {
    EffectPosition_ApplyStepAndYOffset(work->effect->actors[i],
                                       &targetPosition);
    work->particles[i].x = targetPosition.x / 2;
  }
  // The second stage raises blue flames and emits sparks as they reach the
  // ground.
  Resource_LoadAndDecompress((s32)&ResourceId_BlueFlameSheet, work->sheet, 1,
                             1);
  AudioCommand_PlayFar(0x121);
  REG_BG2PA = Q_8_8(0.5);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);

  frame = 0;
  do {
    if (frame == 96) {
      BattleEventRuntime_BeginPhaseFar(134);
    }
    for (i = 0; i != 5; i++) {
      flameColumn = &work->particles[i];
      if (frame == i * 16 + 7) {
        AudioCommand_PlayFar(154);
      }
      if (frame >= i * 16 && flameColumn->variant <= 31) {
        s32 wobble = frame * 16 % 104;
        blit[1](canvas, work->sheet, flameColumn->x - 8,
                flameColumn->y + wobble - 216, 17, 104);
        blit[1](canvas, work->sheet, flameColumn->x - 8,
                flameColumn->y + wobble - 112, 17, 104 - wobble);
        blit[1](canvas, work->sheet + 0x6e8, flameColumn->x - 17,
                flameColumn->y - 65, 34, 65);
        if (flameColumn->y <= 111) {
          flameColumn->y += 16;
        } else {
          flameColumn->variant++;
        }
        if (flameColumn->y > 111) {
          emittedSparks = 0;
          sparkOriginX = flameColumn->x;
          if (flameColumn->variant <= 7) {
            work->shakeFrames = 4;
          }
          for (j = 0; j != NEPTUNE_PARTICLES; j++) {
            if (PARTICLES[j].variant == 0) {
              s32 r = 0x3ff;
              s32 a;
              r &= Random16();
              a = (Random16() & 0x7fff) - 0x4000;
              PARTICLES[j].x = sparkOriginX << 16;
              PARTICLES[j].y = ground << 16;
              r += 32;
              PARTICLES[j].velocityX = (Trig_Sin(a) * r) >> 7;
              PARTICLES[j].velocityY = -(Trig_Cos(a) * r * 2) >> 7;
              PARTICLES[j].variant = (Random16() & 7) + 32;
              if (++emittedSparks == 4) {
                break;
              }
            }
          }
        }
      }
    }
    EFFECT_FOR_EACH_TARGET(i, work->effect) {
      if (frame == i * 16 + 8) {
        ObjectGroup_UpdateMembers(work->effect->actors[i], SPRITE_COLOR_WHITE,
                                  POSE_DAMAGE, i, 8);
        BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 2);
      }
    }
    for (i = 0; i != NEPTUNE_PARTICLES; i++) {
      if (PARTICLES[i].variant > 0) {
        s32 x;
        s32 y;
        PARTICLES[i].variant--;
        EffectStep_AdvanceWithGravity2D(&PARTICLES[i], 60, 0x4000);
        y = PARTICLES[i].y >> 16;
        if (y > 120) {
          PARTICLES[i].velocityY = -PARTICLES[i].velocityY / 2;
        } else if (PARTICLES[i].x >= 0 && (x = PARTICLES[i].x >> 16) <= 126 &&
                   PARTICLES[i].y >= 0) {
          s32 k = PARTICLES[i].variant / 8 + 3;
          EFFECT_DRAW_MOTE(blit[i & 1], canvas, particleSheet, x, y, k);
        }
      }
    }
    Camera_ApplyShake(4, 8);
    EFFECT_END_FRAME(work);
  } while (++frame != 144);

  for (i = 0; i != NEPTUNE_OBJECTS; i++) {
    ResourceObject_ReleaseFar(work->objects[i]);
  }
  EFFECT_FINISH_CANVAS();
}

// Abilities: Thrash (kind 388).
//
// Hop the acting unit toward the first target; trigger type 4 after 29 frames.
void BattleFx_RunThrash(struct BattleEffectArgument *effect) {
  // Hop 3/4 of the distance at displacement / 24, initial vertical speed 12.2.
  BattleMotion_ApproachTargetFar(effect->actor, effect->actors[0], 24, 0xc3333);
  WaitFrames(29);
  effect->variant = 4;
  BattleFx_RunSparkGroups(effect, 2); // 2: from the acting unit's position
}

// Hop the acting unit toward the first target; trigger type 3 after 12 frames.
void BattleFx_RunSparkApproachVariant3(struct BattleEffectArgument *effect) {
  // Hop 3/4 of the distance at displacement / 24, initial vertical speed 7.2.
  BattleMotion_ApproachTargetFar(effect->actor, effect->actors[0], 24, 0x73333);
  WaitFrames(12);
  effect->variant = 3;
  BattleFx_RunSparkGroups(effect, 2); // 2: from the acting unit's position
}

// Smaller object effects: two hops that start a spark group, a four-frame
// image revealed in front of the target, and two mirrored columns rising at
// the target's feet.
//
// Every effect follows the canvas protocol: claim the canvas layer, load
// sheets, draw into the canvas each frame, present it, and finish the canvas
// at the end.

// Abilities: Quick Strike (kind 52).
DEFINE_EFFECT_KIND(BattleFx_RunQuickStrike, BattleFx_RunRevealColumn, 1)

// Abilities: Vorpal Slash (kind 65).
//
// Flash the target using the first pair of reveal-mask resources.
DEFINE_EFFECT_KIND(BattleFx_RunVorpalSlash, BattleFx_RunRevealColumn, 2)

// Draw four 120x120 frames on the canvas, four ticks each,
// then fill the canvas and strike the target.
// variant: 0-2 select the graphics
void BattleFx_RunRevealColumn(struct BattleEffectArgument *effect,
                              s32 variant) {
  void **heap;
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle draw[2];
  s32 frame;

  heap = gBattleFxWork;
  EFFECT_GET_WORK_AND_CANVAS(heap, work, canvas);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);
  REG_BLDCNT = 0;

  // Each pair of 120x120 frames fills the image storage before the particles.
  // Frames 1-2 use the work buffer; frames 3-4 use gMapCellBuffer.
  if (variant == 0) {
    Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskE, work->imageStorage,
                               1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskF, gMapCellBuffer, 1,
                               1);
  } else if (variant == 1) {
    Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskC, work->imageStorage,
                               1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskD, gMapCellBuffer, 1,
                               1);
  } else {
    Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskA, work->imageStorage,
                               1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskB, gMapCellBuffer, 1,
                               1);
  }
  work->transferMode = CANVAS_COPY_AND_CLEAR; // Clear to 0 each frame.
  work->transferValue = 0;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);

  // Move the acting unit toward the target over 16 frames.
  if (variant == 1)
    BattleMotion_ApproachTargetFar(effect->actor, effect->actors[0], 16,
                                   0x80000);
  else
    BattleMotion_ApproachTargetFar(effect->actor, effect->actors[0], 16, 0);
  WaitFrames(16);

  if (work->effect->side == 1)
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER,
                        COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X,
                        COMPOSITE_COPY);
  else
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                        COMPOSITE_COPY);
  draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
  AudioCommand_PlayFar(212);
  for (frame = 0; frame != 21; frame++) {
    if (frame <= 3)
      draw[0](canvas, work->imageStorage, 0, 0, 120, 120);
    else if (frame <= 7)
      draw[0](canvas, work->imageStorage + 0x3840, 0, 0, 120, 120);
    else if (frame <= 11)
      draw[0](canvas, gMapCellBuffer, 0, 0, 120, 120);
    else if (frame <= 15)
      draw[0](canvas, (u8 *)gMapCellBuffer + 0x3840, 0, 0, 120, 120);
    if (frame >= 16 && frame <= 19)
      Iwram_FillWords(canvas, 0x4000,
                      0x3f3f3f3f); // Fill the entire canvas with color 0x3f.
    if (frame == 18)
      BattleEventRuntime_BeginPhaseFar(134);
    if (frame == 20) {
      work->shakeFrames = 8;
      BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 4);
    }
    Camera_ApplyShake(16, 16);
    EFFECT_END_FRAME(work);
  }
  Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  BattleFx_EndCanvasLayer();
}

// Abilities: Undead Sword (kind 320).
//
// Two mirrored columns rise at the target's feet, scattering 16 sparks.
void BattleFx_RunUndeadSword(struct BattleEffectArgument *effect) {
  void **heap;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  void *sprites;
  DrawRectangle draw[2];
  struct EffectPosition screenPosition;
  struct EffectStep *spark;
  struct EffectStep *particle;
  s32 frame;
  s32 i;
  s32 height;
  s32 mode;
  s32 x;
  u8 *src;
  s32 speed;
  s32 angle;
  s32 cell;
  s32 size;

  EFFECT_GET_SLOTS(heap, cursor, work, canvas);
  work->effect = effect;
  sprites = heap[2];
  BattleFx_BeginCanvasLayer(0);
  REG_BG2PA = Q_8_8(1); // Normal horizontal scale too
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sprites, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_LimeColumnSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_BlastSheet, work->sheet + 0x460,
                             1, 0);
  BattleFx_FetchRectangleBlitters(work->effect->side, draw);
  work->transferMode = CANVAS_COPY_AND_FADE; // 3/4 afterimage
  work->transferValue = 75;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);

  // Shift so that canvas x = 64 lines up with the target.
  EffectPosition_ApplyAlternateStepAndYOffset(work->effect->actors[0],
                                              &screenPosition);
  REG_BG2X = (64 - screenPosition.x) << 8;

  // Scatter sparks in all directions from (64, 112), then drift upward.
  for (i = 0, spark = work->particles; i != 16; i++) {
    speed = (Random16() & 0x1ff) + 128;
    angle = Random16() & 0xffff;
    spark->x = 0x400000;
    spark->y = 0x700000;
    spark->velocityX = (Trig_Sin(angle) * speed) >> 8;
    spark->velocityY = (Trig_Cos(angle) * speed) >> 9;
    spark->variant = Random16() & 7;
    spark++;
  }
  work->shakeFrames = 8;
  for (frame = 0; frame != 54; frame++) {
    if (frame == 10) {
      work->shakeFrames = 8;
      BattleEventRuntime_BeginPhaseFar(212);
      BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 0);
      ObjectGroup_UpdateMembers(work->effect->actors[0], SPRITE_COLOR_WHITE,
                                POSE_DAMAGE, 0, 8);
    }

    // Columns grow 12 pixels per frame on frames 8-31, then shrink 6 per frame.
    // Use comparison compositing while held at 80 pixels, otherwise additive.
    if (frame > 7) {
      if (frame <= 31)
        height = frame * 12 - 96;
      else
        height = 272 - frame * 6;
      if (height > 0) {
        if (height > 80) {
          height = 80;
          mode = COMPOSITE_MAX;
        } else {
          mode = COMPOSITE_ADDITIVE;
        }
        for (i = 0, x = 50; i != 2; i++, x += 14) {
          if (i == 0)
            EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER,
                                COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, mode);
          else
            EFFECT_LOAD_BLITTER(
                HEAP_SLOT_BLITTER,
                COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X, mode);
          draw[0] = ((DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER]);
          ((DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER])(
              canvas, work, x, 112 - height, 14, height);
          Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
        }
      }
    }

    // Sparks advance to the next of seven images every three frames.
    BattleFx_FetchRectangleBlitters(work->effect->side, draw);
    for (i = 0, particle = work->particles; i != 16; i++) {
      if (frame >= i / 2 + 8 && particle->variant <= 28) {
        s32 px;
        s32 py;

        cell = particle->variant / 3;
        px = HI(particle->x);
        py = HI(particle->y);
        if (cell > 6)
          cell = 6;
        src = work->sheet + gUndeadSword_SparkCells[cell] + 0x460;
        size = gUndeadSword_SparkSizes[cell];
        draw[0](canvas, src, px - size / 2, py - size / 2, size, size);
        particle->variant++;
        EffectStep_AdvanceWithGravity2D(particle, 62, -0x2000);
      }
      particle++;
    }
    EFFECT_RELEASE_BLITTERS();
    if (frame <= 7)
      Camera_ApplyShake(2, 2);
    else
      Camera_ApplyShake(16, 16);
    EFFECT_END_FRAME(work);
  }
  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  BattleFx_EndCanvasLayer();
}

// Build specialized canvas blitters from the resident ARM kernels.
//
// Effects draw sprite cells onto a 128x128 canvas with small ARM routines
// built at run time for one cell size, one mirroring and one way of combining
// pixels. A general routine that tested all of these for every pixel would be
// far too slow, so each routine is spliced together from a template and the
// pixel fragments of its mode. Some template words are taken as they are,
// some are chosen by the flags, and some have an immediate or a branch
// displacement added before they are written.
//
// The generated routine has these blocks, in order:
//
// 1. Prologue: three entry words, the three-word additive mask for additive
//    compositing, then one of four alternatives that set up the walk for the
//    mirroring asked for (X only: 3 words, Y only: 4, both: 3, neither: 1).
// 2. Vertical clip, for vertically clipped cells only: eight words, the third
//    chosen by vertical mirroring and the last two carrying the cell height.
// 3. Columns: a test and a forward branch that skips the whole routine when
//    no rows remain, then either a width mask (unclipped) or twelve clip
//    words whose shifts carry the cell width as an ARM rotated immediate
//    (gBattleFx_BitOperands). A second test and forward branch skip
//    everything when no columns remain. Six words, then one or two, then
//    five more finish the set-up.
// 4. Row loop: four loop-head words carrying the row count and the width's
//    shift, and a fifth when unclipped; then for each row
//    a. a leading partial run, pixel by pixel up to the first 8-pixel
//       boundary, skipped by a forward branch when empty;
//    b. the full-cell loop, eight pixels per pass, from the mode's 8-pixel
//       fragment, skipped when there are no full cells;
//    c. a trailing partial run like (a).
//    Three words then advance to the next row and a fourth branches back to
//    the loop head.
// 5. Exit: the two skip branches of block 3 land here, on two final words.
//
// Where the template offers alternatives, the cursor always moves past all of
// them, whichever is written; so the walk ends at the template's last word
// for every combination, and BattleFx_LoadWork checks that it does. In each
// partial run and the full-cell loop the template keeps four words where the
// generated routine has its mode's fragment; those four are skipped too.
//
// Forward branches are written with a zero displacement, their word index
// remembered, and patched when the target is reached. Backward branches are
// computed at once. ARM branch displacements count words from the branch
// plus two, as the processor reads two words ahead.
//
// The same walk runs twice: first without an output buffer, to count the
// words, then into a heap block of exactly that size. Branch locations are
// word indices, so the counting pass needs no buffer.

#define DMA_COPY_WORDS(n) (0x84000000 | (n))

#define BRANCH_FIELD(site, target)                                             \
  ((((u32)(target) - (u32)(site) - 8) >> 2) & 0x00FFFFFF)

// The words of a partial run, one pixel at a time: the mode's one-pixel
// fragment, read forwards or, mirrored across, backwards. Each walk writes a
// leading and a trailing run.
#define BLITTER_PARTIAL_RUN(mode, flags)                                       \
  switch (mode) {                                                              \
  default:                                                                     \
  case COMPOSITE_COPY:                                                         \
    if (((flags) & COMPOSITE_FLIP_X) != 0) {                                   \
      COPY_FROM(BattleEffect_CopyReverse, 2);                                  \
    } else {                                                                   \
      COPY_FROM(BattleEffect_CopyForward, 2);                                  \
    }                                                                          \
    break;                                                                     \
  case COMPOSITE_TRANSPARENT:                                                  \
    if (((flags) & COMPOSITE_FLIP_X) != 0) {                                   \
      COPY_FROM(BattleEffect_TransparentReverse, 4);                           \
    } else {                                                                   \
      COPY_FROM(BattleEffect_TransparentForward, 4);                           \
    }                                                                          \
    break;                                                                     \
  case COMPOSITE_MAX:                                                          \
    if (((flags) & COMPOSITE_FLIP_X) != 0) {                                   \
      COPY_FROM(BattleEffect_MaxReverse, 4);                                   \
    } else {                                                                   \
      COPY_FROM(BattleEffect_MaxForward, 4);                                   \
    }                                                                          \
    break;                                                                     \
  case COMPOSITE_ADDITIVE:                                                     \
    if (((flags) & COMPOSITE_FLIP_X) != 0) {                                   \
      COPY_FROM(BattleEffect_AdditiveReverse, 6);                              \
    } else {                                                                   \
      COPY_FROM(BattleEffect_AdditiveForward, 6);                              \
    }                                                                          \
    break;                                                                     \
  }

// The words of the full-cell loop body, eight pixels per pass: the copy
// fragment eight times, or the mode's 8-pixel fragment (the 4-pixel maximum
// and colour fragments twice).
#define BLITTER_FULL_CELLS(mode, flags)                                        \
  switch (mode) {                                                              \
  default:                                                                     \
  case COMPOSITE_COPY: {                                                       \
    s32 i;                                                                     \
                                                                               \
    for (i = 0; i < 8; i++) {                                                  \
      if (((flags) & COMPOSITE_FLIP_X) != 0) {                                 \
        COPY_FROM(BattleEffect_CopyReverse, 2);                                \
      } else {                                                                 \
        COPY_FROM(BattleEffect_CopyForward, 2);                                \
      }                                                                        \
    }                                                                          \
  } break;                                                                     \
  case COMPOSITE_TRANSPARENT:                                                  \
    if (((flags) & COMPOSITE_FLIP_X) != 0) {                                   \
      COPY_FROM(BattleEffect_Transparent8Reverse, 25);                         \
    } else {                                                                   \
      COPY_FROM(BattleEffect_Transparent8Forward, 25);                         \
    }                                                                          \
    break;                                                                     \
  case COMPOSITE_MAX:                                                          \
    if (((flags) & COMPOSITE_FLIP_X) != 0) {                                   \
      COPY_TWICE(BattleEffect_Max4Reverse, 16);                                \
    } else {                                                                   \
      COPY_TWICE(BattleEffect_Max4Forward, 16);                                \
    }                                                                          \
    break;                                                                     \
  case COMPOSITE_ADDITIVE:                                                     \
    if (((flags) & COMPOSITE_FLIP_X) != 0) {                                   \
      COPY_TWICE(BattleEffect_ColorReverse, 14);                               \
    } else {                                                                   \
      COPY_TWICE(BattleEffect_ColorForward, 14);                               \
    }                                                                          \
    break;                                                                     \
  }

// Install a canvas blitter in the requested heap slot. widthShift and
// heightShift are log2 dimensions; flags select clipping and mirroring, and
// mode selects how source pixels combine with the destination.
// Returns whether the template walk reached the expected kernel boundary.
s32 BattleFx_LoadWork(s32 slot, s32 widthShift, s32 heightShift, s32 flags,
                      u32 mode) {
  const u32 *templateWords;
  u32 *codeWords;
  u32 *emptyRowsBranch;
  u32 *emptyColumnsBranch;
  u32 *rowLoop;
  u32 *skipBranch;
  u32 *partialRun;
  u32 *fullCellLoop;
  const u32 *codeEnd;
  s32 wordCount;

#define PUT(word) wordCount++
#define COPY(n)                                                                \
  wordCount += (n);                                                            \
  templateWords += (n)
#define COPY_FROM(from, n) wordCount += (n)
#define COPY_TWICE(from, n)                                                    \
  wordCount += (n);                                                            \
  wordCount += (n)
#define PUT_BRANCH(target) wordCount++
#define MARK(site)
#define PATCH(site)

  // First walk: count the words selected by clipping, mirroring and blend mode.
  wordCount = 0;
  templateWords = BattleEffect_Composite;

  COPY(3);
  if (mode == COMPOSITE_ADDITIVE) {
    COPY_FROM(BattleEffect_Mask, 3);
  }

  if ((flags & COMPOSITE_FLIP) == COMPOSITE_FLIP_X) {
    COPY_FROM(templateWords, 3);
  }
  templateWords += 3;
  if ((flags & COMPOSITE_FLIP) == COMPOSITE_FLIP_Y) {
    COPY_FROM(templateWords, 4);
  }
  templateWords += 4;
  if ((flags & COMPOSITE_FLIP) == COMPOSITE_FLIP) {
    COPY_FROM(templateWords, 3);
  }
  templateWords += 3;
  if ((flags & COMPOSITE_FLIP) == 0) {
    PUT(templateWords[0]);
  }
  templateWords += 1;

  if ((flags & COMPOSITE_CLIP_Y) != 0) {
    PUT(templateWords[0]);
    PUT(templateWords[1]);
    if ((flags & COMPOSITE_FLIP_Y) != 0) {
      PUT(templateWords[2]);
    } else {
      PUT(templateWords[3]);
    }
    PUT(templateWords[4]);
    PUT(templateWords[5]);
    PUT(templateWords[6] + (1 << heightShift));
    PUT(templateWords[7] + (1 << heightShift));
  }
  templateWords += 8;

  PUT(*templateWords++);
  MARK(emptyRowsBranch);
  PUT(*templateWords++);

  if ((flags & COMPOSITE_CLIP_X) == 0) {
    PUT(templateWords[0] + (1 << widthShift) - 1);
  }
  templateWords += 1;
  if ((flags & COMPOSITE_CLIP_X) != 0) {
    PUT(templateWords[0]);
    PUT(templateWords[1]);
    if ((flags & COMPOSITE_FLIP_X) != 0) {
      PUT(templateWords[2]);
      PUT(templateWords[3]);
    } else {
      PUT(templateWords[4]);
      PUT(templateWords[5]);
    }
    PUT(templateWords[6]);
    PUT(templateWords[7]);
    PUT(templateWords[8] + gBattleFx_BitOperands[widthShift]);
    if ((flags & COMPOSITE_FLIP_X) != 0) {
      PUT(templateWords[9]);
    } else {
      PUT(templateWords[10]);
    }
    PUT(templateWords[11] + gBattleFx_BitOperands[widthShift]);
  }
  templateWords += 12;

  PUT(*templateWords++);
  MARK(emptyColumnsBranch);
  PUT(*templateWords++);

  COPY(6);

  PUT(*templateWords++);
  if ((flags & COMPOSITE_CLIP_X) == 0) {
    PUT(templateWords[0]);
  }
  templateWords += 1;

  COPY(5);

  MARK(rowLoop);
  PUT(templateWords[0]);
  PUT(templateWords[1] + (1 << (heightShift - 3)) - 1);
  PUT(templateWords[2] + ((widthShift - 3) << 7));
  PUT(templateWords[3]);
  templateWords += 4;

  if ((flags & COMPOSITE_CLIP_X) == 0) {
    PUT(templateWords[0]);
  }
  templateWords += 1;

  PUT(*templateWords++);
  MARK(skipBranch);
  PUT(*templateWords++);
  if ((flags & COMPOSITE_FLIP_X) != 0) {
    PUT(templateWords[0]);
  } else {
    PUT(templateWords[1]);
  }
  templateWords += 2;

  MARK(partialRun);
  BLITTER_PARTIAL_RUN(mode, flags);
  templateWords += 4;

  PUT(*templateWords++);
  PUT_BRANCH(partialRun);
  PUT(*templateWords++);
  if ((flags & COMPOSITE_CLIP_X) == 0) {
    PUT(templateWords[0]);
    PUT(templateWords[1]);
  }
  templateWords += 2;
  PATCH(skipBranch);

  PUT(*templateWords++);
  MARK(skipBranch);
  PUT(*templateWords++);

  MARK(fullCellLoop);
  BLITTER_FULL_CELLS(mode, flags);
  templateWords += 4;

  PUT(*templateWords++);
  if ((flags & COMPOSITE_CLIP_X) == 0) {
    PUT(templateWords[0]);
    PUT(templateWords[1]);
  }
  templateWords += 2;

  PUT(*templateWords++);
  PUT_BRANCH(fullCellLoop);
  PATCH(skipBranch);

  PUT(*templateWords++);
  MARK(skipBranch);
  PUT(*templateWords++);
  if ((flags & COMPOSITE_FLIP_X) != 0) {
    PUT(templateWords[0]);
  } else {
    PUT(templateWords[1]);
  }
  templateWords += 2;

  MARK(partialRun);
  BLITTER_PARTIAL_RUN(mode, flags);
  templateWords += 4;

  PUT(*templateWords++);
  PUT_BRANCH(partialRun);
  PATCH(skipBranch);

  COPY(3);

  PUT_BRANCH(rowLoop);
  PATCH(emptyRowsBranch);
  PATCH(emptyColumnsBranch);

  PUT(*templateWords++);
  PUT(*templateWords++);

#undef PUT
#undef COPY
#undef COPY_FROM
#undef COPY_TWICE
#undef PUT_BRANCH
#undef MARK
#undef PATCH

  /* FAKEMATCH: one-pass blocks keep each writing step together and in list
   * order while allowing scheduling within it. Measured 2026-10-13 in all six
   * editions, written as plain statements: PUT differs 38, COPY 12 and PATCH 2,
   * so each of the three carries a tag and counts at its uses. COPY_FROM,
   * COPY_TWICE and PUT_BRANCH match without a block and have none. */
#define PUT(word)                                                              \
  do {                                                                         \
    *codeWords++ = (word);                                                     \
  } while (0)
  /* FAKEMATCH: a one-pass block, as PUT's tag explains; written as plain
   * statements, COPY differs 12 in all six editions. */
#define COPY(n)                                                                \
  do {                                                                         \
    Dma_Set(templateWords, codeWords, DMA_COPY_WORDS(n), REG_DMA3);            \
    codeWords += (n);                                                          \
    templateWords += (n);                                                      \
  } while (0)
#define COPY_FROM(from, n)                                                     \
  Dma_Set(from, codeWords, DMA_COPY_WORDS(n), REG_DMA3);                       \
  codeWords += (n)
#define COPY_TWICE(from, n)                                                    \
  {                                                                            \
    const u32 *part = (from);                                                  \
    Dma_Set(part, codeWords, DMA_COPY_WORDS(n), REG_DMA3);                     \
    codeWords += (n);                                                          \
    Dma_Set(part, codeWords, DMA_COPY_WORDS(n), REG_DMA3);                     \
    codeWords += (n);                                                          \
  }
#define PUT_BRANCH(target)                                                     \
  *codeWords = *templateWords++ + BRANCH_FIELD(codeWords, target);             \
  codeWords++
#define MARK(site) site = codeWords
  /* FAKEMATCH: a one-pass block, as PUT's tag explains; written as a plain
   * statement, PATCH differs 2 in all six editions. */
#define PATCH(site)                                                            \
  do {                                                                         \
    *(site) |= BRANCH_FIELD(site, codeWords);                                  \
  } while (0)

  // Second walk: allocate, copy the selected kernels and patch their branches.
  codeWords = (u32 *)Runtime_AllocateHeapBlock(slot, wordCount << 2);
  templateWords = BattleEffect_Composite;

  COPY(3);
  if (mode == COMPOSITE_ADDITIVE) {
    COPY_FROM(BattleEffect_Mask, 3);
  }

  if ((flags & COMPOSITE_FLIP) == COMPOSITE_FLIP_X) {
    COPY_FROM(templateWords, 3);
  }
  templateWords += 3;
  if ((flags & COMPOSITE_FLIP) == COMPOSITE_FLIP_Y) {
    COPY_FROM(templateWords, 4);
  }
  templateWords += 4;
  if ((flags & COMPOSITE_FLIP) == COMPOSITE_FLIP) {
    COPY_FROM(templateWords, 3);
  }
  templateWords += 3;
  if ((flags & COMPOSITE_FLIP) == 0) {
    PUT(templateWords[0]);
  }
  templateWords += 1;

  if ((flags & COMPOSITE_CLIP_Y) != 0) {
    PUT(templateWords[0]);
    PUT(templateWords[1]);
    if ((flags & COMPOSITE_FLIP_Y) != 0) {
      PUT(templateWords[2]);
    } else {
      PUT(templateWords[3]);
    }
    PUT(templateWords[4]);
    PUT(templateWords[5]);
    PUT(templateWords[6] + (1 << heightShift));
    PUT(templateWords[7] + (1 << heightShift));
  }
  templateWords += 8;

  PUT(*templateWords++);
  MARK(emptyRowsBranch);
  PUT(*templateWords++);

  if ((flags & COMPOSITE_CLIP_X) == 0) {
    PUT(templateWords[0] + (1 << widthShift) - 1);
  }
  templateWords += 1;
  if ((flags & COMPOSITE_CLIP_X) != 0) {
    PUT(templateWords[0]);
    PUT(templateWords[1]);
    if ((flags & COMPOSITE_FLIP_X) != 0) {
      PUT(templateWords[2]);
      PUT(templateWords[3]);
    } else {
      PUT(templateWords[4]);
      PUT(templateWords[5]);
    }
    PUT(templateWords[6]);
    PUT(templateWords[7]);
    PUT(templateWords[8] + gBattleFx_BitOperands[widthShift]);
    if ((flags & COMPOSITE_FLIP_X) != 0) {
      PUT(templateWords[9]);
    } else {
      PUT(templateWords[10]);
    }
    PUT(templateWords[11] + gBattleFx_BitOperands[widthShift]);
  }
  templateWords += 12;

  PUT(*templateWords++);
  MARK(emptyColumnsBranch);
  PUT(*templateWords++);

  COPY(6);

  PUT(*templateWords++);
  if ((flags & COMPOSITE_CLIP_X) == 0) {
    PUT(templateWords[0]);
  }
  templateWords += 1;

  COPY(5);

  MARK(rowLoop);
  PUT(templateWords[0]);
  PUT(templateWords[1] + (1 << (heightShift - 3)) - 1);
  PUT(templateWords[2] + ((widthShift - 3) << 7));
  PUT(templateWords[3]);
  templateWords += 4;

  if ((flags & COMPOSITE_CLIP_X) == 0) {
    PUT(templateWords[0]);
  }
  templateWords += 1;

  PUT(*templateWords++);
  MARK(skipBranch);
  PUT(*templateWords++);
  if ((flags & COMPOSITE_FLIP_X) != 0) {
    PUT(templateWords[0]);
  } else {
    PUT(templateWords[1]);
  }
  templateWords += 2;

  MARK(partialRun);
  BLITTER_PARTIAL_RUN(mode, flags);
  templateWords += 4;

  PUT(*templateWords++);
  PUT_BRANCH(partialRun);
  PUT(*templateWords++);
  if ((flags & COMPOSITE_CLIP_X) == 0) {
    PUT(templateWords[0]);
    PUT(templateWords[1]);
  }
  templateWords += 2;
  PATCH(skipBranch);

  PUT(*templateWords++);
  MARK(skipBranch);
  PUT(*templateWords++);

  MARK(fullCellLoop);
  BLITTER_FULL_CELLS(mode, flags);
  templateWords += 4;

  PUT(*templateWords++);
  if ((flags & COMPOSITE_CLIP_X) == 0) {
    PUT(templateWords[0]);
    PUT(templateWords[1]);
  }
  templateWords += 2;

  PUT(*templateWords++);
  PUT_BRANCH(fullCellLoop);
  PATCH(skipBranch);

  PUT(*templateWords++);
  MARK(skipBranch);
  PUT(*templateWords++);
  if ((flags & COMPOSITE_FLIP_X) != 0) {
    PUT(templateWords[0]);
  } else {
    PUT(templateWords[1]);
  }
  templateWords += 2;

  MARK(partialRun);
  BLITTER_PARTIAL_RUN(mode, flags);
  templateWords += 4;

  PUT(*templateWords++);
  PUT_BRANCH(partialRun);
  PATCH(skipBranch);

  COPY(3);

  PUT_BRANCH(rowLoop);
  PATCH(emptyRowsBranch);
  PATCH(emptyColumnsBranch);

  PUT(*templateWords++);
  PUT(*templateWords++);

  codeEnd = (const u32 *)gBattleFx_MoteCellOffsets;
  return templateWords == codeEnd;
}

// These macros describe the two walks above; they are not an effect API.
#undef PUT
#undef COPY
#undef COPY_FROM
#undef COPY_TWICE
#undef PUT_BRANCH
#undef MARK
#undef PATCH
#undef BLITTER_PARTIAL_RUN
#undef BLITTER_FULL_CELLS
#undef BRANCH_FIELD
#undef DMA_COPY_WORDS
