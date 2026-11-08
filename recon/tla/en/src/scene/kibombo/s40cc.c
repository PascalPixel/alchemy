#include "../../../../../../games/tla/src/scene/kibombo/kibombo.h"
#include "scene_tables.h"

void Func_02002eb0(void);
void Func_020032e0(void);
void Func_02003610(void);

void Func_020040cc(void) {
  Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  Actor_SetPartAttribute(Actor_GetById(gMainRoutineWork.currentOwner), 15);
  Kibombo_CamMove(-1, -1, -1, 0);
  Func_02005bbc()->motionFlags = 0;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Kibombo_TaskAdd(Func_02002ccc, 0x480);
  FieldEvent_Wait(40);
  Func_02005c9c(78);
  Kibombo_CamSpeed(SPLIT_CONSTANT(0xcc00, 204), 0x1999);
  Kibombo_CamMove(PIXELS(264), -1, PIXELS(388), 1);
  Func_02005bb4();
  Kibombo_ObjTurn(17, FACING_SOUTHEAST + FACING_STEP);
  Motion_SetModeAndWaitAnimation(17, 3);
  Func_02005b4c((s32)MsgLetUsNow);
  Func_02005b64(0x4000 + 17, 0);
  Func_02005c9c(26);
  Actor_SetSingleChildFlags(Actor_GetById(27), 0);
  Actor_GetById(27)->motionFlags = 0;
  Actor_SetPositionAndResetMotion(Actor_GetById(27), PIXELS(265), PIXELS(24),
                                  PIXELS(344));
  Func_02005ba4(SPLIT_CONSTANT(0x6600, 102), 0xccc);
  Camera_MoveWithinMapBounds(PIXELS(264), -1, PIXELS(340), 1);
  Kibombo_ObjSpeed(17, 0x3333, 0x1999);
  Actor_SetPositionAndReset(17, 264, 374);
  wait(1);
  Kibombo_TaskAdd(Func_02002eb0, 0x480);
  FieldEvent_Wait(20);
  Task_RemoveFar(Func_02002ccc);
  Func_020032e0();
  Kibombo_TaskAdd(Func_02002ccc, 0x480);
  Func_02005b64(17, 0);
  Func_02005c9c(11);
  Func_02003610();
  FieldEvent_Wait(160);
  Func_02005b5c(17, 0, 80);
  Func_02005c9c(0x240);
  Func_02003640(1, 1);
  FieldEvent_Wait(20);
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

  Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  Actor_SetPartAttribute(Actor_GetById(gMainRoutineWork.currentOwner), 15);
  Kibombo_CamMove(-1, -1, -1, 0);
  Func_02005bbc()->motionFlags = 0;
  Actor_SetSingleChildFlags(Actor_GetById(27), 0);
  Actor_GetById(27)->motionFlags = 0;
  Actor_SetPositionAndResetMotion(Actor_GetById(27), PIXELS(265), PIXELS(24),
                                  PIXELS(344));
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
  FieldEvent_Wait(40);
  Func_02005c9c(161);
  Func_02003640(0, 1);
  FieldEvent_Wait(20);
  Func_02005b9c(17, 0x102);
  FieldEvent_Wait(20);
  Func_02005b4c((s32)MsgYouAreStill);
  Func_02005b64(17, 0);
  slot = &gData_02007950;
  object = *slot;
  if (object != 0) {
    // Lowers the object 0x4000 a frame down to y 0x240000; the loop is
    // entered at its test, which reuses the pointer read for the test above.
    object->motionFlags = 0;
    goto test;
    do {
      Kibombo_ObjArm(17, FACING_NORTH + FACING_STEP, 0);
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
  Func_02005b64(17, 0);
  Kibombo_ObjTurn(17, FACING_SOUTHEAST + FACING_STEP);
  Func_02005b64(0x4000 + 17, 0);
  GameFlag_ClearBit(SPLIT_CONSTANT(0x7f8, 255));
  Func_02005c9c(11);
  FieldEvent_Wait(80);
  ((struct KibomboEventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x201;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02005bc4(11);
}
