#ifndef ALCHEMY_EFFECT_INTERNAL_H
#define ALCHEMY_EFFECT_INTERNAL_H

/// @file
/// What every part of the battle effect object shares: its includes, macros
/// and frame-loop helpers. effect.c includes it before the parts, and each
/// part includes it too, so that a part read on its own still resolves.

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
#include "songs.h"
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
#include "battle_system.h"
#include "battle_session.h"
#include "animspr.h"
#include "battle_unit.h"
#include "fixed_point_position.h"
#include "irq.h"
#include "unit/effect/effect_bits.h"
#include "call.h"
#include "macros.h"

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

/// The battle screen's display mode while an effect plays: background mode 1
/// with BG0 to BG2, sprites and both windows on.
#define EFFECT_DISPCNT                                                         \
  (DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG1_ON |     \
   DISPCNT_BG2_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON | DISPCNT_WIN1_ON)

/// Window 0, the battle view, shows BG0 to BG2, sprites and blending. Effects
/// pick window 1's layers themselves.
#define EFFECT_WIN0_LAYERS                                                     \
  (WININ_WIN0_BG0 | WININ_WIN0_BG1 | WININ_WIN0_BG2 | WININ_WIN0_OBJ |         \
   WININ_WIN0_CLR)

/// Outside the battle view: BG0 and blending only.
#define EFFECT_WINOUT (WINOUT_WIN01_BG0 | WINOUT_WIN01_CLR | WINOUT_WINOBJ_ALL)

/// A work slot read through a pointer to the effect's own slot,
/// gWorkSlot[HEAP_SLOT_BATTLE_EFFECT].
#define EFFECT_SLOT(slots, slot) ((slots)[(slot) - HEAP_SLOT_BATTLE_EFFECT])
/// A work slot read through a pointer to the canvas slot,
/// gWorkSlot[HEAP_SLOT_BATTLE_CANVAS], just after the effect's own.
#define CANVAS_SLOT(slots, slot) ((slots)[(slot) - HEAP_SLOT_BATTLE_CANVAS])

/// Index of DMAxCNT_H, the control half, in a channel's 16-bit registers.
#define DMA_CONTROL_HALF 5

/// DMA control word that fills n words from one fixed source word.
#define DMA_FILL_WORDS(n) ((DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | (n))

/// Stops a DMA channel: clears its repeat and start timing bits, then its
/// enable bit, and reads the control register back so the stop takes effect
/// before the channel is set up again. The HBlank DMA arms in display.c,
/// dispatch.c, burst_shower.c and particle_streams.c call it first.
static inline void Dma_Stop(volatile u16 *channel) {
  channel[DMA_CONTROL_HALF] &= (u16)~DMA_START_TIMING_DRQ_REPEAT;
  channel[DMA_CONTROL_HALF] &= (u16)~DMA_ENABLE;
  (void)channel[DMA_CONTROL_HALF];
}

/// How BattleFx_ProcessPendingGraphicsTransfer moves the effect's
/// 128x128 canvas into VRAM once a frame (work->transferMode); the canvas
/// pixels are indices into a palette ramp, so scaling them fades a trail.
enum CanvasTransferMode {
  CANVAS_COPY,             ///< copy as drawn
  CANVAS_COPY_AND_CLEAR,   ///< copy, then fill the canvas with transferValue
  CANVAS_COPY_AND_FADE,    ///< copy, keeping 50% or 75% (transferValue) of each
                           ///< pixel in the canvas for the next frame
  CANVAS_COPY_AND_DARKEN,  ///< copy, then darken the canvas by transferValue
  CANVAS_COPY_AND_BRIGHTEN ///< copy, then brighten the canvas by transferValue
};

/// The work block and canvas occupy adjacent slots, starting at
/// HEAP_SLOT_BATTLE_EFFECT. A cursor reads them in that order.
#define EFFECT_GET_WORK_AND_CANVAS(cursor, work, canvas)                       \
  ((work) = *(cursor)++, (canvas) = *(cursor))

/// Starts at the effect's entry in the whole work-slot array. Earlier slots,
/// such as the camera, then stay within that array while cursor walks ahead.
#define EFFECT_GET_SLOTS(slots, cursor, work, canvas)                          \
  ((slots) = &gWorkSlot[HEAP_SLOT_BATTLE_EFFECT], (cursor) = (slots),          \
   EFFECT_GET_WORK_AND_CANVAS(cursor, work, canvas))

