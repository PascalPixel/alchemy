/// @file
/// Altin, the mining village north of the Alpine Crossing, in two scenes that
/// share this overlay and the area name Altin.
///
/// SceneId_Altin is the village outdoors. SceneId_CottageRoomsD is a sheet of
/// rooms drawn as cottages, all of them Altin's. They are its houses, shops and
/// inn, entered by the village's doors (COTTAGE_ROOMS_D_FROM_ALTIN_NORTHWEST to
/// COTTAGE_ROOMS_D_FROM_ALTIN_SOUTHWEST).
///
/// The getters pick each scene's tables by gMainRoutineWork.next, and both
/// share one exit table. A third, empty set of tables serves any other scene.
///
/// The village is flooded by the water beasts of its mine. The area names call
/// the mine's scenes Altin Peak (altin_peak.c). Two things in Altin's code
/// follow the party's work down there.
///
/// The first is the water. It drops in three stages with the three drains the
/// party opens by beating the mine's water beasts (FLAG_ALTIN_MINE_FIRST_DRAIN
/// to _THIRD_DRAIN). Scene_Initialize turns them into three scene temp flags,
/// the water dropped once, twice, or drained (the second and third drains
/// both). Altin_ApplyWaterLevel copies the layout area for that level over the
/// village, with its cell behaviors, and leaves out whoever would be standing
/// in the water.
///
/// The second is what people say. Every villager has two sets of lines and
/// thoughts, before and after FLAG_HYDROS_STATUES_DEFEATED, which says whether
/// the party has beaten the hydros statues at the bottom of the mine. The row
/// that needs the flag clear comes first in each table, so the first row that
/// matches picks the time.
///
/// In the village (Altin), three villagers come home one by one as the water
/// drops. The armor and item merchants wait outside their flooded shops until
/// theirs dry. The item merchant's dries after the first drop, the armor
/// merchant's after the second.
///
/// A water beast spits water at the houses. It is drawn with its green and blue
/// swapped (SPRITE_COLOR_RBG), and its sound and its spray
/// (Altin_SprayInitialize) run from Altin_ApplyWaterLevel. When the party steps
/// near it (Altin_ScareOffBeast) it stops, cries out and hops away into the
/// flood for good (FLAG_ALTIN_WATER_BEAST_LEFT).
///
/// Doors lead into CottageRoomsD and into the Altin Sanctum (ALTIN_TO_SANCTUM).
/// They are step triggers 2 to 9, run by Altin_EnterDoor, each leaving by the
/// exit of its number. The Sanctum's wide door is redrawn by hand. Coming back
/// out of the Sanctum (ALTIN_FROM_SANCTUM) the area name is not shown again.
///
/// Three doorways (step triggers 10 to 12, Altin_WalkThroughDoorway) and the
/// exit ALTIN_TO_ALTIN_PEAK_SPRITZ lead into the mine, to Altin Peak's
/// AltinPeakFirstWaterBeast, AltinPeakSouthEntrance, AltinPeakJunction and
/// AltinPeakSpritz.
///
/// A pillar that Move pushes to column 22 stays there
/// (FLAG_ALTIN_PILLAR_MOVED).
///
/// Inside, in Cottage rooms D (CottageRoomsD), two families shelter in the
/// inn's Room One and Room Two, as the game calls them. The first and the
/// second drop send them home (CottageRoomsD_PlaceSheltering). The weapon
/// merchant waits in the inn until the water has drained.
///
/// Each merchant opens across the counter once the shop has dried, and a cook
/// from Xian paces between his pots (gCottageRoomsD_CookScript).

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "scene_ids.h"
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
#include "sound/sound.h"
#include "system/main_routine_work.h"
#include "system/system.h"
#include "system/system_call.h"
#include "text/msg_ids.h"

extern struct SceneEntrance gAltin_NoEntrances[];
extern struct SceneEntrance gAltin_Entrances[];
extern struct SceneEntrance gCottageRoomsD_Entrances[];
extern u32 gAltin_Exits[];
extern struct ActorPlacement gAltin_NoActorPlacements[];
extern struct ActorPlacement gAltin_ActorPlacements[];
extern struct ActorPlacement gCottageRoomsD_ActorPlacements[];
extern struct Trigger gAltin_NoTriggers[];
extern struct Trigger gAltin_Triggers[];
extern struct Trigger gCottageRoomsD_Triggers[];
extern struct MapGridCellAnimation gAltin_Doors[];

#if !TBS_PATCH_3
#define ALTIN_COOK_LAST_POT_X 384
#else
#define ALTIN_COOK_LAST_POT_X 380
#endif

#define MAP_ALTIN_VILLAGE (SCENE_ALTIN)
#define MAP_ALTIN_HOUSES (SCENE_COTTAGE_ROOMS_D)

/// The village's actors, numbering the rows of gAltin_ActorPlacements.
///
/// The people without an event of their own are named by their sprite and by
/// what they say before the hydros statues are beaten.
enum AltinVillageActorId {
  /// A bald, bare-chested strong man (SPRITE_BALD_MUSCULAR_MAN). His event is
  /// Altin_AskAboutFirstVisit.
  ACTOR_GREETING_STRONGMAN = ACTOR_FIRST_PLACED,
  /// A stocky strong man flexing his arms (SPRITE_STRONGMAN). His event is
  /// Altin_AskAboutHydros.
  ACTOR_CURIOUS_STRONGMAN,
  /// A woman with fair hair in a white apron (SPRITE_WOMAN_WITH_FAIR_HAIR).
  ACTOR_WOMAN_COATED_HOUSE,
  /// A burly grey-haired man (SPRITE_BURLY_MAN_WITH_GREY_HAIR), who wanders.
  ACTOR_MAN_SUBMERGED_MINE,
  /// A bent old man with a cane (SPRITE_OLD_MAN_WITH_CANE).
  ACTOR_OLD_MAN_DESTROY_MONSTERS,
  /// A woman in a green dress (SPRITE_WOMAN_IN_GREEN_DRESS).
  ACTOR_WOMAN_WATER_LUCK,
  /// An old woman with white hair (SPRITE_OLD_WOMAN). Her event is
  /// Altin_TalkToThirstyOldWoman.
  ACTOR_THIRSTY_OLD_WOMAN,
  /// Three villagers come back as the water recedes, one with each drop
  /// (Altin_ApplyWaterLevel). They are this man with brown hair
  /// (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR), then a strong man and an old man.
  ACTOR_MAN_WATERS_SUBSIDED,
  /// A bald, bare-chested strong man (SPRITE_BALD_MUSCULAR_MAN).
  ACTOR_STRONGMAN_GUARDIAN_STATUES,
  /// A bent old man with a cane (SPRITE_OLD_MAN_WITH_CANE), who wanders.
  ACTOR_OLD_MAN_ROCK_FLUSHED_OUT,
  /// The beast spitting water at the houses until FLAG_ALTIN_WATER_BEAST_LEFT.
  ACTOR_WATER_BEAST,
  /// SPRITE_STONE_STATUE, which Move pushes to column 22
  /// (Altin_CheckPillarMoved).
  ACTOR_STONE_STATUE,
  /// The armor and item merchants, waiting outside while their shops are
  /// under water.
  ACTOR_FLOODED_ARMOR_MERCHANT,
  ACTOR_FLOODED_ITEM_MERCHANT,
  /// Three crates placed with SPRITE_CRATE. Altin_ApplyWaterLevel hides them
  /// (SPRITE_COLOR_HIDDEN) but leaves them solid, as placed actors are.
  ///
  /// They are invisible blocks. One is at the top of the ladder on the east
  /// cliff, and two are on the rails below the doorway into the mine (the
  /// drained village's render with them drawn).
  ACTOR_FIRST_CRATE,
  ACTOR_SECOND_CRATE,
  ACTOR_THIRD_CRATE
};

/// The houses' actors, numbering the rows of
/// gCottageRoomsD_ActorPlacements, named as the village's are.
enum AltinHouseActorId {
  /// The family sheltering in the inn's Room One until the first drop. They are
  /// this man with brown hair (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR) and a woman
  /// with fair hair.
  ACTOR_ROOM_ONE_GUEST = ACTOR_FIRST_PLACED,
  /// A woman with fair hair in a white apron (SPRITE_WOMAN_WITH_FAIR_HAIR).
  ACTOR_ROOM_ONE_SECOND_GUEST,
  /// The family in Room Two, until the second drop. They are this bent old man
  /// with a cane (SPRITE_OLD_MAN_WITH_CANE) and an old woman.
  ACTOR_ROOM_TWO_GUEST,
  /// An old woman with white hair (SPRITE_OLD_WOMAN).
  ACTOR_ROOM_TWO_SECOND_GUEST,
  /// A man with brown hair (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR).
  ///
  /// He and ACTOR_WOMAN_NO_PLACE_LIKE_HOME are the kissing girl's "Mom and
  /// Pop", who have been acting weird since the ruins were found. His thoughts
  /// won't give up a spot at the end of the mine's easternmost passage, and
  /// hers say "My husband's sense for finding treasure has never failed."
  ACTOR_MAN_BACK_IN_HIS_HOMETOWN,
  /// A woman with fair hair in a white apron (SPRITE_WOMAN_WITH_FAIR_HAIR).
  ACTOR_WOMAN_NO_PLACE_LIKE_HOME,
  /// A little girl with orange pigtails (SPRITE_GIRL_WITH_PIGTAILS). Her event
  /// is CottageRoomsD_AskAboutMonsters.
  ACTOR_KISSING_GIRL,
  ACTOR_WEAPON_MERCHANT,
  ACTOR_ITEM_MERCHANT,
  ACTOR_ARMOR_MERCHANT,
  /// Opens the inn (CottageRoomsD_TalkToInnkeeper).
  ACTOR_INNKEEPER,
  /// A woman with red-brown hair in a white apron (SPRITE_WOMAN_IN_PINK_APRON).
  ACTOR_WOMAN_FORMER_GLORY,
  ACTOR_COOK,
  /// A man with brown hair (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR) in the inn,
  /// telling of its guests going home as the water drops.
  ACTOR_MAN_WAITING_TO_GO_HOME
};

