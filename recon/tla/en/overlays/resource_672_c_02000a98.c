// Draft, differ 4 of 79: the native builds the event work's base
// (movs r3, #192; lsls r3, r3, #18) before the party state's offset
// (movs r2, #133) and loads the work between them; sched2 ranks this draft's
// offset chain one higher (122 against 121) and builds 532 first. Tried:
// separate declarations, in either order, &gPartyState as a local before or
// after the work, the heap slots as a local; permute, 8 minutes, finds
// nothing better than 4. Also measured: one-pass blocks around either
// declaration (8 to 10), a volatile view of the heap slots or of the work
// pointer (5, 11), an owner pointer local (5). Its copy group is led by
// 671's Func_020011bc in another lane.
#include "../../../../games/tla/src/scene/gabomba_statue_chamber/gabomba_statue_chamber.h"

void Object_RefreshSelectorById(u32 id);

void Func_02000a98(s32 x, s32 z) {
  struct GabombaStatueChamberEventWork *work = Ram_HeapSlots->eventWork;
  struct GabombaStatueChamberObject *object =
      Object_GetById(gPartyState.currentOwner);

  Func_02001da0();
  Func_02001ee8(0);
  object->motionFlags = 0;
  GabombaStatueChamber_ObjSpeed(gPartyState.currentOwner, 0x10000, 0x8000);
  GabombaStatueChamber_ObjWalk(gPartyState.currentOwner, x, z);
  GabombaStatueChamber_ObjArm(gPartyState.currentOwner, 0xc000, 0);
  Object_RefreshSelectorById(gPartyState.currentOwner);
  ObjectDispatch_SetSingleChildFlags(Object_GetById(gPartyState.currentOwner),
                                     0);
  Object_SetModeById(gPartyState.currentOwner, 13);
  Func_02001d48(object, x << 16, -0xc0000, z << 16);
  Func_02001d50(object);
  Object_SetModeById(gPartyState.currentOwner, 10);
  GabombaStatueChamber_SePlay(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02001eb0(work->exit);
}
