#ifndef ALCHEMY_BATTLE_EFFECT_INTERNAL_H
#define ALCHEMY_BATTLE_EFFECT_INTERNAL_H

/// @file
/// What every part of the battle effect object shares: its includes, macros
/// and frame-loop helpers. battle_effect.c includes it before the parts, and
/// each part includes it too, so that a part read on its own still resolves.

#include "module/battle_effect/battle_effect_table.h"
#include "module/battle_effect/battle_effect.h"
#include "module/overworld/overworld.h"
#include "module/overworld/event_commands.h"
#include "module/runtime/runtime.h"
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
#include "map_grid_scroll.h"
#include "heap.h"
#include "menu_list.h"
#include "types.h"
#include "io_write_queue.h"
#include "io_reg.h"
#include "heap_slots.h"
#include "dma.h"
#include "resource_ids.h"
#include "sounds.h"
#include "sound_driver_symbols.h"
#include "blitter.h"
#include "fixed_math.h"
#include "task.h"
#include "particle.h"
#include "battle_effect_work.h"
#include "battle_display.h"
#include "ram_buffer.h"
#include "resource.h"
#include "battle_actor.h"
#include "iwram_call.h"
#include "transform.h"
#include "battle_work.h"
#include "project.h"
#include "event.h"
#include "keystate.h"
#include "battle_type.h"
#include "battle_system.h"
#include "battle_session.h"
#include "animspr.h"
#include "battle_unit.h"
#include "fixed_point_position.h"
#include "irq.h"
#include "module/battle_effect/battle_effect_bits.h"
#include "call.h"
#include "macros.h"
#include "sprite_ids.h"

// Display, palette and per-line work.
//
// Effects change the screen around their drawing in three ways:
//
// - Display state: the windows that frame the battle view (lines 16 to
//   135), the blend and display registers, and the set-up every effect
//   starts and ends with (BattleEffect_ConfigureEffectDisplay, below).
// - Per-frame tasks the runner adds: background scrolling, the
//   camera shake and the battle background's palette fades that make the canvas
//   stand out.
// - Per-scanline work: HBlank DMA that feeds a window edge, BG2's affine
//   reference or a palette row to the hardware line by line
//   (BattleEffect_ArmWin0HBlankDma, BattleEffect_ArmBg2AffineHBlankDma and
//   BattleEffect_ArmPaletteHBlankDma), and the tables those transfers read.
//
// The tasks read the current effect's work block, so the runner that
// adds them must remove them before that block is released.

/// The battle screen's display mode while an effect plays: background mode 1
/// with BG0 to BG2, sprites and both windows on.
#define EFFECT_DISPCNT                                                         \
  (DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG1_ON |     \
   DISPCNT_BG2_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON | DISPCNT_WIN1_ON)

/// Window 0, the battle view, shows BG0 to BG2, sprites and blending.
/// Effects pick window 1's layers themselves.
#define EFFECT_WIN0_LAYERS                                                     \
  (WININ_WIN0_BG0 | WININ_WIN0_BG1 | WININ_WIN0_BG2 | WININ_WIN0_OBJ |         \
   WININ_WIN0_CLR)

/// Outside the battle view: BG0 and blending only.
#define EFFECT_WINOUT (WINOUT_WIN01_BG0 | WINOUT_WIN01_CLR | WINOUT_WINOBJ_ALL)

/// A heap slot read through a pointer to the effect's own slot,
/// gHeapSlots[HEAP_SLOT_BATTLE_EFFECT].
#define EFFECT_SLOT(slots, slot) ((slots)[(slot) - HEAP_SLOT_BATTLE_EFFECT])
/// A heap slot read through a pointer to the canvas slot,
/// gHeapSlots[HEAP_SLOT_BATTLE_CANVAS], just after the effect's own.
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

/// The work block and canvas occupy adjacent slots, starting at
/// HEAP_SLOT_BATTLE_EFFECT. A cursor reads them in that order.
#define EFFECT_GET_WORK_AND_CANVAS(cursor, work, canvas)                       \
  ((work) = *(cursor)++, (canvas) = *(cursor))

