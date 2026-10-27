/// @file
/// Cottage rooms A (SceneId_CottageRoomsA), a sheet of rooms drawn as cottages:
/// the game packs rooms that share a tile style onto one map, and every room on
/// this one is Vale's. Each room is an entrance with a camera box of its
/// own, named for the door it is entered by, and its door takes the exit of the
/// same number back out. In story order, by the exits that lead in:
///
/// - From the village (SceneId_Vale, and on the stormy night
///   SceneId_ValeStormyNight): the house of Garet's family
///   (COTTAGE_ROOMS_A_FROM_VALE_EAST), his grandfather the mayor, his
///   grandmother and his parents; the house of Jenna's grandparents
///   (COTTAGE_ROOMS_A_FROM_VALE_NORTH); and two more houses
///   (COTTAGE_ROOMS_A_FROM_VALE_SOUTH and COTTAGE_ROOMS_A_FROM_VALE_WEST).
/// - From the part of Vale with the Psynergy Stone (SceneId_ValePlaza): the
/// shops
///   (COTTAGE_ROOMS_A_FROM_VALE_PLAZA_WEST), where the item, weapon and armor
///   shops each open across a counter, and three houses
///   (COTTAGE_ROOMS_A_FROM_VALE_PLAZA_EAST,
///   COTTAGE_ROOMS_A_FROM_VALE_PLAZA_NORTH and
///   COTTAGE_ROOMS_A_FROM_VALE_PLAZA_SOUTH).
/// - Unused: a ninth room drawn at the sheet's lower left, whose camera box no
///   entrance gives, so no door leads in.
///
/// The actor templates and triggers follow the story:
///
/// - The stormy night of the prologue (FLAG_VALE_STORM). Everyone is out:
///   CottageRoomsA_Initialize removes every resident and flashes the
///   lightning outside, and the doors out to the village lead into the
///   storm's scene (SceneId_ValeStormyNight) instead (CottageRoomsA_GetExits).
///   The jars and the barrel are empty, and six more bookcases than by day can
///   be read, a notebook called "Study of Alchemy" among them.
/// - Three years later, the day the story begins. The villagers talk of
///   Kraden's visitors and of Vale's law that keeps Psynergy secret from
///   outsiders, and the jars and the barrel hold coins, an Antidote and a
///   Smoke Bomb, each found once.
/// - After the party has left Vale (FLAG_VALE_PARTY_LEFT), Jenna having been
///   taken on Mt. Aleph. Her grandmother lies sick in bed (Initialize, from
///   COTTAGE_ROOMS_A_FROM_VALE_NORTH), the villagers talk of the quest and
///   of Felix, and the item merchant gives Isaac a Herb as a farewell gift
///   (FLAG_VALE_FAREWELL_HERB_RECEIVED).
/// - Back from Kalay (FLAG_KALAY_REACHED), with Ivan and Mia in the party. The
///   first time the party enters Garet's house, CottageRoomsA_GaretComesHome
///   plays (FLAG_GARET_CAME_HOME): his parents are wondering whether to look
///   for him when his grandmother sees him, the family greets him, Ivan and Mia
///   introduce themselves ("I am Mia, from Imil."), and his father is left
///   pacing. The villagers now worry about Dora, ill since Isaac left.
///
/// While Garet's grandfather dozes in CottageRoomsA_GaretComesHome, three
/// letters Z (ACTOR_FIRST_SLEEP_Z and the two after it) rise from him, one at
/// each of frames 10, 20 and 30 of every 180 (CottageRoomsA_SpawnSleepZ): each
/// starts 24 pixels above him at two fifths of its size and grows a fortieth
/// a frame as it climbs ever more slowly and drifts right ever faster
/// (CottageRoomsA_AdvanceSleepZ).

#include "script_operands.h"
#include "common.h"
#include "unit/import.h"
#include "unit/scene/cottage_rooms_a.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/event_commands.h"
#include "unit/field/field.h"
#include "unit/runtime/runtime.h"
#include "event_layer.h"
#include "scene_tables.h"
#include "call.h"
#include "fxblend.h"
#include "event_layer.h"
#include "event_inline.h"
#include "text/msg_ids.h"
#include "types.h"
#include "ability_ids.h"
#include "scene_tables.h"
#include "item_ids.h"
#include "actor_script.h"
#include "fixed_math.h"
#include "ram_buffer.h"
#include "flags.h"
#include "songs.h"

/// The placed actors, numbered in the order of the actor template
/// tables, which all place them alike;
/// gCottageRoomsA_ActorTemplatesAfterKalay adds Garet, Ivan and Mia,
/// who keep their characters' ids, and the drifting sprites. The villagers'
/// notes quote what they say on the day the story begins.
enum CottageRoomsAActorId {
  /// Garet's grandfather, the mayor, an old man with white hair and a cane
  /// ("your grandpa's the mayor", vale.c). "Isaac, please take care of my
  /// grandson, Garet."
  /// ★ Named from the game: "please take care of my grandson"
  /// (MsgValeIsaacPleaseTakeCareOfMy)
  ACTOR_GARETS_GRANDFATHER = ACTOR_FIRST_PLACED,
  /// Garet's grandmother, an old woman with white hair. "Grandma! What's the
  /// matter? Are you choking?"
  /// ★ Named from the game: "Are you choking" (MsgValeHey2)
  ACTOR_GARETS_GRANDMOTHER,
  /// Garet's father, a man with brown hair in a teal shirt.
  /// "I'm proud of you, Garet. My own son...on such an important quest!"
  /// ★ Named from the game: "My own son" (MsgValeImProudOfYouGaretMy)
  ACTOR_GARETS_FATHER,
  /// Garet's mother, a woman with orange hair under a green headscarf. "As
  /// mothers, Dora and I will always worry about you."
  /// ★ Named from the game: "he is my son" (MsgValeIAmConcernedForGaretBut)
  ACTOR_GARETS_MOTHER,
  /// Jenna's grandfather, a bent old man with a white beard and a cane. "My
  /// granddaughter, Jenna, is finally happy again..."
  /// ★ Named from the game: "My granddaughter,"
  /// (MsgValeMyGranddaughterJennaIsFinallyHappy)
  ACTOR_JENNAS_GRANDFATHER,
  /// Jenna's grandmother, an old woman with white hair. "Please return my
  /// precious granddaughter, Jenna, to me." Once Jenna is taken,
  /// ACTOR_WOMAN_ON_THE_SPITTING_IMAGE says she "grew sick with despair", and
  /// she lies in bed.
  /// ★ Named from the game: "Please return my precious granddaughter"
  /// (MsgValePleaseReturnMyPreciousGranddaughterJenna)
  ACTOR_JENNAS_GRANDMOTHER,
  /// A woman in a green dress in Jenna's grandparents' house. "Jenna is the
  /// spitting image of her mother, whom we lost that day..."
  /// ★ Named from the game: "is the spitting image of her mother"
  /// (MsgValeJennaIsTheSpittingImageOf)
  ACTOR_WOMAN_ON_THE_SPITTING_IMAGE,
  /// A burly grey-haired man: "So Kraden came to Vale to study Mt. Aleph,
  /// right?"
  /// ★ Named from the game: "It's because Kraden was studying Alchemy..."
  /// (MsgValeItsBecauseKradenWasStudyingAlchemy)
  ACTOR_MAN_ON_KRADENS_STUDIES,
  /// A woman in a green dress: "If there are visitors in town, we can't use our
  /// Psynergy."
  /// ★ Named from the game: "If there are visitors in town, we can't use our
  /// Psynergy." (MsgValeIfThereAreVisitorsInTown)
  ACTOR_WOMAN_ON_VISITORS_IN_TOWN,
  /// A young man with brown hair: "Do you ever dream of seeing the world
  /// outside the village?"
  /// ★ Named from the game: "Do you ever dream of seeing the world outside
  /// the village?" (MsgValeDoYouEverDreamOfSeeing)
  ACTOR_YOUNG_MAN_DREAMING_OF_LEAVING,
  /// A woman with fair hair: "Few people outside of Vale possess the power of
  /// Psynergy. That's why we need permission to leave the village..."
  /// ★ Named from the game: "we need permission to leave the village"
  /// (MsgValeFewPeopleOutsideOfValePossess)
  ACTOR_WOMAN_ON_PERMISSION_TO_LEAVE,
  /// A strongman: "I hear old man Kraden's got some visitors. Let's go use our
  /// Psynergy to scare 'em!"
  /// ★ Named from the game: "Let's go use our"
  /// (MsgValeLetsScareKradensVisitors)
  ACTOR_STRONGMAN_SCARING_VISITORS,
  /// ★ Named from what the player sees: a young woman with a brown ponytail
  /// who opens the item shop across its counter, beneath the shelves of jars
  /// (its render)
  ACTOR_ITEM_MERCHANT,
  /// A bald man in a red tunic.
  /// ★ Named from the game: "Good weapons aren't just sharp."
  /// (MsgValeGoodWeaponsDrawOutStrength)
  ACTOR_WEAPON_MERCHANT,
  /// A young man with brown hair in a green shirt.
  /// ★ Named from the game: "Good armor isn't just sturdy."
  /// (MsgValeGoodArmorDrawsOutStrength)
  ACTOR_ARMOR_MERCHANT,
  /// Three letters Z that drift up from Garet's grandfather as he sleeps.
  /// ★ Named from what the player sees: a white letter Z,
  /// spr_280.png (its render)
  ACTOR_FIRST_SLEEP_Z
};

/// Animations of the houses' sprites beyond enum ActorAnimation, as their
/// animation tables and frames under graphics/sprite draw them.
enum CottageRoomsAAnimation {
  /// Garet's grandfather standing with his eyes shut, asleep (spr_117.png).
  ANIM_GRANDFATHER_ASLEEP = 5,
  /// Garet's grandfather glancing quickly from side to side (spr_117.png).
  ANIM_GRANDFATHER_LOOK_ABOUT = 6,
  /// Jenna's grandmother lying in bed, only her head on the pillow
  /// (spr_111.png).
  ANIM_GRANDMOTHER_IN_BED = 5
};

