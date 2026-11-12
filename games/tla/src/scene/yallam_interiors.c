/// @file
/// The scene code of overlay 0x677, in ROM order.

#define OWN_Actor_SetSingleChildFlags
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
#include "yallam_interiors/yallam_interiors.h"
#include "inventory.h"
#include "version.h"
#include "event_inline.h"
#include "main_routine_work.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

u8 *Func_0200004c(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov677Room1)
    return gData_02000ca4;
  if (scene == (s32)&SceneId_Ov677Room2)
    return gData_02000dac;
  return gData_02000c8c;
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "event_inline.h"

s32 Func_0200008c(void) {
  s32 t2;

  Func_020009b0();
  Event_ResolvePendingActions(0);
  t2 = GameFlag_Test(0x301);
  if (t2 != 0) {
    Event_SetMessageId((s32)MsgYallamInteriors37);
  } else {
    Event_SetMessageId((s32)MsgYallamInteriors33);
  }
  Event_ShowMessageAndWait(14, 0);
  Func_020009b8();
  return 0;
}

#include "event_inline.h"

s32 Func_020000cc(void) {
  s32 t2;

  Func_020009b0();
  Event_ResolvePendingActions(0);
  t2 = GameFlag_Test(0x301);
  if (t2 != 0) {
    Event_SetMessageId((s32)MsgYallamInteriors38);
  } else {
    Event_SetMessageId((s32)MsgYallamInteriors34);
  }
  Event_ShowMessageAndWait(15, 0);
  Func_020009b8();
  return 0;
}

#include "event_inline.h"

s32 Func_0200010c(void) {
  s32 t2;

  Func_020009b0();
  Event_ResolvePendingActions(0);
  t2 = GameFlag_Test(0x301);
  if (t2 != 0) {
    Event_SetMessageId((s32)MsgYallamInteriors39);
  } else {
    Event_SetMessageId((s32)MsgYallamInteriors35);
  }
  Event_ShowMessageAndWait(16, 0);
  Func_020009b8();
  return 0;
}

#include "event_inline.h"

s32 Func_0200014c(void) {
  s32 t2;

  Func_020009b0();
  Event_ResolvePendingActions(0);
  t2 = GameFlag_Test(0x301);
  if (t2 != 0) {
    Event_SetMessageId((s32)MsgYallamInteriors36);
  } else {
    Event_SetMessageId((s32)MsgYallamInteriors32);
  }
  Event_ShowMessageAndWait(16, 0);
  Func_020009b8();
  return 0;
}

#include "event_inline.h"

s32 Func_0200018c(void) {
  Func_020009b0();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgYallamInteriors22);
  Event_ShowMessageAndWait(10, 0);
  Func_020009b8();
  return 0;
}

#include "event_inline.h"

s32 Func_020001b4(void) {
  Func_020009b0();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgYallamInteriors24);
  Event_ShowMessageAndWait(11, 0);
  Func_020009b8();
  return 0;
}

#include "event_inline.h"

s32 Func_020001dc(void) {
  Func_020009b0();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgYallamInteriors26);
  Event_ShowMessageAndWait(12, 0);
  Func_020009b8();
  return 0;
}

u8 *Func_02000204(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov677Room1)
    return gData_02000ea8;
  if (scene == (s32)&SceneId_Ov677Room2)
    return gData_02000fe0;
  return gData_02000e84;
}

void Func_02000244(void) {
  s32 msg;

  Func_020009b0();
  Event_ResolvePendingActions(0);
  msg = (s32)MsgYallamInteriors10;
  Event_SetMessageId(msg);
  FieldMessage_ShowAtActor(9, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessageAndWait(9, 0);
  Func_020009b8();
}

void Func_0200029c(s32 actor) {
  u16 facing = ((struct YallamInteriorsObject *)Actor_GetById(gMainRoutineWork.currentOwner))->facing;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Func_02000a68(20, actor);
  } else {
    Func_020009b0();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgYallamInteriors21);
    Event_ShowMessageAndWait(actor, 0);
    Func_020009b8();
  }
}

void Func_020002fc(s32 actor) {
  u16 facing = ((struct YallamInteriorsObject *)Actor_GetById(gMainRoutineWork.currentOwner))->facing;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Func_02000a68(21, actor);
  } else {
    Func_020009b0();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgYallamInteriors23);
    Event_ShowMessageAndWait(actor, 0);
    Func_020009b8();
  }
}

