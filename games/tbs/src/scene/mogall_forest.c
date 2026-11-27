/* Mogall Forest (SceneId_MogallForestSign to SceneId_MogallForestSidePaths),
   the endless woods between Fuchin Temple and Xian. Master Nyunpa teaches the
   party Force to find the way through: apes hide in the great hollow purple
   stumps, and Force on a stump brings its ape out to show the way on, or to
   attack. Each area is a few screens of clearings joined by paths, and a wrong
   path leads into the side paths and back.

   - The first area (SceneId_MogallForestSign) is entered from the World Map
	 at the north of its first clearing. The first time, an ape peeks out of
	 the stump in the middle and dives back in
	 (FLAG_MOGALL_FOREST_FIRST_APE_SEEN). A broken sign by the path reads
	 `North: Fuchin Temple South: Mogall Prairie...` Force on the stump sends
	 its ape off south. Two clearings on, by the river, the stump on the west
	 of the south bank holds an ape that attacks. Logs on the south bank are
	 pushed about until the last one rolls into the river, a stepping place up
	 to the raised meadow north of it, where the ape in the big stump points
	 east to the second area. A Nut hangs in a tree on the river's east bank.
   - The second area (SceneId_MogallForestQuartz). In the clearing of two
	 stumps the west one's ape leads south and the east one's east, to a pond
	 with an Apple in a chest. South, by the river, Move puts a rock into a
	 ring on the ground so that the log above it can be pushed on, and another
	 log rolls into the stream. The ape in the stump on the island in the
	 stream first leaps to a stump on the west bank, then leads west to the
	 third area. Quartz, a Venus Djinni, waits at the bottom of the clearing to
	 be fought.
   - The third area (SceneId_MogallForestKillerApe). Of three stumps in a row
	 the east one's ape attacks, the middle one's leads south and the west
	 one's west, to an Elven Shirt in a chest that a rock moved with Move and
	 a log pushed east let the party reach. South, two logs on an island roll
	 into the river and a rock settles with Move, and the ape in the stump on
	 its own island leaps to a lower stump, then leads south. There a lone
	 stump stands on an island between two log bridges. Walking up to it, the
	 ape bursts out and runs off south over the bridge
	 (FLAG_MOGALL_FOREST_LED_TO_KILLER_APE). Following it, at the top of the
	 bridge the Killer Ape bursts out and attacks
	 (ENCOUNTER_ZONE_SCRIPTED_MOGALL_ALTIN_LAMAKAN, slot 1), and coming back
	 from the battle at MOGALL_FOREST_KILLER_APE_AFTER_KILLER_APE_ATTACKS marks
	 it beaten (FLAG_MOGALL_FOREST_KILLER_APE_BEATEN). The way south leads out
	 to the World Map and Xian. Entering that clearing at
	 MOGALL_FOREST_KILLER_APE_BRIDGES_FROM_RIVER or
	 MOGALL_FOREST_KILLER_APE_FROM_WORLD_MAP once Force is learned and the
	 Killer Ape beaten sets FLAG_NYUNPA_ENLIGHTENED. Back at Fuchin Temple the
	 monks then say that Nyunpa has reached enlightenment and passed away.
   - SceneId_MogallForestSidePaths, also named Mogall Forest: small clearings
	 that the wrong paths lead into, whose far sides lead back. A Psynergy
	 Stone lies there.

   The apes that attack, one in the first area and one in the third, start a
   battle with an Ape (ENCOUNTER_ZONE_SCRIPTED_MOGALL_ALTIN_LAMAKAN, slot 0). An
   ape stays gone once out, and logs pushed into the river stay there, for as
   long as the party is in the forest (area flags from FLAG_AREA_TEMP_00).

   One overlay serves all four scenes. scGetEntrances, _GetActorPlacements and
   _GetCues choose each scene's tables. scInit puts back the apes already out,
   the rolled logs and the settled rocks, scene by scene and entrance by
   entrance.

   The third area's east ape is also the ape at the lone stump and the Killer
   Ape. The editions differ in how it is kept out of sight once it has come out
   (the #if blocks below and in scInit).

   The apes and rocks are told apart by colour mode (SPR_COL_MODE). An ape in
   hiding is SPRITE_COLOR_HIDDEN. A guide that comes out is drawn green, its red
   and green swapped (SPRITE_COLOR_GRB), and an ape that attacks keeps
   SPRITE_APE's own tan. The Killer Ape is the same sprite a fifth larger and
   blue, its colours rotated (SPRITE_COLOR_BRG). The rocks Move pushes are
   SPRITE_TURNING_ROCK drawn in greys (SPRITE_COLOR_GREY). */

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/field_effect.h"
#include "graphics/graphics.h"
#include "scene/floating_nut.h"
#include "scene/catch_nut.h"
#include "text/msg_ids.h"

extern Sint32		  MogallForest_ActorScript[];
extern SCENE_ENTRANCE MogallForestSign_Entrances[];
extern SCENE_ENTRANCE MogallForestQuartz_Entrances[];
extern SCENE_ENTRANCE MogallForestKillerApe_Entrances[];
extern SCENE_ENTRANCE MogallForestSidePaths_Entrances[];
extern Uint32		  MogallForest_Exits[];
extern ACTOR_PLACE	  MogallForestSign_ActorPlacements[];
extern ACTOR_PLACE	  MogallForestQuartz_ActorPlacements[];
extern ACTOR_PLACE	  MogallForestKillerApe_ActorPlacements[];
extern ACTOR_PLACE	  MogallForestSidePaths_ActorPlacements[];
extern CUE			  MogallForestSign_Cues[];
extern CUE			  MogallForestQuartz_Cues[];
extern CUE			  MogallForestKillerApe_Cues[];
extern CUE			  MogallForestSidePaths_Cues[];

/* Actors of the first area (SceneId_MogallForestSign), numbered in the order of
   MogallForestSign_ActorPlacements. */
typedef enum MOGALL_FOREST_SIGN_ACTOR_ID {
	/* A log lying north to south at (488, 464), pushed as the river log is but
	   with nothing happening where it stops. */
	ACTOR_FIRST_AREA_LOG = ACTOR_FIRST_PLACED,
	/* The log lying west to east on the river's south bank, which rolls into
	   the river. */
	ACTOR_FIRST_AREA_RIVER_LOG,
	/* The ape in the stump in the middle of the clearing entered from the World
	   Map, the one that peeks out on the first arrival. */
	ACTOR_FIRST_AREA_NORTH_APE,
	/* The ape in the stump on the west of the river's south bank. */
	ACTOR_FIRST_AREA_ATTACKING_APE,
	/* The ape in the big stump on the raised meadow north of the river. */
	ACTOR_FIRST_AREA_EAST_APE,
	/* The stumps themselves: blank actors on the apes' spots, the targets of
	   Force. */
	ACTOR_FIRST_AREA_NORTH_APE_STUMP,
	ACTOR_FIRST_AREA_ATTACKING_APE_STUMP,
	ACTOR_FIRST_AREA_EAST_APE_STUMP,
	/* A Nut in a tree on the river's east bank, caught with Catch. */
	ACTOR_NUT
} MOGALL_FOREST_SIGN_ACTOR_ID;

/* Actors of the second area (SceneId_MogallForestQuartz), numbered in the order
   of MogallForestQuartz_ActorPlacements. */
typedef enum MOGALL_FOREST_QUARTZ_ACTOR_ID {
	/* The upper log lying west to east by the river, held by the rock below
	   it. */
	ACTOR_SECOND_AREA_LOG = ACTOR_FIRST_PLACED,
	/* A log lying west to east at (592, 456), south of ACTOR_SECOND_AREA_LOG,
	   pushed with nothing happening where it stops. */
	ACTOR_SECOND_AREA_SOUTH_LOG,
	/* The log lying north to south by the stream, which rolls into it. */
	ACTOR_SECOND_AREA_RIVER_LOG,
	/* The small rock below the upper log, moved with Move.

	   It is drawn as the rocks of Altmiller Cave's color puzzle
	   (MsgAltmillerRotatedRock), drawn plain as those are while Reveal is
	   off. */
	ACTOR_SECOND_AREA_ROCK,
	/* The apes in the two stumps side by side in the clearing of stumps. */
	ACTOR_SECOND_AREA_WEST_APE,
	ACTOR_SECOND_AREA_EAST_APE,
	/* The ape in the stump on the island in the stream, which moves to a stump
	   on the stream's west bank. */
	ACTOR_SECOND_AREA_SOUTH_APE,
	ACTOR_SECOND_AREA_WEST_APE_STUMP,
	ACTOR_SECOND_AREA_EAST_APE_STUMP,
	ACTOR_SECOND_AREA_SOUTH_APE_STUMP,
	/* Quartz, a Venus Djinni, at the bottom of the river clearing until the
	   party has it. */
	ACTOR_SECOND_AREA_QUARTZ
} MOGALL_FOREST_QUARTZ_ACTOR_ID;

/* Actors of the third area (SceneId_MogallForestKillerApe), numbered in the
   order of MogallForestKillerApe_ActorPlacements. */
typedef enum MOGALL_FOREST_KILLER_APE_ACTOR_ID {
	/* The log lying north to south beside the Elven Shirt's chest. */
	ACTOR_THIRD_AREA_LOG = ACTOR_FIRST_PLACED,
	/* The upper of the two logs lying north to south on the island in the
	   river, which roll east into it. */
	ACTOR_THIRD_AREA_NORTH_RIVER_LOG,
	/* A log lying north to south at (552, 480), west of the two river logs,
	   pushed with nothing happening where it stops. */
	ACTOR_THIRD_AREA_WEST_LOG,
	ACTOR_THIRD_AREA_SOUTH_RIVER_LOG,
	/* A log lying west to east at (544, 536), south of
	   ACTOR_THIRD_AREA_WEST_LOG, pushed with nothing happening where it
	   stops. */
	ACTOR_THIRD_AREA_SOUTHWEST_LOG,
	/* Two rocks moved with Move, as in the second area: the one beside the
	   Elven Shirt's log and the one beside the upper log on the river
	   island. */
	ACTOR_THIRD_AREA_NORTH_ROCK,
	ACTOR_THIRD_AREA_SOUTH_ROCK,
	/* The apes in the three stumps in a row: the east one, the middle and the
	   west. */
	ACTOR_THIRD_AREA_ATTACKING_APE,
	ACTOR_THIRD_AREA_MIDDLE_APE,
	ACTOR_THIRD_AREA_WEST_APE,
	/* The ape in the stump on its own island in the river clearing, which moves
	   to a lower stump. It is also the ape at the lone stump and the Killer
	   Ape. */
	ACTOR_THIRD_AREA_EAST_APE,
	ACTOR_THIRD_AREA_ATTACKING_APE_STUMP,
	ACTOR_THIRD_AREA_MIDDLE_APE_STUMP,
	ACTOR_THIRD_AREA_WEST_APE_STUMP,
	ACTOR_THIRD_AREA_EAST_APE_STUMP
} MOGALL_FOREST_KILLER_APE_ACTOR_ID;

/* The animations of a river log's sprite (SPRITE_LOG_NORTH_SOUTH and
   SPRITE_LOG_WEST_EAST), named by when these scenes play them. */
typedef enum MOGALL_FOREST_LOG_ANIMATION {
	MOGALL_FOREST_LOG_ROLLING = 3,	  /* as it rolls over the bank */
	MOGALL_FOREST_LOG_IN_RIVER = 4,	  /* lying in the river, set on arrival */
	MOGALL_FOREST_LOG_STEPPED_ON = 7, /* as the leader jumps onto it */
	MOGALL_FOREST_LOG_FALLING_IN = 8  /* as it drops into the water */
} MOGALL_FOREST_LOG_ANIMATION;

/* Kept until the party leaves the forest.

   There is one for each ape once it has come out (or, for two, moved on to a
   second hiding place) and one for each rolled log. */
enum {
	FLAG_MOGALL_FOREST_FIRST_AREA_NORTH_APE_OUT = FLAG_AREA_TEMP_00,
	/* The ape that leaps at the leader instead of pointing the way. */
	FLAG_MOGALL_FOREST_FIRST_AREA_ATTACKING_APE_OUT,
	FLAG_MOGALL_FOREST_FIRST_AREA_EAST_APE_OUT,
	FLAG_MOGALL_FOREST_SECOND_AREA_WEST_APE_OUT,
	FLAG_MOGALL_FOREST_SECOND_AREA_EAST_APE_OUT,
	FLAG_MOGALL_FOREST_SECOND_AREA_SOUTH_APE_MOVED,
	FLAG_MOGALL_FOREST_SECOND_AREA_SOUTH_APE_OUT,
	/* The ape that leaps at the leader instead of pointing the way. */
	FLAG_MOGALL_FOREST_THIRD_AREA_ATTACKING_APE_OUT,
	FLAG_MOGALL_FOREST_THIRD_AREA_MIDDLE_APE_OUT,
	FLAG_MOGALL_FOREST_THIRD_AREA_WEST_APE_OUT,
	FLAG_MOGALL_FOREST_THIRD_AREA_EAST_APE_MOVED,
	FLAG_MOGALL_FOREST_THIRD_AREA_EAST_APE_OUT,
	FLAG_MOGALL_FOREST_FIRST_AREA_LOG_ROLLED = FLAG_AREA_TEMP_10,
	FLAG_MOGALL_FOREST_SECOND_AREA_LOG_ROLLED,
	FLAG_MOGALL_FOREST_THIRD_AREA_NORTH_LOG_ROLLED,
	FLAG_MOGALL_FOREST_THIRD_AREA_SOUTH_LOG_ROLLED,
	FLAG_MOGALL_FOREST_PSYNERGY_STONE = FLAG_AREA_TEMP_50
};

/* The metatile behaviors the cue tables answer. */
typedef enum MOGALL_FOREST_METATILE_BEHAVIOR {
	/* Under each log the party pushes by walking into it. */
	METATILE_BEHAVIOR_MOGALL_FOREST_LOG = 20,
	/* Beside the lone stump on the island between the two log bridges,
	   metatiles (12, 22) and (12, 23). */
	METATILE_BEHAVIOR_MOGALL_FOREST_BESIDE_LONE_STUMP = 22,
	/* At the top of the south log bridge below the lone stump, metatile (8,
	   27). */
	METATILE_BEHAVIOR_MOGALL_FOREST_BELOW_LONE_STUMP = 23,
	/* Where walking on jumps across water: from the first area's river log up
	   to the meadow north of it, and onto the third area's lower river log from
	   the bank east of it. */
	METATILE_BEHAVIOR_MOGALL_FOREST_JUMP = 25,
	/* The edge of the first area's meadow above the river log, where walking
	   south jumps down onto it. */
	METATILE_BEHAVIOR_MOGALL_FOREST_JUMP_SOUTH = 26,
	/* The broken sign at the first clearing, metatiles (9, 3) and (9, 4). */
	METATILE_BEHAVIOR_MOGALL_FOREST_BROKEN_SIGN = 27
} MOGALL_FOREST_METATILE_BEHAVIOR;

/* An exit metatile: its metatile behavior is the number of the exit it
   takes. */
#define EXIT_METATILE(exit) {CUE_EXIT, (exit), CONDITION_ALWAYS, (exit)}

/* Scene flag, kept while the party stays in the area. */
enum { FLAG_MOGALL_FOREST_SOUTH_ROCK_SETTLED = FLAG_SCENE_TEMP_14 };

/* The third area's east ape, at (744, 400). The English edition places it as
   the other apes, only while it has not come out
   (FLAG_MOGALL_FOREST_THIRD_AREA_EAST_APE_OUT clear); the Japanese and European
   editions place it always. */
#if MOGALL_THIRD_AREA_EAST_APE_PLACED_UNTIL_OUT
#define MOGALL_FOREST_EAST_ACTOR_CONDITION \
	FLAG_MOGALL_FOREST_THIRD_AREA_EAST_APE_OUT
#else
#define MOGALL_FOREST_EAST_ACTOR_CONDITION CONDITION_ALWAYS
#endif

/* The east ape's hiding place, which shares its sprite: the Japanese edition
   places it always, the others only while the ape has not come out. */
#if !MOGALL_THIRD_AREA_EAST_APE_HIDING_PLACE_UNTIL_OUT
#define MOGALL_FOREST_EAST_SHARED_SPRITE_CONDITION CONDITION_ALWAYS
#else
#define MOGALL_FOREST_EAST_SHARED_SPRITE_CONDITION \
	FLAG_MOGALL_FOREST_THIRD_AREA_EAST_APE_OUT
