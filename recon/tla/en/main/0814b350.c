// Near miss, differ 89 of 744: Func_0814b350, an effect no table reaches
// (unused), with one tagged dead store (462 without).

// ☀️ twin: BattleEffect_RunSparkTravel (games/tbs/src/effect/effect.c), shape
// 0.888, structure 1.000. ⚓️'s changes: the effect argument read directly,
// the Mercury Djinn's small sheet (directory row 0x155) as ☀️'s, the
// transfer callback at 0xc80, and ⚓️'s names for the shared routines.
// ☀️'s dead store of the size series A draws at carries over with its tag:
// without it the frame and allocation move (462).

// What is left: one instruction more than the native (745 of 744), where
// agscc builds the rider record's address as movs r2, #156; add r2, sp and
// the native as add r2, sp, #156; neighbouring rows in the other order in
// the prologue and the travel set-up; and pool offsets that follow them.

// Tried (differ):
// ☀️'s body with ⚓️'s names, without the dead store                  462
// with ☀️'s tagged dead store                                         89
// greedy declaration-order scan, every local through every place     89

#include "types.h"
#include "system.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "callback_scheduler.h"
#include "mem.h"
#include "effect_step.h"
#include "motobj.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "battle_display.h"
#include "graphics.h"
#include "transform.h"
#include "fixed_math.h"

#define HEAP_BLITTER 104           // ☀️ HEAP_SLOT_BLITTER
#define HEAP_BLITTER_ALTERNATE 188 // ☀️ HEAP_SLOT_BLITTER_ALTERNATE
#define CALLBACK_ORDER 0xc80

// The sparks live in the map cell buffer.
#define PARTICLES ((struct EffectStep *)Ram_MapCellBuffer)

extern char ResourceId_MercuryDjinnSmallSheet; // directory row 0x155
extern char ResourceId_ParticleSpritesA;       // directory row 0x134
extern char ResourceId_SparkleDots;            // directory row 0x17e

extern u16 gData_08197410[]; // ☀️ gBattleEffect_MoteCells
extern u16 Data_0819744c[];  // ☀️ gBattleEffect_PuffCells
extern u8 Data_0819745e[];   // ☀️ gBattleEffect_PuffSizes

void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void); // ☀️ BattleEffect_EndCanvasLayer
void BattleEffect_ProcessPendingGraphicsTransfer(void); // ☀️ ProcessPendingGraphicsTransfer
void BattleEffect_FetchRectangleBlitters(
    s32 side, DrawRectangle *draw); // ☀️ FetchRectangleBlitters
void Func_0814cd48(s32 actor, s32 palette, s32 frames, s32 timerSlot,
                   s32 timerFrames); // ☀️ ObjectGroup_UpdateMembers
void Func_080150e4(s32 angle);       // ☀️ SceneTransform_ApplyRoll
void BattleEffectKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                      s32 gravity);
void SceneTransform_ApplyPosition(s32 *position);
void SceneTransform_ApplyYaw(s32 angle);
void SceneTransform_ApplyPitch(s32 angle);
void Transform_Pop(void);
void Audio_PlayCue(s32 cue);

