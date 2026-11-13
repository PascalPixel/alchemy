/// @file
/// Cottage rooms A (SceneId_CottageRoomsA) is a sheet of rooms drawn as
/// cottages.
///
/// The game packs rooms that share a tile style onto one map, and every room
/// on this one is Vale's.
///
/// Each room is an entrance with a camera box of its own, named for the door
/// it is entered by. Its door takes the exit of the same number back out.
///
/// Here are the rooms in story order, by the exits that lead in:
///
/// - From the village (SceneId_Vale, and on the stormy night
///   SceneId_ValePrologue): the house of Garet's family
///   (COTTAGE_ROOMS_A_FROM_VALE_EAST), his grandfather the mayor, his
///   grandmother and his parents; the house of Jenna's grandparents
///   (COTTAGE_ROOMS_A_FROM_VALE_NORTH); and two more houses
///   (COTTAGE_ROOMS_A_FROM_VALE_SOUTH and COTTAGE_ROOMS_A_FROM_VALE_WEST).
/// - From the part of Vale with the Psynergy Stone (SceneId_ValePlaza): the
///   shops (COTTAGE_ROOMS_A_FROM_VALE_PLAZA_WEST), where the item, weapon and
///   armor shops each open across a counter, and three houses
///   (COTTAGE_ROOMS_A_FROM_VALE_PLAZA_EAST,
///   COTTAGE_ROOMS_A_FROM_VALE_PLAZA_NORTH and
///   COTTAGE_ROOMS_A_FROM_VALE_PLAZA_SOUTH).
/// - Unused: a ninth room drawn at the sheet's lower left, whose camera box no
///   entrance gives, so no door leads in.
///
/// The actor placements and triggers follow the story:
///
/// - The stormy night of the prologue (FLAG_VALE_STORM). Everyone is out.
///   CottageRoomsA_Initialize removes every resident and flashes the
///   lightning outside. The doors out to the village lead into the storm's
///   scene (SceneId_ValePrologue) instead (CottageRoomsA_GetExits). The jars
///   and the barrel are empty, and six more bookcases than by day can be
///   read, a notebook called "Study of Alchemy" among them.
/// - Three years later, the day the story begins. The villagers talk of
///   Kraden's visitors and of Vale's law that keeps Psynergy secret from
///   outsiders. The jars and the barrel hold coins, an Antidote and a Smoke
///   Bomb, each found once.
/// - After the party has left Vale (FLAG_VALE_PARTY_LEFT), Jenna having been
///   taken on Mt. Aleph. Her grandmother lies sick in bed (Initialize, from
///   COTTAGE_ROOMS_A_FROM_VALE_NORTH). The villagers talk of the quest and of
///   Felix, and the item merchant gives Isaac a Herb as a farewell gift
///   (FLAG_VALE_FAREWELL_HERB_RECEIVED).
/// - Back from Kalay (FLAG_KALAY_REACHED), with Ivan and Mia in the party. The
///   first time the party enters Garet's house, CottageRoomsA_GaretComesHome
///   plays (FLAG_GARET_CAME_HOME). His parents are wondering whether to look
///   for him when his grandmother sees him. The family greets him, Ivan and
///   Mia introduce themselves ("I am Mia, from Imil."), and his father is left
///   pacing. The villagers now worry about Dora, ill since Isaac left.
///
/// While Garet's grandfather dozes in CottageRoomsA_GaretComesHome, three
/// letters Z (ACTOR_FIRST_SLEEP_Z and the two after it) rise from him. One
/// appears at each of frames 10, 20 and 30 of every 180
/// (CottageRoomsA_SpawnSleepZ).
///
/// Each starts 24 pixels above him at two fifths of its size. It grows a
/// fortieth a frame as it climbs ever more slowly and drifts right ever faster
/// (CottageRoomsA_AdvanceSleepZ).

#include "script_operands.h"
#include "main_routine_work.h"
#include "module/import.h"
#include "module/scene/cottage_rooms_a.h"
#include "module/field/field_sequence.h"
#include "module/field/event_commands.h"
#include "module/field/field.h"
#include "module/system/system.h"
#include "event.h"
#include "scene_tables.h"
#include "field_blend.h"
#include "event_inline.h"
#include "text/msg_ids.h"
#include "types.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "actor_script.h"
#include "fixed_math.h"
#include "ram_buffer.h"
#include "flags.h"
#include "sounds.h"
#include "heap.h"

/// The placed actors, numbered in the order of the actor placement tables,
/// which all place them alike.
///
/// gCottageRoomsA_ActorPlacementsAfterKalay adds Garet, Ivan and Mia, who keep
/// their characters' ids, and the drifting sprites.
///
/// The villagers' notes quote what they say on the day the story begins.
enum CottageRoomsAActorId {
  /// Garet's grandfather, the mayor, an old man with white hair and a cane
  /// ("your grandpa's the mayor", vale.c). "Isaac, please take care of my
  /// grandson, Garet."
  ACTOR_GARETS_GRANDFATHER = ACTOR_FIRST_PLACED,
  /// Garet's grandmother, an old woman with white hair. "Grandma! What's the
  /// matter? Are you choking?"
  ACTOR_GARETS_GRANDMOTHER,
  /// Garet's father, a man with brown hair in a teal shirt.
  /// "I'm proud of you, Garet. My own son...on such an important quest!"
  ACTOR_GARETS_FATHER,
  /// Garet's mother, a woman with orange hair under a green headscarf. "As
  /// mothers, Dora and I will always worry about you."
  ACTOR_GARETS_MOTHER,
  /// Jenna's grandfather, a bent old man with a white beard and a cane. "My
  /// granddaughter, Jenna, is finally happy again..."
  ACTOR_JENNAS_GRANDFATHER,
  /// Jenna's grandmother, an old woman with white hair. "Please return my
  /// precious granddaughter, Jenna, to me." Once Jenna is taken,
  /// ACTOR_WOMAN_ON_THE_SPITTING_IMAGE says she "grew sick with despair", and
  /// she lies in bed.
  ACTOR_JENNAS_GRANDMOTHER,
  /// A woman in a green dress in Jenna's grandparents' house. "Jenna is the
  /// spitting image of her mother, whom we lost that day..."
  ACTOR_WOMAN_ON_THE_SPITTING_IMAGE,
  /// A burly grey-haired man. "So Kraden came to Vale to study Mt. Aleph,
  /// right?"
  ACTOR_MAN_ON_KRADENS_STUDIES,
  /// A woman in a green dress. "If there are visitors in town, we can't use our
  /// Psynergy."
  ACTOR_WOMAN_ON_VISITORS_IN_TOWN,
  /// A young man with brown hair. "Do you ever dream of seeing the world
  /// outside the village?"
  ACTOR_YOUNG_MAN_DREAMING_OF_LEAVING,
  /// A woman with fair hair. "Few people outside of Vale possess the power of
  /// Psynergy. That's why we need permission to leave the village..."
  ACTOR_WOMAN_ON_PERMISSION_TO_LEAVE,
  /// A strongman. "I hear old man Kraden's got some visitors. Let's go use our
  /// Psynergy to scare 'em!"
  ACTOR_STRONGMAN_SCARING_VISITORS,
  ACTOR_ITEM_MERCHANT,
  /// A bald man in a red tunic.
  ACTOR_WEAPON_MERCHANT,
  /// A young man with brown hair in a green shirt.
  ACTOR_ARMOR_MERCHANT,
  /// Three letters Z that drift up from Garet's grandfather as he sleeps.
  ACTOR_FIRST_SLEEP_Z
};

