#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "event_inline.h"
void Func_02001be4(void);
void Func_02001bec(void);
extern u8 MsgAlhafra28[];
extern u8 gMainRoutineWork[];

void Func_02001958(s32 a0) {
  s32 t22;
  s32 t25;
  s32 t37;
  s32 t40;

  Event_SetFlag(0x8b4);
  Func_02001be4();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgAlhafra28);
  if (a0 == 1) {
    FieldEvent_Wait(10);
    Actor_WalkToAndWait(FIELD_AT(gMainRoutineWork, s32, 532), 168, 736);
    Actor_FaceDirection(FIELD_AT(gMainRoutineWork, s32, 532), 0, 0);
  }
  Actor_ShowEmote(0x19, 0x102, 0x1e);
  FieldEvent_ShowMessageThenWait(0x19, 0, 0xa);
  Actor_SetAnimationAndWait(25, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x19, 0, 0xa);
  Actor_SetAnimationAndWait(25, 4);
  FieldEvent_ShowMessageThenWait(0x19, 0, 0xa);
  Actor_RunRepeatedMotion(25, 2);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(0x19, 0, 0xa);
  if (a0 != 1) {
  } else {
    Actor_CommitPositionAndActivate(25, -8, 0);
    Event_Wait(15);
    Actor_StartRepeatedMotion(25, 3);
    Func_080d489c(0x19, 0x102);
    Event_Wait(40);
    t22 = ((s32)Actor_GetById(25));
    FIELD_AT((t22 + 0x5a), u8, 0) = (0xfe & FIELD_AT((t22 + 0x5a), u8, 0));
    Actor_SetPositionAndReset(25, 192, 736);
    Event_Wait(1);
    t25 = ((s32)Actor_GetById(25));
    FIELD_AT((t25 + 0x5a), u8, 0) = (1 | FIELD_AT((t25 + 0x5a), u8, 0));
    Event_Wait(10);
    Event_ShowMessageThenWait(0x19, 0, 0xa);
    {
      s32 b = 0x107, c = 0x1e;
      Func_080d47b4(0x19, b, c);
    }
    Event_OpenMessage(0x19, 0);
    if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
      Event_Wait(30);
      Motion_SetVarCbAndRefresh(25, 2);
      Event_Wait(20);
      Event_ShowMessageThenWait(0x19, 0, 0xa);
      FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1c4), u16, 0) =
          (FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1c4), u16, 0) + 1);
    } else {
      Event_Wait(30);
      FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1c4), u16, 0) =
          (FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1c4), u16, 0) + 1);
      Event_ShowMessageThenWait(0x19, 0, 0xa);
    }
    t37 = ((s32)Actor_GetById(4));
    FIELD_AT((t37 + 0x5a), u8, 0) = (0xfe & FIELD_AT((t37 + 0x5a), u8, 0));
    Actor_WalkToAndWait(4, 152, 752);
    Event_Wait(1);
    t40 = ((s32)Actor_GetById(4));
    FIELD_AT((t40 + 0x5a), u8, 0) = (1 | FIELD_AT((t40 + 0x5a), u8, 0));
    Actor_FaceDirection(4, 0xe000, 0);
    Event_Wait(20);
    Motion_SetModeAndWaitAnimation(25, 3);
    Event_Wait(10);
    Actor_ArmCallback(25, 0xe000, 0);
    Event_Wait(70);
    {
      s32 b = 0x100, c = 0x32;
      Func_080d47b4(0x19, b, c);
    }
    Event_ShowMessageThenWait(0x19, 0, 0xa);
  }
  Actor_WalkToAndWait(25, 184, 728);
  Actor_WalkByAndWait(25, 0, -110);
  if (a0 == 0) {
    Event_Wait(50);
    Actor_SetPosition(25, 0x1080000, 0x2880000);
    Actor_CommitPositionAndActivate(25, 0, 32);
    Actor_CommitPositionAndActivate(25, 150, 0);
  }
  Actor_SetHorizontalPositionWithTerrain(25, 0, 0);
  Func_02001bec();
}
