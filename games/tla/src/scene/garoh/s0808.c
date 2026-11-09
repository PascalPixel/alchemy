#include "garoh.h"
#include "scene_tables.h"
#include "event_inline.h"

extern u8 MsgGaroh12[];

void Field_DispatchTypeHandler(s32 type);
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

  flag = Event_GetFlag(0x320);
  if (flag == 0) {
    Event_SetFlag(0x320);
    Func_020051ac();
    FieldEvent_ResolvePendingActions(0);
    Actor_SetSingleChildFlags(Actor_Get(22), 0);
    speed = 0x4000;
    Actor_SetPositionAndFacing(22, PIXELS(360), PIXELS(199), speed);
    FieldCamera_MoveTo(PIXELS(359), -1, PIXELS(220), 1);
    Func_020052b4();
    Func_020007a0(22);
    FieldEvent_Wait(30);
    Actor_SetPositionAndFacing(21, PIXELS(360), PIXELS(199), speed);
    Actor_WalkByAndWait(21, 0, 16);
    Func_020007d4(22);
    FieldEvent_SetMessageId((s32)MsgGaroh12);
    Actor_FaceDirectionAndWait(21, 160 << 7);
    Actor_RunRepeatedMotion(21, 2);
    FieldEvent_Wait(20);
    Actor_FaceDirection(4, 224 << 8, 0);
    FieldEvent_ShowMessageThenWait(21, 0, 10);
    FieldActor_ShowEmote(21, 0x102, 50);
    Actor_Get(21)->actionFlags &= ~1;
    Actor_FaceDirection(21, speed, 0);
    FieldEvent_Wait(30);
    Func_020007a0(22);
    FieldEvent_Wait(20);
    Actor_WalkByAndWait(21, 0, -16);
    Actor_Get(21)->motionFlags = flag;
    Actor_WalkByAndWait(21, 0, -16);
    Actor_SetPosition(21, 0, 0);
    FieldEvent_Wait(10);
    Func_020007d4(22);
    FieldEvent_Wait(60);
    Actor_SetPosition(22, 0, 0);
    Func_020051b4();
  }
}
