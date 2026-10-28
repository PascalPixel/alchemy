// 2026-10-25, with the Makefile's options (-mthumb-slow-loads): differ 10 of
// 427; a greedy search over an empty asm after, or a one-pass block around,
// each statement found nothing lower.
// Draft, differ 26 of 427: BattleEffect_RunAnnihilation, effect kind 55, The Broken Seal's
// BattleEffect_RunAnnihilation.

// Twin: ☀️ BattleEffect_RunAnnihilation (games/tbs/src/effect/effect.c),
// ported: the effect stays an argument, the camera comes from its heap slot
// (0x30), the blitters from BattleEffect_LoadBlitter(104, 3, 7 or 19) and slot 0x68, the
// canvas filled through IwramFillWords where ☀️ uses Dma_Fill32, callback
// BattleEffect_ProcessPendingGraphicsTransfer at 0xc80.

// frame declared before the sheet and camera gives the native's spill slots
// (62 to 26).

// Left: everything is reordered rows where the native puts a load or a store
// ahead of an independent constant or copy (the pooled map cell address and
// 0xff after the heap reads, the work's reload before 0x7780 and before the
// blitter's frame, the particle's x before the third random's mask, the
// second size store before the halved size), the same kind as
// Func_08164b2c's, where a load costing 3 and a store 2 give the native order
// (not worked through here).

// Tried (differ): alchemy permute 6 minutes, 2 jobs, on this draft: 24 with
// size * 2 passed through an inline helper (a wrapper device that closes 2 of
// the 26 rows, not kept); the best weighted candidate moved a statement and
// differs 30.
#include "types.h"
#include "ram_buffer.h"
#include "system.h"
#include "iwram_call.h"
#include "task.h"
#include "graphics.h"
#include "transform.h"
#include "motobj.h"
#include "battle_session.h"
#include "battle_effect.h"
#include "battle_effect_work.h"

// The heap blocks the dispatcher hands every canvas effect, as byte offsets
// into Ram_HeapSlots (☀️ uses slot indices).
#define HEAP_BLITTER 104
#define HEAP_BLITTER_ALTERNATE 188

// BattleEffect_LoadBlitter's argument: The Lost Age's blitter mode (☀️'s counted down
// from 3) in the high nibble, the COMPOSITE_ flags in the low one.
#define BLITTER(mode, flags) ((mode) << 4 | (flags))
#define BLIT_ADDITIVE 0
#define BLIT_MAX 1

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
extern char ResourceId_Data134;    // ☀️ ParticleSpritesA
extern const u16 gData_08197410[]; // ☀️ gBattleEffect_MoteCells

struct EffectCamera {
  s32 eye[3];
  s32 pos[3];
};

void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_ProcessPendingGraphicsTransfer(void);
void Func_081180e8(s32 phase); // BattlePlayback_StartFar
void Sound_PlayFar(s32 cue);
void BattleEffect_LoadBlitter(s32 slot, s32 modeAndFlags); // BattleEffect_BuildBlitter(slot,
                                                // 7, 7, flags, mode)
void BattleEffect_FetchRectangleBlitters(
    s32 side,
    DrawRectangle *draw);
void BattleMotion_ApproachTargetFar(s32, s16, s32, s32);
void BattleEffect_CallDoNothing(s32 frames);
void BattleEffectKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                      s32 gravity);
void Func_08118088(s32 actor,
                   s32 variant); // BattleMotion_ApplyVariantMotionFar
void Func_0814cd48(s32 actor, s32 palette, s32 a, s32 timer,
                   s32 frames);   // BattleEffect_TintBattler
void Func_08158ce0(s32 x, s32 y); // Camera_ApplyShake
void BattleEffect_CountDownBattlerTints(void);
void BattleEffect_EndCanvasLayer(void);

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
  WaitFrames(1);
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
      Func_081180e8(PHASE_RESULTS);
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

      BattleEffect_LoadBlitter(HEAP_BLITTER,
                    BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
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
      Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);

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
          draw[(i / 2) & 1](canvas, cells + gData_08197410[size - 1],
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
      Func_08118088(effect->actors[0], 4);
      work->shakeFrames = 4;
    }
    if (frame == STREAK_FRAME)
      Func_0814cd48(effect->actors[0], 10, -1, -1, 0);
    if (frame == FLASH_FRAME)
      Func_0814cd48(effect->actors[0], 10, -1, -1, 0);
    Func_08158ce0(16, 16);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    WaitFrames(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}
