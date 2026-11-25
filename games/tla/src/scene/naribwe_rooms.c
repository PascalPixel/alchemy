/// @file
/// The insides of Naribwe's huts, behind the village's five doors.
///
/// The northwest hut's back door leads into the hut beside it, whose own door
/// opens on the fenced back yard. The southwest hut is the shop, its weapon and
/// armor merchants side by side behind one mat, and the southeast hut the inn.
/// At the top of the village is the fortune-teller's hut, where for 20 coins he
/// reads an item the party sets on his table: a weapon for the foe ahead, a
/// piece of armor for a Djinni still to be found, a prong of the Trident for
/// the prong still missing, the Trident for the beast it was forged against, a
/// Psynergy item for the next power to gain, and anything else for the path to
/// follow.
///
/// Once Piers has carried Gabomba's message (`FLAG_GABOMBA_MESSAGE_CARRIED`)
/// the villagers have other things to say (`gNaribweRooms_TriggersOnReturn`).

#include "flags.h"
#include "ability_ids.h"
#include "djinn_ids.h"
#include "graphics/palette_buffers.h"
#include "system/io_reg.h"
#include "sprite_ids.h"
#include "window/message_argument.h"
#include "shared/tables.h"
#include "field/field.h"
#include "battle/battle.h"
#include "game/game.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "field/scene_tables.h"
#include "field/field_sequence.h"
#include "field/field_sequence_event.h"
#include "scene/scene.h"
#include "field/event_inline.h"
#include "system/main_routine_state.h"
#include "field/actor.h"
#include "version.h"
#include "field/call.h"
#include "system/ram_buffer.h"
#include "item_ids.h"
#include "text/msg_ids.h"
#include "shop/shop.h"

/// The placed actors, numbering the rows of `gActorPlacements`.
enum NaribweRoomsActorId {
  /// "The fortune-teller lives at the highest house in Naribwe..."
  ACTOR_BOY_DIRECTING_TO_THE_FORTUNE_TELLER = ACTOR_FIRST_PLACED,
  /// "We're a small village, but if we all cooperate..."
  ACTOR_WOMAN_ON_A_SMALL_VILLAGE,
  /// "I am a teller of fortunes. Would you have a reading?"
  ACTOR_FORTUNE_TELLER,
  /// "My father is a fortune-teller, not a witch doctor."
  ACTOR_FORTUNE_TELLERS_CHILD,
  /// "By painting our bodies, we bring about good luck."
  ACTOR_VILLAGER_ON_BODY_PAINT,
  /// Behind the swords on the shop's mat; across it, the weapon shop.
  ACTOR_WEAPON_MERCHANT,
  /// Behind the helms and shields beside them; across the mat, the armor shop.
  ACTOR_ARMOR_MERCHANT,
  /// "If you like adventure, you should head out west across the river..."
  ACTOR_VILLAGER_ON_ADVENTURE,
  /// A woman behind the inn's counter; across it, the inn.
  ACTOR_INNKEEPER,
  /// An unseen actor on the fortune-teller's table, which talking to him
  /// across it reaches: "If you desire a reading, stand before the table..."
  ACTOR_FORTUNE_TABLE,
  /// The icon of the item the party sets on the table.
  ACTOR_PLACED_ITEM,
  /// Thin wisps of smoke that rise from the table after the cloud.
  ACTOR_RISING_SMOKE,
  /// A lilac cloud of smoke that bursts on the table as the dance ends.
  ACTOR_SMOKE_CLOUD
};

/// The fortune-teller's dance: arms spread with his staff, the staff raised
/// high, then a crouch, over and over.
#define ANIM_FORTUNE_TELLER_DANCE 5

extern struct Trigger gNaribweRooms_Triggers[];
extern struct Trigger gNaribweRooms_TriggersOnReturn[];

void NaribweRooms_ReadFortune(void);

void NaribweRooms_ReadProng(void);

void NaribweRooms_ReadWeapon(void);

void NaribweRooms_ReadArmor(void);

void NaribweRooms_ReadOtherItem(void);

/// "It costs 20 coins to hear what your future holds. Will you pay?"
#define FORTUNE_PRICE 20

/// The Trident's reading, as messages after MsgNaribweRoomsAreYouPrepared.
enum TridentReadingMessage {
  /// "That object you have placed before me... It is a fearsome weapon to a
  /// particular beast."
  TRIDENT_FEARSOME_WEAPON = 29,
  /// "If you wield this weapon, you can defeat the legendary beast!"
  TRIDENT_DEFEAT_THE_BEAST = 30,
  /// "But there are no more beasts for you to fear in this part of the world."
  TRIDENT_NO_MORE_BEASTS = 31
};

/// "From the garb you have placed before me, I sense the presence of the one
/// you await.", which opens a Djinni's hint, as the message this many before
/// MsgNaribweRoomsInItemYouHavePlaced.
#define ARMOR_PRESENCE_SENSED_BEFORE 42

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

/// Felix casts Mind Read on the fortune-teller across his table, in his hut at
/// the top of Naribwe: "The items that one carries are filled with one's hopes
/// and desires..."
s32 NaribweRooms_MindReadFortuneTeller(void) {
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  FieldEvent_SetMessageId((s32)MsgNaribweRoomsItemsThatOneCarries);
  // "The items that one carries are filled with one's hopes and desires...."
  Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
  Event_End();
  return 0;
}

/// Felix casts Mind Read on the fortune-teller across his table once Piers has
/// carried Gabomba's message: "Items placed before me are filled with the
/// desires of those who..."
s32 NaribweRooms_MindReadFortuneTellerOnReturn(void) {
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  FieldEvent_SetMessageId((s32)MsgNaribweRoomsItemsPlacedBeforeMe);
  // "Items placed before me are filled with the desires of those who..."
  Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
  Event_End();
  return 0;
}

const struct Trigger *Scene_GetTriggers(void) {
  if (Event_GetFlag(FLAG_GABOMBA_MESSAGE_CARRIED))
    return gNaribweRooms_TriggersOnReturn;
  return gNaribweRooms_Triggers;
}

/// The back door between the northwest hut and the west hut, whose own door
/// opens on the back yard: each way's exit, and the entrance it arrives by.
enum {
  NARIBWE_ROOMS_TO_WEST_HUT = 6,
  NARIBWE_ROOMS_TO_NORTHWEST_HUT = 7,
  NARIBWE_ROOMS_FROM_NORTHWEST_HUT = 7,
  NARIBWE_ROOMS_FROM_WEST_HUT = 6
};

/// Naribwe's shops, as `Shop_Run` numbers them.
enum { SHOP_NARIBWE_WEAPONS = 14, SHOP_NARIBWE_ARMOR = 15 };

/// Naribwe's inn, as `Inn_CheckIn` numbers the inns.
enum { NARIBWE_INN = 5 };

/// Felix talks to the weapon merchant behind the swords in Naribwe's shop:
/// across the mat, he can buy; from elsewhere the merchant advises taking a
/// weapon to the fortune-teller.
void NaribweRooms_TalkToWeaponMerchant(s32 speaker) {
  u16 facing = Actor_Get(gMainRoutineState.leaderActorId)->facing;

  if (FACING_NEAREST_CARDINAL(facing) == FACING_NORTH) {
    Shop_Run(SHOP_NARIBWE_WEAPONS, speaker);
  } else {
    Event_Start();
    FieldEvent_EndLastingPsynergy(0);
    FieldEvent_SetMessageId((s32)MsgNaribweRoomsIfYouAreBurdened);
    // "If you are burdened with many concerns, take a weapon to the..."
    Event_ShowMessage(speaker, 0);
    Event_End();
  }
}

