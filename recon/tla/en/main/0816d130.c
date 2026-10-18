// Draft, differ 97 of 298 (297 here), from the listing (The Broken Seal's
// BattleFx_RunDrain is only a hint): a mote leaves the first
// target as a ring of 24 sparks turning about it and is pulled toward the
// user until it passes it. The blitter declared before the camera and the
// sheet gives the native's stack slots; the spark size kept in a local set
// before the ring loop gives the native's unfolded size arithmetic (12 - 2,
// a register 3); the three deltas taken before the velocities are updated
// give the native's load order. Left: the user pointer lives in fp and the
// native copies it to a low register at each of four reads (here two, the
// third read reuses r4), so the deltas and the abs test take other
// registers (45 rows) and one copy is missing; the rest is reordered
// rows. Tried: the deltas in all six orders (97 to 179).
#include "types.h"
#include "ram_buffer.h"
#include "system.h"
#include "iwram_call.h"
#include "callback_scheduler.h"
#include "resource.h"
#include "graphics.h"
#include "transform.h"
#include "motobj.h"
#include "battle_session.h"
#include "effect_step.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "battle_fx_work.h"

struct EffectCamera {
  s32 eye[3];
  s32 pos[3];
};

extern char ResourceId_Data134;
extern char ResourceId_Data17F;
extern u16 gData_08197410[];

void BattleFx_BeginCanvasLayer(s32 layer);
void BattleFx_ProcessPendingGraphicsTransfer(void);
void BattleFx_EndCanvasLayer(void);
s32 Battle_GetObjectTableValueFar(s32 unit);
void BattleFx_LoadBlitter(s32 slot, s32 kind);
void Audio_PlayCue(s32 cue);
void Func_081180e8(s32 phase);
void Func_0814cd48(s32 id, s32 a, s32 b, s32 c, s32 d);
void Transform_Pop(void);
void Func_08015160(s32 x, s32 y, s32 z);
void SceneTransform_ApplyPitch(s32);
void Func_080150e4(s32 angle);
void BattleFxKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                      s32 gravity);
void ObjectGroup_TickMemberTimers(void);

// A mote leaves the first target and is drawn as a ring of 24 sparks
// turning about it while it is pulled back to the user.
void BattleFx_RunNirvana(struct BattleEffectArgument *effect) {
  struct BattleFxWork *work;
  void *canvas;
  DrawRectangle blit;
  struct EffectCamera *view;
  u8 *sheet;
  struct MotionObject *object;
  struct EffectStep *orb;
  s32 half;
  s32 frame;
  s32 i;

  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleFxWork *)Ram_HeapSlots->battleFxWork[0];
  view = Ram_HeapSlots->cameraWork;
  sheet = Ram_HeapSlots->unk64[0];
  BattleFx_BeginCanvasLayer(1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);
  Iwram_CopyWords((void *)0x05000000,
                  Resource_GetTableEntry((s32)&ResourceId_Data17F), 128);
  object = GetBattleObjectSlotFar(effect->actors[0])->object;
  half = Battle_GetObjectTableValueFar(effect->actors[0]) / 2;
  orb = work->particles;
  orb->x = object->x;
  orb->y = object->y + half;
  orb->z = object->z;
  orb->velocityX = 0;
  orb->velocityY = 0;
  orb->velocityZ = 0;
  orb->variant = 0;
  BattleFx_LoadBlitter(104, 19);
  blit = (DrawRectangle)Ram_HeapSlots->unk64[1];
  work->transferMode = 2;
  work->transferValue = 50;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0xc80);
  Audio_PlayCue(142);

  for (frame = 0; frame != 80; frame++) {
    struct MotionObject *user = GetBattleObjectSlotFar(effect->actor)->object;
    s32 userHalf = Battle_GetObjectTableValueFar(effect->actor) / 2;

    if (frame == 64)
      Func_081180e8(134);
    Render_ResetTransformState();
    Transform_SetLookAt(view->eye, view->pos);
    if (frame == 72)
      Func_0814cd48(effect->actor, 7, -1, -1, 0);
    if (frame == 79)
      Func_0814cd48(effect->actor, 0, -1, -1, 0);
    if (frame == 0)
      Func_0814cd48(effect->actors[0], 7, 5, 0, 42);
    if (orb->variant == 0) {
      s32 size = 6;

      for (i = 0; i != 24; i++) {
        s32 point[3];
        struct EffectPosition out;

        Transform_Push();
        Func_08015160(orb->x, orb->y, orb->z);
        if (i <= 11)
          SceneTransform_ApplyPitch(frame << 9);
        else
          SceneTransform_ApplyPitch((frame << 9) + 0x4000);
        Func_080150e4(i * 5461 + (frame << 10));
        Func_08015160(0, Trig_Sin(frame * 409) << 6, 0);
        point[0] = 0;
        point[1] = 0;
        point[2] = 0;
        EffectPosition_ProjectPoint(point, &out);
        out.x >>= 1;
        Transform_Pop();
        blit(canvas, sheet + gData_08197410[size - 1], out.x - size / 2,
             out.y - size, size, size * 2);
      }
      BattleFxKernels_IntegrateVector3(orb, 62, 0);
      if (frame > 0) {
        s32 dx = user->x - orb->x;
        s32 dy = user->y + userHalf - orb->y;
        s32 dz = user->z - orb->z;
        s32 ox;

        orb->velocityX += dx >> 10;
        orb->velocityY += dy >> 10;
        orb->velocityZ += dz >> 10;
        dx = user->x;
        if (dx < 0)
          dx = -dx;
        ox = orb->x;
        if (ox < 0)
          ox = -ox;
        if (dx < ox)
          orb->variant = -1;
      }
    }
    ObjectGroup_TickMemberTimers();
    work->transferPending = 1;
    WaitFrames(1);
  }
  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  Runtime_ReleaseHeapBlock(104);
  BattleFx_EndCanvasLayer();
}
