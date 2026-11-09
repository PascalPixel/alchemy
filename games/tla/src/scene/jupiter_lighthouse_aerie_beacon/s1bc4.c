#include "types.h"
#include "call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "owner_state.h"
#include "event_inline.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Event_FinishScreenTransition();
s32 Event_StartScreenTransition();
s32 Event_WaitTransitionFrames();
s32 Func_02001ac4();
s32 Func_02003a6c();
s32 Func_02003c58();
s32 Func_02005814();
s32 Func_0200581c();
s32 Func_02005924();
s32 Func_0200598c();
extern u8 MsgWasnTThe[];
extern u8 gData_02005ff0[];
extern u8 gData_02005ff8[];
extern u8 gData_02006088[];
extern u8 gData_020061e0[];

static __inline__ void W_Func_020058fc(s32 a0, s32 a1, s32 a2) {
  /* FAKEMATCH(wrapper): W_Func_020058fc forwarded inline; as direct calls to
   * Func_080d47b4, 71 of 1518 instructions differ in tla-en. */
  Func_080d47b4(a0, a1, a2);
}

#define ACTOR_SET_MODE_AND_WAIT(id, mode, frames)                              \
  do {                                                                         \
    Actor_SetModeById(id, mode);                                               \
    Event_Wait(frames);                                                        \
  } while (0)

