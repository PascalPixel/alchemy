/* @file */
/* Sol Sanctum (SceneId_SolSanctumStatue), the room of what Kraden calls the */
/* statue puzzle, off the Luna and Sol rooms (SceneId_SolSanctumLunaSolRooms) */
/* and leading on to a room with a Psynergy Stone */
/* (SceneId_SolSanctumPsynergyStone). */
/* */
/* Eight winged statues stand around a ninth, the center statue, which bears a
 */
/* sun disc. Four can be pushed, or moved with Move, onto seals in the floor. */
/* Two others, pushed in front of the center statue, shine a beam at it. The */
/* last two never move. */
/* */
/* Walking into a statue pushes it a cell (scSolSanctumStatuePushStatue), and */
/* Move ends in a check of where it stands. */
/* */
/* Each move's trigger carries the flag of what it achieves (a seal open, a */
/* beam shining, the center statue moved), so a statue stops answering once it
 */
/* has done its part. scInit redraws the room from those */
/* story flags on every entry. */
/* */
/* The puzzle is a trap, and the room follows the story flags in order: */
/* - Until the center statue has moved (FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED),
 */
/*   the first seal opened cuts away to the Luna and Sol rooms, where the trap
 */
/*   is sprung */
/*   (SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_TRIGGERED, */
/*   FLAG_SOL_SANCTUM_FALLING_LIGHTS_SEEN). Kraden then comes here to explain */
/*   that moving one statue springs the sanctum's trap */
/*   (scSolSanctumStatueExplainStatuePuzzle, */
/*   FLAG_SOL_SANCTUM_STATUE_PUZZLE_EXPLAINED). From then on a seal opened */
/*   too soon closes again. Its statue slides back and the leader jumps clear.
 */
/* - With both beams shining, the floor before the center statue opens */
/*   (scSolSanctumStatueOpenCenterHole), and the center statue, moved south */
/*   with Move, drops into it (scSolSanctumStatueCenterStatueMoved). */
/* - From then on the seals stay open. The first one opened cuts away to the */
/*   Luna and Sol rooms with the trap disarmed */
/*   (SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_DISARMED), and */
/*   Kraden comes here to find the hole (scSolSanctumStatueExplainTrapDisarmed,
 */
/*   FLAG_SOL_SANCTUM_TRAP_DISARMED_EXPLAINED). Once all four seals are open */
/*   (FLAG_SOL_SANCTUM_ALL_SEALS_OPEN), the way on is found in the Luna and Sol
 */
/*   rooms. */
/* */
/* Both of Kraden's visits hold Jenna and Garet in place beside the leader, */
/* then walk them back into the party. Any other arrival hides them. */
/* */
/* Retreat does not work here. */
/* */
/* A task draws three rows of sprites that drift sideways at different speeds */
/* (scSolSanctumStatueUpdateScrollingSprs). They are the clouds of the sky */
/* seen between the pillars of the colonnade along the room's north wall. */
/* They sit 80 pixels down the map at the lowest priority, so they pass in */
/* front of the sky and behind the pillars. The rows move an eighth, a */
/* quarter and a half of a pixel a frame, the fastest drawn lowest */
/* (https://alchemy.pascalpixel.com/places/sol-sanctum-statue/sky-rows.gif: the
 */
/* top of the screen twice its size, above the sprites alone and the */
/* backgrounds alone, the clouds drifting behind the pillars). */
/* */
/* Once Mt. Aleph has erupted (FLAG_SOL_SANCTUM_ERUPTED) the room rumbles and */
/* shakes, as the rest of the sanctum does. */

#include "ability_ids.h"
#include "flags.h"
#include "macros.h"
#include "debug/debug.h"
#include "field/event_inline.h"
#include "field/map_grid_scroll.h"
#include "graphics/graphics.h"
#include "scene/walk_into_leader.h"
#include "sdk/sound_driver_symbols.h"
#include "text/msg_ids.h"

extern SCENE_ENTRANCE SolSanctumStatue_Entrances[];
extern u32 SolSanctumStatue_Exits[];
extern ACTOR_PLACE SolSanctumStatue_ActorPlacements[];
extern TRIGGER SolSanctumStatue_Triggers[];
extern s32 SolSanctumStatue_ScrollOffset;

/* Both beams have opened the hole in the floor before the center statue. */
/* Until then, Move on the center statue first walks the leader clear of it. */
#define FLAG_SOL_SANCTUM_CENTER_HOLE_OPEN FLAG_SCENE_TEMP_07

/* The west hall's beams and hole, drawn from spare cells the map keeps: on map
 */
/* layer 1 (the map's cells from (0, 32)) the beams, from its row 27 on, and on
 */
/* the ground the open hole, from cell (0, 28). */
/* The left beam statue's blue beam, from the statue beside the sun-disc */
/* statue to the floor before it: layer 1's cells (15, 6) to (18, 8). */
#define DRAW_LEFT_BEAM() fdMapGridCopyCellsTo(0, 59, 15, 38, 4, 3)
/* The right beam statue's beam: layer 1's cells (17, 6) to (20, 8). */
#define DRAW_RIGHT_BEAM() fdMapGridCopyCellsTo(4, 59, 17, 38, 4, 3)
/* Both beams, where they cross over the floor before the sun-disc statue: */
/* layer 1's cells (17, 7) to (18, 8). */
#define DRAW_CROSSED_BEAMS() fdMapGridCopyCellsTo(8, 60, 17, 39, 2, 2)
/* The hole open in the floor before the sun-disc statue: cells (17, 8) and */
/* (18, 8). */
#define DRAW_CENTER_HOLE() fdMapGridCopyCellsTo(0, 28, 17, 8, 2, 1)

/* Area flags: scArrive clears 0x300 to 0x3ff when the */
/* party enters another area, and other areas use the numbers for */
/* their own. */
/* Set once Kraden has found the hole and explained the disarmed trap. */
#define FLAG_SOL_SANCTUM_TRAP_DISARMED_EXPLAINED FLAG_AREA_TEMP_0A
/* Set once Kraden has explained the statue puzzle's trap. */
#define FLAG_SOL_SANCTUM_STATUE_PUZZLE_EXPLAINED FLAG_AREA_TEMP_0B

/* How far below the other two the front row of clouds, the fastest, is */
/* drawn: 8 pixels, and 6 from TBS_PATCH_3 on. */
#if TBS_PATCH_3
#define SOL_SANCTUM_STATUE_FRONT_CLOUD_ROW_Y_OFFSET 6
#else
#define SOL_SANCTUM_STATUE_FRONT_CLOUD_ROW_Y_OFFSET 8
#endif

/* A row of sky clouds: nine sprites 32 pixels wide, 288 pixels across the */
/* 240-pixel screen as the row scrolls. */
#define SOL_SANCTUM_STATUE_ROW_SPRITE_COUNT 9
#define SOL_SANCTUM_STATUE_SPRITE_WIDTH		32
/* The basic sprite palettes (syPaletteLoadBasicSprCols) the three rows of */
/* clouds are drawn in, back to front. */
enum {
	BACK_CLOUD_ROW_PALETTE = 10,
	MIDDLE_CLOUD_ROW_PALETTE = 13,
	FRONT_CLOUD_ROW_PALETTE = 0
};

/* The cell behaviors the step triggers answer. */
/* */
/* They are the jump spots and the four seal statues' and the center statue's */
/* cells; the beam statues' tracks follow in their own enum. */
typedef enum SOL_SANCTUM_STATUE_CELL_BEHAVIOR {
	CELL_BEHAVIOR_SOL_SANCTUM_STATUE_NORTHWEST_STATUE = 5,
	CELL_BEHAVIOR_SOL_SANCTUM_STATUE_NORTHEAST_STATUE = 6,
	CELL_BEHAVIOR_SOL_SANCTUM_STATUE_SOUTHWEST_STATUE = 7,
	CELL_BEHAVIOR_SOL_SANCTUM_STATUE_SOUTHEAST_STATUE = 8,
	CELL_BEHAVIOR_SOL_SANCTUM_STATUE_JUMP_NORTH =
		10, /* a jump spot, crossed northward */
	CELL_BEHAVIOR_SOL_SANCTUM_STATUE_JUMP_SOUTH =
		11, /* a jump spot, crossed southward */
	CELL_BEHAVIOR_SOL_SANCTUM_STATUE_CENTER_STATUE = 32
} SOL_SANCTUM_STATUE_CELL_BEHAVIOR;

/* The cells of the beam statues' tracks, where Isaac stands to push them. */
/* */
/* Each track runs north from the statue's start, cells (13, 7) to (13, 10) */
/* for the left statue and (22, 7) to (22, 10) for the right, with one more */
/* cell beside its top on the outer side. From the top the statue is pushed */
/* inward to stand beside the sun-disc statue. */
typedef enum SOL_SANCTUM_STATUE_BEAM_TRACK {
	CELL_BEHAVIOR_SOL_SANCTUM_STATUE_LEFT_TRACK_OUTSIDE_TOP = 20,
	CELL_BEHAVIOR_SOL_SANCTUM_STATUE_LEFT_TRACK_TOP,
	CELL_BEHAVIOR_SOL_SANCTUM_STATUE_LEFT_TRACK_SECOND,
	CELL_BEHAVIOR_SOL_SANCTUM_STATUE_LEFT_TRACK_THIRD,
	CELL_BEHAVIOR_SOL_SANCTUM_STATUE_LEFT_TRACK_BOTTOM,
	CELL_BEHAVIOR_SOL_SANCTUM_STATUE_RIGHT_TRACK_OUTSIDE_TOP,
	CELL_BEHAVIOR_SOL_SANCTUM_STATUE_RIGHT_TRACK_TOP,
	CELL_BEHAVIOR_SOL_SANCTUM_STATUE_RIGHT_TRACK_SECOND,
	CELL_BEHAVIOR_SOL_SANCTUM_STATUE_RIGHT_TRACK_THIRD,
	CELL_BEHAVIOR_SOL_SANCTUM_STATUE_RIGHT_TRACK_BOTTOM
} SOL_SANCTUM_STATUE_BEAM_TRACK;

static void scSolSanctumStatueInitScrollingSprs(void);
/* Isaac uses Move to draw the sun-disc statue one cell south from its place */
/* at the north of the west hall (cells (17, 7) and (18, 7)). Once both beam */
/* statues shine on it, it slides on south into the hole that has opened in */
/* the sun on the floor and settles in it with a grinding of stone; the */
/* room shakes hard, settles, and "Something clicked!" From then on the seals */
/* in the east hall stay open. */
/* */
/* With the beams not both shining, the statue only moves the one cell, and if
 */
