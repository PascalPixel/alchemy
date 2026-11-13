/// @file
/// The scene code of overlay 0x652 that its scene tables belong to, in ROM order; the rest of the overlay is in daila_interiors/.

// ---- games/tla/src/scene/daila_interiors.c ----

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
#include "scene_ids.h"
#include "field_event.h"
#include "main_routine_work.h"
#include "scene_call.h"
#include "text/msg_ids.h"
#include "daila_interiors/daila.h"
#include "ram_buffer.h"
#include "actor.h"
#include "event_work.h"
#include "inventory.h"
#include "module/game/game.h"
#include "module/scene/daila_interiors.h"
#include "module/scene/scene.h"
#include "module/tla.h"
#include "module/window/window.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

/* VIEW(daila_interiors): variable gData_0200116c as extern const u8 gData_0200116c[]; the definition has s32 gData_0200116c[] = { 11, 0x1010100a, - 1, 0x1020200a, - 1, 0x1030300a, - 1, 0x1040400a, - 1, 0x1050500a, - 1, 0x1060600a, - 1, 0x1070308...; tried as the definition's extern s16 gData_0200116c[], it does not compile in tla-ja: games/tla/src/scene/daila_interiors.c:709: conflicting types for `gData_0200116c'. */
extern const u8 gData_0200116c__daila_interiors[] __asm__("gData_0200116c");

const void *Scene_GetExits(void) {
  if (GameFlag_Test(0x873))
    return gData_020011b8;
  return gData_0200116c__daila_interiors;
}

const struct ActorPlacement *Scene_GetPlacements(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_DailaInteriors2)
    return (const struct ActorPlacement *)gData_020013fc;
  if (GameFlag_Test(0x873))
    return (const struct ActorPlacement *)gData_020014a4;
  return (const struct ActorPlacement *)gData_02001204;
}

void DailaInteriors_EmptyHook(void) {}

void Func_020000ac(s32 unused, s32 actor) {
  Daila_Jump(actor, 0x100, 50);
  Actor_Jump(actor, 6, 15);
  Actor_Jump(actor, 6, 15);
  FieldEvent_Wait(10);
  if (!Daila_FlagCheck(0x200)) {
    Import_EventSetMessageId((s32)MsgDailaInteriorsGah);
    Import_EventShowMessageAndWait(actor, 0);
    Daila_ObjWalk(actor, 360, 72);
    Actor_ArmCallback(actor, FACING_SOUTH, 0);
    GameFlag_SetBit(0x200);
  } else {
    Import_EventSetMessageId((s32)MsgDailaInteriorsYarg);
    Import_EventShowMessageAndWait(actor, 0);
    Daila_ObjWalk(actor, 392, 104);
    Actor_ArmCallback(actor, FACING_WEST, 0);
    GameFlag_ClearBit(0x200);
  }
  FieldEvent_Wait(20);
}

void Func_02000150(s32 unused, s32 actor) {
  Actor_SetAngleToward(actor, 4, 0);
  FieldEvent_Wait(10);
  Func_02000d94(actor, 0x103, 50);
  Actor_Jump(actor, 6, 15);
  Actor_Jump(actor, 6, 15);
  FieldEvent_Wait(10);
  Import_EventSetMessageId((s32)MsgDailaInteriorsWhatAreYouDoing);
  Import_EventShowMessageAndWait(actor, 0);
  FieldEvent_Wait(10);
}

s32 DailaInteriors_TalkScamps(void) {
  Import_EventStart();
  Import_EventResolvePendingActions(0);
  Import_EventSetMessageId((s32)MsgDailaInteriorsScamps);
  Import_EventShowMessageAndWait(19, 0);
  Import_EventEnd();
  return 0;
}

s32 DailaInteriors_TalkMazeTreasure(void) {
  Import_EventStart();
  Import_EventResolvePendingActions(0);
  Import_EventSetMessageId((s32)MsgDailaInteriorsMazeTreasure);
  Import_EventShowMessageAndWait(19, 0);
  Import_EventEnd();
  return 0;
}

void DailaInteriors_StartLeaderEvent(void) {
  Import_EventStart();
  Import_EventResolvePendingActions(0);
  Import_ActorSetSpritePriority(gMainRoutineWork.currentOwner, 1);
}

/* VIEW(daila_interiors): variable gData_020016e4 as extern const u8 gData_020016e4[]; the definition has s32 gData_020016e4[] = { 1, (s32) 0xffff0001, 1, 1, (s32) 0xffff0002, 2, 1, (s32) 0xffff0003, 3, 1, (s32) 0xffff0004, 4, 1, (s32) 0xffff0005...; tried as the definition's extern s16 gData_020016e4[], it does not compile in tla-ja: games/tla/src/scene/daila_interiors.c:1010: conflicting types for `gData_020016e4'. */
extern const u8 gData_020016e4__daila_interiors[] __asm__("gData_020016e4");

const struct Trigger *Scene_GetEvents(void) {
  s32 flag = GameFlag_Test(0x873);
  struct MainRoutineWork *party = &gMainRoutineWork;
  s32 id = (s32)&SceneId_DailaInteriors2;

  if (flag) {
    if (party->scene == id)
      return (const struct Trigger *)gData_02001c18;
    return (const struct Trigger *)gData_02001990;
  }
  if (party->scene == id)
    return (const struct Trigger *)gData_02001924;
  return (const struct Trigger *)gData_020016e4__daila_interiors;
}

struct DailaActor *Actor_GetById(s32 id);

/* VIEW(daila_interiors): routine Func_02000dec as void Func_02000dec(s32 slot, s32 actor); module/tla.h has void *Func_02000dec(s32, s32, s32, s32). */
extern void Func_02000dec__daila_interiors(s32 slot, s32 actor) __asm__("Func_02000dec");

