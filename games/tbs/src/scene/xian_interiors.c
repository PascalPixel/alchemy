/// @file
/// Xian's houses, shops and inn (scene 73, a row with no SceneId_ name; the
/// area names call it Xian), and the room behind Master Feh's school.
///
/// Each of Xian's doors leads to a room here, and each room leads back out to
/// its door. XIAN_INTERIORS_HOUSE, XIAN_INTERIORS_HSUS_HOUSE and
/// XIAN_INTERIORS_FROM_XIAN_EAST are villagers' houses;
/// XIAN_INTERIORS_INN_SOUTH opens into the inn, where
/// XIAN_INTERIORS_INN_TO_INN_GUEST_ROOMS leads on into its guest rooms and
/// XIAN_INTERIORS_INN_GUEST_ROOMS_TO_INN back; and
/// XIAN_INTERIORS_WEAPON_AND_ARMOR_SHOP opens into the weapon, armor and item
/// shops. As in the village, every line has a row for each side of
/// FLAG_XIAN_HEARD_HSU_TRAPPED: before, the villagers talk of silk, Silk Road
/// to Tolbi and the mine at Altin, and wonder why the merchants have not come;
/// after, of Hsu's injury, of Silk Road and Lamakan Desert closed, and of
/// Altin's water monsters as Feizhi told of them, two villagers asking the
/// party what it knows (a yes and a no get different replies). The
/// shopkeepers and the innkeeper serve the party only when spoken to across
/// the counter, and otherwise talk. Searching the ovens and shelves shows what
/// the cooks are making and what lies about, such as a kimono for the inn's
/// guests; a barrel holds an Elixir and a jar a Sleep Bomb.
///
/// The room behind Master Feh's school (XIAN_INTERIORS_FROM_FUCHIN_TEMPLE_2) is
/// reached only from the school (FUCHIN_TEMPLE_2_TO_HOUSE, in fuchin_temple.c)
/// and has its own tables. Once Xian has heard of Hsu, Master Feh, Feizhi and
/// Hsu are there, Hsu in his pose (animation 6). Master Feh says Feizhi's
/// prediction came true, Hsu says he was injured on his way from Lama Temple,
/// and Mind Read hears that he knew help would come, however he was freed. If
/// the party helped free him (FLAG_HSU_RESCUED_WITHOUT_PARTY clear), Feizhi
/// thanks it and Master Hama, and her thoughts praise the party's power as well
/// as Hama's; otherwise she thanks Master Hama alone, and her thoughts are glad
/// some of Hama's power passed to her. Arriving here clears
/// FLAG_SHOW_LOCATION_NAME.

#include "game_state.h"
#include "unit/scene/xian_interiors.h"
#include "unit/scene/common/effect_spawn.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
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
#include "field_obj.h"
#include "flags.h"

/// The actors of the houses, shops and inn, numbered by the rows of
/// gXianInteriors_Placements. Each villager is named by what they talk of
/// before Xian hears of Hsu.
enum XianInteriorsActor {
  /// In the room at XIAN_INTERIORS_HOUSE. "The mulberry harvest is over. Now
  /// there is much food for the caterpillars!"
  ACTOR_VILLAGER_MULBERRY = ACTOR_FIRST_PLACED,
  /// In the same room. "Farm work is very hard. Warriors have no idea how hard
  /// it is."
  ACTOR_FARMER,
  /// In the room at XIAN_INTERIORS_HSUS_HOUSE. "Xian silk is sent on Silk Road
  /// to Tolbi."
  ACTOR_VILLAGER_SILK_ROAD,
  /// Walks about the same room. "Silk Road ends in Tolbi. It is a big, big
  /// town."
  ACTOR_VILLAGER_TOLBI,
  /// In the room at XIAN_INTERIORS_FROM_XIAN_EAST. "Follow Silk Road. When you
  /// see the mountain fork, go north."
  ACTOR_VILLAGER_ALTIN_ROAD,
  /// In the same room. "There is a mine in Altin. There is lots of steel and
  /// lots of gems."
  ACTOR_VILLAGER_ALTIN_MINE,
  ACTOR_XIAN_WEAPON_MERCHANT,
  ACTOR_XIAN_ARMOR_MERCHANT,
  ACTOR_XIAN_ITEM_MERCHANT,
  ACTOR_XIAN_INNKEEPER,
  /// Walks about the inn's second room. "Normally, many merchants come to the
  /// village, but they have not come yet."
  ACTOR_VILLAGER_AWAITING_MERCHANTS,
  /// In the inn's room at XIAN_INTERIORS_INN_SOUTH, with the innkeeper. "All
  /// the people who came to eat are from Xian."; once Xian has heard of Hsu,
  /// Mind Read hears "Eat, warrior. I will add many hot peppers and savory
  /// spices."
  ACTOR_COOK
};

