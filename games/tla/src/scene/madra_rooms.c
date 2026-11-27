/* @file */
/* Inside Madra's buildings: the inn, the round sanctum, the houses, the */
/* mayor's house with the Black Crystal on its mantel, the jail where Piers is
 */
/* held, and, as a second scene, the left cavern home in the cliff. Walking */
/* into the jail the first time, Felix sees Shin accuse Piers of being one of */
/* the Champa and Piers, angered, lift him on a shaft of ice. Later the old */
/* couple who love mushrooms trade Char, the Mars Djinni, for the Healing */
/* Fungus from the Gondowan Cliffs, and when Felix brings Piers home the */
/* mayor's household welcomes him. */

#include "item_ids.h"
#include "field/field.h"
#include "game/game.h"
#include "scene/scene.h"
#include "field/event_inline.h"
#include "version.h"
#include "field/field_sprite.h"
#include "field/field_sequence_event.h"
#include "field/actor_script.h"
#include "text/msg_ids.h"

/* Set once the old man of the mushroom house has been given the Healing */
/* Fungus and has traded Char for it. */
#define FLAG_MADRA_HEALING_FUNGUS_GIVEN 0x98a
/* Set as the Healing Fungus is given, for this visit: the couple look forward
 */
/* to dinner until the party leaves the area. */
#define FLAG_MADRA_FUNGUS_FOR_DINNER FLAG_AREA_TEMP_10
/* Set once the old man has been offered the Laughing Fungus on this visit. */
#define FLAG_MADRA_LAUGHING_FUNGUS_OFFERED FLAG_SCENE_TEMP_10
/* Set once the man focusing on making ice has groaned on this visit. */
#define FLAG_MADRA_FOCUSING_MAN_GROANED FLAG_SCENE_TEMP_11

/* Madra's houses, numbering the rows MadraRooms_Placements and */
/* MadraRooms_PlacementsOnReturn share. */
typedef enum MADRA_ROOMS_ACTOR_ID {
	/* A couple who fear what lies south in Gondowan, and then the Kibombo. */
	ACTOR_OLD_MAN_WARY_OF_GONDOWAN = ACTOR_FIRST_PLACED,
	ACTOR_WOMAN_WARY_OF_GONDOWAN,
	/* The old man who loves the rare mushrooms of the Gondowan Cliffs, in the
	 */
	/* house south of the market. */
	ACTOR_MUSHROOM_LOVER,
	/* His wife, who cooks them. */
	ACTOR_MUSHROOM_COOK,
	/* Two men curious about the strange ship grounded in the cove to the east.
	 */
	ACTOR_MAN_ON_THE_STRANGE_SHIP,
	ACTOR_MAN_ON_THE_GROUNDED_SHIP,
	/* The elder's household: who keeps the jewel on his mantel with Milady, */
	/* his daughter, the old woman of the house, and his two grandchildren. */
	ACTOR_JEWEL_KEEPER,
	ACTOR_ELDERS_DAUGHTER,
	ACTOR_OLD_WOMAN_OF_THE_ELDERS_HOUSE,
	ACTOR_ELDERS_GRANDSON,
	ACTOR_ELDERS_GRANDDAUGHTER,
	/* The sanctum's healer and the two women who pray there. */
	ACTOR_HEALER,
	ACTOR_WOMAN_OF_THE_SANCTUM,
	ACTOR_OLD_WOMAN_OF_THE_SANCTUM,
	ACTOR_INNKEEPER,
	/* Who helps at the inn, sure it is safe from the Champa. */
	ACTOR_INN_HELPER,
	/* A girl staying at the inn, who hid under her bed in the raid. */
	ACTOR_GIRL_STAYING_AT_THE_INN,
	/* Char, the Mars Djinni the mushroom couple keep. */
	ACTOR_CHAR,
	/* A blank actor on the elder's mantel that shows the Black Crystal */
	/* (scMadraRoomsPlaceBlackCrystal). */
	ACTOR_BLACK_CRYSTAL
} MADRA_ROOMS_ACTOR_ID;

/* The rest of MadraRooms_Placements. */
typedef enum MADRA_ROOMS_TRAVELERS_ACTOR_ID {
	/* Two travelers at the inn, the first slept through the pirate raid; his */
	/* partner wears the same sprite in other colours. */
	ACTOR_TRAVELER_WHO_SLEPT_THROUGH_THE_RAID = ACTOR_BLACK_CRYSTAL + 1,
	ACTOR_TRAVELERS_PARTNER,
	/* The two warriors on their way to Colosso in Tolbi, there once */
	/* FLAG_TRANSFERRED_WON_COLOSSO is set: "I'm Morgan, and this here's Dekka."
	 */
	ACTOR_MORGAN,
	ACTOR_DEKKA
} MADRA_ROOMS_TRAVELERS_ACTOR_ID;

/* The rest of MadraRooms_PlacementsOnReturn, when the party brings Piers */
/* home. The rows for Piers, Jenna and Sheba go to their party slots. */
typedef enum MADRA_ROOMS_RETURN_ACTOR_ID {
	/* The elder, who held Piers' jewel, and Madra's mayor. */
	ACTOR_ELDER = ACTOR_BLACK_CRYSTAL + 1,
	ACTOR_MAYOR,
	/* Unused: two more rows with Kraden's sprite, out of sight. */
	ACTOR_UNUSED_FIRST_KRADEN,
	ACTOR_UNUSED_SECOND_KRADEN,
	ACTOR_KRADEN,
	ACTOR_TRAVELER_WHO_SLEPT_THROUGH_THE_RAID_ON_RETURN,
	ACTOR_TRAVELERS_PARTNER_ON_RETURN
} MADRA_ROOMS_RETURN_ACTOR_ID;

/* The jail's actors, by the rows of MadraRooms_JailPlacements; Piers' row */
/* goes to his party slot. */
typedef enum MADRA_ROOMS_JAIL_ACTOR_ID {
	/* The puddle Piers freezes into a shaft of ice. */
	ACTOR_JAIL_PUDDLE = ACTOR_FIRST_PLACED,
	/* Shin, who threatens Piers, and his friend, who accuses him of being one
	 */
	/* of the Champa. */
	ACTOR_SHIN,
	ACTOR_SHINS_FRIEND,
	/* Kraden, who jumps out from behind Felix. */
	ACTOR_JAIL_KRADEN,
	ACTOR_SECOND_JAIL_PUDDLE,
	/* Once FLAG_GABOMBA_MESSAGE_CARRIED is set: who saw Isaac's party hear of
	 */
	/* Felix, and a man who has been focusing for days to make ice as Piers */
	/* did. */
	ACTOR_WITNESS_ON_ISAACS_PARTY,
	ACTOR_MAN_FOCUSING_ON_ICE
} MADRA_ROOMS_JAIL_ACTOR_ID;

/* The left cavern home (MadraRoomsCavernHomes), by the rows of */
/* MadraRooms_SecondRoomsPlacements. */
typedef enum MADRA_ROOMS_CAVERN_HOME_ACTOR_ID {
	/* A couple who talk of Alhafra, and later of a plan to make money. */
	ACTOR_MAN_ON_ALHAFRA = ACTOR_FIRST_PLACED,
	ACTOR_WOMAN_ON_ALHAFRA,
	/* A woman lying ill in bed until Briggs is caught. */
	ACTOR_SISTER_IN_BED,
	/* Her sister, glad she is better. */
	ACTOR_HER_SISTER
} MADRA_ROOMS_CAVERN_HOME_ACTOR_ID;

/* The cells, (44, 5) and (44, 6) a few steps in from the mayor's door, whose */
/* step trigger runs scMadraRoomsWelcomePiersHome. */
enum { CELL_BEHAVIOR_MADRA_ROOMS_WELCOME = 30 };

/* Madra's inn, as mnInnCheckIn numbers the inns. */
enum { MADRA_INN = 1 };

extern s32 MadraRooms_ShinLifted[];
extern s32 MadraRooms_ShinFalls[];
void scMadraRoomsSayFarewellAgain(void);

void scMadraRoomsAskToStayLonger(void);

extern s32 MadraRooms_OldManWalksToBackRoom[];
extern s32 MadraRooms_WifeComesBack[];
extern s32 MadraRooms_OldManComesBack[];

extern s32 MadraRooms_CharFollows[];

/* Counts one more weird answer Felix gives (fdEventCountAnswer, through the */
/* overlay's stub). */
void scMadraRoomsCountAnswer();

const SCENE_ENTRANCE *scGetEntrances(void) { return SceneEntrances; }

const SCENE_REGION *scGetExitRegions(void) { return 0; }

const u32 *scGetExits(void) { return SceneExits; }

extern ACTOR_PLACE MadraRooms_Placements[];
extern ACTOR_PLACE MadraRooms_PlacementsOnReturn[];
extern ACTOR_PLACE MadraRooms_JailPlacements[];
extern ACTOR_PLACE MadraRooms_SecondRoomsPlacements[];
extern TRIGGER MadraRooms_TriggersBeforeRaid[];
extern TRIGGER MadraRooms_JailTriggers[];
extern TRIGGER MadraRooms_SecondRoomsTriggers[];
extern TRIGGER MadraRooms_TriggersAfterRaid[];
extern TRIGGER MadraRooms_TriggersOnReturn[];

void scMadraRoomsShinAngersPiers(void);

const ACTOR_PLACE *scGetActorPlacements(void) {
	if(MainRoutineStateWork.next == SCENE_MADRA_ROOMS_CAVERN_HOMES)
		return MadraRooms_SecondRoomsPlacements;
	if(MainRoutineStateWork.entrance == MADRA_ROOMS_HOUSES_FROM_MADRA_WEST)
		return MadraRooms_JailPlacements;
	if(fdEventGetFlag(FLAG_GABOMBA_MESSAGE_CARRIED))
		return MadraRooms_PlacementsOnReturn;
	return MadraRooms_Placements;
}

/* Talking to the auburn-haired woman in the house by the town's south-west */
/* corner: "Indra is in direct contact with southern Gondowan... Will we be */
/* safe?" Told yes, she says Felix is only trying to make her feel better; */
/* told no, she tells him to stop getting folks worked up. */
void scMadraRoomsAskWillWeBeSafe(void) {
	s32 msg;

	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	msg = (s32)MsgMadraRoomsIndraIsInDirectContact;
	syFieldEventSetMessageId(msg);
	/* "Indra is in direct contact with southern Gondowan... Will we be safe?"
	 */
	fdEventOpenMessage(ACTOR_WOMAN_WARY_OF_GONDOWAN, 0);
	if(syFieldEventAskYesNoWithActorAnimation(syFieldPartyGetLeaderActor(),
											  0) == 0) {
		syFieldEventWait(10);
		syFieldEventSetMessageId(msg + 1);
	}
	else {
		syFieldEventWait(20);
		syFieldEventSetMessageId(msg + 2);
	}
	fdEventShowMessage(ACTOR_WOMAN_WARY_OF_GONDOWAN, 0);
	fdEventEnd();
}

/* Talking to Madra's innkeeper from across the counter on the inn's ground */
/* floor lets the party stay the night. From any other side he talks of the */
/* week's troubles, then of the Kibombo's wicked magic, and when Felix brings */
/* Piers home of the dead fish on the shore, which the later editions ask as a
 */
/* question. */
void scMadraRoomsTalkToInnkeeper(void) {
	u16 facing = fdActorGet(MainRoutineStateWork.leaderActorId)->facing;

	if(FACING_NEAREST_CARDINAL(facing) == FACING_NORTH) {
		mnInnCheckIn(MADRA_INN, ACTOR_INNKEEPER);
	}
	else {
#if !TLA_PATCH_1
		/* Before patch 1 there is no choice prompt. */
		if(fdEventGetFlag(FLAG_GABOMBA_MESSAGE_CARRIED))
			syFieldEventSetMessageId((s32)MsgMadraRoomsAreYouWorriedAboutAll);
		else if(fdEventGetFlag(FLAG_BRIGGS_CAUGHT))
			syFieldEventSetMessageId(
				(s32)MsgMadraRoomsIveHeardThatKibomboWield);
		else
			syFieldEventSetMessageId(
				(s32)MsgMadraRoomsEarthquakesPirateAttacksWhatWeek);
		/* "Earthquakes, pirate attacks... What a week!" */
		fdEventShowMessage(ACTOR_INNKEEPER, 0);
#else
		if(fdEventGetFlag(FLAG_GABOMBA_MESSAGE_CARRIED)) {
			s32 msg = (s32)MsgMadraRoomsAreYouWorriedAboutAll;

			syFieldEventSetMessageId(msg);
			fdEventOpenMessage(ACTOR_INNKEEPER, 0);
			if(syFieldEventAskYesNoWithActorAnimation(
				   syFieldPartyGetLeaderActor(), 0) == 0) {
				syFieldEventWait(10);
				syFieldEventSetMessageId(msg + 1);
			}
			else {
				syFieldEventWait(20);
				syFieldEventSetMessageId(msg + 2);
			}
			fdEventShowMessage(ACTOR_INNKEEPER, 0);
		}
		else {
			if(fdEventGetFlag(FLAG_BRIGGS_CAUGHT))
				syFieldEventSetMessageId(
					(s32)MsgMadraRoomsIveHeardThatKibomboWield);
			else
				syFieldEventSetMessageId(
					(s32)MsgMadraRoomsEarthquakesPirateAttacksWhatWeek);
			/* "Earthquakes, pirate attacks... What a week!" */
			fdEventShowMessage(ACTOR_INNKEEPER, 0);
		}
#endif
	}
}

/* Talking to the healer in the round sanctum from across his counter opens the
 */
/* sanctum's services; from any other side he talks of the town's trials, then
 */
