// Inside Garoh's houses, in three scenes that share this overlay. The first
// (SceneIdGarohRoomsHouses) holds four round houses side by side: the home of the young werewolf's
// family, the old couple's home, the weapons and armor shop, and the inn. The second is the Item
// Shopkeeper's house (SceneIdGarohRoomsShop) and the third the sanctum (SceneIdGarohRoomsSanctum).
// Stairs in some of the houses lead down into the tunnels under the village.
//
// Garoh is first seen on a night of the full moon (FLAG_GAROH_NIGHT), when its people hide that
// they are werewolves: the weapons and armor shop sells only claws and fangs, and two travelers who
// climbed Air's Rock for nothing lie asleep at the inn. After the night with Maha the party wakes
// at the inn and is told the stay is on the house (scGarohRoomsWakeAtInn); from then on it is day
// (FLAG_GAROH_SECRET_SHARED) and everyone talks openly.

#include "field/field_mode_script.h"
#include "field/field.h"
#include "game/game.h"
#include "field/script_inline.h"
#include "text/msg_ids.h"
#include "scene/scene.h"
#include "shop/shop.h"

// The morning after a night at the inn (scGarohRoomsWakeAtInn).
enum
{
	ENTRANCE_WAKE_AT_INN = 99
};

// The actors of the four houses, numbering the rows of GarohRoomsHousesActorPlacements.
typedef enum GAROH_HOUSES_ACTOR_ID
{
	// The father of the young werewolf who led the party to Maha. His script is
	// scGarohRoomsAskWhatTheWanderersSeek.
	ACTOR_PUPS_FATHER = ACTOR_FIRST_PLACED,
	ACTOR_PUPS_MOTHER,
	ACTOR_PUPS_BROTHER,
	// A werewolf who climbed Air's Rock for Maha's powers and did not gain them. His script is
	// scGarohRoomsAskAboutReveal.
	ACTOR_REVEAL_SEEKER,
	ACTOR_REVEAL_SEEKERS_MATE,
	ACTOR_WEAPONS_VENDOR,
	ACTOR_ARMOR_VENDOR,
	ACTOR_INNKEEPER, // scGarohRoomsTalkToInnkeeper checks the party in
	                 // The woman beside him, who greets the party the morning after.
	ACTOR_INN_WORKER,
	// A traveler who climbed Air's Rock and found nothing, asleep at night.
	ACTOR_DISAPPOINTED_TRAVELER,
	// His companion, still puzzling over the rock that would not spin.
	ACTOR_STUBBORN_TRAVELER,
	// Unseen markers on the two beds the travelers sleep in at night, so that talking to a bed, or
	// reading its sleeper's mind, reaches the traveler lying in it; they are taken away in the
	// morning.
	ACTOR_DISAPPOINTED_TRAVELERS_BED,
	ACTOR_STUBBORN_TRAVELERS_BED
} GAROH_HOUSES_ACTOR_ID;

// The Item Shopkeeper's house's actors, numbering the rows of GarohRoomsShopActorPlacements.
typedef enum GAROH_SHOP_ACTOR_ID
{
	ACTOR_AIRS_ROCK_GUARDIAN = ACTOR_FIRST_PLACED,
	ACTOR_VILLAGER_ON_WHIRLWIND,
	ACTOR_VILLAGER_ON_THE_WIND_STONE,
	ACTOR_ITEM_SHOPKEEPER
} GAROH_SHOP_ACTOR_ID;

// The Sanctum's priest, its one actor.
typedef enum GAROH_SANCTUM_ACTOR_ID
{
	ACTOR_PRIEST = ACTOR_FIRST_PLACED
} GAROH_SANCTUM_ACTOR_ID;

// The stoves, by their metatiles' behaviour in the map, each with something different cooking on
// it.
typedef enum GAROH_ROOMS_METATILE_BEHAVIOR
{
	// Bug larvae frying, in the young werewolf's family's home.
	METATILE_BEHAVIOR_LARVAE_STOVE = 209,
	// Lamb on the bone, at the inn.
	METATILE_BEHAVIOR_LAMB_STOVE,
	// Lobster grilled in the shell, in the Item Shopkeeper's house.
	METATILE_BEHAVIOR_LOBSTER_STOVE,
	// Newts on skewers, in the old couple's home.
	METATILE_BEHAVIOR_NEWT_STOVE,
	// A pot of strange plants, in the weapons and armor shop.
	METATILE_BEHAVIOR_PLANT_POT_STOVE
} GAROH_ROOMS_METATILE_BEHAVIOR;

extern ACTOR_PLACE    GarohRoomsNoActorPlacements[];
extern ACTOR_PLACE    GarohRoomsHousesActorPlacements[];
extern ACTOR_PLACE    GarohRoomsShopActorPlacements[];
extern ACTOR_PLACE    GarohRoomsSanctumActorPlacements[];
extern CUE            GarohRoomsDoorCues[];
extern CUE            GarohRoomsHousesNightCues[];
extern CUE            GarohRoomsShopNightCues[];
extern CUE            GarohRoomsSanctumCues[];
extern CUE            GarohRoomsHousesMorningCues[];
extern CUE            GarohRoomsShopMorningCues[];

void                  scGarohRoomsWakeAtInn(void);

const SCENE_ENTRANCE *scGetEntrances(void)
{
	return SceneEntrances;
}

const SCENE_REGION *scGetExitRegions(void)
{
	return 0;
}

const Uint32 *scGetExits(void)
{
	return SceneExits;
}

