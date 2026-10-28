#include "types.h"
#include "actor.h"
#include "sequence_parent_work.h"
#include "text/msg_ids.h"
#include "call.h"

extern s32 gData_0200559c[];
extern s32 gData_02005610[];
extern s32 gData_020056c4[];
extern s32 gData_02005728[];
extern s32 gData_0200646c[];
extern s32 gData_020064ec;
extern s32 gData_020064f0;

void GameFlag_SetBit(s32 flag);
struct Actor *Actor_GetById(s32 id);
void Camera_FollowActor(s32 id, s32 value);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Motion_SetModeAndWaitAnimation(s32 id, s32 mode);
void Actor_SetAngleToward(s32 id, s32 target, s32 frames);
void Actor_ResetAndSetPositionInMode2(s32 id, s32 x, s32 z);
void Actor_WaitForMoveById(s32 id);
void Actor_ArmCallback(s32 id, s32 angle, s32 frames);
void FieldEvent_Wait(s32 frames);
void Func_02000f00(void);
void Func_02000fac(void);
void Func_02004e5c(void);
void Func_02004e64(void);
void Func_02004f0c(s32 message);
void Func_02004f14(s32 id, s32 value);
s32 Func_02004f2c(s32 id, s32 emote, s32 frames);
void Func_02004f54(void);
void Func_02004fcc(s32 value);

/// Object 10 turns to the leader, talks and walks to its place while the camera
/// follows; flag 0x9de is set, and object 9's script decides whether two
/// words of gData_0200646c are copied into place.
void Func_02001048(void) {
  struct Actor *object = Actor_GetById(9);

  Func_02004e5c();
  Func_02004fcc(0);
  Func_02004f0c((s32)MsgGaiaRockInside02);
  Camera_FollowActor(10, 1);
  if (object->script == gData_02005610 || object->script == gData_0200559c)
    Actor_RefreshSelectorById(9);
  Func_02000f00();
  Actor_SetModeById(10, 1);
  Actor_SetAngleToward(10, gSequenceParentWork.currentOwner, 0);
  Value3(Func_02004f2c, 10, 256, 0);
  Call1(FieldEvent_Wait, 30);
  Call2(Func_02004f14, 10, 0);
  Call3(Actor_ResetAndSetPositionInMode2, 10, 496, 456);
  Call1(Actor_WaitForMoveById, 10);
  Call3(Actor_ArmCallback, 10, 0x1000, 0);
  Call1(FieldEvent_Wait, 10);
  Call2(Func_02004f14, 10, 0);
  Value3(Func_02004f2c, 10, 261, 0);
  Call1(FieldEvent_Wait, 45);
  Call2(Func_02004f14, 10, 0);
  Call3(Actor_ArmCallback, 10, 0xd000, 0);
  Call1(FieldEvent_Wait, 30);
  Call4(Camera_MoveWithinMapBounds, 0x1e80000, -1, 0x1880000, 1);
  Func_02004f54();
  Call2(Func_02004f14, 10, 0);
  Call2(Camera_FollowActor, 10, 1);
  Func_02004f54();
  Call3(Actor_ArmCallback, 10, 0x1000, 0);
  Call1(FieldEvent_Wait, 10);
  Call2(Func_02004f14, 10, 0);
  Call2(Motion_SetModeAndWaitAnimation, 10, 3);
  Call4(Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  Call1(GameFlag_SetBit, 0x9de);
  if (object->script == gData_020056c4 || object->script == gData_02005728) {
    gData_0200646c[2] = gData_020064ec = gData_0200646c[14];
    gData_0200646c[4] = gData_020064f0 = gData_0200646c[16];
  }
  Func_02000fac();
  Func_02004e64();
}
