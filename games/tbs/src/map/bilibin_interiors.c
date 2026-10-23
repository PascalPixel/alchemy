/// @file
/// Inside Bilibin's buildings: MapId_BilibinInteriors, whose area name is
/// Bilibin. Six of Bilibin's doors (BILIBIN_TO_INN to
/// BILIBIN_TO_WEAPON_AND_ARMOR_SHOP, in bilibin.c) lead into its rooms, and
/// each room's door leads back out to the door it was entered by. One room
/// holds the weapon and armor shops, another the item shop, a third the inn
/// with the restaurant beside it, whose stairs
/// (BILIBIN_INTERIORS_INN_TO_INN_UPSTAIRS and
/// BILIBIN_INTERIORS_INN_UPSTAIRS_TO_INN) join it to its upstairs
/// rooms; three other buildings hold villagers.
/// BilibinInteriors_GetObjectEventTemplates leaves out the object events of the
/// rooms not in view.
///
/// Everyone has a line for while Kolima's curse holds (FLAG_KOLIMA_CURSE_LIFTED
/// clear) and one for after. Before, the townsfolk explain the tree at the
/// village entrance, McCoy's palace and the Water of Hermes, and in the
/// restaurant the head chef, born in Kolima, is too worried to cook. After,
/// the head chef cooks so hard his kitchen cannot keep up: the first time the
/// party talks to him (FLAG_MAPSEC_TEMP_00, kept until the party leaves
/// Bilibin) he shouts a string of orders and the kitchen worker answers, and
/// from then on he sends the party to the waitress
/// (EventScript_TalkToBusyChef).
///
/// gBilibinInteriorsEvents keeps both times in one table, in the ROM's order:
/// every row that needs FLAG_KOLIMA_CURSE_LIFTED clear comes before the rows
/// for afterwards, so the first row that matches picks the time. The
/// merchants and the innkeeper open the shop or the inn only when the leader
/// faces them across the counter (facing north), and otherwise talk of the
/// time; Mind Read on the villager who admires Mia depends on whether she is in
/// the party. Searching finds an Antidote, a Sleep Bomb and a Hard Nut, egg
/// soup and omelettes in the ovens, and a book called "All About Items".
/// BilibinInteriors_Initialize opens the rooms with a window around the leader
/// and draws one diner, LOCALID_BILIBIN_DINER, at a fixed sprite priority. He
/// stands at the north edge of the restaurant's table, on cell (7, 49), which
/// alone of the diners' cells gives an object standing on it priority 3
/// (layout_032's grid_metatile.tsv). Taken from that cell, his priority would
/// put him behind the middle layer (`BG2`, priority 2 in this map); the fixed
/// priority 2 draws him in front of it, as the other diners are.
///
/// Open questions: none.

#include "unit/map/bilibin_interiors.h"
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

/// Bilibin's shops and inn, as Engine_ShopOpen and Engine_InnOpen number them.
enum {
  LOCALID_BILIBIN_WEAPON_SHOP = 7,
  LOCALID_BILIBIN_ARMOR_SHOP = 8,
  LOCALID_BILIBIN_ITEM_SHOP = 9,
  LOCALID_BILIBIN_INN = 2
};

/// The placed object events, numbered in the order of
/// gBilibinInteriorsObjectEventTemplates from LOCALID_FIRST_PLACED. Each room's
/// object events are together: two in each house, then the shops' customers and
/// keepers, the inn and its restaurant, and the room behind the inn's inner
/// door.
enum BilibinInteriorsLocalId {
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

const struct MapWarpDest *BilibinInteriors_GetWarpDests(void) {
  return gBilibinInteriorsWarpDests;
}

const struct MapWarpRegion *BilibinInteriors_GetRegions(void) { return 0; }

const u32 *BilibinInteriors_GetWarps(void) { return gBilibinInteriorsWarps; }

/// Marks object event templates outside the current room's view before object
/// events are spawned.
const struct ObjectEventTemplate *
BilibinInteriors_GetObjectEventTemplates(void) {
  ObjectEventTemplate_ClipToView(gBilibinInteriorsObjectEventTemplates);
  return gBilibinInteriorsObjectEventTemplates;
}

/// Bilibin's weapon merchant: across the counter the shop opens; otherwise,
/// before the curse is lifted, the merchant mourns Vale's lost healer, and
/// after, recommends Altin's store.
static void EventScript_TalkToWeaponMerchant(void) {
  u32 leaderFacing;

  leaderFacing = ObjectEvent_GetById(LOCALID_PARTY_LEADER)->facing;
  Engine_ScriptStart();

  if (FACING_IS_NORTH(leaderFacing)) {
    Engine_ShopOpen(LOCALID_BILIBIN_WEAPON_SHOP,
                    LOCALID_BILIBIN_WEAPON_MERCHANT);
  } else {
    if (Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_LIFTED) == 0) {
      Engine_ScriptSetMessage(MsgBilibinThereWasAbleHealerIn);
    } else {
      Engine_ScriptSetMessage(MsgBilibinOurWeaponsBestCanFind);
    }
    Engine_ScriptShowMessage(LOCALID_BILIBIN_WEAPON_MERCHANT, 0);
  }

