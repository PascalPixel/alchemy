// Colosseum Final 1 (SceneIdColosseumAzartMatch) is the site of the first finals match, where Isaac
// races Azart through four stages to the arena and fights him there.
//
// Azart is the second-best of the warriors who cleared the trials. Babi has put Isaac in the finals
// himself, and only Isaac may race. Garet, Ivan and Mia help from the stages.
//
// The stages, by their attendants' names, are the Rock Challenge, the Pipeworks, the Broken Bridge
// and the Lumber Water Crossing Area.
//
// Isaac comes here three times, each time at its own entrance. scInit blocks Retreat and restores
// the stages from their temporary flags (FLAG_PIPES_MOVED, FLAG_PIPEWORKS_PIPE_PUSHED,
// FLAG_PLANT_GROWN and the rock's column in FLAG_SECOND_ROCK_COLUMN), so that what the friends
// changed stays changed for the race. Then it sets up the visit that entrance stands for.
//
// - COLOSSEUM_AZART_MATCH_FROM_COLOSSEUM, the tour, from the Colosseum's east
//       passage. The guide (scColosseumAzartMatchExplainFinalsRules) tells the
//       party that warriors enter without their own equipment and race through
// the stages to the arena. There the first to arrive takes the better of two
//       prizes and keeps it for the next match. He pans over the stages if
// asked. Each stage's attendant names it, the first time with a demonstration.
//       Isaac appears on the stage to show how it is crossed, or, at the
//       Pipeworks, a pointer shows how the pipes work. The attendant then
// offers a place to cheer there. The friend Isaac picks walks to the stage and
//       leaves the party (scColossoOfferCheeringSpot). When all three are
// placed, the guide sends Isaac on to the Colosseum's waiting room
//       (COLOSSEUM_AZART_MATCH_TO_COLOSSEUM_WEST).
// - COLOSSEUM_AZART_MATCH_FROM_COLOSSEUM_WEST, after the warriors have marched
//       out. The lettering reads `Lend a Hand`, and Garet, Ivan and Mia in turn
//       are the party alone, starting where they chose to cheer. Each may use
//       Psynergy on the stages for Isaac. Douse on the Pipeworks' west pipe
// fills it, Growth grows the plant before the Broken Bridge, and Move shifts the Rock Challenge's
// rocks. A friend's turn ends when they use Psynergy
//       (scColossoTakePlaceForMatch) or tell a stage's attendant they are done
//       (scColossoAskDoneCheering), and where they stand is kept as their
// place. After Mia, COLOSSEUM_AZART_MATCH_LEND_A_HAND_TO_RACE starts the race.
// - COLOSSEUM_AZART_MATCH_RACE_FROM_LEND_A_HAND, the race. After the lettering
//       `The 1st Match` and a count of three, Isaac runs alone against Azart,
// who follows a recorded path (scColossoStartOpponent). A gauge at the top of
//       the screen shows how near each is to the finish
// (scColossoStartRaceGauge). Azart runs east of the finish, which is 1,304 pixels across, where the
// prizes stand. His path is the mirror of one through Isaac's own stages, so he is seen only as he
// comes into the arena. At the arena (scColosseumAzartMatchReachArena) the first to arrive takes
// the Iron
//       Shield and the second the Padded Gloves, which Isaac is given and
// equipped with. Then the two fight. The battle is ENCOUNTER_ZONE_COLOSSO's first or second group,
// the Azart who has the shield or the one who has not.
//
// A win returns to COLOSSEUM_AZART_MATCH_AFTER_MATCH_WON, which shows the result and sends Isaac
// back to the Colosseum (COLOSSEUM_AZART_MATCH_TO_COLOSSEUM_EAST, to
// COLOSSEUM_FROM_COLOSSEUM_AZART_MATCH_EAST).
//
// A loss returns to COLOSSEUM_AZART_MATCH_AFTER_MATCH_LOST, and
// COLOSSEUM_AZART_MATCH_TO_BABI_PALACE wakes the party in Babi's Palace, where Isaac's friends tell
// him he was only dreaming of the finals.

#include "ability_ids.h"
#include "item_ids.h"
#include "macros.h"
#include "resource_ids.h"
#include "debug/debug.h"
#include "field/actor_script.h"
#include "field/script_inline.h"
#include "field/pushed_actor.h"
#include "graphics/graphics.h"
#include "scene/colosso_gauge.h"
#include "scene/colosso_guide.h"
#include "scene/colosso_lettering.h"
#include "scene/colosso_push.h"
#include "scene/colosso_setup.h"
#include "scene/colosso_solo.h"
#include "sound/sound_driver_symbols.h"
#include "text/msg_ids.h"

// The match held here, as the lettering `The 1st Match` reads; the friends' turns open with `Lend a
// Hand!` instead (scColossoShowOpeningLettering).
enum
{
	FINALS_MATCH = 1
};

// The stages, as their attendants number them: "This is stage 1 of the first finals".
typedef enum COLOSSEUM_AZART_MATCH_STAGE
{
	STAGE_ROCK_CHALLENGE        = 1,
	STAGE_PIPEWORKS             = 2,
	STAGE_BROKEN_BRIDGE         = 3,
	STAGE_LUMBER_WATER_CROSSING = 4
} COLOSSEUM_AZART_MATCH_STAGE;

// The shared Colosso code's state, in the order the scene's memory lays it out; the shared files
// that use it say what each holds: the guide's pointer (colosso_guide.c), the lettering
// (colosso_task.c) and where Isaac stood before he was shown on a stage (colosso_guide.c).
Sint16 ColossoPointerStep                     BSS_DATA;
Sint16 ColossoLetteringScaleDuration BSS_DATA ALIGNED(4);
Sint16 ColossoPointerFlip BSS_DATA            ALIGNED(4);
// Isaac's z before he is shown on the stage, 16.16 fixed-point pixels.
FIXED ColossoWarriorStartZ                    BSS_DATA;
Sint16 ColossoPointerEndX                     BSS_DATA;
Sint16 ColossoLetteringParameter BSS_DATA     ALIGNED(4);
Sint16 ColossoLetteringScaleStart BSS_DATA    ALIGNED(4);
Sint16 ColossoLetteringMoveDuration BSS_DATA  ALIGNED(4);
Sint32 ColossoLetteringPosition               BSS_DATA;
Sint16 ColossoPointerBlink                    BSS_DATA;
Sint16 ColossoLetteringScaleTarget BSS_DATA   ALIGNED(4);
Sint16 ColossoLetteringMoveStep BSS_DATA      ALIGNED(4);
Sint16 ColossoPointerY BSS_DATA               ALIGNED(4);
Sint16 ColossoLetteringFadeStep BSS_DATA      ALIGNED(4);
// Isaac's facing before scColossoFadeInWarrior, which scColossoRestoreWarrior puts back.
Sint32 ColossoWarriorStartAngle               BSS_DATA;
Sint16 ColossoPointerSteps                    BSS_DATA;
Sint16 ColossoLetteringMode BSS_DATA          ALIGNED(4);
Sint16 ColossoLetteringFadeTarget BSS_DATA    ALIGNED(4);
Sint16 ColossoLetteringFadeStart BSS_DATA     ALIGNED(4);
Sint16 ColossoLetteringTimer BSS_DATA         ALIGNED(4);
Sint16 *ColossoLetteringScript                BSS_DATA;
POINTER_COORDINATE ColossoPointerStartX       BSS_DATA;
Sint16 ColossoLetteringFadeDuration BSS_DATA  ALIGNED(4);
Uint8                                      ColosseumAzartMatchUnusedAfterBlendDuration[6] BSS_DATA;
// The blinking pointer's sprite part: the queue link and attributes 0 to 2.
Sint32                                     ColossoPointerOam[3] BSS_DATA;
POINTER_COORDINATE ColossoPointerStartY    BSS_DATA;
// The lettering's sprite parts, three words each, written every tick.
Uint32                                     ColossoLetteringSprites[12] BSS_DATA;
Sint16 ColossoLetteringMoveStart           BSS_DATA;
Sint16 ColossoPointerX BSS_DATA            ALIGNED(4);
Sint16 ColossoLetteringMoveTarget BSS_DATA ALIGNED(4);
Sint16 ColossoLetteringScaleStep BSS_DATA  ALIGNED(4);
Sint16 ColossoPointerEndY BSS_DATA         ALIGNED(4);
// Isaac's x before he is shown on the stage, 16.16 fixed-point pixels.
FIXED ColossoWarriorStartX                 BSS_DATA;

Sint32                                     scInit(void);
extern SCENE_ENTRANCE                      ColosseumAzartMatchEntrances[];
extern Uint32                              ColosseumAzartMatchExits[];
extern ACTOR_PLACE                         ColosseumAzartMatchActorPlacements[];
extern Uint32                              ColosseumAzartMatchBridgeTimer;
extern CUE                                 ColosseumAzartMatchCues[];
extern char                                HexDigits[20];

// UB: each of these calls a void function through an Sint32 function type, the call the bytes show;
// UBFIX calls it as declared.
#if FIX_FUNCTION_SIGNATURES
#	define COLOSSEUM_AZART_MATCH_FACE_DIRECTION    fdActorFaceDirectionById
#	define COLOSSEUM_AZART_MATCH_PLACE_STAGED_PAIR scColosseumAzartMatchMoveActorPairByOffset
#else
#	define COLOSSEUM_AZART_MATCH_FACE_DIRECTION ((Sint32 (*)())fdActorFaceDirectionById)
#	define COLOSSEUM_AZART_MATCH_PLACE_STAGED_PAIR                                                \
		((Sint32 (*)())scColosseumAzartMatchMoveActorPairByOffset)
#endif

static void   scColosseumAzartMatchExplainFinalsRules(Sint32 guide);
static void   scColossoShowOpeningLettering(Sint32 match);
static void   scColossoShowMatchResult(Sint32 result);
static void   scColossoNoOperation(void);
static void   scColossoWaitForOpponentToFinish(void);
static Sint32 scColossoTakePrize(Sint32 firstItem, Sint32 secondItem);
static void   scColossoPushActorAhead(void);

