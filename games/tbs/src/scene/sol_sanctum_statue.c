// Sol Sanctum (SceneIdSolSanctumStatue), the room of what Kraden calls the statue puzzle, off the
// Luna and Sol rooms (SceneIdSolSanctumLunaSolRooms) and leading on to a room with a Psynergy Stone
// (SceneIdSolSanctumPsynergyStone).
//
// Eight winged statues stand around a ninth, the center statue, which bears a sun disc. Four can be
// pushed, or moved with Move, onto seals in the floor. Two others, pushed in front of the center
// statue, shine a beam at it. The last two never move.
//
// Walking into a statue pushes it a metatile (scSolSanctumStatuePushStatue), and Move ends in a
// check of where it stands.
//
// Each move's cue carries the flag of what it achieves (a seal open, a beam shining, the center
// statue moved), so a statue stops answering once it has done its part. scInit redraws the room
// from those story flags on every entry.
//
// The puzzle is a trap, and the room follows the story flags in order:
// - Until the center statue has moved (FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED),
//       the first seal opened cuts away to the Luna and Sol rooms, where the
// trap is sprung
//       (SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_TRIGGERED,
//       FLAG_SOL_SANCTUM_FALLING_LIGHTS_SEEN). Kraden then comes here to
// explain that moving one statue springs the sanctum's trap
//       (scSolSanctumStatueExplainStatuePuzzle,
//       FLAG_SOL_SANCTUM_STATUE_PUZZLE_EXPLAINED). From then on a seal opened
//       too soon closes again. Its statue slides back and the leader jumps
// clear.
// - With both beams shining, the floor before the center statue opens
//       (scSolSanctumStatueOpenCenterHole), and the center statue, moved south
//       with Move, drops into it (scSolSanctumStatueCenterStatueMoved).
// - From then on the seals stay open. The first one opened cuts away to the
//       Luna and Sol rooms with the trap disarmed
//       (SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_DISARMED), and
//       Kraden comes here to find the hole
// (scSolSanctumStatueExplainTrapDisarmed,
//       FLAG_SOL_SANCTUM_TRAP_DISARMED_EXPLAINED). Once all four seals are open
//       (FLAG_SOL_SANCTUM_ALL_SEALS_OPEN), the way on is found in the Luna and
// Sol rooms.
//
// Both of Kraden's visits hold Jenna and Garet in place beside the leader, then walk them back into
// the party. Any other arrival hides them.
//
// Retreat does not work here.
//
// An event draws three rows of sprites that drift sideways at different speeds
// (scSolSanctumStatueUpdateScrollingSprites). They are the clouds of the sky seen between the
// pillars of the colonnade along the room's north wall. They sit 80 pixels down the map at the
// lowest priority, so they pass in front of the sky and behind the pillars. The rows move an
// eighth, a quarter and a half of a pixel a tick, the fastest drawn lowest
// (https://alchemy.pascalpixel.com/places/sol-sanctum-statue/sky-rows.gif: the top of the screen
// twice its size, above the sprites alone and the backgrounds alone, the clouds drifting behind the
// pillars).
//
// Once Mt. Aleph has erupted (FLAG_SOL_SANCTUM_ERUPTED) the room rumbles and shakes, as the rest of
// the sanctum does.

#include "ability_ids.h"
#include "flags.h"
#include "macros.h"
#include "debug/debug.h"
#include "field/script_inline.h"
#include "field/map_grid_scroll.h"
#include "graphics/graphics.h"
#include "scene/walk_into_leader.h"
#include "sound/sound_driver_symbols.h"
#include "text/msg_ids.h"

extern SCENE_ENTRANCE SolSanctumStatueEntrances[];
extern Uint32         SolSanctumStatueExits[];
extern ACTOR_PLACE    SolSanctumStatueActorPlacements[];
extern CUE            SolSanctumStatueCues[];
extern Sint32         SolSanctumStatueScrollOffset;

// Both beams have opened the hole in the floor before the center statue. Until then, Move on the
// center statue first walks the leader clear of it.
#define FLAG_SOL_SANCTUM_CENTER_HOLE_OPEN        FLAG_SCENE_TEMP_07

// The west hall's beams and hole, drawn from spare metatiles the map keeps: on map layer 1 (the
// map's metatiles from (0, 32)) the beams, from its row 27 on, and on the ground the open hole,
// from metatile (0, 28). The left beam statue's blue beam, from the statue beside the sun-disc
// statue to the floor before it: layer 1's metatiles (15, 6) to (18, 8).
#define DRAW_LEFT_BEAM()                         fdMapGridCopyMetatilesTo(0, 59, 15, 38, 4, 3)
// The right beam statue's beam: layer 1's metatiles (17, 6) to (20, 8).
#define DRAW_RIGHT_BEAM()                        fdMapGridCopyMetatilesTo(4, 59, 17, 38, 4, 3)
// Both beams, where they cross over the floor before the sun-disc statue: layer 1's metatiles (17,
// 7) to (18, 8).
#define DRAW_CROSSED_BEAMS()                     fdMapGridCopyMetatilesTo(8, 60, 17, 39, 2, 2)
// The hole open in the floor before the sun-disc statue: metatiles (17, 8) and (18, 8).
#define DRAW_CENTER_HOLE()                       fdMapGridCopyMetatilesTo(0, 28, 17, 8, 2, 1)

// Area flags: gmArrive clears 0x300 to 0x3ff when the party enters another area, and other areas
// use the numbers for their own. Set once Kraden has found the hole and explained the disarmed
// trap.
#define FLAG_SOL_SANCTUM_TRAP_DISARMED_EXPLAINED FLAG_AREA_TEMP_0A
// Set once Kraden has explained the statue puzzle's trap.
#define FLAG_SOL_SANCTUM_STATUE_PUZZLE_EXPLAINED FLAG_AREA_TEMP_0B

// How far below the other two the front row of clouds, the fastest, is drawn: 8 pixels, and 6 in
// the French, Spanish and Italian editions.
#define SOL_SANCTUM_STATUE_FRONT_CLOUD_ROW_Y_OFFSET                                                \
	WHEN(SOL_SANCTUM_STATUE_FRONT_CLOUDS_RAISED, 6, 8)

// A row of sky clouds: nine sprites 32 pixels wide, 288 pixels across the 240-pixel screen as the
// row scrolls.
#define SOL_SANCTUM_STATUE_ROW_SPRITE_COUNT 9
#define SOL_SANCTUM_STATUE_SPRITE_WIDTH     32

// The basic sprite palettes (syPaletteLoadBasicSpriteColors) the three rows of clouds are drawn in,
// back to front.
enum
{
	BACK_CLOUD_ROW_PALETTE   = 10,
	MIDDLE_CLOUD_ROW_PALETTE = 13,
	FRONT_CLOUD_ROW_PALETTE  = 0
};

// The metatile behaviors the step cues answer.
//
// They are the jump spots and the four seal statues' and the center statue's metatiles; the beam
// statues' tracks follow in their own enum.
typedef enum SOL_SANCTUM_STATUE_METATILE_BEHAVIOR
{
	METATILE_BEHAVIOR_SOL_SANCTUM_STATUE_NORTHWEST_STATUE = 5,
	METATILE_BEHAVIOR_SOL_SANCTUM_STATUE_NORTHEAST_STATUE = 6,
	METATILE_BEHAVIOR_SOL_SANCTUM_STATUE_SOUTHWEST_STATUE = 7,
	METATILE_BEHAVIOR_SOL_SANCTUM_STATUE_SOUTHEAST_STATUE = 8,
	// a jump spot, crossed northward
	METATILE_BEHAVIOR_SOL_SANCTUM_STATUE_JUMP_NORTH       = 10,
	// a jump spot, crossed southward
	METATILE_BEHAVIOR_SOL_SANCTUM_STATUE_JUMP_SOUTH       = 11,
	METATILE_BEHAVIOR_SOL_SANCTUM_STATUE_CENTER_STATUE    = 32
} SOL_SANCTUM_STATUE_METATILE_BEHAVIOR;

// The metatiles of the beam statues' tracks, where Isaac stands to push them.
//
// Each track runs north from the statue's start, metatiles (13, 7) to (13, 10) for the left statue
// and (22, 7) to (22, 10) for the right, with one more metatile beside its top on the outer side.
// From the top the statue is pushed inward to stand beside the sun-disc statue.
typedef enum SOL_SANCTUM_STATUE_BEAM_TRACK
{
	METATILE_BEHAVIOR_SOL_SANCTUM_STATUE_LEFT_TRACK_OUTSIDE_TOP = 20,
	METATILE_BEHAVIOR_SOL_SANCTUM_STATUE_LEFT_TRACK_TOP,
	METATILE_BEHAVIOR_SOL_SANCTUM_STATUE_LEFT_TRACK_SECOND,
	METATILE_BEHAVIOR_SOL_SANCTUM_STATUE_LEFT_TRACK_THIRD,
	METATILE_BEHAVIOR_SOL_SANCTUM_STATUE_LEFT_TRACK_BOTTOM,
	METATILE_BEHAVIOR_SOL_SANCTUM_STATUE_RIGHT_TRACK_OUTSIDE_TOP,
	METATILE_BEHAVIOR_SOL_SANCTUM_STATUE_RIGHT_TRACK_TOP,
	METATILE_BEHAVIOR_SOL_SANCTUM_STATUE_RIGHT_TRACK_SECOND,
	METATILE_BEHAVIOR_SOL_SANCTUM_STATUE_RIGHT_TRACK_THIRD,
	METATILE_BEHAVIOR_SOL_SANCTUM_STATUE_RIGHT_TRACK_BOTTOM
} SOL_SANCTUM_STATUE_BEAM_TRACK;

static void scSolSanctumStatueInitScrollingSprites(void);
// Isaac uses Move to draw the sun-disc statue one metatile south from its place at the north of the
// west hall (metatiles (17, 7) and (18, 7)). Once both beam statues shine on it, it slides on south
// into the hole that has opened in the sun on the floor and settles in it with a grinding of stone;
// the room shakes hard, settles, and "Something clicked!" From then on the seals in the east hall
// stay open.
//
// With the beams not both shining, the statue only moves the one metatile, and if Isaac stood in
// its way he first steps back south of it. A beam completing the pair runs this too, dropping a
// statue Move has already brought down.
static void scSolSanctumStatueCenterStatueMoved(void);
static void scSolSanctumStatuePushStatue(
	Sint32 statueId, Sint32 statueColumn, Sint32 statueRow, Sint32 leaderColumn, Sint32 leaderRow);
