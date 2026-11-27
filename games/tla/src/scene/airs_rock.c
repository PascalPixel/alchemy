/* Air's Rock, the great purple rock in the desert south of Yampi, climbed from
   outside over three screens before the way inside opens.

   On the first, where the party arrives, Felix casts Whirlwind on the Wind
   Stones to blow away the barriers of sand and rock on the paths, then on the
   huge twister-shaped rock at the foot of the cliff, whose great whirlwind
   blows the mud off the wall above it so that it can be climbed. The second is
   the cliff: statues breathe little whirlwinds that carry Felix along and up
   it, pillars pushed or moved over gaps close them, a puddle Frost turns into a
   pillar, and a golden door low on the cliff leads into the rock's great hall.
   The third holds the ledges below the fog, where the party climbs down level
   by level and rides the statues' whirlwinds back up, the fog Whirlwind lifts
   from the wall above, and the top, where Whirlwind on the last twister rock
   calls down lightning on a pile of rocks and opens the way in
   (airs_rock_inside.c). */

#include "field/field_mode.h"
#include "field/script_inline.h"
#include "scene/airs_rock.h"
#include "scene/scene.h"
#include "field/actor_script.h"
#include "graphics/palette_buffers.h"
#include "macros.h"
#include "text/msg_ids.h"
#include "game/game.h"
#include "field/call.h"

/* Set once the first of the cliff's pillars has dropped into its gap, by the
   golden door, metatile (16, 43). */
#define FLAG_AIRS_ROCK_PILLAR_PLACED_FIRST	0x85f
/* As FLAG_AIRS_ROCK_PILLAR_PLACED_FIRST, the second, metatile (45, 45). */
#define FLAG_AIRS_ROCK_PILLAR_PLACED_SECOND 0x860
/* As FLAG_AIRS_ROCK_PILLAR_PLACED_FIRST, the third, metatile (21, 48). */
#define FLAG_AIRS_ROCK_PILLAR_PLACED_THIRD	0x861
/* As FLAG_AIRS_ROCK_PILLAR_PLACED_FIRST, the fourth, metatile (39, 49). */
#define FLAG_AIRS_ROCK_PILLAR_PLACED_FOURTH 0x862
/* As FLAG_AIRS_ROCK_PILLAR_PLACED_FIRST, the fifth, metatile (49, 18). */
#define FLAG_AIRS_ROCK_PILLAR_PLACED_FIFTH	0x887
/* Set once Move has pushed the pillar in the fog into its gap
   (scAirsRockMoveThirdPillar). */
#define FLAG_AIRS_ROCK_THIRD_PILLAR_MOVED	0x879

/* Set while the left and the right of the two pillars on the ledges below the
   fog stand moved one metatile west (scAirsRockMoveFirstPillar,
   scAirsRockMoveSecondPillar), so that they stand there when the party comes
   back. */
#define FLAG_FIRST_PILLAR_MOVED_WEST  FLAG_AREA_TEMP_10
#define FLAG_SECOND_PILLAR_MOVED_WEST FLAG_AREA_TEMP_11
/* Set while Frost holds the cliff's puddle frozen into a pillar. */
#define FLAG_PUDDLE_FROZEN			  FLAG_SCENE_TEMP_00

/* Story flags: the eight barriers the Wind Stones' gusts blow away, in the
   order of AirsRock_GustTargets, then the mud blown from the wall, the fog
   cleared and the way to the top opened. */
#define FLAG_AIRS_ROCK_FIRST_BARRIER_BLOWN 0x850
#define FLAG_AIRS_ROCK_WALL_CLEARED		   0x858
#define FLAG_AIRS_ROCK_FOG_CLEARED		   0x87a
#define FLAG_AIRS_ROCK_TOP_OPENED		   0x87b

/* What these rooms mark as the collision code (METATILE_CODE) of a metatile on
   the second layer. */
typedef enum AIRS_ROCK_METATILE_MARK {
	/* A barrier a gust blows away is marked this plus its kind, a row of
	   AirsRock_GustTargets; the metatile beyond it holds the barrier's number
	   plus one, which picks its flag. */
	METATILE_MARK_FIRST_GUST_TARGET = 30,
	/* A statue's whirlwind fades here. */
	METATILE_MARK_WHIRLWIND_END = 212,
	/* A gust stops here with nothing to blow, and a whirlwind's rider cannot
	   land here. */
	METATILE_MARK_BLOCKED = 255
} AIRS_ROCK_METATILE_MARK;

/* The kinds of barrier a gust sweeps away, each a row of AirsRock_GustTargets,
   by the way the gust crosses it: a bank of sand across an east-west path the
   gust blows north or south through a metatile at a time, or a mound across a
   north-south path it blows west through a column at a time. */
typedef enum AIRS_ROCK_GUST_BARRIER {
	GUST_BARRIER_BLOWN_NORTH = 0,
	GUST_BARRIER_BLOWN_SOUTH = 2,
	GUST_BARRIER_BLOWN_WEST
} AIRS_ROCK_GUST_BARRIER;

/* Clears a barrier a gust blew away on an earlier visit: a mound three
   metatiles wide or a bank three metatiles tall, (x, z) its north-west
   metatile. */
#define CLEAR_SAND_MOUND(x, z) \
	(fdMapGridCopyMetatilesTo(64, 0, (x) + 64, (z) - 1, 3, 3), \
		fdMapGridCopyMetatileAttributes(64, 0, 3, 2, (x) + 64, z), \
		fdMapGridCopyMetatileAttributes(64, 0, 3, 2, x, z))
#define CLEAR_SAND_BANK(x, z) \
	(fdMapGridCopyMetatilesTo(64, 0, (x) + 64, (z) - 1, 1, 4), \
		fdMapGridCopyMetatileAttributes(64, 0, 1, 3, (x) + 64, z), \
		fdMapGridCopyMetatileAttributes(64, 0, 1, 3, x, z))

/* The Storm Brand chest's room, numbering
   AirsRock_StormBrandChestPlacements. */
typedef enum AIRS_ROCK_STORM_BRAND_CHEST_ACTOR_ID {
	/* The huge twister-shaped rock Whirlwind clears the wall's mud with. */
	ACTOR_TWISTER_ROCK_BY_THE_WALL = ACTOR_FIRST_PLACED,
	/* The five Wind Stones, whose gusts Whirlwind strengthens. */
	ACTOR_WIND_STONE_A,
	ACTOR_WIND_STONE_B,
	ACTOR_WIND_STONE_C,
	ACTOR_WIND_STONE_D,
	ACTOR_WIND_STONE_E
} AIRS_ROCK_STORM_BRAND_CHEST_ACTOR_ID;

/* The slopes by the way into the rock, numbering
   AirsRock_EntrancePlacements. */
typedef enum AIRS_ROCK_ENTRANCE_ACTOR_ID {
	/* The four statues on the slopes that breathe whirlwinds. */
	ACTOR_FIRST_SLOPE_STATUE = ACTOR_FIRST_PLACED,
	ACTOR_SECOND_SLOPE_STATUE,
	ACTOR_THIRD_SLOPE_STATUE,
	ACTOR_FOURTH_SLOPE_STATUE,
	/* The pillars that drop into the cliff's gaps, each with its
	   FLAG_AIRS_ROCK_PILLAR_PLACED_ flag. */
	ACTOR_FIRST_LEDGE_PILLAR,
	ACTOR_SECOND_LEDGE_PILLAR,
	/* The puddle Frost freezes into a pillar. */
	ACTOR_PUDDLE,
	ACTOR_THIRD_LEDGE_PILLAR,
	ACTOR_FOURTH_LEDGE_PILLAR,
	/* The four statues hung on the cliff that breathe whirlwinds. */
	ACTOR_FIRST_CLIFF_STATUE,
	ACTOR_SECOND_CLIFF_STATUE,
	ACTOR_THIRD_CLIFF_STATUE,
	ACTOR_FOURTH_CLIFF_STATUE,
	/* The statue exhaling little whirlwinds to the left. */
	ACTOR_BREATHING_STATUE,
	ACTOR_FIFTH_LEDGE_PILLAR
} AIRS_ROCK_ENTRANCE_ACTOR_ID;

/* The Fujin Shield chest's room, numbering
   AirsRock_FujinShieldChestPlacements. */
typedef enum AIRS_ROCK_FUJIN_SHIELD_CHEST_ACTOR_ID {
	/* The statues whose whirlwinds carry the party up the cliff. */
	ACTOR_FIRST_BREATHING_STATUE = ACTOR_FIRST_PLACED,
	ACTOR_SECOND_BREATHING_STATUE,
	/* The left and right of the two pillars on the ledges below the fog. */
	ACTOR_FIRST_PILLAR,
	ACTOR_SECOND_PILLAR,
	/* The twister rock whose whirlwind clears the fog. */
	ACTOR_TWISTER_ROCK_IN_THE_FOG,
	ACTOR_THIRD_PILLAR,
	/* The twister rock whose whirlwind opens the way to the top. */
	ACTOR_TWISTER_ROCK_AT_THE_TOP
} AIRS_ROCK_FUJIN_SHIELD_CHEST_ACTOR_ID;

/* The slopes' own ways in, where the leader climbs between the lower slopes and
   the cliff above them. */
typedef enum AIRS_ROCK_ENTRANCE_ENTRANCE {
	/* At the top of the lower slopes' wall, climbing down from the cliff. */
	ENTRANCE_DOWN_FROM_THE_CLIFF_WEST = 5,
	ENTRANCE_DOWN_FROM_THE_CLIFF_EAST,
	/* At the foot of the cliff, climbing up from the lower slopes. */
	ENTRANCE_UP_THE_CLIFF_WEST = 10,
	ENTRANCE_UP_THE_CLIFF_EAST
} AIRS_ROCK_ENTRANCE_ENTRANCE;

/* The Fujin Shield chest's room's own ways in: off its ledges onto the paths
   west of them and back, and up and down its wall into and out of the fog. */
typedef enum AIRS_ROCK_FUJIN_SHIELD_CHEST_ENTRANCE {
	/* On the ledges, back from the lower and the upper path. */
	ENTRANCE_LEDGES_FROM_LOWER_PATH = 2,
	ENTRANCE_LEDGES_FROM_UPPER_PATH,
	/* On the paths, from the ledges. */
	ENTRANCE_LOWER_PATH,
	ENTRANCE_UPPER_PATH,
	/* On the wall below the fog, climbing down out of it. */
	ENTRANCE_BELOW_THE_FOG_WEST,
	ENTRANCE_BELOW_THE_FOG_EAST,
	/* On the wall in the fog, climbing up into it; while the fog lasts, the top
	   of the wall leads back here. */
	ENTRANCE_INTO_THE_FOG_WEST,
	ENTRANCE_INTO_THE_FOG_EAST,
	/* At the top of the wall in the fog, climbing down from above it. */
	ENTRANCE_DOWN_INTO_THE_FOG_WEST,
	ENTRANCE_DOWN_INTO_THE_FOG_EAST,
	/* On the wall above the fog, climbing up out of it once it is cleared. */
	ENTRANCE_ABOVE_THE_FOG_WEST,
	ENTRANCE_ABOVE_THE_FOG_EAST,
	/* Unused: no exit leads here. The way to the top opens at once
	   (scAirsRockWhirlwindOpensTheTop). */
	ENTRANCE_OPEN_THE_TOP = 99
} AIRS_ROCK_FUJIN_SHIELD_CHEST_ENTRANCE;

#include "../field/actor_set_sprite_priority.c"

#include "../field/actor_create_silhouette.c"

#include "shared/scene_create_actor.h"

#include "../field/field_effect_move.c"

#define EFFECT_SET_MODE	  fdActorSelectAnimation
#define EFFECT_SET_SCRIPT fdActorStartScript
#define EFFECT_SPRITE_PRIORITY
#include "../field/field_effect_spawn.c"

/* Clears the actor's sprite flags, as a script's call; always 0. */
Sint32 scAirsRockClearScriptedActorSprFlags(Sint32 actorId)
{
	fdActorSetSprFlags(actorId, 0);
	return 0;
}

#include "../field/map_metatile_get.c"

#include "shared/map_metatile_set.c"

#include "../field/actor_wait_until_landed.c"

#undef EFFECT_SET_SCRIPT

#include "shared/blocks_mark_metatiles.c"

/* Frees the metatiles ahead of and behind a block that scBlocksMarkMetatiles
   blocked, and clears its mark. */
void scBlocksUnmarkMetatiles(ACTOR *block)
{
	MAP_STATE *map;

	if(block->sceneByte != 0) {
		map = Ram_HeapSlots->mapGrid;
		if(map != NULL) {
			MAP_METATILE **metatiles = &map->layers[2].metatiles;
			MAP_METATILE  *metatile = *metatiles;
			Sint32		   x = FIELD_METATILE(block->x.fixed);
			Sint32 row = FIELD_METATILE(block->z.fixed - block->y.fixed);

			metatile += x + ((row + 1) << 7);
			metatile->collisionCode = 0;
			metatile = *metatiles;
			metatile += x + ((row - 1) << 7);
			metatile->collisionCode = 0;
			block->sceneByte = 0;
		}
	}
}

const SCENE_ENTRANCE *scGetEntrances(void)
{
	return SceneEntrances;
}

extern SCENE_REGION AirsRock_FujinShieldChestRegions[];

const SCENE_REGION *scGetExitRegions(void)
{
	Sint32 scene = MainRoutineStateWork.next;

	if(scene == SCENE_AIRS_ROCK_STORM_BRAND_CHEST)
		return AirsRock_StormBrandChestRegions;
	if(scene == SCENE_AIRS_ROCK_ENTRANCE)
		return AirsRock_EntranceRegions;
	return AirsRock_FujinShieldChestRegions;
}

const Uint32 *scGetExits(void)
{
	return SceneExits;
}

const ACTOR_PLACE *scGetActorPlacements(void)
{
	Sint32 scene = MainRoutineStateWork.next;

	if(scene == SCENE_AIRS_ROCK_STORM_BRAND_CHEST)
		return AirsRock_StormBrandChestPlacements;
	if(scene == SCENE_AIRS_ROCK_ENTRANCE)
		return AirsRock_EntrancePlacements;
	return AirsRock_FujinShieldChestPlacements;
}

/* Felix reads a Wind Stone on the first screen: "Beware the Wind Stones." and
   its warning that the stones magnify the wind. */
void scAirsRockExamineWindStone(void)
{
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	/* "Beware the Wind Stones." "The stones magnify the power of the wind. Use
	   caution when approaching Air's Rock." */
	mnMessageShowCentered((Sint32)MsgFieldWindStoneWarning, 1);
	fdScriptEnd();
}

/* Felix walks north into the golden door low on the cliff, beside the first
   pillar's gap (metatile (19, 43)): the door slides open, he walks in and the
   screen wipes to the great hall inside Air's Rock. */
void scAirsRockEnterTheRock(void)
{
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdScriptPlaySnd(SE_DOOR_OPEN);
	fdMapGridCopyMetatilesTo(64, 64, 83, 35, 1, 2);
	fdTilesetAnimResume(0);
	syFieldScriptWait(45);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.5);
	fdActorWalkTo(MainRoutineStateWork.leaderActorId, 312, 680);
	syFieldScriptWait(5);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionFrames();
	scFieldActorWaitForMoveById(MainRoutineStateWork.leaderActorId);
	fdScriptPlaySnd(SE_MAP_EXIT);
	syFieldScriptSetExitRequest(AIRS_ROCK_ENTRANCE_TO_AIRS_ROCK_INSIDE_HALL);
	fdScriptEnd();
}

/* Felix casts Move on the left of the two pillars on the ledges below the fog.
   Nothing more happens on screen than the move itself; the game remembers
   whether it now stands one metatile west of where it started (column 31), so
   that it is still there when the party comes back. */