typedef void  (*MAP_TASK)(void);

// The first finals match's actors, by the guide's and attendants' explanations.
//
// Four stages lead to the arena where the two warriors fight: the Rock Challenge, the Pipeworks,
// the Broken Bridge and the Lumber Water Crossing Area.
enum
{
	// Azart, racing Isaac to the arena.
	ACTOR_OPPONENT            = 8,
	// The Rock Challenge's three rocks, stone pillars the party moves to clear a path.
	ACTOR_ROCK_A              = 9,
	ACTOR_ROCK_B              = 10,
	ACTOR_ROCK_C              = 11,
	// A loose length of pipe, pushed west to let water into the Pipeworks' channel. It is drawn as
	// mercury_lighthouse.c's ACTOR_THIRD_PIPE.
	ACTOR_PIPEWORKS_PIPE      = 12,
	// The white pans hanging under the Pipeworks' two water pipes, which rise and sink against each
	// other as water fills one, like scales.
	ACTOR_PIPE_WEST           = 13,
	ACTOR_PIPE_EAST           = 14,
	// The plant Growth grows into a vine before the Broken Bridge.
	ACTOR_GROWTH_PLANT        = 15,
	// A young man with fair hair who works the Broken Bridge.
	ACTOR_BRIDGE_OPERATOR     = 16,
	// The Lumber Water Crossing Area's two floating logs.
	ACTOR_LOG_NORTH_SOUTH     = 17,
	ACTOR_LOG_EAST_WEST       = 18,
	// A soldier who explains the finals.
	ACTOR_GUIDE               = 19,
	// The stages' attendants, young men with fair hair in white tunics.
	ACTOR_ROCK_ATTENDANT      = 20,
	ACTOR_PIPEWORKS_ATTENDANT = 21,
	ACTOR_BRIDGE_ATTENDANT    = 22,
	ACTOR_LUMBER_ATTENDANT    = 23,
	// The arena's prizes, drawn as item icons at two blank actors (scColossoShowItemIcon).
	ACTOR_PRIZE_A             = 24,
	ACTOR_PRIZE_B             = 25
};

// The temporary flags that keep what the friends changed on the stages, for the race.
//
// scInit restores them.
enum
{
	// the Pipeworks' pipes, as water filled them
	FLAG_PIPES_MOVED           = FLAG_AREA_TEMP_01,
	FLAG_PIPEWORKS_PIPE_PUSHED = FLAG_AREA_TEMP_02,
	// by Growth, before the Broken Bridge
	FLAG_PLANT_GROWN           = FLAG_AREA_TEMP_03,
	// The byte of flags that keeps the Rock Challenge's second rock's column.
	FLAG_SECOND_ROCK_COLUMN    = FLAG_AREA_TEMP_10
};

// The chests along the stages have been opened: a Nut, an Oil Drop and a second Nut, from west to
// east.
#define FLAG_COLOSSEUM_AZART_MATCH_FIRST_NUT_CHEST_OPENED  FLAG_AREA_TEMP_08
#define FLAG_COLOSSEUM_AZART_MATCH_OIL_DROP_CHEST_OPENED   FLAG_AREA_TEMP_09
#define FLAG_COLOSSEUM_AZART_MATCH_SECOND_NUT_CHEST_OPENED FLAG_AREA_TEMP_0A

// Metatile behaviors the stages' cues answer.
typedef enum COLOSSEUM_AZART_MATCH_METATILE_BEHAVIOR
{
	// The Rock Challenge's floor round its pillars, (17..21, 9..11), where walking into a pillar
	// pushes it.
	METATILE_BEHAVIOR_ROCK_CHALLENGE_FLOOR       = 10,
	// Metatile (27, 11), at the Rock Challenge's east end, where walking into a pillar pushes it
	// too.
	METATILE_BEHAVIOR_ROCK_CHALLENGE_EAST_END    = 11,
	// The east edge of the Rock Challenge's raised square, metatile (22, 12).
	METATILE_BEHAVIOR_ROCK_CHALLENGE_SQUARE_EDGE = 90,
	// East of the Pipeworks' loose pipe, metatiles (38, 7..10), where walking west pushes it.
	METATILE_BEHAVIOR_PIPEWORKS_PIPE             = 12,
	// On the north-south log, as at (64..66, 24): walking north or south rides it.
	METATILE_BEHAVIOR_LOG_NORTH_SOUTH            = 20,
	// On the east-west log, as at (63, 25..27): walking east or west rides it.
	METATILE_BEHAVIOR_LOG_EAST_WEST              = 21,
	// The way into the arena, metatiles (79, 9..14).
	METATILE_BEHAVIOR_ARENA_ENTRY                = 50,
	// The chests: a Nut at (28, 5), an Oil Drop at (48, 6) and a Nut at (61, 13), opened facing
	// west.
	METATILE_BEHAVIOR_FIRST_NUT_CHEST            = 100,
	METATILE_BEHAVIOR_OIL_DROP_CHEST             = 101,
	METATILE_BEHAVIOR_SECOND_NUT_CHEST           = 103
} COLOSSEUM_AZART_MATCH_METATILE_BEHAVIOR;

// Metatiles kept off the course whose attributes (behaviour and height) the stages' changes copy.
typedef enum COLOSSEUM_AZART_MATCH_SPARE_METATILE
{
	// A square to hop to (METATILE_BEHAVIOR_JUMP_FORWARD), stamped on a pillar's top.
	HOP_SQUARE_COLUMN      = 14,
	HOP_SQUARE_ROW         = 13,
	// Three metatiles of low ground with no behaviour, laid over the Rock Challenge's (23..25, 12)
	// before the pillars' tops are stamped again.
	LOW_GROUND_COLUMN      = 27,
	LOW_GROUND_ROW         = 13,
	// The grown vine's metatile, climbed (METATILE_BEHAVIOR_CLIMB).
	VINE_COLUMN            = 47,
	VINE_ROW               = 24,
	// The Broken Bridge's row of six, (50..55, 12), with the bridge lying down and standing up.
	BRIDGE_DOWN_ROW        = 25,
	BRIDGE_UP_ROW          = 24,
	// Three metatiles to ride the north-south log (METATILE_BEHAVIOR_LOG_NORTH_SOUTH), across, and
	// three to ride the east-west one, down.
	LOG_NORTH_SOUTH_COLUMN = 64,
	LOG_NORTH_SOUTH_ROW    = 24,
	LOG_EAST_WEST_COLUMN   = 63,
	LOG_EAST_WEST_ROW      = 25
} COLOSSEUM_AZART_MATCH_SPARE_METATILE;

// Pictures kept on layer 1 (map rows 28 and down) at columns 92 to 100: the Broken Bridge's spans
// standing up, swinging and lying down, two metatiles square, drawn at the spans' places (50, 38)
// and (54, 38); and the five steps of water pouring into the Pipeworks' channel, one metatile by
// three each, drawn at (34, 38).
typedef enum COLOSSEUM_AZART_MATCH_PICTURE_METATILE
{
	BRIDGE_PICTURE_COLUMN       = 92,
	BRIDGE_UP_PICTURE_ROW       = 29,
	BRIDGE_SWINGING_PICTURE_ROW = 31,
	BRIDGE_DOWN_PICTURE_ROW     = 33,
	WATER_PICTURE_COLUMN        = 96,
	WATER_PICTURE_ROW           = 29
} COLOSSEUM_AZART_MATCH_PICTURE_METATILE;

// The Broken Bridge's cycle, in ticks, counted down: the bridge lies down, walkable, for the last
// 60 of each.
#define BRIDGE_CYCLE_TICKS        120

// The Lumber Water Crossing Area's logs float on fixed lines. The north-south log lies across
// columns 64 to 66 and floats three metatiles between row 8 and LOG_SOUTH_ROW, where it bars the
// way of the east-west log; that one lies across rows 9 to 11 and floats ten metatiles from the
// west bank's column 63 to the east bank, or six from column 67, where the other log stops it.
#define LOG_NORTH_ROW             8
#define LOG_SOUTH_ROW             11
#define LOG_NORTH_SOUTH_FLOAT     48
#define LOG_EAST_WEST_FLOAT       160
#define LOG_EAST_WEST_SHORT_FLOAT 96

// The Broken Bridge's operator's poses as the bridge swings (young_man_with_fair_hair.tsv).
typedef enum BRIDGE_OPERATOR_ANIMATION
{
	BRIDGE_OPERATOR_ANIMATION_SWINGING    = 10, // as the bridge starts to move
	BRIDGE_OPERATOR_ANIMATION_BRIDGE_DOWN = 11,
	BRIDGE_OPERATOR_ANIMATION_BRIDGE_UP   = 12
} BRIDGE_OPERATOR_ANIMATION;

// SPRITE_PIPE_NORTH_SOUTH's animations, as the Pipeworks' loose pipe is pushed
// (pipe_north_south.tsv).
typedef enum PIPE_ANIMATION
{
	PIPE_ANIMATION_STILL   = 1,
	PIPE_ANIMATION_ROLLING = 2 // its four pictures in turn
} PIPE_ANIMATION;

// SPRITE_WHITE_PLATFORM's animations for the Pipeworks' pans (white_platform.tsv).
typedef enum PAN_ANIMATION
{
	PAN_ANIMATION_EMPTY = 2,
	PAN_ANIMATION_FULL  = 3 // water in it
} PAN_ANIMATION;

const SCENE_ENTRANCE *scGetEntrances(void)
{
	return ColosseumAzartMatchEntrances;
}

const SCENE_EXIT_REGION *scGetExitRegions(void)
{
	return 0;
}

const Uint32 *scGetExits(void)
{
	return ColosseumAzartMatchExits;
}

const ACTOR_PLACE *scGetActorPlacements(void)
{
	return ColosseumAzartMatchActorPlacements;
}

