#include "kibombo.h"

void Func_020032b0(void);
void Func_02005bdc(s32 a, s32 b);

void Func_02004424(void) {
  struct KibomboEventWork *work;
  /* FAKEMATCH: the pin keeps the screen effect in r6 for both stores; as a
   * plain local it swaps with the callback in r5 (differ 39). */
  register s32 effect asm("r6");

  Func_02005a9c();
  Func_02005c3c(0);
  Object_SetPartAttribute(Object_GetById(gAdventure.currentOwner), 15);
  ObjectDispatch_SetSingleChildFlags(Object_GetById(gAdventure.currentOwner),
                                     0);
  Func_020032b0();
  KIBOMBO_TASK_ADD(Func_02002ccc, 1152);
  effect = 129;
  work = Ram_HeapSlots->eventWork;
  /* FAKEMATCH: the empty asm starts the effect's 513 before the event work load
   * and finishes it after, as the native does; with a volatile barrier the load
   * comes first, without one both constants come first (differ 2 and 4). */
  asm("" : "+l"(effect) : "l"(work));
  work->screenEffect = effect = effect * 2 + 255;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_WaitFrames(40);
  Func_02005b4c((s32)MsgOhTheMagic);
  Kibombo_MessagePut(17, 0);
  Kibombo_ObjTurn(17, 0x3000);
  Kibombo_MessagePut(0x4011, 0);
  Kibombo_ObjTurn(17, 0x5000);
  Kibombo_MessagePut(0x4011, 0);
  Scheduler_RemoveCallbackFar(Func_02002ccc);
  Kibombo_ObjTurn(17, 0xd000);
  Func_02005bdc(0x10000, 0);
  Func_02005bd4(0, 0);
  ((struct KibomboEventWork *)Ram_HeapSlots->eventWork)->screenEffect = effect;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  GameFlag_SetBit(0x8fc);
  GameFlag_SetBit(0x109);
  Func_02005bc4(12);
}
