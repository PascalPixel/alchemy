#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

#include "text/msg_ids.h"
#include "islets.h"

#include "event_inline.h"

u8 *Scene_GetPlacements(void) {
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
  struct PushTarget target;
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

const u8 *Scene_GetEvents(void) {
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

#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "text/msg_ids.h"

void Func_02003ff4();
void Func_02003dfc(struct Actor *object);
void Func_02003fbc(void);
void Func_02003fc4(void);
void PartyInventory_Remove(s32 item);

void Func_02000210(void) {
  s32 result0;
  s32 result1;

  Event_Start();
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
  Event_End();
  return;
}

void Func_020002ac(void) {
  s32 result0;
  s32 result1;

  Event_Start();
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
  Event_End();
  return;
}

void Func_0200030c(void) {
  s32 result0;
  s32 result1;

  Event_Start();
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
  Event_End();
  return;
}

void Func_02000380(void) {
  s32 result0;
  s32 result1;

  Event_Start();
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

  turn = Actor_Get(gMainRoutineWork.currentOwner)->facing + 0x5fff;
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
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgIAmHere);
  Event_ShowMessage(21, 0);
  Event_End();
L_0200843a:
  return;
}

void Func_02000440(void) {
  s32 result0;

  Event_Start();
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
  Event_End();
  return;
}

void Func_020004f4(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgTheEasternSea);
  Event_ShowMessage(9, 0);
  Event_ShowMessage(9, 0);
  Event_End();
  return;
}

void Func_02000520(void) {
  s32 result0;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  result0 = Event_GetFlag(0xaa4);
  if (result0 == 0)
    goto L_0200854a;
  FieldActor_WatchAndSetScript(10, gMainRoutineWork.currentOwner);
L_0200854a:
  FieldEvent_SetMessageId((s32)MsgChirpChirp2);
  Event_ShowMessage(10, 0);
  Event_End();
  return;
}

/// Object 10's talk, and its exchange of the party's item 0x1c4 for item 0x1c5.
void Func_02000568(void) {
  struct Actor *leader;
  struct Actor *object;
  struct Actor *spawned;

  Event_Start();
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
  Event_End();
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

void Func_02003f74(void);

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

/// The Islets' trade of item 453 with object 10. With the item in the party, if
/// flag 513 is set (asked before) the camera moves and object 10 asks again
/// (MsgAreYouGiving); otherwise it notices the item (MsgOhThatRed), the leader
/// steps up and shows it (object type 489, its picture set to the item by
/// Func_0802372c) and object 10 asks. A yes hands the item across step by step,
/// takes item 453 and gives item 454 (object 10 hands it back over the leader's
/// head), sets flag 0xaa3 and counts the answer in the event work's halfword
/// 0x1c4 the first time; a no sets flag 513. Without the item, object 10 says
/// MsgYupANice after the trade or MsgIVeBeenFeeling before it.
void Func_020006f4(void) {
  struct Actor *item, *leader, *given;
  const u8 *asked, *noticed;
  /* FAKEMATCH(temporary): zero wrapped in a struct of one u16, so it stays a
   * HImode pseudo and its 0 is loaded from the pool; as s32 zero, 25 of 687
   * instructions differ in tla-en. */
  struct {
    u16 v;
  } zero;

  Event_Start();
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
  Event_End();
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
      Actor_StartScript(spawned, gData_020044f0);
    }
  }
}

#include "call.h"

extern const u8 gData_020043f8[], gData_02004300[];

#define STEP(actor, x, z)                                                      \
  (Actor_GetById(actor)->actionFlags &= ~1, Actor_WalkToAndWait(actor, x, z),  \
   FieldEvent_Wait(1), Actor_GetById(actor)->actionFlags |= 1)

void Func_02000e38(void) {
  struct Actor *leader, *trader, *item, *gift;
  s32 right;
  s32 giftItem;

  Event_Start();
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
    Actor_GetById(gMainRoutineWork.currentOwner)->facing = 0x4000;
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
  Event_End();
}