  Engine_ScriptEnd();
}

/// Bilibin's item merchant: across the counter the shop opens; otherwise,
/// before the curse is lifted, the merchant says Lord McCoy could use
/// travelers like the party, and after, that his hired warriors were useless.
static void EventScript_TalkToItemMerchant(void) {
  u32 leaderFacing;

  leaderFacing = ObjectEvent_GetById(LOCALID_PARTY_LEADER)->facing;
  Engine_ScriptStart();

  if (FACING_IS_NORTH(leaderFacing)) {
    Engine_ShopOpen(LOCALID_BILIBIN_ITEM_SHOP, LOCALID_BILIBIN_ITEM_MERCHANT);
  } else {
    if (Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_LIFTED) == 0) {
      Engine_ScriptSetMessage(MsgBilibinMustUsedTypesDangerBeing);
    } else {
      Engine_ScriptSetMessage(MsgBilibinTurnedOutWarriorsHiredBy);
    }
    Engine_ScriptShowMessage(LOCALID_BILIBIN_ITEM_MERCHANT, 0);
  }

  Engine_ScriptEnd();
}

/// Returns both story stages' map events; conditions choose the current
/// dialogue.
const struct MapEvent *BilibinInteriors_GetEvents(void) {
  return gBilibinInteriorsEvents;
}

/// Bilibin's armor merchant: across the counter the shop opens; otherwise,
/// before the curse is lifted, the merchant asks whether the party has heard
/// of the ocean, and after, dreams aloud of it.
static void EventScript_TalkToArmorMerchant(void) {
  u32 leaderFacing;

  leaderFacing = ObjectEvent_GetById(LOCALID_PARTY_LEADER)->facing;
  Engine_ScriptStart();
  if (FACING_IS_NORTH(leaderFacing)) {
    Engine_ShopOpen(LOCALID_BILIBIN_ARMOR_SHOP, LOCALID_BILIBIN_ARMOR_MERCHANT);
  } else {
    if (GameFlag_IsSet(FLAG_KOLIMA_CURSE_LIFTED) == 0) {
      Engine_ScriptSetMessage(MsgBilibinHaveEverHeardOcean);
      ObjectEvent_FaceObjectEvent(LOCALID_BILIBIN_ARMOR_MERCHANT,
                                  LOCALID_PARTY_LEADER, 0);
      Engine_ScriptWait(10);
      Script_AskYesNo(LOCALID_BILIBIN_ARMOR_MERCHANT, 0);
      ObjectEvent_FaceDirection(LOCALID_BILIBIN_ARMOR_MERCHANT,
                                FACING_SOUTHEAST + FACING_STEP, 10);
    } else {
      Engine_ScriptSetMessage(MsgBilibinForSomeReasonOceanFills);
      Script_ShowMessage(LOCALID_BILIBIN_ARMOR_MERCHANT, 0);
    }
  }
  Engine_ScriptEnd();
}

/// The head chef in Bilibin's restaurant, before the curse is lifted: born in
/// Kolima, he is too worried to focus on his cooking.
static void EventScript_TalkToWorriedChef(void) {
  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgBilibinImReallyWorriedAboutKolima);
  ObjectEvent_FaceObjectEvent(LOCALID_BILIBIN_CHEF, LOCALID_PARTY_LEADER, 0);
  Script_ShowMessage(LOCALID_BILIBIN_CHEF, 0);
  ObjectEvent_FaceDirection(LOCALID_BILIBIN_CHEF, FACING_NORTH, 10);
  Engine_ScriptEnd();
}

