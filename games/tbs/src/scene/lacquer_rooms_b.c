/// @file
/// Lacquer rooms B (SceneId_LacquerRoomsB), a sheet of rooms drawn in red
/// lacquer, all of them Xian's: its houses, shops and inn, which the area names
/// call Xian, and the room behind Master Feh's school.
///
/// Each of Xian's doors leads to a room here, and each room leads back out to
/// its door. LACQUER_ROOMS_B_HOUSE, LACQUER_ROOMS_B_HOUSE_NORTH and
/// LACQUER_ROOMS_B_FROM_XIAN_EAST are villagers' houses;
/// LACQUER_ROOMS_B_INN_SOUTH opens into the inn, where
/// LACQUER_ROOMS_B_INN_TO_INN_GUEST_ROOMS leads on into its guest rooms and
/// LACQUER_ROOMS_B_INN_GUEST_ROOMS_TO_INN back; and
/// LACQUER_ROOMS_B_WEAPON_AND_ARMOR_SHOP opens into the weapon, armor and item
/// shops. As in the village, every line has a row for each side of
/// FLAG_XIAN_HEARD_HSU_TRAPPED, and two villagers' questions have a reply
/// for a yes and one for a no. The shopkeepers and the innkeeper serve the
/// party only when spoken to across the counter, and otherwise talk.
///
/// The room behind Master Feh's school
/// (LACQUER_ROOMS_B_FROM_LACQUER_ROOMS_A_FEHS_SCHOOL) is reached only
/// from the school (LACQUER_ROOMS_A_FEHS_SCHOOL_TO_HOUSE, in
/// fuchin_temple.c) and has its own tables (LacquerRoomsB_GetEvents). Once Xian
/// has heard of Hsu, Master Feh, Feizhi and Hsu are there, Hsu lying in the bed
/// (ANIM_HSU_IN_BED; the room's render with Hsu placed).
/// FLAG_HSU_RESCUED_WITHOUT_PARTY, set when the party left the desert with Hsu
/// still trapped (lamakan_desert.c), picks whether Feizhi thanks the party and
/// Master Hama or Hama alone, and what Mind Read hears from each of them.
/// Arriving here clears FLAG_SHOW_LOCATION_NAME.

#include "save_block.h"
#include "unit/scene/lacquer_rooms_b.h"
#include "unit/scene/common/effect_spawn.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/field_services.h"
#include "types.h"
#include "script_layer.h"
#include "map_tables.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "field_effect.h"
#include "text/msg_ids.h"
#include "object_event.h"
#include "flags.h"

/// The object events of the houses, shops and inn, numbered by the rows of
/// gLacquerRoomsB_ObjectEventTemplates. Each villager is named by what they
/// talk of before Xian hears of Hsu.
enum LacquerRoomsBLocalId {
  /// A man with a topknot, in the room at LACQUER_ROOMS_B_HOUSE.
  /// ★ Named from the game: "The mulberry harvest is over. Now there is much
  /// food for the caterpillars!" (MsgXianInteriorsTheMulberryHarvestIsOverNow)
  LOCALID_MAN_ON_THE_MULBERRY_HARVEST = LOCALID_FIRST_PLACED,
  /// A woman in a green apron, in the same room.
  /// ★ Named from the game: "Farm work is very hard."
  /// (MsgXianInteriorsFarmWorkIsVeryHardWarriors)
  LOCALID_FARM_WORKER,
  /// A young man, in the room at LACQUER_ROOMS_B_HOUSE_NORTH.
  /// ★ Named from the game: "Xian silk is sent on Silk Road to Tolbi."
  /// (MsgXianInteriorsXianSilkIsSentOnSilk)
  LOCALID_YOUNG_MAN_ON_SILK_ROAD,
  /// A young woman who walks about the same room.
  /// ★ Named from the game: "Silk Road ends in Tolbi. It is a big, big town."
  /// (MsgXianInteriorsSilkRoadEndsInTolbiIt)
  LOCALID_WOMAN_ON_TOLBI,
  /// An old man, in the room at LACQUER_ROOMS_B_FROM_XIAN_EAST.
  /// ★ Named from the game: "Follow Silk Road. When you see the mountain fork,
  /// go north." (MsgXianInteriorsFollowSilkRoadWhenYouSee)
  LOCALID_OLD_MAN_ON_THE_ALTIN_ROAD,
  /// An old woman with a cane, in the same room.
  /// ★ Named from the game: "There is a mine in Altin. There is lots of steel
  /// and lots of gems." (MsgXianInteriorsThereIsAMineInAltin)
  LOCALID_OLD_WOMAN_ON_THE_ALTIN_MINE,
  /// ★ Named from the game: "it is not strange to sell weapons in Xian."
  /// (MsgXianXianHasMartialArtsBut)
  LOCALID_XIAN_WEAPON_MERCHANT,
  /// ★ Named from the game: "We should sell armor to kung fu masters, as
  /// well." (MsgXianInteriorsWeShouldSellArmorToKung)
  LOCALID_XIAN_ARMOR_MERCHANT,
  /// ★ Named from the game: "Use my goods to recover your strength."
  /// (MsgXianInteriorsYouLoseStrengthInHeatUse)
  LOCALID_XIAN_ITEM_MERCHANT,
  /// ★ Named from the game: "Welcome to our inn." (MsgInnWelcome)
  LOCALID_XIAN_INNKEEPER,
  /// A woman in an apron who walks about the inn's second room.
  /// ★ Named from the game: "Normally, many merchants come to the village, but
  /// they have not come yet." (MsgXianInteriorsNormallyManyMerchantsComeToThe)
  LOCALID_WOMAN_AWAITING_MERCHANTS,
  /// A man in a cook's cap, in the inn's room at LACQUER_ROOMS_B_INN_SOUTH,
  /// with the innkeeper.
  /// ★ Named from the game: "All the people who came to eat are from Xian."
  /// (MsgXianInteriorsAllThePeopleWhoCameTo)
  LOCALID_COOK
};

