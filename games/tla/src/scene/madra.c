/* @file */
/* Madra, the walled town of eastern Gondowan that Felix reaches over the */
/* Dehkan Plateau. Briggs's Champa pirates have just raided it to free their */
/* leader, so the two guards at the south gate stop the party and look it */
/* over before letting it in. Outside the wall in the north-east corner is the
 */
/* hole the Champa dug, with a ladder down into the catacombs; a worker in it */
/* turns Felix away until the guards have let him in. Piers is held in the jail
 */
/* on the west side, and coming out of it after he has made his shaft of ice, */
/* Felix is granted passage to Osenia by Milady. Once Briggs is caught the */
/* Kibombo have raided the town in turn, the hole is being sealed and the */
/* guards question the party again. When Felix brings Piers home the hole is */
/* filled in, and leaving the mayor's house after the welcome the mayor gives */
/* him the Cyclone Chip and Karst, come looking for Menardi, confronts him. */

#include "item_ids.h"
#include "game/game.h"
#include "field/event_inline.h"
#include "field/field_event.h"
#include "scene/scene.h"
#include "window/window.h"
#include "field/field_sequence.h"
#include "text/msg_ids.h"
#include "shop/shop.h"

/* Set once the worker has popped up out of the Champa's hole on this visit. */
#define FLAG_MADRA_WORKER_SEEN_AT_HOLE FLAG_SCENE_TEMP_80
/* Set once Felix has told the worker he is coming through the hole. */
#define FLAG_MADRA_TOLD_WORKER_COMING_THROUGH FLAG_SCENE_TEMP_81
/* Set once the boy has asked the party about the Champa boat, or about */
/* Briggs; he no longer turns to face Felix first. */
#define FLAG_MADRA_BOY_HAS_ASKED FLAG_AREA_TEMP_00
/* Set once Felix has told the boat owner that Jenna is with him. */
#define FLAG_MADRA_TOLD_OF_JENNA FLAG_AREA_TEMP_01
/* Set as the guards let the party go after questioning it about the */
/* Kibombo's raid. */
#define FLAG_MADRA_RAID_QUESTIONING_DONE 0x914
/* Set once Karst has come to Madra looking for Menardi; she is gone from the */
/* town afterwards. */
#define FLAG_MADRA_KARST_MET 0x98e
/* Set once the party has heard Karst call for her sister, this visit. */
#define FLAG_MADRA_KARST_CALLED_FOR_MENARDI FLAG_SCENE_TEMP_0F

/* The cells whose step triggers run Madra's events; a door's cell takes the */
/* number of the exit it leads by. */
typedef enum MADRA_CELL_BEHAVIOR {
	/* The Champa's hole outside the north-east wall, cell (46, 15). */
	CELL_BEHAVIOR_MADRA_CHAMPAS_HOLE = 16,
	/* The gate's inner cells, (29, 25) and (30, 25). */
	CELL_BEHAVIOR_MADRA_GATE_INSIDE = 19,
	/* The gate's outer cells, (29, 26) and (30, 26). */
	CELL_BEHAVIOR_MADRA_GATE_OUTSIDE,
	/* The path east of the mayor's house, (13, 17) to (13, 19). */
	CELL_BEHAVIOR_MADRA_PATH_PAST_KARST
} MADRA_CELL_BEHAVIOR;

/* The town's placed actors, by the rows of Madra_Placements, the first */
/* visit's: Kraden, out of sight, steps out from behind Felix at the gate, and
 */
/* Milady and her guard wait out of sight until they hurry to the jail. */
typedef enum MADRA_ACTOR_ID {
	/* The townsfolk, each named for what they talk of on the first visit; their
	 */
	/* lines change once the Kibombo have raided the town (FLAG_BRIGGS_CAUGHT)
	 */
	/* and again when the party returns with Piers. */
	ACTOR_YOUNG_MAN_ON_THE_ELDER = ACTOR_FIRST_PLACED,
	ACTOR_YOUNG_WOMAN_ON_THE_DISASTERS,
	ACTOR_OLD_MAN_ON_THE_WAVE,
	/* Who owns the boats the tidal wave ruined; on the party's return she asks
	 */
	/* after Jenna for Isaac's party. */
	ACTOR_BOAT_OWNER,
	ACTOR_OLD_MAN_ON_THE_CAPTURED_CHAMPA,
	ACTOR_OLD_WOMAN_ON_AIRS_ROCK,
	/* The boy who asks about the Champa boat and Briggs. */
	ACTOR_BRIGGS_FAN,
	ACTOR_GIRL_ON_THE_SUPERCONTINENT,
	ACTOR_OLD_MAN_WHO_SCARED_THE_CHAMPA,
	ACTOR_OLD_MAN_ON_THE_CHAMPAS_HOLE,
	ACTOR_MAN_ON_THE_CHAMPAS_PLAN,
	ACTOR_GIRL_ON_THE_BIG_HOLE,
	ACTOR_YOUNG_MAN_AGAINST_THE_PRISONER,
	ACTOR_YOUNG_WOMAN_ON_THE_PRISONER,
	/* Two jail guards, one whose friend was on guard when Briggs's men */
	/* attacked, one whose partner ran off. */
	ACTOR_GUARD_ON_HIS_FRIEND,
	ACTOR_GUARD_ON_HIS_PARTNER,
	/* The guards who stop the party at the gate. */
	ACTOR_GATE_GUARD,
	ACTOR_SECOND_GATE_GUARD,
	/* The worker who pops up out of the Champa's hole into the catacombs. */
	ACTOR_HOLE_WORKER,
	/* The two who saw the shaft of ice in the jail, there once Piers has used
	 */
	/* Frost. */
	ACTOR_ICE_SHAFT_WITNESS,
	ACTOR_FRIGHTENED_WITNESS,
	ACTOR_WEAPON_MERCHANT,
	ACTOR_ARMOR_MERCHANT,
	/* The item merchant, back at her stand once Briggs is caught. */
	ACTOR_ITEM_MERCHANT,
	ACTOR_KRADEN,
	/* The lady who grants the party passage to Osenia, called "Milady", and */
	/* her guard. */
	ACTOR_LADY,
	ACTOR_LADYS_GUARD,
	/* Who keeps the item stand while her sister recovers from the raid. */
	ACTOR_ITEM_MERCHANTS_SIBLING,
	/* The map's third treasure spot, where the mayor sets the Cyclone Chip down
	 */
	/* (scMadraMeetKarst); out of sight otherwise. */
	ACTOR_CYCLONE_CHIP = FIELD_TREASURE_ACTOR + 2
} MADRA_ACTOR_ID;

/* The rows of Madra_PlacementsOnReturn that differ: Karst, who comes */
/* looking for Menardi, and the mayor, back from Alhafra. */
typedef enum MADRA_ON_RETURN_ACTOR_ID {
	ACTOR_KARST = ACTOR_LADY,
	ACTOR_MAYOR,
	/* A man with a pack who wanders in front of the jail, asking whether the */
	/* Kibombo scare the party. */
	ACTOR_MAN_AFRAID_OF_THE_KIBOMBO
} MADRA_ON_RETURN_ACTOR_ID;

extern ACTOR_PLACE Madra_Placements[];
extern ACTOR_PLACE Madra_PlacementsOnReturn[];

void scMadraGrantPassageToOsenia(void);

void scMadraMeetKarst(void);
extern TRIGGER Madra_TriggersBeforeRaid[];
extern TRIGGER Madra_TriggersAfterRaid[];
extern TRIGGER Madra_TriggersOnReturn[];

extern u16 Madra_DoorOpening[];
extern u16 Madra_CavernHomeDoorOpening[];

const SCENE_ENTRANCE *scGetEntrances(void) { return SceneEntrances; }

const SCENE_REGION *scGetExitRegions(void) { return 0; }

const u32 *scGetExits(void) { return SceneExits; }

const ACTOR_PLACE *scGetActorPlacements(void) {
	if(fdEventGetFlag(FLAG_GABOMBA_MESSAGE_CARRIED))
		return Madra_PlacementsOnReturn;
	return Madra_Placements;
}

extern MAP_GRID_CELL_ANIMATION Madra_Doors[];

/* Felix walks up to one of Madra's doors: it opens with a door's sound, he */
/* steps through and the screen fades to the room inside. The doors are the */
/* left cavern home's two in the cliff at the top of town, the mayor's house on
 */
/* the north-west, the two of the inn (its ground floor and its upstairs), the
 */
/* house of the couple wary of Gondowan by the south-west corner, the mushroom
 */
/* couple's house and the house east of it, and the round sanctum by the south
 */
/* wall. The jail's doorway on the west side is a plain exit, with no door to */
/* open. */
/* */
/* Each door cell's step trigger is the number of the exit it leads by, and */
/* `Madra_Doors` holds, at that number less one, the cells that redraw the */
/* door open. Everyone else on the map stops while Felix walks in. */
void scMadraEnterDoor(void) {
	FIELD_STATE *state;
	ACTOR *actor;
	s16 *trigger;
	u32 id;
	s32 index;

	state = Ram_HeapSlots->fieldState;
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	for(id = ACTOR_FIRST_PLACED; id < FIELD_TREASURE_ACTOR; id++) {
		actor = fdActorGetById(id);
		if(actor != NULL) {
			actor->motionFlg = 0;
		}
	}
	trigger = &state->stepTrigger;
	index = *trigger - 1;
	fdEventPlaySnd(SE_DOOR_OPEN);
	{
		s32 x = Madra_Doors[index].x;
		s32 y = Madra_Doors[index].y;

		fdMapGridAnimateCells(Madra_Doors[index].steps, x, y);
	}
	fdActorGetById(MainRoutineStateWork.leaderActorId)->motionFlg = 0;
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_WALK);
	fdActorCenterAndWalk(MainRoutineStateWork.leaderActorId, 2, -8);
	fdEventWait(10);
	syImportEventSetExitRequest(*trigger);
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	fdEventEnd();
}

/* Madra's shops under the market tents, as mnShopRun numbers them. */
enum { SHOP_MADRA_WEAPONS = 4, SHOP_MADRA_ARMOR = 5, SHOP_MADRA_ITEMS = 6 };

/* Talking to the weapon merchant, the bald man in red at the east tent of the
 */
/* market, from across his counter opens his shop. Spoken to from any other */
/* side he talks of the merchants fighting off the Champa, and later of the */
/* strange weapons the Kibombo left behind. */
void scMadraTalkToWeaponMerchant(s32 arg) {
	if(FACING_NEAREST_CARDINAL(
		   fdActorGetById(MainRoutineStateWork.leaderActorId)->facing) ==
	   FACING_NORTH) {
		mnShopRun(SHOP_MADRA_WEAPONS, arg);
		return;
	}
	if(fdEventGetFlag(FLAG_GABOMBA_MESSAGE_CARRIED))
		fdEventSetMessageId((s32)&MsgMadraItLooksLikeKibomboMust);
	else if(fdEventGetFlag(FLAG_BRIGGS_CAUGHT))
		fdEventSetMessageId((s32)&MsgMadraArmorerAndIAreBusy);
	else
		fdEventSetMessageId((s32)&MsgMadraAfterChampaOpenedJailCells);
	/* "After the Champa opened the jail cells, they came over here... All..."
	 */
	fdEventShowMessage(ACTOR_WEAPON_MERCHANT, 0);
}

/* Talking to the armor merchant, the young man in green at the middle tent of
 */
/* the market, from across his counter opens his shop. From any other side he */
/* moans of a bone bruise from the fighting, and later talks of the pirates and
 */
/* the Kibombo. */
void scMadraTalkToArmorMerchant(s32 arg) {
	if(FACING_NEAREST_CARDINAL(
		   fdActorGetById(MainRoutineStateWork.leaderActorId)->facing) ==
	   FACING_NORTH) {
		mnShopRun(SHOP_MADRA_ARMOR, arg);
		return;
	}
	if(fdEventGetFlag(FLAG_GABOMBA_MESSAGE_CARRIED))
		fdEventSetMessageId((s32)&MsgMadraPeopleAreSayingChampa);
	else if(fdEventGetFlag(FLAG_BRIGGS_CAUGHT))
		fdEventSetMessageId((s32)&MsgMadraIKnewThereWasSomething);
	else
		fdEventSetMessageId((s32)&MsgMadraOwOwOwIThink);
	/* "Ow. Ow. Ow... I think I got a bone bruise. I hate those..." */
	fdEventShowMessage(ACTOR_ARMOR_MERCHANT, 0);
}

/* Talking to whoever keeps the west tent of the market from across the */
/* counter opens the item shop. Spoken to from any other side, at first the */
/* merchant's sister, keeping the tent, says the merchant was hurt in the */
/* Champa's attack; once Briggs is caught the merchant is back, healed, and */
/* later worries about her boyfriend Shin. */
void scMadraTalkToItemMerchant(s32 arg) {
	if(FACING_NEAREST_CARDINAL(
		   fdActorGetById(MainRoutineStateWork.leaderActorId)->facing) ==
	   FACING_NORTH) {
		mnShopRun(SHOP_MADRA_ITEMS, arg);
		return;
	}
	if(fdEventGetFlag(FLAG_GABOMBA_MESSAGE_CARRIED))
		fdEventSetMessageId((s32)&MsgMadraMyBoyfriendShinSeems);
	else if(fdEventGetFlag(FLAG_BRIGGS_CAUGHT))
		fdEventSetMessageId((s32)&MsgMadraAllMyInjuriesHaveHealed);
	else
		fdEventSetMessageId((s32)&MsgMadraMySisterWasHurtWhen);
	/* "My sister was hurt when the Champa attacked... She's recovering, but..."
	 */
	fdEventShowMessage(arg, 0);
}

/* Karst, told the party does not know her sister, shakes her head and sends */
/* it off: "If you don't know my sister, then I have nothing to say to you." */
void scMadraKarstSendsPartyAway(void) {
	fdEventSetMessageId((s32)&MsgMadraIfYouDontKnow);
	fdActorSetAnimationAndWait(ACTOR_KARST, ANIM_SHAKE_HEAD);
	fdEventWait(20);
	/* "If you don't know my sister, then I have nothing to say to you. Get..."
	 */
	fdEventShowMessageThenWait(ACTOR_KARST, 0, 10);
}

/* Talking to Karst, the red-haired woman outside the mayor's house when Felix
 */
/* brings Piers home: she asks whether the party has seen a girl named Menardi.
 */
/* Answering yes, and yes again when she asks whether her sister is dead, makes
 */
/* her threaten the party; either no sends it away. */
void scMadraKarstAsksAboutMenardi(void) {
	fdEventSetMessageId((s32)MsgMadraHaveYouByChanceSeen);
	/* "Have you by chance seen a girl named Menardi?" */
	fdEventOpenMessage(ACTOR_KARST, 0);
	if(fdEventAskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
		scFieldActorShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN,
							  30);
		/* "You have? And... And you're telling me that my sister is...dead?" */
		fdEventOpenMessage(ACTOR_KARST, 0);
		if(fdEventAskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
			scFieldActorShowEmote(ACTOR_KARST,
								  EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
			/* "If you continue these lies, I might take it upon myself to shut
			 */
			/* you..." */
			fdEventShowMessageThenWait(ACTOR_KARST, 0, 10);
		}
		else {
			scMadraKarstSendsPartyAway();
		}
	}
	else {
		scMadraKarstSendsPartyAway();
	}
}

/* Casting Mind Read on Karst outside the mayor's house: she feels it, smiles */
/* and asks whether the party is too dense to be afraid, then tells it to leave
 */
/* her to her thoughts. */
void scMadraMindReadKarst(void) {
	fdEventSetMessageId((s32)MsgMadraUsingPsynergyOnMe);
	fdActorRunRepeatedMotion(ACTOR_KARST, 2);
	fdEventWait(20);
	scFieldActorShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "Using Psynergy on me? Are you so dense that you don't know when to..."
	 */
	fdEventShowMessageThenWait(ACTOR_KARST, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_KARST, ANIM_SHAKE_HEAD);
	fdEventWait(10);
	/* "I don't have time to play games with you. Now, leave me to my thoughts!"
	 */
	fdEventShowMessageThenWait(ACTOR_KARST, 0, 10);
}

/* Walking towards the mayor's house along the path east of it (cells (13, 17)
 */
/* to (13, 19)) when Felix brings Piers home, the party hears Karst, standing */
/* just east of the path, call for her sister: "Where could Menardi have gone?
 */
/* Where are you, my sister?" She calls once a visit. */
void scMadraKarstCallsForMenardi(void) {
	if(fdEventGetFlag(FLAG_MADRA_KARST_CALLED_FOR_MENARDI) == 0) {
		fdEventStart();
		fdEventEndLastingPsynergy(0);
		scFieldActorShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
		fdEventSetMessageId((s32)&MsgMadraWhereCouldMenardiHaveGone);
		/* "Where could Menardi have gone? Where are you, my sister?" */
		fdEventShowMessageThenWait(ACTOR_KARST, 0, 10);
		fdEventEnd();
		fdEventSetFlag(FLAG_MADRA_KARST_CALLED_FOR_MENARDI);
	}
}