enum {
  XIAN_WEAPON_STOCK = 16,
  XIAN_ARMOR_STOCK = 17,
  XIAN_ITEM_STOCK = 18,
  XIAN_INN = 5
};

/// The actors placed in the room behind Master Feh's school
/// (XIAN_INTERIORS_FROM_FUCHIN_TEMPLE_2), where they stand once Xian has heard
/// of Hsu, as its dialogue shows them: Master Feh says Feizhi's prediction came
/// true, Feizhi welcomes the party, and Hsu was injured on his way from Lama
/// Temple.
enum { ACTOR_FEH = ACTOR_FIRST_PLACED, ACTOR_FEIZHI, ACTOR_HSU };

#include "common/effect_create.h"
#include "common/effect_spawn.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"

const struct SceneEntrance *XianInteriors_GetEntrances(void) {
  return gXianInteriors_Entrances;
}

const struct SceneRegion *XianInteriors_GetRegions(void) { return NULL; }

const u32 *XianInteriors_GetExits(void) { return gXianInteriors_Exits; }

/// The room behind the school places only Master Feh, Feizhi and Hsu. Every
/// other room shares one table, whose condition 0 SceneTable_Prepare replaces
/// with whether each actor stands inside the view bounds of the room entered.
const struct ScenePlacement *XianInteriors_GetPlacements(void) {
  if (gGameState.entrance == XIAN_INTERIORS_FROM_FUCHIN_TEMPLE_2) {
    return gXianInteriors_SchoolRoomPlacements;
  }
  SceneTable_Prepare(gXianInteriors_Placements);
  return gXianInteriors_Placements;
}

/// Reads the leader's facing before a conversation changes actor state.
static inline u32 XianInteriors_GetLeaderFacing(void) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  return leader->facing;
}

/// Opens the weapon shop from its counter side, or discusses martial arts.
static void XianInteriors_TalkToWeaponMerchant(void) {
  u32 leaderFacing = XianInteriors_GetLeaderFacing();

  Engine_EventBegin();
  if (leaderFacing >= FACING_NORTHWEST + 1 &&
      leaderFacing <= FACING_NORTHEAST - 1) {
    Engine_ShopOpen(XIAN_WEAPON_STOCK, ACTOR_XIAN_WEAPON_MERCHANT);
  } else {
    if (Engine_GameFlagIsSet(FLAG_XIAN_HEARD_HSU_TRAPPED) == 0) {
      Engine_EventSetMessage(MsgXianXianHasMartialArtsBut);
    } else {
      Engine_EventSetMessage(MsgXianHsusInjuryTaughtGoodLesson);
    }
    Engine_EventShowMessage(ACTOR_XIAN_WEAPON_MERCHANT, 0);
  }
  Engine_EventEnd();
}

/// Opens the armor shop from its counter side, or discusses armor and kung fu.
static void XianInteriors_TalkToArmorMerchant(void) {
  u32 leaderFacing = XianInteriors_GetLeaderFacing();

  Engine_EventBegin();
  if (leaderFacing >= FACING_NORTHWEST + 1 &&
      leaderFacing <= FACING_NORTHEAST - 1) {
    Engine_ShopOpen(XIAN_ARMOR_STOCK, ACTOR_XIAN_ARMOR_MERCHANT);
  } else {
    if (Engine_GameFlagIsSet(FLAG_XIAN_HEARD_HSU_TRAPPED) == 0) {
      Engine_EventSetMessage(MsgXianMartialArtistsCantUseHeavy);
    } else {
      Engine_EventSetMessage(MsgXianKungFuMakesQuickDoes);
    }
    Engine_EventShowMessage(ACTOR_XIAN_ARMOR_MERCHANT, 0);
  }
  Engine_EventEnd();
}

