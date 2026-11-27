/* @file */
/* Idejima, the peninsula at the end of the road west of Venus Lighthouse, */
/* where the story of The Lost Age begins. */
/* */
/* The scene is entered by its story's three cutscenes, one entrance each, and
 */
/* then by the party washed ashore: */
/* */
/* - Jenna and Kraden reach Idejima and find Menardi's ship without the orb */
/*   that moves it. Alex joins them, the beacon is lit, and the tremor tears */
/*   Idejima away from Gondowan (scIdejimaBreakAwayFromGondowan, entrance 11).
 */
/* - Adrift, Alex shows Jenna and Kraden that Felix and Sheba have washed up on
 */
/*   the island (scIdejimaFindFelixAndSheba, entrance 12). */
/* - Sheba wakes, land comes in sight, and a tidal wave carries the island */
/*   onto it (scIdejimaRideTheTidalWave, entrance 13). */
/* - Felix wakes on the grass of the island, now run aground on the coast of */
/*   Indra near Daila (scIdejimaFelixWakes), with Jenna, Sheba and Kraden still
 */
/*   lying where the wave left them (scIdejimaLayDownTheOthers). Each wakes */
/*   when he talks to them (scIdejimaWakeKraden, scIdejimaWakeJenna, */
/*   scIdejimaWakeSheba), or runs after him if he heads west without them */
/*   (scIdejimaCatchUpWithFelix). Once all three have joined, they wonder where
 */
/*   Alex has gone (scIdejimaWonderWhereAlexHasGone), and the island's west */
/*   edge leads out onto the world map. */
/* */
/* While the island drifts, the sea around it ripples */
/* (scIdejimaUpdateWaveLines, scIdejimaApplyWaveLines). */

#include "field/field.h"
#include "game/game.h"
#include "graphics/palette_buffers.h"
#include "macros.h"
#include "field/field_event.h"
#include "field/event_inline.h"
#include "field/call.h"
#include "scene/scene.h"
#include "field/actor_script.h"
#include "text/msg_ids.h"

/* Set once the leader has been walked back from the edge of the map by */
/* scIdejimaCatchUpWithFelix, so the next one to catch up finds him there. */
#define FLAG_IDEJIMA_LEADER_WALKED_BACK FLAG_SCENE_TEMP_00

/* The entrances the triggers are chosen by: two at the foot of the mountain, */
/* where only the unseen door answers, and the two of the island run aground. */
typedef enum IDEJIMA_ENTRANCE {
	/* At the foot of the mountain, cell (11, 10), with the unseen door north.
	 */
	/* No exit table or event in the game leads here by it; the unseen door */
	/* leads back by ENTRANCE_FROM_DOOR_AGAIN. */
	ENTRANCE_FROM_DOOR = 1,
	/* The island run aground, where Felix wakes after the tidal wave. */
	ENTRANCE_WAKE = 2,
	ENTRANCE_FROM_DOOR_AGAIN = 3,
	/* Back onto the island from the world map, at its west edge. */
	ENTRANCE_BACK_FROM_WORLD_MAP = 4,
	/* Two of the story's three scenes before the shore, each played on arriving
	 */
	/* by its own entrance (scInit); the third is the title's */
	/* IDEJIMA_RIDE_THE_TIDAL_WAVE. */
	ENTRANCE_BREAK_AWAY = 11,
	ENTRANCE_FIND_FELIX_AND_SHEBA = 12
} IDEJIMA_ENTRANCE;

/* The unseen door, which leads back out at ENTRANCE_FROM_DOOR_AGAIN. */
enum { EXIT_DOOR = 3 };

/* The cells the triggers answer, by their behaviour in the map. */
typedef enum IDEJIMA_CELL_BEHAVIOR {
	/* The unseen door at the foot of the mountain, cell (11, 10). */
	CELL_BEHAVIOR_DOOR = 3,
	/* The line across the grass at (5..6, 44..47), where the party talks about
	 */
	/* Alex. */
	CELL_BEHAVIOR_WHERE_ALEX_WENT = 8,
	/* The line across the grass at (8..10, 44..47), east of it, where the */
	/* sleepers catch up with Felix. */
	CELL_BEHAVIOR_CATCH_UP = 9,
	/* Stamped under Kraden, Jenna and Sheba where each lies asleep, from the */
	/* spare cells at (38..39, 32..34). */
	CELL_BEHAVIOR_BY_KRADEN = 10,
	CELL_BEHAVIOR_BY_JENNA,
	CELL_BEHAVIOR_BY_SHEBA
} IDEJIMA_CELL_BEHAVIOR;

/* The scene's placed actors, numbering the rows of IdejimaPlacements; its */
/* rows of Jenna's and Sheba's sprites place ACTOR_JENNA and ACTOR_SHEBA. */
typedef enum IDEJIMA_ACTOR_ID {
	ACTOR_KRADEN = ACTOR_FIRST_PLACED,
	/* Menardi's ship, moored in a cove of the peninsula. */
	ACTOR_SHIP,
	ACTOR_ALEX,
	/* An actor with no picture that speaks the lines no one says, above Felix
	 */
	/* as he wakes. */
	ACTOR_NARRATOR
} IDEJIMA_ACTOR_ID;

/* Kraden's sprite lying asleep on his side. */
enum { KRADEN_ANIMATION_ASLEEP = 5 };

/* Animation 18 of Sheba's sheet sits her up on the ground; of Jenna's, it */
/* lays her on her side. */
enum { ANIM_SHEBA_SIT_UP = 18, ANIM_JENNA_LIE_ON_SIDE = 18 };

/* Menardi's ship rocking every 10 frames, quicker than at rest, as the island
 */
/* drifts. */
enum { SHIP_ANIMATION_ROCK_FAST = 2 };

extern SCENE_ENTRANCE SceneEntrances[];
extern u32 IdejimaExits[];
extern TRIGGER IdejimaEvents[];
extern TRIGGER IdejimaEventsFromDoor[];
extern TRIGGER IdejimaEventsWake[];
extern ACTOR_PLACE IdejimaPlacements[];
extern s32 Idejima_StretchScript[];
extern u32 Idejima_KradenStepsAsideScript[];
extern u32 Idejima_JoinTheLeaderScript[];
extern u32 Idejima_GoAfterAlexScript[];
extern u32 Idejima_KradenToTheShipScript[];
extern u32 Idejima_JennaToTheShipScript[];
extern u32 Idejima_AlexLeadsTheWayScript[];
extern u32 Idejima_KradenFollowsAlexScript[];
extern u32 Idejima_JennaFollowsAlexScript[];
extern u32 Idejima_KradenRunsToFelixScript[];
extern u32 Idejima_JennaRunsToFelixScript[];
extern u32 Idejima_JennaToTheShoreScript[];
extern u32 Idejima_ShebaToTheShoreScript[];
extern u32 Idejima_KradenToTheShoreScript[];
extern u32 Idejima_FelixToTheShoreScript[];

/* The wave scroll's two tables of BG3 row offsets, one for each frame in turn.
 */
s16 Idejima_WaveLines[2][160] BSS_DATA ALIGNED(16);
/* The frame count the wave scroll's phase follows. */
s32 Idejima_WavePhase BSS_DATA;
/* Which of Idejima_WaveLines the next frame computes. */
s16 Idejima_WaveBuffer BSS_DATA;

void scIdejimaResetWaveLines(void);
void scIdejimaApplyWaveLines(void);
void scIdejimaUpdateWaveLines(void);
void scIdejimaUseDriftPalette(void);
void scDriftScenePrepare(void);
void scIdejimaLayDownTheOthers(void);
void scIdejimaWakeKraden(void);
void scIdejimaWakeJenna(void);
void scIdejimaWakeSheba(void);
void scIdejimaCatchUpWithFelix(void);
void scIdejimaWonderWhereAlexHasGone(void);
/* Counts the answer just given (fdEventCountAnswerStub). */
void scIdejimaCountAnswer();

const SCENE_ENTRANCE *scGetEntrances(void) { return SceneEntrances; }

const SCENE_REGION *scGetExitRegions(void) { return 0; }

const u32 *scGetExits(void) { return IdejimaExits; }

const ACTOR_PLACE *scGetActorPlacements(void) { return IdejimaPlacements; }

const TRIGGER *scGetTriggers(void) {
	switch(MainRoutineStateWork.entrance) {
	case ENTRANCE_FROM_DOOR:
	case ENTRANCE_FROM_DOOR_AGAIN:
		return IdejimaEventsFromDoor;
	case ENTRANCE_WAKE:
	case ENTRANCE_BACK_FROM_WORLD_MAP:
		return IdejimaEventsWake;
	default:
		return IdejimaEvents;
	}
}

/* Stands a sleeper up where it lay, with a little hop. */
void scIdejimaGetUp(s32 id) {
	ACTOR *actor = fdActorGet(id);

	fdActorSetAnimation(id, ANIM_STAND);
	fdActorSetSprFlags(fdActorGet(id), 1);
	actor->x.fixed -= toFIXED(12);
	((FIELD_SPR *)actor->spr)->rotation = 0;
	syWait(1);
	fdActorSetSprFlags(fdActorGet(id), 1);
	actor->velY = toFIXED(4);
	actor->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	syWait(40);
}

/* Talking to Kraden, asleep on the grass north-east of where Felix woke, or */
/* reading his mind: he groans awake, marvels that the wave carried them to */
/* solid land and joins the party. */
void scIdejimaWakeKraden(void) {
	s32 message;
	ACTOR *actor;
	ACTOR *leader;

	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 1);
	message = (s32)MsgIdejimaGroan;
	syFieldEventSetMessageId(message);
	/* "Oooog..." */
	fdEventShowMessage(ACTOR_KRADEN, 0);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	/* "Oh... Felix..." */
	fdEventShowMessage(ACTOR_KRADEN, 0);
	fdMapGridCopyCellAttributes(38, 31, 2, 1, 25, 42);
	actor = fdActorGet(ACTOR_KRADEN);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_STAND);
	fdActorSetSprFlags(fdActorGet(ACTOR_KRADEN), 1);
	syWait(1);
	fdActorSetSprFlags(fdActorGet(ACTOR_KRADEN), 1);
	actor->velY = toFIXED(4);
	actor->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	syWait(40);
	/* "Ouch..." */
	fdEventShowMessage(ACTOR_KRADEN, 0);
	fdActorFaceActor(ACTOR_KRADEN, MainRoutineStateWork.leaderActorId, 0);
	fdActorSetAttachedEffect(ACTOR_KRADEN,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldEventWait(20);
	/* "That was quite a blow..." */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 20);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
	/* "What happened when that wave hit?" */
	fdEventShowMessage(ACTOR_KRADEN, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 40);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 40);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 40);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	fdActorSetSpeed(ACTOR_KRADEN, 0.8);
	fdActorWalkToAndWait(ACTOR_KRADEN, 416, 680);
	/* "Felix, look!" */
	fdEventShowMessage(ACTOR_KRADEN, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_WEST, 20);
	fdActorSetAttachedEffect(MainRoutineStateWork.leaderActorId,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldEventWait(20);
	/* "That wave carried us to the continent!" */
	fdEventShowMessage(ACTOR_KRADEN, 0);
	fdActorFaceActor(ACTOR_KRADEN, MainRoutineStateWork.leaderActorId, 0);
	syFieldEventWait(20);
	fdActorFaceActor(MainRoutineStateWork.leaderActorId, ACTOR_KRADEN, 0);
	/* "We've hit solid land! Let's go, Felix!" */
	fdEventShowMessage(ACTOR_KRADEN, 0);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_WALK);
	leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	if(leader != 0)
		fdActorSetDestination(ACTOR_KRADEN, leader->x.part[0].integer,
							  leader->z.part[0].integer);
	scFieldActorWaitForMoveById(ACTOR_KRADEN);
	fdActorSetPos(ACTOR_KRADEN, 0, 0);
	mnMessageShowCentered(message + 8, 1);
	fdEventSetFlag(FLAG_IDEJIMA_KRADEN_JOINED);
	fdEventEnd();
}