/* of the Kibombo, and later of the warming sea. */
void scMadraRoomsTalkToHealer(s32 actor) {
	u16 facing = fdActorGet(MainRoutineStateWork.leaderActorId)->facing;

	if(FACING_NEAREST_CARDINAL(facing) == FACING_NORTH) {
		mnSanctumRun(actor);
	}
	else {
		if(fdEventGetFlag(FLAG_GABOMBA_MESSAGE_CARRIED))
			syFieldEventSetMessageId(
				(s32)MsgMadraRoomsIveNoticedLatelyThatTemperature);
		else if(fdEventGetFlag(FLAG_BRIGGS_CAUGHT))
			syFieldEventSetMessageId(
				(s32)MsgMadraRoomsKibomboAreTribeFromGondowan);
		else
			syFieldEventSetMessageId(
				(s32)MsgMadraRoomsSurelyThisTownIsEnduring);
		/* "Surely this town is enduring some sort of test... We must not
		 * fail..." */
		fdEventShowMessage(actor, 0);
	}
}

/* Felix goes to touch the Black Crystal on the mantel in the mayor's house: */
/* the auburn-haired woman by the table hops up, turns to him and cries "Don't
 */
/* touch that gem! The elder gave us strict orders to protect it!" */
void scMadraRoomsGuardElderGem(void) {
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	fdActorFaceActor(ACTOR_JEWEL_KEEPER, ACTOR_FELIX, 0);
	scFieldActorJump(ACTOR_JEWEL_KEEPER, 6, 20);
	fdActorFaceActor(ACTOR_FELIX, ACTOR_JEWEL_KEEPER, 0);
	syFieldEventSetMessageId((s32)MsgMadraRoomsDontTouchThatGemElder);
	/* "Don't touch that gem! The elder gave us strict orders to protect it!" */
	fdEventShowMessage(ACTOR_JEWEL_KEEPER, 0);
	fdActorFaceDirection(ACTOR_JEWEL_KEEPER, FACING_SOUTHEAST, 0);
	fdEventEnd();
}

/* Talking to the old man who loves the rare mushrooms of the Gondowan */
/* Cliffs, in the house south of the market: he wonders how to climb the cliffs
 */
/* to get one, and once he has the Healing Fungus, he looks forward to dinner,
 */
/* then says he ate too much. */
void scMadraRoomsTalkToMushroomLover(void) {
	s32 msg;

	if(fdEventGetFlag(FLAG_MADRA_HEALING_FUNGUS_GIVEN)) {
		if(fdEventGetFlag(FLAG_MADRA_FUNGUS_FOR_DINNER))
			msg = (s32)MsgMadraRoomsICantWaitForDinner;
		else
			msg = (s32)MsgMadraRoomsThatMushroomWasGreat;
		syFieldEventSetMessageId(msg);
		/* "That mushroom was great, but I ate way too much. I don't ever
		 * want..." */
		fdEventShowMessage(ACTOR_MUSHROOM_LOVER, 0);
	}
	else {
		if(fdEventGetFlag(FLAG_MADRA_LAUGHING_FUNGUS_OFFERED)) {
			syFieldEventSetMessageId((s32)MsgMadraRoomsThanksForBringing);
			/* "Thanks for bringing this by, but it's just not the type of..."
			 */
			fdEventShowMessage(ACTOR_MUSHROOM_LOVER, 0);
		}
		else {
			syFieldEventSetMessageId((s32)MsgMadraRoomsHowAreWeToClimb);
			/* "How are we to climb the cliffs between here and Gondowan? If..."
			 */
			fdEventShowMessage(ACTOR_MUSHROOM_LOVER, 0);
		}
		/* The field menu opens as he finishes: the player's chance to use a
		 * fungus */
		/* on him from the items (the USE_ITEM_ON_ACTOR triggers). */
		FieldStateWork->fieldMenu = 1;
	}
}

/* Talking to the mushroom lover's wife, in the same house: she tells of her */
/* husband's love of the rare mushrooms of the Gondowan Cliffs, begs the party
 */
/* to find the right one once he has turned down the Laughing Fungus, and */
/* after the Healing Fungus talks of cooking it. */
void scMadraRoomsTalkToMushroomCook(void) {
	if(fdEventGetFlag(FLAG_MADRA_HEALING_FUNGUS_GIVEN)) {
		if(fdEventGetFlag(FLAG_MADRA_FUNGUS_FOR_DINNER)) {
			syFieldEventSetMessageId(
				(s32)MsgMadraRoomsThisHeresEnoughOfMushroom);
			/* "This here's enough of a mushroom that we'll be eating it for
			 * days!" */
			fdEventShowMessage(ACTOR_MUSHROOM_COOK, 0);
		}
		else {
			syFieldEventSetMessageId(
				(s32)MsgMadraRoomsActuallyIDidntCookMushrooms);
			/* "Actually, I didn't cook the mushrooms properly that night.
			 * But..." */
			fdEventShowMessage(ACTOR_MUSHROOM_COOK, 0);
		}
	}
	else if(fdEventGetFlag(FLAG_MADRA_LAUGHING_FUNGUS_OFFERED)) {
		syFieldEventSetMessageId((s32)MsgMadraRoomsPleaseWontYouPleaseTry);
		/* "Please... Won't you please try to find the mushrooms my husband..."
		 */
		fdEventShowMessage(ACTOR_MUSHROOM_COOK, 0);
	}
	else {
		syFieldEventSetMessageId((s32)MsgMadraRoomsMyHusbandRelishesThoseRare);
		/* "My husband relishes those rare mushrooms growing in the Gondowan..."
		 */
		fdEventShowMessage(ACTOR_MUSHROOM_COOK, 0);
	}
}

/* Casting Mind Read on the mushroom lover: he thinks of the mushrooms he */
/* longs for, and once he has the Healing Fungus, of dinner. */
void scMadraRoomsMindReadMushroomLover(void) {
	if(fdEventGetFlag(FLAG_MADRA_HEALING_FUNGUS_GIVEN)) {
		if(fdEventGetFlag(FLAG_MADRA_FUNGUS_FOR_DINNER)) {
			syFieldEventSetMessageId((s32)MsgMadraRoomsMoreIThinkAboutDinner);
			/* "The more I think about dinner tonight, the more I drool..." */
			fdEventShowMessage(ACTOR_MUSHROOM_LOVER, 0);
		}
		else {
			syFieldEventSetMessageId(
				(s32)MsgMadraRoomsIDontRememberMushroomsLike);
			/* "I don't remember mushrooms like that ever tasting bad..." */
			fdEventShowMessage(ACTOR_MUSHROOM_LOVER, 0);
		}
	}
	else {
		s32 flag = fdEventGetFlag(FLAG_MADRA_LAUGHING_FUNGUS_OFFERED);
		s32 msg = (s32)MsgMadraRoomsImDyingForSome;

		if(flag) {
			syFieldEventSetMessageId(msg);
			fdEventShowMessage(ACTOR_MUSHROOM_LOVER, 0);
		}
		else {
			syFieldEventSetMessageId(msg);
			fdEventShowMessage(ACTOR_MUSHROOM_LOVER, 0);
		}
	}
}

/* Casting Mind Read on the mushroom lover's wife: she thinks of her husband */
/* growing old, and once he has the Healing Fungus, of cooking it. */
void scMadraRoomsMindReadMushroomCook(void) {
	if(fdEventGetFlag(FLAG_MADRA_HEALING_FUNGUS_GIVEN)) {
		if(fdEventGetFlag(FLAG_MADRA_FUNGUS_FOR_DINNER)) {
			syFieldEventSetMessageId(
				(s32)MsgMadraRoomsTheseMushroomsWillMakeAny);
			/* "These mushrooms will make any meal taste amazing!" */
			fdEventShowMessage(ACTOR_MUSHROOM_COOK, 0);
		}
		else {
			syFieldEventSetMessageId((s32)MsgMadraRoomsIfMyHusbandFindsOut);
			/* "If my husband finds out I messed up the recipe, he'll be so
			 * sad..." */
			fdEventShowMessage(ACTOR_MUSHROOM_COOK, 0);
		}
	}
	else {
		s32 flag = fdEventGetFlag(FLAG_MADRA_LAUGHING_FUNGUS_OFFERED);
		s32 msg = (s32)MsgMadraRoomsMyPoorHusbandsGettingOld;

		if(flag) {
			syFieldEventSetMessageId(msg);
			fdEventShowMessage(ACTOR_MUSHROOM_COOK, 0);
		}
		else {
			syFieldEventSetMessageId(msg);
			fdEventShowMessage(ACTOR_MUSHROOM_COOK, 0);
		}
	}
}

