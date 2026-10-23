/// @file
/// Altin, the mining village north of the Alpine Crossing, in two maps that
/// share this overlay and the area name Altin: MapId_Altin, the village
/// outdoors, and MapId_AltinInteriors, inside its houses, shops and inn.
/// The getters pick each map's tables by gGameState.mapId, both share one
/// warp table, and a third, empty set of tables serves any other map.
///
/// The village is flooded by the water beasts of its mine, whose maps the
/// area names call Altin Peak (altin_peak.c), and two things in Altin's code
/// follow the party's work down there. The water drops in three stages with the
/// three drains it opens by beating the mine's water beasts
/// (FLAG_ALTIN_MINE_FIRST_DRAIN to _THIRD_DRAIN): Map_Initialize turns them
/// into three map temp flags, the water dropped once, twice, or drained (the
/// second and third drains both), and Village_ApplyWaterLevel copies the layout
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
/// spray (Spray_Initialize) running from Village_ApplyWaterLevel, until the
/// party
/// steps near it (Village_ScareOffBeast): it stops, cries out and hops away
/// into the flood for good (FLAG_ALTIN_WATER_BEAST_LEFT). Doors (coord events 2
/// to 9, Village_EnterDoor, each leaving by the warp of its number) lead into
/// AltinInteriors and into the Altin Sanctum (ALTIN_TO_SANCTUM), whose wide
/// door is redrawn by hand; coming back out of the Sanctum (ALTIN_FROM_SANCTUM)
/// the area name is not shown again. Three doorways (coord events 10 to
/// 12, Village_WalkThroughDoorway) and the warp ALTIN_TO_ALTIN_PEAK_SPRITZ lead
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
#include "map_ids.h"
#include "unit/map/altin.h"
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
#include "script_layer.h"
#include "engine_call.h"
#include "map_tables.h"
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

#define MAP_ALTIN_VILLAGE ((s32)&MapId_Altin)
#define MAP_ALTIN_HOUSES ((s32)&MapId_AltinInteriors)

/// The village's object events, numbering the rows of
/// gAltinVillage_ObjectEventTemplates. The people without a scene of their own
/// are named by their sprite and by what they say before the hydros statues are
/// beaten.
enum AltinVillageLocalId {
  /// A bald, bare-chested strong man (SPRITE_BALD_MUSCULAR_MAN);
  /// Village_AskAboutFirstVisit.
  /// ★ Named from the game: "Is this your first time to visit Altin?"
  /// (MsgAltinYourFirstTimeVisitAltin)
  LOCALID_GREETING_STRONG_MAN = LOCALID_FIRST_PLACED,
  /// A stocky strong man flexing his arms (SPRITE_STRONGMAN);
  /// Village_AskAboutHydros.
  /// ★ Named from the game: "How can those creatures spit up so much water?"
  /// (MsgAltinHowCanThoseCreaturesSpitUp)
  LOCALID_CURIOUS_STRONG_MAN,
  /// A woman with fair hair in a white apron (SPRITE_WOMAN_WITH_FAIR_HAIR).
  /// ★ Named from the game: "My house is totally coated in monster spit."
  /// (MsgAltinHowHorribleMyHouseIsTotally)
  LOCALID_WOMAN_COATED_HOUSE,
  /// A burly grey-haired man (SPRITE_BURLY_MAN_WITH_GREY_HAIR), who wanders.
  /// ★ Named from the game: "Even the mine is submerged."
  /// (MsgAltinEvenTheMineIsSubmergedNo)
  LOCALID_MAN_SUBMERGED_MINE,
  /// A bent old man with a cane (SPRITE_OLD_MAN_WITH_CANE).
  /// ★ Named from the game: "If we could just destroy the monsters"
  /// (MsgAltinIfWeCouldJustDestroyThe)
  LOCALID_OLD_MAN_DESTROY_MONSTERS,
  /// A woman in a green dress (SPRITE_WOMAN_IN_GREEN_DRESS).
  /// ★ Named from the game: "Altin always used to run short on water."
  /// (MsgAltinAltinAlwaysUsedToRunShort)
  LOCALID_WOMAN_WATER_LUCK,
  /// An old woman with white hair (SPRITE_OLD_WOMAN);
  /// Village_TalkToThirstyOldWoman.
  /// ★ Named from the game: "We can't drink the water the monsters spew!"
  /// (MsgAltinWeCantDrinkWaterMonsters)
  LOCALID_THIRSTY_OLD_WOMAN,
  /// The three who come back as the water recedes, one with each drop
  /// (Village_ApplyWaterLevel): this man with brown hair
  /// (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR), then a strong man and an old man.
  /// ★ Named from the game: "The waters around here have subsided..."
  /// (MsgAltinTheWatersAroundHereHaveSubsided)
  LOCALID_MAN_WATERS_SUBSIDED,
  /// A bald, bare-chested strong man (SPRITE_BALD_MUSCULAR_MAN).
  /// ★ Named from the game: "The water beasts looked exactly like Altin's
  /// guardian statues..." (MsgAltinTheWaterBeastsLookedExactlyLike)
  LOCALID_STRONG_MAN_GUARDIAN_STATUES,
  /// A bent old man with a cane (SPRITE_OLD_MAN_WITH_CANE), who wanders.
  /// ★ Named from the game: "The waters have flushed out tons of rock from the
  /// mine." (MsgAltinTheWatersHaveFlushedOutTons)
  LOCALID_OLD_MAN_ROCK_FLUSHED_OUT,
  /// The beast spitting water at the houses until FLAG_ALTIN_WATER_BEAST_LEFT.
  /// ★ Named from the game: "Are you the ones who defeated the water beasts?"
  /// (MsgAltinOnesWhoDefeatedWaterBeasts)
  LOCALID_WATER_BEAST,
  /// SPRITE_STONE_STATUE, which Move pushes to column 22
  /// (Village_CheckPillarMoved).
  /// ★ Named from what the player sees: a grey stone statue on a pedestal
  /// above the north path (its render)
  LOCALID_STONE_STATUE,
  /// The armor and item merchants, waiting outside while their shops are
  /// under water.
  /// ★ Named from the game: "My store is submerged... I want to sell armor,
  /// but there's no way." (MsgAltinMyStoreSubmergedWantSell)
  LOCALID_FLOODED_ARMOR_MERCHANT,
  /// ★ Named from the game: "Oh, no! What will I do if all my merchandise is
  /// soaked?" (MsgAltinWillDoIfMyMerchandise)
  LOCALID_FLOODED_ITEM_MERCHANT,
  /// Three crates placed with SPRITE_CRATE, which Village_ApplyWaterLevel hides
  /// (SPRITE_COLOR_HIDDEN) but leaves solid, as placed object events are:
  /// invisible blocks at the top of the ladder on the east cliff and, two of
  /// them, on the rails below the doorway into the mine (the drained village's
  /// render with them drawn).
  /// ★ Named from the game: "Strange... I don't remember all these crates being
  /// up here before!" (MsgVaultVaultCutFree)
  LOCALID_FIRST_CRATE,
  /// ★ Named from the game: "Strange... I don't remember all these crates being
  /// up here before!" (MsgVaultVaultCutFree)
  LOCALID_SECOND_CRATE,
  /// ★ Named from the game: "Strange... I don't remember all these crates being
  /// up here before!" (MsgVaultVaultCutFree)
  LOCALID_THIRD_CRATE
};

/// The houses' object events, numbering the rows of
/// gAltinHouses_ObjectEventTemplates, named as the village's are.
enum AltinHouseLocalId {
  /// The family sheltering in the inn's Room One until the first drop: this
  /// man with brown hair (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR) and a woman with
  /// fair
  /// hair.
  /// ★ Named from the game: "The guests in Room One moved back home when the
  /// waters receded." (MsgAltinTheGuestsInRoomOneMoved)
  LOCALID_ROOM_ONE_GUEST = LOCALID_FIRST_PLACED,
  /// A woman with fair hair in a white apron (SPRITE_WOMAN_WITH_FAIR_HAIR).
  /// ★ Named from the game: "The guests in Room One moved back home when the
  /// waters receded." (MsgAltinTheGuestsInRoomOneMoved)
  LOCALID_ROOM_ONE_SECOND_GUEST,
  /// The family in Room Two, until the second drop: this bent old man with a
  /// cane (SPRITE_OLD_MAN_WITH_CANE) and an old woman.
  /// ★ Named from the game: "The guests in Room Two also went home after the
  /// waters receded." (MsgAltinTheGuestsInRoomTwoAlso)
  LOCALID_ROOM_TWO_GUEST,
  /// An old woman with white hair (SPRITE_OLD_WOMAN).
  /// ★ Named from the game: "The guests in Room Two also went home after the
  /// waters receded." (MsgAltinTheGuestsInRoomTwoAlso)
  LOCALID_ROOM_TWO_SECOND_GUEST,
  /// A man with brown hair (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR). He and
  /// LOCALID_WOMAN_NO_PLACE_LIKE_HOME are the kissing girl's "Mom and Pop", who
  /// have been acting weird since the ruins were found: his thoughts won't
  /// give up a spot at the end of the mine's easternmost passage, and hers say
  /// "My husband's sense for finding treasure has never failed."
  /// ★ Named from the game: "I didn't want to leave my hometown..."
  /// (MsgAltinIDidntWantToLeaveMy)
  LOCALID_MAN_BACK_IN_HIS_HOMETOWN,
  /// A woman with fair hair in a white apron (SPRITE_WOMAN_WITH_FAIR_HAIR).
  /// ★ Named from the game: "There's no place like home."
  /// (MsgAltinIHeardSomeoneKilledTheWater)
  LOCALID_WOMAN_NO_PLACE_LIKE_HOME,
  /// A little girl with orange pigtails (SPRITE_GIRL_WITH_PIGTAILS);
  /// Houses_AskAboutMonsters.
  /// ★ Named from the game: "Here, let me thank you with...a kiss!"
  /// (MsgAltinDefeatedThoseMonstersDidnt)
  LOCALID_KISSING_GIRL,
  /// ★ Named from the game: "Cripes... Is this how my weapon shop ends?"
  /// (MsgAltinCripesIsThisHowMyWeapon)
  LOCALID_WEAPON_MERCHANT,
  /// ★ Named from the game: "Oh, no! What will I do if all my merchandise is
  /// soaked?" (MsgAltinWillDoIfMyMerchandise)
  LOCALID_ITEM_MERCHANT,
  /// ★ Named from the game: "It's great that I can sell armor out of my shop
  /// again." (MsgAltinItsGreatCanSellArmor)
  LOCALID_ARMOR_MERCHANT,
  /// Opens the inn (Houses_TalkToInnkeeper).
  /// ★ Named from the game: "Innkeeper" (MsgDebugGlyphName)
  LOCALID_INNKEEPER,
  /// A woman with red-brown hair in a white apron (SPRITE_WOMAN_IN_PINK_APRON).
  /// ★ Named from the game: "return Altin to its former glory."
  /// (MsgAltinIdGiveAnythingForSomeoneTo)
  LOCALID_WOMAN_FORMER_GLORY,
  /// ★ Named from the game: "I came from Xian. I cook good food."
  /// (MsgAltinICameFromXianICook)
  LOCALID_COOK,
  /// A man with brown hair (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR) in the inn,
  /// telling
  /// of its guests going home as the water drops.
  /// ★ Named from the game: "None of us can go home until the water
  /// subsides." (MsgAltinNoneOfUsCanGoHome)
  LOCALID_MAN_WAITING_TO_GO_HOME
};