// Advances the bridge animation and the metatile behaviors that make it walkable.
static void scColosseumAzartMatchUpdateBrokenBridge(void)
{
	switch (ColosseumAzartMatchBridgeTimer)
	{
		case 66:
			fdMapGridCopyMetatilesLayered(
				BRIDGE_PICTURE_COLUMN, BRIDGE_SWINGING_PICTURE_ROW, 2, 2, 50, 38);
			fdMapGridCopyMetatilesLayered(
				BRIDGE_PICTURE_COLUMN, BRIDGE_SWINGING_PICTURE_ROW, 2, 2, 54, 38);
			fdActorSetAnimation(ACTOR_BRIDGE_OPERATOR, BRIDGE_OPERATOR_ANIMATION_SWINGING);
			break;
		case 60:
			// The bridge lies down, and its metatiles can be walked on.
			fdMapGridCopyMetatilesLayered(
				BRIDGE_PICTURE_COLUMN, BRIDGE_DOWN_PICTURE_ROW, 2, 2, 50, 38);
			fdMapGridCopyMetatilesLayered(
				BRIDGE_PICTURE_COLUMN, BRIDGE_DOWN_PICTURE_ROW, 2, 2, 54, 38);
			fdMapGridCopyMetatileAttributes(50, BRIDGE_DOWN_ROW, 6, 1, 50, 12);
			fdActorSetAnimation(ACTOR_BRIDGE_OPERATOR, BRIDGE_OPERATOR_ANIMATION_BRIDGE_DOWN);
			break;
		case 6:
			fdMapGridCopyMetatilesLayered(
				BRIDGE_PICTURE_COLUMN, BRIDGE_SWINGING_PICTURE_ROW, 2, 2, 50, 38);
			fdMapGridCopyMetatilesLayered(
				BRIDGE_PICTURE_COLUMN, BRIDGE_SWINGING_PICTURE_ROW, 2, 2, 54, 38);
			fdActorSetAnimation(ACTOR_BRIDGE_OPERATOR, BRIDGE_OPERATOR_ANIMATION_SWINGING);
			break;
		case 0:
			// The bridge stands up again, and its metatiles are a gap once more.
			fdMapGridCopyMetatilesLayered(
				BRIDGE_PICTURE_COLUMN, BRIDGE_UP_PICTURE_ROW, 2, 2, 50, 38);
			fdMapGridCopyMetatilesLayered(
				BRIDGE_PICTURE_COLUMN, BRIDGE_UP_PICTURE_ROW, 2, 2, 54, 38);
			fdActorSetAnimation(ACTOR_BRIDGE_OPERATOR, BRIDGE_OPERATOR_ANIMATION_BRIDGE_UP);
			fdMapGridCopyMetatileAttributes(50, BRIDGE_UP_ROW, 6, 1, 50, 12);
			ColosseumAzartMatchBridgeTimer = BRIDGE_CYCLE_TICKS;
			break;
	}
	ColosseumAzartMatchBridgeTimer--;
}

// Stops the bridge event and restores its closed tick.
static void scColosseumAzartMatchResetBrokenBridge(void)
{
	MAP_TASK task;

	ColosseumAzartMatchBridgeTimer = 0;
	task                           = scColosseumAzartMatchUpdateBrokenBridge;
	syCloseEvent(task);
	task();
}

// Starts the broken bridge opening and closing, over and over.
static void scColosseumAzartMatchStartBrokenBridge(void)
{
	ColosseumAzartMatchBridgeTimer = 66;
	sySetTickEvent(scColosseumAzartMatchUpdateBrokenBridge, EVENT_PASS_TICK);
}

// Waits for the crossing window, bounded by one bridge cycle.
static void scColosseumAzartMatchWaitForBridgeOpening(void)
{
	Sint32 i;

	sySynch(10);
	for (i = 0; i < BRIDGE_CYCLE_TICKS && ColosseumAzartMatchBridgeTimer != 22; i++)
		sySynch(1);
}

// Move on one of the stone pillars in the Rock Challenge, the first stage, slides it a metatile,
// which can clear the way or give the racers a step.
//
// The same work follows every push. Once the first pillar has come down to the low ground, level
// with its floor, it is drawn under the party and its top becomes a square to hop across. The
// second pillar's top is always such a square, and the column it stands in is kept in
// FLAG_SECOND_ROCK_COLUMN, so that a friend's Move during Lend a Hand still holds for the race.
static void scColosseumAzartMatchUpdateRockMetatiles(void)
{
	ACTOR *actor;
	FIXED  blocked;

	fdMapGridCopyMetatileAttributes(LOW_GROUND_COLUMN, LOW_GROUND_ROW, 3, 1, 23, 12);
	actor   = fdActorGet(ACTOR_ROCK_A);
	blocked = fdMapGridGetTerrainHeight(0, actor->x.fixed, actor->z.fixed);
	if (actor->y.fixed == 0 && blocked == 0)
	{
		actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
		actor->motionFlags   = 0;
		{
			Sint32 x = FIELD_METATILE(actor->x.fixed);
			Sint32 y = FIELD_METATILE(actor->z.fixed);

			fdMapGridCopyMetatileAttributes(HOP_SQUARE_COLUMN, HOP_SQUARE_ROW, 1, 1, x, y);
		}
	}
	actor = fdActorGet(ACTOR_ROCK_B);
	{
		Sint32 x = FIELD_METATILE(actor->x.fixed);

		syFieldGameFlagSetByte(FLAG_SECOND_ROCK_COLUMN, x);
	}
	{
		Sint32 x = FIELD_METATILE(actor->x.fixed);
		Sint32 y = FIELD_METATILE(actor->z.fixed);

		fdMapGridCopyMetatileAttributes(HOP_SQUARE_COLUMN, HOP_SQUARE_ROW, 1, 1, x, y);
	}
}

// Walking into a stone pillar in the Rock Challenge pushes it a metatile, with Isaac leaning on it,
// if nothing is in the way.
static void scColosseumAzartMatchPushRock(void)
{
	scColossoPushActorAhead();
	scColosseumAzartMatchUpdateRockMetatiles();
}

// Growth on the sprout before the Broken Bridge, the third stage, grows it into a vine up the wall
// beside the bridge's scaffold (metatile (47, 12)), a shortcut up to the bridge.
static void scColosseumAzartMatchGrowPlant(void)
{
	fdMapGridCopyMetatileAttributes(VINE_COLUMN, VINE_ROW, 1, 1, 47, 12);
	gmFlagSet(FLAG_PLANT_GROWN);
}

// Douse on the white pan under the Pipeworks' west pipe, the second stage, fills it with water. The
// camera moves over, the full pan sinks and the one under the east pipe rises, as the attendant
// says the pipes form a kind of scale, and the way on to the next stage opens (metatile (41, 12)).
static void scColosseumAzartMatchDousePipe(void)
{
	ACTOR *actor;

	gmFlagSet(FLAG_PIPES_MOVED);
	actor = fdActorGet(ACTOR_PIPE_WEST);
	fdScriptStart();
	fdCameraSetSpeed(2);
	fdCameraMoveTo(600, CAMERA_KEEP, 200, 1);
	fdActorSelectAnimation(actor, PAN_ANIMATION_FULL);
	fdCameraWaitForMove();
	actor->motionFlags  = 0;
	actor->acceleration = toFIXED(2) / 5;
	actor->speedLimit   = toFIXED(4) / 5;
	syFieldActorSetMoveTargetFX(actor, actor->x.fixed, toFIXED(8), actor->z.fixed);
	actor               = fdActorGet(ACTOR_PIPE_EAST);
	actor->motionFlags  = 0;
	actor->acceleration = toFIXED(2) / 5;
	actor->speedLimit   = toFIXED(4) / 5;
	syFieldActorSetMoveTargetFX(actor, actor->x.fixed, toFIXED(32), actor->z.fixed);
	fdActorWaitForMove(actor);
	fdScriptWait(45);
	// The way on, (41, 12), becomes a square to hop to, as (43, 12) is.
	fdMapGridCopyMetatileAttributes(43, 12, 1, 1, 41, 12);
	fdScriptEnd();
}

// Isaac rides one of the Lumber Water Crossing Area's logs dx or dz pixels, from one metatile's
// center to another, as it rolls under him.
static void scColosseumAzartMatchMoveActorPairByOffset(Sint32 actorId, Sint32 dx, Sint32 dz)
{
	ACTOR *leader;
	ACTOR *actor;
	FIXED  x;
	FIXED  z;

	leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	actor  = fdActorGet(actorId);
	fdScriptStart();
	x                    = ((leader->x.fixed + toFIXED(dx)) & FIELD_METATILE_MASK) + toFIXED(8);
	z                    = ((leader->z.fixed + toFIXED(dz)) & FIELD_METATILE_MASK) + toFIXED(8);

	leader->speedLimit   = toFIXED(1);
	leader->acceleration = toFIXED(1) / 2;
	syFieldActorSetMoveTargetFX(leader, x, leader->y.fixed, z);
	fdActorSelectAnimation(leader, ANIM_LEADER_WALK_BACKWARDS);
	x                   = ((actor->x.fixed + toFIXED(dx)) & FIELD_METATILE_MASK) + toFIXED(8);
	z                   = ((actor->z.fixed + toFIXED(dz)) & FIELD_METATILE_MASK) + toFIXED(8);

	actor->speedLimit   = toFIXED(1);
	actor->acceleration = toFIXED(1) / 2;
	syFieldActorSetMoveTargetFX(actor, x, actor->y.fixed, z);
	if (dx < 0 || dz < 0)
	{
		fdActorSelectAnimation(actor, FLOATING_LOG_ANIMATION_ROLL_WEST_OR_NORTH);
	}
	else
	{
		fdActorSelectAnimation(actor, FLOATING_LOG_ANIMATION_ROLL_EAST_OR_SOUTH);
	}
	sySoundPlay(SE_LOG_PUSH);
	fdActorWaitForMove(leader);
	fdActorSelectAnimation(actor, FLOATING_LOG_ANIMATION_SETTLE);
	sySoundPlay(SE_STOP_ACTION_SE);
	fdScriptEnd();
}