void Func_0200035c(s32 actor) {
  u16 facing = ((struct YallamInteriorsObject *)Actor_GetById(gMainRoutineWork.currentOwner))->facing;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Func_02000a68(22, actor);
  } else {
    Func_020009b0();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgYallamInteriors25);
    Event_ShowMessageAndWait(actor, 0);
    Func_020009b8();
  }
}

void Func_020003bc(s32 actor) {
  u16 facing = ((struct YallamInteriorsObject *)Actor_GetById(gMainRoutineWork.currentOwner))->facing;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Func_02000a78(7, actor);
    GameFlag_ClearBit(0x301);
  } else {
    Func_020009b0();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgYallamInteriors27);
    Event_ShowMessageAndWait(actor, 0);
    Func_020009b8();
  }
}

void Func_02000424(s32 actor) {
  u16 facing = ((struct YallamInteriorsObject *)Actor_GetById(gMainRoutineWork.currentOwner))->facing;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Func_02000a70(actor);
  } else {
    Func_020009b0();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgMikasallaInteriors27);
    Event_ShowMessageAndWait(actor, 0);
    Func_020009b8();
  }
}

void Func_02000480(void) {
  s32 msg;

  Func_020009b0();
  Event_ResolvePendingActions(0);
  Actor_StartRepeatedMotion(14, 2);
  msg = (s32)MsgYallamInteriors31;
  Event_SetMessageId(msg);
  FieldMessage_ShowAtActor(14, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessageAndWait(14, 0);
  Func_020009b8();
}

s32 Func_02000a90(void);
void Func_02000a60(s32 actor, s32 angle);
void Func_02000a80(s32 actor);

s32 Func_020004e0(void) {
  s32 item;

  for (item = 429; item <= 437; item++) {
    if (PartyInventory_FindOwner(item) != -1)
      return 1;
  }
  for (item = 417; item <= 427; item++) {
    if (PartyInventory_FindOwner(item) != -1)
      return 1;
  }
  return 0;
}

/// Actor 14's talk. Flag 0x10f follows Func_020004e0's item check (it stays
/// set while flag 0x301 or Func_02000a90 holds). Once 0x10f is set and
/// neither holds, a yes moves actor 14 and the leader, and if Func_02000a90
/// then holds, places actor 16 and sets flag 0x301.
void Func_02000524(void) {
  s32 message;

  if (Func_020004e0())
    Event_SetFlag(0x10f);
  else if (!Event_GetFlag(0x301) && !Func_02000a90())
    GameFlag_ClearBit(0x10f);

  if (!Event_GetFlag(0x10f)) {
    message = (s32)MsgAhSoYou;
    Event_SetMessageId(message);
    Event_OpenMessage(14, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      FieldEvent_Wait(10);
      Event_SetMessageId(message + 1);
    } else {
      FieldEvent_Wait(20);
      Event_SetMessageId(message + 2);
    }
    Event_ShowMessage(14, 0);
  } else if (Event_GetFlag(0x301)) {
    Event_SetMessageId((s32)MsgICanTWait);
    Event_ShowMessage(14, 0);
  } else if (Func_02000a90()) {
    Event_SetMessageId((s32)MsgMyWifeHas);
    Event_ShowMessage(14, 0);
  } else {
    message = (s32)MsgYouWantMe;
    Event_SetMessageId(message);
    Event_OpenMessage(14, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      Event_SetMessageId(message + 1);
      Event_ShowMessageAndWait(14, 0);
      Func_02000a60(14, 0xc000);
      Actor_SetSingleChildFlags(((struct YallamInteriorsObject *)Actor_GetById(14)), 1);
      Actor_SetDestination(gMainRoutineWork.currentOwner, 488, 408);
      Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
      Actor_ArmCallback(gMainRoutineWork.currentOwner, 0, 0);
      Actor_SetDestination(14, 504, 408);
      Actor_WaitForMoveById(14);
      Actor_SetAngleToward(14, gMainRoutineWork.currentOwner, 0);
      Func_02000a80(14);
      if (Func_02000a90()) {
        Actor_SetSpeed(14, 0x6666, 0x3333);
        Actor_WalkTo(14, 568, 408);
        Actor_WaitForMoveById(14);
        Actor_WalkTo(14, 568, 360);
        Actor_WaitForMoveById(14);
        Actor_WalkTo(16, 632, 346);
        Actor_FaceDirection(16, FACING_SOUTH, 0);
        Actor_SetModeById(16, 0);
        Actor_ResetAndSetPositionInMode2(14, 600, 360);
        Actor_WaitForMoveById(14);
        Actor_SetModeById(14, 0);
        Actor_ArmCallback(14, FACING_SOUTH, 0);
        ((struct YallamInteriorsObject *)Actor_GetById(14))->collisionFlags = 129;
        ((struct YallamInteriorsObject *)Actor_GetById(14))->priorityFlags = 1;
        Actor_SetSpritePriority(14, 1);
        ((struct YallamInteriorsObject *)Actor_GetById(14))->priorityFlags |= 1;
#if TLA_PATCH_1
        Actor_SetHorizontalPositionWithTerrain(17, 0, 0);
#endif
        GameFlag_SetBit(0x301);
      } else {
        Actor_ResetAndSetPosition(14, 491, 398);
        Actor_WaitForMoveById(14);
        Actor_ArmCallback(14, FACING_SOUTH, 0);
        Actor_SetSingleChildFlags(((struct YallamInteriorsObject *)Actor_GetById(14)), 0);
        Func_02000a60(14, 0);
      }
    } else {
      Event_SetMessageId(message + 2);
      Event_ShowMessageAndWait(14, 0);
    }
  }
}

void Party_AdjustCoins(s32 amount);
void Func_02000980(s32 value, s32 slot);
u16 *Func_02000988(s32 item);
s32 Func_020009a0(s32 item);
void Func_02000a40(s32 item, s32 mode);
void Func_02000a88(s32 value);

void Func_0200077c(s32 actor) {
  if (!Event_GetFlag(0x10f)) {
    Event_SetMessageId((s32)MsgThisThisIs);
    Event_ShowMessageAndWait(actor, 0);
  } else if (Event_GetFlag(0x301)) {
    Event_SetMessageId((s32)MsgItTakesOur);
    Event_ShowMessageAndWait(actor, 0);
  } else if (Func_02000a90()) {
    s32 item = Func_02000a90();
    u32 price = *Func_02000988(item);
    s32 message;

    Func_02000980(item, 2);
    Func_02000980(price, 5);
    message = (s32)MsgHereSThe;
    Event_SetMessageId(message);
    FieldMessage_ShowAtActor(actor, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) != 0) {
      Event_SetMessageId(message + 1);
      Event_ShowMessageAndWait(actor, 0);
    } else if (Func_020009a0(item) < 0) {
      Event_SetMessageId(message + 2);
      Event_ShowMessageAndWait(actor, 0);
    } else if ((u32)gMainRoutineWork.coins < price) {
      Event_SetMessageId(message + 3);
      Event_ShowMessageAndWait(actor, 0);
    } else {
      Event_SetMessageId(message + 4);
      Event_ShowMessageAndWait(actor, 0);
      Func_02000a40(item, 3);
      PartyInventory_GiveItem(item, 0);
      Func_02000a88(0);
      Party_AdjustCoins(-price);
    }
  } else {
    Event_SetMessageId((s32)MsgManSunshineWould);
    Event_ShowMessageAndWait(actor, 0);
  }
}