/* Talking to Jenna, asleep on the grass west of where Felix woke, or reading */
/* her mind: she wakes, asks whether he has checked on the others and joins */
/* the party. */
void scIdejimaWakeJenna(void) {
	ACTOR *actor;
	ACTOR *leader;

	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 1);
	syFieldEventWait(40);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
	syFieldEventWait(20);
	syFieldEventSetMessageId(MsgIdejimaAwakeningExclamation);
	/* "What..." */
	syFieldEventShowMessageThenWait(ACTOR_JENNA, 0, 40);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
	/* "What am I doing down here?" */
	fdEventShowMessage(ACTOR_JENNA, 0);
	fdMapGridCopyCellAttributes(38, 31, 2, 1, 17, 46);
	actor = fdActorGet(ACTOR_JENNA);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_STAND);
	fdActorSetSprFlags(fdActorGet(ACTOR_JENNA), 1);
	syWait(1);
	fdActorSetSprFlags(fdActorGet(ACTOR_JENNA), 1);
	actor->velY = toFIXED(4);
	actor->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	syWait(40);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	/* "Oh... I must have blacked out when the wave hit..." */
	fdEventShowMessage(ACTOR_JENNA, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 20);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 20);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 40);
	scFieldActorJump(ACTOR_JENNA, 4, 20);
	/* "Hey! We've hit land!" */
	fdEventShowMessage(ACTOR_JENNA, 0);
	fdActorFaceActor(ACTOR_JENNA, MainRoutineStateWork.leaderActorId, 0);
	syFieldEventWait(20);
	fdActorFaceActor(MainRoutineStateWork.leaderActorId, ACTOR_JENNA, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
	/* "That wave must have carried us here! What luck!" */
	fdEventShowMessage(ACTOR_JENNA, 0);
	fdActorSetAnimationAndWait(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	syFieldEventWait(40);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	/* "Have you checked everyone else? Are they all right?" */
	fdEventOpenMessage(ACTOR_JENNA, 0);
	if(syFieldEventAskYesNoWithActorAnimation(
		   MainRoutineStateWork.leaderActorId, 0) == 0) {
		fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
		/* "Great! Then let's go!" */
		fdEventShowMessage(ACTOR_JENNA, 0);
	}
	else {
		FieldStateWork->msg += 2;
		/* "Then let's go find them and get off this island!" */
		fdEventShowMessage(ACTOR_JENNA, 0);
	}
	fdActorSetSpeed(ACTOR_JENNA, 1.6);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_WALK);
	leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	if(leader != 0)
		fdActorSetDestination(ACTOR_JENNA, leader->x.part[0].integer,
							  leader->z.part[0].integer);
	scFieldActorWaitForMoveById(ACTOR_JENNA);
	fdActorSetPos(ACTOR_JENNA, 0, 0);
	syFieldEventAddPartyMember(ACTOR_JENNA, 1);
	fdEventSetFlag(FLAG_IDEJIMA_JENNA_JOINED);
	fdEventEnd();
}

/* Talking to Sheba, asleep on the grass north-west of where Felix woke: she */
/* wakes, wonders where they are and joins the party. */
void scIdejimaWakeSheba(void) {
	ACTOR *leader;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdActorRunRepeatedMotion(ACTOR_SHEBA, 2);
	fdEventSetMessageId(MsgIdejimaSecondGroan);
	/* "Ohhh..." */
	fdEventShowMessageThenWait(ACTOR_SHEBA, 0, 40);
	/* "Nnng." */
	fdEventShowMessageAndWait(ACTOR_SHEBA, 0);
	fdActorSetAnimationById(ACTOR_SHEBA, ANIM_SHEBA_SIT_UP);
	/* "The wave!" */
	fdEventShowMessageAndWait(ACTOR_SHEBA, 0);
	fdMapGridCopyCellAttributes(38, 31, 2, 1, 14, 40);
	fdActorGetById(ACTOR_SHEBA)->motionFlg =
		ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	fdActorJump(ACTOR_SHEBA, 4, 0);
	fdActorSetAnimationById(ACTOR_SHEBA, ANIM_STAND);
	fdActorSetSprFlags(fdActorGetById(ACTOR_SHEBA), 1);
	fdEventWait(20);
	fdActorTurnToFaceEachOther(ACTOR_SHEBA, MainRoutineStateWork.leaderActorId,
							   0);
	/* "What happened to the tidal wave?" */
	fdEventShowMessageAndWait(ACTOR_SHEBA, 0);
	fdActorSetAnimationAndWait(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	fdActorShowEmoteById(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
	/* "I thought we were done for, but we're all fine..." */
	fdEventShowMessageAndWait(ACTOR_SHEBA, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 20);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 40);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTHWEST, 0);
	fdActorShowEmoteById(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	/* "And we've run aground!" */
	fdEventShowMessageAndWait(ACTOR_SHEBA, 0);
	fdActorFaceActorById(ACTOR_SHEBA, MainRoutineStateWork.leaderActorId, 0);
	fdEventWait(20);
	fdActorSetAnimationById(ACTOR_SHEBA, ANIM_NOD);
	/* "That tidal wave turned out to be a stroke of luck!" */
	fdEventShowMessageAndWait(ACTOR_SHEBA, 0);
	fdActorSetAnimationAndWait(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	/* "I wonder where we are..." */
	fdEventShowMessageAndWait(ACTOR_SHEBA, 0);
	fdActorSetSpeed(ACTOR_SHEBA, 1.6);
	fdActorWalkToAndWait(ACTOR_SHEBA, 212, 680);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 20);
	/* "We should take a look around!" */
	fdEventShowMessageAndWait(ACTOR_SHEBA, 0);
	fdActorSetAnimationById(ACTOR_SHEBA, ANIM_WALK);
	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	if(leader != 0)
		fdActorSetDestinationById(ACTOR_SHEBA, leader->x.part[0].integer,
								  leader->z.part[0].integer);
	fdActorWaitForMoveById(ACTOR_SHEBA);
	fdActorSetPos(ACTOR_SHEBA, 0, 0);
	fdEventAddPartyMember(ACTOR_SHEBA, 1);
	syGameFlagSet(FLAG_IDEJIMA_SHEBA_JOINED);
	fdEventEnd();
}

/* Walking north onto the bare grass at the foot of the mountain, cell */
/* (11, 10), plays a door's sound and leaves through a door that is not drawn,
 */
/* to come straight back out on the same spot. */
/* */
/* Only entrances 1 and 3 answer this cell, and the door itself is the only */
/* way the game enters by 3, so a player who arrives with the story never */
/* meets it. */
void scIdejimaEnterDoor(void) {
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	sySndPlay(SE_MAP_EXIT);
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	syImportEventSetExitRequest(EXIT_DOOR);
}

/* Walks Felix back from the edge of the shore, once a visit. */
void scIdejimaWalkLeaderBack(void) {
	if(!fdEventGetFlag(FLAG_IDEJIMA_LEADER_WALKED_BACK)) {
		fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.8);
		fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 152, 756);
		fdEventSetFlag(FLAG_IDEJIMA_LEADER_WALKED_BACK);
	}
}

