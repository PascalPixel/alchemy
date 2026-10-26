/// @file
/// Log rooms C (SceneId_LogRoomsC), a sheet of rooms drawn in logs, all of them
/// Kolima's: the village's houses, shops and inn. Some of its exits join its
/// own rooms by the stairs up the inn and the weapon and armor shop, five lead
/// back out to the village (SceneId_Kolima) and one into Kolima Tunnel
/// (SceneId_KolimaTunnel, LOG_ROOMS_C_TO_KOLIMA_TUNNEL).
///
/// Like the people outside, the fifteen residents (ACTOR_FIRST_VILLAGER to
/// ACTOR_LAST_VILLAGER) were turned into trees when the curse fell, and until
/// it is lifted in Kolima Forest (FLAG_KOLIMA_CURSE_LIFTED)
/// LogRoomsC_GetActorTemplates gives them tree sprites that stand
/// still: they answer only Mind Read, each with the thoughts of someone stuck
/// as a tree
/// ("Who caused all this? Someone must have tried to cut down Tret!"). Once the
/// curse is lifted they talk, of the warriors said to have saved the village,
/// of McCoy's halted palace and the lumberjacks' lost work, of a father's
/// secret treasure "hidden deep in the forest", and the weapon, armor and item
/// sellers and the innkeeper serve the party across their counters. Three
/// bookcases hold books on the holy tree ("Tret and Laurel are the spirits of
/// the holy trees protecting Kolima forest"), on trees and flowers and on
/// logging; a jar holds a Lucky Medal and a barrel 7 coins.
///
/// LogRoomsC_Initialize opens with a window transition and draws one of
/// two rooms into the same place on the map. Entered by its door from the
/// village (LOG_ROOMS_C_HOUSE_FROM_KOLIMA_SOUTH), it is the father's
/// house, with its bookcase and rug and the father's family
/// (ACTOR_FATHER_WITH_A_SECRET, ACTOR_WIFE_KEPT_FROM_THE_SECRET and
/// ACTOR_CHILD_TOLD_THE_SECRET; places/kolima-interiors_cursed/map-7.png).
/// Entered from behind, by the village doorway the leader steps south into
/// (LOG_ROOMS_C_HOUSE_FROM_KOLIMA_NORTH) or up the ladder from Kolima
/// Tunnel (LOG_ROOMS_C_FROM_KOLIMA_TUNNEL), other cells are copied in and
/// it is a storeroom of crates and sacks with the ladder down in its floor, and
/// the family is put out of sight (places/kolima-interiors_cursed/map-8.png).

#include "save_block.h"
#include "flags.h"
#include "unit/scene/log_rooms_c.h"
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

#include "text/msg_ids.h"
#include "ability_ids.h"
#include "scene_tables.h"
#include "item_ids.h"

/// Kolima's shops and inn, as Shop_Run and Inn_CheckIn number them.
enum {
  KOLIMA_WEAPON_SHOP = 13,
  KOLIMA_ARMOR_SHOP = 14,
  KOLIMA_ITEM_SHOP = 15,
  KOLIMA_INN = 3
};