/// The village's water level, set on entry from the mine's drain flags.
enum {
  /// ★ Named from the game: "The guests in Room One moved back home when the
  /// waters receded." (MsgAltinTheGuestsInRoomOneMoved)
  FLAG_WATER_DROPPED_ONCE = FLAG_MAP_TEMP_40,
  /// ★ Named from the game: "The guests in Room Two also went home after the
  /// waters receded." (MsgAltinTheGuestsInRoomTwoAlso)
  FLAG_WATER_DROPPED_TWICE = FLAG_MAP_TEMP_41,
  /// ★ Named from the game: "Thank goodness the water has receded."
  /// (MsgAltinThankGoodnessWaterHasReceded)
  FLAG_WATER_DRAINED = FLAG_MAP_TEMP_42
};

enum { SHOP_ALTIN_WEAPONS = 19, SHOP_ALTIN_ARMOR = 20, SHOP_ALTIN_ITEMS = 21 };

/// Altin's inn, as Engine_InnOpen numbers the inns.
enum { ALTIN_INN = 6 };

/// The doors of the village, by their index in gAltinVillage_Doors.
enum {
  /// The doors' coord events are numbered from 2 in the village's map events,
  /// and each takes the warp of its own number.
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
// for the object event's signed16-bit timer and wobble values.
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

const struct MapWarpDest *Map_GetWarpDests(void) {
  s16 mapId = gGameState.mapId;

  if (mapId == MAP_ALTIN_VILLAGE) {
    return gAltinVillage_WarpDests;
  }
  if (mapId == MAP_ALTIN_HOUSES) {
    return gAltinHouses_WarpDests;
  }
  return gAltinOther_WarpDests;
}

const struct MapWarpRegion *Map_GetWarpRegions(void) { return 0; }

/// Both maps share one warp table.
const u32 *Map_GetWarps(void) { return gAltin_Warps; }

/// Some villagers only turn back after talking once the flood is over or
/// their family has gone home.
const struct ObjectEventTemplate *Map_GetObjectEventTemplates(void) {
  s32 mapId = gGameState.mapId;
  if (mapId == MAP_ALTIN_VILLAGE) {
    if (Engine_GameFlagIsSet(FLAG_HYDROS_STATUES_DEFEATED)) {
      gAltinVillage_ObjectEventTemplates[LOCALID_WOMAN_WATER_LUCK -
                                         LOCALID_FIRST_PLACED]
          .talkFacing = TALK_FACE_PARTY;
      gAltinVillage_ObjectEventTemplates[LOCALID_THIRSTY_OLD_WOMAN -
                                         LOCALID_FIRST_PLACED]
          .talkFacing = TALK_FACE_PARTY;
    }
    return gAltinVillage_ObjectEventTemplates;
  }
  if (mapId == MAP_ALTIN_HOUSES) {
    if (Engine_GameFlagIsSet(FLAG_ALTIN_MINE_FIRST_DRAIN))
      gAltinHouses_ObjectEventTemplates[LOCALID_ROOM_ONE_SECOND_GUEST -
                                        LOCALID_FIRST_PLACED]
          .talkFacing = TALK_FACE_PARTY_AND_BACK;
    if (Engine_GameFlagIsSet(FLAG_ALTIN_MINE_SECOND_DRAIN) ||
        Engine_GameFlagIsSet(FLAG_ALTIN_MINE_THIRD_DRAIN))
      gAltinHouses_ObjectEventTemplates[LOCALID_ROOM_TWO_SECOND_GUEST -
                                        LOCALID_FIRST_PLACED]
          .talkFacing = TALK_FACE_PARTY_AND_BACK;
    Map_PrepareObjectEvents(gAltinHouses_ObjectEventTemplates);
    return gAltinHouses_ObjectEventTemplates;
  }
  return gAltinOther_ObjectEventTemplates;
}

const struct MapEvent *Map_GetEvents(void) {
  s16 mapId = gGameState.mapId;

  if (mapId == MAP_ALTIN_VILLAGE) {
    return gAltinVillage_Events;
  }
  if (mapId == MAP_ALTIN_HOUSES) {
    return gAltinHouses_Events;
  }
  return gAltinOther_Events;
}

/// A bald strong man asks whether this is the party's first visit. He's in
/// Altin's village before the hydros statues are beaten; either way, he says
/// the flood has changed Altin.
static void Village_AskAboutFirstVisit(void) {
  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgAltinYourFirstTimeVisitAltin);

  Engine_ScriptOpenMessage(LOCALID_GREETING_STRONG_MAN, 0);

  if (Engine_ScriptChooseYesNo(0, 0) == 1) {
    Engine_ScriptShowMessage(LOCALID_GREETING_STRONG_MAN, 0);
  } else {
    Script_SkipMessages(1);
    Engine_ScriptAskYesNo(LOCALID_GREETING_STRONG_MAN, 0);
  }

  Engine_ScriptEnd();
}

/// A strong man asks whether the slain hydros creatures gushed water. He's in
/// Altin's village once the hydros statues are beaten.
static void Village_AskAboutHydros(void) {
  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgAltinDidSeeWaterGushingOut);
  Engine_ScriptAskYesNo(LOCALID_CURIOUS_STRONG_MAN, 0);
  Engine_ScriptEnd();
}

/// An old woman in Altin's village. While the beasts live, she complains that
/// their water cannot be drunk, and the first time the camera pans to the water
/// beast spitting at the houses. Once the statues are gone, she asks for help
/// rebuilding.
static void Village_TalkToThirstyOldWoman(void) {

  struct ObjectEvent *beast;
  struct ObjectEvent *leader;

  Engine_ScriptStart();
  if (GameFlag_IsSet(FLAG_HYDROS_STATUES_DEFEATED) != 0) {
    Engine_ScriptSetMessage(MsgAltinOnesWhoDefeatedWaterBeasts);
    Script_AskYesNo(LOCALID_THIRSTY_OLD_WOMAN, 0);
  } else {
    Engine_ObjectEventSetAnimation(LOCALID_THIRSTY_OLD_WOMAN,
                                   ANIMATION_SHAKE_HEAD);
    Engine_ScriptSetMessage(MsgAltinWeCantDrinkWaterMonsters);
    Script_ShowMessageAndWait(LOCALID_THIRSTY_OLD_WOMAN, 0, 10);
    if (GameFlag_IsSet(FLAG_ALTIN_WATER_BEAST_LEFT) == 0) {
      beast = ObjectEvent_GetById(LOCALID_WATER_BEAST);

      Camera_GetScriptObject()->motionFlags = 0;
      Camera_SetSpeed(0x10000, 0x2000);
      Camera_MoveTo(beast->x.fixed, beast->y.fixed, beast->z.fixed, 1);
      // BUG: FACING_SOUTH's value is passed as a local id. No object event has
      // it, so the leader does not turn south toward the beast.
#ifdef BUGFIX
      ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 0);
#else
      ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, 0x4000, 0);
#endif
      ObjectEvent_FaceDirection(LOCALID_THIRSTY_OLD_WOMAN,
                                FACING_SOUTH - FACING_STEP, 0);
      Engine_CameraWaitForMove();
      Engine_ScriptWait(120);
      leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
      Camera_MoveTo(leader->x.fixed, leader->y.fixed, leader->z.fixed, 1);
      Engine_CameraWaitForMove();
    }
    Engine_ObjectEventSetAnimationAndWait(LOCALID_THIRSTY_OLD_WOMAN,
                                          ANIMATION_SHAKE_HEAD);
  }
  Engine_ScriptEnd();
}

/// An old man asks whether the party really found ruins deep in the mine. He's
/// in Altin's village once the hydros statues are beaten.
static void Village_AskAboutRuins(void) {

  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgAltinTrueFoundAncientRuinsIn);
  Engine_ScriptAskYesNo(LOCALID_OLD_MAN_ROCK_FLUSHED_OUT, 0);
  Engine_ScriptEnd();
}

/// A woman asks whether the party is who the girl from Xian was looking for.
/// She's from the family that sheltered in the inn's Room One, and asks inside
/// Altin once the hydros statues are beaten.
static void Houses_AskAboutXianGirl(void) {

  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgAltinGirlFromXianWasAsking);
  Engine_ScriptAskYesNo(LOCALID_ROOM_ONE_SECOND_GUEST, 0);
  Engine_ScriptEnd();
}

/// An old man points the party to the bottom of the mine. He's from the family
/// sheltering in the inn's Room Two, and inside Altin, before the hydros
/// statues are beaten, he asks whether the party seeks the way west.
static void Houses_AskAboutWayWest(void) {

  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgAltinTryingFindYourWayWest);
  Engine_ScriptAskYesNo(LOCALID_ROOM_TWO_GUEST, 0);
  Engine_ScriptEnd();
}