/// Animations of the houses' sprites beyond enum ActorAnimation, as their
/// animation tables and frames under graphics/sprite draw them.
enum CottageRoomsAAnimation {
  /// Garet's grandfather standing with his eyes shut, asleep
  /// (old_man_with_white_hair.png).
  ANIM_GRANDFATHER_ASLEEP = 5,
  /// Garet's grandfather glancing quickly from side to side
  /// (old_man_with_white_hair.png).
  ANIM_GRANDFATHER_LOOK_ABOUT = 6,
  /// Jenna's grandmother lying in bed, only her head on the pillow
  /// (old_woman.png).
  ANIM_GRANDMOTHER_IN_BED = 5
};

// Forward declarations.
static void CottageRoomsA_GaretComesHome(void);
static void CottageRoomsA_SpawnSleepZ(void);

#include "shared/actor_facing.h"

/// Moves and grows a Z drifting up from Garet's sleeping grandfather.
///
/// The actor's timer and work are its x and y steps in signed 8.8 pixels, set
/// by CottageRoomsA_SpawnSleepZ.
static s32 CottageRoomsA_AdvanceSleepZ(struct Actor *actor) {

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

/// Selects the actor placements for the current visit to Vale, and marks
/// which of their actors are in view.
///
/// The visit is the day the story begins, after the party has left Vale
/// (FLAG_VALE_PARTY_LEFT), or after Kalay (FLAG_KALAY_REACHED).
const struct ActorPlacement *CottageRoomsA_GetActorPlacements(void) {
  struct ActorPlacement *table;

  if (GameFlag_Get(FLAG_KALAY_REACHED)) {
    table = gCottageRoomsA_ActorPlacementsAfterKalay;
  } else if (GameFlag_Get(FLAG_VALE_PARTY_LEFT)) {
    table = gCottageRoomsA_ActorPlacementsAfterLeaving;
  } else {
    table = gCottageRoomsA_ActorPlacements;
  }
  Scene_AssignViewFlags(table);
  return table;
}

/// Selects the triggers for the current visit to Vale.
///
/// The visit is the stormy night (FLAG_VALE_STORM), after Kalay, after the
/// party has left, or the day the story begins.
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
///
/// He stands in a house of Vale on the day the story begins. Kraden came to
/// study Mt. Aleph.
///
/// Reached by talking to ACTOR_MAN_ON_KRADENS_STUDIES on the day the story
/// begins.
static void CottageRoomsA_AskAboutKradensStudies(void) {
  Event_Start();
  Event_SetMessageId(MsgValeKradenCameToStudyMt);
  Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_MAN_ON_KRADENS_STUDIES, 6);
  // "So Kraden came to Vale to study Mt. Aleph, right?"
  // Yes: "He said he was studying Mt. Aleph and Alchemy and the..."
  // No: "But I thought... Well, whatever. If he furthers the wisdom..."
  Event_AskYesNo(ACTOR_MAN_ON_KRADENS_STUDIES, 0);
  Event_End();
}

/// A strongman asks Isaac to help scare Kraden's visitors with Psynergy.
///
/// He stands in a house of Vale on the day the story begins.
///
/// Reached by talking to ACTOR_STRONGMAN_SCARING_VISITORS on the day the
/// story begins.
static void CottageRoomsA_AskToScareVisitors(void) {
  Event_Start();
  Event_SetMessageId(MsgValeLetsScareKradensVisitors);
  Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_STRONGMAN_SCARING_VISITORS, 6);
  // "I hear old man Kraden's got some visitors. Let's go use our..."
  // Yes: "You're more of a prankster than I thought, Isaac. I like that."
  // No: "What? You're too serious, Isaac... Only someone who has..."
  Event_AskYesNo(ACTOR_STRONGMAN_SCARING_VISITORS, 0);
  Event_End();
}

