/// @file
/// Flare.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"
#include "fixed_math.h"
#include "sounds.h"

#define FLAME_COUNT 9

// Nine flames puff up along a half-circle arc, four frames apart.

// Setup: the flames are placed on an arc 32 pixels wide and 16 high, from
// the left when the first target is an enemy (128 and up), else from the
// right, each one starting four frames after the last. Frame loop: a flame
// steps through six pictures, four frames each; variant 1 adds a copy 16
// pixels higher, variant 2 another 32 higher and a brighter trail. The
// targets are hit from frame 16, eight frames apart.
void BattleEffect_RunFlare(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle draw;
  s32 i;
  s32 cell;
  u32 w;
  s32 sign;
  s32 org;
  s32 angle;
  s32 stagger;
  struct EffectStep *puff;
  struct EffectStep *p;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  BattleEffect_StartCanvasLayer(0);
  REG_BLDCNT =
      BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  Resource_LoadAndDecompress((s32)&ResourceId_EmberStreakSheet, work->sheet, 1,
                             1);
  BattleEffect_LoadBlitter(
      104, BLIT_MODE(BLIT_ADDITIVE, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  draw = (DrawRectangle)Ram_HeapSlots->unk64[1];

  // The arc: from -90 degrees in steps of 22.5.
  if (effect->actors[0] > 127) {
    org = 0;
    sign = 1;
  } else {
    org = 64;
    sign = -1;
  }
  angle = -TRIG_TURN / 4;
  i = 0;
  stagger = 0;
  puff = work->particles;
  do {
    puff->x = ((sign * ((Trig_Sin(angle) << 5) >> 16)) + org) + 20;
    puff->y = ((Trig_Cos(angle) << 4) >> 16) + 40;
    puff->variant = stagger;
    angle += TRIG_TURN / 16;
    stagger -= 4;
    i += 1;
    puff++;
  } while (i != FLAME_COUNT);

  work->transferMode = 2;
  if (effect->variant == 2)
    work->transferValue = 75;
  else
    work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  Sound_PlayFar(SE_WHOOSH);
  frame = 0;
  do {
    if (frame == 24)
      BattlePlayback_StartFar(133);

    // A flame moves to its next picture every four frames and goes out
    // after the sixth.
    i = 0;
    p = work->particles;
    do {
      if (p->variant >= 0 && p->variant <= 23) {
        cell = p->variant / 4;
        draw(canvas, work->sheet + gBattleEffect_PuffArcCells[cell],
             p->x - ((w = gData_08197467[cell]) >> 1),
             p->y + gBattleEffect_PuffArcCellBiasY[cell], w,
             gBattleEffect_PuffArcCellHeights[cell]);
        if (effect->variant != 0)
          draw(canvas, work->sheet + gBattleEffect_PuffArcCells[cell],
               p->x - ((w = gData_08197467[cell]) >> 1),
               (p->y + gBattleEffect_PuffArcCellBiasY[cell]) - 16, w,
               gBattleEffect_PuffArcCellHeights[cell]);
        if (effect->variant == 2)
          draw(canvas, work->sheet + gBattleEffect_PuffArcCells[cell],
               p->x - ((w = gData_08197467[cell]) >> 1),
               (p->y + gBattleEffect_PuffArcCellBiasY[cell]) - 32, w,
               gBattleEffect_PuffArcCellHeights[cell]);
      }
      i += 1;
      p->variant += 1;
      p++;
    } while (i != FLAME_COUNT);

    // Hit the targets, eight frames apart.
    i = 0;
    while (i != effect->count) {
      if (frame == (i * 8) + 16)
        BattleEffect_TintBattler(effect->actors[i], 10, 5, i, 12);
      i += 1;
    }
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
    frame += 1;
  } while (frame != 60);
  HeapSlot_Release(104);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

s32 BattleEffect_BuildCenteredIndexList(BattleEffectIndexWindow *window,
                                        s16 *output) {
  s32 length = window->indexRadius * 2 + 1;
  s32 entryCount = 0;
  s32 center = 0;
  s32 entryIndex;

  for (entryIndex = 0; entryIndex != length; entryIndex++) {
    if (window->currentValue == window->values[entryIndex]) {
      center = entryIndex;
      break;
    }
  }
  for (entryIndex = 0; entryIndex != length; entryIndex++) {
    s32 battleValue = center + entryIndex - window->indexRadius;

    if (battleValue >= 0 && battleValue < window->entryLimit) {
      output[entryCount++] = battleValue;
    }
  }
  return entryCount;
}
