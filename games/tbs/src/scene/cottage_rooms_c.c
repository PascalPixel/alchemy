/// @file
/// Cottage rooms C (SceneId_CottageRoomsC), a sheet of rooms drawn as cottages,
/// all of them inside Bilibin's buildings, whose area name is Bilibin. Six of
/// Bilibin's doors (BILIBIN_TO_INN to BILIBIN_TO_WEAPON_AND_ARMOR_SHOP, in
/// bilibin.c) lead into its rooms, and each room's door leads back out to the
/// door it was entered by. One room holds the weapon and armor shops, another
/// the item shop, a third the inn with the restaurant beside it, whose stairs
/// (COTTAGE_ROOMS_C_INN_TO_INN_UPSTAIRS and
/// COTTAGE_ROOMS_C_INN_UPSTAIRS_TO_INN) join it to its upstairs rooms; three
/// other buildings hold villagers. CottageRoomsC_GetActorPlacements leaves
/// out the actors of the rooms not in view.
///
/// Everyone has a line for while Kolima's curse holds (FLAG_KOLIMA_CURSE_LIFTED
/// clear) and one for after. Before, the townsfolk explain the tree at the
/// village entrance, McCoy's palace and the Water of Hermes, and in the
/// restaurant the head chef, born in Kolima, is too worried to cook. After, the
/// head chef cooks so hard his kitchen cannot keep up: the first time the party
/// talks to him (FLAG_BILIBIN_CHEF_CALLED_ORDERS, kept until the party leaves
/// Bilibin) he shouts a string of orders and the kitchen worker answers, and
/// from then on he sends the party to the waitress
/// (CottageRoomsC_TalkToBusyChef).
///
/// gCottageRoomsCTriggers keeps both times in one table, in the ROM's order:
/// every row that needs FLAG_KOLIMA_CURSE_LIFTED clear comes before the rows
/// for afterwards, so the first row that matches picks the time. The merchants
/// and the innkeeper open the shop or the inn only when the leader faces them
/// across the counter (facing north), and otherwise talk of the time; Mind Read
/// on the villager who admires Mia depends on whether she is in the party.
/// Searching finds an Antidote, a Sleep Bomb and a Hard Nut, egg soup and
/// omelettes in the ovens, and a book called "All About Items".
/// CottageRoomsC_Initialize opens the rooms with a window around the leader
/// and draws one diner, ACTOR_BILIBIN_DINER, at a fixed sprite priority. He
/// stands at the north edge of the restaurant's table, on cell (7, 49), which
/// alone of the diners' cells gives an actor standing on it priority 3
/// (map_032's map.txt). Taken from that cell, his priority would
/// put him behind the middle layer (`BG2`, priority 2 in this map); the fixed
/// priority 2 draws him in front of it, as the other diners are.

#include "module/scene/cottage_rooms_c.h"
#include "flags.h"
#include "character_ids.h"
#include "module/field/field_sequence.h"
#include "module/field/event_commands.h"
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
#include "event.h"
#include "scene_tables.h"
#include "call.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "item_ids.h"

/// The head chef has shouted his string of orders once, so from then on he
/// sends the party to the waitress.
/// ★ Named from the game: "Two specials! One dinner! One sandwich! One dessert!
/// Hurry up!" (MsgBilibinTwoSpecialsOneDinnerOne)
#define FLAG_BILIBIN_CHEF_CALLED_ORDERS FLAG_AREA_TEMP_00

/// Bilibin's shops, as Shop_Run numbers the shops.
enum {
  SHOP_BILIBIN_WEAPONS = 7,
  SHOP_BILIBIN_ARMOR = 8,
  SHOP_BILIBIN_ITEMS = 9
};

/// Bilibin's inn, as Inn_CheckIn numbers the inns.
enum { BILIBIN_INN = 2 };