/// A diner in Bilibin's restaurant, before the curse is lifted, still waiting
/// for his dinner: his stomach growls, and he asks whether the party heard it.
static void EventScript_TalkToHungryDiner(void) {
  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgBilibinGrrr);
  Script_ShowMessageAndWait(LOCALID_BILIBIN_HUNGRY_DINER, 0, 20);
  ObjectEvent_FaceObjectEvent(LOCALID_BILIBIN_HUNGRY_DINER,
                              LOCALID_PARTY_LEADER, 0);
  Engine_ScriptWait(10);
  Script_OpenMessage(LOCALID_BILIBIN_HUNGRY_DINER, 0);
  if (Engine_ScriptChooseYesNo(0, 0) != 0) {
    gFieldWork->message++;
  }
  Script_ShowMessage(LOCALID_BILIBIN_HUNGRY_DINER, 0);
  ObjectEvent_FaceDirection(LOCALID_BILIBIN_HUNGRY_DINER, FACING_SOUTH, 10);
  Engine_ScriptEnd();
}

/// A diner asks whether a tree spirit can curse a man. The diner sits in
/// Bilibin's restaurant before the curse is lifted; on yes, sends the party to
/// hear Lord McCoy's story. Then the diner turns back to the table.
static void EventScript_AskAboutTreeSpirit(void) {
  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgBilibinDoBelieveTreeSpiritCan);
  ObjectEvent_FaceObjectEvent(LOCALID_BILIBIN_DINER, LOCALID_PARTY_LEADER, 0);
  Engine_ScriptWait(10);
  Script_OpenMessage(LOCALID_BILIBIN_DINER, 0);
  if (Engine_ScriptChooseYesNo(0, 0) != 0) {
    gFieldWork->message++;
  }
  Script_ShowMessage(LOCALID_BILIBIN_DINER, 0);
  ObjectEvent_FaceDirection(LOCALID_BILIBIN_DINER, FACING_SOUTH, 10);
  Engine_ScriptEnd();
}

/// A young man asks whether Kolima Forest's holy tree survived. He's in Bilibin
/// once the curse is lifted.
static void EventScript_AskAboutHolyTree(void) {
  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgBilibinDoKnowIfHolyTree);
  Engine_ScriptAskYesNo(LOCALID_BILIBIN_YOUNG_MAN_ON_THE_CURSED_MAN, 0);
  Engine_ScriptEnd();
}

/// A little girl in Bilibin, once the curse is lifted, asks whether the party
/// knows what silk is; on yes, that Xian, across the river, makes it, at the
/// start of Silk Road.
static void EventScript_AskAboutSilk(void) {
  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgBilibinDoKnowSilk);
  Engine_ScriptAskYesNo(LOCALID_BILIBIN_GIRL_SILK, 0);
  Engine_ScriptEnd();
}

/// Bilibin's innkeeper: across the counter the inn opens; otherwise, before
/// the curse is lifted, the innkeeper is scared of the curse on Kolima, and
/// after, asks whether it wasn't horrifying.
static void EventScript_TalkToInnkeeper(void) {
  u32 leaderFacing;

  leaderFacing = ObjectEvent_GetById(LOCALID_PARTY_LEADER)->facing;
  Engine_ScriptStart();

  if (FACING_IS_NORTH(leaderFacing)) {
    Engine_InnOpen(LOCALID_BILIBIN_INN, LOCALID_BILIBIN_INNKEEPER);
  } else if (Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_LIFTED) != 0) {
    Engine_ScriptSetMessage(MsgBilibinWasntCurseInKolimaHorrifying);
    Engine_ScriptAskYesNo(LOCALID_BILIBIN_INNKEEPER, 0);
  } else {
    Engine_ScriptSetMessage(MsgBilibinCurseOnKolimaScaryDevelopment);
    Engine_ScriptShowMessage(LOCALID_BILIBIN_INNKEEPER, 0);
  }

  Engine_ScriptEnd();
}

