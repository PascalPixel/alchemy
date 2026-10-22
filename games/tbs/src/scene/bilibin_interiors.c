/// @file
/// Inside Bilibin's buildings: SceneId_BilibinInteriors, whose area name is
/// Bilibin. Six of Bilibin's doors (BILIBIN_TO_INN to
/// BILIBIN_TO_WEAPON_AND_ARMOR_SHOP, in bilibin.c) lead into its rooms, and
/// each room's door leads back out to the door it was entered by. One room
/// holds the weapon and armor shops, another the item shop, a third the inn
/// with the restaurant beside it, whose stairs
/// (BILIBIN_INTERIORS_INN_TO_INN_UPSTAIRS and
/// BILIBIN_INTERIORS_INN_UPSTAIRS_TO_INN) join it to its upstairs
/// rooms; three other buildings hold villagers. BilibinInteriors_GetPlacements
/// leaves out the actors of the rooms not in view.
///
/// Everyone has a line for while Kolima's curse holds (FLAG_KOLIMA_CURSE_LIFTED
/// clear) and one for after. Before, the townsfolk explain the tree at the
/// village entrance, McCoy's palace and the Water of Hermes, and in the
/// restaurant the head chef, born in Kolima, is too worried to cook. After,
/// the head chef cooks so hard his kitchen cannot keep up: the first time the
/// party talks to him (FLAG_PLACE_TEMP_00, kept until the party leaves
/// Bilibin) he shouts a string of orders and the kitchen worker answers, and
/// from then on he sends the party to the waitress (Event_TalkToBusyChef).
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
/// and draws one diner, BILIBIN_DINER, at a fixed sprite priority. He stands
/// at the north edge of the restaurant's table, on cell (7, 49), which alone
/// of the diners' cells gives an object standing on it priority 3 (map_032's
/// grid_metatile.tsv). Taken from that cell, his priority would put him behind
/// the middle layer (`BG2`, priority 2 in this map); the fixed priority 2
/// draws him in front of it, as the other diners are.

#include "unit/scene/bilibin_interiors.h"
#include "flags.h"
#include "character_ids.h"
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
#include "call.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "item_ids.h"

/// Bilibin's shops and inn, as Engine_ShopOpen and Engine_InnOpen number them.
enum {
  BILIBIN_WEAPON_SHOP = 7,
  BILIBIN_ARMOR_SHOP = 8,
  BILIBIN_ITEM_SHOP = 9,
  BILIBIN_INN = 2
};

