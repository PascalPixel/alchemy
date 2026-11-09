#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Event_StartScreenTransition(void);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_0200343c(void);
void Func_02004c20(void);
void Func_02004c28(void);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Actor_SetPositionAndCommit_(s32 actor, s32 x,
                                                          s32 z) {
  Actor_SetPositionAndCommit(actor, x, z);
}

void Func_02003490(void) {
  Func_02004c20();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetSpriteColor(Actor_Get(gMainRoutineWork.currentOwner), 15);
  Actor_SetSingleChildFlags(Actor_Get(gMainRoutineWork.currentOwner),
                                     0);
  Actor_Get(gMainRoutineWork.currentOwner)->update = Func_0200343c;
  Actor_Get(gMainRoutineWork.currentOwner)->acceleration = 0x2000;
  Actor_Get(gMainRoutineWork.currentOwner)->speedLimit = 0x20000;
  System_Wait(5);
  FieldEffect_ScheduleRatioTransition(0xc000, 60);
  Event_StartScreenTransition();
  Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 14537, 8577);
  Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 14465, 8662);
  Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 14452, 8766);
  Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 14387, 8828);
  Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 14331, 8888);
  Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 14284, 8904);
  Actor_SetPositionAndCommit_(gMainRoutineWork.currentOwner, 14255, 8870);
  Actor_SetPositionAndCommit_(gMainRoutineWork.currentOwner, 14255, 8831);
  Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 14255, 8772);
  FieldEffect_ScheduleRatioTransition(0x14000, 120);
  Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 14275, 8736);
  Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 14257, 8720);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02004c28();
  FieldEvent_SetExitRequest(78);
}
