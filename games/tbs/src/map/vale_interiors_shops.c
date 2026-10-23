/// @file
/// Vale (MapId_ValeInteriorsShops): the insides of the village's houses and
/// shops, all on one map. Each room is a warp destination with a camera box of
/// its own, named for the door it is entered by
/// (VALE_INTERIORS_SHOPS_FROM_VALE_WEST and the rest), and its door takes the
/// warp of the same number back out: four open onto the village (MapId_Vale),
/// and four onto the part of Vale with the Psynergy Stone (MapId_ValeGate),
/// as the warps' names say. Garet's family lives in the room entered from
/// VALE_INTERIORS_SHOPS_FROM_VALE_EAST: his grandfather the mayor, his
/// grandmother and his parents. Jenna's grandparents live in the room entered
/// from VALE_INTERIORS_SHOPS_FROM_VALE_NORTH, and the one entered from
/// VALE_INTERIORS_SHOPS_FROM_VALE_GATE_WEST holds the item, weapon and armor
/// shops, each opened across its counter. The object event templates and map
/// events follow the story:
///
/// - The stormy night of the prologue (FLAG_VALE_STORM). Everyone is out:
///   ValeInteriorsShops_Initialize removes every resident and flashes the
///   lightning outside, and the doors out to the village lead into the
///   storm's map (MapId_ValeStorm) instead (ValeInteriorsShops_GetWarps). The
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
///   first time the party enters Garet's house, EventScript_GaretComesHome
///   plays (FLAG_GARET_CAME_HOME): his parents are wondering whether to look
///   for him when his grandmother sees him, the family greets him, Ivan and Mia
///   introduce themselves ("I am Mia, from Imil."), and his father is left
///   pacing. The villagers now worry about Dora, ill since Isaac left.
///
/// While Garet's grandfather dozes in EventScript_GaretComesHome, three letters
/// Z (LOCALID_FIRST_SLEEP_Z and the two after it) rise from him, one at each of
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
#include "unit/overworld/overworld.h"
#include "unit/overworld/field_services.h"
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

/// The placed object events, numbered in the order of the object event template
/// tables, which all place them alike;
/// gValeInteriorsShops_ObjectEventTemplatesAfterKalay adds Garet, Ivan and Mia,
/// who keep their characters' ids, and the drifting sprites. The villagers'
/// notes quote what they say on the day the story begins.
enum ValeInteriorsShopsLocalId {
  /// Garet's grandfather, the mayor, an old man with white hair and a cane
  /// ("your grandpa's the mayor", vale.c). "Isaac, please take care of my
  /// grandson, Garet."
  /// ★ Named from the game: "please take care of my grandson"
  /// (MsgValeIsaacPleaseTakeCareOfMy)
  LOCALID_GARETS_GRANDFATHER = LOCALID_FIRST_PLACED,
  /// Garet's grandmother, an old woman with white hair. "Grandma! What's the
  /// matter? Are you choking?"
  /// ★ Named from the game: "Are you choking" (MsgValeHey2)
  LOCALID_GARETS_GRANDMOTHER,
  /// Garet's father, a man with brown hair in a teal shirt.
  /// "I'm proud of you, Garet. My own son...on such an important quest!"
  /// ★ Named from the game: "My own son" (MsgValeImProudOfYouGaretMy)
  LOCALID_GARETS_FATHER,
  /// Garet's mother, a woman with orange hair under a green headscarf. "As
  /// mothers, Dora and I will always worry about you."
  /// ★ Named from the game: "he is my son" (MsgValeIAmConcernedForGaretBut)
  LOCALID_GARETS_MOTHER,
  /// Jenna's grandfather, a bent old man with a white beard and a cane. "My
  /// granddaughter, Jenna, is finally happy again..."
  /// ★ Named from the game: "My granddaughter,"
  /// (MsgValeMyGranddaughterJennaIsFinallyHappy)
  LOCALID_JENNAS_GRANDFATHER,
  /// Jenna's grandmother, an old woman with white hair. "Please return my
  /// precious granddaughter, Jenna, to me." Once Jenna is taken,
  /// LOCALID_WOMAN_ON_THE_SPITTING_IMAGE says she "grew sick with despair", and
  /// she lies in bed.
  /// ★ Named from the game: "Please return my precious granddaughter"
  /// (MsgValePleaseReturnMyPreciousGranddaughterJenna)
  LOCALID_JENNAS_GRANDMOTHER,
  /// A woman in a green dress in Jenna's grandparents' house. "Jenna is the
  /// spitting image of her mother, whom we lost that day..."
  /// ★ Named from the game: "is the spitting image of her mother"
  /// (MsgValeJennaIsTheSpittingImageOf)
  LOCALID_WOMAN_ON_THE_SPITTING_IMAGE,
  /// A burly grey-haired man: "So Kraden came to Vale to study Mt. Aleph,
  /// right?"
  /// ★ Named from the game: "It's because Kraden was studying Alchemy..."
  /// (MsgValeItsBecauseKradenWasStudyingAlchemy)
  LOCALID_MAN_ON_KRADENS_STUDIES,
  /// A woman in a green dress: "If there are visitors in town, we can't use our
  /// Psynergy."
  /// ★ Named from the game: "If there are visitors in town, we can't use our
  /// Psynergy." (MsgValeIfThereAreVisitorsInTown)
  LOCALID_WOMAN_ON_VISITORS_IN_TOWN,
  /// A young man with brown hair: "Do you ever dream of seeing the world
  /// outside the village?"
  /// ★ Named from the game: "Do you ever dream of seeing the world outside
  /// the village?" (MsgValeDoYouEverDreamOfSeeing)
  LOCALID_YOUNG_MAN_DREAMING_OF_LEAVING,
  /// A woman with fair hair: "Few people outside of Vale possess the power of
  /// Psynergy. That's why we need permission to leave the village..."
  /// ★ Named from the game: "we need permission to leave the village"
  /// (MsgValeFewPeopleOutsideOfValePossess)
  LOCALID_WOMAN_ON_PERMISSION_TO_LEAVE,
  /// A strongman: "I hear old man Kraden's got some visitors. Let's go use our
  /// Psynergy to scare 'em!"
  /// ★ Named from the game: "Let's go use our"
  /// (MsgValeLetsScareKradensVisitors)
  LOCALID_STRONGMAN_SCARING_VISITORS,
  /// ★ Named from what the player sees: a young woman with a brown ponytail
  /// who opens the item shop across its counter, beneath the shelves of jars
  /// (its render)
  LOCALID_ITEM_MERCHANT,
  /// A bald man in a red tunic.
  /// ★ Named from the game: "Good weapons aren't just sharp."
  /// (MsgValeGoodWeaponsDrawOutStrength)
  LOCALID_WEAPON_MERCHANT,
  /// A young man with brown hair in a green shirt.
  /// ★ Named from the game: "Good armor isn't just sturdy."
  /// (MsgValeGoodArmorDrawsOutStrength)
  LOCALID_ARMOR_MERCHANT,
  /// Three letters Z that drift up from Garet's grandfather as he sleeps.
  /// ★ Named from what the player sees: a white letter Z,
  /// spr_280.png (its render)
  LOCALID_FIRST_SLEEP_Z
};

/// Animations of the houses' sprites beyond enum ObjectEventAnimation, as their
/// animation tables and frames under graphics/sprite draw them.
enum ValeInteriorsShopsAnimation {
  /// Garet's grandfather standing with his eyes shut, asleep (spr_117.png).
  ANIM_GRANDFATHER_ASLEEP = 5,
  /// Garet's grandfather glancing quickly from side to side (spr_117.png).
  ANIM_GRANDFATHER_LOOK_ABOUT = 6,
  /// Jenna's grandmother lying in bed, only her head on the pillow
  /// (spr_111.png).
  ANIM_GRANDMOTHER_IN_BED = 5
};

// Forward declarations.
static void EventScript_GaretComesHome(void);
static void ValeInteriorsShops_SpawnSleepZ(void);

#include "common/object_facing.h"

/// Moves and grows a Z drifting up from Garet's sleeping grandfather.
///
/// Called each frame by gValeInteriorsShops_SleepZScript; the object event's
/// timer and work are its x and y steps in signed 8.8 pixels, set by
/// ValeInteriorsShops_SpawnSleepZ.
static s32
ValeInteriorsShops_AdvanceSleepZ(struct ObjectScriptContext *interpreter) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)interpreter;

  objectEvent->x.fixed += (s16)objectEvent->timer * 256;
  objectEvent->y.fixed += (s16)objectEvent->work * 256;
  objectEvent->scaleX += Q16_ONE / 40;
  objectEvent->scaleY += Q16_ONE / 40;
  objectEvent->timer += 5;
  objectEvent->work = (s16)objectEvent->work - 1;
  return 0;
}

const struct MapWarpDest *ValeInteriorsShops_GetWarpDests(void) {
  return gValeInteriorsShops_WarpDests;
}

const struct MapWarpRegion *ValeInteriorsShops_GetRegions(void) { return NULL; }

/// On the stormy night, the doors lead out into the storm.
const u32 *ValeInteriorsShops_GetWarps(void) {
  if (GameFlag_Test(FLAG_VALE_STORM) != 0)
    return gValeInteriorsShops_StormWarps;
  return gValeInteriorsShops_Warps;
}