/// The placed actors, numbered in the order of gBilibinInteriorsPlacements
/// from ACTOR_FIRST_PLACED. Each room's actors are together: two in each house,
/// then the shops' customers and keepers, the inn and its restaurant, and the
/// room behind the inn's inner door.
enum BilibinInteriorsActor {
  /// A young man with brown hair, in the house east.
  /// ★ Named from the game: "That tree at the entrance is really a man cursed"
  /// (MsgBilibinThatTreeAtTheEntranceIs)
  BILIBIN_YOUNG_MAN_ON_THE_CURSED_MAN = ACTOR_FIRST_PLACED,
  /// A woman with fair hair.
  /// ★ Named from the game: "McCoy had the trees cut down so he could build"
  /// (MsgBilibinMccoyHadTheTreesCutDown)
  BILIBIN_VILLAGER_PALACE,
  /// A burly grey-haired man in the house west, who wants to see Mia again.
  /// ★ Named from the game: "When spring comes, I want to go see that angelic"
  /// (MsgBilibinWhenSpringComesWantGo)
  BILIBIN_MIA_ADMIRER,
  /// A small boy with fair hair.
  /// ★ Named from the game: "We rarely see any snow around Bilibin."
  /// (MsgBilibinWeRarelySeeAnySnowAround)
  BILIBIN_BOY_SNOW,
  /// A woman in a green dress, in the house north.
  /// ★ Named from the game: "How are people doing in Kolima?"
  /// (MsgBilibinHowArePeopleDoingInKolima)
  BILIBIN_VILLAGER_WORRIED_FOR_KOLIMA,
  /// A little girl with pigtails, who later asks whether the party knows what
  /// silk is.
  /// ★ Named from the game: "I know that guy who turned into the tree..."
  /// (MsgBilibinIKnowThatGuyWhoTurned)
  BILIBIN_GIRL_SILK,
  /// An old man with a cane, in the item shop.
  /// ★ Named from the game: "Long ago, the lighthouse near Imil held the"
  /// (MsgBilibinLongAgoTheLighthouseNearImil)
  BILIBIN_OLD_MAN_WATER_OF_HERMES,
  /// An old woman.
  /// ★ Named from the game: "But even the legendary Water of Hermes cannot"
  /// (MsgBilibinButEvenTheLegendaryWaterOf)
  BILIBIN_OLD_WOMAN_LOST_YOUTH,
  /// ★ Named from the game: "There was an able healer in a village called
  /// Vale," (MsgBilibinThereWasAbleHealerIn)
  BILIBIN_WEAPON_MERCHANT,
  /// ★ Named from the game: "Have you ever heard of the 'ocean'?"
  /// (MsgBilibinHaveEverHeardOcean)
  BILIBIN_ARMOR_MERCHANT,
  /// ★ Named from the game: "You must be used to all types of danger,"
  /// (MsgBilibinMustUsedTypesDangerBeing)
  BILIBIN_ITEM_MERCHANT,
  /// ★ Named from the game: "A curse on Kolima? What a scary development!"
  /// (MsgBilibinCurseOnKolimaScaryDevelopment)
  BILIBIN_INNKEEPER,
  /// Beside the innkeeper.
  /// ★ Named from the game: "You can't reach Kolima, no matter how you try."
  /// (MsgBilibinYouCantReachKolimaNoMatter)
  BILIBIN_INN_WORKER,
  /// The restaurant's head chef, born in Kolima.
  /// ★ Named from the game: "Hey, Mister Head Chef!"
  /// (MsgBilibinHeyMisterHeadChefAreYou)
  BILIBIN_CHEF,
  /// A second cook in a tall chef's hat.
  /// ★ Named from the game: "I really think the head chef has changed."
  /// (MsgBilibinReallyThinkHeadChefHas)
  BILIBIN_KITCHEN_WORKER,
  /// A young woman with long brown hair.
  /// ★ Named from the game: "Let's see, serve them water...then get the
  /// silverware." (MsgBilibinLetsSeeServeThemWater)
  BILIBIN_WAITRESS,
  /// A man with a pack, waiting for his dinner.
  /// ★ Named from the game: "Grrr..." (MsgBilibinGrrr)
  BILIBIN_HUNGRY_DINER,
  /// A diner with a pack.
  /// ★ Named from the game: "I came to Bilibin hoping to sell furniture."
  /// (MsgBilibinICameToBilibinHopingTo)
  BILIBIN_FURNITURE_SELLER,
  /// A strongman at the table.
  /// ★ Named from the game: "I think that big tree deep in the forest"
  /// (MsgBilibinIThinkThatBigTreeDeep)
  BILIBIN_DINER_BIG_TREE,
  /// A diner with a pickaxe, drawn at a fixed sprite priority.
  /// ★ Named from the game: "Do you believe a tree spirit can curse a man?"
  /// (MsgBilibinDoBelieveTreeSpiritCan)
  BILIBIN_DINER,
  /// An old man at the table.
  /// ★ Named from the game: "I can't wait to see today's special!"
  /// (MsgBilibinICantWaitToSeeTodays)
  BILIBIN_DINER_TODAYS_SPECIAL,
  /// A man in a red bandana, in the room behind the inn's inner door.
  /// ★ Named from the game: "I went to Lord McCoy looking for work..."
  /// (MsgBilibinIWentToLordMccoyLooking)
  BILIBIN_JOB_SEEKER,
  /// A man with a lute.
  /// ★ Named from the game: "I've heard songs that tell of ancient trees"
  /// (MsgBilibinIveHeardSongsThatTellOf)
  BILIBIN_VILLAGER_ANCIENT_TREES
};

const struct SceneEntrance *BilibinInteriors_GetEntrances(void) {
  return gBilibinInteriorsEntrances;
}

const struct SceneRegion *BilibinInteriors_GetRegions(void) { return 0; }

const u32 *BilibinInteriors_GetExits(void) { return gBilibinInteriorsExits; }

/// Marks placements outside the current room's view before actors are spawned.
const struct ScenePlacement *BilibinInteriors_GetPlacements(void) {
  ScenePlacement_ClipToView(gBilibinInteriorsPlacements);
  return gBilibinInteriorsPlacements;
}

/// Bilibin's weapon merchant: across the counter the shop opens; otherwise,
/// before the curse is lifted, the merchant mourns Vale's lost healer, and
/// after, recommends Altin's store.
static void Event_TalkToWeaponMerchant(void) {
  u32 leaderFacing;

  leaderFacing = Object_GetById(ACTOR_PARTY_LEADER)->facing;
  Engine_EventBegin();

  if (FACING_IS_NORTH(leaderFacing)) {
    Engine_ShopOpen(BILIBIN_WEAPON_SHOP, BILIBIN_WEAPON_MERCHANT);
  } else {
    if (Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_LIFTED) == 0) {
      Engine_EventSetMessage(MsgBilibinThereWasAbleHealerIn);
    } else {
      Engine_EventSetMessage(MsgBilibinOurWeaponsBestCanFind);
    }
    Engine_EventShowMessage(BILIBIN_WEAPON_MERCHANT, 0);
  }

  Engine_EventEnd();
}

