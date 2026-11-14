#ifndef ALCHEMY_BATTLE_EFFECT_INTERNAL_H
#define ALCHEMY_BATTLE_EFFECT_INTERNAL_H

/// @file
/// What every part of the battle effect object shares: its includes, macros and
/// frame-loop helpers.

#include "debug.h"
#include "edition.h"
#include "fixed_math.h"
#include "macros.h"
#include "resource_ids.h"
#include "sounds.h"
#include "sprite_ids.h"
#include "types.h"
#include "battle/battle.h"
#include "battle/battle_action.h"
#include "battle/battle_action_run.h"
#include "battle/battle_actor.h"
#include "battle/battle_camera.h"
#include "battle/battle_effect_work.h"
#include "battle/battle_side.h"
#include "battle/battle_unit.h"
#include "battle/battle_work.h"
#include "battle/blitter.h"
#include "battle/canvas.h"
#include "battle/playback_queue.h"
#include "battle_effect/battle_effect.h"
#include "battle_effect/battle_effect_table.h"
#include "battle_effect/blitter_operands.h"
#include "field/event.h"
#include "field/event_commands.h"
#include "field/field.h"
#include "field/field_sequence.h"
#include "field/map_grid_scroll.h"
#include "field/particle.h"
#include "field/sprite.h"
#include "game/game.h"
#include "gamepak/far_call.h"
#include "graphics/battle_sprite_id.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "sound/sound_driver_symbols.h"
#include "system/dma.h"
#include "system/fixed_point_position.h"
#include "system/heap.h"
#include "system/heap_release.h"
#include "system/heap_slots.h"
#include "system/input.h"
#include "system/io_reg.h"
#include "system/io_write_queue.h"
#include "system/irq.h"
#include "system/iwram_call.h"
#include "system/projection.h"
#include "system/ram_buffer.h"
#include "system/system.h"
#include "system/task.h"
#include "system/transform.h"
#include "window/menu_list.h"

// Display, palette and per-line work (README.md, "Around the canvas").
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

/// Index of DMAxCNT_H, the control half, in a channel's 16-bit registers.
#define DMA_CONTROL_HALF 5

/// DMA control word that fills n words from one fixed source word.
#define DMA_FILL_WORDS(n) ((DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | (n))

/// Stops a DMA channel and reads its control back, so the stop takes effect
/// before the channel is set up again.
static inline void Dma_Stop(volatile u16 *channel) {
  channel[DMA_CONTROL_HALF] &= (u16)~DMA_START_TIMING_DRQ_REPEAT;
  channel[DMA_CONTROL_HALF] &= (u16)~DMA_ENABLE;
  (void)channel[DMA_CONTROL_HALF];
}

/// Marks particles[0] to particles[count - 1] free: a particle whose variant
/// is -1 is not drawn or moved until an effect gives it a life.
#define EFFECT_FREE_PARTICLES(i, particles, count)                             \
  for ((i) = 0; (i) != (count); (i)++)                                         \
  (particles)[i].variant = -1

/// Starts a frame's 3D projection from the battle camera.
#define EFFECT_VIEW_FROM_CAMERA(camera)                                        \
  (Transform_Reset(), Transform_SetLookAt((camera)->eye, (camera)->focus))

/// Strikes target `i`: it flashes white in its damage pose and is knocked back
/// by `motion`.
#define EFFECT_STRIKE_TARGET(effect, i, frames, motion)                        \
  (BattleEffect_TintBattler((effect)->targets[i], SPRITE_COLOR_WHITE,          \
                            POSE_DAMAGE, i, frames),                           \
   BattleMotion_ApplyKnockbackFar((effect)->targets[i], motion))

// The canvas life cycle, shared by every canvas effect.

/// Lets the battle show the damage, playing hitSound (0 keeps the one queued)
/// as the targets react; a second use changes nothing.
#define EFFECT_SHOW_DAMAGE(hitSound) BattlePlayback_StartFar(hitSound)

/// The canvas: 128x128 pixels, one byte each, in its heap slot and in VRAM at
/// BG_CHAR_BLOCK(1).
#define CANVAS_BYTES (128 * 128)

