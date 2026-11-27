/* @file */
/* Ancient Lemuria, the ruined shore before the hidden city, where the */
/* party lands after getting past Poseidon. */
/* */
/* From the boat dock at the south, the party climbs the shore. Move pushes */
/* a blue statue onto its spot by the path north, cell (8, 36), and the */
/* Psynergy brings out two Lemurians, who wonder whether this is the party */
/* that defeated Poseidon and, seeing Piers, hurry back into the city */
/* (scAncientLemuriaMeetTheLemurians). Cyclone clears six patches of weeds; */
/* under the first is Rime, the Mercury Djinni, which hops off and hides in */
/* another blue statue until Tremor shakes it out */
/* (scAncientLemuriaCycloneRevealsRime, scAncientLemuriaTremorShakesOutRime). */
/* Walking back to the dock before King Hydros is met, Kraden calls Felix */
/* back to keep exploring. */

#include "scene/event_layer.h"
#include "field/field_event.h"
#include "field/event_inline.h"
#include "game/game.h"
#include "field/field_sequence.h"
#include "scene/scene.h"

void scAncientLemuriaStillTheStatue(s32 id);
void scAncientLemuriaMeetTheLemurians(void);

#include "text/msg_ids.h"
/* The shore's water: its two background palettes, from the first, the colours
 */
/* of each that rotate every four frames, the colour of the first that shines */
/* in the colours of AncientLemuria_ShiningWaterColors in turn, and how many */
/* there are. */
#define FIRST_SHORE_WATER_PALETTE 7
#define SHORE_WATER_FIRST_COLOR	  1
#define SHORE_WATER_LAST_COLOR	  14
#define WATER_SHINING_COLOR		  15
#define SHINING_WATER_COLORS	  12

extern s16 AncientLemuria_ShiningWaterColors[SHINING_WATER_COLORS];

/* The frames the statue Rime hides in takes to rock from side to side and */
/* back, clicking as each rock starts. */
#define RIME_STATUE_ROCK_FRAMES 16

/* Rime's story: brought out of the first weeds by Cyclone, hiding in the blue
 */
/* statue, which rocks, until Tremor shakes it out. */
#define FLAG_RIME_REVEALED			0x96c
#define FLAG_RIME_OUT_OF_THE_STATUE 0x96d
#define FLAG_RIME_IN_THE_STATUE		0x96e

/* Set once Move has pushed the blue statue onto cell (8, 36), where the */
/* Lemurians see the Psynergy and come out. */
#define FLAG_BLUE_STATUE_MOVED 0x961

/* The cell behaviours the shore's trigger table answers: the way south to the
 */
/* boat dock, where Kraden may call Felix back; the rows across the dock where
 */
/* the view stops scrolling north and may again; the ledge's edge beside the */
/* blue statue's spot; and the six patches of weeds Cyclone blows away. */
enum {
	CELL_BEHAVIOR_WAY_TO_DOCK = 2,
	CELL_BEHAVIOR_KEEP_CAMERA_SOUTH = 20,
	CELL_BEHAVIOR_LET_CAMERA_NORTH,
	CELL_BEHAVIOR_LEDGE_BY_STATUE_SPOT,
	CELL_BEHAVIOR_RIMES_WEEDS = 30,
	CELL_BEHAVIOR_SECOND_WEEDS,
	CELL_BEHAVIOR_THIRD_WEEDS,
	CELL_BEHAVIOR_FOURTH_WEEDS,
	CELL_BEHAVIOR_FIFTH_WEEDS,
	CELL_BEHAVIOR_SIXTH_WEEDS
};

/* The shore's own way between the ruined gateway in its north-east and its */
/* south-east corner: the exits by where they lead, the entrances by where the
 */
/* party comes out. */
enum {
	ANCIENT_LEMURIA_TO_SOUTHEAST_CORNER = 1,
	ANCIENT_LEMURIA_TO_NORTHEAST_GATEWAY = 4
};
enum {
	ANCIENT_LEMURIA_AT_NORTHEAST_GATEWAY = 1,
	ANCIENT_LEMURIA_AT_SOUTHEAST_CORNER = 4
};

/* The shore's actors, numbering ActorPlacements' rows that are not the */
/* party's. */
typedef enum ANCIENT_LEMURIA_ACTOR_ID {
	/* The blue statue Move pushes along the shore. */
	ACTOR_BLUE_STATUE = ACTOR_FIRST_PLACED,
	/* The Lemurians who meet the party (scAncientLemuriaMeetTheLemurians). */
	/* @{ */
	ACTOR_LEMURIAN,
	ACTOR_SECOND_LEMURIAN,
	/* @} */
	ACTOR_KRADEN,
	ACTOR_RIME,
	/* The blue statue Rime hides in until Tremor shakes it out. */
	ACTOR_RIMES_STATUE
} ANCIENT_LEMURIA_ACTOR_ID;

/* Felix talks to Rime once Tremor has shaken it out of its statue, and it */
/* joins the party. */
void scAncientLemuriaRimeJoins(void) {
	fdDjinnJoinParty(ACTOR_RIME, DJINNI_RIME / DJINN_PER_ELEMENT,
					 DJINNI_INDEX(DJINNI_RIME));
}

/* The statue Rime hides in rocks from side to side, clicking as each rock */
/* starts. */
void scAncientLemuriaRockTheStatue(ACTOR *actor) {
	FIELD_SPR *sprite = actor->spr;
	s16 time = actor->tmr++;

	sprite->rotation = syMulFX(
		sySin(time * (TRIG_TURN / RIME_STATUE_ROCK_FRAMES)), TRIG_TURN / 256);
	if((time & (RIME_STATUE_ROCK_FRAMES - 1)) == 0)
		sySndPlay(SE_SWITCH);
}

/* The rocking statue stands still. */
void scAncientLemuriaStillTheStatue(s32 id) {
	ACTOR *actor = fdActorGetById(id);
	FIELD_SPR *sprite = actor->spr;

	actor->update = 0;
	actor->tmr = 0;
	sprite->rotation = 0;
}

const SCENE_ENTRANCE *scGetEntrances(void) { return SceneEntrances; }

const SCENE_REGION *scGetExitRegions(void) { return 0; }

const u32 *scGetExits(void) { return SceneExits; }

const ACTOR_PLACE *scGetActorPlacements(void) { return ActorPlacements; }

/* Felix walks north across the boat dock towards the gangway, over cells */
/* (8, 52) to (12, 52): the game stops the view scrolling north. Setting out */
/* from the dock's south edge, the view follows him only a step or so (some */
/* 18 pixels) and then stays put, and he climbs the screen until he steps */
/* onto the gangway; walking back south, the view comes back down with him. */
void scAncientLemuriaKeepCameraSouth(void) {
	((MAP_STATE *)Ram_HeapSlots->mapGrid)->minZ = toFIXED(750);
}

