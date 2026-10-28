#include "gabomba_statue_chamber.h"

void Actor_RefreshSelectorById(u32 id);

void Func_02000a98(s32 x, s32 z) {
  struct GabombaStatueChamberEventWork *work = Ram_HeapSlots->eventWork;
  struct GabombaStatueChamberObject *object =
      Actor_GetById(gMainRoutineWork.currentOwner);

  Func_02001da0();
  Func_02001ee8(0);
  object->motionFlags = 0;
  GabombaStatueChamber_ObjSpeed(gMainRoutineWork.currentOwner, 0x10000, 0x8000);
  GabombaStatueChamber_ObjWalk(gMainRoutineWork.currentOwner, x, z);
  GabombaStatueChamber_ObjArm(gMainRoutineWork.currentOwner, 0xc000, 0);
  Actor_RefreshSelectorById(gMainRoutineWork.currentOwner);
  Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner),
                                     0);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 13);
  Func_02001d48(object, x << 16, -0xc0000, z << 16);
  Func_02001d50(object);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 10);
  GabombaStatueChamber_SePlay(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02001eb0(work->exit);
}
