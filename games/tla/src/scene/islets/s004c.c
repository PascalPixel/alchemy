/// @file
/// Scene code of overlay 0x674, in ROM order, from s004c's offset up to the next object the overlay links.

#include "text/msg_ids.h"
#include "islets.h"

#include "islets.h"
#include "scene_tables.h"
#include "event_inline.h"

u8 *Func_0200004c(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov674Room1)
    return gData_0200476c;
  if (scene == (s32)&SceneId_Ov674Room2)
    return gData_020047e4;
  if (scene == (s32)&SceneId_Ov674Room3)
    return gData_0200494c;
  if (scene == (s32)&SceneId_Ov674Room4)
    return gData_020049c4;
  if (scene == (s32)&SceneId_Ov674Room5) {
    if (Event_GetFlag(0xaa5))
      gData_02004a84[46] = 1;
    return gData_02004a84;
  }
  return gData_02004754;
}

void Func_020000d8(void) {
  struct IsletsTarget target;
  struct Actor *object = Actor_Get(10);

  if (Func_020037d8(&target)) {
    Func_02003a5c(target);
    System_Wait(5);
    object->gravity = 0x8000;
    Actor_SetAnimation(10, 3);
    Actor_SetDestinationOffset(10, 0, 8);
    FieldEvent_Wait(22);
    Event_PlaySound(240);
    Actor_SetAnimation(10, 8);
    object->priorityFlags |= 2;
    object->motionFlags = 0;
    object->velocityY = 0;
    object->y = PIXELS(-8);
    object->terrainHeight = PIXELS(-8);
    MapGrid_CopyCellAttributes(9, 1, 4, 1, 9, 10);
    MapGrid_CopyCellAttributes(9, 8, 4, 1, 9, 9);
    MapGrid_CopyCellAttributes(9, 64, 4, 1, 9, 74);
    Event_SetFlag(0xa2c);
  }
}

const u8 *Func_0200019c(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov674Room1)
    return gData_02004b20;
  if (scene == (s32)&SceneId_Ov674Room2)
    return gData_02004bb0;
  if (scene == (s32)&SceneId_Ov674Room3)
    return gData_02004ca0;
  if (scene == (s32)&SceneId_Ov674Room4)
    return gData_02004d84;
  if (scene == (s32)&SceneId_Ov674Room5)
    return gData_02004e5c;
  return gData_02004b14;
}

#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "text/msg_ids.h"

void Func_02003e64();
void Func_02003e6c();
void Func_02003ff4();
void Func_02003dfc();
void Func_02003fbc();
void Func_02003fc4();
void PartyInventory_Remove();

void Func_02000210(void) {
  s32 result0;
  s32 result1;

  Func_02003e64();
  FieldEvent_ResolvePendingActions(0);
  result0 = Event_GetFlag(0xaa2);
  if (result0 == 0)
    goto L_02008260;
  result1 = Event_GetFlag(0x200);
  if (result1 == 0)
    goto L_02008250;
  Actor_FaceActor(9, gMainRoutineWork.currentOwner, 0);
  FieldEvent_SetMessageId((s32)MsgPenPenPen);
  goto L_02008256;
L_02008250:
  FieldEvent_SetMessageId((s32)MsgPenPen2);
L_02008256:
  Event_ShowMessage(9, 0);
  goto L_02008294;
L_02008260:
  FieldActor_Stop(9);
  System_Wait(1);
  Actor_FaceActor(9, gMainRoutineWork.currentOwner, 0);
  FieldEvent_SetMessageId((s32)MsgPenPen3);
  Event_ShowMessage(9, 0);
  FieldActor_WatchAndSetScript(9, 19);
L_02008294:
  Func_02003e6c();
  return;
}