enum {
  XIAN_WEAPON_STOCK = 16,
  XIAN_ARMOR_STOCK = 17,
  XIAN_ITEM_STOCK = 18,
  XIAN_INN = 5
};

/// The object events placed in the room behind Master Feh's school
/// (LACQUER_ROOMS_B_FROM_LACQUER_ROOMS_A_FEHS_SCHOOL), where they stand
/// once Xian has heard of Hsu, as its dialogue shows them: Master Feh says
/// Feizhi's prediction came true, Feizhi welcomes the party, and Hsu was
/// injured on his way from Lama Temple.
enum {
  /// ★ Named from the game: "You warriors should visit Master Feh's school."
  /// (MsgXianYouWarriorsShouldVisitMasterFehs)
  LOCALID_FEH = LOCALID_FIRST_PLACED,
  /// ★ Named from the game: "Feizhi's prediction came true."
  /// (MsgXianInteriorsFeizhisPredictionCameTrueNowI)
  LOCALID_FEIZHI,
  /// ★ Named from the game: "Hsu was trapped under the rocks"
  /// (MsgXianInteriorsHsuWasTrappedUnderTheRocks)
  LOCALID_HSU
};

/// Hsu on his back with his arms at his sides, as he lies in the bed in the
/// room behind the school (spr_041.png's frame 36; the room's render).
enum { ANIM_HSU_IN_BED = 6 };

#include "common/effect_create.h"
#include "common/effect_spawn.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"

const struct SceneEntrance *LacquerRoomsB_GetEntrances(void) {
  return gLacquerRoomsB_Entrances;
}

const struct SceneExitRegion *LacquerRoomsB_GetRegions(void) { return NULL; }

const u32 *LacquerRoomsB_GetExits(void) { return gLacquerRoomsB_Exits; }

/// The room behind the school places only Master Feh, Feizhi and Hsu. Every
/// other room shares one table, whose condition 0 Scene_AssignViewFlags
/// replaces with whether each object event stands inside the view bounds of the
/// room entered.
const struct ObjectEventTemplate *LacquerRoomsB_GetObjectEventTemplates(void) {
  if (gSaveBlock.entrance == LACQUER_ROOMS_B_FROM_LACQUER_ROOMS_A_FEHS_SCHOOL) {
    return gLacquerRoomsB_SchoolRoomObjectEventTemplates;
  }
  Scene_AssignViewFlags(gLacquerRoomsB_ObjectEventTemplates);
  return gLacquerRoomsB_ObjectEventTemplates;
}

/// Reads the leader's facing before a conversation changes object event state.
static inline u32 LacquerRoomsB_GetLeaderFacing(void) {
  struct ObjectEvent *leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  return leader->facing;
}

