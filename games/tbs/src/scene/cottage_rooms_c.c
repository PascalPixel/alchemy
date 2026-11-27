// Cottage rooms C (SceneIdCottageRoomsC) is a rooms map drawn as cottages, all of them inside
// Bilibin's buildings, whose area name is Bilibin.
//
// Six of Bilibin's doors (BILIBIN_TO_INN to BILIBIN_TO_WEAPON_AND_ARMOR_SHOP, in bilibin.c) lead
// into its rooms, and each room's door leads back out to the door it was entered by.
//
// One room holds the weapon and armor shops, and another the item shop. A third holds the inn with
// the restaurant beside it, and its stairs (COTTAGE_ROOMS_C_INN_TO_INN_UPSTAIRS and
// COTTAGE_ROOMS_C_INN_UPSTAIRS_TO_INN) join it to its upstairs rooms. Three other buildings hold
// villagers. scGetActorPlacements leaves out the actors of the rooms not in view.
//
// Everyone has a line for while Kolima's curse holds (FLAG_KOLIMA_CURSE_LIFTED clear) and one for
// after.
//
// Before, the townsfolk explain the tree at the village entrance, McCoy's palace and the Water of
// Hermes. In the restaurant the head chef, born in Kolima, is too worried to cook.
//
// After, the head chef cooks so hard his kitchen cannot keep up. The first time the party talks to
// him he shouts a string of orders and the kitchen worker answers (FLAG_BILIBIN_CHEF_CALLED_ORDERS,
// kept until the party leaves Bilibin). From then on he sends the party to the waitress
// (scCottageRoomsCTalkToBusyChef).
//
// Every row that needs FLAG_KOLIMA_CURSE_LIFTED clear comes before the rows for afterwards, so the
// first row that matches picks the time.
//
// The merchants and the innkeeper open the shop or the inn only when the leader faces them across
// the counter (facing north), and otherwise talk of the time. Mind Read on the villager who admires
// Mia depends on whether she is in the party.
//
// Searching finds an Antidote, a Sleep Bomb and a Hard Nut, egg soup and omelettes in the ovens,
// and a book called "All About Items".
//
// scInit opens the rooms with a window around the leader and draws one diner,
// ACTOR_BILIBIN_DINER_WITH_PICKAXE, at a fixed sprite priority.
//
// He stands at the north edge of the restaurant's table, on metatile (7, 49). That metatile alone
// of the diners' metatiles gives an actor standing on it priority 3 (cottage_rooms_c's map.txt).
// Taken from that metatile, his priority would put him behind the middle layer (`BG2`, priority 2
// in this map). The fixed priority 2 draws him in front of it, as the other diners are.

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "debug/debug.h"
#include "field/script.h"
#include "graphics/graphics.h"
#include "text/msg_ids.h"

extern SCENE_ENTRANCE CottageRoomsCEntrances[];
extern Uint32         CottageRoomsCExits[];
extern ACTOR_PLACE    CottageRoomsCActorPlacements[];
extern CUE            CottageRoomsCTriggers[];

// The head chef has shouted his string of orders once, so from then on he sends the party to the
// waitress.
#define FLAG_BILIBIN_CHEF_CALLED_ORDERS FLAG_AREA_TEMP_00

// Bilibin's shops, as mnShopRun numbers the shops.
enum
{
	SHOP_BILIBIN_WEAPONS = 7,
	SHOP_BILIBIN_ARMOR   = 8,
	SHOP_BILIBIN_ITEMS   = 9
};

// Bilibin's inn, as mnInnCheckIn numbers the inns.
enum
{
	BILIBIN_INN = 2
};

