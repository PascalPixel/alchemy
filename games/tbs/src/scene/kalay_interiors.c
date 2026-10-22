/// @file
/// Inside Kalay: the houses and the inn, and the Kalay Sanctum.
///
/// - Kalay (SceneId_KalayInteriors), one map holding every house and the
///   inn, each entered from its own door in the town (kalay.c) at an entrance
///   of its own, and left by the matching exit. The inn's entrances,
///   KALAY_INTERIORS_INN_FROM_INN_PASSAGE to
///   KALAY_INTERIORS_INN_PASSAGE_FROM_GUEST_ROOM_SOUTHEAST and
///   KALAY_INTERIORS_GUEST_ROOM_NORTHWEST_FROM_INN_PASSAGE, take the inn's
///   placements and events; every other
///   entrance takes the houses'. The inn is full of tourists on their way to
///   see Colosso in Tolbi. The innkeeper offers a room when spoken to across
///   the counter; from elsewhere he asks `Are you going to Tolbi also?`, and
///   the tour's guide in the dining room says `Please finish eating if you
///   are taking the tour!` Either sets FLAG_KALAY_HEARD_OF_COLOSSO_TOUR, after
///   which leaving the town plays the tour group's departure. Once the tour
///   has left (FLAG_KALAY_COLOSSO_TOUR_LEFT), its guests are gone from the
///   inn, two people are gone from the houses, one of whom was about to join
///   it, and those who stay say so. Searching the ovens shows what the cooks
///   are making, except that the oven with roasted eggplant gives a Smoke
///   Bomb: its find comes first in the table under the same trigger, and a
///   named search ignores its condition, so it answers every time and the
///   eggplant line is unused. Barrels hold
///   11 coins and an Elixir. The inn's table and the houses' both carry these
///   searches.
/// - Kalay Sanctum (SceneId_KalaySanctum), whose healer offers the
///   Sanctum's services when spoken to across the counter. Its exit leads
///   back into the town (KALAY_FROM_KALAY_SANCTUM).
///
/// Open questions: none.

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