const ACTOR_PLACE *scGetActorPlacements(void)
{
	Sint16 scene = MainRoutineStateWork.next;

	if (scene == SCENE_GAROH_ROOMS_HOUSES)
		return GarohRoomsHousesActorPlacements;
	if (scene == SCENE_GAROH_ROOMS_SHOP)
		return GarohRoomsShopActorPlacements;
	if (scene == SCENE_GAROH_ROOMS_SANCTUM)
		return GarohRoomsSanctumActorPlacements;
	return GarohRoomsNoActorPlacements;
}

// Garoh's shops, as mnShopRun numbers them.
enum
{
	SHOP_GAROH_WEAPONS = 10,
	SHOP_GAROH_ARMOR   = 11,
	SHOP_GAROH_ITEMS   = 12
};

// Garoh's inn, as mnInnCheckIn numbers the inns.
enum
{
	GAROH_INN = 3
};

// Talking to the Weapons Vendor, the bald man in red behind the counter of the weapons and armor
// shop. By day, from in front of the counter, his shop opens; at night he is sorry, but on nights
// of the full moon they only sell claws and fangs.
void scGarohRoomsTalkToWeaponsVendor(Sint32 actor)
{
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);

	Uint16 facing = FACING_NEAREST_CARDINAL(leader->facing);

	if (facing == FACING_NORTH && syGameFlagGet(FLAG_GAROH_SECRET_SHARED))
	{
		mnShopRun(SHOP_GAROH_WEAPONS, actor);
	}
	else
	{
		if (syGameFlagGet(FLAG_GAROH_SECRET_SHARED))
			// "What do you think? I'll bet these weapons suit you better than any fangs would!"
			fdScriptSetMessageId((Sint32)MsgGarohRoomsWhatDoYouThinkIll);
		else
			// "I'm sorry, but on nights of the full moon, we only sell claws and fangs."
			fdScriptSetMessageId((Sint32)MsgGarohRoomsImSorryButOnNights);
		fdScriptShowMessageAndWait(actor, 0);
	}
}

// Talking to the Armor Vendor, the man with the orange beard beside the Weapons Vendor. By day,
// from in front of the counter, his shop opens; at night he only says it is unfortunate they came
// on a night of the full moon.
void scGarohRoomsTalkToArmorVendor(Sint32 actor)
{
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);

	Uint16 facing = FACING_NEAREST_CARDINAL(leader->facing);

	if (facing == FACING_NORTH && syGameFlagGet(FLAG_GAROH_SECRET_SHARED))
	{
		mnShopRun(SHOP_GAROH_ARMOR, actor);
	}
	else
	{
		if (syGameFlagGet(FLAG_GAROH_SECRET_SHARED))
			// "When we change form, it's best to be unclothed."
			fdScriptSetMessageId((Sint32)MsgGarohRoomsWhenWeChangeFormIts);
		else
			// "Unfortunate that you should come on a night of the full moon, sir..."
			fdScriptSetMessageId((Sint32)MsgGarohRoomsUnfortunateThatYouShouldCome);
		fdScriptShowMessageAndWait(actor, 0);
	}
}

// Talking to the Item Shopkeeper, the woman with a ponytail in her house: from in front of the
// counter her shop opens, night or day. From anywhere else she talks; at night she asks whether the
// party came from Alhafra.
void scGarohRoomsTalkToItemShopkeeper(Sint32 actor)
{
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);

	Uint16 facing = FACING_NEAREST_CARDINAL(leader->facing);

	if (facing == FACING_NORTH)
	{
		mnShopRun(SHOP_GAROH_ITEMS, actor);
	}
	else if (syGameFlagGet(FLAG_GAROH_SECRET_SHARED))
	{
		fdScriptSetMessageId((Sint32)MsgGarohRoomsIveHeardStoriesAboutOther);
		// "I've heard stories about other mountains like our own Air's Rock..."
		fdScriptShowMessageAndWait(actor, 0);
	}
	else
	{
		Sint32 msg = (Sint32)MsgGarohRoomsDidYouComeHereFrom;

		// "Did you come here from Alhafra?" Yes: "Really? You must have gone through the cave..."
		// No: "Oh, so you must have come by way of Mikasalla."
		fdScriptSetMessageId(msg);
		fdMessageShowAtActor(actor, 0);
		if (fdScriptAskYesNoWithActorAnimation(fdPartyGetLeaderActor(), 0) == 0)
		{
			fdScriptWait(10);
			fdScriptSetMessageId(msg + 1);
		}
		else
		{
			fdScriptWait(20);
			fdScriptSetMessageId(msg + 2);
		}
		fdScriptShowMessageAndWait(actor, 0);
	}
}

// Talking to the innkeeper behind the inn's counter: from in front of it he offers a night's stay,
// and from elsewhere he only chats.
void scGarohRoomsTalkToInnkeeper(Sint32 actor)
{
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);

	Uint16 facing = FACING_NEAREST_CARDINAL(leader->facing);

	if (facing == FACING_NORTH)
	{
		mnInnCheckIn(GAROH_INN, actor);
	}
	else
	{
		if (syGameFlagGet(FLAG_GAROH_SECRET_SHARED))
			// "If you can pass the tests of Air's Rock, you can do anything!"
			fdScriptSetMessageId((Sint32)MsgGarohRoomsIfYouCanPassThe);
		else
			// "A clear night around here always means fair weather on the morrow..."
			fdScriptSetMessageId((Sint32)MsgGarohRoomsAClearNightAroundHere);
		fdScriptShowMessageAndWait(actor, 0);
	}
}

