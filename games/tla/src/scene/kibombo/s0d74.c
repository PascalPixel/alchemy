#include "kibombo.h"
#include "scene_tables.h"

void Func_02005c7c(void);
void Func_02002eb0(void);
void Func_020032b0(void);
void Func_020032c8(void);
void Func_020032e0(void);
void Func_02003610(void);
void Actor_ResetAndSetPositionInMode2(s32 actor, s32 x, s32 z);

void Func_02000d74(void) {
  Func_020032c8();
  Func_02005c9c(0);
  Func_02005b64(17, 0);
  Kibombo_ObjArm(7, FACING_EAST, 0);
  Kibombo_ObjArm(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  Kibombo_ObjArm(26, FACING_EAST, 0);
  Kibombo_ObjArm(5, FACING_EAST, 0);
  Kibombo_ObjArm(6, FACING_EAST, 10);
  Func_02005ba4(0x4cccc, 0x9999);
  Kibombo_CamMove(PIXELS(264), -1, PIXELS(388), 1);
  Func_02005bb4();
  FieldEvent_Wait(40);
  Kibombo_ObjArm(17, FACING_SOUTHEAST + FACING_STEP, 20);
  Motion_SetModeAndWaitAnimation(17, 3);
  Func_02005b64(0x4011, 0);
  Func_02005c9c(26);
  Actor_SetSingleChildFlags(Actor_GetById(27), 0);
  Actor_GetById(27)->motionFlags = 0;
  Kibombo_ObjSetPos(Actor_GetById(27), PIXELS(265), PIXELS(24), PIXELS(344));
  Func_02005ba4(0x6666, 0xccc);
  Kibombo_CamMove(PIXELS(264), -1, PIXELS(340), 1);
  Kibombo_ObjSpeed(17, 0x3333, 0x1999);
  Kibombo_ObjWalk(17, 264, 374);
  Kibombo_TaskAdd(Func_02002eb0, 0x480);
  FieldEvent_Wait(20);
  Task_RemoveFar(Func_02002ccc);
  Func_020032e0();
  KIBOMBO_TASK_ADD(Func_02002ccc, 0x480);
  Func_02005b64(7, 0);
  Func_02005b64(17, 0);
  Func_02005c9c(11);
  Func_02003610();
  FieldEvent_Wait(160);
  Func_02005b5c(17, 0, 80);
  Func_02005c9c(0x240);
  Func_02003640(1, 1);
  FieldEvent_Wait(20);
  Func_02005b5c(17, 0, 80);
  Func_02005c9c(161);
  Func_02003640(0, 1);
  FieldEvent_Wait(20);
  Kibombo_ObjEmoteWait(17, 0x102);
  FieldEvent_Wait(20);
  Func_02005b5c(17, 0, 20);
  Task_RemoveFar(Func_02002eb0);
  wait(1);
  Func_02005b0c(27, 0, 0);
  Kibombo_ObjArm(17, FACING_NORTH + FACING_STEP, 0);
  if (gData_02007950 != 0) {
    if (gData_02007950->y > 0x240000) {
    wait:
      Kibombo_ObjArm(17, FACING_NORTH + FACING_STEP, 0);
      gData_02007950->y += 0xffffc000;
      wait_(1);
      if (gData_02007950->y > 0x240000)
        goto wait;
    }
  }
  Kibombo_ObjTurn(17, FACING_SOUTHEAST + FACING_STEP);
  Func_02005b64(17, 0);
  Func_02005ba4(0x4cccc, 0x9999);
  Kibombo_CamMove(PIXELS(264), PIXELS(16), PIXELS(420), 1);
  Kibombo_ObjSpeed(17, 0x19999, 0xcccc);
  Kibombo_ObjWalk(17, 264, 392);
  FieldEvent_Wait(20);
  Func_02005b64(0x8011, 0);
  Func_02005c9c(148);
  Func_02005c9c(11);
  Func_020032b0();
  FieldEvent_Wait(80);
  Kibombo_CamMove(PIXELS(88), PIXELS(32), PIXELS(392), 1);
  Func_02005bb4();
  FieldEvent_Wait(40);
  Func_02005b64(7, 0);
  Kibombo_ObjEmoteWait(7, 0x102);
  Func_02005b64(7, 0);
  Motion_SetModeAndWaitAnimation(7, 4);
  Func_02005b64(7, 0);
  Actor_SetSpritePriority(7, 2);
  Kibombo_ObjSpeed(7, 0x19999, 0xcccc);
  Kibombo_ObjWalk(7, 152, 386);
  Kibombo_ObjArm(7, FACING_NORTHEAST, 40);
  Kibombo_ObjArm(7, FACING_SOUTHEAST, 20);
  Kibombo_ObjArm(7, FACING_NORTHEAST, 20);
  Kibombo_ObjArm(7, FACING_SOUTHEAST, 40);
  Func_02005b64(7, 0);
  Kibombo_ObjWalk(7, 124, 356);
  Kibombo_ObjArm(7, FACING_NORTHEAST, 20);
  Kibombo_ObjArm(7, FACING_NORTHWEST, 20);
  Kibombo_ObjArm(7, FACING_NORTHEAST, 20);
  Func_02005b64(0x2007, 0);
  Kibombo_ObjArm(5, FACING_SOUTHEAST, 0);
  Func_02005b64(5, 0);
  Kibombo_ObjArm(gMainRoutineWork.currentOwner, FACING_NORTHEAST, 40);
  Kibombo_ObjSpeed(26, 0xcccc, 0x6666);
  Actor_ResetAndSetPositionInMode2(26, 104, 368);
  FieldEvent_Wait(20);
  Func_02005ba4(0xcccc, 0x1999);
  Kibombo_CamMove(PIXELS(111), -1, PIXELS(356), 1);
  Actor_WaitForMoveById(26);
  Actor_SetModeById(26, 1);
  Kibombo_ObjArm(26, FACING_NORTH + FACING_STEP, 40);
  Func_02005b94(26, 0x100, 20);
  Func_02005b64(26, 0);
  Kibombo_ObjArm(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  Kibombo_ObjArm(5, FACING_NORTH, 0);
  Kibombo_ObjArm(6, FACING_NORTH, 0);
  Kibombo_ObjArm(26, FACING_SOUTH + FACING_STEP, 20);
  Kibombo_MessageOpen(0x601a, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    Kibombo_MessagePut(0x601a, 0);
    ((struct KibomboEventWork *)Ram_HeapSlots->eventWork)->field1c4++;
  } else {
    ((struct KibomboEventWork *)Ram_HeapSlots->eventWork)->field1c4++;
    Kibombo_MessagePut(0x601a, 0);
    Func_02005c7c();
  }
}
