#include "types.h"
#include "event_inline.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Animation_ApplyChildValues();
s32 Event_StartScreenTransition();
s32 Event_WaitTransitionFrames();
s32 Func_020006b0();
s32 Func_02000a68();
s32 Func_02000a70();
s32 Func_02000af0();
s32 Actor_SetSingleChildFlags();
extern u8 Func_02000064[];
extern u8 MsgShipSafeFiringDistance[];

void Func_020001d8(void) {
  /* FAKEMATCH(wrapper): calls forwarded through inlines (call.h's Call and
   * Value, this file's own); as direct calls, 174 of 438 instructions differ in
   * tla-en. */
  s32 from[3];
  s32 to[3];
  s32 v_r5, v_r8, v_r6, v_r7, v_r10, t0, t1, t2, t3, t10;
  s32 t11, t12, t13, t14, t16, t18, t20, t26, t31, t36;
  s32 t37, t40, t41, t45, t52, t53, t59, t60, t61, t66;
  s32 t70, t79, t82, t85, t88, t91, t96;

  t0 = (s32)Actor_Get(9);
  t1 = (s32)Actor_Get(10);
  t2 = (s32)Actor_Get(8);
  t3 = Event_GetFlag(0x90e);
  v_r7 = t2;
  v_r6 = t0;
  v_r8 = t1;
  v_r5 = t3;
  if (v_r5 == 0)
    goto L_0200820c;
  goto L_020085f2;
L_0200820c:
  Func_02000a68();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgShipSafeFiringDistance);
  Actor_MoveToAndWait(8, 0x1d0, 216);
  Actor_FaceDirectionAndWait(8, 0xc000);
  FieldEvent_Wait(5);
  t10 = (s32)Actor_Get(11);
  FIELD_AT(t10, s32, 24) = v_r5;
  t11 = (s32)Actor_Get(7);
  FIELD_AT(t11, s32, 24) = v_r5;
  t12 = (s32)Actor_Get(5);
  FIELD_AT(t12, s32, 24) = v_r5;
  t13 = (s32)Actor_Get(6);
  FIELD_AT(t13, s32, 24) = v_r5;
  t14 = (s32)Actor_Get(11);
  Actor_SetSingleChildFlags(t14, 0);
  t16 = (s32)Actor_Get(7);
  Actor_SetSingleChildFlags(t16, 0);
  t18 = (s32)Actor_Get(5);
  Actor_SetSingleChildFlags(t18, 0);
  t20 = (s32)Actor_Get(6);
  Actor_SetSingleChildFlags(t20, 0);
  Actor_SetPosition(11, 0x1d00000, 0xd80000);
  Actor_SetPosition(7, 0x1d00000, 0xd80000);
  Actor_SetPosition(5, 0x1d00000, 0xd80000);
  Actor_SetPosition(6, 0x1d00000, 0xd80000);
  t26 = Event_GetFlag(0x90d);
  v_r10 = (s32)MsgShipSafeFiringDistance;
  if (t26 != 0)
    goto L_020082ea;
  Event_SetFlag(0x90d);
  FieldEvent_ShowMessageThenWait(0x2007, 0, 5);
L_020082ea:
  FieldEvent_SetMessageId((v_r10 + 1));
  Event_OpenMessage(11, 0);
  t31 = FieldEvent_AskYesNoWithActorAnimation(4, 0);
  if (t31 == 0)
    goto L_0200832c;
  FieldEvent_ShowMessageThenWait(11, 0, 5);
  Actor_SetDestinationOffset(8, 0, 16);
  Actor_WaitForMove(v_r7);
  FieldEvent_Wait(5);
  goto L_020085c6;
L_0200832c:
  t36 = FIELD_AT(0x3000000, s32, 108);
  t37 = FIELD_AT(t36, u16, 452);
  FIELD_AT(t36, u16, 452) = (t37 + 1);
  FieldEvent_ShowMessageThenWait(11, 0, 5);
  Actor_SetSpeed(8, 0x30000, 0x18000);
  t40 = (s32)Actor_Get(8);
  FIELD_AT(t40, u8, 90) &= 254;
  Actor_WalkBy(8, 0, 16);
  Event_PlaySound(144);
  FieldEvent_Wait(4);
  FIELD_AT(v_r8, s32, 24) = 0x18000;
  FIELD_AT(v_r8, s32, 28) = 0x18000;
  t45 = (s32)Actor_Get(10);
  Animation_ApplyChildValues(t45, 6);
  Actor_SetPosition(10, 0x1d00000, 0xc80000);
  Actor_SetAnimation(10, 1);
  FIELD_AT(v_r6, s32, 24) = 0x8000;
  FIELD_AT(v_r6, s32, 28) = 0x8000;
  Actor_SetSpeed(9, 0x20000, 0x10000);
  Actor_SetPosition(9, 0x1d00000, 0xd80000);
  Actor_WalkTo(9, 0x1d0, 144);
  v_r5 = 15;