/// The head chef in Bilibin's restaurant, once the curse is lifted: he calls
/// out orders once, then sends the party to the waitress.
static void EventScript_TalkToBusyChef(void) {
  Engine_ScriptStart();
  if (GameFlag_IsSet(FLAG_MAPSEC_TEMP_00) == 0) {
    // The first time (FLAG_MAPSEC_TEMP_00, kept until the party leaves
    // Bilibin), the chef shouts orders and the kitchen worker answers.
    Engine_ScriptSetMessage(MsgBilibinTwoSpecialsOneDinnerOne);
    Script_ShowMessage(LOCALID_BILIBIN_CHEF, 0);
    ObjectEvent_FaceDirection(LOCALID_BILIBIN_CHEF, FACING_WEST, 20);
    Script_ShowMessage(LOCALID_BILIBIN_CHEF, 0);
    Engine_ObjectEventStartRepeatedMotion(LOCALID_BILIBIN_KITCHEN_WORKER, 2);
    ObjectEvent_SetAttachedEffect(LOCALID_BILIBIN_KITCHEN_WORKER,
                                  ATTACHED_EFFECT_IN_FRONT |
                                      ATTACHED_EFFECT_ONCE);
    Engine_ScriptWait(60);
    Script_ShowMessage(LOCALID_BILIBIN_KITCHEN_WORKER, 0);
    Engine_ScriptWait(10);
    GameFlag_Set(FLAG_MAPSEC_TEMP_00);
  }
  // Then, as every later time, he sends the party to the waitress.
  ObjectEvent_FaceObjectEvent(LOCALID_BILIBIN_CHEF, LOCALID_PARTY_LEADER, 0);
  Engine_ScriptSetMessage(MsgBilibinIfWantMealSpeakWaitress);
  Script_ShowMessage(LOCALID_BILIBIN_CHEF, 0);
  ObjectEvent_FaceDirection(LOCALID_BILIBIN_CHEF, FACING_NORTH, 10);
  Engine_ScriptEnd();
}

/// The kitchen worker can't keep up with the head chef's energy. This is in
/// Bilibin's restaurant once the curse is lifted.
static void EventScript_TalkToKitchenWorker(void) {
  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgBilibinReallyThinkHeadChefHas);
  Engine_ScriptShowMessage(LOCALID_BILIBIN_KITCHEN_WORKER, 0);
  Engine_ObjectEventFaceObjectEvent(LOCALID_BILIBIN_KITCHEN_WORKER,
                                    LOCALID_PARTY_LEADER, 0);
  Engine_ScriptShowMessage(LOCALID_BILIBIN_KITCHEN_WORKER, 0);
  Engine_ObjectEventFaceDirection(LOCALID_BILIBIN_KITCHEN_WORKER, FACING_EAST,
                                  10);
  Engine_ScriptEnd();
}

/// The waitress hurries through her work before the head chef yells. She works
/// in Bilibin's restaurant once the curse is lifted.
static void EventScript_TalkToWaitress(void) {
  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgBilibinLetsSeeServeThemWater);
  Script_ShowMessage(LOCALID_BILIBIN_WAITRESS, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_BILIBIN_WAITRESS, LOCALID_PARTY_LEADER,
                              0);
  Script_ShowMessage(LOCALID_BILIBIN_WAITRESS, 0);
  ObjectEvent_FaceDirection(LOCALID_BILIBIN_WAITRESS, FACING_NORTH, 10);
  Engine_ScriptEnd();
}

/// A diner loves the meal and asks whether the party does too. The diner is in
/// Bilibin's restaurant once the curse is lifted and the food flows.
static void EventScript_TalkAboutMeal(void) {
  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgBilibinWasSomeMealDontJust);
  Engine_ScriptAskYesNo(LOCALID_BILIBIN_DINER, 0);
  Engine_ScriptEnd();
}

/// Mind Read on the burly man in Bilibin who admires Mia, before the curse is
/// lifted: with Mia in the party (FLAG_IN_PARTY says who is), even frozen Imil
/// must feel warm with her around; without her, he hopes to see her
/// again in spring.
static void EventScript_ReadMiaAdmirersMind(void) {
  Engine_ScriptStart();
  if (Engine_GameFlagIsSet(FLAG_IN_PARTY(MIA)) != 0) {
    Engine_ScriptSetMessage(MsgBilibinEvenFrozenImilMustFeel);
  } else {
    Engine_ScriptSetMessage(MsgBilibinWhenSpringComesWantGo);
  }
  Engine_ScriptShowMessage(LOCALID_BILIBIN_MIA_ADMIRER, 0);
  Engine_ScriptEnd();
}

/// Sets up Bilibin's buildings: the rooms open through a window, and the diner
/// at the restaurant's table is drawn at sprite priority 2, never by the
/// automatic priority.
s32 BilibinInteriors_Initialize(void) {
  struct ObjectEvent *objectEvent;

  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  objectEvent = ObjectEvent_GetById(LOCALID_BILIBIN_DINER);
  objectEvent->priorityFlags = 0;
  objectEvent->sprite->image.priority = 2;
  return 0;
}

