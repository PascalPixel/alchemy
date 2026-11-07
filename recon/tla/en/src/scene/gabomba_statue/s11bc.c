#include "../../../../../../games/tla/src/scene/gabomba_statue_chamber/gabomba_statue_chamber.h"
#include "scene_tables.h"

void Func_020011bc(s32 x, s32 z);
void Func_020056b4(void);
void Func_0200580c(s32 skip);
void Func_02005664(struct GabombaStatueChamberObject *object);
void Func_02005854(s32 cue);
void Func_020057c4(s32 countdown);

void Actor_RefreshSelectorById(u32 id);

void Func_020011bc(s32 x, s32 z) {
  struct GabombaStatueChamberEventWork *work = Ram_HeapSlots->eventWork;
  struct GabombaStatueChamberObject *object =
      Actor_GetById(gMainRoutineWork.currentOwner);

  Func_020056b4();
  Func_0200580c(0);
  object->motionFlags = 0;
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x10000, 0x8000);
  Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, x, z);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  Actor_RefreshSelectorById(gMainRoutineWork.currentOwner);
  Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 13);
  Func_0200565c(object, x << 16, PIXELS(-12), z << 16);
  Func_02005664(object);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 10);
  Func_02005854(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_020057c4(work->exit);
}
