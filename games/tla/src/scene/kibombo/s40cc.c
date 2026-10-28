#include "kibombo.h"

void Func_02002eb0(void);
void Func_020032e0(void);
void Func_02003610(void);

void Func_020040cc(void) {
  Actor_SetSingleChildFlags(Actor_GetById(gSequenceParentWork.currentOwner),
                                     0);
  Actor_SetPartAttribute(Actor_GetById(gSequenceParentWork.currentOwner), 15);
  Kibombo_CamMove(-1, -1, -1, 0);
  Func_02005bbc()->motionFlags = 0;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Kibombo_TaskAdd(Func_02002ccc, 0x480);
  Kibombo_Wait(40);
  Kibombo_SePlay(78);
  Kibombo_CamSpeed(0xcccc, 0x1999);
  Kibombo_CamMove(132 << 17, -1, 194 << 17, 1);
  Func_02005bb4();
  Kibombo_ObjTurn(17, 0x3000);
  Motion_SetModeAndWaitAnimation(17, 3);
  Func_02005b4c((s32)MsgLetUsNow);
  Kibombo_MessagePut(0x4000 + 17, 0);
  Kibombo_SePlay(26);
  Actor_SetSingleChildFlags(Actor_GetById(27), 0);
  Actor_GetById(27)->motionFlags = 0;
  Kibombo_ObjSetPos(Actor_GetById(27), 0x1090000, 0x180000, 172 << 17);
  Kibombo_CamSpeed(0x6666, 0xccc);
  Kibombo_CamMove(132 << 17, -1, 170 << 17, 1);
  Kibombo_ObjSpeed(17, 0x3333, 0x1999);
  Kibombo_ObjWalk(17, 264, 374);
  wait(1);
  Kibombo_TaskAdd(Func_02002eb0, 0x480);
  Kibombo_Wait(20);
  Task_RemoveFar(Func_02002ccc);
  Func_020032e0();
  Kibombo_TaskAdd(Func_02002ccc, 0x480);
  Kibombo_MessagePut(17, 0);
  Kibombo_SePlay(11);
  Func_02003610();
  Kibombo_Wait(160);
  Func_02005b5c(17, 0, 80);
  Kibombo_SePlay(0x240);
  Func_02003640(1, 1);
  Kibombo_Wait(20);
  Func_02005b5c(17, 0, 40);
  Task_RemoveFar(Func_02002ccc);
  wait(1);
  ((struct KibomboEventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x202;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02005bc4(10);
}

void Func_02004294(void) {
  struct KibomboObject **slot;
  struct KibomboObject *object;
  s32 y;

  Actor_SetSingleChildFlags(Actor_GetById(gSequenceParentWork.currentOwner),
                                     0);
  Actor_SetPartAttribute(Actor_GetById(gSequenceParentWork.currentOwner), 15);
  Kibombo_CamMove(-1, -1, -1, 0);
  Func_02005bbc()->motionFlags = 0;
  Actor_SetSingleChildFlags(Actor_GetById(27), 0);
  Actor_GetById(27)->motionFlags = 0;
  Kibombo_ObjSetPos(Actor_GetById(27), 0x1090000, 0x180000, 172 << 17);
  Func_02003640(1, 1);
  wait(1);
  if (gData_02007950 != 0) {
    gData_02007950->y += 0x200000;
    wait(1);
    gData_02007950->motionFlags = 4;
  }
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  KIBOMBO_TASK_ADD(Func_02002ccc, 0x480);
  Kibombo_Wait(40);
  Kibombo_SePlay(161);
  Func_02003640(0, 1);
  Kibombo_Wait(20);
  Kibombo_ObjEmoteWait(17, 0x102);
  Kibombo_Wait(20);
  Func_02005b4c((s32)MsgYouAreStill);
  Kibombo_MessagePut(17, 0);
  slot = &gData_02007950;
  object = *slot;
  if (object != 0) {
    // Lowers the object 0x4000 a frame down to y 0x240000; the loop is
    // entered at its test, which reuses the pointer read for the test above.
    object->motionFlags = 0;
    goto test;
    do {
      Kibombo_ObjArm(17, 0xd000, 0);
      /* FAKEMATCH(temporary): the slot's address reloaded each pass, as the
       * native does; as gData_02007950->y, 4 of 151 instructions differ in
       * tla-en. */
      slot = &gData_02007950;
      (*slot)->y += 0xffffc000;
      wait(1);
      object = *slot;
    test:
      y = object->y;
      /* FAKEMATCH(barrier): an empty asm on the height, so the limit's first
       * half is built before it loads; as object->y > 0x240000, 2 of 151
       * instructions differ in tla-en. */
      __asm__("" : "+l"(y));
    } while (y > 0x240000);
  }
  Motion_SetModeAndWaitAnimation(17, 4);
  Kibombo_MessagePut(17, 0);
  Kibombo_ObjTurn(17, 0x3000);
  Kibombo_MessagePut(0x4000 + 17, 0);
  GameFlag_ClearBit(0x8f7);
  Kibombo_SePlay(11);
  Kibombo_Wait(80);
  ((struct KibomboEventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x201;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02005bc4(11);
}