void Func_020002ac(void) {
  s32 result0;
  s32 result1;

  Func_02003e64();
  FieldEvent_ResolvePendingActions(0);
  result0 = Event_GetFlag(0xaa2);
  if (result0 == 0)
    goto L_020082ea;
  result1 = Event_GetFlag(0x200);
  if (result1 == 0)
    goto L_020082da;
  FieldEvent_SetMessageId((s32)MsgThereIsNothing);
  goto L_020082e0;
L_020082da:
  FieldEvent_SetMessageId((s32)MsgIMSoHappy);
L_020082e0:
  Event_ShowMessage(9, 0);
  goto L_020082f8;
L_020082ea:
  FieldEvent_SetMessageId((s32)MsgMyDarlingPengulina);
  Event_ShowMessage(9, 0);
L_020082f8:
  Func_02003e6c();
  return;
}

void Func_0200030c(void) {
  s32 result0;
  s32 result1;

  Func_02003e64();
  FieldEvent_ResolvePendingActions(0);
  result0 = Event_GetFlag(0xaa2);
  if (result0 == 0)
    goto L_0200835c;
  result1 = Event_GetFlag(0x200);
  if (result1 == 0)
    goto L_0200834c;
  Actor_FaceActor(19, gMainRoutineWork.currentOwner, 0);
  FieldEvent_SetMessageId((s32)MsgPenPenPehnnnn);
  goto L_02008352;
L_0200834c:
  FieldEvent_SetMessageId((s32)MsgPenPen2);
L_02008352:
  Event_ShowMessage(19, 0);
  goto L_0200836a;
L_0200835c:
  FieldEvent_SetMessageId((s32)MsgPenPenPehnnn);
  Event_ShowMessage(19, 0);
L_0200836a:
  Func_02003e6c();
  return;
}

void Func_02000380(void) {
  s32 result0;
  s32 result1;

  Func_02003e64();
  FieldEvent_ResolvePendingActions(0);
  result0 = Event_GetFlag(0xaa2);
  if (result0 == 0)
    goto L_020083be;
  result1 = Event_GetFlag(0x200);
  if (result1 == 0)
    goto L_020083ae;
  FieldEvent_SetMessageId((s32)MsgPengusIsPretty);
  goto L_020083b4;
L_020083ae:
  FieldEvent_SetMessageId((s32)MsgIMSoGlad);
L_020083b4:
  Event_ShowMessage(19, 0);
  goto L_020083cc;
L_020083be:
  FieldEvent_SetMessageId((s32)MsgOhhMyLeg);
  Event_ShowMessage(19, 0);
L_020083cc:
  return;
}

s32 Func_020003dc(void) {
  u16 turn;
  s32 facing;

  turn = Actor_Get(gMainRoutineWork.currentOwner)->angle + 0x5fff;
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
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgIAmHere);
  Event_ShowMessage(21, 0);
  Func_02003e6c();
L_0200843a:
  return;
}

void Func_02000440(void) {
  s32 result0;

  Func_02003e64();
  FieldEvent_ResolvePendingActions(0);
  result0 = Event_GetFlag(0x214);
  if (result0 != 0)
    goto L_020084b6;
  FieldEvent_SetMessageId((s32)MsgWeLikeThis);
  Event_ShowMessage(8, 0);
  Actor_SetAttachedEffect(9, 0x102);
  Actor_FaceDirection(9, FACING_WEST, 0);
  Event_ShowMessage(9, 0);
  Actor_FaceDirectionAndWait(8, FACING_EAST);
  FieldActor_Jump(9, 4, 10);
  Event_ShowMessage(9, 0);
  Actor_SetAttachedEffect(9, 0x102);
  FieldEvent_Wait(20);
  Event_SetFlag(0x214);
L_020084b6:
  Actor_FaceActor(8, gMainRoutineWork.currentOwner, 0);
  FieldEvent_SetMessageId((s32)MsgLookWeRe);
  Event_ShowMessage(8, 0);
  Actor_FaceDirection(9, FACING_SOUTH + FACING_STEP, 0);
  Func_02003e6c();
  return;
}

void Func_020004f4(void) {
  Func_02003e64();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgTheEasternSea);
  Event_ShowMessage(9, 0);
  Event_ShowMessage(9, 0);
  Func_02003e6c();
  return;
}