static Sint32 scSolSanctumStatueIsActorAtMetatile(Sint32 actorId, Sint32 column, Sint32 row);
static void   scSolSanctumStatueOpenNorthwestSeal(void);
static void   scSolSanctumStatueOpenNortheastSeal(void);
static void   scSolSanctumStatueOpenSouthwestSeal(void);
static void   scSolSanctumStatueOpenSoutheastSeal(void);
static void   scSolSanctumStatueOpenSeal(void);
static void   scSolSanctumStatueShineLeftBeam(void);
static void   scSolSanctumStatueShineRightBeam(void);
static void   scSolSanctumStatueSayThenWait(Sint32 speaker, Sint32 ticks);

// A block of metatiles copied from one place in the map grid to another.
typedef struct METATILE_COPY
{
	Sint32 srcX;   // map metatiles
	Sint32 srcY;   // map metatiles
	Sint32 destX;  // map metatiles
	Sint32 destY;  // map metatiles
	Sint32 width;  // map metatiles
	Sint32 height; // map metatiles
} METATILE_COPY;

// Sets a metatile copy: columns by rows metatiles from (x, y) to (toX, toY).
#define METATILE_COPY_SET(copy, x, y, toX, toY, columns, rows)                                     \
	((copy).srcX   = (x),                                                                          \
	 (copy).srcY   = (y),                                                                          \
	 (copy).destX  = (toX),                                                                        \
	 (copy).destY  = (toY),                                                                        \
	 (copy).width  = (columns),                                                                    \
	 (copy).height = (rows))

// The seal being opened.
//
// It holds the seal's metatile animations, its flag, where the leader jumps back to, the metatiles
// that close it again (`closed`) and where its statue returns if the story does not go on.
typedef struct SEAL_OPENING
{
	METATILE_COPY open;
	METATILE_COPY pulseA;
	METATILE_COPY pulseB;
	METATILE_COPY opened;
	Sint32        flags;
	Sint32        facing;    // 1/65536 turns
	Sint32        destX;     // pixels
	Sint32        destZ;     // pixels
	METATILE_COPY closed[3];
	Sint32        actorId;
	Sint32        actorX;    // pixels
	Sint32        actorZ;    // pixels
	Sint32        unused[3]; // never read or written; the bss keeps its 12 bytes
} SEAL_OPENING;

STATIC_ASSERT(offsetof(SEAL_OPENING, unused) == 196);

// The placed actors, numbered in the order of SolSanctumStatueActorPlacements.
//
// They are Kraden, who stays out of sight until his two visits, the eight statues, and the center
// statue in its two places. Jenna and Garet, placed after them, keep their own slots.
//
// Of the statues, four are pushed onto seals and two shine beams at the center. The other two never
// move.
typedef enum SOL_SANCTUM_STATUE_ACTOR_ID
{
	ACTOR_KRADEN = ACTOR_FIRST_PLACED,
	ACTOR_NORTHWEST_STATUE,
	ACTOR_LEFT_BEAM_STATUE,
	ACTOR_NORTHEAST_STATUE,
	ACTOR_RIGHT_BEAM_STATUE,
	ACTOR_SOUTHWEST_STATUE,
	// South of the left beam statue, and moved by no script.
	ACTOR_LEFT_STILL_STATUE,
	ACTOR_SOUTHEAST_STATUE,
	// South of the right beam statue, and moved by no script.
	ACTOR_RIGHT_STILL_STATUE,
	ACTOR_CENTER_STATUE,
	// Stands in the hole in the floor once the center statue has dropped in.
	ACTOR_MOVED_CENTER_STATUE
} SOL_SANCTUM_STATUE_ACTOR_ID;

// The seal the last statue moved has set up to open.
static SEAL_OPENING SealOpening ALIGNED(16);

// Initialization writes each three-word OAM packet, and drawing updates its fields.
static union
{
	ANIMATION_SPR_PART parts[3 * SOL_SANCTUM_STATUE_ROW_SPRITE_COUNT];
	Sint32             words[3 * SOL_SANCTUM_STATUE_ROW_SPRITE_COUNT * 3];
} ScrollingSprites;

const SCENE_ENTRANCE *scGetEntrances(void)
{
	return SolSanctumStatueEntrances;
}

const SCENE_EXIT_REGION *scGetExitRegions(void)
{
	return NULL;
}

const Uint32 *scGetExits(void)
{
	return SolSanctumStatueExits;
}

// Returns Kraden, the eight statues, the center statue and its moved copy (ACTOR_KRADEN to
// ACTOR_MOVED_CENTER_STATUE), then Jenna and Garet.
const ACTOR_PLACE *scGetActorPlacements(void)
{
	return SolSanctumStatueActorPlacements;
}

// Returns the statues' Move and push scripts, each row only until its seal or beam is done.
const CUE *scGetCues(void)
{
	return SolSanctumStatueCues;
}

// Chooses the script in the Luna and Sol rooms that a newly opened seal leads to, and requests its
// exit.
//
// Returns 1 when it requests an exit, otherwise -1.
static Sint32 scSolSanctumStatueSelectStoryExit(void)
{
	if (gmFlagGet(FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED) != 0)
	{
		if (gmFlagGet(FLAG_SOL_SANCTUM_KRADEN_SAID_FOUND_SOMETHING) == 0)
		{
			syImportScriptSetExitRequest(
				SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_DISARMED);
			return 1;
		}
	}
	else if (gmFlagGet(FLAG_SOL_SANCTUM_FALLING_LIGHTS_SEEN) == 0)
	{
		syImportScriptSetExitRequest(
			SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_TRIGGERED);
		return 1;
	}
	return -1;
}

// The floor before the center statue flashes this many times, eight ticks each, as it opens.
#define CENTER_HOLE_FLASHES 22

// Animates the floor opening before the center statue once both beams shine, then lets the statue
// drop in if Move has already brought it there.
static void scSolSanctumStatueOpenCenterHole(void)
{
	Sint32 flash;

	DRAW_CENTER_HOLE();
	sySoundPlay(SE_SPARKLE_FALL);
	for (flash = 0; flash != CENTER_HOLE_FLASHES; flash++)
	{
		// The crossed beams' lower metatiles flash as the floor opens.
		fdMapGridCopyMetatilesTo(10, 61, 17, 40, 2, 1);
		fdScriptWait(4);
		fdMapGridCopyMetatilesTo(8, 61, 17, 40, 2, 1);
		fdScriptWait(4);
	}
	DRAW_LEFT_BEAM();
	DRAW_RIGHT_BEAM();
	DRAW_CROSSED_BEAMS();
	fdMapGridCopyMetatileAttributes(0, 0, 2, 1, 17, 8);
	gmFlagSet(FLAG_SOL_SANCTUM_CENTER_HOLE_OPEN);
	scSolSanctumStatueCenterStatueMoved();
}

// Kraden's second visit, a script in which he comes to find the hole in the floor.
static void scSolSanctumStatueExplainTrapDisarmed(void)
{
	fdScriptStart();
	FieldStateWork->transition      = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	FieldStateWork->transitionFrame = 32;
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptWait(20);
	fdActorSetPos(ACTOR_KRADEN, 576, 232);
	fdScriptWait(1);
	// Kraden, from the south: "You found it, Isaac!" Jenna: "Kraden!"
	fdScriptSetMessageId(MsgSolSanctumKradenYouFoundIt);
	scSolSanctumStatueSayThenWait(ACTOR_KRADEN, 6);
	fdActorSetPos(ACTOR_KRADEN, 576, 280);
	fdCameraSetSpeed(0.8);
	fdCameraMoveTo(574, CAMERA_KEEP, 180, 1);
	fdActorSetSpeed(ACTOR_KRADEN, 1);
	fdActorWalkToAndWait(ACTOR_KRADEN, 576, 216);
	fdScriptWait(20);
	fdActorJump(ACTOR_JENNA, 2, 0);
	fdScriptWait(30);
	scSolSanctumStatueSayThenWait(ACTOR_JENNA, 6);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	fdScriptWait(6);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST_NORTHWEST, 0);
	fdScriptWait(10);
	// The camera swings west to the hole at the center and back.
	fdCameraSetSpeed(5.6);
	fdCameraMoveTo(287, CAMERA_KEEP, 176, 1);
	fdCameraWaitForMove();
	fdScriptWait(60);
	fdCameraMoveTo(574, CAMERA_KEEP, 180, 1);
	fdCameraWaitForMove();
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
	fdScriptWait(10);
	// Kraden hops up to the party: "Excellent work, Isaac!"
	fdActorJump(ACTOR_KRADEN, 6, 0);
	fdActorSetSpeedAndAcceleration(ACTOR_KRADEN, 3, 2);
	fdActorWalkToAndWait(ACTOR_KRADEN, 576, 184);
	fdScriptWait(40);
	scSolSanctumStatueSayThenWait(ACTOR_KRADEN, 6);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
	fdScriptWait(40);
	// The screen closes and opens on Kraden by the hole: "Where did that hole come from?" Then back
	// to the party.
	FieldStateWork->transition      = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
	FieldStateWork->transitionFrame = MAP_TRANSITION_TICKS;
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	fdCameraMoveTo(287, CAMERA_KEEP, 176, 0);
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptWait(40);
	fdActorSetPos(ACTOR_KRADEN, 424, 200);
	sySynch(1);
	scSolSanctumStatueSayThenWait(ACTOR_KRADEN, 20);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	fdActorSetPos(ACTOR_KRADEN, 576, 184);
	fdCameraMoveTo(574, CAMERA_KEEP, 180, 0);
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptWait(20);
	fdCameraSetSpeed(1.2);
	fdCameraMoveTo(574, CAMERA_KEEP, 157, 1);
	fdCameraWaitForMove();
	fdScriptWait(20);
	// Garet: two beams shot out of the statues Isaac moved, and the hole appeared. Jenna: Isaac
	// dropped the statue into it.
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	scSolSanctumStatueSayThenWait(ACTOR_GARET, 20);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
	scSolSanctumStatueSayThenWait(ACTOR_JENNA, 6);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	fdScriptWait(40);
	// Kraden: "Good work, everyone!" and "So the statue puzzle was the trap..."
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
	scSolSanctumStatueSayThenWait(ACTOR_KRADEN, 80);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdScriptWait(60);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
	fdScriptWait(30);
	scSolSanctumStatueSayThenWait(ACTOR_KRADEN, 20);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdScriptWait(60);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
	fdScriptWait(30);
	// Kraden: the other statue was there to disarm the trap, and now they should be all right.
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
	scSolSanctumStatueSayThenWait(ACTOR_KRADEN, 6);
	fdActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GARET, 0);
	fdScriptWait(40);
	fdActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_JENNA, 0);
	fdScriptWait(40);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	scSolSanctumStatueSayThenWait(ACTOR_KRADEN, 6);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
	fdScriptWait(40);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	// Kraden: he will observe from the Luna room. He leaves south.
	scSolSanctumStatueSayThenWait(ACTOR_KRADEN, 6);
	fdCameraSetSpeed(1.6);
	fdCameraMoveTo(576, CAMERA_KEEP, 215, 1);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH, 0);
	fdScriptWait(10);
	fdActorJump(ACTOR_KRADEN, 6, 0);
	fdActorWalkToAndWait(ACTOR_KRADEN, 576, 217);
	fdScriptWait(20);
	fdActorWalkToAndWait(ACTOR_KRADEN, 576, 321);
	fdActorSetPos(ACTOR_KRADEN, 0, 0);
	fdCameraSetSpeed(3.6);
	fdCameraMoveTo(576, CAMERA_KEEP, 136, 1);
	fdCameraWaitForMove();
	fdScriptWait(20);
	fdActorSetSpeed(ACTOR_GARET, 1);
	fdActorSetSpeed(ACTOR_JENNA, 1);
	scWalkIntoLeader(ACTOR_GARET);
	scWalkIntoLeader(ACTOR_JENNA);
	fdScriptClearFlag(FLAG_SHOW_AREA_NAME);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	FieldStateWork->transitionFrame = MAP_TRANSITION_TICKS;
	fdScriptEnd();
}