/// The placed actors, numbered in the order of
/// gCottageRoomsCActorPlacements from ACTOR_FIRST_PLACED. Each room's
/// actors are together: two in each house, then the shops' customers and
/// keepers, the inn and its restaurant, and the room behind the inn's inner
/// door.
enum CottageRoomsCActorId {
  /// A young man with brown hair, in the house east.
  /// ★ Named from the game: "That tree at the entrance is really a man cursed"
  /// (MsgBilibinThatTreeAtTheEntranceIs)
  ACTOR_BILIBIN_YOUNG_MAN_ON_THE_CURSED_MAN = ACTOR_FIRST_PLACED,
  /// A woman with fair hair.
  /// ★ Named from the game: "McCoy had the trees cut down so he could build"
  /// (MsgBilibinMccoyHadTheTreesCutDown)
  ACTOR_BILIBIN_VILLAGER_PALACE,
  /// A burly grey-haired man in the house west, who wants to see Mia again.
  /// ★ Named from the game: "When spring comes, I want to go see that angelic"
  /// (MsgBilibinWhenSpringComesWantGo)
  ACTOR_BILIBIN_MIA_ADMIRER,
  /// A small boy with fair hair.
  /// ★ Named from the game: "We rarely see any snow around Bilibin."
  /// (MsgBilibinWeRarelySeeAnySnowAround)
  ACTOR_BILIBIN_BOY_SNOW,
  /// A woman in a green dress, in the house north.
  /// ★ Named from the game: "How are people doing in Kolima?"
  /// (MsgBilibinHowArePeopleDoingInKolima)
  ACTOR_BILIBIN_VILLAGER_WORRIED_FOR_KOLIMA,
  /// A little girl with pigtails, who later asks whether the party knows what
  /// silk is.
  /// ★ Named from the game: "I know that guy who turned into the tree..."
  /// (MsgBilibinIKnowThatGuyWhoTurned)
  ACTOR_BILIBIN_GIRL_SILK,
  /// An old man with a cane, in the item shop.
  /// ★ Named from the game: "Long ago, the lighthouse near Imil held the"
  /// (MsgBilibinLongAgoTheLighthouseNearImil)
  ACTOR_BILIBIN_OLD_MAN_WATER_OF_HERMES,
  /// An old woman.
  /// ★ Named from the game: "But even the legendary Water of Hermes cannot"
  /// (MsgBilibinButEvenTheLegendaryWaterOf)
  ACTOR_BILIBIN_OLD_WOMAN_LOST_YOUTH,
  /// ★ Named from the game: "There was an able healer in a village called
  /// Vale," (MsgBilibinThereWasAbleHealerIn)
  ACTOR_BILIBIN_WEAPON_MERCHANT,
  /// ★ Named from the game: "Have you ever heard of the 'ocean'?"
  /// (MsgBilibinHaveEverHeardOcean)
  ACTOR_BILIBIN_ARMOR_MERCHANT,
  /// ★ Named from the game: "You must be used to all types of danger,"
  /// (MsgBilibinMustUsedTypesDangerBeing)
  ACTOR_BILIBIN_ITEM_MERCHANT,
  /// ★ Named from the game: "A curse on Kolima? What a scary development!"
  /// (MsgBilibinCurseOnKolimaScaryDevelopment)
  ACTOR_BILIBIN_INNKEEPER,
  /// Beside the innkeeper.
  /// ★ Named from the game: "You can't reach Kolima, no matter how you try."
  /// (MsgBilibinYouCantReachKolimaNoMatter)
  ACTOR_BILIBIN_INN_WORKER,
  /// The restaurant's head chef, born in Kolima.
  /// ★ Named from the game: "Hey, Mister Head Chef!"
  /// (MsgBilibinHeyMisterHeadChefAreYou)
  ACTOR_BILIBIN_CHEF,
  /// A second cook in a tall chef's hat.
  /// ★ Named from the game: "I really think the head chef has changed."
  /// (MsgBilibinReallyThinkHeadChefHas)
  ACTOR_BILIBIN_KITCHEN_WORKER,
  /// A young woman with long brown hair.
  /// ★ Named from the game: "Let's see, serve them water...then get the
  /// silverware." (MsgBilibinLetsSeeServeThemWater)
  ACTOR_BILIBIN_WAITRESS,
  /// A man with a pack, waiting for his dinner.
  /// ★ Named from the game: "Grrr..." (MsgBilibinGrrr)
  ACTOR_BILIBIN_HUNGRY_DINER,
  /// A diner with a pack.
  /// ★ Named from the game: "I came to Bilibin hoping to sell furniture."
  /// (MsgBilibinICameToBilibinHopingTo)
  ACTOR_BILIBIN_FURNITURE_SELLER,
  /// A strongman at the table.
  /// ★ Named from the game: "I think that big tree deep in the forest"
  /// (MsgBilibinIThinkThatBigTreeDeep)
  ACTOR_BILIBIN_DINER_BIG_TREE,
  /// A diner with a pickaxe, drawn at a fixed sprite priority.
  /// ★ Named from the game: "Do you believe a tree spirit can curse a man?"
  /// (MsgBilibinDoBelieveTreeSpiritCan)
  ACTOR_BILIBIN_DINER,
  /// An old man at the table.
  /// ★ Named from the game: "I can't wait to see today's special!"
  /// (MsgBilibinICantWaitToSeeTodays)
  ACTOR_BILIBIN_DINER_TODAYS_SPECIAL,
  /// A man in a red bandana, in the room behind the inn's inner door.
  /// ★ Named from the game: "I went to Lord McCoy looking for work..."
  /// (MsgBilibinIWentToLordMccoyLooking)
  ACTOR_BILIBIN_JOB_SEEKER,
  /// A man with a lute.
  /// ★ Named from the game: "I've heard songs that tell of ancient trees"
  /// (MsgBilibinIveHeardSongsThatTellOf)
  ACTOR_BILIBIN_VILLAGER_ANCIENT_TREES
};

const struct SceneEntrance *CottageRoomsC_GetEntrances(void) {
  return gCottageRoomsCEntrances;
}

const struct SceneExitRegion *CottageRoomsC_GetRegions(void) { return 0; }

const u32 *CottageRoomsC_GetExits(void) { return gCottageRoomsCExits; }

/// Marks actor placements outside the current room's view before
/// actors are spawned.
const struct ActorPlacement *CottageRoomsC_GetActorPlacements(void) {
  Scene_AssignViewFlags(gCottageRoomsCActorPlacements);
  return gCottageRoomsCActorPlacements;
}

/// Bilibin's weapon merchant: across the counter the shop opens; otherwise,
/// before the curse is lifted, the merchant mourns Vale's lost healer, and
/// after, recommends Altin's store.
static void CottageRoomsC_TalkToWeaponMerchant(void) {
  u32 leaderFacing;

  leaderFacing = Actor_GetById(ACTOR_PARTY_LEADER)->facing;
  Event_Start();

  if (FACING_IS_NORTH(leaderFacing)) {
    Shop_Run(SHOP_BILIBIN_WEAPONS, ACTOR_BILIBIN_WEAPON_MERCHANT);
  } else {
    if (GameFlag_Get(FLAG_KOLIMA_CURSE_LIFTED) == 0) {
      Event_SetMessageId(MsgBilibinThereWasAbleHealerIn);
    } else {
      Event_SetMessageId(MsgBilibinOurWeaponsBestCanFind);
    }
    Event_ShowMessageAndWait(ACTOR_BILIBIN_WEAPON_MERCHANT, 0);
  }

  Event_End();
}

