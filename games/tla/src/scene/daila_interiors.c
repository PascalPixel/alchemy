/// @file
/// The scene code of overlay 0x652, in ROM order.

// ---- games/tla/src/scene/daila_interiors.c ----

#include "shared/tables.h"
#include "shared/actors.h"
#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "flags.h"
#include "scene_tables.h"
#include "scene_ids.h"
#include "field_event.h"
#include "main_routine_work.h"
#include "scene_call.h"
#include "text/msg_ids.h"
#include "types.h"

void Actor_SetHorizontalPositionWithTerrain(s32 actor, s32 x, s32 z);
void Actor_ShowEmoteById(s32 actor, s32 jump, s32 frames);
void Map_CopyCellsRect(s32 srcX, s32 srcY, s32 destX, s32 destY, s32 width,
                       s32 height);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline s32 Daila_FlagCheck(s32 flag) { return GameFlag_Test(flag); }

static inline void Daila_ObjWalk(s32 actor, s32 x, s32 z) {
  Actor_WalkToAndWaitById(actor, x, z);
}

static inline void Daila_ObjPlace(s32 actor, s32 x, s32 z) {
  Actor_SetHorizontalPositionWithTerrain(actor, x, z);
}

static inline void Daila_Jump(s32 actor, s32 height, s32 frames) {
  s32 h = height;
  s32 f = frames;
  s32 a = actor;

  /* FAKEMATCH(barrier): an empty asm over the three copies finishes height and
   * frames before the actor; as Actor_ShowEmoteById(actor, height, frames), the actor
   * is copied two instructions early and, in Func_020000ac, 2 of 62
   * instructions differ in tla-en. */
  __asm__("" : "+l"(h), "+l"(f), "+l"(a));
  Actor_ShowEmoteById(a, h, f);
}

static inline void Daila_CopyCells(s32 srcX, s32 srcY, s32 destX, s32 destY,
                                   s32 width, s32 height) {
  Map_CopyCellsRect(srcX, srcY, destX, destY, width, height);
}
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

/* VIEW(daila_interiors): variable gData_0200116c as extern const u8
 * gData_0200116c[]; the definition has s32 gData_0200116c[] = { 11, 0x1010100a,
 * - 1, 0x1020200a, - 1, 0x1030300a, - 1, 0x1040400a, - 1, 0x1050500a, - 1,
 * 0x1060600a, - 1, 0x1070308...; tried as the definition's extern s16
 * gData_0200116c[], it does not compile in tla-ja:
 * games/tla/src/scene/daila_interiors.c:654: conflicting types for
 * `gData_0200116c'. */
extern const u8 gData_0200116c__daila_interiors[] __asm__("gData_0200116c");

const void *Scene_GetExits(void) {
  if (GameFlag_Test(FLAG_TAVI_RESCUED))
    return gData_020011b8;
  return gData_0200116c__daila_interiors;
}

const struct ActorPlacement *Scene_GetPlacements(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_DailaInteriors2)
    return gData_020013fc;
  if (GameFlag_Test(FLAG_TAVI_RESCUED))
    return gData_020014a4;
  return gData_02001204;
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
  Actor_FaceActorById(actor, 4, 0);
  FieldEvent_Wait(10);
  Actor_ShowEmoteById(actor, 0x103, 50);
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

/* VIEW(daila_interiors): variable gData_020016e4 as extern const u8
 * gData_020016e4[]; the definition has s32 gData_020016e4[] = { 1, (s32)
 * 0xffff0001, 1, 1, (s32) 0xffff0002, 2, 1, (s32) 0xffff0003, 3, 1, (s32)
 * 0xffff0004, 4, 1, (s32) 0xffff0005...; tried as the definition's extern s16
 * gData_020016e4[], it does not compile in tla-ja:
 * games/tla/src/scene/daila_interiors.c:955: conflicting types for
 * `gData_020016e4'. */
extern const u8 gData_020016e4__daila_interiors[] __asm__("gData_020016e4");

const struct Trigger *Scene_GetEvents(void) {
  s32 flag = GameFlag_Test(FLAG_TAVI_RESCUED);
  struct MainRoutineWork *party = &gMainRoutineWork;
  s32 id = (s32)&SceneId_DailaInteriors2;

  if (flag) {
    if (party->scene == id)
      return gData_02001c18;
    return gData_02001990;
  }
  if (party->scene == id)
    return gData_02001924;
  return (const struct Trigger *)gData_020016e4__daila_interiors;
}