// The placed actors, numbered in the order of CottageRoomsCActorPlacements from ACTOR_FIRST_PLACED.
//
// Each room's actors are together: two in each house, then the shops' customers and keepers, the
// inn and its restaurant, and the inn's upstairs.
typedef enum COTTAGE_ROOMS_C_ACTOR_ID
{
	// A young man with brown hair, in the house east.
	ACTOR_BILIBIN_YOUNG_MAN_ON_THE_CURSED_MAN = ACTOR_FIRST_PLACED,
	// A woman with fair hair. "McCoy had the trees cut down so he could build" his wife a palace.
	ACTOR_BILIBIN_WOMAN_ON_MCCOYS_PALACE,
	// A burly grey-haired man in the house west, who wants to see Mia again.
	ACTOR_BILIBIN_MIA_ADMIRER,
	// A small boy with fair hair. "We rarely see any snow around Bilibin."
	ACTOR_BILIBIN_BOY_SNOW,
	// A woman in a green dress, in the house north.
	ACTOR_BILIBIN_VILLAGER_WORRIED_FOR_KOLIMA,
	// A little girl with pigtails, who later asks whether the party knows what silk is.
	ACTOR_BILIBIN_GIRL_SILK,
	// An old man with a cane, in the item shop.
	ACTOR_BILIBIN_OLD_MAN_WATER_OF_HERMES,
	// An old woman. "But even the legendary Water of Hermes cannot turn back time."
	ACTOR_BILIBIN_OLD_WOMAN_LOST_YOUTH,
	ACTOR_BILIBIN_WEAPON_MERCHANT,
	ACTOR_BILIBIN_ARMOR_MERCHANT,
	ACTOR_BILIBIN_ITEM_MERCHANT,
	ACTOR_BILIBIN_INNKEEPER,
	// Beside the innkeeper.
	ACTOR_BILIBIN_INN_WORKER,
	// The restaurant's head chef, born in Kolima.
	ACTOR_BILIBIN_CHEF,
	// A second cook in a tall chef's hat.
	ACTOR_BILIBIN_KITCHEN_WORKER,
	// A young woman with long brown hair.
	ACTOR_BILIBIN_WAITRESS,
	// A man with a pack, waiting for his dinner.
	ACTOR_BILIBIN_HUNGRY_DINER,
	// A diner with a pack, who "came to Bilibin hoping to sell furniture".
	ACTOR_BILIBIN_FURNITURE_SELLER,
	// A strongman at the table. "I think that big tree deep in the forest looks suspicious!"
	ACTOR_BILIBIN_DINER_BIG_TREE,
	// A diner with a pickaxe, drawn at a fixed sprite priority.
	ACTOR_BILIBIN_DINER_WITH_PICKAXE,
	// An old man at the table.
	ACTOR_BILIBIN_DINER_TODAYS_SPECIAL,
	// A man in a red bandana, upstairs at the inn. "I went to Lord McCoy looking for work..."
	ACTOR_BILIBIN_JOB_SEEKER,
	// A man with a lute, upstairs at the inn. "I've heard songs that tell of ancient trees
	// developing strange powers."
	ACTOR_BILIBIN_VILLAGER_ANCIENT_TREES
} COTTAGE_ROOMS_C_ACTOR_ID;

const SCENE_ENTRANCE *scGetEntrances(void)
{
	return CottageRoomsCEntrances;
}

const SCENE_EXIT_REGION *scGetExitRegions(void)
{
	return 0;
}

const Uint32 *scGetExits(void)
{
	return CottageRoomsCExits;
}

// Places Bilibin's townsfolk, leaving out those of the rooms not in view.
const ACTOR_PLACE *scGetActorPlacements(void)
{
	fdAssignViewFlags(CottageRoomsCActorPlacements);
	return CottageRoomsCActorPlacements;
}

// Bilibin's weapon merchant opens the shop across the counter, or talks.
static void scCottageRoomsCTalkToWeaponMerchant(void)
{
	Uint32 leaderFacing;

	leaderFacing = fdActorGet(ACTOR_PARTY_LEADER)->facing;
	fdScriptStart();

	if (FACING_IS_NORTH(leaderFacing))
	{
		mnShopRun(SHOP_BILIBIN_WEAPONS, ACTOR_BILIBIN_WEAPON_MERCHANT);
	}
	else
	{
		if (gmFlagGet(FLAG_KOLIMA_CURSE_LIFTED) == 0)
		{
			fdScriptSetMessageId(MsgBilibinThereWasAbleHealerIn);
		}
		else
		{
			fdScriptSetMessageId(MsgBilibinOurWeaponsBestCanFind);
		}
		fdScriptShowMessage(ACTOR_BILIBIN_WEAPON_MERCHANT, 0);
	}

	fdScriptEnd();
}

// Bilibin's item merchant opens the shop across the counter, or talks.
static void scCottageRoomsCTalkToItemMerchant(void)
{
	Uint32 leaderFacing;

	leaderFacing = fdActorGet(ACTOR_PARTY_LEADER)->facing;
	fdScriptStart();

	if (FACING_IS_NORTH(leaderFacing))
	{
		mnShopRun(SHOP_BILIBIN_ITEMS, ACTOR_BILIBIN_ITEM_MERCHANT);
	}
	else
	{
		if (gmFlagGet(FLAG_KOLIMA_CURSE_LIFTED) == 0)
		{
			fdScriptSetMessageId(MsgBilibinMustUsedTypesDangerBeing);
		}
		else
		{
			fdScriptSetMessageId(MsgBilibinTurnedOutWarriorsHiredBy);
		}
		fdScriptShowMessage(ACTOR_BILIBIN_ITEM_MERCHANT, 0);
	}

	fdScriptEnd();
}

// Returns the cues, whose rows for while the curse holds come first.
const CUE *scGetCues(void)
{
	return CottageRoomsCTriggers;
}

