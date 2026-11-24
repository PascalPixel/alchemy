/// @file
/// Thrash, Quick Strike and Vorpal Slash.
// Part of battle_effect.c.

#include "battle_effect_internal.h"

/// The flashing strike's frames, from the user's arrival: four pictures flash
/// four frames each, then the canvas floods at its brightest for four frames,
/// the damage shows and the target is hurled.
#define FLASHING_STRIKE_FRAMES 21
#define FLASHING_STRIKE_FLOOD_FRAME 16
#define FLASHING_STRIKE_DAMAGE_FRAME 18
#define FLASHING_STRIKE_HIT_FRAME 20

/// One 120x120 picture of a flashing strike.
#define FLASHING_STRIKE_PICTURE_BYTES (SLASH_PICTURE_SIZE * SLASH_PICTURE_SIZE)

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
  BattleMotion_ApproachTargetFar(args->user, args->targets[0], 24,
                                 toFIXED(61) / 5);
  wait(29);
  args->variant = SPARK_GROUPS_THRASH;
  BattleEffect_RunSparkGroups(args, SPARK_GROUPS_RISING);
}

/// Unused: no row of gAbilityBattleEffects names its kind. The user would leap
/// at the first target, then sparks would burst beneath it and the canvas
/// flash; it is Severe Blow's effect in The Lost Age.
void BattleEffect_RunUnusedSevereBlowLeap(struct BattleEffectParams *args) {
  BattleMotion_ApproachTargetFar(args->user, args->targets[0], 24,
                                 toFIXED(36) / 5);
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
  struct BattleEffectState *state;
  void *canvas;
  DrawRectangle draw[2];
  s32 frame;

  state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  state->args = args;
  BattleEffect_StartCanvasLayer(0);
  REG_BLDCNT = 0;

  if (mode == FLASHING_STRIKE_UNUSED) {
    Resource_LoadAndDecompress(RESOURCE_REVEAL_MASK_E, state->imageStorage, 1,
                               0);
    Resource_LoadAndDecompress(RESOURCE_REVEAL_MASK_F, Ram_MapGridCellBuffer, 1,
                               1);
  } else if (mode == FLASHING_STRIKE_QUICK_STRIKE) {
    Resource_LoadAndDecompress(RESOURCE_REVEAL_MASK_C, state->imageStorage, 1,
                               0);
    Resource_LoadAndDecompress(RESOURCE_REVEAL_MASK_D, Ram_MapGridCellBuffer, 1,
                               1);
  } else {
    Resource_LoadAndDecompress(RESOURCE_REVEAL_MASK_A, state->imageStorage, 1,
                               0);
    Resource_LoadAndDecompress(RESOURCE_REVEAL_MASK_B, Ram_MapGridCellBuffer, 1,
                               1);
  }
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);

  // The user closes on the target in 16 frames, Quick Strike with a hop.
  if (mode == FLASHING_STRIKE_QUICK_STRIKE)
    BattleMotion_ApproachTargetFar(args->user, args->targets[0], 16,
                                   toFIXED(8));
  else
    BattleMotion_ApproachTargetFar(args->user, args->targets[0], 16, 0);
  wait(16);

  if (state->args->side == EFFECT_SIDE_PARTY)
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_COPY);
  else
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, COMPOSITE_COPY);
  draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  Sound_PlayFar(SE_POWER_UP);
  for (frame = 0; frame != FLASHING_STRIKE_FRAMES; frame++) {
    if (frame <= 3)
      draw[0](canvas, state->imageStorage, 0, 0, SLASH_PICTURE_SIZE,
              SLASH_PICTURE_SIZE);
    else if (frame <= 7)
      draw[0](canvas, state->imageStorage + FLASHING_STRIKE_PICTURE_BYTES, 0, 0,
              SLASH_PICTURE_SIZE, SLASH_PICTURE_SIZE);
    else if (frame <= 11)
      draw[0](canvas, Ram_MapGridCellBuffer, 0, 0, SLASH_PICTURE_SIZE,
              SLASH_PICTURE_SIZE);
    else if (frame <= 15)
      draw[0](canvas, Ram_MapGridCellBuffer + FLASHING_STRIKE_PICTURE_BYTES, 0,
              0, SLASH_PICTURE_SIZE, SLASH_PICTURE_SIZE);
    // The flood: four frames of the canvas at its brightest.
    if (frame >= FLASHING_STRIKE_FLOOD_FRAME &&
        frame <= FLASHING_STRIKE_FLOOD_FRAME + 3)
      Iwram_FillWords(canvas, CANVAS_BYTES,
                      CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
    if (frame == FLASHING_STRIKE_DAMAGE_FRAME)
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
    if (frame == FLASHING_STRIKE_HIT_FRAME) {
      state->shakeFrames = 8;
      BattleMotion_ApplyKnockbackFar(state->args->targets[0], KNOCKBACK_HURL);
    }
    BattleCamera_Shake(16, 16);
    EFFECT_END_FRAME(state);
  }
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

#undef FLASHING_STRIKE_FRAMES
#undef FLASHING_STRIKE_FLOOD_FRAME
#undef FLASHING_STRIKE_DAMAGE_FRAME
#undef FLASHING_STRIKE_HIT_FRAME
#undef FLASHING_STRIKE_PICTURE_BYTES