/// In Vale's shops after the party has left Vale, the item merchant gives
/// Isaac a Herb as a farewell gift, and afterwards urges him to save Jenna.
///
/// Called by CottageRoomsA_TalkToItemMerchant away from the counter, after the
/// party has left and before Kalay. It gives the Herb and sets
/// FLAG_VALE_FAREWELL_HERB_RECEIVED once. Afterwards the merchant turns to
/// watch the leader.
static void CottageRoomsA_GiveFarewellHerb(void) {
  s32 gift;

  Event_Start();
  if (Event_GetFlag(FLAG_VALE_FAREWELL_HERB_RECEIVED) != 0) {
    Event_SetMessageId(MsgValeYouMustSaveJenna);
    // "You must save Jenna! You must, Isaac!"
    Event_ShowMessage(ACTOR_ITEM_MERCHANT, 0);
    FieldActor_WatchAndStartScript(ACTOR_ITEM_MERCHANT,
                                   ACTOR_WATCH_KEEP_MOTION | ACTOR_PARTY_LEADER,
                                   gCottageRoomsA_FaceTargetScript);
  } else {
    gift = MsgValeThisIsMyFarewellGift;
    Event_SetMessageId(gift);
    // "This is my farewell gift."
    Event_ShowMessageThenWait(ACTOR_ITEM_MERCHANT, 0, 20);
    FieldMessage_Show((gift + 1), 1);
    Party_GiveItem(ITEM_HERB, 0);
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
  // "They say the people who caused this whole mess had strange..."
  // Yes: "If they were using Psynergy... That means there are towns..."
  // No: "If that's the case, then I don't need to worry. It means..."
  Event_AskYesNo(ACTOR_WOMAN_ON_VISITORS_IN_TOWN, 0);
  Event_End();
}

/// Garet's father asks whether Isaac heard them talking about Dora.
///
/// He is in Garet's house in Vale, back from Kalay.
///
/// Reached by talking to Garet's father after Kalay.
static void CottageRoomsA_AskAboutDora(void) {
  Event_Start();
  Event_SetMessageId(MsgValeDidYouHearAboutDora);
  // "Did you hear us talking about Dora just now?"
  // Yes: "Dora told us not to worry...that it's just a fever. Don't..."
  // No: "You didn't hear? But you look concerned. We'll be looking..."
  Event_AskYesNo(ACTOR_GARETS_FATHER, 0);
  Event_End();
}

/// Garet's mother hopes the travelers didn't get sick on the way.
///
/// She is in Garet's house in Vale, back from Kalay.
///
/// Reached by talking to Garet's mother after Kalay.
static void CottageRoomsA_WorryAboutTravelersHealth(void) {
  Event_Start();
  Event_SetMessageId(MsgValeHopeDidntGet);
  // "I hope you didn't get sick during your travels... Garet..."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_GARETS_MOTHER, 0);
  Event_End();
}

/// Leaves a house through its door.
///
/// The exit sound plays and the screen closes. On the stormy night the
/// lightning stops first.
static void CottageRoomsA_RequestWindowExit(s32 exitNumber) {
  if (GameFlag_Get(FLAG_VALE_STORM) != 0)
    Weather_StopLightning();
  Sound_Play(SE_MAP_EXIT);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  gFieldWork->transitionFrames = 16;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(exitNumber);
}

// The rooms' doors: stepping on a door's cell, whose cell behavior is its
// exit's number, takes that exit back out, in every table. On the stormy night
// the exits to SceneId_Vale lead to the storm's scene (SceneId_ValePrologue)
// instead.

/// Leaves the house where a young man dreams of leaving.
///
/// It's the room entered from COTTAGE_ROOMS_A_FROM_VALE_WEST, where
/// ACTOR_YOUNG_MAN_DREAMING_OF_LEAVING wanders.
static void CottageRoomsA_LeaveToValeWest(void) {
  CottageRoomsA_RequestWindowExit(COTTAGE_ROOMS_A_TO_VALE_WEST);
}

/// Leaves Jenna's grandparents' house.
static void CottageRoomsA_LeaveToValeNorth(void) {
  CottageRoomsA_RequestWindowExit(COTTAGE_ROOMS_A_TO_VALE_NORTH);
}

/// Leaves the empty house.
///
/// It's the room entered from COTTAGE_ROOMS_A_FROM_VALE_PLAZA_NORTH, which no
/// actor placement table puts anyone in.
static void CottageRoomsA_LeaveToValePlazaWest(void) {
  CottageRoomsA_RequestWindowExit(COTTAGE_ROOMS_A_TO_VALE_PLAZA_WEST);
}

/// Leaves Vale's shops.
static void CottageRoomsA_LeaveToValePlazaSouth(void) {
  CottageRoomsA_RequestWindowExit(COTTAGE_ROOMS_A_TO_VALE_PLAZA_SOUTH);
}

/// Leaves the house where villagers talk of Kraden and his visitors.
///
/// It's the room entered from COTTAGE_ROOMS_A_FROM_VALE_SOUTH, where
/// ACTOR_MAN_ON_KRADENS_STUDIES and ACTOR_WOMAN_ON_VISITORS_IN_TOWN stand.
static void CottageRoomsA_LeaveToValeSouth(void) {
  CottageRoomsA_RequestWindowExit(COTTAGE_ROOMS_A_TO_VALE_SOUTH);
}

/// Leaves Garet's house.
static void CottageRoomsA_LeaveToValeEast(void) {
  CottageRoomsA_RequestWindowExit(COTTAGE_ROOMS_A_TO_VALE_EAST);
}

/// Leaves the house where a strongman plans to scare Kraden's visitors.
///
/// It's the room entered from COTTAGE_ROOMS_A_FROM_VALE_PLAZA_SOUTH, where
/// ACTOR_STRONGMAN_SCARING_VISITORS stands.
static void CottageRoomsA_LeaveToValePlazaEast(void) {
  CottageRoomsA_RequestWindowExit(COTTAGE_ROOMS_A_TO_VALE_PLAZA_EAST);
}

/// Leaves the house where a woman talks of permission to leave.
///
/// It's the room entered from COTTAGE_ROOMS_A_FROM_VALE_PLAZA_EAST, where
/// ACTOR_WOMAN_ON_PERMISSION_TO_LEAVE stands.
static void CottageRoomsA_LeaveToValePlazaNorth(void) {
  CottageRoomsA_RequestWindowExit(COTTAGE_ROOMS_A_TO_VALE_PLAZA_NORTH);
}

/// Sets up a house of Vale as the party arrives.
///
/// On the stormy night the residents are gone and lightning flashes outside.
/// Back from Kalay, entering Garet's house the first time plays his
/// homecoming.
///
/// This is the scene's initialize entry. It runs CottageRoomsA_GaretComesHome
/// from COTTAGE_ROOMS_A_FROM_VALE_EAST after Kalay until FLAG_GARET_CAME_HOME
/// is set. From COTTAGE_ROOMS_A_FROM_VALE_NORTH, once the party has left
/// Vale, Jenna's grandmother lies sick in bed.
s32 CottageRoomsA_Initialize(void) {
  // The heap slots from the field's on.
  void **slots = (void **)&gFieldWork;

  ((struct FieldWork *)slots[0])->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  // On the stormy night every resident is out, and lightning flashes outside.
  if (GameFlag_Get(FLAG_VALE_STORM) != 0) {
    Actor_SetPosition(ACTOR_GARETS_GRANDFATHER, 0, 0);
    Actor_SetPosition(ACTOR_GARETS_GRANDMOTHER, 0, 0);
    Actor_SetPosition(ACTOR_GARETS_FATHER, 0, 0);
    Actor_SetPosition(ACTOR_GARETS_MOTHER, 0, 0);
    Actor_SetPosition(ACTOR_JENNAS_GRANDFATHER, 0, 0);
    Actor_SetPosition(ACTOR_JENNAS_GRANDMOTHER, 0, 0);
    Actor_SetPosition(ACTOR_WOMAN_ON_THE_SPITTING_IMAGE, 0, 0);
    Actor_SetPosition(ACTOR_MAN_ON_KRADENS_STUDIES, 0, 0);
    Actor_SetPosition(ACTOR_WOMAN_ON_VISITORS_IN_TOWN, 0, 0);
    Actor_SetPosition(ACTOR_YOUNG_MAN_DREAMING_OF_LEAVING, 0, 0);
    Actor_SetPosition(ACTOR_WOMAN_ON_PERMISSION_TO_LEAVE, 0, 0);
    Actor_SetPosition(ACTOR_STRONGMAN_SCARING_VISITORS, 0, 0);
    Actor_SetPosition(ACTOR_ITEM_MERCHANT, 0, 0);
    Actor_SetPosition(ACTOR_WEAPON_MERCHANT, 0, 0);
    Actor_SetPosition(ACTOR_ARMOR_MERCHANT, 0, 0);
    Weather_StartLightning();
    // UB: the blend's slot is read past gFieldWork, a single pointer.
#ifdef UBFIX
    ((struct FieldBlendWork *)gHeapSlots[HEAP_SLOT_FIELD_BLEND])->loud = 1;
#else
    ((struct FieldBlendWork *)slots[HEAP_SLOT_FIELD_BLEND - HEAP_SLOT_FIELD])
        ->loud = 1;
#endif
    Weather_LastLightning();
    wait(30);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Weather_ResumeLightning();
  }
  // The returning party gets the household reunion only on its first visit.
  if (GameFlag_Get(FLAG_KALAY_REACHED) != 0) {
    if (gMainRoutineWork.entrance == COTTAGE_ROOMS_A_FROM_VALE_EAST) {
      if (GameFlag_Get(FLAG_GARET_CAME_HOME) == 0) {
        CottageRoomsA_GaretComesHome();
      }
    }
    Actor_Get(ACTOR_GARETS_FATHER)->collisionFlags |= ACTOR_BLOCKED_BY_SOLID;
  }
  // Once Jenna is taken, her grandmother lies sick in bed.
  if (gMainRoutineWork.entrance == COTTAGE_ROOMS_A_FROM_VALE_NORTH) {
    if (GameFlag_Get(FLAG_VALE_PARTY_LEFT) != 0) {
      Actor_SetPosition(ACTOR_JENNAS_GRANDMOTHER, 454, 150);
      Actor_SetSpriteFlags(Actor_Get(ACTOR_JENNAS_GRANDMOTHER), 0);
      Actor_SetAnimation(ACTOR_JENNAS_GRANDMOTHER, ANIM_GRANDMOTHER_IN_BED);
      TilesetAnim_Resume(4);
    }
  }
  return 0;
}

/// In Vale's shops, opens the weapon shop across the counter, or the
/// merchant speaks.
///
/// Reached by talking to the weapon merchant in every table but the stormy
/// night's. Facing north, across the counter, opens shop 1. Otherwise the
/// merchant's line follows the story.
static void CottageRoomsA_TalkToWeaponMerchant(void) {
  struct Actor *leader;

  leader = Actor_Get(ACTOR_PARTY_LEADER);

  if (FACING_IS_NORTH(leader->facing)) {
    Shop_Run(1, ACTOR_WEAPON_MERCHANT);
    return;
  }

  Event_Start();
  if (GameFlag_Get(FLAG_KALAY_REACHED) != 0) {
    Event_SetMessageId(MsgValeYouCameBack);
    // "Oh, it's you, Isaac. You came back?"
    // Yes: "Vale won't be very quiet anymore... But I'm glad to see you...."
    // No: "Oh, you're not back for good? That's too bad."
    Event_AskYesNo(ACTOR_WEAPON_MERCHANT, 0);
  } else {
    if (GameFlag_Get(FLAG_VALE_PARTY_LEFT) != 0) {
      Event_SetMessageId(MsgValeValeFeelsEmpty);
    } else {
      Event_SetMessageId(MsgValeGoodWeaponsDrawOutStrength);
    }
    Event_ShowMessage(ACTOR_WEAPON_MERCHANT, 0);
  }
  Event_End();
}

/// In Vale's shops, opens the armor shop across the counter, or the merchant
/// speaks.
///
/// Reached by talking to the armor merchant in every table but the stormy
/// night's. It opens shop 2 when facing north.
static void CottageRoomsA_TalkToArmorMerchant(void) {
  struct Actor *actor;

  actor = Actor_Get(ACTOR_PARTY_LEADER);
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
    Event_ShowMessage(ACTOR_ARMOR_MERCHANT, 0);
    Event_End();
  }
}

