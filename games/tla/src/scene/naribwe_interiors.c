/// @file
/// Naribwe's houses, among them the fortune teller's hut, where for 20 coins
/// he reads an item the party sets on his table: a weapon for the foe ahead,
/// armor for an unfound Djinni, a prong for the foe it masters, and other items
/// for the path to follow.

#include "flags.h"
#include "graphics/palette_buffers.h"
#include "system/io_reg.h"
#include "sprite_ids.h"
#include "window/message_argument.h"
#include "shared/tables.h"
#include "field/field.h"
#include "battle/battle.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "field/scene_tables.h"

/// Naribwe's houses, by the rows of gActorPlacements.
enum NaribweInteriorsActorId {
  /// "The fortune-teller lives at the highest house in Naribwe..."
  ACTOR_MAN_DIRECTING_TO_THE_FORTUNE_TELLER = ACTOR_FIRST_PLACED,
  /// "We're a small village, but if we all cooperate..."
  ACTOR_WOMAN_ON_COOPERATION,
  ACTOR_FORTUNE_TELLER,
  /// "My father is a fortune-teller, not a witch doctor."
  ACTOR_FORTUNE_TELLERS_CHILD,
  /// "By painting our bodies, we bring about good luck."
  ACTOR_VILLAGER_ON_BODY_PAINT,
  ACTOR_WEAPON_SHOPKEEPER,
  ACTOR_ARMOR_SHOPKEEPER,
  /// "If you like adventure, you should head out west across the river..."
  ACTOR_VILLAGER_ON_ADVENTURE,
  ACTOR_INNKEEPER,
  /// The spot across the fortune-teller's table that talking to him
  /// answers, and the marker of his reading, its flame and its smoke.
  ACTOR_FORTUNE_TABLE,
  ACTOR_FORTUNE_MARKER,
  ACTOR_FORTUNE_FLAME,
  ACTOR_FORTUNE_SMOKE
};
#include "field/field_sequence_event.h"
#include "scene/scene.h"
#include "field/event_inline.h"
#include "system/main_routine_work.h"
#include "field/actor.h"
#include "version.h"
#include "field/call.h"
#include "system/ram_buffer.h"
#include "item_ids.h"

extern struct Trigger gNaribweInteriors_Triggers[];
extern struct Trigger gNaribweInteriors_TriggersOnReturn[];

void NaribweInteriors_ReadFortune(void);

s32 ItemPicker_RunToShow(s32 *unit, s32 *slot);

void NaribweInteriors_ReadProng(void);

void NaribweInteriors_ReadWeapon(void);

void NaribweInteriors_ReadArmor(void);

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

#include "text/msg_ids.h"

/// "The items that one carries are filled with one's hopes and desires."
s32 NaribweInteriors_MindReadFortuneTeller(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgNaribweInteriorsItemsThatOneCarries);
  // "The items that one carries are filled with one's hopes and desires...."
  Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
  Event_End();
  return 0;
}

s32 NaribweInteriors_MindReadFortuneTellerOnReturn(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgNaribweInteriorsItemsPlacedBeforeMe);
  // "Items placed before me are filled with the desires of those who..."
  Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
  Event_End();
  return 0;
}

const struct Trigger *Scene_GetTriggers(void) {
  if (Event_GetFlag(FLAG_GABOMBA_MESSAGE_CARRIED))
    return gNaribweInteriors_TriggersOnReturn;
  return gNaribweInteriors_Triggers;
}

/// Naribwe's shops, as Shop_Run numbers them.
enum { SHOP_NARIBWE_WEAPONS = 14, SHOP_NARIBWE_ARMOR = 15 };

/// Naribwe's inn, as Inn_CheckIn numbers the inns.
enum { NARIBWE_INN = 5 };

void NaribweInteriors_RecommendFortuneTeller(s32 actor) {
  u16 facing = Actor_Get(gMainRoutineWork.leaderActorId)->facing;

  if (FACING_NEAREST_CARDINAL(facing) == FACING_NORTH) {
    Shop_Run(SHOP_NARIBWE_WEAPONS, actor);
  } else {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    FieldEvent_SetMessageId((s32)MsgNaribweInteriorsIfYouAreBurdened);
    // "If you are burdened with many concerns, take a weapon to the..."
    Event_ShowMessage(actor, 0);
    Event_End();
  }
}