void Func_02000520(void) {
  s32 result0;

  Func_02003e64();
  FieldEvent_ResolvePendingActions(0);
  result0 = Event_GetFlag(0xaa4);
  if (result0 == 0)
    goto L_0200854a;
  FieldActor_WatchAndSetScript(10, gMainRoutineWork.currentOwner);
L_0200854a:
  FieldEvent_SetMessageId((s32)MsgChirpChirp2);
  Event_ShowMessage(10, 0);
  Func_02003e6c();
  return;
}

/// Object 10's talk, and its exchange of the party's item 0x1c4 for item 0x1c5.
void Func_02000568(void) {
  struct Actor *leader;
  struct Actor *object;
  struct Actor *spawned;

  Func_02003e64();
  FieldEvent_ResolvePendingActions(0);
  if (FieldPartyInventory_FindOwner(0x1c4) == -1) {
    if (Event_GetFlag(0xaa4)) {
      FieldActor_WatchAndSetScript(10, gMainRoutineWork.currentOwner);
      FieldEvent_SetMessageId((s32)MsgWhatDoYou);
    } else {
      FieldEvent_SetMessageId((s32)MsgILoveFeeling);
    }
    Event_ShowMessage(10, 0);
  } else {
    leader = Actor_Get(gMainRoutineWork.currentOwner);
    object = Actor_Get(10);
    Actor_FaceActor(10, gMainRoutineWork.currentOwner, 0);
    FieldActor_Jump(10, 4, 10);
    FieldEvent_SetMessageId((s32)MsgOhMyThat);
    Event_ShowMessage(10, 0);
    Actor_SetAnimation(10, 4);
    FieldActor_SetPositionAndResetMotion(object, (leader->x + object->x) / 2,
                                         leader->y,
                                         (leader->z + object->z) / 2);
    System_Wait(20);
    FieldActor_SetPositionAndResetMotion(object, PIXELS(120), PIXELS(128),
                                    PIXELS(312));
    Actor_SetAnimation(10, 5);
    System_Wait(10);
    Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, FACING_SOUTH);
    spawned = FieldActor_Create(0x1e9, leader->x, 0x980000, leader->z);
    if (spawned != 0) {
      Actor_SetAnimation(gMainRoutineWork.currentOwner, 28);
      Actor_SetItemIcon(spawned, 0x1c5);
      Actor_SetSingleChildFlags(spawned, 0);
      spawned->motionFlags = 0;
    }
    FieldEvent_Wait(10);
    Func_02003fc4();
    Message_ShowCentered((s32)MsgOhMyThat + 1, 1);
    PartyInventory_Remove(0x1c4);
    Party_GiveItem(0x1c5, 0);
    Func_02003fbc();
    if (spawned != 0) {
      Func_02003dfc(spawned);
      Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
    }
    Actor_FaceDirectionAndWait(10, FACING_EAST);
    Event_SetFlag(0xaa4);
  }
  Func_02003e6c();
}

// The Islets' trade of item 453 with object 10. With the item in the party,
// if flag 513 is set (asked before) the camera moves and object 10 asks again
// (MsgAreYouGiving); otherwise it notices the item (MsgOhThatRed), the leader
// steps up and shows it (object type 489, its picture set to the item by
// Func_0802372c) and object 10 asks. A yes hands the item across step by
// step, takes item 453 and gives item 454 (object 10 hands it back over the
// leader's head), sets flag 0xaa3 and counts the answer in the event work's
// halfword 0x1c4 the first time; a no sets flag 513. Without the item, object
// 10 says MsgYupANice after the trade or MsgIVeBeenFeeling before it.

// zero is set only inside the first spawn check: set where CSE can see it, the
// store reuses the second answer, which then stays in r7. HAND_BACK is a plain
// block: a do-while(0) ends CSE's block and loses the owner address and
// message bases in the refusals.

void Func_02003e64(void);
void Func_02003e6c(void);
void Func_02003f74(void);
void Func_02003fc4(void);
void Func_02003fbc(void);
void PartyInventory_Remove(s32 item);
void Func_02003dfc(struct Actor *object);

