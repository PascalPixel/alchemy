/* Lemuria's dock, where the Lemurian ship ties up: Kraden steps off to wait by
   the ship and asks whether the party is ready to sail, and a door on the deck
   leads down into the ship's cabins. */

#include "field/field_state.h"
#include "field/field.h"
#include "game/game.h"
#include "scene/scene.h"
#include "field/script_inline.h"
#include "edition.h"

/* The dock's actors, by the rows of ActorPlacements. */
enum {
	ACTOR_KRADEN = ACTOR_FIRST_PLACED,
	/* Loho's cannon, on the ship's deck once the ship carries it (placed only
	   with LEMURIA_DOCK_CANNON_ON_DECK). */
	ACTOR_CANNON
};

/* The gangplank's metatile, (27, 28), between the pier and the ship's deck. */
enum { METATILE_BEHAVIOR_GANGPLANK = 21 };

extern Uint8					   MsgAreWeSetting[];
extern Sint32					   LemuriaDockExits[];
extern Sint32					   LemuriaDockExitsToOtherWayIn[];
extern MAP_GRID_METATILE_ANIMATION LemuriaDockCabinDoor;

void scLemuriaDockEnterDoor(
	const MAP_GRID_METATILE_ANIMATION *door, Sint32 exit, Sint32 mode);
void scLemuriaDockCyclePalette(void);

extern Uint16 LemuriaDockCabinDoorOpening[];

const SCENE_ENTRANCE *scGetEntrances(void)
{
	return SceneEntrances;
}

const SCENE_REGION *scGetExitRegions(void)
{
	return 0;
}

/* The dock's ways lead back the way the party came into Lemuria, which the
   harbour and the shore of Ancient Lemuria note in
   FLAG_LEMURIA_OTHER_WAY_IN. */
Sint32 *scGetExits(void)
{
	if(!fdScriptGetFlag(FLAG_LEMURIA_OTHER_WAY_IN))
		return LemuriaDockExits;
	return LemuriaDockExitsToOtherWayIn;
}

const ACTOR_PLACE *scGetActorPlacements(void)
{
	return ActorPlacements;
}

/* Felix steps onto the gangplank. Coming up the pier from the shore of Ancient
   Lemuria, where Kraden is not already on the dock he steps out from the leader
   and walks to his place by the ship; where he is, he walks back into the
   leader. */
void scLemuriaDockMoveKraden(void)
{
	MAIN_ROUTINE_STATE *party = &MainRoutineStateWork;

	if(party->entrance == LEMURIA_DOCK_FROM_ANCIENT_LEMURIA_EAST_A) {
		fdScriptStart();
		fdScriptEndLastingPsynergy(0);
		if(fdActorGetById(ACTOR_KRADEN)->x.fixed == 0) {
			fdActorWalkByAndWait(ACTOR_FELIX, 0, -16);
			scFieldActorLaunchFromLeader(
				ACTOR_KRADEN, 0, -16, FACING_SOUTH_SOUTHEAST);
			fdActorWaitForMoveById(ACTOR_KRADEN);
			fdActorSetSpeed(ACTOR_KRADEN, 1.2);
			fdActorWalkToAndWait(ACTOR_KRADEN, 472, 392);
			fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST);
		} else {
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
		fdScriptEnd();
	}
}