/* Talking to the man in the jail who has been trying for days to make ice as */
/* Piers did, once Felix brings Piers home: the first time on a visit he */
/* strains and groans "Uhhhnnn... UHHHNNN!", and afterwards he wonders how */
/* anyone makes ice just by thinking. */
void scMadraRoomsTalkToManFocusingOnIce(void) {
	if(!fdEventGetFlag(FLAG_MADRA_FOCUSING_MAN_GROANED)) {
		fdEventSetFlag(FLAG_MADRA_FOCUSING_MAN_GROANED);
		syFieldEventSetMessageId((s32)MsgMadraUhhhnnnUhhhnnn);
		fdActorWalkByAndWait(ACTOR_MAN_FOCUSING_ON_ICE, -12, 0);
		syFieldEventWait(20);
		fdActorRunRepeatedMotion(ACTOR_MAN_FOCUSING_ON_ICE, 2);
		syFieldEventWait(20);
		/* "Uhhhnnn... UHHHNNN!" */
		fdEventShowMessage(ACTOR_MAN_FOCUSING_ON_ICE, 0);
		scFieldActorShowEmote(ACTOR_MAN_FOCUSING_ON_ICE,
							  EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
		syFieldEventWait(20);
		fdActorWalkByAndWait(ACTOR_MAN_FOCUSING_ON_ICE, 12, 0);
		fdActorFaceActor(ACTOR_MAN_FOCUSING_ON_ICE, ACTOR_FELIX, 0);
		/* "They told me that you make Psynergy by focusing your mind...
		 * I've..." */
		fdEventShowMessage(ACTOR_MAN_FOCUSING_ON_ICE, 0);
	}
	else {
		syFieldEventSetMessageId((s32)MsgMadraRoomsHowCanAnyoneJustCreate);
		fdActorFaceActor(ACTOR_MAN_FOCUSING_ON_ICE, ACTOR_FELIX, 0);
		/* "How can anyone just create ice by thinking about it? I mean,
		 * imagine..." */
		fdEventShowMessage(ACTOR_MAN_FOCUSING_ON_ICE, 0);
	}
}

const TRIGGER *scGetTriggers(void) {
	if(MainRoutineStateWork.next == SCENE_MADRA_ROOMS_CAVERN_HOMES)
		return MadraRooms_SecondRoomsTriggers;
	if(MainRoutineStateWork.entrance == MADRA_ROOMS_HOUSES_FROM_MADRA_WEST)
		return MadraRooms_JailTriggers;
	if(fdEventGetFlag(FLAG_GABOMBA_MESSAGE_CARRIED))
		return MadraRooms_TriggersOnReturn;
	if(fdEventGetFlag(FLAG_BRIGGS_CAUGHT))
		return MadraRooms_TriggersAfterRaid;
	return MadraRooms_TriggersBeforeRaid;
}

/* The Black Crystal sits on the mantel in the mayor's house until the Kibombo
 */
/* steal it in their raid (FLAG_BRIGGS_CAUGHT), after which it is gone. */
void scMadraRoomsPlaceBlackCrystal(void) {
	if(fdEventGetFlag(FLAG_BRIGGS_CAUGHT)) {
		fdActorSetPos(ACTOR_BLACK_CRYSTAL, 888, 8);
	}
	else {
		fdActorSetItemIcon(fdActorGet(ACTOR_BLACK_CRYSTAL), ITEM_BLACK_CRYSTAL);
		fdActorSetSprFlags(fdActorGet(ACTOR_BLACK_CRYSTAL), 0);
	}
}

/* Places an actor facing an angle, the angle taken as the u16 an actor's */
/* facing is. */
static inline void scMadraRoomsPlaceFacing(s32 actorId, s32 x, s32 z,
										   u16 facing) {
	fdActorSetPosAndFacing(actorId, x, z, facing);
}

/* Sets up the jail: the first time Felix walks in, Shin angers Piers */
/* (scMadraRoomsShinAngersPiers); afterwards Shin and his friend are gone until
 */
/* the Kibombo's raid, when they are back in the jail. */
void scMadraRoomsSetUpJail(void) {
	if(!fdEventGetFlag(FLAG_BRIGGS_CAUGHT)) {
		FieldStateWork->psynergyDrawFlg =
			PSYNERGY_DRAW_NO_SCENE_SPRITE | PSYNERGY_DRAW_FIXED_PRIORITY;
		if(!fdEventGetFlag(FLAG_PIERS_INTERROGATED)) {
			scMadraRoomsShinAngersPiers();
		}
		else {
			fdActorSetPos(ACTOR_SHIN, 0, 0);
			fdActorSetPos(ACTOR_SHINS_FRIEND, 0, 0);
		}
	}
	else if(fdEventGetFlag(FLAG_GABOMBA_MESSAGE_CARRIED)) {
		scMadraRoomsPlaceFacing(ACTOR_SHIN, toFIXED(515), toFIXED(841),
								FACING_NORTHEAST);
		scMadraRoomsPlaceFacing(ACTOR_SHINS_FRIEND, toFIXED(472), toFIXED(877),
								FACING_NORTH);
		fdActorSetPos(ACTOR_JAIL_PUDDLE, 0, 0);
	}
	else {
		scMadraRoomsPlaceFacing(ACTOR_SHIN, toFIXED(515), toFIXED(841),
								FACING_NORTHWEST);
		scMadraRoomsPlaceFacing(ACTOR_SHINS_FRIEND, toFIXED(472), toFIXED(877),
								FACING_NORTH);
		fdActorSetPos(ACTOR_JAIL_PUDDLE, 0, 0);
	}
}

extern s32 MadraRooms_ShinAndFriendRunAway[];

/* Sets up Madra's buildings as the party walks in: Morgan and Dekka at the inn
 */
/* only for a party that won Colosso in The Broken Seal, the jail, the sister */
/* lying ill in bed in the left cavern home until Briggs is caught, and the */
/* Black Crystal. */
s32 scInit(void) {
	FIELD_SPR *sprite;
	FIELD_STATE *event;

	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
	fdActorGet(MainRoutineStateWork.leaderActorId)->priFlg |=
		ACTOR_PRIORITY_IGNORES_RISES;
	fdEffectStartBlankOverlay(0);
	if(MainRoutineStateWork.next == SCENE_MADRA_ROOMS_HOUSES) {
		event = Ram_HeapSlots->fieldState;
		event->psynergyDrawFlg =
			PSYNERGY_DRAW_NO_SCENE_SPRITE | PSYNERGY_DRAW_FIXED_PRIORITY;
		if(!fdEventGetFlag(FLAG_TRANSFERRED_WON_COLOSSO)) {
			fdActorSetPos(ACTOR_MORGAN, 0, 0);
			fdActorSetPos(ACTOR_DEKKA, 0, 0);
		}
		else {
			scFieldActorSetSprCol(fdActorGet(ACTOR_MORGAN), SPRITE_COLOR_BGR);
			scFieldActorSetSprCol(fdActorGet(ACTOR_DEKKA), SPRITE_COLOR_BRG);
		}
		if(fdEventGetFlag(FLAG_BRIGGS_CAUGHT)) {
			fdMapGridCopyCellsLayered(2, 110, 16, 10, 16, 64);
			fdMapGridCopyCellAttributes(2, 46, 16, 10, 16, 0);
		}
	}
	if(MainRoutineStateWork.entrance == MADRA_ROOMS_HOUSES_FROM_MADRA_WEST)
		scMadraRoomsSetUpJail();
	if(MainRoutineStateWork.next == SCENE_MADRA_ROOMS_CAVERN_HOMES) {
		if(fdEventGetFlag(FLAG_BRIGGS_CAUGHT)) {
			fdActorSetPos(ACTOR_SISTER_IN_BED, 0, 0);
		}
		else {
			fdActorSetSprRotationById(ACTOR_SISTER_IN_BED, TRIG_TURN * 3 / 4);
			fdActorSetSprFlags(fdActorGet(ACTOR_SISTER_IN_BED), 0);
			/* Lying under the covers: her sprite turned, its bottom 8 rows
			 * sunk. */
			((FIELD_SPR *)fdActorGet(ACTOR_SISTER_IN_BED)->spr)->flg |=
				SPRITE_FLAG_SUNK;
			sprite = fdActorGet(ACTOR_SISTER_IN_BED)->spr;
			sprite->sunkRows = 8;
			((FIELD_SPR *)fdActorGet(ACTOR_SISTER_IN_BED)->spr)->dirty = 1;
		}
	}
	scMadraRoomsPlaceBlackCrystal();
	if(fdEventGetFlag(FLAG_GABOMBA_MESSAGE_CARRIED))
		scFieldActorSetSprCol(fdActorGet(ACTOR_TRAVELERS_PARTNER_ON_RETURN),
							  SPRITE_COLOR_RBG);
	else
		scFieldActorSetSprCol(fdActorGet(ACTOR_TRAVELERS_PARTNER),
							  SPRITE_COLOR_RBG);
	return 0;
}

s32 scPrepareMap(void) { return 0; }

/* Turns an actor's sprite a thirty-second of a turn, a frame of Shin falling */
/* over (MadraRooms_ShinFalls). */
s32 scMadraRoomsRotateSpr(ACTOR *actor) {
	FIELD_SPR *sprite;

	sprite = actor->spr;
	if(actor != NULL && sprite != NULL)
		sprite->rotation -= TRIG_TURN / 32;
	return 0;
}

/* Unused: stands an actor's sprite upright again; no script calls it. */
UNUSED s32 scMadraRoomsResetSprRotation(ACTOR *actor) {
	FIELD_SPR *sprite;

	sprite = actor->spr;
	if(actor != NULL && sprite != NULL)
		sprite->rotation = 0;
	return 0;
}

/* The party enters the jail as Shin and his friend accuse Piers of being one */
/* of the Champa, and sees Piers, angered, lift Shin on a shaft of ice. */
void scMadraRoomsShinAngersPiers(void) {
	s32 i;
	ACTOR *felix;

	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	fdActorSetAnimation(ACTOR_JAIL_PUDDLE, PUDDLE_ANIMATION_PUDDLE);
	fdActorSetPos(ACTOR_FELIX, 400, 888);
	fdCameraMoveTo(408, CAMERA_KEEP, 848, 1);
	fdEventStartScrTransition();
	fdEventWaitTransitionFrames();
	fdActorSetSpeed(ACTOR_PIERS, 0.8);
	syFieldEventSetMessageId((s32)MsgMadraRoomsAdmitItYoureOne);
	/* "Admit it! You're one of the Champa, aren't you, Piers?" */
	syFieldEventShowMessageThenWait(ACTOR_SHINS_FRIEND, 0, 10);
	fdActorWalkToAndWait(ACTOR_FELIX, 396, 869);
	scFieldActorLaunchFromLeader(ACTOR_JAIL_KRADEN, 25, 19, FACING_NORTH);
	scFieldActorWaitForScriptEndById(ACTOR_JAIL_KRADEN);
	fdActorFaceDirection(ACTOR_FELIX, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_JAIL_KRADEN, FACING_WEST, 0);
	syFieldEventWait(20);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_JAIL_KRADEN, FACING_NORTHEAST, 0);
	syFieldEventWait(10);
	fdCameraMoveTo(488, CAMERA_KEEP, 820, 1);
	fdCameraWaitForArrival();
	syFieldEventWait(20);
	fdActorRunRepeatedMotion(ACTOR_PIERS, 2);
	syFieldEventWait(20);
	fdActorShowEmote(ACTOR_SHIN, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	/* "Speak! Or let your silence condemn you!" */
	syFieldEventShowMessageThenWait(ACTOR_SHIN, 0, 10);
	syFieldEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_SHAKE_HEAD);
	/* "I told you already... I'm not a Champa." */
	syFieldEventShowMessageThenWait(ACTOR_PIERS, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_SHIN, 2);
	/* "Then where did you come from?" */
	syFieldEventShowMessageThenWait(ACTOR_SHIN, 0, 10);
	fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_NORTH);
	fdActorWalkByAndWait(ACTOR_PIERS, 0, -16);
	syFieldEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_SHAKE_HEAD);
	/* "The heart of the Eastern Sea... If I told you where, you'd never..." */
	syFieldEventShowMessageThenWait(ACTOR_PIERS, 0, 10);
	scFieldActorShowEmote(ACTOR_SHIN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
	fdActorGet(ACTOR_SHIN)->steeringFlg &= ~ACTOR_FACES_MOTION;
	for(i = 0; i < 3; i++) {
		fdActorWalkByAndWait(ACTOR_SHIN, 0, -8);
		fdEventPlaySnd(SE_HEAVY_IMPACT);
		fdActorWalkByAndWait(ACTOR_SHIN, 0, 8);
	}
	fdActorGet(ACTOR_SHIN)->steeringFlg |= ACTOR_FACES_MOTION;
	syFieldEventWait(30);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	/* "Look at me when I'm talking to you! Are you trying to mock me!?" */
	syFieldEventShowMessageThenWait(ACTOR_SHIN, 0, 10);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
	syFieldEventWait(20);
	/* "I implore you, do not anger me." */
	syFieldEventShowMessageThenWait(ACTOR_PIERS, 0, 10);
	fdActorShowEmote(ACTOR_SHIN, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "Oh, do you? So what's going to happen if I make you angry?" */
	syFieldEventShowMessageThenWait(ACTOR_SHIN, 0, 10);
	fdActorFaceActor(ACTOR_SHINS_FRIEND, ACTOR_SHIN, 0);
	syFieldEventWait(20);
	fdActorRunRepeatedMotion(ACTOR_SHINS_FRIEND, 2);
	/* "Hey, Shin... Maybe you should go easy on him..." */
	syFieldEventShowMessageThenWait(ACTOR_SHINS_FRIEND, 0, 10);
	fdActorFaceActor(ACTOR_SHIN, ACTOR_SHINS_FRIEND, 0);
	syFieldEventWait(20);
	fdActorRunRepeatedMotion(ACTOR_SHIN, 2);
	/* "Hey, it wasn't your girlfriend who got hurt!" */
	syFieldEventShowMessageThenWait(ACTOR_SHIN, 0, 10);
	syFieldEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_SHAKE_HEAD);
	/* "I'm sorry she was injured, but I cannot°" */
	syFieldEventShowMessageThenWait(ACTOR_PIERS, 0, 10);
	scFieldActorShowEmote(ACTOR_SHIN, EMOTE_IN_FRONT | EMOTE_SCOWL, 20);
	fdActorFaceDirectionAndWait(ACTOR_SHIN, FACING_NORTH);
	/* "I don't want your sympathy, freak! I want you to get angry for me!" */
	syFieldEventShowMessageThenWait(ACTOR_SHIN, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_PIERS, 2);
	syFieldEventWait(20);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
	/* "Stop, please... Even my patience has its limits." */
	syFieldEventShowMessageThenWait(ACTOR_PIERS, 0, 10);
	fdActorFaceDirection(ACTOR_SHIN, FACING_SOUTH, 0);
	syFieldEventWait(20);
	/* "Hey, don't you want to see what he'll do?" */
	syFieldEventShowMessageThenWait(ACTOR_SHIN, 0, 10);
	fdActorShowEmote(ACTOR_SHINS_FRIEND, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	/* "I think we should just stop..." */
	syFieldEventShowMessageThenWait(ACTOR_SHINS_FRIEND, 0, 10);
	/* "If my words will not cool your temper, then..." */
	syFieldEventShowMessageThenWait(ACTOR_PIERS, 0, 10);
	syFieldEventWait(20);
	fdActorWalkByAndWait(ACTOR_PIERS, 0, 16);
	fdPsynergyLoadResources(ABILITY_FROST, 1);
	fdPsynergySetupActorPair(ACTOR_PIERS, ACTOR_JAIL_PUDDLE);
	fdPsynergyStartCast();
	fdActorShowEmote(ACTOR_SHIN, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	fdActorFaceActor(ACTOR_SHIN, ACTOR_PIERS, 0);
	fdActorFaceActor(ACTOR_SHINS_FRIEND, ACTOR_PIERS, 0);
	fdActorSetSprFlags(fdActorGet(ACTOR_SHIN), 0);
	fdActorSetScript(ACTOR_SHIN, MadraRooms_ShinLifted);
	fdPsynergyRunHandler(1);
	fdBattleEffectStopMainActor();
	fdPsynergyEnd();
	/* "Oh! That was Psynergy!" */
	syFieldEventShowMessageThenWait(ACTOR_JAIL_KRADEN, 0, 10);
	scFieldActorStop(ACTOR_SHIN);
	fdActorGet(ACTOR_SHIN)->scaleX = FIXED_ONE;
	fdActorGet(ACTOR_SHIN)->scaleY = FIXED_ONE;
	syFieldEventWait(4);
	fdActorGet(ACTOR_SHIN)->motionFlg = ACTOR_FALLS;
	fdActorGet(ACTOR_SHIN)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorSetDestinationOffset(ACTOR_SHIN, -8, 0);
	fdActorSetScript(ACTOR_SHIN, MadraRooms_ShinFalls);
	scFieldActorWaitForScriptEndById(ACTOR_SHIN);
	fdEventPlaySnd(SE_LANDING_THUD);
	syFieldEventWait(60);
	fdActorGet(ACTOR_SHIN)->steeringFlg |= ACTOR_FACES_MOTION;
	scFieldActorJump(ACTOR_SHIN, 6, 0);
	syFieldEventWait(3);
	fdActorSetSprRotationById(ACTOR_SHIN, 0);
	fdActorSetSprFlags(fdActorGet(ACTOR_SHIN), 1);
	fdActorFaceDirection(ACTOR_SHIN, FACING_SOUTH, 0);
	syFieldEventWait(20);
	fdActorFaceDirection(ACTOR_SHINS_FRIEND, FACING_EAST, 0);
	syFieldEventWait(20);
	fdActorFaceDirection(ACTOR_SHINS_FRIEND, FACING_NORTH, 0);
	syFieldEventWait(20);
	scFieldActorShowEmote(ACTOR_SHINS_FRIEND,
						  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	fdActorRunRepeatedMotion(ACTOR_SHIN, 2);
	syFieldEventWait(20);
	syFieldEventWait(10);
	scFieldActorJump(ACTOR_SHIN, 6, 20);
	syFieldEventWait(20);
	fdActorFaceActor(ACTOR_SHIN, ACTOR_PIERS, 0);
	syFieldEventWait(20);
	fdActorGet(ACTOR_SHINS_FRIEND)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorGet(ACTOR_SHIN)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkByAndWait(ACTOR_SHIN, -16, 16);
	fdActorRunRepeatedMotion(ACTOR_PIERS, 2);
	syFieldEventWait(20);
	fdActorFaceDirection(ACTOR_SHIN, FACING_SOUTH, 0);
	syFieldEventWait(20);
	syFieldEventWait(10);
	scFieldActorJump(ACTOR_SHIN, 6, 20);
	syFieldEventWait(20);
	fdActorGet(ACTOR_SHIN)->steeringFlg |= ACTOR_FACES_MOTION;
	fdActorSetScript(ACTOR_SHIN, MadraRooms_ShinAndFriendRunAway);
	scFieldActorWatchAndSetScript(ACTOR_FELIX, ACTOR_SHIN);
	scFieldActorWatchAndSetScript(ACTOR_JAIL_KRADEN, ACTOR_SHIN);
	/* "Waaaaaaaaaahh!" */
	syFieldEventShowMessageThenWait(ACTOR_SHIN, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_SHINS_FRIEND, 2);
	syFieldEventWait(20);
	fdActorFaceActor(ACTOR_SHINS_FRIEND, ACTOR_PIERS, 0);
	scFieldActorWatchAndSetScript(ACTOR_FELIX, ACTOR_SHINS_FRIEND);
	scFieldActorWatchAndSetScript(ACTOR_JAIL_KRADEN, ACTOR_SHINS_FRIEND);
	/* "Did you do that?" */
	syFieldEventShowMessageThenWait(ACTOR_SHINS_FRIEND, 0, 10);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
	/* "What did your friend expect?" */
	syFieldEventShowMessageThenWait(ACTOR_PIERS, 0, 10);
	fdActorShowEmote(ACTOR_SHINS_FRIEND, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	fdActorWalkByAndWait(ACTOR_SHINS_FRIEND, -16, 16);
	/* "You monster!" */
	syFieldEventShowMessageThenWait(ACTOR_SHINS_FRIEND, 0, 1);
	fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_SHAKE_HEAD);
	/* "I... I am no monster." */
	syFieldEventShowMessageThenWait(ACTOR_PIERS, 0, 10);
	syFieldEventWait(20);
	fdActorWalkByAndWait(ACTOR_SHINS_FRIEND, -16, 8);
	syFieldEventWait(20);
	fdActorRunRepeatedMotion(ACTOR_SHINS_FRIEND, 2);
	syFieldEventWait(20);
	/* "Help!!!" */
	syFieldEventShowMessageThenWait(ACTOR_SHINS_FRIEND, 0, 10);
	fdActorGet(ACTOR_SHINS_FRIEND)->steeringFlg |= ACTOR_FACES_MOTION;
	fdActorSetScript(ACTOR_SHINS_FRIEND, MadraRooms_ShinAndFriendRunAway);
	syFieldEventWait(40);
	scFieldActorStop(ACTOR_FELIX);
	scFieldActorStop(ACTOR_JAIL_KRADEN);
	scFieldActorWaitForScriptEndById(ACTOR_SHINS_FRIEND);
	fdActorSetPos(ACTOR_SHIN, 0, 0);
	fdActorSetPos(ACTOR_SHINS_FRIEND, 0, 0);
	fdActorSetAnimation(ACTOR_JAIL_PUDDLE, 6);
	fdActorSetSprColById(ACTOR_JAIL_PUDDLE, SPRITE_COLOR_NORMAL);
	fdActorFaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
	fdActorWaitForAnimationChange(8);
	syEventCameraFollowActor(ACTOR_FELIX, 1);
	fdCameraWaitForArrival();
	fdActorFaceEachOther(ACTOR_JAIL_KRADEN, ACTOR_FELIX, 0);
	fdActorRunRepeatedMotion(ACTOR_JAIL_KRADEN, 2);
	syFieldEventWait(20);
	/* "This man... He must be an Adept!" Yes: "Of course! That was clearly */
	/* Psynergy he just used..." No: "You're right... We only know what we..."
	 */
	fdEventOpenMessage(ACTOR_JAIL_KRADEN, 0);
	if(syFieldEventAskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
		fdActorRunRepeatedMotion(ACTOR_JAIL_KRADEN, 2);
		/* "Of course! That was clearly Psynergy he just used..." */
		syFieldEventShowMessageThenWait(ACTOR_JAIL_KRADEN, 0, 10);
		FieldStateWork->msg += 1;
	}
	else {
		fdActorSetAnimationAndWait(ACTOR_JAIL_KRADEN, ANIM_SHAKE_HEAD);
		FieldStateWork->msg += 1;
		/* "You're right... We only know what we just saw." */
		syFieldEventShowMessageThenWait(ACTOR_JAIL_KRADEN, 0, 10);
	}
	fdActorRunRepeatedMotion(ACTOR_JAIL_KRADEN, 2);
	syFieldEventWait(10);
	/* "Perhaps we should try to learn a little more about him." */
	syFieldEventShowMessageThenWait(ACTOR_JAIL_KRADEN, 0, 10);
	fdActorSetAnimation(ACTOR_FELIX, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_JAIL_KRADEN, ANIM_NOD);
	syFieldEventWait(10);
	fdActorSetAnimation(ACTOR_JAIL_KRADEN, ANIM_WALK);
	felix = fdActorGet(ACTOR_FELIX);
	if(felix != NULL)
		fdActorSetDestination(ACTOR_JAIL_KRADEN, felix->x.part[0].integer,
							  felix->z.part[0].integer);
	scFieldActorWaitForMoveById(ACTOR_JAIL_KRADEN);
	fdActorSetPos(ACTOR_JAIL_KRADEN, 0, 0);
	fdEventSetFlag(FLAG_PIERS_INTERROGATED);
	fdEventEnd();
}

/* Talking to Piers in his cell in Madra's jail: "Ah... When will that kindly */
/* elder return and end my imprisonment? I have no time to wait, but I don't */
/* want to have to hurt anyone..." */
void scMadraRoomsTalkToPiers(void) {
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	syFieldEventSetMessageId((s32)MsgMadraRoomsAhWhenWillThatKindly);
	fdActorFaceDirection(ACTOR_JAIL_KRADEN, FACING_SOUTH, 0);
	/* "Ah... When will that kindly elder return and end my imprisonment? I..."
	 */
	syFieldEventShowMessageThenWait(ACTOR_PIERS, 0, 10);
	fdEventEnd();
}

/* Casting Mind Read on Piers in his cell: he regrets using his Psynergy in */
/* anger, then senses someone using Psynergy and looks about for them, and */
/* wonders when he will be freed. */
void scMadraRoomsMindReadPiers(void) {
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	syFieldEventSetMessageId((s32)MsgMadraRoomsWhyDidIUse);
	/* "Why did I use my Psynergy in anger? I must maintain control... My..." */
	syFieldEventShowMessageThenWait(ACTOR_PIERS, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_PIERS, 2);
	syFieldEventWait(20);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
	syFieldEventWait(20);
	/* "I...sense someone... using Psynergy..." */
	syFieldEventShowMessageThenWait(ACTOR_PIERS, 0, 10);
	syFieldEventWait(10);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
	syFieldEventWait(20);
	fdActorFaceDirection(ACTOR_PIERS, FACING_EAST, 0);
	syFieldEventWait(10);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
	syFieldEventWait(10);
	fdActorFaceDirection(ACTOR_PIERS, FACING_WEST, 0);
	syFieldEventWait(10);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
	syFieldEventWait(20);
	/* "That person... Could he be°no, he is not of my people..." */
	syFieldEventShowMessageThenWait(ACTOR_PIERS, 0, 10);
	fdActorFaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
	syFieldEventWait(20);
	/* "When will I be freed?" */
	syFieldEventShowMessageThenWait(ACTOR_PIERS, 0, 10);
	fdEventEnd();
}

/* Unused: Piers' Frost at the jail's puddle alone, as */
/* scMadraRoomsShinAngersPiers plays it; nothing calls it. */
UNUSED void scMadraRoomsPiersUsesPsynergy(void) {
	fdPsynergyLoadResources(ABILITY_FROST, 1);
	fdPsynergySetupActorPair(ACTOR_PIERS, ACTOR_JAIL_PUDDLE);
	fdPsynergyStartCast();
	fdPsynergyRunHandler(1);
	fdBattleEffectStopMainActor();
	fdPsynergyEnd();
}

extern s32 MadraRooms_WifeWalksToBackRoom[];
extern s32 MadraRooms_LeaderStepsAside[];

/* Felix gives the old mushroom lover the Healing Fungus from the Gondowan */
/* Cliffs: "Oh, my... Is... Is that..." He and his wife go to their back room */
/* and come back with Char, the Mars Djinni, who joins the party. */
void scMadraRoomsGiveHealingFungus(void) {
	ACTOR *actor;

	fdEventSetFlag(FLAG_MADRA_HEALING_FUNGUS_GIVEN);
	fdEventSetFlag(FLAG_MADRA_FUNGUS_FOR_DINNER);
	gmPartyInventoryRemove(ITEM_HEALING_FUNGUS);
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	fdActorSetSpeed(ACTOR_MUSHROOM_COOK, 0.4);
	fdActorWalkTo(ACTOR_MUSHROOM_COOK, 410, 109);
	actor = fdActorGet(ACTOR_FELIX);
	if(actor->z.fixed > toFIXED(95))
		fdActorWalkToAndWait(ACTOR_FELIX, actor->x.fixed / FIXED_ONE, 95);
	fdActorWalkToAndWait(ACTOR_FELIX, 362, 95);
	fdActorFaceDirectionAndWait(ACTOR_FELIX, FACING_SOUTH);
	scFieldActorWaitForMoveById(ACTOR_MUSHROOM_COOK);
	syFieldEventSetMessageId((s32)MsgMadraRoomsOhMy);
	fdActorFaceActor(ACTOR_MUSHROOM_LOVER, ACTOR_FELIX, 0);
	syFieldEventWait(20);
	fdActorShowEmote(ACTOR_MUSHROOM_LOVER, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	/* "Oh, my... Is... Is that..." */
	syFieldEventShowMessageThenWait(ACTOR_MUSHROOM_LOVER, 0, 10);
	fdActorShowEmote(ACTOR_MUSHROOM_COOK, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
					 3);
	fdActorWalkToAndWait(ACTOR_MUSHROOM_COOK, 378, 109);
	fdActorFaceDirection(ACTOR_MUSHROOM_COOK, FACING_NORTHWEST, 0);
	syFieldEventWait(20);
	/* "Is that one of his most favorite mushrooms!?" */
	syFieldEventShowMessageThenWait(ACTOR_MUSHROOM_COOK, 0, 10);
	fdActorFaceEachOther(ACTOR_MUSHROOM_LOVER, ACTOR_MUSHROOM_COOK, 0);
	syFieldEventWait(20);
	fdActorFaceActor(ACTOR_MUSHROOM_LOVER, ACTOR_FELIX, 0);
	syFieldEventWait(20);
	fdActorShowEmote(ACTOR_MUSHROOM_LOVER, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "Did you climb all the way up those cliffs just to get that mushroom..."
	 */
	syFieldEventShowMessageThenWait(ACTOR_MUSHROOM_LOVER, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
	syFieldEventWait(20);
	fdActorShowEmote(ACTOR_MUSHROOM_COOK, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	/* "That's the nicest thing anyone has ever done for me. I have to thank..."
	 */
	syFieldEventShowMessageThenWait(ACTOR_MUSHROOM_COOK, 0, 10);
	fdActorFaceActor(ACTOR_MUSHROOM_LOVER, ACTOR_MUSHROOM_COOK, 0);
	syFieldEventWait(30);
	fdActorSetAnimationAndWait(ACTOR_MUSHROOM_LOVER, ANIM_NOD);
	syFieldEventWait(30);
	fdActorShowEmote(ACTOR_MUSHROOM_LOVER, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	/* "But what could I have that a warrior like you would want?" */
	syFieldEventShowMessageThenWait(ACTOR_MUSHROOM_LOVER, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_MUSHROOM_COOK, 2);
	syFieldEventWait(20);
	/* "That's a tricky one..." */
	syFieldEventShowMessageThenWait(ACTOR_MUSHROOM_COOK, 0, 10);
	fdActorFaceDirectionAndWait(ACTOR_MUSHROOM_COOK, FACING_SOUTH);
	syFieldEventWait(10);
	fdActorFaceDirectionAndWait(ACTOR_MUSHROOM_COOK, FACING_NORTH);
	syFieldEventWait(10);
	fdActorFaceDirectionAndWait(ACTOR_MUSHROOM_COOK, FACING_WEST);
	syFieldEventWait(10);
	fdActorShowEmote(ACTOR_MUSHROOM_COOK, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB,
					 30);
	scFieldActorStop(ACTOR_CHAR);
	fdActorSetSpeed(ACTOR_MUSHROOM_LOVER, 0.5);
	fdActorSetSpeed(ACTOR_MUSHROOM_COOK, 0.5);
	fdActorSetSpeed(ACTOR_CHAR, 0.5);
	fdActorSetScript(ACTOR_MUSHROOM_COOK, MadraRooms_WifeWalksToBackRoom);
	fdActorSetScript(ACTOR_FELIX, MadraRooms_LeaderStepsAside);
	syFieldEventWait(60);
	fdActorSetScript(ACTOR_MUSHROOM_LOVER, MadraRooms_OldManWalksToBackRoom);
	scFieldActorWaitForScriptEndById(ACTOR_MUSHROOM_LOVER);
	syFieldEventWait(120);
	fdActorSetScript(ACTOR_MUSHROOM_COOK, MadraRooms_WifeComesBack);
	scFieldActorWatchAndSetScript(ACTOR_FELIX, ACTOR_MUSHROOM_COOK);
	syFieldEventWait(60);
	fdActorSetScript(ACTOR_MUSHROOM_LOVER, MadraRooms_OldManComesBack);
	fdActorSetScript(ACTOR_CHAR, MadraRooms_CharFollows);
	scFieldActorWaitForScriptEndById(ACTOR_MUSHROOM_LOVER);
	scFieldActorWaitForScriptEndById(ACTOR_MUSHROOM_COOK);
	scFieldActorWaitForScriptEndById(ACTOR_CHAR);
	fdActorRunRepeatedMotion(ACTOR_MUSHROOM_COOK, 2);
	syFieldEventWait(20);
	/* "I don't know if you want a pet, but this is all we can offer. Take..."
	 */
	syFieldEventShowMessageThenWait(ACTOR_MUSHROOM_COOK, 0, 10);
	fdDjinnJoinParty(ACTOR_CHAR, DJINNI_CHAR / DJINN_PER_ELEMENT,
					 DJINNI_INDEX(DJINNI_CHAR));
	FieldStateWork->msg += 1;
	fdActorSetAnimation(ACTOR_FELIX, ANIM_STAND);
	fdActorFaceDirection(ACTOR_MUSHROOM_LOVER, FACING_SOUTH, 0);
	syFieldEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_MUSHROOM_LOVER, ANIM_NOD);
	syFieldEventWait(20);
	fdActorFaceDirection(ACTOR_FELIX, FACING_EAST, 0);
	/* "I can't wait for dinner tonight. Batter-fried mushrooms! Mmm!" */
	syFieldEventShowMessageThenWait(ACTOR_MUSHROOM_LOVER, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_MUSHROOM_COOK, ANIM_NOD);
	syFieldEventWait(20);
	/* "This here's enough of a mushroom that we'll be eating it for days!" */
	syFieldEventShowMessageThenWait(ACTOR_MUSHROOM_COOK, 0, 10);
	fdEventEnd();
}

/* Felix gives the old mushroom lover the Laughing Fungus: he tells his wife */
/* "No, dear... They look similar, but these aren't quite right." and turns */
/* it down. */
void scMadraRoomsGiveLaughingFungus(void) {
	ACTOR *felix;

	fdEventSetFlag(FLAG_MADRA_LAUGHING_FUNGUS_OFFERED);
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	felix = fdActorGet(ACTOR_FELIX);
	if(felix->z.fixed > toFIXED(95))
		fdActorWalkToAndWait(ACTOR_FELIX, felix->x.fixed / FIXED_ONE, 95);
	fdActorWalkToAndWait(ACTOR_FELIX, 362, 95);
	fdActorFaceDirectionAndWait(ACTOR_FELIX, FACING_SOUTH);
	syFieldEventSetMessageId((s32)MsgMadraRoomsOhMy);
	fdActorFaceActor(ACTOR_MUSHROOM_LOVER, ACTOR_FELIX, 0);
	syFieldEventWait(30);
	fdActorShowEmote(ACTOR_MUSHROOM_LOVER, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	/* "Oh, my... Is... Is that..." */
	syFieldEventShowMessageThenWait(ACTOR_MUSHROOM_LOVER, 0, 5);
	fdActorShowEmote(ACTOR_MUSHROOM_COOK, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
					 3);
	fdActorFaceActor(ACTOR_MUSHROOM_COOK, ACTOR_FELIX, 0);
	syFieldEventWait(20);
	/* "Is that one of his most favorite mushrooms!?" */
	syFieldEventShowMessageThenWait(ACTOR_MUSHROOM_COOK, 0, 5);
	syFieldEventSetMessageId((s32)MsgMadraRoomsNoDearTheyLookSimilar);
	fdActorShowEmote(ACTOR_MUSHROOM_LOVER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
					 30);
	fdActorFaceDirectionAndWait(ACTOR_MUSHROOM_LOVER, FACING_EAST);
	syFieldEventWait(10);
	/* "No, dear... They look similar, but these aren't quite right." */
	syFieldEventShowMessageThenWait(ACTOR_MUSHROOM_LOVER, 0, 5);
	fdActorFaceEachOther(ACTOR_MUSHROOM_LOVER, ACTOR_MUSHROOM_COOK, 0);
	syFieldEventWait(20);
	fdActorFaceActor(ACTOR_MUSHROOM_LOVER, ACTOR_FELIX, 0);
	syFieldEventWait(20);
	fdActorShowEmote(ACTOR_MUSHROOM_LOVER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	syFieldEventWait(20);
	/* "Thanks for bringing this by, but it's just not the type of mushroom..."
	 */
	syFieldEventShowMessageThenWait(ACTOR_MUSHROOM_LOVER, 0, 5);
	fdActorFaceActor(ACTOR_MUSHROOM_COOK, ACTOR_FELIX, 0);
	syFieldEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_MUSHROOM_COOK, 2);
	syFieldEventWait(5);
	/* "Please... Won't you please try to find the mushrooms my husband..." */
	syFieldEventShowMessageThenWait(ACTOR_MUSHROOM_COOK, 0, 5);
	fdEventEnd();
}

/* A party member walks into Felix and vanishes, as the party gathers up to */
/* leave the mayor's house. */
static inline void scMadraRoomsWalkIntoLeader(s32 actorId) {
	ACTOR *leader;

	fdActorSetSpeed(actorId, 1.2);
	fdActorSetAnimation(actorId, ANIM_WALK);
	leader = fdActorGet(ACTOR_FELIX);
	if(leader != NULL)
		fdActorSetDestination(actorId, leader->x.part[0].integer,
							  leader->z.part[0].integer);
	scFieldActorWaitForMoveById(actorId);
	fdActorSetPos(actorId, 0, 0);
}

/* Walking into the mayor's house when Felix brings Piers home (cells (44, 5) */
/* and (44, 6), a few steps in from the door), the mayor cries "Felix! You're */
/* back!" and his household welcomes Piers home, asking after his jewel and */
/* apologising for his imprisonment. */
void scMadraRoomsWelcomePiersHome(void) {
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	syFieldEventSetMessageId((s32)MsgMadraRoomsFelixYoureBack);
	fdActorShowEmote(ACTOR_MAYOR, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	fdActorFaceActor(ACTOR_MAYOR, ACTOR_FELIX, 0);
	/* "Felix! You're back!" */
	syFieldEventShowMessageThenWait(ACTOR_MAYOR, 0, 10);
	fdActorFaceActor(ACTOR_ELDERS_DAUGHTER, ACTOR_FELIX, 0);
	fdActorFaceDirection(ACTOR_ELDERS_GRANDSON, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceActor(ACTOR_ELDERS_GRANDDAUGHTER, ACTOR_FELIX, 0);
	fdActorFaceActor(ACTOR_JEWEL_KEEPER, ACTOR_FELIX, 0);
	fdActorFaceActor(ACTOR_ELDERS_DAUGHTER, ACTOR_FELIX, 0);
	fdActorFaceActor(ACTOR_OLD_WOMAN_OF_THE_ELDERS_HOUSE, ACTOR_FELIX, 0);
	syFieldEventWait(20);
	fdActorWalkToAndWait(ACTOR_FELIX, 701, 93);
	fdActorWalkBy(ACTOR_FELIX, -32, 0);
	scFieldActorLaunchFromLeader(ACTOR_JENNA, -8, 32, FACING_WEST);
	scFieldActorLaunchFromLeader(ACTOR_KRADEN, -16, 0, FACING_SOUTH_SOUTHWEST);
	scFieldActorLaunchFromLeader(ACTOR_SHEBA, -8, 16, FACING_SOUTHWEST);
	scFieldActorLaunchFromLeader(ACTOR_PIERS, -24, 16, FACING_SOUTHWEST);
	syFieldEventWait(30);
	fdActorShowEmote(ACTOR_ELDERS_DAUGHTER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
					 30);
	/* "And you've brought Piers with you!" */
	syFieldEventShowMessageThenWait(ACTOR_ELDERS_DAUGHTER, 0, 10);
	fdActorShowEmote(ACTOR_ELDER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	syFieldEventWait(20);
	fdActorFaceDirection(ACTOR_ELDER, FACING_EAST, 0);
	syFieldEventWait(20);
	fdActorRunRepeatedMotion(ACTOR_ELDER, 2);
	syFieldEventWait(20);
	/* "Oh! Piers!" */
	syFieldEventShowMessageThenWait(ACTOR_ELDER, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
	syFieldEventWait(20);
	fdActorShowEmote(ACTOR_ELDER, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	/* "Piers, I must apologize for the trouble we've caused you. I'm sorry..."
	 */
	syFieldEventShowMessageThenWait(ACTOR_ELDER, 0, 10);
	fdActorFaceEachOther(ACTOR_PIERS, ACTOR_FELIX, 0);
	syFieldEventWait(30);
	fdActorFaceDirection(ACTOR_PIERS, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTHWEST, 0);
	syFieldEventWait(20);
	/* "Don't be. Everything worked out for the best." */
	syFieldEventShowMessageThenWait(ACTOR_PIERS, 0, 10);
	scFieldActorJump(ACTOR_ELDERS_GRANDDAUGHTER, 6, 20);
	syFieldEventWait(20);
	/* "Piers, you said you were going to go to Gondowan. Did you make it?" */
	syFieldEventShowMessageThenWait(ACTOR_ELDERS_GRANDDAUGHTER, 0, 10);
	fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTH_SOUTHEAST);
	fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
	syFieldEventWait(20);
	/* "Yes. In fact, we have just returned." */
	syFieldEventShowMessageThenWait(ACTOR_PIERS, 0, 10);
	fdActorShowEmote(ACTOR_MAYOR, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	/* "And your jewel..." Yes: "You actually got it back from the Kibombo? */
	/* Incredible..." No: "You didn't recover it from the Kibombo?" */
	fdEventOpenMessage(ACTOR_MAYOR, 0);
	if(syFieldEventAskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
		fdActorShowEmote(ACTOR_ELDER, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
		/* "You actually got it back from the Kibombo? Incredible..." */
		syFieldEventShowMessageThenWait(ACTOR_ELDER, 0, 10);
		fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTHWEST);
		fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
		syFieldEventWait(20);
		/* "I was able to recover it with the help of Felix and his friends." */
		syFieldEventShowMessageThenWait(ACTOR_PIERS, 0, 10);
		fdActorShowEmote(ACTOR_ELDERS_DAUGHTER, EMOTE_IN_FRONT | EMOTE_SMILE,
						 30);
		/* "That's wonderful! I'm so relieved! I was worried that you might..."
		 */
		syFieldEventShowMessageThenWait(ACTOR_ELDERS_DAUGHTER, 0, 10);
		FieldStateWork->msg += 3;
	}
	else {
		fdActorShowEmote(ACTOR_ELDERS_DAUGHTER, EMOTE_IN_FRONT | EMOTE_WINCE,
						 30);
		scMadraRoomsCountAnswer();
		FieldStateWork->msg += 3;
		/* "You didn't recover it from the Kibombo?" */
		syFieldEventShowMessageThenWait(ACTOR_ELDER, 0, 10);
		fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTHWEST);
		fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_SHAKE_HEAD);
		syFieldEventWait(20);
		/* "That's not true, sir. Felix and his companions helped me recover
		 * it." */
		syFieldEventShowMessageThenWait(ACTOR_PIERS, 0, 10);
		scFieldActorStartRepeatedMotion(ACTOR_ELDERS_DAUGHTER, 3);
		fdActorSetAttachedEffect(ACTOR_ELDERS_DAUGHTER,
								 ATTACHED_EFFECT_IN_FRONT |
									 ATTACHED_EFFECT_ONCE);
		syFieldEventWait(50);
		/* "Please! Don't startle me like that! I was afraid you might never..."
		 */
		syFieldEventShowMessageThenWait(ACTOR_ELDERS_DAUGHTER, 0, 10);
	}
	fdActorSetAnimation(ACTOR_PIERS, ANIM_NOD);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_NOD);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
	syFieldEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	syFieldEventWait(20);
	/* "That's why we came back... To let you know that we've recovered it..."
	 */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 10);
	if(fdEventGetFlag(FLAG_PIERS_SETTING_SAIL) != 0)
		scMadraRoomsSayFarewellAgain();
	else
		scMadraRoomsAskToStayLonger();
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
	fdActorFaceDirectionAndWait(ACTOR_FELIX, FACING_SOUTH);
	syFieldEventWait(20);
	fdActorSetAnimation(ACTOR_PIERS, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
	syFieldEventWait(40);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_EAST, 0);
	syFieldEventWait(40);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_NOD);
	fdActorSetAnimation(ACTOR_PIERS, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
	syFieldEventWait(20);
	scMadraRoomsWalkIntoLeader(ACTOR_KRADEN);
	scMadraRoomsWalkIntoLeader(ACTOR_SHEBA);
	scMadraRoomsWalkIntoLeader(ACTOR_PIERS);
	scMadraRoomsWalkIntoLeader(ACTOR_JENNA);
	fdEventSetFlag(FLAG_MADRA_FAREWELL_SAID);
	fdEventEnd();
}

/* The welcome's end when Piers has already agreed to sail for Lemuria: the */
/* household asks "Ah, so you'll be leaving again, will you?" */
void scMadraRoomsSayFarewellAgain(void) {
	syFieldEventSetMessageId((s32)MsgMadraRoomsAhSoYoullBeLeaving);
	/* "Ah, so you'll be leaving again, will you?" */
	syFieldEventShowMessageThenWait(ACTOR_MAYOR, 0, 10);
	fdActorFaceEachOther(ACTOR_FELIX, ACTOR_KRADEN, 0);
	syFieldEventWait(40);
	fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
	syFieldEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
	syFieldEventWait(20);
	fdActorShowEmote(ACTOR_ELDERS_DAUGHTER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
					 30);
	/* "Are you sure you must leave so soon, Piers?" */
	syFieldEventShowMessageThenWait(ACTOR_ELDERS_DAUGHTER, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
	syFieldEventWait(20);
	/* "I may yet return here again, but I'm afraid I must be going soon." */
	syFieldEventShowMessageThenWait(ACTOR_PIERS, 0, 10);
	fdActorShowEmote(ACTOR_ELDER, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	/* "You seem to be quite busy." */
	syFieldEventShowMessageThenWait(ACTOR_ELDER, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	syFieldEventWait(20);
	/* "It is their youth that allows them to be so busy." */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 10);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "But we'll be back, I promise. Maybe we'll be able to visit at a more..."
	 */
	syFieldEventShowMessageThenWait(ACTOR_JENNA, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_ELDERS_GRANDSON, ANIM_NOD);
	syFieldEventWait(20);
	/* "I'd like that..." */
	syFieldEventShowMessageThenWait(ACTOR_ELDERS_GRANDSON, 0, 10);
	fdActorFaceDirectionAndWait(ACTOR_JENNA, FACING_SOUTH);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
	syFieldEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_ELDERS_GRANDDAUGHTER, ANIM_NOD);
	syFieldEventWait(20);
	/* "So would I!" */
	syFieldEventShowMessageThenWait(ACTOR_ELDERS_GRANDDAUGHTER, 0, 10);
}

/* The welcome's end when Piers has not yet agreed to sail: the household asks
 */
/* the party to stay a little longer now that the jewel is recovered, and */
/* Piers explains why he cannot. */
void scMadraRoomsAskToStayLonger(void) {
	syFieldEventSetMessageId((s32)MsgMadraRoomsIfYouveAlreadyRecovered);
	fdActorSetAnimationAndWait(ACTOR_ELDERS_GRANDSON, ANIM_NOD);
	syFieldEventWait(20);
	/* "If you've already recovered your jewel, why don't you stay here for..."
	 */
	syFieldEventShowMessageThenWait(ACTOR_ELDERS_GRANDSON, 0, 10);
	fdActorFaceDirectionAndWait(ACTOR_JENNA, FACING_SOUTH);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
	syFieldEventWait(20);
	/* "Unfortunately, we can't..." */
	syFieldEventShowMessageThenWait(ACTOR_JENNA, 0, 10);
	fdActorFaceActor(ACTOR_SHEBA, ACTOR_JENNA, 0);
	syFieldEventWait(20);
	/* "Hey, have you seen the girl out front?" */
	syFieldEventShowMessageThenWait(ACTOR_SHEBA, 0, 10);
	fdActorFaceDirectionAndWait(ACTOR_JENNA, FACING_NORTH);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
	syFieldEventWait(20);
	fdActorShowEmote(ACTOR_ELDERS_GRANDSON, EMOTE_IN_FRONT | EMOTE_QUESTION,
					 30);
	fdActorFaceActor(ACTOR_PIERS, ACTOR_ELDERS_GRANDSON, 0);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "We must go...to the island that is my home." */
	syFieldEventShowMessageThenWait(ACTOR_PIERS, 0, 10);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	fdActorFaceActor(ACTOR_KRADEN, ACTOR_PIERS, 0);
	/* "Piers, are you... Are you sure?" */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 10);
	fdActorFaceActor(ACTOR_PIERS, ACTOR_KRADEN, 0);
	syFieldEventWait(60);
	fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
	syFieldEventWait(20);
	/* "After what we've been through, I feel I've learned a few things." */
	syFieldEventShowMessageThenWait(ACTOR_PIERS, 0, 10);
	fdActorFaceActor(ACTOR_FELIX, ACTOR_PIERS, 0);
	fdActorFaceActor(ACTOR_SHEBA, ACTOR_PIERS, 0);
	fdActorFaceActor(ACTOR_JENNA, ACTOR_PIERS, 0);
	syFieldEventWait(20);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	/* "Like what!?" */
	syFieldEventShowMessageThenWait(ACTOR_SHEBA, 0, 10);
	fdActorFaceActor(ACTOR_PIERS, ACTOR_SHEBA, 0);
	syFieldEventWait(20);
	/* "Like...the fact that you truly are good people." */
	syFieldEventShowMessageThenWait(ACTOR_PIERS, 0, 10);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	fdActorFaceActor(ACTOR_PIERS, ACTOR_FELIX, 0);
	syFieldEventWait(20);
	/* "And that you have gone to great lengths for no other reason than to..."
	 */
	syFieldEventShowMessageThenWait(ACTOR_PIERS, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
	syFieldEventWait(20);
	/* "I have come to realize that I, too, must do something to help where..."
	 */
	syFieldEventShowMessageThenWait(ACTOR_PIERS, 0, 10);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	/* "Piers..." */
	syFieldEventShowMessageThenWait(ACTOR_JENNA, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_SHAKE_HEAD);
	syFieldEventWait(20);
	fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTHEAST);
	/* "I have made my decision." */
	syFieldEventShowMessageThenWait(ACTOR_PIERS, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
	syFieldEventWait(20);
	fdActorFaceActor(ACTOR_PIERS, ACTOR_KRADEN, 0);
	syFieldEventWait(20);
	/* "And the sooner we depart, the better, right, Kraden?" */
	syFieldEventShowMessageThenWait(ACTOR_PIERS, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	syFieldEventWait(20);
	fdActorFaceActor(ACTOR_PIERS, ACTOR_FELIX, 0);
	syFieldEventWait(20);
	/* "We shall set sail, Felix! To my ship! To the Eastern Sea!" */
	syFieldEventShowMessageThenWait(ACTOR_PIERS, 0, 10);
	fdEventSetFlag(FLAG_PIERS_SETTING_SAIL);
	fdActorShowEmote(ACTOR_ELDERS_DAUGHTER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
					 30);
	/* "Do you truly mean to leave so soon, Piers?" */
	syFieldEventShowMessageThenWait(ACTOR_ELDERS_DAUGHTER, 0, 10);
	fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTHWEST);
	fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
	syFieldEventWait(20);
	/* "I may yet again have reason to visit your town, but... yes, I must go."
	 */
	syFieldEventShowMessageThenWait(ACTOR_PIERS, 0, 10);
	fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTHWEST, 0);
	fdActorShowEmote(ACTOR_ELDER, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	/* "You seem quite busy." */
	syFieldEventShowMessageThenWait(ACTOR_ELDER, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	syFieldEventWait(20);
	/* "It is their youth that allows them to be so busy." */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 10);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
	/* "But we'll be back, I promise. Maybe we'll be able to visit at a more..."
	 */
	syFieldEventShowMessageThenWait(ACTOR_JENNA, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_ELDERS_GRANDSON, ANIM_NOD);
	syFieldEventWait(20);
	/* "I'd like that..." */
	syFieldEventShowMessageThenWait(ACTOR_ELDERS_GRANDSON, 0, 10);
	fdActorFaceDirectionAndWait(ACTOR_JENNA, FACING_SOUTH);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
	syFieldEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_ELDERS_GRANDDAUGHTER, ANIM_NOD);
	syFieldEventWait(20);
	/* "So would I!" */
	syFieldEventShowMessageThenWait(ACTOR_ELDERS_GRANDDAUGHTER, 0, 10);
}

/* clang-format off */
/* The old couple's walks into their back room and out again, Char hopping */
/* after them, as they fetch the Djinni for the Healing Fungus. */
s32 MadraRooms_WifeWalksToBackRoom[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE_XZ(356, 72),
    SC_SYNC,
    SC_MOVE(324, 0, 73),
    SC_SYNC,
    SC_POSITION_ORIGIN,
    SC_END,
};

/* Felix's script as the old man passes: one step aside. */
s32 MadraRooms_LeaderStepsAside[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE_XZ(339, 111),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_SET(SW_FACING, FACING_NORTHWEST),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_END,
};

/* The same script as MadraRooms_WifeWalksToBackRoom, under another name. */
/* The mushroom lover's script off to the back room to fetch Char. */
s32 MadraRooms_OldManWalksToBackRoom[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE_XZ(356, 72),
    SC_SYNC,
    SC_MOVE(324, 0, 73),
    SC_SYNC,
    SC_POSITION_ORIGIN,
    SC_END,
};

/* The mushroom cook's script back to her place. */
s32 MadraRooms_WifeComesBack[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_POSITION(327, 0, 71),
    SC_SET(SW_FACING, FACING_EAST),
    SC_MOVE(352, 0, 72),
    SC_SYNC,
    SC_MOVE_XZ(373, 109),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_SOUTHWEST),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(10),
    SC_END,
};

/* The mushroom lover's script back from the back room with Char. */
s32 MadraRooms_OldManComesBack[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_POSITION(327, 0, 71),
    SC_SET(SW_FACING, FACING_EAST),
    SC_MOVE(352, 0, 72),
    SC_SYNC,
    SC_MOVE_XZ(373, 97),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_SOUTHWEST),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(10),
    SC_END,
};

/* Char's script once the old man gives him to the party: he follows Felix. */
s32 MadraRooms_CharFollows[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_POSITION(327, 24, 71),
    SC_SET(SW_FACING, FACING_EAST),
    SC_MOVE(352, 0, 72),
    SC_SYNC,
    SC_MOVE_XZ(373, 97),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_SOUTHWEST),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(10),
    SC_END,
};

#include "shared/scene_entrances_default.c"

u32 SceneExits[] = {
    SCENE_EXITS(MadraRoomsHouses),
    SCENE_EXIT_UNLESS(MADRA_ROOMS_HOUSES_TO_MADRA_SOUTHWEST, Madra, MADRA_FROM_MADRA_ROOMS_HOUSES_SOUTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MADRA_ROOMS_HOUSES_TO_MADRA_SOUTH_A, Madra, MADRA_FROM_MADRA_ROOMS_HOUSES_SOUTH_A, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, MadraRoomsHouses, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, MadraRoomsHouses, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MADRA_ROOMS_HOUSES_TO_MADRA_NORTHWEST, Madra, MADRA_FROM_MADRA_ROOMS_HOUSES_NORTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, MadraRoomsHouses, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, MadraRoomsHouses, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MADRA_ROOMS_HOUSES_TO_MADRA_SOUTHEAST, Madra, MADRA_FROM_MADRA_ROOMS_HOUSES_SOUTHEAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MADRA_ROOMS_HOUSES_TO_MADRA_SOUTH_B, Madra, MADRA_FROM_MADRA_ROOMS_HOUSES_SOUTH_B, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MADRA_ROOMS_HOUSES_TO_MADRA_NORTHEAST_A, Madra, MADRA_FROM_MADRA_ROOMS_HOUSES_NORTHEAST_A, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(11, MadraRoomsHouses, 12, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(12, MadraRoomsHouses, 11, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(13, MadraRoomsHouses, 14, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(14, MadraRoomsHouses, 13, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MADRA_ROOMS_HOUSES_TO_MADRA_NORTHEAST_B, Madra, MADRA_FROM_MADRA_ROOMS_HOUSES_NORTHEAST_B, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MADRA_ROOMS_HOUSES_TO_MADRA_WEST, Madra, MADRA_FROM_MADRA_ROOMS_HOUSES_WEST, EXIT_FLAG_NONE),
    SCENE_EXITS(MadraRoomsCavernHomes),
    SCENE_EXIT_UNLESS(MADRA_ROOMS_CAVERN_HOMES_TO_MADRA_EAST, Madra, MADRA_FROM_MADRA_ROOMS_CAVERN_HOMES_EAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MADRA_ROOMS_CAVERN_HOMES_TO_MADRA_WEST, Madra, MADRA_FROM_MADRA_ROOMS_CAVERN_HOMES_WEST, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

/* Madra's houses, shops, inn and sanctum until the party returns with Piers */
/* (MADRA_ROOMS_ACTOR_ID, then MADRA_ROOMS_TRAVELERS_ACTOR_ID). */
ACTOR_PLACE MadraRooms_Placements[] = {
    ACTOR_PLACEMENT(SPRITE_BALD_MAN_IN_ORANGE_VEST, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 184, 0, 120, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_AUBURN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 112, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_BLUE_COAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 109, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_IN_GREEN_SHAWL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 96, FACING_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_RED_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 296, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_BLUE_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 384, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_AUBURN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 112, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_BLUE_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 640, 0, 152, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_IN_GREEN_SHAWL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 615, 0, 311, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 728, 0, 364, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 744, 0, 348, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 160, 0, 556, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_AUBURN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 624, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_IN_GREEN_SHAWL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 648, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 604, 0, 548, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PINK_APRON, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 432, 0, 616, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_WHITE_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 568, 0, 518, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, FLAG_DJINNI(DJINNI_CHAR), MOVEMENT_TYPE_WANDER, 368, 0, 320, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 600, 0, 56, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 676, 0, 762, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 730, 0, 712, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WARRIOR_IN_GOLDEN_HELMET, FLAG_BRIGGS_CAUGHT, MOVEMENT_TYPE_STAND, 847, 0, 711, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WARRIOR_IN_GOLDEN_HELMET, FLAG_BRIGGS_CAUGHT, MOVEMENT_TYPE_STAND, 906, 0, 760, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

/* Madra's houses once the party returns with Piers (enum */
/* MADRA_ROOMS_ACTOR_ID, then MADRA_ROOMS_RETURN_ACTOR_ID). */
ACTOR_PLACE MadraRooms_PlacementsOnReturn[] = {
    ACTOR_PLACEMENT(SPRITE_BALD_MAN_IN_ORANGE_VEST, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 184, 0, 120, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_AUBURN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 112, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_BLUE_COAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 109, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_IN_GREEN_SHAWL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 96, FACING_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_RED_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 296, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_BLUE_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 384, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_AUBURN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 584, 0, 88, FACING_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_BLUE_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 640, 0, 152, FACING_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_IN_GREEN_SHAWL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 620, 0, 152, FACING_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 680, 0, 152, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 696, 0, 136, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 160, 0, 552, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_AUBURN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 624, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_IN_GREEN_SHAWL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 648, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 604, 0, 548, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PINK_APRON, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 432, 0, 616, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_WHITE_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 568, 0, 518, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, FLAG_DJINNI(DJINNI_CHAR), MOVEMENT_TYPE_WANDER, 368, 0, 320, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 600, 0, 56, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MADRAS_ELDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 614, 0, 111, FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MADRAS_MAYOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 636, 0, 111, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1288, 0, 8, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1288, 0, 8, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1288, 0, 8, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1288, 0, 8, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 676, 0, 762, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 730, 0, 712, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

/* The jail (MADRA_ROOMS_JAIL_ACTOR_ID). */
ACTOR_PLACE MadraRooms_JailPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 528, 0, 840, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 522, 0, 840, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 472, 0, 840, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PIERS, FLAG_BRIGGS_CAUGHT, MOVEMENT_TYPE_STAND, 504, 0, 808, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 362, 0, 847, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, IF_FLAG_SET(FLAG_GABOMBA_MESSAGE_CARRIED), MOVEMENT_TYPE_STAND, 394, 0, 863, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, IF_FLAG_SET(FLAG_GABOMBA_MESSAGE_CARRIED), MOVEMENT_TYPE_STAND, 396, 0, 848, FACING_WEST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/* The left cavern home (MADRA_ROOMS_CAVERN_HOME_ACTOR_ID). */
ACTOR_PLACE MadraRooms_SecondRoomsPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_MAN_IN_GREEN_VEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 192, 0, 128, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_WHITE_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 128, 0, 192, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_PONYTAIL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 356, 0, 109, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, IF_FLAG_SET(FLAG_BRIGGS_CAUGHT), MOVEMENT_TYPE_STAND, 357, 0, 131, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/* The houses' talk and Mind Read before the Kibombo raid. The exits without a */
/* name lead from one of the map's rooms to another, 3 to 4, 6 to 7, 11 to 12 */
/* and 13 to 14 and back; 16 and 17 have no row in SceneExits. */
TRIGGER MadraRooms_TriggersBeforeRaid[] = {
    {TRIGGER_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTHWEST, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTHWEST},
    {TRIGGER_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTH_A, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTH_A},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_NORTHWEST, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_NORTHWEST},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTHEAST, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTHEAST},
    {TRIGGER_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTH_B, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTH_B},
    {TRIGGER_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_NORTHEAST_A, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_NORTHEAST_A},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {TRIGGER_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_NORTHEAST_B, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_NORTHEAST_B},
    {TRIGGER_EXIT, 16, CONDITION_ALWAYS, 16},
    {TRIGGER_EXIT, 17, CONDITION_ALWAYS, 17},
    {TRIGGER_TALK, ACTOR_OLD_MAN_WARY_OF_GONDOWAN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsLandsToSouthOfGondowan)},
    {TRIGGER_TALK, ACTOR_WOMAN_WARY_OF_GONDOWAN, CONDITION_ALWAYS, EVENT(scMadraRoomsAskWillWeBeSafe)},
    {TRIGGER_TALK, ACTOR_MUSHROOM_LOVER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsWaveBigEnoughToMove)},
    {TALK_TRIGGER_FACING(FACING_NORTH), ACTOR_MUSHROOM_COOK, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsEverythingHappenedSoSuddenlyEarth)},
    {TRIGGER_TALK, ACTOR_MAN_ON_THE_STRANGE_SHIP, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsTheySayThatBoatBelongs)},
    {TRIGGER_TALK, ACTOR_MAN_ON_THE_GROUNDED_SHIP, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsBoatThatRanAground)},
    {TRIGGER_TALK, ACTOR_JEWEL_KEEPER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsMiladyAndIHaveBeen)},
    {TRIGGER_TALK, ACTOR_ELDERS_DAUGHTER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsMyFatherWasOpposed)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN_OF_THE_ELDERS_HOUSE, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsWhenElderGetsBack)},
    {TRIGGER_TALK, ACTOR_ELDERS_GRANDSON, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsDadAndGrandpaHaveGone)},
    {TRIGGER_TALK, ACTOR_ELDERS_GRANDDAUGHTER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsWheneverDadGoesOnTrips)},
    {TRIGGER_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT(scMadraRoomsTalkToHealer)},
    {TRIGGER_TALK, ACTOR_WOMAN_OF_THE_SANCTUM, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsWeMustPassThisTest)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN_OF_THE_SANCTUM, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsWaveThatMovesVeryEarth)},
    {TRIGGER_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, EVENT(scMadraRoomsTalkToInnkeeper)},
    {TRIGGER_TALK, ACTOR_INN_HELPER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsOurInnIsQuiteSafe)},
    {TRIGGER_TALK, ACTOR_GIRL_STAYING_AT_THE_INN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIHopeThoseChampaDont)},
    {TRIGGER_TALK, ACTOR_BLACK_CRYSTAL, CONDITION_ALWAYS, EVENT(scMadraRoomsGuardElderGem)},
    {TRIGGER_TALK, ACTOR_TRAVELER_WHO_SLEPT_THROUGH_THE_RAID, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsISleptRightThroughPirate)},
    {TRIGGER_TALK, ACTOR_TRAVELERS_PARTNER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsImGladMyPartner)},
    {TRIGGER_TALK, ACTOR_MORGAN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsWereHeadingToTolbi)},
    {TRIGGER_TALK, ACTOR_DEKKA, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsHeyIfYouSeeGladiator)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_WARY_OF_GONDOWAN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsKibomboAreWarlikeButThey)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_WARY_OF_GONDOWAN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsTheySayThereArePeople)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MUSHROOM_LOVER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIfMythicalSeaCreatureHas)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MUSHROOM_COOK, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsEverythingsOnFloorAndAll)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_STRANGE_SHIP, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsThatBoatsFarSturdier)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_GROUNDED_SHIP, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsItDidntEvenHaveAny)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_JEWEL_KEEPER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIWishIKnewWhy)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ELDERS_DAUGHTER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsUntilMyFatherReturnsIll)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_OF_THE_ELDERS_HOUSE, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsHeIntendsToFindBriggs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ELDERS_GRANDSON, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIfDadHasToFight)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ELDERS_GRANDDAUGHTER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIHopeHeBringsMe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsHowCouldOneExplainContinent)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_OF_THE_SANCTUM, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIfThisIsTestPlease)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_OF_THE_SANCTUM, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsRaidWasJustAnotherPart)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsNewLandBridgesBetweenContinents)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_HELPER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsActuallyIThink)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_STAYING_AT_THE_INN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsNoOneWasSafe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TRAVELER_WHO_SLEPT_THROUGH_THE_RAID, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIWasHidingUnder)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TRAVELERS_PARTNER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsWeWereFineBecauseOur)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MORGAN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsGladiatorWhoTakesPrize)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DEKKA, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsWeFoughtInColossoLast)},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraRoomsThereAreSomePotatoesBoiling))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraRoomsTheyreMakingNanItLooks))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraRoomsAllSpicesAreNeatlyArranged))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraRoomsTheresChickenCurrySlowlySimmering))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraRoomsPotOfMasalaTea))},
    {TRIGGER_TABLE_END},
};

