/* Vale Cave, the optional cave above Vale where Kite, a Jupiter Djinni, hides.
   Its overlay holds three scenes.

   - The clearing outside (Vale, SceneId_ValeCaveMouth), a grassy hillside
	 reached by the two paths that run north past the west side of Kraden's
   cottage. The lower path comes in at metatiles (5..7, 22) and the raised
   path  east of it at (8, 23..25); both lead back down
   (scValeCaveLeaveRoom). The cave's mouth is a dark opening in the cliff at
   metatile (8, 10),
   beside  stacked crates and two logs. The game still calls the clearing
   Vale, so  arriving here clears FLAG_SHOW_AREA_NAME.
   - The first room of the cave (SceneId_ValeCaveKite), one map holding three
	 parts that the player meets as separate rooms, joined by passages.
	 Inside the mouth is a pool crossed by hopping from post to post. Reveal
	 shows the post missing from the row, at metatile (15, 15)
	 (scValeCaveRevealHiddenTile). Beyond it, a passage in the north-west leads
	 to a pool with two stacks of crates and a log floating by its north wall.
	 Move or a push sinks the eastern stack into the water, where its top
	 becomes a square to walk across on the way to Kite's part. Two Moves sink
	 the western stack beside the log. Isaac then hops onto the log, which
	 carries him across the pool to the passage on its east side
	 (scValeCaveRideLogEast).
   - The room of logs (SceneId_ValeCaveLogs). Its southern passage leads round
	 to a pool between two ledges, each with a log standing on it, and a third
	 log lying across the floor south of the water. Isaac rolls the two
	 standing logs into the water, hops down, and pushes the lying log north
	 into the pool, where it floats and lets him across.
   - Kite's part of the first room, reached from the room of logs and from
	 the riding log's pool. The Halt Gem's and the Nut's chests are near the
	 top of a narrow ridge. Walking down the ridge startles Kite, who hops off
	 its south end onto the round floor below, where three mossy rocks stand in
	 a row (scValeCaveKiteRunsOff). Kite then runs round behind the rocks
	 (scValeCaveKiteRunsBehindRocks) and keeps to their far side as Isaac walks
	 round them (scValeCaveKiteKeepsAway). Halt stops it, and
	 talking to it then makes it join the party (scValeCaveTalkToKite,
	 FLAG_KITE).

   No story flag is read here and no message is shown. Each room's progress
   lives in scene and area flags, and on every arrival scValeCaveApplyEntryState
   rebuilds the metatiles of each puzzle already solved and puts Kite back
   behind its rocks. The two rooms of the cave proper are one area, so their
   area flags (the riding log's end, Kite behind the rocks) last while the party
   goes between them. */

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "debug/debug.h"
#include "field/field_effect.h"
#include "graphics/graphics.h"
#include "sound/sound_driver_symbols.h"
#include "game/system_call.h"

extern Sint32		  ValeCaveWalkTargets[][2];
extern SCENE_ENTRANCE ValeCaveMouthEntrances[];
extern SCENE_ENTRANCE ValeCaveKiteEntrances[];
extern SCENE_ENTRANCE ValeCaveLogsEntrances[];
extern Uint32		  ValeCaveExits[];
extern ACTOR_PLACE	  ValeCaveMouthActorPlacements[];
extern ACTOR_PLACE	  ValeCaveKiteActorPlacements[];
extern ACTOR_PLACE	  ValeCaveLogsActorPlacements[];
extern CUE			  ValeCaveCuesOther[];
extern CUE			  ValeCaveMouthCues[];
extern CUE			  ValeCaveKiteCues[];
extern CUE			  ValeCaveLogsCues[];

/* The clearing's actor placements (ValeCaveMouthActorPlacements), numbered
   from ACTOR_FIRST_PLACED.

   They are three stacks of crates, drawn as the crates of the Bilibin Barricade
   in bilibin.c, then two logs, as in Kolima Forest. None of them moves. */
typedef enum VALE_CAVE_MOUTH_ACTOR_ID {
	ACTOR_VALE_CAVE_MOUTH_FIRST_CRATES = ACTOR_FIRST_PLACED,
	ACTOR_VALE_CAVE_MOUTH_FIRST_LOG = ACTOR_FIRST_PLACED + 3,
	ACTOR_VALE_CAVE_MOUTH_SECOND_LOG
} VALE_CAVE_MOUTH_ACTOR_ID;

/* The first room's actor placements (ValeCaveKiteActorPlacements), numbered
   from ACTOR_FIRST_PLACED. */
typedef enum VALE_CAVE_KITE_ACTOR_ID {
	/* Kite, a Jupiter Djinni, placed until it joins (FLAG_KITE). */
	ACTOR_KITE = ACTOR_FIRST_PLACED,
	/* The two stacks of crates at the edge of the riding log's pool, which sink
	   into the water. The western stack sinks under Move, the eastern one under
	   Move or a push. */
	ACTOR_WEST_CRATES,
	ACTOR_EAST_CRATES,
	/* The log floating by the north wall of its pool, which carries Isaac from
	   one end of the pool to the other. It is drawn as the north-south log of
	   Colosso's Lumber Water Crossing Area (colosseum_azart_match.c). */
	ACTOR_RIDING_LOG,
	/* Two stacks of crates and two logs, stacked by the passage to the room of
	   logs as they are at the cave's mouth. */
	ACTOR_FIRST_STANDING_CRATES,
	ACTOR_SECOND_STANDING_CRATES,
	ACTOR_FIRST_STANDING_LOG,
	ACTOR_SECOND_STANDING_LOG
} VALE_CAVE_KITE_ACTOR_ID;

/* The room of logs' actor placements (ValeCaveLogsActorPlacements), numbered
   from ACTOR_FIRST_PLACED.

   Its first row places Kite as the first room's does, out of sight of this
   room. Its three logs are pushed, and are drawn as Kolima Forest's. */
typedef enum VALE_CAVE_LOGS_ACTOR_ID {
	/* Standing on the ledge west of the pool, rolled east into the water. */
	ACTOR_WEST_LOG = ACTOR_FIRST_PLACED + 1,
	/* Standing on the ledge east of the pool, pushed west until it rolls
	   off. */
	ACTOR_EAST_LOG,
	/* Lying west to east across the floor south of the pool, pushed north. */
	ACTOR_SOUTH_LOG
} VALE_CAVE_LOGS_ACTOR_ID;

/* The metatiles the cave's step cues answer. */
typedef enum VALE_CAVE_METATILE_BEHAVIOR {
	/* The riding log's west end, metatiles (23, 34..36), and its east end,
	   metatiles (35, 34..36). Only the end the log lies at holds its
	   behavior. */
	METATILE_BEHAVIOR_VALE_CAVE_RIDING_LOG_WEST_END = 20,
	METATILE_BEHAVIOR_VALE_CAVE_RIDING_LOG_EAST_END,
	/* The bank east of the riding log's pool's eastern crates, metatile (29,
	   41). */
	METATILE_BEHAVIOR_VALE_CAVE_EAST_CRATES_BANK = 24,
	/* In Kite's part, the top of the ridge, metatile (49, 11), and its foot,
	   metatile (49, 16). */
	METATILE_BEHAVIOR_VALE_CAVE_KITE_RIDGE_TOP = 40,
	METATILE_BEHAVIOR_VALE_CAVE_KITE_RIDGE_FOOT,
	/* The first of the twelve metatile behaviors on the ring of floor round
	   Kite's mossy rocks (scValeCaveKiteKeepsAway), one for each of
	   ValeCaveWalkTargets. */
	METATILE_BEHAVIOR_VALE_CAVE_FIRST_KITE_ROUTE = 45,
	/* In Kite's part, the chests of the Halt Gem and of the Nut. */
	METATILE_BEHAVIOR_VALE_CAVE_HALT_GEM_CHEST = 100,
	METATILE_BEHAVIOR_VALE_CAVE_NUT_CHEST
} VALE_CAVE_METATILE_BEHAVIOR;

/* The metatiles of the room of logs' step cues. */
typedef enum VALE_CAVE_LOGS_METATILE_BEHAVIOR {
	/* The ledge east of the east log, metatiles (20, 16..17). */
	METATILE_BEHAVIOR_VALE_CAVE_EAST_LOG_LEDGE = 20,
	/* The ledge west of the west log, metatiles (11, 16..17). */
	METATILE_BEHAVIOR_VALE_CAVE_WEST_LOG_LEDGE,
	/* The floor south of the south log, metatiles (13..16, 23). */
	METATILE_BEHAVIOR_VALE_CAVE_SOUTH_LOG_FLOOR
} VALE_CAVE_LOGS_METATILE_BEHAVIOR;