// Kraden's first visit, a script in which he runs here to warn that the mechanism is a trap.
static void scSolSanctumStatueExplainStatuePuzzle(void)
{
	FIELD_STATE *state;

	fdScriptStart();
	state                  = FieldStateWork;
	state->transition      = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	state->transitionFrame = 32;
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptWait(20);
	fdActorSetPos(ACTOR_KRADEN, 576, 296);
	fdScriptWait(1);
	// Kraden, from the south: "Wait, Isaac!" Jenna: "What's wrong, Kraden?"
	fdScriptSetMessageId(MsgSolSanctumWait);
	scSolSanctumStatueSayThenWait(ACTOR_KRADEN, 6);
	fdCameraSetSpeed(0.8);
	fdCameraMoveTo(574, CAMERA_KEEP, 180, 1);
	fdActorSetSpeed(ACTOR_KRADEN, 1);
	fdActorWalkToAndWait(ACTOR_KRADEN, 576, 216);
	fdScriptWait(20);
	fdActorJump(ACTOR_JENNA, 2, 0);
	fdScriptWait(30);
	scSolSanctumStatueSayThenWait(ACTOR_JENNA, 6);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	fdScriptWait(6);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST_NORTHWEST, 0);
	fdScriptWait(10);
	fdCameraSetSpeed(5.6);
	fdCameraMoveTo(287, CAMERA_KEEP, 176, 1);
	fdCameraWaitForMove();
	fdScriptWait(60);
	fdCameraMoveTo(574, CAMERA_KEEP, 180, 1);
	fdCameraWaitForMove();
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
	fdScriptWait(10);
	fdActorJump(ACTOR_KRADEN, 6, 0);
	fdActorSetSpeedAndAcceleration(ACTOR_KRADEN, 3, 2);
	fdActorWalkToAndWait(ACTOR_KRADEN, 576, 184);
	fdScriptWait(40);
	// Kraden: "This is a most interesting mechanism you've found, Isaac."
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	scSolSanctumStatueSayThenWait(ACTOR_KRADEN, 6);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdScriptWait(40);
	// Kraden: "But this is clearly... some kind of trap." All three start.
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 3);
	scSolSanctumStatueSayThenWait(ACTOR_KRADEN, 6);
	fdActorJump(ACTOR_PARTY_LEADER, 2, 0);
	fdActorJump(ACTOR_GARET, 2, 0);
	fdActorJump(ACTOR_JENNA, 2, 0);
	fdScriptWait(30);
	// Garet: "This is...a trap?" Kraden: "Go look in the Luna room".
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	scSolSanctumStatueSayThenWait(ACTOR_GARET, 6);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 1);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
	scSolSanctumStatueSayThenWait(ACTOR_KRADEN, 6);
	fdActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_JENNA, 0);
	fdScriptWait(40);
	fdActorFaceActor(ACTOR_KRADEN, ACTOR_PARTY_LEADER, 0);
	fdActorFaceActor(ACTOR_KRADEN, ACTOR_JENNA, 0);
	fdScriptWait(40);
	fdActorJump(ACTOR_KRADEN, 6, 0);
	fdActorWalkToAndWait(ACTOR_KRADEN, 576, 216);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
	fdScriptWait(10);
	fdActorSetSpeed(ACTOR_KRADEN, 1.2);
	fdActorWalkTo(ACTOR_KRADEN, 432, 200);
	fdScriptWait(20);
	fdCameraSetSpeed(2.4);
	fdCameraMoveTo(288, CAMERA_KEEP, 171, 1);
	fdCameraWaitForMove();
	fdScriptWait(80);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_STAND);
	fdCameraMoveTo(574, CAMERA_KEEP, 180, 1);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
	fdScriptWait(30);
	fdActorWalkToAndWait(ACTOR_KRADEN, 576, 216);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
	fdCameraSetSpeed(0.8);
	fdCameraMoveTo(574, CAMERA_KEEP, 171, 1);
	fdActorJump(ACTOR_KRADEN, 6, 0);
	fdActorSetSpeedAndAcceleration(ACTOR_KRADEN, 3, 2);
	fdActorWalkToAndWait(ACTOR_KRADEN, 576, 184);
	fdScriptWait(80);
	// Kraden: they are looking in the right place, but moving one statue springs the trap. Jenna:
	// why would anyone put a trap here?
	scSolSanctumStatueSayThenWait(ACTOR_KRADEN, 6);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
	scSolSanctumStatueSayThenWait(ACTOR_KRADEN, 20);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdScriptWait(40);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
	scSolSanctumStatueSayThenWait(ACTOR_JENNA, 6);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
	fdScriptWait(40);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
	fdScriptWait(30);
	// Kraden: they have hidden something very important, perhaps the key to Alchemy itself.
	scSolSanctumStatueSayThenWait(ACTOR_KRADEN, 6);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdScriptWait(60);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
	scSolSanctumStatueSayThenWait(ACTOR_KRADEN, 6);
	fdActorFaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
	fdScriptWait(40);
	fdActorFaceEachOther(ACTOR_JENNA, ACTOR_PARTY_LEADER, 0);
	fdScriptWait(40);
	// Kraden: "Should I tell you more?" Either answer, he thinks they would not understand yet.
	fdScriptOpenMessage(ACTOR_KRADEN, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
	if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
	{
		fdScriptSetMessageId(MsgSolSanctumHonestlyDoubtUnderstand);
	}
	else
	{
		fdScriptSetMessageId(MsgSolSanctumHmphWellTold);
	}
	scSolSanctumStatueSayThenWait(ACTOR_KRADEN, 6);
	// Kraden: "We must try to find a solution to this mystery." Garet: "Sheesh... Another mystery?"
	// Kraden will keep checking the Luna room.
	fdScriptSetMessageId(MsgSolSanctumTryFindSolution);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	scSolSanctumStatueSayThenWait(ACTOR_KRADEN, 6);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdScriptWait(60);
	scSolSanctumStatueSayThenWait(ACTOR_GARET, 6);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	scSolSanctumStatueSayThenWait(ACTOR_KRADEN, 6);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH, 0);
	fdScriptWait(20);
	fdActorJump(ACTOR_KRADEN, 6, 0);
	fdActorWalkToAndWait(ACTOR_KRADEN, 576, 216);
	fdScriptWait(40);
	fdCameraSetSpeed(0.8);
	fdCameraMoveTo(574, CAMERA_KEEP, 191, 1);
	fdActorSetSpeed(ACTOR_KRADEN, 1);
	fdActorWalkToAndWait(ACTOR_KRADEN, 576, 232);
	fdScriptWait(40);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	fdScriptWait(40);
	// Kraden, leaving: his intuition says the light is the key to the secret.
	scSolSanctumStatueSayThenWait(ACTOR_KRADEN, 6);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
	fdScriptWait(30);
	scSolSanctumStatueSayThenWait(ACTOR_KRADEN, 6);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	fdCameraMoveTo(576, CAMERA_KEEP, 215, 1);
	fdActorWalkToAndWait(ACTOR_KRADEN, 574, 323);
	fdActorSetPos(ACTOR_KRADEN, 0, 0);
	fdCameraSetSpeed(3.6);
	fdCameraMoveTo(576, CAMERA_KEEP, 136, 1);
	fdCameraWaitForMove();
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 0);
	fdScriptWait(10);
	// Jenna: "let's see if we can't figure this out ourselves."
	scSolSanctumStatueSayThenWait(ACTOR_JENNA, 6);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
	fdActorSetSpeed(ACTOR_GARET, 1);
	fdActorSetSpeed(ACTOR_JENNA, 1);
	scWalkIntoLeader(ACTOR_JENNA);
	scWalkIntoLeader(ACTOR_GARET);
	gmFlagClear(FLAG_SHOW_AREA_NAME);
	state                  = FieldStateWork;
	state->transition      = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	state->transitionFrame = MAP_TRANSITION_TICKS;
	fdScriptEnd();
}