/// The scene's start entry: the screen effect 0x209, then the actors of
/// rooms 1 and 2.
s32 Func_0200086c(void) {
  s32 scene;

  ((struct YallamInteriorsEventWork *)Ram_HeapSlots->eventWork)->screenEffect =
      0x209;
  {
    struct MainRoutineWork *party = &gMainRoutineWork;

    scene = party->scene;
  }
  if (scene == (s32)&SceneId_Ov677Room1) {
    if (Event_GetFlag(0x301)) {
      Actor_SetPosition(14, PIXELS(600), PIXELS(360));
      Actor_SetPosition(16, PIXELS(632), PIXELS(346));
      Actor_FaceDirection(16, FACING_SOUTH, 0);
#if TLA_PATCH_1
      Actor_SetHorizontalPositionWithTerrain(17, 0, 0);
#endif
    } else {
      Func_02000a60(14, 0);
    }
    Actor_SetSingleChildFlags(
        ((struct YallamInteriorsObject *)Actor_GetById(17)), 0);
    Actor_SetSpritePriority(11, 2);
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov677Room2) {
    Func_02000a48(0);
    Actor_SetSpritePriority(12, 2);
    ((struct YallamInteriorsObject *)Actor_GetById(12))->collisionFlags |= 4;
    Actor_SetSpritePriority(10, 2);
    ((struct YallamInteriorsObject *)Actor_GetById(10))->collisionFlags |= 4;
    Actor_SetSpritePriority(11, 2);
    ((struct YallamInteriorsObject *)Actor_GetById(11))->collisionFlags |= 4;
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }
