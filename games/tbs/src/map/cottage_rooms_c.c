/// @file
/// Inside Bilibin's buildings: MapId_CottageRoomsC, whose area name is
/// Bilibin. Six of Bilibin's doors (BILIBIN_TO_INN to
/// BILIBIN_TO_WEAPON_AND_ARMOR_SHOP, in bilibin.c) lead into its rooms, and
/// each room's door leads back out to the door it was entered by. One room
/// holds the weapon and armor shops, another the item shop, a third the inn
/// with the restaurant beside it, whose stairs
/// (COTTAGE_ROOMS_C_INN_TO_INN_UPSTAIRS and
/// COTTAGE_ROOMS_C_INN_UPSTAIRS_TO_INN) join it to its upstairs rooms; three
/// other buildings hold villagers. CottageRoomsC_GetObjectEventTemplates
/// leaves out the object events of the rooms not in view.
///
/// Everyone has a line for while Kolima's curse holds (FLAG_KOLIMA_CURSE_LIFTED
/// clear) and one for after. Before, the townsfolk explain the tree at the
/// village entrance, McCoy's palace and the Water of Hermes, and in the
/// restaurant the head chef, born in Kolima, is too worried to cook. After, the
/// head chef cooks so hard his kitchen cannot keep up: the first time the party
/// talks to him (FLAG_BILIBIN_CHEF_CALLED_ORDERS, kept until the party leaves
/// Bilibin) he shouts a string of orders and the kitchen worker answers, and
/// from then on he sends the party to the waitress
/// (EventScript_TalkToBusyChef).
///
/// gCottageRoomsCEvents keeps both times in one table, in the ROM's order:
/// every row that needs FLAG_KOLIMA_CURSE_LIFTED clear comes before the rows
/// for afterwards, so the first row that matches picks the time. The merchants
/// and the innkeeper open the shop or the inn only when the leader faces them
/// across the counter (facing north), and otherwise talk of the time; Mind Read
/// on the villager who admires Mia depends on whether she is in the party.
/// Searching finds an Antidote, a Sleep Bomb and a Hard Nut, egg soup and
/// omelettes in the ovens, and a book called "All About Items".
/// CottageRoomsC_Initialize opens the rooms with a window around the leader
/// and draws one diner, LOCALID_BILIBIN_DINER, at a fixed sprite priority. He
/// stands at the north edge of the restaurant's table, on cell (7, 49), which
/// alone of the diners' cells gives an object standing on it priority 3
/// (layout_032's map.txt). Taken from that cell, his priority would
/// put him behind the middle layer (`BG2`, priority 2 in this map); the fixed
/// priority 2 draws him in front of it, as the other diners are.

#include "unit/map/cottage_rooms_c.h"
#include "flags.h"
#include "character_ids.h"
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
#include "map_tables.h"
#include "call.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "item_ids.h"

/// The head chef has shouted his string of orders once, so from then on he
/// sends the party to the waitress.
/// ★ Named from the game: "Two specials! One dinner! One sandwich! One dessert!
/// Hurry up!" (MsgBilibinTwoSpecialsOneDinnerOne)
#define FLAG_BILIBIN_CHEF_CALLED_ORDERS FLAG_MAPSEC_TEMP_00

/// Bilibin's shops, as Shop_Run numbers the shops.
enum {
  SHOP_BILIBIN_WEAPONS = 7,
  SHOP_BILIBIN_ARMOR = 8,
  SHOP_BILIBIN_ITEMS = 9
};

/// Bilibin's inn, as Inn_CheckIn numbers the inns.
enum { BILIBIN_INN = 2 };

