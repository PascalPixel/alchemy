/// @file
/// Prism.
// Part of battle_effect.c.

#include "battle_effect_internal.h"

#define PRISM_GROUND_Y px(56)          ///< y 56, where the crystals shatter
#define PRISM_SHARD_PICTURES (32 * 64) ///< the shards, after the 32x64 crystal

/// Crystals drop out of the sky onto the targets' side and shatter on the
/// ground into spinning shards, flashing the targets and shaking the screen.
///
/// Abilities: Prism, Hail Prism, Freeze Prism.
void BattleEffect_RunPrism(struct BattleEffectParams *args) {
  BlitterPair draw;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  struct Particle *seed;
  struct Particle *crystal;
  struct Particle *shards;
  struct Particle *shard;
  s32 x;
  s32 picture;
  s32 i;
  s32 j;

  shards = PARTICLES;
  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  work->args = args;
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  Resource_LoadAndDecompress(RESOURCE_CRYSTAL_SHEET, work->sheet, 1, 1);
  BattleEffect_FetchRectangleBlitters(work->args->side, draw);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

  for (j = 0; j != gPrism_Counts[work->args->variant][PRISM_CRYSTAL_COUNT];
       j++) {
    seed = &work->particles[j];
    // DEAD: x keeps a first random number, drawn and never read; the ROM
    // keeps the mask and the store.
    x = Random16() & 31;
    seed->y = -px(64);
    if (work->args->side == EFFECT_SIDE_PARTY) {
      x = ((Random16() & 31) + 80) << 16;
      seed->velocityX = (Random16() & 63) << 12;
    } else {
      x = ((Random16() & 31) + 8) << 16;
      seed->velocityX = -(Random16() & 63) << 12;
    }
    seed->x = x - seed->velocityX * 18;
    seed->velocityY = 0;
    seed->z = 0;
    seed->variant = j * 8;
  }

  for (frame = 0; frame != gPrism_Counts[work->args->variant][PRISM_FRAMES];
       frame++) {
    if (work->args->variant == EFFECT_VARIANT_STRONGEST && frame <= 103) {
      struct BattleCamera *camera = gBattleCamera;
      s32 speed;

      if (frame <= 95)
        speed = 192;
      else
        speed = 2496 - frame * 24;
      if (work->args->side == EFFECT_SIDE_ENEMIES)
        camera->yaw -= speed;
      else
        camera->yaw += speed;
    }
    if (frame == gPrism_Counts[work->args->variant][PRISM_FRAMES] - 80)
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
    if (frame == gPrism_Counts[work->args->variant][PRISM_FRAMES] - 8) {
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(6));
    }
    if (frame <= gPrism_Counts[work->args->variant][PRISM_FRAMES] - 8) {
      for (i = 0; i != gPrism_Counts[work->args->variant][PRISM_CRYSTAL_COUNT];
           i++) {
        crystal = &work->particles[i];
        if (crystal->z == 1) {
          // A shattered crystal's shards fly and spin; kind 4 asks for pictures
          // past the twelve the tables hold, whose heights read as zero, so
          // those three shards of every crystal never show.
          // XXX (no evidence): whether a fifth kind of shard was meant; the
          // twelve pictures and their tables are all the ROM holds.
          for (j = 0; j != 16; j++) {
            picture = (j % 5) * 3 + (shards[i * 16 + j].variant / 96) % 3;
            shard = &shards[i * 16 + j];
            draw[j <= 2](
                canvas,
                work->sheet + gPrism_ShardPictures[picture] +
                    PRISM_SHARD_PICTURES,
                Q_16_16_TO_INT(shard->x) - gPrism_ShardWidths[picture] / 2,
                Q_16_16_TO_INT(shard->y) - gPrism_ShardHeights[picture] / 2,
                gPrism_ShardWidths[picture], gPrism_ShardHeights[picture]);
            Particle_AdvanceWithGravity2D(shard, 64, px(1) / 8);
            shard->variant += shard->z;
            if (shard->z > 1 && (frame & 1))
              shard->z--;
          }
        } else if (frame >= crystal->variant) {
          // A falling crystal.
          draw[i & 1](canvas, work->sheet, Q_16_16_TO_INT(crystal->x) - 16,
                      Q_16_16_TO_INT(crystal->y), 32, 64);
          Particle_AdvanceWithGravity2D(crystal, 64, Q16_ONE);
          if (crystal->y > PRISM_GROUND_Y) {
            // Shatter it on reaching the ground (y 56).
            crystal->z = 1;
            crystal->y = PRISM_GROUND_Y;
            for (j = 0; j != 16; j++) {
              shard = &shards[i * 16 + j];
              shard->x = px(gPrism_ShardStarts[j][PRISM_SHARD_START_X] - 40) +
                         crystal->x;
              shard->y = px(gPrism_ShardStarts[j][PRISM_SHARD_START_Y]);
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
            EFFECT_FOR_EACH_TARGET(j, work->args)
              BattleEffect_TintBattler(work->args->targets[j],
                                       SPRITE_COLOR_WHITE, POSE_DAMAGE, j, 4);
          }
        }
      }
    }
    BattleCamera_Shake(work->args->variant * 2 + 4,
                       work->args->variant * 4 + 8);
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS();
}

// The 64 entries, black first, are copied to colours 1 to 64, so level n shows
// entry n - 1, half a level darker.
// XXX (no evidence): whether the shift by one was meant; on screen it is
// within one shade, so neither the ROM nor a render tells the two apart.
/// Rebuilds the background palette's first ramp as a grey whose channels sway
/// with the frame, so the effect shimmers through colours.
void BattleEffect_UpdatePhasePalette(s32 frame, s32 redPhase, s32 greenPhase,
                                     s32 bluePhase) {
  u16 palette[64];
  u16 *output;
  s32 phase;
  s32 redOffset;
  s32 greenOffset;
  s32 blueOffset;
  s32 index;

  phase = frame * (TRIG_TURN / 64);
  redOffset = (Trig_Sin(phase + redPhase) * 16) >> 15;
  greenOffset = (Trig_Sin(phase + greenPhase) * 16) >> 15;
  blueOffset = (Trig_Sin(phase + bluePhase) * 16) >> 15;

  palette[0] = 0;
  index = 1;
  output = &palette[1];
  do {
    s32 red;
    s32 green;
    s32 blue;

    red = (index + redOffset) / 2;
    green = (index + greenOffset) / 2;
    blue = (index + blueOffset) / 2;

    CLAMP(red, 0, 31);
    CLAMP(green, 0, 31);
    CLAMP(blue, 0, 31);

    *output++ = RGB(red, green, blue);
    index++;
  } while (index != 64);

  Iwram_CopyWords((void *)&BG_PLTT[1], palette, sizeof(palette));
}

#undef PRISM_GROUND_Y
#undef PRISM_SHARD_PICTURES