/// In Vale's shops, opens the item shop across the counter, or the merchant
/// speaks.
///
/// Reached by talking to the item merchant in every table but the stormy
/// night's. It opens shop 3 when facing north. After the party has left and
/// before Kalay, CottageRoomsA_GiveFarewellHerb speaks instead.
static void CottageRoomsA_TalkToItemMerchant(void) {
  struct Actor *actor;

  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (FACING_IS_NORTH(actor->facing)) {
    Shop_Run(3, ACTOR_ITEM_MERCHANT);
  } else {
    if (GameFlag_Get(FLAG_KALAY_REACHED) != 0) {
      Event_Start();
      Event_SetMessageId(MsgValeWhenDidYouComeBack);
      // "Isaac! When did you come back? After the way you left, I..."
      Event_ShowMessage(ACTOR_ITEM_MERCHANT, 0);
      Event_End();
    } else {
      if (GameFlag_Get(FLAG_VALE_PARTY_LEFT) != 0) {
        CottageRoomsA_GiveFarewellHerb();
      } else {
        Event_Start();
        Event_SetMessageId(MsgValeAWiseManFleesWhen);
        // "A wise man knows to flee from battle when he's injured."
        Event_ShowMessage(ACTOR_ITEM_MERCHANT, 0);
        Event_End();
      }
    }
  }
}