void Func_02000270(s32 actor) {
  u16 facing = Actor_GetById(gMainRoutineWork.currentOwner)->facing;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Func_02000ddc(1, actor);
  } else {
    Import_EventStart();
    Import_EventResolvePendingActions(0);
    Import_EventSetMessageId((s32)MsgDailaInteriorsSouthRoadDanger);
    Import_EventShowMessageAndWait(actor, 0);
    Import_EventEnd();
  }
}

void Func_020002d0(s32 actor) {
  u16 facing = Actor_GetById(gMainRoutineWork.currentOwner)->facing;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Func_02000ddc(2, actor);
  } else {
    Import_EventStart();
    Import_EventResolvePendingActions(0);
    Import_EventSetMessageId((s32)MsgDailaInteriorsNewTravelers);
    Import_EventShowMessageAndWait(actor, 0);
    Import_EventEnd();
  }
}

void Func_02000330(s32 actor) {
  u16 facing = Actor_GetById(gMainRoutineWork.currentOwner)->facing;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0x8000) {
    Func_02000dec__daila_interiors(0, actor);
  } else {
    Import_EventStart();
    Import_EventResolvePendingActions(0);
    Import_EventSetMessageId((s32)MsgDailaInteriorsDinnerChoice);
    Import_EventShowMessageAndWait(actor, 0);
    Import_EventEnd();
  }
}

void Func_02000390(s32 actor) {
  s32 msg;
  u16 facing = Actor_GetById(gMainRoutineWork.currentOwner)->facing;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    if (GameFlag_Test(0x847) == 0) {
      Import_EventStart();
      Import_EventResolvePendingActions(0);
      msg = (s32)MsgDailaInteriorsTravelersFreeTime;
      Import_EventSetMessageId(msg);
      FieldMessage_ShowAtActor(actor, 0);
      if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
        FieldEvent_Wait(10);
        Import_EventSetMessageId(msg + 1);
      } else {
        FieldEvent_Wait(20);
        Import_EventSetMessageId(msg + 2);
      }
      Import_EventShowMessageAndWait(actor, 0);
      Import_EventEnd();
      GameFlag_SetBit(0x847);
    } else {
      Func_02000de4(actor);
    }
  } else {
    Import_EventStart();
    Import_EventResolvePendingActions(0);
    msg = (s32)MsgDailaInteriorsTravelersFreeTime;
    Import_EventSetMessageId(msg);
    FieldMessage_ShowAtActor(actor, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      FieldEvent_Wait(10);
      Import_EventSetMessageId(msg + 1);
    } else {
      FieldEvent_Wait(20);
      Import_EventSetMessageId(msg + 2);
    }
    Import_EventShowMessageAndWait(actor, 0);
    Import_EventEnd();
    GameFlag_SetBit(0x847);
  }
}

void Func_02000498(s32 actor) {
  u16 facing = Actor_GetById(gMainRoutineWork.currentOwner)->facing;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Func_02000de4(actor);
  } else {
    Import_EventStart();
    Import_EventResolvePendingActions(0);
    Import_EventSetMessageId((s32)MsgDailaInteriorsLeaveForMadra);
    Import_EventShowMessageAndWait(actor, 0);
    Import_EventEnd();
  }
}

void Func_020004f4(s32 actor) {
  u16 facing = Actor_GetById(gMainRoutineWork.currentOwner)->facing;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Func_02000ddc(1, actor);
  } else {
    Import_EventStart();
    Import_EventResolvePendingActions(0);
    Import_EventSetMessageId((s32)MsgDailaInteriorsPirateTreasure);
    Import_EventShowMessageAndWait(actor, 0);
    Import_EventEnd();
  }
}

void Func_02000554(s32 actor) {
  u16 facing = Actor_GetById(gMainRoutineWork.currentOwner)->facing;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Func_02000ddc(2, actor);
  } else {
    Import_EventStart();
    Import_EventResolvePendingActions(0);
    Import_EventSetMessageId((s32)MsgDailaInteriorsSpaciousVillage);
    Import_EventShowMessageAndWait(actor, 0);
    Import_EventEnd();
  }
}

void Func_020005b4(s32 actor) {
  u16 facing = Actor_GetById(gMainRoutineWork.currentOwner)->facing;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0x8000) {
    Func_02000dec__daila_interiors(0, actor);
  } else {
    Import_EventStart();
    Import_EventResolvePendingActions(0);
    Import_EventSetMessageId((s32)MsgDailaInteriorsFishermenWaiting);
    Import_EventShowMessageAndWait(actor, 0);
    Import_EventEnd();
  }
}

void Func_02000614(void) {
  s32 msg;

  Import_EventStart();
  Import_EventResolvePendingActions(0);
  msg = (s32)MsgDailaInteriorsGoingToMadra;
  Import_EventSetMessageId(msg);
  FieldMessage_ShowAtActor(12, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    FieldEvent_Wait(10);
    Import_EventSetMessageId(msg + 1);
  } else {
    FieldEvent_Wait(20);
    Import_EventSetMessageId(msg + 2);
  }
  Import_EventShowMessageAndWait(12, 0);
  Import_EventEnd();
}

void Func_0200066c(void) {
  s32 msg;

  Import_EventStart();
  Import_EventResolvePendingActions(0);
  msg = (s32)MsgDailaInteriorsLookingForBoat;
  Import_EventSetMessageId(msg);
  FieldMessage_ShowAtActor(14, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    FieldEvent_Wait(10);
    Import_EventSetMessageId(msg + 1);
  } else {
    FieldEvent_Wait(20);
    Import_EventSetMessageId(msg + 2);
  }
  Import_EventShowMessageAndWait(14, 0);
  Import_EventEnd();
}

