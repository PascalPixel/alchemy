// Daila's rooms, in two scenes that share this overlay: the houses (DailaRoomsHouses) and the
// sanctuary (DailaRoomsSanctum).
//
// The houses are six rooms, one behind each door of the village: Riki's home, Tavi's home, the
// middle house, the mayor's house, the inn and the weapon and armor shop. The same scene also holds
// the house on the islet with the cow, reached from North Osenia. Once Tavi is rescued
// (FLAG_TAVI_RESCUED) the people have other things to say, the two boys are home eating, and a
// woman at the inn is caught practicing for the dinner show.
//
// The sanctuary, up the steps at the north of the village, has a healer at his counter, and behind
// him the Sea God's statue with the Sea God's Tear at its foot, beyond a pool of water. Once Tavi
// is rescued the water has drained to three puddles (scPrepareMap). Felix can hop over them, and
// Frost raises each into a pillar of ice; with the two on the left frozen he can reach the Tear and
// take it (scDailaRoomsTakeSeaGodsTear).

#include "field/script.h"
#include "game/game.h"
#include "item_ids.h"
#include "field/actor_script.h"
#include "field/field_mode_script.h"
#include "field/field_mode.h"
#include "scene/scene.h"
#include "scene/ground_effects.h"
#include "window/window.h"
#include "shop/shop.h"
#include "edition.h"
#include "text/msg_ids.h"

// The metatiles beside each of the sanctuary's puddles once it has drained, named by the side they
// lie on: walking from one into the puddle hops over it.
typedef enum DAILA_ROOMS_METATILE
{
	METATILE_NORTH_OF_PUDDLE = 30,
	METATILE_WEST_OF_PUDDLE,
	METATILE_EAST_OF_PUDDLE,
	METATILE_SOUTH_OF_PUDDLE,
	// The foot of the statue where the Sea God's Tear lies, metatile (14, 12).
	METATILE_SEA_GODS_TEAR = 40
} DAILA_ROOMS_METATILE;

// The houses, shops and inn of DailaRoomsHouses, numbering the rows its two placement tables share.
typedef enum DAILA_ROOMS_ACTOR_ID
{
	// Riki's parents, in his home at the west of the village, scolding the boy who took his
	// father's fishing pole.
	ACTOR_RIKIS_FATHER = ACTOR_FIRST_PLACED,
	ACTOR_RIKIS_MOTHER,
	// Tavi's parents, next door, waiting for him with his three o'clock snack.
	ACTOR_TAVIS_FATHER,
	ACTOR_TAVIS_MOTHER,
	// An old man in the middle house who sends boat seekers to Madra.
	ACTOR_OLD_MAN_ON_MADRA,
	// An old woman in the middle house who tells the way to the Kandorean Temple.
	ACTOR_OLD_WOMAN_ON_THE_KANDOREAN_TEMPLE,
	// "In all my years as mayor, I've never seen a wave like that before..."
	ACTOR_MAYOR,
	// An older woman in the mayor's house, shaken by the noise after the wave.
	ACTOR_WOMAN_ON_THE_NOISE,
	// A man in the mayor's house who heard the rumbling from the southeast.
	ACTOR_MAN_ON_THE_RUMBLING,
	// A woman in the mayor's house on Daila's bad luck with pirates and waves.
	ACTOR_WOMAN_ON_DAILAS_LUCK,
	ACTOR_WEAPON_MERCHANT,
	ACTOR_ARMOR_MERCHANT,
	ACTOR_INNKEEPER,
	// The innkeeper's one helper, who later practices for the dinner show.
	ACTOR_INN_WORKER,
	// A guest of the inn, a man carrying a pack, who misses Madra's food.
	ACTOR_TRAVELER_FROM_MADRA,
	// A guest of the inn who longs for Madra's seafood.
	ACTOR_GUEST_ON_MADRAS_SEAFOOD,
	// Until Tavi is rescued: the family in the house on the islet with the cow, whose sons try the
	// Sea of Time.
	ACTOR_WOMAN_ON_THE_CURRENTS,
	ACTOR_OLD_MAN_ON_THE_RED_ROCKS,
	ACTOR_OLD_WOMAN_ON_HER_SONS,
	// An unseen actor on the armor shop's counter, so that the merchant behind it answers Felix
	// from across it.
	ACTOR_ARMOR_COUNTER
} DAILA_ROOMS_ACTOR_ID;

// The rows of DailaRoomsHousesPlacementsOnceTaviIsRescued after those it shares with
// DailaRoomsHousesPlacements.
typedef enum DAILA_ROOMS_ACTOR_ID_AFTER_RESCUE
{
	// The two boys, back home, with their mouths full.
	ACTOR_RIKI = ACTOR_GUEST_ON_MADRAS_SEAFOOD + 1,
	ACTOR_TAVI,
	// A woman of the inn caught practicing for the dinner show.
	ACTOR_PRACTICING_PERFORMER,
	ACTOR_WOMAN_ON_THE_CURRENTS_AFTER_RESCUE,
	ACTOR_OLD_MAN_ON_THE_RED_ROCKS_AFTER_RESCUE,
	ACTOR_OLD_WOMAN_ON_HER_SONS_AFTER_RESCUE,
	ACTOR_ARMOR_COUNTER_AFTER_RESCUE
} DAILA_ROOMS_ACTOR_ID_AFTER_RESCUE;

// Riki's and Tavi's own animation 5: eating at the table at home, their mouths full.
enum
{
	ANIM_EATING = 5
};

// The way into the house on the islet with the cow (NorthOseniaIslet), which this overlay also
// holds: the family by entrance 7.
enum
{
	ENTRANCE_COW_ISLET_HOUSE = 7
};

// The sanctuary's actors, numbering the rows of DailaRoomsSanctumPlacements.
typedef enum DAILA_SANCTUARY_ACTOR_ID
{
	// The sanctuary's healer, who sends travelers with nowhere to go to Poi at the Kandorean
	// Temple, and once that is done heals the party.
	ACTOR_HEALER = ACTOR_FIRST_PLACED,
	// The Sea God's Tear at the statue's foot: an actor with no picture of its own, on which the
	// game draws the item (fdActorSetItemIcon) until it is taken.
	ACTOR_SEA_GODS_TEAR,
	// The three puddles on the drained floor, from west to east, which Frost raises into pillars of
	// ice.
	ACTOR_FIRST_PUDDLE,
	ACTOR_SECOND_PUDDLE,
	ACTOR_THIRD_PUDDLE,
	// An actor with no picture, put where the Tear lay once it is taken.
	ACTOR_TEAR_SPOT
} DAILA_SANCTUARY_ACTOR_ID;

// The first of three flags, one for each of the sanctuary's puddles Frost has frozen into a pillar
// for this visit, by DailaRoomsPillarFrozenFlags.
#define FLAG_DAILA_SANCTUARY_PUDDLE_FROZEN_FIRST 0x220
// Holds which of his two spots the practicing man has jumped to last.
#define FLAG_DAILA_PRACTICE_AT_DOOR              0x200

extern Sint32         DailaRoomsExitsOnceTaviIsRescued[];
extern ACTOR_PLACE    DailaRoomsHousesPlacements[];
extern ACTOR_PLACE    DailaRoomsSanctumPlacements[];
extern ACTOR_PLACE    DailaRoomsHousesPlacementsOnceTaviIsRescued[];
extern CUE            DailaRoomsSanctumCues[];
extern CUE            DailaRoomsHousesCuesOnceTaviIsRescued[];
extern CUE            DailaRoomsSanctumCuesOnceTaviIsRescued[];

extern Sint16         DailaRoomsPillarFrozenFlags[];

Sint32                scDailaRoomsGiveItem(Sint32 item, Sint32 mode);
void                  scFadeInAndClearFlag(void);

const SCENE_ENTRANCE *scGetEntrances(void)
{
	return SceneEntrances;
}