/// Felix talks to the weapon merchant in Naribwe's shop once Piers has carried
/// Gabomba's message: across the mat, he can buy; from elsewhere the merchant
/// sends out-of-town customers to the fortune-teller.
void NaribweRooms_TalkToWeaponMerchantOnReturn(s32 speaker) {
  u16 facing = Actor_Get(gMainRoutineState.leaderActorId)->facing;

  if (FACING_NEAREST_CARDINAL(facing) == FACING_NORTH) {
    Shop_Run(SHOP_NARIBWE_WEAPONS, speaker);
  } else {
    Event_Start();
    FieldEvent_EndLastingPsynergy(0);
    FieldEvent_SetMessageId((s32)MsgNaribweRoomsWheneverIGetCustomers);
    // "Whenever I get customers from out of town, I always tell them the..."
    Event_ShowMessage(speaker, 0);
    Event_End();
  }
}

/// Felix talks to the armor merchant behind the helms and shields in Naribwe's
/// shop: across the mat, he can buy; from elsewhere the merchant advises
/// visiting the fortune-teller while in the village.
void NaribweRooms_TalkToArmorMerchant(s32 speaker) {
  u16 facing = Actor_Get(gMainRoutineState.leaderActorId)->facing;

  if (FACING_NEAREST_CARDINAL(facing) == FACING_NORTH) {
    Shop_Run(SHOP_NARIBWE_ARMOR, speaker);
  } else {
    Event_Start();
    FieldEvent_EndLastingPsynergy(0);
    FieldEvent_SetMessageId((s32)MsgNaribweRoomsYouShouldVisitFortuneTeller);
    // "You should visit the fortune- teller while you are in our village...."
    Event_ShowMessage(speaker, 0);
    Event_End();
  }
}

/// Felix talks to the armor merchant in Naribwe's shop once Piers has carried
/// Gabomba's message: across the mat, he can buy; from elsewhere the merchant
/// sends the party to the fortune-teller for anything it wishes to know.
void NaribweRooms_TalkToArmorMerchantOnReturn(s32 speaker) {
  u16 facing = Actor_Get(gMainRoutineState.leaderActorId)->facing;

  if (FACING_NEAREST_CARDINAL(facing) == FACING_NORTH) {
    Shop_Run(SHOP_NARIBWE_ARMOR, speaker);
  } else {
    Event_Start();
    FieldEvent_EndLastingPsynergy(0);
    FieldEvent_SetMessageId((s32)MsgNaribweRoomsIfYouWishToKnow);
    // "If you wish to know anything, go to the fortune-teller. He's fast..."
    Event_ShowMessage(speaker, 0);
    Event_End();
  }
}

/// Felix talks to the woman behind the counter of Naribwe's inn: across it, he
/// can stay the night; from elsewhere she talks of the village's cooking.
void NaribweRooms_TalkToInnkeeper(s32 speaker) {
  u16 facing = Actor_Get(gMainRoutineState.leaderActorId)->facing;

  if (FACING_NEAREST_CARDINAL(facing) == FACING_NORTH) {
    Inn_CheckIn(NARIBWE_INN, speaker);
  } else {
    Event_Start();
    FieldEvent_EndLastingPsynergy(0);
    FieldEvent_SetMessageId((s32)MsgNaribweRoomsOurCookingIsFamousAll);
    // "Our cooking is famous all across Gondowan. If you find you like it..."
    Event_ShowMessage(speaker, 0);
    Event_End();
  }
}

/// Felix talks to the innkeeper once Piers has carried Gabomba's message:
/// across the counter, he can stay the night; from elsewhere she sees too many
/// travelers to remember Piers.
void NaribweRooms_TalkToInnkeeperOnReturn(s32 speaker) {
  u16 facing = Actor_Get(gMainRoutineState.leaderActorId)->facing;

  if (FACING_NEAREST_CARDINAL(facing) == FACING_NORTH) {
    Inn_CheckIn(NARIBWE_INN, speaker);
  } else {
    Event_Start();
    FieldEvent_EndLastingPsynergy(0);
    FieldEvent_SetMessageId((s32)MsgNaribweRoomsISeeSoManyTravelers);
    // "I see so many travelers... How am I supposed to remember who Piers..."
    Event_ShowMessage(speaker, 0);
    Event_End();
  }
}

/// Felix talks to the fortune-teller across his table, in his hut at the top of
/// Naribwe: he offers a reading for 20 coins, and a No to either question ends
/// it.
void NaribweRooms_TalkToFortuneTeller(void) {
  u8 *message;

  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  message = MsgNaribweRoomsIAmTellerOfFortunes;
  FieldEvent_SetMessageId((s32)message);
  // "I am a teller of fortunes. Would you have a reading?"
  Event_OpenMessage(ACTOR_FORTUNE_TELLER, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) ==
      0) {
    FieldEvent_SetMessageId((s32)(message + 2));
    // "It costs 20 coins to hear what your future holds. Will you pay?"
    Event_OpenMessage(ACTOR_FORTUNE_TELLER, 0);
    if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) ==
        0) {
      if (gMainRoutineState.coins > FORTUNE_PRICE - 1) {
        NaribweRooms_ReadFortune();
        Event_End();
      } else {
        FieldEvent_SetMessageId((s32)(message + 4));
        // "Many want things they cannot afford. If you do not have the..."
        Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
        Event_End();
      }
      return;
    }
  }
  FieldEvent_SetMessageId((s32)MsgNaribweRoomsThenStandBack);
  // "Then stand back."
  Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
  Event_End();
}

struct Unit *Unit_Get(u32 unitId);
void NaribweRooms_ReadShiningItem(void);

/// The fortune-teller steps sideways behind his table to x, still facing the
/// party, and dances on.
#define DANCE_TO(x)                                                            \
  (Actor_Get(ACTOR_FORTUNE_TELLER)->steeringFlags &= ~ACTOR_FACES_MOTION,      \
   Actor_WalkTo(ACTOR_FORTUNE_TELLER, x, 96),                                  \
   Actor_SetAnimation(ACTOR_FORTUNE_TELLER, ANIM_FORTUNE_TELLER_DANCE))