/// A little girl thanks the leader with a kiss. She's back home inside Altin
/// before the hydros statues are beaten, and asks whether the party defeated
/// the monsters; she gives the kiss either way.
static void Houses_AskAboutMonsters(void) {

  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgAltinDefeatedThoseMonstersDidnt);
  Engine_ScriptAskYesNo(LOCALID_KISSING_GIRL, 0);
  Engine_ScriptEnd();
}

/// A man at the inn asks whether the remaining beasts are deep below. He's
/// staying at Altin's inn once the hydros statues are beaten.
static void Houses_AskAboutMineBeasts(void) {

  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgAltinThereFewBeastsInMine);
  Engine_ScriptAskYesNo(LOCALID_MAN_WAITING_TO_GO_HOME, 0);
  Engine_ScriptEnd();
}

/// Returns whether the party leader faces north, toward a shop counter: its
/// facing lies strictly between northwest and northeast, measured in 16 bits.
static s32 Leader_FacesCounter(void) {
  struct ObjectEvent *leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);

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
    Engine_ScriptStart();
    Engine_ScriptSetMessage(MsgAltinDoWantWeapons);

    Engine_ScriptAskYesNo(LOCALID_WEAPON_MERCHANT, 0);
    Engine_ScriptEnd();
    return;
  }

  if (Leader_FacesCounter() != 0) {
    Engine_ShopOpen(SHOP_ALTIN_WEAPONS, LOCALID_WEAPON_MERCHANT);
    return;
  }

  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgAltinThankGoodnessWaterHasReceded);
  if (Engine_GameFlagIsSet(FLAG_HYDROS_STATUES_DEFEATED) != 0) {
    Engine_ScriptSetMessage(MsgAltinYoullHaveFindPassageIn);
  }
  Engine_ScriptShowMessage(LOCALID_WEAPON_MERCHANT, 0);
  Engine_ScriptEnd();
}

/// Altin's armor merchant, in either map: outside, while his shop is under
/// water, he laments; once it has dried after the second drop, he sells across
/// his counter in the houses.
static void Altin_TalkToArmorMerchant(void) {

  if (Engine_GameFlagIsSet(FLAG_WATER_DROPPED_TWICE) == 0) {
    Engine_ScriptStart();
    Engine_ScriptSetMessage(MsgAltinMyStoreSubmergedWantSell);
    Engine_ScriptShowMessage(LOCALID_FLOODED_ARMOR_MERCHANT, 0);
    Engine_ScriptEnd();
    return;
  }

  if (Leader_FacesCounter() != 0) {
    Engine_ShopOpen(SHOP_ALTIN_ARMOR, LOCALID_ARMOR_MERCHANT);
    return;
  }

  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgAltinItsGreatCanSellArmor);
  if (Engine_GameFlagIsSet(FLAG_HYDROS_STATUES_DEFEATED) != 0) {
    Engine_ScriptSetMessage(MsgAltinHowAboutArentImpressedBy);
  }
  Engine_ScriptShowMessage(LOCALID_ARMOR_MERCHANT, 0);
  Engine_ScriptEnd();
}

/// Altin's item merchant, the same way: she reopens after the first drop.
static void Altin_TalkToItemMerchant(void) {
  if (Engine_GameFlagIsSet(FLAG_WATER_DROPPED_ONCE) == 0) {
    Engine_ScriptStart();
    Engine_ScriptSetMessage(MsgAltinWillDoIfMyMerchandise);
    Engine_ScriptShowMessage(LOCALID_FLOODED_ITEM_MERCHANT, 0);
    Engine_ScriptEnd();
    return;
  }

  if (Leader_FacesCounter() != 0) {
    Engine_ShopOpen(SHOP_ALTIN_ITEMS, LOCALID_ITEM_MERCHANT);
    return;
  }

  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgAltinNoneMyGoodsWereDamaged);
  if (Engine_GameFlagIsSet(FLAG_HYDROS_STATUES_DEFEATED) != 0) {
    Engine_ScriptSetMessage(MsgAltinGirlFromXianBoughtLot);
  }
  Engine_ScriptShowMessage(LOCALID_ITEM_MERCHANT, 0);
  Engine_ScriptEnd();
}

/// Altin's innkeeper opens the inn when spoken to across the counter. Spoken to
/// otherwise, the innkeeper talks about the flood or, once it is over, the
/// temple west of the mine.
static void Houses_TalkToInnkeeper(void) {
  struct ObjectEvent *leader;

  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);

  if (ALTIN_FACES_COUNTER(leader->facing)) {
    Engine_InnOpen(ALTIN_INN, LOCALID_INNKEEPER);
    return;
  }

  Engine_ScriptStart();

  if (Engine_GameFlagIsSet(FLAG_HYDROS_STATUES_DEFEATED) != 0) {
    Engine_ScriptSetMessage(MsgAltinThereSmallTempleWestAltin);
    Engine_ScriptShowMessage(LOCALID_INNKEEPER, 0);
  } else {
    Engine_ScriptSetMessage(MsgAltinWeGotLittleDampBut);
    Engine_ScriptAskYesNo(LOCALID_INNKEEPER, 0);
  }

  Engine_ScriptEnd();
}

/// Walking north into one of the doors of Altin's village: everyone stops, the
/// door opens and the party leader walks in. The Sanctum's wide door is redrawn
/// in two halves by hand; the others play their opening pictures.
static void Village_EnterDoor(void) {
  u32 localId;
  struct EventWork *work;
  struct ObjectEvent *objectEvent;
  s32 doorIndex;
  u8 *doorBytes;
  s32 doorByteOffset;
  s32 coordinatesByteOffset;
  s32 column;
  s32 row;
  s32 rightColumn;

  work = gFieldWork;
  Engine_ScriptStart();
  for (localId = LOCALID_FIRST_PLACED; localId <= LOCALID_LAST_SLOT;
       localId++) {
    objectEvent = ObjectEvent_GetById(localId);
    if (objectEvent != 0) {
      objectEvent->motionFlags = 0;
    }
  }

  // The bytes read the entry through byte offsets and sign-extend the door
  // number by shifts; indexing the structs loads the fields differently.
  doorBytes = (u8 *)gAltinVillage_Doors;
  doorIndex =
      ((*(u16 *)&work->touchedTrigger - FIRST_DOOR_TRIGGER) << 16) >> 16;
  doorByteOffset = doorIndex << 3;
  coordinatesByteOffset = doorByteOffset + 4;
  column = *(s16 *)(doorBytes + coordinatesByteOffset);
  row = *(s16 *)(doorBytes + coordinatesByteOffset + 2);
  if (doorIndex == DOOR_SANCTUM) {
    // The Sanctum's door slides open with SE_188, as Bilibin's does.
    Script_PlaySong(SE_188);
    MapGrid_CopyCellsTo(42, 33, column, row, 2, 2);
    rightColumn = column + 2;
    MapGrid_CopyCellsTo(42, 35, rightColumn, row, 2, 2);
    Engine_ScriptWait(4);
    MapGrid_CopyCellsTo(40, 33, column, row, 2, 2);
    MapGrid_CopyCellsTo(40, 35, rightColumn, row, 2, 2);
    Engine_ScriptWait(4);
  } else {
    Script_PlaySong(SE_DOOR_OPEN);
    // The north door's row in gAltinVillage_Doors plays its opening at
    // (39, 17), outside the village's camera box; its house stands at
    // (8, 17), above where the party comes back out
    // (ALTIN_FROM_ALTIN_INTERIORS_NORTH), so that doorway is drawn open here
    // (a render with this copy made on entry shows the open doorway).
    if (doorIndex == ALTIN_TO_ALTIN_INTERIORS_NORTH - FIRST_DOOR_TRIGGER) {
      MapGrid_CopyCellsTo(33, 42, 8, 17, 1, 2);
    }
    MapGrid_AnimateCells(gAltinVillage_Doors[doorIndex].steps, column, row);
  }

  // Walk the leader through.
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, 0x8000, 0x4000);
  gFieldWork->startTransition = MAP_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->motionFlags = 0;
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_WALK);
  if (doorIndex == DOOR_STRAIGHT_IN) {
    ObjectEvent_CenterAndWalk(LOCALID_PARTY_LEADER, 2, 0);
  } else {
    if (doorIndex != DOOR_SANCTUM) {
      ObjectEvent_CenterAndWalk(LOCALID_PARTY_LEADER, 2, -4);
    } else {
      Engine_ObjectEventSetSpritePriority(LOCALID_PARTY_LEADER, 2);
      ObjectEvent_SetDestinationOffset(LOCALID_PARTY_LEADER, 0, -4);
    }
  }
  Engine_ScriptWait(10);
  Engine_ScriptRequestWarp(work->touchedTrigger);
  Engine_ScriptCloseScreen();
  Engine_ScriptWaitForScreen();
  Engine_ScriptEnd();
}

/// Walking north into one of the doorways of Altin's village that lead into
/// the mine: the leader walks out.
static void Village_WalkThroughDoorway(void) {
  struct EventWork *work;

  work = gFieldWork;
  Engine_ScriptStart();
  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->motionFlags = 0;
  Script_PlaySong(SE_MAP_EXIT);
  ObjectEvent_CenterAndWalk(LOCALID_PARTY_LEADER, 2, -16);
  Engine_ScriptRequestWarp(work->touchedTrigger);
  Engine_ScriptCloseScreen();
  Engine_ScriptWaitForScreen();
  Engine_ScriptEnd();
}

