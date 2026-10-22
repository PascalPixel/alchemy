/// @file
/// Altin, the mining village north of the Alpine Crossing, in two scenes that
/// share this overlay and the area name Altin: SceneId_Altin, the village
/// outdoors, and SceneId_AltinInteriors, inside its houses, shops and inn.
/// The getters pick each scene's tables by gGameState.scene, both share one
/// exit table, and a third, empty set of tables serves any other scene.
///
/// The village is flooded by the water beasts of its mine, whose scenes the
/// area names call Altin Peak (altin_peak.c), and two things in Altin's code
/// follow the party's work down there. The water drops in three stages with the
/// three drains it opens by beating the mine's water beasts
/// (FLAG_ALTIN_MINE_FIRST_DRAIN to _THIRD_DRAIN): Scene_Initialize turns them
/// into three scene flags, the water dropped once, twice, or drained (the
/// second and third drains both), and Village_ApplyWaterLevel copies the map
/// block for that level over the village, with its collision, and leaves out
/// whoever would be standing in the water. And every villager has two sets of
/// lines and thoughts, before and after FLAG_HYDROS_STATUES_DEFEATED, whether
/// the party has beaten the hydros statues at the bottom of the mine; the row
/// that needs the flag clear comes first in each table, so the first row that
/// matches picks the time.
///
/// In the village (Altin), three villagers come home one by one as the water
/// drops, and the armor and item merchants wait outside their flooded shops
/// until theirs dry (the item merchant's after the first drop, the armor
/// merchant's after the second). A water beast, drawn with its green and blue
/// swapped (SPRITE_COLOR_RBG), spits water at the houses, its sound and its
/// spray (Spray_Initialize) running from Village_ApplyWaterLevel, until the
/// party
/// steps near it (Village_ScareOffBeast): it stops, cries out and hops away
/// into the flood for good (FLAG_ALTIN_WATER_BEAST_LEFT). Doors (touches 2 to
/// 9, Village_EnterDoor, each leaving by the exit of its number) lead into
/// AltinInteriors and into the Altin Sanctum (ALTIN_TO_SANCTUM), whose wide
/// door is redrawn by hand; coming back out of the Sanctum (ALTIN_FROM_SANCTUM)
/// the area name is not shown again. Three doorways (touches 10 to 12,
/// Village_WalkThroughDoorway) and the exit ALTIN_TO_ALTIN_PEAK_SPRITZ lead
/// into the mine, to Altin Peak's AltinPeakFirstWaterBeast,
/// AltinPeakSouthEntrance, AltinPeakJunction and AltinPeakSpritz. A pillar that
/// Move pushes to column 22 stays there (FLAG_ALTIN_PILLAR_MOVED).
///
/// Inside (AltinInteriors), two families shelter in the inn's Room One and Room
/// Two, as the game calls them, until the first and the second drop send them
/// home (Houses_PlaceSheltering), and the weapon merchant waits in the inn
/// until the water has drained. Each merchant opens across the counter once the
/// shop has dried, and a cook from Xian paces between his pots
/// (gAltinHouses_CookScript).
///
/// Open questions: none.

#include "game_state.h"
#include "version.h"
#include "scene_ids.h"
#include "unit/scene/altin.h"
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
#include "engine_call.h"
#include "field_scene.h"
#include "field_effect.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "songs.h"
#include "objcmd.h"

#if !TBS_PATCH_3
#define ALTIN_COOK_LAST_POT_X 384
#else
#define ALTIN_COOK_LAST_POT_X 380
#endif

#define SCENE_ALTIN_VILLAGE ((s32)&SceneId_Altin)
#define SCENE_ALTIN_HOUSES ((s32)&SceneId_AltinInteriors)

/// The village's actors, numbering the rows of gAltinVillage_Placements.
/// The people without a scene of their own are named by their sprite and by
/// what they say before the hydros statues are beaten.
enum AltinVillageActor {
  /// A bald, bare-chested strong man (SPRITE_BALD_MUSCULAR_MAN);
  /// Village_AskAboutFirstVisit.
  /// ★ Named from the game: "Is this your first time to visit Altin?"
  /// (MsgAltinYourFirstTimeVisitAltin)
  ACTOR_GREETING_STRONG_MAN = ACTOR_FIRST_PLACED,
  /// A stocky strong man flexing his arms (SPRITE_STRONGMAN);
  /// Village_AskAboutHydros.
  /// ★ Named from the game: "How can those creatures spit up so much water?"
  /// (MsgAltinHowCanThoseCreaturesSpitUp)
  ACTOR_CURIOUS_STRONG_MAN,
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
  /// Village_TalkToThirstyOldWoman.
  /// ★ Named from the game: "We can't drink the water the monsters spew!"
  /// (MsgAltinWeCantDrinkWaterMonsters)
  ACTOR_THIRSTY_OLD_WOMAN,
  /// The three who come back as the water recedes, one with each drop
  /// (Village_ApplyWaterLevel): this man with brown hair
  /// (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR), then a strong man and an old man.
  /// ★ Named from the game: "The waters around here have subsided..."
  /// (MsgAltinTheWatersAroundHereHaveSubsided)
  ACTOR_MAN_WATERS_SUBSIDED,
  /// A bald, bare-chested strong man (SPRITE_BALD_MUSCULAR_MAN).
  /// ★ Named from the game: "The water beasts looked exactly like Altin's
  /// guardian statues..." (MsgAltinTheWaterBeastsLookedExactlyLike)
  ACTOR_STRONG_MAN_GUARDIAN_STATUES,
  /// A bent old man with a cane (SPRITE_OLD_MAN_WITH_CANE), who wanders.
  /// ★ Named from the game: "The waters have flushed out tons of rock from the
  /// mine." (MsgAltinTheWatersHaveFlushedOutTons)
  ACTOR_OLD_MAN_ROCK_FLUSHED_OUT,
  /// The beast spitting water at the houses until FLAG_ALTIN_WATER_BEAST_LEFT.
  /// ★ Named from the game: "Are you the ones who defeated the water beasts?"
  /// (MsgAltinOnesWhoDefeatedWaterBeasts)
  ACTOR_WATER_BEAST,
  /// SPRITE_STONE_STATUE, which Move pushes to column 22
  /// (Village_CheckPillarMoved).
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
  /// Three crates placed with SPRITE_CRATE, which Village_ApplyWaterLevel hides
  /// (SPRITE_COLOR_HIDDEN) but leaves solid, as placed actors are: invisible
  /// blocks at the top of the ladder on the east cliff and, two of them, on
  /// the rails below the doorway into the mine (the drained village's render
  /// with them drawn).
  ACTOR_FIRST_CRATE,
  ACTOR_SECOND_CRATE,
  ACTOR_THIRD_CRATE
};

/// The houses' actors, numbering the rows of gAltinHouses_Placements, named as
/// the village's are.
enum AltinHouseActor {
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
  /// Houses_AskAboutMonsters.
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
  /// Opens the inn (Houses_TalkToInnkeeper).
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
  FLAG_WATER_DROPPED_ONCE = FLAG_SCENE_TEMP_40,
  /// ★ Named from the game: "The guests in Room Two also went home after the
  /// waters receded." (MsgAltinTheGuestsInRoomTwoAlso)
  FLAG_WATER_DROPPED_TWICE = FLAG_SCENE_TEMP_41,
  /// ★ Named from the game: "Thank goodness the water has receded."
  /// (MsgAltinThankGoodnessWaterHasReceded)
  FLAG_WATER_DRAINED = FLAG_SCENE_TEMP_42
};

enum {
  SHOP_ALTIN_WEAPONS = 19,
  SHOP_ALTIN_ARMOR = 20,
  SHOP_ALTIN_ITEMS = 21,
  INN_ALTIN = 6
};

/// The doors of the village, by their index in gAltinVillage_Doors.
enum {
  /// The door triggers are numbered from 2 in the village's event table, and
  /// each takes the exit of its own number.
  FIRST_DOOR_TRIGGER = 2,
  /// The Sanctum's door, which leads to SANCTUM_FROM_ALTIN.
  DOOR_SANCTUM = ALTIN_TO_SANCTUM - FIRST_DOOR_TRIGGER,
  DOOR_STRAIGHT_IN = ALTIN_TO_ALTIN_INTERIORS_EAST - FIRST_DOOR_TRIGGER
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

enum { TRIGGER_NEAR_BEAST = 20 };

static void Village_ApplyWaterLevel(void);
static void Houses_PlaceSheltering(void);
static void Beast_SpawnSpray(void);

const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 scene = gGameState.scene;

  if (scene == SCENE_ALTIN_VILLAGE) {
    return gAltinVillage_Entrances;
  }
  if (scene == SCENE_ALTIN_HOUSES) {
    return gAltinHouses_Entrances;
  }
  return gAltinOther_Entrances;
}

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

/// Both scenes share one exit table.
const u32 *Scene_GetExits(void) { return gAltin_Exits; }