/* Isaac stood in its way he first steps back south of it. A beam completing */
/* the pair runs this too, dropping a statue Move has already brought down. */
static void scSolSanctumStatueCenterStatueMoved(void);
static void scSolSanctumStatuePushStatue(s32 statueId, s32 statueColumn,
										 s32 statueRow, s32 leaderColumn,
										 s32 leaderRow);
static s32 scSolSanctumStatueIsActorAtCell(s32 actorId, s32 column, s32 row);
static void scSolSanctumStatueOpenNorthwestSeal(void);
static void scSolSanctumStatueOpenNortheastSeal(void);
static void scSolSanctumStatueOpenSouthwestSeal(void);
static void scSolSanctumStatueOpenSoutheastSeal(void);
static void scSolSanctumStatueOpenSeal(void);
static void scSolSanctumStatueShineLeftBeam(void);
static void scSolSanctumStatueShineRightBeam(void);
static void scSolSanctumStatueSayThenWait(s32 speaker, s32 frames);

/* A block of cells copied from one place in the map grid to another. */
typedef struct CELL_COPY {
	s32 srcX;	/* map cells */
	s32 srcY;	/* map cells */
	s32 dstX;	/* map cells */
	s32 dstY;	/* map cells */
	s32 width;	/* map cells */
	s32 height; /* map cells */
} CELL_COPY;

/* Sets a cell copy: columns by rows cells from (x, y) to (toX, toY). */
#define CELL_COPY_SET(copy, x, y, toX, toY, columns, rows)                     \
	((copy).srcX = (x), (copy).srcY = (y), (copy).dstX = (toX),                \
	 (copy).dstY = (toY), (copy).width = (columns), (copy).height = (rows))

/* The seal being opened. */
/* */
/* It holds the seal's cell animations, its flag, where the leader jumps back */
/* to, the cells that close it again (`closed`) and where its statue returns */
/* if the story does not go on. */
typedef struct SEAL_OPENING {
	CELL_COPY open;
	CELL_COPY pulseA;
	CELL_COPY pulseB;
	CELL_COPY opened;
	s32 flg;
	s32 facing; /* 1/65536 turns */
	s32 dstX;	/* pixels */
	s32 dstZ;	/* pixels */
	CELL_COPY closed[3];
	s32 actorId;
	s32 actorX;	   /* pixels */
	s32 actorZ;	   /* pixels */
	s32 unused[3]; /* never read or written; the bss keeps its 12 bytes */
} SEAL_OPENING;

STATIC_ASSERT(offsetof(SEAL_OPENING, unused) == 196);

/* The placed actors, numbered in the order of */
/* SolSanctumStatue_ActorPlacements. */
/* */
/* They are Kraden, who stays out of sight until his two visits, the eight */
/* statues, and the center statue in its two places. Jenna and Garet, placed */
/* after them, keep their own slots. */
/* */
/* Of the statues, four are pushed onto seals and two shine beams at the */
/* center. The other two never move. */
typedef enum SOL_SANCTUM_STATUE_ACTOR_ID {
	ACTOR_KRADEN = ACTOR_FIRST_PLACED,
	ACTOR_NORTHWEST_STATUE,
	ACTOR_LEFT_BEAM_STATUE,
	ACTOR_NORTHEAST_STATUE,
	ACTOR_RIGHT_BEAM_STATUE,
	ACTOR_SOUTHWEST_STATUE,
	/* South of the left beam statue, and moved by no event. */
	ACTOR_LEFT_STILL_STATUE,
	ACTOR_SOUTHEAST_STATUE,
	/* South of the right beam statue, and moved by no event. */
	ACTOR_RIGHT_STILL_STATUE,
	ACTOR_CENTER_STATUE,
	/* Stands in the hole in the floor once the center statue has dropped in. */
	ACTOR_MOVED_CENTER_STATUE
} SOL_SANCTUM_STATUE_ACTOR_ID;

/* The seal the last statue moved has set up to open. */
static SEAL_OPENING sSealOpening ALIGNED(16);
/* Initialization writes each three-word OAM packet, and drawing updates its */
/* fields. */
static union {
	ANIMATION_SPR_PART parts[3 * SOL_SANCTUM_STATUE_ROW_SPRITE_COUNT];
	s32 words[3 * SOL_SANCTUM_STATUE_ROW_SPRITE_COUNT * 3];
} sScrollingSprites;

const SCENE_ENTRANCE *scGetEntrances(void) {
	return SolSanctumStatue_Entrances;
}

const SCENE_EXIT_REGION *scGetExitRegions(void) { return NULL; }

const u32 *scGetExits(void) { return SolSanctumStatue_Exits; }

/* Returns Kraden, the eight statues, the center statue and its moved copy */
/* (ACTOR_KRADEN to ACTOR_MOVED_CENTER_STATUE), then Jenna and Garet. */
const ACTOR_PLACE *scGetActorPlacements(void) {
	return SolSanctumStatue_ActorPlacements;
}

/* Returns the statues' Move and push events, each row only until its seal or */
/* beam is done. */
const TRIGGER *scGetTriggers(void) { return SolSanctumStatue_Triggers; }

/* Chooses the event in the Luna and Sol rooms that a newly opened */
/* seal leads to, and requests its exit. */
/* */
/* Returns 1 when it requests an exit, otherwise -1. */
static s32 scSolSanctumStatueSelectStoryExit(void) {
	if(gmFlagGet(FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED) != 0) {
		if(gmFlagGet(FLAG_SOL_SANCTUM_KRADEN_SAID_FOUND_SOMETHING) == 0) {
			syImportEventSetExitRequest(
				SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_DISARMED);
			return 1;
		}
	}
	else if(gmFlagGet(FLAG_SOL_SANCTUM_FALLING_LIGHTS_SEEN) == 0) {
		syImportEventSetExitRequest(
			SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_TRIGGERED);
		return 1;
	}
	return -1;
}

/* The floor before the center statue flashes this many times, eight frames */
/* each, as it opens. */
#define CENTER_HOLE_FLASHES 22

/* Animates the floor opening before the center statue once both beams shine, */
/* then lets the statue drop in if Move has already brought it there. */
static void scSolSanctumStatueOpenCenterHole(void) {
	s32 flash;

	DRAW_CENTER_HOLE();
	sySndPlay(SE_SPARKLE_FALL);
	for(flash = 0; flash != CENTER_HOLE_FLASHES; flash++) {
		/* The crossed beams' lower cells flash as the floor opens. */
		fdMapGridCopyCellsTo(10, 61, 17, 40, 2, 1);
		fdEventWait(4);
		fdMapGridCopyCellsTo(8, 61, 17, 40, 2, 1);
		fdEventWait(4);
	}
	DRAW_LEFT_BEAM();
	DRAW_RIGHT_BEAM();
	DRAW_CROSSED_BEAMS();
	fdMapGridCopyCellAttributes(0, 0, 2, 1, 17, 8);
	gmFlagSet(FLAG_SOL_SANCTUM_CENTER_HOLE_OPEN);
	scSolSanctumStatueCenterStatueMoved();
}

/* Kraden's second visit, an event in which he comes to find the hole in the */
/* floor. */
static void scSolSanctumStatueExplainTrapDisarmed(void) {
	fdEventStart();
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	FieldStateWork->transitionFrm = 32;
	fdEventStartScrTransition();
	fdEventWaitTransitionFrames();
	fdEventWait(20);
	fdActorSetPos(ACTOR_KRADEN, 576, 232);
	fdEventWait(1);
	/* Kraden, from the south: "You found it, Isaac!" Jenna: "Kraden!" */
	fdEventSetMessageId(MsgSolSanctumKradenYouFoundIt);
	scSolSanctumStatueSayThenWait(ACTOR_KRADEN, 6);
	fdActorSetPos(ACTOR_KRADEN, 576, 280);
	fdCameraSetSpeed(0.8);
	fdCameraMoveTo(574, CAMERA_KEEP, 180, 1);
	fdActorSetSpeed(ACTOR_KRADEN, 1);
	fdActorWalkToAndWait(ACTOR_KRADEN, 576, 216);
	fdEventWait(20);
	fdActorJump(ACTOR_JENNA, 2, 0);
	fdEventWait(30);
	scSolSanctumStatueSayThenWait(ACTOR_JENNA, 6);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	fdEventWait(6);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST_NORTHWEST, 0);
	fdEventWait(10);
	/* The camera swings west to the hole at the center and back. */
	fdCameraSetSpeed(5.6);
	fdCameraMoveTo(287, CAMERA_KEEP, 176, 1);
	fdCameraWaitForMove();
	fdEventWait(60);
	fdCameraMoveTo(574, CAMERA_KEEP, 180, 1);
	fdCameraWaitForMove();
	fdEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	fdEventWait(10);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
	fdEventWait(10);
	/* Kraden hops up to the party: "Excellent work, Isaac!" */
	fdActorJump(ACTOR_KRADEN, 6, 0);
	fdActorSetSpeedAndAcceleration(ACTOR_KRADEN, 3, 2);
	fdActorWalkToAndWait(ACTOR_KRADEN, 576, 184);
	fdEventWait(40);
	scSolSanctumStatueSayThenWait(ACTOR_KRADEN, 6);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
	fdEventWait(40);
	/* The screen closes and opens on Kraden by the hole: "Where did that hole
	 */
	/* come from?" Then back to the party. */
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
	FieldStateWork->transitionFrm = MAP_TRANSITION_FRAMES;
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	fdCameraMoveTo(287, CAMERA_KEEP, 176, 0);
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	fdEventStartScrTransition();
	fdEventWaitTransitionFrames();
	fdEventWait(40);
	fdActorSetPos(ACTOR_KRADEN, 424, 200);
	sySynch(1);
	scSolSanctumStatueSayThenWait(ACTOR_KRADEN, 20);
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	fdActorSetPos(ACTOR_KRADEN, 576, 184);
	fdCameraMoveTo(574, CAMERA_KEEP, 180, 0);
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	fdEventStartScrTransition();
	fdEventWaitTransitionFrames();
	fdEventWait(20);
	fdCameraSetSpeed(1.2);
	fdCameraMoveTo(574, CAMERA_KEEP, 157, 1);
	fdCameraWaitForMove();
	fdEventWait(20);
	/* Garet: two beams shot out of the statues Isaac moved, and the hole */
	/* appeared. Jenna: Isaac dropped the statue into it. */
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	scSolSanctumStatueSayThenWait(ACTOR_GARET, 20);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
	scSolSanctumStatueSayThenWait(ACTOR_JENNA, 6);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	fdEventWait(40);
	/* Kraden: "Good work, everyone!" and "So the statue puzzle was the trap..."
	 */
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
	scSolSanctumStatueSayThenWait(ACTOR_KRADEN, 80);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdEventWait(60);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
	fdEventWait(30);
	scSolSanctumStatueSayThenWait(ACTOR_KRADEN, 20);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdEventWait(60);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
	fdEventWait(30);
	/* Kraden: the other statue was there to disarm the trap, and now they */
	/* should be all right. */
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
	scSolSanctumStatueSayThenWait(ACTOR_KRADEN, 6);
	fdActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GARET, 0);
	fdEventWait(40);
	fdActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_JENNA, 0);
	fdEventWait(40);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	scSolSanctumStatueSayThenWait(ACTOR_KRADEN, 6);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
	fdEventWait(40);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	/* Kraden: he will observe from the Luna room. He leaves south. */
	scSolSanctumStatueSayThenWait(ACTOR_KRADEN, 6);
	fdCameraSetSpeed(1.6);
	fdCameraMoveTo(576, CAMERA_KEEP, 215, 1);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH, 0);
	fdEventWait(10);
	fdActorJump(ACTOR_KRADEN, 6, 0);
	fdActorWalkToAndWait(ACTOR_KRADEN, 576, 217);
	fdEventWait(20);
	fdActorWalkToAndWait(ACTOR_KRADEN, 576, 321);
	fdActorSetPos(ACTOR_KRADEN, 0, 0);
	fdCameraSetSpeed(3.6);
	fdCameraMoveTo(576, CAMERA_KEEP, 136, 1);
	fdCameraWaitForMove();
	fdEventWait(20);
	fdActorSetSpeed(ACTOR_GARET, 1);
	fdActorSetSpeed(ACTOR_JENNA, 1);
	scWalkIntoLeader(ACTOR_GARET);
	scWalkIntoLeader(ACTOR_JENNA);
	fdEventClearFlag(FLAG_SHOW_AREA_NAME);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	FieldStateWork->transitionFrm = MAP_TRANSITION_FRAMES;
	fdEventEnd();
}

