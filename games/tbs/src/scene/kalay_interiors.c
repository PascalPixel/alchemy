/// @file
/// Inside Kalay: the houses and the inn, and the Kalay Sanctum.
///
/// - Kalay (KalayInteriors1, scene 100), one map holding every house and the
///   inn, each entered from its own door in the town (kalay.c) at an entrance
///   of its own, and left by the matching exit. Entrances 9 to 15 and 17 are
///   the inn's rooms and take the inn's placements and events; every other
///   entrance takes the houses'. The inn is full of tourists on their way to
///   see Colosso in Tolbi. The innkeeper offers a room when spoken to across
///   the counter; from elsewhere he asks `Are you going to Tolbi also?`, and
///   the tour's guide in the dining room says `Please finish eating if you
///   are taking the tour!` Either sets FLAG_KALAY_HEARD_OF_COLOSSO_TOUR, after
///   which leaving the town plays the tour group's departure. Once the tour
///   has left (FLAG_KALAY_COLOSSO_TOUR_LEFT), its guests are gone from the
///   inn, two people are gone from the houses, one of whom was about to join
///   it, and those who stay say so. Searching the ovens shows what the cooks
///   are making; the oven with roasted eggplant first gives a Smoke Bomb (its
///   find comes first in the table, under the same trigger), and barrels hold
///   11 coins and an Elixir. The inn's table and the houses' both carry these
///   searches.
/// - Kalay Sanctum (KalaySanctum, scene 101), whose priest offers the
///   Sanctum's services when spoken to across the counter. Its exit leads
///   back to the town's entrance 8.

#include "game_state.h"
#include "scene_ids.h"
#include "unit/scene/kalay_interiors.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "resource_3a9.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "flags.h"
#include "songs.h"

/// The inn's rooms are entered through entrances 9 to 15 and 17.
enum KalayInteriorsActor {
  ACTOR_INNKEEPER = 8,  ///< in the inn
  ACTOR_PRIEST = 8,     ///< in the sanctum
  ACTOR_TOUR_GUIDE = 12 ///< in the inn's dining room
};

static void KalayInteriors_ApplyRoomState(void);

/// Selects the entrance table of Kalay's houses and inn, or of the Kalay
/// Sanctum.
const struct SceneEntrance *KalayInteriors_GetEntrances(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_KalayInteriors1) {
    return gKalayInteriors_HouseEntrances;
  }
  if (scene == (s32)&SceneId_KalaySanctum) {
    return gKalayInteriors_SanctumEntrances;
  }
  return gKalayInteriors_NoEntrances;
}

const struct SceneRegion *KalayInteriors_GetRegions(void) { return NULL; }

const u32 *KalayInteriors_GetExits(void) { return gKalayInteriors_Exits; }

/// The inn's rooms share one placement table and the houses another, chosen
/// by entrance and clipped to the current view.
const struct ScenePlacement *KalayInteriors_GetPlacements(void) {
  s32 scene = gGameState.scene;

  if (scene == (s32)&SceneId_KalayInteriors1) {
    s32 entrance = gGameState.entrance;
    struct ScenePlacement *table;

    switch (entrance) {
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 17:
      table = gKalayInteriors_InnPlacements;
      break;
    default:
      table = gKalayInteriors_HousePlacements;
      break;
    }
    FieldScene_PrepareActors(table);
    return table;
  }
  if (scene == (s32)&SceneId_KalaySanctum) {
    return gKalayInteriors_SanctumPlacements;
  }
  return gKalayInteriors_NoPlacements;
}

/// Selects room dialogue with the same entrance grouping as the placements.
const struct SceneEvent *KalayInteriors_GetEvents(void) {
  s32 scene = gGameState.scene;

  if (scene == (s32)&SceneId_KalayInteriors1) {
    s32 entrance = gGameState.entrance;

    switch (entrance) {
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 17:
      return gKalayInteriors_InnEvents;
    default:
      return gKalayInteriors_HouseEvents;
    }
  }
  if (scene == (s32)&SceneId_KalaySanctum) {
    return gKalayInteriors_SanctumEvents;
  }
  return gKalayInteriors_NoEvents;
}

