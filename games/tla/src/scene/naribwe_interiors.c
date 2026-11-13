/// @file
/// Overlay 0x66e's scene: its code, then its tables, in the order the ROM has them.

#include "field/field.h"

#include "battle.h"
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

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

#include "text/msg_ids.h"
#include "event_inline.h"

void Event_Start(void);
void Event_End(void);

s32 Func_02000054(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgTheItemsThat);
  Event_ShowMessage(10, 0);
  Event_End();
  return 0;
}

s32 Func_0200007c(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgItemsPlacedBefore);
  Event_ShowMessage(10, 0);
  Event_End();
  return 0;
}

#include "main_routine_work.h"
#include "actor.h"

extern struct Trigger gData_02001370[];
extern struct Trigger gData_020014c0[];

void Func_02001190(s32 slot, s32 actor);
void Func_02001198(s32 slot, s32 actor);

const struct Trigger *Scene_GetEvents(void) {
  if (Event_GetFlag(0x8ff))
    return gData_020014c0;
  return gData_02001370;
}

void Func_020000c4(s32 actor) {
  u16 facing = Actor_Get(gMainRoutineWork.currentOwner)->facing;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Func_02001190(14, actor);
  } else {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    FieldEvent_SetMessageId((s32)MsgIfYouAre);
    Event_ShowMessage(actor, 0);
    Event_End();
  }
}

void Func_02000124(s32 actor) {
  u16 facing = Actor_Get(gMainRoutineWork.currentOwner)->facing;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Func_02001190(14, actor);
  } else {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    FieldEvent_SetMessageId((s32)MsgWheneverIGet);
    Event_ShowMessage(actor, 0);
    Event_End();
  }
}

void Func_02000184(s32 actor) {
  u16 facing = Actor_Get(gMainRoutineWork.currentOwner)->facing;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Func_02001190(15, actor);
  } else {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    FieldEvent_SetMessageId((s32)MsgYouShouldVisit);
    Event_ShowMessage(actor, 0);
    Event_End();
  }
}

void Func_020001e4(s32 actor) {
  u16 facing = Actor_Get(gMainRoutineWork.currentOwner)->facing;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Func_02001190(15, actor);
  } else {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    FieldEvent_SetMessageId((s32)MsgIfYouWish);
    Event_ShowMessage(actor, 0);
    Event_End();
  }
}

void Func_02000244(s32 actor) {
  u16 facing = Actor_Get(gMainRoutineWork.currentOwner)->facing;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Func_02001198(5, actor);
  } else {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    FieldEvent_SetMessageId((s32)MsgOurCookingIs);
    Event_ShowMessage(actor, 0);
    Event_End();
  }
}

void Func_020002a4(s32 actor) {
  u16 facing = Actor_Get(gMainRoutineWork.currentOwner)->facing;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Func_02001198(5, actor);
  } else {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    FieldEvent_SetMessageId((s32)MsgISeeSo);
    Event_ShowMessage(actor, 0);
    Event_End();
  }
}

void Func_02000390(void);