#define STEP(actor, x, z)                                                      \
  (Actor_GetById(actor)->actionFlags &= ~1, Actor_WalkToAndWait(actor, x, z),  \
   Event_Wait(1), Actor_GetById(actor)->actionFlags |= 1)

/// Object 10 walks round and hands item 454 back over the leader's head.
#define HAND_BACK(leader, given, off)                                          \
  {                                                                            \
    Actor_SetSpeed(10, 0xcccc, 0x6666);                                        \
    Actor_WalkToAndWait(10, 0x80, 164);                                        \
    Actor_ArmCallbackAndRefresh(10, 0x8000);                                   \
    Event_Wait(20);                                                            \
    Actor_SetSpeed(10, 0x19999, 0xcccc);                                       \
    STEP(10, 108, 168);                                                        \
    given =                                                                    \
        FieldActor_Create(489, leader->x, leader->y + 0x180000, leader->z);    \
    if (given != 0) {                                                          \
      Actor_SetModeById(gMainRoutineWork.currentOwner, 28);                    \
      given->motionFlags = off;                                                \
      Func_0802372c(given, 454);                                               \
      Event_Wait(20);                                                          \
    }                                                                          \
    STEP(10, 116, 164);                                                        \
    Func_02003fc4();                                                           \
    PartyInventory_Remove(453);                                                \
    PartyInventory_GiveItem(454, 0);                                           \
    if (given != 0) {                                                          \
      Func_02003dfc(given);                                                    \
      Actor_SetModeById(gMainRoutineWork.currentOwner, 1);                     \
    }                                                                          \
    Func_02003fbc();                                                           \
    GameFlag_SetBit(0xaa3);                                                    \
  }