/// Selects the object event templates for the day the story begins, after the
/// party has left Vale (FLAG_VALE_PARTY_LEFT), or after Kalay
/// (FLAG_KALAY_REACHED), and marks which of their object events are in view.
const struct ObjectEventTemplate *
ValeInteriorsShops_GetObjectEventTemplates(void) {
  struct ObjectEventTemplate *table;

  if (GameFlag_Test(FLAG_KALAY_REACHED)) {
    table = gValeInteriorsShops_ObjectEventTemplatesAfterKalay;
  } else if (GameFlag_Test(FLAG_VALE_PARTY_LEFT)) {
    table = gValeInteriorsShops_ObjectEventTemplatesAfterLeaving;
  } else {
    table = gValeInteriorsShops_ObjectEventTemplates;
  }
  Map_AssignViewFlags(table);
  return table;
}

/// Selects the map events for the current visit to Vale: the stormy night's
/// (FLAG_VALE_STORM), after Kalay, after the party has left, or the day the
/// story begins.
const struct MapEvent *ValeInteriorsShops_GetEvents(void) {
  if (GameFlag_Test(FLAG_VALE_STORM) != 0)
    return gValeInteriorsShops_StormEvents;
  if (GameFlag_Test(FLAG_KALAY_REACHED) != 0)
    return gValeInteriorsShops_EventsAfterKalay;
  if (GameFlag_Test(FLAG_VALE_PARTY_LEFT) != 0)
    return gValeInteriorsShops_EventsAfterLeaving;
  return gValeInteriorsShops_Events;
}

/// A burly grey-haired man asks whether Isaac knew why Kraden came to Vale.
/// He stands in a house of Vale on the day the story begins. Kraden came to
/// study Mt. Aleph.
///
/// Reached by talking to LOCALID_MAN_ON_KRADENS_STUDIES on the day the story
/// begins.
static void EventScript_AskAboutKradensStudies(void) {
  FieldScript_Start();
  FieldScript_SetMessageId(MsgValeKradenCameToStudyMt);
  ObjectEvent_LinkPair(LOCALID_PARTY_LEADER, LOCALID_MAN_ON_KRADENS_STUDIES, 6);
  FieldScript_AskYesNoAndShowResponse(LOCALID_MAN_ON_KRADENS_STUDIES, 0);
  FieldScript_End();
}

/// A strongman asks Isaac to help scare Kraden's visitors with Psynergy. He
/// stands in a house of Vale on the day the story begins.
///
/// Reached by talking to LOCALID_STRONGMAN_SCARING_VISITORS on the day the
/// story begins.
static void EventScript_AskToScareVisitors(void) {
  FieldScript_Start();
  FieldScript_SetMessageId(MsgValeLetsScareKradensVisitors);
  ObjectEvent_LinkPair(LOCALID_PARTY_LEADER, LOCALID_STRONGMAN_SCARING_VISITORS,
                       6);
  FieldScript_AskYesNoAndShowResponse(LOCALID_STRONGMAN_SCARING_VISITORS, 0);
  FieldScript_End();
}

/// In Vale's shops after the party has left Vale, the item merchant gives
/// Isaac a Herb as a farewell gift, and afterwards urges him to save Jenna.
///
/// Called by EventScript_TalkToItemMerchant away from the counter, after the
/// party has left and before Kalay. Gives the Herb and sets
/// FLAG_VALE_FAREWELL_HERB_RECEIVED once; afterwards the merchant turns to
/// watch the leader.
static void ValeInteriorsShops_GiveFarewellHerb(void) {
  s32 gift;

  FieldScript_Start();
  if (GameFlag_IsSet(FLAG_VALE_FAREWELL_HERB_RECEIVED) != 0) {
    FieldScript_SetMessageId(MsgValeYouMustSaveJenna);
    Script_ShowMessage(LOCALID_ITEM_MERCHANT, 0);
    Object_SetTargetAndCallback(LOCALID_ITEM_MERCHANT,
                                OBJ_EVENT_LINK_KEEP_MOTION |
                                    LOCALID_PARTY_LEADER,
                                gValeInteriorsShops_FaceTargetScript);
  } else {
    gift = MsgValeThisIsMyFarewellGift;
    FieldScript_SetMessageId(gift);
    Script_ShowMessageAndWait(LOCALID_ITEM_MERCHANT, 0, 20);
    FieldMessage_Show((gift + 1), 1);
    PartyInventory_GiveItem(ITEM_HERB, 0);
    GameFlag_Set(FLAG_VALE_FAREWELL_HERB_RECEIVED);
  }
  FieldScript_End();
}

/// A woman in a green dress asks whether the people who caused the eruption
/// had strange powers, in a house of Vale after the party has left.
///
/// Reached by talking to LOCALID_WOMAN_ON_VISITORS_IN_TOWN after the party has
/// left Vale.
static void EventScript_AskAboutStrangePowers(void) {
  FieldScript_Start();
  FieldScript_SetMessageId(MsgValeTheCulpritsHadStrangePowers);
  FieldScript_AskYesNoAndShowResponse(LOCALID_WOMAN_ON_VISITORS_IN_TOWN, 0);
  FieldScript_End();
}

/// Garet's father asks whether Isaac heard them talking about Dora. He is in
/// Garet's house in Vale, back from Kalay.
///
/// Reached by talking to Garet's father after Kalay.
static void EventScript_AskAboutDora(void) {
  FieldScript_Start();
  FieldScript_SetMessageId(MsgValeDidYouHearAboutDora);
  FieldScript_AskYesNoAndShowResponse(LOCALID_GARETS_FATHER, 0);
  FieldScript_End();
}

/// Garet's mother hopes the travelers didn't get sick on the way. She is in
/// Garet's house in Vale, back from Kalay.
///
/// Reached by talking to Garet's mother after Kalay.
static void EventScript_WorryAboutTravelersHealth(void) {
  FieldScript_Start();
  FieldScript_SetMessageId(MsgValeHopeDidntGet);
  FieldScript_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | LOCALID_GARETS_MOTHER,
                                 0);
  FieldScript_End();
}

/// Leaves a house through its door: the warp sound plays and the screen
/// closes, stopping the lightning first on the stormy night.
static void ValeInteriorsShops_RequestWindowWarp(s32 warpNumber) {
  if (GameFlag_Test(FLAG_VALE_STORM) != 0)
    Weather_StopLightning();
  Sound_PlaySong(SE_MAP_EXIT);
  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  gFieldWork->transitionFrames = 16;
  FieldScript_FinishScreenTransition();
  FieldScript_WaitTransitionFrames();
  Engine_ScriptRequestWarp(warpNumber);
}

// The rooms' doors: stepping on a door's cell, whose metatile behavior is its
// warp's number, takes that warp back out, in every table. On the stormy night
// the warps to MapId_Vale lead to the storm's map (MapId_ValeStorm)
// instead.

/// Leaves the house where a young man dreams of leaving. It's the room entered
/// from VALE_INTERIORS_SHOPS_FROM_VALE_WEST, where
/// LOCALID_YOUNG_MAN_DREAMING_OF_LEAVING wanders.
static void EventScript_LeaveToValeWest(void) {
  ValeInteriorsShops_RequestWindowWarp(VALE_INTERIORS_SHOPS_TO_VALE_WEST);
}

/// Leaves Jenna's grandparents' house.
static void EventScript_LeaveToValeNorth(void) {
  ValeInteriorsShops_RequestWindowWarp(VALE_INTERIORS_SHOPS_TO_VALE_NORTH);
}

/// Leaves the empty house. It's the room entered from
/// VALE_INTERIORS_SHOPS_FROM_VALE_GATE_NORTH, which no object event template
/// table puts anyone in.
static void EventScript_LeaveToValeGateWest(void) {
  ValeInteriorsShops_RequestWindowWarp(VALE_INTERIORS_SHOPS_TO_VALE_GATE_WEST);
}

/// Leaves Vale's shops.
static void EventScript_LeaveToValeGateSouth(void) {
  ValeInteriorsShops_RequestWindowWarp(VALE_INTERIORS_SHOPS_TO_VALE_GATE_SOUTH);
}

/// Leaves the house where villagers talk of Kraden and his visitors. It's the
/// room entered from VALE_INTERIORS_SHOPS_FROM_VALE_SOUTH, where
/// LOCALID_MAN_ON_KRADENS_STUDIES and LOCALID_WOMAN_ON_VISITORS_IN_TOWN stand.
static void EventScript_LeaveToValeSouth(void) {
  ValeInteriorsShops_RequestWindowWarp(VALE_INTERIORS_SHOPS_TO_VALE_SOUTH);
}

/// Leaves Garet's house.
static void EventScript_LeaveToValeEast(void) {
  ValeInteriorsShops_RequestWindowWarp(VALE_INTERIORS_SHOPS_TO_VALE_EAST);
}

/// Leaves the house where a strongman plans to scare Kraden's visitors. It's
/// the room entered from VALE_INTERIORS_SHOPS_FROM_VALE_GATE_SOUTH, where
/// LOCALID_STRONGMAN_SCARING_VISITORS stands.
static void EventScript_LeaveToValeGateEast(void) {
  ValeInteriorsShops_RequestWindowWarp(VALE_INTERIORS_SHOPS_TO_VALE_GATE_EAST);
}

/// Leaves the house where a woman talks of permission to leave. It's the room
/// entered from VALE_INTERIORS_SHOPS_FROM_VALE_GATE_EAST, where
/// LOCALID_WOMAN_ON_PERMISSION_TO_LEAVE stands.
static void EventScript_LeaveToValeGateNorth(void) {
  ValeInteriorsShops_RequestWindowWarp(VALE_INTERIORS_SHOPS_TO_VALE_GATE_NORTH);
}

