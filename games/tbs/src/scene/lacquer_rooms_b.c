/// @file
/// Lacquer rooms B (SceneId_LacquerRoomsB), a sheet of rooms drawn in red
/// lacquer, all of them Xian's: its houses, shops and inn, which the area names
/// call Xian, and the room behind Master Feh's school.
///
/// Each of Xian's doors leads to a room here, and each room leads back out to
/// its door:
///
/// - LACQUER_ROOMS_B_HOUSE, LACQUER_ROOMS_B_HOUSE_NORTH and
///   LACQUER_ROOMS_B_FROM_XIAN_EAST are villagers' houses.
/// - LACQUER_ROOMS_B_INN_SOUTH opens into the inn, where
///   LACQUER_ROOMS_B_INN_TO_INN_GUEST_ROOMS leads on into its guest rooms and
///   LACQUER_ROOMS_B_INN_GUEST_ROOMS_TO_INN back.
/// - LACQUER_ROOMS_B_WEAPON_AND_ARMOR_SHOP opens into the weapon, armor and
///   item shops.
///
/// As in the village, every line has a row for each side of
/// FLAG_XIAN_HEARD_HSU_TRAPPED, and two villagers' questions have a reply for a
/// yes and one for a no. The shopkeepers and the innkeeper serve the party only
/// when spoken to across the counter, and otherwise talk.
///
/// The room behind Master Feh's school
/// (LACQUER_ROOMS_B_FROM_LACQUER_ROOMS_A_FEHS_SCHOOL) is reached only from the
/// school (LACQUER_ROOMS_A_FEHS_SCHOOL_TO_HOUSE, in fuchin_temple.c) and has
/// its own tables (Scene_GetTriggers).
///
/// Once Xian has heard of Hsu, Master Feh, Feizhi and Hsu are there, Hsu lying
/// in the bed (ANIM_HSU_IN_BED; the room's render with Hsu placed).
///
/// FLAG_HSU_RESCUED_WITHOUT_PARTY, set when the party left the desert with Hsu
/// still trapped (lamakan_desert.c), picks whether Feizhi thanks the party and
/// Master Hama or Hama alone, and what Mind Read hears from each of them.
///
/// Arriving here clears FLAG_SHOW_AREA_NAME.

#include "main_routine_work.h"
#include "module/scene/lacquer_rooms_b.h"
#include "module/scene/shared/field_effect_spawn.h"
#include "event.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "field_effect.h"
#include "text/msg_ids.h"
#include "flags.h"
#include "ability_ids.h"
#include "item_ids.h"

/// The actors of the houses, shops and inn, numbered by the rows of
/// gLacquerRoomsB_ActorPlacements. Each villager is named by what they
/// talk of before Xian hears of Hsu.
enum LacquerRoomsBActorId {
  /// A man with a topknot, in the room at LACQUER_ROOMS_B_HOUSE.
  ACTOR_MAN_ON_THE_MULBERRY_HARVEST = ACTOR_FIRST_PLACED,
  /// A woman in a green apron, in the same room.
  ACTOR_FARM_WORKER,
  /// A young man, in the room at LACQUER_ROOMS_B_HOUSE_NORTH.
  ACTOR_YOUNG_MAN_ON_SILK_ROAD,
  /// A young woman who walks about the same room.
  ACTOR_WOMAN_ON_TOLBI,
  /// An old man, in the room at LACQUER_ROOMS_B_FROM_XIAN_EAST.
  ACTOR_OLD_MAN_ON_THE_ALTIN_ROAD,
  /// An old woman with a cane, in the same room.
  ACTOR_OLD_WOMAN_ON_THE_ALTIN_MINE,
  ACTOR_XIAN_WEAPON_MERCHANT,
  ACTOR_XIAN_ARMOR_MERCHANT,
  ACTOR_XIAN_ITEM_MERCHANT,
  ACTOR_XIAN_INNKEEPER,
  /// A woman in an apron who walks about the inn's second room.
  ACTOR_WOMAN_AWAITING_MERCHANTS,
  /// A man in a cook's cap, in the inn's room at LACQUER_ROOMS_B_INN_SOUTH,
  /// with the innkeeper.
  ACTOR_COOK
};