void Func_020006f4(void) {
  struct Actor *item, *leader, *given;
  const u8 *asked, *noticed;
  /* FAKEMATCH(temporary): zero wrapped in a struct of one u16, so it stays a
   * HImode pseudo and its 0 is loaded from the pool; as s32 zero, 25 of 687
   * instructions differ in tla-en. */
  struct {
    u16 v;
  } zero;

  Func_02003e64();
  Event_ResolvePendingActions(0);
  if (FieldPartyInventory_FindOwner(453) != -1) {
    if (Event_GetFlag(513)) {
      FieldCamera_MoveTo(-1, -1, -1, 0);
      FieldCamera_SetSpeed(0xcccc, 0x1999);
      FieldCamera_MoveTo(0x8c0000, -1, 0xbe0000, 1);
      Actor_SetAngleToward(10, gMainRoutineWork.currentOwner, 0);
      asked = MsgAreYouGiving;
      Event_SetMessageId(asked);
      Event_ShowMessageAndWait(10, 0);
      Func_02003f74();
      Func_02003fc4();
      if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) ==
          0) {
        leader = Actor_GetById(gMainRoutineWork.currentOwner);
        Actor_WatchAndSetScript(10, gMainRoutineWork.currentOwner);
        Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
        Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 98, 168);
        Actor_ArmCallbackAndRefresh(gMainRoutineWork.currentOwner, 0xc000);
        Actor_Jump(gMainRoutineWork.currentOwner, 4, 10);
        item = FieldActor_Create(489, 0x680000, 0x180000, 0xa00000);
        // Default: the object handed back is set for real in HAND_BACK.
        given = 0;
        if (item != 0) {
          Func_0802372c(item, 453);
          Actor_SetSingleChildFlags(item, 0);
          item->motionFlags = 0;
          item->unk5c[0] = 1;
        }
        Event_Wait(10);
        Func_02003fbc();
        Actor_Stop(10);
        Actor_ArmCallback(gMainRoutineWork.currentOwner, 0, 0);
        Func_080d47b4(10, 264, 20);
        Event_SetMessageId(asked + 1);
        Event_ShowMessageAndWait(0x800a, 0);
        HAND_BACK(leader, given, 0)
      } else {
        Func_02003fbc();
        Func_080d47b4(10, 265, 20);
        Event_SetMessageId(asked + 4);
        Event_ShowMessageAndWait(0x800a, 0);
      }
    } else {
      noticed = MsgOhThatRed;
      Event_SetMessageId(noticed);
      Event_ShowMessageAndWait(10, 0);
      FieldCamera_MoveTo(-1, -1, -1, 0);
      FieldCamera_SetSpeed(0xcccc, 0x1999);
      FieldCamera_MoveTo(0x8c0000, -1, 0xbe0000, 1);
      Actor_FaceDirection(10, 0x5000, 0);
      Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
      Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 120, 190);
      Actor_ArmCallbackAndRefresh(gMainRoutineWork.currentOwner, 0);
      STEP(gMainRoutineWork.currentOwner, 132, 190);
      item = FieldActor_Create(489, 0x8c0000, 0, 0xbe0000);
      if (item != 0) {
        Func_0802372c(item, 453);
        Actor_SetSingleChildFlags(item, 0);
        item->motionFlags = 0;
        zero.v = 0;
        item->unk5c[0] = 1;
      }
      STEP(gMainRoutineWork.currentOwner, 120, 190);
      Actor_ArmCallback(10, 0x3000, 0);
      Actor_Jump(10, 4, 10);
      Event_ShowMessageAndWait(10, 0);
      Actor_WatchAndSetScript(10, gMainRoutineWork.currentOwner);
      STEP(gMainRoutineWork.currentOwner, 132, 190);
      FieldActor_SetPositionAndResetMotion(item, 0xc30000, 0, 0x730000);
      STEP(gMainRoutineWork.currentOwner, 120, 190);
      Event_Wait(20);
      Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 98, 168);
      Actor_Jump(gMainRoutineWork.currentOwner, 4, 10);
      Actor_SetPositionAndResetMotion(item, 0x680000, 0x180000, 0xa00000);
      Event_Wait(10);
      Func_02003fc4();
      Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, 0x4000);
      FieldMessage_Show(noticed + 2, 1);
      (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
      Func_02003fbc();
      Actor_Jump(10, 6, 10);
      Event_SetMessageId(noticed + 3);
      Event_ShowMessageAndWait(10, 0);
      Actor_Stop(10);
      Actor_ArmCallbackAndRefresh(gMainRoutineWork.currentOwner, 0);
      Func_02003fc4();
      if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner,
                                                0) == 0) {
        item = Actor_GetById(gMainRoutineWork.currentOwner);
        Func_02003fbc();
        Func_080d47b4(10, 264, 20);
        Event_SetMessageId(noticed + 4);
        Event_ShowMessageAndWait(0x800a, 0);
        HAND_BACK(item, item, zero.v)
      } else {
        Func_02003fbc();
        Func_080d47b4(10, 265, 20);
        Event_SetMessageId(noticed + 7);
        Event_ShowMessageAndWait(0x800a, 0);
        Actor_ArmCallbackAndRefresh(gMainRoutineWork.currentOwner, 0xc000);
        Actor_Jump(gMainRoutineWork.currentOwner, 4, 10);
        if (item != 0)
          Func_02003dfc(item);
        Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, 0x4000);
        Event_SetFlag(513);
      }
    }
  } else {
    if (Event_GetFlag(0xaa3))
      Event_SetMessageId(MsgYupANice);
    else
      Event_SetMessageId(MsgIVeBeenFeeling);
    Event_ShowMessageAndWait(10, 0);
  }
  Actor_SetModeById(10, 4);
  Func_02003e6c();
}
#undef STEP

struct Actor *Func_02000da0(struct Actor *object, s32 value) {
  struct Actor *spawned;

  spawned =
      FieldActor_Create(489, object->x, object->y + 0x180000, object->z);
  if (spawned != 0) {
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 28);
    spawned->motionFlags = 0;
    Actor_SetItemIcon(spawned, value);
    Actor_SetSingleChildFlags(spawned, 0);
    spawned->unk5c[0] = 1;
  }
  return spawned;
}

void Func_02000df8(struct Actor *object) {
  struct Actor *spawned;

  if ((gFrameCount & 3) == 0) {
    spawned = FieldActor_Create(14, object->x, object->y, object->z);
    if (spawned != 0) {
      Actor_SetSingleChildFlags(spawned, 0);
      Func_02003de4(spawned, 1);
      Func_02003dec(spawned, gData_020044f0);
    }
  }
}

