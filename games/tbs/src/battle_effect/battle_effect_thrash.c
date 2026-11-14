/// @file
/// Thrash, Quick Strike and Vorpal Slash.
// Part of battle_effect.c.

#include "battle_effect_internal.h"

/// One 120x120 picture of a flashing strike.
#define FLASHING_STRIKE_PICTURE_BYTES (120 * 120)

/// The flashing strike's modes (BattleEffect_RunFlashingStrike): which two
/// RevealMask resources it shows.
enum FlashingStrikeMode {
  FLASHING_STRIKE_UNUSED,       ///< RevealMaskE and F; nothing passes it
  FLASHING_STRIKE_QUICK_STRIKE, ///< RevealMaskC and D, with a hop
  FLASHING_STRIKE_VORPAL_SLASH  ///< RevealMaskA and B
};

/// The user leaps high at the first target, then sparks burst beneath it.
///
/// Abilities: Thrash.
void BattleEffect_RunThrash(struct BattleEffectParams *args) {
  BattleMotion_ApproachTargetFar(args->user, args->targets[0], 24, px(61) / 5);
  wait(29);
  args->variant = SPARK_GROUPS_THRASH;
  BattleEffect_RunSparkGroups(args, SPARK_GROUPS_RISING);
}

/// Unused: no row of gAbilityBattleEffects names its kind. The user would leap
/// at the first target, then sparks would burst beneath it and the canvas
/// flash; it is Severe Blow's effect in The Lost Age.
void BattleEffect_RunUnusedSevereBlowLeap(struct BattleEffectParams *args) {
  BattleMotion_ApproachTargetFar(args->user, args->targets[0], 24, px(36) / 5);
  wait(12);
  args->variant = SPARK_GROUPS_SEVERE_BLOW;
  BattleEffect_RunSparkGroups(args, SPARK_GROUPS_RISING);
}

/// The user leaps at the first target, four pictures flash across the effect
/// layer, then it fills with one colour as the target is struck.
///
/// Abilities: Quick Strike.
void BattleEffect_RunQuickStrike(struct BattleEffectParams *args) {
  BattleEffect_RunFlashingStrike(args, FLASHING_STRIKE_QUICK_STRIKE);
}

/// The user steps up to the first target, four pictures flash across the
/// effect layer, then it fills with one colour as the target is struck.
///
/// Abilities: Vorpal Slash.
void BattleEffect_RunVorpalSlash(struct BattleEffectParams *args) {
  BattleEffect_RunFlashingStrike(args, FLASHING_STRIKE_VORPAL_SLASH);
}

/// Plays a flashing strike; nothing passes FLASHING_STRIKE_UNUSED.
void BattleEffect_RunFlashingStrike(struct BattleEffectParams *args, s32 mode) {
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle draw[2];
  s32 frame;

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  work->args = args;
  BattleEffect_StartCanvasLayer(0);
  REG_BLDCNT = 0;

  if (mode == FLASHING_STRIKE_UNUSED) {
    Resource_LoadAndDecompress(RESOURCE_REVEAL_MASK_E, work->imageStorage, 1,
                               0);
    Resource_LoadAndDecompress(RESOURCE_REVEAL_MASK_F, Ram_MapGridCellBuffer, 1,
                               1);
  } else if (mode == FLASHING_STRIKE_QUICK_STRIKE) {
    Resource_LoadAndDecompress(RESOURCE_REVEAL_MASK_C, work->imageStorage, 1,
                               0);
    Resource_LoadAndDecompress(RESOURCE_REVEAL_MASK_D, Ram_MapGridCellBuffer, 1,
                               1);
  } else {
    Resource_LoadAndDecompress(RESOURCE_REVEAL_MASK_A, work->imageStorage, 1,
                               0);
    Resource_LoadAndDecompress(RESOURCE_REVEAL_MASK_B, Ram_MapGridCellBuffer, 1,
                               1);
  }
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);

  if (mode == FLASHING_STRIKE_QUICK_STRIKE)
    BattleMotion_ApproachTargetFar(args->user, args->targets[0], 16, px(8));
  else
    BattleMotion_ApproachTargetFar(args->user, args->targets[0], 16, 0);
  wait(16);

  if (work->args->side == EFFECT_SIDE_PARTY)
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_COPY);
  else
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, COMPOSITE_COPY);
  draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  Sound_PlayFar(SE_POWER_UP);
  for (frame = 0; frame != 21; frame++) {
    if (frame <= 3)
      draw[0](canvas, work->imageStorage, 0, 0, 120, 120);
    else if (frame <= 7)
      draw[0](canvas, work->imageStorage + FLASHING_STRIKE_PICTURE_BYTES, 0, 0,
              120, 120);
    else if (frame <= 11)
      draw[0](canvas, Ram_MapGridCellBuffer, 0, 0, 120, 120);
    else if (frame <= 15)
      draw[0](canvas, Ram_MapGridCellBuffer + FLASHING_STRIKE_PICTURE_BYTES, 0,
              0, 120, 120);
    if (frame >= 16 && frame <= 19)
      Iwram_FillWords(canvas, CANVAS_BYTES,
                      CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
    if (frame == 18)
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
    if (frame == 20) {
      work->shakeFrames = 8;
      BattleMotion_ApplyKnockbackFar(work->args->targets[0], KNOCKBACK_HURL);
    }
    BattleCamera_Shake(16, 16);
    EFFECT_END_FRAME(work);
  }
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

#undef FLASHING_STRIKE_PICTURE_BYTES