/// Bilibin's item merchant: across the counter the shop opens; otherwise,
/// before the curse is lifted, the merchant says Lord McCoy could use
/// travelers like the party, and after, that his hired warriors were useless.
static void Event_TalkToItemMerchant(void) {
  u32 leaderFacing;

  leaderFacing = Object_GetById(ACTOR_PARTY_LEADER)->facing;
  Engine_EventBegin();

  if (FACING_IS_NORTH(leaderFacing)) {
    Engine_ShopOpen(BILIBIN_ITEM_SHOP, BILIBIN_ITEM_MERCHANT);
  } else {
    if (Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_LIFTED) == 0) {
      Engine_EventSetMessage(MsgBilibinMustUsedTypesDangerBeing);
    } else {
      Engine_EventSetMessage(MsgBilibinTurnedOutWarriorsHiredBy);
    }
    Engine_EventShowMessage(BILIBIN_ITEM_MERCHANT, 0);
  }

  Engine_EventEnd();
}

/// Returns both story stages' events; conditions choose the current dialogue.
const struct SceneEvent *BilibinInteriors_GetEvents(void) {
  return gBilibinInteriorsEvents;
}

/// Bilibin's armor merchant: across the counter the shop opens; otherwise,
/// before the curse is lifted, the merchant asks whether the party has heard
/// of the ocean, and after, dreams aloud of it.
static void Event_TalkToArmorMerchant(void) {
  u32 leaderFacing;

  leaderFacing = Object_GetById(ACTOR_PARTY_LEADER)->facing;
  Engine_EventBegin();
  if (FACING_IS_NORTH(leaderFacing)) {
    Engine_ShopOpen(BILIBIN_ARMOR_SHOP, BILIBIN_ARMOR_MERCHANT);
  } else {
    if (GameFlag_IsSet(FLAG_KOLIMA_CURSE_LIFTED) == 0) {
      Engine_EventSetMessage(MsgBilibinHaveEverHeardOcean);
      Actor_FaceActor(BILIBIN_ARMOR_MERCHANT, ACTOR_PARTY_LEADER, 0);
      Engine_EventWait(10);
      Event_AskYesNo(BILIBIN_ARMOR_MERCHANT, 0);
      Actor_FaceDirection(BILIBIN_ARMOR_MERCHANT,
                          FACING_SOUTHEAST + FACING_STEP, 10);
    } else {
      Engine_EventSetMessage(MsgBilibinForSomeReasonOceanFills);
      Event_ShowMessage(BILIBIN_ARMOR_MERCHANT, 0);
    }
  }
  Engine_EventEnd();
}

/// The head chef in Bilibin's restaurant, before the curse is lifted: born in
/// Kolima, he is too worried to focus on his cooking.
static void Event_TalkToWorriedChef(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgBilibinImReallyWorriedAboutKolima);
  Actor_FaceActor(BILIBIN_CHEF, ACTOR_PARTY_LEADER, 0);
  Event_ShowMessage(BILIBIN_CHEF, 0);
  Actor_FaceDirection(BILIBIN_CHEF, FACING_NORTH, 10);
  Engine_EventEnd();
}

/// A diner in Bilibin's restaurant, before the curse is lifted, still waiting
/// for his dinner: his stomach growls, and he asks whether the party heard it.
static void Event_TalkToHungryDiner(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgBilibinGrrr);
  Event_ShowMessageAndWait(BILIBIN_HUNGRY_DINER, 0, 20);
  Actor_FaceActor(BILIBIN_HUNGRY_DINER, ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(10);
  Event_OpenMessage(BILIBIN_HUNGRY_DINER, 0);
  if (Engine_EventChooseYesNo(0, 0) != 0) {
    gEventWork->message++;
  }
  Event_ShowMessage(BILIBIN_HUNGRY_DINER, 0);
  Actor_FaceDirection(BILIBIN_HUNGRY_DINER, FACING_SOUTH, 10);
  Engine_EventEnd();
}

/// A diner asks whether a tree spirit can curse a man. The diner sits in
/// Bilibin's restaurant before the curse is lifted; on yes, sends the party to
/// hear Lord McCoy's story. Then the diner turns back to the table.
static void Event_AskAboutTreeSpirit(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgBilibinDoBelieveTreeSpiritCan);
  Actor_FaceActor(BILIBIN_DINER, ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(10);
  Event_OpenMessage(BILIBIN_DINER, 0);
  if (Engine_EventChooseYesNo(0, 0) != 0) {
    gEventWork->message++;
  }
  Event_ShowMessage(BILIBIN_DINER, 0);
  Actor_FaceDirection(BILIBIN_DINER, FACING_SOUTH, 10);
  Engine_EventEnd();
}

/// A young man asks whether Kolima Forest's holy tree survived. He's in Bilibin
/// once the curse is lifted.
static void Event_AskAboutHolyTree(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgBilibinDoKnowIfHolyTree);
  Engine_EventAskYesNo(BILIBIN_YOUNG_MAN_ON_THE_CURSED_MAN, 0);
  Engine_EventEnd();
}

