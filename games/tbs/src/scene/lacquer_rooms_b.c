/// @file
/// The insides of Xian (SceneId_LacquerRoomsB): its houses, its inn and its
/// shop, red lacquered rooms laid side by side on one rooms map, and the small
/// bedroom behind Master Feh's school.
///
/// Each of Xian's doors opens into a room of its own here. Three are villagers'
/// houses, two to a house. The inn's front room has the innkeeper and a cook,
/// and an inner doorway leads to the room behind it with its beds. In the shop
/// the weapon, armor and item merchants stand side by side behind one long
/// white counter. Every villager has a line, and a thought for Mind Read, for
/// before and after the village hears that Hsu is trapped
/// (FLAG_XIAN_HEARD_HSU_TRAPPED); two of them ask a question with an answer
/// for yes and one for no. The merchants and the innkeeper serve the party
/// only when spoken to across the counter, and otherwise just talk.
///
/// The bedroom is reached only through a door of Master Feh's school
/// (fuchin_temple.c) and has its own actors and triggers. Once Xian has heard
/// of Hsu, he lies there in bed with Feizhi at his side and Master Feh by the
/// wall. If the party lifted the boulder off him at the Alpine Crossing,
/// Feizhi thanks it and Master Hama; if he was freed while the party was away
/// (FLAG_HSU_RESCUED_WITHOUT_PARTY), she thanks Hama alone, and the three of
/// them think otherwise under Mind Read. Coming in from the school clears
/// FLAG_SHOW_AREA_NAME.

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "types.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/event.h"
#include "field/field.h"
#include "field/field_effect.h"
#include "field/scene_tables.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "scene/field_effect_spawn.h"
#include "sound/sound.h"
#include "system/main_routine_work.h"
#include "system/system.h"
#include "text/msg_ids.h"

extern struct SceneEntrance gLacquerRoomsB_Entrances[];
extern u32 gLacquerRoomsB_Exits[];
extern struct ActorPlacement gLacquerRoomsB_ActorPlacements[];
extern struct ActorPlacement gLacquerRoomsB_SchoolRoomActorPlacements[];
extern struct Trigger gLacquerRoomsB_Triggers[];
extern struct Trigger gLacquerRoomsB_SchoolRoomTriggers[];

/// The actors of the houses, shops and inn, numbered by the rows of
/// gLacquerRoomsB_ActorPlacements. Each villager is named by what they
/// talk of before Xian hears of Hsu.
enum LacquerRoomsBActorId {
  /// A man with a topknot, at the table of his house.
  ACTOR_MAN_ON_THE_MULBERRY_HARVEST = ACTOR_FIRST_PLACED,
  /// A woman in a green apron, in the same house.
  ACTOR_FARM_WORKER,
  /// A young man in a blue jacket, at the back of another house.
  ACTOR_YOUNG_MAN_ON_SILK_ROAD,
  /// A young woman with black hair who walks about the same house.
  ACTOR_WOMAN_ON_TOLBI,
  /// An old man in blue, by the door of the third house.
  ACTOR_OLD_MAN_ON_THE_ALTIN_ROAD,
  /// An old woman with a cane, in the same house.
  ACTOR_OLD_WOMAN_ON_THE_ALTIN_MINE,
  ACTOR_XIAN_WEAPON_MERCHANT,
  ACTOR_XIAN_ARMOR_MERCHANT,
  ACTOR_XIAN_ITEM_MERCHANT,
  ACTOR_XIAN_INNKEEPER,
  /// A woman in a pink apron who walks about the inn's back room.
  ACTOR_WOMAN_AWAITING_MERCHANTS,
  /// A man in a cook's cap, in the inn's front room with the innkeeper.
  ACTOR_COOK
};

/// Xian's shops and inn, by their numbers in the shop and inn tables.
enum {
  XIAN_WEAPON_STOCK = 16,
  XIAN_ARMOR_STOCK = 17,
  XIAN_ITEM_STOCK = 18,
  XIAN_INN = 5
};

/// The three in the bedroom behind Master Feh's school once Xian has heard of
/// Hsu: Master Feh, who says Feizhi's prediction came true, Feizhi, and Hsu,
/// hurt by the rocks on his way from Lama Temple.
enum { ACTOR_FEH = ACTOR_FIRST_PLACED, ACTOR_FEIZHI, ACTOR_HSU };

/// Hsu on his back with his arms at his sides, as he lies in the bed in the
/// bedroom behind the school (hsu.png's frame 36).
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

/// The way the leader faces, read before the event turns anyone.
static inline u32 LacquerRoomsB_GetLeaderFacing(void) {
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);
  return leader->facing;
}

