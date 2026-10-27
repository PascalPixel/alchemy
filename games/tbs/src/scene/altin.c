/// @file
/// Altin, the mining village north of the Alpine Crossing, in two scenes that
/// share this overlay and the area name Altin: SceneId_Altin, the village
/// outdoors, and SceneId_CottageRoomsD, a sheet of rooms drawn as cottages, all
/// of them Altin's: its houses, shops and inn, entered by the village's doors
/// (COTTAGE_ROOMS_D_FROM_ALTIN_NORTHWEST to
/// COTTAGE_ROOMS_D_FROM_ALTIN_SOUTHWEST). The getters pick each scene's tables
/// by gSaveBlock.sceneId, both share one exit table, and a third, empty set of
/// tables serves any other scene.
///
/// The village is flooded by the water beasts of its mine, whose scenes the
/// area names call Altin Peak (altin_peak.c), and two things in Altin's code
/// follow the party's work down there. The water drops in three stages with the
/// three drains it opens by beating the mine's water beasts
/// (FLAG_ALTIN_MINE_FIRST_DRAIN to _THIRD_DRAIN): Scene_Initialize turns them
/// into three scene temp flags, the water dropped once, twice, or drained (the
/// second and third drains both), and Altin_ApplyWaterLevel copies the layout
/// area for that level over the village, with its metatile behaviors, and
/// leaves out whoever would be standing in the water. And every villager has
/// two sets of lines and thoughts, before and after
/// FLAG_HYDROS_STATUES_DEFEATED, whether the party has beaten the hydros
/// statues at the bottom of the mine; the row that needs the flag clear comes
/// first in each table, so the first row that matches picks the time.
///
/// In the village (Altin), three villagers come home one by one as the water
/// drops, and the armor and item merchants wait outside their flooded shops
/// until theirs dry (the item merchant's after the first drop, the armor
/// merchant's after the second). A water beast, drawn with its green and blue
/// swapped (SPRITE_COLOR_RBG), spits water at the houses, its sound and its
/// spray (Altin_SprayInitialize) running from Altin_ApplyWaterLevel, until the
/// party
/// steps near it (Altin_ScareOffBeast): it stops, cries out and hops away
/// into the flood for good (FLAG_ALTIN_WATER_BEAST_LEFT). Doors (step triggers
/// 2 to 9, Altin_EnterDoor, each leaving by the exit of its number) lead into
/// CottageRoomsD and into the Altin Sanctum (ALTIN_TO_SANCTUM), whose wide
/// door is redrawn by hand; coming back out of the Sanctum (ALTIN_FROM_SANCTUM)
/// the area name is not shown again. Three doorways (step triggers 10 to
/// 12, Altin_WalkThroughDoorway) and the exit ALTIN_TO_ALTIN_PEAK_SPRITZ lead
/// into the mine, to Altin Peak's AltinPeakFirstWaterBeast,
/// AltinPeakSouthEntrance, AltinPeakJunction and AltinPeakSpritz. A pillar that
/// Move pushes to column 22 stays there (FLAG_ALTIN_PILLAR_MOVED).
///
/// Inside, in Cottage rooms D (CottageRoomsD), two families shelter in the
/// inn's Room One and Room Two, as the game calls them, until the first and the
/// second drop send them home (CottageRoomsD_PlaceSheltering), and the weapon
/// merchant waits in the inn until the water has drained. Each merchant opens
/// across the counter once the shop has dried, and a cook from Xian paces
/// between his pots (gCottageRoomsD_CookScript).

#include "save_block.h"
#include "version.h"
#include "scene_ids.h"
#include "unit/scene/altin.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/event_commands.h"
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
#include "event_layer.h"
#include "engine_call.h"
#include "scene_tables.h"
#include "field_effect.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "songs.h"
#include "actor_script.h"

#if !TBS_PATCH_3
#define ALTIN_COOK_LAST_POT_X 384
#else
#define ALTIN_COOK_LAST_POT_X 380
#endif

#define MAP_ALTIN_VILLAGE ((s32)&SceneId_Altin)
#define MAP_ALTIN_HOUSES ((s32)&SceneId_CottageRoomsD)

