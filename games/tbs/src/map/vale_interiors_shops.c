/// @file
/// Vale (SceneId_ValeInteriorsShops): the insides of the village's houses and
/// shops, all on one map. Each room is an entrance with a camera box of its
/// own, named for the door it is entered by
/// (VALE_INTERIORS_SHOPS_FROM_VALE_WEST and the rest), and its door takes the
/// exit of the same number back out: four open onto the village (SceneId_Vale),
/// and four onto the part of Vale with the Psynergy Stone (SceneId_ValeGate),
/// as the exits' names say. Garet's family lives in the room entered from
/// VALE_INTERIORS_SHOPS_FROM_VALE_EAST: his grandfather the mayor, his
/// grandmother and his parents. Jenna's grandparents live in the room entered
/// from VALE_INTERIORS_SHOPS_FROM_VALE_NORTH, and the one entered from
/// VALE_INTERIORS_SHOPS_FROM_VALE_GATE_WEST holds the item, weapon and armor
/// shops, each opened across its counter. The placements and events follow the
/// story:
///
/// - The stormy night of the prologue (FLAG_VALE_STORM). Everyone is out:
///   ValeInteriorsShops_Initialize removes every resident and flashes the
///   lightning outside, and the doors out to the village lead into the
///   storm's map (SceneId_ValeStorm) instead (ValeInteriorsShops_GetExits). The
///   jars and the barrel are empty, and six more bookcases than by day can be
///   read, a notebook called "Study of Alchemy" among them.
/// - Three years later, the day the story begins. The villagers talk of
///   Kraden's visitors and of Vale's law that keeps Psynergy secret from
///   outsiders, and the jars and the barrel hold coins, an Antidote and a
///   Smoke Bomb, each found once.
/// - After the party has left Vale (FLAG_VALE_PARTY_LEFT), Jenna having been
///   taken on Mt. Aleph. Her grandmother lies sick in bed (Initialize, from
///   VALE_INTERIORS_SHOPS_FROM_VALE_NORTH), the villagers talk of the quest and
///   of Felix, and the item merchant gives Isaac a Herb as a farewell gift
///   (FLAG_VALE_FAREWELL_HERB_RECEIVED).
/// - Back from Kalay (FLAG_KALAY_REACHED), with Ivan and Mia in the party. The
///   first time the party enters Garet's house, Event_GaretComesHome plays
///   (FLAG_GARET_CAME_HOME): his parents are wondering whether to look for
///   him when his grandmother sees him, the family greets him, Ivan and Mia
///   introduce themselves ("I am Mia, from Imil."), and his father is left
///   pacing. The villagers now worry about Dora, ill since Isaac left.
///
/// While Garet's grandfather dozes in Event_GaretComesHome, three letters Z
/// (ACTOR_FIRST_SLEEP_Z and the two after it) rise from him, one at each of
/// frames 10, 20 and 30 of every 180 (ValeInteriorsShops_SpawnSleepZ): each
/// starts 24 pixels above him at two fifths of its size and grows a fortieth
/// a frame as it climbs ever more slowly and drifts right ever faster
/// (ValeInteriorsShops_AdvanceSleepZ)
/// (https://alchemy.pascalpixel.com/places/vale-interiors-shops/sleep-z.gif:
/// two cycles, the screen with the grandfather and the three letters boxed and
/// the cycle's frame written, beside the sprites alone).
///
/// Open questions: none.

#include "action_script.h"
#include "game_state.h"
#include "unit/import.h"
#include "unit/map/vale_interiors_shops.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "unit/runtime/runtime.h"
#include "facing_object.h"
#include "script_layer.h"
#include "map_tables.h"
#include "call.h"
#include "fxblend.h"
#include "script_layer.h"
#include "field_service.h"
#include "text/msg_ids.h"
#include "types.h"
#include "ability_ids.h"
#include "map_tables.h"
#include "item_ids.h"
#include "objcmd.h"
#include "facing_object.h"
#include "fixed_math.h"
#include "ram_buffer.h"
#include "flags.h"
#include "songs.h"

/// The placed actors, numbered in the order of the placement tables, which
/// all place them alike; gValeInteriorsShops_PlacementsAfterKalay adds Garet,
/// Ivan and Mia, who keep their characters' ids, and the drifting sprites. The
/// villagers' notes quote what they say on the day the story begins.
enum ValeInteriorsShopsActor {
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
enum ValeInteriorsShopsAnimation {
  /// Garet's grandfather standing with his eyes shut, asleep (spr_117.png).
  ANIMATION_GRANDFATHER_ASLEEP = 5,
  /// Garet's grandfather glancing quickly from side to side (spr_117.png).
  ANIMATION_GRANDFATHER_LOOK_ABOUT = 6,
  /// Jenna's grandmother lying in bed, only her head on the pillow
  /// (spr_111.png).
  ANIMATION_GRANDMOTHER_IN_BED = 5
};

// Forward declarations.
static void Event_GaretComesHome(void);
static void ValeInteriorsShops_SpawnSleepZ(void);

#include "common/object_facing.h"

/// Moves and grows a Z drifting up from Garet's sleeping grandfather.
///
/// Called each frame by gValeInteriorsShops_SleepZScript; the actor's
/// timer and work are its x and y steps in signed 8.8 pixels, set by
/// ValeInteriorsShops_SpawnSleepZ.
static s32
ValeInteriorsShops_AdvanceSleepZ(struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;

  actor->x.fixed += (s16)actor->timer * 256;
  actor->y.fixed += (s16)actor->work * 256;
  actor->scaleX += Q16_ONE / 40;
  actor->scaleY += Q16_ONE / 40;
  actor->timer += 5;
  actor->work = (s16)actor->work - 1;
  return 0;
}

const struct SceneEntrance *ValeInteriorsShops_GetEntrances(void) {
  return gValeInteriorsShops_Entrances;
}

const struct SceneRegion *ValeInteriorsShops_GetRegions(void) { return NULL; }

/// On the stormy night, the doors lead out into the storm.
const u32 *ValeInteriorsShops_GetExits(void) {
  if (Engine_GameFlagIsSet(FLAG_VALE_STORM) != 0)
    return gValeInteriorsShops_StormExits;
  return gValeInteriorsShops_Exits;
}

/// Selects the placements for the day the story begins, after the party has
/// left Vale (FLAG_VALE_PARTY_LEFT), or after Kalay (FLAG_KALAY_REACHED), and
/// marks which of their actors are in view.
const struct ScenePlacement *ValeInteriorsShops_GetPlacements(void) {
  struct ScenePlacement *table;

  if (Engine_GameFlagIsSet(FLAG_KALAY_REACHED)) {
    table = gValeInteriorsShops_PlacementsAfterKalay;
  } else if (Engine_GameFlagIsSet(FLAG_VALE_PARTY_LEFT)) {
    table = gValeInteriorsShops_PlacementsAfterLeaving;
  } else {
    table = gValeInteriorsShops_Placements;
  }
  SceneEvents_UpdateInView(table);
  return table;
}

/// Selects the events for the current visit to Vale: the stormy night's
/// (FLAG_VALE_STORM), after Kalay, after the party has left, or the day the
/// story begins.
const struct SceneEvent *ValeInteriorsShops_GetEvents(void) {
  if (Engine_GameFlagIsSet(FLAG_VALE_STORM) != 0)
    return gValeInteriorsShops_StormEvents;
  if (Engine_GameFlagIsSet(FLAG_KALAY_REACHED) != 0)
    return gValeInteriorsShops_EventsAfterKalay;
  if (Engine_GameFlagIsSet(FLAG_VALE_PARTY_LEFT) != 0)
    return gValeInteriorsShops_EventsAfterLeaving;
  return gValeInteriorsShops_Events;
}

/// A burly grey-haired man asks whether Isaac knew why Kraden came to Vale.
/// He stands in a house of Vale on the day the story begins. Kraden came to
/// study Mt. Aleph.
///
/// Reached by talking to ACTOR_MAN_ON_KRADENS_STUDIES on the day the story
/// begins.
static void Event_AskAboutKradensStudies(void) {
  Engine_EventStart();
  Engine_EventSetMessage(MsgValeKradenCameToStudyMt);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_MAN_ON_KRADENS_STUDIES,
                            6);
  Engine_EventAskYesNo(ACTOR_MAN_ON_KRADENS_STUDIES, 0);
  Engine_EventEnd();
}

/// A strongman asks Isaac to help scare Kraden's visitors with Psynergy. He
/// stands in a house of Vale on the day the story begins.
///
/// Reached by talking to ACTOR_STRONGMAN_SCARING_VISITORS on the day the story
/// begins.
static void Event_AskToScareVisitors(void) {
  Engine_EventStart();
  Engine_EventSetMessage(MsgValeLetsScareKradensVisitors);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER,
                            ACTOR_STRONGMAN_SCARING_VISITORS, 6);
  Engine_EventAskYesNo(ACTOR_STRONGMAN_SCARING_VISITORS, 0);
  Engine_EventEnd();
}