/// Some villagers only turn back after talking once the flood is over or
/// their family has gone home.
const struct ScenePlacement *Scene_GetPlacements(void) {
  s32 scene = gGameState.scene;
  if (scene == SCENE_ALTIN_VILLAGE) {
    if (Engine_GameFlagIsSet(FLAG_HYDROS_STATUES_DEFEATED)) {
      gAltinVillage_Placements[ACTOR_WOMAN_WATER_LUCK - ACTOR_FIRST_PLACED]
          .talkFacing = TALK_FACE_PARTY;
      gAltinVillage_Placements[ACTOR_THIRSTY_OLD_WOMAN - ACTOR_FIRST_PLACED]
          .talkFacing = TALK_FACE_PARTY;
    }
    return gAltinVillage_Placements;
  }
  if (scene == SCENE_ALTIN_HOUSES) {
    if (Engine_GameFlagIsSet(FLAG_ALTIN_MINE_FIRST_DRAIN))
      gAltinHouses_Placements[ACTOR_ROOM_ONE_SECOND_GUEST - ACTOR_FIRST_PLACED]
          .talkFacing = TALK_FACE_PARTY_AND_BACK;
    if (Engine_GameFlagIsSet(FLAG_ALTIN_MINE_SECOND_DRAIN) ||
        Engine_GameFlagIsSet(FLAG_ALTIN_MINE_THIRD_DRAIN))
      gAltinHouses_Placements[ACTOR_ROOM_TWO_SECOND_GUEST - ACTOR_FIRST_PLACED]
          .talkFacing = TALK_FACE_PARTY_AND_BACK;
    FieldScene_PrepareActors(gAltinHouses_Placements);
    return gAltinHouses_Placements;
  }
  return gAltinOther_Placements;
}

const struct SceneEvent *Scene_GetEvents(void) {
  s16 scene = gGameState.scene;

  if (scene == SCENE_ALTIN_VILLAGE) {
    return gAltinVillage_Events;
  }
  if (scene == SCENE_ALTIN_HOUSES) {
    return gAltinHouses_Events;
  }
  return gAltinOther_Events;
}

/// A bald strong man asks whether this is the party's first visit. He's in
/// Altin's village before the hydros statues are beaten; either way, he says
/// the flood has changed Altin.
static void Village_AskAboutFirstVisit(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgAltinYourFirstTimeVisitAltin);

  Engine_EventOpenMessage(ACTOR_GREETING_STRONG_MAN, 0);

  if (Engine_EventChooseYesNo(0, 0) == 1) {
    Engine_EventShowMessage(ACTOR_GREETING_STRONG_MAN, 0);
  } else {
    Event_SkipMessages(1);
    Engine_EventAskYesNo(ACTOR_GREETING_STRONG_MAN, 0);
  }

  Engine_EventEnd();
}

/// A strong man asks whether the slain hydros creatures gushed water. He's in
/// Altin's village once the hydros statues are beaten.
static void Village_AskAboutHydros(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgAltinDidSeeWaterGushingOut);
  Engine_EventAskYesNo(ACTOR_CURIOUS_STRONG_MAN, 0);
  Engine_EventEnd();
}

/// An old woman in Altin's village. While the beasts live, she complains that
/// their water cannot be drunk, and the first time the camera pans to the water
/// beast spitting at the houses. Once the statues are gone, she asks for help
/// rebuilding.
static void Village_TalkToThirstyOldWoman(void) {

  struct FieldActor *beast;
  struct FieldActor *leader;

  Engine_EventBegin();
  if (GameFlag_IsSet(FLAG_HYDROS_STATUES_DEFEATED) != 0) {
    Engine_EventSetMessage(MsgAltinOnesWhoDefeatedWaterBeasts);
    Event_AskYesNo(ACTOR_THIRSTY_OLD_WOMAN, 0);
  } else {
    Engine_ActorSetAnimation(ACTOR_THIRSTY_OLD_WOMAN, ANIM_SHAKE_HEAD);
    Engine_EventSetMessage(MsgAltinWeCantDrinkWaterMonsters);
    Event_ShowMessageAndWait(ACTOR_THIRSTY_OLD_WOMAN, 0, 10);
    if (GameFlag_IsSet(FLAG_ALTIN_WATER_BEAST_LEFT) == 0) {
      beast = Object_GetById(ACTOR_WATER_BEAST);

      Camera_GetEventObject()->motionFlags = 0;
      Camera_SetSpeed(0x10000, 0x2000);
      Camera_MoveTo(beast->x.fixed, beast->y.fixed, beast->z.fixed, 1);
      // BUG: FACING_SOUTH's value is passed as an actor number. No actor has
      // it, so the leader does not turn south toward the beast.
#ifdef BUGFIX
      Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
#else
      Actor_FaceActor(ACTOR_PARTY_LEADER, 0x4000, 0);
#endif
      Actor_FaceDirection(ACTOR_THIRSTY_OLD_WOMAN, FACING_SOUTH - FACING_STEP,
                          0);
      Engine_CameraWaitForMove();
      Engine_EventWait(120);
      leader = Object_GetById(ACTOR_PARTY_LEADER);
      Camera_MoveTo(leader->x.fixed, leader->y.fixed, leader->z.fixed, 1);
      Engine_CameraWaitForMove();
    }
    Engine_ActorSetAnimationAndWait(ACTOR_THIRSTY_OLD_WOMAN, ANIM_SHAKE_HEAD);
  }
  Engine_EventEnd();
}

/// An old man asks whether the party really found ruins deep in the mine. He's
/// in Altin's village once the hydros statues are beaten.
static void Village_AskAboutRuins(void) {

  Engine_EventBegin();
  Engine_EventSetMessage(MsgAltinTrueFoundAncientRuinsIn);
  Engine_EventAskYesNo(ACTOR_OLD_MAN_ROCK_FLUSHED_OUT, 0);
  Engine_EventEnd();
}

/// A woman asks whether the party is who the girl from Xian was looking for.
/// She's from the family that sheltered in the inn's Room One, and asks inside
/// Altin once the hydros statues are beaten.
static void Houses_AskAboutXianGirl(void) {

  Engine_EventBegin();
  Engine_EventSetMessage(MsgAltinGirlFromXianWasAsking);
  Engine_EventAskYesNo(ACTOR_ROOM_ONE_SECOND_GUEST, 0);
  Engine_EventEnd();
}

/// An old man points the party to the bottom of the mine. He's from the family
/// sheltering in the inn's Room Two, and inside Altin, before the hydros
/// statues are beaten, he asks whether the party seeks the way west.
static void Houses_AskAboutWayWest(void) {

  Engine_EventBegin();
  Engine_EventSetMessage(MsgAltinTryingFindYourWayWest);
  Engine_EventAskYesNo(ACTOR_ROOM_TWO_GUEST, 0);
  Engine_EventEnd();
}

/// A little girl thanks the leader with a kiss. She's back home inside Altin
/// before the hydros statues are beaten, and asks whether the party defeated
/// the monsters; she gives the kiss either way.
static void Houses_AskAboutMonsters(void) {

  Engine_EventBegin();
  Engine_EventSetMessage(MsgAltinDefeatedThoseMonstersDidnt);
  Engine_EventAskYesNo(ACTOR_KISSING_GIRL, 0);
  Engine_EventEnd();
}

/// A man at the inn asks whether the remaining beasts are deep below. He's
/// staying at Altin's inn once the hydros statues are beaten.
static void Houses_AskAboutMineBeasts(void) {

  Engine_EventBegin();
  Engine_EventSetMessage(MsgAltinThereFewBeastsInMine);
  Engine_EventAskYesNo(ACTOR_MAN_WAITING_TO_GO_HOME, 0);
  Engine_EventEnd();
}

/// Returns whether the party leader faces north, toward a shop counter: its
/// facing lies strictly between northwest and northeast, measured in 16 bits.
static s32 Leader_FacesCounter(void) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);

  if (ALTIN_FACES_COUNTER(leader->facing)) {
    return 1;
  }
  return 0;
}

/// Altin's weapon merchant: until the water drains he waits in the inn and
/// cannot sell; afterwards he opens his shop when spoken to across the
/// counter.
static void Houses_TalkToWeaponMerchant(void) {

  if (Engine_GameFlagIsSet(FLAG_WATER_DRAINED) == 0) {
    Engine_EventBegin();
    Engine_EventSetMessage(MsgAltinDoWantWeapons);

    Engine_EventAskYesNo(ACTOR_WEAPON_MERCHANT, 0);
    Engine_EventEnd();
    return;
  }

  if (Leader_FacesCounter() != 0) {
    Engine_ShopOpen(SHOP_ALTIN_WEAPONS, ACTOR_WEAPON_MERCHANT);
    return;
  }

  Engine_EventBegin();
  Engine_EventSetMessage(MsgAltinThankGoodnessWaterHasReceded);
  if (Engine_GameFlagIsSet(FLAG_HYDROS_STATUES_DEFEATED) != 0) {
    Engine_EventSetMessage(MsgAltinYoullHaveFindPassageIn);
  }
  Engine_EventShowMessage(ACTOR_WEAPON_MERCHANT, 0);
  Engine_EventEnd();
}