#endif

#include "shared/pushed_actor.c"

#include "shared/field_effect_create.c"
#include "shared/field_effect_spawn.c"

/* Talking to Quartz, the Venus Djinni at the bottom of the second area's river
   clearing, starts the battle with it. */
static void scMogallForestStartDjinnBattle(void)
{
	fdRequestDjinnBattle(ACTOR_SECOND_AREA_QUARTZ, DJINNI_QUARTZ);
}

/* The leader springs and leaps to a spot (in 16.16 pixels). Returns 0, and does
   nothing, when the spot is blocked. */
static Bool scMogallForestTryJumpLeaderTo(FIELD_POS *position)
{
	ACTOR *leader = fdActorGet(ACTOR_PARTY_LEADER);
	Sint32 savedMotionFlags = leader->motionFlg;
	Sint32 collision = syFieldActorCheckMovementCollision(leader, position);

	if(collision == 0) {
		fdScriptStart();
		fdActorSelectAnimation(leader, ANIM_LEADER_SPRING);
		sySynch(6);
		sySndPlay(SE_JUMP);
		fdActorSelectAnimation(leader, ANIM_LEADER_LEAP);
		leader->speedLimit = toFIXED(3);
		leader->acceleration = toFIXED(2);
		leader->velY = toFIXED(4);
		leader->motionFlg &= ACTOR_JUMP_KEEPS_MOTION;
		fdActorSetSprFlags(leader, 0);
		fdActorMoveToAndWait(ACTOR_PARTY_LEADER, FIXEDtoINT(position->x),
			FIXEDtoINT(position->z));
		fdActorSelectAnimation(leader, ANIM_LEADER_SPRING);
		fdActorSetSprFlags(leader, SPRITE_FLAG_SHADOW);
		leader->motionFlg = savedMotionFlags;
		fdScriptEnd();
		return TRUE;
	}
	return FALSE;
}

/* An actor script call: draws the actor in the colour mode its timer holds. */
static Sint32 scMogallForestSetColFromTimer(ACTOR *actor)
{
	fdActorSetSprCol(actor, actor->tmr);
	return 0;
}

/* Clears the actor's sprite flags when its script requests it. */
static Sint32 scMogallForestClearActorSprFlags(ACTOR *actor)
{
	fdActorSetSprFlags(actor, 0);
	return 0;
}

/* Hops an actor to a destination in whole pixels with a vertical speed in 16.16
   fixed point; scenes call it through scMogallForestHopActorTo. */
static void scMogallForestHopActorToWithSpeedFX(
	Sint32 actorId, Sint32 targetX, Sint32 targetZ, FIXED verticalSpeed)
{
	ACTOR *actor;

	actor = fdActorGet(actorId);
	fdActorSetSprPriority(actorId, 1);
	fdActorSetSpeed(actorId, 3);
	sySndPlay(SE_JUMP);
	actor->velY = verticalSpeed;
	actor->gravity = toFIXED(1) / 2;
	actor->bounce = 0;
	fdActorSetSprFlags(actor, 0);
	fdActorMoveToAndWait(actorId, targetX, targetZ);
	fdActorSetPos(actorId, targetX, targetZ);
	fdActorSetSprFlags(actor, SPRITE_FLAG_SHADOW);
	actor->gravity = toFIXED(1);
}

/* Hops an actor with a vertical speed in pixels a frame:
   `scMogallForestHopActorTo(ACTOR_FIRST_AREA_NORTH_APE, 88, 120, 6)`. */
#define scMogallForestHopActorTo(actorId, targetX, targetZ, verticalSpeed) \
	scMogallForestHopActorToWithSpeedFX( \
		actorId, targetX, targetZ, toFIXED(verticalSpeed))

/* With a thump, scatters a ring of puffs of dust round an actor: the puff an
   ape makes popping out of a stump or diving back in. */
static void scMogallForestSpawnPuffRing(Sint32 id)
{
	VECTOR3		   velocity;
	EFFECT_OPTIONS options;
	ACTOR		  *actor;
	Uint32		   direction;

	actor = fdActorGet(id);
	sySndPlay(SE_THUMP);
	options.pri = 1;
	/* Seventeen puffs a sixteenth of a turn apart, the first and last together,
	   spread a third wider east to west. */
	for(direction = 0; direction <= TRIG_TURN / FACING_STEP; direction++) {
		velocity.x = syCos(direction * FACING_STEP);
		velocity.y = 0;
		velocity.z = sySin(direction * FACING_STEP);
		velocity.x += velocity.x / 3;
		fdEffectSpawn(actor->x.fixed, toFIXED(16), actor->z.fixed, velocity.x,
			velocity.y + toFIXED(1) / 10, velocity.z, EFFECT_USE_PRIORITY,
			&options);
	}
}

/* Pans the camera to an ape's stump and, half a second later, brings the ape
   out in a puff of dust, drawn in the colour mode given. */
static void scMogallForestApeComesOut(Sint32 actorId, Sint32 color)
{
	fdCameraSetSpeedParms(toFIXED(2), toFIXED(1) / 4);
	fdCameraMoveToActorById(actorId, 1);
	fdCameraWaitForArrival();
	fdScriptWait(30);
	scMogallForestSpawnPuffRing(actorId);
	fdActorSetSprColById(actorId, color);
}

/* The ape turns to the leader, hops on the spot and gestures the way on. */
static inline void scMogallForestApePointsTheWay(Sint32 guideId)
{
	fdCameraFollowActor(guideId, 1);
	fdActorTurnToFaceEachOther(guideId, ACTOR_PARTY_LEADER, 0);
	fdScriptWait(20);
	fdActorStartRepeatedMotion(guideId, 2);
	fdActorSetAttachedEffect(
		guideId, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(60);
}

const SCENE_ENTRANCE *scGetEntrances(void)
{
	Sint16 scene = MainRoutineStateWork.next;

	if(scene == SCENE_MOGALL_FOREST_SIGN) {
		return MogallForestSign_Entrances;
	}
	if(scene == SCENE_MOGALL_FOREST_QUARTZ) {
		return MogallForestQuartz_Entrances;
	}
	if(scene == SCENE_MOGALL_FOREST_KILLER_APE) {
		return MogallForestKillerApe_Entrances;
	}
	return MogallForestSidePaths_Entrances;
}

const SCENE_EXIT_REGION *scGetExitRegions(void)
{
	return NULL;
}

/* Returns the exits, one table for all four scenes. */
const Uint32 *scGetExits(void)
{
	return MogallForest_Exits;
}

const ACTOR_PLACE *scGetActorPlacements(void)
{
	Sint16 scene = MainRoutineStateWork.next;

	if(scene == SCENE_MOGALL_FOREST_SIGN) {
		return MogallForestSign_ActorPlacements;
	}
	if(scene == SCENE_MOGALL_FOREST_QUARTZ) {
		return MogallForestQuartz_ActorPlacements;
	}
	if(scene == SCENE_MOGALL_FOREST_KILLER_APE) {
		return MogallForestKillerApe_ActorPlacements;
	}
	return MogallForestSidePaths_ActorPlacements;
}

/* The map rows and columns where a pushed log reaches the water and rolls in,
   or is out of the way. */
enum {
	/* The first area's river log, at the river's south bank. */
	FIRST_AREA_RIVER_BANK_ROW = 26,
	/* The second area's upper log, pushed a metatile south into the place of
	   the rock below it. */
	SECOND_AREA_LOG_OPENS_WAY_ROW = 23,
	/* The second area's river log, at the stream's east bank. */
	SECOND_AREA_RIVER_BANK_COLUMN = 35,
	/* The third area's upper river log, at the island's east bank. */
	THIRD_AREA_NORTH_RIVER_BANK_COLUMN = 42,
	/* The third area's lower river log, at the island's east bank. */
	THIRD_AREA_SOUTH_RIVER_BANK_COLUMN = 40
};

/* Isaac pushes a log on the first area's river bank a metatile by walking into
   it. The log lying west to east, pushed north to the river (row 26), rolls in
   and lies there, a stepping place up to the meadow on the far side.

   Rolling in, the log gives the metatiles (31, 25) to (34, 26) the jump north
   on their north row, copied from a spare stretch of the map at (38, 27). */
static void scMogallForestFirstAreaPushLog(void)
{
	PUSHED_ACTOR_PROBE probe;

	fdScriptStart();
	if(scPushedActorFindClearPos(&probe) != 0) {
		scPushedActorRoll(probe);
		if(probe.actorSlot == ACTOR_FIRST_AREA_RIVER_LOG &&
			FIELD_METATILE(probe.pos.z) == FIRST_AREA_RIVER_BANK_ROW) {
			gmFlagSet(FLAG_MOGALL_FOREST_FIRST_AREA_LOG_ROLLED);
			fdActorSetAnimation(
				ACTOR_FIRST_AREA_RIVER_LOG, MOGALL_FOREST_LOG_ROLLING);
			fdActorSetSpeedAndAcceleration(
				ACTOR_FIRST_AREA_RIVER_LOG, 0.25, 0.5);
			fdActorSetDestinationOffsetById(ACTOR_FIRST_AREA_RIVER_LOG, 0, -16);
			fdScriptWait(45);
			fdActorSetAnimation(
				ACTOR_FIRST_AREA_RIVER_LOG, MOGALL_FOREST_LOG_FALLING_IN);
			sySndPlay(SE_LOG_FALL);
			fdActorSetSprPriority(ACTOR_FIRST_AREA_RIVER_LOG, 1);
			fdActorGet(ACTOR_FIRST_AREA_RIVER_LOG)->priFlg =
				ACTOR_PRIORITY_UNDERFOOT;
			fdMapGridCopyMetatileAttributes(38, 27, 4, 2, 31, 25);
		}
	}
	fdScriptEnd();
}

/* Walking north along the log lying in the first area's river (metatiles (31,
   25) to (34, 25)) makes Isaac leap up onto the raised meadow north of it, by
   the big stump. */
static void scMogallForestFirstAreaJumpNorth(void)
{
	FIELD_POS position;
#if JUMP_LANDS_ON_CELL_CENTER
	ACTOR *leader = fdActorGet(ACTOR_PARTY_LEADER);

	position.x = (leader->x.fixed & FIELD_METATILE_MASK) + toFIXED(8);
	position.y = leader->y.fixed;
	position.z = (leader->z.fixed & FIELD_METATILE_MASK) - toFIXED(24);
#else
	position.x = fdActorGet(ACTOR_PARTY_LEADER)->x.fixed;
	position.y = fdActorGet(ACTOR_PARTY_LEADER)->y.fixed;
	position.z = fdActorGet(ACTOR_PARTY_LEADER)->z.fixed - toFIXED(32);
#endif
	scMogallForestTryJumpLeaderTo(&position);
}

/* Walking south off the edge of the first area's raised meadow (metatiles (31,
   23) to (34, 23)) makes Isaac leap down onto the log lying in the river, which
   dips under him.

   The ground and the leader sink a pixel, then another, and rise back. */
static void scMogallForestFirstAreaJumpSouth(void)
{
	FIELD_POS position;
	ACTOR	 *leader = fdActorGet(ACTOR_PARTY_LEADER);
	Sint32	  savedMotionFlags = leader->motionFlg;

#if JUMP_LANDS_ON_CELL_CENTER
	position.x = (leader->x.fixed & FIELD_METATILE_MASK) + toFIXED(8);
	position.y = leader->y.fixed;
	position.z = (leader->z.fixed & FIELD_METATILE_MASK) + toFIXED(40);
#else
	position.x = fdActorGet(ACTOR_PARTY_LEADER)->x.fixed;
	position.y = fdActorGet(ACTOR_PARTY_LEADER)->y.fixed;
	position.z = fdActorGet(ACTOR_PARTY_LEADER)->z.fixed + toFIXED(32);
#endif
	if(scMogallForestTryJumpLeaderTo(&position) != 0) {
		fdScriptStart();
		leader->motionFlg = 0;
		fdActorSetAnimation(
			ACTOR_FIRST_AREA_RIVER_LOG, MOGALL_FOREST_LOG_STEPPED_ON);
		leader->y.fixed -= toFIXED(1);
		leader->groundHeight -= toFIXED(1);
		sySynch(2);
		leader->y.fixed -= toFIXED(1);
		leader->groundHeight -= toFIXED(1);
		sySynch(10);
		leader->y.fixed += toFIXED(1);
		leader->groundHeight += toFIXED(1);
		sySynch(4);
		leader->y.fixed += toFIXED(1);
		leader->groundHeight += toFIXED(1);
		leader->motionFlg = savedMotionFlags;
		fdScriptEnd();
	}
}

/* Isaac reads the broken sign by the path just inside the forest, a broken post
   a step from where he arrives from the World Map (metatiles (9, 3) and (9,
   4)):
   `The broken sign reads: North: Fuchin Temple South: Mogall Prairie...` */
static void scMogallForestReadBrokenSign(void)
{
	fdScriptStart();
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	mnFieldMessageShow(MSGNO(MsgMogallForestBrokenSign), 1);
	fdScriptEnd();
}

/* Force on the stump in the middle of the first clearing brings a green ape out
   of it. The ape turns to Isaac, points the way, and hops off south, the way on
   to the river. */
static void scMogallForestFirstAreaNorthApeShowsWay(void)
{
	ACTOR *ape;

	ape = fdActorGet(ACTOR_FIRST_AREA_NORTH_APE);

	fdScriptStart();

	scMogallForestApeComesOut(ACTOR_FIRST_AREA_NORTH_APE, SPRITE_COLOR_GRB);
	scMogallForestHopActorTo(ACTOR_FIRST_AREA_NORTH_APE, 88, 120, 6);

	fdEffectSpawn(ape->x.fixed, ape->y.fixed, ape->z.fixed + toFIXED(24), 0, 0,
		0, EFFECT_SCRIPT_MEDIUM, 0);

	scMogallForestApePointsTheWay(ACTOR_FIRST_AREA_NORTH_APE);

	scMogallForestHopActorTo(ACTOR_FIRST_AREA_NORTH_APE, 88, 152, 3);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_FIRST_AREA_NORTH_APE, 0);
	fdScriptWait(10);

	scMogallForestHopActorTo(ACTOR_FIRST_AREA_NORTH_APE, 120, 192, 3);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_FIRST_AREA_NORTH_APE, 0);
	fdScriptWait(10);

	scMogallForestHopActorTo(ACTOR_FIRST_AREA_NORTH_APE, 120, 240, 3);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_FIRST_AREA_NORTH_APE, 0);
	fdScriptWait(10);

	gmFlagSet(FLAG_MOGALL_FOREST_FIRST_AREA_NORTH_APE_OUT);
	fdActorSetPos(ACTOR_FIRST_AREA_NORTH_APE_STUMP, 0, 0);
	fdActorSetPos(ACTOR_FIRST_AREA_NORTH_APE, 0, 0);

	fdScriptEnd();
}

/* Force on the stump on the west of the river's south bank, in the first area,
   brings out an ape that cries out in anger, leaps at Isaac and starts a battle
   with it. */