/// Update callback of the hopping water beast in Altin's village: once a
/// second it leaps up, falls back with a splash that shakes the screen
/// briefly, and stands while on the ground.
static void Beast_Hop(union FieldObject *fieldObject) {
  struct ObjectEvent *beast = &fieldObject->objectEvent;
  s16 *shakeFrames = (s16 *)&beast->work;

  // While this hook owns the beast, timer is its hop phase, work counts down
  // the landing shake, and the effect's countdown is a landing-sound latch.
  // Read work through its corresponding signed type for the countdown test.
  if (*shakeFrames != 0) {
    if (--*shakeFrames == 1)
      MapGrid_SetShake(-1, -1, 0xe666);
  }
  if (beast->velocityY == 0) {
    ObjectEvent_SetMode(beast, 1);
    beast->y.fixed += -0x18000;
    if (beast->y.fixed < beast->terrainHeight) {
      if (fieldObject->effect.countdown != 0) {
        Script_PlaySong(SE_SPLASH);
        fieldObject->effect.countdown = 0;
        beast->work = 4;
        MapGrid_SetShake(0, 0x10000, 0x10000);
      }
      beast->y.fixed = beast->terrainHeight;
    }
    beast->paused = 1;
  } else {
    beast->paused = 0;
  }
  if (beast->timer == 0) {
    Script_PlaySong(SE_JUMP);
    fieldObject->effect.countdown = 1;
    ObjectEvent_SetMode(beast, 2);
    beast->velocityY = 0x30000;
  }
  if (++beast->timer == WATER_BEAST_HOP_PERIOD)
    beast->timer = 0;
}

/// The water beast hops away across the flood and dives in with a splash.
static void Beast_HopAway(void) {
  struct ObjectEvent *beast;

  beast = ObjectEvent_GetById(LOCALID_WATER_BEAST);
  beast->timer = 0;
  beast->work = 0;
  beast->gravity = 0x6666;
  beast->update = Beast_Hop;
  ObjectEvent_SetSpeed(LOCALID_WATER_BEAST, 0x13333, 0x9999);
  ObjectEvent_MoveToAndWait(LOCALID_WATER_BEAST, 28, 460);
  ObjectEvent_MoveToAndWait(LOCALID_WATER_BEAST, 24, 448);
  Script_PlaySong(SE_SPLASH);
  Engine_ObjectEventDestroy(LOCALID_WATER_BEAST);
  MapGrid_SetShake(0, 0x10000, 0x10000);
  Engine_ScriptWait(4);
  MapGrid_SetShake(-1, -1, 0xe666);
  Engine_ScriptWait(40);
  Engine_ObjectEventSetAnimation(LOCALID_WATER_BEAST, ANIMATION_STAND);
}

/// After Move: remembers whether the pillar now stands in map column 22, the
/// spot it is pushed to.
static void Village_CheckPillarMoved(void) {
  struct ObjectEvent *pillar = ObjectEvent_GetById(LOCALID_STONE_STATUE);

  if ((pillar->x.fixed >> 20) == 22) {
    Engine_GameFlagSet(FLAG_ALTIN_PILLAR_MOVED);
  } else {
    Engine_GameFlagClear(FLAG_ALTIN_PILLAR_MOVED);
  }
}

/// Sets up the map being entered, at the village's water level. It works out
/// the water level from the mine's drain flags first. Returns 0.
s32 Map_Initialize(void) {
  s16 mapId;

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

  mapId = gGameState.mapId;
  if (mapId == MAP_ALTIN_VILLAGE) {
    Village_ApplyWaterLevel();
  } else if (mapId == MAP_ALTIN_HOUSES) {
    Houses_PlaceSheltering();
  }

  return 0;
}

/// Sets up Altin's village (Altin): the layout area for the current water
/// level (one of four 32 by 32 cell areas kept in the layout), the villagers
/// and merchants present at that level, the water beast with its spray while
/// it remains, and the pushed pillar. Then redraws the map around the leader.
static void Village_ApplyWaterLevel(void) {
  struct ObjectEvent *leader;

  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  // Select the water-level layout area and remove villagers whose areas are
  // flooded.
  if (GameFlag_IsSet(FLAG_WATER_DRAINED) != 0) {
    MapGrid_CopyCellsTo(64, 32, 0, 32, 32, 32);
    MapGrid_CopyCellAttributes(64, 32, 32, 32, 0, 0);
    Engine_ObjectEventDestroy(LOCALID_FLOODED_ARMOR_MERCHANT);
    Engine_ObjectEventDestroy(LOCALID_FLOODED_ITEM_MERCHANT);
  } else if (GameFlag_IsSet(FLAG_WATER_DROPPED_TWICE) != 0) {
    MapGrid_CopyCellsTo(64, 0, 0, 32, 32, 32);
    MapGrid_CopyCellAttributes(64, 0, 32, 32, 0, 0);
    Engine_ObjectEventDestroy(LOCALID_OLD_MAN_ROCK_FLUSHED_OUT);
    Engine_ObjectEventDestroy(LOCALID_FLOODED_ARMOR_MERCHANT);
    Engine_ObjectEventDestroy(LOCALID_FLOODED_ITEM_MERCHANT);
  } else if (GameFlag_IsSet(FLAG_WATER_DROPPED_ONCE) != 0) {
    MapGrid_CopyCellsTo(0, 64, 0, 32, 32, 32);
    MapGrid_CopyCellAttributes(0, 64, 32, 32, 0, 0);
    Engine_ObjectEventDestroy(LOCALID_STRONG_MAN_GUARDIAN_STATUES);
    Engine_ObjectEventDestroy(LOCALID_OLD_MAN_ROCK_FLUSHED_OUT);
    Engine_ObjectEventDestroy(LOCALID_FLOODED_ITEM_MERCHANT);
  } else {
    MapGrid_CopyCellAttributes(0, 32, 32, 32, 0, 0);
    Engine_ObjectEventDestroy(LOCALID_MAN_WATERS_SUBSIDED);
    Engine_ObjectEventDestroy(LOCALID_STRONG_MAN_GUARDIAN_STATUES);
    Engine_ObjectEventDestroy(LOCALID_OLD_MAN_ROCK_FLUSHED_OUT);
  }
  // Restore the remaining beast spitting water, with its sound (SE_170) and
  // its spray.
  if (GameFlag_IsSet(FLAG_ALTIN_WATER_BEAST_LEFT) != 0) {
    Engine_ObjectEventDestroy(LOCALID_WATER_BEAST);
  } else {
    FieldEffect_SetQueuedSoundAndPlay(SE_170);
    ObjectEvent_SetChildValue(LOCALID_WATER_BEAST, SPRITE_COLOR_RBG);
    Engine_ObjectEventSetAnimation(LOCALID_WATER_BEAST,
                                   WATER_BEAST_ANIMATION_SPIT);
    FieldScheduler_AddCallback(Beast_SpawnSpray, TASK_PRIORITY_MAP);
  }
  if (gGameState.warpDest == ALTIN_FROM_SANCTUM) {
    GameFlag_Clear(FLAG_SHOW_LOCATION_NAME);
  }
  MapGrid_CopyCellAttributes(0, 33, 4, 3, 20, 41);
  if (GameFlag_IsSet(FLAG_ALTIN_PILLAR_MOVED) != 0) {
    ObjectEvent_SetPosition(LOCALID_STONE_STATUE, PIXELS(360), PIXELS(168));
  }
  Engine_ObjectEventSetSpriteFlags(ObjectEvent_GetById(LOCALID_STONE_STATUE),
                                   0);
  // Hide the three crates, which stay solid, and draw them under the object
  // events.
  ObjectEvent_SetChildValue(LOCALID_FIRST_CRATE, SPRITE_COLOR_HIDDEN);
  Engine_ObjectEventSetChildValue(LOCALID_SECOND_CRATE, SPRITE_COLOR_HIDDEN);
  ObjectEvent_SetChildValue(LOCALID_THIRD_CRATE, SPRITE_COLOR_HIDDEN);
  ObjectEvent_GetById(LOCALID_FIRST_CRATE)->collisionFlags |=
      OBJ_EVENT_NOT_TARGETED;
  ObjectEvent_GetById(LOCALID_SECOND_CRATE)->collisionFlags |=
      OBJ_EVENT_NOT_TARGETED;
  ObjectEvent_GetById(LOCALID_THIRD_CRATE)->collisionFlags |=
      OBJ_EVENT_NOT_TARGETED;
  ObjectEvent_GetById(LOCALID_FIRST_CRATE)->priorityFlags |=
      OBJ_EVENT_PRIORITY_UNDERFOOT;
  ObjectEvent_GetById(LOCALID_SECOND_CRATE)->priorityFlags |=
      OBJ_EVENT_PRIORITY_UNDERFOOT;
  ObjectEvent_GetById(LOCALID_THIRD_CRATE)->priorityFlags |=
      OBJ_EVENT_PRIORITY_UNDERFOOT;
  Engine_ObjectEventSetSpritePriority(LOCALID_FIRST_CRATE, 1);
  Engine_ObjectEventSetSpritePriority(LOCALID_SECOND_CRATE, 1);
  Engine_ObjectEventSetSpritePriority(LOCALID_THIRD_CRATE, 1);
  // Redraw at the leader after the map and object state are ready.
  Engine_TaskWait(1);
  Engine_ScriptStart();
  Camera_MoveTo(leader->x.fixed, leader->y.fixed, leader->z.fixed, 0);
  Engine_MapGridRedraw();
  Engine_ScriptEnd();
  Engine_TaskWait(1);
}

/// Sets up inside Altin's buildings (AltinInteriors): the families still
/// sheltering in the inn's rooms, the weapon merchant waiting there until the
/// water drains, and the shopkeepers kept from wandering.
static void Houses_PlaceSheltering(void) {
  struct ObjectEvent *objectEvent;

  if (GameFlag_IsSet(FLAG_WATER_DROPPED_ONCE) == 0) {
    ObjectEvent_SetPosition(LOCALID_ROOM_ONE_GUEST, PIXELS(808), PIXELS(727));
    ObjectEvent_GetById(LOCALID_ROOM_ONE_GUEST)->facing =
        FACING_SOUTH - FACING_STEP;
    ObjectEvent_SetPosition(LOCALID_ROOM_ONE_SECOND_GUEST, PIXELS(794),
                            PIXELS(825));
  }
  if (GameFlag_IsSet(FLAG_WATER_DROPPED_TWICE) == 0) {
    ObjectEvent_SetPosition(LOCALID_ROOM_TWO_GUEST, PIXELS(560), PIXELS(710));
    ObjectEvent_GetById(LOCALID_ROOM_TWO_GUEST)->facing =
        FACING_EAST + FACING_STEP;
    ObjectEvent_SetPosition(LOCALID_ROOM_TWO_SECOND_GUEST, PIXELS(576),
                            PIXELS(710));
  }
  if (GameFlag_IsSet(FLAG_WATER_DRAINED) == 0) {
    ObjectEvent_SetPosition(LOCALID_WEAPON_MERCHANT, PIXELS(295), PIXELS(744));
    ObjectEvent_GetById(LOCALID_WEAPON_MERCHANT)->facing =
        FACING_NORTHWEST + FACING_STEP;
  } else {
    objectEvent = ObjectEvent_GetById(LOCALID_WEAPON_MERCHANT);
    objectEvent->collisionFlags |= OBJ_EVENT_NEAR_TALK;
  }
  objectEvent = ObjectEvent_GetById(LOCALID_ARMOR_MERCHANT);
  if (objectEvent != 0) {
    objectEvent->collisionFlags |= OBJ_EVENT_NEAR_TALK;
  }
  objectEvent = ObjectEvent_GetById(LOCALID_ITEM_MERCHANT);
  if (objectEvent != 0) {
    objectEvent->collisionFlags |= OBJ_EVENT_NEAR_TALK;
  }
}

