#include "types.h"
#include "ram_buffer.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
#include "event_inline.h"
s32 Func_02005814();
s32 Event_StartScreenTransition();
s32 Event_WaitTransitionFrames();
s32 Func_02005924();
s32 Func_02001ac4();
s32 Func_02003a6c();
s32 Func_0200598c();
s32 Func_02003c58();
s32 Event_FinishScreenTransition();
s32 Func_0200581c();
extern u8 MsgWeDidItWe[];
extern u8 gData_02005ff8[];
extern u8 gData_02006088[];
extern u8 gData_02005ff0[];
extern u8 gData_020061e0[];

static __inline__ void W_Func_020058ec(s32 a0, s32 a1) {
  Actor_ArmCallbackAndRefresh(a0, a1);
}
static __inline__ void W_Func_020058fc(s32 a0, s32 a1, s32 a2) {
  Func_080d47b4(a0, a1, a2);
}
static __inline__ void W_Func_02005914(s32 a0, s32 a1) {
  Camera_SetSpeedParameters(a0, a1);
}
static __inline__ void W_Actor_ArmCallback(s32 a0, s32 a1, s32 a2) {
  Actor_ArmCallback(a0, a1, a2);
}
static __inline__ void W_Actor_EnableActionAndSetCallback(s32 a0, s32 a1) {
  Actor_SetMovementTypeById(a0, a1);
}

// Every Actor_SetModeById of mode 3 or 4 that is followed by its wait goes
// through this macro: its loop notes put the argument loads in the listing's
// order, and all 20 uses match through it.
#define SET_MODE_AND_WAIT(id, mode, frames)                                    \
  do {                                                                         \
    Actor_SetAnimation(id, mode);                                              \
    FieldEvent_Wait(frames);                                                   \
  } while (0)