// The north-south log carries the leader to the other bank.
static void scColosseumAzartMatchRollLogNorthSouth(void)
{
	ACTOR *actor;
	Sint32 row;
	Sint32 offset;

	actor  = fdActorGet(MainRoutineStateWork.leaderActorId);
	row    = FIELD_METATILE(actor->z.fixed);
	offset = -LOG_NORTH_SOUTH_FLOAT;
	if (row <= LOG_NORTH_ROW)
	{
		offset = LOG_NORTH_SOUTH_FLOAT;
	}
	// Its row goes back to water, three metatiles of the pool beside the log's north line, and its
	// new row is the log's.
	fdMapGridCopyMetatileAttributes(67, 8, 3, 1, 64, row);
	scColosseumAzartMatchMoveActorPairByOffset(ACTOR_LOG_NORTH_SOUTH, 0, offset);
	actor = fdActorGet(ACTOR_LOG_NORTH_SOUTH);
	row   = FIELD_METATILE(actor->z.fixed);
	fdMapGridCopyMetatileAttributes(LOG_NORTH_SOUTH_COLUMN, LOG_NORTH_SOUTH_ROW, 3, 1, 64, row);
}

// The east-west log carries the leader the way the pad is held.
static void scColosseumAzartMatchRollLogEastWest(void)
{
	Sint32 column;
	Sint32 offset;
	Sint32 direction;

	column = FIELD_METATILE(fdActorGet(MainRoutineStateWork.leaderActorId)->x.fixed);
	if ((PerData & KEY_LEFT) != 0)
	{
		direction = -1;
	}
	if ((PerData & KEY_RIGHT) != 0)
	{
		direction = 1;
	}
	offset = FIELD_METATILE(fdActorGet(ACTOR_LOG_NORTH_SOUTH)->z.fixed);
	if (column == 63)
	{
		if (offset == LOG_SOUTH_ROW)
		{
			return;
		}
		offset = LOG_EAST_WEST_FLOAT;
	}
	else if (column == 67)
	{
		if (offset == LOG_SOUTH_ROW && direction == -1)
		{
			return;
		}
		offset = LOG_EAST_WEST_SHORT_FLOAT;
	}
	else
	{
		if (offset == LOG_SOUTH_ROW)
		{
			offset = LOG_EAST_WEST_SHORT_FLOAT;
		}
		else
		{
			offset = LOG_EAST_WEST_FLOAT;
		}
		offset = -offset;
	}
	// Its column goes back to water, three metatiles of the pool, and its new column is the log's.
	fdMapGridCopyMetatileAttributes(72, 9, 1, 3, column, 9);
	scColosseumAzartMatchMoveActorPairByOffset(ACTOR_LOG_EAST_WEST, offset, 0);
	column = FIELD_METATILE(fdActorGet(ACTOR_LOG_EAST_WEST)->x.fixed);
	fdMapGridCopyMetatileAttributes(LOG_EAST_WEST_COLUMN, LOG_EAST_WEST_ROW, 1, 3, column, 9);
}

// Runs the five ticks that show water entering the Pipeworks channel.
static inline void scColosseumAzartMatchAnimatePipeworksWater(void)
{
	fdMapGridCopyMetatiles(WATER_PICTURE_COLUMN, WATER_PICTURE_ROW, 1, 3, 34, 38);
	fdScriptWait(3);
	fdMapGridCopyMetatiles(WATER_PICTURE_COLUMN + 1, WATER_PICTURE_ROW, 1, 3, 34, 38);
	fdScriptWait(3);
	fdMapGridCopyMetatiles(WATER_PICTURE_COLUMN + 2, WATER_PICTURE_ROW, 1, 3, 34, 38);
	fdScriptWait(3);
	fdMapGridCopyMetatiles(WATER_PICTURE_COLUMN + 3, WATER_PICTURE_ROW, 1, 3, 34, 38);
	fdScriptWait(3);
	fdMapGridCopyMetatiles(WATER_PICTURE_COLUMN + 4, WATER_PICTURE_ROW, 1, 3, 34, 38);
}

// Walking west into the loose length of pipe in the Pipeworks pushes it three metatiles west, under
// the spout, and the camera follows it. Water then runs down into the channel, and if the pans have
// not tipped already, the west pan fills and sinks and the east one rises, opening the way on
// (metatile (41, 12)).
static void scColosseumAzartMatchPushPipeworksPipe(void)
{
	Sint32 leaderId;
	ACTOR *leader;
	ACTOR *actor;
	Sint32 none;

	leaderId = MainRoutineStateWork.leaderActorId;
	leader   = fdActorGet(leaderId);
	actor    = fdActorGet(ACTOR_PIPEWORKS_PIPE);
	gmFlagSet(FLAG_PIPEWORKS_PIPE_PUSHED);
	fdScriptStart();
	fdActorSetAnimation(leaderId, ANIM_PUSH);
	fdScriptWait(6);
	actor->speedLimit   = toFIXED(1) / 2;
	actor->acceleration = toFIXED(1) / 5;
	sySoundPlay(SE_HEAVY_MOVE);
	fdActorSelectAnimation(actor, PIPE_ANIMATION_ROLLING);
	syFieldActorSetMoveTargetFX(actor, actor->x.fixed - toFIXED(48), 0, actor->z.fixed);
	fdScriptWait(6);
	fdActorSetAnimation(leaderId, ANIM_WALK);
	fdActorStartCamera(
		((FIELD_STATE *)syHeapSlotAllocateEWRAM(HEAP_SLOT_FIELD, sizeof(FIELD_STATE)))->camera,
		(Sint32)actor);
	fdActorSetSpeedAndAcceleration(leaderId, 0.3, 0.2);
	syFieldActorSetMoveTargetFX(leader, leader->x.fixed - toFIXED(24), 0, leader->z.fixed);
	fdActorWaitForMoveById(leaderId);
	fdActorSetAnimation(leaderId, ANIM_STAND);
	fdActorWaitForMove(actor);
	fdActorSelectAnimation(actor, PIPE_ANIMATION_STILL);
	sySoundPlay(SE_STOP_ACTION_SE);
	sySoundPlay(SE_HEAVY_STOP);
	fdScriptWait(15);
	fdScriptEnd();
	// The pipe's column of wall, (37, 7..10), moves to where it now lies, and the floor beside it,
	// column 36, takes its place.
	fdMapGridCopyMetatileAttributes(37, 7, 1, 4, 34, 7);
	fdMapGridCopyMetatileAttributes(36, 7, 1, 4, 37, 7);
	none = fdScriptGetFlag(FLAG_PIPES_MOVED);
	if (none != 0)
	{
		fdScriptStart();
		fdCameraSetSpeed(2);
		fdCameraMoveTo(552, CAMERA_KEEP, 200, 1);
		fdCameraWaitForMove();
		scColosseumAzartMatchAnimatePipeworksWater();
		fdScriptWait(15);
		fdScriptEnd();
		return;
	}
	gmFlagSet(FLAG_PIPES_MOVED);
	fdScriptStart();
	fdCameraSetSpeed(2);
	fdCameraMoveTo(600, CAMERA_KEEP, 200, 1);
	fdCameraWaitForMove();
	actor               = fdActorGet(ACTOR_PIPE_WEST);
	actor->motionFlags  = none;
	actor->acceleration = toFIXED(2) / 5;
	actor->speedLimit   = toFIXED(4) / 5;
	syFieldActorSetMoveTargetFX(actor, actor->x.fixed, toFIXED(8), actor->z.fixed);
	fdActorSelectAnimation(actor, PAN_ANIMATION_FULL);
	scColosseumAzartMatchAnimatePipeworksWater();
	actor               = fdActorGet(ACTOR_PIPE_EAST);
	actor->motionFlags  = none;
	actor->acceleration = toFIXED(2) / 5;
	actor->speedLimit   = toFIXED(4) / 5;
	syFieldActorSetMoveTargetFX(actor, actor->x.fixed, toFIXED(32), actor->z.fixed);
	fdActorWaitForMove(actor);
	fdScriptWait(15);
	fdScriptEnd();
	fdMapGridCopyMetatileAttributes(43, 12, 1, 1, 41, 12);
}

// Unused: sets FLAG_AREA_TEMP_00. Nothing calls it.
UNUSED static void scColosseumAzartMatchSetCourseFlag(void)
{
	gmFlagSet(FLAG_AREA_TEMP_00);
}

// Reaching Colosseum Final 1's arena, the leader takes a prize and fights Azart.
static void scColosseumAzartMatchReachArena(void)
{
	Sint32 opponentFirst;
	Sint32 i;
	Sint32 opponentFirstSlot = 0;
	scColossoNoOperation();
	fdScriptStart();
	opponentFirst = scColossoTakePrize(ITEM_IRON_SHIELD, ITEM_PADDED_GLOVES);
	scColossoWaitForOpponentToFinish();
	// Up to ten waits of 300 ticks while the opponent holds up his prize.
	for (i = 0; i < 10; i++)
	{
		fdActorWaitForScriptEndById(ACTOR_OPPONENT);
	}
	fdActorSetSpeed(ACTOR_OPPONENT, 1);
	fdActorWalkTo(ACTOR_OPPONENT, 1320, 192);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 1);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 1288, 192);
	fdActorSetAnimation(ACTOR_OPPONENT, ANIM_STAND);
	fdActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_OPPONENT, 0);
	fdScriptWait(10);
	fdActorSetAnimation(ACTOR_OPPONENT, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdScriptWait(20);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 2);
	fdActorSetSpeed(ACTOR_OPPONENT, 2);
	fdActorWalkTo(ACTOR_PARTY_LEADER, 1296, 192);
	fdActorWalkToAndWait(ACTOR_OPPONENT, 1312, 192);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_STRIDE);
	fdActorSetAnimation(ACTOR_OPPONENT, WARRIOR_ANIMATION_FIGHTING_STANCE);
	fdScriptWait(10);
	fdRequestBattleFromEncounterTable(ENCOUNTER_ZONE_COLOSSO,
	                                  opponentFirstSlot - opponentFirst + 1);
	SET_BATTLE_TYPE(BATTLE_TYPE_SCRIPTED);
	fdScriptSetAfterBattleDestination(SCENE_COLOSSEUM_AZART_MATCH,
	                                  COLOSSEUM_AZART_MATCH_AFTER_MATCH_WON);
	fdScriptSetDefeatDestination(SCENE_COLOSSEUM_AZART_MATCH,
	                             COLOSSEUM_AZART_MATCH_AFTER_MATCH_LOST);
	gmFlagSet(FLAG_KEEP_MUSIC_ONCE);
}