void Func_020006c4(void) {
  Import_EventStart();
  Import_EventResolvePendingActions(0);
  Import_EventSetMessageId((s32)MsgDailaInteriorsWatchingPractice);
  Import_EventShowMessageAndWait(26, 0);
  FieldEvent_Wait(30);
  Actor_SetModeById(26, 3);
  FieldEvent_Wait(30);
  Import_EventShowMessageAndWait(26, 0);
  Import_EventEnd();
}

void Func_02000704(void) {
  u8 *event = Ram_HeapSlots->eventWork;
  struct Actor *object;
  s32 flags;

  Func_02000dcc(gData_02000e04);
  object = *(struct Actor **)(event + 0xcbc);
  flags = 1;
  flags |= object->priorityFlags;
  flags &= 0xfd;
  object->priorityFlags = flags;
}

// ---- games/tla/src/scene/daila_interiors/item.c ----
// Scene code of overlay 0x652, in ROM order, from item's offset up to the next object the overlay links.

#include "item.h"

/* VIEW(item): tag struct DailaActor as struct DailaActor__item; daila_interiors.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/scene/daila_interiors.c:373: structure has no member named `unk48'. */
struct DailaActor__item {
  u8 unk00[0xc];
  s32 y;
  u8 unk10[8];
  s32 speed18;
  s32 speed1c;
  u8 unk20[8];
  s32 unk28;
  u8 unk2c[0x1c];
  s32 unk48;
  u8 unk4c[9];
  u8 kind55;
  u8 unk56[3];
  u8 flags59;
  u8 unk5a[0x12];
  s32 unk6c;
};

/* VIEW(item): routine Actor_GetById as struct DailaActor__item *Actor_GetById(s32 id); daila_interiors has struct DailaActor *Actor_GetById(s32 id); tried as the definition's struct Actor *Actor_GetById(u32 objectId), which the object already declares, it does not compile in tla-ja: games/tla/src/scene/daila_interiors.c:389: structure has no member named `unk48'. */
extern struct DailaActor__item *Actor_GetById__item(s32 id) __asm__("Actor_GetById");
void Func_02000da4(struct DailaActor__item *object, s32 mode);
void Sound_Play(s32 cue);
/* VIEW(daila_interiors): routine MessageArgument_Push as void MessageArgument_Push(s32 value, s32 slot); module/window/window.h has void MessageArgument_Push(u32 value, u32 kind). */
extern void MessageArgument_Push__daila_interiors(s32 value, s32 slot) __asm__("MessageArgument_Push");
void Func_02000c9c(struct DailaActor__item *object);

