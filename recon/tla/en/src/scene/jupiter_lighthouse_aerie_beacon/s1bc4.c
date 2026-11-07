#include "types.h"
#include "call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "owner_state.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Camera_FollowActor();
s32 Camera_MoveWithinMapBounds();
s32 Event_AskYesNoWithActorAnimation();
s32 Event_FinishScreenTransition();
s32 Event_StartScreenTransition();
s32 Event_WaitTransitionFrames();
s32 FieldEvent_Wait();
s32 Func_02001ac4();
s32 Func_02003a6c();
s32 Func_02003c58();
s32 Func_02005814();
s32 Func_0200581c();
s32 Func_0200588c();
s32 Func_020058c4();
s32 Func_020058d4();
s32 Func_020058ec();
s32 Func_020058fc();
s32 Func_02005924();
s32 Func_02005934();
s32 Func_02005984();
s32 Func_0200598c();
s32 Func_020059f4();
s32 Motion_SetVarCbAndRefresh();
s32 Actor_ArmCallback();
s32 Actor_WaitForMoveById();
s32 Actor_CommitPositionAndActivate();
s32 Actor_SetMovementTypeById();
s32 Actor_ResetAndSetPosition();
s32 Actor_ResetAndSetPositionInMode2();
s32 Actor_SetPositionAndReset();
s32 Actor_SetSpeedParameters();
s32 Actor_StartRepeatedMotion();
struct Actor *Actor_GetById();
s32 Actor_WatchAndSetScript();
s32 Actor_TurnToFaceEachOther();
extern u8 MsgWasnTThe[];
extern u8 gData_02005ff0[];
extern u8 gData_02005ff8[];
extern u8 gData_02006088[];
extern u8 gData_020061e0[];

static __inline__ void W_Func_020058fc(s32 a0, s32 a1, s32 a2) {
  /* FAKEMATCH(wrapper): W_Func_020058fc forwarded inline; as direct calls to
   * Func_020058fc, 71 of 1518 instructions differ in tla-en. */
  Func_020058fc(a0, a1, a2);
}

