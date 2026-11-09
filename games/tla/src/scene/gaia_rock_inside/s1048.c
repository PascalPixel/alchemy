#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "text/msg_ids.h"
#include "scene_tables.h"
#include "event_inline.h"

extern s32 gData_0200559c[];
extern s32 gData_02005610[];
extern s32 gData_020056c4[];
extern s32 gData_02005728[];
extern s32 gData_0200646c[];
extern s32 gData_020064ec;
extern s32 gData_020064f0;

void Func_02000f00(void);
void Func_02000fac(void);
void Func_02004e5c(void);
void Func_02004e64(void);
void Func_02004f54(void);

/// Object 10 turns to the leader, talks and walks to its place while the camera
/// follows; flag 0x9de is set, and object 9's script decides whether two
/// words of gData_0200646c are copied into place.
void Func_02001048(void) {
  struct Actor *object = Actor_GetById(9);

  Func_02004e5c();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgGaiaRockInside02);
  Camera_FollowActor(10, 1);
  if (object->script == gData_02005610 || object->script == gData_0200559c)
    Actor_RefreshSelectorById(9);
  Func_02000f00();
  Actor_SetModeById(10, 1);
  Actor_SetAngleToward(10, gMainRoutineWork.currentOwner, 0);
  Func_080d47b4(10, 256, 0);
  Event_Wait(30);
  Event_ShowMessageAndWait(10, 0);
  Actor_ResetAndSetPositionInMode2(10, 496, 456);
  Actor_WaitForMoveById(10);
  Actor_FaceDirection(10, FACING_EAST + FACING_STEP, 0);
  Event_Wait(10);
  Event_ShowMessageAndWait(10, 0);
  Func_080d47b4(10, 261, 0);
  Event_Wait(45);
  Event_ShowMessageAndWait(10, 0);
  Actor_ArmCallback(10, FACING_NORTH + FACING_STEP, 0);
  Event_Wait(30);
  FieldCamera_MoveTo(PIXELS(488), -1, PIXELS(392), 1);
  Func_02004f54();
  Event_ShowMessageAndWait(10, 0);
  Camera_FollowActor(10, 1);
  Func_02004f54();
  Actor_ArmCallback(10, FACING_EAST + FACING_STEP, 0);
  Event_Wait(10);
  Event_ShowMessageAndWait(10, 0);
  Motion_SetModeAndWaitAnimation(10, 3);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  GameFlag_SetBit(0x9de);
  if (object->script == gData_020056c4 || object->script == gData_02005728) {
    gData_0200646c[2] = gData_020064ec = gData_0200646c[14];
    gData_0200646c[4] = gData_020064f0 = gData_0200646c[16];
  }
  Func_02000fac();
  Func_02004e64();
}