/* SPRITE_CRATES sunk into the water: its top crate alone, as a square to walk
   on (crates.png). */
enum { CRATES_ANIMATION_SUNK = 2 };

/* The pushed logs' animations beyond LOG_ANIMATION, from log_north_south.tsv
   and log_west_east.tsv, which order theirs alike. */
typedef enum VALE_CAVE_LOG_ANIMATION {
	/* log_west_east.tsv's roll in the order of LOG_ANIMATION_ROLL_WEST, as the
	   south log tips north into the pool. */
	LOG_WEST_EAST_ANIMATION_ROLL = 3
} VALE_CAVE_LOG_ANIMATION;

/* The map columns where the riding log's pool's western and eastern crates
   sink: the western stack at metatile (23, 39), after two Moves east, and the
   eastern one at metatile (27, 41), after one Move or push west. */
#define WEST_CRATES_SINK_COLUMN 23
#define EAST_CRATES_SINK_COLUMN 27
/* The map column at the west end of the east log's ledge, metatile (17, 16),
   from which the log rolls off into the pool. */
#define LEDGE_EDGE_COLUMN		17
/* The map row the south log can stop on when pushed north. */
#define SOUTH_LOG_SOUTH_ROW		20

/* The flag of Kite, set once it is caught. */
#define FLAG_KITE (FLAG_DJINNI(DJINNI_KITE))

/* The cave's scene flags: each room reuses them for its own puzzle, so the
   first room's two have one name at the riding log's pool and another in Kite's
   part. The riding log's pool: the western crates have sunk into the water. */
#define FLAG_VALE_CAVE_WEST_CRATES_SUNK		 FLAG_SCENE_TEMP_00
/* The riding log's pool: the eastern crates have sunk into the water. */
#define FLAG_VALE_CAVE_EAST_CRATES_SUNK		 FLAG_SCENE_TEMP_01
/* Kite's part: Kite has run off once (scValeCaveKiteRunsOff). */
#define FLAG_VALE_CAVE_KITE_RAN_OFF			 FLAG_SCENE_TEMP_00
/* Kite's part: Kite has run behind the mossy rocks
   (scValeCaveKiteRunsBehindRocks). */
#define FLAG_VALE_CAVE_KITE_RAN_BEHIND_ROCKS FLAG_SCENE_TEMP_01

/* The cave's area flags. The first room and the room of logs are one area, so
   these last while the party goes between them; the clearing is another. The
   riding log is at the east end of its pool. */
#define FLAG_VALE_CAVE_RIDING_LOG_EAST	 FLAG_AREA_TEMP_01
/* Kite has run behind the mossy rocks and keeps to their far side. */
#define FLAG_VALE_CAVE_KITE_BEHIND_ROCKS FLAG_AREA_TEMP_02

static void scValeCaveCenterActorOnMetatile(
	Sint32 actorId, Sint32 column, Sint32 row);
static void scValeCaveApplyEntryState(void);

#include "shared/field_effect_create.c"
#include "shared/field_effect_spawn.c"
#include "shared/pushed_actor.c"

/* In the room of logs, the west log has been rolled into the pool. */
#define FLAG_VALE_CAVE_WEST_LOG_PUSHED_OFF_LEDGE FLAG_SCENE_TEMP_02
/* In the room of logs, the east log has been pushed along its ledge and has
   rolled off into the pool. */
#define FLAG_VALE_CAVE_EAST_LOG_PUSHED_OVER_EDGE FLAG_SCENE_TEMP_03
/* In the room of logs, the south log has been pushed north and stopped on the
   northern of its two rows. */
#define FLAG_VALE_CAVE_SOUTH_LOG_ON_NORTH_ROW	 FLAG_SCENE_TEMP_04
/* In the room of logs, the south log has been pushed north and stopped on the
   southern of its two rows (SOUTH_LOG_SOUTH_ROW). */
#define FLAG_VALE_CAVE_SOUTH_LOG_ON_SOUTH_ROW	 FLAG_SCENE_TEMP_05

/* Talking to Kite, once Halt has stopped it on the round floor below the ridge,
   makes the Jupiter Djinni join the party. */
static void scValeCaveTalkToKite(void)
{
	fdDjinnJoinParty(ACTOR_KITE, ELEMENT_JUPITER, DJINNI_INDEX(DJINNI_KITE));
}

/* Waits for an actor to land, bounded by a one-second timeout. */
static void scValeCaveWaitForActorLanding(ACTOR *actor)
{
	Sint32 ticksRemaining = 60;

	while(ticksRemaining != 0) {
		sySynch(1);
		ticksRemaining--;
		if(actor->y.fixed <= actor->groundHeight)
			break;
	}
	actor->y.fixed = actor->groundHeight;
}

/* Turns Kite to face the party leader each tick, so that it watches Isaac from
   behind its rocks. */
static UNREAD_RESULT scValeCaveFaceActorTowardLeader(ACTOR *actor)
{
	ACTOR *leader = fdActorGet(ACTOR_PARTY_LEADER);
	FIXED  distanceZ = leader->z.fixed - actor->z.fixed;
	FIXED  distanceX = leader->x.fixed - actor->x.fixed;

	actor->facing = syAtan(distanceZ, distanceX);
	RETURN_UNREAD(0);
}

/* scValeCaveFaceActorTowardLeader, as an actor's update takes it. */
#if FIX_FUNCTION_SIGNATURES
#define VALE_CAVE_FACE_LEADER_CALLBACK scValeCaveFaceActorTowardLeader
#else
#define VALE_CAVE_FACE_LEADER_CALLBACK \
	((void (*)(ACTOR *))scValeCaveFaceActorTowardLeader)
#endif

/* Selects entrance positions and camera bounds for the current cave room. */
const SCENE_ENTRANCE *scGetEntrances(void)
{
	Sint16 sceneId = MainRoutineStateWork.next;

	if(sceneId == SCENE_VALE_CAVE_MOUTH) {
		return ValeCaveMouthEntrances;
	}
	if(sceneId == SCENE_VALE_CAVE_KITE) {
		return ValeCaveKiteEntrances;
	}
	if(sceneId == SCENE_VALE_CAVE_LOGS) {
		return ValeCaveLogsEntrances;
	}
	return SceneFallbackEntrances;
}

const SCENE_EXIT_REGION *scGetExitRegions(void)
{
	return NULL;
}

/* Returns the shared packed exits for all cave rooms. */
const Uint32 *scGetExits(void)
{
	return ValeCaveExits;
}

/* Selects the current room's actor placements, which are Kite, and the crates
   and logs. */
const ACTOR_PLACE *scGetActorPlacements(void)
{
	Sint16 sceneId = MainRoutineStateWork.next;

	if(sceneId == SCENE_VALE_CAVE_MOUTH) {
		return ValeCaveMouthActorPlacements;
	}
	if(sceneId == SCENE_VALE_CAVE_KITE) {
		return ValeCaveKiteActorPlacements;
	}
	if(sceneId == SCENE_VALE_CAVE_LOGS) {
		return ValeCaveLogsActorPlacements;
	}
	return ValeCaveMouthActorPlacements;
}

/* When Reveal fades in the pool just inside the cave, the post it showed at
   metatile (15, 15) is gone again and the gap in the row of posts is water. */
static void scValeCaveHideRevealedTile(void)
{
	fdMapGridCopyMetatiles(16, 15, 1, 1, 15, 15);
}

/* Reveal, cast in the pool just inside the cave's mouth, shows the post missing
   from the row of posts across the water, at metatile (15, 15), so that Isaac
   can hop over it to the west side.

   The post is copied from a spare one at metatile (16, 17), and
   scValeCaveHideRevealedTile puts the water back when Reveal fades. */
static void scValeCaveRevealHiddenTile(void)
{
	fdMapGridCopyMetatiles(16, 17, 1, 1, 15, 15);
}

/* Sinks a stack of crates into the water over 68 ticks, with a white splash of
   spray round it. */
static inline void scValeCaveSinkActorWithParticles(
	Sint32 actorId, Sint32 baseX, Sint32 baseZ, EFFECT_OPTIONS *particleOptions)
{
	Uint32 frm;

	for(frm = 0; frm < 68; frm++) {
		FIXED particleX = toFIXED(syRandomBelow(17)) + baseX;
		FIXED particleZ = toFIXED(syRandomBelow(14)) + baseZ;

		fdEffectSpawn(particleX, 0, particleZ, 0, 0, 0,
			EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE, particleOptions);
		fdActorGet(actorId)->y.fixed -= toFIXED(1) / 2;
		fdScriptWait(1);
	}
}