void scAirsRockMoveFirstPillar(void)
{
	ACTOR *actor;

	actor = fdActorGet(ACTOR_FIRST_PILLAR);
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	if(FIELD_METATILE(actor->x.fixed) == 31) {
		fdScriptSetFlag(FLAG_FIRST_PILLAR_MOVED_WEST);
	} else {
		fdScriptClearFlag(FLAG_FIRST_PILLAR_MOVED_WEST);
	}
	fdScriptEnd();
}

/* Felix casts Move on the right of the two pillars on the ledges below the fog,
   from across the gap east of them. With the left pillar moved one metatile
   west, the hand pulls this one west beside it, and the metatile where it
   stood, (33, 54), is open to walk on: Felix jumps the gap west onto it, and
   stepping south off it he drops to the ledge below (scAirsRockDropOffLedge),
   where a whirlwind carries him up past the pillars' ledge to a ledge above
   (scAirsRockRideWhirlwind). The game remembers the pillar there; anywhere else
   that metatile stays closed.

   The ledge's metatile is changed in the copy of it kept for the level it is on
   (scAirsRockSetLedgeMetatilesByLevel) and, while Felix stands on that level,
   in the map itself. */
void scAirsRockMoveSecondPillar(void)
{
	ACTOR *actor;
	ACTOR *leader;
	Sint32 value;

	actor = fdActorGet(ACTOR_SECOND_PILLAR);
	value = fdMapMetatileGetAttributes(3, 70, 67);
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	if(FIELD_METATILE(actor->x.fixed) == 32) {
		fdScriptSetFlag(FLAG_SECOND_PILLAR_MOVED_WEST);
		value &= METATILE_ATTRIBUTE_BITS;
		scMapMetatileSetAttributes(4, 70, 67, value);
		scMapMetatileSetAttributes(2, 33, 54, value);
	} else {
		fdScriptClearFlag(FLAG_SECOND_PILLAR_MOVED_WEST);
		value = METATILE_ATTRIBUTES(
			METATILE_MARK_BLOCKED, value & METATILE_ATTRIBUTE_BITS);
		scMapMetatileSetAttributes(4, 70, 67, value);
		scMapMetatileSetAttributes(2, 33, 54, value);
	}
	leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	if(METATILE_HEIGHT_STEPS(leader->y.fixed) == 26)
		fdMapGridCopyMetatileAttributes(70, 67, 1, 1, 33, 54);
	fdScriptEnd();
}

/* Felix casts Move on the pillar in the fog: pushed one metatile east into the
   gap (metatile (11, 17)), it stays there for good, a way across the gap and a
   shortcut back. */
void scAirsRockMoveThirdPillar(void)
{
	ACTOR *actor;

	actor = fdActorGet(ACTOR_THIRD_PILLAR);
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	if(FIELD_HALF_METATILE(actor->x.fixed) == 23) {
		fdScriptSetFlag(FLAG_AIRS_ROCK_THIRD_PILLAR_MOVED);
	}
	fdScriptEnd();
}

/* Felix walks south off the edge of the ledges below the fog, in front of the
   two pillars (metatile (33, 55), on the second level): he hops over the edge,
   lowers himself a little way down the rock face, lets go and drops to the
   ledge below. */
void scAirsRockDropOffLedge(void)
{
	ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	FIXED  z = leader->z.fixed;
	Uint32 i;

	fdScriptPlaySnd(SOUND_FADE_OUT_ACTION_SE);
	fdActorSelectAnimation(leader, ANIM_LEADER_SPRING);
	syWait(10);
	fdActorSelectAnimation(leader, ANIM_STAND);
	fdActorSetSprFlags(leader, 0);
	leader->motionFlg |= ACTOR_FALLS;
	leader->velY = toFIXED(4);
	scFieldActorSetMoveTargetFX(
		leader, leader->x.fixed, leader->y.fixed, z + toFIXED(12));
	syWait(6);
	fdScriptPlaySnd(SE_FLOOR_SWITCH);
	fdActorFaceDirection(
		MainRoutineStateWork.leaderActorId, FACING_SOUTH_SOUTHWEST, 0);
	leader->motionFlg = 0;
	for(i = 0; i < 14; i++) {
		leader->y.fixed -= toFIXED(2);
		leader->tgtY = leader->y.fixed;
		syWait(1);
	}
	leader->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	leader->velY = toFIXED(3);
	scFieldActorSetMoveTargetFX(
		leader, leader->x.fixed, leader->y.fixed, z + toFIXED(16));
	scFieldActorWaitForMove(leader);
	fdActorSetSprFlags(leader, 1);
	fdScriptPlaySnd(SOUND_FADE_OUT_ACTION_SE);
	fdScriptNoOperation();
}

const CUE *scGetCues(void)
{
	Sint32 scene = MainRoutineStateWork.next;

	if(scene == SCENE_AIRS_ROCK_STORM_BRAND_CHEST)
		return AirsRock_StormBrandChestCues;
	if(scene == SCENE_AIRS_ROCK_ENTRANCE)
		return AirsRock_EntranceCues;
	return AirsRock_FujinShieldChestCues;
}

/* Nothing visible happens of its own as Felix starts casting Whirlwind on a
   Wind Stone or a twister rock; the game clears what is pending so that the
   cast can go on to the stone's or rock's own whirlwind
   (scAirsRockTakeOverWhirlwind). */
void scAirsRockStartWhirlwind(void)
{
	fdScriptEndLastingPsynergy(0);
}

/* Nothing visible happens of its own as Felix casts Whirlwind on a Wind Stone
   or a twister rock; the game tells Whirlwind to leave out its usual little
   whirlwind, so that what follows is the stone's or the rock's own
   (scAirsRockBlowGust and the rest). */
void scAirsRockTakeOverWhirlwind(void)
{
	((FIELD_EFFECT_STATE *)Ram_HeapSlots->fieldEffState)->sceneVariant = 1;
}

/* The turn a gust's spark makes each frame: a lap every 32 frames. */
#define GUST_LAP_STEP (TRIG_TURN / 32)

/* A gust's spark: circles the centre kept in bounce and sceneWord, a lap every
   32 frames, its radius growing as it rises. */
void scAirsRockSpiralGust(ACTOR *actor)
{
	Sint32 radius = actor->y.fixed;
	Sint32 step = actor->speedLimit;

	radius >>= 1;
	radius += toFIXED(16);
	actor->x.fixed =
		actor->bounce + syMulFX(syCos((step & 0xff) * GUST_LAP_STEP), radius);
	actor->z.fixed = actor->sceneWord +
		syMulFX(syMathSin((actor->speedLimit & 0xff) * GUST_LAP_STEP), radius) /
			3;
	actor->y.fixed += actor->gravity;
	actor->speedLimit++;
}

/* The same circle on a smaller radius, twice as wide, blinking blue every other
   two frames. */
void scAirsRockSpiralSpark(ACTOR *actor)
{
	Sint32 radius = actor->y.fixed;
	Sint32 step = actor->speedLimit;

	radius >>= 2;
	radius += toFIXED(12);
	actor->x.fixed = actor->bounce +
		syMulFX(syCos((step & 0xff) * GUST_LAP_STEP), radius) * 2;
	actor->z.fixed = actor->sceneWord +
		syMulFX(syMathSin((actor->speedLimit & 0xff) * GUST_LAP_STEP), radius);
	actor->y.fixed += actor->gravity;
	actor->speedLimit++;
	scFieldActorSetSprCol(
		actor, ((FrameCount & 2) >> 1) * SPRITE_COLOR_BLUE_TINT);
}

/* A Whirlwind target's update while the gust gathers: it flickers to the colour
   mode in sceneCnt (a SPR_COL_MODE), at the rate sceneByte sets. */
void scAirsRockFlickerTarget(ACTOR *actor)
{
	Uint32 on = ((FrameCount & 3) >> actor->sceneByte) & 1;

	scFieldActorSetSprCol(actor, (Uint8)(on * actor->sceneCnt));
}

/* The Whirlwind gust in Air's Rock. */

extern Sint16 AirsRock_GustTargets[][4];

/* Felix casts Whirlwind on one of the five Wind Stones on the first screen: a
   whirlwind rises from the stone, which flickers as it grows, then blows off
   the way Felix faces, sparkling as it goes. If a barrier of sand or rocks
   stands in its path it blows it away for good, opening the path; with nothing
   in its path it fades at the next wall with a whoosh.

   The gust looks ahead along the second layer for the first metatile with a
   mark: a wall (METATILE_MARK_BLOCKED) or a barrier
   (METATILE_MARK_FIRST_GUST_TARGET plus its kind). It travels to that metatile,
   sweeps a barrier's metatiles away as it blows three metatiles on, and sets
   the flag counted from FLAG_AIRS_ROCK_FIRST_BARRIER_BLOWN by the metatile
   beyond it. */
void scAirsRockBlowGust(void)
{
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	FIELD_EFFECT_STATE *effect = Ram_HeapSlots->fieldEffState;
	ACTOR			   *gust, *spark;
	VECTOR3				velocity;
	EFFECT_OPTIONS		glints, dust;
	CELL				delta, metatile;
	Sint32				newZ, newX, flag = 0, steps = 0, hit, angle, i;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	sySndPlay(SE_RUSH);
	gust = scFieldActorCreate(
		SPRITE_WHIRLWIND, leader->x.fixed, leader->y.fixed, leader->z.fixed);
	fdActorSelectAnimation(gust, 2);
	fdActorSetSprFlags(gust, 0);
	gust->scaleX = toFIXED(17) / 10;
	gust->scaleY = toFIXED(1) / 5;
	gust->motionFlg = 0;
	((FIELD_SPR *)gust->spr)->pri = ((FIELD_SPR *)effect->tgt->spr)->pri;
	fdActorSetMoveTarget(gust, effect->tgt->x.fixed, effect->tgt->y.fixed,
		effect->tgt->z.fixed - toFIXED(1));
	while(gust->scaleY < toFIXED(13) / 10) {
		gust->scaleY += toFIXED(1) / 10;
		fdScriptWait(1);
	}
	fdActorWaitForMove(gust);
	gust->z.fixed = effect->tgt->z.fixed - toFIXED(1);
	while(gust->scaleY < toFIXED(18) / 10) {
		if((FrameCount & 3) == 0) {
			spark = scFieldActorCreate(
				SPRITE_PARTICLE, effect->tgt->x.fixed, 0, effect->tgt->z.fixed);
			spark->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
			spark->motionFlg = 0;
			spark->bounce = spark->x.fixed;
			spark->gravity = toFIXED(4) / 5;
			spark->sceneWord = spark->z.fixed;
			spark->speedLimit = syRandom() & 0xffff;
			fdActorSelectAnimation(spark, 7);
			fdActorStartScript(spark, FieldEffect_MediumScript);
			fdActorSetSprFlags(spark, 0);
			fdActorSetSprCol(spark, SPRITE_COLOR_RBG);
			spark->update = scAirsRockSpiralGust;
		}
		if(FrameCount & 1) {
			gust->scaleX += toFIXED(1) / 100;
			gust->scaleY += toFIXED(1) / 100;
			fdActorSetSprCol(effect->tgt, SPRITE_COLOR_HIDDEN);
		} else {
			fdActorSetSprCol(effect->tgt, SPRITE_COLOR_GBR);
		}
		fdScriptWait(1);
	}
	fdActorSetSprCol(effect->tgt, SPRITE_COLOR_NORMAL);
	sySndPlay(SE_WHOOSH);
	metatile.x = FIELD_METATILE(effect->tgt->x.fixed);
	metatile.z = FIELD_METATILE(effect->tgt->z.fixed);
	/* The leader's facing rounded to the nearest of east, south, west and
	   north. */
	angle = FACING_QUARTER(leader->facing + DEGtoANG(45));
	delta.x = FIXEDtoINT(syCos(angle));
	delta.z = FIXEDtoINT(sySin(angle));
	do {
		hit = fdMapMetatileGetAttributes(2, metatile.x, metatile.z);
		hit = METATILE_CODE(hit);
		metatile.x += delta.x;
		metatile.z += delta.z;
		steps += 2;
	} while(hit == 0);
	flag = fdMapMetatileGetAttributes(2, metatile.x, metatile.z);
	flag = METATILE_CODE(flag);
	flag -= 1;
	steps -= 2;
	newX = effect->tgt->x.part[0].integer + ((steps * delta.x) << 3);
	newZ = effect->tgt->z.part[0].integer + ((steps * delta.z) << 3);
	((FIELD_SPR *)gust->spr)->pri = 1;
	/* metatile takes the gust's target in pixels: the ROM stores both words in
	   metatile's slots before the move, though nothing reads them (a later arm
	   stores its own target there and reads it back). */
	metatile.x = newX;
	metatile.z = newZ;
	gust->speedLimit = toFIXED(4);
	gust->acceleration = toFIXED(4);
	fdActorSetMoveTarget(
		gust, INTtoFIXED(metatile.x), gust->y.fixed, INTtoFIXED(metatile.z));
	fdCameraSetSpeedAndAcceleration(5, 5);
	fdCameraMoveToFX(INTtoFIXED(newX), -1, toFIXED(newZ), 1);
	while(fdActorHasArrived(gust) == 0) {
		if((FrameCount & 1) == 0) {
			glints.pri = 1;
			glints.type = SPRITE_GLINT;
			glints.pal = 2;
			velocity.x = syCos((FrameCount & 0x1ff) * FACING_STEP) << 1;
			velocity.y = gust->y.fixed + toFIXED(syRandom() & 31);
			velocity.z = sySin((FrameCount & 0x1ff) * FACING_STEP);
			scFieldEffectSpawn(gust->x.fixed, velocity.y, gust->z.fixed,
				velocity.x, 0, velocity.z,
				EFFECT_USE_TYPE | EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
				&glints);
		}
		fdScriptWait(1);
	}
	if(hit == METATILE_MARK_BLOCKED) {
		sySndPlay(SE_WHOOSH);
	} else {
		metatile.x = gust->x.part[0].integer + delta.x * 48;
		metatile.z = gust->z.part[0].integer + delta.z * 48;
		gust->speedLimit = toFIXED(1) / 2;
		gust->acceleration = toFIXED(1) / 2;
		fdActorSetMoveTarget(gust, INTtoFIXED(metatile.x), gust->y.fixed,
			INTtoFIXED(metatile.z));
		sySndPlay(SE_GUST);
		hit -= METATILE_MARK_FIRST_GUST_TARGET;
		while(fdActorHasArrived(gust) == 0) {
			if((FrameCount & 1) == 0) {
				dust.pri = 1;
				dust.startScaleX = dust.startScaleY =
					(syRandom() & 0xffff) + toFIXED(1);
				velocity.x = syCos((FrameCount & 0x1ff) * FACING_STEP) << 1;
				velocity.y = toFIXED(1);
				velocity.z = sySin((FrameCount & 0x1ff) * FACING_STEP);
				scFieldEffectSpawn(gust->x.fixed, gust->y.fixed, gust->z.fixed,
					velocity.x, velocity.y, velocity.z,
					EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY, &dust);
			}
			if((FrameCount & 3) == 0) {
				metatile.x = AirsRock_GustTargets[hit][2] +
					FIELD_METATILE(gust->x.fixed);
				metatile.z = AirsRock_GustTargets[hit][3] +
					FIELD_METATILE(gust->z.fixed);
				if(hit & 1) {
					fdMapGridCopyMetatilesTo(AirsRock_GustTargets[hit][0],
						AirsRock_GustTargets[hit][1], metatile.x,
						metatile.z - 1, 1, 3);
					fdMapGridCopyMetatileAttributes(
						AirsRock_GustTargets[hit][0],
						AirsRock_GustTargets[hit][1], 1, 2, metatile.x,
						metatile.z);
					fdMapGridCopyMetatileAttributes(
						AirsRock_GustTargets[hit][0],
						AirsRock_GustTargets[hit][1], 1, 2,
						FIELD_METATILE(gust->x.fixed),
						FIELD_METATILE(gust->z.fixed));
				} else {
					fdMapGridCopyMetatilesTo(AirsRock_GustTargets[hit][0],
						AirsRock_GustTargets[hit][1], metatile.x,
						metatile.z - (hit >> 1), 1, 1);
					fdMapGridCopyMetatileAttributes(
						AirsRock_GustTargets[hit][0],
						AirsRock_GustTargets[hit][1], 1, 1, metatile.x,
						metatile.z);
					fdMapGridCopyMetatileAttributes(
						AirsRock_GustTargets[hit][0],
						AirsRock_GustTargets[hit][1], 1, 1,
						FIELD_METATILE(gust->x.fixed),
						FIELD_METATILE(gust->z.fixed));
				}
				fdMapGridCenterOnFollowedPoint();
			}
			fdScriptWait(1);
		}
		/* Blown south, the sweep clears the row behind the gust, so the row it
		   stops on is cleared last. */
		if(hit == GUST_BARRIER_BLOWN_SOUTH) {
			fdMapGridCopyMetatilesTo(AirsRock_GustTargets[2][0],
				AirsRock_GustTargets[2][1], metatile.x, metatile.z, 1, 1);
			fdMapGridCenterOnFollowedPoint();
		}
		sySndPlay(SE_STOP_EXIT_SE);
		fdScriptSetFlag(flag + FLAG_AIRS_ROCK_FIRST_BARRIER_BLOWN);
	}
	gust->acceleration = 0;
	gust->speedLimit = 0;
	gust->tgtZ = 0;
	gust->tgtY = 0;
	gust->tgtX = 0;
	for(i = 0; i != 8; i++) {
		gust->scaleX -= toFIXED(3) / 20;
		gust->scaleY -= toFIXED(1) / 4;
		gust->y.fixed += toFIXED(7) / 2;
		fdScriptWait(1);
	}
	fdActorRelease(gust);
	fdScriptWait(30);
	fdCameraMoveToActorById(MainRoutineStateWork.leaderActorId, 1);
	fdCameraWaitForArrival();
	fdScriptEnd();
}