/* Walking west across the grass towards the island's edge, cells */
/* (8..10, 44..47), while any of the others is still asleep: Felix is walked */
/* back a little, and each sleeper in turn wakes, runs after him, scolds him */
/* for leaving them behind and joins the party. */
void scIdejimaCatchUpWithFelix(void) {
	ACTOR *leader;

	if(fdEventGetFlag(FLAG_IDEJIMA_KRADEN_JOINED) == 0) {
		fdEventStart();
		syFieldEventEndLastingPsynergy(0);
		scIdejimaWalkLeaderBack();
		syFieldEventSetMessageId((s32)MsgIdejimaHeyFelix);
		/* "Hey, Felix!" */
		fdEventShowMessage(ACTOR_KRADEN, 0);
		fdActorSetPos(ACTOR_JENNA, 0, 0);
		fdActorSetPos(ACTOR_SHEBA, 0, 0);
		fdCameraSetSpeed(0.6);
		fdCameraMoveTo(168, CAMERA_KEEP, 721, 1);
		fdActorFaceDirection(MainRoutineStateWork.leaderActorId,
							 FACING_NORTHEAST, 0);
		fdMapGridCopyCellAttributes(38, 31, 2, 1, 25, 42);
		scIdejimaGetUp(ACTOR_KRADEN);
		fdActorSetPos(ACTOR_KRADEN, 312, 666);
		syWait(1);
		fdActorSetSpeed(ACTOR_KRADEN, 1.6);
		fdActorWalkToAndWait(ACTOR_KRADEN, 212, 680);
		fdActorWalkToAndWait(ACTOR_KRADEN, 192, 712);
		fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
		scFieldActorWaitForScriptEndById(ACTOR_KRADEN);
		fdActorRunScriptAndWait(ACTOR_KRADEN, Idejima_StretchScript);
		scFieldActorJump(ACTOR_KRADEN, 4, 20);
		/* "You weren't planning on leaving me, were you?" */
		fdEventShowMessage(ACTOR_KRADEN, 0);
		fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
		/* "Hm?" */
		fdEventShowMessage(ACTOR_KRADEN, 0);
		fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
		/* "Well! It's land!" */
		fdEventShowMessage(ACTOR_KRADEN, 0);
		fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 0);
		/* "The wave carried us back ashore!" */
		fdEventShowMessage(ACTOR_KRADEN, 0);
		fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
		/* "I assume you merely...forgot about me in your excitement?" */
		fdEventShowMessage(ACTOR_KRADEN, 0);
		fdActorSetAnimation(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
		/* "Understandable, but you should have waited for me, Felix." */
		fdEventShowMessage(ACTOR_KRADEN, 0);
		fdActorWalkToAndWait(ACTOR_KRADEN, 172, 756);
		fdActorFaceEachOther(ACTOR_KRADEN, MainRoutineStateWork.leaderActorId,
							 0);
		fdActorSetAnimation(ACTOR_KRADEN, ANIM_NOD);
		/* "Nevertheless, let us be on our way!" */
		fdEventShowMessage(
			SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
		fdActorSetAnimation(ACTOR_KRADEN, ANIM_WALK);
		leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
		if(leader != 0)
			fdActorSetDestinationById(ACTOR_KRADEN, leader->x.part[0].integer,
									  leader->z.part[0].integer);
		scFieldActorWaitForMoveById(ACTOR_KRADEN);
		fdActorSetPos(ACTOR_KRADEN, 0, 0);
		/* "Kraden has joined your party." */
		mnMessageShowCentered((s32)MsgIdejimaHeyFelix + 8, 1);
		fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_SOUTH,
							 20);
		fdEventSetFlag(FLAG_IDEJIMA_KRADEN_JOINED);
		fdEventEnd();
	}
	if(fdEventGetFlag(FLAG_IDEJIMA_JENNA_JOINED) == 0) {
		fdEventStart();
		syFieldEventEndLastingPsynergy(0);
		scIdejimaWalkLeaderBack();
		fdActorSetPos(ACTOR_JENNA, 312, 666);
		syWait(1);
		syFieldEventSetMessageId((s32)MsgIdejimaCantBelieveYouTwo);
		/* "I can't believe you two!" */
		fdEventShowMessage(ACTOR_JENNA, 0);
		fdActorSetPos(ACTOR_SHEBA, 0, 0);
		fdCameraSetSpeed(0.6);
		fdCameraMoveTo(168, CAMERA_KEEP, 721, 1);
		fdActorSetSpeed(ACTOR_KRADEN, 1.6);
		fdActorSetScript(ACTOR_KRADEN, Idejima_KradenStepsAsideScript);
		fdActorFaceDirection(MainRoutineStateWork.leaderActorId,
							 FACING_NORTHEAST, 0);
		fdMapGridCopyCellAttributes(38, 31, 2, 1, 17, 46);
		scIdejimaGetUp(ACTOR_JENNA);
		fdActorSetSpeed(ACTOR_JENNA, 1.6);
		fdActorWalkToAndWait(ACTOR_JENNA, 212, 680);
		fdActorWalkToAndWait(ACTOR_JENNA, 192, 712);
		fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 0);
		scFieldActorWaitForScriptEndById(ACTOR_JENNA);
		fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
		scFieldActorJump(ACTOR_JENNA, 4, 20);
		/* "How could you!?" */
		fdEventShowMessage(ACTOR_JENNA, 0);
		/* "Leaving me on that island while you go off exploring on your own!"
		 */
		fdEventShowMessage(ACTOR_JENNA, 0);
		fdActorSetAttachedEffect(MainRoutineStateWork.leaderActorId,
								 ATTACHED_EFFECT_IN_FRONT |
									 ATTACHED_EFFECT_ONCE);
		fdActorSetAttachedEffect(ACTOR_KRADEN, ATTACHED_EFFECT_IN_FRONT |
												   ATTACHED_EFFECT_ONCE);
		fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SCOWL, 20);
		/* "And here I was, looking everywhere for you!" */
		fdEventShowMessage(ACTOR_JENNA, 0);
		fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
		fdActorSetAnimationAndWait(MainRoutineStateWork.leaderActorId,
								   ANIM_NOD);
		fdActorSetAnimationAndWait(MainRoutineStateWork.leaderActorId,
								   ANIM_NOD);
		/* "I'm sorry, my dear... Can you ever forgive us?" */
		syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 40);
		fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
		/* "Oh... All right. Just don't leave me alone like that again..." */
		fdEventShowMessage(ACTOR_JENNA, 0);
		fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 40);
		/* "Anyway, look! That wave must have carried us to a new continent!" */
		fdEventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_JENNA, 0);
		fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
		fdActorFaceDirection(MainRoutineStateWork.leaderActorId,
							 FACING_NORTHWEST, 20);
		fdActorSetAnimation(ACTOR_KRADEN, ANIM_NOD);
		fdActorSetAnimationAndWait(MainRoutineStateWork.leaderActorId,
								   ANIM_NOD);
		fdActorWalkToAndWait(ACTOR_JENNA, 178, 728);
		fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 0);
		/* "Let's go exploring!" */
		fdEventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_JENNA, 0);
		fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 0);
		fdActorFaceDirection(MainRoutineStateWork.leaderActorId,
							 FACING_NORTHEAST, 20);
		fdActorSetAnimationAndWait(MainRoutineStateWork.leaderActorId,
								   ANIM_NOD);
		fdActorSetScript(ACTOR_KRADEN, Idejima_JoinTheLeaderScript);
		fdActorRunScriptAndWait(ACTOR_JENNA, Idejima_JoinTheLeaderScript);
		syFieldEventAddPartyMember(ACTOR_JENNA, 1);
		fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_SOUTH,
							 20);
		fdEventSetFlag(FLAG_IDEJIMA_JENNA_JOINED);
		fdEventEnd();
	}
	if(fdEventGetFlag(FLAG_IDEJIMA_SHEBA_JOINED) == 0) {
		fdEventStart();
		fdEventEndLastingPsynergy(0);
		scIdejimaWalkLeaderBack();
		fdActorSetPos(ACTOR_SHEBA, 312, 666);
		syWait(1);
		fdEventSetMessageId((s32)MsgIdejimaWaitForMe);
		/* "Wa-Wait!" */
		fdEventShowMessageAndWait(ACTOR_SHEBA, 0);
		fdCameraSetSpeed(0.6);
		fdCameraMoveTo(168, CAMERA_KEEP, 721, 1);
		fdActorSetSpeed(ACTOR_KRADEN, 1.6);
		fdActorSetScript(ACTOR_KRADEN, (s32)Idejima_KradenStepsAsideScript);
		fdActorFaceDirection(MainRoutineStateWork.leaderActorId,
							 FACING_NORTHEAST, 0);
		fdMapGridCopyCellAttributes(38, 31, 2, 1, 14, 40);
		fdActorGetById(ACTOR_SHEBA)->motionFlg =
			ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
		fdActorJump(ACTOR_SHEBA, 4, 0);
		fdActorSetAnimationById(ACTOR_SHEBA, ANIM_STAND);
		fdActorSetSprFlags(fdActorGetById(ACTOR_SHEBA), 1);
		fdEventWait(20);
		fdActorSetSpeed(ACTOR_SHEBA, 1.6);
		fdActorWalkToAndWait(ACTOR_SHEBA, 212, 680);
		fdActorWalkToAndWait(ACTOR_SHEBA, 192, 712);
		fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTHWEST, 0);
		scFieldActorWaitForScriptEndById(ACTOR_SHEBA);
		/* "Aren't you worried about me in the slightest?" */
		fdEventShowMessageAndWait(ACTOR_SHEBA, 0);
		fdActorSetAttachedEffect(MainRoutineStateWork.leaderActorId,
								 ATTACHED_EFFECT_IN_FRONT |
									 ATTACHED_EFFECT_ONCE);
		fdActorShowEmoteById(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SCOWL, 20);
		/* "Did you rescue me from the lighthouse just to leave me for dead
		 * on..." */
		fdEventShowMessageAndWait(ACTOR_SHEBA, 0);
		/* "Now, I'm sure he meant to do no such thing..." */
		fdEventShowMessageThenWait(ACTOR_KRADEN, 0, 40);
		/* "I wonder..." */
		fdEventShowMessageAndWait(ACTOR_SHEBA, 0);
		fdActorShowEmoteById(MainRoutineStateWork.leaderActorId,
							 EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 40);
		/* "Oh, don't give me that sad face... You know I can't stay mad at
		 * you..." */
		fdEventShowMessageAndWait(ACTOR_SHEBA, 0);
		fdActorSetAnimationById(ACTOR_SHEBA, ANIM_SHAKE_HEAD);
		/* "After all, you did save me, Felix. I won't soon forget that..." */
		fdEventShowMessageAndWait(ACTOR_SHEBA, 0);
		fdActorSetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
		/* "But don't leave me behind like that again, OK?" */
		fdEventShowMessageAndWait(ACTOR_SHEBA, 0);
		fdActorSetAnimationById(ACTOR_KRADEN, ANIM_NOD);
		fdActorSetAnimationAndWait(MainRoutineStateWork.leaderActorId,
								   ANIM_NOD);
		fdActorFaceDirection(ACTOR_SHEBA, FACING_WEST, 40);
		fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTHEAST, 40);
		fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTHWEST, 0);
		/* "So that big wave pushed us aground? Way to go, Nature!" */
		fdEventShowMessageAndWait(ACTOR_SHEBA, 0);
		fdActorShowEmoteById(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
		/* "What's out here, anyway?" */
		fdEventShowMessageAndWait(ACTOR_SHEBA, 0);
		fdActorSetAnimationAndWait(MainRoutineStateWork.leaderActorId,
								   ANIM_SHAKE_HEAD);
		fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
		/* "That's what we were about to find out!" */
		fdEventShowMessageAndWait(ACTOR_KRADEN, 0);
		fdActorSetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
		/* "Let's get started, then!" */
		fdEventShowMessageAndWait(ACTOR_SHEBA, 0);
		fdActorSetAnimationById(ACTOR_KRADEN, ANIM_NOD);
		fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
		fdActorSetScript(ACTOR_KRADEN, (s32)Idejima_JoinTheLeaderScript);
		fdActorRunScriptAndWait(ACTOR_SHEBA, (s32)Idejima_JoinTheLeaderScript);
		fdEventAddPartyMember(ACTOR_SHEBA, 1);
		fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_SOUTH,
							 20);
		syGameFlagSet(FLAG_IDEJIMA_SHEBA_JOINED);
		fdEventEnd();
	}
}

