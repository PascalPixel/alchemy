#include "kibombo.h"
#include "scene_tables.h"

void Func_020032b0(void);

void Func_02003dec(void) {
  struct KibomboObject *leader;

  Kibombo_CamMove(-1, -1, -1, 0);
  Func_02005bbc()->motionFlags = 0;
  Func_02005b4c((s32)MsgAtLastThe);
  Kibombo_MessagePut(17, 0);
  Kibombo_SePlay(78);
  Kibombo_ObjSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  Kibombo_ObjWalk(gMainRoutineWork.currentOwner, 86, 455);
  Func_02005b0c(26, PIXELS(86), PIXELS(455));
  wait(1);
  Kibombo_ObjSpeed(26, 0xcccc, 0x6666);
  Kibombo_ObjWalk(26, 72, 455);
  Kibombo_ObjArm(26, FACING_EAST, 0);
  Kibombo_ObjArm(gMainRoutineWork.currentOwner, FACING_WEST, 40);
  Kibombo_ObjArm(26, FACING_NORTH + FACING_STEP, 0);
  Kibombo_ObjArm(gMainRoutineWork.currentOwner, FACING_NORTHEAST, 0);
  Kibombo_CamSpeed(0x26666, 0x4ccc);
  Kibombo_CamMove(PIXELS(264), PIXELS(16), PIXELS(420), 1);
  Func_02005bb4();
  Kibombo_Wait(40);
  Kibombo_CamSpeed(0x6666, 0xccc);
  Kibombo_CamMove(PIXELS(264), PIXELS(32), PIXELS(320), 1);
  Func_02005bb4();
  Kibombo_Wait(80);
  Actor_Jump(17, 4, 40);
  Kibombo_MessagePut(17, 0);
  Kibombo_CamSpeed(0x4cccc, 0x9999);
  Kibombo_CamMove(PIXELS(264), PIXELS(32), PIXELS(400), 1);
  Func_02005bb4();
  Kibombo_Wait(20);
  Kibombo_ObjArm(17, FACING_SOUTHEAST + FACING_STEP, 20);
  Kibombo_MessagePut(0x4000 + 17, 0);
  Kibombo_SePlay(11);
  GameFlag_SetBit(0x8f2);
  Kibombo_SePlay(148);
  Actor_GetById(18)->velocityY = PIXELS(4);
  wait(5);
  Actor_GetById(19)->velocityY = PIXELS(4);
  wait(5);
  Actor_GetById(20)->velocityY = PIXELS(4);
  wait(5);
  Actor_GetById(21)->velocityY = PIXELS(4);
  wait(5);
  Actor_GetById(22)->velocityY = PIXELS(4);
  wait(5);
  Actor_GetById(23)->velocityY = PIXELS(4);
  wait(5);
  Actor_GetById(24)->velocityY = PIXELS(4);
  wait(5);
  Actor_GetById(25)->velocityY = PIXELS(4);
  wait(20);
  Kibombo_ObjArm(17, FACING_NORTH + FACING_STEP, 20);
  Func_020032b0();
  Kibombo_Wait(40);
  Kibombo_CamSpeed(0x3333, 0x666);
  Kibombo_CamMove(PIXELS(264), 0, PIXELS(400), 1);
  Func_02005bb4();
  Kibombo_Wait(200);
  Kibombo_CamSpeed(0x4cccc, 0x9999);
  Kibombo_CamMove(PIXELS(111), PIXELS(32), PIXELS(456), 1);
  Func_02005bb4();
  Kibombo_ObjArm(26, FACING_EAST, 0);
  Kibombo_MessagePut(26, 0);
  Kibombo_ObjArm(gMainRoutineWork.currentOwner, FACING_WEST, 20);
  Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
  Actor_SetModeById(26, 4);
  Kibombo_MessagePut(26, 0);
  Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
  Motion_SetModeAndWaitAnimation(26, 3);
  Actor_SetModeById(26, 2);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  if (leader != 0)
    Actor_ResetAndSetPosition(26, ((s16 *)&leader->x)[1],
                              ((s16 *)&leader->z)[1]);
  Actor_WaitForMoveById(26);
  Func_02005b0c(26, 0, 0);
  GameFlag_SetBit(0x8f1);
}
