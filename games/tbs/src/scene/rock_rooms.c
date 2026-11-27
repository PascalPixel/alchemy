// The rooms inside Lunpa's buildings, and the house beside Suhalla Gate (SceneIdRockRooms). The
// game packs rooms that share a rocky tile style onto one map; the door the party comes in by
// decides which room they are in, and the area name follows it.
//
// Lunpa's doors (lunpa.c) lead into three homes, the weapon and armor shop, the item shop, the inn,
// the temple and a crate room the walkthrough calls the shop's attic. Two villagers live in each
// home and talk of Dodonpa, his fortress, and old Lunpa, who was lost in the great flood. The
// wooden box in the second home (metatile (23, 10)) holds a Lucky Medal and the barrel in the crate
// room (metatile (30, 53)) a Vial. Something is cooking in the ovens of the first and third homes:
// Thieves' Stew, and Thieves' Cookies in the shape of Dodonpa's face.
//
// While the village is sealed, the weapon and armor merchants will not sell and the innkeeper only
// offers his own home, though the item shop and the temple's Healer serve as usual. The inn's chef
// is the one villager let into the fortress, and talking to him or reading his mind tells of the
// prisoner he feeds there, Hammet; what Layana says in Kalay changes once the party has heard it.
// Once Donpa has dealt with his son (FLAG_DODONPA_LOCKED_UP), the shops sell their last stock, the
// inn takes guests, and the villagers talk of a shimmering creature seen slipping into the fortress
// and of the guests Donpa is expecting. `scGetCues` picks the sealed or the reopened table by that
// flag.
//
// The temple has a second entrance before the altar, where a party defeated in Lunpa wakes.
//
// The house beside Suhalla Gate's checkpoint (suhalla_gate.c) is a small room with two beds, where
// a traveling Healer offers the Sanctum's services. It has its own cue table. Entering it makes it
// the place a defeated party wakes, and hides the area name.
//
// Each door leads back out the way the party came in: a room's entrance number is also its
// doorway's metatile behavior and its exit (`ROOM_DOOR`).
//
// A merchant or Healer serves a leader who talks to them facing north, from in front of the
// counter; from any other side they only talk. The counters are invisible actors standing in front
// of the merchants, so talking across one reaches the merchant.

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "field/script.h"
#include "text/msg_ids.h"

// The rooms by the entrance the party comes in by; each is also its door's cue and exit kind
// (ROOM_DOOR).
typedef enum ROCK_ROOMS_ROOM
{
	ROOM_FIRST_HOME = 1,
	ROOM_SECOND_HOME,
	ROOM_THIRD_HOME,
	ROOM_ARMS_SHOP,
	ROOM_ITEM_SHOP,
	ROOM_INN,
	ROOM_TEMPLE,
	ROOM_CRATE_ROOM,
	ROOM_NINTH_UNUSED, // no exit leads here
	ROOM_GATE_TO_SUHALLA,
	ROOM_TEMPLE_SECOND_ENTRANCE
} ROCK_ROOMS_ROOM;

// A doorway's exit, its metatiles carrying its room's number.
#define ROOM_DOOR(room) {CUE_EXIT, (room), CONDITION_ALWAYS, (room)}

// The metatiles the rooms' examine cues answer. A doorway's metatiles carry the number of its room
// instead.
typedef enum ROCK_ROOMS_METATILE_BEHAVIOR
{
	// The wooden box in the second home, metatile (23, 10): a Lucky Medal.
	METATILE_BEHAVIOR_ROCK_ROOMS_WOODEN_BOX = 100,
	// The barrel in the crate room, metatile (30, 53): a Vial.
	METATILE_BEHAVIOR_ROCK_ROOMS_BARREL,
	// The oven in the first home, metatiles (6, 4) and (7, 4): Thieves' Stew.
	METATILE_BEHAVIOR_ROCK_ROOMS_FIRST_OVEN = 200,
	// The oven in the third home, metatiles (8, 29) and (9, 29): Thieves' Cookies.
	METATILE_BEHAVIOR_ROCK_ROOMS_SECOND_OVEN
} ROCK_ROOMS_METATILE_BEHAVIOR;