/// Talking to Xian's weapon merchant: across the counter, the weapon shop;
/// from elsewhere, talk of martial arts.
static void LacquerRoomsB_TalkToWeaponMerchant(void) {
  u32 leaderFacing = LacquerRoomsB_GetLeaderFacing();

  FieldScript_Start();
  if (leaderFacing >= FACING_NORTHWEST + 1 &&
      leaderFacing <= FACING_NORTHEAST - 1) {
    Shop_Run(XIAN_WEAPON_STOCK, LOCALID_XIAN_WEAPON_MERCHANT);
  } else {
    if (GameFlag_Get(FLAG_XIAN_HEARD_HSU_TRAPPED) == 0) {
      FieldScript_SetMessageId(MsgXianXianHasMartialArtsBut);
    } else {
      FieldScript_SetMessageId(MsgXianHsusInjuryTaughtGoodLesson);
    }
    FieldScript_ShowMessageAndWait(LOCALID_XIAN_WEAPON_MERCHANT, 0);
  }
  FieldScript_End();
}

/// Talking to Xian's armor merchant: across the counter, the armor shop;
/// from elsewhere, talk of armor and kung fu.
static void LacquerRoomsB_TalkToArmorMerchant(void) {
  u32 leaderFacing = LacquerRoomsB_GetLeaderFacing();

  FieldScript_Start();
  if (leaderFacing >= FACING_NORTHWEST + 1 &&
      leaderFacing <= FACING_NORTHEAST - 1) {
    Shop_Run(XIAN_ARMOR_STOCK, LOCALID_XIAN_ARMOR_MERCHANT);
  } else {
    if (GameFlag_Get(FLAG_XIAN_HEARD_HSU_TRAPPED) == 0) {
      FieldScript_SetMessageId(MsgXianMartialArtistsCantUseHeavy);
    } else {
      FieldScript_SetMessageId(MsgXianKungFuMakesQuickDoes);
    }
    FieldScript_ShowMessageAndWait(LOCALID_XIAN_ARMOR_MERCHANT, 0);
  }
  FieldScript_End();
}

/// Talking to Xian's item merchant: across the counter, the item shop; from
/// elsewhere, talk of armor, or, once Xian has heard of Hsu, whether the party
/// will cross the desert.
static void LacquerRoomsB_TalkToItemMerchant(void) {

  u32 leaderFacing = LacquerRoomsB_GetLeaderFacing();

  FieldScript_Start();
  if (leaderFacing >= FACING_NORTHWEST + 1 &&
      leaderFacing <= FACING_NORTHEAST - 1) {
    Shop_Run(XIAN_ITEM_STOCK, LOCALID_XIAN_ITEM_MERCHANT);
  } else if (GameFlag_Get(FLAG_XIAN_HEARD_HSU_TRAPPED) == 0) {
    FieldScript_SetMessageId(MsgXianXiansSpecialArmorNotSuited);
    FieldScript_ShowMessageAndWait(LOCALID_XIAN_ITEM_MERCHANT, 0);
  } else {
    FieldScript_SetMessageId(MsgXianDoPlanCrossDesertWarrior);
    FieldScript_AskYesNoAndShowResponse(LOCALID_XIAN_ITEM_MERCHANT, 0);
  }
  FieldScript_End();
}

/// Talking to Xian's innkeeper: across the counter, the inn; from elsewhere,
/// talk of towns and travelers.
static void LacquerRoomsB_TalkToInnkeeper(void) {

  u32 leaderFacing = LacquerRoomsB_GetLeaderFacing();

  FieldScript_Start();
  if (leaderFacing < FACING_SOUTHEAST || leaderFacing > FACING_NORTHEAST) {
    Inn_CheckIn(XIAN_INN, LOCALID_XIAN_INNKEEPER);
  } else {
    if (GameFlag_Get(FLAG_XIAN_HEARD_HSU_TRAPPED) == 0) {
      FieldScript_SetMessageId(MsgXianWarriorsShouldLearnAboutTowns);
    } else {
      FieldScript_SetMessageId(MsgXianWeHaveFewCustomersThese);
    }
    FieldScript_ShowMessageAndWait(LOCALID_XIAN_INNKEEPER, 0);
  }
  FieldScript_End();
}

