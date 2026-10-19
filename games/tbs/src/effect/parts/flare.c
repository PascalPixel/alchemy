/// @file
/// Part of effect.c, the battle effect object, compiled only through it.
/// Flare.

#include "../effect_internal.h"

#define FLARE_ARC_COUNT 9 // flames on the arc

/// Abilities: Flare, Flare Wall, Flare Storm (kind 11).
///
/// Flame arc: nine flames rise one after another, 4 frames apart, along a
/// half circle, over 80 frames. Each target is hit in turn, 8 frames apart
/// from frame 16.
///
/// effect: the targets. variant 1 adds a second flame 16 pixels above each
///         one, variant 2 a third 32 pixels above (and longer trails).
void BattleFx_RunFlare(struct BattleEffectArgument *effect) {
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
  struct EffectStep *arcFlame;
  struct EffectStep *flame;

  // The work, the canvas, and BG1 and BG2 blended at 14:16.
  slots = (u32 *)&gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];
  cursor = slots;
  work = (struct BattleEffectWork *)*cursor++;
  canvas = (void *)*cursor;
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);
  REG_BLDCNT =
      BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BLDALPHA = BLEND_ALPHA(14, 16);
  Resource_LoadAndDecompress((s32)&ResourceId_EmberStreakSheet, work->sheet, 1,
                             1);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_ADDITIVE);
  draw = (DrawRectangle)EFFECT_SLOT(slots, HEAP_SLOT_BLITTER);

  // The arc: from -90 degrees in steps of 22.5, 32 pixels across and 16
  // high. Against enemies (ids from 128) it runs from the left, otherwise
  // from the right.
  if (work->effect->actors[0] > 127) {
    originX = 0;
    direction = 1;
  } else {
    originX = 64;
    direction = -1;
  }
  angle = -0x4000;
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
    angle += 0x1000;
    delay -= 4;
    i += 1;
    arcFlame++;
  } while (i != FLARE_ARC_COUNT);

  work->transferMode = CANVAS_COPY_AND_FADE;
  if (work->effect->variant == 2) {
    work->transferValue = 75;
  } else {
    work->transferValue = 50;
  }
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer,
                                EFFECT_CALLBACK_ORDER);
  AudioCommand_PlayFar(SE_136);
  frame = 0;
  do {
    // Frame 24 lets the battle show the damage.
    if (frame == 24) {
      EFFECT_SHOW_DAMAGE(SE_133);
    }

    // Each flame shows six pictures, 4 frames each, and goes out.
    i = 0;
    flame = work->particles;
    do {
      if ((flame->variant >= 0) && (flame->variant <= 23)) {
        cell = flame->variant / 4;
        // UB: width is assigned in one argument and read in another.
#ifdef UBFIX
        width = gBattleFx_PuffArcCellWidths[cell];
        draw(canvas, work->sheet + gBattleFx_PuffArcCells[cell],
             flame->x - (width >> 1),
             flame->y + gBattleFx_PuffArcCellBiasY[cell], width,
             gBattleFx_PuffArcCellHeights[cell]);
#else
        draw(canvas, work->sheet + gBattleFx_PuffArcCells[cell],
             flame->x - ((width = gBattleFx_PuffArcCellWidths[cell]) >> 1),
             flame->y + gBattleFx_PuffArcCellBiasY[cell], width,
             gBattleFx_PuffArcCellHeights[cell]);
#endif
        if (work->effect->variant != 0) {
#ifdef UBFIX
          width = gBattleFx_PuffArcCellWidths[cell];
          draw(canvas, work->sheet + gBattleFx_PuffArcCells[cell],
               flame->x - (width >> 1),
               (flame->y + gBattleFx_PuffArcCellBiasY[cell]) - 16, width,
               gBattleFx_PuffArcCellHeights[cell]);
#else
          draw(canvas, work->sheet + gBattleFx_PuffArcCells[cell],
               flame->x - ((width = gBattleFx_PuffArcCellWidths[cell]) >> 1),
               (flame->y + gBattleFx_PuffArcCellBiasY[cell]) - 16, width,
               gBattleFx_PuffArcCellHeights[cell]);
#endif
        }
        if (work->effect->variant == 2) {
#ifdef UBFIX
          width = gBattleFx_PuffArcCellWidths[cell];
          draw(canvas, work->sheet + gBattleFx_PuffArcCells[cell],
               flame->x - (width >> 1),
               (flame->y + gBattleFx_PuffArcCellBiasY[cell]) - 32, width,
               gBattleFx_PuffArcCellHeights[cell]);
#else
          draw(canvas, work->sheet + gBattleFx_PuffArcCells[cell],
               flame->x - ((width = gBattleFx_PuffArcCellWidths[cell]) >> 1),
               (flame->y + gBattleFx_PuffArcCellBiasY[cell]) - 32, width,
               gBattleFx_PuffArcCellHeights[cell]);
#endif
        }
      }
      i += 1;
      flame->variant += 1;
      flame++;
    } while (i != FLARE_ARC_COUNT);

    i = 0;
    while (i != work->effect->count) {
      if (frame == (i * 8) + 16) {
        ObjectGroup_UpdateMembers(work->effect->actors[i],
                                  SPRITE_COLOR_RED_TINT, POSE_DAMAGE, i, 12);
      }
      i += 1;
    }
    EFFECT_END_FRAME(work);
    frame += 1;
  } while (frame != 80);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  BattleFx_EndCanvasLayer();
}

/// List the indices within indexRadius of where currentValue sits in values,
/// keeping only those from 0 to entryLimit - 1. Writes the indices to output
/// and returns how many there are.
UNUSED s32 BattleFx_BuildCenteredIndexList(BattleEffectIndexWindow *window,
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
