// Draft, differ 48 of 298 (2026-10-26; 95 before), from the listing (The
// Broken Seal's BattleEffect_RunDrain is only a hint): a mote leaves the first
// target as a ring of 24 sparks turning about it and is pulled toward the
// user until it passes it. The blitter declared before the camera and the
// sheet gives the native's stack slots; the spark size kept in a local set
// before the ring loop gives the native's unfolded size arithmetic (12 - 2,
// a register 3). The pull's three deltas are shifted in their locals, as
// ☀️'s RunDrain shifts its deltas (95 -> 48: the native's registers and load
// order there). Left, scheduling: the native spills the effect before it
// builds the heap base (the known store-before-build pattern) and loads the
// work's slot before the camera's; after each of the orb's first two
// position stores it sets an argument before the next load from the target
// (str, movs, ldr), where alchemy-gcc loads first; and two loads of the ring
// loop sit a row from the native's. Tried before: the deltas in all six
// orders (97 to 179); now the six orders of the shifted deltas (71 to 188).
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

void BattleEffect_StartCanvasLayer(s32 layer);
void BattleEffect_ProcessPendingGraphicsTransfer(void);
void BattleEffect_EndCanvasLayer(void);
s32 BattleUnit_GetEffectHeightFar(s32 unit);
void BattleEffect_LoadBlitter(s32 slot, s32 kind);
void Audio_PlayCue(s32 cue);
void Func_081180e8(s32 phase);
void Func_0814cd48(s32 id, s32 a, s32 b, s32 c, s32 d);
void Transform_Pop(void);
void Func_08015160(s32 x, s32 y, s32 z);
void SceneTransform_ApplyPitch(s32);
void Func_080150e4(s32 angle);
void BattleEffectKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                      s32 gravity);
void ObjectGroup_TickMemberTimers(void);

// A mote leaves the first target and is drawn as a ring of 24 sparks
// turning about it while it is pulled back to the user.
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

  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleFxWork *)Ram_HeapSlots->battleFxWork[0];
  view = Ram_HeapSlots->cameraWork;
  sheet = Ram_HeapSlots->unk64[0];
  BattleEffect_StartCanvasLayer(1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);
  Iwram_CopyWords((void *)0x05000000,
                  Resource_GetTableEntry((s32)&ResourceId_Data17F), 128);
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
  Scheduler_AddOrUpdateCallback(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  Audio_PlayCue(142);

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

        orb->velocityX += dx;
        orb->velocityY += dy;
        orb->velocityZ += dz;
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
  Scheduler_RemoveCallback(BattleEffect_ProcessPendingGraphicsTransfer);
  Runtime_ReleaseHeapBlock(104);
  BattleEffect_EndCanvasLayer();
}
