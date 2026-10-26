/// @file
/// Inside Kalay: the houses and the inn, and the Kalay Sanctum.
///
/// - Stucco rooms A (SceneId_StuccoRoomsA), a sheet of rooms drawn in stucco,
/// all
///   of them Kalay's, whose area name is Kalay: every house and the inn, each
///   entered from its own door in the town (kalay.c) at an entrance of
///   its own, and left by the matching exit. The inn's entrances,
///   STUCCO_ROOMS_A_INN_FROM_INN_PASSAGE to
///   STUCCO_ROOMS_A_INN_PASSAGE_FROM_GUEST_ROOM_SOUTHEAST and
///   STUCCO_ROOMS_A_GUEST_ROOM_NORTHWEST_FROM_INN_PASSAGE, take the inn's
///   actor templates and triggers; every other entrance takes
///   the houses'. The inn is full of tourists on their way to see Colosso in
///   Tolbi. The innkeeper offers a room when spoken to across the counter; from
///   elsewhere he asks `Are you going to Tolbi also?`, and the tour's guide in
///   the dining room says `Please finish eating if you are taking the tour!`
///   Either sets FLAG_KALAY_HEARD_OF_COLOSSO_TOUR, after which leaving the town
///   plays the tour group's departure. Once the tour has left
///   (FLAG_KALAY_COLOSSO_TOUR_LEFT), its guests are gone from the inn, two
///   people are gone from the houses, one of whom was about to join it, and
///   those who stay say so. Searching the ovens shows what the cooks are
///   making, except that the oven with roasted eggplant gives a Smoke Bomb: its
///   find comes first in the table under the same metatile behavior, and a
///   named examine trigger ignores its condition, so it answers every time and
///   the eggplant line is unused. Barrels hold 11 coins and an Elixir. The
///   inn's table and the houses' both carry these examine triggers.
/// - Kalay Sanctum (SceneId_KalaySanctum), whose healer offers the
///   Sanctum's services when spoken to across the counter. Its exit leads
///   back into the town (KALAY_FROM_KALAY_SANCTUM).

#include "save_block.h"
#include "scene_ids.h"
#include "unit/scene/kalay_interiors.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/field_services.h"
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
#include "script_layer.h"
#include "scene_tables.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "flags.h"
#include "songs.h"