// Talking to the priest in Garoh's sanctum: he turns to Felix, and from in front of his counter
// offers the sanctum's services; from elsewhere he only chats. He then faces south again.
void scGarohRoomsTalkToPriest(void)
{
	Sint16 facing;

	facing = FACING_NEAREST_CARDINAL(fdActorGetById(MainRoutineStateWork.leaderActorId)->facing);
	fdActorFaceActorById(ACTOR_PRIEST, MainRoutineStateWork.leaderActorId, 0);
	if ((Uint16)facing == FACING_NORTH)
	{
		mnSanctumRun(ACTOR_PRIEST);
	}
	else
	{
		if (syGameFlagGet(FLAG_GAROH_SECRET_SHARED))
			// "I've heard about you. They say you're special, like the people in this town."
			fdScriptSetMessageId((Sint32)MsgGarohRoomsIveHeardAboutYouThey);
		else
			// "The people here may be different, but they certainly are nice..."
			fdScriptSetMessageId((Sint32)MsgGarohRoomsThePeopleHereMayBe);
		fdScriptShowMessageAndWait(ACTOR_PRIEST, 0);
	}
	fdActorFaceDirection(ACTOR_PRIEST, FACING_SOUTH, 0);
}

// Talking at night to the young werewolf's father, the man with brown hair pacing his family's
// home: "Good evening, wanderers! Are you looking for something?", and then whether they are
// searching for werewolves.
void scGarohRoomsAskWhatTheWanderersSeek(Sint32 actor)
{
	Sint32 msg;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	msg = (Sint32)MsgGarohRoomsGoodEveningWanderersAreYou;
	fdScriptSetMessageId(msg);
	// "Good evening, wanderers! Are you looking for something?"
	fdMessageShowAtActor(actor, 0);
	if (fdScriptAskYesNoWithActorAnimation(MainRoutineStateWork.leaderActorId, 0) == 0)
	{
		fdScriptSetMessageId(msg + 2);
		// "Searching for werewolves, eh?"
		fdMessageShowAtActor(actor, 0);
		if (fdScriptAskYesNoWithActorAnimation(MainRoutineStateWork.leaderActorId, 0) == 0)
		{
			// "You don't really believe they exist, do you? If so, you'd best..."
			fdScriptShowMessageAndWait(actor, 0);
		}
		else
		{
			// Past "You don't really believe they exist, do you?" to the next line.
			FieldStateWork->msg++;
			// "Hehrrr... Sorry. Just thought I'd ask. We get some crazy folk..."
			fdScriptShowMessageAndWait(actor, 0);
		}
	}
	else
	{
		// "Just...staying up late, eh? Can't say I blame you, on a night like..."
		fdScriptShowMessageAndWait(actor, 0);
	}
	fdScriptEnd();
}

// Talking to the disappointed traveler, one of the two warriors in golden helmets at the inn. At
// night, asleep in his bed, he mumbles "What a waste... Just a big rock..."; by day he grumbles
// that the wolves were noisy again last night.
void scGarohRoomsTalkToDisappointedTraveler(void)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	if (syGameFlagGet(FLAG_GAROH_SECRET_SHARED))
		// "Those wolves were really noisy last night, too."
		fdScriptSetMessageId((Sint32)MsgGarohRoomsThoseWolvesWereReallyNoisy);
	else
		// "Zzzz... Zzzz... What a waste... Just a big rock..."
		fdScriptSetMessageId((Sint32)MsgGarohRoomsZzzzZzzzWhatAWaste);
	fdScriptShowMessageAndWait(ACTOR_DISAPPOINTED_TRAVELER, 0);
	fdScriptEnd();
}

// Talking to the stubborn traveler, the other warrior in a golden helmet at the inn. At night,
// asleep in his bed, he mumbles "Whirlwind? Wind Stone?"; by day he wants to stay and try climbing
// Air's Rock once more.
void scGarohRoomsTalkToStubbornTraveler(void)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	if (syGameFlagGet(FLAG_GAROH_SECRET_SHARED))
		// "Maybe we should stay and try climbing Air's Rock one more time."
		fdScriptSetMessageId((Sint32)MsgGarohRoomsMaybeWeShouldStayAnd);
	else
		// "Zzz... Whuzza? Whirlwind? Wind Stone? Zzzzrrrgg..."
		fdScriptSetMessageId((Sint32)MsgGarohRoomsZzzWhuzzaWhirlwindWindStone);
	fdScriptShowMessageAndWait(ACTOR_STUBBORN_TRAVELER, 0);
	fdScriptEnd();
}

// Mind Read on the disappointed traveler at the inn. At night he is bitter that he climbed all over
// that stupid rock for nothing; by day he is scared of wolves and keeps indoors at night.
void scGarohRoomsReadDisappointedTravelersMind(void)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	if (syGameFlagGet(FLAG_GAROH_SECRET_SHARED))
		// "I'm scared of wolves, so I don't go out much at night..."
		fdScriptSetMessageId((Sint32)MsgGarohRooms64);
	else
		// "...Climbed all over that stupid rock, and for what? Nothing!"
		fdScriptSetMessageId((Sint32)MsgGarohRoomsClimbedAllOverThatStupid);
	fdScriptShowMessageAndWait(ACTOR_DISAPPOINTED_TRAVELER, 0);
	fdScriptEnd();
}