// Forward declarations.
static void CottageRoomsA_GaretComesHome(void);
static void CottageRoomsA_SpawnSleepZ(void);

#include "shared/actor_facing.h"

/// Moves and grows a Z drifting up from Garet's sleeping grandfather.
///
/// Called each frame by gCottageRoomsA_SleepZScript; the actor's
/// timer and work are its x and y steps in signed 8.8 pixels, set by
/// CottageRoomsA_SpawnSleepZ.
static s32 CottageRoomsA_AdvanceSleepZ(struct Actor *interpreter) {
  struct Actor *actor = interpreter;

  actor->x.fixed += (s16)actor->timer * 256;
  actor->y.fixed += (s16)actor->userValue * 256;
  actor->scaleX += Q16_ONE / 40;
  actor->scaleY += Q16_ONE / 40;
  actor->timer += 5;
  actor->userValue = (s16)actor->userValue - 1;
  return 0;
}

const struct SceneEntrance *CottageRoomsA_GetEntrances(void) {
  return gCottageRoomsA_Entrances;
}

const struct SceneExitRegion *CottageRoomsA_GetRegions(void) { return NULL; }

/// On the stormy night, the doors lead out into the storm.
const u32 *CottageRoomsA_GetExits(void) {
  if (GameFlag_Get(FLAG_VALE_STORM) != 0)
    return gCottageRoomsA_StormExits;
  return gCottageRoomsA_Exits;
}

/// Selects the actor templates for the day the story begins, after the
/// party has left Vale (FLAG_VALE_PARTY_LEFT), or after Kalay
/// (FLAG_KALAY_REACHED), and marks which of their actors are in view.
const struct ActorTemplate *CottageRoomsA_GetActorTemplates(void) {
  struct ActorTemplate *table;

  if (GameFlag_Get(FLAG_KALAY_REACHED)) {
    table = gCottageRoomsA_ActorTemplatesAfterKalay;
  } else if (GameFlag_Get(FLAG_VALE_PARTY_LEFT)) {
    table = gCottageRoomsA_ActorTemplatesAfterLeaving;
  } else {
    table = gCottageRoomsA_ActorTemplates;
  }
  Scene_AssignViewFlags(table);
  return table;
}

/// Selects the triggers for the current visit to Vale: the stormy night's
/// (FLAG_VALE_STORM), after Kalay, after the party has left, or the day the
/// story begins.
const struct Trigger *CottageRoomsA_GetTriggers(void) {
  if (GameFlag_Get(FLAG_VALE_STORM) != 0)
    return gCottageRoomsA_StormTriggers;
  if (GameFlag_Get(FLAG_KALAY_REACHED) != 0)
    return gCottageRoomsA_TriggersAfterKalay;
  if (GameFlag_Get(FLAG_VALE_PARTY_LEFT) != 0)
    return gCottageRoomsA_TriggersAfterLeaving;
  return gCottageRoomsA_Triggers;
}

/// A burly grey-haired man asks whether Isaac knew why Kraden came to Vale.
/// He stands in a house of Vale on the day the story begins. Kraden came to
/// study Mt. Aleph.
///
/// Reached by talking to ACTOR_MAN_ON_KRADENS_STUDIES on the day the story
/// begins.
static void CottageRoomsA_AskAboutKradensStudies(void) {
  Event_Start();
  Event_SetMessageId(MsgValeKradenCameToStudyMt);
  Actor_LinkPair(ACTOR_PARTY_LEADER, ACTOR_MAN_ON_KRADENS_STUDIES, 6);
  Event_AskYesNoAndShowResponse(ACTOR_MAN_ON_KRADENS_STUDIES, 0);
  Event_End();
}

/// A strongman asks Isaac to help scare Kraden's visitors with Psynergy. He
/// stands in a house of Vale on the day the story begins.
///
/// Reached by talking to ACTOR_STRONGMAN_SCARING_VISITORS on the day the
/// story begins.
static void CottageRoomsA_AskToScareVisitors(void) {
  Event_Start();
  Event_SetMessageId(MsgValeLetsScareKradensVisitors);
  Actor_LinkPair(ACTOR_PARTY_LEADER, ACTOR_STRONGMAN_SCARING_VISITORS, 6);
  Event_AskYesNoAndShowResponse(ACTOR_STRONGMAN_SCARING_VISITORS, 0);
  Event_End();
}

/// In Vale's shops after the party has left Vale, the item merchant gives
/// Isaac a Herb as a farewell gift, and afterwards urges him to save Jenna.
///
/// Called by CottageRoomsA_TalkToItemMerchant away from the counter, after the
/// party has left and before Kalay. Gives the Herb and sets
/// FLAG_VALE_FAREWELL_HERB_RECEIVED once; afterwards the merchant turns to
/// watch the leader.
static void CottageRoomsA_GiveFarewellHerb(void) {
  s32 gift;

  Event_Start();
  if (Event_GetFlag(FLAG_VALE_FAREWELL_HERB_RECEIVED) != 0) {
    Event_SetMessageId(MsgValeYouMustSaveJenna);
    Event_ShowMessage(ACTOR_ITEM_MERCHANT, 0);
    FieldActor_SetTargetAndCallback(ACTOR_ITEM_MERCHANT,
                                    ACTOR_LINK_KEEP_MOTION | ACTOR_PARTY_LEADER,
                                    gCottageRoomsA_FaceTargetScript);
  } else {
    gift = MsgValeThisIsMyFarewellGift;
    Event_SetMessageId(gift);
    FieldEvent_ShowMessageThenWait(ACTOR_ITEM_MERCHANT, 0, 20);
    FieldMessage_Show((gift + 1), 1);
    PartyInventory_GiveItem(ITEM_HERB, 0);
    GameFlag_Set(FLAG_VALE_FAREWELL_HERB_RECEIVED);
  }
  Event_End();
}

/// A woman in a green dress asks whether the people who caused the eruption
/// had strange powers, in a house of Vale after the party has left.
///
/// Reached by talking to ACTOR_WOMAN_ON_VISITORS_IN_TOWN after the party has
/// left Vale.
static void CottageRoomsA_AskAboutStrangePowers(void) {
  Event_Start();
  Event_SetMessageId(MsgValeTheCulpritsHadStrangePowers);
  Event_AskYesNoAndShowResponse(ACTOR_WOMAN_ON_VISITORS_IN_TOWN, 0);
  Event_End();
}

/// Garet's father asks whether Isaac heard them talking about Dora. He is in
/// Garet's house in Vale, back from Kalay.
///
/// Reached by talking to Garet's father after Kalay.
static void CottageRoomsA_AskAboutDora(void) {
  Event_Start();
  Event_SetMessageId(MsgValeDidYouHearAboutDora);
  Event_AskYesNoAndShowResponse(ACTOR_GARETS_FATHER, 0);
  Event_End();
}

/// Garet's mother hopes the travelers didn't get sick on the way. She is in
/// Garet's house in Vale, back from Kalay.
///
/// Reached by talking to Garet's mother after Kalay.
static void CottageRoomsA_WorryAboutTravelersHealth(void) {
  Event_Start();
  Event_SetMessageId(MsgValeHopeDidntGet);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_GARETS_MOTHER, 0);
  Event_End();
}

/// Leaves a house through its door: the exit sound plays and the screen
/// closes, stopping the lightning first on the stormy night.
static void CottageRoomsA_RequestWindowExit(s32 exitNumber) {
  if (GameFlag_Get(FLAG_VALE_STORM) != 0)
    Weather_StopLightning();
  Sound_PlaySong(SE_MAP_EXIT);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  gFieldWork->transitionFrames = 16;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(exitNumber);
}

// The rooms' doors: stepping on a door's cell, whose metatile behavior is its
// exit's number, takes that exit back out, in every table. On the stormy night
// the exits to SceneId_Vale lead to the storm's scene (SceneId_ValeStormyNight)
// instead.

/// Leaves the house where a young man dreams of leaving. It's the room entered
/// from COTTAGE_ROOMS_A_FROM_VALE_WEST, where
/// ACTOR_YOUNG_MAN_DREAMING_OF_LEAVING wanders.
static void CottageRoomsA_LeaveToValeWest(void) {
  CottageRoomsA_RequestWindowExit(COTTAGE_ROOMS_A_TO_VALE_WEST);
}

/// Leaves Jenna's grandparents' house.
static void CottageRoomsA_LeaveToValeNorth(void) {
  CottageRoomsA_RequestWindowExit(COTTAGE_ROOMS_A_TO_VALE_NORTH);
}

/// Leaves the empty house. It's the room entered from
/// COTTAGE_ROOMS_A_FROM_VALE_PLAZA_NORTH, which no actor template
/// table puts anyone in.
static void CottageRoomsA_LeaveToValePlazaWest(void) {
  CottageRoomsA_RequestWindowExit(COTTAGE_ROOMS_A_TO_VALE_PLAZA_WEST);
}

/// Leaves Vale's shops.
static void CottageRoomsA_LeaveToValePlazaSouth(void) {
  CottageRoomsA_RequestWindowExit(COTTAGE_ROOMS_A_TO_VALE_PLAZA_SOUTH);
}

/// Leaves the house where villagers talk of Kraden and his visitors. It's the
/// room entered from COTTAGE_ROOMS_A_FROM_VALE_SOUTH, where
/// ACTOR_MAN_ON_KRADENS_STUDIES and ACTOR_WOMAN_ON_VISITORS_IN_TOWN stand.
static void CottageRoomsA_LeaveToValeSouth(void) {
  CottageRoomsA_RequestWindowExit(COTTAGE_ROOMS_A_TO_VALE_SOUTH);
}

/// Leaves Garet's house.
static void CottageRoomsA_LeaveToValeEast(void) {
  CottageRoomsA_RequestWindowExit(COTTAGE_ROOMS_A_TO_VALE_EAST);
}

/// Leaves the house where a strongman plans to scare Kraden's visitors. It's
/// the room entered from COTTAGE_ROOMS_A_FROM_VALE_PLAZA_SOUTH, where
/// ACTOR_STRONGMAN_SCARING_VISITORS stands.
static void CottageRoomsA_LeaveToValePlazaEast(void) {
  CottageRoomsA_RequestWindowExit(COTTAGE_ROOMS_A_TO_VALE_PLAZA_EAST);
}