/// The reading: the party sets one of a member's items on the table, the room
/// turns red while the fortune-teller dances, and he reads the item by its
/// type, a prong of the Trident and the Trident itself on their own, then
/// takes his 20 coins.
void NaribweRooms_ReadFortune(void) {
  s32 slot, unit;
  struct Unit *unitData;
  const u8 *message, *refuse;
  const struct ItemDefinition *info;
#if !TLA_PATCH_1
  s32 item;
#else
  u32 item;
#endif
  u8 type;
  s32 rawType;

  FieldEvent_SetMessageId(MsgNaribweRoomsVeryWellShowMeOne);
  // "Very well. Show me one of your items."
  Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
  FieldEvent_Wait(30);
  for (;;) {
#if !TLA_PATCH_1
    Actor_SetAnimationAndWait(gMainRoutineState.leaderActorId, ANIM_NOD);
    // Before patch 1 the item variable first holds the choice's result.
    item = ItemPicker_RunToShow(&unit, &slot);
    if (item == -1) {
#else
    Actor_SetAnimationAndWait(gMainRoutineState.leaderActorId, ANIM_NOD);
    if (ItemPicker_RunToShow(&unit, &slot) == -1) {
#endif
      refuse = MsgNaribweRoomsShallIGiveYouAnother;
      FieldEvent_SetMessageId(refuse);
      // "Shall I give you another reading for a different item?"
      Event_OpenMessage(ACTOR_FORTUNE_TELLER, 0);
      if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(),
                                                0) != 1)
        continue;
      FieldEvent_SetMessageId(refuse + 1);
      // "Then away with you."
      Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
      return;
    }
    unitData = Unit_Get(unit);
    Item_GetDirect(unitData->inventory[slot]);
    Actor_SetItemIcon(Actor_Get(ACTOR_PLACED_ITEM), unitData->inventory[slot]);
    Actor_SetPosition(ACTOR_PLACED_ITEM, 672, 104);
    FieldEvent_Wait(60);
    MessageArgument_Push(unitData->inventory[slot], MESSAGE_ARGUMENT_ITEM);
    // "... placed on the table."
    Message_ShowCentered(MsgPlacedOnThe, 1);
    message = MsgNaribweRoomsAreYouPrepared;
    FieldEvent_SetMessageId(message);
    // "Are you prepared?"
    Event_OpenMessage(ACTOR_FORTUNE_TELLER, 0);
    if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) ==
        1) {
      Actor_SetPosition(ACTOR_PLACED_ITEM, 0, 0);
      MessageArgument_Push(unitData->inventory[slot], MESSAGE_ARGUMENT_ITEM);
      // "... changed his mind and took back the item."
      Message_ShowCentered(message + 1, 1);
      FieldEvent_SetMessageId(message + 2);
      // "Shall I give you another reading for a different item?"
      Event_OpenMessage(ACTOR_FORTUNE_TELLER, 0);
      if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(),
                                                0) != 1)
        continue;
      FieldEvent_SetMessageId(message + 3);
      // "Then away with you."
      Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
      return;
    }
    break;
  }
  FieldEvent_SetMessageId(message + 4);
  // "I shall begin my reading..."
  Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
  PaletteFade_ApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
  FieldEvent_Wait(30);
  PaletteFade_ApplyTarget(PALETTE_TINT_BLEND | RGB(13, 8, 9), PALETTES_BG);
  PaletteFade_Interpolate(30);
  FieldEvent_Wait(120);
  Event_PlaySound(MUS_RITUALS);
  Actor_SetAnimation(ACTOR_FORTUNE_TELLER, ANIM_FORTUNE_TELLER_DANCE);
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
  Actor_SetAnimation(ACTOR_FORTUNE_TELLER, ANIM_FORTUNE_TELLER_DANCE);
  Actor_SetAnimation(ACTOR_FORTUNE_TELLER, ANIM_STILL);
  Event_PlaySound(MUS_RITUALS);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
  PaletteFade_Interpolate(30);
  Actor_SetPosition(ACTOR_SMOKE_CLOUD, 672, 104);
  Event_PlaySound(SE_CLATTER);
  Actor_SetAnimation(ACTOR_SMOKE_CLOUD, 2);
  Actor_WaitForAnimationChange(ACTOR_SMOKE_CLOUD);
  Actor_SetPosition(ACTOR_SMOKE_CLOUD, 0, 0);
  FieldEvent_Wait(4);
  Actor_SetPosition(ACTOR_RISING_SMOKE, 672, 104);
  Actor_SetAnimation(ACTOR_RISING_SMOKE, 4);
  Actor_WaitForAnimationChange(ACTOR_RISING_SMOKE);
  FieldEvent_Wait(15);
  Actor_SetPosition(ACTOR_RISING_SMOKE, 0, 0);
  FieldEvent_Wait(30);
  Actor_ShowEmote(gMainRoutineState.leaderActorId, EMOTE_IN_FRONT | EMOTE_WINCE,
                  0);
  FieldEvent_Wait(60);
  Event_PlaySound(MUS_KIBOMBO);
  Actor_ShowEmote(ACTOR_FORTUNE_TELLER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  // "I see..."
  Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
  unitData = Unit_Get(unit);
  info = Item_GetDirect(unitData->inventory[slot]);
#if !TLA_PATCH_1
  item = unitData->inventory[slot];
#else
  item = unitData->inventory[slot] & INVENTORY_ITEM_ID_MASK;
#endif
  if ((u16)(item - ITEM_RIGHT_PRONG) <= ITEM_CENTER_PRONG - ITEM_RIGHT_PRONG) {
    NaribweRooms_ReadProng();
  } else if ((u16)item == ITEM_TRIDENT) {
    FieldEvent_SetMessageId(message + TRIDENT_FEARSOME_WEAPON);
    // "That object you have placed before me... It is a fearsome weapon to..."
    Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
    if (!Event_GetFlag(FLAG_SEA_OF_TIME_OPENED)) {
      FieldEvent_SetMessageId(message + TRIDENT_DEFEAT_THE_BEAST);
      // "If you wield this weapon, you can defeat the legendary beast!"
      Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
    } else {
      FieldEvent_SetMessageId(message + TRIDENT_NO_MORE_BEASTS);
      // "But there are no more beasts for you to fear in this part of the..."
      Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
    }
  } else {
    rawType = info->type;
    type = rawType;
    if (type == ITEM_TYPE_PSYNERGY)
      NaribweRooms_ReadShiningItem();
    else if (type == ITEM_TYPE_WEAPON)
      NaribweRooms_ReadWeapon();
    else if ((u8)((u8)rawType - ITEM_TYPE_ARMOR) <=
             ITEM_TYPE_BOOTS - ITEM_TYPE_ARMOR)
      NaribweRooms_ReadArmor();
    else
      NaribweRooms_ReadOtherItem();
  }
  Actor_SetPosition(ACTOR_PLACED_ITEM, 0, 0);
  Party_AdjustCoins(-FORTUNE_PRICE);
}

/// The fortune-teller's readings, one numbering for all four kinds: the foe a
/// weapon shows, the power a Psynergy item shows, the path any other item shows
/// and the prong a prong of the Trident shows.
enum FortuneReading {
  /// Before Gabomba's message is carried, for a weapon "Many enemies await
  /// you, far beyond the mountains.", for any other item "I see you inside a
  /// sacred icon."
  READING_BEFORE_GABOMBAS_MESSAGE = 0,
  READING_FOE_IN_MISTY_SEA = 1, ///< "In a misty sea, a mighty foe..."
  READING_EAST_CONQUERED = 2,   ///< "None in the eastern world can stand..."
  /// The first power still lacking: Reveal, Sand, Parch, the Burst Brooch, the
  /// Grindstone, or none.
  ///@{
  READING_SEEK_REVEAL = 3,
  READING_SEEK_SAND = 4,
  READING_SEEK_PARCH = 5,
  READING_SEEK_BURST_BROOCH = 6,
  READING_SEEK_GRINDSTONE = 7,
  READING_ALL_POWERS = 8,
  ///@}
  READING_FORGE_WEAPON = 9,     ///< "You must forge a weapon..."
  READING_RAISE_COLUMN = 10,    ///< "You will raise a mighty column..."
  READING_EASTERN_VILLAGE = 11, ///< "...help from a village on the eastern..."
  READING_SEA_BEAST = 12,       ///< "You have the weapon you need..."
  READING_WEST = 13,            ///< "The object that you seek lies...west."
  /// Where the prongs not yet held lie: the ruins by the beach, the
  /// moss-covered ruins, the icy cape's tower, or all held.
  ///@{
  READING_PRONG_BY_BEACH = 14,
  READING_PRONG_IN_MOSSY_RUINS = 15,
  READING_PRONG_IN_TOWER = 16,
  READING_ALL_PRONGS = 17
  ///@}
};