/// A young man in a house in Xian asks whether something is wrong on Silk
/// Road. He is LOCALID_YOUNG_MAN_ON_SILK_ROAD, after the news of Hsu, and asks
/// about Lamakan Desert too. A yes: no one can use Silk Road; a no: it is good
/// to hear the desert is fine.
static void LacquerRoomsB_AskAboutSilkRoad(void) {
  FieldScript_Start();
  FieldScript_SetMessageId(MsgXianSomethingWrongOnSilkRoad);
  FieldScript_AskYesNoAndShowResponse(LOCALID_YOUNG_MAN_ON_SILK_ROAD, 0);
  FieldScript_End();
}

/// An old man in a house in Xian, LOCALID_OLD_MAN_ON_THE_ALTIN_ROAD, asks after
/// the news of Hsu whether the monsters in Altin spat water. A yes: how
/// strange; a no: but why would Feizhi lie?
static void LacquerRoomsB_AskAboutAltinMonsters(void) {
  FieldScript_Start();
  FieldScript_SetMessageId(MsgXianDidMonstersInAltinSpit);
  FieldScript_AskYesNoAndShowResponse(LOCALID_OLD_MAN_ON_THE_ALTIN_ROAD, 0);
  FieldScript_End();
}

/// Feizhi thanks the party and Master Hama for freeing Hsu, in Xian. She is in
/// the room behind Master Feh's school, when the party helped free Hsu
/// (FLAG_HSU_RESCUED_WITHOUT_PARTY clear); she welcomes the party, looks at
/// Hsu, then thanks the party and Master Hama. "Oh, warrior! Welcome!"
static void LacquerRoomsB_FeizhiThanksParty(void) {
  FieldScript_Start();
  FieldScript_SetMessageId(MsgXianWarriorWelcome);
  FieldScript_ShowMessageThenWait(LOCALID_FEIZHI, 0, 20);
  ObjectMotion_SetAngleToward(LOCALID_FEIZHI, LOCALID_HSU, 0);
  FieldScript_WaitFrames(60);
  ObjectMotion_SetAngleToward(LOCALID_FEIZHI, LOCALID_PARTY_LEADER, 0);
  FieldScript_WaitFrames(20);
  FieldScript_ShowMessageAndWait(LOCALID_FEIZHI, 0);
  FieldScript_End();
}

/// The room behind the school has its own map events.
const struct MapEvent *LacquerRoomsB_GetEvents(void) {
  if (gSaveBlock.entrance == LACQUER_ROOMS_B_FROM_LACQUER_ROOMS_A_FEHS_SCHOOL) {
    return gLacquerRoomsB_SchoolRoomEvents;
  }
  return gLacquerRoomsB_Events;
}

/// Opens the room through a window transition and places hidden still objects
/// of SPRITE_CHEST's sheet (ObjectEvent_CreateConfiguredObject): one in the
/// inn (LACQUER_ROOMS_B_INN_SOUTH and
/// LACQUER_ROOMS_B_INN_FROM_INN_GUEST_ROOMS), two in the shops
/// (LACQUER_ROOMS_B_WEAPON_AND_ARMOR_SHOP). In the room behind the school, it
/// clears FLAG_SHOW_LOCATION_NAME and lays Hsu in the bed.
s32 LacquerRoomsB_Initialize(void) {
  s16 entrance;

  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  entrance = gSaveBlock.entrance;
  // XXX (no evidence): what these hidden chests are for; the player neither
  // sees nor meets them.
  if (entrance == LACQUER_ROOMS_B_INN_SOUTH ||
      entrance == LACQUER_ROOMS_B_INN_FROM_INN_GUEST_ROOMS) {
    ObjectEvent_CreateConfiguredObject(PIXELS(248), 0, PIXELS(417),
                                       SPRITE_CHEST);
  } else if (entrance == LACQUER_ROOMS_B_WEAPON_AND_ARMOR_SHOP) {
    ObjectEvent_CreateConfiguredObject(PIXELS(460), 0, PIXELS(568),
                                       SPRITE_CHEST);
    ObjectEvent_CreateConfiguredObject(PIXELS(484), 0, PIXELS(568),
                                       SPRITE_CHEST);
  } else if (entrance == LACQUER_ROOMS_B_FROM_LACQUER_ROOMS_A_FEHS_SCHOOL) {
    GameFlag_Clear(FLAG_SHOW_LOCATION_NAME);
    Import_ObjectEventSetAnimation(LOCALID_HSU, ANIM_HSU_IN_BED);
  }
  return 0;
}