/// Leaves the house where a woman talks of permission to leave. It's the room
/// entered from COTTAGE_ROOMS_A_FROM_VALE_PLAZA_EAST, where
/// ACTOR_WOMAN_ON_PERMISSION_TO_LEAVE stands.
static void CottageRoomsA_LeaveToValePlazaNorth(void) {
  CottageRoomsA_RequestWindowExit(COTTAGE_ROOMS_A_TO_VALE_PLAZA_NORTH);
}

/// Arriving in a house of Vale: on the stormy night the residents are gone and
/// lightning flashes outside; back from Kalay, entering Garet's house the first
/// time plays his homecoming.
///
/// The scene's initialize entry. Runs CottageRoomsA_GaretComesHome from
/// COTTAGE_ROOMS_A_FROM_VALE_EAST after Kalay until FLAG_GARET_CAME_HOME
/// is set. From COTTAGE_ROOMS_A_FROM_VALE_NORTH, once the party has left
/// Vale, Jenna's grandmother lies sick in bed.
s32 CottageRoomsA_Initialize(void) {
  struct Actor *actor;
  struct FieldWork **eventSlot = &gFieldWork;

  (*eventSlot)->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  // On the stormy night every resident is out, and lightning flashes outside.
  if (GameFlag_Get(FLAG_VALE_STORM) != 0) {
    Import_ActorSetPosition(ACTOR_GARETS_GRANDFATHER, 0, 0);
    Import_ActorSetPosition(ACTOR_GARETS_GRANDMOTHER, 0, 0);
    Import_ActorSetPosition(ACTOR_GARETS_FATHER, 0, 0);
    Import_ActorSetPosition(ACTOR_GARETS_MOTHER, 0, 0);
    Import_ActorSetPosition(ACTOR_JENNAS_GRANDFATHER, 0, 0);
    Import_ActorSetPosition(ACTOR_JENNAS_GRANDMOTHER, 0, 0);
    Import_ActorSetPosition(ACTOR_WOMAN_ON_THE_SPITTING_IMAGE, 0, 0);
    Import_ActorSetPosition(ACTOR_MAN_ON_KRADENS_STUDIES, 0, 0);
    Import_ActorSetPosition(ACTOR_WOMAN_ON_VISITORS_IN_TOWN, 0, 0);
    Import_ActorSetPosition(ACTOR_YOUNG_MAN_DREAMING_OF_LEAVING, 0, 0);
    Import_ActorSetPosition(ACTOR_WOMAN_ON_PERMISSION_TO_LEAVE, 0, 0);
    Import_ActorSetPosition(ACTOR_STRONGMAN_SCARING_VISITORS, 0, 0);
    Import_ActorSetPosition(ACTOR_ITEM_MERCHANT, 0, 0);
    Import_ActorSetPosition(ACTOR_WEAPON_MERCHANT, 0, 0);
    Import_ActorSetPosition(ACTOR_ARMOR_MERCHANT, 0, 0);
    Weather_StartLightning();
    (*(struct FieldBlendWork **)((u8 *)eventSlot + Ram_FieldBlendWorkOffset))
        ->loud = 1;
    Weather_LastLightning();
    WaitFrames(30);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Weather_ResumeLightning();
  }
  // The returning party gets the household reunion only on its first visit.
  if (GameFlag_Get(FLAG_KALAY_REACHED) != 0) {
    if (gCommon.entrance == COTTAGE_ROOMS_A_FROM_VALE_EAST) {
      if (GameFlag_Get(FLAG_GARET_CAME_HOME) == 0) {
        CottageRoomsA_GaretComesHome();
      }
    }
    Actor_GetById(ACTOR_GARETS_FATHER)->collisionFlags |= 0x80;
  }
  // Once Jenna is taken, her grandmother lies sick in bed.
  if (gCommon.entrance == COTTAGE_ROOMS_A_FROM_VALE_NORTH) {
    if (GameFlag_Get(FLAG_VALE_PARTY_LEFT) != 0) {
      Import_ActorSetPosition(ACTOR_JENNAS_GRANDMOTHER, PIXELS(454),
                              PIXELS(150));
      actor = Actor_GetById(ACTOR_JENNAS_GRANDMOTHER);
      Actor_SetSpriteFlags(actor, 0);
      Import_ActorSetAnimation(ACTOR_JENNAS_GRANDMOTHER,
                               ANIM_GRANDMOTHER_IN_BED);
      TilesetAnim_Resume(4);
    }
  }
  return 0;
}

/// In Vale's shops, opens the weapon shop across the counter, or the
/// merchant speaks.
///
/// Reached by talking to the weapon merchant in every table but the stormy
/// night's. Facing north, across the counter, opens shop 1; else the
/// merchant's line follows the story.
static void CottageRoomsA_TalkToWeaponMerchant(void) {
  struct Actor *leader;

  leader = Actor_GetById(ACTOR_PARTY_LEADER);

  if (FACING_IS_NORTH(leader->facing)) {
    Shop_Run(1, ACTOR_WEAPON_MERCHANT);
    return;
  }

  Event_Start();
  if (GameFlag_Get(FLAG_KALAY_REACHED) != 0) {
    Event_SetMessageId(MsgValeYouCameBack);
    Event_AskYesNoAndShowResponse(ACTOR_WEAPON_MERCHANT, 0);
  } else {
    if (GameFlag_Get(FLAG_VALE_PARTY_LEFT) != 0) {
      Event_SetMessageId(MsgValeValeFeelsEmpty);
    } else {
      Event_SetMessageId(MsgValeGoodWeaponsDrawOutStrength);
    }
    Event_ShowMessageAndWait(ACTOR_WEAPON_MERCHANT, 0);
  }
  Event_End();
}

/// In Vale's shops, opens the armor shop across the counter, or the merchant
/// speaks.
///
/// Reached by talking to the armor merchant in every table but the stormy
/// night's; opens shop 2 when facing north.
static void CottageRoomsA_TalkToArmorMerchant(void) {
  struct Actor *actor;

  actor = Actor_GetById(ACTOR_PARTY_LEADER);
  if (FACING_IS_NORTH(actor->facing)) {
    Shop_Run(2, ACTOR_ARMOR_MERCHANT);
  } else {
    Event_Start();
    if (GameFlag_Get(FLAG_KALAY_REACHED) != 0) {
      Event_SetMessageId(MsgValeTheRumorWasTrue);
    } else {
      if (GameFlag_Get(FLAG_VALE_PARTY_LEFT) != 0) {
        Event_SetMessageId(MsgValeImNotSadJustGo);
      } else {
        Event_SetMessageId(MsgValeGoodArmorDrawsOutStrength);
      }
    }
    Event_ShowMessageAndWait(ACTOR_ARMOR_MERCHANT, 0);
    Event_End();
  }
}

/// In Vale's shops, opens the item shop across the counter, or the merchant
/// speaks.
///
/// Reached by talking to the item merchant in every table but the stormy
/// night's; opens shop 3 when facing north. After the party has left and
/// before Kalay, CottageRoomsA_GiveFarewellHerb speaks instead.
static void CottageRoomsA_TalkToItemMerchant(void) {
  struct Actor *actor;

  actor = Actor_GetById(ACTOR_PARTY_LEADER);
  if (FACING_IS_NORTH(actor->facing)) {
    Shop_Run(3, ACTOR_ITEM_MERCHANT);
  } else {
    if (GameFlag_Get(FLAG_KALAY_REACHED) != 0) {
      Event_Start();
      Event_SetMessageId(MsgValeWhenDidYouComeBack);
      Event_ShowMessageAndWait(ACTOR_ITEM_MERCHANT, 0);
      Event_End();
    } else {
      if (GameFlag_Get(FLAG_VALE_PARTY_LEFT) != 0) {
        CottageRoomsA_GiveFarewellHerb();
      } else {
        Event_Start();
        Event_SetMessageId(MsgValeAWiseManFleesWhen);
        Event_ShowMessageAndWait(ACTOR_ITEM_MERCHANT, 0);
        Event_End();
      }
    }
  }
}

/// Garet's grandparents and father nod.
static inline void CottageRoomsA_FamilyNods(void) {
  Import_ActorSetAnimation(ACTOR_GARETS_GRANDFATHER, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_GARETS_GRANDMOTHER, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_GARETS_FATHER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_GARETS_GRANDMOTHER, ANIM_NOD);
}

/// Starts the repeated motion before waiting for the requested animation.
static inline void CottageRoomsA_PlayRepeatedAnimationAndWait(s32 actorId,
                                                              s32 animation) {
  Actor_StartRepeatedMotion(actorId, 1);
  Actor_SetAnimationAndWait(actorId, animation);
}