/// Opens the item shop or asks whether the party will cross the desert.
static void XianInteriors_TalkToItemMerchant(void) {

  u32 leaderFacing = XianInteriors_GetLeaderFacing();

  Engine_EventBegin();
  if (leaderFacing >= FACING_NORTHWEST + 1 &&
      leaderFacing <= FACING_NORTHEAST - 1) {
    Engine_ShopOpen(XIAN_ITEM_STOCK, ACTOR_XIAN_ITEM_MERCHANT);
  } else if (Engine_GameFlagIsSet(FLAG_XIAN_HEARD_HSU_TRAPPED) == 0) {
    Engine_EventSetMessage(MsgXianXiansSpecialArmorNotSuited);
    Engine_EventShowMessage(ACTOR_XIAN_ITEM_MERCHANT, 0);
  } else {
    Engine_EventSetMessage(MsgXianDoPlanCrossDesertWarrior);
    Engine_EventAskYesNo(ACTOR_XIAN_ITEM_MERCHANT, 0);
  }
  Engine_EventEnd();
}

/// Offers a room from the counter side, or discusses travel through Xian.
static void XianInteriors_TalkToInnkeeper(void) {

  u32 leaderFacing = XianInteriors_GetLeaderFacing();

  Engine_EventBegin();
  if (leaderFacing < FACING_SOUTHEAST || leaderFacing > FACING_NORTHEAST) {
    Engine_InnOpen(XIAN_INN, ACTOR_XIAN_INNKEEPER);
  } else {
    if (Engine_GameFlagIsSet(FLAG_XIAN_HEARD_HSU_TRAPPED) == 0) {
      Engine_EventSetMessage(MsgXianWarriorsShouldLearnAboutTowns);
    } else {
      Engine_EventSetMessage(MsgXianWeHaveFewCustomersThese);
    }
    Engine_EventShowMessage(ACTOR_XIAN_INNKEEPER, 0);
  }
  Engine_EventEnd();
}

/// After the news of Hsu, ACTOR_VILLAGER_SILK_ROAD asks whether something is
/// wrong on Silk Road and in Lamakan Desert. A yes: no one can use Silk Road; a
/// no: it is good to hear the desert is fine.
static void XianInteriors_AskAboutSilkRoad(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgXianSomethingWrongOnSilkRoad);
  Engine_EventAskYesNo(ACTOR_VILLAGER_SILK_ROAD, 0);
  Engine_EventEnd();
}

/// After the news of Hsu, ACTOR_VILLAGER_ALTIN_ROAD asks whether the monsters
/// in Altin spat water. A yes: how strange; a no: but why would Feizhi lie?
static void XianInteriors_AskAboutAltinMonsters(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgXianDidMonstersInAltinSpit);
  Engine_EventAskYesNo(ACTOR_VILLAGER_ALTIN_ROAD, 0);
  Engine_EventEnd();
}

/// In the room behind Master Feh's school, when the party helped free Hsu
/// (FLAG_HSU_RESCUED_WITHOUT_PARTY clear), Feizhi welcomes the party, looks at
/// Hsu, then thanks the party and Master Hama. "Oh, warrior! Welcome!"
static void XianInteriors_FeizhiThanksParty(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgXianWarriorWelcome);
  Engine_EventShowMessageAndWait(ACTOR_FEIZHI, 0, 20);
  Engine_ActorFaceActor(ACTOR_FEIZHI, ACTOR_HSU, 0);
  Engine_EventWait(60);
  Engine_ActorFaceActor(ACTOR_FEIZHI, ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(20);
  Engine_EventShowMessage(ACTOR_FEIZHI, 0);
  Engine_EventEnd();
}