/// Arriving in a house of Vale: on the stormy night the residents are gone and
/// lightning flashes outside; back from Kalay, entering Garet's house the first
/// time plays his homecoming.
///
/// The map's initialize entry. Runs EventScript_GaretComesHome from
/// VALE_INTERIORS_SHOPS_FROM_VALE_EAST after Kalay until FLAG_GARET_CAME_HOME
/// is set. From VALE_INTERIORS_SHOPS_FROM_VALE_NORTH, once the party has left
/// Vale, Jenna's grandmother lies sick in bed.
s32 ValeInteriorsShops_Initialize(void) {
  struct ObjectEvent *objectEvent;
  struct EventWork **eventSlot = &gFieldWork;

  (*eventSlot)->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  // On the stormy night every resident is out, and lightning flashes outside.
  if (GameFlag_Test(FLAG_VALE_STORM) != 0) {
    Engine_ObjectEventSetPosition(LOCALID_GARETS_GRANDFATHER, 0, 0);
    Engine_ObjectEventSetPosition(LOCALID_GARETS_GRANDMOTHER, 0, 0);
    Engine_ObjectEventSetPosition(LOCALID_GARETS_FATHER, 0, 0);
    Engine_ObjectEventSetPosition(LOCALID_GARETS_MOTHER, 0, 0);
    Engine_ObjectEventSetPosition(LOCALID_JENNAS_GRANDFATHER, 0, 0);
    Engine_ObjectEventSetPosition(LOCALID_JENNAS_GRANDMOTHER, 0, 0);
    Engine_ObjectEventSetPosition(LOCALID_WOMAN_ON_THE_SPITTING_IMAGE, 0, 0);
    Engine_ObjectEventSetPosition(LOCALID_MAN_ON_KRADENS_STUDIES, 0, 0);
    Engine_ObjectEventSetPosition(LOCALID_WOMAN_ON_VISITORS_IN_TOWN, 0, 0);
    Engine_ObjectEventSetPosition(LOCALID_YOUNG_MAN_DREAMING_OF_LEAVING, 0, 0);
    Engine_ObjectEventSetPosition(LOCALID_WOMAN_ON_PERMISSION_TO_LEAVE, 0, 0);
    Engine_ObjectEventSetPosition(LOCALID_STRONGMAN_SCARING_VISITORS, 0, 0);
    Engine_ObjectEventSetPosition(LOCALID_ITEM_MERCHANT, 0, 0);
    Engine_ObjectEventSetPosition(LOCALID_WEAPON_MERCHANT, 0, 0);
    Engine_ObjectEventSetPosition(LOCALID_ARMOR_MERCHANT, 0, 0);
    Weather_StartLightning();
    (*(struct LightningWork **)((u8 *)eventSlot + Ram_LightningWorkOffset))
        ->loud = 1;
    Weather_LastLightning();
    WaitFrames(30);
    FieldScript_StartScreenTransition();
    FieldScript_WaitTransitionFrames();
    Weather_ResumeLightning();
  }
  // The returning party gets the household reunion only on its first visit.
  if (GameFlag_Test(FLAG_KALAY_REACHED) != 0) {
    if (gGameState.warpDest == VALE_INTERIORS_SHOPS_FROM_VALE_EAST) {
      if (GameFlag_Test(FLAG_GARET_CAME_HOME) == 0) {
        EventScript_GaretComesHome();
      }
    }
    ObjectEvent_GetById(LOCALID_GARETS_FATHER)->collisionFlags |= 0x80;
  }
  // Once Jenna is taken, her grandmother lies sick in bed.
  if (gGameState.warpDest == VALE_INTERIORS_SHOPS_FROM_VALE_NORTH) {
    if (GameFlag_Test(FLAG_VALE_PARTY_LEFT) != 0) {
      Engine_ObjectEventSetPosition(LOCALID_JENNAS_GRANDMOTHER, PIXELS(454),
                                    PIXELS(150));
      objectEvent = ObjectEvent_GetById(LOCALID_JENNAS_GRANDMOTHER);
      ObjectEvent_SetSpriteFlags(objectEvent, 0);
      Engine_ObjectEventSetAnimation(LOCALID_JENNAS_GRANDMOTHER,
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
static void EventScript_TalkToWeaponMerchant(void) {
  struct ObjectEvent *leader;

  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);

  if (FACING_IS_NORTH(leader->facing)) {
    Shop_Run(1, LOCALID_WEAPON_MERCHANT);
    return;
  }

  FieldScript_Start();
  if (GameFlag_Test(FLAG_KALAY_REACHED) != 0) {
    FieldScript_SetMessageId(MsgValeYouCameBack);
    FieldScript_AskYesNoAndShowResponse(LOCALID_WEAPON_MERCHANT, 0);
  } else {
    if (GameFlag_Test(FLAG_VALE_PARTY_LEFT) != 0) {
      FieldScript_SetMessageId(MsgValeValeFeelsEmpty);
    } else {
      FieldScript_SetMessageId(MsgValeGoodWeaponsDrawOutStrength);
    }
    FieldScript_ShowMessageAndWait(LOCALID_WEAPON_MERCHANT, 0);
  }
  FieldScript_End();
}

/// In Vale's shops, opens the armor shop across the counter, or the merchant
/// speaks.
///
/// Reached by talking to the armor merchant in every table but the stormy
/// night's; opens shop 2 when facing north.
static void EventScript_TalkToArmorMerchant(void) {
  struct ObjectEvent *objectEvent;

  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (FACING_IS_NORTH(objectEvent->facing)) {
    Shop_Run(2, LOCALID_ARMOR_MERCHANT);
  } else {
    FieldScript_Start();
    if (GameFlag_Test(FLAG_KALAY_REACHED) != 0) {
      FieldScript_SetMessageId(MsgValeTheRumorWasTrue);
    } else {
      if (GameFlag_Test(FLAG_VALE_PARTY_LEFT) != 0) {
        FieldScript_SetMessageId(MsgValeImNotSadJustGo);
      } else {
        FieldScript_SetMessageId(MsgValeGoodArmorDrawsOutStrength);
      }
    }
    FieldScript_ShowMessageAndWait(LOCALID_ARMOR_MERCHANT, 0);
    FieldScript_End();
  }
}

/// In Vale's shops, opens the item shop across the counter, or the merchant
/// speaks.
///
/// Reached by talking to the item merchant in every table but the stormy
/// night's; opens shop 3 when facing north. After the party has left and
/// before Kalay, ValeInteriorsShops_GiveFarewellHerb speaks instead.
static void EventScript_TalkToItemMerchant(void) {
  struct ObjectEvent *objectEvent;

  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (FACING_IS_NORTH(objectEvent->facing)) {
    Shop_Run(3, LOCALID_ITEM_MERCHANT);
  } else {
    if (GameFlag_Test(FLAG_KALAY_REACHED) != 0) {
      FieldScript_Start();
      FieldScript_SetMessageId(MsgValeWhenDidYouComeBack);
      FieldScript_ShowMessageAndWait(LOCALID_ITEM_MERCHANT, 0);
      FieldScript_End();
    } else {
      if (GameFlag_Test(FLAG_VALE_PARTY_LEFT) != 0) {
        ValeInteriorsShops_GiveFarewellHerb();
      } else {
        FieldScript_Start();
        FieldScript_SetMessageId(MsgValeAWiseManFleesWhen);
        FieldScript_ShowMessageAndWait(LOCALID_ITEM_MERCHANT, 0);
        FieldScript_End();
      }
    }
  }
}

/// Garet's grandparents and father nod.
static inline void ValeInteriorsShops_FamilyNods(void) {
  Engine_ObjectEventSetAnimation(LOCALID_GARETS_GRANDFATHER, ANIM_NOD);
  Engine_ObjectEventSetAnimation(LOCALID_GARETS_GRANDMOTHER, ANIM_NOD);
  Engine_ObjectEventSetAnimation(LOCALID_GARETS_FATHER, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(LOCALID_GARETS_GRANDMOTHER, ANIM_NOD);
}

/// Starts the repeated motion before waiting for the requested animation.
static inline void
ValeInteriorsShops_PlayRepeatedAnimationAndWait(s32 localId, s32 animation) {
  ObjectMotion_SetVariantCallback(localId, 1);
  ObjectMotion_SetAnimationAndWait(localId, animation);
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
static void EventScript_GaretComesHome(void) {
  struct ObjectEvent *sleepZ;
  struct ObjectEvent *objectEvent;
  s32 motionFlags;
  TaskFunc timedUpdate;
  const s32 *partyScript;

  // The party walks in while Garet's grandfather dozes, sleep drifting up
  // from him.
  FieldScript_Start();
  FieldCamera_MoveTo(-1, -1, -1, 0);
  WaitFrames(1);
  ObjectMotion_SetActionVariant(MIA, 1);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(2) / 5, PIXELS(1) / 5);
  ObjectEvent_SetSpeed(GARET, PIXELS(2) / 5, PIXELS(1) / 5);
  ObjectEvent_SetSpeed(IVAN, PIXELS(2) / 5, PIXELS(1) / 5);
  ObjectEvent_SetSpeed(MIA, PIXELS(2) / 5, PIXELS(1) / 5);
  Engine_ObjectEventSetAnimation(LOCALID_GARETS_GRANDFATHER,
                                 ANIM_GRANDFATHER_ASLEEP);
  ObjectEvent_WalkTo(LOCALID_PARTY_LEADER, 808, 508);
  sleepZ = ObjectEvent_Get(LOCALID_FIRST_SLEEP_Z);
  ObjectEvent_SetSpriteFlags(sleepZ, 0);
  sleepZ = ObjectEvent_Get(LOCALID_FIRST_SLEEP_Z + 1);
  ObjectEvent_SetSpriteFlags(sleepZ, 0);
  sleepZ = ObjectEvent_Get(LOCALID_FIRST_SLEEP_Z + 2);
  ObjectEvent_SetSpriteFlags(sleepZ, 0);
  motionFlags = 0;
  ObjectEvent_GetById(LOCALID_FIRST_SLEEP_Z)->motionFlags = motionFlags;
  ObjectEvent_GetById(LOCALID_FIRST_SLEEP_Z + 1)->motionFlags = motionFlags;
  ObjectEvent_GetById(LOCALID_FIRST_SLEEP_Z + 2)->motionFlags = motionFlags;
  timedUpdate = ValeInteriorsShops_SpawnSleepZ;
  FieldScheduler_AddCallback(timedUpdate, TASK_PRIORITY_MAP);
  WaitFrames(1);
  gFieldWork->transitionFrames = 32;
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_PARTY_LEADER);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_STAND);
  objectEvent = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  if (objectEvent != NULL) {
    ObjectEvent_SetPosition(GARET, objectEvent->x.fixed, objectEvent->z.fixed);
  }
  objectEvent = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  if (objectEvent != NULL) {
    ObjectEvent_SetPosition(IVAN, objectEvent->x.fixed, objectEvent->z.fixed);
  }
  objectEvent = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  if (objectEvent != NULL) {
    ObjectEvent_SetPosition(MIA, objectEvent->x.fixed, objectEvent->z.fixed);
  }
  ObjectEvent_WalkTo(GARET, 792, 512);
  ObjectEvent_WalkTo(IVAN, 824, 504);
  ObjectEvent_WalkToAndWait(MIA, 818, 524);
  Engine_ObjectEventSetAnimation(GARET, ANIM_STAND);
  Engine_ObjectEventSetAnimation(IVAN, ANIM_STAND);
  FieldScript_WaitFrames(10);
  Object_SetTargetAndCallback(
      LOCALID_PARTY_LEADER, OBJ_EVENT_LINK_KEEP_MOTION | LOCALID_GARETS_FATHER,
      gValeInteriorsShops_FaceTargetScript);
  Object_SetTargetAndCallback(
      GARET, OBJ_EVENT_LINK_KEEP_MOTION | LOCALID_GARETS_FATHER,
      gValeInteriorsShops_FaceTargetScript);
  Object_SetTargetAndCallback(
      IVAN, OBJ_EVENT_LINK_KEEP_MOTION | LOCALID_GARETS_FATHER,
      gValeInteriorsShops_FaceTargetScript);
  Object_SetTargetAndCallback(
      MIA, OBJ_EVENT_LINK_KEEP_MOTION | LOCALID_GARETS_FATHER,
      gValeInteriorsShops_FaceTargetScript);
  FieldScript_WaitFrames(300);
  FieldCamera_GetScriptObject()->motionFlags = motionFlags;
  // The camera pans slowly up to Garet's parents. His father, pacing, asks
  // whether they should look for Garet; Isaac still does not know about
  // Dora. His mother teases that he misses Garet.
  FieldCamera_SetSpeed(PIXELS(1) / 10, PIXELS(1) / 80);
  FieldCamera_MoveTo(PIXELS(786), 0, PIXELS(430), 1);
  FieldScript_WaitFrames(240);
  ObjectMotion_EnableActionAndResetMotion(LOCALID_GARETS_FATHER);
  ObjectEvent_ShowEmote(LOCALID_GARETS_FATHER, EMOTE_IN_FRONT | EMOTE_WINCE,
                        80);
  ObjectEvent_WalkToAndWait(LOCALID_GARETS_FATHER, 819, 405);
  FieldScript_WaitFrames(40);
  ObjectMotion_SetAnimationAndWait(LOCALID_GARETS_FATHER, ANIM_SHAKE_HEAD);
  FieldScript_WaitFrames(40);
  ObjectEvent_FaceDirection(LOCALID_GARETS_FATHER, FACING_NORTH + FACING_STEP,
                            20);
  FieldScript_SetMessageId(MsgValeHey2);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT |
                                LOCALID_GARETS_FATHER,
                            0, 20);
  ObjectMotion_EnableActionAndResetMotion(LOCALID_PARTY_LEADER);
  ObjectMotion_EnableActionAndResetMotion(GARET);
  ObjectMotion_EnableActionAndResetMotion(IVAN);
  ObjectMotion_EnableActionAndResetMotion(MIA);
  ObjectEvent_ShowEmote(LOCALID_GARETS_MOTHER,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | LOCALID_GARETS_MOTHER, 0,
                            20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARETS_FATHER, 2);
  FieldScript_WaitFrames(40);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT |
                                LOCALID_GARETS_FATHER,
                            0, 10);
  ObjectEvent_FaceDirection(LOCALID_GARETS_MOTHER, FACING_SOUTH + FACING_STEP,
                            10);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | LOCALID_GARETS_MOTHER, 0,
                            40);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARETS_FATHER, 2);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT |
                                LOCALID_GARETS_FATHER,
                            0, 20);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  ObjectEvent_ShowEmote(LOCALID_GARETS_MOTHER,
                        EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 40);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | LOCALID_GARETS_MOTHER, 0,
                            40);
  ObjectMotion_SetVariantCallback(LOCALID_GARETS_FATHER, 2);
  ObjectEvent_ShowEmote(LOCALID_GARETS_FATHER, EMOTE_IN_FRONT | EMOTE_WINCE,
                        20);
  Engine_ObjectEventSetAnimation(LOCALID_GARETS_FATHER, ANIM_SHAKE_HEAD);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT |
                                LOCALID_GARETS_FATHER,
                            0, 10);
  ValeInteriorsShops_PlayRepeatedAnimationAndWait(LOCALID_GARETS_MOTHER,
                                                  ANIM_NOD);
  FieldScript_WaitFrames(20);
  ValeInteriorsShops_PlayRepeatedAnimationAndWait(LOCALID_GARETS_FATHER,
                                                  ANIM_SHAKE_HEAD);
  ValeInteriorsShops_PlayRepeatedAnimationAndWait(LOCALID_GARETS_MOTHER,
                                                  ANIM_NOD);
  ValeInteriorsShops_PlayRepeatedAnimationAndWait(LOCALID_GARETS_FATHER,
                                                  ANIM_SHAKE_HEAD);
  // Garet's grandmother sees him and chokes on her words; his mother asks if
  // she is choking, his father thinks she misses Garet too.
  ObjectEvent_ShowEmote(LOCALID_GARETS_GRANDMOTHER,
                        EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARETS_GRANDMOTHER, 1);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_GARETS_GRANDMOTHER,
                            FACING_EAST + FACING_STEP, 40);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARETS_GRANDMOTHER, 2);
  FieldScript_WaitFrames(60);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARETS_GRANDMOTHER, 3);
  FieldScript_WaitFrames(40);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | LOCALID_GARETS_GRANDMOTHER,
                            0, 40);
  Engine_ObjectEventSetAnimation(LOCALID_GARETS_MOTHER, ANIM_STILL);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARETS_MOTHER, 2);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | LOCALID_GARETS_MOTHER, 0,
                            10);
  ObjectMotion_SetAnimationAndWait(LOCALID_GARETS_GRANDMOTHER, ANIM_SHAKE_HEAD);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARETS_GRANDMOTHER, 2);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | LOCALID_GARETS_GRANDMOTHER,
                            0, 10);
  ObjectEvent_ShowEmote(LOCALID_GARETS_FATHER,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  ObjectEvent_FaceDirection(LOCALID_GARETS_FATHER, FACING_SOUTH + FACING_STEP,
                            40);
  ObjectMotion_SetAnimationAndWait(LOCALID_GARETS_FATHER, ANIM_NOD);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | LOCALID_GARETS_FATHER, 0,
                            10);
  // No, it really is Garet: both parents turn and call his name together.
  ObjectMotion_SetAnimationAndWait(LOCALID_GARETS_GRANDMOTHER, ANIM_SHAKE_HEAD);
  ObjectEvent_FaceDirection(LOCALID_GARETS_GRANDMOTHER,
                            FACING_NORTH + FACING_STEP, 10);
  ObjectMotion_Launch(LOCALID_GARETS_GRANDMOTHER, 2, 0);
  Engine_ObjectEventSetAnimation(LOCALID_GARETS_GRANDMOTHER, ANIM_SHAKE_HEAD);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | LOCALID_GARETS_GRANDMOTHER,
                            0, 10);
  ObjectEvent_ShowEmote(LOCALID_GARETS_MOTHER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                        0);
  ObjectEvent_ShowEmote(LOCALID_GARETS_FATHER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                        40);
  ObjectEvent_FaceDirection(LOCALID_GARETS_FATHER, FACING_NORTH + FACING_STEP,
                            80);
  ObjectEvent_FaceDirection(LOCALID_GARETS_FATHER, FACING_SOUTH + FACING_STEP,
                            60);
  ObjectMotion_SetVariantCallback(LOCALID_GARETS_FATHER, 2);
  ObjectMotion_SetVariantCallback(LOCALID_GARETS_MOTHER, 2);
  Script_ShowTwoMessagesAndWait(LOCALID_GARETS_FATHER, 11, 6, 6, 6,
                                LOCALID_GARETS_MOTHER, 12, 1, 7, 1, 0);
  FieldScript_WaitFrames(20);
  FieldCamera_SetSpeed(PIXELS(8) / 5, PIXELS(1) / 5);
  FieldCamera_MoveTo(PIXELS(777), 0, PIXELS(468), 1);
  FieldCamera_WaitForArrival();
  FieldScript_WaitFrames(40);
  ObjectMotion_SetAnimationAndWait(GARET, ANIM_NOD);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | GARET, 0, 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARETS_GRANDFATHER, 2);
  // His grandfather wakes, and the sleep stops drifting.
  Task_Remove(timedUpdate);
  FieldScript_WaitFrames(40);
  ObjectMotion_SetAnimationAndWait(LOCALID_GARETS_GRANDFATHER,
                                   ANIM_GRANDFATHER_LOOK_ABOUT);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | LOCALID_GARETS_GRANDFATHER,
                            0, 20);
  // The grandfather looks about, then jumps up with Garet's name.
  FieldCamera_MoveTo(PIXELS(750), 0, PIXELS(451), 1);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_GARETS_MOTHER, FACING_SOUTH + FACING_STEP,
                            0);
  ObjectEvent_FaceDirection(LOCALID_GARETS_FATHER, FACING_SOUTH + FACING_STEP,
                            10);
  ObjectEvent_FaceDirection(LOCALID_GARETS_GRANDFATHER,
                            FACING_EAST + FACING_STEP, 40);
  ObjectEvent_ShowEmote(LOCALID_GARETS_GRANDFATHER,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  ObjectEvent_FaceDirection(LOCALID_GARETS_GRANDFATHER,
                            FACING_SOUTHEAST + FACING_STEP, 20);
  ObjectEvent_FaceDirection(LOCALID_GARETS_GRANDFATHER,
                            FACING_EAST + FACING_STEP, 20);
  ObjectEvent_FaceDirection(LOCALID_GARETS_GRANDFATHER,
                            FACING_SOUTHEAST + FACING_STEP, 40);
  ObjectMotion_SetAnimationAndWait(LOCALID_GARETS_GRANDFATHER,
                                   ANIM_GRANDFATHER_LOOK_ABOUT);
  FieldScript_WaitFrames(60);
  ObjectMotion_Launch(LOCALID_GARETS_GRANDFATHER, 6, 0);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | LOCALID_GARETS_GRANDFATHER,
                            0, 20);
  // Garet goes to him; the grandfather asks whether the Wise One's mission
  // is done, and a no skips one line of Garet's father.
  ObjectEvent_SetSpeed(GARET, PIXELS(8) / 5, PIXELS(4) / 5);
  ObjectEvent_WalkToAndWait(GARET, 789, 473);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTHWEST + FACING_STEP, 20);
  ObjectMotion_SetAnimationAndWait(GARET, ANIM_NOD);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | GARET, 0, 10);
  ObjectEvent_FaceDirection(LOCALID_GARETS_GRANDFATHER,
                            FACING_EAST + FACING_STEP, 20);
  ObjectMotion_SetAnimationAndWait(LOCALID_GARETS_GRANDFATHER, ANIM_NOD);
  Script_OpenMessage(SPEAKER_WINDOW_ABOVE | LOCALID_GARETS_GRANDFATHER, 0);
  ObjectEvent_FaceDirection(LOCALID_GARETS_FATHER, FACING_SOUTH + FACING_STEP,
                            0);
  ObjectEvent_FaceDirection(LOCALID_GARETS_GRANDMOTHER,
                            FACING_EAST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_SOUTHWEST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(MIA, FACING_NORTHWEST + FACING_STEP, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 1) {
    gFieldWork->message++;
  }
  FieldCamera_MoveTo(PIXELS(777), 0, PIXELS(428), 1);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARETS_FATHER, 2);
  Script_ShowMessage(LOCALID_GARETS_FATHER, 0);
  ObjectMotion_SetAnimationAndWait(LOCALID_GARETS_MOTHER, ANIM_SHAKE_HEAD);
  FieldScript_WaitFrames(20);
  // His mother knows he is leaving again soon; his grandparents are glad to
  // see him well.
  FieldScript_SetMessageId(MsgValeYoureLeavingAgainSoon);
  Script_ShowMessage(SPEAKER_PORTRAIT_RIGHT | LOCALID_GARETS_MOTHER, 0);
  FieldCamera_MoveTo(PIXELS(777), 0, PIXELS(468), 1);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 20);
  ObjectMotion_SetAnimationAndWait(GARET, ANIM_NOD);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetAnimationAndWait(LOCALID_GARETS_GRANDMOTHER, ANIM_SHAKE_HEAD);
  ObjectEvent_FaceDirection(LOCALID_GARETS_GRANDMOTHER,
                            FACING_NORTH + FACING_STEP, 10);
  Script_ShowMessage(SPEAKER_WINDOW_ABOVE | LOCALID_GARETS_GRANDMOTHER, 0);
  ObjectMotion_SetAnimationAndWait(LOCALID_GARETS_GRANDFATHER, ANIM_NOD);
  Script_ShowMessage(SPEAKER_WINDOW_ABOVE | LOCALID_GARETS_GRANDFATHER, 0);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTHWEST + FACING_STEP, 10);
  ObjectMotion_SetAnimationAndWait(GARET, ANIM_NOD);
  ObjectEvent_FaceDirection(LOCALID_GARETS_GRANDMOTHER,
                            FACING_EAST + FACING_STEP, 10);
  Engine_ObjectEventSetAnimation(LOCALID_GARETS_MOTHER, ANIM_NOD);
  Engine_ObjectEventSetAnimation(LOCALID_GARETS_FATHER, ANIM_NOD);
  Engine_ObjectEventSetAnimation(LOCALID_GARETS_GRANDMOTHER, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(LOCALID_GARETS_GRANDFATHER, ANIM_NOD);
  FieldScript_WaitFrames(20);
  // Garet introduces his new friends.
  ObjectEvent_FaceDirection(GARET, FACING_SOUTHEAST + FACING_STEP, 20);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTHWEST + FACING_STEP, 20);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | GARET, 0, 20);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTHEAST + FACING_STEP, 10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_EAST, 40);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(GARET, ANIM_NOD);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 20);
  ObjectMotion_SetAnimationAndWait(MIA, ANIM_NOD);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(GARET, FACING_EAST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTHEAST, 0);
  // Ivan, then Mia, step forward and introduce themselves; the family nods.
  ObjectEvent_SetSpeed(IVAN, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkToAndWait(IVAN, 819, 489);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTHWEST + FACING_STEP, 40);
  ObjectMotion_SetVarCbAndRefresh(IVAN, 2);
  Script_ShowMessageAndWait(IVAN, 0, 20);
  ObjectMotion_SetAnimationAndWait(IVAN, ANIM_NOD);
  ValeInteriorsShops_FamilyNods();
  ObjectEvent_GetById(MIA)->priorityFlags &= ~OBJ_EVENT_PRIORITY_AUTOMATIC;
  ObjectMotion_SetActionVariant(MIA, 1);
  ObjectEvent_SetSpeed(MIA, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkToAndWait(MIA, 794, 520);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTHWEST, 0);
  ObjectEvent_WalkToAndWait(MIA, 784, 496);
  ObjectEvent_FaceDirection(MIA, FACING_WEST + FACING_STEP, 10);
  ObjectEvent_GetById(MIA)->priorityFlags |= OBJ_EVENT_PRIORITY_AUTOMATIC;
  Script_ShowMessageAndWait(MIA, 0, 20);
  ValeInteriorsShops_FamilyNods();
  FieldScript_WaitFrames(20);
  FieldCamera_MoveTo(PIXELS(777), 0, PIXELS(428), 1);
  FieldScript_WaitFrames(20);
  // Garet's mother comes forward and asks them to look after young Garet.
  ObjectEvent_SetSpeed(LOCALID_GARETS_MOTHER, PIXELS(2) / 5, PIXELS(1) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_GARETS_MOTHER, 835, 388);
  ObjectEvent_FaceDirection(LOCALID_GARETS_MOTHER, FACING_SOUTH + FACING_STEP,
                            0);
  ObjectEvent_ShowEmote(LOCALID_GARETS_MOTHER, EMOTE_IN_FRONT | EMOTE_SMILE,
                        40);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | LOCALID_GARETS_MOTHER, 0,
                            20);
  FieldCamera_MoveTo(PIXELS(777), 0, PIXELS(468), 1);
  FieldScript_WaitFrames(40);
  ObjectEvent_FaceDirection(IVAN, FACING_SOUTHWEST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(MIA, FACING_NORTHEAST + FACING_STEP, 40);
  ObjectEvent_FaceDirection(IVAN, FACING_WEST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(MIA, FACING_NORTH + FACING_STEP, 20);
  Engine_ObjectEventSetAnimation(IVAN, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(MIA, ANIM_NOD);
  FieldScript_WaitFrames(20);
  // His father: enough chit-chat, relax and enjoy your stay.
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARETS_FATHER, 1);
  FieldScript_WaitFrames(20);
  Engine_ObjectEventSetAnimation(LOCALID_GARETS_FATHER, ANIM_NOD);
  Script_ShowMessageAndWait(LOCALID_GARETS_FATHER, 0, 20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTHWEST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(MIA, FACING_NORTH + FACING_STEP, 40);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  Engine_ObjectEventSetAnimation(GARET, ANIM_NOD);
  Engine_ObjectEventSetAnimation(IVAN, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(MIA, ANIM_NOD);
  FieldScript_WaitFrames(20);
  ObjectEvent_SetSpeed(IVAN, PIXELS(1), PIXELS(1) / 2);
  partyScript = gValeInteriorsShops_PartyLeavesScript;
  ObjectEvent_SetActionScript(GARET, partyScript);
  ObjectEvent_SetActionScript(IVAN, partyScript);
  ObjectEvent_RunActionScriptAndWait(MIA, partyScript);
  ObjectEvent_SetActionScript(LOCALID_GARETS_FATHER,
                              gValeInteriorsShops_FatherPacingScript);
  ObjectEvent_WalkToAndWait(LOCALID_GARETS_MOTHER, 837, 376);
  ObjectEvent_FaceDirection(LOCALID_GARETS_MOTHER, FACING_NORTH + FACING_STEP,
                            20);
  GameFlag_Set(FLAG_GARET_CAME_HOME);
  FieldScript_End();
}

/// Sends a sprite drifting up from Garet's sleeping grandfather, one of the
/// three at frames 10, 20 and 30 of every 180.
///
/// A callback EventScript_GaretComesHome adds while the grandfather dozes.
static void ValeInteriorsShops_SpawnSleepZ(void) {
  s32 localId;
  u32 cycleFrame;
  struct ObjectEvent *objectEvent;
  struct ObjectEvent *origin;

  cycleFrame = gFrameCount % 180;
  localId = LOCALID_FIRST_SLEEP_Z;
  switch (cycleFrame) {
  case 10:
    break;
  case 20:
    localId = LOCALID_FIRST_SLEEP_Z + 1;
    break;
  case 30:
    localId = LOCALID_FIRST_SLEEP_Z + 2;
    break;
  default:
    return;
  }
  objectEvent = ObjectEvent_GetById(localId);
  if (objectEvent == NULL) {
    return;
  }
  origin = ObjectEvent_GetById(LOCALID_GARETS_GRANDFATHER);
  if (origin != NULL) {
    Engine_ObjectEventSetPosition(localId, origin->x.fixed, origin->z.fixed);
  }
  // Restart this sprite above the origin with a small scale and 8.8 motion.
  objectEvent->scaleX = 2 * Q16_ONE / 5;
  objectEvent->scaleY = 2 * Q16_ONE / 5;
  {
    s32 height = objectEvent->y.fixed + PIXELS(24);

    objectEvent->y.fixed = height;
    objectEvent->targetY = height;
    // The drifting script consumes these two work fields as signed 8.8 steps.
    objectEvent->timer = 25; // 25/256 pixel per frame.
    objectEvent->work = 128; // 1/2 pixel per frame.
  }
  ObjectEvent_SetActionScript(localId, gValeInteriorsShops_SleepZScript);
}

s32 gValeInteriorsShops_FaceTargetScript[] = {
    SC_CALL(Map_TurnTowardTarget),
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

struct MapWarpDest gValeInteriorsShops_WarpDests[] = {
    {WARP_DEST_DEFAULT, CONDITION_ALWAYS, 120, 0, 100, FACING_SOUTH, 0, -1, -1,
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
    {MAP_TABLE_END},
};

u32 gValeInteriorsShops_Warps[] = {
    MAP_WARPS(ValeInteriorsShops),
    MAP_WARP(VALE_INTERIORS_SHOPS_TO_VALE_WEST, Vale,
             VALE_FROM_VALE_INTERIORS_SHOPS_WEST),
    MAP_WARP(VALE_INTERIORS_SHOPS_TO_VALE_NORTH, Vale,
             VALE_FROM_VALE_INTERIORS_SHOPS_NORTH),
    MAP_WARP(VALE_INTERIORS_SHOPS_TO_VALE_GATE_WEST, ValeGate,
             VALE_GATE_FROM_VALE_INTERIORS_SHOPS_WEST),
    MAP_WARP(VALE_INTERIORS_SHOPS_TO_VALE_GATE_SOUTH, ValeGate,
             VALE_GATE_FROM_VALE_INTERIORS_SHOPS_SOUTH),
    MAP_WARP(VALE_INTERIORS_SHOPS_TO_VALE_SOUTH, Vale,
             VALE_FROM_VALE_INTERIORS_SHOPS_SOUTH),
    MAP_WARP(VALE_INTERIORS_SHOPS_TO_VALE_EAST, Vale,
             VALE_FROM_VALE_INTERIORS_SHOPS_EAST),
    MAP_WARP(VALE_INTERIORS_SHOPS_TO_VALE_GATE_EAST, ValeGate,
             VALE_GATE_FROM_VALE_INTERIORS_SHOPS_EAST),
    MAP_WARP(VALE_INTERIORS_SHOPS_TO_VALE_GATE_NORTH, ValeGate,
             VALE_GATE_FROM_VALE_INTERIORS_SHOPS_NORTH),
    MAP_WARPS_END,
};

u32 gValeInteriorsShops_StormWarps[] = {
    MAP_WARPS(ValeInteriorsShops),
    MAP_WARP(VALE_INTERIORS_SHOPS_TO_VALE_WEST, ValeStorm,
             VALE_STORM_FROM_VALE_INTERIORS_SHOPS_WEST),
    MAP_WARP(VALE_INTERIORS_SHOPS_TO_VALE_NORTH, ValeStorm,
             VALE_STORM_FROM_VALE_INTERIORS_SHOPS_NORTH),
    MAP_WARP(VALE_INTERIORS_SHOPS_TO_VALE_GATE_WEST, ValeGate,
             VALE_GATE_FROM_VALE_INTERIORS_SHOPS_WEST),
    MAP_WARP(VALE_INTERIORS_SHOPS_TO_VALE_GATE_SOUTH, ValeGate,
             VALE_GATE_FROM_VALE_INTERIORS_SHOPS_SOUTH),
    MAP_WARP(VALE_INTERIORS_SHOPS_TO_VALE_SOUTH, ValeStorm,
             VALE_STORM_FROM_VALE_INTERIORS_SHOPS_SOUTH),
    MAP_WARP(VALE_INTERIORS_SHOPS_TO_VALE_EAST, ValeStorm,
             VALE_STORM_FROM_VALE_INTERIORS_SHOPS_EAST),
    MAP_WARP(VALE_INTERIORS_SHOPS_TO_VALE_GATE_EAST, ValeGate,
             VALE_GATE_FROM_VALE_INTERIORS_SHOPS_EAST),
    MAP_WARP(VALE_INTERIORS_SHOPS_TO_VALE_GATE_NORTH, ValeGate,
             VALE_GATE_FROM_VALE_INTERIORS_SHOPS_NORTH),
    MAP_WARPS_END,
};

// clang-format off
struct ObjectEventTemplate gValeInteriorsShops_ObjectEventTemplates[] = {
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
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gValeInteriorsShops_ObjectEventTemplatesAfterLeaving[] = {
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
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gValeInteriorsShops_ObjectEventTemplatesAfterKalay[] = {
    { GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0 },
    { IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0 },
    { MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_WHITE_HAIR, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(697), 0, PIXELS(468), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_WOMAN, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(726), 0, PIXELS(486), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_IN_TEAL_SHIRT, 0x0, (s32)gValeInteriorsShops_FatherPacingScript,
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
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { 280, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { MAP_TABLE_END },
};

struct MapEvent gValeInteriorsShops_StormEvents[] = {
    { MAP_EVENT_COORD, 1, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveToValeWest) },
    { MAP_EVENT_COORD, 2, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveToValeNorth) },
    { MAP_EVENT_COORD, 3, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveToValeGateWest) },
    { MAP_EVENT_COORD, 4, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveToValeGateSouth) },
    { MAP_EVENT_COORD, 5, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveToValeSouth) },
    { MAP_EVENT_COORD, 6, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveToValeEast) },
    { MAP_EVENT_COORD, 7, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveToValeGateEast) },
    { MAP_EVENT_COORD, 8, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveToValeGateNorth) },
    { BG_EVENT(BG_EVENT_BOOKCASE), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBooksAboutContinents)) },
    { BG_EVENT(BG_EVENT_BOOKCASE), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookLawsOfVale)) },
    { BG_EVENT(BG_EVENT_BOOKCASE) | MAP_EVENT_FLAG_FACING | FACING_NORTH, 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookPowerOfNature)) },
    { BG_EVENT(BG_EVENT_BOOKCASE) | MAP_EVENT_FLAG_FACING | FACING_NORTH, 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookPsynergyStone)) },
    { BG_EVENT(BG_EVENT_BOOKCASE) | MAP_EVENT_FLAG_FACING | FACING_NORTH, 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeAuntNotebook)) },
    { BG_EVENT(BG_EVENT_BOOKCASE) | MAP_EVENT_FLAG_FACING | FACING_NORTH, 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookMakingGreatVillage)) },
    { BG_EVENT(BG_EVENT_BOOKCASE), 206, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeNotebookStudyOfAlchemy)) },
    { BG_EVENT(BG_EVENT_BOOKCASE), 207, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookAncientLemurians)) },
    { BG_EVENT(BG_EVENT_BOOKCASE), 208, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookFountainOfTolbi)) },
    { BG_EVENT(BG_EVENT_BOOKCASE), 209, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeEncyclopediaOfAlchemy)) },
    { BG_EVENT(BG_EVENT_BOOKCASE), 210, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookLegendOfAleph)) },
    { BG_EVENT(BG_EVENT_BOOKCASE), 211, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookBeingAdept)) },
    { BG_EVENT(BG_EVENT_BARREL), 100, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgVaultButDidntFindAnything)) },
    { BG_EVENT(BG_EVENT_JAR), 101, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeJarEmpty)) },
    { BG_EVENT(BG_EVENT_JAR), 102, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeJarEmpty)) },
    { BG_EVENT(BG_EVENT_JAR), 103, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeJarEmpty)) },
    { BG_EVENT(BG_EVENT_JAR), 104, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeJarEmpty)) },
    { MAP_EVENTS_END },
};