void Func_02002ae4(void) {
  /* FAKEMATCH(wrapper): calls forwarded through inlines (call.h's Call and
   * Value, this file's own); as direct calls, 652 of 1539 instructions differ
   * in tla-en. */
  s32 fast;
  s32 slow;
  s32 mid;
  s32 t2;
  s32 t3;
  s32 t4;
  s32 t5;
  s32 t6;
  s32 t7;
  s32 t8;
  s32 t9;
  s32 t10;
  s32 t11;
  s32 t12;
  s32 t13;
  s32 t67;
  s32 t166;
  s32 t167;
  s32 t168;
  s32 t169;
  s32 t184;
  s32 t185;
  s32 t186;
  s32 t269;
  s32 t292;
  s32 t295;
  s32 t298;
  s32 t325;
  s32 t330;
  s32 t358;
  s32 t361;
  s32 t364;
  s32 t367;
  s32 t381;
  s32 t382;
  s32 work;

  Func_02005814();
  FieldEvent_ResolvePendingActions(0);
  t2 = (s32)Actor_Get(0);
  FIELD_AT((t2 + 0x23), u8, 0) = (FIELD_AT((t2 + 0x23), u8, 0) | 1);
  t3 = (s32)Actor_Get(1);
  FIELD_AT((t3 + 0x23), u8, 0) = (FIELD_AT((t3 + 0x23), u8, 0) | 1);
  t4 = (s32)Actor_Get(2);
  FIELD_AT((t4 + 0x23), u8, 0) = (FIELD_AT((t4 + 0x23), u8, 0) | 1);
  t5 = (s32)Actor_Get(3);
  FIELD_AT((t5 + 0x23), u8, 0) = (FIELD_AT((t5 + 0x23), u8, 0) | 1);
  t6 = (s32)Actor_Get(4);
  FIELD_AT((t6 + 0x23), u8, 0) = (FIELD_AT((t6 + 0x23), u8, 0) | 1);
  t7 = (s32)Actor_Get(5);
  FIELD_AT((t7 + 0x23), u8, 0) = (FIELD_AT((t7 + 0x23), u8, 0) | 1);
  t8 = (s32)Actor_Get(7);
  FIELD_AT((t8 + 0x23), u8, 0) = (FIELD_AT((t8 + 0x23), u8, 0) | 1);
  t9 = (s32)Actor_Get(6);
  FIELD_AT((t9 + 0x23), u8, 0) = (FIELD_AT((t9 + 0x23), u8, 0) | 1);
  t10 = (s32)Actor_Get(0x1e);
  FIELD_AT((t10 + 0x23), u8, 0) = (FIELD_AT((t10 + 0x23), u8, 0) | 1);
  t11 = (s32)Actor_Get(0x1b);
  FIELD_AT((t11 + 0x23), u8, 0) = (FIELD_AT((t11 + 0x23), u8, 0) | 1);
  t12 = (s32)Actor_Get(0x1c);
  FIELD_AT((t12 + 0x23), u8, 0) = (FIELD_AT((t12 + 0x23), u8, 0) | 1);
  t13 = (s32)Actor_Get(0x1d);
  FIELD_AT((t13 + 0x23), u8, 0) = (1 | FIELD_AT((t13 + 0x23), u8, 0));
  fast = 0xa000;
  Actor_SetPositionAndFacing(4, 0x2080000, 0x1300000, fast);
  Actor_SetPositionAndFacing(7, 0x2180000, 0x1200000, fast);
  slow = 0x2000;
  Actor_SetPositionAndFacing(0x1b, 0x1e00000, 0x1280000, slow);
  Actor_SetPositionAndFacing(0x1c, 0x1f00000, 0x1180000, slow);
  Actor_SetPositionAndFacing(5, 0x2200000, 0x1300000, fast);
  mid = 0xc000;
  Actor_SetPositionAndFacing(6, 0x2300000, 0x1200000, mid);
  Actor_SetPositionAndFacing(0x1d, 0x1e00000, 0x1680000, mid);
  Actor_SetPositionAndFacing(0x1e, 0x2500000, 0x1880000, 0);
  Actor_SetAnimation(0x1b, 0xa);
  Actor_SetAnimation(0x1c, 9);
  W_Func_02005914(0x10000, slow);
  FieldCamera_MoveTo(0x2080000, 0x680000, 0x1380000, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetMessageId((s32)MsgWeDidItWe);
  SET_MODE_AND_WAIT(7, 4, 0x21);
  FieldEvent_ShowMessageThenWait(7, 0, 0);
  SET_MODE_AND_WAIT(5, 3, 0x14);
  FieldEvent_Wait(0xa);
  FieldEvent_ShowMessageThenWait(5, 0, 0);
  Actor_FaceDirectionAndWait(6, 0x6000);
  FieldEvent_Wait(0xa);
  FieldEvent_ShowMessageThenWait(6, 0, 0);
  Actor_RunRepeatedMotion(0x1b, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(0x1b, 0, 0);
  Actor_RunRepeatedMotion(0x1c, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(0x1c, 0, 0);
  Actor_FaceDirection(4, 0, 0);
  Actor_FaceDirectionAndWait(5, 0x8000);
  FieldEvent_Wait(0x28);
  W_Actor_ArmCallback(4, fast, 0);
  Actor_FaceDirectionAndWait(5, fast);
  FieldEvent_Wait(0x14);
  SET_MODE_AND_WAIT(5, 4, 0x21);
  FieldEvent_ShowMessageThenWait(5, 0, 0);
  Actor_RunRepeatedMotion(0x1b, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(0x1b, 0, 0);
  Actor_FaceDirection(4, 0xe000, 0);
  Actor_FaceDirection(7, 0x6000, 0);
  Actor_FaceDirection(5, 0xe000, 0);
  Actor_FaceDirectionAndWait(6, 0x6000);
  FieldEvent_Wait(0x1e);
  Actor_RunRepeatedMotion(0x1c, 2);
  Event_OpenMessage(0x1c, 0);
  Actor_FaceDirection(6, 0x6000, 0);
  Actor_FaceDirectionAndWait(5, 0x8000);
  Actor_FaceDirection(4, 0x8000, 0);
  t67 = FieldEvent_AskYesNoWithActorAnimation(4, 0);
  if (t67 == 0) {
    FieldEvent_ShowMessageThenWait(0x1d, 0, 0);
    FIELD_AT(((s32)Ram_HeapSlots->eventWork + 0x1c4), u16, 0) =
        (FIELD_AT(((s32)Ram_HeapSlots->eventWork + 0x1c4), u16, 0) + 1);
  } else {
    FIELD_AT(((s32)Ram_HeapSlots->eventWork + 0x1c4), u16, 0) =
        (FIELD_AT(((s32)Ram_HeapSlots->eventWork + 0x1c4), u16, 0) + 1);
    FieldEvent_ShowMessageThenWait(0x1d, 0, 0);
  }
  Event_PlaySound(0x4e);
  FieldCamera_MoveTo(0x2080000, 0x680000, 0x1600000, 1);
  Func_02005924();
  FieldEvent_Wait(0xa);
  Event_PlaySound(0x27);
  W_Func_020058fc(5, 0x100, 0x1e);
  Actor_FaceDirection(5, 0x6000, 0);
  FieldEvent_ShowMessageThenWait(5, 0, 0);
  FieldCamera_MoveTo(0x2080000, 0x680000, 0x1380000, 1);
  Actor_WalkToAndWait(0x1d, 0x1e0, 0x140);
  SET_MODE_AND_WAIT(0x1d, 4, 0x21);
  FieldEvent_ShowMessageThenWait(0x1d, 0, 0);
  Actor_FaceDirectionAndWait(0x1d, 0);
  FieldEvent_ShowMessageThenWait(0x1d, 0, 0);
  W_Func_020058fc(7, 0x109, 0);
  W_Func_020058fc(5, 0x109, 0);
  W_Func_020058fc(6, 0x109, 0);
  W_Func_020058fc(4, 0x109, 0x1e);
  Actor_FaceDirectionAndWait(0x1d, mid);
  Actor_WalkByAndWait(0x1d, 0x14, -20);
  Actor_FaceDirectionAndWait(0x1d, 0x8000);
  Call2(Func_02001ac4, 0x1d, 0x1b);
  W_Func_020058fc(6, 0x100, 0x1e);
  FieldEvent_ShowMessageThenWait(6, 0, 0);
  Actor_FaceDirectionAndWait(0x1d, mid);
  Call2(Func_02001ac4, 0x1d, 0x1c);
  Actor_WalkByAndWait(0x1d, -20, 0x14);
  Actor_FaceDirectionAndWait(0x1d, 0);
  W_Func_020058fc(0x1d, 0x108, 0x1e);
  FieldEvent_ShowMessageThenWait(0x1d, 0, 0);
  FieldActor_StartRepeatedMotion(0x1b, 2);
  Actor_RunRepeatedMotion(0x1c, 2);
  FieldEvent_Wait(5);
  W_Func_020058fc(5, 0x102, 0x1e);
  FieldEvent_ShowMessageThenWait(5, 0, 0);
  SET_MODE_AND_WAIT(0x1d, 4, 0x21);
  FieldEvent_ShowMessageThenWait(0x1d, 0, 0);
  SET_MODE_AND_WAIT(7, 4, 0x21);
  FieldEvent_ShowMessageThenWait(7, 0, 0);
  W_Func_020058fc(0x1d, 0x109, 0x1e);
  FieldEvent_ShowMessageThenWait(0x1d, 0, 0);
  Actor_FaceDirectionAndWait(0x1b, 0x4000);
  W_Func_020058fc(0x1b, 0x107, 0x1e);
  FieldEvent_ShowMessageThenWait(0x1b, 0, 0);
  Actor_FaceDirectionAndWait(0x1c, 0x4000);
  W_Func_020058fc(0x1c, 0x107, 0x1e);
  FieldEvent_ShowMessageThenWait(0x1c, 0, 0);
  W_Func_020058fc(0x1d, 0x108, 0x1e);
  FieldEvent_ShowMessageThenWait(0x1d, 0, 0);
  FieldEvent_ShowMessageThenWait(0x1e, 0, 0);
  Actor_FaceDirection(0x1b, 0xe000, 0);
  Actor_FaceDirectionAndWait(0x1c, 0x6000);
  FieldEvent_Wait(0xa);
  W_Func_020058fc(0x1c, 0x102, 0);
  W_Func_020058fc(0x1b, 0x102, 0x1e);
  Actor_FaceDirectionAndWait(0x1d, 0);
  Actor_FaceDirection(0x1c, 0x4000, 0);
  FieldEvent_Wait(0xa);
  SET_MODE_AND_WAIT(0x1d, 3, 0x14);
  FieldEvent_Wait(0xa);
  FieldEvent_ShowMessageThenWait(0x1d, 0, 0);
  Actor_RunRepeatedMotion(6, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(6, 0, 0);
  W_Func_020058fc(0x1d, 0x108, 0x1e);
  FieldEvent_ShowMessageThenWait(0x1d, 0, 0);
  Actor_FaceDirection(5, 0xe000, 0);
  Actor_FaceDirectionAndWait(6, 0x6000);
  FieldEvent_Wait(0xa);
  SET_MODE_AND_WAIT(0x1d, 3, 0x14);
  FieldEvent_Wait(0xa);
  FieldEvent_ShowMessageThenWait(0x1d, 0, 0);
  Actor_FaceDirection(5, 0x8000, 0);
  Actor_FaceDirectionAndWait(0x1d, mid);
  SET_MODE_AND_WAIT(0x1d, 3, 0x14);
  FieldEvent_Wait(0xa);
  Actor_FaceDirection(0x1c, 0x6000, 0);
  Actor_FaceDirectionAndWait(0x1b, 0xe000);
  FieldEvent_Wait(0x14);
  Actor_SetAnimation(0x1c, 3);
  SET_MODE_AND_WAIT(0x1b, 3, 0x14);
  FieldEvent_Wait(0xa);
  FieldCamera_FollowActor(0x1b, 1);
  Actor_SetScript(0x1d, gData_02005ff8);
  FieldEvent_Wait(0x1e);
  Actor_SetScript(0x1b, gData_02005ff8);
  FieldEvent_Wait(0x1e);
  Actor_SetScript(0x1c, gData_02005ff8);
  FieldActor_RefreshSelectorById(0x1d);
  t166 = (s32)Actor_Get(0x1d);
  FIELD_AT((t166 + 0x23), u8, 0) = (FIELD_AT((t166 + 0x23), u8, 0) | 0x40);
  t167 = (s32)Actor_Get(0x1b);
  FIELD_AT((t167 + 0x23), u8, 0) = (FIELD_AT((t167 + 0x23), u8, 0) | 0x40);
  t168 = (s32)Actor_Get(0x1c);
  FIELD_AT((t168 + 0x23), u8, 0) = (0x40 | FIELD_AT((t168 + 0x23), u8, 0));
  t169 = (s32)Actor_Get(0x1d);
  FIELD_AT((t169 + 0x55), u8, 0) = 2;
  Actor_SetScript(0x1d, gData_02006088);
  FieldActor_RefreshSelectorById(0x1b);
  Actor_SetScript(0x1b, gData_02006088);
  FieldActor_RefreshSelectorById(0x1c);
  Actor_SetScript(0x1c, gData_02006088);
  FieldActor_RefreshSelectorById(0x1d);
  Actor_WalkToAndWait(0x1d, 0x137, 0x100);
  Actor_FaceDirection(0x1d, 0x4000, 0);
  FieldActor_RefreshSelectorById(0x1b);
  Actor_WalkToAndWait(0x1b, 0x14c, 0x10a);
  Actor_FaceDirection(0x1b, 0x4000, 0);
  FieldActor_RefreshSelectorById(0x1c);
  Actor_WalkToAndWait(0x1c, 0x122, 0x10a);
  Actor_FaceDirection(0x1c, 0x4000, 0);
  t184 = (s32)Actor_Get(0x1d);
  FIELD_AT((t184 + 0x23), u8, 0) = (FIELD_AT((t184 + 0x23), u8, 0) | 0x20);
  t185 = (s32)Actor_Get(0x1b);
  FIELD_AT((t185 + 0x23), u8, 0) = (FIELD_AT((t185 + 0x23), u8, 0) | 0x20);
  t186 = (s32)Actor_Get(0x1c);
  FIELD_AT((t186 + 0x23), u8, 0) = (0x20 | FIELD_AT((t186 + 0x23), u8, 0));
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Event_PlaySound(0x4e);
  FieldEvent_ShowMessageThenWait(0x1e, 0, 0);
  {
    s32 speed = 0xc000;
    Actor_SetPositionAndFacing(0x1e, 0x2000000, 0x1480000, speed);
  }
  Actor_SetPositionAndFacing(0, 0x2180000, 0x1500000, 0xc000);
  Actor_SetPositionAndFacing(1, 0x2180000, 0x1600000, 0xc000);
  Actor_SetPositionAndFacing(2, 0x2280000, 0x1600000, 0xc000);
  Actor_SetPositionAndFacing(3, 0x2280000, 0x1700000, 0xc000);
  Actor_FaceDirection(4, 0x4000, 0);
  Actor_FaceDirection(5, 0x4000, 0);
  Actor_FaceDirection(6, 0x4000, 0);
  Actor_FaceDirection(7, 0x4000, 0);
  Func_02003a6c(3);
  Func_0200598c();
  FieldEvent_ShowMessageThenWait(0, 0, 0);
  Actor_ShowEmote(4, 0x102, 0x1e);
  W_Func_020058fc(5, 0x109, 0x1e);
  FieldEvent_ShowMessageThenWait(5, 0, 0);
  Actor_FaceDirectionAndWait(1, 0x8000);
  Actor_FaceDirectionAndWait(1, 0xc000);
  FieldEvent_Wait(0x14);
  Func_02003c58(0);
  W_Func_020058fc(1, 0x103, 0x1e);
  FieldEvent_ShowMessageThenWait(1, 0, 0);
  Actor_FaceDirectionAndWait(0, 0xa000);
  Actor_RunRepeatedMotion(0, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(0, 0, 0);
  W_Func_020058fc(0x1e, 0x109, 0x1e);
  Actor_FaceDirectionAndWait(0x1e, 0x2000);
  FieldEvent_ShowMessageThenWait(0x1e, 0, 0);
  W_Func_020058fc(1, 0x107, 0x1e);
  FieldEvent_ShowMessageThenWait(1, 0, 0);
  W_Func_020058fc(2, 0x109, 0x1e);
  Actor_WalkTo(2, 0x228, 0x140);
  Actor_FaceDirectionAndWait(2, 0xc000);
  Actor_FaceDirectionAndWait(2, 0x6000);
  Actor_FaceDirectionAndWait(2, 0x8000);
  SET_MODE_AND_WAIT(2, 4, 0x21);
  Actor_RunRepeatedMotion(2, 2);
  FieldEvent_ShowMessageThenWait(2, 0, 0);
  Actor_RunRepeatedMotion(0, 2);
  Actor_FaceDirectionAndWait(0, 0xe000);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(0, 0, 0);
  Actor_FaceDirectionAndWait(2, 0x6000);
  Actor_RunRepeatedMotion(2, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(2, 0, 0);
  Actor_FaceDirection(5, 0xe000, 0);
  Actor_FaceDirection(6, 0x6000, 0);
  Actor_FaceDirection(7, 0x6000, 0);
  Actor_FaceDirection(4, 0xe000, 0);
  FieldEvent_Wait(0xa);
  Actor_FaceDirectionAndWait(0x1e, 0);
  SET_MODE_AND_WAIT(0x1e, 3, 0x14);
  FieldEvent_Wait(0xa);
  Actor_FaceDirectionAndWait(0x1e, 0);
  FieldEvent_ShowMessageThenWait(0x1e, 0, 0);
  Actor_FaceDirection(4, 0x4000, 0);
  Actor_FaceDirection(5, 0x4000, 0);
  Actor_FaceDirection(7, 0x4000, 0);
  Actor_FaceDirection(6, 0x4000, 0);
  Actor_FaceDirectionAndWait(0, 0x4000);
  FieldEvent_Wait(0x3c);
  W_Func_020058ec(1, 0xc000);
  Actor_SetAnimation(3, 3);
  SET_MODE_AND_WAIT(1, 3, 0x14);
  FieldEvent_Wait(0xa);
  Actor_FaceDirectionAndWait(0, 0xe000);
  FieldEvent_Wait(0x3c);
  Actor_SetAnimation(6, 3);
  Actor_SetAnimation(7, 3);
  SET_MODE_AND_WAIT(5, 3, 0x14);
  FieldEvent_Wait(0xa);
  Actor_FaceDirectionAndWait(0, 0xc000);
  FieldEvent_Wait(0x1e);
  Actor_FaceDirectionAndWait(4, 0x4000);
  t269 = FieldEvent_AskYesNoWithActorAnimation(4, 0);
  if (t269 == 0) {
    SET_MODE_AND_WAIT(0, 3, 0x14);
    FieldEvent_Wait(0xa);
    FieldEvent_ShowMessageThenWait(0, 0, 0);
    SET_MODE_AND_WAIT(0, 3, 0x14);
    FieldEvent_Wait(0xa);
  } else {
    W_Func_020058fc(0, 0x107, 0x1e);
    SET_MODE_AND_WAIT(5, 4, 0x21);
    FIELD_AT(((s32)Ram_HeapSlots->eventWork + 0x1c4), u16, 0) =
        (FIELD_AT(((s32)Ram_HeapSlots->eventWork + 0x1c4), u16, 0) + 2);
    Actor_FaceDirectionAndWait(5, 0x8000);
    FieldEvent_ShowMessageThenWait(5, 0, 0);
    Actor_FaceDirectionAndWait(5, 0x4000);
    FieldEvent_Wait(0xa);
    Actor_RunRepeatedMotion(5, 2);
    FieldEvent_Wait(5);
    FieldEvent_ShowMessageThenWait(5, 0, 0);
    SET_MODE_AND_WAIT(0, 3, 0x14);
    FieldEvent_Wait(0xa);
    FieldEvent_ShowMessageThenWait(0, 0, 0);
  }
  Actor_SetAnimation(2, 2);
  t292 = (s32)Actor_Get(0);
  if (t292 != 0) {
    Actor_SetDestination(2, FIELD_AT(t292, s16, 10), FIELD_AT(t292, s16, 18));
  }
  Actor_SetAnimation(3, 2);
  t295 = (s32)Actor_Get(0);
  if (t295 != 0) {
    Actor_SetDestination(3, FIELD_AT(t295, s16, 10), FIELD_AT(t295, s16, 18));
  }
  Actor_SetAnimation(1, 2);
  t298 = (s32)Actor_Get(0);
  if (t298 != 0) {
    Actor_SetDestination(1, FIELD_AT(t298, s16, 10), FIELD_AT(t298, s16, 18));
  }
  Actor_SetScript(2, gData_02005ff0);
  W_Actor_EnableActionAndSetCallback(3, (s32)gData_02005ff0);
  Actor_SetScript(1, gData_02005ff0);
  FieldActor_RefreshSelectorById(2);
  FieldActor_RefreshSelectorById(3);
  FieldActor_RefreshSelectorById(1);
  FieldCamera_FollowActor(0, 1);
  FieldActor_WatchAndSetScript(4, 0);
  FieldActor_WatchAndSetScript(7, 0);
  FieldActor_WatchAndSetScript(5, 0);
  FieldActor_WatchAndSetScript(6, 0);
  FieldActor_WatchAndSetScript(0x1e, 0);
  FieldEvent_Wait(0xa);
  Actor_SetSpeed(0, 0x18000, 0xc000);
  Actor_WalkToAndWait(0, 0x21f, 0x16b);
  Actor_FaceDirectionAndWait(0, 0xc000);
  FieldEvent_Wait(0xa);
  Actor_RunRepeatedMotion(0, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(0, 0, 0);
  Actor_WalkToAndWait(0, 0x23d, 0x166);
  Actor_WalkToAndWait(0, 0x248, 0x12c);
  Actor_WalkToAndWait(0, 0x248, 0xfc);
  Actor_WalkToAndWait(0, 0x268, 0xf7);
  Actor_WalkToAndWait(0, 0x269, 0x138);
  t325 = (s32)Actor_Get(0);
  FIELD_AT((t325 + 0x55), u8, 0) = 2;
  Actor_SetScript(0, gData_020061e0);
  FieldActor_RefreshSelectorById(0);
  Actor_FaceDirectionAndWait(0, 0x4000);
  FieldEvent_Wait(0x1e);
  t330 = (s32)Actor_Get(0);
  FIELD_AT((t330 + 0x23), u8, 0) = (0x20 | FIELD_AT((t330 + 0x23), u8, 0));
  Actor_FaceDirection(4, 0, 0);
  Actor_FaceDirection(7, 0, 0);
  Actor_FaceDirection(5, 0, 0);
  Actor_FaceDirection(6, 0, 0);
  Actor_FaceDirection(0x1e, 0, 0);
  Func_02003a6c(1);
  FieldEvent_Wait(0x64);
  Actor_FaceDirection(4, 0x8000, 0);
  Actor_FaceDirection(7, 0x8000, 0);
  Actor_FaceDirection(5, 0x8000, 0);
  Actor_FaceDirection(6, 0x8000, 0);
  Actor_FaceDirection(0x1e, 0x8000, 0);
  Func_02003c58(1);
  FieldEvent_Wait(0x3c);
  FieldCamera_FollowActor(4, 1);
  Func_02005924();
  FieldEvent_Wait(0x3c);
  Actor_FaceDirectionAndWait(4, 0);
  FieldEvent_Wait(0x3c);
  SET_MODE_AND_WAIT(5, 3, 0x14);
  FieldEvent_Wait(0xa);
  Actor_FaceDirectionAndWait(4, 0xe000);
  Actor_SetAnimation(7, 3);
  SET_MODE_AND_WAIT(6, 3, 0x14);
  Actor_SetAnimation(7, 2);
  t358 = (s32)Actor_Get(4);
  if (t358 != 0) {
    Actor_SetDestination(7, FIELD_AT(t358, s16, 10), FIELD_AT(t358, s16, 18));
  }
  Actor_SetAnimation(6, 2);
  t361 = (s32)Actor_Get(4);
  if (t361 != 0) {
    Actor_SetDestination(6, FIELD_AT(t361, s16, 10), FIELD_AT(t361, s16, 18));
  }
  Actor_SetAnimation(5, 2);
  t364 = (s32)Actor_Get(4);
  if (t364 != 0) {
    Actor_SetDestination(5, FIELD_AT(t364, s16, 10), FIELD_AT(t364, s16, 18));
  }
  Actor_SetAnimation(0x1e, 2);
  t367 = (s32)Actor_Get(4);
  if (t367 != 0) {
    Actor_SetDestination(0x1e, FIELD_AT(t367, s16, 10),
                         FIELD_AT(t367, s16, 18));
  }
  Actor_SetScript(7, gData_02005ff0);
  Actor_SetScript(6, gData_02005ff0);
  Actor_SetScript(5, gData_02005ff0);
  Actor_SetScript(0x1e, gData_02005ff0);
  FieldActor_RefreshSelectorById(7);
  FieldActor_RefreshSelectorById(6);
  FieldActor_RefreshSelectorById(5);
  FieldActor_RefreshSelectorById(0x1e);
  FieldCamera_FollowActor(4, 1);
  Actor_WalkByAndWait(4, -16, 0);
  Actor_SetScript(4, gData_02005ff8);
  FieldActor_RefreshSelectorById(4);
  t381 = (s32)Actor_Get(4);
  FIELD_AT((t381 + 0x23), u8, 0) = (FIELD_AT((t381 + 0x23), u8, 0) | 0x40);
  t382 = (s32)Actor_Get(0x1d);
  FIELD_AT((t382 + 0x55), u8, 0) = 2;
  Actor_SetScript(4, gData_02006088);
  FieldActor_RefreshSelectorById(4);
  Actor_WalkToAndWait(4, 0x137, 0x100);
  Actor_FaceDirectionAndWait(4, 0x4000);
  FieldEvent_Wait(0xa);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Func_02003a6c(2);
  work = (s32)Ram_HeapSlots->eventWork;
  FIELD_AT((work + 0x1ac), s32, 0) = 0x200;
  FIELD_AT((work + 0x1b4), s32, 0) = 0x18;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(1);
  Func_0200581c();
}