// Mind Read on the stubborn traveler at the inn. At night he wonders how he was supposed to make so
// huge a rock spin; by day he thinks a lever would do it.
void scGarohRoomsReadStubbornTravelersMind(void)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	if (syGameFlagGet(FLAG_GAROH_SECRET_SHARED))
		// "If I used a lever, I could probably make that rock spin..."
		fdScriptSetMessageId((Sint32)MsgGarohRoomsIfIUsedALever);
	else
		// "That rock was huge. How was I supposed to make it spin?"
		fdScriptSetMessageId((Sint32)MsgGarohRoomsThatRockWasHugeHow);
	fdScriptShowMessageAndWait(ACTOR_STUBBORN_TRAVELER, 0);
	fdScriptEnd();
}

// Talking by day to the old man with a cane in the old couple's home, the first time: "You... Is it
// true that you learned Reveal?", with an answer for Yes and one for No.
void scGarohRoomsAskAboutReveal(void)
{
	Sint32 msg = (Sint32)MsgGarohRoomsYouIsItTrueThat;

	// "You... Is it true that you learned Reveal?"
	fdScriptSetMessageId(msg);
	fdMessageShowAtActor(ACTOR_REVEAL_SEEKER, 0);
	if (fdScriptAskYesNoWithActorAnimation(fdPartyGetLeaderActor(), 0) == 0)
	{
		fdScriptWait(10);
		fdScriptSetMessageId(msg + 1);
	}
	else
	{
		fdScriptWait(20);
		fdScriptSetMessageId(msg + 2);
	}
	fdScriptShowMessageAndWait(ACTOR_REVEAL_SEEKER, 0);
	syGameFlagSet(FLAG_GAROH_ASKED_ABOUT_REVEAL);
}

// Returns the room's cues: the night's until the secret is shared, the morning's after, and the
// inn's morning ones as the party wakes there.
const CUE *scGetCues(void)
{
	Sint32              shared = syGameFlagGet(FLAG_GAROH_SECRET_SHARED);
	MAIN_ROUTINE_STATE *party  = &MainRoutineStateWork;
	Sint32              houses = SCENE_GAROH_ROOMS_HOUSES;
	Sint32              scene;

	if (shared)
	{
		scene = party->next;
		if (scene == houses)
			return GarohRoomsHousesMorningCues;
		if (scene == SCENE_GAROH_ROOMS_SHOP)
			return GarohRoomsShopMorningCues;
	}
	else
	{
		scene = party->next;
		if (scene == houses)
		{
			if (party->entrance == ENTRANCE_WAKE_AT_INN)
				return GarohRoomsHousesMorningCues;
			return GarohRoomsHousesNightCues;
		}
		if (scene == SCENE_GAROH_ROOMS_SHOP)
			return GarohRoomsShopNightCues;
	}
	if (scene == SCENE_GAROH_ROOMS_SANCTUM)
		return GarohRoomsSanctumCues;
	return GarohRoomsDoorCues;
}

// The map layers the rooms show at night (fdMapGridSetShownLayers), which leave out the light
// falling through the windows.
#define GAROH_ROOMS_NIGHT_LAYERS (DISPCNT_BG1_ON | DISPCNT_BG3_ON)

// Sets up the room being entered: the travelers in their beds at night, or up once the secret is
// shared; waking at the inn, and night's layers.
Sint32 scInit(void)
{
	Sint32 turn;

	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
	fdActorGetById(MainRoutineStateWork.leaderActorId)->priorityFlags |=
		ACTOR_PRIORITY_IGNORES_RISES;
	fdEffectStartBlankOverlay(0);
	if (MainRoutineStateWork.next == SCENE_GAROH_ROOMS_HOUSES)
	{
		if (syGameFlagGet(FLAG_GAROH_SECRET_SHARED)
		    || MainRoutineStateWork.entrance == ENTRANCE_WAKE_AT_INN)
		{
			fdActorSetPos(ACTOR_DISAPPOINTED_TRAVELERS_BED, 0, 0);
			fdActorSetPos(ACTOR_STUBBORN_TRAVELERS_BED, 0, 0);
			fdActorSetMovementTypeById(ACTOR_STUBBORN_TRAVELER, MOVEMENT_TYPE_WANDER_NEAR);
		}
		else
		{
			turn = FACING_SOUTHEAST;
			fdActorSetPosAndFacing(ACTOR_DISAPPOINTED_TRAVELER, toFIXED(306), toFIXED(335), turn);
			{
				Sint32 angle = FACING_SOUTHWEST;
				FIXED  z     = toFIXED(336);

				fdActorSetHorizontalPosAndAngleWithTerrain(
					ACTOR_STUBBORN_TRAVELER, toFIXED(345), z, angle);
			}
			fdActorSetSpriteFlags(fdActorGetById(ACTOR_DISAPPOINTED_TRAVELERS_BED), 0);
			fdActorSetSpriteFlags(fdActorGetById(ACTOR_STUBBORN_TRAVELERS_BED), 0);
			fdActorGetById(ACTOR_DISAPPOINTED_TRAVELER)->motionFlags      = 0;
			fdActorGetById(ACTOR_DISAPPOINTED_TRAVELER)->y.fixed          = 0;
			fdActorGetById(ACTOR_STUBBORN_TRAVELER)->motionFlags          = 0;
			fdActorGetById(ACTOR_STUBBORN_TRAVELER)->y.fixed              = 0;
			fdActorGetById(ACTOR_DISAPPOINTED_TRAVELERS_BED)->motionFlags = 0;
			fdActorGetById(ACTOR_DISAPPOINTED_TRAVELERS_BED)->y.fixed     = 0;
			fdActorGetById(ACTOR_STUBBORN_TRAVELERS_BED)->motionFlags     = 0;
			fdActorGetById(ACTOR_STUBBORN_TRAVELERS_BED)->y.fixed         = 0;
			fdActorSetSpriteRotationById(ACTOR_DISAPPOINTED_TRAVELER, DEGtoANG(315));
			fdActorSetSpriteFlags(fdActorGetById(ACTOR_DISAPPOINTED_TRAVELER), 0);
			fdActorSetSpriteRotationById(ACTOR_STUBBORN_TRAVELER, turn);
			fdActorSetSpriteFlags(fdActorGetById(ACTOR_STUBBORN_TRAVELER), 0);
		}
		if (MainRoutineStateWork.entrance == ENTRANCE_WAKE_AT_INN
		    && !syGameFlagGet(FLAG_GAROH_SECRET_SHARED))
			scGarohRoomsWakeAtInn();
	}
	if (syGameFlagGet(FLAG_GAROH_SECRET_SHARED))
		syGameFlagClear(FLAG_GAROH_NIGHT);
	if (syGameFlagGet(FLAG_GAROH_NIGHT))
		fdMapGridSetShownLayers(GAROH_ROOMS_NIGHT_LAYERS);
	return 0;
}

