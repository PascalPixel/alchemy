/// @file
/// Scene code of overlay 0x681, in ROM order, from s1048's offset up to the next object the overlay links.

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
  struct Actor *object = Actor_Get(9);

  Func_02004e5c();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgGaiaRockInside02);
  FieldCamera_FollowActor(10, 1);
  if (object->script == gData_02005610 || object->script == gData_0200559c)
    FieldActor_RefreshSelectorById(9);
  Func_02000f00();
  Actor_SetAnimation(10, 1);
  Actor_FaceActor(10, gMainRoutineWork.currentOwner, 0);
  Actor_ShowEmote(10, 256, 0);
  FieldEvent_Wait(30);
  Event_ShowMessage(10, 0);
  Actor_WalkTo(10, 496, 456);
  FieldActor_WaitForMoveById(10);
  Actor_FaceDirection(10, FACING_EAST + FACING_STEP, 0);
  FieldEvent_Wait(10);
  Event_ShowMessage(10, 0);
  Actor_ShowEmote(10, 261, 0);
  FieldEvent_Wait(45);
  Event_ShowMessage(10, 0);
  Actor_FaceDirection(10, FACING_NORTH + FACING_STEP, 0);
  FieldEvent_Wait(30);
  FieldCamera_MoveTo(PIXELS(488), -1, PIXELS(392), 1);
  Func_02004f54();
  Event_ShowMessage(10, 0);
  FieldCamera_FollowActor(10, 1);
  Func_02004f54();
  Actor_FaceDirection(10, FACING_EAST + FACING_STEP, 0);
  FieldEvent_Wait(10);
  Event_ShowMessage(10, 0);
  Actor_SetAnimationAndWait(10, 3);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Event_SetFlag(0x9de);
  if (object->script == gData_020056c4 || object->script == gData_02005728) {
    gData_0200646c[2] = gData_020064ec = gData_0200646c[14];
    gData_0200646c[4] = gData_020064f0 = gData_0200646c[16];
  }
  Func_02000fac();
  Func_02004e64();
}

/// Talking to object 10 while its byte at 0x60 is at most 2: once flag 0x9ac is
/// set it says one of two lines, by whether any of flags 0x9bc to 0x9bf is set;
/// before that it turns to the leader and speaks.
void Func_020011c8(void) {
  if (*((u8 *)Actor_Get(10) + 96) > 2)
    return;
  Func_02004e5c();
  FieldEvent_ResolvePendingActions(0);
  if (Event_GetFlag(0x9ac)) {
    if (Event_GetFlag(0x9bc) || Event_GetFlag(0x9bd) || Event_GetFlag(0x9be) ||
        Event_GetFlag(0x9bf))
      FieldEvent_SetMessageId((s32)MsgGaiaRockInside10);
    else
      FieldEvent_SetMessageId((s32)MsgGaiaRockInside06);
    Event_ShowMessage(10, 0);
  } else {
    if (Actor_Get(9)->script == gData_02005610)
      FieldActor_RefreshSelectorById(9);
    Func_02000f00();
    FieldEvent_SetMessageId((s32)MsgGaiaRockInside03);
    Actor_FaceActor(10, gMainRoutineWork.currentOwner, 0);
    FieldEvent_Wait(10);
    Event_ShowMessage(10, 0);
    Actor_SetAnimationAndWait(10, 3);
    FieldEvent_Wait(10);
    Func_02000fac();
  }
  Func_02004e64();
}