// Unused: called only from the branch of scSolSanctumStatueOpenSeal that cannot run, since
// scSolSanctumStatueSelectStoryExit returns only 1 or -1.
static void scSolSanctumStatueUnusedPutOutSeals(void)
{
	MAP_GRID_SCROLL_STATE *grid;
	ACTOR                 *leader;
	Sint32                *savedOrigin;
	FIXED                  cameraPos[3];
	Sint32                 approachSide;
	Sint32                 frame;

	grid   = MapGridState[0];
	leader = fdActorGet(ACTOR_PARTY_LEADER);
	// Temporarily follow a local point, panning from the leader toward the seal.
	if (leader->z.fixed < toFIXED(179))
	{
		fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 575, 132);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
		fdScriptWait(30);
		savedOrigin = grid->origin;
		SET_VECTOR(cameraPos, leader->x.fixed, leader->y.fixed, leader->z.fixed);
		grid->origin = cameraPos;
		for (frame = 0; frame != 30; frame++)
		{
			cameraPos[2] += toFIXED(1);
			fdScriptWait(1);
		}
		fdScriptWait(40);
		approachSide = 1;
	}
	else
	{
		fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 577, 222);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
		fdScriptWait(30);
		SET_VECTOR(cameraPos, leader->x.fixed, leader->y.fixed, leader->z.fixed);
		savedOrigin  = grid->origin;
		grid->origin = cameraPos;
		for (frame = 0; frame != 30; frame++)
		{
			cameraPos[2] -= toFIXED(1);
			fdScriptWait(1);
		}
		fdScriptWait(40);
		approachSide = 2;
	}
	for (frame = 0; frame != 6; frame++)
	{
		fdMapGridCopyMetatilesTo(2, 28, 34, 10, 4, 2);
		fdScriptWait(8);
		fdMapGridCopyMetatilesTo(2, 30, 34, 10, 4, 2);
		fdScriptWait(8);
	}
	for (frame = 0; frame != 10; frame++)
	{
		fdMapGridCopyMetatilesTo(2, 28, 34, 10, 4, 2);
		fdScriptWait(4);
		fdMapGridCopyMetatilesTo(2, 30, 34, 10, 4, 2);
		fdScriptWait(4);
	}
	for (frame = 0; frame != 12; frame++)
	{
		fdMapGridCopyMetatilesTo(2, 28, 34, 10, 4, 2);
		fdScriptWait(2);
		fdMapGridCopyMetatilesTo(2, 30, 34, 10, 4, 2);
		fdScriptWait(2);
	}
	fdMapGridCopyMetatilesTo(2, 28, 34, 10, 4, 2);
	fdMapGridCopyMetatilesTo(8, 55, 32, 40, 8, 4);
	fdScriptWait(60);
	// Reverse the pan before restoring the caller's camera target.
	if (approachSide == 1)
	{
		for (frame = 0; frame != 30; frame++)
		{
			cameraPos[2] -= toFIXED(1);
			fdScriptWait(1);
		}
	}
	else if (approachSide == 2)
	{
		for (frame = 0; frame != 30; frame++)
		{
			cameraPos[2] += toFIXED(1);
			fdScriptWait(1);
		}
	}
	grid->origin = savedOrigin;
}

// The clouds' scroll offset gains this a tick. The back row moves by its 1024th part, the middle by
// its 512th and the front by its 256th: an eighth, a quarter and half a pixel a tick.
#define SCROLL_OFFSET_STEP 128

// Draws the three rows of sky clouds while they overlap the screen.
static void scSolSanctumStatueUpdateScrollingSprites(void)
{
	MAP_GRID_SCROLL_STATE *grid    = MapGridState[0];
	ANIMATION_SPR_PART    *sprite  = ScrollingSprites.parts;
	Sint32                 cameraX = syMapGridScrollStateView(grid)[0] / toFIXED(1);
	Sint32                 screenY = 80 - syMapGridScrollStateView(grid)[1] / toFIXED(1);
	Sint32                 scrollX;
	Uint32                 column;

	if (screenY >= -16 && screenY < DISPLAY_HEIGHT)
	{
		scrollX =
			((SolSanctumStatueScrollOffset >> 10) - cameraX) | -SOL_SANCTUM_STATUE_SPRITE_WIDTH;
		for (column = 0; column < SOL_SANCTUM_STATUE_ROW_SPRITE_COUNT; column++)
		{
			sprite->x = scrollX;
			sprite->y = screenY;
			syOAMAddSpriteAtDepth(sprite, 0);
			scrollX += SOL_SANCTUM_STATUE_SPRITE_WIDTH;
			sprite++;
		}
		scrollX =
			((SolSanctumStatueScrollOffset >> 9) - cameraX) | -SOL_SANCTUM_STATUE_SPRITE_WIDTH;
		for (column = 0; column < SOL_SANCTUM_STATUE_ROW_SPRITE_COUNT; column++)
		{
			sprite->x = scrollX;
			sprite->y = screenY;
			syOAMAddSpriteAtDepth(sprite, 0);
			scrollX += SOL_SANCTUM_STATUE_SPRITE_WIDTH;
			sprite++;
		}
		scrollX =
			((SolSanctumStatueScrollOffset >> 8) - cameraX) | -SOL_SANCTUM_STATUE_SPRITE_WIDTH;
		for (column = 0; column < SOL_SANCTUM_STATUE_ROW_SPRITE_COUNT; column++)
		{
			sprite->x = scrollX;
			sprite->y = screenY + SOL_SANCTUM_STATUE_FRONT_CLOUD_ROW_Y_OFFSET;
			syOAMAddSpriteAtDepth(sprite, 0);
			scrollX += SOL_SANCTUM_STATUE_SPRITE_WIDTH;
			sprite++;
		}
	}
	SolSanctumStatueScrollOffset += SCROLL_OFFSET_STEP;
}

// Walking north over a jump spot jumps the leader two metatiles ahead (fdTryJumpForward).
static void scSolSanctumStatueJumpNorth(void)
{
	ACTOR *leader = fdActorGet(ACTOR_PARTY_LEADER);
	if (leader->facing == FACING_NORTH)
	{
		fdTryJumpForward();
	}
}

// Walking south over a jump spot jumps the leader two metatiles ahead.
static void scSolSanctumStatueJumpSouth(void)
{
	ACTOR *leader = fdActorGet(ACTOR_PARTY_LEADER);
	if (leader->facing == FACING_SOUTH)
	{
		fdTryJumpForward();
	}
}

// Unused: the eastern hall's northwest statue slides by itself to (504, 152), the metatile where
// Move opens its seal, unless that seal is already open.
//
// It opens nothing.
//
// The player moves the statue there with Move instead.
UNUSED static void scSolSanctumStatueUnusedMoveNorthwestStatue(void)
{
	if (gmFlagGet(FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN) == 0)
	{
		fdScriptStart();
		fdActorGet(ACTOR_NORTHWEST_STATUE);
		fdActorSetSpeed(ACTOR_NORTHWEST_STATUE, 0.2);
		fdActorWalkToAndWait(ACTOR_NORTHWEST_STATUE, 504, 152);
		fdScriptEnd();
	}
}

// Restores the room's statues, beams and seals from their flags and plays Kraden's two visits.
Sint32 scInit(void)
{
	ACTOR *centerEffect;

	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	scSolSanctumStatueInitScrollingSprites();
	gmFlagSet(FLAG_RETREAT_BLOCKED);
	centerEffect                 = fdActorGet(ACTOR_MOVED_CENTER_STATUE);
	centerEffect->collisionFlags = 0;
	{
		Uint8 flags                 = centerEffect->priorityFlags | ACTOR_PRIORITY_UNDERFOOT;

		centerEffect->priorityFlags = flags;
	}
	fdActorSetSpriteFlags(fdActorGet(ACTOR_MOVED_CENTER_STATUE), 0);
	fdActorGet(ACTOR_MOVED_CENTER_STATUE)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
	fdActorSetSpritePriority(ACTOR_MOVED_CENTER_STATUE, 1);
	// Only the returns with the trap disarmed or triggered keep Jenna and Garet staged for their
	// conversations.
	if (MainRoutineStateWork.entrance
	        != SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_DISARMED
	    && MainRoutineStateWork.entrance
	           != SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_TRIGGERED)
	{
		fdActorSetPos(ACTOR_JENNA, 0, 0);
		fdActorSetPos(ACTOR_GARET, 0, 0);
	}
	if (gmFlagGet(FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED) != 0)
	{
		fdActorSetPos(ACTOR_MOVED_CENTER_STATUE, 288, 178);
		fdActorSetPos(ACTOR_CENTER_STATUE, 1608, 1608);
		fdActorSetPos(ACTOR_LEFT_BEAM_STATUE, 232, 120);
		fdActorSetPos(ACTOR_RIGHT_BEAM_STATUE, 344, 120);
		fdActorSetPos(ACTOR_LEFT_BEAM_STATUE, 232, 120);
		DRAW_LEFT_BEAM();
		fdActorSetPos(ACTOR_RIGHT_BEAM_STATUE, 344, 120);
		DRAW_RIGHT_BEAM();
		DRAW_CROSSED_BEAMS();
		fdMapGridCopyMetatileAttributes(0, 1, 2, 1, 17, 7);
	}
	else if (gmFlagGet(FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING) != 0
	         && gmFlagGet(FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING) != 0)
	{
		fdActorSetPos(ACTOR_LEFT_BEAM_STATUE, 232, 120);
		fdActorSetPos(ACTOR_RIGHT_BEAM_STATUE, 344, 120);
		DRAW_CENTER_HOLE();
		fdActorSetPos(ACTOR_LEFT_BEAM_STATUE, 232, 120);
		DRAW_LEFT_BEAM();
		fdActorSetPos(ACTOR_RIGHT_BEAM_STATUE, 344, 120);
		DRAW_RIGHT_BEAM();
		DRAW_CROSSED_BEAMS();
		fdMapGridCopyMetatileAttributes(0, 0, 2, 1, 17, 8);
	}
	else
	{
		if (gmFlagGet(FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING) != 0)
		{
			fdActorSetPos(ACTOR_LEFT_BEAM_STATUE, 232, 120);
			DRAW_LEFT_BEAM();
		}
		if (gmFlagGet(FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING) != 0)
		{
			fdActorSetPos(ACTOR_RIGHT_BEAM_STATUE, 344, 120);
			DRAW_RIGHT_BEAM();
		}
	}
	// Each open seal has its statue on it, and swaps its quarter of the two halls' floor pictures:
	// the east hall's moon (metatiles (34, 10) to (37, 11)) lit as the sun, and the west hall's sun
	// ((16, 10) to (19, 11)) darkened to the moon, from the spare sun and moon at metatiles (2, 28)
	// and (2, 30). Its statue's beam is drawn over the east picture on map layer 1.
	if (gmFlagGet(FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN) != 0)
	{
		fdActorSetPos(ACTOR_NORTHWEST_STATUE, 504, 152);
		fdMapGridCopyMetatilesTo(2, 28, 34, 10, 2, 1);
		fdMapGridCopyMetatilesTo(2, 30, 16, 10, 2, 1);
		fdMapGridCopyMetatilesTo(0, 55, 32, 40, 4, 3);
	}
	if (gmFlagGet(FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN) != 0)
	{
		fdActorSetPos(ACTOR_NORTHEAST_STATUE, 648, 152);
		fdMapGridCopyMetatilesTo(4, 28, 36, 10, 2, 1);
		fdMapGridCopyMetatilesTo(4, 30, 18, 10, 2, 1);
		fdMapGridCopyMetatilesTo(4, 55, 36, 40, 4, 3);
	}
	if (gmFlagGet(FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN) != 0)
	{
		fdActorSetPos(ACTOR_SOUTHWEST_STATUE, 504, 200);
		fdMapGridCopyMetatilesTo(2, 29, 34, 11, 2, 1);
		fdMapGridCopyMetatilesTo(2, 31, 16, 11, 2, 1);
		fdMapGridCopyMetatilesTo(0, 58, 32, 43, 4, 1);
	}
	if (gmFlagGet(FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN) != 0)
	{
		fdActorSetPos(ACTOR_SOUTHEAST_STATUE, 648, 200);
		fdMapGridCopyMetatilesTo(4, 29, 36, 11, 2, 1);
		fdMapGridCopyMetatilesTo(4, 31, 18, 11, 2, 1);
		fdMapGridCopyMetatilesTo(4, 58, 36, 43, 4, 1);
	}
	if (MainRoutineStateWork.entrance
	    == SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_DISARMED)
	{
		if (fdScriptGetFlag(FLAG_SOL_SANCTUM_TRAP_DISARMED_EXPLAINED) != 0)
		{
			fdActorSetPos(ACTOR_GARET, 0, 0);
			fdActorSetPos(ACTOR_JENNA, 0, 0);
		}
		else if (gmFlagGet(FLAG_KEEP_PARTY_POSITION) == 0)
		{
			scSolSanctumStatueExplainTrapDisarmed();
			gmFlagSet(FLAG_SOL_SANCTUM_TRAP_DISARMED_EXPLAINED);
		}
	}
	if (MainRoutineStateWork.entrance
	    == SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_TRIGGERED)
	{
		if (fdScriptGetFlag(FLAG_SOL_SANCTUM_STATUE_PUZZLE_EXPLAINED) != 0)
		{
			fdActorSetPos(ACTOR_GARET, 0, 0);
			fdActorSetPos(ACTOR_JENNA, 0, 0);
		}
		else if (gmFlagGet(FLAG_KEEP_PARTY_POSITION) == 0)
		{
			scSolSanctumStatueExplainStatuePuzzle();
			gmFlagSet(FLAG_SOL_SANCTUM_STATUE_PUZZLE_EXPLAINED);
		}
	}
	if (gmFlagGet(FLAG_SOL_SANCTUM_ERUPTED) != 0)
	{
		fdQueueMusic(SE_RUMBLE);
		fdMapGridSetShake(1, 1, 1);
		fdEffectInitSparkles();
	}
	return 0;
}