/* Move on the western stack of crates in the riding log's pool sinks it into
   the water once a second Move has brought it to metatile (23, 39), just south
   of the floating log. Its top is then a square Isaac hops onto, on his way to
   the log.

   The game's Move has already slid the stack a metatile east. Short of the sink
   column the script does nothing more. */
static void scValeCaveMoveWestCratesIntoWater(void)
{
	EFFECT_OPTIONS options;
	ACTOR		  *leftEffect;
	ACTOR		  *rightEffect;

	fdScriptStart();
	if(FIELD_METATILE(fdActorGet(ACTOR_WEST_CRATES)->x.fixed) ==
		WEST_CRATES_SINK_COLUMN) {
		fdActorMoveToAndWait(ACTOR_PARTY_LEADER, 360, 664);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 10);
		fdActorGet(ACTOR_WEST_CRATES)->x.fixed += toFIXED(2);
		leftEffect = scActorPrepareFX(fdActorGet(ACTOR_WEST_CRATES)->x.fixed, 0,
			fdActorGet(ACTOR_WEST_CRATES)->z.fixed + toFIXED(52),
			SPRITE_CRATES);
		rightEffect = scActorPrepareFX(
			fdActorGet(ACTOR_WEST_CRATES)->x.fixed + toFIXED(16), 0,
			fdActorGet(ACTOR_WEST_CRATES)->z.fixed + toFIXED(52),
			SPRITE_CRATES);
		fdActorGet(ACTOR_WEST_CRATES)->motionFlg = 0;
		/* Surround the sinking crates with one particle per tick. */
		options.startScaleX = toFIXED(3) / 5;
		options.startScaleY = toFIXED(3) / 5;
		options.pal = SPRITE_COLOR_WHITE;
		sySndPlay(SE_BURST);
		scValeCaveSinkActorWithParticles(
			ACTOR_WEST_CRATES, toFIXED(368), toFIXED(624), &options);
		fdMapGridCopyMetatileAttributes(23, 41, 1, 1, 23, 39);
		fdActorGet(ACTOR_WEST_CRATES)->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
		gmFlagSet(FLAG_VALE_CAVE_WEST_CRATES_SUNK);
		fdActorGet(ACTOR_WEST_CRATES)->y.fixed = -toFIXED(8);
		fdActorSetAnimation(ACTOR_WEST_CRATES, CRATES_ANIMATION_SUNK);
		fdActorRelease(leftEffect);
		fdActorRelease(rightEffect);
		fdScriptWait(30);
	}
	fdScriptEnd();
}

/* Move on the eastern stack of crates, at the foot of the rock pillar in the
   riding log's pool, slides it one metatile west into the water at metatile
   (27, 41). It sinks with a splash until only its top shows, a square of floor
   Isaac can walk across. */
static void scValeCaveMoveEastCratesIntoWater(void)
{
	EFFECT_OPTIONS options;
	ACTOR		  *leftEffect;
	ACTOR		  *rightEffect;

	fdScriptStart();
	if(FIELD_METATILE(fdActorGet(ACTOR_EAST_CRATES)->x.fixed) ==
		EAST_CRATES_SINK_COLUMN) {
		leftEffect = scActorPrepareFX(
			fdActorGet(ACTOR_EAST_CRATES)->x.fixed - toFIXED(8), 0,
			fdActorGet(ACTOR_EAST_CRATES)->z.fixed + toFIXED(52),
			SPRITE_CRATES);
		rightEffect = scActorPrepareFX(
			fdActorGet(ACTOR_EAST_CRATES)->x.fixed + toFIXED(8), 0,
			fdActorGet(ACTOR_EAST_CRATES)->z.fixed + toFIXED(52),
			SPRITE_CRATES);
		/* This clears the western crates' motion flags, not these crates',
		   where scValeCaveMoveWestCratesIntoWater clears its own. These sink
		   all the same, since the sinking writes their height directly and
		   nothing pulls them back up, as the recording of this script shows. */
		fdActorGet(ACTOR_WEST_CRATES)->motionFlg = 0;
		/* Surround the sinking crates with one particle per tick. */
		options.startScaleX = toFIXED(3) / 5;
		options.startScaleY = toFIXED(3) / 5;
		options.pal = SPRITE_COLOR_WHITE;
		sySndPlay(SE_BURST);
		scValeCaveSinkActorWithParticles(
			ACTOR_EAST_CRATES, toFIXED(432), toFIXED(656), &options);
		fdMapGridCopyMetatileAttributes(31, 39, 2, 1, 27, 41);
		fdActorGet(ACTOR_EAST_CRATES)->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
		gmFlagSet(FLAG_VALE_CAVE_EAST_CRATES_SUNK);
		fdActorGet(ACTOR_EAST_CRATES)->y.fixed = -toFIXED(8);
		fdActorSetAnimation(ACTOR_EAST_CRATES, CRATES_ANIMATION_SUNK);
		fdActorRelease(leftEffect);
		fdActorRelease(rightEffect);
		fdScriptWait(30);
	}
	fdScriptEnd();
}

/* Pushing the eastern stack of crates west, from the bank east of it at
   metatile (29, 41), slides it a metatile into the water, where it sinks as it
   does under Move (scValeCaveMoveEastCratesIntoWater). */
static void scValeCavePushEastCratesIntoWater(void)
{
	fdScriptStart();
	scPushedActorAdvancePair();
	fdScriptEnd();
	scValeCaveMoveEastCratesIntoWater();
}

/* Walking south off the riding log, once it has carried him to the east side of
   its pool (metatile (35, 36)), Isaac leaps down onto the rock bank below.

   He lands two metatiles south, at metatile (35, 38), and only if nothing
   stands there. */
static Bool scValeCaveLeapOffRidingLog(Sint32 unusedArgument)
{
	FIELD_COORDINATE landingPos[3];
	ACTOR			*leader;
	Sint32			 savedMotionFlg;

	leader = fdActorGet(ACTOR_PARTY_LEADER);
	savedMotionFlg = leader->motionFlg;
#if JUMP_LANDS_ON_CELL_CENTER
	landingPos[0].fixed = (leader->x.fixed & FIELD_METATILE_MASK) + toFIXED(8);
	landingPos[1].fixed = leader->y.fixed;
	landingPos[2].fixed = (leader->z.fixed & FIELD_METATILE_MASK) + toFIXED(40);
#else
	landingPos[0].fixed = leader->x.fixed;
	landingPos[1].fixed = leader->y.fixed;
	landingPos[2].fixed = leader->z.fixed + toFIXED(32);
#endif
	if(syFieldActorCheckMovementCollision(leader, (FIELD_POS *)landingPos) ==
		0) {
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
		fdActorMoveToAndWait(ACTOR_PARTY_LEADER, landingPos[0].part[0].integer,
			landingPos[2].part[0].integer);
		fdActorSelectAnimation(leader, ANIM_LEADER_SPRING);
		fdActorSetSprFlags(leader, SPRITE_FLAG_SHADOW);
		leader->motionFlg = savedMotionFlg;
		fdScriptEnd();
		return TRUE;
	}
	return FALSE;
}

/* Walking onto the narrow ridge that runs south between two channels of water
   in Kite's part (metatile (49, 11)) startles Kite, who stands further down it.
   The camera follows Kite as it gives a start and hops off the ridge's south
   end onto the round floor below. */
