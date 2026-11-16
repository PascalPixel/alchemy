/// @file
/// Spire, Gust and Bear Claw.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

#undef BLITTER

/// The shards live in the map cell buffer, 21 for each boulder.
#define SHARDS ((struct Particle *)Ram_MapCellBuffer)
#define SHARDS_PER_ROCK 21
/// A boulder shatters this many frames after it starts to fall.
#define FALL_FRAMES 18

#define SPIRE_ROCK_WIDTH 34  ///< the boulder picture, the first in the sheet
#define SPIRE_ROCK_HEIGHT 62 ///< the boulder picture's height

/// Boulders drop along the row of targets, centred on it, and shatter into
/// tumbling shards, hitting every target as each lands.
///
/// Abilities: Spire, Clay Spire, Stone Spire.
void BattleEffect_RunSpire(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle blit;
  s32 rocks;
  struct ScreenPoint first;
  struct ScreenPoint final;
  struct Particle *rock;
  struct Particle *shard;
  s32 frame;
  s32 i;
  s32 j;
  s32 cel;
  u32 width;
  u32 height;

  // Setup: the canvas on BG2, the sheet, a transparent blitter.
  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  EFFECT_START_NARROW_CANVAS(BGCNT_PRIORITY(1));
  REG_BLDCNT = 0;
  Resource_LoadAndDecompress(RESOURCE_BOULDER_SHEET, work->sheet, 1, 1);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      BLITTER_TRANSPARENT);
  blit = (DrawRectangle)Ram_HeapSlots->ramCode;

  // The canvas is centred between the first and the last target.
  EffectPosition_ProjectScaledBattler(args->targets[0], &first);
  EffectPosition_ProjectScaledBattler(args->targets[args->targetCount - 1],
                                      &final);
  first.x += (final.x - first.x) / 2;
  REG_BG2X = (64 - first.x) << 8;
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);

  // Each boulder waits 64 pixels above the canvas; each shard sits at its
  // place in its boulder, ready to fly up at random.
  rocks = gSpire_RockCounts[args->variant];
  for (i = 0; i != rocks; i++) {
    rock = &work->particles[i];
    rock->y = -toFIXED(64);
    rock->velocityY = 0;
  }
  for (i = 0; i != rocks; i++) {
    for (j = 0; j != SHARDS_PER_ROCK; j++) {
      struct Particle *seed = &SHARDS[i * SHARDS_PER_ROCK + j];

      seed->x = INTtoFIXED(gSpire_ShardOffsets[j * 2] + gSpire_RockX[i]);
      seed->y = INTtoFIXED(gSpire_ShardOffsets[j * 2 + 1]);
      seed->velocityX = ((Random16() % 96) - 48) << 10;
      seed->velocityY = -((Random16() & 127) + 32) << 11;
      seed->z = 32;
      seed->variant = 0;
    }
  }

  for (frame = 0; frame != gSpire_DropFrames[rocks - 1] + 65; frame++) {
    // 48 frames after the last drop the battle resumes, hit cue 133.
    if (frame == gSpire_DropFrames[rocks - 1] + 48)
      EFFECT_SHOW_DAMAGE(SE_IMPACT);
    for (i = 0; i != rocks; i++) {
      rock = &work->particles[i];
      // The crash: the first one resumes the battle, the others sound.
      if (frame == gSpire_DropFrames[i] + FALL_FRAMES) {
        if (i == 0)
          EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
        else
          Sound_PlayFar(SE_HEAVY_IMPACT);
        work->shakeFrames = 4;
      }
      // The shards fly, each tumbling through its kind's three cels more
      // slowly every other frame.
      if (frame >= gSpire_DropFrames[i] + FALL_FRAMES) {
        for (j = 0; j != SHARDS_PER_ROCK; j++) {
          shard = &SHARDS[i * SHARDS_PER_ROCK + j];
          cel = (j % 5) * 3 + shard->variant / 96 % 3;
          blit(canvas, work->sheet + gSpire_ShardPictures[cel] + 0x83c,
               FIXED_INTEGER_PART(shard->x) -
                   ((width = gSpire_ShardWidths[cel]) >> 1),
               FIXED_INTEGER_PART(shard->y) -
                   ((height = gSpire_ShardHeights[cel]) >> 1),
               width, height);
          BattleEffectKernels_IntegrateVector2(shard, PARTICLE_DAMPING_NONE,
                                               toFIXED(1) / 4);
          shard->variant += shard->z;
          if (shard->z > 1 && (frame & 1))
            shard->z--;
        }
      }
      // Until then the boulder falls, a pixel a frame faster each frame,
      // and rests at y 50.
      if (frame < gSpire_DropFrames[i] + FALL_FRAMES) {
        if (frame >= gSpire_DropFrames[i])
          blit(canvas, work->sheet, gSpire_RockX[i] + 47,
               FIXED_INTEGER_PART(rock->y), SPIRE_ROCK_WIDTH,
               SPIRE_ROCK_HEIGHT);
        rock->y += rock->velocityY;
        if (frame > gSpire_DropFrames[i])
          rock->velocityY += toFIXED(1);
        if (rock->y > toFIXED(50))
          rock->y = toFIXED(50);
      }
      // Every target flashes for eight frames at each crash.
      if (frame == gSpire_DropFrames[i] + FALL_FRAMES) {
        for (j = 0; j != args->targetCount; j++)
          BattleEffect_TintBattler(args->targets[j], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, j, 8);
      }
    }
    BattleCamera_Shake(2, 4);
    EFFECT_END_FRAME(work);
  }

  // Teardown.
  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}