/* Kraden's first visit, an event in which he runs here to warn that the */
/* mechanism is a trap. */
static void scSolSanctumStatueExplainStatuePuzzle(void) {
	FIELD_STATE *state;

	fdEventStart();
	state = FieldStateWork;
	state->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	state->transitionFrm = 32;
	fdEventStartScrTransition();
	fdEventWaitTransitionFrames();
	fdEventWait(20);
	fdActorSetPos(ACTOR_KRADEN, 576, 296);
	fdEventWait(1);
	/* Kraden, from the south: "Wait, Isaac!" Jenna: "What's wrong, Kraden?" */
	fdEventSetMessageId(MsgSolSanctumWait);
	scSolSanctumStatueSayThenWait(ACTOR_KRADEN, 6);
	fdCameraSetSpeed(0.8);
	fdCameraMoveTo(574, CAMERA_KEEP, 180, 1);
	fdActorSetSpeed(ACTOR_KRADEN, 1);
	fdActorWalkToAndWait(ACTOR_KRADEN, 576, 216);
	fdEventWait(20);
	fdActorJump(ACTOR_JENNA, 2, 0);
	fdEventWait(30);
	scSolSanctumStatueSayThenWait(ACTOR_JENNA, 6);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	fdEventWait(6);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST_NORTHWEST, 0);
	fdEventWait(10);
	fdCameraSetSpeed(5.6);
	fdCameraMoveTo(287, CAMERA_KEEP, 176, 1);
	fdCameraWaitForMove();
	fdEventWait(60);
	fdCameraMoveTo(574, CAMERA_KEEP, 180, 1);
	fdCameraWaitForMove();
	fdEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	fdEventWait(10);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
	fdEventWait(10);
	fdActorJump(ACTOR_KRADEN, 6, 0);
	fdActorSetSpeedAndAcceleration(ACTOR_KRADEN, 3, 2);
	fdActorWalkToAndWait(ACTOR_KRADEN, 576, 184);
	fdEventWait(40);
	/* Kraden: "This is a most interesting mechanism you've found, Isaac." */
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	scSolSanctumStatueSayThenWait(ACTOR_KRADEN, 6);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdEventWait(40);
	/* Kraden: "But this is clearly... some kind of trap." All three start. */
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 3);
	scSolSanctumStatueSayThenWait(ACTOR_KRADEN, 6);
	fdActorJump(ACTOR_PARTY_LEADER, 2, 0);
	fdActorJump(ACTOR_GARET, 2, 0);
	fdActorJump(ACTOR_JENNA, 2, 0);
	fdEventWait(30);
	/* Garet: "This is...a trap?" Kraden: "Go look in the Luna room". */
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	scSolSanctumStatueSayThenWait(ACTOR_GARET, 6);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 1);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
	scSolSanctumStatueSayThenWait(ACTOR_KRADEN, 6);
	fdActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_JENNA, 0);
	fdEventWait(40);
	fdActorFaceActor(ACTOR_KRADEN, ACTOR_PARTY_LEADER, 0);
	fdActorFaceActor(ACTOR_KRADEN, ACTOR_JENNA, 0);
	fdEventWait(40);
	fdActorJump(ACTOR_KRADEN, 6, 0);
	fdActorWalkToAndWait(ACTOR_KRADEN, 576, 216);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
	fdEventWait(10);
	fdActorSetSpeed(ACTOR_KRADEN, 1.2);
	fdActorWalkTo(ACTOR_KRADEN, 432, 200);
	fdEventWait(20);
	fdCameraSetSpeed(2.4);
	fdCameraMoveTo(288, CAMERA_KEEP, 171, 1);
	fdCameraWaitForMove();
	fdEventWait(80);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_STAND);
	fdCameraMoveTo(574, CAMERA_KEEP, 180, 1);
	fdEventWait(20);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
	fdEventWait(30);
	fdActorWalkToAndWait(ACTOR_KRADEN, 576, 216);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
	fdEventWait(10);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
	fdCameraSetSpeed(0.8);
	fdCameraMoveTo(574, CAMERA_KEEP, 171, 1);
	fdActorJump(ACTOR_KRADEN, 6, 0);
	fdActorSetSpeedAndAcceleration(ACTOR_KRADEN, 3, 2);
	fdActorWalkToAndWait(ACTOR_KRADEN, 576, 184);
	fdEventWait(80);
	/* Kraden: they are looking in the right place, but moving one statue */
	/* springs the trap. Jenna: why would anyone put a trap here? */
	scSolSanctumStatueSayThenWait(ACTOR_KRADEN, 6);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
	scSolSanctumStatueSayThenWait(ACTOR_KRADEN, 20);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdEventWait(40);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
	scSolSanctumStatueSayThenWait(ACTOR_JENNA, 6);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
	fdEventWait(40);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
	fdEventWait(30);
	/* Kraden: they have hidden something very important, perhaps the key to */
	/* Alchemy itself. */
	scSolSanctumStatueSayThenWait(ACTOR_KRADEN, 6);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdEventWait(60);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
	scSolSanctumStatueSayThenWait(ACTOR_KRADEN, 6);
	fdActorFaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
	fdEventWait(40);
	fdActorFaceEachOther(ACTOR_JENNA, ACTOR_PARTY_LEADER, 0);
	fdEventWait(40);
	/* Kraden: "Should I tell you more?" Either answer, he thinks they would not
	 */
	/* understand yet. */
	fdEventOpenMessage(ACTOR_KRADEN, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
	if(fdEventAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
		fdEventSetMessageId(MsgSolSanctumHonestlyDoubtUnderstand);
	}
	else {
		fdEventSetMessageId(MsgSolSanctumHmphWellTold);
	}
	scSolSanctumStatueSayThenWait(ACTOR_KRADEN, 6);
	/* Kraden: "We must try to find a solution to this mystery." Garet: */
	/* "Sheesh... Another mystery?" Kraden will keep checking the Luna room. */
	fdEventSetMessageId(MsgSolSanctumTryFindSolution);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	scSolSanctumStatueSayThenWait(ACTOR_KRADEN, 6);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdEventWait(60);
	scSolSanctumStatueSayThenWait(ACTOR_GARET, 6);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	scSolSanctumStatueSayThenWait(ACTOR_KRADEN, 6);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH, 0);
	fdEventWait(20);
	fdActorJump(ACTOR_KRADEN, 6, 0);
	fdActorWalkToAndWait(ACTOR_KRADEN, 576, 216);
	fdEventWait(40);
	fdCameraSetSpeed(0.8);
	fdCameraMoveTo(574, CAMERA_KEEP, 191, 1);
	fdActorSetSpeed(ACTOR_KRADEN, 1);
	fdActorWalkToAndWait(ACTOR_KRADEN, 576, 232);
	fdEventWait(40);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	fdEventWait(40);
	/* Kraden, leaving: his intuition says the light is the key to the secret.
	 */
	scSolSanctumStatueSayThenWait(ACTOR_KRADEN, 6);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
	fdEventWait(30);
	scSolSanctumStatueSayThenWait(ACTOR_KRADEN, 6);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	fdCameraMoveTo(576, CAMERA_KEEP, 215, 1);
	fdActorWalkToAndWait(ACTOR_KRADEN, 574, 323);
	fdActorSetPos(ACTOR_KRADEN, 0, 0);
	fdCameraSetSpeed(3.6);
	fdCameraMoveTo(576, CAMERA_KEEP, 136, 1);
	fdCameraWaitForMove();
	fdEventWait(20);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 0);
	fdEventWait(10);
	/* Jenna: "let's see if we can't figure this out ourselves." */
	scSolSanctumStatueSayThenWait(ACTOR_JENNA, 6);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
	fdActorSetSpeed(ACTOR_GARET, 1);
	fdActorSetSpeed(ACTOR_JENNA, 1);
	scWalkIntoLeader(ACTOR_JENNA);
	scWalkIntoLeader(ACTOR_GARET);
	gmFlagClear(FLAG_SHOW_AREA_NAME);
	state = FieldStateWork;
	state->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	state->transitionFrm = MAP_TRANSITION_FRAMES;
	fdEventEnd();
}

