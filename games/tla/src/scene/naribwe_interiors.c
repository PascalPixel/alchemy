/// @file
/// Naribwe's houses, among them the fortune teller's hut, where for 20 coins
/// he reads an item the party sets on his table: a weapon for the foe ahead,
/// armor for an unfound Djinni, a prong for the foe it masters, and other items
/// for the path to follow.

#include "shared/tables.h"
#include "field/field.h"
#include "battle/battle.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "scene_tables.h"
#include "event_work.h"
#include "module/scene/scene.h"
#include "field/event_inline.h"
#include "system/main_routine_work.h"
#include "actor.h"
#include "version.h"
#include "call.h"
#include "system/ram_buffer.h"

extern struct Trigger gNaribweInteriors_Triggers[];
extern struct Trigger gNaribweInteriors_TriggersOnReturn[];

void NaribweInteriors_ReadFortune(void);

s32 ItemPicker_RunMode1(s32 *unit, s32 *slot);

void NaribweInteriors_ReadProng(void);

void NaribweInteriors_ReadWeapon(void);

void NaribweInteriors_ReadArmor(void);

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

#include "text/msg_ids.h"

/// "The items that one carries are filled with one's hopes and desires."
s32 NaribweInteriors_MindReadFortuneTeller(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgTheItemsThat);
  Event_ShowMessage(10, 0);
  Event_End();
  return 0;
}

s32 NaribweInteriors_MindReadFortuneTellerOnReturn(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgItemsPlacedBefore);
  Event_ShowMessage(10, 0);
  Event_End();
  return 0;
}

const struct Trigger *Scene_GetEvents(void) {
  if (Event_GetFlag(0x8ff))
    return gNaribweInteriors_TriggersOnReturn;
  return gNaribweInteriors_Triggers;
}

void NaribweInteriors_RecommendFortuneTeller(s32 actor) {
  u16 facing = Actor_Get(gMainRoutineWork.currentOwner)->facing;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Shop_Run(14, actor);
  } else {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    FieldEvent_SetMessageId((s32)MsgIfYouAre);
    Event_ShowMessage(actor, 0);
    Event_End();
  }
}

void NaribweInteriors_RecommendFortuneTellerOnReturn(s32 actor) {
  u16 facing = Actor_Get(gMainRoutineWork.currentOwner)->facing;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Shop_Run(14, actor);
  } else {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    FieldEvent_SetMessageId((s32)MsgWheneverIGet);
    Event_ShowMessage(actor, 0);
    Event_End();
  }
}

void NaribweInteriors_SendToFortuneTeller(s32 actor) {
  u16 facing = Actor_Get(gMainRoutineWork.currentOwner)->facing;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Shop_Run(15, actor);
  } else {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    FieldEvent_SetMessageId((s32)MsgYouShouldVisit);
    Event_ShowMessage(actor, 0);
    Event_End();
  }
}

void NaribweInteriors_SendToFortuneTellerOnReturn(s32 actor) {
  u16 facing = Actor_Get(gMainRoutineWork.currentOwner)->facing;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Shop_Run(15, actor);
  } else {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    FieldEvent_SetMessageId((s32)MsgIfYouWish);
    Event_ShowMessage(actor, 0);
    Event_End();
  }
}

/// "Our cooking is famous all across Gondowan."
void NaribweInteriors_TalkAboutCooking(s32 actor) {
  u16 facing = Actor_Get(gMainRoutineWork.currentOwner)->facing;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Inn_CheckIn(5, actor);
  } else {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    FieldEvent_SetMessageId((s32)MsgOurCookingIs);
    Event_ShowMessage(actor, 0);
    Event_End();
  }
}

void NaribweInteriors_TalkAboutTravelers(s32 actor) {
  u16 facing = Actor_Get(gMainRoutineWork.currentOwner)->facing;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Inn_CheckIn(5, actor);
  } else {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    FieldEvent_SetMessageId((s32)MsgISeeSo);
    Event_ShowMessage(actor, 0);
    Event_End();
  }
}