/// Lets EFFECT_SLOT(slots, s) reach any slot: an earlier one such as
/// HEAP_SLOT_CAMERA, before HEAP_SLOT_BATTLE_EFFECT, sits at a negative index
/// that is still inside gHeapSlots.
#define EFFECT_GET_SLOTS(slots, cursor, work, canvas)                          \
  ((slots) = &gHeapSlots[HEAP_SLOT_BATTLE_EFFECT], (cursor) = (slots),         \
   EFFECT_GET_WORK_AND_CANVAS(cursor, work, canvas))

/// Marks particles[0] to particles[count - 1] free: a particle whose variant
/// is -1 is not drawn or moved until an effect gives it a life.
#define EFFECT_FREE_PARTICLES(i, particles, count)                             \
  for ((i) = 0; (i) != (count); (i)++)                                         \
  (particles)[i].variant = -1

/// Starts a frame's 3D projection from the battle camera: resets the
/// transform, then looks from camera->eye towards camera->focus. Effects
/// that place pictures at battle positions do this before projecting them.
#define EFFECT_VIEW_FROM_CAMERA(camera)                                        \
  (Transform_Reset(), Transform_SetLookAt((camera)->eye, (camera)->focus))

/// Strikes target `i`: it flashes white in its damage pose for `frames`
/// frames, counted by actor timer `i`, and is knocked back by `motion`, an enum
/// BattleKnockback (BattleMotion_ApplyKnockbackFar).
#define EFFECT_STRIKE_TARGET(effect, i, frames, motion)                        \
  (BattleEffect_TintBattler((effect)->targets[i], SPRITE_COLOR_WHITE,          \
                            POSE_DAMAGE, i, frames),                           \
   BattleMotion_ApplyKnockbackFar((effect)->targets[i], motion))

// The canvas life cycle, shared by every canvas effect. The statement macros
// are comma expressions, not do { } while (0) blocks: agscc compiles a
// do-while (0) in an effect's frame loop differently at some sites, and the
// comma form matches at all of them.

/// Lets the battle show the damage: resumes its paused playback, which plays
/// hitSound (an SE_ id; 0 keeps the one already queued) as the targets react.
/// It acts only while the playback is idle (BattlePlayback_Start),
/// so a second use while the damage is showing changes nothing.
#define EFFECT_SHOW_DAMAGE(hitSound) BattlePlayback_StartFar(hitSound)

/// The canvas: 128x128 pixels, one byte each, in its heap slot and in VRAM at
/// BG_CHAR_BLOCK(1).
#define CANVAS_BYTES (128 * 128)

/// Opens the canvas layer at the canvas's own width, one texel per pixel,
/// where BattleEffect_StartCanvasLayer stretches it to twice the width.
#define EFFECT_START_NARROW_CANVAS(bgControl)                                  \
  (BattleEffect_StartCanvasLayer(bgControl), REG_BG2PA = Q_8_8(1))

/// Builds a blitter for the 128x128 canvas (1 << 7 pixels each way) in heap
/// slot `slot`: BattleEffect_BuildBlitter's drawing routine for one clip,
/// mirror and blend mode. Every canvas effect draws through one or two of
/// these.
#define EFFECT_LOAD_BLITTER(slot, flags, mode)                                 \
  BattleEffect_BuildBlitter(slot, 7, 7, flags, mode)

/// Sets how the canvas transfer treats what is left in the canvas after each
/// copy: `mode` with `value` (see enum CanvasTransferMode).
#define EFFECT_SET_TRANSFER(work, mode, value)                                 \
  ((work)->transferMode = (mode), (work)->transferValue = (value))

/// Sets the transfer, then starts copying the canvas to VRAM once a frame.
#define EFFECT_START_TRANSFER(work, mode, value)                               \
  (EFFECT_SET_TRANSFER(work, mode, value),                                     \
   Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK))

/// Presents the canvas: marks it ready for the transfer task, then waits for
/// the next frame. Nearly every effect loop ends so.
#define EFFECT_PRESENT_CANVAS(work) ((work)->transferPending = 1, wait(1))

