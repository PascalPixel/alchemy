/* @file */
/* Lemuria's dock, where the Lemurian ship ties up: Kraden steps off to wait by
 */
/* the ship and asks whether the party is ready to sail, and a door on the */
/* deck leads down into the ship's cabins. */

#include "field/field_state.h"
#include "field/field.h"
#include "game/game.h"
#include "scene/scene.h"
#include "field/event_inline.h"
#include "version.h"

/* The dock's actors, by the rows of ActorPlacements. */
enum {
	ACTOR_KRADEN = ACTOR_FIRST_PLACED,
	/* Loho's cannon, on the ship's deck once the ship carries it (placed only
	 */
	/* under TLA_PATCH_1). */
	ACTOR_CANNON
};

/* The gangplank's cell, (27, 28), between the pier and the ship's deck. */
enum { CELL_BEHAVIOR_GANGPLANK = 21 };

extern u8 MsgAreWeSetting[];
extern s32 LemuriaDock_Exits[];
extern s32 LemuriaDock_ExitsToOtherWayIn[];
extern MAP_GRID_CELL_ANIMATION LemuriaDock_CabinDoor;

void scLemuriaDockEnterDoor(const MAP_GRID_CELL_ANIMATION *door, s32 exit,
							s32 mode);
void scLemuriaDockCyclePalette(void);

extern u16 LemuriaDock_CabinDoorOpening[];

const SCENE_ENTRANCE *scGetEntrances(void) { return SceneEntrances; }

const SCENE_REGION *scGetExitRegions(void) { return 0; }

/* The dock's ways lead back the way the party came into Lemuria, which the */
/* harbour and the shore of Ancient Lemuria note in FLAG_LEMURIA_OTHER_WAY_IN.
 */
s32 *scGetExits(void) {
	if(!fdEventGetFlag(FLAG_LEMURIA_OTHER_WAY_IN))
		return LemuriaDock_Exits;
	return LemuriaDock_ExitsToOtherWayIn;
}

const ACTOR_PLACE *scGetActorPlacements(void) { return ActorPlacements; }

/* Felix steps onto the gangplank. Coming up the pier from the shore of */
/* Ancient Lemuria, where Kraden is not already on the dock he steps out from */
/* the leader and walks to his place by the ship; where he is, he walks back */
/* into the leader. */
void scLemuriaDockMoveKraden(void) {
	MAIN_ROUTINE_STATE *party = &MainRoutineStateWork;

	if(party->entrance == LEMURIA_DOCK_FROM_ANCIENT_LEMURIA_EAST_A) {
		fdEventStart();
		fdEventEndLastingPsynergy(0);
		if(fdActorGetById(ACTOR_KRADEN)->x.fixed == 0) {
			fdActorWalkByAndWait(ACTOR_FELIX, 0, -16);
			scFieldActorLaunchFromLeader(ACTOR_KRADEN, 0, -16,
										 FACING_SOUTH_SOUTHEAST);
			fdActorWaitForMoveById(ACTOR_KRADEN);
			fdActorSetSpeed(ACTOR_KRADEN, 1.2);
			fdActorWalkToAndWait(ACTOR_KRADEN, 472, 392);
			fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST);
		}
		else {
			ACTOR *leader;

			fdActorWalkByAndWait(ACTOR_FELIX, 0, 16);
			fdActorSetSpeed(ACTOR_KRADEN, 2);
			fdActorWalkToAndWait(ACTOR_KRADEN, 440, 440);
			fdActorSetSpeed(ACTOR_KRADEN, 1.2);
			fdActorSetAnimation(ACTOR_KRADEN, ANIM_WALK);
			leader = fdActorGetById(party->leaderActorId);
			if(leader != NULL)
				fdActorSetDestination(ACTOR_KRADEN, leader->x.part[0].integer,
									  leader->z.part[0].integer);
			fdActorWaitForMoveById(ACTOR_KRADEN);
			fdActorSetPos(ACTOR_KRADEN, 0, 0);
		}
		fdEventEnd();
	}
}