/* Talking to the man with a pack who wanders in front of the jail when Felix */
/* brings Piers home: "Everyone here in Madra is afraid of the Kibombo now. Do
 */
/* they scare you, too?" Either way he says their smoke signals have since */
/* announced a truce. */
void scMadraAskAfraidOfKibombo(s32 actor) {
	s32 msg = (s32)&MsgMadraEveryoneHereInMadra;

	fdEventSetMessageId(msg);
	fdEventOpenMessage(actor, 0);
	if(fdEventAskYesNoWithActorAnimation(fdPartyGetLeaderActor(), 0) == 0) {
		fdEventWait(10);
		fdEventSetMessageId(msg + 1);
	}
	else {
		fdEventWait(20);
		fdEventSetMessageId(msg + 2);
	}
	fdEventShowMessage(actor, 0);
}

/* The worker in the Champa's hole outside the north-east wall asks "You comin'
 */
/* through here?", and whatever Felix answers, tells him nobody can come */
/* through while they work and he must enter Madra through the front gates. */
/* Once told yes, he says only that. */
void scMadraAskComingThroughHole(void) {
	s32 msg;

	fdActorFaceActor(ACTOR_HOLE_WORKER, ACTOR_FELIX, 0);
	if(fdEventGetFlag(FLAG_MADRA_TOLD_WORKER_COMING_THROUGH)) {
		fdEventSetMessageId((s32)&MsgMadraYeahWellWereWorkin);
	}
	else {
		msg = (s32)&MsgMadraYouCominThroughHere;
		fdEventSetMessageId(msg);
		/* "You comin' through here?" */
		fdEventOpenMessage(ACTOR_HOLE_WORKER, 0);
		if(fdEventAskYesNoWithActorAnimation(fdPartyGetLeaderActor(), 0) == 0) {
			fdEventSetFlag(FLAG_MADRA_TOLD_WORKER_COMING_THROUGH);
			fdEventWait(10);
			scFieldActorShowEmote(ACTOR_HOLE_WORKER,
								  EMOTE_IN_FRONT | EMOTE_WINCE, 0);
			fdEventSetMessageId(msg + 1);
		}
		else {
			fdEventWait(20);
			fdActorSetAnimationAndWait(ACTOR_HOLE_WORKER, ANIM_NOD);
			fdEventSetMessageId(msg + 2);
		}
	}
	fdEventShowMessage(ACTOR_HOLE_WORKER, 0);
}

/* Once the Kibombo have raided Madra, the worker in the Champa's hole asks */
/* "Oh, uh... You coming through?", and whatever Felix answers, says they are */
/* sealing the hole up and sends him round to the main gates. */
void scMadraAskComingThroughSealedHole(void) {
	s32 msg;

	fdActorFaceActor(ACTOR_HOLE_WORKER, ACTOR_FELIX, 0);
	if(fdEventGetFlag(FLAG_MADRA_TOLD_WORKER_COMING_THROUGH)) {
		fdEventSetMessageId((s32)&MsgMadraYeahUhWereSealingHole);
	}
	else {
		msg = (s32)&MsgMadraOhUhYouComingThrough;
		fdEventSetMessageId(msg);
		/* "Oh, uh... You coming through?" */
		fdEventOpenMessage(ACTOR_HOLE_WORKER, 0);
		if(fdEventAskYesNoWithActorAnimation(fdPartyGetLeaderActor(), 0) == 0) {
			fdEventSetFlag(FLAG_MADRA_TOLD_WORKER_COMING_THROUGH);
			fdEventWait(10);
			scFieldActorShowEmote(ACTOR_HOLE_WORKER,
								  EMOTE_IN_FRONT | EMOTE_WINCE, 0);
			fdEventSetMessageId(msg + 1);
		}
		else {
			fdEventWait(20);
			fdActorSetAnimationAndWait(ACTOR_HOLE_WORKER, ANIM_NOD);
			fdEventSetMessageId(msg + 2);
		}
	}
	fdEventShowMessage(ACTOR_HOLE_WORKER, 0);
}

/* Places an actor facing an angle, the angle taken as the u16 an actor's */
/* facing is. */
static inline void scMadraPlaceFacing(s32 actorId, s32 x, s32 z, u16 facing) {
	fdActorSetPosAndFacing(actorId, x, z, facing);
}

/* Felix steps into the hole the Champa dug among the flowers outside the town
 */
/* wall in the north-east corner (cell (46, 15)). Once the guards at the gate */
/* have let the party in, the ladder in it takes him down into the catacombs. */
/* Before that, the first time on a visit, a worker pops his head up out of */
/* the hole, Felix hops back out of his way, and the worker turns him away */
/* (scMadraAskComingThroughHole); later steps that visit do nothing. */
void scMadraWorkerBlocksHole(void) {
	if(fdEventGetFlag(FLAG_MADRA_GATE_INSPECTED)) {
		fdEventPlaySnd(SE_MAP_EXIT);
		syImportEventSetExitRequest(MADRA_TO_MADRA_CATACOMBS_LADDERS_EAST);
	}
	else if(!fdEventGetFlag(FLAG_MADRA_WORKER_SEEN_AT_HOLE)) {
		fdEventStart();
		fdEventEndLastingPsynergy(0);
		scMadraPlaceFacing(ACTOR_HOLE_WORKER, toFIXED(744), toFIXED(257),
						   FACING_SOUTH);
		fdActorGetById(ACTOR_FELIX)->velY = toFIXED(5);
		fdActorSetSpeed(ACTOR_FELIX, 2);
		fdActorWalkToAndWait(ACTOR_FELIX, 720, 244);
		fdActorFaceDirectionAndWait(ACTOR_FELIX, FACING_SOUTHEAST);
		scMadraPlaceFacing(ACTOR_FELIX, toFIXED(720), toFIXED(244),
						   FACING_SOUTHEAST);
		fdEventSetFlag(FLAG_MADRA_WORKER_SEEN_AT_HOLE);
		scMadraAskComingThroughHole();
		fdEventEnd();
	}
}

/* Once the Kibombo have raided Madra, stepping into the Champa's hole (cell */
/* (46, 15)) the first time on a visit makes the worker pop up out of it again,
 */
/* Felix hops back, and the worker says they are sealing it */
/* (scMadraAskComingThroughSealedHole). Later steps that visit do nothing, and
 */
/* the hole no longer leads down. */
void scMadraWorkerBlocksSealedHole(void) {
	if(!fdEventGetFlag(FLAG_MADRA_WORKER_SEEN_AT_HOLE)) {
		fdEventStart();
		fdEventEndLastingPsynergy(0);
		scMadraPlaceFacing(ACTOR_HOLE_WORKER, toFIXED(744), toFIXED(256),
						   FACING_SOUTH);
		fdActorGetById(ACTOR_FELIX)->velY = toFIXED(5);
		fdActorSetSpeed(ACTOR_FELIX, 2);
		fdActorWalkToAndWait(ACTOR_FELIX, 720, 244);
		fdActorFaceDirectionAndWait(ACTOR_FELIX, FACING_SOUTHEAST);
		scMadraPlaceFacing(ACTOR_FELIX, toFIXED(720), toFIXED(244),
						   FACING_SOUTHEAST);
		fdEventSetFlag(FLAG_MADRA_WORKER_SEEN_AT_HOLE);
		scMadraAskComingThroughSealedHole();
		fdEventEnd();
	}
}

const TRIGGER *scGetTriggers(void) {
	if(fdEventGetFlag(FLAG_GABOMBA_MESSAGE_CARRIED))
		return Madra_TriggersOnReturn;
	if(fdEventGetFlag(FLAG_BRIGGS_CAUGHT))
		return Madra_TriggersAfterRaid;
	return Madra_TriggersBeforeRaid;
}

/* Makes the cells around the Champa's hole, (45, 14) to (47, 16), plain */
/* ground, copied from spare cells at (109, 14), so that stepping there no */
/* longer runs anything. */
void scMadraSealHoleCells(void) {
	fdMapGridCopyCellAttributes(109, 14, 3, 3, 45, 14);
}

/* When Felix brings Piers home the Champa's hole is filled in: a mound of */
/* earth is drawn where the ladder was, copied from spare cells at (51, 10), */
/* and its cells become plain ground. */
void scMadraSealHole(void) {
	fdMapGridCopyCellsLayered(51, 10, 3, 3, 45, 10);
	scMadraSealHoleCells();
}

/* Sets up the town as the party enters: the hole is filled in once Felix */
/* brings Piers home; otherwise the worker stands down in it, out of sight */
/* unless he has already popped up this visit, and the two who saw the shaft of
 */
/* ice are gone once Briggs is caught. Coming out of the jail Felix is granted
 */
/* passage to Osenia (scMadraGrantPassageToOsenia), and on the return, coming */
/* out of the mayor's house after the farewell, he is met once by the mayor and
 */
/* Karst (scMadraMeetKarst). */
s32 scInit(void) {
	ACTOR *actor;

	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	if(fdEventGetFlag(FLAG_GABOMBA_MESSAGE_CARRIED)) {
		scMadraSealHole();
	}
	else {
		/* The worker stands down in the hole. */
		fdActorSetSprFlags(fdActorGetById(ACTOR_HOLE_WORKER), 0);
		actor = fdActorGetById(ACTOR_HOLE_WORKER);
		actor->radius = fdActorGetById(ACTOR_HOLE_WORKER)->radius * 4 / 3;
		((FIELD_SPR *)fdActorGetById(ACTOR_HOLE_WORKER)->spr)->flg |=
			SPRITE_FLAG_SUNK;
		((SPR *)fdActorGetById(ACTOR_HOLE_WORKER)->spr)->sunkRows = 8;
		((FIELD_SPR *)fdActorGetById(ACTOR_HOLE_WORKER)->spr)->dirty = 1;
		if(fdEventGetFlag(FLAG_GABOMBA_MESSAGE_CARRIED) == 0 &&
		   fdEventGetFlag(FLAG_BRIGGS_CAUGHT) != 0) {
			fdActorSetPos(ACTOR_ICE_SHAFT_WITNESS, 0, 0);
			fdActorSetPos(ACTOR_FRIGHTENED_WITNESS, 0, 0);
			if(fdEventGetFlag(FLAG_MADRA_WORKER_SEEN_AT_HOLE) == 0)
				fdActorSetPos(ACTOR_HOLE_WORKER, 0, 0);
		}
		else if(fdEventGetFlag(FLAG_GABOMBA_MESSAGE_CARRIED) != 0 ||
				fdEventGetFlag(FLAG_MADRA_GATE_INSPECTED) != 0 ||
				fdEventGetFlag(FLAG_MADRA_WORKER_SEEN_AT_HOLE) == 0) {
			fdActorSetPos(ACTOR_HOLE_WORKER, 0, 0);
		}
	}
	if(MainRoutineStateWork.entrance == MADRA_FROM_MADRA_ROOMS_HOUSES_WEST &&
	   fdEventGetFlag(FLAG_OSENIA_PASSAGE_GRANTED) == 0)
		scMadraGrantPassageToOsenia();
	if(fdEventGetFlag(FLAG_GABOMBA_MESSAGE_CARRIED) != 0 &&
	   fdEventGetFlag(FLAG_MADRA_FAREWELL_SAID) != 0) {
		if(MainRoutineStateWork.entrance ==
			   MADRA_FROM_MADRA_ROOMS_HOUSES_NORTHWEST &&
		   fdEventGetFlag(FLAG_MADRA_KARST_MET) == 0) {
			scMadraMeetKarst();
			fdEventSetFlag(FLAG_MADRA_KARST_MET);
		}
		if(fdEventGetFlag(FLAG_MADRA_KARST_MET))
			fdActorSetPos(ACTOR_LADY, 0, 0);
	}
	else {
		fdActorSetPos(ACTOR_CYCLONE_CHIP, 0, 0);
	}
	return 0;
}

s32 scPrepareMap(void) { return 0; }

/* Talking to the strongman outside the jail who saw Piers raise his shaft of */
/* ice: "Was I just dreaming when that shaft of ice appeared?" Told yes, he is
 */
/* relieved; told no, he hopes Felix is joking. */
void scMadraAskAboutIceShaft(s32 actor) {
	s32 msg;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	msg = (s32)&MsgMadraWasIJustDreamingWhen;
	fdEventSetMessageId(msg);
	/* "Was I just dreaming when that shaft of ice appeared?" */
	fdEventOpenMessage(actor, 0);
	if(fdEventAskYesNoWithActorAnimation(fdPartyGetLeaderActor(), 0) == 0) {
		fdEventWait(10);
		fdEventSetMessageId(msg + 1);
	}
	else {
		fdEventWait(20);
		fdEventSetMessageId(msg + 2);
	}
	fdEventShowMessage(actor, 0);
	fdEventEnd();
}

/* The actor the lines of the boy who asks about Briggs open beside. */
/* BUG: the boy's lines open beside ACTOR_FIRST_PLACED, the young man who talks
 */
/* of the elder, wherever he stands, and not beside the boy: */
/* fdMessageShowAtActor places the window by its speaker's place on screen. */
/* In a render of the talk the young man is off screen to the north-west and */
/* every window stands at the screen's left edge; with BUGFIX's speaker, the */
/* boy, they stand over his head. */
#ifdef BUGFIX
#define BRIGGS_FAN_SPEAKER ACTOR_BRIGGS_FAN
#else
#define BRIGGS_FAN_SPEAKER ACTOR_FIRST_PLACED
#endif

/* Talking to the boy in a blue shirt beside the round sanctum: he asks whether
 */
/* the party came over the Dehkan Plateau and saw the boat grounded near the */
/* cave, or, once Briggs is caught, whether Felix is the one who beat him up. */
void scMadraAskAboutBriggs(void) {
	s32 msg;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	if(fdEventGetFlag(FLAG_BRIGGS_CAUGHT)) {
		msg = (s32)&MsgMadraHeyMisterAreYouGuy;
		fdEventSetMessageId(msg);
		/* "Hey, mister! Are you the guy who beat up Briggs?" */
		fdEventOpenMessage(BRIGGS_FAN_SPEAKER, 0);
		if(fdEventAskYesNoWithActorAnimation(fdPartyGetLeaderActor(), 0) == 0) {
			fdEventWait(10);
			fdEventSetMessageId(msg + 1);
		}
		else {
			fdEventWait(20);
			fdEventSetMessageId(msg + 2);
		}
		fdEventShowMessage(BRIGGS_FAN_SPEAKER, 0);
	}
	else {
		if(fdEventGetFlag(FLAG_MADRA_BOY_HAS_ASKED) == 0)
			fdActorFaceActor(ACTOR_BRIGGS_FAN, ACTOR_FELIX, 0);
		msg = (s32)&MsgMadraDidYouComeHereOver;
		fdEventSetMessageId(msg);
		/* "Did you come here over the Dehkan Plateau? Yeah? You must have
		 * seen..." */
		fdEventOpenMessage(BRIGGS_FAN_SPEAKER, 0);
		if(fdEventAskYesNoWithActorAnimation(fdPartyGetLeaderActor(), 0) == 0) {
			fdEventWait(10);
			fdEventSetMessageId(msg + 1);
		}
		else {
			fdEventWait(20);
			fdEventSetMessageId(msg + 2);
		}
		fdEventShowMessage(BRIGGS_FAN_SPEAKER, 0);
	}
	fdActorFaceDirection(ACTOR_BRIGGS_FAN, FACING_SOUTH, 0);
	fdEventSetFlag(FLAG_MADRA_BOY_HAS_ASKED);
	fdEventEnd();
}

/* Casting Mind Read on the boy beside the sanctum before he has asked his */
/* question makes him ask it aloud, as talking to him does */
/* (scMadraAskAboutBriggs); while Briggs is free he starts, an exclamation mark
 */
/* over his head, and turns to Felix first. */
void scMadraMindReadBriggsFan(void) {
	s32 msg, x, z;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	if(fdEventGetFlag(FLAG_BRIGGS_CAUGHT)) {
		msg = (s32)&MsgMadraHeyMisterAreYouGuy;
		fdEventSetMessageId(msg);
		/* "Hey, mister! Are you the guy who beat up Briggs?" */
		fdEventOpenMessage(BRIGGS_FAN_SPEAKER, 0);
		if(fdEventAskYesNoWithActorAnimation(fdPartyGetLeaderActor(), 0) == 0) {
			fdEventWait(10);
			fdEventSetMessageId(msg + 1);
		}
		else {
			fdEventWait(20);
			fdEventSetMessageId(msg + 2);
		}
		fdEventShowMessage(BRIGGS_FAN_SPEAKER, 0);
	}
	else {
		if(fdEventGetFlag(FLAG_MADRA_BOY_HAS_ASKED) == 0) {
			x = EMOTE_IN_FRONT | EMOTE_EXCLAMATION;
			z = 30;
			scFieldActorShowEmote(ACTOR_BRIGGS_FAN, x, z);
			fdActorFaceActor(ACTOR_BRIGGS_FAN, ACTOR_FELIX, 0);
		}
		msg = (s32)&MsgMadraDidYouComeHereOver;
		fdEventSetMessageId(msg);
		/* "Did you come here over the Dehkan Plateau? Yeah? You must have
		 * seen..." */
		fdEventOpenMessage(BRIGGS_FAN_SPEAKER, 0);
		if(fdEventAskYesNoWithActorAnimation(fdPartyGetLeaderActor(), 0) == 0) {
			fdEventWait(10);
			fdEventSetMessageId(msg + 1);
		}
		else {
			fdEventWait(20);
			fdEventSetMessageId(msg + 2);
		}
		fdEventShowMessage(BRIGGS_FAN_SPEAKER, 0);
	}
	fdActorFaceDirection(ACTOR_BRIGGS_FAN, FACING_SOUTH, 0);
	fdEventSetFlag(FLAG_MADRA_BOY_HAS_ASKED);
	fdEventEnd();
}