/// The residents, numbering gLogRoomsC_ActorTemplates' rows from
/// ACTOR_FIRST_PLACED. Their notes quote what they say once the curse is
/// lifted.
enum KolimaInteriorActorId {
  /// An old man with a cane.
  /// ★ Named from the game: "I heard that traveling warriors from the Goma
  /// Range saved the village."
  /// (MsgKolimaInteriorsIHeardThatTravelingWarriorsFrom)
  ACTOR_OLD_MAN_HEARD_OF_WARRIORS = ACTOR_FIRST_PLACED,
  /// An old woman.
  /// ★ Named from the game: "Big, strong warriors with great beards freed
  /// Kolima from the curse."
  /// (MsgKolimaInteriorsBigStrongWarriorsWithGreatBeards)
  ACTOR_OLD_WOMAN_BEARDED_WARRIORS,
  /// The father with a secret, a burly grey-haired man: "You saved Kolima? Wow!
  /// That makes me want to tell you a secret!" His thoughts: "It's hidden deep
  /// in the forest, but I can't tell him that!"
  /// ★ Named from the game: "That makes me want to tell you a secret!"
  /// (MsgKolimaInteriorsYouSavedKolimaWowThatMakes)
  ACTOR_FATHER_WITH_A_SECRET,
  /// His wife, a woman in a green dress.
  /// ★ Named from the game: "My husband seems to be keeping a secret from me."
  /// (MsgKolimaInteriorsMyHusbandSeemsToBeKeeping)
  ACTOR_WIFE_KEPT_FROM_THE_SECRET,
  /// Their son, a small boy with fair hair.
  /// ★ Named from the game: "Daddy told me his secret."
  /// (MsgKolimaInteriorsDaddyToldMeHisSecret)
  ACTOR_CHILD_TOLD_THE_SECRET,
  /// A strongman, stocky and muscular.
  /// ★ Named from the game: "I'm never going to use an axe again."
  /// (MsgKolimaInteriorsImNeverGoingToUseAn)
  ACTOR_LUMBERJACK_FATHER,
  /// His daughter, a woman with long fair hair.
  /// ★ Named from the game: "Dad, will you please go back to being a
  /// lumberjack?" (MsgKolimaInteriorsDadWillYouPleaseGoBack)
  ACTOR_LUMBERJACKS_DAUGHTER,
  /// His son, a small boy with fair hair.
  /// ★ Named from the game: "Dad! How are we going to eat if you don't work?"
  /// (MsgKolimaInteriorsDadHowAreWeGoingTo)
  ACTOR_LUMBERJACKS_HUNGRY_SON,
  /// ★ Named from the game: "I used to carve weapons out of wood."
  /// (MsgKolimaInteriorsIUsedToCarveWeaponsOut)
  ACTOR_WEAPON_MERCHANT,
  /// ★ Named from the game: "Ask me anything about armor."
  /// (MsgArmorShopWelcome)
  ACTOR_ARMOR_MERCHANT,
  /// ★ Named from the game: "Welcome! We have just the gear you need."
  /// (MsgItemShopWelcome)
  ACTOR_ITEM_MERCHANT,
  /// ★ Named from the game: "Welcome to our inn." (MsgInnWelcome)
  ACTOR_INNKEEPER,
  /// A woman in a pink dress and an apron.
  /// ★ Named from the game: "With no more work here, Kolima can go back to
  /// being a quiet little village."
  /// (MsgKolimaInteriorsWithNoMoreWorkHereKolima)
  ACTOR_VILLAGER_QUIET_VILLAGE,
  /// A strongman, stocky and muscular.
  /// ★ Named from the game: "I've been a lumberjack for years"
  /// (MsgKolimaInteriorsIveBeenALumberjackForYears)
  ACTOR_LUMBERJACK_FROM_IMIL,
  /// A man carrying a pack.
  /// ★ Named from the game: "I'm only a merchant, but I got turned into a
  /// tree just for staying here." (MsgKolimaInteriorsImOnlyAMerchantButI)
  ACTOR_MERCHANT_GUEST,
  /// ★ Named from the game: "I need to help evacuate the other villagers."
  /// (MsgValeKyleAbleStop)
  ACTOR_FIRST_VILLAGER = ACTOR_OLD_MAN_HEARD_OF_WARRIORS,
  /// ★ Named from the game: "I need to help evacuate the other villagers."
  /// (MsgValeKyleAbleStop)
  ACTOR_LAST_VILLAGER = ACTOR_MERCHANT_GUEST
};

/// Replaces the cursed residents' sprites and disables their movement and
/// turning.
static void LogRoomsC_TurnVillagersIntoTrees(struct ActorTemplate *template) {
  u32 villagerIndex;
  u8 talkFacing;
  s32 behavior;
  u16 treeSprite;
  u16 alternateTreeSprite;

  villagerIndex = 0;
  talkFacing = TALK_FACING_KEEP;
  behavior = MOVEMENT_TYPE_STAND;
  treeSprite = SPRITE_TREE;
  alternateTreeSprite = SPRITE_SMALL_TREE;
  do {
    template->talkFacing = talkFacing;
    template->movement = behavior;
    template->sprite = treeSprite;
    // The two residents drawn as SPRITE_BOY_WITH_FAIR_HAIR take the small tree.
    if (villagerIndex == ACTOR_CHILD_TOLD_THE_SECRET - ACTOR_FIRST_VILLAGER ||
        villagerIndex == ACTOR_LUMBERJACKS_HUNGRY_SON - ACTOR_FIRST_VILLAGER) {
      template->sprite = alternateTreeSprite;
    }
    villagerIndex++;
    template++;
  } while (villagerIndex < ACTOR_LAST_VILLAGER - ACTOR_FIRST_VILLAGER + 1);
}