static void scValeCaveKiteRunsOff(void)
{
	if(fdScriptGetFlag(FLAG_VALE_CAVE_KITE_RAN_OFF) == 0) {
		gmFlagSet(FLAG_VALE_CAVE_KITE_RAN_OFF);
		fdScriptStart();
		fdCameraSetSpeed(1);
		fdCameraFollowActor(ACTOR_KITE, 1);
		fdCameraWaitForArrival();
		fdScriptWait(60);
		fdActorFaceDirection(ACTOR_KITE, FACING_NORTH, 20);
		fdActorSetAttachedEffect(
			ACTOR_KITE, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
		fdActorRunRepeatedMotion(ACTOR_KITE, 2);
		fdScriptWait(20);
		fdActorSetSpeed(ACTOR_KITE, 1);
		fdActorWalkToAndWait(ACTOR_KITE, 792, 248);
		sySndPlay(SE_JUMP);
		fdActorGet(ACTOR_KITE)->velY = toFIXED(8);
		fdActorWalkToAndWait(ACTOR_KITE, 792, 280);
		fdScriptWait(20);
		fdActorFaceDirection(ACTOR_KITE, FACING_NORTH, 20);
		fdScriptWait(30);
		fdScriptEnd();
	}
}

/* Reaching the foot of the ridge (metatile (49, 16)) after Kite has fled makes
   it run round behind the three mossy rocks on the round floor, where it turns
   to watch Isaac. */
static void scValeCaveKiteRunsBehindRocks(void)
{
	if(gmFlagGet(FLAG_VALE_CAVE_KITE_RAN_OFF) != 0) {
		if(fdScriptGetFlag(FLAG_VALE_CAVE_KITE_RAN_BEHIND_ROCKS) == 0) {
			gmFlagSet(FLAG_VALE_CAVE_KITE_RAN_BEHIND_ROCKS);
			gmFlagSet(FLAG_VALE_CAVE_KITE_BEHIND_ROCKS);
			fdScriptStart();
			fdActorSetAttachedEffect(
				ACTOR_KITE, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
			fdActorRunRepeatedMotion(ACTOR_KITE, 2);
			fdScriptWait(20);
			fdActorSetSpeed(ACTOR_KITE, 2);
			fdActorWalkToAndWait(ACTOR_KITE, 760, 280);
			fdActorWalkToAndWait(ACTOR_KITE, 760, 312);
			fdActorWalkToAndWait(ACTOR_KITE, 792, 312);
			fdScriptWait(10);
			fdActorFaceDirection(ACTOR_KITE, FACING_NORTH, 20);
			fdActorGet(ACTOR_KITE)->update = VALE_CAVE_FACE_LEADER_CALLBACK;
			fdScriptEnd();
		}
	}
}

/* As Isaac walks round the three mossy rocks, Kite keeps to their far side,
   matching his pace, until Halt stops it.

   Each metatile of the ring of floor round the rocks (metatiles (46..52,
   17..20)) sends Kite to the point opposite, from ValeCaveWalkTargets. */
static void scValeCaveKiteKeepsAway(void)
{
	FIELD_STATE *state = FieldStateWork;
	Sint32		 route;

	if(gmFlagGet(FLAG_VALE_CAVE_KITE_BEHIND_ROCKS) != 0 &&
		MainRoutineStateWork.haltedActor != ACTOR_KITE) {
		route = state->stepCue - METATILE_BEHAVIOR_VALE_CAVE_FIRST_KITE_ROUTE;
		fdActorGet(ACTOR_KITE)->speedLimit =
			fdActorGet(ACTOR_PARTY_LEADER)->speedLimit;
		fdActorGet(ACTOR_KITE)->acceleration =
			fdActorGet(ACTOR_PARTY_LEADER)->acceleration;
		fdActorWalkTo(ACTOR_KITE, ValeCaveWalkTargets[route][0],
			ValeCaveWalkTargets[route][1]);
	}
}

/* Halt on Kite stops it where it stands behind the mossy rocks, so that Isaac
   can walk round and talk to it. */
static void scValeCaveHaltKite(void)
{
	fdScriptStart();
	fdActorSetAnimation(ACTOR_KITE, ANIM_STILL);
	fdScriptEnd();
}

/* Pushing the log standing on the ledge east of the pool in the room of logs
   moves it west a metatile at a time. At the ledge's west end, metatile (17,
   16), it rolls off into the pool. */
static void scValeCavePushLogOverEdge(void)
{
	PUSHED_ACTOR_PROBE probe;

	fdScriptStart();
	if(scPushedActorFindClearPos(&probe) != 0) {
		scPushedActorRoll(probe);
		/* At the ledge's edge the log slides west and drops. */
		if(FIELD_METATILE(probe.pos.x) == LEDGE_EDGE_COLUMN) {
			fdActorSetAnimation(probe.actorSlot, LOG_ANIMATION_ROLL_WEST);
			fdActorGet(probe.actorSlot)->motionFlg = 0;
			fdActorGet(probe.actorSlot)->bounce = 0;
			fdActorSetDestinationOffset(probe.actorSlot, -12, 0);
			fdActorWaitForMoveById(probe.actorSlot);
			fdActorSetAnimation(probe.actorSlot, LOG_ANIMATION_ROLL_WEST);
			fdActorSetSprPriority(ACTOR_EAST_LOG, 3);
			fdActorGet(probe.actorSlot)->motionFlg =
				ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
			fdActorSetDestinationOffset(probe.actorSlot, -6, 0);
			scValeCaveWaitForActorLanding(fdActorGet(probe.actorSlot));
			fdActorSetAnimation(probe.actorSlot, LOG_ANIMATION_DROP_IN);

			fdActorGet(probe.actorSlot)->priFlg = ACTOR_PRIORITY_UNDERFOOT;
			/* The metatiles it held on the ledge, and its landing, become open
			   floor. */
			scMapGridFillCollisionRectangle(0, FIELD_METATILE(probe.pos.x),
				(FIELD_METATILE(probe.pos.z) - 2), 1, 4,
				METATILE_BEHAVIOR_NORMAL);
			scMapGridFillCollisionRectangle(2, FIELD_METATILE(probe.pos.x),
				(FIELD_METATILE(probe.pos.z) - 2), 1, 4,
				METATILE_BEHAVIOR_NORMAL);
			scMapGridFillCollisionRectangle(
				2, 16, 18, 1, 2, METATILE_BEHAVIOR_NORMAL);
			scMapGridFillCollisionRectangle(
				0, 16, 16, 1, 4, METATILE_BEHAVIOR_NORMAL);
			gmFlagSet(FLAG_VALE_CAVE_EAST_LOG_PUSHED_OVER_EDGE);
			sySndPlay(SE_LOG_FALL);
		}
	}
	fdScriptEnd();
}

/* Pushing the log standing on the ledge west of the pool in the room of logs,
   from metatile (11, 16), rolls it east off the ledge into the pool. */
static void scValeCavePushLogOffLedge(void)
{
	fdScriptStart();
	fdScriptWait(10);
	fdActorSetSpeedAndAcceleration(ACTOR_PARTY_LEADER, 0.5, 0.1);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_PUSH);
	fdScriptWait(15);
	fdActorSetDestinationOffset(ACTOR_PARTY_LEADER, 8, 0);
	fdScriptWait(4);
	fdScriptPlaySnd(SE_STOP_ACTION_SE);
	sySndPlay(SE_HEAVY_MOVE);
	fdActorSetSpeedAndAcceleration(ACTOR_WEST_LOG, 0.5, 0.1);
	fdActorSetAnimation(ACTOR_WEST_LOG, LOG_ANIMATION_ROLL_EAST);
	fdActorGet(ACTOR_WEST_LOG)->motionFlg = 0;
	fdActorGet(ACTOR_WEST_LOG)->bounce = 0;
	fdActorSetDestinationOffset(ACTOR_WEST_LOG, 12, 0);
	fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	fdActorWaitForMoveById(ACTOR_WEST_LOG);
	sySndPlay(SE_STOP_ACTION_SE);
	sySndPlay(SE_HEAVY_STOP);
	fdActorSetAnimation(ACTOR_WEST_LOG, LOG_ANIMATION_ROLL_WEST);
	fdActorGet(ACTOR_WEST_LOG)->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	fdActorSetDestinationOffset(ACTOR_WEST_LOG, 6, 0);
	scValeCaveWaitForActorLanding(fdActorGet(ACTOR_WEST_LOG));
	fdActorSetAnimation(ACTOR_WEST_LOG, LOG_ANIMATION_DROP_IN);
	fdActorSetSprPriority(ACTOR_WEST_LOG, 3);
	fdActorGet(ACTOR_WEST_LOG)->priFlg = ACTOR_PRIORITY_UNDERFOOT;
	scMapGridFillCollisionRectangle(0, 12, 16, 1, 4, METATILE_BEHAVIOR_NORMAL);
	scMapGridFillCollisionRectangle(0, 13, 16, 1, 4, METATILE_BEHAVIOR_NORMAL);
	gmFlagSet(FLAG_VALE_CAVE_WEST_LOG_PUSHED_OFF_LEDGE);
	sySndPlay(SE_LOG_FALL);
	fdScriptEnd();
}

/* Pushing the log that lies across the floor south of the pool in the room of
   logs north tips it into the pool's south end, where it floats with a splash
   and makes a way across. */
static void scValeCavePushLogNorth(void)
{
	PUSHED_ACTOR_PROBE probe;

	fdScriptStart();
	if(scPushedActorFindClearPos(&probe) != 0) {
		scPushedActorRoll(probe);
		fdActorSetAnimation(ACTOR_SOUTH_LOG, LOG_WEST_EAST_ANIMATION_ROLL);
		fdActorSetSpeedAndAcceleration(ACTOR_SOUTH_LOG, 0.25, 0.5);
		fdActorSetDestinationOffset(ACTOR_SOUTH_LOG, 0, -16);
		fdScriptWait(45);
		sySndPlay(SE_LOG_FALL);
		fdActorSetAnimation(ACTOR_SOUTH_LOG, LOG_ANIMATION_DROP_IN);

		fdActorGet(ACTOR_SOUTH_LOG)->priFlg = ACTOR_PRIORITY_UNDERFOOT;
		scMapGridFillCollisionRectangle(0, 13,
			(FIELD_METATILE(probe.pos.z)) - 1, 4, 2, METATILE_BEHAVIOR_NORMAL);
		if(FIELD_METATILE(probe.pos.z) == SOUTH_LOG_SOUTH_ROW) {
			gmFlagSet(FLAG_VALE_CAVE_SOUTH_LOG_ON_SOUTH_ROW);
		} else {
			gmFlagSet(FLAG_VALE_CAVE_SOUTH_LOG_ON_NORTH_ROW);
			fdMapGridCopyMetatileAttributes(14, 17, 2, 1, 14, 16);
			fdMapGridCopyMetatileAttributes(14, 13, 1, 1, 14, 15);
		}
	}
	fdScriptEnd();
}

/* Selects the current room's step, Psynergy and examine cues. */
const CUE *scGetCues(void)
{
	Sint16 sceneId = MainRoutineStateWork.next;

	if(sceneId == SCENE_VALE_CAVE_MOUTH) {
		return ValeCaveMouthCues;
	}
	if(sceneId == SCENE_VALE_CAVE_KITE) {
		return ValeCaveKiteCues;
	}
	if(sceneId == SCENE_VALE_CAVE_LOGS) {
		return ValeCaveLogsCues;
	}
	return ValeCaveCuesOther;
}

/* Arriving in any of the three rooms chooses the entry transition and restores
   the room's puzzle state. */
Sint32 scInit(void)
{
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	if(MainRoutineStateWork.next == SCENE_VALE_CAVE_MOUTH) {
		FieldStateWork->transition =
			SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
		sySynch(1);
		fdActorSetSprPriority(ACTOR_VALE_CAVE_MOUTH_FIRST_LOG, 3);
		fdActorSetSprPriority(ACTOR_VALE_CAVE_MOUTH_SECOND_LOG, 3);
		gmFlagClear(FLAG_SHOW_AREA_NAME);
	}
	scValeCaveApplyEntryState();
	return 0;
}

/* In the clearing outside Vale Cave, Isaac walks into the cave's mouth
   (metatile (8, 10)) or down either path back to Kraden's cottage: the lower
   path at metatiles (5..7, 22) or the raised one east of it at (8, 23..25).

   Each plays the exit sound first. The metatile's behavior is the exit's
   number. */
static void scValeCaveLeaveRoom(void)
{
	Sint32 exitNum = FieldStateWork->stepCue;

	sySndPlay(SE_MAP_EXIT);
	syImportScriptSetExitRequest(exitNum);
}

/* Puts the riding log at the end of its pool where it was left, and makes that
   end the one Isaac can step onto.

   The passage's metatiles are copied back from their spare copy at metatiles
   (0..12, 34..36), and the far end is then covered with water. */
static void scValeCaveUpdateRidingLogPassage(void)
{
	fdMapGridCopyMetatileAttributes(0, 34, 13, 3, 23, 34);

	if(gmFlagGet(FLAG_VALE_CAVE_RIDING_LOG_EAST) != 0) {
		scValeCaveCenterActorOnMetatile(ACTOR_RIDING_LOG, 35, 35);
		fdMapGridCopyMetatileAttributes(24, 34, 1, 3, 23, 34);
	} else {
		scValeCaveCenterActorOnMetatile(ACTOR_RIDING_LOG, 23, 35);
		fdMapGridCopyMetatileAttributes(24, 34, 1, 3, 35, 34);
	}
}

#include "shared/log_ride.c"

/* Walking east onto the riding log at the west end of its pool (metatile (23,
   35)) has it float east along the north wall with Isaac on it, to the pool's
   east side, where a post leads to the passage into the room of logs. */
static void scValeCaveRideLogEast(void)
{
	sySndPlay(SE_WATER_MOVE);
	scActorFloatLogWithLeader(ACTOR_RIDING_LOG, 112, 0);
	scActorFloatLogWithLeader(ACTOR_RIDING_LOG, 80, 0);
	gmFlagSet(FLAG_VALE_CAVE_RIDING_LOG_EAST);
	sySynch(2);
	scValeCaveUpdateRidingLogPassage();
	sySndPlay(SOUND_FADE_OUT_ACTION_SE);
}

/* Walking west onto the riding log at the east end of its pool (metatile (35,
   35)) has it float back west along the north wall with Isaac on it. */
static void scValeCaveRideLogWest(void)
{
	sySndPlay(SE_WATER_MOVE);
	scActorFloatLogWithLeader(ACTOR_RIDING_LOG, -112, 0);
	scActorFloatLogWithLeader(ACTOR_RIDING_LOG, -80, 0);
	gmFlagClear(FLAG_VALE_CAVE_RIDING_LOG_EAST);
	sySynch(2);
	scValeCaveUpdateRidingLogPassage();
	sySndPlay(SOUND_FADE_OUT_ACTION_SE);
}

/* Centers an actor on a map metatile and gives it underfoot priority. */
static void scValeCaveCenterActorOnMetatile(
	Sint32 actorId, Sint32 column, Sint32 row)
{
	ACTOR *actor = fdActorGet(actorId);

	if(actor != NULL) {
		fdActorSetSprPriority(actorId, 3);
		actor->layer = 2;
		actor->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
		actor->x.fixed = FIELD_METATILE_CENTER(column);
		actor->z.fixed = FIELD_METATILE_CENTER(row);
	}
}

/* Restores, on entry, Kite behind its mossy rocks and the crates and logs of
   the puzzles already solved. */
static void scValeCaveApplyEntryState(void)
{
	Sint32 southLogOnSouthernRow;

	/* Each room reuses the temporary flags for its own puzzle state. */
	if(MainRoutineStateWork.next == SCENE_VALE_CAVE_KITE) {
		switch(MainRoutineStateWork.entrance) {
		case VALE_CAVE_KITE_FROM_VALE_CAVE_MOUTH:
		case VALE_CAVE_KITE_WEST_FROM_EAST:
		case VALE_CAVE_KITE_WEST_FROM_SOUTH:
		case VALE_CAVE_KITE_FROM_VALE_CAVE_LOGS_WEST:
			fdActorSetSprPriority(ACTOR_SECOND_STANDING_LOG, 3);
			fdActorSetSprPriority(ACTOR_SECOND_STANDING_CRATES, 3);
			/* A still actor of sprite 223 (crate.png, a wooden crate) that
			   shows nothing. XXX (no evidence): what it and the chest below
			   were meant to show; the sprites alone hold neither, and neither
			   actor is solid or a target. */
			fdActorCreateConfigured(120, 0, 232, SPRITE_CRATE);
			break;
		case VALE_CAVE_KITE_EAST_FROM_WEST:
		case VALE_CAVE_KITE_EAST_FROM_SOUTH:
		case VALE_CAVE_KITE_FROM_VALE_CAVE_LOGS_EAST:
			if(gmFlagGet(FLAG_KITE) != 0) {
				break;
			}
			if(gmFlagGet(FLAG_VALE_CAVE_KITE_BEHIND_ROCKS) == 0) {
				break;
			}
			gmFlagSet(FLAG_VALE_CAVE_KITE_RAN_OFF);
			if(MainRoutineStateWork.entrance == VALE_CAVE_KITE_EAST_FROM_WEST) {
				gmFlagSet(FLAG_VALE_CAVE_KITE_RAN_BEHIND_ROCKS);
			}
			sySynch(1);
			if(gmFlagGet(FLAG_KEEP_PARTY_POSITION) != 0) {
				break;
			}
			/* Kite waits behind the mossy rocks, watching the leader. */
			fdActorSetPos(ACTOR_KITE, 792, 280);
			fdActorGet(ACTOR_KITE)->update = VALE_CAVE_FACE_LEADER_CALLBACK;
			break;
		case VALE_CAVE_KITE_SOUTH_FROM_WEST:
		case VALE_CAVE_KITE_SOUTH_FROM_EAST:
		case VALE_CAVE_KITE_FROM_VALE_CAVE_LOGS_SOUTH:
			/* The riding log's part: a still chest actor that shows nothing,
			   the riding log's passage, and the metatiles of crates already
			   sunk. */
			fdActorCreateConfigured(642, 0, 552, SPRITE_CHEST);
			fdMapGridCopyMetatileAttributes(23, 34, 13, 3, 0, 34);
			scValeCaveUpdateRidingLogPassage();
			/* The metatiles scValeCaveMoveWestCratesIntoWater copies as it sets
			   the flag, redrawn on arrival. */
			if(gmFlagGet(FLAG_VALE_CAVE_WEST_CRATES_SUNK) != 0) {
				fdMapGridCopyMetatileAttributes(23, 41, 1, 1, 23, 39);
			}
			/* The metatiles scValeCaveMoveEastCratesIntoWater copies as it sets
			   the flag, redrawn on arrival. */
			if(gmFlagGet(FLAG_VALE_CAVE_EAST_CRATES_SUNK) != 0) {
				fdMapGridCopyMetatileAttributes(31, 39, 2, 1, 27, 41);
			}
			break;
		}
	} else if(MainRoutineStateWork.next == SCENE_VALE_CAVE_LOGS) {
		switch(MainRoutineStateWork.entrance) {
		case VALE_CAVE_LOGS_FROM_VALE_CAVE_KITE_WEST:
		case VALE_CAVE_LOGS_NORTH_FROM_SOUTH:
		case VALE_CAVE_LOGS_FROM_VALE_CAVE_KITE_EAST:
			/* Each log already pushed leaves open floor; else its rectangle
			   blocks its metatiles again. */
			if(gmFlagGet(FLAG_VALE_CAVE_WEST_LOG_PUSHED_OFF_LEDGE) != 0) {
				scMapGridFillCollisionRectangle(
					0, 12, 16, 1, 4, METATILE_BEHAVIOR_NORMAL);
				scMapGridFillCollisionRectangle(
					0, 13, 16, 1, 4, METATILE_BEHAVIOR_NORMAL);
			} else {
				scPushedActorBlockFootprint(ACTOR_WEST_LOG);
			}
			if(gmFlagGet(FLAG_VALE_CAVE_EAST_LOG_PUSHED_OVER_EDGE) != 0) {
				scMapGridFillCollisionRectangle(
					2, 16, 16, 1, 4, METATILE_BEHAVIOR_NORMAL);
				scMapGridFillCollisionRectangle(
					0, 16, 16, 1, 4, METATILE_BEHAVIOR_NORMAL);
			} else {
				scPushedActorBlockFootprint(ACTOR_EAST_LOG);
			}
			southLogOnSouthernRow =
				gmFlagGet(FLAG_VALE_CAVE_SOUTH_LOG_ON_SOUTH_ROW);
			if(southLogOnSouthernRow != 0) {
				scMapGridFillCollisionRectangle(
					0, 13, 19, 4, 2, METATILE_BEHAVIOR_NORMAL);
			} else if(gmFlagGet(FLAG_VALE_CAVE_SOUTH_LOG_ON_NORTH_ROW) != 0) {
				scMapGridFillCollisionRectangle(
					0, 13, 15, 4, 2, southLogOnSouthernRow);
				fdMapGridCopyMetatileAttributes(14, 17, 2, 1, 14, 16);
				fdMapGridCopyMetatileAttributes(14, 13, 1, 1, 14, 15);
			} else {
				scPushedActorBlockFootprint(ACTOR_SOUTH_LOG);
				fdActorSetSprPriority(ACTOR_SOUTH_LOG, 3);
			}
			break;
		}
	}
}

/* Stepping onto either end of the riding log makes it dip under Isaac's step.
   Setting it bobbing first starts the dip over if it is already playing. */
static void scValeCaveTouchRidingLog(void)
{
	fdActorSetAnimation(ACTOR_RIDING_LOG, FLOATING_LOG_ANIMATION_FLOAT);
	fdActorSetAnimation(ACTOR_RIDING_LOG, FLOATING_LOG_ANIMATION_SETTLE);
}

#include "shared/field_effect_duration_scripts.c"

#include "shared/pushed_actor_tables.c"
/* clang-format off */

/* The twelve points round Kite's mossy rocks, x then z in pixels, */
/* one for each metatile behavior from */
/* METATILE_BEHAVIOR_VALE_CAVE_FIRST_KITE_ROUTE on. */
Sint32 ValeCaveWalkTargets[][2] = {
	760, 296,
	760, 280,
	776, 280,
	792, 280,
	808, 280,
	824, 280,
	824, 296,
	824, 312,
	808, 312,
	792, 312,
	776, 312,
	760, 312,
};

const Sint32 *FieldEffectDurationScripts[] = {
	FieldEffectShortScript,
	FieldEffectMediumScript,
	FieldEffectLongScript,
};

#include "shared/fallback_entrances.c"

SCENE_ENTRANCE ValeCaveMouthEntrances[] = {
	{ ENTRANCE_DEFAULT, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, 16, 0, 336, 368, 0 },
	{ VALE_CAVE_MOUTH_FROM_VALE_KRADENS_COTTAGE_WEST, CONDITION_ALWAYS, 104, 0, 344, FACING_NORTH, 0, 16, 0, 336, 368, 0 },
	{ VALE_CAVE_MOUTH_FROM_VALE_CAVE_KITE, CONDITION_ALWAYS, 136, 0, 184, FACING_SOUTH, 0, 16, 0, 336, 368, 0 },
	{ VALE_CAVE_MOUTH_FROM_VALE_KRADENS_COTTAGE_EAST, CONDITION_ALWAYS, 136, 0, 376, FACING_NORTH, 0, 16, 0, 336, 368, 0 },
	{ SCENE_TABLE_END },
};

SCENE_ENTRANCE ValeCaveKiteEntrances[] = {
	{ ENTRANCE_DEFAULT, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ VALE_CAVE_KITE_FROM_VALE_CAVE_MOUTH, CONDITION_ALWAYS, 328, 0, 392, FACING_NORTH, 0, 48, 32, 544, 416, 0 },
	{ VALE_CAVE_KITE_WEST_FROM_EAST, CONDITION_ALWAYS, 312, 0, 160, FACING_SOUTH, 0, 48, 32, 544, 416, 0 },
	{ VALE_CAVE_KITE_WEST_FROM_SOUTH, CONDITION_ALWAYS, 120, 0, 120, FACING_SOUTH, 0, 48, 32, 544, 416, 0 },
	{ VALE_CAVE_KITE_FROM_VALE_CAVE_LOGS_WEST, CONDITION_ALWAYS, 504, 0, 136, FACING_SOUTH, 0, 48, 32, 544, 416, 0 },
	{ VALE_CAVE_KITE_EAST_FROM_WEST, CONDITION_ALWAYS, 792, 0, 360, FACING_NORTH, 0, 624, 16, 944, 384, 0 },
	{ VALE_CAVE_KITE_EAST_FROM_SOUTH, CONDITION_ALWAYS, 680, 0, 248, FACING_SOUTH, 0, 624, 16, 944, 384, 0 },
	{ VALE_CAVE_KITE_FROM_VALE_CAVE_LOGS_EAST, CONDITION_ALWAYS, 904, 0, 232, FACING_SOUTH, 0, 624, 16, 944, 384, 0 },
	{ VALE_CAVE_KITE_SOUTH_FROM_WEST, CONDITION_ALWAYS, 408, 0, 872, FACING_NORTH, 0, 312, 472, 704, 896, 0 },
	{ VALE_CAVE_KITE_SOUTH_FROM_EAST, CONDITION_ALWAYS, 488, 0, 744, FACING_NORTH, 0, 312, 472, 704, 896, 0 },
	{ VALE_CAVE_KITE_FROM_VALE_CAVE_LOGS_SOUTH, CONDITION_ALWAYS, 632, 0, 552, FACING_SOUTH, 0, 312, 472, 704, 896, 0 },
	{ SCENE_TABLE_END },
};

SCENE_ENTRANCE ValeCaveLogsEntrances[] = {
	{ ENTRANCE_DEFAULT, CONDITION_ALWAYS, 200, 0, 432, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ VALE_CAVE_LOGS_FROM_VALE_CAVE_KITE_WEST, CONDITION_ALWAYS, 200, 0, 432, FACING_NORTH, 0, 64, 24, 416, 464, 0 },
	{ VALE_CAVE_LOGS_NORTH_FROM_SOUTH, CONDITION_ALWAYS, 344, 0, 120, FACING_SOUTH, 0, 64, 24, 416, 464, 0 },
	{ VALE_CAVE_LOGS_FROM_VALE_CAVE_KITE_EAST, CONDITION_ALWAYS, 120, 0, 264, FACING_NORTH, 0, 64, 24, 416, 464, 0 },
	{ VALE_CAVE_LOGS_FROM_VALE_CAVE_KITE_SOUTH, CONDITION_ALWAYS, 536, 0, 776, FACING_NORTH, 0, 400, 440, 896, 800, 0 },
	{ VALE_CAVE_LOGS_SOUTH_FROM_NORTH, CONDITION_ALWAYS, 840, 0, 776, FACING_NORTH, 0, 400, 440, 896, 800, 0 },
	{ SCENE_TABLE_END },
};

Uint32 ValeCaveExits[] = {
	SCENE_EXITS(ValeCaveMouth),
	SCENE_EXIT(VALE_CAVE_MOUTH_TO_VALE_KRADENS_COTTAGE_WEST, ValeKradensCottage, VALE_KRADENS_COTTAGE_FROM_VALE_CAVE_MOUTH_WEST),
	SCENE_EXIT(VALE_CAVE_MOUTH_TO_VALE_CAVE_KITE, ValeCaveKite, VALE_CAVE_KITE_FROM_VALE_CAVE_MOUTH),
	SCENE_EXIT(VALE_CAVE_MOUTH_TO_VALE_KRADENS_COTTAGE_EAST, ValeKradensCottage, VALE_KRADENS_COTTAGE_FROM_VALE_CAVE_MOUTH_EAST),
	SCENE_EXITS(ValeCaveKite),
	SCENE_EXIT(VALE_CAVE_KITE_TO_VALE_CAVE_MOUTH, ValeCaveMouth, VALE_CAVE_MOUTH_FROM_VALE_CAVE_KITE),
	SCENE_EXIT(VALE_CAVE_KITE_WEST_TO_EAST, ValeCaveKite, VALE_CAVE_KITE_EAST_FROM_WEST),
	SCENE_EXIT(VALE_CAVE_KITE_WEST_TO_SOUTH, ValeCaveKite, VALE_CAVE_KITE_SOUTH_FROM_WEST),
	SCENE_EXIT(VALE_CAVE_KITE_TO_VALE_CAVE_LOGS_WEST, ValeCaveLogs, VALE_CAVE_LOGS_FROM_VALE_CAVE_KITE_WEST),
	SCENE_EXIT(VALE_CAVE_KITE_EAST_TO_WEST, ValeCaveKite, VALE_CAVE_KITE_WEST_FROM_EAST),
	SCENE_EXIT(VALE_CAVE_KITE_EAST_TO_SOUTH, ValeCaveKite, VALE_CAVE_KITE_SOUTH_FROM_EAST),
	SCENE_EXIT(VALE_CAVE_KITE_TO_VALE_CAVE_LOGS_EAST, ValeCaveLogs, VALE_CAVE_LOGS_FROM_VALE_CAVE_KITE_EAST),
	SCENE_EXIT(VALE_CAVE_KITE_SOUTH_TO_WEST, ValeCaveKite, VALE_CAVE_KITE_WEST_FROM_SOUTH),
	SCENE_EXIT(VALE_CAVE_KITE_SOUTH_TO_EAST, ValeCaveKite, VALE_CAVE_KITE_EAST_FROM_SOUTH),
	SCENE_EXIT(VALE_CAVE_KITE_TO_VALE_CAVE_LOGS_SOUTH, ValeCaveLogs, VALE_CAVE_LOGS_FROM_VALE_CAVE_KITE_SOUTH),
	SCENE_EXITS(ValeCaveLogs),
	SCENE_EXIT(VALE_CAVE_LOGS_TO_VALE_CAVE_KITE_WEST, ValeCaveKite, VALE_CAVE_KITE_FROM_VALE_CAVE_LOGS_WEST),
	SCENE_EXIT(VALE_CAVE_LOGS_NORTH_TO_SOUTH, ValeCaveLogs, VALE_CAVE_LOGS_SOUTH_FROM_NORTH),
	SCENE_EXIT(VALE_CAVE_LOGS_TO_VALE_CAVE_KITE_EAST, ValeCaveKite, VALE_CAVE_KITE_FROM_VALE_CAVE_LOGS_EAST),
	SCENE_EXIT(VALE_CAVE_LOGS_TO_VALE_CAVE_KITE_SOUTH, ValeCaveKite, VALE_CAVE_KITE_FROM_VALE_CAVE_LOGS_SOUTH),
	SCENE_EXIT(VALE_CAVE_LOGS_SOUTH_TO_NORTH, ValeCaveLogs, VALE_CAVE_LOGS_NORTH_FROM_SOUTH),
	SCENE_EXITS_END,
};

ACTOR_PLACE ValeCaveMouthActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_CRATES, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		232, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_CRATES, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		232, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_CRATES, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		248, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		192, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		208, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE ValeCaveKiteActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_JUPITER_DJINNI, FLAG_KITE, MOVEMENT_TYPE_STAND,
		792, 0, 232, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_CRATES, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		344, 0, 632, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_CRATES, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		456, 0, 664, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		376, 0, 576, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_CRATES, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		422, 0, 206, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_CRATES, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		440, 0, 206, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		384, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		400, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE ValeCaveLogsActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_JUPITER_DJINNI, FLAG_KITE, MOVEMENT_TYPE_STAND,
		792, 0, 232, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		200, 0, 288, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		312, 0, 288, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		240, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{ SCENE_TABLE_END },
};