/// Gives a new spray particle its look: SPRITE_DUST_PUFF standing still, drawn
/// with its red and blue swapped so the dust is water, on sprite priority 1
/// and at 0.3 scale. Spray_Drift turns some white as they fade.
static void Spray_Initialize(struct ObjectEvent *spray) {
  spray->motionFlags = 0;
  spray->sprite->image.priority = 1;
  ObjectGroup_SetChildValue(spray, SPRITE_COLOR_BGR);
  Engine_ObjectEventSetSpriteFlags(spray, 0);
  spray->scaleX = WATER_SPRAY_START_SCALE;
  spray->scaleY = WATER_SPRAY_START_SCALE;
}

/// Update callback of a spray particle: it drifts and grows with a random
/// wobble, then curls upward and flattens as its movement timer runs down.
/// The separate lifetime releases it after 20 updates.
static void Spray_Drift(union FieldObject *fieldObject) {
  struct ObjectEvent *spray = &fieldObject->objectEvent;
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
    spray = ((union FieldObject *)ObjectEvent_Create(
        SPRITE_DUST_PUFF, PIXELS(64), 0, PIXELS(400)));
    if (spray != 0) {
      spray->objectEvent.timer = WATER_SPRAY_LIFETIME;
      spray->objectEvent.work = 0;
      spray->effect.countdown = WATER_SPRAY_LIFETIME;
      Spray_Initialize(&spray->objectEvent);
      spray->objectEvent.update = Spray_Drift;
      ObjectEvent_SetMode(&spray->objectEvent, 1);
    }
  }
}

/// Stepping near the water beast in Altin's village: the camera pans to it, it
/// stops spitting, turns to the party, cries out and hops away into the flood.
static void Village_ScareOffBeast(void) {
  Engine_ScriptStart();
  Camera_SetSpeed(0x6666, 0xccc);
  Camera_MoveTo(PIXELS(63), -1, PIXELS(450), 1);
  Engine_CameraWaitForMove();
  Engine_ScriptWait(30);
  Engine_ObjectEventSetAnimation(LOCALID_WATER_BEAST, ANIMATION_STAND);
  // The spitting stops: its sound and its spray end.
  FieldEffect_SetQueuedSoundAndPlay(-1);
  Engine_TaskRemoveCallback(Beast_SpawnSpray);
  Engine_ScriptWait(20);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_WATER_BEAST, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(LOCALID_WATER_BEAST, FACING_EAST, 20);
  ObjectEvent_FaceDirection(LOCALID_WATER_BEAST, FACING_NORTH + FACING_STEP,
                            40);
  Script_PlaySong(SE_147);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_WATER_BEAST, 2);
  Engine_ScriptWait(20);
  ObjectEvent_FaceDirection(LOCALID_WATER_BEAST, FACING_NORTHWEST + FACING_STEP,
                            40);
  Beast_HopAway();
  Engine_CameraFollowObjectEvent(LOCALID_PARTY_LEADER, 1);
  Engine_CameraWaitForMove();
  Engine_ObjectEventSetAnimationAndWait(LOCALID_THIRSTY_OLD_WOMAN,
                                        ANIMATION_SHAKE_HEAD);
  GameFlag_Set(FLAG_ALTIN_WATER_BEAST_LEFT);
  Engine_ScriptEnd();
}

/// The cook from Xian paces between his pots, nodding over each.
s32 gAltinHouses_CookScript[] = {
    SC_ADD(SW_COLLISION, 128),
    SC_SET(SW_SPEED, 0x26666),
    SC_SET(SW_ACCELERATION, 0x13333),
    SC_LABEL(1),
    SC_ANIMATION(ANIMATION_WALK),
    SC_MOVE(PIXELS(342), 0, PIXELS(746)),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_NORTHWEST + FACING_STEP),
    SC_ANIMATION(ANIMATION_NOD),
    SC_WAIT(30),
    SC_ANIMATION(ANIMATION_WALK),
    SC_MOVE(PIXELS(356), 0, PIXELS(746)),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_NORTH + FACING_STEP),
    SC_ANIMATION(ANIMATION_NOD),
    SC_WAIT(50),
    SC_ANIMATION(ANIMATION_WALK),
    SC_MOVE(PIXELS(342), 0, PIXELS(746)),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_NORTHWEST + FACING_STEP),
    SC_ANIMATION(ANIMATION_NOD),
    SC_WAIT(20),
    SC_ANIMATION(ANIMATION_WALK),
    SC_MOVE(PIXELS(ALTIN_COOK_LAST_POT_X), 0, PIXELS(746)),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_NORTH + FACING_STEP),
    SC_ANIMATION(ANIMATION_NOD),
    SC_WAIT(30),
    SC_SET(SW_FACING, FACING_EAST + FACING_STEP),
    SC_WAIT(90),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
};