void NaribweInteriors_RecommendFortuneTellerOnReturn(s32 actor) {
  u16 facing = Actor_Get(gMainRoutineWork.leaderActorId)->facing;

  if (FACING_NEAREST_CARDINAL(facing) == FACING_NORTH) {
    Shop_Run(SHOP_NARIBWE_WEAPONS, actor);
  } else {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    FieldEvent_SetMessageId((s32)MsgNaribweInteriorsWheneverIGetCustomers);
    // "Whenever I get customers from out of town, I always tell them the..."
    Event_ShowMessage(actor, 0);
    Event_End();
  }
}

void NaribweInteriors_SendToFortuneTeller(s32 actor) {
  u16 facing = Actor_Get(gMainRoutineWork.leaderActorId)->facing;

  if (FACING_NEAREST_CARDINAL(facing) == FACING_NORTH) {
    Shop_Run(SHOP_NARIBWE_ARMOR, actor);
  } else {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    FieldEvent_SetMessageId(
        (s32)MsgNaribweInteriorsYouShouldVisitFortuneTeller);
    // "You should visit the fortune- teller while you are in our village...."
    Event_ShowMessage(actor, 0);
    Event_End();
  }
}

void NaribweInteriors_SendToFortuneTellerOnReturn(s32 actor) {
  u16 facing = Actor_Get(gMainRoutineWork.leaderActorId)->facing;

  if (FACING_NEAREST_CARDINAL(facing) == FACING_NORTH) {
    Shop_Run(SHOP_NARIBWE_ARMOR, actor);
  } else {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    FieldEvent_SetMessageId((s32)MsgNaribweInteriorsIfYouWishToKnow);
    // "If you wish to know anything, go to the fortune-teller. He's fast..."
    Event_ShowMessage(actor, 0);
    Event_End();
  }
}

/// "Our cooking is famous all across Gondowan."
void NaribweInteriors_TalkAboutCooking(s32 actor) {
  u16 facing = Actor_Get(gMainRoutineWork.leaderActorId)->facing;

  if (FACING_NEAREST_CARDINAL(facing) == FACING_NORTH) {
    Inn_CheckIn(NARIBWE_INN, actor);
  } else {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    FieldEvent_SetMessageId((s32)MsgNaribweInteriorsOurCookingIsFamousAll);
    // "Our cooking is famous all across Gondowan. If you find you like it..."
    Event_ShowMessage(actor, 0);
    Event_End();
  }
}

void NaribweInteriors_TalkAboutTravelers(s32 actor) {
  u16 facing = Actor_Get(gMainRoutineWork.leaderActorId)->facing;

  if (FACING_NEAREST_CARDINAL(facing) == FACING_NORTH) {
    Inn_CheckIn(NARIBWE_INN, actor);
  } else {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    FieldEvent_SetMessageId((s32)MsgNaribweInteriorsISeeSoManyTravelers);
    // "I see so many travelers... How am I supposed to remember who Piers..."
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
  message = MsgNaribweInteriorsIAmTellerOfFortunes;
  FieldEvent_SetMessageId((s32)message);
  Event_OpenMessage(ACTOR_FORTUNE_TELLER, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) ==
      0) {
    FieldEvent_SetMessageId((s32)(message + 2));
    Event_OpenMessage(ACTOR_FORTUNE_TELLER, 0);
    if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) ==
        0) {
      if ((u32)gMainRoutineWork.coins > 19) {
        NaribweInteriors_ReadFortune();
        Event_End();
      } else {
        FieldEvent_SetMessageId((s32)(message + 4));
        Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
        Event_End();
      }
      return;
    }
  }
  FieldEvent_SetMessageId((s32)MsgNaribweInteriorsThenStandBack);
  // "Then stand back."
  Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
  Event_End();
}

struct Unit *Unit_Get(u32 unitId);
void NaribweInteriors_ReadShiningItem(void);

/// Object 10 steps to (x, 96) without turning, then dances (mode 5).
#define DANCE_TO(x)                                                            \
  (Actor_Get(ACTOR_FORTUNE_TELLER)->steeringFlags &= ~1,                       \
   Actor_WalkTo(ACTOR_FORTUNE_TELLER, x, 96),                                  \
   Actor_SetAnimation(ACTOR_FORTUNE_TELLER, 5))

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

  FieldEvent_SetMessageId(MsgNaribweInteriorsVeryWellShowMeOne);
  // "Very well. Show me one of your items."
  Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
  FieldEvent_Wait(30);