Sint32 scPrepareMap(void)
{
	return 0;
}

// The morning after the night with Maha, at the inn: "Felix and party stayed the night at the
// inn.", the woman at the counter says they did not sleep much, and the innkeeper says Master Maha
// told them all about the party, the bill is on them, and bids them farewell.
void scGarohRoomsWakeAtInn(void)
{
	syGameFlagSet(FLAG_GAROH_SECRET_SHARED);
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdScriptWait(30);
	fdActorSystemSuspendProjectionAndShowSprites();
	sySoundPlay(MUS_OVERNIGHT_AT_THE_INN);
	// "Felix and party stayed the night at the inn."
	mnFieldMessageShowCentered((Sint32)MsgGarohRoomsAndPartyStayedTheNight, 0, 0);
	fdScriptWait(20);
	fdActorSystemResumeProjectionAndHideLayers();
	fdMapGridCenterOnFollowedPoint();
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	// "You folks didn't sleep much last night..."
	fdScriptSetMessageId((Sint32)MsgGarohRoomsYouFolksDidntSleepMuch);
	// "You folks didn't sleep much last night... You don't look like you..."
	fdScriptShowMessageThenWait(ACTOR_INN_WORKER, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_INNKEEPER, 2);
	fdScriptWait(20);
	// "Master Maha told us all about you. Don't you worry about the bill. It's on us."
	fdScriptShowMessageThenWait(ACTOR_INNKEEPER, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_INNKEEPER, ANIM_NOD);
	fdScriptWait(30);
	// "Farewell, friends."
	fdScriptShowMessageThenWait(ACTOR_INNKEEPER, 0, 10);
	fdScriptEnd();
}

#include "shared/scene_entrances_default.c"