struct MapWarpDest gAltinOther_WarpDests[] = {
    {0, CONDITION_ALWAYS, 380, 0, 433, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {MAP_TABLE_END},
};

struct MapWarpDest gAltinVillage_WarpDests[] = {
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
    {MAP_TABLE_END},
};

struct MapWarpDest gAltinHouses_WarpDests[] = {
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
    {MAP_TABLE_END},
};

u32 gAltin_Warps[] = {
    MAP_WARPS(Altin),
    MAP_WARP(ALTIN_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_ALTIN),
    MAP_WARP(ALTIN_TO_ALTIN_INTERIORS_SOUTHWEST, AltinInteriors,
             ALTIN_INTERIORS_FROM_ALTIN_SOUTHWEST),
    MAP_WARP(ALTIN_TO_SANCTUM, Sanctum, SANCTUM_FROM_ALTIN),
    MAP_WARP(ALTIN_TO_ALTIN_INTERIORS_NORTHWEST, AltinInteriors,
             ALTIN_INTERIORS_FROM_ALTIN_NORTHWEST),
    MAP_WARP(ALTIN_TO_ALTIN_INTERIORS_NORTH, AltinInteriors,
             ALTIN_INTERIORS_FROM_ALTIN_NORTH),
    MAP_WARP(ALTIN_TO_ALTIN_INTERIORS_NORTHEAST, AltinInteriors,
             ALTIN_INTERIORS_FROM_ALTIN_NORTHEAST),
    MAP_WARP(ALTIN_TO_ALTIN_INTERIORS_SOUTHEAST, AltinInteriors,
             ALTIN_INTERIORS_FROM_ALTIN_SOUTHEAST),
    MAP_WARP(ALTIN_TO_ALTIN_INTERIORS_EAST, AltinInteriors,
             ALTIN_INTERIORS_FROM_ALTIN_EAST),
    MAP_WARP(ALTIN_TO_ALTIN_INTERIORS_WEST, AltinInteriors,
             ALTIN_INTERIORS_FROM_ALTIN_WEST),
    MAP_WARP(ALTIN_TO_ALTIN_PEAK_FIRST_WATER_BEAST, AltinPeakFirstWaterBeast,
             ALTIN_PEAK_FIRST_WATER_BEAST_FROM_ALTIN),
    MAP_WARP(ALTIN_TO_ALTIN_PEAK_SOUTH_ENTRANCE, AltinPeakSouthEntrance,
             ALTIN_PEAK_SOUTH_ENTRANCE_FROM_ALTIN),
    MAP_WARP(ALTIN_TO_ALTIN_PEAK_JUNCTION, AltinPeakJunction,
             ALTIN_PEAK_JUNCTION_FROM_ALTIN),
    MAP_WARP(ALTIN_TO_ALTIN_PEAK_SPRITZ, AltinPeakSpritz,
             ALTIN_PEAK_SPRITZ_FROM_ALTIN),
    MAP_WARPS(AltinInteriors),
    MAP_WARP(ALTIN_INTERIORS_TO_ALTIN_NORTHWEST, Altin,
             ALTIN_FROM_ALTIN_INTERIORS_NORTHWEST),
    MAP_WARP(ALTIN_INTERIORS_TO_ALTIN_NORTH, Altin,
             ALTIN_FROM_ALTIN_INTERIORS_NORTH),
    MAP_WARP(ALTIN_INTERIORS_TO_ALTIN_NORTHEAST, Altin,
             ALTIN_FROM_ALTIN_INTERIORS_NORTHEAST),
    MAP_WARP(ALTIN_INTERIORS_TO_ALTIN_WEST, Altin,
             ALTIN_FROM_ALTIN_INTERIORS_WEST),
    MAP_WARP(ALTIN_INTERIORS_TO_ALTIN_SOUTHEAST, Altin,
             ALTIN_FROM_ALTIN_INTERIORS_SOUTHEAST),
    MAP_WARP(ALTIN_INTERIORS_TO_ALTIN_EAST, Altin,
             ALTIN_FROM_ALTIN_INTERIORS_EAST),
    MAP_WARP(ALTIN_INTERIORS_TO_ALTIN_SOUTHWEST, Altin,
             ALTIN_FROM_ALTIN_INTERIORS_SOUTHWEST),
    MAP_WARP(ALTIN_INTERIORS_INN_TO_INN_UPSTAIRS, AltinInteriors,
             ALTIN_INTERIORS_INN_UPSTAIRS_FROM_INN),
    MAP_WARP(ALTIN_INTERIORS_INN_UPSTAIRS_TO_INN, AltinInteriors,
             ALTIN_INTERIORS_INN_FROM_INN_UPSTAIRS),
    MAP_WARPS_END,
};

struct ObjectEventTemplate gAltinOther_ObjectEventTemplates[] = {
    {MAP_TABLE_END},
};

struct ObjectEventTemplate gAltinVillage_ObjectEventTemplates[] = {
    {SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
     PIXELS(440), 0, PIXELS(200), FACING_SOUTH + FACING_STEP,
     TALK_FACE_PARTY_AND_BACK, 0},
    {SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(408), 0,
     PIXELS(216), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0},
    {SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
     PIXELS(310), 0, PIXELS(217), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY,
     0},
    {SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
     PIXELS(246), 0, PIXELS(186), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY,
     0},
    {SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR,
     PIXELS(58), 0, PIXELS(206), FACING_EAST + FACING_STEP, TALK_FACE_PARTY, 0},
    {SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
     PIXELS(104), 0, PIXELS(286), FACING_EAST + FACING_STEP,
     TALK_FACE_PARTY_AND_BACK, 0},
    {SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(41), 0,
     PIXELS(441), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0},
    {SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
     PIXELS(176), 0, PIXELS(296), FACING_SOUTHEAST + FACING_STEP,
     TALK_FACE_PARTY, 0},
    {SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
     PIXELS(136), 0, PIXELS(461), FACING_EAST + FACING_STEP, TALK_FACE_PARTY,
     0},
    {SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
     PIXELS(350), 0, PIXELS(286), FACING_NORTH, TALK_FACE_PARTY, 0},
    {SPRITE_WATER_BEAST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(60), 0,
     PIXELS(474), FACING_SOUTHEAST, TALK_FACE_PARTY, 0},
    {SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(344), 0,
     PIXELS(168), FACING_SOUTH, TALK_FACE_PARTY, 0},
    {SPRITE_YOUNG_MAN_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
     PIXELS(88), 0, PIXELS(397), FACING_EAST + FACING_STEP, TALK_FACE_PARTY, 0},
    {SPRITE_WOMAN_WITH_PONYTAIL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
     PIXELS(422), 0, PIXELS(208), FACING_EAST + FACING_STEP, TALK_FACE_PARTY,
     0},
    {SPRITE_CRATE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION, PIXELS(452), 0,
     PIXELS(216), FACING_SOUTH, TALK_KEEP_FACING, 0},
    {SPRITE_CRATE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION, PIXELS(332), 0,
     PIXELS(360), FACING_SOUTH, TALK_KEEP_FACING,
     OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE},
    {SPRITE_CRATE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION, PIXELS(332), 0,
     PIXELS(396), FACING_SOUTH, TALK_KEEP_FACING,
     OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE},
    {MAP_TABLE_END},
};

// clang-format off
struct ObjectEventTemplate gAltinHouses_ObjectEventTemplates[] = {
    {SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(104), 0, PIXELS(120), FACING_EAST + FACING_STEP,
        TALK_FACE_PARTY, 0},
    {SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(105), 0, PIXELS(233), FACING_SOUTH + FACING_STEP,
        TALK_FACE_PARTY, 0},
    {SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(456), 0, PIXELS(182), FACING_WEST + FACING_STEP, TALK_FACE_PARTY_THEN_WATCH,
        0},
    {SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(376), 0, PIXELS(120),
        FACING_SOUTHWEST + FACING_STEP, TALK_FACE_PARTY, 0},
    {SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, 0x0, MOVEMENT_TYPE_STAND, PIXELS(690), 0, PIXELS(128), FACING_SOUTHWEST + FACING_STEP,
        TALK_FACE_PARTY_AND_BACK, 0},
    {SPRITE_WOMAN_WITH_FAIR_HAIR, 0x0, MOVEMENT_TYPE_STAND, PIXELS(669), 0, PIXELS(128), FACING_EAST + FACING_STEP,
        TALK_FACE_PARTY_AND_BACK, 0},
    {SPRITE_GIRL_WITH_PIGTAILS, 0x0, MOVEMENT_TYPE_STAND, PIXELS(604), 0, PIXELS(145), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_THEN_WATCH, 0},
    {SPRITE_BALD_MAN_IN_RED, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(263), 0, PIXELS(436), FACING_SOUTH + FACING_STEP,
        TALK_FACE_PARTY, 0},
    {SPRITE_WOMAN_WITH_PONYTAIL, 0x0, MOVEMENT_TYPE_STAND, PIXELS(520), 0, PIXELS(454), FACING_SOUTH + FACING_STEP,
        TALK_FACE_PARTY, 0},
    {SPRITE_YOUNG_MAN_IN_GREEN, 0x0, MOVEMENT_TYPE_STAND, PIXELS(743), 0, PIXELS(454), FACING_SOUTHEAST + FACING_STEP,
        TALK_FACE_PARTY, 0},
    {SPRITE_MAN_WITH_DARK_HAIR, 0x0, MOVEMENT_TYPE_STAND, PIXELS(103), 0, PIXELS(744), FACING_SOUTHEAST + FACING_STEP,
        TALK_FACE_PARTY, 0},
    {SPRITE_WOMAN_IN_PINK_APRON, 0x0, MOVEMENT_TYPE_WANDER_NEAR, PIXELS(228), 0, PIXELS(756), FACING_SOUTHWEST + FACING_STEP, TALK_FACE_PARTY, 0},
    {SPRITE_COOK, 0x0, (s32)gAltinHouses_CookScript, PIXELS(350), 0, PIXELS(746),
        FACING_SOUTHWEST + FACING_STEP, TALK_FACE_PARTY, 0},
    {SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, 0x0, MOVEMENT_TYPE_STAND, PIXELS(646), 0, PIXELS(734), FACING_SOUTHEAST + FACING_STEP,
        TALK_FACE_PARTY, 0},
    {MAP_TABLE_END},
};

struct MapEvent gAltinOther_Events[] = {
    {MAP_EVENTS_END},
};

struct MapEvent gAltinVillage_Events[] = {
    {COORD_EVENT_FACING(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(Village_WalkThroughDoorway)},
    {COORD_EVENT_FACING(FACING_NORTH), 11, CONDITION_ALWAYS, EVENT_SCRIPT(Village_WalkThroughDoorway)},
    {COORD_EVENT_FACING(FACING_NORTH), 12, CONDITION_ALWAYS, EVENT_SCRIPT(Village_WalkThroughDoorway)},
    {MAP_EVENT_WARP, 13, CONDITION_ALWAYS, ALTIN_TO_ALTIN_PEAK_SPRITZ},
    {MAP_EVENT_WARP, 1, CONDITION_ALWAYS, ALTIN_TO_WORLD_MAP},
    {COORD_EVENT_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS,
        EVENT_SCRIPT(Village_EnterDoor)},
    {COORD_EVENT_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS,
        EVENT_SCRIPT(Village_EnterDoor)},
    {COORD_EVENT_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS,
        EVENT_SCRIPT(Village_EnterDoor)},
    {COORD_EVENT_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS,
        EVENT_SCRIPT(Village_EnterDoor)},
    {COORD_EVENT_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS,
        EVENT_SCRIPT(Village_EnterDoor)},
    {COORD_EVENT_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS,
        EVENT_SCRIPT(Village_EnterDoor)},
    {COORD_EVENT_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS,
        EVENT_SCRIPT(Village_EnterDoor)},
    {COORD_EVENT_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS,
        EVENT_SCRIPT(Village_EnterDoor)},
    {MAP_EVENT_TALK, LOCALID_GREETING_STRONG_MAN, FLAG_HYDROS_STATUES_DEFEATED, EVENT_SCRIPT(Village_AskAboutFirstVisit)},
    {MAP_EVENT_TALK, LOCALID_GREETING_STRONG_MAN, CONDITION_ALWAYS, MSGNO(MsgAltinThankYouSoMuchForDefeating)},
    {MAP_EVENT_TALK, LOCALID_CURIOUS_STRONG_MAN, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinHowCanThoseCreaturesSpitUp)},
    {MAP_EVENT_TALK, LOCALID_CURIOUS_STRONG_MAN, CONDITION_ALWAYS, EVENT_SCRIPT(Village_AskAboutHydros)},
    {MAP_EVENT_TALK, LOCALID_WOMAN_COATED_HOUSE, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinHowHorribleMyHouseIsTotally)},
    {MAP_EVENT_TALK, LOCALID_WOMAN_COATED_HOUSE, CONDITION_ALWAYS, MSGNO(MsgAltinOnceTheMineOpensAgainWell)},
    {MAP_EVENT_TALK, LOCALID_MAN_SUBMERGED_MINE, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinEvenTheMineIsSubmergedNo)},
    {MAP_EVENT_TALK, LOCALID_MAN_SUBMERGED_MINE, CONDITION_ALWAYS, MSGNO(MsgAltinTheyAlreadyStartedMiningTheUpper)},
    {MAP_EVENT_TALK, LOCALID_OLD_MAN_DESTROY_MONSTERS, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinIfWeCouldJustDestroyThe)},
    {MAP_EVENT_TALK, LOCALID_OLD_MAN_DESTROY_MONSTERS, CONDITION_ALWAYS, MSGNO(MsgAltinTheWaterDrainedOutOfThe)},
    {MAP_EVENT_TALK, LOCALID_WOMAN_WATER_LUCK, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinAltinAlwaysUsedToRunShort)},
    {MAP_EVENT_TALK, LOCALID_WOMAN_WATER_LUCK, CONDITION_ALWAYS, MSGNO(MsgAltinThankGoodnessTheVillageIsFree)},
    {MAP_EVENT_TALK, LOCALID_THIRSTY_OLD_WOMAN, CONDITION_ALWAYS, EVENT_SCRIPT(Village_TalkToThirstyOldWoman)},
    {MAP_EVENT_TALK, LOCALID_MAN_WATERS_SUBSIDED, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinTheWatersAroundHereHaveSubsided)},
    {MAP_EVENT_TALK, LOCALID_MAN_WATERS_SUBSIDED, CONDITION_ALWAYS, MSGNO(MsgAltinTheVillageWentRightBackTo)},
    {MAP_EVENT_TALK, LOCALID_STRONG_MAN_GUARDIAN_STATUES, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinTheWaterBeastsLookedExactlyLike)},
    {MAP_EVENT_TALK, LOCALID_STRONG_MAN_GUARDIAN_STATUES, CONDITION_ALWAYS, MSGNO(MsgAltinSoThoseHydrosCreaturesReallyWere)},
    {MAP_EVENT_TALK, LOCALID_OLD_MAN_ROCK_FLUSHED_OUT, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinTheWatersHaveFlushedOutTons)},
    {MAP_EVENT_TALK, LOCALID_OLD_MAN_ROCK_FLUSHED_OUT, CONDITION_ALWAYS, EVENT_SCRIPT(Village_AskAboutRuins)},
    {MAP_EVENT_TALK, LOCALID_FLOODED_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(Altin_TalkToArmorMerchant)},
    {MAP_EVENT_TALK, LOCALID_FLOODED_ITEM_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(Altin_TalkToItemMerchant)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_GREETING_STRONG_MAN, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinYouCameToAltinAtA)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_GREETING_STRONG_MAN, CONDITION_ALWAYS,
        MSGNO(MsgAltinWeCanGoBackToWork)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_CURIOUS_STRONG_MAN, FLAG_HYDROS_STATUES_DEFEATED,
        MSGNO(MsgAltinItsAlmostLikeTheyreTurningThe)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_CURIOUS_STRONG_MAN, CONDITION_ALWAYS,
        MSGNO(MsgAltinIWasAfraidMoreWaterWould)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WOMAN_COATED_HOUSE, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinImGladISavedMyVery)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WOMAN_COATED_HOUSE, CONDITION_ALWAYS,
        MSGNO(MsgAltinIHopeAltinMineWillThrive)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_MAN_SUBMERGED_MINE, FLAG_HYDROS_STATUES_DEFEATED,
        MSGNO(MsgAltinEvenTheToughestOfMinersCant)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_MAN_SUBMERGED_MINE, CONDITION_ALWAYS,
        MSGNO(MsgAltinTheWaterSeepedIntoTheMine)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_OLD_MAN_DESTROY_MONSTERS, FLAG_HYDROS_STATUES_DEFEATED,
        MSGNO(MsgAltinIfTheWaterBeastsContinueTo)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_OLD_MAN_DESTROY_MONSTERS, CONDITION_ALWAYS,
        MSGNO(MsgAltinItsSoHumidInsideThatCave)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WOMAN_WATER_LUCK, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinWaterIsNiceButNotWhen)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WOMAN_WATER_LUCK, CONDITION_ALWAYS,
        MSGNO(MsgAltinThatStrangeChamberInTheMines)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_THIRSTY_OLD_WOMAN, FLAG_HYDROS_STATUES_DEFEATED,
        MSGNO(MsgAltinUghTheMonstersSpewedOutAll)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_THIRSTY_OLD_WOMAN, CONDITION_ALWAYS,
        MSGNO(MsgAltinYouFoughtPrettyWellDespiteAll)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_MAN_WATERS_SUBSIDED, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinAsTheMonstersGoAwayThe)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_MAN_WATERS_SUBSIDED, CONDITION_ALWAYS,
        MSGNO(MsgAltinThatMineIsFullOfHoles)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STRONG_MAN_GUARDIAN_STATUES, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinComeToThinkOfItIt)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STRONG_MAN_GUARDIAN_STATUES, CONDITION_ALWAYS,
        MSGNO(MsgAltinTheOldGuardianStatuesAlwaysCreeped)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_OLD_MAN_ROCK_FLUSHED_OUT, FLAG_HYDROS_STATUES_DEFEATED,
        MSGNO(MsgAltinItllBeBetterThanDiggingUp)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_OLD_MAN_ROCK_FLUSHED_OUT, CONDITION_ALWAYS,
        MSGNO(MsgAltinIUsedToFindAllSorts)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_FLOODED_ARMOR_MERCHANT, CONDITION_ALWAYS,
        MSGNO(MsgAltinAllMyArmorInventoryIsRusting)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_FLOODED_ITEM_MERCHANT, CONDITION_ALWAYS,
        MSGNO(MsgAltinIfMyGoodsAreWetIll)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_STONE_STATUE, CONDITION_ALWAYS, EVENT_SCRIPT(Village_CheckPillarMoved)},
    {MAP_EVENT_COORD, TRIGGER_NEAR_BEAST, FLAG_ALTIN_WATER_BEAST_LEFT, EVENT_SCRIPT(Village_ScareOffBeast)},
    {MAP_EVENTS_END},
};