/// Talking to the bald weapon merchant in Xian's shop from in front of the
/// counter opens the weapon shop. From anywhere else he talks of martial arts,
/// or once the village has heard of Hsu, of the lesson his injury taught.
static void LacquerRoomsB_TalkToWeaponMerchant(void) {
  u32 leaderFacing = LacquerRoomsB_GetLeaderFacing();

  Event_Start();
  // Across the counter: facing north, between northwest and northeast.
  if (leaderFacing >= FACING_NORTHWEST + 1 &&
      leaderFacing <= FACING_NORTHEAST - 1) {
    Shop_Run(XIAN_WEAPON_STOCK, ACTOR_XIAN_WEAPON_MERCHANT);
  } else {
    if (GameFlag_Get(FLAG_XIAN_HEARD_HSU_TRAPPED) == 0) {
      Event_SetMessageId(MsgXianXianHasMartialArtsBut);
      // "Xian has martial arts, but kung fu alone is not enough...."
    } else {
      Event_SetMessageId(MsgXianHsusInjuryTaughtGoodLesson);
      // "Hsu's injury taught a good lesson. Even kung fu is no good..."
    }
    Event_ShowMessage(ACTOR_XIAN_WEAPON_MERCHANT, 0);
  }
  Event_End();
}

/// Talking to the young armor merchant in Xian's shop from in front of the
/// counter opens the armor shop. From anywhere else he talks of armor and kung
/// fu.
static void LacquerRoomsB_TalkToArmorMerchant(void) {
  u32 leaderFacing = LacquerRoomsB_GetLeaderFacing();

  Event_Start();
  // Across the counter: facing north, between northwest and northeast.
  if (leaderFacing >= FACING_NORTHWEST + 1 &&
      leaderFacing <= FACING_NORTHEAST - 1) {
    Shop_Run(XIAN_ARMOR_STOCK, ACTOR_XIAN_ARMOR_MERCHANT);
  } else {
    if (GameFlag_Get(FLAG_XIAN_HEARD_HSU_TRAPPED) == 0) {
      Event_SetMessageId(MsgXianMartialArtistsCantUseHeavy);
      // "Martial artists can't use heavy armor. If they do, they can't..."
    } else {
      Event_SetMessageId(MsgXianKungFuMakesQuickDoes);
      // "Kung fu makes you quick, does it not? Even so, Hsu could not..."
    }
    Event_ShowMessage(ACTOR_XIAN_ARMOR_MERCHANT, 0);
  }
  Event_End();
}

/// Talking to the item merchant, the woman at the end of Xian's shop counter,
/// from in front of it opens the item shop. From anywhere else she talks of
/// Xian's armor, or once the village has heard of Hsu, asks whether the party
/// means to cross the desert.
static void LacquerRoomsB_TalkToItemMerchant(void) {
  u32 leaderFacing = LacquerRoomsB_GetLeaderFacing();

  Event_Start();
  // Across the counter: facing north, between northwest and northeast.
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

/// Talking to Xian's innkeeper across his counter, from the west, lets the
/// party stay at the inn. From anywhere else he talks of towns, or of how few
/// guests come these days.
static void LacquerRoomsB_TalkToInnkeeper(void) {
  u32 leaderFacing = LacquerRoomsB_GetLeaderFacing();

  Event_Start();
  // Across the counter: facing east, between northeast and southeast.
  if (leaderFacing < FACING_SOUTHEAST || leaderFacing > FACING_NORTHEAST) {
    Inn_CheckIn(XIAN_INN, ACTOR_XIAN_INNKEEPER);
  } else {
    if (GameFlag_Get(FLAG_XIAN_HEARD_HSU_TRAPPED) == 0) {
      Event_SetMessageId(MsgXianWarriorsShouldLearnAboutTowns);
      // "Warriors should learn about the towns they visit. Just..."
    } else {
      Event_SetMessageId(MsgXianWeHaveFewCustomersThese);
      // "We have few customers these days. Customers can't come if..."
    }
    Event_ShowMessage(ACTOR_XIAN_INNKEEPER, 0);
  }
  Event_End();
}

/// Once Xian has heard of Hsu, a young man at home asks whether something is
/// wrong on Silk Road and in Lamakan Desert.
static void LacquerRoomsB_AskAboutSilkRoad(void) {
  Event_Start();
  Event_SetMessageId(MsgXianSomethingWrongOnSilkRoad);
  // "Is something wrong on Silk Road? In Lamakan Desert, too?"
  // Yes: "That is terrible. No one can use Silk Road!"
  // No: "The desert is fine? That is good to hear."
  Event_AskYesNo(ACTOR_YOUNG_MAN_ON_SILK_ROAD, 0);
  Event_End();
}

/// Once Xian has heard of Hsu, an old man at home asks whether the monsters in
/// Altin really spat water, as Feizhi says.
static void LacquerRoomsB_AskAboutAltinMonsters(void) {
  Event_Start();
  Event_SetMessageId(MsgXianDidMonstersInAltinSpit);
  // "Did the monsters in Altin spit water?"
  // Yes: "The town was flooded by monsters? That takes much water. How..."
  // No: "It is not true? But why would Feizhi lie?"
  Event_AskYesNo(ACTOR_OLD_MAN_ON_THE_ALTIN_ROAD, 0);
  Event_End();
}

/// In the bedroom behind Master Feh's school, Feizhi welcomes the party, turns
/// to look at Hsu in his bed and back, and thanks it and Master Hama for
/// saving him.
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

/// Each room opens with the window transition. The inn and the shop also get
/// unseen still actors of SPRITE_CHEST's sprite sheet near their counters
/// (Actor_CreateConfigured), and the bedroom lays Hsu in his bed.
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