/// The fortune-teller offers a reading for 20 coins.
/// "I am a teller of fortunes. Would you have a reading?"
void NaribweInteriors_TalkToFortuneTeller(void) {
  u8 *message;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  message = MsgIAmA;
  FieldEvent_SetMessageId((s32)message);
  Event_OpenMessage(10, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) ==
      0) {
    FieldEvent_SetMessageId((s32)(message + 2));
    Event_OpenMessage(10, 0);
    if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) ==
        0) {
      if ((u32)gMainRoutineWork.coins > 19) {
        NaribweInteriors_ReadFortune();
        Event_End();
      } else {
        FieldEvent_SetMessageId((s32)(message + 4));
        Event_ShowMessage(10, 0);
        Event_End();
      }
      return;
    }
  }
  FieldEvent_SetMessageId((s32)MsgThenStandBack);
  Event_ShowMessage(10, 0);
  Event_End();
}

void *Unit_Get(s32 owner);
void NaribweInteriors_ReadShiningItem(void);

/// Object 10 steps to (x, 96) without turning, then dances (mode 5).
#define DANCE_TO(x)                                                            \
  (Actor_Get(10)->actionFlags &= ~1, Actor_WalkTo(10, x, 96),                  \
   Actor_SetAnimation(10, 5))

/// The reading: the party sets one of a member's items on the table and the
/// fortune-teller reads it by its kind, a prong of the Trident and the Trident
/// itself on their own, then takes his 20 coins.
void NaribweInteriors_ReadFortune(void) {
  s32 slot, unit;
  struct UnitData *unitData;
  const u8 *message, *refuse, *info;
#if !TLA_PATCH_1
  s32 item;
#else
  u32 item;
#endif
  u8 kind;
  s32 rawKind;

  FieldEvent_SetMessageId(MsgVeryWellShow);
  Event_ShowMessage(10, 0);
  FieldEvent_Wait(30);
ask:
#if !TLA_PATCH_1
  Actor_SetAnimationAndWait(gMainRoutineWork.currentOwner, 3);
  // Before patch 1 the item variable first holds the choice's result.
  item = ItemPicker_RunMode1(&unit, &slot);
  if (item == -1) {
#else
  Actor_SetAnimationAndWait(gMainRoutineWork.currentOwner, 3);
  if (ItemPicker_RunMode1(&unit, &slot) == -1) {
#endif
    refuse = MsgShallIGive;
    FieldEvent_SetMessageId(refuse);
    Event_OpenMessage(10, 0);
    if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) !=
        1)
      goto ask;
    FieldEvent_SetMessageId(refuse + 1);
    Event_ShowMessage(10, 0);
    return;
  }
  unitData = Unit_Get(unit);
  Item_GetDirect(unitData->items[slot]);
  Actor_SetItemIcon(Actor_Get(18), unitData->items[slot]);
  Actor_SetPosition(18, 672, 104);
  FieldEvent_Wait(60);
  MessageArgument_Push(unitData->items[slot], 2);
  Message_ShowCentered(MsgPlacedOnThe, 1);
  message = MsgAreYouPrepared;
  FieldEvent_SetMessageId(message);
  Event_OpenMessage(10, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) !=
      1)
    goto read;
  Actor_SetPosition(18, 0, 0);
  MessageArgument_Push(unitData->items[slot], 2);
  Message_ShowCentered(message + 1, 1);
  FieldEvent_SetMessageId(message + 2);
  Event_OpenMessage(10, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) !=
      1)
    goto ask;
  FieldEvent_SetMessageId(message + 3);
  Event_ShowMessage(10, 0);
  return;
