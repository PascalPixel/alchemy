#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "task.h"

struct EventWork {
  u8 unk000[0x1ac];
  s32 startTransition;
  u8 unk1b0[4];
  s32 transitionFrames;
};

extern const u8 gData_02004eb8[];

struct Actor *Actor_GetById(s32 id);
void wait(s32 frames);
void FieldEvent_Wait(s32 frames);
void Actor_SetPartAttribute(struct Actor *object, s32 value);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Camera_FollowActor(s32 actor, s32 mode);
void Actor_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void Actor_SetMovementTypeById(s32 actor, const u8 *action);
s32 Task_Add(void (*func)(void), s32 order);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Event_FinishScreenTransition(void);
void Func_020028cc(void);
void Func_02004c20(void);
void Func_02004d40(s32 value);
void Func_02004d78(s32 a, s32 b);
void Func_02004db8(s32 value);
void Func_02004df8(s32 value, s32 mode);

static __inline__ void Actor_SetSpeedParameters_(s32 actor, s32 speed,
                                                        s32 acceleration) {
  Actor_SetSpeedParameters(actor, speed, acceleration);
}

void Func_020029a4(void) {
  struct Actor *actor;

  actor = Actor_GetById(8);
  FieldEvent_Wait(60);
  Func_02004c20();
  Func_02004db8(0);
  Func_02004df8(0x9999, 1);
  actor->scaleX = 0x13333;
  actor->scaleY = 0x13333;
  Camera_FollowActor(8, 1);
  wait(1);
  Actor_SetPartAttribute(Actor_GetById(gMainRoutineWork.currentOwner), 15);
  Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner),
                                     0);
  Actor_SetSingleChildFlags(Actor_GetById(8), 0);
  Actor_SetSpeedParameters_(8, 0x6666, 0x3333);
  actor->action = 0;
  Actor_SetMovementTypeById(8, gData_02004eb8);
  Task_Add(Func_020028cc, TASK_PASS_VBLANK);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->startTransition = 0x100;
  Func_02004d78(0x10003, 1);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->transitionFrames = 32;
  Event_StartScreenTransition();
  FieldEvent_Wait(120);
  Func_02004df8(0x16666, 300);
  FieldEvent_Wait(270);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->transitionFrames = 16;
  *(u16 *)0x05000000 = 0x7fff;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02004d40(77);
}