/// The village's water level, set on entry from the mine's drain flags.
enum {
  FLAG_ALTIN_WATER_DROPPED_ONCE = FLAG_SCENE_TEMP_40,
  FLAG_ALTIN_WATER_DROPPED_TWICE = FLAG_SCENE_TEMP_41,
  FLAG_ALTIN_WATER_DRAINED = FLAG_SCENE_TEMP_42
};

enum { SHOP_ALTIN_WEAPONS = 19, SHOP_ALTIN_ARMOR = 20, SHOP_ALTIN_ITEMS = 21 };

/// Altin's inn, as Inn_CheckIn numbers the inns.
enum { ALTIN_INN = 6 };

/// The cell behaviors Altin's triggers answer.
enum AltinCellBehavior {
  /// The doors' step triggers are numbered from 2 in the village's triggers,
  /// and each takes the exit of its own number.
  CELL_BEHAVIOR_ALTIN_FIRST_DOOR = 2,
  CELL_BEHAVIOR_ALTIN_NEAR_BEAST = 20
};

/// The doors of the village, by their index in gAltin_Doors.
enum {
  /// The Sanctum's door, which leads to SANCTUM_FROM_ALTIN.
  DOOR_SANCTUM = ALTIN_TO_SANCTUM - CELL_BEHAVIOR_ALTIN_FIRST_DOOR,
  DOOR_STRAIGHT_IN = ALTIN_TO_COTTAGE_ROOMS_D_EAST -
      CELL_BEHAVIOR_ALTIN_FIRST_DOOR
};

#define WATER_BEAST_HOP_PERIOD 60
#define WATER_SPRAY_LIFETIME 20
#define WATER_SPRAY_START_SCALE (3 * Q16_ONE / 10)

// UB: the facing test overflows a signed left shift for many headings.
#ifdef UBFIX
#define ALTIN_FACES_COUNTER(facing) FACING_IS_NORTH(facing)
#else
#define ALTIN_FACES_COUNTER(facing)                                            \
  ((u32)(((facing) + 0x5fff) << 16) <= 0x3ffe0000)
#endif

// UB: the random wobble can be negative. Both factors fit in signed32 bits
// for the actor's signed16-bit timer and wobble values.
#ifdef UBFIX
#define WATER_SPRAY_LATERAL_STEP(timer, wobble)                                \
  ((timer) * 4096 + (wobble) * 32768)
#else
#define WATER_SPRAY_LATERAL_STEP(timer, wobble)                                \
  (((timer) << 12) + ((wobble) << 15))
#endif

static void Altin_ApplyWaterLevel(void);
static void CottageRoomsD_PlaceSheltering(void);
static void Altin_BeastSpawnSpray(void);

const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 sceneId = gMainRoutineWork.next;

  if (sceneId == MAP_ALTIN_VILLAGE) {
    return gAltin_Entrances;
  }
  if (sceneId == MAP_ALTIN_HOUSES) {
    return gCottageRoomsD_Entrances;
  }
  return gAltin_NoEntrances;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return 0; }

/// Both scenes share one exit table.
const u32 *Scene_GetExits(void) { return gAltin_Exits; }

/// Some villagers only turn back after talking once the flood is over or
/// their family has gone home.
const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s32 sceneId = gMainRoutineWork.next;
  if (sceneId == MAP_ALTIN_VILLAGE) {
    if (GameFlag_Get(FLAG_HYDROS_STATUES_DEFEATED)) {
      gAltin_ActorPlacements[ACTOR_WOMAN_WATER_LUCK - ACTOR_FIRST_PLACED]
          .talkFacing = TALK_FACING_PARTY;
      gAltin_ActorPlacements[ACTOR_THIRSTY_OLD_WOMAN - ACTOR_FIRST_PLACED]
          .talkFacing = TALK_FACING_PARTY;
    }
    return gAltin_ActorPlacements;
  }
  if (sceneId == MAP_ALTIN_HOUSES) {
    if (GameFlag_Get(FLAG_ALTIN_MINE_FIRST_DRAIN))
      gCottageRoomsD_ActorPlacements[ACTOR_ROOM_ONE_SECOND_GUEST -
                                     ACTOR_FIRST_PLACED]
          .talkFacing = TALK_FACING_PARTY_AND_BACK;
    if (GameFlag_Get(FLAG_ALTIN_MINE_SECOND_DRAIN) ||
        GameFlag_Get(FLAG_ALTIN_MINE_THIRD_DRAIN))
      gCottageRoomsD_ActorPlacements[ACTOR_ROOM_TWO_SECOND_GUEST -
                                     ACTOR_FIRST_PLACED]
          .talkFacing = TALK_FACING_PARTY_AND_BACK;
    Scene_AssignViewFlags(gCottageRoomsD_ActorPlacements);
    return gCottageRoomsD_ActorPlacements;
  }
  return gAltin_NoActorPlacements;
}

const struct Trigger *Scene_GetTriggers(void) {
  s16 sceneId = gMainRoutineWork.next;

  if (sceneId == MAP_ALTIN_VILLAGE) {
    return gAltin_Triggers;
  }
  if (sceneId == MAP_ALTIN_HOUSES) {
    return gCottageRoomsD_Triggers;
  }
  return gAltin_NoTriggers;
}

/// A bald strong man asks whether this is the party's first visit.
static void Altin_AskAboutFirstVisit(void) {
  Event_Start();
  Event_SetMessageId(MsgAltinYourFirstTimeVisitAltin);

  // "Is this your first time to visit Altin?"
  Event_OpenMessage(ACTOR_GREETING_STRONGMAN, 0);

  if (Event_AskYesNoWithActorAnimation(0, 0) == 1) {
    // "Look at how much Altin has changed! You can't do much here..."
    Event_ShowMessage(ACTOR_GREETING_STRONGMAN, 0);
  } else {
    Event_SkipMessages(1);
    // "We must look a lot like a coastal town, huh?"
    // Yes: "Nothing gets by you... Altin never even had a well in the..."
    // No: "The day the gems fell from the sky, these water-spewing..."
    Event_AskYesNo(ACTOR_GREETING_STRONGMAN, 0);
  }

  Event_End();
}

/// A strong man asks whether the slain hydros creatures gushed water. He's in
/// Altin's village once the hydros statues are beaten.
static void Altin_AskAboutHydros(void) {
  Event_Start();
  Event_SetMessageId(MsgAltinDidSeeWaterGushingOut);
  // "Did you see water gushing out when you slew those hydros..."
  // Yes: "I see... So the water monsters are like giant water tanks..."
  // No: "Really... Then how could they have carried all that water?"
  Event_AskYesNo(ACTOR_CURIOUS_STRONGMAN, 0);
  Event_End();
}

/// An old woman in Altin's village.
static void Altin_TalkToThirstyOldWoman(void) {
  struct Actor *beast;
  struct Actor *leader;

  Event_Start();
  if (GameFlag_Get(FLAG_HYDROS_STATUES_DEFEATED) != 0) {
    Event_SetMessageId(MsgAltinOnesWhoDefeatedWaterBeasts);
    // "Are you the ones who defeated the water beasts?"
    // Yes: "We'd be much obliged if you'd also help rebuild the village."
    // No: "I heard our rescuers were young. I'd like to meet them."
    Event_AskYesNo(ACTOR_THIRSTY_OLD_WOMAN, 0);
  } else {
    Actor_SetAnimation(ACTOR_THIRSTY_OLD_WOMAN, ANIM_SHAKE_HEAD);
    Event_SetMessageId(MsgAltinWeCantDrinkWaterMonsters);
    // "We can't drink the water the monsters spew! It's disgusting...."
    Event_ShowMessageThenWait(ACTOR_THIRSTY_OLD_WOMAN, 0, 10);
    if (GameFlag_Get(FLAG_ALTIN_WATER_BEAST_LEFT) == 0) {
      beast = Actor_Get(ACTOR_WATER_BEAST);

      FieldCamera_GetActor()->motionFlags = 0;
      FieldCamera_SetSpeed(1);
      FieldCamera_MoveToFixed(beast->x.fixed, beast->y.fixed, beast->z.fixed,
                              1);
      // BUG: FACING_SOUTH's value is passed as an actor id. No actor has
      // it, so the leader does not turn south toward the beast.
#ifdef BUGFIX
      Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
#else
      Actor_FaceActor(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
#endif
      Actor_FaceDirection(ACTOR_THIRSTY_OLD_WOMAN, FACING_SOUTH_SOUTHEAST, 0);
      FieldCamera_WaitForArrival();
      Event_Wait(120);
      leader = Actor_Get(ACTOR_PARTY_LEADER);
      FieldCamera_MoveToFixed(leader->x.fixed, leader->y.fixed, leader->z.fixed,
                              1);
      FieldCamera_WaitForArrival();
    }
    Actor_SetAnimationAndWait(ACTOR_THIRSTY_OLD_WOMAN, ANIM_SHAKE_HEAD);
  }
  Event_End();
}

/// An old man asks whether the party really found ruins deep in the mine. He's
/// in Altin's village once the hydros statues are beaten.
static void Altin_AskAboutRuins(void) {
  Event_Start();
  Event_SetMessageId(MsgAltinTrueFoundAncientRuinsIn);
  // "Is it true you found ancient ruins in the deepest part of..."
  // Yes: "Really? I always thought that mine held some kind of secret."
  // No: "Hmmm, are you sure you even went down there? Someone ought..."
  Event_AskYesNo(ACTOR_OLD_MAN_ROCK_FLUSHED_OUT, 0);
  Event_End();
}

/// A woman asks whether the party is who the girl from Xian was looking for.
static void CottageRoomsD_AskAboutXianGirl(void) {
  Event_Start();
  Event_SetMessageId(MsgAltinGirlFromXianWasAsking);
  // "A girl from Xian was asking questions about young warriors...."
  // Yes: "She was searching very hard."
  // No: "Well, if not, I hope she finds whoever she was looking for."
  Event_AskYesNo(ACTOR_ROOM_ONE_SECOND_GUEST, 0);
  Event_End();
}

/// An old man points the party to the bottom of the mine.
static void CottageRoomsD_AskAboutWayWest(void) {
  Event_Start();
  Event_SetMessageId(MsgAltinTryingFindYourWayWest);
  // "Are you trying to find your way west through the mountains?"
  // Yes: "There should be a passage that leads west at the bottom of..."
  // No: "I don't know what you did, but we are grateful you defeated..."
  Event_AskYesNo(ACTOR_ROOM_TWO_GUEST, 0);
  Event_End();
}

/// A little girl thanks the leader with a kiss.
static void CottageRoomsD_AskAboutMonsters(void) {
  Event_Start();
  Event_SetMessageId(MsgAltinDefeatedThoseMonstersDidnt);
  // "You defeated those monsters, didn't you?"
  // Yes: "Here, let me thank you with...a kiss! Hey, you're turning..."
  // No: "Oh, you didn't? Still, I'm happy to be back home, so here's..."
  Event_AskYesNo(ACTOR_KISSING_GIRL, 0);
  Event_End();
}

/// A man at the inn asks whether the remaining beasts are deep below. He's
/// staying at Altin's inn once the hydros statues are beaten.
static void CottageRoomsD_AskAboutMineBeasts(void) {
  Event_Start();
  Event_SetMessageId(MsgAltinThereFewBeastsInMine);
  // "There are a few beasts in the mine still, but they're down..."
  // Yes: "Good. Then we can just mine the upper passages."
  // No: "If they're in the upper passages, then we can't mine like we..."
  Event_AskYesNo(ACTOR_MAN_WAITING_TO_GO_HOME, 0);
  Event_End();
}

/// Returns whether the party leader faces north, toward a shop counter.
static s32 Altin_LeaderFacesCounter(void) {
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);

  if (ALTIN_FACES_COUNTER(leader->facing)) {
    return 1;
  }
  return 0;
}