read:
  FieldEvent_SetMessageId(message + 4);
  Event_ShowMessage(10, 0);
  PaletteFade_ApplySource(0x10000, 0);
  FieldEvent_Wait(30);
  PaletteFade_ApplyTarget(0x40250d, 1);
  PaletteFade_Interpolate(30);
  FieldEvent_Wait(120);
  Event_PlaySound(MUS_011);
  Actor_SetAnimation(10, 5);
  Actor_WaitForAnimationChange(10);
  DANCE_TO(664);
  FieldEvent_Wait(30);
  DANCE_TO(680);
  FieldActor_Jump(10, 4, 90);
  DANCE_TO(664);
  FieldEvent_Wait(30);
  DANCE_TO(680);
  FieldActor_Jump(10, 4, 90);
  DANCE_TO(672);
  FieldActor_Jump(10, 4, 30);
  FieldActor_Jump(10, 4, 30);
  Actor_SetAnimation(10, 5);
  Actor_SetAnimation(10, 0);
  Event_PlaySound(MUS_011);
  PaletteFade_ApplyTarget(0x10000, 1);
  PaletteFade_Interpolate(30);
  Actor_SetPosition(20, 672, 104);
  Event_PlaySound(SE_CLATTER);
  Actor_SetAnimation(20, 2);
  Actor_WaitForAnimationChange(20);
  Actor_SetPosition(20, 0, 0);
  FieldEvent_Wait(4);
  Actor_SetPosition(19, 672, 104);
  Actor_SetAnimation(19, 4);
  Actor_WaitForAnimationChange(19);
  FieldEvent_Wait(15);
  Actor_SetPosition(19, 0, 0);
  FieldEvent_Wait(30);
  Actor_ShowEmote(gMainRoutineWork.currentOwner, EMOTE_IN_FRONT | EMOTE_WINCE,
                  0);
  FieldEvent_Wait(60);
  Event_PlaySound(MUS_010);
  Actor_ShowEmote(10, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  Event_ShowMessage(10, 0);
  unitData = Unit_Get(unit);
  info = Item_GetDirect(unitData->items[slot]);
#if !TLA_PATCH_1
  item = unitData->items[slot];
#else
  item = unitData->items[slot] & 0x1ff;
#endif
  if ((u16)(item - 0x1b7) <= 2) {
    NaribweInteriors_ReadProng();
  } else if ((u16)item == 326) {
    FieldEvent_SetMessageId(message + 29);
    Event_ShowMessage(10, 0);
    if (!Event_GetFlag(0x940)) {
      FieldEvent_SetMessageId(message + 30);
      Event_ShowMessage(10, 0);
    } else {
      FieldEvent_SetMessageId(message + 31);
      Event_ShowMessage(10, 0);
    }
  } else {
    rawKind = info[2];
    kind = rawKind;
    if (kind == 6)
      NaribweInteriors_ReadShiningItem();
    else if (kind == 1)
      NaribweInteriors_ReadWeapon();
    else if ((u8)((u8)rawKind - 2) <= 3)
      NaribweInteriors_ReadArmor();
    else
      NaribweInteriors_ReadOtherItem();
  }
  Actor_SetPosition(18, 0, 0);
  Party_AdjustCoins(-20);
}

s32 Party_FindOwnerWithAction(s32 item);
s32 NaribweInteriors_FoeAhead(void);
s32 NaribweInteriors_NextPowerToGain(void);
s32 NaribweInteriors_NextPath(void);
s32 NaribweInteriors_ProngsHeld(void);
u8 *NaribweInteriors_GetDjinniHint(void);

/// A weapon: the foe that lies beyond it.
/// "Beyond the weapon you set down lies a terrible foe."
void NaribweInteriors_ReadWeapon(void) {
  u8 *message = MsgBeyondTheWeapon;

  FieldEvent_SetMessageId((s32)message);
  Event_ShowMessage(10, 0);
  switch (NaribweInteriors_FoeAhead()) {
  case 0:
    FieldEvent_SetMessageId((s32)(message + 1));
    Event_ShowMessage(10, 0);
    break;
  case 1:
    FieldEvent_SetMessageId((s32)(message + 2));
    Event_ShowMessage(10, 0);
    break;
  case 2:
    FieldEvent_SetMessageId((s32)(message + 3));
    Event_ShowMessage(10, 0);
    break;
  }
}