/* Unused: called only from the branch of scSolSanctumStatueOpenSeal that */
/* cannot run, since scSolSanctumStatueSelectStoryExit returns only 1 or -1. */
static void scSolSanctumStatueUnusedPutOutSeals(void) {
	MAP_GRID_SCROLL_STATE *grid;
	ACTOR *leader;
	s32 *savedOrigin;
	FIXED cameraPosition[3];
	s32 approachSide;
	s32 frame;

	grid = MapGridState[0];
	leader = fdActorGet(ACTOR_PARTY_LEADER);
	/* Temporarily follow a local point, panning from the leader toward the
	 * seal. */
	if(leader->z.fixed < toFIXED(179)) {
		fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 575, 132);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
		fdEventWait(30);
		savedOrigin = grid->origin;
		SET_VECTOR(cameraPosition, leader->x.fixed, leader->y.fixed,
				   leader->z.fixed);
		grid->origin = cameraPosition;
		for(frame = 0; frame != 30; frame++) {
			cameraPosition[2] += toFIXED(1);
			fdEventWait(1);
		}
		fdEventWait(40);
		approachSide = 1;
	}
	else {
		fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 577, 222);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
		fdEventWait(30);
		SET_VECTOR(cameraPosition, leader->x.fixed, leader->y.fixed,
				   leader->z.fixed);
		savedOrigin = grid->origin;
		grid->origin = cameraPosition;
		for(frame = 0; frame != 30; frame++) {
			cameraPosition[2] -= toFIXED(1);
			fdEventWait(1);
		}
		fdEventWait(40);
		approachSide = 2;
	}
	for(frame = 0; frame != 6; frame++) {
		fdMapGridCopyCellsTo(2, 28, 34, 10, 4, 2);
		fdEventWait(8);
		fdMapGridCopyCellsTo(2, 30, 34, 10, 4, 2);
		fdEventWait(8);
	}
	for(frame = 0; frame != 10; frame++) {
		fdMapGridCopyCellsTo(2, 28, 34, 10, 4, 2);
		fdEventWait(4);
		fdMapGridCopyCellsTo(2, 30, 34, 10, 4, 2);
		fdEventWait(4);
	}
	for(frame = 0; frame != 12; frame++) {
		fdMapGridCopyCellsTo(2, 28, 34, 10, 4, 2);
		fdEventWait(2);
		fdMapGridCopyCellsTo(2, 30, 34, 10, 4, 2);
		fdEventWait(2);
	}
	fdMapGridCopyCellsTo(2, 28, 34, 10, 4, 2);
	fdMapGridCopyCellsTo(8, 55, 32, 40, 8, 4);
	fdEventWait(60);
	/* Reverse the pan before restoring the caller's camera target. */
	if(approachSide == 1) {
		for(frame = 0; frame != 30; frame++) {
			cameraPosition[2] -= toFIXED(1);
			fdEventWait(1);
		}
	}
	else if(approachSide == 2) {
		for(frame = 0; frame != 30; frame++) {
			cameraPosition[2] += toFIXED(1);
			fdEventWait(1);
		}
	}
	grid->origin = savedOrigin;
}

/* The clouds' scroll offset gains this a frame. The back row moves by its */
/* 1024th part, the middle by its 512th and the front by its 256th: an eighth,
 */
/* a quarter and half a pixel a frame. */
#define SCROLL_OFFSET_STEP 128

/* Draws the three rows of sky clouds while they overlap the screen. */
static void scSolSanctumStatueUpdateScrollingSprs(void) {
	MAP_GRID_SCROLL_STATE *grid = MapGridState[0];
	ANIMATION_SPR_PART *sprite = sScrollingSprites.parts;
	s32 cameraX = syMapGridScrollStateView(grid)[0] / toFIXED(1);
	s32 screenY = 80 - syMapGridScrollStateView(grid)[1] / toFIXED(1);
	s32 scrollX;
	u32 column;

	if(screenY >= -16 && screenY < DISPLAY_HEIGHT) {
		scrollX = ((SolSanctumStatue_ScrollOffset >> 10) - cameraX) |
				  -SOL_SANCTUM_STATUE_SPRITE_WIDTH;
		for(column = 0; column < SOL_SANCTUM_STATUE_ROW_SPRITE_COUNT;
			column++) {
			sprite->x = scrollX;
			sprite->y = screenY;
			syOamAddSprAtDepth(sprite, 0);
			scrollX += SOL_SANCTUM_STATUE_SPRITE_WIDTH;
			sprite++;
		}
		scrollX = ((SolSanctumStatue_ScrollOffset >> 9) - cameraX) |
				  -SOL_SANCTUM_STATUE_SPRITE_WIDTH;
		for(column = 0; column < SOL_SANCTUM_STATUE_ROW_SPRITE_COUNT;
			column++) {
			sprite->x = scrollX;
			sprite->y = screenY;
			syOamAddSprAtDepth(sprite, 0);
			scrollX += SOL_SANCTUM_STATUE_SPRITE_WIDTH;
			sprite++;
		}
		scrollX = ((SolSanctumStatue_ScrollOffset >> 8) - cameraX) |
				  -SOL_SANCTUM_STATUE_SPRITE_WIDTH;
		for(column = 0; column < SOL_SANCTUM_STATUE_ROW_SPRITE_COUNT;
			column++) {
			sprite->x = scrollX;
			sprite->y = screenY + SOL_SANCTUM_STATUE_FRONT_CLOUD_ROW_Y_OFFSET;
			syOamAddSprAtDepth(sprite, 0);
			scrollX += SOL_SANCTUM_STATUE_SPRITE_WIDTH;
			sprite++;
		}
	}
	SolSanctumStatue_ScrollOffset += SCROLL_OFFSET_STEP;
}

/* Walking north over a jump spot jumps the leader two cells ahead */
/* (fdTryJumpForward). */
static void scSolSanctumStatueJumpNorth(void) {
	ACTOR *leader = fdActorGet(ACTOR_PARTY_LEADER);
	if(leader->facing == FACING_NORTH) {
		fdTryJumpForward();
	}
}

/* Walking south over a jump spot jumps the leader two cells ahead. */
static void scSolSanctumStatueJumpSouth(void) {
	ACTOR *leader = fdActorGet(ACTOR_PARTY_LEADER);
	if(leader->facing == FACING_SOUTH) {
		fdTryJumpForward();
	}
}

/* Unused: the eastern hall's northwest statue slides by itself to (504, 152),
 */
/* the cell where Move opens its seal, unless that seal is already open. */
/* */
/* It opens nothing. */
/* */
/* The player moves the statue there with Move instead. */
UNUSED static void scSolSanctumStatueUnusedMoveNorthwestStatue(void) {
	if(gmFlagGet(FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN) == 0) {
		fdEventStart();
		fdActorGet(ACTOR_NORTHWEST_STATUE);
		fdActorSetSpeed(ACTOR_NORTHWEST_STATUE, 0.2);
		fdActorWalkToAndWait(ACTOR_NORTHWEST_STATUE, 504, 152);
		fdEventEnd();
	}
}

/* Restores the room's statues, beams and seals from their flags and plays */
/* Kraden's two visits. */
s32 scInit(void) {
	ACTOR *centerEffect;

	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	scSolSanctumStatueInitScrollingSprs();
	gmFlagSet(FLAG_RETREAT_BLOCKED);
	centerEffect = fdActorGet(ACTOR_MOVED_CENTER_STATUE);
	centerEffect->collisionFlg = 0;
	{
		u8 flags = centerEffect->priFlg | ACTOR_PRIORITY_UNDERFOOT;

		centerEffect->priFlg = flags;
	}
	fdActorSetSprFlags(fdActorGet(ACTOR_MOVED_CENTER_STATUE), 0);
	fdActorGet(ACTOR_MOVED_CENTER_STATUE)->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
	fdActorSetSprPriority(ACTOR_MOVED_CENTER_STATUE, 1);
	/* Only the returns with the trap disarmed or triggered keep Jenna and Garet
	 */
	/* staged for their conversations. */
	if(MainRoutineStateWork.entrance !=
		   SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_DISARMED &&
	   MainRoutineStateWork.entrance !=
		   SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_TRIGGERED) {
		fdActorSetPos(ACTOR_JENNA, 0, 0);
		fdActorSetPos(ACTOR_GARET, 0, 0);
	}
	if(gmFlagGet(FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED) != 0) {
		fdActorSetPos(ACTOR_MOVED_CENTER_STATUE, 288, 178);
		fdActorSetPos(ACTOR_CENTER_STATUE, 1608, 1608);
		fdActorSetPos(ACTOR_LEFT_BEAM_STATUE, 232, 120);
		fdActorSetPos(ACTOR_RIGHT_BEAM_STATUE, 344, 120);
		fdActorSetPos(ACTOR_LEFT_BEAM_STATUE, 232, 120);
		DRAW_LEFT_BEAM();
		fdActorSetPos(ACTOR_RIGHT_BEAM_STATUE, 344, 120);
		DRAW_RIGHT_BEAM();
		DRAW_CROSSED_BEAMS();
		fdMapGridCopyCellAttributes(0, 1, 2, 1, 17, 7);
	}
	else if(gmFlagGet(FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING) != 0 &&
			gmFlagGet(FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING) != 0) {
		fdActorSetPos(ACTOR_LEFT_BEAM_STATUE, 232, 120);
		fdActorSetPos(ACTOR_RIGHT_BEAM_STATUE, 344, 120);
		DRAW_CENTER_HOLE();
		fdActorSetPos(ACTOR_LEFT_BEAM_STATUE, 232, 120);
		DRAW_LEFT_BEAM();
		fdActorSetPos(ACTOR_RIGHT_BEAM_STATUE, 344, 120);
		DRAW_RIGHT_BEAM();
		DRAW_CROSSED_BEAMS();
		fdMapGridCopyCellAttributes(0, 0, 2, 1, 17, 8);
	}
	else {
		if(gmFlagGet(FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING) != 0) {
			fdActorSetPos(ACTOR_LEFT_BEAM_STATUE, 232, 120);
			DRAW_LEFT_BEAM();
		}
		if(gmFlagGet(FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING) != 0) {
			fdActorSetPos(ACTOR_RIGHT_BEAM_STATUE, 344, 120);
			DRAW_RIGHT_BEAM();
		}
	}
	/* Each open seal has its statue on it, and swaps its quarter of the two */
	/* halls' floor pictures: the east hall's moon (cells (34, 10) to (37, 11))
	 */
	/* lit as the sun, and the west hall's sun ((16, 10) to (19, 11)) darkened
	 * to */
	/* the moon, from the spare sun and moon at cells (2, 28) and (2, 30). Its
	 */
	/* statue's beam is drawn over the east picture on map layer 1. */
	if(gmFlagGet(FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN) != 0) {
		fdActorSetPos(ACTOR_NORTHWEST_STATUE, 504, 152);
		fdMapGridCopyCellsTo(2, 28, 34, 10, 2, 1);
		fdMapGridCopyCellsTo(2, 30, 16, 10, 2, 1);
		fdMapGridCopyCellsTo(0, 55, 32, 40, 4, 3);
	}
	if(gmFlagGet(FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN) != 0) {
		fdActorSetPos(ACTOR_NORTHEAST_STATUE, 648, 152);
		fdMapGridCopyCellsTo(4, 28, 36, 10, 2, 1);
		fdMapGridCopyCellsTo(4, 30, 18, 10, 2, 1);
		fdMapGridCopyCellsTo(4, 55, 36, 40, 4, 3);
	}
	if(gmFlagGet(FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN) != 0) {
		fdActorSetPos(ACTOR_SOUTHWEST_STATUE, 504, 200);
		fdMapGridCopyCellsTo(2, 29, 34, 11, 2, 1);
		fdMapGridCopyCellsTo(2, 31, 16, 11, 2, 1);
		fdMapGridCopyCellsTo(0, 58, 32, 43, 4, 1);
	}
	if(gmFlagGet(FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN) != 0) {
		fdActorSetPos(ACTOR_SOUTHEAST_STATUE, 648, 200);
		fdMapGridCopyCellsTo(4, 29, 36, 11, 2, 1);
		fdMapGridCopyCellsTo(4, 31, 18, 11, 2, 1);
		fdMapGridCopyCellsTo(4, 58, 36, 43, 4, 1);
	}
	if(MainRoutineStateWork.entrance ==
	   SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_DISARMED) {
		if(fdEventGetFlag(FLAG_SOL_SANCTUM_TRAP_DISARMED_EXPLAINED) != 0) {
			fdActorSetPos(ACTOR_GARET, 0, 0);
			fdActorSetPos(ACTOR_JENNA, 0, 0);
		}
		else if(gmFlagGet(FLAG_KEEP_PARTY_POSITION) == 0) {
			scSolSanctumStatueExplainTrapDisarmed();
			gmFlagSet(FLAG_SOL_SANCTUM_TRAP_DISARMED_EXPLAINED);
		}
	}
	if(MainRoutineStateWork.entrance ==
	   SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_TRIGGERED) {
		if(fdEventGetFlag(FLAG_SOL_SANCTUM_STATUE_PUZZLE_EXPLAINED) != 0) {
			fdActorSetPos(ACTOR_GARET, 0, 0);
			fdActorSetPos(ACTOR_JENNA, 0, 0);
		}
		else if(gmFlagGet(FLAG_KEEP_PARTY_POSITION) == 0) {
			scSolSanctumStatueExplainStatuePuzzle();
			gmFlagSet(FLAG_SOL_SANCTUM_STATUE_PUZZLE_EXPLAINED);
		}
	}
	if(gmFlagGet(FLAG_SOL_SANCTUM_ERUPTED) != 0) {
		scQueueMusic(SE_RUMBLE);
		fdMapGridSetShake(1, 1, 1);
		fdEffectInitSparkles();
	}
	return 0;
}