enum {
  XIAN_WEAPON_STOCK = 16,
  XIAN_ARMOR_STOCK = 17,
  XIAN_ITEM_STOCK = 18,
  XIAN_INN = 5
};

/// The actors placed in the room behind Master Feh's school
/// (LACQUER_ROOMS_B_FROM_LACQUER_ROOMS_A_FEHS_SCHOOL), where they stand once
/// Xian has heard of Hsu. Its dialogue shows them: Master Feh says Feizhi's
/// prediction came true, Feizhi welcomes the party, and Hsu was injured on his
/// way from Lama Temple.
enum { ACTOR_FEH = ACTOR_FIRST_PLACED, ACTOR_FEIZHI, ACTOR_HSU };

/// Hsu on his back with his arms at his sides, as he lies in the bed in the
/// room behind the school (hsu.png's frame 36; the room's render).
enum { ANIM_HSU_IN_BED = 6 };

#include "shared/field_effect_create.c"
#include "shared/field_effect_spawn.c"

const struct SceneEntrance *Scene_GetEntrances(void) {
  return gLacquerRoomsB_Entrances;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) { return gLacquerRoomsB_Exits; }

/// The room behind the school places only Master Feh, Feizhi and Hsu.
const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (gMainRoutineWork.entrance ==
      LACQUER_ROOMS_B_FROM_LACQUER_ROOMS_A_FEHS_SCHOOL) {
    return gLacquerRoomsB_SchoolRoomActorPlacements;
  }
  Scene_AssignViewFlags(gLacquerRoomsB_ActorPlacements);
  return gLacquerRoomsB_ActorPlacements;
}

/// Reads the leader's facing before a conversation changes actor state.
static inline u32 LacquerRoomsB_GetLeaderFacing(void) {
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);
  return leader->facing;
}

/// Talking to Xian's weapon merchant across the counter opens the weapon shop.
/// From elsewhere the merchant talks of martial arts.
static void LacquerRoomsB_TalkToWeaponMerchant(void) {
  u32 leaderFacing = LacquerRoomsB_GetLeaderFacing();

  Event_Start();
  if (leaderFacing >= FACING_NORTHWEST + 1 &&
      leaderFacing <= FACING_NORTHEAST - 1) {
    Shop_Run(XIAN_WEAPON_STOCK, ACTOR_XIAN_WEAPON_MERCHANT);
  } else {
    if (GameFlag_Get(FLAG_XIAN_HEARD_HSU_TRAPPED) == 0) {
      Event_SetMessageId(MsgXianXianHasMartialArtsBut);
    } else {
      Event_SetMessageId(MsgXianHsusInjuryTaughtGoodLesson);
    }
    Event_ShowMessage(ACTOR_XIAN_WEAPON_MERCHANT, 0);
  }
  Event_End();
}

/// Talking to Xian's armor merchant across the counter opens the armor shop.
/// From elsewhere the merchant talks of armor and kung fu.
static void LacquerRoomsB_TalkToArmorMerchant(void) {
  u32 leaderFacing = LacquerRoomsB_GetLeaderFacing();

  Event_Start();
  if (leaderFacing >= FACING_NORTHWEST + 1 &&
      leaderFacing <= FACING_NORTHEAST - 1) {
    Shop_Run(XIAN_ARMOR_STOCK, ACTOR_XIAN_ARMOR_MERCHANT);
  } else {
    if (GameFlag_Get(FLAG_XIAN_HEARD_HSU_TRAPPED) == 0) {
      Event_SetMessageId(MsgXianMartialArtistsCantUseHeavy);
    } else {
      Event_SetMessageId(MsgXianKungFuMakesQuickDoes);
    }
    Event_ShowMessage(ACTOR_XIAN_ARMOR_MERCHANT, 0);
  }
  Event_End();
}

