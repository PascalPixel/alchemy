#include "types.h"
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
s32 Func_0200593c();
extern char SceneId_Ov6a5;
extern u8 gMainRoutineWork[];
extern u8 MsgBlank5[];

static __inline__ void W_Func_020058fc(s32 a0, s32 a1, s32 a2) {
  Func_080d47b4(a0, a1, a2);
}

void Func_02001388(void) {
  s32 fast;
  s32 slow;
  s32 t24;
  s32 t25;
  s32 t26;
  s32 t27;
  s32 t66;
  s32 t67;
  s32 t68;
  s32 t69;
  s32 t70;
  s32 t20;
  s32 t21;
  s32 t22;
  s32 t23;
  s32 t33;
  s32 t34;
  s32 t35;
  s32 t36;

  Func_02005814();
  FieldEvent_ResolvePendingActions(0);
  fast = 0xa000;
  Actor_SetPositionAndFacing(4, 0x2000000, 0x1380000, fast);
  Actor_SetPositionAndFacing(7, 0x2100000, 0x1300000, fast);
  slow = 0x2000;
  Actor_SetPositionAndFacing(0x1b, 0x1f80000, 0x1300000, slow);
  {
    Actor_SetPositionAndFacing(0x1c, 0x2080000, 0x1200000, slow);
  }
  {
    s32 north = 0xc000;
    Actor_SetPositionAndFacing(5, 0x2280000, 0x1880000, north);
  }
  Actor_SetAnimation(0x1b, 9);
  Actor_SetAnimation(0x1c, 8);
  Actor_SetAnimation(4, 0x32);
  Actor_SetAnimation(7, 0xc);
  FieldCamera_MoveTo(0x2080000, 0x680000, 0x1380000, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetMessageId((s32)MsgBlank5);
  FieldEvent_ShowMessageThenWait(5, 0, 0);
  W_Func_020058fc(4, 0x100, 0);
  W_Func_020058fc(7, 0x100, 0);
  W_Func_020058fc(0x1c, 0x100, 0);
  W_Func_020058fc(0x1b, 0x100, 0x1e);
  t20 = (s32)Actor_Get(4);
  FIELD_AT((t20 + 0x5a), u8, 0) = (0xfe & FIELD_AT((t20 + 0x5a), u8, 0));
  t21 = (s32)Actor_Get(7);
  FIELD_AT((t21 + 0x5a), u8, 0) = (0xfe & FIELD_AT((t21 + 0x5a), u8, 0));
  t22 = (s32)Actor_Get(0x1b);
  FIELD_AT((t22 + 0x5a), u8, 0) = (0xfe & FIELD_AT((t22 + 0x5a), u8, 0));
  t23 = (s32)Actor_Get(0x1c);
  FIELD_AT((t23 + 0x5a), u8, 0) = (0xfe & FIELD_AT((t23 + 0x5a), u8, 0));
  t24 = (s32)Actor_Get(4);
  FIELD_AT(t24, s32, 40) = 0x30000;
  t25 = (s32)Actor_Get(7);
  FIELD_AT(t25, s32, 40) = 0x30000;
  t26 = (s32)Actor_Get(0x1b);
  FIELD_AT(t26, s32, 40) = 0x30000;
  t27 = (s32)Actor_Get(0x1c);
  FIELD_AT(t27, s32, 40) = 0x30000;
  Actor_SetDestinationOffset(4, 8, 4);
  Actor_SetDestinationOffset(7, 8, 4);
  Actor_SetDestinationOffset(0x1b, -16, -8);
  Actor_SetDestinationOffset(0x1c, -16, -8);
  FieldActor_WaitForMoveById(4);
  t33 = (s32)Actor_Get(4);
  FIELD_AT((t33 + 0x5a), u8, 0) = (FIELD_AT((t33 + 0x5a), u8, 0) | 1);
  t34 = (s32)Actor_Get(7);
  FIELD_AT((t34 + 0x5a), u8, 0) = (FIELD_AT((t34 + 0x5a), u8, 0) | 1);
  t35 = (s32)Actor_Get(0x1b);
  FIELD_AT((t35 + 0x5a), u8, 0) = (FIELD_AT((t35 + 0x5a), u8, 0) | 1);
  t36 = (s32)Actor_Get(0x1c);
  FIELD_AT((t36 + 0x5a), u8, 0) = (1 | FIELD_AT((t36 + 0x5a), u8, 0));
  FieldCamera_FollowActor(4, 1);
  Actor_WalkToAndWait(5, 0x228, 0x158);
  Actor_ShowEmote(5, 0x102, 0x1e);
  FieldEvent_ShowMessageThenWait(5, 0, 0);
  Actor_FaceDirectionAndWait(7, 0x4000);
  FieldEvent_Wait(0xa);
  FieldEvent_ShowMessageThenWait(7, 0, 0);
  Actor_FaceDirectionAndWait(0x1c, 0x6000);
  Actor_ShowEmote(0x1c, 0x107, 0x1e);
  FieldEvent_ShowMessageThenWait(0x1c, 0, 0);
  FieldEvent_ShowMessageThenWait(0x1b, 0, 0);
  Actor_FaceDirection(0x1c, slow, 0);
  Actor_FaceDirectionAndWait(0x1b, slow);
  Actor_FaceDirectionAndWait(5, fast);
  FieldEvent_Wait(0xa);
  Event_ShowMessage(5, 0);
  Actor_WalkBy(5, -8, -24);
  Actor_FaceDirectionAndWait(4, slow);
  /* FAKEMATCH(one-pass): the statement in a once-only block, whose end holds
   * the order around it; as the plain statement, 2 of 349 instructions differ
   * in tla-en. */
  do {
    Actor_SetAnimation(4, 3);
  } while (0);
  /* FAKEMATCH(one-pass): the statement in a once-only block, whose end holds
   * the order around it; as the plain statement, 2 of 349 instructions differ
   * in tla-en. */
  do {
    FieldEvent_Wait(0x14);
  } while (0);
  Actor_FaceDirection(7, fast, 0);
  Actor_FaceDirectionAndWait(4, fast);
  Actor_FaceDirectionAndWait(5, fast);
  FieldEvent_Wait(0xa);
  Actor_SetDestinationOffset(4, -16, -8);
  Actor_SetDestinationOffset(7, -16, -8);
  Actor_SetDestinationOffset(5, -16, -8);
  Actor_SetDestinationOffset(0x1b, 0x10, 8);
  Actor_SetDestinationOffset(0x1c, 0x10, 8);
  t66 = (s32)Actor_Get(4);
  FIELD_AT(t66, s32, 40) = 0x40000;
  t67 = (s32)Actor_Get(7);
  FIELD_AT(t67, s32, 40) = 0x40000;
  t68 = (s32)Actor_Get(5);
  FIELD_AT(t68, s32, 40) = 0x40000;
  t69 = (s32)Actor_Get(0x1b);
  FIELD_AT(t69, s32, 40) = 0x40000;
  t70 = (s32)Actor_Get(0x1c);
  FIELD_AT(t70, s32, 40) = 0x40000;
  FieldEvent_Wait(4);
  if (FIELD_AT(gMainRoutineWork, s16, 598) == -1) {
    FieldUnit_AddHp(4, -10);
  }
  /* FAKEMATCH(one-pass): the statement in a once-only block, whose end holds
   * the order around it; as the plain statement, 4 of 349 instructions differ
   * in tla-en. */
  do {
    FIELD_AT(gMainRoutineWork, u8, 587) = 2;
  } while (0);
  Event_SetAfterBattleDestination((s32)&SceneId_Ov6a5, 0xb);
  Event_SetDefeatDestination((s32)&SceneId_Ov6a5, 0xb);
  Call2(Func_0200593c, 0xd, 6);
}