/* Each drifting row's picture: one 32x8 sprite, repeated along the row. */
#define ROW_SPRITE_BYTES (4 * TILE_SIZE_4BPP)

/* Loads the three strips of graphics and fills in a row of sprites for each. */
static void scSolSanctumStatueInitScrollingSprs(void) {
	u32 column;
	s32 tile;
	s32 *spriteWords;
	u8 *pixels;

	spriteWords = sScrollingSprites.words;
	pixels = syHeapSlotAllocEwram(HEAP_SLOT_TILE_SCRATCH, 1024);
	fdDecompressByteLz(SolSanctumStatue_SpriteRowsGraphics, pixels);
	tile =
		syVramCacheLoad(syVramCacheFindFreeEntry(), ROW_SPRITE_BYTES, pixels);
	for(column = 0; column < SOL_SANCTUM_STATUE_ROW_SPRITE_COUNT; column++) {
		s32 *attributeWords = spriteWords;

		*attributeWords++ = 0;
		*attributeWords++ = OBJ_SIZE_32X8;
		spriteWords += 3;
		*attributeWords =
			tile | OBJ_PRIORITY(3) | OBJ_PALETTE(BACK_CLOUD_ROW_PALETTE);
	}
	tile = syVramCacheLoad(syVramCacheFindFreeEntry(), ROW_SPRITE_BYTES,
						   pixels + ROW_SPRITE_BYTES);
	for(column = 0; column < SOL_SANCTUM_STATUE_ROW_SPRITE_COUNT; column++) {
		s32 *attributeWords = spriteWords;

		*attributeWords++ = 0;
		*attributeWords++ = OBJ_SIZE_32X8;
		spriteWords += 3;
		*attributeWords =
			tile | OBJ_PRIORITY(3) | OBJ_PALETTE(MIDDLE_CLOUD_ROW_PALETTE);
	}
	tile = syVramCacheLoad(syVramCacheFindFreeEntry(), ROW_SPRITE_BYTES,
						   pixels + 2 * ROW_SPRITE_BYTES);
	for(column = 0; column < SOL_SANCTUM_STATUE_ROW_SPRITE_COUNT; column++) {
		s32 *attributeWords = spriteWords;

		*attributeWords++ = 0;
		*attributeWords++ = OBJ_SIZE_32X8;
		spriteWords += 3;
		*attributeWords =
			tile | OBJ_PRIORITY(3) | OBJ_PALETTE(FRONT_CLOUD_ROW_PALETTE);
	}
	syHeapSlotRelease(HEAP_SLOT_TILE_SCRATCH);
	syTaskAddFrame(scSolSanctumStatueUpdateScrollingSprs, TASK_PASS_FRAME);
}

/* Isaac moves the northwest statue of the east hall with Move; if it now */
/* stands on its seal (cell (31, 9)), the seal opens. */
static void scSolSanctumStatueNorthwestStatueMoved(void) {
	if(scSolSanctumStatueIsActorAtCell(ACTOR_NORTHWEST_STATUE, 31, 9) != 0) {
		scSolSanctumStatueOpenNorthwestSeal();
	}
}

/* Isaac moves the northeast statue of the east hall with Move; if it now */
/* stands on its seal (cell (40, 9)), the seal opens. */
static void scSolSanctumStatueNortheastStatueMoved(void) {
	if(scSolSanctumStatueIsActorAtCell(ACTOR_NORTHEAST_STATUE, 40, 9) != 0) {
		scSolSanctumStatueOpenNortheastSeal();
	}
}

/* Isaac moves the southwest statue of the east hall with Move; if it now */
/* stands on its seal (cell (31, 12)), the seal opens. */
static void scSolSanctumStatueSouthwestStatueMoved(void) {
	if(scSolSanctumStatueIsActorAtCell(ACTOR_SOUTHWEST_STATUE, 31, 12) != 0) {
		scSolSanctumStatueOpenSouthwestSeal();
	}
}

/* Isaac moves the southeast statue of the east hall with Move; if it now */
/* stands on its seal (cell (40, 12)), the seal opens. */
static void scSolSanctumStatueSoutheastStatueMoved(void) {
	if(scSolSanctumStatueIsActorAtCell(ACTOR_SOUTHEAST_STATUE, 40, 12) != 0) {
		scSolSanctumStatueOpenSoutheastSeal();
	}
}

/* Isaac moves the left beam statue with Move; once it stands beside the */
/* sun-disc statue (cell (14, 7)), it shines its beam on it. */
static void scSolSanctumStatueLeftBeamStatueMoved(void) {
	fdSetCellBehaviorAt(MAPGRID_LAYER_OVERHEAD, 208, 112, CELL_BEHAVIOR_NORMAL);
	if(scSolSanctumStatueIsActorAtCell(ACTOR_LEFT_BEAM_STATUE, 14, 7) != 0) {
		scSolSanctumStatueShineLeftBeam();
	}
}

/* Isaac moves the right beam statue with Move; once it stands beside the */
/* sun-disc statue (cell (21, 7)), it shines its beam on it. */
static void scSolSanctumStatueRightBeamStatueMoved(void) {
	fdSetCellBehaviorAt(MAPGRID_LAYER_OVERHEAD, 352, 112, CELL_BEHAVIOR_NORMAL);
	if(scSolSanctumStatueIsActorAtCell(ACTOR_RIGHT_BEAM_STATUE, 21, 7) != 0) {
		scSolSanctumStatueShineRightBeam();
	}
}

/* A statue Move carries is put on map layer 2 (MAPGRID_LAYER_OVERHEAD, */
/* event_commands_move.c), and a wall among that layer's cell behaviors stops */
/* it: the casts below wall off cells there for the statue alone, and its */
/* landing opens them again. */

/* Nothing visible happens as Isaac starts to cast Move on the sun-disc statue
 */
/* while standing right below it (cells (17, 8) and (18, 8)). The game makes */
/* the two cells he stands on walls for the cast, so that Move cannot draw the
 */
/* statue south on top of him; scSolSanctumStatueCenterStatueMoved opens them */
/* again when the cast ends. */
static void scSolSanctumStatuePrepareMoveOnCenterStatue(void) {
	s32 column;

	column = FIELD_CELL(fdActorGet(ACTOR_PARTY_LEADER)->x.fixed);
	if(FIELD_CELL(fdActorGet(ACTOR_PARTY_LEADER)->z.fixed) == 8) {
		if(column >= 17 && column <= 18) {
			fdSetCellBehaviorAt(MAPGRID_LAYER_OVERHEAD, 272, 128,
								CELL_BEHAVIOR_WALL);
			fdSetCellBehaviorAt(MAPGRID_LAYER_OVERHEAD, 288, 128,
								CELL_BEHAVIOR_WALL);
		}
	}
}

/* Nothing visible happens as Isaac starts to cast Move on the left beam */
/* statue while standing at the top of its track or just east of it (cells */
/* (13, 7) and (14, 7)). The game makes the track's top cell, (13, 7), a wall */
/* for the cast, so that Move cannot bring the statue up into it there; */
/* scSolSanctumStatueLeftBeamStatueMoved opens it again when the cast ends. */
static void scSolSanctumStatuePrepareMoveOnLeftBeamStatue(void) {
	s32 column = FIELD_CELL(fdActorGet(ACTOR_PARTY_LEADER)->x.fixed);
	s32 row = FIELD_CELL(fdActorGet(ACTOR_PARTY_LEADER)->z.fixed);

	if(row == 7 && (column >= 13 && column <= 14)) {
		fdSetCellBehaviorAt(MAPGRID_LAYER_OVERHEAD, 208, 112,
							CELL_BEHAVIOR_WALL);
	}
}