// The placed actors of Lunpa's houses, shops, inn and temple, numbered in the order of
// RockRoomsActorPlacements. Two people live in each of the three homes, named by their sprites and
// by what they say while the town is sealed.
typedef enum ROCK_ROOMS_ACTOR_ID
{
	// In the first home: this man with brown hair (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR) and a woman
	// with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR).
	ACTOR_MAN_ON_HIS_LATE_GRANDFATHER = ACTOR_FIRST_PLACED,
	ACTOR_WOMAN_NEVER_WORKING_FOR_DODONPA,
	// In the second home: this burly grey-haired man (SPRITE_BURLY_MAN_WITH_GREY_HAIR) and a woman
	// in a pink apron (SPRITE_WOMAN_IN_PINK_APRON).
	ACTOR_MAN_ON_DONPAS_FORTRESS,
	ACTOR_WOMAN_ON_THE_INNS_CHEF,
	// In the third home: this bent old man with a cane (SPRITE_OLD_MAN_WITH_CANE) and an old woman
	// (SPRITE_OLD_WOMAN).
	ACTOR_OLD_MAN_ON_LUNPAS_DEATH,
	ACTOR_OLD_WOMAN_ON_THE_GREAT_FLOOD,
	ACTOR_WEAPON_MERCHANT,
	ACTOR_ARMOR_MERCHANT,
	ACTOR_ITEM_MERCHANT,
	ACTOR_INNKEEPER,
	// A woman in a pink apron (SPRITE_WOMAN_IN_PINK_APRON) at the inn, beside the innkeeper.
	ACTOR_WOMAN_ASKING_FOR_DONPAS_GUEST,
	ACTOR_INN_WORKER,
	ACTOR_CHEF,
	// Behind the temple's altar; across it, the Sanctum's services (scRockRoomsTalkToHealer).
	ACTOR_HEALER,
	// In the Healer's sprite, a bearded man in a long robe (SPRITE_BEARDED_MAN_IN_LONG_ROBE), in
	// the house beside Suhalla Gate.
	ACTOR_TRAVELING_HEALER,
	// Invisible (SPRITE_BLANK), on the counter before each merchant, so that talking across it
	// reaches the merchant.
	ACTOR_WEAPON_COUNTER,
	ACTOR_ARMOR_COUNTER,
	ACTOR_ITEM_COUNTER
} ROCK_ROOMS_ACTOR_ID;

// Lunpa's shops, as mnShopRun numbers them.
enum
{
	SHOP_LUNPA_ITEMS   = 25,
	SHOP_LUNPA_WEAPONS = 29,
	SHOP_LUNPA_ARMOR   = 30
};

// Lunpa's inn, as mnInnCheckIn numbers the inns.
enum
{
	LUNPA_INN = 8
};

// clang-format off
const SCENE_ENTRANCE RockRoomsEntrances[] = {
	{ ENTRANCE_DEFAULT, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ ROOM_FIRST_HOME, CONDITION_ALWAYS, 192, 0, 184, FACING_NORTH, 0, 16, 0, 256, 224, 0 },
	{ ROOM_SECOND_HOME, CONDITION_ALWAYS, 432, 0, 264, FACING_NORTH, 0, 296, 48, 536, 288, 0 },
	{ ROOM_THIRD_HOME, CONDITION_ALWAYS, 160, 0, 584, FACING_NORTH, 0, 16, 400, 256, 624, 0 },
	{ ROOM_ARMS_SHOP, CONDITION_ALWAYS, 464, 0, 616, FACING_NORTH, 0, 296, 384, 536, 640, 0 },
	{ ROOM_ITEM_SHOP, CONDITION_ALWAYS, 704, 0, 584, FACING_NORTH, 0, 576, 352, 816, 608, 0 },
	{ ROOM_INN, CONDITION_ALWAYS, 288, 0, 840, FACING_NORTH, 0, 40, 656, 368, 944, 0 },
	{ ROOM_TEMPLE, CONDITION_ALWAYS, 688, 0, 280, FACING_NORTH, 0, 568, 32, 808, 304, 0 },
	{ ROOM_CRATE_ROOM, CONDITION_ALWAYS, 560, 0, 880, FACING_NORTH, 0, 400, 656, 640, 912, 0 },
	// XXX (no evidence): what this entrance was meant for; no exit leads to it, and it stands the leader in the empty dark between the crate room and the house at Suhalla Gate.
	{ ROOM_NINTH_UNUSED, CONDITION_ALWAYS, 672, 0, 888, FACING_NORTH, 0, 520, 656, 760, 896, 0 },
	{ ROOM_GATE_TO_SUHALLA, CONDITION_ALWAYS, 832, 0, 848, FACING_NORTH, 0, 680, 656, 920, 896, 0 },
	{ ROOM_TEMPLE_SECOND_ENTRANCE, CONDITION_ALWAYS, 688, 0, 184, FACING_NORTH, 0, 568, 32, 808, 288, 0 },
	{ SCENE_TABLE_END },
};

const Uint32 RockRoomsExits[] = {
	SCENE_EXITS(RockRooms),
	SCENE_EXIT(ROOM_FIRST_HOME, Lunpa, LUNPA_FROM_FIRST_HOME),
	SCENE_EXIT(ROOM_SECOND_HOME, Lunpa, LUNPA_FROM_SECOND_HOME),
	SCENE_EXIT(ROOM_THIRD_HOME, Lunpa, LUNPA_FROM_THIRD_HOME),
	SCENE_EXIT(ROOM_ARMS_SHOP, Lunpa, LUNPA_FROM_WEAPON_AND_ARMOR_SHOP),
	SCENE_EXIT(ROOM_ITEM_SHOP, Lunpa, LUNPA_FROM_ITEM_SHOP),
	SCENE_EXIT(ROOM_INN, Lunpa, LUNPA_FROM_INN),
	SCENE_EXIT(ROOM_TEMPLE, Lunpa, LUNPA_FROM_TEMPLE),
	SCENE_EXIT(ROOM_CRATE_ROOM, Lunpa, LUNPA_FROM_CRATE_ROOM),
	SCENE_EXIT(ROOM_GATE_TO_SUHALLA, SuhallaGateCheckpoint, SUHALLA_GATE_CHECKPOINT_FROM_ROCK_ROOMS),
	SCENE_EXITS_END,
};

