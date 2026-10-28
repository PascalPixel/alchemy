#include "types.h"
#include "system.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "task.h"
#include "heap_release.h"
#include "effect_step.h"
#include "motobj.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "battle_display.h"
#include "graphics.h"
#include "transform.h"

// Heap slots The Lost Age addresses by byte offset in Ram_HeapSlots.
#define HEAP_BLITTER 104           // ☀️ HEAP_SLOT_BLITTER
#define HEAP_BLITTER_ALTERNATE 188 // ☀️ HEAP_SLOT_BLITTER_ALTERNATE

// The Lost Age numbers its blitter modes in the reverse of The Broken Seal's
// order: Func_08196404 builds additive for 0, max for 1, transparent for 2.
enum { BLITTER_ADDITIVE, BLITTER_MAX, BLITTER_TRANSPARENT, BLITTER_COPY };

#define MOTE_COUNT 30
#define SEAT_FIRST 32 // the particles that hold each target's seat

extern char ResourceId_VortexSheet;

// ☀️ gVortices_Counts: motes, then frames, for each variant.
extern u8 gData_081979a8[];

void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_ProcessPendingGraphicsTransfer(void);                  // ☀️ ProcessPendingGraphicsTransfer
void Camera_ApplyPhasedDelta(void);
void BattleEffect_CountDownBattlerTints(void);
void Func_08158ce0(s32 x, s32 y);          // ☀️ Camera_ApplyShake
void Func_081180e8(s32 phase);             // ☀️ BattleEvent_StartPhaseFar
void Func_08118088(s32 actor, s32 motion); // ☀️ ApplyVariantMotionFar
s32 BattleEffect_LoadBlitter(s32 slot, s32 flags);    // ☀️ BattleEffect_BuildBlitter
void Func_0814cd48(s32 actor, s32 palette, s32 frames, s32 timerSlot,
                   s32 timerFrames); // ☀️ BattleEffect_TintBattler
void Sound_PlayFar(s32 cue);
s32 BattleUnit_GetEffectHeightFar(s32 id);

// Thirty motes rise from the caster, swirl, and home in on the targets; each
// one that reaches the ground flashes and knocks back its target and plays a
// three-cel splash where it landed.
void BattleEffect_RunFroth(struct BattleEffectArgument *effect) {
  struct EffectPosition pos;
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle draw[2];
  struct MotionObject *source;
  struct MotionObject *object;
  struct EffectStep *point;
  struct EffectStep *seat;
  struct EffectStep *target;
  struct BattleCamera *camera;
  s32 frame;
  s32 y;
  s32 i;
  s8 *landed;

  // Set up: the canvas layer, the vortex sheet and two blitters, the one on
  // the right mirrored.
  landed = (s8 *)Ram_MapCellBuffer;
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  BattleEffect_StartCanvasLayer(1);
  Resource_LoadAndDecompress((s32)&ResourceId_VortexSheet, work->sheet, 1, 1);
  BattleEffect_LoadBlitter(HEAP_BLITTER,
                BLITTER_MAX << 4 | COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y);
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE, BLITTER_MAX << 4 | COMPOSITE_CLIP_X |
                                            COMPOSITE_CLIP_Y | COMPOSITE_FLIP);
  REG_BLDALPHA = BLEND_ALPHA(15, 15);
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];

  // The motes start at the caster's chest with random velocities.
  source = GetBattleObjectSlotFar(effect->actor)->object;
  y = source->y + BattleUnit_GetEffectHeightFar(effect->actor);
  for (i = 0; i != MOTE_COUNT; i++) {
    point = &work->particles[i];
    point->x = source->x;
    point->y = y;
    point->z = source->z;
    point->velocityX = ((s32)((Random16() & 255) - 127) << 16) >> 5;
    point->velocityY = ((s32)((Random16() & 127) - 16) << 16) >> 6;
    point->velocityZ = ((s32)((Random16() & 255) - 127) << 16) >> 5;
    point->variant = -1;
    landed[i] = 0;
  }

  // Each target's seat on the ground.
  for (i = 0; i != effect->count; i++) {
    seat = &work->particles[SEAT_FIRST + i];
    object = GetBattleObjectSlotFar(effect->actors[i])->object;
    seat->x = object->x;
    seat->y = 0;
    seat->z = object->z;
  }

  work->cameraDelta = 0;
  work->cameraPhase = 0;
  Task_Add(Camera_ApplyPhasedDelta, TASK_PASS_FRAME);
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  Sound_PlayFar(164);

  for (frame = 0; frame != gData_081979a8[effect->variant * 2 + 1]; frame++) {
    // The camera turns between frames 17 and 63.
    camera = Ram_HeapSlots->cameraWork;
    if (frame >= 17 && frame < 64)
      work->cameraDelta = 0x180;
    else
      work->cameraDelta = 0;
    if (frame == gData_081979a8[effect->variant * 2 + 1] - 16)
      Func_081180e8(133);
    Render_ResetTransformState();
    Transform_SetLookAt(camera->eye, camera->pos);

    for (i = 0; i != gData_081979a8[effect->variant * 2]; i++) {
      point = &work->particles[i];

      // A mote leaves the caster two frames after the one before it.
      if (frame > i * 2 && landed[i] == 0) {
        EffectPosition_ProjectPoint((s32 *)point, &pos);
        pos.x >>= 1;
        if (pos.depth < 160)
          pos.depth = 160;
        if (pos.depth > 0x31f)
          pos.depth = 0x31f;
        draw[0](canvas, work->sheet + 0xc00, pos.x - 6, pos.y - 12, 12, 24);
        point->x += point->velocityX;
        point->y += point->velocityY;
        point->z += point->velocityZ;
      }

      // After 48 frames it steers toward its target's seat, damped for the
      // first 37 of them, and lands.
      if (frame > i * 2 + 48 && landed[i] == 0) {
        target = &work->particles[SEAT_FIRST + i % effect->count];
        point->velocityX += (target->x - point->x) >> 9;
        point->velocityY += (target->y - point->y) >> 9;
        point->velocityZ += (target->z - point->z) >> 9;
        if (frame < i * 2 + 85) {
          point->velocityX = point->velocityX * 60 / 64;
          point->velocityY = point->velocityY * 60 / 64;
          point->velocityZ = point->velocityZ * 60 / 64;
        }
        if (point->y < 0) {
          landed[i] = 1;
          point->variant = 0;
          point->x = pos.x;
          point->y = pos.y + (Random16() & 31) - 16;
          Func_0814cd48(effect->actors[i % effect->count], 7, 5,
                        i % effect->count, 4);
          Func_08118088(effect->actors[i % effect->count], 0);
          work->shakeFrames = 4;
          Sound_PlayFar(132);
        }
      }

      // The splash: three cels, two frames each, drawn as mirrored halves.
      if (point->variant >= 0 && point->variant < 16) {
        draw[0](canvas, work->sheet + ((point->variant / 2) % 3 << 10),
                point->x - 16, point->y - 56, 16, 64);
        draw[1](canvas, work->sheet + ((point->variant / 2) % 3 << 10),
                point->x, point->y - 56, 16, 64);
        point->variant++;
      }
    }

    Func_08158ce0(effect->variant * 2 + 2, effect->variant * 2 + 2);
    if (work->cameraPhase == 0)
      work->cameraPhase = 1;
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  // Tear down.
  Task_Remove(Camera_ApplyPhasedDelta);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}