/// Talking to Xian's item merchant across the counter opens the item shop.
static void LacquerRoomsB_TalkToItemMerchant(void) {
  u32 leaderFacing = LacquerRoomsB_GetLeaderFacing();

  Event_Start();
  if (leaderFacing >= FACING_NORTHWEST + 1 &&
      leaderFacing <= FACING_NORTHEAST - 1) {
    Shop_Run(XIAN_ITEM_STOCK, ACTOR_XIAN_ITEM_MERCHANT);
  } else if (GameFlag_Get(FLAG_XIAN_HEARD_HSU_TRAPPED) == 0) {
    Event_SetMessageId(MsgXianXiansSpecialArmorNotSuited);
    // "Xian's special armor is not suited to warriors... It is..."
    Event_ShowMessage(ACTOR_XIAN_ITEM_MERCHANT, 0);
  } else {
    Event_SetMessageId(MsgXianDoPlanCrossDesertWarrior);
    // "Do you plan to cross the desert, warrior?"
    // Yes: "It will take much strength. Perhaps you should buy many..."
    // No: "You are wise, warrior. The wise man knows to avoid danger."
    Event_AskYesNo(ACTOR_XIAN_ITEM_MERCHANT, 0);
  }
  Event_End();
}

/// Talking to Xian's innkeeper across the counter opens the inn. From elsewhere
/// the innkeeper talks of towns and travelers.
static void LacquerRoomsB_TalkToInnkeeper(void) {
  u32 leaderFacing = LacquerRoomsB_GetLeaderFacing();

  Event_Start();
  if (leaderFacing < FACING_SOUTHEAST || leaderFacing > FACING_NORTHEAST) {
    Inn_CheckIn(XIAN_INN, ACTOR_XIAN_INNKEEPER);
  } else {
    if (GameFlag_Get(FLAG_XIAN_HEARD_HSU_TRAPPED) == 0) {
      Event_SetMessageId(MsgXianWarriorsShouldLearnAboutTowns);
    } else {
      Event_SetMessageId(MsgXianWeHaveFewCustomersThese);
    }
    Event_ShowMessage(ACTOR_XIAN_INNKEEPER, 0);
  }
  Event_End();
}

/// A young man in a house in Xian asks whether something is wrong on Silk Road.
static void LacquerRoomsB_AskAboutSilkRoad(void) {
  Event_Start();
  Event_SetMessageId(MsgXianSomethingWrongOnSilkRoad);
  // "Is something wrong on Silk Road? In Lamakan Desert, too?"
  // Yes: "That is terrible. No one can use Silk Road!"
  // No: "The desert is fine? That is good to hear."
  Event_AskYesNo(ACTOR_YOUNG_MAN_ON_SILK_ROAD, 0);
  Event_End();
}

/// An old man in a house in Xian, ACTOR_OLD_MAN_ON_THE_ALTIN_ROAD, asks after
/// the news of Hsu whether the monsters in Altin spat water.
static void LacquerRoomsB_AskAboutAltinMonsters(void) {
  Event_Start();
  Event_SetMessageId(MsgXianDidMonstersInAltinSpit);
  // "Did the monsters in Altin spit water?"
  // Yes: "The town was flooded by monsters? That takes much water. How..."
  // No: "It is not true? But why would Feizhi lie?"
  Event_AskYesNo(ACTOR_OLD_MAN_ON_THE_ALTIN_ROAD, 0);
  Event_End();
}

/// Feizhi thanks the party and Master Hama for freeing Hsu, in Xian.
static void LacquerRoomsB_FeizhiThanksParty(void) {
  Event_Start();
  Event_SetMessageId(MsgXianWarriorWelcome);
  // "Oh, warrior! Welcome!"
  Event_ShowMessageThenWait(ACTOR_FEIZHI, 0, 20);
  Actor_FaceActor(ACTOR_FEIZHI, ACTOR_HSU, 0);
  Event_Wait(60);
  Actor_FaceActor(ACTOR_FEIZHI, ACTOR_PARTY_LEADER, 0);
  Event_Wait(20);
  // "Hsu was saved, thanks to you and Master Hama. You have my..."
  Event_ShowMessage(ACTOR_FEIZHI, 0);
  Event_End();
}

/// The room behind the school has its own triggers.
const struct Trigger *Scene_GetTriggers(void) {
  if (gMainRoutineWork.entrance ==
      LACQUER_ROOMS_B_FROM_LACQUER_ROOMS_A_FEHS_SCHOOL) {
    return gLacquerRoomsB_SchoolRoomTriggers;
  }
  return gLacquerRoomsB_Triggers;
}