/// The placed object events, numbered in the order of
/// gCottageRoomsCObjectEventTemplates from LOCALID_FIRST_PLACED. Each room's
/// object events are together: two in each house, then the shops' customers and
/// keepers, the inn and its restaurant, and the room behind the inn's inner
/// door.
enum CottageRoomsCLocalId {
  /// A young man with brown hair, in the house east.
  /// ★ Named from the game: "That tree at the entrance is really a man cursed"
  /// (MsgBilibinThatTreeAtTheEntranceIs)
  LOCALID_BILIBIN_YOUNG_MAN_ON_THE_CURSED_MAN = LOCALID_FIRST_PLACED,
  /// A woman with fair hair.
  /// ★ Named from the game: "McCoy had the trees cut down so he could build"
  /// (MsgBilibinMccoyHadTheTreesCutDown)
  LOCALID_BILIBIN_VILLAGER_PALACE,
  /// A burly grey-haired man in the house west, who wants to see Mia again.
  /// ★ Named from the game: "When spring comes, I want to go see that angelic"
  /// (MsgBilibinWhenSpringComesWantGo)
  LOCALID_BILIBIN_MIA_ADMIRER,
  /// A small boy with fair hair.
  /// ★ Named from the game: "We rarely see any snow around Bilibin."
  /// (MsgBilibinWeRarelySeeAnySnowAround)
  LOCALID_BILIBIN_BOY_SNOW,
  /// A woman in a green dress, in the house north.
  /// ★ Named from the game: "How are people doing in Kolima?"
  /// (MsgBilibinHowArePeopleDoingInKolima)
  LOCALID_BILIBIN_VILLAGER_WORRIED_FOR_KOLIMA,
  /// A little girl with pigtails, who later asks whether the party knows what
  /// silk is.
  /// ★ Named from the game: "I know that guy who turned into the tree..."
  /// (MsgBilibinIKnowThatGuyWhoTurned)
  LOCALID_BILIBIN_GIRL_SILK,
  /// An old man with a cane, in the item shop.
  /// ★ Named from the game: "Long ago, the lighthouse near Imil held the"
  /// (MsgBilibinLongAgoTheLighthouseNearImil)
  LOCALID_BILIBIN_OLD_MAN_WATER_OF_HERMES,
  /// An old woman.
  /// ★ Named from the game: "But even the legendary Water of Hermes cannot"
  /// (MsgBilibinButEvenTheLegendaryWaterOf)
  LOCALID_BILIBIN_OLD_WOMAN_LOST_YOUTH,
  /// ★ Named from the game: "There was an able healer in a village called
  /// Vale," (MsgBilibinThereWasAbleHealerIn)
  LOCALID_BILIBIN_WEAPON_MERCHANT,
  /// ★ Named from the game: "Have you ever heard of the 'ocean'?"
  /// (MsgBilibinHaveEverHeardOcean)
  LOCALID_BILIBIN_ARMOR_MERCHANT,
  /// ★ Named from the game: "You must be used to all types of danger,"
  /// (MsgBilibinMustUsedTypesDangerBeing)
  LOCALID_BILIBIN_ITEM_MERCHANT,
  /// ★ Named from the game: "A curse on Kolima? What a scary development!"
  /// (MsgBilibinCurseOnKolimaScaryDevelopment)
  LOCALID_BILIBIN_INNKEEPER,
  /// Beside the innkeeper.
  /// ★ Named from the game: "You can't reach Kolima, no matter how you try."
  /// (MsgBilibinYouCantReachKolimaNoMatter)
  LOCALID_BILIBIN_INN_WORKER,
  /// The restaurant's head chef, born in Kolima.
  /// ★ Named from the game: "Hey, Mister Head Chef!"
  /// (MsgBilibinHeyMisterHeadChefAreYou)
  LOCALID_BILIBIN_CHEF,
  /// A second cook in a tall chef's hat.
  /// ★ Named from the game: "I really think the head chef has changed."
  /// (MsgBilibinReallyThinkHeadChefHas)
  LOCALID_BILIBIN_KITCHEN_WORKER,
  /// A young woman with long brown hair.
  /// ★ Named from the game: "Let's see, serve them water...then get the
  /// silverware." (MsgBilibinLetsSeeServeThemWater)
  LOCALID_BILIBIN_WAITRESS,
  /// A man with a pack, waiting for his dinner.
  /// ★ Named from the game: "Grrr..." (MsgBilibinGrrr)
  LOCALID_BILIBIN_HUNGRY_DINER,
  /// A diner with a pack.
  /// ★ Named from the game: "I came to Bilibin hoping to sell furniture."
  /// (MsgBilibinICameToBilibinHopingTo)
  LOCALID_BILIBIN_FURNITURE_SELLER,
  /// A strongman at the table.
  /// ★ Named from the game: "I think that big tree deep in the forest"
  /// (MsgBilibinIThinkThatBigTreeDeep)
  LOCALID_BILIBIN_DINER_BIG_TREE,
  /// A diner with a pickaxe, drawn at a fixed sprite priority.
  /// ★ Named from the game: "Do you believe a tree spirit can curse a man?"
  /// (MsgBilibinDoBelieveTreeSpiritCan)
  LOCALID_BILIBIN_DINER,
  /// An old man at the table.
  /// ★ Named from the game: "I can't wait to see today's special!"
  /// (MsgBilibinICantWaitToSeeTodays)
  LOCALID_BILIBIN_DINER_TODAYS_SPECIAL,
  /// A man in a red bandana, in the room behind the inn's inner door.
  /// ★ Named from the game: "I went to Lord McCoy looking for work..."
  /// (MsgBilibinIWentToLordMccoyLooking)
  LOCALID_BILIBIN_JOB_SEEKER,
  /// A man with a lute.
  /// ★ Named from the game: "I've heard songs that tell of ancient trees"
  /// (MsgBilibinIveHeardSongsThatTellOf)
  LOCALID_BILIBIN_VILLAGER_ANCIENT_TREES
};

const struct MapWarpDest *CottageRoomsC_GetWarpDests(void) {
  return gCottageRoomsCWarpDests;
}

const struct MapWarpRegion *CottageRoomsC_GetRegions(void) { return 0; }

const u32 *CottageRoomsC_GetWarps(void) { return gCottageRoomsCWarps; }

/// Marks object event templates outside the current room's view before object
/// events are spawned.
const struct ObjectEventTemplate *CottageRoomsC_GetObjectEventTemplates(void) {
  Map_AssignViewFlags(gCottageRoomsCObjectEventTemplates);
  return gCottageRoomsCObjectEventTemplates;
}

/// Bilibin's weapon merchant: across the counter the shop opens; otherwise,
/// before the curse is lifted, the merchant mourns Vale's lost healer, and
/// after, recommends Altin's store.
static void EventScript_TalkToWeaponMerchant(void) {
  u32 leaderFacing;

  leaderFacing = ObjectEvent_GetById(LOCALID_PARTY_LEADER)->facing;
  FieldScript_Start();

  if (FACING_IS_NORTH(leaderFacing)) {
    Shop_Run(SHOP_BILIBIN_WEAPONS, LOCALID_BILIBIN_WEAPON_MERCHANT);
  } else {
    if (GameFlag_Get(FLAG_KOLIMA_CURSE_LIFTED) == 0) {
      FieldScript_SetMessageId(MsgBilibinThereWasAbleHealerIn);
    } else {
      FieldScript_SetMessageId(MsgBilibinOurWeaponsBestCanFind);
    }
    FieldScript_ShowMessageAndWait(LOCALID_BILIBIN_WEAPON_MERCHANT, 0);
  }

  FieldScript_End();
}