struct MapEvent gValeInteriorsShops_Events[] = {
    { MAP_EVENT_TALK, LOCALID_GARETS_GRANDFATHER, CONDITION_ALWAYS, MSGNO(MsgValeAtTheVeryLeastVisitorsShould) },
    { MAP_EVENT_TALK, LOCALID_GARETS_GRANDMOTHER, CONDITION_ALWAYS, MSGNO(MsgValeIHearKradenHasGuestsAgain) },
    { MAP_EVENT_TALK, LOCALID_GARETS_FATHER, CONDITION_ALWAYS, MSGNO(MsgValeGaretHasGrownQuiteStrongIn) },
    { MAP_EVENT_TALK, LOCALID_GARETS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgValeYouAndGaretAreOlderNow) },
    { MAP_EVENT_TALK, LOCALID_JENNAS_GRANDFATHER, CONDITION_ALWAYS, MSGNO(MsgValeMyGranddaughterJennaIsFinallyHappy) },
    { MAP_EVENT_TALK, LOCALID_JENNAS_GRANDMOTHER, CONDITION_ALWAYS, MSGNO(MsgValeIHopeYouTwoWontBe) },
    { MAP_EVENT_TALK, LOCALID_WOMAN_ON_THE_SPITTING_IMAGE, CONDITION_ALWAYS, MSGNO(MsgValeJennaIsTheSpittingImageOf) },
    { MAP_EVENT_TALK, LOCALID_MAN_ON_KRADENS_STUDIES, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_AskAboutKradensStudies) },
    { MAP_EVENT_TALK, LOCALID_WOMAN_ON_VISITORS_IN_TOWN, CONDITION_ALWAYS, MSGNO(MsgValeIfThereAreVisitorsInTown) },
    { MAP_EVENT_TALK, LOCALID_YOUNG_MAN_DREAMING_OF_LEAVING, CONDITION_ALWAYS, MSGNO(MsgValeDoYouEverDreamOfSeeing) },
    { MAP_EVENT_TALK, LOCALID_WOMAN_ON_PERMISSION_TO_LEAVE, CONDITION_ALWAYS, MSGNO(MsgValeFewPeopleOutsideOfValePossess) },
    { MAP_EVENT_TALK, LOCALID_STRONGMAN_SCARING_VISITORS, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_AskToScareVisitors) },
    { MAP_EVENT_TALK, LOCALID_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToWeaponMerchant) },
    { MAP_EVENT_TALK, LOCALID_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToArmorMerchant) },
    { MAP_EVENT_TALK, LOCALID_ITEM_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToItemMerchant) },
    { MAP_EVENT_COORD, 1, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveToValeWest) },
    { MAP_EVENT_COORD, 2, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveToValeNorth) },
    { MAP_EVENT_COORD, 3, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveToValeGateWest) },
    { MAP_EVENT_COORD, 4, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveToValeGateSouth) },
    { MAP_EVENT_COORD, 5, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveToValeSouth) },
    { MAP_EVENT_COORD, 6, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveToValeEast) },
    { MAP_EVENT_COORD, 7, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveToValeGateEast) },
    { MAP_EVENT_COORD, 8, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveToValeGateNorth) },
    { BG_EVENT(BG_EVENT_BOOKCASE), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBooksAboutContinents)) },
    { BG_EVENT(BG_EVENT_BOOKCASE), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookLawsOfVale)) },
    { BG_EVENT(BG_EVENT_BOOKCASE) | MAP_EVENT_FLAG_FACING | FACING_NORTH, 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookPowerOfNature)) },
    { BG_EVENT(BG_EVENT_BOOKCASE) | MAP_EVENT_FLAG_FACING | FACING_NORTH, 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookPsynergyStone)) },
    { BG_EVENT(BG_EVENT_BOOKCASE) | MAP_EVENT_FLAG_FACING | FACING_NORTH, 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeAuntNotebook)) },
    { BG_EVENT(BG_EVENT_BOOKCASE) | MAP_EVENT_FLAG_FACING | FACING_NORTH, 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookMakingGreatVillage)) },
    { BG_EVENT(BG_EVENT_BARREL), 100, FLAG_VALE_INTERIORS_SHOPS_BARREL_ANTIDOTE, FIND_ITEM(ITEM_ANTIDOTE) },
    { BG_EVENT(BG_EVENT_JAR), 101, FLAG_VALE_INTERIORS_SHOPS_JAR_6_COINS, FIND_COINS(6) },
    { BG_EVENT(BG_EVENT_JAR), 102, FLAG_VALE_INTERIORS_SHOPS_JAR_2_COINS, FIND_COINS(2) },
    { BG_EVENT(BG_EVENT_JAR), 103, FLAG_VALE_INTERIORS_SHOPS_JAR_SMOKE_BOMB, FIND_ITEM(ITEM_SMOKE_BOMB) },
    { BG_EVENT(BG_EVENT_JAR), 104, FLAG_VALE_INTERIORS_SHOPS_JAR_3_COINS, FIND_COINS(3) },
    { MAP_EVENTS_END },
};