/// In Garet's house in Vale, back from Kalay, Garet comes home: his
/// grandfather dozes, his parents argue over whether to look for him, and his
/// grandmother spots him first. Garet introduces Ivan and Mia, his mother asks
/// them to look after him, and his father tells them to enjoy their stay; the
/// three walk back into Isaac, and the father is left pacing.
///
/// Called by CottageRoomsA_Initialize from
/// COTTAGE_ROOMS_A_FROM_VALE_EAST after Kalay, until FLAG_GARET_CAME_HOME
/// is set; sets it. Adds and removes the task
/// CottageRoomsA_SpawnSleepZ, and leaves the father pacing.
static void CottageRoomsA_GaretComesHome(void) {
  struct Actor *sleepZ;
  struct Actor *actor;
  s32 motionFlags;
  TaskFunc timedUpdate;
  const s32 *partyScript;

  // The party walks in while Garet's grandfather dozes, sleep drifting up
  // from him.
  Event_Start();
  FieldCamera_MoveTo(-1, -1, -1, 0);
  WaitFrames(1);
  Actor_SetSpritePriority(ACTOR_MIA, 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(2) / 5, PIXELS(1) / 5);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(2) / 5, PIXELS(1) / 5);
  Actor_SetSpeed(ACTOR_IVAN, PIXELS(2) / 5, PIXELS(1) / 5);
  Actor_SetSpeed(ACTOR_MIA, PIXELS(2) / 5, PIXELS(1) / 5);
  Import_ActorSetAnimation(ACTOR_GARETS_GRANDFATHER, ANIM_GRANDFATHER_ASLEEP);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 808, 508);
  sleepZ = Actor_Get(ACTOR_FIRST_SLEEP_Z);
  Actor_SetSpriteFlags(sleepZ, 0);
  sleepZ = Actor_Get(ACTOR_FIRST_SLEEP_Z + 1);
  Actor_SetSpriteFlags(sleepZ, 0);
  sleepZ = Actor_Get(ACTOR_FIRST_SLEEP_Z + 2);
  Actor_SetSpriteFlags(sleepZ, 0);
  motionFlags = 0;
  Actor_GetById(ACTOR_FIRST_SLEEP_Z)->motionFlags = motionFlags;
  Actor_GetById(ACTOR_FIRST_SLEEP_Z + 1)->motionFlags = motionFlags;
  Actor_GetById(ACTOR_FIRST_SLEEP_Z + 2)->motionFlags = motionFlags;
  timedUpdate = CottageRoomsA_SpawnSleepZ;
  Task_AddFrame(timedUpdate, TASK_PASS_FRAME);
  WaitFrames(1);
  gFieldWork->transitionFrames = 32;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetPosition(ACTOR_GARET, actor->x.fixed, actor->z.fixed);
  }
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetPosition(ACTOR_IVAN, actor->x.fixed, actor->z.fixed);
  }
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetPosition(ACTOR_MIA, actor->x.fixed, actor->z.fixed);
  }
  Actor_WalkTo(ACTOR_GARET, 792, 512);
  Actor_WalkTo(ACTOR_IVAN, 824, 504);
  Actor_WalkToAndWait(ACTOR_MIA, 818, 524);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_STAND);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_STAND);
  Event_WaitFrames(10);
  FieldActor_SetTargetAndCallback(ACTOR_PARTY_LEADER,
                                  ACTOR_LINK_KEEP_MOTION | ACTOR_GARETS_FATHER,
                                  gCottageRoomsA_FaceTargetScript);
  FieldActor_SetTargetAndCallback(ACTOR_GARET,
                                  ACTOR_LINK_KEEP_MOTION | ACTOR_GARETS_FATHER,
                                  gCottageRoomsA_FaceTargetScript);
  FieldActor_SetTargetAndCallback(ACTOR_IVAN,
                                  ACTOR_LINK_KEEP_MOTION | ACTOR_GARETS_FATHER,
                                  gCottageRoomsA_FaceTargetScript);
  FieldActor_SetTargetAndCallback(ACTOR_MIA,
                                  ACTOR_LINK_KEEP_MOTION | ACTOR_GARETS_FATHER,
                                  gCottageRoomsA_FaceTargetScript);
  Event_WaitFrames(300);
  FieldCamera_GetActor()->motionFlags = motionFlags;
  // The camera pans slowly up to Garet's parents. His father, pacing, asks
  // whether they should look for Garet; Isaac still does not know about
  // Dora. His mother teases that he misses Garet.
  FieldCamera_SetSpeed(PIXELS(1) / 10, PIXELS(1) / 80);
  FieldCamera_MoveTo(PIXELS(786), 0, PIXELS(430), 1);
  Event_WaitFrames(240);
  Actor_Stop(ACTOR_GARETS_FATHER);
  Actor_ShowEmote(ACTOR_GARETS_FATHER, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  Actor_WalkToAndWait(ACTOR_GARETS_FATHER, 819, 405);
  Event_WaitFrames(40);
  Actor_SetAnimationAndWait(ACTOR_GARETS_FATHER, ANIM_SHAKE_HEAD);
  Event_WaitFrames(40);
  Actor_FaceDirection(ACTOR_GARETS_FATHER, FACING_NORTH + FACING_STEP, 20);
  Event_SetMessageId(MsgValeHey2);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT |
                                     ACTOR_GARETS_FATHER,
                                 0, 20);
  Actor_Stop(ACTOR_PARTY_LEADER);
  Actor_Stop(ACTOR_GARET);
  Actor_Stop(ACTOR_IVAN);
  Actor_Stop(ACTOR_MIA);
  Actor_ShowEmote(ACTOR_GARETS_MOTHER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_GARETS_MOTHER,
                                 0, 20);
  Actor_RunRepeatedMotion(ACTOR_GARETS_FATHER, 2);
  Event_WaitFrames(40);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT |
                                     ACTOR_GARETS_FATHER,
                                 0, 10);
  Actor_FaceDirection(ACTOR_GARETS_MOTHER, FACING_SOUTH + FACING_STEP, 10);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_GARETS_MOTHER,
                                 0, 40);
  Actor_RunRepeatedMotion(ACTOR_GARETS_FATHER, 2);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT |
                                     ACTOR_GARETS_FATHER,
                                 0, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  Actor_ShowEmote(ACTOR_GARETS_MOTHER, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 40);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_GARETS_MOTHER,
                                 0, 40);
  Actor_StartRepeatedMotion(ACTOR_GARETS_FATHER, 2);
  Actor_ShowEmote(ACTOR_GARETS_FATHER, EMOTE_IN_FRONT | EMOTE_WINCE, 20);
  Import_ActorSetAnimation(ACTOR_GARETS_FATHER, ANIM_SHAKE_HEAD);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT |
                                     ACTOR_GARETS_FATHER,
                                 0, 10);
  CottageRoomsA_PlayRepeatedAnimationAndWait(ACTOR_GARETS_MOTHER, ANIM_NOD);
  Event_WaitFrames(20);
  CottageRoomsA_PlayRepeatedAnimationAndWait(ACTOR_GARETS_FATHER,
                                             ANIM_SHAKE_HEAD);
  CottageRoomsA_PlayRepeatedAnimationAndWait(ACTOR_GARETS_MOTHER, ANIM_NOD);
  CottageRoomsA_PlayRepeatedAnimationAndWait(ACTOR_GARETS_FATHER,
                                             ANIM_SHAKE_HEAD);
  // Garet's grandmother sees him and chokes on her words; his mother asks if
  // she is choking, his father thinks she misses Garet too.
  Actor_ShowEmote(ACTOR_GARETS_GRANDMOTHER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_RunRepeatedMotion(ACTOR_GARETS_GRANDMOTHER, 1);
  Event_WaitFrames(20);
  Actor_FaceDirection(ACTOR_GARETS_GRANDMOTHER, FACING_EAST + FACING_STEP, 40);
  Actor_RunRepeatedMotion(ACTOR_GARETS_GRANDMOTHER, 2);
  Event_WaitFrames(60);
  Actor_RunRepeatedMotion(ACTOR_GARETS_GRANDMOTHER, 3);
  Event_WaitFrames(40);
  FieldEvent_ShowMessageThenWait(
      SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_GRANDMOTHER, 0, 40);
  Import_ActorSetAnimation(ACTOR_GARETS_MOTHER, ANIM_STILL);
  Actor_RunRepeatedMotion(ACTOR_GARETS_MOTHER, 2);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_GARETS_MOTHER,
                                 0, 10);
  Actor_SetAnimationAndWait(ACTOR_GARETS_GRANDMOTHER, ANIM_SHAKE_HEAD);
  Actor_RunRepeatedMotion(ACTOR_GARETS_GRANDMOTHER, 2);
  FieldEvent_ShowMessageThenWait(
      SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_GRANDMOTHER, 0, 10);
  Actor_ShowEmote(ACTOR_GARETS_FATHER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Actor_FaceDirection(ACTOR_GARETS_FATHER, FACING_SOUTH + FACING_STEP, 40);
  Actor_SetAnimationAndWait(ACTOR_GARETS_FATHER, ANIM_NOD);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_FATHER, 0,
                                 10);
  // No, it really is Garet: both parents turn and call his name together.
  Actor_SetAnimationAndWait(ACTOR_GARETS_GRANDMOTHER, ANIM_SHAKE_HEAD);
  Actor_FaceDirection(ACTOR_GARETS_GRANDMOTHER, FACING_NORTH + FACING_STEP, 10);
  Actor_Jump(ACTOR_GARETS_GRANDMOTHER, 2, 0);
  Import_ActorSetAnimation(ACTOR_GARETS_GRANDMOTHER, ANIM_SHAKE_HEAD);
  FieldEvent_ShowMessageThenWait(
      SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_GRANDMOTHER, 0, 10);
  Actor_ShowEmote(ACTOR_GARETS_MOTHER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_GARETS_FATHER, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_FaceDirection(ACTOR_GARETS_FATHER, FACING_NORTH + FACING_STEP, 80);
  Actor_FaceDirection(ACTOR_GARETS_FATHER, FACING_SOUTH + FACING_STEP, 60);
  Actor_StartRepeatedMotion(ACTOR_GARETS_FATHER, 2);
  Actor_StartRepeatedMotion(ACTOR_GARETS_MOTHER, 2);
  FieldEvent_ShowTwoMessagesAndWait(ACTOR_GARETS_FATHER, 11, 6, 6, 6,
                                    ACTOR_GARETS_MOTHER, 12, 1, 7, 1, 0);
  Event_WaitFrames(20);
  FieldCamera_SetSpeed(PIXELS(8) / 5, PIXELS(1) / 5);
  FieldCamera_MoveTo(PIXELS(777), 0, PIXELS(468), 1);
  FieldCamera_WaitForArrival();
  Event_WaitFrames(40);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_GARET, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 2);
  // His grandfather wakes, and the sleep stops drifting.
  Task_Remove(timedUpdate);
  Event_WaitFrames(40);
  Actor_SetAnimationAndWait(ACTOR_GARETS_GRANDFATHER,
                            ANIM_GRANDFATHER_LOOK_ABOUT);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(
      SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_GRANDFATHER, 0, 20);
  // The grandfather looks about, then jumps up with Garet's name.
  FieldCamera_MoveTo(PIXELS(750), 0, PIXELS(451), 1);
  Event_WaitFrames(20);
  Actor_FaceDirection(ACTOR_GARETS_MOTHER, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_GARETS_FATHER, FACING_SOUTH + FACING_STEP, 10);
  Actor_FaceDirection(ACTOR_GARETS_GRANDFATHER, FACING_EAST + FACING_STEP, 40);
  Actor_ShowEmote(ACTOR_GARETS_GRANDFATHER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                  40);
  Actor_FaceDirection(ACTOR_GARETS_GRANDFATHER, FACING_SOUTHEAST + FACING_STEP,
                      20);
  Actor_FaceDirection(ACTOR_GARETS_GRANDFATHER, FACING_EAST + FACING_STEP, 20);
  Actor_FaceDirection(ACTOR_GARETS_GRANDFATHER, FACING_SOUTHEAST + FACING_STEP,
                      40);
  Actor_SetAnimationAndWait(ACTOR_GARETS_GRANDFATHER,
                            ANIM_GRANDFATHER_LOOK_ABOUT);
  Event_WaitFrames(60);
  Actor_Jump(ACTOR_GARETS_GRANDFATHER, 6, 0);
  FieldEvent_ShowMessageThenWait(
      SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_GRANDFATHER, 0, 20);
  // Garet goes to him; the grandfather asks whether the Wise One's mission
  // is done, and a no skips one line of Garet's father.
  Actor_SetSpeed(ACTOR_GARET, PIXELS(8) / 5, PIXELS(4) / 5);
  Actor_WalkToAndWait(ACTOR_GARET, 789, 473);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST + FACING_STEP, 20);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0, 10);
  Actor_FaceDirection(ACTOR_GARETS_GRANDFATHER, FACING_EAST + FACING_STEP, 20);
  Actor_SetAnimationAndWait(ACTOR_GARETS_GRANDFATHER, ANIM_NOD);
  Event_OpenMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_GRANDFATHER, 0);
  Actor_FaceDirection(ACTOR_GARETS_FATHER, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_GARETS_GRANDMOTHER, FACING_EAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHWEST + FACING_STEP, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 1) {
    gFieldWork->message++;
  }
  FieldCamera_MoveTo(PIXELS(777), 0, PIXELS(428), 1);
  Event_WaitFrames(20);
  Actor_RunRepeatedMotion(ACTOR_GARETS_FATHER, 2);
  Event_ShowMessage(ACTOR_GARETS_FATHER, 0);
  Actor_SetAnimationAndWait(ACTOR_GARETS_MOTHER, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  // His mother knows he is leaving again soon; his grandparents are glad to
  // see him well.
  Event_SetMessageId(MsgValeYoureLeavingAgainSoon);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_GARETS_MOTHER, 0);
  FieldCamera_MoveTo(PIXELS(777), 0, PIXELS(468), 1);
  Event_WaitFrames(20);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH + FACING_STEP, 20);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_WaitFrames(20);
  Actor_SetAnimationAndWait(ACTOR_GARETS_GRANDMOTHER, ANIM_SHAKE_HEAD);
  Actor_FaceDirection(ACTOR_GARETS_GRANDMOTHER, FACING_NORTH + FACING_STEP, 10);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_GRANDMOTHER, 0);
  Actor_SetAnimationAndWait(ACTOR_GARETS_GRANDFATHER, ANIM_NOD);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_GRANDFATHER, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST + FACING_STEP, 10);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Actor_FaceDirection(ACTOR_GARETS_GRANDMOTHER, FACING_EAST + FACING_STEP, 10);
  Import_ActorSetAnimation(ACTOR_GARETS_MOTHER, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_GARETS_FATHER, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_GARETS_GRANDMOTHER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_GARETS_GRANDFATHER, ANIM_NOD);
  Event_WaitFrames(20);
  // Garet introduces his new friends.
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST + FACING_STEP, 20);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST + FACING_STEP, 20);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0, 20);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST + FACING_STEP, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 40);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_WaitFrames(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 20);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_WaitFrames(20);
  Actor_FaceDirection(ACTOR_GARET, FACING_EAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
  // Ivan, then Mia, step forward and introduce themselves; the family nods.
  Actor_SetSpeed(ACTOR_IVAN, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(ACTOR_IVAN, 819, 489);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST + FACING_STEP, 40);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  FieldEvent_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  CottageRoomsA_FamilyNods();
  Actor_GetById(ACTOR_MIA)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  Actor_SetSpritePriority(ACTOR_MIA, 1);
  Actor_SetSpeed(ACTOR_MIA, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(ACTOR_MIA, 794, 520);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_WalkToAndWait(ACTOR_MIA, 784, 496);
  Actor_FaceDirection(ACTOR_MIA, FACING_WEST + FACING_STEP, 10);
  Actor_GetById(ACTOR_MIA)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  FieldEvent_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  CottageRoomsA_FamilyNods();
  Event_WaitFrames(20);
  FieldCamera_MoveTo(PIXELS(777), 0, PIXELS(428), 1);
  Event_WaitFrames(20);
  // Garet's mother comes forward and asks them to look after young Garet.
  Actor_SetSpeed(ACTOR_GARETS_MOTHER, PIXELS(2) / 5, PIXELS(1) / 5);
  Actor_WalkToAndWait(ACTOR_GARETS_MOTHER, 835, 388);
  Actor_FaceDirection(ACTOR_GARETS_MOTHER, FACING_SOUTH + FACING_STEP, 0);
  Actor_ShowEmote(ACTOR_GARETS_MOTHER, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_GARETS_MOTHER,
                                 0, 20);
  FieldCamera_MoveTo(PIXELS(777), 0, PIXELS(468), 1);
  Event_WaitFrames(40);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHEAST + FACING_STEP, 40);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH + FACING_STEP, 20);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_WaitFrames(20);
  // His father: enough chit-chat, relax and enjoy your stay.
  Actor_RunRepeatedMotion(ACTOR_GARETS_FATHER, 1);
  Event_WaitFrames(20);
  Import_ActorSetAnimation(ACTOR_GARETS_FATHER, ANIM_NOD);
  FieldEvent_ShowMessageThenWait(ACTOR_GARETS_FATHER, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH + FACING_STEP, 40);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_WaitFrames(20);
  Actor_SetSpeed(ACTOR_IVAN, PIXELS(1), PIXELS(1) / 2);
  partyScript = gCottageRoomsA_PartyLeavesScript;
  Actor_SetScript(ACTOR_GARET, partyScript);
  Actor_SetScript(ACTOR_IVAN, partyScript);
  Actor_RunScriptAndWait(ACTOR_MIA, partyScript);
  Actor_SetScript(ACTOR_GARETS_FATHER, gCottageRoomsA_FatherPacingScript);
  Actor_WalkToAndWait(ACTOR_GARETS_MOTHER, 837, 376);
  Actor_FaceDirection(ACTOR_GARETS_MOTHER, FACING_NORTH + FACING_STEP, 20);
  GameFlag_Set(FLAG_GARET_CAME_HOME);
  Event_End();
}