// Each drifting row's picture: one 32x8 sprite, repeated along the row.
#define ROW_SPRITE_BYTES (4 * TILE_SIZE_4BPP)

// Loads the three strips of graphics and fills in a row of sprites for each.
static void scSolSanctumStatueInitScrollingSprites(void)
{
	Uint32  column;
	Sint32  tile;
	Sint32 *spriteWords;
	Uint8  *pixels;

	spriteWords = ScrollingSprites.words;
	pixels      = syHeapSlotAllocateEWRAM(HEAP_SLOT_TILE_SCRATCH, 1024);
	syDecompressByteLZ(SolSanctumStatueSpriteRowsGraphics, pixels);
	tile = syVRAMCacheLoad(syVRAMCacheFindFreeEntry(), ROW_SPRITE_BYTES, pixels);
	for (column = 0; column < SOL_SANCTUM_STATUE_ROW_SPRITE_COUNT; column++)
	{
		Sint32 *attributeWords  = spriteWords;

		*attributeWords++       = 0;
		*attributeWords++       = OBJ_SIZE_32X8;
		spriteWords            += 3;
		*attributeWords         = tile | OBJ_PRIORITY(3) | OBJ_PALETTE(BACK_CLOUD_ROW_PALETTE);
	}
	tile = syVRAMCacheLoad(syVRAMCacheFindFreeEntry(), ROW_SPRITE_BYTES, pixels + ROW_SPRITE_BYTES);
	for (column = 0; column < SOL_SANCTUM_STATUE_ROW_SPRITE_COUNT; column++)
	{
		Sint32 *attributeWords  = spriteWords;

		*attributeWords++       = 0;
		*attributeWords++       = OBJ_SIZE_32X8;
		spriteWords            += 3;
		*attributeWords         = tile | OBJ_PRIORITY(3) | OBJ_PALETTE(MIDDLE_CLOUD_ROW_PALETTE);
	}
	tile = syVRAMCacheLoad(
		syVRAMCacheFindFreeEntry(), ROW_SPRITE_BYTES, pixels + 2 * ROW_SPRITE_BYTES);
	for (column = 0; column < SOL_SANCTUM_STATUE_ROW_SPRITE_COUNT; column++)
	{
		Sint32 *attributeWords  = spriteWords;

		*attributeWords++       = 0;
		*attributeWords++       = OBJ_SIZE_32X8;
		spriteWords            += 3;
		*attributeWords         = tile | OBJ_PRIORITY(3) | OBJ_PALETTE(FRONT_CLOUD_ROW_PALETTE);
	}
	syHeapSlotRelease(HEAP_SLOT_TILE_SCRATCH);
	sySetTickEvent(scSolSanctumStatueUpdateScrollingSprites, EVENT_PASS_TICK);
}

// Isaac moves the northwest statue of the east hall with Move; if it now stands on its seal
// (metatile (31, 9)), the seal opens.
static void scSolSanctumStatueNorthwestStatueMoved(void)
{
	if (scSolSanctumStatueIsActorAtMetatile(ACTOR_NORTHWEST_STATUE, 31, 9) != 0)
	{
		scSolSanctumStatueOpenNorthwestSeal();
	}
}

// Isaac moves the northeast statue of the east hall with Move; if it now stands on its seal
// (metatile (40, 9)), the seal opens.
static void scSolSanctumStatueNortheastStatueMoved(void)
{
	if (scSolSanctumStatueIsActorAtMetatile(ACTOR_NORTHEAST_STATUE, 40, 9) != 0)
	{
		scSolSanctumStatueOpenNortheastSeal();
	}
}

// Isaac moves the southwest statue of the east hall with Move; if it now stands on its seal
// (metatile (31, 12)), the seal opens.
static void scSolSanctumStatueSouthwestStatueMoved(void)
{
	if (scSolSanctumStatueIsActorAtMetatile(ACTOR_SOUTHWEST_STATUE, 31, 12) != 0)
	{
		scSolSanctumStatueOpenSouthwestSeal();
	}
}

// Isaac moves the southeast statue of the east hall with Move; if it now stands on its seal
// (metatile (40, 12)), the seal opens.
static void scSolSanctumStatueSoutheastStatueMoved(void)
{
	if (scSolSanctumStatueIsActorAtMetatile(ACTOR_SOUTHEAST_STATUE, 40, 12) != 0)
	{
		scSolSanctumStatueOpenSoutheastSeal();
	}
}

// Isaac moves the left beam statue with Move; once it stands beside the sun-disc statue (metatile
// (14, 7)), it shines its beam on it.
static void scSolSanctumStatueLeftBeamStatueMoved(void)
{
	fdSetMetatileBehaviorAt(MAPGRID_LAYER_OVERHEAD, 208, 112, METATILE_BEHAVIOR_NORMAL);
	if (scSolSanctumStatueIsActorAtMetatile(ACTOR_LEFT_BEAM_STATUE, 14, 7) != 0)
	{
		scSolSanctumStatueShineLeftBeam();
	}
}

// Isaac moves the right beam statue with Move; once it stands beside the sun-disc statue (metatile
// (21, 7)), it shines its beam on it.
static void scSolSanctumStatueRightBeamStatueMoved(void)
{
	fdSetMetatileBehaviorAt(MAPGRID_LAYER_OVERHEAD, 352, 112, METATILE_BEHAVIOR_NORMAL);
	if (scSolSanctumStatueIsActorAtMetatile(ACTOR_RIGHT_BEAM_STATUE, 21, 7) != 0)
	{
		scSolSanctumStatueShineRightBeam();
	}
}

// A statue Move carries is put on map layer 2 (MAPGRID_LAYER_OVERHEAD, script_commands_move.c), and
// a wall among that layer's metatile behaviors stops it: the casts below wall off metatiles there
// for the statue alone, and its landing opens them again.

// Nothing visible happens as Isaac starts to cast Move on the sun-disc statue while standing right
// below it (metatiles (17, 8) and (18, 8)). The game makes the two metatiles he stands on walls for
// the cast, so that Move cannot draw the statue south on top of him;
// scSolSanctumStatueCenterStatueMoved opens them again when the cast ends.
static void scSolSanctumStatuePrepareMoveOnCenterStatue(void)
{
	Sint32 column;

	column = FIELD_METATILE(fdActorGet(ACTOR_PARTY_LEADER)->x.fixed);
	if (FIELD_METATILE(fdActorGet(ACTOR_PARTY_LEADER)->z.fixed) == 8)
	{
		if (column >= 17 && column <= 18)
		{
			fdSetMetatileBehaviorAt(MAPGRID_LAYER_OVERHEAD, 272, 128, METATILE_BEHAVIOR_WALL);
			fdSetMetatileBehaviorAt(MAPGRID_LAYER_OVERHEAD, 288, 128, METATILE_BEHAVIOR_WALL);
		}
	}
}

// Nothing visible happens as Isaac starts to cast Move on the left beam statue while standing at
// the top of its track or just east of it (metatiles (13, 7) and (14, 7)). The game makes the
// track's top metatile, (13, 7), a wall for the cast, so that Move cannot bring the statue up into
// it there; scSolSanctumStatueLeftBeamStatueMoved opens it again when the cast ends.
static void scSolSanctumStatuePrepareMoveOnLeftBeamStatue(void)
{
	Sint32 column = FIELD_METATILE(fdActorGet(ACTOR_PARTY_LEADER)->x.fixed);
	Sint32 row    = FIELD_METATILE(fdActorGet(ACTOR_PARTY_LEADER)->z.fixed);

	if (row == 7 && (column >= 13 && column <= 14))
	{
		fdSetMetatileBehaviorAt(MAPGRID_LAYER_OVERHEAD, 208, 112, METATILE_BEHAVIOR_WALL);
	}
}