static void scMogallForestFirstAreaApeAttacks(void)
{
	ACTOR *ape;

	ape = fdActorGet(ACTOR_FIRST_AREA_ATTACKING_APE);

	fdScriptStart();

	scMogallForestApeComesOut(
		ACTOR_FIRST_AREA_ATTACKING_APE, SPRITE_COLOR_NORMAL);
	scMogallForestHopActorTo(ACTOR_FIRST_AREA_ATTACKING_APE, 408, 456, 6);

	fdEffectSpawn(ape->x.fixed, ape->y.fixed, ape->z.fixed + toFIXED(24), 0, 0,
		0, EFFECT_SCRIPT_MEDIUM, 0);

	fdCameraFollowActor(ACTOR_FIRST_AREA_ATTACKING_APE, 1);
	fdActorTurnToFaceEachOther(
		ACTOR_FIRST_AREA_ATTACKING_APE, ACTOR_PARTY_LEADER, 0);
	fdScriptWait(30);
	fdActorStartRepeatedMotion(ACTOR_FIRST_AREA_ATTACKING_APE, 2);
	fdActorShowEmote(
		ACTOR_FIRST_AREA_ATTACKING_APE, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
	sySndPlay(SE_BEAST_CRY);
	fdScriptWait(60);

	scMogallForestHopActorTo(ACTOR_FIRST_AREA_ATTACKING_APE,
		fdActorGet(ACTOR_PARTY_LEADER)->x.part[0].integer,
		fdActorGet(ACTOR_PARTY_LEADER)->z.part[0].integer, 4);

	fdScriptWait(10);
	gmFlagSet(FLAG_MOGALL_FOREST_FIRST_AREA_ATTACKING_APE_OUT);
	fdActorSetPos(ACTOR_FIRST_AREA_ATTACKING_APE_STUMP, 0, 0);

	SET_BATTLE_TYPE(BATTLE_TYPE_SCRIPTED);

	fdRequestBattleFromEncounterTable(
		ENCOUNTER_ZONE_SCRIPTED_MOGALL_ALTIN_LAMAKAN, 0);

	fdScriptEnd();
}

/* Force on the big stump on the meadow north of the first area's river brings a
   green ape out of it. The ape points the way and hops off east, the way to the
   second area. */
static void scMogallForestFirstAreaEastApeShowsWay(void)
{
	ACTOR *ape;

	ape = fdActorGet(ACTOR_FIRST_AREA_EAST_APE);

	fdScriptStart();

	scMogallForestApeComesOut(ACTOR_FIRST_AREA_EAST_APE, SPRITE_COLOR_GRB);
	scMogallForestHopActorTo(ACTOR_FIRST_AREA_EAST_APE, 536, 344, 7);

	fdEffectSpawn(ape->x.fixed, ape->y.fixed, ape->z.fixed + toFIXED(16), 0, 0,
		0, EFFECT_SCRIPT_MEDIUM, 0);

	scMogallForestApePointsTheWay(ACTOR_FIRST_AREA_EAST_APE);

	scMogallForestHopActorTo(ACTOR_FIRST_AREA_EAST_APE, 584, 344, 3);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_FIRST_AREA_EAST_APE, 0);
	fdScriptWait(6);

	scMogallForestHopActorTo(ACTOR_FIRST_AREA_EAST_APE, 632, 344, 3);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_FIRST_AREA_EAST_APE, 0);
	fdScriptWait(6);

	scMogallForestHopActorTo(ACTOR_FIRST_AREA_EAST_APE, 680, 344, 3);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_FIRST_AREA_EAST_APE, 0);
	fdScriptWait(6);

	gmFlagSet(FLAG_MOGALL_FOREST_FIRST_AREA_EAST_APE_OUT);
	fdActorSetPos(ACTOR_FIRST_AREA_EAST_APE_STUMP, 0, 0);

	fdScriptEnd();
}

/* Catch on the Nut in the tree on the east bank of the first area's river
   brings it down to Isaac: `Isaac got a Nut.` */
static void scMogallForestCatchNut(void)
{
	scCatchNut(ACTOR_NUT, FLAG_MOGALL_FOREST_NUT_CAUGHT);
}

/* Isaac pushes a log in the second area's river clearing a metatile by walking
   into it. Once Move has put the rock below it into its ring, the upper log can
   go a metatile south and the way on is open. The log by the stream, pushed
   west to its bank, rolls into the stream and lies there as a way across.

   The upper log's push copies the metatiles (35, 67) to (38, 67) onto the row
   below them. The rolled log copies its metatiles in the stream from a spare
   stretch of the map at (44, 30). */
static void scMogallForestSecondAreaPushLog(void)
{
	PUSHED_ACTOR_PROBE probe;
	Sint32			   destinationColumn;
	Sint32			   destinationRow;

	fdScriptStart();

	if(scPushedActorFindClearPos(&probe) != 0) {
		scPushedActorRoll(probe);

		if(probe.actorSlot == ACTOR_SECOND_AREA_LOG &&
			(FIELD_METATILE(probe.pos.z)) == SECOND_AREA_LOG_OPENS_WAY_ROW) {
			destinationColumn = 35;
			destinationRow = 68;
			fdMapGridCopyMetatileAttributes(
				35, 67, 4, 1, destinationColumn, destinationRow);
		} else if(probe.actorSlot == ACTOR_SECOND_AREA_RIVER_LOG &&
			(FIELD_METATILE(probe.pos.x)) == SECOND_AREA_RIVER_BANK_COLUMN) {
			gmFlagSet(FLAG_MOGALL_FOREST_SECOND_AREA_LOG_ROLLED);
			fdActorSetAnimation(
				ACTOR_SECOND_AREA_RIVER_LOG, MOGALL_FOREST_LOG_ROLLING);
			fdActorSetDestinationOffsetById(
				ACTOR_SECOND_AREA_RIVER_LOG, -16, 6);
			fdScriptWait(30);
			fdActorSetAnimation(
				ACTOR_SECOND_AREA_RIVER_LOG, MOGALL_FOREST_LOG_FALLING_IN);
			sySndPlay(SE_LOG_FALL);

			fdActorGet(ACTOR_SECOND_AREA_RIVER_LOG)->priFlg =
				ACTOR_PRIORITY_UNDERFOOT;

			destinationColumn = 34;
			destinationRow = 30;
			fdMapGridCopyMetatileAttributes(
				44, 30, 2, 4, destinationColumn, destinationRow);
			scMapGridFillCollisionRectangle(2, 35, 30, 1, 4, 0);
		}
	}

	fdScriptEnd();
}

/* Makes the rock's new metatile solid and clears the four metatiles round it,
   one of which it came from. */
static inline void scMogallForestUpdateRockMetatiles(Sint32 column, Sint32 row)
{
	scMapGridFillCollisionRectangle(
		2, column, row, 1, 1, METATILE_BEHAVIOR_WALL);
	scMapGridFillCollisionRectangle(
		2, column + 1, row, 1, 1, METATILE_BEHAVIOR_NORMAL);
	scMapGridFillCollisionRectangle(
		2, column - 1, row, 1, 1, METATILE_BEHAVIOR_NORMAL);
	scMapGridFillCollisionRectangle(
		2, column, row + 1, 1, 1, METATILE_BEHAVIOR_NORMAL);
	scMapGridFillCollisionRectangle(
		2, column, row - 1, 1, 1, METATILE_BEHAVIOR_NORMAL);
}

/* Move on the small grey rock below the upper log in the second area's river
   clearing pushes it a metatile. Pushed south into the ring on the ground
   (metatile (36, 24)), it sinks 2 pixels into place and stays, and the log can
   be pushed. */
static void scMogallForestSecondAreaUpdateRock(void)
{
	Sint32 column;
	Sint32 row;

	fdScriptStart();

	column = FIELD_METATILE(fdActorGet(ACTOR_SECOND_AREA_ROCK)->x.fixed);
	row = FIELD_METATILE(fdActorGet(ACTOR_SECOND_AREA_ROCK)->z.fixed);

	scMogallForestUpdateRockMetatiles(column, row);

	if(column == 36 && row == 24) {
		ACTOR *rock = fdActorGet(ACTOR_SECOND_AREA_ROCK);

		rock->motionFlg = 0;
		rock->groundHeight = -toFIXED(2);
		rock->y.fixed = -toFIXED(2);
	}

	fdScriptEnd();
}

/* Force on the west one of the two stumps side by side in the second area
   brings a green ape out of it. The ape points the way and hops off south, the
   way on toward the river. */
static void scMogallForestSecondAreaWestApeShowsWay(void)
{
	ACTOR *ape;

	ape = fdActorGet(ACTOR_SECOND_AREA_WEST_APE);
	fdScriptStart();
	scMogallForestApeComesOut(ACTOR_SECOND_AREA_WEST_APE, SPRITE_COLOR_GRB);
	scMogallForestHopActorTo(ACTOR_SECOND_AREA_WEST_APE, 392, 104, 7);
	fdScriptWait(10);
	fdEffectSpawn(ape->x.fixed, ape->y.fixed, (ape->z.fixed + toFIXED(4)), 0, 0,
		0, EFFECT_SCRIPT_MEDIUM, 0);
	scMogallForestApePointsTheWay(ACTOR_SECOND_AREA_WEST_APE);
	scMogallForestHopActorTo(ACTOR_SECOND_AREA_WEST_APE, 424, 120, 3);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_SECOND_AREA_WEST_APE, 0);
	fdScriptWait(6);
	scMogallForestHopActorTo(ACTOR_SECOND_AREA_WEST_APE, 424, 168, 3);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_SECOND_AREA_WEST_APE, 0);
	fdScriptWait(6);
	scMogallForestHopActorTo(ACTOR_SECOND_AREA_WEST_APE, 424, 208, 3);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_SECOND_AREA_WEST_APE, 0);
	fdScriptWait(6);
	scMogallForestHopActorTo(ACTOR_SECOND_AREA_WEST_APE, 424, 232, 3);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_SECOND_AREA_WEST_APE, 0);
	fdScriptWait(6);
	fdActorSetPos(ACTOR_SECOND_AREA_WEST_APE, 0, 0);
	gmFlagSet(FLAG_MOGALL_FOREST_SECOND_AREA_WEST_APE_OUT);
	fdActorSetPos(ACTOR_SECOND_AREA_WEST_APE_STUMP, 0, 0);
	fdScriptEnd();
}

/* Force on the east one of the two stumps side by side in the second area
   brings a green ape out of it. The ape points the way and hops off east,
   toward the pond with the Apple's chest. */
static void scMogallForestSecondAreaEastApeShowsWay(void)
{
	ACTOR *ape;

	ape = fdActorGet(ACTOR_SECOND_AREA_EAST_APE);

	fdScriptStart();

	scMogallForestApeComesOut(ACTOR_SECOND_AREA_EAST_APE, SPRITE_COLOR_GRB);
	scMogallForestHopActorTo(ACTOR_SECOND_AREA_EAST_APE, 456, 104, 7);
	fdScriptWait(10);

	fdEffectSpawn(ape->x.fixed, ape->y.fixed, ape->z.fixed + toFIXED(4), 0, 0,
		0, EFFECT_SCRIPT_MEDIUM, 0);

	scMogallForestApePointsTheWay(ACTOR_SECOND_AREA_EAST_APE);

	scMogallForestHopActorTo(ACTOR_SECOND_AREA_EAST_APE, 472, 136, 3);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_SECOND_AREA_EAST_APE, 0);
	fdScriptWait(6);

	scMogallForestHopActorTo(ACTOR_SECOND_AREA_EAST_APE, 504, 136, 3.2);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_SECOND_AREA_EAST_APE, 0);
	fdScriptWait(6);

	scMogallForestHopActorTo(ACTOR_SECOND_AREA_EAST_APE, 552, 136, 3.5);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_SECOND_AREA_EAST_APE, 0);
	fdScriptWait(6);

	scMogallForestHopActorTo(ACTOR_SECOND_AREA_EAST_APE, 584, 136, 3.5);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_SECOND_AREA_EAST_APE, 0);
	fdScriptWait(6);

	fdActorSetPos(ACTOR_SECOND_AREA_EAST_APE, 0, 0);
	gmFlagSet(FLAG_MOGALL_FOREST_SECOND_AREA_EAST_APE_OUT);
	fdActorSetPos(ACTOR_SECOND_AREA_EAST_APE_STUMP, 0, 0);

	fdScriptEnd();
}

/* Force on the stump on the island in the second area's stream brings a green
   ape out, which leaps west across the stream into the stump on the bank and
   hides again. */
static void scMogallForestSecondAreaSouthApeHidesAgain(void)
{
	fdScriptStart();
	scMogallForestApeComesOut(ACTOR_SECOND_AREA_SOUTH_APE, SPRITE_COLOR_GRB);
	scMogallForestHopActorTo(ACTOR_SECOND_AREA_SOUTH_APE, 424, 480, 7.6);
	fdScriptWait(2);
	scMogallForestSpawnPuffRing(ACTOR_SECOND_AREA_SOUTH_APE);
	fdActorSetSprColById(ACTOR_SECOND_AREA_SOUTH_APE, SPRITE_COLOR_HIDDEN);
	fdActorSetSprFlags(fdActorGet(ACTOR_SECOND_AREA_SOUTH_APE), 0);
	fdScriptWait(30);
	gmFlagSet(FLAG_MOGALL_FOREST_SECOND_AREA_SOUTH_APE_MOVED);
	fdActorSetPos(ACTOR_SECOND_AREA_SOUTH_APE_STUMP, 424, 480);
	fdScriptEnd();
}

/* Force on the stump on the west bank of the second area's stream, where the
   ape from the island hid, brings it out again. It points the way and hops off
   west, the way to the third area. */
static void scMogallForestSecondAreaSouthApeShowsWay(void)
{
	ACTOR *ape;

	ape = fdActorGet(ACTOR_SECOND_AREA_SOUTH_APE);

	fdScriptStart();

	scMogallForestApeComesOut(ACTOR_SECOND_AREA_SOUTH_APE, SPRITE_COLOR_GRB);
	scMogallForestHopActorTo(ACTOR_SECOND_AREA_SOUTH_APE, 392, 504, 6);
	fdScriptWait(10);

	fdEffectSpawn(ape->x.fixed, ape->y.fixed, ape->z.fixed + toFIXED(4), 0, 0,
		0, EFFECT_SCRIPT_MEDIUM, 0);

	scMogallForestApePointsTheWay(ACTOR_SECOND_AREA_SOUTH_APE);

	scMogallForestHopActorTo(ACTOR_SECOND_AREA_SOUTH_APE, 360, 528, 3);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_SECOND_AREA_SOUTH_APE, 0);
	fdScriptWait(6);

	scMogallForestHopActorTo(ACTOR_SECOND_AREA_SOUTH_APE, 328, 528, 3);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_SECOND_AREA_SOUTH_APE, 0);
	fdScriptWait(6);

	scMogallForestHopActorTo(ACTOR_SECOND_AREA_SOUTH_APE, 288, 528, 3);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_SECOND_AREA_SOUTH_APE, 0);
	fdScriptWait(6);

	scMogallForestHopActorTo(ACTOR_SECOND_AREA_SOUTH_APE, 256, 528, 3);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_SECOND_AREA_SOUTH_APE, 0);
	fdScriptWait(6);

	fdCameraFollowActor(ACTOR_PARTY_LEADER, 1);
	fdActorSetPos(ACTOR_SECOND_AREA_SOUTH_APE, 0, 0);
	fdScriptWait(30);
	gmFlagSet(FLAG_MOGALL_FOREST_SECOND_AREA_SOUTH_APE_OUT);
	fdActorSetPos(ACTOR_SECOND_AREA_SOUTH_APE_STUMP, 0, 0);

	fdScriptEnd();
}

/* Isaac pushes a log in the third area a metatile by walking into it: the log
   by the Elven Shirt's chest, or the logs on the island in the river clearing.
   The two lying north to south on the island, pushed east to its bank, roll
   into the river and lie there as ways across.

   At every push the chest's log and the upper river log copy metatiles onto
   their new column from spare stretches of the map, in rows 49 to 52 and 68 to
   71. A rolled log copies its metatiles in the river from (26, 20). */
static void scMogallForestThirdAreaPushLog(void)
{
	PUSHED_ACTOR_PROBE probe;
	Sint32			   landed;

	fdScriptStart();
	landed = 0;
	if(scPushedActorFindClearPos(&probe) != 0) {
		scPushedActorRoll(probe);
		if(probe.actorSlot == ACTOR_THIRD_AREA_NORTH_RIVER_LOG ||
			probe.actorSlot == ACTOR_THIRD_AREA_SOUTH_RIVER_LOG) {
			if(probe.actorSlot == ACTOR_THIRD_AREA_NORTH_RIVER_LOG) {
				fdMapGridCopyMetatileAttributes(
					38, 68, 1, 4, FIELD_METATILE(probe.pos.x), 68);
				if(FIELD_METATILE(probe.pos.x) ==
					THIRD_AREA_NORTH_RIVER_BANK_COLUMN) {
					fdMapGridCopyMetatileAttributes(
						26, 20, 2, 4, FIELD_METATILE(probe.pos.x), 23);
					fdActorSetSprPriority(ACTOR_THIRD_AREA_NORTH_RIVER_LOG, 1);
					landed = 1;
					gmFlagSet(FLAG_MOGALL_FOREST_THIRD_AREA_NORTH_LOG_ROLLED);
				}
			} else {
				if(FIELD_METATILE(probe.pos.x) ==
					THIRD_AREA_SOUTH_RIVER_BANK_COLUMN) {
					fdMapGridCopyMetatileAttributes(
						26, 20, 2, 4, FIELD_METATILE(probe.pos.x), 32);
					fdActorSetSprPriority(ACTOR_THIRD_AREA_SOUTH_RIVER_LOG, 1);
					landed = 1;
					gmFlagSet(FLAG_MOGALL_FOREST_THIRD_AREA_SOUTH_LOG_ROLLED);
				}
			}
			if(landed == 0) {
				fdScriptEnd();
				return;
			}
			fdActorSetAnimation(probe.actorSlot, MOGALL_FOREST_LOG_ROLLING);
			fdActorSetDestinationOffsetById(probe.actorSlot, 18, 6);
			fdScriptWait(30);
			fdActorSetAnimation(probe.actorSlot, MOGALL_FOREST_LOG_FALLING_IN);
			sySndPlay(SE_LOG_FALL);
			fdActorGet(probe.actorSlot)->priFlg = ACTOR_PRIORITY_UNDERFOOT;
		} else {
			if(probe.actorSlot == ACTOR_THIRD_AREA_LOG)
				fdMapGridCopyMetatileAttributes(
					42, 49, 1, 4, FIELD_METATILE(probe.pos.x), 49);
		}
	}
	fdScriptEnd();
}