/// The village's actors, numbering the rows of
/// gAltin_ActorTemplates. The people without an event of their own
/// are named by their sprite and by what they say before the hydros statues are
/// beaten.
enum AltinVillageActorId {
  /// A bald, bare-chested strong man (SPRITE_BALD_MUSCULAR_MAN);
  /// Altin_AskAboutFirstVisit.
  /// ★ Named from the game: "Is this your first time to visit Altin?"
  /// (MsgAltinYourFirstTimeVisitAltin)
  ACTOR_GREETING_STRONGMAN = ACTOR_FIRST_PLACED,
  /// A stocky strong man flexing his arms (SPRITE_STRONGMAN);
  /// Altin_AskAboutHydros.
  /// ★ Named from the game: "How can those creatures spit up so much water?"
  /// (MsgAltinHowCanThoseCreaturesSpitUp)
  ACTOR_CURIOUS_STRONGMAN,
  /// A woman with fair hair in a white apron (SPRITE_WOMAN_WITH_FAIR_HAIR).
  /// ★ Named from the game: "My house is totally coated in monster spit."
  /// (MsgAltinHowHorribleMyHouseIsTotally)
  ACTOR_WOMAN_COATED_HOUSE,
  /// A burly grey-haired man (SPRITE_BURLY_MAN_WITH_GREY_HAIR), who wanders.
  /// ★ Named from the game: "Even the mine is submerged."
  /// (MsgAltinEvenTheMineIsSubmergedNo)
  ACTOR_MAN_SUBMERGED_MINE,
  /// A bent old man with a cane (SPRITE_OLD_MAN_WITH_CANE).
  /// ★ Named from the game: "If we could just destroy the monsters"
  /// (MsgAltinIfWeCouldJustDestroyThe)
  ACTOR_OLD_MAN_DESTROY_MONSTERS,
  /// A woman in a green dress (SPRITE_WOMAN_IN_GREEN_DRESS).
  /// ★ Named from the game: "Altin always used to run short on water."
  /// (MsgAltinAltinAlwaysUsedToRunShort)
  ACTOR_WOMAN_WATER_LUCK,
  /// An old woman with white hair (SPRITE_OLD_WOMAN);
  /// Altin_TalkToThirstyOldWoman.
  /// ★ Named from the game: "We can't drink the water the monsters spew!"
  /// (MsgAltinWeCantDrinkWaterMonsters)
  ACTOR_THIRSTY_OLD_WOMAN,
  /// The three who come back as the water recedes, one with each drop
  /// (Altin_ApplyWaterLevel): this man with brown hair
  /// (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR), then a strong man and an old man.
  /// ★ Named from the game: "The waters around here have subsided..."
  /// (MsgAltinTheWatersAroundHereHaveSubsided)
  ACTOR_MAN_WATERS_SUBSIDED,
  /// A bald, bare-chested strong man (SPRITE_BALD_MUSCULAR_MAN).
  /// ★ Named from the game: "The water beasts looked exactly like Altin's
  /// guardian statues..." (MsgAltinTheWaterBeastsLookedExactlyLike)
  ACTOR_STRONGMAN_GUARDIAN_STATUES,
  /// A bent old man with a cane (SPRITE_OLD_MAN_WITH_CANE), who wanders.
  /// ★ Named from the game: "The waters have flushed out tons of rock from the
  /// mine." (MsgAltinTheWatersHaveFlushedOutTons)
  ACTOR_OLD_MAN_ROCK_FLUSHED_OUT,
  /// The beast spitting water at the houses until FLAG_ALTIN_WATER_BEAST_LEFT.
  /// ★ Named from the game: "Are you the ones who defeated the water beasts?"
  /// (MsgAltinOnesWhoDefeatedWaterBeasts)
  ACTOR_WATER_BEAST,
  /// SPRITE_STONE_STATUE, which Move pushes to column 22
  /// (Altin_CheckPillarMoved).
  /// ★ Named from what the player sees: a grey stone statue on a pedestal
  /// above the north path (its render)
  ACTOR_STONE_STATUE,
  /// The armor and item merchants, waiting outside while their shops are
  /// under water.
  /// ★ Named from the game: "My store is submerged... I want to sell armor,
  /// but there's no way." (MsgAltinMyStoreSubmergedWantSell)
  ACTOR_FLOODED_ARMOR_MERCHANT,
  /// ★ Named from the game: "Oh, no! What will I do if all my merchandise is
  /// soaked?" (MsgAltinWillDoIfMyMerchandise)
  ACTOR_FLOODED_ITEM_MERCHANT,
  /// Three crates placed with SPRITE_CRATE, which Altin_ApplyWaterLevel hides
  /// (SPRITE_COLOR_HIDDEN) but leaves solid, as placed actors are:
  /// invisible blocks at the top of the ladder on the east cliff and, two of
  /// them, on the rails below the doorway into the mine (the drained village's
  /// render with them drawn).
  /// ★ Named from the game: "Strange... I don't remember all these crates being
  /// up here before!" (MsgVaultVaultCutFree)
  ACTOR_FIRST_CRATE,
  /// ★ Named from the game: "Strange... I don't remember all these crates being
  /// up here before!" (MsgVaultVaultCutFree)
  ACTOR_SECOND_CRATE,
  /// ★ Named from the game: "Strange... I don't remember all these crates being
  /// up here before!" (MsgVaultVaultCutFree)
  ACTOR_THIRD_CRATE
};

/// The houses' actors, numbering the rows of
/// gCottageRoomsD_ActorTemplates, named as the village's are.
enum AltinHouseActorId {
  /// The family sheltering in the inn's Room One until the first drop: this
  /// man with brown hair (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR) and a woman with
  /// fair
  /// hair.
  /// ★ Named from the game: "The guests in Room One moved back home when the
  /// waters receded." (MsgAltinTheGuestsInRoomOneMoved)
  ACTOR_ROOM_ONE_GUEST = ACTOR_FIRST_PLACED,
  /// A woman with fair hair in a white apron (SPRITE_WOMAN_WITH_FAIR_HAIR).
  /// ★ Named from the game: "The guests in Room One moved back home when the
  /// waters receded." (MsgAltinTheGuestsInRoomOneMoved)
  ACTOR_ROOM_ONE_SECOND_GUEST,
  /// The family in Room Two, until the second drop: this bent old man with a
  /// cane (SPRITE_OLD_MAN_WITH_CANE) and an old woman.
  /// ★ Named from the game: "The guests in Room Two also went home after the
  /// waters receded." (MsgAltinTheGuestsInRoomTwoAlso)
  ACTOR_ROOM_TWO_GUEST,
  /// An old woman with white hair (SPRITE_OLD_WOMAN).
  /// ★ Named from the game: "The guests in Room Two also went home after the
  /// waters receded." (MsgAltinTheGuestsInRoomTwoAlso)
  ACTOR_ROOM_TWO_SECOND_GUEST,
  /// A man with brown hair (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR). He and
  /// ACTOR_WOMAN_NO_PLACE_LIKE_HOME are the kissing girl's "Mom and Pop", who
  /// have been acting weird since the ruins were found: his thoughts won't
  /// give up a spot at the end of the mine's easternmost passage, and hers say
  /// "My husband's sense for finding treasure has never failed."
  /// ★ Named from the game: "I didn't want to leave my hometown..."
  /// (MsgAltinIDidntWantToLeaveMy)
  ACTOR_MAN_BACK_IN_HIS_HOMETOWN,
  /// A woman with fair hair in a white apron (SPRITE_WOMAN_WITH_FAIR_HAIR).
  /// ★ Named from the game: "There's no place like home."
  /// (MsgAltinIHeardSomeoneKilledTheWater)
  ACTOR_WOMAN_NO_PLACE_LIKE_HOME,
  /// A little girl with orange pigtails (SPRITE_GIRL_WITH_PIGTAILS);
  /// CottageRoomsD_AskAboutMonsters.
  /// ★ Named from the game: "Here, let me thank you with...a kiss!"
  /// (MsgAltinDefeatedThoseMonstersDidnt)
  ACTOR_KISSING_GIRL,
  /// ★ Named from the game: "Cripes... Is this how my weapon shop ends?"
  /// (MsgAltinCripesIsThisHowMyWeapon)
  ACTOR_WEAPON_MERCHANT,
  /// ★ Named from the game: "Oh, no! What will I do if all my merchandise is
  /// soaked?" (MsgAltinWillDoIfMyMerchandise)
  ACTOR_ITEM_MERCHANT,
  /// ★ Named from the game: "It's great that I can sell armor out of my shop
  /// again." (MsgAltinItsGreatCanSellArmor)
  ACTOR_ARMOR_MERCHANT,
  /// Opens the inn (CottageRoomsD_TalkToInnkeeper).
  /// ★ Named from the game: "Innkeeper" (MsgDebugGlyphName)
  ACTOR_INNKEEPER,
  /// A woman with red-brown hair in a white apron (SPRITE_WOMAN_IN_PINK_APRON).
  /// ★ Named from the game: "return Altin to its former glory."
  /// (MsgAltinIdGiveAnythingForSomeoneTo)
  ACTOR_WOMAN_FORMER_GLORY,
  /// ★ Named from the game: "I came from Xian. I cook good food."
  /// (MsgAltinICameFromXianICook)
  ACTOR_COOK,
  /// A man with brown hair (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR) in the inn,
  /// telling
  /// of its guests going home as the water drops.
  /// ★ Named from the game: "None of us can go home until the water
  /// subsides." (MsgAltinNoneOfUsCanGoHome)
  ACTOR_MAN_WAITING_TO_GO_HOME
};

