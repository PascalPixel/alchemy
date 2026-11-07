#include "../../../../../../games/tla/src/scene/gabomba_statue_chamber/gabomba_statue_chamber.h"
#include "scene_tables.h"

void Actor_RefreshSelectorById(u32 id);

void Func_02000a98(s32 x, s32 z) {
  struct GabombaStatueChamberEventWork *work = Ram_HeapSlots->eventWork;
  struct GabombaStatueChamberObject *object =
      Actor_GetById(gMainRoutineWork.currentOwner);

  Func_02001da0();
  Func_02001ee8(0);
  object->motionFlags = 0;
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x10000, 0x8000);
  Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, x, z);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  Actor_RefreshSelectorById(gMainRoutineWork.currentOwner);
  Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 13);
  Func_02001d48(object, x << 16, PIXELS(-12), z << 16);
  Func_02001d50(object);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 10);
  Func_02001f10(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02001eb0(work->exit);
}