/// The tour's guide in the dining room of Kalay's inn: now the party has heard
/// of the tour. `Please finish eating if you are taking the tour! We'll be
/// leaving soon!`
static void KalayInteriors_TalkToTourGuide(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgKalayPleaseFinishEatingIfTaking);
  Engine_EventShowMessage(ACTOR_TOUR_GUIDE, 0);
  Engine_GameFlagSet(FLAG_KALAY_HEARD_OF_COLOSSO_TOUR);
  Engine_EventEnd();
}

/// Someone in one of Kalay's houses asks whether the party knows the continent
/// south of Tolbi, and answers either way.
static void KalayInteriors_AskAboutSouthernContinent(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgKalayDoKnowAboutContinentSouth);
  Engine_EventAskYesNo(16, 0);
  Engine_EventEnd();
}

/// Spoken to across the counter of Kalay's inn, the leader facing west, the
/// innkeeper offers a room. From any other side he asks whether the party is
/// going to Tolbi too, or, once the tour has left, says how empty the inn
/// feels.
static void KalayInteriors_TalkToInnkeeper(void) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);

  if ((u16)(leader->facing - 0x6001) <= 0x3FFE) {
    Engine_InnOpen(7, ACTOR_INNKEEPER);
  } else {
    Engine_EventBegin();

    if (Engine_GameFlagIsSet(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0) {
      Engine_EventSetMessage(MsgKalayOurInnFeelsEmptyNow);
      Engine_EventShowMessage(ACTOR_INNKEEPER, 0);
    } else {
      Engine_EventSetMessage(MsgKalayGoingTolbiAlso);
      Engine_EventAskYesNo(ACTOR_INNKEEPER, 0);
      Engine_GameFlagSet(FLAG_KALAY_HEARD_OF_COLOSSO_TOUR);
    }

    Engine_EventEnd();
  }
}

/// Spoken to across the counter of the Kalay Sanctum, the leader facing north,
/// the priest offers the Sanctum's services; from elsewhere he says Kalay is a
/// peaceful place.
static void KalayInteriors_TalkToPriest(void) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);

  // FACING_IS_NORTH removes the native EN halfword shift and changes its
  // comparison constant. Keep the original unsigned-halfword arithmetic.
  if ((u16)(leader->facing + 0x5fff) <= 0x3ffe) {
    Engine_SanctumOpen(ACTOR_PRIEST);
  } else {
    Engine_EventBegin();
    Engine_EventSetMessage(MsgKalayCanLiveInPeaceIn);
    Engine_EventShowMessage(ACTOR_PRIEST, 0);
    Engine_EventEnd();
  }
}

/// Walking north into one of four doors inside Kalay's houses and inn
/// (triggers 12, 13, 16 and 19): stops the room's actors, opens the door and
/// walks the leader out.
static void KalayInteriors_OpenDoorAndExit(void) {
  struct EventWork *work = gEventWork;
  u32 slot;
  s32 doorIndex;
  struct FieldActor *leader;

  Engine_EventBegin();

  // Prevent idle movement while the doorway cells are animated.
  for (slot = ACTOR_FIRST_PLACED; slot <= 65; slot++) {
    struct FieldActor *actor = Object_GetById(slot);

    if (actor != 0) {
      actor->motionFlags = 0;
    }
  }

  switch (work->touchedTrigger) {
  case 12:
    doorIndex = 0;
    break;
  case 13:
    doorIndex = 1;
    break;
  case 16:
    doorIndex = 2;
    break;
  case 19:
    doorIndex = 3;
    break;
  default:
    return;
  }

  Engine_AudioPlayCue(SE_DOOR_OPEN);

  {
    u32 x = gKalayInteriors_DoorCells[doorIndex].x;
    u32 y = gKalayInteriors_DoorCells[doorIndex].y;

    Engine_MapAnimateCells(gKalayInteriors_DoorCells[doorIndex].steps, x, y);
  }

  Engine_ActorSetSpeed(ACTOR_PARTY_LEADER, 0x00008000, 0x00004000);

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  leader->motionFlags = 0;

  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  Engine_ActorCenterAndWalk(ACTOR_PARTY_LEADER, 3, -8);
  Engine_EventWait(10);

  Engine_EventRequestExit(work->touchedTrigger);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventEnd();
}

/// Sets the opening transition and restores room-specific actors and tiles.
s32 KalayInteriors_Initialize(void) {
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  if (gGameState.scene == (s32)&SceneId_KalayInteriors1) {
    KalayInteriors_ApplyRoomState();
  }
  return 0;
}