/// Garet's grandparents and father nod.
static inline void CottageRoomsA_FamilyNods(void) {
  Actor_SetAnimation(ACTOR_GARETS_GRANDFATHER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARETS_GRANDMOTHER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARETS_FATHER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_GARETS_GRANDMOTHER, ANIM_NOD);
}

/// Starts the repeated motion before waiting for the requested animation.
static inline void CottageRoomsA_PlayRepeatedAnimationAndWait(s32 actorId,
                                                              s32 animation) {
  Actor_StartRepeatedMotion(actorId, 1);
  Actor_SetAnimationAndWait(actorId, animation);
}

/// In Garet's house in Vale, back from Kalay, Garet comes home.
///
/// - His grandfather dozes, and his parents argue over whether to look for
///   him.
/// - His grandmother spots him first.
/// - Garet introduces Ivan and Mia.
/// - His mother asks them to look after him, and his father tells them to
///   enjoy their stay.
/// - The three walk back into Isaac, and the father is left pacing.
///
/// Called by CottageRoomsA_Initialize from COTTAGE_ROOMS_A_FROM_VALE_EAST
/// after Kalay, until FLAG_GARET_CAME_HOME is set. It sets that flag. It adds
/// and removes the task CottageRoomsA_SpawnSleepZ, and leaves the father
/// pacing.
static void CottageRoomsA_GaretComesHome(void) {
  struct Actor *sleepZ;
  struct Actor *actor;
  s32 motionFlags = 0;
  TaskFunc timedUpdate;
  const s32 *partyScript;

  // The party walks in while Garet's grandfather dozes, sleep drifting up
  // from him.
  Event_Start();
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  wait(1);
  Actor_SetSpritePriority(ACTOR_MIA, 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.4);
  Actor_SetSpeed(ACTOR_GARET, 0.4);
  Actor_SetSpeed(ACTOR_IVAN, 0.4);
  Actor_SetSpeed(ACTOR_MIA, 0.4);
  Actor_SetAnimation(ACTOR_GARETS_GRANDFATHER, ANIM_GRANDFATHER_ASLEEP);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 808, 508);
  sleepZ = Actor_Get(ACTOR_FIRST_SLEEP_Z);
  Actor_SetSpriteFlags(sleepZ, 0);
  sleepZ = Actor_Get(ACTOR_FIRST_SLEEP_Z + 1);
  Actor_SetSpriteFlags(sleepZ, 0);
  sleepZ = Actor_Get(ACTOR_FIRST_SLEEP_Z + 2);
  Actor_SetSpriteFlags(sleepZ, 0);
  Actor_Get(ACTOR_FIRST_SLEEP_Z)->motionFlags = motionFlags;
  Actor_Get(ACTOR_FIRST_SLEEP_Z + 1)->motionFlags = motionFlags;
  Actor_Get(ACTOR_FIRST_SLEEP_Z + 2)->motionFlags = motionFlags;
  timedUpdate = CottageRoomsA_SpawnSleepZ;
  Task_AddFrame(timedUpdate, TASK_PASS_FRAME);
  wait(1);
  gFieldWork->transitionFrames = 32;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetPositionFixed(ACTOR_GARET, actor->x.fixed, actor->z.fixed);
  }
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetPositionFixed(ACTOR_IVAN, actor->x.fixed, actor->z.fixed);
  }
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetPositionFixed(ACTOR_MIA, actor->x.fixed, actor->z.fixed);
  }
  Actor_WalkTo(ACTOR_GARET, 792, 512);
  Actor_WalkTo(ACTOR_IVAN, 824, 504);
  Actor_WalkToAndWait(ACTOR_MIA, 818, 524);
  Actor_SetAnimation(ACTOR_GARET, ANIM_STAND);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_STAND);
  Event_Wait(10);
  FieldActor_WatchAndStartScript(ACTOR_PARTY_LEADER,
                                 ACTOR_WATCH_KEEP_MOTION | ACTOR_GARETS_FATHER,
                                 gCottageRoomsA_FaceTargetScript);
  FieldActor_WatchAndStartScript(ACTOR_GARET,
                                 ACTOR_WATCH_KEEP_MOTION | ACTOR_GARETS_FATHER,
                                 gCottageRoomsA_FaceTargetScript);
  FieldActor_WatchAndStartScript(ACTOR_IVAN,
                                 ACTOR_WATCH_KEEP_MOTION | ACTOR_GARETS_FATHER,
                                 gCottageRoomsA_FaceTargetScript);
  FieldActor_WatchAndStartScript(ACTOR_MIA,
                                 ACTOR_WATCH_KEEP_MOTION | ACTOR_GARETS_FATHER,
                                 gCottageRoomsA_FaceTargetScript);
  Event_Wait(300);
  FieldCamera_GetActor()->motionFlags = motionFlags;
  // The camera pans slowly up to Garet's parents. His father, pacing, asks
  // whether they should look for Garet; Isaac still does not know about
  // Dora. His mother teases that he misses Garet.
  FieldCamera_SetSpeed(0.1);
  FieldCamera_MoveTo(786, 0, 430, 1);
  Event_Wait(240);
  Actor_Stop(ACTOR_GARETS_FATHER);
  Actor_ShowEmote(ACTOR_GARETS_FATHER, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  Actor_WalkToAndWait(ACTOR_GARETS_FATHER, 819, 405);
  Event_Wait(40);
  Actor_SetAnimationAndWait(ACTOR_GARETS_FATHER, ANIM_SHAKE_HEAD);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_GARETS_FATHER, FACING_NORTH_NORTHEAST, 20);
  Event_SetMessageId(MsgValeHey2);
  // "Hey..."
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT |
                                     ACTOR_GARETS_FATHER,
                                 0, 20);
  Actor_Stop(ACTOR_PARTY_LEADER);
  Actor_Stop(ACTOR_GARET);
  Actor_Stop(ACTOR_IVAN);
  Actor_Stop(ACTOR_MIA);
  Actor_ShowEmote(ACTOR_GARETS_MOTHER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  // "What?"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_GARETS_MOTHER,
                                 0, 20);
  Actor_RunRepeatedMotion(ACTOR_GARETS_FATHER, 2);
  Event_Wait(40);
  // "Shouldn't we look for Garet?"
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT |
                                     ACTOR_GARETS_FATHER,
                                 0, 10);
  Actor_FaceDirection(ACTOR_GARETS_MOTHER, FACING_SOUTH_SOUTHWEST, 10);
  // "Why?"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_GARETS_MOTHER,
                                 0, 40);
  Actor_RunRepeatedMotion(ACTOR_GARETS_FATHER, 2);
  Event_Wait(20);
  // "Isaac still doesn't know about Dora...right? Someone should..."
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT |
                                     ACTOR_GARETS_FATHER,
                                 0, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  Actor_ShowEmote(ACTOR_GARETS_MOTHER, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 40);
  // "You miss Garet, don't you? You're looking for an excuse to..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_GARETS_MOTHER,
                                 0, 40);
  Actor_StartRepeatedMotion(ACTOR_GARETS_FATHER, 2);
  Actor_ShowEmote(ACTOR_GARETS_FATHER, EMOTE_IN_FRONT | EMOTE_WINCE, 20);
  Actor_SetAnimation(ACTOR_GARETS_FATHER, ANIM_SHAKE_HEAD);
  // "What? No! I just..."
  Event_ShowMessageThenWait(SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT |
                                ACTOR_GARETS_FATHER,
                            0, 10);
  CottageRoomsA_PlayRepeatedAnimationAndWait(ACTOR_GARETS_MOTHER, ANIM_NOD);
  Event_Wait(20);
  CottageRoomsA_PlayRepeatedAnimationAndWait(ACTOR_GARETS_FATHER,
                                             ANIM_SHAKE_HEAD);
  CottageRoomsA_PlayRepeatedAnimationAndWait(ACTOR_GARETS_MOTHER, ANIM_NOD);
  CottageRoomsA_PlayRepeatedAnimationAndWait(ACTOR_GARETS_FATHER,
                                             ANIM_SHAKE_HEAD);
  // Garet's grandmother sees him and chokes on her words; his mother asks if
  // she is choking, his father thinks she misses Garet too.
  Actor_ShowEmote(ACTOR_GARETS_GRANDMOTHER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_RunRepeatedMotion(ACTOR_GARETS_GRANDMOTHER, 1);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_GARETS_GRANDMOTHER, FACING_EAST_SOUTHEAST, 40);
  Actor_RunRepeatedMotion(ACTOR_GARETS_GRANDMOTHER, 2);
  Event_Wait(60);
  Actor_RunRepeatedMotion(ACTOR_GARETS_GRANDMOTHER, 3);
  Event_Wait(40);
  FieldEvent_ShowMessageThenWait(
      SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_GRANDMOTHER, 0, 40);
  Actor_SetAnimation(ACTOR_GARETS_MOTHER, ANIM_STILL);
  Actor_RunRepeatedMotion(ACTOR_GARETS_MOTHER, 2);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_GARETS_MOTHER,
                                 0, 10);
  Actor_SetAnimationAndWait(ACTOR_GARETS_GRANDMOTHER, ANIM_SHAKE_HEAD);
  Actor_RunRepeatedMotion(ACTOR_GARETS_GRANDMOTHER, 2);
  FieldEvent_ShowMessageThenWait(
      SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_GRANDMOTHER, 0, 10);
  Actor_ShowEmote(ACTOR_GARETS_FATHER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Actor_FaceDirection(ACTOR_GARETS_FATHER, FACING_SOUTH_SOUTHWEST, 40);
  Actor_SetAnimationAndWait(ACTOR_GARETS_FATHER, ANIM_NOD);
  Event_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_FATHER, 0, 10);
  // No, it really is Garet: both parents turn and call his name together.
  Actor_SetAnimationAndWait(ACTOR_GARETS_GRANDMOTHER, ANIM_SHAKE_HEAD);
  Actor_FaceDirection(ACTOR_GARETS_GRANDMOTHER, FACING_NORTH_NORTHEAST, 10);
  Actor_Jump(ACTOR_GARETS_GRANDMOTHER, 2, 0);
  Actor_SetAnimation(ACTOR_GARETS_GRANDMOTHER, ANIM_SHAKE_HEAD);
  FieldEvent_ShowMessageThenWait(
      SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_GRANDMOTHER, 0, 10);
  Actor_ShowEmote(ACTOR_GARETS_MOTHER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_GARETS_FATHER, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_FaceDirection(ACTOR_GARETS_FATHER, FACING_NORTH_NORTHEAST, 80);
  Actor_FaceDirection(ACTOR_GARETS_FATHER, FACING_SOUTH_SOUTHWEST, 60);
  Actor_StartRepeatedMotion(ACTOR_GARETS_FATHER, 2);
  Actor_StartRepeatedMotion(ACTOR_GARETS_MOTHER, 2);
  FieldEvent_ShowTwoMessagesAndWait(ACTOR_GARETS_FATHER, 11, 6, 6, 6,
                                    ACTOR_GARETS_MOTHER, 12, 1, 7, 1, 0);
  Event_Wait(20);
  FieldCamera_SetSpeed(1.6);
  FieldCamera_MoveTo(777, 0, 468, 1);
  FieldCamera_WaitForMove();
  Event_Wait(40);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_GARET, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 2);
  // His grandfather wakes, and the sleep stops drifting.
  Task_Remove(timedUpdate);
  Event_Wait(40);
  Actor_SetAnimationAndWait(ACTOR_GARETS_GRANDFATHER,
                            ANIM_GRANDFATHER_LOOK_ABOUT);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(
      SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_GRANDFATHER, 0, 20);
  // The grandfather looks about, then jumps up with Garet's name.
  FieldCamera_MoveTo(750, 0, 451, 1);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_GARETS_MOTHER, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARETS_FATHER, FACING_SOUTH_SOUTHWEST, 10);
  Actor_FaceDirection(ACTOR_GARETS_GRANDFATHER, FACING_EAST_SOUTHEAST, 40);
  Actor_ShowEmote(ACTOR_GARETS_GRANDFATHER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                  40);
  Actor_FaceDirection(ACTOR_GARETS_GRANDFATHER, FACING_SOUTH_SOUTHEAST, 20);
  Actor_FaceDirection(ACTOR_GARETS_GRANDFATHER, FACING_EAST_SOUTHEAST, 20);
  Actor_FaceDirection(ACTOR_GARETS_GRANDFATHER, FACING_SOUTH_SOUTHEAST, 40);
  Actor_SetAnimationAndWait(ACTOR_GARETS_GRANDFATHER,
                            ANIM_GRANDFATHER_LOOK_ABOUT);
  Event_Wait(60);
  Actor_Jump(ACTOR_GARETS_GRANDFATHER, 6, 0);
  FieldEvent_ShowMessageThenWait(
      SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_GRANDFATHER, 0, 20);
  // Garet goes to him; the grandfather asks whether the Wise One's mission
  // is done, and a no skips one line of Garet's father.
  Actor_SetSpeed(ACTOR_GARET, 1.6);
  Actor_WalkToAndWait(ACTOR_GARET, 789, 473);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST_SOUTHWEST, 20);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0, 10);
  Actor_FaceDirection(ACTOR_GARETS_GRANDFATHER, FACING_EAST_SOUTHEAST, 20);
  Actor_SetAnimationAndWait(ACTOR_GARETS_GRANDFATHER, ANIM_NOD);
  Event_OpenMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_GRANDFATHER, 0);
  Actor_FaceDirection(ACTOR_GARETS_FATHER, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARETS_GRANDMOTHER, FACING_EAST_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH_NORTHWEST, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 1) {
    gFieldWork->message++;
  }
  FieldCamera_MoveTo(777, 0, 428, 1);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_GARETS_FATHER, 2);
  Event_ShowMessage(ACTOR_GARETS_FATHER, 0);
  Actor_SetAnimationAndWait(ACTOR_GARETS_MOTHER, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // His mother knows he is leaving again soon; his grandparents are glad to
  // see him well.
  Event_SetMessageId(MsgValeYoureLeavingAgainSoon);
  // "You're leaving again soon...aren't you?"
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_GARETS_MOTHER, 0);
  FieldCamera_MoveTo(777, 0, 468, 1);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH_NORTHEAST, 20);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_GARETS_GRANDMOTHER, ANIM_SHAKE_HEAD);
  Actor_FaceDirection(ACTOR_GARETS_GRANDMOTHER, FACING_NORTH_NORTHEAST, 10);
  // "I'm just glad to see you're doing well!"
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_GRANDMOTHER, 0);
  Actor_SetAnimationAndWait(ACTOR_GARETS_GRANDFATHER, ANIM_NOD);
  // "That's right."
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_GRANDFATHER, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST_SOUTHWEST, 10);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Actor_FaceDirection(ACTOR_GARETS_GRANDMOTHER, FACING_EAST_SOUTHEAST, 10);
  Actor_SetAnimation(ACTOR_GARETS_MOTHER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARETS_FATHER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARETS_GRANDMOTHER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_GARETS_GRANDFATHER, ANIM_NOD);
  Event_Wait(20);
  // Garet introduces his new friends.
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHEAST, 20);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST_SOUTHWEST, 20);
  // "Let me introduce you to my new friends."
  Event_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0, 20);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHEAST, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 40);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 20);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_GARET, FACING_EAST_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
  // Ivan, then Mia, step forward and introduce themselves; the family nods.
  Actor_SetSpeed(ACTOR_IVAN, 1);
  Actor_WalkToAndWait(ACTOR_IVAN, 819, 489);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH_NORTHWEST, 40);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  // "Umm... I am Ivan. Pleased to meet you."
  Event_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  CottageRoomsA_FamilyNods();
  Actor_Get(ACTOR_MIA)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  Actor_SetSpritePriority(ACTOR_MIA, 1);
  Actor_SetSpeed(ACTOR_MIA, 1);
  Actor_WalkToAndWait(ACTOR_MIA, 794, 520);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_WalkToAndWait(ACTOR_MIA, 784, 496);
  Actor_FaceDirection(ACTOR_MIA, FACING_WEST_NORTHWEST, 10);
  Actor_Get(ACTOR_MIA)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  CottageRoomsA_FamilyNods();
  Event_Wait(20);
  FieldCamera_MoveTo(777, 0, 428, 1);
  Event_Wait(20);
  // Garet's mother comes forward and asks them to look after young Garet.
  Actor_SetSpeed(ACTOR_GARETS_MOTHER, 0.4);
  Actor_WalkToAndWait(ACTOR_GARETS_MOTHER, 835, 388);
  Actor_FaceDirection(ACTOR_GARETS_MOTHER, FACING_SOUTH_SOUTHWEST, 0);
  Actor_ShowEmote(ACTOR_GARETS_MOTHER, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_GARETS_MOTHER, 0,
                            20);
  FieldCamera_MoveTo(777, 0, 468, 1);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_EAST_NORTHEAST, 40);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH_NORTHEAST, 20);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_Wait(20);
  // His father: enough chit-chat, relax and enjoy your stay.
  Actor_RunRepeatedMotion(ACTOR_GARETS_FATHER, 1);
  Event_Wait(20);
  Actor_SetAnimation(ACTOR_GARETS_FATHER, ANIM_NOD);
  Event_ShowMessageThenWait(ACTOR_GARETS_FATHER, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH_NORTHEAST, 40);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_Wait(20);
  Actor_SetSpeed(ACTOR_IVAN, 1);
  partyScript = gCottageRoomsA_PartyLeavesScript;
  Actor_SetScript(ACTOR_GARET, partyScript);
  Actor_SetScript(ACTOR_IVAN, partyScript);
  Actor_RunScriptAndWait(ACTOR_MIA, partyScript);
  Actor_SetScript(ACTOR_GARETS_FATHER, gCottageRoomsA_FatherPacingScript);
  Actor_WalkToAndWait(ACTOR_GARETS_MOTHER, 837, 376);
  Actor_FaceDirection(ACTOR_GARETS_MOTHER, FACING_NORTH_NORTHEAST, 20);
  GameFlag_Set(FLAG_GARET_CAME_HOME);
  Event_End();
}