/// Each table numbers its own rows from ACTOR_FIRST_PLACED: the houses'
/// (gKalayInteriors_HousePlacements), the inn's, whose rooms are entered
/// through the inn's entrances (gKalayInteriors_InnPlacements), and the
/// Kalay Sanctum's (gKalayInteriors_SanctumPlacements). Unless a note says
/// otherwise, it quotes what its actor says before the Colosso tour leaves.
enum KalayInteriorsActor {
  // Kalay's houses
  /// A young man with brown hair (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR).
  /// ★ Named from the game: "I'm shocked to hear the righteous thieves of
  /// Lunpa have gone bad." (MsgKalayInteriorsImShockedToHearTheRighteous)
  ACTOR_YOUNG_MAN_ON_LUNPA_GONE_BAD = ACTOR_FIRST_PLACED,
  /// A woman with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR).
  /// ★ Named from the game: "Lunpa was a kind thief."
  /// (MsgKalayInteriorsLunpaWasAKindThiefHis)
  ACTOR_WOMAN_SAYING_LUNPA_WAS_KIND,
  /// A burly grey-haired man (SPRITE_BURLY_MAN_WITH_GREY_HAIR).
  /// ★ Named from the game: "Somehow, the north has become the center of
  /// much attention lately." (MsgKalayInteriorsSomehowTheNorthHasBecomeThe)
  ACTOR_MAN_ON_THE_NORTH,
  /// A woman in a green dress (SPRITE_WOMAN_IN_GREEN_DRESS), who says the
  /// soldiers took the merchant to Lord Hammet's Palace.
  /// ★ Named from the game: "A merchant from Vault had been saying many
  /// things about the north..." (MsgKalayInteriorsAMerchantFromVaultHadBeen)
  ACTOR_WOMAN_ON_THE_VAULT_MERCHANT,
  /// The worried father's son, drawn with the bald man in red's frames
  /// (sprite 126, spr_124.png's picture), who may not go to Vault.
  /// ★ Named from the game: "I'm an adult, now. I don't want my father
  /// ordering me around..." (MsgKalayInteriorsImAnAdultNowIDont)
  ACTOR_WORRIED_FATHERS_SON,
  /// An old man with a cane (SPRITE_OLD_MAN_WITH_CANE).
  /// ★ Named from the game: "I do not want my son to face any danger."
  /// (MsgKalayInteriorsIDoNotWantMySon)
  ACTOR_WORRIED_FATHER,
  /// Drawn with the young man in green's frames (sprite 127, spr_125.png's
  /// picture), whose mother's thoughts call him a merchant.
  /// ★ Named from the game: "I feel like I am living with a merchant...
  /// Actually, he is one!" (MsgKalayInteriorsMySonIsVeryTightWith)
  ACTOR_MERCHANT,
  /// The merchant's mother, an old woman (SPRITE_OLD_WOMAN).
  /// ★ Named from the game: "My son received his knack for business from Lord
  /// Hammet." (MsgKalayInteriorsMySonReceivedHisKnackFor)
  ACTOR_MERCHANTS_MOTHER,
  /// A young man with brown hair (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR), gone once
  /// the tour has left.
  /// ★ Named from the game: "Do you know about the continent south of
  /// Tolbi?" (MsgKalayDoKnowAboutContinentSouth)
  ACTOR_YOUNG_MAN_ASKING_ABOUT_THE_SOUTH,
  /// A woman with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR), about to join the
  /// tour, and gone with it.
  /// ★ Named from the game: "I better get ready! The Colosso Tour is about
  /// to leave!" (MsgKalayInteriorsIBetterGetReadyTheColosso)
  ACTOR_WOMAN_BOUND_FOR_THE_TOUR,
  /// Two men in white turbans (SPRITE_MAN_IN_WHITE_TURBAN) who fled Lalivero,
  /// in Gondowan.
  /// ★ Named from the game: "We lived in Lalivero"
  /// (MsgKalayInteriorsWeFledFromGondowanToThe)
  ACTOR_MAN_FROM_LALIVERO,
  /// ★ Named from the game: "Lalivero, where I'm from, is the largest village
  /// in Gondowan." (MsgKalayInteriorsLaliveroWhereImFromIsThe)
  ACTOR_SECOND_MAN_FROM_LALIVERO,
  // Kalay's inn: all but the innkeeper, his wife and ACTOR_INN_GUEST are gone
  // once the tour has left.
  /// A man with dark hair (SPRITE_MAN_WITH_DARK_HAIR).
  /// ★ Named from the game: "Our inn feels so empty"
  /// (MsgKalayOurInnFeelsEmptyNow)
  ACTOR_INNKEEPER = ACTOR_FIRST_PLACED,
  /// The innkeeper's wife, a woman in a pink apron
  /// (SPRITE_WOMAN_IN_PINK_APRON), who says once the tour has left that even
  /// their chef went with it.
  /// ★ Named from the game: "Even if my husband wanted to go on the Colosso
  /// tour" (MsgKalayInteriorsEvenIfMyHusbandWantedTo)
  ACTOR_INNKEEPERS_WIFE,
  /// A chef (SPRITE_CHEF).
  /// ★ Named from the game: "I've signed on for a tour group going to
  /// Tolbi." (MsgKalayInteriorsIveSignedOnForATour)
  ACTOR_FIRST_TOURIST,
  /// A young woman in green (SPRITE_YOUNG_WOMAN_IN_GREEN), who talks of the
  /// Colosso gladiators. Like the other guests the tour takes, she is named
  /// by what Kalay's people say of the inn's guests.
  /// ★ Named from the game: "The guests at the inn are tourists on their way
  /// to see Colosso." (MsgKalayInnFestivalTourists)
  ACTOR_SECOND_TOURIST,
  /// The tour's guides, men carrying packs (SPRITE_MAN_CARRYING_A_PACK), in
  /// the inn's dining room.
  /// ★ Named from the game: "Please finish eating if you are taking the tour!"
  /// (MsgKalayPleaseFinishEatingIfTaking)
  ACTOR_TOUR_GUIDE,
  /// ★ Named from the game: "I'm exhausted from preparing this tour."
  /// (MsgKalayInteriorsImExhaustedFromPreparingThisTour)
  ACTOR_SECOND_TOUR_GUIDE,
  /// A bald, muscular man (SPRITE_BALD_MUSCULAR_MAN), who expects Lord Hammet
  /// in Tolbi.
  /// ★ Named from the game: "The guests at the inn are tourists on their way
  /// to see Colosso." (MsgKalayInnFestivalTourists)
  ACTOR_THIRD_TOURIST,
  /// An old man with a cane (SPRITE_OLD_MAN_WITH_CANE), who would have
  /// entered Colosso if he were younger.
  /// ★ Named from the game: "The guests at the inn are tourists on their way
  /// to see Colosso." (MsgKalayInnFestivalTourists)
  ACTOR_FOURTH_TOURIST,
  /// An old woman (SPRITE_OLD_WOMAN), who stays once the tour has left.
  /// ★ Named from the game: "Many of the guests here seem to be heading to
  /// Tolbi." (MsgKalayInteriorsManyOfTheGuestsHereSeem)
  ACTOR_INN_GUEST,
  /// A woman with a sword (SPRITE_PERSON_WITH_SWORD).
  /// ★ Named from the game: "I'm going to sign up for Colosso as soon as I
  /// arrive in Tolbi." (MsgKalayInteriorsImGoingToSignUpFor)
  ACTOR_WARRIOR_SIGNING_UP_FOR_COLOSSO,
  /// The warriors bound for Colosso, Ouranos and Sean, young men with red and
  /// with blue hair (SPRITE_YOUNG_MAN_WITH_RED_HAIR and
  /// SPRITE_YOUNG_MAN_WITH_BLUE_HAIR, as in tolbi_ship_interiors.c), named by
  /// what each says of the other.
  /// ★ Named from the game: "Then, Ouranos and I will fight each other"
  /// (MsgKalayInteriorsWereSoStrongMakingTheFinals)
  ACTOR_OURANOS,
  /// ★ Named from the game: "I have power, and Sean has speed."
  /// (MsgKalayInteriorsIHavePowerAndSeanHas)
  ACTOR_SEAN,
  // The Kalay Sanctum
  /// A man in a tall hat (SPRITE_MAN_IN_TALL_HAT).
  /// ★ Named from the game: "Then I call upon my healing powers."
  /// (MsgReviveDonation)
  ACTOR_HEALER = ACTOR_FIRST_PLACED
};

/// Kalay's inn, as Engine_InnOpen numbers the inns.
enum { KALAY_INN = 7 };

static void KalayInteriors_ApplyRoomState(void);