/* Nothing visible happens as Felix walks over the next row, cells (8, 53) to */
/* (12, 53); the view may scroll north over the whole shore again. */
void scAncientLemuriaLetCameraNorth(void) {
	((MAP_STATE *)Ram_HeapSlots->mapGrid)->minZ = toFIXED(352);
}

/* Nothing more than the move itself happens as Felix starts casting Move on */
/* the blue statue by the path north. */
void scAncientLemuriaStartMovingStatue(void) {}

/* Felix moves the blue statue by the path north with Move. Set down on its */
/* spot beside the ledge, cell (8, 36), it stays there, and the two Lemurians */
/* come out to meet the party (scAncientLemuriaMeetTheLemurians). */
void scAncientLemuriaFinishMovingStatue(s32 unused, s32 id) {
	ACTOR *actor = fdActorGetById(id);
	s32 x = FIELD_CELL(actor->x.fixed);
	s32 z = FIELD_CELL(actor->z.fixed);

	if(x == 8 && z == 36)
		syGameFlagSet(FLAG_BLUE_STATUE_MOVED);
	if(syGameFlagGet(FLAG_BLUE_STATUE_MOVED))
		scAncientLemuriaMeetTheLemurians();
}

/* One of Rime's hops, by (dx, dz). */
static inline void Hop(s32 dx, s32 dz) {
	fdActorJump(ACTOR_RIME, 5, 0);
	fdActorWalkByAndWaitById(ACTOR_RIME, dx, dz);
	fdEventWait(3);
}

/* Felix casts Cyclone on the first patch of weeds (cell (5, 13)): the weeds */
/* blow away and, the first time, Rime pops out of the ground, spinning as it */
/* grows, then hops off east with Felix turning to watch it and hides in a blue
 */
/* statue, which starts to rock. */
void scAncientLemuriaCycloneRevealsRime(void) {
	ACTOR *actor;
	s32 frame;

	syFieldTileCopiesCopyTouchedCells(7, 13, 1, 1, 5, 13, 0);
	if(fdEventGetFlag(FLAG_RIME_REVEALED))
		return;
	actor = fdActorGetById(ACTOR_RIME);
	fdEventSetFlag(FLAG_RIME_REVEALED);
	fdEventSetFlag(FLAG_RIME_IN_THE_STATUE);
	fdActorSetSprFlags(fdActorGetById(ACTOR_RIME), 0);
	fdActorSetPos(ACTOR_RIME, 88, 216);
	fdActorJump(ACTOR_RIME, 9, 0);
	actor->scaleY = FIXED_ONE / 2;
	actor->scaleX = FIXED_ONE / 2;
	for(frame = 0; frame < 30; frame++) {
		actor->facing += TRIG_TURN / 16;
		if(actor->scaleX < FIXED_ONE)
			actor->scaleX += FIXED_ONE * 5 / 128;
		actor->scaleY = actor->scaleX;
		sySynch(1);
	}
	actor->facing = FACING_SOUTH_SOUTHEAST;
	actor->scaleY = FIXED_ONE;
	actor->scaleX = FIXED_ONE;
	fdActorSetSprFlags(fdActorGetById(ACTOR_RIME), 1);
	fdActorSetSprPriority(ACTOR_RIME, 1);
	fdEventWait(8);
	fdActorJump(ACTOR_RIME, 9, 0);
	fdActorWalkToAndWaitById(ACTOR_RIME, 120, 200);
	fdActorGetById(ACTOR_RIME)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
	fdActorWalkToAndWaitById(ACTOR_RIME, 136, 200);
	fdEventWait(3);
	fdActorSetAnimationById(MainRoutineStateWork.leaderActorId, ANIM_STAND);
	fdActorFaceDirectionById(MainRoutineStateWork.leaderActorId,
							 FACING_NORTHEAST, 0);
	Hop(0, -16);
	Hop(16, 0);
	Hop(16, 0);
	Hop(0, -16);
	Hop(16, 0);
	Hop(0, -16);
	Hop(16, 0);
	Hop(16, 0);
	fdActorFaceDirectionById(MainRoutineStateWork.leaderActorId, FACING_EAST,
							 0);
	Hop(0, 16);
	Hop(16, 0);
	Hop(16, 0);
	Hop(16, 0);
	Hop(16, 0);
	fdActorSetPos(ACTOR_RIME, 0, 0);
	fdActorGetById(ACTOR_RIMES_STATUE)->update = scAncientLemuriaRockTheStatue;
	fdActorGetById(ACTOR_RIMES_STATUE)->tmr = 0;
}

/* Felix casts Cyclone on the second patch of weeds (cell (8, 9)), and */
/* they blow away. */
void scAncientLemuriaCycloneSecondWeeds(void) {
	syFieldTileCopiesCopyTouchedCells(9, 9, 2, 3, 8, 9, 0);
}

/* Felix casts Cyclone on the third patch of weeds (cell (12, 7)), and */
/* they blow away. */
void scAncientLemuriaCycloneThirdWeeds(void) {
	syFieldTileCopiesCopyTouchedCells(38, 0, 5, 2, 12, 7, 0);
}

/* Felix casts Cyclone on the fourth patch of weeds (cell (14, 11)), and they */
/* blow away. */
void scAncientLemuriaCycloneFourthWeeds(void) {
	syFieldTileCopiesCopyTouchedCells(39, 2, 4, 3, 14, 11, 0);
}

/* Felix casts Cyclone on the fifth patch of weeds (cell (21, 9)), and they */
/* blow away. */
void scAncientLemuriaCycloneFifthWeeds(void) {
	syFieldTileCopiesCopyTouchedCells(39, 2, 3, 3, 21, 9, 0);
}

/* Felix casts Cyclone on the sixth patch of weeds (cell (25, 8)), and they */
/* blow away. */
void scAncientLemuriaCycloneSixthWeeds(void) {
	syFieldTileCopiesCopyTouchedCells(39, 0, 4, 3, 25, 8, 0);
}

/* As Felix starts casting Tremor on the rocking statue Rime hides in, it */
/* stands still. */
void scAncientLemuriaStartTremorOnTheStatue(void) {
	scAncientLemuriaStillTheStatue(ACTOR_RIMES_STATUE);
}

/* Tremor on the shaking statue, once Rime hides in it: Rime is thrown out, */
/* spinning as it grows back to its full size. */
void scAncientLemuriaTremorShakesOutRime(void) {
	ACTOR *actor;
	s32 frame;

	if(fdEventGetFlag(FLAG_RIME_OUT_OF_THE_STATUE))
		return;
	if(!syGameFlagGet(FLAG_RIME_REVEALED))
		return;
	actor = fdActorGetById(ACTOR_RIME);
	syGameFlagSet(FLAG_RIME_OUT_OF_THE_STATUE);
	syGameFlagClear(FLAG_RIME_IN_THE_STATUE);
	scAncientLemuriaStillTheStatue(ACTOR_RIMES_STATUE);
	fdActorSetPos(ACTOR_RIME, 270, 184);
	actor->y.fixed = toFIXED(22);
	fdActorJump(ACTOR_RIME, 11, 0);
	actor->scaleY = FIXED_ONE / 2;
	actor->scaleX = FIXED_ONE / 2;
	fdActorSetSpeed(ACTOR_RIME, 2.2);
	fdActorWalkTo(ACTOR_RIME, 312, 184);
	for(frame = 0; frame < 24; frame++) {
		actor->facing += TRIG_TURN / 8;
		if(actor->scaleX < FIXED_ONE)
			actor->scaleX += FIXED_ONE * 5 / 128;
		actor->scaleY = actor->scaleX;
		sySynch(1);
	}
	actor->facing = FACING_SOUTH_SOUTHEAST;
	actor->scaleY = FIXED_ONE;
	actor->scaleX = FIXED_ONE;
	fdActorWaitForMoveById(ACTOR_RIME);
}

