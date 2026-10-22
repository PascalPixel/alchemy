/// @file
/// Spire.
// Part of effect.c.

#include "../effect_internal.h"

#define SPIRE_ROCK_WIDTH 34  ///< the boulder picture, the first in the sheet
#define SPIRE_ROCK_HEIGHT 62 ///< the boulder picture's height
/// The shard pictures in the sheet, after the boulder.
#define SPIRE_SHARD_CELLS (SPIRE_ROCK_WIDTH * SPIRE_ROCK_HEIGHT)
#define SPIRE_SHARDS 21 ///< shards per rock
/// Frames a boulder falls, from its drop frame (gSpire_DropFrames) until it
/// lands, shakes the screen and strikes the targets.
#define SPIRE_FALL_FRAMES 18

/// Boulders drop along the row of targets, centred on it, and shatter into
/// tumbling shards, hitting every target as each lands.
///
/// Abilities: Spire, Clay Spire, Stone Spire (BATTLE_EFFECT_SPIRE).
///
/// The boulders are work->particles, of which only y and its speed move; their
/// shards are PARTICLES, SPIRE_SHARDS for each boulder. Each transfer clears
/// the canvas, so everything is redrawn every frame.
///
/// effect: the targets; its variant picks the number of boulders
///         (gSpire_RockCounts: 1, 3 or 5).
void BattleEffect_RunSpire(struct BattleEffectArgument *effect) {
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle blit;
  s32 rockCount;
  struct EffectPosition first;
  struct EffectPosition last;
  struct EffectStep *rock;
  struct EffectStep *shard;
  s32 frame;
  s32 i;
  s32 j;
  s32 cell;
  u32 width;
  u32 height;

  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  work->effect = effect;
  EFFECT_BEGIN_NARROW_CANVAS(1);
  REG_BLDCNT = 0;
  Resource_LoadAndDecompress((s32)&ResourceId_BoulderSheet, work->sheet, 1, 1);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_TRANSPARENT);
  blit = EFFECT_SLOT(slots, HEAP_SLOT_BLITTER);
  // BG2 is scrolled so that canvas column 64 is the middle of the row of
  // targets.
  EffectPosition_ProjectScaledBattler(work->effect->actors[0], &first);
  EffectPosition_ProjectScaledBattler(
      work->effect->actors[work->effect->count - 1], &last);
  first.x += (last.x - first.x) / 2;
  REG_BG2X = (64 - first.x) << 8;
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  // The rocks wait 64 pixels above the canvas until they drop.
  rockCount = gSpire_RockCounts[work->effect->variant];
  for (i = 0; i != rockCount; i++) {
    rock = &work->particles[i];
    rock->y = -0x400000;
    rock->velocityY = 0;
  }
  // Each rock breaks into 21 shards, thrown up from where it lands: under a
  // pixel a frame to either side and 1 to 5 pixels a frame up. z is the
  // shard's tumbling speed and variant how far it has tumbled.
  for (i = 0; i != rockCount; i++) {
    for (j = 0; j != SPIRE_SHARDS; j++) {
      struct EffectStep *shard = &PARTICLES[i * SPIRE_SHARDS + j];

      shard->x = (gSpire_ShardOffsets[j][SPIRE_SHARD_X] + gSpire_RockX[i])
                 << 16;
      shard->y = gSpire_ShardOffsets[j][SPIRE_SHARD_Y] << 16;
      shard->velocityX = ((Random16() % 96) - 48) << 10;
      shard->velocityY = -((Random16() & 127) + 32) << 11;
      shard->z = 32;
      shard->variant = 0;
    }
  }
  // The scene lasts until 80 frames after the last rock drops; the damage
  // shows 30 frames after that rock lands.
  for (frame = 0; frame != gSpire_DropFrames[rockCount - 1] + 80; frame++) {
    if (frame == gSpire_DropFrames[rockCount - 1] + 48)
      EFFECT_SHOW_DAMAGE(SE_132);
    for (i = 0; i != rockCount; i++) {
      rock = &work->particles[i];
      if (frame == gSpire_DropFrames[i] + SPIRE_FALL_FRAMES) {
        AudioCommand_PlayFar(SE_HEAVY_IMPACT);
        work->shakeFrames = 4;
      }
      // From its landing on, the rock is gone and its shards fly.
      if (frame >= gSpire_DropFrames[i] + SPIRE_FALL_FRAMES) {
        for (j = 0; j != SPIRE_SHARDS; j++) {
          shard = &PARTICLES[i * SPIRE_SHARDS + j];
          // Five shard shapes, each tumbling through three pictures; z is the
          // tumbling speed, slowing by one every other frame down to 1. The
          // shards gain 1/4 pixel a frame of downward speed each frame,
          // undamped.
          cell = (j % 5) * 3 + shard->variant / 96 % 3;
          // UB: width is assigned in one argument and read in another.
#ifdef UBFIX
          width = gSpire_ShardWidths[cell];
          height = gSpire_ShardHeights[cell];
          blit(canvas,
               work->sheet + gSpire_ShardCells[cell] + SPIRE_SHARD_CELLS,
               Q_16_16_TO_INT(shard->x) - (width >> 1),
               Q_16_16_TO_INT(shard->y) - (height >> 1), width, height);
#else
          blit(canvas,
               work->sheet + gSpire_ShardCells[cell] + SPIRE_SHARD_CELLS,
               Q_16_16_TO_INT(shard->x) -
                   ((width = gSpire_ShardWidths[cell]) >> 1),
               Q_16_16_TO_INT(shard->y) -
                   ((height = gSpire_ShardHeights[cell]) >> 1),
               width, height);
#endif
          EffectStep_AdvanceWithGravity2D(shard, 64, 0x4000);
          shard->variant += shard->z;
          if (shard->z > 1 && (frame & 1))
            shard->z--;
        }
      }
      // The rock falls from its drop frame, a pixel a frame faster each
      // frame, never lower than y 50; it is drawn centred over its column.
      if (frame < gSpire_DropFrames[i] + SPIRE_FALL_FRAMES) {
        if (frame >= gSpire_DropFrames[i])
          blit(canvas, work->sheet, gSpire_RockX[i] + 64 - SPIRE_ROCK_WIDTH / 2,
               Q_16_16_TO_INT(rock->y), SPIRE_ROCK_WIDTH, SPIRE_ROCK_HEIGHT);
        rock->y += rock->velocityY;
        if (frame > gSpire_DropFrames[i])
          rock->velocityY += 0x10000;
        if (rock->y > 0x320000)
          rock->y = 0x320000;
      }
      if (frame == gSpire_DropFrames[i] + SPIRE_FALL_FRAMES) {
        EFFECT_FOR_EACH_TARGET(j, work->effect)
          ObjectGroup_UpdateMembers(work->effect->actors[j], SPRITE_COLOR_WHITE,
                                    POSE_DAMAGE, j, 8);
      }
    }
    Camera_ApplyShake(2, 4);
    EFFECT_END_FRAME(work);
  }
  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

#undef SPIRE_ROCK_WIDTH
#undef SPIRE_ROCK_HEIGHT
#undef SPIRE_SHARD_CELLS
#undef SPIRE_SHARDS
#undef SPIRE_FALL_FRAMES