void Func_02000304(void) {
  u8 *message;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  message = MsgIAmA;
  FieldEvent_SetMessageId((s32)message);
  Event_OpenMessage(10, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) == 0) {
    FieldEvent_SetMessageId((s32)(message + 2));
    Event_OpenMessage(10, 0);
    if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) == 0) {
      if ((u32)gMainRoutineWork.coins > 19) {
        Func_02000390();
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

#include "version.h"

s32 Func_020011a0(s32 *unit, s32 *slot);
void *Unit_Get(s32 owner);
const u8 *Func_020010b8(s32 item);
void MessageArgument_Push(s32 item, s32 a);
void Func_02000920(void);
void Func_020007b4(void);
void Func_02000760(void);
void Func_02000838(void);
void Func_02000878(void);
void Party_AdjustCoins(s32 coins);

struct UnitData {
  u8 filler[216];
  u16 items[15];
};

/// Object 10 steps to (x, 96) without turning, then dances (mode 5).
#define DANCE_TO(x)                                                            \
  (Actor_Get(10)->actionFlags &= ~1, Actor_WalkTo(10, x, 96),              \
   Actor_SetAnimation(10, 5))

void Func_02000390(void) {
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
  /* FAKEMATCH(temporary): item also holds this call's result; as a separate
   * test of the call, 23 of 374 instructions differ in tla-ja. */
  item = Func_020011a0(&unit, &slot);
  if (item == -1) {
#else
  Actor_SetAnimationAndWait(gMainRoutineWork.currentOwner, 3);
  if (Func_020011a0(&unit, &slot) == -1) {
#endif
    refuse = MsgShallIGive;
    FieldEvent_SetMessageId(refuse);
    Event_OpenMessage(10, 0);
    if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) != 1)
      goto ask;
    FieldEvent_SetMessageId(refuse + 1);
    Event_ShowMessage(10, 0);
    return;
  }
  unitData = Unit_Get(unit);
  Func_020010b8(unitData->items[slot]);
  Actor_SetItemIcon(Actor_Get(18), unitData->items[slot]);
  Actor_SetPosition(18, 0x2a00000, 0x680000);
  FieldEvent_Wait(60);
  MessageArgument_Push(unitData->items[slot], 2);
  Message_ShowCentered(MsgPlacedOnThe, 1);
  message = MsgAreYouPrepared;
  FieldEvent_SetMessageId(message);
  Event_OpenMessage(10, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) != 1)
    goto read;
  Actor_SetPosition(18, 0, 0);
  MessageArgument_Push(unitData->items[slot], 2);
  Message_ShowCentered(message + 1, 1);
  FieldEvent_SetMessageId(message + 2);
  Event_OpenMessage(10, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) != 1)
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
  Event_PlaySound(11);
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
  DANCE_TO(0x2a0);
  FieldActor_Jump(10, 4, 30);
  FieldActor_Jump(10, 4, 30);
  Actor_SetAnimation(10, 5);
  Actor_SetAnimation(10, 0);
  Event_PlaySound(11);
  PaletteFade_ApplyTarget(0x10000, 1);
  PaletteFade_Interpolate(30);
  Actor_SetPosition(20, 0x2a00000, 0x680000);
  Event_PlaySound(0x90);
  Actor_SetAnimation(20, 2);
  Actor_WaitForAnimationChange(20);
  Actor_SetPosition(20, 0, 0);
  FieldEvent_Wait(4);
  Actor_SetPosition(19, 0x2a00000, 0x680000);
  Actor_SetAnimation(19, 4);
  Actor_WaitForAnimationChange(19);
  FieldEvent_Wait(15);
  Actor_SetPosition(19, 0, 0);
  FieldEvent_Wait(30);
  Actor_ShowEmote(gMainRoutineWork.currentOwner, 258, 0);
  FieldEvent_Wait(60);
  Event_PlaySound(10);
  Actor_ShowEmote(10, 0x100, 60);
  Event_ShowMessage(10, 0);
  unitData = Unit_Get(unit);
  info = Func_020010b8(unitData->items[slot]);
#if !TLA_PATCH_1
  item = unitData->items[slot];
#else
  item = unitData->items[slot] & 0x1ff;
#endif
  if ((u16)(item - 0x1b7) <= 2) {
    Func_02000920();
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
      Func_020007b4();
    else if (kind == 1)
      Func_02000760();
    else if ((u8)((u8)rawKind - 2) <= 3)
      Func_02000838();
    else
      Func_02000878();
  }
  Actor_SetPosition(18, 0, 0);
  Party_AdjustCoins(-20);
}

s32 Party_FindOwnerWithAction(s32 item);
s32 Func_0200097c(void);
s32 Func_020009a4(void);
s32 Func_020009f8(void);
s32 Func_02000a7c(void);
u8 *Func_02000c04(void);