/* Walking on west, cells (5..6, 44..47), with all four together: Kraden */
/* wonders where Alex has gone, the party decides to follow him to the Great */
/* Western Sea, and they ask Sheba why she travels with them. */
void scIdejimaWonderWhereAlexHasGone(void) {
	const u8 *script;

	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	fdCameraSetSpeed(0.6);
	fdCameraMoveTo(108, CAMERA_KEEP, 720, 1);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.8);
	fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 86, 712);
	fdActorPlaceOnActor(ACTOR_KRADEN, MainRoutineStateWork.leaderActorId);
	syWait(1);
	fdActorSetSpeed(ACTOR_KRADEN, 0.8);
	fdActorWalkToAndWait(ACTOR_KRADEN, 104, 712);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 20);
	syFieldEventSetMessageId((s32)MsgIdejimaWhereHasAlexGone);
	/* "By the way... Where has Alex gone?" */
	fdEventShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_EAST, 40);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 20);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
	fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 104, 728);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_EAST, 20);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_SOUTHEAST,
						 20);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_EAST, 0);
	/* "He doesn't seem to be anywhere on the island." */
	fdEventShowMessage(ACTOR_KRADEN, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_NORTH, 20);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_NOD);
	/* "You don't think he set out on his own, do you?" Yes: "Then I guess
	 * there's */
	/* no reason to look for him here." No: "Well, he's no longer on Idejima, at
	 */
	/* any rate." */
	fdEventAskYesNo(ACTOR_KRADEN, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 40);
	/* "He might have gone to look for a ship." */
	fdEventShowMessage(ACTOR_KRADEN, 0);
	fdActorPlaceOnActor(ACTOR_JENNA, MainRoutineStateWork.leaderActorId);
	fdActorPlaceOnActor(ACTOR_SHEBA, MainRoutineStateWork.leaderActorId);
	syWait(1);
	fdActorSetSpeed(ACTOR_SHEBA, 0.8);
	fdActorSetSpeed(ACTOR_JENNA, 0.8);
	fdActorWalkTo(ACTOR_JENNA, 86, 712);
	fdActorWalkToAndWait(ACTOR_SHEBA, 86, 728);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 0);
	scFieldActorWaitForMoveById(ACTOR_JENNA);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_STAND);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
	/* "Alex wants a ship?" */
	fdEventShowMessage(ACTOR_JENNA, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_EAST, 40);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_NORTH, 0);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
	/* "Did you forget what he was saying?" */
	fdEventOpenMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
	if(syFieldEventAskYesNoWithActorAnimation(
		   MainRoutineStateWork.leaderActorId, 0) == 0) {
		fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
		/* "He wants to return Alchemy to its former place in the world." */
		fdEventShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
		FieldStateWork->msg += 1;
	}
	else {
		FieldStateWork->msg += 1;
		fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
		/* "Then you know he wants to restore Alchemy to the world." */
		fdEventShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
	}
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 0);
	/* "By lighting the four elemental lighthouses, he might just succeed." */
	fdEventShowMessage(ACTOR_JENNA, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
	/* "But why the ship?" */
	fdEventShowMessage(ACTOR_SHEBA, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
	/* "The remaining two lighthouses are unreachable by land." */
	fdEventShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
	fdActorShowEmote(MainRoutineStateWork.leaderActorId,
					 EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
	/* "None remain to be lit across the Eastern Sea." */
	fdEventShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	/* "So what should we°" */
	fdEventShowMessage(ACTOR_JENNA, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 20);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	/* "We must go to the Great Western Sea..." */
	fdEventShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_EAST, 20);
	/* "The Western Sea..." */
	fdEventShowMessage(ACTOR_SHEBA, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
	/* "Is that where we're going, Felix?" */
	fdEventOpenMessage(ACTOR_JENNA, 0);
	if(syFieldEventAskYesNoWithActorAnimation(ACTOR_KRADEN, 0) == 0) {
		fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 20);
		fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
		/* "Well said, Felix! And go there, we shall!" */
		fdEventShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
		FieldStateWork->msg += 1;
	}
	else {
		FieldStateWork->msg += 1;
		fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
		fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH, 20);
		/* "Don't be foolish, Felix. We must pursue Alex. We have to go
		 * there..." */
		fdEventShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
		scIdejimaCountAnswer();
	}
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	/* "That's right! Our parents' lives depend on it!" */
	fdEventShowMessage(ACTOR_JENNA, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
	/* "What about you, Sheba?" */
	fdEventShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 40);
	/* "What do you mean?" */
	fdEventShowMessage(ACTOR_SHEBA, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 20);
	/* "It's...not going to be an easy trip." */
	fdEventShowMessage(ACTOR_JENNA, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_NORTH, 20);
	/* "And there is no reason you should have to face that danger... Right..."
	 */
	fdEventOpenMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
	if(syFieldEventAskYesNoWithActorAnimation(
		   MainRoutineStateWork.leaderActorId, 0) == 0) {
		fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
		fdActorFaceDirection(ACTOR_SHEBA, FACING_EAST, 20);
		/* "Then you should know my reason for traveling with all of you,
		 * Felix..." */
		fdEventShowMessage(ACTOR_SHEBA, 0);
		FieldStateWork->msg += 1;
	}
	else {
		FieldStateWork->msg += 1;
		fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
		fdActorFaceDirection(ACTOR_SHEBA, FACING_EAST, 20);
		/* "Felix remembers... I have a reason to be traveling with you..." */
		fdEventShowMessage(ACTOR_SHEBA, 0);
	}
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	/* "What do you mean, Sheba? What reason?" */
	fdEventShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 0);
	fdActorSetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
	/* "It's my destiny..." */
	fdEventShowMessage(ACTOR_SHEBA, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	/* "This quest is your... destiny?" */
	fdEventShowMessage(ACTOR_JENNA, 0);
	scFieldActorWatchAndSetScript(MainRoutineStateWork.leaderActorId,
								  ACTOR_SHEBA);
	scFieldActorWatchAndSetScript(ACTOR_KRADEN, ACTOR_SHEBA);
	scFieldActorWatchAndSetScript(ACTOR_JENNA, ACTOR_SHEBA);
	fdActorSetSpeed(ACTOR_SHEBA, 0.4);
	fdActorWalkToAndWait(ACTOR_SHEBA, 86, 748);
	syFieldEventWait(40);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 80);
	/* "And you couldn't tell us about this earlier?" */
	fdEventShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
	scFieldActorStop(MainRoutineStateWork.leaderActorId);
	scFieldActorStop(ACTOR_KRADEN);
	scFieldActorStop(ACTOR_JENNA);
	fdActorSetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
	syFieldEventWait(40);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_SHAKE_HEAD);
	/* "But how can we trust you when we don't know why you're with us?" */
	fdEventShowMessage(ACTOR_JENNA, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
	scFieldActorWaitForScriptEndById(ACTOR_SHEBA);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_SHAKE_HEAD);
	/* "I'm sorry... But please, you have to take me! You must! You need me!" */
	fdEventShowMessage(ACTOR_SHEBA, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_NORTH, 40);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_SOUTHWEST,
						 20);
	/* "I don't understand. Why exactly do we need you?" */
	fdEventShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
	fdActorSetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
	/* "You know that I am an Adept, don't you?" */
	fdEventShowMessage(ACTOR_SHEBA, 0);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_NOD);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimationAndWait(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 0);
	/* "I can control the wind, as Felix already knows..." */
	fdEventShowMessage(ACTOR_SHEBA, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
	/* "You knew that she was a Wind Adept?" Yes: "Saturos must have mentioned
	 * it */
	/* atop the lighthouse... I see..." No: "Saturos must have known it, even */
	/* though he never told you..." */
	fdEventAskYesNo(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 40);
	/* "They saw that I was a Wind Adept right away..." */
	fdEventShowMessage(ACTOR_SHEBA, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 20);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_SHAKE_HEAD);
	/* "They kidnapped me for my powers... They needed them on their journey."
	 */
	fdEventShowMessage(ACTOR_SHEBA, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
	/* "Oh, yeah... They would have needed an alignment that complemented..." */
	fdEventShowMessage(ACTOR_JENNA, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
	/* "They said they would need a Wind Adept in order to light Jupiter's..."
	 */
	fdEventShowMessage(ACTOR_SHEBA, 0);
	fdActorSetAttachedEffect(ACTOR_KRADEN,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	/* "Ah, of course... And I suspect we'll need your power there as well." */
	fdEventShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 20);
	/* "So you see, you do need me." */
	fdEventShowMessage(ACTOR_SHEBA, 0);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	/* "All right... I guess I understand now." */
	fdEventShowMessage(ACTOR_JENNA, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
	/* "But you want to know what she meant by destiny, too, don't you?" */
	fdEventOpenMessage(ACTOR_JENNA, 0);
	if(syFieldEventAskYesNoWithActorAnimation(
		   MainRoutineStateWork.leaderActorId, 0) == 0) {
		fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
		/* "I wish we could read minds like Sheba. Then we'd know..." */
		fdEventShowMessage(ACTOR_JENNA, 0);
		FieldStateWork->msg += 1;
	}
	else {
		FieldStateWork->msg += 1;
		fdActorSetAnimation(ACTOR_JENNA, ANIM_SHAKE_HEAD);
		/* "Oh, so it's just me? Too bad I can't read minds like Sheba, huh?" */
		fdEventShowMessage(ACTOR_JENNA, 0);
	}
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 40);
	fdActorSetAnimationAndWait(ACTOR_SHEBA, ANIM_SHAKE_HEAD);
	syFieldEventWait(20);
	/* "I'm sure she will tell us in her own time. Won't you, Sheba?" */
	fdEventShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
	fdActorSetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 20);
	/* "I think, for now, that we should simply trust Sheba." */
	fdEventShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
	scFieldActorJump(ACTOR_KRADEN, 4, 20);
	/* "Now, I am quite famished. Shall we get moving?" */
	fdEventShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
	fdActorSetSpeed(ACTOR_SHEBA, 0.8);
	script = Idejima_GoAfterAlexScript;
	fdActorSetScript(ACTOR_KRADEN, script);
	fdActorSetScript(ACTOR_JENNA, script);
	fdActorRunScriptAndWait(ACTOR_SHEBA, script);
	fdEventSetFlag(FLAG_IDEJIMA_ASKED_WHERE_ALEX_WENT);
	fdEventEnd();
}

/* Starts the wave scroll from its first phase and table. */
void scIdejimaResetWaveLines(void) {
	Idejima_WaveBuffer = 0;
	Idejima_WavePhase = 0;
}

/* The sea's wave scroll: each scanline's vertical offset for BG3 from the */
/* table finished last frame, streamed by DMA 0 at every HBlank. */
void scIdejimaApplyWaveLines(void) {
	s16 *line = Idejima_WaveLines[(s16)(Idejima_WaveBuffer ^ 1)];

	syIoWrite16(line[0], &REG_BG3VOFS);
	syDMAXCopy(line + 1, (void *)&REG_BG3VOFS, DMA_HBLANK_HALFWORD_PER_LINE,
			   REG_DMA0);
}

/* Fills the other table of Idejima_WaveLines with BG3's scroll plus twice */
/* the sine plus one, 0 to 4 pixels: a wave that moves down the screen a row */
/* each frame. */
void scIdejimaUpdateWaveLines(void) {
	MAP_LAYER *layer = fdMapGridGetLayer(0);
	s16 *line = Idejima_WaveLines[Idejima_WaveBuffer];
	s32 i;

	for(i = 0; i != DISPLAY_HEIGHT; i++) {
		s32 wave = syMathSin(((u8)Idejima_WavePhase - i - FIXEDtoINT(layer->y) +
							  DISPLAY_HEIGHT)
							 << 9) +
				   FIXED_ONE;

		line[i] = FIXEDtoINT(layer->y) + FIXEDtoINT_TWICE(wave);
	}
	Idejima_WaveBuffer ^= 1;
	Idejima_WavePhase++;
}

/* Copies BG palettes 8 to 11 of the palette being faded to over palettes 0 to
 */
/* 3 and clears colour 0, for the island adrift. */
void scIdejimaUseDriftPalette(void) {
	FADE_PALETTE *palette = Ram_HeapSlots->palFade;
	u16 *to = palette->cols;
	u16 *from = palette->cols + 8 * PALETTE_COLORS;
	u32 i;

	for(i = 0; i < 4 * PALETTE_COLORS; i++)
		*to++ = *from++;
	palette->cols[0] = 0;
	fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, 0);
}

/* Readies the island adrift: the sea's layer drifts a quarter of a pixel east
 */
/* and an eighth south each frame, the beacon's cells are copied in and the */
/* ship rocks quickly. */
void scDriftScenePrepare(void) {
	MAP_LAYER *layer = fdMapGridGetLayer(0);

	layer->speedX = toFIXED(1) / 4;
	layer->speedY = toFIXED(1) / 8;
	fdMapGridCopyCellsTo(72, 10, 75, 8, 1, 1);
	fdActorSetAnimation(ACTOR_SHIP, SHIP_ANIMATION_ROCK_FAST);
}

#include "shared/emote_pop_tilted.c"