/* Talking to the auburn-haired boat owner south-east of the jail when Felix */
/* brings Piers home: she asks whether a girl named Jenna is with him. Told */
/* yes, she says the four weatherworn travellers with the amazing ship must */
/* have been looking for them; told no, she asks him to let Jenna know Isaac */
/* is looking for her, and asks again next time. */
void scMadraAskAboutJenna(void) {
	s32 flags;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdActorFaceActor(ACTOR_BOAT_OWNER, ACTOR_FELIX, 0);
	fdEventSetMessageId((s32)&MsgMadraIsThereByAnyChance);
	flags = 0;
	/* "Is there by any chance a girl named Jenna with you?" Yes: "Ah! Those */
	/* people must have been looking for..." No: "Oh... Well, if you run across
	 */
	/* anyone named..." */
	fdEventOpenMessage(ACTOR_BOAT_OWNER, flags);
	if(fdEventAskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
		fdEventSetFlag(FLAG_MADRA_TOLD_OF_JENNA);
		/* "Ah! Those people must have been looking for you! There were four
		 * of..." */
		fdEventShowMessage(ACTOR_BOAT_OWNER, 0);
		(FieldStateWork->msg)++;
	}
	else {
		(FieldStateWork->msg)++;
		/* "Oh... Well, if you run across anyone named Jenna, let her know
		 * Isaac..." */
		fdEventShowMessage(ACTOR_BOAT_OWNER, 0);
	}
	fdEventEnd();
}

/* The first time Felix walks north into the south gate (cells (29, 26) and */
/* (30, 26)), the two guards stop the party: "You there! Hold your ground!" */
/* Kraden asks what the trouble is, one guard walks round the party looking it
 */
/* over, and they agree it is not the Champa, who raided the town to free */
/* Briggs, and let it pass. */
void scMadraInspectPartyAtGate(void) {
	ACTOR *actor;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdEventSetMessageId((s32)MsgMadraYouThereHoldYourGround);
	/* "You there! Hold your ground!" */
	fdEventShowMessageThenWait(ACTOR_GATE_GUARD, 0, 2);
	fdCameraMoveTo(476, CAMERA_KEEP, 420, 1);
	fdActorWalkBy(ACTOR_SECOND_GATE_GUARD, 3, 8);
	fdActorWalkByAndWait(ACTOR_GATE_GUARD, -3, 8);
	fdActorWaitForMoveById(ACTOR_SECOND_GATE_GUARD);
	fdActorWalkToAndWait(ACTOR_FELIX, 472, 426);
	fdEventWait(10);
	fdActorFaceDirectionAndWait(MainRoutineStateWork.leaderActorId,
								FACING_NORTH);
	scFieldActorLaunchFromLeader(ACTOR_KRADEN, 16, 1, FACING_NORTH);
	fdCameraWaitForArrival();
	fdActorWaitForMoveById(ACTOR_KRADEN);
	fdActorFaceDirection(ACTOR_FELIX, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
	fdEventWait(30);
	fdEventWait(10);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
	fdEventWait(20);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	fdEventWait(20);
	/* "What seems to be the trouble?" */
	fdEventShowMessageThenWait(ACTOR_KRADEN, 0, 2);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_GATE_GUARD, ANIM_NOD);
	fdEventWait(20);
	/* "Just wait there..." */
	fdEventShowMessageThenWait(ACTOR_SECOND_GATE_GUARD, 0, 2);
	fdEventWait(10);
	fdActorFaceActor(ACTOR_GATE_GUARD, ACTOR_SECOND_GATE_GUARD, 0);
	fdEventWait(10);
	fdActorFaceActor(ACTOR_SECOND_GATE_GUARD, ACTOR_GATE_GUARD, 0);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_GATE_GUARD, ANIM_NOD);
	fdEventWait(30);
	fdActorFaceActor(ACTOR_SECOND_GATE_GUARD, ACTOR_GATE_GUARD, 0);
	fdEventWait(10);
	fdActorFaceActor(ACTOR_SECOND_GATE_GUARD, ACTOR_KRADEN, 0);
	fdActorSetAnimationAndWait(ACTOR_SECOND_GATE_GUARD, ANIM_NOD);
	fdEventWait(30);
	fdActorFaceDirection(ACTOR_GATE_GUARD, FACING_SOUTH_SOUTHWEST, 0);
	fdActorGetById(ACTOR_SECOND_GATE_GUARD)->priFlg &=
		~ACTOR_PRIORITY_AUTOMATIC;
	fdActorSetSprPriority(ACTOR_SECOND_GATE_GUARD, 3);
	fdActorWalkToAndWait(ACTOR_SECOND_GATE_GUARD, 457, 402);
	fdActorWalkByAndWait(ACTOR_SECOND_GATE_GUARD, 16, 0);
	fdActorWalkToAndWait(ACTOR_SECOND_GATE_GUARD, 472, 420);
	fdEventWait(30);
	fdActorWalkToAndWait(ACTOR_SECOND_GATE_GUARD, 484, 420);
	fdActorGetById(ACTOR_SECOND_GATE_GUARD)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
	fdActorFaceDirection(ACTOR_SECOND_GATE_GUARD, FACING_SOUTH, 0);
	fdEventWait(60);
	fdEventWait(10);
	fdActorFaceDirection(ACTOR_SECOND_GATE_GUARD, FACING_NORTH, 0);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_SECOND_GATE_GUARD, ANIM_SHAKE_HEAD);
	/* "They don't look like they're Champa..." */
	fdEventShowMessageThenWait(ACTOR_SECOND_GATE_GUARD, 0, 2);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_GATE_GUARD, ANIM_NOD);
	/* "I agree..." */
	fdEventShowMessageThenWait(ACTOR_GATE_GUARD, 0, 2);
	fdActorFaceDirectionAndWait(ACTOR_SECOND_GATE_GUARD, FACING_SOUTH);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_SECOND_GATE_GUARD, ANIM_NOD);
	fdEventWait(5);
	/* "Sorry for the delay. You may pass." */
	fdEventShowMessageThenWait(ACTOR_SECOND_GATE_GUARD, 0, 2);
	fdActorGetById(ACTOR_SECOND_GATE_GUARD)->priFlg &=
		~ACTOR_PRIORITY_AUTOMATIC;
	fdActorSetSprPriority(ACTOR_SECOND_GATE_GUARD, 3);
	fdActorWalkByAndWait(ACTOR_SECOND_GATE_GUARD, 0, -16);
	fdActorWalkToAndWait(ACTOR_SECOND_GATE_GUARD, 475, 400);
	fdActorWalkByAndWait(ACTOR_SECOND_GATE_GUARD, -16, 0);
	fdActorGetById(ACTOR_SECOND_GATE_GUARD)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
	fdCameraMoveTo(476, CAMERA_KEEP, 396, 1);
	fdActorWalkToAndWait(ACTOR_SECOND_GATE_GUARD, 459, 416);
	fdActorFaceDirection(ACTOR_SECOND_GATE_GUARD, FACING_SOUTHEAST, 0);
	fdEventWait(10);
	scFieldActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
	/* "What were you checking us for?" */
	fdEventShowMessageThenWait(ACTOR_KRADEN, 0, 2);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_GATE_GUARD, 2);
	fdEventWait(20);
	/* "Our town was sacked by pirates..." */
	fdEventShowMessageThenWait(ACTOR_GATE_GUARD, 0, 2);
	fdEventWait(10);
	scFieldActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	scFieldActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 50);
	fdEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_SECOND_GATE_GUARD, ANIM_SHAKE_HEAD);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
	/* "They'd come to free their leader, Briggs." */
	fdEventShowMessageThenWait(ACTOR_SECOND_GATE_GUARD, 0, 2);
	fdEventWait(10);
	fdActorFaceEachOther(ACTOR_FELIX, ACTOR_KRADEN, 0);
	fdEventWait(20);
	fdEventWait(10);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
	scFieldActorShowEmote(ACTOR_SECOND_GATE_GUARD,
						  EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 50);
	/* "They came from the east, a small party, and we repelled them with..." */
	fdEventShowMessageThenWait(ACTOR_GATE_GUARD, 0, 2);
	scFieldActorShowEmote(ACTOR_SECOND_GATE_GUARD,
						  EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
	/* "But then, foreigners struck from the west, exploiting a weak point..."
	 */
	fdEventShowMessageThenWait(ACTOR_SECOND_GATE_GUARD, 0, 2);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_GATE_GUARD, ANIM_NOD);
	fdEventWait(30);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 0);
	/* "That's over now, though... Feel free to relax and enjoy your time..." */
	fdEventShowMessageThenWait(ACTOR_GATE_GUARD, 0, 2);
	fdActorSetSpeed(ACTOR_GATE_GUARD, 0.3);
	fdActorSetSpeed(ACTOR_SECOND_GATE_GUARD, 0.3);
	fdActorWalkBy(ACTOR_GATE_GUARD, 3, -8);
	fdActorWalkBy(ACTOR_SECOND_GATE_GUARD, -3, -8);
	fdActorWaitForMoveById(ACTOR_GATE_GUARD);
	fdActorWaitForMoveById(ACTOR_SECOND_GATE_GUARD);
	fdActorFaceDirection(ACTOR_SECOND_GATE_GUARD, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_GATE_GUARD, FACING_SOUTH_SOUTHWEST, 0);
	fdEventWait(20);
	fdEventWait(10);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
	fdEventWait(30);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_WALK);
	actor = fdActorGetById(MainRoutineStateWork.leaderActorId);
	if(actor != NULL)
		fdActorSetDestination(ACTOR_KRADEN, actor->x.part[0].integer,
							  actor->z.part[0].integer);
	fdActorWaitForMoveById(ACTOR_KRADEN);
	fdActorSetPos(ACTOR_KRADEN, 0, 0);
	fdEventSetFlag(FLAG_MADRA_GATE_INSPECTED);
	fdActorSetPos(ACTOR_HOLE_WORKER, 0, 0);
	fdEventEnd();
}