/* The jail's talk and Mind Read, by the story flags. */
TRIGGER MadraRooms_JailTriggers[] = {
    {TRIGGER_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_WEST, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_WEST},
    {TRIGGER_TALK, ACTOR_SHIN, IF_FLAG_SET(FLAG_GABOMBA_MESSAGE_CARRIED), MSGNO(MsgMadraRoomsOhPiersAfterWay)},
    {TRIGGER_TALK, ACTOR_SHINS_FRIEND, IF_FLAG_SET(FLAG_GABOMBA_MESSAGE_CARRIED), MSGNO(MsgMadraRoomsShinsDevelopedTerribleFear)},
    {TALK_TRIGGER_FACING(FACING_NORTH), ACTOR_WITNESS_ON_ISAACS_PARTY, IF_FLAG_SET(FLAG_GABOMBA_MESSAGE_CARRIED), MSGNO(MsgWhenAndHis)},
    {TALK_TRIGGER_FACING(FACING_NORTHWEST), ACTOR_WITNESS_ON_ISAACS_PARTY, IF_FLAG_SET(FLAG_GABOMBA_MESSAGE_CARRIED), MSGNO(MsgWhenAndHis)},
    {TALK_TRIGGER_FACING(FACING_WEST), ACTOR_WITNESS_ON_ISAACS_PARTY, IF_FLAG_SET(FLAG_GABOMBA_MESSAGE_CARRIED), MSGNO(MsgWhenAndHis)},
    {TRIGGER_TALK, ACTOR_MAN_FOCUSING_ON_ICE, IF_FLAG_SET(FLAG_GABOMBA_MESSAGE_CARRIED), EVENT(scMadraRoomsTalkToManFocusingOnIce)},
    {TRIGGER_TALK, ACTOR_SHIN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsPiersWasntOneOfChampa)},
    {TRIGGER_TALK, ACTOR_SHINS_FRIEND, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsSoThatPiersGuyWasnt)},
    {TRIGGER_TALK, ACTOR_PIERS, CONDITION_ALWAYS, EVENT(scMadraRoomsTalkToPiers)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SHIN, IF_FLAG_SET(FLAG_GABOMBA_MESSAGE_CARRIED), MSGNO(MsgMadraRoomsEvenSoundOfNamePiers)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SHINS_FRIEND, IF_FLAG_SET(FLAG_GABOMBA_MESSAGE_CARRIED), MSGNO(MsgMadraRoomsIUnderstandWhyShinsScared)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WITNESS_ON_ISAACS_PARTY, IF_FLAG_SET(FLAG_GABOMBA_MESSAGE_CARRIED), MSGNO(MsgSoThisFellow)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_FOCUSING_ON_ICE, IF_FLAG_SET(FLAG_MADRA_FOCUSING_MAN_GROANED), MSGNO(MsgMadraIKnewIShouldHave)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_FOCUSING_ON_ICE | TRIGGER_SUBJECT_OWN_EFFECT, IF_FLAG_SET(FLAG_GABOMBA_MESSAGE_CARRIED), EVENT(scMadraRoomsTalkToManFocusingOnIce)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SHIN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsThereWasSomethingReallyFrightening)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SHINS_FRIEND, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsThatPiersWasStrangeFellow)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PIERS, CONDITION_ALWAYS, EVENT(scMadraRoomsMindReadPiers)},
    {TRIGGER_TABLE_END},
};