Uint32 SceneExits[] = {
	SCENE_EXITS(GarohRoomsHouses),
	SCENE_EXIT_UNLESS(GAROH_ROOMS_HOUSES_TO_GAROH_SOUTH,
                      Garoh,
                      GAROH_FROM_GAROH_ROOMS_HOUSES_SOUTH,
                      EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(GAROH_ROOMS_HOUSES_TO_GAROH_NORTHWEST,
                      Garoh,
                      GAROH_FROM_GAROH_ROOMS_HOUSES_NORTHWEST,
                      EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(GAROH_ROOMS_HOUSES_TO_GAROH_SOUTHWEST,
                      Garoh,
                      GAROH_FROM_GAROH_ROOMS_HOUSES_SOUTHWEST,
                      EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(GAROH_ROOMS_HOUSES_TO_GAROH_SOUTHEAST,
                      Garoh,
                      GAROH_FROM_GAROH_ROOMS_HOUSES_SOUTHEAST,
                      EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(GAROH_ROOMS_HOUSES_TO_GAROH_TUNNELS_EAST,
                      GarohTunnels,
                      GAROH_TUNNELS_FROM_GAROH_ROOMS_HOUSES_EAST,
                      EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(GAROH_ROOMS_HOUSES_TO_GAROH_TUNNELS_WEST,
                      GarohTunnels,
                      GAROH_TUNNELS_FROM_GAROH_ROOMS_HOUSES_WEST,
                      EXIT_FLAG_NONE),
	SCENE_EXITS(GarohRoomsShop),
	SCENE_EXIT_UNLESS(GAROH_ROOMS_SHOP_TO_GAROH_TUNNELS_WEST,
                      GarohTunnels,
                      GAROH_TUNNELS_FROM_GAROH_ROOMS_SHOP_WEST,
                      EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(
		GAROH_ROOMS_SHOP_TO_GAROH, Garoh, GAROH_FROM_GAROH_ROOMS_SHOP, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(GAROH_ROOMS_SHOP_TO_GAROH_TUNNELS_EAST,
                      GarohTunnels,
                      GAROH_TUNNELS_FROM_GAROH_ROOMS_SHOP_EAST,
                      EXIT_FLAG_NONE),
	SCENE_EXITS(GarohRoomsSanctum),
	SCENE_EXIT_UNLESS(
		GAROH_ROOMS_SANCTUM_TO_GAROH, Garoh, GAROH_FROM_GAROH_ROOMS_SANCTUM, EXIT_FLAG_NONE),
	SCENE_EXITS_END,
};

// clang-format off
ACTOR_PLACE GarohRoomsNoActorPlacements[] = {
	{SCENE_TABLE_END},
};

ACTOR_PLACE GarohRoomsHousesActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 336, 0, 120, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 104, FACING_WEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 136, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 568, 0, 136, FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 592, 0, 96, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_BALD_MAN_IN_RED, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 134, 0, 120, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_WITH_ORANGE_BEARD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 112, 0, 120, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 424, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PINK_APRON, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 420, FACING_WEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 344, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 330, 0, 376, FACING_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 336, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 352, 0, 344, FACING_SOUTHWEST, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

ACTOR_PLACE GarohRoomsShopActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 372, 0, 406, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 424, 0, 424, FACING_NORTHWEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 352, 0, 456, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_PONYTAIL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 412, FACING_SOUTH, TALK_FACING_PARTY, 0),
	{SCENE_TABLE_END},
};

ACTOR_PLACE GarohRoomsSanctumActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 448, 0, 152, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	{SCENE_TABLE_END},
};

CUE GarohRoomsDoorCues[] = {
	{CUE_EXIT, 1, CONDITION_ALWAYS, 1},
	{CUE_EXIT, 2, CONDITION_ALWAYS, 2},
	{CUE_TABLE_END},
};
// clang-format on

CUE GarohRoomsHousesNightCues[] = {
	{CUE_EXIT, 1, CONDITION_ALWAYS, 1},
	{CUE_EXIT, 2, CONDITION_ALWAYS, 2},
	{CUE_EXIT, 3, CONDITION_ALWAYS, 3},
	{CUE_EXIT, 4, CONDITION_ALWAYS, 4},
	{CUE_EXIT, 5, CONDITION_ALWAYS, 5},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 6, CONDITION_ALWAYS, 6},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 7, CONDITION_ALWAYS, 7},
	{CUE_TALK,
     ACTOR_PUPS_FATHER, CONDITION_ALWAYS,
     SCRIPT_REF(scGarohRoomsAskWhatTheWanderersSeek)},
	{CUE_TALK, ACTOR_PUPS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgGarohRoomsOurOldestSonIsAlways)},
	{CUE_TALK, ACTOR_PUPS_BROTHER, CONDITION_ALWAYS, MSGNO(MsgGarohRoomsNoFairMyBrotherAlways)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_PUPS_FATHER, CONDITION_ALWAYS,
     MSGNO(MsgGarohRoomsOutsidersCouldNeverUnderstandThe)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_PUPS_MOTHER, CONDITION_ALWAYS,
     MSGNO(MsgGarohRoomsThatPupOfMineJust)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_PUPS_BROTHER, CONDITION_ALWAYS,
     MSGNO(MsgGarohRoomsWhatMadeMasterMahaTake)},
	{CUE_TALK, ACTOR_REVEAL_SEEKER, CONDITION_ALWAYS, MSGNO(MsgGarohRoomsWhenOurPeopleGrowOlder)},
	{CUE_TALK, ACTOR_REVEAL_SEEKERS_MATE, CONDITION_ALWAYS, MSGNO(MsgGarohRoomsMyMateIsCloseTo)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_REVEAL_SEEKER, CONDITION_ALWAYS,
     MSGNO(MsgGarohRoomsICastWhirlwindOnThe)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_REVEAL_SEEKERS_MATE, CONDITION_ALWAYS,
     MSGNO(MsgGarohRooms16)},
	{CUE_TALK, ACTOR_WEAPONS_VENDOR, CONDITION_ALWAYS, SCRIPT_REF(scGarohRoomsTalkToWeaponsVendor)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_WEAPONS_VENDOR, CONDITION_ALWAYS,
     MSGNO(MsgGarohRoomsIShouldBeOutsideHowling)},
	{CUE_TALK, ACTOR_ARMOR_VENDOR, CONDITION_ALWAYS, SCRIPT_REF(scGarohRoomsTalkToArmorVendor)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_ARMOR_VENDOR, CONDITION_ALWAYS,
     MSGNO(MsgGarohRoomsTheseStrangersSeemNormalEnough)},
	{CUE_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, SCRIPT_REF(scGarohRoomsTalkToInnkeeper)},
	{CUE_TALK, ACTOR_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgGarohRoomsAirsRockCanGetQuite)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_INNKEEPER, CONDITION_ALWAYS,
     MSGNO(MsgGarohRoomsItOnlyRainsAFew)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_INN_WORKER, CONDITION_ALWAYS,
     MSGNO(MsgGarohRoomsMostPeopleSeeNothingOf)},
	{CUE_TALK,
     ACTOR_DISAPPOINTED_TRAVELERS_BED, CONDITION_ALWAYS,
     SCRIPT_REF(scGarohRoomsTalkToDisappointedTraveler)},
	{CUE_TALK,
     ACTOR_STUBBORN_TRAVELERS_BED, CONDITION_ALWAYS,
     SCRIPT_REF(scGarohRoomsTalkToStubbornTraveler)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_DISAPPOINTED_TRAVELERS_BED, CONDITION_ALWAYS,
     SCRIPT_REF(scGarohRoomsReadDisappointedTravelersMind)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_STUBBORN_TRAVELERS_BED, CONDITION_ALWAYS,
     SCRIPT_REF(scGarohRoomsReadStubbornTravelersMind)},
	{EXAMINE_CUE(EXAMINE_OVEN),
     METATILE_BEHAVIOR_LARVAE_STOVE, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgGarohRoomsEeeeewTheyreFryingUpBug))},
	{EXAMINE_CUE(EXAMINE_OVEN),
     METATILE_BEHAVIOR_LAMB_STOVE, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgGarohRoomsItsLambOnTheBone))},
	{EXAMINE_CUE(EXAMINE_OVEN),
     METATILE_BEHAVIOR_NEWT_STOVE, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgGarohRoomsNewtsSkeweredOverAnOpen))},
	{EXAMINE_CUE(EXAMINE_OVEN),
     METATILE_BEHAVIOR_PLANT_POT_STOVE, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgGarohRoomsThereAreLotsOfStrange))},
	{CUE_TABLE_END},
};