const SCENE_REGION *scGetExitRegions(void)
{
	return 0;
}

extern Sint32 DailaRoomsExits[];

const void   *scGetExits(void)
{
	if (syGameFlagGet(FLAG_TAVI_RESCUED))
		return DailaRoomsExitsOnceTaviIsRescued;
	return DailaRoomsExits;
}

const ACTOR_PLACE *scGetActorPlacements(void)
{
	if (MainRoutineStateWork.next == SCENE_DAILA_ROOMS_SANCTUM)
		return DailaRoomsSanctumPlacements;
	if (syGameFlagGet(FLAG_TAVI_RESCUED))
		return DailaRoomsHousesPlacementsOnceTaviIsRescued;
	return DailaRoomsHousesPlacements;
}

// Unused: does nothing.
UNUSED void scDailaRoomsNoOperationHook(void)
{
}

// Unused: the practicing man shouts and jumps to the other of his two spots.
UNUSED void scDailaRoomsPracticeShout(Sint32 unused, Sint32 actor)
{
	scFieldActorShowEmote(actor, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 50);
	fdActorJump(actor, 6, 15);
	fdActorJump(actor, 6, 15);
	syFieldScriptWait(10);
	if (!fdScriptGetFlag(FLAG_DAILA_PRACTICE_AT_DOOR))
	{
		fdScriptSetMessageId((Sint32)MsgDailaRoomsGah);
		// "GAH!"
		fdScriptShowMessageAndWait(actor, 0);
		fdActorWalkToAndWait(actor, 360, 72);
		fdActorFaceDirectionById(actor, FACING_SOUTH, 0);
		syGameFlagSet(FLAG_DAILA_PRACTICE_AT_DOOR);
	}
	else
	{
		fdScriptSetMessageId((Sint32)MsgDailaRoomsYarg);
		// "YARG!"
		fdScriptShowMessageAndWait(actor, 0);
		fdActorWalkToAndWait(actor, 392, 104);
		fdActorFaceDirectionById(actor, FACING_WEST, 0);
		syGameFlagClear(FLAG_DAILA_PRACTICE_AT_DOOR);
	}
	syFieldScriptWait(20);
}

// Unused: the practicing man turns to the leader, startled.
UNUSED void scDailaRoomsStartleAtTheLeader(Sint32 unused, Sint32 actor)
{
	fdActorFaceActorById(actor, ACTOR_FELIX, 0);
	syFieldScriptWait(10);
	scFieldActorShowEmote(actor, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 50);
	fdActorJump(actor, 6, 15);
	fdActorJump(actor, 6, 15);
	syFieldScriptWait(10);
	fdScriptSetMessageId((Sint32)MsgDailaRoomsWhatAreYouDoing);
	// "What are you doing???"
	fdScriptShowMessageAndWait(actor, 0);
	syFieldScriptWait(10);
}

// Mind Read on the armor merchant behind the left-hand counter of the shop, before Tavi is rescued:
// he thinks of the two scamps.
Sint32 scDailaRoomsReadArmorMerchantsMind(void)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdScriptSetMessageId((Sint32)MsgDailaRoomsScamps);
	// "If those two scamps knew there were travelers in town, they'd have a..."
	fdScriptShowMessageAndWait(ACTOR_ARMOR_MERCHANT, 0);
	fdScriptEnd();
	return 0;
}

// Mind Read on the armor merchant once Tavi is rescued: he thinks of hidden treasure.
Sint32 scDailaRoomsReadArmorMerchantsMindAfterRescue(void)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdScriptSetMessageId((Sint32)MsgDailaRoomsMazeTreasure);
	// "Maybe they hid treasure here because this town is so mazelike..."
	fdScriptShowMessageAndWait(ACTOR_ARMOR_MERCHANT, 0);
	fdScriptEnd();
	return 0;
}

// Unused: starts a script and draws the leader in front of the counters.
UNUSED void scDailaRoomsStartLeaderScript(void)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdActorSetSpritePriority(MainRoutineStateWork.leaderActorId, 1);
}

extern CUE DailaRoomsHousesCues[];

const CUE *scGetCues(void)
{
	Sint32              taviRescued = syGameFlagGet(FLAG_TAVI_RESCUED);
	MAIN_ROUTINE_STATE *state       = &MainRoutineStateWork;
	Sint32              sanctum     = SCENE_DAILA_ROOMS_SANCTUM;

	if (taviRescued)
	{
		if (state->next == sanctum)
			return DailaRoomsSanctumCuesOnceTaviIsRescued;
		return DailaRoomsHousesCuesOnceTaviIsRescued;
	}
	if (state->next == sanctum)
		return DailaRoomsSanctumCues;
	return DailaRoomsHousesCues;
}

// The weapon merchant at the right-hand counter of the shop sells to Felix when he talks to him
// from in front of it, facing north; from any other side he warns of the roads south.
void scDailaRoomsTalkToWeaponMerchant(Sint32 actor)
{
	Uint16 facing = fdActorGetById(MainRoutineStateWork.leaderActorId)->facing;

	if (FACING_NEAREST_CARDINAL(facing) == FACING_NORTH)
	{
		mnShopRun(SHOP_DAILA_WEAPONS, actor);
	}
	else
	{
		fdScriptStart();
		fdScriptEndLastingPsynergy(0);
		fdScriptSetMessageId((Sint32)MsgDailaRoomsSouthRoadDanger);
		// "You folks ain't thinkin' 'bout goin' south, are ya? The roads to the..."
		fdScriptShowMessageAndWait(actor, 0);
		fdScriptEnd();
	}
}

// The armor merchant at the left-hand counter of the shop sells to Felix when he talks to him from
// in front of it, facing north; from any other side he asks what brings the party to Daila.
void scDailaRoomsTalkToArmorMerchant(Sint32 actor)
{
	Uint16 facing = fdActorGetById(MainRoutineStateWork.leaderActorId)->facing;

	if (FACING_NEAREST_CARDINAL(facing) == FACING_NORTH)
	{
		mnShopRun(SHOP_DAILA_ARMOR, actor);
	}
	else
	{
		fdScriptStart();
		fdScriptEndLastingPsynergy(0);
		fdScriptSetMessageId((Sint32)MsgDailaRoomsNewTravelers);
		// "You guys look new 'round these parts! What brings you to here?..."
		fdScriptShowMessageAndWait(actor, 0);
		fdScriptEnd();
	}
}

// The innkeeper offers a room when Felix talks to him across the counter at his east side, facing
// west; from any other side he wonders what to cook.
void scDailaRoomsTalkToInnkeeper(Sint32 actor)
{
	Uint16 facing = fdActorGetById(MainRoutineStateWork.leaderActorId)->facing;

	if (FACING_NEAREST_CARDINAL(facing) == FACING_WEST)
	{
		mnInnCheckIn(DAILA_INN, actor);
	}
	else
	{
		fdScriptStart();
		fdScriptEndLastingPsynergy(0);
		fdScriptSetMessageId((Sint32)MsgDailaRoomsDinnerChoice);
		// "What should I cook for our guests tonight? I just can't decide."
		fdScriptShowMessageAndWait(actor, 0);
		fdScriptEnd();
	}
}