struct DailaActor *Actor_GetById(s32 id);

/* VIEW(daila_interiors): routine Inn_CheckIn as void Inn_CheckIn(s32 slot, s32
 * actor); module/tla.h has void *Inn_CheckIn(s32, s32, s32, s32); tried as the
 * rest of the object declares it, it does not compile in tla-ja:
 * games/tla/src/scene/daila_interiors.c:165: too few arguments to function
 * `Inn_CheckIn'. */
extern void Func_02000dec__daila_interiors(s32 slot,
                                           s32 actor) __asm__("Inn_CheckIn");

void DailaInteriors_TalkToWeaponMerchant(s32 actor) {
  u16 facing = Actor_GetById(gMainRoutineWork.currentOwner)->facing;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Shop_Run(1, actor);
  } else {
    Import_EventStart();
    Import_EventResolvePendingActions(0);
    Import_EventSetMessageId((s32)MsgDailaInteriorsSouthRoadDanger);
    Import_EventShowMessageAndWait(actor, 0);
    Import_EventEnd();
  }
}

void DailaInteriors_TalkToArmorMerchant(s32 actor) {
  u16 facing = Actor_GetById(gMainRoutineWork.currentOwner)->facing;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Shop_Run(2, actor);
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

void DailaInteriors_TalkToHealer(s32 actor) {
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
      Sanctum_Run(actor);
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

void DailaInteriors_TalkToHealerOnceTaviIsRescued(s32 actor) {
  u16 facing = Actor_GetById(gMainRoutineWork.currentOwner)->facing;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Sanctum_Run(actor);
  } else {
    Import_EventStart();
    Import_EventResolvePendingActions(0);
    Import_EventSetMessageId((s32)MsgDailaInteriorsLeaveForMadra);
    Import_EventShowMessageAndWait(actor, 0);
    Import_EventEnd();
  }
}

void DailaInteriors_TalkToWeaponMerchantOnceTaviIsRescued(s32 actor) {
  u16 facing = Actor_GetById(gMainRoutineWork.currentOwner)->facing;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Shop_Run(1, actor);
  } else {
    Import_EventStart();
    Import_EventResolvePendingActions(0);
    Import_EventSetMessageId((s32)MsgDailaInteriorsPirateTreasure);
    Import_EventShowMessageAndWait(actor, 0);
    Import_EventEnd();
  }
}