void Func_02001278(struct Actor *object) {
  struct Actor *source = Actor_Get(9);

  FieldActor_SetPositionAndResetMotion(object, source->x, source->y + PIXELS(16),
                                  source->z);
  object->facing = source->facing;
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
  ((u8 *)HeapSlot_AllocEwram(88, 664))[661] = 99;
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

// The value stored at the event work's 0x1ac: 521 in the Japanese release,
// 513 from patch 1.
#if TLA_PATCH_1
#define STORED_AT_1AC 513
#else
#define STORED_AT_1AC 521
#endif

#define SPAWN(a, b, answer)                                                    \
  {                                                                            \
    a = FieldActor_Create(163, 0x1280000, \
                               0, 0x1a80000);                                  \
    b = FieldActor_Create(14, 0x1280000, \
                               0, 0x1a80000);                                  \
    if (a != 0) {                                                              \
      a->motionFlags = answer;                                                 \
      Actor_SetSingleChildFlags(a, 0);                                         \
    }                                                                          \
    if (b != 0) {                                                              \
      b->scaleY = 0x20000;                                                     \
      Actor_SetSingleChildFlags(b, 0);                                         \
      Func_02003de4(b, 1);                                                     \
      Actor_StartScript(b, gData_020044f0);                                        \
    }                                                                          \
  }

void Func_020014e0(void) {
  struct Actor *leader, *a, *b, *other;
  const u8 *message;
  s32 answer;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if (PartyInventory_FindOwner(455) != -1) {
    leader = Actor_GetById(gMainRoutineWork.currentOwner);
    if (GameFlag_Test(513) != 0) {
      Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
      /* FAKEMATCH(temporary): the form kept; as leader->update = Func_02000df8,
       * 94 of 569 instructions differ in tla-en. */
      a = (struct Actor *)Func_02000df8;
      leader->update = (void (*)(void))a;
      Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 318, 424);
      leader->update = 0;
      Actor_FaceDirection(gMainRoutineWork.currentOwner, 0x4000, 0);
      Actor_FaceDirectionAndWait(9, 0xd000);
      message = MsgOhYouRe;
      Event_SetMessageId(message);
      Event_ShowMessage(9, 0);
      Func_02003fc4();
      answer =
          Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0);
      if (answer == 0) {
        Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, 0x8000);
        Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
        leader->update = (void (*)(void))a;
        STEP(gMainRoutineWork.currentOwner, 302, 424);
        SPAWN(a, b, answer)
        STEP(gMainRoutineWork.currentOwner, 318, 424);
        leader->update = 0;
        Actor_FaceDirection(gMainRoutineWork.currentOwner, 0x6000, 0);
        Func_0200129c(a);
      } else {
        Func_02003fbc();
        Event_SetMessageId(message - 1);
        goto refused;
      }
    } else {
      Event_SetMessageId(MsgIMSoVery);
      Event_ShowMessage(9, 0);
      Func_080d47b4(gMainRoutineWork.currentOwner, 262, 20);
      Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
      leader->update = Func_02000df8;
      // Default: a is set for real when object 163 is made.
      a = 0;
      Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 318, 424);
      leader->update = 0;
      Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, 0x8000);
      Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
      leader->update = Func_02000df8;
      STEP(gMainRoutineWork.currentOwner, 302, 424);
      SPAWN(a, b, 0)
      STEP(gMainRoutineWork.currentOwner, 318, 424);
      leader->update = 0;
      Actor_FaceDirection(gMainRoutineWork.currentOwner, 0x4000, 0);
      Func_080d47b4(9, 256, 20);
      Actor_FaceDirectionAndWait(9, 0xb000);
      other = Actor_GetById(9);
      other->update = Func_02000df8;
      FieldActor_Jump(9, 4, 20);
      other->update = 0;
      Event_ShowMessage(9, 0);
      Actor_FaceDirectionAndWait(9, 0xd000);
      Event_ShowMessage(9, 0);
      Func_02003fc4();
      if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) ==
          0) {
        Func_0200129c(a);
      } else {
        Func_02003fbc();
        Func_080d47b4(9, 265, 0);
        Event_ShowMessage(9, 0);
        Actor_FaceDirection(9, 0x5000, 0);
        Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, 0x8000);
        leader->update = Func_02000df8;
        Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 302, 424);
        leader->update = 0;
        if (a != 0)
          Func_02003dfc(a);
        Event_SetFlag(513);
      }
    }
  } else if (GameFlag_Test(0xaa5)) {
    Actor_FaceActor(9, gMainRoutineWork.currentOwner, 0);
    Event_SetMessageId(MsgWantToGo);
    Event_ShowMessage(9, 0);
    Func_02003fc4();
    if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) ==
        0) {
      struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
      Func_02003fbc();
      (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
      Event_ShowMessage(9, 0);
      ((u8 *)HeapSlot_AllocEwram(88, 664))[0x295] = 99;
      FieldActor_SetAnimationAndWait(gMainRoutineWork.currentOwner, 3);
      Actor_FaceDirectionAndWait(9, 0x5000);
      Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
      FieldActor_Jump(gMainRoutineWork.currentOwner, 6, 0);
      Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 296, 452);
      leader->update = Func_02001278;
      Actor_SetSpriteColor(Actor_GetById(gMainRoutineWork.currentOwner), 15);
      Actor_SetAnimation(9, 3);
      FieldEvent_Wait(20);
      Event_ShowMessage(9, 0);
      Func_02003fc4();
      Actor_SetSpeed(9, 0xcccc, 0x6666);
      Actor_SetSpeed(10, 0xcccc, 0x6666);
      Actor_GetById(9)->update = Func_02000df8;
      Actor_SetDestination(9, 288, 538);
      FieldEvent_Wait(40);
      Func_02003fc4();
      Func_020034ec();
      Actor_GetById(10)->update = Func_02000df8;
      Actor_WalkTo(10, 288, 538);
      FieldEvent_Wait(20);
      *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = STORED_AT_1AC;
      Event_FinishScreenTransition();
      Event_WaitTransitionFrames();
      Func_02003fbc();
      FieldEvent_SetExitRequest(3);
    } else {
      Func_02003fbc();
    refused:
      Event_ShowMessage(9, 0);
      Actor_FaceDirectionAndWait(9, 0x5000);
    }
  } else {
    Event_SetMessageId(MsgIMSoVeryVery);
    Event_ShowMessage(9, 0);
  }
  Event_End();
}

extern const u8 gData_020045a4[];
extern const u8 gData_020045f0[];

void Func_02001a68(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct Actor *object = Actor_Get(9);
  void (*update)(void);
  s32 result;
  u8 *done;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_FaceActor(9, gMainRoutineWork.currentOwner, 0);
  FieldEvent_Wait(10);
  FieldEvent_SetMessageId((s32)MsgYouAllSet);
  Event_ShowMessage(9, 0);
  Func_02003fc4();
  result = FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0);
  if (result == 0) {
    Func_02003fbc();
    Event_ShowMessage(9, 0);
    ((struct {
      u8 unk000[0x295];
      u8 unk295;
    } *)HeapSlot_AllocEwram(88, 0x298))
        ->unk295 = 99;
    FieldActor_Jump(gMainRoutineWork.currentOwner, 6, 0);
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
    Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 152, 768);
    leader->update = (void (*)(void))Func_02001278;
    Actor_SetSpriteColor(Actor_Get(gMainRoutineWork.currentOwner), 15);
    Actor_SetAnimation(9, 3);
    FieldEvent_Wait(20);
    Func_02003fc4();
    Actor_SetSpeed(9, 0x19999, 0xcccc);
    Actor_SetSpeed(10, 0x19999, 0xcccc);
    update = (void (*)(void))Func_02000df8;
    done = (u8 *)object + 99;
    *done = 0;
    object->update = update;
    Actor_SetScript(9, gData_020045a4);
    FieldEvent_Wait(30);
    Actor_Get(10)->update = update;
    Actor_SetScript(10, gData_020045f0);
    do {
      System_Wait(1);
    } while (*done == 0);
    Func_020034ec();
    System_Wait(1);
    ((struct IsletsEventWork *)Ram_HeapSlots->eventWork)->field1ac = 0x201;
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_02003fbc();
    FieldEvent_SetExitRequest(4);
  } else {
    Func_02003fbc();
    FieldEvent_SetMessageId((s32)MsgYouAllSet + 4);
    Event_ShowMessage(9, 0);
    Actor_FaceDirectionAndWait(9, FACING_SOUTH);
  }
  Event_End();
}