/// Sends a sprite drifting up from Garet's sleeping grandfather, one of the
/// three at frames 10, 20 and 30 of every 180.
///
/// A task CottageRoomsA_GaretComesHome adds while the grandfather dozes.
static void CottageRoomsA_SpawnSleepZ(void) {
  s32 actorId;
  u32 cycleFrame;
  struct Actor *actor;
  struct Actor *origin;

  cycleFrame = gFrameCount % 180;
  actorId = ACTOR_FIRST_SLEEP_Z;
  switch (cycleFrame) {
  case 10:
    break;
  case 20:
    actorId = ACTOR_FIRST_SLEEP_Z + 1;
    break;
  case 30:
    actorId = ACTOR_FIRST_SLEEP_Z + 2;
    break;
  default:
    return;
  }
  actor = Actor_GetById(actorId);
  if (actor == NULL) {
    return;
  }
  origin = Actor_GetById(ACTOR_GARETS_GRANDFATHER);
  if (origin != NULL) {
    Import_ActorSetPosition(actorId, origin->x.fixed, origin->z.fixed);
  }
  // Restart this sprite above the origin with a small scale and 8.8 motion.
  actor->scaleX = 2 * Q16_ONE / 5;
  actor->scaleY = 2 * Q16_ONE / 5;
  {
    s32 height = actor->y.fixed + PIXELS(24);

    actor->y.fixed = height;
    actor->targetY = height;
    // The drifting script consumes these two work fields as signed 8.8 steps.
    actor->timer = 25;      // 25/256 pixel per frame.
    actor->userValue = 128; // 1/2 pixel per frame.
  }
  Actor_SetScript(actorId, gCottageRoomsA_SleepZScript);
}