CUE GarohRoomsShopNightCues[] = {
	{CUE_EXIT, 1, CONDITION_ALWAYS, 1},
	{CUE_EXIT, 2, CONDITION_ALWAYS, 2},
	{CUE_EXIT, 3, CONDITION_ALWAYS, 3},
	{CUE_EXIT, 4, CONDITION_ALWAYS, 4},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 5, CONDITION_ALWAYS, 5},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 6, CONDITION_ALWAYS, 6},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 7, CONDITION_ALWAYS, 7},
	{CUE_TALK,
     ACTOR_AIRS_ROCK_GUARDIAN, CONDITION_ALWAYS,
     MSGNO(MsgGarohRoomsGarohExistsToProtectAirs)},
	{CUE_TALK,
     ACTOR_VILLAGER_ON_WHIRLWIND, CONDITION_ALWAYS,
     MSGNO(MsgGarohRoomsSoYouveHeardOfWhirlwind)},
	{CUE_TALK,
     ACTOR_VILLAGER_ON_THE_WIND_STONE, CONDITION_ALWAYS,
     MSGNO(MsgGarohRoomsIveHeardAboutTheWind)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_AIRS_ROCK_GUARDIAN, CONDITION_ALWAYS,
     MSGNO(MsgGarohRoomsWeHaveOtherPowersThan)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_VILLAGER_ON_WHIRLWIND, CONDITION_ALWAYS,
     MSGNO(MsgGarohRoomsAndDifferentPeopleUseWhirlwind)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_VILLAGER_ON_THE_WIND_STONE, CONDITION_ALWAYS,
     MSGNO(MsgGarohRoomsIWishIKnewWhat)},
	{CUE_TALK,
     ACTOR_ITEM_SHOPKEEPER, CONDITION_ALWAYS,
     SCRIPT_REF(scGarohRoomsTalkToItemShopkeeper)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_ITEM_SHOPKEEPER, CONDITION_ALWAYS,
     MSGNO(MsgGarohRoomsINeverChangeMyStock)},
	{EXAMINE_CUE(EXAMINE_OVEN),
     METATILE_BEHAVIOR_LOBSTER_STOVE, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgGarohRoomsHuhWhatIsThatLobster))},
	{CUE_TABLE_END},
};

CUE GarohRoomsSanctumCues[] = {
	{CUE_EXIT, 2, CONDITION_ALWAYS, 2},
	{CUE_TALK, ACTOR_PRIEST, CONDITION_ALWAYS, SCRIPT_REF(scGarohRoomsTalkToPriest)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_PRIEST, FLAG_GAROH_SECRET_SHARED,
     MSGNO(MsgGarohRooms32)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_PRIEST, CONDITION_ALWAYS,
     MSGNO(MsgGarohRoomsAmITheOnlyOne)},
	// The same rows again for an actor 9, which the sanctum does not place.
	{CUE_TALK, 9, CONDITION_ALWAYS, SCRIPT_REF(scGarohRoomsTalkToPriest)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, FLAG_GAROH_SECRET_SHARED, MSGNO(MsgGarohRooms32)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgGarohRoomsAmITheOnlyOne)},
	{CUE_TABLE_END},
};