/// Bilibin's item merchant: across the counter the shop opens; otherwise,
/// before the curse is lifted, the merchant says Lord McCoy could use
/// travelers like the party, and after, that his hired warriors were useless.
static void EventScript_TalkToItemMerchant(void) {
  u32 leaderFacing;

  leaderFacing = ObjectEvent_GetById(LOCALID_PARTY_LEADER)->facing;
  FieldScript_Start();

  if (FACING_IS_NORTH(leaderFacing)) {
    Shop_Run(SHOP_BILIBIN_ITEMS, LOCALID_BILIBIN_ITEM_MERCHANT);
  } else {
    if (GameFlag_Get(FLAG_KOLIMA_CURSE_LIFTED) == 0) {
      FieldScript_SetMessageId(MsgBilibinMustUsedTypesDangerBeing);
    } else {
      FieldScript_SetMessageId(MsgBilibinTurnedOutWarriorsHiredBy);
    }
    FieldScript_ShowMessageAndWait(LOCALID_BILIBIN_ITEM_MERCHANT, 0);
  }

  FieldScript_End();
}

/// Returns both story stages' map events; conditions choose the current
/// dialogue.
const struct MapEvent *CottageRoomsC_GetEvents(void) {
  return gCottageRoomsCEvents;
}

/// Bilibin's armor merchant: across the counter the shop opens; otherwise,
/// before the curse is lifted, the merchant asks whether the party has heard
/// of the ocean, and after, dreams aloud of it.
static void EventScript_TalkToArmorMerchant(void) {
  u32 leaderFacing;

  leaderFacing = ObjectEvent_GetById(LOCALID_PARTY_LEADER)->facing;
  FieldScript_Start();
  if (FACING_IS_NORTH(leaderFacing)) {
    Shop_Run(SHOP_BILIBIN_ARMOR, LOCALID_BILIBIN_ARMOR_MERCHANT);
  } else {
    if (GameFlag_Get(FLAG_KOLIMA_CURSE_LIFTED) == 0) {
      FieldScript_SetMessageId(MsgBilibinHaveEverHeardOcean);
      ObjectEvent_FaceObjectEvent(LOCALID_BILIBIN_ARMOR_MERCHANT,
                                  LOCALID_PARTY_LEADER, 0);
      FieldScript_WaitFrames(10);
      Script_AskYesNo(LOCALID_BILIBIN_ARMOR_MERCHANT, 0);
      ObjectEvent_FaceDirection(LOCALID_BILIBIN_ARMOR_MERCHANT,
                                FACING_SOUTHEAST + FACING_STEP, 10);
    } else {
      FieldScript_SetMessageId(MsgBilibinForSomeReasonOceanFills);
      Script_ShowMessage(LOCALID_BILIBIN_ARMOR_MERCHANT, 0);
    }
  }
  FieldScript_End();
}

/// The head chef in Bilibin's restaurant, before the curse is lifted: born in
/// Kolima, he is too worried to focus on his cooking.
static void EventScript_TalkToWorriedChef(void) {
  FieldScript_Start();
  FieldScript_SetMessageId(MsgBilibinImReallyWorriedAboutKolima);
  ObjectEvent_FaceObjectEvent(LOCALID_BILIBIN_CHEF, LOCALID_PARTY_LEADER, 0);
  Script_ShowMessage(LOCALID_BILIBIN_CHEF, 0);
  ObjectEvent_FaceDirection(LOCALID_BILIBIN_CHEF, FACING_NORTH, 10);
  FieldScript_End();
}

/// A diner in Bilibin's restaurant, before the curse is lifted, still waiting
/// for his dinner: his stomach growls, and he asks whether the party heard it.
static void EventScript_TalkToHungryDiner(void) {
  FieldScript_Start();
  FieldScript_SetMessageId(MsgBilibinGrrr);
  Script_ShowMessageAndWait(LOCALID_BILIBIN_HUNGRY_DINER, 0, 20);
  ObjectEvent_FaceObjectEvent(LOCALID_BILIBIN_HUNGRY_DINER,
                              LOCALID_PARTY_LEADER, 0);
  FieldScript_WaitFrames(10);
  Script_OpenMessage(LOCALID_BILIBIN_HUNGRY_DINER, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) != 0) {
    gFieldWork->message++;
  }
  Script_ShowMessage(LOCALID_BILIBIN_HUNGRY_DINER, 0);
  ObjectEvent_FaceDirection(LOCALID_BILIBIN_HUNGRY_DINER, FACING_SOUTH, 10);
  FieldScript_End();
}

/// A diner asks whether a tree spirit can curse a man. The diner sits in
/// Bilibin's restaurant before the curse is lifted; on yes, sends the party to
/// hear Lord McCoy's story. Then the diner turns back to the table.
static void EventScript_AskAboutTreeSpirit(void) {
  FieldScript_Start();
  FieldScript_SetMessageId(MsgBilibinDoBelieveTreeSpiritCan);
  ObjectEvent_FaceObjectEvent(LOCALID_BILIBIN_DINER, LOCALID_PARTY_LEADER, 0);
  FieldScript_WaitFrames(10);
  Script_OpenMessage(LOCALID_BILIBIN_DINER, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) != 0) {
    gFieldWork->message++;
  }
  Script_ShowMessage(LOCALID_BILIBIN_DINER, 0);
  ObjectEvent_FaceDirection(LOCALID_BILIBIN_DINER, FACING_SOUTH, 10);
  FieldScript_End();
}

/// A young man asks whether Kolima Forest's holy tree survived. He's in Bilibin
/// once the curse is lifted.
static void EventScript_AskAboutHolyTree(void) {
  FieldScript_Start();
  FieldScript_SetMessageId(MsgBilibinDoKnowIfHolyTree);
  FieldScript_AskYesNoAndShowResponse(
      LOCALID_BILIBIN_YOUNG_MAN_ON_THE_CURSED_MAN, 0);
  FieldScript_End();
}