/* The line scroll of the great whirlwind or the fog, a halfword a line; three a
   line while the fog lifts. */
Sint16 AirsRock_WaveLines[DISPLAY_HEIGHT * 3] BSS_DATA ALIGNED(16);

/* The great whirlwind's sway: BG0's line scroll, by HBlank DMA. */
#define WAVE_SCROLL		  scAirsRockScrollWave
#define WAVE_SCROLL_LINES AirsRock_WaveLines
#include "../field/field_effect_wave_scroll.c"

/* A cloud of the wall's mud swept round the great whirlwind: west of x 320 it
   speeds up eastward and grows, past it westward and shrinks, passing in front
   of the whirlwind going east and behind it going west, and it spins. */
void scAirsRockSwirlDust(ACTOR *actor)
{
	FIXED scale;

	actor->y.fixed += actor->gravity;
	actor->x.fixed += actor->bounce;
	actor->z.fixed += actor->sceneWord;
	if(FIELD_HALF_METATILE(actor->x.fixed) <= 39) {
		actor->bounce += toFIXED(3) / 2;
		scale = actor->scaleX + (toFIXED(1) / 20);
	} else {
		actor->bounce -= toFIXED(3) / 2;
		scale = actor->scaleX - (toFIXED(1) / 50);
	}
	actor->scaleX = scale;
	actor->scaleY = scale;
	if(actor->bounce > 0)
		fdSetActorSprPriority(actor, 1);
	else
		fdSetActorSprPriority(actor, 2);
	((FIELD_SPR *)actor->spr)->rotation += TRIG_TURN * 3 / 32;
}

/* The bytes of one row of the whirlwind's tiles. */
#define WHIRLWIND_TILE_ROW_SIZE	 (WHIRLWIND_ROW_TILES * TILE_SIZE_4BPP)
/* Where each row of the whirlwind's map is copied into its screen block row, in
   bytes: the word copy drops the odd bytes, so the picture's rows start at
   column 6. */
#define WHIRLWIND_MAP_ROW_OFFSET 15

/* Felix casts Whirlwind on the huge twister-shaped rock below the cliff on the
   first screen: it glows, the view pans north to the wall caked in mud, and a
   great whirlwind rises and blows the mud away, baring the holds that climb it.
   The rock turns grey, spent. */
void scAirsRockWhirlwindClearsWall(void)
{
	MAIN_ROUTINE_STATE *state;
	Uint16			   *buffer = syHeapAlloc(2 * WHIRLWIND_TILE_ROW_SIZE);
	Uint16			   *metatile;
	ACTOR			   *spark;
	Uint32				i;
	FIXED				x;

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	REG_DISPCNT &= ~DISPCNT_BG0_ON;
	syDecompressTaggedLZ(
		syResourceGetTableEntry(RESOURCE_WHIRLWIND_TILES), buffer);
	syDMAXCopy(buffer, BG_CHAR_BLOCK(3) + WHIRLWIND_TOP_TILE * TILE_SIZE_4BPP,
		DMA_COPY_WORDS(WHIRLWIND_TILE_ROW_SIZE / 4), REG_DMA3);
	syDMAXCopy((Uint8 *)buffer + WHIRLWIND_TILE_ROW_SIZE,
		BG_CHAR_BLOCK(3) + WHIRLWIND_BOTTOM_TILE * TILE_SIZE_4BPP,
		DMA_COPY_WORDS(WHIRLWIND_TILE_ROW_SIZE / 4), REG_DMA3);
	syHeapRelease(buffer);
	scFieldActorSetSprPriority(MainRoutineStateWork.leaderActorId, 1);
	scFieldActorSetSprPriority(ACTOR_TWISTER_ROCK_BY_THE_WALL, 1);
	syIOWrite16(BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(3) | BGCNT_SCREEN_BASE(4),
		&REG_BG0CNT);
	REG_BG1CNT &= ~BGCNT_PRIORITY(3);
	REG_BG1CNT |= BGCNT_PRIORITY(2);
	DMA_FILL16(WHIRLWIND_FILL_TILE, BG_SCREEN_BLOCK(4), 1280);
	buffer = syHeapAlloc(
		WHIRLWIND_MAP_HEIGHT * WHIRLWIND_MAP_WIDTH * sizeof(Uint16));
	syDecompressTaggedLZ(
		syResourceGetTableEntry(RESOURCE_WHIRLWIND_MAP), buffer);
	metatile = buffer;
	for(i = 0; i != WHIRLWIND_MAP_HEIGHT * WHIRLWIND_MAP_WIDTH; i++) {
		*metatile -= WHIRLWIND_MAP_FIRST_TILE - WHIRLWIND_TOP_TILE;
		metatile++;
	}
	metatile = buffer;
	for(i = 0; i != WHIRLWIND_MAP_HEIGHT; i++) {
		syDMAXCopy(metatile,
			BG_SCREEN_BLOCK(4) + WHIRLWIND_MAP_ROW_OFFSET +
				i * BG_SCREEN_WIDTH * sizeof(Uint16),
			DMA_COPY_WORDS(WHIRLWIND_MAP_WIDTH / 2), REG_DMA3);
		metatile += WHIRLWIND_MAP_WIDTH;
	}
	syHeapRelease(buffer);
	fdScriptPlaySnd(SE_GLITTER);
	fdActorGet(ACTOR_TWISTER_ROCK_BY_THE_WALL)->sceneCnt =
		SPRITE_COLOR_BLUE_TINT;
	fdActorGet(ACTOR_TWISTER_ROCK_BY_THE_WALL)->sceneByte = 1;
	fdActorGet(ACTOR_TWISTER_ROCK_BY_THE_WALL)->update =
		scAirsRockFlickerTarget;
	for(i = 0; i != 45; i++) {
		if((FrameCount & 3) == 0) {
			spark = scFieldActorCreate(SPRITE_PARTICLE,
				fdActorGet(ACTOR_TWISTER_ROCK_BY_THE_WALL)->x.fixed,
				fdActorGet(ACTOR_TWISTER_ROCK_BY_THE_WALL)->y.fixed,
				fdActorGet(ACTOR_TWISTER_ROCK_BY_THE_WALL)->z.fixed);
			((FIELD_SPR *)spark->spr)->pri =
				((FIELD_SPR *)fdActorGet(ACTOR_TWISTER_ROCK_BY_THE_WALL)->spr)
					->pri;
			spark->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
			spark->motionFlg = 0;
			spark->bounce = spark->x.fixed;
			spark->gravity = toFIXED(3) / 2;
			spark->sceneWord = spark->z.fixed;
			spark->speedLimit = syRandom() & 0xffff;
			fdActorSelectAnimation(spark, 7);
			fdActorStartScript(spark, FieldEffect_MediumScript);
			fdActorSetSprFlags(spark, 0);
			scFieldActorSetSprCol(spark, SPRITE_COLOR_RBG);
			spark->update = scAirsRockSpiralSpark;
		}
		syFieldScriptWait(1);
	}
	syScriptCameraFollowActor(MainRoutineStateWork.leaderActorId, 1);
	fdCameraWaitForArrival();
	fdCameraSetSpeed(1);
	fdCameraMoveTo(330, CAMERA_KEEP, 73, 1);
	fdCameraWaitForArrival();
	sySetFrameEvent(scAirsRockScrollWave, EVENT_PASS_VBLANK);
	fdScriptPlaySnd(SE_WAVE_HIGH);
	REG_BLDALPHA = BLEND_ALPHA(0, 15);
	REG_BLDCNT = BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
	REG_DISPCNT |= DISPCNT_BG0_ON;
	fdMapGridSetShake(1, 1, 1);
	for(i = 0; i != 64; i++) {
		REG_BLDALPHA = ((BLEND_WEIGHT_MAX - 1 - (i >> 2)) << 8) | (i >> 2);
		fdActorGet(ACTOR_TWISTER_ROCK_BY_THE_WALL)->sceneCnt = syRandom() & 7;
		syFieldScriptWait(1);
	}
	syFieldScriptWait(60);
	REG_BLDALPHA = BLEND_ALPHA(15, 0);
	REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BG2 |
		BLDCNT_TGT2_BG3;
	fdMapGridSetShake(3, 3, 1);
	{
		Uint32 j = 0;

		do {
			j++;
			if((j & 3) == 0) {
				VECTOR3		   velocity;
				EFFECT_OPTIONS options;

				options.pri = 1;
				options.update = scAirsRockSwirlDust;
				velocity.x = 0;
				velocity.y = 0;
				velocity.z = -toFIXED(2);
				x = toFIXED(syRandom() % 144) + toFIXED(256);
				scFieldEffectSpawn(x, 0, toFIXED(syRandom() % 144), velocity.x,
					velocity.y, velocity.z,
					EFFECT_USE_UPDATE | EFFECT_USE_PRIORITY |
						EFFECT_SCRIPT_MEDIUM,
					&options);
			}
			REG_BLDALPHA =
				(((j >> 4) & 0xf) << 8) | (BLEND_WEIGHT_MAX - ((j >> 4) & 0xf));
			fdActorGet(ACTOR_TWISTER_ROCK_BY_THE_WALL)->sceneCnt =
				syRandom() & 7;
			syFieldScriptWait(1);
		} while(j != 255);
	}
	fdMapGridCopyMetatilesTo(68, 0, 79, 0, 11, 10);
	fdMapGridCopyMetatileAttributes(38, 9, 1, 1, 16, 9);
	fdMapGridCopyMetatileAttributes(38, 9, 1, 1, 24, 9);
	fdMapGridCenterOnFollowedPoint();
	syFieldScriptWait(1);
	REG_BLDCNT = BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
	REG_BLDALPHA = BLEND_ALPHA(15, 0);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	fdScriptPlaySnd(SOUND_FADE_OUT_ACTION_SE);
	fdActorGet(ACTOR_TWISTER_ROCK_BY_THE_WALL)->sceneCnt = SPRITE_COLOR_GREY;
	for(i = 0; i != 65; i++) {
		REG_BLDALPHA = ((i >> 2) << 8) | (BLEND_WEIGHT_MAX - (i >> 2));
		syFieldScriptWait(1);
	}
	syFieldCloseEvent(scAirsRockScrollWave);
	fdActorGet(ACTOR_TWISTER_ROCK_BY_THE_WALL)->sceneByte = 0;
	syWait(30);
	fdActorGet(ACTOR_TWISTER_ROCK_BY_THE_WALL)->update = NULL;
	scFieldActorSetSprCol(
		fdActorGet(ACTOR_TWISTER_ROCK_BY_THE_WALL), SPRITE_COLOR_GREY);
	syFieldMapGridWaitForShakeToSettle();
	syFieldScriptWait(60);
	REG_BLDCNT = 0;
	syWindowLoadGraphics();
	syPrintClear();
	state = &MainRoutineStateWork;
	mnWindowColApply(state->winHue, state->winBrightness);
	REG_BG1CNT &= ~BGCNT_PRIORITY(3);
	REG_BG1CNT |= BGCNT_PRIORITY(1);
	{
		Sint32 *ownerId = &state->leaderActorId;

		fdActorGet(*ownerId)->priFlg &= ~ACTOR_PRIORITY_IGNORES_RISES;
		fdActorGet(*ownerId)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
		{
			Uint8 *flags = &fdActorGet(ACTOR_TWISTER_ROCK_BY_THE_WALL)->priFlg;
			Uint8  v = *flags | ACTOR_PRIORITY_AUTOMATIC;

			*flags = v;
		}
		syScriptCameraFollowActor(*ownerId, 1);
	}
	fdCameraWaitForArrival();
	fdScriptSetFlag(FLAG_AIRS_ROCK_WALL_CLEARED);
	fdScriptEnd();
}

/* Felix is caught by one of the little whirlwinds the cliff's statues breathe:
   he spins round on top of it as it carries him along, and where it fades he
   jumps off. On a whirlwind blowing up the cliff, he has a moment to hold Up,
   Left or Right and choose which way to leap onto the ledge at its top;
   otherwise he drops off where it ends. */