// Walking on from the east edge of the Rock Challenge's raised square (metatile (22, 12)) pushes
// the stone pillar ahead if there is one. With no pillar there, Isaac hops forward across the gap
// where there is somewhere to land, and otherwise nothing happens.
static void scColosseumAzartMatchPushRockOrHopAcross(void)
{
	if (scColossoFindActorAhead() == 0)
	{
		fdTryJumpForward();
	}
	else
	{
		scColosseumAzartMatchPushRock();
	}
}

const CUE *scGetCues(void)
{
	return ColosseumAzartMatchCues;
}

// Back from the battle, Isaac and Azart stand in the arena as the result is shown.
static void scColosseumAzartMatchRestoreMatchResult(Sint32 result)
{
	fdActorReleaseById(ACTOR_PRIZE_A);
	fdActorReleaseById(ACTOR_PRIZE_B);
	gmPartyRestoreHpPp(1);
	fdScriptStart();
	fdActorSetPos(ACTOR_OPPONENT, 1320, 192);
	fdActorSetPos(ACTOR_PARTY_LEADER, 1288, 192);
	// BUG: FACING_SOUTH is passed where fdActorFaceActor takes an actor id; fdActorLookup has no
	// actor past 191, so neither turns.
#if FIX_FACING_PASSED_AS_ACTOR_ID
	fdActorFaceDirection(ACTOR_OPPONENT, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
#else
	fdActorFaceActor(ACTOR_OPPONENT, FACING_SOUTH, 0);
	fdActorFaceActor(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
#endif
	if (result < 0)
	{
		fdActorSetAnimation(ACTOR_OPPONENT, WARRIOR_ANIMATION_ARMS_RAISED);
		fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_CROUCH);
	}
	else
	{
		fdActorSetAnimation(ACTOR_OPPONENT, WARRIOR_ANIMATION_DEFEATED);
		fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_ARMS_RAISED);
	}
	sySynch(1);
	fdCameraMoveTo(1304, 0, 128, 0);
	scColossoShowMatchResult(result);
	fdScriptEnd();
}

// A friend whose cheering spot is the vine's metatile before the Broken Bridge, half metatiles
// (94..95, 24..26), is put on layer 1 for the race, the layer the plant itself stands on.
static void scColosseumAzartMatchSetLayerOnVineMetatile(Sint32 actorId)
{
	ACTOR *target;
	Sint32 column;
	Sint32 row;

	target = fdActorGet(actorId);
	if (target != NULL)
	{
		column = FIELD_HALF_METATILE(target->x.fixed);
		row    = FIELD_HALF_METATILE(target->z.fixed);
		if (column >= 94 && column <= 95 && row > 23 && row <= 26)
		{
			target->layer = 1;
		}
	}
}

// Sets up Colosseum Final 1 for the visit its entrance stands for.
Sint32 scInit(void)
{
	ACTOR *actor;
	FIXED  hit;
	Sint32 x;
	Sint32 color;

	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_FADE, 0);
	gmFlagSet(FLAG_RETREAT_BLOCKED);
	actor = fdActorGet(ACTOR_ROCK_A);
	hit   = fdMapGridGetTerrainHeight(0, actor->x.fixed, actor->z.fixed);
	if (actor->y.fixed == 0 && hit == 0)
	{
		actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
		actor->motionFlags   = hit;
		fdMapGridCopyMetatileAttributes(HOP_SQUARE_COLUMN,
		                                HOP_SQUARE_ROW,
		                                1,
		                                1,
		                                FIELD_METATILE(actor->x.fixed),
		                                FIELD_METATILE(actor->z.fixed));
	}
	x = syFieldGameFlagGetByte(FLAG_SECOND_ROCK_COLUMN);
	if (x == 0)
	{
		x = 25;
	}
	actor                = fdActorGet(ACTOR_ROCK_B);
	actor->x.fixed       = FIELD_METATILE_CENTER(x);
	actor->motionFlags   = 0;
	actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
	fdMapGridCopyMetatileAttributes(HOP_SQUARE_COLUMN, HOP_SQUARE_ROW, 1, 1, x, 12);
	sySetTickEvent(scColosseumAzartMatchUpdateBrokenBridge, EVENT_PASS_TICK);
	actor        = fdActorGet(ACTOR_GROWTH_PLANT);
	actor->layer = 1;
	if (gmFlagGet(FLAG_PLANT_GROWN))
	{
		fdActorSelectAnimation(actor, SPROUT_ANIMATION_GROWN);
		fdActorSetSpriteFlags(actor, 0);
		actor->collisionFlags = 0;
		actor->priorityFlags  = ACTOR_PRIORITY_AUTOMATIC | ACTOR_PRIORITY_UNDERFOOT;
		fdMapGridCopyMetatileAttributes(VINE_COLUMN, VINE_ROW, 1, 1, 47, 12);
	}
	actor                = fdActorGet(ACTOR_LOG_NORTH_SOUTH);
	color                = FIELD_METATILE(actor->z.fixed);
	actor->motionFlags   = 0;
	actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
	fdMapGridCopyMetatileAttributes(LOG_NORTH_SOUTH_COLUMN, LOG_NORTH_SOUTH_ROW, 3, 1, 64, color);
	actor                = fdActorGet(ACTOR_LOG_EAST_WEST);
	color                = FIELD_METATILE(actor->x.fixed);
	actor->motionFlags   = 0;
	actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
	fdMapGridCopyMetatileAttributes(LOG_EAST_WEST_COLUMN, LOG_EAST_WEST_ROW, 1, 3, color, 9);
	// With the Pipeworks' loose pipe pushed under the spout, water pours down into the channel.
	if (gmFlagGet(FLAG_PIPEWORKS_PIPE_PUSHED))
	{
		fdMapGridCopyMetatileAttributes(37, 7, 1, 4, 34, 7);
		fdMapGridCopyMetatileAttributes(36, 7, 1, 4, 37, 7);
		// the water's last step
		fdMapGridCopyMetatiles(WATER_PICTURE_COLUMN + 4, WATER_PICTURE_ROW, 1, 3, 34, 38);
	}
	actor = fdActorGet(ACTOR_PIPE_WEST);
	if (gmFlagGet(FLAG_PIPES_MOVED))
	{
		fdMapGridCopyMetatileAttributes(43, 12, 1, 1, 41, 12);
		actor->motionFlags  = 0;
		actor->acceleration = toFIXED(2) / 5;
		actor->speedLimit   = toFIXED(4) / 5;
		actor->y.fixed      = toFIXED(8);
		fdActorSelectAnimation(actor, PAN_ANIMATION_FULL);
	}
	else
	{
		fdActorSelectAnimation(actor, PAN_ANIMATION_EMPTY);
	}
	fdActorGet(ACTOR_PIPE_EAST)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
	scColossoShowItemIcon(ACTOR_PRIZE_A, ITEM_IRON_SHIELD);
	scColossoShowItemIcon(ACTOR_PRIZE_B, ITEM_PADDED_GLOVES);
	switch (MainRoutineStateWork.entrance)
	{
		case COLOSSEUM_AZART_MATCH_RACE_FROM_LEND_A_HAND:
			scColossoStartRaceGauge(ACTOR_PARTY_LEADER,
			                        ACTOR_OPPONENT,
			                        4,
			                        toFIXED(1304),
			                        toFIXED(192),
			                        ACTOR_PRIZE_A,
			                        ACTOR_PRIZE_B);
			// The doorway Isaac sets out from, (19, 2..3), takes the two metatiles kept at (127,
			// 0..1) for the race.
			fdMapGridCopyMetatilesLayered(127, 0, 1, 2, 19, 2);
			fdActorReleaseById(ACTOR_GUIDE);
			fdActorReleaseById(ACTOR_ROCK_ATTENDANT);
			fdActorReleaseById(ACTOR_PIPEWORKS_ATTENDANT);
			fdActorReleaseById(ACTOR_BRIDGE_ATTENDANT);
			fdActorReleaseById(ACTOR_LUMBER_ATTENDANT);
			if (!gmFlagGet(FLAG_KEEP_PARTY_POSITION))
			{
				sySoundPlay(SOUND_FADE_OUT_BGM);
				scColossoControlCharacterAlone(ISAAC);
				scColossoPlaceFriendsAtCheeringSpots();
				scColosseumAzartMatchSetLayerOnVineMetatile(ACTOR_GARET);
				scColosseumAzartMatchSetLayerOnVineMetatile(ACTOR_IVAN);
				scColosseumAzartMatchSetLayerOnVineMetatile(ACTOR_MIA);
				scColossoShowOpeningLettering(FINALS_MATCH);
			}
			fdActorWatchAndSetScript(ACTOR_GARET, ACTOR_PARTY_LEADER);
			fdActorWatchAndSetScript(ACTOR_IVAN, ACTOR_PARTY_LEADER);
			fdActorWatchAndSetScript(ACTOR_MIA, ACTOR_PARTY_LEADER);
			scColossoStartOpponent(RESOURCE_AZART_OPPONENT_PATH);
			break;
		case COLOSSEUM_AZART_MATCH_FROM_COLOSSEUM_WEST:
			sySetTickEvent(scColossoRaiseTakePlaceOnPsynergy, EVENT_PASS_TICK);
			fdActorReleaseById(ACTOR_PRIZE_A);
			fdActorReleaseById(ACTOR_PRIZE_B);
			if (!gmFlagGet(FLAG_KEEP_PARTY_POSITION))
			{
				scColossoPlaceFriendsAtCheeringSpots();
				scColossoControlCharacterAlone(GARET);
				scColossoShowOpeningLettering(LETTERING_LEND_A_HAND);
			}
			break;
		case COLOSSEUM_AZART_MATCH_FROM_COLOSSEUM:
			if (!gmFlagGet(FLAG_KEEP_PARTY_POSITION))
			{
				scColosseumAzartMatchExplainFinalsRules(ACTOR_GUIDE);
				scColossoClearCheeringSpots();
			}
			break;
		case COLOSSEUM_AZART_MATCH_AFTER_MATCH_WON:
			scColosseumAzartMatchRestoreMatchResult(1);
			syImportScriptSetExitRequest(COLOSSEUM_AZART_MATCH_TO_COLOSSEUM_EAST);
			break;
		case COLOSSEUM_AZART_MATCH_AFTER_MATCH_LOST:
			scColosseumAzartMatchRestoreMatchResult(-1);
			syImportScriptSetExitRequest(COLOSSEUM_AZART_MATCH_TO_BABI_PALACE);
			break;
	}
	return 0;
}