/* Nothing visible happens as Isaac starts to cast Move on the right beam */
/* statue while standing at the top of its track or just west of it (cells */
/* (21, 7) and (22, 7)). The game makes the track's top cell, (22, 7), a wall */
/* for the cast, so that Move cannot bring the statue up into it there; */
/* scSolSanctumStatueRightBeamStatueMoved opens it again when the cast ends. */
static void scSolSanctumStatuePrepareMoveOnRightBeamStatue(void) {
	s32 column = FIELD_CELL(fdActorGet(ACTOR_PARTY_LEADER)->x.fixed);
	s32 row = FIELD_CELL(fdActorGet(ACTOR_PARTY_LEADER)->z.fixed);

	if(row == 7 && (column >= 21 && column <= 22)) {
		fdSetCellBehaviorAt(MAPGRID_LAYER_OVERHEAD, 352, 112,
							CELL_BEHAVIOR_WALL);
	}
}

/* The sun-disc statue's Move and drop, described at its declaration above. */
static void scSolSanctumStatueCenterStatueMoved(void) {
	ACTOR *statue;
	FIXED statueZ;

	statue = fdActorGet(ACTOR_CENTER_STATUE);
	fdSetCellBehaviorAt(MAPGRID_LAYER_OVERHEAD, 272, 128, CELL_BEHAVIOR_NORMAL);
	fdSetCellBehaviorAt(MAPGRID_LAYER_OVERHEAD, 288, 128, CELL_BEHAVIOR_NORMAL);
	if(statue != NULL) {
		statueZ = statue->z.fixed;
		fdEventStart();
		if(FIELD_CELL(statueZ) == 8) {
			if(gmFlagGet(FLAG_SOL_SANCTUM_CENTER_HOLE_OPEN) == 0) {
				s32 halfCell =
					FIELD_HALF_CELL(fdActorGet(ACTOR_PARTY_LEADER)->z.fixed);

				if(halfCell >= 0 && halfCell <= 17) {
					fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 289, 158);
					fdActorGet(ACTOR_PARTY_LEADER)->facing = FACING_NORTH;
				}
			}
			if(gmFlagGet(FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING) != 0 &&
			   gmFlagGet(FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING) != 0) {
				gmFlagSet(FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED);
				/* The statue slides south into the hole;
				 * ACTOR_MOVED_CENTER_STATUE */
				/* stands in for it there and the room shakes hard, then
				 * settles. */
				fdCameraSetSpeed(2);
				fdCameraMoveTo(286, CAMERA_KEEP, 146, 1);
				fdCameraWaitForMove();
				fdActorGet(ACTOR_CENTER_STATUE)->steeringFlg &=
					~ACTOR_FACES_MOTION;
				fdActorSetSpeedAndAcceleration(ACTOR_CENTER_STATUE, 3, 1);
				statue->motionFlg = 0;
				fdActorSetSprPriority(ACTOR_CENTER_STATUE, 3);
				sySndPlay(SE_STONE_GRIND);
				fdActorSetDestination(ACTOR_CENTER_STATUE, 288, 178);
				fdEventWait(8);
				fdActorSetPos(ACTOR_MOVED_CENTER_STATUE, 288, 178);
				statue->tgtX = ACTOR_NO_TARGET;
				statue->tgtY = ACTOR_NO_TARGET;
				statue->tgtZ = ACTOR_NO_TARGET;
				statue->x.fixed = 0;
				statue->y.fixed = 0;
				statue->z.fixed = 0;
				SET_VELOCITY(statue, 0, 0, 0);
				fdActorSetPos(ACTOR_CENTER_STATUE, 0, 0);
				fdMapGridSetShake(1, 1, 1);
				fdEventWait(10);
				sySndPlay(SE_RUMBLE);
				fdMapGridSetShake(3, 3, 1);
				fdEventWait(10);
				fdMapGridSetShake(5, 5, 1);
				fdEventWait(35);
				fdMapGridSetShake(4, 4, 1);
				fdEventWait(20);
				fdMapGridSetShake(3, 3, 1);
				fdEventWait(30);
				fdMapGridSetShake(2, 2, 1);
				fdEventWait(40);
				sySndPlay(SOUND_FADE_OUT_ACTION_SE);
				fdMapGridSetShake(1, 1, 1);
				fdEventWait(10);
				fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
				fdEventWait(60);
				sySndPlay(SE_THUMP);
				if(gmFlagGet(FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN) != 0 &&
				   gmFlagGet(FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN) != 0 &&
				   gmFlagGet(FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN) != 0 &&
				   gmFlagGet(FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN) != 0) {
					gmFlagSet(FLAG_SOL_SANCTUM_ALL_SEALS_OPEN);
				}
				fdEventWait(40);
				mnFieldMessageShow(MsgSolSanctumSomethingClicked, 1);
				fdMapGridCopyCellAttributes(0, 1, 2, 1, 17, 8);
				fdMapGridCopyCellAttributes(17, 9, 2, 1, 17, 7);
			}
		}
		fdEventEnd();
	}
}

/* Isaac walks north into the sun-disc statue once Move has drawn it a cell */
/* south, and pushes it back to its place. */
static void scSolSanctumStatuePushCenterStatueNorth(void) {
	ACTOR *movingActor;

	movingActor = fdActorGet(ACTOR_CENTER_STATUE);
	if(movingActor != NULL) {
		if(FIELD_CELL(movingActor->z.fixed) == 8) {
			fdEventStart();
			sySndPlay(SE_PUSH);
			fdActorSetSpeed(ACTOR_CENTER_STATUE, 0.2);
			fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.2);
			fdActorGet(ACTOR_CENTER_STATUE)->steeringFlg &= ~ACTOR_FACES_MOTION;
			fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_PUSH);
			fdActorSetDestination(
				ACTOR_PARTY_LEADER,
				fdActorGet(ACTOR_PARTY_LEADER)->x.part[0].integer, 136);
			fdActorSetDestination(ACTOR_CENTER_STATUE, 288, 120);
			fdActorWaitForMoveById(ACTOR_CENTER_STATUE);
			fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
			fdEventEnd();
		}
	}
}

/* Isaac walks into the northwest statue of the east hall and pushes it onto */
/* its seal (cell (31, 9)), which opens. */
static void scSolSanctumStatuePushNorthwestStatue(void) {
	scSolSanctumStatuePushStatue(ACTOR_NORTHWEST_STATUE, 31, 9, 30, 9);
	scSolSanctumStatueOpenNorthwestSeal();
}

/* Isaac walks into the northeast statue of the east hall and pushes it onto */
/* its seal (cell (40, 9)), which opens. */
static void scSolSanctumStatuePushNortheastStatue(void) {
	scSolSanctumStatuePushStatue(ACTOR_NORTHEAST_STATUE, 40, 9, 41, 9);
	scSolSanctumStatueOpenNortheastSeal();
}

/* Isaac walks into the southwest statue of the east hall and pushes it onto */
/* its seal (cell (31, 12)), which opens. */
static void scSolSanctumStatuePushSouthwestStatue(void) {
	scSolSanctumStatuePushStatue(ACTOR_SOUTHWEST_STATUE, 31, 12, 30, 12);
	scSolSanctumStatueOpenSouthwestSeal();
}

/* Isaac walks into the southeast statue of the east hall and pushes it onto */
/* its seal (cell (40, 12)), which opens. */
static void scSolSanctumStatuePushSoutheastStatue(void) {
	scSolSanctumStatuePushStatue(ACTOR_SOUTHEAST_STATUE, 40, 12, 41, 12);
	scSolSanctumStatueOpenSoutheastSeal();
}

/* Walking into the left beam statue from the west slides it east in front */
/* of the center, where it shines its beam. */
static void scSolSanctumStatuePushLeftBeamStatueEast(void) {
	scSolSanctumStatuePushStatue(ACTOR_LEFT_BEAM_STATUE, 14, 7, 13, 7);
	scSolSanctumStatueShineLeftBeam();
}

/* Walking into the left beam statue from the north slides it one cell south. */
static void scSolSanctumStatuePushLeftBeamStatueSouth(void) {
	ACTOR *statue = fdActorGet(ACTOR_LEFT_BEAM_STATUE);
	s32 row;

	if(statue == NULL) {
		return;
	}

	row = FIELD_CELL(statue->z.fixed);
	scSolSanctumStatuePushStatue(ACTOR_LEFT_BEAM_STATUE, 13, row + 1, 13, row);
}

/* Walking into the left beam statue from the south slides it one cell north. */
static void scSolSanctumStatuePushLeftBeamStatueNorth(void) {
	ACTOR *statue = fdActorGet(ACTOR_LEFT_BEAM_STATUE);
	if(statue != NULL) {
		s32 row = FIELD_CELL(statue->z.fixed);
		scSolSanctumStatuePushStatue(ACTOR_LEFT_BEAM_STATUE, 13, row - 1, 13,
									 row);
	}
}

/* Walking into the right beam statue from the east does as the left one. */
static void scSolSanctumStatuePushRightBeamStatueWest(void) {
	scSolSanctumStatuePushStatue(ACTOR_RIGHT_BEAM_STATUE, 21, 7, 22, 7);
	scSolSanctumStatueShineRightBeam();
}

/* Walking into the right beam statue from the north slides it one cell south.
 */
static void scSolSanctumStatuePushRightBeamStatueSouth(void) {
	ACTOR *statue = fdActorGet(ACTOR_RIGHT_BEAM_STATUE);
	if(statue != NULL) {
		s32 row = FIELD_CELL(statue->z.fixed);
		scSolSanctumStatuePushStatue(ACTOR_RIGHT_BEAM_STATUE, 22, row + 1, 22,
									 row);
	}
}

/* Walking into the right beam statue from the south slides it one cell north.
 */
static void scSolSanctumStatuePushRightBeamStatueNorth(void) {
	ACTOR *statue = fdActorGet(ACTOR_RIGHT_BEAM_STATUE);
	if(statue != NULL) {
		s32 row = FIELD_CELL(statue->z.fixed);
		scSolSanctumStatuePushStatue(ACTOR_RIGHT_BEAM_STATUE, 22, row - 1, 22,
									 row);
	}
}

