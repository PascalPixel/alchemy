#include "local.h"

void Func_02000d84(void) {
  struct ShamanVillageCaveEventWork *work = Ram_HeapSlots->eventWork;
  struct Actor *object;
  u32 id;

  Func_02001080();
  Func_020011e0(0);
  id = 8;
next:
  object = Actor_GetById(id);
  if (object)
    object->motionFlags = 0;
  id++;
  if (id < 64)
    goto next;
  Actor_GetById(gSequenceParentWork.currentOwner)->motionFlags = 0;
  {
    s32 mode = 2;

    /* FAKEMATCH: the barrier holds r0 until the mode is set, so the leader is
     * loaded after it as the native has; plain, or with the mode pinned to r1,
     * the leader is loaded first (one reordered row). */
    __asm__("" : "+l"(mode) : : "r0");
    Actor_SetModeById(gSequenceParentWork.currentOwner, mode);
  }
  Actor_CenterOnColumn(gSequenceParentWork.currentOwner, 2, -8);
  FieldEvent_Wait(5);
  Func_02001210(123);
  Func_02001198(work->value154);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02001088();
}