const ACTOR_PLACE RockRoomsActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
	  144, 0, 168, FACING_WEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
	  96, 0, 128, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
	  392, 0, 240, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PINK_APRON, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
	  403, 0, 138, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
	  160, 0, 508, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
	  88, 0, 584, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_BALD_MAN_IN_RED, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
	  376, 0, 531, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
	  464, 0, 531, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_PONYTAIL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
	  720, 0, 499, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
	  264, 0, 744, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PINK_APRON, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
	  248, 0, 808, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
	  144, 0, 888, FACING_NORTHWEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_CHEF, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
	  152, 0, 744, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
	  688, 0, 152, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
	  776, 0, 800, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
	  376, 0, 544, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
	  464, 0, 544, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
	  720, 0, 512, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	{ SCENE_TABLE_END },
};

// clang-format on

extern const CUE      RockRoomsSealedCues[];
extern const CUE      RockRoomsReopenedCues[];
extern const CUE      RockRoomsGateToSuhallaCues[];

const SCENE_ENTRANCE *scGetEntrances(void)
{
	return RockRoomsEntrances;
}

const SCENE_EXIT_REGION *scGetExitRegions(void)
{
	return NULL;
}

const Uint32 *scGetExits(void)
{
	return RockRoomsExits;
}

// The villagers, shopkeepers and counters of every room, and the traveling Healer of the house
// beside Suhalla Gate.
const ACTOR_PLACE *scGetActorPlacements(void)
{
	return RockRoomsActorPlacements;
}

// The house beside Suhalla Gate has its own cues; Lunpa's rooms have one table for the sealed
// village and one for after Donpa has dealt with Dodonpa.
const CUE *scGetCues(void)
{
	if (MainRoutineStateWork.entrance == ROOM_GATE_TO_SUHALLA)
	{
		return RockRoomsGateToSuhallaCues;
	}
	if (gmFlagGet(FLAG_DODONPA_LOCKED_UP) != 0)
	{
		return RockRoomsReopenedCues;
	}
	return RockRoomsSealedCues;
}

// Talking to the innkeeper behind the counter of Lunpa's inn offers a night's rest once the village
// has reopened, and only a joke about his own home while it is sealed. Facing him across the
// counter after Donpa has dealt with Dodonpa checks the party in (`mnInnCheckIn`). From any other
// side, he asks whether they want to rest at the inn, and either answer is only words. While the
// village is sealed, he asks whether they want to sleep at his place, and a yes gets "Go rest in
// your own home!"
static void scRockRoomsTalkToInnkeeper(void)
{
	if (FACING_IS_NORTH(fdActorGet(ACTOR_PARTY_LEADER)->facing))
	{
		if (gmFlagGet(FLAG_DODONPA_LOCKED_UP) != 0)
		{
			mnInnCheckIn(LUNPA_INN, ACTOR_INNKEEPER);
			return;
		}
	}
	fdScriptStart();
	if (gmFlagGet(FLAG_DODONPA_LOCKED_UP) != 0)
	{
		fdScriptSetMessageId(MsgLunpaInnkeeperOffersRoom);
		// "Do you want rest in our inn?" Yes: "I see. In that case, go ahead and stay. I won't ask
		// why." No: "Well, do as you like. Feel free to stay here anytime you like."
		fdScriptAskYesNo(ACTOR_INNKEEPER, 0);
	}
	else
	{
		fdScriptSetMessageId(MsgLunpaInnkeeperOffersOwnHome);
		// "Do you want to sleep at my place?" Yes: "You must be kidding. Go rest in your own home!"
		// No: "Of course not! With no travelers in town, no one wants to..."
		fdScriptAskYesNo(ACTOR_INNKEEPER, 0);
	}
	fdScriptEnd();
}

// Talking to the chef at Lunpa's inn, west of the counter, while the village is sealed: he asks
// whether the party wants to hear how he delivered meals to the fortress. Yes tells of being led
// blindfolded down an echoing path underground; no, of his relief at getting back, and that the man
// he fed is called Hammet. Either way the party has now heard of the prisoner
// (FLAG_LUNPA_HEARD_OF_PRISONER), which changes what Layana says in Kalay.
static void scRockRoomsTalkToChef(void)
{
	fdScriptStart();
	fdScriptSetMessageId(MsgLunpaChefOffersStory);
	// "Do you want to hear about how I delivered meals to the fortress?" Yes: "I was blindfolded
	// and guided along a cool path. I walked for..." No: "I was so relieved when I got back to the
	// village. The guy I..."
	fdScriptAskYesNo(ACTOR_CHEF, 0);
	gmFlagSet(FLAG_LUNPA_HEARD_OF_PRISONER);
	fdScriptEnd();
}