/* Moves the leader and statue together to the centers of their target cells. */
static void scSolSanctumStatuePushStatue(s32 statueId, s32 statueColumn,
										 s32 statueRow, s32 leaderColumn,
										 s32 leaderRow) {
	s32 statueX;
	s32 statueZ;

	fdEventStart();
	sySndPlay(SE_PUSH);
	fdActorSetSpeed(statueId, 0.2);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.2);
	fdActorGet(statueId)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_PUSH);
	fdActorSetDestination(ACTOR_PARTY_LEADER, (leaderColumn << 4) + 8,
						  (leaderRow << 4) + 8);
	statueX = statueColumn << 4;
	statueZ = statueRow << 4;
	fdActorSetDestination(statueId, statueX + 8, statueZ + 8);
	fdActorWaitForMoveById(statueId);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	fdEventEnd();
}

/* Returns 1 when the actor stands in the given map cell. */
static s32 scSolSanctumStatueIsActorAtCell(s32 actorId, s32 column, s32 row) {
	ACTOR *actor = fdActorGet(actorId);
	if(actor == NULL || FIELD_CELL(actor->x.fixed) != column) {
		return 0;
	}
	if(FIELD_CELL(actor->z.fixed) != row) {
		return 0;
	}
	return 1;
}

/* Opens the northwest seal (scSolSanctumStatueOpenSeal): the cells it lights,
 */
/* its flag, where the leader jumps back to and where its statue slides back to
 */
/* if the seal closes again. */
static void scSolSanctumStatueOpenNorthwestSeal(void) {
	SEAL_OPENING *seal = &sSealOpening;
	CELL_COPY_SET(seal->open, 0, 55, 32, 40, 4, 3);
	CELL_COPY_SET(seal->pulseA, 2, 30, 34, 10, 2, 1);
	CELL_COPY_SET(seal->pulseB, 2, 28, 34, 10, 2, 1);
	CELL_COPY_SET(seal->opened, 2, 30, 16, 10, 2, 1);
	seal->flg = FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN;
	seal->facing = FACING_SOUTH;
	seal->dstX = 500;
	seal->dstZ = 132;
	CELL_COPY_SET(seal->closed[0], 8, 55, 32, 40, 4, 3);
	CELL_COPY_SET(seal->closed[1], 2, 30, 34, 10, 2, 1);
	CELL_COPY_SET(seal->closed[2], 2, 28, 16, 10, 2, 1);
	seal->actorId = ACTOR_NORTHWEST_STATUE;
	seal->actorX = 488;
	seal->actorZ = 152;
	scSolSanctumStatueOpenSeal();
}

/* Opens the northeast seal (scSolSanctumStatueOpenSeal): the cells it lights,
 */
/* its flag, where the leader jumps back to and where its statue slides back to
 */
/* if the seal closes again. */
static void scSolSanctumStatueOpenNortheastSeal(void) {
	SEAL_OPENING *seal = &sSealOpening;
	CELL_COPY_SET(seal->open, 4, 55, 36, 40, 4, 3);
	CELL_COPY_SET(seal->pulseA, 4, 30, 36, 10, 2, 1);
	CELL_COPY_SET(seal->pulseB, 4, 28, 36, 10, 2, 1);
	CELL_COPY_SET(seal->opened, 4, 30, 18, 10, 2, 1);
	seal->flg = FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN;
	seal->facing = FACING_SOUTH;
	seal->dstX = 654;
	seal->dstZ = 132;
	CELL_COPY_SET(seal->closed[0], 12, 55, 36, 40, 4, 3);
	CELL_COPY_SET(seal->closed[1], 4, 30, 36, 10, 2, 1);
	CELL_COPY_SET(seal->closed[2], 4, 28, 18, 10, 2, 1);
	seal->actorId = ACTOR_NORTHEAST_STATUE;
	seal->actorX = 664;
	seal->actorZ = 152;
	scSolSanctumStatueOpenSeal();
}

/* Opens the southwest seal (scSolSanctumStatueOpenSeal): the cells it lights,
 */
/* its flag, where the leader jumps back to and where its statue slides back to
 */
/* if the seal closes again. */
static void scSolSanctumStatueOpenSouthwestSeal(void) {
	SEAL_OPENING *seal = &sSealOpening;
	CELL_COPY_SET(seal->open, 0, 58, 32, 43, 4, 1);
	CELL_COPY_SET(seal->pulseA, 2, 31, 34, 11, 2, 1);
	CELL_COPY_SET(seal->pulseB, 2, 29, 34, 11, 2, 1);
	CELL_COPY_SET(seal->opened, 2, 31, 16, 11, 2, 1);
	seal->flg = FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN;
	seal->facing = FACING_NORTH;
	seal->dstX = 500;
	seal->dstZ = 216;
	CELL_COPY_SET(seal->closed[0], 8, 58, 32, 43, 4, 1);
	CELL_COPY_SET(seal->closed[1], 2, 31, 34, 11, 2, 1);
	CELL_COPY_SET(seal->closed[2], 2, 29, 16, 11, 2, 1);
	seal->actorId = ACTOR_SOUTHWEST_STATUE;
	seal->actorX = 488;
	seal->actorZ = 200;
	scSolSanctumStatueOpenSeal();
}

/* Opens the southeast seal (scSolSanctumStatueOpenSeal): the cells it lights,
 */
/* its flag, where the leader jumps back to and where its statue slides back to
 */
/* if the seal closes again. */
static void scSolSanctumStatueOpenSoutheastSeal(void) {
	SEAL_OPENING *seal = &sSealOpening;
	CELL_COPY_SET(seal->open, 4, 58, 36, 43, 4, 1);
	CELL_COPY_SET(seal->pulseA, 4, 31, 36, 11, 2, 1);
	CELL_COPY_SET(seal->pulseB, 4, 29, 36, 11, 2, 1);
	CELL_COPY_SET(seal->opened, 4, 31, 18, 11, 2, 1);
	seal->flg = FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN;
	seal->facing = FACING_NORTH;
	seal->dstX = 654;
	seal->dstZ = 216;
	CELL_COPY_SET(seal->closed[0], 12, 58, 36, 43, 4, 1);
	CELL_COPY_SET(seal->closed[1], 4, 31, 36, 11, 2, 1);
	CELL_COPY_SET(seal->closed[2], 4, 29, 18, 11, 2, 1);
	seal->actorId = ACTOR_SOUTHEAST_STATUE;
	seal->actorX = 664;
	seal->actorZ = 200;
	scSolSanctumStatueOpenSeal();
}

/* Copies a seal's cell copy (CELL_COPY) onto the map. */
#define COPY_CELLS(copy)                                                       \
	fdMapGridCopyCellsTo((copy).srcX, (copy).srcY, (copy).dstX, (copy).dstY,   \
						 (copy).width, (copy).height)

/* The opened seal pulses this many times, a glitter at each. */
#define SEAL_PULSES 20

/* Animates the seal and either advances the story or resets its statue. */
static void scSolSanctumStatueOpenSeal(void) {
	SEAL_OPENING *seal;
	s32 exitResult;
	s32 pulse;

	exitResult = 0;
	fdEventStart();
	if(!gmFlagGet(FLAG_SOL_SANCTUM_ALL_SEALS_OPEN)) {
		fdCameraSetSpeed(2);
		fdCameraMoveTo(576, CAMERA_KEEP, 172, 1);
		fdCameraWaitForMove();
		sySndPlay(SE_SOL_SANCTUM_LIGHT_UP);
		COPY_CELLS(sSealOpening.open);
		for(pulse = 0; pulse != SEAL_PULSES; pulse++) {
			sySndPlay(SE_GLITTER);
			COPY_CELLS(sSealOpening.pulseA);
			fdEventWait(4);
			sySndPlay(SE_GLITTER);
			COPY_CELLS(sSealOpening.pulseB);
			fdEventWait(4);
		}
		seal = &sSealOpening;
		COPY_CELLS(seal->opened);
		gmFlagSet(seal->flg);
		exitResult = scSolSanctumStatueSelectStoryExit();
		if(exitResult == -1) {
			if(!gmFlagGet(FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED)) {
				fdCameraFollowActor(ACTOR_PARTY_LEADER, 1);
				fdActorGet(ACTOR_PARTY_LEADER)->facing = seal->facing;
				fdActorSetSpeedAndAcceleration(ACTOR_PARTY_LEADER, 2, 2);
				fdActorGet(ACTOR_PARTY_LEADER)->steeringFlg &=
					~ACTOR_FACES_MOTION;
				fdActorJump(ACTOR_PARTY_LEADER, 4, 0);
				fdActorSetDestination(ACTOR_PARTY_LEADER, seal->dstX,
									  seal->dstZ);
				COPY_CELLS(seal->closed[0]);
				COPY_CELLS(seal->closed[1]);
				COPY_CELLS(seal->closed[2]);
				fdActorGet(seal->actorId)->steeringFlg &= ~ACTOR_FACES_MOTION;
				fdActorMoveToAndWait(seal->actorId, seal->actorX, seal->actorZ);
				fdActorGet(ACTOR_PARTY_LEADER)->steeringFlg |=
					ACTOR_FACES_MOTION;
				gmFlagClear(seal->flg);
			}
		}
		else if(exitResult == 0 &&
				gmFlagGet(FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED)) {
			/* Unused: scSolSanctumStatueSelectStoryExit returns only 1 or -1,
			 * so */
			/* this branch cannot run. */
			if(gmFlagGet(FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN) &&
			   gmFlagGet(FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN) &&
			   gmFlagGet(FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN)) {
				if(!fdEventGetFlag(FLAG_SOL_SANCTUM_ALL_SEALS_OPEN)) {
					gmFlagSet(FLAG_SOL_SANCTUM_ALL_SEALS_OPEN);
					scSolSanctumStatueUnusedPutOutSeals();
				}
			}
			else if(gmFlagGet(FLAG_SOL_SANCTUM_FALLING_LIGHTS_SEEN)) {
				syImportEventSetExitRequest(
					SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_LUNA_SOL_ROOMS_LIGHTS_SHOWN);
				exitResult = 1;
			}
		}
	}
	if(exitResult == 1) {
		fdEventFinishScrTransition();
		fdEventWaitTransitionFrames();
	}
	fdEventEnd();
}

/* Draws the left beam and opens the floor when the right beam is also lit. */
static void scSolSanctumStatueShineLeftBeam(void) {
	fdEventStart();
	if(gmFlagGet(FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED) == 0) {
		if(fdEventGetFlag(FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING) == 0) {
			fdCameraSetSpeed(2);
			fdCameraMoveTo(286, CAMERA_KEEP, 146, 1);
			fdCameraWaitForMove();
			sySndPlay(SE_SOL_SANCTUM_LIGHT_UP);
			DRAW_LEFT_BEAM();
			/* With the other beam already shining, the two now cross. */
			if(fdEventGetFlag(FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING) != 0) {
				DRAW_CROSSED_BEAMS();
			}
			fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
			fdEventWait(30);
			gmFlagSet(FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING);
			if(gmFlagGet(FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING) != 0) {
				scSolSanctumStatueOpenCenterHole();
			}
		}
	}
	fdEventEnd();
}

