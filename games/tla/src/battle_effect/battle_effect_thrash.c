/// @file
/// Thrash and the spark approaches, and the picture that floods the canvas over
/// the target.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

/// The flashing strike's modes (BattleEffect_RunFlashingStrike), each named for
/// the ability that plays it: which pictures it shows and how the user moves.
enum FlashingStrikeMode {
  FLASHING_STRIKE_UNUSED,       ///< RevealMaskE and F; nothing passes it
  FLASHING_STRIKE_QUICK_STRIKE, ///< RevealMaskC and D, with a hop
  FLASHING_STRIKE_VORPAL_SLASH, ///< RevealMaskA and B
  FLASHING_STRIKE_LUNAR_SLASH,  ///< RevealMaskC and D, then Weaken
  FLASHING_STRIKE_BOOST_HACK,   ///< RevealMaskA and B in other colours, a pose
  FLASHING_STRIKE_TWIN_SHEAR    ///< the arc fragments, each with its reflection
};

/// The user leaps high at the first target, then sparks burst beneath it.
///
/// Abilities: Thrash.
void BattleEffect_RunThrash(struct BattleEffectParams *args) {
  BattleMotion_ApproachTargetFar(args->user, args->targets[0], 24,
                                 toFIXED(61) / 5);
  wait(29);
  args->variant = SPARK_GROUPS_THRASH;
  BattleEffect_RunSparkGroups(args, SPARK_GROUPS_RISING);
}

/// The user leaps at the first target, waits 26 frames, and the spark groups
/// burst below it as the strongest variant.
///
/// Abilities: Mighty Press.
void BattleEffect_RunMightyPress(struct BattleEffectParams *args) {
  BattleMotion_ApproachTargetFar(args->user, args->targets[0], 24,
                                 toFIXED(61) / 5);
  wait(26);
  args->variant = SPARK_GROUPS_MIGHTY_PRESS;
  BattleEffect_RunSparkGroups(args, SPARK_GROUPS_RISING);
}

/// The user leaps at the first target, then sparks burst beneath it and the
/// canvas flashes.
///
/// Abilities: Severe Blow.
void BattleEffect_RunSevereBlow(struct BattleEffectParams *args) {
  BattleMotion_ApproachTargetFar(args->user, args->targets[0], 24,
                                 toFIXED(36) / 5);
  wait(12);
  args->variant = SPARK_GROUPS_SEVERE_BLOW;
  BattleEffect_RunSparkGroups(args, SPARK_GROUPS_RISING);
}

/// Abilities: Quick Strike.
void BattleEffect_RunQuickStrike(struct BattleEffectParams *args) {
  BattleEffect_RunFlashingStrike(args, FLASHING_STRIKE_QUICK_STRIKE);
}

/// Abilities: Vorpal Slash.
void BattleEffect_RunVorpalSlash(struct BattleEffectParams *args) {
  BattleEffect_RunFlashingStrike(args, FLASHING_STRIKE_VORPAL_SLASH);
}

/// Abilities: Lunar Slash.
void BattleEffect_RunLunarSlash(struct BattleEffectParams *args) {
  BattleEffect_RunFlashingStrike(args, FLASHING_STRIKE_LUNAR_SLASH);
}

/// Abilities: Boost Hack.
void BattleEffect_RunBoostHack(struct BattleEffectParams *args) {
  BattleEffect_RunFlashingStrike(args, FLASHING_STRIKE_BOOST_HACK);
}

/// Abilities: Twin Shear.
void BattleEffect_RunTwinShear(struct BattleEffectParams *args) {
  BattleEffect_RunFlashingStrike(args, FLASHING_STRIKE_TWIN_SHEAR);
}

#undef RESULTS_FRAME