/* Walking west off the bank east of the third area's river channel (metatiles
   (43, 32) to (43, 35)) makes Isaac leap onto the lower log once it lies in the
   channel, which dips under him. */
static void scMogallForestThirdAreaJumpWest(void)
{
	ACTOR	 *leader;
	Uint8	  savedMotionFlags;
	FIELD_POS position;

	leader = fdActorGet(ACTOR_PARTY_LEADER);
	savedMotionFlags = leader->motionFlg;

	position.x = fdActorGet(ACTOR_PARTY_LEADER)->x.fixed - toFIXED(32);
	position.y = fdActorGet(ACTOR_PARTY_LEADER)->y.fixed;
	position.z = fdActorGet(ACTOR_PARTY_LEADER)->z.fixed;

	if(scMogallForestTryJumpLeaderTo(&position) != 0) {
		fdScriptStart();

		leader->motionFlg = 0;
		fdActorSetAnimation(
			ACTOR_THIRD_AREA_SOUTH_RIVER_LOG, MOGALL_FOREST_LOG_STEPPED_ON);

		leader->y.fixed -= toFIXED(1);
		leader->groundHeight -= toFIXED(1);
		sySynch(2);

		leader->y.fixed -= toFIXED(1);
		leader->groundHeight -= toFIXED(1);
		sySynch(10);

		leader->y.fixed += toFIXED(1);
		leader->groundHeight += toFIXED(1);
		sySynch(4);

		leader->y.fixed += toFIXED(1);
		leader->groundHeight += toFIXED(1);

		leader->motionFlg = savedMotionFlags;
		fdScriptEnd();
	}
}

/* Move on the rock beside the log by the Elven Shirt's chest pushes it a
   metatile. In the ring among the stumps by the stream (metatile (45, 6)) it
   settles into place and stays, out of the way of the log. */
static void scMogallForestThirdAreaUpdateNorthRock(void)
{
	Sint32 column;
	Sint32 row;

	fdScriptStart();

	column = FIELD_METATILE(fdActorGet(ACTOR_THIRD_AREA_NORTH_ROCK)->x.fixed);
	row = FIELD_METATILE(fdActorGet(ACTOR_THIRD_AREA_NORTH_ROCK)->z.fixed);

	scMogallForestUpdateRockMetatiles(column, row);

	if(column == 45 && row == 6) {
		ACTOR *rock = fdActorGet(ACTOR_THIRD_AREA_NORTH_ROCK);

		rock->motionFlg = 0;
		rock->groundHeight = -toFIXED(2);
		rock->y.fixed = -toFIXED(2);
	}

	fdScriptEnd();
}

/* Move on the rock beside the upper log on the third area's river island pushes
   it a metatile. Pushed south into the ring on row 27, it settles into place
   and stays, and the metatile (43, 23) across the channel is made solid.

   scInit runs it too, to settle the rock again on arrival. */
static void scMogallForestThirdAreaUpdateSouthRock(void)
{
	ACTOR *rock;

	fdScriptStart();
	scMogallForestUpdateRockMetatiles(
		FIELD_METATILE(fdActorGet(ACTOR_THIRD_AREA_SOUTH_ROCK)->x.fixed),
		FIELD_METATILE(fdActorGet(ACTOR_THIRD_AREA_SOUTH_ROCK)->z.fixed));
	if(FIELD_METATILE(fdActorGet(ACTOR_THIRD_AREA_SOUTH_ROCK)->z.fixed) == 27) {
		rock = fdActorGet(ACTOR_THIRD_AREA_SOUTH_ROCK);
		rock->motionFlg = 0;
		rock->groundHeight = -toFIXED(2);
		rock->y.fixed = -toFIXED(2);
		gmFlagSet(FLAG_MOGALL_FOREST_SOUTH_ROCK_SETTLED);
		scMapGridFillCollisionRectangle(
			2, 43, 23, 1, 1, METATILE_BEHAVIOR_WALL);
	}
	fdScriptEnd();
}

/* Force on the east one of the three stumps in a row in the third area brings
   out an ape that cries out in anger, leaps at Isaac and starts a battle with
   it. */
static void scMogallForestThirdAreaApeAttacks(void)
{
	ACTOR *ape;
	Sint32 leaderX;
	ACTOR *leaderForZ;

	ape = fdActorGet(ACTOR_THIRD_AREA_ATTACKING_APE);
	fdScriptStart();
	scMogallForestApeComesOut(
		ACTOR_THIRD_AREA_ATTACKING_APE, SPRITE_COLOR_NORMAL);
	scMogallForestHopActorTo(ACTOR_THIRD_AREA_ATTACKING_APE, 472, 104, 8);
	fdScriptWait(10);
	fdEffectSpawn(ape->x.fixed, ape->y.fixed, (ape->z.fixed + toFIXED(8)), 0, 0,
		0, EFFECT_SCRIPT_MEDIUM, 0);
	fdCameraFollowActor(ACTOR_THIRD_AREA_ATTACKING_APE, 1);
	fdActorTurnToFaceEachOther(
		ACTOR_THIRD_AREA_ATTACKING_APE, ACTOR_PARTY_LEADER, 0);
	fdScriptWait(30);
	fdActorStartRepeatedMotion(ACTOR_THIRD_AREA_ATTACKING_APE, 2);
	fdActorShowEmote(
		ACTOR_THIRD_AREA_ATTACKING_APE, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
	sySndPlay(SE_BEAST_CRY);
	fdScriptWait(60);
	leaderX = fdActorGet(ACTOR_PARTY_LEADER)->x.part[0].integer;
	leaderForZ = fdActorGet(ACTOR_PARTY_LEADER);
	scMogallForestHopActorTo(ACTOR_THIRD_AREA_ATTACKING_APE, leaderX,
		leaderForZ->z.part[0].integer, 6);
	fdScriptWait(10);
	gmFlagSet(FLAG_MOGALL_FOREST_THIRD_AREA_ATTACKING_APE_OUT);
	SET_BATTLE_TYPE(BATTLE_TYPE_SCRIPTED);
	fdRequestBattleFromEncounterTable(
		ENCOUNTER_ZONE_SCRIPTED_MOGALL_ALTIN_LAMAKAN, 0);
	fdScriptEnd();
}

/* Force on the middle one of the three stumps in a row in the third area brings
   a green ape out of it. The ape points the way and hops off south, the way on
   to the river. */
static void scMogallForestThirdAreaMiddleApeShowsWay(void)
{
	ACTOR *ape;

	ape = fdActorGet(ACTOR_THIRD_AREA_MIDDLE_APE);

	fdScriptStart();

	scMogallForestApeComesOut(ACTOR_THIRD_AREA_MIDDLE_APE, SPRITE_COLOR_GRB);
	scMogallForestHopActorTo(ACTOR_THIRD_AREA_MIDDLE_APE, 456, 152, 6);
	fdScriptWait(10);

	fdEffectSpawn(ape->x.fixed, ape->y.fixed, ape->z.fixed + toFIXED(4), 0, 0,
		0, EFFECT_SCRIPT_MEDIUM, 0);

	scMogallForestApePointsTheWay(ACTOR_THIRD_AREA_MIDDLE_APE);

	scMogallForestHopActorTo(ACTOR_THIRD_AREA_MIDDLE_APE, 448, 192, 3);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_THIRD_AREA_MIDDLE_APE, 0);
	fdScriptWait(6);

	scMogallForestHopActorTo(ACTOR_THIRD_AREA_MIDDLE_APE, 424, 208, 3);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_THIRD_AREA_MIDDLE_APE, 0);
	fdScriptWait(6);

	scMogallForestHopActorTo(ACTOR_THIRD_AREA_MIDDLE_APE, 424, 224, 3);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_THIRD_AREA_MIDDLE_APE, 0);
	fdScriptWait(6);

	fdCameraFollowActor(ACTOR_PARTY_LEADER, 1);
	fdActorSetPos(ACTOR_THIRD_AREA_MIDDLE_APE, 0, 0);
	fdScriptWait(30);
	gmFlagSet(FLAG_MOGALL_FOREST_THIRD_AREA_MIDDLE_APE_OUT);
	fdActorSetPos(ACTOR_THIRD_AREA_MIDDLE_APE_STUMP, 0, 0);

	fdScriptEnd();
}

/* Force on the west one of the three stumps in a row in the third area brings a
   green ape out of it. The ape points the way and hops off west, toward the
   Elven Shirt's chest. */
static void scMogallForestThirdAreaWestApeShowsWay(void)
{
	ACTOR *ape;

	ape = fdActorGet(ACTOR_THIRD_AREA_WEST_APE);

	fdScriptStart();

	scMogallForestApeComesOut(ACTOR_THIRD_AREA_WEST_APE, SPRITE_COLOR_GRB);
	scMogallForestHopActorTo(ACTOR_THIRD_AREA_WEST_APE, 392, 104, 6);
	fdScriptWait(10);

	fdEffectSpawn(ape->x.fixed, ape->y.fixed, ape->z.fixed + toFIXED(4), 0, 0,
		0, EFFECT_SCRIPT_MEDIUM, 0);

	scMogallForestApePointsTheWay(ACTOR_THIRD_AREA_WEST_APE);

	scMogallForestHopActorTo(ACTOR_THIRD_AREA_WEST_APE, 376, 152, 6);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_THIRD_AREA_WEST_APE, 0);
	fdScriptWait(10);

	scMogallForestHopActorTo(ACTOR_THIRD_AREA_WEST_APE, 328, 160, 3);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_THIRD_AREA_WEST_APE, 0);
	fdScriptWait(6);

	scMogallForestHopActorTo(ACTOR_THIRD_AREA_WEST_APE, 296, 160, 3);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_THIRD_AREA_WEST_APE, 0);
	fdScriptWait(6);

	fdCameraFollowActor(ACTOR_PARTY_LEADER, 1);
	fdActorSetPos(ACTOR_THIRD_AREA_WEST_APE, 0, 0);
	fdScriptWait(30);
	gmFlagSet(FLAG_MOGALL_FOREST_THIRD_AREA_WEST_APE_OUT);
	fdActorSetPos(ACTOR_THIRD_AREA_WEST_APE_STUMP, 0, 0);

	fdScriptEnd();
}

/* Force on the stump on its own island in the third area's river clearing
   brings a green ape out, which leaps south into a lower stump and hides again,
   the camera following it. */
static void scMogallForestThirdAreaEastApeHidesAgain(void)
{
	fdScriptStart();
	scMogallForestApeComesOut(ACTOR_THIRD_AREA_EAST_APE, SPRITE_COLOR_GRB);
	fdCameraMoveTo(744, CAMERA_KEEP, 504, 1);
	scMogallForestHopActorTo(ACTOR_THIRD_AREA_EAST_APE, 744, 504, 9);
	scMogallForestSpawnPuffRing(ACTOR_THIRD_AREA_EAST_APE);
	fdActorSetSprColById(ACTOR_THIRD_AREA_EAST_APE, SPRITE_COLOR_HIDDEN);
	fdActorSetSprFlags(fdActorGet(ACTOR_THIRD_AREA_EAST_APE), 0);
	fdScriptWait(30);
	gmFlagSet(FLAG_MOGALL_FOREST_THIRD_AREA_EAST_APE_MOVED);
	fdActorSetPos(ACTOR_THIRD_AREA_EAST_APE_STUMP, 744, 504);
	fdScriptEnd();
}

/* Force on the lower stump in the third area's river clearing, where the ape
   from the island hid, brings it out again. It points the way and hops off
   south, the way to the lone stump. */
static void scMogallForestThirdAreaEastApeShowsWay(void)
{
	ACTOR *ape;

	ape = fdActorGet(ACTOR_THIRD_AREA_EAST_APE);

	fdScriptStart();

	scMogallForestApeComesOut(ACTOR_THIRD_AREA_EAST_APE, SPRITE_COLOR_GRB);
	scMogallForestHopActorTo(ACTOR_THIRD_AREA_EAST_APE, 712, 536, 6);
	fdScriptWait(10);

	fdEffectSpawn(ape->x.fixed, ape->y.fixed, ape->z.fixed + toFIXED(4), 0, 0,
		0, EFFECT_SCRIPT_MEDIUM, 0);

	scMogallForestApePointsTheWay(ACTOR_THIRD_AREA_EAST_APE);

	scMogallForestHopActorTo(ACTOR_THIRD_AREA_EAST_APE, 712, 568, 6);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_THIRD_AREA_EAST_APE, 0);
	fdScriptWait(10);

	scMogallForestHopActorTo(ACTOR_THIRD_AREA_EAST_APE, 712, 600, 3);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_THIRD_AREA_EAST_APE, 0);
	fdScriptWait(6);

	scMogallForestHopActorTo(ACTOR_THIRD_AREA_EAST_APE, 736, 640, 3);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_THIRD_AREA_EAST_APE, 0);
	fdScriptWait(6);

	scMogallForestHopActorTo(ACTOR_THIRD_AREA_EAST_APE, 736, 704, 3);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_THIRD_AREA_EAST_APE, 0);
	fdScriptWait(6);

	fdCameraFollowActor(ACTOR_PARTY_LEADER, 1);
	fdActorSetPos(ACTOR_THIRD_AREA_EAST_APE, 0, 0);
	fdScriptWait(30);
	gmFlagSet(FLAG_MOGALL_FOREST_THIRD_AREA_EAST_APE_OUT);

	fdScriptEnd();
}

/* Walking up to the lone stump on the island between the two log bridges, on
   its east side (metatiles (12, 22) and (12, 23)), the ape that led the party
   there bursts out of it and hops away south over the bridge. It happens
   once. */
static void scMogallForestApeLeadsToKillerApe(void)
{
	ACTOR *ape;

	ape = fdActorGet(ACTOR_THIRD_AREA_EAST_APE);
	fdScriptStart();
	fdActorSetPos(ACTOR_THIRD_AREA_EAST_APE, 136, 360);
	scMogallForestApeComesOut(ACTOR_THIRD_AREA_EAST_APE, SPRITE_COLOR_GRB);
	scMogallForestHopActorTo(ACTOR_THIRD_AREA_EAST_APE, 136, 408, 8);
	fdScriptWait(10);
	fdEffectSpawn(ape->x.fixed, ape->y.fixed, (ape->z.fixed + toFIXED(4)), 0, 0,
		0, EFFECT_SCRIPT_MEDIUM, 0);
	fdActorFaceDirection(ACTOR_THIRD_AREA_EAST_APE, FACING_NORTH, 40);
	fdActorSetAttachedEffect(ACTOR_THIRD_AREA_EAST_APE,
		ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorRunRepeatedMotion(ACTOR_THIRD_AREA_EAST_APE, 2);
	fdCameraFollowActor(ACTOR_THIRD_AREA_EAST_APE, 1);
	scMogallForestHopActorTo(ACTOR_THIRD_AREA_EAST_APE, 136, 440, 6);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_THIRD_AREA_EAST_APE, 0);
	fdScriptWait(10);
	scMogallForestHopActorTo(ACTOR_THIRD_AREA_EAST_APE, 136, 472, 3);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_THIRD_AREA_EAST_APE, 0);
	fdScriptWait(6);
	scMogallForestHopActorTo(ACTOR_THIRD_AREA_EAST_APE, 136, 504, 3);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_THIRD_AREA_EAST_APE, 0);
	fdScriptWait(6);
	fdCameraFollowActor(ACTOR_PARTY_LEADER, 1);
	fdActorSetPos(ACTOR_THIRD_AREA_EAST_APE, 0, 0);
	fdScriptWait(60);
	gmFlagSet(FLAG_MOGALL_FOREST_LED_TO_KILLER_APE);
	fdScriptEnd();
}

