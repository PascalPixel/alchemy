// The battle effect object: everything drawn over the battle while an
// ability plays. This file holds what every effect shares (includes, macros
// and the frame-loop helpers); the effects are in the files under parts/,
// included at the end in the order their code has in the ROM. That order
// mixes families, Djinn, summons and shared machinery, and each part is one
// run of it, named for what it holds. Camelot's own files are not
// recoverable: the ROM shows one object, and these parts are ours, cut where
// one family ends and the next begins.
//
// To find the effect a move plays, search for the move's English name: each
// handler's "Abilities:" comment lists the abilities that reach it, and each
// row of gBattleFx_EffectHandlers (effect_table.c) gives its kind, its part
// and its first ability. Functions here are BattleFx_ unless they act on one
// thing the engine names elsewhere: those keep its prefix (Camera_,
// Palette_, Graphics_, Resource_, EffectStep_, EffectPosition_,
// ObjectGroup_), as their Lost Age twins do. A part's macros are its own;
// one that two parts use is defined in this file. A work slot is read as
// WORK_SLOT(n) or as gWorkSlot[n], whichever the site matches with: in this
// GCC the two can compile differently. The shared machinery:
//
//   Entry points    dispatch.c: BattleFx_RunTableEffect,
//                   BattleFx_RunSceneEffect; positions.c:
//                   BattleFx_DispatchByIdRange
//   Canvas          canvas.c: BattleFx_BeginCanvasLayer,
//                   BattleFx_EndCanvasLayer, BattleFx_WipeCanvas,
//                   BattleFx_ProcessPendingGraphicsTransfer
//   Blitters        blitter.c: BattleFx_BuildBlitter; mirrored_wall.c:
//                   BattleFx_FetchRectangleBlitters
//   Motion          positions.c: EffectStep_AdvanceWithGravity2D and 3D,
//                   BattleUnit_ProjectToScreen; hail.c: Camera_ApplyShake
//   Targets         dispatch.c: BattleFx_SelectLivingTargets;
//                   helm_splitter.c: BattleFx_PlaceFormationObjects
//
// README.md in this folder explains the ideas: the canvas, blitters, the
// frame loop, particles, pictures and how to change an effect.

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

// Index of DMAxCNT_H, the control half, in a channel's 16-bit registers.
#define DMA_CONTROL_HALF 5

// DMA control word that copies one halfword per HBlank, repeating, with the
// destination reloaded each line: the scanline table that drives a window
// edge or a scroll register.
#define DMA_HBLANK_HALFWORD_PER_LINE 0xa2600001

// The same a word at a time: BG2's affine reference point.
#define DMA_HBLANK_WORD_PER_LINE 0xa6600001

// DMA control word that fills n words from one fixed source word.
#define DMA_FILL_WORDS(n) (DMA_ENABLE32 | DMA_32BIT | DMA_SRC_FIXED32 | (n))