// The guide gives the party the tour of Colosseum Final 1 (COLOSSEUM_AZART_MATCH_FROM_COLOSSEUM).
static void scColosseumAzartMatchExplainFinalsRules(Sint32 guide)
{
	ACTOR *actor;
	Sint32 x;
	Sint32 z;

	actor = fdActorGet(guide);
	x     = actor->x.part[0].integer;
	z     = actor->z.part[0].integer;
	fdScriptStart();
	fdActorSetSpeed(guide, 1);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 1);
	fdActorSetSpeed(ACTOR_GARET, 1);
	fdActorSetSpeed(ACTOR_IVAN, 1);
	fdActorSetSpeed(ACTOR_MIA, 1);
	fdActorSetPosFX(ACTOR_PARTY_LEADER, toFIXED(x), toFIXED(z) - toFIXED(48));
	fdActorSetPosFX(ACTOR_GARET, toFIXED(x) - toFIXED(16), toFIXED(z) - toFIXED(40));
	fdActorSetPosFX(ACTOR_IVAN, toFIXED(x) + toFIXED(16), toFIXED(z) - toFIXED(40));
	fdActorSetPosFX(ACTOR_MIA, toFIXED(x), toFIXED(z) - toFIXED(32));
	fdActorSetPosFX(guide, toFIXED(x), toFIXED(z) - toFIXED(80));
	actor         = fdActorGet(ACTOR_PARTY_LEADER);
	actor->facing = FACING_NORTH;
	fdCameraFollowActor(ACTOR_PARTY_LEADER, 0);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptSetMessageId(MsgColossoSiteFirstFinalsBattle);
	// "This is the site of the first finals battle."
	fdScriptShowMessage(guide, 0);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	// "So this is where warriors test their mettle against each other."
	fdScriptShowMessage(ACTOR_MIA, 0);
	fdActorStartRepeatedMotion(guide, 3);
	// "Well, no... It's actually more than that."
	fdScriptShowMessage(guide, 0);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	// "What else is involved?"
	fdScriptShowMessage(ACTOR_IVAN, 0);
	fdActorFaceActor(guide, ACTOR_IVAN, 0);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(guide, ANIM_NOD);
	// "The warriors are racing to get here first and get the best..."
	fdScriptShowMessage(guide, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	// "So there's an advantage to getting here first? That's quite..."
	fdScriptShowMessage(ACTOR_GARET, 0);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	// "That's too hard for Isaac. We must support him as well we can..."
	fdScriptShowMessage(ACTOR_MIA, 0);
	fdActorShowEmote(guide, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
	// Pan over each stage while the guide explains the finals rules. "Um, don't you want to hear my
	// explanation?" Yes: "All right. Then let me explain in detail..." No: "Ask the attendants for
	// explanations of the stages."
	if (fdScriptAskYesNo(guide, 0) == 0)
	{
		do
		{
			fdScriptSetMessageId(MsgColossoWarriorsEnterFinalsWithoutAny);
			fdActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
			fdScriptWait(2);
			fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
			fdScriptWait(2);
			fdActorSetAnimation(ACTOR_MIA, ANIM_NOD);
			fdScriptWait(1);
			fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
			fdActorSetAnimationAndWait(guide, ANIM_NOD);
			// "Warriors enter the finals without any of their own equipment."
			fdScriptShowMessage(guide, 0);
			fdActorFaceDirection(guide, FACING_NORTHWEST, 0);
			fdScriptWait(20);
			// "Please look over there."
			fdScriptShowMessage(guide, 0);
			fdCameraSetSpeed(3);
			fdCameraMoveTo(312, CAMERA_KEEP, 104, 1);
			fdCameraWaitForMove();
			// "That is the starting point of the first stage of the finals."
			fdScriptShowMessage(guide, 0);
			fdCameraSetSpeed(1.5);
			fdCameraMoveTo(776, CAMERA_KEEP, 104, 1);
			// "You must get through several stages before you reach the arena."
			fdScriptShowMessage(guide, 0);
			fdCameraWaitForMove();
			// "The chests along the way contain items that may be useful in..."
			fdScriptShowMessage(guide, 0);
			fdCameraSetSpeed(3);
			fdCameraMoveTo(1240, CAMERA_KEEP, 168, 1);
			fdCameraWaitForMove();
			// BUG: FACING_SOUTHWEST is passed where an actor id belongs; no actor has it, so the
			// guide does not turn.
			fdActorFaceActor(guide, FACING_SOUTHWEST, 0);
			// "This is the battle arena. After getting here, the warriors..."
			fdScriptShowMessage(guide, 0);
			fdCameraMoveTo(1304, CAMERA_KEEP, 168, 1);
			fdCameraWaitForMove();
			fdActorFaceActor(guide, ACTOR_PARTY_LEADER, 0);
			// "You can get weapons in the arena, too, but one is weaker..."
			fdScriptShowMessage(guide, 0);
			// "So you'll have the advantage if you get here first."
			fdScriptShowMessage(guide, 0);
			// "Also, you can use the equipment you get here in your next match."
			fdScriptShowMessage(guide, 0);
			fdCameraFollowActor(ACTOR_PARTY_LEADER, 0);
			fdActorRunRepeatedMotion(guide, 2);
		} while (fdScriptAskYesNo(guide, 0) != 0);
		fdActorRunRepeatedMotion(guide, 2);
		fdScriptSetMessageId(MsgColossoAskAttendantsForExplanationsStages);
		// "Ask the attendants for explanations of the stages."
		fdScriptShowMessage(guide, 0);
	}
	fdScriptSetMessageId(MsgColossoIsaacYoureContestantInFinals);
	fdActorRunRepeatedMotion(guide, 2);
	// "...Isaac, you're a contestant in the finals. Do not forget..."
	fdScriptShowMessage(guide, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_WEST, 0);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	// "Hey, where do we cheer from?"
	fdScriptShowMessage(ACTOR_GARET, 0);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
	// "Would you like to visit the stages and hear a description..."
	fdScriptShowMessage(ACTOR_IVAN, 0);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
	// "Yes. We'll decide where to cheer for Isaac after hearing the..."
	fdScriptShowMessage(ACTOR_MIA, 0);
	fdActorSetAnimation(ACTOR_MIA, ANIM_NOD);
	fdScriptWait(1);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdScriptWait(2);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
	fdScriptWait(1);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdScriptWait(6);
	fdActorSetAnimation(ACTOR_GARET, ANIM_WALK);
	actor = fdActorGet(ACTOR_PARTY_LEADER);
	if (actor != NULL)
	{
		fdActorSetDestination(ACTOR_GARET, actor->x.part[0].integer, actor->z.part[0].integer);
	}
	fdActorSetAnimation(ACTOR_IVAN, ANIM_WALK);
	actor = fdActorGet(ACTOR_PARTY_LEADER);
	if (actor != NULL)
	{
		fdActorSetDestination(ACTOR_IVAN, actor->x.part[0].integer, actor->z.part[0].integer);
	}
	fdActorSetAnimation(ACTOR_MIA, ANIM_WALK);
	actor = fdActorGet(ACTOR_PARTY_LEADER);
	if (actor != NULL)
	{
		fdActorSetDestination(ACTOR_MIA, actor->x.part[0].integer, actor->z.part[0].integer);
	}
	fdActorWalkToAndWait(guide, x - 16, z - 64);
	fdActorSetPos(ACTOR_GARET, 0, 0);
	fdActorSetPos(ACTOR_IVAN, 0, 0);
	fdActorSetPos(ACTOR_MIA, 0, 0);
	fdActorWalkToAndWait(guide, x - 16, z - 16);
	fdActorWalkToAndWait(guide, x, z);
	fdActorFaceDirection(guide, FACING_NORTH, 10);
	fdScriptEnd();
}

// The Rock Challenge's attendant explains stage 1 of Colosseum Final 1.
static void scColosseumAzartMatchTalkToRockAttendant(Sint32 actorId)
{
	Sint32 path;

	if (MainRoutineStateWork.entrance == COLOSSEUM_AZART_MATCH_FROM_COLOSSEUM_WEST)
	{
		scColossoAskDoneCheering(actorId);
	}
	else
	{
		fdScriptStart();
		path = scColossoGreetAtStage(actorId, STAGE_ROCK_CHALLENGE);
		if (path == COLOSSO_GUIDE_DESCRIBE)
		{
			fdScriptSetMessageId(MsgColossoCallRockChallenge);
			fdCameraSetSpeed(3);
			fdCameraMoveTo(328, CAMERA_KEEP, 168, 1);
			fdCameraWaitForMove();
			// "They call this the Rock Challenge."
			fdScriptShowMessage(actorId, 0);
			scColossoFadeInWarrior(ACTOR_PARTY_LEADER, 280, 200);
			fdActorSetSpeed(ACTOR_PARTY_LEADER, 1);
			fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 360, 200);
			fdScriptWait(30);
			fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
			// "Warriors must create steps they can use to get through here."
			fdScriptShowMessage(actorId, 0);
			fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 312, 200);
			fdScriptWait(30);
			fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 10);
			fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 60);
			fdActorSetSpeed(ACTOR_PARTY_LEADER, 1.5);
			scColossoWalkActorToAndStand(ACTOR_PARTY_LEADER, 296, 184);
			scColossoWalkActorToAndStand(ACTOR_PARTY_LEADER, 296, 152);
			scColossoWalkActorToAndStand(ACTOR_PARTY_LEADER, 312, 152);
			fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 15);
			scColossoPushActorAhead();
			scActorResetMotionFields(ACTOR_PARTY_LEADER);
			scColossoPushActorAhead();
			scActorResetMotionFields(ACTOR_PARTY_LEADER);
			fdActorSetSpeed(ACTOR_PARTY_LEADER, 1.5);
			fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 304, 184);
			fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 296, 192);
			fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 296, 200);
			fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 15);
			scColossoPushActorAhead();
			scActorResetMotionFields(ACTOR_PARTY_LEADER);
			scColossoPushActorAhead();
			scActorResetMotionFields(ACTOR_PARTY_LEADER);
			fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
			// "Warriors must use only the objects in this stage, along with..."
			fdScriptShowMessage(actorId, 0);
			scColossoRestoreWarrior(ACTOR_PARTY_LEADER);
			fdCameraFollowActor(ACTOR_PARTY_LEADER, 0);
			fdActorSetPos(ACTOR_ROCK_A, 312, 168);
			scColossoRepeatStageNumber(actorId, STAGE_ROCK_CHALLENGE);
		}
		else if (path == COLOSSO_GUIDE_OFFER_CHEERING)
		{
			fdScriptSetMessageId(MsgColossoObjectiveStageClear);
			// "The objective in this stage is to clear a path as quickly as..."
			fdScriptShowMessage(actorId, 0);
		}
		scColossoOfferCheeringSpot(path, actorId, STAGE_ROCK_CHALLENGE);
		fdScriptEnd();
	}
}

