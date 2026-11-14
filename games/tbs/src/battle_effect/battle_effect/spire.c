/// @file
/// Spire.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"

#define SPIRE_ROCK_WIDTH 34  ///< the boulder picture, the first in the sheet
#define SPIRE_ROCK_HEIGHT 62 ///< the boulder picture's height
/// The shard pictures in the sheet, after the boulder.
#define SPIRE_SHARD_PICTURES (SPIRE_ROCK_WIDTH * SPIRE_ROCK_HEIGHT)
#define SPIRE_SHARDS 21 ///< shards per rock
/// Frames a boulder falls before it lands and strikes the targets.
#define SPIRE_FALL_FRAMES 18

/// Boulders drop along the row of targets, centred on it, and shatter into
/// tumbling shards, hitting every target as each lands.
///
/// Abilities: Spire, Clay Spire, Stone Spire.
void BattleEffect_RunSpire(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle blit;
  s32 rockCount;
  struct ScreenPoint first;
  struct ScreenPoint last;
  struct Particle *rock;
  struct Particle *shard;
  s32 frame;
  s32 i;
  s32 j;
  s32 picture;
  u32 width;
  u32 height;

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  work->args = args;
  EFFECT_START_NARROW_CANVAS(BGCNT_PRIORITY(1));
  REG_BLDCNT = 0;
  Resource_LoadAndDecompress((s32)&ResourceId_BoulderSheet, work->sheet, 1, 1);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_TRANSPARENT);
  blit = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  Projection_ScaledUnit(work->args->targets[0], &first);
  Projection_ScaledUnit(work->args->targets[work->args->targetCount - 1],
                        &last);
  first.x += (last.x - first.x) / 2;
  REG_BG2X = (64 - first.x) << 8;
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  rockCount = gSpire_RockCounts[work->args->variant];
  for (i = 0; i != rockCount; i++) {
    rock = &work->particles[i];
    rock->y = -px(64);
    rock->velocityY = 0;
  }
  for (i = 0; i != rockCount; i++) {
    for (j = 0; j != SPIRE_SHARDS; j++) {
      struct Particle *shard = &PARTICLES[i * SPIRE_SHARDS + j];

      shard->x = px(gSpire_ShardOffsets[j][SPIRE_SHARD_X] + gSpire_RockX[i]);
      shard->y = px(gSpire_ShardOffsets[j][SPIRE_SHARD_Y]);
      shard->velocityX = ((Random16() % 96) - 48) << 10;
      shard->velocityY = -((Random16() & 127) + 32) << 11;
      shard->z = 32;
      shard->variant = 0;
    }
  }
  for (frame = 0; frame != gSpire_DropFrames[rockCount - 1] + 80; frame++) {
    if (frame == gSpire_DropFrames[rockCount - 1] + 48)
      EFFECT_SHOW_DAMAGE(SE_WATER);
    for (i = 0; i != rockCount; i++) {
      rock = &work->particles[i];
      if (frame == gSpire_DropFrames[i] + SPIRE_FALL_FRAMES) {
        Sound_PlayFar(SE_HEAVY_IMPACT);
        work->shakeFrames = 4;
      }
      if (frame >= gSpire_DropFrames[i] + SPIRE_FALL_FRAMES) {
        for (j = 0; j != SPIRE_SHARDS; j++) {
          shard = &PARTICLES[i * SPIRE_SHARDS + j];
          picture = (j % 5) * 3 + shard->variant / 96 % 3;
          // UB: width is assigned in one argument and read in another.
#ifdef UBFIX
          width = gSpire_ShardWidths[picture];
          height = gSpire_ShardHeights[picture];
          blit(canvas,
               work->sheet + gSpire_ShardPictures[picture] +
                   SPIRE_SHARD_PICTURES,
               Q_16_16_TO_INT(shard->x) - (width >> 1),
               Q_16_16_TO_INT(shard->y) - (height >> 1), width, height);
#else
          blit(canvas,
               work->sheet + gSpire_ShardPictures[picture] +
                   SPIRE_SHARD_PICTURES,
               Q_16_16_TO_INT(shard->x) -
                   ((width = gSpire_ShardWidths[picture]) >> 1),
               Q_16_16_TO_INT(shard->y) -
                   ((height = gSpire_ShardHeights[picture]) >> 1),
               width, height);
#endif
          Particle_AdvanceWithGravity2D(shard, 64, px(1) / 4);
          shard->variant += shard->z;
          if (shard->z > 1 && (frame & 1))
            shard->z--;
        }
      }
      // The boulder falls over its column.
      if (frame < gSpire_DropFrames[i] + SPIRE_FALL_FRAMES) {
        if (frame >= gSpire_DropFrames[i])
          blit(canvas, work->sheet, gSpire_RockX[i] + 64 - SPIRE_ROCK_WIDTH / 2,
               Q_16_16_TO_INT(rock->y), SPIRE_ROCK_WIDTH, SPIRE_ROCK_HEIGHT);
        rock->y += rock->velocityY;
        if (frame > gSpire_DropFrames[i])
          rock->velocityY += px(1);
        if (rock->y > px(50))
          rock->y = px(50);
      }
      if (frame == gSpire_DropFrames[i] + SPIRE_FALL_FRAMES) {
        EFFECT_FOR_EACH_TARGET(j, work->args)
          BattleEffect_TintBattler(work->args->targets[j], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, j, 8);
      }
    }
    BattleCamera_Shake(2, 4);
    EFFECT_END_FRAME(work);
  }
  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

#undef SPIRE_ROCK_WIDTH
#undef SPIRE_ROCK_HEIGHT
#undef SPIRE_SHARD_PICTURES
#undef SPIRE_SHARDS
#undef SPIRE_FALL_FRAMES
