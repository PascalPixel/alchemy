/* @file */
/* The Madra Drawbridge, east of Madra, where two guards keep the way into */
/* Osenia shut until Madra's elder gives the party permission. */
/* */
/* The first time the party steps onto the bridge */
/* (scMadraDrawbridgeBlockBridge) the guards close ranks across it and send the
 */
/* party back for the elder's permission. Once FLAG_OSENIA_PASSAGE_GRANTED is */
/* set they greet the party instead (scMadraDrawbridgeLetPermittedPartyThrough)
 */
/* and step aside. */

#include "field/field.h"
#include "game/game.h"
#include "field/event_inline.h"
#include "field/field_sequence_event.h"
#include "text/msg_ids.h"

/* The guards have let the permitted party over the bridge. */
#define FLAG_DRAWBRIDGE_GUARDS_LET_PARTY_THROUGH 0x916

/* Set once the guards have closed ranks on this visit. */
#define FLAG_GUARDS_BLOCKED_BRIDGE FLAG_SCENE_TEMP_00
/* Set once the guards have told the party to fetch the elder's permission. */
#define FLAG_GUARDS_ASKED_FOR_PERMISSION FLAG_AREA_TEMP_01

/* The cells at the foot of the bridge, whose step trigger brings the guards */
/* across it. */
enum { CELL_BEHAVIOR_MADRA_DRAWBRIDGE_FOOT = 10 };

/* The two guards, numbering the rows of ActorPlacements. */
typedef enum MADRA_DRAWBRIDGE_ACTOR_ID {
	/* The guard on the east side of the bridge. */
	ACTOR_EAST_GUARD = ACTOR_FIRST_PLACED,
	/* The guard on the west side of the bridge. */
	ACTOR_WEST_GUARD
} MADRA_DRAWBRIDGE_ACTOR_ID;

const SCENE_ENTRANCE *scGetEntrances(void) { return SceneEntrances; }

const SCENE_REGION *scGetExitRegions(void) { return 0; }

const u32 *scGetExits(void) { return SceneExits; }

const ACTOR_PLACE *scGetActorPlacements(void) { return ActorPlacements; }

/* The east guard asks whether the party is bound for Osenia, and without the */
/* elder's permission sends it back to town for a permit. */
void scMadraDrawbridgeAskGoingToOsenia(void) {
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdEventSetMessageId((s32)MsgMadraGoingToOsenia);
	/* "You goin' to Osenia?" Yes: "You'll have to head back to town and get..."
	 */
	/* No: "Then get lost, pal. These parts can be dangerous..." */
	fdEventOpenMessage(ACTOR_EAST_GUARD, 0);
	if(fdEventAskYesNoWithActorAnimation(MainRoutineStateWork.leaderActorId,
										 0) == 0) {
		fdActorSetAnimationAndWait(ACTOR_EAST_GUARD, ANIM_NOD);
		/* "You'll have to head back to town and get a permit! Just go to
		 * the..." */
		fdEventShowMessageThenWait(ACTOR_EAST_GUARD, 0, 2);
	}
	else {
		(FieldStateWork->msg)++;
		/* "Then get lost, pal. These parts can be dangerous..." */
		fdEventShowMessageThenWait(ACTOR_EAST_GUARD, 0, 2);
	}
	fdEventEnd();
}

const TRIGGER *scGetTriggers(void) { return SceneEvents; }

/* The guards close ranks across the bridge as the party steps onto it, once a
 */
/* visit, and the first time they ask for the elder's permission. */
void scMadraDrawbridgeBlockBridge(void) {
	if(fdEventGetFlag(FLAG_GUARDS_BLOCKED_BRIDGE) == 0) {
		fdEventStart();
		fdEventEndLastingPsynergy(0);
		if(fdEventGetFlag(FLAG_GUARDS_ASKED_FOR_PERMISSION) == 0) {
			fdCameraSetSpeed(1.5);
			fdCameraMoveTo(164, CAMERA_KEEP, 260, 1);
			fdActorWalkTo(MainRoutineStateWork.leaderActorId, 158, 316);
		}
		fdActorGetById(ACTOR_EAST_GUARD)->steeringFlg &= ~ACTOR_FACES_MOTION;
		fdActorGetById(ACTOR_WEST_GUARD)->steeringFlg &= ~ACTOR_FACES_MOTION;
		fdActorWalkTo(ACTOR_WEST_GUARD, 148, 252);
		fdActorWalkToAndWait(ACTOR_EAST_GUARD, 170, 252);
		fdActorWaitForMoveById(ACTOR_WEST_GUARD);
		fdActorGetById(ACTOR_EAST_GUARD)->steeringFlg |= ACTOR_FACES_MOTION;
		fdActorGetById(ACTOR_WEST_GUARD)->steeringFlg |= ACTOR_FACES_MOTION;
		fdActorFaceDirection(ACTOR_EAST_GUARD, FACING_SOUTH, 0);
		fdActorFaceDirection(ACTOR_WEST_GUARD, FACING_SOUTH, 0);
		fdActorWaitForMoveById(MainRoutineStateWork.leaderActorId);
		fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_NORTH,
							 0);
		fdEventSetFlag(FLAG_GUARDS_BLOCKED_BRIDGE);
		fdCameraWaitForArrival();
		/* The guards now stand across the bridge: no one walks past them. */
		fdMapGridCopyCellAttributes(11, 9, 4, 1, 8, 14);
		if(fdEventGetFlag(FLAG_GUARDS_ASKED_FOR_PERMISSION) == 0) {
			fdEventSetMessageId((s32)MsgMadraNeedEldersPermission);
			fdEventWait(10);
			fdActorRunRepeatedMotion(ACTOR_EAST_GUARD, 1);
			/* "If you wanna cross here, you'll need permission from the town
			 * elder!" */
			fdEventShowMessageThenWait(ACTOR_EAST_GUARD, 0, 5);
			fdActorSetAnimationAndWait(ACTOR_WEST_GUARD, ANIM_NOD);
			/* "Without permission, ain't no one's gonna cross this bridge,
			 * no..." */
			fdEventShowMessage(ACTOR_WEST_GUARD, 0);
			fdEventSetFlag(FLAG_GUARDS_ASKED_FOR_PERMISSION);
			fdEventEnd();
		}
	}
}