/// Bilibin's item merchant: across the counter the shop opens; otherwise,
/// before the curse is lifted, the merchant says Lord McCoy could use
/// travelers like the party, and after, that his hired warriors were useless.
static void CottageRoomsC_TalkToItemMerchant(void) {
  u32 leaderFacing;

  leaderFacing = Actor_GetById(ACTOR_PARTY_LEADER)->facing;
  Event_Start();

  if (FACING_IS_NORTH(leaderFacing)) {
    Shop_Run(SHOP_BILIBIN_ITEMS, ACTOR_BILIBIN_ITEM_MERCHANT);
  } else {
    if (GameFlag_Get(FLAG_KOLIMA_CURSE_LIFTED) == 0) {
      Event_SetMessageId(MsgBilibinMustUsedTypesDangerBeing);
    } else {
      Event_SetMessageId(MsgBilibinTurnedOutWarriorsHiredBy);
    }
    Event_ShowMessageAndWait(ACTOR_BILIBIN_ITEM_MERCHANT, 0);
  }

  Event_End();
}

/// Returns both story stages' triggers; conditions choose the current
/// dialogue.
const struct Trigger *CottageRoomsC_GetTriggers(void) {
  return gCottageRoomsCTriggers;
}

/// Bilibin's armor merchant: across the counter the shop opens; otherwise,
/// before the curse is lifted, the merchant asks whether the party has heard
/// of the ocean, and after, dreams aloud of it.
static void CottageRoomsC_TalkToArmorMerchant(void) {
  u32 leaderFacing;

  leaderFacing = Actor_GetById(ACTOR_PARTY_LEADER)->facing;
  Event_Start();
  if (FACING_IS_NORTH(leaderFacing)) {
    Shop_Run(SHOP_BILIBIN_ARMOR, ACTOR_BILIBIN_ARMOR_MERCHANT);
  } else {
    if (GameFlag_Get(FLAG_KOLIMA_CURSE_LIFTED) == 0) {
      Event_SetMessageId(MsgBilibinHaveEverHeardOcean);
      Actor_FaceActor(ACTOR_BILIBIN_ARMOR_MERCHANT, ACTOR_PARTY_LEADER, 0);
      Event_Wait(10);
      Event_AskYesNo(ACTOR_BILIBIN_ARMOR_MERCHANT, 0);
      Actor_FaceDirection(ACTOR_BILIBIN_ARMOR_MERCHANT,
                          FACING_SOUTHEAST + FACING_STEP, 10);
    } else {
      Event_SetMessageId(MsgBilibinForSomeReasonOceanFills);
      Event_ShowMessage(ACTOR_BILIBIN_ARMOR_MERCHANT, 0);
    }
  }
  Event_End();
}

/// The head chef in Bilibin's restaurant, before the curse is lifted: born in
/// Kolima, he is too worried to focus on his cooking.
static void CottageRoomsC_TalkToWorriedChef(void) {
  Event_Start();
  Event_SetMessageId(MsgBilibinImReallyWorriedAboutKolima);
  Actor_FaceActor(ACTOR_BILIBIN_CHEF, ACTOR_PARTY_LEADER, 0);
  Event_ShowMessage(ACTOR_BILIBIN_CHEF, 0);
  Actor_FaceDirection(ACTOR_BILIBIN_CHEF, FACING_NORTH, 10);
  Event_End();
}

/// A diner in Bilibin's restaurant, before the curse is lifted, still waiting
/// for his dinner: his stomach growls, and he asks whether the party heard it.
static void CottageRoomsC_TalkToHungryDiner(void) {
  Event_Start();
  Event_SetMessageId(MsgBilibinGrrr);
  FieldEvent_ShowMessageThenWait(ACTOR_BILIBIN_HUNGRY_DINER, 0, 20);
  Actor_FaceActor(ACTOR_BILIBIN_HUNGRY_DINER, ACTOR_PARTY_LEADER, 0);
  Event_Wait(10);
  Event_OpenMessage(ACTOR_BILIBIN_HUNGRY_DINER, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) != 0) {
    gFieldWork->message++;
  }
  Event_ShowMessage(ACTOR_BILIBIN_HUNGRY_DINER, 0);
  Actor_FaceDirection(ACTOR_BILIBIN_HUNGRY_DINER, FACING_SOUTH, 10);
  Event_End();
}

/// A diner asks whether a tree spirit can curse a man. The diner sits in
/// Bilibin's restaurant before the curse is lifted; on yes, sends the party to
/// hear Lord McCoy's story. Then the diner turns back to the table.
static void CottageRoomsC_AskAboutTreeSpirit(void) {
  Event_Start();
  Event_SetMessageId(MsgBilibinDoBelieveTreeSpiritCan);
  Actor_FaceActor(ACTOR_BILIBIN_DINER, ACTOR_PARTY_LEADER, 0);
  Event_Wait(10);
  Event_OpenMessage(ACTOR_BILIBIN_DINER, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) != 0) {
    gFieldWork->message++;
  }
  Event_ShowMessage(ACTOR_BILIBIN_DINER, 0);
  Actor_FaceDirection(ACTOR_BILIBIN_DINER, FACING_SOUTH, 10);
  Event_End();
}