/// Selects the entrance table of Kalay's houses and inn, or of the Kalay
/// Sanctum.
const struct SceneEntrance *KalayInteriors_GetEntrances(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_KalayInteriors) {
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

  if (scene == (s32)&SceneId_KalayInteriors) {
    s32 entrance = gGameState.entrance;
    struct ScenePlacement *table;

    switch (entrance) {
    case KALAY_INTERIORS_INN_FROM_INN_PASSAGE:
    case KALAY_INTERIORS_INN:
    case KALAY_INTERIORS_INN_PASSAGE_FROM_INN:
    case KALAY_INTERIORS_INN_PASSAGE_FROM_GUEST_ROOM_NORTHWEST:
    case KALAY_INTERIORS_INN_PASSAGE_FROM_GUEST_ROOM_NORTHEAST:
    case KALAY_INTERIORS_INN_PASSAGE_FROM_GUEST_ROOM_SOUTHWEST:
    case KALAY_INTERIORS_INN_PASSAGE_FROM_GUEST_ROOM_SOUTHEAST:
    case KALAY_INTERIORS_GUEST_ROOM_NORTHWEST_FROM_INN_PASSAGE:
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

  if (scene == (s32)&SceneId_KalayInteriors) {
    s32 entrance = gGameState.entrance;

    switch (entrance) {
    case KALAY_INTERIORS_INN_FROM_INN_PASSAGE:
    case KALAY_INTERIORS_INN:
    case KALAY_INTERIORS_INN_PASSAGE_FROM_INN:
    case KALAY_INTERIORS_INN_PASSAGE_FROM_GUEST_ROOM_NORTHWEST:
    case KALAY_INTERIORS_INN_PASSAGE_FROM_GUEST_ROOM_NORTHEAST:
    case KALAY_INTERIORS_INN_PASSAGE_FROM_GUEST_ROOM_SOUTHWEST:
    case KALAY_INTERIORS_INN_PASSAGE_FROM_GUEST_ROOM_SOUTHEAST:
    case KALAY_INTERIORS_GUEST_ROOM_NORTHWEST_FROM_INN_PASSAGE:
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

/// Someone asks whether the party knows the continent south of Tolbi. They're
/// in one of Kalay's houses, and answer either way the party replies.
static void KalayInteriors_AskAboutSouthernContinent(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgKalayDoKnowAboutContinentSouth);
  Engine_EventAskYesNo(ACTOR_YOUNG_MAN_ASKING_ABOUT_THE_SOUTH, 0);
  Engine_EventEnd();
}

/// Spoken to across the counter of Kalay's inn, the leader facing west, the
/// innkeeper offers a room. From any other side he asks whether the party is
/// going to Tolbi too, or, once the tour has left, says how empty the inn
/// feels.
static void KalayInteriors_TalkToInnkeeper(void) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);

  if ((u16)(leader->facing - 0x6001) <= 0x3FFE) {
    Engine_InnOpen(KALAY_INN, ACTOR_INNKEEPER);
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

/// The Kalay Sanctum's healer offers its services across the counter. He does
/// so when spoken to across the counter with the leader facing north; from
/// elsewhere he says Kalay is a peaceful place.
static void KalayInteriors_TalkToHealer(void) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);

  // FACING_IS_NORTH removes the native EN halfword shift and changes its
  // comparison constant. Keep the original unsigned-halfword arithmetic.
  if ((u16)(leader->facing + 0x5fff) <= 0x3ffe) {
    Engine_SanctumOpen(ACTOR_HEALER);
  } else {
    Engine_EventBegin();
    Engine_EventSetMessage(MsgKalayCanLiveInPeaceIn);
    Engine_EventShowMessage(ACTOR_HEALER, 0);
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
  for (slot = ACTOR_FIRST_PLACED; slot <= ACTOR_LAST_SLOT; slot++) {
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

  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIMATION_WALK);
  Engine_ActorCenterAndWalk(ACTOR_PARTY_LEADER, 3, -8);
  Engine_EventWait(10);

  Engine_EventRequestExit(work->touchedTrigger);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventEnd();
}

/// Sets the opening transition and restores room-specific actors and tiles.
s32 KalayInteriors_Initialize(void) {
  gEventWork->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  if (gGameState.scene == (s32)&SceneId_KalayInteriors) {
    KalayInteriors_ApplyRoomState();
  }
  return 0;
}

/// Once the Colosso tour has left, its guests are gone from Kalay's inn, and
/// two people from the houses; until then the second tour guide is drawn with
/// his green and blue swapped, as in kalay.c. Entering by
/// KALAY_INTERIORS_HOUSE_NORTHEAST_LOWER instead blanks four by two cells
/// below the ground floor, where the corner of a neighbouring room would
/// otherwise show at the bottom edge of the view (the room's render with and
/// without the copy).
static void KalayInteriors_ApplyRoomState(void) {
  s16 entrance = gGameState.entrance;

  switch (entrance) {
  case KALAY_INTERIORS_HOUSE_NORTHEAST_LOWER: {
    Map_CopyCellsTo(30, 14, 30, 16, 4, 2);
    return;
  }
  case KALAY_INTERIORS_INN_FROM_INN_PASSAGE:
  case KALAY_INTERIORS_INN:
  case KALAY_INTERIORS_INN_PASSAGE_FROM_INN:
  case KALAY_INTERIORS_INN_PASSAGE_FROM_GUEST_ROOM_NORTHWEST:
  case KALAY_INTERIORS_INN_PASSAGE_FROM_GUEST_ROOM_NORTHEAST:
  case KALAY_INTERIORS_INN_PASSAGE_FROM_GUEST_ROOM_SOUTHWEST:
  case KALAY_INTERIORS_INN_PASSAGE_FROM_GUEST_ROOM_SOUTHEAST:
  case KALAY_INTERIORS_GUEST_ROOM_NORTHWEST_FROM_INN_PASSAGE:
    if (Engine_GameFlagIsSet(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0) {
      Engine_ActorDestroy(ACTOR_FIRST_TOURIST);
      Engine_ActorDestroy(ACTOR_SECOND_TOURIST);
      Engine_ActorDestroy(ACTOR_TOUR_GUIDE);
      Engine_ActorDestroy(ACTOR_SECOND_TOUR_GUIDE);
      Engine_ActorDestroy(ACTOR_THIRD_TOURIST);
      Engine_ActorDestroy(ACTOR_WARRIOR_SIGNING_UP_FOR_COLOSSO);
      Engine_ActorDestroy(ACTOR_OURANOS);
      Engine_ActorDestroy(ACTOR_SEAN);
      Engine_ActorDestroy(ACTOR_FOURTH_TOURIST);
    } else {
      Engine_ActorSetChildValue(ACTOR_SECOND_TOUR_GUIDE, SPRITE_COLOR_RBG);
    }
    return;
  default:
    if (Engine_GameFlagIsSet(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0) {
      Engine_ActorDestroy(ACTOR_YOUNG_MAN_ASKING_ABOUT_THE_SOUTH);
      Engine_ActorDestroy(ACTOR_WOMAN_BOUND_FOR_THE_TOUR);
    }
    break;
  }
}

struct SceneEntrance gKalayInteriors_NoEntrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 168, 0, 168, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gKalayInteriors_HouseEntrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 168, 0, 168, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {KALAY_INTERIORS_HOUSE_NORTHWEST, CONDITION_ALWAYS, 144, 0, 192,
     FACING_NORTH, 0, 5, 45, 245, 230, 0},
    {KALAY_INTERIORS_HOUSE_NORTHEAST_UPPER, CONDITION_ALWAYS, 400, 0, 192,
     FACING_NORTH, 0, 255, 45, 485, 230, 0},
    {KALAY_INTERIORS_HOUSE_NORTHEAST_LOWER, CONDITION_ALWAYS, 624, 0, 240,
     FACING_NORTH, 0, 500, 45, 735, 275, 0},
    {KALAY_INTERIORS_HOUSE_SOUTHEAST, CONDITION_ALWAYS, 128, 0, 432,
     FACING_NORTH, 0, 35, 290, 275, 468, 0},
    {KALAY_INTERIORS_WEAPON_SHOP_COUNTER, CONDITION_ALWAYS, 384, 0, 384,
     FACING_NORTH, 0, 305, 255, 545, 485, 0},
    {KALAY_INTERIORS_WEAPON_SHOP, CONDITION_ALWAYS, 464, 0, 448, FACING_NORTH,
     0, 305, 255, 545, 485, 0},
    {KALAY_INTERIORS_ARMOR_SHOP, CONDITION_ALWAYS, 720, 0, 496, FACING_NORTH, 0,
     630, 305, 875, 535, 0},
    {KALAY_INTERIORS_ARMOR_SHOP_COUNTER, CONDITION_ALWAYS, 800, 0, 440,
     FACING_NORTH, 0, 630, 305, 875, 535, 0},
    {KALAY_INTERIORS_INN_FROM_INN_PASSAGE, CONDITION_ALWAYS, 48, 0, 640,
     FACING_NORTH, 0, 0, 495, 305, 775, 0},
    {KALAY_INTERIORS_INN, CONDITION_ALWAYS, 224, 0, 736, FACING_NORTH, 0, 0,
     495, 305, 775, 0},
    {KALAY_INTERIORS_INN_PASSAGE_FROM_INN, CONDITION_ALWAYS, 408, 0, 616,
     FACING_SOUTH, 0, 355, 525, 690, 710, 0},
    {KALAY_INTERIORS_INN_PASSAGE_FROM_GUEST_ROOM_NORTHWEST, CONDITION_ALWAYS,
     552, 0, 648, FACING_SOUTH, 0, 355, 525, 690, 710, 0},
    {KALAY_INTERIORS_INN_PASSAGE_FROM_GUEST_ROOM_NORTHEAST, CONDITION_ALWAYS,
     632, 0, 648, FACING_SOUTH, 0, 355, 525, 690, 710, 0},
    {KALAY_INTERIORS_INN_PASSAGE_FROM_GUEST_ROOM_SOUTHWEST, CONDITION_ALWAYS,
     552, 0, 680, FACING_NORTH, 0, 355, 525, 690, 710, 0},
    {KALAY_INTERIORS_INN_PASSAGE_FROM_GUEST_ROOM_SOUTHEAST, CONDITION_ALWAYS,
     632, 0, 680, FACING_NORTH, 0, 355, 525, 690, 710, 0},
    {KALAY_INTERIORS_GUEST_ROOM_SOUTHEAST_FROM_INN_PASSAGE, CONDITION_ALWAYS,
     808, 0, 616, FACING_SOUTH, 0, 745, 570, 935, 720, 0},
    {KALAY_INTERIORS_GUEST_ROOM_NORTHWEST_FROM_INN_PASSAGE, CONDITION_ALWAYS,
     440, 0, 872, FACING_NORTH, 0, 340, 740, 520, 900, 0},
    {KALAY_INTERIORS_GUEST_ROOM_NORTHEAST_FROM_INN_PASSAGE, CONDITION_ALWAYS,
     584, 0, 872, FACING_NORTH, 0, 540, 740, 730, 900, 0},
    {KALAY_INTERIORS_GUEST_ROOM_SOUTHWEST_FROM_INN_PASSAGE, CONDITION_ALWAYS,
     840, 0, 792, FACING_SOUTH, 0, 710, 740, 940, 900, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gKalayInteriors_SanctumEntrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 168, 0, 168, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {KALAY_SANCTUM_FROM_KALAY, CONDITION_ALWAYS, 248, 0, 304, FACING_NORTH, 0,
     110, 35, 390, 345, 0},
    {2, CONDITION_ALWAYS, 248, 0, 128, FACING_SOUTH, 0, 110, 35, 390, 345, 0},
    {SCENE_TABLE_END},
};

u32 gKalayInteriors_Exits[] = {
    SCENE_EXITS(KalayInteriors),
    SCENE_EXIT(KALAY_INTERIORS_HOUSE_NORTHWEST_TO_KALAY, Kalay,
               KALAY_FROM_HOUSE_NORTHWEST),
    SCENE_EXIT(KALAY_INTERIORS_HOUSE_NORTHEAST_UPPER_TO_KALAY, Kalay,
               KALAY_FROM_HOUSE_NORTHEAST_UPPER),
    SCENE_EXIT(KALAY_INTERIORS_HOUSE_NORTHEAST_LOWER_TO_KALAY, Kalay,
               KALAY_FROM_HOUSE_NORTHEAST_LOWER),
    SCENE_EXIT(KALAY_INTERIORS_HOUSE_SOUTHEAST_TO_KALAY, Kalay,
               KALAY_FROM_HOUSE_SOUTHEAST),
    SCENE_EXIT(KALAY_INTERIORS_WEAPON_SHOP_COUNTER_TO_KALAY, Kalay,
               KALAY_FROM_WEAPON_SHOP_COUNTER),
    SCENE_EXIT(KALAY_INTERIORS_WEAPON_SHOP_TO_KALAY, Kalay,
               KALAY_FROM_WEAPON_SHOP),
    SCENE_EXIT(KALAY_INTERIORS_ARMOR_SHOP_TO_KALAY, Kalay,
               KALAY_FROM_ARMOR_SHOP),
    SCENE_EXIT(KALAY_INTERIORS_ARMOR_SHOP_COUNTER_TO_KALAY, Kalay,
               KALAY_FROM_ARMOR_SHOP_COUNTER),
    SCENE_EXIT(KALAY_INTERIORS_INN_TO_INN_PASSAGE, KalayInteriors,
               KALAY_INTERIORS_INN_PASSAGE_FROM_INN),
    SCENE_EXIT(KALAY_INTERIORS_INN_TO_KALAY, Kalay, KALAY_FROM_INN),
    SCENE_EXIT(KALAY_INTERIORS_INN_PASSAGE_TO_INN, KalayInteriors,
               KALAY_INTERIORS_INN_FROM_INN_PASSAGE),
    SCENE_EXIT(KALAY_INTERIORS_INN_PASSAGE_TO_GUEST_ROOM_NORTHWEST,
               KalayInteriors,
               KALAY_INTERIORS_GUEST_ROOM_NORTHWEST_FROM_INN_PASSAGE),
    SCENE_EXIT(KALAY_INTERIORS_INN_PASSAGE_TO_GUEST_ROOM_NORTHEAST,
               KalayInteriors,
               KALAY_INTERIORS_GUEST_ROOM_NORTHEAST_FROM_INN_PASSAGE),
    SCENE_EXIT(KALAY_INTERIORS_INN_PASSAGE_TO_GUEST_ROOM_SOUTHWEST,
               KalayInteriors,
               KALAY_INTERIORS_GUEST_ROOM_SOUTHWEST_FROM_INN_PASSAGE),
    SCENE_EXIT(KALAY_INTERIORS_INN_PASSAGE_TO_GUEST_ROOM_SOUTHEAST,
               KalayInteriors,
               KALAY_INTERIORS_GUEST_ROOM_SOUTHEAST_FROM_INN_PASSAGE),
    SCENE_EXIT(KALAY_INTERIORS_GUEST_ROOM_SOUTHEAST_TO_INN_PASSAGE,
               KalayInteriors,
               KALAY_INTERIORS_INN_PASSAGE_FROM_GUEST_ROOM_SOUTHEAST),
    SCENE_EXIT(KALAY_INTERIORS_GUEST_ROOM_NORTHWEST_TO_INN_PASSAGE,
               KalayInteriors,
               KALAY_INTERIORS_INN_PASSAGE_FROM_GUEST_ROOM_NORTHWEST),
    SCENE_EXIT(KALAY_INTERIORS_GUEST_ROOM_NORTHEAST_TO_INN_PASSAGE,
               KalayInteriors,
               KALAY_INTERIORS_INN_PASSAGE_FROM_GUEST_ROOM_NORTHEAST),
    SCENE_EXIT(KALAY_INTERIORS_GUEST_ROOM_SOUTHWEST_TO_INN_PASSAGE,
               KalayInteriors,
               KALAY_INTERIORS_INN_PASSAGE_FROM_GUEST_ROOM_SOUTHWEST),
    SCENE_EXITS(KalaySanctum),
    SCENE_EXIT(KALAY_SANCTUM_TO_KALAY, Kalay, KALAY_FROM_KALAY_SANCTUM),
    SCENE_EXITS_END,
};

struct ScenePlacement gKalayInteriors_NoPlacements[] = {
    {SCENE_TABLE_END},
};

// clang-format off
struct ScenePlacement gKalayInteriors_HousePlacements[] = {
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, 0x0, ACTOR_STAND,
        PIXELS(101), 0, PIXELS(181), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, 0x0, ACTOR_STAND,
        PIXELS(167), 0, PIXELS(109), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, 0x0, ACTOR_STAND,
        PIXELS(383), 0, PIXELS(108), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, 0x0, ACTOR_STAND,
        PIXELS(346), 0, PIXELS(109), FACING_NORTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_BALD_MAN_IN_RED_2, 0x0, ACTOR_WANDER,
        PIXELS(573), 0, PIXELS(163), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, 0x0, ACTOR_STAND,
        PIXELS(670), 0, PIXELS(154), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_YOUNG_MAN_IN_GREEN_2, 0x0, ACTOR_STAND,
        PIXELS(183), 0, PIXELS(416), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_OLD_WOMAN, 0x0, ACTOR_STAND,
        PIXELS(120), 0, PIXELS(369), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, 0x0, ACTOR_STAND,
        PIXELS(856), 0, PIXELS(628), FACING_SOUTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, 0x0, ACTOR_STAND,
        PIXELS(780), 0, PIXELS(633), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_MAN_IN_WHITE_TURBAN, 0x0, ACTOR_STAND,
        PIXELS(795), 0, PIXELS(806), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_MAN_IN_WHITE_TURBAN, 0x0, ACTOR_STAND,
        PIXELS(857), 0, PIXELS(841), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gKalayInteriors_InnPlacements[] = {
    { SPRITE_MAN_WITH_DARK_HAIR, 0x0, ACTOR_STAND,
        PIXELS(120), 0, PIXELS(627), FACING_EAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_IN_PINK_APRON, 0x0, ACTOR_WANDER,
        PIXELS(505), 0, PIXELS(665), FACING_EAST, TALK_FACE_PARTY, 0 },
    { SPRITE_CHEF, 0x0, ACTOR_STAND,
        PIXELS(66), 0, PIXELS(622), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_WOMAN_IN_GREEN, 0x0, ACTOR_STAND,
        PIXELS(196), 0, PIXELS(624), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_MAN_CARRYING_A_PACK, 0x0, ACTOR_STAND,
        PIXELS(232), 0, PIXELS(586), FACING_SOUTH + FACING_STEP, TALK_KEEP_FACING, 0 },
    { SPRITE_MAN_CARRYING_A_PACK, 0x0, ACTOR_STAND,
        PIXELS(168), 0, PIXELS(570), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_BALD_MUSCULAR_MAN, 0x0, ACTOR_STAND,
        PIXELS(207), 0, PIXELS(672), FACING_WEST, TALK_FACE_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, 0x0, ACTOR_STAND,
        PIXELS(168), 0, PIXELS(643), FACING_SOUTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_OLD_WOMAN, 0x0, ACTOR_STAND,
        PIXELS(247), 0, PIXELS(736), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_PERSON_WITH_SWORD, 0x0, ACTOR_STAND,
        PIXELS(263), 0, PIXELS(627), FACING_SOUTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_RED_HAIR, 0x0, ACTOR_STAND,
        PIXELS(417), 0, PIXELS(810), FACING_EAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BLUE_HAIR, 0x0, ACTOR_STAND,
        PIXELS(459), 0, PIXELS(820), FACING_WEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gKalayInteriors_SanctumPlacements[] = {
    { SPRITE_MAN_IN_TALL_HAT, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(248), 0, PIXELS(106), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gKalayInteriors_NoEvents[] = {
    { SCENE_EVENTS_END },
};

/// A bare flag as a condition holds while that flag is clear: these lines are
/// spoken before the Colosso tour leaves, the next ones after.
struct SceneEvent gKalayInteriors_HouseEvents[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, KALAY_INTERIORS_HOUSE_NORTHWEST_TO_KALAY },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, KALAY_INTERIORS_HOUSE_NORTHEAST_UPPER_TO_KALAY },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, KALAY_INTERIORS_HOUSE_NORTHEAST_LOWER_TO_KALAY },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, KALAY_INTERIORS_HOUSE_SOUTHEAST_TO_KALAY },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, KALAY_INTERIORS_WEAPON_SHOP_COUNTER_TO_KALAY },
    { EVENT_EXIT, 6, CONDITION_ALWAYS, KALAY_INTERIORS_WEAPON_SHOP_TO_KALAY },
    { EVENT_EXIT, 7, CONDITION_ALWAYS, KALAY_INTERIORS_ARMOR_SHOP_TO_KALAY },
    { EVENT_EXIT, 8, CONDITION_ALWAYS, KALAY_INTERIORS_ARMOR_SHOP_COUNTER_TO_KALAY },
    { TOUCH_WALKING_INTO(FACING_NORTH), 16, CONDITION_ALWAYS, EVENT_SCRIPT(KalayInteriors_OpenDoorAndExit) },
    { EVENT_EXIT, 18, CONDITION_ALWAYS, KALAY_INTERIORS_GUEST_ROOM_NORTHEAST_TO_INN_PASSAGE },
    { TOUCH_WALKING_INTO(FACING_NORTH), 19, CONDITION_ALWAYS, EVENT_SCRIPT(KalayInteriors_OpenDoorAndExit) },
    { EVENT_TALK, ACTOR_YOUNG_MAN_ON_LUNPA_GONE_BAD, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsImShockedToHearTheRighteous) },
    { EVENT_TALK, ACTOR_WOMAN_SAYING_LUNPA_WAS_KIND, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsLunpaWasAKindThiefHis) },
    { EVENT_TALK, ACTOR_MAN_ON_THE_NORTH, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsSomehowTheNorthHasBecomeThe) },
    { EVENT_TALK, ACTOR_WOMAN_ON_THE_VAULT_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsAMerchantFromVaultHadBeen) },
    { EVENT_TALK, ACTOR_WORRIED_FATHERS_SON, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsNoOneIsAllowedToGo) },
    { EVENT_TALK, ACTOR_WORRIED_FATHER, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsIDoNotWantMySon) },
    { EVENT_TALK, ACTOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsTheMostImportantSkillForA) },
    { EVENT_TALK, ACTOR_MERCHANTS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsMySonReceivedHisKnackFor) },
    { EVENT_TALK, ACTOR_YOUNG_MAN_ASKING_ABOUT_THE_SOUTH, CONDITION_ALWAYS, EVENT_SCRIPT(KalayInteriors_AskAboutSouthernContinent) },
    { EVENT_TALK, ACTOR_WOMAN_BOUND_FOR_THE_TOUR, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsIBetterGetReadyTheColosso) },
    { EVENT_TALK, ACTOR_MAN_FROM_LALIVERO, FLAG_KALAY_COLOSSO_TOUR_LEFT, MSGNO(MsgKalayInteriorsWeFledFromGondowanToThe) },
    { EVENT_TALK, ACTOR_MAN_FROM_LALIVERO, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsWeCameHereToEscapeThe) },
    { EVENT_TALK, ACTOR_SECOND_MAN_FROM_LALIVERO, FLAG_KALAY_COLOSSO_TOUR_LEFT, MSGNO(MsgKalayInteriorsLaliveroWhereImFromIsThe) },
    { EVENT_TALK, ACTOR_SECOND_MAN_FROM_LALIVERO, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsWeEscapedFromTheNortheasternRegion) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_ON_LUNPA_GONE_BAD, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsTheRegionNorthOfVaultWas) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_SAYING_LUNPA_WAS_KIND, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsLunpasReputationIsVeryBadNowadays) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_NORTH, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsThreeYearsAgoThievesRaidedMt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_VAULT_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsTheySayOtherNorthernPartsAre) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WORRIED_FATHERS_SON, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsImAnAdultNowIDont) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WORRIED_FATHER, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsIDontCareHowMySon) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsTheMostImportantThingIsNot) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MERCHANTS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsMySonIsVeryTightWith) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_ASKING_ABOUT_THE_SOUTH, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsPeopleFromTheContinentSouthOf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_BOUND_FOR_THE_TOUR, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsImPrettyExcitedBecauseThisIs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_FROM_LALIVERO, FLAG_KALAY_COLOSSO_TOUR_LEFT, MSGNO(MsgKalayInteriorsIFledAllThisWayNow) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_FROM_LALIVERO, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsIWonderWhatColossoIsLike) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_MAN_FROM_LALIVERO, FLAG_KALAY_COLOSSO_TOUR_LEFT, MSGNO(MsgKalayInteriorsItLooksLikeColossoWillSoon) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_MAN_FROM_LALIVERO, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsIWasSoScaredComingTo) },
    { SEARCH(SEARCH_BARREL), 100, FLAG_KALAY_INTERIORS_COINS_BARREL, FIND_COINS(11) },
    { SEARCH(SEARCH_OVEN), 101, FLAG_KALAY_INTERIORS_SMOKE_BOMB_OVEN, FIND_ITEM(ITEM_SMOKE_BOMB) },
    { SEARCH(SEARCH_BARREL), 102, FLAG_KALAY_INTERIORS_ELIXIR_BARREL, FIND_ITEM(ITEM_ELIXIR) },
    { SEARCH(SEARCH_OVEN), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKalayInteriorsShesToastingSomeBreadItSmells)) },
    { SEARCH(SEARCH_OVEN), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKalayInteriorsHesMakingCurryItsSpicyScent)) },
    // Unused: this oven's item row answers first (see the scenes README).
    { SEARCH(SEARCH_OVEN), 101, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKalayInteriorsHesMakingRoastedEggplantWithCheese)) },
    { SEARCH(SEARCH_OVEN), 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKalayInteriorsItsAKalaySaladWithCucumber)) },
    { SEARCH(SEARCH_OVEN), 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKalayInteriorsItsPicklesWithGarlicThePickle)) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gKalayInteriors_InnEvents[] = {
    { EVENT_EXIT, 9, CONDITION_ALWAYS, KALAY_INTERIORS_INN_TO_INN_PASSAGE },
    { EVENT_EXIT, 10, CONDITION_ALWAYS, KALAY_INTERIORS_INN_TO_KALAY },
    { EVENT_EXIT, 11, CONDITION_ALWAYS, KALAY_INTERIORS_INN_PASSAGE_TO_INN },
    { TOUCH_WALKING_INTO(FACING_NORTH), 12, CONDITION_ALWAYS, EVENT_SCRIPT(KalayInteriors_OpenDoorAndExit) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 13, CONDITION_ALWAYS, EVENT_SCRIPT(KalayInteriors_OpenDoorAndExit) },
    { EVENT_EXIT, 14, CONDITION_ALWAYS, KALAY_INTERIORS_INN_PASSAGE_TO_GUEST_ROOM_SOUTHWEST },
    { EVENT_EXIT, 15, CONDITION_ALWAYS, KALAY_INTERIORS_INN_PASSAGE_TO_GUEST_ROOM_SOUTHEAST },
    { EVENT_EXIT, 17, CONDITION_ALWAYS, KALAY_INTERIORS_GUEST_ROOM_NORTHWEST_TO_INN_PASSAGE },
    { EVENT_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(KalayInteriors_TalkToInnkeeper) },
    { EVENT_TALK, ACTOR_INNKEEPERS_WIFE, FLAG_KALAY_COLOSSO_TOUR_LEFT, MSGNO(MsgKalayInteriorsEveryoneIsGoingToTolbiTo) },
    { EVENT_TALK, ACTOR_INNKEEPERS_WIFE, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsEvenOurChefLeftSoWe) },
    { EVENT_TALK, ACTOR_FIRST_TOURIST, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsIveSignedOnForATour) },
    { EVENT_TALK, ACTOR_SECOND_TOURIST, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsTheColossoGladiatorsAreAllStrong) },
    { EVENT_TALK, ACTOR_TOUR_GUIDE, CONDITION_ALWAYS, EVENT_SCRIPT(KalayInteriors_TalkToTourGuide) },
    { EVENT_TALK, ACTOR_SECOND_TOUR_GUIDE, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsEveryoneLooksForwardToGoingTo) },
    { EVENT_TALK, ACTOR_THIRD_TOURIST, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsSurelyLordHammetWillPopUp) },
    { EVENT_TALK, ACTOR_FOURTH_TOURIST, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsIWouldHaveEnteredColossoIf) },
    { EVENT_TALK, ACTOR_INN_GUEST, FLAG_KALAY_COLOSSO_TOUR_LEFT, MSGNO(MsgKalayInteriorsManyOfTheGuestsHereSeem) },
    { EVENT_TALK, ACTOR_INN_GUEST, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsUhOhHesStillHereEven) },
    { EVENT_TALK, ACTOR_WARRIOR_SIGNING_UP_FOR_COLOSSO, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsArentYouGoingToColossoYou) },
    { EVENT_TALK, ACTOR_OURANOS, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsIfYouAreEnteringColossoYou) },
    { EVENT_TALK, ACTOR_SEAN, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsWereSoStrongMakingTheFinals) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, FLAG_KALAY_COLOSSO_TOUR_LEFT, MSGNO(MsgKalayInteriorsHesAShrewdMerchantIfHe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsIdHaveGoneOnTheColosso) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPERS_WIFE, FLAG_KALAY_COLOSSO_TOUR_LEFT, MSGNO(MsgKalayInteriorsEveryYearEveryoneGoesToTolbi) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPERS_WIFE, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsEvenIfMyHusbandWantedTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_TOURIST, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsJoiningATourGroupIsThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_TOURIST, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsIWonderWhichWarriorWillWin) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOUR_GUIDE, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsItsBusinessSoYouCantHelp) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_TOUR_GUIDE, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsImExhaustedFromPreparingThisTour) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_THIRD_TOURIST, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsIWasSoExcitedICouldnt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FOURTH_TOURIST, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsMyAnimalInstinctsStartWakingUp) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_GUEST, FLAG_KALAY_COLOSSO_TOUR_LEFT, MSGNO(MsgKalayInteriorsColossoIsSoCommonWhatsSo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_GUEST, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsICantSeemToRelaxNow) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WARRIOR_SIGNING_UP_FOR_COLOSSO, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsImGoingToSignUpFor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OURANOS, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsIHavePowerAndSeanHas) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEAN, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsWeAreTheToughestOfAll) },
    { SEARCH(SEARCH_BARREL), 100, FLAG_KALAY_INTERIORS_COINS_BARREL, FIND_COINS(11) },
    { SEARCH(SEARCH_OVEN), 101, FLAG_KALAY_INTERIORS_SMOKE_BOMB_OVEN, FIND_ITEM(ITEM_SMOKE_BOMB) },
    { SEARCH(SEARCH_BARREL), 102, FLAG_KALAY_INTERIORS_ELIXIR_BARREL, FIND_ITEM(ITEM_ELIXIR) },
    { SEARCH(SEARCH_OVEN), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKalayInteriorsShesToastingSomeBreadItSmells)) },
    { SEARCH(SEARCH_OVEN), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKalayInteriorsHesMakingCurryItsSpicyScent)) },
    // Unused: this oven's item row answers first (see the scenes README).
    { SEARCH(SEARCH_OVEN), 101, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKalayInteriorsHesMakingRoastedEggplantWithCheese)) },
    { SEARCH(SEARCH_OVEN), 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKalayInteriorsItsAKalaySaladWithCucumber)) },
    { SEARCH(SEARCH_OVEN), 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKalayInteriorsItsPicklesWithGarlicThePickle)) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gKalayInteriors_SanctumEvents[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, KALAY_SANCTUM_TO_KALAY },
    { EVENT_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(KalayInteriors_TalkToHealer) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsICameHereToSpreadGood) },
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