// Mind Read on the chef at Lunpa's inn while the village is sealed: he feels bad for the poor man
// locked up in the dungeon. Like talking to him, it counts as hearing of the prisoner
// (FLAG_LUNPA_HEARD_OF_PRISONER).
static void scRockRoomsReadChefsThoughts(void)
{
	fdScriptStart();
	fdScriptSetMessageId(MsgLunpaChefThinksOfPrisoner);
	// "I feel bad for that poor man they've got locked up in the..."
	fdScriptShowMessage(ACTOR_CHEF, 0);
	gmFlagSet(FLAG_LUNPA_HEARD_OF_PRISONER);
	fdScriptEnd();
}

// Talking to the woman in an apron by the door of Lunpa's inn, once the village has reopened: she
// asks "Are you Donpa's guest?" A yes gets the royal treatment she was told to give, and a reminder
// to visit Donpa again; a no leaves her wondering who the guest is.
static void scRockRoomsAskWhetherDonpasGuest(void)
{
	fdScriptStart();
	fdScriptSetMessageId(MsgLunpaInnClerkAsksAboutGuest);
	// "Are you Donpa's guest?" Yes: "So, you're the one we were told to give the royal
	// treatment..." No: "Really, you're not? Well, then, who is Donpa's guest?"
	fdScriptAskYesNo(ACTOR_WOMAN_ASKING_FOR_DONPAS_GUEST, 0);
	fdScriptEnd();
}

// Talking to the Healer behind the altar of Lunpa Temple: facing him across it, the Sanctum's
// services, sealed village or not. From any other side he speaks of Dodonpa: while the village is
// sealed, that it is not fair to blame the Lunpans for him; once it has reopened, that a number of
// Dodonpa's men set out not long ago.
static void scRockRoomsTalkToHealer(void)
{
	if (FACING_IS_NORTH(fdActorGet(ACTOR_PARTY_LEADER)->facing))
	{
		mnSanctumRun(ACTOR_HEALER);
	}
	else if (gmFlagGet(FLAG_DODONPA_LOCKED_UP) != 0)
	{
		fdScriptStart();
		fdScriptSetMessageId(MsgLunpaTemplePriestReopened);
		// "A number of Dodonpa's men set out not too long ago. I wonder..."
		fdScriptShowMessage(ACTOR_HEALER, 0);
		fdScriptEnd();
	}
	else
	{
		fdScriptStart();
		fdScriptSetMessageId(MsgLunpaTemplePriestSealed);
		// "I think that thief Dodonpa is a terrible man. But it's not..."
		fdScriptShowMessage(ACTOR_HEALER, 0);
		fdScriptEnd();
	}
}

// Mind Read on the woman with a ponytail behind the counter of Lunpa's item shop: while the village
// is sealed she is already running out of the stock she bought; once it has reopened, she is almost
// out of products too, like the weapons vendors.
static void scRockRoomsReadItemMerchantsThoughts(void)
{
	if (gmFlagGet(FLAG_DODONPA_LOCKED_UP) != 0)
	{
		fdScriptStart();
		fdScriptSetMessageId(MsgLunpaIsntWeaponsVendors);
		// "It isn't just the weapons vendors whose inventory is..."
		fdScriptShowMessage(ACTOR_ITEM_MERCHANT, 0);
		fdScriptEnd();
	}
	else
	{
		fdScriptStart();
		fdScriptSetMessageId(MsgLunpaItemMerchantSealedThoughts);
		// "I bought a lot of stock, but I'm already running out..."
		fdScriptShowMessage(ACTOR_ITEM_MERCHANT, 0);
		fdScriptEnd();
	}
}

// Talking to the woman behind the counter of Lunpa's item shop: facing her across it opens the
// shop, even while the village is sealed. From any other side she talks of the weapon and armor
// merchants: holding up on the stock they bought while the village is sealed, selling the last of
// it once it has reopened.
static void scRockRoomsTalkToItemMerchant(void)
{
	if (FACING_IS_NORTH(fdActorGet(ACTOR_PARTY_LEADER)->facing))
	{
		mnShopRun(SHOP_LUNPA_ITEMS, ACTOR_ITEM_MERCHANT);
	}
	else if (gmFlagGet(FLAG_DODONPA_LOCKED_UP) != 0)
	{
		fdScriptStart();
		fdScriptSetMessageId(MsgLunpaItemMerchantReopened);
		// "The weapons and armor vendors are looking desperate. The..."
		fdScriptShowMessage(ACTOR_ITEM_MERCHANT, 0);
		fdScriptEnd();
	}
	else
	{
		fdScriptStart();
		fdScriptSetMessageId(MsgLunpaItemMerchantSealed);
		// "They bought a lot of stock, which is why they're holding up,..."
		fdScriptShowMessage(ACTOR_ITEM_MERCHANT, 0);
		fdScriptEnd();
	}
}