void Func_02001bf4(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_FaceActor(9, gMainRoutineWork.currentOwner, 0);
  FieldEvent_SetMessageId((s32)MsgMyPopS);
  Event_ShowMessage(9, 0);
  Actor_FaceDirectionAndWait(9, FACING_NORTHWEST + FACING_STEP);
  Event_End();
}

void Func_02001c38(void) {
  struct IsletsEventWork *work = Ram_HeapSlots->eventWork;

  work->field15a = 1;
}

void Func_02001c4c(void) {
  if (Actor_Get(gMainRoutineWork.currentOwner)->y < 0x280000) {
    Actor_Get(11)->collisionFlags |= 8;
    Actor_Get(12)->collisionFlags |= 8;
  } else {
    Actor_Get(11)->collisionFlags &= ~8;
    Actor_Get(12)->collisionFlags &= ~8;
  }
}

#include "dma.h"
#include "io_reg.h"

extern u8 gData_02004f04;
extern s16 gData_02004f10[2][160];

/// Applies BG1 horizontal line scrolling with HBlank DMA.
void Func_02001cac(void) {
  s16 *lines = gData_02004f10[gData_02004f04];
  s32 first = lines[0];
  volatile u16 *scroll;
  volatile u16 *dma;

  /* FAKEMATCH(barrier): an asm reading the first line keeps its load before the
   * register address is built; as no asm, 6 of 34 instructions differ in
   * tla-en. */
  __asm__("" : : "r"(first));
  scroll = &REG_BG1HOFS;
  *scroll = first;
  dma = REG_DMA0;

  dma[5] &= 0xc5ff; // Clear start timing and repeat.
  dma[5] &= 0x7fff; // Stop.
  lines++;
  (void)dma[5];
  Dma_Set(lines, (void *)scroll, DMA_HBLANK_HALFWORD_PER_LINE,
          (volatile u32 *)dma);
}

void Func_02001cfc(void) {
  s16 *layer = (s16 *)&Ram_HeapSlots->mapWork->layers[2];
  s16 *lines = gData_02004f10[gData_02004f04 ^ 1];
  s32 v[2];
  s32 *count = &v[1];
  s32 value;
  s32 sine;

  for (*count = 0; *count != 160; (*count)++) {
    /* FAKEMATCH(dead): the sine's call and store in a once-only block; as plain
     * statements, 2 of 65 instructions differ in tla-en. */
    do {
      sine = Math_Sin((*count + *(u8 *)&gFrameCount + layer[3]) << 9);
      v[0] = sine;
    } while (0);
    value = layer[1] + (sine >> 15);
    value += *count % 3;
    *lines++ = value - 1;
  }
  gData_02004f04 ^= 1;
}

void Func_02001d90(void) {
  Task_AddFrame(Func_02001cfc, 0x480);
  Task_AddFrame(Func_02001cac, 0xc80);
}

/// When object 19 stands in its target area, actor 9 and the leader react and
/// the party is given item 0x1c4.
void Func_02001db0(void) {
  struct Actor *object = Actor_GetById(19);
  struct Actor *leader;
  struct Actor *item;
  s32 z;

  if (object->x < 0xe40000 || object->x > 0xec0000)
    return;
  z = object->z;
  if (z < 0xa40000)
    return;
  if (z > 0xac0000)
    return;
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetAnimation(19, 1);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 216, 184);
  FieldPsynergy_StartMindReadWave(gMainRoutineWork.currentOwner, 9);
  FieldEvent_SetMessageId((s32)MsgThankYouSo);
  FieldActor_Jump(9, 2, 10);
  FieldActor_Jump(9, 4, 10);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  Actor_FaceDirection(9, FACING_WEST, 10);
  Event_ShowMessage(9, 0);
  Actor_GetById(9)->actionFlags &= ~1;
  Actor_WalkToAndWait(9, 222, 186);
  FieldEvent_Wait(1);
  Actor_GetById(9)->actionFlags |= 1;
  Actor_GetById(9)->actionFlags &= ~1;
  Actor_WalkToAndWait(9, 232, 186);
  FieldEvent_Wait(1);
  Actor_GetById(9)->actionFlags |= 1;
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, FACING_SOUTH);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 28);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  item = Actor_GetById(20);
  item->motionFlags = 0;
  Actor_SetItemIcon(item, 0x1c4);
  FieldActor_SetPositionAndResetMotion(item, leader->x, leader->y + PIXELS(24),
                                  leader->z);
  FieldEvent_Wait(20);
  Func_02003fc4();
  Party_GiveItem(0x1c4, 0);
  Func_02003fbc();
  Actor_SetPosition(20, 0, 0);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
  Func_02003fb4();
  Actor_FaceDirectionAndWait(9, FACING_NORTHWEST + FACING_STEP);
  Event_SetFlag(0xaa2);
  Event_End();
}

void Func_02001f58(void) {
  if ((gFrameCount & 15) == 0)
    Event_PlaySound(118);
}

void Func_02001f74(void) {
  struct Actor *object = Actor_Get(10);

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldActor_Stop(10);
  System_Wait(1);
  object->motionFlags = 0;
  Actor_SetSpeed(10, 0x19999, 0xcccc);
  FieldActor_SetPositionAndResetMotion(object, PIXELS(268), PIXELS(64), PIXELS(390));
  System_Wait(1);
  object->terrainHeight = PIXELS(128);
  System_Wait(1);
  object->y = PIXELS(160);
  System_Wait(1);
  FieldTask_AddFrame(Func_02001f58, 0x480);
  FieldActor_SetMoveTarget(object, PIXELS(132), PIXELS(128), PIXELS(320));
  Actor_WaitForMove(object);
  FieldTask_Remove(Func_02001f58);
  Actor_SetAnimation(10, 5);
  object->motionFlags = 3;
  FieldActor_SetSpritePriority(10, 2);
  Actor_FaceActor(10, gMainRoutineWork.currentOwner, 0);
  FieldEvent_Wait(10);
  FieldActor_Jump(10, 4, 0);
  Actor_MoveToAndWait(10, 120, 312);
  Actor_SetAnimation(10, 5);
  object->motionFlags = 0;
  Actor_FaceDirectionAndWait(10, FACING_EAST);
  Event_SetFlag(0x215);
  System_Wait(1);
  Event_End();
}