/// The room behind the school has its own events.
const struct SceneEvent *XianInteriors_GetEvents(void) {
  if (gGameState.entrance == XIAN_INTERIORS_FROM_FUCHIN_TEMPLE_2) {
    return gXianInteriors_SchoolRoomEvents;
  }
  return gXianInteriors_Events;
}

/// Opens the room through a window transition and creates its configured
/// objects (kind 20): one in the inn (XIAN_INTERIORS_INN_SOUTH and
/// XIAN_INTERIORS_INN_FROM_INN_GUEST_ROOMS), two in the shops
/// (XIAN_INTERIORS_WEAPON_AND_ARMOR_SHOP). In the room behind the school, it
/// clears FLAG_SHOW_LOCATION_NAME and Hsu takes his pose.
s32 XianInteriors_Initialize(void) {
  s16 entrance;

  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  entrance = gGameState.entrance;
  // Restore the ambient effects shared by connected interior entrances.
  if (entrance == XIAN_INTERIORS_INN_SOUTH ||
      entrance == XIAN_INTERIORS_INN_FROM_INN_GUEST_ROOMS) {
    OverlayObject_CreateConfiguredObject(PIXELS(248), 0, PIXELS(417), 20);
  } else if (entrance == XIAN_INTERIORS_WEAPON_AND_ARMOR_SHOP) {
    OverlayObject_CreateConfiguredObject(PIXELS(460), 0, PIXELS(568), 20);
    OverlayObject_CreateConfiguredObject(PIXELS(484), 0, PIXELS(568), 20);
  } else if (entrance == XIAN_INTERIORS_FROM_FUCHIN_TEMPLE_2) {
    Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
    Engine_ActorSetAnimation(ACTOR_HSU, 6);
  }
  return 0;
}