s32 gFieldEffect_ShortScript[] = {
    SC_WAIT(1),           SC_LOOP(22, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gFieldEffect_MediumScript[] = {
    SC_WAIT(1),           SC_LOOP(44, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gFieldEffect_LongScript[] = {
    SC_WAIT(1),           SC_LOOP(126, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),           SC_FREE,
};

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
    {MAP_TABLE_END},
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
struct ObjectEventTemplate gLacquerRoomsB_ObjectEventTemplates[] = {
    { SPRITE_MAN_WITH_TOPKNOT, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(144), 0, PIXELS(152), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_IN_GREEN_APRON, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(136), 0, PIXELS(80), FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_IN_BLUE_JACKET, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(496), 0, PIXELS(72), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_BLACK_HAIR, 0x0, MOVEMENT_TYPE_WANDER,
        PIXELS(368), 0, PIXELS(128), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_MAN_IN_BLUE, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(760), 0, PIXELS(184), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_WOMAN_WITH_CANE, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(680), 0, PIXELS(80), FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_BALD_MAN_IN_RED, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(472), 0, PIXELS(600), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_IN_GREEN, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(448), 0, PIXELS(600), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_WITH_PONYTAIL, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(552), 0, PIXELS(600), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_MAN_WITH_DARK_HAIR, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(232), 0, PIXELS(448), FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_IN_PINK_APRON, 0x0, MOVEMENT_TYPE_WANDER,
        PIXELS(568), 0, PIXELS(416), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_COOK, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(280), 0, PIXELS(408), FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gLacquerRoomsB_SchoolRoomObjectEventTemplates[] = {
    { SPRITE_FEH, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), MOVEMENT_TYPE_STAND,
        PIXELS(224), 0, PIXELS(656), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_FEIZHI, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), MOVEMENT_TYPE_STAND,
        PIXELS(200), 0, PIXELS(704), FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_HSU, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), MOVEMENT_TYPE_STAND,
        PIXELS(224), 0, PIXELS(704), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

struct MapEvent gLacquerRoomsB_Events[] = {
    { MAP_EVENT_EXIT, 1, CONDITION_ALWAYS, LACQUER_ROOMS_B_WEAPON_AND_ARMOR_SHOP_TO_XIAN },
    { MAP_EVENT_EXIT, 2, CONDITION_ALWAYS, LACQUER_ROOMS_B_HOUSE_NORTH_TO_XIAN },
    { MAP_EVENT_EXIT, 3, CONDITION_ALWAYS, LACQUER_ROOMS_B_TO_XIAN_EAST },
    { MAP_EVENT_EXIT, 4, CONDITION_ALWAYS, LACQUER_ROOMS_B_TO_XIAN_SOUTH },
    { MAP_EVENT_EXIT, 5, CONDITION_ALWAYS, LACQUER_ROOMS_B_INN_GUEST_ROOMS_TO_INN },
    { MAP_EVENT_EXIT, 6, CONDITION_ALWAYS, LACQUER_ROOMS_B_TO_XIAN_WEST },
    { MAP_EVENT_EXIT, 7, CONDITION_ALWAYS, LACQUER_ROOMS_B_INN_TO_INN_GUEST_ROOMS },
    { MAP_EVENT_TALK, LOCALID_MAN_ON_THE_MULBERRY_HARVEST, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsTheMulberryHarvestIsOverNow) },
    { MAP_EVENT_TALK, LOCALID_FARM_WORKER, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsFarmWorkIsVeryHardWarriors) },
    { MAP_EVENT_TALK, LOCALID_YOUNG_MAN_ON_SILK_ROAD, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsXianSilkIsSentOnSilk) },
    { MAP_EVENT_TALK, LOCALID_WOMAN_ON_TOLBI, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsSilkRoadEndsInTolbiIt) },
    { MAP_EVENT_TALK, LOCALID_OLD_MAN_ON_THE_ALTIN_ROAD, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsFollowSilkRoadWhenYouSee) },
    { MAP_EVENT_TALK, LOCALID_OLD_WOMAN_ON_THE_ALTIN_MINE, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsThereIsAMineInAltin) },
    { MAP_EVENT_TALK, LOCALID_XIAN_WEAPON_MERCHANT, FLAG_XIAN_HEARD_HSU_TRAPPED, EVENT_SCRIPT(LacquerRoomsB_TalkToWeaponMerchant) },
    { MAP_EVENT_TALK, LOCALID_XIAN_ARMOR_MERCHANT, FLAG_XIAN_HEARD_HSU_TRAPPED, EVENT_SCRIPT(LacquerRoomsB_TalkToArmorMerchant) },
    { MAP_EVENT_TALK, LOCALID_XIAN_ITEM_MERCHANT, FLAG_XIAN_HEARD_HSU_TRAPPED, EVENT_SCRIPT(LacquerRoomsB_TalkToItemMerchant) },
    { MAP_EVENT_TALK, LOCALID_XIAN_INNKEEPER, FLAG_XIAN_HEARD_HSU_TRAPPED, EVENT_SCRIPT(LacquerRoomsB_TalkToInnkeeper) },
    { MAP_EVENT_TALK, LOCALID_WOMAN_AWAITING_MERCHANTS, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsNormallyManyMerchantsComeToThe) },
    { MAP_EVENT_TALK, LOCALID_COOK, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsAllThePeopleWhoCameTo) },
    { MAP_EVENT_TALK, LOCALID_MAN_ON_THE_MULBERRY_HARVEST, CONDITION_ALWAYS, MSGNO(MsgXianWhereAreTheMerchantsNoOne) },
    { MAP_EVENT_TALK, LOCALID_FARM_WORKER, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsMerchantsBringUsCoinsTheyMust) },
    { MAP_EVENT_TALK, LOCALID_YOUNG_MAN_ON_SILK_ROAD, CONDITION_ALWAYS, EVENT_SCRIPT(LacquerRoomsB_AskAboutSilkRoad) },
    { MAP_EVENT_TALK, LOCALID_WOMAN_ON_TOLBI, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsNoneCanPassThroughLamakanThis) },
    { MAP_EVENT_TALK, LOCALID_OLD_MAN_ON_THE_ALTIN_ROAD, CONDITION_ALWAYS, EVENT_SCRIPT(LacquerRoomsB_AskAboutAltinMonsters) },
    { MAP_EVENT_TALK, LOCALID_OLD_WOMAN_ON_THE_ALTIN_MINE, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsTheMineInAltinFilledWith) },
    { MAP_EVENT_TALK, LOCALID_XIAN_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(LacquerRoomsB_TalkToWeaponMerchant) },
    { MAP_EVENT_TALK, LOCALID_XIAN_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(LacquerRoomsB_TalkToArmorMerchant) },
    { MAP_EVENT_TALK, LOCALID_XIAN_ITEM_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(LacquerRoomsB_TalkToItemMerchant) },
    { MAP_EVENT_TALK, LOCALID_XIAN_INNKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(LacquerRoomsB_TalkToInnkeeper) },
    { MAP_EVENT_TALK, LOCALID_WOMAN_AWAITING_MERCHANTS, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsWhenLamakanDesertBurnsItIs) },
    { MAP_EVENT_TALK, LOCALID_COOK, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsHotFoodIsGoodForCrossing) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_MAN_ON_THE_MULBERRY_HARVEST, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsSoonMasterHammetWillComeHe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_FARM_WORKER, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsFarmWorkIsHardOnThe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_YOUNG_MAN_ON_SILK_ROAD, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsSilkRoadIsFullOfSilk) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WOMAN_ON_TOLBI, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsMerchantsSayTolbiIsVeryBeautiful) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_OLD_MAN_ON_THE_ALTIN_ROAD, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsAltinHasManyMenAllScary) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_OLD_WOMAN_ON_THE_ALTIN_MINE, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsMiningIsVeryHardWorkIt) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_XIAN_WEAPON_MERCHANT, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsSomeSayFightingWithWeaponsIs) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_XIAN_ARMOR_MERCHANT, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsInWarriorBattlesYouFightWith) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_XIAN_ITEM_MERCHANT, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsXianIsAVillageOfCraftsmen) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_XIAN_INNKEEPER, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsWhenYouGoToANew) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WOMAN_AWAITING_MERCHANTS, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsWhenManyMerchantsComeMyBusiness) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_COOK, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsThereAreFewerTravelersSoFewer) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_MAN_ON_THE_MULBERRY_HARVEST, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsWhyDoesMasterHammetNotCome) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_FARM_WORKER, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsMerchantsSellSilkItIsLike) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_YOUNG_MAN_ON_SILK_ROAD, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsFeizhiSaidTheDesertBecameEvil) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WOMAN_ON_TOLBI, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsTheDesertWasVeryHotNow) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_OLD_MAN_ON_THE_ALTIN_ROAD, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsTheVillageIsFloodedBecauseOf) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_OLD_WOMAN_ON_THE_ALTIN_MINE, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsWaterMonstersHowTerribleForAltin) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_XIAN_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsHsuWasPummeledByHugeBoulders) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_XIAN_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsWeShouldSellArmorToKung) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_XIAN_ITEM_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsYouLoseStrengthInHeatUse) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_XIAN_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsCaravansCannotBringMerchantsHereWhen) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WOMAN_AWAITING_MERCHANTS, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsTheMerchantsDoNotComeBecause) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_COOK, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsEatWarriorIWillAddMany) },
    { BG_EVENT(BG_EVENT_BARREL), 100, FLAG_LACQUER_ROOMS_B_ELIXIR_BARREL, FIND_ITEM(ITEM_ELIXIR) },
    { BG_EVENT(BG_EVENT_JAR), 101, FLAG_LACQUER_ROOMS_B_SLEEP_BOMB_JAR, FIND_ITEM(ITEM_SLEEP_BOMB) },
    { BG_EVENT(BG_EVENT_OVEN), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgXianInteriorsTheyreMakingStirFryTheCabbage)) },
    { BG_EVENT(BG_EVENT_OVEN), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgXianInteriorsItsTheXianGourmetTheBest)) },
    { BG_EVENT(BG_EVENT_OVEN), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgXianInteriorsTheyreSteamedDumplingsTheyreGreatPiping)) },
    { BG_EVENT(BG_EVENT_OVEN), 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgXianInteriorsIsaacAndHisFriendsAreShocked)) },
    { BG_EVENT(BG_EVENT_OVEN), 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgXianInteriorsHesMakingHotBeanCurdTastes)) },
    { BG_EVENT(BG_EVENT_SHELF) | MAP_EVENT_FLAG_FACING | FACING_NORTH, 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgXianInteriorsThereAreManyChopsticksButNot)) },
    { BG_EVENT(BG_EVENT_SHELF), 206, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgXianInteriorsThereAreMedicinalHerbsTheyHave)) },
    { BG_EVENT(BG_EVENT_SHELF), 207, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgXianInteriorsTheresAKimonoItLooksLike)) },
    { BG_EVENT(BG_EVENT_SHELF), 208, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgXianInteriorsItsJustASalesReportIt)) },
    { MAP_EVENTS_END },
};