/* Stepping onto the top of the south bridge below the lone stump (metatile (8,
   27)) until the Killer Ape is beaten: the camera turns to the stump and the
   ground shakes twice with puffs of dust round it. Then the Killer Ape, a big
   blue ape, bursts out, roars, leaps at Isaac and its battle begins. */
static void scMogallForestKillerApeAttacks(void)
{
	VECTOR3 velocity;
	ACTOR  *actor;
	Uint32	directionIndex;
	Sint32	x;
	FIXED	z;

	actor = fdActorGet(ACTOR_THIRD_AREA_EAST_APE);
	fdScriptStart();
	fdActorSetSprColById(ACTOR_THIRD_AREA_EAST_APE, SPRITE_COLOR_HIDDEN);
	fdActorSetSprFlags(fdActorGet(ACTOR_THIRD_AREA_EAST_APE), 0);
	fdActorSetPos(ACTOR_THIRD_AREA_EAST_APE, 136, 360);
	fdCameraSetSpeed(0.5);
	fdCameraMoveTo(136, CAMERA_KEEP, 392, 1);
	fdCameraWaitForArrival();
	fdScriptWait(60);
	fdMapGridSetShake(5, 5, 1);
	scMogallForestSpawnPuffRing(ACTOR_THIRD_AREA_EAST_APE);
	fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	fdMapGridWaitForShakeToSettle();
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
	fdScriptWait(40);
	fdMapGridSetShake(5, 5, 1);
	scMogallForestSpawnPuffRing(ACTOR_THIRD_AREA_EAST_APE);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	fdMapGridWaitForShakeToSettle();
	fdScriptWait(40);
	scMogallForestSpawnPuffRing(ACTOR_THIRD_AREA_EAST_APE);
	actor->scaleX = toFIXED(6) / 5;
	actor->scaleY = toFIXED(6) / 5;
	fdActorSetSprColById(ACTOR_THIRD_AREA_EAST_APE, SPRITE_COLOR_BRG);
	scMogallForestHopActorTo(ACTOR_THIRD_AREA_EAST_APE, 136, 392, 15);
	fdScriptWait(15);
	/* The larger reveal uses a wider, level ring of puffs. */
	fdMapGridSetShake(5, 5, 1);
	for(directionIndex = 0; directionIndex <= TRIG_TURN / FACING_STEP;
		directionIndex++) {
		velocity.x = syCos(directionIndex * FACING_STEP);
		velocity.y = 0;
		z = sySin(directionIndex * FACING_STEP);
		x = velocity.x;
		velocity.z = z;
		x += x / 2;
		velocity.x = x;
		fdEffectSpawn(actor->x.fixed, 0, actor->z.fixed, x, velocity.y, z,
			EFFECT_SCRIPT_MEDIUM, NULL);
	}
	fdScriptWait(30);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	fdMapGridWaitForShakeToSettle();
	sySndPlay(SE_BOSS_ROAR);
	fdActorRunRepeatedMotion(ACTOR_THIRD_AREA_EAST_APE, 2);
	fdScriptWait(20);
	/* It leaps at Isaac, and the battle begins. */
	{
		Sint32 leaderX = fdActorGet(ACTOR_PARTY_LEADER)->x.part[0].integer;

		scMogallForestHopActorTo(ACTOR_THIRD_AREA_EAST_APE, leaderX,
			fdActorGet(ACTOR_PARTY_LEADER)->z.part[0].integer - 16, 8);
	}
	fdScriptWait(10);
	SET_BATTLE_TYPE(BATTLE_TYPE_SCRIPTED);
	fdScriptSetAfterBattleDestination(SCENE_MOGALL_FOREST_KILLER_APE,
		MOGALL_FOREST_KILLER_APE_AFTER_KILLER_APE_ATTACKS);
	fdRequestBattleFromEncounterTable(
		ENCOUNTER_ZONE_SCRIPTED_MOGALL_ALTIN_LAMAKAN, 1);
	fdScriptEnd();
}

const CUE *scGetCues(void)
{
	Sint16 scene = MainRoutineStateWork.next;

	if(scene == SCENE_MOGALL_FOREST_SIGN) {
		return MogallForestSign_Cues;
	}
	if(scene == SCENE_MOGALL_FOREST_QUARTZ) {
		return MogallForestQuartz_Cues;
	}
	if(scene == SCENE_MOGALL_FOREST_KILLER_APE) {
		return MogallForestKillerApe_Cues;
	}
	return MogallForestSidePaths_Cues;
}

/* Sets up the clearing entered. On the first arrival in the forest it plays the
   ape peeking out of the first stump; otherwise it puts back the apes already
   out, the logs in the river and the settled rocks. */
Sint32 scInit(void)
{
	ACTOR *actor;
	Sint32 sceneId;
	Sint32 entrance;

	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	sceneId = MainRoutineStateWork.next;
	if(sceneId == SCENE_MOGALL_FOREST_SIGN) {
		entrance = MainRoutineStateWork.entrance;
		switch(entrance) {
		case MOGALL_FOREST_SIGN_FROM_WORLD_MAP:
		case MOGALL_FOREST_SIGN_FROM_MOGALL_FOREST_SIDE_PATHS_WEST:
		case MOGALL_FOREST_SIGN_FROM_MOGALL_FOREST_SIDE_PATHS_NORTH:
		case MOGALL_FOREST_SIGN_ENTRANCE_FROM_SOUTHWEST_CLEARING:
			if(fdScriptGetFlag(FLAG_MOGALL_FOREST_FIRST_APE_SEEN) == 0) {
				fdScriptStart();
				sySynch(1);
				fdActorSetSprColById(
					ACTOR_FIRST_AREA_NORTH_APE, SPRITE_COLOR_GRB);
				fdActorSetPos(ACTOR_FIRST_AREA_NORTH_APE, 92, 120);
				fdActorFaceDirection(
					ACTOR_FIRST_AREA_NORTH_APE, FACING_NORTH_NORTHEAST, 0);
				fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.4);
				fdActorWalkTo(ACTOR_PARTY_LEADER, 136, 64);
				fdScriptStartScreenTransition();
				fdScriptWaitTransitionFrames();
				fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
				fdScriptWait(30);
				fdActorShowEmote(ACTOR_FIRST_AREA_NORTH_APE,
					EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
				fdActorRunRepeatedMotion(ACTOR_FIRST_AREA_NORTH_APE, 2);
				fdScriptWait(30);
				scMogallForestHopActorTo(
					ACTOR_FIRST_AREA_NORTH_APE, 136, 116, 7);
				scMogallForestSpawnPuffRing(ACTOR_FIRST_AREA_NORTH_APE);
				fdActorSetSprColById(
					ACTOR_FIRST_AREA_NORTH_APE, SPRITE_COLOR_HIDDEN);
				fdActorSetSprFlags(fdActorGet(ACTOR_FIRST_AREA_NORTH_APE), 0);
				gmFlagSet(FLAG_MOGALL_FOREST_FIRST_APE_SEEN);
				fdScriptWait(60);
				fdScriptEnd();
			}
			if(gmFlagGet(FLAG_KEEP_PARTY_POSITION) == 0) {
				break;
			}
			if(gmFlagGet(FLAG_MOGALL_FOREST_FIRST_AREA_NORTH_APE_OUT) != 0) {
				break;
			}
			fdActorSetSprColById(
				ACTOR_FIRST_AREA_NORTH_APE, SPRITE_COLOR_HIDDEN);
			fdActorSetPos(ACTOR_FIRST_AREA_NORTH_APE, 136, 116);
			break;

		case MOGALL_FOREST_SIGN_RIVER_FROM_SOUTHWEST_CLEARING:
		case MOGALL_FOREST_SIGN_FROM_MOGALL_FOREST_SIDE_PATHS_EAST:
		case MOGALL_FOREST_SIGN_FROM_MOGALL_FOREST_QUARTZ:
			actor = fdActorGet(ACTOR_PARTY_LEADER);
			if(actor != NULL) {
				fdActorSetPosFX(ACTOR_NUT, actor->x.fixed, actor->z.fixed);
			}
			fdActorGet(ACTOR_NUT)->update = 0;
			if(gmFlagGet(FLAG_KEEP_PARTY_POSITION) != 0) {
				fdActorGet(ACTOR_NUT)->y.fixed = toFIXED(32);
			}
			sySynch(1);
			fdActorSetPos(ACTOR_NUT, 632, 440);
			if(gmFlagGet(FLAG_MOGALL_FOREST_NUT_CAUGHT) == 0) {
				scFloatNut(ACTOR_NUT);
			}
			fdActorSetSprColById(
				ACTOR_FIRST_AREA_ATTACKING_APE, SPRITE_COLOR_HIDDEN);
			fdActorSetSprColById(
				ACTOR_FIRST_AREA_EAST_APE, SPRITE_COLOR_HIDDEN);
			fdActorSetSprFlags(fdActorGet(ACTOR_FIRST_AREA_ATTACKING_APE), 0);
			fdActorSetSprFlags(fdActorGet(ACTOR_FIRST_AREA_EAST_APE), 0);
			scPushedActorBlockFootprint(ACTOR_FIRST_AREA_LOG);
			if(gmFlagGet(FLAG_MOGALL_FOREST_FIRST_AREA_LOG_ROLLED) == 0) {
				scPushedActorBlockFootprint(ACTOR_FIRST_AREA_RIVER_LOG);
				break;
			}
			sySynch(1);
			fdActorSetPos(ACTOR_FIRST_AREA_RIVER_LOG, 528, 408);
			fdActorSetAnimation(
				ACTOR_FIRST_AREA_RIVER_LOG, MOGALL_FOREST_LOG_IN_RIVER);
			fdMapGridCopyMetatileAttributes(38, 27, 4, 2, 31, 25);
			fdActorGet(ACTOR_FIRST_AREA_RIVER_LOG)->priFlg =
				ACTOR_PRIORITY_UNDERFOOT;
			break;
		}
	} else if(sceneId == SCENE_MOGALL_FOREST_QUARTZ) {
		entrance = MainRoutineStateWork.entrance;
		switch(entrance) {
		case MOGALL_FOREST_QUARTZ_FROM_MOGALL_FOREST_SIDE_PATHS_NORTH:
		case MOGALL_FOREST_QUARTZ_STUMPS_FROM_NORTHWEST_CLEARING:
		case MOGALL_FOREST_QUARTZ_STUMPS_FROM_POND:
		case MOGALL_FOREST_QUARTZ_STUMPS_FROM_NORTHEAST_CLEARING:
			if(gmFlagGet(FLAG_MOGALL_FOREST_SECOND_AREA_WEST_APE_OUT) == 0) {
				fdActorSetSprColById(
					ACTOR_SECOND_AREA_WEST_APE, SPRITE_COLOR_HIDDEN);
				fdActorSetSprFlags(fdActorGet(ACTOR_SECOND_AREA_WEST_APE), 0);
			}
			if(gmFlagGet(FLAG_MOGALL_FOREST_SECOND_AREA_EAST_APE_OUT) != 0) {
				break;
			}
			fdActorSetSprColById(
				ACTOR_SECOND_AREA_EAST_APE, SPRITE_COLOR_HIDDEN);
			fdActorSetSprFlags(fdActorGet(ACTOR_SECOND_AREA_EAST_APE), 0);
			break;

		case MOGALL_FOREST_QUARTZ_RIVER_FROM_NORTHEAST_CLEARING:
		case MOGALL_FOREST_QUARTZ_FROM_MOGALL_FOREST_KILLER_APE:
		case MOGALL_FOREST_QUARTZ_FROM_MOGALL_FOREST_SIDE_PATHS_SOUTH:
			if(gmFlagGet(FLAG_MOGALL_FOREST_SECOND_AREA_LOG_ROLLED) == 0) {
				scPushedActorBlockFootprint(ACTOR_SECOND_AREA_RIVER_LOG);
			} else {
				sySynch(1);
				fdActorSetPos(ACTOR_SECOND_AREA_RIVER_LOG, 552, 510);
				fdActorSetAnimation(
					ACTOR_SECOND_AREA_RIVER_LOG, MOGALL_FOREST_LOG_IN_RIVER);
				fdActorGet(ACTOR_SECOND_AREA_RIVER_LOG)->priFlg =
					ACTOR_PRIORITY_UNDERFOOT;
				fdMapGridCopyMetatileAttributes(44, 30, 2, 4, 34, 30);
#if MOGALL_SECOND_AREA_LOG_PUSH_CELL_CLEARED_ON_RETURN
				scMapGridFillCollisionRectangle(0, 35, 29, 1, 4, 0);
#endif
			}
			scPushedActorBlockFootprint(ACTOR_SECOND_AREA_LOG);
			scPushedActorBlockFootprint(ACTOR_SECOND_AREA_SOUTH_LOG);
			scMapGridFillCollisionRectangle(2,
				FIELD_METATILE(fdActorGet(ACTOR_SECOND_AREA_ROCK)->x.fixed),
				FIELD_METATILE(fdActorGet(ACTOR_SECOND_AREA_ROCK)->z.fixed), 1,
				1, METATILE_BEHAVIOR_WALL);
			sySynch(1);
			fdActorSetSprColById(ACTOR_SECOND_AREA_ROCK, SPRITE_COLOR_GREY);
			fdActorGet(ACTOR_SECOND_AREA_LOG)->collisionFlg |=
				ACTOR_NOT_TARGETED;
			if(gmFlagGet(FLAG_MOGALL_FOREST_SECOND_AREA_SOUTH_APE_OUT) != 0) {
				break;
			}
			fdActorSetSprColById(
				ACTOR_SECOND_AREA_SOUTH_APE, SPRITE_COLOR_HIDDEN);
			fdActorSetSprFlags(fdActorGet(ACTOR_SECOND_AREA_SOUTH_APE), 0);
			if(gmFlagGet(FLAG_MOGALL_FOREST_SECOND_AREA_SOUTH_APE_MOVED) == 0) {
				break;
			}
			fdActorSetPos(ACTOR_SECOND_AREA_SOUTH_APE, 424, 480);
			fdActorSetPos(ACTOR_SECOND_AREA_SOUTH_APE_STUMP, 424, 480);
			break;
		}
	} else if(sceneId == SCENE_MOGALL_FOREST_KILLER_APE) {
		entrance = MainRoutineStateWork.entrance;
		switch(entrance) {
		case MOGALL_FOREST_KILLER_APE_FROM_MOGALL_FOREST_SIDE_PATHS_WEST:
		case MOGALL_FOREST_KILLER_APE_STUMPS_FROM_CHEST:
		case MOGALL_FOREST_KILLER_APE_STUMPS_FROM_NORTHWEST_CLEARING:
		case MOGALL_FOREST_KILLER_APE_STUMPS_FROM_RIVER:
			sySynch(1);
			if(gmFlagGet(FLAG_MOGALL_FOREST_THIRD_AREA_ATTACKING_APE_OUT) ==
				0) {
				fdActorSetSprColById(
					ACTOR_THIRD_AREA_ATTACKING_APE, SPRITE_COLOR_HIDDEN);
				fdActorSetSprFlags(
					fdActorGet(ACTOR_THIRD_AREA_ATTACKING_APE), 0);
				fdActorSetSprFlags(
					fdActorGet(ACTOR_THIRD_AREA_ATTACKING_APE_STUMP), 0);
			}
			if(gmFlagGet(FLAG_MOGALL_FOREST_THIRD_AREA_MIDDLE_APE_OUT) == 0) {
				fdActorSetSprColById(
					ACTOR_THIRD_AREA_MIDDLE_APE, SPRITE_COLOR_HIDDEN);
				fdActorSetSprFlags(fdActorGet(ACTOR_THIRD_AREA_MIDDLE_APE), 0);
				fdActorSetSprFlags(
					fdActorGet(ACTOR_THIRD_AREA_MIDDLE_APE_STUMP), 0);
			}
			if(gmFlagGet(FLAG_MOGALL_FOREST_THIRD_AREA_WEST_APE_OUT) != 0) {
				break;
			}
			fdActorSetSprColById(
				ACTOR_THIRD_AREA_WEST_APE, SPRITE_COLOR_HIDDEN);
			fdActorSetSprFlags(fdActorGet(ACTOR_THIRD_AREA_WEST_APE), 0);
			fdActorSetSprFlags(fdActorGet(ACTOR_THIRD_AREA_WEST_APE_STUMP), 0);
			break;

		case MOGALL_FOREST_KILLER_APE_CHEST_FROM_STUMPS:
			scMapGridFillCollisionRectangle(2,
				FIELD_METATILE(
					fdActorGet(ACTOR_THIRD_AREA_NORTH_ROCK)->x.fixed),
				FIELD_METATILE(
					fdActorGet(ACTOR_THIRD_AREA_NORTH_ROCK)->z.fixed),
				1, 1, METATILE_BEHAVIOR_WALL);
			fdActorSetSprColById(
				ACTOR_THIRD_AREA_NORTH_ROCK, SPRITE_COLOR_GREY);
			sySynch(1);
			fdActorGet(ACTOR_THIRD_AREA_LOG)->collisionFlg |=
				ACTOR_NOT_TARGETED;
			scPushedActorBlockFootprint(ACTOR_THIRD_AREA_LOG);
			break;

		case MOGALL_FOREST_KILLER_APE_RIVER_FROM_STUMPS:
		case MOGALL_FOREST_KILLER_APE_FROM_MOGALL_FOREST_SIDE_PATHS_EAST:
		case MOGALL_FOREST_KILLER_APE_FROM_MOGALL_FOREST_SIDE_PATHS_SOUTH:
		case MOGALL_FOREST_KILLER_APE_RIVER_FROM_BRIDGES:
			/* XXX (no evidence): what these two crates are for; they are
			   hidden, not solid and never a target, so the player neither sees
			   nor meets them. */
			fdActorCreateConfigured(734, 0, 370, SPRITE_CRATE);
			fdActorCreateConfigured(754, 0, 370, SPRITE_CRATE);
			scPushedActorBlockFootprint(ACTOR_THIRD_AREA_WEST_LOG);
			scPushedActorBlockFootprint(ACTOR_THIRD_AREA_SOUTHWEST_LOG);
			if(gmFlagGet(FLAG_MOGALL_FOREST_THIRD_AREA_NORTH_LOG_ROLLED) == 0) {
				scPushedActorBlockFootprint(ACTOR_THIRD_AREA_NORTH_RIVER_LOG);
			} else {
				sySynch(1);
				fdActorSetAnimation(ACTOR_THIRD_AREA_NORTH_RIVER_LOG,
					MOGALL_FOREST_LOG_IN_RIVER);
				fdActorSetPos(ACTOR_THIRD_AREA_NORTH_RIVER_LOG, 698, 398);
				fdActorGet(ACTOR_THIRD_AREA_NORTH_RIVER_LOG)->priFlg |=
					ACTOR_PRIORITY_UNDERFOOT;
				fdMapGridCopyMetatileAttributes(26, 20, 2, 4, 42, 23);
				gmFlagSet(FLAG_MOGALL_FOREST_SOUTH_ROCK_SETTLED);
				fdActorSetPos(ACTOR_THIRD_AREA_SOUTH_ROCK, 632, 440);
				fdActorSetSprFlags(fdActorGet(ACTOR_THIRD_AREA_SOUTH_ROCK), 0);
			}
			if(gmFlagGet(FLAG_MOGALL_FOREST_THIRD_AREA_SOUTH_LOG_ROLLED) == 0) {
				scPushedActorBlockFootprint(ACTOR_THIRD_AREA_SOUTH_RIVER_LOG);
			} else {
				sySynch(1);
				fdActorSetAnimation(ACTOR_THIRD_AREA_SOUTH_RIVER_LOG,
					MOGALL_FOREST_LOG_IN_RIVER);
				fdActorSetPos(ACTOR_THIRD_AREA_SOUTH_RIVER_LOG, 666, 550);
				fdActorGet(ACTOR_THIRD_AREA_SOUTH_RIVER_LOG)->priFlg =
					ACTOR_PRIORITY_UNDERFOOT;
				fdMapGridCopyMetatileAttributes(26, 20, 2, 4, 40, 32);
			}
			scMapGridFillCollisionRectangle(2,
				FIELD_METATILE(
					fdActorGet(ACTOR_THIRD_AREA_SOUTH_ROCK)->x.fixed),
				FIELD_METATILE(
					fdActorGet(ACTOR_THIRD_AREA_SOUTH_ROCK)->z.fixed),
				1, 1, METATILE_BEHAVIOR_WALL);
			fdActorSetSprColById(
				ACTOR_THIRD_AREA_SOUTH_ROCK, SPRITE_COLOR_GREY);
			sySynch(1);
			fdActorGet(ACTOR_THIRD_AREA_NORTH_RIVER_LOG)->collisionFlg |=
				ACTOR_NOT_TARGETED;
			if(gmFlagGet(FLAG_MOGALL_FOREST_THIRD_AREA_EAST_APE_OUT) == 0) {
				fdActorSetSprColById(
					ACTOR_THIRD_AREA_EAST_APE, SPRITE_COLOR_HIDDEN);
				fdActorSetSprFlags(fdActorGet(ACTOR_THIRD_AREA_EAST_APE), 0);
				if(gmFlagGet(FLAG_MOGALL_FOREST_THIRD_AREA_EAST_APE_MOVED) !=
					0) {
					fdActorSetPos(ACTOR_THIRD_AREA_EAST_APE_STUMP, 744, 504);
					fdActorSetPos(ACTOR_THIRD_AREA_EAST_APE, 744, 504);
				}
#if MOGALL_THIRD_AREA_EAST_APE_MOVED_OFF_ON_ENTRY
			} else {
				fdActorSetPos(ACTOR_THIRD_AREA_EAST_APE, 0, 0);
#endif
			}
			scMogallForestThirdAreaUpdateSouthRock();
			break;

		case MOGALL_FOREST_KILLER_APE_BRIDGES_FROM_RIVER:
		case MOGALL_FOREST_KILLER_APE_FROM_WORLD_MAP:
			fdActorSetScript(
				ACTOR_THIRD_AREA_EAST_APE, MogallForest_ActorScript);
			if(gmFlagGet(FLAG_NYUNPA_TAUGHT_FORCE) == 0) {
				break;
			}
			if(gmFlagGet(FLAG_MOGALL_FOREST_KILLER_APE_BEATEN) == 0) {
				break;
			}
			gmFlagSet(FLAG_NYUNPA_ENLIGHTENED);
			break;

		case MOGALL_FOREST_KILLER_APE_AFTER_KILLER_APE_ATTACKS:
			gmFlagSet(FLAG_MOGALL_FOREST_KILLER_APE_BEATEN);
			break;
		}
	}
	return 0;
}