/* Draws the right beam and opens the floor when the left beam is also lit. */
static void scSolSanctumStatueShineRightBeam(void) {
	fdEventStart();
	if(gmFlagGet(FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED) == 0) {
		if(fdEventGetFlag(FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING) == 0) {
			fdCameraSetSpeed(2);
			fdCameraMoveTo(286, CAMERA_KEEP, 146, 1);
			fdCameraWaitForMove();
			sySndPlay(SE_SOL_SANCTUM_LIGHT_UP);
			DRAW_RIGHT_BEAM();
			/* With the other beam already shining, the two now cross. */
			if(fdEventGetFlag(FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING) != 0) {
				DRAW_CROSSED_BEAMS();
			}
			fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
			fdEventWait(30);
			gmFlagSet(FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING);
			if(gmFlagGet(FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING) != 0) {
				scSolSanctumStatueOpenCenterHole();
			}
		}
	}
	fdEventEnd();
}

/* Shows the next message of the event, then waits. */
static void scSolSanctumStatueSayThenWait(s32 speaker, s32 frames) {
	fdEventShowMessage(speaker, 0);
	fdEventWait(frames);
}

SCENE_ENTRANCE SolSanctumStatue_Entrances[] = {
	{SOL_SANCTUM_STATUE_FROM_VALE_PLAZA, CONDITION_ALWAYS, 287, 0, 176,
	 FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS, CONDITION_ALWAYS, 646,
	 0, 329, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_PSYNERGY_STONE, CONDITION_ALWAYS, 647,
	 0, 470, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_DISARMED,
	 CONDITION_ALWAYS, 576, 0, 136, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_TRIGGERED,
	 CONDITION_ALWAYS, 576, 0, 136, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS_LIGHTS_SHOWN,
	 CONDITION_ALWAYS, 576, 0, 136, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS_UNUSED,
	 CONDITION_ALWAYS, 576, 0, 136, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{SOL_SANCTUM_STATUE_DEBUG_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS, CONDITION_ALWAYS,
	 576, 0, 136, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

u32 SolSanctumStatue_Exits[] = {
	SCENE_EXITS(SolSanctumStatue),
	SCENE_EXIT(SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_LUNA_SOL_ROOMS,
			   SolSanctumLunaSolRooms,
			   SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE),
	SCENE_EXIT(SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_PSYNERGY_STONE,
			   SolSanctumPsynergyStone,
			   SOL_SANCTUM_PSYNERGY_STONE_FROM_SOL_SANCTUM_STATUE),
	SCENE_EXIT(
		SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_DISARMED,
		SolSanctumLunaSolRooms,
		SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE_TRAP_DISARMED),
	SCENE_EXIT(
		SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_TRIGGERED,
		SolSanctumLunaSolRooms,
		SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE_TRAP_TRIGGERED),
	SCENE_EXIT(SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_LUNA_SOL_ROOMS_LIGHTS_SHOWN,
			   SolSanctumLunaSolRooms,
			   SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE_LIGHTS_SHOWN),
	SCENE_EXIT(SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_LUNA_SOL_ROOMS_UNUSED,
			   SolSanctumLunaSolRooms,
			   SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE_UNUSED),
	EXIT_TO_BATTLE(SOL_SANCTUM_STATUE_TO_BATTLE, 1),
	SCENE_EXITS_END,
};

/* clang-format off */
ACTOR_PLACE SolSanctumStatue_ActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        1608, 0, 1608, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WINGED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        488, 0, 152, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WINGED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        216, 0, 152, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_WINGED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        664, 0, 152, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WINGED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        360, 0, 152, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_WINGED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        488, 0, 200, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WINGED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        216, 0, 216, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_WINGED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        664, 0, 200, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WINGED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        360, 0, 216, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_SUN_DISC_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        288, 0, 120, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SUN_DISC, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        1608, 0, 1608, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        560, 0, 136, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        592, 0, 136, FACING_SOUTH, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};
/* clang-format on */

TRIGGER SolSanctumStatue_Triggers[] = {
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN,
	 SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_LUNA_SOL_ROOMS, CONDITION_ALWAYS,
	 SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_LUNA_SOL_ROOMS},
	{TRIGGER_EXIT, SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_PSYNERGY_STONE,
	 CONDITION_ALWAYS, SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_PSYNERGY_STONE},
	{STEP_TRIGGER_WALKING, CELL_BEHAVIOR_SOL_SANCTUM_STATUE_JUMP_NORTH,
	 CONDITION_ALWAYS, EVENT(scSolSanctumStatueJumpNorth)},
	{STEP_TRIGGER_WALKING, CELL_BEHAVIOR_SOL_SANCTUM_STATUE_JUMP_SOUTH,
	 CONDITION_ALWAYS, EVENT(scSolSanctumStatueJumpSouth)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_NORTHWEST_STATUE,
	 FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN,
	 EVENT(scSolSanctumStatueNorthwestStatueMoved)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_NORTHEAST_STATUE,
	 FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN,
	 EVENT(scSolSanctumStatueNortheastStatueMoved)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SOUTHWEST_STATUE,
	 FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN,
	 EVENT(scSolSanctumStatueSouthwestStatueMoved)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SOUTHEAST_STATUE,
	 FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN,
	 EVENT(scSolSanctumStatueSoutheastStatueMoved)},
	{STEP_TRIGGER_WALKING_INTO(FACING_EAST),
	 CELL_BEHAVIOR_SOL_SANCTUM_STATUE_NORTHWEST_STATUE,
	 FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN,
	 EVENT(scSolSanctumStatuePushNorthwestStatue)},
	{STEP_TRIGGER_WALKING_INTO(FACING_WEST),
	 CELL_BEHAVIOR_SOL_SANCTUM_STATUE_NORTHEAST_STATUE,
	 FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN,
	 EVENT(scSolSanctumStatuePushNortheastStatue)},
	{STEP_TRIGGER_WALKING_INTO(FACING_EAST),
	 CELL_BEHAVIOR_SOL_SANCTUM_STATUE_SOUTHWEST_STATUE,
	 FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN,
	 EVENT(scSolSanctumStatuePushSouthwestStatue)},
	{STEP_TRIGGER_WALKING_INTO(FACING_WEST),
	 CELL_BEHAVIOR_SOL_SANCTUM_STATUE_SOUTHEAST_STATUE,
	 FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN,
	 EVENT(scSolSanctumStatuePushSoutheastStatue)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
	 ACTOR_LEFT_BEAM_STATUE, FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING,
	 EVENT(scSolSanctumStatuePrepareMoveOnLeftBeamStatue)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LEFT_BEAM_STATUE,
	 FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING,
	 EVENT(scSolSanctumStatueLeftBeamStatueMoved)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
	 ACTOR_RIGHT_BEAM_STATUE, FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING,
	 EVENT(scSolSanctumStatuePrepareMoveOnRightBeamStatue)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_RIGHT_BEAM_STATUE,
	 FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING,
	 EVENT(scSolSanctumStatueRightBeamStatueMoved)},
	{STEP_TRIGGER_WALKING_INTO(FACING_EAST),
	 CELL_BEHAVIOR_SOL_SANCTUM_STATUE_LEFT_TRACK_OUTSIDE_TOP,
	 FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING,
	 EVENT(scSolSanctumStatuePushLeftBeamStatueEast)},
	{STEP_TRIGGER_WALKING_INTO(FACING_EAST),
	 CELL_BEHAVIOR_SOL_SANCTUM_STATUE_LEFT_TRACK_TOP,
	 FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING,
	 EVENT(scSolSanctumStatuePushLeftBeamStatueEast)},
	{STEP_TRIGGER_WALKING_INTO(FACING_SOUTH),
	 CELL_BEHAVIOR_SOL_SANCTUM_STATUE_LEFT_TRACK_TOP,
	 FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING,
	 EVENT(scSolSanctumStatuePushLeftBeamStatueSouth)},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
	 CELL_BEHAVIOR_SOL_SANCTUM_STATUE_LEFT_TRACK_THIRD,
	 FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING,
	 EVENT(scSolSanctumStatuePushLeftBeamStatueNorth)},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
	 CELL_BEHAVIOR_SOL_SANCTUM_STATUE_LEFT_TRACK_BOTTOM,
	 FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING,
	 EVENT(scSolSanctumStatuePushLeftBeamStatueNorth)},
	{STEP_TRIGGER_WALKING_INTO(FACING_WEST),
	 CELL_BEHAVIOR_SOL_SANCTUM_STATUE_RIGHT_TRACK_OUTSIDE_TOP,
	 FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING,
	 EVENT(scSolSanctumStatuePushRightBeamStatueWest)},
	{STEP_TRIGGER_WALKING_INTO(FACING_WEST),
	 CELL_BEHAVIOR_SOL_SANCTUM_STATUE_RIGHT_TRACK_TOP,
	 FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING,
	 EVENT(scSolSanctumStatuePushRightBeamStatueWest)},
	{STEP_TRIGGER_WALKING_INTO(FACING_SOUTH),
	 CELL_BEHAVIOR_SOL_SANCTUM_STATUE_RIGHT_TRACK_TOP,
	 FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING,
	 EVENT(scSolSanctumStatuePushRightBeamStatueSouth)},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
	 CELL_BEHAVIOR_SOL_SANCTUM_STATUE_RIGHT_TRACK_THIRD,
	 FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING,
	 EVENT(scSolSanctumStatuePushRightBeamStatueNorth)},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
	 CELL_BEHAVIOR_SOL_SANCTUM_STATUE_RIGHT_TRACK_BOTTOM,
	 FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING,
	 EVENT(scSolSanctumStatuePushRightBeamStatueNorth)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
	 ACTOR_CENTER_STATUE, FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED,
	 EVENT(scSolSanctumStatuePrepareMoveOnCenterStatue)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_CENTER_STATUE,
	 FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED,
	 EVENT(scSolSanctumStatueCenterStatueMoved)},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
	 CELL_BEHAVIOR_SOL_SANCTUM_STATUE_CENTER_STATUE,
	 FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED,
	 EVENT(scSolSanctumStatuePushCenterStatueNorth)},
	{TRIGGER_TABLE_END},
};

/* The clouds' scroll, SCROLL_OFFSET_STEP more each frame. */
s32 SolSanctumStatue_ScrollOffset = 0;