// Moves a Pipeworks actor with the speed used throughout its demonstration.
static inline void scColosseumAzartMatchSetPipeHeight(ACTOR *actor, FIXED height)
{
	actor->motionFlags  = 0;
	actor->acceleration = toFIXED(2) / 5;
	actor->speedLimit   = toFIXED(4) / 5;
	syFieldActorSetMoveTargetFX(actor, actor->x.fixed, height, actor->z.fixed);
}

// The Pipeworks' attendant explains stage 2 of Colosseum Final 1.
static void scColosseumAzartMatchTalkToPipeworksAttendant(Sint32 attendant)
{
	Sint32 path;
	ACTOR *second;

	if (MainRoutineStateWork.entrance == COLOSSEUM_AZART_MATCH_FROM_COLOSSEUM_WEST)
	{
		scColossoAskDoneCheering(attendant);
	}
	else
	{
		fdScriptStart();
		path = scColossoGreetAtStage(attendant, STAGE_PIPEWORKS);
		if (path == COLOSSO_GUIDE_DESCRIBE)
		{
			fdScriptSetMessageId(MsgColossoAreaCalledPipeworks);
			fdCameraSetSpeed(3);
			fdCameraMoveTo(592, CAMERA_KEEP, 120, 1);
			fdCameraWaitForMove();
			fdScriptWait(60);
			fdCameraSetSpeed(1.5);
			fdCameraMoveTo(608, CAMERA_KEEP, 216, 1);
			fdCameraWaitForMove();
			// "This area is called the Pipeworks."
			fdScriptShowMessage(attendant, 0);
			scColossoShowPointer(56, 64, 0);
			fdScriptWait(60);
			scColossoMovePointer(160, 96, 10);
			fdScriptWait(70);
			// "See the water pipes? They actually form a kind of scale."
			fdScriptShowMessage(attendant, 0);
			scColossoHidePointer();
			sySynch(2);
			scColosseumAzartMatchSetPipeHeight(fdActorGet(ACTOR_PIPE_WEST), toFIXED(8));
			second = fdActorGet(ACTOR_PIPE_EAST);
			scColosseumAzartMatchSetPipeHeight(second, toFIXED(32));
			fdActorWaitForMove(second);
			fdScriptWait(45);
			scColosseumAzartMatchSetPipeHeight(fdActorGet(ACTOR_PIPE_WEST), toFIXED(24));
			second = fdActorGet(ACTOR_PIPE_EAST);
			scColosseumAzartMatchSetPipeHeight(second, 0);
			fdActorWaitForMove(second);
			fdScriptWait(15);
			// "When one side lowers, the other side rises."
			fdScriptShowMessage(attendant, 0);
			scColossoShowPointer(56, 64, 0);
			fdScriptWait(30);
			scColossoMovePointer(160, 96, 10);
			fdScriptWait(40);
			scColossoMovePointer(56, 64, 10);
			fdScriptWait(70);
			// "Using this premise, you must find a way to reach the next stage."
			fdScriptShowMessage(attendant, 0);
			scColossoHidePointer();
			sySynch(2);
			fdCameraFollowActor(ACTOR_PARTY_LEADER, 0);
			scColossoRepeatStageNumber(attendant, STAGE_PIPEWORKS);
		}
		else if (path == COLOSSO_GUIDE_OFFER_CHEERING)
		{
			fdScriptSetMessageId(MsgColossoObjectiveMakeGood);
			// "The objective here is to make good use of the scales."
			fdScriptShowMessage(attendant, 0);
		}
		scColossoOfferCheeringSpot(path, attendant, STAGE_PIPEWORKS);
		fdScriptEnd();
	}
}

// The Broken Bridge's attendant explains stage 3 of Colosseum Final 1.
static void scColosseumAzartMatchTalkToBridgeAttendant(Sint32 actorId)
{
	Sint32 path;

	if (MainRoutineStateWork.entrance == COLOSSEUM_AZART_MATCH_FROM_COLOSSEUM_WEST)
	{
		scColossoAskDoneCheering(actorId);
	}
	else
	{
		fdScriptStart();
		path = scColossoGreetAtStage(actorId, STAGE_BROKEN_BRIDGE);
		if (path == COLOSSO_GUIDE_DESCRIBE)
		{
			fdScriptSetMessageId(MsgColossoTheyCallBrokenBridge);
			scColosseumAzartMatchResetBrokenBridge();
			fdCameraSetSpeed(3);
			fdCameraMoveTo(840, CAMERA_KEEP, 216, 1);
			fdCameraWaitForMove();
			// "They call this the Broken Bridge."
			fdScriptShowMessage(actorId, 0);
			scColosseumAzartMatchStartBrokenBridge();
			fdScriptWait(60);
			// "Carefully time your bridge crossings to reach the next stage."
			fdScriptShowMessage(actorId, 0);
			scColossoFadeInWarrior(ACTOR_PARTY_LEADER, 736, 200);
			COLOSSEUM_AZART_MATCH_FACE_DIRECTION(ACTOR_PARTY_LEADER, 0, 0);
			scColosseumAzartMatchWaitForBridgeOpening();
			fdActorSetSpeed(ACTOR_PARTY_LEADER, 1);
			fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 816, 200);
			fdScriptWait(30);
			fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
			// "If your timing is off, you'll fall down, which will cost you time."
			fdScriptShowMessage(actorId, 0);
			scColossoRestoreWarrior(ACTOR_PARTY_LEADER);
			fdCameraFollowActor(ACTOR_PARTY_LEADER, 0);
			scColossoRepeatStageNumber(actorId, STAGE_BROKEN_BRIDGE);
		}
		else if (path == COLOSSO_GUIDE_OFFER_CHEERING)
		{
			fdScriptSetMessageId(MsgColossoOperatorBridgeWillAlsoCheer);
			// "The operator of the bridge will also cheer for you in this..."
			fdScriptShowMessage(actorId, 0);
		}
		scColossoOfferCheeringSpot(path, actorId, STAGE_BROKEN_BRIDGE);
		fdScriptEnd();
	}
}

// The Lumber Water Crossing Area's attendant explains stage 4 of Colosseum Final 1.
static void scColosseumAzartMatchTalkToLumberAttendant(Sint32 actorId)
{
	Sint32 path;

	if (MainRoutineStateWork.entrance == COLOSSEUM_AZART_MATCH_FROM_COLOSSEUM_WEST)
	{
		scColossoAskDoneCheering(actorId);
		return;
	}
	fdScriptStart();
	path = scColossoGreetAtStage(actorId, STAGE_LUMBER_WATER_CROSSING);
	if (path == COLOSSO_GUIDE_DESCRIBE)
	{
		fdScriptSetMessageId(MsgColossoPlaceNormallyCalledLumberWater);
		fdCameraSetSpeed(3);
		fdCameraMoveTo(1088, CAMERA_KEEP, 168, 1);
		fdCameraWaitForMove();
		// "This place is normally called the Lumber Water Crossing Area."
		fdScriptShowMessage(actorId, 0);
		scColossoShowPointer(120, 72, 0);
		// "The obstacle at this stage is that large water tank."
		fdScriptShowMessage(actorId, 0);
		scColossoHidePointer();
		fdScriptWait(15);
		scColossoFadeInWarrior(ACTOR_PARTY_LEADER, 984, 200);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 10);
		// "If you can get all the way across in one go, you don't have to..."
		fdScriptShowMessage(actorId, 0);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 30);
		fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 60);
		fdActorSetSpeed(ACTOR_PARTY_LEADER, 1.5);
		scColossoWalkActorToAndStand(ACTOR_PARTY_LEADER, 1000, 192);
		scColossoWalkActorToAndStand(ACTOR_PARTY_LEADER, 1000, 176);
		scColossoWalkActorToAndStand(ACTOR_PARTY_LEADER, 1016, 168);
		fdScriptWait(15);
		COLOSSEUM_AZART_MATCH_PLACE_STAGED_PAIR(ACTOR_LOG_EAST_WEST, LOG_EAST_WEST_FLOAT, 0);
		fdCameraMoveTo(1088, CAMERA_KEEP, 168, 1);
		fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
		fdScriptWait(10);
		fdActorSetSpeed(ACTOR_PARTY_LEADER, 1);
		fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 1192, 168);
		fdScriptWait(10);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 30);
		fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
		// "But if you want to reach the treasure box, it'll take a little..."
		fdScriptShowMessage(actorId, 0);
		scColossoRestoreWarrior(ACTOR_PARTY_LEADER);
		fdCameraFollowActor(ACTOR_PARTY_LEADER, 0);
		fdActorSetPos(ACTOR_LOG_EAST_WEST, 1016, 168);
		scColossoRepeatStageNumber(actorId, STAGE_LUMBER_WATER_CROSSING);
	}
	else if (path == COLOSSO_GUIDE_OFFER_CHEERING)
	{
		fdScriptSetMessageId(MsgColossoLogsKeyClearingStage);
		// "The logs are the key to clearing this stage."
		fdScriptShowMessage(actorId, 0);
	}
	scColossoOfferCheeringSpot(path, actorId, STAGE_LUMBER_WATER_CROSSING);
	fdScriptEnd();
}