/// A little girl in Bilibin, once the curse is lifted, asks whether the party
/// knows what silk is; on yes, that Xian, across the river, makes it, at the
/// start of Silk Road.
static void EventScript_AskAboutSilk(void) {
  FieldScript_Start();
  FieldScript_SetMessageId(MsgBilibinDoKnowSilk);
  FieldScript_AskYesNoAndShowResponse(LOCALID_BILIBIN_GIRL_SILK, 0);
  FieldScript_End();
}

/// Bilibin's innkeeper: across the counter the inn opens; otherwise, before
/// the curse is lifted, the innkeeper is scared of the curse on Kolima, and
/// after, asks whether it wasn't horrifying.
static void EventScript_TalkToInnkeeper(void) {
  u32 leaderFacing;

  leaderFacing = ObjectEvent_GetById(LOCALID_PARTY_LEADER)->facing;
  FieldScript_Start();

  if (FACING_IS_NORTH(leaderFacing)) {
    Inn_CheckIn(BILIBIN_INN, LOCALID_BILIBIN_INNKEEPER);
  } else if (GameFlag_Get(FLAG_KOLIMA_CURSE_LIFTED) != 0) {
    FieldScript_SetMessageId(MsgBilibinWasntCurseInKolimaHorrifying);
    FieldScript_AskYesNoAndShowResponse(LOCALID_BILIBIN_INNKEEPER, 0);
  } else {
    FieldScript_SetMessageId(MsgBilibinCurseOnKolimaScaryDevelopment);
    FieldScript_ShowMessageAndWait(LOCALID_BILIBIN_INNKEEPER, 0);
  }

  FieldScript_End();
}

/// The head chef in Bilibin's restaurant, once the curse is lifted: he calls
/// out orders once, then sends the party to the waitress.
static void EventScript_TalkToBusyChef(void) {
  FieldScript_Start();
  // The test is the script layer's inline and the set below a direct call,
  // each as it matches: an inline call's argument is a pseudo of its own,
  // where a direct call keeps the flag number in a callee-saved register.
  if (Script_GetFlag(FLAG_BILIBIN_CHEF_CALLED_ORDERS) == 0) {
    // The first time (FLAG_BILIBIN_CHEF_CALLED_ORDERS, kept until the party
    // leaves Bilibin), the chef shouts orders and the kitchen worker answers.
    FieldScript_SetMessageId(MsgBilibinTwoSpecialsOneDinnerOne);
    Script_ShowMessage(LOCALID_BILIBIN_CHEF, 0);
    ObjectEvent_FaceDirection(LOCALID_BILIBIN_CHEF, FACING_WEST, 20);
    Script_ShowMessage(LOCALID_BILIBIN_CHEF, 0);
    ObjectMotion_SetVariantCallback(LOCALID_BILIBIN_KITCHEN_WORKER, 2);
    ObjectEvent_SetAttachedEffect(LOCALID_BILIBIN_KITCHEN_WORKER,
                                  ATTACHED_EFFECT_IN_FRONT |
                                      ATTACHED_EFFECT_ONCE);
    FieldScript_WaitFrames(60);
    Script_ShowMessage(LOCALID_BILIBIN_KITCHEN_WORKER, 0);
    FieldScript_WaitFrames(10);
    GameFlag_Set(FLAG_BILIBIN_CHEF_CALLED_ORDERS);
  }
  // Then, as every later time, he sends the party to the waitress.
  ObjectEvent_FaceObjectEvent(LOCALID_BILIBIN_CHEF, LOCALID_PARTY_LEADER, 0);
  FieldScript_SetMessageId(MsgBilibinIfWantMealSpeakWaitress);
  Script_ShowMessage(LOCALID_BILIBIN_CHEF, 0);
  ObjectEvent_FaceDirection(LOCALID_BILIBIN_CHEF, FACING_NORTH, 10);
  FieldScript_End();
}

/// The kitchen worker can't keep up with the head chef's energy. This is in
/// Bilibin's restaurant once the curse is lifted.
static void EventScript_TalkToKitchenWorker(void) {
  FieldScript_Start();
  FieldScript_SetMessageId(MsgBilibinReallyThinkHeadChefHas);
  FieldScript_ShowMessageAndWait(LOCALID_BILIBIN_KITCHEN_WORKER, 0);
  ObjectMotion_SetAngleToward(LOCALID_BILIBIN_KITCHEN_WORKER,
                              LOCALID_PARTY_LEADER, 0);
  FieldScript_ShowMessageAndWait(LOCALID_BILIBIN_KITCHEN_WORKER, 0);
  ObjectMotion_ArmCallback(LOCALID_BILIBIN_KITCHEN_WORKER, FACING_EAST, 10);
  FieldScript_End();
}

/// The waitress hurries through her work before the head chef yells. She works
/// in Bilibin's restaurant once the curse is lifted.
static void EventScript_TalkToWaitress(void) {
  FieldScript_Start();
  FieldScript_SetMessageId(MsgBilibinLetsSeeServeThemWater);
  Script_ShowMessage(LOCALID_BILIBIN_WAITRESS, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_BILIBIN_WAITRESS, LOCALID_PARTY_LEADER,
                              0);
  Script_ShowMessage(LOCALID_BILIBIN_WAITRESS, 0);
  ObjectEvent_FaceDirection(LOCALID_BILIBIN_WAITRESS, FACING_NORTH, 10);
  FieldScript_End();
}