CUE GarohRoomsHousesMorningCues[] = {
	{CUE_EXIT, 1, CONDITION_ALWAYS, 1},
	{CUE_EXIT, 2, CONDITION_ALWAYS, 2},
	{CUE_EXIT, 3, CONDITION_ALWAYS, 3},
	{CUE_EXIT, 4, CONDITION_ALWAYS, 4},
	{CUE_EXIT, 5, CONDITION_ALWAYS, 5},
	{CUE_EXIT, 6, CONDITION_ALWAYS, 6},
	{CUE_EXIT, 7, CONDITION_ALWAYS, 7},
	{CUE_TALK, ACTOR_PUPS_FATHER, CONDITION_ALWAYS, MSGNO(MsgGarohRoomsHeyItsYouThanksFor)},
	{CUE_TALK, ACTOR_PUPS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgGarohRoomsEveryoneIsAmazedThatYou)},
	{CUE_TALK, ACTOR_PUPS_BROTHER, CONDITION_ALWAYS, MSGNO(MsgGarohRoomsMyBrotherShouldHaveGotten)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_PUPS_FATHER, CONDITION_ALWAYS,
     MSGNO(MsgGarohRoomsTheYoungerTheWerewolfThe)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_PUPS_MOTHER, CONDITION_ALWAYS,
     MSGNO(MsgGarohRoomsOhThankGoodnessEverythingWorked)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_PUPS_BROTHER, CONDITION_ALWAYS,
     MSGNO(MsgGarohRoomsAndDadSaidHeWas)},
	{CUE_TALK,
     ACTOR_REVEAL_SEEKER, FLAG_GAROH_ASKED_ABOUT_REVEAL,
     SCRIPT_REF(scGarohRoomsAskAboutReveal)},
	{CUE_TALK, ACTOR_REVEAL_SEEKER, CONDITION_ALWAYS, MSGNO(MsgGarohRoomsToThinkIveWastedSo)},
	{CUE_TALK, ACTOR_REVEAL_SEEKERS_MATE, CONDITION_ALWAYS, MSGNO(MsgGarohRoomsHesWorkedSoHardBut)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_REVEAL_SEEKER, CONDITION_ALWAYS,
     MSGNO(MsgGarohRoomsIThoughtICouldBecome)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_REVEAL_SEEKERS_MATE, CONDITION_ALWAYS,
     MSGNO(MsgGarohRoomsSoNowEvenOutsidersAre)},
	{CUE_TALK, ACTOR_WEAPONS_VENDOR, CONDITION_ALWAYS, SCRIPT_REF(scGarohRoomsTalkToWeaponsVendor)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_WEAPONS_VENDOR, CONDITION_ALWAYS,
     MSGNO(MsgGarohRoomsIPreferTheToothAnd)},
	{CUE_TALK, ACTOR_ARMOR_VENDOR, CONDITION_ALWAYS, SCRIPT_REF(scGarohRoomsTalkToArmorVendor)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_ARMOR_VENDOR, CONDITION_ALWAYS,
     MSGNO(MsgGarohRoomsMahaSaysThatWhenWere)},
	{CUE_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, SCRIPT_REF(scGarohRoomsTalkToInnkeeper)},
	{CUE_TALK, ACTOR_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgGarohRoomsMahaCameAlongAndSaid)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_INNKEEPER, CONDITION_ALWAYS,
     MSGNO(MsgGarohRoomsIveHeardStoriesAboutOtherPlaces)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_INN_WORKER, CONDITION_ALWAYS,
     MSGNO(MsgGarohRoomsHeSaidIfTheyWere)},
	{CUE_TALK,
     ACTOR_DISAPPOINTED_TRAVELER, CONDITION_ALWAYS,
     SCRIPT_REF(scGarohRoomsTalkToDisappointedTraveler)},
	{CUE_TALK,
     ACTOR_STUBBORN_TRAVELER, CONDITION_ALWAYS,
     SCRIPT_REF(scGarohRoomsTalkToStubbornTraveler)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_DISAPPOINTED_TRAVELER, CONDITION_ALWAYS,
     SCRIPT_REF(scGarohRoomsReadDisappointedTravelersMind)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_STUBBORN_TRAVELER, CONDITION_ALWAYS,
     SCRIPT_REF(scGarohRoomsReadStubbornTravelersMind)},
	{EXAMINE_CUE(EXAMINE_OVEN),
     METATILE_BEHAVIOR_LARVAE_STOVE, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgGarohRoomsEeeeewTheyreFryingUpBug))},
	{EXAMINE_CUE(EXAMINE_OVEN),
     METATILE_BEHAVIOR_LAMB_STOVE, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgGarohRoomsItsLambOnTheBone))},
	{EXAMINE_CUE(EXAMINE_OVEN),
     METATILE_BEHAVIOR_NEWT_STOVE, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgGarohRoomsNewtsSkeweredOverAnOpen))},
	{EXAMINE_CUE(EXAMINE_OVEN),
     METATILE_BEHAVIOR_PLANT_POT_STOVE, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgGarohRoomsThereAreLotsOfStrange))},
	{CUE_TABLE_END},
};

CUE GarohRoomsShopMorningCues[] = {
	{CUE_EXIT, 1, CONDITION_ALWAYS, 1},
	{CUE_EXIT, 2, CONDITION_ALWAYS, 2},
	{CUE_EXIT, 3, CONDITION_ALWAYS, 3},
	{CUE_EXIT, 4, CONDITION_ALWAYS, 4},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 5, CONDITION_ALWAYS, 5},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 6, CONDITION_ALWAYS, 6},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 7, CONDITION_ALWAYS, 7},
	{CUE_TALK,
     ACTOR_AIRS_ROCK_GUARDIAN, CONDITION_ALWAYS,
     MSGNO(MsgGarohRoomsSoYoureSomethingSpecialAn)},
	{CUE_TALK,
     ACTOR_VILLAGER_ON_WHIRLWIND, CONDITION_ALWAYS,
     MSGNO(MsgGarohRoomsYouLearnedRevealAtAirs)},
	{CUE_TALK,
     ACTOR_VILLAGER_ON_THE_WIND_STONE, CONDITION_ALWAYS,
     MSGNO(MsgGarohRoomsIJustCantBelieveThat)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_AIRS_ROCK_GUARDIAN, CONDITION_ALWAYS,
     MSGNO(MsgGarohRoomsDidMahaMeanThatThey)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_VILLAGER_ON_WHIRLWIND, CONDITION_ALWAYS,
     MSGNO(MsgGarohRoomsSoThesePeopleCanUse)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_VILLAGER_ON_THE_WIND_STONE, CONDITION_ALWAYS,
     MSGNO(MsgGarohRoomsNormallyOutsidersNeverGetIn)},
	{CUE_TALK,
     ACTOR_ITEM_SHOPKEEPER, CONDITION_ALWAYS,
     SCRIPT_REF(scGarohRoomsTalkToItemShopkeeper)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_ITEM_SHOPKEEPER, CONDITION_ALWAYS,
     MSGNO(MsgGarohRoomsIHeardThatOneThats)},
	{EXAMINE_CUE(EXAMINE_OVEN),
     METATILE_BEHAVIOR_LOBSTER_STOVE, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgGarohRoomsHuhWhatIsThatLobster))},
	{CUE_TABLE_END},
};