/// Altin's weapon merchant waits in the inn until the water drains, and cannot
/// sell.
static void CottageRoomsD_TalkToWeaponMerchant(void) {
  if (GameFlag_Get(FLAG_ALTIN_WATER_DRAINED) == 0) {
    Event_Start();
    Event_SetMessageId(MsgAltinDoWantWeapons);

    // "Do you want weapons?"
    // Yes: "I'll sell you anything you want once the waters have..."
    // No: "You wouldn't say that if you saw my weapons. I wish I could..."
    Event_AskYesNo(ACTOR_WEAPON_MERCHANT, 0);
    Event_End();
    return;
  }

  if (Altin_LeaderFacesCounter() != 0) {
    Shop_Run(SHOP_ALTIN_WEAPONS, ACTOR_WEAPON_MERCHANT);
    return;
  }

  Event_Start();
  Event_SetMessageId(MsgAltinThankGoodnessWaterHasReceded);
  if (GameFlag_Get(FLAG_HYDROS_STATUES_DEFEATED) != 0) {
    Event_SetMessageId(MsgAltinYoullHaveFindPassageIn);
  }
  Event_ShowMessage(ACTOR_WEAPON_MERCHANT, 0);
  Event_End();
}

/// Altin's armor merchant speaks in either scene.
static void Altin_TalkToArmorMerchant(void) {
  if (GameFlag_Get(FLAG_ALTIN_WATER_DROPPED_TWICE) == 0) {
    Event_Start();
    Event_SetMessageId(MsgAltinMyStoreSubmergedWantSell);
    // "My store is submerged... I want to sell armor, but there's..."
    Event_ShowMessage(ACTOR_FLOODED_ARMOR_MERCHANT, 0);
    Event_End();
    return;
  }

  if (Altin_LeaderFacesCounter() != 0) {
    Shop_Run(SHOP_ALTIN_ARMOR, ACTOR_ARMOR_MERCHANT);
    return;
  }

  Event_Start();
  Event_SetMessageId(MsgAltinItsGreatCanSellArmor);
  if (GameFlag_Get(FLAG_HYDROS_STATUES_DEFEATED) != 0) {
    Event_SetMessageId(MsgAltinHowAboutArentImpressedBy);
  }
  Event_ShowMessage(ACTOR_ARMOR_MERCHANT, 0);
  Event_End();
}

/// Altin's item merchant works the same way, and reopens after the first drop.
static void Altin_TalkToItemMerchant(void) {
  if (GameFlag_Get(FLAG_ALTIN_WATER_DROPPED_ONCE) == 0) {
    Event_Start();
    Event_SetMessageId(MsgAltinWillDoIfMyMerchandise);
    // "Oh, no! What will I do if all my merchandise is soaked?"
    Event_ShowMessage(ACTOR_FLOODED_ITEM_MERCHANT, 0);
    Event_End();
    return;
  }

  if (Altin_LeaderFacesCounter() != 0) {
    Shop_Run(SHOP_ALTIN_ITEMS, ACTOR_ITEM_MERCHANT);
    return;
  }

  Event_Start();
  Event_SetMessageId(MsgAltinNoneMyGoodsWereDamaged);
  if (GameFlag_Get(FLAG_HYDROS_STATUES_DEFEATED) != 0) {
    Event_SetMessageId(MsgAltinGirlFromXianBoughtLot);
  }
  Event_ShowMessage(ACTOR_ITEM_MERCHANT, 0);
  Event_End();
}

/// Altin's innkeeper opens the inn when spoken to across the counter.
static void CottageRoomsD_TalkToInnkeeper(void) {
  if (ALTIN_FACES_COUNTER(Actor_Get(ACTOR_PARTY_LEADER)->facing)) {
    Inn_CheckIn(ALTIN_INN, ACTOR_INNKEEPER);
    return;
  }

  Event_Start();

  if (GameFlag_Get(FLAG_HYDROS_STATUES_DEFEATED) != 0) {
    Event_SetMessageId(MsgAltinThereSmallTempleWestAltin);
    // "There is a small temple to the west of Altin Mine. They may..."
    Event_ShowMessage(ACTOR_INNKEEPER, 0);
  } else {
    Event_SetMessageId(MsgAltinWeGotLittleDampBut);
    // "We got a little damp, but we're doing okay. We must count..."
    // Yes: "After all, a lot of good people have been hurt in this flooding."
    // No: "You have to remember, lots of people lost everything they..."
    Event_AskYesNo(ACTOR_INNKEEPER, 0);
  }

  Event_End();
}

/// The party leader walks north into one of the doors of Altin's village.
static void Altin_EnterDoor(void) {
  u32 actorId;
  struct FieldWork *work;
  struct Actor *actor;
  s32 doorIndex;
  s32 column;
  s32 row;
  s32 rightColumn;

  work = gFieldWork;
  Event_Start();
  for (actorId = ACTOR_FIRST_PLACED; actorId <= ACTOR_LAST_ID; actorId++) {
    actor = Actor_Get(actorId);
    if (actor != NULL) {
      actor->motionFlags = 0;
    }
  }

  doorIndex = (s16)(work->stepTrigger - CELL_BEHAVIOR_ALTIN_FIRST_DOOR);
  column = (s16)gAltin_Doors[doorIndex].x;
  row = (s16)gAltin_Doors[doorIndex].y;
  if (doorIndex == DOOR_SANCTUM) {
    // The Sanctum's door slides open with SE_THUMP, as Bilibin's does.
    Sound_Play(SE_THUMP);
    MapGrid_CopyCellsTo(42, 33, column, row, 2, 2);
    rightColumn = column + 2;
    MapGrid_CopyCellsTo(42, 35, rightColumn, row, 2, 2);
    Event_Wait(4);
    MapGrid_CopyCellsTo(40, 33, column, row, 2, 2);
    MapGrid_CopyCellsTo(40, 35, rightColumn, row, 2, 2);
    Event_Wait(4);
  } else {
    Sound_Play(SE_DOOR_OPEN);
    // The north door's row in gAltin_Doors plays its opening at
    // (39, 17), outside the village's camera box; its house stands at
    // (8, 17), above where the party comes back out
    // (ALTIN_FROM_COTTAGE_ROOMS_D_NORTH), so that doorway is drawn open here
    // (a render with this copy made on entry shows the open doorway).
    if (doorIndex ==
        ALTIN_TO_COTTAGE_ROOMS_D_NORTH - CELL_BEHAVIOR_ALTIN_FIRST_DOOR) {
      MapGrid_CopyCellsTo(33, 42, 8, 17, 1, 2);
    }
    MapGrid_AnimateCells(gAltin_Doors[doorIndex].steps, column, row);
  }

  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.5);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Actor_Get(ACTOR_PARTY_LEADER)->motionFlags = 0;
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  if (doorIndex == DOOR_STRAIGHT_IN) {
    Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, 0);
  } else {
    if (doorIndex != DOOR_SANCTUM) {
      Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, -4);
    } else {
      Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 2);
      Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -4);
    }
  }
  Event_Wait(10);
  Import_EventSetExitRequest(work->stepTrigger);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