// Nothing visible happens as Isaac starts to cast Move on the right beam statue while standing at
// the top of its track or just west of it (metatiles (21, 7) and (22, 7)). The game makes the
// track's top metatile, (22, 7), a wall for the cast, so that Move cannot bring the statue up into
// it there; scSolSanctumStatueRightBeamStatueMoved opens it again when the cast ends.
static void scSolSanctumStatuePrepareMoveOnRightBeamStatue(void)
{
	Sint32 column = FIELD_METATILE(fdActorGet(ACTOR_PARTY_LEADER)->x.fixed);
	Sint32 row    = FIELD_METATILE(fdActorGet(ACTOR_PARTY_LEADER)->z.fixed);

	if (row == 7 && (column >= 21 && column <= 22))
	{
		fdSetMetatileBehaviorAt(MAPGRID_LAYER_OVERHEAD, 352, 112, METATILE_BEHAVIOR_WALL);
	}
}

// The sun-disc statue's Move and drop, described at its declaration above.
static void scSolSanctumStatueCenterStatueMoved(void)
{
	ACTOR *statue;
	FIXED  statueZ;

	statue = fdActorGet(ACTOR_CENTER_STATUE);
	fdSetMetatileBehaviorAt(MAPGRID_LAYER_OVERHEAD, 272, 128, METATILE_BEHAVIOR_NORMAL);
	fdSetMetatileBehaviorAt(MAPGRID_LAYER_OVERHEAD, 288, 128, METATILE_BEHAVIOR_NORMAL);
	if (statue != NULL)
	{
		statueZ = statue->z.fixed;
		fdScriptStart();
		if (FIELD_METATILE(statueZ) == 8)
		{
			if (gmFlagGet(FLAG_SOL_SANCTUM_CENTER_HOLE_OPEN) == 0)
			{
				Sint32 halfMetatile = FIELD_HALF_METATILE(fdActorGet(ACTOR_PARTY_LEADER)->z.fixed);

				if (halfMetatile >= 0 && halfMetatile <= 17)
				{
					fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 289, 158);
					fdActorGet(ACTOR_PARTY_LEADER)->facing = FACING_NORTH;
				}
			}
			if (gmFlagGet(FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING) != 0
			    && gmFlagGet(FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING) != 0)
			{
				gmFlagSet(FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED);
				// The statue slides south into the hole; ACTOR_MOVED_CENTER_STATUE stands in for it
				// there and the room shakes hard, then settles.
				fdCameraSetSpeed(2);
				fdCameraMoveTo(286, CAMERA_KEEP, 146, 1);
				fdCameraWaitForMove();
				fdActorGet(ACTOR_CENTER_STATUE)->steeringFlags &= ~ACTOR_FACES_MOTION;
				fdActorSetSpeedAndAcceleration(ACTOR_CENTER_STATUE, 3, 1);
				statue->motionFlags = 0;
				fdActorSetSpritePriority(ACTOR_CENTER_STATUE, 3);
				sySoundPlay(SE_STONE_GRIND);
				fdActorSetDestination(ACTOR_CENTER_STATUE, 288, 178);
				fdScriptWait(8);
				fdActorSetPos(ACTOR_MOVED_CENTER_STATUE, 288, 178);
				statue->targetX = ACTOR_NO_TARGET;
				statue->targetY = ACTOR_NO_TARGET;
				statue->targetZ = ACTOR_NO_TARGET;
				statue->x.fixed = 0;
				statue->y.fixed = 0;
				statue->z.fixed = 0;
				SET_VELOCITY(statue, 0, 0, 0);
				fdActorSetPos(ACTOR_CENTER_STATUE, 0, 0);
				fdMapGridSetShake(1, 1, 1);
				fdScriptWait(10);
				sySoundPlay(SE_RUMBLE);
				fdMapGridSetShake(3, 3, 1);
				fdScriptWait(10);
				fdMapGridSetShake(5, 5, 1);
				fdScriptWait(35);
				fdMapGridSetShake(4, 4, 1);
				fdScriptWait(20);
				fdMapGridSetShake(3, 3, 1);
				fdScriptWait(30);
				fdMapGridSetShake(2, 2, 1);
				fdScriptWait(40);
				sySoundPlay(SOUND_FADE_OUT_ACTION_SE);
				fdMapGridSetShake(1, 1, 1);
				fdScriptWait(10);
				fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
				fdScriptWait(60);
				sySoundPlay(SE_THUMP);
				if (gmFlagGet(FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN) != 0
				    && gmFlagGet(FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN) != 0
				    && gmFlagGet(FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN) != 0
				    && gmFlagGet(FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN) != 0)
				{
					gmFlagSet(FLAG_SOL_SANCTUM_ALL_SEALS_OPEN);
				}
				fdScriptWait(40);
				mnFieldMessageShow(MsgSolSanctumSomethingClicked, 1);
				fdMapGridCopyMetatileAttributes(0, 1, 2, 1, 17, 8);
				fdMapGridCopyMetatileAttributes(17, 9, 2, 1, 17, 7);
			}
		}
		fdScriptEnd();
	}
}

// Isaac walks north into the sun-disc statue once Move has drawn it a metatile south, and pushes it
// back to its place.
static void scSolSanctumStatuePushCenterStatueNorth(void)
{
	ACTOR *movingActor;

	movingActor = fdActorGet(ACTOR_CENTER_STATUE);
	if (movingActor != NULL)
	{
		if (FIELD_METATILE(movingActor->z.fixed) == 8)
		{
			fdScriptStart();
			sySoundPlay(SE_PUSH);
			fdActorSetSpeed(ACTOR_CENTER_STATUE, 0.2);
			fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.2);
			fdActorGet(ACTOR_CENTER_STATUE)->steeringFlags &= ~ACTOR_FACES_MOTION;
			fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_PUSH);
			fdActorSetDestination(
				ACTOR_PARTY_LEADER, fdActorGet(ACTOR_PARTY_LEADER)->x.part[0].integer, 136);
			fdActorSetDestination(ACTOR_CENTER_STATUE, 288, 120);
			fdActorWaitForMoveById(ACTOR_CENTER_STATUE);
			fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
			fdScriptEnd();
		}
	}
}

// Isaac walks into the northwest statue of the east hall and pushes it onto its seal (metatile (31,
// 9)), which opens.
static void scSolSanctumStatuePushNorthwestStatue(void)
{
	scSolSanctumStatuePushStatue(ACTOR_NORTHWEST_STATUE, 31, 9, 30, 9);
	scSolSanctumStatueOpenNorthwestSeal();
}

// Isaac walks into the northeast statue of the east hall and pushes it onto its seal (metatile (40,
// 9)), which opens.
static void scSolSanctumStatuePushNortheastStatue(void)
{
	scSolSanctumStatuePushStatue(ACTOR_NORTHEAST_STATUE, 40, 9, 41, 9);
	scSolSanctumStatueOpenNortheastSeal();
}

// Isaac walks into the southwest statue of the east hall and pushes it onto its seal (metatile (31,
// 12)), which opens.
static void scSolSanctumStatuePushSouthwestStatue(void)
{
	scSolSanctumStatuePushStatue(ACTOR_SOUTHWEST_STATUE, 31, 12, 30, 12);
	scSolSanctumStatueOpenSouthwestSeal();
}

// Isaac walks into the southeast statue of the east hall and pushes it onto its seal (metatile (40,
// 12)), which opens.
static void scSolSanctumStatuePushSoutheastStatue(void)
{
	scSolSanctumStatuePushStatue(ACTOR_SOUTHEAST_STATUE, 40, 12, 41, 12);
	scSolSanctumStatueOpenSoutheastSeal();
}

// Walking into the left beam statue from the west slides it east in front of the center, where it
// shines its beam.
static void scSolSanctumStatuePushLeftBeamStatueEast(void)
{
	scSolSanctumStatuePushStatue(ACTOR_LEFT_BEAM_STATUE, 14, 7, 13, 7);
	scSolSanctumStatueShineLeftBeam();
}

// Walking into the left beam statue from the north slides it one metatile south.
static void scSolSanctumStatuePushLeftBeamStatueSouth(void)
{
	ACTOR *statue = fdActorGet(ACTOR_LEFT_BEAM_STATUE);
	Sint32 row;

	if (statue == NULL)
	{
		return;
	}

	row = FIELD_METATILE(statue->z.fixed);
	scSolSanctumStatuePushStatue(ACTOR_LEFT_BEAM_STATUE, 13, row + 1, 13, row);
}

// Walking into the left beam statue from the south slides it one metatile north.
static void scSolSanctumStatuePushLeftBeamStatueNorth(void)
{
	ACTOR *statue = fdActorGet(ACTOR_LEFT_BEAM_STATUE);
	if (statue != NULL)
	{
		Sint32 row = FIELD_METATILE(statue->z.fixed);
		scSolSanctumStatuePushStatue(ACTOR_LEFT_BEAM_STATUE, 13, row - 1, 13, row);
	}
}

// Walking into the right beam statue from the east does as the left one.
static void scSolSanctumStatuePushRightBeamStatueWest(void)
{
	scSolSanctumStatuePushStatue(ACTOR_RIGHT_BEAM_STATUE, 21, 7, 22, 7);
	scSolSanctumStatueShineRightBeam();
}

// Walking into the right beam statue from the north slides it one metatile south.
static void scSolSanctumStatuePushRightBeamStatueSouth(void)
{
	ACTOR *statue = fdActorGet(ACTOR_RIGHT_BEAM_STATUE);
	if (statue != NULL)
	{
		Sint32 row = FIELD_METATILE(statue->z.fixed);
		scSolSanctumStatuePushStatue(ACTOR_RIGHT_BEAM_STATUE, 22, row + 1, 22, row);
	}
}

// Walking into the right beam statue from the south slides it one metatile north.
static void scSolSanctumStatuePushRightBeamStatueNorth(void)
{
	ACTOR *statue = fdActorGet(ACTOR_RIGHT_BEAM_STATUE);
	if (statue != NULL)
	{
		Sint32 row = FIELD_METATILE(statue->z.fixed);
		scSolSanctumStatuePushStatue(ACTOR_RIGHT_BEAM_STATUE, 22, row - 1, 22, row);
	}
}

// Moves the leader and statue together to the centers of their target metatiles.
static void scSolSanctumStatuePushStatue(
	Sint32 statueId, Sint32 statueColumn, Sint32 statueRow, Sint32 leaderColumn, Sint32 leaderRow)
{
	Sint32 statueX;
	Sint32 statueZ;

	fdScriptStart();
	sySoundPlay(SE_PUSH);
	fdActorSetSpeed(statueId, 0.2);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.2);
	fdActorGet(statueId)->steeringFlags &= ~ACTOR_FACES_MOTION;
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_PUSH);
	fdActorSetDestination(ACTOR_PARTY_LEADER, (leaderColumn << 4) + 8, (leaderRow << 4) + 8);
	statueX = statueColumn << 4;
	statueZ = statueRow << 4;
	fdActorSetDestination(statueId, statueX + 8, statueZ + 8);
	fdActorWaitForMoveById(statueId);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	fdScriptEnd();
}