void scAirsRockRideWhirlwind(void)
{
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	ACTOR *ride = &leader->watchedActor->actor;
	ACTOR *statue, *camera;
	Sint32 rise;
	FIXED  dx, dz;
	Sint32 moved, i, height;
	Sint32 angle;
	Uint16 facing;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	leader->motionFlg = ACTOR_FLOATS;
	fdActorSetSprFlags(leader, 0);
	syGameFlagSet(FLAG_RIDING_WHIRLWIND);
	sySndPlay(SE_KIRIN);
	rise = 0;
	while(ride->sceneByte != 0) {
		leader->x.fixed = ride->x.fixed;
		leader->y.fixed = ride->y.fixed + rise;
		leader->z.fixed = ride->z.fixed;
		if(rise < toFIXED(12))
			rise += toFIXED(1) / 10;
		leader->facing = (Uint8)FrameCount * FACING_STEP;
		leader->priFlg ^= ACTOR_PRIORITY_UNDERFOOT << (FrameCount & 1);
		sySynch(1);
	}
	sySndPlay(SE_STOP_ACTION_SE);
	facing = ride->facing;
	if(facing == FACING_NORTH) {
		statue = &ride->watchedActor->actor;
		dx = 0;
		dz = 0;
		leader->priFlg = ACTOR_PRIORITY_OVERHEAD;
		leader->x.fixed = FIELD_HALF_METATILE_ORIGIN(
			(FIELD_HALF_METATILE(leader->x.fixed) & ~1) + 1);
		leader->y.fixed = FIELD_HALF_METATILE_ORIGIN(
			(FIELD_HALF_METATILE(ride->y.fixed) & ~1) + 3);
		leader->z.fixed = FIELD_HALF_METATILE_ORIGIN(
			(FIELD_HALF_METATILE(leader->z.fixed) & ~1) + 1);
		for(i = 0; i != 60; i++) {
			moved = 0;
			if(PerWork.data & KEY_UP) {
				dx = 0;
				dz = -toFIXED(16);
				leader->facing = FACING_NORTH;
				moved = 1;
			}
			if(PerWork.data & KEY_RIGHT) {
				dx = toFIXED(16);
				dz = 0;
				leader->facing = FACING_EAST;
				moved = 1;
			}
			if(PerWork.data & KEY_LEFT) {
				dx = -toFIXED(16);
				leader->facing = FACING_WEST;
				dz = 0;
				moved = 1;
			}
			if(moved &&
				METATILE_CODE(fdMapMetatileGetAttributes(2,
					FIELD_METATILE(ride->x.fixed + dx),
					FIELD_METATILE(ride->z.fixed - ride->y.fixed + dz))) !=
					METATILE_MARK_BLOCKED)
				goto found;
			sySynch(1);
		}
		dx = 0;
		leader->facing = FACING_NORTH;
		dz = -toFIXED(16);
		if((Uint16)METATILE_CODE(
			   fdMapMetatileGetAttributes(2, FIELD_METATILE(ride->x.fixed),
				   FIELD_METATILE(ride->z.fixed - ride->y.fixed + dz))) ==
			METATILE_MARK_BLOCKED) {
			dx = toFIXED(16);
			dz = 0;
			leader->facing = FACING_EAST;
			if((Uint16)METATILE_CODE(fdMapMetatileGetAttributes(2,
				   FIELD_METATILE(ride->x.fixed + dx),
				   FIELD_METATILE(ride->z.fixed - ride->y.fixed))) ==
				METATILE_MARK_BLOCKED) {
				dx = -toFIXED(16);
				leader->facing = FACING_WEST;
			}
		}
	found:
		/* The statue's sceneCnt holds the layer of the ledge he lands on. */
		height = fdMapGetTerrainHeight(
			statue->sceneCnt, ride->x.fixed + dx, ride->z.fixed + dz);
		leader->y.fixed = height;
		leader->groundHeight = height;
		fdActorSelectAnimation(leader, ANIM_LEADER_SPRING);
		sySynch(6);
		sySndPlay(SE_JUMP);
		fdActorSelectAnimation(leader, ANIM_LEADER_LEAP);
		leader->speedLimit = toFIXED(2);
		leader->acceleration = toFIXED(1) / 2;
		leader->velY = toFIXED(4);
		leader->motionFlg = ACTOR_FALLS;
		leader->priFlg = ACTOR_PRIORITY_AUTOMATIC;
		leader->layer = statue->sceneCnt;
		scFieldActorSetMoveTargetFX(leader, leader->x.fixed + dx,
			leader->y.fixed, leader->z.fixed + dz);
		fdActorWaitUntilLanded(leader);
		scFieldActorWaitForMove(leader);
		fdActorSetSprFlags(leader, 1);
		leader->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	} else {
		angle = ride->facing;
		leader->x.fixed = FIELD_HALF_METATILE_ORIGIN(
			(FIELD_HALF_METATILE(ride->x.fixed) & ~1) +
			(angle = FACING_EIGHTH_INDEX(angle)) - 1);
		leader->z.fixed = FIELD_HALF_METATILE_ORIGIN(
			(FIELD_HALF_METATILE(ride->z.fixed) & ~1) - (angle & 2) + 1);
		fdActorSetSprFlags(leader, 1);
		leader->velY = toFIXED(6);
		leader->priFlg = ACTOR_PRIORITY_AUTOMATIC;
		leader->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
		fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
		camera = fdCameraGetActor();
		camera->speedLimit = toFIXED(8);
		camera->acceleration = toFIXED(1);
		camera->motionFlg = 0;
		scFieldActorSetMoveTargetFX(camera, leader->x.fixed,
			fdMapGetTerrainHeight(0, leader->x.fixed, leader->z.fixed),
			leader->z.fixed);
		fdActorWaitUntilLanded(leader);
		scFieldActorWaitForMove(camera);
	}
	fdActorGetById(MainRoutineStateWork.leaderActorId)->priFlg |=
		ACTOR_PRIORITY_AUTOMATIC;
	fdActorGetById(MainRoutineStateWork.leaderActorId)->priFlg |=
		ACTOR_PRIORITY_IGNORES_RISES;
	fdScriptClearFlag(FLAG_RIDING_WHIRLWIND);
	syScriptCameraFollowActor(MainRoutineStateWork.leaderActorId, 1);
	fdCameraWaitForArrival();
	sySynch(10);
	fdScriptEnd();
}

#include "shared/position_flat_distance.h"

#include "shared/position_distance_above.h"

extern Uint32 AirsRock_WhirlwindRisesScript[];
extern Uint32 AirsRock_WhirlwindFadesScript[];

/* How near a whirlwind passes to pick up the leader across the wall he climbs,
   in pixels; across the ground it is WHIRLWIND_REACH. */
#define WHIRLWIND_REACH_ON_WALL 16

/* A statue's whirlwind's update: it travels its way, picks up a leader it
   touches and fades on a wall or another statue. */
void scAirsRockMoveWhirlwind(ACTOR *shot)
{
	ACTOR		*leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	FIXED		 speed = toFIXED(3) / 2;
	Sint32		 snd = SE_SAND_EMERGE;
	ACTOR		*other, *launcher;
	Sint32		 i;
	Sint32		*leaderPosition;
	Sint32		*shotPosition;
	ACTOR	   **actors;

	if(state->psynergy != 0 || state->fieldMenu != 0 || state->pauseMenu != 0 ||
		state->item != 0 ||
		MainRoutineStateWork.movementMode == LEADER_MOVEMENT_SAND) {
		fdActorSetAnimationSpeed(shot, 0);
		return;
	}
	fdActorSetAnimationSpeed(shot, ANIM_STEP_NORMAL);
	if(METATILE_CODE(
		   fdMapMetatileGetAttributes(2, FIELD_METATILE(shot->x.fixed),
			   FIELD_METATILE(shot->z.fixed - shot->y.fixed))) ==
		METATILE_MARK_WHIRLWIND_END) {
	end:
		fdActorPlaySndInView(shot, snd);
		shot->sceneByte = 0;
		shot->update = 0;
		if(shot->facing == FACING_NORTH)
			fdActorStartScript(shot, AirsRock_WhirlwindRisesScript);
		else
			fdActorStartScript(shot, AirsRock_WhirlwindFadesScript);
		return;
	}
	if(shot->sceneCnt == 0) {
		/* i first says whether the whirlwind is beside the leader; the loop
		   below reuses it as the actor index. */
		i = 0;
		leaderPosition = &leader->x.fixed;
		shotPosition = &shot->x.fixed;
		if(MainRoutineStateWork.movementMode == LEADER_MOVEMENT_KEYS) {
			if(scPosDistanceAbove(leaderPosition, shotPosition) <=
					WHIRLWIND_REACH_ON_WALL &&
				FIELD_METATILE(leader->z.fixed) ==
					FIELD_METATILE(shot->z.fixed))
				i = 1;
		} else {
			if(scPosFlatDistance(leaderPosition, shotPosition) <=
					WHIRLWIND_REACH &&
				FIELD_METATILE(leader->y.fixed) ==
					FIELD_METATILE(shot->y.fixed))
				i = 1;
		}
		if(i && !syGameFlagGet(FLAG_FIELD_HANDLING_REQUESTS)) {
			leader->facing = shot->facing;
			leader->watchedActor = (FIELD_OBJECT *)shot;
			leader->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
			((FIELD_SPR *)leader->spr)->pri = ((FIELD_SPR *)shot->spr)->pri;
			state->raisedCue = RAISED_RIDE_WHIRLWIND;
			MainRoutineStateWork.movementMode = LEADER_MOVEMENT_WALK;
			shot->sceneCnt = 1;
			shot->sceneByte = 1;
		}
	}
	actors = &state->actors[ACTOR_FIRST_PLACED];
	for(i = ACTOR_FIRST_PLACED; i != FIELD_TREASURE_ACTOR; i++) {
		other = *actors++;
		if(other->script != 0) {
			launcher = &shot->watchedActor->actor;
			if(FIELD_METATILE(launcher->x.fixed) !=
					FIELD_METATILE(other->x.fixed) ||
				FIELD_METATILE(launcher->y.fixed) !=
					FIELD_METATILE(other->y.fixed) ||
				FIELD_METATILE(launcher->z.fixed) !=
					FIELD_METATILE(other->z.fixed)) {
				if(scPosFlatDistance(&other->x.fixed, &shot->x.fixed) <=
						WHIRLWIND_REACH &&
					FIELD_METATILE(other->y.fixed) ==
						FIELD_METATILE(shot->y.fixed))
					goto end;
			}
		}
	}
	if(shot->facing == FACING_NORTH) {
		shot->x.fixed += syMulFX(syCos(shot->facing), speed);
		shot->y.fixed -= syMulFX(syMathSin(shot->facing), speed);
	} else {
		shot->x.fixed += syMulFX(syCos(shot->facing), speed);
		shot->z.fixed += syMulFX(syMathSin(shot->facing), speed);
	}
}

/* A statue's update: while no Psynergy, menu or item is in use it counts
   frames, and every WHIRLWIND_BREATH_FRAMES breathes a whirlwind
   (scAirsRockMoveWhirlwind) 8 pixels ahead of it with a puff of dust, with a
   gust's sound unless the leader is riding one. */
void scAirsRockStatueBreathesWhirlwind(ACTOR *actor)
{
	FIELD_STATE	  *state = Ram_HeapSlots->fieldState;
	ACTOR		  *shot;
	VECTOR3		   position;
	EFFECT_OPTIONS options;

	if(state->psynergy != 0 || state->fieldMenu != 0 || state->pauseMenu != 0 ||
		state->item != 0)
		return;
	if(actor->tmr == WHIRLWIND_BREATH_FRAMES) {
		if(!fdScriptGetFlag(FLAG_RIDING_WHIRLWIND))
			fdActorPlaySndInView(actor, SE_FLAME_FLARE);
		if(actor->facing == FACING_NORTH) {
			position.x = actor->x.fixed;
			position.z = actor->z.fixed;
		} else {
			position.x =
				actor->x.fixed + syMathMulFX(toFIXED(8), syCos(actor->facing));
			position.z = actor->z.fixed +
				syMathMulFX(toFIXED(8), syMathSin(actor->facing));
		}
		shot = scFieldActorCreate(
			SPRITE_WHIRLWIND, position.x, actor->y.fixed, position.z);
		fdActorSelectAnimation(shot, 2);
		fdActorSetSprFlags(shot, 0);
		((FIELD_SPR *)shot->spr)->pri = 1;
		shot->motionFlg = 0;
		shot->sceneCnt = 0;
		shot->sceneByte = 0;
		shot->facing = actor->facing;
		shot->update = scAirsRockMoveWhirlwind;
		shot->watchedActor = (FIELD_OBJECT *)actor;
		actor->tmr = 0;
		options.pri = 1;
		options.pal = 7;
		scFieldEffectSpawn(position.x, actor->y.fixed, position.z - toFIXED(6),
			0, 0, 0, EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE, &options);
	}
	actor->tmr++;
}

/* Nothing visible happens as Felix starts to push one of the cliff's pillars,
   or casts Move on one; the game takes it off the metatiles it was closing so
   that it can slide. */
void scAirsRockLiftPillar(void)
{
	FIELD_STATE *state = Ram_HeapSlots->fieldState;

	fdActorLiftTargetBlock(AirsRock_EntrancePillars);
	scBlocksUnmarkMetatiles(state->movedBlock);
}

/* Once Felix has pushed or moved one of the cliff's pillars: if it now stands
   over a gap it drops into it with a thump, closing the gap so the party can
   walk over it, and it stays there for good. */
void scAirsRockPlacePillar(void)
{
	ACTOR *actor = FieldStateWork->movedBlock;
	Sint32 x, z;

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdActorPlaceMovedBlock();
	scBlocksMarkMetatiles(((Uint8 *)AirsRock_EntrancePillars));
	if(fdMapGetTerrainHeight(2, actor->x.fixed, actor->z.fixed) !=
		actor->y.fixed) {
		actor->layer = 2;
		actor->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
		fdActorWaitUntilLanded(actor);
		fdActorWaitUntilLanded(actor);
		fdScriptPlaySnd(SE_THUMP);
		actor->motionFlg = 0;
		actor->layer = 0;
	}
	x = FIELD_METATILE(actor->x.fixed);
	z = FIELD_METATILE(actor->z.fixed);
	if(x == 16 && z == 43)
		fdScriptSetFlag(FLAG_AIRS_ROCK_PILLAR_PLACED_FIRST);
	else if(x == 45 && z == 45)
		fdScriptSetFlag(FLAG_AIRS_ROCK_PILLAR_PLACED_SECOND);
	else if(x == 21 && z == 48)
		fdScriptSetFlag(FLAG_AIRS_ROCK_PILLAR_PLACED_THIRD);
	else if(x == 39 && z == 49)
		fdScriptSetFlag(FLAG_AIRS_ROCK_PILLAR_PLACED_FOURTH);
	else if(x == 49 && z == 18)
		fdScriptSetFlag(FLAG_AIRS_ROCK_PILLAR_PLACED_FIFTH);
	fdScriptEnd();
}

/* Felix casts Frost on the puddle on the cliff and it freezes into a pillar of
   ice. */
void scAirsRockFreezePuddle(void)
{
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdFlagBlockApplyFlags((Sint32)((Uint8 *)AirsRock_FrozenPuddle));
	fdScriptEnd();
}

extern Sint32 AirsRock_RubbleScript[];

/* A crumbling rock's clouds of dust, every fourth at its own size and the rest
   twice as big, and the pieces of rubble it scatters. */
#define CRUMBLE_DUST_CLOUDS 12
#define CRUMBLE_PIECES		6

/* A rock crumbles at x, z into rubble that scatters, shaking the screen. */
void scAirsRockCrumbleRock(Sint32 x, Sint32 z)
{
	ACTOR		  *first, *rock;
	EFFECT_OPTIONS options;
	VECTOR3		   scatter;
	Sint32		   i;
	FIXED		   radius = toFIXED(64);

	first = scFieldActorCreate(SPRITE_ROCK, x, 0, z);
	options.pri = 1;
	options.startScaleX = toFIXED(2);
	options.startScaleY = toFIXED(2);
	for(i = 0; i != CRUMBLE_DUST_CLOUDS; i++) {
		scatter.x = x + toFIXED(syRandom() & 63) - toFIXED(32);
		scatter.y = (syRandom() & 15) << 12;
		scatter.z = z + toFIXED(syRandom() & 63) - toFIXED(48);
		if(i & 3)
			scFieldEffectSpawn(scatter.x, 0, scatter.z, 0, scatter.y, 0,
				EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY |
					EFFECT_SCRIPT_MEDIUM,
				&options);
		else
			scFieldEffectSpawn(scatter.x, 0, scatter.z, 0, scatter.y, 0,
				EFFECT_USE_PRIORITY | EFFECT_SCRIPT_MEDIUM, &options);
	}
	for(i = 0; i != CRUMBLE_PIECES; i++) {
		rock = first;
		if(i != 0) {
			rock = scFieldActorCreate(SPRITE_ROCK, x, 0, z);
			fdSprShareVRAMCacheEntry(rock->spr, first->spr);
		}
		fdActorSetSprFlags(rock, 0);
		fdActorStartScript(rock, AirsRock_RubbleScript);
		fdActorSelectAnimation(rock, 2);
		rock->velY = toFIXED(syRandom() & 3) + toFIXED(4);
		rock->gravity = toFIXED(1) / 5;
		rock->bounce = toFIXED(2) / 5;
		rock->scaleX = toFIXED(4) / 5;
		rock->scaleY = toFIXED(4) / 5;
		rock->speedLimit = toFIXED(4) / 5;
		scatter.y = syRandom() & 7;
		scatter.x = syMulFX(syCos(scatter.y * FACING_STEP), radius);
		scatter.z = syMulFX(syMathSin(scatter.y * FACING_STEP), radius);
		((FIELD_SPR *)rock->spr)->pri = 1;
		scFieldActorSetMoveTargetFX(rock, rock->x.fixed + scatter.x,
			rock->y.fixed, rock->z.fixed + scatter.z);
	}
	fdMapGridSetShake(5, 7, 1);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
}