/// A little girl in Bilibin, once the curse is lifted, asks whether the party
/// knows what silk is; on yes, that Xian, across the river, makes it, at the
/// start of Silk Road.
static void Event_AskAboutSilk(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgBilibinDoKnowSilk);
  Engine_EventAskYesNo(BILIBIN_GIRL_SILK, 0);
  Engine_EventEnd();
}

/// Bilibin's innkeeper: across the counter the inn opens; otherwise, before
/// the curse is lifted, the innkeeper is scared of the curse on Kolima, and
/// after, asks whether it wasn't horrifying.
static void Event_TalkToInnkeeper(void) {
  u32 leaderFacing;

  leaderFacing = Object_GetById(ACTOR_PARTY_LEADER)->facing;
  Engine_EventBegin();

  if (FACING_IS_NORTH(leaderFacing)) {
    Engine_InnOpen(BILIBIN_INN, BILIBIN_INNKEEPER);
  } else if (Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_LIFTED) != 0) {
    Engine_EventSetMessage(MsgBilibinWasntCurseInKolimaHorrifying);
    Engine_EventAskYesNo(BILIBIN_INNKEEPER, 0);
  } else {
    Engine_EventSetMessage(MsgBilibinCurseOnKolimaScaryDevelopment);
    Engine_EventShowMessage(BILIBIN_INNKEEPER, 0);
  }

  Engine_EventEnd();
}

/// The head chef in Bilibin's restaurant, once the curse is lifted: he calls
/// out orders once, then sends the party to the waitress.
static void Event_TalkToBusyChef(void) {
  Engine_EventBegin();
  if (GameFlag_IsSet(FLAG_PLACE_TEMP_00) == 0) {
    // The first time (FLAG_PLACE_TEMP_00, kept until the party leaves Bilibin),
    // the chef shouts orders and the kitchen worker answers.
    Engine_EventSetMessage(MsgBilibinTwoSpecialsOneDinnerOne);
    Event_ShowMessage(BILIBIN_CHEF, 0);
    Actor_FaceDirection(BILIBIN_CHEF, FACING_WEST, 20);
    Event_ShowMessage(BILIBIN_CHEF, 0);
    Engine_ActorStartRepeatedMotion(BILIBIN_KITCHEN_WORKER, 2);
    Actor_SetAttachedEffect(BILIBIN_KITCHEN_WORKER,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Engine_EventWait(60);
    Event_ShowMessage(BILIBIN_KITCHEN_WORKER, 0);
    Engine_EventWait(10);
    GameFlag_Set(FLAG_PLACE_TEMP_00);
  }
  // Then, as every later time, he sends the party to the waitress.
  Actor_FaceActor(BILIBIN_CHEF, ACTOR_PARTY_LEADER, 0);
  Engine_EventSetMessage(MsgBilibinIfWantMealSpeakWaitress);
  Event_ShowMessage(BILIBIN_CHEF, 0);
  Actor_FaceDirection(BILIBIN_CHEF, FACING_NORTH, 10);
  Engine_EventEnd();
}

/// The kitchen worker can't keep up with the head chef's energy. This is in
/// Bilibin's restaurant once the curse is lifted.
static void Event_TalkToKitchenWorker(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgBilibinReallyThinkHeadChefHas);
  Engine_EventShowMessage(BILIBIN_KITCHEN_WORKER, 0);
  Engine_ActorFaceActor(BILIBIN_KITCHEN_WORKER, ACTOR_PARTY_LEADER, 0);
  Engine_EventShowMessage(BILIBIN_KITCHEN_WORKER, 0);
  Engine_ActorFaceDirection(BILIBIN_KITCHEN_WORKER, FACING_EAST, 10);
  Engine_EventEnd();
}

/// The waitress hurries through her work before the head chef yells. She works
/// in Bilibin's restaurant once the curse is lifted.
static void Event_TalkToWaitress(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgBilibinLetsSeeServeThemWater);
  Event_ShowMessage(BILIBIN_WAITRESS, 0);
  Actor_FaceActor(BILIBIN_WAITRESS, ACTOR_PARTY_LEADER, 0);
  Event_ShowMessage(BILIBIN_WAITRESS, 0);
  Actor_FaceDirection(BILIBIN_WAITRESS, FACING_NORTH, 10);
  Engine_EventEnd();
}

/// A diner loves the meal and asks whether the party does too. The diner is in
/// Bilibin's restaurant once the curse is lifted and the food flows.
static void Event_TalkAboutMeal(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgBilibinWasSomeMealDontJust);
  Engine_EventAskYesNo(BILIBIN_DINER, 0);
  Engine_EventEnd();
}