void DailaInteriors_TalkToArmorMerchantOnceTaviIsRescued(s32 actor) {
  u16 facing = Actor_GetById(gMainRoutineWork.currentOwner)->facing;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Shop_Run(2, actor);
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

DEFINE_SCENE_RAISE_HELD_ACTOR(Scene_RaiseHeldActor, Func_02000dcc,
                              gData_02000e04)

// ---- games/tla/src/scene/daila_interiors/item.c ----
// Scene code of overlay 0x652, in ROM order, from item's offset up to the next
// object the overlay links.

#include "item.h"

/* VIEW(item): tag struct DailaActor as struct DailaActor__item;
 * daila_interiors.c has another definition; tried with the definition the rest
 * of the object has, it does not compile in tla-ja:
 * games/tla/src/scene/daila_interiors.c:357: structure has no member named
 * `unk48'. */
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

/* VIEW(item): routine Actor_GetById as struct DailaActor__item
 * *Actor_GetById(s32 id); daila_interiors has struct DailaActor
 * *Actor_GetById(s32 id); tried as the definition's struct Actor
 * *Actor_GetById(u32 objectId), which the object already declares, it does not
 * compile in tla-ja: games/tla/src/scene/daila_interiors.c:373: structure has
 * no member named `unk48'. */
extern struct DailaActor__item *
Actor_GetById__item(s32 id) __asm__("Actor_GetById");
void ItemDisplay_ShowPicture(struct DailaActor__item *object,
                                         s32 mode);
void Sound_Play(s32 cue);
void Import_ActorResetMotion(struct DailaActor__item *object);

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
    ItemDisplay_ShowPicture(Actor_GetById__item(9), 3);
    Sound_Play(83);
    MessageArgument_Push(item, 2);
    FieldMessage_Show((s32)MsgItemReceived, 1);
    PartyInventory_Add(item);
    Actor_SetHorizontalPositionWithTerrain(9, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(13, 232 << 16, 194 << 16);
    GameFlag_SetBit(0xa20);
  } else {
    ItemDisplay_ShowPicture(Actor_GetById__item(9), 3);
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
      Import_ActorResetMotion(Actor_GetById__item(9));
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

struct ItemDefinition *Import_ItemGet(s32 item);

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
    MessageArgument_Push(item, 2);
    FieldMessage_Show((s32)MsgItemReceived, 1);
    FieldMessage_Show((s32)MsgItemCannotCarry, 1);
  retry:
    FieldMessage_Show((s32)MsgItemDropPrompt, 1);
    result = ItemPicker_RunToDrop(&member, &slot);
    if (result == -1) {
      if (Import_ItemGet(item)->flags & 8) {
        MessageArgument_Push(item, 2);
        FieldMessage_Show((s32)MsgItemDropPrompt + 4, 1);
        goto retry;
      }
      MessageArgument_Push(item, 2);
      FieldMessage_Show((s32)MsgItemDropPrompt + 1, 5);
      result = FieldYesNo_Ask(1);
      Message_CloseFinishedWindows();
      if (result != 0)
        goto retry;
      MessageArgument_Push(item, 2);
      FieldMessage_Show((s32)MsgItemDropPrompt + 2, 1);
      work->message = saved;
      return owner;
    } else {
      Unit_Get(member);
      count = Inventory_GetQuantity(member, slot);
      if (Inventory_CountItem(member, item) > 29) {
        MessageArgument_Push(member, 1);
        MessageArgument_Push(item, 2);
        FieldMessage_Show((s32)MsgItemDropPrompt + 7, 1);
        goto retry;
      }
      for (i = 0; i < count; i++)
        Inventory_Discard(member, slot);
      Unit_RefreshPsynergy(member);
      Unit_Calculate(member);
      owner = PartyInventory_Add(item);
      Sound_Play(83);
      if (owner == gMainRoutineWork.currentOwner) {
        MessageArgument_Push(item, 2);
        FieldMessage_Show((s32)MsgItemReceived, 3);
      } else {
        MessageArgument_Push(item, 2);
        MessageArgument_Push(owner, 1);
        FieldMessage_Show((s32)MsgItemGiven, 3);
      }
      work->message = saved;
      return owner;
    }
  } else {
    Sound_Play(83);
    MessageArgument_Push(item, 2);
    FieldMessage_Show((s32)MsgItemReceived, 3);
    if (owner != gMainRoutineWork.currentOwner) {
      MessageArgument_Push(item, 2);
      MessageArgument_Push(owner, 1);
      FieldMessage_Show((s32)MsgItemReceived + 1, 1);
    }
    work->message = saved;
  }
  return owner;
}

// ---- games/tla/src/scene/daila_interiors/arrival.c ----
#include "event_work.h"

void Map_SetLayerEntryFlag(s32 mode);

/* VIEW(arrival): variable gData_02000e04 as extern s16 gData_02000e04[];
 * daila_interiors has extern const u8 gData_02000e04[]; tried as the rest of
 * the object declares it, it does not compile in tla-ja:
 * games/tla/src/scene/daila_interiors.c:560: `gData_02000e04__arrival'
 * undeclared (first use in this function). */
extern s16 gData_02000e04__arrival[] __asm__("gData_02000e04");

/* VIEW(arrival): routine Actor_GetById as struct DailaStartActor
 * *Actor_GetById(s32 id); item has struct DailaActor__item *Actor_GetById(s32
 * id); tried as the definition's struct Actor *Actor_GetById(u32 objectId),
 * which the object already declares, it does not compile in tla-ja:
 * games/tla/src/scene/daila_interiors.c:530: invalid type argument of `->'. */
