/// @file
/// Thrash and the spark approaches, and the picture that floods the canvas over
/// the target.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

/// Jumps the user toward the first target and emits type 4 sparks after 29
/// frames.
void BattleEffect_RunThrash(struct BattleEffectParams *args) {
  // Jump 3/4 of the distance at displacement / 24, with initial vertical
  // speed 12.2.
  BattleMotion_ApproachTargetFar(args->user, args->targets[0], 24, 0xc3333);
  wait(29);
  args->variant = 4;
  BattleEffect_RunSparkGroups(
      args, SPARK_GROUPS_RISING); // 2: from the user's position.
}

/// The user leaps at the first target, waits 26 frames, and the spark groups
/// burst below it as the strongest variant.
///
/// Abilities: Mighty Press.
void BattleEffect_RunMightyPress(struct BattleEffectParams *args) {
  BattleMotion_ApproachTargetFar(args->user, args->targets[0], 0x18, 0xc3333);
  wait(26);
  args->variant = 5;
  BattleEffect_RunSparkGroups(args, SPARK_GROUPS_RISING);
}

/// Jumps the user toward the first target and emits type 3 sparks after 12
/// frames.
void BattleEffect_RunSevereBlow(struct BattleEffectParams *args) {
  // Jump 3/4 of the distance at displacement / 24, with initial vertical
  // speed 7.2.
  BattleMotion_ApproachTargetFar(args->user, args->targets[0], 24, 0x73333);
  wait(12);
  args->variant = 3;
  BattleEffect_RunSparkGroups(
      args, SPARK_GROUPS_RISING); // 2: from the user's position.
}

/// Abilities: Quick Strike.
void BattleEffect_RunQuickStrike(struct BattleEffectParams *args) {
  BattleEffect_RunFlashingStrike(args, 1);
}

/// Abilities: Vorpal Slash.
void BattleEffect_RunVorpalSlash(struct BattleEffectParams *args) {
  BattleEffect_RunFlashingStrike(args, 2);
}

/// Abilities: Lunar Slash.
void BattleEffect_RunLunarSlash(struct BattleEffectParams *args) {
  BattleEffect_RunFlashingStrike(args, 3);
}

/// Abilities: Boost Hack.
void BattleEffect_RunBoostHack(struct BattleEffectParams *args) {
  BattleEffect_RunFlashingStrike(args, 4);
}

/// Abilities: Twin Shear.
void BattleEffect_RunTwinShear(struct BattleEffectParams *args) {
  BattleEffect_RunFlashingStrike(args, 5);
}

#undef RESULTS_FRAME

/// Each of the four pictures is PICTURE_SIZE pixels square; the reflected
/// variant's are half as tall.
#define PICTURE_SIZE 120
#define PICTURE_BYTES (PICTURE_SIZE * PICTURE_SIZE)
#define HALF_PICTURE_BYTES (PICTURE_SIZE * PICTURE_SIZE / 2)

#define REVEAL_FRAMES 21
#define FLOOD_FRAME 16 ///< the pictures end and the canvas floods
#define RESULTS_FRAME 18
#define HIT_FRAME 20

