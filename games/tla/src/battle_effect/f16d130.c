/// @file
/// Nirvana's ring of sparks.

#include "types.h"
#include "ram_buffer.h"
#include "system.h"
#include "iwram_call.h"
#include "task.h"
#include "resource.h"
#include "graphics.h"
#include "transform.h"
#include "motobj.h"
#include "battle_action.h"
#include "effect_step.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "battle_fx_work.h"

struct EffectCamera {
  s32 eye[3];
  s32 pos[3];
};

extern char ResourceId_Data134;
extern char ResourceId_PinkBurstSheet;
extern u16 gData_08197410[];

void BattleEffect_StartCanvasLayer(s32 layer);
void BattleEffect_ProcessPendingGraphicsTransfer(void);
void BattleEffect_EndCanvasLayer(void);
s32 BattleUnit_GetEffectHeightFar(s32 unit);
s32 BattleEffect_LoadBlitter(s32 slot, s32 kind);
void Sound_PlayFar(s32 cue);
void Func_081180e8(s32 phase);
void Func_0814cd48(s32 id, s32 a, s32 b, s32 c, s32 d);
void Transform_Pop(void);
void Func_08015160(s32 x, s32 y, s32 z);
void SceneTransform_ApplyPitch(s32);
void Func_080150e4(s32 angle);
void BattleEffectKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                          s32 gravity);
void BattleEffect_CountDownBattlerTints(void);

/// A mote leaves the first target and is drawn as a ring of 24 sparks
/// turning about it while it is pulled back to the user.
void BattleEffect_RunNirvana(struct BattleEffectArgument *effect) {
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

  work = (struct BattleFxWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  view = Ram_HeapSlots->cameraWork;
  sheet = Ram_HeapSlots->unk64[0];
  BattleEffect_StartCanvasLayer(1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);
  Iwram_CopyWords((void *)0x05000000,
                  Resource_GetTableEntry((s32)&ResourceId_PinkBurstSheet), 128);
  object = GetBattleObjectSlotFar(effect->actors[0])->object;
  half = BattleUnit_GetEffectHeightFar(effect->actors[0]) / 2;
  orb = work->particles;
  orb->x = object->x;
  orb->y = object->y + half;
  orb->z = object->z;
  orb->velocityX = 0;
  orb->velocityY = 0;
  orb->velocityZ = 0;
  orb->variant = 0;
  BattleEffect_LoadBlitter(104, 19);
  blit = (DrawRectangle)Ram_HeapSlots->unk64[1];
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  Sound_PlayFar(142);

  for (frame = 0; frame != 80; frame++) {
    struct MotionObject *user = GetBattleObjectSlotFar(effect->actor)->object;
    s32 userHalf = BattleUnit_GetEffectHeightFar(effect->actor) / 2;

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
      BattleEffectKernels_IntegrateVector3(orb, 62, 0);
      if (frame > 0) {
        s32 dx = (user->x - orb->x) >> 10;
        s32 dy = (user->y + userHalf - orb->y) >> 10;
        s32 dz = (user->z - orb->z) >> 10;
        s32 ox;
        s32 ux;

        orb->velocityX += dx;
        orb->velocityY += dy;
        orb->velocityZ += dz;
        ux = user->x;
        if (ux < 0)
          ux = -ux;
        ox = orb->x;
        if (ox < 0)
          ox = -ox;
        if (ux < ox)
          orb->variant = -1;
      }
    }
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(104);
  BattleEffect_EndCanvasLayer();
}