/// The leader walks out through one of the doorways of Altin's village that
/// lead into the mine, on walking north into it.
static void Altin_WalkThroughDoorway(void) {
  struct FieldWork *work;

  work = gFieldWork;
  Event_Start();
  Actor_Get(ACTOR_PARTY_LEADER)->motionFlags = 0;
  Sound_Play(SE_MAP_EXIT);
  Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, -16);
  Import_EventSetExitRequest(work->stepTrigger);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

/// Update routine of the hopping water beast in Altin's village.
static void Altin_BeastHop(struct Actor *actor) {
  struct Actor *beast = actor;
  s16 *shakeFrames = (s16 *)&beast->userValue;

  // While this update routine owns the beast, timer is its hop phase, work
  // counts down the landing shake, and the effect's countdown is a
  // landing-sound latch. Read work through its corresponding signed type for
  // the countdown test.
  if (*shakeFrames != 0) {
    if (--*shakeFrames == 1)
      MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  }
  if (beast->velocityY == 0) {
    Actor_SelectAnimation(beast, 1);
    beast->y.fixed -= px(3) / 2;
    if (beast->y.fixed < beast->groundHeight) {
      if (((struct FieldEffect *)actor)->countdown != 0) {
        Sound_Play(SE_SPLASH);
        ((struct FieldEffect *)actor)->countdown = 0;
        beast->userValue = 4;
        MapGrid_SetShake(0, 1, 1);
      }
      beast->y.fixed = beast->groundHeight;
    }
    beast->paused = 1;
  } else {
    beast->paused = 0;
  }
  if (beast->timer == 0) {
    Sound_Play(SE_JUMP);
    ((struct FieldEffect *)actor)->countdown = 1;
    Actor_SelectAnimation(beast, 2);
    beast->velocityY = px(3);
  }
  if (++beast->timer == WATER_BEAST_HOP_PERIOD)
    beast->timer = 0;
}

/// The water beast hops away across the flood and dives in with a splash.
static void Altin_BeastHopAway(void) {
  struct Actor *beast;

  beast = Actor_Get(ACTOR_WATER_BEAST);
  beast->timer = 0;
  beast->userValue = 0;
  beast->gravity = px(2) / 5;
  beast->update = Altin_BeastHop;
  Actor_SetSpeed(ACTOR_WATER_BEAST, 1.2);
  Actor_MoveToAndWait(ACTOR_WATER_BEAST, 28, 460);
  Actor_MoveToAndWait(ACTOR_WATER_BEAST, 24, 448);
  Sound_Play(SE_SPLASH);
  Actor_ReleaseById(ACTOR_WATER_BEAST);
  MapGrid_SetShake(0, 1, 1);
  Event_Wait(4);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  Event_Wait(40);
  Actor_SetAnimation(ACTOR_WATER_BEAST, ANIM_STAND);
}

/// After Move, remembers whether the pillar now stands in map column 22, the
/// spot it is pushed to.
static void Altin_CheckPillarMoved(void) {
  struct Actor *pillar = Actor_Get(ACTOR_STONE_STATUE);

  if (FIELD_CELL(pillar->x.fixed) == 22) {
    GameFlag_Set(FLAG_ALTIN_PILLAR_MOVED);
  } else {
    GameFlag_Clear(FLAG_ALTIN_PILLAR_MOVED);
  }
}

/// Sets up the scene being entered, at the village's water level. It works out
/// the water level from the mine's drain flags first. Returns 0.
s32 Scene_Initialize(void) {
  s16 sceneId;

  if (GameFlag_Get(FLAG_ALTIN_MINE_FIRST_DRAIN) != 0) {
    GameFlag_Set(FLAG_ALTIN_WATER_DROPPED_ONCE);
  }

  if (GameFlag_Get(FLAG_ALTIN_MINE_SECOND_DRAIN) != 0 ||
      GameFlag_Get(FLAG_ALTIN_MINE_THIRD_DRAIN) != 0) {
    GameFlag_Set(FLAG_ALTIN_WATER_DROPPED_TWICE);
  }

  if (GameFlag_Get(FLAG_ALTIN_MINE_SECOND_DRAIN) != 0 &&
      GameFlag_Get(FLAG_ALTIN_MINE_THIRD_DRAIN) != 0) {
    GameFlag_Set(FLAG_ALTIN_WATER_DRAINED);
  }

  sceneId = gMainRoutineWork.next;
  if (sceneId == MAP_ALTIN_VILLAGE) {
    Altin_ApplyWaterLevel();
  } else if (sceneId == MAP_ALTIN_HOUSES) {
    CottageRoomsD_PlaceSheltering();
  }

  return 0;
}

/// Sets up Altin's village (Altin).
static void Altin_ApplyWaterLevel(void) {
  struct Actor *leader;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (GameFlag_Get(FLAG_ALTIN_WATER_DRAINED) != 0) {
    MapGrid_CopyCellsTo(64, 32, 0, 32, 32, 32);
    MapGrid_CopyCellAttributes(64, 32, 32, 32, 0, 0);
    Actor_ReleaseById(ACTOR_FLOODED_ARMOR_MERCHANT);
    Actor_ReleaseById(ACTOR_FLOODED_ITEM_MERCHANT);
  } else if (GameFlag_Get(FLAG_ALTIN_WATER_DROPPED_TWICE) != 0) {
    MapGrid_CopyCellsTo(64, 0, 0, 32, 32, 32);
    MapGrid_CopyCellAttributes(64, 0, 32, 32, 0, 0);
    Actor_ReleaseById(ACTOR_OLD_MAN_ROCK_FLUSHED_OUT);
    Actor_ReleaseById(ACTOR_FLOODED_ARMOR_MERCHANT);
    Actor_ReleaseById(ACTOR_FLOODED_ITEM_MERCHANT);
  } else if (GameFlag_Get(FLAG_ALTIN_WATER_DROPPED_ONCE) != 0) {
    MapGrid_CopyCellsTo(0, 64, 0, 32, 32, 32);
    MapGrid_CopyCellAttributes(0, 64, 32, 32, 0, 0);
    Actor_ReleaseById(ACTOR_STRONGMAN_GUARDIAN_STATUES);
    Actor_ReleaseById(ACTOR_OLD_MAN_ROCK_FLUSHED_OUT);
    Actor_ReleaseById(ACTOR_FLOODED_ITEM_MERCHANT);
  } else {
    MapGrid_CopyCellAttributes(0, 32, 32, 32, 0, 0);
    Actor_ReleaseById(ACTOR_MAN_WATERS_SUBSIDED);
    Actor_ReleaseById(ACTOR_STRONGMAN_GUARDIAN_STATUES);
    Actor_ReleaseById(ACTOR_OLD_MAN_ROCK_FLUSHED_OUT);
  }
  if (GameFlag_Get(FLAG_ALTIN_WATER_BEAST_LEFT) != 0) {
    Actor_ReleaseById(ACTOR_WATER_BEAST);
  } else {
    Scene_QueueMusic(SE_AMBIENT_WATER);
    Actor_SetSpriteColorById(ACTOR_WATER_BEAST, SPRITE_COLOR_RBG);
    Actor_SetAnimation(ACTOR_WATER_BEAST, WATER_BEAST_ANIMATION_SPIT);
    Task_AddFrame(Altin_BeastSpawnSpray, TASK_PASS_FRAME);
  }
  if (gMainRoutineWork.entrance == ALTIN_FROM_SANCTUM) {
    GameFlag_Clear(FLAG_SHOW_AREA_NAME);
  }
  MapGrid_CopyCellAttributes(0, 33, 4, 3, 20, 41);
  if (GameFlag_Get(FLAG_ALTIN_PILLAR_MOVED) != 0) {
    Actor_SetPosition(ACTOR_STONE_STATUE, 360, 168);
  }
  Actor_SetSpriteFlags(Actor_Get(ACTOR_STONE_STATUE), 0);
  Actor_SetSpriteColorById(ACTOR_FIRST_CRATE, SPRITE_COLOR_HIDDEN);
  Actor_SetSpriteColorById(ACTOR_SECOND_CRATE, SPRITE_COLOR_HIDDEN);
  Actor_SetSpriteColorById(ACTOR_THIRD_CRATE, SPRITE_COLOR_HIDDEN);
  Actor_Get(ACTOR_FIRST_CRATE)->collisionFlags |= ACTOR_NOT_TARGETED;
  Actor_Get(ACTOR_SECOND_CRATE)->collisionFlags |= ACTOR_NOT_TARGETED;
  Actor_Get(ACTOR_THIRD_CRATE)->collisionFlags |= ACTOR_NOT_TARGETED;
  Actor_Get(ACTOR_FIRST_CRATE)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  Actor_Get(ACTOR_SECOND_CRATE)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  Actor_Get(ACTOR_THIRD_CRATE)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  Actor_SetSpritePriority(ACTOR_FIRST_CRATE, 1);
  Actor_SetSpritePriority(ACTOR_SECOND_CRATE, 1);
  Actor_SetSpritePriority(ACTOR_THIRD_CRATE, 1);
  wait(1);
  Event_Start();
  FieldCamera_MoveToFixed(leader->x.fixed, leader->y.fixed, leader->z.fixed, 0);
  MapGrid_CenterOnFollowedPoint();
  Event_End();
  wait(1);
}

