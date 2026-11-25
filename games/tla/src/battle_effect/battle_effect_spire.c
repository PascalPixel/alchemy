/// @file
/// Spire.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

#define SPIRE_SHARDS 21 ///< shards per rock
/// Frames a boulder falls before it lands and strikes the targets.
#define SPIRE_FALL_FRAMES 18

/// Boulders drop along the row of targets, centred on it, and shatter into
/// tumbling shards, hitting every target as each lands.
///
/// Abilities: Spire, Clay Spire, Stone Spire.
void BattleEffect_RunSpire(struct BattleEffectParams *args) {
  struct BattleEffectState *state;
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

  state = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  EFFECT_START_NARROW_CANVAS(BGCNT_PRIORITY(1));
  REG_BLDCNT = 0;
  Resource_LoadAndDecompress(RESOURCE_BOULDER_SHEET, state->sheet, 1, 1);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      BLITTER_TRANSPARENT);
  blit = Ram_HeapSlots->ramCode;

  // The canvas is centred between the first and the last target.
  EffectPosition_ProjectScaledBattler(args->targets[0], &first);
  EffectPosition_ProjectScaledBattler(args->targets[args->targetCount - 1],
                                      &last);
  first.x += (last.x - first.x) / 2;
  REG_BG2X = (CANVAS_SIZE / 2 - first.x) << 8;
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);

  // Each boulder waits 64 pixels above the canvas; each shard sits at its
  // place in its boulder, ready to fly up at random.
  rockCount = gSpire_RockCounts[args->variant];
  for (i = 0; i != rockCount; i++) {
    rock = &state->particles[i];
    rock->y = -toFIXED(64);
    rock->velocityY = 0;
  }
  for (i = 0; i != rockCount; i++) {
    for (j = 0; j != SPIRE_SHARDS; j++) {
      struct Particle *shard = &PARTICLES[i * SPIRE_SHARDS + j];

      shard->x = INTtoFIXED(gSpire_ShardOffsets[j * 2] + gSpire_RockX[i]);
      shard->y = INTtoFIXED(gSpire_ShardOffsets[j * 2 + 1]);
      shard->velocityX = ((Random16() % 96) - 48) << 10;
      shard->velocityY = -((Random16() & 127) + 32) << 11;
      shard->z = 32;
      shard->variant = 0;
    }
  }

  for (frame = 0; frame != gSpire_DropFrames[rockCount - 1] + 65; frame++) {
    // DEAD: the first boulder's crash has already let the battle show the
    // damage, so this second use changes nothing; the ROM keeps the test and
    // the call.
    if (frame == gSpire_DropFrames[rockCount - 1] + 48)
      EFFECT_SHOW_DAMAGE(SE_IMPACT);
    for (i = 0; i != rockCount; i++) {
      rock = &state->particles[i];
      // Each boulder crashes and shakes the screen; the first lets the battle
      // show the damage.
      if (frame == gSpire_DropFrames[i] + SPIRE_FALL_FRAMES) {
        if (i == 0)
          EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
        else
          Sound_PlayFar(SE_HEAVY_IMPACT);
        state->shakeFrames = 4;
      }
      // Its shards fly, each turning through its kind's three pictures, more
      // slowly every other frame.
      if (frame >= gSpire_DropFrames[i] + SPIRE_FALL_FRAMES) {
        for (j = 0; j != SPIRE_SHARDS; j++) {
          shard = &PARTICLES[i * SPIRE_SHARDS + j];
          picture = (j % 5) * 3 + shard->variant / SHARD_TURN % 3;
          // UB: width is assigned in one argument and read in another.
#ifdef UBFIX
          width = gSpire_ShardWidths[picture];
          height = gSpire_ShardHeights[picture];
          blit(canvas,
               state->sheet + gSpire_ShardPictures[picture] +
                   BOULDER_SHARD_PICTURES,
               FIXED_INTEGER_PART(shard->x) - (width >> 1),
               FIXED_INTEGER_PART(shard->y) - (height >> 1), width, height);
#else
          blit(canvas,
               state->sheet + gSpire_ShardPictures[picture] +
                   BOULDER_SHARD_PICTURES,
               FIXED_INTEGER_PART(shard->x) -
                   ((width = gSpire_ShardWidths[picture]) >> 1),
               FIXED_INTEGER_PART(shard->y) -
                   ((height = gSpire_ShardHeights[picture]) >> 1),
               width, height);
#endif
          Particle_AdvanceWithGravity2D(shard, PARTICLE_DAMPING_NONE,
                                        toFIXED(1) / 4);
          shard->variant += shard->z;
          if (shard->z > 1 && (frame & 1))
            shard->z--;
        }
      }
      // Until then the boulder falls, a pixel a frame faster each frame,
      // and rests at y 50.
      if (frame < gSpire_DropFrames[i] + SPIRE_FALL_FRAMES) {
        if (frame >= gSpire_DropFrames[i])
          blit(canvas, state->sheet,
               gSpire_RockX[i] + CANVAS_SIZE / 2 - BOULDER_WIDTH / 2,
               FIXED_INTEGER_PART(rock->y), BOULDER_WIDTH, BOULDER_HEIGHT);
        rock->y += rock->velocityY;
        if (frame > gSpire_DropFrames[i])
          rock->velocityY += toFIXED(1);
        if (rock->y > toFIXED(50))
          rock->y = toFIXED(50);
      }
      if (frame == gSpire_DropFrames[i] + SPIRE_FALL_FRAMES) {
        EFFECT_FOR_EACH_TARGET(j, args)
          BattleEffect_TintBattler(args->targets[j], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, j, 8);
      }
    }
    BattleCamera_Shake(2, 4);
    EFFECT_END_FRAME(state);
  }

  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

#undef SPIRE_SHARDS
#undef SPIRE_FALL_FRAMES