/// Altin's armor merchant, in either scene: outside, while his shop is under
/// water, he laments; once it has dried after the second drop, he sells across
/// his counter in the houses.
static void Altin_TalkToArmorMerchant(void) {

  if (Engine_GameFlagIsSet(FLAG_WATER_DROPPED_TWICE) == 0) {
    Engine_EventBegin();
    Engine_EventSetMessage(MsgAltinMyStoreSubmergedWantSell);
    Engine_EventShowMessage(ACTOR_FLOODED_ARMOR_MERCHANT, 0);
    Engine_EventEnd();
    return;
  }

  if (Leader_FacesCounter() != 0) {
    Engine_ShopOpen(SHOP_ALTIN_ARMOR, ACTOR_ARMOR_MERCHANT);
    return;
  }

  Engine_EventBegin();
  Engine_EventSetMessage(MsgAltinItsGreatCanSellArmor);
  if (Engine_GameFlagIsSet(FLAG_HYDROS_STATUES_DEFEATED) != 0) {
    Engine_EventSetMessage(MsgAltinHowAboutArentImpressedBy);
  }
  Engine_EventShowMessage(ACTOR_ARMOR_MERCHANT, 0);
  Engine_EventEnd();
}

/// Altin's item merchant, the same way: she reopens after the first drop.
static void Altin_TalkToItemMerchant(void) {
  if (Engine_GameFlagIsSet(FLAG_WATER_DROPPED_ONCE) == 0) {
    Engine_EventBegin();
    Engine_EventSetMessage(MsgAltinWillDoIfMyMerchandise);
    Engine_EventShowMessage(ACTOR_FLOODED_ITEM_MERCHANT, 0);
    Engine_EventEnd();
    return;
  }

  if (Leader_FacesCounter() != 0) {
    Engine_ShopOpen(SHOP_ALTIN_ITEMS, ACTOR_ITEM_MERCHANT);
    return;
  }

  Engine_EventBegin();
  Engine_EventSetMessage(MsgAltinNoneMyGoodsWereDamaged);
  if (Engine_GameFlagIsSet(FLAG_HYDROS_STATUES_DEFEATED) != 0) {
    Engine_EventSetMessage(MsgAltinGirlFromXianBoughtLot);
  }
  Engine_EventShowMessage(ACTOR_ITEM_MERCHANT, 0);
  Engine_EventEnd();
}

/// Altin's innkeeper opens the inn when spoken to across the counter. Spoken to
/// otherwise, the innkeeper talks about the flood or, once it is over, the
/// temple west of the mine.
static void Houses_TalkToInnkeeper(void) {
  struct FieldActor *leader;

  leader = Object_GetById(ACTOR_PARTY_LEADER);

  if (ALTIN_FACES_COUNTER(leader->facing)) {
    Engine_InnOpen(INN_ALTIN, ACTOR_INNKEEPER);
    return;
  }

  Engine_EventBegin();

  if (Engine_GameFlagIsSet(FLAG_HYDROS_STATUES_DEFEATED) != 0) {
    Engine_EventSetMessage(MsgAltinThereSmallTempleWestAltin);
    Engine_EventShowMessage(ACTOR_INNKEEPER, 0);
  } else {
    Engine_EventSetMessage(MsgAltinWeGotLittleDampBut);
    Engine_EventAskYesNo(ACTOR_INNKEEPER, 0);
  }

  Engine_EventEnd();
}

/// Walking north into one of the doors of Altin's village: everyone stops, the
/// door opens and the party leader walks in. The Sanctum's wide door is redrawn
/// in two halves by hand; the others play their opening pictures.
static void Village_EnterDoor(void) {
  u32 actorId;
  struct EventWork *event;
  struct FieldActor *actor;
  s32 doorIndex;
  u8 *doorBytes;
  s32 doorByteOffset;
  s32 coordinatesByteOffset;
  s32 column;
  s32 row;
  s32 rightColumn;

  event = gEventWork;
  Engine_EventBegin();
  for (actorId = ACTOR_FIRST_PLACED; actorId <= ACTOR_LAST_SLOT; actorId++) {
    actor = Object_GetById(actorId);
    if (actor != 0) {
      actor->motionFlags = 0;
    }
  }

  // The bytes read the entry through byte offsets and sign-extend the door
  // number by shifts; indexing the structs loads the fields differently.
  doorBytes = (u8 *)gAltinVillage_Doors;
  doorIndex =
      ((*(u16 *)&event->touchedTrigger - FIRST_DOOR_TRIGGER) << 16) >> 16;
  doorByteOffset = doorIndex << 3;
  coordinatesByteOffset = doorByteOffset + 4;
  column = *(s16 *)(doorBytes + coordinatesByteOffset);
  row = *(s16 *)(doorBytes + coordinatesByteOffset + 2);
  if (doorIndex == DOOR_SANCTUM) {
    // The Sanctum's door slides open with SE_188, as Bilibin's does.
    Event_PlayCue(SE_188);
    Map_CopyCellsTo(42, 33, column, row, 2, 2);
    rightColumn = column + 2;
    Map_CopyCellsTo(42, 35, rightColumn, row, 2, 2);
    Engine_EventWait(4);
    Map_CopyCellsTo(40, 33, column, row, 2, 2);
    Map_CopyCellsTo(40, 35, rightColumn, row, 2, 2);
    Engine_EventWait(4);
  } else {
    Event_PlayCue(SE_DOOR_OPEN);
    // The north door's row in gAltinVillage_Doors plays its opening at
    // (39, 17), outside the village's camera box; its house stands at
    // (8, 17), above where the party comes back out
    // (ALTIN_FROM_ALTIN_INTERIORS_NORTH), so that doorway is drawn open here
    // (a render with this copy made on entry shows the open doorway).
    if (doorIndex == ALTIN_TO_ALTIN_INTERIORS_NORTH - FIRST_DOOR_TRIGGER) {
      Map_CopyCellsTo(33, 42, 8, 17, 1, 2);
    }
    Map_AnimateCells(gAltinVillage_Doors[doorIndex].steps, column, row);
  }

  // Walk the leader through.
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Object_GetById(ACTOR_PARTY_LEADER)->motionFlags = 0;
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  if (doorIndex == DOOR_STRAIGHT_IN) {
    Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, 0);
  } else {
    if (doorIndex != DOOR_SANCTUM) {
      Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, -4);
    } else {
      Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 2);
      Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -4);
    }
  }
  Engine_EventWait(10);
  Engine_EventRequestExit(event->touchedTrigger);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventEnd();
}

/// Walking north into one of the doorways of Altin's village that lead into
/// the mine: the leader walks out.
static void Village_WalkThroughDoorway(void) {
  struct EventWork *work;

  work = gEventWork;
  Engine_EventBegin();
  Object_GetById(ACTOR_PARTY_LEADER)->motionFlags = 0;
  Event_PlayCue(SE_MAP_EXIT);
  Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, -16);
  Engine_EventRequestExit(work->touchedTrigger);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventEnd();
}

/// Update callback of the hopping water beast in Altin's village: once a
/// second it leaps up, falls back with a splash that shakes the screen
/// briefly, and stands while on the ground.
static void Beast_Hop(union FieldObject *fieldObject) {
  struct FieldActor *beast = &fieldObject->actor;
  s16 *shakeFrames = (s16 *)&beast->work;

  // While this hook owns the beast, timer is its hop phase, work counts down
  // the landing shake, and the effect's countdown is a landing-sound latch.
  // Read work through its corresponding signed type for the countdown test.
  if (*shakeFrames != 0) {
    if (--*shakeFrames == 1)
      Map_SetShake(-1, -1, 0xe666);
  }
  if (beast->velocityY == 0) {
    Object_SetMode(beast, 1);
    beast->y.fixed += -0x18000;
    if (beast->y.fixed < beast->terrainHeight) {
      if (fieldObject->effect.countdown != 0) {
        Event_PlayCue(SE_229);
        fieldObject->effect.countdown = 0;
        beast->work = 4;
        Map_SetShake(0, 0x10000, 0x10000);
      }
      beast->y.fixed = beast->terrainHeight;
    }
    beast->paused = 1;
  } else {
    beast->paused = 0;
  }
  if (beast->timer == 0) {
    Event_PlayCue(SE_JUMP);
    fieldObject->effect.countdown = 1;
    Object_SetMode(beast, 2);
    beast->velocityY = 0x30000;
  }
  if (++beast->timer == WATER_BEAST_HOP_PERIOD)
    beast->timer = 0;
}

/// The water beast hops away across the flood and dives in with a splash.
static void Beast_HopAway(void) {
  struct FieldActor *beast;

  beast = Object_GetById(ACTOR_WATER_BEAST);
  beast->timer = 0;
  beast->work = 0;
  beast->gravity = 0x6666;
  beast->update = Beast_Hop;
  Actor_SetSpeed(ACTOR_WATER_BEAST, 0x13333, 0x9999);
  Actor_MoveToAndWait(ACTOR_WATER_BEAST, 28, 460);
  Actor_MoveToAndWait(ACTOR_WATER_BEAST, 24, 448);
  Event_PlayCue(SE_229);
  Engine_ActorDestroy(ACTOR_WATER_BEAST);
  Map_SetShake(0, 0x10000, 0x10000);
  Engine_EventWait(4);
  Map_SetShake(-1, -1, 0xe666);
  Engine_EventWait(40);
  Engine_ActorSetAnimation(ACTOR_WATER_BEAST, ANIM_STAND);
}

/// After Move: remembers whether the pillar now stands in map column 22, the
/// spot it is pushed to.
static void Village_CheckPillarMoved(void) {
  struct FieldActor *pillar = Object_GetById(ACTOR_STONE_STATUE);

  if ((pillar->x.fixed >> 20) == 22) {
    Engine_GameFlagSet(FLAG_ALTIN_PILLAR_MOVED);
  } else {
    Engine_GameFlagClear(FLAG_ALTIN_PILLAR_MOVED);
  }
}

