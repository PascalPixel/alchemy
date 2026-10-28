#include "kibombo.h"
#include "scene_tables.h"

void Func_02005a4c(void);
void Func_02005b14(s32 actor, s32 x, s32 z, s32 angle);
void Func_02005bd4(s32 a, s32 b);
void Func_02005bdc(s32 a, s32 b);
void Func_02005be4(s32 frames);

void Func_02004cf4(void) {
  struct KibomboEventWork *work;
  struct KibomboObject *leader;

  Func_02005a9c();
  Func_02005c3c(0);
  Kibombo_CamMove(-1, -1, -1, 0);
  Kibombo_CamMove(PIXELS(384), -1, PIXELS(601), 0);
  wait(1);
  Func_02005b14(gMainRoutineWork.currentOwner, PIXELS(370), PIXELS(636),
                FACING_NORTH);
  Func_02005a4c();
  wait(1);
  ((struct KibomboEventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x100;
  Func_02005bdc(0, 0);
  Func_02005bd4(0x10002, 0);
  ((struct KibomboEventWork *)Ram_HeapSlots->eventWork)->field1b4 = 60;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Kibombo_Wait(40);
  Kibombo_ObjArm(10, FACING_NORTHWEST + FACING_STEP, 20);
  Kibombo_ObjArm(10, FACING_NORTH + FACING_STEP, 20);
  Motion_SetModeAndWaitAnimation(10, 3);
  Actor_SetModeById(9, 3);
  Motion_SetModeAndWaitAnimation(8, 3);
  Kibombo_ObjArm(10, FACING_NORTHWEST + FACING_STEP, 20);
  Kibombo_ObjArm(10, FACING_NORTH + FACING_STEP, 20);
  Actor_GetById(10)->velocityY = PIXELS(3);
  Kibombo_Wait(40);
  Actor_SetModeById(9, 3);
  Motion_SetModeAndWaitAnimation(8, 3);
  Func_02005bd4(0x10000, 0);
  Func_02005be4(40);
  wait(60);
  Motion_SetModeAndWaitAnimation(10, 3);
  Func_02005b4c((s32)MsgRightWellI);
  Kibombo_MessagePut(10, 0);
  Motion_SetModeAndWaitAnimation(9, 3);
  Kibombo_MessagePut(9, 0);
  Kibombo_ObjArm(10, FACING_EAST, 0);
  Kibombo_ObjArm(gMainRoutineWork.currentOwner, FACING_WEST, 40);
  Kibombo_ObjArm(10, FACING_NORTH + FACING_STEP, 0);
  Kibombo_ObjArm(gMainRoutineWork.currentOwner, FACING_NORTH, 10);
  Actor_SetModeById(10, 3);
  Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
  Actor_Jump(8, 4, 10);
  Kibombo_MessagePut(0x2008, 0);
  Kibombo_ObjTurn(9, FACING_EAST);
  Kibombo_ObjEmote(8, 259, 20);
  Kibombo_MessagePut(0x2008, 0);
  Kibombo_ObjEmote(9, 256, 20);
  Kibombo_MessagePut(9, 0);
  Kibombo_ObjTurn(8, FACING_WEST);
  Actor_SetModeById(8, 4);
  Kibombo_MessagePut(0x2008, 0);
  Kibombo_ObjTurn(8, FACING_SOUTH + FACING_STEP);
  Kibombo_MessagePut(0x2008, 0);
  Actor_SetModeById(8, 4);
  Kibombo_MessagePut(0x2008, 0);
  Kibombo_ObjArm(9, FACING_NORTH + FACING_STEP, 0);
  Kibombo_ObjSpeed(8, 0x19999, 0xcccc);
  Kibombo_ObjWalk(8, 364, 576);
  Kibombo_ObjArm(8, FACING_NORTHWEST + FACING_STEP, 0);
  Kibombo_ObjEmote(9, 261, 80);
  Kibombo_MessagePut(9, 0);
  Kibombo_ObjTurn(9, FACING_SOUTHEAST + FACING_STEP);
  Kibombo_MessagePut(9, 0);
  Actor_SetModeById(9, 4);
  Kibombo_MessagePut(9, 0);
  Kibombo_MessagePut(9, 0);
  Motion_SetModeAndWaitAnimation(10, 3);
  Kibombo_MessagePut(10, 0);
  Kibombo_ObjTurn(10, FACING_EAST);
  Kibombo_MessagePut(10, 0);
  Kibombo_ObjTurn(gMainRoutineWork.currentOwner, FACING_WEST);
  Actor_SetModeById(10, 3);
  Kibombo_MessagePut(10, 0);
  Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
  Kibombo_ObjSpeed(10, 0xcccc, 0x6666);
  Actor_SetModeById(10, 2);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  if (leader != 0)
    Actor_ResetAndSetPosition(10, ((s16 *)&leader->x)[1],
                              ((s16 *)&leader->z)[1]);
  Actor_WaitForMoveById(10);
  Func_02005b0c(10, 0, 0);
  work = Ram_HeapSlots->eventWork;
  work->field1b4 = 16;
  work->screenEffect = 0x209;
  GameFlag_SetBit(0x8ef);
  Func_02005aa4();
}