// The sanctuary's healer first asks whether the party has a lot of time and nowhere to go, and
// sends them to Poi at the Kandorean Temple whatever they answer. After that he heals when Felix
// talks to him from in front of his counter, facing north, and from any other side asks again.
void scDailaRoomsTalkToHealer(Sint32 actor)
{
	Sint32 msg;
	Uint16 facing = fdActorGetById(MainRoutineStateWork.leaderActorId)->facing;

	if (FACING_NEAREST_CARDINAL(facing) == FACING_NORTH)
	{
		if (syGameFlagGet(FLAG_DAILA_HEALER_SENT_TO_POI) == 0)
		{
			fdScriptStart();
			fdScriptEndLastingPsynergy(0);
			msg = (Sint32)MsgDailaRoomsTravelersFreeTime;
			fdScriptSetMessageId(msg);
			// "Travelers! Well, I'll be! Do you have a lot of time and nowhere to..."
			fdMessageShowAtActor(actor, 0);
			if (fdScriptAskYesNoWithActorAnimation(fdPartyGetLeaderActor(), 0) == 0)
			{
				syFieldScriptWait(10);
				fdScriptSetMessageId(msg + 1);
			}
			else
			{
				syFieldScriptWait(20);
				fdScriptSetMessageId(msg + 2);
			}
			fdScriptShowMessageAndWait(actor, 0);
			fdScriptEnd();
			syGameFlagSet(FLAG_DAILA_HEALER_SENT_TO_POI);
		}
		else
		{
			mnSanctumRun(actor);
		}
	}
	else
	{
		fdScriptStart();
		fdScriptEndLastingPsynergy(0);
		msg = (Sint32)MsgDailaRoomsTravelersFreeTime;
		fdScriptSetMessageId(msg);
		// "Travelers! Well, I'll be! Do you have a lot of time and nowhere to go?"
		fdMessageShowAtActor(actor, 0);
		if (fdScriptAskYesNoWithActorAnimation(fdPartyGetLeaderActor(), 0) == 0)
		{
			syFieldScriptWait(10);
			fdScriptSetMessageId(msg + 1);
		}
		else
		{
			syFieldScriptWait(20);
			fdScriptSetMessageId(msg + 2);
		}
		fdScriptShowMessageAndWait(actor, 0);
		fdScriptEnd();
		syGameFlagSet(FLAG_DAILA_HEALER_SENT_TO_POI);
	}
}

// Once Tavi is rescued, the sanctuary's healer heals when Felix talks to him from in front of his
// counter, facing north, and from any other side tells the party there is nothing left for them in
// Daila.
void scDailaRoomsTalkToHealerOnceTaviIsRescued(Sint32 actor)
{
	Uint16 facing = fdActorGetById(MainRoutineStateWork.leaderActorId)->facing;

	if (FACING_NEAREST_CARDINAL(facing) == FACING_NORTH)
	{
		mnSanctumRun(actor);
	}
	else
	{
		fdScriptStart();
		fdScriptEndLastingPsynergy(0);
		fdScriptSetMessageId((Sint32)MsgDailaRoomsLeaveForMadra);
		// "There is nothing left for you in Daila... Please be careful on your..."
		fdScriptShowMessageAndWait(actor, 0);
		fdScriptEnd();
	}
}

// Once Tavi is rescued, the weapon merchant still sells from in front of his counter, and from any
// other side tells of Daila's pirate past.
void scDailaRoomsTalkToWeaponMerchantOnceTaviIsRescued(Sint32 actor)
{
	Uint16 facing = fdActorGetById(MainRoutineStateWork.leaderActorId)->facing;

	if (FACING_NEAREST_CARDINAL(facing) == FACING_NORTH)
	{
		mnShopRun(SHOP_DAILA_WEAPONS, actor);
	}
	else
	{
		fdScriptStart();
		fdScriptEndLastingPsynergy(0);
		fdScriptSetMessageId((Sint32)MsgDailaRoomsPirateTreasure);
		// "Way back when, Daila used to be a hideout for a band of pirates......"
		fdScriptShowMessageAndWait(actor, 0);
		fdScriptEnd();
	}
}

// Once Tavi is rescued, the armor merchant still sells from in front of his counter, and from any
// other side talks of how spacious the village is.
void scDailaRoomsTalkToArmorMerchantOnceTaviIsRescued(Sint32 actor)
{
	Uint16 facing = fdActorGetById(MainRoutineStateWork.leaderActorId)->facing;

	if (FACING_NEAREST_CARDINAL(facing) == FACING_NORTH)
	{
		mnShopRun(SHOP_DAILA_ARMOR, actor);
	}
	else
	{
		fdScriptStart();
		fdScriptEndLastingPsynergy(0);
		fdScriptSetMessageId((Sint32)MsgDailaRoomsSpaciousVillage);
		// "We don't have a lot of people in Daila, but it's a very spacious..."
		fdScriptShowMessageAndWait(actor, 0);
		fdScriptEnd();
	}
}

// Once Tavi is rescued, the innkeeper still offers a room across the counter at his east side, and
// from any other side wonders when the fishermen can put to sea again.
void scDailaRoomsTalkToInnkeeperOnceTaviIsRescued(Sint32 actor)
{
	Uint16 facing = fdActorGetById(MainRoutineStateWork.leaderActorId)->facing;

	if (FACING_NEAREST_CARDINAL(facing) == FACING_WEST)
	{
		mnInnCheckIn(DAILA_INN, actor);
	}
	else
	{
		fdScriptStart();
		fdScriptEndLastingPsynergy(0);
		fdScriptSetMessageId((Sint32)MsgDailaRoomsFishermenWaiting);
		// "Our fishermen still can't head out to sea... When will they start..."
		fdScriptShowMessageAndWait(actor, 0);
		fdScriptEnd();
	}
}

// Once Tavi is rescued, the old man in the middle house asks whether Felix is going to Madra, and
// answers yes or no.
void scDailaRoomsAskGoingToMadra(void)
{
	Sint32 msg;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	msg = (Sint32)MsgDailaRoomsGoingToMadra;
	fdScriptSetMessageId(msg);
	// "Are you going to Madra?"
	fdMessageShowAtActor(ACTOR_OLD_MAN_ON_MADRA, 0);
	if (fdScriptAskYesNoWithActorAnimation(fdPartyGetLeaderActor(), 0) == 0)
	{
		syFieldScriptWait(10);
		fdScriptSetMessageId(msg + 1);
	}
	else
	{
		syFieldScriptWait(20);
		fdScriptSetMessageId(msg + 2);
	}
	fdScriptShowMessageAndWait(ACTOR_OLD_MAN_ON_MADRA, 0);
	fdScriptEnd();
}

// Once Tavi is rescued, the mayor in his house asks whether the party is looking for a boat too,
// and answers yes or no.
void scDailaRoomsAskLookingForBoat(void)
{
	Sint32 msg;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	msg = (Sint32)MsgDailaRoomsLookingForBoat;
	fdScriptSetMessageId(msg);
	// "Don't tell me you're all looking for a boat, too!"
	fdMessageShowAtActor(ACTOR_MAYOR, 0);
	if (fdScriptAskYesNoWithActorAnimation(fdPartyGetLeaderActor(), 0) == 0)
	{
		syFieldScriptWait(10);
		fdScriptSetMessageId(msg + 1);
	}
	else
	{
		syFieldScriptWait(20);
		fdScriptSetMessageId(msg + 2);
	}
	fdScriptShowMessageAndWait(ACTOR_MAYOR, 0);
	fdScriptEnd();
}

// Once Tavi is rescued, the woman at the inn caught practicing for the dinner show hopes Felix was
// not watching, then explains.
void scDailaRoomsTalkCaughtPracticing(void)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdScriptSetMessageId((Sint32)MsgDailaRoomsWatchingPractice);
	// "Er... Aheh heh... You... weren't watching, were you?"
	fdScriptShowMessageAndWait(ACTOR_PRACTICING_PERFORMER, 0);
	syFieldScriptWait(30);
	fdActorSetAnimationById(ACTOR_PRACTICING_PERFORMER, ANIM_NOD);
	syFieldScriptWait(30);
	// "I was just practicing for the show we're putting on tonight."
	fdScriptShowMessageAndWait(ACTOR_PRACTICING_PERFORMER, 0);
	fdScriptEnd();
}

#include "shared/scene_freeze_puddle_into_pillar.h"