/// The village's water level, set on entry from the mine's drain flags.
enum {
  /// ★ Named from the game: "The guests in Room One moved back home when the
  /// waters receded." (MsgAltinTheGuestsInRoomOneMoved)
  FLAG_ALTIN_WATER_DROPPED_ONCE = FLAG_SCENE_TEMP_40,
  /// ★ Named from the game: "The guests in Room Two also went home after the
  /// waters receded." (MsgAltinTheGuestsInRoomTwoAlso)
  FLAG_ALTIN_WATER_DROPPED_TWICE = FLAG_SCENE_TEMP_41,
  /// ★ Named from the game: "Thank goodness the water has receded."
  /// (MsgAltinThankGoodnessWaterHasReceded)
  FLAG_ALTIN_WATER_DRAINED = FLAG_SCENE_TEMP_42
};

enum { SHOP_ALTIN_WEAPONS = 19, SHOP_ALTIN_ARMOR = 20, SHOP_ALTIN_ITEMS = 21 };

/// Altin's inn, as Inn_CheckIn numbers the inns.
enum { ALTIN_INN = 6 };

/// The metatile behaviors Altin's triggers answer.
enum AltinMetatileBehavior {
  /// The doors' step triggers are numbered from 2 in the village's triggers,
  /// and each takes the exit of its own number.
  MB_ALTIN_FIRST_DOOR = 2,
  MB_ALTIN_NEAR_BEAST = 20
};

/// The doors of the village, by their index in gAltin_Doors.
enum {
  /// The Sanctum's door, which leads to SANCTUM_FROM_ALTIN.
  DOOR_SANCTUM = ALTIN_TO_SANCTUM - MB_ALTIN_FIRST_DOOR,
  DOOR_STRAIGHT_IN = ALTIN_TO_COTTAGE_ROOMS_D_EAST - MB_ALTIN_FIRST_DOOR
};

#define WATER_BEAST_HOP_PERIOD 60
#define WATER_SPRAY_LIFETIME 20
#define WATER_SPRAY_START_SCALE 0x4ccc

// UB: the native facing test overflows a signed left shift for many headings.
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
  ((timer) * 0x1000 + (wobble) * 0x8000)
#else
#define WATER_SPRAY_LATERAL_STEP(timer, wobble)                                \
  (((timer) << 12) + ((wobble) << 15))
#endif

static void Altin_ApplyWaterLevel(void);
static void CottageRoomsD_PlaceSheltering(void);
static void Altin_BeastSpawnSpray(void);

const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 sceneId = gSaveBlock.sceneId;

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
const struct ActorTemplate *Scene_GetActorTemplates(void) {
  s32 sceneId = gSaveBlock.sceneId;
  if (sceneId == MAP_ALTIN_VILLAGE) {
    if (GameFlag_Get(FLAG_HYDROS_STATUES_DEFEATED)) {
      gAltin_ActorTemplates[ACTOR_WOMAN_WATER_LUCK - ACTOR_FIRST_PLACED]
          .talkFacing = TALK_FACING_PARTY;
      gAltin_ActorTemplates[ACTOR_THIRSTY_OLD_WOMAN - ACTOR_FIRST_PLACED]
          .talkFacing = TALK_FACING_PARTY;
    }
    return gAltin_ActorTemplates;
  }
  if (sceneId == MAP_ALTIN_HOUSES) {
    if (GameFlag_Get(FLAG_ALTIN_MINE_FIRST_DRAIN))
      gCottageRoomsD_ActorTemplates[ACTOR_ROOM_ONE_SECOND_GUEST -
                                    ACTOR_FIRST_PLACED]
          .talkFacing = TALK_FACING_PARTY_AND_BACK;
    if (GameFlag_Get(FLAG_ALTIN_MINE_SECOND_DRAIN) ||
        GameFlag_Get(FLAG_ALTIN_MINE_THIRD_DRAIN))
      gCottageRoomsD_ActorTemplates[ACTOR_ROOM_TWO_SECOND_GUEST -
                                    ACTOR_FIRST_PLACED]
          .talkFacing = TALK_FACING_PARTY_AND_BACK;
    Scene_AssignViewFlags(gCottageRoomsD_ActorTemplates);
    return gCottageRoomsD_ActorTemplates;
  }
  return gAltin_NoActorTemplates;
}

const struct Trigger *Scene_GetTriggers(void) {
  s16 sceneId = gSaveBlock.sceneId;

  if (sceneId == MAP_ALTIN_VILLAGE) {
    return gAltin_Triggers;
  }
  if (sceneId == MAP_ALTIN_HOUSES) {
    return gCottageRoomsD_Triggers;
  }
  return gAltin_NoTriggers;
}

/// A bald strong man asks whether this is the party's first visit. He's in
/// Altin's village before the hydros statues are beaten; either way, he says
/// the flood has changed Altin.
static void Altin_AskAboutFirstVisit(void) {
  Event_Start();
  Event_SetMessageId(MsgAltinYourFirstTimeVisitAltin);

  FieldMessage_ShowAtActor(ACTOR_GREETING_STRONGMAN, 0);

  if (Event_AskYesNoWithActorAnimation(0, 0) == 1) {
    Event_ShowMessageAndWait(ACTOR_GREETING_STRONGMAN, 0);
  } else {
    Event_SkipMessages(1);
    Event_AskYesNoAndShowResponse(ACTOR_GREETING_STRONGMAN, 0);
  }

  Event_End();
}