/* The left cavern home's talk and Mind Read, by the story flags. */
TRIGGER MadraRooms_SecondRoomsTriggers[] = {
    {TRIGGER_EXIT, MADRA_ROOMS_CAVERN_HOMES_TO_MADRA_EAST, CONDITION_ALWAYS, MADRA_ROOMS_CAVERN_HOMES_TO_MADRA_EAST},
    {TRIGGER_EXIT, MADRA_ROOMS_CAVERN_HOMES_TO_MADRA_WEST, CONDITION_ALWAYS, MADRA_ROOMS_CAVERN_HOMES_TO_MADRA_WEST},
    {TRIGGER_TALK, ACTOR_MAN_ON_ALHAFRA, FLAG_BRIGGS_CAUGHT, MSGNO(MsgMadraRoomsInAllThisChaosEveryone)},
    {TRIGGER_TALK, ACTOR_MAN_ON_ALHAFRA, FLAG_GABOMBA_MESSAGE_CARRIED, MSGNO(MsgMadraRoomsSoBriggsAndHisPirates)},
    {TRIGGER_TALK, ACTOR_MAN_ON_ALHAFRA, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsTheresTalkOfMightyTravelers)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_ALHAFRA, FLAG_BRIGGS_CAUGHT, MSGNO(MsgMadraRoomsMadraAndAlhafraOff)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_ALHAFRA, FLAG_GABOMBA_MESSAGE_CARRIED, MSGNO(MsgMadraRoomsItWasSureSneaky)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_ALHAFRA, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsSinceThingsAreFinallyCalming)},
    {TRIGGER_TALK, ACTOR_SISTER_IN_BED, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsOhhOhhh)},
    {TRIGGER_TALK, ACTOR_HER_SISTER, FLAG_GABOMBA_MESSAGE_CARRIED, MSGNO(MsgMadraRoomsImSoHappy)},
    {TRIGGER_TALK, ACTOR_HER_SISTER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIveToldMySisterNot)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_ALHAFRA, FLAG_BRIGGS_CAUGHT, MSGNO(MsgMadraRoomsWaitSecondIllBetBriggs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_ALHAFRA, FLAG_GABOMBA_MESSAGE_CARRIED, MSGNO(MsgMadraRoomsNowThatBriggsHasBeen)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_ALHAFRA, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsWhatIfWeJustTraveled)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_ALHAFRA, FLAG_BRIGGS_CAUGHT, MSGNO(MsgMadraRoomsAlhafraHasLongBeenTrade)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_ALHAFRA, FLAG_GABOMBA_MESSAGE_CARRIED, MSGNO(MsgMadraRoomsItSoundsLikeOsenia)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_ALHAFRA, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsBusinessPlanOurPeopleHave)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SISTER_IN_BED, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsEyesShiningInDarknessNo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HER_SISTER, FLAG_GABOMBA_MESSAGE_CARRIED, MSGNO(MsgMadraRoomsIWasSoScared)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HER_SISTER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsShinsNormallySoKind)},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraRoomsTheyreMakingTandooriChicken))},
    {TRIGGER_TABLE_END},
};

