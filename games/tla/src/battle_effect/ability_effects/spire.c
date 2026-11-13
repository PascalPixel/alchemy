/// @file
/// Spire, Gust and Bear Claw.
// Part of ability_effects.c.

#include "../ability_effects_internal.h"

#undef BLITTER

#define BLITTER(mode, flags) ((mode) << 4 | (flags))

// The shards live in the map cell buffer, 21 for each boulder.
#define SHARDS ((struct EffectStep *)Ram_MapCellBuffer)
#define SHARDS_PER_ROCK 21
// A boulder shatters this many frames after it starts to fall.
#define FALL_FRAMES 18

extern char ResourceId_BoulderSheet;
// ☀️'s gSpire_RockCounts, by variant.
extern u8 Data_081985b0[];
// ☀️'s gSpire_ShardOffsets: x, y in the boulder for each shard.
extern u8 Data_08198566[];
// ☀️'s gSpire_RockX and gSpire_DropFrames, by boulder.
extern s8 Data_081985a6[];
extern u8 Data_081985ab[];
// The shard cels: ☀️'s gSpire_ShardCells, _ShardWidths and
// _ShardHeights.
extern u16 gData_081974bc[];
extern u8 gData_0819749e[];
extern u8 gData_081974ad[];

void BattleEffect_RunSpire(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle blit;
  s32 rocks;
  struct EffectPosition first;
  struct EffectPosition final;
  struct EffectStep *rock;
  struct EffectStep *shard;
  s32 frame;
  s32 i;
  s32 j;
  s32 cel;
  u32 width;
  u32 height;

  // Setup: the canvas on BG2, the sheet, a transparent blitter.
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  BattleEffect_StartCanvasLayer(1);
  REG_BG2PA = 0x100;
  REG_BLDCNT = 0;
  Resource_LoadAndDecompress((s32)&ResourceId_BoulderSheet, work->sheet, 1, 1);
  BattleEffect_LoadBlitter(
      HEAP_BLITTER,
      BLITTER(BLIT_TRANSPARENT, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  blit = (DrawRectangle)Ram_HeapSlots->unk64[1];

  // The canvas is centred between the first and the last target.
  EffectPosition_ProjectScaledBattler(effect->actors[0], &first);
  EffectPosition_ProjectScaledBattler(effect->actors[effect->count - 1],
                                      &final);
  first.x += (final.x - first.x) / 2;
  REG_BG2X = (64 - first.x) << 8;
  work->transferMode = 1;
  work->transferValue = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  // Each boulder waits 64 pixels above the canvas; each shard sits at its
  // place in its boulder, ready to fly up at random.
  rocks = Data_081985b0[effect->variant];
  for (i = 0; i != rocks; i++) {
    rock = &work->particles[i];
    rock->y = -0x400000;
    rock->velocityY = 0;
  }
  for (i = 0; i != rocks; i++) {
    for (j = 0; j != SHARDS_PER_ROCK; j++) {
      struct EffectStep *seed = &SHARDS[i * SHARDS_PER_ROCK + j];

      seed->x = (Data_08198566[j * 2] + Data_081985a6[i]) << 16;
      seed->y = Data_08198566[j * 2 + 1] << 16;
      seed->velocityX = ((Random16() % 96) - 48) << 10;
      seed->velocityY = -((Random16() & 127) + 32) << 11;
      seed->z = 32;
      seed->variant = 0;
    }
  }

  for (frame = 0; frame != Data_081985ab[rocks - 1] + 65; frame++) {
    // 48 frames after the last drop the battle resumes, hit cue 133.
    if (frame == Data_081985ab[rocks - 1] + 48)
      BattlePlayback_StartFar(133);
    for (i = 0; i != rocks; i++) {
      rock = &work->particles[i];
      // The crash: the first one resumes the battle, the others sound.
      if (frame == Data_081985ab[i] + FALL_FRAMES) {
        if (i == 0)
          BattlePlayback_StartFar(134);
        else
          Sound_PlayFar(134);
        work->shakeFrames = 4;
      }
      // The shards fly, each tumbling through its kind's three cels more
      // slowly every other frame.
      if (frame >= Data_081985ab[i] + FALL_FRAMES) {
        for (j = 0; j != SHARDS_PER_ROCK; j++) {
          shard = &SHARDS[i * SHARDS_PER_ROCK + j];
          cel = (j % 5) * 3 + shard->variant / 96 % 3;
          blit(canvas, work->sheet + gData_081974bc[cel] + 0x83c,
               HI(shard->x) - ((width = gData_0819749e[cel]) >> 1),
               HI(shard->y) - ((height = gData_081974ad[cel]) >> 1), width,
               height);
          BattleEffectKernels_IntegrateVector2(shard, 64, 0x4000);
          shard->variant += shard->z;
          if (shard->z > 1 && (frame & 1))
            shard->z--;
        }
      }
      // Until then the boulder falls, a pixel a frame faster each frame,
      // and rests at y 50.
      if (frame < Data_081985ab[i] + FALL_FRAMES) {
        if (frame >= Data_081985ab[i])
          blit(canvas, work->sheet, Data_081985a6[i] + 47, HI(rock->y), 34, 62);
        rock->y += rock->velocityY;
        if (frame > Data_081985ab[i])
          rock->velocityY += 0x10000;
        if (rock->y > 0x320000)
          rock->y = 0x320000;
      }
      // Every target flashes for eight frames at each crash.
      if (frame == Data_081985ab[i] + FALL_FRAMES) {
        for (j = 0; j != effect->count; j++)
          Func_0814cd48(effect->actors[j], 7, 5, j, 8);
      }
    }
    BattleCamera_Shake(2, 4);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  // Teardown.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}

// Effects: burst effect entry points.

/// Each entry passes a burst type to BattleEffect_RunBurstScene.
/// step: effect arguments.
void BattleEffect_RunGust(struct EffectStep *step) {
  if (step->variant == 0) {
    BattleEffect_RunBurstScene(step, 6);
    return;
  }
  if (step->variant == 1) {
    BattleEffect_RunBurstScene(step, 7);
    return;
  }
  BattleEffect_RunBurstScene(step, 8);
}

void BattleEffect_RunBearClaw(struct EffectStep *step) {
  BattleEffect_RunBurstScene(step, 0);
}