void Actor_JumpDownLedge();

void Func_0200208c(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_JumpDownLedge(gMainRoutineWork.currentOwner, 6, 1);
  Event_End();
  return;
}

void Func_020020b4(void) {
  struct Actor *object0;

  object0 = Actor_Get(gMainRoutineWork.currentOwner);
  object0->terrainId = 2;
  return;
}

void Func_020020d0(void) {
  struct Actor *object0;

  object0 = Actor_Get(gMainRoutineWork.currentOwner);
  object0->terrainId = 0;
  return;
}

void Func_020020ec(s32 x, s32 z, s32 layers, s32 value) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct MapCell *cells0 = map->layers[0].cells;
  struct MapCell *cells2 = map->layers[2].cells;
  s32 index = x / 0x100000 + (z / 0x100000) * 128;

  if (layers & 1) {
    cells0 += index;
    cells0->collisionCode = value;
  }
  if (layers & 2) {
    cells2 += index;
    cells2->collisionCode = value;
  }
}

/// Records objects 12 and 13's positions; with them on cells (7, 29) and
/// (7, 28), clears layer 2's collision under object 12, and with them on
/// (12, 25) and (12, 26), under object 13.
void Func_02002144(void) {
  struct Actor *object;
  s32 *x0;
  s32 *z0;
  s32 x1;
  s32 z1;

  object = Actor_Get(12);
  x0 = &gData_0200519c;
  z0 = &gData_02005194;
  *x0 = object->x;
  *z0 = object->z;
  object = Actor_Get(13);
  gData_02005198 = x1 = object->x;
  {
    /* FAKEMATCH(pin): object 13's z in r2, as the native keeps it for its
     * test; as a plain local, 31 of 58 instructions differ in tla-en. */
    register s32 z asm("r2");

    z = object->z;
    gData_02005190 = z;
    z1 = z;
  }
  if (*x0 >> 20 == 7 && *z0 >> 20 == 29 && x1 >> 20 == 7 && z1 >> 20 == 28)
    Func_020020ec(*x0, *z0, 2, 0);
  if (gData_0200519c >> 20 == 12 && gData_02005194 >> 20 == 25 &&
      gData_02005198 >> 20 == 12 && gData_02005190 >> 20 == 26)
    Func_020020ec(gData_02005198, gData_02005190, 2, 0);
}

void Func_020021d0(void) {
  struct Actor *first = Actor_Get(12);
  struct Actor *second = Actor_Get(13);
  s32 firstX = first->x >> 20;
  s32 firstZ = first->z >> 20;
  s32 secondX = second->x >> 20;
  s32 secondZ = second->z >> 20;
  s32 both;

  if (gData_0200519c >> 20 == 12 && gData_02005194 >> 20 == 24) {
    Func_020020ec(gData_0200519c, gData_02005194, 1, 21);
    Func_020020ec(gData_0200519c, gData_02005194, 2, 0);
  } else {
    s32 *z = &gData_02005194;

    Func_020020ec(gData_0200519c, *z, 3, 0);
  }
  if (gData_02005198 >> 20 == 7 && gData_02005190 >> 20 == 27) {
    Func_020020ec(gData_02005198, gData_02005190, 1, 21);
    Func_020020ec(gData_02005198, gData_02005190, 2, 0);
  } else {
    s32 *z = &gData_02005190;

    Func_020020ec(gData_02005198, *z, 3, 0);
  }
  Func_020020ec(first->x, first->z, 3, 255);
  Func_020020ec(second->x, second->z, 3, 255);
  if (!Event_GetFlag(0x210) && firstX == 12 && firstZ == 26) {
    both = 0;
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    if (secondX == 12 && secondZ == 26) {
      MapGrid_CopyCellAttributes(7, 2, 1, 1, firstX, firstZ);
      both = 1;
    }
    first->motionFlags = 3;
    System_Wait(10);
    Event_PlaySound(161);
    System_Wait(10);
    first->motionFlags = 0;
    Event_SetFlag(0x210);
    if (both) {
      Func_020020ec(first->x, first->z, 3, 0);
      Func_020020ec(second->x, second->z, 3, 0);
      MapGrid_CopyCellAttributes(7, 3, 1, 1, 12, 26);
      Event_SetFlag(0x212);
      ((u8 *)first)[0x62] = 1;
      ((u8 *)second)[0x62] = 1;
    }
    Event_End();
  }
  if (!Event_GetFlag(0x211) && secondX == 7 && secondZ == 29) {
    both = 0;
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    if (firstX == 7 && firstZ == 29) {
      MapGrid_CopyCellAttributes(7, 2, 1, 1, firstX, firstZ);
      both = 1;
    }
    second->motionFlags = 3;
    System_Wait(10);
    Event_PlaySound(161);
    System_Wait(10);
    second->motionFlags = 0;
    Event_SetFlag(0x211);
    if (both) {
      Func_020020ec(first->x, first->z, 3, 0);
      Func_020020ec(second->x, second->z, 3, 0);
      MapGrid_CopyCellAttributes(7, 3, 1, 1, 7, 29);
      Event_SetFlag(0x212);
      ((u8 *)first)[0x62] = 1;
      ((u8 *)second)[0x62] = 1;
    }
    Event_End();
  }
}