/// In Vale's shops after the party has left Vale, the item merchant gives
/// Isaac a Herb as a farewell gift, and afterwards urges him to save Jenna.
///
/// Called by Event_TalkToItemMerchant away from the counter, after the party
/// has left and before Kalay. Gives the Herb and sets
/// FLAG_VALE_FAREWELL_HERB_RECEIVED once; afterwards the merchant turns to
/// watch the leader.
static void ValeInteriorsShops_GiveFarewellHerb(void) {
  s32 gift;

  Engine_EventStart();
  if (GameFlag_IsSet(FLAG_VALE_FAREWELL_HERB_RECEIVED) != 0) {
    Engine_EventSetMessage(MsgValeYouMustSaveJenna);
    Event_ShowMessage(ACTOR_ITEM_MERCHANT, 0);
    FieldObject_SetTargetAndCallback(
        ACTOR_ITEM_MERCHANT, ACTOR_LINK_KEEP_MOTION | ACTOR_PARTY_LEADER,
        gValeInteriorsShops_FaceTargetScript);
  } else {
    gift = MsgValeThisIsMyFarewellGift;
    Engine_EventSetMessage(gift);
    Event_ShowMessageAndWait(ACTOR_ITEM_MERCHANT, 0, 20);
    Engine_MessageShowCentered((gift + 1), 1);
    Engine_PartyGiveItem(ITEM_HERB, 0);
    GameFlag_Set(FLAG_VALE_FAREWELL_HERB_RECEIVED);
  }
  Engine_EventEnd();
}

/// A woman in a green dress asks whether the people who caused the eruption
/// had strange powers, in a house of Vale after the party has left.
///
/// Reached by talking to ACTOR_WOMAN_ON_VISITORS_IN_TOWN after the party has
/// left Vale.
static void Event_AskAboutStrangePowers(void) {
  Engine_EventStart();
  Engine_EventSetMessage(MsgValeTheCulpritsHadStrangePowers);
  Engine_EventAskYesNo(ACTOR_WOMAN_ON_VISITORS_IN_TOWN, 0);
  Engine_EventEnd();
}

/// Garet's father asks whether Isaac heard them talking about Dora. He is in
/// Garet's house in Vale, back from Kalay.
///
/// Reached by talking to Garet's father after Kalay.
static void Event_AskAboutDora(void) {
  Engine_EventStart();
  Engine_EventSetMessage(MsgValeDidYouHearAboutDora);
  Engine_EventAskYesNo(ACTOR_GARETS_FATHER, 0);
  Engine_EventEnd();
}

/// Garet's mother hopes the travelers didn't get sick on the way. She is in
/// Garet's house in Vale, back from Kalay.
///
/// Reached by talking to Garet's mother after Kalay.
static void Event_WorryAboutTravelersHealth(void) {
  Engine_EventStart();
  Engine_EventSetMessage(MsgValeHopeDidntGet);
  Engine_EventShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_GARETS_MOTHER, 0);
  Engine_EventEnd();
}

/// Leaves a house through its door: the exit sound plays and the screen
/// closes, stopping the lightning first on the stormy night.
static void ValeInteriorsShops_RequestWindowExit(s32 exitId) {
  if (Engine_GameFlagIsSet(FLAG_VALE_STORM) != 0)
    FieldEffect_StopLightning();
  Engine_AudioPlayCue(SE_MAP_EXIT);
  gEventWork->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  gEventWork->transitionFrames = 16;
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(exitId);
}

// The rooms' doors: stepping on a door's cell, whose collision code is its
// exit's number, takes that exit back out, in every table. On the stormy night
// the exits to SceneId_Vale lead to the storm's map (SceneId_ValeStorm)
// instead.

/// Leaves the house where a young man dreams of leaving. It's the room entered
/// from VALE_INTERIORS_SHOPS_FROM_VALE_WEST, where
/// ACTOR_YOUNG_MAN_DREAMING_OF_LEAVING wanders.
static void Event_LeaveToValeWest(void) {
  ValeInteriorsShops_RequestWindowExit(VALE_INTERIORS_SHOPS_TO_VALE_WEST);
}

/// Leaves Jenna's grandparents' house.
static void Event_LeaveToValeNorth(void) {
  ValeInteriorsShops_RequestWindowExit(VALE_INTERIORS_SHOPS_TO_VALE_NORTH);
}

/// Leaves the empty house. It's the room entered from
/// VALE_INTERIORS_SHOPS_FROM_VALE_GATE_NORTH, which no placement table puts
/// anyone in.
static void Event_LeaveToValeGateWest(void) {
  ValeInteriorsShops_RequestWindowExit(VALE_INTERIORS_SHOPS_TO_VALE_GATE_WEST);
}

/// Leaves Vale's shops.
static void Event_LeaveToValeGateSouth(void) {
  ValeInteriorsShops_RequestWindowExit(VALE_INTERIORS_SHOPS_TO_VALE_GATE_SOUTH);
}

/// Leaves the house where villagers talk of Kraden and his visitors. It's the
/// room entered from VALE_INTERIORS_SHOPS_FROM_VALE_SOUTH, where
/// ACTOR_MAN_ON_KRADENS_STUDIES and ACTOR_WOMAN_ON_VISITORS_IN_TOWN stand.
static void Event_LeaveToValeSouth(void) {
  ValeInteriorsShops_RequestWindowExit(VALE_INTERIORS_SHOPS_TO_VALE_SOUTH);
}

/// Leaves Garet's house.
static void Event_LeaveToValeEast(void) {
  ValeInteriorsShops_RequestWindowExit(VALE_INTERIORS_SHOPS_TO_VALE_EAST);
}

/// Leaves the house where a strongman plans to scare Kraden's visitors. It's
/// the room entered from VALE_INTERIORS_SHOPS_FROM_VALE_GATE_SOUTH, where
/// ACTOR_STRONGMAN_SCARING_VISITORS stands.
static void Event_LeaveToValeGateEast(void) {
  ValeInteriorsShops_RequestWindowExit(VALE_INTERIORS_SHOPS_TO_VALE_GATE_EAST);
}

/// Leaves the house where a woman talks of permission to leave. It's the room
/// entered from VALE_INTERIORS_SHOPS_FROM_VALE_GATE_EAST, where
/// ACTOR_WOMAN_ON_PERMISSION_TO_LEAVE stands.
static void Event_LeaveToValeGateNorth(void) {
  ValeInteriorsShops_RequestWindowExit(VALE_INTERIORS_SHOPS_TO_VALE_GATE_NORTH);
}