s32 Party_FindOwnerWithAction(s32 item);
s32 NaribweRooms_FoeAhead(void);
s32 NaribweRooms_NextPowerToGain(void);
s32 NaribweRooms_NextPath(void);
s32 NaribweRooms_ProngToSeek(void);
u8 *NaribweRooms_GetDjinniHint(void);

/// A weapon: the foe that lies beyond it.
void NaribweRooms_ReadWeapon(void) {
  u8 *message = MsgNaribweRoomsBeyondWeaponYouSetDown;

  FieldEvent_SetMessageId((s32)message);
  // "Beyond the weapon you set down lies a terrible foe."
  Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
  switch (NaribweRooms_FoeAhead()) {
  case READING_BEFORE_GABOMBAS_MESSAGE:
    FieldEvent_SetMessageId((s32)(message + 1));
    // "Many enemies await you, far beyond the mountains. If you move to..."
    Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
    break;
  case READING_FOE_IN_MISTY_SEA:
    FieldEvent_SetMessageId((s32)(message + 2));
    // "In a misty sea, a mighty foe... A fiersome battle awaits you..."
    Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
    break;
  case READING_EAST_CONQUERED:
    FieldEvent_SetMessageId((s32)(message + 3));
    // "None in the eastern world can stand against the power you possess."
    Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
    break;
  }
}

/// A Psynergy item: a power shining beyond it, and where the next power to gain
/// waits.
void NaribweRooms_ReadShiningItem(void) {
  u8 *message = MsgNaribweRoomsIFeelShiningPowerGlowing;

  FieldEvent_SetMessageId((s32)message);
  // "I feel a shining power glowing beyond the item before me."
  Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
  switch (NaribweRooms_NextPowerToGain()) {
  case READING_SEEK_REVEAL:
    FieldEvent_SetMessageId((s32)MsgNaribweRoomsYouAreLost);
    // "You are lost in your way... Another continent needs your attention..."
    Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
    break;
  case READING_SEEK_SAND:
    FieldEvent_SetMessageId((s32)MsgNaribweRoomsDistantIsleLiesFar);
    // "A distant isle lies far to the northeast... Sail there. High atop a..."
    Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
    break;
  case READING_SEEK_PARCH:
    FieldEvent_SetMessageId((s32)MsgNaribweRoomsISeeParadiseAwaiting);
    // "I see a paradise awaiting you... An island in the east. But even..."
    Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
    break;
  case READING_SEEK_BURST_BROOCH:
    FieldEvent_SetMessageId(
        (s32)MsgNaribweRoomsTinyFigureStandsAgainstWasteland);
    // "A tiny figure stands against a wasteland wrought in snow... The..."
    Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
    break;
  case READING_SEEK_GRINDSTONE:
    FieldEvent_SetMessageId((s32)MsgNaribweRoomsISeeYouLost);
    // "I see you lost in mists, lost in time, lost in the seas... And..."
    Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
    break;
  default:
    FieldEvent_SetMessageId((s32)(message + 6));
    // "The eastern lands hold little for you now. You will not remain..."
    Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
    break;
  }
}

/// Armor, a shield, a helm or boots: where a Djinni still to be found waits, or
/// nothing of the future once none is left.
void NaribweRooms_ReadArmor(void) {
  s32 message = NaribweRooms_GetDjinniHint();

  if (message == (s32)MsgNaribweRoomsInItemYouHavePlaced) {
    FieldEvent_SetMessageId(message);
    // "In the item you have placed before me, I can see nothing of the..."
    Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
  } else {
    FieldEvent_SetMessageId((s32)MsgNaribweRoomsInItemYouHavePlaced -
                            ARMOR_PRESENCE_SENSED_BEFORE);
    // "From the garb you have placed before me, I sense the presence of the..."
    Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
    FieldEvent_SetMessageId(message);
    Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
  }
}

/// Any other item: the path the party should follow.
void NaribweRooms_ReadOtherItem(void) {
  FieldEvent_SetMessageId((s32)MsgNaribweRoomsBeyondItemBeforeMe);
  // "Beyond the item before me, I see a path for you to follow."
  Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
  switch (NaribweRooms_NextPath()) {
  case READING_BEFORE_GABOMBAS_MESSAGE:
    FieldEvent_SetMessageId((s32)MsgNaribweRoomsISeeYouInsideSacred);
    // "I see you inside a sacred icon. By listening to the voices, you will..."
    Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
    break;
  case READING_FORGE_WEAPON:
    FieldEvent_SetMessageId((s32)MsgNaribweRoomsYouMustForgeWeapon);
    // "You must forge a weapon to defeat a mortal enemy. Seek out the pieces!"
    Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
    break;
  case READING_RAISE_COLUMN:
    FieldEvent_SetMessageId((s32)MsgNaribweRoomsYouWillRaiseMightyColumn);
    // "You will raise a mighty column and gain assistance where it is least..."
    Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
    break;
  case READING_EASTERN_VILLAGE:
    FieldEvent_SetMessageId((s32)MsgNaribweRoomsYouWillGainTremendousHelp);
    // "You will gain tremendous help from a village on the eastern reaches..."
    Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
    break;
  case READING_SEA_BEAST:
    FieldEvent_SetMessageId((s32)MsgNaribweRoomsYouHaveWeaponYouNeed);
    // "You have the weapon you need to defeat the sea beast. Go to the sea..."
    Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
    break;
  case READING_WEST:
    FieldEvent_SetMessageId((s32)MsgNaribweRoomsObjectThatYouSeekLies);
    // "The object that you seek lies somewhere in the west. Why do you..."
    Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
    break;
  }
}

/// A prong of the Trident: the power it holds over a foe, and where a prong
/// still missing lies.
void NaribweRooms_ReadProng(void) {
  u8 *message = MsgNaribweRoomsObjectYouHavePlacedBefore;

  FieldEvent_SetMessageId((s32)message);
  // "The object you have placed before me wields a mighty power over a..."
  Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
  switch (NaribweRooms_ProngToSeek()) {
  case READING_PRONG_BY_BEACH:
    FieldEvent_SetMessageId((s32)(message + 1));
    // "Near the beach where you washed ashore lie ancient ruins. Seek them..."
    Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
    break;
  case READING_PRONG_IN_MOSSY_RUINS:
    FieldEvent_SetMessageId((s32)(message + 2));
    // "In the northern reaches of the Eastern Sea lie moss-covered,..."
    Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
    break;
  case READING_PRONG_IN_TOWER:
    FieldEvent_SetMessageId((s32)(message + 3));
    // "In the distant south lies an icy cape. Above the rocky shore stands..."
    Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
    break;
  case READING_ALL_PRONGS:
    FieldEvent_SetMessageId((s32)(message + 4));
    // "You have the weapon you need in many fragments.. Bring them to the..."
    Event_ShowMessage(ACTOR_FORTUNE_TELLER, 0);
    break;
  }
}