/// Opens the canvas layer at the canvas's own width.
#define EFFECT_START_NARROW_CANVAS(bgControl)                                  \
  (BattleEffect_StartCanvasLayer(bgControl), REG_BG2PA = Q_8_8(1))

/// Builds a blitter for the canvas in heap slot `slot`, for one clip, mirror
/// and blend mode.
#define EFFECT_LOAD_BLITTER(slot, flags, mode)                                 \
  BattleEffect_BuildBlitter(slot, 7, 7, flags, mode)

/// The blitter built in heap slot `slot`.
#define EFFECT_BLITTER(slot) ((DrawRectangle)gHeapSlots[slot])

/// Sets what the canvas transfer leaves in the canvas after each copy.
#define EFFECT_SET_TRANSFER(work, mode, value)                                 \
  ((work)->transferMode = (mode), (work)->transferValue = (value))

/// Sets the transfer, then starts copying the canvas to VRAM once a frame.
#define EFFECT_START_TRANSFER(work, mode, value)                               \
  (EFFECT_SET_TRANSFER(work, mode, value),                                     \
   Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK))

/// Presents the canvas and waits for the next frame.
#define EFFECT_PRESENT_CANVAS(work) ((work)->transferPending = 1, wait(1))

/// The usual end of an effect's frame: counts down the targets' flash timers,
/// then presents the canvas.
#define EFFECT_END_FRAME(work)                                                 \
  (BattleEffect_CountDownBattlerTints(), EFFECT_PRESENT_CANVAS(work))

/// Frees the two blitters, the alternate first.
#define EFFECT_RELEASE_BLITTERS()                                              \
  (HeapSlot_Release(HEAP_SLOT_RAM_CODE_2), HeapSlot_Release(HEAP_SLOT_RAM_CODE))

/// The usual end of a canvas effect: the transfer, the blitters, then the
/// canvas layer.
#define EFFECT_FINISH_CANVAS()                                                 \
  (Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer),                   \
   EFFECT_RELEASE_BLITTERS(), BattleEffect_EndCanvasLayer())

/// The same for an effect that drew through one blitter.
#define EFFECT_FINISH_CANVAS_ONE_BLITTER()                                     \
  (Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer),                   \
   HeapSlot_Release(HEAP_SLOT_RAM_CODE), BattleEffect_EndCanvasLayer())

/// The bytes of one set of the ten mote pictures: 2 * (1 + 4 + ... + 100).
#define MOTE_SET_BYTES 770

/// The bytes of the six glint pictures that open the tornado sheet.
#define GLINT_SET_BYTES                                                        \
  (19 * 31 + 23 * 31 + 23 * 31 + 19 * 21 + 16 * 21 + 17 * 24)

/// Draws mote picture `size` (1 to 10: size pixels wide, twice that tall),
/// centred across x and standing on y.
#define EFFECT_DRAW_MOTE(draw, canvas, sheet, x, y, size)                      \
  (draw)(canvas, (sheet) + gBattleEffect_MotePictures[(size) - 1],             \
         (x) - (size) / 2, (y) - (size), size, (size) * 2)

/// Draws flare picture `size`, a square, centred on (x, y).
#define EFFECT_DRAW_FLARE(draw, canvas, sheet, x, y, size)                     \
  (draw)(canvas, (sheet) + gBattleEffect_FlarePictures[(size) - 1],            \
         (x) - (size), (y) - (size), (size) * 2, (size) * 2)

/// Swells a falling mote as the drops clear (Nereid's drops, Titan Blade's
/// dust).
#define EFFECT_SWELL_MOTE(size, frame)                                         \
  ((frame) > 68 && (size) <= 5 && ((size) = 6),                                \
   (frame) > 70 && (size) <= 6 && ((size) = 7),                                \
   (frame) > 72 && (size) <= 7 && ((size) = 8),                                \
   (frame) > 74 && (size) <= 8 && ((size) = 9), (frame) > 76 && ((size) = 10))