/* Jenna and Kraden reach Idejima and find Menardi's ship, which cannot sail */
/* without the orb that moved it. Alex joins them, the beacon is lit and the */
/* tremor tears Idejima away from Gondowan. */
void scIdejimaBreakAwayFromGondowan(void) {
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	syWait(1);
	fdCameraMoveTo(252, CAMERA_KEEP, 216, 0);
	syWait(1);
	fdMapGridCenterOnFollowedPoint();
	fdActorSetPos(MainRoutineStateWork.leaderActorId, 0, 0);
	fdActorSetPosAndFacing(ACTOR_JENNA, toFIXED(184), toFIXED(168),
						   FACING_SOUTH);
	{
		ACTOR *kraden = fdActorGet(ACTOR_KRADEN);

		kraden->facing = FACING_WEST;
	}
	fdActorSetPos(ACTOR_ALEX, 186, 164);
	scFieldActorSetSprCol(fdActorGet(ACTOR_ALEX), SPRITE_COLOR_HIDDEN);
	fdActorSetSprFlags(fdActorGet(ACTOR_ALEX), 0);
	syWait(1);
	fdEventStartScrTransition();
	fdCameraSetSpeed(0.6);
	fdActorSetSpeed(ACTOR_JENNA, 0.8);
	fdActorWalkToAndWait(ACTOR_JENNA, 204, 196);
	fdActorWalkToAndWait(ACTOR_JENNA, 208, 218);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 0);
	fdActorPlaceOnActor(ACTOR_KRADEN, ACTOR_JENNA);
	syWait(1);
	fdActorSetSpeed(ACTOR_KRADEN, 0.8);
	fdActorWalkToAndWait(ACTOR_KRADEN, 186, 216);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 60);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 40);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 20);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 40);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
	syFieldEventSetMessageId(MsgIdejimaIsThisThePlace);
	/* "This is the place, isn't it?" */
	fdEventOpenMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
	syFieldEventAskYesNoWithActorAnimation(ACTOR_JENNA, 0);
	fdActorWaitForAnimationChange(ACTOR_JENNA);
	/* "The peninsula at the end of the road to the west... That's what he..."
	 */
	fdEventShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 80);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
	/* "This must be Idejima!" */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0,
									40);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 40);
	/* "Hm... Menardi said a ship would be here..." */
	fdEventShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
	scFieldActorJump(ACTOR_KRADEN, 4, 0);
	/* "We ought to see if we can find it." */
	fdEventShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	fdActorSetSpeed(ACTOR_JENNA, 0.8);
	fdActorSetSpeed(ACTOR_KRADEN, 0.8);
	fdActorSetScript(ACTOR_KRADEN, Idejima_KradenToTheShipScript);
	syFieldEventWait(40);
	fdActorRunScriptAndWait(ACTOR_JENNA, Idejima_JennaToTheShipScript);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 40);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
	fdActorSetSpeed(ACTOR_KRADEN, 1.6);
	fdActorWalkToAndWait(ACTOR_KRADEN, 258, 172);
	/* "Jenna, it's over here!" */
	fdEventShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
	fdActorSetSpeed(ACTOR_JENNA, 0.4);
	fdActorWalkToAndWait(ACTOR_JENNA, 242, 172);
	fdActorWalkToAndWait(ACTOR_KRADEN, 248, 156);
	fdActorWalkToAndWait(ACTOR_KRADEN, 226, 164);
	fdActorWalkToAndWait(ACTOR_KRADEN, 228, 180);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 40);
	/* "At first glance, it seems like a normal ship, but..." */
	fdEventShowMessage(ACTOR_KRADEN, 0);
	fdActorWalkToAndWait(ACTOR_KRADEN, 226, 164);
	fdActorWalkToAndWait(ACTOR_KRADEN, 248, 156);
	fdActorWalkToAndWait(ACTOR_KRADEN, 258, 172);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 20);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
	/* "There's something odd about it..." */
	fdEventShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
	scFieldActorJump(ACTOR_KRADEN, 6, 0);
	fdActorWalkToAndWait(ACTOR_KRADEN, 256, 198);
	fdActorSetPos(ACTOR_KRADEN, 0, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 20);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
	/* "Kraden, wait! Oh, I swear! You're like a kid with a new toy!" */
	syFieldEventShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_JENNA, 0, 20);
	fdActorWalkToAndWait(ACTOR_JENNA, 224, 182);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 40);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
	fdActorWalkToAndWait(ACTOR_JENNA, 242, 172);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 40);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 120);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SCOWL, 40);
	scFieldActorJump(ACTOR_JENNA, 2, 10);
	scFieldActorJump(ACTOR_JENNA, 4, 20);
	/* "Hurry up in there!" */
	syFieldEventShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_JENNA, 0, 40);
	fdCameraSetSpeed(0.6);
	fdCameraMoveTo(262, CAMERA_KEEP, 180, 1);
	fdActorSetPos(ACTOR_KRADEN, 256, 198);
	syWait(1);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 0);
	scFieldActorJump(ACTOR_KRADEN, 6, 0);
	fdActorWalkToAndWait(ACTOR_KRADEN, 258, 172);
	syFieldEventWait(40);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
	/* "What's wrong?" */
	fdEventShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_JENNA, 0);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
	/* "It's missing..." */
	fdEventShowMessage(
		SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	/* "Hm? What are you talking about?" */
	syFieldEventShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_JENNA, 0, 20);
	/* "The thingie... that makes it go." */
	fdEventShowMessage(
		SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
	syEmotePopTilted(toFIXED(238), toFIXED(48), toFIXED(172), DEGtoANG(315));
	syFieldEventWait(10);
	syEmotePopTilted(toFIXED(246), toFIXED(48), toFIXED(172), DEGtoANG(45));
	syFieldEventWait(30);
	/* "What do you mean? What's so important about this...thingie?" */
	fdEventShowMessage(ACTOR_JENNA, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 40);
	/* "The boat can't move without it..." */
	syFieldEventShowMessageThenWait(
		SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 20);
	fdActorSetSpeed(ACTOR_JENNA, 0.8);
	fdActorWalkToAndWait(ACTOR_JENNA, 248, 148);
	/* "That Menardi! She tricked us!" */
	fdEventShowMessage(ACTOR_JENNA, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
	/* "Tricked us?" */
	fdEventShowMessage(
		SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
	/* "Yeah, by passing this dud boat off on us!" */
	syFieldEventShowMessageThenWait(ACTOR_JENNA, 0, 20);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
	/* "There's no trick. If this boat is a dud, how did it get here in the..."
	 */
	fdEventShowMessage(
		SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	/* "Oh, right." */
	fdEventShowMessage(ACTOR_JENNA, 0);
	fdActorSetSpeed(ACTOR_KRADEN, 0.8);
	fdActorWalkToAndWait(ACTOR_KRADEN, 244, 170);
	syFieldEventWait(10);
	fdActorWalkToAndWait(ACTOR_KRADEN, 258, 170);
	syFieldEventWait(10);
	fdActorWalkToAndWait(ACTOR_KRADEN, 274, 170);
	syFieldEventWait(10);
	fdActorWalkToAndWait(ACTOR_KRADEN, 258, 170);
	syFieldEventWait(10);
	fdActorWalkToAndWait(ACTOR_KRADEN, 244, 170);
	syFieldEventWait(10);
	fdActorWalkToAndWait(ACTOR_KRADEN, 258, 170);
	syFieldEventWait(10);
	fdActorWalkToAndWait(ACTOR_KRADEN, 274, 170);
	syFieldEventWait(10);
	fdActorWalkToAndWait(ACTOR_KRADEN, 258, 170);
	syFieldEventWait(20);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 40);
	/* "It must have been the orb..." */
	fdEventShowMessage(
		SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	/* "What orb?" */
	fdEventShowMessage(ACTOR_JENNA, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 20);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	/* "Don't you remember that crystal Saturos was carrying?" */
	fdEventShowMessage(
		SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	/* "That big, black pearl thing?" */
	fdEventShowMessage(ACTOR_JENNA, 0);
	/* "I'm certain he said he could move the ship with it." */
	fdEventShowMessage(
		SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
	/* "Jenna, Kraden!" */
	fdEventShowMessage(
		SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT | ACTOR_ALEX, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
	fdActorSetSprFlags(fdActorGet(ACTOR_ALEX), 1);
	scFieldActorSetSprCol(fdActorGet(ACTOR_ALEX), 0);
	syFieldEventWait(10);
	fdActorFaceDirection(ACTOR_ALEX, FACING_EAST, 40);
	fdActorSetAnimation(ACTOR_ALEX, ANIM_NOD);
	/* "There you are." */
	fdEventShowMessage(
		SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT | ACTOR_ALEX, 0);
	fdActorSetSpeed(ACTOR_ALEX, 0.8);
	fdActorWalkToAndWait(ACTOR_ALEX, 216, 164);
	fdActorWalkToAndWait(ACTOR_ALEX, 234, 160);
	syFieldEventWait(40);
	fdActorFaceDirection(ACTOR_ALEX, FACING_SOUTH_SOUTHEAST, 20);
	/* "I see you've found the boat." */
	fdEventShowMessage(ACTOR_ALEX, 0);
	fdActorFaceDirection(ACTOR_ALEX, FACING_NORTH_NORTHEAST, 0);
	/* "We set sail as soon as the beacon has been fired, correct?" */
	fdEventShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_ALEX, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 20);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	/* "Strange..." */
	fdEventShowMessage(
		SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_ALEX, FACING_EAST, 0);
	/* "What is it?" */
	fdEventShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_ALEX, 0);
	/* "The beacon... It hasn't been lit yet." */
	fdEventShowMessage(
		SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
	fdActorFaceDirection(ACTOR_ALEX, FACING_NORTH_NORTHEAST, 40);
	/* "You're right... Why is it taking so long?" */
	fdEventShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_ALEX, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 0);
	/* "And why hasn't my brother returned?" */
	fdEventShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 80);
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	syFieldEventSetExitRequest(IDEJIMA_TO_WORLD_MAP_NORTH_A);
}

/* Adrift, Alex leads Jenna and Kraden to Felix and Sheba, washed up on the */
/* island. */
void scIdejimaFindFelixAndSheba(void) {
	ACTOR *actor = fdActorGet(ACTOR_JENNA);

	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	scIdejimaUseDriftPalette();
	scDriftScenePrepare();
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdActorSetPos(ACTOR_SHEBA, 444, 212);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_COLLAPSE);
	fdActorSetSprFlags(fdActorGet(ACTOR_SHEBA), 0);
	fdActorSetPos(MainRoutineStateWork.leaderActorId, 432, 234);
	fdActorGet(MainRoutineStateWork.leaderActorId)->scaleX = -FIXED_ONE;
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId,
						ANIM_LEADER_LIE_ON_BACK);
	fdActorSetSprFlags(fdActorGet(MainRoutineStateWork.leaderActorId), 0);
	fdActorSetPos(ACTOR_ALEX, 392, 172);
	syWait(1);
	fdCameraMoveTo(192, CAMERA_KEEP, 214, 0);
	fdMapGridCenterOnFollowedPoint();
	syWait(1);
	fdActorSetPos(ACTOR_JENNA, 208, 218);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_COLLAPSE);
	fdActorSetSprFlags(fdActorGet(ACTOR_JENNA), 0);
	fdActorSetPos(ACTOR_KRADEN, 176, 204);
	fdActorSetAnimation(ACTOR_KRADEN, KRADEN_ANIMATION_ASLEEP);
	fdActorSetSprFlags(fdActorGet(ACTOR_KRADEN), 0);
	syWait(1);
	fdEventStartScrTransition();
	fdEventWaitTransitionFrames();
	syFieldEventWait(120);
	syFieldEventSetMessageId((s32)MsgIdejimaHowLongWillWeDrift);
	/* "Oh, Kraden... How long will this island continue to drift?" */
	syFieldEventShowMessageThenWait(ACTOR_JENNA, 0, 40);
	/* "I don't know... I wish you could tell me." */
	fdEventShowMessage(ACTOR_KRADEN, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
	/* "Sheesh... You're no help in times like this." */
	fdEventShowMessage(ACTOR_JENNA, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
	/* "Hey, I'm hungry..." */
	fdEventShowMessage(ACTOR_KRADEN, 0);
	/* "How can you even think about food?" */
	fdEventShowMessage(ACTOR_JENNA, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 80);
	/* "I wonder what happened to my brother..." */
	fdEventShowMessage(ACTOR_JENNA, 0);
	/* "I can't be certain, but it looks like they lit the Venus beacon." */
	fdEventShowMessage(ACTOR_KRADEN, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	/* "Yeah, I know. You don't have to tell me that. That's how we wound up..."
	 */
	fdEventShowMessage(ACTOR_JENNA, 0);
	fdActorSetAttachedEffect(ACTOR_KRADEN,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldEventWait(40);
	/* "My, aren't you cranky today, Jenna!" */
	fdEventShowMessage(ACTOR_KRADEN, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
	/* "Well, I suppose you're just worried about Felix." */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 40);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
	/* "Isaac and the others must have been at the lighthouse, too." */
	syFieldEventShowMessageThenWait(ACTOR_JENNA, 0, 20);
	/* "I suspect you're right..." */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 40);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
	/* "I miss them... Isaac... Felix..." */
	fdEventShowMessage(ACTOR_JENNA, 0);
	fdActorSetSpeed(ACTOR_ALEX, 0.8);
	fdActorWalkToAndWait(ACTOR_ALEX, 304, 172);
	fdActorWalkToAndWait(ACTOR_ALEX, 224, 172);
	fdActorWalkToAndWait(ACTOR_ALEX, 208, 192);
	/* "Come with me." */
	fdEventShowMessage(ACTOR_ALEX, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
	/* "There is something I wish to show you." */
	fdEventShowMessage(ACTOR_ALEX, 0);
	fdActorSetScript(ACTOR_ALEX, Idejima_AlexLeadsTheWayScript);
	syFieldEventWait(40);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_STAND);
	fdActorSetSprFlags(fdActorGet(ACTOR_KRADEN), 1);
	scFieldActorJump(ACTOR_KRADEN, 4, 20);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_STAND);
	fdActorSetSprFlags(fdActorGet(ACTOR_JENNA), 1);
	scFieldActorJump(ACTOR_JENNA, 4, 20);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 40);
	fdActorSetSpeed(ACTOR_KRADEN, 0.8);
	fdActorSetSpeed(ACTOR_JENNA, 0.8);
	fdActorSetScript(ACTOR_KRADEN, Idejima_KradenFollowsAlexScript);
	syFieldEventWait(40);
	fdCameraSetSpeed(0.8);
	fdCameraMoveTo(368, CAMERA_KEEP, 210, 1);
	actor->tmr = 0;
	fdActorSetScript(ACTOR_JENNA, Idejima_JennaFollowsAlexScript);
	do {
		syWait(1);
	} while(actor->tmr == 0);
	syFieldEventWait(10);
	fdActorSetAttachedEffect(ACTOR_KRADEN,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(ACTOR_JENNA,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	/* "Felix... Sheba..." */
	/* "Sheba! Felix!" */
	syFieldEventShowTwoMessagesAndWait(ACTOR_JENNA, 7, 13, 2, 12, ACTOR_KRADEN,
									   9, 2, 4, 1, 0);
	fdActorFaceDirection(ACTOR_ALEX, FACING_SOUTH_SOUTHEAST, 0);
	fdActorSetSpeed(ACTOR_KRADEN, 1.6);
	fdActorSetSpeed(ACTOR_JENNA, 1.6);
	fdActorSetScript(ACTOR_KRADEN, Idejima_KradenRunsToFelixScript);
	fdActorRunScriptAndWait(ACTOR_JENNA, Idejima_JennaRunsToFelixScript);
	syFieldEventWait(40);
	/* "They... They're alive..." */
	fdEventShowMessage(
		SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
	scFieldActorJump(ACTOR_JENNA, 4, 10);
	/* "They're really alive!" */
	fdEventShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
	/* "What is this? Alex, what happened?" */
	syFieldEventShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 0, 40);
	FieldStateWork->transitionFrm = 120;
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	syFieldEventSetExitRequest(IDEJIMA_TO_TITLE);
}

/* Sheba wakes and tells what happened at the lighthouse, a new continent comes
 */
/* in sight, and a tidal wave sweeps the island towards it. */
void scIdejimaRideTheTidalWave(void) {
	ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);

	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	scDriftScenePrepare();
	fdActorSetPosAndFacing(ACTOR_KRADEN, toFIXED(314), toFIXED(200),
						   FACING_SOUTH_SOUTHEAST);
	fdActorSetPosAndFacing(ACTOR_ALEX, toFIXED(328), toFIXED(240),
						   FACING_NORTH_NORTHEAST);
	fdActorSetPosAndFacing(ACTOR_JENNA, toFIXED(340), toFIXED(200),
						   FACING_SOUTH);
	fdActorSetPosAndFacing(ACTOR_SHEBA, toFIXED(342), toFIXED(230),
						   FACING_EAST);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_COLLAPSE);
	fdActorSetSprFlags(fdActorGet(ACTOR_SHEBA), 0);
	fdActorSetPosAndFacing(MainRoutineStateWork.leaderActorId, toFIXED(312),
						   toFIXED(230), FACING_WEST);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId,
						ANIM_LEADER_LIE_ON_BACK);
	fdActorSetSprFlags(fdActorGet(MainRoutineStateWork.leaderActorId), 0);
	syWait(1);
	fdEventStartScrTransition();
	fdEventWaitTransitionFrames();
	syFieldEventWait(20);
	syFieldEventSetMessageId(MsgIdejimaThoughtTheyWereInsideLighthouse);
	/* "I thought Felix and Sheba were still inside the lighthouse." */
	fdEventShowMessage(ACTOR_KRADEN, 0);
	fdActorFaceDirection(ACTOR_ALEX, FACING_NORTH_NORTHWEST, 0);
	/* "It's miraculous..." */
	fdEventShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_SHAKE_HEAD);
	/* "Miracle or no, I'm just happy my brother's alive." */
	fdEventShowMessage(ACTOR_JENNA, 0);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	fdActorRunRepeatedMotion(ACTOR_SHEBA, 2);
	fdActorFaceDirection(ACTOR_ALEX, FACING_NORTH_NORTHEAST, 0);
	fdActorShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 80);
	fdActorFaceDirection(ACTOR_ALEX, FACING_NORTH_NORTHWEST, 0);
	/* "What happened back there?" */
	/* "..." */
	syFieldEventShowTwoMessagesAndWait(ACTOR_KRADEN, 9, 1, 7, 6, ACTOR_ALEX, 12,
									   16, 18, 11, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	fdActorRunRepeatedMotion(ACTOR_SHEBA, 2);
	fdActorSetAttachedEffect(ACTOR_JENNA,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	/* "Sheba..." */
	syFieldEventShowMessageThenWait(ACTOR_JENNA, 0, 20);
	fdActorRunRepeatedMotion(ACTOR_SHEBA, 1);
	syFieldEventWait(40);
	/* "Oh..." */
	fdEventShowMessage(ACTOR_SHEBA, 0);
	/* "Thank the elements, she's awake!" */
	fdEventShowMessage(ACTOR_KRADEN, 0);
	/* "Sheba, are you all right? It's me, Jenna." */
	fdEventShowMessage(ACTOR_JENNA, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	/* "Jenna?" */
	fdEventShowMessage(ACTOR_SHEBA, 0);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_SHEBA_SIT_UP);
	syFieldEventWait(40);
	/* "What...happened?" */
	fdEventShowMessage(ACTOR_SHEBA, 0);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_NOD);
	/* "It seems you drifted here with Felix." */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 40);
	/* "With Felix...?" */
	syFieldEventShowMessageThenWait(ACTOR_SHEBA, 0, 40);
	fdActorGet(ACTOR_SHEBA)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkTo(ACTOR_SHEBA, 340, 226);
	scFieldActorJump(ACTOR_SHEBA, 4, 0);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_STAND);
	fdActorSetSprFlags(fdActorGet(ACTOR_SHEBA), 1);
	syFieldEventWait(20);
	fdActorSetAttachedEffect(ACTOR_JENNA,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldEventWait(20);
	/* "Take your time... Do you feel all right, Sheba?" */
	fdEventShowMessage(ACTOR_JENNA, 0);
	fdActorGet(ACTOR_SHEBA)->steeringFlg |= ACTOR_FACES_MOTION;
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
	scFieldActorWaitForScriptEndById(ACTOR_SHEBA);
	fdActorSetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
	syFieldEventWait(40);
	scFieldActorWatchAndSetScript(ACTOR_KRADEN, ACTOR_SHEBA);
	scFieldActorWatchAndSetScript(ACTOR_JENNA, ACTOR_SHEBA);
	scFieldActorWatchAndSetScript(ACTOR_ALEX, ACTOR_SHEBA);
	fdActorSetSpeed(ACTOR_SHEBA, 0.4);
	fdActorWalkToAndWait(ACTOR_SHEBA, 358, 222);
	syFieldEventWait(40);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTHEAST, 80);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 40);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
	/* "Where are we?" */
	syFieldEventShowMessageThenWait(ACTOR_SHEBA, 0, 20);
	/* "Idejima. We were all to meet here... But now, the island is floating..."
	 */
	fdEventShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_ALEX, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 40);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTHEAST, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTHWEST, 0);
	/* "This island is moving?" */
	fdEventShowMessage(ACTOR_SHEBA, 0);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
	/* "I know it must be very hard to believe, Sheba." */
	fdEventShowMessage(ACTOR_KRADEN, 0);
	scFieldActorJump(ACTOR_JENNA, 4, 0);
	/* "When the Venus Lighthouse was lit, a massive tremor tore us off the..."
	 */
	fdEventShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_WEST, 20);
	fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
	/* "The ground beneath the lighthouse roiled, as though it might crumble..."
	 */
	fdEventShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_ALEX, 0);
	fdActorSetSpeed(ACTOR_SHEBA, 1.6);
	fdActorWalkToAndWait(ACTOR_SHEBA, 348, 222);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTHWEST, 0);
	/* "I remember now... And that's what carried us away from Gondowan." */
	fdEventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_SHEBA, 0);
	scFieldActorStop(ACTOR_KRADEN);
	scFieldActorStop(ACTOR_JENNA);
	scFieldActorStop(ACTOR_ALEX);
	fdActorSetSpeed(ACTOR_KRADEN, 0.4);
	fdActorWalkToAndWait(ACTOR_KRADEN, 320, 200);
	/* "But what happened to you, Sheba? You were in the sea, and Saturos°" */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 40);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_WEST, 40);
	/* "Saturos and Menardi are gone..." */
	fdEventShowMessage(ACTOR_SHEBA, 0);
	fdActorShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	/* "What do you mean?" */
	fdEventShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_ALEX, 0);
	fdActorSetAnimationAndWait(ACTOR_SHEBA, ANIM_SHAKE_HEAD);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTHWEST, 0);
	/* "Another group came... They fought Saturos and Menardi and won." */
	fdEventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_SHEBA, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
	/* "Was it Isaac?" */
	fdEventShowMessage(ACTOR_JENNA, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
	/* "Isaac... Yes, I think that's what they called him..." */
	fdEventShowMessage(ACTOR_SHEBA, 0);
	/* "You expect me to believe Isaac and his companions defeated Saturos..."
	 */
	fdEventShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_ALEX, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTHWEST, 0);
	scFieldActorWaitForScriptEndById(ACTOR_SHEBA);
	fdActorSetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
	syFieldEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
	/* "Have they really grown so powerful in so short a time?" */
	fdEventShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_ALEX, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST, 0);
	/* "But how did you wind up in the sea?" */
	fdEventShowMessage(ACTOR_KRADEN, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
	scFieldActorWaitForScriptEndById(ACTOR_SHEBA);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_SHAKE_HEAD);
	/* "When the beacon was lit, the earthquake knocked me off the
	 * lighthouse..." */
	syFieldEventShowMessageThenWait(ACTOR_SHEBA, 0, 20);
	fdActorRunRepeatedMotion(MainRoutineStateWork.leaderActorId, 2);
	syFieldEventWait(20);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
	/* "Then what's my brother doing here?" */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0, 20);
	fdActorShowEmote(MainRoutineStateWork.leaderActorId,
					 EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
	scFieldActorWaitForScriptEndById(ACTOR_SHEBA);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
	/* "He tried to save me from drowning..." */
	fdEventShowMessage(ACTOR_SHEBA, 0);
	fdActorShowEmote(MainRoutineStateWork.leaderActorId,
					 EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	fdActorRunRepeatedMotion(MainRoutineStateWork.leaderActorId, 2);
	fdActorSetAttachedEffect(ACTOR_KRADEN,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	/* "Felix jumped from the top of the lighthouse!? Dear me!" */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 20);
	scFieldActorStartRepeatedMotion(MainRoutineStateWork.leaderActorId, 3);
	fdActorShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	fdActorFaceDirection(ACTOR_ALEX, FACING_NORTH_NORTHWEST, 20);
	/* "Felix... are you awake?" */
	fdEventShowMessage(ACTOR_ALEX, 0);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_LEADER_SIT_UP);
	syFieldEventWait(20);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_WEST, 20);
	/* "Ah! Felix, you're awake!" */
	fdEventShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 40);
	/* "Brother..." */
	fdEventShowMessage(ACTOR_JENNA, 0);
	fdActorGet(MainRoutineStateWork.leaderActorId)->steeringFlg &=
		~ACTOR_FACES_MOTION;
	fdActorWalkTo(MainRoutineStateWork.leaderActorId, 314, 226);
	scFieldActorJump(MainRoutineStateWork.leaderActorId, 4, 0);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_STAND);
	fdActorSetSprFlags(fdActorGet(MainRoutineStateWork.leaderActorId), 1);
	leader->scaleX = FIXED_ONE;
	syFieldEventWait(20);
	fdActorSetAttachedEffect(ACTOR_JENNA,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	/* "Are you sure you should be standing?" */
	fdEventShowMessage(ACTOR_JENNA, 0);
	fdActorGet(MainRoutineStateWork.leaderActorId)->steeringFlg |=
		ACTOR_FACES_MOTION;
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_SOUTHWEST,
						 40);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_SOUTHEAST,
						 40);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_NORTHWEST,
						 80);
	/* "You don't seem very surprised to find yourself on a floating island." */
	fdEventShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
	fdActorShowEmote(MainRoutineStateWork.leaderActorId,
					 EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_EAST, 20);
	fdActorFaceDirection(ACTOR_ALEX, FACING_NORTH_NORTHWEST, 0);
	/* "Felix, once you'd saved Sheba, you must have swum out here, correct?" */
	fdEventShowMessage(ACTOR_ALEX, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_SOUTHEAST,
						 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 0);
	fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
	/* "You must have seen that this island was floating when you were
	 * swimming." */
	fdEventShowMessage(ACTOR_ALEX, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 40);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
	/* "But what do we do now?" */
	fdEventShowMessage(ACTOR_JENNA, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 20);
	/* "I have no idea. Nobody knows what lies beyond the Eastern Sea." */
	fdEventShowMessage(ACTOR_SHEBA, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
	/* "Unfortunately, I am a student of Alchemy, not geography..." */
	fdEventShowMessage(ACTOR_KRADEN, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_WEST, 20);
	fdActorFaceDirection(ACTOR_ALEX, FACING_EAST, 0);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_NOD);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
	fdActorSetSpeed(ACTOR_ALEX, 1.6);
	fdActorWalkToAndWait(ACTOR_ALEX, 368, 256);
	fdActorFaceDirection(ACTOR_ALEX, FACING_EAST, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTHEAST, 0);
	scFieldActorWaitForScriptEndById(ACTOR_SHEBA);
	/* "What is it...Alex?" */
	fdEventShowMessage(
		SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_LEFT | ACTOR_SHEBA, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_SOUTHEAST,
						 0);
	fdActorWalkToAndWait(ACTOR_ALEX, 400, 264);
	fdActorFaceDirection(ACTOR_ALEX, FACING_EAST, 0);
	/* "What's the matter?" */
	fdEventShowMessage(ACTOR_KRADEN, 0);
	fdActorShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
	/* "Can't you see it?" */
	fdEventShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_ALEX, 0);
	fdCameraSetSpeed(0.6);
	fdCameraMoveTo(396, CAMERA_KEEP, 234, 1);
	fdActorSetSpeed(ACTOR_JENNA, 0.8);
	fdActorSetSpeed(ACTOR_SHEBA, 0.8);
	fdActorSetSpeed(ACTOR_KRADEN, 0.8);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.8);
	leader->tmr = 0;
	fdActorSetScript(ACTOR_KRADEN, Idejima_KradenToTheShoreScript);
	fdActorSetScript(ACTOR_JENNA, Idejima_JennaToTheShoreScript);
	fdActorSetScript(MainRoutineStateWork.leaderActorId,
					 Idejima_FelixToTheShoreScript);
	fdActorSetScript(ACTOR_SHEBA, Idejima_ShebaToTheShoreScript);
	do
		syWait(1);
	while(leader->tmr == 0);
	fdCameraWaitForArrival();
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	/* "It's land! An island!" */
	fdEventShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0);
	/* "It's a little... big for an island." */
	fdEventShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_SHEBA, 0);
	/* "That's no island... It's a new continent!" */
	fdEventShowMessage(ACTOR_KRADEN, 0);
	syFieldEventSetExitRequest(IDEJIMA_TO_WORLD_MAP_NORTHEAST);
}