/// Which foe a weapon's reading names: none before Gabomba's message is
/// carried, then the foe in the misty sea, and none once Poseidon is beaten.
s32 NaribweRooms_FoeAhead(void) {
  if (Event_GetFlag(FLAG_GABOMBA_MESSAGE_CARRIED) == 0)
    return READING_BEFORE_GABOMBAS_MESSAGE;
  return (Event_GetFlag(FLAG_SEA_OF_TIME_OPENED) != 0) +
         READING_FOE_IN_MISTY_SEA;
}

/// The first power the party still lacks, for a Psynergy item's reading: three
/// Psynergy (Reveal, Sand and Parch) and two items (the Burst Brooch and the
/// Grindstone), or READING_ALL_POWERS once it has them all.
s32 NaribweRooms_NextPowerToGain(void) {
  if (Party_FindOwnerWithAction(ABILITY_REVEAL) == -1)
    return READING_SEEK_REVEAL;
  if (Party_FindOwnerWithAction(ABILITY_SAND) == -1)
    return READING_SEEK_SAND;
  if (Party_FindOwnerWithAction(ABILITY_PARCH) == -1)
    return READING_SEEK_PARCH;
  if (FieldPartyInventory_FindMember(ITEM_BURST_BROOCH) == -1)
    return READING_SEEK_BURST_BROOCH;
  if (FieldPartyInventory_FindMember(ITEM_GRINDSTONE) == -1)
    return READING_SEEK_GRINDSTONE;
  return READING_ALL_POWERS;
}

/// The path an ordinary item's reading points along, by the story's flags, the
/// Trident's prongs and the Trident.
s32 NaribweRooms_NextPath(void) {
  if (Event_GetFlag(FLAG_GABOMBA_MESSAGE_CARRIED) == 0)
    return READING_BEFORE_GABOMBAS_MESSAGE;
  if (FieldPartyInventory_FindMember(ITEM_TRIDENT) == -1) {
    if (Event_GetFlag(FLAG_SEA_OF_TIME_OPENED) == 0) {
      if (FieldPartyInventory_FindMember(ITEM_RIGHT_PRONG) == -1 ||
          FieldPartyInventory_FindMember(ITEM_LEFT_PRONG) == -1 ||
          FieldPartyInventory_FindMember(ITEM_CENTER_PRONG) == -1)
        return READING_FORGE_WEAPON;
      return (Event_GetFlag(FLAG_WORKERS_TOLD_MAYOR) != 0) +
             READING_RAISE_COLUMN;
    }
  } else {
    if (Event_GetFlag(FLAG_SEA_OF_TIME_OPENED) == 0)
      return READING_SEA_BEAST;
  }
  return READING_WEST;
}

/// Where a prong's reading sends the party: to the ruins by the beach while the
/// Right Prong is missing, the moss-covered ruins while the Left Prong is, the
/// icy cape's tower while only the Center Prong is, and on once it holds all
/// three.
s32 NaribweRooms_ProngToSeek(void) {
  if (FieldPartyInventory_FindMember(ITEM_RIGHT_PRONG) == -1 &&
      (FieldPartyInventory_FindMember(ITEM_CENTER_PRONG) != -1 ||
       FieldPartyInventory_FindMember(ITEM_LEFT_PRONG) != -1))
    return READING_PRONG_BY_BEACH;
  if (FieldPartyInventory_FindMember(ITEM_RIGHT_PRONG) != -1 &&
      FieldPartyInventory_FindMember(ITEM_LEFT_PRONG) == -1 &&
      FieldPartyInventory_FindMember(ITEM_CENTER_PRONG) == -1)
    return READING_PRONG_IN_MOSSY_RUINS;
  if (FieldPartyInventory_FindMember(ITEM_RIGHT_PRONG) != -1 &&
      FieldPartyInventory_FindMember(ITEM_LEFT_PRONG) == -1 &&
      FieldPartyInventory_FindMember(ITEM_CENTER_PRONG) != -1)
    return READING_PRONG_IN_MOSSY_RUINS;
  if (FieldPartyInventory_FindMember(ITEM_RIGHT_PRONG) != -1 &&
      FieldPartyInventory_FindMember(ITEM_LEFT_PRONG) != -1 &&
      FieldPartyInventory_FindMember(ITEM_CENTER_PRONG) == -1)
    return READING_PRONG_IN_TOWER;
  if (FieldPartyInventory_FindMember(ITEM_RIGHT_PRONG) != -1 &&
      FieldPartyInventory_FindMember(ITEM_LEFT_PRONG) != -1 &&
      FieldPartyInventory_FindMember(ITEM_CENTER_PRONG) != -1)
    return READING_ALL_PRONGS;
}

/// One past the last Djinni of an element the party has found, of those this
/// game hides and the fortune-teller hints at, or -1 when it has found none.
s32 NaribweRooms_DjinniHintLimit(s32 group) {
  s32 i = DJINNI_INDEX(DJINNI_CRYSTAL);

  do {
    s32 flag = FLAG_DJINNI(group * DJINN_PER_ELEMENT + i);

    if (group == ELEMENT_VENUS) {
      if (i == DJINNI_INDEX(DJINNI_ECHO))
        continue;
    }
    if (group == ELEMENT_MERCURY) {
      if (i == DJINNI_INDEX(DJINNI_SPRING))
        continue;
      if (i == DJINNI_INDEX(DJINNI_SHADE))
        continue;
    }
    if (Event_GetFlag(flag))
      return i + 1;
  } while (--i >= TRANSFERRED_DJINN_PER_ELEMENT);
  return -1;
}

/// Lists the flags of an element's Djinn still to be found, from the first
/// Djinni not carried over from Isaac's party up to the limit and skipping
/// Echo, Spring and Shade, which the fortune-teller never hints at; returns
/// how many.
s32 NaribweRooms_ListMissingDjinn(s32 limit, s32 group, s32 *out) {
  s32 count = 0;
  s32 i = TRANSFERRED_DJINN_PER_ELEMENT;

  if (i < limit) {
    s32 *cursor = out;

    for (; i < limit; i++) {
      s32 flag;

      if (group == ELEMENT_VENUS && i == DJINNI_INDEX(DJINNI_ECHO))
        continue;
      if (group == ELEMENT_MERCURY) {
        if (i == DJINNI_INDEX(DJINNI_SPRING))
          continue;
        if (i == DJINNI_INDEX(DJINNI_SHADE))
          continue;
      }
      flag = FLAG_DJINNI(group * DJINN_PER_ELEMENT + i);
      if (Event_GetFlag(flag) == 0) {
        *cursor++ = flag;
        count++;
      }
    }
  }
  return count;
}

/// Higher than any element's hint limit, so that the first one is lower.
#define HINT_LIMIT_ABOVE_ALL 9999