/// A young man asks whether Kolima Forest's holy tree survived. He's in Bilibin
/// once the curse is lifted.
static void CottageRoomsC_AskAboutHolyTree(void) {
  Event_Start();
  Event_SetMessageId(MsgBilibinDoKnowIfHolyTree);
  Event_AskYesNoAndShowResponse(ACTOR_BILIBIN_YOUNG_MAN_ON_THE_CURSED_MAN, 0);
  Event_End();
}

/// A little girl in Bilibin, once the curse is lifted, asks whether the party
/// knows what silk is; on yes, that Xian, across the river, makes it, at the
/// start of Silk Road.
static void CottageRoomsC_AskAboutSilk(void) {
  Event_Start();
  Event_SetMessageId(MsgBilibinDoKnowSilk);
  Event_AskYesNoAndShowResponse(ACTOR_BILIBIN_GIRL_SILK, 0);
  Event_End();
}

/// Bilibin's innkeeper: across the counter the inn opens; otherwise, before
/// the curse is lifted, the innkeeper is scared of the curse on Kolima, and
/// after, asks whether it wasn't horrifying.
static void CottageRoomsC_TalkToInnkeeper(void) {
  u32 leaderFacing;

  leaderFacing = Actor_GetById(ACTOR_PARTY_LEADER)->facing;
  Event_Start();

  if (FACING_IS_NORTH(leaderFacing)) {
    Inn_CheckIn(BILIBIN_INN, ACTOR_BILIBIN_INNKEEPER);
  } else if (GameFlag_Get(FLAG_KOLIMA_CURSE_LIFTED) != 0) {
    Event_SetMessageId(MsgBilibinWasntCurseInKolimaHorrifying);
    Event_AskYesNoAndShowResponse(ACTOR_BILIBIN_INNKEEPER, 0);
  } else {
    Event_SetMessageId(MsgBilibinCurseOnKolimaScaryDevelopment);
    Event_ShowMessageAndWait(ACTOR_BILIBIN_INNKEEPER, 0);
  }

  Event_End();
}