/// Mind Read on the burly man in Bilibin who admires Mia, before the curse is
/// lifted: with Mia in the party (FLAG_IN_PARTY says who is), even frozen Imil
/// must feel warm with her around; without her, he hopes to see her
/// again in spring.
static void Event_ReadMiaAdmirersMind(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_IN_PARTY(MIA)) != 0) {
    Engine_EventSetMessage(MsgBilibinEvenFrozenImilMustFeel);
  } else {
    Engine_EventSetMessage(MsgBilibinWhenSpringComesWantGo);
  }
  Engine_EventShowMessage(BILIBIN_MIA_ADMIRER, 0);
  Engine_EventEnd();
}

/// Sets up Bilibin's buildings: the rooms open through a window, and the diner
/// at the restaurant's table is drawn at sprite priority 2, never by the
/// automatic priority.
s32 BilibinInteriors_Initialize(void) {
  struct FieldActor *actor;

  gEventWork->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  actor = Object_GetById(BILIBIN_DINER);
  actor->priorityFlags = 0;
  actor->sprite->image.priority = 2;
  return 0;
}

struct SceneEntrance gBilibinInteriorsEntrances[] = {
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
    {SCENE_TABLE_END},
};

u32 gBilibinInteriorsExits[] = {
    SCENE_EXITS(BilibinInteriors),
    SCENE_EXIT(BILIBIN_INTERIORS_HOUSE_EAST_TO_BILIBIN, Bilibin,
               BILIBIN_FROM_HOUSE_EAST),
    SCENE_EXIT(BILIBIN_INTERIORS_HOUSE_WEST_TO_BILIBIN, Bilibin,
               BILIBIN_FROM_HOUSE_WEST),
    SCENE_EXIT(BILIBIN_INTERIORS_HOUSE_NORTH_TO_BILIBIN, Bilibin,
               BILIBIN_FROM_HOUSE_NORTH),
    SCENE_EXIT(BILIBIN_INTERIORS_WEAPON_AND_ARMOR_SHOP_TO_BILIBIN, Bilibin,
               BILIBIN_FROM_WEAPON_AND_ARMOR_SHOP),
    SCENE_EXIT(BILIBIN_INTERIORS_ITEM_SHOP_TO_BILIBIN, Bilibin,
               BILIBIN_FROM_ITEM_SHOP),
    SCENE_EXIT(BILIBIN_INTERIORS_INN_TO_BILIBIN, Bilibin, BILIBIN_FROM_INN),
    SCENE_EXIT(BILIBIN_INTERIORS_INN_UPSTAIRS_TO_INN, BilibinInteriors,
               BILIBIN_INTERIORS_INN_FROM_INN_UPSTAIRS),
    SCENE_EXIT(BILIBIN_INTERIORS_INN_TO_INN_UPSTAIRS, BilibinInteriors,
               BILIBIN_INTERIORS_INN_UPSTAIRS_FROM_INN),
    SCENE_EXITS_END,
};