/* Coming out of the jail after seeing Piers raise his shaft of ice, Felix */
/* meets Milady and her guard, who hurry up crying "Has Piers escaped!?"; she */
/* asks whether the party is going to Osenia and grants it passage over the */
/* drawbridge. It runs once (FLAG_PIERS_INTERROGATED, then */
/* FLAG_OSENIA_PASSAGE_GRANTED). */
void scMadraGrantPassageToOsenia(void) {
	if(!syGameFlagGet(FLAG_PIERS_INTERROGATED))
		return;
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdCameraMoveTo(124, CAMERA_KEEP, 420, 1);
	fdCameraWaitForArrival();
	fdActorSetSpeed(ACTOR_LADY, 0.8);
	fdActorSetSpeed(ACTOR_LADYS_GUARD, 0.8);
	fdEventStartScrTransition();
	fdActorSetPos(ACTOR_LADY, 172, 332);
	fdActorSetPos(ACTOR_LADYS_GUARD, 172, 324);
	fdEventSetMessageId((s32)MsgMadraHasPiersEscaped);
	fdEventWaitTransitionFrames();
	/* "Has Piers escaped!?" */
	fdEventShowMessageThenWait(ACTOR_LADY, 0, 10);
	fdActorWalkTo(ACTOR_LADY, 156, 372);
	fdEventWait(20);
	fdActorWalkTo(ACTOR_LADYS_GUARD, 156, 372);
	fdActorWalkTo(ACTOR_FELIX, 76, 388);
	fdActorWaitForMoveById(ACTOR_LADY);
	fdActorWalkTo(ACTOR_LADY, 108, 380);
	fdActorWaitForMoveById(ACTOR_LADYS_GUARD);
	fdActorWalkTo(ACTOR_LADYS_GUARD, 124, 380);
	fdActorWaitForMoveById(ACTOR_FELIX);
	fdActorFaceDirectionById(ACTOR_FELIX, FACING_SOUTHEAST, 0);
	fdActorWaitForMoveById(ACTOR_LADY);
	fdActorWaitForMoveById(ACTOR_LADYS_GUARD);
	fdActorFaceDirection(ACTOR_LADY, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_LADYS_GUARD, FACING_NORTH, 0);
	fdEventWait(20);
	fdActorFaceDirection(ACTOR_LADY, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_LADYS_GUARD, FACING_NORTH, 0);
	fdActorSetAttachedEffect(ACTOR_LADYS_GUARD,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(ACTOR_LADY,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdEventWait(50);
	fdActorSetAnimationAndWait(ACTOR_LADYS_GUARD, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_LADY, ANIM_NOD);
	fdEventWait(20);
	fdActorWalkTo(ACTOR_LADY, 102, 340);
	fdEventWait(20);
	fdActorWalkTo(ACTOR_LADYS_GUARD, 102, 340);
	fdActorWaitForMoveById(ACTOR_LADY);
	fdActorSetPos(ACTOR_LADY, 4, 4);
	fdActorWaitForMoveById(ACTOR_LADYS_GUARD);
	fdActorSetPos(ACTOR_LADYS_GUARD, 4, 4);
	fdActorFaceDirectionById(ACTOR_FELIX, FACING_NORTHEAST, 0);
	fdEventWait(40);
	scFieldActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	fdEventWait(40);
	fdEventWait(120);
	fdActorSetPos(ACTOR_LADY, 102, 340);
	fdActorSetPos(ACTOR_LADYS_GUARD, 102, 340);
	fdActorWalkToAndWait(ACTOR_LADYS_GUARD, 100, 364);
	fdActorWalkTo(ACTOR_LADYS_GUARD, 124, 380);
	fdActorWalkToAndWait(ACTOR_LADY, 100, 380);
	fdActorWaitForMoveById(ACTOR_LADYS_GUARD);
	fdActorWalkTo(ACTOR_LADYS_GUARD, 132, 396);
	fdActorWalkToAndWait(ACTOR_LADY, 108, 396);
	fdActorWaitForMoveById(ACTOR_LADYS_GUARD);
	fdActorFaceActorById(ACTOR_FELIX, ACTOR_LADY, 0);
	fdActorFaceDirection(ACTOR_LADYS_GUARD, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_LADY, FACING_NORTH, 0);
	scFieldActorShowEmote(ACTOR_LADY, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	/* "Oh, I'm so relieved... So Piers hasn't escaped?" */
	fdEventShowMessageThenWait(ACTOR_LADY, 0, 10);
	fdActorFaceActorById(ACTOR_LADYS_GUARD, ACTOR_LADY, 0);
	fdActorSetAnimationAndWait(ACTOR_LADYS_GUARD, ANIM_NOD);
	fdEventWait(20);
	/* "If he had escaped, the elder would no longer be able to protect him." */
	fdEventShowMessageThenWait(ACTOR_LADYS_GUARD, 0, 10);
	fdActorFaceActorById(ACTOR_LADY, ACTOR_LADYS_GUARD, 0);
	fdEventWait(20);
	fdActorRunRepeatedMotion(ACTOR_LADY, 2);
	fdEventWait(20);
	/* "But...from the look of things, it seems there was a scuffle..." */
	fdEventShowMessageThenWait(ACTOR_LADY, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_LADYS_GUARD, ANIM_NOD);
	fdEventWait(20);
	/* "The cell is in disarray..." */
	fdEventShowMessageThenWait(ACTOR_LADYS_GUARD, 0, 10);
	scFieldActorShowEmote(ACTOR_LADY, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	/* "What happened in there, I wonder?" */
	fdEventShowMessageThenWait(ACTOR_LADY, 0, 10);
	fdActorFaceDirection(ACTOR_LADY, FACING_NORTH, 0);
	fdActorFaceDirectionById(ACTOR_LADYS_GUARD, FACING_NORTH, 0);
	fdEventWait(20);
	fdActorRunRepeatedMotion(ACTOR_LADYS_GUARD, 2);
	fdEventWait(20);
	/* "It seems Piers is a man of many mysteries." */
	fdEventShowMessageThenWait(ACTOR_LADYS_GUARD, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_LADY, ANIM_NOD);
	fdEventWait(20);
	/* "Not the least of which being that boat he arrived in..." */
	fdEventShowMessageThenWait(ACTOR_LADY, 0, 10);
	fdEventWait(20);
	fdActorFaceDirectionById(ACTOR_LADY, FACING_WEST, 0);
	fdEventWait(20);
	scFieldActorShowEmote(ACTOR_LADY, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	/* "Oh, you there..." */
	fdEventShowMessageThenWait(ACTOR_LADY, 0, 10);
	/* "You look like travelers." */
	fdEventShowMessageThenWait(ACTOR_LADY, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_LADYS_GUARD, 2);
	fdActorFaceActorById(ACTOR_LADYS_GUARD, ACTOR_LADY, 0);
	fdEventWait(20);
	/* "Milady, now is not the time to be speaking with strangers!" */
	fdEventShowMessageThenWait(ACTOR_LADYS_GUARD, 0, 10);
	fdActorFaceActorById(ACTOR_LADY, ACTOR_LADYS_GUARD, 0);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_LADY, ANIM_SHAKE_HEAD);
	fdEventWait(20);
	/* "It's all right... This one has such kind eyes." */
	fdEventShowMessageThenWait(ACTOR_LADY, 0, 10);
	fdActorFaceActorById(ACTOR_LADY, ACTOR_FELIX, 0);
	fdEventWait(30);
	/* "When did you arrive? And where are you going?" */
	fdEventShowMessageThenWait(ACTOR_LADY, 0, 10);
	scFieldActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
	fdActorRunRepeatedMotion(ACTOR_LADYS_GUARD, 2);
	fdEventWait(20);
	/* "My lady, please..." */
	fdEventShowMessageThenWait(ACTOR_LADYS_GUARD, 0, 10);
	fdEventWait(20);
	scFieldActorShowEmote(ACTOR_LADY, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 30);
	/* "Are you...going to Osenia?" Yes: "I knew it!" No: "No need to be shy, my
	 */
	/* friends." */
	fdMessageShowAtActor(ACTOR_LADY, 0);
	if(fdEventAskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
		/* "I knew it!" */
		fdEventShowMessageThenWait(ACTOR_LADY, 0, 10);
		(FieldStateWork->msg)++;
	}
	else {
		(FieldStateWork->msg)++;
		/* "No need to be shy, my friends." */
		fdEventShowMessageThenWait(ACTOR_LADY, 0, 10);
	}
	fdActorFaceActorById(ACTOR_LADY, ACTOR_LADYS_GUARD, 0);
	fdEventWait(20);
	/* "One requires permission to cross the bridge to Osenia, correct?" */
	fdEventShowMessageThenWait(ACTOR_LADY, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_LADYS_GUARD, ANIM_NOD);
	fdEventWait(20);
	fdActorRunRepeatedMotion(ACTOR_LADY, 2);
	fdEventWait(20);
	/* "Then I hereby grant my permission to these four travelers!" */
	fdEventShowMessageThenWait(ACTOR_LADY, 0, 10);
	scFieldActorShowEmote(ACTOR_LADYS_GUARD, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	fdActorSetAnimationAndWait(ACTOR_LADY, ANIM_NOD);
	fdEventWait(20);
	/* "Relay that message at once!" */
	fdEventShowMessageThenWait(ACTOR_LADY, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_LADYS_GUARD, 2);
	fdEventWait(60);
	fdActorSetAnimationAndWait(ACTOR_LADYS_GUARD, ANIM_NOD);
	fdEventWait(20);
	fdEventWait(20);
	fdActorWalkByAndWaitById(ACTOR_LADYS_GUARD, 6, 0);
	fdActorWalkToAndWait(ACTOR_LADYS_GUARD, 164, 316);
	fdEventWait(10);
	fdActorFaceDirectionAndWait(ACTOR_LADY, FACING_WEST);
	fdActorSetAnimationAndWait(ACTOR_LADY, ANIM_NOD);
	fdEventWait(20);
	/* "You may all continue your journey and cross into Osenia." */
	fdEventShowMessageThenWait(ACTOR_LADY, 0, 10);
	fdActorSetPos(ACTOR_LADYS_GUARD, 0, 0);
	fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
	fdEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_LADY, ANIM_NOD);
	fdEventWait(20);
	syGameFlagSet(FLAG_OSENIA_PASSAGE_GRANTED);
	fdActorWalkByAndWaitById(ACTOR_LADY, 30, 0);
	fdActorWalkToAndWait(ACTOR_LADY, 164, 316);
	fdEventWait(20);
	fdActorSetPos(ACTOR_LADY, 0, 0);
	fdEventWait(20);
	fdEventEnd();
}

/* Once the Kibombo have raided Madra, the first time Felix walks north into */
/* the south gate (cells (29, 26) and (30, 26)) a guard cries "Hey! You lot! */
/* Hold it right there!", but the other says the party could not be involved, */
/* and with Kraden's answers they let it go. */
void scMadraStopPartyAfterRaid(void) {
	ACTOR *actor;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdEventSetMessageId((s32)MsgMadraHeyYouLotHold);
	/* "Hey! You lot! Hold it right there!" */
	fdEventShowMessageThenWait(ACTOR_GATE_GUARD, 0, 2);
	fdCameraMoveTo(476, CAMERA_KEEP, 400, 1);
	fdActorWalkByAndWait(ACTOR_GATE_GUARD, -3, 8);
	fdActorWalkToAndWait(ACTOR_FELIX, 472, 426);
	fdEventWait(10);
	fdActorFaceDirectionAndWait(MainRoutineStateWork.leaderActorId,
								FACING_NORTH);
	fdActorLaunchFromLeader(ACTOR_KRADEN, 16, 1, FACING_NORTH);
	fdCameraWaitForArrival();
	fdActorWaitForMoveById(ACTOR_KRADEN);
	fdActorWalkByAndWait(ACTOR_SECOND_GATE_GUARD, 3, 8);
	fdActorFaceDirectionAndWait(ACTOR_SECOND_GATE_GUARD, FACING_EAST);
	fdActorSetAnimation(ACTOR_SECOND_GATE_GUARD, ANIM_SHAKE_HEAD);
	fdEventWait(20);
	/* "Hang on, there's no need to stop them. They couldn't be involved in..."
	 */
	fdEventShowMessageThenWait(ACTOR_SECOND_GATE_GUARD, 0, 10);
	fdActorFaceDirection(ACTOR_FELIX, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
	fdEventWait(20);
	scFieldActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	/* "If you're talking about the pirate incident, I believe we resolved..."
	 */
	fdEventShowMessage(ACTOR_KRADEN, 0);
	fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_SHAKE_HEAD);
	fdActorFaceDirectionAndWait(ACTOR_GATE_GUARD, FACING_SOUTH_SOUTHWEST);
	fdActorSetAnimationAndWait(ACTOR_GATE_GUARD, ANIM_SHAKE_HEAD);
	/* "This has nothing to do with the Champa this time, old man! This..." */
	fdEventShowMessageThenWait(ACTOR_GATE_GUARD, 0, 10);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 0);
	scFieldActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
	fdActorJump(ACTOR_KRADEN, 6, 15);
	fdActorJump(ACTOR_KRADEN, 6, 23);
	/* ""Old man"?!? Hmph!" */
	fdEventShowMessage(ACTOR_KRADEN, 0);
	scFieldActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	fdActorFaceActor(ACTOR_FELIX, ACTOR_GATE_GUARD, 0);
	fdEventWait(20);
	scFieldActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHEAST);
	/* "Wait... Did you say you were attacked again? By the Kibombo?" */
	fdEventShowMessage(ACTOR_KRADEN, 0);
	fdActorFaceDirectionAndWait(ACTOR_SECOND_GATE_GUARD, FACING_SOUTHEAST);
	fdActorSetAnimationAndWait(ACTOR_SECOND_GATE_GUARD, ANIM_NOD);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
	fdEventWait(10);
	/* "Do you know about the Kibombo?" Yes: "You must be pretty worldly, */
	/* considering that..." No: "Not many people do... They're a barbaric..." */
	fdEventOpenMessage(ACTOR_SECOND_GATE_GUARD, 0);
	fdActorFaceDirectionAndWait(ACTOR_FELIX, FACING_NORTH_NORTHWEST);
	if(fdEventAskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
		fdEventWait(25);
		fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
		/* "You must be pretty worldly, considering that they rarely leave
		 * the..." */
		fdEventShowMessageThenWait(ACTOR_SECOND_GATE_GUARD, 0, 10);
		(FieldStateWork->msg)++;
	}
	else {
		fdEventWait(25);
		fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 0);
		(FieldStateWork->msg)++;
		/* "Not many people do... They're a barbaric tribe from central..." */
		fdEventShowMessageThenWait(ACTOR_SECOND_GATE_GUARD, 0, 10);
	}
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	fdEventWait(20);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHWEST);
	/* "When did all this happen?!" */
	fdEventShowMessage(ACTOR_KRADEN, 0);
	scFieldActorShowEmote(ACTOR_SECOND_GATE_GUARD, EMOTE_IN_FRONT | EMOTE_WINCE,
						  30);
	/* "Not too long ago..." */
	fdEventShowMessageThenWait(ACTOR_SECOND_GATE_GUARD, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_GATE_GUARD, 2);
	fdEventWait(20);
	/* "Right after the mayor left, actually..." */
	fdEventShowMessageThenWait(ACTOR_GATE_GUARD, 0, 10);
	if(fdEventGetFlag(FLAG_KIBOMBO_WARRIORS_FOUND)) {
		fdEventSetMessageId((s32)MsgMadraItWasThemWasnt);
		fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
		fdEventWait(20);
		/* "It was them, wasn't it, Felix?" */
		fdEventOpenMessage(ACTOR_KRADEN, 0);
		fdActorFaceDirection(ACTOR_FELIX, FACING_EAST, 0);
		if(fdEventAskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
			fdEventWait(20);
			fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
			/* "Of course! They did seem to be acting awfully strange when we
			 * saw..." */
			fdEventShowMessage(ACTOR_KRADEN, 0);
			(FieldStateWork->msg)++;
		}
		else {
			fdEventWait(20);
			(FieldStateWork->msg)++;
			fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
			/* "Surely you remember, Felix! The cave? Those strange men we saw
			 * in..." */
			fdEventShowMessage(ACTOR_KRADEN, 0);
		}
	}
	fdEventSetMessageId((s32)MsgMadraWasAnythingStolen);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH_NORTHEAST, 0);
	fdEventWait(20);
	/* "Was anything stolen?" */
	fdEventShowMessage(ACTOR_KRADEN, 0);
	fdEventWait(40);
	scFieldActorShowEmote(ACTOR_GATE_GUARD, EMOTE_IN_FRONT | EMOTE_QUESTION,
						  30);
	fdActorFaceDirection(ACTOR_GATE_GUARD, FACING_WEST, 0);
	fdEventWait(20);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH_NORTHWEST, 0);
	scFieldActorShowEmote(ACTOR_SECOND_GATE_GUARD, EMOTE_IN_FRONT | EMOTE_WINCE,
						  30);
	fdActorSetAnimationAndWait(ACTOR_SECOND_GATE_GUARD, ANIM_SHAKE_HEAD);
	fdEventWait(10);
	fdActorFaceDirection(ACTOR_SECOND_GATE_GUARD, FACING_SOUTHEAST, 0);
	/* "Only a single black orb... Everything else was untouched." */
	fdEventShowMessageThenWait(ACTOR_SECOND_GATE_GUARD, 0, 10);
	scFieldActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	scFieldActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	/* "A black orb..." */
	fdEventShowMessage(ACTOR_KRADEN, 0);
	fdActorFaceDirectionAndWait(ACTOR_GATE_GUARD, FACING_SOUTH_SOUTHWEST);
	fdActorSetAnimationAndWait(ACTOR_GATE_GUARD, ANIM_SHAKE_HEAD);
	fdEventWait(10);
	/* "I'm afraid we don't have much more information than that." */
	fdEventShowMessageThenWait(ACTOR_GATE_GUARD, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_SECOND_GATE_GUARD, ANIM_NOD);
	fdEventWait(10);
	/* "You helped us with the Champa, so, I'll tell you what... Maybe you..."
	 */
	fdEventShowMessageThenWait(ACTOR_SECOND_GATE_GUARD, 0, 10);
	fdActorSetSpeed(ACTOR_GATE_GUARD, 0.3);
	fdActorSetSpeed(ACTOR_SECOND_GATE_GUARD, 0.3);
	fdActorWalkBy(ACTOR_GATE_GUARD, 3, -8);
	fdActorWalkBy(ACTOR_SECOND_GATE_GUARD, -3, -8);
	fdActorWaitForMoveById(ACTOR_GATE_GUARD);
	fdActorWaitForMoveById(ACTOR_SECOND_GATE_GUARD);
	fdActorFaceDirection(ACTOR_SECOND_GATE_GUARD, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_GATE_GUARD, FACING_SOUTH_SOUTHWEST, 0);
	fdEventWait(20);
	fdActorFaceDirection(ACTOR_FELIX, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
	fdEventWait(20);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
	fdEventWait(10);
	fdEventSetFlag(FLAG_MADRA_RAID_QUESTIONING_DONE);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_WALK);
	actor = fdActorGetById(MainRoutineStateWork.leaderActorId);
	if(actor != NULL)
		fdActorSetDestination(ACTOR_KRADEN, actor->x.part[0].integer,
							  actor->z.part[0].integer);
	fdActorWaitForMoveById(ACTOR_KRADEN);
	fdActorSetPos(ACTOR_KRADEN, 0, 0);
	fdActorFaceDirectionAndWait(ACTOR_FELIX, FACING_NORTH);
	fdEventEnd();
}

