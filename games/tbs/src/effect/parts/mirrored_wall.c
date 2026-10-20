/// @file
/// The walls of Berserk, Spasm and Recovery.
// Part of effect.c.

#include "../effect_internal.h"

// The canvas blitters, and the 48-frame wall effects.

/// Builds the two usual routines that draw onto the 128x128 canvas, both
/// clipped on both axes.
///
/// Called during setup by twenty effect parts, from the gathering
/// lights and the Djinn beams to the casting impacts and Ragnarok, usually
/// with effect->side as `flip` so pictures face the targets. Builds the
/// blitters in heap slots HEAP_SLOT_BLITTER and HEAP_SLOT_BLITTER_ALTERNATE,
/// which the caller frees (EFFECT_RELEASE_BLITTERS).
///
/// flip : nonzero draws mirrored.
/// out  : [0] keeps the higher colour index, [1] adds the colours (up to 63).
void BattleFx_FetchRectangleBlitters(s32 flip, BlitterPair out) {
  if (flip == 0) {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS, COMPOSITE_MAX);
    out[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE, EFFECT_CLIP_BOUNDS,
                        COMPOSITE_ADDITIVE);
    out[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
  } else {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_MAX);
    out[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                        COMPOSITE_ADDITIVE);
    out[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
  }
}

/// The wall rises wide and shakes the screen hard.
///
/// Abilities: Berserk (kind 353).
void BattleFx_RunBerserk(struct BattleEffectArgument *effect) {
  BattleFx_RunMirroredWall(effect, 1);
}
/// The wall rises with a long gentle shake and the recovery sound.
///
/// Abilities: Spasm (kind 348).
void BattleFx_RunSpasm(struct BattleEffectArgument *effect) {
  BattleFx_RunMirroredWall(effect, 0);
}
/// A wide wall of fire, without a shake.
///
/// Abilities: Recovery (kind 389).
void BattleFx_RunRecovery(struct BattleEffectArgument *effect) {
  BattleFx_RunMirroredWall(effect, 2);
}
/// Raises a symmetrical wall (the EarthWall images) at the first target's
/// feet for 48 frames; the target glows and the battle shows the result on
/// frame 24.
///
/// Called by BattleFx_RunBerserk, BattleFx_RunSpasm and
/// BattleFx_RunRecovery. Loads its colours over the first 64 of the
/// background palette, sets BG2PA and BG2X so the wall stands on the target,
/// and sets work->shakeFrames. Frees its blitters and closes the canvas
/// layer before it returns.
///
/// mode : 0 actual size, a gentle shake for 32 frames
///        1 1.25 times as wide, a hard shake for 8 frames
///        2 1.25 times as wide, no shake, in the Fireball colours
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

  // Setup. The EarthWall images, coloured with the Mars Djinn palette (the
  // Fireball palette in mode 2), each palette replacing the one before.
  palette = Resource_GetTableEntry((s32)&ResourceId_EarthWallSheet);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  palette += 128;
  Resource_DecodeType01(palette, work->sheet);
  palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  if (mode == 2) {
    palette = Resource_GetTableEntry((s32)&ResourceId_FireballSheet);
    Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  }

  // Scroll BG2 so the canvas's x 64 is on the target. Modes 1 and 2 show
  // the canvas 1.25 times as wide, so the target's x is scaled by 4/5.
  EffectPosition_ProjectBattler(work->effect->actors[0], &screenPosition);
  if (mode == 0) {
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
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS, COMPOSITE_MAX);
  slots = gWorkSlot;
  draw[0] = slots[HEAP_SLOT_BLITTER];
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_MAX);
  draw[1] = slots[HEAP_SLOT_BLITTER_ALTERNATE];
  // Trails keep half a frame.
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 50);

  // Mode 2 sets no shake but still applies Camera_ApplyShake's 16 below,
  // which does nothing while shakeFrames is 0.
  if (mode == 2) {
    work->shakeFrames = 0;
    AudioCommand_PlayFar(SE_212);
  } else if (mode == 1) {
    work->shakeFrames = 8;
    AudioCommand_PlayFar(SE_212);
  } else {
    work->shakeFrames = 32;
  }

  for (frame = 0; frame != 48; frame++) {
    // Frame loop. The target glows for 32 frames from frame 0: white in
    // mode 2, red otherwise.
    if (frame == 0) {
      if (mode == 2)
        ObjectGroup_UpdateMembers(work->effect->actors[0], SPRITE_COLOR_WHITE,
                                  -1, 0, 32);
      else
        ObjectGroup_UpdateMembers(work->effect->actors[0],
                                  SPRITE_COLOR_RED_TINT, -1, 0, 32);
    }
    if (frame == 24)
      EFFECT_SHOW_DAMAGE(0);
    if (frame == 8 && mode == 0)
      AudioCommand_PlayFar(SE_RECOVERY);

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
    if (mode == 0)
      Camera_ApplyShake(2, 2);
    else
      Camera_ApplyShake(16, 16);
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS();
}
