/// @file
/// The walls of Berserk, Spasm and Recovery.
// Part of battle_anim.c.

#include "../battle_anim_internal.h"

// The canvas blitters, and the 48-frame wall animations.

/// The mirrored wall's modes (BattleAnim_RunMirroredWall), each named for the
/// animation that plays it.
enum MirroredWallMode {
  MIRRORED_WALL_SPASM,   ///< actual size, a gentle shake for 32 frames
  MIRRORED_WALL_BERSERK, ///< 1.25 times as wide, a hard shake for 8 frames
  MIRRORED_WALL_RECOVERY ///< 1.25 times as wide, no shake, in the Fireball
                         ///< colours
};

/// Builds the two usual routines that draw onto the 128x128 canvas, both
/// clipped on both axes.
///
/// Called during setup by twenty animation parts, from the gathering
/// lights and the Djinn beams to the casting impacts and Ragnarok, usually
/// with effect->side as `flip` so pictures face the targets. Builds the
/// blitters in heap slots HEAP_SLOT_RAM_CODE and HEAP_SLOT_RAM_CODE_2,
/// which the caller frees (ANIM_RELEASE_BLITTERS).
///
/// flip : nonzero draws mirrored.
/// out  : [0] keeps the higher colour index, [1] adds the colours (up to 63).
void BattleAnim_FetchRectangleBlitters(s32 flip, BlitterPair out) {
  if (flip == 0) {
    ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, ANIM_CLIP_BOUNDS, COMPOSITE_LIGHTEN);
    out[0] = (DrawRectangle)gHeapSlots[HEAP_SLOT_RAM_CODE];
    ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, ANIM_CLIP_BOUNDS,
                      COMPOSITE_ADDITIVE);
    out[1] = (DrawRectangle)gHeapSlots[HEAP_SLOT_RAM_CODE_2];
  } else {
    ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, ANIM_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                      COMPOSITE_LIGHTEN);
    out[0] = (DrawRectangle)gHeapSlots[HEAP_SLOT_RAM_CODE];
    ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, ANIM_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                      COMPOSITE_ADDITIVE);
    out[1] = (DrawRectangle)gHeapSlots[HEAP_SLOT_RAM_CODE_2];
  }
}