/// A diner loves the meal and asks whether the party does too. The diner is in
/// Bilibin's restaurant once the curse is lifted and the food flows.
static void EventScript_TalkAboutMeal(void) {
  FieldScript_Start();
  FieldScript_SetMessageId(MsgBilibinWasSomeMealDontJust);
  FieldScript_AskYesNoAndShowResponse(LOCALID_BILIBIN_DINER, 0);
  FieldScript_End();
}

/// Mind Read on the burly man in Bilibin who admires Mia, before the curse is
/// lifted: with Mia in the party (FLAG_IN_PARTY says who is), even frozen Imil
/// must feel warm with her around; without her, he hopes to see her
/// again in spring.
static void EventScript_ReadMiaAdmirersMind(void) {
  FieldScript_Start();
  if (GameFlag_Get(FLAG_IN_PARTY(MIA)) != 0) {
    FieldScript_SetMessageId(MsgBilibinEvenFrozenImilMustFeel);
  } else {
    FieldScript_SetMessageId(MsgBilibinWhenSpringComesWantGo);
  }
  FieldScript_ShowMessageAndWait(LOCALID_BILIBIN_MIA_ADMIRER, 0);
  FieldScript_End();
}

/// Sets up Bilibin's buildings: the rooms open through a window, and the diner
/// at the restaurant's table is drawn at sprite priority 2, never by the
/// automatic priority.
s32 CottageRoomsC_Initialize(void) {
  struct ObjectEvent *objectEvent;

  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  objectEvent = ObjectEvent_GetById(LOCALID_BILIBIN_DINER);
  objectEvent->priorityFlags = 0;
  objectEvent->sprite->image.priority = 2;
  return 0;
}

struct MapWarpDest gCottageRoomsCWarpDests[] = {
    {0, CONDITION_ALWAYS, 176, 0, 216, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {COTTAGE_ROOMS_C_HOUSE_EAST, CONDITION_ALWAYS, 176, 0, 216, FACING_NORTH, 0,
     24, 32, 264, 240, 0},
    {COTTAGE_ROOMS_C_HOUSE_WEST, CONDITION_ALWAYS, 400, 0, 264, FACING_NORTH, 0,
     256, 0, 496, 288, 0},
    {COTTAGE_ROOMS_C_HOUSE_NORTH, CONDITION_ALWAYS, 688, 0, 264, FACING_NORTH,
     0, 544, 0, 784, 288, 0},
    {COTTAGE_ROOMS_C_WEAPON_AND_ARMOR_SHOP, CONDITION_ALWAYS, 240, 0, 504,
     FACING_NORTH, 0, 72, 336, 312, 528, 0},
    {COTTAGE_ROOMS_C_ITEM_SHOP, CONDITION_ALWAYS, 576, 0, 536, FACING_NORTH, 0,
     400, 304, 640, 552, 0},
    {COTTAGE_ROOMS_C_INN, CONDITION_ALWAYS, 352, 0, 872, FACING_NORTH, 0, 0,
     640, 400, 896, 0},
    {COTTAGE_ROOMS_C_INN_UPSTAIRS_FROM_INN, CONDITION_ALWAYS, 656, 0, 696,
     FACING_WEST, 0, 432, 640, 848, 896, 0},
    {COTTAGE_ROOMS_C_INN_FROM_INN_UPSTAIRS, CONDITION_ALWAYS, 344, 0, 696,
     FACING_EAST, 0, 0, 640, 400, 896, 0},
    {MAP_TABLE_END},
};

u32 gCottageRoomsCWarps[] = {
    MAP_WARPS(CottageRoomsC),
    MAP_WARP(COTTAGE_ROOMS_C_HOUSE_EAST_TO_BILIBIN, Bilibin,
             BILIBIN_FROM_HOUSE_EAST),
    MAP_WARP(COTTAGE_ROOMS_C_HOUSE_WEST_TO_BILIBIN, Bilibin,
             BILIBIN_FROM_HOUSE_WEST),
    MAP_WARP(COTTAGE_ROOMS_C_HOUSE_NORTH_TO_BILIBIN, Bilibin,
             BILIBIN_FROM_HOUSE_NORTH),
    MAP_WARP(COTTAGE_ROOMS_C_WEAPON_AND_ARMOR_SHOP_TO_BILIBIN, Bilibin,
             BILIBIN_FROM_WEAPON_AND_ARMOR_SHOP),
    MAP_WARP(COTTAGE_ROOMS_C_ITEM_SHOP_TO_BILIBIN, Bilibin,
             BILIBIN_FROM_ITEM_SHOP),
    MAP_WARP(COTTAGE_ROOMS_C_INN_TO_BILIBIN, Bilibin, BILIBIN_FROM_INN),
    MAP_WARP(COTTAGE_ROOMS_C_INN_UPSTAIRS_TO_INN, CottageRoomsC,
             COTTAGE_ROOMS_C_INN_FROM_INN_UPSTAIRS),
    MAP_WARP(COTTAGE_ROOMS_C_INN_TO_INN_UPSTAIRS, CottageRoomsC,
             COTTAGE_ROOMS_C_INN_UPSTAIRS_FROM_INN),
    MAP_WARPS_END,
};

// clang-format off
struct ObjectEventTemplate gCottageRoomsCObjectEventTemplates[] = {
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(104), 0, PIXELS(168), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, 0x0, MOVEMENT_TYPE_WANDER,
        PIXELS(200), 0, PIXELS(136), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(384), 0, PIXELS(192), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_BOY_WITH_FAIR_HAIR, 0x0, MOVEMENT_TYPE_WANDER,
        PIXELS(360), 0, PIXELS(136), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(640), 0, PIXELS(156), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_GIRL_WITH_PIGTAILS, 0x0, MOVEMENT_TYPE_WANDER,
        PIXELS(704), 0, PIXELS(200), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(456), 0, PIXELS(472), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_WOMAN, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(488), 0, PIXELS(424), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_BALD_MAN_IN_RED, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(200), 0, PIXELS(440), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_IN_GREEN, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(168), 0, PIXELS(440), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_WITH_PONYTAIL, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(584), 0, PIXELS(456), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_MAN_WITH_DARK_HAIR, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(328), 0, PIXELS(744), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_IN_PINK_APRON, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(280), 0, PIXELS(760), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_CHEF, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(88), 0, PIXELS(760), FACING_NORTH, TALK_FACING_KEEP, 0 },
    { SPRITE_CHEF, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(56), 0, PIXELS(760), FACING_EAST, TALK_FACING_KEEP, 0 },
    { SPRITE_YOUNG_WOMAN_IN_GREEN, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(72), 0, PIXELS(776), FACING_NORTHEAST, TALK_FACING_KEEP, 0 },
    { SPRITE_MAN_CARRYING_A_PACK, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(176), 0, PIXELS(744), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_MAN_CARRYING_A_PACK, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(152), 0, PIXELS(768), FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_STRONGMAN, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(152), 0, PIXELS(832), FACING_NORTHWEST, TALK_FACING_KEEP, 0 },
    { SPRITE_MAN_WITH_PICKAXE, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(120), 0, PIXELS(796), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(88), 0, PIXELS(840), FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_MAN_IN_RED_BANDANA, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(808), 0, PIXELS(774), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_WITH_LUTE, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(568), 0, PIXELS(768), FACING_SOUTHEAST, TALK_FACING_PARTY, 0 },
    { MAP_TABLE_END },
};