#define ACTOR_SET_MODE_AND_WAIT(id, mode, frames)                              \
  do {                                                                         \
    Actor_SetModeById(id, mode);                                               \
    FieldEvent_Wait(frames);                                                   \
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
  Func_02005984(0);
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
  Call4(Func_0200588c, 4, 0x1f80000, 0x1300000, fast);
  Call4(Func_0200588c, 7, 0x2100000, 0x1200000, fast);
  slow = 0x2000;
  Call4(Func_0200588c, 27, 0x1e80000, 0x1280000, slow);
  Call4(Func_0200588c, 28, 0x1f80000, 0x1180000, slow);
  Call4(Func_0200588c, 5, 0x2100000, 0x1300000, fast);
  Call4(Func_0200588c, 6, 0x2280000, 0x1200000, mid);
  Call4(Func_0200588c, 29, 0x1e00000, 0x1680000, mid);
  Call4(Func_0200588c, 30, 0x2500000, 0x1880000, 0);
  Actor_SetModeById(5, 19);
  Actor_SetModeById(6, 19);
  Actor_SetModeById(4, 19);
  Actor_SetModeById(7, 13);
  actor = Actor_GetById(4);
  actor->scaleX = -actor->scaleX;
  actor = Actor_GetById(6);
  actor->scaleX = -actor->scaleX;
  Call4(Camera_MoveWithinMapBounds, 0x2080000, 0x680000, 0x1380000, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Func_020058c4((s32)MsgWasnTThe);
  Actor_SetModeById(27, 9);
  Actor_SetModeById(28, 8);
  Motion_SetVarCbAndRefresh(28, 2);
  FieldEvent_Wait(5);
  Call3(Func_020058d4, 28, 0, 0);
  Call2(Func_020058ec, 27, 0xe000);
  W_Func_020058fc(27, 0x108, 30);
  Call3(Func_020058d4, 27, 0, 0);
  Motion_SetVarCbAndRefresh(4, 2);
  FieldEvent_Wait(5);
  Func_020058ec(28, slow);
  Func_020058ec(27, slow);
  FieldEvent_Wait(10);
  W_Func_020058fc(27, 0x102, 0);
  W_Func_020058fc(28, 0x102, 30);
  Call2(Motion_SetVarCbAndRefresh, 28, 2);
  Call3(Func_020058d4, 28, 0, 0);
  ACTOR_SET_MODE_AND_WAIT(27, 4, 33);
  Func_020058d4(27, 0, 0);
  W_Func_020058fc(28, 0x107, 30);
  Call3(Func_020058d4, 28, 0, 0);
  Func_020058d4(29, 0, 0);
  Func_020059f4(78);
  Call3(Actor_ArmCallback, 27, 0x4000, 0);
  Call2(Func_020058ec, 28, 0x4000);
  FieldEvent_Wait(10);
  Camera_MoveWithinMapBounds(0x2080000, 0x680000, 0x1580000, 1);
  Func_02005924();
  FieldEvent_Wait(10);
  Func_020059f4(39);
  W_Func_020058fc(27, 0x107, 30);
  Call3(Func_020058d4, 27, 0, 0);
  ACTOR_SET_MODE_AND_WAIT(29, 4, 33);
  Func_020058d4(29, 0, 0);
  W_Func_020058fc(28, 0x107, 30);
  Call3(Func_020058d4, 28, 0, 0);
  Func_020058fc(29, 0x108, 30);
  Call3(Func_020058d4, 29, 0, 0);
  Actor_SetModeById(28, 1);
  Call3(Actor_ArmCallback, 28, 0x6000, 0);
  Call2(Func_020058ec, 27, 0xe000);
  FieldEvent_Wait(10);
  ACTOR_SET_MODE_AND_WAIT(29, 4, 33);
  Func_020058d4(29, 0, 0);
  Call2(Func_020058ec, 29, 0);
  Call3(Func_020058d4, 29, 0, 0);
  Call3(Func_020058d4, 30, 0, 0);
  Call2(Actor_StartRepeatedMotion, 28, 2);
  Call2(Motion_SetVarCbAndRefresh, 27, 2);
  Actor_ArmCallback(28, slow, 0);
  Func_020058ec(27, slow);
  FieldEvent_Wait(5);
  ACTOR_SET_MODE_AND_WAIT(29, 4, 33);
  Func_020058ec(29, mid);
  Call3(Func_020058d4, 29, 0, 0);
  W_Func_020058fc(28, 0x102, 30);
  Call3(Actor_ArmCallback, 28, 0x4000, 0);
  Call3(Func_020058d4, 28, 0, 0);
  ACTOR_SET_MODE_AND_WAIT(29, 3, 20);
  Func_020058d4(29, 0, 0);
  Call2(Func_020058ec, 28, 0x6000);
  FieldEvent_Wait(10);
  Call2(Func_020058ec, 27, 0xe000);
  ACTOR_SET_MODE_AND_WAIT(27, 3, 20);
  Func_020058d4(27, 0, 0);
  ACTOR_SET_MODE_AND_WAIT(29, 3, 20);
  FieldEvent_Wait(10);
  Call3(Func_020058d4, 29, 0, 0);
  Actor_SetModeById(27, 3);
  ACTOR_SET_MODE_AND_WAIT(28, 3, 20);
  FieldEvent_Wait(10);
  Call2(Camera_FollowActor, 27, 1);
  Actor_SetMovementTypeById(27, (s32)gData_02005ff8);
  FieldEvent_Wait(30);
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
  Call3(Actor_SetPositionAndReset, 29, 0x137, 0x100);
  Call3(Actor_ArmCallback, 29, 0x4000, 0);
  Actor_RefreshSelectorById(27);
  Call3(Actor_SetPositionAndReset, 27, 0x14c, 0x10a);
  Call3(Actor_ArmCallback, 27, 0x4000, 0);
  Actor_RefreshSelectorById(28);
  Call3(Actor_SetPositionAndReset, 28, 0x122, 0x10a);
  Call3(Actor_ArmCallback, 28, 0x4000, 0);
  Actor_GetById(29)->priorityFlags |= 32;
  Actor_GetById(27)->priorityFlags |= 32;
  Actor_GetById(28)->priorityFlags |= 32;
  Call4(Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  Func_020059f4(78);
  Call3(Func_020058d4, 30, 0, 0);
  Call4(Func_0200588c, 30, 0x2000000, 0x1480000, mid);
  Func_02003a6c(0);
  Func_0200598c();
  FieldEvent_Wait(10);
  Call4(Func_0200588c, 0, 0x2180000, 0x1700000, mid);
  Call4(Func_0200588c, 1, 0x2180000, 0x1800000, mid);
  Call4(Func_0200588c, 2, 0x2280000, 0x1700000, mid);
  Call4(Func_0200588c, 3, 0x2280000, 0x1600000, mid);
  Call3(Func_020058d4, 0, 0, 0);
  Call2(Camera_FollowActor, 0, 1);
  Call3(Actor_ResetAndSetPositionInMode2, 0, 0x218, 0x150);
  Call3(Actor_ResetAndSetPositionInMode2, 1, 0x218, 0x160);
  Call3(Actor_ResetAndSetPositionInMode2, 2, 0x228, 0x150);
  Call3(Actor_ResetAndSetPositionInMode2, 3, 0x228, 0x140);
  Actor_WaitForMoveById(0);
  Actor_WaitForMoveById(1);
  Actor_WaitForMoveById(2);
  Actor_WaitForMoveById(3);
  Func_020058ec(30, 0x3000);
  FieldEvent_Wait(10);
  ACTOR_SET_MODE_AND_WAIT(30, 4, 33);
  Func_020058d4(30, 0, 0);
  ACTOR_SET_MODE_AND_WAIT(2, 4, 33);
  Func_020058d4(2, 0, 0);
  Call2(Func_020058ec, 3, 0x4000);
  FieldEvent_Wait(10);
  Call3(Func_020058d4, 3, 0, 0);
  Func_020058ec(0, fast);
  Call4(Camera_MoveWithinMapBounds, 0x1400000, 0x600000, 0x1680000, 1);
  Func_02005924();
  FieldEvent_Wait(10);
  Call3(Func_020058d4, 0, 0, 0);
  Camera_MoveWithinMapBounds(0x2000000, 0x600000, 0x1600000, 1);
  Func_02005924();
  Func_02003c58(0);
  ACTOR_SET_MODE_AND_WAIT(1, 4, 33);
  Func_020058d4(1, 0, 0);
  Func_020058ec(2, fast);
  FieldEvent_Wait(10);
  Motion_SetVarCbAndRefresh(2, 2);
  FieldEvent_Wait(5);
  Call3(Func_020058d4, 2, 0, 0);
  Func_020058fc(3, 0x102, 30);
  Call3(Func_020058d4, 3, 0, 0);
  Motion_SetVarCbAndRefresh(4, 2);
  FieldEvent_Wait(5);
  Func_020058ec(30, mid);
  Motion_SetVarCbAndRefresh(30, 2);
  FieldEvent_Wait(5);
  Func_020058d4(30, 0, 0);
  FieldEvent_Wait(10);
  Actor_StartRepeatedMotion(5, 2);
  FieldEvent_Wait(5);
  Actor_StartRepeatedMotion(7, 2);
  FieldEvent_Wait(5);
  Motion_SetVarCbAndRefresh(6, 2);
  FieldEvent_Wait(30);
  Call2(Func_020058ec, 0, 0xe000);
  Call3(Func_020058d4, 0, 0, 0);
  ACTOR_SET_MODE_AND_WAIT(3, 3, 20);
  FieldEvent_Wait(10);
  Func_020058ec(1, fast);
  Call3(Func_020058d4, 1, 0, 0);
  Func_020058ec(30, slow);
  FieldEvent_Wait(10);
  Call3(Func_020058d4, 30, 0, 0);
  Func_020058ec(30, mid);
  Call3(Actor_CommitPositionAndActivate, 3, -0x30, 0);
  Actor_ArmCallback(3, mid, 0);
  Call2(Func_02001ac4, 3, 4);
  Actor_ArmCallback(4, slow, 0);
  actor = Actor_GetById(4);
  actor->scaleX = -actor->scaleX;
  Actor_CommitPositionAndActivate(3, 40, 0);
  Call3(Actor_CommitPositionAndActivate, 3, 8, -16);
  Actor_ArmCallback(3, mid, 0);
  Call2(Func_02001ac4, 3, 6);
  Call3(Actor_ArmCallback, 6, 0x4000, 0);
  actor = Actor_GetById(6);
  actor->scaleX = -actor->scaleX;
  Actor_ArmCallback(3, fast, 0);
  Call2(Func_02001ac4, 3, 7);
  Call3(Actor_ArmCallback, 7, 0x4000, 0);
  Call3(Actor_ArmCallback, 3, 0x8000, 0);
  Call2(Func_02001ac4, 3, 5);
  Call3(Actor_ArmCallback, 5, 0x4000, 0);
  Call2(Motion_SetVarCbAndRefresh, 0, 2);
  Func_020058ec(0, fast);
  Call3(Func_020058d4, 0, 0, 0);
  Call3(Actor_ArmCallback, 30, slow, 0);
  W_Func_020058fc(30, 0x109, 30);
  Call3(Func_020058d4, 30, 0, 0);
  Func_020058fc(1, 0x107, 30);
  Call3(Func_020058d4, 1, 0, 0);
  Func_020058fc(2, 0x109, 30);
  Call3(Actor_ResetAndSetPositionInMode2, 2, 0x228, 0x140);
  Call2(Func_020058ec, 2, 0x8000);
  Call3(Actor_ArmCallback, 1, mid, 0);
  ACTOR_SET_MODE_AND_WAIT(2, 4, 33);
  Motion_SetVarCbAndRefresh(2, 2);
  Call3(Func_020058d4, 2, 0, 0);
  Call2(Func_020058ec, 0, 0xe000);
  Motion_SetVarCbAndRefresh(0, 2);
  FieldEvent_Wait(5);
  Call3(Func_020058d4, 0, 0, 0);
  Call2(Func_020058ec, 2, 0x6000);
  Motion_SetVarCbAndRefresh(2, 2);
  FieldEvent_Wait(5);
  Call3(Func_020058d4, 2, 0, 0);
  Call3(Actor_TurnToFaceEachOther, 5, 6, 0);
  Actor_TurnToFaceEachOther(7, 4, 0);
  FieldEvent_Wait(10);
  Call2(Func_020058ec, 30, 0);
  ACTOR_SET_MODE_AND_WAIT(30, 3, 20);
  FieldEvent_Wait(10);
  Call2(Func_020058ec, 30, 0);
  Call3(Func_020058d4, 30, 0, 0);
  Call3(Actor_ArmCallback, 4, 0x4000, 0);
  Call3(Actor_ArmCallback, 5, 0x4000, 0);
  Call3(Actor_ArmCallback, 7, 0x4000, 0);
  Call3(Actor_ArmCallback, 6, 0x4000, 0);
  Call3(Actor_ArmCallback, 3, 0x6000, 0);
  Func_020058ec(0, 0x4000);
  FieldEvent_Wait(50);
  ACTOR_SET_MODE_AND_WAIT(1, 3, 20);
  FieldEvent_Wait(10);
  Func_020058ec(0, mid);
  FieldEvent_Wait(50);
  Actor_SetModeById(3, 3);
  Actor_SetModeById(7, 3);
  Actor_SetModeById(6, 3);
  ACTOR_SET_MODE_AND_WAIT(5, 3, 20);
  FieldEvent_Wait(10);
  Call2(Func_020058ec, 0, fast);
  Call2(Func_020058ec, 4, slow);
  t302 = Event_AskYesNoWithActorAnimation(4, 0);
  if (t302 != 0)
    goto L_0200a678;
  ACTOR_SET_MODE_AND_WAIT(0, 3, 20);
  FieldEvent_Wait(10);
  Func_020058d4(0, 0, 0);
  goto L_0200a6fe;
L_0200a678:
  Func_020058fc(0, 0x107, 30);
  ACTOR_SET_MODE_AND_WAIT(5, 4, 33);
  t310 = FIELD_AT(0x3000000, s32, 108);
  t311 = FIELD_AT(t310, u16, 452);
  FIELD_AT(t310, u16, 452) = (t311 + 2);
  Call2(Func_020058ec, 5, 0x8000);
  Call3(Func_020058d4, 5, 0, 0);
  Func_020058ec(5, 0x4000);
  FieldEvent_Wait(10);
  Motion_SetVarCbAndRefresh(5, 2);
  FieldEvent_Wait(5);
  Call3(Func_020058d4, 5, 0, 0);
  ACTOR_SET_MODE_AND_WAIT(0, 3, 20);
  FieldEvent_Wait(10);
  Func_020058d4(0, 0, 0);
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
  Call2(Camera_FollowActor, 0, 1);
  Call2(Actor_WatchAndSetScript, 4, 0);
  Call2(Actor_WatchAndSetScript, 7, 0);
  Call2(Actor_WatchAndSetScript, 5, 0);
  Call2(Actor_WatchAndSetScript, 6, 0);
  Actor_WatchAndSetScript(30, 0);
  FieldEvent_Wait(10);
  Call3(Actor_SetSpeedParameters, 0, 0x18000, 0xc000);
  Actor_SetPositionAndReset(0, 0x21f, 0x16b);
  Actor_WaitForMoveById(0);
  Call2(Func_020058ec, 0, 0xc000);
  Motion_SetVarCbAndRefresh(0, 2);
  FieldEvent_Wait(5);
  Call3(Func_020058d4, 0, 0, 0);
  Call3(Actor_SetPositionAndReset, 0, 0x23d, 0x166);
  Call3(Actor_SetPositionAndReset, 0, 0x248, 0x12c);
  Call3(Actor_SetPositionAndReset, 0, 0x248, 252);
  Call3(Actor_SetPositionAndReset, 0, 0x268, 247);
  Actor_SetPositionAndReset(0, 0x269, 0x138);
  Actor_GetById(0)->motionFlags = 2;
  Actor_SetMovementTypeById(0, (s32)gData_020061e0);
  Actor_RefreshSelectorById(0);
  Call2(Func_020058ec, 0, 0x4000);
  FieldEvent_Wait(30);
  Actor_GetById(0)->priorityFlags |= 32;
  Call3(Actor_ArmCallback, 4, 0, 0);
  Call3(Actor_ArmCallback, 7, 0, 0);
  Call3(Actor_ArmCallback, 5, 0, 0);
  Call3(Actor_ArmCallback, 6, 0, 0);
  Actor_ArmCallback(30, 0, 0);
  Func_02003a6c(1);
  FieldEvent_Wait(100);
  Call3(Actor_ArmCallback, 4, 0x8000, 0);
  Call3(Actor_ArmCallback, 7, 0x8000, 0);
  Call3(Actor_ArmCallback, 5, 0x8000, 0);
  Call3(Actor_ArmCallback, 6, 0x8000, 0);
  Actor_ArmCallback(30, 0x8000, 0);
  Func_02003c58(1);
  FieldEvent_Wait(60);
  Camera_FollowActor(4, 1);
  Func_02005924();
  FieldEvent_Wait(60);
  Func_020058ec(4, 0);
  FieldEvent_Wait(60);
  ACTOR_SET_MODE_AND_WAIT(5, 3, 20);
  FieldEvent_Wait(10);
  Call2(Func_020058ec, 4, 0xe000);
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
  Call2(Camera_FollowActor, 4, 1);
  Call3(Actor_CommitPositionAndActivate, 4, -16, 0);
  Actor_SetMovementTypeById(4, (s32)gData_02005ff8);
  Actor_RefreshSelectorById(4);
  Actor_GetById(4)->priorityFlags |= 64;
  Actor_GetById(29)->motionFlags = two;
  Actor_SetMovementTypeById(4, (s32)gData_02006088);
  Actor_RefreshSelectorById(4);
  Call3(Actor_SetPositionAndReset, 4, 0x137, 0x100);
  Func_020058ec(4, 0x4000);
  FieldEvent_Wait(10);
  Call4(Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  Func_02003a6c(2);
  t438 = FIELD_AT(0x3000000, s32, 108);
  FIELD_AT(t438, s32, 428) = 0x200;
  FIELD_AT(t438, s32, 436) = 24;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02001b04(0);
  Func_02005934(1);
  Func_0200581c();
  return;
}