/// An item of kind 6: a power shining beyond it, and where the story goes next.
/// "I feel a shining power glowing beyond the item before me."
void NaribweInteriors_ReadShiningItem(void) {
  u8 *message = MsgIFeelA;

  FieldEvent_SetMessageId((s32)message);
  Event_ShowMessage(10, 0);
  switch (NaribweInteriors_NextPowerToGain()) {
  case 3:
    FieldEvent_SetMessageId((s32)MsgYouAreLost);
    Event_ShowMessage(10, 0);
    break;
  case 4:
    FieldEvent_SetMessageId((s32)MsgADistantIsle);
    Event_ShowMessage(10, 0);
    break;
  case 5:
    FieldEvent_SetMessageId((s32)MsgISeeA);
    Event_ShowMessage(10, 0);
    break;
  case 6:
    FieldEvent_SetMessageId((s32)MsgATinyFigure);
    Event_ShowMessage(10, 0);
    break;
  case 7:
    FieldEvent_SetMessageId((s32)MsgISeeYou);
    Event_ShowMessage(10, 0);
    break;
  default:
    FieldEvent_SetMessageId((s32)(message + 6));
    Event_ShowMessage(10, 0);
    break;
  }
}

/// A piece of armor, kinds 2 to 5: no future, and where an unfound Djinni
/// waits.
/// "In the item you have placed before me, I can see nothing of the future..."
void NaribweInteriors_ReadArmor(void) {
  s32 message = NaribweInteriors_GetDjinniHint();

  if (message == (s32)MsgInTheItem) {
    FieldEvent_SetMessageId(message);
    Event_ShowMessage(10, 0);
  } else {
    FieldEvent_SetMessageId((s32)MsgInTheItem - 42);
    Event_ShowMessage(10, 0);
    FieldEvent_SetMessageId(message);
    Event_ShowMessage(10, 0);
  }
}

/// Any other item: the path the party should follow.
/// "Beyond the item before me, I see a path for you to follow."
void NaribweInteriors_ReadOtherItem(void) {
  FieldEvent_SetMessageId((s32)MsgBeyondTheItem);
  Event_ShowMessage(10, 0);
  switch (NaribweInteriors_NextPath()) {
  case 0:
    FieldEvent_SetMessageId((s32)MsgISeeYouInside);
    Event_ShowMessage(10, 0);
    break;
  case 9:
    FieldEvent_SetMessageId((s32)MsgYouMustForge);
    Event_ShowMessage(10, 0);
    break;
  case 10:
    FieldEvent_SetMessageId((s32)MsgYouWillRaise);
    Event_ShowMessage(10, 0);
    break;
  case 11:
    FieldEvent_SetMessageId((s32)MsgYouWillGain);
    Event_ShowMessage(10, 0);
    break;
  case 12:
    FieldEvent_SetMessageId((s32)MsgYouHaveThe);
    Event_ShowMessage(10, 0);
    break;
  case 13:
    FieldEvent_SetMessageId((s32)MsgTheObjectThat);
    Event_ShowMessage(10, 0);
    break;
  }
}

/// A prong of the Trident: the foe it holds power over.
/// "The object you have placed before me wields a mighty power over a terrible
/// foe."
void NaribweInteriors_ReadProng(void) {
  u8 *message = MsgTheObjectYou;

  FieldEvent_SetMessageId((s32)message);
  Event_ShowMessage(10, 0);
  switch (NaribweInteriors_ProngsHeld()) {
  case 14:
    FieldEvent_SetMessageId((s32)(message + 1));
    Event_ShowMessage(10, 0);
    break;
  case 15:
    FieldEvent_SetMessageId((s32)(message + 2));
    Event_ShowMessage(10, 0);
    break;
  case 16:
    FieldEvent_SetMessageId((s32)(message + 3));
    Event_ShowMessage(10, 0);
    break;
  case 17:
    FieldEvent_SetMessageId((s32)(message + 4));
    Event_ShowMessage(10, 0);
    break;
  }
}

/// Which foe a weapon's reading names: 0 before the return with Piers, then 1,
/// and 2 once flag 0x940 is set.
s32 NaribweInteriors_FoeAhead(void) {
  if (Event_GetFlag(0x8ff) == 0)
    return 0;
  return (Event_GetFlag(0x940) != 0) + 1;
}

/// The first power the party still lacks, for a shining item's reading: three
/// Psynergy (abilities 144, 139 and 138) and two items (213, 214); 8 when it
/// has them all.
s32 NaribweInteriors_NextPowerToGain(void) {
  if (Party_FindOwnerWithAction(144) == -1)
    return 3;
  if (Party_FindOwnerWithAction(139) == -1)
    return 4;
  if (Party_FindOwnerWithAction(138) == -1)
    return 5;
  if (FieldPartyInventory_FindMember(213) == -1)
    return 6;
  if (FieldPartyInventory_FindMember(214) == -1)
    return 7;
  return 8;
}