const struct SceneEntrance *LogRoomsC_GetEntrances(void) {
  return gLogRoomsC_Entrances;
}

const struct SceneExitRegion *LogRoomsC_GetRegions(void) { return NULL; }

/// The exits between the rooms, back to the village and into Kolima Tunnel.
const u32 *LogRoomsC_GetExits(void) { return gLogRoomsC_Exits; }

/// Kolima's residents, turned into trees first while the curse lasts.
const struct ActorTemplate *LogRoomsC_GetActorTemplates(void) {
  struct ActorTemplate *templates;

  if (GameFlag_Get(FLAG_KOLIMA_CURSE_LIFTED) == 0) {
    LogRoomsC_TurnVillagersIntoTrees(gLogRoomsC_ActorTemplates);
  }
  templates = gLogRoomsC_ActorTemplates;
  Scene_AssignViewFlags(templates);
  return templates;
}

/// Kolima's weapon shop, once the curse is lifted: spoken to across the counter
/// (facing north), the shop opens; from any other side, the seller says Fuchin
/// Temple is on the other side of the river.
static void LogRoomsC_TalkToWeaponMerchant(void) {
  u32 facing = Actor_GetById(ACTOR_PARTY_LEADER)->facing;

  FieldScript_Start();
  if (FACING_IS_NORTH(facing)) {
    Shop_Run(KOLIMA_WEAPON_SHOP, ACTOR_WEAPON_MERCHANT);
  } else {
    FieldScript_SetMessageId(MsgKolimaFuchinTempleOnOtherSide);
    FieldScript_ShowMessageAndWait(ACTOR_WEAPON_MERCHANT, 0);
  }
  FieldScript_End();
}

/// Kolima's armor shop, once the curse is lifted: across the counter the shop
/// opens; otherwise the seller calls the grand bridge across the river the
/// pride of Kolima.
static void LogRoomsC_TalkToArmorMerchant(void) {
  u32 facing = Actor_GetById(ACTOR_PARTY_LEADER)->facing;

  FieldScript_Start();
  if (FACING_IS_NORTH(facing)) {
    Shop_Run(KOLIMA_ARMOR_SHOP, ACTOR_ARMOR_MERCHANT);
  } else {
    FieldScript_SetMessageId(MsgKolimaGrandBridgeAcrossRiverPride);
    FieldScript_ShowMessageAndWait(ACTOR_ARMOR_MERCHANT, 0);
  }
  FieldScript_End();
}

/// Kolima's item shop, once the curse is lifted: across the counter the shop
/// opens; otherwise, "When I was a tree, I learned to appreciate the sun."
static void LogRoomsC_TalkToItemMerchant(void) {
  u32 facing = Actor_GetById(ACTOR_PARTY_LEADER)->facing;

  FieldScript_Start();
  if (FACING_IS_NORTH(facing)) {
    Shop_Run(KOLIMA_ITEM_SHOP, ACTOR_ITEM_MERCHANT);
  } else {
    FieldScript_SetMessageId(MsgKolimaWhenWasTreeLearnedAppreciate);
    FieldScript_ShowMessageAndWait(ACTOR_ITEM_MERCHANT, 0);
  }
  FieldScript_End();
}

/// Kolima's inn, once the curse is lifted: across the counter the inn opens;
/// otherwise the innkeeper says McCoy has halted the palace's construction, so
/// the lumberjacks who came for the work will leave.
static void LogRoomsC_TalkToInnkeeper(void) {
  u32 facing = Actor_GetById(ACTOR_PARTY_LEADER)->facing;

  FieldScript_Start();

  if (FACING_IS_NORTH(facing)) {
    Inn_CheckIn(KOLIMA_INN, ACTOR_INNKEEPER);
  } else {
    FieldScript_SetMessageId(MsgKolimaTheySayMccoyHaltedConstruction);
    FieldScript_ShowMessageAndWait(ACTOR_INNKEEPER, 0);
  }

  FieldScript_End();
}