/// Once the Colosso tour has left, its guests are gone from Kalay's inn, and
/// two people from the houses. Entrance 3 instead redraws a rectangle of
/// cells.
static void KalayInteriors_ApplyRoomState(void) {
  s16 entrance = gGameState.entrance;

  switch (entrance) {
  case 3: {
    Map_CopyCellsTo(30, 14, 30, 16, 4, 2);
    return;
  }
  case 9:
  case 10:
  case 11:
  case 12:
  case 13:
  case 14:
  case 15:
  case 17:
    if (Engine_GameFlagIsSet(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0) {
      Engine_ActorDestroy(10);
      Engine_ActorDestroy(11);
      Engine_ActorDestroy(12);
      Engine_ActorDestroy(13);
      Engine_ActorDestroy(14);
      Engine_ActorDestroy(17);
      Engine_ActorDestroy(18);
      Engine_ActorDestroy(19);
      Engine_ActorDestroy(15);
    } else {
      Engine_ActorSetChildValue(13, 2);
    }
    return;
  default:
    if (Engine_GameFlagIsSet(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0) {
      Engine_ActorDestroy(16);
      Engine_ActorDestroy(17);
    }
    break;
  }
}

struct SceneEntrance gKalayInteriors_NoEntrances[] = {
    {0, CONDITION_ALWAYS, 168, 0, 168, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gKalayInteriors_HouseEntrances[] = {
    {0, CONDITION_ALWAYS, 168, 0, 168, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 144, 0, 192, FACING_NORTH, 0, 5, 45, 245, 230, 0},
    {2, CONDITION_ALWAYS, 400, 0, 192, FACING_NORTH, 0, 255, 45, 485, 230, 0},
    {3, CONDITION_ALWAYS, 624, 0, 240, FACING_NORTH, 0, 500, 45, 735, 275, 0},
    {4, CONDITION_ALWAYS, 128, 0, 432, FACING_NORTH, 0, 35, 290, 275, 468, 0},
    {5, CONDITION_ALWAYS, 384, 0, 384, FACING_NORTH, 0, 305, 255, 545, 485, 0},
    {6, CONDITION_ALWAYS, 464, 0, 448, FACING_NORTH, 0, 305, 255, 545, 485, 0},
    {7, CONDITION_ALWAYS, 720, 0, 496, FACING_NORTH, 0, 630, 305, 875, 535, 0},
    {8, CONDITION_ALWAYS, 800, 0, 440, FACING_NORTH, 0, 630, 305, 875, 535, 0},
    {9, CONDITION_ALWAYS, 48, 0, 640, FACING_NORTH, 0, 0, 495, 305, 775, 0},
    {10, CONDITION_ALWAYS, 224, 0, 736, FACING_NORTH, 0, 0, 495, 305, 775, 0},
    {11, CONDITION_ALWAYS, 408, 0, 616, FACING_SOUTH, 0, 355, 525, 690, 710, 0},
    {12, CONDITION_ALWAYS, 552, 0, 648, FACING_SOUTH, 0, 355, 525, 690, 710, 0},
    {13, CONDITION_ALWAYS, 632, 0, 648, FACING_SOUTH, 0, 355, 525, 690, 710, 0},
    {14, CONDITION_ALWAYS, 552, 0, 680, FACING_NORTH, 0, 355, 525, 690, 710, 0},
    {15, CONDITION_ALWAYS, 632, 0, 680, FACING_NORTH, 0, 355, 525, 690, 710, 0},
    {16, CONDITION_ALWAYS, 808, 0, 616, FACING_SOUTH, 0, 745, 570, 935, 720, 0},
    {17, CONDITION_ALWAYS, 440, 0, 872, FACING_NORTH, 0, 340, 740, 520, 900, 0},
    {18, CONDITION_ALWAYS, 584, 0, 872, FACING_NORTH, 0, 540, 740, 730, 900, 0},
    {19, CONDITION_ALWAYS, 840, 0, 792, FACING_SOUTH, 0, 710, 740, 940, 900, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gKalayInteriors_SanctumEntrances[] = {
    {0, CONDITION_ALWAYS, 168, 0, 168, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {KALAY_SANCTUM_FROM_KALAY_1, CONDITION_ALWAYS, 248, 0, 304, FACING_NORTH, 0,
     110, 35, 390, 345, 0},
    {2, CONDITION_ALWAYS, 248, 0, 128, FACING_SOUTH, 0, 110, 35, 390, 345, 0},
    {SCENE_TABLE_END},
};

u32 gKalayInteriors_Exits[] = {
    SCENE_EXITS(KalayInteriors1),
    SCENE_EXIT(1, Kalay1, 1),
    SCENE_EXIT(2, Kalay1, 2),
    SCENE_EXIT(3, Kalay1, 3),
    SCENE_EXIT(4, Kalay1, 4),
    SCENE_EXIT(5, Kalay1, 12),
    SCENE_EXIT(6, Kalay1, 5),
    SCENE_EXIT(7, Kalay1, 6),
    SCENE_EXIT(8, Kalay1, 13),
    SCENE_EXIT(9, KalayInteriors1, 11),
    SCENE_EXIT(10, Kalay1, 7),
    SCENE_EXIT(11, KalayInteriors1, 9),
    SCENE_EXIT(12, KalayInteriors1, 17),
    SCENE_EXIT(13, KalayInteriors1, 18),
    SCENE_EXIT(14, KalayInteriors1, 19),
    SCENE_EXIT(15, KalayInteriors1, 16),
    SCENE_EXIT(16, KalayInteriors1, 15),
    SCENE_EXIT(17, KalayInteriors1, 12),
    SCENE_EXIT(18, KalayInteriors1, 13),
    SCENE_EXIT(19, KalayInteriors1, 14),
    SCENE_EXITS(KalaySanctum),
    SCENE_EXIT(KALAY_SANCTUM_TO_KALAY_1, Kalay1, KALAY_1_FROM_KALAY_SANCTUM),
    SCENE_EXITS_END,
};

struct ScenePlacement gKalayInteriors_NoPlacements[] = {
    {SCENE_TABLE_END},
};

// clang-format off
struct ScenePlacement gKalayInteriors_HousePlacements[] = {
    { 0x6c, 0x0, ACTOR_STAND,
        PIXELS(101), 0, PIXELS(181), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x6a, 0x0, ACTOR_STAND,
        PIXELS(167), 0, PIXELS(109), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x65, 0x0, ACTOR_STAND,
        PIXELS(383), 0, PIXELS(108), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x73, 0x0, ACTOR_STAND,
        PIXELS(346), 0, PIXELS(109), FACING_NORTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x7e, 0x0, ACTOR_WANDER,
        PIXELS(573), 0, PIXELS(163), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x67, 0x0, ACTOR_STAND,
        PIXELS(670), 0, PIXELS(154), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x7f, 0x0, ACTOR_STAND,
        PIXELS(183), 0, PIXELS(416), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x6f, 0x0, ACTOR_STAND,
        PIXELS(120), 0, PIXELS(369), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x6c, 0x0, ACTOR_STAND,
        PIXELS(856), 0, PIXELS(628), FACING_SOUTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x6a, 0x0, ACTOR_STAND,
        PIXELS(780), 0, PIXELS(633), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x8f, 0x0, ACTOR_STAND,
        PIXELS(795), 0, PIXELS(806), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x8f, 0x0, ACTOR_STAND,
        PIXELS(857), 0, PIXELS(841), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gKalayInteriors_InnPlacements[] = {
    { 0x77, 0x0, ACTOR_STAND,
        PIXELS(120), 0, PIXELS(627), FACING_EAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x80, 0x0, ACTOR_WANDER,
        PIXELS(505), 0, PIXELS(665), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x84, 0x0, ACTOR_STAND,
        PIXELS(66), 0, PIXELS(622), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x94, 0x0, ACTOR_STAND,
        PIXELS(196), 0, PIXELS(624), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0xab, 0x0, ACTOR_STAND,
        PIXELS(232), 0, PIXELS(586), FACING_SOUTH + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0xab, 0x0, ACTOR_STAND,
        PIXELS(168), 0, PIXELS(570), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x72, 0x0, ACTOR_STAND,
        PIXELS(207), 0, PIXELS(672), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x67, 0x0, ACTOR_STAND,
        PIXELS(168), 0, PIXELS(643), FACING_SOUTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x6f, 0x0, ACTOR_STAND,
        PIXELS(247), 0, PIXELS(736), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x9d, 0x0, ACTOR_STAND,
        PIXELS(263), 0, PIXELS(627), FACING_SOUTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x43, 0x0, ACTOR_STAND,
        PIXELS(417), 0, PIXELS(810), FACING_EAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x42, 0x0, ACTOR_STAND,
        PIXELS(459), 0, PIXELS(820), FACING_WEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gKalayInteriors_SanctumPlacements[] = {
    { 0x79, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(248), 0, PIXELS(106), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gKalayInteriors_NoEvents[] = {
    { SCENE_EVENTS_END },
};

/// A bare flag as a condition holds while that flag is clear: these lines are
/// spoken before the Colosso tour leaves, the next ones after.
struct SceneEvent gKalayInteriors_HouseEvents[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
    { EVENT_EXIT, 6, CONDITION_ALWAYS, 6 },
    { EVENT_EXIT, 7, CONDITION_ALWAYS, 7 },
    { EVENT_EXIT, 8, CONDITION_ALWAYS, 8 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 16, CONDITION_ALWAYS, EVENT_SCRIPT(KalayInteriors_OpenDoorAndExit) },
    { EVENT_EXIT, 18, CONDITION_ALWAYS, 18 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 19, CONDITION_ALWAYS, EVENT_SCRIPT(KalayInteriors_OpenDoorAndExit) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsImShockedToHearTheRighteous) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsLunpaWasAKindThiefHis) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsSomehowTheNorthHasBecomeThe) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsAMerchantFromVaultHadBeen) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsNoOneIsAllowedToGo) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsIDoNotWantMySon) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsTheMostImportantSkillForA) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsMySonReceivedHisKnackFor) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, EVENT_SCRIPT(KalayInteriors_AskAboutSouthernContinent) },
    { EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsIBetterGetReadyTheColosso) },
    { EVENT_TALK, 18, FLAG_KALAY_COLOSSO_TOUR_LEFT, MSGNO(MsgKalayInteriorsWeFledFromGondowanToThe) },
    { EVENT_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsWeCameHereToEscapeThe) },
    { EVENT_TALK, 19, FLAG_KALAY_COLOSSO_TOUR_LEFT, MSGNO(MsgKalayInteriorsLaliveroWhereImFromIsThe) },
    { EVENT_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsWeEscapedFromTheNortheasternRegion) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsTheRegionNorthOfVaultWas) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsLunpasReputationIsVeryBadNowadays) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsThreeYearsAgoThievesRaidedMt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsTheySayOtherNorthernPartsAre) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsImAnAdultNowIDont) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsIDontCareHowMySon) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsTheMostImportantThingIsNot) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsMySonIsVeryTightWith) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsPeopleFromTheContinentSouthOf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsImPrettyExcitedBecauseThisIs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, FLAG_KALAY_COLOSSO_TOUR_LEFT, MSGNO(MsgKalayInteriorsIFledAllThisWayNow) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsIWonderWhatColossoIsLike) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, FLAG_KALAY_COLOSSO_TOUR_LEFT, MSGNO(MsgKalayInteriorsItLooksLikeColossoWillSoon) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsIWasSoScaredComingTo) },
    { SEARCH(SEARCH_BARREL), 100, 0xf83, FIND_COINS(11) },
    { SEARCH(SEARCH_OVEN), 101, 0xf84, FIND_ITEM(ITEM_SMOKE_BOMB) },
    { SEARCH(SEARCH_BARREL), 102, 0xf85, FIND_ITEM(ITEM_ELIXIR) },
    { SEARCH(SEARCH_OVEN), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKalayInteriorsShesToastingSomeBreadItSmells)) },
    { SEARCH(SEARCH_OVEN), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKalayInteriorsHesMakingCurryItsSpicyScent)) },
    { SEARCH(SEARCH_OVEN), 101, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKalayInteriorsHesMakingRoastedEggplantWithCheese)) },
    { SEARCH(SEARCH_OVEN), 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKalayInteriorsItsAKalaySaladWithCucumber)) },
    { SEARCH(SEARCH_OVEN), 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKalayInteriorsItsPicklesWithGarlicThePickle)) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gKalayInteriors_InnEvents[] = {
    { EVENT_EXIT, 9, CONDITION_ALWAYS, 9 },
    { EVENT_EXIT, 10, CONDITION_ALWAYS, 10 },
    { EVENT_EXIT, 11, CONDITION_ALWAYS, 11 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 12, CONDITION_ALWAYS, EVENT_SCRIPT(KalayInteriors_OpenDoorAndExit) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 13, CONDITION_ALWAYS, EVENT_SCRIPT(KalayInteriors_OpenDoorAndExit) },
    { EVENT_EXIT, 14, CONDITION_ALWAYS, 14 },
    { EVENT_EXIT, 15, CONDITION_ALWAYS, 15 },
    { EVENT_EXIT, 17, CONDITION_ALWAYS, 17 },
    { EVENT_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(KalayInteriors_TalkToInnkeeper) },
    { EVENT_TALK, 9, FLAG_KALAY_COLOSSO_TOUR_LEFT, MSGNO(MsgKalayInteriorsEveryoneIsGoingToTolbiTo) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsEvenOurChefLeftSoWe) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsIveSignedOnForATour) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsTheColossoGladiatorsAreAllStrong) },
    { EVENT_TALK, ACTOR_TOUR_GUIDE, CONDITION_ALWAYS, EVENT_SCRIPT(KalayInteriors_TalkToTourGuide) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsEveryoneLooksForwardToGoingTo) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsSurelyLordHammetWillPopUp) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsIWouldHaveEnteredColossoIf) },
    { EVENT_TALK, 16, FLAG_KALAY_COLOSSO_TOUR_LEFT, MSGNO(MsgKalayInteriorsManyOfTheGuestsHereSeem) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsUhOhHesStillHereEven) },
    { EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsArentYouGoingToColossoYou) },
    { EVENT_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsIfYouAreEnteringColossoYou) },
    { EVENT_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsWereSoStrongMakingTheFinals) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, FLAG_KALAY_COLOSSO_TOUR_LEFT, MSGNO(MsgKalayInteriorsHesAShrewdMerchantIfHe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsIdHaveGoneOnTheColosso) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, FLAG_KALAY_COLOSSO_TOUR_LEFT, MSGNO(MsgKalayInteriorsEveryYearEveryoneGoesToTolbi) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsEvenIfMyHusbandWantedTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsJoiningATourGroupIsThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsIWonderWhichWarriorWillWin) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsItsBusinessSoYouCantHelp) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsImExhaustedFromPreparingThisTour) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsIWasSoExcitedICouldnt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsMyAnimalInstinctsStartWakingUp) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, FLAG_KALAY_COLOSSO_TOUR_LEFT, MSGNO(MsgKalayInteriorsColossoIsSoCommonWhatsSo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsICantSeemToRelaxNow) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsImGoingToSignUpFor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsIHavePowerAndSeanHas) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsWeAreTheToughestOfAll) },
    { SEARCH(SEARCH_BARREL), 100, 0xf83, FIND_COINS(11) },
    { SEARCH(SEARCH_OVEN), 101, 0xf84, FIND_ITEM(ITEM_SMOKE_BOMB) },
    { SEARCH(SEARCH_BARREL), 102, 0xf85, FIND_ITEM(ITEM_ELIXIR) },
    { SEARCH(SEARCH_OVEN), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKalayInteriorsShesToastingSomeBreadItSmells)) },
    { SEARCH(SEARCH_OVEN), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKalayInteriorsHesMakingCurryItsSpicyScent)) },
    { SEARCH(SEARCH_OVEN), 101, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKalayInteriorsHesMakingRoastedEggplantWithCheese)) },
    { SEARCH(SEARCH_OVEN), 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKalayInteriorsItsAKalaySaladWithCucumber)) },
    { SEARCH(SEARCH_OVEN), 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKalayInteriorsItsPicklesWithGarlicThePickle)) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gKalayInteriors_SanctumEvents[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_TALK, ACTOR_PRIEST, CONDITION_ALWAYS, EVENT_SCRIPT(KalayInteriors_TalkToPriest) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsICameHereToSpreadGood) },
    { SCENE_EVENTS_END },
};
// clang-format on

u16 gKalayInteriors_DoorCellSteps[] = {
    MAP_CELL_STEP(7, 125, 1, 2, 5),
    MAP_CELL_STEP(8, 125, 1, 2, 5),
    MAP_CELL_STEPS_END,
};

struct MapCellAnim gKalayInteriors_DoorCells[] = {
    {gKalayInteriors_DoorCellSteps, 34, 98},
    {gKalayInteriors_DoorCellSteps, 39, 98},
    {gKalayInteriors_DoorCellSteps, 50, 96},
    {gKalayInteriors_DoorCellSteps, 52, 107},
};
