#include "types.h"
#include "ram_buffer.h"
#include "event_inline.h"
#include "field.h"

#define ACTOR_MODE_THEN_WAIT(id, mode, frames)                                 \
  do {                                                                         \
    Actor_SetAnimation(id, mode);                                              \
    FieldEvent_Wait(frames);                                                   \
  } while (0)

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

s32 Func_020013b0();
s32 Func_02001418();
s32 Func_02001454();
s32 Func_020051ac();
s32 Func_020051b4();
s32 Func_020052b4();
s32 __divsi3();
extern u8 MsgGaroh28[];
extern u8 gMainRoutineWork[];

void Func_02003878(void) {
  s32 v5;
  s32 v6;
  s32 v7;
  s32 v8;
  s32 t1;
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
  s32 t14;
  s32 t15;
  s32 t16;
  s32 t17;
  s32 t18;
  s32 t19;
  s32 t20;
  s32 t21;
  s32 t22;
  s32 t23;
  s32 t24;
  s32 t25;
  s32 t26;
  s32 t27;
  s32 t28;
  s32 t29;
  s32 t30;
  s32 t31;
  s32 b1;
  s32 p1;
  s32 p2;

  Event_SetFlag(0x91f);
  Func_020051ac();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId(((s32)MsgGaroh28));
  Actor_SetPosition(10, 0x1480000, 0x280000);
  Actor_FaceDirection(10, 0x4000, 0);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  Actor_SetSpeed(4, 0xcccc, 0x6666);
  Actor_WalkToAndWait(4, 0x140, 224);
  FieldActor_LaunchFromLeader(9, -16, 0, 0xc000);
  FieldActor_LaunchFromLeader(5, 16, 0, 0xc000);
  FieldActor_LaunchFromLeader(6, 32, 0, 0xc000);
  t1 = Event_GetFlag(7);
  if (t1 == 0)
    goto L_0200b920;
  FieldActor_LaunchFromLeader(7, 48, 0, 0xc000);
L_0200b920:;
  Actor_FaceDirection(4, 0xc000, 0);
  FieldActor_WaitForMoveById(6);
  FieldEvent_Wait(20);
  FieldCamera_MoveTo(0x1480000, -1, 0x900000, 1);
  Func_020052b4();
  FieldEvent_Wait(20);
  Actor_SetPosition(11, 0x1480000, 0x880000);
  t2 = (s32)Actor_Get(11);
  Actor_SetSingleChildFlags(t2, 0);
  Func_02001418(11);
  FieldEvent_Wait(30);
  Actor_SetSpeed(10, 0xb333, 0x5999);
  Actor_SetPosition(10, 0x1480000, 0x800000);
  FieldEvent_Wait(10);
  Actor_WalkByAndWait(10, 0, 16);
  FieldEvent_Wait(20);
  Func_02001454(11);
  Actor_SetPosition(11, 0, 0);
  FieldCamera_FollowActor(10, 1);
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(10, 0, 48);
  Actor_FaceDirection(9, 0xe000, 0);
  Actor_FaceDirection(4, 0xc000, 0);
  Actor_FaceDirection(5, 0xc000, 0);
  Actor_FaceDirection(6, 0xa000, 0);
  t3 = Event_GetFlag(7);
  if (t3 == 0)
    goto L_0200ba1e;
  Actor_FaceDirection(7, 0xa000, 0);
L_0200ba1e:;
  FieldCamera_MoveTo(0x1480000, -1, 0xb80000, 1);
  Func_020052b4();
  FieldEvent_Wait(10);
  ACTOR_MODE_THEN_WAIT(10, 3, 20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x600a, 0, 0);
  Actor_SetAnimation(4, 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimation(6, 3);
  t4 = Event_GetFlag(7);
  if (t4 == 0)
    goto L_0200ba8c;
  Actor_SetAnimation(7, 3);
  goto L_0200ba8c;
L_0200ba8c:;
  Actor_SetAnimation(9, 3);
  FieldEvent_Wait(20);
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(9, 0, -16);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(9, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  Actor_WalkByAndWait(10, -8, 0);
  Actor_FaceDirection(10, 0x5000, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(0x600a, 0, 0);
  {
    s32 a1 = 0x102;
    s32 a2 = 30;
    /* FAKEMATCH(barrier): the native loads the later arguments before the
     * first; as plain calls, 177 of 2484 instructions differ in tla-en. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Actor_ShowEmote(9, a1, a2);
  }
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  Actor_ShowEmote(10, 0x107, 30);
  FieldEvent_ShowMessageThenWait(0x600a, 0, 0);
  Actor_ShowEmote(9, 0x109, 30);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  Actor_RunRepeatedMotion(10, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x600a, 0, 0);
  {
    s32 a1 = 0x101;
    s32 a2 = 30;
    /* FAKEMATCH(barrier): the native loads the later arguments before the
     * first; as plain calls, 177 of 2484 instructions differ in tla-en. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Actor_ShowEmote(9, a1, a2);
  }
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  Actor_WalkByAndWait(10, 16, 0);
  Actor_FaceDirection(10, 0x5000, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(0x600a, 0, 0);
  {
    s32 a1 = 0x100;
    s32 a2 = 20;
    /* FAKEMATCH(barrier): the native loads the later arguments before the
     * first; as plain calls, 177 of 2484 instructions differ in tla-en. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Actor_ShowEmote(5, a1, a2);
  }
  FieldEvent_ShowMessageThenWait(5, 0, 0);
  Actor_FaceDirection(10, 0x2000, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(0x600a, 0, 0);
  Actor_RunRepeatedMotion(6, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(6, 0, 0);
  Actor_WalkByAndWait(10, -8, 0);
  Actor_FaceDirection(10, 0x5000, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(0x600a, 0, 0);
  ACTOR_MODE_THEN_WAIT(4, 3, 20);
  FieldEvent_Wait(35);
  Actor_WalkByAndWait(10, -8, 0);
  {
    s32 a1 = 0x5000;
    s32 a2 = 0;
    /* FAKEMATCH(barrier): the native loads the later arguments before the
     * first; as plain calls, 177 of 2484 instructions differ in tla-en. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Actor_FaceDirection(10, a1, a2);
  }
  FieldEvent_Wait(25);
  ACTOR_MODE_THEN_WAIT(10, 3, 20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x600a, 0, 0);
  {
    s32 a1 = 0x102;
    s32 a2 = 30;
    /* FAKEMATCH(barrier): the native loads the later arguments before the
     * first; as plain calls, 177 of 2484 instructions differ in tla-en. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Actor_ShowEmote(9, a1, a2);
  }
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  ACTOR_MODE_THEN_WAIT(10, 4, 33);
  FieldEvent_ShowMessageThenWait(0x600a, 0, 0);
  Actor_ShowEmote(9, 0x103, 45);
  FieldActor_Jump(9, 6, 15);
  FieldActor_Jump(9, 6, 23);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  Actor_ShowEmote(10, 0x108, 30);
  FieldEvent_ShowMessageThenWait(0x600a, 0, 0);
  Actor_RunRepeatedMotion(9, 2);
  FieldEvent_Wait(10);
  Actor_FaceDirection(10, 0xc000, 0);
  FieldEvent_Wait(30);
  FieldEvent_ShowMessageThenWait(0x600a, 0, 0);
  Actor_WalkByAndWait(10, 4, -12);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(10, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x200a, 0, 0);
  {
    s32 a1 = 4;
    s32 a2 = -12;
    /* FAKEMATCH(barrier): the native loads the later arguments before the
     * first; as plain calls, 177 of 2484 instructions differ in tla-en. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Actor_WalkByAndWait(10, a1, a2);
  }
  FieldEvent_Wait(10);
  ACTOR_MODE_THEN_WAIT(10, 4, 33);
  FieldEvent_ShowMessageThenWait(0x200a, 0, 0);
  Actor_FaceDirection(10, 0x4000, 0);
  FieldEvent_Wait(30);
  FieldEvent_ShowMessageThenWait(0x200a, 0, 0);
  Actor_SetAnimation(4, 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimation(6, 3);
  ACTOR_MODE_THEN_WAIT(9, 3, 20);
  FieldEvent_Wait(20);
  ACTOR_MODE_THEN_WAIT(10, 3, 20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x200a, 0, 0);
  Actor_RunRepeatedMotion(10, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x200a, 0, 0);
  Actor_RunRepeatedMotion(9, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  Actor_ShowEmote(10, 0x105, 30);
  FieldEvent_ShowMessageThenWait(0x200a, 0, 0);
  Actor_RunRepeatedMotion(10, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x200a, 0, 0);
  {
    s32 a1 = 0x101;
    s32 a2 = 30;
    /* FAKEMATCH(barrier): the native loads the later arguments before the
     * first; as plain calls, 177 of 2484 instructions differ in tla-en. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Actor_ShowEmote(6, a1, a2);
  }
  FieldEvent_ShowMessageThenWait(6, 0, 0);
  {
    s32 a1 = 0x3000;
    s32 a2 = 0;
    /* FAKEMATCH(barrier): the native loads the later arguments before the
     * first; as plain calls, 177 of 2484 instructions differ in tla-en. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Actor_FaceDirection(10, a1, a2);
  }
  FieldEvent_Wait(20);
  ACTOR_MODE_THEN_WAIT(10, 3, 20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x200a, 0, 0);
  {
    s32 a1 = 0x102;
    s32 a2 = 30;
    /* FAKEMATCH(barrier): the native loads the later arguments before the
     * first; as plain calls, 177 of 2484 instructions differ in tla-en. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Actor_ShowEmote(10, a1, a2);
  }
  FieldEvent_ShowMessageThenWait(0x200a, 0, 0);
  ACTOR_MODE_THEN_WAIT(9, 3, 20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  {
    s32 a1 = 0x5000;
    s32 a2 = 0;
    /* FAKEMATCH(barrier): the native loads the later arguments before the
     * first; as plain calls, 177 of 2484 instructions differ in tla-en. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Actor_FaceDirection(10, a1, a2);
  }
  FieldEvent_Wait(20);
  ACTOR_MODE_THEN_WAIT(10, 4, 33);
  FieldEvent_ShowMessageThenWait(0x200a, 0, 0);
  Actor_FaceDirection(9, 0x2000, 0);
  Actor_FaceDirection(4, 0xa000, 0);
  Actor_FaceDirection(6, 0x8000, 0);
  Actor_FaceDirection(5, 0, 0);
  t5 = Event_GetFlag(7);
  if (t5 == 0)
    goto L_0200bf32;
  Actor_FaceDirection(7, 0x8000, 0);
L_0200bf32:;
  FieldEvent_Wait(40);
  Actor_RunRepeatedMotion(10, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x200a, 0, 0);
  t6 = Event_GetFlag(7);
  if (t6 == 0)
    goto L_0200bf6a;
  Actor_FaceDirection(7, 0xa000, 0);
L_0200bf6a:;
  Actor_FaceDirection(9, 0xe000, 0);
  Actor_FaceDirection(4, 0xc000, 0);
  Actor_FaceDirection(5, 0xc000, 0);
  Actor_FaceDirection(6, 0xa000, 0);
  FieldEvent_Wait(25);
  Actor_RunRepeatedMotion(9, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  Actor_ShowEmote(10, 0x101, 30);
  FieldEvent_ShowMessageThenWait(0x200a, 0, 0);
  ACTOR_MODE_THEN_WAIT(9, 3, 20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  ACTOR_MODE_THEN_WAIT(10, 3, 20);
  FieldEvent_Wait(10);
  ACTOR_MODE_THEN_WAIT(10, 3, 20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x200a, 0, 0);
  {
    s32 a1 = 0x100;
    s32 a2 = 30;
    /* FAKEMATCH(barrier): the native loads the later arguments before the
     * first; as plain calls, 177 of 2484 instructions differ in tla-en. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Actor_ShowEmote(10, a1, a2);
  }
  FieldEvent_ShowMessageThenWait(0x200a, 0, 0);
  ACTOR_MODE_THEN_WAIT(9, 3, 20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  {
    s32 a1 = 0x2000;
    s32 a2 = 0;
    /* FAKEMATCH(barrier): the native loads the later arguments before the
     * first; as plain calls, 177 of 2484 instructions differ in tla-en. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Actor_FaceDirection(9, a1, a2);
  }
  FieldEvent_Wait(30);
  ACTOR_MODE_THEN_WAIT(9, 3, 20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  Actor_RunRepeatedMotion(10, 2);
  FieldEvent_Wait(15);
  {
    s32 a1 = 0xc000;
    s32 a2 = 0;
    /* FAKEMATCH(barrier): the native loads the later arguments before the
     * first; as plain calls, 177 of 2484 instructions differ in tla-en. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Actor_FaceDirection(10, a1, a2);
  }
  FieldEvent_Wait(30);
  ACTOR_MODE_THEN_WAIT(10, 3, 20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x200a, 0, 0);
  Actor_SetAnimation(4, 3);
  ACTOR_MODE_THEN_WAIT(5, 3, 20);
  FieldEvent_Wait(30);
  {
    s32 a1 = 0xe000;
    s32 a2 = 0;
    /* FAKEMATCH(barrier): the native loads the later arguments before the
     * first; as plain calls, 177 of 2484 instructions differ in tla-en. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Actor_FaceDirection(9, a1, a2);
  }
  FieldEvent_Wait(25);
  ACTOR_MODE_THEN_WAIT(9, 3, 20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  FieldActor_StartRepeatedMotion(10, 3);
  Actor_SetAttachedEffect(10, 0x102);
  FieldEvent_Wait(40);
  Actor_FaceDirection(10, 0x5000, 0);
  FieldEvent_Wait(30);
  FieldEvent_ShowMessageThenWait(0x200a, 0, 0);
  ACTOR_MODE_THEN_WAIT(9, 3, 20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  Actor_ShowEmote(10, 0x100, 30);
  FieldEvent_ShowMessageThenWait(0x200a, 0, 0);
  Actor_FaceDirection(9, 0, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  Actor_WalkByAndWait(9, 16, 0);
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(9, 16, 0);
  Actor_FaceDirection(4, 0xe000, 0);
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(9, 16, 0);
  Actor_FaceDirection(10, 0x3000, 0);
  Actor_FaceDirection(5, 0xe000, 0);
  Actor_FaceDirection(6, 0xc000, 0);
  t7 = Event_GetFlag(7);
  if (t7 == 0)
    goto L_0200c216;
  Actor_FaceDirection(7, 0xc000, 0);
L_0200c216:;
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(9, 16, 0);
  Actor_FaceDirection(6, 0xe000, 0);
  FieldEvent_Wait(20);
  FieldCamera_MoveTo(0x1580000, -1, 0xd00000, 1);
  Func_020052b4();
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  Actor_SetSpeed(10, 0x13333, 0x9999);
  Actor_WalkByAndWait(10, 16, 16);
  Actor_SetSpeed(10, 0xb333, 0x5999);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  ACTOR_MODE_THEN_WAIT(9, 3, 20);
  FieldEvent_Wait(10);
  t8 = Event_GetFlag(7);
  if (t8 == 0)
    goto L_0200c2bc;
  Actor_FaceDirection(7, 0xa000, 0);
L_0200c2bc:;
  Actor_FaceDirection(4, 0xe000, 0);
  Actor_FaceDirection(5, 0xc000, 0);
  Actor_FaceDirection(6, 0xc000, 0);
  Actor_FaceDirection(9, 0xa000, 0);
  FieldEvent_Wait(30);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  Actor_RunRepeatedMotion(10, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  Actor_RunRepeatedMotion(5, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(5, 0, 0);
  Actor_RunRepeatedMotion(10, 2);
  FieldEvent_Wait(10);
  Actor_FaceDirection(10, 0x5000, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  ACTOR_MODE_THEN_WAIT(9, 4, 33);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  Actor_FaceDirection(10, 0x2000, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  ACTOR_MODE_THEN_WAIT(9, 3, 20);
  FieldEvent_Wait(10);
  FieldEvent_Wait(20);
  Actor_ShowEmote(6, 0x106, 40);
  Actor_FaceDirection(6, 0x8000, 0);
  FieldEvent_Wait(20);
  Event_OpenMessage(6, 0);
  Actor_FaceDirection(9, 0x5000, 0);
  Actor_FaceDirection(4, 0, 0);
  Actor_FaceDirection(5, 0, 0);
  t9 = Event_GetFlag(7);
  if (t9 == 0)
    goto L_0200c400;
  Actor_FaceDirection(7, 0x8000, 0);
L_0200c400:;
  t10 = FieldEvent_AskYesNoWithActorAnimation(4, 0);
  if (t10 != 0)
    goto L_0200c434;
  FieldEvent_Wait(25);
  FieldEvent_ShowMessageThenWait(6, 0, 0);
  *(u16 *)(((s32)Ram_HeapSlots->eventWork + 0x1c4)) =
      (*(u16 *)(((s32)Ram_HeapSlots->eventWork + 0x1c4)) + 1);
  goto L_0200c456;
L_0200c434:;
  FieldEvent_Wait(40);
  *(u16 *)(((s32)Ram_HeapSlots->eventWork + 0x1c4)) =
      (*(u16 *)(((s32)Ram_HeapSlots->eventWork + 0x1c4)) + 1);
  FieldEvent_ShowMessageThenWait(6, 0, 0);
L_0200c456:;
  {
    s32 a1 = 0x101;
    s32 a2 = 0;
    /* FAKEMATCH(barrier): the native loads the later arguments before the
     * first; as plain calls, 177 of 2484 instructions differ in tla-en. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Actor_ShowEmote(9, a1, a2);
  }
  Actor_ShowEmote(5, 0x101, 0);
  t11 = Event_GetFlag(7);
  if (t11 == 0)
    goto L_0200c484;
  Actor_ShowEmote(7, 0x101, 0);
L_0200c484:;
  {
    s32 a1 = 0x101;
    s32 a2 = 50;
    /* FAKEMATCH(barrier): the native loads the later arguments before the
     * first; as plain calls, 177 of 2484 instructions differ in tla-en. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Actor_ShowEmote(10, a1, a2);
  }
  Actor_FaceDirection(9, 0x5000, 0);
  Actor_FaceDirection(6, 0xe000, 0);
  FieldEvent_Wait(30);
  FieldEvent_ShowMessageThenWait(6, 0, 0);
  Actor_ShowEmote(9, 0x101, 30);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  Actor_ShowEmote(6, 0x107, 30);
  FieldEvent_ShowMessageThenWait(6, 0, 0);
  ACTOR_MODE_THEN_WAIT(10, 4, 33);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  Actor_FaceDirection(6, 0xc000, 0);
  FieldEvent_Wait(20);
  Actor_ShowEmote(6, 0x103, 30);
  FieldEvent_ShowMessageThenWait(6, 0, 0);
  ACTOR_MODE_THEN_WAIT(6, 4, 33);
  FieldEvent_ShowMessageThenWait(6, 0, 0);
  {
    s32 a1 = 0x105;
    s32 a2 = 0;
    /* FAKEMATCH(barrier): the native loads the later arguments before the
     * first; as plain calls, 177 of 2484 instructions differ in tla-en. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Actor_ShowEmote(9, a1, a2);
  }
  Actor_ShowEmote(10, 0x105, 50);
  Actor_RunRepeatedMotion(6, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(6, 0, 0);
  Actor_SetAnimation(4, 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimation(10, 3);
  t12 = Event_GetFlag(7);
  if (t12 == 0)
    goto L_0200c596;
  Actor_SetAnimation(7, 3);
L_0200c596:;
  Actor_SetAnimation(9, 3);
  FieldEvent_Wait(20);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(6, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(6, 0, 0);
  FieldActor_StartRepeatedMotion(10, 3);
  Actor_SetAttachedEffect(10, 0x102);
  FieldActor_StartRepeatedMotion(9, 3);
  Actor_SetAttachedEffect(9, 0x102);
  FieldEvent_Wait(40);
  Actor_FaceDirection(4, 0xe000, 0);
  Actor_FaceDirection(5, 0xe000, 0);
  Actor_FaceDirection(9, 0xa000, 0);
  Actor_FaceDirection(10, 0x2000, 0);
  FieldEvent_Wait(25);
  FieldActor_Jump(9, 6, 15);
  FieldActor_Jump(9, 6, 23);
  FieldEvent_Wait(10);
  Actor_SetAnimation(9, 3);
  ACTOR_MODE_THEN_WAIT(10, 3, 20);
  FieldEvent_Wait(10);
  v5 = 0;
  FieldEvent_ShowTwoMessagesAndWait(10, 9, 2, 9, 5, 9, 17, 14, 20, 9, v5);
  t13 = Event_GetFlag(7);
  if (t13 == 0)
    goto L_0200c694;
  Actor_FaceDirection(7, 0xa000, 0);
L_0200c694:;
  FieldActor_StartRepeatedMotion(10, 3);
  Actor_SetAttachedEffect(10, 0x102);
  FieldEvent_Wait(40);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  FieldActor_StartRepeatedMotion(9, 3);
  Actor_SetAttachedEffect(9, 0x102);
  FieldEvent_Wait(40);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  Actor_ShowEmote(10, 0x100, 30);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  Actor_RunRepeatedMotion(10, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  ACTOR_MODE_THEN_WAIT(10, 3, 20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  ACTOR_MODE_THEN_WAIT(6, 3, 20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(6, 0, 0);
  Actor_RunRepeatedMotion(9, 2);
  FieldEvent_Wait(10);
  Actor_FaceDirection(9, 0x6000, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  Actor_FaceDirection(9, 0xa000, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  ACTOR_MODE_THEN_WAIT(10, 3, 20);
  FieldEvent_Wait(10);
  Actor_WalkByAndWait(9, -18, 0);
  Actor_FaceDirection(5, 0xc000, 0);
  Actor_FaceDirection(9, 0x5000, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(0x2009, 0, 0);
  Actor_FaceDirection(9, 0xc000, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(10, 0x5000, 0);
  FieldEvent_Wait(20);
  ACTOR_MODE_THEN_WAIT(10, 3, 20);
  FieldEvent_Wait(20);
  ACTOR_MODE_THEN_WAIT(5, 3, 20);
  FieldEvent_Wait(10);
  t14 = Event_GetFlag(7);
  if (t14 == 0)
    goto L_0200c880;
  Actor_FaceDirection(9, 0x2000, 0);
  FieldEvent_ShowMessageThenWait(0x2009, 0, 0);
  Actor_FaceDirection(10, 0x5000, 0);
  FieldEvent_Wait(20);
  ACTOR_MODE_THEN_WAIT(10, 3, 20);
  FieldEvent_Wait(20);
  Actor_SetAnimation(7, 3);
  FieldEvent_Wait(20);
  FieldEvent_Wait(10);
  goto L_0200c892;
L_0200c880:;
  *(u16 *)(((s32)Ram_HeapSlots->eventWork + 0x1c4)) =
      (*(u16 *)(((s32)Ram_HeapSlots->eventWork + 0x1c4)) + 1);
L_0200c892:;
  Actor_WalkByAndWait(9, -42, 0);
  Actor_FaceDirection(4, 0xc000, 0);
  {
    s32 a1 = 0x3000;
    s32 a2 = 0;
    /* FAKEMATCH(barrier): the native loads the later arguments before the
     * first; as plain calls, 177 of 2484 instructions differ in tla-en. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Actor_FaceDirection(9, a1, a2);
  }
  FieldEvent_Wait(20);
  ACTOR_MODE_THEN_WAIT(9, 3, 20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  Actor_FaceDirection(9, 0xe000, 0);
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(10, -16, 0);
  Actor_FaceDirection(6, 0xa000, 0);
  {
    s32 a1 = 0x5000;
    s32 a2 = 0;
    /* FAKEMATCH(barrier): the native loads the later arguments before the
     * first; as plain calls, 177 of 2484 instructions differ in tla-en. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Actor_FaceDirection(10, a1, a2);
  }
  FieldEvent_Wait(20);
  ACTOR_MODE_THEN_WAIT(10, 3, 20);
  FieldEvent_Wait(30);
  ACTOR_MODE_THEN_WAIT(4, 3, 20);
  FieldEvent_Wait(25);
  FieldEvent_ShowMessageThenWait(0x200a, 0, 0);
  ACTOR_MODE_THEN_WAIT(9, 4, 33);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  Actor_FaceDirection(10, 0xc000, 0);
  FieldEvent_Wait(40);
  Func_020013b0(10);
  Func_020013b0(10);
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(10, 16, 0);
  FieldEvent_Wait(20);
  FieldActor_StartRepeatedMotion(10, 3);
  Actor_SetAttachedEffect(10, 0x102);
  FieldEvent_Wait(40);
  Actor_FaceDirection(10, 0x8000, 0);
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(10, -16, 0);
  FieldEvent_Wait(10);
  {
    s32 a1 = -16;
    s32 a2 = 0;
    /* FAKEMATCH(barrier): the native loads the later arguments before the
     * first; as plain calls, 177 of 2484 instructions differ in tla-en. */
    Actor_WalkByAndWait(10, a1, a2);
  }
  FieldEvent_Wait(20);
  ACTOR_MODE_THEN_WAIT(10, 4, 33);
  {
    s32 a0 = 10;
    Actor_FaceDirection(a0, 0, 0);
  }
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(10, 16, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(10, 0xc000, 0);
  FieldEvent_Wait(20);
  FieldActor_StartRepeatedMotion(10, 3);
  Actor_SetAttachedEffect(10, 0x102);
  FieldEvent_Wait(40);
  Actor_WalkByAndWait(10, 0, -12);
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(10, 0, -12);
  FieldEvent_Wait(20);
  FieldEvent_Wait(20);
  Actor_ShowEmote(10, 0x106, 40);
  Actor_FaceDirection(10, 0x4000, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  Actor_RunRepeatedMotion(10, 2);
  FieldEvent_Wait(10);
  ACTOR_MODE_THEN_WAIT(10, 3, 20);
  FieldEvent_Wait(5);
  t15 = (s32)Actor_Get(12);
  v5 = t15;
  v8 = *(s32 *)(v5 + 80);
  *(s32 *)(v8 + 12) = 0x6666;
  FieldActor_SetSpritePriority(12, 1);
  t16 = (s32)Actor_Get(12);
  Actor_SetSingleChildFlags(t16, 0);
  Actor_SetPosition(12, 0x1480000, 0xa00000);
  FieldActor_Jump(12, 9, 0);
  v7 = 0;
  v6 = 20;
L_0200caf6:;
  t17 = Math_Divide(v7, 20);
  *(s32 *)(v8 + 12) = (t17 + 0x6666);
  {
    s32 angle = *(u16 *)(v5 + 6);
    /* FAKEMATCH(barrier): the native starts the 0x1000 step before it loads
     * the angle; as the plain sum, 2 of 2484 instructions differ in tla-en. */
    __asm__("" : "+l"(angle));
    *(u16 *)(v5 + 6) = angle + 0x1000;
  }
  System_Wait(1);
  v6 = (v6 - 1);
  v7 = (v7 + 0x9999);
  if (v6 >= 0)
    goto L_0200caf6;
  FIELD_AT(v5, u16, 6) = 0x4000;
  {
    s32 a2 = 8;
    s32 a1 = 0;

    Actor_WalkByAndWait(12, a1, a2);
  }
  t18 = (s32)Actor_Get(12);
  Actor_SetSingleChildFlags(t18, 1);
  FieldEvent_Wait(5);
  Actor_WalkByAndWait(12, 0, 8);
  FieldEvent_Wait(5);
  Actor_WalkByAndWait(12, 0, 8);
  FieldEvent_Wait(5);
  Actor_WalkByAndWait(12, 0, 8);
  FieldEvent_Wait(5);
  Actor_ShowEmote(5, 0x100, 30);
  FieldEvent_ShowMessageThenWait(0x2005, 0, 0);
  Actor_ShowEmote(10, 0x101, 30);
  Actor_FaceDirection(10, 0x3000, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(0x200a, 0, 0);
  Actor_WalkByAndWait(10, 0, 24);
  Actor_FaceDirection(10, 0x3000, 0);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x200a, 0, 0);
  Actor_FaceDirection(10, 0x5000, 0);
  FieldEvent_Wait(20);
  {
    s32 a1 = 0;

    Event_OpenMessage(0x200a, a1);
  }
  Actor_FaceDirection(9, 0x3000, 0);
  Actor_FaceDirection(5, 0x8000, 0);
  Actor_FaceDirection(6, 0x8000, 0);
  t19 = Event_GetFlag(7);
  if (t19 == 0)
    goto L_0200cc42;
  Actor_FaceDirection(7, 0x8000, 0);
L_0200cc42:;
  FieldEvent_Wait(20);
  t20 = FieldEvent_AskYesNoWithActorAnimation(4, 0);
  if (t20 != 0)
    goto L_0200cc7c;
  FieldEvent_Wait(30);
  FieldEvent_ShowMessageThenWait(0x200a, 0, 0);
  *(u16 *)(((s32)Ram_HeapSlots->eventWork + 0x1c4)) =
      (*(u16 *)(((s32)Ram_HeapSlots->eventWork + 0x1c4)) + 1);
  goto L_0200cca2;
L_0200cc7c:;
  FieldEvent_Wait(40);
  *(u16 *)(((s32)Ram_HeapSlots->eventWork + 0x1c4)) =
      (*(u16 *)(((s32)Ram_HeapSlots->eventWork + 0x1c4)) + 1);
  {
    s32 a0 = 0x200a;
    FieldEvent_ShowMessageThenWait(a0, 0, 0);
  }
L_0200cca2:;
  Actor_FaceDirection(5, 0xc000, 0);
  Actor_FaceDirection(9, 0xe000, 0);
  Actor_FaceDirection(6, 0xa000, 0);
  t21 = Event_GetFlag(7);
  if (t21 == 0)
    goto L_0200ccdc;
  Actor_FaceDirection(7, 0xa000, 0);
L_0200ccdc:;
  FieldEvent_Wait(30);
  Actor_RunRepeatedMotion(10, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x200a, 0, 0);
  Actor_SetSpeed(10, 0x26666, 0x13333);
  t22 = (s32)Actor_Get(10);
  *(u8 *)((t22 + 90)) = (254 & *(u8 *)((t22 + 90)));
  Actor_FaceDirection(10, 0x5000, 0);
  Actor_WalkByAndWait(10, 0, 8);
  Actor_WalkByAndWait(10, 0, -8);
  FieldEvent_Wait(1);
  t23 = (s32)Actor_Get(10);
  /* FAKEMATCH(temporary): the value is ORed into the register that holds the
   * constant; as one |=, 4 of 2484 instructions differ in tla-en. */
  b1 = 1;
  b1 |= *(u8 *)((t23 + 90));
  *(u8 *)((t23 + 90)) = b1;
  FieldEvent_Wait(1);
  Actor_SetSpeed(10, 0xb333, 0x5999);
  Actor_FaceDirection(10, 0x5000, 0);
  Actor_FaceDirection(9, 0x3000, 0);
  Actor_FaceDirection(5, 0x8000, 0);
  t24 = Event_GetFlag(7);
  if (t24 == 0)
    goto L_0200cda2;
  Actor_FaceDirection(7, 0x8000, 0);
L_0200cda2:;
  Actor_FaceDirection(6, 0x8000, 0);
  Djinn_JoinParty(12, 3, 9);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Actor_SetAnimation(4, 1);
  FieldEvent_Wait(20);
  Actor_FaceDirection(4, 0xc000, 0);
  Actor_FaceDirection(10, 0x4000, 0);
  FieldEvent_Wait(20);
  *(u16 *)(((s32)Ram_HeapSlots->eventWork + 0x1c4)) =
      (*(u16 *)(((s32)Ram_HeapSlots->eventWork + 0x1c4)) + 1);
  {
    s32 a1 = 0x108;
    s32 a2 = 30;
    /* FAKEMATCH(barrier): the native loads the later arguments before the
     * first; as plain calls, 177 of 2484 instructions differ in tla-en. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Actor_ShowEmote(6, a1, a2);
  }
  FieldEvent_ShowMessageThenWait(6, 0, 0);
  Actor_FaceDirection(5, 0, 0);
  FieldEvent_Wait(30);
  ACTOR_MODE_THEN_WAIT(5, 3, 20);
  FieldEvent_Wait(20);
  Actor_FaceDirection(9, 0xe000, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  Actor_FaceDirection(6, 0xa000, 0);
  {
    s32 a1 = 0xc000;
    s32 a2 = 0;
    /* FAKEMATCH(barrier): the native loads the later arguments before the
     * first; as plain calls, 177 of 2484 instructions differ in tla-en. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Actor_FaceDirection(5, a1, a2);
  }
  FieldEvent_Wait(20);
  ACTOR_MODE_THEN_WAIT(10, 4, 33);
  FieldEvent_ShowMessageThenWait(0x200a, 0, 0);
  ACTOR_MODE_THEN_WAIT(9, 3, 20);
  FieldEvent_Wait(20);
  ACTOR_MODE_THEN_WAIT(10, 3, 20);
  FieldEvent_Wait(10);
  {
    s32 a1 = 0x108;
    s32 a2 = 30;
    /* FAKEMATCH(barrier): the native loads the later arguments before the
     * first; as plain calls, 177 of 2484 instructions differ in tla-en. */
    Actor_ShowEmote(5, a1, a2);
  }
  FieldEvent_ShowMessageThenWait(0x2005, 0, 0);
  {
    s32 a1 = 0x3000;
    s32 a2 = 0;
    /* FAKEMATCH(barrier): the native loads the later arguments before the
     * first; as plain calls, 177 of 2484 instructions differ in tla-en. */
    Actor_FaceDirection(10, a1, a2);
  }
  FieldEvent_Wait(20);
  ACTOR_MODE_THEN_WAIT(10, 3, 20);
  FieldEvent_Wait(20);
  Actor_SetAnimation(4, 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimation(6, 3);
  t25 = Event_GetFlag(7);
  if (t25 == 0)
    goto L_0200cf2a;
  Actor_SetAnimation(7, 3);
L_0200cf2a:;
  Actor_SetAnimation(9, 3);
  FieldEvent_Wait(20);
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(9, 12, 0);
  Actor_FaceDirection(10, 0x5000, 0);
  Actor_FaceDirection(6, 0x8000, 0);
  Actor_FaceDirection(5, 0x8000, 0);
  Actor_FaceDirection(9, 0x4000, 0);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(9, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  ACTOR_MODE_THEN_WAIT(4, 3, 20);
  FieldEvent_Wait(20);
  t26 = Event_GetFlag(7);
  if (t26 == 0)
    goto L_0200cfbc;
  Actor_SetAnimation(7, 3);
L_0200cfbc:;
  Actor_SetAnimation(5, 3);
  Actor_SetAnimation(6, 3);
  ACTOR_MODE_THEN_WAIT(4, 3, 20);
  FieldEvent_Wait(20);
  Actor_SetSpeed(9, 0x13333, 0x9999);
  Actor_SetAnimation(9, 2);
  p1 = (s32)gMainRoutineWork;
  v5 = (p1 + 0x214);
  t27 = (s32)Actor_Get(*(s32 *)(v5));
  if (t27 == 0)
    goto L_0200d016;
  Actor_SetDestination(9, *(s16 *)(t27 + 10), *(s16 *)(t27 + 18));
L_0200d016:;
  FieldActor_WaitForMoveById(9);
  Actor_SetPosition(9, 0, 0);
  Actor_SetSpeed(5, 0x13333, 0x9999);
  Actor_SetAnimation(5, 2);
  t28 = (s32)Actor_Get(*(s32 *)(v5));
  if (t28 == 0)
    goto L_0200d054;
  Actor_SetDestination(5, *(s16 *)(t28 + 10), *(s16 *)(t28 + 18));
L_0200d054:;
  FieldActor_WaitForMoveById(5);
  Actor_SetPosition(5, 0, 0);
  Actor_SetSpeed(6, 0x13333, 0x9999);
  Actor_SetAnimation(6, 2);
  t29 = (s32)Actor_Get(*(s32 *)(v5));
  if (t29 == 0)
    goto L_0200d092;
  Actor_SetDestination(6, *(s16 *)(t29 + 10), *(s16 *)(t29 + 18));
L_0200d092:;
  FieldActor_WaitForMoveById(6);
  Actor_SetPosition(6, 0, 0);
  t30 = Event_GetFlag(7);
  if (t30 == 0)
    goto L_0200d0fa;
  Actor_SetSpeed(7, 0x13333, 0x9999);
  Actor_SetAnimation(7, 2);
  t31 = (s32)Actor_Get(*(s32 *)(v5));
  if (t31 == 0)
    goto L_0200d0ea;
  goto L_0200d0dc;
L_0200d0dc:;
  Actor_SetDestination(7, *(s16 *)(t31 + 10), *(s16 *)(t31 + 18));
L_0200d0ea:;
  FieldActor_WaitForMoveById(7);
  Actor_SetPosition(7, 0, 0);
L_0200d0fa:;
  p2 = (s32)gMainRoutineWork;
  {
    s32 a0 = *(s32 *)((p2 + 0x214));
    FieldCamera_FollowActor(a0, 1);
  }
  Func_020051b4();
}