/// Each table numbers its own rows from ACTOR_FIRST_PLACED: the houses'
/// (gKalayInteriors_HouseActorTemplates), the inn's, whose rooms are
/// entered through the inn's entrances
/// (gKalayInteriors_InnActorTemplates), and the Kalay Sanctum's
/// (gKalayInteriors_SanctumActorTemplates). Unless a note says otherwise,
/// it quotes what its actor says before the Colosso tour leaves.
enum KalayInteriorsActorId {
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
  /// SPRITE_YOUNG_MAN_WITH_BLUE_HAIR, as in ship_rooms.c), named by
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

/// Kalay's inn, as Inn_CheckIn numbers the inns.
enum { KALAY_INN = 7 };

static void KalayInteriors_ApplyRoomState(void);

/// Selects the entrance table of Kalay's houses and inn, or of the
/// Kalay Sanctum.
const struct SceneEntrance *KalayInteriors_GetEntrances(void) {
  s16 sceneId = gSaveBlock.sceneId;

  if (sceneId == (s32)&SceneId_StuccoRoomsA) {
    return gKalayInteriors_HouseEntrances;
  }
  if (sceneId == (s32)&SceneId_KalaySanctum) {
    return gKalayInteriors_SanctumEntrances;
  }
  return gKalayInteriors_NoEntrances;
}

const struct SceneExitRegion *KalayInteriors_GetRegions(void) { return NULL; }

const u32 *KalayInteriors_GetExits(void) { return gKalayInteriors_Exits; }

/// The inn's rooms share one actor template table and the houses
/// another, chosen by entrance and clipped to the current view.
const struct ActorTemplate *KalayInteriors_GetActorTemplates(void) {
  s32 sceneId = gSaveBlock.sceneId;

  if (sceneId == (s32)&SceneId_StuccoRoomsA) {
    s32 entrance = gSaveBlock.entrance;
    struct ActorTemplate *table;

    switch (entrance) {
    case STUCCO_ROOMS_A_INN_FROM_INN_PASSAGE:
    case STUCCO_ROOMS_A_INN:
    case STUCCO_ROOMS_A_INN_PASSAGE_FROM_INN:
    case STUCCO_ROOMS_A_INN_PASSAGE_FROM_GUEST_ROOM_NORTHWEST:
    case STUCCO_ROOMS_A_INN_PASSAGE_FROM_GUEST_ROOM_NORTHEAST:
    case STUCCO_ROOMS_A_INN_PASSAGE_FROM_GUEST_ROOM_SOUTHWEST:
    case STUCCO_ROOMS_A_INN_PASSAGE_FROM_GUEST_ROOM_SOUTHEAST:
    case STUCCO_ROOMS_A_GUEST_ROOM_NORTHWEST_FROM_INN_PASSAGE:
      table = gKalayInteriors_InnActorTemplates;
      break;
    default:
      table = gKalayInteriors_HouseActorTemplates;
      break;
    }
    Scene_AssignViewFlags(table);
    return table;
  }
  if (sceneId == (s32)&SceneId_KalaySanctum) {
    return gKalayInteriors_SanctumActorTemplates;
  }
  return gKalayInteriors_NoActorTemplates;
}

/// Selects room dialogue with the same entrance grouping as the
/// actor templates.
const struct Trigger *KalayInteriors_GetTriggers(void) {
  s32 sceneId = gSaveBlock.sceneId;

  if (sceneId == (s32)&SceneId_StuccoRoomsA) {
    s32 entrance = gSaveBlock.entrance;

    switch (entrance) {
    case STUCCO_ROOMS_A_INN_FROM_INN_PASSAGE:
    case STUCCO_ROOMS_A_INN:
    case STUCCO_ROOMS_A_INN_PASSAGE_FROM_INN:
    case STUCCO_ROOMS_A_INN_PASSAGE_FROM_GUEST_ROOM_NORTHWEST:
    case STUCCO_ROOMS_A_INN_PASSAGE_FROM_GUEST_ROOM_NORTHEAST:
    case STUCCO_ROOMS_A_INN_PASSAGE_FROM_GUEST_ROOM_SOUTHWEST:
    case STUCCO_ROOMS_A_INN_PASSAGE_FROM_GUEST_ROOM_SOUTHEAST:
    case STUCCO_ROOMS_A_GUEST_ROOM_NORTHWEST_FROM_INN_PASSAGE:
      return gKalayInteriors_InnTriggers;
    default:
      return gKalayInteriors_HouseTriggers;
    }
  }
  if (sceneId == (s32)&SceneId_KalaySanctum) {
    return gKalayInteriors_SanctumTriggers;
  }
  return gKalayInteriors_NoTriggers;
}

/// The tour's guide in the dining room of Kalay's inn: now the party has heard
/// of the tour. `Please finish eating if you are taking the tour! We'll be
/// leaving soon!`
static void KalayInteriors_TalkToTourGuide(void) {
  FieldScript_Start();
  FieldScript_SetMessageId(MsgKalayPleaseFinishEatingIfTaking);
  FieldScript_ShowMessageAndWait(ACTOR_TOUR_GUIDE, 0);
  GameFlag_Set(FLAG_KALAY_HEARD_OF_COLOSSO_TOUR);
  FieldScript_End();
}

/// Someone asks whether the party knows the continent south of Tolbi. They're
/// in one of Kalay's houses, and answer either way the party replies.
static void KalayInteriors_AskAboutSouthernContinent(void) {
  FieldScript_Start();
  FieldScript_SetMessageId(MsgKalayDoKnowAboutContinentSouth);
  FieldScript_AskYesNoAndShowResponse(ACTOR_YOUNG_MAN_ASKING_ABOUT_THE_SOUTH,
                                      0);
  FieldScript_End();
}

/// Spoken to across the counter of Kalay's inn, the leader facing west, the
/// innkeeper offers a room. From any other side he asks whether the party is
/// going to Tolbi too, or, once the tour has left, says how empty the inn
/// feels.
static void KalayInteriors_TalkToInnkeeper(void) {
  struct Actor *leader = Actor_GetById(ACTOR_PARTY_LEADER);

  if ((u16)(leader->facing - 0x6001) <= 0x3FFE) {
    Inn_CheckIn(KALAY_INN, ACTOR_INNKEEPER);
  } else {
    FieldScript_Start();

    if (GameFlag_Get(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0) {
      FieldScript_SetMessageId(MsgKalayOurInnFeelsEmptyNow);
      FieldScript_ShowMessageAndWait(ACTOR_INNKEEPER, 0);
    } else {
      FieldScript_SetMessageId(MsgKalayGoingTolbiAlso);
      FieldScript_AskYesNoAndShowResponse(ACTOR_INNKEEPER, 0);
      GameFlag_Set(FLAG_KALAY_HEARD_OF_COLOSSO_TOUR);
    }

    FieldScript_End();
  }
}

/// The Kalay Sanctum's healer offers its services across the counter. He does
/// so when spoken to across the counter with the leader facing north; from
/// elsewhere he says Kalay is a peaceful place.
static void KalayInteriors_TalkToHealer(void) {
  struct Actor *leader = Actor_GetById(ACTOR_PARTY_LEADER);

  // FACING_IS_NORTH removes the native EN halfword shift and changes its
  // comparison constant. Keep the original unsigned-halfword arithmetic.
  if ((u16)(leader->facing + 0x5fff) <= 0x3ffe) {
    Sanctum_Run(ACTOR_HEALER);
  } else {
    FieldScript_Start();
    FieldScript_SetMessageId(MsgKalayCanLiveInPeaceIn);
    FieldScript_ShowMessageAndWait(ACTOR_HEALER, 0);
    FieldScript_End();
  }
}

/// Walking north into one of four doors inside Kalay's houses and inn
/// (step triggers 12, 13, 16 and 19): stops the room's actors, opens the
/// door and walks the leader out.
static void KalayInteriors_OpenDoorAndExit(void) {
  struct FieldWork *work = gFieldWork;
  u32 slot;
  s32 doorIndex;
  struct Actor *leader;

  FieldScript_Start();

  // Prevent idle movement while the doorway cells are animated.
  for (slot = ACTOR_FIRST_PLACED; slot <= ACTOR_LAST_SLOT; slot++) {
    struct Actor *actor = Actor_GetById(slot);

    if (actor != 0) {
      actor->motionFlags = 0;
    }
  }

  switch (work->stepTrigger) {
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

  Sound_PlaySong(SE_DOOR_OPEN);

  {
    u32 x = gKalayInteriors_DoorCells[doorIndex].x;
    u32 y = gKalayInteriors_DoorCells[doorIndex].y;

    MapGrid_AnimateCells(gKalayInteriors_DoorCells[doorIndex].steps, x, y);
  }

  ObjectMotion_SetSpeedParameters(ACTOR_PARTY_LEADER, 0x00008000, 0x00004000);

  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  leader->motionFlags = 0;

  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  ObjectMotion_CenterOnColumn(ACTOR_PARTY_LEADER, 3, -8);
  FieldScript_WaitFrames(10);

  Import_ScriptRequestExit(work->stepTrigger);
  FieldScript_FinishScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldScript_End();
}

/// Sets the opening transition and restores room-specific actors and
/// tiles.
s32 KalayInteriors_Initialize(void) {
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  if (gSaveBlock.sceneId == (s32)&SceneId_StuccoRoomsA) {
    KalayInteriors_ApplyRoomState();
  }
  return 0;
}

/// Once the Colosso tour has left, its guests are gone from Kalay's inn, and
/// two people from the houses; until then the second tour guide is drawn with
/// his green and blue swapped, as in kalay.c. Entering by
/// STUCCO_ROOMS_A_HOUSE_NORTHEAST_LOWER instead blanks four by two cells
/// below the ground floor, where the corner of a neighbouring room would
/// otherwise show at the bottom edge of the view (the room's render with and
/// without the copy).
static void KalayInteriors_ApplyRoomState(void) {
  s16 entrance = gSaveBlock.entrance;

  switch (entrance) {
  case STUCCO_ROOMS_A_HOUSE_NORTHEAST_LOWER: {
    MapGrid_CopyCellsTo(30, 14, 30, 16, 4, 2);
    return;
  }
  case STUCCO_ROOMS_A_INN_FROM_INN_PASSAGE:
  case STUCCO_ROOMS_A_INN:
  case STUCCO_ROOMS_A_INN_PASSAGE_FROM_INN:
  case STUCCO_ROOMS_A_INN_PASSAGE_FROM_GUEST_ROOM_NORTHWEST:
  case STUCCO_ROOMS_A_INN_PASSAGE_FROM_GUEST_ROOM_NORTHEAST:
  case STUCCO_ROOMS_A_INN_PASSAGE_FROM_GUEST_ROOM_SOUTHWEST:
  case STUCCO_ROOMS_A_INN_PASSAGE_FROM_GUEST_ROOM_SOUTHEAST:
  case STUCCO_ROOMS_A_GUEST_ROOM_NORTHWEST_FROM_INN_PASSAGE:
    if (GameFlag_Get(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0) {
      Actor_DestroyById(ACTOR_FIRST_TOURIST);
      Actor_DestroyById(ACTOR_SECOND_TOURIST);
      Actor_DestroyById(ACTOR_TOUR_GUIDE);
      Actor_DestroyById(ACTOR_SECOND_TOUR_GUIDE);
      Actor_DestroyById(ACTOR_THIRD_TOURIST);
      Actor_DestroyById(ACTOR_WARRIOR_SIGNING_UP_FOR_COLOSSO);
      Actor_DestroyById(ACTOR_OURANOS);
      Actor_DestroyById(ACTOR_SEAN);
      Actor_DestroyById(ACTOR_FOURTH_TOURIST);
    } else {
      Actor_ConfigureSpriteColor(ACTOR_SECOND_TOUR_GUIDE, SPRITE_COLOR_RBG);
    }
    return;
  default:
    if (GameFlag_Get(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0) {
      Actor_DestroyById(ACTOR_YOUNG_MAN_ASKING_ABOUT_THE_SOUTH);
      Actor_DestroyById(ACTOR_WOMAN_BOUND_FOR_THE_TOUR);
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
    {STUCCO_ROOMS_A_HOUSE_NORTHWEST, CONDITION_ALWAYS, 144, 0, 192,
     FACING_NORTH, 0, 5, 45, 245, 230, 0},
    {STUCCO_ROOMS_A_HOUSE_NORTHEAST_UPPER, CONDITION_ALWAYS, 400, 0, 192,
     FACING_NORTH, 0, 255, 45, 485, 230, 0},
    {STUCCO_ROOMS_A_HOUSE_NORTHEAST_LOWER, CONDITION_ALWAYS, 624, 0, 240,
     FACING_NORTH, 0, 500, 45, 735, 275, 0},
    {STUCCO_ROOMS_A_HOUSE_SOUTHEAST, CONDITION_ALWAYS, 128, 0, 432,
     FACING_NORTH, 0, 35, 290, 275, 468, 0},
    {STUCCO_ROOMS_A_WEAPON_SHOP_COUNTER, CONDITION_ALWAYS, 384, 0, 384,
     FACING_NORTH, 0, 305, 255, 545, 485, 0},
    {STUCCO_ROOMS_A_WEAPON_SHOP, CONDITION_ALWAYS, 464, 0, 448, FACING_NORTH, 0,
     305, 255, 545, 485, 0},
    {STUCCO_ROOMS_A_ARMOR_SHOP, CONDITION_ALWAYS, 720, 0, 496, FACING_NORTH, 0,
     630, 305, 875, 535, 0},
    {STUCCO_ROOMS_A_ARMOR_SHOP_COUNTER, CONDITION_ALWAYS, 800, 0, 440,
     FACING_NORTH, 0, 630, 305, 875, 535, 0},
    {STUCCO_ROOMS_A_INN_FROM_INN_PASSAGE, CONDITION_ALWAYS, 48, 0, 640,
     FACING_NORTH, 0, 0, 495, 305, 775, 0},
    {STUCCO_ROOMS_A_INN, CONDITION_ALWAYS, 224, 0, 736, FACING_NORTH, 0, 0, 495,
     305, 775, 0},
    {STUCCO_ROOMS_A_INN_PASSAGE_FROM_INN, CONDITION_ALWAYS, 408, 0, 616,
     FACING_SOUTH, 0, 355, 525, 690, 710, 0},
    {STUCCO_ROOMS_A_INN_PASSAGE_FROM_GUEST_ROOM_NORTHWEST, CONDITION_ALWAYS,
     552, 0, 648, FACING_SOUTH, 0, 355, 525, 690, 710, 0},
    {STUCCO_ROOMS_A_INN_PASSAGE_FROM_GUEST_ROOM_NORTHEAST, CONDITION_ALWAYS,
     632, 0, 648, FACING_SOUTH, 0, 355, 525, 690, 710, 0},
    {STUCCO_ROOMS_A_INN_PASSAGE_FROM_GUEST_ROOM_SOUTHWEST, CONDITION_ALWAYS,
     552, 0, 680, FACING_NORTH, 0, 355, 525, 690, 710, 0},
    {STUCCO_ROOMS_A_INN_PASSAGE_FROM_GUEST_ROOM_SOUTHEAST, CONDITION_ALWAYS,
     632, 0, 680, FACING_NORTH, 0, 355, 525, 690, 710, 0},
    {STUCCO_ROOMS_A_GUEST_ROOM_SOUTHEAST_FROM_INN_PASSAGE, CONDITION_ALWAYS,
     808, 0, 616, FACING_SOUTH, 0, 745, 570, 935, 720, 0},
    {STUCCO_ROOMS_A_GUEST_ROOM_NORTHWEST_FROM_INN_PASSAGE, CONDITION_ALWAYS,
     440, 0, 872, FACING_NORTH, 0, 340, 740, 520, 900, 0},
    {STUCCO_ROOMS_A_GUEST_ROOM_NORTHEAST_FROM_INN_PASSAGE, CONDITION_ALWAYS,
     584, 0, 872, FACING_NORTH, 0, 540, 740, 730, 900, 0},
    {STUCCO_ROOMS_A_GUEST_ROOM_SOUTHWEST_FROM_INN_PASSAGE, CONDITION_ALWAYS,
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
    SCENE_EXITS(StuccoRoomsA),
    SCENE_EXIT(STUCCO_ROOMS_A_HOUSE_NORTHWEST_TO_KALAY, Kalay,
               KALAY_FROM_HOUSE_NORTHWEST),
    SCENE_EXIT(STUCCO_ROOMS_A_HOUSE_NORTHEAST_UPPER_TO_KALAY, Kalay,
               KALAY_FROM_HOUSE_NORTHEAST_UPPER),
    SCENE_EXIT(STUCCO_ROOMS_A_HOUSE_NORTHEAST_LOWER_TO_KALAY, Kalay,
               KALAY_FROM_HOUSE_NORTHEAST_LOWER),
    SCENE_EXIT(STUCCO_ROOMS_A_HOUSE_SOUTHEAST_TO_KALAY, Kalay,
               KALAY_FROM_HOUSE_SOUTHEAST),
    SCENE_EXIT(STUCCO_ROOMS_A_WEAPON_SHOP_COUNTER_TO_KALAY, Kalay,
               KALAY_FROM_WEAPON_SHOP_COUNTER),
    SCENE_EXIT(STUCCO_ROOMS_A_WEAPON_SHOP_TO_KALAY, Kalay,
               KALAY_FROM_WEAPON_SHOP),
    SCENE_EXIT(STUCCO_ROOMS_A_ARMOR_SHOP_TO_KALAY, Kalay,
               KALAY_FROM_ARMOR_SHOP),
    SCENE_EXIT(STUCCO_ROOMS_A_ARMOR_SHOP_COUNTER_TO_KALAY, Kalay,
               KALAY_FROM_ARMOR_SHOP_COUNTER),
    SCENE_EXIT(STUCCO_ROOMS_A_INN_TO_INN_PASSAGE, StuccoRoomsA,
               STUCCO_ROOMS_A_INN_PASSAGE_FROM_INN),
    SCENE_EXIT(STUCCO_ROOMS_A_INN_TO_KALAY, Kalay, KALAY_FROM_INN),
    SCENE_EXIT(STUCCO_ROOMS_A_INN_PASSAGE_TO_INN, StuccoRoomsA,
               STUCCO_ROOMS_A_INN_FROM_INN_PASSAGE),
    SCENE_EXIT(STUCCO_ROOMS_A_INN_PASSAGE_TO_GUEST_ROOM_NORTHWEST, StuccoRoomsA,
               STUCCO_ROOMS_A_GUEST_ROOM_NORTHWEST_FROM_INN_PASSAGE),
    SCENE_EXIT(STUCCO_ROOMS_A_INN_PASSAGE_TO_GUEST_ROOM_NORTHEAST, StuccoRoomsA,
               STUCCO_ROOMS_A_GUEST_ROOM_NORTHEAST_FROM_INN_PASSAGE),
    SCENE_EXIT(STUCCO_ROOMS_A_INN_PASSAGE_TO_GUEST_ROOM_SOUTHWEST, StuccoRoomsA,
               STUCCO_ROOMS_A_GUEST_ROOM_SOUTHWEST_FROM_INN_PASSAGE),
    SCENE_EXIT(STUCCO_ROOMS_A_INN_PASSAGE_TO_GUEST_ROOM_SOUTHEAST, StuccoRoomsA,
               STUCCO_ROOMS_A_GUEST_ROOM_SOUTHEAST_FROM_INN_PASSAGE),
    SCENE_EXIT(STUCCO_ROOMS_A_GUEST_ROOM_SOUTHEAST_TO_INN_PASSAGE, StuccoRoomsA,
               STUCCO_ROOMS_A_INN_PASSAGE_FROM_GUEST_ROOM_SOUTHEAST),
    SCENE_EXIT(STUCCO_ROOMS_A_GUEST_ROOM_NORTHWEST_TO_INN_PASSAGE, StuccoRoomsA,
               STUCCO_ROOMS_A_INN_PASSAGE_FROM_GUEST_ROOM_NORTHWEST),
    SCENE_EXIT(STUCCO_ROOMS_A_GUEST_ROOM_NORTHEAST_TO_INN_PASSAGE, StuccoRoomsA,
               STUCCO_ROOMS_A_INN_PASSAGE_FROM_GUEST_ROOM_NORTHEAST),
    SCENE_EXIT(STUCCO_ROOMS_A_GUEST_ROOM_SOUTHWEST_TO_INN_PASSAGE, StuccoRoomsA,
               STUCCO_ROOMS_A_INN_PASSAGE_FROM_GUEST_ROOM_SOUTHWEST),
    SCENE_EXITS(KalaySanctum),
    SCENE_EXIT(KALAY_SANCTUM_TO_KALAY, Kalay, KALAY_FROM_KALAY_SANCTUM),
    SCENE_EXITS_END,
};

struct ActorTemplate gKalayInteriors_NoActorTemplates[] = {
    {SCENE_TABLE_END},
};

// clang-format off
struct ActorTemplate gKalayInteriors_HouseActorTemplates[] = {
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(101), 0, PIXELS(181), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(167), 0, PIXELS(109), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(383), 0, PIXELS(108), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(346), 0, PIXELS(109), FACING_NORTHEAST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_BALD_MAN_IN_RED_KALAY, 0x0, MOVEMENT_TYPE_WANDER,
        PIXELS(573), 0, PIXELS(163), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(670), 0, PIXELS(154), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_YOUNG_MAN_IN_GREEN_KALAY, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(183), 0, PIXELS(416), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_WOMAN, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(120), 0, PIXELS(369), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(856), 0, PIXELS(628), FACING_SOUTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(780), 0, PIXELS(633), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_MAN_IN_WHITE_TURBAN, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(795), 0, PIXELS(806), FACING_NORTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_IN_WHITE_TURBAN, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(857), 0, PIXELS(841), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ActorTemplate gKalayInteriors_InnActorTemplates[] = {
    { SPRITE_MAN_WITH_DARK_HAIR, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(120), 0, PIXELS(627), FACING_EAST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_IN_PINK_APRON, 0x0, MOVEMENT_TYPE_WANDER,
        PIXELS(505), 0, PIXELS(665), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_CHEF, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(66), 0, PIXELS(622), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_WOMAN_IN_GREEN, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(196), 0, PIXELS(624), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_CARRYING_A_PACK, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(232), 0, PIXELS(586), FACING_SOUTH + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_MAN_CARRYING_A_PACK, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(168), 0, PIXELS(570), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_BALD_MUSCULAR_MAN, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(207), 0, PIXELS(672), FACING_WEST, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(168), 0, PIXELS(643), FACING_SOUTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_WOMAN, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(247), 0, PIXELS(736), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_PERSON_WITH_SWORD, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(263), 0, PIXELS(627), FACING_SOUTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_RED_HAIR, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(417), 0, PIXELS(810), FACING_EAST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BLUE_HAIR, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(459), 0, PIXELS(820), FACING_WEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ActorTemplate gKalayInteriors_SanctumActorTemplates[] = {
    { SPRITE_MAN_IN_TALL_HAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(248), 0, PIXELS(106), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct Trigger gKalayInteriors_NoTriggers[] = {
    { TRIGGERS_END },
};

/// A bare flag as a condition holds while that flag is clear: these lines are
/// spoken before the Colosso tour leaves, the next ones after.
struct Trigger gKalayInteriors_HouseTriggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, STUCCO_ROOMS_A_HOUSE_NORTHWEST_TO_KALAY },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, STUCCO_ROOMS_A_HOUSE_NORTHEAST_UPPER_TO_KALAY },
    { TRIGGER_EXIT, 3, CONDITION_ALWAYS, STUCCO_ROOMS_A_HOUSE_NORTHEAST_LOWER_TO_KALAY },
    { TRIGGER_EXIT, 4, CONDITION_ALWAYS, STUCCO_ROOMS_A_HOUSE_SOUTHEAST_TO_KALAY },
    { TRIGGER_EXIT, 5, CONDITION_ALWAYS, STUCCO_ROOMS_A_WEAPON_SHOP_COUNTER_TO_KALAY },
    { TRIGGER_EXIT, 6, CONDITION_ALWAYS, STUCCO_ROOMS_A_WEAPON_SHOP_TO_KALAY },
    { TRIGGER_EXIT, 7, CONDITION_ALWAYS, STUCCO_ROOMS_A_ARMOR_SHOP_TO_KALAY },
    { TRIGGER_EXIT, 8, CONDITION_ALWAYS, STUCCO_ROOMS_A_ARMOR_SHOP_COUNTER_TO_KALAY },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 16, CONDITION_ALWAYS, EVENT_SCRIPT(KalayInteriors_OpenDoorAndExit) },
    { TRIGGER_EXIT, 18, CONDITION_ALWAYS, STUCCO_ROOMS_A_GUEST_ROOM_NORTHEAST_TO_INN_PASSAGE },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 19, CONDITION_ALWAYS, EVENT_SCRIPT(KalayInteriors_OpenDoorAndExit) },
    { TRIGGER_TALK, ACTOR_YOUNG_MAN_ON_LUNPA_GONE_BAD, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsImShockedToHearTheRighteous) },
    { TRIGGER_TALK, ACTOR_WOMAN_SAYING_LUNPA_WAS_KIND, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsLunpaWasAKindThiefHis) },
    { TRIGGER_TALK, ACTOR_MAN_ON_THE_NORTH, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsSomehowTheNorthHasBecomeThe) },
    { TRIGGER_TALK, ACTOR_WOMAN_ON_THE_VAULT_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsAMerchantFromVaultHadBeen) },
    { TRIGGER_TALK, ACTOR_WORRIED_FATHERS_SON, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsNoOneIsAllowedToGo) },
    { TRIGGER_TALK, ACTOR_WORRIED_FATHER, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsIDoNotWantMySon) },
    { TRIGGER_TALK, ACTOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsTheMostImportantSkillForA) },
    { TRIGGER_TALK, ACTOR_MERCHANTS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsMySonReceivedHisKnackFor) },
    { TRIGGER_TALK, ACTOR_YOUNG_MAN_ASKING_ABOUT_THE_SOUTH, CONDITION_ALWAYS, EVENT_SCRIPT(KalayInteriors_AskAboutSouthernContinent) },
    { TRIGGER_TALK, ACTOR_WOMAN_BOUND_FOR_THE_TOUR, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsIBetterGetReadyTheColosso) },
    { TRIGGER_TALK, ACTOR_MAN_FROM_LALIVERO, FLAG_KALAY_COLOSSO_TOUR_LEFT, MSGNO(MsgKalayInteriorsWeFledFromGondowanToThe) },
    { TRIGGER_TALK, ACTOR_MAN_FROM_LALIVERO, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsWeCameHereToEscapeThe) },
    { TRIGGER_TALK, ACTOR_SECOND_MAN_FROM_LALIVERO, FLAG_KALAY_COLOSSO_TOUR_LEFT, MSGNO(MsgKalayInteriorsLaliveroWhereImFromIsThe) },
    { TRIGGER_TALK, ACTOR_SECOND_MAN_FROM_LALIVERO, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsWeEscapedFromTheNortheasternRegion) },
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
    { EXAMINE_TRIGGER(EXAMINE_BARREL), 100, FLAG_STUCCO_ROOMS_A_COINS_BARREL, FIND_COINS(11) },
    { EXAMINE_TRIGGER(EXAMINE_OVEN), 101, FLAG_STUCCO_ROOMS_A_SMOKE_BOMB_OVEN, FIND_ITEM(ITEM_SMOKE_BOMB) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), 102, FLAG_STUCCO_ROOMS_A_ELIXIR_BARREL, FIND_ITEM(ITEM_ELIXIR) },
    { EXAMINE_TRIGGER(EXAMINE_OVEN), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKalayInteriorsShesToastingSomeBreadItSmells)) },
    { EXAMINE_TRIGGER(EXAMINE_OVEN), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKalayInteriorsHesMakingCurryItsSpicyScent)) },
    // Unused: this oven's item row answers first (see the maps README).
    { EXAMINE_TRIGGER(EXAMINE_OVEN), 101, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKalayInteriorsHesMakingRoastedEggplantWithCheese)) },
    { EXAMINE_TRIGGER(EXAMINE_OVEN), 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKalayInteriorsItsAKalaySaladWithCucumber)) },
    { EXAMINE_TRIGGER(EXAMINE_OVEN), 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKalayInteriorsItsPicklesWithGarlicThePickle)) },
    { TRIGGERS_END },
};