/* Felix walks south onto the boat dock: once the Lemurians have met the */
/* party and until King Hydros is met, Kraden calls him back to keep */
/* exploring; otherwise the party leaves Lemuria by boat. */
void scAncientLemuriaHeadForTheDock(void) {
	ACTOR *actor;

	if(!syGameFlagGet(FLAG_BLUE_STATUE_MOVED) ||
	   syGameFlagGet(FLAG_MET_KING_HYDROS)) {
		sySndPlay(SE_MAP_EXIT);
		syImportEventSetExitRequest(ANCIENT_LEMURIA_TO_LEMURIA_DOCK_EAST_A);
	}
	else {
		fdEventStart();
		fdEventEndLastingPsynergy(0);
		scFieldActorLaunchFromLeader(ACTOR_KRADEN, 0, -16,
									 FACING_SOUTH_SOUTHWEST);
		fdActorFaceDirectionById(ACTOR_FELIX, FACING_NORTH, 0);
		fdActorWaitForMoveById(ACTOR_KRADEN);
		fdEventWait(20);
		if(syGameFlagGet(FLAG_IN_PARTY(ACTOR_PIERS))) {
			fdEventSetMessageId(MsgLemuriaStayExploring);
			/* "Lost already, Felix? That's the way to the boat dock! You
			 * can't..." */
			fdEventShowMessageThenWait(ACTOR_KRADEN, 0, 10);
		}
		else {
			fdEventSetMessageId(MsgLemuriaStayExploringAlternate);
			/* "Lost already, Felix? That's the way to the boat dock! You
			 * can't..." */
			fdEventShowMessageThenWait(ACTOR_KRADEN, 0, 10);
		}
		fdActorSetSpeed(ACTOR_KRADEN, 1.2);
		fdActorSetAnimationById(ACTOR_KRADEN, ANIM_WALK);
		actor = fdActorGetById(MainRoutineStateWork.leaderActorId);
		if(actor != NULL)
			fdActorSetDestinationById(ACTOR_KRADEN, actor->x.part[0].integer,
									  actor->z.part[0].integer);
		fdActorWaitForMoveById(ACTOR_KRADEN);
		fdActorSetPos(ACTOR_KRADEN, 0, 0);
		fdActorWalkToAndWait(ACTOR_FELIX, 168, 832);
		fdEventEnd();
	}
}

const TRIGGER *scGetTriggers(void) { return SceneEvents; }

/* The water's shine: every four frames the rotating colours of both water */
/* palettes move on by one, and the shining colour takes the next of */
/* AncientLemuria_ShiningWaterColors. */
s32 scAncientLemuriaCycleWaterCols(void) {
	if((*Ram_FrameCount & 3) == 0) {
		s16 saved;
		s32 index;

		saved =
			BG_PLTT_COLOR(FIRST_SHORE_WATER_PALETTE, SHORE_WATER_LAST_COLOR);
		for(index = SHORE_WATER_LAST_COLOR; index != SHORE_WATER_FIRST_COLOR;
			index--)
			BG_PLTT_COLOR(FIRST_SHORE_WATER_PALETTE, index) =
				BG_PLTT_COLOR(FIRST_SHORE_WATER_PALETTE, index - 1);
		BG_PLTT_COLOR(FIRST_SHORE_WATER_PALETTE, SHORE_WATER_FIRST_COLOR) =
			saved;
		saved = BG_PLTT_COLOR(FIRST_SHORE_WATER_PALETTE + 1,
							  SHORE_WATER_LAST_COLOR);
		for(index = SHORE_WATER_LAST_COLOR; index != SHORE_WATER_FIRST_COLOR;
			index--)
			BG_PLTT_COLOR(FIRST_SHORE_WATER_PALETTE + 1, index) =
				BG_PLTT_COLOR(FIRST_SHORE_WATER_PALETTE + 1, index - 1);
		BG_PLTT_COLOR(FIRST_SHORE_WATER_PALETTE + 1, SHORE_WATER_FIRST_COLOR) =
			saved;
	}
	{
		s16 color = AncientLemuria_ShiningWaterColors[(*Ram_FrameCount >> 2) %
													  SHINING_WATER_COLORS];

		BG_PLTT_COLOR(FIRST_SHORE_WATER_PALETTE, WATER_SHINING_COLOR) = color;
		return color;
	}
}

/* Sets the shore up on entry: its water shines, the area's name shows only */
/* on coming out at the north-east gateway, the blue statue and the Lemurians */
/* are as the party left them, and Rime's statue rocks while Rime hides in it.
 */
s32 scInit(void) {
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
	sySetEvent(scAncientLemuriaCycleWaterCols, TASK_PASS_VBLANK);
	if(MainRoutineStateWork.entrance != ANCIENT_LEMURIA_AT_NORTHEAST_GATEWAY)
		syGameFlagClear(FLAG_SHOW_AREA_NAME);
	else
		syGameFlagSet(FLAG_SHOW_AREA_NAME);
	if(syGameFlagGet(FLAG_BLUE_STATUE_MOVED)) {
		fdActorSetPos(ACTOR_BLUE_STATUE, 136, 584);
		fdActorSetPos(ACTOR_LEMURIAN, 0, 0);
		fdActorSetPos(ACTOR_SECOND_LEMURIAN, 0, 0);
	}
	if(fdEventGetFlag(FLAG_RIME_OUT_OF_THE_STATUE))
		fdActorSetPos(ACTOR_RIME, 312, 184);
	if(MainRoutineStateWork.entrance == ANCIENT_LEMURIA_AT_NORTHEAST_GATEWAY &&
	   !syGameFlagGet(FLAG_RIME_OUT_OF_THE_STATUE) &&
	   syGameFlagGet(FLAG_RIME_REVEALED)) {
		fdActorGetById(ACTOR_RIMES_STATUE)->update =
			scAncientLemuriaRockTheStatue;
		fdActorGetById(ACTOR_RIMES_STATUE)->tmr = 0;
	}
	fdActorSetSprFlags(fdActorGetById(ACTOR_RIMES_STATUE), 0);
	return 0;
}