/* Felix casts Whirlwind on the twister rock at the top of Air's Rock: it glows,
   the light turns yellow, lightning strikes the pile of rocks in the hollow
   below it twice and they crumble with a crash and a flash
   (scAirsRockCrumbleRock), clearing the way to the doorway into the rock. The
   rock turns grey, spent. */
void scAirsRockWhirlwindOpensTheTop(void)
{
	Uint16 *buffer = syHeapAlloc(LIGHTNING_TILES * TILE_SIZE_4BPP);
	Uint16 *metatile;
	ACTOR  *spark;
	Uint32	i;

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	REG_DISPCNT &= ~DISPCNT_BG0_ON;
	syIOWrite16(BGCNT_SCREEN_BASE(LIGHTNING_SCREEN_BLOCK), &REG_BG0CNT);
	syDecompressTaggedLZ(
		syResourceGetTableEntry(RESOURCE_LIGHTNING_TILES), buffer);
	syDMAXCopy(buffer, (void *)BG_CHAR_BLOCK(0),
		DMA_COPY_WORDS(LIGHTNING_TILES * TILE_SIZE_4BPP / 4), REG_DMA3);
	syHeapRelease(buffer);
	DMA_FILL16(
		0, (void *)BG_SCREEN_BLOCK(LIGHTNING_SCREEN_BLOCK), BG_SCREEN_SIZE);
	buffer = syHeapAlloc(
		LIGHTNING_MAP_ROWS * LIGHTNING_MAP_COLUMNS * sizeof(Uint16));
	syDecompressTaggedLZ(
		syResourceGetTableEntry(RESOURCE_LIGHTNING_MAP), buffer);
	metatile = buffer;
	/* The map's tile numbers less 512. */
	for(i = 0; i != LIGHTNING_MAP_ROWS * LIGHTNING_MAP_COLUMNS; i++) {
		*metatile &= 0xfcff;
		metatile++;
	}
	metatile = buffer;
	for(i = 0; i != LIGHTNING_MAP_ROWS; i++) {
		syDMAXCopy(metatile,
			(void *)(BG_SCREEN_BLOCK(LIGHTNING_SCREEN_BLOCK) +
				i * MAP_ROW_BYTES),
			DMA_COPY_WORDS(LIGHTNING_MAP_COLUMNS * sizeof(Uint16) / 4),
			REG_DMA3);
		metatile += LIGHTNING_MAP_COLUMNS;
	}
	syHeapRelease(buffer);
	fdScriptPlaySnd(SE_GLITTER);
	fdActorGet(ACTOR_TWISTER_ROCK_AT_THE_TOP)->sceneCnt =
		SPRITE_COLOR_BLUE_TINT;
	fdActorGet(ACTOR_TWISTER_ROCK_AT_THE_TOP)->sceneByte = 1;
	fdActorGet(ACTOR_TWISTER_ROCK_AT_THE_TOP)->update = scAirsRockFlickerTarget;
	for(i = 0; i != 45; i++) {
		if((FrameCount & 3) == 0) {
			spark = (scFieldActorCreate(SPRITE_PARTICLE,
				fdActorGet(ACTOR_TWISTER_ROCK_AT_THE_TOP)->x.fixed,
				fdActorGet(ACTOR_TWISTER_ROCK_AT_THE_TOP)->y.fixed,
				fdActorGet(ACTOR_TWISTER_ROCK_AT_THE_TOP)->z.fixed));
			((FIELD_SPR *)spark->spr)->pri =
				((FIELD_SPR *)fdActorGet(ACTOR_TWISTER_ROCK_AT_THE_TOP)->spr)
					->pri;
			spark->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
			spark->motionFlg = 0;
			spark->bounce = spark->x.fixed;
			spark->gravity = toFIXED(3) / 2;
			spark->sceneWord = spark->z.fixed;
			spark->speedLimit = syRandom() & 0xffff;
			fdActorSelectAnimation(spark, 7);
			fdActorStartScript(spark, FieldEffect_MediumScript);
			fdActorSetSprFlags(spark, 0);
			scFieldActorSetSprCol(spark, SPRITE_COLOR_RBG);
			spark->update = scAirsRockSpiralSpark;
		}
		syFieldScriptWait(1);
	}
	fdPaletteFadeApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
	fdPaletteFadeApplyTarget(PALETTE_TINT_WARM, PALETTES_ALL);
	fdPaletteFadeInterpolate(60);
	syScriptCameraFollowActor(MainRoutineStateWork.leaderActorId, 1);
	fdCameraWaitForArrival();
	fdCameraSetSpeed(1);
	fdCameraMoveTo(536, CAMERA_KEEP, 216, 1);
	fdCameraWaitForArrival();
	fdActorGet(ACTOR_TWISTER_ROCK_AT_THE_TOP)->update = 0;
	scFieldActorSetSprCol(
		fdActorGet(ACTOR_TWISTER_ROCK_AT_THE_TOP), SPRITE_COLOR_GREY);
	REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX, BLEND_WEIGHT_MAX);
	REG_BLDCNT = BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
	BgScrollWork[0].x = -120;
	BgScrollWork[0].y = 16;
	syWait(30);
	fdScriptPlaySnd(SE_ITEM_BREAK);
	fdPaletteFadeApplySource(PALETTE_TINT_WARM, PALETTES_ALL);
	fdPaletteFadeApplyTarget(RGB_WHITE, PALETTES_ALL);
	fdPaletteFadeInterpolate(1);
	fdActorGet(MainRoutineStateWork.leaderActorId)->facing = FACING_SOUTH;
	syWait(1);
	fdPaletteFadeApplySource(RGB_WHITE, PALETTES_ALL);
	fdPaletteFadeApplyTarget(PALETTE_TINT_WARM, PALETTES_ALL);
	fdPaletteFadeInterpolate(8);
	REG_DISPCNT |= DISPCNT_BG0_ON;
	fdMapGridCopyMetatilesTo(52, 12, 29, 9, 9, 9);
	fdMapGridCopyMetatilesTo(52, 76, 29, 73, 9, 9);
	scAirsRockCrumbleRock(toFIXED(552), toFIXED(228));
	for(i = 0; i != BLEND_WEIGHT_MAX * 2; i++) {
		REG_BLDALPHA =
			BLEND_ALPHA(BLEND_WEIGHT_MAX - (i >> 1), BLEND_WEIGHT_MAX);
		syWait(1);
	}
	BgScrollWork[0].x = -84;
	BgScrollWork[0].y = 0;
	syWait(1);
	fdScriptPlaySnd(SE_ITEM_BREAK);
	fdPaletteFadeApplySource(PALETTE_TINT_WARM, PALETTES_ALL);
	fdPaletteFadeApplyTarget(RGB_WHITE, PALETTES_ALL);
	fdPaletteFadeInterpolate(1);
	syWait(1);
	fdPaletteFadeApplySource(RGB_WHITE, PALETTES_ALL);
	fdPaletteFadeApplyTarget(PALETTE_TINT_WARM, PALETTES_ALL);
	fdPaletteFadeInterpolate(8);
	REG_DISPCNT |= DISPCNT_BG0_ON;
	fdMapGridCopyMetatilesTo(52, 22, 29, 9, 9, 9);
	fdMapGridCopyMetatilesTo(52, 86, 29, 73, 9, 9);
	scAirsRockCrumbleRock(toFIXED(520), toFIXED(242));
	for(i = 0; i != BLEND_WEIGHT_MAX * 2; i++) {
		REG_BLDALPHA =
			BLEND_ALPHA(BLEND_WEIGHT_MAX - (i >> 1), BLEND_WEIGHT_MAX);
		syWait(1);
	}
	fdScriptPlaySnd(SE_CRASH);
	fdPaletteFadeApplySource(PALETTE_TINT_WARM, PALETTES_ALL);
	fdPaletteFadeApplyTarget(RGB_WHITE, PALETTES_ALL);
	fdPaletteFadeInterpolate(1);
	REG_DISPCNT &= ~DISPCNT_BG0_ON;
	syFieldScriptWait(20);
	scAirsRockCrumbleRock(toFIXED(512), toFIXED(198));
	scAirsRockCrumbleRock(toFIXED(562), toFIXED(242));
	scAirsRockCrumbleRock(toFIXED(521), toFIXED(223));
	fdMapGridCopyMetatilesTo(62, 2, 29, 9, 9, 9);
	fdMapGridCopyMetatileAttributes(62, 2, 9, 9, 29, 9);
	fdMapGridCopyMetatilesTo(62, 66, 29, 73, 9, 9);
	fdPaletteFadeApplySource(RGB_WHITE, PALETTES_ALL);
	fdPaletteFadeApplyTarget(PALETTE_TINT_WARM, PALETTES_ALL);
	fdPaletteFadeInterpolate(60);
	syWait(60);
	fdPaletteFadeApplySource(PALETTE_TINT_WARM, PALETTES_ALL);
	fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
	fdPaletteFadeInterpolate(60);
	syWait(70);
	syWindowLoadGraphics();
	syPrintClear();
	syFieldWindowColApply(
		MainRoutineStateWork.winHue, MainRoutineStateWork.winBrightness);
	REG_BLDCNT = 0;
	REG_DISPCNT |= DISPCNT_BG0_ON;
	BgScrollWork[0].x = 0;
	BgScrollWork[0].y = 0;
	fdScriptSetFlag(FLAG_AIRS_ROCK_TOP_OPENED);
	fdScriptEnd();
}

/* The fog's blend weight at its thickest; it breathes down to half that. */
#define FOG_WEIGHT		 8
/* A fog line's ripple: a sine shifted down to at most 8 pixels either way. */
#define FOG_RIPPLE_SHIFT 13

/* The fog over the wall drifts, thickening and thinning between FOG_WEIGHT and
   half that while its lines ripple. */
void scAirsRockDriftFog(void)
{
	MAP_LAYER		*layer = fdMapGridGetLayer(2);
	Sint16			*line = AirsRock_WaveLines;
	Sint32			 i;
	volatile Uint16 *channel;

	REG_BLDALPHA = BLEND_ALPHA(AirsRock_FogAlpha, BLEND_WEIGHT_MAX);
	if((*Ram_FrameCount & 15) == 0) {
		AirsRock_FogAlpha += AirsRock_FogAlphaStep;
		if(AirsRock_FogAlpha == FOG_WEIGHT ||
			AirsRock_FogAlpha == FOG_WEIGHT / 2)
			AirsRock_FogAlphaStep *= -1;
	}
	for(i = 0; i != DISPLAY_HEIGHT; i++) {
		*line = FIXEDtoINT(layer->x) +
			(syMathSin(
				 (i + (Uint8)*Ram_FrameCount + FIXEDtoINT(layer->y)) << 9) >>
				FOG_RIPPLE_SHIFT);
		line++;
	}
	line = AirsRock_WaveLines;
	REG_BG1HOFS = line[0];
	channel = REG_DMA0;
	channel[5] &= ~DMA_START_TIMING_DRQ_REPEAT;
	channel[5] &= ~DMA_ENABLE;
	channel[5];
	syDMAXCopy(line + 1, (void *)&REG_BG1HOFS, DMA_HBLANK_HALFWORD_PER_LINE,
		(volatile Uint32 *)channel);
}

/* As the fog lifts, the great whirlwind sways and scatters the more the higher
   it stands above its foot, while the fog ripples and thickens and thins as in
   scAirsRockDriftFog. */
void scAirsRockLiftFog(void)
{
	MAP_LAYER		*layer = fdMapGridGetLayer(2);
	Sint16			*line = AirsRock_WaveLines;
	Sint32			 i;
	FIXED			 wave;
	Sint32			 spread, value;
	Sint16			*table;
	volatile Uint16 *channel;

	for(i = 0; i != DISPLAY_HEIGHT; i++) {
		spread = (DISPLAY_HEIGHT - i) >> 2;
		wave = syMathSin((i + (Uint8)FrameCount) << 8);
		/* Swayed by up to half the line's height above the foot, less 4, and
		   jittered by up to a quarter of the lines below; a line swung past
		   WAVE_SCROLL_MAX or WAVE_SCROLL_MIN is pulled back by as much. */
		value =
			(syMulFX(wave, (WAVE_SCROLL_CALM_LINE - i) * (toFIXED(1) / 64)) >>
				11) -
			4;
		line[0] = value;
		line[0] += syRandom() % (spread * 2) - spread;
		if(line[0] > WAVE_SCROLL_MAX)
			line[0] -= WAVE_SCROLL_MAX;
		else if(line[0] < WAVE_SCROLL_MIN)
			line[0] -= WAVE_SCROLL_MIN;
		line[1] = 0;
		value = wave + FIXEDtoINT(layer->y);
		line[2] = FIXEDtoINT(layer->x) + (value >> FOG_RIPPLE_SHIFT);
		line += 3;
	}
	REG_BLDALPHA = BLEND_ALPHA(AirsRock_LiftingFogAlpha, BLEND_WEIGHT_MAX);
	if((FrameCount & 15) == 0) {
		AirsRock_LiftingFogAlpha += AirsRock_LiftingFogAlphaStep;
		if(AirsRock_LiftingFogAlpha == FOG_WEIGHT ||
			AirsRock_LiftingFogAlpha == FOG_WEIGHT / 2)
			AirsRock_LiftingFogAlphaStep *= -1;
	}
	table = AirsRock_WaveLines;
	*(volatile Uint16 *)REG_BG0HOFS = table[0];
	table += 2;
	REG_BG1HOFS = table[0];
	channel = REG_DMA0;
	channel[5] &= ~DMA_START_TIMING_DRQ_REPEAT;
	channel[5] &= ~DMA_ENABLE;
	channel[5];
	syDMAXCopy(table + 1, REG_BG0HOFS, DMA_HBLANK_HALFWORD_PER_LINE + 2,
		(volatile Uint32 *)channel);
}

/* Felix casts Whirlwind on the twister rock at the foot of the foggy wall:
   a great whirlwind rises from it and the fog lifts (scAirsRockLiftFog) for
   good, so that the wall can be climbed out of it. The rock turns grey,
   spent. */