/* Kraden asks whether the party is ready to sail; yes sets sail. */
void scLemuriaDockAskSettingOff(void) {
	MAIN_ROUTINE_STATE *party;
	s32 id;
	s32 limit;
	FIXED z;

	if(FieldStateWork->psynergy != 0) {
		fdActorFaceActor(ACTOR_KRADEN, MainRoutineStateWork.leaderActorId, 0);
		fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	}
	fdEventSetMessageId((s32)MsgAreWeSetting);
	/* "Felix, are we setting off now?" Yes: "Then, to sea we go!" No: "Please
	 */
	/* don't rush on my account. Let's enjoy..." */
	fdEventOpenMessage(ACTOR_KRADEN, 0);
	if(fdEventAskYesNoWithActorAnimation(fdPartyGetLeaderActor(), 0) == 0) {
		fdEventWait(15);
		/* "Then, to sea we go!" */
		fdEventShowMessage(ACTOR_KRADEN, 0);
		fdEventWait(5);
		fdActorJump(ACTOR_KRADEN, 4, 0);
		fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_EAST);
		fdEventWait(5);
		party = &MainRoutineStateWork;
		z = fdActorGetById(party->leaderActorId)->z.fixed;
		limit = toFIXED(392);
		id = party->leaderActorId;
		if(z < limit)
			fdActorWalkToAndWait(id, 472, 376);
		else
			fdActorWalkToAndWait(id, 472, 408);
		id = ACTOR_FELIX;
		fdActorFaceDirectionAndWait(id, FACING_EAST);
		fdEventWait(10);

		MainRoutineStateWork.ownerOverride = 1;
		syImportEventSetExitRequest(LEMURIA_DOCK_TO_LEMURIA_ENTRANCE_NORTH);
	}
	else {
		fdEventWait(20);
		FieldStateWork->msg++;
		/* "Please don't rush on my account. Let's enjoy our stay." */
		fdEventShowMessage(ACTOR_KRADEN, 0);
	}
}

/* The door opens and the leader walks in through it, lined up with it first */
/* (`mode` 0) or straight ahead (`mode` 1), leaving by `exit`. */
void scLemuriaDockEnterDoor(const MAP_GRID_CELL_ANIMATION *door, s32 exit,
							s32 mode) {
	s32 id;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdEventPlaySnd(SE_DOOR_OPEN);
	fdMapGridAnimateCells(door->steps, door->x, door->y);
	fdActorGetById(MainRoutineStateWork.leaderActorId)->motionFlg = ACTOR_FALLS;
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.5);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_WALK);
	id = MainRoutineStateWork.leaderActorId;
	if(mode == 0)
		fdActorCenterAndWalk(id, 2, -8);
	else
		fdActorSetDestinationOffset(id, 0, -8);
	fdEventWait(10);
	syImportEventSetExitRequest(exit);
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	fdEventEnd();
}

/* Felix walks north into the door on the ship's deck: it opens and he goes */
/* down into the cabins. */
void scLemuriaDockEnterCabins(s32 exit) {
	scLemuriaDockEnterDoor(&LemuriaDock_CabinDoor, exit, 0);
}

const TRIGGER *scGetTriggers(void) { return SceneEvents; }

/* A palette task: every eighth frame it turns both runs of */
/* LEMURIAN_SHIP_PALETTE's colours round by one, as the Lemurian Ship's deck */
/* turns its own: the blues of the band along the ship's hull, and the surf. */
void scLemuriaDockCyclePalette(void) {
	if((FrameCount & 7) == 0) {
		s16 saved;
		s32 i;

		saved = BG_PLTT_COLOR(LEMURIAN_SHIP_PALETTE, HULL_BAND_LAST_COLOR);
		for(i = HULL_BAND_LAST_COLOR; i != HULL_BAND_FIRST_COLOR; i--)
			BG_PLTT_COLOR(LEMURIAN_SHIP_PALETTE, i) =
				BG_PLTT_COLOR(LEMURIAN_SHIP_PALETTE, i - 1);
		BG_PLTT_COLOR(LEMURIAN_SHIP_PALETTE, HULL_BAND_FIRST_COLOR) = saved;
		saved = BG_PLTT_COLOR(LEMURIAN_SHIP_PALETTE, SURF_LAST_COLOR);
		for(i = SURF_LAST_COLOR; i != SURF_FIRST_COLOR; i--)
			BG_PLTT_COLOR(LEMURIAN_SHIP_PALETTE, i) =
				BG_PLTT_COLOR(LEMURIAN_SHIP_PALETTE, i - 1);
		BG_PLTT_COLOR(LEMURIAN_SHIP_PALETTE, SURF_FIRST_COLOR) = saved;
	}
}

/* Sets up the dock: the ship's colours cycling, no area name, and from patch 1
 */
/* Loho's cannon on deck once the ship carries it. */
s32 scInit(void) {
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	fdActorGetById(MainRoutineStateWork.leaderActorId)->priFlg |=
		ACTOR_PRIORITY_IGNORES_RISES;
	sySetEvent(scLemuriaDockCyclePalette, TASK_PASS_FRAME);
	fdEventClearFlag(FLAG_SHOW_AREA_NAME);
#if TLA_PATCH_1 /* From patch 1. */
	if(fdEventGetFlag(FLAG_SHIP_CARRIES_CANNON))
		fdActorSetSprPriority(ACTOR_CANNON, 1);
	else
		fdActorSetPos(ACTOR_CANNON, 0, 0);
#endif
	return 0;
}