// Felix picks up the Sea God's Tear at the foot of the statue behind the healer (metatile (14,
// 12)): "Felix got the Sea God's Tear." When the party's bags are full he is asked what to drop for
// it, and if he keeps everything the Tear stays where it was.
void scDailaRoomsTakeSeaGodsTear(void)
{
	Sint32 savedGravity;
	Sint32 savedVelocityY;
	FIXED  savedY;
	Sint32 item;

	savedGravity   = fdActorGetById(ACTOR_SEA_GODS_TEAR)->gravity;
	item           = ITEM_SEA_GODS_TEAR;
	savedVelocityY = fdActorGetById(ACTOR_SEA_GODS_TEAR)->velocityY;
	savedY         = fdActorGetById(ACTOR_SEA_GODS_TEAR)->y.fixed;
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	if (gmPartyInventoryFindRoom(item) >= 0)
	{
		fdItemDisplayShowPicture(fdActorGetById(ACTOR_SEA_GODS_TEAR),
		                         ITEM_DISPLAY_SPARKLES | ITEM_DISPLAY_RAISED);
		sySoundPlay(MUS_TREASURE_FOUND);
		mnMessageArgumentPush(item, MESSAGE_ARGUMENT_ITEM);
		// "Felix got ...."
		mnFieldMessageShow((Sint32)MsgItemReceived, FIELD_MESSAGE_ZOOM);
		gmPartyInventoryAdd(item);
		fdActorSetPos(ACTOR_SEA_GODS_TEAR, 0, 0);
		fdActorSetPos(ACTOR_TEAR_SPOT, 232, 194);
		syGameFlagSet(FLAG_SEA_GODS_TEAR_TAKEN);
	}
	else
	{
		fdItemDisplayShowPicture(fdActorGetById(ACTOR_SEA_GODS_TEAR),
		                         ITEM_DISPLAY_SPARKLES | ITEM_DISPLAY_RAISED);
		sySoundPlay(MUS_TREASURE_FOUND);
		if (scDailaRoomsGiveItem(item, 0) != -1)
		{
			fdActorSetPos(ACTOR_SEA_GODS_TEAR, 0, 0);
			fdActorSetPos(ACTOR_TEAR_SPOT, 232, 196);
			syGameFlagSet(FLAG_SEA_GODS_TEAR_TAKEN);
		}
		else
		{
			fdActorSetPos(ACTOR_SEA_GODS_TEAR, 0, 0);
			syFieldScriptWait(1);
			fdActorGetById(ACTOR_SEA_GODS_TEAR)->velocityY = savedVelocityY;
			fdActorGetById(ACTOR_SEA_GODS_TEAR)->y.fixed   = savedY;
			fdActorGetById(ACTOR_SEA_GODS_TEAR)->gravity   = savedGravity;
			fdActorGetById(ACTOR_SEA_GODS_TEAR)->update    = NULL;
			fdActorStartIdle(fdActorGetById(ACTOR_SEA_GODS_TEAR));
			fdActorSetSpritePriority(ACTOR_SEA_GODS_TEAR, 1);
			fdActorGetById(ACTOR_SEA_GODS_TEAR)->motionFlags     = ACTOR_FALLS;
			fdActorGetById(ACTOR_SEA_GODS_TEAR)->scaleY          = toFIXED(6) / 5;
			fdActorGetById(ACTOR_SEA_GODS_TEAR)->scaleX          = toFIXED(6) / 5;
			fdActorGetById(ACTOR_SEA_GODS_TEAR)->collisionFlags |= ACTOR_NEAR_TALK;
			fdActorSetPos(ACTOR_SEA_GODS_TEAR, 232, 196);
		}
	}
	fdScriptEnd();
}

// Gives an item to the party, asking what to drop when no one has room for it; returns who took it,
// or -1. The overlay's own copy of the field engine's fdPartyInventoryGiveItem.
Sint32 scDailaRoomsGiveItem(Sint32 item, Sint32 unused)
{
	FIELD_STATE *state;
	Sint32       saved;
	Sint32       owner;
	Sint32       result;
	Sint32       count;
	Sint32       i;
	Sint32       member;
	Sint32       slot;

	state = RamHeapSlots->fieldState;
	saved = state->msg;
	owner = gmPartyInventoryAdd(item);
	if (owner == -1)
	{
		mnMessageArgumentPush(item, MESSAGE_ARGUMENT_ITEM);
		// "Felix got ...."
		mnFieldMessageShow((Sint32)MsgItemReceived, FIELD_MESSAGE_ZOOM);
		// "But you can't carry any more items."
		mnFieldMessageShow((Sint32)MsgItemCannotCarry, FIELD_MESSAGE_ZOOM);
		for (;;)
		{
			// "What will you drop?"
			mnFieldMessageShow((Sint32)MsgItemDropPrompt, FIELD_MESSAGE_ZOOM);
			result = mnItemPickerRunToDrop(&member, &slot);
			if (result == -1)
			{
				if (gmItemGetDirect(item)->flags & ITEM_INDISPENSABLE)
				{
					mnMessageArgumentPush(item, MESSAGE_ARGUMENT_ITEM);
					// "That ...'s valuable! You must get it!"
					mnFieldMessageShow((Sint32)MsgItemDropPrompt + 4, FIELD_MESSAGE_ZOOM);
					continue;
				}
				mnMessageArgumentPush(item, MESSAGE_ARGUMENT_ITEM);
				// "Drop the ...?"
				mnFieldMessageShow((Sint32)MsgItemDropPrompt + 1,
				                   FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_KEEP_OPEN);
				result = fdYesNoAsk(1);
				mnMessageCloseFinishedWindows();
				if (result != 0)
					continue;
				mnMessageArgumentPush(item, MESSAGE_ARGUMENT_ITEM);
				// "Felix drops the ...."
				mnFieldMessageShow((Sint32)MsgItemDropPrompt + 2, FIELD_MESSAGE_ZOOM);
				state->msg = saved;
				return owner;
			}
			syUnitGet(member);
			count = gmInventoryGetQuantity(member, slot);
			if (gmInventoryCountItem(member, item) >= INVENTORY_STACK_MAX)
			{
				mnMessageArgumentPush(member, MESSAGE_ARGUMENT_ACTOR);
				mnMessageArgumentPush(item, MESSAGE_ARGUMENT_ITEM);
				// That member already carries a full stack of the item; the line this shows reads
				// only "Empty".
				mnFieldMessageShow((Sint32)MsgItemDropPrompt + 7, FIELD_MESSAGE_ZOOM);
				continue;
			}
			break;
		}
		for (i = 0; i < count; i++)
			gmInventoryDiscard(member, slot);
		gmUnitRefreshPsynergy(member);
		gmUnitCalculate(member);
		owner = gmPartyInventoryAdd(item);
		sySoundPlay(MUS_TREASURE_FOUND);
		if (owner == MainRoutineStateWork.leaderActorId)
		{
			mnMessageArgumentPush(item, MESSAGE_ARGUMENT_ITEM);
			// "Felix got ...."
			mnFieldMessageShow((Sint32)MsgItemReceived,
			                   FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_WAITING_FOR_SOUND);
		}
		else
		{
			mnMessageArgumentPush(item, MESSAGE_ARGUMENT_ITEM);
			mnMessageArgumentPush(owner, MESSAGE_ARGUMENT_ACTOR);
			// "Felix gave ... to ...."
			mnFieldMessageShow((Sint32)MsgItemGiven,
			                   FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_WAITING_FOR_SOUND);
		}
		state->msg = saved;
		return owner;
	}
	else
	{
		sySoundPlay(MUS_TREASURE_FOUND);
		mnMessageArgumentPush(item, MESSAGE_ARGUMENT_ITEM);
		// "Felix got ...."
		mnFieldMessageShow((Sint32)MsgItemReceived,
		                   FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_WAITING_FOR_SOUND);
		if (owner != MainRoutineStateWork.leaderActorId)
		{
			mnMessageArgumentPush(item, MESSAGE_ARGUMENT_ITEM);
			mnMessageArgumentPush(owner, MESSAGE_ARGUMENT_ACTOR);
			// "Felix gave ... to ...."
			mnFieldMessageShow((Sint32)MsgItemReceived + 1, FIELD_MESSAGE_ZOOM);
		}
		state->msg = saved;
	}
	return owner;
}

