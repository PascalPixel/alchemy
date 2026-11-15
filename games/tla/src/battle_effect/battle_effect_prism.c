/// @file
/// Prism.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"
#include "gamepak/far_call/battle.h"
#include "resource_ids.h"

#undef GROUND_Y

#define SHARDS_PER_CRYSTAL 16
#define GROUND_Y px(56)
#define SHATTERED 1 ///< a crystal's z once it has hit the ground

/// Crystals drop out of the sky onto the targets' side and shatter on the
/// ground into spinning shards, flashing the targets and shaking the screen.
///
/// Abilities: Prism, Hail Prism, Freeze Prism.
void BattleEffect_RunPrism(struct BattleEffectParams *args) {
  DrawRectangle draw[2];
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  struct Particle *seed;
  struct Particle *cry;
  struct Particle *shards;
  struct Particle *shard;
  s32 x;
  s32 cell;
  s32 i;
  s32 j;
  s32 k;

  // Set up: the canvas layer, the two sheets and the blitter pair.
  shards = (struct Particle *)Ram_MapCellBuffer;
  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  Resource_LoadAndDecompress((s32)&ResourceId_BoulderSheet, work->sheet, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_CrystalSheet, work->sheet, 1, 1);
  BattleEffect_FetchRectangleBlitters(args->side, draw);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

  // The crystals wait above the screen, each eight frames behind the last,
  // and drift sideways so that they land over the targets.
  for (k = 0; k != gPrism_Counts[args->variant * 3]; k++) {
    seed = &work->particles[k];
    x = Random16() & 31;
    seed->y = -px(64);
    if (args->side == EFFECT_SIDE_PARTY) {
      x = px((Random16() & 31) + 80);
      seed->velocityX = (Random16() & 63) << 12;
    } else {
      x = px((Random16() & 31) + 8);
      seed->velocityX = -(Random16() & 63) << 12;
    }
    seed->x = x - seed->velocityX * 18;
    seed->velocityY = 0;
    seed->z = 0;
    seed->variant = k * 8;
  }

  for (frame = 0; frame != gPrism_Counts[args->variant * 3 + 1]; frame++) {
    // The strongest variant turns the camera, slowing from frame 96.
    if (args->variant == EFFECT_VARIANT_STRONGEST && frame <= 103) {
      struct BattleCamera *camera = Ram_HeapSlots->cameraWork;
      s32 speed;

      if (frame <= 95)
        speed = 192;
      else
        speed = 2496 - frame * 24;
      if (args->side == EFFECT_SIDE_ENEMIES)
        camera->yaw -= speed;
      else
        camera->yaw += speed;
    }
    if (frame == gPrism_Counts[args->variant * 3 + 2])
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);

    // Darken over the last eight frames.
    if (frame == gPrism_Counts[args->variant * 3 + 1] - 8) {
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(6));
    }

    if (frame <= gPrism_Counts[args->variant * 3 + 1] - 8) {
      for (i = 0; i != gPrism_Counts[args->variant * 3]; i++) {
        cry = &work->particles[i];
        if (cry->z == SHATTERED) {
          // A shattered crystal: its shards fly and spin, slowing down.
          for (j = 0; j != SHARDS_PER_CRYSTAL; j++) {
            cell = (j % 5) * 3 +
                   (shards[i * SHARDS_PER_CRYSTAL + j].variant / 96) % 3;
            shard = &shards[i * SHARDS_PER_CRYSTAL + j];
            draw[j <= 2](
                canvas, work->sheet + gPrism_ShardPictures[cell] + 0x800,
                Q_16_16_UPPER_HALF(shard->x) - gPrism_ShardWidths[cell] / 2,
                Q_16_16_UPPER_HALF(shard->y) - gPrism_ShardHeights[cell] / 2,
                gPrism_ShardWidths[cell], gPrism_ShardHeights[cell]);
            BattleEffectKernels_IntegrateVector2(shard, PARTICLE_DAMPING_NONE,
                                                 px(1) / 8);
            shard->variant += shard->z;
            if (shard->z > 1 && (frame & 1))
              shard->z--;
          }
        } else if (frame >= cry->variant) {
          // A falling crystal (32x64), until it reaches the ground.
          draw[i & 1](canvas, work->sheet, Q_16_16_UPPER_HALF(cry->x) - 16,
                      Q_16_16_UPPER_HALF(cry->y), CRYSTAL_WIDTH,
                      CRYSTAL_HEIGHT);
          BattleEffectKernels_IntegrateVector2(cry, PARTICLE_DAMPING_NONE,
                                               px(1));
          if (cry->y > GROUND_Y) {
            cry->z = SHATTERED;
            cry->y = GROUND_Y;
            for (j = 0; j != SHARDS_PER_CRYSTAL; j++) {
              shard = &shards[i * SHARDS_PER_CRYSTAL + j];
              shard->x = px(gPrism_ShardStarts[j * 2] - 40) + cry->x;
              shard->y = px(gPrism_ShardStarts[j * 2 + 1]);
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
            for (j = 0; j != args->targetCount; j++)
              BattleEffect_TintBattler(args->targets[j], SPRITE_COLOR_WHITE,
                                       POSE_DAMAGE, j, 4);
          }
        }
      }
    }

    BattleCamera_Shake(args->variant * 2 + 4, args->variant * 4 + 8);
    EFFECT_END_FRAME(work);
  }

  // Tear down.
  EFFECT_FINISH_CANVAS();
}

#undef SPARK_COUNT
#undef GROUND_Y

/// Rebuilds the background palette's first ramp as a grey whose channels sway
/// with the frame, so the effect shimmers through colours.
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
    if (red > RGB_LEVEL_MAX)
      red = RGB_LEVEL_MAX;
    if (green < 0)
      green = 0;
    if (green > RGB_LEVEL_MAX)
      green = RGB_LEVEL_MAX;
    if (blue < 0)
      blue = 0;
    if (blue > RGB_LEVEL_MAX)
      blue = RGB_LEVEL_MAX;

    palette[index] = RGB(red, green, blue);
    index++;
  } while (index != 64);

  Iwram_CopyWords((void *)&BG_PLTT_COLOR(0, 1), palette, sizeof(palette));
}
