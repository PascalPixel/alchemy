#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_effect.h"
#include "event_work.h"
#include "call.h"

struct Actor *Actor_GetById(u32 objectId);
void Func_02003398(void);
void Func_02003488(s32 value);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Func_02003308(void);
void wait(s32 frames);
void Func_020034b8(s32 sound);
void FieldEvent_Wait(s32 frames);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
u32 Random16Far(void);
void Effect_AdvanceMotion(union FieldObject *object);
void Func_02002780(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags, struct EffectOptions *options);
void Func_02003410(s32 id, s32 value);
void Func_02003368(s32 a, s32 b, s32 c);
void Func_02003370(void);
void Func_020033a0(void);

void Func_02002f48(void) {
  struct Actor *actor = Actor_GetById(gMainRoutineWork.currentOwner);
  /* FAKEMATCH(pin): i in r10, which leaves r8 to the options pointer; as a
   * plain local, 12 of 169 instructions differ in tla-en. */
  register u32 i asm("r10");
  struct EffectOptions options;
  s32 spawn = 1;

  Func_02003398();
  Func_02003488(0);
  /* FAKEMATCH(wrapper): the call forwarded through Call4, which builds each -1
   * in its own register; as a direct call, 8 of 169 instructions differ in
   * tla-en. */
  Call4(Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  Func_02003308();
  wait(1);
  actor->y = 0x820000;
  actor->gravity = 0x8000;
  actor->bounce = 0;
  actor->motionFlags = 0;
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Func_020034b8(204);
  actor->motionFlags = 3;
  FieldEvent_Wait(24);
  options.palette = 7;
  options.update = Effect_AdvanceMotion;
  options.startScaleX = 0xa3d7;
  options.startScaleY = 0xa3d7;
  i = 0;
  // A goto loop carries no loop notes: as a do-while loop, loop.c moves the
  // velocity array's address out of the loop and 4 of 169 instructions differ
  // in tla-en (10 as a for or while loop).
loop:
  {
    s32 angle;
    s32 velocity[3];
    s32 cosine;

    angle = i << 12;
    cosine = Math_Cosine(angle);
    /* FAKEMATCH(dead): spawn is always 1, a constant GCSE only folds after its
     * partial redundancy pass, so that pass leaves the velocity array's
     * address in the loop, built after the cosine call as the native has it;
     * as the plain body, 4 of 169 instructions differ in tla-en. */
    if (spawn) {
      velocity[0] = cosine;
      velocity[1] = 0;
      velocity[2] = Math_Sine(angle);
      velocity[0] += velocity[0] >> 1;
      velocity[0] += (Random16Far() * 0xc000 >> 16) - 0x6000;
      velocity[2] += (Random16Far() * 0x6000 >> 16) - 0x3000;
      Func_02002780(actor->x, actor->y, actor->z, velocity[0], velocity[1],
                    velocity[2], 0x1090001, &options);
    }
    i++;
  }
  if (i <= 16)
    goto loop;
  Func_020034b8(188);
  Func_02003410(gMainRoutineWork.currentOwner, 0x101);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 49);
  /* FAKEMATCH(wrapper): the call forwarded through Call3, which builds each
   * constant in its own register; as a direct call, 26 of 169 instructions
   * differ in tla-en. */
  Call3(Func_02003368, 0x50000, 0x50000, 0x10000);
  /* FAKEMATCH(wrapper): the call forwarded through Call3, which builds each -1
   * in its own register; as a direct call, 7 of 169 instructions differ in
   * tla-en. */
  Call3(Func_02003368, -1, -1, 0xe666);
  Func_02003370();
  Func_02003410(gMainRoutineWork.currentOwner, 0x100);
  actor->gravity = 0x10000;
  actor->bounce = 0x4000;
  FieldEvent_Wait(10);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 1);
  Func_020033a0();
}