// Sets up the houses (the boys eating once they are home, the armor merchant behind his counter) or
// the sanctuary (the Tear on the pillars' top until it is taken, the puddles until Tavi is rescued,
// the frozen pillars after).
Sint32 scInit(void)
{
	MAP_STATE_GROUND_EFFECTS *map;
	Sint32                   *effect = &FieldStateWork->transition;
	Sint32                    value = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);

	*effect                         = value;
	if (MainRoutineStateWork.next == SCENE_DAILA_ROOMS_HOUSES)
	{
		if (MainRoutineStateWork.entrance == ENTRANCE_COW_ISLET_HOUSE)
		{
			scFadeInAndClearFlag();
		}
		else
		{
			if (syGameFlagGet(FLAG_TAVI_RESCUED))
			{
				fdActorSetAnimationById(ACTOR_RIKI, ANIM_EATING);
				fdActorSetSpriteFlags(fdActorGetById(ACTOR_RIKI), 0);
				fdActorSetAnimationById(ACTOR_TAVI, ANIM_EATING);
				fdActorSetSpriteFlags(fdActorGetById(ACTOR_TAVI), 0);
			}
			fdActorGetById(MainRoutineStateWork.leaderActorId)->priorityFlags |=
				ACTOR_PRIORITY_IGNORES_RISES;
			fdActorGetById(ACTOR_OLD_WOMAN_ON_THE_KANDOREAN_TEMPLE)->priorityFlags |=
				ACTOR_PRIORITY_IGNORES_RISES;
			fdActorGetById(ACTOR_ARMOR_MERCHANT)->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
			fdActorSetSpritePriority(ACTOR_ARMOR_MERCHANT, 2);
			fdActorGetById(ACTOR_ARMOR_MERCHANT)->collisionFlags |= ACTOR_NEAR_TALK;
		}
	}
	else if (MainRoutineStateWork.next == SCENE_DAILA_ROOMS_SANCTUM)
	{
		map = (MAP_STATE_GROUND_EFFECTS *)RamHeapSlots->mapGrid;
		fdScriptStart();
		fdScriptEndLastingPsynergy(0);
		fdActorSetSpriteFlags(fdActorGetById(ACTOR_TEAR_SPOT), 0);
		fdEffectStartBlankOverlay(0);
		if (!syGameFlagGet(FLAG_SEA_GODS_TEAR_TAKEN))
		{
			fdActorSetItemIcon(fdActorGetById(ACTOR_SEA_GODS_TEAR), ITEM_SEA_GODS_TEAR);
			fdActorSetSpriteFlags(fdActorGetById(ACTOR_SEA_GODS_TEAR), 0);
			fdActorSetSpritePriority(ACTOR_SEA_GODS_TEAR, 1);
			fdActorGetById(ACTOR_SEA_GODS_TEAR)->scaleY          = toFIXED(6) / 5;
			fdActorGetById(ACTOR_SEA_GODS_TEAR)->scaleX          = toFIXED(6) / 5;
			fdActorGetById(ACTOR_SEA_GODS_TEAR)->collisionFlags |= ACTOR_NEAR_TALK;
			fdActorGetById(ACTOR_TEAR_SPOT)->collisionFlags     |= ACTOR_NEAR_TALK;
		}
		else
		{
			fdActorGetById(ACTOR_SEA_GODS_TEAR)->collisionFlags |= ACTOR_NEAR_TALK;
			fdActorSetSpriteFlags(fdActorGetById(ACTOR_SEA_GODS_TEAR), 0);
		}
		if (!syGameFlagGet(FLAG_TAVI_RESCUED))
		{
			fdActorSetPos(ACTOR_FIRST_PUDDLE, 0, 0);
			fdActorSetPos(ACTOR_SECOND_PUDDLE, 0, 0);
			fdActorSetPos(ACTOR_THIRD_PUDDLE, 0, 0);
			map->wading = MAP_WADING_UPPER_LEVEL;
		}
		else
		{
			fdFlagBlockPlaceAll(DailaRoomsPillarFrozenFlags);
		}
		fdScriptEnd();
	}
	return 0;
}

// Once Tavi is rescued, draws the sanctuary's pool drained down to its three puddles, from the
// map's spare copy of it.
Sint32 scPrepareMap(void)
{
	if (MainRoutineStateWork.next == SCENE_DAILA_ROOMS_SANCTUM && syGameFlagGet(FLAG_TAVI_RESCUED))
	{
		fdTilesetAnimationPause(0);
		fdMapGridCopyMetatiles(29, 33, 10, 6, 10, 12);
	}
	return 0;
}

#include "shared/scene_fade_in_and_clear_flag.h"

// Felix hops over one of the sanctuary's drained puddles when he walks into it from the metatile
// beside it on any side, landing on the far side if the ground there is clear.
void scDailaRoomsJumpForward(void)
{
	fdTryJumpForward();
}

// clang-format off
Sint16 DailaRoomsPillarFrozenFlags[] = {
	ACTOR_FIRST_PUDDLE,  FLAG_DAILA_SANCTUARY_PUDDLE_FROZEN_FIRST + 0,
	ACTOR_SECOND_PUDDLE, FLAG_DAILA_SANCTUARY_PUDDLE_FROZEN_FIRST + 1,
	ACTOR_THIRD_PUDDLE,  FLAG_DAILA_SANCTUARY_PUDDLE_FROZEN_FIRST + 2,
	-1, 0,
};

// Tavi's father's script: he paces his house back and forth.
Sint32 DailaRoomsPacingScript[] = {
#if !DAILA_APOJII_TUNDARIA_PACERS_NOT_PUSHABLE
	SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_PUSHABLE | ACTOR_SOLID),
#else
	SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_SOLID),
#endif
	SC_ACCELERATION(0.1),
	SC_SPEED(0.3),
	SC_LABEL(1),
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(312, 0, 168),
	SC_SYNC,
	SC_WAIT(20),
	SC_ANIMATION(ANIM_STAND),
	SC_SET(SW_FACING, FACING_EAST),
	SC_WAIT(20),
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(392, 0, 168),
	SC_SYNC,
	SC_WAIT(20),
	SC_ANIMATION(ANIM_STAND),
	SC_SET(SW_FACING, FACING_EAST),
	SC_WAIT(20),
	SC_LOOP(SC_FOREVER, SC_TO(1)),
	SC_END,
};

// The inn's two performers' script once Tavi is rescued: they turn on the spot, rehearsing the dinner show.
Sint32 DailaRoomsLookAroundScript[] = {
	SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_SOLID),
	SC_ACCELERATION(0.1),
	SC_SPEED(0.3),
	SC_LABEL(1),
	SC_ANIMATION(ANIM_STAND),
	SC_SET(SW_FACING, FACING_SOUTHWEST),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_WEST),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_NORTHWEST),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_NORTH),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_EAST),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_SOUTHEAST),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_SOUTHWEST),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_WEST),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_NORTHWEST),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_NORTH),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_EAST),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_SOUTHEAST),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_SOUTHWEST),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_WEST),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_NORTHWEST),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_NORTH),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_EAST),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_SOUTHEAST),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_WAIT(5),
	SC_SET(SW_Y, toFIXED(8)),
	SC_WAIT(30),
	SC_SET(SW_Y, toFIXED(8)),
	SC_WAIT(30),
	SC_LOOP(SC_FOREVER, SC_TO(1)),
	SC_END,
	// Unused: a second script after the end, wandering near home.