/* The houses' talk and Mind Read once the Kibombo have raided Madra. */
TRIGGER MadraRooms_TriggersAfterRaid[] = {
    {TRIGGER_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTHWEST, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTHWEST},
    {TRIGGER_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTH_A, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTH_A},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_NORTHWEST, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_NORTHWEST},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTHEAST, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTHEAST},
    {TRIGGER_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTH_B, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTH_B},
    {TRIGGER_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_NORTHEAST_A, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_NORTHEAST_A},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {TRIGGER_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_NORTHEAST_B, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_NORTHEAST_B},
    {TRIGGER_EXIT, 16, CONDITION_ALWAYS, 16},
    {TRIGGER_EXIT, 17, CONDITION_ALWAYS, 17},
    {TRIGGER_TALK, ACTOR_OLD_MAN_WARY_OF_GONDOWAN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsKibomboAreVeryWarlikeTribe)},
    {TRIGGER_TALK, ACTOR_WOMAN_WARY_OF_GONDOWAN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsImGladKibombosAttackWasnt)},
    {TRIGGER_TALK, ACTOR_MUSHROOM_LOVER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsKibomboApparentlyUsedRopes)},
    {TRIGGER_TALK, ACTOR_MUSHROOM_COOK, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsMyHusbandPursuedKibomboAfter)},
    {TRIGGER_TALK, ACTOR_MAN_ON_THE_STRANGE_SHIP, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsPiersLeftThatBoat)},
    {TRIGGER_TALK, ACTOR_MAN_ON_THE_GROUNDED_SHIP, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsICantStopThinkingAbout)},
    {TRIGGER_TALK, ACTOR_JEWEL_KEEPER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsAllKibomboWantedWhenThey)},
    {TRIGGER_TALK, ACTOR_ELDERS_DAUGHTER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsMyFatherWasHolding)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN_OF_THE_ELDERS_HOUSE, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsPiersWentToKibombo)},
    {TRIGGER_TALK, ACTOR_ELDERS_GRANDSON, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIfMyFatherdBeen)},
    {TRIGGER_TALK, ACTOR_ELDERS_GRANDDAUGHTER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsMyFatherHasntComeHome)},
    {TRIGGER_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT(scMadraRoomsTalkToHealer)},
    {TRIGGER_TALK, ACTOR_WOMAN_OF_THE_SANCTUM, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIfThatTidalWaveHadnt)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN_OF_THE_SANCTUM, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsThemKibomboPossessEvilPowers)},
    {TRIGGER_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, EVENT(scMadraRoomsTalkToInnkeeper)},
    {TRIGGER_TALK, ACTOR_INN_HELPER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsThingsHaveBeenBusyHere)},
    {TRIGGER_TALK, ACTOR_GIRL_STAYING_AT_THE_INN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIHidUnderMyBed)},
    {TRIGGER_TALK, ACTOR_BLACK_CRYSTAL, CONDITION_ALWAYS, EVENT(scMadraRoomsGuardElderGem)},
    {TRIGGER_TALK, ACTOR_TRAVELER_WHO_SLEPT_THROUGH_THE_RAID, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIWatchedEntireAttack)},
    {TRIGGER_TALK, ACTOR_TRAVELERS_PARTNER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIfYouWantToKnow)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_WARY_OF_GONDOWAN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsOurBestGuess)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_WARY_OF_GONDOWAN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsWhatCanWeDo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MUSHROOM_LOVER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIfIWereJustLittle)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MUSHROOM_COOK, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsInHisYouthMyHusband)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_STRANGE_SHIP, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsSomePeopleInTownSaw)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_GROUNDED_SHIP, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIWonderIfThatShip)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_JEWEL_KEEPER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsWhenMyHusbandHears)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ELDERS_DAUGHTER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIWontEverForgetLook)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_OF_THE_ELDERS_HOUSE, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsWhenPiersLeftHeSaid)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ELDERS_GRANDSON, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsDadAlwaysKnowsWhat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ELDERS_GRANDDAUGHTER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIHopeHesJustLate)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIHopePowerOfOur)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_OF_THE_SANCTUM, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsEveryoneSaysThemKibombo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_OF_THE_SANCTUM, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsWereLuckyKibomboDidntUse)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsTheresNothingWorseThanWicked)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_HELPER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIfItWereMe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_STAYING_AT_THE_INN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsAfterICrawledUnderBed)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TRAVELER_WHO_SLEPT_THROUGH_THE_RAID, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIHadToGo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TRAVELERS_PARTNER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsKibomboAttackedInWeeHours)},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraRoomsThereAreSomePotatoesBoiling))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraRoomsTheyreMakingNanItLooks))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraRoomsAllSpicesAreNeatlyArranged))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraRoomsTheresChickenCurrySlowlySimmering))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraRoomsPotOfMasalaTea))},
    {TRIGGER_TABLE_END},
};