CUE ValeCaveCuesOther[] = {
	{ CUE_TABLE_END },
};

CUE ValeCaveMouthCues[] = {
	{ CUE_STEP, VALE_CAVE_MOUTH_TO_VALE_KRADENS_COTTAGE_WEST, CONDITION_ALWAYS, SCRIPT_REF(scValeCaveLeaveRoom) },
	{ CUE_STEP, VALE_CAVE_MOUTH_TO_VALE_CAVE_KITE, CONDITION_ALWAYS, SCRIPT_REF(scValeCaveLeaveRoom) },
	{ CUE_STEP, VALE_CAVE_MOUTH_TO_VALE_KRADENS_COTTAGE_EAST, CONDITION_ALWAYS, SCRIPT_REF(scValeCaveLeaveRoom) },
	{ CUE_TABLE_END },
};

CUE ValeCaveKiteCues[] = {
	{ CUE_EXIT, VALE_CAVE_KITE_TO_VALE_CAVE_MOUTH, CONDITION_ALWAYS, VALE_CAVE_KITE_TO_VALE_CAVE_MOUTH },
	{ CUE_EXIT, VALE_CAVE_KITE_WEST_TO_EAST, CONDITION_ALWAYS, VALE_CAVE_KITE_WEST_TO_EAST },
	{ CUE_EXIT, VALE_CAVE_KITE_WEST_TO_SOUTH, CONDITION_ALWAYS, VALE_CAVE_KITE_WEST_TO_SOUTH },
	{ CUE_EXIT, VALE_CAVE_KITE_TO_VALE_CAVE_LOGS_WEST, CONDITION_ALWAYS, VALE_CAVE_KITE_TO_VALE_CAVE_LOGS_WEST },
	{ CUE_EXIT, VALE_CAVE_KITE_EAST_TO_WEST, CONDITION_ALWAYS, VALE_CAVE_KITE_EAST_TO_WEST },
	{ CUE_EXIT, VALE_CAVE_KITE_EAST_TO_SOUTH, CONDITION_ALWAYS, VALE_CAVE_KITE_EAST_TO_SOUTH },
	{ CUE_EXIT, VALE_CAVE_KITE_TO_VALE_CAVE_LOGS_EAST, CONDITION_ALWAYS, VALE_CAVE_KITE_TO_VALE_CAVE_LOGS_EAST },
	{ CUE_EXIT, VALE_CAVE_KITE_SOUTH_TO_WEST, CONDITION_ALWAYS, VALE_CAVE_KITE_SOUTH_TO_WEST },
	{ CUE_EXIT, VALE_CAVE_KITE_SOUTH_TO_EAST, CONDITION_ALWAYS, VALE_CAVE_KITE_SOUTH_TO_EAST },
	{ CUE_EXIT, VALE_CAVE_KITE_TO_VALE_CAVE_LOGS_SOUTH, CONDITION_ALWAYS, VALE_CAVE_KITE_TO_VALE_CAVE_LOGS_SOUTH },
	{ STEP_CUE_WALKING_INTO(FACING_EAST), METATILE_BEHAVIOR_VALE_CAVE_RIDING_LOG_WEST_END, CONDITION_ALWAYS, SCRIPT_REF(scValeCaveRideLogEast) },
	{ STEP_CUE_WALKING_INTO(FACING_WEST), METATILE_BEHAVIOR_VALE_CAVE_RIDING_LOG_EAST_END, CONDITION_ALWAYS, SCRIPT_REF(scValeCaveRideLogWest) },
	{ STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_BEHAVIOR_VALE_CAVE_RIDING_LOG_EAST_END, CONDITION_ALWAYS, SCRIPT_REF(scValeCaveLeapOffRidingLog) },
	{ CUE_STEP, METATILE_BEHAVIOR_VALE_CAVE_RIDING_LOG_WEST_END, CONDITION_ALWAYS, SCRIPT_REF(scValeCaveTouchRidingLog) },
	{ CUE_STEP, METATILE_BEHAVIOR_VALE_CAVE_RIDING_LOG_EAST_END, CONDITION_ALWAYS, SCRIPT_REF(scValeCaveTouchRidingLog) },
	{ CUE_STEP | STEP_CUE_FLAG_WALKING, METATILE_BEHAVIOR_VALE_CAVE_EAST_CRATES_BANK, FLAG_VALE_CAVE_EAST_CRATES_SUNK, SCRIPT_REF(scValeCavePushEastCratesIntoWater) },
	{ CUE_STEP, METATILE_BEHAVIOR_VALE_CAVE_KITE_RIDGE_TOP, FLAG_KITE, SCRIPT_REF(scValeCaveKiteRunsOff) },
	{ CUE_STEP, METATILE_BEHAVIOR_VALE_CAVE_KITE_RIDGE_FOOT, FLAG_KITE, SCRIPT_REF(scValeCaveKiteRunsBehindRocks) },
	{ CUE_STEP, METATILE_BEHAVIOR_VALE_CAVE_FIRST_KITE_ROUTE, FLAG_KITE, SCRIPT_REF(scValeCaveKiteKeepsAway) },
	{ CUE_STEP, METATILE_BEHAVIOR_VALE_CAVE_FIRST_KITE_ROUTE + 1, FLAG_KITE, SCRIPT_REF(scValeCaveKiteKeepsAway) },
	{ CUE_STEP, METATILE_BEHAVIOR_VALE_CAVE_FIRST_KITE_ROUTE + 2, FLAG_KITE, SCRIPT_REF(scValeCaveKiteKeepsAway) },
	{ CUE_STEP, METATILE_BEHAVIOR_VALE_CAVE_FIRST_KITE_ROUTE + 3, FLAG_KITE, SCRIPT_REF(scValeCaveKiteKeepsAway) },
	{ CUE_STEP, METATILE_BEHAVIOR_VALE_CAVE_FIRST_KITE_ROUTE + 4, FLAG_KITE, SCRIPT_REF(scValeCaveKiteKeepsAway) },
	{ CUE_STEP, METATILE_BEHAVIOR_VALE_CAVE_FIRST_KITE_ROUTE + 5, FLAG_KITE, SCRIPT_REF(scValeCaveKiteKeepsAway) },
	{ CUE_STEP, METATILE_BEHAVIOR_VALE_CAVE_FIRST_KITE_ROUTE + 6, FLAG_KITE, SCRIPT_REF(scValeCaveKiteKeepsAway) },
	{ CUE_STEP, METATILE_BEHAVIOR_VALE_CAVE_FIRST_KITE_ROUTE + 7, FLAG_KITE, SCRIPT_REF(scValeCaveKiteKeepsAway) },
	{ CUE_STEP, METATILE_BEHAVIOR_VALE_CAVE_FIRST_KITE_ROUTE + 8, FLAG_KITE, SCRIPT_REF(scValeCaveKiteKeepsAway) },
	{ CUE_STEP, METATILE_BEHAVIOR_VALE_CAVE_FIRST_KITE_ROUTE + 9, FLAG_KITE, SCRIPT_REF(scValeCaveKiteKeepsAway) },
	{ CUE_STEP, METATILE_BEHAVIOR_VALE_CAVE_FIRST_KITE_ROUTE + 10, FLAG_KITE, SCRIPT_REF(scValeCaveKiteKeepsAway) },
	{ CUE_STEP, METATILE_BEHAVIOR_VALE_CAVE_FIRST_KITE_ROUTE + 11, FLAG_KITE, SCRIPT_REF(scValeCaveKiteKeepsAway) },
	{ CUE_TALK_HALTED, ACTOR_KITE, CONDITION_ALWAYS, SCRIPT_REF(scValeCaveTalkToKite) },
	{ EXAMINE_CUE(EXAMINE_CHEST), METATILE_BEHAVIOR_VALE_CAVE_HALT_GEM_CHEST, FLAG_VALE_CAVE_HALT_GEM, FIND_ITEM(ITEM_HALT_GEM) },
	{ EXAMINE_CUE(EXAMINE_CHEST), METATILE_BEHAVIOR_VALE_CAVE_NUT_CHEST, FLAG_VALE_CAVE_NUT, FIND_ITEM(ITEM_NUT) },
	{ PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, SCRIPT_REF(scValeCaveRevealHiddenTile) },
	{ PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, SCRIPT_REF(scValeCaveHideRevealedTile) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_WEST_CRATES, FLAG_VALE_CAVE_WEST_CRATES_SUNK, SCRIPT_REF(scValeCaveMoveWestCratesIntoWater) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_EAST_CRATES, FLAG_VALE_CAVE_EAST_CRATES_SUNK, SCRIPT_REF(scValeCaveMoveEastCratesIntoWater) },
	{ PSYNERGY_ON_ACTOR(ABILITY_HALT), ACTOR_KITE, CONDITION_ALWAYS, SCRIPT_REF(scValeCaveHaltKite) },
	{ CUE_TABLE_END },
};
/* clang-format on */