void Func_02000760(void) {
  u8 *message = MsgBeyondTheWeapon;

  FieldEvent_SetMessageId((s32)message);
  Event_ShowMessage(10, 0);
  switch (Func_0200097c()) {
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

void Func_020007b4(void) {
  u8 *message = MsgIFeelA;

  FieldEvent_SetMessageId((s32)message);
  Event_ShowMessage(10, 0);
  switch (Func_020009a4()) {
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

void Func_02000838(void) {
  s32 message = Func_02000c04();

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

void Func_02000878(void) {
  FieldEvent_SetMessageId((s32)MsgBeyondTheItem);
  Event_ShowMessage(10, 0);
  switch (Func_020009f8()) {
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

void Func_02000920(void) {
  u8 *message = MsgTheObjectYou;

  FieldEvent_SetMessageId((s32)message);
  Event_ShowMessage(10, 0);
  switch (Func_02000a7c()) {
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

s32 Func_0200097c(void) {
  if (Event_GetFlag(0x8ff) == 0)
    return 0;
  return (Event_GetFlag(0x940) != 0) + 1;
}

s32 Func_020009a4(void) {
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

s32 Func_020009f8(void) {
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

s32 Func_02000a7c(void) {
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
  // No remaining combinations: the native falls off the end without a value.
}

s32 Func_02000b60(s32 group) {
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

// Lists the flags still unset for a group: from 7 up to the limit, skipping 7
// in group 0 and 9 and 10 in group 1, each flag group * 20 + i + 48 not yet set
// goes to the output; returns how many.
s32 Func_02000ba0(s32 limit, s32 group, s32 *out) {
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

#include "call.h"

u8 *Func_02000c04(void) {
  s32 *hints = (s32 *)Value2((s32 (*)())HeapSlot_AllocEwram, 252, 320);
  s32 limits[4];
  s32 count = 0;
  s32 lowest = 9999;
  s32 lowestGroup = 0;
  u32 group;
  s32 index;
  u8 *message;

  for (group = 0; group <= 3; group++) {
    s32 limit = Func_02000b60(group);

    limits[group] = limit;
    if (lowest > limit) {
      lowest = limit;
      lowestGroup = group;
    }
    if (limit != -1)
      count += Func_02000ba0(limit, group, hints + count);
  }
  if (count == 0) {
    for (group = 0; group <= 3; group++)
      count += Func_02000ba0(18, group, hints + count);
    if (count == 0) {
      HeapSlot_Release(252);
      return MsgInTheItem;
    }
    Func_02000ba0(18, lowestGroup, hints);
    index = 0;
  } else {
    index = Random16Far() % count;
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

#include "ram_buffer.h"

void Func_02001158(s32 id);

s32 Scene_Initialize(void) {
  struct Actor *object;

  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x209;
  Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 0x20;
  Func_02001158(17);
  Func_02001158(18);
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

  Actor_SetSingleChildFlags(Actor_Get(19), 0);
  Actor_SetSingleChildFlags(Actor_Get(20), 0);
  Actor_SetSpriteColor(Actor_Get(20), 4);
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(106),
    SCENE_EXIT_IF(1, 105, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(2, 105, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(3, 105, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(4, 105, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(5, 105, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(6, 106, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(7, 106, 6, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    {115, CONDITION_ALWAYS, 2, PIXELS(120), 0, PIXELS(104), FACING_SOUTHWEST, 1,
     0},
    {116, CONDITION_ALWAYS, 1, PIXELS(88), 0, PIXELS(360), FACING_SOUTHEAST, 1,
     0},
    {25, CONDITION_ALWAYS, 1, PIXELS(672), 0, PIXELS(94), FACING_SOUTH, 1, 0},
    {106, CONDITION_ALWAYS, 1, PIXELS(651), 0, PIXELS(106), FACING_SOUTHEAST, 1,
     0},
    {107, CONDITION_ALWAYS, 1, PIXELS(693), 0, PIXELS(106), FACING_SOUTH, 1, 0},
    {108, CONDITION_ALWAYS, 1, PIXELS(392), 0, PIXELS(104), FACING_SOUTH, 1, 0},
    {109, CONDITION_ALWAYS, 1, PIXELS(408), 0, PIXELS(104), FACING_SOUTH, 1, 0},
    {108, CONDITION_ALWAYS, 2, PIXELS(424), 0, PIXELS(360), FACING_SOUTHWEST, 1,
     0},
    {135, CONDITION_ALWAYS, 1, PIXELS(328), 0, PIXELS(368), FACING_SOUTH, 1, 0},
    {489, CONDITION_ALWAYS, 1, PIXELS(672), 0, PIXELS(112), FACING_SOUTH, 0, 0},
    {489, CONDITION_ALWAYS, 1, PIXELS(672), 0, PIXELS(104), FACING_SOUTH, 0, 0},
    {262, CONDITION_ALWAYS, 1, PIXELS(672), 0, PIXELS(104), FACING_SOUTH, 0, 0},
    {285, CONDITION_ALWAYS, 1, PIXELS(672), 0, PIXELS(104), FACING_SOUTH, 0, 0},
    {SCENE_TABLE_END},
};

struct Trigger gData_02001370[] = {
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
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, EVENT(Func_020000c4)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, EVENT(Func_02000184)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgIfYouLike)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, EVENT(Func_02000244)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, EVENT(Func_02000304)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgTheFortuneTellerIn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgAsLongAs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgTheItemsThat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgMyFatherMay)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgBodyPaintingAlso)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgICanLook)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgWheneverIGetMy)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgTheRiverIs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgOurSpicesAre)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, EVENT(Func_02000054)},
    {TRIGGERS_END},
};

struct Trigger gData_020014c0[] = {
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
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, EVENT(Func_02000124)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, EVENT(Func_020001e4)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgIfIsTraveling)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, EVENT(Func_020002a4)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, EVENT(Func_02000304)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgThePowerOf)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgTheFortuneTellerSaid)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgItemsPlacedBefore)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgFatherShouldCharge)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgAfterFatherS)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgWhenILook)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgILikeOffering)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgHePaidMe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgWhyWouldTravel)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, EVENT(Func_0200007c)},
    {TRIGGERS_END},
};