/* Notes which way the party came into the shore (FLAG_LEMURIA_OTHER_WAY_IN), */
/* and come the other way, redraws the south of the shore to match. */
s32 scPrepareMap(void) {
	s32 entrance = MainRoutineStateWork.entrance;

	if(entrance == ANCIENT_LEMURIA_AT_NORTHEAST_GATEWAY)
		syGameFlagSet(FLAG_LEMURIA_OTHER_WAY_IN);
	else if(entrance == ANCIENT_LEMURIA_FROM_LEMURIA_DOCK_EAST_A)
		syGameFlagClear(FLAG_LEMURIA_OTHER_WAY_IN);
	else if(entrance == ANCIENT_LEMURIA_FROM_LEMURIA)
		syGameFlagClear(FLAG_LEMURIA_OTHER_WAY_IN);
	else if(entrance == ANCIENT_LEMURIA_AT_SOUTHEAST_CORNER)
		syGameFlagSet(FLAG_LEMURIA_OTHER_WAY_IN);
	else if(entrance == ANCIENT_LEMURIA_FROM_LEMURIA_DOCK_EAST_B)
		syGameFlagSet(FLAG_LEMURIA_OTHER_WAY_IN);
	if(syGameFlagGet(FLAG_LEMURIA_OTHER_WAY_IN)) {
		fdMapGridCopyCellTilesCellsRect(38, 47, 26, 12, 8, 47);
		fdMapGridCopyCellTilesCellsRect(102, 47, 26, 12, 72, 47);
		fdMapCopyCellAttributeRect(38, 47, 26, 12, 8, 47);
	}
	return 0;
}

/* Felix walks east off the ledge beside the blue statue's spot (cell */
/* (9, 36)) and jumps across, if nothing stands in the way. */
void scAncientLemuriaJumpForward(void) { fdTryJumpForward(); }

