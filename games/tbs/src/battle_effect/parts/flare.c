/// @file
/// Flare.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"

#define FLARE_ARC_COUNT 9 ///< flames on the arc

/// Nine flames rise one after another, 4 frames apart, along half an ellipse,
/// over 80 frames.
///
/// Abilities: Flare, Flare Wall, Flare Storm (BATTLE_EFFECT_FLARE).
///
/// Each target flashes red in turn, 8 frames apart from frame 16. Writes
/// REG_BLDCNT and REG_BLDALPHA, and ends with the blitter freed, the transfer
/// task removed and the canvas layer closed.
///
/// args: the targets. Flare Wall, the stronger variant, adds a second flame
///         16 pixels above each one, and Flare Storm, the strongest, a third
///         32 pixels above (and longer trails).
void BattleEffect_RunFlare(struct BattleEffectParams *args) {
  u32 *slots;
  u32 *cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle draw;
  s32 i;
  s32 cell;
  u32 width;
  s32 direction;
  s32 originX;
  s32 angle;
  s32 delay;
  struct Particle *arcFlame;
  struct Particle *flame;

  // The work, the canvas, and BG1 and BG2 blended at 14:16.
  slots = (u32 *)&gHeapSlots[HEAP_SLOT_BATTLE_EFFECT];
  cursor = slots;
  work = (struct BattleEffectWork *)*cursor++;
  canvas = (void *)*cursor;
  work->args = args;
  BattleEffect_StartCanvasLayer(0);
  REG_BLDCNT =
      BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BLDALPHA = BLEND_ALPHA(14, 16);
  Resource_LoadAndDecompress((s32)&ResourceId_EmberStreakSheet, work->sheet, 1,
                             1);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_ADDITIVE);
  draw = (DrawRectangle)EFFECT_SLOT(slots, HEAP_SLOT_RAM_CODE);

  // The arc: from -90 to 90 degrees in steps of 22.5, half an ellipse with
  // radii of 32 pixels across and 16 up and down, centred on x 20 (84 on the
  // party's side) and dipping from y 40 at its ends to 56 at its middle.
  // Against enemies (ids from FIRST_ENEMY_UNIT) it runs from the left,
  // otherwise from the right.
  if (work->args->targets[0] >= FIRST_ENEMY_UNIT) {
    originX = 0;
    direction = 1;
  } else {
    originX = 64;
    direction = -1;
  }
  angle = -EFFECT_TURN / 4;
  i = 0;
  delay = 0;
  // Each flame starts 4 frames after the one before: its variant counts up
  // from a negative delay, and the flame shows from 0.
  arcFlame = work->particles;
  do {
    arcFlame->x =
        ((direction * (((Trig_Sin(angle) * 32)) >> 16)) + originX) + 20;
    arcFlame->y = (((Trig_Cos(angle) * 16)) >> 16) + 40;
    arcFlame->variant = delay;
    angle += EFFECT_TURN / 16;
    delay -= 4;
    i += 1;
    arcFlame++;
  } while (i != FLARE_ARC_COUNT);

  // Trails keep half of each level, or three quarters for Flare Storm.
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
    // Frame 24 lets the battle show the damage, with SE_IMPACT as the hit
    // sound.
    if (frame == 24) {
      EFFECT_SHOW_DAMAGE(SE_IMPACT);
    }

    // Each flame shows six pictures, 4 frames each, and goes out; the last
    // flame (delay -32) is out after frame 55. Each puff cell stands on its
    // own baseline (gBattleEffect_PuffArcCellBiasY) and is centred across x.
    i = 0;
    flame = work->particles;
    do {
      if ((flame->variant >= 0) && (flame->variant <= 23)) {
        cell = flame->variant / 4;
        // UB: width is assigned in one argument and read in another.
#ifdef UBFIX
        width = gBattleEffect_PuffArcCellWidths[cell];
        draw(canvas, work->sheet + gBattleEffect_PuffArcCells[cell],
             flame->x - (width >> 1),
             flame->y + gBattleEffect_PuffArcCellBiasY[cell], width,
             gBattleEffect_PuffArcCellHeights[cell]);
#else
        draw(canvas, work->sheet + gBattleEffect_PuffArcCells[cell],
             flame->x - ((width = gBattleEffect_PuffArcCellWidths[cell]) >> 1),
             flame->y + gBattleEffect_PuffArcCellBiasY[cell], width,
             gBattleEffect_PuffArcCellHeights[cell]);
#endif
        if (work->args->variant != EFFECT_VARIANT_BASIC) {
#ifdef UBFIX
          width = gBattleEffect_PuffArcCellWidths[cell];
          draw(canvas, work->sheet + gBattleEffect_PuffArcCells[cell],
               flame->x - (width >> 1),
               (flame->y + gBattleEffect_PuffArcCellBiasY[cell]) - 16, width,
               gBattleEffect_PuffArcCellHeights[cell]);
#else
          draw(canvas, work->sheet + gBattleEffect_PuffArcCells[cell],
               flame->x -
                   ((width = gBattleEffect_PuffArcCellWidths[cell]) >> 1),
               (flame->y + gBattleEffect_PuffArcCellBiasY[cell]) - 16, width,
               gBattleEffect_PuffArcCellHeights[cell]);
#endif
        }
        if (work->args->variant == EFFECT_VARIANT_STRONGEST) {
#ifdef UBFIX
          width = gBattleEffect_PuffArcCellWidths[cell];
          draw(canvas, work->sheet + gBattleEffect_PuffArcCells[cell],
               flame->x - (width >> 1),
               (flame->y + gBattleEffect_PuffArcCellBiasY[cell]) - 32, width,
               gBattleEffect_PuffArcCellHeights[cell]);
#else
          draw(canvas, work->sheet + gBattleEffect_PuffArcCells[cell],
               flame->x -
                   ((width = gBattleEffect_PuffArcCellWidths[cell]) >> 1),
               (flame->y + gBattleEffect_PuffArcCellBiasY[cell]) - 32, width,
               gBattleEffect_PuffArcCellHeights[cell]);
#endif
        }
      }
      i += 1;
      flame->variant += 1;
      flame++;
    } while (i != FLARE_ARC_COUNT);

    // The targets flash red, 8 frames apart from frame 16.
    i = 0;
    while (i != work->args->targetCount) {
      if (frame == (i * 8) + 16) {
        BattleEffect_TintBattler(work->args->targets[i], SPRITE_COLOR_RED_TINT,
                                 POSE_DAMAGE, i, 12);
      }
      i += 1;
    }
    EFFECT_END_FRAME(work);
    frame += 1;
  } while (frame != 80);
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

/// Lists the indices within indexRadius of where currentValue sits in values,
/// keeping only those from 0 to entryLimit - 1. Writes the indices to output
/// and returns how many there are.
///
/// Only the first indexRadius * 2 + 1 values are searched, and with no match
/// the window centres on index 0. Changes nothing but output. Nothing calls it.
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