struct Trigger gKalayInteriors_InnTriggers[] = {
    { TRIGGER_EXIT, 9, CONDITION_ALWAYS, STUCCO_ROOMS_A_INN_TO_INN_PASSAGE },
    { TRIGGER_EXIT, 10, CONDITION_ALWAYS, STUCCO_ROOMS_A_INN_TO_KALAY },
    { TRIGGER_EXIT, 11, CONDITION_ALWAYS, STUCCO_ROOMS_A_INN_PASSAGE_TO_INN },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 12, CONDITION_ALWAYS, EVENT_SCRIPT(KalayInteriors_OpenDoorAndExit) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 13, CONDITION_ALWAYS, EVENT_SCRIPT(KalayInteriors_OpenDoorAndExit) },
    { TRIGGER_EXIT, 14, CONDITION_ALWAYS, STUCCO_ROOMS_A_INN_PASSAGE_TO_GUEST_ROOM_SOUTHWEST },
    { TRIGGER_EXIT, 15, CONDITION_ALWAYS, STUCCO_ROOMS_A_INN_PASSAGE_TO_GUEST_ROOM_SOUTHEAST },
    { TRIGGER_EXIT, 17, CONDITION_ALWAYS, STUCCO_ROOMS_A_GUEST_ROOM_NORTHWEST_TO_INN_PASSAGE },
    { TRIGGER_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(KalayInteriors_TalkToInnkeeper) },
    { TRIGGER_TALK, ACTOR_INNKEEPERS_WIFE, FLAG_KALAY_COLOSSO_TOUR_LEFT, MSGNO(MsgKalayInteriorsEveryoneIsGoingToTolbiTo) },
    { TRIGGER_TALK, ACTOR_INNKEEPERS_WIFE, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsEvenOurChefLeftSoWe) },
    { TRIGGER_TALK, ACTOR_FIRST_TOURIST, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsIveSignedOnForATour) },
    { TRIGGER_TALK, ACTOR_SECOND_TOURIST, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsTheColossoGladiatorsAreAllStrong) },
    { TRIGGER_TALK, ACTOR_TOUR_GUIDE, CONDITION_ALWAYS, EVENT_SCRIPT(KalayInteriors_TalkToTourGuide) },
    { TRIGGER_TALK, ACTOR_SECOND_TOUR_GUIDE, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsEveryoneLooksForwardToGoingTo) },
    { TRIGGER_TALK, ACTOR_THIRD_TOURIST, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsSurelyLordHammetWillPopUp) },
    { TRIGGER_TALK, ACTOR_FOURTH_TOURIST, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsIWouldHaveEnteredColossoIf) },
    { TRIGGER_TALK, ACTOR_INN_GUEST, FLAG_KALAY_COLOSSO_TOUR_LEFT, MSGNO(MsgKalayInteriorsManyOfTheGuestsHereSeem) },
    { TRIGGER_TALK, ACTOR_INN_GUEST, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsUhOhHesStillHereEven) },
    { TRIGGER_TALK, ACTOR_WARRIOR_SIGNING_UP_FOR_COLOSSO, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsArentYouGoingToColossoYou) },
    { TRIGGER_TALK, ACTOR_OURANOS, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsIfYouAreEnteringColossoYou) },
    { TRIGGER_TALK, ACTOR_SEAN, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsWereSoStrongMakingTheFinals) },
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
    { EXAMINE_TRIGGER(EXAMINE_BARREL), 100, FLAG_STUCCO_ROOMS_A_COINS_BARREL, FIND_COINS(11) },
    { EXAMINE_TRIGGER(EXAMINE_OVEN), 101, FLAG_STUCCO_ROOMS_A_SMOKE_BOMB_OVEN, FIND_ITEM(ITEM_SMOKE_BOMB) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), 102, FLAG_STUCCO_ROOMS_A_ELIXIR_BARREL, FIND_ITEM(ITEM_ELIXIR) },
    { EXAMINE_TRIGGER(EXAMINE_OVEN), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKalayInteriorsShesToastingSomeBreadItSmells)) },
    { EXAMINE_TRIGGER(EXAMINE_OVEN), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKalayInteriorsHesMakingCurryItsSpicyScent)) },
    // Unused: this oven's item row answers first (see the maps README).
    { EXAMINE_TRIGGER(EXAMINE_OVEN), 101, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKalayInteriorsHesMakingRoastedEggplantWithCheese)) },
    { EXAMINE_TRIGGER(EXAMINE_OVEN), 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKalayInteriorsItsAKalaySaladWithCucumber)) },
    { EXAMINE_TRIGGER(EXAMINE_OVEN), 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKalayInteriorsItsPicklesWithGarlicThePickle)) },
    { TRIGGERS_END },
};

struct Trigger gKalayInteriors_SanctumTriggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, KALAY_SANCTUM_TO_KALAY },
    { TRIGGER_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(KalayInteriors_TalkToHealer) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsICameHereToSpreadGood) },
    { TRIGGERS_END },
};
// clang-format on

u16 gKalayInteriors_DoorCellSteps[] = {
    MAPGRID_ANIMATION_STEP(7, 125, 1, 2, 5),
    MAPGRID_ANIMATION_STEP(8, 125, 1, 2, 5),
    MAPGRID_ANIMATION_STEPS_END,
};

struct MapGridCellAnimation gKalayInteriors_DoorCells[] = {
    {gKalayInteriors_DoorCellSteps, 34, 98},
    {gKalayInteriors_DoorCellSteps, 39, 98},
    {gKalayInteriors_DoorCellSteps, 50, 96},
    {gKalayInteriors_DoorCellSteps, 52, 107},
};