/// One 120x120 picture of a flashing strike; the reflected variant's are half
/// as tall.
#define FLASHING_STRIKE_PICTURE_BYTES (SLASH_PICTURE_SIZE * SLASH_PICTURE_SIZE)
#define FLASHING_STRIKE_HALF_BYTES (SLASH_PICTURE_SIZE * SLASH_PICTURE_SIZE / 2)

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
  if ((u32)(variant - FLASHING_STRIKE_LUNAR_SLASH) > 2)
    REG_BLDCNT = 0;
  REG_BLDALPHA = EFFECT_BLEND_ADD;

  // The pictures: 1 and 2 in the sheet, 3 and 4 in the map cell
  // buffer (Twin Shear's four half pictures all there).
  if (variant == FLASHING_STRIKE_UNUSED) {
    Resource_LoadAndDecompress(RESOURCE_REVEAL_MASK_E, work->sheet, 1, 0);
    Resource_LoadAndDecompress(RESOURCE_REVEAL_MASK_F, Ram_MapCellBuffer, 1, 1);
  } else if (variant == FLASHING_STRIKE_QUICK_STRIKE) {
    Resource_LoadAndDecompress(RESOURCE_REVEAL_MASK_C, work->sheet, 1, 0);
    Resource_LoadAndDecompress(RESOURCE_REVEAL_MASK_D, Ram_MapCellBuffer, 1, 1);
  } else if (variant == FLASHING_STRIKE_BOOST_HACK) {
    s32 i;

    Resource_LoadAndDecompress(RESOURCE_REVEAL_MASK_A, work->sheet, 1, 0);
    Resource_LoadAndDecompress(RESOURCE_REVEAL_MASK_B, Ram_MapCellBuffer, 1, 1);
    // Turn the first 64 BG colours: red to blue, blue to green, green to red.
    for (i = 0; i != 64; i++) {
      s32 blue = RGB_BLUE(((u16 *)BG_PLTT)[i]);
      s32 green = RGB_GREEN(((u16 *)BG_PLTT)[i]);
      s32 red = RGB_RED(((u16 *)BG_PLTT)[i]);

      ((u16 *)BG_PLTT)[i] = RGB(green, blue, red);
    }
  } else if (variant == FLASHING_STRIKE_LUNAR_SLASH) {
    Resource_LoadAndDecompress(RESOURCE_REVEAL_MASK_C, work->sheet, 1, 0);
    Resource_LoadAndDecompress(RESOURCE_REVEAL_MASK_D, Ram_MapCellBuffer, 1, 1);
  } else if (variant == FLASHING_STRIKE_TWIN_SHEAR) {
    Resource_LoadAndDecompress(RESOURCE_ARC_FRAGMENTS_SHEET, Ram_MapCellBuffer,
                               1, 1);
  } else {
    Resource_LoadAndDecompress(RESOURCE_REVEAL_MASK_A, work->sheet, 1, 0);
    Resource_LoadAndDecompress(RESOURCE_REVEAL_MASK_B, Ram_MapCellBuffer, 1, 1);
  }
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);

  // The user closes on the target in 16 frames (Lunar Slash jumps in place,
  // Boost Hack strikes a pose instead and waits 8).
  if (variant == FLASHING_STRIKE_QUICK_STRIKE ||
      variant == FLASHING_STRIKE_TWIN_SHEAR)
    BattleMotion_ApproachTargetFar(args->user, args->targets[0], 16,
                                   toFIXED(8));
  else if (variant == FLASHING_STRIKE_LUNAR_SLASH)
    // The target is a whole word: the user, passed twice, is loaded once and
    // copied, where an s16 parameter would load it again halved.
    BattleMotion_ApproachTargetFar(args->user, args->user, 16, toFIXED(10));
  else if (variant == FLASHING_STRIKE_BOOST_HACK) {
    struct MotionObject *user = BattleObject_GetFar(args->user)->object;

    Actor_SelectAnimationFar(user, 2);
    Actor_SetAnimationSpeedFar(user, ANIMATION_STEP_NORMAL * 3);
  } else
    BattleMotion_ApproachTargetFar(args->user, args->targets[0], 16, 0);
  if (variant == FLASHING_STRIKE_BOOST_HACK)
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
    if (variant == FLASHING_STRIKE_TWIN_SHEAR) {
      // Half the picture, then its reflection below it.
      if (frame <= 3) {
        draw[0](canvas, Ram_MapCellBuffer, 0, 0, SLASH_PICTURE_SIZE,
                SLASH_PICTURE_SIZE / 2);
        draw[1](canvas, Ram_MapCellBuffer, 0, SLASH_PICTURE_SIZE / 2,
                SLASH_PICTURE_SIZE, SLASH_PICTURE_SIZE / 2);
      } else if (frame <= 7) {
        draw[0](canvas, Ram_MapCellBuffer + FLASHING_STRIKE_HALF_BYTES, 0, 0,
                SLASH_PICTURE_SIZE, SLASH_PICTURE_SIZE / 2);
        draw[1](canvas, Ram_MapCellBuffer + FLASHING_STRIKE_HALF_BYTES, 0,
                SLASH_PICTURE_SIZE / 2, SLASH_PICTURE_SIZE,
                SLASH_PICTURE_SIZE / 2);
      } else if (frame <= 11) {
        draw[0](canvas, Ram_MapCellBuffer + FLASHING_STRIKE_HALF_BYTES * 2, 0,
                0, SLASH_PICTURE_SIZE, SLASH_PICTURE_SIZE / 2);
        draw[1](canvas, Ram_MapCellBuffer + FLASHING_STRIKE_HALF_BYTES * 2, 0,
                SLASH_PICTURE_SIZE / 2, SLASH_PICTURE_SIZE,
                SLASH_PICTURE_SIZE / 2);
      } else if (frame <= 15) {
        draw[0](canvas, Ram_MapCellBuffer + FLASHING_STRIKE_HALF_BYTES * 3, 0,
                0, SLASH_PICTURE_SIZE, SLASH_PICTURE_SIZE / 2);
        draw[1](canvas, Ram_MapCellBuffer + FLASHING_STRIKE_HALF_BYTES * 3, 0,
                SLASH_PICTURE_SIZE / 2, SLASH_PICTURE_SIZE,
                SLASH_PICTURE_SIZE / 2);
      }
    } else {
      if (frame <= 3)
        draw[0](canvas, work->sheet, 0, 0, SLASH_PICTURE_SIZE,
                SLASH_PICTURE_SIZE);
      else if (frame <= 7)
        draw[0](canvas, work->sheet + FLASHING_STRIKE_PICTURE_BYTES, 0, 0,
                SLASH_PICTURE_SIZE, SLASH_PICTURE_SIZE);
      else if (frame <= 11)
        draw[0](canvas, Ram_MapCellBuffer, 0, 0, SLASH_PICTURE_SIZE,
                SLASH_PICTURE_SIZE);
      else if (frame <= 15)
        draw[0](canvas, Ram_MapCellBuffer + FLASHING_STRIKE_PICTURE_BYTES, 0, 0,
                SLASH_PICTURE_SIZE, SLASH_PICTURE_SIZE);
    }

    // The flood: four frames of the canvas filled with colour 0x3f.
    if ((u32)(frame - FLOOD_FRAME) <= 3)
      Iwram_FillWordsEntry(canvas, CANVAS_BYTES,
                           CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
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
  if (variant == FLASHING_STRIKE_LUNAR_SLASH) {
    // Clear the canvas and its tiles and hand the layer to the next scene.
    Iwram_ClearWords((void *)BG_CHAR_BLOCK(1), CANVAS_VIEW_BYTES);
    Iwram_ClearWords(canvas, CANVAS_VIEW_BYTES);
    Task_Remove(Palette_StepFadeTransfer);
    BattleEffect_RunWeaken(args);
  } else {
    if (variant == FLASHING_STRIKE_BOOST_HACK)
      Actor_SetAnimationSpeedFar(BattleObject_GetFar(args->user)->object,
                                 ANIMATION_STEP_NORMAL);
    BattleEffect_EndCanvasLayer();
  }
}

#undef FLASHING_STRIKE_PICTURE_BYTES
#undef FLASHING_STRIKE_HALF_BYTES