void Func_02002430(void) {
  struct Actor *first = Actor_Get(12);
  struct Actor *second = Actor_Get(13);

  gData_0200519c = first->x;
  gData_02005194 = first->z;
  gData_02005198 = second->x;
  gData_02005190 = second->z;
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if (gData_0200519c >> 20 == 7 && gData_02005194 >> 20 == 29 &&
      gData_02005198 >> 20 == 7 && gData_02005190 >> 20 == 28) {
    ((u8 *)first)[0x62] = 0;
    ((void (*)())Func_020020ec)(gData_0200519c, gData_02005194, 2);
  }
  Func_02003fe4();
  Func_020021d0();
  Event_End();
}

void Func_020024c8(void) {
  MapGrid_CopyCellAttributes(5, 2, 1, 2, 5, 28);
  Event_SetFlag(0x213);
}

void Func_020024f0(void) {
  struct Actor *object;

  if (gInput.held & 0x40) {
    object = Actor_Get(gMainRoutineWork.currentOwner);
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    object->motionFlags = 0;
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 72, 280);
    object->motionFlags = 3;
    Event_End();
  }
}

void Func_0200253c(void) {
  struct Actor *object;
  u16 *state;

  if (gInput.held & 0x80) {
    state = gSceneState;
    object = Actor_Get(gMainRoutineWork.currentOwner);
    if ((u16)(*state + 0xfff6) > 200) {
      Event_Start();
      FieldEvent_ResolvePendingActions(0);
      object->motionFlags = 0;
      Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 72, 296);
      object->motionFlags = 3;
      Event_End();
    }
  }
}

void Func_02003fec();

void Func_020025a4(void) {
  Event_SetFlag(0x218);
  return;
}

void Func_020025b0(void) {
  Event_ClearFlag(0x218);
  return;
}

// The following six are from patch 1.
#if TLA_PATCH_1
void Func_020025bc(void) {
  Call7((void (*)())Func_02003fec, 65, 0, 4, 4, 65, 10, 1);
  return;
}

void Func_020025dc(void) {
  Call7((void (*)())Func_02003fec, 65, 0, 2, 2, 71, 10, 1);
  return;
}

void Func_020025fc(void) {
  Call7((void (*)())Func_02003fec, 65, 0, 1, 1, 68, 15, 1);
  return;
}

void Func_0200261c(void) {
  Call7((void (*)())Func_02003fec, 65, 0, 2, 3, 75, 20, 1);
  return;
}

void Func_0200263c(void) {
  Call7((void (*)())Func_02003fec, 65, 0, 4, 1, 78, 22, 1);
  return;
}

void Func_0200265c(void) {
  Call7((void (*)())Func_02003fec, 65, 0, 1, 1, 79, 23, 1);
  return;
}
#endif

void Func_0200267c(void) {
  struct IsletsEventWork *work = Ram_HeapSlots->eventWork;
  struct MainRoutineWork *party = &gMainRoutineWork;
  struct Actor *object;
  s16 *exit;
  s32 index;

  object = Actor_Get(party->currentOwner);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Actor_SetAnimation(party->currentOwner, 2);
  Actor_SetSpeed(party->currentOwner, 0x8000, 0x4000);
  FieldActor_SetMoveTarget(object, ((8 - (object->x >> 16) % 16) << 16) + object->x,
                      object->y, object->z);
  Actor_WaitForMove(object);
  object->facing = FACING_NORTH;
  exit = &work->exit;
  index = *exit;
  Event_PlaySound(158);
  index--;
  MapGrid_AnimateCells(gData_020046bc[index].value, gData_020046bc[index].pair.first, gData_020046bc[index].pair.second);
  Actor_SetScript(party->currentOwner, gData_02004028);
  System_Wait(12);
  Event_PlaySound(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(*exit);
}

void Func_0200274c(void) {
  struct IsletsEventWork *work = Ram_HeapSlots->eventWork;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Func_020034ec();
  Event_PlaySound(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(work->exit);
}

void Func_02002780(void) {
  struct IsletsEventWork *work = Ram_HeapSlots->eventWork;
  struct MainRoutineWork *party = &gMainRoutineWork;
  struct Actor *object;

  object = Actor_Get(party->currentOwner);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Actor_SetAnimation(party->currentOwner, 2);
  Actor_SetSpeed(party->currentOwner, 0x8000, 0x4000);
  FieldActor_SetMoveTarget(object, ((8 - (object->x >> 16) % 16) << 16) + object->x,
                      object->y, object->z);
  Actor_WaitForMove(object);
  object->facing = FACING_NORTH;
  Actor_SetScript(party->currentOwner, gData_02004028);
  System_Wait(12);
  Event_PlaySound(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(work->exit);
}

void Func_02002830(void) {
  struct IsletsEventWork *work;
  struct Actor *owner = Actor_Get(gMainRoutineWork.currentOwner);
  struct Actor *object = Actor_Get(9);

  Actor_MoveToObject(9, gMainRoutineWork.currentOwner);
  Actor_SetPosition(10, PIXELS(273), PIXELS(520));
  System_Wait(1);
  object->facing = FACING_NORTH;
  owner->update = (void (*)(void))Func_02001278;
  Actor_SetSpriteColor(Actor_Get(gMainRoutineWork.currentOwner), 15);
  Actor_SetAnimation(9, 3);
  System_Wait(1);
  work = Ram_HeapSlots->eventWork;
  work->field1ac = 0x201;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_SetSpeed(9, 0x9999, 0x4ccc);
  Actor_SetSpeed(10, 0x9999, 0x4ccc);
  Actor_Get(10)->update = (void (*)(void))Func_02000df8;
  object->update = (void (*)(void))Func_02000df8;
  Actor_WalkTo(10, 280, 468);
  Actor_MoveToAndWait(9, 296, 452);
  Actor_SetAnimation(10, 1);
  owner->update = 0;
  object->update = 0;
  Actor_Get(10)->update = 0;
  FieldPsynergy_StartMindReadWave(gMainRoutineWork.currentOwner, 9);
  FieldEvent_SetMessageId((s32)MsgHereWeAre);
  Event_ShowMessage(9, 0);
  Actor_SetAnimation(9, 1);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
  Actor_SetSpriteColor(Actor_Get(gMainRoutineWork.currentOwner), 0);
  FieldActor_Jump(gMainRoutineWork.currentOwner, 6, 0);
  Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 312, 440);
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, FACING_SOUTHWEST);
  Event_ShowMessage(9, 0);
  Func_02003fb4();
  Actor_FaceDirectionAndWait(9, FACING_SOUTH);
  work = Ram_HeapSlots->eventWork;
  work->field1ac = 0x100;
  Func_020034d8();
}

#define EVENT_WORK ((u8 *)Ram_HeapSlots->eventWork)
#define OBJECT(id) ((struct Actor *)((s32)Actor_Get((id))))

extern const u8 gData_02004508[];
extern const u8 gData_02004554[];

/// The Islets' arrival by entrance 4: actor 9 joins the leader and actor 10
/// is placed, the screen fades in, both run their action scripts
/// (Func_02000df8 updates) until actor 9's byte 0x63 is set, then actor 9
/// speaks (MsgWellHereWe) and the leader is launched up to (152, 742) and
/// turned; ends with Func_020034d8. The Japanese edition does not set the
/// event work's transition word 0x1ac.
void Func_020029c4(void) {
  struct Actor *leader = OBJECT(gMainRoutineWork.currentOwner);
  struct Actor *guide = OBJECT(9);
  u8 *done;

  Actor_MoveToObject(9, gMainRoutineWork.currentOwner);
  Actor_SetPosition(10, PIXELS(240), PIXELS(540));
  System_Wait(1);
  guide->facing = FACING_SOUTH;
  leader->update = Func_02001278;
  Actor_SetSpriteColor(((s32)Actor_Get(gMainRoutineWork.currentOwner)),
                         15);
  Actor_SetAnimation(9, 3);
  System_Wait(1);
#if TLA_PATCH_1 /* The Japanese release leaves the transition word. */
  *(s32 *)(EVENT_WORK + 0x1ac) = 513;
#endif
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_SetSpeed(9, 0xcccc, 0x6666);
  Actor_SetSpeed(10, 0xcccc, 0x6666);
  OBJECT(10)->update = Func_02000df8;
  guide->update = Func_02000df8;
  done = (u8 *)guide + 0x63;
  *done = 0;
  Actor_StartScript((s32)guide, gData_02004508);
  Actor_SetScript(10, gData_02004554);
  do
    System_Wait(1);
  while (*done == 0);
  leader->update = 0;
  guide->update = 0;
  OBJECT(10)->update = 0;
  Actor_FaceDirectionAndWait(9, FACING_NORTH);
  FieldPsynergy_StartMindReadWave(gMainRoutineWork.currentOwner, 9);
  FieldEvent_SetMessageId(MsgWellHereWe);
  Event_ShowMessage(9, 0);
  Actor_SetAnimation(9, 1);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
  Actor_SetSpriteColor(((s32)Actor_Get(gMainRoutineWork.currentOwner)),
                         0);
  FieldActor_Jump(gMainRoutineWork.currentOwner, 6, 0);
  Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 152, 742);
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, FACING_SOUTH);
  Event_ShowMessage(9, 0);
  Func_02003fb4();
  Actor_FaceDirectionAndWait(9, FACING_SOUTH);
  Func_020034d8();