// Mind Read on the bald man in red behind the west counter of Lunpa's weapon and armor shop: while
// the village is sealed he fears he will have to close the shop; once it has reopened, he doubts
// Dodonpa's henchmen can use all those weapons.
static void scRockRoomsReadWeaponMerchantsThoughts(void)
{
	if (gmFlagGet(FLAG_DODONPA_LOCKED_UP) != 0)
	{
		fdScriptSetMessageId(MsgLunpaWeaponMerchantReopenedThoughts);
		// "All those weapons, but I doubt Dodonpa's henchmen can use them!"
		fdScriptShowMessage(ACTOR_WEAPON_MERCHANT, 0);
	}
	else
	{
		fdScriptSetMessageId(MsgLunpaWeaponMerchantSealedThoughts);
		// "I'll have to close this shop soon if things don't change..."
		fdScriptShowMessage(ACTOR_WEAPON_MERCHANT, 0);
	}
}

// Talking to the bald man in red behind the west counter of Lunpa's weapon and armor shop: while
// the village is sealed, from any side, he says they can't buy or sell anything. Once it has
// reopened, facing him across the counter opens the weapon shop, and from any other side he cries
// "To arms!" and urges the party to buy while stocks last.
static void scRockRoomsTalkToWeaponMerchant(void)
{
	Sint32 facing;

	facing = fdActorGet(ACTOR_PARTY_LEADER)->facing;
	if (gmFlagGet(FLAG_DODONPA_LOCKED_UP) != 0)
	{
		if (FACING_IS_NORTH(facing))
		{
			mnShopRun(SHOP_LUNPA_WEAPONS, ACTOR_WEAPON_MERCHANT);
		}
		else
		{
			fdScriptStart();
			fdScriptSetMessageId(MsgLunpaWeaponMerchantReopened);
			// "The end is near! Doom is upon us! To arms! To arms! Once..."
			fdScriptShowMessage(ACTOR_WEAPON_MERCHANT, 0);
			fdScriptEnd();
		}
	}
	else
	{
		fdScriptSetMessageId(MsgLunpaWeaponMerchantSealed);
		// "The village is sealed, so we can't buy or sell anything...."
		fdScriptShowMessage(ACTOR_WEAPON_MERCHANT, 0);
	}
}

// Mind Read on the young man in green behind the east counter of Lunpa's weapon and armor shop:
// while the village is sealed he wishes he had bought more stock; once it has reopened, he means to
// close the shop when his armor is sold.
static void scRockRoomsReadArmorMerchantsThoughts(void)
{
	if (gmFlagGet(FLAG_DODONPA_LOCKED_UP) != 0)
	{
		fdScriptSetMessageId(MsgLunpaArmorMerchantReopenedThoughts);
		// "Once I sell this armor, I'm closing shop... It's kind of a..."
		fdScriptShowMessage(ACTOR_ARMOR_MERCHANT, 0);
	}
	else
	{
		fdScriptSetMessageId(MsgLunpaArmorMerchantSealedThoughts);
		// "I should have bought more stock while I had the chance..."
		fdScriptShowMessage(ACTOR_ARMOR_MERCHANT, 0);
	}
}

// Talking to the young man in green behind the east counter of Lunpa's weapon and armor shop: while
// the village is sealed, from any side, he promises all the armor the party likes once it reopens.
// Once it has, facing him across the counter opens the armor shop, and from any other side he
// boasts of his incredible products.
static void scRockRoomsTalkToArmorMerchant(void)
{
	Sint32 facing;

	facing = fdActorGet(ACTOR_PARTY_LEADER)->facing;
	if (gmFlagGet(FLAG_DODONPA_LOCKED_UP) != 0)
	{
		if (FACING_IS_NORTH(facing))
		{
			mnShopRun(SHOP_LUNPA_ARMOR, ACTOR_ARMOR_MERCHANT);
		}
		else
		{
			fdScriptStart();
			fdScriptSetMessageId(MsgLunpaArmorMerchantReopened);
			// "What do you think? I've got some incredible products here,..."
			fdScriptShowMessage(ACTOR_ARMOR_MERCHANT, 0);
			fdScriptEnd();
		}
	}
	else
	{
		fdScriptSetMessageId(MsgLunpaArmorMerchantSealed);
		// "I'll sell you as much armor as you like, once the village is..."
		fdScriptShowMessage(ACTOR_ARMOR_MERCHANT, 0);
	}
}