struct MapEvent gCottageRoomsCEvents[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, COTTAGE_ROOMS_C_HOUSE_EAST_TO_BILIBIN },
    { MAP_EVENT_WARP, 2, CONDITION_ALWAYS, COTTAGE_ROOMS_C_HOUSE_WEST_TO_BILIBIN },
    { MAP_EVENT_WARP, 3, CONDITION_ALWAYS, COTTAGE_ROOMS_C_HOUSE_NORTH_TO_BILIBIN },
    { MAP_EVENT_WARP, 4, CONDITION_ALWAYS, COTTAGE_ROOMS_C_WEAPON_AND_ARMOR_SHOP_TO_BILIBIN },
    { MAP_EVENT_WARP, 5, CONDITION_ALWAYS, COTTAGE_ROOMS_C_ITEM_SHOP_TO_BILIBIN },
    { MAP_EVENT_WARP, 6, CONDITION_ALWAYS, COTTAGE_ROOMS_C_INN_TO_BILIBIN },
    { MAP_EVENT_WARP, 7, CONDITION_ALWAYS, COTTAGE_ROOMS_C_INN_UPSTAIRS_TO_INN },
    { MAP_EVENT_WARP, 8, CONDITION_ALWAYS, COTTAGE_ROOMS_C_INN_TO_INN_UPSTAIRS },
    { MAP_EVENT_TALK, LOCALID_BILIBIN_YOUNG_MAN_ON_THE_CURSED_MAN, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinThatTreeAtTheEntranceIs) },
    { MAP_EVENT_TALK, LOCALID_BILIBIN_VILLAGER_PALACE, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinMccoyHadTheTreesCutDown) },
    { MAP_EVENT_TALK, LOCALID_BILIBIN_MIA_ADMIRER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinTheSnowIsAlreadyPilingUp) },
    { MAP_EVENT_TALK, LOCALID_BILIBIN_BOY_SNOW, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinWeRarelySeeAnySnowAround) },
    { MAP_EVENT_TALK, LOCALID_BILIBIN_VILLAGER_WORRIED_FOR_KOLIMA, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinHowArePeopleDoingInKolima) },
    { MAP_EVENT_TALK, LOCALID_BILIBIN_GIRL_SILK, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinIKnowThatGuyWhoTurned) },
    { MAP_EVENT_TALK, LOCALID_BILIBIN_OLD_MAN_WATER_OF_HERMES, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinLongAgoTheLighthouseNearImil) },
    { MAP_EVENT_TALK, LOCALID_BILIBIN_OLD_WOMAN_LOST_YOUTH, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinButEvenTheLegendaryWaterOf) },
    { MAP_EVENT_TALK, LOCALID_BILIBIN_WEAPON_MERCHANT, FLAG_KOLIMA_CURSE_LIFTED, EVENT_SCRIPT(EventScript_TalkToWeaponMerchant) },
    { MAP_EVENT_TALK, LOCALID_BILIBIN_ARMOR_MERCHANT, FLAG_KOLIMA_CURSE_LIFTED, EVENT_SCRIPT(EventScript_TalkToArmorMerchant) },
    { MAP_EVENT_TALK, LOCALID_BILIBIN_ITEM_MERCHANT, FLAG_KOLIMA_CURSE_LIFTED, EVENT_SCRIPT(EventScript_TalkToItemMerchant) },
    { MAP_EVENT_TALK, LOCALID_BILIBIN_INNKEEPER, FLAG_KOLIMA_CURSE_LIFTED, EVENT_SCRIPT(EventScript_TalkToInnkeeper) },
    { MAP_EVENT_TALK, LOCALID_BILIBIN_INN_WORKER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinYouCantReachKolimaNoMatter) },
    { MAP_EVENT_TALK, LOCALID_BILIBIN_CHEF, FLAG_KOLIMA_CURSE_LIFTED, EVENT_SCRIPT(EventScript_TalkToWorriedChef) },
    { MAP_EVENT_TALK, LOCALID_BILIBIN_KITCHEN_WORKER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinHeyMisterHeadChefAreYou) },
    { MAP_EVENT_TALK, LOCALID_BILIBIN_WAITRESS, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinStartCookingAlready) },
    { MAP_EVENT_TALK, LOCALID_BILIBIN_HUNGRY_DINER, FLAG_KOLIMA_CURSE_LIFTED, EVENT_SCRIPT(EventScript_TalkToHungryDiner) },
    { MAP_EVENT_TALK, LOCALID_BILIBIN_FURNITURE_SELLER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinWeCameHereBecauseLordMccoy) },
    { MAP_EVENT_TALK, LOCALID_BILIBIN_DINER_BIG_TREE, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinIThinkThatBigTreeDeep) },
    { MAP_EVENT_TALK, LOCALID_BILIBIN_DINER, FLAG_KOLIMA_CURSE_LIFTED, EVENT_SCRIPT(EventScript_AskAboutTreeSpirit) },
    { MAP_EVENT_TALK, LOCALID_BILIBIN_DINER_TODAYS_SPECIAL, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinICantWaitToSeeTodays) },
    { MAP_EVENT_TALK, LOCALID_BILIBIN_JOB_SEEKER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinIWentToLordMccoyLooking) },
    { MAP_EVENT_TALK, LOCALID_BILIBIN_VILLAGER_ANCIENT_TREES, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinIveHeardSongsThatTellOf) },
    { MAP_EVENT_TALK, LOCALID_BILIBIN_YOUNG_MAN_ON_THE_CURSED_MAN, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_AskAboutHolyTree) },
    { MAP_EVENT_TALK, LOCALID_BILIBIN_VILLAGER_PALACE, CONDITION_ALWAYS, MSGNO(MsgBilibinThankGoodnessTheyStoppedConstructionOf) },
    { MAP_EVENT_TALK, LOCALID_BILIBIN_MIA_ADMIRER, CONDITION_ALWAYS, MSGNO(MsgBilibinIAlwaysMissAltinInThe) },
    { MAP_EVENT_TALK, LOCALID_BILIBIN_BOY_SNOW, CONDITION_ALWAYS, MSGNO(MsgBilibinMyFatherSaysThatItsWarm) },
    { MAP_EVENT_TALK, LOCALID_BILIBIN_VILLAGER_WORRIED_FOR_KOLIMA, CONDITION_ALWAYS, MSGNO(MsgBilibinTheRoadToKolimaIsOpen) },
    { MAP_EVENT_TALK, LOCALID_BILIBIN_GIRL_SILK, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_AskAboutSilk) },
    { MAP_EVENT_TALK, LOCALID_BILIBIN_OLD_MAN_WATER_OF_HERMES, CONDITION_ALWAYS, MSGNO(MsgBilibinIThoughtISawLightsIn) },
    { MAP_EVENT_TALK, LOCALID_BILIBIN_OLD_WOMAN_LOST_YOUTH, CONDITION_ALWAYS, MSGNO(MsgBilibinHisEyesAreRedBecauseHe) },
    { MAP_EVENT_TALK, LOCALID_BILIBIN_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToWeaponMerchant) },
    { MAP_EVENT_TALK, LOCALID_BILIBIN_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToArmorMerchant) },
    { MAP_EVENT_TALK, LOCALID_BILIBIN_ITEM_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToItemMerchant) },
    { MAP_EVENT_TALK, LOCALID_BILIBIN_INNKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToInnkeeper) },
    { MAP_EVENT_TALK, LOCALID_BILIBIN_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgBilibinThereWillBeFewerGuestsNow) },
    { MAP_EVENT_TALK, LOCALID_BILIBIN_CHEF, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToBusyChef) },
    { MAP_EVENT_TALK, LOCALID_BILIBIN_KITCHEN_WORKER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToKitchenWorker) },
    { MAP_EVENT_TALK, LOCALID_BILIBIN_WAITRESS, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToWaitress) },
    { MAP_EVENT_TALK, LOCALID_BILIBIN_HUNGRY_DINER, CONDITION_ALWAYS, MSGNO(MsgBilibinImALittleSurprisedThatThe) },
    { MAP_EVENT_TALK, LOCALID_BILIBIN_FURNITURE_SELLER, CONDITION_ALWAYS, MSGNO(MsgBilibinTheresNoBusinessHereSoIm) },
    { MAP_EVENT_TALK, LOCALID_BILIBIN_DINER_BIG_TREE, CONDITION_ALWAYS, MSGNO(MsgBilibinTheyServedSoMuchFoodToday) },
    { MAP_EVENT_TALK, LOCALID_BILIBIN_DINER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkAboutMeal) },
    { MAP_EVENT_TALK, LOCALID_BILIBIN_DINER_TODAYS_SPECIAL, CONDITION_ALWAYS, MSGNO(MsgBilibinOooohThatsItITriedMy) },
    { MAP_EVENT_TALK, LOCALID_BILIBIN_JOB_SEEKER, CONDITION_ALWAYS, MSGNO(MsgBilibinTheIncidentWasResolvedBeforeI) },
    { MAP_EVENT_TALK, LOCALID_BILIBIN_VILLAGER_ANCIENT_TREES, CONDITION_ALWAYS, MSGNO(MsgBilibinIWonderWhatThatEerieHowling) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BILIBIN_YOUNG_MAN_ON_THE_CURSED_MAN, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinThisIsAllLordMccoysFault) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BILIBIN_VILLAGER_PALACE, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinAhLadyMccoyIWouldJust) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BILIBIN_MIA_ADMIRER, FLAG_KOLIMA_CURSE_LIFTED, EVENT_SCRIPT(EventScript_ReadMiaAdmirersMind) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BILIBIN_BOY_SNOW, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinItMustBeDifficultToLive) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BILIBIN_VILLAGER_WORRIED_FOR_KOLIMA, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinICantImagineThatEveryoneIn) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BILIBIN_GIRL_SILK, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinHeSeemedSoHappyAndThen) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BILIBIN_OLD_MAN_WATER_OF_HERMES, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinCouldTheWaterOfHermesSave) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BILIBIN_OLD_WOMAN_LOST_YOUTH, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinTheWaterOfHermesVanishedLong) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BILIBIN_WEAPON_MERCHANT, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinTheWorldWillNeverSeeSuch) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BILIBIN_ARMOR_MERCHANT, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinIWishICouldSeeAnother) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BILIBIN_ITEM_MERCHANT, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinOnlySomeoneReallyBraveWouldGo) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BILIBIN_INNKEEPER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinIDontBelieveInCursesFrom) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BILIBIN_INN_WORKER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinAnyTravelersHeadingEastAreStuck) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BILIBIN_CHEF, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinFatherMotherIHopeTheyreAll) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BILIBIN_KITCHEN_WORKER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinShouldIJustForgetThatIm) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BILIBIN_WAITRESS, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinIveHadAllTheCustomerComplaints) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BILIBIN_HUNGRY_DINER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinArghhhThatsItIJustWant) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BILIBIN_FURNITURE_SELLER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinICameToBilibinHopingTo) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BILIBIN_DINER_BIG_TREE, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinISeeWhyTheyCallThat) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BILIBIN_DINER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinIDontCareWhoDoesIt) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BILIBIN_DINER_TODAYS_SPECIAL, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinYippeeIsItReadyYetMy) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BILIBIN_JOB_SEEKER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinImGettingALittleScaredHearing) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BILIBIN_VILLAGER_ANCIENT_TREES, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinItHasBeenSaidThatA) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BILIBIN_YOUNG_MAN_ON_THE_CURSED_MAN, CONDITION_ALWAYS, MSGNO(MsgBilibinItTakesManyYearsForA) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BILIBIN_VILLAGER_PALACE, CONDITION_ALWAYS, MSGNO(MsgBilibinSometimesIWishICouldSee) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BILIBIN_MIA_ADMIRER, CONDITION_ALWAYS, MSGNO(MsgBilibinIWonderIfThereAreStill) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BILIBIN_BOY_SNOW, CONDITION_ALWAYS, MSGNO(MsgBilibinMyFatherHasTraveledAllOver) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BILIBIN_VILLAGER_WORRIED_FOR_KOLIMA, CONDITION_ALWAYS, MSGNO(MsgBilibinTravelingMakesYoungWarriorsStronger) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BILIBIN_GIRL_SILK, CONDITION_ALWAYS, MSGNO(MsgBilibinClothesMadeWithSilkAreSo) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BILIBIN_OLD_MAN_WATER_OF_HERMES, CONDITION_ALWAYS, MSGNO(MsgBilibinTheresNoDoubtAboutItIts) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BILIBIN_OLD_WOMAN_LOST_YOUTH, CONDITION_ALWAYS, MSGNO(MsgBilibinWhatsTheBigDealWhyAre) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BILIBIN_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgBilibinAltinsGotGoodWeaponsButI) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BILIBIN_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgBilibinIWishICouldGoOut) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BILIBIN_ITEM_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgBilibinTheyCallThemselvesWarriorsButYou) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BILIBIN_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgBilibinWellIWasOnlyActingScared) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BILIBIN_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgBilibinItdBeBetterForBusinessIf) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BILIBIN_CHEF, CONDITION_ALWAYS, MSGNO(MsgBilibinIllGoToKolimaWhenThe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BILIBIN_KITCHEN_WORKER, CONDITION_ALWAYS, MSGNO(MsgBilibinLetsSeeSoupSaladAndI) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BILIBIN_WAITRESS, CONDITION_ALWAYS, MSGNO(MsgBilibinTheHeadChefWasSoLazy) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BILIBIN_HUNGRY_DINER, CONDITION_ALWAYS, MSGNO(MsgBilibinItUsedToTakeThemSo) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BILIBIN_FURNITURE_SELLER, CONDITION_ALWAYS, MSGNO(MsgBilibinWhenTheyStoppedBuildingThePalace) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BILIBIN_DINER_BIG_TREE, CONDITION_ALWAYS, MSGNO(MsgBilibinIWishICouldEatLike) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BILIBIN_DINER, CONDITION_ALWAYS, MSGNO(MsgBilibinIdBetterEatALotThat) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BILIBIN_DINER_TODAYS_SPECIAL, CONDITION_ALWAYS, MSGNO(MsgBilibinTheFoodIsSuchAGood) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BILIBIN_JOB_SEEKER, CONDITION_ALWAYS, MSGNO(MsgBilibinIfAKidLikeThisCould) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BILIBIN_VILLAGER_ANCIENT_TREES, CONDITION_ALWAYS, MSGNO(MsgBilibinItLooksLikeScaryThingsAre) },
    { BG_EVENT(BG_EVENT_BARREL), 100, 0xf52, FIND_ITEM(ITEM_ANTIDOTE) },
    { BG_EVENT(BG_EVENT_BED), 101, 0xf53, FIND_ITEM(ITEM_SLEEP_BOMB) },
    { BG_EVENT(BG_EVENT_BARREL), 102, 0xf54, FIND_ITEM(ITEM_HARD_NUT) },
    { BG_EVENT(BG_EVENT_OVEN), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBilibinLooksLikeEggSoupSoftEggs)) },
    { BG_EVENT(BG_EVENT_OVEN), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBilibinFreshOmelettesBilibinoOmelettesUseWild)) },
    { BG_EVENT(BG_EVENT_BOOKCASE), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBilibinTheresABookEntitledAllAbout)) },
    { MAP_EVENTS_END },
};
// clang-format on