#include "call.h"

extern const u8 gData_020043f8[], gData_02004300[];
struct Actor *Func_02000da0(struct Actor *holder, s32 item);

#define STEP(actor, x, z)                                                      \
  (Actor_GetById(actor)->actionFlags &= ~1, Actor_WalkToAndWait(actor, x, z),  \
   FieldEvent_Wait(1), Actor_GetById(actor)->actionFlags |= 1)

void Func_02000e38(void) {
  struct Actor *leader, *trader, *item, *gift;
  s32 right;
  s32 giftItem;

  Func_02003e64();
  FieldEvent_ResolvePendingActions(0);
  if (FieldPartyInventory_FindOwner(454) != -1) {
    leader = Actor_GetById(gMainRoutineWork.currentOwner);
    trader = Actor_GetById(11);
    // Default: the trader's zero update stores read it.
    gift = 0;
    right = 0;
    Actor_FaceActor(11, gMainRoutineWork.currentOwner, 0);
    trader->update = Func_02000df8;
    FieldActor_Jump(11, 4, 10);
    trader->update = 0;
    Event_SetMessageId(MsgSniffSniffI);
    Event_ShowMessage(11, 0);
    if (leader->x > trader->x)
      right = 1;
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
    Actor_SetSpeed(11, 0x19999, 0xcccc);
    if (right) {
      Actor_FaceDirection(11, 0, 0);
      Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 296, 856);
      Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, 0x8000);
      item = (struct Actor *)Value2((s32 (*)())Func_02000da0, (s32)leader, 454);
      trader->update = Func_02000df8;
      Actor_RunScriptAndWait(11, gData_020043f8);
      trader->update = 0;
    } else {
      Actor_FaceDirection(11, 0x8000, 0);
      Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 264, 856);
      Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, 0);
      item = (struct Actor *)Value2((s32 (*)())Func_02000da0, (s32)leader, 454);
      trader->update = Func_02000df8;
      Actor_RunScriptAndWait(11, gData_02004300);
      trader->update = 0;
    }
    Event_ShowMessage(11, 0);
    FieldActor_WatchAndSetScript(11, gMainRoutineWork.currentOwner);
    if (item != 0) {
      void (*hook2)(void);
      item->collisionFlags = 9;
      FieldActor_SetPositionAndResetMotion(item, 0x680000, 0, 0x3480000);
      hook2 = Func_02000df8;
      if (right) {
        leader->update = hook2;
        Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 296, 840);
        leader->update = 0;
        Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, 0x8000);
        leader->update = hook2;
        STEP(gMainRoutineWork.currentOwner, 0x120, 840);
        FieldActor_SetPositionAndResetMotion(item, 0x1180000, 0, 0x3480000);
        STEP(gMainRoutineWork.currentOwner, 296, 840);
        leader->update = 0;
        FieldActor_Stop(gMainRoutineWork.currentOwner);
        System_Wait(1);
        trader->update = hook2;
        STEP(11, 292, 844);
      } else {
        leader->update = hook2;
        Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 264, 840);
        leader->update = 0;
        Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, 0);
        leader->update = hook2;
        STEP(gMainRoutineWork.currentOwner, 0x110, 840);
        FieldActor_SetPositionAndResetMotion(item, 0x1180000, 0, 0x3480000);
        STEP(gMainRoutineWork.currentOwner, 264, 840);
        leader->update = 0;
        FieldActor_Stop(gMainRoutineWork.currentOwner);
        System_Wait(1);
        trader->update = hook2;
        STEP(11, 268, 844);
      }
    }
    giftItem = 455;
    gift =
        (struct Actor *)Value2((s32 (*)())Func_02000da0, (s32)leader, giftItem);
    STEP(11, 280, 856);
    trader->update = 0;
    Func_02003fc4();
    Message_ShowCentered((s32)MsgTradedHisMilk, 1);
    PartyInventory_Remove(454);
    Party_GiveItem(giftItem, 0);
    Func_02003fbc();
    Actor_GetById(gMainRoutineWork.currentOwner)->angle = 0x4000;
    if (gift != 0) {
      Func_02003dfc(gift);
      Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
    }
    Actor_FaceDirectionAndWait(11, 0xc000);
    Event_SetFlag(0xaa1);
    Event_SetFlag(534);
  } else {
    if (Event_GetFlag(0xaa1)) {
      FieldActor_WatchAndSetScript(11, gMainRoutineWork.currentOwner);
      if (Event_GetFlag(534))
        Event_SetMessageId(MsgIDonTLike);
      else
        Event_SetMessageId(MsgIDonTWantTo);
    } else {
      Event_SetMessageId(MsgTheWaterHere);
    }
    Event_ShowMessage(11, 0);
  }
  Func_02003e6c();
}