/// Sets up the scene being entered, at the village's water level. It works out
/// the water level from the mine's drain flags first. Returns 0.
s32 Scene_Initialize(void) {
  s16 scene;

  if (Engine_GameFlagIsSet(FLAG_ALTIN_MINE_FIRST_DRAIN) != 0) {
    Engine_GameFlagSet(FLAG_WATER_DROPPED_ONCE);
  }

  if (Engine_GameFlagIsSet(FLAG_ALTIN_MINE_SECOND_DRAIN) != 0 ||
      Engine_GameFlagIsSet(FLAG_ALTIN_MINE_THIRD_DRAIN) != 0) {
    Engine_GameFlagSet(FLAG_WATER_DROPPED_TWICE);
  }

  if (Engine_GameFlagIsSet(FLAG_ALTIN_MINE_SECOND_DRAIN) != 0 &&
      Engine_GameFlagIsSet(FLAG_ALTIN_MINE_THIRD_DRAIN) != 0) {
    Engine_GameFlagSet(FLAG_WATER_DRAINED);
  }

  scene = gGameState.scene;
  if (scene == SCENE_ALTIN_VILLAGE) {
    Village_ApplyWaterLevel();
  } else if (scene == SCENE_ALTIN_HOUSES) {
    Houses_PlaceSheltering();
  }

  return 0;
}

/// Sets up Altin's village (Altin): the map block for the current water
/// level (one of four 32 by 32 cell blocks kept on the map), the villagers and
/// merchants present at that level, the water beast with its spray while it
/// remains, and the pushed pillar. Then redraws the map around the leader.
static void Village_ApplyWaterLevel(void) {
  struct FieldActor *leader;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  // Select the water-level map and remove villagers whose areas are flooded.
  if (GameFlag_IsSet(FLAG_WATER_DRAINED) != 0) {
    Map_CopyCellsTo(64, 32, 0, 32, 32, 32);
    Map_CopyCellAttributes(64, 32, 32, 32, 0, 0);
    Engine_ActorDestroy(ACTOR_FLOODED_ARMOR_MERCHANT);
    Engine_ActorDestroy(ACTOR_FLOODED_ITEM_MERCHANT);
  } else if (GameFlag_IsSet(FLAG_WATER_DROPPED_TWICE) != 0) {
    Map_CopyCellsTo(64, 0, 0, 32, 32, 32);
    Map_CopyCellAttributes(64, 0, 32, 32, 0, 0);
    Engine_ActorDestroy(ACTOR_OLD_MAN_ROCK_FLUSHED_OUT);
    Engine_ActorDestroy(ACTOR_FLOODED_ARMOR_MERCHANT);
    Engine_ActorDestroy(ACTOR_FLOODED_ITEM_MERCHANT);
  } else if (GameFlag_IsSet(FLAG_WATER_DROPPED_ONCE) != 0) {
    Map_CopyCellsTo(0, 64, 0, 32, 32, 32);
    Map_CopyCellAttributes(0, 64, 32, 32, 0, 0);
    Engine_ActorDestroy(ACTOR_STRONG_MAN_GUARDIAN_STATUES);
    Engine_ActorDestroy(ACTOR_OLD_MAN_ROCK_FLUSHED_OUT);
    Engine_ActorDestroy(ACTOR_FLOODED_ITEM_MERCHANT);
  } else {
    Map_CopyCellAttributes(0, 32, 32, 32, 0, 0);
    Engine_ActorDestroy(ACTOR_MAN_WATERS_SUBSIDED);
    Engine_ActorDestroy(ACTOR_STRONG_MAN_GUARDIAN_STATUES);
    Engine_ActorDestroy(ACTOR_OLD_MAN_ROCK_FLUSHED_OUT);
  }
  // Restore the remaining beast spitting water, with its sound (SE_170) and
  // its spray.
  if (GameFlag_IsSet(FLAG_ALTIN_WATER_BEAST_LEFT) != 0) {
    Engine_ActorDestroy(ACTOR_WATER_BEAST);
  } else {
    FieldFx_SetQueuedSoundAndPlay(SE_170);
    Actor_SetChildValue(ACTOR_WATER_BEAST, SPRITE_COLOR_RBG);
    Engine_ActorSetAnimation(ACTOR_WATER_BEAST, WATER_BEAST_ANIM_SPIT);
    FieldScheduler_AddCallback(Beast_SpawnSpray, TASK_PRIORITY_SCENE);
  }
  if (gGameState.entrance == ALTIN_FROM_SANCTUM) {
    GameFlag_Clear(FLAG_SHOW_LOCATION_NAME);
  }
  Map_CopyCellAttributes(0, 33, 4, 3, 20, 41);
  if (GameFlag_IsSet(FLAG_ALTIN_PILLAR_MOVED) != 0) {
    Actor_SetPosition(ACTOR_STONE_STATUE, PIXELS(360), PIXELS(168));
  }
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_STONE_STATUE), 0);
  // Hide the three crates, which stay solid, and draw them under the actors.
  Actor_SetChildValue(ACTOR_FIRST_CRATE, SPRITE_COLOR_HIDDEN);
  Engine_ActorSetChildValue(ACTOR_SECOND_CRATE, SPRITE_COLOR_HIDDEN);
  Actor_SetChildValue(ACTOR_THIRD_CRATE, SPRITE_COLOR_HIDDEN);
  Object_GetById(ACTOR_FIRST_CRATE)->collisionFlags |= ACTOR_NOT_TARGETED;
  Object_GetById(ACTOR_SECOND_CRATE)->collisionFlags |= ACTOR_NOT_TARGETED;
  Object_GetById(ACTOR_THIRD_CRATE)->collisionFlags |= ACTOR_NOT_TARGETED;
  Object_GetById(ACTOR_FIRST_CRATE)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  Object_GetById(ACTOR_SECOND_CRATE)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  Object_GetById(ACTOR_THIRD_CRATE)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  Engine_ActorSetSpritePriority(ACTOR_FIRST_CRATE, 1);
  Engine_ActorSetSpritePriority(ACTOR_SECOND_CRATE, 1);
  Engine_ActorSetSpritePriority(ACTOR_THIRD_CRATE, 1);
  // Redraw at the leader after the map and object state are ready.
  Engine_TaskWait(1);
  Engine_EventBegin();
  Camera_MoveTo(leader->x.fixed, leader->y.fixed, leader->z.fixed, 0);
  Engine_MapRedraw();
  Engine_EventEnd();
  Engine_TaskWait(1);
}

/// Sets up inside Altin's buildings (AltinInteriors): the families still
/// sheltering in the inn's rooms, the weapon merchant waiting there until the
/// water drains, and the shopkeepers kept from wandering.
static void Houses_PlaceSheltering(void) {
  struct FieldActor *actor;

  if (GameFlag_IsSet(FLAG_WATER_DROPPED_ONCE) == 0) {
    Actor_SetPosition(ACTOR_ROOM_ONE_GUEST, PIXELS(808), PIXELS(727));
    Object_GetById(ACTOR_ROOM_ONE_GUEST)->facing = FACING_SOUTH - FACING_STEP;
    Actor_SetPosition(ACTOR_ROOM_ONE_SECOND_GUEST, PIXELS(794), PIXELS(825));
  }
  if (GameFlag_IsSet(FLAG_WATER_DROPPED_TWICE) == 0) {
    Actor_SetPosition(ACTOR_ROOM_TWO_GUEST, PIXELS(560), PIXELS(710));
    Object_GetById(ACTOR_ROOM_TWO_GUEST)->facing = FACING_EAST + FACING_STEP;
    Actor_SetPosition(ACTOR_ROOM_TWO_SECOND_GUEST, PIXELS(576), PIXELS(710));
  }
  if (GameFlag_IsSet(FLAG_WATER_DRAINED) == 0) {
    Actor_SetPosition(ACTOR_WEAPON_MERCHANT, PIXELS(295), PIXELS(744));
    Object_GetById(ACTOR_WEAPON_MERCHANT)->facing =
        FACING_NORTHWEST + FACING_STEP;
  } else {
    actor = Object_GetById(ACTOR_WEAPON_MERCHANT);
    actor->collisionFlags |= ACTOR_NEAR_TALK;
  }
  actor = Object_GetById(ACTOR_ARMOR_MERCHANT);
  if (actor != 0) {
    actor->collisionFlags |= ACTOR_NEAR_TALK;
  }
  actor = Object_GetById(ACTOR_ITEM_MERCHANT);
  if (actor != 0) {
    actor->collisionFlags |= ACTOR_NEAR_TALK;
  }
}

/// Gives a new spray particle its look: SPRITE_DUST_PUFF standing still, drawn
/// with its red and blue swapped so the dust is water, on sprite priority 1
/// and at 0.3 scale. Spray_Drift turns some white as they fade.
static void Spray_Initialize(struct FieldActor *spray) {
  spray->motionFlags = 0;
  spray->sprite->image.priority = 1;
  ObjectGroup_SetChildValue(spray, SPRITE_COLOR_BGR);
  Engine_ActorSetSpriteFlags(spray, 0);
  spray->scaleX = WATER_SPRAY_START_SCALE;
  spray->scaleY = WATER_SPRAY_START_SCALE;
}

