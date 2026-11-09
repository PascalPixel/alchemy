#include "garoh.h"
#include "call.h"
#include "scene_tables.h"

extern u8 MsgGaroh12[];

void Field_DispatchTypeHandler(s32 type);
void Camera_MoveWithinMapBounds(s32 x, s32 a, s32 z, s32 b);
void Actor_CommitPositionAndActivate(s32 id, s32 dx, s32 dz);
void Motion_SetVarCbAndRefresh(s32 id, s32 value);
void Actor_ArmCallback(s32 id, s32 angle, s32 frames);
void Actor_SetHorizontalPositionWithTerrain(s32 id, s32 x, s32 z);
void Actor_SetHorizontalPositionAndAngleWithTerrain();
void Actor_ArmCallbackAndRefresh(s32 id, s32 angle);
void Func_080d47b4(s32 id, s32 a, s32 b);
void Func_020052b4(void);
void Func_02005304(s32 a, s32 b);
void Func_0200530c(s32 id, s32 a);
void Func_02005314(void);
void Func_0200531c(void);

void Func_02000808(void) {
  s32 flag;
  s32 speed;

  void Func_020007a0(s32 id) {
    Func_02005304(144, 1);
    Func_0200530c(id, 0);
    Field_DispatchTypeHandler(1);
    Func_02005314();
    Func_0200531c();
  }

  void Func_020007d4(s32 id) {
    Func_02005304(144, 1);
    Func_0200530c(id, 0);
    Field_DispatchTypeHandler(3);
    Func_02005314();
    Func_0200531c();
  }

  flag = Value1((s32 (*)())GameFlag_Test, 0x320);
  if (flag == 0) {
    GameFlag_SetBit(0x320);
    Func_020051ac();
    Event_ResolvePendingActions(0);
    Actor_SetSingleChildFlags(Actor_GetById(22), 0);
    speed = 0x4000;
    Call4(Actor_SetHorizontalPositionAndAngleWithTerrain, 22, PIXELS(360),
          PIXELS(199), speed);
    Camera_MoveWithinMapBounds(PIXELS(359), -1, PIXELS(220), 1);
    Func_020052b4();
    Func_020007a0(22);
    Event_Wait(30);
    Call4(Actor_SetHorizontalPositionAndAngleWithTerrain, 21, PIXELS(360),
          PIXELS(199), speed);
    Actor_CommitPositionAndActivate(21, 0, 16);
    Func_020007d4(22);
    Event_SetMessageId((s32)MsgGaroh12);
    Call2((void (*)())Actor_ArmCallbackAndRefresh, 21, 160 << 7);
    Motion_SetVarCbAndRefresh(21, 2);
    Event_Wait(20);
    Call3((void (*)())Actor_ArmCallback, 4, 224 << 8, 0);
    Event_ShowMessageThenWait(21, 0, 10);
    Func_080d47b4(21, 0x102, 50);
    Actor_GetById(21)->actionFlags &= ~1;
    Actor_ArmCallback(21, speed, 0);
    Event_Wait(30);
    Func_020007a0(22);
    Event_Wait(20);
    Call3((void (*)())Actor_CommitPositionAndActivate, 21, 0, -16);
    Actor_GetById(21)->motionFlags = flag;
    Call3((void (*)())Actor_CommitPositionAndActivate, 21, 0, -16);
    Actor_SetHorizontalPositionWithTerrain(21, 0, 0);
    Event_Wait(10);
    Func_020007d4(22);
    Event_Wait(60);
    Actor_SetHorizontalPositionWithTerrain(22, 0, 0);
    Func_020051b4();
  }
}