// Stops a DMA channel: clears its repeat and start timing bits, then its
// enable bit, and reads the control register back so the stop takes effect
// before the channel is set up again.
static inline void Dma_Stop(volatile u16 *channel) {
  channel[DMA_CONTROL_HALF] &= (u16)~DMA_START_TIMING_DRQ_REPEAT;
  channel[DMA_CONTROL_HALF] &= (u16)~DMA_ENABLE;
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

// Strikes target `i`: it flashes white in its damage pose for `frames`
// frames and plays battle motion `motion` (BattleMotion_ApplyVariantMotionFar).
#define EFFECT_STRIKE_TARGET(effect, i, frames, motion)                        \
  (ObjectGroup_UpdateMembers((effect)->actors[i], SPRITE_COLOR_WHITE,          \
                             POSE_DAMAGE, i, frames),                          \
   BattleMotion_ApplyVariantMotionFar((effect)->actors[i], motion))

// The canvas life cycle, shared by every canvas effect. The statement macros
// are comma expressions, not do { } while (0) blocks: agscc compiles a
// do-while (0) in an effect's frame loop differently at some sites, and the
// comma form matches at all of them.

// Opens the canvas layer at the canvas's own width, one texel per pixel,
// where BattleFx_BeginCanvasLayer stretches it to twice the width.
#define EFFECT_BEGIN_NARROW_CANVAS(bgControl)                                  \
  (BattleFx_BeginCanvasLayer(bgControl), REG_BG2PA = Q_8_8(1))

// Builds a blitter for the 128x128 canvas (1 << 7 pixels each way) in work
// slot `slot`: BattleFx_BuildBlitter's drawing routine for one clip, mirror and
// blend mode. Every canvas effect draws through one or two of these.
#define EFFECT_LOAD_BLITTER(slot, flags, mode)                                 \
  BattleFx_BuildBlitter(slot, 7, 7, flags, mode)

// Sets how the canvas transfer treats what is left in the canvas after each
// copy: `mode` with `value` (see enum CanvasTransferMode).
#define EFFECT_SET_TRANSFER(work, mode, value)                                 \
  ((work)->transferMode = (mode), (work)->transferValue = (value))

// Sets the transfer, then starts copying the canvas to VRAM once a frame.
#define EFFECT_START_TRANSFER(work, mode, value)                               \
  (EFFECT_SET_TRANSFER(work, mode, value),                                     \
   Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer,      \
                                 EFFECT_CALLBACK_ORDER))

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
  (draw)(canvas, (sheet) + gBattleFx_MoteCells[(size) - 1],              \
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

// Used for clipping canvas lines and comparing projectile travel distances.
// Scheduler orders (Scheduler_AddOrUpdateCallback sorts the frame's callbacks
// by them): every per-frame effect callback takes EFFECT_CALLBACK_ORDER, and
// the palette fade EFFECT_FADE_ORDER, where the battle's own playback runs.
#define EFFECT_CALLBACK_ORDER 0x480
#define EFFECT_FADE_ORDER 0xc80

// The full-screen picture Graphics_ConvertBackgroundToBlueRamp (bind.c)
// works on, which Condemn draws too.
#define BLUE_RAMP_PIXELS Ram_MapCellBuffer

#define EFFECT_ABS(v) ((v) < 0 ? -(v) : (v))

// Caps a colour level at cap, and at 0 from below. Fades draw from darker
// copies of a picture built before the first frame, copy i capped at
// 64 - 7i.
#define EFFECT_CAP_LEVEL(level, cap)                                           \
  {                                                                            \
    if ((level) > (cap))                                                       \
      (level) = (cap);                                                         \
    if ((level) < 0)                                                           \
      (level) = 0;                                                             \
  }

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

// The effect object's code, in ROM order. Each part is compiled here, as
// part of this one object, never on its own: the order of these includes is
// the order of the code in the ROM.
#include "parts/display.c"
#include "parts/falling.c"
#include "parts/streaks.c"
#include "parts/target_strike.c"
#include "parts/death_plunge.c"
#include "parts/spider_web.c"
#include "parts/canvas.c"
#include "parts/planet_diver.c"
#include "parts/gathering_lights.c"
#include "parts/mirrored_wall.c"
#include "parts/djinn_beams.c"
#include "parts/lobbed_shells.c"
#include "parts/condemn.c"
#include "parts/bind.c"
#include "parts/stars.c"
#include "parts/procne.c"
#include "parts/tiamat.c"
#include "parts/nereid.c"
#include "parts/vortices.c"
#include "parts/prism.c"
#include "parts/plasma.c"
#include "parts/volcano.c"
#include "parts/target_motes.c"
#include "parts/djinn_heal.c"
#include "parts/dispatch.c"
#include "parts/cybele.c"
#include "parts/boreas.c"
#include "parts/drain.c"
#include "parts/healing.c"
#include "parts/stat_change.c"
#include "parts/flare.c"
#include "parts/quake.c"
#include "parts/fire.c"
#include "parts/frost.c"
#include "parts/ray.c"
#include "parts/eruption.c"
#include "parts/burst_shower.c"
#include "parts/orbiting.c"
#include "parts/atalanta.c"
#include "parts/growth.c"
#include "parts/bolt.c"
#include "parts/projectiles.c"
#include "parts/rushing_strike.c"
#include "parts/djinn_attacks.c"
#include "parts/hail.c"
#include "parts/thor.c"
#include "parts/spire.c"
#include "parts/burst_scene.c"
#include "parts/gaia.c"
#include "parts/positions.c"
#include "parts/beams.c"
#include "parts/casting_impact.c"
#include "parts/torch.c"
#include "parts/helm_splitter.c"
#include "parts/particle_streams.c"
#include "parts/ramses.c"
#include "parts/dragon_cloud.c"
#include "parts/annihilation.c"
#include "parts/ragnarok.c"
#include "parts/titan_blade.c"
#include "parts/judgment.c"
#include "parts/kirin.c"
#include "parts/neptune.c"
#include "parts/thrash.c"
#include "parts/undead_sword.c"
#include "parts/blitter.c"