s32 gCottageRoomsA_FaceTargetScript[] = {
    SC_CALL(Scene_TurnTowardTarget),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gCottageRoomsA_FatherPacingScript[] = {
    SC_SET(SW_SPEED, PIXELS(3) / 5),
    SC_SET(SW_ACCELERATION, PIXELS(3) / 10),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(784), 0, PIXELS(405)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(40),
    SC_LABEL(1),
    SC_ADD(SW_FACING, -1024),
    SC_WAIT(1),
    SC_LOOP(16, SC_TO(1)),
    SC_WAIT(40),
    SC_LABEL(3),
    SC_ADD(SW_FACING, -1024),
    SC_WAIT(1),
    SC_LOOP(16, SC_TO(3)),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(819), 0, PIXELS(405)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_LABEL(2),
    SC_ADD(SW_FACING, -2048),
    SC_WAIT(1),
    SC_LOOP(16, SC_TO(2)),
    SC_WAIT(40),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gCottageRoomsA_PartyLeavesScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(808), 0, PIXELS(508)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

s32 gCottageRoomsA_SleepZScript[] = {
    SC_CALL(CottageRoomsA_AdvanceSleepZ),
    SC_WAIT(1),
    SC_LOOP(60, SC_TOP),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

struct SceneEntrance gCottageRoomsA_Entrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 120, 0, 100, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {COTTAGE_ROOMS_A_FROM_VALE_WEST, CONDITION_ALWAYS, 96, 0, 222, FACING_NORTH,
     0, 4, 4, 244, 292, 0},
    {COTTAGE_ROOMS_A_FROM_VALE_NORTH, CONDITION_ALWAYS, 337, 0, 199,
     FACING_NORTH, 0, 252, 4, 548, 236, 0},
    {COTTAGE_ROOMS_A_FROM_VALE_PLAZA_NORTH, CONDITION_ALWAYS, 656, 0, 244,
     FACING_NORTH, 0, 556, 4, 900, 276, 0},
    {COTTAGE_ROOMS_A_FROM_VALE_PLAZA_WEST, CONDITION_ALWAYS, 159, 0, 525,
     FACING_NORTH, 0, 4, 324, 332, 564, 0},
    {COTTAGE_ROOMS_A_FROM_VALE_SOUTH, CONDITION_ALWAYS, 528, 0, 486,
     FACING_NORTH, 0, 348, 284, 612, 516, 0},
    {COTTAGE_ROOMS_A_FROM_VALE_EAST, CONDITION_ALWAYS, 816, 0, 522,
     FACING_NORTH, 0, 628, 284, 884, 548, 0},
    {COTTAGE_ROOMS_A_FROM_VALE_PLAZA_SOUTH, CONDITION_ALWAYS, 463, 0, 851,
     FACING_NORTH, 0, 324, 604, 588, 884, 0},
    {COTTAGE_ROOMS_A_FROM_VALE_PLAZA_EAST, CONDITION_ALWAYS, 769, 0, 857,
     FACING_NORTH, 0, 580, 628, 852, 884, 0},
    {SCENE_TABLE_END},
};

u32 gCottageRoomsA_Exits[] = {
    SCENE_EXITS(CottageRoomsA),
    SCENE_EXIT(COTTAGE_ROOMS_A_TO_VALE_WEST, Vale,
               VALE_FROM_COTTAGE_ROOMS_A_WEST),
    SCENE_EXIT(COTTAGE_ROOMS_A_TO_VALE_NORTH, Vale,
               VALE_FROM_COTTAGE_ROOMS_A_NORTH),
    SCENE_EXIT(COTTAGE_ROOMS_A_TO_VALE_PLAZA_WEST, ValePlaza,
               VALE_PLAZA_FROM_COTTAGE_ROOMS_A_WEST),
    SCENE_EXIT(COTTAGE_ROOMS_A_TO_VALE_PLAZA_SOUTH, ValePlaza,
               VALE_PLAZA_FROM_COTTAGE_ROOMS_A_SOUTH),
    SCENE_EXIT(COTTAGE_ROOMS_A_TO_VALE_SOUTH, Vale,
               VALE_FROM_COTTAGE_ROOMS_A_SOUTH),
    SCENE_EXIT(COTTAGE_ROOMS_A_TO_VALE_EAST, Vale,
               VALE_FROM_COTTAGE_ROOMS_A_EAST),
    SCENE_EXIT(COTTAGE_ROOMS_A_TO_VALE_PLAZA_EAST, ValePlaza,
               VALE_PLAZA_FROM_COTTAGE_ROOMS_A_EAST),
    SCENE_EXIT(COTTAGE_ROOMS_A_TO_VALE_PLAZA_NORTH, ValePlaza,
               VALE_PLAZA_FROM_COTTAGE_ROOMS_A_NORTH),
    SCENE_EXITS_END,
};

u32 gCottageRoomsA_StormExits[] = {
    SCENE_EXITS(CottageRoomsA),
    SCENE_EXIT(COTTAGE_ROOMS_A_TO_VALE_WEST, ValeStormyNight,
               VALE_STORMY_NIGHT_FROM_COTTAGE_ROOMS_A_WEST),
    SCENE_EXIT(COTTAGE_ROOMS_A_TO_VALE_NORTH, ValeStormyNight,
               VALE_STORMY_NIGHT_FROM_COTTAGE_ROOMS_A_NORTH),
    SCENE_EXIT(COTTAGE_ROOMS_A_TO_VALE_PLAZA_WEST, ValePlaza,
               VALE_PLAZA_FROM_COTTAGE_ROOMS_A_WEST),
    SCENE_EXIT(COTTAGE_ROOMS_A_TO_VALE_PLAZA_SOUTH, ValePlaza,
               VALE_PLAZA_FROM_COTTAGE_ROOMS_A_SOUTH),
    SCENE_EXIT(COTTAGE_ROOMS_A_TO_VALE_SOUTH, ValeStormyNight,
               VALE_STORMY_NIGHT_FROM_COTTAGE_ROOMS_A_SOUTH),
    SCENE_EXIT(COTTAGE_ROOMS_A_TO_VALE_EAST, ValeStormyNight,
               VALE_STORMY_NIGHT_FROM_COTTAGE_ROOMS_A_EAST),
    SCENE_EXIT(COTTAGE_ROOMS_A_TO_VALE_PLAZA_EAST, ValePlaza,
               VALE_PLAZA_FROM_COTTAGE_ROOMS_A_EAST),
    SCENE_EXIT(COTTAGE_ROOMS_A_TO_VALE_PLAZA_NORTH, ValePlaza,
               VALE_PLAZA_FROM_COTTAGE_ROOMS_A_NORTH),
    SCENE_EXITS_END,
};

// clang-format off
struct ActorTemplate gCottageRoomsA_ActorTemplates[] = {
    { SPRITE_OLD_MAN_WITH_WHITE_HAIR, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(697), 0, PIXELS(468), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_OLD_WOMAN, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(726), 0, PIXELS(486), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_MAN_IN_TEAL_SHIRT, 0x0, MOVEMENT_TYPE_WANDER,
        PIXELS(815), 0, PIXELS(386), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_IN_GREEN_HEADSCARF, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(841), 0, PIXELS(455), FACING_NORTH, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(403), 0, PIXELS(144), FACING_WEST, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_WOMAN, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(331), 0, PIXELS(144), FACING_EAST, TALK_FACING_KEEP, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(486), 0, PIXELS(136), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(495), 0, PIXELS(453), FACING_NORTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(405), 0, PIXELS(421), FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, 0x0, MOVEMENT_TYPE_WANDER,
        PIXELS(99), 0, PIXELS(125), FACING_NORTH, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(768), 0, PIXELS(768), FACING_WEST, TALK_FACING_PARTY, 0 },
    { SPRITE_STRONGMAN, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(392), 0, PIXELS(697), FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_WITH_PONYTAIL, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(72), 0, PIXELS(424), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_BALD_MAN_IN_RED, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(232), 0, PIXELS(440), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_YOUNG_MAN_IN_GREEN, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(264), 0, PIXELS(440), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ActorTemplate gCottageRoomsA_ActorTemplatesAfterLeaving[] = {
    { SPRITE_OLD_MAN_WITH_WHITE_HAIR, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(697), 0, PIXELS(468), FACING_SOUTHEAST + FACING_STEP, 3, 0 },
    { SPRITE_OLD_WOMAN, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(726), 0, PIXELS(486), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_MAN_IN_TEAL_SHIRT, 0x0, MOVEMENT_TYPE_WANDER,
        PIXELS(815), 0, PIXELS(386), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_IN_GREEN_HEADSCARF, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(841), 0, PIXELS(455), FACING_NORTH, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(403), 0, PIXELS(144), FACING_WEST, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_WOMAN, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(456), 0, PIXELS(156), FACING_EAST, TALK_FACING_KEEP, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(486), 0, PIXELS(118), FACING_NORTH, TALK_FACING_PARTY, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(495), 0, PIXELS(453), FACING_NORTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(405), 0, PIXELS(421), FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, 0x0, MOVEMENT_TYPE_WANDER,
        PIXELS(99), 0, PIXELS(125), FACING_NORTH, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(768), 0, PIXELS(768), FACING_WEST, TALK_FACING_PARTY, 0 },
    { SPRITE_STRONGMAN, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(392), 0, PIXELS(697), FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_WITH_PONYTAIL, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(72), 0, PIXELS(424), FACING_SOUTH, 3, 0 },
    { SPRITE_BALD_MAN_IN_RED, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(232), 0, PIXELS(440), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_YOUNG_MAN_IN_GREEN, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(264), 0, PIXELS(440), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ActorTemplate gCottageRoomsA_ActorTemplatesAfterKalay[] = {
    { ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0 },
    { ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0 },
    { ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_WHITE_HAIR, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(697), 0, PIXELS(468), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_WOMAN, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(726), 0, PIXELS(486), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_IN_TEAL_SHIRT, 0x0, (s32)gCottageRoomsA_FatherPacingScript,
        PIXELS(819), 0, PIXELS(405), FACING_WEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_IN_GREEN_HEADSCARF, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(841), 0, PIXELS(370), FACING_NORTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(403), 0, PIXELS(144), FACING_WEST, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_WOMAN, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(456), 0, PIXELS(156), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(494), 0, PIXELS(112), FACING_NORTH, TALK_FACING_PARTY, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(495), 0, PIXELS(453), FACING_NORTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(405), 0, PIXELS(421), FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, 0x0, MOVEMENT_TYPE_WANDER,
        PIXELS(99), 0, PIXELS(125), FACING_NORTH, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(768), 0, PIXELS(768), FACING_WEST, TALK_FACING_PARTY, 0 },
    { SPRITE_STRONGMAN, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(392), 0, PIXELS(697), FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_WITH_PONYTAIL, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(72), 0, PIXELS(424), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_BALD_MAN_IN_RED, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(232), 0, PIXELS(440), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_YOUNG_MAN_IN_GREEN, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(264), 0, PIXELS(440), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { 280, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { 280, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { 280, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SCENE_TABLE_END },
};

struct Trigger gCottageRoomsA_StormTriggers[] = {
    { TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(CottageRoomsA_LeaveToValeWest) },
    { TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(CottageRoomsA_LeaveToValeNorth) },
    { TRIGGER_STEP, 3, CONDITION_ALWAYS, EVENT(CottageRoomsA_LeaveToValePlazaWest) },
    { TRIGGER_STEP, 4, CONDITION_ALWAYS, EVENT(CottageRoomsA_LeaveToValePlazaSouth) },
    { TRIGGER_STEP, 5, CONDITION_ALWAYS, EVENT(CottageRoomsA_LeaveToValeSouth) },
    { TRIGGER_STEP, 6, CONDITION_ALWAYS, EVENT(CottageRoomsA_LeaveToValeEast) },
    { TRIGGER_STEP, 7, CONDITION_ALWAYS, EVENT(CottageRoomsA_LeaveToValePlazaEast) },
    { TRIGGER_STEP, 8, CONDITION_ALWAYS, EVENT(CottageRoomsA_LeaveToValePlazaNorth) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBooksAboutContinents)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookLawsOfVale)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE) | TRIGGER_FLAG_FACING | FACING_NORTH, 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookPowerOfNature)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE) | TRIGGER_FLAG_FACING | FACING_NORTH, 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookPsynergyStone)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE) | TRIGGER_FLAG_FACING | FACING_NORTH, 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeAuntNotebook)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE) | TRIGGER_FLAG_FACING | FACING_NORTH, 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookMakingGreatVillage)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 206, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeNotebookStudyOfAlchemy)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 207, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookAncientLemurians)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 208, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookFountainOfTolbi)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 209, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeEncyclopediaOfAlchemy)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 210, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookLegendOfAleph)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 211, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookBeingAdept)) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), 100, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgVaultButDidntFindAnything)) },
    { EXAMINE_TRIGGER(EXAMINE_JAR), 101, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeJarEmpty)) },
    { EXAMINE_TRIGGER(EXAMINE_JAR), 102, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeJarEmpty)) },
    { EXAMINE_TRIGGER(EXAMINE_JAR), 103, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeJarEmpty)) },
    { EXAMINE_TRIGGER(EXAMINE_JAR), 104, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeJarEmpty)) },
    { TRIGGERS_END },
};