/* Begins a step of Felix's waking: empty. */
void scFelixWakeStartStep(void) {}

/* Ends a step of Felix's waking: empty. */
void scFelixWakeEndStep(void) {}

/* Waits for a button, or 3 seconds. */
void scFelixWakeWaitForButton(void) {
	s32 frames = 180;

	scFelixWakeStartStep();
	while(--frames != -1) {
		syWait(1);
		if(InputWork.pressed != 0)
			break;
	}
	scFelixWakeEndStep();
}

/* Felix wakes on the grass, stirring a little more at each button, gets up */
/* and is asked "Do you want to check yourself for injuries?"; Yes has him */
/* check his arms and then his legs, No has him walk about a bit. */
void scIdejimaFelixWakes(void) {
	s32 answer;
	s32 leader;

	scFelixWakeStartStep();
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	syWait(1);
	fdCameraMoveTo(384, CAMERA_KEEP, 728, 0);
	syWait(1);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId,
						ANIM_LEADER_LIE_ON_BACK);
	syWait(1);
	fdActorSetSprFlags(fdActorGet(MainRoutineStateWork.leaderActorId), 0);
	syWait(1);
	fdActorSetPos(MainRoutineStateWork.leaderActorId, 384, 728);
	fdActorSetPos(ACTOR_NARRATOR, 384, 702);
	syWait(1);
	fdMapGridCenterOnFollowedPoint();
	syWait(1);
	scFelixWakeEndStep();
	fdEventStartScrTransition();
	fdEventWaitTransitionFrames();
	syWait(10);
	scFelixWakeWaitForButton();
	scFelixWakeStartStep();
	fdActorRunRepeatedMotion(MainRoutineStateWork.leaderActorId, 2);
	syWait(1);
	scFelixWakeEndStep();
	scFelixWakeWaitForButton();
	scFelixWakeStartStep();
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_LEADER_SIT_UP);
	syWait(1);
	scFelixWakeEndStep();
	scFelixWakeWaitForButton();
	scFelixWakeStartStep();
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_STAND);
	fdActorSetSprFlags(fdActorGet(MainRoutineStateWork.leaderActorId), 1);
	syWait(1);
	scFieldActorJump(MainRoutineStateWork.leaderActorId, 4, 20);
	syFieldEventSetMessageId((s32)MsgIdejimaCheckForInjuries);
	/* "Do you want to check yourself for injuries?" */
	fdEventOpenMessage(ACTOR_NARRATOR, 0);
	syWait(1);
	answer = syFieldEventAskYesNoWithActorAnimation(
		MainRoutineStateWork.leaderActorId, 0);
	leader = MainRoutineStateWork.leaderActorId;
	if(answer == 0) {
		fdActorWaitForAnimationChange(leader);
		fdActorSetAnimation(MainRoutineStateWork.leaderActorId,
							ANIM_LEADER_SWING_RIGHT_ARM);
		syWait(1);
		scFelixWakeEndStep();
		syFieldEventWait(50);
		scFelixWakeStartStep();
		fdActorSetAnimation(MainRoutineStateWork.leaderActorId,
							ANIM_LEADER_SWING_LEFT_ARM);
		syWait(1);
		scFelixWakeEndStep();
		syFieldEventWait(50);
		scFelixWakeStartStep();
		/* "Looks like the arms are still attached. That's a good start." */
		fdEventShowMessage(ACTOR_NARRATOR, 0);
		syWait(1);
		fdActorSetAnimationAndWait(MainRoutineStateWork.leaderActorId,
								   ANIM_NOD);
		scFelixWakeEndStep();
		syFieldEventWait(20);
		scFelixWakeStartStep();
		fdActorSetAnimation(MainRoutineStateWork.leaderActorId,
							ANIM_LEADER_KICK_RIGHT_LEG);
		syWait(1);
		scFelixWakeEndStep();
		syFieldEventWait(50);
		scFelixWakeStartStep();
		fdActorSetAnimation(MainRoutineStateWork.leaderActorId,
							ANIM_LEADER_KICK_LEFT_LEG);
		syWait(1);
		scFelixWakeEndStep();
		syFieldEventWait(50);
		scFelixWakeStartStep();
		/* "The legs are working fine, too. Yup. You're fine!" */
		fdEventShowMessage(ACTOR_NARRATOR, 0);
		syWait(1);
		fdActorSetAnimationAndWait(MainRoutineStateWork.leaderActorId,
								   ANIM_NOD);
	}
	else {
		fdActorWaitForAnimationChange(leader);
		FieldStateWork->msg += 2;
		fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.4);
		fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 384, 746);
		scFelixWakeEndStep();
		syFieldEventWait(20);
		scFelixWakeStartStep();
		/* "You walk around a bit, and everything seems fine." */
		fdEventShowMessage(ACTOR_NARRATOR, 0);
		fdActorSetAnimationAndWait(MainRoutineStateWork.leaderActorId,
								   ANIM_NOD);
	}
	fdActorSetPos(ACTOR_NARRATOR, 0, 0);
	fdEventClearFlag(FLAG_SHOW_AREA_NAME);
	fdEventSetFlag(FLAG_IDEJIMA_FELIX_WOKE);
	fdEventEnd();
	scFelixWakeEndStep();
}