/// Marks particles[0] to particles[count - 1] free: a particle whose variant
/// is -1 is not drawn or moved until an effect gives it a life.
#define EFFECT_FREE_PARTICLES(i, particles, count)                             \
  for ((i) = 0; (i) != (count); (i)++)                                         \
  (particles)[i].variant = -1

/// Starts a frame's 3D projection from the battle camera: resets the
/// transform, then looks from camera->eye towards camera->pos. Effects that
/// place pictures at battle positions do this before projecting them.
#define EFFECT_VIEW_FROM_CAMERA(camera)                                        \
  (Render_ResetTransformState(),                                               \
   Transform_SetLookAt((camera)->eye, (camera)->pos))

/// Strikes target `i`: it flashes white in its damage pose for `frames`
/// frames and plays battle motion `motion`
/// (BattleMotion_ApplyVariantMotionFar).
#define EFFECT_STRIKE_TARGET(effect, i, frames, motion)                        \
  (ObjectGroup_UpdateMembers((effect)->actors[i], SPRITE_COLOR_WHITE,          \
                             POSE_DAMAGE, i, frames),                          \
   BattleMotion_ApplyVariantMotionFar((effect)->actors[i], motion))

// The canvas life cycle, shared by every canvas effect. The statement macros
// are comma expressions, not do { } while (0) blocks: agscc compiles a
// do-while (0) in an effect's frame loop differently at some sites, and the
// comma form matches at all of them.

/// Lets the battle show the damage: resumes its paused playback, which plays
/// hitSound (an SE_ id; 0 keeps the one already queued) as the targets react.
#define EFFECT_SHOW_DAMAGE(hitSound) BattleEventRuntime_BeginPhaseFar(hitSound)

/// The canvas: 128x128 pixels, one byte each, in its heap slot and in VRAM at
/// BG_CHAR_BLOCK(1).
#define CANVAS_BYTES (128 * 128)

/// Opens the canvas layer at the canvas's own width, one texel per pixel,
/// where BattleFx_BeginCanvasLayer stretches it to twice the width.
#define EFFECT_BEGIN_NARROW_CANVAS(bgControl)                                  \
  (BattleFx_BeginCanvasLayer(bgControl), REG_BG2PA = Q_8_8(1))

/// Builds a blitter for the 128x128 canvas (1 << 7 pixels each way) in work
/// slot `slot`: BattleFx_BuildBlitter's drawing routine for one clip, mirror
/// and blend mode. Every canvas effect draws through one or two of these.
#define EFFECT_LOAD_BLITTER(slot, flags, mode)                                 \
  BattleFx_BuildBlitter(slot, 7, 7, flags, mode)

/// Sets how the canvas transfer treats what is left in the canvas after each
/// copy: `mode` with `value` (see enum CanvasTransferMode).
#define EFFECT_SET_TRANSFER(work, mode, value)                                 \
  ((work)->transferMode = (mode), (work)->transferValue = (value))

/// Sets the transfer, then starts copying the canvas to VRAM once a frame.
#define EFFECT_START_TRANSFER(work, mode, value)                               \
  (EFFECT_SET_TRANSFER(work, mode, value),                                     \
   Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer,      \
                                 EFFECT_CALLBACK_ORDER))

/// Ends a frame of a canvas effect: marks the canvas ready for the transfer
/// callback, then waits for the next frame. Nearly every effect loop ends so.
#define EFFECT_PRESENT_FRAME(work) ((work)->transferPending = 1, WaitFrames(1))

/// The usual end of an effect frame: counts down the targets' flash timers
/// (ObjectGroup_TickMemberTimers), then presents the canvas.
#define EFFECT_END_FRAME(work)                                                 \
  (ObjectGroup_TickMemberTimers(), EFFECT_PRESENT_FRAME(work))

/// Frees the two blitters, the alternate first.
#define EFFECT_RELEASE_BLITTERS()                                              \
  (Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE),                      \
   Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER))

/// The usual end of a canvas effect: stops the transfer callback before
/// freeing what it reads, then the blitters, then the canvas layer.
#define EFFECT_FINISH_CANVAS()                                                 \
  (Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer),          \
   EFFECT_RELEASE_BLITTERS(), BattleFx_EndCanvasLayer())

/// The same for an effect that drew through one blitter.
#define EFFECT_FINISH_CANVAS_ONE_BLITTER()                                     \
  (Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer),          \
   Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER), BattleFx_EndCanvasLayer())

/// The bytes of one set of the ten mote cells: 2 * (1 + 4 + ... + 100).
#define MOTE_SET_BYTES 770