#include "shared/floating_nut.c"

#include "shared/pushed_actor_tables.c"
/* clang-format off */

#include "shared/field_effect_duration_scripts.c"

const Sint32 *FieldEffect_DurationScripts[] = {
	FieldEffect_ShortScript,
	FieldEffect_MediumScript,
	FieldEffect_LongScript,
};

/* The script of each rock moved with Move, which draws it in greys. */
Sint32 MogallForest_RockScript[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_SET(SW_TIMER, SPRITE_COLOR_GREY),
	SC_CALL(scMogallForestSetColFromTimer),
	SC_END,
};

/* The script of each ape, at four fifths of its size, hidden while it hides, */
/* and with its shadow off. */
Sint32 MogallForest_ActorScript[] = {
	SC_SET(SW_SCALE_X, toFIXED(4) / 5),
	SC_SET(SW_SCALE_Y, toFIXED(4) / 5),
	SC_SET(SW_TIMER, SPRITE_COLOR_HIDDEN),
	SC_CALL(scMogallForestSetColFromTimer),
	SC_CALL(scMogallForestClearActorSprFlags),
	SC_END,
};

/* The script of each stump's blank actor, the target of Force, which stays */
/* still with its sprite hidden. */
Sint32 MogallForest_HidingPlaceScript[] = {
	SC_SET(SW_MOTION, 0),
	SC_SET(SW_Y, 0),
	SC_SET(SW_TARGET_Y, ACTOR_NO_TARGET),
	SC_CALL(scMogallForestClearActorSprFlags),
	SC_WAIT(2),
	SC_END,
};

SCENE_ENTRANCE MogallForestSign_Entrances[] = {
	{ MOGALL_FOREST_SIGN_FROM_WORLD_MAP, CONDITION_ALWAYS, 136, 0, 24, FACING_SOUTH, 0, 0, 0, 272, 224, 0 },
	{ MOGALL_FOREST_SIGN_FROM_MOGALL_FOREST_SIDE_PATHS_WEST, CONDITION_ALWAYS, 24, 0, 128, FACING_EAST, 0, 0, 0, 272, 224, 0 },
	{ MOGALL_FOREST_SIGN_FROM_MOGALL_FOREST_SIDE_PATHS_NORTH, CONDITION_ALWAYS, 248, 0, 128, FACING_WEST, 0, 0, 0, 272, 224, 0 },
	{ MOGALL_FOREST_SIGN_ENTRANCE_FROM_SOUTHWEST_CLEARING, CONDITION_ALWAYS, 136, 0, 200, FACING_NORTH, 0, 0, 0, 272, 224, 0 },
	{ MOGALL_FOREST_SIGN_SOUTHWEST_CLEARING_FROM_ENTRANCE, CONDITION_ALWAYS, 72, 0, 264, FACING_SOUTH, 0, 0, 240, 272, 464, 0 },
	{ MOGALL_FOREST_SIGN_SOUTHWEST_CLEARING_FROM_RIVER, CONDITION_ALWAYS, 200, 0, 440, FACING_NORTH, 0, 0, 240, 272, 464, 0 },
	{ MOGALL_FOREST_SIGN_RIVER_FROM_SOUTHWEST_CLEARING, CONDITION_ALWAYS, 416, 0, 264, FACING_SOUTH, 0, 288, 240, 672, 624, 0 },
	{ MOGALL_FOREST_SIGN_FROM_MOGALL_FOREST_SIDE_PATHS_EAST, CONDITION_ALWAYS, 560, 0, 264, FACING_SOUTH, 0, 288, 240, 672, 624, 0 },
	{ MOGALL_FOREST_SIGN_FROM_MOGALL_FOREST_QUARTZ, CONDITION_ALWAYS, 648, 0, 344, FACING_WEST, 0, 288, 240, 672, 624, 0 },
	{ SCENE_TABLE_END },
};

SCENE_ENTRANCE MogallForestQuartz_Entrances[] = {
	{ ENTRANCE_DEFAULT, CONDITION_ALWAYS, 120, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ MOGALL_FOREST_QUARTZ_FROM_MOGALL_FOREST_SIGN, CONDITION_ALWAYS, 24, 0, 128, FACING_EAST, 0, 0, 0, 272, 224, 0 },
	{ MOGALL_FOREST_QUARTZ_NORTHWEST_CLEARING_FROM_STUMPS, CONDITION_ALWAYS, 248, 0, 128, FACING_WEST, 0, 0, 0, 272, 224, 0 },
	{ MOGALL_FOREST_QUARTZ_FROM_MOGALL_FOREST_SIDE_PATHS_NORTH, CONDITION_ALWAYS, 424, 0, 24, FACING_SOUTH, 0, 288, 0, 560, 224, 0 },
	{ MOGALL_FOREST_QUARTZ_STUMPS_FROM_NORTHWEST_CLEARING, CONDITION_ALWAYS, 312, 0, 136, FACING_EAST, 0, 288, 0, 560, 224, 0 },
	{ MOGALL_FOREST_QUARTZ_STUMPS_FROM_POND, CONDITION_ALWAYS, 536, 0, 120, FACING_WEST, 0, 288, 0, 560, 224, 0 },
	{ MOGALL_FOREST_QUARTZ_STUMPS_FROM_NORTHEAST_CLEARING, CONDITION_ALWAYS, 424, 0, 200, FACING_NORTH, 0, 288, 0, 560, 224, 0 },
	{ MOGALL_FOREST_QUARTZ_NORTHEAST_CLEARING_FROM_STUMPS, CONDITION_ALWAYS, 744, 0, 24, FACING_SOUTH, 0, 576, 0, 848, 224, 0 },
	{ MOGALL_FOREST_QUARTZ_NORTHEAST_CLEARING_FROM_RIVER, CONDITION_ALWAYS, 744, 0, 200, FACING_NORTH, 0, 576, 0, 848, 224, 0 },
	{ MOGALL_FOREST_QUARTZ_POND_FROM_STUMPS, CONDITION_ALWAYS, 24, 0, 368, FACING_EAST, 0, 0, 240, 272, 464, 0 },
	{ MOGALL_FOREST_QUARTZ_RIVER_FROM_NORTHEAST_CLEARING, CONDITION_ALWAYS, 488, 0, 264, FACING_SOUTH, 0, 288, 240, 736, 624, 0 },
	{ MOGALL_FOREST_QUARTZ_FROM_MOGALL_FOREST_KILLER_APE, CONDITION_ALWAYS, 312, 0, 528, FACING_EAST, 0, 288, 240, 736, 624, 0 },
	{ MOGALL_FOREST_QUARTZ_FROM_MOGALL_FOREST_SIDE_PATHS_SOUTH, CONDITION_ALWAYS, 360, 0, 600, FACING_NORTH, 0, 288, 240, 736, 624, 0 },
	{ SCENE_TABLE_END },
};

SCENE_ENTRANCE MogallForestKillerApe_Entrances[] = {
	{ ENTRANCE_DEFAULT, CONDITION_ALWAYS, 172, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ MOGALL_FOREST_KILLER_APE_NORTHWEST_CLEARING_FROM_STUMPS, CONDITION_ALWAYS, 24, 0, 128, FACING_EAST, 0, 0, 0, 272, 224, 0 },
	{ MOGALL_FOREST_KILLER_APE_FROM_MOGALL_FOREST_QUARTZ, CONDITION_ALWAYS, 248, 0, 128, FACING_WEST, 0, 0, 0, 272, 224, 0 },
	{ MOGALL_FOREST_KILLER_APE_FROM_MOGALL_FOREST_SIDE_PATHS_WEST, CONDITION_ALWAYS, 424, 0, 24, FACING_SOUTH, 0, 288, 0, 560, 224, 0 },
	{ MOGALL_FOREST_KILLER_APE_STUMPS_FROM_CHEST, CONDITION_ALWAYS, 312, 0, 160, FACING_EAST, 0, 288, 0, 560, 224, 0 },
	{ MOGALL_FOREST_KILLER_APE_STUMPS_FROM_NORTHWEST_CLEARING, CONDITION_ALWAYS, 536, 0, 160, FACING_WEST, 0, 288, 0, 560, 224, 0 },
	{ MOGALL_FOREST_KILLER_APE_STUMPS_FROM_RIVER, CONDITION_ALWAYS, 424, 0, 200, FACING_NORTH, 0, 288, 0, 560, 224, 0 },
	{ MOGALL_FOREST_KILLER_APE_CHEST_FROM_STUMPS, CONDITION_ALWAYS, 824, 0, 136, FACING_WEST, 0, 576, 0, 848, 224, 0 },
	{ MOGALL_FOREST_KILLER_APE_RIVER_FROM_STUMPS, CONDITION_ALWAYS, 728, 0, 264, FACING_SOUTH, 0, 400, 240, 848, 640, 0 },
	{ MOGALL_FOREST_KILLER_APE_FROM_MOGALL_FOREST_SIDE_PATHS_EAST, CONDITION_ALWAYS, 824, 0, 512, FACING_WEST, 0, 400, 240, 848, 640, 0 },
	{ MOGALL_FOREST_KILLER_APE_FROM_MOGALL_FOREST_SIDE_PATHS_SOUTH, CONDITION_ALWAYS, 824, 0, 608, FACING_WEST, 0, 400, 240, 848, 640, 0 },
	{ MOGALL_FOREST_KILLER_APE_RIVER_FROM_BRIDGES, CONDITION_ALWAYS, 736, 0, 616, FACING_NORTH, 0, 400, 240, 848, 640, 0 },
	{ MOGALL_FOREST_KILLER_APE_BRIDGES_FROM_RIVER, CONDITION_ALWAYS, 264, 0, 264, FACING_SOUTH, 0, 0, 240, 384, 496, 0 },
	{ MOGALL_FOREST_KILLER_APE_FROM_WORLD_MAP, CONDITION_ALWAYS, 136, 0, 472, FACING_NORTH, 0, 0, 240, 384, 496, 0 },
	{ MOGALL_FOREST_KILLER_APE_AFTER_KILLER_APE_ATTACKS, CONDITION_ALWAYS, 136, 0, 432, FACING_NORTH, 0, 0, 240, 384, 496, 0 },
	{ SCENE_TABLE_END },
};

