#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "text/msg_ids.h"
#include "scene_tables.h"
#include "event_inline.h"

void Func_02003e64();
void Func_02003e6c();
void Func_02003ff4();
void Func_02003dfc();
void Func_02003fbc();
void Func_02003fc4();
void Actor_SetSingleChildFlags();
void PartyInventory_Remove();

void Func_02000210(void) {
  s32 result0;
  s32 result1;

  Func_02003e64();
  Event_ResolvePendingActions(0);
  result0 = Event_GetFlag(0xaa2);
  if (result0 == 0)
    goto L_02008260;
  result1 = Event_GetFlag(0x200);
  if (result1 == 0)
    goto L_02008250;
  Actor_SetAngleToward(9, gMainRoutineWork.currentOwner, 0);
  Event_SetMessageId((s32)MsgPenPenPen);
  goto L_02008256;
L_02008250:
  Event_SetMessageId((s32)MsgPenPen2);
L_02008256:
  Event_ShowMessageAndWait(9, 0);
  goto L_02008294;
L_02008260:
  Actor_Stop(9);
  wait(1);
  Actor_SetAngleToward(9, gMainRoutineWork.currentOwner, 0);
  Event_SetMessageId((s32)MsgPenPen3);
  Event_ShowMessageAndWait(9, 0);
  Actor_WatchAndSetScript(9, 19);
L_02008294:
  Func_02003e6c();
  return;
}

void Func_020002ac(void) {
  s32 result0;
  s32 result1;

  Func_02003e64();
  Event_ResolvePendingActions(0);
  result0 = Event_GetFlag(0xaa2);
  if (result0 == 0)
    goto L_020082ea;
  result1 = Event_GetFlag(0x200);
  if (result1 == 0)
    goto L_020082da;
  Event_SetMessageId((s32)MsgThereIsNothing);
  goto L_020082e0;
L_020082da:
  Event_SetMessageId((s32)MsgIMSoHappy);
L_020082e0:
  Event_ShowMessageAndWait(9, 0);
  goto L_020082f8;
L_020082ea:
  Event_SetMessageId((s32)MsgMyDarlingPengulina);
  Event_ShowMessageAndWait(9, 0);
L_020082f8:
  Func_02003e6c();
  return;
}

void Func_0200030c(void) {
  s32 result0;
  s32 result1;

  Func_02003e64();
  Event_ResolvePendingActions(0);
  result0 = Event_GetFlag(0xaa2);
  if (result0 == 0)
    goto L_0200835c;
  result1 = Event_GetFlag(0x200);
  if (result1 == 0)
    goto L_0200834c;
  Actor_SetAngleToward(19, gMainRoutineWork.currentOwner, 0);
  Event_SetMessageId((s32)MsgPenPenPehnnnn);
  goto L_02008352;
L_0200834c:
  Event_SetMessageId((s32)MsgPenPen2);
L_02008352:
  Event_ShowMessageAndWait(19, 0);
  goto L_0200836a;
L_0200835c:
  Event_SetMessageId((s32)MsgPenPenPehnnn);
  Event_ShowMessageAndWait(19, 0);
L_0200836a:
  Func_02003e6c();
  return;
}

void Func_02000380(void) {
  s32 result0;
  s32 result1;

  Func_02003e64();
  Event_ResolvePendingActions(0);
  result0 = Event_GetFlag(0xaa2);
  if (result0 == 0)
    goto L_020083be;
  result1 = Event_GetFlag(0x200);
  if (result1 == 0)
    goto L_020083ae;
  Event_SetMessageId((s32)MsgPengusIsPretty);
  goto L_020083b4;
L_020083ae:
  Event_SetMessageId((s32)MsgIMSoGlad);
L_020083b4:
  Event_ShowMessageAndWait(19, 0);
  goto L_020083cc;
L_020083be:
  Event_SetMessageId((s32)MsgOhhMyLeg);
  Event_ShowMessageAndWait(19, 0);
L_020083cc:
  return;
}

s32 Func_020003dc(void) {
  u16 turn;
  s32 facing;

  turn = Actor_GetById(gMainRoutineWork.currentOwner)->angle + 0x5fff;
  facing = 1;
  if (turn >= 0x3fff)
    facing = 0;
  return facing;
}

void Func_0200040c(void) {
  s32 result0;

  result0 = Func_020003dc();
  if (result0 == 0)
    goto L_0200841e;
  Func_02003ff4(21);
  goto L_0200843a;
L_0200841e:
  Func_02003e64();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgIAmHere);
  Event_ShowMessageAndWait(21, 0);
  Func_02003e6c();
