#include "gabomba_statue_chamber.h"

void Object_RefreshSelectorById(u32 id);

void Func_02000a98(s32 x, s32 z) {
  struct GabombaStatueChamberEventWork *work = Ram_HeapSlots->eventWork;
  struct GabombaStatueChamberObject *object =
      Object_GetById(gAdventure.currentOwner);

  Func_02001da0();
  Func_02001ee8(0);
  object->motionFlags = 0;
  GabombaStatueChamber_ObjSpeed(gAdventure.currentOwner, 0x10000, 0x8000);
  GabombaStatueChamber_ObjWalk(gAdventure.currentOwner, x, z);
  GabombaStatueChamber_ObjArm(gAdventure.currentOwner, 0xc000, 0);
  Object_RefreshSelectorById(gAdventure.currentOwner);
  ObjectDispatch_SetSingleChildFlags(Object_GetById(gAdventure.currentOwner),
                                     0);
  Object_SetModeById(gAdventure.currentOwner, 13);
  Func_02001d48(object, x << 16, -0xc0000, z << 16);
  Func_02001d50(object);
  Object_SetModeById(gAdventure.currentOwner, 10);
  GabombaStatueChamber_SePlay(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02001eb0(work->exit);
}