// Bilibin's armor merchant opens the shop across the counter, or talks.
static void scCottageRoomsCTalkToArmorMerchant(void)
{
	Uint32 leaderFacing;

	leaderFacing = fdActorGet(ACTOR_PARTY_LEADER)->facing;
	fdScriptStart();
	if (FACING_IS_NORTH(leaderFacing))
	{
		mnShopRun(SHOP_BILIBIN_ARMOR, ACTOR_BILIBIN_ARMOR_MERCHANT);
	}
	else
	{
		if (gmFlagGet(FLAG_KOLIMA_CURSE_LIFTED) == 0)
		{
			fdScriptSetMessageId(MsgBilibinHaveEverHeardOcean);
			fdActorFaceActor(ACTOR_BILIBIN_ARMOR_MERCHANT, ACTOR_PARTY_LEADER, 0);
			fdScriptWait(10);
			// "Have you ever heard of the 'ocean'?" Yes: "If you travel over the ocean to far, far
			// away, you'll find..." No: "The ocean is immense... You would want to sing if you
			// ever..."
			fdScriptAskYesNo(ACTOR_BILIBIN_ARMOR_MERCHANT, 0);
			fdActorFaceDirection(ACTOR_BILIBIN_ARMOR_MERCHANT, FACING_SOUTH_SOUTHEAST, 10);
		}
		else
		{
			fdScriptSetMessageId(MsgBilibinForSomeReasonOceanFills);
			// "For some reason, the ocean fills me with hope and desire......"
			fdScriptShowMessage(ACTOR_BILIBIN_ARMOR_MERCHANT, 0);
		}
	}
	fdScriptEnd();
}

// The head chef in Bilibin's restaurant, before the curse is lifted, is too worried to focus on his
// cooking.
static void scCottageRoomsCTalkToWorriedChef(void)
{
	fdScriptStart();
	fdScriptSetMessageId(MsgBilibinImReallyWorriedAboutKolima);
	fdActorFaceActor(ACTOR_BILIBIN_CHEF, ACTOR_PARTY_LEADER, 0);
	// "I'm really worried about Kolima. I was born there, you know?..."
	fdScriptShowMessage(ACTOR_BILIBIN_CHEF, 0);
	fdActorFaceDirection(ACTOR_BILIBIN_CHEF, FACING_NORTH, 10);
	fdScriptEnd();
}

// A diner in Bilibin's restaurant, before the curse is lifted, is still waiting for his dinner.
static void scCottageRoomsCTalkToHungryDiner(void)
{
	fdScriptStart();
	fdScriptSetMessageId(MsgBilibinGrrr);
	// "Grrr..."
	fdScriptShowMessageThenWait(ACTOR_BILIBIN_HUNGRY_DINER, 0, 20);
	fdActorFaceActor(ACTOR_BILIBIN_HUNGRY_DINER, ACTOR_PARTY_LEADER, 0);
	fdScriptWait(10);
	// "Did you hear a sound? You heard it, right?"
	fdScriptOpenMessage(ACTOR_BILIBIN_HUNGRY_DINER, 0);
	if (fdScriptAskYesNoWithActorAnimation(0, 0) != 0)
	{
		FieldStateWork->msg++;
	}
	fdScriptShowMessage(ACTOR_BILIBIN_HUNGRY_DINER, 0);
	fdActorFaceDirection(ACTOR_BILIBIN_HUNGRY_DINER, FACING_SOUTH, 10);
	fdScriptEnd();
}

// A diner asks whether a tree spirit can curse a man.
static void scCottageRoomsCAskAboutTreeSpirit(void)
{
	fdScriptStart();
	fdScriptSetMessageId(MsgBilibinDoBelieveTreeSpiritCan);
	fdActorFaceActor(ACTOR_BILIBIN_DINER_WITH_PICKAXE, ACTOR_PARTY_LEADER, 0);
	fdScriptWait(10);
	// "Do you believe a tree spirit can curse a man?"
	fdScriptOpenMessage(ACTOR_BILIBIN_DINER_WITH_PICKAXE, 0);
	if (fdScriptAskYesNoWithActorAnimation(0, 0) != 0)
	{
		FieldStateWork->msg++;
	}
	fdScriptShowMessage(ACTOR_BILIBIN_DINER_WITH_PICKAXE, 0);
	fdActorFaceDirection(ACTOR_BILIBIN_DINER_WITH_PICKAXE, FACING_SOUTH, 10);
	fdScriptEnd();
}

// A young man asks whether Kolima Forest's holy tree survived.
static void scCottageRoomsCAskAboutHolyTree(void)
{
	fdScriptStart();
	fdScriptSetMessageId(MsgBilibinDoKnowIfHolyTree);
	// "Do you know if the holy tree survived?" Yes: "That makes sense. Those who were turned into
	// trees are back..." No: "Then how did the people who became trees get returned to..."
	fdScriptAskYesNo(ACTOR_BILIBIN_YOUNG_MAN_ON_THE_CURSED_MAN, 0);
	fdScriptEnd();
}