void Func_02001bc4(void) {
  s32 fast;
  s32 slow;
  s32 mid;
  struct Actor *actor;
  s32 two, v_r5, t302, t310, t311, t438;

  void Func_02001b04(s32 clearStatus) {
    s32 count;
    s32 n;
    struct BattleUnit *member;
    s16 maxHp;

    count = Party_CountActiveOwners();
    for (n = 0; n < count; n++) {
      struct MainRoutineWork *party = &gMainRoutineWork;

      member = Unit_Get(party->activeOwners[n]);
      maxHp = member->hp = member->maxHp;
      member->hpGauge = Vitals_Clamp((maxHp << 14) / maxHp);
      if (member->hpGauge == 0 && member->hp != 0)
        member->hpGauge = 1;
      member->ppGauge = Vitals_Clamp((member->pp << 14) / member->maxPp);
      if (member->ppGauge == 0 && member->pp != 0)
        member->ppGauge = 1;
      if (clearStatus == 1) {
        member->poison = 0;
        member->haunt = 0;
      }
    }
  }

  Func_02005814();
  Event_ResolvePendingActions(0);
  Actor_GetById(0)->priorityFlags |= 1;
  Actor_GetById(1)->priorityFlags |= 1;
  Actor_GetById(2)->priorityFlags |= 1;
  Actor_GetById(3)->priorityFlags |= 1;
  Actor_GetById(4)->priorityFlags |= 1;
  Actor_GetById(5)->priorityFlags |= 1;
  Actor_GetById(7)->priorityFlags |= 1;
  Actor_GetById(6)->priorityFlags |= 1;
  Actor_GetById(30)->priorityFlags |= 1;
  Actor_GetById(27)->priorityFlags |= 1;
  Actor_GetById(28)->priorityFlags |= 1;
  mid = 0xc000;
  fast = 0xa000;
  Actor_SetPositionAndFacing(4, 0x1f80000, 0x1300000, fast);
  Actor_SetPositionAndFacing(7, 0x2100000, 0x1200000, fast);
  slow = 0x2000;
  Actor_SetPositionAndFacing(27, 0x1e80000, 0x1280000, slow);
  Actor_SetPositionAndFacing(28, 0x1f80000, 0x1180000, slow);
  Actor_SetPositionAndFacing(5, 0x2100000, 0x1300000, fast);
  Actor_SetPositionAndFacing(6, 0x2280000, 0x1200000, mid);
  Actor_SetPositionAndFacing(29, 0x1e00000, 0x1680000, mid);
  Actor_SetPositionAndFacing(30, 0x2500000, 0x1880000, 0);
  Actor_SetModeById(5, 19);
  Actor_SetModeById(6, 19);
  Actor_SetModeById(4, 19);
  Actor_SetModeById(7, 13);
  actor = Actor_GetById(4);
  actor->scaleX = -actor->scaleX;
  actor = Actor_GetById(6);
  actor->scaleX = -actor->scaleX;
  FieldCamera_MoveTo(0x2080000, 0x680000, 0x1380000, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_SetMessageId((s32)MsgWasnTThe);
  Actor_SetModeById(27, 9);
  Actor_SetModeById(28, 8);
  Motion_SetVarCbAndRefresh(28, 2);
  Event_Wait(5);
  FieldEvent_ShowMessageThenWait(28, 0, 0);
  Actor_FaceDirectionAndWait(27, 0xe000);
  W_Func_020058fc(27, 0x108, 30);
  FieldEvent_ShowMessageThenWait(27, 0, 0);
  Motion_SetVarCbAndRefresh(4, 2);
  Event_Wait(5);
  Actor_ArmCallbackAndRefresh(28, slow);
  Actor_ArmCallbackAndRefresh(27, slow);
  Event_Wait(10);
  W_Func_020058fc(27, 0x102, 0);
  W_Func_020058fc(28, 0x102, 30);
  Actor_RunRepeatedMotion(28, 2);
  FieldEvent_ShowMessageThenWait(28, 0, 0);
  ACTOR_SET_MODE_AND_WAIT(27, 4, 33);
  Event_ShowMessageThenWait(27, 0, 0);
  W_Func_020058fc(28, 0x107, 30);
  FieldEvent_ShowMessageThenWait(28, 0, 0);
  Event_ShowMessageThenWait(29, 0, 0);
  Sound_Play(78);
  Actor_FaceDirection(27, 0x4000, 0);
  Actor_FaceDirectionAndWait(28, 0x4000);
  Event_Wait(10);
  Camera_MoveWithinMapBounds(0x2080000, 0x680000, 0x1580000, 1);
  Func_02005924();
  Event_Wait(10);
  Sound_Play(39);
  W_Func_020058fc(27, 0x107, 30);
  FieldEvent_ShowMessageThenWait(27, 0, 0);
  ACTOR_SET_MODE_AND_WAIT(29, 4, 33);
  Event_ShowMessageThenWait(29, 0, 0);
  W_Func_020058fc(28, 0x107, 30);
  FieldEvent_ShowMessageThenWait(28, 0, 0);
  Func_080d47b4(29, 0x108, 30);
  FieldEvent_ShowMessageThenWait(29, 0, 0);
  Actor_SetModeById(28, 1);
  Actor_FaceDirection(28, 0x6000, 0);
  Actor_FaceDirectionAndWait(27, 0xe000);
  Event_Wait(10);
  ACTOR_SET_MODE_AND_WAIT(29, 4, 33);
  Event_ShowMessageThenWait(29, 0, 0);
  Actor_FaceDirectionAndWait(29, 0);
  FieldEvent_ShowMessageThenWait(29, 0, 0);
  FieldEvent_ShowMessageThenWait(30, 0, 0);
  FieldActor_StartRepeatedMotion(28, 2);
  Actor_RunRepeatedMotion(27, 2);
  Actor_ArmCallback(28, slow, 0);
  Actor_ArmCallbackAndRefresh(27, slow);
  Event_Wait(5);
  ACTOR_SET_MODE_AND_WAIT(29, 4, 33);
  Actor_ArmCallbackAndRefresh(29, mid);
  FieldEvent_ShowMessageThenWait(29, 0, 0);
  W_Func_020058fc(28, 0x102, 30);
  Actor_FaceDirection(28, 0x4000, 0);
  FieldEvent_ShowMessageThenWait(28, 0, 0);
  ACTOR_SET_MODE_AND_WAIT(29, 3, 20);
  Event_ShowMessageThenWait(29, 0, 0);
  Actor_FaceDirectionAndWait(28, 0x6000);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(27, 0xe000);
  ACTOR_SET_MODE_AND_WAIT(27, 3, 20);
  Event_ShowMessageThenWait(27, 0, 0);
  ACTOR_SET_MODE_AND_WAIT(29, 3, 20);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(29, 0, 0);
  Actor_SetModeById(27, 3);
  ACTOR_SET_MODE_AND_WAIT(28, 3, 20);
  Event_Wait(10);
  FieldCamera_FollowActor(27, 1);
  Actor_SetMovementTypeById(27, (s32)gData_02005ff8);
  Event_Wait(30);
  Actor_SetMovementTypeById(29, (s32)gData_02005ff8);
  Actor_SetMovementTypeById(28, (s32)gData_02005ff8);
  Actor_RefreshSelectorById(29);
  Actor_GetById(29)->priorityFlags |= 64;
  Actor_GetById(27)->priorityFlags |= 64;
  Actor_GetById(28)->priorityFlags |= 64;
  Actor_GetById(29)->motionFlags = 2;
  Actor_SetMovementTypeById(29, (s32)gData_02006088);
  Actor_RefreshSelectorById(27);
  Actor_SetMovementTypeById(27, (s32)gData_02006088);
  Actor_RefreshSelectorById(28);
  Actor_SetMovementTypeById(28, (s32)gData_02006088);
  Actor_RefreshSelectorById(29);
  Actor_WalkToAndWait(29, 0x137, 0x100);
  Actor_FaceDirection(29, 0x4000, 0);
  Actor_RefreshSelectorById(27);
  Actor_WalkToAndWait(27, 0x14c, 0x10a);
  Actor_FaceDirection(27, 0x4000, 0);
  Actor_RefreshSelectorById(28);
  Actor_WalkToAndWait(28, 0x122, 0x10a);
  Actor_FaceDirection(28, 0x4000, 0);
  Actor_GetById(29)->priorityFlags |= 32;
  Actor_GetById(27)->priorityFlags |= 32;
  Actor_GetById(28)->priorityFlags |= 32;
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Sound_Play(78);
  FieldEvent_ShowMessageThenWait(30, 0, 0);
  Actor_SetPositionAndFacing(30, 0x2000000, 0x1480000, mid);
  Func_02003a6c(0);
  Func_0200598c();
  Event_Wait(10);
  Actor_SetPositionAndFacing(0, 0x2180000, 0x1700000, mid);
  Actor_SetPositionAndFacing(1, 0x2180000, 0x1800000, mid);
  Actor_SetPositionAndFacing(2, 0x2280000, 0x1700000, mid);
  Actor_SetPositionAndFacing(3, 0x2280000, 0x1600000, mid);
  FieldEvent_ShowMessageThenWait(0, 0, 0);
  FieldCamera_FollowActor(0, 1);
  Actor_WalkTo(0, 0x218, 0x150);
  Actor_WalkTo(1, 0x218, 0x160);
  Actor_WalkTo(2, 0x228, 0x150);
  Actor_WalkTo(3, 0x228, 0x140);
  Actor_WaitForMoveById(0);
  Actor_WaitForMoveById(1);
  Actor_WaitForMoveById(2);
  Actor_WaitForMoveById(3);
  Actor_ArmCallbackAndRefresh(30, 0x3000);
  Event_Wait(10);
  ACTOR_SET_MODE_AND_WAIT(30, 4, 33);
  Event_ShowMessageThenWait(30, 0, 0);
  ACTOR_SET_MODE_AND_WAIT(2, 4, 33);
  Event_ShowMessageThenWait(2, 0, 0);
  Actor_FaceDirectionAndWait(3, 0x4000);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(3, 0, 0);
  Actor_ArmCallbackAndRefresh(0, fast);
  FieldCamera_MoveTo(0x1400000, 0x600000, 0x1680000, 1);
  Func_02005924();
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0, 0, 0);
  Camera_MoveWithinMapBounds(0x2000000, 0x600000, 0x1600000, 1);
  Func_02005924();
  Func_02003c58(0);
  ACTOR_SET_MODE_AND_WAIT(1, 4, 33);
  Event_ShowMessageThenWait(1, 0, 0);
  Actor_ArmCallbackAndRefresh(2, fast);
  Event_Wait(10);
  Motion_SetVarCbAndRefresh(2, 2);
  Event_Wait(5);
  FieldEvent_ShowMessageThenWait(2, 0, 0);
  Func_080d47b4(3, 0x102, 30);
  FieldEvent_ShowMessageThenWait(3, 0, 0);
  Motion_SetVarCbAndRefresh(4, 2);
  Event_Wait(5);
  Actor_ArmCallbackAndRefresh(30, mid);
  Motion_SetVarCbAndRefresh(30, 2);
  Event_Wait(5);
  Event_ShowMessageThenWait(30, 0, 0);
  Event_Wait(10);
  Actor_StartRepeatedMotion(5, 2);
  Event_Wait(5);
  Actor_StartRepeatedMotion(7, 2);
  Event_Wait(5);
  Motion_SetVarCbAndRefresh(6, 2);
  Event_Wait(30);
  Actor_FaceDirectionAndWait(0, 0xe000);
  FieldEvent_ShowMessageThenWait(0, 0, 0);
  ACTOR_SET_MODE_AND_WAIT(3, 3, 20);
  Event_Wait(10);
  Actor_ArmCallbackAndRefresh(1, fast);
  FieldEvent_ShowMessageThenWait(1, 0, 0);
  Actor_ArmCallbackAndRefresh(30, slow);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(30, 0, 0);
  Actor_ArmCallbackAndRefresh(30, mid);
  Actor_WalkByAndWait(3, -0x30, 0);
  Actor_ArmCallback(3, mid, 0);
  Call2(Func_02001ac4, 3, 4);
  Actor_ArmCallback(4, slow, 0);
  actor = Actor_GetById(4);
  actor->scaleX = -actor->scaleX;
  Actor_CommitPositionAndActivate(3, 40, 0);
  Actor_WalkByAndWait(3, 8, -16);
  Actor_ArmCallback(3, mid, 0);
  Call2(Func_02001ac4, 3, 6);
  Actor_FaceDirection(6, 0x4000, 0);
  actor = Actor_GetById(6);
  actor->scaleX = -actor->scaleX;
  Actor_ArmCallback(3, fast, 0);
  Call2(Func_02001ac4, 3, 7);
  Actor_FaceDirection(7, 0x4000, 0);
  Actor_FaceDirection(3, 0x8000, 0);
  Call2(Func_02001ac4, 3, 5);
  Actor_FaceDirection(5, 0x4000, 0);
  Actor_RunRepeatedMotion(0, 2);
  Actor_ArmCallbackAndRefresh(0, fast);
  FieldEvent_ShowMessageThenWait(0, 0, 0);
  Actor_FaceDirection(30, slow, 0);
  W_Func_020058fc(30, 0x109, 30);
  FieldEvent_ShowMessageThenWait(30, 0, 0);
  Func_080d47b4(1, 0x107, 30);
  FieldEvent_ShowMessageThenWait(1, 0, 0);
  Func_080d47b4(2, 0x109, 30);
  Actor_WalkTo(2, 0x228, 0x140);
  Actor_FaceDirectionAndWait(2, 0x8000);
  Actor_FaceDirection(1, mid, 0);
  ACTOR_SET_MODE_AND_WAIT(2, 4, 33);
  Motion_SetVarCbAndRefresh(2, 2);
  FieldEvent_ShowMessageThenWait(2, 0, 0);
  Actor_FaceDirectionAndWait(0, 0xe000);
  Motion_SetVarCbAndRefresh(0, 2);
  Event_Wait(5);
  FieldEvent_ShowMessageThenWait(0, 0, 0);
  Actor_FaceDirectionAndWait(2, 0x6000);
  Motion_SetVarCbAndRefresh(2, 2);
  Event_Wait(5);
  FieldEvent_ShowMessageThenWait(2, 0, 0);
  Actor_FaceEachOther(5, 6, 0);
  Actor_TurnToFaceEachOther(7, 4, 0);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(30, 0);
  ACTOR_SET_MODE_AND_WAIT(30, 3, 20);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(30, 0);
  FieldEvent_ShowMessageThenWait(30, 0, 0);
  Actor_FaceDirection(4, 0x4000, 0);
  Actor_FaceDirection(5, 0x4000, 0);
  Actor_FaceDirection(7, 0x4000, 0);
  Actor_FaceDirection(6, 0x4000, 0);
  Actor_FaceDirection(3, 0x6000, 0);
  Actor_ArmCallbackAndRefresh(0, 0x4000);
  Event_Wait(50);
  ACTOR_SET_MODE_AND_WAIT(1, 3, 20);
  Event_Wait(10);
  Actor_ArmCallbackAndRefresh(0, mid);
  Event_Wait(50);
  Actor_SetModeById(3, 3);
  Actor_SetModeById(7, 3);
  Actor_SetModeById(6, 3);
  ACTOR_SET_MODE_AND_WAIT(5, 3, 20);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(0, fast);
  Actor_FaceDirectionAndWait(4, slow);
  t302 = Event_AskYesNoWithActorAnimation(4, 0);
  if (t302 != 0)
    goto L_0200a678;
  ACTOR_SET_MODE_AND_WAIT(0, 3, 20);
  Event_Wait(10);
  Event_ShowMessageThenWait(0, 0, 0);
  goto L_0200a6fe;
L_0200a678:
  Func_080d47b4(0, 0x107, 30);
  ACTOR_SET_MODE_AND_WAIT(5, 4, 33);
  t310 = FIELD_AT(0x3000000, s32, 108);
  t311 = FIELD_AT(t310, u16, 452);
  FIELD_AT(t310, u16, 452) = (t311 + 2);
  Actor_FaceDirectionAndWait(5, 0x8000);
  FieldEvent_ShowMessageThenWait(5, 0, 0);
  Actor_ArmCallbackAndRefresh(5, 0x4000);
  Event_Wait(10);
  Motion_SetVarCbAndRefresh(5, 2);
  Event_Wait(5);
  FieldEvent_ShowMessageThenWait(5, 0, 0);
  ACTOR_SET_MODE_AND_WAIT(0, 3, 20);
  Event_Wait(10);
  Event_ShowMessageThenWait(0, 0, 0);
L_0200a6fe:
  Actor_SetModeById(2, 2);
  actor = Actor_GetById(0);
  if (actor == 0)
    goto L_0200a71e;
  Actor_ResetAndSetPosition(2, FIELD_AT(actor, s16, 10),
                            FIELD_AT(actor, s16, 18));
L_0200a71e:
  Actor_SetModeById(3, 2);
  actor = Actor_GetById(0);
  if (actor == 0)
    goto L_0200a73e;
  Actor_ResetAndSetPosition(3, FIELD_AT(actor, s16, 10),
                            FIELD_AT(actor, s16, 18));
L_0200a73e:
  Actor_SetModeById(1, 2);
  actor = Actor_GetById(0);
  if (actor == 0)
    goto L_0200a75e;
  Actor_ResetAndSetPosition(1, FIELD_AT(actor, s16, 10),
                            FIELD_AT(actor, s16, 18));
L_0200a75e:
  Actor_SetMovementTypeById(2, (s32)gData_02005ff0);
  Actor_SetMovementTypeById(3, (s32)gData_02005ff0);
  Actor_SetMovementTypeById(1, (s32)gData_02005ff0);
  Actor_RefreshSelectorById(2);
  Actor_RefreshSelectorById(3);
  Actor_RefreshSelectorById(1);
  FieldCamera_FollowActor(0, 1);
  FieldActor_WatchAndSetScript(4, 0);
  FieldActor_WatchAndSetScript(7, 0);
  FieldActor_WatchAndSetScript(5, 0);
  FieldActor_WatchAndSetScript(6, 0);
  Actor_WatchAndSetScript(30, 0);
  Event_Wait(10);
  Actor_SetSpeed(0, 0x18000, 0xc000);
  Actor_SetPositionAndReset(0, 0x21f, 0x16b);
  Actor_WaitForMoveById(0);
  Actor_FaceDirectionAndWait(0, 0xc000);
  Motion_SetVarCbAndRefresh(0, 2);
  Event_Wait(5);
  FieldEvent_ShowMessageThenWait(0, 0, 0);
  Actor_WalkToAndWait(0, 0x23d, 0x166);
  Actor_WalkToAndWait(0, 0x248, 0x12c);
  Actor_WalkToAndWait(0, 0x248, 252);
  Actor_WalkToAndWait(0, 0x268, 247);
  Actor_SetPositionAndReset(0, 0x269, 0x138);
  Actor_GetById(0)->motionFlags = 2;
  Actor_SetMovementTypeById(0, (s32)gData_020061e0);
  Actor_RefreshSelectorById(0);
  Actor_FaceDirectionAndWait(0, 0x4000);
  Event_Wait(30);
  Actor_GetById(0)->priorityFlags |= 32;
  Actor_FaceDirection(4, 0, 0);
  Actor_FaceDirection(7, 0, 0);
  Actor_FaceDirection(5, 0, 0);
  Actor_FaceDirection(6, 0, 0);
  Actor_ArmCallback(30, 0, 0);
  Func_02003a6c(1);
  Event_Wait(100);
  Actor_FaceDirection(4, 0x8000, 0);
  Actor_FaceDirection(7, 0x8000, 0);
  Actor_FaceDirection(5, 0x8000, 0);
  Actor_FaceDirection(6, 0x8000, 0);
  Actor_ArmCallback(30, 0x8000, 0);
  Func_02003c58(1);
  Event_Wait(60);
  Camera_FollowActor(4, 1);
  Func_02005924();
  Event_Wait(60);
  Actor_ArmCallbackAndRefresh(4, 0);
  Event_Wait(60);
  ACTOR_SET_MODE_AND_WAIT(5, 3, 20);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(4, 0xe000);
  Actor_SetModeById(7, 3);
  ACTOR_SET_MODE_AND_WAIT(6, 3, 20);
  Actor_SetModeById(7, 2);
  actor = Actor_GetById(4);
  two = 2;
  v_r5 = (s32)gData_02005ff0;
  if (actor == 0)
    goto L_0200a980;
  Actor_ResetAndSetPosition(7, FIELD_AT(actor, s16, 10),
                            FIELD_AT(actor, s16, 18));
L_0200a980:
  Actor_SetModeById(6, 2);
  actor = Actor_GetById(4);
  if (actor == 0)
    goto L_0200a9a0;
  Actor_ResetAndSetPosition(6, FIELD_AT(actor, s16, 10),
                            FIELD_AT(actor, s16, 18));
L_0200a9a0:
  Actor_SetModeById(5, 2);
  actor = Actor_GetById(4);
  if (actor == 0)
    goto L_0200a9c0;
  Actor_ResetAndSetPosition(5, FIELD_AT(actor, s16, 10),
                            FIELD_AT(actor, s16, 18));
L_0200a9c0:
  Actor_SetModeById(30, 2);
  actor = Actor_GetById(4);
  if (actor == 0)
    goto L_0200a9e0;
  Actor_ResetAndSetPosition(30, FIELD_AT(actor, s16, 10),
                            FIELD_AT(actor, s16, 18));
L_0200a9e0:
  Actor_SetMovementTypeById(7, v_r5);
  Actor_SetMovementTypeById(6, v_r5);
  Actor_SetMovementTypeById(5, v_r5);
  Actor_SetMovementTypeById(30, v_r5);
  Actor_RefreshSelectorById(7);
  Actor_RefreshSelectorById(6);
  Actor_RefreshSelectorById(5);
  Actor_RefreshSelectorById(30);
  FieldCamera_FollowActor(4, 1);
  Actor_WalkByAndWait(4, -16, 0);
  Actor_SetMovementTypeById(4, (s32)gData_02005ff8);
  Actor_RefreshSelectorById(4);
  Actor_GetById(4)->priorityFlags |= 64;
  Actor_GetById(29)->motionFlags = two;
  Actor_SetMovementTypeById(4, (s32)gData_02006088);
  Actor_RefreshSelectorById(4);
  Actor_WalkToAndWait(4, 0x137, 0x100);
  Actor_ArmCallbackAndRefresh(4, 0x4000);
  Event_Wait(10);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Func_02003a6c(2);
  t438 = FIELD_AT(0x3000000, s32, 108);
  FIELD_AT(t438, s32, 428) = 0x200;
  FIELD_AT(t438, s32, 436) = 24;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02001b04(0);
  Import_EventSetExitRequest(1);
  Func_0200581c();
  return;
}