/// Plays a flashing strike; nothing passes FLASHING_STRIKE_UNUSED.
void BattleEffect_RunFlashingStrike(struct BattleEffectParams *args,
                                    s32 variant) {
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle draw[2];
  s32 frame;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  BattleEffect_StartCanvasLayer(0);
  if ((u32)(variant - 3) > 2)
    REG_BLDCNT = 0;
  REG_BLDALPHA = BLEND_ALPHA(16, 16);

  // The pictures: 1 and 2 in the sheet, 3 and 4 in the map cell
  // buffer (variant 5's four half pictures all there).
  if (variant == 0) {
    Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskE, work->sheet, 1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskF, Ram_MapCellBuffer,
                               1, 1);
  } else if (variant == 1) {
    Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskC, work->sheet, 1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskD, Ram_MapCellBuffer,
                               1, 1);
  } else if (variant == 4) {
    s32 i;

    Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskA, work->sheet, 1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskB, Ram_MapCellBuffer,
                               1, 1);
    // Turn the first 64 BG colours: red to blue, blue to green, green to red.
    for (i = 0; i != 64; i++) {
      s32 blue = (((u16 *)BG_PLTT)[i] >> 10) & 0x1f;
      s32 green = (((u16 *)BG_PLTT)[i] >> 5) & 0x1f;
      s32 red = ((u16 *)BG_PLTT)[i] & 0x1f;

      ((u16 *)BG_PLTT)[i] = (red << 10) | (blue << 5) | green;
    }
  } else if (variant == 3) {
    Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskC, work->sheet, 1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskD, Ram_MapCellBuffer,
                               1, 1);
  } else if (variant == 5) {
    Resource_LoadAndDecompress((s32)&ResourceId_ArcFragmentsSheet,
                               Ram_MapCellBuffer, 1, 1);
  } else {
    Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskA, work->sheet, 1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskB, Ram_MapCellBuffer,
                               1, 1);
  }
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);

  // The user closes on the target in 16 frames (variant 3 jumps in place,
  // variant 4 strikes a pose instead and waits 8).
  if (variant == 1 || variant == 5)
    BattleMotion_ApproachTargetFar(args->user, args->targets[0], 16, px(8));
  else if (variant == 3)
    // The target is a whole word: the user, passed twice, is loaded once and
    // copied, where an s16 parameter would load it again halved.
    BattleMotion_ApproachTargetFar(args->user, args->user, 16, px(10));
  else if (variant == 4) {
    struct MotionObject *user = BattleObject_GetFar(args->user)->object;

    Actor_SelectAnimationFar(user, 2);
    Actor_SetAnimationSpeedFar(user, 48);
  } else
    BattleMotion_ApproachTargetFar(args->user, args->targets[0], 16, 0);
  if (variant == 4)
    wait(8);
  else
    wait(16);

  // The blitters face the target: one for the picture, one flipped upside
  // down for its reflection.
  if (args->side == EFFECT_SIDE_PARTY)
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                        BLITTER_ADDITIVE);
  else
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                        BLITTER_ADDITIVE);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  if (args->side == EFFECT_SIDE_PARTY)
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP, BLITTER_ADDITIVE);
  else
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y,
                        BLITTER_ADDITIVE);
  draw[1] = (DrawRectangle)Ram_HeapSlots->ramCodePaired;

  Sound_PlayFar(SE_POWER_UP);
  for (frame = 0; frame != REVEAL_FRAMES; frame++) {
    if (variant == 5) {
      // Half the picture, then its reflection below it.
      if (frame <= 3) {
        draw[0](canvas, Ram_MapCellBuffer, 0, 0, PICTURE_SIZE,
                PICTURE_SIZE / 2);
        draw[1](canvas, Ram_MapCellBuffer, 0, PICTURE_SIZE / 2, PICTURE_SIZE,
                PICTURE_SIZE / 2);
      } else if (frame <= 7) {
        draw[0](canvas, Ram_MapCellBuffer + HALF_PICTURE_BYTES, 0, 0,
                PICTURE_SIZE, PICTURE_SIZE / 2);
        draw[1](canvas, Ram_MapCellBuffer + HALF_PICTURE_BYTES, 0,
                PICTURE_SIZE / 2, PICTURE_SIZE, PICTURE_SIZE / 2);
      } else if (frame <= 11) {
        draw[0](canvas, Ram_MapCellBuffer + HALF_PICTURE_BYTES * 2, 0, 0,
                PICTURE_SIZE, PICTURE_SIZE / 2);
        draw[1](canvas, Ram_MapCellBuffer + HALF_PICTURE_BYTES * 2, 0,
                PICTURE_SIZE / 2, PICTURE_SIZE, PICTURE_SIZE / 2);
      } else if (frame <= 15) {
        draw[0](canvas, Ram_MapCellBuffer + HALF_PICTURE_BYTES * 3, 0, 0,
                PICTURE_SIZE, PICTURE_SIZE / 2);
        draw[1](canvas, Ram_MapCellBuffer + HALF_PICTURE_BYTES * 3, 0,
                PICTURE_SIZE / 2, PICTURE_SIZE, PICTURE_SIZE / 2);
      }
    } else {
      if (frame <= 3)
        draw[0](canvas, work->sheet, 0, 0, PICTURE_SIZE, PICTURE_SIZE);
      else if (frame <= 7)
        draw[0](canvas, work->sheet + PICTURE_BYTES, 0, 0, PICTURE_SIZE,
                PICTURE_SIZE);
      else if (frame <= 11)
        draw[0](canvas, Ram_MapCellBuffer, 0, 0, PICTURE_SIZE, PICTURE_SIZE);
      else if (frame <= 15)
        draw[0](canvas, Ram_MapCellBuffer + PICTURE_BYTES, 0, 0, PICTURE_SIZE,
                PICTURE_SIZE);
    }

    // The flood: four frames of the canvas filled with colour 0x3f.
    if ((u32)(frame - FLOOD_FRAME) <= 3)
      Iwram_FillWordsEntry(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(63));
    if (frame == RESULTS_FRAME)
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
    if (frame == HIT_FRAME) {
      work->shakeFrames = 8;
      BattleMotion_ApplyKnockbackFar(args->targets[0], KNOCKBACK_HURL);
    }
    BattleCamera_Shake(16, 16);
    EFFECT_END_FRAME(work);
  }

  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  if (variant == 3) {
    // Clear the canvas and its tiles and hand the layer to the next scene.
    Iwram_ClearWords((void *)BG_CHAR_BLOCK(1), 0x3c00);
    Iwram_ClearWords(canvas, 0x3c00);
    Task_Remove(Palette_StepFadeTransfer);
    BattleEffect_RunWeaken(args);
  } else {
    if (variant == 4)
      Actor_SetAnimationSpeedFar(BattleObject_GetFar(args->user)->object, 16);
    BattleEffect_EndCanvasLayer();
  }
}