// A little girl in Bilibin, once the curse is lifted, asks whether the party knows what silk is.
static void scCottageRoomsCAskAboutSilk(void)
{
	fdScriptStart();
	fdScriptSetMessageId(MsgBilibinDoKnowSilk);
	// "Do you know what silk is?" Yes: "That's what they make in Xian village on the other side
	// of..." No: "Caterpillars are raised on mulberry leaves. When the..."
	fdScriptAskYesNo(ACTOR_BILIBIN_GIRL_SILK, 0);
	fdScriptEnd();
}

// Bilibin's innkeeper opens the inn across the counter, or talks.
static void scCottageRoomsCTalkToInnkeeper(void)
{
	Uint32 leaderFacing;

	leaderFacing = fdActorGet(ACTOR_PARTY_LEADER)->facing;
	fdScriptStart();

	if (FACING_IS_NORTH(leaderFacing))
	{
		mnInnCheckIn(BILIBIN_INN, ACTOR_BILIBIN_INNKEEPER);
	}
	else if (gmFlagGet(FLAG_KOLIMA_CURSE_LIFTED) != 0)
	{
		fdScriptSetMessageId(MsgBilibinWasntCurseInKolimaHorrifying);
		// "Wasn't that curse in Kolima horrifying?" Yes: "Even heroes like you can feel fear. Just
		// imagine how scary..." No: "You can say that now, because it's all over. At the time,
		// we..."
		fdScriptAskYesNo(ACTOR_BILIBIN_INNKEEPER, 0);
	}
	else
	{
		fdScriptSetMessageId(MsgBilibinCurseOnKolimaScaryDevelopment);
		// "A curse on Kolima? What a scary development!"
		fdScriptShowMessage(ACTOR_BILIBIN_INNKEEPER, 0);
	}

	fdScriptEnd();
}