// A Djinn rides a swarm of sparks from the caster to the first target: 32
// sparks spin around it in four planes while it travels, then it hovers
// over the targets while puffs of sparkle gather on each in turn.
void Func_0814b350(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 member;
  u8 *sheet;
  DrawRectangle *draw;
  struct MotionObject *actor;
  struct MotionObject *goal;
  s32 frame;
  s32 i;
  s32 position[3];
  s32 target[3];
  s32 delta[3];
  s32 rider[3];
  struct EffectPosition screen;
  s32 point[3];
  struct EffectPosition view;
  DrawRectangle callbacks[2];

  // Set up: the canvas layer, the Djinn and the particle sprites, and the
  // blitter pair for the side the Djinn faces.
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sheet = Ram_HeapSlots->unk64[0];
  BattleEffect_StartCanvasLayer(0);
  Resource_LoadAndDecompress((s32)&ResourceId_MercuryDjinnSmallSheet,
                             work->sheet, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  BattleEffect_FetchRectangleBlitters(effect->side ^ 1, draw = callbacks);
  actor = GetBattleObjectSlotFar(effect->actor)->object;
  goal = GetBattleObjectSlotFar(effect->actors[0])->object;

  // The swarm: 64 sparks thrown out level from the Djinn.
  for (i = 0; i != 64; i++) {
    struct EffectStep *spark = &PARTICLES[i];
    s32 heading = Random16() & 0xffff;
    s32 speed = (Random16() & 0xff) + 128;

    spark->x = 0;
    spark->y = ((Random16() & 31) + 20) << 16;
    spark->z = 0;
    spark->velocityX = (Trig_Sin(heading) * speed) >> 5;
    spark->velocityY = 0;
    spark->velocityZ = (Trig_Cos(heading) * speed) >> 5;
    spark->variant = 0;
  }
  work->transferMode = 2;
  work->transferValue = 75;
  Scheduler_AddOrUpdateCallback(BattleEffect_ProcessPendingGraphicsTransfer, CALLBACK_ORDER);

  // The Djinn travels from the caster to above the first target in 40
  // frames.
  position[0] = actor->x;
  position[1] = 0;
  position[2] = actor->z;
  target[0] = goal->x;
  target[1] = 180 << 15;
  target[2] = 0;
  delta[0] = (target[0] - position[0]) / 40;
  delta[1] = (target[1] - position[1]) / 40;
  delta[2] = (target[2] - position[2]) / 40;

  for (frame = 0; frame != 98; frame++) {
    struct BattleCamera *camera = Ram_HeapSlots->cameraWork;

    if (frame == 8)
      Audio_PlayCue(212);
    if (frame == 80)
      Audio_PlayCue(142);
    Render_ResetTransformState();
    Transform_SetLookAt(camera->eye, camera->pos);
    if (frame >= 30 && frame < 70) {
      position[0] += delta[0];
      position[1] += delta[1];
      position[2] += delta[2];
    }
    SceneTransform_ApplyPosition(position);

    // The caster flashes for the first 24 frames.
    if (frame == 0)
      Func_0814cd48(effect->actor, 7, -1, -1, 0);
    if (frame == 24)
      Func_0814cd48(effect->actor, 0, -1, -1, 0);

    // Each spark spins in one of four planes, smaller further away, and
    // after 30 frames is pulled back toward the Djinn.
    for (i = 0; i != 32; i++) {
      struct EffectStep *spark = &PARTICLES[i];

      if (frame > i && spark->variant == 0) {
        s32 size;

        Transform_Push();
        switch (i & 3) {
        case 0:
          SceneTransform_ApplyYaw(frame * (i * 32 + 256));
          break;
        case 1:
          SceneTransform_ApplyPitch(-frame * (i * 32 + 256));
          break;
        case 2:
          Func_080150e4(-frame * (i * 32 + 256));
          break;
        case 3:
          SceneTransform_ApplyPitch(-frame * (i * 32 + 256));
          Func_080150e4(-frame * (i * 32 + 256));
          break;
        }
        EffectPosition_ProjectPoint((s32 *)spark, &screen);
        screen.x >>= 1;
        Transform_Pop();
        if (screen.depth < 250)
          screen.depth = 250;
        if (screen.depth > 634)
          screen.depth = 634;
        size = 9 - (screen.depth - 250) / 64;
        callbacks[0](canvas, sheet + gData_08197410[size - 1],
                     screen.x - size / 2, screen.y - size, size, size * 2);
        BattleEffectKernels_IntegrateVector3(spark, 60, 0);
        if (frame > i + 30) {
          s32 dx = -spark->x >> 8;
          s32 dy = -spark->y >> 8;
          s32 dz = -spark->z >> 8;

          spark->velocityX += dx;
          spark->velocityY += dy;
          spark->velocityZ += dz;
        }
      }
    }

    // The Djinn bobs over the target for the last 15 frames.
    if (frame > 82) {
      rider[0] = 0;
      rider[1] = Trig_Sin(frame << 10) << 2;
      rider[2] = 0;
      EffectPosition_ProjectPoint(rider, &screen);
      screen.x >>= 1;
      draw[1](canvas, work->sheet, screen.x - 10, screen.y - 17, 20, 34);
    }
    work->transferPending = 1;
    WaitFrames(1);
  }

  // The second part: 64 puffs of sparkle for each target.
  for (i = 0; i != 512; i++) {
    struct EffectStep *spark = &PARTICLES[i];

    spark->x = ((Random16() & 0xff) - 127) << 15;
    spark->y = ((Random16() & 0x7f) + 64) << 15;
    spark->z = ((Random16() & 0xff) - 127) << 15;
    spark->variant = 0;
  }
  Resource_LoadAndDecompress((s32)&ResourceId_SparkleDots, sheet, 0, 0);

  for (frame = 0; frame != effect->count * 8 + 72; frame++) {
    struct BattleCamera *camera = Ram_HeapSlots->cameraWork;

    // The Djinn sways over the targets, then rises away for the last 32
    // frames.
    Render_ResetTransformState();
    Transform_SetLookAt(camera->eye, camera->pos);
    if (frame >= effect->count * 8 + 40)
      target[1] += 0x40000;
    point[0] = target[0];
    point[1] = target[1];
    point[2] = target[2] + Trig_Sin(frame << 11) * 40;
    EffectPosition_ProjectPoint(point, &view);
    view.x >>= 1;
    callbacks[0](canvas, work->sheet, view.x - 10, view.y - 17, 20, 34);

    // Each target in turn, eight frames apart: its puffs close in around
    // it, and it flashes from 40 to 64 frames into its turn.
    for (member = 0; member != effect->count; member++) {
      struct MotionObject *object =
          GetBattleObjectSlotFar(effect->actors[member])->object;
      s32 start = member * 8;

      Render_ResetTransformState();
      Transform_SetLookAt(camera->eye, camera->pos);
      position[0] = object->x;
      position[1] = 160 << 14;
      position[2] = object->z;
      SceneTransform_ApplyPosition(position);
      if (frame == start + 30)
        Audio_PlayCue(126);
      if (frame == start + 40)
        Func_0814cd48(effect->actors[member], 7, -1, -1, 0);
      if (frame == start + 64)
        Func_0814cd48(effect->actors[member], 0, -1, -1, 0);
      if (frame > start) {
        SceneTransform_ApplyYaw((frame - start) << 9);
        for (i = 0; i != 64; i++) {
          struct EffectStep *spark = &PARTICLES[member * 64 + i];

          if (frame > start + i / 2) {
            s32 xx = (spark->x >> 8) * (spark->x >> 8);
            s32 yy = (spark->y >> 8) * (spark->y >> 8);
            s32 zz = (spark->z >> 8) * (spark->z >> 8);
            s32 distance = Iwram_Sqrt(xx + yy + zz) >> 9;

            if (distance != 0) {
              s32 cel;

              EffectPosition_ProjectPoint((s32 *)spark, &view);
              view.x >>= 1;
              if (view.depth < 314)
                view.depth = 314;
              if (view.depth > 634)
                view.depth = 634;
              /* FAKEMATCH: the size series A draws its sparks at, kept from
               * that effect and never used here, as in ☀️. Without the dead
               * store: differ 462 (☀️: 479). */
              cel = 3 - (view.depth - 314) / 128;
              cel = (i * 4 + frame) % 9;
              {
                u8 *cell = sheet + Data_0819744c[cel];
                u32 edge = Data_0819745e[cel];
                u32 half = edge >> 1;

                draw[1](canvas, cell, view.x - half, view.y - half, edge, edge);
              }
              spark->x -= spark->x / distance;
              spark->y -= spark->y / distance;
              spark->z -= spark->z / distance;
            }
          }
        }
      }
    }
    work->transferPending = 1;
    WaitFrames(1);
  }

  // Tear down.
  Scheduler_RemoveCallback(BattleEffect_ProcessPendingGraphicsTransfer);
  Runtime_ReleaseHeapBlock(HEAP_BLITTER_ALTERNATE);
  Runtime_ReleaseHeapBlock(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}