s32 scPrepareMap(void) {
	/* Until the ship can fly, its wings are not out: plain hull and sea are */
	/* drawn where they would show. */
	if(!fdEventGetFlag(FLAG_SHIP_FLIES)) {
		fdMapGridCopyCellsTo(57, 52, 64, 15, 1, 1);
		fdMapGridCopyCellsTo(60, 53, 67, 16, 6, 3);
		fdMapGridCopyCellsTo(50, 52, 16, 16, 6, 3);
		fdMapGridCopyCellsTo(50, 61, 16, 28, 10, 3);
		fdMapGridCopyCellsTo(61, 61, 16, 63, 10, 3);
	}
	return 0;
}

SCENE_ENTRANCE SceneEntrances[] = {
	{0, CONDITION_ALWAYS, 440, 0, 520, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

/* clang-format off */
/* The dock's exits, when the party came in by the harbour's north quay and */
/* the shore's first way. */
s32 LemuriaDock_Exits[] = {
    SCENE_EXITS(LemuriaDock),
    SCENE_EXIT_UNLESS(LEMURIA_DOCK_TO_LEMURIA_ENTRANCE_NORTH, LemuriaEntrance, LEMURIA_ENTRANCE_FROM_LEMURIA_DOCK_NORTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(LEMURIA_DOCK_TO_ANCIENT_LEMURIA_EAST_A, AncientLemuria, ANCIENT_LEMURIA_FROM_LEMURIA_DOCK_EAST_A, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(LEMURIA_DOCK_TO_LEMURIAN_SHIP_CABINS, LemurianShipCabins, LEMURIAN_SHIP_CABINS_FROM_LEMURIAN_SHIP_MAIN_DECK, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

/* The dock's exits once the party came by Lemuria's other way in. */
s32 LemuriaDock_ExitsToOtherWayIn[] = {
    SCENE_EXITS(LemuriaDock),
    SCENE_EXIT_UNLESS(LEMURIA_DOCK_TO_LEMURIA_ENTRANCE_NORTH, LemuriaEntrance, LEMURIA_ENTRANCE_FROM_LEMURIA_DOCK_SOUTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(LEMURIA_DOCK_TO_ANCIENT_LEMURIA_EAST_A, AncientLemuria, ANCIENT_LEMURIA_FROM_LEMURIA_DOCK_EAST_B, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(LEMURIA_DOCK_TO_LEMURIAN_SHIP_CABINS, LemurianShipCabins, LEMURIAN_SHIP_CABINS_FROM_LEMURIAN_SHIP_MAIN_DECK, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

ACTOR_PLACE ActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
#if TLA_PATCH_1
    ACTOR_PLACEMENT(SPRITE_CANNON, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 384, 0, 392, FACING_EAST, TALK_FACING_KEEP, 0),
#endif
    {SCENE_TABLE_END},
};

/* The cabin door's opening, one frame of cells copied over the shut door. */
u16 LemuriaDock_CabinDoorOpening[] = {
    MAPGRID_ANIMATION_STEP(51, 38, 1, 1, 6),
    MAPGRID_ANIMATION_STEPS_END,
};
/* clang-format on */

/* The cabin door's opening, at its place on the map grid. */
MAP_GRID_CELL_ANIMATION LemuriaDock_CabinDoor = {LemuriaDock_CabinDoorOpening,
												 70, 22};

TRIGGER SceneEvents[] = {
	{TRIGGER_EXIT, LEMURIA_DOCK_TO_ANCIENT_LEMURIA_EAST_A, CONDITION_ALWAYS,
	 LEMURIA_DOCK_TO_ANCIENT_LEMURIA_EAST_A},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
	 LEMURIA_DOCK_TO_LEMURIAN_SHIP_CABINS, CONDITION_ALWAYS,
	 EVENT(scLemuriaDockEnterCabins)},
	{TRIGGER_STEP, CELL_BEHAVIOR_GANGPLANK, CONDITION_ALWAYS,
	 EVENT(scLemuriaDockMoveKraden)},
	{TRIGGER_TALK, ACTOR_KRADEN, CONDITION_ALWAYS,
	 EVENT(scLemuriaDockAskSettingOff)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
	 ACTOR_KRADEN | TRIGGER_SUBJECT_OWN_EFFECT, CONDITION_ALWAYS,
	 EVENT(scLemuriaDockAskSettingOff)},
	{TRIGGER_TABLE_END},
};