/* With the elder's permission, the guards greet the party and let it through.
 */
void scMadraDrawbridgeLetPermittedPartyThrough(void) {
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdEventSetMessageId((s32)MsgMadraHeardAboutYou);
	fdCameraMoveTo(164, CAMERA_KEEP, 260, 1);
	fdCameraWaitForArrival();
	fdActorSetAnimationAndWait(ACTOR_EAST_GUARD, ANIM_NOD);
	/* "Yeah, we heard about you already. If you're going to Osenia, you'd..."
	 */
	fdEventShowMessage(ACTOR_EAST_GUARD, 0);
	fdActorSetAnimationAndWait(ACTOR_WEST_GUARD, ANIM_NOD);
	/* "Alhafra is up along the northeast of Osenia... You'll see it
	 * eventually." */
	fdEventShowMessage(ACTOR_WEST_GUARD, 0);
	fdEventSetFlag(FLAG_DRAWBRIDGE_GUARDS_LET_PARTY_THROUGH);
	fdEventEnd();
}

s32 scInit(void) {
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	return 0;
}

s32 scPrepareMap(void) { return 0; }

SCENE_ENTRANCE SceneEntrances[] = {
	{0, CONDITION_ALWAYS, 152, 0, 176, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

u32 SceneExits[] = {
	SCENE_EXITS(MadraDrawbridge),
	SCENE_EXIT_UNLESS(1, WorldMap, 10, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(2, WorldMap, 11, EXIT_FLAG_NONE),
	SCENE_EXITS_END,
};

ACTOR_PLACE ActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_BALD_MAN_IN_ORANGE_VEST, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 188, 0, 260, FACING_SOUTH,
					TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MADRAN_SOLDIER, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 132, 0, 260, FACING_SOUTH,
					TALK_FACING_PARTY_AND_BACK, 0),
	{SCENE_TABLE_END},
};

TRIGGER SceneEvents[] = {
	{TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
	{TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
	{TRIGGER_STEP, CELL_BEHAVIOR_MADRA_DRAWBRIDGE_FOOT,
	 FLAG_OSENIA_PASSAGE_GRANTED, EVENT(scMadraDrawbridgeBlockBridge)},
	{TRIGGER_STEP, CELL_BEHAVIOR_MADRA_DRAWBRIDGE_FOOT,
	 FLAG_DRAWBRIDGE_GUARDS_LET_PARTY_THROUGH,
	 EVENT(scMadraDrawbridgeLetPermittedPartyThrough)},
	{TRIGGER_TALK, ACTOR_EAST_GUARD, FLAG_OSENIA_PASSAGE_GRANTED,
	 EVENT(scMadraDrawbridgeAskGoingToOsenia)},
	{TRIGGER_TALK, ACTOR_EAST_GUARD, CONDITION_ALWAYS,
	 MSGNO(MsgMadraMikasallaVillage)},
	{TRIGGER_TALK, ACTOR_WEST_GUARD, FLAG_OSENIA_PASSAGE_GRANTED,
	 MSGNO(MsgMadraCanCrossWithPermission)},
	{TRIGGER_TALK, ACTOR_WEST_GUARD, CONDITION_ALWAYS,
	 MSGNO(MsgMadraAvoidYampiDesert)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EAST_GUARD,
	 FLAG_OSENIA_PASSAGE_GRANTED, MSGNO(MsgMadraForTheirSafety)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EAST_GUARD, CONDITION_ALWAYS,
	 MSGNO(MsgMadraWouldNeedBoat)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEST_GUARD,
	 FLAG_OSENIA_PASSAGE_GRANTED, MSGNO(MsgMadraElderReturn)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEST_GUARD, CONDITION_ALWAYS,
	 MSGNO(MsgMadraRatherStayHome)},
	{TRIGGER_TABLE_END},
};