void Func_02001278(struct Actor *object) {
  struct Actor *source = Actor_Get(9);

  FieldActor_SetPositionAndResetMotion(object, source->x, source->y + PIXELS(16),
                                  source->z);
  object->angle = source->angle;
}

#include "version.h"

#if !TLA_PATCH_1
#define ISLETS_GIFT_TRANSITION 521
#else
#define ISLETS_GIFT_TRANSITION 513
#endif

struct ActionScript {
  u32 words[9];
};

extern struct ActionScript gData_02004004;
extern u8 MsgAtLongLast[];
void Func_02003de4(struct Actor *object, s32 mode);
u8 *HeapSlot_AllocEwram(s32 block, s32 size);
void Func_020034ec(void);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);

void Func_0200129c(struct Actor *gift) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct Actor *guide = Actor_Get(9);
  struct ActionScript script = gData_02004004;

  FieldCamera_MoveTo(-1, -1, -1, 0);
  Func_02003fbc();
  Actor_ShowEmote(9, 264, 20);
  guide->update = Func_02000df8;
  Actor_RunScriptAndWait(9, &script);
  guide->update = 0;
  FieldEvent_SetMessageId(MsgAtLongLast);
  Event_ShowMessage(9, 0);
  Event_ShowMessage(9, 0);
  Actor_FaceDirectionAndWait(9, 0x5000);
  Actor_SetSpeed(9, 0x19999, 0xcccc);
  guide->update = Func_02000df8;
  Actor_WalkToAndWait(9, 296, 452);
  guide->update = 0;
  Actor_FaceDirection(9, 0x5000, 0);
  gift->speedLimit = 0x19999;
  gift->acceleration = 0xcccc;
  gift->update = Func_02000df8;
  Func_02003de4(gift, 2);
  FieldActor_SetMoveTarget(gift, 0x1160000, 0, 0x1c40000);
  Actor_WaitForMove(gift);
  Func_02003de4(gift, 1);
  gift->update = 0;
  PartyInventory_Remove(455);
  Event_ShowMessage(9, 0);
  HeapSlot_AllocEwram(88, 664)[661] = 99;
  Actor_SetAnimationAndWait(gMainRoutineWork.currentOwner, 3);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
  leader->update = Func_02000df8;
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 312, 446);
  leader->update = 0;
  FieldActor_Jump(gMainRoutineWork.currentOwner, 6, 0);
  Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 296, 452);
  leader->update = Func_02001278;
  Actor_SetSpriteColor(Actor_Get(gMainRoutineWork.currentOwner), 15);
  Actor_SetAnimation(9, 3);
  FieldEvent_Wait(20);
  Event_ShowMessage(9, 0);
  Actor_SetSpeed(9, 0xcccc, 0x6666);
  gift->speedLimit = 0xcccc;
  gift->acceleration = 0x6666;
  guide->update = Func_02000df8;
  Actor_SetDestination(9, 288, 538);
  FieldEvent_Wait(20);
  Func_02003fc4();
  Func_020034ec();
  gift->update = Func_02000df8;
  Func_02003de4(gift, 2);
  FieldActor_SetMoveTarget(gift, 0x1200000, 0, 0x21a0000);
  FieldEvent_Wait(20);
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = ISLETS_GIFT_TRANSITION;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(3);
  Event_SetFlag(0xaa5);
}