// Returns 1 when the actor stands in the given map metatile.
static Sint32 scSolSanctumStatueIsActorAtMetatile(Sint32 actorId, Sint32 column, Sint32 row)
{
	ACTOR *actor = fdActorGet(actorId);
	if (actor == NULL || FIELD_METATILE(actor->x.fixed) != column)
	{
		return 0;
	}
	if (FIELD_METATILE(actor->z.fixed) != row)
	{
		return 0;
	}
	return 1;
}

// Opens the northwest seal (scSolSanctumStatueOpenSeal): the metatiles it lights, its flag, where
// the leader jumps back to and where its statue slides back to if the seal closes again.
static void scSolSanctumStatueOpenNorthwestSeal(void)
{
	SEAL_OPENING *seal = &SealOpening;
	METATILE_COPY_SET(seal->open, 0, 55, 32, 40, 4, 3);
	METATILE_COPY_SET(seal->pulseA, 2, 30, 34, 10, 2, 1);
	METATILE_COPY_SET(seal->pulseB, 2, 28, 34, 10, 2, 1);
	METATILE_COPY_SET(seal->opened, 2, 30, 16, 10, 2, 1);
	seal->flags  = FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN;
	seal->facing = FACING_SOUTH;
	seal->destX  = 500;
	seal->destZ  = 132;
	METATILE_COPY_SET(seal->closed[0], 8, 55, 32, 40, 4, 3);
	METATILE_COPY_SET(seal->closed[1], 2, 30, 34, 10, 2, 1);
	METATILE_COPY_SET(seal->closed[2], 2, 28, 16, 10, 2, 1);
	seal->actorId = ACTOR_NORTHWEST_STATUE;
	seal->actorX  = 488;
	seal->actorZ  = 152;
	scSolSanctumStatueOpenSeal();
}

// Opens the northeast seal (scSolSanctumStatueOpenSeal): the metatiles it lights, its flag, where
// the leader jumps back to and where its statue slides back to if the seal closes again.
static void scSolSanctumStatueOpenNortheastSeal(void)
{
	SEAL_OPENING *seal = &SealOpening;
	METATILE_COPY_SET(seal->open, 4, 55, 36, 40, 4, 3);
	METATILE_COPY_SET(seal->pulseA, 4, 30, 36, 10, 2, 1);
	METATILE_COPY_SET(seal->pulseB, 4, 28, 36, 10, 2, 1);
	METATILE_COPY_SET(seal->opened, 4, 30, 18, 10, 2, 1);
	seal->flags  = FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN;
	seal->facing = FACING_SOUTH;
	seal->destX  = 654;
	seal->destZ  = 132;
	METATILE_COPY_SET(seal->closed[0], 12, 55, 36, 40, 4, 3);
	METATILE_COPY_SET(seal->closed[1], 4, 30, 36, 10, 2, 1);
	METATILE_COPY_SET(seal->closed[2], 4, 28, 18, 10, 2, 1);
	seal->actorId = ACTOR_NORTHEAST_STATUE;
	seal->actorX  = 664;
	seal->actorZ  = 152;
	scSolSanctumStatueOpenSeal();
}

// Opens the southwest seal (scSolSanctumStatueOpenSeal): the metatiles it lights, its flag, where
// the leader jumps back to and where its statue slides back to if the seal closes again.
static void scSolSanctumStatueOpenSouthwestSeal(void)
{
	SEAL_OPENING *seal = &SealOpening;
	METATILE_COPY_SET(seal->open, 0, 58, 32, 43, 4, 1);
	METATILE_COPY_SET(seal->pulseA, 2, 31, 34, 11, 2, 1);
	METATILE_COPY_SET(seal->pulseB, 2, 29, 34, 11, 2, 1);
	METATILE_COPY_SET(seal->opened, 2, 31, 16, 11, 2, 1);
	seal->flags  = FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN;
	seal->facing = FACING_NORTH;
	seal->destX  = 500;
	seal->destZ  = 216;
	METATILE_COPY_SET(seal->closed[0], 8, 58, 32, 43, 4, 1);
	METATILE_COPY_SET(seal->closed[1], 2, 31, 34, 11, 2, 1);
	METATILE_COPY_SET(seal->closed[2], 2, 29, 16, 11, 2, 1);
	seal->actorId = ACTOR_SOUTHWEST_STATUE;
	seal->actorX  = 488;
	seal->actorZ  = 200;
	scSolSanctumStatueOpenSeal();
}

// Opens the southeast seal (scSolSanctumStatueOpenSeal): the metatiles it lights, its flag, where
// the leader jumps back to and where its statue slides back to if the seal closes again.
static void scSolSanctumStatueOpenSoutheastSeal(void)
{
	SEAL_OPENING *seal = &SealOpening;
	METATILE_COPY_SET(seal->open, 4, 58, 36, 43, 4, 1);
	METATILE_COPY_SET(seal->pulseA, 4, 31, 36, 11, 2, 1);
	METATILE_COPY_SET(seal->pulseB, 4, 29, 36, 11, 2, 1);
	METATILE_COPY_SET(seal->opened, 4, 31, 18, 11, 2, 1);
	seal->flags  = FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN;
	seal->facing = FACING_NORTH;
	seal->destX  = 654;
	seal->destZ  = 216;
	METATILE_COPY_SET(seal->closed[0], 12, 58, 36, 43, 4, 1);
	METATILE_COPY_SET(seal->closed[1], 4, 31, 36, 11, 2, 1);
	METATILE_COPY_SET(seal->closed[2], 4, 29, 18, 11, 2, 1);
	seal->actorId = ACTOR_SOUTHEAST_STATUE;
	seal->actorX  = 664;
	seal->actorZ  = 200;
	scSolSanctumStatueOpenSeal();
}

// Copies a seal's metatile copy (METATILE_COPY) onto the map.
#define COPY_METATILES(copy)                                                                       \
	fdMapGridCopyMetatilesTo(                                                                      \
		(copy).srcX, (copy).srcY, (copy).destX, (copy).destY, (copy).width, (copy).height)

// The opened seal pulses this many times, a glitter at each.
#define SEAL_PULSES 20