/// The message telling where a Djinni still to be found waits, or
/// MsgNaribweRoomsInItemYouHavePlaced when none is left to find.
///
/// A Djinni the party has passed by, one before the last of its element it has
/// found, is picked at random; when it has passed none by, the first still to
/// be found of the element it has found the fewest of.
u8 *NaribweRooms_GetDjinniHint(void) {
  s32 *hints =
      (s32 *)Value2((s32 (*)())HeapSlot_AllocEwram, HEAP_SLOT_DJINNI_HINTS,
                    ELEMENT_COUNT * DJINN_PER_ELEMENT * sizeof(s32));
  s32 limits[ELEMENT_COUNT];
  s32 count = 0;
  s32 lowest = HINT_LIMIT_ABOVE_ALL;
  s32 lowestGroup = 0;
  u32 group;
  s32 index;
  u8 *message;

  for (group = 0; group < ELEMENT_COUNT; group++) {
    s32 limit = NaribweRooms_DjinniHintLimit(group);

    // DEAD: each element's limit, which nothing reads; the ROM keeps its store.
    limits[group] = limit;
    if (lowest > limit) {
      lowest = limit;
      lowestGroup = group;
    }
    if (limit != -1)
      count += NaribweRooms_ListMissingDjinn(limit, group, hints + count);
  }
  if (count == 0) {
    for (group = 0; group < ELEMENT_COUNT; group++)
      count += NaribweRooms_ListMissingDjinn(DJINNI_INDEX(DJINNI_CRYSTAL) + 1,
                                             group, hints + count);
    if (count == 0) {
      HeapSlot_Release(HEAP_SLOT_DJINNI_HINTS);
      return MsgNaribweRoomsInItemYouHavePlaced;
    }
    NaribweRooms_ListMissingDjinn(DJINNI_INDEX(DJINNI_CRYSTAL) + 1, lowestGroup,
                                  hints);
    index = 0;
  } else {
    index = Random16() % count;
  }
  switch (hints[index]) {
  case FLAG_DJINNI(DJINNI_IRON):
    message = MsgNaribweRoomsToEastLiesContinentMoved;
    break;
  case FLAG_DJINNI(DJINNI_STEEL):
    message = MsgNaribweRoomsToNorthInsideSacredIcon;
    break;
  case FLAG_DJINNI(DJINNI_MUD):
    message = MsgNaribweRoomsToNorthBeneathSacredIcon;
    break;
  case FLAG_DJINNI(DJINNI_FLOWER):
    message = MsgNaribweRoomsHeadEastBySoutheast;
    break;
  case FLAG_DJINNI(DJINNI_MELD):
    message = MsgNaribweRoomsHeadEastByNortheast;
    break;
  case FLAG_DJINNI(DJINNI_PETRA):
    message = MsgNaribweRoomsHeadNorthByNorthwest;
    break;
  case FLAG_DJINNI(DJINNI_SALT):
    message = MsgNaribweRoomsToNorthwestInRuins;
    break;
  case FLAG_DJINNI(DJINNI_GEODE):
    message = MsgNaribweRoomsHeadWestByNorthwest;
    break;
  case FLAG_DJINNI(DJINNI_MOLD):
    message = MsgNaribweRoomsToNorthInVillage;
    break;
  case FLAG_DJINNI(DJINNI_CRYSTAL):
    message = MsgNaribweRoomsHeadEastBySoutheastInDesert;
    break;
  case FLAG_DJINNI(DJINNI_FOG):
    message = MsgNaribweRoomsHeadEastByNortheastInCave;
    break;
  case FLAG_DJINNI(DJINNI_SOUR):
    message = MsgNaribweRoomsToSoutheastInPlains;
    break;
  case FLAG_DJINNI(DJINNI_CHILL):
    message = MsgNaribweRoomsToSouthInForestOne;
    break;
  case FLAG_DJINNI(DJINNI_STEAM):
    message = MsgNaribweRoomsToEastBesideBoulder;
    break;
  case FLAG_DJINNI(DJINNI_RIME):
    message = MsgHeadEastByNortheastInThe;
    break;
  case FLAG_DJINNI(DJINNI_GEL):
    message = MsgNaribweRoomsToNorthSomewhereOnSmall;
    break;
  case FLAG_DJINNI(DJINNI_EDDY):
    message = MsgNaribweRoomsHeadNorthByNorthwestInCave;
    break;
  case FLAG_DJINNI(DJINNI_BALM):
    message = MsgNaribweRoomsToNorthInRuinsChilled;
    break;
  case FLAG_DJINNI(DJINNI_SERAC):
    message = MsgHeadEastByNortheastInA;
    break;
  case FLAG_DJINNI(DJINNI_CANNON):
    message = MsgNaribweRoomsToEastInMountainPaths;
    break;
  case FLAG_DJINNI(DJINNI_SPARK):
    message = MsgNaribweRoomsToSoutheastInVillageLulled;
    break;
  case FLAG_DJINNI(DJINNI_KINDLE):
    message = MsgNaribweRoomsHeadSouthBySoutheast;
    break;
  case FLAG_DJINNI(DJINNI_CHAR):
    message = MsgNaribweRoomsHeadEastBySoutheastInTown;
    break;
  case FLAG_DJINNI(DJINNI_COAL):
    message = MsgNaribweRoomsHeadEastByNortheastOnIsland;
    break;
  case FLAG_DJINNI(DJINNI_REFLUX):
    message = MsgNaribweRoomsHeadSouthBySoutheastInRuins;
    break;
  case FLAG_DJINNI(DJINNI_CORE):
    message = MsgNaribweRoomsToNorthwestInForest;
    break;
  case FLAG_DJINNI(DJINNI_TINDER):
    message = MsgNaribweRoomsToNorthwestAlongCoast;
    break;
  case FLAG_DJINNI(DJINNI_SHINE):
    message = MsgNaribweRoomsToNorthwestBesideRuins;
    break;
  case FLAG_DJINNI(DJINNI_FURY):
    message = MsgNaribweRoomsHeadNorthByNorthwestAtGreat;
    break;
  case FLAG_DJINNI(DJINNI_FUGUE):
    message = MsgNaribweRoomsToNorthInRuinsChilledByIcy;
    break;
  case FLAG_DJINNI(DJINNI_BREATH):
    message = MsgHeadEastByNortheastInACave;
    break;
  case FLAG_DJINNI(DJINNI_BLITZ):
    message = MsgHeadEastBySoutheastInThe;
    break;
  case FLAG_DJINNI(DJINNI_ETHER):
    message = MsgHeadEastBySoutheastInA;
    break;
  case FLAG_DJINNI(DJINNI_WAFT):
    message = MsgToTheNorthInTheMountain;
    break;
  case FLAG_DJINNI(DJINNI_HAZE):
    message = MsgNaribweRoomsToEastSomewhereOnTiny;
    break;
  case FLAG_DJINNI(DJINNI_WHEEZE):
    message = MsgNaribweRoomsToEastInForest;
    break;
  case FLAG_DJINNI(DJINNI_AROMA):
    message = MsgNaribweRoomsHeadNorthByNorthwestInVillage;
    break;
  case FLAG_DJINNI(DJINNI_WHORL):
    message = MsgNaribweRoomsInNorthwestInRuins;
    break;
  case FLAG_DJINNI(DJINNI_GASP):
    message = MsgNaribweRoomsHeadNorthByNorthwestNear;
    break;
  case FLAG_DJINNI(DJINNI_LULL):
    message = MsgToTheNorthInTheRuins;
    break;
  case FLAG_DJINNI(DJINNI_GALE):
    message = MsgNaribweRoomsHeadNorthByNortheast;
    break;
  default:
    message = MsgNaribweRoomsInItemYouHavePlaced;
    break;
  }
  HeapSlot_Release(HEAP_SLOT_DJINNI_HINTS);
  return message;
}

