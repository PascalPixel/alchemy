#ifndef ALCHEMY_BATTLE_EFFECT_INTERNAL_H
#define ALCHEMY_BATTLE_EFFECT_INTERNAL_H

/// @file
/// What every part of the battle effect object shares: its includes, macros and
/// frame-loop helpers.

#include "debug/debug.h"
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
#include "battle/battle_effect_state.h"
#include "battle/battle_side.h"
#include "battle/battle_unit.h"
#include "battle/battle_state.h"
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
#include "system/point.h"
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
#include "system/matrix.h"
#include "window/menu_list.h"

/// The canvas added to the battle under it: both blend weights whole.
#define EFFECT_BLEND_ADD BLEND_ALPHA(BLEND_WEIGHT_MAX, BLEND_WEIGHT_MAX)
/// The canvas at `level` sixteenths over the whole battle under it.
#define EFFECT_BLEND_CANVAS(level) BLEND_ALPHA(level, BLEND_WEIGHT_MAX)
/// The canvas's usual weight over the battle, in sixteenths: every canvas
/// opens with it.
#define CANVAS_BLEND_LEVEL 14

// Display, palette and per-line state (README.md, "Around the canvas").
// The tasks read the current effect's state block, so the runner that
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
  (Matrix_Initialize(), Matrix_SetLookAt((camera)->eye, (camera)->focus))

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
#define CANVAS_SIZE 128
#define CANVAS_BYTES (CANVAS_SIZE * CANVAS_SIZE)
/// The canvas's rows and columns of 8x8 tiles.
#define CANVAS_TILES (CANVAS_SIZE / 8)
/// The byte of canvas pixel (x, y): the canvas is laid out in 8x8 tiles, row
/// by row, each tile 64 bytes row by row.
#define CANVAS_PIXEL_OFFSET(x, y)                                              \
  ((((y) / 8 * CANVAS_TILES + (x) / 8) * 8 + ((y) & 7)) * 8 + ((x) & 7))
/// The byte of pixel (x, y) of the battle background: 8x8 tiles of 256
/// colours, 64 bytes each, 32 to a row, from BG_CHAR_BLOCK(2).
#define BACKGROUND_PIXEL_OFFSET(x, y)                                          \
  (((x) & 7) + (((x) / 8) << 6) + (((y) & 7) << 3) + (((y) / 8) << 11))

/// The battle view, under the top window and over the menus: screen lines 16
/// to 135.
#define BATTLE_VIEW_TOP 16
#define BATTLE_VIEW_HEIGHT 120
#define BATTLE_VIEW_BOTTOM (BATTLE_VIEW_TOP + BATTLE_VIEW_HEIGHT)

/// How far right the narrow canvas is set to show over the party: against the
/// screen's right edge.
#define NARROW_CANVAS_PARTY_X (DISPLAY_WIDTH - CANVAS_SIZE)

/// Where Atalanta and Boreas centre the projection while their formations
/// fly, lower than the battle's BATTLE_PROJECTION_CENTER_Y.
#define SUMMON_PROJECTION_CENTER_Y 240

/// The frames the battle background darkens behind an effect's canvas
/// (state->fadeFrames, Palette_StepFadeTransfer).
#define BACKGROUND_DARKEN_FRAMES 24
/// The level a canvas darkens the battle background to, a level a frame.
#define BACKGROUND_DARKENED_LEVEL BACKGROUND_DARKEN_FRAMES

/// A sprite sheet resource opens with its 64 colours, ahead of its compressed
/// pictures.
#define SHEET_PALETTE_COLORS 64
#define SHEET_PALETTE_BYTES (SHEET_PALETTE_COLORS * 2)

/// Particle_AdvanceWithGravity3D's and 2D's damping that keeps a particle's
/// whole speed.
#define PARTICLE_DAMPING_NONE DAMPING_SCALE