struct MapEvent gValeInteriorsShops_EventsAfterLeaving[] = {
    { MAP_EVENT_TALK, LOCALID_GARETS_GRANDFATHER, CONDITION_ALWAYS, MSGNO(MsgValeIsaacPleaseTakeCareOfMy) },
    { MAP_EVENT_TALK, LOCALID_GARETS_GRANDMOTHER, CONDITION_ALWAYS, MSGNO(MsgValeReceivingAQuestDirectlyFromThe) },
    { MAP_EVENT_TALK, LOCALID_GARETS_FATHER, CONDITION_ALWAYS, MSGNO(MsgValeImProudOfYouGaretMy) },
    { MAP_EVENT_TALK, LOCALID_GARETS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgValeAsMothersDoraAndIWill) },
    { MAP_EVENT_TALK, LOCALID_JENNAS_GRANDFATHER, CONDITION_ALWAYS, MSGNO(MsgValeIHeardThatJennaBrotherFelix) },
    { MAP_EVENT_TALK, LOCALID_JENNAS_GRANDMOTHER, CONDITION_ALWAYS, MSGNO(MsgValePleaseReturnMyPreciousGranddaughterJenna) },
    { MAP_EVENT_TALK, LOCALID_WOMAN_ON_THE_SPITTING_IMAGE, CONDITION_ALWAYS, MSGNO(MsgValeAfterJennaWasTakenGrandmotherGrew) },
    { MAP_EVENT_TALK, LOCALID_MAN_ON_KRADENS_STUDIES, CONDITION_ALWAYS, MSGNO(MsgValeItsBecauseKradenWasStudyingAlchemy) },
    { MAP_EVENT_TALK, LOCALID_WOMAN_ON_VISITORS_IN_TOWN, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_AskAboutStrangePowers) },
    { MAP_EVENT_TALK, LOCALID_YOUNG_MAN_DREAMING_OF_LEAVING, CONDITION_ALWAYS, MSGNO(MsgValeImSoJealousOfYouBeing) },
    { MAP_EVENT_TALK, LOCALID_WOMAN_ON_PERMISSION_TO_LEAVE, CONDITION_ALWAYS, MSGNO(MsgValeOnceYouLeaveTownTakeCare) },
    { MAP_EVENT_TALK, LOCALID_STRONGMAN_SCARING_VISITORS, CONDITION_ALWAYS, MSGNO(MsgValeIHadNoIdeaThatSuch) },
    { MAP_EVENT_TALK, LOCALID_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToWeaponMerchant) },
    { MAP_EVENT_TALK, LOCALID_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToArmorMerchant) },
    { MAP_EVENT_TALK, LOCALID_ITEM_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToItemMerchant) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgValeIsaacYoureGoingOnADifficult) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgValeIfYouFaceAnyHardshipIn) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ITEM_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgValeSoIfThatWasFelixWho) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_GARETS_GRANDFATHER, CONDITION_ALWAYS, MSGNO(MsgValeItsHardLettingMyPreciousGaret) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_GARETS_GRANDMOTHER, CONDITION_ALWAYS, MSGNO(MsgValeTheWiseOneChoseGaretHimself) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_GARETS_FATHER, CONDITION_ALWAYS, MSGNO(MsgValeOnceGaretLeavesIWontBe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_GARETS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgValeIAmConcernedForGaretBut) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_JENNAS_GRANDFATHER, CONDITION_ALWAYS, MSGNO(MsgValeFelixInheritedHisFathersSkillWith) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_JENNAS_GRANDMOTHER, CONDITION_ALWAYS, MSGNO(MsgValeJennaAnytimeYouFaceDangerJust) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WOMAN_ON_THE_SPITTING_IMAGE, CONDITION_ALWAYS, MSGNO(MsgValeWhyWouldFelixTakeHisSister) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_MAN_ON_KRADENS_STUDIES, CONDITION_ALWAYS, MSGNO(MsgValeWeAlmostLostTheTownOver) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WOMAN_ON_VISITORS_IN_TOWN, CONDITION_ALWAYS, MSGNO(MsgValeIdLikeToBelieveThatOnly) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_YOUNG_MAN_DREAMING_OF_LEAVING, CONDITION_ALWAYS, MSGNO(MsgValeIfOnlyIdGoneToSol) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WOMAN_ON_PERMISSION_TO_LEAVE, CONDITION_ALWAYS, MSGNO(MsgValeValesLawsStateThatYouCannot) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STRONGMAN_SCARING_VISITORS, CONDITION_ALWAYS, MSGNO(MsgValeIWishIWereTheOne) },
    { MAP_EVENT_COORD, 1, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveToValeWest) },
    { MAP_EVENT_COORD, 2, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveToValeNorth) },
    { MAP_EVENT_COORD, 3, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveToValeGateWest) },
    { MAP_EVENT_COORD, 4, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveToValeGateSouth) },
    { MAP_EVENT_COORD, 5, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveToValeSouth) },
    { MAP_EVENT_COORD, 6, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveToValeEast) },
    { MAP_EVENT_COORD, 7, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveToValeGateEast) },
    { MAP_EVENT_COORD, 8, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveToValeGateNorth) },
    { BG_EVENT(BG_EVENT_BARREL), 100, FLAG_VALE_INTERIORS_SHOPS_BARREL_ANTIDOTE, FIND_ITEM(ITEM_ANTIDOTE) },
    { BG_EVENT(BG_EVENT_JAR), 101, FLAG_VALE_INTERIORS_SHOPS_JAR_6_COINS, FIND_COINS(6) },
    { BG_EVENT(BG_EVENT_JAR), 102, FLAG_VALE_INTERIORS_SHOPS_JAR_2_COINS, FIND_COINS(2) },
    { BG_EVENT(BG_EVENT_JAR), 103, FLAG_VALE_INTERIORS_SHOPS_JAR_SMOKE_BOMB, FIND_ITEM(ITEM_SMOKE_BOMB) },
    { BG_EVENT(BG_EVENT_JAR), 104, FLAG_VALE_INTERIORS_SHOPS_JAR_3_COINS, FIND_COINS(3) },
    { BG_EVENT(BG_EVENT_BOOKCASE), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBooksAboutContinents)) },
    { BG_EVENT(BG_EVENT_BOOKCASE), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookLawsOfVale)) },
    { BG_EVENT(BG_EVENT_BOOKCASE) | MAP_EVENT_FLAG_FACING | FACING_NORTH, 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookPowerOfNature)) },
    { BG_EVENT(BG_EVENT_BOOKCASE) | MAP_EVENT_FLAG_FACING | FACING_NORTH, 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookPsynergyStone)) },
    { BG_EVENT(BG_EVENT_BOOKCASE) | MAP_EVENT_FLAG_FACING | FACING_NORTH, 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeAuntNotebook)) },
    { BG_EVENT(BG_EVENT_BOOKCASE) | MAP_EVENT_FLAG_FACING | FACING_NORTH, 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookMakingGreatVillage)) },
    { MAP_EVENTS_END },
};