/// Update callback of a spray particle: it drifts and grows with a random
/// wobble, then curls upward and flattens as its movement timer runs down.
/// The separate lifetime releases it after 20 updates.
static void Spray_Drift(union FieldObject *fieldObject) {
  struct FieldActor *spray = &fieldObject->actor;
  spray->x.fixed += WATER_SPRAY_LATERAL_STEP(
      spray->timer, (s16)((s32)(((u32)Engine_RandomNext() * 2) >> 16) - 1));
  if (spray->timer <= 3) {
    spray->z.fixed += -(((u32)Engine_RandomNext() * 0x8000) >> 16) - 0x10000;
    spray->scaleX += 0x2666;
    spray->scaleY += -0xa3d;
  } else {
    spray->z.fixed += 0x20000;
    spray->scaleX += 0x7ae;
    spray->scaleY += 0x7ae;
  }
  if (((u32)Engine_RandomNext() * spray->timer) >> 16 == 0)
    ObjectGroup_SetChildValue(spray, SPRITE_COLOR_WHITE);
  if (spray->timer != 0)
    spray->timer--;
  else
    spray->timer = (((u32)Engine_RandomNext() * 5) >> 16) * 2 + 2;
  if (--fieldObject->effect.countdown == 0) {
    spray->update = 0;
    Engine_ObjectDispatchRelease(spray);
  }
}

/// Scheduler task while the water beast spits: every fourth frame a spray
/// particle starts from its spout, living 20 frames.
static void Beast_SpawnSpray(void) {
  union FieldObject *spray;
  if ((gFrameCount & 3) == 0) {
    spray = ((union FieldObject *)Object_Create(SPRITE_DUST_PUFF, PIXELS(64), 0,
                                                PIXELS(400)));
    if (spray != 0) {
      spray->actor.timer = WATER_SPRAY_LIFETIME;
      spray->actor.work = 0;
      spray->effect.countdown = WATER_SPRAY_LIFETIME;
      Spray_Initialize(&spray->actor);
      spray->actor.update = Spray_Drift;
      Object_SetMode(&spray->actor, 1);
    }
  }
}

/// Stepping near the water beast in Altin's village: the camera pans to it, it
/// stops spitting, turns to the party, cries out and hops away into the flood.
static void Village_ScareOffBeast(void) {
  Engine_EventBegin();
  Camera_SetSpeed(0x6666, 0xccc);
  Camera_MoveTo(PIXELS(63), -1, PIXELS(450), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(30);
  Engine_ActorSetAnimation(ACTOR_WATER_BEAST, ANIM_STAND);
  // The spitting stops: its sound and its spray end.
  FieldFx_SetQueuedSoundAndPlay(-1);
  Engine_TaskRemoveCallback(Beast_SpawnSpray);
  Engine_EventWait(20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_WATER_BEAST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_WATER_BEAST, FACING_EAST, 20);
  Actor_FaceDirection(ACTOR_WATER_BEAST, FACING_NORTH + FACING_STEP, 40);
  Event_PlayCue(SE_147);
  Engine_ActorRunRepeatedMotion(ACTOR_WATER_BEAST, 2);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_WATER_BEAST, FACING_NORTHWEST + FACING_STEP, 40);
  Beast_HopAway();
  Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 1);
  Engine_CameraWaitForMove();
  Engine_ActorSetAnimationAndWait(ACTOR_THIRSTY_OLD_WOMAN, ANIM_SHAKE_HEAD);
  GameFlag_Set(FLAG_ALTIN_WATER_BEAST_LEFT);
  Engine_EventEnd();
}

/// The cook from Xian paces between his pots, nodding over each.
s32 gAltinHouses_CookScript[] = {
    SC_ADD(SW_COLLISION, 128),
    SC_SET(SW_SPEED, 0x26666),
    SC_SET(SW_ACCEL, 0x13333),
    SC_LABEL(1),
    SC_ARG(ANIM_WALK),
    SC_MOVE(PIXELS(342), 0, PIXELS(746)),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_NORTHWEST + FACING_STEP),
    SC_ARG(ANIM_NOD),
    SC_WAIT(30),
    SC_ARG(ANIM_WALK),
    SC_MOVE(PIXELS(356), 0, PIXELS(746)),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_NORTH + FACING_STEP),
    SC_ARG(ANIM_NOD),
    SC_WAIT(50),
    SC_ARG(ANIM_WALK),
    SC_MOVE(PIXELS(342), 0, PIXELS(746)),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_NORTHWEST + FACING_STEP),
    SC_ARG(ANIM_NOD),
    SC_WAIT(20),
    SC_ARG(ANIM_WALK),
    SC_MOVE(PIXELS(ALTIN_COOK_LAST_POT_X), 0, PIXELS(746)),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_NORTH + FACING_STEP),
    SC_ARG(ANIM_NOD),
    SC_WAIT(30),
    SC_SET(SW_FACING, FACING_EAST + FACING_STEP),
    SC_WAIT(90),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
};

