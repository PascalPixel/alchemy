/// @file
/// Flare, and an unused routine that lists the indices around a value.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

#define FLARE_FRAMES 60 ///< frames of the flare

#define FLARE_ARC_COUNT 9 ///< flames on the arc
/// The half ellipse's radii, across and up and down, in canvas pixels.
#define FLARE_ARC_RADIUS_X 32
#define FLARE_ARC_RADIUS_Y 16
/// A flame's six pictures, four frames each, before it goes out.
#define FLARE_FLAME_PICTURES 6
#define FLARE_PICTURE_FRAMES 4

/// Nine flames rise one after another along half an ellipse.
///
/// Abilities: Flare, Flare Wall, Flare Storm.
void BattleEffect_RunFlare(struct BattleEffectParams *args) {
  struct BattleEffectState *state;
  void *canvas;
  s32 frame;
  DrawRectangle draw;
  s32 i;
  s32 picture;
  u32 width;
  s32 direction;
  s32 originX;
  struct Particle *arcFlame;
  struct Particle *flame;

  state = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  BattleEffect_StartCanvasLayer(0);
  REG_BLDCNT =
      BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  Resource_LoadAndDecompress(RESOURCE_EMBER_STREAK_SHEET, state->sheet, 1, 1);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
  draw = (DrawRectangle)Ram_HeapSlots->ramCode;

  // The arc of flames, from the canvas's left edge over the enemies and back
  // from its right half over the party, each flame starting four frames
  // after the one before.
  if (args->targets[0] >= FIRST_ENEMY_UNIT) {
    originX = 0;
    direction = 1;
  } else {
    originX = CANVAS_SIZE / 2;
    direction = -1;
  }
  for (i = 0; i != FLARE_ARC_COUNT; i++) {
    s32 angle = i * (TRIG_TURN / 16) - DEGtoANG(90);

    arcFlame = &state->particles[i];
    arcFlame->x =
        ((direction * FIXEDtoINT(Trig_Sin(angle) * FLARE_ARC_RADIUS_X)) +
         originX) +
        20;
    arcFlame->y = FIXEDtoINT(Trig_Cos(angle) * FLARE_ARC_RADIUS_Y) + 40;
    arcFlame->variant = -i * 4;
  }

  state->transferMode = CANVAS_COPY_AND_FADE;
  if (args->variant == EFFECT_VARIANT_STRONGEST)
    state->transferValue = CANVAS_FADE_THREE_QUARTERS;
  else
    state->transferValue = CANVAS_FADE_HALF;
  Task_Add(BattleEffect_TransferCanvas, TASK_PASS_VBLANK);
  Sound_PlayFar(SE_WHOOSH);
  for (frame = 0; frame != FLARE_FRAMES; frame++) {
    if (frame == 24)
      EFFECT_SHOW_DAMAGE(SE_IMPACT);

    // Each flame plays its pictures and goes out; Flare Wall's and Flare
    // Storm's stack a second and a third above it.
    for (i = 0; i != FLARE_ARC_COUNT; i++) {
      flame = &state->particles[i];
      if (flame->variant >= 0 &&
          flame->variant < FLARE_FLAME_PICTURES * FLARE_PICTURE_FRAMES) {
        picture = flame->variant / FLARE_PICTURE_FRAMES;
        // UB: width is assigned in one argument and read in another.
#ifdef UBFIX
        width = gBattleEffect_EmberStreakPictureWidths[picture];
        draw(canvas, state->sheet + gBattleEffect_EmberStreakPictures[picture],
             flame->x - (width >> 1),
             flame->y + gBattleEffect_EmberStreakPictureBiasY[picture], width,
             gBattleEffect_EmberStreakPictureHeights[picture]);
#else
        draw(canvas, state->sheet + gBattleEffect_EmberStreakPictures[picture],
             flame->x -
                 ((width = gBattleEffect_EmberStreakPictureWidths[picture]) >>
                  1),
             flame->y + gBattleEffect_EmberStreakPictureBiasY[picture], width,
             gBattleEffect_EmberStreakPictureHeights[picture]);
#endif
        if (args->variant != EFFECT_VARIANT_BASIC) {
#ifdef UBFIX
          width = gBattleEffect_EmberStreakPictureWidths[picture];
          draw(canvas,
               state->sheet + gBattleEffect_EmberStreakPictures[picture],
               flame->x - (width >> 1),
               (flame->y + gBattleEffect_EmberStreakPictureBiasY[picture]) - 16,
               width, gBattleEffect_EmberStreakPictureHeights[picture]);
#else
          draw(canvas,
               state->sheet + gBattleEffect_EmberStreakPictures[picture],
               flame->x -
                   ((width = gBattleEffect_EmberStreakPictureWidths[picture]) >>
                    1),
               (flame->y + gBattleEffect_EmberStreakPictureBiasY[picture]) - 16,
               width, gBattleEffect_EmberStreakPictureHeights[picture]);
#endif
        }
        if (args->variant == EFFECT_VARIANT_STRONGEST) {
#ifdef UBFIX
          width = gBattleEffect_EmberStreakPictureWidths[picture];
          draw(canvas,
               state->sheet + gBattleEffect_EmberStreakPictures[picture],
               flame->x - (width >> 1),
               (flame->y + gBattleEffect_EmberStreakPictureBiasY[picture]) - 32,
               width, gBattleEffect_EmberStreakPictureHeights[picture]);
#else
          draw(canvas,
               state->sheet + gBattleEffect_EmberStreakPictures[picture],
               flame->x -
                   ((width = gBattleEffect_EmberStreakPictureWidths[picture]) >>
                    1),
               (flame->y + gBattleEffect_EmberStreakPictureBiasY[picture]) - 32,
               width, gBattleEffect_EmberStreakPictureHeights[picture]);
#endif
        }
      }
      flame->variant++;
    }

    // The targets flash red, eight frames apart.
    EFFECT_FOR_EACH_TARGET(i, args) {
      if (frame == (i * 8) + 16)
        BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_RED_TINT,
                                 POSE_DAMAGE, i, 12);
    }
    EFFECT_END_FRAME(state);
  }
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  Task_Remove(BattleEffect_TransferCanvas);
  BattleEffect_EndCanvasLayer();
}

/// Lists the indices within indexRadius of where currentValue sits in values,
/// up to entryLimit, and returns how many.
UNUSED s32
BattleEffect_BuildCenteredIndexList(struct BattleEffectIndexWindow *window,
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

#undef FLARE_FRAMES
#undef FLARE_ARC_COUNT
#undef FLARE_ARC_RADIUS_X
#undef FLARE_ARC_RADIUS_Y
#undef FLARE_FLAME_PICTURES
#undef FLARE_PICTURE_FRAMES