/// Sets up inside Altin's buildings (CottageRoomsD).
static void CottageRoomsD_PlaceSheltering(void) {
  struct Actor *actor;

  if (GameFlag_Get(FLAG_ALTIN_WATER_DROPPED_ONCE) == 0) {
    Actor_SetPosition(ACTOR_ROOM_ONE_GUEST, 808, 727);
    Actor_Get(ACTOR_ROOM_ONE_GUEST)->facing = FACING_SOUTH_SOUTHEAST;
    Actor_SetPosition(ACTOR_ROOM_ONE_SECOND_GUEST, 794, 825);
  }
  if (GameFlag_Get(FLAG_ALTIN_WATER_DROPPED_TWICE) == 0) {
    Actor_SetPosition(ACTOR_ROOM_TWO_GUEST, 560, 710);
    Actor_Get(ACTOR_ROOM_TWO_GUEST)->facing = FACING_EAST_SOUTHEAST;
    Actor_SetPosition(ACTOR_ROOM_TWO_SECOND_GUEST, 576, 710);
  }
  if (GameFlag_Get(FLAG_ALTIN_WATER_DRAINED) == 0) {
    Actor_SetPosition(ACTOR_WEAPON_MERCHANT, 295, 744);
    Actor_Get(ACTOR_WEAPON_MERCHANT)->facing = FACING_NORTH_NORTHWEST;
  } else {
    actor = Actor_Get(ACTOR_WEAPON_MERCHANT);
    actor->collisionFlags |= ACTOR_NEAR_TALK;
  }
  actor = Actor_Get(ACTOR_ARMOR_MERCHANT);
  if (actor != NULL) {
    actor->collisionFlags |= ACTOR_NEAR_TALK;
  }
  actor = Actor_Get(ACTOR_ITEM_MERCHANT);
  if (actor != NULL) {
    actor->collisionFlags |= ACTOR_NEAR_TALK;
  }
}

/// Gives a new spray particle its look.
static void Altin_SprayInitialize(struct Actor *spray) {
  spray->motionFlags = 0;
  spray->sprite->image.priority = 1;
  Actor_SetSpriteColor(spray, SPRITE_COLOR_BGR);
  Actor_SetSpriteFlags(spray, 0);
  spray->scaleX = WATER_SPRAY_START_SCALE;
  spray->scaleY = WATER_SPRAY_START_SCALE;
}

/// Update routine of a spray particle.
static void Altin_SprayDrift(struct Actor *spray) {
  spray->x.fixed +=
      WATER_SPRAY_LATERAL_STEP(spray->timer, (s16)((s32)(Random_Below(2)) - 1));
  if (spray->timer <= 3) {
    spray->z.fixed -= Random_Below(px(1) / 2) + px(1);
    spray->scaleX += 3 * Q16_ONE / 20;
    spray->scaleY -= Q16_ONE / 25;
  } else {
    spray->z.fixed += px(2);
    spray->scaleX += 3 * Q16_ONE / 100;
    spray->scaleY += 3 * Q16_ONE / 100;
  }
  if (Random_Below(spray->timer) == 0)
    Actor_SetSpriteColor(spray, SPRITE_COLOR_WHITE);
  if (spray->timer != 0)
    spray->timer--;
  else
    spray->timer = (Random_Below(5)) * 2 + 2;
  if (--((struct FieldEffect *)spray)->countdown == 0) {
    spray->update = 0;
    Actor_Release(spray);
  }
}

/// Task while the water beast spits. Every fourth frame a spray particle starts
/// from its spout, living 20 frames.
static void Altin_BeastSpawnSpray(void) {
  struct Actor *spray;
  if ((gFrameCount & 3) == 0) {
    spray = ((struct Actor *)FieldActor_Create(SPRITE_DUST_PUFF, px(64), 0,
                                               px(400)));
    if (spray != NULL) {
      spray->timer = WATER_SPRAY_LIFETIME;
      spray->userValue = 0;
      ((struct FieldEffect *)spray)->countdown = WATER_SPRAY_LIFETIME;
      Altin_SprayInitialize(spray);
      spray->update = Altin_SprayDrift;
      Actor_SelectAnimation(spray, 1);
    }
  }
}

/// The party scares off the water beast by stepping near it in Altin's village.
static void Altin_ScareOffBeast(void) {
  Event_Start();
  FieldCamera_SetSpeed(0.4);
  FieldCamera_MoveTo(63, CAMERA_KEEP, 450, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(30);
  Actor_SetAnimation(ACTOR_WATER_BEAST, ANIM_STAND);
  // The spitting stops: its sound and its spray end.
  Scene_QueueMusic(-1);
  Task_Remove(Altin_BeastSpawnSpray);
  Event_Wait(20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_WATER_BEAST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_WATER_BEAST, FACING_EAST, 20);
  Actor_FaceDirection(ACTOR_WATER_BEAST, FACING_NORTH_NORTHEAST, 40);
  Sound_Play(SE_BEAST_CRY);
  Actor_RunRepeatedMotion(ACTOR_WATER_BEAST, 2);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_WATER_BEAST, FACING_NORTH_NORTHWEST, 40);
  Altin_BeastHopAway();
  FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 1);
  FieldCamera_WaitForArrival();
  Actor_SetAnimationAndWait(ACTOR_THIRSTY_OLD_WOMAN, ANIM_SHAKE_HEAD);
  GameFlag_Set(FLAG_ALTIN_WATER_BEAST_LEFT);
  Event_End();
}

/// The cook from Xian paces between his pots, nodding over each.
s32 gCottageRoomsD_CookScript[] = {
    SC_ADD(SW_COLLISION, 128),
    SC_SPEED(2.4),
    SC_ACCELERATION(1.2),
    SC_LABEL(1),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(342, 0, 746),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_NORTH_NORTHWEST),
    SC_ANIMATION(ANIM_NOD),
    SC_WAIT(30),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(356, 0, 746),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
    SC_ANIMATION(ANIM_NOD),
    SC_WAIT(50),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(342, 0, 746),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_NORTH_NORTHWEST),
    SC_ANIMATION(ANIM_NOD),
    SC_WAIT(20),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(ALTIN_COOK_LAST_POT_X, 0, 746),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
    SC_ANIMATION(ANIM_NOD),
    SC_WAIT(30),
    SC_SET(SW_FACING, FACING_EAST_SOUTHEAST),
    SC_WAIT(90),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
};