struct SceneEntrance gAltinOther_Entrances[] = {
    {0, CONDITION_ALWAYS, 380, 0, 433, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gAltinVillage_Entrances[] = {
    {0, CONDITION_ALWAYS, 380, 0, 433, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {ALTIN_FROM_WORLD_MAP, CONDITION_ALWAYS, 488, 0, 184, FACING_WEST, 0, -1,
     -1, -1, -1, 0},
    {ALTIN_FROM_ALTIN_INTERIORS_SOUTHWEST, CONDITION_ALWAYS, 168, 0, 216,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {ALTIN_FROM_SANCTUM, CONDITION_ALWAYS, 336, 0, 200, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {ALTIN_FROM_ALTIN_INTERIORS_NORTHWEST, CONDITION_ALWAYS, 424, 0, 344,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {ALTIN_FROM_ALTIN_INTERIORS_NORTH, CONDITION_ALWAYS, 136, 0, 312,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {ALTIN_FROM_ALTIN_INTERIORS_NORTHEAST, CONDITION_ALWAYS, 280, 0, 408,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {ALTIN_FROM_ALTIN_INTERIORS_SOUTHEAST, CONDITION_ALWAYS, 456, 0, 224,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {ALTIN_FROM_ALTIN_INTERIORS_EAST, CONDITION_ALWAYS, 184, 0, 408,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {ALTIN_FROM_ALTIN_INTERIORS_WEST, CONDITION_ALWAYS, 168, 0, 440,
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

struct SceneEntrance gAltinHouses_Entrances[] = {
    {0, CONDITION_ALWAYS, 200, 0, 200, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {ALTIN_INTERIORS_FROM_ALTIN_NORTHWEST, CONDITION_ALWAYS, 159, 0, 258,
     FACING_NORTH, 0, 25, 16, 255, 300, 0},
    {ALTIN_INTERIORS_FROM_ALTIN_NORTH, CONDITION_ALWAYS, 336, 0, 213,
     FACING_NORTH, 0, 268, 40, 500, 250, 0},
    {ALTIN_INTERIORS_FROM_ALTIN_NORTHEAST, CONDITION_ALWAYS, 688, 0, 204,
     FACING_NORTH, 0, 542, 40, 768, 250, 0},
    {ALTIN_INTERIORS_FROM_ALTIN_WEST, CONDITION_ALWAYS, 240, 0, 500,
     FACING_NORTH, 0, 110, 336, 350, 540, 0},
    {ALTIN_INTERIORS_FROM_ALTIN_SOUTHEAST, CONDITION_ALWAYS, 496, 0, 504,
     FACING_NORTH, 0, 360, 336, 600, 540, 0},
    {ALTIN_INTERIORS_FROM_ALTIN_EAST, CONDITION_ALWAYS, 752, 0, 516,
     FACING_NORTH, 0, 635, 280, 845, 556, 0},
    {ALTIN_INTERIORS_FROM_ALTIN_SOUTHWEST, CONDITION_ALWAYS, 80, 0, 864,
     FACING_NORTH, 0, 16, 638, 432, 912, 0},
    {ALTIN_FROM_ALTIN_INTERIORS_EAST, CONDITION_ALWAYS, 68, 0, 728,
     FACING_SOUTH, 0, 16, 638, 432, 912, 0},
    {ALTIN_FROM_ALTIN_INTERIORS_WEST, CONDITION_ALWAYS, 690, 0, 732,
     FACING_SOUTH, 0, 480, 626, 896, 882, 0},
    {SCENE_TABLE_END},
};

u32 gAltin_Exits[] = {
    SCENE_EXITS(Altin),
    SCENE_EXIT(ALTIN_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_ALTIN),
    SCENE_EXIT(ALTIN_TO_ALTIN_INTERIORS_SOUTHWEST, AltinInteriors,
               ALTIN_INTERIORS_FROM_ALTIN_SOUTHWEST),
    SCENE_EXIT(ALTIN_TO_SANCTUM, Sanctum, SANCTUM_FROM_ALTIN),
    SCENE_EXIT(ALTIN_TO_ALTIN_INTERIORS_NORTHWEST, AltinInteriors,
               ALTIN_INTERIORS_FROM_ALTIN_NORTHWEST),
    SCENE_EXIT(ALTIN_TO_ALTIN_INTERIORS_NORTH, AltinInteriors,
               ALTIN_INTERIORS_FROM_ALTIN_NORTH),
    SCENE_EXIT(ALTIN_TO_ALTIN_INTERIORS_NORTHEAST, AltinInteriors,
               ALTIN_INTERIORS_FROM_ALTIN_NORTHEAST),
    SCENE_EXIT(ALTIN_TO_ALTIN_INTERIORS_SOUTHEAST, AltinInteriors,
               ALTIN_INTERIORS_FROM_ALTIN_SOUTHEAST),
    SCENE_EXIT(ALTIN_TO_ALTIN_INTERIORS_EAST, AltinInteriors,
               ALTIN_INTERIORS_FROM_ALTIN_EAST),
    SCENE_EXIT(ALTIN_TO_ALTIN_INTERIORS_WEST, AltinInteriors,
               ALTIN_INTERIORS_FROM_ALTIN_WEST),
    SCENE_EXIT(ALTIN_TO_ALTIN_PEAK_FIRST_WATER_BEAST, AltinPeakFirstWaterBeast,
               ALTIN_PEAK_FIRST_WATER_BEAST_FROM_ALTIN),
    SCENE_EXIT(ALTIN_TO_ALTIN_PEAK_SOUTH_ENTRANCE, AltinPeakSouthEntrance,
               ALTIN_PEAK_SOUTH_ENTRANCE_FROM_ALTIN),
    SCENE_EXIT(ALTIN_TO_ALTIN_PEAK_JUNCTION, AltinPeakJunction,
               ALTIN_PEAK_JUNCTION_FROM_ALTIN),
    SCENE_EXIT(ALTIN_TO_ALTIN_PEAK_SPRITZ, AltinPeakSpritz,
               ALTIN_PEAK_SPRITZ_FROM_ALTIN),
    SCENE_EXITS(AltinInteriors),
    SCENE_EXIT(ALTIN_INTERIORS_TO_ALTIN_NORTHWEST, Altin,
               ALTIN_FROM_ALTIN_INTERIORS_NORTHWEST),
    SCENE_EXIT(ALTIN_INTERIORS_TO_ALTIN_NORTH, Altin,
               ALTIN_FROM_ALTIN_INTERIORS_NORTH),
    SCENE_EXIT(ALTIN_INTERIORS_TO_ALTIN_NORTHEAST, Altin,
               ALTIN_FROM_ALTIN_INTERIORS_NORTHEAST),
    SCENE_EXIT(ALTIN_INTERIORS_TO_ALTIN_WEST, Altin,
               ALTIN_FROM_ALTIN_INTERIORS_WEST),
    SCENE_EXIT(ALTIN_INTERIORS_TO_ALTIN_SOUTHEAST, Altin,
               ALTIN_FROM_ALTIN_INTERIORS_SOUTHEAST),
    SCENE_EXIT(ALTIN_INTERIORS_TO_ALTIN_EAST, Altin,
               ALTIN_FROM_ALTIN_INTERIORS_EAST),
    SCENE_EXIT(ALTIN_INTERIORS_TO_ALTIN_SOUTHWEST, Altin,
               ALTIN_FROM_ALTIN_INTERIORS_SOUTHWEST),
    SCENE_EXIT(ALTIN_INTERIORS_INN_TO_INN_UPSTAIRS, AltinInteriors,
               ALTIN_INTERIORS_INN_UPSTAIRS_FROM_INN),
    SCENE_EXIT(ALTIN_INTERIORS_INN_UPSTAIRS_TO_INN, AltinInteriors,
               ALTIN_INTERIORS_INN_FROM_INN_UPSTAIRS),
    SCENE_EXITS_END,
};

struct ScenePlacement gAltinOther_Placements[] = {
    {SCENE_TABLE_END},
};

struct ScenePlacement gAltinVillage_Placements[] = {
    {SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(440), 0,
     PIXELS(200), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0},
    {SPRITE_STRONGMAN, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(408), 0,
     PIXELS(216), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0},
    {SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(310), 0,
     PIXELS(217), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0},
    {SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, ACTOR_WANDER,
     PIXELS(246), 0, PIXELS(186), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY,
     0},
    {SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, ACTOR_WANDER_NEAR, PIXELS(58),
     0, PIXELS(206), FACING_EAST + FACING_STEP, TALK_FACE_PARTY, 0},
    {SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(104), 0,
     PIXELS(286), FACING_EAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0},
    {SPRITE_OLD_WOMAN, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(41), 0,
     PIXELS(441), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0},
    {SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
     PIXELS(176), 0, PIXELS(296), FACING_SOUTHEAST + FACING_STEP,
     TALK_FACE_PARTY, 0},
    {SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(136), 0,
     PIXELS(461), FACING_EAST + FACING_STEP, TALK_FACE_PARTY, 0},
    {SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, ACTOR_WANDER, PIXELS(350), 0,
     PIXELS(286), FACING_NORTH, TALK_FACE_PARTY, 0},
    {SPRITE_WATER_BEAST, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(60), 0,
     PIXELS(474), FACING_SOUTHEAST, TALK_FACE_PARTY, 0},
    {SPRITE_STONE_STATUE, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(344), 0,
     PIXELS(168), FACING_SOUTH, TALK_FACE_PARTY, 0},
    {SPRITE_YOUNG_MAN_IN_GREEN, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(88), 0,
     PIXELS(397), FACING_EAST + FACING_STEP, TALK_FACE_PARTY, 0},
    {SPRITE_WOMAN_WITH_PONYTAIL, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(422), 0,
     PIXELS(208), FACING_EAST + FACING_STEP, TALK_FACE_PARTY, 0},
    {SPRITE_CRATE, CONDITION_ALWAYS, ACTOR_RESET_ACTION, PIXELS(452), 0,
     PIXELS(216), FACING_SOUTH, TALK_KEEP_FACING, 0},
    {SPRITE_CRATE, CONDITION_ALWAYS, ACTOR_RESET_ACTION, PIXELS(332), 0,
     PIXELS(360), FACING_SOUTH, TALK_KEEP_FACING,
     PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_CRATE, CONDITION_ALWAYS, ACTOR_RESET_ACTION, PIXELS(332), 0,
     PIXELS(396), FACING_SOUTH, TALK_KEEP_FACING,
     PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SCENE_TABLE_END},
};

// clang-format off
struct ScenePlacement gAltinHouses_Placements[] = {
    {SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(104), 0, PIXELS(120), FACING_EAST + FACING_STEP,
        TALK_FACE_PARTY, 0},
    {SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(105), 0, PIXELS(233), FACING_SOUTH + FACING_STEP,
        TALK_FACE_PARTY, 0},
    {SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(456), 0, PIXELS(182), FACING_WEST + FACING_STEP, TALK_FACE_PARTY_THEN_WATCH,
        0},
    {SPRITE_OLD_WOMAN, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(376), 0, PIXELS(120),
        FACING_SOUTHWEST + FACING_STEP, TALK_FACE_PARTY, 0},
    {SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, 0x0, ACTOR_STAND, PIXELS(690), 0, PIXELS(128), FACING_SOUTHWEST + FACING_STEP,
        TALK_FACE_PARTY_AND_BACK, 0},
    {SPRITE_WOMAN_WITH_FAIR_HAIR, 0x0, ACTOR_STAND, PIXELS(669), 0, PIXELS(128), FACING_EAST + FACING_STEP,
        TALK_FACE_PARTY_AND_BACK, 0},
    {SPRITE_GIRL_WITH_PIGTAILS, 0x0, ACTOR_STAND, PIXELS(604), 0, PIXELS(145), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_THEN_WATCH, 0},
    {SPRITE_BALD_MAN_IN_RED, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(263), 0, PIXELS(436), FACING_SOUTH + FACING_STEP,
        TALK_FACE_PARTY, 0},
    {SPRITE_WOMAN_WITH_PONYTAIL, 0x0, ACTOR_STAND, PIXELS(520), 0, PIXELS(454), FACING_SOUTH + FACING_STEP,
        TALK_FACE_PARTY, 0},
    {SPRITE_YOUNG_MAN_IN_GREEN, 0x0, ACTOR_STAND, PIXELS(743), 0, PIXELS(454), FACING_SOUTHEAST + FACING_STEP,
        TALK_FACE_PARTY, 0},
    {SPRITE_MAN_WITH_DARK_HAIR, 0x0, ACTOR_STAND, PIXELS(103), 0, PIXELS(744), FACING_SOUTHEAST + FACING_STEP,
        TALK_FACE_PARTY, 0},
    {SPRITE_WOMAN_IN_PINK_APRON, 0x0, ACTOR_WANDER_NEAR, PIXELS(228), 0, PIXELS(756), FACING_SOUTHWEST + FACING_STEP, TALK_FACE_PARTY, 0},
    {SPRITE_COOK, 0x0, (s32)gAltinHouses_CookScript, PIXELS(350), 0, PIXELS(746),
        FACING_SOUTHWEST + FACING_STEP, TALK_FACE_PARTY, 0},
    {SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, 0x0, ACTOR_STAND, PIXELS(646), 0, PIXELS(734), FACING_SOUTHEAST + FACING_STEP,
        TALK_FACE_PARTY, 0},
    {SCENE_TABLE_END},
};

struct SceneEvent gAltinOther_Events[] = {
    {SCENE_EVENTS_END},
};

struct SceneEvent gAltinVillage_Events[] = {
    {TOUCH_FACING(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(Village_WalkThroughDoorway)},
    {TOUCH_FACING(FACING_NORTH), 11, CONDITION_ALWAYS, EVENT_SCRIPT(Village_WalkThroughDoorway)},
    {TOUCH_FACING(FACING_NORTH), 12, CONDITION_ALWAYS, EVENT_SCRIPT(Village_WalkThroughDoorway)},
    {EVENT_EXIT, 13, CONDITION_ALWAYS, ALTIN_TO_ALTIN_PEAK_SPRITZ},
    {EVENT_EXIT, 1, CONDITION_ALWAYS, ALTIN_TO_WORLD_MAP},
    {TOUCH_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS,
        EVENT_SCRIPT(Village_EnterDoor)},
    {TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS,
        EVENT_SCRIPT(Village_EnterDoor)},
    {TOUCH_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS,
        EVENT_SCRIPT(Village_EnterDoor)},
    {TOUCH_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS,
        EVENT_SCRIPT(Village_EnterDoor)},
    {TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS,
        EVENT_SCRIPT(Village_EnterDoor)},
    {TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS,
        EVENT_SCRIPT(Village_EnterDoor)},
    {TOUCH_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS,
        EVENT_SCRIPT(Village_EnterDoor)},
    {TOUCH_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS,
        EVENT_SCRIPT(Village_EnterDoor)},
    {EVENT_TALK, ACTOR_GREETING_STRONG_MAN, FLAG_HYDROS_STATUES_DEFEATED, EVENT_SCRIPT(Village_AskAboutFirstVisit)},
    {EVENT_TALK, ACTOR_GREETING_STRONG_MAN, CONDITION_ALWAYS, MSGNO(MsgAltinThankYouSoMuchForDefeating)},
    {EVENT_TALK, ACTOR_CURIOUS_STRONG_MAN, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinHowCanThoseCreaturesSpitUp)},
    {EVENT_TALK, ACTOR_CURIOUS_STRONG_MAN, CONDITION_ALWAYS, EVENT_SCRIPT(Village_AskAboutHydros)},
    {EVENT_TALK, ACTOR_WOMAN_COATED_HOUSE, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinHowHorribleMyHouseIsTotally)},
    {EVENT_TALK, ACTOR_WOMAN_COATED_HOUSE, CONDITION_ALWAYS, MSGNO(MsgAltinOnceTheMineOpensAgainWell)},
    {EVENT_TALK, ACTOR_MAN_SUBMERGED_MINE, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinEvenTheMineIsSubmergedNo)},
    {EVENT_TALK, ACTOR_MAN_SUBMERGED_MINE, CONDITION_ALWAYS, MSGNO(MsgAltinTheyAlreadyStartedMiningTheUpper)},
    {EVENT_TALK, ACTOR_OLD_MAN_DESTROY_MONSTERS, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinIfWeCouldJustDestroyThe)},
    {EVENT_TALK, ACTOR_OLD_MAN_DESTROY_MONSTERS, CONDITION_ALWAYS, MSGNO(MsgAltinTheWaterDrainedOutOfThe)},
    {EVENT_TALK, ACTOR_WOMAN_WATER_LUCK, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinAltinAlwaysUsedToRunShort)},
    {EVENT_TALK, ACTOR_WOMAN_WATER_LUCK, CONDITION_ALWAYS, MSGNO(MsgAltinThankGoodnessTheVillageIsFree)},
    {EVENT_TALK, ACTOR_THIRSTY_OLD_WOMAN, CONDITION_ALWAYS, EVENT_SCRIPT(Village_TalkToThirstyOldWoman)},
    {EVENT_TALK, ACTOR_MAN_WATERS_SUBSIDED, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinTheWatersAroundHereHaveSubsided)},
    {EVENT_TALK, ACTOR_MAN_WATERS_SUBSIDED, CONDITION_ALWAYS, MSGNO(MsgAltinTheVillageWentRightBackTo)},
    {EVENT_TALK, ACTOR_STRONG_MAN_GUARDIAN_STATUES, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinTheWaterBeastsLookedExactlyLike)},
    {EVENT_TALK, ACTOR_STRONG_MAN_GUARDIAN_STATUES, CONDITION_ALWAYS, MSGNO(MsgAltinSoThoseHydrosCreaturesReallyWere)},
    {EVENT_TALK, ACTOR_OLD_MAN_ROCK_FLUSHED_OUT, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinTheWatersHaveFlushedOutTons)},
    {EVENT_TALK, ACTOR_OLD_MAN_ROCK_FLUSHED_OUT, CONDITION_ALWAYS, EVENT_SCRIPT(Village_AskAboutRuins)},
    {EVENT_TALK, ACTOR_FLOODED_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(Altin_TalkToArmorMerchant)},
    {EVENT_TALK, ACTOR_FLOODED_ITEM_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(Altin_TalkToItemMerchant)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GREETING_STRONG_MAN, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinYouCameToAltinAtA)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GREETING_STRONG_MAN, CONDITION_ALWAYS,
        MSGNO(MsgAltinWeCanGoBackToWork)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CURIOUS_STRONG_MAN, FLAG_HYDROS_STATUES_DEFEATED,
        MSGNO(MsgAltinItsAlmostLikeTheyreTurningThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CURIOUS_STRONG_MAN, CONDITION_ALWAYS,
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
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRONG_MAN_GUARDIAN_STATUES, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinComeToThinkOfItIt)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRONG_MAN_GUARDIAN_STATUES, CONDITION_ALWAYS,
        MSGNO(MsgAltinTheOldGuardianStatuesAlwaysCreeped)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ROCK_FLUSHED_OUT, FLAG_HYDROS_STATUES_DEFEATED,
        MSGNO(MsgAltinItllBeBetterThanDiggingUp)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ROCK_FLUSHED_OUT, CONDITION_ALWAYS,
        MSGNO(MsgAltinIUsedToFindAllSorts)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FLOODED_ARMOR_MERCHANT, CONDITION_ALWAYS,
        MSGNO(MsgAltinAllMyArmorInventoryIsRusting)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FLOODED_ITEM_MERCHANT, CONDITION_ALWAYS,
        MSGNO(MsgAltinIfMyGoodsAreWetIll)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STONE_STATUE, CONDITION_ALWAYS, EVENT_SCRIPT(Village_CheckPillarMoved)},
    {EVENT_TOUCH, TRIGGER_NEAR_BEAST, FLAG_ALTIN_WATER_BEAST_LEFT, EVENT_SCRIPT(Village_ScareOffBeast)},
    {SCENE_EVENTS_END},
};

struct SceneEvent gAltinHouses_Events[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS, ALTIN_INTERIORS_TO_ALTIN_NORTHWEST},
    {EVENT_EXIT, 2, CONDITION_ALWAYS, ALTIN_INTERIORS_TO_ALTIN_NORTH},
    {EVENT_EXIT, 3, CONDITION_ALWAYS, ALTIN_INTERIORS_TO_ALTIN_NORTHEAST},
    {EVENT_EXIT, 4, CONDITION_ALWAYS, ALTIN_INTERIORS_TO_ALTIN_WEST},
    {EVENT_EXIT, 5, CONDITION_ALWAYS, ALTIN_INTERIORS_TO_ALTIN_SOUTHEAST},
    {EVENT_EXIT, 6, CONDITION_ALWAYS, ALTIN_INTERIORS_TO_ALTIN_EAST},
    {EVENT_EXIT, 7, CONDITION_ALWAYS, ALTIN_INTERIORS_TO_ALTIN_SOUTHWEST},
    {EVENT_EXIT, 8, CONDITION_ALWAYS, ALTIN_INTERIORS_INN_TO_INN_UPSTAIRS},
    {EVENT_EXIT, 9, CONDITION_ALWAYS, ALTIN_INTERIORS_INN_UPSTAIRS_TO_INN},
    {EVENT_TALK, ACTOR_ROOM_ONE_GUEST, FLAG_WATER_DROPPED_ONCE, MSGNO(MsgAltinNoMatterWhatHappensIdNever)},
    {EVENT_TALK, ACTOR_ROOM_ONE_GUEST, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinOhNoTheWholeHouseIs)},
    {EVENT_TALK, ACTOR_ROOM_ONE_GUEST, CONDITION_ALWAYS, MSGNO(MsgAltinIHearThatSilkRoadIs)},
    {EVENT_TALK, ACTOR_ROOM_ONE_SECOND_GUEST, FLAG_WATER_DROPPED_ONCE, MSGNO(MsgAltinOfCourseOurHouseWillBe)},
    {EVENT_TALK, ACTOR_ROOM_ONE_SECOND_GUEST, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinIfTheMonsterThatDidThis)},
    {EVENT_TALK, ACTOR_ROOM_ONE_SECOND_GUEST, CONDITION_ALWAYS, EVENT_SCRIPT(Houses_AskAboutXianGirl)},
    {EVENT_TALK, ACTOR_ROOM_TWO_GUEST, FLAG_WATER_DROPPED_TWICE, MSGNO(MsgAltinNotManyPeopleComeUpSilk)},
    {EVENT_TALK, ACTOR_ROOM_TWO_GUEST, FLAG_HYDROS_STATUES_DEFEATED, EVENT_SCRIPT(Houses_AskAboutWayWest)},
    {EVENT_TALK, ACTOR_ROOM_TWO_GUEST, CONDITION_ALWAYS, MSGNO(MsgAltinAncientRuinsAndTreasureDefinitelyGo)},
    {EVENT_TALK, ACTOR_ROOM_TWO_SECOND_GUEST, FLAG_WATER_DROPPED_TWICE, MSGNO(MsgAltinItsBeenALongTimeSince)},
    {EVENT_TALK, ACTOR_ROOM_TWO_SECOND_GUEST, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinDontBeTooHarshOnThose)},
    {EVENT_TALK, ACTOR_ROOM_TWO_SECOND_GUEST, CONDITION_ALWAYS, MSGNO(MsgAltinIfOnlyTheyHadLeftA)},
    {EVENT_TALK, ACTOR_MAN_BACK_IN_HIS_HOMETOWN, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinIDidntWantToLeaveMy)},
    {EVENT_TALK, ACTOR_MAN_BACK_IN_HIS_HOMETOWN, CONDITION_ALWAYS, MSGNO(MsgAltinReallyAncientRuinsAtTheBottom)},
    {EVENT_TALK, ACTOR_WOMAN_NO_PLACE_LIKE_HOME, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinIHeardSomeoneKilledTheWater)},
    {EVENT_TALK, ACTOR_WOMAN_NO_PLACE_LIKE_HOME, CONDITION_ALWAYS, MSGNO(MsgAltinThereProbablyIsntMuchTreasureIn)},
    {EVENT_TALK, ACTOR_KISSING_GIRL, FLAG_HYDROS_STATUES_DEFEATED, EVENT_SCRIPT(Houses_AskAboutMonsters)},
    {EVENT_TALK, ACTOR_KISSING_GIRL, CONDITION_ALWAYS, MSGNO(MsgAltinSomethingsWrongWithMomAndPop)},
    {EVENT_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(Houses_TalkToWeaponMerchant)},
    {EVENT_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(Altin_TalkToArmorMerchant)},
    {EVENT_TALK, ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(Altin_TalkToItemMerchant)},
    {EVENT_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(Houses_TalkToInnkeeper)},
    {EVENT_TALK, ACTOR_WOMAN_FORMER_GLORY, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinIdGiveAnythingForSomeoneTo)},
    {EVENT_TALK, ACTOR_WOMAN_FORMER_GLORY, CONDITION_ALWAYS, MSGNO(MsgAltinThereIsAnEndlessDesertWest)},
    {EVENT_TALK, ACTOR_COOK, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinICameFromXianICook)},
    {EVENT_TALK, ACTOR_COOK, CONDITION_ALWAYS, MSGNO(MsgAltinWestOfTheMountainsIsVery)},
    {EVENT_TALK, ACTOR_MAN_WAITING_TO_GO_HOME, FLAG_WATER_DROPPED_ONCE, MSGNO(MsgAltinNoneOfUsCanGoHome)},
    {EVENT_TALK, ACTOR_MAN_WAITING_TO_GO_HOME, FLAG_WATER_DROPPED_TWICE, MSGNO(MsgAltinTheGuestsInRoomOneMoved)},
    {EVENT_TALK, ACTOR_MAN_WAITING_TO_GO_HOME, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinTheGuestsInRoomTwoAlso)},
    {EVENT_TALK, ACTOR_MAN_WAITING_TO_GO_HOME, CONDITION_ALWAYS, EVENT_SCRIPT(Houses_AskAboutMineBeasts)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROOM_ONE_GUEST, FLAG_WATER_DROPPED_ONCE,
        MSGNO(MsgAltinLeavingAltinForTolbiWellThats)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROOM_ONE_GUEST, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinItsGoingToTakeALot)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROOM_ONE_GUEST, CONDITION_ALWAYS,
        MSGNO(MsgAltinTheMerchantsMustBeHavingA)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROOM_ONE_SECOND_GUEST, FLAG_WATER_DROPPED_ONCE, MSGNO(MsgAltinNowIFinallyHaveAnExcuse)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROOM_ONE_SECOND_GUEST, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinOneBlowsNotEnoughJabJab)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROOM_ONE_SECOND_GUEST, CONDITION_ALWAYS,
        MSGNO(MsgAltinIWonderWhatBusinessThatGirl)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROOM_TWO_GUEST, FLAG_WATER_DROPPED_TWICE, MSGNO(MsgAltinOfCourseYouCantGoWest)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROOM_TWO_GUEST, FLAG_HYDROS_STATUES_DEFEATED,
        MSGNO(MsgAltinIThoughtTheVillageWasDone)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROOM_TWO_GUEST, CONDITION_ALWAYS,
        MSGNO(MsgAltinTheySayYouCanStillFind)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROOM_TWO_SECOND_GUEST, FLAG_WATER_DROPPED_TWICE,
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
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT, FLAG_WATER_DRAINED, MSGNO(MsgAltinCripesIsThisHowMyWeapon)},
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
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_WAITING_TO_GO_HOME, FLAG_WATER_DROPPED_ONCE, MSGNO(MsgAltinMostOfTheVillageIsStill)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_WAITING_TO_GO_HOME, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinItLooksLikeTheWaterHas)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_WAITING_TO_GO_HOME, CONDITION_ALWAYS,
        MSGNO(MsgAltinItWillTakeSomeTimeBefore)},
    {SEARCH(SEARCH_BARREL), 100, FLAG_ALTIN_NUT, FIND_ITEM(ITEM_NUT)},
    {SEARCH(SEARCH_BARREL), 101, FLAG_ALTIN_COINS, FIND_COINS(9)},
    {SEARCH(SEARCH_BED), 102, FLAG_ALTIN_SLEEP_BOMB, FIND_ITEM(ITEM_SLEEP_BOMB)},
    {SEARCH(SEARCH_OVEN), 200, CONDITION_ALWAYS,
        FIND_MESSAGE(MSGNO(MsgAltinThePotsAndPansAreRusted))},
    {SEARCH(SEARCH_OVEN), 201, CONDITION_ALWAYS,
        FIND_MESSAGE(MSGNO(MsgAltinHeIsMakingRoastBeefEven))},
    {SEARCH(SEARCH_OVEN), 202, CONDITION_ALWAYS,
        FIND_MESSAGE(MSGNO(MsgAltinHeIsMakingShrimpInA))},
    {SEARCH(SEARCH_BOOKCASE), 203, CONDITION_ALWAYS,
        FIND_MESSAGE(MSGNO(MsgAltinThereIsABookTitledAltin))},
    {SCENE_EVENTS_END},
};
// clang-format on