/// A falling mote's vertical speed: it falls under `gravity`, then from frame
/// 80 rises.
#define EFFECT_FALL_MOTE(velocityY, gravity, frame)                            \
  ((frame) > 80 ? ((velocityY) -= px(1) / 2) : ((velocityY) += (gravity)))

/// Shows the canvas stretched to twice its width, `top` sixteenths of its
/// colour added over what lies under it.
#define EFFECT_SHOW_WIDE_CANVAS(top)                                           \
  (REG_DISPCNT = EFFECT_DISPCNT, REG_BG2PA = Q_8_8(0.5),                       \
   REG_BLDALPHA = BLEND_ALPHA(top, 16),                                        \
   REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL)

/// A WIN0H value whose left edge is past its right edge, written only on lines
/// outside window 0.
#define WINDOW_SPAN_INVERTED WIN_RANGE(1, 0)

/// The battle background's picture as BG1 shows it from BG character block 2:
/// 256x120 pixels, one byte each.
#define BATTLE_BACKGROUND_BYTES (256 * 120)

/// Ram_MapGridCellBuffer as the unused blue ramp uses it: a working copy of the
/// battle background's picture.
#define BLUE_RAMP_PIXELS Ram_MapGridCellBuffer

/// Caps a colour level at cap, and at 0 from below.
#define EFFECT_CAP_LEVEL(level, cap)                                           \
  {                                                                            \
    if ((level) > (cap))                                                       \
      (level) = (cap);                                                         \
    if ((level) < 0)                                                           \
      (level) = 0;                                                             \
  }

/// The reaper in CounterRevealSheet, which Condemn and Mortal Danger decode:
/// the reaper with its scythe raised, then swung, then the red crescent the
/// swing leaves.
enum ReaperSheetPicture {
  /// 57x98
  REAPER_SHEET_RAISED = 0,
  /// 99x69
  REAPER_SHEET_SWUNG = REAPER_SHEET_RAISED + 57 * 98,
  /// 128x91
  REAPER_SHEET_CRESCENT = REAPER_SHEET_SWUNG + 99 * 69
};

/// RedCrescentSheetA: the swing's crescent fading in four pictures.
enum RedCrescentPicture {
  /// 128x91
  RED_CRESCENT_FADE_1 = 0,
  /// 128x59
  RED_CRESCENT_FADE_2 = RED_CRESCENT_FADE_1 + 128 * 91,
  /// 122x29
  RED_CRESCENT_FADE_3 = RED_CRESCENT_FADE_2 + 128 * 59,
  /// 76x25
  RED_CRESCENT_FADE_4 = RED_CRESCENT_FADE_3 + 122 * 29
};

/// Closing bolts and lightning columns use the same 24x120 pictures.
#define LIGHTNING_BOLT_PICTURE_BYTES 2880

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

/// Writes one row of BG2's map for the canvas, two tiles at a time.
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
  DJINNI_ANCHOR_OVER_USER,
  DJINNI_ANCHOR_USER_X, ///< at the user's x, at depth 0; no caller
  DJINNI_ANCHOR_CENTRE  ///< in the centre of the field
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
  SUMMON_FORMATION_THOR,   ///< nine in a 3x3 grid at 32-pixel spacing
  SUMMON_FORMATION_NEREID, ///< twelve in rows of 1, 3, 4 and 4, from 16 left
                           ///< and 32 up
  SUMMON_FORMATION_CYBELE, ///< eight in rows of 3, 2 and 3, from 16 right
  SUMMON_FORMATION_TIAMAT  ///< eight drawn at 1.25 times their size
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

/// Where each range of BattleEffect_RunByIdRange's effect kinds starts: the
/// casting impacts, the beams, and the ring and spark effect of critical hits.
enum BattleEffectIdRange {
  EFFECT_RANGE_CASTING_IMPACT = 100,
  EFFECT_RANGE_BEAM = 136,
  EFFECT_RANGE_RING_AND_SPARK = EFFECT_ID_CRITICAL_OFFSET
};

#endif // ALCHEMY_BATTLE_EFFECT_INTERNAL_H