void scAirsRockWhirlwindClearsFog(void)
{
	Uint16	  *buffer = syHeapAlloc(2 * WHIRLWIND_TILE_ROW_SIZE);
	MAP_LAYER *fog = fdMapGridGetLayer(2);
	Uint16	  *metatile;
	ACTOR	  *spark;
	Sint32	   i;
	Sint32	   z;

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	REG_DISPCNT &= ~DISPCNT_BG0_ON;
	syDecompressTaggedLZ(
		syResourceGetTableEntry(RESOURCE_WHIRLWIND_TILES), buffer);
	syDMAXCopy(buffer, BG_CHAR_BLOCK(3) + WHIRLWIND_TOP_TILE * TILE_SIZE_4BPP,
		DMA_COPY_WORDS(WHIRLWIND_TILE_ROW_SIZE / 4), REG_DMA3);
	syDMAXCopy((Uint8 *)buffer + WHIRLWIND_TILE_ROW_SIZE,
		BG_CHAR_BLOCK(3) + WHIRLWIND_BOTTOM_TILE * TILE_SIZE_4BPP,
		DMA_COPY_WORDS(WHIRLWIND_TILE_ROW_SIZE / 4), REG_DMA3);
	syHeapRelease(buffer);
	syIOWrite16(BGCNT_PRIORITY(2) | BGCNT_CHAR_BASE(3) | BGCNT_SCREEN_BASE(4),
		&REG_BG0CNT);
	DMA_FILL16(WHIRLWIND_FILL_TILE, BG_SCREEN_BLOCK(4), 1280);
	buffer = syHeapAlloc(
		WHIRLWIND_MAP_HEIGHT * WHIRLWIND_MAP_WIDTH * sizeof(Uint16));
	syDecompressTaggedLZ(
		syResourceGetTableEntry(RESOURCE_WHIRLWIND_MAP), buffer);
	metatile = buffer;
	for(i = 0; i != WHIRLWIND_MAP_HEIGHT * WHIRLWIND_MAP_WIDTH; i++) {
		*metatile -= WHIRLWIND_MAP_FIRST_TILE - WHIRLWIND_TOP_TILE;
		metatile++;
	}
	metatile = buffer;
	for(i = 0; i != WHIRLWIND_MAP_HEIGHT; i++) {
		syDMAXCopy(metatile,
			BG_SCREEN_BLOCK(4) + WHIRLWIND_MAP_ROW_OFFSET +
				i * BG_SCREEN_WIDTH * sizeof(Uint16),
			DMA_COPY_WORDS(WHIRLWIND_MAP_WIDTH / 2), REG_DMA3);
		metatile += WHIRLWIND_MAP_WIDTH;
	}
	syHeapRelease(buffer);
	fdScriptPlaySnd(SE_GLITTER);
	fdActorGet(ACTOR_TWISTER_ROCK_IN_THE_FOG)->sceneCnt =
		SPRITE_COLOR_BLUE_TINT;
	fdActorGet(ACTOR_TWISTER_ROCK_IN_THE_FOG)->sceneByte = 1;
	fdActorGet(ACTOR_TWISTER_ROCK_IN_THE_FOG)->update = scAirsRockFlickerTarget;
	for(i = 0; i != 45; i++) {
		Sint32 low = FrameCount & 3;

		if(low == 0) {
			spark = (scFieldActorCreate(SPRITE_PARTICLE,
				fdActorGet(ACTOR_TWISTER_ROCK_IN_THE_FOG)->x.fixed,
				fdActorGet(ACTOR_TWISTER_ROCK_IN_THE_FOG)->y.fixed,
				fdActorGet(ACTOR_TWISTER_ROCK_IN_THE_FOG)->z.fixed));
			((FIELD_SPR *)spark->spr)->pri =
				((FIELD_SPR *)fdActorGet(ACTOR_TWISTER_ROCK_IN_THE_FOG)->spr)
					->pri;
			spark->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
			spark->motionFlg = 0;
			spark->bounce = spark->x.fixed;
			spark->gravity = toFIXED(3) / 2;
			spark->sceneWord = spark->z.fixed;
			spark->speedLimit = syRandom() & 0xffff;
			fdActorSelectAnimation(spark, 7);
			fdActorStartScript(spark, FieldEffect_MediumScript);
			fdActorSetSprFlags(spark, 0);
			scFieldActorSetSprCol(spark, SPRITE_COLOR_RBG);
			spark->update = scAirsRockSpiralSpark;
		}
		syFieldScriptWait(1);
	}
	syScriptCameraFollowActor(MainRoutineStateWork.leaderActorId, 1);
	fdCameraWaitForArrival();
	fdCameraSetSpeed(1);
	fdCameraMoveTo(152, CAMERA_KEEP, 144, 1);
	fdCameraWaitForArrival();
	syFieldCloseEvent(scAirsRockDriftFog);
	REG_BG1CNT &= ~BGCNT_PRIORITY(3);
	REG_BG1CNT |= BGCNT_PRIORITY(0);
	fdPaletteFadeApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
	fdPaletteFadeApplyTarget(RGB_WHITE, PALETTES_ALL);
	fdPaletteFadeInterpolate(1);
	syWait(1);
	fdPaletteFadeApplySource(RGB_WHITE, PALETTES_ALL);
	fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
	fdPaletteFadeInterpolate(8);
	sySetFrameEvent(scAirsRockLiftFog, EVENT_PASS_VBLANK);
	fdScriptPlaySnd(SE_WAVE_HIGH);
	REG_DISPCNT |= DISPCNT_BG0_ON;
	fdMapGridSetShake(3, 3, 1);
	z = -toFIXED(1) / 2;
	for(i = toFIXED(1) / 4; i != toFIXED(9) / 2;
		i += toFIXED(1) / 64, z -= toFIXED(1) / 64) {
		fog->speedX = i;
		fog->speedY = z;
		fdActorGet(ACTOR_TWISTER_ROCK_IN_THE_FOG)->sceneCnt = syRandom() & 7;
		syWait(1);
	}
	syFieldCloseEvent(scAirsRockLiftFog);
	sySetFrameEvent(scAirsRockScrollWave, EVENT_PASS_VBLANK);
	REG_BLDALPHA = BLEND_ALPHA(FOG_WEIGHT, 0);
	REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
	/* The fog thins a weight every 32 frames, to none. */
	for(i = 0; i != (FOG_WEIGHT << 5) + 1; i++) {
		REG_BLDALPHA = BLEND_ALPHA(FOG_WEIGHT - (i >> 5), BLEND_WEIGHT_MAX);
		fdActorGet(ACTOR_TWISTER_ROCK_IN_THE_FOG)->sceneCnt = syRandom() & 7;
		syFieldScriptWait(1);
	}
	REG_DISPCNT &= ~DISPCNT_BG1_ON;
	syWait(30);
	fdScriptPlaySnd(SOUND_FADE_OUT_ACTION_SE);
	REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX, 0);
	REG_BLDCNT = BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	fdActorGet(ACTOR_TWISTER_ROCK_IN_THE_FOG)->sceneCnt = SPRITE_COLOR_GREY;
	for(i = 0; i != 129; i++) {
		REG_BLDALPHA = ((i >> 3) << 8) | (BLEND_WEIGHT_MAX - (i >> 3));
		syFieldScriptWait(1);
	}
	syFieldCloseEvent(scAirsRockScrollWave);
	fdActorGet(ACTOR_TWISTER_ROCK_IN_THE_FOG)->sceneByte = 0;
	syWait(30);
	fdActorGet(ACTOR_TWISTER_ROCK_IN_THE_FOG)->update = NULL;
	scFieldActorSetSprCol(
		fdActorGet(ACTOR_TWISTER_ROCK_IN_THE_FOG), SPRITE_COLOR_GREY);
	syFieldMapGridWaitForShakeToSettle();
	syFieldScriptWait(60);
	REG_BLDCNT = 0;
	syWindowLoadGraphics();
	syPrintClear();
	syFieldWindowColApply(
		MainRoutineStateWork.winHue, MainRoutineStateWork.winBrightness);
	REG_BG1CNT &= ~BGCNT_PRIORITY(3);
	REG_BG1CNT |= BGCNT_PRIORITY(1);
	fdActorGet(MainRoutineStateWork.leaderActorId)->priFlg &=
		~ACTOR_PRIORITY_IGNORES_RISES;
	fdActorGet(MainRoutineStateWork.leaderActorId)->priFlg |=
		ACTOR_PRIORITY_AUTOMATIC;
	fdActorGet(ACTOR_TWISTER_ROCK_IN_THE_FOG)->priFlg |=
		ACTOR_PRIORITY_AUTOMATIC;
	syScriptCameraFollowActor(MainRoutineStateWork.leaderActorId, 1);
	fdCameraWaitForArrival();
	fdScriptSetFlag(FLAG_AIRS_ROCK_FOG_CLEARED);
	fdScriptEnd();
}

extern Sint32 AirsRock_LeaderLevel;

/* The climb's levels are this many pixels apart. */
#define CLIMB_LEVEL_HEIGHT 112

/* Nothing visible happens: as Felix climbs or drops between the four levels of
   the ledges below the fog, the game swaps in the metatiles of the ledges in
   front of the two pillars for the level he is on, since the ledges of all four
   levels are drawn over the same rows of the map. On the second level they hold
   the edge he drops off (scAirsRockDropOffLedge). */
void scAirsRockSetLedgeMetatilesByLevel(void)
{
	ACTOR *actor = fdActorGet(syFieldPartyGetLeaderActor());
	Sint32 level;

	actor->priFlg |= ACTOR_PRIORITY_IGNORES_RISES;
	level = FIXEDtoINT(actor->y.fixed + toFIXED(32)) / CLIMB_LEVEL_HEIGHT;
	if(AirsRock_LeaderLevel != level) {
		switch(level) {
		case 3:
			fdMapGridCopyMetatileAttributes(64, 64, 21, 3, 27, 54);
			break;
		case 2:
			fdMapGridCopyMetatileAttributes(64, 67, 21, 3, 27, 54);
			break;
		case 1:
			fdMapGridCopyMetatileAttributes(64, 70, 21, 3, 27, 54);
			break;
		case 0:
			fdMapGridCopyMetatileAttributes(64, 73, 21, 3, 27, 54);
			break;
		}
		AirsRock_LeaderLevel = level;
	}
}

#include "shared/leader_lift.h"

/* The leader's actor. */
#define LEADER() fdActorGetById(MainRoutineStateWork.leaderActorId)

/* The fog's picture repeats every FOG_WRAP map metatiles (64 pixels) across and
   down. */
#define FOG_WRAP 4

/* Sets a statue breathing whirlwinds (scAirsRockStatueBreathesWhirlwind), its
   count started at `frames` of WHIRLWIND_BREATH_FRAMES. */
#define START_BREATHING(id, frames) \
	(fdActorGetById(id)->tmr = (frames), \
		fdActorGetById(id)->update = scAirsRockStatueBreathesWhirlwind, \
		fdActorSetSprFlags(fdActorGetById(id), 0))

/* The same for a statue hung on the cliff at the given height. */
#define HANG_BREATHING(id, height, frames) \
	(fdActorGetById(id)->motionFlg = 0, \
		fdActorGetById(id)->y.fixed = (height), START_BREATHING(id, frames))