/// The path an ordinary item's reading points along, by the story's flags and
/// the Trident's prongs (items 439 to 441) and the Trident (item 326).
s32 NaribweInteriors_NextPath(void) {
  if (Event_GetFlag(0x8ff) == 0)
    return 0;
  if (FieldPartyInventory_FindMember(326) == -1) {
    if (Event_GetFlag(0x940) == 0) {
      if (FieldPartyInventory_FindMember(439) == -1 ||
          FieldPartyInventory_FindMember(440) == -1 ||
          FieldPartyInventory_FindMember(441) == -1)
        return 9;
      return (Event_GetFlag(0x905) != 0) + 10;
    }
  } else {
    if (Event_GetFlag(0x940) == 0)
      return 12;
  }
  return 13;
}

/// Which of the three prongs the party carries, as the prong's reading tells
/// it.
s32 NaribweInteriors_ProngsHeld(void) {
  if (FieldPartyInventory_FindMember(439) == -1 &&
      (FieldPartyInventory_FindMember(441) != -1 ||
       FieldPartyInventory_FindMember(440) != -1))
    return 14;
  if (FieldPartyInventory_FindMember(439) != -1 &&
      FieldPartyInventory_FindMember(440) == -1 &&
      FieldPartyInventory_FindMember(441) == -1)
    return 15;
  if (FieldPartyInventory_FindMember(439) != -1 &&
      FieldPartyInventory_FindMember(440) == -1 &&
      FieldPartyInventory_FindMember(441) != -1)
    return 15;
  if (FieldPartyInventory_FindMember(439) != -1 &&
      FieldPartyInventory_FindMember(440) != -1 &&
      FieldPartyInventory_FindMember(441) == -1)
    return 16;
  if (FieldPartyInventory_FindMember(439) != -1 &&
      FieldPartyInventory_FindMember(440) != -1 &&
      FieldPartyInventory_FindMember(441) != -1)
    return 17;
}

/// One past the last Djinni of an element group found, skipping the ones never
/// hinted at, or -1 when none is.
s32 NaribweInteriors_DjinniHintLimit(s32 group) {
  s32 i = 17;

loop:
  {
    s32 flag = group * 20 + i + 48;

    if (group == 0) {
      if (i == 7)
        goto next;
    }
    if (group == 1) {
      if (i == 9)
        goto next;
      if (i == 10)
        goto next;
    }
    if (Event_GetFlag(flag))
      return i + 1;
  }
next:
  if (--i > 6)
    goto loop;
  return -1;
}

/// Lists the flags still unset for a group: from 7 up to the limit, skipping 7
/// in group 0 and 9 and 10 in group 1, each flag group * 20 + i + 48 not yet
/// set goes to the output; returns how many.
s32 NaribweInteriors_ListUnfoundDjinn(s32 limit, s32 group, s32 *out) {
  s32 count = 0;
  s32 i = 7;

  if (i < limit) {
    s32 *cursor = out;

    do {
      s32 flag;

      if (group == 0 && i == 7)
        goto next;
      if (group == 1) {
        if (i == 9)
          goto next;
        if (i == 10)
          goto next;
      }
      flag = group * 20 + i + 48;
      if (Event_GetFlag(flag) == 0) {
        *cursor++ = flag;
        count++;
      }
    next:
      i++;
    } while (i < limit);
  }
  return count;
}