// The head chef in Bilibin's restaurant, once the curse is lifted, calls out orders once, then
// sends the party to the waitress.
static void scCottageRoomsCTalkToBusyChef(void)
{
	fdScriptStart();
	if (fdScriptGetFlag(FLAG_BILIBIN_CHEF_CALLED_ORDERS) == 0)
	{
		// The first time (FLAG_BILIBIN_CHEF_CALLED_ORDERS, kept until the party leaves Bilibin),
		// the chef shouts orders and the kitchen worker answers.
		fdScriptSetMessageId(MsgBilibinTwoSpecialsOneDinnerOne);
		// "Two specials! One dinner! One sandwich! One dessert! Hurry up!"
		fdScriptShowMessage(ACTOR_BILIBIN_CHEF, 0);
		fdActorFaceDirection(ACTOR_BILIBIN_CHEF, FACING_WEST, 20);
		// "Hey, you! Are you done yet?"
		fdScriptShowMessage(ACTOR_BILIBIN_CHEF, 0);
		fdActorStartRepeatedMotion(ACTOR_BILIBIN_KITCHEN_WORKER, 2);
		fdActorSetAttachedEffect(ACTOR_BILIBIN_KITCHEN_WORKER,
		                         ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
		fdScriptWait(60);
		// "Almost..."
		fdScriptShowMessage(ACTOR_BILIBIN_KITCHEN_WORKER, 0);
		fdScriptWait(10);
		gmFlagSet(FLAG_BILIBIN_CHEF_CALLED_ORDERS);
	}
	// Then, as every later time, he sends the party to the waitress.
	fdActorFaceActor(ACTOR_BILIBIN_CHEF, ACTOR_PARTY_LEADER, 0);
	fdScriptSetMessageId(MsgBilibinIfWantMealSpeakWaitress);
	// "If you want a meal, speak to the waitress."
	fdScriptShowMessage(ACTOR_BILIBIN_CHEF, 0);
	fdActorFaceDirection(ACTOR_BILIBIN_CHEF, FACING_NORTH, 10);
	fdScriptEnd();
}

// The kitchen worker can't keep up with the head chef's energy.
static void scCottageRoomsCTalkToKitchenWorker(void)
{
	fdScriptStart();
	fdScriptSetMessageId(MsgBilibinReallyThinkHeadChefHas);
	// "I really think the head chef has changed. It's too much!"
	fdScriptShowMessage(ACTOR_BILIBIN_KITCHEN_WORKER, 0);
	fdActorFaceActor(ACTOR_BILIBIN_KITCHEN_WORKER, ACTOR_PARTY_LEADER, 0);
	// "I can't keep up with his energy!"
	fdScriptShowMessage(ACTOR_BILIBIN_KITCHEN_WORKER, 0);
	fdActorFaceDirection(ACTOR_BILIBIN_KITCHEN_WORKER, FACING_EAST, 10);
	fdScriptEnd();
}

// The waitress hurries through her work before the head chef yells.
static void scCottageRoomsCTalkToWaitress(void)
{
	fdScriptStart();
	fdScriptSetMessageId(MsgBilibinLetsSeeServeThemWater);
	// "Let's see, serve them water...then get the silverware."
	fdScriptShowMessage(ACTOR_BILIBIN_WAITRESS, 0);
	fdActorFaceActor(ACTOR_BILIBIN_WAITRESS, ACTOR_PARTY_LEADER, 0);
	// "The head chef will yell if you slow down..."
	fdScriptShowMessage(ACTOR_BILIBIN_WAITRESS, 0);
	fdActorFaceDirection(ACTOR_BILIBIN_WAITRESS, FACING_NORTH, 10);
	fdScriptEnd();
}

// A diner loves the meal and asks whether the party does too.
static void scCottageRoomsCTalkAboutMeal(void)
{
	fdScriptStart();
	fdScriptSetMessageId(MsgBilibinWasSomeMealDontJust);
	// "That was some meal! Don't you just love the food here?" Yes: "I know how you feel. Go find a
	// table and pig out!" No: "What!? Why am I even talking to you, when I could be eating!?"
	fdScriptAskYesNo(ACTOR_BILIBIN_DINER_WITH_PICKAXE, 0);
	fdScriptEnd();
}

// Mind Read on the burly man in Bilibin who admires Mia, before the curse is lifted, hears his
// thoughts of her.
static void scCottageRoomsCReadMiaAdmirersMind(void)
{
	fdScriptStart();
	if (gmFlagGet(FLAG_IN_PARTY(MIA)) != 0)
	{
		fdScriptSetMessageId(MsgBilibinEvenFrozenImilMustFeel);
	}
	else
	{
		fdScriptSetMessageId(MsgBilibinWhenSpringComesWantGo);
	}
	fdScriptShowMessage(ACTOR_BILIBIN_MIA_ADMIRER, 0);
	fdScriptEnd();
}

// Sets up Bilibin's buildings.
Sint32 scInit(void)
{
	ACTOR *actor;

	FieldStateWork->transition    = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	actor                         = fdActorGet(ACTOR_BILIBIN_DINER_WITH_PICKAXE);
	actor->priorityFlags          = 0;
	actor->sprite->image.priority = 2;
	return 0;
}

SCENE_ENTRANCE CottageRoomsCEntrances[] = {
	{ENTRANCE_DEFAULT, CONDITION_ALWAYS, 176, 0, 216, FACING_NORTH, 0, -1, -1, -1, -1, 0},
	{COTTAGE_ROOMS_C_HOUSE_EAST,
     CONDITION_ALWAYS, 176,
     0, 216,
     FACING_NORTH, 0,
     24, 32,
     264, 240,
     0},
	{COTTAGE_ROOMS_C_HOUSE_WEST,
     CONDITION_ALWAYS, 400,
     0, 264,
     FACING_NORTH, 0,
     256, 0,
     496, 288,
     0},
	{COTTAGE_ROOMS_C_HOUSE_NORTH,
     CONDITION_ALWAYS, 688,
     0, 264,
     FACING_NORTH, 0,
     544, 0,
     784, 288,
     0},
	{COTTAGE_ROOMS_C_WEAPON_AND_ARMOR_SHOP,
     CONDITION_ALWAYS, 240,
     0, 504,
     FACING_NORTH, 0,
     72, 336,
     312, 528,
     0},
	{COTTAGE_ROOMS_C_ITEM_SHOP,
     CONDITION_ALWAYS, 576,
     0, 536,
     FACING_NORTH, 0,
     400, 304,
     640, 552,
     0},
	{COTTAGE_ROOMS_C_INN, CONDITION_ALWAYS, 352, 0, 872, FACING_NORTH, 0, 0, 640, 400, 896, 0},
	{COTTAGE_ROOMS_C_INN_UPSTAIRS_FROM_INN,
     CONDITION_ALWAYS, 656,
     0, 696,
     FACING_WEST, 0,
     432, 640,
     848, 896,
     0},
	{COTTAGE_ROOMS_C_INN_FROM_INN_UPSTAIRS,
     CONDITION_ALWAYS, 344,
     0, 696,
     FACING_EAST, 0,
     0, 640,
     400, 896,
     0},
	{SCENE_TABLE_END},
};

Uint32 CottageRoomsCExits[] = {
	SCENE_EXITS(CottageRoomsC),
	SCENE_EXIT(COTTAGE_ROOMS_C_HOUSE_EAST_TO_BILIBIN, Bilibin, BILIBIN_FROM_HOUSE_EAST),
	SCENE_EXIT(COTTAGE_ROOMS_C_HOUSE_WEST_TO_BILIBIN, Bilibin, BILIBIN_FROM_HOUSE_WEST),
	SCENE_EXIT(COTTAGE_ROOMS_C_HOUSE_NORTH_TO_BILIBIN, Bilibin, BILIBIN_FROM_HOUSE_NORTH),
	SCENE_EXIT(COTTAGE_ROOMS_C_WEAPON_AND_ARMOR_SHOP_TO_BILIBIN,
               Bilibin,
               BILIBIN_FROM_WEAPON_AND_ARMOR_SHOP),
	SCENE_EXIT(COTTAGE_ROOMS_C_ITEM_SHOP_TO_BILIBIN, Bilibin, BILIBIN_FROM_ITEM_SHOP),
	SCENE_EXIT(COTTAGE_ROOMS_C_INN_TO_BILIBIN, Bilibin, BILIBIN_FROM_INN),
	SCENE_EXIT(
		COTTAGE_ROOMS_C_INN_UPSTAIRS_TO_INN, CottageRoomsC, COTTAGE_ROOMS_C_INN_FROM_INN_UPSTAIRS),
	SCENE_EXIT(
		COTTAGE_ROOMS_C_INN_TO_INN_UPSTAIRS, CottageRoomsC, COTTAGE_ROOMS_C_INN_UPSTAIRS_FROM_INN),
	SCENE_EXITS_END,
};

// clang-format off
ACTOR_PLACE CottageRoomsCActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		104, 0, 168, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_WANDER,
		200, 0, 136, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		384, 0, 192, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_WANDER,
		360, 0, 136, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		640, 0, 156, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_IN_VIEW, MOVEMENT_TYPE_WANDER,
		704, 0, 200, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		456, 0, 472, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		488, 0, 424, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_BALD_MAN_IN_RED, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		200, 0, 440, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_IN_GREEN, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		168, 0, 440, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_PONYTAIL, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		584, 0, 456, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_WITH_DARK_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		328, 0, 744, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PINK_APRON, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		280, 0, 760, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_CHEF, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		88, 0, 760, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_CHEF, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		56, 0, 760, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_YOUNG_WOMAN_IN_GREEN, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		72, 0, 776, FACING_NORTHEAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_CARRYING_A_PACK, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		176, 0, 744, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_CARRYING_A_PACK, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		152, 0, 768, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		152, 0, 832, FACING_NORTHWEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_WITH_PICKAXE, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		120, 0, 796, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		88, 0, 840, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_IN_RED_BANDANA, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		808, 0, 774, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_WITH_LUTE, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		568, 0, 768, FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
	{ SCENE_TABLE_END },
};