/* Coming out of the mayor's house after Piers's farewell, Felix hears the */
/* mayor call "Felix, wait!" and is given the Cyclone Chip promised for the */
/* boat (with no room for it, the mayor sets it down beside his house to be */
/* picked up later). Then Karst, hearing Felix's name, confronts the party */
/* about Menardi, learns of Isaac and swears to have her revenge. */
void scMadraMeetKarst(void) {
	ACTOR *actor;
	FIXED x = toFIXED(153);
	FIXED z;

	z = toFIXED(259);
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdEventStartScrTransition();
	/* The second flag is the one after the six gmDataTransferApply copies from
	 */
	/* The Broken Seal's save, which no code sets. */
	if(syGameFlagGet(FLAG_TRANSFERRED_VAULT_WARNED_OF_ESCAPED_THIEVES) ||
	   syGameFlagGet(FLAG_TRANSFERRED_DODONPA_LOCKED_UP +
					 TRANSFERRED_FLAG_COUNT))
		syGameFlagSet(FLAG_BANDIT_ON_WORLD_MAP);
	fdEventSetMessageId((s32)MsgMadraFelixWait);
	/* "Felix, wait!" */
	fdEventShowMessageThenWait(ACTOR_MAYOR, 0, 0);
	fdActorWalkToAndWait(ACTOR_FELIX, 150, 280);
	fdActorWalkByById(ACTOR_FELIX, 16, 0);
	scFieldActorLaunchFromLeader(ACTOR_JENNA, 8, 16, FACING_NORTH);
	scFieldActorLaunchFromLeader(ACTOR_KRADEN, 0, 0, FACING_NORTH);
	scFieldActorLaunchFromLeader(ACTOR_SHEBA, -8, 16, FACING_NORTH);
	scFieldActorLaunchFromLeader(ACTOR_PIERS, -16, 0, FACING_NORTH);
	fdActorWaitForMoveById(ACTOR_FELIX);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	fdEventWait(20);
	fdMapGridAnimateCells(
		Madra_Doors[MADRA_TO_MADRA_ROOMS_HOUSES_NORTHWEST - 1].steps,
		Madra_Doors[MADRA_TO_MADRA_ROOMS_HOUSES_NORTHWEST - 1].x,
		Madra_Doors[MADRA_TO_MADRA_ROOMS_HOUSES_NORTHWEST - 1].y);
	fdActorSetHorizontalPosAndAngWithTerrain(ACTOR_MAYOR, x, z, FACING_SOUTH);
	fdActorWalkByAndWaitById(ACTOR_MAYOR, 0, 4);
	fdActorAnimateThenWait(ACTOR_MAYOR, ANIM_NOD, 20);
	/* "I believe I promised you a reward for helping with the boat, didn't I?"
	 */
	fdMessageShowAtActor(ACTOR_MAYOR, 0);
	fdActorFaceActorById(ACTOR_KRADEN, ACTOR_FELIX, 0);
	fdActorFaceActorById(ACTOR_JENNA, ACTOR_FELIX, 0);
	fdActorFaceActorById(ACTOR_PIERS, ACTOR_FELIX, 0);
	fdActorFaceActorById(ACTOR_SHEBA, ACTOR_FELIX, 0);
	fdActorFaceDirectionById(ACTOR_MAYOR, FACING_SOUTH, 0);
	fdEventWait(20);
	if(fdEventAskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
		fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
		fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
		fdActorFaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
		fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
		fdActorFaceDirectionById(ACTOR_MAYOR, FACING_SOUTH, 0);
		fdEventWait(20);
		fdActorAnimateThenWait(ACTOR_MAYOR, ANIM_SHAKE_HEAD, 33);
		fdEventWait(20);
		/* "Imagine! You came all this way to put us at ease, and I totally..."
		 */
		fdEventShowMessageAndWait(ACTOR_MAYOR, 0);
		(FieldStateWork->msg)++;
	}
	else {
		fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
		fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
		fdActorFaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
		fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
		fdActorFaceDirectionById(ACTOR_MAYOR, FACING_SOUTH, 0);
		fdEventWait(20);
		fdActorAnimateThenWait(ACTOR_MAYOR, ANIM_SHAKE_HEAD, 33);
		fdEventWait(20);
		(FieldStateWork->msg)++;
		/* "Well, you may have forgotten, Felix, but I have not." */
		fdEventShowMessageThenWait(ACTOR_MAYOR, 0, 0);
	}
	scFieldActorShowEmote(ACTOR_MAYOR, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	/* "Regardless, I told you that I would reward you when the boat was..." */
	fdEventShowMessageThenWait(ACTOR_MAYOR, 0, 0);
	fdActorAnimateThenWait(ACTOR_MAYOR, ANIM_NOD, 20);
	fdEventWait(20);
	/* "That being the case, I'd like to give you that reward now." */
	fdEventShowMessageThenWait(ACTOR_MAYOR, 0, 0);
	scFieldActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	fdActorFaceActorById(ACTOR_SHEBA, ACTOR_JENNA, 0);
	fdEventWait(20);
	/* "We can't accept it now, though, could we, Jenna?" */
	fdEventShowMessageAndWait(ACTOR_SHEBA, 0);
	fdActorFaceActorById(ACTOR_JENNA, ACTOR_SHEBA, 0);
	fdEventWait(20);
	fdActorAnimateThenWait(ACTOR_JENNA, ANIM_SHAKE_HEAD, 33);
	fdEventWait(20);
	/* "Madra is still in shambles from the disaster... We couldn't possibly..."
	 */
	fdEventShowMessageAndWait(ACTOR_JENNA, 0);
	scFieldActorShowEmote(ACTOR_MAYOR, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	fdEventWait(20);
	/* "Nonsense! Misfortune or no, a promise is a promise!" */
	fdEventShowMessageThenWait(ACTOR_MAYOR, 0, 0);
	fdActorWalkByAndWaitById(ACTOR_MAYOR, 16, 8);
	fdActorFaceDirection(ACTOR_MAYOR, FACING_SOUTH, 0);
	if(gmPartyInventoryFindRoom(ITEM_CYCLONE_CHIP) >= 0) {
		fdItemDispShowItem(ITEM_CYCLONE_CHIP,
						   ITEM_DISPLAY_SPARKLES | ITEM_DISPLAY_RAISED);
		fdPartyInventoryGiveItem(ITEM_CYCLONE_CHIP, 0);
		(FieldStateWork->msg)++;
		fdActorFaceDirectionById(ACTOR_FELIX, FACING_NORTH, 0);
		syGameFlagSet(FLAG_MADRA_CYCLONE_CHIP_GIVEN);
	}
	else {
		s32 count = FieldStateWork->msg + 1;

		fdEventSetMessageId((s32)MsgMadraMayorTriedToGiveFelix);
		mnMessageArgumentPush(ITEM_CYCLONE_CHIP, MESSAGE_ARGUMENT_ITEM);
		mnFieldMessageShow((FieldStateWork->msg)++, 5);
		fdActorRunRepeatedMotion(ACTOR_MAYOR, 2);
		fdEventShowMessageAndWait(ACTOR_MAYOR, 0);
		fdActorWalkByAndWaitById(ACTOR_MAYOR, 16, 0);
		mnMessageArgumentPush(ITEM_CYCLONE_CHIP, MESSAGE_ARGUMENT_ITEM);
		mnFieldMessageShow((FieldStateWork->msg)++, 5);
		fdActorWalkByAndWait(ACTOR_MAYOR, -16, 0);
		fdActorFaceDirectionAndWait(ACTOR_MAYOR, FACING_SOUTH);
		fdActorSetPos(ACTOR_CYCLONE_CHIP, 192, 256);
		FieldStateWork->msg = count;
	}
	fdActorWalkByAndWait(ACTOR_MAYOR, -16, -8);
	fdActorFaceDirectionAndWait(ACTOR_MAYOR, FACING_SOUTH);
	scFieldActorShowEmote(ACTOR_MAYOR, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
	/* "I am glad that I was able to give it to you before you left." */
	fdEventShowMessageThenWait(ACTOR_MAYOR, 0, 0);
	fdActorAnimateThenWait(ACTOR_MAYOR, ANIM_NOD, 20);
	fdEventWait(20);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
	/* "Your travels seem to take you from danger into greater danger.
	 * Please..." */
	fdEventShowMessageThenWait(ACTOR_MAYOR, 0, 0);
	fdActorSetAnimationById(ACTOR_KRADEN, ANIM_NOD);
	fdActorSetAnimation(ACTOR_FELIX, ANIM_NOD);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
	fdActorAnimateThenWait(ACTOR_PIERS, ANIM_NOD, 20);
	fdEventWait(20);
	fdActorFaceDirectionAndWait(ACTOR_MAYOR, FACING_NORTH);
	fdActorWalkToAndWait(ACTOR_MAYOR, 153, 259);
	fdActorSetPos(ACTOR_MAYOR, 0, 0);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD, 33);
	fdEventWait(20);
	/* "Thank you, Mr. Mayor." */
	fdEventShowMessageAndWait(ACTOR_KRADEN, 0);
	/* "...Felix?" */
	fdEventShowMessageAndWait(ACTOR_KARST, 0);
	sySndPlay(SOUND_FADE_OUT_BGM);
	scFieldActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	scFieldActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	scFieldActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	scFieldActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	scFieldActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	fdActorFaceActorById(ACTOR_FELIX, ACTOR_KARST, 0);
	fdActorFaceActorById(ACTOR_PIERS, ACTOR_KARST, 0);
	fdActorFaceActorById(ACTOR_SHEBA, ACTOR_KARST, 0);
	fdActorFaceActorById(ACTOR_JENNA, ACTOR_KARST, 0);
	fdActorFaceActorById(ACTOR_KRADEN, ACTOR_KARST, 0);
	fdEventWait(20);
	fdCameraFollowActor(ACTOR_KARST, 1);
	fdEventWait(60);
	sySndPlay(MUS_CONFRONTATION);
	scFieldActorShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	/* "You... He just called you Felix, didn't he?" */
	fdEventShowMessageAndWait(ACTOR_KARST, 0);
	fdActorFaceDirection(ACTOR_KARST, FACING_WEST, 0);
	fdActorTurnToFaceEachOther(ACTOR_SHEBA, ACTOR_JENNA, 0);
	fdActorTurnToFaceEachOther(ACTOR_PIERS, ACTOR_KRADEN, 0);
	fdEventWait(40);
	scFieldActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	fdActorRunRepeatedMotion(ACTOR_KARST, 2);
	fdEventWait(20);
	/* "Why didn't I see it before? You are Felix!" */
	fdEventShowMessageAndWait(ACTOR_KARST, 0);
	fdActorFaceActorById(ACTOR_FELIX, ACTOR_KARST, 0);
	fdActorFaceActorById(ACTOR_PIERS, ACTOR_KARST, 0);
	fdActorFaceActorById(ACTOR_SHEBA, ACTOR_KARST, 0);
	fdActorFaceActorById(ACTOR_JENNA, ACTOR_KARST, 0);
	fdActorFaceActorById(ACTOR_KRADEN, ACTOR_KARST, 0);
	fdEventWait(20);
	fdActorFaceDirectionAndWait(ACTOR_KARST, FACING_WEST);
	fdActorFaceDirectionAndRefreshById(ACTOR_KARST, FACING_EAST);
	fdEventWait(20);
	/* "Then Menardi should be somewhere around here, too... And yet..." */
	fdEventShowMessageAndWait(ACTOR_KARST, 0);
	scFieldActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	fdActorFaceActorById(ACTOR_SHEBA, ACTOR_JENNA, 0);
	fdEventWait(20);
	/* "You won't see Menardi or Saturos ever again. Isaac killed them..." */
	fdEventShowMessageAndWait(ACTOR_SHEBA, 0);
	scFieldActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	/* "Sheba! Quiet!" */
	fdEventShowMessageAndWait(ACTOR_JENNA, 0);
	scFieldActorShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	/* "Hmph! I heard that, you wretched little thing!" */
	fdEventShowMessageAndWait(ACTOR_KARST, 0);
	fdActorFaceActorById(ACTOR_KARST, ACTOR_FELIX, 0);
	fdEventWait(20);
	/* "Can it be true? Is my sister dead?" Yes: "I can't believe it... No one
	 */
	/* has the power..." No: "No? Then where is she!? Show me my sister!" */
	fdMessageShowAtActor(ACTOR_KARST, 0);
	if(fdEventAskYesNoWithActorAnimation(ACTOR_FELIX, 0) != 0) {
		/* "No? Then where is she!? Show me my sister!" */
		fdEventShowMessageAndWait(ACTOR_KARST, 0);
		scFieldActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
		fdEventWait(40);
		fdActorAnimateThenWait(ACTOR_KARST, ANIM_NOD, 20);
		fdEventWait(20);
		/* "What is the matter? Is it so very hard? You were traveling with
		 * her..." */
		fdEventShowMessageAndWait(ACTOR_KARST, 0);
		fdActorTurnToFaceEachOther(ACTOR_SHEBA, ACTOR_JENNA, 0);
		fdEventWait(40);
		fdActorFaceActorById(ACTOR_SHEBA, ACTOR_KARST, 0);
		fdActorFaceActorById(ACTOR_JENNA, ACTOR_KARST, 0);
		fdEventWait(40);
		fdActorSetAnimationById(ACTOR_JENNA, ANIM_SHAKE_HEAD);
		fdActorAnimateThenWait(ACTOR_SHEBA, ANIM_SHAKE_HEAD, 33);
		fdEventWait(20);
		scFieldActorShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP,
							  30);
		/* "She would never have left you behind...and yet you are alone...
		 * So..." */
		fdEventShowMessageAndWait(ACTOR_KARST, 0);
	}
	else {
		fdEventSetMessageId((s32)MsgMadraICantBelieveItNo);
		/* "I can't believe it... No one has the power to defeat Saturos and
		 * my..." */
		fdEventShowMessageAndWait(ACTOR_KARST, 0);
		fdActorTurnToFaceEachOther(ACTOR_SHEBA, ACTOR_JENNA, 0);
		fdEventWait(40);
		fdActorFaceActorById(ACTOR_SHEBA, ACTOR_KARST, 0);
		fdActorFaceActorById(ACTOR_JENNA, ACTOR_KARST, 0);
		fdEventWait(40);
		fdActorSetAnimationById(ACTOR_JENNA, ANIM_SHAKE_HEAD);
		fdActorAnimateThenWait(ACTOR_SHEBA, ANIM_SHAKE_HEAD, 33);
		fdEventWait(20);
		scFieldActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP,
							  30);
		/* "Yet you all avoid my gaze... And that look on your faces...
		 * It...is..." */
		fdEventShowMessageAndWait(ACTOR_KARST, 0);
	}
	fdEventSetMessageId((s32)MsgMadraWhoIsThisIsaac);
	fdActorRunRepeatedMotion(ACTOR_KARST, 2);
	fdEventWait(20);
	/* "Who is this Isaac?" */
	fdEventShowMessageAndWait(ACTOR_KARST, 0);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
	fdActorSetAnimation(ACTOR_FELIX, ANIM_SHAKE_HEAD);
	fdActorSetAnimation(ACTOR_PIERS, ANIM_SHAKE_HEAD);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_SHAKE_HEAD);
	fdActorAnimateThenWait(ACTOR_JENNA, ANIM_SHAKE_HEAD, 33);
	fdEventWait(20);
	fdActorAnimateThenWait(ACTOR_KARST, ANIM_NOD, 20);
	fdEventWait(20);
	/* "What? You feign ignorance? Why? Would you protect him from my fury!?" */
	fdEventShowMessageAndWait(ACTOR_KARST, 0);
	scFieldActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
	scFieldActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
	scFieldActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
	scFieldActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	fdEventWait(60);
	scFieldActorShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 30);
	/* "Never mind. I believe I've heard some rumors of a traveler named..." */
	fdEventShowMessageThenWait(ACTOR_KARST, 0, 0);
	scFieldActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	scFieldActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	scFieldActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	scFieldActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	fdActorAnimateThenWait(ACTOR_KARST, ANIM_NOD, 20);
	fdEventWait(20);
	/* "Is this Isaac the same one they've been speaking of? Is he pursuing
	 * you?" */
	fdEventShowMessageAndWait(ACTOR_KARST, 0);
	fdActorFaceDirectionById(ACTOR_KARST, FACING_EAST, 0);
	fdEventWait(20);
	/* "If that is the case, then this Isaac should show up here sooner or..."
	 */
	fdEventShowMessageAndWait(ACTOR_KARST, 0);
	scFieldActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	/* "How do you know it wasn't Felix who defeated your sister?" */
	fdEventShowMessageAndWait(ACTOR_KRADEN, 0);
	fdActorRunRepeatedMotion(ACTOR_KARST, 2);
	fdEventWait(20);
	/* "What do you take me for? I'm a warrior, like my sister... I can..." */
	fdEventShowMessageAndWait(ACTOR_KARST, 0);
	fdActorAnimateThenWait(ACTOR_KARST, ANIM_SHAKE_HEAD, 33);
	fdEventWait(20);
	/* "Even if you doubled Felix's power, he would still be no match for my..."
	 */
	fdEventShowMessageAndWait(ACTOR_KARST, 0);
	scFieldActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
	scFieldActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
	scFieldActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
	scFieldActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
	fdActorFaceDirection(ACTOR_KARST, FACING_WEST, 0);
	fdEventWait(20);
	/* "And besides, I doubt Felix could find it in himself to betray my
	 * sister." */
	fdEventShowMessageAndWait(ACTOR_KARST, 0);
	scFieldActorShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "He knows that the lives of those he holds dear would hang in the..." */
	fdEventShowMessageAndWait(ACTOR_KARST, 0);
	fdActorRunRepeatedMotion(ACTOR_FELIX, 2);
	fdEventWait(20);
	fdActorAnimateThenWait(ACTOR_KARST, ANIM_NOD, 20);
	fdEventWait(20);
	/* "But I must congratulate you on your successes at both the Mercury..." */
	fdEventShowMessageAndWait(ACTOR_KARST, 0);
	fdActorAnimateThenWait(ACTOR_KARST, ANIM_SHAKE_HEAD, 33);
	fdEventWait(20);
	/* "Although your success there now prevents us from returning to the..." */
	fdEventShowMessageAndWait(ACTOR_KARST, 0);
	fdActorRunRepeatedMotion(ACTOR_KARST, 2);
	fdEventWait(20);
	/* "You would have failed, of course, had we not researched how to climb..."
	 */
	fdEventShowMessageAndWait(ACTOR_KARST, 0);
	scFieldActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	scFieldActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	scFieldActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	scFieldActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	scFieldActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	scFieldActorShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	/* "Oh, didn't Menardi tell you?" */
	fdEventShowMessageAndWait(ACTOR_KARST, 0);
	fdActorFaceDirectionAndRefreshById(ACTOR_KARST, FACING_EAST);
	fdEventWait(20);
	fdActorFaceDirectionAndWait(ACTOR_KARST, FACING_WEST);
	fdEventWait(20);
	fdActorFaceDirectionAndRefreshById(ACTOR_KARST, FACING_EAST);
	fdEventWait(20);
	/* "Regardless, Isaac won't be getting in my way." */
	fdEventShowMessageAndWait(ACTOR_KARST, 0);
	fdActorFaceDirectionById(ACTOR_KARST, FACING_WEST, 0);
	fdEventWait(20);
	/* "You will have to find your own way to the Western Sea." */
	fdEventShowMessageAndWait(ACTOR_KARST, 0);
	fdActorRunRepeatedMotion(ACTOR_KARST, 2);
	fdEventWait(20);
	/* "And if you make the journey, you'll have to light Jupiter on your..." */
	fdEventShowMessageAndWait(ACTOR_KARST, 0);
	fdActorAnimateThenWait(ACTOR_KARST, ANIM_NOD, 20);
	fdEventWait(20);
	/* "We have our own work to do, and number one on the list is taking..."
	 * Yes: */
	/* "Aren't you an obedient boy... If you're worried..." No: "What's the */
	/* matter? Afraid you can't perform..." */
	fdMessageShowAtActor(ACTOR_KARST, 0);
	if(fdEventAskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
		scFieldActorShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
		/* "Aren't you an obedient boy... If you're worried you can't get it..."
		 */
		fdEventShowMessageThenWait(ACTOR_KARST, 0, 0);
		(FieldStateWork->msg)++;
	}
	else {
		scFieldActorShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN,
							  30);
		(FieldStateWork->msg)++;
		/* "What's the matter? Afraid you can't perform your duties? We
		 * could..." */
		fdEventShowMessageThenWait(ACTOR_KARST, 0, 0);
	}
	scFieldActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD, 33);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD, 33);
	/* "No, thank you. I believe we'll just find Jupiter Lighthouse on our..."
	 */
	fdEventShowMessageAndWait(ACTOR_KRADEN, 0);
	fdActorAnimateThenWait(ACTOR_KARST, ANIM_NOD, 20);
	fdEventWait(20);
	/* "I see. Just don't let us down." */
	fdEventShowMessageAndWait(ACTOR_KARST, 0);
	fdActorRunRepeatedMotion(ACTOR_KARST, 2);
	fdEventWait(20);
	/* "We're going to find that Isaac..." */
	fdEventShowMessageAndWait(ACTOR_KARST, 0);
	fdActorFaceDirectionById(ACTOR_KARST, FACING_EAST, 0);
	fdEventWait(20);
	/* "I'm going to find this Isaac... And I swear that the last thing he..."
	 */
	fdEventShowMessageAndWait(ACTOR_KARST, 0);
	sySndPlay(SOUND_FADE_OUT_BGM);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdActorWalkByAndWaitById(ACTOR_KARST, 24, 24);
	fdActorWalkByAndWaitById(ACTOR_KARST, 132, 0);
	fdActorSetPos(ACTOR_KARST, 0, 0);
	scPlayMusic();
	fdCameraFollowActor(ACTOR_FELIX, 1);
	scFieldActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	/* "Wow. And I thought Saturos and Menardi had issues." */
	fdEventShowMessageAndWait(ACTOR_SHEBA, 0);
	fdActorAnimateThenWait(ACTOR_JENNA, ANIM_NOD, 20);
	fdEventWait(20);
	/* "We shouldn't take Karst too lightly..." */
	fdEventShowMessageAndWait(ACTOR_JENNA, 0);
	scFieldActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	fdActorFaceActorById(ACTOR_PIERS, ACTOR_FELIX, 0);
	fdEventWait(20);
	/* "Wait a moment... Karst did say "we," didn't she?" */
	fdEventShowMessageThenWait(ACTOR_PIERS, 0, 0);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
	fdEventWait(10);
	/* "Yes. Saturos and Menardi traveled as a pair. Perhaps she has a..." */
	fdEventShowMessageAndWait(ACTOR_KRADEN, 0);
	fdActorRunRepeatedMotion(ACTOR_SHEBA, 2);
	fdEventWait(20);
	/* "We ought to warn Isaac that Karst is after him. Maybe we should try..."
	 */
	fdEventShowMessageAndWait(ACTOR_SHEBA, 0);
	fdActorFaceActorById(ACTOR_JENNA, ACTOR_SHEBA, 0);
	fdEventWait(20);
	/* "Believe me, I'd love to see Isaac again, but we just don't have time..."
	 */
	fdEventShowMessageAndWait(ACTOR_JENNA, 0);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
	fdEventWait(20);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH, 0);
	fdEventWait(20);
	/* "Even if we did find them, I think there is a good chance we'd end up..."
	 */
	fdEventShowMessageAndWait(ACTOR_KRADEN, 0);
	scFieldActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
	fdActorFaceActorById(ACTOR_PIERS, ACTOR_KRADEN, 0);
	fdActorFaceActorById(ACTOR_FELIX, ACTOR_KRADEN, 0);
	fdActorFaceActorById(ACTOR_SHEBA, ACTOR_KRADEN, 0);
	fdActorFaceActorById(ACTOR_JENNA, ACTOR_KRADEN, 0);
	fdEventWait(20);
	/* "Why?" */
	fdEventShowMessageThenWait(ACTOR_PIERS, 0, 0);
	fdActorFaceActorById(ACTOR_KRADEN, ACTOR_PIERS, 0);
	fdEventWait(20);
	/* "What we are trying to achieve, they are trying to prevent... And..." */
	fdEventShowMessageAndWait(ACTOR_KRADEN, 0);
	fdActorFaceActorById(ACTOR_SHEBA, ACTOR_JENNA, 0);
	fdEventWait(20);
	/* "But, Jenna... Aren't you and Isaac an item? Couldn't you, you know..."
	 */
	fdEventShowMessageAndWait(ACTOR_SHEBA, 0);
	fdActorSetSprCol(fdActorGetById(ACTOR_JENNA), SPRITE_COLOR_RED_TINT);
	/* "A... An item? No! I mean... It's not like that! Not...really..." */
	fdEventShowMessageAndWait(ACTOR_JENNA, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
	fdEventWait(20);
	/* "...Stupid Sheba..." */
	fdEventShowMessageAndWait(ACTOR_JENNA, 0);
	fdActorTurnToFaceEachOther(ACTOR_FELIX, ACTOR_PIERS, 0);
	fdEventWait(40);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	fdEventWait(20);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH);
	/* "Ah! Well! Er, yes. For now, we should press onward to Jupiter..." */
	fdEventShowMessageAndWait(ACTOR_KRADEN, 0);
	fdActorSetSprCol(fdActorGetById(ACTOR_JENNA), SPRITE_COLOR_NORMAL);
	fdEventWait(20);
	scFieldActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	fdActorFaceActorById(ACTOR_JENNA, ACTOR_KRADEN, 0);
	fdEventWait(20);
	/* "But...what about Garet and the others?" */
	fdEventShowMessageAndWait(ACTOR_JENNA, 0);
	scFieldActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "Don't you mean...Isaac?" */
	fdEventShowMessageAndWait(ACTOR_SHEBA, 0);
	fdActorSetSprCol(fdActorGetById(ACTOR_JENNA), SPRITE_COLOR_RED_TINT);
	fdActorFaceActorById(ACTOR_JENNA, ACTOR_SHEBA, 0);
	fdEventWait(20);
	/* "Sheba! Be quiet! Sheesh..." */
	fdEventShowMessageAndWait(ACTOR_JENNA, 0);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	fdEventWait(20);
	/* "*Ahem!* Are you ready to listen yet?" */
	fdEventShowMessageAndWait(ACTOR_KRADEN, 0);
	fdActorSetSprCol(fdActorGetById(ACTOR_JENNA), SPRITE_COLOR_NORMAL);
	fdActorFaceActorById(ACTOR_FELIX, ACTOR_KRADEN, 0);
	fdActorFaceActorById(ACTOR_JENNA, ACTOR_KRADEN, 0);
	fdActorFaceActorById(ACTOR_SHEBA, ACTOR_KRADEN, 0);
	fdActorFaceActorById(ACTOR_PIERS, ACTOR_KRADEN, 0);
	fdEventWait(40);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
	fdEventWait(20);
	/* "They were strong enough to defeat Saturos and Menardi. They won't be..."
	 */
	fdEventShowMessageAndWait(ACTOR_KRADEN, 0);
	fdActorSetAnimation(ACTOR_FELIX, ANIM_NOD);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
	fdActorAnimateThenWait(ACTOR_PIERS, ANIM_NOD, 20);
	fdEventWait(20);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
	fdEventWait(20);
	/* "We should continue on ahead, just as Saturos and Menardi would have..."
	 */
	fdEventShowMessageAndWait(ACTOR_KRADEN, 0);
	fdActorAnimateThenWait(ACTOR_JENNA, ANIM_NOD, 20);
	fdEventWait(20);
	scFieldActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "Sheba? Piers? Are you ready?" */
	fdEventShowMessageAndWait(ACTOR_KRADEN, 0);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
	fdActorAnimateThenWait(ACTOR_PIERS, ANIM_NOD, 20);
	fdEventWait(20);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
	fdEventWait(20);
	fdActorFaceDirectionAndRefreshById(ACTOR_KRADEN, FACING_EAST);
	/* "Then we should be going, right, Felix?" */
	fdEventShowMessageAndWait(ACTOR_KRADEN, 0);
	fdActorAnimateThenWait(ACTOR_FELIX, ANIM_NOD, 20);
	fdEventWait(20);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
	fdActorAnimateThenWait(ACTOR_JENNA, ANIM_NOD, 20);
	fdActorAnimateThenWait(ACTOR_FELIX, ANIM_NOD, 20);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
	fdActorSetSpeed(ACTOR_KRADEN, 1.2);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_WALK);
	actor = fdActorGetById(ACTOR_FELIX);
	if(actor != NULL)
		fdActorSetDestinationById(ACTOR_KRADEN, actor->x.part[0].integer,
								  actor->z.part[0].integer);
	fdActorWaitForMoveById(ACTOR_KRADEN);
	fdActorSetPos(ACTOR_KRADEN, 0, 0);
	fdActorSetSpeed(ACTOR_JENNA, 1.2);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_WALK);
	actor = fdActorGetById(ACTOR_FELIX);
	if(actor != NULL)
		fdActorSetDestinationById(ACTOR_JENNA, actor->x.part[0].integer,
								  actor->z.part[0].integer);
	fdActorWaitForMoveById(ACTOR_JENNA);
	fdActorSetPos(ACTOR_JENNA, 0, 0);
	fdActorSetSpeed(ACTOR_SHEBA, 1.2);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_WALK);
	actor = fdActorGetById(ACTOR_FELIX);
	if(actor != NULL)
		fdActorSetDestinationById(ACTOR_SHEBA, actor->x.part[0].integer,
								  actor->z.part[0].integer);
	fdActorWaitForMoveById(ACTOR_SHEBA);
	fdActorSetPos(ACTOR_SHEBA, 0, 0);
	fdActorSetSpeed(ACTOR_PIERS, 1.2);
	fdActorSetAnimation(ACTOR_PIERS, ANIM_WALK);
	actor = fdActorGetById(ACTOR_FELIX);
	if(actor != NULL)
		fdActorSetDestinationById(ACTOR_PIERS, actor->x.part[0].integer,
								  actor->z.part[0].integer);
	fdActorWaitForMoveById(ACTOR_PIERS);
	fdActorSetPos(ACTOR_PIERS, 0, 0);
	syGameFlagSet(FLAG_MENARDIS_SHIP_SEEN);
	syGameFlagSet(FLAG_MADRA_KARST_MET);
	fdEventEnd();
}

