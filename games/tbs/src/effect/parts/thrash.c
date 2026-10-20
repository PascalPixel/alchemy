/// @file
/// Thrash, Quick Strike and Vorpal Slash.
/// Part of effect.c, the battle effect object, compiled only through it.

#include "../effect_internal.h"

// The effects here: two hops that start a spark group, a four-frame
// image revealed in front of the target, and two mirrored columns rising at
// the target's feet.

/// Hops the acting unit toward the first target; triggers type 4 after 29
/// frames.
///
/// Abilities: Thrash (kind 388).
void BattleFx_RunThrash(struct BattleEffectArgument *effect) {
  // Hop 3/4 of the distance at displacement / 24, initial vertical speed 12.2.
  BattleMotion_ApproachTargetFar(effect->actor, effect->actors[0], 24, 0xc3333);
  WaitFrames(29);
  effect->variant = 4;
  BattleFx_RunSparkGroups(effect, 2); // 2: from the acting unit's position
}

/// Hops the acting unit toward the first target; triggers type 3 after 12
/// frames.
void BattleFx_RunSparkApproachVariant3(struct BattleEffectArgument *effect) {
  // Hop 3/4 of the distance at displacement / 24, initial vertical speed 7.2.
  BattleMotion_ApproachTargetFar(effect->actor, effect->actors[0], 24, 0x73333);
  WaitFrames(12);
  effect->variant = 3;
  BattleFx_RunSparkGroups(effect, 2); // 2: from the acting unit's position
}

/// The user leaps at the first target, four pictures flash across the effect
/// layer, then it fills with one colour as the target is struck.
///
/// Abilities: Quick Strike (kind 52).
void BattleFx_RunQuickStrike(struct BattleEffectArgument *effect) {
  BattleFx_RunFlashingStrike(effect, 1);
}
/// Flashes the target using the first pair of reveal-mask resources.
///
/// Abilities: Vorpal Slash (kind 65).
void BattleFx_RunVorpalSlash(struct BattleEffectArgument *effect) {
  BattleFx_RunFlashingStrike(effect, 2);
}
/// Shared runner of Quick Strike (mode 1) and Vorpal Slash (mode 2); no kind
/// uses mode 0. The user steps up to the first target over 16 frames, with a
/// hop in mode 1. Then four 120x120 pictures, chosen by mode, show for four
/// frames each (mirrored on side 1), the canvas fills with colour 0x3f on
/// frames 16 to 19, the battle shows the damage on frame 18, and on frame 20
/// the target reels and the screen shakes. 21 frames.
void BattleFx_RunFlashingStrike(struct BattleEffectArgument *effect, s32 mode) {
  void **slots;
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle draw[2];
  s32 frame;

  slots = gBattleFxWork;
  EFFECT_GET_WORK_AND_CANVAS(slots, work, canvas);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);
  REG_BLDCNT = 0;

  // Each pair of 120x120 frames fills the image storage before the particles.
  // Frames 1-2 use the work buffer; frames 3-4 use Ram_MapCellBuffer.
  if (mode == 0) {
    Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskE, work->imageStorage,
                               1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskF, Ram_MapCellBuffer,
                               1, 1);
  } else if (mode == 1) {
    Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskC, work->imageStorage,
                               1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskD, Ram_MapCellBuffer,
                               1, 1);
  } else {
    Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskA, work->imageStorage,
                               1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskB, Ram_MapCellBuffer,
                               1, 1);
  }
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR,
                        0); // Clear to 0 each frame.

  if (mode == 1)
    BattleMotion_ApproachTargetFar(effect->actor, effect->actors[0], 16,
                                   0x80000);
  else
    BattleMotion_ApproachTargetFar(effect->actor, effect->actors[0], 16, 0);
  WaitFrames(16);

  if (work->effect->side == 1)
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_COPY);
  else
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS, COMPOSITE_COPY);
  draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
  AudioCommand_PlayFar(SE_212);
  for (frame = 0; frame != 21; frame++) {
    if (frame <= 3)
      draw[0](canvas, work->imageStorage, 0, 0, 120, 120);
    else if (frame <= 7)
      draw[0](canvas, work->imageStorage + 0x3840, 0, 0, 120, 120);
    else if (frame <= 11)
      draw[0](canvas, Ram_MapCellBuffer, 0, 0, 120, 120);
    else if (frame <= 15)
      draw[0](canvas, Ram_MapCellBuffer + 0x3840, 0, 0, 120, 120);
    if (frame >= 16 && frame <= 19)
      Iwram_FillWords(canvas, CANVAS_BYTES,
                      0x3f3f3f3f); // Fill the entire canvas with color 0x3f.
    if (frame == 18)
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
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