#include "shared/colosso_solo.c"
#include "shared/item_equip.c"
#include "shared/colosso_task.c"

#include "shared/colosso_lettering.c"

#include "shared/colosso_guide.c"
#include "shared/colosso_item_icon.c"

#include "shared/colosso_hex_string.c"
#include "shared/colosso_finish.c"

#include "shared/colosso_path_opponent.c"
#include "shared/colosso_setup.c"
#include "shared/colosso_map_state.c"
#include "shared/actor_find_at_position.c"

#include "shared/colosso_push.c"

#include "shared/colosso_lettering_tables.c"
// clang-format off

char HexDigits[20] = "0123456789ABCDEF";

#include "shared/colosso_prize_scripts.c"

#include "shared/pushed_actor_direction_steps.c"

SCENE_ENTRANCE ColosseumAzartMatchEntrances[] = {
	{ ENTRANCE_DEFAULT, CONDITION_ALWAYS, 920, 0, 200, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ COLOSSEUM_AZART_MATCH_RACE_FROM_LEND_A_HAND, CONDITION_ALWAYS, 312, 0, 104, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

Uint32 ColosseumAzartMatchExits[] = {
	SCENE_EXITS(ColosseumAzartMatch),
	SCENE_EXIT(COLOSSEUM_AZART_MATCH_LEND_A_HAND_TO_RACE, ColosseumAzartMatch, COLOSSEUM_AZART_MATCH_RACE_FROM_LEND_A_HAND),
	SCENE_EXIT(COLOSSEUM_AZART_MATCH_TO_COLOSSEUM_WEST, Colosseum, COLOSSEUM_FROM_COLOSSEUM_AZART_MATCH_WEST),
	SCENE_EXIT(COLOSSEUM_AZART_MATCH_TO_COLOSSEUM_EAST, Colosseum, COLOSSEUM_FROM_COLOSSEUM_AZART_MATCH_EAST),
	SCENE_EXIT(COLOSSEUM_AZART_MATCH_TO_BABI_PALACE, BabiPalace, BABI_PALACE_FROM_COLOSSEUM_AZART_MATCH),
	SCENE_EXITS_END,
};

ACTOR_PLACE ColosseumAzartMatchActorPlacements[] = {
	ACTOR_PLACEMENT(ACTOR_PARTY_LEADER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		312, 0, 104, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_WEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		312, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		408, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		456, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, 7,
		601, 0, 146, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, 7,
		552, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, 7,
		664, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		760, 0, 194, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		944, 0, 216, FACING_WEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, 7,
		1048, 0, 136, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7,
		1016, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		360, 0, 424, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		376, 0, 312, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		600, 0, 312, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		824, 0, 312, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		1080, 0, 312, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		1304, 0, 176, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		1304, 0, 208, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{ SCENE_TABLE_END },
};

// The ticks left in the broken bridge's cycle (BRIDGE_CYCLE_TICKS).
Uint32 ColosseumAzartMatchBridgeTimer = 0;

CUE ColosseumAzartMatchCues[] = {
	{ STEP_CUE_WALKING, METATILE_BEHAVIOR_ROCK_CHALLENGE_SQUARE_EDGE, CONDITION_ALWAYS, SCRIPT_REF(scColosseumAzartMatchPushRockOrHopAcross) },
	{ STEP_CUE_WALKING, METATILE_BEHAVIOR_ROCK_CHALLENGE_FLOOR, CONDITION_ALWAYS, SCRIPT_REF(scColosseumAzartMatchPushRock) },
	{ STEP_CUE_WALKING, METATILE_BEHAVIOR_ROCK_CHALLENGE_EAST_END, CONDITION_ALWAYS, SCRIPT_REF(scColosseumAzartMatchPushRock) },
	{ STEP_CUE_WALKING_INTO(FACING_WEST), METATILE_BEHAVIOR_PIPEWORKS_PIPE, FLAG_PIPEWORKS_PIPE_PUSHED, SCRIPT_REF(scColosseumAzartMatchPushPipeworksPipe) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_LOG_NORTH_SOUTH, CONDITION_ALWAYS, SCRIPT_REF(scColosseumAzartMatchRollLogNorthSouth) },
	{ STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_BEHAVIOR_LOG_NORTH_SOUTH, CONDITION_ALWAYS, SCRIPT_REF(scColosseumAzartMatchRollLogNorthSouth) },
	{ STEP_CUE_WALKING_INTO(FACING_WEST), METATILE_BEHAVIOR_LOG_EAST_WEST, CONDITION_ALWAYS, SCRIPT_REF(scColosseumAzartMatchRollLogEastWest) },
	{ STEP_CUE_WALKING_INTO(FACING_EAST), METATILE_BEHAVIOR_LOG_EAST_WEST, CONDITION_ALWAYS, SCRIPT_REF(scColosseumAzartMatchRollLogEastWest) },
	{ CUE_STEP, METATILE_BEHAVIOR_ARENA_ENTRY, FLAG_COLOSSEUM_ARENA_CLOSED, SCRIPT_REF(scColosseumAzartMatchReachArena) },
	{ EXAMINE_CUE(EXAMINE_CHEST), METATILE_BEHAVIOR_FIRST_NUT_CHEST, FLAG_COLOSSEUM_AZART_MATCH_FIRST_NUT_CHEST_OPENED, FIND_ITEM(ITEM_NUT) },
	{ EXAMINE_CUE(EXAMINE_CHEST), METATILE_BEHAVIOR_OIL_DROP_CHEST, FLAG_COLOSSEUM_AZART_MATCH_OIL_DROP_CHEST_OPENED, FIND_ITEM(ITEM_OIL_DROP) },
	{ EXAMINE_CUE(EXAMINE_CHEST) | CUE_FLAG_FACING | FACING_WEST, METATILE_BEHAVIOR_SECOND_NUT_CHEST, FLAG_COLOSSEUM_AZART_MATCH_SECOND_NUT_CHEST_OPENED, FIND_ITEM(ITEM_NUT) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ROCK_A, CONDITION_ALWAYS, SCRIPT_REF(scColosseumAzartMatchUpdateRockMetatiles) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ROCK_B, CONDITION_ALWAYS, SCRIPT_REF(scColosseumAzartMatchUpdateRockMetatiles) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ROCK_C, CONDITION_ALWAYS, SCRIPT_REF(scColosseumAzartMatchUpdateRockMetatiles) },
	{ PSYNERGY_ON_ACTOR(ABILITY_GROWTH), ACTOR_GROWTH_PLANT, FLAG_PLANT_GROWN, SCRIPT_REF(scColosseumAzartMatchGrowPlant) },
	{ PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_PIPE_WEST, FLAG_PIPES_MOVED, SCRIPT_REF(scColosseumAzartMatchDousePipe) },
	{ CUE_TALK, ACTOR_GUIDE, CONDITION_ALWAYS, SCRIPT_REF(scColossoAskDoneLookingAround) },
	{ CUE_TALK, ACTOR_ROCK_ATTENDANT, CONDITION_ALWAYS, SCRIPT_REF(scColosseumAzartMatchTalkToRockAttendant) },
	{ CUE_TALK, ACTOR_PIPEWORKS_ATTENDANT, CONDITION_ALWAYS, SCRIPT_REF(scColosseumAzartMatchTalkToPipeworksAttendant) },
	{ CUE_TALK, ACTOR_BRIDGE_ATTENDANT, CONDITION_ALWAYS, SCRIPT_REF(scColosseumAzartMatchTalkToBridgeAttendant) },
	{ CUE_TALK, ACTOR_LUMBER_ATTENDANT, CONDITION_ALWAYS, SCRIPT_REF(scColosseumAzartMatchTalkToLumberAttendant) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GUIDE, CONDITION_ALWAYS, MSGNO(MsgColossoRiverThisYearsFinalsAreIncredibleThis) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROCK_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoRiverMovingSuchAHugeRockMust) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PIPEWORKS_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoRiverTheresOnlyOneFinalAndThis) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BRIDGE_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoRiverDidYouSeeHisMovesOut) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LUMBER_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoRiverHowManyWarriorsHaveSwallowedTheir) },
	{ CUE_RAISED, COLOSSO_TAKE_PLACE_CUE, CONDITION_ALWAYS,
	  SCRIPT_REF(scColossoTakePlaceForMatch) },
	{ CUE_TABLE_END },
};

// The VRAM cache entry holding the lettering's tiles, -1 when none is loaded.
Sint16 ColossoLetteringSlot = -1;

#include "shared/colosso_lettering_scripts.c"
// clang-format on

// The VRAM cache entry holding the marker's tiles, -1 when none is loaded.
Sint16 ColossoPointerSlot = -1;

#include "shared/colosso_finish_scripts.c"