ask:
#if !TLA_PATCH_1
  Actor_SetAnimationAndWait(gMainRoutineWork.leaderActorId, ANIM_NOD);
  // Before patch 1 the item variable first holds the choice's result.
  item = ItemPicker_RunToShow(&unit, &slot);
  if (item == -1) {
#else
  Actor_SetAnimationAndWait(gMainRoutineWork.leaderActorId, ANIM_NOD);
  if (ItemPicker_RunToShow(&unit, &slot) == -1) {
#endif
    refuse = MsgNaribweInteriorsShallIGiveYouAnother;
    FieldEvent_SetMessageId(refuse);
    Event_OpenMessage(ACTOR_FORTUNE_TELLER, 0);
    if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) !=
        1)
      goto ask;
    FieldEvent_SetMessageId(refuse + 1);
    Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
    return;
  }
  unitData = Unit_Get(unit);
  Item_GetDirect(unitData->items[slot]);
  Actor_SetItemIcon(Actor_Get(ACTOR_FORTUNE_MARKER), unitData->items[slot]);
  Actor_SetPosition(ACTOR_FORTUNE_MARKER, 672, 104);
  FieldEvent_Wait(60);
  MessageArgument_Push(unitData->items[slot], MESSAGE_ARGUMENT_ITEM);
  Message_ShowCentered(MsgPlacedOnThe, 1);
  message = MsgNaribweInteriorsAreYouPrepared;
  FieldEvent_SetMessageId(message);
  Event_OpenMessage(ACTOR_FORTUNE_TELLER, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) !=
      1)
    goto read;
  Actor_SetPosition(ACTOR_FORTUNE_MARKER, 0, 0);
  MessageArgument_Push(unitData->items[slot], MESSAGE_ARGUMENT_ITEM);
  Message_ShowCentered(message + 1, 1);
  FieldEvent_SetMessageId(message + 2);
  Event_OpenMessage(ACTOR_FORTUNE_TELLER, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) !=
      1)
    goto ask;
  FieldEvent_SetMessageId(message + 3);
  Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
  return;