/// Opens the canvas layer at the canvas's own width.
#define EFFECT_START_NARROW_CANVAS(bgControl)                                  \
  (BattleEffect_StartCanvasLayer(bgControl), REG_BG2PA = Q_8_8(1))

/// Builds a blitter for the 128x128 canvas (its width and height given to
/// BattleEffect_BuildBlitter as powers of two) in heap slot `slot`, for one
/// clip, mirror and blend mode.
#define EFFECT_LOAD_BLITTER(slot, flags, mode)                                 \
  BattleEffect_BuildBlitter(slot, 7, 7, flags, mode)

/// The blitter built in heap slot `slot`.
#define EFFECT_BLITTER(slot) ((DrawRectangle)gHeapSlots[slot])

/// Sets what the canvas transfer leaves in the canvas after each copy.
#define EFFECT_SET_TRANSFER(state, mode, value)                                \
  ((state)->transferMode = (mode), (state)->transferValue = (value))

/// Sets the transfer, then starts copying the canvas to VRAM once a frame.
#define EFFECT_START_TRANSFER(state, mode, value)                              \
  (EFFECT_SET_TRANSFER(state, mode, value),                                    \
   Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK))

/// Presents the canvas and waits for the next frame.
#define EFFECT_PRESENT_CANVAS(state) ((state)->transferPending = 1, wait(1))

/// The usual end of an effect's frame: counts down the targets' flash timers,
/// then presents the canvas.
#define EFFECT_END_FRAME(state)                                                \
  (BattleEffect_CountDownBattlerTints(), EFFECT_PRESENT_CANVAS(state))

/// Frees the two blitters, the alternate first.
#define EFFECT_RELEASE_BLITTERS()                                              \
  (HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED),                                \
   HeapSlot_Release(HEAP_SLOT_RAM_CODE))

/// The usual end of a canvas effect: the transfer, the blitters, then the
/// canvas layer.
#define EFFECT_FINISH_CANVAS()                                                 \
  (Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer),                   \
   EFFECT_RELEASE_BLITTERS(), BattleEffect_EndCanvasLayer())

/// The same for an effect that drew through one blitter.
#define EFFECT_FINISH_CANVAS_ONE_BLITTER()                                     \
  (Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer),                   \
   HeapSlot_Release(HEAP_SLOT_RAM_CODE), BattleEffect_EndCanvasLayer())

/// The bytes of a sprite sheet picture, one byte a pixel, from its name's
/// _WIDTH and _HEIGHT.
#define PICTURE_BYTES(name) (name##_WIDTH * name##_HEIGHT)