extern struct DailaStartActor *
Actor_GetById__arrival(s32 id) __asm__("Actor_GetById");
/* VIEW(arrival): routine Actor_SetSingleChildFlags as void
 * Actor_SetSingleChildFlags(struct DailaStartActor *object, s32 value); field.h
 * has void Actor_SetSingleChildFlags(struct Actor *object, u32 flags); tried as
 * the 35 declarations' void Actor_SetSingleChildFlags(struct FieldEffect
 * *effect, s32 flags), it does not compile in tla-ja:
 * games/tla/src/scene/daila_interiors.c:506: conflicting types for
 * `Actor_SetSingleChildFlags'. */
extern void Actor_SetSingleChildFlags__arrival(
    struct DailaStartActor *object,
    s32 value) __asm__("Actor_SetSingleChildFlags");
void Actor_SetItemIcon(struct DailaStartActor *object, s32 item);
/* VIEW(daila_interiors): routine Func_02000dc4 as void Func_02000dc4(const void
 * *script); module/scene/atteka_inlet.h has void Func_02000dc4(void); tried as
 * the definition's void Func_02000dc4(void), which the object already declares,
 * DailaInteriors_Start moves: 1 of 174 instructions in tla-ja. */
extern void
Func_02000dc4__daila_interiors(const void *script) __asm__("Func_02000dc4");