struct SceneEntrance gAltin_NoEntrances[] = {
    {0, CONDITION_ALWAYS, 380, 0, 433, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gAltin_Entrances[] = {
    {0, CONDITION_ALWAYS, 380, 0, 433, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {ALTIN_FROM_WORLD_MAP, CONDITION_ALWAYS, 488, 0, 184, FACING_WEST, 0, -1,
     -1, -1, -1, 0},
    {ALTIN_FROM_COTTAGE_ROOMS_D_SOUTHWEST, CONDITION_ALWAYS, 168, 0, 216,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {ALTIN_FROM_SANCTUM, CONDITION_ALWAYS, 336, 0, 200, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {ALTIN_FROM_COTTAGE_ROOMS_D_NORTHWEST, CONDITION_ALWAYS, 424, 0, 344,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {ALTIN_FROM_COTTAGE_ROOMS_D_NORTH, CONDITION_ALWAYS, 136, 0, 312,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {ALTIN_FROM_COTTAGE_ROOMS_D_NORTHEAST, CONDITION_ALWAYS, 280, 0, 408,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {ALTIN_FROM_COTTAGE_ROOMS_D_SOUTHEAST, CONDITION_ALWAYS, 456, 0, 224,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {ALTIN_FROM_COTTAGE_ROOMS_D_EAST, CONDITION_ALWAYS, 184, 0, 408,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {ALTIN_FROM_COTTAGE_ROOMS_D_WEST, CONDITION_ALWAYS, 168, 0, 440,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {ALTIN_FROM_ALTIN_PEAK_FIRST_WATER_BEAST, CONDITION_ALWAYS, 24, 0, 456,
     FACING_SOUTHEAST, 0, -1, -1, -1, -1, 0},
    {ALTIN_FROM_ALTIN_PEAK_SOUTH_ENTRANCE, CONDITION_ALWAYS, 264, 0, 224,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {ALTIN_FROM_ALTIN_PEAK_JUNCTION, CONDITION_ALWAYS, 328, 0, 264,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {ALTIN_FROM_ALTIN_PEAK_SPRITZ, CONDITION_ALWAYS, 343, 0, 177, FACING_SOUTH,
     0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gCottageRoomsD_Entrances[] = {
    {0, CONDITION_ALWAYS, 200, 0, 200, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {COTTAGE_ROOMS_D_FROM_ALTIN_NORTHWEST, CONDITION_ALWAYS, 159, 0, 258,
     FACING_NORTH, 0, 25, 16, 255, 300, 0},
    {COTTAGE_ROOMS_D_FROM_ALTIN_NORTH, CONDITION_ALWAYS, 336, 0, 213,
     FACING_NORTH, 0, 268, 40, 500, 250, 0},
    {COTTAGE_ROOMS_D_FROM_ALTIN_NORTHEAST, CONDITION_ALWAYS, 688, 0, 204,
     FACING_NORTH, 0, 542, 40, 768, 250, 0},
    {COTTAGE_ROOMS_D_FROM_ALTIN_WEST, CONDITION_ALWAYS, 240, 0, 500,
     FACING_NORTH, 0, 110, 336, 350, 540, 0},
    {COTTAGE_ROOMS_D_FROM_ALTIN_SOUTHEAST, CONDITION_ALWAYS, 496, 0, 504,
     FACING_NORTH, 0, 360, 336, 600, 540, 0},
    {COTTAGE_ROOMS_D_FROM_ALTIN_EAST, CONDITION_ALWAYS, 752, 0, 516,
     FACING_NORTH, 0, 635, 280, 845, 556, 0},
    {COTTAGE_ROOMS_D_FROM_ALTIN_SOUTHWEST, CONDITION_ALWAYS, 80, 0, 864,
     FACING_NORTH, 0, 16, 638, 432, 912, 0},
    {ALTIN_FROM_COTTAGE_ROOMS_D_EAST, CONDITION_ALWAYS, 68, 0, 728,
     FACING_SOUTH, 0, 16, 638, 432, 912, 0},
    {ALTIN_FROM_COTTAGE_ROOMS_D_WEST, CONDITION_ALWAYS, 690, 0, 732,
     FACING_SOUTH, 0, 480, 626, 896, 882, 0},
    {SCENE_TABLE_END},
};

u32 gAltin_Exits[] = {
    SCENE_EXITS(Altin),
    SCENE_EXIT(ALTIN_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_ALTIN),
    SCENE_EXIT(ALTIN_TO_COTTAGE_ROOMS_D_SOUTHWEST, CottageRoomsD,
               COTTAGE_ROOMS_D_FROM_ALTIN_SOUTHWEST),
    SCENE_EXIT(ALTIN_TO_SANCTUM, Sanctum, SANCTUM_FROM_ALTIN),
    SCENE_EXIT(ALTIN_TO_COTTAGE_ROOMS_D_NORTHWEST, CottageRoomsD,
               COTTAGE_ROOMS_D_FROM_ALTIN_NORTHWEST),
    SCENE_EXIT(ALTIN_TO_COTTAGE_ROOMS_D_NORTH, CottageRoomsD,
               COTTAGE_ROOMS_D_FROM_ALTIN_NORTH),
    SCENE_EXIT(ALTIN_TO_COTTAGE_ROOMS_D_NORTHEAST, CottageRoomsD,
               COTTAGE_ROOMS_D_FROM_ALTIN_NORTHEAST),
    SCENE_EXIT(ALTIN_TO_COTTAGE_ROOMS_D_SOUTHEAST, CottageRoomsD,
               COTTAGE_ROOMS_D_FROM_ALTIN_SOUTHEAST),
    SCENE_EXIT(ALTIN_TO_COTTAGE_ROOMS_D_EAST, CottageRoomsD,
               COTTAGE_ROOMS_D_FROM_ALTIN_EAST),
    SCENE_EXIT(ALTIN_TO_COTTAGE_ROOMS_D_WEST, CottageRoomsD,
               COTTAGE_ROOMS_D_FROM_ALTIN_WEST),
    SCENE_EXIT(ALTIN_TO_ALTIN_PEAK_FIRST_WATER_BEAST, AltinPeakFirstWaterBeast,
               ALTIN_PEAK_FIRST_WATER_BEAST_FROM_ALTIN),
    SCENE_EXIT(ALTIN_TO_ALTIN_PEAK_SOUTH_ENTRANCE, AltinPeakSouthEntrance,
               ALTIN_PEAK_SOUTH_ENTRANCE_FROM_ALTIN),
    SCENE_EXIT(ALTIN_TO_ALTIN_PEAK_JUNCTION, AltinPeakJunction,
               ALTIN_PEAK_JUNCTION_FROM_ALTIN),
    SCENE_EXIT(ALTIN_TO_ALTIN_PEAK_SPRITZ, AltinPeakSpritz,
               ALTIN_PEAK_SPRITZ_FROM_ALTIN),
    SCENE_EXITS(CottageRoomsD),
    SCENE_EXIT(COTTAGE_ROOMS_D_TO_ALTIN_NORTHWEST, Altin,
               ALTIN_FROM_COTTAGE_ROOMS_D_NORTHWEST),
    SCENE_EXIT(COTTAGE_ROOMS_D_TO_ALTIN_NORTH, Altin,
               ALTIN_FROM_COTTAGE_ROOMS_D_NORTH),
    SCENE_EXIT(COTTAGE_ROOMS_D_TO_ALTIN_NORTHEAST, Altin,
               ALTIN_FROM_COTTAGE_ROOMS_D_NORTHEAST),
    SCENE_EXIT(COTTAGE_ROOMS_D_TO_ALTIN_WEST, Altin,
               ALTIN_FROM_COTTAGE_ROOMS_D_WEST),
    SCENE_EXIT(COTTAGE_ROOMS_D_TO_ALTIN_SOUTHEAST, Altin,
               ALTIN_FROM_COTTAGE_ROOMS_D_SOUTHEAST),
    SCENE_EXIT(COTTAGE_ROOMS_D_TO_ALTIN_EAST, Altin,
               ALTIN_FROM_COTTAGE_ROOMS_D_EAST),
    SCENE_EXIT(COTTAGE_ROOMS_D_TO_ALTIN_SOUTHWEST, Altin,
               ALTIN_FROM_COTTAGE_ROOMS_D_SOUTHWEST),
    SCENE_EXIT(COTTAGE_ROOMS_D_INN_TO_INN_UPSTAIRS, CottageRoomsD,
               COTTAGE_ROOMS_D_INN_UPSTAIRS_FROM_INN),
    SCENE_EXIT(COTTAGE_ROOMS_D_INN_UPSTAIRS_TO_INN, CottageRoomsD,
               COTTAGE_ROOMS_D_INN_FROM_INN_UPSTAIRS),
    SCENE_EXITS_END,
};

struct ActorPlacement gAltin_NoActorPlacements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gAltin_ActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 440, 0, 200, FACING_SOUTH_SOUTHWEST,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    408, 0, 216, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 310, 0, 217, FACING_SOUTH_SOUTHWEST,
                    TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_WANDER, 246, 0, 186, FACING_SOUTH_SOUTHWEST,
                    TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_WANDER_NEAR, 58, 0, 206,
                    FACING_EAST_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 104, 0, 286, FACING_EAST_SOUTHEAST,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 41,
                    0, 441, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK,
                    0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 176, 0, 296, FACING_SOUTH_SOUTHEAST,
                    TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 136, 0, 461, FACING_EAST_SOUTHEAST,
                    TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_WANDER, 350, 0, 286, FACING_NORTH,
                    TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WATER_BEAST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    60, 0, 474, FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    344, 0, 168, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_IN_GREEN, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 88, 0, 397, FACING_EAST_SOUTHEAST,
                    TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_PONYTAIL, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 422, 0, 208, FACING_EAST_SOUTHEAST,
                    TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_CRATE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
                    452, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CRATE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
                    332, 0, 360, FACING_SOUTH, TALK_FACING_KEEP,
                    ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_CRATE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
                    332, 0, 396, FACING_SOUTH, TALK_FACING_KEEP,
                    ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SCENE_TABLE_END},
};

// clang-format off
struct ActorPlacement gCottageRoomsD_ActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 120, FACING_EAST_SOUTHEAST,
        TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 105, 0, 233, FACING_SOUTH_SOUTHWEST,
        TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 182, FACING_WEST_NORTHWEST, TALK_FACING_PARTY_THEN_WATCH,
        0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 120,
        FACING_WEST_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND, 690, 0, 128, FACING_WEST_SOUTHWEST,
        TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND, 669, 0, 128, FACING_EAST_SOUTHEAST,
        TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND, 604, 0, 145, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_MAN_IN_RED, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 263, 0, 436, FACING_SOUTH_SOUTHWEST,
        TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_PONYTAIL, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND, 520, 0, 454, FACING_SOUTH_SOUTHWEST,
        TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_IN_GREEN, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND, 743, 0, 454, FACING_SOUTH_SOUTHEAST,
        TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_DARK_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND, 103, 0, 744, FACING_SOUTH_SOUTHEAST,
        TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PINK_APRON, CONDITION_IN_VIEW, MOVEMENT_TYPE_WANDER_NEAR, 228, 0, 756, FACING_WEST_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_COOK, CONDITION_IN_VIEW, (s32)gCottageRoomsD_CookScript, 350, 0, 746,
        FACING_WEST_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND, 646, 0, 734, FACING_SOUTH_SOUTHEAST,
        TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct Trigger gAltin_NoTriggers[] = {
    {TRIGGER_TABLE_END},
};

struct Trigger gAltin_Triggers[] = {
    {STEP_TRIGGER_FACING(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT(Altin_WalkThroughDoorway)},
    {STEP_TRIGGER_FACING(FACING_NORTH), 11, CONDITION_ALWAYS, EVENT(Altin_WalkThroughDoorway)},
    {STEP_TRIGGER_FACING(FACING_NORTH), 12, CONDITION_ALWAYS, EVENT(Altin_WalkThroughDoorway)},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, ALTIN_TO_ALTIN_PEAK_SPRITZ},
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, ALTIN_TO_WORLD_MAP},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS,
        EVENT(Altin_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS,
        EVENT(Altin_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS,
        EVENT(Altin_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS,
        EVENT(Altin_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS,
        EVENT(Altin_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS,
        EVENT(Altin_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS,
        EVENT(Altin_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS,
        EVENT(Altin_EnterDoor)},
    {TRIGGER_TALK, ACTOR_GREETING_STRONGMAN, FLAG_HYDROS_STATUES_DEFEATED, EVENT(Altin_AskAboutFirstVisit)},
    {TRIGGER_TALK, ACTOR_GREETING_STRONGMAN, CONDITION_ALWAYS, MSGNO(MsgAltinThankYouSoMuchForDefeating)},
    {TRIGGER_TALK, ACTOR_CURIOUS_STRONGMAN, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinHowCanThoseCreaturesSpitUp)},
    {TRIGGER_TALK, ACTOR_CURIOUS_STRONGMAN, CONDITION_ALWAYS, EVENT(Altin_AskAboutHydros)},
    {TRIGGER_TALK, ACTOR_WOMAN_COATED_HOUSE, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinHowHorribleMyHouseIsTotally)},
    {TRIGGER_TALK, ACTOR_WOMAN_COATED_HOUSE, CONDITION_ALWAYS, MSGNO(MsgAltinOnceTheMineOpensAgainWell)},
    {TRIGGER_TALK, ACTOR_MAN_SUBMERGED_MINE, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinEvenTheMineIsSubmergedNo)},
    {TRIGGER_TALK, ACTOR_MAN_SUBMERGED_MINE, CONDITION_ALWAYS, MSGNO(MsgAltinTheyAlreadyStartedMiningTheUpper)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_DESTROY_MONSTERS, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinIfWeCouldJustDestroyThe)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_DESTROY_MONSTERS, CONDITION_ALWAYS, MSGNO(MsgAltinTheWaterDrainedOutOfThe)},
    {TRIGGER_TALK, ACTOR_WOMAN_WATER_LUCK, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinAltinAlwaysUsedToRunShort)},
    {TRIGGER_TALK, ACTOR_WOMAN_WATER_LUCK, CONDITION_ALWAYS, MSGNO(MsgAltinThankGoodnessTheVillageIsFree)},
    {TRIGGER_TALK, ACTOR_THIRSTY_OLD_WOMAN, CONDITION_ALWAYS, EVENT(Altin_TalkToThirstyOldWoman)},
    {TRIGGER_TALK, ACTOR_MAN_WATERS_SUBSIDED, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinTheWatersAroundHereHaveSubsided)},
    {TRIGGER_TALK, ACTOR_MAN_WATERS_SUBSIDED, CONDITION_ALWAYS, MSGNO(MsgAltinTheVillageWentRightBackTo)},
    {TRIGGER_TALK, ACTOR_STRONGMAN_GUARDIAN_STATUES, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinTheWaterBeastsLookedExactlyLike)},
    {TRIGGER_TALK, ACTOR_STRONGMAN_GUARDIAN_STATUES, CONDITION_ALWAYS, MSGNO(MsgAltinSoThoseHydrosCreaturesReallyWere)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_ROCK_FLUSHED_OUT, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinTheWatersHaveFlushedOutTons)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_ROCK_FLUSHED_OUT, CONDITION_ALWAYS, EVENT(Altin_AskAboutRuins)},
    {TRIGGER_TALK, ACTOR_FLOODED_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT(Altin_TalkToArmorMerchant)},
    {TRIGGER_TALK, ACTOR_FLOODED_ITEM_MERCHANT, CONDITION_ALWAYS, EVENT(Altin_TalkToItemMerchant)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GREETING_STRONGMAN, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinYouCameToAltinAtA)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GREETING_STRONGMAN, CONDITION_ALWAYS,
        MSGNO(MsgAltinWeCanGoBackToWork)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CURIOUS_STRONGMAN, FLAG_HYDROS_STATUES_DEFEATED,
        MSGNO(MsgAltinItsAlmostLikeTheyreTurningThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CURIOUS_STRONGMAN, CONDITION_ALWAYS,
        MSGNO(MsgAltinIWasAfraidMoreWaterWould)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_COATED_HOUSE, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinImGladISavedMyVery)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_COATED_HOUSE, CONDITION_ALWAYS,
        MSGNO(MsgAltinIHopeAltinMineWillThrive)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_SUBMERGED_MINE, FLAG_HYDROS_STATUES_DEFEATED,
        MSGNO(MsgAltinEvenTheToughestOfMinersCant)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_SUBMERGED_MINE, CONDITION_ALWAYS,
        MSGNO(MsgAltinTheWaterSeepedIntoTheMine)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_DESTROY_MONSTERS, FLAG_HYDROS_STATUES_DEFEATED,
        MSGNO(MsgAltinIfTheWaterBeastsContinueTo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_DESTROY_MONSTERS, CONDITION_ALWAYS,
        MSGNO(MsgAltinItsSoHumidInsideThatCave)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_WATER_LUCK, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinWaterIsNiceButNotWhen)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_WATER_LUCK, CONDITION_ALWAYS,
        MSGNO(MsgAltinThatStrangeChamberInTheMines)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_THIRSTY_OLD_WOMAN, FLAG_HYDROS_STATUES_DEFEATED,
        MSGNO(MsgAltinUghTheMonstersSpewedOutAll)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_THIRSTY_OLD_WOMAN, CONDITION_ALWAYS,
        MSGNO(MsgAltinYouFoughtPrettyWellDespiteAll)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_WATERS_SUBSIDED, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinAsTheMonstersGoAwayThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_WATERS_SUBSIDED, CONDITION_ALWAYS,
        MSGNO(MsgAltinThatMineIsFullOfHoles)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRONGMAN_GUARDIAN_STATUES, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinComeToThinkOfItIt)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRONGMAN_GUARDIAN_STATUES, CONDITION_ALWAYS,
        MSGNO(MsgAltinTheOldGuardianStatuesAlwaysCreeped)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ROCK_FLUSHED_OUT, FLAG_HYDROS_STATUES_DEFEATED,
        MSGNO(MsgAltinItllBeBetterThanDiggingUp)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ROCK_FLUSHED_OUT, CONDITION_ALWAYS,
        MSGNO(MsgAltinIUsedToFindAllSorts)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FLOODED_ARMOR_MERCHANT, CONDITION_ALWAYS,
        MSGNO(MsgAltinAllMyArmorInventoryIsRusting)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FLOODED_ITEM_MERCHANT, CONDITION_ALWAYS,
        MSGNO(MsgAltinIfMyGoodsAreWetIll)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STONE_STATUE, CONDITION_ALWAYS, EVENT(Altin_CheckPillarMoved)},
    {TRIGGER_STEP, CELL_BEHAVIOR_ALTIN_NEAR_BEAST, FLAG_ALTIN_WATER_BEAST_LEFT, EVENT(Altin_ScareOffBeast)},
    {TRIGGER_TABLE_END},
};

struct Trigger gCottageRoomsD_Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, COTTAGE_ROOMS_D_TO_ALTIN_NORTHWEST},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, COTTAGE_ROOMS_D_TO_ALTIN_NORTH},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, COTTAGE_ROOMS_D_TO_ALTIN_NORTHEAST},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, COTTAGE_ROOMS_D_TO_ALTIN_WEST},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, COTTAGE_ROOMS_D_TO_ALTIN_SOUTHEAST},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, COTTAGE_ROOMS_D_TO_ALTIN_EAST},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, COTTAGE_ROOMS_D_TO_ALTIN_SOUTHWEST},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, COTTAGE_ROOMS_D_INN_TO_INN_UPSTAIRS},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, COTTAGE_ROOMS_D_INN_UPSTAIRS_TO_INN},
    {TRIGGER_TALK, ACTOR_ROOM_ONE_GUEST, FLAG_ALTIN_WATER_DROPPED_ONCE, MSGNO(MsgAltinNoMatterWhatHappensIdNever)},
    {TRIGGER_TALK, ACTOR_ROOM_ONE_GUEST, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinOhNoTheWholeHouseIs)},
    {TRIGGER_TALK, ACTOR_ROOM_ONE_GUEST, CONDITION_ALWAYS, MSGNO(MsgAltinIHearThatSilkRoadIs)},
    {TRIGGER_TALK, ACTOR_ROOM_ONE_SECOND_GUEST, FLAG_ALTIN_WATER_DROPPED_ONCE, MSGNO(MsgAltinOfCourseOurHouseWillBe)},
    {TRIGGER_TALK, ACTOR_ROOM_ONE_SECOND_GUEST, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinIfTheMonsterThatDidThis)},
    {TRIGGER_TALK, ACTOR_ROOM_ONE_SECOND_GUEST, CONDITION_ALWAYS, EVENT(CottageRoomsD_AskAboutXianGirl)},
    {TRIGGER_TALK, ACTOR_ROOM_TWO_GUEST, FLAG_ALTIN_WATER_DROPPED_TWICE, MSGNO(MsgAltinNotManyPeopleComeUpSilk)},
    {TRIGGER_TALK, ACTOR_ROOM_TWO_GUEST, FLAG_HYDROS_STATUES_DEFEATED, EVENT(CottageRoomsD_AskAboutWayWest)},
    {TRIGGER_TALK, ACTOR_ROOM_TWO_GUEST, CONDITION_ALWAYS, MSGNO(MsgAltinAncientRuinsAndTreasureDefinitelyGo)},
    {TRIGGER_TALK, ACTOR_ROOM_TWO_SECOND_GUEST, FLAG_ALTIN_WATER_DROPPED_TWICE, MSGNO(MsgAltinItsBeenALongTimeSince)},
    {TRIGGER_TALK, ACTOR_ROOM_TWO_SECOND_GUEST, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinDontBeTooHarshOnThose)},
    {TRIGGER_TALK, ACTOR_ROOM_TWO_SECOND_GUEST, CONDITION_ALWAYS, MSGNO(MsgAltinIfOnlyTheyHadLeftA)},
    {TRIGGER_TALK, ACTOR_MAN_BACK_IN_HIS_HOMETOWN, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinIDidntWantToLeaveMy)},
    {TRIGGER_TALK, ACTOR_MAN_BACK_IN_HIS_HOMETOWN, CONDITION_ALWAYS, MSGNO(MsgAltinReallyAncientRuinsAtTheBottom)},
    {TRIGGER_TALK, ACTOR_WOMAN_NO_PLACE_LIKE_HOME, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinIHeardSomeoneKilledTheWater)},
    {TRIGGER_TALK, ACTOR_WOMAN_NO_PLACE_LIKE_HOME, CONDITION_ALWAYS, MSGNO(MsgAltinThereProbablyIsntMuchTreasureIn)},
    {TRIGGER_TALK, ACTOR_KISSING_GIRL, FLAG_HYDROS_STATUES_DEFEATED, EVENT(CottageRoomsD_AskAboutMonsters)},
    {TRIGGER_TALK, ACTOR_KISSING_GIRL, CONDITION_ALWAYS, MSGNO(MsgAltinSomethingsWrongWithMomAndPop)},
    {TRIGGER_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT(CottageRoomsD_TalkToWeaponMerchant)},
    {TRIGGER_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT(Altin_TalkToArmorMerchant)},
    {TRIGGER_TALK, ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, EVENT(Altin_TalkToItemMerchant)},
    {TRIGGER_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, EVENT(CottageRoomsD_TalkToInnkeeper)},
    {TRIGGER_TALK, ACTOR_WOMAN_FORMER_GLORY, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinIdGiveAnythingForSomeoneTo)},
    {TRIGGER_TALK, ACTOR_WOMAN_FORMER_GLORY, CONDITION_ALWAYS, MSGNO(MsgAltinThereIsAnEndlessDesertWest)},
    {TRIGGER_TALK, ACTOR_COOK, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinICameFromXianICook)},
    {TRIGGER_TALK, ACTOR_COOK, CONDITION_ALWAYS, MSGNO(MsgAltinWestOfTheMountainsIsVery)},
    {TRIGGER_TALK, ACTOR_MAN_WAITING_TO_GO_HOME, FLAG_ALTIN_WATER_DROPPED_ONCE, MSGNO(MsgAltinNoneOfUsCanGoHome)},
    {TRIGGER_TALK, ACTOR_MAN_WAITING_TO_GO_HOME, FLAG_ALTIN_WATER_DROPPED_TWICE, MSGNO(MsgAltinTheGuestsInRoomOneMoved)},
    {TRIGGER_TALK, ACTOR_MAN_WAITING_TO_GO_HOME, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinTheGuestsInRoomTwoAlso)},
    {TRIGGER_TALK, ACTOR_MAN_WAITING_TO_GO_HOME, CONDITION_ALWAYS, EVENT(CottageRoomsD_AskAboutMineBeasts)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROOM_ONE_GUEST, FLAG_ALTIN_WATER_DROPPED_ONCE,
        MSGNO(MsgAltinLeavingAltinForTolbiWellThats)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROOM_ONE_GUEST, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinItsGoingToTakeALot)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROOM_ONE_GUEST, CONDITION_ALWAYS,
        MSGNO(MsgAltinTheMerchantsMustBeHavingA)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROOM_ONE_SECOND_GUEST, FLAG_ALTIN_WATER_DROPPED_ONCE, MSGNO(MsgAltinNowIFinallyHaveAnExcuse)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROOM_ONE_SECOND_GUEST, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinOneBlowsNotEnoughJabJab)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROOM_ONE_SECOND_GUEST, CONDITION_ALWAYS,
        MSGNO(MsgAltinIWonderWhatBusinessThatGirl)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROOM_TWO_GUEST, FLAG_ALTIN_WATER_DROPPED_TWICE, MSGNO(MsgAltinOfCourseYouCantGoWest)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROOM_TWO_GUEST, FLAG_HYDROS_STATUES_DEFEATED,
        MSGNO(MsgAltinIThoughtTheVillageWasDone)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROOM_TWO_GUEST, CONDITION_ALWAYS,
        MSGNO(MsgAltinTheySayYouCanStillFind)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROOM_TWO_SECOND_GUEST, FLAG_ALTIN_WATER_DROPPED_TWICE,
        MSGNO(MsgAltinIHateSittingAroundLikeThis)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROOM_TWO_SECOND_GUEST, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinImJustGladToGetMy)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROOM_TWO_SECOND_GUEST, CONDITION_ALWAYS,
        MSGNO(MsgAltinHowCanITrulyBeHappy)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_BACK_IN_HIS_HOMETOWN, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinTheBottomOfTheMineThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_BACK_IN_HIS_HOMETOWN, CONDITION_ALWAYS,
        MSGNO(MsgAltinCripesAncientRuinsDownInThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_NO_PLACE_LIKE_HOME, FLAG_HYDROS_STATUES_DEFEATED,
        MSGNO(MsgAltinMyHusbandsSenseForFindingTreasure)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_NO_PLACE_LIKE_HOME, CONDITION_ALWAYS,
        MSGNO(MsgAltinIHadNoIdeaThereWere)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_KISSING_GIRL, FLAG_HYDROS_STATUES_DEFEATED,
        MSGNO(MsgAltinMomAndPopKnowSomethingAbout)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_KISSING_GIRL, CONDITION_ALWAYS,
        MSGNO(MsgAltinIWonderIfMomAndPop)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT, FLAG_ALTIN_WATER_DRAINED, MSGNO(MsgAltinCripesIsThisHowMyWeapon)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinOnlyAMerchantKnowsTheJoy)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS,
        MSGNO(MsgAltinIveHeardOfAPassageIn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinWeDriedOffAllThatArmor)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS,
        MSGNO(MsgAltinWellIfYouAreImpressedThen)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANT, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinAFewThingsGotDampBut)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS,
        MSGNO(MsgAltinTheGirlFromXianBoughtAll)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, FLAG_HYDROS_STATUES_DEFEATED,
        MSGNO(MsgAltinWellComeThroughThisAllRight)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS,
        MSGNO(MsgAltinIThinkThatTempleWasCalled)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_FORMER_GLORY, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinIKnowTheyDontLookSo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_FORMER_GLORY, CONDITION_ALWAYS,
        MSGNO(MsgAltinYouWontBeAbleToGo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_COOK, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinIBecameACookAfterI)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_COOK, CONDITION_ALWAYS,
        MSGNO(MsgAltinNoOneCanGoWestOf)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_WAITING_TO_GO_HOME, FLAG_ALTIN_WATER_DROPPED_ONCE, MSGNO(MsgAltinMostOfTheVillageIsStill)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_WAITING_TO_GO_HOME, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinItLooksLikeTheWaterHas)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_WAITING_TO_GO_HOME, CONDITION_ALWAYS,
        MSGNO(MsgAltinItWillTakeSomeTimeBefore)},
    {EXAMINE_TRIGGER(EXAMINE_BARREL), 100, FLAG_ALTIN_NUT, FIND_ITEM(ITEM_NUT)},
    {EXAMINE_TRIGGER(EXAMINE_BARREL), 101, FLAG_ALTIN_COINS, FIND_COINS(9)},
    {EXAMINE_TRIGGER(EXAMINE_BED), 102, FLAG_ALTIN_SLEEP_BOMB, FIND_ITEM(ITEM_SLEEP_BOMB)},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 200, CONDITION_ALWAYS,
        FIND_MESSAGE(MSGNO(MsgAltinThePotsAndPansAreRusted))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 201, CONDITION_ALWAYS,
        FIND_MESSAGE(MSGNO(MsgAltinHeIsMakingRoastBeefEven))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 202, CONDITION_ALWAYS,
        FIND_MESSAGE(MSGNO(MsgAltinHeIsMakingShrimpInA))},
    {EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 203, CONDITION_ALWAYS,
        FIND_MESSAGE(MSGNO(MsgAltinThereIsABookTitledAltin))},
    {TRIGGER_TABLE_END},
};
// clang-format on