/// The opening pictures of the village's doors, two cells square. The
/// Sanctum's door (the second) is wider, and Village_EnterDoor redraws it by
/// hand instead.
u16 gAltinVillage_SanctumDoorCells[] = {
    MAP_CELL_STEP(42, 33, 2, 2, 0),
    MAP_CELL_STEP(42, 35, 2, 2, 5),
    MAP_CELL_STEP(40, 33, 2, 2, 0),
    MAP_CELL_STEP(40, 35, 2, 2, 5),
    MAP_CELL_STEPS_END,
};

u16 gAltinVillage_SouthwestDoorCells[] = {
    MAP_CELL_STEP(35, 48, 2, 2, 5),
    MAP_CELL_STEP(33, 48, 2, 2, 5),
    MAP_CELL_STEPS_END,
};

u16 gAltinVillage_SoutheastDoorCells[] = {
    MAP_CELL_STEP(35, 35, 2, 2, 5),
    MAP_CELL_STEP(33, 35, 2, 2, 5),
    MAP_CELL_STEPS_END,
};

u16 gAltinVillage_EastDoorCells[] = {
    MAP_CELL_STEP(35, 46, 2, 2, 5),
    MAP_CELL_STEP(33, 46, 2, 2, 5),
    MAP_CELL_STEPS_END,
};