/// The message telling where an unfound Djinni waits, or MsgInTheItem when none
/// is left to find.
u8 *NaribweInteriors_GetDjinniHint(void) {
  s32 *hints = (s32 *)Value2((s32 (*)())HeapSlot_AllocEwram, 252, 320);
  s32 limits[4];
  s32 count = 0;
  s32 lowest = 9999;
  s32 lowestGroup = 0;
  u32 group;
  s32 index;
  u8 *message;

  for (group = 0; group <= 3; group++) {
    s32 limit = NaribweInteriors_DjinniHintLimit(group);

    limits[group] = limit;
    if (lowest > limit) {
      lowest = limit;
      lowestGroup = group;
    }
    if (limit != -1)
      count += NaribweInteriors_ListUnfoundDjinn(limit, group, hints + count);
  }
  if (count == 0) {
    for (group = 0; group <= 3; group++)
      count += NaribweInteriors_ListUnfoundDjinn(18, group, hints + count);
    if (count == 0) {
      HeapSlot_Release(252);
      return MsgInTheItem;
    }
    NaribweInteriors_ListUnfoundDjinn(18, lowestGroup, hints);
    index = 0;
  } else {
    index = Random16() % count;
  }
  switch (hints[index]) {
  case 56:
    message = MsgToTheEast;
    break;
  case 57:
    message = MsgToTheNorth;
    break;
  case 58:
    message = MsgToTheNorthBeneath;
    break;
  case 59:
    message = MsgHeadEastBy;
    break;
  case 60:
    message = MsgHeadEastByNortheast;
    break;
  case 61:
    message = MsgHeadNorthBy;
    break;
  case 62:
    message = MsgToTheNorthwest;
    break;
  case 63:
    message = MsgHeadWestBy;
    break;
  case 64:
    message = MsgToTheNorthIn;
    break;
  case 65:
    message = MsgHeadEastBySoutheast;
    break;
  case 75:
    message = MsgHeadEastByNortheastIn;
    break;
  case 76:
    message = MsgToTheSoutheast;
    break;
  case 79:
    message = MsgToTheSouth;
    break;
  case 80:
    message = MsgToTheEastBeside;
    break;
  case 81:
    message = MsgHeadEastByNortheastInThe;
    break;
  case 82:
    message = MsgToTheNorthSomewhere;
    break;
  case 83:
    message = MsgHeadNorthByNorthwest;
    break;
  case 84:
    message = MsgToTheNorthInRuins;
    break;
  case 85:
    message = MsgHeadEastByNortheastInA;
    break;
  case 95:
    message = MsgToTheEastIn;
    break;
  case 96:
    message = MsgToTheSoutheastIn;
    break;
  case 97:
    message = MsgHeadSouthBy;
    break;
  case 98:
    message = MsgHeadEastBySoutheastIn;
    break;
  case 99:
    message = MsgHeadEastByNortheastOn;
    break;
  case 100:
    message = MsgHeadSouthBySoutheast;
    break;
  case 101:
    message = MsgToTheNorthwestIn;
    break;
  case 102:
    message = MsgToTheNorthwestAlong;
    break;
  case 103:
    message = MsgToTheNorthwestBeside;
    break;
  case 104:
    message = MsgHeadNorthByNorthwestAt;
    break;
  case 105:
    message = MsgToTheNorthInThe;
    break;
  case 115:
    message = MsgHeadEastByNortheastInACave;
    break;
  case 116:
    message = MsgHeadEastBySoutheastInThe;
    break;
  case 117:
    message = MsgHeadEastBySoutheastInA;
    break;
  case 118:
    message = MsgToTheNorthInTheMountain;
    break;
  case 119:
    message = MsgToTheEastSomewhere;
    break;
  case 120:
    message = MsgToTheEastInThe;
    break;
  case 121:
    message = MsgHeadNorthByNorthwestIn;
    break;
  case 122:
    message = MsgInTheNorthwest;
    break;
  case 123:
    message = MsgHeadNorthByNorthwestNear;
    break;
  case 124:
    message = MsgToTheNorthInTheRuins;
    break;
  case 125:
    message = MsgHeadNorthByNortheast;
    break;
  default:
    message = MsgInTheItem;
    break;
  }
  HeapSlot_Release(252);
  return message;
}

void Actor_DisableById(s32 id);