struct MapEvent gAltinHouses_Events[] = {
    {MAP_EVENT_WARP, 1, CONDITION_ALWAYS, ALTIN_INTERIORS_TO_ALTIN_NORTHWEST},
    {MAP_EVENT_WARP, 2, CONDITION_ALWAYS, ALTIN_INTERIORS_TO_ALTIN_NORTH},
    {MAP_EVENT_WARP, 3, CONDITION_ALWAYS, ALTIN_INTERIORS_TO_ALTIN_NORTHEAST},
    {MAP_EVENT_WARP, 4, CONDITION_ALWAYS, ALTIN_INTERIORS_TO_ALTIN_WEST},
    {MAP_EVENT_WARP, 5, CONDITION_ALWAYS, ALTIN_INTERIORS_TO_ALTIN_SOUTHEAST},
    {MAP_EVENT_WARP, 6, CONDITION_ALWAYS, ALTIN_INTERIORS_TO_ALTIN_EAST},
    {MAP_EVENT_WARP, 7, CONDITION_ALWAYS, ALTIN_INTERIORS_TO_ALTIN_SOUTHWEST},
    {MAP_EVENT_WARP, 8, CONDITION_ALWAYS, ALTIN_INTERIORS_INN_TO_INN_UPSTAIRS},
    {MAP_EVENT_WARP, 9, CONDITION_ALWAYS, ALTIN_INTERIORS_INN_UPSTAIRS_TO_INN},
    {MAP_EVENT_TALK, LOCALID_ROOM_ONE_GUEST, FLAG_WATER_DROPPED_ONCE, MSGNO(MsgAltinNoMatterWhatHappensIdNever)},
    {MAP_EVENT_TALK, LOCALID_ROOM_ONE_GUEST, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinOhNoTheWholeHouseIs)},
    {MAP_EVENT_TALK, LOCALID_ROOM_ONE_GUEST, CONDITION_ALWAYS, MSGNO(MsgAltinIHearThatSilkRoadIs)},
    {MAP_EVENT_TALK, LOCALID_ROOM_ONE_SECOND_GUEST, FLAG_WATER_DROPPED_ONCE, MSGNO(MsgAltinOfCourseOurHouseWillBe)},
    {MAP_EVENT_TALK, LOCALID_ROOM_ONE_SECOND_GUEST, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinIfTheMonsterThatDidThis)},
    {MAP_EVENT_TALK, LOCALID_ROOM_ONE_SECOND_GUEST, CONDITION_ALWAYS, EVENT_SCRIPT(Houses_AskAboutXianGirl)},
    {MAP_EVENT_TALK, LOCALID_ROOM_TWO_GUEST, FLAG_WATER_DROPPED_TWICE, MSGNO(MsgAltinNotManyPeopleComeUpSilk)},
    {MAP_EVENT_TALK, LOCALID_ROOM_TWO_GUEST, FLAG_HYDROS_STATUES_DEFEATED, EVENT_SCRIPT(Houses_AskAboutWayWest)},
    {MAP_EVENT_TALK, LOCALID_ROOM_TWO_GUEST, CONDITION_ALWAYS, MSGNO(MsgAltinAncientRuinsAndTreasureDefinitelyGo)},
    {MAP_EVENT_TALK, LOCALID_ROOM_TWO_SECOND_GUEST, FLAG_WATER_DROPPED_TWICE, MSGNO(MsgAltinItsBeenALongTimeSince)},
    {MAP_EVENT_TALK, LOCALID_ROOM_TWO_SECOND_GUEST, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinDontBeTooHarshOnThose)},
    {MAP_EVENT_TALK, LOCALID_ROOM_TWO_SECOND_GUEST, CONDITION_ALWAYS, MSGNO(MsgAltinIfOnlyTheyHadLeftA)},
    {MAP_EVENT_TALK, LOCALID_MAN_BACK_IN_HIS_HOMETOWN, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinIDidntWantToLeaveMy)},
    {MAP_EVENT_TALK, LOCALID_MAN_BACK_IN_HIS_HOMETOWN, CONDITION_ALWAYS, MSGNO(MsgAltinReallyAncientRuinsAtTheBottom)},
    {MAP_EVENT_TALK, LOCALID_WOMAN_NO_PLACE_LIKE_HOME, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinIHeardSomeoneKilledTheWater)},
    {MAP_EVENT_TALK, LOCALID_WOMAN_NO_PLACE_LIKE_HOME, CONDITION_ALWAYS, MSGNO(MsgAltinThereProbablyIsntMuchTreasureIn)},
    {MAP_EVENT_TALK, LOCALID_KISSING_GIRL, FLAG_HYDROS_STATUES_DEFEATED, EVENT_SCRIPT(Houses_AskAboutMonsters)},
    {MAP_EVENT_TALK, LOCALID_KISSING_GIRL, CONDITION_ALWAYS, MSGNO(MsgAltinSomethingsWrongWithMomAndPop)},
    {MAP_EVENT_TALK, LOCALID_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(Houses_TalkToWeaponMerchant)},
    {MAP_EVENT_TALK, LOCALID_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(Altin_TalkToArmorMerchant)},
    {MAP_EVENT_TALK, LOCALID_ITEM_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(Altin_TalkToItemMerchant)},
    {MAP_EVENT_TALK, LOCALID_INNKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(Houses_TalkToInnkeeper)},
    {MAP_EVENT_TALK, LOCALID_WOMAN_FORMER_GLORY, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinIdGiveAnythingForSomeoneTo)},
    {MAP_EVENT_TALK, LOCALID_WOMAN_FORMER_GLORY, CONDITION_ALWAYS, MSGNO(MsgAltinThereIsAnEndlessDesertWest)},
    {MAP_EVENT_TALK, LOCALID_COOK, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinICameFromXianICook)},
    {MAP_EVENT_TALK, LOCALID_COOK, CONDITION_ALWAYS, MSGNO(MsgAltinWestOfTheMountainsIsVery)},
    {MAP_EVENT_TALK, LOCALID_MAN_WAITING_TO_GO_HOME, FLAG_WATER_DROPPED_ONCE, MSGNO(MsgAltinNoneOfUsCanGoHome)},
    {MAP_EVENT_TALK, LOCALID_MAN_WAITING_TO_GO_HOME, FLAG_WATER_DROPPED_TWICE, MSGNO(MsgAltinTheGuestsInRoomOneMoved)},
    {MAP_EVENT_TALK, LOCALID_MAN_WAITING_TO_GO_HOME, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinTheGuestsInRoomTwoAlso)},
    {MAP_EVENT_TALK, LOCALID_MAN_WAITING_TO_GO_HOME, CONDITION_ALWAYS, EVENT_SCRIPT(Houses_AskAboutMineBeasts)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROOM_ONE_GUEST, FLAG_WATER_DROPPED_ONCE,
        MSGNO(MsgAltinLeavingAltinForTolbiWellThats)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROOM_ONE_GUEST, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinItsGoingToTakeALot)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROOM_ONE_GUEST, CONDITION_ALWAYS,
        MSGNO(MsgAltinTheMerchantsMustBeHavingA)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROOM_ONE_SECOND_GUEST, FLAG_WATER_DROPPED_ONCE, MSGNO(MsgAltinNowIFinallyHaveAnExcuse)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROOM_ONE_SECOND_GUEST, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinOneBlowsNotEnoughJabJab)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROOM_ONE_SECOND_GUEST, CONDITION_ALWAYS,
        MSGNO(MsgAltinIWonderWhatBusinessThatGirl)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROOM_TWO_GUEST, FLAG_WATER_DROPPED_TWICE, MSGNO(MsgAltinOfCourseYouCantGoWest)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROOM_TWO_GUEST, FLAG_HYDROS_STATUES_DEFEATED,
        MSGNO(MsgAltinIThoughtTheVillageWasDone)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROOM_TWO_GUEST, CONDITION_ALWAYS,
        MSGNO(MsgAltinTheySayYouCanStillFind)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROOM_TWO_SECOND_GUEST, FLAG_WATER_DROPPED_TWICE,
        MSGNO(MsgAltinIHateSittingAroundLikeThis)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROOM_TWO_SECOND_GUEST, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinImJustGladToGetMy)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ROOM_TWO_SECOND_GUEST, CONDITION_ALWAYS,
        MSGNO(MsgAltinHowCanITrulyBeHappy)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_MAN_BACK_IN_HIS_HOMETOWN, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinTheBottomOfTheMineThe)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_MAN_BACK_IN_HIS_HOMETOWN, CONDITION_ALWAYS,
        MSGNO(MsgAltinCripesAncientRuinsDownInThe)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WOMAN_NO_PLACE_LIKE_HOME, FLAG_HYDROS_STATUES_DEFEATED,
        MSGNO(MsgAltinMyHusbandsSenseForFindingTreasure)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WOMAN_NO_PLACE_LIKE_HOME, CONDITION_ALWAYS,
        MSGNO(MsgAltinIHadNoIdeaThereWere)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_KISSING_GIRL, FLAG_HYDROS_STATUES_DEFEATED,
        MSGNO(MsgAltinMomAndPopKnowSomethingAbout)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_KISSING_GIRL, CONDITION_ALWAYS,
        MSGNO(MsgAltinIWonderIfMomAndPop)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WEAPON_MERCHANT, FLAG_WATER_DRAINED, MSGNO(MsgAltinCripesIsThisHowMyWeapon)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WEAPON_MERCHANT, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinOnlyAMerchantKnowsTheJoy)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WEAPON_MERCHANT, CONDITION_ALWAYS,
        MSGNO(MsgAltinIveHeardOfAPassageIn)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ARMOR_MERCHANT, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinWeDriedOffAllThatArmor)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ARMOR_MERCHANT, CONDITION_ALWAYS,
        MSGNO(MsgAltinWellIfYouAreImpressedThen)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ITEM_MERCHANT, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinAFewThingsGotDampBut)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ITEM_MERCHANT, CONDITION_ALWAYS,
        MSGNO(MsgAltinTheGirlFromXianBoughtAll)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_INNKEEPER, FLAG_HYDROS_STATUES_DEFEATED,
        MSGNO(MsgAltinWellComeThroughThisAllRight)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_INNKEEPER, CONDITION_ALWAYS,
        MSGNO(MsgAltinIThinkThatTempleWasCalled)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WOMAN_FORMER_GLORY, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinIKnowTheyDontLookSo)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WOMAN_FORMER_GLORY, CONDITION_ALWAYS,
        MSGNO(MsgAltinYouWontBeAbleToGo)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_COOK, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinIBecameACookAfterI)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_COOK, CONDITION_ALWAYS,
        MSGNO(MsgAltinNoOneCanGoWestOf)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_MAN_WAITING_TO_GO_HOME, FLAG_WATER_DROPPED_ONCE, MSGNO(MsgAltinMostOfTheVillageIsStill)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_MAN_WAITING_TO_GO_HOME, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinItLooksLikeTheWaterHas)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_MAN_WAITING_TO_GO_HOME, CONDITION_ALWAYS,
        MSGNO(MsgAltinItWillTakeSomeTimeBefore)},
    {BG_EVENT(BG_EVENT_BARREL), 100, FLAG_ALTIN_NUT, FIND_ITEM(ITEM_NUT)},
    {BG_EVENT(BG_EVENT_BARREL), 101, FLAG_ALTIN_COINS, FIND_COINS(9)},
    {BG_EVENT(BG_EVENT_BED), 102, FLAG_ALTIN_SLEEP_BOMB, FIND_ITEM(ITEM_SLEEP_BOMB)},
    {BG_EVENT(BG_EVENT_OVEN), 200, CONDITION_ALWAYS,
        FIND_MESSAGE(MSGNO(MsgAltinThePotsAndPansAreRusted))},
    {BG_EVENT(BG_EVENT_OVEN), 201, CONDITION_ALWAYS,
        FIND_MESSAGE(MSGNO(MsgAltinHeIsMakingRoastBeefEven))},
    {BG_EVENT(BG_EVENT_OVEN), 202, CONDITION_ALWAYS,
        FIND_MESSAGE(MSGNO(MsgAltinHeIsMakingShrimpInA))},
    {BG_EVENT(BG_EVENT_BOOKCASE), 203, CONDITION_ALWAYS,
        FIND_MESSAGE(MSGNO(MsgAltinThereIsABookTitledAltin))},
    {MAP_EVENTS_END},
};
// clang-format on