read:
  FieldEvent_SetMessageId(message + 4);
  Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
  PaletteFade_ApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
  FieldEvent_Wait(30);
  PaletteFade_ApplyTarget(PALETTE_TINT_BLEND | RGB(13, 8, 9), PALETTES_BG);
  PaletteFade_Interpolate(30);
  FieldEvent_Wait(120);
  Event_PlaySound(MUS_KIBOMBO);
  Actor_SetAnimation(ACTOR_FORTUNE_TELLER, 5);
  Actor_WaitForAnimationChange(ACTOR_FORTUNE_TELLER);
  DANCE_TO(664);
  FieldEvent_Wait(30);
  DANCE_TO(680);
  FieldActor_Jump(ACTOR_FORTUNE_TELLER, 4, 90);
  DANCE_TO(664);
  FieldEvent_Wait(30);
  DANCE_TO(680);
  FieldActor_Jump(ACTOR_FORTUNE_TELLER, 4, 90);
  DANCE_TO(672);
  FieldActor_Jump(ACTOR_FORTUNE_TELLER, 4, 30);
  FieldActor_Jump(ACTOR_FORTUNE_TELLER, 4, 30);
  Actor_SetAnimation(ACTOR_FORTUNE_TELLER, 5);
  Actor_SetAnimation(ACTOR_FORTUNE_TELLER, 0);
  Event_PlaySound(MUS_KIBOMBO);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
  PaletteFade_Interpolate(30);
  Actor_SetPosition(ACTOR_FORTUNE_SMOKE, 672, 104);
  Event_PlaySound(SE_CLATTER);
  Actor_SetAnimation(ACTOR_FORTUNE_SMOKE, 2);
  Actor_WaitForAnimationChange(ACTOR_FORTUNE_SMOKE);
  Actor_SetPosition(ACTOR_FORTUNE_SMOKE, 0, 0);
  FieldEvent_Wait(4);
  Actor_SetPosition(ACTOR_FORTUNE_FLAME, 672, 104);
  Actor_SetAnimation(ACTOR_FORTUNE_FLAME, 4);
  Actor_WaitForAnimationChange(ACTOR_FORTUNE_FLAME);
  FieldEvent_Wait(15);
  Actor_SetPosition(ACTOR_FORTUNE_FLAME, 0, 0);
  FieldEvent_Wait(30);
  Actor_ShowEmote(gMainRoutineWork.leaderActorId, EMOTE_IN_FRONT | EMOTE_WINCE,
                  0);
  FieldEvent_Wait(60);
  Event_PlaySound(MUS_NARIBWE);
  Actor_ShowEmote(ACTOR_FORTUNE_TELLER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
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
    Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
    if (!Event_GetFlag(0x940)) {
      FieldEvent_SetMessageId(message + 30);
      Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
    } else {
      FieldEvent_SetMessageId(message + 31);
      Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
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
  Actor_SetPosition(ACTOR_FORTUNE_MARKER, 0, 0);
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
  u8 *message = MsgNaribweInteriorsBeyondWeaponYouSetDown;

  FieldEvent_SetMessageId((s32)message);
  Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
  switch (NaribweInteriors_FoeAhead()) {
  case 0:
    FieldEvent_SetMessageId((s32)(message + 1));
    Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
    break;
  case 1:
    FieldEvent_SetMessageId((s32)(message + 2));
    Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
    break;
  case 2:
    FieldEvent_SetMessageId((s32)(message + 3));
    Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
    break;
  }
}

/// An item of kind 6: a power shining beyond it, and where the story goes next.
/// "I feel a shining power glowing beyond the item before me."
void NaribweInteriors_ReadShiningItem(void) {
  u8 *message = MsgNaribweInteriorsIFeelShiningPowerGlowing;

  FieldEvent_SetMessageId((s32)message);
  Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
  switch (NaribweInteriors_NextPowerToGain()) {
  case 3:
    FieldEvent_SetMessageId((s32)MsgNaribweInteriorsYouAreLost);
    // "You are lost in your way... Another continent needs your attention..."
    Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
    break;
  case 4:
    FieldEvent_SetMessageId((s32)MsgNaribweInteriorsDistantIsleLiesFar);
    // "A distant isle lies far to the northeast... Sail there. High atop a..."
    Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
    break;
  case 5:
    FieldEvent_SetMessageId((s32)MsgNaribweInteriorsISeeParadiseAwaiting);
    // "I see a paradise awaiting you... An island in the east. But even..."
    Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
    break;
  case 6:
    FieldEvent_SetMessageId(
        (s32)MsgNaribweInteriorsTinyFigureStandsAgainstWasteland);
    // "A tiny figure stands against a wasteland wrought in snow... The..."
    Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
    break;
  case 7:
    FieldEvent_SetMessageId((s32)MsgNaribweInteriorsISeeYouLost);
    // "I see you lost in mists, lost in time, lost in the seas... And..."
    Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
    break;
  default:
    FieldEvent_SetMessageId((s32)(message + 6));
    Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
    break;
  }
}

/// A piece of armor, kinds 2 to 5: no future, and where an unfound Djinni
/// waits.
/// "In the item you have placed before me, I can see nothing of the future..."
void NaribweInteriors_ReadArmor(void) {
  s32 message = NaribweInteriors_GetDjinniHint();

  if (message == (s32)MsgNaribweInteriorsInItemYouHavePlaced) {
    FieldEvent_SetMessageId(message);
    Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
  } else {
    FieldEvent_SetMessageId((s32)MsgNaribweInteriorsInItemYouHavePlaced - 42);
    Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
    FieldEvent_SetMessageId(message);
    Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
  }
}

/// Any other item: the path the party should follow.
/// "Beyond the item before me, I see a path for you to follow."
void NaribweInteriors_ReadOtherItem(void) {
  FieldEvent_SetMessageId((s32)MsgNaribweInteriorsBeyondItemBeforeMe);
  // "Beyond the item before me, I see a path for you to follow."
  Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
  switch (NaribweInteriors_NextPath()) {
  case 0:
    FieldEvent_SetMessageId((s32)MsgNaribweInteriorsISeeYouInsideSacred);
    // "I see you inside a sacred icon. By listening to the voices, you will..."
    Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
    break;
  case 9:
    FieldEvent_SetMessageId((s32)MsgNaribweInteriorsYouMustForgeWeapon);
    // "You must forge a weapon to defeat a mortal enemy. Seek out the pieces!"
    Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
    break;
  case 10:
    FieldEvent_SetMessageId((s32)MsgNaribweInteriorsYouWillRaiseMightyColumn);
    // "You will raise a mighty column and gain assistance where it is least..."
    Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
    break;
  case 11:
    FieldEvent_SetMessageId((s32)MsgNaribweInteriorsYouWillGainTremendousHelp);
    // "You will gain tremendous help from a village on the eastern reaches..."
    Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
    break;
  case 12:
    FieldEvent_SetMessageId((s32)MsgNaribweInteriorsYouHaveWeaponYouNeed);
    // "You have the weapon you need to defeat the sea beast. Go to the sea..."
    Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
    break;
  case 13:
    FieldEvent_SetMessageId((s32)MsgNaribweInteriorsObjectThatYouSeekLies);
    // "The object that you seek lies somewhere in the west. Why do you..."
    Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
    break;
  }
}

/// A prong of the Trident: the foe it holds power over.
/// "The object you have placed before me wields a mighty power over a terrible
/// foe."
void NaribweInteriors_ReadProng(void) {
  u8 *message = MsgNaribweInteriorsObjectYouHavePlacedBefore;

  FieldEvent_SetMessageId((s32)message);
  Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
  switch (NaribweInteriors_ProngsHeld()) {
  case 14:
    FieldEvent_SetMessageId((s32)(message + 1));
    Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
    break;
  case 15:
    FieldEvent_SetMessageId((s32)(message + 2));
    Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
    break;
  case 16:
    FieldEvent_SetMessageId((s32)(message + 3));
    Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
    break;
  case 17:
    FieldEvent_SetMessageId((s32)(message + 4));
    Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
    break;
  }
}

/// Which foe a weapon's reading names: 0 before the return with Piers, then 1,
/// and 2 once flag 0x940 is set.
s32 NaribweInteriors_FoeAhead(void) {
  if (Event_GetFlag(FLAG_GABOMBA_MESSAGE_CARRIED) == 0)
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
  if (FieldPartyInventory_FindMember(ITEM_BURST_BROOCH) == -1)
    return 6;
  if (FieldPartyInventory_FindMember(ITEM_GRINDSTONE) == -1)
    return 7;
  return 8;
}

/// The path an ordinary item's reading points along, by the story's flags and
/// the Trident's prongs (items 439 to 441) and the Trident (item 326).
s32 NaribweInteriors_NextPath(void) {
  if (Event_GetFlag(FLAG_GABOMBA_MESSAGE_CARRIED) == 0)
    return 0;
  if (FieldPartyInventory_FindMember(ITEM_TRIDENT) == -1) {
    if (Event_GetFlag(0x940) == 0) {
      if (FieldPartyInventory_FindMember(ITEM_RIGHT_PRONG) == -1 ||
          FieldPartyInventory_FindMember(ITEM_LEFT_PRONG) == -1 ||
          FieldPartyInventory_FindMember(ITEM_CENTER_PRONG) == -1)
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
  if (FieldPartyInventory_FindMember(ITEM_RIGHT_PRONG) == -1 &&
      (FieldPartyInventory_FindMember(ITEM_CENTER_PRONG) != -1 ||
       FieldPartyInventory_FindMember(ITEM_LEFT_PRONG) != -1))
    return 14;
  if (FieldPartyInventory_FindMember(ITEM_RIGHT_PRONG) != -1 &&
      FieldPartyInventory_FindMember(ITEM_LEFT_PRONG) == -1 &&
      FieldPartyInventory_FindMember(ITEM_CENTER_PRONG) == -1)
    return 15;
  if (FieldPartyInventory_FindMember(ITEM_RIGHT_PRONG) != -1 &&
      FieldPartyInventory_FindMember(ITEM_LEFT_PRONG) == -1 &&
      FieldPartyInventory_FindMember(ITEM_CENTER_PRONG) != -1)
    return 15;
  if (FieldPartyInventory_FindMember(ITEM_RIGHT_PRONG) != -1 &&
      FieldPartyInventory_FindMember(ITEM_LEFT_PRONG) != -1 &&
      FieldPartyInventory_FindMember(ITEM_CENTER_PRONG) == -1)
    return 16;
  if (FieldPartyInventory_FindMember(ITEM_RIGHT_PRONG) != -1 &&
      FieldPartyInventory_FindMember(ITEM_LEFT_PRONG) != -1 &&
      FieldPartyInventory_FindMember(ITEM_CENTER_PRONG) != -1)
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

/// The message telling where an unfound Djinni waits, or
/// MsgNaribweInteriorsInItemYouHavePlaced when none is left to find.
u8 *NaribweInteriors_GetDjinniHint(void) {
  s32 *hints = (s32 *)Value2((s32 (*)())HeapSlot_AllocEwram,
                             HEAP_SLOT_DJINNI_HINTS, 320);
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
      HeapSlot_Release(HEAP_SLOT_DJINNI_HINTS);
      return MsgNaribweInteriorsInItemYouHavePlaced;
    }
    NaribweInteriors_ListUnfoundDjinn(18, lowestGroup, hints);
    index = 0;
  } else {
    index = Random16() % count;
  }
  switch (hints[index]) {
  case 56:
    message = MsgNaribweInteriorsToEastLiesContinentMoved;
    break;
  case 57:
    message = MsgNaribweInteriorsToNorthInsideSacredIcon;
    break;
  case 58:
    message = MsgNaribweInteriorsToNorthBeneathSacredIcon;
    break;
  case 59:
    message = MsgNaribweInteriorsHeadEastBySoutheast;
    break;
  case 60:
    message = MsgNaribweInteriorsHeadEastByNortheast;
    break;
  case 61:
    message = MsgNaribweInteriorsHeadNorthByNorthwest;
    break;
  case 62:
    message = MsgNaribweInteriorsToNorthwestInRuins;
    break;
  case 63:
    message = MsgNaribweInteriorsHeadWestByNorthwest;
    break;
  case 64:
    message = MsgNaribweInteriorsToNorthInVillage;
    break;
  case 65:
    message = MsgNaribweInteriorsHeadEastBySoutheastInDesert;
    break;
  case 75:
    message = MsgNaribweInteriorsHeadEastByNortheastInCave;
    break;
  case 76:
    message = MsgNaribweInteriorsToSoutheastInPlains;
    break;
  case 79:
    message = MsgNaribweInteriorsToSouthInForestOne;
    break;
  case 80:
    message = MsgNaribweInteriorsToEastBesideBoulder;
    break;
  case 81:
    message = MsgHeadEastByNortheastInThe;
    break;
  case 82:
    message = MsgNaribweInteriorsToNorthSomewhereOnSmall;
    break;
  case 83:
    message = MsgNaribweInteriorsHeadNorthByNorthwestInCave;
    break;
  case 84:
    message = MsgNaribweInteriorsToNorthInRuinsChilled;
    break;
  case 85:
    message = MsgHeadEastByNortheastInA;
    break;
  case 95:
    message = MsgNaribweInteriorsToEastInMountainPaths;
    break;
  case 96:
    message = MsgNaribweInteriorsToSoutheastInVillageLulled;
    break;
  case 97:
    message = MsgNaribweInteriorsHeadSouthBySoutheast;
    break;
  case 98:
    message = MsgNaribweInteriorsHeadEastBySoutheastInTown;
    break;
  case 99:
    message = MsgNaribweInteriorsHeadEastByNortheastOnIsland;
    break;
  case 100:
    message = MsgNaribweInteriorsHeadSouthBySoutheastInRuins;
    break;
  case 101:
    message = MsgNaribweInteriorsToNorthwestInForest;
    break;
  case 102:
    message = MsgNaribweInteriorsToNorthwestAlongCoast;
    break;
  case 103:
    message = MsgNaribweInteriorsToNorthwestBesideRuins;
    break;
  case 104:
    message = MsgNaribweInteriorsHeadNorthByNorthwestAtGreat;
    break;
  case 105:
    message = MsgNaribweInteriorsToNorthInRuinsChilledByIcy;
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
    message = MsgNaribweInteriorsToEastSomewhereOnTiny;
    break;
  case 120:
    message = MsgNaribweInteriorsToEastInForest;
    break;
  case 121:
    message = MsgNaribweInteriorsHeadNorthByNorthwestInVillage;
    break;
  case 122:
    message = MsgNaribweInteriorsInNorthwestInRuins;
    break;
  case 123:
    message = MsgNaribweInteriorsHeadNorthByNorthwestNear;
    break;
  case 124:
    message = MsgToTheNorthInTheRuins;
    break;
  case 125:
    message = MsgNaribweInteriorsHeadNorthByNortheast;
    break;
  default:
    message = MsgNaribweInteriorsInItemYouHavePlaced;
    break;
  }
  HeapSlot_Release(HEAP_SLOT_DJINNI_HINTS);
  return message;
}

void Actor_DisableById(s32 id);

s32 Scene_Initialize(void) {
  struct Actor *object;

  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  Actor_Get(gMainRoutineWork.leaderActorId)->priorityFlags |= 0x20;
  Actor_DisableById(ACTOR_FORTUNE_TABLE);
  Actor_DisableById(ACTOR_FORTUNE_MARKER);
  FieldActor_SetSpritePriority(ACTOR_FORTUNE_TELLER, 2);
  Actor_Get(ACTOR_VILLAGER_ON_BODY_PAINT)->collisionFlags |= ACTOR_NEAR_TALK;
  FieldActor_SetSpritePriority(ACTOR_INNKEEPER, 2);
  Actor_Get(ACTOR_INNKEEPER)->collisionFlags |= ACTOR_NEAR_TALK;

  object = Actor_Get(ACTOR_FORTUNE_MARKER);
  object->motionFlags = 0;
  object->y.fixed = -px(4);
  object->groundHeight = -px(4);
  Actor_Get(ACTOR_FORTUNE_MARKER)->priorityFlags |= 0x20;
  FieldActor_SetSpritePriority(ACTOR_FORTUNE_MARKER, 1);
  Actor_SetPosition(ACTOR_FORTUNE_MARKER, 0, 0);

  object = Actor_Get(ACTOR_FORTUNE_FLAME);
  object->motionFlags = 0;
  object->y.fixed = px(12);
  object->groundHeight = px(12);
  Actor_Get(ACTOR_FORTUNE_FLAME)->priorityFlags |= 0x20;
  FieldActor_SetSpritePriority(ACTOR_FORTUNE_FLAME, 1);
  Actor_SetPosition(ACTOR_FORTUNE_FLAME, 0, 0);

  object = Actor_Get(ACTOR_FORTUNE_SMOKE);
  object->motionFlags = 0;
  object->y.fixed = px(12);
  object->groundHeight = px(12);
  Actor_Get(ACTOR_FORTUNE_SMOKE)->priorityFlags |= 0x20;
  FieldActor_SetSpritePriority(ACTOR_FORTUNE_SMOKE, 1);
  Actor_SetPosition(ACTOR_FORTUNE_SMOKE, 0, 0);

  Actor_SetSpriteFlags(Actor_Get(ACTOR_FORTUNE_FLAME), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_FORTUNE_SMOKE), 0);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_FORTUNE_SMOKE), 4);
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
    ACTOR_PLACEMENT(SPRITE_FLAME_AND_SMOKE, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 672, 0, 104, FACING_SOUTH,
                    TALK_FACING_PARTY, 0),
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
    {TRIGGER_TALK, ACTOR_MAN_DIRECTING_TO_THE_FORTUNE_TELLER, CONDITION_ALWAYS,
     MSGNO(MsgNaribweInteriorsFortuneTellerLivesAtHighest)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_COOPERATION, CONDITION_ALWAYS,
     MSGNO(MsgNaribweInteriorsWereSmallVillage)},
    {TRIGGER_TALK, ACTOR_FORTUNE_TELLER, CONDITION_ALWAYS,
     MSGNO(MsgNaribweInteriorsIfYouDesireReadingStand)},
    {TRIGGER_TALK, ACTOR_FORTUNE_TELLERS_CHILD, CONDITION_ALWAYS,
     MSGNO(MsgNaribweInteriorsMyFatherIsFortuneTeller)},
    {TRIGGER_TALK, ACTOR_VILLAGER_ON_BODY_PAINT, CONDITION_ALWAYS,
     MSGNO(MsgNaribweInteriorsByPaintingOurBodies)},
    {TRIGGER_TALK, ACTOR_WEAPON_SHOPKEEPER, CONDITION_ALWAYS,
     EVENT(NaribweInteriors_RecommendFortuneTeller)},
    {TRIGGER_TALK, ACTOR_ARMOR_SHOPKEEPER, CONDITION_ALWAYS,
     EVENT(NaribweInteriors_SendToFortuneTeller)},
    {TRIGGER_TALK, ACTOR_VILLAGER_ON_ADVENTURE, CONDITION_ALWAYS,
     MSGNO(MsgNaribweInteriorsIfYouLikeAdventure)},
    {TRIGGER_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS,
     EVENT(NaribweInteriors_TalkAboutCooking)},
    {TRIGGER_TALK, ACTOR_FORTUNE_TABLE, CONDITION_ALWAYS,
     EVENT(NaribweInteriors_TalkToFortuneTeller)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_MAN_DIRECTING_TO_THE_FORTUNE_TELLER, CONDITION_ALWAYS,
     MSGNO(MsgNaribweInteriorsFortuneTellerInNaribweHas)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_COOPERATION,
     CONDITION_ALWAYS, MSGNO(MsgNaribweInteriorsAsLongAsWeListen)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FORTUNE_TELLER,
     CONDITION_ALWAYS, MSGNO(MsgNaribweInteriorsItemsThatOneCarries)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FORTUNE_TELLERS_CHILD,
     CONDITION_ALWAYS, MSGNO(MsgNaribweInteriorsMyFatherMayNot)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_BODY_PAINT,
     CONDITION_ALWAYS, MSGNO(MsgNaribweInteriorsBodyPaintingAlsoProtectsUs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_SHOPKEEPER,
     CONDITION_ALWAYS, MSGNO(MsgNaribweInteriorsICanLookAtAny)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_SHOPKEEPER,
     CONDITION_ALWAYS, MSGNO(MsgNaribweInteriorsWheneverIGetMyFortune)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_ADVENTURE,
     CONDITION_ALWAYS, MSGNO(MsgNaribweInteriorsRiverIsProbablyVeryDangerous)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS,
     MSGNO(MsgNaribweInteriorsOurSpicesAreOurSecret)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FORTUNE_TABLE,
     CONDITION_ALWAYS, EVENT(NaribweInteriors_MindReadFortuneTeller)},
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
    {TRIGGER_TALK, ACTOR_MAN_DIRECTING_TO_THE_FORTUNE_TELLER, CONDITION_ALWAYS,
     MSGNO(MsgNaribweInteriorsIGuessThoseWarriorsLearned)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_COOPERATION, CONDITION_ALWAYS,
     MSGNO(MsgNaribweInteriorsOurFortuneTellerHasBeen)},
    {TRIGGER_TALK, ACTOR_FORTUNE_TELLER, CONDITION_ALWAYS,
     MSGNO(MsgNaribweInteriorsIfYouDesireReadingStand)},
    {TRIGGER_TALK, ACTOR_FORTUNE_TELLERS_CHILD, CONDITION_ALWAYS,
     MSGNO(MsgNaribweInteriorsIfTroubleResides)},
    {TRIGGER_TALK, ACTOR_VILLAGER_ON_BODY_PAINT, CONDITION_ALWAYS,
     MSGNO(MsgNaribweInteriorsReadingFortunesInPeoplesItems)},
    {TRIGGER_TALK, ACTOR_WEAPON_SHOPKEEPER, CONDITION_ALWAYS,
     EVENT(NaribweInteriors_RecommendFortuneTellerOnReturn)},
    {TRIGGER_TALK, ACTOR_ARMOR_SHOPKEEPER, CONDITION_ALWAYS,
     EVENT(NaribweInteriors_SendToFortuneTellerOnReturn)},
    {TRIGGER_TALK, ACTOR_VILLAGER_ON_ADVENTURE, CONDITION_ALWAYS,
     MSGNO(MsgIfIsTraveling)},
    {TRIGGER_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS,
     EVENT(NaribweInteriors_TalkAboutTravelers)},
    {TRIGGER_TALK, ACTOR_FORTUNE_TABLE, CONDITION_ALWAYS,
     EVENT(NaribweInteriors_TalkToFortuneTeller)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_MAN_DIRECTING_TO_THE_FORTUNE_TELLER, CONDITION_ALWAYS,
     MSGNO(MsgNaribweInteriorsPowerOfFortuneTeller)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_COOPERATION,
     CONDITION_ALWAYS, MSGNO(MsgNaribweInteriorsFortuneTellerSaidThatPower)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FORTUNE_TELLER,
     CONDITION_ALWAYS, MSGNO(MsgNaribweInteriorsItemsPlacedBeforeMe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FORTUNE_TELLERS_CHILD,
     CONDITION_ALWAYS, MSGNO(MsgNaribweInteriorsFatherShouldChargeMore)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_BODY_PAINT,
     CONDITION_ALWAYS, MSGNO(MsgNaribweInteriorsAfterFathersBeenReadingAll)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_SHOPKEEPER,
     CONDITION_ALWAYS, MSGNO(MsgNaribweInteriorsWhenILookAtWeapons)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_SHOPKEEPER,
     CONDITION_ALWAYS, MSGNO(MsgNaribweInteriorsILikeOfferingArmor)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_ADVENTURE,
     CONDITION_ALWAYS, MSGNO(MsgNaribweInteriorsHePaidMeLot)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS,
     MSGNO(MsgWhyWouldTravel)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FORTUNE_TABLE,
     CONDITION_ALWAYS, EVENT(NaribweInteriors_MindReadFortuneTellerOnReturn)},
    {TRIGGER_TABLE_END},
};