// Animates the seal and either advances the story or resets its statue.
static void scSolSanctumStatueOpenSeal(void)
{
	SEAL_OPENING *seal;
	Sint32        exitResult;
	Sint32        pulse;

	exitResult = 0;
	fdScriptStart();
	if (!gmFlagGet(FLAG_SOL_SANCTUM_ALL_SEALS_OPEN))
	{
		fdCameraSetSpeed(2);
		fdCameraMoveTo(576, CAMERA_KEEP, 172, 1);
		fdCameraWaitForMove();
		sySoundPlay(SE_SOL_SANCTUM_LIGHT_UP);
		COPY_METATILES(SealOpening.open);
		for (pulse = 0; pulse != SEAL_PULSES; pulse++)
		{
			sySoundPlay(SE_GLITTER);
			COPY_METATILES(SealOpening.pulseA);
			fdScriptWait(4);
			sySoundPlay(SE_GLITTER);
			COPY_METATILES(SealOpening.pulseB);
			fdScriptWait(4);
		}
		seal = &SealOpening;
		COPY_METATILES(seal->opened);
		gmFlagSet(seal->flags);
		exitResult = scSolSanctumStatueSelectStoryExit();
		if (exitResult == -1)
		{
			if (!gmFlagGet(FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED))
			{
				fdCameraFollowActor(ACTOR_PARTY_LEADER, 1);
				fdActorGet(ACTOR_PARTY_LEADER)->facing = seal->facing;
				fdActorSetSpeedAndAcceleration(ACTOR_PARTY_LEADER, 2, 2);
				fdActorGet(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
				fdActorJump(ACTOR_PARTY_LEADER, 4, 0);
				fdActorSetDestination(ACTOR_PARTY_LEADER, seal->destX, seal->destZ);
				COPY_METATILES(seal->closed[0]);
				COPY_METATILES(seal->closed[1]);
				COPY_METATILES(seal->closed[2]);
				fdActorGet(seal->actorId)->steeringFlags &= ~ACTOR_FACES_MOTION;
				fdActorMoveToAndWait(seal->actorId, seal->actorX, seal->actorZ);
				fdActorGet(ACTOR_PARTY_LEADER)->steeringFlags |= ACTOR_FACES_MOTION;
				gmFlagClear(seal->flags);
			}
		}
		else if (exitResult == 0 && gmFlagGet(FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED))
		{
			// Unused: scSolSanctumStatueSelectStoryExit returns only 1 or -1, so this branch cannot
			// run.
			if (gmFlagGet(FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN)
			    && gmFlagGet(FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN)
			    && gmFlagGet(FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN))
			{
				if (!fdScriptGetFlag(FLAG_SOL_SANCTUM_ALL_SEALS_OPEN))
				{
					gmFlagSet(FLAG_SOL_SANCTUM_ALL_SEALS_OPEN);
					scSolSanctumStatueUnusedPutOutSeals();
				}
			}
			else if (gmFlagGet(FLAG_SOL_SANCTUM_FALLING_LIGHTS_SEEN))
			{
				syImportScriptSetExitRequest(
					SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_LUNA_SOL_ROOMS_LIGHTS_SHOWN);
				exitResult = 1;
			}
		}
	}
	if (exitResult == 1)
	{
		fdScriptFinishScreenTransition();
		fdScriptWaitTransitionTicks();
	}
	fdScriptEnd();
}

// Draws the left beam and opens the floor when the right beam is also lit.
static void scSolSanctumStatueShineLeftBeam(void)
{
	fdScriptStart();
	if (gmFlagGet(FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED) == 0)
	{
		if (fdScriptGetFlag(FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING) == 0)
		{
			fdCameraSetSpeed(2);
			fdCameraMoveTo(286, CAMERA_KEEP, 146, 1);
			fdCameraWaitForMove();
			sySoundPlay(SE_SOL_SANCTUM_LIGHT_UP);
			DRAW_LEFT_BEAM();
			// With the other beam already shining, the two now cross.
			if (fdScriptGetFlag(FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING) != 0)
			{
				DRAW_CROSSED_BEAMS();
			}
			fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
			fdScriptWait(30);
			gmFlagSet(FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING);
			if (gmFlagGet(FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING) != 0)
			{
				scSolSanctumStatueOpenCenterHole();
			}
		}
	}
	fdScriptEnd();
}

// Draws the right beam and opens the floor when the left beam is also lit.
static void scSolSanctumStatueShineRightBeam(void)
{
	fdScriptStart();
	if (gmFlagGet(FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED) == 0)
	{
		if (fdScriptGetFlag(FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING) == 0)
		{
			fdCameraSetSpeed(2);
			fdCameraMoveTo(286, CAMERA_KEEP, 146, 1);
			fdCameraWaitForMove();
			sySoundPlay(SE_SOL_SANCTUM_LIGHT_UP);
			DRAW_RIGHT_BEAM();
			// With the other beam already shining, the two now cross.
			if (fdScriptGetFlag(FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING) != 0)
			{
				DRAW_CROSSED_BEAMS();
			}
			fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
			fdScriptWait(30);
			gmFlagSet(FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING);
			if (gmFlagGet(FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING) != 0)
			{
				scSolSanctumStatueOpenCenterHole();
			}
		}
	}
	fdScriptEnd();
}

// Shows the next message of the script, then waits.
static void scSolSanctumStatueSayThenWait(Sint32 speaker, Sint32 ticks)
{
	fdScriptShowMessage(speaker, 0);
	fdScriptWait(ticks);
}

SCENE_ENTRANCE SolSanctumStatueEntrances[] = {
	{SOL_SANCTUM_STATUE_FROM_VALE_PLAZA,
     CONDITION_ALWAYS, 287,
     0, 176,
     FACING_SOUTH, 0,
     -1,
     -1,
     -1,
     -1,
     0},
	{SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS,
     CONDITION_ALWAYS, 646,
     0, 329,
     FACING_SOUTH, 0,
     -1,
     -1,
     -1,
     -1,
     0},
	{SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_PSYNERGY_STONE,
     CONDITION_ALWAYS, 647,
     0, 470,
     FACING_SOUTH, 0,
     -1,
     -1,
     -1,
     -1,
     0},
	{SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_DISARMED,
     CONDITION_ALWAYS, 576,
     0, 136,
     FACING_SOUTH, 0,
     -1,
     -1,
     -1,
     -1,
     0},
	{SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_TRIGGERED,
     CONDITION_ALWAYS, 576,
     0, 136,
     FACING_SOUTH, 0,
     -1,
     -1,
     -1,
     -1,
     0},
	{SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS_LIGHTS_SHOWN,
     CONDITION_ALWAYS, 576,
     0, 136,
     FACING_SOUTH, 0,
     -1,
     -1,
     -1,
     -1,
     0},
	{SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS_UNUSED,
     CONDITION_ALWAYS, 576,
     0, 136,
     FACING_SOUTH, 0,
     -1,
     -1,
     -1,
     -1,
     0},
	{SOL_SANCTUM_STATUE_DEBUG_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS,
     CONDITION_ALWAYS, 576,
     0, 136,
     FACING_SOUTH, 0,
     -1,
     -1,
     -1,
     -1,
     0},
	{SCENE_TABLE_END},
};

Uint32 SolSanctumStatueExits[] = {
	SCENE_EXITS(SolSanctumStatue),
	SCENE_EXIT(SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_LUNA_SOL_ROOMS,
               SolSanctumLunaSolRooms,
               SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE),
	SCENE_EXIT(SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_PSYNERGY_STONE,
               SolSanctumPsynergyStone,
               SOL_SANCTUM_PSYNERGY_STONE_FROM_SOL_SANCTUM_STATUE),
	SCENE_EXIT(SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_DISARMED,
               SolSanctumLunaSolRooms,
               SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE_TRAP_DISARMED),
	SCENE_EXIT(SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_TRIGGERED,
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

// clang-format off
ACTOR_PLACE SolSanctumStatueActorPlacements[] = {
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
// clang-format on

CUE SolSanctumStatueCues[] = {
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN,
     SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_LUNA_SOL_ROOMS, CONDITION_ALWAYS,
     SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_LUNA_SOL_ROOMS},
	{CUE_EXIT,
     SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_PSYNERGY_STONE, CONDITION_ALWAYS,
     SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_PSYNERGY_STONE},
	{STEP_CUE_WALKING,
     METATILE_BEHAVIOR_SOL_SANCTUM_STATUE_JUMP_NORTH, CONDITION_ALWAYS,
     SCRIPT_REF(scSolSanctumStatueJumpNorth)},
	{STEP_CUE_WALKING,
     METATILE_BEHAVIOR_SOL_SANCTUM_STATUE_JUMP_SOUTH, CONDITION_ALWAYS,
     SCRIPT_REF(scSolSanctumStatueJumpSouth)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE),
     ACTOR_NORTHWEST_STATUE, FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN,
     SCRIPT_REF(scSolSanctumStatueNorthwestStatueMoved)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE),
     ACTOR_NORTHEAST_STATUE, FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN,
     SCRIPT_REF(scSolSanctumStatueNortheastStatueMoved)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE),
     ACTOR_SOUTHWEST_STATUE, FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN,
     SCRIPT_REF(scSolSanctumStatueSouthwestStatueMoved)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE),
     ACTOR_SOUTHEAST_STATUE, FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN,
     SCRIPT_REF(scSolSanctumStatueSoutheastStatueMoved)},
	{STEP_CUE_WALKING_INTO(FACING_EAST),
     METATILE_BEHAVIOR_SOL_SANCTUM_STATUE_NORTHWEST_STATUE, FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN,
     SCRIPT_REF(scSolSanctumStatuePushNorthwestStatue)},
	{STEP_CUE_WALKING_INTO(FACING_WEST),
     METATILE_BEHAVIOR_SOL_SANCTUM_STATUE_NORTHEAST_STATUE, FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN,
     SCRIPT_REF(scSolSanctumStatuePushNortheastStatue)},
	{STEP_CUE_WALKING_INTO(FACING_EAST),
     METATILE_BEHAVIOR_SOL_SANCTUM_STATUE_SOUTHWEST_STATUE, FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN,
     SCRIPT_REF(scSolSanctumStatuePushSouthwestStatue)},
	{STEP_CUE_WALKING_INTO(FACING_WEST),
     METATILE_BEHAVIOR_SOL_SANCTUM_STATUE_SOUTHEAST_STATUE, FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN,
     SCRIPT_REF(scSolSanctumStatuePushSoutheastStatue)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_LEFT_BEAM_STATUE, FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING,
     SCRIPT_REF(scSolSanctumStatuePrepareMoveOnLeftBeamStatue)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE),
     ACTOR_LEFT_BEAM_STATUE, FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING,
     SCRIPT_REF(scSolSanctumStatueLeftBeamStatueMoved)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_RIGHT_BEAM_STATUE, FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING,
     SCRIPT_REF(scSolSanctumStatuePrepareMoveOnRightBeamStatue)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE),
     ACTOR_RIGHT_BEAM_STATUE, FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING,
     SCRIPT_REF(scSolSanctumStatueRightBeamStatueMoved)},
	{STEP_CUE_WALKING_INTO(FACING_EAST),
     METATILE_BEHAVIOR_SOL_SANCTUM_STATUE_LEFT_TRACK_OUTSIDE_TOP, FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING,
     SCRIPT_REF(scSolSanctumStatuePushLeftBeamStatueEast)},
	{STEP_CUE_WALKING_INTO(FACING_EAST),
     METATILE_BEHAVIOR_SOL_SANCTUM_STATUE_LEFT_TRACK_TOP, FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING,
     SCRIPT_REF(scSolSanctumStatuePushLeftBeamStatueEast)},
	{STEP_CUE_WALKING_INTO(FACING_SOUTH),
     METATILE_BEHAVIOR_SOL_SANCTUM_STATUE_LEFT_TRACK_TOP, FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING,
     SCRIPT_REF(scSolSanctumStatuePushLeftBeamStatueSouth)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH),
     METATILE_BEHAVIOR_SOL_SANCTUM_STATUE_LEFT_TRACK_THIRD, FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING,
     SCRIPT_REF(scSolSanctumStatuePushLeftBeamStatueNorth)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH),
     METATILE_BEHAVIOR_SOL_SANCTUM_STATUE_LEFT_TRACK_BOTTOM, FLAG_SOL_SANCTUM_LEFT_BEAM_SHINING,
     SCRIPT_REF(scSolSanctumStatuePushLeftBeamStatueNorth)},
	{STEP_CUE_WALKING_INTO(FACING_WEST),
     METATILE_BEHAVIOR_SOL_SANCTUM_STATUE_RIGHT_TRACK_OUTSIDE_TOP, FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING,
     SCRIPT_REF(scSolSanctumStatuePushRightBeamStatueWest)},
	{STEP_CUE_WALKING_INTO(FACING_WEST),
     METATILE_BEHAVIOR_SOL_SANCTUM_STATUE_RIGHT_TRACK_TOP, FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING,
     SCRIPT_REF(scSolSanctumStatuePushRightBeamStatueWest)},
	{STEP_CUE_WALKING_INTO(FACING_SOUTH),
     METATILE_BEHAVIOR_SOL_SANCTUM_STATUE_RIGHT_TRACK_TOP, FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING,
     SCRIPT_REF(scSolSanctumStatuePushRightBeamStatueSouth)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH),
     METATILE_BEHAVIOR_SOL_SANCTUM_STATUE_RIGHT_TRACK_THIRD, FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING,
     SCRIPT_REF(scSolSanctumStatuePushRightBeamStatueNorth)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH),
     METATILE_BEHAVIOR_SOL_SANCTUM_STATUE_RIGHT_TRACK_BOTTOM, FLAG_SOL_SANCTUM_RIGHT_BEAM_SHINING,
     SCRIPT_REF(scSolSanctumStatuePushRightBeamStatueNorth)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_CENTER_STATUE, FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED,
     SCRIPT_REF(scSolSanctumStatuePrepareMoveOnCenterStatue)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE),
     ACTOR_CENTER_STATUE, FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED,
     SCRIPT_REF(scSolSanctumStatueCenterStatueMoved)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH),
     METATILE_BEHAVIOR_SOL_SANCTUM_STATUE_CENTER_STATUE, FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED,
     SCRIPT_REF(scSolSanctumStatuePushCenterStatueNorth)},
	{CUE_TABLE_END},
};

// The clouds' scroll, SCROLL_OFFSET_STEP more each tick.
Sint32 SolSanctumStatueScrollOffset = 0;