/// Arriving in a house of Vale: on the stormy night the residents are gone and
/// lightning flashes outside; back from Kalay, entering Garet's house the first
/// time plays his homecoming.
///
/// The scene's initialize entry. Runs Event_GaretComesHome from
/// VALE_INTERIORS_SHOPS_FROM_VALE_EAST after Kalay until FLAG_GARET_CAME_HOME
/// is set. From VALE_INTERIORS_SHOPS_FROM_VALE_NORTH, once the party has left
/// Vale, Jenna's grandmother lies sick in bed.
s32 ValeInteriorsShops_Initialize(void) {
  struct FieldActor *actor;
  struct EventWork **eventSlot = &gEventWork;

  (*eventSlot)->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  // On the stormy night every resident is out, and lightning flashes outside.
  if (Engine_GameFlagIsSet(FLAG_VALE_STORM) != 0) {
    Engine_ActorSetPosition(ACTOR_GARETS_GRANDFATHER, 0, 0);
    Engine_ActorSetPosition(ACTOR_GARETS_GRANDMOTHER, 0, 0);
    Engine_ActorSetPosition(ACTOR_GARETS_FATHER, 0, 0);
    Engine_ActorSetPosition(ACTOR_GARETS_MOTHER, 0, 0);
    Engine_ActorSetPosition(ACTOR_JENNAS_GRANDFATHER, 0, 0);
    Engine_ActorSetPosition(ACTOR_JENNAS_GRANDMOTHER, 0, 0);
    Engine_ActorSetPosition(ACTOR_WOMAN_ON_THE_SPITTING_IMAGE, 0, 0);
    Engine_ActorSetPosition(ACTOR_MAN_ON_KRADENS_STUDIES, 0, 0);
    Engine_ActorSetPosition(ACTOR_WOMAN_ON_VISITORS_IN_TOWN, 0, 0);
    Engine_ActorSetPosition(ACTOR_YOUNG_MAN_DREAMING_OF_LEAVING, 0, 0);
    Engine_ActorSetPosition(ACTOR_WOMAN_ON_PERMISSION_TO_LEAVE, 0, 0);
    Engine_ActorSetPosition(ACTOR_STRONGMAN_SCARING_VISITORS, 0, 0);
    Engine_ActorSetPosition(ACTOR_ITEM_MERCHANT, 0, 0);
    Engine_ActorSetPosition(ACTOR_WEAPON_MERCHANT, 0, 0);
    Engine_ActorSetPosition(ACTOR_ARMOR_MERCHANT, 0, 0);
    FieldEffect_StartLightning();
    (*(struct FieldBlendWork **)((u8 *)eventSlot + Ram_SceneMapStateOffset))
        ->loud = 1;
    FieldEffect_LastLightning();
    Engine_TaskWait(30);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    FieldEffect_ResumeLightning();
  }
  // The returning party gets the household reunion only on its first visit.
  if (Engine_GameFlagIsSet(FLAG_KALAY_REACHED) != 0) {
    if (gGameState.entrance == VALE_INTERIORS_SHOPS_FROM_VALE_EAST) {
      if (Engine_GameFlagIsSet(FLAG_GARET_CAME_HOME) == 0) {
        Event_GaretComesHome();
      }
    }
    Object_GetById(ACTOR_GARETS_FATHER)->collisionFlags |= 0x80;
  }
  // Once Jenna is taken, her grandmother lies sick in bed.
  if (gGameState.entrance == VALE_INTERIORS_SHOPS_FROM_VALE_NORTH) {
    if (Engine_GameFlagIsSet(FLAG_VALE_PARTY_LEFT) != 0) {
      Engine_ActorSetPosition(ACTOR_JENNAS_GRANDMOTHER, PIXELS(454),
                              PIXELS(150));
      actor = Object_GetById(ACTOR_JENNAS_GRANDMOTHER);
      Engine_ActorSetSpriteFlags(actor, 0);
      Engine_ActorSetAnimation(ACTOR_JENNAS_GRANDMOTHER,
                               ANIMATION_GRANDMOTHER_IN_BED);
      Map_ClearLayerEntryFlag(4);
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
static void Event_TalkToWeaponMerchant(void) {
  struct FieldActor *leader;

  leader = Object_GetById(ACTOR_PARTY_LEADER);

  if (FACING_IS_NORTH(leader->facing)) {
    Engine_ShopOpen(1, ACTOR_WEAPON_MERCHANT);
    return;
  }

  Engine_EventStart();
  if (Engine_GameFlagIsSet(FLAG_KALAY_REACHED) != 0) {
    Engine_EventSetMessage(MsgValeYouCameBack);
    Engine_EventAskYesNo(ACTOR_WEAPON_MERCHANT, 0);
  } else {
    if (Engine_GameFlagIsSet(FLAG_VALE_PARTY_LEFT) != 0) {
      Engine_EventSetMessage(MsgValeValeFeelsEmpty);
    } else {
      Engine_EventSetMessage(MsgValeGoodWeaponsDrawOutStrength);
    }
    Engine_EventShowMessage(ACTOR_WEAPON_MERCHANT, 0);
  }
  Engine_EventEnd();
}

/// In Vale's shops, opens the armor shop across the counter, or the merchant
/// speaks.
///
/// Reached by talking to the armor merchant in every table but the stormy
/// night's; opens shop 2 when facing north.
static void Event_TalkToArmorMerchant(void) {
  struct FieldActor *actor;

  actor = Object_GetById(ACTOR_PARTY_LEADER);
  if (FACING_IS_NORTH(actor->facing)) {
    Engine_ShopOpen(2, ACTOR_ARMOR_MERCHANT);
  } else {
    Engine_EventStart();
    if (Engine_GameFlagIsSet(FLAG_KALAY_REACHED) != 0) {
      Engine_EventSetMessage(MsgValeTheRumorWasTrue);
    } else {
      if (Engine_GameFlagIsSet(FLAG_VALE_PARTY_LEFT) != 0) {
        Engine_EventSetMessage(MsgValeImNotSadJustGo);
      } else {
        Engine_EventSetMessage(MsgValeGoodArmorDrawsOutStrength);
      }
    }
    Engine_EventShowMessage(ACTOR_ARMOR_MERCHANT, 0);
    Engine_EventEnd();
  }
}

/// In Vale's shops, opens the item shop across the counter, or the merchant
/// speaks.
///
/// Reached by talking to the item merchant in every table but the stormy
/// night's; opens shop 3 when facing north. After the party has left and
/// before Kalay, ValeInteriorsShops_GiveFarewellHerb speaks instead.
static void Event_TalkToItemMerchant(void) {
  struct FieldActor *actor;

  actor = Object_GetById(ACTOR_PARTY_LEADER);
  if (FACING_IS_NORTH(actor->facing)) {
    Engine_ShopOpen(3, ACTOR_ITEM_MERCHANT);
  } else {
    if (Engine_GameFlagIsSet(FLAG_KALAY_REACHED) != 0) {
      Engine_EventStart();
      Engine_EventSetMessage(MsgValeWhenDidYouComeBack);
      Engine_EventShowMessage(ACTOR_ITEM_MERCHANT, 0);
      Engine_EventEnd();
    } else {
      if (Engine_GameFlagIsSet(FLAG_VALE_PARTY_LEFT) != 0) {
        ValeInteriorsShops_GiveFarewellHerb();
      } else {
        Engine_EventStart();
        Engine_EventSetMessage(MsgValeAWiseManFleesWhen);
        Engine_EventShowMessage(ACTOR_ITEM_MERCHANT, 0);
        Engine_EventEnd();
      }
    }
  }
}

/// Garet's grandparents and father nod.
static inline void ValeInteriorsShops_FamilyNods(void) {
  Engine_ActorSetAnimation(ACTOR_GARETS_GRANDFATHER, ANIMATION_NOD);
  Engine_ActorSetAnimation(ACTOR_GARETS_GRANDMOTHER, ANIMATION_NOD);
  Engine_ActorSetAnimation(ACTOR_GARETS_FATHER, ANIMATION_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_GARETS_GRANDMOTHER, ANIMATION_NOD);
}

/// Starts the repeated motion before waiting for the requested animation.
static inline void
ValeInteriorsShops_PlayRepeatedAnimationAndWait(s32 actorId, s32 animation) {
  Engine_ActorStartRepeatedMotion(actorId, 1);
  Engine_ActorSetAnimationAndWait(actorId, animation);
}

/// In Garet's house in Vale, back from Kalay, Garet comes home: his
/// grandfather dozes, his parents argue over whether to look for him, and his
/// grandmother spots him first. Garet introduces Ivan and Mia, his mother asks
/// them to look after him, and his father tells them to enjoy their stay; the
/// three walk back into Isaac, and the father is left pacing.
///
/// Called by ValeInteriorsShops_Initialize from
/// VALE_INTERIORS_SHOPS_FROM_VALE_EAST after Kalay, until FLAG_GARET_CAME_HOME
/// is set; sets it. Adds and removes the callback
/// ValeInteriorsShops_SpawnSleepZ, and leaves the father pacing.
static void Event_GaretComesHome(void) {
  struct FieldActor *sleepZ;
  struct FieldActor *actor;
  s32 motionFlags;
  SchedulerCallback timedUpdate;
  const s32 *partyScript;

  // The party walks in while Garet's grandfather dozes, sleep drifting up
  // from him.
  Engine_EventStart();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_TaskWait(1);
  Engine_ActorSetSpritePriority(MIA, 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(2) / 5, PIXELS(1) / 5);
  Actor_SetSpeed(GARET, PIXELS(2) / 5, PIXELS(1) / 5);
  Actor_SetSpeed(IVAN, PIXELS(2) / 5, PIXELS(1) / 5);
  Actor_SetSpeed(MIA, PIXELS(2) / 5, PIXELS(1) / 5);
  Engine_ActorSetAnimation(ACTOR_GARETS_GRANDFATHER,
                           ANIMATION_GRANDFATHER_ASLEEP);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 808, 508);
  sleepZ = Actor_Get(ACTOR_FIRST_SLEEP_Z);
  Engine_ActorSetSpriteFlags(sleepZ, 0);
  sleepZ = Actor_Get(ACTOR_FIRST_SLEEP_Z + 1);
  Engine_ActorSetSpriteFlags(sleepZ, 0);
  sleepZ = Actor_Get(ACTOR_FIRST_SLEEP_Z + 2);
  Engine_ActorSetSpriteFlags(sleepZ, 0);
  motionFlags = 0;
  Object_GetById(ACTOR_FIRST_SLEEP_Z)->motionFlags = motionFlags;
  Object_GetById(ACTOR_FIRST_SLEEP_Z + 1)->motionFlags = motionFlags;
  Object_GetById(ACTOR_FIRST_SLEEP_Z + 2)->motionFlags = motionFlags;
  timedUpdate = ValeInteriorsShops_SpawnSleepZ;
  FieldScheduler_AddCallback(timedUpdate, TASK_PRIORITY_SCENE);
  Engine_TaskWait(1);
  gEventWork->transitionFrames = 32;
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIMATION_STAND);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetPosition(GARET, actor->x.fixed, actor->z.fixed);
  }
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetPosition(IVAN, actor->x.fixed, actor->z.fixed);
  }
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetPosition(MIA, actor->x.fixed, actor->z.fixed);
  }
  Actor_WalkTo(GARET, 792, 512);
  Actor_WalkTo(IVAN, 824, 504);
  Actor_WalkToAndWait(MIA, 818, 524);
  Engine_ActorSetAnimation(GARET, ANIMATION_STAND);
  Engine_ActorSetAnimation(IVAN, ANIMATION_STAND);
  Engine_EventWait(10);
  FieldObject_SetTargetAndCallback(ACTOR_PARTY_LEADER,
                                   ACTOR_LINK_KEEP_MOTION | ACTOR_GARETS_FATHER,
                                   gValeInteriorsShops_FaceTargetScript);
  FieldObject_SetTargetAndCallback(GARET,
                                   ACTOR_LINK_KEEP_MOTION | ACTOR_GARETS_FATHER,
                                   gValeInteriorsShops_FaceTargetScript);
  FieldObject_SetTargetAndCallback(IVAN,
                                   ACTOR_LINK_KEEP_MOTION | ACTOR_GARETS_FATHER,
                                   gValeInteriorsShops_FaceTargetScript);
  FieldObject_SetTargetAndCallback(MIA,
                                   ACTOR_LINK_KEEP_MOTION | ACTOR_GARETS_FATHER,
                                   gValeInteriorsShops_FaceTargetScript);
  Engine_EventWait(300);
  Camera_GetEventObject()->motionFlags = motionFlags;
  // The camera pans slowly up to Garet's parents. His father, pacing, asks
  // whether they should look for Garet; Isaac still does not know about
  // Dora. His mother teases that he misses Garet.
  Camera_SetSpeed(PIXELS(1) / 10, PIXELS(1) / 80);
  Camera_MoveTo(PIXELS(786), 0, PIXELS(430), 1);
  Engine_EventWait(240);
  Engine_ActorStop(ACTOR_GARETS_FATHER);
  Actor_ShowEmote(ACTOR_GARETS_FATHER, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  Actor_WalkToAndWait(ACTOR_GARETS_FATHER, 819, 405);
  Engine_EventWait(40);
  Engine_ActorSetAnimationAndWait(ACTOR_GARETS_FATHER, ANIMATION_SHAKE_HEAD);
  Engine_EventWait(40);
  Actor_FaceDirection(ACTOR_GARETS_FATHER, FACING_NORTH + FACING_STEP, 20);
  Engine_EventSetMessage(MsgValeHey2);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT |
                               ACTOR_GARETS_FATHER,
                           0, 20);
  Engine_ActorStop(ACTOR_PARTY_LEADER);
  Engine_ActorStop(GARET);
  Engine_ActorStop(IVAN);
  Engine_ActorStop(MIA);
  Actor_ShowEmote(ACTOR_GARETS_MOTHER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_GARETS_MOTHER, 0, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_GARETS_FATHER, 2);
  Engine_EventWait(40);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT |
                               ACTOR_GARETS_FATHER,
                           0, 10);
  Actor_FaceDirection(ACTOR_GARETS_MOTHER, FACING_SOUTH + FACING_STEP, 10);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_GARETS_MOTHER, 0, 40);
  Engine_ActorRunRepeatedMotion(ACTOR_GARETS_FATHER, 2);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT |
                               ACTOR_GARETS_FATHER,
                           0, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  Actor_ShowEmote(ACTOR_GARETS_MOTHER, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 40);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_GARETS_MOTHER, 0, 40);
  Engine_ActorStartRepeatedMotion(ACTOR_GARETS_FATHER, 2);
  Actor_ShowEmote(ACTOR_GARETS_FATHER, EMOTE_IN_FRONT | EMOTE_WINCE, 20);
  Engine_ActorSetAnimation(ACTOR_GARETS_FATHER, ANIMATION_SHAKE_HEAD);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT |
                               ACTOR_GARETS_FATHER,
                           0, 10);
  ValeInteriorsShops_PlayRepeatedAnimationAndWait(ACTOR_GARETS_MOTHER,
                                                  ANIMATION_NOD);
  Engine_EventWait(20);
  ValeInteriorsShops_PlayRepeatedAnimationAndWait(ACTOR_GARETS_FATHER,
                                                  ANIMATION_SHAKE_HEAD);
  ValeInteriorsShops_PlayRepeatedAnimationAndWait(ACTOR_GARETS_MOTHER,
                                                  ANIMATION_NOD);
  ValeInteriorsShops_PlayRepeatedAnimationAndWait(ACTOR_GARETS_FATHER,
                                                  ANIMATION_SHAKE_HEAD);
  // Garet's grandmother sees him and chokes on her words; his mother asks if
  // she is choking, his father thinks she misses Garet too.
  Actor_ShowEmote(ACTOR_GARETS_GRANDMOTHER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Engine_ActorRunRepeatedMotion(ACTOR_GARETS_GRANDMOTHER, 1);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_GARETS_GRANDMOTHER, FACING_EAST + FACING_STEP, 40);
  Engine_ActorRunRepeatedMotion(ACTOR_GARETS_GRANDMOTHER, 2);
  Engine_EventWait(60);
  Engine_ActorRunRepeatedMotion(ACTOR_GARETS_GRANDMOTHER, 3);
  Engine_EventWait(40);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_GRANDMOTHER, 0,
                           40);
  Engine_ActorSetAnimation(ACTOR_GARETS_MOTHER, ANIMATION_STILL);
  Engine_ActorRunRepeatedMotion(ACTOR_GARETS_MOTHER, 2);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_GARETS_MOTHER, 0, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_GARETS_GRANDMOTHER,
                                  ANIMATION_SHAKE_HEAD);
  Engine_ActorRunRepeatedMotion(ACTOR_GARETS_GRANDMOTHER, 2);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_GRANDMOTHER, 0,
                           10);
  Actor_ShowEmote(ACTOR_GARETS_FATHER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Actor_FaceDirection(ACTOR_GARETS_FATHER, FACING_SOUTH + FACING_STEP, 40);
  Engine_ActorSetAnimationAndWait(ACTOR_GARETS_FATHER, ANIMATION_NOD);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_FATHER, 0, 10);
  // No, it really is Garet: both parents turn and call his name together.
  Engine_ActorSetAnimationAndWait(ACTOR_GARETS_GRANDMOTHER,
                                  ANIMATION_SHAKE_HEAD);
  Actor_FaceDirection(ACTOR_GARETS_GRANDMOTHER, FACING_NORTH + FACING_STEP, 10);
  Engine_ActorJump(ACTOR_GARETS_GRANDMOTHER, 2, 0);
  Engine_ActorSetAnimation(ACTOR_GARETS_GRANDMOTHER, ANIMATION_SHAKE_HEAD);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_GRANDMOTHER, 0,
                           10);
  Actor_ShowEmote(ACTOR_GARETS_MOTHER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_GARETS_FATHER, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_FaceDirection(ACTOR_GARETS_FATHER, FACING_NORTH + FACING_STEP, 80);
  Actor_FaceDirection(ACTOR_GARETS_FATHER, FACING_SOUTH + FACING_STEP, 60);
  Engine_ActorStartRepeatedMotion(ACTOR_GARETS_FATHER, 2);
  Engine_ActorStartRepeatedMotion(ACTOR_GARETS_MOTHER, 2);
  Event_ShowTwoMessagesAndWait(ACTOR_GARETS_FATHER, 11, 6, 6, 6,
                               ACTOR_GARETS_MOTHER, 12, 1, 7, 1, 0);
  Engine_EventWait(20);
  Camera_SetSpeed(PIXELS(8) / 5, PIXELS(1) / 5);
  Camera_MoveTo(PIXELS(777), 0, PIXELS(468), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(40);
  Engine_ActorSetAnimationAndWait(GARET, ANIMATION_NOD);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | GARET, 0, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 2);
  // His grandfather wakes, and the sleep stops drifting.
  Engine_TaskRemoveCallback(timedUpdate);
  Engine_EventWait(40);
  Engine_ActorSetAnimationAndWait(ACTOR_GARETS_GRANDFATHER,
                                  ANIMATION_GRANDFATHER_LOOK_ABOUT);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_GRANDFATHER, 0,
                           20);
  // The grandfather looks about, then jumps up with Garet's name.
  Camera_MoveTo(PIXELS(750), 0, PIXELS(451), 1);
  Engine_EventWait(20);
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
  Engine_ActorSetAnimationAndWait(ACTOR_GARETS_GRANDFATHER,
                                  ANIMATION_GRANDFATHER_LOOK_ABOUT);
  Engine_EventWait(60);
  Engine_ActorJump(ACTOR_GARETS_GRANDFATHER, 6, 0);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_GRANDFATHER, 0,
                           20);
  // Garet goes to him; the grandfather asks whether the Wise One's mission
  // is done, and a no skips one line of Garet's father.
  Actor_SetSpeed(GARET, PIXELS(8) / 5, PIXELS(4) / 5);
  Actor_WalkToAndWait(GARET, 789, 473);
  Actor_FaceDirection(GARET, FACING_SOUTHWEST + FACING_STEP, 20);
  Engine_ActorSetAnimationAndWait(GARET, ANIMATION_NOD);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | GARET, 0, 10);
  Actor_FaceDirection(ACTOR_GARETS_GRANDFATHER, FACING_EAST + FACING_STEP, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_GARETS_GRANDFATHER, ANIMATION_NOD);
  Event_OpenMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_GRANDFATHER, 0);
  Actor_FaceDirection(ACTOR_GARETS_FATHER, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_GARETS_GRANDMOTHER, FACING_EAST + FACING_STEP, 0);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(IVAN, FACING_SOUTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(MIA, FACING_NORTHWEST + FACING_STEP, 0);
  if (Engine_EventChooseYesNo(0, 0) == 1) {
    gEventWork->message++;
  }
  Camera_MoveTo(PIXELS(777), 0, PIXELS(428), 1);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(ACTOR_GARETS_FATHER, 2);
  Event_ShowMessage(ACTOR_GARETS_FATHER, 0);
  Engine_ActorSetAnimationAndWait(ACTOR_GARETS_MOTHER, ANIMATION_SHAKE_HEAD);
  Engine_EventWait(20);
  // His mother knows he is leaving again soon; his grandparents are glad to
  // see him well.
  Engine_EventSetMessage(MsgValeYoureLeavingAgainSoon);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_GARETS_MOTHER, 0);
  Camera_MoveTo(PIXELS(777), 0, PIXELS(468), 1);
  Engine_EventWait(20);
  Actor_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 20);
  Engine_ActorSetAnimationAndWait(GARET, ANIMATION_NOD);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_GARETS_GRANDMOTHER,
                                  ANIMATION_SHAKE_HEAD);
  Actor_FaceDirection(ACTOR_GARETS_GRANDMOTHER, FACING_NORTH + FACING_STEP, 10);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_GRANDMOTHER, 0);
  Engine_ActorSetAnimationAndWait(ACTOR_GARETS_GRANDFATHER, ANIMATION_NOD);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_GRANDFATHER, 0);
  Actor_FaceDirection(GARET, FACING_SOUTHWEST + FACING_STEP, 10);
  Engine_ActorSetAnimationAndWait(GARET, ANIMATION_NOD);
  Actor_FaceDirection(ACTOR_GARETS_GRANDMOTHER, FACING_EAST + FACING_STEP, 10);
  Engine_ActorSetAnimation(ACTOR_GARETS_MOTHER, ANIMATION_NOD);
  Engine_ActorSetAnimation(ACTOR_GARETS_FATHER, ANIMATION_NOD);
  Engine_ActorSetAnimation(ACTOR_GARETS_GRANDMOTHER, ANIMATION_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_GARETS_GRANDFATHER, ANIMATION_NOD);
  Engine_EventWait(20);
  // Garet introduces his new friends.
  Actor_FaceDirection(GARET, FACING_SOUTHEAST + FACING_STEP, 20);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  Actor_FaceDirection(GARET, FACING_SOUTHWEST + FACING_STEP, 20);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | GARET, 0, 20);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST + FACING_STEP, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 40);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIMATION_NOD);
  Engine_ActorSetAnimationAndWait(GARET, ANIMATION_NOD);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 20);
  Engine_ActorSetAnimationAndWait(MIA, ANIMATION_NOD);
  Engine_EventWait(20);
  Actor_FaceDirection(GARET, FACING_EAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
  // Ivan, then Mia, step forward and introduce themselves; the family nods.
  Actor_SetSpeed(IVAN, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(IVAN, 819, 489);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST + FACING_STEP, 40);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Event_ShowMessageAndWait(IVAN, 0, 20);
  Engine_ActorSetAnimationAndWait(IVAN, ANIMATION_NOD);
  ValeInteriorsShops_FamilyNods();
  Object_GetById(MIA)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  Engine_ActorSetSpritePriority(MIA, 1);
  Actor_SetSpeed(MIA, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(MIA, 794, 520);
  Actor_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_WalkToAndWait(MIA, 784, 496);
  Actor_FaceDirection(MIA, FACING_WEST + FACING_STEP, 10);
  Object_GetById(MIA)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Event_ShowMessageAndWait(MIA, 0, 20);
  ValeInteriorsShops_FamilyNods();
  Engine_EventWait(20);
  Camera_MoveTo(PIXELS(777), 0, PIXELS(428), 1);
  Engine_EventWait(20);
  // Garet's mother comes forward and asks them to look after young Garet.
  Actor_SetSpeed(ACTOR_GARETS_MOTHER, PIXELS(2) / 5, PIXELS(1) / 5);
  Actor_WalkToAndWait(ACTOR_GARETS_MOTHER, 835, 388);
  Actor_FaceDirection(ACTOR_GARETS_MOTHER, FACING_SOUTH + FACING_STEP, 0);
  Actor_ShowEmote(ACTOR_GARETS_MOTHER, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_GARETS_MOTHER, 0, 20);
  Camera_MoveTo(PIXELS(777), 0, PIXELS(468), 1);
  Engine_EventWait(40);
  Actor_FaceDirection(IVAN, FACING_SOUTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(MIA, FACING_NORTHEAST + FACING_STEP, 40);
  Actor_FaceDirection(IVAN, FACING_WEST + FACING_STEP, 0);
  Actor_FaceDirection(MIA, FACING_NORTH + FACING_STEP, 20);
  Engine_ActorSetAnimation(IVAN, ANIMATION_NOD);
  Engine_ActorSetAnimationAndWait(MIA, ANIMATION_NOD);
  Engine_EventWait(20);
  // His father: enough chit-chat, relax and enjoy your stay.
  Engine_ActorRunRepeatedMotion(ACTOR_GARETS_FATHER, 1);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(ACTOR_GARETS_FATHER, ANIMATION_NOD);
  Event_ShowMessageAndWait(ACTOR_GARETS_FATHER, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(MIA, FACING_NORTH + FACING_STEP, 40);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIMATION_NOD);
  Engine_ActorSetAnimation(GARET, ANIMATION_NOD);
  Engine_ActorSetAnimation(IVAN, ANIMATION_NOD);
  Engine_ActorSetAnimationAndWait(MIA, ANIMATION_NOD);
  Engine_EventWait(20);
  Actor_SetSpeed(IVAN, PIXELS(1), PIXELS(1) / 2);
  partyScript = gValeInteriorsShops_PartyLeavesScript;
  Actor_SetActionScript(GARET, partyScript);
  Actor_SetActionScript(IVAN, partyScript);
  Actor_RunActionScriptAndWait(MIA, partyScript);
  Actor_SetActionScript(ACTOR_GARETS_FATHER,
                        gValeInteriorsShops_FatherPacingScript);
  Actor_WalkToAndWait(ACTOR_GARETS_MOTHER, 837, 376);
  Actor_FaceDirection(ACTOR_GARETS_MOTHER, FACING_NORTH + FACING_STEP, 20);
  GameFlag_Set(FLAG_GARET_CAME_HOME);
  Engine_EventEnd();
}

/// Sends a sprite drifting up from Garet's sleeping grandfather, one of the
/// three at frames 10, 20 and 30 of every 180.
///
/// A callback Event_GaretComesHome adds while the grandfather dozes.
static void ValeInteriorsShops_SpawnSleepZ(void) {
  s32 actorId;
  u32 cycleFrame;
  struct FieldActor *actor;
  struct FieldActor *origin;

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
  actor = Object_GetById(actorId);
  if (actor == NULL) {
    return;
  }
  origin = Object_GetById(ACTOR_GARETS_GRANDFATHER);
  if (origin != NULL) {
    Engine_ActorSetPosition(actorId, origin->x.fixed, origin->z.fixed);
  }
  // Restart this sprite above the origin with a small scale and 8.8 motion.
  actor->scaleX = 2 * Q16_ONE / 5;
  actor->scaleY = 2 * Q16_ONE / 5;
  {
    s32 height = actor->y.fixed + PIXELS(24);

    actor->y.fixed = height;
    actor->targetY = height;
    // The drifting script consumes these two work fields as signed 8.8 steps.
    actor->timer = 25; // 25/256 pixel per frame.
    actor->work = 128; // 1/2 pixel per frame.
  }
  Actor_SetActionScript(actorId, gValeInteriorsShops_SleepZScript);
}

s32 gValeInteriorsShops_FaceTargetScript[] = {
    SC_CALL(Scene_TurnTowardTarget),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gValeInteriorsShops_FatherPacingScript[] = {
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

s32 gValeInteriorsShops_PartyLeavesScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(808), 0, PIXELS(508)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

s32 gValeInteriorsShops_SleepZScript[] = {
    SC_CALL(ValeInteriorsShops_AdvanceSleepZ),
    SC_WAIT(1),
    SC_LOOP(60, SC_TOP),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

struct SceneEntrance gValeInteriorsShops_Entrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 120, 0, 100, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {VALE_INTERIORS_SHOPS_FROM_VALE_WEST, CONDITION_ALWAYS, 96, 0, 222,
     FACING_NORTH, 0, 4, 4, 244, 292, 0},
    {VALE_INTERIORS_SHOPS_FROM_VALE_NORTH, CONDITION_ALWAYS, 337, 0, 199,
     FACING_NORTH, 0, 252, 4, 548, 236, 0},
    {VALE_INTERIORS_SHOPS_FROM_VALE_GATE_NORTH, CONDITION_ALWAYS, 656, 0, 244,
     FACING_NORTH, 0, 556, 4, 900, 276, 0},
    {VALE_INTERIORS_SHOPS_FROM_VALE_GATE_WEST, CONDITION_ALWAYS, 159, 0, 525,
     FACING_NORTH, 0, 4, 324, 332, 564, 0},
    {VALE_INTERIORS_SHOPS_FROM_VALE_SOUTH, CONDITION_ALWAYS, 528, 0, 486,
     FACING_NORTH, 0, 348, 284, 612, 516, 0},
    {VALE_INTERIORS_SHOPS_FROM_VALE_EAST, CONDITION_ALWAYS, 816, 0, 522,
     FACING_NORTH, 0, 628, 284, 884, 548, 0},
    {VALE_INTERIORS_SHOPS_FROM_VALE_GATE_SOUTH, CONDITION_ALWAYS, 463, 0, 851,
     FACING_NORTH, 0, 324, 604, 588, 884, 0},
    {VALE_INTERIORS_SHOPS_FROM_VALE_GATE_EAST, CONDITION_ALWAYS, 769, 0, 857,
     FACING_NORTH, 0, 580, 628, 852, 884, 0},
    {SCENE_TABLE_END},
};

u32 gValeInteriorsShops_Exits[] = {
    SCENE_EXITS(ValeInteriorsShops),
    SCENE_EXIT(VALE_INTERIORS_SHOPS_TO_VALE_WEST, Vale,
               VALE_FROM_VALE_INTERIORS_SHOPS_WEST),
    SCENE_EXIT(VALE_INTERIORS_SHOPS_TO_VALE_NORTH, Vale,
               VALE_FROM_VALE_INTERIORS_SHOPS_NORTH),
    SCENE_EXIT(VALE_INTERIORS_SHOPS_TO_VALE_GATE_WEST, ValeGate,
               VALE_GATE_FROM_VALE_INTERIORS_SHOPS_WEST),
    SCENE_EXIT(VALE_INTERIORS_SHOPS_TO_VALE_GATE_SOUTH, ValeGate,
               VALE_GATE_FROM_VALE_INTERIORS_SHOPS_SOUTH),
    SCENE_EXIT(VALE_INTERIORS_SHOPS_TO_VALE_SOUTH, Vale,
               VALE_FROM_VALE_INTERIORS_SHOPS_SOUTH),
    SCENE_EXIT(VALE_INTERIORS_SHOPS_TO_VALE_EAST, Vale,
               VALE_FROM_VALE_INTERIORS_SHOPS_EAST),
    SCENE_EXIT(VALE_INTERIORS_SHOPS_TO_VALE_GATE_EAST, ValeGate,
               VALE_GATE_FROM_VALE_INTERIORS_SHOPS_EAST),
    SCENE_EXIT(VALE_INTERIORS_SHOPS_TO_VALE_GATE_NORTH, ValeGate,
               VALE_GATE_FROM_VALE_INTERIORS_SHOPS_NORTH),
    SCENE_EXITS_END,
};

u32 gValeInteriorsShops_StormExits[] = {
    SCENE_EXITS(ValeInteriorsShops),
    SCENE_EXIT(VALE_INTERIORS_SHOPS_TO_VALE_WEST, ValeStorm,
               VALE_STORM_FROM_VALE_INTERIORS_SHOPS_WEST),
    SCENE_EXIT(VALE_INTERIORS_SHOPS_TO_VALE_NORTH, ValeStorm,
               VALE_STORM_FROM_VALE_INTERIORS_SHOPS_NORTH),
    SCENE_EXIT(VALE_INTERIORS_SHOPS_TO_VALE_GATE_WEST, ValeGate,
               VALE_GATE_FROM_VALE_INTERIORS_SHOPS_WEST),
    SCENE_EXIT(VALE_INTERIORS_SHOPS_TO_VALE_GATE_SOUTH, ValeGate,
               VALE_GATE_FROM_VALE_INTERIORS_SHOPS_SOUTH),
    SCENE_EXIT(VALE_INTERIORS_SHOPS_TO_VALE_SOUTH, ValeStorm,
               VALE_STORM_FROM_VALE_INTERIORS_SHOPS_SOUTH),
    SCENE_EXIT(VALE_INTERIORS_SHOPS_TO_VALE_EAST, ValeStorm,
               VALE_STORM_FROM_VALE_INTERIORS_SHOPS_EAST),
    SCENE_EXIT(VALE_INTERIORS_SHOPS_TO_VALE_GATE_EAST, ValeGate,
               VALE_GATE_FROM_VALE_INTERIORS_SHOPS_EAST),
    SCENE_EXIT(VALE_INTERIORS_SHOPS_TO_VALE_GATE_NORTH, ValeGate,
               VALE_GATE_FROM_VALE_INTERIORS_SHOPS_NORTH),
    SCENE_EXITS_END,
};

// clang-format off
struct ScenePlacement gValeInteriorsShops_Placements[] = {
    { SPRITE_OLD_MAN_WITH_WHITE_HAIR, 0x0, ACTOR_STAND,
        PIXELS(697), 0, PIXELS(468), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_OLD_WOMAN, 0x0, ACTOR_STAND,
        PIXELS(726), 0, PIXELS(486), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_MAN_IN_TEAL_SHIRT, 0x0, ACTOR_WANDER,
        PIXELS(815), 0, PIXELS(386), FACING_EAST, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_IN_GREEN_HEADSCARF, 0x0, ACTOR_STAND,
        PIXELS(841), 0, PIXELS(455), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, 0x0, ACTOR_STAND,
        PIXELS(403), 0, PIXELS(144), FACING_WEST, TALK_FACE_PARTY, 0 },
    { SPRITE_OLD_WOMAN, 0x0, ACTOR_STAND,
        PIXELS(331), 0, PIXELS(144), FACING_EAST, TALK_KEEP_FACING, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, 0x0, ACTOR_STAND,
        PIXELS(486), 0, PIXELS(136), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, 0x0, ACTOR_STAND,
        PIXELS(495), 0, PIXELS(453), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, 0x0, ACTOR_STAND,
        PIXELS(405), 0, PIXELS(421), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, 0x0, ACTOR_WANDER,
        PIXELS(99), 0, PIXELS(125), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, 0x0, ACTOR_STAND,
        PIXELS(768), 0, PIXELS(768), FACING_WEST, TALK_FACE_PARTY, 0 },
    { SPRITE_STRONGMAN, 0x0, ACTOR_STAND,
        PIXELS(392), 0, PIXELS(697), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_WITH_PONYTAIL, 0x0, ACTOR_STAND,
        PIXELS(72), 0, PIXELS(424), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_BALD_MAN_IN_RED, 0x0, ACTOR_STAND,
        PIXELS(232), 0, PIXELS(440), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_YOUNG_MAN_IN_GREEN, 0x0, ACTOR_STAND,
        PIXELS(264), 0, PIXELS(440), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gValeInteriorsShops_PlacementsAfterLeaving[] = {
    { SPRITE_OLD_MAN_WITH_WHITE_HAIR, 0x0, ACTOR_STAND,
        PIXELS(697), 0, PIXELS(468), FACING_SOUTHEAST + FACING_STEP, 3, 0 },
    { SPRITE_OLD_WOMAN, 0x0, ACTOR_STAND,
        PIXELS(726), 0, PIXELS(486), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_MAN_IN_TEAL_SHIRT, 0x0, ACTOR_WANDER,
        PIXELS(815), 0, PIXELS(386), FACING_EAST, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_IN_GREEN_HEADSCARF, 0x0, ACTOR_STAND,
        PIXELS(841), 0, PIXELS(455), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, 0x0, ACTOR_STAND,
        PIXELS(403), 0, PIXELS(144), FACING_WEST, TALK_FACE_PARTY, 0 },
    { SPRITE_OLD_WOMAN, 0x0, ACTOR_STAND,
        PIXELS(456), 0, PIXELS(156), FACING_EAST, TALK_KEEP_FACING, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, 0x0, ACTOR_STAND,
        PIXELS(486), 0, PIXELS(118), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, 0x0, ACTOR_STAND,
        PIXELS(495), 0, PIXELS(453), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, 0x0, ACTOR_STAND,
        PIXELS(405), 0, PIXELS(421), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, 0x0, ACTOR_WANDER,
        PIXELS(99), 0, PIXELS(125), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, 0x0, ACTOR_STAND,
        PIXELS(768), 0, PIXELS(768), FACING_WEST, TALK_FACE_PARTY, 0 },
    { SPRITE_STRONGMAN, 0x0, ACTOR_STAND,
        PIXELS(392), 0, PIXELS(697), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_WITH_PONYTAIL, 0x0, ACTOR_STAND,
        PIXELS(72), 0, PIXELS(424), FACING_SOUTH, 3, 0 },
    { SPRITE_BALD_MAN_IN_RED, 0x0, ACTOR_STAND,
        PIXELS(232), 0, PIXELS(440), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_YOUNG_MAN_IN_GREEN, 0x0, ACTOR_STAND,
        PIXELS(264), 0, PIXELS(440), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gValeInteriorsShops_PlacementsAfterKalay[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { MIA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_WHITE_HAIR, 0x0, ACTOR_STAND,
        PIXELS(697), 0, PIXELS(468), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_OLD_WOMAN, 0x0, ACTOR_STAND,
        PIXELS(726), 0, PIXELS(486), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_MAN_IN_TEAL_SHIRT, 0x0, (s32)gValeInteriorsShops_FatherPacingScript,
        PIXELS(819), 0, PIXELS(405), FACING_WEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_IN_GREEN_HEADSCARF, 0x0, ACTOR_STAND,
        PIXELS(841), 0, PIXELS(370), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, 0x0, ACTOR_STAND,
        PIXELS(403), 0, PIXELS(144), FACING_WEST, TALK_FACE_PARTY, 0 },
    { SPRITE_OLD_WOMAN, 0x0, ACTOR_STAND,
        PIXELS(456), 0, PIXELS(156), FACING_EAST, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, 0x0, ACTOR_STAND,
        PIXELS(494), 0, PIXELS(112), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, 0x0, ACTOR_STAND,
        PIXELS(495), 0, PIXELS(453), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, 0x0, ACTOR_STAND,
        PIXELS(405), 0, PIXELS(421), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, 0x0, ACTOR_WANDER,
        PIXELS(99), 0, PIXELS(125), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, 0x0, ACTOR_STAND,
        PIXELS(768), 0, PIXELS(768), FACING_WEST, TALK_FACE_PARTY, 0 },
    { SPRITE_STRONGMAN, 0x0, ACTOR_STAND,
        PIXELS(392), 0, PIXELS(697), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_WITH_PONYTAIL, 0x0, ACTOR_STAND,
        PIXELS(72), 0, PIXELS(424), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_BALD_MAN_IN_RED, 0x0, ACTOR_STAND,
        PIXELS(232), 0, PIXELS(440), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_YOUNG_MAN_IN_GREEN, 0x0, ACTOR_STAND,
        PIXELS(264), 0, PIXELS(440), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 280, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 280, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 280, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SCENE_TABLE_END },
};

struct SceneEvent gValeInteriorsShops_StormEvents[] = {
    { EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveToValeWest) },
    { EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveToValeNorth) },
    { EVENT_TOUCH, 3, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveToValeGateWest) },
    { EVENT_TOUCH, 4, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveToValeGateSouth) },
    { EVENT_TOUCH, 5, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveToValeSouth) },
    { EVENT_TOUCH, 6, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveToValeEast) },
    { EVENT_TOUCH, 7, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveToValeGateEast) },
    { EVENT_TOUCH, 8, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveToValeGateNorth) },
    { SEARCH(SEARCH_BOOKCASE), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBooksAboutContinents)) },
    { SEARCH(SEARCH_BOOKCASE), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookLawsOfVale)) },
    { SEARCH(SEARCH_BOOKCASE) | EVENT_TOUCH_FACING | FACING_NORTH, 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookPowerOfNature)) },
    { SEARCH(SEARCH_BOOKCASE) | EVENT_TOUCH_FACING | FACING_NORTH, 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookPsynergyStone)) },
    { SEARCH(SEARCH_BOOKCASE) | EVENT_TOUCH_FACING | FACING_NORTH, 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeAuntNotebook)) },
    { SEARCH(SEARCH_BOOKCASE) | EVENT_TOUCH_FACING | FACING_NORTH, 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookMakingGreatVillage)) },
    { SEARCH(SEARCH_BOOKCASE), 206, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeNotebookStudyOfAlchemy)) },
    { SEARCH(SEARCH_BOOKCASE), 207, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookAncientLemurians)) },
    { SEARCH(SEARCH_BOOKCASE), 208, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookFountainOfTolbi)) },
    { SEARCH(SEARCH_BOOKCASE), 209, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeEncyclopediaOfAlchemy)) },
    { SEARCH(SEARCH_BOOKCASE), 210, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookLegendOfAleph)) },
    { SEARCH(SEARCH_BOOKCASE), 211, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookBeingAdept)) },
    { SEARCH(SEARCH_BARREL), 100, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgVaultButDidntFindAnything)) },
    { SEARCH(SEARCH_JAR), 101, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeJarEmpty)) },
    { SEARCH(SEARCH_JAR), 102, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeJarEmpty)) },
    { SEARCH(SEARCH_JAR), 103, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeJarEmpty)) },
    { SEARCH(SEARCH_JAR), 104, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeJarEmpty)) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gValeInteriorsShops_Events[] = {
    { EVENT_TALK, ACTOR_GARETS_GRANDFATHER, CONDITION_ALWAYS, MSGNO(MsgValeAtTheVeryLeastVisitorsShould) },
    { EVENT_TALK, ACTOR_GARETS_GRANDMOTHER, CONDITION_ALWAYS, MSGNO(MsgValeIHearKradenHasGuestsAgain) },
    { EVENT_TALK, ACTOR_GARETS_FATHER, CONDITION_ALWAYS, MSGNO(MsgValeGaretHasGrownQuiteStrongIn) },
    { EVENT_TALK, ACTOR_GARETS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgValeYouAndGaretAreOlderNow) },
    { EVENT_TALK, ACTOR_JENNAS_GRANDFATHER, CONDITION_ALWAYS, MSGNO(MsgValeMyGranddaughterJennaIsFinallyHappy) },
    { EVENT_TALK, ACTOR_JENNAS_GRANDMOTHER, CONDITION_ALWAYS, MSGNO(MsgValeIHopeYouTwoWontBe) },
    { EVENT_TALK, ACTOR_WOMAN_ON_THE_SPITTING_IMAGE, CONDITION_ALWAYS, MSGNO(MsgValeJennaIsTheSpittingImageOf) },
    { EVENT_TALK, ACTOR_MAN_ON_KRADENS_STUDIES, CONDITION_ALWAYS, EVENT_SCRIPT(Event_AskAboutKradensStudies) },
    { EVENT_TALK, ACTOR_WOMAN_ON_VISITORS_IN_TOWN, CONDITION_ALWAYS, MSGNO(MsgValeIfThereAreVisitorsInTown) },
    { EVENT_TALK, ACTOR_YOUNG_MAN_DREAMING_OF_LEAVING, CONDITION_ALWAYS, MSGNO(MsgValeDoYouEverDreamOfSeeing) },
    { EVENT_TALK, ACTOR_WOMAN_ON_PERMISSION_TO_LEAVE, CONDITION_ALWAYS, MSGNO(MsgValeFewPeopleOutsideOfValePossess) },
    { EVENT_TALK, ACTOR_STRONGMAN_SCARING_VISITORS, CONDITION_ALWAYS, EVENT_SCRIPT(Event_AskToScareVisitors) },
    { EVENT_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToWeaponMerchant) },
    { EVENT_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToArmorMerchant) },
    { EVENT_TALK, ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToItemMerchant) },
    { EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveToValeWest) },
    { EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveToValeNorth) },
    { EVENT_TOUCH, 3, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveToValeGateWest) },
    { EVENT_TOUCH, 4, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveToValeGateSouth) },
    { EVENT_TOUCH, 5, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveToValeSouth) },
    { EVENT_TOUCH, 6, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveToValeEast) },
    { EVENT_TOUCH, 7, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveToValeGateEast) },
    { EVENT_TOUCH, 8, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveToValeGateNorth) },
    { SEARCH(SEARCH_BOOKCASE), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBooksAboutContinents)) },
    { SEARCH(SEARCH_BOOKCASE), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookLawsOfVale)) },
    { SEARCH(SEARCH_BOOKCASE) | EVENT_TOUCH_FACING | FACING_NORTH, 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookPowerOfNature)) },
    { SEARCH(SEARCH_BOOKCASE) | EVENT_TOUCH_FACING | FACING_NORTH, 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookPsynergyStone)) },
    { SEARCH(SEARCH_BOOKCASE) | EVENT_TOUCH_FACING | FACING_NORTH, 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeAuntNotebook)) },
    { SEARCH(SEARCH_BOOKCASE) | EVENT_TOUCH_FACING | FACING_NORTH, 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookMakingGreatVillage)) },
    { SEARCH(SEARCH_BARREL), 100, FLAG_VALE_INTERIORS_SHOPS_BARREL_ANTIDOTE, FIND_ITEM(ITEM_ANTIDOTE) },
    { SEARCH(SEARCH_JAR), 101, FLAG_VALE_INTERIORS_SHOPS_JAR_6_COINS, FIND_COINS(6) },
    { SEARCH(SEARCH_JAR), 102, FLAG_VALE_INTERIORS_SHOPS_JAR_2_COINS, FIND_COINS(2) },
    { SEARCH(SEARCH_JAR), 103, FLAG_VALE_INTERIORS_SHOPS_JAR_SMOKE_BOMB, FIND_ITEM(ITEM_SMOKE_BOMB) },
    { SEARCH(SEARCH_JAR), 104, FLAG_VALE_INTERIORS_SHOPS_JAR_3_COINS, FIND_COINS(3) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gValeInteriorsShops_EventsAfterLeaving[] = {
    { EVENT_TALK, ACTOR_GARETS_GRANDFATHER, CONDITION_ALWAYS, MSGNO(MsgValeIsaacPleaseTakeCareOfMy) },
    { EVENT_TALK, ACTOR_GARETS_GRANDMOTHER, CONDITION_ALWAYS, MSGNO(MsgValeReceivingAQuestDirectlyFromThe) },
    { EVENT_TALK, ACTOR_GARETS_FATHER, CONDITION_ALWAYS, MSGNO(MsgValeImProudOfYouGaretMy) },
    { EVENT_TALK, ACTOR_GARETS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgValeAsMothersDoraAndIWill) },
    { EVENT_TALK, ACTOR_JENNAS_GRANDFATHER, CONDITION_ALWAYS, MSGNO(MsgValeIHeardThatJennaBrotherFelix) },
    { EVENT_TALK, ACTOR_JENNAS_GRANDMOTHER, CONDITION_ALWAYS, MSGNO(MsgValePleaseReturnMyPreciousGranddaughterJenna) },
    { EVENT_TALK, ACTOR_WOMAN_ON_THE_SPITTING_IMAGE, CONDITION_ALWAYS, MSGNO(MsgValeAfterJennaWasTakenGrandmotherGrew) },
    { EVENT_TALK, ACTOR_MAN_ON_KRADENS_STUDIES, CONDITION_ALWAYS, MSGNO(MsgValeItsBecauseKradenWasStudyingAlchemy) },
    { EVENT_TALK, ACTOR_WOMAN_ON_VISITORS_IN_TOWN, CONDITION_ALWAYS, EVENT_SCRIPT(Event_AskAboutStrangePowers) },
    { EVENT_TALK, ACTOR_YOUNG_MAN_DREAMING_OF_LEAVING, CONDITION_ALWAYS, MSGNO(MsgValeImSoJealousOfYouBeing) },
    { EVENT_TALK, ACTOR_WOMAN_ON_PERMISSION_TO_LEAVE, CONDITION_ALWAYS, MSGNO(MsgValeOnceYouLeaveTownTakeCare) },
    { EVENT_TALK, ACTOR_STRONGMAN_SCARING_VISITORS, CONDITION_ALWAYS, MSGNO(MsgValeIHadNoIdeaThatSuch) },
    { EVENT_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToWeaponMerchant) },
    { EVENT_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToArmorMerchant) },
    { EVENT_TALK, ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToItemMerchant) },
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
    { EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveToValeWest) },
    { EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveToValeNorth) },
    { EVENT_TOUCH, 3, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveToValeGateWest) },
    { EVENT_TOUCH, 4, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveToValeGateSouth) },
    { EVENT_TOUCH, 5, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveToValeSouth) },
    { EVENT_TOUCH, 6, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveToValeEast) },
    { EVENT_TOUCH, 7, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveToValeGateEast) },
    { EVENT_TOUCH, 8, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveToValeGateNorth) },
    { SEARCH(SEARCH_BARREL), 100, FLAG_VALE_INTERIORS_SHOPS_BARREL_ANTIDOTE, FIND_ITEM(ITEM_ANTIDOTE) },
    { SEARCH(SEARCH_JAR), 101, FLAG_VALE_INTERIORS_SHOPS_JAR_6_COINS, FIND_COINS(6) },
    { SEARCH(SEARCH_JAR), 102, FLAG_VALE_INTERIORS_SHOPS_JAR_2_COINS, FIND_COINS(2) },
    { SEARCH(SEARCH_JAR), 103, FLAG_VALE_INTERIORS_SHOPS_JAR_SMOKE_BOMB, FIND_ITEM(ITEM_SMOKE_BOMB) },
    { SEARCH(SEARCH_JAR), 104, FLAG_VALE_INTERIORS_SHOPS_JAR_3_COINS, FIND_COINS(3) },
    { SEARCH(SEARCH_BOOKCASE), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBooksAboutContinents)) },
    { SEARCH(SEARCH_BOOKCASE), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookLawsOfVale)) },
    { SEARCH(SEARCH_BOOKCASE) | EVENT_TOUCH_FACING | FACING_NORTH, 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookPowerOfNature)) },
    { SEARCH(SEARCH_BOOKCASE) | EVENT_TOUCH_FACING | FACING_NORTH, 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookPsynergyStone)) },
    { SEARCH(SEARCH_BOOKCASE) | EVENT_TOUCH_FACING | FACING_NORTH, 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeAuntNotebook)) },
    { SEARCH(SEARCH_BOOKCASE) | EVENT_TOUCH_FACING | FACING_NORTH, 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookMakingGreatVillage)) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gValeInteriorsShops_EventsAfterKalay[] = {
    { EVENT_TALK, ACTOR_GARETS_GRANDFATHER, CONDITION_ALWAYS, MSGNO(MsgValeMiaIsSuchANiceGirl) },
    { EVENT_TALK, ACTOR_GARETS_GRANDMOTHER, CONDITION_ALWAYS, MSGNO(MsgValeThatIvanSeemsToBeA) },
    { EVENT_TALK, ACTOR_GARETS_FATHER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_AskAboutDora) },
    { EVENT_TALK, ACTOR_GARETS_MOTHER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_WorryAboutTravelersHealth) },
    { EVENT_TALK, ACTOR_JENNAS_GRANDFATHER, CONDITION_ALWAYS, MSGNO(MsgValeOhSoYouStillHaventRescued) },
    { EVENT_TALK, ACTOR_JENNAS_GRANDMOTHER, CONDITION_ALWAYS, MSGNO(MsgValeIGetOverColdsQuicklyBut) },
    { EVENT_TALK, ACTOR_WOMAN_ON_THE_SPITTING_IMAGE, CONDITION_ALWAYS, MSGNO(MsgValeAColdIsGoingAroundI) },
    { EVENT_TALK, ACTOR_MAN_ON_KRADENS_STUDIES, CONDITION_ALWAYS, MSGNO(MsgValeWhatWasThatAlchemyThatKraden) },
    { EVENT_TALK, ACTOR_WOMAN_ON_VISITORS_IN_TOWN, CONDITION_ALWAYS, MSGNO(MsgValeEarthquakesStillStrikeOnceInA) },
    { EVENT_TALK, ACTOR_YOUNG_MAN_DREAMING_OF_LEAVING, CONDITION_ALWAYS, MSGNO(MsgValeTheMountainIsApparentlyStillCasting) },
    { EVENT_TALK, ACTOR_WOMAN_ON_PERMISSION_TO_LEAVE, CONDITION_ALWAYS, MSGNO(MsgValeTheNorthwestFaceOfMtAleph) },
    { EVENT_TALK, ACTOR_STRONGMAN_SCARING_VISITORS, CONDITION_ALWAYS, MSGNO(MsgValeTheGreatHealersAreTooAfraid) },
    { EVENT_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToWeaponMerchant) },
    { EVENT_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToArmorMerchant) },
    { EVENT_TALK, ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToItemMerchant) },
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
    { EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveToValeWest) },
    { EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveToValeNorth) },
    { EVENT_TOUCH, 3, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveToValeGateWest) },
    { EVENT_TOUCH, 4, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveToValeGateSouth) },
    { EVENT_TOUCH, 5, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveToValeSouth) },
    { EVENT_TOUCH, 6, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveToValeEast) },
    { EVENT_TOUCH, 7, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveToValeGateEast) },
    { EVENT_TOUCH, 8, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveToValeGateNorth) },
    { SEARCH(SEARCH_BARREL), 100, FLAG_VALE_INTERIORS_SHOPS_BARREL_ANTIDOTE, FIND_ITEM(ITEM_ANTIDOTE) },
    { SEARCH(SEARCH_JAR), 101, FLAG_VALE_INTERIORS_SHOPS_JAR_6_COINS, FIND_COINS(6) },
    { SEARCH(SEARCH_JAR), 102, FLAG_VALE_INTERIORS_SHOPS_JAR_2_COINS, FIND_COINS(2) },
    { SEARCH(SEARCH_JAR), 103, FLAG_VALE_INTERIORS_SHOPS_JAR_SMOKE_BOMB, FIND_ITEM(ITEM_SMOKE_BOMB) },
    { SEARCH(SEARCH_JAR), 104, FLAG_VALE_INTERIORS_SHOPS_JAR_3_COINS, FIND_COINS(3) },
    { SEARCH(SEARCH_BOOKCASE), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBooksAboutContinents)) },
    { SEARCH(SEARCH_BOOKCASE), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookLawsOfVale)) },
    { SEARCH(SEARCH_BOOKCASE) | EVENT_TOUCH_FACING | FACING_NORTH, 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookPowerOfNature)) },
    { SEARCH(SEARCH_BOOKCASE) | EVENT_TOUCH_FACING | FACING_NORTH, 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookPsynergyStone)) },
    { SEARCH(SEARCH_BOOKCASE) | EVENT_TOUCH_FACING | FACING_NORTH, 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeAuntNotebook)) },
    { SEARCH(SEARCH_BOOKCASE) | EVENT_TOUCH_FACING | FACING_NORTH, 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookMakingGreatVillage)) },
    { SCENE_EVENTS_END },
};
// clang-format on