struct MapEvent gLacquerRoomsB_SchoolRoomEvents[] = {
    { MAP_EVENT_EXIT, 8, CONDITION_ALWAYS, LACQUER_ROOMS_B_TO_LACQUER_ROOMS_A_FEHS_SCHOOL },
    { MAP_EVENT_TALK, LOCALID_FEH, FLAG_HSU_RESCUED_WITHOUT_PARTY, MSGNO(MsgXianInteriorsFeizhisPredictionCameTrueNowI) },
    { MAP_EVENT_TALK, LOCALID_FEIZHI, FLAG_HSU_RESCUED_WITHOUT_PARTY, EVENT_SCRIPT(LacquerRoomsB_FeizhiThanksParty) },
    { MAP_EVENT_TALK, LOCALID_HSU, FLAG_HSU_RESCUED_WITHOUT_PARTY, MSGNO(MsgXianInteriorsIWasInjuredByRocksOn) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_FEH, FLAG_HSU_RESCUED_WITHOUT_PARTY, MSGNO(MsgXianInteriorsFeizhisAbilityToSenseTroubleIs) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_FEIZHI, FLAG_HSU_RESCUED_WITHOUT_PARTY, MSGNO(MsgXianInteriorsMasterHamaHasGreatPowerYou) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_HSU, FLAG_HSU_RESCUED_WITHOUT_PARTY, MSGNO(MsgXianInteriorsIKnewHelpWouldComeWhen) },
    { MAP_EVENT_TALK, LOCALID_FEH, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsFeizhisPredictionCameTrueNowIBelieve) },
    { MAP_EVENT_TALK, LOCALID_FEIZHI, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsHsuWasTrappedUnderTheRocks) },
    { MAP_EVENT_TALK, LOCALID_HSU, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsIWasInjuredByBouldersOn) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_FEH, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsFeizhisAbilityToForeseeIsReal) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_FEIZHI, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsMasterHamaHasGreatPowerSome) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_HSU, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsIKnewHelpWouldComeWhenI) },
    { MAP_EVENTS_END },
};
// clang-format on