void Actor_DisableById(s32 id);

/// Opens the houses through the window transition, and keeps the reading's
/// item, smoke and cloud out of sight until a reading needs them.
s32 Scene_Initialize(void) {
  struct Actor *actor;

  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  Actor_Get(gMainRoutineState.leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_IGNORES_RISES;
  Actor_DisableById(ACTOR_FORTUNE_TABLE);
  Actor_DisableById(ACTOR_PLACED_ITEM);
  FieldActor_SetSpritePriority(ACTOR_FORTUNE_TELLER, 2);
  Actor_Get(ACTOR_VILLAGER_ON_BODY_PAINT)->collisionFlags |= ACTOR_NEAR_TALK;
  FieldActor_SetSpritePriority(ACTOR_INNKEEPER, 2);
  Actor_Get(ACTOR_INNKEEPER)->collisionFlags |= ACTOR_NEAR_TALK;

  actor = Actor_Get(ACTOR_PLACED_ITEM);
  actor->motionFlags = 0;
  actor->y.fixed = -toFIXED(4);
  actor->groundHeight = -toFIXED(4);
  Actor_Get(ACTOR_PLACED_ITEM)->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
  FieldActor_SetSpritePriority(ACTOR_PLACED_ITEM, 1);
  Actor_SetPosition(ACTOR_PLACED_ITEM, 0, 0);

  actor = Actor_Get(ACTOR_RISING_SMOKE);
  actor->motionFlags = 0;
  actor->y.fixed = toFIXED(12);
  actor->groundHeight = toFIXED(12);
  Actor_Get(ACTOR_RISING_SMOKE)->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
  FieldActor_SetSpritePriority(ACTOR_RISING_SMOKE, 1);
  Actor_SetPosition(ACTOR_RISING_SMOKE, 0, 0);

  actor = Actor_Get(ACTOR_SMOKE_CLOUD);
  actor->motionFlags = 0;
  actor->y.fixed = toFIXED(12);
  actor->groundHeight = toFIXED(12);
  Actor_Get(ACTOR_SMOKE_CLOUD)->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
  FieldActor_SetSpritePriority(ACTOR_SMOKE_CLOUD, 1);
  Actor_SetPosition(ACTOR_SMOKE_CLOUD, 0, 0);

  Actor_SetSpriteFlags(Actor_Get(ACTOR_RISING_SMOKE), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_SMOKE_CLOUD), 0);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_SMOKE_CLOUD), SPRITE_COLOR_GBR);
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

#include "shared/scene_entrances_default.c"