s32 Scene_Initialize(void) {
  struct Actor *object;

  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x209;
  Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 0x20;
  Actor_DisableById(17);
  Actor_DisableById(18);
  FieldActor_SetSpritePriority(10, 2);
  Actor_Get(12)->collisionFlags |= 4;
  FieldActor_SetSpritePriority(16, 2);
  Actor_Get(16)->collisionFlags |= 4;

  object = Actor_Get(18);
  object->motionFlags = 0;
  object->y = -0x40000;
  object->terrainHeight = -0x40000;
  Actor_Get(18)->priorityFlags |= 0x20;
  FieldActor_SetSpritePriority(18, 1);
  Actor_SetPosition(18, 0, 0);

  object = Actor_Get(19);
  object->motionFlags = 0;
  object->y = 0xc0000;
  object->terrainHeight = 0xc0000;
  Actor_Get(19)->priorityFlags |= 0x20;
  FieldActor_SetSpritePriority(19, 1);
  Actor_SetPosition(19, 0, 0);

  object = Actor_Get(20);
  object->motionFlags = 0;
  object->y = 0xc0000;
  object->terrainHeight = 0xc0000;
  Actor_Get(20)->priorityFlags |= 0x20;
  FieldActor_SetSpritePriority(20, 1);
  Actor_SetPosition(20, 0, 0);

  Actor_SetSpriteFlags(Actor_Get(19), 0);
  Actor_SetSpriteFlags(Actor_Get(20), 0);
  FieldActor_SetSpriteColor(Actor_Get(20), 4);
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(NaribweInteriors),
    SCENE_EXIT_UNLESS(1, Naribwe, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, Naribwe, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, Naribwe, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, Naribwe, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, Naribwe, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, NaribweInteriors, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, NaribweInteriors, 6, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(115, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 120, 0, 104,
                    FACING_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(116, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 360,
                    FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(25, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 672, 0, 94,
                    FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(106, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 651, 0, 106,
                    FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(107, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 693, 0, 106,
                    FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(108, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 104,
                    FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(109, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 104,
                    FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(108, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 424, 0, 360,
                    FACING_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(135, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 368,
                    FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 672, 0, 112,
                    FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 672, 0, 104,
                    FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(262, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 672, 0, 104,
                    FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(285, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 672, 0, 104,
                    FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct Trigger gNaribweInteriors_Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgTheFortuneTeller)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgWeReA)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgIfYouDesire)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgMyFatherIs)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgByPaintingOur)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS,
     EVENT(NaribweInteriors_RecommendFortuneTeller)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS,
     EVENT(NaribweInteriors_SendToFortuneTeller)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgIfYouLike)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS,
     EVENT(NaribweInteriors_TalkAboutCooking)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS,
     EVENT(NaribweInteriors_TalkToFortuneTeller)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgTheFortuneTellerIn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgAsLongAs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     MSGNO(MsgTheItemsThat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
     MSGNO(MsgMyFatherMay)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS,
     MSGNO(MsgBodyPaintingAlso)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS,
     MSGNO(MsgICanLook)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS,
     MSGNO(MsgWheneverIGetMy)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS,
     MSGNO(MsgTheRiverIs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS,
     MSGNO(MsgOurSpicesAre)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS,
     EVENT(NaribweInteriors_MindReadFortuneTeller)},
    {TRIGGER_TABLE_END},
};

struct Trigger gNaribweInteriors_TriggersOnReturn[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgIGuessThose)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgOurFortuneTeller)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgIfYouDesire)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgIfTroubleResides)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgReadingFortunesIn)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS,
     EVENT(NaribweInteriors_RecommendFortuneTellerOnReturn)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS,
     EVENT(NaribweInteriors_SendToFortuneTellerOnReturn)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgIfIsTraveling)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS,
     EVENT(NaribweInteriors_TalkAboutTravelers)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS,
     EVENT(NaribweInteriors_TalkToFortuneTeller)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgThePowerOf)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgTheFortuneTellerSaid)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     MSGNO(MsgItemsPlacedBefore)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
     MSGNO(MsgFatherShouldCharge)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS,
     MSGNO(MsgAfterFatherS)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS,
     MSGNO(MsgWhenILook)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS,
     MSGNO(MsgILikeOffering)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS,
     MSGNO(MsgHePaidMe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS,
     MSGNO(MsgWhyWouldTravel)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS,
     EVENT(NaribweInteriors_MindReadFortuneTellerOnReturn)},
    {TRIGGER_TABLE_END},
};