void Func_02000730(void) {
  s32 saved48;
  s32 saved28;
  s32 savedY;
  s32 item;

  saved48 = Actor_GetById__item(9)->unk48;
  item = 458;
  saved28 = Actor_GetById__item(9)->unk28;
  savedY = Actor_GetById__item(9)->y;
  Import_EventStart();
  Import_EventResolvePendingActions(0);
  if (PartyInventory_FindRoom(item) >= 0) {
    Func_02000da4(Actor_GetById__item(9), 3);
    Sound_Play(83);
    MessageArgument_Push__daila_interiors(item, 2);
    FieldMessage_Show((s32)MsgItemReceived, 1);
    PartyInventory_Add(item);
    Actor_SetHorizontalPositionWithTerrain(9, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(13, 232 << 16, 194 << 16);
    GameFlag_SetBit(0xa20);
  } else {
    Func_02000da4(Actor_GetById__item(9), 3);
    Sound_Play(83);
    if (Func_02000878(item, 0) != -1) {
      Actor_SetHorizontalPositionWithTerrain(9, 0, 0);
      Actor_SetHorizontalPositionWithTerrain(13, 232 << 16, 196 << 16);
      GameFlag_SetBit(0xa20);
    } else {
      Actor_SetHorizontalPositionWithTerrain(9, 0, 0);
      FieldEvent_Wait(1);
      Actor_GetById__item(9)->unk28 = saved28;
      Actor_GetById__item(9)->y = savedY;
      Actor_GetById__item(9)->unk48 = saved48;
      Actor_GetById__item(9)->unk6c = 0;
      Func_02000c9c(Actor_GetById__item(9));
      Import_ActorSetSpritePriority(9, 1);
      Actor_GetById__item(9)->kind55 = 2;
      Actor_GetById__item(9)->speed1c = 0x13333;
      Actor_GetById__item(9)->speed18 = 0x13333;
      Actor_GetById__item(9)->flags59 |= 4;
      Daila_ObjPlace(9, 232 << 16, 196 << 16);
    }
  }
  Import_EventEnd();
}

struct ItemDefinition *Func_02000ce4(s32 item);
void FieldMessage_Show(s32 msg, s32 mode);
void *Unit_Get(s32 owner);
/* VIEW(daila_interiors): routine Inventory_Discard as void Inventory_Discard(s32 member, s32 slot); inventory.h has s32 Inventory_Discard(s32 owner, s32 slot). */
extern void Inventory_Discard__daila_interiors(s32 member, s32 slot) __asm__("Inventory_Discard");

s32 Func_02000878(s32 item, s32 unused) {
  struct DailaEventWork *work;
  s32 saved;
  s32 owner;
  s32 result;
  s32 count;
  s32 i;
  s32 member;
  s32 slot;

  work = Ram_HeapSlots->eventWork;
  saved = work->message;
  owner = PartyInventory_Add(item);
  if (owner == -1) {
    MessageArgument_Push__daila_interiors(item, 2);
    FieldMessage_Show((s32)MsgItemReceived, 1);
    FieldMessage_Show((s32)MsgItemCannotCarry, 1);
  retry:
    FieldMessage_Show((s32)MsgItemDropPrompt, 1);
    result = Func_02000df4(&member, &slot);
    if (result == -1) {
      if (Func_02000ce4(item)->flags & 8) {
        MessageArgument_Push__daila_interiors(item, 2);
        FieldMessage_Show((s32)MsgItemDropPrompt + 4, 1);
        goto retry;
      }
      MessageArgument_Push__daila_interiors(item, 2);
      FieldMessage_Show((s32)MsgItemDropPrompt + 1, 5);
      result = Func_02000dbc(1);
      Message_CloseFinishedWindows();
      if (result != 0)
        goto retry;
      MessageArgument_Push__daila_interiors(item, 2);
      FieldMessage_Show((s32)MsgItemDropPrompt + 2, 1);
      work->message = saved;
      return owner;
    } else {
      Unit_Get(member);
      count = Inventory_GetQuantity(member, slot);
      if (Inventory_CountItem(member, item) > 29) {
        MessageArgument_Push__daila_interiors(member, 1);
        MessageArgument_Push__daila_interiors(item, 2);
        FieldMessage_Show((s32)MsgItemDropPrompt + 7, 1);
        goto retry;
      }
      for (i = 0; i < count; i++)
        Inventory_Discard__daila_interiors(member, slot);
      Unit_RefreshPsynergy(member);
      Unit_Calculate(member);
      owner = PartyInventory_Add(item);
      Sound_Play(83);
      if (owner == gMainRoutineWork.currentOwner) {
        MessageArgument_Push__daila_interiors(item, 2);
        FieldMessage_Show((s32)MsgItemReceived, 3);
      } else {
        MessageArgument_Push__daila_interiors(item, 2);
        MessageArgument_Push__daila_interiors(owner, 1);
        FieldMessage_Show((s32)MsgItemGiven, 3);
      }
      work->message = saved;
      return owner;
    }
  } else {
    Sound_Play(83);
    MessageArgument_Push__daila_interiors(item, 2);
    FieldMessage_Show((s32)MsgItemReceived, 3);
    if (owner != gMainRoutineWork.currentOwner) {
      MessageArgument_Push__daila_interiors(item, 2);
      MessageArgument_Push__daila_interiors(owner, 1);
      FieldMessage_Show((s32)MsgItemReceived + 1, 1);
    }
    work->message = saved;
  }
  return owner;
}

// ---- games/tla/src/scene/daila_interiors/arrival.c ----
#include "event_work.h"

void Func_02000ca4(s32 mode);

/* VIEW(arrival): variable gData_02000e04 as extern s16 gData_02000e04[]; daila_interiors has extern const u8 gData_02000e04[]; tried as the rest of the object declares it, it does not compile in tla-ja: games/tla/src/scene/daila_interiors.c:609: `gData_02000e04__arrival' undeclared (first use in this function). */
extern s16 gData_02000e04__arrival[] __asm__("gData_02000e04");

/* VIEW(arrival): routine Actor_GetById as struct DailaStartActor *Actor_GetById(s32 id); item has struct DailaActor__item *Actor_GetById(s32 id); tried as the definition's struct Actor *Actor_GetById(u32 objectId), which the object already declares, it does not compile in tla-ja: games/tla/src/scene/daila_interiors.c:579: invalid type argument of `->'. */
extern struct DailaStartActor *Actor_GetById__arrival(s32 id) __asm__("Actor_GetById");
/* VIEW(arrival): routine Actor_SetSingleChildFlags as void Actor_SetSingleChildFlags(struct DailaStartActor *object, s32 value); field.h has void Actor_SetSingleChildFlags(struct Actor *object, u32 flags); tried as the rest of the object declares it, DailaInteriors_Start moves: 5 of 174 instructions in tla-ja. */
extern void Actor_SetSingleChildFlags__arrival(struct DailaStartActor *object,
                                        s32 value) __asm__("Actor_SetSingleChildFlags");
void Func_02000cbc(struct DailaStartActor *object, s32 item);
/* VIEW(daila_interiors): routine Func_02000dc4 as void Func_02000dc4(const void *script); module/scene/atteka_inlet.h has void Func_02000dc4(void). */
extern void Func_02000dc4__daila_interiors(const void *script) __asm__("Func_02000dc4");

s32 DailaInteriors_Start(void) {
  struct DailaMapWork *map;
  s32 *effect = &((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect;
  s32 value = 0x209;

  /* FAKEMATCH(barrier): an empty asm after the address and the value, so gMainRoutineWork's address loads just before the store; as one assignment, 2
   * of 174 instructions differ in tla-en. */
  __asm__("");
  *effect = value;
  if (gMainRoutineWork.scene == (s32)&SceneId_DailaInteriors1) {
    if (gMainRoutineWork.entrance == 7) {
      Func_02000c58();
    } else {
      if (GameFlag_Test(0x873)) {
        Actor_SetModeById(24, 5);
        Actor_SetSingleChildFlags__arrival(Actor_GetById__arrival(24), 0);
        Actor_SetModeById(25, 5);
        Actor_SetSingleChildFlags__arrival(Actor_GetById__arrival(25), 0);
      }
      Actor_GetById__arrival(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
      Actor_GetById__arrival(13)->priorityFlags |= 32;
      Actor_GetById__arrival(19)->priorityFlags |= 32;
      Import_ActorSetSpritePriority(19, 2);
      Actor_GetById__arrival(19)->collisionFlags |= 4;
    }
  } else if (gMainRoutineWork.scene == (s32)&SceneId_DailaInteriors2) {
    map = (struct DailaMapWork *)Ram_HeapSlots->mapWork;
    Import_EventStart();
    Import_EventResolvePendingActions(0);
    Actor_SetSingleChildFlags__arrival(Actor_GetById__arrival(13), 0);
    Func_02000dac(0);
    if (!GameFlag_Test(0xa20)) {
      Func_02000cbc(Actor_GetById__arrival(9), 458);
      Actor_SetSingleChildFlags__arrival(Actor_GetById__arrival(9), 0);
      Import_ActorSetSpritePriority(9, 1);
      Actor_GetById__arrival(9)->speed1c = 0x13333;
      Actor_GetById__arrival(9)->speed18 = 0x13333;
      Actor_GetById__arrival(9)->collisionFlags |= 4;
      Actor_GetById__arrival(13)->collisionFlags |= 4;
    } else {
      Actor_GetById__arrival(9)->collisionFlags |= 4;
      Actor_SetSingleChildFlags__arrival(Actor_GetById__arrival(9), 0);
    }
    if (!GameFlag_Test(0x873)) {
      Actor_SetHorizontalPositionWithTerrain(10, 0, 0);
      Actor_SetHorizontalPositionWithTerrain(11, 0, 0);
      Actor_SetHorizontalPositionWithTerrain(12, 0, 0);
      map->mode17 = 1;
    } else {
      Func_02000dc4__daila_interiors(gData_02000e04__arrival);
    }
    Import_EventEnd();
  }
  return 0;
}

s32 Scene_PrepareMap(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_DailaInteriors2 &&
      GameFlag_Test(0x873)) {
    Func_02000ca4(0);
    Daila_CopyCells(29, 33, 10, 6, 10, 12);
  }
  return 0;
}

void Func_02000c58(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;

  work->screenEffect = 0x209;
  GameFlag_ClearBit(0x12f);
}

void Func_02000c74(void) { Func_02000d9c(); }

#include "version.h"

// clang-format off
s16 gData_02000e04__arrival[] = {
    10, 544, 11, 545, 12, 546, -1, 0,
};

s32 gData_02000e14[] = {
    22, 30,
#if !TLA_PATCH_1
    131,
#else
    129,
#endif
    22, 16, 0x1999, 22, 15, 0x4ccc,
    (s32)0x80010000, 39, 2, 4, 0x1380000, 0,
    0xa80000, 1, 0, 20, 39, 1,
    22, 3, 0, 0, 20, 39,
    2, 4, 0x1880000, 0, 0xa80000, 1,
    0, 20, 39, 1, 22, 3,
    0, 0, 20, 13, 0xffff, (s32)0xc0010000,
    17,
};

s32 gData_02000ecc[] = {
    22, 30, 129, 22, 16, 0x1999,
    22, 15, 0x4ccc, (s32)0x80010000, 39, 1,
    22, 3, 0x6000, 0, 5, 22,
    3, 0x8000, 0, 5, 22, 3,
    0xa000, 0, 5, 22, 3, 0xc000,
    0, 5, 22, 3, 0, 0,
    5, 22, 3, 0x2000, 0, 5,
    22, 3, 0x4000, 0, 5, 22,
    3, 0x6000, 0, 5, 22, 3,
    0x8000, 0, 5, 22, 3, 0xa000,
    0, 5, 22, 3, 0xc000, 0,
    5, 22, 3, 0, 0, 5,
    22, 3, 0x2000, 0, 5, 22,
    3, 0x4000, 0, 5, 22, 3,
    0x6000, 0, 5, 22, 3, 0x8000,
    0, 5, 22, 3, 0xa000, 0,
    5, 22, 3, 0xc000, 0, 5,
    22, 3, 0, 0, 5, 22,
    3, 0x2000, 0, 5, 22, 3,
    0x4000, 0, 5, 22, 6, 0x80000,
    0, 30, 22, 6, 0x80000, 0,
    30, 13, 0xffff, (s32)0xc0010000, 17, 22,
    30,
#if !TLA_PATCH_1
    131,
#else
    129,
#endif
    22, 16, 0x1999, 22, 15, 0x4ccc,
    (s32)0x80010000, 39, 1, 39, 2, 10,
    0x20000, 0x20000, 0x60000, 1, 0, 4,
    13, 0xffff, (s32)0xc0010000, 17,
};
// clang-format on

void Func_0200066c(void);

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

// clang-format off
s32 gData_0200116c[] = {
    11, 0x1010100a, -1, 0x1020200a, -1, 0x1030300a,
    -1, 0x1040400a, -1, 0x1050500a, -1, 0x1060600a,
    -1, 0x10703085, -1, 12, 0x1010700a, -1,
    511,
};

s32 gData_020011b8[] = {
    11, 0x1010100d, -1, 0x1020200d, -1, 0x1030300d,
    -1, 0x1040400d, -1, 0x1050500d, -1, 0x1060600d,
    -1, 0x10703085, -1, 12, 0x1010700d, -1,
    511,
};

s32 gData_02001204[] = {
    (s32)0xffff0073,
    1,
    0x980000,
    0,
    0x680000,
    0x8000,
    (s32)0xffff0074,
    2,
    0x580000,
    0,
    0x580000,
    0,
    (s32)0xffff0072,
    (s32)gData_02000e14,
    0x1680000,
    0,
    0xa80000,
    0x18000,
    (s32)0xffff0080,
    1,
    0x1380000,
    0,
    0x480000,
    0x12000,
    (s32)0xffff0070,
    2,
    0x2780000,
    0,
    0xa80000,
    0,
    (s32)0xffff0071,
    2,
    0x2380000,
    0,
    0x580000,
    0x4000,
    (s32)0xffff0072,
    2,
    0xa00000,
    0,
    0x1680000,
    0x4000,
    (s32)0xffff0080,
    1,
    0x980000,
    0,
    0x1c80000,
    0x1e000,
    (s32)0xffff0073,
    1,
    0xa80000,
    0,
    0x1b80000,
    0x16000,
    (s32)0xffff0074,
    2,
    0xd80000,
    0,
    0x1d80000,
    0,
    (s32)0xffff006c,
    1,
    0xc80000,
    0,
    0x2c80000,
    0x14000,
    (s32)0xffff006d,
    1,
    0x580000,
    0,
    0x2d40000,
    0x14000,
    (s32)0xffff0086,
    1,
    0x1c80000,
    0,
    0x1d40000,
    0,
    (s32)0xffff0087,
    2,
    0x2180000,
    0,
    0x1c80000,
    0,
    (s32)0xffff00ee,
    1,
    0x1b80000,
    0,
    0x1780000,
    0xa000,
    (s32)0xffff0074,
    1,
    0x1a80000,
    0,
    0x1680000,
    0x12000,
    (s32)0xffff004a,
    3,
    0x1d80000,
    0,
    0x2ea0000,
    0x1c000,
    (s32)0xffff004b,
    1,
    0x2380000,
    0,
    0x2c80000,
    0x14000,
    (s32)0xffff004c,
    1,
    0x1f80000,
    0,
    0x2c80000,
    0x18000,
    (s32)0xffff01e9,
    1,
    0x580000,
    0,
    0x2d80000,
    0x24000,
    0xffff,
    0,
    0,
    0,
    0,
    0,
};

u32 gData_020013fc[] = {
    0xffff00c9, 1, 0xe80000, 0, 0x14d0000, 0x14000,
    0xffff01e9, 1, 0xe80000, 0, 0xc40000, 0x24000,
    0x2230122, 1, 0xb80000, 0, 0xf80000, 0x24000,
    0x2240122, 1, 0xd80000, 0, 0xf80000, 0x24000,
    0x2250122, 1, 0xf80000, 0, 0xe80000, 0x24000,
    0xffff01e9, 1, 0x80000, 0, 0x80000, 0x24000,
    0xffff, 0, 0, 0, 0, 0,
};
// clang-format on

s32 gData_020014a4[] = {
    (s32)0xffff0073,
    1,
    0xb00000,
    0,
    0x780000,
    0xa000,
    (s32)0xffff0074,
    2,
    0x580000,
    0,
    0x580000,
    0,
    (s32)0xffff0072,
    (s32)gData_02000e14,
    0x1680000,
    0,
    0xa80000,
    0x18000,
    (s32)0xffff0080,
    1,
    0x1380000,
    0,
    0x480000,
    0x12000,
    (s32)0xffff0070,
    2,
    0x2780000,
    0,
    0xa80000,
    0,
    (s32)0xffff0071,
    2,
    0x2380000,
    0,
    0x580000,
    0x4000,
    (s32)0xffff0072,
    2,
    0xa00000,
    0,
    0x1680000,
    0x4000,
    (s32)0xffff0080,
    1,
    0x580000,
    0,
    0x1b80000,
    0x1e000,
    (s32)0xffff0073,
    1,
    0x980000,
    0,
    0x1e80000,
    0x16000,
    (s32)0xffff0074,
    2,
    0xd80000,
    0,
    0x1d80000,
    0,
    (s32)0xffff006c,
    1,
    0xc80000,
    0,
    0x2c80000,
    0x14000,
    (s32)0xffff006d,
    1,
    0x580000,
    0,
    0x2d40000,
    0x14000,
    (s32)0xffff0086,
    1,
    0x1c80000,
    0,
    0x1d40000,
    0,
    (s32)0xffff0087,
    (s32)gData_02000ecc,
    0x1a80000,
    0,
    0x2080000,
    0x4000,
    (s32)0xffff00ee,
    1,
    0x1b80000,
    0,
    0x1780000,
    0xa000,
    (s32)0xffff0074,
    1,
    0x1a80000,
    0,
    0x1680000,
    0x12000,
    (s32)0xffff0012,
    1,
    0xa10000,
    0,
    0x6f0000,
    0x14000,
    (s32)0xffff0011,
    1,
    0x1480000,
    0,
    0x700000,
    0x14000,
    (s32)0xffff0074,
    (s32)gData_02000ecc,
    0x1c80000,
    0,
    0x2080000,
    0x14000,
    (s32)0xffff004a,
    3,
    0x1d80000,
    0,
    0x2ea0000,
    0x1c000,
    (s32)0xffff004b,
    1,
    0x2380000,
    0,
    0x2c80000,
    0x14000,
    (s32)0xffff004c,
    1,
    0x1f80000,
    0,
    0x2c80000,
    0x18000,
    (s32)0xffff01e9,
    1,
    0x580000,
    0,
    0x2d80000,
    0x24000,
    0xffff,
    0,
    0,
    0,
    0,
    0,
};

s32 gData_020016e4[] = {
    1,
    (s32)0xffff0001,
    1,
    1,
    (s32)0xffff0002,
    2,
    1,
    (s32)0xffff0003,
    3,
    1,
    (s32)0xffff0004,
    4,
    1,
    (s32)0xffff0005,
    5,
    1,
    (s32)0xffff0006,
    6,
    1,
    (s32)0xffff0007,
    7,
    0,
    (s32)0xffff0008,
    MSGNO(MsgFieldRikisReallyInForItWhen),
    0,
    (s32)0xffff0009,
    MSGNO(MsgFieldThatNoGoodKidOfMine),
    0,
    (s32)0xffff000a,
    MSGNO(MsgFieldIWonderWhereOurLittleTavi),
    0,
    (s32)0xffff000b,
    MSGNO(MsgFieldTaviNeverMissesThreeOclockSnacksies),
    0,
    (s32)0xffff000c,
    MSGNO(MsgFieldIfYoureLookingForABoat),
    0,
    (s32)0xffff000d,
    MSGNO(MsgFieldIfYouHeadSouthYoullFind),
    0,
    (s32)0xffff000e,
    MSGNO(MsgFieldInAllMyYearsAsMayor),
    0,
    (s32)0xffff000f,
    MSGNO(MsgFieldTheWaveWasAwfulButThe),
    0,
    (s32)0xffff0010,
    MSGNO(MsgFieldALotOfFolkSaidThey),
    0,
    (s32)0xffff0011,
    MSGNO(MsgFieldBetweenThePiratesAndTheTidal),
    0,
    (s32)0xffff0012,
    (s32)Func_02000270,
    0,
    (s32)0xffff0013,
    (s32)Func_020002d0,
    0,
    (s32)0xffff0014,
    (s32)Func_02000330,
    0,
    (s32)0xffff0015,
    MSGNO(MsgFieldWeRunAVerySmallInn),
    0,
    (s32)0xffff0016,
    MSGNO(MsgFieldIDontKnowWhenIllBe),
    0,
    (s32)0xffff0017,
    MSGNO(MsgFieldIHearTheSeafoodInMadra),
    0,
    (s32)0xffff0018,
    MSGNO(MsgSeaOfTimeCurrents),
    0,
    (s32)0xffff0019,
    MSGNO(MsgSeaOfTimeRedRocks),
    0,
    (s32)0xffff001a,
    MSGNO(MsgSeaOfTimeThreeGenerations),
    0,
    (s32)0xffff001b,
    (s32)Func_020002d0,
    0x8d15,
    (s32)0xffff0008,
    MSGNO(MsgFieldRikiTaviWhereAreYouWhat),
    0x8d15,
    (s32)0xffff0009,
    MSGNO(MsgFieldThatTavisABadInfluenceOn),
    0x8d15,
    (s32)0xffff000a,
    MSGNO(MsgFieldIllBetThatWhereverTaviIs),
    0x8d15,
    (s32)0xffff000b,
    MSGNO(MsgFieldIfTaviIsntHomeSoonWere),
    0x8d15,
    (s32)0xffff000c,
    MSGNO(MsgFieldIWonderHowTheRoadTo),
    0x8d15,
    (s32)0xffff000d,
    MSGNO(MsgFieldIWonderIfTheHolyMan),
    0x8d15,
    (s32)0xffff000e,
    MSGNO(MsgFieldAtLeastNobodySeemsToHave),
    0x8d15,
    (s32)0xffff000f,
    MSGNO(MsgFieldIStillDontKnowWhatThat),
    0x8d15,
    (s32)0xffff0010,
    MSGNO(MsgFieldWeShouldFindADifferentWay),
    0x8d15,
    (s32)0xffff0011,
    MSGNO(MsgFieldNowThatBriggsIsLockedAway),
    0x8d15,
    (s32)0xffff0012,
    MSGNO(MsgFieldImGladImNotGoingTo),
    0x8d15,
    (s32)0xffff0013,
    MSGNO(MsgDailaInteriorsScamps),
    0x8d15,
    (s32)0xffff0014,
    MSGNO(MsgFieldWeCantSailAndWeCant),
    0x8d15,
    (s32)0xffff0015,
    MSGNO(MsgFieldLookAtAllTheseGuestsWe),
    0x8d15,
    (s32)0xffff0016,
    MSGNO(MsgFieldAfterAllThatShakingIWouldnt),
    0x8d15,
    (s32)0xffff0017,
    MSGNO(MsgFieldImSoTornDoILike),
    0x8d15,
    (s32)0xffff0018,
    MSGNO(MsgSeaOfTimeHusbandDizzy),
    0x8d15,
    (s32)0xffff0019,
    MSGNO(MsgSeaOfTimeYeppToldMe),
    0x8d15,
    (s32)0xffff001a,
    MSGNO(MsgSeaOfTimeGiveUp),
    0x8d15,
    (s32)0xffff001b,
    (s32)DailaInteriors_TalkScamps,
    -1,
    0,
    0,
};

s32 gData_02001924[] = {
    1,
    (s32)0xffff0001,
    1,
    1,
    (s32)0xffff0002,
    2,
    1,
    (s32)0xffff0003,
    3,
    1,
    (s32)0xffff0004,
    4,
    1,
    (s32)0xffff0005,
    5,
    1,
    (s32)0xffff0006,
    6,
    0,
    (s32)0xffff0008,
    (s32)Func_02000390,
    0x8d15,
    (s32)0xffff0008,
    MSGNO(MsgFieldTravelersWithNowhereToGoThis),
    -1,
    0,
    0,
};

s32 gData_02001990[] = {
    1,
    (s32)0xffff0001,
    1,
    1,
    (s32)0xffff0002,
    2,
    1,
    (s32)0xffff0003,
    3,
    1,
    (s32)0xffff0004,
    4,
    1,
    (s32)0xffff0005,
    5,
    1,
    (s32)0xffff0006,
    6,
    1,
    (s32)0xffff0007,
    7,
    0,
    (s32)0xffff0008,
    MSGNO(MsgFieldLetMeTellYouKidsAre),
    0,
    (s32)0xffff0009,
    MSGNO(MsgFieldWhoStaysOutsideUntilHesAbsolutely),
    0,
    (s32)0xffff000a,
    MSGNO(MsgFieldOurLittleOneIsFinallyHome),
    0,
    (s32)0xffff000b,
    MSGNO(MsgFieldTaviWolfedDownHisSnackAnd),
    0,
    (s32)0xffff000c,
    (s32)Func_02000614,
    0,
    (s32)0xffff000d,
    MSGNO(MsgFieldIThinkMadraIsEastOf),
    0,
    (s32)0xffff000e,
    (s32)Func_0200066c,
    0,
    (s32)0xffff000f,
    MSGNO(MsgFieldItsTooDangerousToGoTo),
    0,
    (s32)0xffff0010,
    MSGNO(MsgFieldWhoWasThatGuyIWonder),
    0,
    (s32)0xffff0011,
    MSGNO(MsgFieldWhyDidThePirateBriggsPlague),
    0,
    (s32)0xffff0012,
    (s32)Func_020004f4,
    0,
    (s32)0xffff0013,
    (s32)Func_02000554,
    0,
    (s32)0xffff0014,
    (s32)Func_020005b4,
    0,
    (s32)0xffff0015,
    MSGNO(MsgDailaInteriorsDinnerShowPractice),
    0,
    (s32)0xffff0016,
    MSGNO(MsgDailaInteriorsSeafoodWhen),
    0,
    (s32)0xffff0017,
    MSGNO(MsgDailaInteriorsVegetablesAgain),
    0,
    (s32)0xffff0018,
    MSGNO(MsgFieldMmmMmnphDadWasSoMad),
    0,
    (s32)0xffff0019,
    MSGNO(MsgFieldMmmphMmmmDontTellAnyoneWe),
    0,
    (s32)0xffff001a,
    (s32)Func_020006c4,
    0,
    (s32)0xffff001b,
    MSGNO(MsgSeaOfTimeCurrents),
    0,
    (s32)0xffff001c,
    MSGNO(MsgSeaOfTimeRedRocks),
    0,
    (s32)0xffff001d,
    MSGNO(MsgSeaOfTimeThreeGenerations),
    0,
    (s32)0xffff001e,
    (s32)Func_020002d0,
    0x8d15,
    (s32)0xffff0008,
    MSGNO(MsgFieldDarnThatRikiAhImGlad),
    0x8d15,
    (s32)0xffff0009,
    MSGNO(MsgFieldHeeHeeMyRikiIsMommys),
    0x8d15,
    (s32)0xffff000a,
    MSGNO(MsgFieldIGuessRikiNextDoorMade),
    0x8d15,
    (s32)0xffff000b,
    MSGNO(MsgFieldOnceTaviFinishesEatingImPutting),
    0x8d15,
    (s32)0xffff000c,
    MSGNO(MsgFieldDailaGotHitPrettyHardI),
    0x8d15,
    (s32)0xffff000d,
    MSGNO(MsgFieldSomeoneShouldSeeHowTheRoad),
    0x8d15,
    (s32)0xffff000e,
    MSGNO(MsgFieldHeSaidHedPayMeA),
    0x8d15,
    (s32)0xffff000f,
    MSGNO(MsgFieldTheRoadToMadraGoesThrough),
    0x8d15,
    (s32)0xffff0010,
    MSGNO(MsgFieldMaybeThatGuyWashedUpOn),
    0x8d15,
    (s32)0xffff0011,
    MSGNO(MsgFieldBriggsProbablyHasLotsOfMen),
    0x8d15,
    (s32)0xffff0012,
    MSGNO(MsgFieldThisTownMayBeDirtPoor),
    0x8d15,
    (s32)0xffff0013,
    MSGNO(MsgDailaInteriorsMazeTreasure),
    0x8d15,
    (s32)0xffff0014,
    MSGNO(MsgDailaInteriorsTiredPerformer),
    0x8d15,
    (s32)0xffff0015,
    MSGNO(MsgDailaInteriorsDragonKingLine),
    0x8d15,
    (s32)0xffff0016,
    MSGNO(MsgDailaInteriorsBadShows),
    0x8d15,
    (s32)0xffff0017,
    MSGNO(MsgDailaInteriorsGuestDance),
    0x8d15,
    (s32)0xffff0018,
    MSGNO(MsgFieldIStillWantToCatchThat),
    0x8d15,
    (s32)0xffff0019,
    MSGNO(MsgFieldImSureGladIMadeIt),
    0x8d15,
    (s32)0xffff001a,
    MSGNO(MsgDailaInteriorsSeafoodStory),
    0x8d15,
    (s32)0xffff001b,
    MSGNO(MsgSeaOfTimeHusbandDizzy),
    0x8d15,
    (s32)0xffff001c,
    MSGNO(MsgSeaOfTimeYeppToldMe),
    0x8d15,
    (s32)0xffff001d,
    MSGNO(MsgSeaOfTimeGiveUp),
    0x8d15,
    (s32)0xffff001e,
    (s32)DailaInteriors_TalkMazeTreasure,
    -1,
    0,
    0,
};

s32 gData_02001c18[] = {
    1,
    (s32)0xffff0001,
    1,
    1,
    (s32)0xffff0002,
    2,
    1,
    (s32)0xffff0003,
    3,
    1,
    (s32)0xffff0004,
    4,
    1,
    (s32)0xffff0005,
    5,
    1,
    (s32)0xffff0006,
    6,
    0x4602,
    (s32)0xffff001e,
    (s32)Func_02000c74,
    0x602,
    (s32)0xffff001f,
    (s32)Func_02000c74,
    0x8602,
    (s32)0xffff0020,
    (s32)Func_02000c74,
    0xc602,
    (s32)0xffff0021,
    (s32)Func_02000c74,
    0,
    (s32)0xffff0008,
    (s32)Func_02000498,
    0x8d15,
    (s32)0xffff0008,
    MSGNO(MsgDailaInteriorsBoringVillage),
    0x1815,
    0x220000a,
    (s32)Func_02000704,
    0x1815,
    0x221000b,
    (s32)Func_02000704,
    0x1815,
    0x222000c,
    (s32)Func_02000704,
    3,
    0xa200028,
    (s32)Func_02000730,
    -1,
    0,
    0,
};