struct MapEvent gValeInteriorsShops_EventsAfterKalay[] = {
    { MAP_EVENT_TALK, LOCALID_GARETS_GRANDFATHER, CONDITION_ALWAYS, MSGNO(MsgValeMiaIsSuchANiceGirl) },
    { MAP_EVENT_TALK, LOCALID_GARETS_GRANDMOTHER, CONDITION_ALWAYS, MSGNO(MsgValeThatIvanSeemsToBeA) },
    { MAP_EVENT_TALK, LOCALID_GARETS_FATHER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_AskAboutDora) },
    { MAP_EVENT_TALK, LOCALID_GARETS_MOTHER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WorryAboutTravelersHealth) },
    { MAP_EVENT_TALK, LOCALID_JENNAS_GRANDFATHER, CONDITION_ALWAYS, MSGNO(MsgValeOhSoYouStillHaventRescued) },
    { MAP_EVENT_TALK, LOCALID_JENNAS_GRANDMOTHER, CONDITION_ALWAYS, MSGNO(MsgValeIGetOverColdsQuicklyBut) },
    { MAP_EVENT_TALK, LOCALID_WOMAN_ON_THE_SPITTING_IMAGE, CONDITION_ALWAYS, MSGNO(MsgValeAColdIsGoingAroundI) },
    { MAP_EVENT_TALK, LOCALID_MAN_ON_KRADENS_STUDIES, CONDITION_ALWAYS, MSGNO(MsgValeWhatWasThatAlchemyThatKraden) },
    { MAP_EVENT_TALK, LOCALID_WOMAN_ON_VISITORS_IN_TOWN, CONDITION_ALWAYS, MSGNO(MsgValeEarthquakesStillStrikeOnceInA) },
    { MAP_EVENT_TALK, LOCALID_YOUNG_MAN_DREAMING_OF_LEAVING, CONDITION_ALWAYS, MSGNO(MsgValeTheMountainIsApparentlyStillCasting) },
    { MAP_EVENT_TALK, LOCALID_WOMAN_ON_PERMISSION_TO_LEAVE, CONDITION_ALWAYS, MSGNO(MsgValeTheNorthwestFaceOfMtAleph) },
    { MAP_EVENT_TALK, LOCALID_STRONGMAN_SCARING_VISITORS, CONDITION_ALWAYS, MSGNO(MsgValeTheGreatHealersAreTooAfraid) },
    { MAP_EVENT_TALK, LOCALID_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToWeaponMerchant) },
    { MAP_EVENT_TALK, LOCALID_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToArmorMerchant) },
    { MAP_EVENT_TALK, LOCALID_ITEM_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToItemMerchant) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_GARETS_GRANDFATHER, CONDITION_ALWAYS, MSGNO(MsgValeItMustBeNiceToBe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_GARETS_GRANDMOTHER, CONDITION_ALWAYS, MSGNO(MsgValeHisNewFriendsAreAllSo) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_GARETS_FATHER, CONDITION_ALWAYS, MSGNO(MsgValeItWasntTooSmartToTalk) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_GARETS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgValeGaretSeemsToHaveGrownUp) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgValeWeveSeenManyMonstersLatelyAnd) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgValeIWishICouldLearnThe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ITEM_MERCHANT, FLAG_VALE_FAREWELL_HERB_RECEIVED, MSGNO(MsgValeWhatHappenedToJennaHaventYou) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ITEM_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgValeIGaveHimAFarewellGift) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_JENNAS_GRANDFATHER, CONDITION_ALWAYS, MSGNO(MsgValeWillIEverSeeJennaAnd) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_JENNAS_GRANDMOTHER, CONDITION_ALWAYS, MSGNO(MsgValeDoraSaysItsJustACold) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WOMAN_ON_THE_SPITTING_IMAGE, CONDITION_ALWAYS, MSGNO(MsgValeIKnowThatThereAreAlways) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_MAN_ON_KRADENS_STUDIES, CONDITION_ALWAYS, MSGNO(MsgValeEvenIfTheEruptionHadNo) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WOMAN_ON_VISITORS_IN_TOWN, CONDITION_ALWAYS, MSGNO(MsgValeEveryTimeTheGroundShakesI) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_YOUNG_MAN_DREAMING_OF_LEAVING, CONDITION_ALWAYS, MSGNO(MsgValeIfThePsynergyStonesInThe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WOMAN_ON_PERMISSION_TO_LEAVE, CONDITION_ALWAYS, MSGNO(MsgValeTheySayTheLavaLookedLike) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STRONGMAN_SCARING_VISITORS, CONDITION_ALWAYS, MSGNO(MsgValeTheHealersAreSupposedToGuard) },
    { MAP_EVENT_COORD, 1, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveToValeWest) },
    { MAP_EVENT_COORD, 2, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveToValeNorth) },
    { MAP_EVENT_COORD, 3, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveToValeGateWest) },
    { MAP_EVENT_COORD, 4, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveToValeGateSouth) },
    { MAP_EVENT_COORD, 5, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveToValeSouth) },
    { MAP_EVENT_COORD, 6, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveToValeEast) },
    { MAP_EVENT_COORD, 7, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveToValeGateEast) },
    { MAP_EVENT_COORD, 8, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveToValeGateNorth) },
    { BG_EVENT(BG_EVENT_BARREL), 100, FLAG_VALE_INTERIORS_SHOPS_BARREL_ANTIDOTE, FIND_ITEM(ITEM_ANTIDOTE) },
    { BG_EVENT(BG_EVENT_JAR), 101, FLAG_VALE_INTERIORS_SHOPS_JAR_6_COINS, FIND_COINS(6) },
    { BG_EVENT(BG_EVENT_JAR), 102, FLAG_VALE_INTERIORS_SHOPS_JAR_2_COINS, FIND_COINS(2) },
    { BG_EVENT(BG_EVENT_JAR), 103, FLAG_VALE_INTERIORS_SHOPS_JAR_SMOKE_BOMB, FIND_ITEM(ITEM_SMOKE_BOMB) },
    { BG_EVENT(BG_EVENT_JAR), 104, FLAG_VALE_INTERIORS_SHOPS_JAR_3_COINS, FIND_COINS(3) },
    { BG_EVENT(BG_EVENT_BOOKCASE), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBooksAboutContinents)) },
    { BG_EVENT(BG_EVENT_BOOKCASE), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookLawsOfVale)) },
    { BG_EVENT(BG_EVENT_BOOKCASE) | MAP_EVENT_FLAG_FACING | FACING_NORTH, 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookPowerOfNature)) },
    { BG_EVENT(BG_EVENT_BOOKCASE) | MAP_EVENT_FLAG_FACING | FACING_NORTH, 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookPsynergyStone)) },
    { BG_EVENT(BG_EVENT_BOOKCASE) | MAP_EVENT_FLAG_FACING | FACING_NORTH, 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeAuntNotebook)) },
    { BG_EVENT(BG_EVENT_BOOKCASE) | MAP_EVENT_FLAG_FACING | FACING_NORTH, 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookMakingGreatVillage)) },
    { MAP_EVENTS_END },
};
// clang-format on