/// Opens the room through a window transition and places hidden still actors of
/// SPRITE_CHEST's sheet (Actor_CreateConfigured).
s32 Scene_Initialize(void) {
  s16 entrance;

  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  entrance = gMainRoutineWork.entrance;
  // XXX (no evidence): what these hidden chests are for; the player neither
  // sees nor meets them.
  if (entrance == LACQUER_ROOMS_B_INN_SOUTH ||
      entrance == LACQUER_ROOMS_B_INN_FROM_INN_GUEST_ROOMS) {
    Actor_CreateConfigured(248, 0, 417, SPRITE_CHEST);
  } else if (entrance == LACQUER_ROOMS_B_WEAPON_AND_ARMOR_SHOP) {
    Actor_CreateConfigured(460, 0, 568, SPRITE_CHEST);
    Actor_CreateConfigured(484, 0, 568, SPRITE_CHEST);
  } else if (entrance == LACQUER_ROOMS_B_FROM_LACQUER_ROOMS_A_FEHS_SCHOOL) {
    GameFlag_Clear(FLAG_SHOW_AREA_NAME);
    Actor_SetAnimation(ACTOR_HSU, ANIM_HSU_IN_BED);
  }
  return 0;
}

#include "shared/field_effect_duration_scripts.c"

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

struct SceneEntrance gLacquerRoomsB_Entrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {LACQUER_ROOMS_B_HOUSE, CONDITION_ALWAYS, 128, 0, 184, FACING_NORTH, 0, 24,
     8, 248, 208, 0},
    {LACQUER_ROOMS_B_HOUSE_NORTH, CONDITION_ALWAYS, 448, 0, 200, FACING_NORTH,
     0, 288, 8, 592, 224, 0},
    {LACQUER_ROOMS_B_FROM_XIAN_EAST, CONDITION_ALWAYS, 736, 0, 200,
     FACING_NORTH, 0, 608, 8, 848, 224, 0},
    {LACQUER_ROOMS_B_INN_SOUTH, CONDITION_ALWAYS, 160, 0, 472, FACING_NORTH, 0,
     48, 288, 336, 496, 0},
    {LACQUER_ROOMS_B_INN_GUEST_ROOMS_FROM_INN, CONDITION_ALWAYS, 472, 0, 360,
     FACING_EAST, 0, 384, 288, 672, 496, 0},
    {LACQUER_ROOMS_B_WEAPON_AND_ARMOR_SHOP, CONDITION_ALWAYS, 512, 0, 696,
     FACING_NORTH, 0, 352, 496, 656, 720, 0},
    {LACQUER_ROOMS_B_INN_FROM_INN_GUEST_ROOMS, CONDITION_ALWAYS, 120, 0, 360,
     FACING_EAST, 0, 48, 288, 336, 496, 0},
    {LACQUER_ROOMS_B_FROM_LACQUER_ROOMS_A_FEHS_SCHOOL, CONDITION_ALWAYS, 120, 0,
     696, FACING_NORTH, 0, 64, 544, 304, 744, 0},
    {SCENE_TABLE_END},
};

u32 gLacquerRoomsB_Exits[] = {
    SCENE_EXITS(LacquerRoomsB),
    SCENE_EXIT(LACQUER_ROOMS_B_WEAPON_AND_ARMOR_SHOP_TO_XIAN, Xian,
               XIAN_FROM_WEAPON_AND_ARMOR_SHOP),
    SCENE_EXIT(LACQUER_ROOMS_B_HOUSE_NORTH_TO_XIAN, Xian,
               XIAN_FROM_HOUSE_NORTH),
    SCENE_EXIT(LACQUER_ROOMS_B_TO_XIAN_EAST, Xian,
               XIAN_FROM_LACQUER_ROOMS_B_EAST),
    SCENE_EXIT(LACQUER_ROOMS_B_TO_XIAN_SOUTH, Xian,
               XIAN_FROM_LACQUER_ROOMS_B_SOUTH),
    SCENE_EXIT(LACQUER_ROOMS_B_INN_GUEST_ROOMS_TO_INN, LacquerRoomsB,
               LACQUER_ROOMS_B_INN_FROM_INN_GUEST_ROOMS),
    SCENE_EXIT(LACQUER_ROOMS_B_TO_XIAN_WEST, Xian,
               XIAN_FROM_LACQUER_ROOMS_B_WEST),
    SCENE_EXIT(LACQUER_ROOMS_B_INN_TO_INN_GUEST_ROOMS, LacquerRoomsB,
               LACQUER_ROOMS_B_INN_GUEST_ROOMS_FROM_INN),
    SCENE_EXIT(LACQUER_ROOMS_B_TO_LACQUER_ROOMS_A_FEHS_SCHOOL, LacquerRoomsA,
               LACQUER_ROOMS_A_FEHS_SCHOOL_FROM_HOUSE),
    SCENE_EXITS_END,
};