/* Nothing visible happens when Felix walks south out through the gate (cells */
/* (29, 25) and (30, 25)) without having been inspected; the game marks the */
/* inspection done, so that the guards do not stop him on the cells south of */
/* it. */
void scMadraSkipGateInspection(void) {
	syGameFlagSet(FLAG_MADRA_GATE_INSPECTED);
}

/* Nothing visible happens when Felix walks south out through the gate (cells */
/* (29, 25) and (30, 25)) after the Kibombo's raid without having been */
/* questioned; the game marks the questioning done, so that the guards do not */
/* stop him on the cells south of it. */
void scMadraSkipRaidQuestioning(void) {
	syGameFlagSet(FLAG_MADRA_RAID_QUESTIONING_DONE);
}

/* The last of the ladder tops' step triggers that lead down to the */
/* catacombs' east ladders; those above it lead to the west ones. No cell of */
/* Madra's map has one. */
enum { CELL_BEHAVIOR_MADRA_EAST_LADDERS_LAST = 34 };

/* Felix, already beside a ladder top, turns north, steps onto the rungs and */
/* climbs down out of sight to the catacombs' east or west ladders, by the */
/* ladder top's step trigger. Only the unused functions below call it. */
void scMadraClimbDownLadder(void) {
	s32 next;

	next = MADRA_TO_MADRA_CATACOMBS_LADDERS_EAST;
	if(FieldStateWork->stepTrigger > CELL_BEHAVIOR_MADRA_EAST_LADDERS_LAST)
		next = MADRA_TO_MADRA_CATACOMBS_LADDERS_WEST;
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	fdEventWait(10);
	fdActorGetById(ACTOR_FELIX)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorSetDestinationOffset(ACTOR_FELIX, 0, 4);
	fdActorWaitForMoveById(ACTOR_FELIX);
	fdActorSetSprFlags(fdActorGetById(ACTOR_FELIX), 0);
	fdActorSetAnimation(ACTOR_FELIX, ANIM_LEADER_CLIMB_DOWN_FROM_TOP);
	fdActorSetDestinationOffset(ACTOR_FELIX, 0, 16);
	fdActorWaitForMoveById(ACTOR_FELIX);
	fdActorSetAnimation(ACTOR_FELIX, ANIM_LEADER_CLIMB);
	fdEventWait(10);
	fdEventPlaySnd(SE_MAP_EXIT);
	syImportEventSetExitRequest(next);
	fdEventEnd();
}

/* Unused: Felix steps up to a ladder top from its west side and climbs down; */
/* no trigger runs it. */
UNUSED void scMadraStepOntoLadderFacingEast(void) {
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdActorWalkByAndWait(ACTOR_FELIX, 0, -14);
	fdActorWalkByAndWait(ACTOR_FELIX, 16, 0);
	scMadraClimbDownLadder();
}

/* Unused: Felix steps up to a ladder top from its east side and climbs down; */
/* no trigger runs it. */
UNUSED void scMadraStepOntoLadderFacingWest(void) {
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdActorWalkByAndWait(ACTOR_FELIX, -2, -6);
	fdActorWalkByAndWait(ACTOR_FELIX, -14, -8);
	scMadraClimbDownLadder();
}

/* Unused: Felix turns north on the spot and climbs down; no trigger runs it. */
UNUSED void scMadraTurnAndClimbDownLadder(void) {
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	scMadraClimbDownLadder();
}

#include "shared/scene_entrances_default.c"