/* Kraden asks whether the party is ready to sail; yes sets sail. */
void scLemuriaDockAskSettingOff(void)
{
	MAIN_ROUTINE_STATE *party;
	Sint32				id;
	Sint32				limit;
	FIXED				z;

	if(FieldStateWork->psynergy != 0) {
		fdActorFaceActor(ACTOR_KRADEN, MainRoutineStateWork.leaderActorId, 0);
		fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	}
	fdScriptSetMessageId((Sint32)MsgAreWeSetting);
	/* "Felix, are we setting off now?" Yes: "Then, to sea we go!" No: "Please
	   don't rush on my account. Let's enjoy..." */
	fdScriptOpenMessage(ACTOR_KRADEN, 0);
	if(fdScriptAskYesNoWithActorAnimation(fdPartyGetLeaderActor(), 0) == 0) {
		fdScriptWait(15);
		/* "Then, to sea we go!" */
		fdScriptShowMessage(ACTOR_KRADEN, 0);
		fdScriptWait(5);
		fdActorJump(ACTOR_KRADEN, 4, 0);
		fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_EAST);
		fdScriptWait(5);
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
		fdScriptWait(10);

		MainRoutineStateWork.ownerOverride = 1;
		syImportScriptSetExitRequest(LEMURIA_DOCK_TO_LEMURIA_ENTRANCE_NORTH);
	} else {
		fdScriptWait(20);
		FieldStateWork->msg++;
		/* "Please don't rush on my account. Let's enjoy our stay." */
		fdScriptShowMessage(ACTOR_KRADEN, 0);
	}
}

/* The door opens and the leader walks in through it, lined up with it first
   (`mode` 0) or straight ahead (`mode` 1), leaving by `exit`. */
void scLemuriaDockEnterDoor(
	const MAP_GRID_METATILE_ANIMATION *door, Sint32 exit, Sint32 mode)
{
	Sint32 id;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdScriptPlaySound(SE_DOOR_OPEN);
	fdMapGridAnimateMetatiles(door->steps, door->x, door->y);
	fdActorGetById(MainRoutineStateWork.leaderActorId)->motionFlags =
		ACTOR_FALLS;
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.5);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_WALK);
	id = MainRoutineStateWork.leaderActorId;
	if(mode == 0)
		fdActorCenterAndWalk(id, 2, -8);
	else
		fdActorSetDestinationOffset(id, 0, -8);
	fdScriptWait(10);
	syImportScriptSetExitRequest(exit);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptEnd();
}

/* Felix walks north into the door on the ship's deck: it opens and he goes down
   into the cabins. */
void scLemuriaDockEnterCabins(Sint32 exit)
{
	scLemuriaDockEnterDoor(&LemuriaDockCabinDoor, exit, 0);
}

const CUE *scGetCues(void)
{
	return SceneCues;
}

/* A palette event: every eighth tick it turns both runs of
   LEMURIAN_SHIP_PALETTE's colours round by one, as the Lemurian Ship's deck
   turns its own: the blues of the band along the ship's hull, and the surf. */