struct Trigger gCottageRoomsA_Triggers[] = {
    { TRIGGER_TALK, ACTOR_GARETS_GRANDFATHER, CONDITION_ALWAYS, MSGNO(MsgValeAtTheVeryLeastVisitorsShould) },
    { TRIGGER_TALK, ACTOR_GARETS_GRANDMOTHER, CONDITION_ALWAYS, MSGNO(MsgValeIHearKradenHasGuestsAgain) },
    { TRIGGER_TALK, ACTOR_GARETS_FATHER, CONDITION_ALWAYS, MSGNO(MsgValeGaretHasGrownQuiteStrongIn) },
    { TRIGGER_TALK, ACTOR_GARETS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgValeYouAndGaretAreOlderNow) },
    { TRIGGER_TALK, ACTOR_JENNAS_GRANDFATHER, CONDITION_ALWAYS, MSGNO(MsgValeMyGranddaughterJennaIsFinallyHappy) },
    { TRIGGER_TALK, ACTOR_JENNAS_GRANDMOTHER, CONDITION_ALWAYS, MSGNO(MsgValeIHopeYouTwoWontBe) },
    { TRIGGER_TALK, ACTOR_WOMAN_ON_THE_SPITTING_IMAGE, CONDITION_ALWAYS, MSGNO(MsgValeJennaIsTheSpittingImageOf) },
    { TRIGGER_TALK, ACTOR_MAN_ON_KRADENS_STUDIES, CONDITION_ALWAYS, EVENT(CottageRoomsA_AskAboutKradensStudies) },
    { TRIGGER_TALK, ACTOR_WOMAN_ON_VISITORS_IN_TOWN, CONDITION_ALWAYS, MSGNO(MsgValeIfThereAreVisitorsInTown) },
    { TRIGGER_TALK, ACTOR_YOUNG_MAN_DREAMING_OF_LEAVING, CONDITION_ALWAYS, MSGNO(MsgValeDoYouEverDreamOfSeeing) },
    { TRIGGER_TALK, ACTOR_WOMAN_ON_PERMISSION_TO_LEAVE, CONDITION_ALWAYS, MSGNO(MsgValeFewPeopleOutsideOfValePossess) },
    { TRIGGER_TALK, ACTOR_STRONGMAN_SCARING_VISITORS, CONDITION_ALWAYS, EVENT(CottageRoomsA_AskToScareVisitors) },
    { TRIGGER_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT(CottageRoomsA_TalkToWeaponMerchant) },
    { TRIGGER_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT(CottageRoomsA_TalkToArmorMerchant) },
    { TRIGGER_TALK, ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, EVENT(CottageRoomsA_TalkToItemMerchant) },
    { TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(CottageRoomsA_LeaveToValeWest) },
    { TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(CottageRoomsA_LeaveToValeNorth) },
    { TRIGGER_STEP, 3, CONDITION_ALWAYS, EVENT(CottageRoomsA_LeaveToValePlazaWest) },
    { TRIGGER_STEP, 4, CONDITION_ALWAYS, EVENT(CottageRoomsA_LeaveToValePlazaSouth) },
    { TRIGGER_STEP, 5, CONDITION_ALWAYS, EVENT(CottageRoomsA_LeaveToValeSouth) },
    { TRIGGER_STEP, 6, CONDITION_ALWAYS, EVENT(CottageRoomsA_LeaveToValeEast) },
    { TRIGGER_STEP, 7, CONDITION_ALWAYS, EVENT(CottageRoomsA_LeaveToValePlazaEast) },
    { TRIGGER_STEP, 8, CONDITION_ALWAYS, EVENT(CottageRoomsA_LeaveToValePlazaNorth) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBooksAboutContinents)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookLawsOfVale)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE) | TRIGGER_FLAG_FACING | FACING_NORTH, 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookPowerOfNature)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE) | TRIGGER_FLAG_FACING | FACING_NORTH, 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookPsynergyStone)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE) | TRIGGER_FLAG_FACING | FACING_NORTH, 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeAuntNotebook)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE) | TRIGGER_FLAG_FACING | FACING_NORTH, 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookMakingGreatVillage)) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), 100, FLAG_COTTAGE_ROOMS_A_BARREL_ANTIDOTE, FIND_ITEM(ITEM_ANTIDOTE) },
    { EXAMINE_TRIGGER(EXAMINE_JAR), 101, FLAG_COTTAGE_ROOMS_A_JAR_6_COINS, FIND_COINS(6) },
    { EXAMINE_TRIGGER(EXAMINE_JAR), 102, FLAG_COTTAGE_ROOMS_A_JAR_2_COINS, FIND_COINS(2) },
    { EXAMINE_TRIGGER(EXAMINE_JAR), 103, FLAG_COTTAGE_ROOMS_A_JAR_SMOKE_BOMB, FIND_ITEM(ITEM_SMOKE_BOMB) },
    { EXAMINE_TRIGGER(EXAMINE_JAR), 104, FLAG_COTTAGE_ROOMS_A_JAR_3_COINS, FIND_COINS(3) },
    { TRIGGERS_END },
};

struct Trigger gCottageRoomsA_TriggersAfterLeaving[] = {
    { TRIGGER_TALK, ACTOR_GARETS_GRANDFATHER, CONDITION_ALWAYS, MSGNO(MsgValeIsaacPleaseTakeCareOfMy) },
    { TRIGGER_TALK, ACTOR_GARETS_GRANDMOTHER, CONDITION_ALWAYS, MSGNO(MsgValeReceivingAQuestDirectlyFromThe) },
    { TRIGGER_TALK, ACTOR_GARETS_FATHER, CONDITION_ALWAYS, MSGNO(MsgValeImProudOfYouGaretMy) },
    { TRIGGER_TALK, ACTOR_GARETS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgValeAsMothersDoraAndIWill) },
    { TRIGGER_TALK, ACTOR_JENNAS_GRANDFATHER, CONDITION_ALWAYS, MSGNO(MsgValeIHeardThatJennaBrotherFelix) },
    { TRIGGER_TALK, ACTOR_JENNAS_GRANDMOTHER, CONDITION_ALWAYS, MSGNO(MsgValePleaseReturnMyPreciousGranddaughterJenna) },
    { TRIGGER_TALK, ACTOR_WOMAN_ON_THE_SPITTING_IMAGE, CONDITION_ALWAYS, MSGNO(MsgValeAfterJennaWasTakenGrandmotherGrew) },
    { TRIGGER_TALK, ACTOR_MAN_ON_KRADENS_STUDIES, CONDITION_ALWAYS, MSGNO(MsgValeItsBecauseKradenWasStudyingAlchemy) },
    { TRIGGER_TALK, ACTOR_WOMAN_ON_VISITORS_IN_TOWN, CONDITION_ALWAYS, EVENT(CottageRoomsA_AskAboutStrangePowers) },
    { TRIGGER_TALK, ACTOR_YOUNG_MAN_DREAMING_OF_LEAVING, CONDITION_ALWAYS, MSGNO(MsgValeImSoJealousOfYouBeing) },
    { TRIGGER_TALK, ACTOR_WOMAN_ON_PERMISSION_TO_LEAVE, CONDITION_ALWAYS, MSGNO(MsgValeOnceYouLeaveTownTakeCare) },
    { TRIGGER_TALK, ACTOR_STRONGMAN_SCARING_VISITORS, CONDITION_ALWAYS, MSGNO(MsgValeIHadNoIdeaThatSuch) },
    { TRIGGER_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT(CottageRoomsA_TalkToWeaponMerchant) },
    { TRIGGER_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT(CottageRoomsA_TalkToArmorMerchant) },
    { TRIGGER_TALK, ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, EVENT(CottageRoomsA_TalkToItemMerchant) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgValeIsaacYoureGoingOnADifficult) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgValeIfYouFaceAnyHardshipIn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgValeSoIfThatWasFelixWho) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GARETS_GRANDFATHER, CONDITION_ALWAYS, MSGNO(MsgValeItsHardLettingMyPreciousGaret) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GARETS_GRANDMOTHER, CONDITION_ALWAYS, MSGNO(MsgValeTheWiseOneChoseGaretHimself) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GARETS_FATHER, CONDITION_ALWAYS, MSGNO(MsgValeOnceGaretLeavesIWontBe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GARETS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgValeIAmConcernedForGaretBut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_JENNAS_GRANDFATHER, CONDITION_ALWAYS, MSGNO(MsgValeFelixInheritedHisFathersSkillWith) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_JENNAS_GRANDMOTHER, CONDITION_ALWAYS, MSGNO(MsgValeJennaAnytimeYouFaceDangerJust) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_SPITTING_IMAGE, CONDITION_ALWAYS, MSGNO(MsgValeWhyWouldFelixTakeHisSister) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_KRADENS_STUDIES, CONDITION_ALWAYS, MSGNO(MsgValeWeAlmostLostTheTownOver) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_VISITORS_IN_TOWN, CONDITION_ALWAYS, MSGNO(MsgValeIdLikeToBelieveThatOnly) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_DREAMING_OF_LEAVING, CONDITION_ALWAYS, MSGNO(MsgValeIfOnlyIdGoneToSol) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_PERMISSION_TO_LEAVE, CONDITION_ALWAYS, MSGNO(MsgValeValesLawsStateThatYouCannot) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRONGMAN_SCARING_VISITORS, CONDITION_ALWAYS, MSGNO(MsgValeIWishIWereTheOne) },
    { TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(CottageRoomsA_LeaveToValeWest) },
    { TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(CottageRoomsA_LeaveToValeNorth) },
    { TRIGGER_STEP, 3, CONDITION_ALWAYS, EVENT(CottageRoomsA_LeaveToValePlazaWest) },
    { TRIGGER_STEP, 4, CONDITION_ALWAYS, EVENT(CottageRoomsA_LeaveToValePlazaSouth) },
    { TRIGGER_STEP, 5, CONDITION_ALWAYS, EVENT(CottageRoomsA_LeaveToValeSouth) },
    { TRIGGER_STEP, 6, CONDITION_ALWAYS, EVENT(CottageRoomsA_LeaveToValeEast) },
    { TRIGGER_STEP, 7, CONDITION_ALWAYS, EVENT(CottageRoomsA_LeaveToValePlazaEast) },
    { TRIGGER_STEP, 8, CONDITION_ALWAYS, EVENT(CottageRoomsA_LeaveToValePlazaNorth) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), 100, FLAG_COTTAGE_ROOMS_A_BARREL_ANTIDOTE, FIND_ITEM(ITEM_ANTIDOTE) },
    { EXAMINE_TRIGGER(EXAMINE_JAR), 101, FLAG_COTTAGE_ROOMS_A_JAR_6_COINS, FIND_COINS(6) },
    { EXAMINE_TRIGGER(EXAMINE_JAR), 102, FLAG_COTTAGE_ROOMS_A_JAR_2_COINS, FIND_COINS(2) },
    { EXAMINE_TRIGGER(EXAMINE_JAR), 103, FLAG_COTTAGE_ROOMS_A_JAR_SMOKE_BOMB, FIND_ITEM(ITEM_SMOKE_BOMB) },
    { EXAMINE_TRIGGER(EXAMINE_JAR), 104, FLAG_COTTAGE_ROOMS_A_JAR_3_COINS, FIND_COINS(3) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBooksAboutContinents)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookLawsOfVale)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE) | TRIGGER_FLAG_FACING | FACING_NORTH, 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookPowerOfNature)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE) | TRIGGER_FLAG_FACING | FACING_NORTH, 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookPsynergyStone)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE) | TRIGGER_FLAG_FACING | FACING_NORTH, 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeAuntNotebook)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE) | TRIGGER_FLAG_FACING | FACING_NORTH, 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookMakingGreatVillage)) },
    { TRIGGERS_END },
};