/* clang-format off */
u32 SceneExits[] = {
    SCENE_EXITS(Madra),
    SCENE_EXIT_UNLESS(MADRA_TO_MADRA_ROOMS_CAVERN_HOMES_EAST, MadraRoomsCavernHomes, MADRA_ROOMS_CAVERN_HOMES_FROM_MADRA_EAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MADRA_TO_MADRA_ROOMS_HOUSES_SOUTHWEST, MadraRoomsHouses, MADRA_ROOMS_HOUSES_FROM_MADRA_SOUTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MADRA_TO_MADRA_ROOMS_HOUSES_SOUTH_A, MadraRoomsHouses, MADRA_ROOMS_HOUSES_FROM_MADRA_SOUTH_A, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MADRA_TO_MADRA_ROOMS_HOUSES_SOUTHEAST, MadraRoomsHouses, MADRA_ROOMS_HOUSES_FROM_MADRA_SOUTHEAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MADRA_TO_MADRA_ROOMS_HOUSES_NORTHEAST_A, MadraRoomsHouses, MADRA_ROOMS_HOUSES_FROM_MADRA_NORTHEAST_A, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MADRA_TO_MADRA_ROOMS_HOUSES_NORTHEAST_B, MadraRoomsHouses, MADRA_ROOMS_HOUSES_FROM_MADRA_NORTHEAST_B, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MADRA_TO_MADRA_ROOMS_HOUSES_NORTHWEST, MadraRoomsHouses, MADRA_ROOMS_HOUSES_FROM_MADRA_NORTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MADRA_TO_MADRA_ROOMS_HOUSES_SOUTH_B, MadraRoomsHouses, MADRA_ROOMS_HOUSES_FROM_MADRA_SOUTH_B, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MADRA_TO_MADRA_ROOMS_HOUSES_WEST, MadraRoomsHouses, MADRA_ROOMS_HOUSES_FROM_MADRA_WEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MADRA_TO_MADRA_ROOMS_CAVERN_HOMES_WEST, MadraRoomsCavernHomes, MADRA_ROOMS_CAVERN_HOMES_FROM_MADRA_WEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MADRA_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_MADRA, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MADRA_TO_MADRA_CATACOMBS_LADDERS_WEST, MadraCatacombsLadders, MADRA_CATACOMBS_LADDERS_FROM_MADRA_WEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MADRA_TO_MADRA_CATACOMBS_LADDERS_EAST, MadraCatacombsLadders, MADRA_CATACOMBS_LADDERS_FROM_MADRA_EAST, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

/* The town until the party returns with Piers (MADRA_ACTOR_ID). */
ACTOR_PLACE Madra_Placements[] = {
    ACTOR_PLACEMENT(SPRITE_MAN_IN_GREEN_VEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 296, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_WHITE_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 744, 0, 392, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_MAN_IN_ORANGE_VEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 360, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_AUBURN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 440, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_BLUE_COAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 280, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_IN_GREEN_SHAWL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 384, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_IN_BLUE_SHIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 274, 0, 510, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_BLONDE_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 376, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_BLUE_COAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 472, 0, 264, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_MAN_IN_ORANGE_VEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 232, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_RED_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 472, 0, 296, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_BLONDE_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 552, 0, 259, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_GREEN_VEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 472, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_WHITE_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 440, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 360, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 128, 0, 360, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_GREEN_VEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 408, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_GREEN_VEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 408, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_GREEN_VEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 744, 0, 248, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, IF_FLAG_SET(FLAG_PIERS_INTERROGATED), MOVEMENT_TYPE_STAND, 112, 0, 440, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, IF_FLAG_SET(FLAG_PIERS_INTERROGATED), MOVEMENT_TYPE_STAND, 88, 0, 440, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_MAN_IN_RED, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 272, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 256, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_PONYTAIL, IF_FLAG_SET(FLAG_BRIGGS_CAUGHT), MOVEMENT_TYPE_STAND, 280, 0, 256, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_BLUE_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MADRAN_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, FLAG_BRIGGS_CAUGHT, MOVEMENT_TYPE_STAND, 280, 0, 256, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

/* The town once the party returns with Piers (MADRA_ACTOR_ID, then */
/* MADRA_ON_RETURN_ACTOR_ID). */
ACTOR_PLACE Madra_PlacementsOnReturn[] = {
    ACTOR_PLACEMENT(SPRITE_MAN_IN_GREEN_VEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 150, 0, 344, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_WHITE_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 744, 0, 392, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_MAN_IN_ORANGE_VEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 360, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_AUBURN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 440, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_BLUE_COAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 280, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_IN_GREEN_SHAWL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 392, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_IN_BLUE_SHIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 274, 0, 510, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_BLONDE_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 376, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_BLUE_COAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 472, 0, 264, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_MAN_IN_ORANGE_VEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 232, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_RED_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 472, 0, 296, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_BLONDE_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 552, 0, 259, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_GREEN_VEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 472, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_WHITE_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 440, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_GREEN_VEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 408, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_GREEN_VEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 408, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_GREEN_VEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, IF_FLAG_SET(FLAG_PIERS_INTERROGATED), MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, IF_FLAG_SET(FLAG_PIERS_INTERROGATED), MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_MAN_IN_RED, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 272, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 256, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_PONYTAIL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 256, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_KARST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 276, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MADRAS_MAYOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 112, 0, 393, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

/* A door opening: the one cell animation step of most of the town's doors. */
u16 Madra_DoorOpening[] = {
    MAPGRID_ANIMATION_STEP(1, 39, 1, 2, 6),
    MAPGRID_ANIMATION_STEPS_END,
};

/* The door opening of the left cavern home's two doors, from other cells. */
u16 Madra_CavernHomeDoorOpening[] = {
    MAPGRID_ANIMATION_STEP(3, 39, 1, 2, 6),
    MAPGRID_ANIMATION_STEPS_END,
};
/* clang-format on */

/* The town's doors that open with a cell animation as the party walks through,
 */
/* by the exits they lead by. */
MAP_GRID_CELL_ANIMATION Madra_Doors[] = {
	{Madra_CavernHomeDoorOpening, 20, 3},
	{Madra_DoorOpening, 13, 23},
	{Madra_DoorOpening, 17, 17},
	{Madra_DoorOpening, 23, 18},
	{Madra_DoorOpening, 28, 10},
	{Madra_DoorOpening, 28, 6},
	{Madra_DoorOpening, 9, 6},
	{Madra_DoorOpening, 19, 24},
	{Madra_DoorOpening, 0, 0}, /* the jail's doorway has no door */
	{Madra_CavernHomeDoorOpening, 17, 3},
};

/* The town's doors, gate, talk and Mind Read before the Kibombo raid. */
TRIGGER Madra_TriggersBeforeRaid[] = {
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
	 MADRA_TO_MADRA_ROOMS_CAVERN_HOMES_EAST, CONDITION_ALWAYS,
	 EVENT(scMadraEnterDoor)},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
	 MADRA_TO_MADRA_ROOMS_HOUSES_SOUTHWEST, CONDITION_ALWAYS,
	 EVENT(scMadraEnterDoor)},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
	 MADRA_TO_MADRA_ROOMS_HOUSES_SOUTH_A, CONDITION_ALWAYS,
	 EVENT(scMadraEnterDoor)},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
	 MADRA_TO_MADRA_ROOMS_HOUSES_SOUTHEAST, CONDITION_ALWAYS,
	 EVENT(scMadraEnterDoor)},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
	 MADRA_TO_MADRA_ROOMS_HOUSES_NORTHEAST_A, CONDITION_ALWAYS,
	 EVENT(scMadraEnterDoor)},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
	 MADRA_TO_MADRA_ROOMS_HOUSES_NORTHEAST_B, CONDITION_ALWAYS,
	 EVENT(scMadraEnterDoor)},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
	 MADRA_TO_MADRA_ROOMS_HOUSES_NORTHWEST, CONDITION_ALWAYS,
	 EVENT(scMadraEnterDoor)},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
	 MADRA_TO_MADRA_ROOMS_HOUSES_SOUTH_B, CONDITION_ALWAYS,
	 EVENT(scMadraEnterDoor)},
	{TRIGGER_EXIT, MADRA_TO_MADRA_ROOMS_HOUSES_WEST, CONDITION_ALWAYS,
	 MADRA_TO_MADRA_ROOMS_HOUSES_WEST},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
	 MADRA_TO_MADRA_ROOMS_CAVERN_HOMES_WEST, CONDITION_ALWAYS,
	 EVENT(scMadraEnterDoor)},
	{TRIGGER_EXIT, MADRA_TO_WORLD_MAP, CONDITION_ALWAYS, MADRA_TO_WORLD_MAP},
	{TRIGGER_EXIT, MADRA_TO_MADRA_CATACOMBS_LADDERS_WEST, CONDITION_ALWAYS,
	 MADRA_TO_MADRA_CATACOMBS_LADDERS_WEST},
	{TRIGGER_STEP, CELL_BEHAVIOR_MADRA_CHAMPAS_HOLE, CONDITION_ALWAYS,
	 EVENT(scMadraWorkerBlocksHole)},
	{TRIGGER_STEP, CELL_BEHAVIOR_MADRA_GATE_INSIDE, FLAG_MADRA_GATE_INSPECTED,
	 EVENT(scMadraSkipGateInspection)},
	{TRIGGER_STEP, CELL_BEHAVIOR_MADRA_GATE_OUTSIDE, FLAG_MADRA_GATE_INSPECTED,
	 EVENT(scMadraInspectPartyAtGate)},
	{TRIGGER_TALK, ACTOR_YOUNG_MAN_ON_THE_ELDER, CONDITION_ALWAYS,
	 MSGNO(MsgMadraTownElderTookOff)},
	{TRIGGER_TALK, ACTOR_YOUNG_WOMAN_ON_THE_DISASTERS, CONDITION_ALWAYS,
	 MSGNO(MsgMadraYouPickedWrongTime)},
	{TRIGGER_TALK, ACTOR_OLD_MAN_ON_THE_WAVE, CONDITION_ALWAYS,
	 MSGNO(MsgMadraThatBigWaveMovedWhole)},
	{TRIGGER_TALK, ACTOR_BOAT_OWNER, CONDITION_ALWAYS,
	 MSGNO(MsgMadraLetMeGuessYouWant)},
	{TRIGGER_TALK, ACTOR_OLD_MAN_ON_THE_CAPTURED_CHAMPA, CONDITION_ALWAYS,
	 MSGNO(MsgMadraMaybeWellLearnBriggssWhereabouts)},
	{TRIGGER_TALK, ACTOR_OLD_WOMAN_ON_AIRS_ROCK, CONDITION_ALWAYS,
	 MSGNO(MsgMadraIfICouldWalk)},
	{TRIGGER_TALK, ACTOR_BRIGGS_FAN, CONDITION_ALWAYS,
	 EVENT(scMadraAskAboutBriggs)},
	{TRIGGER_TALK, ACTOR_GIRL_ON_THE_SUPERCONTINENT, CONDITION_ALWAYS,
	 MSGNO(MsgMadraItsLikeAllThreeContinents)},
	{TRIGGER_TALK, ACTOR_OLD_MAN_WHO_SCARED_THE_CHAMPA, CONDITION_ALWAYS,
	 MSGNO(MsgMadraChampaWereSwarmingOverVillage)},
	{TRIGGER_TALK, ACTOR_OLD_MAN_ON_THE_CHAMPAS_HOLE, CONDITION_ALWAYS,
	 MSGNO(MsgMadraThoseChampaCursDugHole)},
	{TRIGGER_TALK, ACTOR_MAN_ON_THE_CHAMPAS_PLAN, CONDITION_ALWAYS,
	 MSGNO(MsgMadraIDoubtChampaCouldHave)},
	{TRIGGER_TALK, ACTOR_GIRL_ON_THE_BIG_HOLE, CONDITION_ALWAYS,
	 MSGNO(MsgMadraIGotSomeoneToShow)},
	{TRIGGER_TALK, ACTOR_YOUNG_MAN_AGAINST_THE_PRISONER, CONDITION_ALWAYS,
	 MSGNO(MsgMadraIDontCareWhatElder)},
	{TRIGGER_TALK, ACTOR_YOUNG_WOMAN_ON_THE_PRISONER, CONDITION_ALWAYS,
	 MSGNO(MsgMadraWeTookThatUnconsciousGuy)},
	{TRIGGER_TALK, ACTOR_GUARD_ON_HIS_FRIEND, CONDITION_ALWAYS,
	 MSGNO(MsgMadraMyFriendWasOnGuard)},
	{TRIGGER_TALK, ACTOR_GUARD_ON_HIS_PARTNER, CONDITION_ALWAYS,
	 MSGNO(MsgMadraMyPartnerWasGuardingPrison)},
	{TRIGGER_TALK, ACTOR_GATE_GUARD, CONDITION_ALWAYS,
	 MSGNO(MsgMadraTheresNoWayThosePirates)},
	{TRIGGER_TALK, ACTOR_SECOND_GATE_GUARD, CONDITION_ALWAYS,
	 MSGNO(MsgMadraAllOfIndraShifted)},
	{TRIGGER_TALK, ACTOR_HOLE_WORKER, CONDITION_ALWAYS,
	 EVENT(scMadraAskComingThroughHole)},
	{TRIGGER_TALK, ACTOR_ICE_SHAFT_WITNESS, CONDITION_ALWAYS,
	 EVENT(scMadraAskAboutIceShaft)},
	{TRIGGER_TALK, ACTOR_FRIGHTENED_WITNESS, CONDITION_ALWAYS,
	 MSGNO(MsgMadraEvenIfWeTried)},
	{TRIGGER_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS,
	 EVENT(scMadraTalkToWeaponMerchant)},
	{TRIGGER_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS,
	 EVENT(scMadraTalkToArmorMerchant)},
	{TRIGGER_TALK, ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS,
	 EVENT(scMadraTalkToItemMerchant)},
	{TRIGGER_TALK, ACTOR_ITEM_MERCHANTS_SIBLING, CONDITION_ALWAYS,
	 EVENT(scMadraTalkToItemMerchant)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_ON_THE_ELDER,
	 CONDITION_ALWAYS, MSGNO(MsgMadraEveryoneThinksBriggsFled)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_WOMAN_ON_THE_DISASTERS,
	 CONDITION_ALWAYS, MSGNO(MsgMadraTownElderIsGettingPretty)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_THE_WAVE,
	 CONDITION_ALWAYS, MSGNO(MsgMadraIfBigWaveCanMove)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOAT_OWNER, CONDITION_ALWAYS,
	 MSGNO(MsgMadraItsBitOfProblem)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_THE_CAPTURED_CHAMPA,
	 CONDITION_ALWAYS, MSGNO(MsgMadraThatChampaInCellLooks)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_AIRS_ROCK,
	 CONDITION_ALWAYS, MSGNO(MsgMadraIveAlwaysWantedToSee)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
	 ACTOR_BRIGGS_FAN | TRIGGER_SUBJECT_OWN_EFFECT, FLAG_MADRA_BOY_HAS_ASKED,
	 EVENT(scMadraMindReadBriggsFan)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BRIGGS_FAN, CONDITION_ALWAYS,
	 MSGNO(MsgMadraWhoWasItWhoTold)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_ON_THE_SUPERCONTINENT,
	 CONDITION_ALWAYS, MSGNO(MsgMadraNowMadraIsPinnedBetween)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_WHO_SCARED_THE_CHAMPA,
	 CONDITION_ALWAYS, MSGNO(MsgMadraWhenISawThoseChampa)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_THE_CHAMPAS_HOLE,
	 CONDITION_ALWAYS, MSGNO(MsgMadraChampaMustHaveKnownExactly)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_CHAMPAS_PLAN,
	 CONDITION_ALWAYS, MSGNO(MsgMadraItsNotLikeChampa)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_ON_THE_BIG_HOLE,
	 CONDITION_ALWAYS, MSGNO(MsgMadraImNotSureChampaActually)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_AGAINST_THE_PRISONER,
	 CONDITION_ALWAYS, MSGNO(MsgMadraIllNeverForgiveThoseDogs)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_WOMAN_ON_THE_PRISONER,
	 CONDITION_ALWAYS, MSGNO(MsgMadraILookedIntoThatMans)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GUARD_ON_HIS_FRIEND,
	 CONDITION_ALWAYS, MSGNO(MsgMadraIWonderHowMany)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GUARD_ON_HIS_PARTNER,
	 CONDITION_ALWAYS, MSGNO(MsgMadraIMissMyPartner)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GATE_GUARD, CONDITION_ALWAYS,
	 MSGNO(MsgMadraItsSoWeird)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_GATE_GUARD,
	 CONDITION_ALWAYS, MSGNO(MsgMadraTheseAreRoughTimes)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HOLE_WORKER, CONDITION_ALWAYS,
	 MSGNO(MsgMadraChampaDugThisHoleWhen)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ICE_SHAFT_WITNESS,
	 CONDITION_ALWAYS, MSGNO(MsgMadraItWasAllJustDream)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FRIGHTENED_WITNESS,
	 CONDITION_ALWAYS, MSGNO(MsgMadraMySoreBottomTellsMe)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT,
	 CONDITION_ALWAYS, MSGNO(MsgMadraItemVendorGotWounded)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT,
	 CONDITION_ALWAYS, MSGNO(MsgMadraThoseLousyChampaNextTime)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANT,
	 CONDITION_ALWAYS, MSGNO(MsgMadraBriggsAndHisMen)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANTS_SIBLING,
	 CONDITION_ALWAYS, MSGNO(MsgMadraBriggsAndHisMen)},
	{TRIGGER_TABLE_END},
};