/// The opening pictures of the village's doors, two cells square. The
/// Sanctum's door (the second) is wider, and Village_EnterDoor redraws it by
/// hand instead.
u16 gAltinVillage_SanctumDoorCells[] = {
    MAPGRID_ANIMATION_STEP(42, 33, 2, 2, 0),
    MAPGRID_ANIMATION_STEP(42, 35, 2, 2, 5),
    MAPGRID_ANIMATION_STEP(40, 33, 2, 2, 0),
    MAPGRID_ANIMATION_STEP(40, 35, 2, 2, 5),
    MAPGRID_ANIMATION_STEPS_END,
};

u16 gAltinVillage_SouthwestDoorCells[] = {
    MAPGRID_ANIMATION_STEP(35, 48, 2, 2, 5),
    MAPGRID_ANIMATION_STEP(33, 48, 2, 2, 5),
    MAPGRID_ANIMATION_STEPS_END,
};

u16 gAltinVillage_SoutheastDoorCells[] = {
    MAPGRID_ANIMATION_STEP(35, 35, 2, 2, 5),
    MAPGRID_ANIMATION_STEP(33, 35, 2, 2, 5),
    MAPGRID_ANIMATION_STEPS_END,
};

u16 gAltinVillage_EastDoorCells[] = {
    MAPGRID_ANIMATION_STEP(35, 46, 2, 2, 5),
    MAPGRID_ANIMATION_STEP(33, 46, 2, 2, 5),
    MAPGRID_ANIMATION_STEPS_END,
};

u16 gAltinVillage_NorthwestDoorCells[] = {
    MAPGRID_ANIMATION_STEP(35, 50, 2, 2, 5),
    MAPGRID_ANIMATION_STEP(33, 50, 2, 2, 5),
    MAPGRID_ANIMATION_STEPS_END,
};

u16 gAltinVillage_NorthAndWestDoorCells[] = {
    MAPGRID_ANIMATION_STEP(35, 33, 2, 2, 5),
    MAPGRID_ANIMATION_STEP(33, 33, 2, 2, 5),
    MAPGRID_ANIMATION_STEPS_END,
};

u16 gAltinVillage_NortheastDoorCells[] = {
    MAPGRID_ANIMATION_STEP(35, 52, 2, 2, 5),
    MAPGRID_ANIMATION_STEP(33, 52, 2, 2, 5),
    MAPGRID_ANIMATION_STEPS_END,
};

/// Each door coord event's opening picture and cell, from coord event 2.
struct MapGridCellAnimation gAltinVillage_Doors[] = {
    {gAltinVillage_SouthwestDoorCells, 41, 5},
    {gAltinVillage_SanctumDoorCells, 51, 6},
    {gAltinVillage_NorthwestDoorCells, 57, 17},
    {gAltinVillage_NorthAndWestDoorCells, 39, 17},
    {gAltinVillage_NortheastDoorCells, 48, 25},
    {gAltinVillage_SoutheastDoorCells, 59, 9},
    {gAltinVillage_EastDoorCells, 42, 23},
    {gAltinVillage_NorthAndWestDoorCells, 41, 27},
};