u32 gSceneExits[] = {
    SCENE_EXITS(NaribweRooms),
    SCENE_EXIT_UNLESS(NARIBWE_ROOMS_TO_NARIBWE_NORTHWEST, Naribwe,
                      NARIBWE_FROM_NARIBWE_ROOMS_NORTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(NARIBWE_ROOMS_TO_NARIBWE_WEST, Naribwe,
                      NARIBWE_FROM_NARIBWE_ROOMS_WEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(NARIBWE_ROOMS_TO_NARIBWE_SOUTHWEST, Naribwe,
                      NARIBWE_FROM_NARIBWE_ROOMS_SOUTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(NARIBWE_ROOMS_TO_NARIBWE_SOUTHEAST, Naribwe,
                      NARIBWE_FROM_NARIBWE_ROOMS_SOUTHEAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(NARIBWE_ROOMS_TO_NARIBWE_NORTHEAST, Naribwe,
                      NARIBWE_FROM_NARIBWE_ROOMS_NORTHEAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(NARIBWE_ROOMS_TO_WEST_HUT, NaribweRooms,
                      NARIBWE_ROOMS_FROM_NORTHWEST_HUT, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(NARIBWE_ROOMS_TO_NORTHWEST_HUT, NaribweRooms,
                      NARIBWE_ROOMS_FROM_WEST_HUT, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_WANDER, 120, 0, 104, FACING_SOUTHWEST,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 88, 0, 360, FACING_SOUTHEAST,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_FORTUNE_TELLER, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 672, 0, 94, FACING_SOUTH,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_SPIKY_HAIR, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 651, 0, 106, FACING_SOUTHEAST,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_WIFE, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 693, 0, 106, FACING_SOUTH,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WEAPON_MERCHANT, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 392, 0, 104, FACING_SOUTH,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_ARMOR_MERCHANT, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 408, 0, 104, FACING_SOUTH,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WEAPON_MERCHANT, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_WANDER, 424, 0, 360, FACING_SOUTHWEST,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_INN_WORKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    328, 0, 368, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    672, 0, 112, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    672, 0, 104, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_FLAME_AND_SMOKE, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 672, 0, 104, FACING_SOUTH,
                    TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_DUST_CLOUD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    672, 0, 104, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

/// The ways out, the villagers talked to and read, and the fortune-teller's
/// table, until Gabomba's message is carried.
struct Trigger gNaribweRooms_Triggers[] = {
    {TRIGGER_EXIT, NARIBWE_ROOMS_TO_NARIBWE_NORTHWEST, CONDITION_ALWAYS,
     NARIBWE_ROOMS_TO_NARIBWE_NORTHWEST},
    {TRIGGER_EXIT, NARIBWE_ROOMS_TO_NARIBWE_WEST, CONDITION_ALWAYS,
     NARIBWE_ROOMS_TO_NARIBWE_WEST},
    {TRIGGER_EXIT, NARIBWE_ROOMS_TO_NARIBWE_SOUTHWEST, CONDITION_ALWAYS,
     NARIBWE_ROOMS_TO_NARIBWE_SOUTHWEST},
    {TRIGGER_EXIT, NARIBWE_ROOMS_TO_NARIBWE_SOUTHEAST, CONDITION_ALWAYS,
     NARIBWE_ROOMS_TO_NARIBWE_SOUTHEAST},
    {TRIGGER_EXIT, NARIBWE_ROOMS_TO_NARIBWE_NORTHEAST, CONDITION_ALWAYS,
     NARIBWE_ROOMS_TO_NARIBWE_NORTHEAST},
    {TRIGGER_EXIT, NARIBWE_ROOMS_TO_WEST_HUT, CONDITION_ALWAYS,
     NARIBWE_ROOMS_TO_WEST_HUT},
    {TRIGGER_EXIT, NARIBWE_ROOMS_TO_NORTHWEST_HUT, CONDITION_ALWAYS,
     NARIBWE_ROOMS_TO_NORTHWEST_HUT},
    {TRIGGER_TALK, ACTOR_BOY_DIRECTING_TO_THE_FORTUNE_TELLER, CONDITION_ALWAYS,
     MSGNO(MsgNaribweRoomsFortuneTellerLivesAtHighest)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_A_SMALL_VILLAGE, CONDITION_ALWAYS,
     MSGNO(MsgNaribweRoomsWereSmallVillage)},
    {TRIGGER_TALK, ACTOR_FORTUNE_TELLER, CONDITION_ALWAYS,
     MSGNO(MsgNaribweRoomsIfYouDesireReadingStand)},
    {TRIGGER_TALK, ACTOR_FORTUNE_TELLERS_CHILD, CONDITION_ALWAYS,
     MSGNO(MsgNaribweRoomsMyFatherIsFortuneTeller)},
    {TRIGGER_TALK, ACTOR_VILLAGER_ON_BODY_PAINT, CONDITION_ALWAYS,
     MSGNO(MsgNaribweRoomsByPaintingOurBodies)},
    {TRIGGER_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS,
     EVENT(NaribweRooms_TalkToWeaponMerchant)},
    {TRIGGER_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS,
     EVENT(NaribweRooms_TalkToArmorMerchant)},
    {TRIGGER_TALK, ACTOR_VILLAGER_ON_ADVENTURE, CONDITION_ALWAYS,
     MSGNO(MsgNaribweRoomsIfYouLikeAdventure)},
    {TRIGGER_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS,
     EVENT(NaribweRooms_TalkToInnkeeper)},
    {TRIGGER_TALK, ACTOR_FORTUNE_TABLE, CONDITION_ALWAYS,
     EVENT(NaribweRooms_TalkToFortuneTeller)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_BOY_DIRECTING_TO_THE_FORTUNE_TELLER, CONDITION_ALWAYS,
     MSGNO(MsgNaribweRoomsFortuneTellerInNaribweHas)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_A_SMALL_VILLAGE,
     CONDITION_ALWAYS, MSGNO(MsgNaribweRoomsAsLongAsWeListen)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FORTUNE_TELLER,
     CONDITION_ALWAYS, MSGNO(MsgNaribweRoomsItemsThatOneCarries)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FORTUNE_TELLERS_CHILD,
     CONDITION_ALWAYS, MSGNO(MsgNaribweRoomsMyFatherMayNot)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_BODY_PAINT,
     CONDITION_ALWAYS, MSGNO(MsgNaribweRoomsBodyPaintingAlsoProtectsUs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT,
     CONDITION_ALWAYS, MSGNO(MsgNaribweRoomsICanLookAtAny)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT,
     CONDITION_ALWAYS, MSGNO(MsgNaribweRoomsWheneverIGetMyFortune)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_ADVENTURE,
     CONDITION_ALWAYS, MSGNO(MsgNaribweRoomsRiverIsProbablyVeryDangerous)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS,
     MSGNO(MsgNaribweRoomsOurSpicesAreOurSecret)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FORTUNE_TABLE,
     CONDITION_ALWAYS, EVENT(NaribweRooms_MindReadFortuneTeller)},
    {TRIGGER_TABLE_END},
};

/// The same, once Gabomba's message is carried.
struct Trigger gNaribweRooms_TriggersOnReturn[] = {
    {TRIGGER_EXIT, NARIBWE_ROOMS_TO_NARIBWE_NORTHWEST, CONDITION_ALWAYS,
     NARIBWE_ROOMS_TO_NARIBWE_NORTHWEST},
    {TRIGGER_EXIT, NARIBWE_ROOMS_TO_NARIBWE_WEST, CONDITION_ALWAYS,
     NARIBWE_ROOMS_TO_NARIBWE_WEST},
    {TRIGGER_EXIT, NARIBWE_ROOMS_TO_NARIBWE_SOUTHWEST, CONDITION_ALWAYS,
     NARIBWE_ROOMS_TO_NARIBWE_SOUTHWEST},
    {TRIGGER_EXIT, NARIBWE_ROOMS_TO_NARIBWE_SOUTHEAST, CONDITION_ALWAYS,
     NARIBWE_ROOMS_TO_NARIBWE_SOUTHEAST},
    {TRIGGER_EXIT, NARIBWE_ROOMS_TO_NARIBWE_NORTHEAST, CONDITION_ALWAYS,
     NARIBWE_ROOMS_TO_NARIBWE_NORTHEAST},
    {TRIGGER_EXIT, NARIBWE_ROOMS_TO_WEST_HUT, CONDITION_ALWAYS,
     NARIBWE_ROOMS_TO_WEST_HUT},
    {TRIGGER_EXIT, NARIBWE_ROOMS_TO_NORTHWEST_HUT, CONDITION_ALWAYS,
     NARIBWE_ROOMS_TO_NORTHWEST_HUT},
    {TRIGGER_TALK, ACTOR_BOY_DIRECTING_TO_THE_FORTUNE_TELLER, CONDITION_ALWAYS,
     MSGNO(MsgNaribweRoomsIGuessThoseWarriorsLearned)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_A_SMALL_VILLAGE, CONDITION_ALWAYS,
     MSGNO(MsgNaribweRoomsOurFortuneTellerHasBeen)},
    {TRIGGER_TALK, ACTOR_FORTUNE_TELLER, CONDITION_ALWAYS,
     MSGNO(MsgNaribweRoomsIfYouDesireReadingStand)},
    {TRIGGER_TALK, ACTOR_FORTUNE_TELLERS_CHILD, CONDITION_ALWAYS,
     MSGNO(MsgNaribweRoomsIfTroubleResides)},
    {TRIGGER_TALK, ACTOR_VILLAGER_ON_BODY_PAINT, CONDITION_ALWAYS,
     MSGNO(MsgNaribweRoomsReadingFortunesInPeoplesItems)},
    {TRIGGER_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS,
     EVENT(NaribweRooms_TalkToWeaponMerchantOnReturn)},
    {TRIGGER_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS,
     EVENT(NaribweRooms_TalkToArmorMerchantOnReturn)},
    {TRIGGER_TALK, ACTOR_VILLAGER_ON_ADVENTURE, CONDITION_ALWAYS,
     MSGNO(MsgIfIsTraveling)},
    {TRIGGER_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS,
     EVENT(NaribweRooms_TalkToInnkeeperOnReturn)},
    {TRIGGER_TALK, ACTOR_FORTUNE_TABLE, CONDITION_ALWAYS,
     EVENT(NaribweRooms_TalkToFortuneTeller)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_BOY_DIRECTING_TO_THE_FORTUNE_TELLER, CONDITION_ALWAYS,
     MSGNO(MsgNaribweRoomsPowerOfFortuneTeller)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_A_SMALL_VILLAGE,
     CONDITION_ALWAYS, MSGNO(MsgNaribweRoomsFortuneTellerSaidThatPower)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FORTUNE_TELLER,
     CONDITION_ALWAYS, MSGNO(MsgNaribweRoomsItemsPlacedBeforeMe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FORTUNE_TELLERS_CHILD,
     CONDITION_ALWAYS, MSGNO(MsgNaribweRoomsFatherShouldChargeMore)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_BODY_PAINT,
     CONDITION_ALWAYS, MSGNO(MsgNaribweRoomsAfterFathersBeenReadingAll)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT,
     CONDITION_ALWAYS, MSGNO(MsgNaribweRoomsWhenILookAtWeapons)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT,
     CONDITION_ALWAYS, MSGNO(MsgNaribweRoomsILikeOfferingArmor)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_ADVENTURE,
     CONDITION_ALWAYS, MSGNO(MsgNaribweRoomsHePaidMeLot)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS,
     MSGNO(MsgWhyWouldTravel)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FORTUNE_TABLE,
     CONDITION_ALWAYS, EVENT(NaribweRooms_MindReadFortuneTellerOnReturn)},
    {TRIGGER_TABLE_END},
};