/// The exits, the residents' words and thoughts, the shops, the bookcases, the
/// jar and the barrel.
const struct Trigger *LogRoomsC_GetTriggers(void) {
  return gLogRoomsC_Triggers;
}

/// Sets up the houses for the curse and the way they're entered. It opens with
/// a window transition, sets the trees' sprites while the curse lasts, and
/// draws the father's room for the way it is entered.
s32 LogRoomsC_Initialize(void) {
  struct FieldWork *work = gFieldWork;
  s32 actorId;
  u32 entrance;

  work->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);

  // The residents remain visible as trees, with their sprite flags cleared.
  if (GameFlag_Get(FLAG_KOLIMA_CURSE_LIFTED) == 0) {
    actorId = ACTOR_FIRST_VILLAGER;
    do {
      struct Actor *actor = Actor_GetById(actorId);

      actorId++;
      Actor_SetSpriteFlags(actor, 0);
    } while ((u32)actorId <= ACTOR_LAST_VILLAGER);
  }

  entrance = (u16)gSaveBlock.entrance;

  // LOG_ROOMS_C_HOUSE_FROM_KOLIMA_SOUTH uses one layout;
  // LOG_ROOMS_C_HOUSE_FROM_KOLIMA_NORTH and
  // LOG_ROOMS_C_FROM_KOLIMA_TUNNEL use the other. The unsigned packed
  // comparison below tests the low 16-bit difference.
  if ((s16)entrance == LOG_ROOMS_C_HOUSE_FROM_KOLIMA_SOUTH) {
    s32 height;
    s32 width;
    width = 13;
    height = 8;
    MapGrid_CopyCellsTo(34, 34, 18, 16, width, height);
    MapGrid_CopyCellsTo(34, 94, 18, 76, width, height);
    MapGrid_CopyCellsTo(94, 34, 78, 16, width, height);
  } else if (((entrance - LOG_ROOMS_C_HOUSE_FROM_KOLIMA_NORTH) << 16) <=
             (1u << 16)) {
    s32 height;
    s32 width;
    width = 11;
    height = 8;
    MapGrid_CopyCellsTo(34, 43, 19, 23, width, height);
    MapGrid_CopyCellsTo(34, 94, 19, 83, width, height);
    MapGrid_CopyCellsTo(94, 34, 79, 23, width, height);
    Import_ActorSetPosition(ACTOR_FATHER_WITH_A_SECRET, 0, 0);
    Import_ActorSetPosition(ACTOR_WIFE_KEPT_FROM_THE_SECRET, 0, 0);
    Import_ActorSetPosition(ACTOR_CHILD_TOLD_THE_SECRET, 0, 0);
  }
  return 0;
}

