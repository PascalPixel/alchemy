#include "types.h"
#include "call.h"
#include "actor.h"
#include "main_routine_work.h"

struct Actor *Actor_GetById(s32 id);
void wait(s32 frames);
void Actor_SetPartAttribute(struct Actor *object, s32 value);
void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
void Actor_SetPositionAndCommit(s32 actor, s32 x, s32 z);
void Event_StartScreenTransition(void);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_0200343c(void);
void Func_02004c20(void);
void Func_02004c28(void);
void Func_02004d40(s32 value);
void Func_02004db8(s32 value);
void Func_02004df8(s32 value, s32 mode);

static __inline__ void Actor_SetPositionAndCommit_(s32 actor, s32 x, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_SetPositionAndCommit(actor, x, z);
}

void Func_02003490(void) {
  /* FAKEMATCH(barrier): SPLIT_CONSTANT builds each constant as the game does, a
   * shifted byte plus an addend; as plain constants, 0 of 135 instructions
   * differ in tla-en. */
  Func_02004c20();
  Func_02004db8(0);
  Actor_SetPartAttribute(Actor_GetById(gMainRoutineWork.currentOwner), 15);
  Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  Actor_GetById(gMainRoutineWork.currentOwner)->update = Func_0200343c;
  Actor_GetById(gMainRoutineWork.currentOwner)->acceleration = 0x2000;
  Actor_GetById(gMainRoutineWork.currentOwner)->speedLimit = 0x20000;
  wait(5);
  Func_02004df8(0xc000, 60);
  Event_StartScreenTransition();
  Actor_SetPositionAndCommit(gMainRoutineWork.currentOwner,
                             SPLIT_CONSTANT(14537), SPLIT_CONSTANT(8577));
  Actor_SetPositionAndCommit(gMainRoutineWork.currentOwner,
                             SPLIT_CONSTANT(14465), SPLIT_CONSTANT(8662));
  Actor_SetPositionAndCommit(gMainRoutineWork.currentOwner,
                             SPLIT_CONSTANT(14452), SPLIT_CONSTANT(8766));
  Actor_SetPositionAndCommit(gMainRoutineWork.currentOwner,
                             SPLIT_CONSTANT(14387), SPLIT_CONSTANT(8828));
  Actor_SetPositionAndCommit(gMainRoutineWork.currentOwner,
                             SPLIT_CONSTANT(14331), SPLIT_CONSTANT(8888));
  Actor_SetPositionAndCommit(gMainRoutineWork.currentOwner,
                             SPLIT_CONSTANT(14284), SPLIT_CONSTANT(8904));
  Actor_SetPositionAndCommit_(gMainRoutineWork.currentOwner,
                              SPLIT_CONSTANT(14255), SPLIT_CONSTANT(8870));
  Actor_SetPositionAndCommit_(gMainRoutineWork.currentOwner,
                              SPLIT_CONSTANT(14255), SPLIT_CONSTANT(8831));
  Actor_SetPositionAndCommit(gMainRoutineWork.currentOwner,
                             SPLIT_CONSTANT(14255), SPLIT_CONSTANT(8772));
  Func_02004df8(0x14000, 120);
  Actor_SetPositionAndCommit(gMainRoutineWork.currentOwner,
                             SPLIT_CONSTANT(14275), SPLIT_CONSTANT(8736));
  Actor_SetPositionAndCommit(gMainRoutineWork.currentOwner,
                             SPLIT_CONSTANT(14257), SPLIT_CONSTANT(8720));
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02004c28();
  Func_02004d40(78);
}