/// The head chef in Bilibin's restaurant, once the curse is lifted: he calls
/// out orders once, then sends the party to the waitress.
static void CottageRoomsC_TalkToBusyChef(void) {
  Event_Start();
  // The test is event.h's inline and the set below a direct call,
  // each as it matches: an inline call's argument is a pseudo of its own,
  // where a direct call keeps the flag number in a callee-saved register.
  if (Event_GetFlag(FLAG_BILIBIN_CHEF_CALLED_ORDERS) == 0) {
    // The first time (FLAG_BILIBIN_CHEF_CALLED_ORDERS, kept until the party
    // leaves Bilibin), the chef shouts orders and the kitchen worker answers.
    Event_SetMessageId(MsgBilibinTwoSpecialsOneDinnerOne);
    Event_ShowMessage(ACTOR_BILIBIN_CHEF, 0);
    Actor_FaceDirection(ACTOR_BILIBIN_CHEF, FACING_WEST, 20);
    Event_ShowMessage(ACTOR_BILIBIN_CHEF, 0);
    Actor_StartRepeatedMotion(ACTOR_BILIBIN_KITCHEN_WORKER, 2);
    Actor_SetAttachedEffect(ACTOR_BILIBIN_KITCHEN_WORKER,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Event_Wait(60);
    Event_ShowMessage(ACTOR_BILIBIN_KITCHEN_WORKER, 0);
    Event_Wait(10);
    GameFlag_Set(FLAG_BILIBIN_CHEF_CALLED_ORDERS);
  }
  // Then, as every later time, he sends the party to the waitress.
  Actor_FaceActor(ACTOR_BILIBIN_CHEF, ACTOR_PARTY_LEADER, 0);
  Event_SetMessageId(MsgBilibinIfWantMealSpeakWaitress);
  Event_ShowMessage(ACTOR_BILIBIN_CHEF, 0);
  Actor_FaceDirection(ACTOR_BILIBIN_CHEF, FACING_NORTH, 10);
  Event_End();
}

/// The kitchen worker can't keep up with the head chef's energy. This is in
/// Bilibin's restaurant once the curse is lifted.
static void CottageRoomsC_TalkToKitchenWorker(void) {
  Event_Start();
  Event_SetMessageId(MsgBilibinReallyThinkHeadChefHas);
  Event_ShowMessageAndWait(ACTOR_BILIBIN_KITCHEN_WORKER, 0);
  Actor_SetAngleToward(ACTOR_BILIBIN_KITCHEN_WORKER, ACTOR_PARTY_LEADER, 0);
  Event_ShowMessageAndWait(ACTOR_BILIBIN_KITCHEN_WORKER, 0);
  Actor_ArmCallback(ACTOR_BILIBIN_KITCHEN_WORKER, FACING_EAST, 10);
  Event_End();
}

/// The waitress hurries through her work before the head chef yells. She works
/// in Bilibin's restaurant once the curse is lifted.
static void CottageRoomsC_TalkToWaitress(void) {
  Event_Start();
  Event_SetMessageId(MsgBilibinLetsSeeServeThemWater);
  Event_ShowMessage(ACTOR_BILIBIN_WAITRESS, 0);
  Actor_FaceActor(ACTOR_BILIBIN_WAITRESS, ACTOR_PARTY_LEADER, 0);
  Event_ShowMessage(ACTOR_BILIBIN_WAITRESS, 0);
  Actor_FaceDirection(ACTOR_BILIBIN_WAITRESS, FACING_NORTH, 10);
  Event_End();
}

/// A diner loves the meal and asks whether the party does too. The diner is in
/// Bilibin's restaurant once the curse is lifted and the food flows.
static void CottageRoomsC_TalkAboutMeal(void) {
  Event_Start();
  Event_SetMessageId(MsgBilibinWasSomeMealDontJust);
  Event_AskYesNoAndShowResponse(ACTOR_BILIBIN_DINER, 0);
  Event_End();
}

/// Mind Read on the burly man in Bilibin who admires Mia, before the curse is
/// lifted: with Mia in the party (FLAG_IN_PARTY says who is), even frozen Imil
/// must feel warm with her around; without her, he hopes to see her
/// again in spring.
static void CottageRoomsC_ReadMiaAdmirersMind(void) {
  Event_Start();
  if (GameFlag_Get(FLAG_IN_PARTY(MIA)) != 0) {
    Event_SetMessageId(MsgBilibinEvenFrozenImilMustFeel);
  } else {
    Event_SetMessageId(MsgBilibinWhenSpringComesWantGo);
  }
  Event_ShowMessageAndWait(ACTOR_BILIBIN_MIA_ADMIRER, 0);
  Event_End();
}

/// Sets up Bilibin's buildings: the rooms open through a window, and the diner
/// at the restaurant's table is drawn at sprite priority 2, never by the
/// automatic priority.
s32 CottageRoomsC_Initialize(void) {
  struct Actor *actor;

  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  actor = Actor_GetById(ACTOR_BILIBIN_DINER);
  actor->priorityFlags = 0;
  actor->sprite->image.priority = 2;
  return 0;
}

struct SceneEntrance gCottageRoomsCEntrances[] = {
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
    {SCENE_TABLE_END},
};

u32 gCottageRoomsCExits[] = {
    SCENE_EXITS(CottageRoomsC),
    SCENE_EXIT(COTTAGE_ROOMS_C_HOUSE_EAST_TO_BILIBIN, Bilibin,
               BILIBIN_FROM_HOUSE_EAST),
    SCENE_EXIT(COTTAGE_ROOMS_C_HOUSE_WEST_TO_BILIBIN, Bilibin,
               BILIBIN_FROM_HOUSE_WEST),
    SCENE_EXIT(COTTAGE_ROOMS_C_HOUSE_NORTH_TO_BILIBIN, Bilibin,
               BILIBIN_FROM_HOUSE_NORTH),
    SCENE_EXIT(COTTAGE_ROOMS_C_WEAPON_AND_ARMOR_SHOP_TO_BILIBIN, Bilibin,
               BILIBIN_FROM_WEAPON_AND_ARMOR_SHOP),
    SCENE_EXIT(COTTAGE_ROOMS_C_ITEM_SHOP_TO_BILIBIN, Bilibin,
               BILIBIN_FROM_ITEM_SHOP),
    SCENE_EXIT(COTTAGE_ROOMS_C_INN_TO_BILIBIN, Bilibin, BILIBIN_FROM_INN),
    SCENE_EXIT(COTTAGE_ROOMS_C_INN_UPSTAIRS_TO_INN, CottageRoomsC,
               COTTAGE_ROOMS_C_INN_FROM_INN_UPSTAIRS),
    SCENE_EXIT(COTTAGE_ROOMS_C_INN_TO_INN_UPSTAIRS, CottageRoomsC,
               COTTAGE_ROOMS_C_INN_UPSTAIRS_FROM_INN),
    SCENE_EXITS_END,
};

// clang-format off
struct ActorPlacement gCottageRoomsCActorPlacements[] = {
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        PIXELS(104), 0, PIXELS(168), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_WANDER,
        PIXELS(200), 0, PIXELS(136), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        PIXELS(384), 0, PIXELS(192), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_WANDER,
        PIXELS(360), 0, PIXELS(136), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        PIXELS(640), 0, PIXELS(156), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_GIRL_WITH_PIGTAILS, CONDITION_IN_VIEW, MOVEMENT_TYPE_WANDER,
        PIXELS(704), 0, PIXELS(200), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        PIXELS(456), 0, PIXELS(472), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        PIXELS(488), 0, PIXELS(424), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_BALD_MAN_IN_RED, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        PIXELS(200), 0, PIXELS(440), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_IN_GREEN, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        PIXELS(168), 0, PIXELS(440), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_WITH_PONYTAIL, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        PIXELS(584), 0, PIXELS(456), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_MAN_WITH_DARK_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        PIXELS(328), 0, PIXELS(744), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_IN_PINK_APRON, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        PIXELS(280), 0, PIXELS(760), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_CHEF, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        PIXELS(88), 0, PIXELS(760), FACING_NORTH, TALK_FACING_KEEP, 0 },
    { SPRITE_CHEF, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        PIXELS(56), 0, PIXELS(760), FACING_EAST, TALK_FACING_KEEP, 0 },
    { SPRITE_YOUNG_WOMAN_IN_GREEN, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        PIXELS(72), 0, PIXELS(776), FACING_NORTHEAST, TALK_FACING_KEEP, 0 },
    { SPRITE_MAN_CARRYING_A_PACK, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        PIXELS(176), 0, PIXELS(744), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_MAN_CARRYING_A_PACK, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        PIXELS(152), 0, PIXELS(768), FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_STRONGMAN, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        PIXELS(152), 0, PIXELS(832), FACING_NORTHWEST, TALK_FACING_KEEP, 0 },
    { SPRITE_MAN_WITH_PICKAXE, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        PIXELS(120), 0, PIXELS(796), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        PIXELS(88), 0, PIXELS(840), FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_MAN_IN_RED_BANDANA, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        PIXELS(808), 0, PIXELS(774), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_WITH_LUTE, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        PIXELS(568), 0, PIXELS(768), FACING_SOUTHEAST, TALK_FACING_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct Trigger gCottageRoomsCTriggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, COTTAGE_ROOMS_C_HOUSE_EAST_TO_BILIBIN },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, COTTAGE_ROOMS_C_HOUSE_WEST_TO_BILIBIN },
    { TRIGGER_EXIT, 3, CONDITION_ALWAYS, COTTAGE_ROOMS_C_HOUSE_NORTH_TO_BILIBIN },
    { TRIGGER_EXIT, 4, CONDITION_ALWAYS, COTTAGE_ROOMS_C_WEAPON_AND_ARMOR_SHOP_TO_BILIBIN },
    { TRIGGER_EXIT, 5, CONDITION_ALWAYS, COTTAGE_ROOMS_C_ITEM_SHOP_TO_BILIBIN },
    { TRIGGER_EXIT, 6, CONDITION_ALWAYS, COTTAGE_ROOMS_C_INN_TO_BILIBIN },
    { TRIGGER_EXIT, 7, CONDITION_ALWAYS, COTTAGE_ROOMS_C_INN_UPSTAIRS_TO_INN },
    { TRIGGER_EXIT, 8, CONDITION_ALWAYS, COTTAGE_ROOMS_C_INN_TO_INN_UPSTAIRS },
    { TRIGGER_TALK, ACTOR_BILIBIN_YOUNG_MAN_ON_THE_CURSED_MAN, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinThatTreeAtTheEntranceIs) },
    { TRIGGER_TALK, ACTOR_BILIBIN_VILLAGER_PALACE, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinMccoyHadTheTreesCutDown) },
    { TRIGGER_TALK, ACTOR_BILIBIN_MIA_ADMIRER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinTheSnowIsAlreadyPilingUp) },
    { TRIGGER_TALK, ACTOR_BILIBIN_BOY_SNOW, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinWeRarelySeeAnySnowAround) },
    { TRIGGER_TALK, ACTOR_BILIBIN_VILLAGER_WORRIED_FOR_KOLIMA, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinHowArePeopleDoingInKolima) },
    { TRIGGER_TALK, ACTOR_BILIBIN_GIRL_SILK, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinIKnowThatGuyWhoTurned) },
    { TRIGGER_TALK, ACTOR_BILIBIN_OLD_MAN_WATER_OF_HERMES, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinLongAgoTheLighthouseNearImil) },
    { TRIGGER_TALK, ACTOR_BILIBIN_OLD_WOMAN_LOST_YOUTH, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinButEvenTheLegendaryWaterOf) },
    { TRIGGER_TALK, ACTOR_BILIBIN_WEAPON_MERCHANT, FLAG_KOLIMA_CURSE_LIFTED, EVENT(CottageRoomsC_TalkToWeaponMerchant) },
    { TRIGGER_TALK, ACTOR_BILIBIN_ARMOR_MERCHANT, FLAG_KOLIMA_CURSE_LIFTED, EVENT(CottageRoomsC_TalkToArmorMerchant) },
    { TRIGGER_TALK, ACTOR_BILIBIN_ITEM_MERCHANT, FLAG_KOLIMA_CURSE_LIFTED, EVENT(CottageRoomsC_TalkToItemMerchant) },
    { TRIGGER_TALK, ACTOR_BILIBIN_INNKEEPER, FLAG_KOLIMA_CURSE_LIFTED, EVENT(CottageRoomsC_TalkToInnkeeper) },
    { TRIGGER_TALK, ACTOR_BILIBIN_INN_WORKER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinYouCantReachKolimaNoMatter) },
    { TRIGGER_TALK, ACTOR_BILIBIN_CHEF, FLAG_KOLIMA_CURSE_LIFTED, EVENT(CottageRoomsC_TalkToWorriedChef) },
    { TRIGGER_TALK, ACTOR_BILIBIN_KITCHEN_WORKER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinHeyMisterHeadChefAreYou) },
    { TRIGGER_TALK, ACTOR_BILIBIN_WAITRESS, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinStartCookingAlready) },
    { TRIGGER_TALK, ACTOR_BILIBIN_HUNGRY_DINER, FLAG_KOLIMA_CURSE_LIFTED, EVENT(CottageRoomsC_TalkToHungryDiner) },
    { TRIGGER_TALK, ACTOR_BILIBIN_FURNITURE_SELLER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinWeCameHereBecauseLordMccoy) },
    { TRIGGER_TALK, ACTOR_BILIBIN_DINER_BIG_TREE, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinIThinkThatBigTreeDeep) },
    { TRIGGER_TALK, ACTOR_BILIBIN_DINER, FLAG_KOLIMA_CURSE_LIFTED, EVENT(CottageRoomsC_AskAboutTreeSpirit) },
    { TRIGGER_TALK, ACTOR_BILIBIN_DINER_TODAYS_SPECIAL, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinICantWaitToSeeTodays) },
    { TRIGGER_TALK, ACTOR_BILIBIN_JOB_SEEKER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinIWentToLordMccoyLooking) },
    { TRIGGER_TALK, ACTOR_BILIBIN_VILLAGER_ANCIENT_TREES, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinIveHeardSongsThatTellOf) },
    { TRIGGER_TALK, ACTOR_BILIBIN_YOUNG_MAN_ON_THE_CURSED_MAN, CONDITION_ALWAYS, EVENT(CottageRoomsC_AskAboutHolyTree) },
    { TRIGGER_TALK, ACTOR_BILIBIN_VILLAGER_PALACE, CONDITION_ALWAYS, MSGNO(MsgBilibinThankGoodnessTheyStoppedConstructionOf) },
    { TRIGGER_TALK, ACTOR_BILIBIN_MIA_ADMIRER, CONDITION_ALWAYS, MSGNO(MsgBilibinIAlwaysMissAltinInThe) },
    { TRIGGER_TALK, ACTOR_BILIBIN_BOY_SNOW, CONDITION_ALWAYS, MSGNO(MsgBilibinMyFatherSaysThatItsWarm) },
    { TRIGGER_TALK, ACTOR_BILIBIN_VILLAGER_WORRIED_FOR_KOLIMA, CONDITION_ALWAYS, MSGNO(MsgBilibinTheRoadToKolimaIsOpen) },
    { TRIGGER_TALK, ACTOR_BILIBIN_GIRL_SILK, CONDITION_ALWAYS, EVENT(CottageRoomsC_AskAboutSilk) },
    { TRIGGER_TALK, ACTOR_BILIBIN_OLD_MAN_WATER_OF_HERMES, CONDITION_ALWAYS, MSGNO(MsgBilibinIThoughtISawLightsIn) },
    { TRIGGER_TALK, ACTOR_BILIBIN_OLD_WOMAN_LOST_YOUTH, CONDITION_ALWAYS, MSGNO(MsgBilibinHisEyesAreRedBecauseHe) },
    { TRIGGER_TALK, ACTOR_BILIBIN_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT(CottageRoomsC_TalkToWeaponMerchant) },
    { TRIGGER_TALK, ACTOR_BILIBIN_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT(CottageRoomsC_TalkToArmorMerchant) },
    { TRIGGER_TALK, ACTOR_BILIBIN_ITEM_MERCHANT, CONDITION_ALWAYS, EVENT(CottageRoomsC_TalkToItemMerchant) },
    { TRIGGER_TALK, ACTOR_BILIBIN_INNKEEPER, CONDITION_ALWAYS, EVENT(CottageRoomsC_TalkToInnkeeper) },
    { TRIGGER_TALK, ACTOR_BILIBIN_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgBilibinThereWillBeFewerGuestsNow) },
    { TRIGGER_TALK, ACTOR_BILIBIN_CHEF, CONDITION_ALWAYS, EVENT(CottageRoomsC_TalkToBusyChef) },
    { TRIGGER_TALK, ACTOR_BILIBIN_KITCHEN_WORKER, CONDITION_ALWAYS, EVENT(CottageRoomsC_TalkToKitchenWorker) },
    { TRIGGER_TALK, ACTOR_BILIBIN_WAITRESS, CONDITION_ALWAYS, EVENT(CottageRoomsC_TalkToWaitress) },
    { TRIGGER_TALK, ACTOR_BILIBIN_HUNGRY_DINER, CONDITION_ALWAYS, MSGNO(MsgBilibinImALittleSurprisedThatThe) },
    { TRIGGER_TALK, ACTOR_BILIBIN_FURNITURE_SELLER, CONDITION_ALWAYS, MSGNO(MsgBilibinTheresNoBusinessHereSoIm) },
    { TRIGGER_TALK, ACTOR_BILIBIN_DINER_BIG_TREE, CONDITION_ALWAYS, MSGNO(MsgBilibinTheyServedSoMuchFoodToday) },
    { TRIGGER_TALK, ACTOR_BILIBIN_DINER, CONDITION_ALWAYS, EVENT(CottageRoomsC_TalkAboutMeal) },
    { TRIGGER_TALK, ACTOR_BILIBIN_DINER_TODAYS_SPECIAL, CONDITION_ALWAYS, MSGNO(MsgBilibinOooohThatsItITriedMy) },
    { TRIGGER_TALK, ACTOR_BILIBIN_JOB_SEEKER, CONDITION_ALWAYS, MSGNO(MsgBilibinTheIncidentWasResolvedBeforeI) },
    { TRIGGER_TALK, ACTOR_BILIBIN_VILLAGER_ANCIENT_TREES, CONDITION_ALWAYS, MSGNO(MsgBilibinIWonderWhatThatEerieHowling) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BILIBIN_YOUNG_MAN_ON_THE_CURSED_MAN, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinThisIsAllLordMccoysFault) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BILIBIN_VILLAGER_PALACE, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinAhLadyMccoyIWouldJust) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BILIBIN_MIA_ADMIRER, FLAG_KOLIMA_CURSE_LIFTED, EVENT(CottageRoomsC_ReadMiaAdmirersMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BILIBIN_BOY_SNOW, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinItMustBeDifficultToLive) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BILIBIN_VILLAGER_WORRIED_FOR_KOLIMA, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinICantImagineThatEveryoneIn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BILIBIN_GIRL_SILK, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinHeSeemedSoHappyAndThen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BILIBIN_OLD_MAN_WATER_OF_HERMES, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinCouldTheWaterOfHermesSave) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BILIBIN_OLD_WOMAN_LOST_YOUTH, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinTheWaterOfHermesVanishedLong) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BILIBIN_WEAPON_MERCHANT, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinTheWorldWillNeverSeeSuch) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BILIBIN_ARMOR_MERCHANT, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinIWishICouldSeeAnother) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BILIBIN_ITEM_MERCHANT, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinOnlySomeoneReallyBraveWouldGo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BILIBIN_INNKEEPER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinIDontBelieveInCursesFrom) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BILIBIN_INN_WORKER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinAnyTravelersHeadingEastAreStuck) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BILIBIN_CHEF, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinFatherMotherIHopeTheyreAll) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BILIBIN_KITCHEN_WORKER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinShouldIJustForgetThatIm) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BILIBIN_WAITRESS, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinIveHadAllTheCustomerComplaints) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BILIBIN_HUNGRY_DINER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinArghhhThatsItIJustWant) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BILIBIN_FURNITURE_SELLER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinICameToBilibinHopingTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BILIBIN_DINER_BIG_TREE, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinISeeWhyTheyCallThat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BILIBIN_DINER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinIDontCareWhoDoesIt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BILIBIN_DINER_TODAYS_SPECIAL, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinYippeeIsItReadyYetMy) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BILIBIN_JOB_SEEKER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinImGettingALittleScaredHearing) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BILIBIN_VILLAGER_ANCIENT_TREES, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinItHasBeenSaidThatA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BILIBIN_YOUNG_MAN_ON_THE_CURSED_MAN, CONDITION_ALWAYS, MSGNO(MsgBilibinItTakesManyYearsForA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BILIBIN_VILLAGER_PALACE, CONDITION_ALWAYS, MSGNO(MsgBilibinSometimesIWishICouldSee) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BILIBIN_MIA_ADMIRER, CONDITION_ALWAYS, MSGNO(MsgBilibinIWonderIfThereAreStill) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BILIBIN_BOY_SNOW, CONDITION_ALWAYS, MSGNO(MsgBilibinMyFatherHasTraveledAllOver) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BILIBIN_VILLAGER_WORRIED_FOR_KOLIMA, CONDITION_ALWAYS, MSGNO(MsgBilibinTravelingMakesYoungWarriorsStronger) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BILIBIN_GIRL_SILK, CONDITION_ALWAYS, MSGNO(MsgBilibinClothesMadeWithSilkAreSo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BILIBIN_OLD_MAN_WATER_OF_HERMES, CONDITION_ALWAYS, MSGNO(MsgBilibinTheresNoDoubtAboutItIts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BILIBIN_OLD_WOMAN_LOST_YOUTH, CONDITION_ALWAYS, MSGNO(MsgBilibinWhatsTheBigDealWhyAre) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BILIBIN_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgBilibinAltinsGotGoodWeaponsButI) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BILIBIN_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgBilibinIWishICouldGoOut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BILIBIN_ITEM_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgBilibinTheyCallThemselvesWarriorsButYou) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BILIBIN_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgBilibinWellIWasOnlyActingScared) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BILIBIN_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgBilibinItdBeBetterForBusinessIf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BILIBIN_CHEF, CONDITION_ALWAYS, MSGNO(MsgBilibinIllGoToKolimaWhenThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BILIBIN_KITCHEN_WORKER, CONDITION_ALWAYS, MSGNO(MsgBilibinLetsSeeSoupSaladAndI) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BILIBIN_WAITRESS, CONDITION_ALWAYS, MSGNO(MsgBilibinTheHeadChefWasSoLazy) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BILIBIN_HUNGRY_DINER, CONDITION_ALWAYS, MSGNO(MsgBilibinItUsedToTakeThemSo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BILIBIN_FURNITURE_SELLER, CONDITION_ALWAYS, MSGNO(MsgBilibinWhenTheyStoppedBuildingThePalace) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BILIBIN_DINER_BIG_TREE, CONDITION_ALWAYS, MSGNO(MsgBilibinIWishICouldEatLike) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BILIBIN_DINER, CONDITION_ALWAYS, MSGNO(MsgBilibinIdBetterEatALotThat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BILIBIN_DINER_TODAYS_SPECIAL, CONDITION_ALWAYS, MSGNO(MsgBilibinTheFoodIsSuchAGood) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BILIBIN_JOB_SEEKER, CONDITION_ALWAYS, MSGNO(MsgBilibinIfAKidLikeThisCould) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BILIBIN_VILLAGER_ANCIENT_TREES, CONDITION_ALWAYS, MSGNO(MsgBilibinItLooksLikeScaryThingsAre) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), 100, FLAG_BILIBIN_ANTIDOTE, FIND_ITEM(ITEM_ANTIDOTE) },
    { EXAMINE_TRIGGER(EXAMINE_BED), 101, FLAG_BILIBIN_SLEEP_BOMB, FIND_ITEM(ITEM_SLEEP_BOMB) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), 102, FLAG_BILIBIN_HARD_NUT, FIND_ITEM(ITEM_HARD_NUT) },
    { EXAMINE_TRIGGER(EXAMINE_OVEN), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBilibinLooksLikeEggSoupSoftEggs)) },
    { EXAMINE_TRIGGER(EXAMINE_OVEN), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBilibinFreshOmelettesBilibinoOmelettesUseWild)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBilibinTheresABookEntitledAllAbout)) },
    { TRIGGERS_END },
};
// clang-format on