struct SceneEntrance gLogRoomsC_Entrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {LOG_ROOMS_C_WEAPON_AND_ARMOR_SHOP_SECOND_FLOOR_FROM_WEAPON_AND_ARMOR_SHOP,
     CONDITION_ALWAYS, 128, 0, 192, FACING_EAST, 0, 16, 56, 256, 216, 0},
    {LOG_ROOMS_C_WEAPON_AND_ARMOR_SHOP_SECOND_FLOOR_FROM_WEAPON_AND_ARMOR_SHOP_THIRD_FLOOR,
     CONDITION_ALWAYS, 192, 0, 168, FACING_SOUTHWEST, 0, 16, 56, 256, 216, 0},
    {LOG_ROOMS_C_ITEM_SHOP_FROM_KOLIMA, CONDITION_ALWAYS, 392, 0, 200,
     FACING_NORTH, 0, 288, 64, 512, 232, 0},
    {LOG_ROOMS_C_INN_FROM_KOLIMA, CONDITION_ALWAYS, 648, 0, 200, FACING_NORTH,
     0, 528, 64, 768, 232, 0},
    {LOG_ROOMS_C_INN_FROM_INN_SECOND_FLOOR, CONDITION_ALWAYS, 704, 0, 168,
     FACING_SOUTHWEST, 0, 528, 64, 768, 224, 0},
    {LOG_ROOMS_C_INN_THIRD_FLOOR_FROM_INN_SECOND_FLOOR, CONDITION_ALWAYS, 128,
     0, 416, FACING_EAST, 0, 16, 280, 256, 440, 0},
    {LOG_ROOMS_C_HOUSE_FROM_KOLIMA_SOUTH, CONDITION_ALWAYS, 392, 0, 472,
     FACING_NORTH, 0, 288, 264, 512, 504, 0},
    {LOG_ROOMS_C_HOUSE_FROM_KOLIMA_NORTH, CONDITION_ALWAYS, 392, 0, 328,
     FACING_SOUTH, 0, 288, 264, 512, 496, 0},
    {LOG_ROOMS_C_FROM_KOLIMA_TUNNEL, CONDITION_ALWAYS, 392, 0, 344,
     FACING_NORTH, 0, 288, 264, 512, 496, 0},
    {LOG_ROOMS_C_WEAPON_AND_ARMOR_SHOP_FROM_KOLIMA, CONDITION_ALWAYS, 648, 0,
     424, FACING_NORTH, 0, 528, 288, 768, 456, 0},
    {LOG_ROOMS_C_WEAPON_AND_ARMOR_SHOP_FROM_WEAPON_AND_ARMOR_SHOP_SECOND_FLOOR,
     CONDITION_ALWAYS, 704, 0, 392, FACING_SOUTHWEST, 0, 528, 288, 768, 448, 0},
    {LOG_ROOMS_C_INN_SECOND_FLOOR_FROM_INN, CONDITION_ALWAYS, 128, 0, 688,
     FACING_EAST, 0, 16, 552, 256, 712, 0},
    {LOG_ROOMS_C_INN_SECOND_FLOOR_FROM_INN_THIRD_FLOOR, CONDITION_ALWAYS, 192,
     0, 664, FACING_SOUTHWEST, 0, 16, 552, 256, 712, 0},
    {LOG_ROOMS_C_WEAPON_AND_ARMOR_SHOP_THIRD_FLOOR_FROM_WEAPON_AND_ARMOR_SHOP_SECOND_FLOOR,
     CONDITION_ALWAYS, 400, 0, 688, FACING_NORTH, 0, 288, 552, 528, 712, 0},
    {SCENE_TABLE_END},
};

u32 gLogRoomsC_Exits[] = {
    SCENE_EXITS(LogRoomsC),
    SCENE_EXIT(
        LOG_ROOMS_C_WEAPON_AND_ARMOR_SHOP_SECOND_FLOOR_TO_WEAPON_AND_ARMOR_SHOP,
        LogRoomsC,
        LOG_ROOMS_C_WEAPON_AND_ARMOR_SHOP_FROM_WEAPON_AND_ARMOR_SHOP_SECOND_FLOOR),
    SCENE_EXIT(
        LOG_ROOMS_C_WEAPON_AND_ARMOR_SHOP_SECOND_FLOOR_TO_WEAPON_AND_ARMOR_SHOP_THIRD_FLOOR,
        LogRoomsC,
        LOG_ROOMS_C_WEAPON_AND_ARMOR_SHOP_THIRD_FLOOR_FROM_WEAPON_AND_ARMOR_SHOP_SECOND_FLOOR),
    SCENE_EXIT(LOG_ROOMS_C_ITEM_SHOP_TO_KOLIMA, Kolima, KOLIMA_FROM_ITEM_SHOP),
    SCENE_EXIT(LOG_ROOMS_C_INN_TO_KOLIMA, Kolima, KOLIMA_FROM_INN),
    SCENE_EXIT(LOG_ROOMS_C_INN_TO_INN_SECOND_FLOOR, LogRoomsC,
               LOG_ROOMS_C_INN_SECOND_FLOOR_FROM_INN),
    SCENE_EXIT(LOG_ROOMS_C_INN_THIRD_FLOOR_TO_INN_SECOND_FLOOR, LogRoomsC,
               LOG_ROOMS_C_INN_SECOND_FLOOR_FROM_INN_THIRD_FLOOR),
    SCENE_EXIT(LOG_ROOMS_C_HOUSE_TO_KOLIMA_SOUTH, Kolima,
               KOLIMA_FROM_HOUSE_SOUTH),
    SCENE_EXIT(LOG_ROOMS_C_HOUSE_TO_KOLIMA_NORTH, Kolima,
               KOLIMA_FROM_HOUSE_NORTH),
    SCENE_EXIT(LOG_ROOMS_C_WEAPON_AND_ARMOR_SHOP_TO_KOLIMA, Kolima,
               KOLIMA_FROM_WEAPON_AND_ARMOR_SHOP),
    SCENE_EXIT(
        LOG_ROOMS_C_WEAPON_AND_ARMOR_SHOP_TO_WEAPON_AND_ARMOR_SHOP_SECOND_FLOOR,
        LogRoomsC,
        LOG_ROOMS_C_WEAPON_AND_ARMOR_SHOP_SECOND_FLOOR_FROM_WEAPON_AND_ARMOR_SHOP),
    SCENE_EXIT(LOG_ROOMS_C_INN_SECOND_FLOOR_TO_INN, LogRoomsC,
               LOG_ROOMS_C_INN_FROM_INN_SECOND_FLOOR),
    SCENE_EXIT(LOG_ROOMS_C_INN_SECOND_FLOOR_TO_INN_THIRD_FLOOR, LogRoomsC,
               LOG_ROOMS_C_INN_THIRD_FLOOR_FROM_INN_SECOND_FLOOR),
    SCENE_EXIT(
        LOG_ROOMS_C_WEAPON_AND_ARMOR_SHOP_THIRD_FLOOR_TO_WEAPON_AND_ARMOR_SHOP_SECOND_FLOOR,
        LogRoomsC,
        LOG_ROOMS_C_WEAPON_AND_ARMOR_SHOP_SECOND_FLOOR_FROM_WEAPON_AND_ARMOR_SHOP_THIRD_FLOOR),
    SCENE_EXIT(LOG_ROOMS_C_TO_KOLIMA_TUNNEL, KolimaTunnel,
               KOLIMA_TUNNEL_FROM_LOG_ROOMS_C),
    SCENE_EXITS_END,
};