/* The two Lemurians who saw the Psynergy come out, wonder whether the party */
/* are the ones who defeated Poseidon and, seeing Piers, hurry back into the */
/* city ahead of the party. */
void scAncientLemuriaMeetTheLemurians(void) {
	ACTOR *leader;
	s32 result;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdEventSetMessageId((s32)MsgLemuriaRecognizePsynergy);
	/* "Why, that was Psynergy, wasn't it?" */
	fdEventShowMessageThenWait(ACTOR_LEMURIAN, 0, 10);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	/* "Are they Adepts?" */
	fdEventShowMessageThenWait(ACTOR_SECOND_LEMURIAN, 0, 10);
	fdTryJumpForward();
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdEventWait(20);
	fdActorSetAnimationById(ACTOR_FELIX, ANIM_STAND);
	fdEventWait(20);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	fdEventWait(30);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
	fdEventWait(30);
	fdActorFaceDirectionById(ACTOR_FELIX, FACING_NORTHWEST, 0);
	fdEventWait(30);
	fdActorFaceDirectionById(ACTOR_FELIX, FACING_NORTHEAST, 0);
	fdEventWait(30);
	fdActorFaceDirectionById(ACTOR_FELIX, FACING_NORTH, 0);
	fdEventWait(30);
	fdActorSetSpeed(ACTOR_FELIX, 1);
	fdActorWalkByAndWait(ACTOR_FELIX, 0, -16);
	fdEventWait(10);
	fdCameraMoveTo(168, CAMERA_KEEP, 536, 1);
	fdCameraWaitForArrival();
	fdEventWait(20);
	fdActorSetSpeed(ACTOR_LEMURIAN, 0.8);
	fdActorWalkByAndWaitById(ACTOR_LEMURIAN, 24, 0);
	fdActorFaceDirection(ACTOR_LEMURIAN, FACING_SOUTH_SOUTHWEST, 0);
	fdEventWait(20);
	fdActorRunRepeatedMotion(ACTOR_LEMURIAN, 2);
	fdEventWait(10);
	/* "Who are you!?" */
	fdEventShowMessageThenWait(ACTOR_LEMURIAN, 0, 10);
	fdActorSetSpeed(ACTOR_SECOND_LEMURIAN, 0.8);
	fdActorWalkByAndWait(ACTOR_SECOND_LEMURIAN, -24, 0);
	fdActorFaceDirectionById(ACTOR_SECOND_LEMURIAN, FACING_SOUTH_SOUTHWEST, 0);
	fdEventWait(20);
	fdActorRunRepeatedMotion(ACTOR_SECOND_LEMURIAN, 2);
	fdEventWait(10);
	/* "Are you the ones who defeated Poseidon?" */
	fdEventOpenMessage(ACTOR_SECOND_LEMURIAN, 0);
	result = syFieldEventAskYesNoWithActorAnimation(ACTOR_FELIX, 0);
	if(result == 0) {
		fdEventWait(30);
		fdActorStartRepeatedMotion(ACTOR_SECOND_LEMURIAN, 3);
		fdEffectSetAttached(ACTOR_SECOND_LEMURIAN,
							ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
		fdEventWait(40);
		/* "That's quite something! Who are you, that you could defeat that..."
		 */
		fdEventShowMessageThenWait(ACTOR_SECOND_LEMURIAN, 0, 10);
		FieldStateWork->msg++;
	}
	else {
		fdEventWait(40);
		fdActorStartRepeatedMotion(ACTOR_SECOND_LEMURIAN, 3);
		fdEffectSetAttached(ACTOR_SECOND_LEMURIAN,
							ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
		fdEventWait(40);
		FieldStateWork->msg++;
		/* "But clearly, you are here, and the beast is gone..." */
		fdEventShowMessageThenWait(ACTOR_SECOND_LEMURIAN, 0, 0);
		fdActorJump(ACTOR_SECOND_LEMURIAN, 6, 15);
		fdActorJump(ACTOR_SECOND_LEMURIAN, 6, 23);
		fdEventWait(10);
	}
	fdActorShowEmote(ACTOR_LEMURIAN, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	/* "If you are strong enough to defeat Poseidon, you are too dangerous
	 * to..." */
	fdEventShowMessageThenWait(ACTOR_LEMURIAN, 0, 10);
	fdActorShowEmote(ACTOR_SECOND_LEMURIAN, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	/* "The king's orders are clear! We cannot grant you entry." */
	fdEventShowMessageThenWait(ACTOR_SECOND_LEMURIAN, 0, 10);
	scFieldActorLaunchFromLeader(ACTOR_KRADEN, 16, -16, FACING_NORTH_NORTHWEST);
	fdActorWaitForMoveById(ACTOR_KRADEN);
	fdEventWait(20);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	/* "King? Who is your king?" */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0,
									10);
	fdActorFaceDirection(ACTOR_LEMURIAN, FACING_SOUTH_SOUTHEAST, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_LEMURIAN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "His Highness, King Hydros, of course..." */
	fdEventShowMessageThenWait(ACTOR_LEMURIAN, 0, 10);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
	fdEventWait(50);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
	fdEventWait(30);
	fdActorSetAnimationById(ACTOR_FELIX, ANIM_SHAKE_HEAD);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_SECOND_LEMURIAN, 2);
	fdEventWait(10);
	/* "His Majesty has ruled over Lemuria for centuries." */
	fdEventShowMessageThenWait(ACTOR_SECOND_LEMURIAN, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_LEMURIAN, ANIM_NOD);
	fdEventWait(10);
	/* "King Hydros has decreed that Lemuria must be kept safe from
	 * dangerous..." */
	fdEventShowMessageThenWait(ACTOR_LEMURIAN, 0, 10);
	fdActorPlaceOnActor(ACTOR_PIERS, ACTOR_FELIX);
	fdActorGetById(ACTOR_PIERS)->scaleX = 0;
	/* "You say that we are dangerous, but do you include me as well?" */
	fdEventShowMessageThenWait(ACTOR_PIERS, 0, 10);
	fdActorFaceDirectionById(ACTOR_LEMURIAN, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_SECOND_LEMURIAN, FACING_WEST, 0);
	fdEventWait(45);
	fdActorFaceDirection(ACTOR_LEMURIAN, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_SECOND_LEMURIAN, FACING_SOUTH_SOUTHWEST, 0);
	fdEventWait(25);
	fdActorGetById(ACTOR_PIERS)->scaleX = FIXED_ONE;
	scFieldActorLaunchFromLeader(ACTOR_PIERS, 0, -16, FACING_NORTH);
	fdActorWaitForMoveById(ACTOR_PIERS);
	fdEventWait(20);
	fdActorFaceDirection(ACTOR_LEMURIAN, FACING_SOUTH_SOUTHWEST, 0);
	fdActorShowEmote(ACTOR_LEMURIAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_SECOND_LEMURIAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
					 30);
	/* "Piers!" */
	fdMessageShowAtActor(ACTOR_LEMURIAN, 0);
	fdEventWait(20);
	/* "Piers!" */
	fdEventShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_SECOND_LEMURIAN, 0,
							   0);
	mnMessageCloseFinishedWindows();
	fdEventWait(10);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
	fdActorWalkByAndWait(ACTOR_LEMURIAN, -8, 8);
	fdActorFaceDirection(ACTOR_LEMURIAN, FACING_SOUTH_SOUTHEAST, 0);
	fdEventWait(20);
	fdActorWalkByAndWait(ACTOR_SECOND_LEMURIAN, -8, 8);
	fdActorFaceDirection(ACTOR_SECOND_LEMURIAN, FACING_SOUTH_SOUTHWEST, 0);
	fdEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_LEMURIAN, ANIM_NOD);
	fdEventWait(10);
	/* "Did you join them in battle against Poseidon?" */
	fdEventShowMessageThenWait(ACTOR_LEMURIAN, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
	fdEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_SECOND_LEMURIAN, ANIM_SHAKE_HEAD);
	/* "Impossible! Everyone knows you possess great courage, but you still..."
	 */
	fdEventShowMessageThenWait(ACTOR_SECOND_LEMURIAN, 0, 10);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	/* "Such comments do us all a great disservice! I battled fiercely..." */
	fdEventShowMessageThenWait(ACTOR_PIERS, 0, 10);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTH_SOUTHEAST, 0);
	fdEventWait(35);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirectionById(ACTOR_FELIX, FACING_NORTHEAST, 0);
	fdEventWait(50);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	fdEventWait(30);
	fdActorSetAnimationById(ACTOR_KRADEN, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
	fdEventWait(25);
	fdActorStartRepeatedMotion(ACTOR_LEMURIAN, 3);
	fdActorSetAttachedEffect(ACTOR_LEMURIAN,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorStartRepeatedMotion(ACTOR_SECOND_LEMURIAN, 3);
	fdActorSetAttachedEffect(ACTOR_SECOND_LEMURIAN,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdEventWait(40);
	fdActorFaceDirectionById(ACTOR_LEMURIAN, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_SECOND_LEMURIAN, FACING_WEST, 0);
	fdEventWait(40);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTH_SOUTHEAST, 0);
	fdEventWait(30);
	/* "Leave our landing to me." */
	fdEventShowMessageThenWait(ACTOR_PIERS, 0, 10);
	fdActorSetSpeed(ACTOR_PIERS, 1);
	fdActorWalkByAndWait(ACTOR_PIERS, 0, -16);
	fdEventWait(10);
	/* "You have my word that we shall create no problems during our stay..." */
	fdEventShowMessageThenWait(ACTOR_PIERS, 0, 10);
	fdActorStartRepeatedMotion(ACTOR_LEMURIAN, 2);
	fdActorRunRepeatedMotion(ACTOR_SECOND_LEMURIAN, 2);
	fdEventWait(10);
	fdActorFaceDirection(ACTOR_LEMURIAN, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_SECOND_LEMURIAN, FACING_SOUTH_SOUTHWEST, 0);
	fdEventWait(30);
	fdActorShowEmote(ACTOR_LEMURIAN, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	/* "Piers, I know you are a man of your word, but the king's orders must..."
	 */
	fdEventShowMessageThenWait(ACTOR_LEMURIAN, 0, 10);
	fdActorShowEmote(ACTOR_SECOND_LEMURIAN, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	/* "You understand that we cannot simply ignore the king's orders, don't..."
	 */
	fdEventShowMessageThenWait(ACTOR_SECOND_LEMURIAN, 0, 10);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	/* "What is King Hydros doing..." */
	fdEventShowMessageThenWait(ACTOR_PIERS, 0, 10);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	/* "So, you people really take your kings seriously, don't you?" */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0,
									10);
	fdActorJump(ACTOR_LEMURIAN, 6, 15);
	fdActorJump(ACTOR_LEMURIAN, 6, 23);
	fdActorShowEmote(ACTOR_LEMURIAN, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	/* "Insolent pup! Do not dishonor the king of Lemuria! We are peaceful..."
	 */
	fdEventShowMessageThenWait(ACTOR_LEMURIAN, 0, 10);
	fdActorSetSprPriority(ACTOR_KRADEN, 2);
	fdActorJump(ACTOR_KRADEN, 6, 15);
	fdActorJump(ACTOR_KRADEN, 6, 23);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
	/* "Pup? I must be twice your age! Who are you calling "pup"?" */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0,
									10);
	fdActorShowEmote(ACTOR_SECOND_LEMURIAN, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	/* "You wish to see Lemuria, and yet you know so little of us! I am far..."
	 */
	fdEventShowMessageThenWait(ACTOR_SECOND_LEMURIAN, 0, 10);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	/* "Pah! Older than me?! Inconceivable!" */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0,
									10);
	fdActorFaceDirectionById(ACTOR_PIERS, FACING_SOUTHEAST, 0);
	fdEventWait(30);
	fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_SHAKE_HEAD);
	/* "Er... Actually, Kraden, these two are perhaps older than all of you..."
	 */
	fdEventShowMessageThenWait(ACTOR_PIERS, 0, 10);
	fdActorStartRepeatedMotion(ACTOR_KRADEN, 3);
	fdActorSetAttachedEffect(ACTOR_KRADEN,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdEventWait(50);
	fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
	fdEventWait(10);
	/* "Babi must have spoken of this to you. In Lemuria, time passes quite..."
	 */
	fdEventShowMessageThenWait(ACTOR_PIERS, 0, 10);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	/* "Ah... Well... I suppose I recall something of that sort or another..."
	 */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0,
									10);
	fdActorWalkByAndWait(ACTOR_KRADEN, 0, -16);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirectionById(ACTOR_PIERS, FACING_EAST, 0);
	fdEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	fdEventWait(10);
	/* "But perhaps I was rude, and I hope you accept my apology..." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0,
									10);
	fdActorSetAnimationAndWait(ACTOR_LEMURIAN, ANIM_NOD);
	fdEventWait(10);
	/* "Well, just so long as we're clear on that." */
	fdEventShowMessageThenWait(ACTOR_LEMURIAN, 0, 10);
	fdActorFaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
	fdEventWait(20);
	fdActorFaceDirection(ACTOR_SECOND_LEMURIAN, FACING_WEST, 0);
	fdEventWait(20);
	/* "But if your fighting is as skilled as your wordplay, then perhaps
	 * you..." */
	fdEventShowMessageThenWait(ACTOR_SECOND_LEMURIAN, 0, 10);
	fdActorFaceDirectionById(ACTOR_LEMURIAN, FACING_EAST, 0);
	fdEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_LEMURIAN, ANIM_NOD);
	fdEventWait(10);
	/* "I agree. Perhaps His Majesty will provide an exception in their case."
	 */
	fdEventShowMessageThenWait(ACTOR_LEMURIAN, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_SECOND_LEMURIAN, ANIM_NOD);
	fdEventWait(10);
	/* "King Hydros might be willing to reverse his order if we confer with..."
	 */
	fdEventShowMessageThenWait(ACTOR_SECOND_LEMURIAN, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_LEMURIAN, ANIM_NOD);
	fdEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_SECOND_LEMURIAN, ANIM_NOD);
	fdEventWait(20);
	fdActorFaceDirection(ACTOR_LEMURIAN, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_SECOND_LEMURIAN, FACING_SOUTH_SOUTHWEST, 0);
	fdEventWait(30);
	fdActorShowEmote(ACTOR_LEMURIAN, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "We shall speak with him at once!" */
	fdEventShowMessageThenWait(ACTOR_LEMURIAN, 0, 10);
	fdActorShowEmote(ACTOR_SECOND_LEMURIAN, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "King Hydros has been seeking news from beyond the Sea of Time. He..." */
	fdEventShowMessageThenWait(ACTOR_SECOND_LEMURIAN, 0, 10);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_SMILE, 55);
	fdActorSetAnimationAndWait(ACTOR_LEMURIAN, ANIM_NOD);
	fdEventWait(10);
	/* "Wait here." */
	fdEventShowMessageThenWait(ACTOR_LEMURIAN, 0, 10);
	fdActorFaceDirection(ACTOR_LEMURIAN, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_SECOND_LEMURIAN, FACING_NORTH_NORTHWEST, 0);
	fdEventWait(20);
	fdActorWalkBy(ACTOR_LEMURIAN, 0, -16);
	fdActorWalkByAndWait(ACTOR_SECOND_LEMURIAN, 0, -16);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
	fdEventWait(20);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	fdEventWait(10);
	/* "How long do you think we'll be waiting?" */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0,
									10);
	fdActorFaceDirectionById(ACTOR_PIERS, FACING_EAST, 0);
	fdEventWait(20);
	/* "Good point. Time means little to most Lemurians. Mere moments to us..."
	 */
	fdEventShowMessageThenWait(ACTOR_PIERS, 0, 10);
	fdActorStartRepeatedMotion(ACTOR_LEMURIAN, 3);
	fdActorSetAttachedEffect(ACTOR_LEMURIAN,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorStartRepeatedMotion(ACTOR_SECOND_LEMURIAN, 3);
	fdActorSetAttachedEffect(ACTOR_SECOND_LEMURIAN,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdEventWait(40);
	fdActorFaceDirection(ACTOR_LEMURIAN, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_SECOND_LEMURIAN, FACING_SOUTH_SOUTHWEST, 0);
	fdEventWait(30);
	fdCameraMoveTo(168, CAMERA_KEEP, 512, 1);
	fdCameraWaitForArrival();
	fdEventWait(10);
	fdActorShowEmote(ACTOR_LEMURIAN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	/* "You seem young to me, and yet you are probably an old man among your..."
	 */
	fdEventShowMessageThenWait(ACTOR_LEMURIAN, 0, 10);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
	fdEventWait(20);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 55);
	fdActorShowEmote(ACTOR_SECOND_LEMURIAN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP,
					 30);
	/* "King Hydros instructed us to be kind to the elderly at all times." */
	fdEventShowMessageThenWait(ACTOR_SECOND_LEMURIAN, 0, 10);
	fdActorFaceDirectionById(ACTOR_LEMURIAN, FACING_EAST, 0);
	fdEventWait(20);
	/* "They seem trustworthy enough... Shall we let them in?" */
	fdEventShowMessageThenWait(ACTOR_LEMURIAN, 0, 10);
	fdActorFaceDirection(ACTOR_SECOND_LEMURIAN, FACING_WEST, 0);
	fdEventWait(20);
	/* "I can see no reason not to. We won't get in trouble, will we?" */
	fdEventShowMessageThenWait(ACTOR_SECOND_LEMURIAN, 0, 10);
	fdActorFaceDirectionById(ACTOR_PIERS, FACING_EAST, 0);
	fdActorFaceDirectionById(ACTOR_KRADEN, FACING_WEST, 0);
	fdEventWait(20);
	fdActorSetSprPriority(ACTOR_FELIX, 2);
	fdActorWalkByAndWait(ACTOR_FELIX, 8, -20);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	fdEventWait(20);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "I will be with them! Now, please, let me walk the streets of my home..."
	 */
	fdEventShowMessageThenWait(ACTOR_PIERS, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	fdEventWait(20);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
	fdActorFaceDirectionById(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
	fdEventWait(20);
	/* "I beseech you!" */
	fdEventShowMessageThenWait(ACTOR_PIERS, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_LEMURIAN, ANIM_NOD);
	fdEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_SECOND_LEMURIAN, ANIM_NOD);
	fdEventWait(20);
	fdActorFaceDirection(ACTOR_LEMURIAN, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_SECOND_LEMURIAN, FACING_SOUTH_SOUTHWEST, 0);
	fdEventWait(30);
	fdActorSetAnimationAndWait(ACTOR_LEMURIAN, ANIM_NOD);
	fdEventWait(10);
	/* "Then we place our trust in each of you. You may enter. But...don't
	 * do..." */
	fdEventShowMessageThenWait(ACTOR_LEMURIAN, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_SECOND_LEMURIAN, ANIM_NOD);
	fdEventWait(10);
	/* "If you cause any trouble, we'll hunt you down and exact swift and..." */
	fdEventShowMessageThenWait(ACTOR_SECOND_LEMURIAN, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_LEMURIAN, 2);
	fdEventWait(10);
	/* "Do we have your word?" */
	fdMessageShowAtActor(ACTOR_LEMURIAN, 0);
	if((s8)MainRoutineStateWork.weirdAnswers <= 4) {
		result = syFieldEventAskYesNoWithActorAnimation(ACTOR_FELIX, 0);
		if(result == 0) {
			fdEventWait(25);
			fdActorFaceDirectionById(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
			fdEventWait(30);
			fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
			fdEventWait(10);
			/* "Ooo! Ooo! Finally! What could be waiting for me in Lemuria?
			 * I..." */
			fdEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0,
									   10);
			FieldStateWork->msg++;
		}
		else {
			fdEventWait(35);
			fdActorShowEmote(ACTOR_LEMURIAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
			fdActorShowEmote(ACTOR_SECOND_LEMURIAN,
							 EMOTE_IN_FRONT | EMOTE_WINCE, 50);
			fdActorFaceDirectionById(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
			fdEventWait(30);
			fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN,
							 30);
			FieldStateWork->msg++;
			/* "We've come a long way to see Lemuria. I won't have anything
			 * ruin..." */
			fdEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0,
									   10);
		}
		FieldStateWork->msg += 2;
	}
	else {
		FieldStateWork->msg += 2;
		if(syFieldEventAskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
			fdEventWait(25);
			fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
			fdActorFaceDirectionAndRefreshById(ACTOR_PIERS,
											   FACING_SOUTH_SOUTHEAST);
			fdEventWait(10);
			fdActorShowEmoteById(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
			fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
			fdEventWait(10);
			/* "But thank you for speaking honestly. Sometimes, you can say
			 * the..." */
			fdEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0,
									   10);
			FieldStateWork->msg++;
		}
		else {
			fdEventWait(35);
			fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
			fdActorFaceDirectionAndRefreshById(ACTOR_PIERS,
											   FACING_SOUTH_SOUTHEAST);
			fdEventWait(10);
			fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN,
							 40);
			fdActorJump(ACTOR_KRADEN, 6, 15);
			fdActorJump(ACTOR_KRADEN, 6, 23);
			FieldStateWork->msg++;
			/* "What!? Are you INSANE!? Or maybe you think you're funny?
			 * Because..." */
			fdEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0,
									   10);
		}
		fdActorFaceDirectionAndRefreshById(ACTOR_PIERS, FACING_NORTH);
		fdEventWait(10);
	}
	fdActorWalkByAndWait(ACTOR_PIERS, 0, -10);
	fdEventWait(15);
	fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
	fdEventWait(10);
	/* "Oh, hush. I'll keep a close eye on them. You have nothing to fear." */
	fdEventShowMessageThenWait(ACTOR_PIERS, 0, 10);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
	fdEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_SECOND_LEMURIAN, ANIM_NOD);
	fdEventWait(10);
	/* "Very well, Piers. We place our faith in you, then. Enter freely and..."
	 */
	fdEventShowMessageThenWait(ACTOR_SECOND_LEMURIAN, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_LEMURIAN, 2);
	fdEventWait(10);
	/* "You must already be aware of this, but you are not permitted to
	 * enter..." */
	fdEventShowMessageThenWait(ACTOR_LEMURIAN, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_SECOND_LEMURIAN, ANIM_SHAKE_HEAD);
	/* "If the king were to find out about this, we'd be in big trouble." */
	fdEventShowMessageThenWait(ACTOR_SECOND_LEMURIAN, 0, 10);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "Don't worry! I won't let that happen." */
	fdEventShowMessageThenWait(ACTOR_PIERS, 0, 10);
	fdActorSetAnimationById(ACTOR_LEMURIAN, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_SECOND_LEMURIAN, ANIM_NOD);
	fdEventWait(25);
	fdCameraFollowActor(ACTOR_SECOND_LEMURIAN, 1);
	fdActorWalkBy(ACTOR_SECOND_LEMURIAN, 0, -32);
	fdEventWait(10);
	fdActorWalkByAndWaitById(ACTOR_LEMURIAN, 16, 0);
	fdActorWalkBy(ACTOR_LEMURIAN, 0, -32);
	fdActorWaitForMoveById(ACTOR_SECOND_LEMURIAN);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdActorSetPos(ACTOR_SECOND_LEMURIAN, 0, 0);
	fdActorWaitForMoveById(ACTOR_LEMURIAN);
	fdActorSetPos(ACTOR_LEMURIAN, 0, 0);
	fdEventWait(30);
	fdCameraMoveTo(168, CAMERA_KEEP, 536, 1);
	fdCameraWaitForArrival();
	fdEventWait(20);
	fdActorWalkByAndWait(ACTOR_KRADEN, 0, -10);
	fdActorFaceDirectionById(ACTOR_KRADEN, FACING_WEST, 0);
	fdEventWait(20);
	/* "Is this Hydros really so fearsome?" */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0,
									10);
	fdActorFaceDirectionById(ACTOR_PIERS, FACING_EAST, 0);
	fdEventWait(20);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	/* "Fearsome isn't the right word... King Hydros has lived longer than..."
	 */
	fdEventShowMessageThenWait(ACTOR_PIERS, 0, 10);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "While everyone respects him and holds him in great reverence... It..."
	 */
	fdEventShowMessageThenWait(ACTOR_PIERS, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_PIERS, 2);
	fdEventWait(10);
	/* "I certainly do not fear him. The king... He..." */
	fdEventShowMessageThenWait(ACTOR_PIERS, 0, 10);
	fdActorFaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
	fdEventWait(40);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	/* "He what?" */
	fdEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 10);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 50);
	fdActorFaceDirectionById(ACTOR_PIERS, FACING_EAST, 0);
	fdEventWait(30);
	fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_SHAKE_HEAD);
	/* "You will understand once you meet the king yourself." */
	fdEventShowMessageThenWait(ACTOR_PIERS, 0, 10);
	fdActorFaceDirectionById(ACTOR_PIERS, FACING_NORTH, 0);
	fdEventWait(20);
	fdActorWalkByAndWait(ACTOR_PIERS, 0, -32);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
	fdEventWait(10);
	fdActorFaceDirectionById(ACTOR_PIERS, FACING_SOUTH_SOUTHEAST, 0);
	fdEventWait(30);
	/* "But that does not matter now. Let's go!" */
	fdEventShowMessageThenWait(ACTOR_PIERS, 0, 10);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "To Lemuria... The city you've all been longing to see..." */
	fdEventShowMessageThenWait(ACTOR_PIERS, 0, 10);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
	fdActorFaceDirectionById(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
	fdEventWait(30);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
	fdEventWait(20);
	fdActorSetSpeed(ACTOR_FELIX, 0.8);
	fdActorSetSpeed(ACTOR_KRADEN, 0.8);
	fdActorWalkByAndWait(ACTOR_FELIX, -8, -16);
	fdActorWalkBy(ACTOR_KRADEN, 0, -10);
	fdActorWalkByAndWait(ACTOR_FELIX, 0, -16);
	fdActorWaitForMoveById(ACTOR_KRADEN);
	fdActorFaceDirectionById(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
	fdEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
	fdEventWait(20);
	fdActorSetAnimationById(ACTOR_FELIX, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	fdEventWait(20);
	fdActorSetSpeed(ACTOR_PIERS, 1.2);
	fdActorSetAnimationById(ACTOR_PIERS, ANIM_WALK);
	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	if(leader != 0)
		fdActorSetDestinationById(ACTOR_PIERS, leader->x.part[0].integer,
								  leader->z.part[0].integer);
	fdActorWaitForMoveById(ACTOR_PIERS);
	fdActorSetPos(ACTOR_PIERS, 0, 0);
	fdActorSetSpeed(ACTOR_KRADEN, 1.2);
	fdActorSetAnimationById(ACTOR_KRADEN, ANIM_WALK);
	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	if(leader != 0)
		fdActorSetDestinationById(ACTOR_KRADEN, leader->x.part[0].integer,
								  leader->z.part[0].integer);
	fdActorWaitForMoveById(ACTOR_KRADEN);
	fdActorSetPos(ACTOR_KRADEN, 0, 0);
	fdEventEnd();
}

SCENE_ENTRANCE SceneEntrances[] = {
	{0, CONDITION_ALWAYS, 100, 0, 100, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

u32 SceneExits[] = {
	SCENE_EXITS(AncientLemuria),
	SCENE_EXIT_UNLESS(ANCIENT_LEMURIA_TO_SOUTHEAST_CORNER, AncientLemuria,
					  ANCIENT_LEMURIA_AT_SOUTHEAST_CORNER, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(ANCIENT_LEMURIA_TO_LEMURIA_DOCK_EAST_A, LemuriaDock,
					  LEMURIA_DOCK_FROM_ANCIENT_LEMURIA_EAST_A, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(ANCIENT_LEMURIA_TO_LEMURIA, Lemuria,
					  LEMURIA_FROM_ANCIENT_LEMURIA, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(ANCIENT_LEMURIA_TO_NORTHEAST_GATEWAY, AncientLemuria,
					  ANCIENT_LEMURIA_AT_NORTHEAST_GATEWAY, EXIT_FLAG_NONE),
	SCENE_EXITS_END,
};

ACTOR_PLACE ActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_BLUE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
					152, 0, 584, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_LEMURIAN_GIRL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
					136, 0, 504, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_LEMURIAN_GIRL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
					200, 0, 504, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
	{SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -toFIXED(56), 0,
	 -toFIXED(56), FACING_NORTH, TALK_FACING_KEEP, 0},
	{SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -toFIXED(56), 0,
	 -toFIXED(56), FACING_NORTH, TALK_FACING_KEEP, 0},
	{SPRITE_MERCURY_DJINNI, FLAG_DJINNI(DJINNI_RIME), MOVEMENT_TYPE_STAND,
	 -toFIXED(56), 0, -toFIXED(56), FACING_SOUTH, TALK_FACING_PARTY, 0},
	ACTOR_PLACEMENT(SPRITE_BLUE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
					264, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

TRIGGER SceneEvents[] = {
	{TRIGGER_EXIT, 1, CONDITION_ALWAYS, ANCIENT_LEMURIA_TO_SOUTHEAST_CORNER},
	{TRIGGER_STEP, CELL_BEHAVIOR_WAY_TO_DOCK, CONDITION_ALWAYS,
	 EVENT(scAncientLemuriaHeadForTheDock)},
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 3, CONDITION_ALWAYS,
	 ANCIENT_LEMURIA_TO_LEMURIA},
	{TRIGGER_EXIT, 4, CONDITION_ALWAYS, ANCIENT_LEMURIA_TO_NORTHEAST_GATEWAY},
	{TRIGGER_STEP, CELL_BEHAVIOR_KEEP_CAMERA_SOUTH, CONDITION_ALWAYS,
	 EVENT(scAncientLemuriaKeepCameraSouth)},
	{TRIGGER_STEP, CELL_BEHAVIOR_LET_CAMERA_NORTH, CONDITION_ALWAYS,
	 EVENT(scAncientLemuriaLetCameraNorth)},
	{STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_LEDGE_BY_STATUE_SPOT,
	 CONDITION_ALWAYS, EVENT(scAncientLemuriaJumpForward)},
	{TRIGGER_TALK, ACTOR_RIME, CONDITION_ALWAYS,
	 EVENT(scAncientLemuriaRimeJoins)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
	 ACTOR_BLUE_STATUE, FLAG_BLUE_STATUE_MOVED,
	 (u32)(scAncientLemuriaStartMovingStatue)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_BLUE_STATUE, FLAG_BLUE_STATUE_MOVED,
	 EVENT(scAncientLemuriaFinishMovingStatue)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
	 CELL_BEHAVIOR_RIMES_WEEDS, CONDITION_ALWAYS,
	 EVENT(scAncientLemuriaCycloneRevealsRime)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
	 CELL_BEHAVIOR_SECOND_WEEDS, CONDITION_ALWAYS,
	 EVENT(scAncientLemuriaCycloneSecondWeeds)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
	 CELL_BEHAVIOR_THIRD_WEEDS, CONDITION_ALWAYS,
	 EVENT(scAncientLemuriaCycloneThirdWeeds)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
	 CELL_BEHAVIOR_FOURTH_WEEDS, CONDITION_ALWAYS,
	 EVENT(scAncientLemuriaCycloneFourthWeeds)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
	 CELL_BEHAVIOR_FIFTH_WEEDS, CONDITION_ALWAYS,
	 EVENT(scAncientLemuriaCycloneFifthWeeds)},
	{PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
	 CELL_BEHAVIOR_SIXTH_WEEDS, CONDITION_ALWAYS,
	 EVENT(scAncientLemuriaCycloneSixthWeeds)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_TREMOR, PSYNERGY_PHASE_BEFORE_CAST),
	 ACTOR_RIMES_STATUE, IF_FLAG_SET(FLAG_RIME_IN_THE_STATUE),
	 EVENT(scAncientLemuriaStartTremorOnTheStatue)},
	{PSYNERGY_ON_ACTOR(ABILITY_TREMOR), ACTOR_RIMES_STATUE,
	 IF_FLAG_SET(FLAG_RIME_IN_THE_STATUE),
	 EVENT(scAncientLemuriaTremorShakesOutRime)},
	{TRIGGER_TABLE_END},
};

/* The colours the shining water takes in turn. */
/* clang-format off */
s16 AncientLemuria_ShiningWaterColors[SHINING_WATER_COLORS] = {
    RGB(12, 24, 23), RGB(8, 22, 24), RGB(4, 20, 25),
    RGB(0, 18, 26),  RGB(0, 16, 26), RGB(0, 14, 26),
    RGB(0, 12, 26),  RGB(0, 14, 26), RGB(0, 16, 26),
    RGB(0, 18, 26),  RGB(4, 20, 25), RGB(8, 22, 24),
};
/* clang-format on */