/// The wall rises wide and shakes the screen hard.
///
/// Abilities: Berserk (BATTLE_ANIM_BERSERK).
void BattleAnim_RunBerserk(struct BattleAnimArgs *effect) {
  BattleAnim_RunMirroredWall(effect, MIRRORED_WALL_BERSERK);
}
/// The wall rises with a long gentle shake and the recovery sound.
///
/// Abilities: Spasm (BATTLE_ANIM_SPASM).
void BattleAnim_RunSpasm(struct BattleAnimArgs *effect) {
  BattleAnim_RunMirroredWall(effect, MIRRORED_WALL_SPASM);
}
/// A wide wall of fire rises without shaking the screen.
///
/// Abilities: Recovery (BATTLE_ANIM_RECOVERY).
void BattleAnim_RunRecovery(struct BattleAnimArgs *effect) {
  BattleAnim_RunMirroredWall(effect, MIRRORED_WALL_RECOVERY);
}
/// Raises a symmetrical wall (the EarthWall images) at the first target's
/// feet for 48 frames; the target glows and the battle shows the result on
/// frame 24.
///
/// Called by BattleAnim_RunBerserk, BattleAnim_RunSpasm and
/// BattleAnim_RunRecovery. Loads its colours over the first 64 of the
/// background palette, sets BG2PA and BG2X so the wall stands on the target,
/// and sets work->shakeFrames. Frees its blitters and closes the canvas
/// layer before it returns.
///
/// mode: the wall's width, shake and colours (enum MirroredWallMode).
void BattleAnim_RunMirroredWall(struct BattleAnimArgs *effect, s32 mode) {
  void **cursor;
  struct BattleAnimWork *work;
  void *canvas;
  u8 *palette;
  s32 x;
  struct ScreenPoint screenPosition;
  BlitterPair draw;
  void **slots;
  s32 frame;
  s32 cell;

  cursor = gBattleAnimWork;
  ANIM_GET_WORK_AND_CANVAS(cursor, work, canvas);
  work->effect = effect;
  BattleAnim_StartCanvasLayer(0);

  // Setup. The EarthWall images, coloured with the Mars Djinn palette (the
  // Fireball palette for Recovery), each palette replacing the one before.
  palette = Resource_GetTableEntry((s32)&ResourceId_EarthWallSheet);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  palette += 128;
  Decompress_TaggedLz(palette, work->sheet);
  palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  if (mode == MIRRORED_WALL_RECOVERY) {
    palette = Resource_GetTableEntry((s32)&ResourceId_FireballSheet);
    Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  }

  // Scroll BG2 so the canvas's x 64 is on the target. Berserk and Recovery
  // show the canvas 1.25 times as wide, so the target's x is scaled by 4/5.
  Projection_Unit(work->effect->targets[0], &screenPosition);
  if (mode == MIRRORED_WALL_SPASM) {
    REG_BG2PA = Q_8_8(1);
    x = 64 - screenPosition.x;
    REG_BG2X = x << 8;
  } else {
    REG_BG2PA = Q_8_8(0.8);
    x = (-screenPosition.x * 4) / 5 + 64;
    REG_BG2X = x << 8;
  }

  // The left half as drawn, the right half mirrored, keeping the higher
  // colour.
  ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, ANIM_CLIP_BOUNDS, COMPOSITE_LIGHTEN);
  slots = gHeapSlots;
  draw[0] = slots[HEAP_SLOT_RAM_CODE];
  ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, ANIM_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                    COMPOSITE_LIGHTEN);
  draw[1] = slots[HEAP_SLOT_RAM_CODE_2];
  // Trails keep half a frame.
  ANIM_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 50);

  // Recovery sets no shake but still applies BattleCamera_Shake's 16 below,
  // which does nothing while shakeFrames is 0.
  if (mode == MIRRORED_WALL_RECOVERY) {
    work->shakeFrames = 0;
    Sound_PlaySongFar(SE_212);
  } else if (mode == MIRRORED_WALL_BERSERK) {
    work->shakeFrames = 8;
    Sound_PlaySongFar(SE_212);
  } else {
    work->shakeFrames = 32;
  }

  for (frame = 0; frame != 48; frame++) {
    // Frame loop. The target glows for 32 frames from frame 0: white for
    // Recovery, red otherwise.
    if (frame == 0) {
      if (mode == MIRRORED_WALL_RECOVERY)
        BattleAnim_TintBattler(work->effect->targets[0], SPRITE_COLOR_WHITE, -1,
                               0, 32);
      else
        BattleAnim_TintBattler(work->effect->targets[0], SPRITE_COLOR_RED_TINT,
                               -1, 0, 32);
    }
    if (frame == 24)
      ANIM_SHOW_DAMAGE(0);
    if (frame == 8 && mode == MIRRORED_WALL_SPASM)
      Sound_PlaySongFar(SE_RECOVERY);

    // Images 0, 1 and 2, four frames each, then 2, 1, 2 and 1, up to frame
    // 27, standing 8 pixels below the target's feet; the trail fades after
    // that. The left half is drawn as is, the right mirrored, meeting at the
    // canvas's x 64.
    if (frame <= 31) {
      cell = frame / 4;
      if (cell > 2)
        cell = (cell & 1) + 1;
      if (frame <= 27) {
        draw[0](canvas, work->sheet + gMirroredWall_Cells[cell],
                64 - gMirroredWall_CellWidths[cell],
                (screenPosition.y - gMirroredWall_CellHeights[cell]) + 8,
                gMirroredWall_CellWidths[cell],
                gMirroredWall_CellHeights[cell]);
        draw[1](canvas, work->sheet + gMirroredWall_Cells[cell], 64,
                (screenPosition.y - gMirroredWall_CellHeights[cell]) + 8,
                gMirroredWall_CellWidths[cell],
                gMirroredWall_CellHeights[cell]);
      }
    }
    if (mode == MIRRORED_WALL_SPASM)
      BattleCamera_Shake(2, 2);
    else
      BattleCamera_Shake(16, 16);
    ANIM_END_FRAME(work);
  }

  ANIM_FINISH_CANVAS();
}