/* The houses' talk and Mind Read once the party returns with Piers, the */
/* welcome, and the mushroom trade. */
TRIGGER MadraRooms_TriggersOnReturn[] = {
    {TRIGGER_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTHWEST, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTHWEST},
    {TRIGGER_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTH_A, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTH_A},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_NORTHWEST, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_NORTHWEST},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTHEAST, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTHEAST},
    {TRIGGER_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTH_B, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTH_B},
    {TRIGGER_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_NORTHEAST_A, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_NORTHEAST_A},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {TRIGGER_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_NORTHEAST_B, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_NORTHEAST_B},
    {TRIGGER_EXIT, 16, CONDITION_ALWAYS, 16},
    {TRIGGER_EXIT, 17, CONDITION_ALWAYS, 17},
    {TRIGGER_STEP, CELL_BEHAVIOR_MADRA_ROOMS_WELCOME, FLAG_MADRA_FAREWELL_SAID, EVENT(scMadraRoomsWelcomePiersHome)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_WARY_OF_GONDOWAN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsOurSalesmanSaidKibombosCeremony)},
    {TRIGGER_TALK, ACTOR_WOMAN_WARY_OF_GONDOWAN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIWonderIfKibomboHave)},
    {TRIGGER_TALK, ACTOR_MUSHROOM_LOVER, CONDITION_ALWAYS, EVENT(scMadraRoomsTalkToMushroomLover)},
    {TRIGGER_TALK, ACTOR_MUSHROOM_COOK, CONDITION_ALWAYS, EVENT(scMadraRoomsTalkToMushroomCook)},
    {TRIGGER_TALK, ACTOR_MAN_ON_THE_STRANGE_SHIP, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsSameSailorsCameThroughRecently)},
    {TRIGGER_TALK, ACTOR_MAN_ON_THE_GROUNDED_SHIP, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsOneOfSailorsIsaac)},
    {TRIGGER_TALK, ACTOR_ELDER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIKnowYouWent)},
    {TRIGGER_TALK, ACTOR_MAYOR, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsWeJustHaveToFinish)},
    {TRIGGER_TALK, ACTOR_JEWEL_KEEPER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsImSoRelievedPiersGot)},
    {TRIGGER_TALK, ACTOR_ELDERS_DAUGHTER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsICantBelievePiersActually)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN_OF_THE_ELDERS_HOUSE, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsGondowanIsSuchAwfulDangerous)},
    {TRIGGER_TALK, ACTOR_ELDERS_GRANDSON, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsILikeYouGuysLot)},
    {TRIGGER_TALK, ACTOR_ELDERS_GRANDDAUGHTER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsMyDaddyWentToGondowan)},
    {TRIGGER_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT(scMadraRoomsTalkToHealer)},
    {TRIGGER_TALK, ACTOR_WOMAN_OF_THE_SANCTUM, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsOceansBeenWarmingUpLot)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN_OF_THE_SANCTUM, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsWhenOceanWarmsEasternSea)},
    {TRIGGER_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, EVENT(scMadraRoomsTalkToInnkeeper)},
    {TRIGGER_TALK, ACTOR_INN_HELPER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsHasSomeoneBeenTelling)},
    {TRIGGER_TALK, ACTOR_GIRL_STAYING_AT_THE_INN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsOneOfOurGuestsKept)},
    {TRIGGER_TALK, ACTOR_BLACK_CRYSTAL, CONDITION_ALWAYS, EVENT(scMadraRoomsGuardElderGem)},
    {TRIGGER_TALK, ACTOR_TRAVELER_WHO_SLEPT_THROUGH_THE_RAID_ON_RETURN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIGuessThatLot)},
    {TRIGGER_TALK, ACTOR_TRAVELERS_PARTNER_ON_RETURN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIveHeardThatRisingOcean)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_WARY_OF_GONDOWAN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsContinentShiftMakesItFeel)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_WARY_OF_GONDOWAN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsSalesmanThatJustCameBack)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MUSHROOM_LOVER, CONDITION_ALWAYS, EVENT(scMadraRoomsMindReadMushroomLover)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MUSHROOM_COOK, CONDITION_ALWAYS, EVENT(scMadraRoomsMindReadMushroomCook)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_STRANGE_SHIP, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsOarsIUnderstandHeyIll)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_GROUNDED_SHIP, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsWeveHadLotOfStrange)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ELDER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsAllOfThisCouldHave)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAYOR, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsWhyWontMayorOfAlhafra)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_JEWEL_KEEPER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsICantBelieveItDespite)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ELDERS_DAUGHTER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIFeltLittleResponsible)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_OF_THE_ELDERS_HOUSE, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsPiersSaidHeWasGoing)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ELDERS_GRANDSON, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIWishTheydTellMe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ELDERS_GRANDDAUGHTER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsImSoGladDaddyGot)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIKnewTemperaturesWereRising)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_OF_THE_SANCTUM, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsOurNewShipIsSpectacular)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_OF_THE_SANCTUM, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsSeafaringTownsKnowManyMore)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIveNeverSeenSingleCustomer)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_HELPER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsSomeoneMustHaveSeenMe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_STAYING_AT_THE_INN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsItMustBeVeryStrange)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TRAVELER_WHO_SLEPT_THROUGH_THE_RAID_ON_RETURN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIfIHadBoat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TRAVELERS_PARTNER_ON_RETURN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsHmIWonderIfTidal)},
    {USE_ITEM_ON_ACTOR(ITEM_HEALING_FUNGUS), ACTOR_MUSHROOM_LOVER, FLAG_MADRA_HEALING_FUNGUS_GIVEN, EVENT(scMadraRoomsGiveHealingFungus)},
    {USE_ITEM_ON_ACTOR(ITEM_LAUGHING_FUNGUS), ACTOR_MUSHROOM_LOVER, FLAG_MADRA_HEALING_FUNGUS_GIVEN, EVENT(scMadraRoomsGiveLaughingFungus)},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraRoomsThereAreSomePotatoesBoiling))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraRoomsTheyreMakingNanItLooks))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraRoomsAllSpicesAreNeatlyArranged))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraRoomsTheresChickenCurrySlowlySimmering))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraRoomsPotOfMasalaTea))},
    {TRIGGER_TABLE_END},
};

