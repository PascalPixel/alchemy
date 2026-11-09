#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

struct DoorStep {
  s32 script;
  u16 x;
  u16 z;
};

void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02001be4(void);
void Func_02001bec(void);

extern const u8 gData_02001e48[];
extern const u8 gData_02002100[];
extern const u8 gData_02002358[];
extern const u8 gData_020025b0[];
extern struct DoorStep gData_0200277c[];

const u8 *Func_0200004c(void) {
  if (GameFlag_Test(0x8a7)) {
    if (GameFlag_Test(0x90f) == 0)
      return gData_020025b0;
    return gData_02002358;
  }
  if (GameFlag_Test(0x8ab))
    return gData_02002100;
  return gData_02001e48;
}

void Func_02000098(struct DoorStep *step, s32 countdown, s32 unused) {
  Func_02001be4();
  Event_ResolvePendingActions(0);
  Sound_Play(158);
  Map_PlayMapEdit(step->script, step->x, step->z);
  Actor_GetById(gMainRoutineWork.currentOwner)->motionFlags = 2;
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 2);
  Actor_CenterAndWalk(gMainRoutineWork.currentOwner, 2, -8);
  Event_Wait(10);
  Import_EventSetExitRequest(countdown);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02001bec();
}

void Func_0200010c(s32 index) {
  struct DoorStep *table = gData_0200277c;

  
  Func_02000098(&table[index], index, 0);
}