CUE CottageRoomsCTriggers[] = {
	{ CUE_EXIT, 1, CONDITION_ALWAYS, COTTAGE_ROOMS_C_HOUSE_EAST_TO_BILIBIN },
	{ CUE_EXIT, 2, CONDITION_ALWAYS, COTTAGE_ROOMS_C_HOUSE_WEST_TO_BILIBIN },
	{ CUE_EXIT, 3, CONDITION_ALWAYS, COTTAGE_ROOMS_C_HOUSE_NORTH_TO_BILIBIN },
	{ CUE_EXIT, 4, CONDITION_ALWAYS, COTTAGE_ROOMS_C_WEAPON_AND_ARMOR_SHOP_TO_BILIBIN },
	{ CUE_EXIT, 5, CONDITION_ALWAYS, COTTAGE_ROOMS_C_ITEM_SHOP_TO_BILIBIN },
	{ CUE_EXIT, 6, CONDITION_ALWAYS, COTTAGE_ROOMS_C_INN_TO_BILIBIN },
	{ CUE_EXIT, 7, CONDITION_ALWAYS, COTTAGE_ROOMS_C_INN_UPSTAIRS_TO_INN },
	{ CUE_EXIT, 8, CONDITION_ALWAYS, COTTAGE_ROOMS_C_INN_TO_INN_UPSTAIRS },
	{ CUE_TALK, ACTOR_BILIBIN_YOUNG_MAN_ON_THE_CURSED_MAN, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinThatTreeAtTheEntranceIs) },
	{ CUE_TALK, ACTOR_BILIBIN_WOMAN_ON_MCCOYS_PALACE, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinMccoyHadTheTreesCutDown) },
	{ CUE_TALK, ACTOR_BILIBIN_MIA_ADMIRER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinTheSnowIsAlreadyPilingUp) },
	{ CUE_TALK, ACTOR_BILIBIN_BOY_SNOW, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinWeRarelySeeAnySnowAround) },
	{ CUE_TALK, ACTOR_BILIBIN_VILLAGER_WORRIED_FOR_KOLIMA, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinHowArePeopleDoingInKolima) },
	{ CUE_TALK, ACTOR_BILIBIN_GIRL_SILK, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinIKnowThatGuyWhoTurned) },
	{ CUE_TALK, ACTOR_BILIBIN_OLD_MAN_WATER_OF_HERMES, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinLongAgoTheLighthouseNearImil) },
	{ CUE_TALK, ACTOR_BILIBIN_OLD_WOMAN_LOST_YOUTH, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinButEvenTheLegendaryWaterOf) },
	{ CUE_TALK, ACTOR_BILIBIN_WEAPON_MERCHANT, FLAG_KOLIMA_CURSE_LIFTED, SCRIPT_REF(scCottageRoomsCTalkToWeaponMerchant) },
	{ CUE_TALK, ACTOR_BILIBIN_ARMOR_MERCHANT, FLAG_KOLIMA_CURSE_LIFTED, SCRIPT_REF(scCottageRoomsCTalkToArmorMerchant) },
	{ CUE_TALK, ACTOR_BILIBIN_ITEM_MERCHANT, FLAG_KOLIMA_CURSE_LIFTED, SCRIPT_REF(scCottageRoomsCTalkToItemMerchant) },
	{ CUE_TALK, ACTOR_BILIBIN_INNKEEPER, FLAG_KOLIMA_CURSE_LIFTED, SCRIPT_REF(scCottageRoomsCTalkToInnkeeper) },
	{ CUE_TALK, ACTOR_BILIBIN_INN_WORKER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinYouCantReachKolimaNoMatter) },
	{ CUE_TALK, ACTOR_BILIBIN_CHEF, FLAG_KOLIMA_CURSE_LIFTED, SCRIPT_REF(scCottageRoomsCTalkToWorriedChef) },
	{ CUE_TALK, ACTOR_BILIBIN_KITCHEN_WORKER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinHeyMisterHeadChefAreYou) },
	{ CUE_TALK, ACTOR_BILIBIN_WAITRESS, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinStartCookingAlready) },
	{ CUE_TALK, ACTOR_BILIBIN_HUNGRY_DINER, FLAG_KOLIMA_CURSE_LIFTED, SCRIPT_REF(scCottageRoomsCTalkToHungryDiner) },
	{ CUE_TALK, ACTOR_BILIBIN_FURNITURE_SELLER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinWeCameHereBecauseLordMccoy) },
	{ CUE_TALK, ACTOR_BILIBIN_DINER_BIG_TREE, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinIThinkThatBigTreeDeep) },
	{ CUE_TALK, ACTOR_BILIBIN_DINER_WITH_PICKAXE, FLAG_KOLIMA_CURSE_LIFTED, SCRIPT_REF(scCottageRoomsCAskAboutTreeSpirit) },
	{ CUE_TALK, ACTOR_BILIBIN_DINER_TODAYS_SPECIAL, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinICantWaitToSeeTodays) },
	{ CUE_TALK, ACTOR_BILIBIN_JOB_SEEKER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinIWentToLordMccoyLooking) },
	{ CUE_TALK, ACTOR_BILIBIN_VILLAGER_ANCIENT_TREES, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinIveHeardSongsThatTellOf) },
	{ CUE_TALK, ACTOR_BILIBIN_YOUNG_MAN_ON_THE_CURSED_MAN, CONDITION_ALWAYS, SCRIPT_REF(scCottageRoomsCAskAboutHolyTree) },
	{ CUE_TALK, ACTOR_BILIBIN_WOMAN_ON_MCCOYS_PALACE, CONDITION_ALWAYS, MSGNO(MsgBilibinThankGoodnessTheyStoppedConstructionOf) },
	{ CUE_TALK, ACTOR_BILIBIN_MIA_ADMIRER, CONDITION_ALWAYS, MSGNO(MsgBilibinIAlwaysMissAltinInThe) },
	{ CUE_TALK, ACTOR_BILIBIN_BOY_SNOW, CONDITION_ALWAYS, MSGNO(MsgBilibinMyFatherSaysThatItsWarm) },
	{ CUE_TALK, ACTOR_BILIBIN_VILLAGER_WORRIED_FOR_KOLIMA, CONDITION_ALWAYS, MSGNO(MsgBilibinTheRoadToKolimaIsOpen) },
	{ CUE_TALK, ACTOR_BILIBIN_GIRL_SILK, CONDITION_ALWAYS, SCRIPT_REF(scCottageRoomsCAskAboutSilk) },
	{ CUE_TALK, ACTOR_BILIBIN_OLD_MAN_WATER_OF_HERMES, CONDITION_ALWAYS, MSGNO(MsgBilibinIThoughtISawLightsIn) },
	{ CUE_TALK, ACTOR_BILIBIN_OLD_WOMAN_LOST_YOUTH, CONDITION_ALWAYS, MSGNO(MsgBilibinHisEyesAreRedBecauseHe) },
	{ CUE_TALK, ACTOR_BILIBIN_WEAPON_MERCHANT, CONDITION_ALWAYS, SCRIPT_REF(scCottageRoomsCTalkToWeaponMerchant) },
	{ CUE_TALK, ACTOR_BILIBIN_ARMOR_MERCHANT, CONDITION_ALWAYS, SCRIPT_REF(scCottageRoomsCTalkToArmorMerchant) },
	{ CUE_TALK, ACTOR_BILIBIN_ITEM_MERCHANT, CONDITION_ALWAYS, SCRIPT_REF(scCottageRoomsCTalkToItemMerchant) },
	{ CUE_TALK, ACTOR_BILIBIN_INNKEEPER, CONDITION_ALWAYS, SCRIPT_REF(scCottageRoomsCTalkToInnkeeper) },
	{ CUE_TALK, ACTOR_BILIBIN_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgBilibinThereWillBeFewerGuestsNow) },
	{ CUE_TALK, ACTOR_BILIBIN_CHEF, CONDITION_ALWAYS, SCRIPT_REF(scCottageRoomsCTalkToBusyChef) },
	{ CUE_TALK, ACTOR_BILIBIN_KITCHEN_WORKER, CONDITION_ALWAYS, SCRIPT_REF(scCottageRoomsCTalkToKitchenWorker) },
	{ CUE_TALK, ACTOR_BILIBIN_WAITRESS, CONDITION_ALWAYS, SCRIPT_REF(scCottageRoomsCTalkToWaitress) },
	{ CUE_TALK, ACTOR_BILIBIN_HUNGRY_DINER, CONDITION_ALWAYS, MSGNO(MsgBilibinImALittleSurprisedThatThe) },
	{ CUE_TALK, ACTOR_BILIBIN_FURNITURE_SELLER, CONDITION_ALWAYS, MSGNO(MsgBilibinTheresNoBusinessHereSoIm) },
	{ CUE_TALK, ACTOR_BILIBIN_DINER_BIG_TREE, CONDITION_ALWAYS, MSGNO(MsgBilibinTheyServedSoMuchFoodToday) },
	{ CUE_TALK, ACTOR_BILIBIN_DINER_WITH_PICKAXE, CONDITION_ALWAYS, SCRIPT_REF(scCottageRoomsCTalkAboutMeal) },
	{ CUE_TALK, ACTOR_BILIBIN_DINER_TODAYS_SPECIAL, CONDITION_ALWAYS, MSGNO(MsgBilibinOooohThatsItITriedMy) },
	{ CUE_TALK, ACTOR_BILIBIN_JOB_SEEKER, CONDITION_ALWAYS, MSGNO(MsgBilibinTheIncidentWasResolvedBeforeI) },
	{ CUE_TALK, ACTOR_BILIBIN_VILLAGER_ANCIENT_TREES, CONDITION_ALWAYS, MSGNO(MsgBilibinIWonderWhatThatEerieHowling) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BILIBIN_YOUNG_MAN_ON_THE_CURSED_MAN, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinThisIsAllLordMccoysFault) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BILIBIN_WOMAN_ON_MCCOYS_PALACE, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinAhLadyMccoyIWouldJust) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BILIBIN_MIA_ADMIRER, FLAG_KOLIMA_CURSE_LIFTED, SCRIPT_REF(scCottageRoomsCReadMiaAdmirersMind) },
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
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BILIBIN_DINER_WITH_PICKAXE, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinIDontCareWhoDoesIt) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BILIBIN_DINER_TODAYS_SPECIAL, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinYippeeIsItReadyYetMy) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BILIBIN_JOB_SEEKER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinImGettingALittleScaredHearing) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BILIBIN_VILLAGER_ANCIENT_TREES, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinItHasBeenSaidThatA) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BILIBIN_YOUNG_MAN_ON_THE_CURSED_MAN, CONDITION_ALWAYS, MSGNO(MsgBilibinItTakesManyYearsForA) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BILIBIN_WOMAN_ON_MCCOYS_PALACE, CONDITION_ALWAYS, MSGNO(MsgBilibinSometimesIWishICouldSee) },
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
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BILIBIN_DINER_WITH_PICKAXE, CONDITION_ALWAYS, MSGNO(MsgBilibinIdBetterEatALotThat) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BILIBIN_DINER_TODAYS_SPECIAL, CONDITION_ALWAYS, MSGNO(MsgBilibinTheFoodIsSuchAGood) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BILIBIN_JOB_SEEKER, CONDITION_ALWAYS, MSGNO(MsgBilibinIfAKidLikeThisCould) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BILIBIN_VILLAGER_ANCIENT_TREES, CONDITION_ALWAYS, MSGNO(MsgBilibinItLooksLikeScaryThingsAre) },
	{ EXAMINE_CUE(EXAMINE_BARREL), 100, FLAG_BILIBIN_ANTIDOTE, FIND_ITEM(ITEM_ANTIDOTE) },
	{ EXAMINE_CUE(EXAMINE_BED), 101, FLAG_BILIBIN_SLEEP_BOMB, FIND_ITEM(ITEM_SLEEP_BOMB) },
	{ EXAMINE_CUE(EXAMINE_BARREL), 102, FLAG_BILIBIN_HARD_NUT, FIND_ITEM(ITEM_HARD_NUT) },
	{ EXAMINE_CUE(EXAMINE_OVEN), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBilibinLooksLikeEggSoupSoftEggs)) },
	{ EXAMINE_CUE(EXAMINE_OVEN), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBilibinFreshOmelettesBilibinoOmelettesUseWild)) },
	{ EXAMINE_CUE(EXAMINE_BOOKCASE), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBilibinTheresABookEntitledAllAbout)) },
	{ CUE_TABLE_END },
};
// clang-format on