// clang-format off
struct ActorTemplate gLogRoomsC_ActorTemplates[] = {
    { SPRITE_OLD_MAN_WITH_CANE, 0, MOVEMENT_TYPE_WANDER,
        PIXELS(166), 0, PIXELS(184), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_WOMAN, 0, MOVEMENT_TYPE_STAND,
        PIXELS(88), 0, PIXELS(144), FACING_EAST, 3, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, 0, MOVEMENT_TYPE_STAND,
        PIXELS(408), 0, PIXELS(472), FACING_WEST, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, 0, MOVEMENT_TYPE_STAND,
        PIXELS(360), 0, PIXELS(440), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_BOY_WITH_FAIR_HAIR, 0, 3,
        PIXELS(440), 0, PIXELS(456), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_STRONGMAN, 0, MOVEMENT_TYPE_STAND,
        PIXELS(376), 0, PIXELS(616), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, 0, MOVEMENT_TYPE_STAND,
        PIXELS(448), 0, PIXELS(680), FACING_WEST, TALK_FACING_PARTY, 0 },
    { SPRITE_BOY_WITH_FAIR_HAIR, 0, MOVEMENT_TYPE_WANDER,
        PIXELS(424), 0, PIXELS(648), FACING_WEST, TALK_FACING_PARTY, 0 },
    { SPRITE_BALD_MAN_IN_RED, 0, MOVEMENT_TYPE_STAND,
        PIXELS(624), 0, PIXELS(360), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_IN_GREEN, 0, MOVEMENT_TYPE_STAND,
        PIXELS(648), 0, PIXELS(360), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_WITH_PONYTAIL, 0, MOVEMENT_TYPE_STAND,
        PIXELS(392), 0, PIXELS(136), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_MAN_WITH_DARK_HAIR, 0, MOVEMENT_TYPE_STAND,
        PIXELS(648), 0, PIXELS(136), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_IN_PINK_APRON, 0, MOVEMENT_TYPE_WANDER,
        PIXELS(624), 0, PIXELS(200), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_STRONGMAN, 0, MOVEMENT_TYPE_WANDER,
        PIXELS(120), 0, PIXELS(656), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_CARRYING_A_PACK, 0, MOVEMENT_TYPE_STAND,
        PIXELS(144), 0, PIXELS(368), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct Trigger gLogRoomsC_Triggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, LOG_ROOMS_C_WEAPON_AND_ARMOR_SHOP_SECOND_FLOOR_TO_WEAPON_AND_ARMOR_SHOP },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, LOG_ROOMS_C_WEAPON_AND_ARMOR_SHOP_SECOND_FLOOR_TO_WEAPON_AND_ARMOR_SHOP_THIRD_FLOOR },
    { TRIGGER_EXIT, 3, CONDITION_ALWAYS, LOG_ROOMS_C_ITEM_SHOP_TO_KOLIMA },
    { TRIGGER_EXIT, 4, CONDITION_ALWAYS, LOG_ROOMS_C_INN_TO_KOLIMA },
    { TRIGGER_EXIT, 5, CONDITION_ALWAYS, LOG_ROOMS_C_INN_TO_INN_SECOND_FLOOR },
    { TRIGGER_EXIT, 6, CONDITION_ALWAYS, LOG_ROOMS_C_INN_THIRD_FLOOR_TO_INN_SECOND_FLOOR },
    { TRIGGER_EXIT, 7, CONDITION_ALWAYS, LOG_ROOMS_C_HOUSE_TO_KOLIMA_SOUTH },
    { TRIGGER_EXIT, 8, CONDITION_ALWAYS, LOG_ROOMS_C_HOUSE_TO_KOLIMA_NORTH },
    { TRIGGER_EXIT, 9, CONDITION_ALWAYS, LOG_ROOMS_C_WEAPON_AND_ARMOR_SHOP_TO_KOLIMA },
    { TRIGGER_EXIT, 10, CONDITION_ALWAYS, LOG_ROOMS_C_WEAPON_AND_ARMOR_SHOP_TO_WEAPON_AND_ARMOR_SHOP_SECOND_FLOOR },
    { TRIGGER_EXIT, 11, CONDITION_ALWAYS, LOG_ROOMS_C_INN_SECOND_FLOOR_TO_INN },
    { TRIGGER_EXIT, 12, CONDITION_ALWAYS, LOG_ROOMS_C_INN_SECOND_FLOOR_TO_INN_THIRD_FLOOR },
    { TRIGGER_EXIT, 13, CONDITION_ALWAYS, LOG_ROOMS_C_WEAPON_AND_ARMOR_SHOP_THIRD_FLOOR_TO_WEAPON_AND_ARMOR_SHOP_SECOND_FLOOR },
    { TRIGGER_EXIT, 14, CONDITION_ALWAYS, LOG_ROOMS_C_TO_KOLIMA_TUNNEL },
    { TRIGGER_TALK, ACTOR_OLD_MAN_HEARD_OF_WARRIORS, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsIHeardThatTravelingWarriorsFrom) },
    { TRIGGER_TALK, ACTOR_OLD_WOMAN_BEARDED_WARRIORS, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsBigStrongWarriorsWithGreatBeards) },
    { TRIGGER_TALK, ACTOR_FATHER_WITH_A_SECRET, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsYouSavedKolimaWowThatMakes) },
    { TRIGGER_TALK, ACTOR_WIFE_KEPT_FROM_THE_SECRET, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsMyHusbandSeemsToBeKeeping) },
    { TRIGGER_TALK, ACTOR_CHILD_TOLD_THE_SECRET, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsDaddyToldMeHisSecret) },
    { TRIGGER_TALK, ACTOR_LUMBERJACK_FATHER, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsImNeverGoingToUseAn) },
    { TRIGGER_TALK, ACTOR_LUMBERJACKS_DAUGHTER, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsDadWillYouPleaseGoBack) },
    { TRIGGER_TALK, ACTOR_LUMBERJACKS_HUNGRY_SON, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsDadHowAreWeGoingTo) },
    { TRIGGER_TALK, ACTOR_WEAPON_MERCHANT, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), EVENT_SCRIPT(LogRoomsC_TalkToWeaponMerchant) },
    { TRIGGER_TALK, ACTOR_ARMOR_MERCHANT, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), EVENT_SCRIPT(LogRoomsC_TalkToArmorMerchant) },
    { TRIGGER_TALK, ACTOR_ITEM_MERCHANT, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), EVENT_SCRIPT(LogRoomsC_TalkToItemMerchant) },
    { TRIGGER_TALK, ACTOR_INNKEEPER, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), EVENT_SCRIPT(LogRoomsC_TalkToInnkeeper) },
    { TRIGGER_TALK, ACTOR_VILLAGER_QUIET_VILLAGE, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsWithNoMoreWorkHereKolima) },
    { TRIGGER_TALK, ACTOR_LUMBERJACK_FROM_IMIL, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsIveBeenALumberjackForYears) },
    { TRIGGER_TALK, ACTOR_MERCHANT_GUEST, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsImOnlyAMerchantButI) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_HEARD_OF_WARRIORS, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsIDontReallyKnowButYou) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_BEARDED_WARRIORS, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsAllWarriorsAreBigAndAll) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FATHER_WITH_A_SECRET, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsItsHiddenDeepInTheForest) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WIFE_KEPT_FROM_THE_SECRET, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsIThinkHeFoundSomethingValuable) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHILD_TOLD_THE_SECRET, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsStillTellingMeItsOutsideThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LUMBERJACK_FATHER, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsPeopleTurnedIntoTreesBecauseOf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LUMBERJACKS_DAUGHTER, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsKnowingMyDadHellStartWorking) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LUMBERJACKS_HUNGRY_SON, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsHowAmISupposedToGrow) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsFuchinTemplesMasterNyunpaIsSaid) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsItsAShameWeHadTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANT, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsItFeltSoGoodWhenMy) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsTheVillageWasReallyBoomingWith) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_QUIET_VILLAGE, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsRemainingAQuietVillageMeansRemaining) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LUMBERJACK_FROM_IMIL, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsImALumberjackAndImOkay) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MERCHANT_GUEST, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsIWishIdGrabbedSomeOf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_HEARD_OF_WARRIORS, CONDITION_ALWAYS, MSGNO(MsgKolimaInteriorsWhatAreTheyStaringAtThey) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_BEARDED_WARRIORS, CONDITION_ALWAYS, MSGNO(MsgKolimaInteriorsIDreamtThatAMightyWarrior) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FATHER_WITH_A_SECRET, CONDITION_ALWAYS, MSGNO(MsgKolimaInteriorsWhatWillHappenToMyTreasure) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WIFE_KEPT_FROM_THE_SECRET, CONDITION_ALWAYS, MSGNO(MsgKolimaInteriorsSoonAllOfKolimaWillBe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHILD_TOLD_THE_SECRET, CONDITION_ALWAYS, MSGNO(MsgKolimaInteriorsIWonderWhatFathersSecretIs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LUMBERJACK_FATHER, CONDITION_ALWAYS, MSGNO(MsgKolimaInteriorsMaybeWeTurnedIntoTreesBecause) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LUMBERJACKS_DAUGHTER, CONDITION_ALWAYS, MSGNO(MsgKolimaInteriorsAtLeastMyFatherWasntThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LUMBERJACKS_HUNGRY_SON, CONDITION_ALWAYS, MSGNO(MsgKolimaInteriorsTheHolyTreeOfKolimaHas) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgKolimaInteriorsIUsedToCarveWeaponsOut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgKolimaInteriorsManImStuckInsideThatMeans) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgKolimaInteriorsNoIDontWantToBe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgKolimaInteriorsWhoCausedAllThisSomeoneMust) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_QUIET_VILLAGE, CONDITION_ALWAYS, MSGNO(MsgKolimaInteriorsHowAmISupposedToCharge) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LUMBERJACK_FROM_IMIL, CONDITION_ALWAYS, MSGNO(MsgKolimaInteriorsTheVillagersWereRightINever) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MERCHANT_GUEST, CONDITION_ALWAYS, MSGNO(MsgKolimaInteriorsIfOnlyIdLeftTownA) },
    { EXAMINE_TRIGGER(EXAMINE_JAR), 100, 0xf59, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), 101, 0xf5a, FIND_COINS(7) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKolimaInteriorsTheresABookEntitledTheHoly)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKolimaInteriorsTheresABookEntitledDictionaryOf)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKolimaInteriorsTheresABookEntitledTheArt)) },
    { TRIGGERS_END },
};
// clang-format on