// Entering the house beside Suhalla Gate makes it the place a defeated party wakes, and shows no
// area name. In every room, the three invisible counter actors are given no shadow
// (`fdActorSetSpriteFlags` with no flags), so nothing shows where they stand.
Sint32 scInit(void)
{
	Sint16 entrance;

	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	entrance                   = MainRoutineStateWork.entrance;
	if (entrance == ROOM_GATE_TO_SUHALLA)
	{
		gmFlagClear(FLAG_SHOW_AREA_NAME);
		MainRoutineStateWork.restartScene    = SCENE_ROCK_ROOMS;
		MainRoutineStateWork.restartEntrance = entrance;
	}
	fdActorSetSpriteFlags(fdActorGet(ACTOR_WEAPON_COUNTER), 0);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_ARMOR_COUNTER), 0);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_ITEM_COUNTER), 0);
	return 0;
}

// Talking to the traveling Healer, the bearded man in a long robe in the house beside Suhalla Gate:
// facing him from the south, the Sanctum's services; from any other side, "I have been traveling
// the lands spreading good will."
static void scRockRoomsTalkToTravelingHealer(void)
{
	if (FACING_IS_NORTH(fdActorGet(ACTOR_PARTY_LEADER)->facing))
	{
		// BUG: the temple's Healer, not the traveling one. The Sanctum only takes its keeper's
		// portrait from the actor (mnSanctumRun), and both Healers are placed in this scene in the
		// same sprite, so the player sees no difference.
#if FIX_LUNPA_TRAVELING_HEALER_SANCTUM
		mnSanctumRun(ACTOR_TRAVELING_HEALER);
#else
		mnSanctumRun(ACTOR_HEALER);
#endif
	}
	else
	{
		fdScriptSetMessageId(MsgLunpaTravelingPriest);
		// "I have been traveling the lands spreading good will."
		fdScriptShowMessage(ACTOR_TRAVELING_HEALER, 0);
	}
}

// clang-format off
const CUE RockRoomsSealedCues[] = {
	ROOM_DOOR(ROOM_FIRST_HOME),
	ROOM_DOOR(ROOM_SECOND_HOME),
	ROOM_DOOR(ROOM_THIRD_HOME),
	ROOM_DOOR(ROOM_ARMS_SHOP),
	ROOM_DOOR(ROOM_ITEM_SHOP),
	ROOM_DOOR(ROOM_INN),
	ROOM_DOOR(ROOM_TEMPLE),
	ROOM_DOOR(ROOM_CRATE_ROOM),
	{ CUE_TALK, ACTOR_MAN_ON_HIS_LATE_GRANDFATHER, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsFirstHomeASealed) },
	{ CUE_TALK, ACTOR_WOMAN_NEVER_WORKING_FOR_DODONPA, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsFirstHomeBSealed) },
	{ CUE_TALK, ACTOR_MAN_ON_DONPAS_FORTRESS, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsSecondHomeASealed) },
	{ CUE_TALK, ACTOR_WOMAN_ON_THE_INNS_CHEF, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsSecondHomeBSealed) },
	{ CUE_TALK, ACTOR_OLD_MAN_ON_LUNPAS_DEATH, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsThirdHomeASealed) },
	{ CUE_TALK, ACTOR_OLD_WOMAN_ON_THE_GREAT_FLOOD, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsThirdHomeBSealed) },
	{ CUE_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, SCRIPT_REF(scRockRoomsTalkToWeaponMerchant) },
	{ CUE_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, SCRIPT_REF(scRockRoomsTalkToArmorMerchant) },
	{ CUE_TALK, ACTOR_WEAPON_COUNTER, CONDITION_ALWAYS, SCRIPT_REF(scRockRoomsTalkToWeaponMerchant) },
	{ CUE_TALK, ACTOR_ARMOR_COUNTER, CONDITION_ALWAYS, SCRIPT_REF(scRockRoomsTalkToArmorMerchant) },
	{ CUE_TALK, ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, SCRIPT_REF(scRockRoomsTalkToItemMerchant) },
	{ CUE_TALK, ACTOR_ITEM_COUNTER, CONDITION_ALWAYS, SCRIPT_REF(scRockRoomsTalkToItemMerchant) },
	{ CUE_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, SCRIPT_REF(scRockRoomsTalkToInnkeeper) },
	{ CUE_TALK, ACTOR_WOMAN_ASKING_FOR_DONPAS_GUEST, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsInnClerkSealed) },
	{ CUE_TALK, ACTOR_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsInnWorkerSealed) },
	{ CUE_TALK, ACTOR_CHEF, CONDITION_ALWAYS, SCRIPT_REF(scRockRoomsTalkToChef) },
	{ CUE_TALK, ACTOR_HEALER, CONDITION_ALWAYS, SCRIPT_REF(scRockRoomsTalkToHealer) },
	{ CUE_TALK, ACTOR_TRAVELING_HEALER, CONDITION_ALWAYS, SCRIPT_REF(scRockRoomsTalkToTravelingHealer) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_HIS_LATE_GRANDFATHER, CONDITION_ALWAYS,
	  MSGNO(MsgLunpaInteriorsFirstHomeASealedThoughts) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_NEVER_WORKING_FOR_DODONPA, CONDITION_ALWAYS,
	  MSGNO(MsgLunpaInteriorsFirstHomeBSealedThoughts) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_DONPAS_FORTRESS, CONDITION_ALWAYS,
	  MSGNO(MsgLunpaInteriorsSecondHomeASealedThoughts) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_INNS_CHEF, CONDITION_ALWAYS,
	  MSGNO(MsgLunpaInteriorsSecondHomeBSealedThoughts) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_LUNPAS_DEATH, CONDITION_ALWAYS,
	  MSGNO(MsgLunpaInteriorsThirdHomeASealedThoughts) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_THE_GREAT_FLOOD, CONDITION_ALWAYS,
	  MSGNO(MsgLunpaInteriorsThirdHomeBSealedThoughts) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS,
	  SCRIPT_REF(scRockRoomsReadWeaponMerchantsThoughts) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS,
	  SCRIPT_REF(scRockRoomsReadArmorMerchantsThoughts) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_COUNTER, CONDITION_ALWAYS,
	  SCRIPT_REF(scRockRoomsReadWeaponMerchantsThoughts) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_COUNTER, CONDITION_ALWAYS,
	  SCRIPT_REF(scRockRoomsReadArmorMerchantsThoughts) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS,
	  SCRIPT_REF(scRockRoomsReadItemMerchantsThoughts) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_COUNTER, CONDITION_ALWAYS,
	  SCRIPT_REF(scRockRoomsReadItemMerchantsThoughts) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS,
	  MSGNO(MsgLunpaInteriorsInnkeeperSealedThoughts) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ASKING_FOR_DONPAS_GUEST, CONDITION_ALWAYS,
	  MSGNO(MsgLunpaInteriorsInnClerkSealedThoughts) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_WORKER, CONDITION_ALWAYS,
	  MSGNO(MsgLunpaInteriorsInnWorkerSealedThoughts) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHEF, CONDITION_ALWAYS,
	  SCRIPT_REF(scRockRoomsReadChefsThoughts) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS,
	  MSGNO(MsgLunpaInteriorsTemplePriestSealedThoughts) },
	{ EXAMINE_CUE(EXAMINE_WOODEN_BOX), METATILE_BEHAVIOR_ROCK_ROOMS_WOODEN_BOX, FLAG_LUNPA_WOODEN_BOX_LUCKY_MEDAL,
	  FIND_ITEM(ITEM_LUCKY_MEDAL) },
	{ EXAMINE_CUE(EXAMINE_BARREL), METATILE_BEHAVIOR_ROCK_ROOMS_BARREL, FLAG_LUNPA_BARREL_VIAL, FIND_ITEM(ITEM_VIAL) },
	{ EXAMINE_CUE(EXAMINE_OVEN), METATILE_BEHAVIOR_ROCK_ROOMS_FIRST_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLunpaInteriorsFirstOven)) },
	{ EXAMINE_CUE(EXAMINE_OVEN), METATILE_BEHAVIOR_ROCK_ROOMS_SECOND_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLunpaInteriorsSecondOven)) },
	{ CUE_TABLE_END },
};