// clang-format off
struct ScenePlacement gBilibinInteriorsPlacements[] = {
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, 0x0, ACTOR_STAND,
        PIXELS(104), 0, PIXELS(168), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, 0x0, ACTOR_WANDER,
        PIXELS(200), 0, PIXELS(136), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, 0x0, ACTOR_STAND,
        PIXELS(384), 0, PIXELS(192), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_BOY_WITH_FAIR_HAIR, 0x0, ACTOR_WANDER,
        PIXELS(360), 0, PIXELS(136), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, 0x0, ACTOR_STAND,
        PIXELS(640), 0, PIXELS(156), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_GIRL_WITH_PIGTAILS, 0x0, ACTOR_WANDER,
        PIXELS(704), 0, PIXELS(200), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, 0x0, ACTOR_STAND,
        PIXELS(456), 0, PIXELS(472), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_OLD_WOMAN, 0x0, ACTOR_STAND,
        PIXELS(488), 0, PIXELS(424), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_BALD_MAN_IN_RED, 0x0, ACTOR_STAND,
        PIXELS(200), 0, PIXELS(440), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_IN_GREEN, 0x0, ACTOR_STAND,
        PIXELS(168), 0, PIXELS(440), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_WITH_PONYTAIL, 0x0, ACTOR_STAND,
        PIXELS(584), 0, PIXELS(456), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_MAN_WITH_DARK_HAIR, 0x0, ACTOR_STAND,
        PIXELS(328), 0, PIXELS(744), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_IN_PINK_APRON, 0x0, ACTOR_STAND,
        PIXELS(280), 0, PIXELS(760), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_CHEF, 0x0, ACTOR_STAND,
        PIXELS(88), 0, PIXELS(760), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { SPRITE_CHEF, 0x0, ACTOR_STAND,
        PIXELS(56), 0, PIXELS(760), FACING_EAST, TALK_KEEP_FACING, 0 },
    { SPRITE_YOUNG_WOMAN_IN_GREEN, 0x0, ACTOR_STAND,
        PIXELS(72), 0, PIXELS(776), FACING_NORTHEAST, TALK_KEEP_FACING, 0 },
    { SPRITE_MAN_CARRYING_A_PACK, 0x0, ACTOR_STAND,
        PIXELS(176), 0, PIXELS(744), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_MAN_CARRYING_A_PACK, 0x0, ACTOR_STAND,
        PIXELS(152), 0, PIXELS(768), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_STRONGMAN, 0x0, ACTOR_STAND,
        PIXELS(152), 0, PIXELS(832), FACING_NORTHWEST, TALK_KEEP_FACING, 0 },
    { SPRITE_MAN_WITH_PICKAXE, 0x0, ACTOR_STAND,
        PIXELS(120), 0, PIXELS(796), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, 0x0, ACTOR_STAND,
        PIXELS(88), 0, PIXELS(840), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_MAN_IN_RED_BANDANA, 0x0, ACTOR_STAND,
        PIXELS(808), 0, PIXELS(774), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_MAN_WITH_LUTE, 0x0, ACTOR_STAND,
        PIXELS(568), 0, PIXELS(768), FACING_SOUTHEAST, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gBilibinInteriorsEvents[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, BILIBIN_INTERIORS_HOUSE_EAST_TO_BILIBIN },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, BILIBIN_INTERIORS_HOUSE_WEST_TO_BILIBIN },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, BILIBIN_INTERIORS_HOUSE_NORTH_TO_BILIBIN },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, BILIBIN_INTERIORS_WEAPON_AND_ARMOR_SHOP_TO_BILIBIN },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, BILIBIN_INTERIORS_ITEM_SHOP_TO_BILIBIN },
    { EVENT_EXIT, 6, CONDITION_ALWAYS, BILIBIN_INTERIORS_INN_TO_BILIBIN },
    { EVENT_EXIT, 7, CONDITION_ALWAYS, BILIBIN_INTERIORS_INN_UPSTAIRS_TO_INN },
    { EVENT_EXIT, 8, CONDITION_ALWAYS, BILIBIN_INTERIORS_INN_TO_INN_UPSTAIRS },
    { EVENT_TALK, BILIBIN_YOUNG_MAN_ON_THE_CURSED_MAN, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinThatTreeAtTheEntranceIs) },
    { EVENT_TALK, BILIBIN_VILLAGER_PALACE, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinMccoyHadTheTreesCutDown) },
    { EVENT_TALK, BILIBIN_MIA_ADMIRER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinTheSnowIsAlreadyPilingUp) },
    { EVENT_TALK, BILIBIN_BOY_SNOW, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinWeRarelySeeAnySnowAround) },
    { EVENT_TALK, BILIBIN_VILLAGER_WORRIED_FOR_KOLIMA, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinHowArePeopleDoingInKolima) },
    { EVENT_TALK, BILIBIN_GIRL_SILK, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinIKnowThatGuyWhoTurned) },
    { EVENT_TALK, BILIBIN_OLD_MAN_WATER_OF_HERMES, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinLongAgoTheLighthouseNearImil) },
    { EVENT_TALK, BILIBIN_OLD_WOMAN_LOST_YOUTH, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinButEvenTheLegendaryWaterOf) },
    { EVENT_TALK, BILIBIN_WEAPON_MERCHANT, FLAG_KOLIMA_CURSE_LIFTED, EVENT_SCRIPT(Event_TalkToWeaponMerchant) },
    { EVENT_TALK, BILIBIN_ARMOR_MERCHANT, FLAG_KOLIMA_CURSE_LIFTED, EVENT_SCRIPT(Event_TalkToArmorMerchant) },
    { EVENT_TALK, BILIBIN_ITEM_MERCHANT, FLAG_KOLIMA_CURSE_LIFTED, EVENT_SCRIPT(Event_TalkToItemMerchant) },
    { EVENT_TALK, BILIBIN_INNKEEPER, FLAG_KOLIMA_CURSE_LIFTED, EVENT_SCRIPT(Event_TalkToInnkeeper) },
    { EVENT_TALK, BILIBIN_INN_WORKER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinYouCantReachKolimaNoMatter) },
    { EVENT_TALK, BILIBIN_CHEF, FLAG_KOLIMA_CURSE_LIFTED, EVENT_SCRIPT(Event_TalkToWorriedChef) },
    { EVENT_TALK, BILIBIN_KITCHEN_WORKER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinHeyMisterHeadChefAreYou) },
    { EVENT_TALK, BILIBIN_WAITRESS, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinStartCookingAlready) },
    { EVENT_TALK, BILIBIN_HUNGRY_DINER, FLAG_KOLIMA_CURSE_LIFTED, EVENT_SCRIPT(Event_TalkToHungryDiner) },
    { EVENT_TALK, BILIBIN_FURNITURE_SELLER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinWeCameHereBecauseLordMccoy) },
    { EVENT_TALK, BILIBIN_DINER_BIG_TREE, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinIThinkThatBigTreeDeep) },
    { EVENT_TALK, BILIBIN_DINER, FLAG_KOLIMA_CURSE_LIFTED, EVENT_SCRIPT(Event_AskAboutTreeSpirit) },
    { EVENT_TALK, BILIBIN_DINER_TODAYS_SPECIAL, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinICantWaitToSeeTodays) },
    { EVENT_TALK, BILIBIN_JOB_SEEKER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinIWentToLordMccoyLooking) },
    { EVENT_TALK, BILIBIN_VILLAGER_ANCIENT_TREES, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinIveHeardSongsThatTellOf) },
    { EVENT_TALK, BILIBIN_YOUNG_MAN_ON_THE_CURSED_MAN, CONDITION_ALWAYS, EVENT_SCRIPT(Event_AskAboutHolyTree) },
    { EVENT_TALK, BILIBIN_VILLAGER_PALACE, CONDITION_ALWAYS, MSGNO(MsgBilibinThankGoodnessTheyStoppedConstructionOf) },
    { EVENT_TALK, BILIBIN_MIA_ADMIRER, CONDITION_ALWAYS, MSGNO(MsgBilibinIAlwaysMissAltinInThe) },
    { EVENT_TALK, BILIBIN_BOY_SNOW, CONDITION_ALWAYS, MSGNO(MsgBilibinMyFatherSaysThatItsWarm) },
    { EVENT_TALK, BILIBIN_VILLAGER_WORRIED_FOR_KOLIMA, CONDITION_ALWAYS, MSGNO(MsgBilibinTheRoadToKolimaIsOpen) },
    { EVENT_TALK, BILIBIN_GIRL_SILK, CONDITION_ALWAYS, EVENT_SCRIPT(Event_AskAboutSilk) },
    { EVENT_TALK, BILIBIN_OLD_MAN_WATER_OF_HERMES, CONDITION_ALWAYS, MSGNO(MsgBilibinIThoughtISawLightsIn) },
    { EVENT_TALK, BILIBIN_OLD_WOMAN_LOST_YOUTH, CONDITION_ALWAYS, MSGNO(MsgBilibinHisEyesAreRedBecauseHe) },
    { EVENT_TALK, BILIBIN_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToWeaponMerchant) },
    { EVENT_TALK, BILIBIN_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToArmorMerchant) },
    { EVENT_TALK, BILIBIN_ITEM_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToItemMerchant) },
    { EVENT_TALK, BILIBIN_INNKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToInnkeeper) },
    { EVENT_TALK, BILIBIN_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgBilibinThereWillBeFewerGuestsNow) },
    { EVENT_TALK, BILIBIN_CHEF, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToBusyChef) },
    { EVENT_TALK, BILIBIN_KITCHEN_WORKER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToKitchenWorker) },
    { EVENT_TALK, BILIBIN_WAITRESS, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToWaitress) },
    { EVENT_TALK, BILIBIN_HUNGRY_DINER, CONDITION_ALWAYS, MSGNO(MsgBilibinImALittleSurprisedThatThe) },
    { EVENT_TALK, BILIBIN_FURNITURE_SELLER, CONDITION_ALWAYS, MSGNO(MsgBilibinTheresNoBusinessHereSoIm) },
    { EVENT_TALK, BILIBIN_DINER_BIG_TREE, CONDITION_ALWAYS, MSGNO(MsgBilibinTheyServedSoMuchFoodToday) },
    { EVENT_TALK, BILIBIN_DINER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkAboutMeal) },
    { EVENT_TALK, BILIBIN_DINER_TODAYS_SPECIAL, CONDITION_ALWAYS, MSGNO(MsgBilibinOooohThatsItITriedMy) },
    { EVENT_TALK, BILIBIN_JOB_SEEKER, CONDITION_ALWAYS, MSGNO(MsgBilibinTheIncidentWasResolvedBeforeI) },
    { EVENT_TALK, BILIBIN_VILLAGER_ANCIENT_TREES, CONDITION_ALWAYS, MSGNO(MsgBilibinIWonderWhatThatEerieHowling) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_YOUNG_MAN_ON_THE_CURSED_MAN, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinThisIsAllLordMccoysFault) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_VILLAGER_PALACE, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinAhLadyMccoyIWouldJust) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_MIA_ADMIRER, FLAG_KOLIMA_CURSE_LIFTED, EVENT_SCRIPT(Event_ReadMiaAdmirersMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_BOY_SNOW, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinItMustBeDifficultToLive) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_VILLAGER_WORRIED_FOR_KOLIMA, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinICantImagineThatEveryoneIn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_GIRL_SILK, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinHeSeemedSoHappyAndThen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_OLD_MAN_WATER_OF_HERMES, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinCouldTheWaterOfHermesSave) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_OLD_WOMAN_LOST_YOUTH, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinTheWaterOfHermesVanishedLong) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_WEAPON_MERCHANT, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinTheWorldWillNeverSeeSuch) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_ARMOR_MERCHANT, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinIWishICouldSeeAnother) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_ITEM_MERCHANT, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinOnlySomeoneReallyBraveWouldGo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_INNKEEPER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinIDontBelieveInCursesFrom) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_INN_WORKER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinAnyTravelersHeadingEastAreStuck) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_CHEF, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinFatherMotherIHopeTheyreAll) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_KITCHEN_WORKER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinShouldIJustForgetThatIm) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_WAITRESS, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinIveHadAllTheCustomerComplaints) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_HUNGRY_DINER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinArghhhThatsItIJustWant) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_FURNITURE_SELLER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinICameToBilibinHopingTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_DINER_BIG_TREE, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinISeeWhyTheyCallThat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_DINER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinIDontCareWhoDoesIt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_DINER_TODAYS_SPECIAL, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinYippeeIsItReadyYetMy) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_JOB_SEEKER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinImGettingALittleScaredHearing) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_VILLAGER_ANCIENT_TREES, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinItHasBeenSaidThatA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_YOUNG_MAN_ON_THE_CURSED_MAN, CONDITION_ALWAYS, MSGNO(MsgBilibinItTakesManyYearsForA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_VILLAGER_PALACE, CONDITION_ALWAYS, MSGNO(MsgBilibinSometimesIWishICouldSee) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_MIA_ADMIRER, CONDITION_ALWAYS, MSGNO(MsgBilibinIWonderIfThereAreStill) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_BOY_SNOW, CONDITION_ALWAYS, MSGNO(MsgBilibinMyFatherHasTraveledAllOver) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_VILLAGER_WORRIED_FOR_KOLIMA, CONDITION_ALWAYS, MSGNO(MsgBilibinTravelingMakesYoungWarriorsStronger) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_GIRL_SILK, CONDITION_ALWAYS, MSGNO(MsgBilibinClothesMadeWithSilkAreSo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_OLD_MAN_WATER_OF_HERMES, CONDITION_ALWAYS, MSGNO(MsgBilibinTheresNoDoubtAboutItIts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_OLD_WOMAN_LOST_YOUTH, CONDITION_ALWAYS, MSGNO(MsgBilibinWhatsTheBigDealWhyAre) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgBilibinAltinsGotGoodWeaponsButI) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgBilibinIWishICouldGoOut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_ITEM_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgBilibinTheyCallThemselvesWarriorsButYou) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgBilibinWellIWasOnlyActingScared) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgBilibinItdBeBetterForBusinessIf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_CHEF, CONDITION_ALWAYS, MSGNO(MsgBilibinIllGoToKolimaWhenThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_KITCHEN_WORKER, CONDITION_ALWAYS, MSGNO(MsgBilibinLetsSeeSoupSaladAndI) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_WAITRESS, CONDITION_ALWAYS, MSGNO(MsgBilibinTheHeadChefWasSoLazy) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_HUNGRY_DINER, CONDITION_ALWAYS, MSGNO(MsgBilibinItUsedToTakeThemSo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_FURNITURE_SELLER, CONDITION_ALWAYS, MSGNO(MsgBilibinWhenTheyStoppedBuildingThePalace) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_DINER_BIG_TREE, CONDITION_ALWAYS, MSGNO(MsgBilibinIWishICouldEatLike) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_DINER, CONDITION_ALWAYS, MSGNO(MsgBilibinIdBetterEatALotThat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_DINER_TODAYS_SPECIAL, CONDITION_ALWAYS, MSGNO(MsgBilibinTheFoodIsSuchAGood) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_JOB_SEEKER, CONDITION_ALWAYS, MSGNO(MsgBilibinIfAKidLikeThisCould) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_VILLAGER_ANCIENT_TREES, CONDITION_ALWAYS, MSGNO(MsgBilibinItLooksLikeScaryThingsAre) },
    { SEARCH(SEARCH_BARREL), 100, 0xf52, FIND_ITEM(ITEM_ANTIDOTE) },
    { SEARCH(SEARCH_BED), 101, 0xf53, FIND_ITEM(ITEM_SLEEP_BOMB) },
    { SEARCH(SEARCH_BARREL), 102, 0xf54, FIND_ITEM(ITEM_HARD_NUT) },
    { SEARCH(SEARCH_OVEN), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBilibinLooksLikeEggSoupSoftEggs)) },
    { SEARCH(SEARCH_OVEN), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBilibinFreshOmelettesBilibinoOmelettesUseWild)) },
    { SEARCH(SEARCH_BOOKCASE), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBilibinTheresABookEntitledAllAbout)) },
    { SCENE_EVENTS_END },
};
// clang-format on