#if TLA_PATCH_1
  *(s32 *)(EVENT_WORK + 0x1ac) = 0x100;
#endif
}

s32 Scene_Initialize(void) {
  struct IsletsEventWork *work = Ram_HeapSlots->eventWork;
  s16 scene;

  work->field1ac = 0x100;
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_Ov674Room1)
    Func_02002c38();
  else if (scene == (s32)&SceneId_Ov674Room2)
    Func_02002f1c();
  else if (scene == (s32)&SceneId_Ov674Room3)
    Func_020030b4();
  else if (scene == (s32)&SceneId_Ov674Room4)
    Func_02003164();
  else if (scene == (s32)&SceneId_Ov674Room5)
    Func_0200329c();
  return 0;
}

void Func_02002bd8(void) {
  if (Actor_Get(gMainRoutineWork.currentOwner)->y > 0x80000) {
    Actor_Get(9)->collisionFlags &= ~8;
    Actor_Get(10)->collisionFlags &= ~8;
  } else {
    Actor_Get(9)->collisionFlags |= 8;
    Actor_Get(10)->collisionFlags |= 8;
  }
}

void Func_02002c38(void) {
  struct IsletsMapFlags *map = (struct IsletsMapFlags *)Ram_HeapSlots->mapWork;
  struct Actor *spawned;

  if (gMainRoutineWork.entrance == 2)
    Event_ClearFlag(0x12f);
  map->mode = 1;
  Actor_SetSingleChildFlags(Actor_Get(11), 0);
  Actor_SetSpeed(10, 0xcccc, 0x6666);
  Actor_Get(10)->collisionFlags |= 0x80;
  if (Event_GetFlag(0xaa1)) {
    spawned = FieldActor_Create(489, 0x1180000, 0, 0x3480000);
    if (spawned != 0) {
      spawned->motionFlags = 0;
      Actor_SetItemIcon(spawned, 454);
      Actor_SetSingleChildFlags(spawned, 0);
      spawned->collisionFlags = 9;
      spawned->unk5c[0] = 1;
    }
    Actor_Get(11)->facing = FACING_NORTH + FACING_STEP;
  } else {
    Actor_SetAnimation(11, 5);
  }
  Task_AddFrame(Func_02002bd8, 0x480);
}

void Func_02002d0c(s32 id) {
  struct Actor *object = Actor_Get(id);

  Actor_SetSingleChildFlags(object, 0);
  object->unk5c[0] = 3;
  object->motionFlags = 0;
  object->priorityFlags |= 0x80;
  object->acceleration = 0x2666;
  object->speedLimit = 0x4ccc;
}

void Func_02002d48(s32 id, s32 column, s32 row) {
  struct Actor *object = Actor_Get(id);
  s32 x = (column * 32 + object->timer) << 16;
  s32 z = (row * 32 + (s16)object->unk66) << 16;

  FieldActor_SetMoveTarget(object, x, object->y, z);
}