/* Starts the sea's ripple, then runs what the entrance taken calls for: the */
/* island run aground (after the tidal wave, or back from the world map, both */
/* making Daila's sanctum the place a defeat returns the party to), or one of */
/* the story's three cutscenes before it. */
s32 scInit(void) {
	s32 entrance;

	scIdejimaResetWaveLines();
	syTaskAddFrame(scIdejimaApplyWaveLines, TASK_PASS_VBLANK);
	syTaskAddFrame(scIdejimaUpdateWaveLines, TASK_PASS_FRAME);
	{
		MAIN_ROUTINE_STATE *party = &MainRoutineStateWork;

		entrance = party->entrance;
	}
	switch(entrance) {
	case ENTRANCE_WAKE:
		if(fdEventGetFlag(FLAG_KEEP_PARTY_POSITION) == 0)
			gmPartyRestoreHpPp(1);
	case ENTRANCE_BACK_FROM_WORLD_MAP: {
		MAIN_ROUTINE_STATE *party = &MainRoutineStateWork;

		party->restartScene = SCENE_DAILA_ROOMS_SANCTUM;
		party->restartEntrance = 1;
	}
		scIdejimaLayDownTheOthers();
		break;
	case ENTRANCE_BREAK_AWAY:
		scIdejimaBreakAwayFromGondowan();
		break;
	case ENTRANCE_FIND_FELIX_AND_SHEBA:
		scIdejimaFindFelixAndSheba();
		break;
	case IDEJIMA_RIDE_THE_TIDAL_WAVE:
		scIdejimaRideTheTidalWave();
		break;
	}
	return 0;
}