CUE ValeCaveLogsCues[] = {
	{CUE_EXIT, VALE_CAVE_LOGS_TO_VALE_CAVE_KITE_WEST, CONDITION_ALWAYS,
		VALE_CAVE_LOGS_TO_VALE_CAVE_KITE_WEST},
	{CUE_EXIT, VALE_CAVE_LOGS_NORTH_TO_SOUTH, CONDITION_ALWAYS,
		VALE_CAVE_LOGS_NORTH_TO_SOUTH},
	{CUE_EXIT, VALE_CAVE_LOGS_TO_VALE_CAVE_KITE_EAST, CONDITION_ALWAYS,
		VALE_CAVE_LOGS_TO_VALE_CAVE_KITE_EAST},
	{CUE_EXIT, VALE_CAVE_LOGS_TO_VALE_CAVE_KITE_SOUTH, CONDITION_ALWAYS,
		VALE_CAVE_LOGS_TO_VALE_CAVE_KITE_SOUTH},
	{CUE_EXIT, VALE_CAVE_LOGS_SOUTH_TO_NORTH, CONDITION_ALWAYS,
		VALE_CAVE_LOGS_SOUTH_TO_NORTH},
	{CUE_STEP | STEP_CUE_FLAG_WALKING,
		METATILE_BEHAVIOR_VALE_CAVE_EAST_LOG_LEDGE, CONDITION_ALWAYS,
		SCRIPT_REF(scValeCavePushLogOverEdge)},
	{STEP_CUE_WALKING_INTO(FACING_EAST),
		METATILE_BEHAVIOR_VALE_CAVE_WEST_LOG_LEDGE, CONDITION_ALWAYS,
		SCRIPT_REF(scValeCavePushLogOffLedge)},
	{CUE_STEP | STEP_CUE_FLAG_WALKING,
		METATILE_BEHAVIOR_VALE_CAVE_SOUTH_LOG_FLOOR, CONDITION_ALWAYS,
		SCRIPT_REF(scValeCavePushLogNorth)},
	{CUE_TABLE_END},
};