struct MapWarpDest gBilibinInteriorsWarpDests[] = {
    {0, CONDITION_ALWAYS, 176, 0, 216, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {BILIBIN_INTERIORS_HOUSE_EAST, CONDITION_ALWAYS, 176, 0, 216, FACING_NORTH,
     0, 24, 32, 264, 240, 0},
    {BILIBIN_INTERIORS_HOUSE_WEST, CONDITION_ALWAYS, 400, 0, 264, FACING_NORTH,
     0, 256, 0, 496, 288, 0},
    {BILIBIN_INTERIORS_HOUSE_NORTH, CONDITION_ALWAYS, 688, 0, 264, FACING_NORTH,
     0, 544, 0, 784, 288, 0},
    {BILIBIN_INTERIORS_WEAPON_AND_ARMOR_SHOP, CONDITION_ALWAYS, 240, 0, 504,
     FACING_NORTH, 0, 72, 336, 312, 528, 0},
    {BILIBIN_INTERIORS_ITEM_SHOP, CONDITION_ALWAYS, 576, 0, 536, FACING_NORTH,
     0, 400, 304, 640, 552, 0},
    {BILIBIN_INTERIORS_INN, CONDITION_ALWAYS, 352, 0, 872, FACING_NORTH, 0, 0,
     640, 400, 896, 0},
    {BILIBIN_INTERIORS_INN_UPSTAIRS_FROM_INN, CONDITION_ALWAYS, 656, 0, 696,
     FACING_WEST, 0, 432, 640, 848, 896, 0},
    {BILIBIN_INTERIORS_INN_FROM_INN_UPSTAIRS, CONDITION_ALWAYS, 344, 0, 696,
     FACING_EAST, 0, 0, 640, 400, 896, 0},
    {MAP_TABLE_END},
};

u32 gBilibinInteriorsWarps[] = {
    MAP_WARPS(BilibinInteriors),
    MAP_WARP(BILIBIN_INTERIORS_HOUSE_EAST_TO_BILIBIN, Bilibin,
             BILIBIN_FROM_HOUSE_EAST),
    MAP_WARP(BILIBIN_INTERIORS_HOUSE_WEST_TO_BILIBIN, Bilibin,
             BILIBIN_FROM_HOUSE_WEST),
    MAP_WARP(BILIBIN_INTERIORS_HOUSE_NORTH_TO_BILIBIN, Bilibin,
             BILIBIN_FROM_HOUSE_NORTH),
    MAP_WARP(BILIBIN_INTERIORS_WEAPON_AND_ARMOR_SHOP_TO_BILIBIN, Bilibin,
             BILIBIN_FROM_WEAPON_AND_ARMOR_SHOP),
    MAP_WARP(BILIBIN_INTERIORS_ITEM_SHOP_TO_BILIBIN, Bilibin,
             BILIBIN_FROM_ITEM_SHOP),
    MAP_WARP(BILIBIN_INTERIORS_INN_TO_BILIBIN, Bilibin, BILIBIN_FROM_INN),
    MAP_WARP(BILIBIN_INTERIORS_INN_UPSTAIRS_TO_INN, BilibinInteriors,
             BILIBIN_INTERIORS_INN_FROM_INN_UPSTAIRS),
    MAP_WARP(BILIBIN_INTERIORS_INN_TO_INN_UPSTAIRS, BilibinInteriors,
             BILIBIN_INTERIORS_INN_UPSTAIRS_FROM_INN),
    MAP_WARPS_END,
};

// clang-format off
struct ObjectEventTemplate gBilibinInteriorsObjectEventTemplates[] = {
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

struct MapEvent gBilibinInteriorsEvents[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, BILIBIN_INTERIORS_HOUSE_EAST_TO_BILIBIN },
    { MAP_EVENT_WARP, 2, CONDITION_ALWAYS, BILIBIN_INTERIORS_HOUSE_WEST_TO_BILIBIN },
    { MAP_EVENT_WARP, 3, CONDITION_ALWAYS, BILIBIN_INTERIORS_HOUSE_NORTH_TO_BILIBIN },
    { MAP_EVENT_WARP, 4, CONDITION_ALWAYS, BILIBIN_INTERIORS_WEAPON_AND_ARMOR_SHOP_TO_BILIBIN },
    { MAP_EVENT_WARP, 5, CONDITION_ALWAYS, BILIBIN_INTERIORS_ITEM_SHOP_TO_BILIBIN },
    { MAP_EVENT_WARP, 6, CONDITION_ALWAYS, BILIBIN_INTERIORS_INN_TO_BILIBIN },
    { MAP_EVENT_WARP, 7, CONDITION_ALWAYS, BILIBIN_INTERIORS_INN_UPSTAIRS_TO_INN },
    { MAP_EVENT_WARP, 8, CONDITION_ALWAYS, BILIBIN_INTERIORS_INN_TO_INN_UPSTAIRS },
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