s32 scPrepareMap(void) { return 0; }

/* Unused: lays an actor down, the reverse of scIdejimaGetUp. */
UNUSED void scIdejimaLieDown(s32 id) {
	ACTOR *actor = fdActorGet(id);

	fdActorSetSprFlags(fdActorGet(id), 0);
	actor->facing = FACING_SOUTH;
	actor->motionFlg = 0;
	actor->x.fixed += toFIXED(12);
	actor->y.fixed = toFIXED(16);
	actor->collisionFlg = 0;
	((FIELD_SPR *)actor->spr)->rotation = DEGtoANG(270);
}

/* Lays each of Kraden, Jenna and Sheba who has not yet joined asleep on the */
/* grass where the wave left them, and wakes Felix the first time. */
void scIdejimaLayDownTheOthers(void) {
	s32 flag;

	if(!fdEventGetFlag(FLAG_IDEJIMA_KRADEN_JOINED)) {
		ACTOR *actor;

		fdActorSetPos(ACTOR_KRADEN, 416, 680);
		fdMapGridCopyCellAttributes(38, 32, 2, 1, 25, 42);
		actor = fdActorGet(ACTOR_KRADEN);
		fdActorSetSprFlags(fdActorGet(ACTOR_KRADEN), 0);
		actor->facing = FACING_SOUTH;
		actor->motionFlg = 0;
		actor->y.fixed = toFIXED(12);
		actor->collisionFlg = 0;
		fdActorSetAnimation(ACTOR_KRADEN, KRADEN_ANIMATION_ASLEEP);
	}
	if(!fdEventGetFlag(FLAG_IDEJIMA_JENNA_JOINED)) {
		ACTOR *actor;

		fdActorSetPos(ACTOR_JENNA, 288, 744);
		fdMapGridCopyCellAttributes(38, 33, 2, 1, 17, 46);
		actor = fdActorGet(ACTOR_JENNA);
		fdActorSetSprFlags(fdActorGet(ACTOR_JENNA), 0);
		actor->facing = FACING_SOUTH;
		actor->motionFlg = 0;
		actor->y.fixed = toFIXED(12);
		actor->collisionFlg = 0;
		fdActorSetAnimation(ACTOR_JENNA, ANIM_JENNA_LIE_ON_SIDE);
	}
	flag = fdEventGetFlag(FLAG_IDEJIMA_SHEBA_JOINED);
	if(flag == 0) {
		ACTOR *actor;

		actor = fdActorGet(ACTOR_SHEBA);
		fdActorSetPos(ACTOR_SHEBA, 240, 648);
		fdMapGridCopyCellAttributes(38, 34, 2, 1, 14, 40);
		fdActorSetAnimation(ACTOR_SHEBA, ANIM_COLLAPSE);
		fdActorSetSprFlags(fdActorGet(ACTOR_SHEBA), 0);
		actor->motionFlg = 0;
		actor->y.fixed = toFIXED(12);
	}
	fdMapGridCopyCellsTo(72, 42, 75, 40, 1, 1);
	syWait(1);
	if(!fdEventGetFlag(FLAG_IDEJIMA_FELIX_WOKE))
		scIdejimaFelixWakes();
}

u32 Idejima_KradenToTheShipScript[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(216, 0, 172),
	SC_SYNC,
	SC_MOVE(264, 0, 164),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

u32 Idejima_JennaToTheShipScript[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(216, 0, 182),
	SC_SYNC,
	SC_MOVE(242, 0, 164),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

u32 Idejima_AlexLeadsTheWayScript[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(224, 0, 172),
	SC_SYNC,
	SC_MOVE(392, 0, 172),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_WAIT(1),
	SC_SET(SW_FACING, FACING_WEST),
	SC_WAIT(1),
	SC_END,
};

u32 Idejima_KradenFollowsAlexScript[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(224, 0, 172),
	SC_SYNC,
	SC_MOVE(304, 0, 172),
	SC_SYNC,
	SC_MOVE(336, 0, 192),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_WAIT(1),
	SC_SET(SW_FACING, FACING_EAST),
	SC_WAIT(1),
	SC_END,
};

u32 Idejima_JennaFollowsAlexScript[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(216, 0, 172),
	SC_SYNC,
	SC_MOVE(296, 0, 172),
	SC_SYNC,
	SC_MOVE(316, 0, 208),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_WAIT(1),
	SC_SET(SW_FACING, FACING_EAST),
	SC_WAIT(1),
	SC_SET(SW_TIMER, 1),
	SC_END,
};

u32 Idejima_KradenRunsToFelixScript[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(420, 0, 200),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

u32 Idejima_JennaRunsToFelixScript[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(408, 0, 232),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

u32 Idejima_JennaToTheShoreScript[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(424, 0, 200),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_WAIT(1),
	SC_SET(SW_FACING, FACING_EAST),
	SC_WAIT(1),
	SC_END,
};

u32 Idejima_ShebaToTheShoreScript[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(408, 0, 228),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_WAIT(1),
	SC_SET(SW_FACING, FACING_EAST),
	SC_WAIT(1),
	SC_END,
};

u32 Idejima_KradenToTheShoreScript[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(400, 0, 214),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_WAIT(1),
	SC_SET(SW_FACING, FACING_EAST),
	SC_WAIT(1),
	SC_END,
};

u32 Idejima_FelixToTheShoreScript[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(412, 0, 248),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_WAIT(1),
	SC_SET(SW_FACING, FACING_EAST),
	SC_WAIT(1),
	SC_SET(SW_TIMER, 1),
	SC_END,
};

s32 Idejima_StretchScript[] = {
	SC_LABEL(1),
	SC_ADD(SW_SCALE_X, 192),
	SC_ADD(SW_SCALE_Y, 384),
	SC_WAIT(1),
	SC_LOOP(24, SC_TO(1)),
	SC_LABEL(2),
	SC_ADD(SW_SCALE_X, -384),
	SC_ADD(SW_SCALE_Y, -768),
	SC_WAIT(1),
	SC_LOOP(12, SC_TO(2)),
	SC_LABEL(3),
	SC_ADD(SW_SCALE_X, 192),
	SC_ADD(SW_SCALE_Y, 384),
	SC_WAIT(1),
	SC_LOOP(24, SC_TO(3)),
	SC_LABEL(4),
	SC_ADD(SW_SCALE_X, -384),
	SC_ADD(SW_SCALE_Y, -768),
	SC_WAIT(1),
	SC_LOOP(12, SC_TO(4)),
	SC_SET(SW_SCALE_X, FIXED_ONE),
	SC_SET(SW_SCALE_Y, FIXED_ONE),
	SC_WAIT(1),
	SC_END,
};

u32 Idejima_KradenStepsAsideScript[] = {
	SC_POSITION(152, 0, 756),
	SC_WAIT(1),
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(146, 0, 736),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_WAIT(1),
	SC_SET(SW_FACING, FACING_NORTHEAST),
	SC_WAIT(1),
	SC_END,
};

u32 Idejima_JoinTheLeaderScript[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(152, 0, 756),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_POSITION_ORIGIN,
	SC_WAIT(1),
	SC_END,
};

u32 Idejima_GoAfterAlexScript[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(104, 0, 728),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_POSITION_ORIGIN,
	SC_WAIT(1),
	SC_END,
};

#include "shared/scene_entrances_default.c"

/* clang-format off */
u32 IdejimaExits[] = {
    SCENE_EXITS(Idejima),
    SCENE_EXIT(EXIT_DOOR, Idejima, ENTRANCE_FROM_DOOR_AGAIN),
    SCENE_EXIT(IDEJIMA_TO_WORLD_MAP_SOUTHWEST, WorldMap, WORLD_MAP_FROM_IDEJIMA_SOUTHWEST),
    SCENE_EXIT(IDEJIMA_TO_WORLD_MAP_NORTH_A, WorldMap, WORLD_MAP_VENUS_COLLAPSE),
    SCENE_EXIT(IDEJIMA_TO_TITLE, Title, TITLE_FROM_IDEJIMA),
    SCENE_EXIT(IDEJIMA_TO_WORLD_MAP_NORTHEAST, WorldMap, WORLD_MAP_DRIFT_NORTH),
    SCENE_EXITS_END,
};

ACTOR_PLACE IdejimaPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MENARDIS_SHIP, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 256, 0, 200, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ALEX, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};
/* clang-format on */

TRIGGER IdejimaEvents[] = {
	{TRIGGER_TABLE_END},
};

TRIGGER IdejimaEventsFromDoor[] = {
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_DOOR,
	 CONDITION_ALWAYS, EVENT(scIdejimaEnterDoor)},
	{TRIGGER_TABLE_END},
};

TRIGGER IdejimaEventsWake[] = {
	{TRIGGER_EXIT, IDEJIMA_TO_WORLD_MAP_SOUTHWEST, CONDITION_ALWAYS,
	 IDEJIMA_TO_WORLD_MAP_SOUTHWEST},
	{TRIGGER_EXAMINE, CELL_BEHAVIOR_BY_KRADEN, FLAG_IDEJIMA_KRADEN_JOINED,
	 EVENT(scIdejimaWakeKraden)},
	{TRIGGER_EXAMINE, CELL_BEHAVIOR_BY_JENNA, FLAG_IDEJIMA_JENNA_JOINED,
	 EVENT(scIdejimaWakeJenna)},
	{TRIGGER_EXAMINE, CELL_BEHAVIOR_BY_SHEBA, FLAG_IDEJIMA_SHEBA_JOINED,
	 EVENT(scIdejimaWakeSheba)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
	 ACTOR_KRADEN | TRIGGER_SUBJECT_OWN_EFFECT, CONDITION_ALWAYS,
	 EVENT(scIdejimaWakeKraden)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
	 ACTOR_JENNA | TRIGGER_SUBJECT_OWN_EFFECT, CONDITION_ALWAYS,
	 EVENT(scIdejimaWakeJenna)},
	{TRIGGER_STEP, CELL_BEHAVIOR_CATCH_UP, CONDITION_ALWAYS,
	 EVENT(scIdejimaCatchUpWithFelix)},
	{TRIGGER_STEP, CELL_BEHAVIOR_WHERE_ALEX_WENT,
	 FLAG_IDEJIMA_ASKED_WHERE_ALEX_WENT,
	 EVENT(scIdejimaWonderWhereAlexHasGone)},
	{TRIGGER_TABLE_END},
};

u32 Emote_PopTiltedScript[] = {
	SC_WAIT(46),
	SC_RELEASE,
};