/// Sends a sprite drifting up from Garet's sleeping grandfather.
///
/// It sends one of the three at frames 10, 20 and 30 of every 180.
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
  actor = Actor_Get(actorId);
  if (actor == NULL) {
    return;
  }
  origin = Actor_Get(ACTOR_GARETS_GRANDFATHER);
  if (origin != NULL) {
    Actor_SetPositionFixed(actorId, origin->x.fixed, origin->z.fixed);
  }
  // Restart this sprite above the origin with a small scale and 8.8 motion.
  actor->scaleX = 2 * Q16_ONE / 5;
  actor->scaleY = 2 * Q16_ONE / 5;
  {
    s32 height = actor->y.fixed + px(24);

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
    SC_SPEED(0.6),
    SC_ACCELERATION(0.3),
    SC_ANIMATION(2),
    SC_MOVE(784, 0, 405),
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
    SC_MOVE(819, 0, 405),
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
    SC_ANIMATION(2),      SC_MOVE(808, 0, 508), SC_SYNC, SC_ANIMATION(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),           SC_END,
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
    SCENE_EXIT(COTTAGE_ROOMS_A_TO_VALE_WEST, ValePrologue,
               VALE_PROLOGUE_FROM_COTTAGE_ROOMS_A_WEST),
    SCENE_EXIT(COTTAGE_ROOMS_A_TO_VALE_NORTH, ValePrologue,
               VALE_PROLOGUE_FROM_COTTAGE_ROOMS_A_NORTH),
    SCENE_EXIT(COTTAGE_ROOMS_A_TO_VALE_PLAZA_WEST, ValePlaza,
               VALE_PLAZA_FROM_COTTAGE_ROOMS_A_WEST),
    SCENE_EXIT(COTTAGE_ROOMS_A_TO_VALE_PLAZA_SOUTH, ValePlaza,
               VALE_PLAZA_FROM_COTTAGE_ROOMS_A_SOUTH),
    SCENE_EXIT(COTTAGE_ROOMS_A_TO_VALE_SOUTH, ValePrologue,
               VALE_PROLOGUE_FROM_COTTAGE_ROOMS_A_SOUTH),
    SCENE_EXIT(COTTAGE_ROOMS_A_TO_VALE_EAST, ValePrologue,
               VALE_PROLOGUE_FROM_COTTAGE_ROOMS_A_EAST),
    SCENE_EXIT(COTTAGE_ROOMS_A_TO_VALE_PLAZA_EAST, ValePlaza,
               VALE_PLAZA_FROM_COTTAGE_ROOMS_A_EAST),
    SCENE_EXIT(COTTAGE_ROOMS_A_TO_VALE_PLAZA_NORTH, ValePlaza,
               VALE_PLAZA_FROM_COTTAGE_ROOMS_A_NORTH),
    SCENE_EXITS_END,
};