SCENE_ENTRANCE MogallForestSidePaths_Entrances[] = {
	{ MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_SIGN_NORTH, CONDITION_ALWAYS, 312, 0, 128, FACING_EAST, 0, 288, 0, 560, 224, 0 },
	{ MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_SIGN_WEST, CONDITION_ALWAYS, 248, 0, 368, FACING_WEST, 0, 0, 240, 272, 464, 0 },
	{ MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_QUARTZ_NORTH, CONDITION_ALWAYS, 424, 0, 440, FACING_NORTH, 0, 288, 240, 560, 464, 0 },
	{ MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_QUARTZ_SOUTH, CONDITION_ALWAYS, 712, 0, 264, FACING_SOUTH, 0, 576, 240, 848, 464, 0 },
	{ MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_KILLER_APE_WEST, CONDITION_ALWAYS, 136, 0, 680, FACING_NORTH, 0, 0, 480, 272, 704, 0 },
	{ MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_KILLER_APE_EAST, CONDITION_ALWAYS, 312, 0, 552, FACING_EAST, 0, 288, 480, 560, 704, 0 },
	{ MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_KILLER_APE_SOUTH, CONDITION_ALWAYS, 312, 0, 648, FACING_EAST, 0, 288, 480, 560, 704, 0 },
	{ MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_SIGN_EAST, CONDITION_ALWAYS, 648, 0, 680, FACING_NORTH, 0, 576, 480, 848, 704, 0 },
	{ MOGALL_FOREST_SIDE_PATHS_NORTH_CLEARING_EAST, CONDITION_ALWAYS, 536, 0, 128, FACING_WEST, 0, 288, 0, 560, 224, 0 },
	{ MOGALL_FOREST_SIDE_PATHS_WEST_CLEARING_WEST, CONDITION_ALWAYS, 24, 0, 368, FACING_EAST, 0, 0, 240, 272, 464, 0 },
	{ SCENE_TABLE_END },
};

Uint32 MogallForest_Exits[] = {
	SCENE_EXITS(MogallForestSign),
	SCENE_EXIT(MOGALL_FOREST_SIGN_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_MOGALL_FOREST_SIGN),
	SCENE_EXIT(MOGALL_FOREST_SIGN_TO_MOGALL_FOREST_SIDE_PATHS_WEST, MogallForestSidePaths, MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_SIGN_WEST),
	SCENE_EXIT(MOGALL_FOREST_SIGN_TO_MOGALL_FOREST_SIDE_PATHS_NORTH, MogallForestSidePaths, MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_SIGN_NORTH),
	SCENE_EXIT(MOGALL_FOREST_SIGN_ENTRANCE_TO_SOUTHWEST_CLEARING, MogallForestSign, MOGALL_FOREST_SIGN_SOUTHWEST_CLEARING_FROM_ENTRANCE),
	SCENE_EXIT(MOGALL_FOREST_SIGN_SOUTHWEST_CLEARING_TO_ENTRANCE, MogallForestSign, MOGALL_FOREST_SIGN_ENTRANCE_FROM_SOUTHWEST_CLEARING),
	SCENE_EXIT(MOGALL_FOREST_SIGN_SOUTHWEST_CLEARING_TO_RIVER, MogallForestSign, MOGALL_FOREST_SIGN_RIVER_FROM_SOUTHWEST_CLEARING),
	SCENE_EXIT(MOGALL_FOREST_SIGN_RIVER_TO_SOUTHWEST_CLEARING, MogallForestSign, MOGALL_FOREST_SIGN_SOUTHWEST_CLEARING_FROM_RIVER),
	SCENE_EXIT(MOGALL_FOREST_SIGN_TO_MOGALL_FOREST_SIDE_PATHS_EAST, MogallForestSidePaths, MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_SIGN_EAST),
	SCENE_EXIT(MOGALL_FOREST_SIGN_TO_MOGALL_FOREST_QUARTZ, MogallForestQuartz, MOGALL_FOREST_QUARTZ_FROM_MOGALL_FOREST_SIGN),
	SCENE_EXITS(MogallForestQuartz),
	SCENE_EXIT(MOGALL_FOREST_QUARTZ_TO_MOGALL_FOREST_SIGN, MogallForestSign, MOGALL_FOREST_SIGN_FROM_MOGALL_FOREST_QUARTZ),
	SCENE_EXIT(MOGALL_FOREST_QUARTZ_NORTHWEST_CLEARING_TO_STUMPS, MogallForestQuartz, MOGALL_FOREST_QUARTZ_STUMPS_FROM_NORTHWEST_CLEARING),
	SCENE_EXIT(MOGALL_FOREST_QUARTZ_TO_MOGALL_FOREST_SIDE_PATHS_NORTH, MogallForestSidePaths, MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_QUARTZ_NORTH),
	SCENE_EXIT(MOGALL_FOREST_QUARTZ_STUMPS_TO_NORTHWEST_CLEARING, MogallForestQuartz, MOGALL_FOREST_QUARTZ_NORTHWEST_CLEARING_FROM_STUMPS),
	SCENE_EXIT(MOGALL_FOREST_QUARTZ_STUMPS_TO_POND, MogallForestQuartz, MOGALL_FOREST_QUARTZ_POND_FROM_STUMPS),
	SCENE_EXIT(MOGALL_FOREST_QUARTZ_STUMPS_TO_NORTHEAST_CLEARING, MogallForestQuartz, MOGALL_FOREST_QUARTZ_NORTHEAST_CLEARING_FROM_STUMPS),
	SCENE_EXIT(MOGALL_FOREST_QUARTZ_NORTHEAST_CLEARING_TO_STUMPS, MogallForestQuartz, MOGALL_FOREST_QUARTZ_STUMPS_FROM_NORTHEAST_CLEARING),
	SCENE_EXIT(MOGALL_FOREST_QUARTZ_NORTHEAST_CLEARING_TO_RIVER, MogallForestQuartz, MOGALL_FOREST_QUARTZ_RIVER_FROM_NORTHEAST_CLEARING),
	SCENE_EXIT(MOGALL_FOREST_QUARTZ_POND_TO_STUMPS, MogallForestQuartz, MOGALL_FOREST_QUARTZ_STUMPS_FROM_POND),
	SCENE_EXIT(MOGALL_FOREST_QUARTZ_RIVER_TO_NORTHEAST_CLEARING, MogallForestQuartz, MOGALL_FOREST_QUARTZ_NORTHEAST_CLEARING_FROM_RIVER),
	SCENE_EXIT(MOGALL_FOREST_QUARTZ_TO_MOGALL_FOREST_KILLER_APE, MogallForestKillerApe, MOGALL_FOREST_KILLER_APE_FROM_MOGALL_FOREST_QUARTZ),
	SCENE_EXIT(MOGALL_FOREST_QUARTZ_TO_MOGALL_FOREST_SIDE_PATHS_SOUTH, MogallForestSidePaths, MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_QUARTZ_SOUTH),
	SCENE_EXITS(MogallForestKillerApe),
	SCENE_EXIT(MOGALL_FOREST_KILLER_APE_NORTHWEST_CLEARING_TO_STUMPS, MogallForestKillerApe, MOGALL_FOREST_KILLER_APE_STUMPS_FROM_NORTHWEST_CLEARING),
	SCENE_EXIT(MOGALL_FOREST_KILLER_APE_TO_MOGALL_FOREST_QUARTZ, MogallForestQuartz, MOGALL_FOREST_QUARTZ_FROM_MOGALL_FOREST_KILLER_APE),
	SCENE_EXIT(MOGALL_FOREST_KILLER_APE_TO_MOGALL_FOREST_SIDE_PATHS_WEST, MogallForestSidePaths, MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_KILLER_APE_WEST),
	SCENE_EXIT(MOGALL_FOREST_KILLER_APE_STUMPS_TO_CHEST, MogallForestKillerApe, MOGALL_FOREST_KILLER_APE_CHEST_FROM_STUMPS),
	SCENE_EXIT(MOGALL_FOREST_KILLER_APE_STUMPS_TO_NORTHWEST_CLEARING, MogallForestKillerApe, MOGALL_FOREST_KILLER_APE_NORTHWEST_CLEARING_FROM_STUMPS),
	SCENE_EXIT(MOGALL_FOREST_KILLER_APE_STUMPS_TO_RIVER, MogallForestKillerApe, MOGALL_FOREST_KILLER_APE_RIVER_FROM_STUMPS),
	SCENE_EXIT(MOGALL_FOREST_KILLER_APE_CHEST_TO_STUMPS, MogallForestKillerApe, MOGALL_FOREST_KILLER_APE_STUMPS_FROM_CHEST),
	SCENE_EXIT(MOGALL_FOREST_KILLER_APE_RIVER_TO_STUMPS, MogallForestKillerApe, MOGALL_FOREST_KILLER_APE_STUMPS_FROM_RIVER),
	SCENE_EXIT(MOGALL_FOREST_KILLER_APE_TO_MOGALL_FOREST_SIDE_PATHS_EAST, MogallForestSidePaths, MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_KILLER_APE_EAST),
	SCENE_EXIT(MOGALL_FOREST_KILLER_APE_TO_MOGALL_FOREST_SIDE_PATHS_SOUTH, MogallForestSidePaths, MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_KILLER_APE_SOUTH),
	SCENE_EXIT(MOGALL_FOREST_KILLER_APE_RIVER_TO_BRIDGES, MogallForestKillerApe, MOGALL_FOREST_KILLER_APE_BRIDGES_FROM_RIVER),
	SCENE_EXIT(MOGALL_FOREST_KILLER_APE_BRIDGES_TO_RIVER, MogallForestKillerApe, MOGALL_FOREST_KILLER_APE_RIVER_FROM_BRIDGES),
	SCENE_EXIT(MOGALL_FOREST_KILLER_APE_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_MOGALL_FOREST_KILLER_APE),
	SCENE_EXITS(MogallForestSidePaths),
	SCENE_EXIT(MOGALL_FOREST_SIDE_PATHS_TO_MOGALL_FOREST_SIGN_NORTH, MogallForestSign, MOGALL_FOREST_SIGN_FROM_MOGALL_FOREST_SIDE_PATHS_NORTH),
	SCENE_EXIT(MOGALL_FOREST_SIDE_PATHS_TO_MOGALL_FOREST_SIGN_WEST, MogallForestSign, MOGALL_FOREST_SIGN_FROM_MOGALL_FOREST_SIDE_PATHS_WEST),
	SCENE_EXIT(MOGALL_FOREST_SIDE_PATHS_TO_MOGALL_FOREST_QUARTZ_NORTH, MogallForestQuartz, MOGALL_FOREST_QUARTZ_FROM_MOGALL_FOREST_SIDE_PATHS_NORTH),
	SCENE_EXIT(MOGALL_FOREST_SIDE_PATHS_TO_MOGALL_FOREST_QUARTZ_SOUTH, MogallForestQuartz, MOGALL_FOREST_QUARTZ_FROM_MOGALL_FOREST_SIDE_PATHS_SOUTH),
	SCENE_EXIT(MOGALL_FOREST_SIDE_PATHS_TO_MOGALL_FOREST_KILLER_APE_WEST, MogallForestKillerApe, MOGALL_FOREST_KILLER_APE_FROM_MOGALL_FOREST_SIDE_PATHS_WEST),
	SCENE_EXIT(MOGALL_FOREST_SIDE_PATHS_TO_MOGALL_FOREST_KILLER_APE_EAST, MogallForestKillerApe, MOGALL_FOREST_KILLER_APE_FROM_MOGALL_FOREST_SIDE_PATHS_EAST),
	SCENE_EXIT(MOGALL_FOREST_SIDE_PATHS_TO_MOGALL_FOREST_KILLER_APE_SOUTH, MogallForestKillerApe, MOGALL_FOREST_KILLER_APE_FROM_MOGALL_FOREST_SIDE_PATHS_SOUTH),
	SCENE_EXIT(MOGALL_FOREST_SIDE_PATHS_TO_MOGALL_FOREST_SIGN_EAST, MogallForestSign, MOGALL_FOREST_SIGN_FROM_MOGALL_FOREST_SIDE_PATHS_EAST),
	SCENE_EXIT(MOGALL_FOREST_SIDE_PATHS_TO_NORTH_CLEARING_WEST, MogallForestSidePaths, MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_SIGN_NORTH),
	SCENE_EXIT(MOGALL_FOREST_SIDE_PATHS_TO_WEST_CLEARING_EAST, MogallForestSidePaths, MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_SIGN_WEST),
	SCENE_EXIT(MOGALL_FOREST_SIDE_PATHS_TO_CENTER_CLEARING, MogallForestSidePaths, MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_QUARTZ_NORTH),
	SCENE_EXIT(MOGALL_FOREST_SIDE_PATHS_TO_EAST_CLEARING, MogallForestSidePaths, MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_QUARTZ_SOUTH),
	SCENE_EXIT(MOGALL_FOREST_SIDE_PATHS_TO_SOUTHWEST_CLEARING, MogallForestSidePaths, MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_KILLER_APE_WEST),
	SCENE_EXIT(MOGALL_FOREST_SIDE_PATHS_TO_SOUTH_CLEARING_NORTHWEST, MogallForestSidePaths, MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_KILLER_APE_EAST),
	SCENE_EXIT(MOGALL_FOREST_SIDE_PATHS_TO_SOUTH_CLEARING_SOUTHWEST, MogallForestSidePaths, MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_KILLER_APE_SOUTH),
	SCENE_EXIT(MOGALL_FOREST_SIDE_PATHS_TO_SOUTHEAST_CLEARING, MogallForestSidePaths, MOGALL_FOREST_SIDE_PATHS_FROM_MOGALL_FOREST_SIGN_EAST),
	SCENE_EXIT(MOGALL_FOREST_SIDE_PATHS_TO_NORTH_CLEARING_EAST, MogallForestSidePaths, MOGALL_FOREST_SIDE_PATHS_NORTH_CLEARING_EAST),
	SCENE_EXIT(MOGALL_FOREST_SIDE_PATHS_TO_WEST_CLEARING_WEST, MogallForestSidePaths, MOGALL_FOREST_SIDE_PATHS_WEST_CLEARING_WEST),
	SCENE_EXITS_END,
};

