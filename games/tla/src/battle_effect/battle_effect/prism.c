/// @file
/// Prism.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"
#include "sounds.h"
#include "module/battle_effect/battle_effect.h"
#include "gamepak/far_call/battle.h"
#include "module/battle_effect/battle_effect.h"
#include "resource_ids.h"

#undef GROUND_Y

#define SHARDS_PER_CRYSTAL 16
#define GROUND_Y (56 << 16)
#define SHATTERED 1 // a crystal's z once it has hit the ground

// Crystals fall one after another, eight frames apart, onto the targets'
// ground and shatter into sixteen spinning shards each; the screen darkens
// over the last eight frames.
void BattleEffect_RunPrism(struct BattleEffectArgument *effect) {
  DrawRectangle draw[2];
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  struct EffectStep *seed;
  struct EffectStep *cry;
  struct EffectStep *shards;
  struct EffectStep *shard;
  s32 x;
  s32 cell;
  s32 i;
  s32 j;
  s32 k;

  // Set up: the canvas layer, the two sheets and the blitter pair.
  shards = (struct EffectStep *)Ram_MapCellBuffer;
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  Resource_LoadAndDecompress((s32)&ResourceId_BoulderSheet, work->sheet, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_CrystalSheet, work->sheet, 1, 1);
  BattleEffect_FetchRectangleBlitters(effect->side, draw);
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  // The crystals wait above the screen, each eight frames behind the last,
  // and drift sideways so that they land over the targets.
  for (k = 0; k != Data_081979b7[effect->variant * 3]; k++) {
    seed = &work->particles[k];
    x = Random16() & 31;
    seed->y = -px(64);
    if (effect->side == 1) {
      x = ((Random16() & 31) + 80) << 16;
      seed->velocityX = (Random16() & 63) << 12;
    } else {
      x = ((Random16() & 31) + 8) << 16;
      seed->velocityX = -(Random16() & 63) << 12;
    }
    seed->x = x - seed->velocityX * 18;
    seed->velocityY = 0;
    seed->z = 0;
    seed->variant = k * 8;
  }

  for (frame = 0; frame != Data_081979b7[effect->variant * 3 + 1]; frame++) {
    // The strongest variant turns the camera, slowing from frame 96.
    if (effect->variant == 2 && frame <= 103) {
      struct BattleCamera *camera = Ram_HeapSlots->cameraWork;
      s32 speed;

      if (frame <= 95)
        speed = 192;
      else
        speed = 2496 - frame * 24;
      if (effect->side == 0)
        camera->yaw -= speed;
      else
        camera->yaw += speed;
    }
    if (frame == Data_081979b7[effect->variant * 3 + 2])
      BattlePlayback_StartFar(134);

    // Darken over the last eight frames.
    if (frame == Data_081979b7[effect->variant * 3 + 1] - 8) {
      work->transferMode = 3;
      work->transferValue = 0x06060606;
    }

    if (frame <= Data_081979b7[effect->variant * 3 + 1] - 8) {
      for (i = 0; i != Data_081979b7[effect->variant * 3]; i++) {
        cry = &work->particles[i];
        if (cry->z == SHATTERED) {
          // A shattered crystal: its shards fly and spin, slowing down.
          for (j = 0; j != SHARDS_PER_CRYSTAL; j++) {
            cell = (j % 5) * 3 +
                   (shards[i * SHARDS_PER_CRYSTAL + j].variant / 96) % 3;
            shard = &shards[i * SHARDS_PER_CRYSTAL + j];
            draw[j <= 2](canvas, work->sheet + gData_08197834[cell] + 0x800,
                         HI(shard->x) - gPrism_ShardWidths[cell] / 2,
                         HI(shard->y) - gData_08197826[cell] / 2,
                         gPrism_ShardWidths[cell], gData_08197826[cell]);
            BattleEffectKernels_IntegrateVector2(shard, 64, 0x2000);
            shard->variant += shard->z;
            if (shard->z > 1 && (frame & 1))
              shard->z--;
          }
        } else if (frame >= cry->variant) {
          // A falling crystal (32x64), until it reaches the ground.
          draw[i & 1](canvas, work->sheet, HI(cry->x) - 16, HI(cry->y), 32, 64);
          BattleEffectKernels_IntegrateVector2(cry, 64, 0x10000);
          if (cry->y > GROUND_Y) {
            cry->z = SHATTERED;
            cry->y = GROUND_Y;
            for (j = 0; j != SHARDS_PER_CRYSTAL; j++) {
              shard = &shards[i * SHARDS_PER_CRYSTAL + j];
              shard->x = ((gPrism_ShardStarts[j * 2] - 40) << 16) + cry->x;
              shard->y = gPrism_ShardStarts[j * 2 + 1] << 16;
              shard->velocityX = ((Random16() & 127) - 64) << 11;
              shard->velocityY = -(Random16() & 127) << 11;
              if (i & 1) {
                shard->velocityX *= 2;
                shard->velocityY *= 2;
              }
              shard->z = 32;
              shard->variant = 0;
            }
            work->shakeFrames = 8;
            Sound_PlayFar(SE_CLATTER);
            for (j = 0; j != effect->count; j++)
              BattleEffect_TintBattler(effect->actors[j], 7, 5, j, 4);
          }
        }
      }
    }

    BattleCamera_Shake(effect->variant * 2 + 4, effect->variant * 4 + 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  // Tear down.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}

#undef SPARK_COUNT
#undef GROUND_Y

void BattleEffect_UpdatePhasePalette(s32 frame, s32 redPhase, s32 greenPhase,
                                     s32 bluePhase) {
  u16 palette[64];
  s32 phase;
  s32 redOffset;
  s32 greenOffset;
  s32 blueOffset;
  s32 index;

  phase = frame * 0x400;
  redOffset = (Trig_Sin(phase + redPhase) * 16) >> 15;
  greenOffset = (Trig_Sin(phase + greenPhase) * 16) >> 15;
  blueOffset = (Trig_Sin(phase + bluePhase) * 16) >> 15;
  palette[0] = 0;

  index = 1;
  do {
    s32 red;
    s32 green;
    s32 blue;

    red = (redOffset + 16) * index / 64;
    green = (greenOffset + 16) * index / 64;
    blue = (blueOffset + 16) * index / 64;

    if (red < 0)
      red = 0;
    if (red > 31)
      red = 31;
    if (green < 0)
      green = 0;
    if (green > 31)
      green = 31;
    if (blue < 0)
      blue = 0;
    if (blue > 31)
      blue = 31;

    palette[index] = (blue << 10) | (green << 5) | red;
    index++;
  } while (index != 64);

  Iwram_CopyWords((void *)(BG_PLTT + 1), palette, sizeof(palette));
}