// clang-format off
struct ActorPlacement gLacquerRoomsB_ActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_TOPKNOT, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        144, 0, 152, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_APRON, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        136, 0, 80, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_IN_BLUE_JACKET, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        496, 0, 72, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_BLACK_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_WANDER,
        368, 0, 128, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_BLUE, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        760, 0, 184, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_CANE, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        680, 0, 80, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_MAN_IN_RED, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        472, 0, 600, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_IN_GREEN, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        448, 0, 600, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_PONYTAIL, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        552, 0, 600, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_DARK_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        232, 0, 448, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PINK_APRON, CONDITION_IN_VIEW, MOVEMENT_TYPE_WANDER,
        568, 0, 416, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_COOK, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        280, 0, 408, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gLacquerRoomsB_SchoolRoomActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_FEH, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), MOVEMENT_TYPE_STAND,
        224, 0, 656, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_FEIZHI, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), MOVEMENT_TYPE_STAND,
        200, 0, 704, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_HSU, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), MOVEMENT_TYPE_STAND,
        224, 0, 704, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

struct Trigger gLacquerRoomsB_Triggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, LACQUER_ROOMS_B_WEAPON_AND_ARMOR_SHOP_TO_XIAN },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, LACQUER_ROOMS_B_HOUSE_NORTH_TO_XIAN },
    { TRIGGER_EXIT, 3, CONDITION_ALWAYS, LACQUER_ROOMS_B_TO_XIAN_EAST },
    { TRIGGER_EXIT, 4, CONDITION_ALWAYS, LACQUER_ROOMS_B_TO_XIAN_SOUTH },
    { TRIGGER_EXIT, 5, CONDITION_ALWAYS, LACQUER_ROOMS_B_INN_GUEST_ROOMS_TO_INN },
    { TRIGGER_EXIT, 6, CONDITION_ALWAYS, LACQUER_ROOMS_B_TO_XIAN_WEST },
    { TRIGGER_EXIT, 7, CONDITION_ALWAYS, LACQUER_ROOMS_B_INN_TO_INN_GUEST_ROOMS },
    { TRIGGER_TALK, ACTOR_MAN_ON_THE_MULBERRY_HARVEST, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsTheMulberryHarvestIsOverNow) },
    { TRIGGER_TALK, ACTOR_FARM_WORKER, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsFarmWorkIsVeryHardWarriors) },
    { TRIGGER_TALK, ACTOR_YOUNG_MAN_ON_SILK_ROAD, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsXianSilkIsSentOnSilk) },
    { TRIGGER_TALK, ACTOR_WOMAN_ON_TOLBI, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsSilkRoadEndsInTolbiIt) },
    { TRIGGER_TALK, ACTOR_OLD_MAN_ON_THE_ALTIN_ROAD, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsFollowSilkRoadWhenYouSee) },
    { TRIGGER_TALK, ACTOR_OLD_WOMAN_ON_THE_ALTIN_MINE, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsThereIsAMineInAltin) },
    { TRIGGER_TALK, ACTOR_XIAN_WEAPON_MERCHANT, FLAG_XIAN_HEARD_HSU_TRAPPED, EVENT(LacquerRoomsB_TalkToWeaponMerchant) },
    { TRIGGER_TALK, ACTOR_XIAN_ARMOR_MERCHANT, FLAG_XIAN_HEARD_HSU_TRAPPED, EVENT(LacquerRoomsB_TalkToArmorMerchant) },
    { TRIGGER_TALK, ACTOR_XIAN_ITEM_MERCHANT, FLAG_XIAN_HEARD_HSU_TRAPPED, EVENT(LacquerRoomsB_TalkToItemMerchant) },
    { TRIGGER_TALK, ACTOR_XIAN_INNKEEPER, FLAG_XIAN_HEARD_HSU_TRAPPED, EVENT(LacquerRoomsB_TalkToInnkeeper) },
    { TRIGGER_TALK, ACTOR_WOMAN_AWAITING_MERCHANTS, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsNormallyManyMerchantsComeToThe) },
    { TRIGGER_TALK, ACTOR_COOK, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsAllThePeopleWhoCameTo) },
    { TRIGGER_TALK, ACTOR_MAN_ON_THE_MULBERRY_HARVEST, CONDITION_ALWAYS, MSGNO(MsgXianWhereAreTheMerchantsNoOne) },
    { TRIGGER_TALK, ACTOR_FARM_WORKER, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsMerchantsBringUsCoinsTheyMust) },
    { TRIGGER_TALK, ACTOR_YOUNG_MAN_ON_SILK_ROAD, CONDITION_ALWAYS, EVENT(LacquerRoomsB_AskAboutSilkRoad) },
    { TRIGGER_TALK, ACTOR_WOMAN_ON_TOLBI, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsNoneCanPassThroughLamakanThis) },
    { TRIGGER_TALK, ACTOR_OLD_MAN_ON_THE_ALTIN_ROAD, CONDITION_ALWAYS, EVENT(LacquerRoomsB_AskAboutAltinMonsters) },
    { TRIGGER_TALK, ACTOR_OLD_WOMAN_ON_THE_ALTIN_MINE, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsTheMineInAltinFilledWith) },
    { TRIGGER_TALK, ACTOR_XIAN_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT(LacquerRoomsB_TalkToWeaponMerchant) },
    { TRIGGER_TALK, ACTOR_XIAN_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT(LacquerRoomsB_TalkToArmorMerchant) },
    { TRIGGER_TALK, ACTOR_XIAN_ITEM_MERCHANT, CONDITION_ALWAYS, EVENT(LacquerRoomsB_TalkToItemMerchant) },
    { TRIGGER_TALK, ACTOR_XIAN_INNKEEPER, CONDITION_ALWAYS, EVENT(LacquerRoomsB_TalkToInnkeeper) },
    { TRIGGER_TALK, ACTOR_WOMAN_AWAITING_MERCHANTS, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsWhenLamakanDesertBurnsItIs) },
    { TRIGGER_TALK, ACTOR_COOK, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsHotFoodIsGoodForCrossing) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_MULBERRY_HARVEST, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsSoonMasterHammetWillComeHe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FARM_WORKER, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsFarmWorkIsHardOnThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_ON_SILK_ROAD, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsSilkRoadIsFullOfSilk) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_TOLBI, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsMerchantsSayTolbiIsVeryBeautiful) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_THE_ALTIN_ROAD, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsAltinHasManyMenAllScary) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_THE_ALTIN_MINE, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsMiningIsVeryHardWorkIt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_XIAN_WEAPON_MERCHANT, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsSomeSayFightingWithWeaponsIs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_XIAN_ARMOR_MERCHANT, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsInWarriorBattlesYouFightWith) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_XIAN_ITEM_MERCHANT, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsXianIsAVillageOfCraftsmen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_XIAN_INNKEEPER, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsWhenYouGoToANew) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_AWAITING_MERCHANTS, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsWhenManyMerchantsComeMyBusiness) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_COOK, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsThereAreFewerTravelersSoFewer) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_MULBERRY_HARVEST, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsWhyDoesMasterHammetNotCome) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FARM_WORKER, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsMerchantsSellSilkItIsLike) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_ON_SILK_ROAD, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsFeizhiSaidTheDesertBecameEvil) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_TOLBI, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsTheDesertWasVeryHotNow) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_THE_ALTIN_ROAD, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsTheVillageIsFloodedBecauseOf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_THE_ALTIN_MINE, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsWaterMonstersHowTerribleForAltin) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_XIAN_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsHsuWasPummeledByHugeBoulders) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_XIAN_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsWeShouldSellArmorToKung) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_XIAN_ITEM_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsYouLoseStrengthInHeatUse) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_XIAN_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsCaravansCannotBringMerchantsHereWhen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_AWAITING_MERCHANTS, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsTheMerchantsDoNotComeBecause) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_COOK, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsEatWarriorIWillAddMany) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), 100, FLAG_LACQUER_ROOMS_B_ELIXIR_BARREL, FIND_ITEM(ITEM_ELIXIR) },
    { EXAMINE_TRIGGER(EXAMINE_JAR), 101, FLAG_LACQUER_ROOMS_B_SLEEP_BOMB_JAR, FIND_ITEM(ITEM_SLEEP_BOMB) },
    { EXAMINE_TRIGGER(EXAMINE_OVEN), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgXianInteriorsTheyreMakingStirFryTheCabbage)) },
    { EXAMINE_TRIGGER(EXAMINE_OVEN), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgXianInteriorsItsTheXianGourmetTheBest)) },
    { EXAMINE_TRIGGER(EXAMINE_OVEN), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgXianInteriorsTheyreSteamedDumplingsTheyreGreatPiping)) },
    { EXAMINE_TRIGGER(EXAMINE_OVEN), 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgXianInteriorsIsaacAndHisFriendsAreShocked)) },
    { EXAMINE_TRIGGER(EXAMINE_OVEN), 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgXianInteriorsHesMakingHotBeanCurdTastes)) },
    { EXAMINE_TRIGGER(EXAMINE_SHELF) | TRIGGER_FLAG_FACING | FACING_NORTH, 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgXianInteriorsThereAreManyChopsticksButNot)) },
    { EXAMINE_TRIGGER(EXAMINE_SHELF), 206, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgXianInteriorsThereAreMedicinalHerbsTheyHave)) },
    { EXAMINE_TRIGGER(EXAMINE_SHELF), 207, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgXianInteriorsTheresAKimonoItLooksLike)) },
    { EXAMINE_TRIGGER(EXAMINE_SHELF), 208, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgXianInteriorsItsJustASalesReportIt)) },
    { TRIGGER_TABLE_END },
};