u16 gAltinVillage_NorthwestDoorCells[] = {
    MAP_CELL_STEP(35, 50, 2, 2, 5),
    MAP_CELL_STEP(33, 50, 2, 2, 5),
    MAP_CELL_STEPS_END,
};

u16 gAltinVillage_NorthAndWestDoorCells[] = {
    MAP_CELL_STEP(35, 33, 2, 2, 5),
    MAP_CELL_STEP(33, 33, 2, 2, 5),
    MAP_CELL_STEPS_END,
};

u16 gAltinVillage_NortheastDoorCells[] = {
    MAP_CELL_STEP(35, 52, 2, 2, 5),
    MAP_CELL_STEP(33, 52, 2, 2, 5),
    MAP_CELL_STEPS_END,
};

/// Each door trigger's opening picture and cell, from trigger 2.
struct MapCellAnim gAltinVillage_Doors[] = {
    {gAltinVillage_SouthwestDoorCells, 41, 5},
    {gAltinVillage_SanctumDoorCells, 51, 6},
    {gAltinVillage_NorthwestDoorCells, 57, 17},
    {gAltinVillage_NorthAndWestDoorCells, 39, 17},
    {gAltinVillage_NortheastDoorCells, 48, 25},
    {gAltinVillage_SoutheastDoorCells, 59, 9},
    {gAltinVillage_EastDoorCells, 42, 23},
    {gAltinVillage_NorthAndWestDoorCells, 41, 27},
};