L_020083da:
  t52 = FIELD_AT(v_r6, s32, 24);
  FIELD_AT(v_r6, s32, 24) = (t52 + 0x800);
  t53 = FIELD_AT(v_r6, s32, 28);
  FIELD_AT(v_r6, s32, 28) = (t53 + 0x800);
  System_Wait(1);
  v_r5 = (v_r5 - 1);
  if (v_r5 >= 0)
    goto L_020083da;
  FieldCamera_MoveTo(0x1d80000, -1, 0xb00000, 1);
  Func_02000af0();
  FieldActor_WaitForMoveById(9);
  Event_PlaySound(145);
  t59 = FIELD_AT(v_r6, s32, 8);
  from[0] = (t59 - 0x20000);
  t60 = FIELD_AT(v_r6, s32, 12);
  from[1] = t60;
  t61 = FIELD_AT(v_r6, s32, 16);
  from[2] = (t61 - 0x80000);
  to[0] = 0x1900000;
  to[1] = 0;
  to[2] = 0x500000;
  Func_020006b0((s32)from, to, Func_02000064);
  FieldEvent_Wait(20);
  Actor_SetPosition(10, 0, 0);
  Actor_SetPosition(9, 0, 0);
  t66 = FIELD_AT(0x3000000, s32, 108);
  FIELD_AT(t66, s32, 436) = 100;
  PaletteFade_ApplyTarget(0x10000, 0);
  /* FAKEMATCH(one-pass): the statement in a once-only block, whose end holds
   * the order around it; as the plain statement, 2 of 438 instructions differ
   * in tla-en. */
  do {
    Event_StartScreenTransition();
  } while (0);
  Event_WaitTransitionFrames();
  t70 = FIELD_AT(0x3000000, s32, 108);
  FIELD_AT(t70, s32, 436) = 8;
  FieldEvent_ShowMessageThenWait(0x2005, 0, 5);
  FieldEvent_ShowMessageThenWait(11, 0, 5);
  FieldEvent_ShowMessageThenWait(0x2006, 0, 5);
  Actor_SetSpeed(8, 0xcccc, 0x6666);
  Actor_MoveToAndWait(8, 0x1d0, 176);
  FieldCamera_FollowActor(8, 1);
  Actor_MoveToAndWait(8, 0x1d0, 152);
  Actor_SetSpeed(8, 0x9999, 0x4ccc);
  t79 = FIELD_AT(v_r7, s32, 28);
  FIELD_AT(v_r7, s32, 28) = (t79 - 0x1000);
  FIELD_AT(v_r7, s32, 24) = (t79 - 0x1000);
  Actor_SetDestinationOffset(8, 0, -16);
  FieldActor_WaitForMoveById(8);
  t82 = FIELD_AT(v_r7, s32, 28);
  FIELD_AT(v_r7, s32, 28) = (t82 - 0x1000);
  FIELD_AT(v_r7, s32, 24) = (t82 - 0x1000);
  Actor_SetDestinationOffset(8, 0, -16);
  FieldActor_WaitForMoveById(8);
  t85 = FIELD_AT(v_r7, s32, 28);
  FIELD_AT(v_r7, s32, 28) = (t85 - 0x1000);
  FIELD_AT(v_r7, s32, 24) = (t85 - 0x1000);
  Actor_SetDestinationOffset(8, 0, -16);
  FieldActor_WaitForMoveById(8);
  t88 = FIELD_AT(v_r7, s32, 28);
  FIELD_AT(v_r7, s32, 28) = (t88 - 0x1000);
  FIELD_AT(v_r7, s32, 24) = (t88 - 0x1000);
  Actor_SetDestinationOffset(8, 0, -16);
  FieldActor_WaitForMoveById(8);
  t91 = FIELD_AT(v_r7, s32, 28);
  FIELD_AT(v_r7, s32, 28) = (t91 - 0x1000);
  FIELD_AT(v_r7, s32, 24) = (t91 - 0x1000);
  Actor_SetDestinationOffset(8, 0, -16);
  FieldActor_WaitForMoveById(8);
  Actor_SetSpeed(8, 0x3333, 0x1999);
  Actor_SetDestinationOffset(8, 0, -12);
  v_r5 = 19;
L_02008594:
  t96 = FIELD_AT(v_r7, s32, 28);
  FIELD_AT(v_r7, s32, 28) = (t96 - 0x900);
  FIELD_AT(v_r7, s32, 24) = (t96 - 0x900);
  System_Wait(7);
  v_r5 = (v_r5 - 1);
  if (v_r5 >= 0)
    goto L_02008594;
  FIELD_AT(v_r7, s32, 28) = 0;
  FIELD_AT(v_r7, s32, 24) = 0;
  FieldEvent_Wait(10);
  Event_SetFlag(0x90e);
  FieldEvent_SetExitRequest(4);
L_020085c6:
  Actor_SetPosition(11, 0, 0);
  Actor_SetPosition(5, 0, 0);
  Actor_SetPosition(6, 0, 0);
  Actor_SetPosition(7, 0, 0);
  Func_02000a70();
L_020085f2:
  return;
}