s32 DailaInteriors_Start(void) {
  struct DailaMapWork *map;
  s32 *effect = &((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect;
  s32 value = 0x209;

  *effect = value;
  if (gMainRoutineWork.scene == (s32)&SceneId_DailaInteriors1) {
    if (gMainRoutineWork.entrance == 7) {
      Scene_FadeInAndClearFlag();
    } else {
      if (GameFlag_Test(FLAG_TAVI_RESCUED)) {
        Actor_SetModeById(24, 5);
        Actor_SetSingleChildFlags__arrival(Actor_GetById__arrival(24), 0);
        Actor_SetModeById(25, 5);
        Actor_SetSingleChildFlags__arrival(Actor_GetById__arrival(25), 0);
      }
      Actor_GetById__arrival(gMainRoutineWork.currentOwner)->priorityFlags |=
          32;
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
      Actor_SetItemIcon(Actor_GetById__arrival(9), 458);
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
    if (!GameFlag_Test(FLAG_TAVI_RESCUED)) {
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
      GameFlag_Test(FLAG_TAVI_RESCUED)) {
    Map_SetLayerEntryFlag(0);
    Daila_CopyCells(29, 33, 10, 6, 10, 12);
  }
  return 0;
}

DEFINE_SCENE_FADE_IN_AND_CLEAR_FLAG(Scene_FadeInAndClearFlag)

void Func_02000c74(void) { Field_TryJumpForward(); }

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

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

// clang-format off
DEFINE_G_DATA_0200116C(gData_0200116c, 0x1010100a, 0x1020200a, 0x1030300a,
    0x1040400a, 0x1050500a, 0x1060600a, 0x1010700a)

DEFINE_G_DATA_0200116C(gData_020011b8, 0x1010100d, 0x1020200d, 0x1030300d,
    0x1040400d, 0x1050500d, 0x1060600d, 0x1010700d)

struct ActorPlacement gData_02001204[] = {
    {115, CONDITION_ALWAYS, 1, PIXELS(152), 0, PIXELS(104), FACING_WEST, 0, 0},
    {116, CONDITION_ALWAYS, 2, PIXELS(88), 0, PIXELS(88), FACING_EAST, 0, 0},
    {114, CONDITION_ALWAYS, (s32)gData_02000e14, PIXELS(360), 0, PIXELS(168), FACING_WEST, 1, 0},
    {128, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(72), FACING_SOUTHEAST, 1, 0},
    {112, CONDITION_ALWAYS, 2, PIXELS(632), 0, PIXELS(168), FACING_EAST, 0, 0},
    {113, CONDITION_ALWAYS, 2, PIXELS(568), 0, PIXELS(88), FACING_SOUTH, 0, 0},
    {114, CONDITION_ALWAYS, 2, PIXELS(160), 0, PIXELS(360), FACING_SOUTH, 0, 0},
    {128, CONDITION_ALWAYS, 1, PIXELS(152), 0, PIXELS(456), FACING_NORTHEAST, 1, 0},
    {115, CONDITION_ALWAYS, 1, PIXELS(168), 0, PIXELS(440), FACING_SOUTHWEST, 1, 0},
    {116, CONDITION_ALWAYS, 2, PIXELS(216), 0, PIXELS(472), FACING_EAST, 0, 0},
    {108, CONDITION_ALWAYS, 1, PIXELS(200), 0, PIXELS(712), FACING_SOUTH, 1, 0},
    {109, CONDITION_ALWAYS, 1, PIXELS(88), 0, PIXELS(724), FACING_SOUTH, 1, 0},
    {134, CONDITION_ALWAYS, 1, PIXELS(456), 0, PIXELS(468), FACING_EAST, 0, 0},
    {135, CONDITION_ALWAYS, 2, PIXELS(536), 0, PIXELS(456), FACING_EAST, 0, 0},
    {238, CONDITION_ALWAYS, 1, PIXELS(440), 0, PIXELS(376), FACING_NORTHWEST, 0, 0},
    {116, CONDITION_ALWAYS, 1, PIXELS(424), 0, PIXELS(360), FACING_SOUTHEAST, 1, 0},
    {74, CONDITION_ALWAYS, 3, PIXELS(472), 0, PIXELS(746), FACING_NORTH, 1, 0},
    {75, CONDITION_ALWAYS, 1, PIXELS(568), 0, PIXELS(712), FACING_SOUTH, 1, 0},
    {76, CONDITION_ALWAYS, 1, PIXELS(504), 0, PIXELS(712), FACING_WEST, 1, 0},
    {489, CONDITION_ALWAYS, 1, PIXELS(88), 0, PIXELS(728), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_020013fc[] = {
    {201, CONDITION_ALWAYS, 1, PIXELS(232), 0, PIXELS(333), FACING_SOUTH, 1, 0},
    {489, CONDITION_ALWAYS, 1, PIXELS(232), 0, PIXELS(196), FACING_SOUTH, 2, 0},
    {290, 0x223, 1, PIXELS(184), 0, PIXELS(248), FACING_SOUTH, 2, 0},
    {290, 0x224, 1, PIXELS(216), 0, PIXELS(248), FACING_SOUTH, 2, 0},
    {290, 0x225, 1, PIXELS(248), 0, PIXELS(232), FACING_SOUTH, 2, 0},
    {489, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_020014a4[] = {
    {115, CONDITION_ALWAYS, 1, PIXELS(176), 0, PIXELS(120), FACING_NORTHWEST, 0, 0},
    {116, CONDITION_ALWAYS, 2, PIXELS(88), 0, PIXELS(88), FACING_EAST, 0, 0},
    {114, CONDITION_ALWAYS, (s32)gData_02000e14, PIXELS(360), 0, PIXELS(168), FACING_WEST, 1, 0},
    {128, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(72), FACING_SOUTHEAST, 1, 0},
    {112, CONDITION_ALWAYS, 2, PIXELS(632), 0, PIXELS(168), FACING_EAST, 0, 0},
    {113, CONDITION_ALWAYS, 2, PIXELS(568), 0, PIXELS(88), FACING_SOUTH, 0, 0},
    {114, CONDITION_ALWAYS, 2, PIXELS(160), 0, PIXELS(360), FACING_SOUTH, 0, 0},
    {128, CONDITION_ALWAYS, 1, PIXELS(88), 0, PIXELS(440), FACING_NORTHEAST, 1, 0},
    {115, CONDITION_ALWAYS, 1, PIXELS(152), 0, PIXELS(488), FACING_SOUTHWEST, 1, 0},
    {116, CONDITION_ALWAYS, 2, PIXELS(216), 0, PIXELS(472), FACING_EAST, 0, 0},
    {108, CONDITION_ALWAYS, 1, PIXELS(200), 0, PIXELS(712), FACING_SOUTH, 1, 0},
    {109, CONDITION_ALWAYS, 1, PIXELS(88), 0, PIXELS(724), FACING_SOUTH, 1, 0},
    {134, CONDITION_ALWAYS, 1, PIXELS(456), 0, PIXELS(468), FACING_EAST, 0, 0},
    {135, CONDITION_ALWAYS, (s32)gData_02000ecc, PIXELS(424), 0, PIXELS(520), FACING_SOUTH, 0, 0},
    {238, CONDITION_ALWAYS, 1, PIXELS(440), 0, PIXELS(376), FACING_NORTHWEST, 0, 0},
    {116, CONDITION_ALWAYS, 1, PIXELS(424), 0, PIXELS(360), FACING_SOUTHEAST, 1, 0},
    {18, CONDITION_ALWAYS, 1, PIXELS(161), 0, PIXELS(111), FACING_SOUTH, 1, 0},
    {17, CONDITION_ALWAYS, 1, PIXELS(328), 0, PIXELS(112), FACING_SOUTH, 1, 0},
    {116, CONDITION_ALWAYS, (s32)gData_02000ecc, PIXELS(456), 0, PIXELS(520), FACING_SOUTH, 1, 0},
    {74, CONDITION_ALWAYS, 3, PIXELS(472), 0, PIXELS(746), FACING_NORTH, 1, 0},
    {75, CONDITION_ALWAYS, 1, PIXELS(568), 0, PIXELS(712), FACING_SOUTH, 1, 0},
    {76, CONDITION_ALWAYS, 1, PIXELS(504), 0, PIXELS(712), FACING_WEST, 1, 0},
    {489, CONDITION_ALWAYS, 1, PIXELS(88), 0, PIXELS(728), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct Trigger gData_020016e4[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgFieldRikisReallyInForItWhen)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgFieldThatNoGoodKidOfMine)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgFieldIWonderWhereOurLittleTavi)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgFieldTaviNeverMissesThreeOclockSnacksies)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgFieldIfYoureLookingForABoat)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgFieldIfYouHeadSouthYoullFind)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgFieldInAllMyYearsAsMayor)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgFieldTheWaveWasAwfulButThe)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgFieldALotOfFolkSaidThey)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgFieldBetweenThePiratesAndTheTidal)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, EVENT(DailaInteriors_TalkToWeaponMerchant)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, EVENT(DailaInteriors_TalkToArmorMerchant)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, EVENT(Func_02000330)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgFieldWeRunAVerySmallInn)},
    {TRIGGER_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgFieldIDontKnowWhenIllBe)},
    {TRIGGER_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgFieldIHearTheSeafoodInMadra)},
    {TRIGGER_TALK, 24, CONDITION_ALWAYS, MSGNO(MsgSeaOfTimeCurrents)},
    {TRIGGER_TALK, 25, CONDITION_ALWAYS, MSGNO(MsgSeaOfTimeRedRocks)},
    {TRIGGER_TALK, 26, CONDITION_ALWAYS, MSGNO(MsgSeaOfTimeThreeGenerations)},
    {TRIGGER_TALK, 27, CONDITION_ALWAYS, EVENT(DailaInteriors_TalkToArmorMerchant)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgFieldRikiTaviWhereAreYouWhat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgFieldThatTavisABadInfluenceOn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgFieldIllBetThatWhereverTaviIs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgFieldIfTaviIsntHomeSoonWere)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgFieldIWonderHowTheRoadTo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgFieldIWonderIfTheHolyMan)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgFieldAtLeastNobodySeemsToHave)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgFieldIStillDontKnowWhatThat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgFieldWeShouldFindADifferentWay)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgFieldNowThatBriggsIsLockedAway)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgFieldImGladImNotGoingTo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgDailaInteriorsScamps)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgFieldWeCantSailAndWeCant)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgFieldLookAtAllTheseGuestsWe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgFieldAfterAllThatShakingIWouldnt)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS, MSGNO(MsgFieldImSoTornDoILike)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS, MSGNO(MsgSeaOfTimeHusbandDizzy)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, CONDITION_ALWAYS, MSGNO(MsgSeaOfTimeYeppToldMe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 26, CONDITION_ALWAYS, MSGNO(MsgSeaOfTimeGiveUp)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 27, CONDITION_ALWAYS, EVENT(DailaInteriors_TalkScamps)},
    {TRIGGERS_END},
};