Sint32 scInit(void)
{
	ACTOR				   *leader;
	volatile DMA_REGISTERS *dma;

	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	MainRoutineStateWork.retreatScene = SCENE_AIRS_ROCK_STORM_BRAND_CHEST;
	MainRoutineStateWork.retreatEntrance =
		AIRS_ROCK_STORM_BRAND_CHEST_FROM_WORLD_MAP;
	LEADER()->priFlg |= ACTOR_PRIORITY_IGNORES_RISES;
	if(MainRoutineStateWork.next == SCENE_AIRS_ROCK_STORM_BRAND_CHEST) {
		switch(MainRoutineStateWork.entrance) {
		case AIRS_ROCK_STORM_BRAND_CHEST_FROM_AIRS_ROCK_ENTRANCE_WEST:
		case AIRS_ROCK_STORM_BRAND_CHEST_FROM_AIRS_ROCK_ENTRANCE_EAST_A:
		case AIRS_ROCK_STORM_BRAND_CHEST_FROM_AIRS_ROCK_ENTRANCE_EAST_B:
			scLeaderLift(toFIXED(88));
		}
		/* The barriers already blown away stay gone. */
		if(syGameFlagGet(FLAG_AIRS_ROCK_FIRST_BARRIER_BLOWN))
			CLEAR_SAND_MOUND(49, 12);
		if(syGameFlagGet(FLAG_AIRS_ROCK_FIRST_BARRIER_BLOWN + 1))
			CLEAR_SAND_BANK(6, 40);
		if(syGameFlagGet(FLAG_AIRS_ROCK_FIRST_BARRIER_BLOWN + 2))
			CLEAR_SAND_BANK(39, 39);
		if(syGameFlagGet(FLAG_AIRS_ROCK_FIRST_BARRIER_BLOWN + 3))
			CLEAR_SAND_BANK(23, 44);
		if(syGameFlagGet(FLAG_AIRS_ROCK_FIRST_BARRIER_BLOWN + 4)) {
			/* The bank at (6, 52), the one a gust blows south through: its
			   edits start a row lower than the other banks'. */
			fdMapGridCopyMetatilesTo(64, 0, 70, 52, 1, 4);
			fdMapGridCopyMetatileAttributes(64, 0, 1, 3, 70, 52);
			fdMapGridCopyMetatileAttributes(64, 0, 1, 3, 6, 53);
		}
		if(syGameFlagGet(FLAG_AIRS_ROCK_FIRST_BARRIER_BLOWN + 5))
			CLEAR_SAND_MOUND(15, 50);
		if(syGameFlagGet(FLAG_AIRS_ROCK_FIRST_BARRIER_BLOWN + 6))
			CLEAR_SAND_BANK(39, 49);
		if(syGameFlagGet(FLAG_AIRS_ROCK_FIRST_BARRIER_BLOWN + 7))
			CLEAR_SAND_MOUND(47, 53);
		if(syGameFlagGet(FLAG_AIRS_ROCK_WALL_CLEARED)) {
			fdMapGridCopyMetatilesTo(68, 0, 79, 0, 11, 10);
			fdMapGridCopyMetatileAttributes(38, 9, 1, 1, 16, 9);
			fdMapGridCopyMetatileAttributes(38, 9, 1, 1, 24, 9);
			fdActorSetSprCol(fdActorGetById(ACTOR_TWISTER_ROCK_BY_THE_WALL),
				SPRITE_COLOR_GREY);
		}
	}
	if(MainRoutineStateWork.next == SCENE_AIRS_ROCK_ENTRANCE) {
		switch(MainRoutineStateWork.entrance) {
		case AIRS_ROCK_ENTRANCE_FROM_AIRS_ROCK_STORM_BRAND_CHEST_EAST_B:
			if(!syGameFlagGet(FLAG_KEEP_PARTY_POSITION))
				LEADER()->z.fixed += toFIXED(16);
			scLeaderLift(-toFIXED(72));
			goto breathing;
		case AIRS_ROCK_ENTRANCE_FROM_AIRS_ROCK_STORM_BRAND_CHEST_WEST:
		case AIRS_ROCK_ENTRANCE_FROM_AIRS_ROCK_STORM_BRAND_CHEST_EAST_A:
			if(!syGameFlagGet(FLAG_KEEP_PARTY_POSITION))
				LEADER()->z.fixed += toFIXED(16);
			scLeaderLift(-toFIXED(40));
			goto breathing;
		case ENTRANCE_DOWN_FROM_THE_CLIFF_WEST:
			if(!syGameFlagGet(FLAG_KEEP_PARTY_POSITION)) {
				LEADER()->x.fixed += toFIXED(48);
				LEADER()->y.fixed += toFIXED(16);
			}
		case ENTRANCE_DOWN_FROM_THE_CLIFF_EAST:
			scLeaderLift(toFIXED(56));
		case AIRS_ROCK_ENTRANCE_FROM_AIRS_ROCK_INSIDE_HALL:
		breathing:
			START_BREATHING(ACTOR_FIRST_SLOPE_STATUE, 60);
			START_BREATHING(ACTOR_SECOND_SLOPE_STATUE, 180);
			START_BREATHING(ACTOR_THIRD_SLOPE_STATUE, 0);
			START_BREATHING(ACTOR_FOURTH_SLOPE_STATUE, 60);
			if(syGameFlagGet(FLAG_AIRS_ROCK_PILLAR_PLACED_FIRST))
				fdActorSetPos(ACTOR_FIRST_LEDGE_PILLAR, 264, 696);
			if(syGameFlagGet(FLAG_AIRS_ROCK_PILLAR_PLACED_SECOND))
				fdActorSetPos(ACTOR_SECOND_LEDGE_PILLAR, 728, 728);
			if(syGameFlagGet(FLAG_AIRS_ROCK_PILLAR_PLACED_THIRD))
				fdActorSetPos(ACTOR_THIRD_LEDGE_PILLAR, 344, 776);
			if(syGameFlagGet(FLAG_AIRS_ROCK_PILLAR_PLACED_FOURTH))
				fdActorSetPos(ACTOR_FOURTH_LEDGE_PILLAR, 632, 792);
			fdActorPlaceBlocks(AirsRock_EntrancePillars);
			scBlocksMarkMetatiles(AirsRock_EntrancePillars);
			fdFlagBlockPlaceAll(AirsRock_FrozenPuddle);
			break;
		case ENTRANCE_UP_THE_CLIFF_EAST:
			if(!syGameFlagGet(FLAG_KEEP_PARTY_POSITION)) {
				LEADER()->x.fixed += toFIXED(16);
				LEADER()->y.fixed -= toFIXED(16);
			}
		case ENTRANCE_UP_THE_CLIFF_WEST:
			if(!syGameFlagGet(FLAG_KEEP_PARTY_POSITION))
				LEADER()->z.fixed += toFIXED(16);
			scLeaderLift(-toFIXED(56));
		case AIRS_ROCK_ENTRANCE_FROM_AIRS_ROCK_FUJIN_SHIELD_CHEST_B:
			if(syGameFlagGet(FLAG_AIRS_ROCK_PILLAR_PLACED_FIFTH))
				fdActorSetPos(ACTOR_FIFTH_LEDGE_PILLAR, 792, 296);
			fdActorPlaceBlocks(AirsRock_EntrancePillars);
			scBlocksMarkMetatiles(AirsRock_EntrancePillars);
			HANG_BREATHING(ACTOR_FIRST_CLIFF_STATUE, toFIXED(16), 60);
			HANG_BREATHING(ACTOR_THIRD_CLIFF_STATUE, toFIXED(80), 90);
			HANG_BREATHING(ACTOR_SECOND_CLIFF_STATUE, -toFIXED(48), 100);
			HANG_BREATHING(ACTOR_FOURTH_CLIFF_STATUE, toFIXED(144), 180);
			fdActorGetById(ACTOR_BREATHING_STATUE)->motionFlg = 0;
			fdActorGetById(ACTOR_BREATHING_STATUE)->y.fixed = -toFIXED(144);
			fdActorSetSprPriority(ACTOR_BREATHING_STATUE, 2);
			fdActorGetById(ACTOR_BREATHING_STATUE)->priFlg |=
				ACTOR_PRIORITY_UNDERFOOT;
			fdActorGetById(ACTOR_BREATHING_STATUE)->tmr = 180;
			fdActorGetById(ACTOR_BREATHING_STATUE)->sceneCnt = 1;
			fdActorGetById(ACTOR_BREATHING_STATUE)->update =
				scAirsRockStatueBreathesWhirlwind;
			fdActorSetSprFlags(fdActorGetById(ACTOR_BREATHING_STATUE), 0);
			break;
		}
	}
	if(MainRoutineStateWork.next == SCENE_AIRS_ROCK_FUJIN_SHIELD_CHEST) {
		switch(MainRoutineStateWork.entrance) {
		case AIRS_ROCK_FUJIN_SHIELD_CHEST_FROM_AIRS_ROCK_ENTRANCE:
		case ENTRANCE_LEDGES_FROM_LOWER_PATH:
		case ENTRANCE_LEDGES_FROM_UPPER_PATH:
			sySetFrameEvent(
				scAirsRockSetLedgeMetatilesByLevel, EVENT_PASS_FRAME);
			fdActorGetById(ACTOR_FIRST_BREATHING_STATUE)->motionFlg = 0;
			fdActorGetById(ACTOR_FIRST_BREATHING_STATUE)->y.fixed = -toFIXED(8);
			fdActorSetSprPriority(ACTOR_FIRST_BREATHING_STATUE, 2);
			fdActorGetById(ACTOR_FIRST_BREATHING_STATUE)->tmr = 60;
			fdActorGetById(ACTOR_FIRST_BREATHING_STATUE)->sceneCnt = 0;
			fdActorGetById(ACTOR_FIRST_BREATHING_STATUE)->update =
				scAirsRockStatueBreathesWhirlwind;
			fdActorSetSprFlags(fdActorGetById(ACTOR_FIRST_BREATHING_STATUE), 0);
			fdActorGetById(ACTOR_SECOND_BREATHING_STATUE)->motionFlg = 0;
			fdActorGetById(ACTOR_SECOND_BREATHING_STATUE)->y.fixed =
				-toFIXED(8);
			fdActorSetSprPriority(ACTOR_SECOND_BREATHING_STATUE, 2);
			fdActorGetById(ACTOR_SECOND_BREATHING_STATUE)->tmr = 180;
			fdActorGetById(ACTOR_SECOND_BREATHING_STATUE)->sceneCnt = 0;
			fdActorGetById(ACTOR_SECOND_BREATHING_STATUE)->update =
				scAirsRockStatueBreathesWhirlwind;
			fdActorSetSprFlags(
				fdActorGetById(ACTOR_SECOND_BREATHING_STATUE), 0);
			fdActorGetById(ACTOR_FIRST_PILLAR)->layer = 2;
			fdActorSetSprPriority(ACTOR_FIRST_PILLAR, 1);
			fdActorGetById(ACTOR_FIRST_PILLAR)->y.fixed = toFIXED(208);
			fdActorGetById(ACTOR_FIRST_PILLAR)->groundHeight =
				fdActorGetById(ACTOR_FIRST_PILLAR)->y.fixed;
			fdActorGetById(ACTOR_SECOND_PILLAR)->layer = 2;
			fdActorSetSprPriority(ACTOR_SECOND_PILLAR, 1);
			fdActorGetById(ACTOR_SECOND_PILLAR)->y.fixed = toFIXED(208);
			fdActorGetById(ACTOR_SECOND_PILLAR)->groundHeight =
				fdActorGetById(ACTOR_SECOND_PILLAR)->y.fixed;
			if(syGameFlagGet(FLAG_FIRST_PILLAR_MOVED_WEST))
				fdActorSetPos(ACTOR_FIRST_PILLAR, 504, 872);
			if(syGameFlagGet(FLAG_SECOND_PILLAR_MOVED_WEST))
				fdActorSetPos(ACTOR_SECOND_PILLAR, 520, 872);
			scAirsRockMoveSecondPillar();
			fdMapGridGetLayer(2)->ofsX = toFIXED(1408);
			fdMapGridGetLayer(2)->ofsY = toFIXED(576);
			fdMapGridGetLayer(2)->scaleX = toFIXED(1) / 4;
			fdMapGridGetLayer(2)->scaleY = toFIXED(1) / 4;
			fdMapGridCenterOnFollowedPoint();
			syWait(1);
			break;
		case ENTRANCE_BELOW_THE_FOG_WEST:
			if(!syGameFlagGet(FLAG_KEEP_PARTY_POSITION))
				LEADER()->y.fixed += toFIXED(24);
		case ENTRANCE_BELOW_THE_FOG_EAST:
			scLeaderLift(toFIXED(72));
		case ENTRANCE_LOWER_PATH:
		case ENTRANCE_UPPER_PATH:
			fdActorSetSprPriority(MainRoutineStateWork.leaderActorId, 1);
			break;
		case ENTRANCE_INTO_THE_FOG_WEST:
		case ENTRANCE_INTO_THE_FOG_EAST:
			if(!syGameFlagGet(FLAG_KEEP_PARTY_POSITION))
				LEADER()->z.fixed += toFIXED(16);
			scLeaderLift(-toFIXED(24));
			goto fog;
		case ENTRANCE_DOWN_INTO_THE_FOG_WEST:
			if(!syGameFlagGet(FLAG_KEEP_PARTY_POSITION))
				LEADER()->x.fixed += toFIXED(48);
		case ENTRANCE_DOWN_INTO_THE_FOG_EAST:
			scLeaderLift(toFIXED(144));
		fog:
			REG_BG1CNT &= ~BGCNT_PRIORITY(3);
			REG_BG1CNT |= BGCNT_PRIORITY(1);
			REG_BG2CNT &= ~BGCNT_PRIORITY(3);
			REG_BG2CNT |= BGCNT_PRIORITY(2);
			REG_BG3CNT &= ~BGCNT_PRIORITY(3);
			REG_BG3CNT |= BGCNT_PRIORITY(3);
			fdActorGetById(ACTOR_TWISTER_ROCK_IN_THE_FOG)->priFlg |=
				ACTOR_PRIORITY_UNDERFOOT;
			fdEffectStartBlankOverlay(0);
			if(syGameFlagGet(FLAG_AIRS_ROCK_THIRD_PILLAR_MOVED))
				fdActorSetPos(ACTOR_THIRD_PILLAR, 184, 280);
			if(!syGameFlagGet(FLAG_AIRS_ROCK_FOG_CLEARED)) {
				FieldStateWork->transition =
					SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
				REG_BLDALPHA = BLEND_ALPHA(FOG_WEIGHT, BLEND_WEIGHT_MAX);
				REG_BLDCNT =
					BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
				dma = (volatile DMA_REGISTERS *)REG_DMA0;
				while((Sint32)dma->ctrl < 0)
					;
				sySetFrameEvent(scAirsRockDriftFog, EVENT_PASS_VBLANK);
				fdMapGridGetLayer(2)->speedX = toFIXED(0.25);
				fdMapGridGetLayer(2)->speedY = -toFIXED(0.5);
				fdMapGridGetLayer(2)->maskX = FOG_WRAP - 1;
				fdMapGridGetLayer(2)->maskY = (FOG_WRAP - 1) * MAPGRID_SIZE;
			} else {
				REG_BLDALPHA = BLEND_ALPHA(0, BLEND_WEIGHT_MAX);
				REG_BLDCNT =
					BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
				REG_DISPCNT &= ~DISPCNT_BG1_ON;
				fdActorSetSprCol(fdActorGetById(ACTOR_TWISTER_ROCK_IN_THE_FOG),
					SPRITE_COLOR_GREY);
			}
			break;
		case ENTRANCE_ABOVE_THE_FOG_WEST:
			if(!syGameFlagGet(FLAG_KEEP_PARTY_POSITION))
				LEADER()->y.fixed -= toFIXED(32);
		case ENTRANCE_ABOVE_THE_FOG_EAST:
			if(!syGameFlagGet(FLAG_KEEP_PARTY_POSITION))
				LEADER()->z.fixed += toFIXED(16);
			scLeaderLift(-toFIXED(48));
		case AIRS_ROCK_FUJIN_SHIELD_CHEST_FROM_AIRS_ROCK_INSIDE_VIAL_CHEST:
			fdMapGridGetLayer(2)->ofsX = toFIXED(1152);
			fdMapGridGetLayer(2)->ofsY = toFIXED(800);
			fdMapGridGetLayer(2)->scaleX = toFIXED(13) / 16;
			fdMapGridGetLayer(2)->scaleY = toFIXED(13) / 16;
			if(!syGameFlagGet(FLAG_AIRS_ROCK_TOP_OPENED)) {
				fdMapGridCopyMetatilesTo(29, 9, 62, 2, 9, 9);
				fdMapGridCopyMetatileAttributes(29, 9, 9, 9, 62, 2);
				fdMapGridCopyMetatilesTo(29, 73, 62, 66, 9, 9);
				fdMapGridCopyMetatilesTo(52, 2, 29, 9, 9, 9);
				fdMapGridCopyMetatileAttributes(52, 2, 9, 9, 29, 9);
				fdMapGridCopyMetatilesTo(52, 66, 29, 73, 9, 9);
			} else {
				fdActorSetSprCol(fdActorGetById(ACTOR_TWISTER_ROCK_AT_THE_TOP),
					SPRITE_COLOR_GREY);
			}
			fdActorSetSprPriority(ACTOR_TWISTER_ROCK_AT_THE_TOP, 1);
			fdMapGridCenterOnFollowedPoint();
			syWait(1);
			if(syGameFlagGet(FLAG_KEEP_PARTY_POSITION)) {
				leader = LEADER();
				leader->layer = 0;
				leader->y.fixed = leader->groundHeight = fdMapGetTerrainHeight(
					0, leader->x.fixed, leader->z.fixed - leader->y.fixed);
				syScriptCameraFollowActor(
					MainRoutineStateWork.leaderActorId, 0);
			}
			break;
		case ENTRANCE_OPEN_THE_TOP:
			fdMapGridCopyMetatilesTo(29, 9, 62, 2, 9, 9);
			fdMapGridCopyMetatileAttributes(29, 9, 9, 9, 62, 2);
			fdMapGridCopyMetatilesTo(29, 73, 62, 66, 9, 9);
			fdMapGridCopyMetatilesTo(52, 2, 29, 9, 9, 9);
			fdMapGridCopyMetatileAttributes(52, 2, 9, 9, 29, 9);
			fdMapGridCopyMetatilesTo(52, 66, 29, 73, 9, 9);
			fdScriptStartScreenTransition();
			fdScriptWaitTransitionFrames();
			scAirsRockWhirlwindOpensTheTop();
			break;
		}
	}
	return 0;
}

Sint32 scPrepareMap(void)
{
	if(MainRoutineStateWork.next == SCENE_AIRS_ROCK_FUJIN_SHIELD_CHEST) {
		scAirsRockSetLedgeMetatilesByLevel();
	}
	return 0;
}

#undef WAVE_SCROLL
#undef WAVE_SCROLL_LINES
#undef LEADER
#undef FOG_WRAP
#undef START_BREATHING
#undef HANG_BREATHING
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SPRITE_PRIORITY

/* clang-format off */
#include "shared/field_effect_duration_scripts.c"

/* The five pillars on the entrance's ledges that Move pushes, ended by -1. */
Sint16 AirsRock_EntrancePillars[] = {
	ACTOR_FIRST_LEDGE_PILLAR,  ACTOR_SECOND_LEDGE_PILLAR, ACTOR_THIRD_LEDGE_PILLAR,
	ACTOR_FOURTH_LEDGE_PILLAR, ACTOR_FIFTH_LEDGE_PILLAR,  -1,
};

const Sint32 *FieldEffect_DurationScripts[] = {
	FieldEffect_ShortScript,
	FieldEffect_MediumScript,
	FieldEffect_LongScript,
};

Sint32 AirsRock_WindStoneScript[] = {
	SC_SET(SW_SCALE_Y, toFIXED(3) / 2),
	SC_ANIMATION(2),
	SC_CALL((Sint32)scAirsRockClearScriptedActorSprFlags),
	SC_END,
};

Uint32 AirsRock_WhirlwindFadesScript[] = {
	SC_ANIMATION(5),
	SC_WAIT(12),
	SC_RELEASE,
};

Uint32 AirsRock_WhirlwindRisesScript[] = {
	SC_WAIT(60),
	SC_ANIMATION(5),
	SC_WAIT(12),
	SC_RELEASE,
};

Sint32 AirsRock_RubbleScript[] = {
	SC_WAIT(1),
	SC_ADD(SW_SCALE_X, -toFIXED(1) / 50),
	SC_ADD(SW_SCALE_Y, -toFIXED(1) / 50),
	SC_LOOP(32, SC_TOP),
	SC_WAIT(1),
	SC_POSITION(0, 0, 0),
	SC_WAIT(1),
	SC_RELEASE,
};

/* The puddle Frost freezes into a pillar, and the flag set while it stands */
/* (fdFlagBlockPlaceAll). */
Sint16 AirsRock_FrozenPuddle[] = {
	ACTOR_PUDDLE, FLAG_PUDDLE_FROZEN, -1, 0,
};

#include "shared/scene_entrances_default.c"

SCENE_REGION AirsRock_StormBrandChestRegions[] = {
	{256, 96, 144, 272, 112, 160, CONDITION_ALWAYS, 2},
	{608, 96, 144, 624, 112, 160, CONDITION_ALWAYS, 3},
	{864, 96, 144, 880, 112, 160, CONDITION_ALWAYS, 4},
	{SCENE_TABLE_END},
};

