/// @file
/// Annihilation.
// Part of ability_effects.c.

#include "../ability_effects_internal.h"

#undef HIT_FRAME
#undef SE_HIT

#define SHARD_COUNT 64
#define STREAK_PICTURE_BYTES (48 * 72)
#define BURST_PICTURE_BYTES (40 * 80)
#define BURST_STRIP_BYTES (3 * BURST_PICTURE_BYTES) // the stronger variant's

#define SHARDS_FRAMES 64
#define STREAK_FRAME 6 // the user's fire streak, six frames
#define HIT_FRAME 8
#define FLASH_FRAME 14
#define BURST_FRAME 16 // the burst on the target, until frame 47

#define PHASE_RESULTS 134
#define SE_HIT 212

extern char ResourceId_FireStreakSheet;
extern char ResourceId_FireBurstSheet;

void BattleEffect_CallDoNothing(s32 frames);

// The user lunges, a streak of fire leaves them, and the target bursts into
// flame, throwing 64 burning shards away from the user.
//
// Setup: open the canvas, load the streak, burst and shard pictures, dash
// the user four frames toward the target, and seed the shards at the target
// with random speeds (flipped to fly away from the user's side). Each frame:
// the streak's six pictures at the user from frame 6; the hit at frame 8
// (results phase, the stronger variant's cue, the canvas flooded, the target
// knocked back); the burst's three pictures (two frames each, then held) on
// the target from frame 16 to 47; from frame 4 the shards in 3D, their
// picture shrinking with their life.
void BattleEffect_RunAnnihilation(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  u8 *cells;
  struct EffectCamera *view;
  struct MotionObject *target;
  struct EffectStep *shard;
  struct EffectPosition user;
  struct EffectPosition origin;
  struct EffectPosition screen;
  DrawRectangle draw[2];
  s32 i;
  s32 size;
  u8 *burst;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  cells = Ram_HeapSlots->unk64[0];
  view = Ram_HeapSlots->cameraWork;
  burst = Ram_MapCellBuffer;

  // Open the canvas, load the pictures, and dash at the target.
  BattleEffect_StartCanvasLayer(0);
  Resource_LoadAndDecompress((s32)&ResourceId_FireStreakSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_FireBurstSheet, burst, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, cells, 0, 0);
  BattleMotion_ApproachTargetFar(effect->actor, effect->actors[0], 4, 0);
  wait(1);
  target = GetBattleObjectSlotFar(effect->actors[0])->object;

  // The shards start inside the target; each group of four lives two frames
  // longer than the one before.
  i = 0;
  do {
    struct EffectStep *seed = &work->particles[i];

    seed->x = target->x;
    seed->y = target->y;
    seed->z = target->z;
    seed->velocityX = (Random16() & 255) << 11;
    seed->velocityY = ((Random16() & 255) - 127) << 12;
    seed->velocityZ = ((Random16() & 255) - 127) << 12;
    if (seed->x > 0)
      seed->velocityX = -seed->velocityX;
    seed->variant = i / 4 * 2 + 16;
    i++;
  } while (i != SHARD_COUNT);

  EffectPosition_ProjectScaledBattler(effect->actors[0], &origin);
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  work->transferMode = 2;
  work->transferValue = 75;

  for (frame = 0; frame != SHARDS_FRAMES; frame++) {
    if (frame == HIT_FRAME)
      BattlePlayback_StartFar(PHASE_RESULTS);
    if (effect->variant != 0 && frame == HIT_FRAME)
      Sound_PlayFar(SE_HIT);

    // The streak, drawn from the user toward the target.
    EffectPosition_ProjectScaledBattler(effect->actor, &user);
    if (frame >= STREAK_FRAME && frame <= STREAK_FRAME + 5) {
      if (effect->side == 0)
        BattleEffect_LoadBlitter(
            HEAP_BLITTER,
            BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
      else
        BattleEffect_LoadBlitter(HEAP_BLITTER,
                                 BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_X |
                                                            COMPOSITE_CLIP_Y |
                                                            COMPOSITE_FLIP_X));
      draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
      if (effect->side == 0)
        draw[0](canvas,
                work->sheet + (frame - STREAK_FRAME) * STREAK_PICTURE_BYTES,
                user.x / 2 - 24, user.y - 24, 48, 72);
      else
        draw[0](canvas,
                work->sheet + (frame - STREAK_FRAME) * STREAK_PICTURE_BYTES,
                user.x / 2, user.y - 24, 48, 72);
      HeapSlot_Release(HEAP_BLITTER);
    }

    // The burst on the target; the stronger variant has its own pictures.
    if (frame >= BURST_FRAME && frame <= 47) {
      s32 step = (frame - BURST_FRAME) / 2;
      s32 strip;

      BattleEffect_LoadBlitter(
          HEAP_BLITTER, BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
      draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
      if (step > 2)
        step = 2;
      if (effect->variant == 0)
        strip = 0;
      else
        strip = BURST_STRIP_BYTES;
      draw[0](canvas, Ram_MapCellBuffer + strip + step * BURST_PICTURE_BYTES,
              origin.x / 2 - 20, origin.y - 48, 40, 80);
      BattleEffect_CallDoNothing(10000);
      HeapSlot_Release(HEAP_BLITTER);
    }
    if (frame == HIT_FRAME)
      Iwram_FillWordsEntry(canvas, 0x4000, 0x3f3f3f3f);

    // The shards, projected from the battle camera.
    Render_ResetTransformState();
    Transform_SetLookAt(view->eye, view->pos);
    if (frame > 3) {
      BattleEffect_FetchRectangleBlitters(effect->side, draw);
      for (i = 0; i != SHARD_COUNT; i++) {
        s32 life;

        shard = &work->particles[i];
        life = shard->variant;
        if (life > 0) {
          EffectPosition_ProjectPoint((s32 *)shard, &screen);
          screen.x >>= 1;
          screen.y = screen.y + origin.y - 112;
          size = (life >> 3) + 2;
          draw[(i / 2) & 1](canvas, cells + gBattleEffect_MoteCells[size - 1],
                            screen.x - size / 2, screen.y - size, size,
                            size * 2);
          BattleEffectKernels_IntegrateVector3(shard, 60, -0x400);
          shard->variant--;
        }
      }
      HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
      HeapSlot_Release(HEAP_BLITTER);
    }

    // The hit's knock-back, and the target's flashes.
    if (frame == HIT_FRAME) {
      BattleMotion_ApplyKnockbackFar(effect->actors[0], 4);
      work->shakeFrames = 4;
    }
    if (frame == STREAK_FRAME)
      BattleEffect_TintBattler(effect->actors[0], 10, -1, -1, 0);
    if (frame == FLASH_FRAME)
      BattleEffect_TintBattler(effect->actors[0], 10, -1, -1, 0);
    BattleCamera_Shake(16, 16);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}