struct Trigger gCottageRoomsA_TriggersAfterKalay[] = {
    { TRIGGER_TALK, ACTOR_GARETS_GRANDFATHER, CONDITION_ALWAYS, MSGNO(MsgValeMiaIsSuchANiceGirl) },
    { TRIGGER_TALK, ACTOR_GARETS_GRANDMOTHER, CONDITION_ALWAYS, MSGNO(MsgValeThatIvanSeemsToBeA) },
    { TRIGGER_TALK, ACTOR_GARETS_FATHER, CONDITION_ALWAYS, EVENT(CottageRoomsA_AskAboutDora) },
    { TRIGGER_TALK, ACTOR_GARETS_MOTHER, CONDITION_ALWAYS, EVENT(CottageRoomsA_WorryAboutTravelersHealth) },
    { TRIGGER_TALK, ACTOR_JENNAS_GRANDFATHER, CONDITION_ALWAYS, MSGNO(MsgValeOhSoYouStillHaventRescued) },
    { TRIGGER_TALK, ACTOR_JENNAS_GRANDMOTHER, CONDITION_ALWAYS, MSGNO(MsgValeIGetOverColdsQuicklyBut) },
    { TRIGGER_TALK, ACTOR_WOMAN_ON_THE_SPITTING_IMAGE, CONDITION_ALWAYS, MSGNO(MsgValeAColdIsGoingAroundI) },
    { TRIGGER_TALK, ACTOR_MAN_ON_KRADENS_STUDIES, CONDITION_ALWAYS, MSGNO(MsgValeWhatWasThatAlchemyThatKraden) },
    { TRIGGER_TALK, ACTOR_WOMAN_ON_VISITORS_IN_TOWN, CONDITION_ALWAYS, MSGNO(MsgValeEarthquakesStillStrikeOnceInA) },
    { TRIGGER_TALK, ACTOR_YOUNG_MAN_DREAMING_OF_LEAVING, CONDITION_ALWAYS, MSGNO(MsgValeTheMountainIsApparentlyStillCasting) },
    { TRIGGER_TALK, ACTOR_WOMAN_ON_PERMISSION_TO_LEAVE, CONDITION_ALWAYS, MSGNO(MsgValeTheNorthwestFaceOfMtAleph) },
    { TRIGGER_TALK, ACTOR_STRONGMAN_SCARING_VISITORS, CONDITION_ALWAYS, MSGNO(MsgValeTheGreatHealersAreTooAfraid) },
    { TRIGGER_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT(CottageRoomsA_TalkToWeaponMerchant) },
    { TRIGGER_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT(CottageRoomsA_TalkToArmorMerchant) },
    { TRIGGER_TALK, ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, EVENT(CottageRoomsA_TalkToItemMerchant) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GARETS_GRANDFATHER, CONDITION_ALWAYS, MSGNO(MsgValeItMustBeNiceToBe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GARETS_GRANDMOTHER, CONDITION_ALWAYS, MSGNO(MsgValeHisNewFriendsAreAllSo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GARETS_FATHER, CONDITION_ALWAYS, MSGNO(MsgValeItWasntTooSmartToTalk) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GARETS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgValeGaretSeemsToHaveGrownUp) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgValeWeveSeenManyMonstersLatelyAnd) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgValeIWishICouldLearnThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANT, FLAG_VALE_FAREWELL_HERB_RECEIVED, MSGNO(MsgValeWhatHappenedToJennaHaventYou) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgValeIGaveHimAFarewellGift) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_JENNAS_GRANDFATHER, CONDITION_ALWAYS, MSGNO(MsgValeWillIEverSeeJennaAnd) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_JENNAS_GRANDMOTHER, CONDITION_ALWAYS, MSGNO(MsgValeDoraSaysItsJustACold) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_SPITTING_IMAGE, CONDITION_ALWAYS, MSGNO(MsgValeIKnowThatThereAreAlways) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_KRADENS_STUDIES, CONDITION_ALWAYS, MSGNO(MsgValeEvenIfTheEruptionHadNo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_VISITORS_IN_TOWN, CONDITION_ALWAYS, MSGNO(MsgValeEveryTimeTheGroundShakesI) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_DREAMING_OF_LEAVING, CONDITION_ALWAYS, MSGNO(MsgValeIfThePsynergyStonesInThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_PERMISSION_TO_LEAVE, CONDITION_ALWAYS, MSGNO(MsgValeTheySayTheLavaLookedLike) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRONGMAN_SCARING_VISITORS, CONDITION_ALWAYS, MSGNO(MsgValeTheHealersAreSupposedToGuard) },
    { TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(CottageRoomsA_LeaveToValeWest) },
    { TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(CottageRoomsA_LeaveToValeNorth) },
    { TRIGGER_STEP, 3, CONDITION_ALWAYS, EVENT(CottageRoomsA_LeaveToValePlazaWest) },
    { TRIGGER_STEP, 4, CONDITION_ALWAYS, EVENT(CottageRoomsA_LeaveToValePlazaSouth) },
    { TRIGGER_STEP, 5, CONDITION_ALWAYS, EVENT(CottageRoomsA_LeaveToValeSouth) },
    { TRIGGER_STEP, 6, CONDITION_ALWAYS, EVENT(CottageRoomsA_LeaveToValeEast) },
    { TRIGGER_STEP, 7, CONDITION_ALWAYS, EVENT(CottageRoomsA_LeaveToValePlazaEast) },
    { TRIGGER_STEP, 8, CONDITION_ALWAYS, EVENT(CottageRoomsA_LeaveToValePlazaNorth) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), 100, FLAG_COTTAGE_ROOMS_A_BARREL_ANTIDOTE, FIND_ITEM(ITEM_ANTIDOTE) },
    { EXAMINE_TRIGGER(EXAMINE_JAR), 101, FLAG_COTTAGE_ROOMS_A_JAR_6_COINS, FIND_COINS(6) },
    { EXAMINE_TRIGGER(EXAMINE_JAR), 102, FLAG_COTTAGE_ROOMS_A_JAR_2_COINS, FIND_COINS(2) },
    { EXAMINE_TRIGGER(EXAMINE_JAR), 103, FLAG_COTTAGE_ROOMS_A_JAR_SMOKE_BOMB, FIND_ITEM(ITEM_SMOKE_BOMB) },
    { EXAMINE_TRIGGER(EXAMINE_JAR), 104, FLAG_COTTAGE_ROOMS_A_JAR_3_COINS, FIND_COINS(3) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBooksAboutContinents)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookLawsOfVale)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE) | TRIGGER_FLAG_FACING | FACING_NORTH, 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookPowerOfNature)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE) | TRIGGER_FLAG_FACING | FACING_NORTH, 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookPsynergyStone)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE) | TRIGGER_FLAG_FACING | FACING_NORTH, 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeAuntNotebook)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE) | TRIGGER_FLAG_FACING | FACING_NORTH, 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookMakingGreatVillage)) },
    { TRIGGERS_END },
};
// clang-format on