#if !DAILA_APOJII_TUNDARIA_PACERS_NOT_PUSHABLE
	SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_PUSHABLE | ACTOR_SOLID),
#else
	SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_SOLID),
#endif
	SC_ACCELERATION(0.1),
	SC_SPEED(0.3),
	SC_LABEL(1),
	SC_ANIMATION(ANIM_STAND),
	SC_ANIMATION(ANIM_WALK),
	SC_WANDER_HOME(2, 2, 6),
	SC_SYNC,
	SC_WAIT(4),
	SC_LOOP(SC_FOREVER, SC_TO(1)),
	SC_END,
};
// clang-format on

#include "shared/scene_entrances_default.c"

// clang-format off
// Where the houses' doors lead, before Tavi is rescued, out into the flooded village.
Sint32 DailaRoomsExits[] = {
	SCENE_EXITS(DailaRoomsHouses),
	SCENE_EXIT_UNLESS(1, DailaFlooded, 1, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(2, DailaFlooded, 2, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(3, DailaFlooded, 3, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(4, DailaFlooded, 4, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(5, DailaFlooded, 5, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(6, DailaFlooded, 6, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(7, NorthOseniaIslet, 3, EXIT_FLAG_NONE),
	SCENE_EXITS(DailaRoomsSanctum),
	SCENE_EXIT_UNLESS(1, DailaFlooded, 7, EXIT_FLAG_NONE),
	SCENE_EXITS_END,
};

// Where the houses' doors lead once Tavi is rescued, out into the drained village.
Sint32 DailaRoomsExitsOnceTaviIsRescued[] = {
	SCENE_EXITS(DailaRoomsHouses),
	SCENE_EXIT_UNLESS(1, Daila, 1, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(2, Daila, 2, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(3, Daila, 3, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(4, Daila, 4, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(5, Daila, 5, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(6, Daila, 6, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(7, NorthOseniaIslet, 3, EXIT_FLAG_NONE),
	SCENE_EXITS(DailaRoomsSanctum),
	SCENE_EXIT_UNLESS(1, Daila, 7, EXIT_FLAG_NONE),
	SCENE_EXITS_END,
};

// The houses, shops and inn before Tavi is rescued (enum DAILA_ROOMS_ACTOR_ID).
ACTOR_PLACE DailaRoomsHousesPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 104, FACING_WEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 88, 0, 88, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, (Sint32)DailaRoomsPacingScript, 360, 0, 168, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 72, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 632, 0, 168, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 568, 0, 88, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 160, 0, 360, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 456, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 440, FACING_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 216, 0, 472, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WEAPON_MERCHANT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 712, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_ARMOR_MERCHANT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 724, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_INNKEEPER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 468, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_INN_WORKER, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 536, 0, 456, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 376, FACING_NORTHWEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 424, 0, 360, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_AUBURN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 472, 0, 746, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_BLUE_COAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 568, 0, 712, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_IN_GREEN_SHAWL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 712, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 728, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

// The sanctuary (DAILA_SANCTUARY_ACTOR_ID).
ACTOR_PLACE DailaRoomsSanctumPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_HEALER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 333, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 196, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, FLAG_SCENE_TEMP_23, MOVEMENT_TYPE_STAND, 184, 0, 248, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, FLAG_SCENE_TEMP_24, MOVEMENT_TYPE_STAND, 216, 0, 248, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, FLAG_SCENE_TEMP_25, MOVEMENT_TYPE_STAND, 248, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

// The houses, shops and inn once Tavi is rescued, with Riki and Tavi home.
ACTOR_PLACE DailaRoomsHousesPlacementsOnceTaviIsRescued[] = {
	ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 176, 0, 120, FACING_NORTHWEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 88, 0, 88, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, (Sint32)DailaRoomsPacingScript, 360, 0, 168, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 72, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 632, 0, 168, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 568, 0, 88, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 160, 0, 360, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 440, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 488, FACING_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 216, 0, 472, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WEAPON_MERCHANT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 712, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_ARMOR_MERCHANT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 724, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_INNKEEPER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 468, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_INN_WORKER, CONDITION_ALWAYS, (Sint32)DailaRoomsLookAroundScript, 424, 0, 520, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 376, FACING_NORTHWEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 424, 0, 360, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_RIKI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 161, 0, 111, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_TAVI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 112, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, (Sint32)DailaRoomsLookAroundScript, 456, 0, 520, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_AUBURN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 472, 0, 746, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_BLUE_COAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 568, 0, 712, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_IN_GREEN_SHAWL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 712, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 728, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

// The houses' doors, talk and Mind Read before Tavi is rescued.
CUE DailaRoomsHousesCues[] = {
	{CUE_EXIT, DAILA_ROOMS_HOUSES_TO_DAILA_WEST, CONDITION_ALWAYS, DAILA_ROOMS_HOUSES_TO_DAILA_WEST},
	{CUE_EXIT, DAILA_ROOMS_HOUSES_TO_DAILA_NORTHWEST, CONDITION_ALWAYS, DAILA_ROOMS_HOUSES_TO_DAILA_NORTHWEST},
	{CUE_EXIT, DAILA_ROOMS_HOUSES_TO_DAILA_NORTHEAST, CONDITION_ALWAYS, DAILA_ROOMS_HOUSES_TO_DAILA_NORTHEAST},
	{CUE_EXIT, DAILA_ROOMS_HOUSES_TO_DAILA_EAST, CONDITION_ALWAYS, DAILA_ROOMS_HOUSES_TO_DAILA_EAST},
	{CUE_EXIT, DAILA_ROOMS_HOUSES_TO_DAILA_SOUTHWEST, CONDITION_ALWAYS, DAILA_ROOMS_HOUSES_TO_DAILA_SOUTHWEST},
	{CUE_EXIT, DAILA_ROOMS_HOUSES_TO_DAILA_SOUTHEAST, CONDITION_ALWAYS, DAILA_ROOMS_HOUSES_TO_DAILA_SOUTHEAST},
	{CUE_EXIT, DAILA_ROOMS_HOUSES_TO_NORTH_OSENIA_ISLET, CONDITION_ALWAYS, DAILA_ROOMS_HOUSES_TO_NORTH_OSENIA_ISLET},
	{CUE_TALK, ACTOR_RIKIS_FATHER, CONDITION_ALWAYS, MSGNO(MsgFieldRikisReallyInForItWhen)},
	{CUE_TALK, ACTOR_RIKIS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgFieldThatNoGoodKidOfMine)},
	{CUE_TALK, ACTOR_TAVIS_FATHER, CONDITION_ALWAYS, MSGNO(MsgFieldIWonderWhereOurLittleTavi)},
	{CUE_TALK, ACTOR_TAVIS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgFieldTaviNeverMissesThreeOclockSnacksies)},
	{CUE_TALK, ACTOR_OLD_MAN_ON_MADRA, CONDITION_ALWAYS, MSGNO(MsgFieldIfYoureLookingForABoat)},
	{CUE_TALK, ACTOR_OLD_WOMAN_ON_THE_KANDOREAN_TEMPLE, CONDITION_ALWAYS, MSGNO(MsgFieldIfYouHeadSouthYoullFind)},
	{CUE_TALK, ACTOR_MAYOR, CONDITION_ALWAYS, MSGNO(MsgFieldInAllMyYearsAsMayor)},
	{CUE_TALK, ACTOR_WOMAN_ON_THE_NOISE, CONDITION_ALWAYS, MSGNO(MsgFieldTheWaveWasAwfulButThe)},
	{CUE_TALK, ACTOR_MAN_ON_THE_RUMBLING, CONDITION_ALWAYS, MSGNO(MsgFieldALotOfFolkSaidThey)},
	{CUE_TALK, ACTOR_WOMAN_ON_DAILAS_LUCK, CONDITION_ALWAYS, MSGNO(MsgFieldBetweenThePiratesAndTheTidal)},
	{CUE_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, SCRIPT_REF(scDailaRoomsTalkToWeaponMerchant)},
	{CUE_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, SCRIPT_REF(scDailaRoomsTalkToArmorMerchant)},
	{CUE_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, SCRIPT_REF(scDailaRoomsTalkToInnkeeper)},
	{CUE_TALK, ACTOR_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgFieldWeRunAVerySmallInn)},
	{CUE_TALK, ACTOR_TRAVELER_FROM_MADRA, CONDITION_ALWAYS, MSGNO(MsgFieldIDontKnowWhenIllBe)},
	{CUE_TALK, ACTOR_GUEST_ON_MADRAS_SEAFOOD, CONDITION_ALWAYS, MSGNO(MsgFieldIHearTheSeafoodInMadra)},
	{CUE_TALK, ACTOR_WOMAN_ON_THE_CURRENTS, CONDITION_ALWAYS, MSGNO(MsgSeaOfTimeCurrents)},
	{CUE_TALK, ACTOR_OLD_MAN_ON_THE_RED_ROCKS, CONDITION_ALWAYS, MSGNO(MsgSeaOfTimeRedRocks)},
	{CUE_TALK, ACTOR_OLD_WOMAN_ON_HER_SONS, CONDITION_ALWAYS, MSGNO(MsgSeaOfTimeThreeGenerations)},
	{CUE_TALK, ACTOR_ARMOR_COUNTER, CONDITION_ALWAYS, SCRIPT_REF(scDailaRoomsTalkToArmorMerchant)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_RIKIS_FATHER, CONDITION_ALWAYS, MSGNO(MsgFieldRikiTaviWhereAreYouWhat)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_RIKIS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgFieldThatTavisABadInfluenceOn)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TAVIS_FATHER, CONDITION_ALWAYS, MSGNO(MsgFieldIllBetThatWhereverTaviIs)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TAVIS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgFieldIfTaviIsntHomeSoonWere)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_MADRA, CONDITION_ALWAYS, MSGNO(MsgFieldIWonderHowTheRoadTo)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_THE_KANDOREAN_TEMPLE, CONDITION_ALWAYS, MSGNO(MsgFieldIWonderIfTheHolyMan)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAYOR, CONDITION_ALWAYS, MSGNO(MsgFieldAtLeastNobodySeemsToHave)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_NOISE, CONDITION_ALWAYS, MSGNO(MsgFieldIStillDontKnowWhatThat)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_RUMBLING, CONDITION_ALWAYS, MSGNO(MsgFieldWeShouldFindADifferentWay)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_DAILAS_LUCK, CONDITION_ALWAYS, MSGNO(MsgFieldNowThatBriggsIsLockedAway)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgFieldImGladImNotGoingTo)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgDailaRoomsScamps)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgFieldWeCantSailAndWeCant)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgFieldLookAtAllTheseGuestsWe)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TRAVELER_FROM_MADRA, CONDITION_ALWAYS, MSGNO(MsgFieldAfterAllThatShakingIWouldnt)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GUEST_ON_MADRAS_SEAFOOD, CONDITION_ALWAYS, MSGNO(MsgFieldImSoTornDoILike)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_CURRENTS, CONDITION_ALWAYS, MSGNO(MsgSeaOfTimeHusbandDizzy)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_THE_RED_ROCKS, CONDITION_ALWAYS, MSGNO(MsgSeaOfTimeYeppToldMe)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_HER_SONS, CONDITION_ALWAYS, MSGNO(MsgSeaOfTimeGiveUp)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_COUNTER, CONDITION_ALWAYS, SCRIPT_REF(scDailaRoomsReadArmorMerchantsMind)},
	{CUE_TABLE_END},
};

// The sanctuary's doors, talk and Mind Read before Tavi is rescued.
CUE DailaRoomsSanctumCues[] = {
	{CUE_EXIT, DAILA_ROOMS_SANCTUM_TO_DAILA, CONDITION_ALWAYS, DAILA_ROOMS_SANCTUM_TO_DAILA},
	// Unused: the sanctuary has no metatiles for exits 2 to 6, nor exits of those numbers.
	{CUE_EXIT, 2, CONDITION_ALWAYS, 2},
	{CUE_EXIT, 3, CONDITION_ALWAYS, 3},
	{CUE_EXIT, 4, CONDITION_ALWAYS, 4},
	{CUE_EXIT, 5, CONDITION_ALWAYS, 5},
	{CUE_EXIT, 6, CONDITION_ALWAYS, 6},
	{CUE_TALK, ACTOR_HEALER, CONDITION_ALWAYS, SCRIPT_REF(scDailaRoomsTalkToHealer)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgFieldTravelersWithNowhereToGoThis)},
	{CUE_TABLE_END},
};

// The houses' doors, talk and Mind Read once Tavi is rescued.
CUE DailaRoomsHousesCuesOnceTaviIsRescued[] = {
	{CUE_EXIT, DAILA_ROOMS_HOUSES_TO_DAILA_WEST, CONDITION_ALWAYS, DAILA_ROOMS_HOUSES_TO_DAILA_WEST},
	{CUE_EXIT, DAILA_ROOMS_HOUSES_TO_DAILA_NORTHWEST, CONDITION_ALWAYS, DAILA_ROOMS_HOUSES_TO_DAILA_NORTHWEST},
	{CUE_EXIT, DAILA_ROOMS_HOUSES_TO_DAILA_NORTHEAST, CONDITION_ALWAYS, DAILA_ROOMS_HOUSES_TO_DAILA_NORTHEAST},
	{CUE_EXIT, DAILA_ROOMS_HOUSES_TO_DAILA_EAST, CONDITION_ALWAYS, DAILA_ROOMS_HOUSES_TO_DAILA_EAST},
	{CUE_EXIT, DAILA_ROOMS_HOUSES_TO_DAILA_SOUTHWEST, CONDITION_ALWAYS, DAILA_ROOMS_HOUSES_TO_DAILA_SOUTHWEST},
	{CUE_EXIT, DAILA_ROOMS_HOUSES_TO_DAILA_SOUTHEAST, CONDITION_ALWAYS, DAILA_ROOMS_HOUSES_TO_DAILA_SOUTHEAST},
	{CUE_EXIT, DAILA_ROOMS_HOUSES_TO_NORTH_OSENIA_ISLET, CONDITION_ALWAYS, DAILA_ROOMS_HOUSES_TO_NORTH_OSENIA_ISLET},
	{CUE_TALK, ACTOR_RIKIS_FATHER, CONDITION_ALWAYS, MSGNO(MsgFieldLetMeTellYouKidsAre)},
	{CUE_TALK, ACTOR_RIKIS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgFieldWhoStaysOutsideUntilHesAbsolutely)},
	{CUE_TALK, ACTOR_TAVIS_FATHER, CONDITION_ALWAYS, MSGNO(MsgFieldOurLittleOneIsFinallyHome)},
	{CUE_TALK, ACTOR_TAVIS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgFieldTaviWolfedDownHisSnackAnd)},
	{CUE_TALK, ACTOR_OLD_MAN_ON_MADRA, CONDITION_ALWAYS, SCRIPT_REF(scDailaRoomsAskGoingToMadra)},
	{CUE_TALK, ACTOR_OLD_WOMAN_ON_THE_KANDOREAN_TEMPLE, CONDITION_ALWAYS, MSGNO(MsgFieldIThinkMadraIsEastOf)},
	{CUE_TALK, ACTOR_MAYOR, CONDITION_ALWAYS, SCRIPT_REF(scDailaRoomsAskLookingForBoat)},
	{CUE_TALK, ACTOR_WOMAN_ON_THE_NOISE, CONDITION_ALWAYS, MSGNO(MsgFieldItsTooDangerousToGoTo)},
	{CUE_TALK, ACTOR_MAN_ON_THE_RUMBLING, CONDITION_ALWAYS, MSGNO(MsgFieldWhoWasThatGuyIWonder)},
	{CUE_TALK, ACTOR_WOMAN_ON_DAILAS_LUCK, CONDITION_ALWAYS, MSGNO(MsgFieldWhyDidThePirateBriggsPlague)},
	{CUE_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, SCRIPT_REF(scDailaRoomsTalkToWeaponMerchantOnceTaviIsRescued)},
	{CUE_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, SCRIPT_REF(scDailaRoomsTalkToArmorMerchantOnceTaviIsRescued)},
	{CUE_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, SCRIPT_REF(scDailaRoomsTalkToInnkeeperOnceTaviIsRescued)},
	{CUE_TALK, ACTOR_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgDailaRoomsDinnerShowPractice)},
	{CUE_TALK, ACTOR_TRAVELER_FROM_MADRA, CONDITION_ALWAYS, MSGNO(MsgDailaRoomsSeafoodWhen)},
	{CUE_TALK, ACTOR_GUEST_ON_MADRAS_SEAFOOD, CONDITION_ALWAYS, MSGNO(MsgDailaRoomsVegetablesAgain)},
	{CUE_TALK, ACTOR_RIKI, CONDITION_ALWAYS, MSGNO(MsgFieldMmmMmnphDadWasSoMad)},
	{CUE_TALK, ACTOR_TAVI, CONDITION_ALWAYS, MSGNO(MsgFieldMmmphMmmmDontTellAnyoneWe)},
	{CUE_TALK, ACTOR_PRACTICING_PERFORMER, CONDITION_ALWAYS, SCRIPT_REF(scDailaRoomsTalkCaughtPracticing)},
	{CUE_TALK, ACTOR_WOMAN_ON_THE_CURRENTS_AFTER_RESCUE, CONDITION_ALWAYS, MSGNO(MsgSeaOfTimeCurrents)},
	{CUE_TALK, ACTOR_OLD_MAN_ON_THE_RED_ROCKS_AFTER_RESCUE, CONDITION_ALWAYS, MSGNO(MsgSeaOfTimeRedRocks)},
	{CUE_TALK, ACTOR_OLD_WOMAN_ON_HER_SONS_AFTER_RESCUE, CONDITION_ALWAYS, MSGNO(MsgSeaOfTimeThreeGenerations)},
	{CUE_TALK, ACTOR_ARMOR_COUNTER_AFTER_RESCUE, CONDITION_ALWAYS, SCRIPT_REF(scDailaRoomsTalkToArmorMerchant)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_RIKIS_FATHER, CONDITION_ALWAYS, MSGNO(MsgFieldDarnThatRikiAhImGlad)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_RIKIS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgFieldHeeHeeMyRikiIsMommys)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TAVIS_FATHER, CONDITION_ALWAYS, MSGNO(MsgFieldIGuessRikiNextDoorMade)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TAVIS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgFieldOnceTaviFinishesEatingImPutting)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_MADRA, CONDITION_ALWAYS, MSGNO(MsgFieldDailaGotHitPrettyHardI)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_THE_KANDOREAN_TEMPLE, CONDITION_ALWAYS, MSGNO(MsgFieldSomeoneShouldSeeHowTheRoad)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAYOR, CONDITION_ALWAYS, MSGNO(MsgFieldHeSaidHedPayMeA)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_NOISE, CONDITION_ALWAYS, MSGNO(MsgFieldTheRoadToMadraGoesThrough)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_RUMBLING, CONDITION_ALWAYS, MSGNO(MsgFieldMaybeThatGuyWashedUpOn)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_DAILAS_LUCK, CONDITION_ALWAYS, MSGNO(MsgFieldBriggsProbablyHasLotsOfMen)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgFieldThisTownMayBeDirtPoor)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgDailaRoomsMazeTreasure)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgDailaRoomsTiredPerformer)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgDailaRoomsDragonKingLine)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TRAVELER_FROM_MADRA, CONDITION_ALWAYS, MSGNO(MsgDailaRoomsBadShows)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GUEST_ON_MADRAS_SEAFOOD, CONDITION_ALWAYS, MSGNO(MsgDailaRoomsGuestDance)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_RIKI, CONDITION_ALWAYS, MSGNO(MsgFieldIStillWantToCatchThat)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TAVI, CONDITION_ALWAYS, MSGNO(MsgFieldImSureGladIMadeIt)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PRACTICING_PERFORMER, CONDITION_ALWAYS, MSGNO(MsgDailaRoomsSeafoodStory)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_CURRENTS_AFTER_RESCUE, CONDITION_ALWAYS, MSGNO(MsgSeaOfTimeHusbandDizzy)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_THE_RED_ROCKS_AFTER_RESCUE, CONDITION_ALWAYS, MSGNO(MsgSeaOfTimeYeppToldMe)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_HER_SONS_AFTER_RESCUE, CONDITION_ALWAYS, MSGNO(MsgSeaOfTimeGiveUp)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_COUNTER_AFTER_RESCUE, CONDITION_ALWAYS, SCRIPT_REF(scDailaRoomsReadArmorMerchantsMindAfterRescue)},
	{CUE_TABLE_END},
};

