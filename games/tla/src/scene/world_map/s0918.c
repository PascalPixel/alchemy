#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "flag_call.h"
#include "text/msg_ids.h"
#include "event_inline.h"

struct EventDestination {
  u8 unk000[0x154];
  s16 scene;
};

void Func_02004c20(void);
void Func_02004c28(void);
void Func_02004c98(s32 actor, s32 target);
void Func_02004da8(void);
void Func_02004db0(void);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_0200050c(void);

/// Back to the map: actor 8's position is kept in gMainRoutineWork.
void Func_02000918(void) {
  struct EventDestination *work = Ram_HeapSlots->eventWork;
  struct Actor *leader = Actor_Get(8);

  Func_02004c20();
  FieldEvent_ResolvePendingActions(0);
  if (gMainRoutineWork.renderMode == 8)
    Func_0200050c();
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  gMainRoutineWork.savedX = leader->x;
  gMainRoutineWork.savedZ = leader->z;
  gMainRoutineWork.unk280 = 0;
  gMainRoutineWork.ownerOverride = 0;
  Event_ClearFlag(0x176);
  Event_PlaySound(123);
  FieldEvent_SetExitRequest(work->scene);
}