/* The town's doors, gate, talk and Mind Read once the Kibombo have raided */
/* Madra. */
TRIGGER Madra_TriggersAfterRaid[] = {
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
	 MADRA_TO_MADRA_ROOMS_CAVERN_HOMES_EAST, CONDITION_ALWAYS,
	 EVENT(scMadraEnterDoor)},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
	 MADRA_TO_MADRA_ROOMS_HOUSES_SOUTHWEST, CONDITION_ALWAYS,
	 EVENT(scMadraEnterDoor)},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
	 MADRA_TO_MADRA_ROOMS_HOUSES_SOUTH_A, CONDITION_ALWAYS,
	 EVENT(scMadraEnterDoor)},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
	 MADRA_TO_MADRA_ROOMS_HOUSES_SOUTHEAST, CONDITION_ALWAYS,
	 EVENT(scMadraEnterDoor)},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
	 MADRA_TO_MADRA_ROOMS_HOUSES_NORTHEAST_A, CONDITION_ALWAYS,
	 EVENT(scMadraEnterDoor)},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
	 MADRA_TO_MADRA_ROOMS_HOUSES_NORTHEAST_B, CONDITION_ALWAYS,
	 EVENT(scMadraEnterDoor)},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
	 MADRA_TO_MADRA_ROOMS_HOUSES_NORTHWEST, CONDITION_ALWAYS,
	 EVENT(scMadraEnterDoor)},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
	 MADRA_TO_MADRA_ROOMS_HOUSES_SOUTH_B, CONDITION_ALWAYS,
	 EVENT(scMadraEnterDoor)},
	{TRIGGER_EXIT, MADRA_TO_MADRA_ROOMS_HOUSES_WEST, CONDITION_ALWAYS,
	 MADRA_TO_MADRA_ROOMS_HOUSES_WEST},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
	 MADRA_TO_MADRA_ROOMS_CAVERN_HOMES_WEST, CONDITION_ALWAYS,
	 EVENT(scMadraEnterDoor)},
	{TRIGGER_EXIT, MADRA_TO_WORLD_MAP, CONDITION_ALWAYS, MADRA_TO_WORLD_MAP},
	{TRIGGER_EXIT, MADRA_TO_MADRA_CATACOMBS_LADDERS_WEST, CONDITION_ALWAYS,
	 MADRA_TO_MADRA_CATACOMBS_LADDERS_WEST},
	{TRIGGER_STEP, CELL_BEHAVIOR_MADRA_CHAMPAS_HOLE, CONDITION_ALWAYS,
	 EVENT(scMadraWorkerBlocksSealedHole)},
	{TRIGGER_STEP, CELL_BEHAVIOR_MADRA_GATE_INSIDE,
	 FLAG_MADRA_RAID_QUESTIONING_DONE, EVENT(scMadraSkipRaidQuestioning)},
	{TRIGGER_STEP, CELL_BEHAVIOR_MADRA_GATE_OUTSIDE,
	 FLAG_MADRA_RAID_QUESTIONING_DONE, EVENT(scMadraStopPartyAfterRaid)},
	{TRIGGER_TALK, ACTOR_YOUNG_MAN_ON_THE_ELDER, CONDITION_ALWAYS,
	 MSGNO(MsgMadraWeThinkKibomboCameInto)},
	{TRIGGER_TALK, ACTOR_YOUNG_WOMAN_ON_THE_DISASTERS, CONDITION_ALWAYS,
	 MSGNO(MsgMadraSoKibomboGotIntoMadra)},
	{TRIGGER_TALK, ACTOR_OLD_MAN_ON_THE_WAVE, CONDITION_ALWAYS,
	 MSGNO(MsgMadraKibomboCameInThroughBroken)},
	{TRIGGER_TALK, ACTOR_BOAT_OWNER, CONDITION_ALWAYS,
	 MSGNO(MsgMadraIHeardThatElderGot)},
	{TRIGGER_TALK, ACTOR_OLD_MAN_ON_THE_CAPTURED_CHAMPA, CONDITION_ALWAYS,
	 MSGNO(MsgMadraIGuessThatGuy)},
	{TRIGGER_TALK, ACTOR_OLD_WOMAN_ON_AIRS_ROCK, CONDITION_ALWAYS,
	 MSGNO(MsgMadraGondowanSoundsLikeFrighteningPlace)},
	{TRIGGER_TALK, ACTOR_BRIGGS_FAN, CONDITION_ALWAYS,
	 EVENT(scMadraAskAboutBriggs)},
	{TRIGGER_TALK, ACTOR_GIRL_ON_THE_SUPERCONTINENT, CONDITION_ALWAYS,
	 MSGNO(MsgMadraICantBelieveSomeoneStomped)},
	{TRIGGER_TALK, ACTOR_OLD_MAN_WHO_SCARED_THE_CHAMPA, CONDITION_ALWAYS,
	 MSGNO(MsgMadraNowThatWereSealingOff)},
	{TRIGGER_TALK, ACTOR_OLD_MAN_ON_THE_CHAMPAS_HOLE, CONDITION_ALWAYS,
	 MSGNO(MsgMadraThereAreStillPlenty)},
	{TRIGGER_TALK, ACTOR_MAN_ON_THE_CHAMPAS_PLAN, CONDITION_ALWAYS,
	 MSGNO(MsgMadraBriggsDidntHaveNearly)},
	{TRIGGER_TALK, ACTOR_GIRL_ON_THE_BIG_HOLE, CONDITION_ALWAYS,
	 MSGNO(MsgMadraTheseCavesWereMySecret)},
	{TRIGGER_TALK, ACTOR_YOUNG_MAN_AGAINST_THE_PRISONER, CONDITION_ALWAYS,
	 MSGNO(MsgThatGuyWasn)},
	{TRIGGER_TALK, ACTOR_YOUNG_WOMAN_ON_THE_PRISONER, CONDITION_ALWAYS,
	 MSGNO(MsgOhWeReleased)},
	{TRIGGER_TALK, ACTOR_GUARD_ON_HIS_FRIEND, CONDITION_ALWAYS,
	 MSGNO(MsgMadraEveryoneYelledAtMe)},
	{TRIGGER_TALK, ACTOR_GUARD_ON_HIS_PARTNER, CONDITION_ALWAYS,
	 MSGNO(MsgMadraYouShouldaSeenMyPartner)},
	{TRIGGER_TALK, ACTOR_GATE_GUARD, CONDITION_ALWAYS,
	 MSGNO(MsgMadraFirstPiratesRaidOurTown)},
	{TRIGGER_TALK, ACTOR_SECOND_GATE_GUARD, CONDITION_ALWAYS,
	 MSGNO(MsgMadraMyGuessIsKibomboHave)},
	{TRIGGER_TALK, ACTOR_HOLE_WORKER, CONDITION_ALWAYS,
	 EVENT(scMadraAskComingThroughSealedHole)},
	{TRIGGER_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS,
	 EVENT(scMadraTalkToWeaponMerchant)},
	{TRIGGER_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS,
	 EVENT(scMadraTalkToArmorMerchant)},
	{TRIGGER_TALK, ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS,
	 EVENT(scMadraTalkToItemMerchant)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_ON_THE_ELDER,
	 CONDITION_ALWAYS, MSGNO(MsgMadraIfItWerent)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_WOMAN_ON_THE_DISASTERS,
	 CONDITION_ALWAYS, MSGNO(MsgMadraIfGondowanIsReally)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_THE_WAVE,
	 CONDITION_ALWAYS, MSGNO(MsgMadraItsStrangeItsAlmost)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOAT_OWNER, CONDITION_ALWAYS,
	 MSGNO(MsgMadraMayorIsSupposed)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_THE_CAPTURED_CHAMPA,
	 CONDITION_ALWAYS, MSGNO(MsgMadraHeToldUsHe)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_AIRS_ROCK,
	 CONDITION_ALWAYS, MSGNO(MsgMadraInGondowanTheyHaveEvil)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BRIGGS_FAN, CONDITION_ALWAYS,
	 MSGNO(MsgMadraIHeardGuysWhoCaptured)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_ON_THE_SUPERCONTINENT,
	 CONDITION_ALWAYS, MSGNO(MsgMadraIfTheyWereStrongEnough)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_WHO_SCARED_THE_CHAMPA,
	 CONDITION_ALWAYS, MSGNO(MsgMadraItWasSoConvenientBeing)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_THE_CHAMPAS_HOLE,
	 CONDITION_ALWAYS, MSGNO(MsgMadraIWonderWhoDugThose)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_CHAMPAS_PLAN,
	 CONDITION_ALWAYS, MSGNO(MsgMadraWaitIfBriggsOnlyHas)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_ON_THE_BIG_HOLE,
	 CONDITION_ALWAYS, MSGNO(MsgMadraIveNeverSeenAnyMonsters)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_AGAINST_THE_PRISONER,
	 CONDITION_ALWAYS, MSGNO(MsgDidnTSeem)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_WOMAN_ON_THE_PRISONER,
	 CONDITION_ALWAYS, MSGNO(MsgWasSoHappy)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GUARD_ON_HIS_FRIEND,
	 CONDITION_ALWAYS, MSGNO(MsgMadraIDidntHaveChance)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GUARD_ON_HIS_PARTNER,
	 CONDITION_ALWAYS, MSGNO(MsgMadraICantBelieveNobodySeems)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GATE_GUARD, CONDITION_ALWAYS,
	 MSGNO(MsgMadraWayThingsHaveBeenGoing)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_GATE_GUARD,
	 CONDITION_ALWAYS, MSGNO(MsgMadraIveBeenStanding)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HOLE_WORKER, CONDITION_ALWAYS,
	 MSGNO(MsgMadraWeSealedUpHole)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT,
	 CONDITION_ALWAYS, MSGNO(MsgMadraChampaWeaponsWeFoundBefore)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT,
	 CONDITION_ALWAYS, MSGNO(MsgMadraGroupThatAttackedFromEast)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANT,
	 CONDITION_ALWAYS, MSGNO(MsgMadraThatShinIsAlwaysBugging)},
	{TRIGGER_TABLE_END},
};

/* The town's doors, talk and Mind Read once the party returns with Piers, and
 */
/* Karst's visit. */
TRIGGER Madra_TriggersOnReturn[] = {
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
	 MADRA_TO_MADRA_ROOMS_CAVERN_HOMES_EAST, CONDITION_ALWAYS,
	 EVENT(scMadraEnterDoor)},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
	 MADRA_TO_MADRA_ROOMS_HOUSES_SOUTHWEST, CONDITION_ALWAYS,
	 EVENT(scMadraEnterDoor)},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
	 MADRA_TO_MADRA_ROOMS_HOUSES_SOUTH_A, CONDITION_ALWAYS,
	 EVENT(scMadraEnterDoor)},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
	 MADRA_TO_MADRA_ROOMS_HOUSES_SOUTHEAST, CONDITION_ALWAYS,
	 EVENT(scMadraEnterDoor)},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
	 MADRA_TO_MADRA_ROOMS_HOUSES_NORTHEAST_A, CONDITION_ALWAYS,
	 EVENT(scMadraEnterDoor)},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
	 MADRA_TO_MADRA_ROOMS_HOUSES_NORTHEAST_B, CONDITION_ALWAYS,
	 EVENT(scMadraEnterDoor)},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
	 MADRA_TO_MADRA_ROOMS_HOUSES_NORTHWEST, CONDITION_ALWAYS,
	 EVENT(scMadraEnterDoor)},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
	 MADRA_TO_MADRA_ROOMS_HOUSES_SOUTH_B, CONDITION_ALWAYS,
	 EVENT(scMadraEnterDoor)},
	{TRIGGER_EXIT, MADRA_TO_MADRA_ROOMS_HOUSES_WEST, CONDITION_ALWAYS,
	 MADRA_TO_MADRA_ROOMS_HOUSES_WEST},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
	 MADRA_TO_MADRA_ROOMS_CAVERN_HOMES_WEST, CONDITION_ALWAYS,
	 EVENT(scMadraEnterDoor)},
	{TRIGGER_EXIT, MADRA_TO_WORLD_MAP, CONDITION_ALWAYS, MADRA_TO_WORLD_MAP},
	{TRIGGER_EXIT, MADRA_TO_MADRA_CATACOMBS_LADDERS_WEST, CONDITION_ALWAYS,
	 MADRA_TO_MADRA_CATACOMBS_LADDERS_WEST},
	{TRIGGER_STEP, CELL_BEHAVIOR_MADRA_PATH_PAST_KARST, FLAG_MADRA_KARST_MET,
	 EVENT(scMadraKarstCallsForMenardi)},
	{TRIGGER_TALK, ACTOR_YOUNG_MAN_ON_THE_ELDER, CONDITION_ALWAYS,
	 MSGNO(MsgMadraIDontThinkMayorEver)},
	{TRIGGER_TALK, ACTOR_YOUNG_WOMAN_ON_THE_DISASTERS, CONDITION_ALWAYS,
	 MSGNO(MsgMadraWeGoToAll)},
	{TRIGGER_TALK, ACTOR_OLD_MAN_ON_THE_WAVE, CONDITION_ALWAYS,
	 MSGNO(MsgMadraIHeardBoatMayorGot)},
	{TRIGGER_TALK, ACTOR_BOAT_OWNER, FLAG_MADRA_TOLD_OF_JENNA,
	 EVENT(scMadraAskAboutJenna)},
	{TRIGGER_TALK, ACTOR_BOAT_OWNER, CONDITION_ALWAYS,
	 MSGNO(MsgMadraThoseTravelersLookedWorriedAbout)},
	{TRIGGER_TALK, ACTOR_OLD_MAN_ON_THE_CAPTURED_CHAMPA, CONDITION_ALWAYS,
	 MSGNO(MsgMadraIWonderIfLegends)},
	{TRIGGER_TALK, ACTOR_OLD_WOMAN_ON_AIRS_ROCK, CONDITION_ALWAYS,
	 MSGNO(MsgIDonTKnowWhatWill)},
	{TRIGGER_TALK, ACTOR_BRIGGS_FAN, CONDITION_ALWAYS,
	 MSGNO(MsgMadraGroupOfSharpLookingTravelers)},
	{TRIGGER_TALK, ACTOR_GIRL_ON_THE_SUPERCONTINENT, CONDITION_ALWAYS,
	 MSGNO(MsgMadraIfYouHaveBigEnough)},
	{TRIGGER_TALK, ACTOR_OLD_MAN_WHO_SCARED_THE_CHAMPA, CONDITION_ALWAYS,
	 MSGNO(MsgMadraIWasAtMayorsHouse)},
	{TRIGGER_TALK, ACTOR_OLD_MAN_ON_THE_CHAMPAS_HOLE, CONDITION_ALWAYS,
	 MSGNO(MsgMadraItsNotThatIDont)},
	{TRIGGER_TALK, ACTOR_MAN_ON_THE_CHAMPAS_PLAN, CONDITION_ALWAYS,
	 MSGNO(MsgMadraMayorAskedMeToGo)},
	{TRIGGER_TALK, ACTOR_GIRL_ON_THE_BIG_HOLE, CONDITION_ALWAYS,
	 MSGNO(MsgMadraItSeemsNoOne)},
	{TRIGGER_TALK, ACTOR_YOUNG_MAN_AGAINST_THE_PRISONER, CONDITION_ALWAYS,
	 MSGNO(MsgSayThereWas)},
	{TRIGGER_TALK, ACTOR_YOUNG_WOMAN_ON_THE_PRISONER, CONDITION_ALWAYS,
	 MSGNO(MsgMadraIThoughtShinAndOthers)},
	{TRIGGER_TALK, ACTOR_GUARD_ON_HIS_FRIEND, CONDITION_ALWAYS,
	 MSGNO(MsgWhenAndHis)},
	{TRIGGER_TALK, ACTOR_GUARD_ON_HIS_PARTNER, CONDITION_ALWAYS,
	 MSGNO(MsgMadraUhhhnnnUhhhnnn)},
	{TRIGGER_TALK, ACTOR_GATE_GUARD, CONDITION_ALWAYS,
	 MSGNO(MsgMadraMayorFinallyReturnedFromAlhafra)},
	{TRIGGER_TALK, ACTOR_SECOND_GATE_GUARD, CONDITION_ALWAYS,
	 MSGNO(MsgMadraHeyItsYouGuys)},
	{TRIGGER_TALK, ACTOR_HOLE_WORKER, CONDITION_ALWAYS,
	 MSGNO(MsgMadraYeahWellWereWorkin)},
	{TRIGGER_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS,
	 EVENT(scMadraTalkToWeaponMerchant)},
	{TRIGGER_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS,
	 EVENT(scMadraTalkToArmorMerchant)},
	{TRIGGER_TALK, ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS,
	 EVENT(scMadraTalkToItemMerchant)},
	{TRIGGER_TALK, ACTOR_KARST, CONDITION_ALWAYS,
	 EVENT(scMadraKarstAsksAboutMenardi)},
	{TRIGGER_TALK, ACTOR_MAN_AFRAID_OF_THE_KIBOMBO, CONDITION_ALWAYS,
	 EVENT(scMadraAskAfraidOfKibombo)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_ON_THE_ELDER,
	 CONDITION_ALWAYS, MSGNO(MsgMadraWhyWouldAnyoneBeHappy)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_WOMAN_ON_THE_DISASTERS,
	 CONDITION_ALWAYS, MSGNO(MsgMadraTheyveBeenHavingSoMany)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_THE_WAVE,
	 CONDITION_ALWAYS, MSGNO(MsgMadraEveryoneGetsUrgeToExplore)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOAT_OWNER, CONDITION_ALWAYS,
	 MSGNO(MsgMadraIWonderWhyThoseTravelers)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_THE_CAPTURED_CHAMPA,
	 CONDITION_ALWAYS, MSGNO(MsgMadraWhatWasThatIslandAgain)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_AIRS_ROCK,
	 CONDITION_ALWAYS, MSGNO(MsgMadraPeopleOfMadraKnowOur)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BRIGGS_FAN, CONDITION_ALWAYS,
	 MSGNO(MsgMadraThatIslandThatGroup)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_ON_THE_SUPERCONTINENT,
	 CONDITION_ALWAYS, MSGNO(MsgMadraHowBigWouldShip)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_WHO_SCARED_THE_CHAMPA,
	 CONDITION_ALWAYS, MSGNO(MsgMadraTheseNewBoatsApparentlyUse)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_THE_CHAMPAS_HOLE,
	 CONDITION_ALWAYS, MSGNO(MsgMadraIfBuildingHasBrokenColumns)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_CHAMPAS_PLAN,
	 CONDITION_ALWAYS, MSGNO(MsgMadraSailorsAreSupposedToUse)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_ON_THE_BIG_HOLE,
	 CONDITION_ALWAYS, MSGNO(MsgMadraOurMayorsDoneSoLittle)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_AGAINST_THE_PRISONER,
	 CONDITION_ALWAYS, MSGNO(MsgMadraIsTurningWaterToIce)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_WOMAN_ON_THE_PRISONER,
	 CONDITION_ALWAYS, MSGNO(MsgMadraIsAlways)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GUARD_ON_HIS_FRIEND,
	 CONDITION_ALWAYS, MSGNO(MsgSoThisFellow)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GUARD_ON_HIS_PARTNER,
	 CONDITION_ALWAYS, MSGNO(MsgMadraIKnewIShouldHave)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GATE_GUARD, CONDITION_ALWAYS,
	 MSGNO(MsgMadraHeWasGoneSoLong)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_GATE_GUARD,
	 CONDITION_ALWAYS, MSGNO(MsgMadraHowMuchLongerDo)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT,
	 CONDITION_ALWAYS, MSGNO(MsgMadraPiratesAndKibomboMustHave)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT,
	 CONDITION_ALWAYS, MSGNO(MsgMadraThatNightChampaCame)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANT,
	 CONDITION_ALWAYS, MSGNO(MsgMadraShinSeemsTerrifiedOfPiers)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_KARST, CONDITION_ALWAYS,
	 EVENT(scMadraMindReadKarst)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_AFRAID_OF_THE_KIBOMBO,
	 CONDITION_ALWAYS, MSGNO(MsgMadraWereStillRecoveringFromKibombo)},
	{TRIGGER_TABLE_END},
};