SCENE_REGION AirsRock_EntranceRegions[] = {
	{256, -192, 816, 272, -176, 832, CONDITION_ALWAYS, 1},
	{608, -192, 816, 624, -176, 832, CONDITION_ALWAYS, 2},
	{864, -192, 816, 880, -176, 832, CONDITION_ALWAYS, 3},
	{624, 176, 704, 640, 192, 720, CONDITION_ALWAYS, 5},
	{784, 160, 688, 800, 176, 704, CONDITION_ALWAYS, 6},
	{624, -200, 320, 640, -184, 336, CONDITION_ALWAYS, 7},
	{784, -216, 304, 800, -200, 320, CONDITION_ALWAYS, 8},
	{SCENE_TABLE_END},
};

SCENE_REGION AirsRock_FujinShieldChestRegions[] = {
	{144, -96, 304, 160, -80, 320, CONDITION_ALWAYS, 6},
	{208, -96, 304, 224, -80, 320, CONDITION_ALWAYS, 7},
	{144, -80, 304, 160, -64, 320, CONDITION_ALWAYS, 8},
	{208, -80, 304, 224, -64, 320, CONDITION_ALWAYS, 9},
	{144, 160, 192, 160, 176, 208, CONDITION_ALWAYS, 10},
	{208, 160, 192, 224, 176, 208, CONDITION_ALWAYS, 11},
	{144, 160, 192, 160, 176, 208, CONDITION_ALWAYS, 10},
	{208, 160, 192, 224, 176, 208, CONDITION_ALWAYS, 11},
	{528, -96, 304, 544, -80, 320, CONDITION_ALWAYS, 12},
	{592, -96, 304, 608, -80, 320, CONDITION_ALWAYS, 13},
	{SCENE_TABLE_END},
};

Uint32 SceneExits[] = {
	SCENE_EXITS(AirsRockStormBrandChest),
	SCENE_EXIT_UNLESS(AIRS_ROCK_STORM_BRAND_CHEST_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_AIRS_ROCK_STORM_BRAND_CHEST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(AIRS_ROCK_STORM_BRAND_CHEST_TO_AIRS_ROCK_ENTRANCE_WEST, AirsRockEntrance, AIRS_ROCK_ENTRANCE_FROM_AIRS_ROCK_STORM_BRAND_CHEST_WEST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(AIRS_ROCK_STORM_BRAND_CHEST_TO_AIRS_ROCK_ENTRANCE_EAST_A, AirsRockEntrance, AIRS_ROCK_ENTRANCE_FROM_AIRS_ROCK_STORM_BRAND_CHEST_EAST_A, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(AIRS_ROCK_STORM_BRAND_CHEST_TO_AIRS_ROCK_ENTRANCE_EAST_B, AirsRockEntrance, AIRS_ROCK_ENTRANCE_FROM_AIRS_ROCK_STORM_BRAND_CHEST_EAST_B, EXIT_FLAG_NONE),
	SCENE_EXITS(AirsRockEntrance),
	SCENE_EXIT_UNLESS(AIRS_ROCK_ENTRANCE_TO_AIRS_ROCK_STORM_BRAND_CHEST_WEST, AirsRockStormBrandChest, AIRS_ROCK_STORM_BRAND_CHEST_FROM_AIRS_ROCK_ENTRANCE_WEST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(AIRS_ROCK_ENTRANCE_TO_AIRS_ROCK_STORM_BRAND_CHEST_EAST_A, AirsRockStormBrandChest, AIRS_ROCK_STORM_BRAND_CHEST_FROM_AIRS_ROCK_ENTRANCE_EAST_A, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(AIRS_ROCK_ENTRANCE_TO_AIRS_ROCK_STORM_BRAND_CHEST_EAST_B, AirsRockStormBrandChest, AIRS_ROCK_STORM_BRAND_CHEST_FROM_AIRS_ROCK_ENTRANCE_EAST_B, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(AIRS_ROCK_ENTRANCE_TO_AIRS_ROCK_INSIDE_HALL, AirsRockInsideHall, AIRS_ROCK_INSIDE_HALL_FROM_AIRS_ROCK_ENTRANCE, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(5, AirsRockEntrance, ENTRANCE_UP_THE_CLIFF_WEST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(6, AirsRockEntrance, ENTRANCE_UP_THE_CLIFF_EAST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(7, AirsRockEntrance, ENTRANCE_DOWN_FROM_THE_CLIFF_WEST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(8, AirsRockEntrance, ENTRANCE_DOWN_FROM_THE_CLIFF_EAST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(AIRS_ROCK_ENTRANCE_TO_AIRS_ROCK_FUJIN_SHIELD_CHEST_A, AirsRockFujinShieldChest, AIRS_ROCK_FUJIN_SHIELD_CHEST_FROM_AIRS_ROCK_ENTRANCE, EXIT_FLAG_NONE),
	SCENE_EXITS(AirsRockFujinShieldChest),
	SCENE_EXIT_UNLESS(AIRS_ROCK_FUJIN_SHIELD_CHEST_TO_AIRS_ROCK_ENTRANCE, AirsRockEntrance, AIRS_ROCK_ENTRANCE_FROM_AIRS_ROCK_FUJIN_SHIELD_CHEST_B, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(2, AirsRockFujinShieldChest, ENTRANCE_LOWER_PATH, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(3, AirsRockFujinShieldChest, ENTRANCE_UPPER_PATH, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(4, AirsRockFujinShieldChest, ENTRANCE_LEDGES_FROM_LOWER_PATH, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(5, AirsRockFujinShieldChest, ENTRANCE_LEDGES_FROM_UPPER_PATH, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(6, AirsRockFujinShieldChest, ENTRANCE_INTO_THE_FOG_WEST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(7, AirsRockFujinShieldChest, ENTRANCE_INTO_THE_FOG_EAST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(8, AirsRockFujinShieldChest, ENTRANCE_BELOW_THE_FOG_WEST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(9, AirsRockFujinShieldChest, ENTRANCE_BELOW_THE_FOG_EAST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(10, AirsRockFujinShieldChest, ENTRANCE_INTO_THE_FOG_WEST, FLAG_AIRS_ROCK_FOG_CLEARED),
	SCENE_EXIT_UNLESS(11, AirsRockFujinShieldChest, ENTRANCE_INTO_THE_FOG_EAST, FLAG_AIRS_ROCK_FOG_CLEARED),
	SCENE_EXIT_UNLESS(10, AirsRockFujinShieldChest, ENTRANCE_ABOVE_THE_FOG_WEST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(11, AirsRockFujinShieldChest, ENTRANCE_ABOVE_THE_FOG_EAST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(12, AirsRockFujinShieldChest, ENTRANCE_DOWN_INTO_THE_FOG_WEST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(13, AirsRockFujinShieldChest, ENTRANCE_DOWN_INTO_THE_FOG_EAST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(AIRS_ROCK_FUJIN_SHIELD_CHEST_TO_AIRS_ROCK_INSIDE_VIAL_CHEST, AirsRockInsideVialChest, AIRS_ROCK_INSIDE_VIAL_CHEST_FROM_AIRS_ROCK_FUJIN_SHIELD_CHEST, EXIT_FLAG_NONE),
	SCENE_EXITS_END,
};

ACTOR_PLACE AirsRock_StormBrandChestPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_TWISTER_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 328, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WIND_STONE, CONDITION_ALWAYS, (Sint32)AirsRock_WindStoneScript, 920, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WIND_STONE, CONDITION_ALWAYS, (Sint32)AirsRock_WindStoneScript, 104, 0, 760, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WIND_STONE, CONDITION_ALWAYS, (Sint32)AirsRock_WindStoneScript, 376, 0, 808, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WIND_STONE, CONDITION_ALWAYS, (Sint32)AirsRock_WindStoneScript, 632, 0, 888, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WIND_STONE, CONDITION_ALWAYS, (Sint32)AirsRock_WindStoneScript, 872, 0, 856, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

ACTOR_PLACE AirsRock_EntrancePlacements[] = {
	{SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1, 0, 0, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 2, 0, 0, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 3, 0, 0, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 4, 0, 0, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 6, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 7, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 9, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 10, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	ACTOR_PLACEMENT(SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 536, 0, 296, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 680, 0, 296, FACING_WEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 536, 0, 296, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 648, 0, 296, FACING_WEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 712, 0, 312, FACING_NORTH, TALK_FACING_KEEP, 0),
	{SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 11, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SCENE_TABLE_END},
};

ACTOR_PLACE AirsRock_FujinShieldChestPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 536, 0, 904, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 712, 0, 904, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 872, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 872, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_TWISTER_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 152, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 280, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_TWISTER_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 536, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

CUE AirsRock_StormBrandChestCues[] = {
	{CUE_EXIT, 1, CONDITION_ALWAYS, 1},
	{CUE_EXAMINE, 20, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockExamineWindStone)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), ACTOR_TWISTER_ROCK_BY_THE_WALL, FLAG_AIRS_ROCK_WALL_CLEARED, SCRIPT_REF(scAirsRockStartWhirlwind)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_TWISTER_ROCK_BY_THE_WALL, FLAG_AIRS_ROCK_WALL_CLEARED, SCRIPT_REF(scAirsRockTakeOverWhirlwind)},
	{PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_TWISTER_ROCK_BY_THE_WALL, FLAG_AIRS_ROCK_WALL_CLEARED, SCRIPT_REF(scAirsRockWhirlwindClearsWall)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), ACTOR_WIND_STONE_A, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockStartWhirlwind)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_WIND_STONE_A, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockTakeOverWhirlwind)},
	{PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_WIND_STONE_A, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockBlowGust)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), ACTOR_WIND_STONE_B, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockStartWhirlwind)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_WIND_STONE_B, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockTakeOverWhirlwind)},
	{PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_WIND_STONE_B, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockBlowGust)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), ACTOR_WIND_STONE_C, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockStartWhirlwind)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_WIND_STONE_C, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockTakeOverWhirlwind)},
	{PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_WIND_STONE_C, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockBlowGust)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), ACTOR_WIND_STONE_D, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockStartWhirlwind)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_WIND_STONE_D, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockTakeOverWhirlwind)},
	{PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_WIND_STONE_D, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockBlowGust)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), ACTOR_WIND_STONE_E, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockStartWhirlwind)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_WIND_STONE_E, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockTakeOverWhirlwind)},
	{PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_WIND_STONE_E, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockBlowGust)},
	{CUE_TABLE_END},
};

CUE AirsRock_EntranceCues[] = {
	{STEP_CUE_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockEnterTheRock)},
	{CUE_EXIT, 2, CONDITION_ALWAYS, 9},
	{CUE_PUSH, 0, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockLiftPillar)},
	{CUE_PUSHED, 0, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockPlacePillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FIRST_LEDGE_PILLAR, FLAG_AIRS_ROCK_PILLAR_PLACED_FIRST, SCRIPT_REF(scAirsRockLiftPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_LEDGE_PILLAR, FLAG_AIRS_ROCK_PILLAR_PLACED_FIRST, SCRIPT_REF(scAirsRockPlacePillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_SECOND_LEDGE_PILLAR, FLAG_AIRS_ROCK_PILLAR_PLACED_SECOND, SCRIPT_REF(scAirsRockLiftPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SECOND_LEDGE_PILLAR, FLAG_AIRS_ROCK_PILLAR_PLACED_SECOND, SCRIPT_REF(scAirsRockPlacePillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_THIRD_LEDGE_PILLAR, FLAG_AIRS_ROCK_PILLAR_PLACED_THIRD, SCRIPT_REF(scAirsRockLiftPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_THIRD_LEDGE_PILLAR, FLAG_AIRS_ROCK_PILLAR_PLACED_THIRD, SCRIPT_REF(scAirsRockPlacePillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FOURTH_LEDGE_PILLAR, FLAG_AIRS_ROCK_PILLAR_PLACED_FOURTH, SCRIPT_REF(scAirsRockLiftPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FOURTH_LEDGE_PILLAR, FLAG_AIRS_ROCK_PILLAR_PLACED_FOURTH, SCRIPT_REF(scAirsRockPlacePillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PUDDLE, FLAG_PUDDLE_FROZEN, SCRIPT_REF(scAirsRockFreezePuddle)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FIFTH_LEDGE_PILLAR, FLAG_AIRS_ROCK_PILLAR_PLACED_FIFTH, SCRIPT_REF(scAirsRockLiftPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIFTH_LEDGE_PILLAR, FLAG_AIRS_ROCK_PILLAR_PLACED_FIFTH, SCRIPT_REF(scAirsRockPlacePillar)},
	{CUE_RAISED, RAISED_RIDE_WHIRLWIND, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockRideWhirlwind)},
	{CUE_TABLE_END},
};

CUE AirsRock_FujinShieldChestCues[] = {
	{CUE_EXIT, 1, CONDITION_ALWAYS, 1},
	{CUE_EXIT, 2, CONDITION_ALWAYS, 2},
	{CUE_EXIT, 3, CONDITION_ALWAYS, 3},
	{CUE_EXIT, 4, CONDITION_ALWAYS, 4},
	{CUE_EXIT, 5, CONDITION_ALWAYS, 5},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 6, CONDITION_ALWAYS, 14},
	{STEP_CUE_WALKING_INTO(FACING_SOUTH), 20, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockDropOffLedge)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_PILLAR, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockMoveFirstPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SECOND_PILLAR, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockMoveSecondPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_THIRD_PILLAR, FLAG_AIRS_ROCK_THIRD_PILLAR_MOVED, SCRIPT_REF(scAirsRockMoveThirdPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), ACTOR_TWISTER_ROCK_IN_THE_FOG, FLAG_AIRS_ROCK_FOG_CLEARED, SCRIPT_REF(scAirsRockStartWhirlwind)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_TWISTER_ROCK_IN_THE_FOG, FLAG_AIRS_ROCK_FOG_CLEARED, SCRIPT_REF(scAirsRockTakeOverWhirlwind)},
	{PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_TWISTER_ROCK_IN_THE_FOG, FLAG_AIRS_ROCK_FOG_CLEARED, SCRIPT_REF(scAirsRockWhirlwindClearsFog)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), ACTOR_TWISTER_ROCK_AT_THE_TOP, FLAG_AIRS_ROCK_TOP_OPENED, SCRIPT_REF(scAirsRockStartWhirlwind)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_TWISTER_ROCK_AT_THE_TOP, FLAG_AIRS_ROCK_TOP_OPENED, SCRIPT_REF(scAirsRockTakeOverWhirlwind)},
	{PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_TWISTER_ROCK_AT_THE_TOP, FLAG_AIRS_ROCK_TOP_OPENED, SCRIPT_REF(scAirsRockWhirlwindOpensTheTop)},
	{CUE_RAISED, RAISED_RIDE_WHIRLWIND, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockRideWhirlwind)},
	{CUE_TABLE_END},
};

/* The kinds of barrier a gust sweeps away (AIRS_ROCK_GUST_BARRIER), a row */
/* each: the map metatile the cleared ground's picture is copied from, and */
/* how far the barrier's pictures lie from the metatiles the gust crosses, 64 */
/* metatiles east. */
/* An odd kind clears a column three metatiles tall, an even one a metatile; */
/* no barrier is of kind 1. */
Sint16 AirsRock_GustTargets[][4] = {
	{64, 0, 64, 0}, {64, 0, 64, 0}, {64, 0, 64, 0}, {64, 0, 64, 0},
};
/* clang-format on */

/* The fog's blend weight, in sixteenths, and its step every sixteenth frame: it
   thickens and thins between FOG_WEIGHT / 2 and FOG_WEIGHT
   (scAirsRockDriftFog). */
Sint8 AirsRock_FogAlpha = 6;
Sint8 AirsRock_FogAlphaStep = 1;

/* As AirsRock_FogAlpha, while the fog lifts (scAirsRockLiftFog). */
Sint8 AirsRock_LiftingFogAlpha = 6;
Sint8 AirsRock_LiftingFogAlphaStep = 1;

/* The ledges' level the leader stood on when their metatiles were last swapped;
   -1 before. */
Sint32 AirsRock_LeaderLevel = -1;