/// The usual end of an effect's frame: counts down the targets' flash timers
/// (BattleEffect_CountDownBattlerTints), then presents the canvas.
#define EFFECT_END_FRAME(work)                                                 \
  (BattleEffect_CountDownBattlerTints(), EFFECT_PRESENT_CANVAS(work))

/// Frees the two blitters, the alternate first.
#define EFFECT_RELEASE_BLITTERS()                                              \
  (HeapSlot_Release(HEAP_SLOT_RAM_CODE_2), HeapSlot_Release(HEAP_SLOT_RAM_CODE))

/// The usual end of a canvas effect: stops the transfer task before
/// freeing what it reads, then the blitters, then the canvas layer.
#define EFFECT_FINISH_CANVAS()                                                 \
  (Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer),                   \
   EFFECT_RELEASE_BLITTERS(), BattleEffect_EndCanvasLayer())

/// The same for an effect that drew through one blitter.
#define EFFECT_FINISH_CANVAS_ONE_BLITTER()                                     \
  (Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer),                   \
   HeapSlot_Release(HEAP_SLOT_RAM_CODE), BattleEffect_EndCanvasLayer())

/// The bytes of one set of the ten mote cells: 2 * (1 + 4 + ... + 100).
#define MOTE_SET_BYTES 770

/// The bytes of the six glint cells that open the tornado sheet, the sum of
/// their pictures in gBattleEffect_GlintCells' order (its widths by its
/// heights). The tornado strips, and the bolts or bursts some effects decode
/// over them, follow.
#define GLINT_SET_BYTES                                                        \
  (19 * 31 + 23 * 31 + 23 * 31 + 19 * 21 + 16 * 21 + 17 * 24)

/// Particle sheets hold ten mote cells, 1x2 to 10x20 pixels. Draws mote cell
/// `size` (size pixels wide, twice that tall) of `sheet` through blitter
/// `draw`, centred across x and standing on y: the motes, sparks and dust of
/// 29 parts, from the mote fountain to the summons. `size` is 1 to 10.
#define EFFECT_DRAW_MOTE(draw, canvas, sheet, x, y, size)                      \
  (draw)(canvas, (sheet) + gBattleEffect_MoteCells[(size) - 1],                \
         (x) - (size) / 2, (y) - (size), size, (size) * 2)

/// Draws flare cell `size` of `sheet`, a square `size` pixels from its centre
/// to each edge, centred on (x, y): Douse's sparks and the glowing points of
/// the element pillar and burst, stat boosts, beams and rings.
#define EFFECT_DRAW_FLARE(draw, canvas, sheet, x, y, size)                     \
  (draw)(canvas, (sheet) + gBattleEffect_FlareCells[(size) - 1], (x) - (size), \
         (y) - (size), (size) * 2, (size) * 2)

/// Shows the canvas: the effect display mode, BG2 stretched to twice its
/// width (the 128-pixel canvas shown 256 pixels wide) and blended over
/// everything below: `top` sixteenths of the canvas's colour are added to all
/// of what lies under it, so the canvas brightens what it covers.
#define EFFECT_SHOW_WIDE_CANVAS(top)                                           \
  (REG_DISPCNT = EFFECT_DISPCNT, REG_BG2PA = Q_8_8(0.5),                       \
   REG_BLDALPHA = BLEND_ALPHA(top, 16),                                        \
   REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL)

/// A WIN0H value whose left edge is past its right edge. The hardware then
/// takes the right edge as 240, so the span is not empty in itself; the
/// effects write it only on lines outside window 0's vertical range.
#define WINDOW_SPAN_INVERTED WIN_RANGE(1, 0)

/// The battle background's picture as BG1 shows it from BG character block 2:
/// 256x120 pixels, one byte each.
#define BATTLE_BACKGROUND_BYTES (256 * 120)