/* Shin and then his friend run out of the jail, crying for help. */
s32 MadraRooms_ShinAndFriendRunAway[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(408, 0, 872),
    SC_SYNC,
    SC_MOVE(408, 0, 904),
    SC_SYNC,
    SC_MOVE(408, 0, 920),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

/* Shin falls over, a quarter turn in eight frames. */
s32 MadraRooms_ShinFalls[] = {
    SC_CALL(scMadraRoomsRotateSpr),
    SC_WAIT(1),
    SC_CALL(scMadraRoomsRotateSpr),
    SC_WAIT(1),
    SC_CALL(scMadraRoomsRotateSpr),
    SC_WAIT(1),
    SC_CALL(scMadraRoomsRotateSpr),
    SC_WAIT(1),
    SC_CALL(scMadraRoomsRotateSpr),
    SC_WAIT(1),
    SC_CALL(scMadraRoomsRotateSpr),
    SC_WAIT(1),
    SC_CALL(scMadraRoomsRotateSpr),
    SC_WAIT(1),
    SC_CALL(scMadraRoomsRotateSpr),
    SC_WAIT(1),
    SC_END,
};

/* Piers' shaft of ice lifts Shin and holds him bobbing in the air. */
s32 MadraRooms_ShinLifted[] = {
    SC_SET(SW_MOTION, 0),
    SC_WAIT(100),
    SC_WAIT(36),
    SC_ADD(SW_Y, toFIXED(3)),
    SC_WAIT(9),
    SC_ADD(SW_Y, toFIXED(5)),
    SC_ADD(SW_X, toFIXED(1)),
    SC_WAIT(9),
    SC_ADD(SW_Y, toFIXED(4)),
    SC_WAIT(9),
    SC_SET(SW_FACING, FACING_WEST),
    SC_ADD(SW_X, toFIXED(1)),
    SC_ADD(SW_Y, toFIXED(5)),
    SC_WAIT(9),
    SC_ADD(SW_Y, toFIXED(4)),
    SC_WAIT(9),
    SC_ADD(SW_Y, toFIXED(4)),
    SC_ADD(SW_X, toFIXED(1)),
    SC_WAIT(9),
    SC_ADD(SW_Y, toFIXED(4)),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_ADD(SW_X, toFIXED(1)),
    SC_LABEL(1),
    SC_SET(SW_SCALE_X, FIXED_ONE),
    SC_SET(SW_SCALE_Y, FIXED_ONE),
    SC_ADD(SW_Y, toFIXED(-1)),
    SC_ADD(SW_SCALE_Y, FIXED_ONE / 8),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 16),
    SC_WAIT(2),
    SC_ADD(SW_Y, toFIXED(1)),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
    SC_ADD(SW_Y, toFIXED(-1)),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_Y, FIXED_ONE / 8),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 16),
    SC_WAIT(2),
    SC_ADD(SW_Y, toFIXED(1)),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
    SC_WAIT(4),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};
/* clang-format on */