s32 gEffectScript0[] = {
    SC_WAIT(1),      SC_LOOP(22, SC_TOP), SC_WAIT(1),
    SC_POS(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gEffectScript1[] = {
    SC_WAIT(1),      SC_LOOP(44, SC_TOP), SC_WAIT(1),
    SC_POS(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gEffectScript2[] = {
    SC_WAIT(1),      SC_LOOP(126, SC_TOP), SC_WAIT(1),
    SC_POS(0, 0, 0), SC_WAIT(1),           SC_FREE,
};

const s32 *gEffectScripts[] = {
    gEffectScript0,
    gEffectScript1,
    gEffectScript2,
};

struct SceneEntrance gXianInteriors_Entrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {XIAN_INTERIORS_HOUSE, CONDITION_ALWAYS, 128, 0, 184, FACING_NORTH, 0, 24,
     8, 248, 208, 0},
    {XIAN_INTERIORS_HSUS_HOUSE, CONDITION_ALWAYS, 448, 0, 200, FACING_NORTH, 0,
     288, 8, 592, 224, 0},
    {XIAN_INTERIORS_FROM_XIAN_EAST, CONDITION_ALWAYS, 736, 0, 200, FACING_NORTH,
     0, 608, 8, 848, 224, 0},
    {XIAN_INTERIORS_INN_SOUTH, CONDITION_ALWAYS, 160, 0, 472, FACING_NORTH, 0,
     48, 288, 336, 496, 0},
    {5, CONDITION_ALWAYS, 472, 0, 360, FACING_EAST, 0, 384, 288, 672, 496, 0},
    {XIAN_INTERIORS_WEAPON_AND_ARMOR_SHOP, CONDITION_ALWAYS, 512, 0, 696,
     FACING_NORTH, 0, 352, 496, 656, 720, 0},
    {XIAN_INTERIORS_INN_FROM_INN_GUEST_ROOMS, CONDITION_ALWAYS, 120, 0, 360,
     FACING_EAST, 0, 48, 288, 336, 496, 0},
    {XIAN_INTERIORS_FROM_FUCHIN_TEMPLE_2, CONDITION_ALWAYS, 120, 0, 696,
     FACING_NORTH, 0, 64, 544, 304, 744, 0},
    {SCENE_TABLE_END},
};

u32 gXianInteriors_Exits[] = {
    SCENE_EXITS(XianInteriors),
    SCENE_EXIT(XIAN_INTERIORS_WEAPON_AND_ARMOR_SHOP_TO_XIAN, Xian,
               XIAN_FROM_WEAPON_AND_ARMOR_SHOP),
    SCENE_EXIT(XIAN_INTERIORS_HSUS_HOUSE_TO_XIAN, Xian, XIAN_FROM_HSUS_HOUSE),
    SCENE_EXIT(XIAN_INTERIORS_TO_XIAN_EAST, Xian,
               XIAN_FROM_XIAN_INTERIORS_EAST),
    SCENE_EXIT(XIAN_INTERIORS_TO_XIAN_SOUTH, Xian,
               XIAN_FROM_XIAN_INTERIORS_SOUTH),
    SCENE_EXIT(XIAN_INTERIORS_INN_GUEST_ROOMS_TO_INN, XianInteriors,
               XIAN_INTERIORS_INN_FROM_INN_GUEST_ROOMS),
    SCENE_EXIT(XIAN_INTERIORS_TO_XIAN_WEST, Xian,
               XIAN_FROM_XIAN_INTERIORS_WEST),
    SCENE_EXIT(XIAN_INTERIORS_INN_TO_INN_GUEST_ROOMS, XianInteriors,
               XIAN_INTERIORS_INN_GUEST_ROOMS_FROM_INN),
    SCENE_EXIT(XIAN_INTERIORS_TO_FUCHIN_TEMPLE_2, FuchinTemple2,
               FUCHIN_TEMPLE_2_FROM_HOUSE),
    SCENE_EXITS_END,
};

// clang-format off
struct ScenePlacement gXianInteriors_Placements[] = {
    { 0xa0, 0x0, ACTOR_STAND,
        PIXELS(144), 0, PIXELS(152), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xa2, 0x0, ACTOR_STAND,
        PIXELS(136), 0, PIXELS(80), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x9c, 0x0, ACTOR_STAND,
        PIXELS(496), 0, PIXELS(72), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x9f, 0x0, ACTOR_WANDER,
        PIXELS(368), 0, PIXELS(128), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xa4, 0x0, ACTOR_STAND,
        PIXELS(760), 0, PIXELS(184), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0xa6, 0x0, ACTOR_STAND,
        PIXELS(680), 0, PIXELS(80), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x7c, 0x0, ACTOR_STAND,
        PIXELS(472), 0, PIXELS(600), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x7d, 0x0, ACTOR_STAND,
        PIXELS(448), 0, PIXELS(600), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x76, 0x0, ACTOR_STAND,
        PIXELS(552), 0, PIXELS(600), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x77, 0x0, ACTOR_STAND,
        PIXELS(232), 0, PIXELS(448), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x80, 0x0, ACTOR_WANDER,
        PIXELS(568), 0, PIXELS(416), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x7b, 0x0, ACTOR_STAND,
        PIXELS(280), 0, PIXELS(408), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gXianInteriors_SchoolRoomPlacements[] = {
    { 0x28, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), ACTOR_STAND,
        PIXELS(224), 0, PIXELS(656), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x27, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), ACTOR_STAND,
        PIXELS(200), 0, PIXELS(704), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x29, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), ACTOR_STAND,
        PIXELS(224), 0, PIXELS(704), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gXianInteriors_Events[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
    { EVENT_EXIT, 6, CONDITION_ALWAYS, 6 },
    { EVENT_EXIT, 7, CONDITION_ALWAYS, 7 },
    { EVENT_TALK, ACTOR_VILLAGER_MULBERRY, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsTheMulberryHarvestIsOverNow) },
    { EVENT_TALK, ACTOR_FARMER, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsFarmWorkIsVeryHardWarriors) },
    { EVENT_TALK, ACTOR_VILLAGER_SILK_ROAD, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsXianSilkIsSentOnSilk) },
    { EVENT_TALK, ACTOR_VILLAGER_TOLBI, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsSilkRoadEndsInTolbiIt) },
    { EVENT_TALK, ACTOR_VILLAGER_ALTIN_ROAD, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsFollowSilkRoadWhenYouSee) },
    { EVENT_TALK, ACTOR_VILLAGER_ALTIN_MINE, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsThereIsAMineInAltin) },
    { EVENT_TALK, ACTOR_XIAN_WEAPON_MERCHANT, FLAG_XIAN_HEARD_HSU_TRAPPED, EVENT_SCRIPT(XianInteriors_TalkToWeaponMerchant) },
    { EVENT_TALK, ACTOR_XIAN_ARMOR_MERCHANT, FLAG_XIAN_HEARD_HSU_TRAPPED, EVENT_SCRIPT(XianInteriors_TalkToArmorMerchant) },
    { EVENT_TALK, ACTOR_XIAN_ITEM_MERCHANT, FLAG_XIAN_HEARD_HSU_TRAPPED, EVENT_SCRIPT(XianInteriors_TalkToItemMerchant) },
    { EVENT_TALK, ACTOR_XIAN_INNKEEPER, FLAG_XIAN_HEARD_HSU_TRAPPED, EVENT_SCRIPT(XianInteriors_TalkToInnkeeper) },
    { EVENT_TALK, ACTOR_VILLAGER_AWAITING_MERCHANTS, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsNormallyManyMerchantsComeToThe) },
    { EVENT_TALK, ACTOR_COOK, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsAllThePeopleWhoCameTo) },
    { EVENT_TALK, ACTOR_VILLAGER_MULBERRY, CONDITION_ALWAYS, MSGNO(MsgXianWhereAreTheMerchantsNoOne) },
    { EVENT_TALK, ACTOR_FARMER, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsMerchantsBringUsCoinsTheyMust) },
    { EVENT_TALK, ACTOR_VILLAGER_SILK_ROAD, CONDITION_ALWAYS, EVENT_SCRIPT(XianInteriors_AskAboutSilkRoad) },
    { EVENT_TALK, ACTOR_VILLAGER_TOLBI, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsNoneCanPassThroughLamakanThis) },
    { EVENT_TALK, ACTOR_VILLAGER_ALTIN_ROAD, CONDITION_ALWAYS, EVENT_SCRIPT(XianInteriors_AskAboutAltinMonsters) },
    { EVENT_TALK, ACTOR_VILLAGER_ALTIN_MINE, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsTheMineInAltinFilledWith) },
    { EVENT_TALK, ACTOR_XIAN_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(XianInteriors_TalkToWeaponMerchant) },
    { EVENT_TALK, ACTOR_XIAN_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(XianInteriors_TalkToArmorMerchant) },
    { EVENT_TALK, ACTOR_XIAN_ITEM_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(XianInteriors_TalkToItemMerchant) },
    { EVENT_TALK, ACTOR_XIAN_INNKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(XianInteriors_TalkToInnkeeper) },
    { EVENT_TALK, ACTOR_VILLAGER_AWAITING_MERCHANTS, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsWhenLamakanDesertBurnsItIs) },
    { EVENT_TALK, ACTOR_COOK, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsHotFoodIsGoodForCrossing) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_MULBERRY, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsSoonMasterHammetWillComeHe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FARMER, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsFarmWorkIsHardOnThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_SILK_ROAD, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsSilkRoadIsFullOfSilk) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_TOLBI, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsMerchantsSayTolbiIsVeryBeautiful) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ALTIN_ROAD, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsAltinHasManyMenAllScary) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ALTIN_MINE, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsMiningIsVeryHardWorkIt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_XIAN_WEAPON_MERCHANT, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsSomeSayFightingWithWeaponsIs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_XIAN_ARMOR_MERCHANT, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsInWarriorBattlesYouFightWith) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_XIAN_ITEM_MERCHANT, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsXianIsAVillageOfCraftsmen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_XIAN_INNKEEPER, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsWhenYouGoToANew) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_AWAITING_MERCHANTS, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsWhenManyMerchantsComeMyBusiness) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_COOK, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsThereAreFewerTravelersSoFewer) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_MULBERRY, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsWhyDoesMasterHammetNotCome) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FARMER, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsMerchantsSellSilkItIsLike) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_SILK_ROAD, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsFeizhiSaidTheDesertBecameEvil) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_TOLBI, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsTheDesertWasVeryHotNow) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ALTIN_ROAD, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsTheVillageIsFloodedBecauseOf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ALTIN_MINE, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsWaterMonstersHowTerribleForAltin) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_XIAN_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsHsuWasPummeledByHugeBoulders) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_XIAN_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsWeShouldSellArmorToKung) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_XIAN_ITEM_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsYouLoseStrengthInHeatUse) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_XIAN_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsCaravansCannotBringMerchantsHereWhen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_AWAITING_MERCHANTS, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsTheMerchantsDoNotComeBecause) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_COOK, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsEatWarriorIWillAddMany) },
    { SEARCH(SEARCH_BARREL), 100, FLAG_XIAN_INTERIORS_ELIXIR_BARREL, FIND_ITEM(ITEM_ELIXIR) },
    { SEARCH(SEARCH_JAR), 101, FLAG_XIAN_INTERIORS_SLEEP_BOMB_JAR, FIND_ITEM(ITEM_SLEEP_BOMB) },
    { SEARCH(SEARCH_OVEN), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgXianInteriorsTheyreMakingStirFryTheCabbage)) },
    { SEARCH(SEARCH_OVEN), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgXianInteriorsItsTheXianGourmetTheBest)) },
    { SEARCH(SEARCH_OVEN), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgXianInteriorsTheyreSteamedDumplingsTheyreGreatPiping)) },
    { SEARCH(SEARCH_OVEN), 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgXianInteriorsIsaacAndHisFriendsAreShocked)) },
    { SEARCH(SEARCH_OVEN), 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgXianInteriorsHesMakingHotBeanCurdTastes)) },
    { 0xc5b3, 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgXianInteriorsThereAreManyChopsticksButNot)) },
    { SEARCH(SEARCH_SHELF), 206, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgXianInteriorsThereAreMedicinalHerbsTheyHave)) },
    { SEARCH(SEARCH_SHELF), 207, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgXianInteriorsTheresAKimonoItLooksLike)) },
    { SEARCH(SEARCH_SHELF), 208, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgXianInteriorsItsJustASalesReportIt)) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gXianInteriors_SchoolRoomEvents[] = {
    { EVENT_EXIT, 8, CONDITION_ALWAYS, 8 },
    { EVENT_TALK, ACTOR_FEH, FLAG_HSU_RESCUED_WITHOUT_PARTY, MSGNO(MsgXianInteriorsFeizhisPredictionCameTrueNowI) },
    { EVENT_TALK, ACTOR_FEIZHI, FLAG_HSU_RESCUED_WITHOUT_PARTY, EVENT_SCRIPT(XianInteriors_FeizhiThanksParty) },
    { EVENT_TALK, ACTOR_HSU, FLAG_HSU_RESCUED_WITHOUT_PARTY, MSGNO(MsgXianInteriorsIWasInjuredByRocksOn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FEH, FLAG_HSU_RESCUED_WITHOUT_PARTY, MSGNO(MsgXianInteriorsFeizhisAbilityToSenseTroubleIs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FEIZHI, FLAG_HSU_RESCUED_WITHOUT_PARTY, MSGNO(MsgXianInteriorsMasterHamaHasGreatPowerYou) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HSU, FLAG_HSU_RESCUED_WITHOUT_PARTY, MSGNO(MsgXianInteriorsIKnewHelpWouldComeWhen) },
    { EVENT_TALK, ACTOR_FEH, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsFeizhisPredictionCameTrueNowIBelieve) },
    { EVENT_TALK, ACTOR_FEIZHI, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsHsuWasTrappedUnderTheRocks) },
    { EVENT_TALK, ACTOR_HSU, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsIWasInjuredByBouldersOn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FEH, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsFeizhisAbilityToForeseeIsReal) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FEIZHI, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsMasterHamaHasGreatPowerSome) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HSU, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsIKnewHelpWouldComeWhenI) },
    { SCENE_EVENTS_END },
};
// clang-format on