ACTOR_PLACE MogallForestSign_ActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7,
		488, 0, 464, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, 7,
		528, 0, 504, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_APE, FLAG_MOGALL_FOREST_FIRST_AREA_NORTH_APE_OUT, (Sint32)MogallForest_ActorScript,
		136, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_APE, FLAG_MOGALL_FOREST_FIRST_AREA_ATTACKING_APE_OUT, (Sint32)MogallForest_ActorScript,
		376, 0, 432, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_APE, FLAG_MOGALL_FOREST_FIRST_AREA_EAST_APE_OUT, (Sint32)MogallForest_ActorScript,
		504, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BLANK, FLAG_MOGALL_FOREST_FIRST_AREA_NORTH_APE_OUT, (Sint32)MogallForest_HidingPlaceScript,
		136, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BLANK, FLAG_MOGALL_FOREST_FIRST_AREA_ATTACKING_APE_OUT, (Sint32)MogallForest_HidingPlaceScript,
		376, 0, 432, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_BLANK, FLAG_MOGALL_FOREST_FIRST_AREA_EAST_APE_OUT, (Sint32)MogallForest_HidingPlaceScript,
		504, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_BLANK, FLAG_MOGALL_FOREST_NUT_CAUGHT, 7,
		632, 0, 440, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE MogallForestQuartz_ActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, 7,
		592, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, 7,
		592, 0, 456, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7,
		648, 0, 512, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_TURNING_ROCK, CONDITION_ALWAYS, (Sint32)MogallForest_RockScript,
		584, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_APE, FLAG_MOGALL_FOREST_SECOND_AREA_WEST_APE_OUT, (Sint32)MogallForest_ActorScript,
		360, 0, 80, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_APE, FLAG_MOGALL_FOREST_SECOND_AREA_EAST_APE_OUT, (Sint32)MogallForest_ActorScript,
		488, 0, 80, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_APE, FLAG_MOGALL_FOREST_SECOND_AREA_SOUTH_APE_OUT, (Sint32)MogallForest_ActorScript,
		504, 0, 480, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BLANK, FLAG_MOGALL_FOREST_SECOND_AREA_WEST_APE_OUT, (Sint32)MogallForest_HidingPlaceScript,
		360, 0, 80, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BLANK, FLAG_MOGALL_FOREST_SECOND_AREA_EAST_APE_OUT, (Sint32)MogallForest_HidingPlaceScript,
		488, 0, 80, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_BLANK, FLAG_MOGALL_FOREST_SECOND_AREA_SOUTH_APE_OUT, (Sint32)MogallForest_HidingPlaceScript,
		504, 0, 480, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_VENUS_DJINNI, FLAG_DJINNI(DJINNI_QUARTZ), MOVEMENT_TYPE_STAND,
		488, 0, 568, FACING_EAST, TALK_FACING_PARTY, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE MogallForestKillerApe_ActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7,
		680, 0, 96, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7,
		616, 0, 400, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7,
		552, 0, 480, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7,
		600, 0, 544, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, 7,
		544, 0, 536, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_TURNING_ROCK, CONDITION_ALWAYS, (Sint32)MogallForest_RockScript,
		696, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_TURNING_ROCK, CONDITION_ALWAYS, (Sint32)MogallForest_RockScript,
		632, 0, 408, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_APE, FLAG_MOGALL_FOREST_THIRD_AREA_ATTACKING_APE_OUT, (Sint32)MogallForest_ActorScript,
		488, 0, 88, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_APE, FLAG_MOGALL_FOREST_THIRD_AREA_MIDDLE_APE_OUT, (Sint32)MogallForest_ActorScript,
		424, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_APE, FLAG_MOGALL_FOREST_THIRD_AREA_WEST_APE_OUT, (Sint32)MogallForest_ActorScript,
		360, 0, 80, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_APE, MOGALL_FOREST_EAST_ACTOR_CONDITION, (Sint32)MogallForest_ActorScript,
		744, 0, 400, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BLANK, FLAG_MOGALL_FOREST_THIRD_AREA_ATTACKING_APE_OUT, (Sint32)MogallForest_HidingPlaceScript,
		488, 0, 88, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BLANK, FLAG_MOGALL_FOREST_THIRD_AREA_MIDDLE_APE_OUT, (Sint32)MogallForest_HidingPlaceScript,
		424, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_BLANK, FLAG_MOGALL_FOREST_THIRD_AREA_WEST_APE_OUT, (Sint32)MogallForest_HidingPlaceScript,
		360, 0, 80, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_BLANK, MOGALL_FOREST_EAST_SHARED_SPRITE_CONDITION, (Sint32)MogallForest_HidingPlaceScript,
		744, 0, 400, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE MogallForestSidePaths_ActorPlacements[] = {
	{ SCENE_TABLE_END },
};

CUE MogallForestSign_Cues[] = {
	EXIT_METATILE(MOGALL_FOREST_SIGN_TO_WORLD_MAP),
	EXIT_METATILE(MOGALL_FOREST_SIGN_TO_MOGALL_FOREST_SIDE_PATHS_WEST),
	EXIT_METATILE(MOGALL_FOREST_SIGN_TO_MOGALL_FOREST_SIDE_PATHS_NORTH),
	EXIT_METATILE(MOGALL_FOREST_SIGN_ENTRANCE_TO_SOUTHWEST_CLEARING),
	EXIT_METATILE(MOGALL_FOREST_SIGN_SOUTHWEST_CLEARING_TO_ENTRANCE),
	EXIT_METATILE(MOGALL_FOREST_SIGN_SOUTHWEST_CLEARING_TO_RIVER),
	EXIT_METATILE(MOGALL_FOREST_SIGN_RIVER_TO_SOUTHWEST_CLEARING),
	EXIT_METATILE(MOGALL_FOREST_SIGN_TO_MOGALL_FOREST_SIDE_PATHS_EAST),
	EXIT_METATILE(MOGALL_FOREST_SIGN_TO_MOGALL_FOREST_QUARTZ),
	{ STEP_CUE_WALKING, METATILE_BEHAVIOR_MOGALL_FOREST_LOG, CONDITION_ALWAYS, SCRIPT_REF(scMogallForestFirstAreaPushLog) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_MOGALL_FOREST_JUMP, CONDITION_ALWAYS, SCRIPT_REF(scMogallForestFirstAreaJumpNorth) },
	{ STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_BEHAVIOR_MOGALL_FOREST_JUMP_SOUTH, CONDITION_ALWAYS, SCRIPT_REF(scMogallForestFirstAreaJumpSouth) },
	{ EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_MOGALL_FOREST_BROKEN_SIGN, CONDITION_ALWAYS, SCRIPT_REF(scMogallForestReadBrokenSign) },
	{ PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_FIRST_AREA_NORTH_APE_STUMP, FLAG_MOGALL_FOREST_FIRST_AREA_NORTH_APE_OUT, SCRIPT_REF(scMogallForestFirstAreaNorthApeShowsWay) },
	{ PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_FIRST_AREA_ATTACKING_APE_STUMP, FLAG_MOGALL_FOREST_FIRST_AREA_ATTACKING_APE_OUT, SCRIPT_REF(scMogallForestFirstAreaApeAttacks) },
	{ PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_FIRST_AREA_EAST_APE_STUMP, FLAG_MOGALL_FOREST_FIRST_AREA_EAST_APE_OUT, SCRIPT_REF(scMogallForestFirstAreaEastApeShowsWay) },
	{ PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_NUT, FLAG_MOGALL_FOREST_NUT_CAUGHT, SCRIPT_REF(scMogallForestCatchNut) },
	{ CUE_TABLE_END },
};

CUE MogallForestQuartz_Cues[] = {
	EXIT_METATILE(MOGALL_FOREST_QUARTZ_TO_MOGALL_FOREST_SIGN),
	EXIT_METATILE(MOGALL_FOREST_QUARTZ_NORTHWEST_CLEARING_TO_STUMPS),
	EXIT_METATILE(MOGALL_FOREST_QUARTZ_TO_MOGALL_FOREST_SIDE_PATHS_NORTH),
	EXIT_METATILE(MOGALL_FOREST_QUARTZ_STUMPS_TO_NORTHWEST_CLEARING),
	EXIT_METATILE(MOGALL_FOREST_QUARTZ_STUMPS_TO_POND),
	EXIT_METATILE(MOGALL_FOREST_QUARTZ_STUMPS_TO_NORTHEAST_CLEARING),
	EXIT_METATILE(MOGALL_FOREST_QUARTZ_NORTHEAST_CLEARING_TO_STUMPS),
	EXIT_METATILE(MOGALL_FOREST_QUARTZ_NORTHEAST_CLEARING_TO_RIVER),
	EXIT_METATILE(MOGALL_FOREST_QUARTZ_POND_TO_STUMPS),
	EXIT_METATILE(MOGALL_FOREST_QUARTZ_RIVER_TO_NORTHEAST_CLEARING),
	EXIT_METATILE(MOGALL_FOREST_QUARTZ_TO_MOGALL_FOREST_KILLER_APE),
	EXIT_METATILE(MOGALL_FOREST_QUARTZ_TO_MOGALL_FOREST_SIDE_PATHS_SOUTH),
	{ STEP_CUE_WALKING, METATILE_BEHAVIOR_MOGALL_FOREST_LOG, CONDITION_ALWAYS, SCRIPT_REF(scMogallForestSecondAreaPushLog) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SECOND_AREA_ROCK, CONDITION_ALWAYS, SCRIPT_REF(scMogallForestSecondAreaUpdateRock) },
	{ PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_SECOND_AREA_WEST_APE_STUMP, FLAG_MOGALL_FOREST_SECOND_AREA_WEST_APE_OUT, SCRIPT_REF(scMogallForestSecondAreaWestApeShowsWay) },
	{ PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_SECOND_AREA_EAST_APE_STUMP, FLAG_MOGALL_FOREST_SECOND_AREA_EAST_APE_OUT, SCRIPT_REF(scMogallForestSecondAreaEastApeShowsWay) },
	{ PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_SECOND_AREA_SOUTH_APE_STUMP, FLAG_MOGALL_FOREST_SECOND_AREA_SOUTH_APE_MOVED, SCRIPT_REF(scMogallForestSecondAreaSouthApeHidesAgain) },
	{ PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_SECOND_AREA_SOUTH_APE_STUMP, FLAG_MOGALL_FOREST_SECOND_AREA_SOUTH_APE_OUT, SCRIPT_REF(scMogallForestSecondAreaSouthApeShowsWay) },
	{ EXAMINE_CUE(EXAMINE_CHEST), 100, FLAG_MOGALL_FOREST_APPLE_CHEST, FIND_ITEM(ITEM_APPLE) },
	{ CUE_TALK, ACTOR_SECOND_AREA_QUARTZ, CONDITION_ALWAYS, SCRIPT_REF(scMogallForestStartDjinnBattle) },
	{ CUE_TABLE_END },
};
/* clang-format on */

CUE MogallForestKillerApe_Cues[] = {
	EXIT_METATILE(MOGALL_FOREST_KILLER_APE_NORTHWEST_CLEARING_TO_STUMPS),
	EXIT_METATILE(MOGALL_FOREST_KILLER_APE_TO_MOGALL_FOREST_QUARTZ),
	EXIT_METATILE(MOGALL_FOREST_KILLER_APE_TO_MOGALL_FOREST_SIDE_PATHS_WEST),
	EXIT_METATILE(MOGALL_FOREST_KILLER_APE_STUMPS_TO_CHEST),
	EXIT_METATILE(MOGALL_FOREST_KILLER_APE_STUMPS_TO_NORTHWEST_CLEARING),
	EXIT_METATILE(MOGALL_FOREST_KILLER_APE_STUMPS_TO_RIVER),
	EXIT_METATILE(MOGALL_FOREST_KILLER_APE_CHEST_TO_STUMPS),
	EXIT_METATILE(MOGALL_FOREST_KILLER_APE_RIVER_TO_STUMPS),
	EXIT_METATILE(MOGALL_FOREST_KILLER_APE_TO_MOGALL_FOREST_SIDE_PATHS_EAST),
	EXIT_METATILE(MOGALL_FOREST_KILLER_APE_TO_MOGALL_FOREST_SIDE_PATHS_SOUTH),
	EXIT_METATILE(MOGALL_FOREST_KILLER_APE_RIVER_TO_BRIDGES),
	EXIT_METATILE(MOGALL_FOREST_KILLER_APE_BRIDGES_TO_RIVER),
	EXIT_METATILE(MOGALL_FOREST_KILLER_APE_TO_WORLD_MAP),
	{STEP_CUE_WALKING, METATILE_BEHAVIOR_MOGALL_FOREST_LOG, CONDITION_ALWAYS,
		SCRIPT_REF(scMogallForestThirdAreaPushLog)},
	{STEP_CUE_WALKING_INTO(FACING_WEST), METATILE_BEHAVIOR_MOGALL_FOREST_JUMP,
		CONDITION_ALWAYS, SCRIPT_REF(scMogallForestThirdAreaJumpWest)},
	{CUE_STEP, METATILE_BEHAVIOR_MOGALL_FOREST_BESIDE_LONE_STUMP,
		FLAG_MOGALL_FOREST_LED_TO_KILLER_APE,
		SCRIPT_REF(scMogallForestApeLeadsToKillerApe)},
	{CUE_STEP, METATILE_BEHAVIOR_MOGALL_FOREST_BELOW_LONE_STUMP,
		FLAG_MOGALL_FOREST_KILLER_APE_BEATEN,
		SCRIPT_REF(scMogallForestKillerApeAttacks)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_THIRD_AREA_NORTH_ROCK,
		CONDITION_ALWAYS, SCRIPT_REF(scMogallForestThirdAreaUpdateNorthRock)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_THIRD_AREA_SOUTH_ROCK,
		FLAG_MOGALL_FOREST_SOUTH_ROCK_SETTLED,
		SCRIPT_REF(scMogallForestThirdAreaUpdateSouthRock)},
	{PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_THIRD_AREA_ATTACKING_APE_STUMP,
		FLAG_MOGALL_FOREST_THIRD_AREA_ATTACKING_APE_OUT,
		SCRIPT_REF(scMogallForestThirdAreaApeAttacks)},
	{PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_THIRD_AREA_MIDDLE_APE_STUMP,
		FLAG_MOGALL_FOREST_THIRD_AREA_MIDDLE_APE_OUT,
		SCRIPT_REF(scMogallForestThirdAreaMiddleApeShowsWay)},
	{PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_THIRD_AREA_WEST_APE_STUMP,
		FLAG_MOGALL_FOREST_THIRD_AREA_WEST_APE_OUT,
		SCRIPT_REF(scMogallForestThirdAreaWestApeShowsWay)},
	{PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_THIRD_AREA_EAST_APE_STUMP,
		FLAG_MOGALL_FOREST_THIRD_AREA_EAST_APE_MOVED,
		SCRIPT_REF(scMogallForestThirdAreaEastApeHidesAgain)},
	{PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_THIRD_AREA_EAST_APE_STUMP,
		FLAG_MOGALL_FOREST_THIRD_AREA_EAST_APE_OUT,
		SCRIPT_REF(scMogallForestThirdAreaEastApeShowsWay)},
	{EXAMINE_CUE(EXAMINE_CHEST), 100, FLAG_MOGALL_FOREST_ELVEN_SHIRT_CHEST,
		FIND_ITEM(ITEM_ELVEN_SHIRT)},
	{CUE_TABLE_END},
};

CUE MogallForestSidePaths_Cues[] = {
	EXIT_METATILE(MOGALL_FOREST_SIDE_PATHS_TO_MOGALL_FOREST_SIGN_NORTH),
	EXIT_METATILE(MOGALL_FOREST_SIDE_PATHS_TO_MOGALL_FOREST_SIGN_WEST),
	EXIT_METATILE(MOGALL_FOREST_SIDE_PATHS_TO_MOGALL_FOREST_QUARTZ_NORTH),
	EXIT_METATILE(MOGALL_FOREST_SIDE_PATHS_TO_MOGALL_FOREST_QUARTZ_SOUTH),
	EXIT_METATILE(MOGALL_FOREST_SIDE_PATHS_TO_MOGALL_FOREST_KILLER_APE_WEST),
	EXIT_METATILE(MOGALL_FOREST_SIDE_PATHS_TO_MOGALL_FOREST_KILLER_APE_EAST),
	EXIT_METATILE(MOGALL_FOREST_SIDE_PATHS_TO_MOGALL_FOREST_KILLER_APE_SOUTH),
	EXIT_METATILE(MOGALL_FOREST_SIDE_PATHS_TO_MOGALL_FOREST_SIGN_EAST),
	/* The two exit metatiles whose metatile behaviors are 11 and 12 take exits
	   20 and 19, not their own numbers. */
	{CUE_EXIT, 11, CONDITION_ALWAYS,
		MOGALL_FOREST_SIDE_PATHS_TO_WEST_CLEARING_WEST},
	{CUE_EXIT, 12, CONDITION_ALWAYS,
		MOGALL_FOREST_SIDE_PATHS_TO_NORTH_CLEARING_EAST},
	EXIT_METATILE(MOGALL_FOREST_SIDE_PATHS_TO_CENTER_CLEARING),
	EXIT_METATILE(MOGALL_FOREST_SIDE_PATHS_TO_EAST_CLEARING),
	EXIT_METATILE(MOGALL_FOREST_SIDE_PATHS_TO_SOUTHWEST_CLEARING),
	EXIT_METATILE(MOGALL_FOREST_SIDE_PATHS_TO_SOUTH_CLEARING_NORTHWEST),
	EXIT_METATILE(MOGALL_FOREST_SIDE_PATHS_TO_SOUTH_CLEARING_SOUTHWEST),
	EXIT_METATILE(MOGALL_FOREST_SIDE_PATHS_TO_SOUTHEAST_CLEARING),
	{EXAMINE_CUE(EXAMINE_SILENT), 100, FLAG_MOGALL_FOREST_PSYNERGY_STONE,
		FIND_PSYNERGY_STONE},
	{CUE_TABLE_END},
};