// clang-format off
struct ActorPlacement gCottageRoomsA_ActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_WHITE_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        697, 0, 468, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        726, 0, 486, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_TEAL_SHIRT, CONDITION_IN_VIEW, MOVEMENT_TYPE_WANDER,
        815, 0, 386, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_HEADSCARF, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        841, 0, 455, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        403, 0, 144, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        331, 0, 144, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        486, 0, 136, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        495, 0, 453, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        405, 0, 421, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_WANDER,
        99, 0, 125, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        768, 0, 768, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        392, 0, 697, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_PONYTAIL, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        72, 0, 424, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_MAN_IN_RED, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        232, 0, 440, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_IN_GREEN, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        264, 0, 440, FACING_SOUTH, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gCottageRoomsA_ActorPlacementsAfterLeaving[] = {
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_WHITE_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        697, 0, 468, FACING_SOUTH_SOUTHEAST, 3, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        726, 0, 486, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_TEAL_SHIRT, CONDITION_IN_VIEW, MOVEMENT_TYPE_WANDER,
        815, 0, 386, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_HEADSCARF, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        841, 0, 455, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        403, 0, 144, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        456, 0, 156, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        486, 0, 118, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        495, 0, 453, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        405, 0, 421, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_WANDER,
        99, 0, 125, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        768, 0, 768, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        392, 0, 697, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_PONYTAIL, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        72, 0, 424, FACING_SOUTH, 3, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_MAN_IN_RED, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        232, 0, 440, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_IN_GREEN, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        264, 0, 440, FACING_SOUTH, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gCottageRoomsA_ActorPlacementsAfterKalay[] = {
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_WHITE_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        697, 0, 468, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        726, 0, 486, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_TEAL_SHIRT, CONDITION_IN_VIEW, (s32)gCottageRoomsA_FatherPacingScript,
        819, 0, 405, FACING_WEST_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_HEADSCARF, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        841, 0, 370, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        403, 0, 144, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        456, 0, 156, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        494, 0, 112, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        495, 0, 453, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        405, 0, 421, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_WANDER,
        99, 0, 125, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        768, 0, 768, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        392, 0, 697, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_PONYTAIL, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        72, 0, 424, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_MAN_IN_RED, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        232, 0, 440, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_IN_GREEN, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        264, 0, 440, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_LETTER_Z, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_LETTER_Z, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_LETTER_Z, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
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
    { TRIGGER_TABLE_END },
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
    { TRIGGER_TABLE_END },
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
    { TRIGGER_TABLE_END },
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
    { TRIGGER_TABLE_END },
};
// clang-format on