// The Islets' marching guards (objects 11 to 18): while the event work is busy
// or flag 0x218 is set they stand still; once the controlled character drops
// below height -12 they stand and the event work's counter 0x16a becomes 200;
// otherwise, for each frame owed (gSceneState's halfword 2) while no event
// runs, a 260-frame routine sets them walking with their facing steps
// (Func_02002d48) at 0, still at 100 and 220, and walking in place at 130.
void Func_02002d7c(void) {
  u16 *step = (u16 *)((u8 *)gSceneState);
  u8 *work = Ram_HeapSlots->eventWork;
  u16 *owed = (u16 *)(((u8 *)gSceneState) + 2);
  s32 i;
  s32 count;

  if (*(s16 *)(work + 0x166) != 0 || GameFlag_Test(0x218)) {
    for (i = 11; i <= 18; i++)
      Actor_GetById(i)->movementState = 1;
    return;
  }
  for (i = 11; i <= 18; i++)
    Actor_GetById(i)->movementState = 0;
  if (Actor_GetById(gMainRoutineWork.currentOwner)->y < -0xc0000) {
    work = Ram_HeapSlots->eventWork;
    for (i = 11; i <= 18; i++)
      Actor_GetById(i)->movementState = 1;
    {
      s16 *countdown = (s16 *)(work + 0x16a);
      s32 ticks = 200;

      *countdown = ticks;
    }
    return;
  }
  count = *owed + 1;
  *owed = count;
  if (*(s16 *)(work + 0x156) != 0 || (u16)count == 0)
    return;
  do {
    switch (*step) {
    case 0:
      for (i = 11; i <= 18; i++)
        Actor_SetAnimation(i, 2);
      Func_02002d48(11, 0, 1);
      Func_02002d48(12, 0, -1);
      Func_02002d48(13, 0, 1);
      Func_02002d48(14, 0, 1);
      Func_02002d48(15, -1, 0);
      Func_02002d48(16, 0, 1);
      Func_02002d48(17, 1, 0);
      Func_02002d48(18, 0, -1);
      break;
    case 100:
    case 220:
      for (i = 11; i <= 18; i++)
        Actor_SetAnimation(i, 1);
      break;
    case 130:
      for (i = 11; i <= 18; i++) {
        Actor_SetAnimation(i, 2);
        Func_02002d48(i, 0, 0);
      }
      break;
    }
    if (++*step == 260)
      *step = 0;
  } while (--*owed != 0);
}

s32 Func_02003684(s32 id);
extern const u8 gData_02004274[];

/// The room's setup: flags, the objects' places and the scene state.
void Func_02002f1c(void) {
  struct Actor *object;
  struct Actor *other;
  u16 *state;
  u16 *next;
  s32 angle;
  s32 i;

  Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 64;
  if (gMainRoutineWork.entrance == 2)
    Event_ClearFlag(0x12f);
  i = 11;
  do {
    Func_02002d0c(i++);
  } while (i <= 18);
  state = gSceneState;
  next = &gSceneState[1];
  if (!Event_GetFlag(0x109)) {
    *state = 0;
    *next = 0;
  }
  FieldTask_AddFrame(Func_02002d7c, 0x480);
  if (Event_GetFlag(0xa2c)) {
    object = Actor_Get(10);
    Actor_SetPosition(10, PIXELS(176), PIXELS(168));
    Actor_SetAnimation(10, 4);
    object->priorityFlags |= 2;
    object->motionFlags = 0;
    object->y = PIXELS(-8);
    object->terrainHeight = PIXELS(-8);
    MapGrid_CopyCellAttributes(9, 1, 4, 1, 9, 10);
    MapGrid_CopyCellAttributes(9, 64, 4, 1, 9, 74);
  } else {
    Func_02003684(10);
  }
  if (!Event_GetFlag(0xaa2)) {
    FieldActor_WatchAndSetScript(9, 19);
    Actor_SetAnimation(19, 0);
    Actor_Get(19)->actionFlags &= ~1;
  } else if (!Event_GetFlag(0x109)) {
    other = Actor_Get(9);
    other->facing = FACING_SOUTH + FACING_STEP;
    other->collisionFlags |= 0x80;
    /* FAKEMATCH(temporary): the angle held in a local is built before the
     * position; as the constant argument 0x3000, 4 of 166 instructions differ
     * in tla-en. */
    angle = 0x3000;
    Actor_SetPositionAndFacing(19, PIXELS(234), PIXELS(204),
                                                   angle);
    Actor_Get(19)->collisionFlags |= 0x80;
    Actor_SetSpeed(9, 0x20000, 0x10000);
    Actor_SetSpeed(19, 0x19999, 0xcccc);
    Actor_SetScript(9, gData_02004274);
    Actor_SetScript(19, gData_02004274);
    Event_SetFlag(0x200);
  }
}

void Func_020030b4(void) {
  struct Actor *spawned;

  if (gMainRoutineWork.entrance == 3)
    Event_ClearFlag(0x12f);
  Actor_SetSingleChildFlags(Actor_Get(10), 0);
  Actor_SetAnimation(10, 4);
  if (Event_GetFlag(0xaa3)) {
    spawned = FieldActor_Create(489, 0x680000, 0x180000, 0xa00000);
    if (spawned != 0) {
      spawned->motionFlags = 0;
      Actor_SetItemIcon(spawned, 453);
      Actor_SetSingleChildFlags(spawned, 0);
      spawned->unk5c[0] = 1;
    }
    Actor_SetPosition(10, PIXELS(104), PIXELS(204));
  }
  FieldActor_SetSpritePriority(11, 1);
  Actor_SetSpriteColor(Actor_Get(11), 15);
  Actor_Get(11)->priorityFlags |= 2;
}

// A room's setup.

void Func_02003f4c(s32 actor);

struct IsletsMapWork {
  u8 unk000[0x190];
  s32 value190;
  s32 value194;
};