/// Ram_MapGridCellBuffer as the unused BattleEffect_ConvertBackgroundToBlueRamp
/// (bind.c) uses it: a working copy of the battle background's picture, the
/// BATTLE_BACKGROUND_BYTES of BG character block 2.
#define BLUE_RAMP_PIXELS Ram_MapGridCellBuffer

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
  (REG_BG2X = 0, REG_BG2Y = -(16 << 8), REG_BG2PA = Q_8_8(0.5), REG_BG2PB = 0, \
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

/// What BattleEffect_RunDjinniEntrance takes added to its element; the element
/// alone plays the 64-frame entrance.
enum DjinniEntranceLength {
  /// The long entrance: 84 frames instead of 64, ending in a burst of motes.
  DJINNI_ENTRANCE_LONG = ELEMENT_COUNT
};

/// Where BattleEffect_RunDjinniEntrance's Djinni appears.
enum DjinniEntranceAnchor {
  DJINNI_ANCHOR_OVER_TARGET, ///< over the first target
  DJINNI_ANCHOR_TARGET_X,    ///< at the first target's x, at depth 0
  DJINNI_ANCHOR_OVER_USER,   ///< over the user
  DJINNI_ANCHOR_USER_X,      ///< at the user's x, at depth 0; no caller
  DJINNI_ANCHOR_CENTRE       ///< in the centre of the field
};

/// Where BattleEffect_RunSparkGroups' bursts go off.
enum SparkGroupsMode {
  SPARK_GROUPS_NOVA,     ///< at (60, 48); the damage shows on frame 24
  SPARK_GROUPS_DYNAMITE, ///< at (60, 64), orange; the damage shows on frame 2
  SPARK_GROUPS_RISING    ///< below the user, in the LightningBolt colours, the
                         ///< sparks rising; the damage shows on frame 2
};

/// Which way BattleEffect_WipeCanvas sweeps.
enum CanvasWipeDirection {
  CANVAS_WIPE_DOWN,  ///< top to bottom
  CANVAS_WIPE_ACROSS ///< left to right
};
/// What BattleEffect_WipeCanvas leaves behind it.
enum CanvasWipeFill {
  CANVAS_WIPE_FILL, ///< colour 1, which its callers make black first
  CANVAS_WIPE_CLEAR ///< colour 0: the canvas cleared
};

/// The summons' sprite formations (BattleEffect_PlaceFormationObjects and
/// BattleEffect_RunImpactBurst), each named for the summon that keeps it.
enum SummonFormation {
  /// ★ Named from the game: "Thor" (MsgDjinnName)
  SUMMON_FORMATION_THOR, ///< nine in a 3x3 grid at 32-pixel spacing
  /// ★ Named from the game: "Nereid" (MsgDjinnName)
  SUMMON_FORMATION_NEREID, ///< twelve in rows of 1, 3, 4 and 4, from 16 left
                           ///< and 32 up
  /// ★ Named from the game: "Cybele" (MsgDjinnName)
  SUMMON_FORMATION_CYBELE, ///< eight in rows of 3, 2 and 3, from 16 right
  /// ★ Named from the game: "Tiamat" (MsgDjinnName)
  SUMMON_FORMATION_TIAMAT ///< eight drawn at 1.25 times their size
};

/// The axis a swirling spark, mote or star turns about, which its index & 3
/// picks.
enum SpinAxis { SPIN_YAW, SPIN_PITCH, SPIN_ROLL, SPIN_PITCH_AND_ROLL };

/// work->cameraPhase, which BattleCamera_ApplyPhasedDelta and the effects
/// that turn the camera through it (Fire, Froth) pass between them.
enum CameraPhase {
  CAMERA_PHASE_IDLE,       ///< nothing marked since the last turn
  CAMERA_PHASE_FRAME_DONE, ///< the effect has finished a frame: turn the
                           ///< whole delta
  CAMERA_PHASE_HALF_TURNED ///< half a delta turned on an unmarked frame
};

/// Where each range of BattleEffect_RunByIdRange's effect kinds starts:
/// the casting impacts (the kind less EFFECT_RANGE_CASTING_IMPACT is an enum
/// CastingImpact), the beams, which also take every kind below the
/// casting impacts, and the ring and spark effect, which plays critical
/// hits.
enum BattleEffectIdRange {
  EFFECT_RANGE_CASTING_IMPACT = 100,
  EFFECT_RANGE_BEAM = 136,
  EFFECT_RANGE_RING_AND_SPARK = EFFECT_ID_CRITICAL_OFFSET
};

#endif // ALCHEMY_BATTLE_EFFECT_INTERNAL_H
