#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"

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

struct Actor *Actor_GetById(s32 id);
void FieldEvent_Wait(s32 frames);
void Actor_CenterOnColumn(s32 id, s32 column, s32 offset);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02002ae8(const void *data, s32 a, s32 b);
void Func_02002b28(void);
void Func_02002b30(void);
void Func_02002c30(s32 a);
void Func_02002c58(s32 a);
void Func_02002c90(s32 a);

static inline void Actor_CenterAndWalk(s32 actorId, s32 priority, s32 dz) {
  Actor_CenterOnColumn(actorId, priority, dz);
}

void Func_0200006c(void) {
  struct MadraEventWork *work;
  struct Actor *object;
  s16 *trigger;
  u32 id;
  s32 index;

  work = Ram_HeapSlots->eventWork;
  Func_02002b28();
  Func_02002c58(0);
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
  Func_02002c90(158);
  {
    s32 x = gData_02003308[index].x;
    s32 y = gData_02003308[index].y;

    Func_02002ae8(gData_02003308[index].data, x, y);
  }
  Actor_GetById(gMainRoutineWork.currentOwner)->motionFlags = 0;
  Actor_SetModeById(gMainRoutineWork.currentOwner, 2);
  Actor_CenterAndWalk(gMainRoutineWork.currentOwner, 2, -8);
  FieldEvent_Wait(10);
  Func_02002c30(*trigger);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02002b30();
}
