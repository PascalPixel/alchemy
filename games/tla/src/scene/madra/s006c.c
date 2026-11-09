#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

struct MadraEventWork {
  u8 unk000[0x154];
  s16 value154;
};

struct MadraPlacement {
  const void *data;
  u16 x;
  u16 y;
};

extern struct MadraPlacement gData_02003308[];

void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02002b28(void);
void Func_02002b30(void);

void Func_0200006c(void) {
  struct MadraEventWork *work;
  struct Actor *object;
  s16 *trigger;
  u32 id;
  s32 index;

  work = Ram_HeapSlots->eventWork;
  Func_02002b28();
  Event_ResolvePendingActions(0);
  id = 8;
next:
  object = Actor_GetById(id);
  if (object != NULL) {
    object->motionFlags = 0;
  }
  id++;
  if (id < 64)
    goto next;
  trigger = &work->value154;
  index = *trigger - 1;
  Sound_Play(158);
  {
    s32 x = gData_02003308[index].x;
    s32 y = gData_02003308[index].y;

    Map_PlayMapEdit(gData_02003308[index].data, x, y);
  }
  Actor_GetById(gMainRoutineWork.currentOwner)->motionFlags = 0;
  Actor_SetModeById(gMainRoutineWork.currentOwner, 2);
  Actor_CenterAndWalk(gMainRoutineWork.currentOwner, 2, -8);
  Event_Wait(10);
  Import_EventSetExitRequest(*trigger);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02002b30();
}