/// The opening pictures of the village's doors, two cells square. The Sanctum's
/// door (the second) is wider, and Altin_EnterDoor redraws it by hand instead.
u16 gAltin_SanctumDoorCells[] = {
    MAPGRID_ANIMATION_STEP(42, 33, 2, 2, 0),
    MAPGRID_ANIMATION_STEP(42, 35, 2, 2, 5),
    MAPGRID_ANIMATION_STEP(40, 33, 2, 2, 0),
    MAPGRID_ANIMATION_STEP(40, 35, 2, 2, 5),
    MAPGRID_ANIMATION_STEPS_END,
};

u16 gAltin_SouthwestDoorCells[] = {
    MAPGRID_ANIMATION_STEP(35, 48, 2, 2, 5),
    MAPGRID_ANIMATION_STEP(33, 48, 2, 2, 5),
    MAPGRID_ANIMATION_STEPS_END,
};

u16 gAltin_SoutheastDoorCells[] = {
    MAPGRID_ANIMATION_STEP(35, 35, 2, 2, 5),
    MAPGRID_ANIMATION_STEP(33, 35, 2, 2, 5),
    MAPGRID_ANIMATION_STEPS_END,
};

u16 gAltin_EastDoorCells[] = {
    MAPGRID_ANIMATION_STEP(35, 46, 2, 2, 5),
    MAPGRID_ANIMATION_STEP(33, 46, 2, 2, 5),
    MAPGRID_ANIMATION_STEPS_END,
};