/// A strong man asks whether the slain hydros creatures gushed water. He's in
/// Altin's village once the hydros statues are beaten.
static void Altin_AskAboutHydros(void) {
  Event_Start();
  Event_SetMessageId(MsgAltinDidSeeWaterGushingOut);
  Event_AskYesNoAndShowResponse(ACTOR_CURIOUS_STRONGMAN, 0);
  Event_End();
}

/// An old woman in Altin's village. While the beasts live, she complains that
/// their water cannot be drunk, and the first time the camera pans to the water
/// beast spitting at the houses. Once the statues are gone, she asks for help
/// rebuilding.
static void Altin_TalkToThirstyOldWoman(void) {

  struct Actor *beast;
  struct Actor *leader;

  Event_Start();
  if (GameFlag_Get(FLAG_HYDROS_STATUES_DEFEATED) != 0) {
    Event_SetMessageId(MsgAltinOnesWhoDefeatedWaterBeasts);
    Event_AskYesNo(ACTOR_THIRSTY_OLD_WOMAN, 0);
  } else {
    Import_ActorSetAnimation(ACTOR_THIRSTY_OLD_WOMAN, ANIM_SHAKE_HEAD);
    Event_SetMessageId(MsgAltinWeCantDrinkWaterMonsters);
    FieldEvent_ShowMessageThenWait(ACTOR_THIRSTY_OLD_WOMAN, 0, 10);
    if (GameFlag_Get(FLAG_ALTIN_WATER_BEAST_LEFT) == 0) {
      beast = Actor_GetById(ACTOR_WATER_BEAST);

      FieldCamera_GetActor()->motionFlags = 0;
      FieldCamera_SetSpeed(0x10000, 0x2000);
      FieldCamera_MoveTo(beast->x.fixed, beast->y.fixed, beast->z.fixed, 1);
      // BUG: FACING_SOUTH's value is passed as an actor id. No actor has
      // it, so the leader does not turn south toward the beast.
#ifdef BUGFIX
      Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
#else
      Actor_FaceActor(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
#endif
      Actor_FaceDirection(ACTOR_THIRSTY_OLD_WOMAN, FACING_SOUTH - FACING_STEP,
                          0);
      FieldCamera_WaitForArrival();
      Event_WaitFrames(120);
      leader = Actor_GetById(ACTOR_PARTY_LEADER);
      FieldCamera_MoveTo(leader->x.fixed, leader->y.fixed, leader->z.fixed, 1);
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
  Event_AskYesNoAndShowResponse(ACTOR_OLD_MAN_ROCK_FLUSHED_OUT, 0);
  Event_End();
}

/// A woman asks whether the party is who the girl from Xian was looking for.
/// She's from the family that sheltered in the inn's Room One, and asks inside
/// Altin once the hydros statues are beaten.
static void CottageRoomsD_AskAboutXianGirl(void) {

  Event_Start();
  Event_SetMessageId(MsgAltinGirlFromXianWasAsking);
  Event_AskYesNoAndShowResponse(ACTOR_ROOM_ONE_SECOND_GUEST, 0);
  Event_End();
}

/// An old man points the party to the bottom of the mine. He's from the family
/// sheltering in the inn's Room Two, and inside Altin, before the hydros
/// statues are beaten, he asks whether the party seeks the way west.
static void CottageRoomsD_AskAboutWayWest(void) {

  Event_Start();
  Event_SetMessageId(MsgAltinTryingFindYourWayWest);
  Event_AskYesNoAndShowResponse(ACTOR_ROOM_TWO_GUEST, 0);
  Event_End();
}

/// A little girl thanks the leader with a kiss. She's back home inside Altin
/// before the hydros statues are beaten, and asks whether the party defeated
/// the monsters; she gives the kiss either way.
static void CottageRoomsD_AskAboutMonsters(void) {

  Event_Start();
  Event_SetMessageId(MsgAltinDefeatedThoseMonstersDidnt);
  Event_AskYesNoAndShowResponse(ACTOR_KISSING_GIRL, 0);
  Event_End();
}

/// A man at the inn asks whether the remaining beasts are deep below. He's
/// staying at Altin's inn once the hydros statues are beaten.
static void CottageRoomsD_AskAboutMineBeasts(void) {

  Event_Start();
  Event_SetMessageId(MsgAltinThereFewBeastsInMine);
  Event_AskYesNoAndShowResponse(ACTOR_MAN_WAITING_TO_GO_HOME, 0);
  Event_End();
}

/// Returns whether the party leader faces north, toward a shop counter: its
/// facing lies strictly between northwest and northeast, measured in 16 bits.
static s32 Altin_LeaderFacesCounter(void) {
  struct Actor *leader = Actor_GetById(ACTOR_PARTY_LEADER);

  if (ALTIN_FACES_COUNTER(leader->facing)) {
    return 1;
  }
  return 0;
}

/// Altin's weapon merchant: until the water drains he waits in the inn and
/// cannot sell; afterwards he opens his shop when spoken to across the
/// counter.
static void CottageRoomsD_TalkToWeaponMerchant(void) {

  if (GameFlag_Get(FLAG_ALTIN_WATER_DRAINED) == 0) {
    Event_Start();
    Event_SetMessageId(MsgAltinDoWantWeapons);

    Event_AskYesNoAndShowResponse(ACTOR_WEAPON_MERCHANT, 0);
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
  Event_ShowMessageAndWait(ACTOR_WEAPON_MERCHANT, 0);
  Event_End();
}

/// Altin's armor merchant, in either scene: outside, while his shop is under
/// water, he laments; once it has dried after the second drop, he sells across
/// his counter in the houses.
static void Altin_TalkToArmorMerchant(void) {

  if (GameFlag_Get(FLAG_ALTIN_WATER_DROPPED_TWICE) == 0) {
    Event_Start();
    Event_SetMessageId(MsgAltinMyStoreSubmergedWantSell);
    Event_ShowMessageAndWait(ACTOR_FLOODED_ARMOR_MERCHANT, 0);
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
  Event_ShowMessageAndWait(ACTOR_ARMOR_MERCHANT, 0);
  Event_End();
}

/// Altin's item merchant, the same way: she reopens after the first drop.
static void Altin_TalkToItemMerchant(void) {
  if (GameFlag_Get(FLAG_ALTIN_WATER_DROPPED_ONCE) == 0) {
    Event_Start();
    Event_SetMessageId(MsgAltinWillDoIfMyMerchandise);
    Event_ShowMessageAndWait(ACTOR_FLOODED_ITEM_MERCHANT, 0);
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
  Event_ShowMessageAndWait(ACTOR_ITEM_MERCHANT, 0);
  Event_End();
}

/// Altin's innkeeper opens the inn when spoken to across the counter. Spoken to
/// otherwise, the innkeeper talks about the flood or, once it is over, the
/// temple west of the mine.
static void CottageRoomsD_TalkToInnkeeper(void) {
  struct Actor *leader;

  leader = Actor_GetById(ACTOR_PARTY_LEADER);

  if (ALTIN_FACES_COUNTER(leader->facing)) {
    Inn_CheckIn(ALTIN_INN, ACTOR_INNKEEPER);
    return;
  }

  Event_Start();

  if (GameFlag_Get(FLAG_HYDROS_STATUES_DEFEATED) != 0) {
    Event_SetMessageId(MsgAltinThereSmallTempleWestAltin);
    Event_ShowMessageAndWait(ACTOR_INNKEEPER, 0);
  } else {
    Event_SetMessageId(MsgAltinWeGotLittleDampBut);
    Event_AskYesNoAndShowResponse(ACTOR_INNKEEPER, 0);
  }

  Event_End();
}

/// Walking north into one of the doors of Altin's village: everyone stops, the
/// door opens and the party leader walks in. The Sanctum's wide door is redrawn
/// in two halves by hand; the others play their opening pictures.
static void Altin_EnterDoor(void) {
  u32 actorId;
  struct FieldWork *work;
  struct Actor *actor;
  s32 doorIndex;
  u8 *doorBytes;
  s32 doorByteOffset;
  s32 coordinatesByteOffset;
  s32 column;
  s32 row;
  s32 rightColumn;

  work = gFieldWork;
  Event_Start();
  for (actorId = ACTOR_FIRST_PLACED; actorId <= ACTOR_LAST_ID; actorId++) {
    actor = Actor_GetById(actorId);
    if (actor != 0) {
      actor->motionFlags = 0;
    }
  }

  // The bytes read the entry through byte offsets and sign-extend the door
  // number by shifts; indexing the structs loads the fields differently.
  doorBytes = (u8 *)gAltin_Doors;
  doorIndex = ((*(u16 *)&work->stepTrigger - MB_ALTIN_FIRST_DOOR) << 16) >> 16;
  doorByteOffset = doorIndex << 3;
  coordinatesByteOffset = doorByteOffset + 4;
  column = *(s16 *)(doorBytes + coordinatesByteOffset);
  row = *(s16 *)(doorBytes + coordinatesByteOffset + 2);
  if (doorIndex == DOOR_SANCTUM) {
    // The Sanctum's door slides open with SE_THUMP, as Bilibin's does.
    Event_PlaySong(SE_THUMP);
    MapGrid_CopyCellsTo(42, 33, column, row, 2, 2);
    rightColumn = column + 2;
    MapGrid_CopyCellsTo(42, 35, rightColumn, row, 2, 2);
    Event_WaitFrames(4);
    MapGrid_CopyCellsTo(40, 33, column, row, 2, 2);
    MapGrid_CopyCellsTo(40, 35, rightColumn, row, 2, 2);
    Event_WaitFrames(4);
  } else {
    Event_PlaySong(SE_DOOR_OPEN);
    // The north door's row in gAltin_Doors plays its opening at
    // (39, 17), outside the village's camera box; its house stands at
    // (8, 17), above where the party comes back out
    // (ALTIN_FROM_COTTAGE_ROOMS_D_NORTH), so that doorway is drawn open here
    // (a render with this copy made on entry shows the open doorway).
    if (doorIndex == ALTIN_TO_COTTAGE_ROOMS_D_NORTH - MB_ALTIN_FIRST_DOOR) {
      MapGrid_CopyCellsTo(33, 42, 8, 17, 1, 2);
    }
    MapGrid_AnimateCells(gAltin_Doors[doorIndex].steps, column, row);
  }

  // Walk the leader through.
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Actor_GetById(ACTOR_PARTY_LEADER)->motionFlags = 0;
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
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
  Event_WaitFrames(10);
  Import_EventSetExitRequest(work->stepTrigger);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

/// Walking north into one of the doorways of Altin's village that lead into
/// the mine: the leader walks out.
static void Altin_WalkThroughDoorway(void) {
  struct FieldWork *work;

  work = gFieldWork;
  Event_Start();
  Actor_GetById(ACTOR_PARTY_LEADER)->motionFlags = 0;
  Event_PlaySong(SE_MAP_EXIT);
  Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, -16);
  Import_EventSetExitRequest(work->stepTrigger);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

/// Update routine of the hopping water beast in Altin's village: once a
/// second it leaps up, falls back with a splash that shakes the screen
/// briefly, and stands while on the ground.
static void Altin_BeastHop(struct Actor *actor) {
  struct Actor *beast = actor;
  s16 *shakeFrames = (s16 *)&beast->userValue;

  // While this update routine owns the beast, timer is its hop phase, work
  // counts down the landing shake, and the effect's countdown is a
  // landing-sound latch. Read work through its corresponding signed type for
  // the countdown test.
  if (*shakeFrames != 0) {
    if (--*shakeFrames == 1)
      MapGrid_SetShake(-1, -1, 0xe666);
  }
  if (beast->velocityY == 0) {
    Actor_SelectAnimation(beast, 1);
    beast->y.fixed += -0x18000;
    if (beast->y.fixed < beast->groundHeight) {
      if (((struct FieldEffect *)actor)->countdown != 0) {
        Event_PlaySong(SE_SPLASH);
        ((struct FieldEffect *)actor)->countdown = 0;
        beast->userValue = 4;
        MapGrid_SetShake(0, 0x10000, 0x10000);
      }
      beast->y.fixed = beast->groundHeight;
    }
    beast->paused = 1;
  } else {
    beast->paused = 0;
  }
  if (beast->timer == 0) {
    Event_PlaySong(SE_JUMP);
    ((struct FieldEffect *)actor)->countdown = 1;
    Actor_SelectAnimation(beast, 2);
    beast->velocityY = 0x30000;
  }
  if (++beast->timer == WATER_BEAST_HOP_PERIOD)
    beast->timer = 0;
}

/// The water beast hops away across the flood and dives in with a splash.
static void Altin_BeastHopAway(void) {
  struct Actor *beast;

  beast = Actor_GetById(ACTOR_WATER_BEAST);
  beast->timer = 0;
  beast->userValue = 0;
  beast->gravity = 0x6666;
  beast->update = Altin_BeastHop;
  Actor_SetSpeed(ACTOR_WATER_BEAST, 0x13333, 0x9999);
  Actor_MoveToAndWait(ACTOR_WATER_BEAST, 28, 460);
  Actor_MoveToAndWait(ACTOR_WATER_BEAST, 24, 448);
  Event_PlaySong(SE_SPLASH);
  Actor_ReleaseById(ACTOR_WATER_BEAST);
  MapGrid_SetShake(0, 0x10000, 0x10000);
  Event_WaitFrames(4);
  MapGrid_SetShake(-1, -1, 0xe666);
  Event_WaitFrames(40);
  Import_ActorSetAnimation(ACTOR_WATER_BEAST, ANIM_STAND);
}

/// After Move: remembers whether the pillar now stands in map column 22, the
/// spot it is pushed to.
static void Altin_CheckPillarMoved(void) {
  struct Actor *pillar = Actor_GetById(ACTOR_STONE_STATUE);

  if ((pillar->x.fixed >> 20) == 22) {
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

  sceneId = gSaveBlock.sceneId;
  if (sceneId == MAP_ALTIN_VILLAGE) {
    Altin_ApplyWaterLevel();
  } else if (sceneId == MAP_ALTIN_HOUSES) {
    CottageRoomsD_PlaceSheltering();
  }

  return 0;
}

/// Sets up Altin's village (Altin): the layout area for the current water
/// level (one of four 32 by 32 cell areas kept in the layout), the villagers
/// and merchants present at that level, the water beast with its spray while
/// it remains, and the pushed pillar. Then redraws the map around the leader.
static void Altin_ApplyWaterLevel(void) {
  struct Actor *leader;

  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  // Select the water-level layout area and remove villagers whose areas are
  // flooded.
  if (GameFlag_Get(FLAG_ALTIN_WATER_DRAINED) != 0) {
    MapGrid_CopyCellsTo(64, 32, 0, 32, 32, 32);
    MapGrid_CopyCellAttributes(64, 32, 32, 32, 0, 0);
    Actor_ReleaseById(ACTOR_FLOODED_ARMOR_MERCHANT);
    Actor_ReleaseById(ACTOR_FLOODED_ITEM_MERCHANT);
  } else if (Event_GetFlag(FLAG_ALTIN_WATER_DROPPED_TWICE) != 0) {
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
  // Restore the remaining beast spitting water, with its sound
  // (SE_AMBIENT_WATER) and its spray.
  if (GameFlag_Get(FLAG_ALTIN_WATER_BEAST_LEFT) != 0) {
    Actor_ReleaseById(ACTOR_WATER_BEAST);
  } else {
    Scene_QueueMusic(SE_AMBIENT_WATER);
    Actor_SetSpriteColorById(ACTOR_WATER_BEAST, SPRITE_COLOR_RBG);
    Import_ActorSetAnimation(ACTOR_WATER_BEAST, WATER_BEAST_ANIMATION_SPIT);
    Task_AddFrame(Altin_BeastSpawnSpray, TASK_PASS_FRAME);
  }
  if (gSaveBlock.entrance == ALTIN_FROM_SANCTUM) {
    GameFlag_Clear(FLAG_SHOW_AREA_NAME);
  }
  MapGrid_CopyCellAttributes(0, 33, 4, 3, 20, 41);
  if (GameFlag_Get(FLAG_ALTIN_PILLAR_MOVED) != 0) {
    Actor_SetPosition(ACTOR_STONE_STATUE, PIXELS(360), PIXELS(168));
  }
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_STONE_STATUE), 0);
  // Hide the three crates, which stay solid, and draw them under the
  // actors.
  Actor_SetSpriteColorById(ACTOR_FIRST_CRATE, SPRITE_COLOR_HIDDEN);
  Actor_ConfigureSpriteColor(ACTOR_SECOND_CRATE, SPRITE_COLOR_HIDDEN);
  Actor_SetSpriteColorById(ACTOR_THIRD_CRATE, SPRITE_COLOR_HIDDEN);
  Actor_GetById(ACTOR_FIRST_CRATE)->collisionFlags |= ACTOR_NOT_TARGETED;
  Actor_GetById(ACTOR_SECOND_CRATE)->collisionFlags |= ACTOR_NOT_TARGETED;
  Actor_GetById(ACTOR_THIRD_CRATE)->collisionFlags |= ACTOR_NOT_TARGETED;
  Actor_GetById(ACTOR_FIRST_CRATE)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  Actor_GetById(ACTOR_SECOND_CRATE)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  Actor_GetById(ACTOR_THIRD_CRATE)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  Actor_SetSpritePriority(ACTOR_FIRST_CRATE, 1);
  Actor_SetSpritePriority(ACTOR_SECOND_CRATE, 1);
  Actor_SetSpritePriority(ACTOR_THIRD_CRATE, 1);
  // Redraw at the leader after the map and actor state are ready.
  WaitFrames(1);
  Event_Start();
  FieldCamera_MoveTo(leader->x.fixed, leader->y.fixed, leader->z.fixed, 0);
  MapGrid_CenterOnFollowedPoint();
  Event_End();
  WaitFrames(1);
}

/// Sets up inside Altin's buildings (CottageRoomsD): the families still
/// sheltering in the inn's rooms, the weapon merchant waiting there until the
/// water drains, and the shopkeepers kept from wandering.
static void CottageRoomsD_PlaceSheltering(void) {
  struct Actor *actor;

  if (GameFlag_Get(FLAG_ALTIN_WATER_DROPPED_ONCE) == 0) {
    Actor_SetPosition(ACTOR_ROOM_ONE_GUEST, PIXELS(808), PIXELS(727));
    Actor_GetById(ACTOR_ROOM_ONE_GUEST)->facing = FACING_SOUTH - FACING_STEP;
    Actor_SetPosition(ACTOR_ROOM_ONE_SECOND_GUEST, PIXELS(794), PIXELS(825));
  }
  if (GameFlag_Get(FLAG_ALTIN_WATER_DROPPED_TWICE) == 0) {
    Actor_SetPosition(ACTOR_ROOM_TWO_GUEST, PIXELS(560), PIXELS(710));
    Actor_GetById(ACTOR_ROOM_TWO_GUEST)->facing = FACING_EAST + FACING_STEP;
    Actor_SetPosition(ACTOR_ROOM_TWO_SECOND_GUEST, PIXELS(576), PIXELS(710));
  }
  if (GameFlag_Get(FLAG_ALTIN_WATER_DRAINED) == 0) {
    Actor_SetPosition(ACTOR_WEAPON_MERCHANT, PIXELS(295), PIXELS(744));
    Actor_GetById(ACTOR_WEAPON_MERCHANT)->facing =
        FACING_NORTHWEST + FACING_STEP;
  } else {
    actor = Actor_GetById(ACTOR_WEAPON_MERCHANT);
    actor->collisionFlags |= ACTOR_NEAR_TALK;
  }
  actor = Actor_GetById(ACTOR_ARMOR_MERCHANT);
  if (actor != 0) {
    actor->collisionFlags |= ACTOR_NEAR_TALK;
  }
  actor = Actor_GetById(ACTOR_ITEM_MERCHANT);
  if (actor != 0) {
    actor->collisionFlags |= ACTOR_NEAR_TALK;
  }
}

/// Gives a new spray particle its look: SPRITE_DUST_PUFF standing still, drawn
/// with its red and blue swapped so the dust is water, on sprite priority 1
/// and at 0.3 scale. Altin_SprayDrift turns some white as they fade.
static void Altin_SprayInitialize(struct Actor *spray) {
  spray->motionFlags = 0;
  spray->sprite->image.priority = 1;
  Actor_SetSpriteColor(spray, SPRITE_COLOR_BGR);
  Actor_SetSpriteFlags(spray, 0);
  spray->scaleX = WATER_SPRAY_START_SCALE;
  spray->scaleY = WATER_SPRAY_START_SCALE;
}

/// Update routine of a spray particle: it drifts and grows with a random
/// wobble, then curls upward and flattens as its movement timer runs down.
/// The separate lifetime releases it after 20 updates.
static void Altin_SprayDrift(struct Actor *actor) {
  struct Actor *spray = actor;
  spray->x.fixed += WATER_SPRAY_LATERAL_STEP(
      spray->timer, (s16)((s32)(((u32)Import_RandomNext() * 2) >> 16) - 1));
  if (spray->timer <= 3) {
    spray->z.fixed += -(((u32)Import_RandomNext() * 0x8000) >> 16) - 0x10000;
    spray->scaleX += 0x2666;
    spray->scaleY += -0xa3d;
  } else {
    spray->z.fixed += 0x20000;
    spray->scaleX += 0x7ae;
    spray->scaleY += 0x7ae;
  }
  if (((u32)Import_RandomNext() * spray->timer) >> 16 == 0)
    Actor_SetSpriteColor(spray, SPRITE_COLOR_WHITE);
  if (spray->timer != 0)
    spray->timer--;
  else
    spray->timer = (((u32)Import_RandomNext() * 5) >> 16) * 2 + 2;
  if (--((struct FieldEffect *)actor)->countdown == 0) {
    spray->update = 0;
    Actor_Release(spray);
  }
}

/// Task while the water beast spits: every fourth frame a spray
/// particle starts from its spout, living 20 frames.
static void Altin_BeastSpawnSpray(void) {
  struct Actor *spray;
  if ((gFrameCount & 3) == 0) {
    spray = ((struct Actor *)FieldActor_Create(SPRITE_DUST_PUFF, PIXELS(64), 0,
                                               PIXELS(400)));
    if (spray != 0) {
      spray->timer = WATER_SPRAY_LIFETIME;
      spray->userValue = 0;
      ((struct FieldEffect *)spray)->countdown = WATER_SPRAY_LIFETIME;
      Altin_SprayInitialize(spray);
      spray->update = Altin_SprayDrift;
      Actor_SelectAnimation(spray, 1);
    }
  }
}

/// Stepping near the water beast in Altin's village: the camera pans to it, it
/// stops spitting, turns to the party, cries out and hops away into the flood.
static void Altin_ScareOffBeast(void) {
  Event_Start();
  FieldCamera_SetSpeed(0x6666, 0xccc);
  FieldCamera_MoveTo(PIXELS(63), -1, PIXELS(450), 1);
  FieldCamera_WaitForArrival();
  Event_WaitFrames(30);
  Import_ActorSetAnimation(ACTOR_WATER_BEAST, ANIM_STAND);
  // The spitting stops: its sound and its spray end.
  Scene_QueueMusic(-1);
  Task_Remove(Altin_BeastSpawnSpray);
  Event_WaitFrames(20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_WATER_BEAST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_WATER_BEAST, FACING_EAST, 20);
  Actor_FaceDirection(ACTOR_WATER_BEAST, FACING_NORTH + FACING_STEP, 40);
  Event_PlaySong(SE_BEAST_CRY);
  Actor_RunRepeatedMotion(ACTOR_WATER_BEAST, 2);
  Event_WaitFrames(20);
  Actor_FaceDirection(ACTOR_WATER_BEAST, FACING_NORTHWEST + FACING_STEP, 40);
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
    SC_SET(SW_SPEED, 0x26666),
    SC_SET(SW_ACCELERATION, 0x13333),
    SC_LABEL(1),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(PIXELS(342), 0, PIXELS(746)),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_NORTHWEST + FACING_STEP),
    SC_ANIMATION(ANIM_NOD),
    SC_WAIT(30),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(PIXELS(356), 0, PIXELS(746)),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_NORTH + FACING_STEP),
    SC_ANIMATION(ANIM_NOD),
    SC_WAIT(50),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(PIXELS(342), 0, PIXELS(746)),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_NORTHWEST + FACING_STEP),
    SC_ANIMATION(ANIM_NOD),
    SC_WAIT(20),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(PIXELS(ALTIN_COOK_LAST_POT_X), 0, PIXELS(746)),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_NORTH + FACING_STEP),
    SC_ANIMATION(ANIM_NOD),
    SC_WAIT(30),
    SC_SET(SW_FACING, FACING_EAST + FACING_STEP),
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

struct ActorTemplate gAltin_NoActorTemplates[] = {
    {SCENE_TABLE_END},
};

struct ActorTemplate gAltin_ActorTemplates[] = {
    {SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
     PIXELS(440), 0, PIXELS(200), FACING_SOUTH + FACING_STEP,
     TALK_FACING_PARTY_AND_BACK, 0},
    {SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(408), 0,
     PIXELS(216), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0},
    {SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
     PIXELS(310), 0, PIXELS(217), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY,
     0},
    {SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
     PIXELS(246), 0, PIXELS(186), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY,
     0},
    {SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR,
     PIXELS(58), 0, PIXELS(206), FACING_EAST + FACING_STEP, TALK_FACING_PARTY,
     0},
    {SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
     PIXELS(104), 0, PIXELS(286), FACING_EAST + FACING_STEP,
     TALK_FACING_PARTY_AND_BACK, 0},
    {SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(41), 0,
     PIXELS(441), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY_AND_BACK,
     0},
    {SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
     PIXELS(176), 0, PIXELS(296), FACING_SOUTHEAST + FACING_STEP,
     TALK_FACING_PARTY, 0},
    {SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
     PIXELS(136), 0, PIXELS(461), FACING_EAST + FACING_STEP, TALK_FACING_PARTY,
     0},
    {SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
     PIXELS(350), 0, PIXELS(286), FACING_NORTH, TALK_FACING_PARTY, 0},
    {SPRITE_WATER_BEAST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(60), 0,
     PIXELS(474), FACING_SOUTHEAST, TALK_FACING_PARTY, 0},
    {SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(344), 0,
     PIXELS(168), FACING_SOUTH, TALK_FACING_PARTY, 0},
    {SPRITE_YOUNG_MAN_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
     PIXELS(88), 0, PIXELS(397), FACING_EAST + FACING_STEP, TALK_FACING_PARTY,
     0},
    {SPRITE_WOMAN_WITH_PONYTAIL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
     PIXELS(422), 0, PIXELS(208), FACING_EAST + FACING_STEP, TALK_FACING_PARTY,
     0},
    {SPRITE_CRATE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION, PIXELS(452), 0,
     PIXELS(216), FACING_SOUTH, TALK_FACING_KEEP, 0},
    {SPRITE_CRATE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION, PIXELS(332), 0,
     PIXELS(360), FACING_SOUTH, TALK_FACING_KEEP,
     ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE},
    {SPRITE_CRATE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION, PIXELS(332), 0,
     PIXELS(396), FACING_SOUTH, TALK_FACING_KEEP,
     ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE},
    {SCENE_TABLE_END},
};

// clang-format off
struct ActorTemplate gCottageRoomsD_ActorTemplates[] = {
    {SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(104), 0, PIXELS(120), FACING_EAST + FACING_STEP,
        TALK_FACING_PARTY, 0},
    {SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(105), 0, PIXELS(233), FACING_SOUTH + FACING_STEP,
        TALK_FACING_PARTY, 0},
    {SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(456), 0, PIXELS(182), FACING_WEST + FACING_STEP, TALK_FACING_PARTY_THEN_WATCH,
        0},
    {SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(376), 0, PIXELS(120),
        FACING_SOUTHWEST + FACING_STEP, TALK_FACING_PARTY, 0},
    {SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, 0x0, MOVEMENT_TYPE_STAND, PIXELS(690), 0, PIXELS(128), FACING_SOUTHWEST + FACING_STEP,
        TALK_FACING_PARTY_AND_BACK, 0},
    {SPRITE_WOMAN_WITH_FAIR_HAIR, 0x0, MOVEMENT_TYPE_STAND, PIXELS(669), 0, PIXELS(128), FACING_EAST + FACING_STEP,
        TALK_FACING_PARTY_AND_BACK, 0},
    {SPRITE_GIRL_WITH_PIGTAILS, 0x0, MOVEMENT_TYPE_STAND, PIXELS(604), 0, PIXELS(145), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY_THEN_WATCH, 0},
    {SPRITE_BALD_MAN_IN_RED, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(263), 0, PIXELS(436), FACING_SOUTH + FACING_STEP,
        TALK_FACING_PARTY, 0},
    {SPRITE_WOMAN_WITH_PONYTAIL, 0x0, MOVEMENT_TYPE_STAND, PIXELS(520), 0, PIXELS(454), FACING_SOUTH + FACING_STEP,
        TALK_FACING_PARTY, 0},
    {SPRITE_YOUNG_MAN_IN_GREEN, 0x0, MOVEMENT_TYPE_STAND, PIXELS(743), 0, PIXELS(454), FACING_SOUTHEAST + FACING_STEP,
        TALK_FACING_PARTY, 0},
    {SPRITE_MAN_WITH_DARK_HAIR, 0x0, MOVEMENT_TYPE_STAND, PIXELS(103), 0, PIXELS(744), FACING_SOUTHEAST + FACING_STEP,
        TALK_FACING_PARTY, 0},
    {SPRITE_WOMAN_IN_PINK_APRON, 0x0, MOVEMENT_TYPE_WANDER_NEAR, PIXELS(228), 0, PIXELS(756), FACING_SOUTHWEST + FACING_STEP, TALK_FACING_PARTY, 0},
    {SPRITE_COOK, 0x0, (s32)gCottageRoomsD_CookScript, PIXELS(350), 0, PIXELS(746),
        FACING_SOUTHWEST + FACING_STEP, TALK_FACING_PARTY, 0},
    {SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, 0x0, MOVEMENT_TYPE_STAND, PIXELS(646), 0, PIXELS(734), FACING_SOUTHEAST + FACING_STEP,
        TALK_FACING_PARTY, 0},
    {SCENE_TABLE_END},
};

struct Trigger gAltin_NoTriggers[] = {
    {TRIGGERS_END},
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
    {TRIGGER_STEP, MB_ALTIN_NEAR_BEAST, FLAG_ALTIN_WATER_BEAST_LEFT, EVENT(Altin_ScareOffBeast)},
    {TRIGGERS_END},
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
    {TRIGGERS_END},
};
// clang-format on

/// The opening pictures of the village's doors, two cells square. The
/// Sanctum's door (the second) is wider, and Altin_EnterDoor redraws it by
/// hand instead.
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
