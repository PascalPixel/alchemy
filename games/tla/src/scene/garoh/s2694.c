#include "types.h"
#include "call.h"
#include "scene_tables.h"
#include "event_inline.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

s32 Func_02001344();
s32 Func_02001418();
s32 Func_02001454();
s32 Func_020051ac();
s32 Func_020051b4();
s32 Func_020052b4();
s32 Func_020052bc();
s32 Actor_SetSingleChildFlags();
extern u8 MsgGaroh14[];

void Func_02002694(void) {
  s32 v5;
  s32 t2;
  s32 t3;
  s32 t4;
  s32 t6;
  s32 t8;
  s32 t9;
  s32 t10;
  s32 t12;
  s32 t13;
  s32 t14;
  s32 t15;
  s32 t16;
  s32 t17;
  s32 t19;
  s32 t20;
  s32 t21;
  s32 t22;
  s32 t23;
  s32 t24;
  s32 b1;
  s32 p1;
  s32 p2;

  Event_SetFlag(0x91e);
  Func_020051ac();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId(((s32)MsgGaroh14));
  Actor_SetPosition(10, PIXELS(328), PIXELS(40));
  Actor_SetPosition(8, PIXELS(328), PIXELS(40));
  Actor_FaceDirection(10, FACING_SOUTH, 0);
  Actor_FaceDirection(8, FACING_SOUTH, 0);
  Actor_WalkToAndWait(4, 320, 224);
  Actor_FaceDirection(4, FACING_NORTH, 0);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x8009, 0, 0);
  FieldActor_LaunchFromLeader(9, -16, -8, FACING_NORTH);
  FieldActor_WaitForMoveById(9);
  FieldEvent_Wait(30);
  FieldCamera_SetSpeed(0x10000, 0x2000);
  FieldCamera_MoveTo(PIXELS(328), -1, PIXELS(152), 1);
  Actor_FaceDirection(9, FACING_NORTHWEST, 0);
  FieldEvent_Wait(25);
  Actor_FaceDirection(9, FACING_NORTHEAST, 0);
  FieldEvent_Wait(25);
  Actor_FaceDirection(9, FACING_NORTHWEST, 0);
  FieldEvent_Wait(25);
  Actor_FaceDirection(9, FACING_NORTH, 0);
  FieldEvent_Wait(25);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  Actor_FaceDirection(4, FACING_NORTHWEST, 0);
  {
    s32 a0 = 9;
    s32 a1 = FACING_SOUTHEAST;
    /* FAKEMATCH(barrier): an empty asm on a0 and a1, so the third argument is
     * loaded, then the first, then the second finished; as the plain call, 4 of
     * 1799 instructions differ in tla-en. */
    __asm__("" : "+l"(a0), "+l"(a1));
    Actor_FaceDirection(a0, a1, 0);
  }
  {
    s32 a0 = 4;
    /* FAKEMATCH(barrier): an empty asm on a0, so the first argument is loaded
     * before the others; as the plain call, 2 of 1799 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(a0));
    Func_020052bc(a0, 1);
  }
  Func_020052b4();
  FieldCamera_SetSpeed(0x18000, 0x3000);
  Event_OpenMessage(9, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(4, 0) != 0)
    goto L_0200a818;
  FieldEvent_Wait(20);
  Actor_ShowEmote(9, 0x109, 30);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  *(u16 *)((FIELD_AT(0x3000000, s32, 108) + 0x1c4)) =
      (*(u16 *)((FIELD_AT(0x3000000, s32, 108) + 0x1c4)) + 1);
  goto L_0200a846;
L_0200a818:;
  FieldEvent_Wait(30);
  Actor_ShowEmote(9, 0x109, 30);
  *(u16 *)((FIELD_AT(0x3000000, s32, 108) + 0x1c4)) =
      (*(u16 *)((FIELD_AT(0x3000000, s32, 108) + 0x1c4)) + 1);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
L_0200a846:;
  FieldEvent_ShowMessageThenWait(8, 0, 0);
  Actor_RunRepeatedMotion(4, 2);
  FieldEvent_Wait(10);
  Actor_FaceDirection(4, FACING_NORTH, 0);
  Actor_FaceDirection(9, FACING_NORTH, 0);
  FieldEvent_Wait(20);
  FieldActor_LaunchFromLeader(5, 16, 0, FACING_NORTH);
  FieldActor_LaunchFromLeader(6, 32, 0, FACING_NORTH);
  t2 = Event_GetFlag(7);
  if (t2 == 0)
    goto L_0200a8b2;
  FieldActor_LaunchFromLeader(7, 48, -8, FACING_NORTHWEST);
L_0200a8b2:;
  FieldActor_WaitForMoveById(6);
  FieldEvent_Wait(20);
  {
    s32 a1 = 0x100;
    s32 a2 = 30;
    /* FAKEMATCH(barrier): an empty asm on a1 and a2, so the later arguments are
     * loaded before the first; as the plain call, 23 of 1799 instructions
     * differ in tla-en. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Actor_ShowEmote(5, a1, a2);
  }
  FieldEvent_ShowMessageThenWait(5, 0, 0);
  Actor_FaceDirection(6, FACING_NORTHWEST, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(6, FACING_NORTHEAST, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(6, FACING_NORTHWEST, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(6, FACING_NORTH, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(6, 0, 0);
  {
    s32 a1 = 0x101;
    s32 a2 = 30;
    /* FAKEMATCH(barrier): an empty asm on a1 and a2, so the later arguments are
     * loaded before the first; as the plain call, 22 of 1799 instructions
     * differ in tla-en. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Actor_ShowEmote(9, a1, a2);
  }
  Actor_FaceDirection(9, FACING_NORTHWEST, 0);
  FieldEvent_Wait(25);
  Actor_FaceDirection(9, FACING_NORTHEAST, 0);
  FieldEvent_Wait(25);
  Actor_FaceDirection(9, FACING_NORTHWEST, 0);
  FieldEvent_Wait(25);
  Actor_FaceDirection(9, FACING_NORTHEAST, 0);
  FieldEvent_Wait(25);
  Actor_FaceDirection(9, FACING_SOUTHEAST, 0);
  FieldEvent_Wait(25);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  Actor_FaceDirection(6, FACING_WEST, 0);
  Actor_FaceDirection(5, FACING_EAST, 0);
  FieldEvent_Wait(40);
  Actor_FaceDirection(6, FACING_NORTHWEST, 0);
  Actor_FaceDirection(5, FACING_NORTHWEST, 0);
  FieldEvent_Wait(25);
  Actor_SetAnimation(6, 3);
  {
    s32 a0 = 5;
    /* FAKEMATCH(barrier): an empty asm on a0, so the first argument is loaded
     * before the others; as the plain call, 2 of 1799 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(a0));
    FieldActor_SetAnimation(a0, 3);
  }
  FieldEvent_Wait(20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  Actor_FaceDirection(5, FACING_NORTH, 0);
  Actor_FaceDirection(6, FACING_NORTH, 0);
  Actor_FaceDirection(4, FACING_NORTH, 0);
  Actor_FaceDirection(9, FACING_NORTHEAST, 0);
  FieldCamera_MoveTo(PIXELS(328), -1, PIXELS(144), 1);
  Func_020052b4();
  FieldEvent_Wait(20);
  Actor_SetPosition(11, PIXELS(328), PIXELS(136));
  t3 = Actor_Get(11);
  Actor_SetSingleChildFlags(t3, 0);
  Func_02001418(11);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(5, 0, 0);
  Actor_SetSpeed(8, 0xb333, 0x5999);
  Actor_SetSpeed(10, 0xb333, 0x5999);
  Actor_SetPosition(10, PIXELS(328), PIXELS(128));
  FieldEvent_Wait(10);
  Actor_WalkByAndWait(10, 0, 16);
  Actor_SetPosition(8, PIXELS(328), PIXELS(128));
  FieldEvent_Wait(10);
  Actor_WalkBy(8, 0, 16);
  Actor_WalkByAndWait(10, 0, 16);
  FieldEvent_Wait(10);
  Func_02001454(11);
  Actor_SetPosition(11, 0, 0);
  FieldCamera_FollowActor(10, 1);
  FieldEvent_Wait(20);
  Actor_WalkBy(8, 0, 24);
  Actor_WalkByAndWait(10, 0, 24);
  Actor_WalkByAndWait(8, 16, 8);
  Actor_FaceDirection(8, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  FieldCamera_MoveTo(PIXELS(328), -1, PIXELS(208), 1);
  Func_020052b4();
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(8, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(8, 0, 0);
  FieldActor_StartRepeatedMotion(9, 2);
  FieldActor_StartRepeatedMotion(6, 2);
  FieldActor_StartRepeatedMotion(5, 2);
  t4 = Event_GetFlag(7);
  if (t4 == 0)
    goto L_0200ab98;
  FieldActor_StartRepeatedMotion(7, 2);
L_0200ab98:;
  Actor_RunRepeatedMotion(4, 2);
  FieldEvent_Wait(20);
  {
    s32 a0 = 10;
    /* FAKEMATCH(barrier): an empty asm on a0, so the first argument is loaded
     * before the others; as the plain call, 17 of 1799 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(a0));
    FieldActor_SetAnimation(a0, 4);
  }
  FieldEvent_Wait(33);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  Actor_ShowEmote(6, 0x100, 30);
  FieldEvent_ShowMessageThenWait(6, 0, 0);
  FieldActor_StartRepeatedMotion(8, 3);
  Actor_SetAttachedEffect(8, 0x102);
  FieldEvent_Wait(40);
  FieldEvent_ShowMessageThenWait(8, 0, 0);
  Actor_RunRepeatedMotion(9, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  FieldActor_Jump(8, 6, 15);
  FieldActor_Jump(8, 6, 23);
  {
    s32 a1 = FACING_SOUTHWEST;
    s32 a2 = 0;
    /* FAKEMATCH(barrier): an empty asm on a1 and a2, so the later arguments are
     * loaded before the first; as the plain call, 13 of 1799 instructions
     * differ in tla-en. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Actor_FaceDirection(8, a1, a2);
  }
  FieldEvent_Wait(20);
  {
    s32 a0 = 10;
    /* FAKEMATCH(barrier): an empty asm on a0, so the first argument is loaded
     * before the others; as the plain call, 15 of 1799 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(a0));
    FieldActor_SetAnimation(a0, 4);
  }
  FieldEvent_Wait(33);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  {
    s32 a1 = 0x101;
    s32 a2 = 0;
    /* FAKEMATCH(barrier): an empty asm on a1 and a2, so the later arguments are
     * loaded before the first; as the plain call, 26 of 1799 instructions
     * differ in tla-en. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Actor_ShowEmote(4, a1, a2);
  }
  {
    s32 a1 = 0x101;
    s32 a2 = 0;
    /* FAKEMATCH(barrier): an empty asm on a1 and a2, so the later arguments are
     * loaded before the first; as the plain call, 16 of 1799 instructions
     * differ in tla-en. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Actor_ShowEmote(5, a1, a2);
  }
  Actor_ShowEmote(6, 0x101, 0);
  if (Event_GetFlag(7) == 0)
    goto L_0200ac86;
  Actor_ShowEmote(7, 0x101, 0);
L_0200ac86:;
  Actor_ShowEmote(9, 0x101, 60);
  Actor_RunRepeatedMotion(10, 2);
  FieldEvent_Wait(10);
  Event_OpenMessage(10, 0);
  t6 = FieldEvent_AskYesNoWithActorAnimation(4, 0);
  if (t6 == 0)
    goto L_0200ace6;
  FieldEvent_Wait(40);
  {
    s32 a0 = 10;
    /* FAKEMATCH(barrier): an empty asm on a0, so the first argument is loaded
     * before the others; as the plain call, 15 of 1799 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(a0));
    FieldActor_SetAnimation(a0, 4);
  }
  FieldEvent_Wait(33);
  {
    s32 a0 = 10;
    /* FAKEMATCH(barrier): an empty asm on a0, so the first argument is loaded
     * before the others; as the plain call, 2 of 1799 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(a0));
    FieldEvent_ShowMessageThenWait(a0, 0, 0);
  }
  *(u16 *)((FIELD_AT(0x3000000, s32, 108) + 0x1c4)) =
      (*(u16 *)((FIELD_AT(0x3000000, s32, 108) + 0x1c4)) + 1);
  goto L_0200ad16;
L_0200ace6:;
  FieldEvent_Wait(30);
  Actor_RunRepeatedMotion(10, 2);
  FieldEvent_Wait(10);
  *(u16 *)((FIELD_AT(0x3000000, s32, 108) + 0x1c4)) =
      (*(u16 *)((FIELD_AT(0x3000000, s32, 108) + 0x1c4)) + 1);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
L_0200ad16:;
  Actor_RunRepeatedMotion(8, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(8, 0, 0);
  Actor_FaceDirection(10, FACING_EAST, 0);
  FieldEvent_Wait(30);
  {
    s32 a0 = 10;
    /* FAKEMATCH(barrier): an empty asm on a0, so the first argument is loaded
     * before the others; as the plain call, 17 of 1799 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(a0));
    FieldActor_SetAnimation(a0, 3);
  }
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  {
    s32 a1 = 0x101;
    s32 a2 = 30;
    /* FAKEMATCH(barrier): an empty asm on a1 and a2, so the later arguments are
     * loaded before the first; as the plain call, 32 of 1799 instructions
     * differ in tla-en. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Actor_ShowEmote(5, a1, a2);
  }
  FieldEvent_ShowMessageThenWait(5, 0, 0);
  Actor_RunRepeatedMotion(10, 2);
  FieldEvent_Wait(10);
  Actor_FaceDirection(8, FACING_SOUTH, 0);
  Actor_FaceDirection(10, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  Actor_ShowEmote(6, 0x106, 30);
  Actor_FaceDirection(6, FACING_NORTHWEST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(6, 0, 0);
  {
    s32 a0 = 10;
    /* FAKEMATCH(barrier): an empty asm on a0, so the first argument is loaded
     * before the others; as the plain call, 4 of 1799 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(a0));
    FieldActor_SetAnimation(a0, 4);
  }
  FieldEvent_Wait(33);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  {
    s32 a1 = 0x101;
    s32 a2 = 30;
    /* FAKEMATCH(barrier): an empty asm on a1 and a2, so the later arguments are
     * loaded before the first; as the plain call, 22 of 1799 instructions
     * differ in tla-en. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Actor_ShowEmote(10, a1, a2);
  }
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  {
    s32 a1 = 0x101;
    s32 a2 = 0;
    /* FAKEMATCH(barrier): an empty asm on a1 and a2, so the later arguments are
     * loaded before the first; as the plain call, 26 of 1799 instructions
     * differ in tla-en. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Actor_ShowEmote(4, a1, a2);
  }
  {
    s32 a1 = 0x101;
    s32 a2 = 0;
    /* FAKEMATCH(barrier): an empty asm on a1 and a2, so the later arguments are
     * loaded before the first; as the plain call, 16 of 1799 instructions
     * differ in tla-en. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Actor_ShowEmote(5, a1, a2);
  }
  Actor_ShowEmote(6, 0x101, 0);
  if (Event_GetFlag(7) == 0)
    goto L_0200ae32;
  Actor_ShowEmote(7, 0x101, 0);
L_0200ae32:;
  {
    s32 a1 = 0x101;
    s32 a2 = 60;
    Actor_ShowEmote(9, a1, a2);
  }
  {
    s32 a0 = 10;
    /* FAKEMATCH(barrier): an empty asm on a0, so the first argument is loaded
     * before the others; as the plain call, 4 of 1799 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(a0));
    FieldActor_SetAnimation(a0, 3);
  }
  FieldEvent_Wait(20);
  Call2(FieldMessage_ShowAtActor, 10, 0);
  Actor_FaceDirection(9, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(5, FACING_WEST, 0);
  Actor_FaceDirection(6, FACING_WEST, 0);
  t8 = Event_GetFlag(7);
  if (t8 == 0)
    goto L_0200ae8e;
  Actor_FaceDirection(7, FACING_WEST, 0);
L_0200ae8e:;
  FieldEvent_Wait(20);
  t9 = FieldEvent_AskYesNoWithActorAnimation(4, 0);
  if (t9 == 0)
    goto L_0200aed2;
  FieldEvent_Wait(40);
  Actor_RunRepeatedMotion(5, 2);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(5, 0, 0);
  *(u16 *)((FIELD_AT(0x3000000, s32, 108) + 0x1c4)) =
      (*(u16 *)((FIELD_AT(0x3000000, s32, 108) + 0x1c4)) + 1);
  goto L_0200af08;
L_0200aed2:;
  FieldEvent_Wait(30);
  {
    s32 a0 = 5;
    /* FAKEMATCH(barrier): an empty asm on a0, so the first argument is loaded
     * before the others; as the plain call, 2 of 1799 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(a0));
    FieldActor_SetAnimation(a0, 3);
  }
  FieldEvent_Wait(20);
  FieldEvent_Wait(20);
  *(u16 *)((FIELD_AT(0x3000000, s32, 108) + 0x1c4)) =
      (*(u16 *)((FIELD_AT(0x3000000, s32, 108) + 0x1c4)) + 1);
  FieldEvent_ShowMessageThenWait(5, 0, 0);
L_0200af08:;
  Actor_FaceDirection(10, FACING_NORTHEAST, 0);
  Actor_FaceDirection(8, FACING_SOUTHWEST, 0);
  FieldEvent_Wait(30);
  FieldEvent_ShowMessageThenWait(8, 0, 0);
  Actor_FaceDirection(9, FACING_NORTHEAST, 0);
  Actor_FaceDirection(5, FACING_NORTH, 0);
  Actor_FaceDirection(6, FACING_NORTH, 0);
  t10 = Event_GetFlag(7);
  if (t10 == 0)
    goto L_0200af6a;
  Actor_FaceDirection(7, FACING_NORTHWEST, 0);
L_0200af6a:;
  FieldEvent_Wait(30);
  Actor_RunRepeatedMotion(6, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(6, 0, 0);
  Actor_FaceDirection(10, FACING_SOUTH, 0);
  FieldEvent_Wait(30);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  {
    s32 a0 = 9;
    /* FAKEMATCH(barrier): an empty asm on a0, so the first argument is loaded
     * before the others; as the plain call, 23 of 1799 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(a0));
    FieldActor_SetAnimation(a0, 3);
  }
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  {
    s32 a1 = 0x100;
    s32 a2 = 30;
    /* FAKEMATCH(barrier): an empty asm on a1 and a2, so the later arguments are
     * loaded before the first; as the plain call, 33 of 1799 instructions
     * differ in tla-en. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Actor_ShowEmote(10, a1, a2);
  }
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  Actor_SetAnimation(4, 3);
  Actor_SetAnimation(5, 3);
  FieldActor_SetAnimation(6, 3);
  if (Event_GetFlag(7) == 0)
    goto L_0200affc;
  FieldActor_SetAnimation(7, 3);
L_0200affc:;
  FieldActor_SetAnimation(9, 3);
  FieldEvent_Wait(20);
  FieldEvent_Wait(20);
  Actor_FaceDirection(9, FACING_SOUTHEAST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  {
    s32 a1 = FACING_NORTHEAST;
    s32 a2 = 0;
    /* FAKEMATCH(barrier): an empty asm on a1 and a2, so the later arguments are
     * loaded before the first; as the plain call, 44 of 1799 instructions
     * differ in tla-en. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Actor_FaceDirection(9, a1, a2);
  }
  FieldEvent_Wait(30);
  {
    s32 a0 = 10;
    /* FAKEMATCH(barrier): an empty asm on a0, so the first argument is loaded
     * before the others; as the plain call, 33 of 1799 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(a0));
    FieldActor_SetAnimation(a0, 3);
  }
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  {
    s32 a1 = 0x102;
    s32 a2 = 30;
    /* FAKEMATCH(barrier): an empty asm on a1 and a2, so the later arguments are
     * loaded before the first; as the plain call, 38 of 1799 instructions
     * differ in tla-en. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Actor_ShowEmote(8, a1, a2);
  }
  FieldEvent_ShowMessageThenWait(8, 0, 0);
  Actor_FaceDirection(10, FACING_NORTHEAST, 0);
  FieldEvent_Wait(30);
  Actor_RunRepeatedMotion(5, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(5, 0, 0);
  Actor_FaceDirection(10, FACING_SOUTHEAST + FACING_STEP, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  {
    s32 a1 = 0x109;
    s32 a2 = 30;
    /* FAKEMATCH(barrier): an empty asm on a1 and a2, so the later arguments are
     * loaded before the first; as the plain call, 29 of 1799 instructions
     * differ in tla-en. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Actor_ShowEmote(6, a1, a2);
  }
  FieldEvent_ShowMessageThenWait(6, 0, 0);
  Actor_FaceDirection(8, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(8, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(8, 0, 0);
  Actor_FaceDirection(9, FACING_EAST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  {
    s32 a1 = FACING_NORTHEAST;
    s32 a2 = 0;
    /* FAKEMATCH(barrier): an empty asm on a1 and a2, so the later arguments are
     * loaded before the first; as the plain call, 36 of 1799 instructions
     * differ in tla-en. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Actor_FaceDirection(9, a1, a2);
  }
  FieldEvent_Wait(30);
  {
    s32 a0 = 10;
    /* FAKEMATCH(barrier): an empty asm on a0, so the first argument is loaded
     * before the others; as the plain call, 31 of 1799 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(a0));
    FieldActor_SetAnimation(a0, 3);
  }
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  {
    s32 a1 = 0x102;
    s32 a2 = 30;
    /* FAKEMATCH(barrier): an empty asm on a1 and a2, so the later arguments are
     * loaded before the first; as the plain call, 39 of 1799 instructions
     * differ in tla-en. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Actor_ShowEmote(5, a1, a2);
  }
  FieldEvent_ShowMessageThenWait(5, 0, 0);
  Actor_RunRepeatedMotion(10, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  {
    s32 a0 = 9;
    /* FAKEMATCH(barrier): an empty asm on a0, so the first argument is loaded
     * before the others; as the plain call, 21 of 1799 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(a0));
    FieldActor_SetAnimation(a0, 4);
  }
  FieldEvent_Wait(33);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  Actor_FaceDirection(8, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(8, 0, 16);
  FieldEvent_Wait(10);
  FieldActor_Jump(8, 6, 15);
  FieldActor_Jump(8, 6, 23);
  {
    s32 a1 = 0x102;
    s32 a2 = 30;
    /* FAKEMATCH(barrier): an empty asm on a1 and a2, so the later arguments are
     * loaded before the first; as the plain call, 26 of 1799 instructions
     * differ in tla-en. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Actor_ShowEmote(6, a1, a2);
  }
  FieldEvent_ShowMessageThenWait(6, 0, 0);
  Actor_SetSpeed(10, 0x28000, 0x14000);
  Actor_SetSpeed(8, 0x28000, 0x14000);
  Actor_FaceDirection(10, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(10, 8, 0);
  FieldActor_Jump(8, 4, 0);
  t12 = Actor_Get(10);
  v5 = 254;
  *(u8 *)((t12 + 90)) = (v5 & *(u8 *)((t12 + 90)));
  Actor_WalkBy(10, -8, 0);
  t13 = Actor_Get(8);
  v5 = (v5 & *(u8 *)((t13 + 90)));
  *(u8 *)((t13 + 90)) = v5;
  Actor_WalkByAndWait(8, 16, 0);
  FieldEvent_Wait(1);
  t14 = Actor_Get(8);
  /* FAKEMATCH(temporary): a local holding the constant 1, so the value is ORed
   * into the register that holds it; as one |= 1, 4 of 1799 instructions differ
   * in tla-en. */
  b1 = 1;
  b1 |= *(u8 *)((t14 + 90));
  *(u8 *)((t14 + 90)) = b1;
  FieldActor_SetAnimation(8, 5);
  FieldEvent_Wait(20);
  Actor_SetSpeed(8, 0x14ccc, 0xa666);
  Actor_SetSpeed(10, 0xb333, 0x5999);
  {
    s32 a0 = 10;
    /* FAKEMATCH(barrier): an empty asm on a0, so the first argument is loaded
     * before the others; as the plain call, 27 of 1799 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(a0));
    FieldActor_SetAnimation(a0, 4);
  }
  FieldEvent_Wait(33);
  FieldEvent_Wait(10);
  {
    s32 a0 = 8;
    /* FAKEMATCH(barrier): an empty asm on a0, so the first argument is loaded
     * after the second is finished; as the plain call, 17 of 1799 instructions
     * differ in tla-en. */
    __asm__("" : "+l"(a0));
    Actor_ShowEmote(a0, 0x102, 50);
  }
  Actor_SetAnimation(8, 1);
  FieldActor_Jump(8, 6, 0);
  Actor_FaceDirection(8, FACING_WEST, 0);
  FieldEvent_Wait(30);
  Actor_SetAttachedEffect(8, 0x102);
  {
    s32 a0 = 8;
    /* FAKEMATCH(barrier): an empty asm on a0, so the first argument is loaded
     * before the others; as the plain call, 17 of 1799 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(a0));
    FieldActor_SetAnimation(a0, 3);
  }
  FieldEvent_Wait(20);
  FieldEvent_Wait(15);
  Actor_SetAttachedEffect(8, 0x102);
  {
    s32 a0 = 8;
    /* FAKEMATCH(barrier): an empty asm on a0, so the first argument is loaded
     * before the others; as the plain call, 15 of 1799 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(a0));
    FieldActor_SetAnimation(a0, 3);
  }
  FieldEvent_Wait(20);
  FieldEvent_Wait(20);
  {
    s32 a0 = 10;
    /* FAKEMATCH(barrier): an empty asm on a0, so the first argument is loaded
     * before the others; as the plain call, 23 of 1799 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(a0));
    FieldActor_SetAnimation(a0, 3);
  }
  FieldEvent_Wait(20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  {
    s32 a1 = 0x101;
    s32 a2 = 30;
    /* FAKEMATCH(barrier): an empty asm on a1 and a2, so the later arguments are
     * loaded before the first; as the plain call, 18 of 1799 instructions
     * differ in tla-en. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Actor_ShowEmote(8, a1, a2);
  }
  FieldEvent_ShowMessageThenWait(8, 0, 0);
  {
    s32 a0 = 10;
    /* FAKEMATCH(barrier): an empty asm on a0, so the first argument is loaded
     * before the others; as the plain call, 23 of 1799 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(a0));
    FieldActor_SetAnimation(a0, 4);
  }
  FieldEvent_Wait(33);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  Actor_FaceDirection(8, FACING_SOUTH + FACING_STEP, 0);
  FieldEvent_Wait(45);
  Actor_FaceDirection(8, FACING_WEST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(8, 0, 0);
  {
    s32 a0 = 10;
    /* FAKEMATCH(barrier): an empty asm on a0, so the first argument is loaded
     * before the others; as the plain call, 21 of 1799 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(a0));
    FieldActor_SetAnimation(a0, 4);
  }
  FieldEvent_Wait(33);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  Actor_FaceDirection(10, FACING_SOUTH + FACING_STEP, 0);
  FieldEvent_Wait(45);
  Actor_FaceDirection(10, FACING_EAST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  {
    s32 a0 = 8;
    /* FAKEMATCH(barrier): an empty asm on a0, so the first argument is loaded
     * before the others; as the plain call, 15 of 1799 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(a0));
    FieldActor_SetAnimation(a0, 3);
  }
  FieldEvent_Wait(20);
  FieldEvent_Wait(20);
  {
    s32 a1 = FACING_SOUTH;
    s32 a2 = 0;
    /* FAKEMATCH(barrier): an empty asm on a1 and a2, so the later arguments are
     * loaded before the first; as the plain call, 8 of 1799 instructions differ
     * in tla-en. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Actor_FaceDirection(8, a1, a2);
  }
  FieldEvent_Wait(35);
  {
    s32 a0 = 8;
    /* FAKEMATCH(barrier): an empty asm on a0, so the first argument is loaded
     * before the others; as the plain call, 2 of 1799 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(a0));
    FieldActor_SetAnimation(a0, 3);
  }
  FieldEvent_Wait(20);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(8, 0, 0);
  Actor_SetAnimation(4, 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimation(6, 3);
  {
    s32 a0 = 9;
    /* FAKEMATCH(barrier): an empty asm on a0, so the first argument is loaded
     * before the others; as the plain call, 2 of 1799 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(a0));
    FieldActor_SetAnimation(a0, 3);
  }
  FieldEvent_Wait(20);
  FieldEvent_Wait(20);
  FieldActor_WatchAndSetScript(4, 8);
  FieldActor_WatchAndSetScript(9, 8);
  FieldActor_WatchAndSetScript(5, 8);
  FieldActor_WatchAndSetScript(6, 8);
  FieldActor_WatchAndSetScript(10, 8);
  t15 = Event_GetFlag(7);
  if (t15 == 0)
    goto L_0200b46e;
  FieldActor_WatchAndSetScript(7, 8);
L_0200b46e:;
  Actor_WalkToAndWait(8, 376, 208);
  Actor_WalkByAndWait(8, 0, 16);
  Actor_WalkByAndWait(8, -16, 24);
  Actor_WalkByAndWait(8, -32, 16);
  Actor_WalkByAndWait(8, 0, 48);
  FieldEvent_Wait(20);
  FieldActor_Stop(4);
  FieldActor_Stop(9);
  FieldActor_Stop(5);
  FieldActor_Stop(6);
  FieldActor_Stop(10);
  t16 = Event_GetFlag(7);
  if (t16 == 0)
    goto L_0200b4da;
  FieldActor_Stop(7);
L_0200b4da:;
  Actor_SetPosition(8, 0, 0);
  Actor_RunRepeatedMotion(10, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  Actor_FaceDirection(9, FACING_NORTHEAST, 0);
  Actor_FaceDirection(5, FACING_NORTH, 0);
  Actor_FaceDirection(6, FACING_NORTH, 0);
  Actor_FaceDirection(4, FACING_NORTH, 0);
  t17 = Event_GetFlag(7);
  if (t17 == 0)
    goto L_0200b542;
  Actor_FaceDirection(7, FACING_NORTHWEST, 0);
L_0200b542:;
  FieldEvent_Wait(30);
  {
    s32 a0 = 10;
    /* FAKEMATCH(barrier): an empty asm on a0, so the first argument is loaded
     * before the others; as the plain call, 17 of 1799 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(a0));
    FieldActor_SetAnimation(a0, 3);
  }
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  Actor_ShowEmote(9, 0x102, 30);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  Actor_ShowEmote(10, 0x108, 30);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  {
    s32 a1 = 0x101;
    s32 a2 = 30;
    /* FAKEMATCH(barrier): an empty asm on a1 and a2, so the later arguments are
     * loaded before the first; as the plain call, 8 of 1799 instructions differ
     * in tla-en. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Actor_ShowEmote(6, a1, a2);
  }
  FieldEvent_ShowMessageThenWait(6, 0, 0);
  Actor_ShowEmote(10, 0x109, 30);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  Actor_ShowEmote(9, 0x100, 30);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  {
    s32 a0 = 10;
    /* FAKEMATCH(barrier): an empty asm on a0, so the first argument is loaded
     * before the others; as the plain call, 17 of 1799 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(a0));
    FieldActor_SetAnimation(a0, 3);
  }
  FieldEvent_Wait(20);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(5, 2);
  FieldEvent_Wait(10);
  Actor_FaceDirection(5, FACING_SOUTH, 0);
  FieldEvent_Wait(30);
  FieldEvent_ShowMessageThenWait(5, 0, 0);
  {
    s32 a0 = 10;
    /* FAKEMATCH(barrier): an empty asm on a0, so the first argument is loaded
     * before the others; as the plain call, 15 of 1799 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(a0));
    FieldActor_SetAnimation(a0, 4);
  }
  FieldEvent_Wait(33);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  Actor_ShowEmote(5, 0x101, 50);
  Actor_FaceDirection(5, FACING_NORTHWEST + FACING_STEP, 0);
  FieldEvent_Wait(30);
  Actor_FaceDirection(9, FACING_SOUTHEAST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  {
    s32 a0 = 10;
    /* FAKEMATCH(barrier): an empty asm on a0, so the first argument is loaded
     * before the others; as the plain call, 4 of 1799 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(a0));
    FieldActor_SetAnimation(a0, 3);
  }
  FieldEvent_Wait(20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  Actor_FaceDirection(9, FACING_NORTHEAST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimation(4, 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimation(6, 3);
  {
    s32 a0 = 9;
    /* FAKEMATCH(barrier): an empty asm on a0, so the first argument is loaded
     * before the others; as the plain call, 2 of 1799 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(a0));
    FieldActor_SetAnimation(a0, 3);
  }
  FieldEvent_Wait(20);
  FieldEvent_Wait(20);
  Actor_FaceDirection(9, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(5, FACING_WEST, 0);
  Actor_FaceDirection(6, FACING_WEST, 0);
  if (Event_GetFlag(7) == 0)
    goto L_0200b6f4;
  Actor_FaceDirection(7, FACING_WEST, 0);
L_0200b6f4:;
  FieldEvent_Wait(20);
  Actor_SetAnimation(5, 3);
  FieldActor_SetAnimation(6, 3);
  t19 = Event_GetFlag(7);
  if (t19 == 0)
    goto L_0200b71c;
  FieldActor_SetAnimation(7, 3);
L_0200b71c:;
  FieldActor_SetAnimation(9, 3);
  FieldEvent_Wait(20);
  FieldEvent_Wait(20);
  {
    s32 a0 = 4;
    /* FAKEMATCH(barrier): an empty asm on a0, so the first argument is loaded
     * before the others; as the plain call, 2 of 1799 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(a0));
    FieldActor_SetAnimation(a0, 3);
  }
  FieldEvent_Wait(20);
  FieldEvent_Wait(10);
  Actor_SetSpeed(9, 0x13333, 0x9999);
  FieldActor_SetAnimation(9, 2);
  p1 = (s32)&gMainRoutineWork;
  v5 = (p1 + 0x214);
  t20 = Actor_Get(*(s32 *)(v5));
  if (t20 == 0)
    goto L_0200b77a;
  Actor_SetDestination(9, *(s16 *)(t20 + 10), *(s16 *)(t20 + 18));
L_0200b77a:;
  FieldActor_WaitForMoveById(9);
  Actor_SetPosition(9, 0, 0);
  Actor_SetSpeed(5, 0x13333, 0x9999);
  FieldActor_SetAnimation(5, 2);
  t21 = Actor_Get(*(s32 *)(v5));
  if (t21 == 0)
    goto L_0200b7b8;
  Actor_SetDestination(5, *(s16 *)(t21 + 10), *(s16 *)(t21 + 18));
L_0200b7b8:;
  FieldActor_WaitForMoveById(5);
  Actor_SetPosition(5, 0, 0);
  Actor_SetSpeed(6, 0x13333, 0x9999);
  FieldActor_SetAnimation(6, 2);
  t22 = Actor_Get(*(s32 *)(v5));
  if (t22 == 0)
    goto L_0200b7f6;
  Actor_SetDestination(6, *(s16 *)(t22 + 10), *(s16 *)(t22 + 18));
L_0200b7f6:;
  FieldActor_WaitForMoveById(6);
  Actor_SetPosition(6, 0, 0);
  t23 = Event_GetFlag(7);
  if (t23 == 0)
    goto L_0200b84e;
  Actor_SetSpeed(7, 0x13333, 0x9999);
  FieldActor_SetAnimation(7, 2);
  t24 = Actor_Get(*(s32 *)(v5));
  if (t24 == 0)
    goto L_0200b83e;
  Actor_SetDestination(7, *(s16 *)(t24 + 10), *(s16 *)(t24 + 18));
L_0200b83e:;
  FieldActor_WaitForMoveById(7);
  Actor_SetPosition(7, 0, 0);
L_0200b84e:;
  p2 = (s32)&gMainRoutineWork;
  {
    s32 a0 = *(s32 *)((p2 + 0x214));
    FieldCamera_FollowActor(a0, 1);
  }
  Func_02001344();
  FieldEvent_SetExitRequest(11);
  Func_020051b4();
}