void scLemuriaDockCyclePalette(void)
{
	if((TickCount & 7) == 0) {
		Sint16 saved;
		Sint32 i;

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

/* Sets up the dock: the ship's colours cycling, no area name, and in every
   edition but the Japanese Loho's cannon on deck once the ship carries it. */
Sint32 scInit(void)
{
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	fdActorGetById(MainRoutineStateWork.leaderActorId)->priorityFlags |=
		ACTOR_PRIORITY_IGNORES_RISES;
	sySetEvent(scLemuriaDockCyclePalette, EVENT_PASS_TICK);
	fdScriptClearFlag(FLAG_SHOW_AREA_NAME);
#if LEMURIA_DOCK_CANNON_ON_DECK
	if(fdScriptGetFlag(FLAG_SHIP_CARRIES_CANNON))
		fdActorSetSpritePriority(ACTOR_CANNON, 1);
	else
		fdActorSetPos(ACTOR_CANNON, 0, 0);
#endif
	return 0;
}

Sint32 scPrepareMap(void)
{
	/* Until the ship can fly, its wings are not out: plain hull and sea are
	   drawn where they would show. */
	if(!fdScriptGetFlag(FLAG_SHIP_FLIES)) {
		fdMapGridCopyMetatilesTo(57, 52, 64, 15, 1, 1);
		fdMapGridCopyMetatilesTo(60, 53, 67, 16, 6, 3);
		fdMapGridCopyMetatilesTo(50, 52, 16, 16, 6, 3);
		fdMapGridCopyMetatilesTo(50, 61, 16, 28, 10, 3);
		fdMapGridCopyMetatilesTo(61, 61, 16, 63, 10, 3);
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
Sint32 LemuriaDockExits[] = {
	SCENE_EXITS(LemuriaDock),
	SCENE_EXIT_UNLESS(LEMURIA_DOCK_TO_LEMURIA_ENTRANCE_NORTH, LemuriaEntrance, LEMURIA_ENTRANCE_FROM_LEMURIA_DOCK_NORTH, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(LEMURIA_DOCK_TO_ANCIENT_LEMURIA_EAST_A, AncientLemuria, ANCIENT_LEMURIA_FROM_LEMURIA_DOCK_EAST_A, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(LEMURIA_DOCK_TO_LEMURIAN_SHIP_CABINS, LemurianShipCabins, LEMURIAN_SHIP_CABINS_FROM_LEMURIAN_SHIP_MAIN_DECK, EXIT_FLAG_NONE),
	SCENE_EXITS_END,
};

/* The dock's exits once the party came by Lemuria's other way in. */
Sint32 LemuriaDockExitsToOtherWayIn[] = {
	SCENE_EXITS(LemuriaDock),
	SCENE_EXIT_UNLESS(LEMURIA_DOCK_TO_LEMURIA_ENTRANCE_NORTH, LemuriaEntrance, LEMURIA_ENTRANCE_FROM_LEMURIA_DOCK_SOUTH, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(LEMURIA_DOCK_TO_ANCIENT_LEMURIA_EAST_A, AncientLemuria, ANCIENT_LEMURIA_FROM_LEMURIA_DOCK_EAST_B, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(LEMURIA_DOCK_TO_LEMURIAN_SHIP_CABINS, LemurianShipCabins, LEMURIAN_SHIP_CABINS_FROM_LEMURIAN_SHIP_MAIN_DECK, EXIT_FLAG_NONE),
	SCENE_EXITS_END,
};

ACTOR_PLACE ActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
#if LEMURIA_DOCK_CANNON_ON_DECK
	ACTOR_PLACEMENT(SPRITE_CANNON, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 384, 0, 392, FACING_EAST, TALK_FACING_KEEP, 0),
#endif
	{SCENE_TABLE_END},
};

/* The cabin door's opening, one tick of metatiles copied over the shut */
/* door. */
Uint16 LemuriaDockCabinDoorOpening[] = {
	MAPGRID_ANIMATION_STEP(51, 38, 1, 1, 6),
	MAPGRID_ANIMATION_STEPS_END,
};
/* clang-format on */

/* The cabin door's opening, at its place on the map grid. */
MAP_GRID_METATILE_ANIMATION LemuriaDockCabinDoor = {
	LemuriaDockCabinDoorOpening, 70, 22};

CUE SceneCues[] = {
	{CUE_EXIT, LEMURIA_DOCK_TO_ANCIENT_LEMURIA_EAST_A, CONDITION_ALWAYS,
		LEMURIA_DOCK_TO_ANCIENT_LEMURIA_EAST_A},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), LEMURIA_DOCK_TO_LEMURIAN_SHIP_CABINS,
		CONDITION_ALWAYS, SCRIPT_REF(scLemuriaDockEnterCabins)},
	{CUE_STEP, METATILE_BEHAVIOR_GANGPLANK, CONDITION_ALWAYS,
		SCRIPT_REF(scLemuriaDockMoveKraden)},
	{CUE_TALK, ACTOR_KRADEN, CONDITION_ALWAYS,
		SCRIPT_REF(scLemuriaDockAskSettingOff)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
		ACTOR_KRADEN | CUE_SUBJECT_OWN_EFFECT, CONDITION_ALWAYS,
		SCRIPT_REF(scLemuriaDockAskSettingOff)},
	{CUE_TABLE_END},
};