/// Draws picture `name` of `sheet` at its own size, its top left at (x, y).
#define EFFECT_DRAW_PICTURE(draw, canvas, sheet, name, x, y)                   \
  (draw)(canvas, (sheet) + (name), x, y, name##_WIDTH, name##_HEIGHT)

/// The bytes of one set of the ten mote pictures: 2 * (1 + 4 + ... + 100).
#define MOTE_SET_BYTES 770
/// A picture's dimmed copies: DIMMED_COPIES of it, copy i with every level
/// capped at DIMMED_COPY_CAP(i), 7 levels lower than the copy before, copy 0
/// not at all. Thor's ring and Judgment's darken through copies of the mote
/// pictures, a stat reduction's shards through copies of the shard.
#define DIMMED_COPIES 8
#define DIMMED_COPY_CAP(copy) (CANVAS_LEVEL_BRIGHTEST + 1 - (copy) * 7)

/// The bytes of the six glint pictures that open the tornado sprite sheet.
#define GLINT_SET_BYTES                                                        \
  (19 * 31 + 23 * 31 + 23 * 31 + 19 * 21 + 16 * 21 + 17 * 24)

/// Draws mote picture `size` (1 to MOTE_SIZE_MAX: size pixels wide, twice that
/// tall), centred across x and standing on y.
#define EFFECT_DRAW_MOTE(draw, canvas, sheet, x, y, size)                      \
  (draw)(canvas, (sheet) + gBattleEffect_MotePictures[(size) - 1],             \
         (x) - (size) / 2, (y) - (size), size, (size) * 2)
/// The largest of the ten mote pictures.
#define MOTE_SIZE_MAX 10
/// A mote sized by its depth is drawn one size smaller for every
/// MOTE_DEPTH_STEP past MOTE_DEPTH_NEAR, its depth held between
/// MOTE_DEPTH_NEAR and MOTE_DEPTH_FAR.
#define MOTE_DEPTH_NEAR 160
#define MOTE_DEPTH_FAR 799
#define MOTE_DEPTH_STEP 64

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
  ((frame) > 80 ? ((velocityY) -= toFIXED(1) / 2) : ((velocityY) += (gravity)))

/// Shows the canvas stretched to twice its width, `top` sixteenths of its
/// colour added over what lies under it.
#define EFFECT_SHOW_WIDE_CANVAS(top)                                           \
  (REG_DISPCNT = EFFECT_DISPCNT, REG_BG2PA = Q_8_8(0.5),                       \
   REG_BLDALPHA = EFFECT_BLEND_CANVAS(top),                                    \
   REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL)

/// A WIN0H value whose left edge is past its right edge, written only on lines
/// outside window 0.
#define WINDOW_SPAN_INVERTED WIN_RANGE(1, 0)

/// The battle background's image as BG1 shows it from BG character block 2,
/// one byte a pixel.
#define BATTLE_BACKGROUND_WIDTH 256
#define BATTLE_BACKGROUND_HEIGHT 120
#define BATTLE_BACKGROUND_BYTES                                                \
  (BATTLE_BACKGROUND_WIDTH * BATTLE_BACKGROUND_HEIGHT)

/// Ram_MapGridCellBuffer as BattleEffect_BuildWindowEdgeTable reads it: window
/// 0's right edge, how far each of its lines is set in from that edge, and the
/// right edge it builds for every screen line.
#define WINDOW_EDGE_RIGHT ((u16 *)Ram_MapGridCellBuffer)
#define WINDOW_EDGE_INSETS (Ram_MapGridCellBuffer + 2)
#define WINDOW_EDGE_INSET_LINES 128
#define WINDOW_EDGE_LINES (WINDOW_EDGE_INSETS + WINDOW_EDGE_INSET_LINES)

/// Ram_MapGridCellBuffer as the unused blue ramp uses it: a working copy of the
/// battle background's image.
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
#define REAPER_SHEET_RAISED_WIDTH 57
#define REAPER_SHEET_RAISED_HEIGHT 98
#define REAPER_SHEET_SWUNG_WIDTH 99
#define REAPER_SHEET_SWUNG_HEIGHT 69
#define REAPER_SHEET_CRESCENT_WIDTH 128
#define REAPER_SHEET_CRESCENT_HEIGHT 91
/// Where each of the reaper's pictures begins in its sprite sheet, in bytes.
enum ReaperSheetPicture {
  REAPER_SHEET_RAISED = 0,
  REAPER_SHEET_SWUNG = REAPER_SHEET_RAISED + PICTURE_BYTES(REAPER_SHEET_RAISED),
  REAPER_SHEET_CRESCENT = REAPER_SHEET_SWUNG + PICTURE_BYTES(REAPER_SHEET_SWUNG)
};

/// RedCrescentSheetA: the swing's crescent fading in four pictures.
#define RED_CRESCENT_FADE_A_WIDTH 128
#define RED_CRESCENT_FADE_A_HEIGHT 91
#define RED_CRESCENT_FADE_B_WIDTH 128
#define RED_CRESCENT_FADE_B_HEIGHT 59
#define RED_CRESCENT_FADE_C_WIDTH 122
#define RED_CRESCENT_FADE_C_HEIGHT 29
#define RED_CRESCENT_FADE_D_WIDTH 76
#define RED_CRESCENT_FADE_D_HEIGHT 25
/// Where each picture of the fading red crescent begins in its sprite sheet, in
/// bytes.
enum RedCrescentPicture {
  RED_CRESCENT_FADE_A = 0,
  RED_CRESCENT_FADE_B = RED_CRESCENT_FADE_A +
      PICTURE_BYTES(RED_CRESCENT_FADE_A),
  RED_CRESCENT_FADE_C = RED_CRESCENT_FADE_B +
      PICTURE_BYTES(RED_CRESCENT_FADE_B),
  RED_CRESCENT_FADE_D = RED_CRESCENT_FADE_C + PICTURE_BYTES(RED_CRESCENT_FADE_C)
};

/// Closing bolts and lightning columns use the same 24x120 pictures.
#define LIGHTNING_BOLT_WIDTH 24
#define LIGHTNING_BOLT_HEIGHT 120
#define LIGHTNING_BOLT_PICTURE_BYTES PICTURE_BYTES(LIGHTNING_BOLT)

/// BlueFlameSheet: the column of flame, then the flame that splashes from it,
/// twice its width.
#define BLUE_FLAME_COLUMN_WIDTH 17
#define BLUE_FLAME_COLUMN_HEIGHT 104
#define BLUE_FLAME_SPLASH_WIDTH 34
#define BLUE_FLAME_SPLASH_HEIGHT 65
/// WaveSheet's six pictures, BlueArcSheetA and B's six, and YellowOrbSheet's.
#define WAVE_WIDTH 48
#define WAVE_HEIGHT 16
#define BLUE_ARC_WIDTH 48
#define BLUE_ARC_HEIGHT 72
#define YELLOW_ORB_WIDTH 40
#define YELLOW_ORB_HEIGHT 48
/// The yellow orb as BattleEffect_PackTileRows packs it, every second byte.
#define YELLOW_ORB_PACKED_WIDTH (YELLOW_ORB_WIDTH / 2)
#define YELLOW_ORB_PACKED_HEIGHT YELLOW_ORB_HEIGHT
#define YELLOW_ORB_PICTURES 6
/// The slash pictures as wide and tall as the canvas's view: StarBurstSheet's,
/// CrescentSheet's and the reveal masks.
#define SLASH_PICTURE_SIZE 120

/// FireStreakSheet's six pictures and FireBurstSheet's.
#define FIRE_STREAK_WIDTH 48
#define FIRE_STREAK_HEIGHT 72
#define FIRE_BURST_WIDTH 40
#define FIRE_BURST_HEIGHT 80
/// The small Djinn sprite sheets' pictures are SMALL_DJINNI_WIDTH wide;
/// Mercury's and Venus's are shorter than SMALL_DJINNI_HEIGHT.
#define MERCURY_SMALL_DJINNI_HEIGHT 34
#define VENUS_SMALL_DJINNI_HEIGHT 38
/// FireBlobSheet's blobs and CrystalSheet's crystal.
#define FIRE_BLOB_WIDTH 48
#define FIRE_BLOB_HEIGHT 32
#define CRYSTAL_WIDTH 32
#define CRYSTAL_HEIGHT 64

/// LightningPillarSheet's pillar, in two pictures.
#define LIGHTNING_PILLAR_WIDTH 48
#define LIGHTNING_PILLAR_HEIGHT 112
/// FirePillarSheetA: the pillar of fire, then two flames.
#define FIRE_PILLAR_WIDTH 34
#define FIRE_PILLAR_HEIGHT 104
#define FIRE_PILLAR_FLAME_WIDTH 20
#define FIRE_PILLAR_FLAME_HEIGHT 37
/// SpiralSheet's pictures.
#define SPIRAL_WIDTH 32
#define SPIRAL_HEIGHT 32

/// FlameballSheet and FireballSheet's pictures.
#define FLAMEBALL_WIDTH 40
#define FLAMEBALL_HEIGHT 64

/// Each half of RuneSheet's rune, the second drawn mirrored.
#define RUNE_HALF_WIDTH 24
#define RUNE_HALF_HEIGHT 48

/// A Djinni's picture: each frame of the full Djinn sprite sheets
/// (VenusDjinnSheet and its kind), and the one picture of the small sprite
/// sheets.
#define DJINNI_WIDTH 40
#define DJINNI_HEIGHT 40
#define SMALL_DJINNI_WIDTH 20
#define SMALL_DJINNI_HEIGHT 40

/// The pictures of the sprite sheets several effects share, each sprite sheet's
/// pictures all one size: SmokeSheet's puffs, FlashBurstSheet's bursts,
/// SkullSheet's skulls and the tornado strips after TornadoSheet's glints.
#define SMOKE_PUFF_WIDTH 24
#define SMOKE_PUFF_HEIGHT 48
#define FLASH_BURST_WIDTH 32
#define FLASH_BURST_HEIGHT 64
#define SKULL_WIDTH 24
#define SKULL_HEIGHT 24
#define TORNADO_STRIP_WIDTH 40
#define TORNADO_STRIP_HEIGHT 32
/// The pictures of DaggerSheet, ShieldSheet, GlowOrbSheet, VioletStreakSheet
/// and WispSheet.
#define DAGGER_WIDTH 24
#define DAGGER_HEIGHT 40
#define SHIELD_WIDTH 40
#define SHIELD_HEIGHT 40
#define GLOW_ORB_WIDTH 40
#define GLOW_ORB_HEIGHT 40
#define VIOLET_STREAK_WIDTH 32
#define VIOLET_STREAK_HEIGHT 64
#define WISP_WIDTH 64
#define WISP_HEIGHT 32
/// VortexSheet's six pictures and HeartSheet's heart.
#define VORTEX_WIDTH 24
#define VORTEX_HEIGHT 48
#define HEART_WIDTH 22
#define HEART_HEIGHT 42
/// WaterSpraySheet: the wave, then the splash's pictures.
#define WATER_WAVE_WIDTH 32
#define WATER_WAVE_HEIGHT 32
#define WATER_SPLASH_WIDTH 48
#define WATER_SPLASH_HEIGHT 48

/// Windows 0 and 1 both cover the battle view: the full width, lines 16 to
/// 135.
#define EFFECT_SET_BATTLE_WINDOWS()                                            \
  (REG_WIN0H = WIN_RANGE(0, DISPLAY_WIDTH),                                    \
   REG_WIN0V = WIN_RANGE(BATTLE_VIEW_TOP, BATTLE_VIEW_BOTTOM),                 \
   REG_WIN1H = WIN_RANGE(0, DISPLAY_WIDTH),                                    \
   REG_WIN1V = WIN_RANGE(BATTLE_VIEW_TOP, BATTLE_VIEW_BOTTOM))

/// BG2 shows the canvas from screen line 16 down (BG2Y is -16.0), stretched to
/// twice its width: the 128x128 canvas across the battle view.
#define EFFECT_SET_CANVAS_AFFINE()                                             \
  (REG_BG2X = 0, REG_BG2Y = -(BATTLE_VIEW_TOP << 8), REG_BG2PA = Q_8_8(0.5),   \
   REG_BG2PB = 0, REG_BG2PC = 0, REG_BG2PD = Q_8_8(1))

/// A line's sideways sway as its BG2X value in state->bg2xLines, which the
/// wave HBlank DMA writes line by line. BG2X counts 256ths of a canvas pixel,
/// so a sway of toFIXED(n) moves the line n / 4 canvas pixels: n / 2 screen
/// pixels where the canvas is shown at twice its width.
#define CANVAS_WAVE_BG2X(sway) ((sway) >> 10)

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

/// state->cameraPhase, which BattleCamera_ApplyPhasedDelta and the effects
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
