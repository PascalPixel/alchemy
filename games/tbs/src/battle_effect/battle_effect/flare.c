/// @file
/// Flare.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"

#define FLARE_ARC_COUNT 9 ///< flames on the arc

/// Nine flames rise one after another along half an ellipse.
///
/// Abilities: Flare, Flare Wall, Flare Storm.
void BattleEffect_RunFlare(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle draw;
  s32 i;
  s32 picture;
  u32 width;
  s32 direction;
  s32 originX;
  s32 angle;
  s32 delay;
  struct Particle *arcFlame;
  struct Particle *flame;

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  work->args = args;
  BattleEffect_StartCanvasLayer(0);
  REG_BLDCNT =
      BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BLDALPHA = BLEND_ALPHA(14, 16);
  Resource_LoadAndDecompress((s32)&ResourceId_EmberStreakSheet, work->sheet, 1,
                             1);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_ADDITIVE);
  draw = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);

  // The arc of flames.
  if (work->args->targets[0] >= FIRST_ENEMY_UNIT) {
    originX = 0;
    direction = 1;
  } else {
    originX = 64;
    direction = -1;
  }
  angle = -TRIG_TURN / 4;
  i = 0;
  delay = 0;
  arcFlame = work->particles;
  do {
    arcFlame->x =
        ((direction * (((Trig_Sin(angle) * 32)) >> 16)) + originX) + 20;
    arcFlame->y = (((Trig_Cos(angle) * 16)) >> 16) + 40;
    arcFlame->variant = delay;
    angle += TRIG_TURN / 16;
    delay -= 4;
    i += 1;
    arcFlame++;
  } while (i != FLARE_ARC_COUNT);

  work->transferMode = CANVAS_COPY_AND_FADE;
  if (work->args->variant == EFFECT_VARIANT_STRONGEST) {
    work->transferValue = CANVAS_FADE_THREE_QUARTERS;
  } else {
    work->transferValue = CANVAS_FADE_HALF;
  }
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK);
  Sound_PlayFar(SE_WHOOSH);
  frame = 0;
  do {
    if (frame == 24) {
      EFFECT_SHOW_DAMAGE(SE_IMPACT);
    }

    // Each flame plays six pictures and goes out.
    i = 0;
    flame = work->particles;
    do {
      if ((flame->variant >= 0) && (flame->variant <= 23)) {
        picture = flame->variant / 4;
        // UB: width is assigned in one argument and read in another.
#ifdef UBFIX
        width = gBattleEffect_PuffArcPictureWidths[picture];
        draw(canvas, work->sheet + gBattleEffect_PuffArcPictures[picture],
             flame->x - (width >> 1),
             flame->y + gBattleEffect_PuffArcPictureBiasY[picture], width,
             gBattleEffect_PuffArcPictureHeights[picture]);
#else
        draw(canvas, work->sheet + gBattleEffect_PuffArcPictures[picture],
             flame->x -
                 ((width = gBattleEffect_PuffArcPictureWidths[picture]) >> 1),
             flame->y + gBattleEffect_PuffArcPictureBiasY[picture], width,
             gBattleEffect_PuffArcPictureHeights[picture]);
#endif
        if (work->args->variant != EFFECT_VARIANT_BASIC) {
#ifdef UBFIX
          width = gBattleEffect_PuffArcPictureWidths[picture];
          draw(canvas, work->sheet + gBattleEffect_PuffArcPictures[picture],
               flame->x - (width >> 1),
               (flame->y + gBattleEffect_PuffArcPictureBiasY[picture]) - 16,
               width, gBattleEffect_PuffArcPictureHeights[picture]);
#else
          draw(canvas, work->sheet + gBattleEffect_PuffArcPictures[picture],
               flame->x -
                   ((width = gBattleEffect_PuffArcPictureWidths[picture]) >> 1),
               (flame->y + gBattleEffect_PuffArcPictureBiasY[picture]) - 16,
               width, gBattleEffect_PuffArcPictureHeights[picture]);
#endif
        }
        if (work->args->variant == EFFECT_VARIANT_STRONGEST) {
#ifdef UBFIX
          width = gBattleEffect_PuffArcPictureWidths[picture];
          draw(canvas, work->sheet + gBattleEffect_PuffArcPictures[picture],
               flame->x - (width >> 1),
               (flame->y + gBattleEffect_PuffArcPictureBiasY[picture]) - 32,
               width, gBattleEffect_PuffArcPictureHeights[picture]);
#else
          draw(canvas, work->sheet + gBattleEffect_PuffArcPictures[picture],
               flame->x -
                   ((width = gBattleEffect_PuffArcPictureWidths[picture]) >> 1),
               (flame->y + gBattleEffect_PuffArcPictureBiasY[picture]) - 32,
               width, gBattleEffect_PuffArcPictureHeights[picture]);
#endif
        }
      }
      i += 1;
      flame->variant += 1;
      flame++;
    } while (i != FLARE_ARC_COUNT);

    // The targets flash red.
    for (i = 0; i != work->args->targetCount; i++) {
      if (frame == (i * 8) + 16) {
        BattleEffect_TintBattler(work->args->targets[i], SPRITE_COLOR_RED_TINT,
                                 POSE_DAMAGE, i, 12);
      }
    }
    EFFECT_END_FRAME(work);
    frame += 1;
  } while (frame != 80);
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

/// Lists the indices within indexRadius of where currentValue sits in values,
/// up to entryLimit, and returns how many.
UNUSED s32 BattleEffect_BuildCenteredIndexList(BattleEffectIndexWindow *window,
                                               s16 *output) {
  s32 windowLength = window->indexRadius * 2 + 1;
  s32 outputCount = 0;
  s32 centerIndex = 0;
  s32 windowIndex;

  for (windowIndex = 0; windowIndex != windowLength; windowIndex++) {
    if (window->currentValue == window->values[windowIndex]) {
      centerIndex = windowIndex;
      break;
    }
  }
  for (windowIndex = 0; windowIndex != windowLength; windowIndex++) {
    s32 index = centerIndex + windowIndex - window->indexRadius;

    if (index >= 0 && index < window->entryLimit) {
      output[outputCount++] = index;
    }
  }
  return outputCount;
}

#undef FLARE_ARC_COUNT
