#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "task.h"
#include "io_reg.h"
#include "event_inline.h"

struct EventWork {
  u8 unk000[0x1ac];
  s32 startTransition;
  u8 unk1b0[4];
  s32 transitionFrames;
};

extern const u8 gData_02004eb8[];

void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Event_FinishScreenTransition(void);
void Func_020028cc(void);
void Func_02004c20(void);

static __inline__ void Actor_SetSpeedParameters_(s32 actor, s32 speed,
                                                        s32 acceleration) {
  Actor_SetSpeedParameters(actor, speed, acceleration);
}

void Func_020029a4(void) {
  struct Actor *actor;

  actor = Actor_Get(8);
  FieldEvent_Wait(60);
  Func_02004c20();
  FieldEvent_ResolvePendingActions(0);
  FieldEffect_ScheduleRatioTransition(0x9999, 1);
  actor->scaleX = 0x13333;
  actor->scaleY = 0x13333;
  FieldCamera_FollowActor(8, 1);
  System_Wait(1);
  Actor_SetSpriteColor(Actor_Get(gMainRoutineWork.currentOwner), 15);
  Actor_SetSingleChildFlags(Actor_Get(gMainRoutineWork.currentOwner),
                                     0);
  Actor_SetSingleChildFlags(Actor_Get(8), 0);
  Actor_SetSpeedParameters_(8, 0x6666, 0x3333);
  actor->action = 0;
  Actor_SetScript(8, gData_02004eb8);
  Task_AddFrame(Func_020028cc, TASK_PASS_VBLANK);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->startTransition = 0x100;
  PaletteFade_ApplyTarget(0x10003, 1);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->transitionFrames = 32;
  Event_StartScreenTransition();
  FieldEvent_Wait(120);
  FieldEffect_ScheduleRatioTransition(0x16666, 300);
  FieldEvent_Wait(270);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->transitionFrames = 16;
  *(u16 *)BG_PLTT = RGB(31, 31, 31);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(77);
}