u16 gAltin_NorthwestDoorCells[] = {
    MAPGRID_ANIMATION_STEP(35, 50, 2, 2, 5),
    MAPGRID_ANIMATION_STEP(33, 50, 2, 2, 5),
    MAPGRID_ANIMATION_STEPS_END,
};

u16 gAltin_NorthAndWestDoorCells[] = {
    MAPGRID_ANIMATION_STEP(35, 33, 2, 2, 5),
    MAPGRID_ANIMATION_STEP(33, 33, 2, 2, 5),
    MAPGRID_ANIMATION_STEPS_END,
};

u16 gAltin_NortheastDoorCells[] = {
    MAPGRID_ANIMATION_STEP(35, 52, 2, 2, 5),
    MAPGRID_ANIMATION_STEP(33, 52, 2, 2, 5),
    MAPGRID_ANIMATION_STEPS_END,
};

/// Each door step trigger's opening picture and cell, from step trigger 2.
struct MapGridCellAnimation gAltin_Doors[] = {
    {gAltin_SouthwestDoorCells, 41, 5},  {gAltin_SanctumDoorCells, 51, 6},
    {gAltin_NorthwestDoorCells, 57, 17}, {gAltin_NorthAndWestDoorCells, 39, 17},
    {gAltin_NortheastDoorCells, 48, 25}, {gAltin_SoutheastDoorCells, 59, 9},
    {gAltin_EastDoorCells, 42, 23},      {gAltin_NorthAndWestDoorCells, 41, 27},
};