/// Particle sheets hold ten mote cells, 1x2 to 10x20 pixels. Draws mote cell
/// `size` (size pixels wide, twice that tall) of `sheet` through blitter
/// `draw`, centred across x and standing on y: the motes, sparks and dust of
/// 32 effects, from the mote fountain to the summons.
#define EFFECT_DRAW_MOTE(draw, canvas, sheet, x, y, size)                      \
  (draw)(canvas, (sheet) + gBattleFx_MoteCells[(size) - 1], (x) - (size) / 2,  \
         (y) - (size), size, (size) * 2)

/// Draws flare cell `size` of `sheet`, a square `size` pixels from its centre
/// to each edge, centred on (x, y): Douse's sparks and the glowing points of
/// the element pillar and burst, stat boosts, beams and rings.
#define EFFECT_DRAW_FLARE(draw, canvas, sheet, x, y, size)                     \
  (draw)(canvas, (sheet) + gBattleFx_FlareCells[(size) - 1], (x) - (size),     \
         (y) - (size), (size) * 2, (size) * 2)

/// Shows the canvas: the effect display mode, BG2 stretched to twice its width
/// (the 128-pixel canvas shown 256 pixels wide) and blended over everything
/// below: `top` sixteenths of the canvas's colour are added to all of what
/// lies under it, so the canvas brightens what it covers.
#define EFFECT_SHOW_WIDE_CANVAS(top)                                           \
  (REG_DISPCNT = EFFECT_DISPCNT, REG_BG2PA = Q_8_8(0.5),                       \
   REG_BLDALPHA = BLEND_ALPHA(top, 16),                                        \
   REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL)

/// A WIN0H value whose left edge is past its right edge. The hardware then
/// takes the right edge as 240, so the span is not empty in itself; the
/// effects write it only on lines outside window 0's vertical range.
#define WINDOW_SPAN_INVERTED WIN_RANGE(1, 0)

/// The scheduler order of every per-frame effect callback
/// (Scheduler_AddOrUpdateCallback sorts the frame's callbacks by order).
#define EFFECT_CALLBACK_ORDER 0x480
/// The order where the battle's own playback runs, which the palette fade and
/// the element streaks take.
#define EFFECT_PLAYBACK_ORDER 0xc80

/// The full-screen picture Graphics_ConvertBackgroundToBlueRamp (bind.c)
/// works on, which Condemn draws too.
#define BLUE_RAMP_PIXELS Ram_MapCellBuffer

/// Caps a colour level at cap, and at 0 from below. Fades draw from darker
/// copies of a picture built before the first frame, copy i capped at
/// 64 - 7i.
#define EFFECT_CAP_LEVEL(level, cap)                                           \
  {                                                                            \
    if ((level) > (cap))                                                       \
      (level) = (cap);                                                         \
    if ((level) < 0)                                                           \
      (level) = 0;                                                             \
  }

/// Closing bolts and lightning columns use the same 24x120 picture cells.
#define LIGHTNING_BOLT_CELL_BYTES 2880

/// Windows 0 and 1 both cover the battle view: the full width, lines 16 to
/// 135.
#define EFFECT_SET_BATTLE_WINDOWS()                                            \
  (REG_WIN0H = WIN_RANGE(0, 240), REG_WIN0V = WIN_RANGE(16, 136),              \
   REG_WIN1H = WIN_RANGE(0, 240), REG_WIN1V = WIN_RANGE(16, 136))

/// BG2 shows the canvas from screen line 16 down (BG2Y is -16.0), stretched to
/// twice its width: the 128x128 canvas across the battle view.
#define EFFECT_SET_CANVAS_AFFINE()                                             \
  (REG_BG2X = 0, REG_BG2Y = -0x1000, REG_BG2PA = Q_8_8(0.5), REG_BG2PB = 0,    \
   REG_BG2PC = 0, REG_BG2PD = Q_8_8(1))

/// Writes one row of BG2's map for the canvas: its 16 tiles of row `row` in
/// order. An affine map has one byte per tile, so they are written two at a
/// time, eight halfwords from `offset` on.
#define EFFECT_WRITE_CANVAS_MAP_ROW(row, column, offset)                       \
  for ((column) = 0; (column) != 8; (column)++, (offset) += 2) {               \
    s32 tileIndex;                                                             \
    s16 pair;                                                                  \
                                                                               \
    tileIndex = (row) * 16 + (column) * 2;                                     \
    pair = ((tileIndex + 1) << 8) | tileIndex;                                 \
    *(volatile s16 *)((u8 *)BG_CANVAS_MAP + (offset)) = pair;                  \
  }

#endif // ALCHEMY_EFFECT_INTERNAL_H