L_0200843a:
  return;
}

void Func_02000440(void) {
  s32 result0;

  Func_02003e64();
  Event_ResolvePendingActions(0);
  result0 = Event_GetFlag(0x214);
  if (result0 != 0)
    goto L_020084b6;
  Event_SetMessageId((s32)MsgWeLikeThis);
  Event_ShowMessageAndWait(8, 0);
  Actor_SetAttachedEffect(9, 0x102);
  Actor_FaceDirection(9, FACING_WEST, 0);
  Event_ShowMessageAndWait(9, 0);
  Actor_ArmCallbackAndRefresh(8, FACING_EAST);
  Actor_Jump(9, 4, 10);
  Event_ShowMessageAndWait(9, 0);
  Actor_SetAttachedEffect(9, 0x102);
  Event_Wait(20);
  GameFlag_SetBit(0x214);
L_020084b6:
  Actor_SetAngleToward(8, gMainRoutineWork.currentOwner, 0);
  Event_SetMessageId((s32)MsgLookWeRe);
  Event_ShowMessageAndWait(8, 0);
  Actor_FaceDirection(9, FACING_SOUTH + FACING_STEP, 0);
  Func_02003e6c();
  return;
}

void Func_020004f4(void) {
  Func_02003e64();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgTheEasternSea);
  Event_ShowMessageAndWait(9, 0);
  Event_ShowMessageAndWait(9, 0);
  Func_02003e6c();
  return;
}

void Func_02000520(void) {
  s32 result0;

  Func_02003e64();
  Event_ResolvePendingActions(0);
  result0 = Event_GetFlag(0xaa4);
  if (result0 == 0)
    goto L_0200854a;
  Actor_WatchAndSetScript(10, gMainRoutineWork.currentOwner);
L_0200854a:
  Event_SetMessageId((s32)MsgChirpChirp2);
  Event_ShowMessageAndWait(10, 0);
  Func_02003e6c();
  return;
}

/// Object 10's talk, and its exchange of the party's item 0x1c4 for item 0x1c5.
void Func_02000568(void) {
  struct Actor *leader;
  struct Actor *object;
  struct Actor *spawned;

  Func_02003e64();
  Event_ResolvePendingActions(0);
  if (FieldPartyInventory_FindOwner(0x1c4) == -1) {
    if (Event_GetFlag(0xaa4)) {
      Actor_WatchAndSetScript(10, gMainRoutineWork.currentOwner);
      Event_SetMessageId((s32)MsgWhatDoYou);
    } else {
      Event_SetMessageId((s32)MsgILoveFeeling);
    }
    Event_ShowMessageAndWait(10, 0);
  } else {
    leader = Actor_GetById(gMainRoutineWork.currentOwner);
    object = Actor_GetById(10);
    Actor_SetAngleToward(10, gMainRoutineWork.currentOwner, 0);
    Actor_Jump(10, 4, 10);
    Event_SetMessageId((s32)MsgOhMyThat);
    Event_ShowMessageAndWait(10, 0);
    Actor_SetModeById(10, 4);
    FieldActor_SetPositionAndResetMotion(object, (leader->x + object->x) / 2,
                                         leader->y,
                                         (leader->z + object->z) / 2);
    wait(20);
    Actor_SetPositionAndResetMotion(object, PIXELS(120), PIXELS(128),
                                    PIXELS(312));
    Actor_SetModeById(10, 5);
    wait(10);
    Actor_ArmCallbackAndRefresh(gMainRoutineWork.currentOwner, FACING_SOUTH);
    spawned = FieldActor_Create(0x1e9, leader->x, 0x980000, leader->z);
    if (spawned != 0) {
      Actor_SetModeById(gMainRoutineWork.currentOwner, 28);
      Actor_SetItemIcon(spawned, 0x1c5);
      Actor_SetSingleChildFlags(spawned, 0);
      spawned->motionFlags = 0;
    }
    Event_Wait(10);
    Func_02003fc4();
    FieldMessage_Show((s32)MsgOhMyThat + 1, 1);
    PartyInventory_Remove(0x1c4);
    Party_GiveItem(0x1c5, 0);
    Func_02003fbc();
    if (spawned != 0) {
      Func_02003dfc(spawned);
      Actor_SetModeById(gMainRoutineWork.currentOwner, 1);
    }
    Actor_ArmCallbackAndRefresh(10, FACING_EAST);
    GameFlag_SetBit(0xaa4);
  }
  Func_02003e6c();
}