const CUE RockRoomsReopenedCues[] = {
	ROOM_DOOR(ROOM_FIRST_HOME),
	ROOM_DOOR(ROOM_SECOND_HOME),
	ROOM_DOOR(ROOM_THIRD_HOME),
	ROOM_DOOR(ROOM_ARMS_SHOP),
	ROOM_DOOR(ROOM_ITEM_SHOP),
	ROOM_DOOR(ROOM_INN),
	ROOM_DOOR(ROOM_TEMPLE),
	ROOM_DOOR(ROOM_CRATE_ROOM),
	{ CUE_TALK, ACTOR_MAN_ON_HIS_LATE_GRANDFATHER, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsFirstHomeAReopened) },
	{ CUE_TALK, ACTOR_WOMAN_NEVER_WORKING_FOR_DODONPA, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsFirstHomeBReopened) },
	{ CUE_TALK, ACTOR_MAN_ON_DONPAS_FORTRESS, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsSecondHomeAReopened) },
	{ CUE_TALK, ACTOR_WOMAN_ON_THE_INNS_CHEF, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsSecondHomeBReopened) },
	{ CUE_TALK, ACTOR_OLD_MAN_ON_LUNPAS_DEATH, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsThirdHomeAReopened) },
	{ CUE_TALK, ACTOR_OLD_WOMAN_ON_THE_GREAT_FLOOD, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsThirdHomeBReopened) },
	{ CUE_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, SCRIPT_REF(scRockRoomsTalkToWeaponMerchant) },
	{ CUE_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, SCRIPT_REF(scRockRoomsTalkToArmorMerchant) },
	{ CUE_TALK, ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, SCRIPT_REF(scRockRoomsTalkToItemMerchant) },
	{ CUE_TALK, ACTOR_ITEM_COUNTER, CONDITION_ALWAYS, SCRIPT_REF(scRockRoomsTalkToItemMerchant) },
	{ CUE_TALK, ACTOR_WEAPON_COUNTER, CONDITION_ALWAYS, SCRIPT_REF(scRockRoomsTalkToWeaponMerchant) },
	{ CUE_TALK, ACTOR_ARMOR_COUNTER, CONDITION_ALWAYS, SCRIPT_REF(scRockRoomsTalkToArmorMerchant) },
	{ CUE_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, SCRIPT_REF(scRockRoomsTalkToInnkeeper) },
	{ CUE_TALK, ACTOR_WOMAN_ASKING_FOR_DONPAS_GUEST, CONDITION_ALWAYS, SCRIPT_REF(scRockRoomsAskWhetherDonpasGuest) },
	{ CUE_TALK, ACTOR_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsInnWorkerReopened) },
	{ CUE_TALK, ACTOR_CHEF, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsChefReopened) },
	{ CUE_TALK, ACTOR_HEALER, CONDITION_ALWAYS, SCRIPT_REF(scRockRoomsTalkToHealer) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_HIS_LATE_GRANDFATHER, CONDITION_ALWAYS,
	  MSGNO(MsgLunpaInteriorsFirstHomeAReopenedThoughts) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_NEVER_WORKING_FOR_DODONPA, CONDITION_ALWAYS,
	  MSGNO(MsgLunpaInteriorsFirstHomeBReopenedThoughts) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_DONPAS_FORTRESS, CONDITION_ALWAYS,
	  MSGNO(MsgLunpaInteriorsSecondHomeAReopenedThoughts) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_INNS_CHEF, CONDITION_ALWAYS,
	  MSGNO(MsgLunpaInteriorsSecondHomeBReopenedThoughts) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_LUNPAS_DEATH, CONDITION_ALWAYS,
	  MSGNO(MsgLunpaInteriorsThirdHomeAReopenedThoughts) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_THE_GREAT_FLOOD, CONDITION_ALWAYS,
	  MSGNO(MsgLunpaInteriorsThirdHomeBReopenedThoughts) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS,
	  SCRIPT_REF(scRockRoomsReadWeaponMerchantsThoughts) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS,
	  SCRIPT_REF(scRockRoomsReadArmorMerchantsThoughts) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_COUNTER, CONDITION_ALWAYS,
	  SCRIPT_REF(scRockRoomsReadWeaponMerchantsThoughts) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_COUNTER, CONDITION_ALWAYS,
	  SCRIPT_REF(scRockRoomsReadArmorMerchantsThoughts) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS,
	  SCRIPT_REF(scRockRoomsReadItemMerchantsThoughts) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_COUNTER, CONDITION_ALWAYS,
	  SCRIPT_REF(scRockRoomsReadItemMerchantsThoughts) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS,
	  MSGNO(MsgLunpaInteriorsInnkeeperReopenedThoughts) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ASKING_FOR_DONPAS_GUEST, CONDITION_ALWAYS,
	  MSGNO(MsgLunpaInteriorsInnClerkReopenedThoughts) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_WORKER, CONDITION_ALWAYS,
	  MSGNO(MsgLunpaInteriorsInnWorkerReopenedThoughts) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHEF, CONDITION_ALWAYS,
	  MSGNO(MsgLunpaInteriorsChefReopenedThoughts) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS,
	  MSGNO(MsgLunpaInteriorsTemplePriestReopenedThoughts) },
	{ EXAMINE_CUE(EXAMINE_WOODEN_BOX), METATILE_BEHAVIOR_ROCK_ROOMS_WOODEN_BOX, FLAG_LUNPA_WOODEN_BOX_LUCKY_MEDAL,
	  FIND_ITEM(ITEM_LUCKY_MEDAL) },
	{ EXAMINE_CUE(EXAMINE_BARREL), METATILE_BEHAVIOR_ROCK_ROOMS_BARREL, FLAG_LUNPA_BARREL_VIAL, FIND_ITEM(ITEM_VIAL) },
	{ EXAMINE_CUE(EXAMINE_OVEN), METATILE_BEHAVIOR_ROCK_ROOMS_FIRST_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLunpaInteriorsFirstOven)) },
	{ EXAMINE_CUE(EXAMINE_OVEN), METATILE_BEHAVIOR_ROCK_ROOMS_SECOND_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLunpaInteriorsSecondOven)) },
	{ CUE_TABLE_END },
};
// clang-format on

const CUE RockRoomsGateToSuhallaCues[] = {
	ROOM_DOOR(ROOM_GATE_TO_SUHALLA),
	{CUE_TALK,
                            ACTOR_TRAVELING_HEALER, CONDITION_ALWAYS,
                            SCRIPT_REF(scRockRoomsTalkToTravelingHealer)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
                            ACTOR_TRAVELING_HEALER, CONDITION_ALWAYS,
                            MSGNO(MsgLunpaInteriorsTravelingPriestThoughts)},
	{CUE_TABLE_END},
};