struct Trigger gData_02001924[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, EVENT(DailaInteriors_TalkToHealer)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgFieldTravelersWithNowhereToGoThis)},
    {TRIGGERS_END},
};

struct Trigger gData_02001990[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgFieldLetMeTellYouKidsAre)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgFieldWhoStaysOutsideUntilHesAbsolutely)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgFieldOurLittleOneIsFinallyHome)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgFieldTaviWolfedDownHisSnackAnd)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, EVENT(Func_02000614)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgFieldIThinkMadraIsEastOf)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, EVENT(Func_0200066c)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgFieldItsTooDangerousToGoTo)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgFieldWhoWasThatGuyIWonder)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgFieldWhyDidThePirateBriggsPlague)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, EVENT(DailaInteriors_TalkToWeaponMerchantOnceTaviIsRescued)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, EVENT(DailaInteriors_TalkToArmorMerchantOnceTaviIsRescued)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, EVENT(Func_020005b4)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgDailaInteriorsDinnerShowPractice)},
    {TRIGGER_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgDailaInteriorsSeafoodWhen)},
    {TRIGGER_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgDailaInteriorsVegetablesAgain)},
    {TRIGGER_TALK, 24, CONDITION_ALWAYS, MSGNO(MsgFieldMmmMmnphDadWasSoMad)},
    {TRIGGER_TALK, 25, CONDITION_ALWAYS, MSGNO(MsgFieldMmmphMmmmDontTellAnyoneWe)},
    {TRIGGER_TALK, 26, CONDITION_ALWAYS, EVENT(Func_020006c4)},
    {TRIGGER_TALK, 27, CONDITION_ALWAYS, MSGNO(MsgSeaOfTimeCurrents)},
    {TRIGGER_TALK, 28, CONDITION_ALWAYS, MSGNO(MsgSeaOfTimeRedRocks)},
    {TRIGGER_TALK, 29, CONDITION_ALWAYS, MSGNO(MsgSeaOfTimeThreeGenerations)},
    {TRIGGER_TALK, 30, CONDITION_ALWAYS, EVENT(DailaInteriors_TalkToArmorMerchant)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgFieldDarnThatRikiAhImGlad)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgFieldHeeHeeMyRikiIsMommys)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgFieldIGuessRikiNextDoorMade)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgFieldOnceTaviFinishesEatingImPutting)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgFieldDailaGotHitPrettyHardI)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgFieldSomeoneShouldSeeHowTheRoad)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgFieldHeSaidHedPayMeA)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgFieldTheRoadToMadraGoesThrough)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgFieldMaybeThatGuyWashedUpOn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgFieldBriggsProbablyHasLotsOfMen)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgFieldThisTownMayBeDirtPoor)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgDailaInteriorsMazeTreasure)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgDailaInteriorsTiredPerformer)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgDailaInteriorsDragonKingLine)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgDailaInteriorsBadShows)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS, MSGNO(MsgDailaInteriorsGuestDance)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS, MSGNO(MsgFieldIStillWantToCatchThat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, CONDITION_ALWAYS, MSGNO(MsgFieldImSureGladIMadeIt)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 26, CONDITION_ALWAYS, MSGNO(MsgDailaInteriorsSeafoodStory)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 27, CONDITION_ALWAYS, MSGNO(MsgSeaOfTimeHusbandDizzy)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 28, CONDITION_ALWAYS, MSGNO(MsgSeaOfTimeYeppToldMe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 29, CONDITION_ALWAYS, MSGNO(MsgSeaOfTimeGiveUp)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 30, CONDITION_ALWAYS, EVENT(DailaInteriors_TalkMazeTreasure)},
    {TRIGGERS_END},
};

struct Trigger gData_02001c18[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {0x4602, 30, CONDITION_ALWAYS, EVENT(Func_02000c74)},
    {0x602, 31, CONDITION_ALWAYS, EVENT(Func_02000c74)},
    {0x8602, 32, CONDITION_ALWAYS, EVENT(Func_02000c74)},
    {0xc602, 33, CONDITION_ALWAYS, EVENT(Func_02000c74)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, EVENT(DailaInteriors_TalkToHealerOnceTaviIsRescued)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgDailaInteriorsBoringVillage)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 10, 0x220, EVENT(Scene_RaiseHeldActor)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 11, 0x221, EVENT(Scene_RaiseHeldActor)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 12, 0x222, EVENT(Scene_RaiseHeldActor)},
    {TRIGGER_EXAMINE, 40, 0xa20, EVENT(Func_02000730)},
    {TRIGGERS_END},
};