/// Sets up the room's actors and map work.
void Func_02003164(void) {
  struct IsletsMapWork *map = (struct IsletsMapWork *)Ram_HeapSlots->mapWork;
  struct Actor *object = Actor_Get(10);
  struct Actor *last;
  s32 set;
  s32 zero = 0;

  map->value190 = zero;
  map->value194 = zero;
  if (gMainRoutineWork.entrance == 11)
    Event_ClearFlag(0x12f);
  if (!Event_GetFlag(0x109)) {
    object->motionFlags = zero;
    FieldActor_SetSpritePriority(10, 1);
  }
  Actor_SetPosition(11, 0, 0);
  Func_02003f4c(12);
  Func_02003f4c(13);
  set = Event_GetFlag(0x212);
  if (set) {
    struct Actor *first = Actor_Get(12);
    struct Actor *second = Actor_Get(13);

    first->motionFlags = zero;
    second->motionFlags = zero;
    ((u8 *)first)[0x62] = 1;
    ((u8 *)second)[0x62] = 1;
    {
      s32 held = 29;

      MapGrid_CopyCellAttributes(7, 3, 1, 1, 7, held);
    }
  } else {
    struct Actor *first = Actor_Get(12);
    struct Actor *second = Actor_Get(13);

    first->motionFlags = 0;
    second->motionFlags = 0;
    Func_020020ec(first->x, first->z, 3, 255);
    Func_020020ec(second->x, second->z, 3, 255);
  }
  Func_02003f4c(14);
  last = Actor_Get(14);
  {
    // A local that holds 0, as the native keeps its own register for it.
    s32 still = 0;

    ((u8 *)last)[0x62] = 1;
    last->motionFlags = still;
  }
  Func_020020ec(last->x, last->z, 1, 255);
  if (Event_GetFlag(0x213))
    MapGrid_CopyCellAttributes(5, 2, 1, 2, 5, 28);
}

#if TLA_PATCH_1

struct MapLayerBits {
  u8 unk00[23];
  u8 unk17Low : 2;
  u8 priority : 2;
  u8 unk17High : 4;
};

void Func_02003410(void);
void PaletteQueue_Add(s32 a, s32 b, s32 c, s32 d);

void Func_0200329c(void) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  s32 done;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  ((struct MapLayerBits *)map)->priority = 1;
  if (gMainRoutineWork.entrance == 2)
    Event_ClearFlag(303);
  Func_02003410();
  {
    s32 a1 = 12;
    s32 a2 = 5;
    s32 a3 = 4;
    s32 a0 = 3;

    /* FAKEMATCH(barrier): the native loads the first argument last; as the
     * plain call, 2 of 145 instructions differ in tla-en. */
    asm volatile("" : "+l"(a1), "+l"(a2), "+l"(a3));
    PaletteQueue_Add(a0, a1, a2, a3);
  }
  PaletteQueue_Add(4, 12, 5, 4);
  if (Event_GetFlag(0xaa5) && (done = Event_GetFlag(265)) == 0) {
    Actor_SetPosition(10, 0x1180000, 0x1d40000);
    Actor_Get(10)->motionFlags = done;
    {
      s32 a1 = 0x1280000;

      Actor_SetPosition(9, a1, 0x1c40000);
    }
  }
  Actor_SetSingleChildFlags(Actor_Get(9), 0);
  Actor_SetSingleChildFlags(Actor_Get(11), 0);
  Actor_SetSingleChildFlags(Actor_Get(12), 0);
  FieldTask_AddFrame(Func_02001c4c, 0x480);
  if ((u16)(gMainRoutineWork.entrance - 4) <= 1) {
    Event_SetFlag(0x217);
    if (!Event_GetFlag(265)) {
      {
        s32 a3 = 0x4000;

        Actor_SetPositionAndFacing(9, 0x980000, 0x3000000, a3);
      }
      Actor_SetPosition(10, 0xb00000, 0x3090000);
      System_Wait(1);
    }
  }
  if (!Event_GetFlag(265)) {
    s32 entrance = gMainRoutineWork.entrance;

    if (entrance == 3) {
      Func_02002830();
    } else if (entrance == 4) {
      Func_020029c4();
    } else if (entrance != 5) {
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
      Func_020034d8();
    }
  } else {
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Func_020034d8();
  }
  Event_End();
}

#else

// The Japanese release's older form.

struct MapLayerBits {
  u8 unk00[23];
  u8 unk17Low : 2;
  u8 priority : 2;
  u8 unk17High : 4;
};

void Func_02003410(void);
s32 PaletteQueue_Add(s16 bank, s16 index, s16 delay, s16 count);

void Func_0200329c(void) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  s32 done;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  ((struct MapLayerBits *)map)->priority = 1;
  if (gMainRoutineWork.entrance == 2)
    Event_ClearFlag(303);
  Func_02003410();
  PaletteQueue_Add(3, 12, 5, 4);
  PaletteQueue_Add(4, 12, 5, 4);
  if (Event_GetFlag(0xaa5) && (done = Event_GetFlag(265)) == 0) {
    Actor_SetPosition(10, 0x1180000, 0x1d40000);
    Actor_Get(10)->motionFlags = done;
    {
      s32 a1 = 0x1280000;

      Actor_SetPosition(9, a1, 0x1c40000);
    }
  }
  Actor_SetSingleChildFlags(Actor_Get(9), 0);
  Actor_SetSingleChildFlags(Actor_Get(11), 0);
  Actor_SetSingleChildFlags(Actor_Get(12), 0);
  FieldTask_AddFrame(Func_02001c4c, 0x480);
  if ((u16)(gMainRoutineWork.entrance - 4) <= 1) {
    Event_SetFlag(0x217);
    if (!Event_GetFlag(265)) {
      {
        s32 a3 = 0x4000;

        Actor_SetPositionAndFacing(9, 0x980000, 0x3000000, a3);
      }
      Actor_SetPosition(10, 0xb00000, 0x3090000);
      System_Wait(1);
    }
  }
  if (!Event_GetFlag(265)) {
    s32 entrance = gMainRoutineWork.entrance;

    if (entrance == 3) {
      Func_02002830();
    } else if (entrance == 4) {
      Func_020029c4();
    } else {
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
      Func_020034d8();
    }
  } else {
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Func_020034d8();
  }
  Event_End();
}

#endif

s32 Scene_PrepareMap(void) { return 0; }