// The sanctuary once Tavi is rescued: Frost on the three puddles, the jumps between the pillars and the Tear at their top.
CUE DailaRoomsSanctumCuesOnceTaviIsRescued[] = {
	{CUE_EXIT, DAILA_ROOMS_SANCTUM_TO_DAILA, CONDITION_ALWAYS, DAILA_ROOMS_SANCTUM_TO_DAILA},
	// Unused: the sanctuary has no metatiles for exits 2 to 6, nor exits of those numbers.
	{CUE_EXIT, 2, CONDITION_ALWAYS, 2},
	{CUE_EXIT, 3, CONDITION_ALWAYS, 3},
	{CUE_EXIT, 4, CONDITION_ALWAYS, 4},
	{CUE_EXIT, 5, CONDITION_ALWAYS, 5},
	{CUE_EXIT, 6, CONDITION_ALWAYS, 6},
	{STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_NORTH_OF_PUDDLE, CONDITION_ALWAYS, SCRIPT_REF(scDailaRoomsJumpForward)},
	{STEP_CUE_WALKING_INTO(FACING_EAST), METATILE_WEST_OF_PUDDLE, CONDITION_ALWAYS, SCRIPT_REF(scDailaRoomsJumpForward)},
	{STEP_CUE_WALKING_INTO(FACING_WEST), METATILE_EAST_OF_PUDDLE, CONDITION_ALWAYS, SCRIPT_REF(scDailaRoomsJumpForward)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_SOUTH_OF_PUDDLE, CONDITION_ALWAYS, SCRIPT_REF(scDailaRoomsJumpForward)},
	{CUE_TALK, ACTOR_HEALER, CONDITION_ALWAYS, SCRIPT_REF(scDailaRoomsTalkToHealerOnceTaviIsRescued)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgDailaRoomsBoringVillage)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_FIRST_PUDDLE, FLAG_DAILA_SANCTUARY_PUDDLE_FROZEN_FIRST + 0, SCRIPT_REF(scFreezePuddleIntoPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SECOND_PUDDLE, FLAG_DAILA_SANCTUARY_PUDDLE_FROZEN_FIRST + 1, SCRIPT_REF(scFreezePuddleIntoPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_THIRD_PUDDLE, FLAG_DAILA_SANCTUARY_PUDDLE_FROZEN_FIRST + 2, SCRIPT_REF(scFreezePuddleIntoPillar)},
	{CUE_EXAMINE, METATILE_SEA_GODS_TEAR, FLAG_SEA_GODS_TEAR_TAKEN, SCRIPT_REF(scDailaRoomsTakeSeaGodsTear)},
	{CUE_TABLE_END},
};