struct Trigger gLacquerRoomsB_SchoolRoomTriggers[] = {
    { TRIGGER_EXIT, 8, CONDITION_ALWAYS, LACQUER_ROOMS_B_TO_LACQUER_ROOMS_A_FEHS_SCHOOL },
    { TRIGGER_TALK, ACTOR_FEH, FLAG_HSU_RESCUED_WITHOUT_PARTY, MSGNO(MsgXianInteriorsFeizhisPredictionCameTrueNowI) },
    { TRIGGER_TALK, ACTOR_FEIZHI, FLAG_HSU_RESCUED_WITHOUT_PARTY, EVENT(LacquerRoomsB_FeizhiThanksParty) },
    { TRIGGER_TALK, ACTOR_HSU, FLAG_HSU_RESCUED_WITHOUT_PARTY, MSGNO(MsgXianInteriorsIWasInjuredByRocksOn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FEH, FLAG_HSU_RESCUED_WITHOUT_PARTY, MSGNO(MsgXianInteriorsFeizhisAbilityToSenseTroubleIs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FEIZHI, FLAG_HSU_RESCUED_WITHOUT_PARTY, MSGNO(MsgXianInteriorsMasterHamaHasGreatPowerYou) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HSU, FLAG_HSU_RESCUED_WITHOUT_PARTY, MSGNO(MsgXianInteriorsIKnewHelpWouldComeWhen) },
    { TRIGGER_TALK, ACTOR_FEH, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsFeizhisPredictionCameTrueNowIBelieve) },
    { TRIGGER_TALK, ACTOR_FEIZHI, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsHsuWasTrappedUnderTheRocks) },
    { TRIGGER_TALK, ACTOR_HSU, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsIWasInjuredByBouldersOn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FEH, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsFeizhisAbilityToForeseeIsReal) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FEIZHI, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsMasterHamaHasGreatPowerSome) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HSU, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsIKnewHelpWouldComeWhenI) },
    { TRIGGER_TABLE_END },
};
// clang-format on
