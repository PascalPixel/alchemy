/* Map ground: the height function of each metatile kind, the world map's battle
   environments and the sprite viewer's small tables after them; then the actor
   scripts the field starts by itself, the actor script command table and the
   work value handlers. Each map's resources follow in map_resources.s. */

#include "field/field.h"
#include "field/curve.h"
#include "field/script_operands.h"
#include "field/actor_script.h"
#include "field/map_grid.h"

/* --------------------------------------------------------------------------
   Map terrain: the height function of each metatile kind, the world map's
   battle environments, and the small tables after them.
   -------------------------------------------------------------------------- */

/* Each height function reads only the arguments it needs, so the type leaves
   them unspecified. */
typedef Sint32 (*TERRAIN_HEIGHT_FN)();

FIXED fdTerrainGetStepDifferenceDiagonalHeight();
FIXED fdTerrainGetBendSumDiagonalHeight();
FIXED fdTerrainGetBendDifferenceDiagonalHeight();
FIXED fdTerrainGetRoundedSquareHeight();
FIXED fdTerrainGetStepXHeight();
FIXED fdTerrainGetStepYHeight();
FIXED fdTerrainGetSampleTableTransposedHeight();
FIXED fdTerrainGetSampleTableTransposedMirroredHeight();
FIXED fdTerrainGetSampleTableMirroredHeight();
FIXED fdTerrainGetSplitTopStepHeight();
FIXED fdTerrainGetSplitBottomStepHeight();

/* Ground height functions, by TERRAIN_KIND, called with (the remaining samples,
   x within the metatile, y within the metatile). Each reads only the arguments
   it needs. */
const TERRAIN_HEIGHT_FN MapGridTerrainHeightFunctions[TERRAIN_KIND_COUNT] = {
	[TERRAIN_FLAT] = fdTerrainGetFlatHeight,
	[TERRAIN_SLOPE_X] = fdTerrainGetSlopeXHeight,
	[TERRAIN_SLOPE_Y] = fdTerrainGetSlopeYHeight,
	[TERRAIN_STEP_SUM_DIAGONAL] = fdTerrainGetStepSumDiagonalHeight,
	[TERRAIN_STEP_DIFFERENCE_DIAGONAL] =
		fdTerrainGetStepDifferenceDiagonalHeight,
	[TERRAIN_BEND_SUM_DIAGONAL] = fdTerrainGetBendSumDiagonalHeight,
	[TERRAIN_BEND_DIFFERENCE_DIAGONAL] =
		fdTerrainGetBendDifferenceDiagonalHeight,
	[TERRAIN_ROUNDED_SQUARE] = fdTerrainGetRoundedSquareHeight,
	[TERRAIN_STEP_X] = fdTerrainGetStepXHeight,
	[TERRAIN_STEP_Y] = fdTerrainGetStepYHeight,
	[TERRAIN_SAMPLE_TABLE_TRANSPOSED] = fdTerrainGetSampleTableTransposedHeight,
	[TERRAIN_SAMPLE_TABLE_TRANSPOSED_MIRRORED] =
		fdTerrainGetSampleTableTransposedMirroredHeight,
	[TERRAIN_SAMPLE_TABLE] = fdTerrainGetSampleTableHeight,
	[TERRAIN_SAMPLE_TABLE_MIRRORED] = fdTerrainGetSampleTableMirroredHeight,
	[TERRAIN_SPLIT_TOP_STEP] = fdTerrainGetSplitTopStepHeight,
	[TERRAIN_SPLIT_BOTTOM_STEP] = fdTerrainGetSplitBottomStepHeight,
};

/* The world map's battle environments, read at a collision value plus a set of
   rows (fdWorldMapGetBattleEnvironment): the plain rows; the beach rows for a
   metatile with its beach bit; the snowfield rows for the encounter regions
   from SNOWFIELD_REGION_FIRST; the desert rows for the regions round Air's Rock
   and north of Shaman Village; the blizzard rows for the region round Prox. */
/* clang-format off */
/* ENV(name) is BATTLE_ENVIRONMENT_name, to keep each row's sixteen short. */
#define ENV(name) BATTLE_ENVIRONMENT_##name
/* The World Map's battle environments, by collision value, sixteen to each */
/* of its sets of rows. */
const Uint8 WorldMapTerrainBehaviorTable[80] = {
	/* plain */
	ENV(NONE), ENV(SEA), ENV(SEA), ENV(SEA), ENV(SEA_LAST), ENV(GRASSLAND),
	ENV(SHALLOWS), ENV(GRASSLAND), ENV(GRASSLAND), ENV(FOREST), ENV(GRASSLAND),
	ENV(GRASSLAND), ENV(GRASSLAND), ENV(GRASSLAND), ENV(NONE), ENV(NONE),
	/* beach */
	ENV(NONE), ENV(SEA), ENV(SEA), ENV(SEA), ENV(SEA_LAST), ENV(BEACH),
	ENV(SHALLOWS), ENV(GRASSLAND), ENV(GRASSLAND), ENV(FOREST), ENV(GRASSLAND),
	ENV(GRASSLAND), ENV(GRASSLAND), ENV(GRASSLAND), ENV(NONE), ENV(NONE),
	/* snowfield */
	ENV(NONE), ENV(SNOWFIELD_SEA), ENV(SNOWFIELD_SEA), ENV(SNOWFIELD_SEA),
	ENV(SNOWFIELD_SEA), ENV(SNOWFIELD), ENV(SNOWFIELD), ENV(SNOWFIELD),
	ENV(SNOWFIELD), ENV(SNOW_FOREST), ENV(SNOWFIELD), ENV(SNOWFIELD),
	ENV(SNOWFIELD), ENV(SNOWFIELD), ENV(NONE), ENV(NONE),
	/* desert */
	ENV(NONE), ENV(SEA), ENV(SEA), ENV(SEA), ENV(SEA_LAST), ENV(DESERT),
	ENV(SHALLOWS), ENV(GRASSLAND), ENV(GRASSLAND), ENV(FOREST), ENV(GRASSLAND),
	ENV(GRASSLAND), ENV(GRASSLAND), ENV(GRASSLAND), ENV(NONE), ENV(NONE),
	/* blizzard */
	ENV(NONE), ENV(SNOWFIELD_SEA), ENV(SNOWFIELD_SEA), ENV(SNOWFIELD_SEA),
	ENV(SNOWFIELD_SEA), ENV(BLIZZARD), ENV(BLIZZARD), ENV(BLIZZARD),
	ENV(BLIZZARD), ENV(BLIZZARD_FOREST), ENV(BLIZZARD), ENV(BLIZZARD),
	ENV(BLIZZARD), ENV(BLIZZARD), ENV(NONE), ENV(NONE),
};
#undef ENV

/* The sprite viewer's backdrops, which B steps through: black, blue, green, */
/* red, dark grey, grey, white and dark red. */
const Uint16 SpriteViewerBackdropColors[8] = {
	RGB(0, 0, 0),    RGB(0, 0, 31),   RGB(0, 31, 0),
	RGB(31, 0, 0),   RGB(7, 7, 7),    RGB(15, 15, 15),
	RGB(31, 31, 31), RGB(15, 0, 0),
};

/* The sprite ids the sprite viewer's fast step (Start held with L or R) */
/* stops at, the first of each group of sprites: a skip forward goes to the */
/* next stop above the sprite shown, back to the last stop below it. (Uint16)-1 */
/* ends the list. */
const Uint16 SpriteViewerSkipStops[10] = {
	SPRITE_ISAAC,              /* the party */
	SPRITE_BRIGGS,             /* the people */
	SPRITE_VENUS_DJINNI,       /* the Djinn */
	SPRITE_PUNCH_ANT,          /* monsters on the field */
	SPRITE_VINE_PILLAR,        /* things on the field */
	SPRITE_BATTLE_ISAAC,       /* the party in battle, and their weapons */
	SPRITE_BATTLE_BLUE_DRAGON, /* monsters in battle */
	SPRITE_THOR,               /* the summons */
	SPRITE_STAR_BURST,         /* effects */
	(Uint16)-1,
};
/* clang-format on */

/* Unused: nothing refers to it. Eight directions at a radius of 40 (x, y). */
const Sint8 WorldMapDirectionOffsets[8][2] = {
	{0, -40},
	{28, -28},
	{40, 0},
	{28, 28},
	{0, 40},
	{-28, 28},
	{-40, 0},
	{-28, -28},
};

/* Unused: nothing refers to it. A scale of 1 on both axes (16.16). */
const Uint32 WorldMapUnitScale[2] = {
	toFIXED(1),
	toFIXED(1),
};

/* The sprite viewer's scale for its ring of copies: full size. */
const SPR_SCALE SpriteViewerScale = {{{toFIXED(1), toFIXED(1)}}};

/* --------------------------------------------------------------------------
   Actor scripts: the ones the field starts by itself, the command table and the
   work value handlers SC_SET, SC_ADD and SC_CMP call.
   -------------------------------------------------------------------------- */

Sint32 fdActorScriptWait();
Bool   fdActorScriptWaitForArrival();
Sint32 fdActorScriptSetPosAndResetMotion();
Sint32 fdActorScriptResetToOrigin();
Sint32 fdActorScriptApplyAbsolutePos();
Sint32 fdActorScriptApplyRelativePos();
Sint32 fdActorScriptStoreAngleToWatchedActor();
Sint32 fdActorScriptApplyWatchedActorPos();
Sint32 fdActorScriptApplyLocalOffsetPos();
Bool   fdActorWander();
Sint32 fdActorScriptWanderNearHome();
Bool   fdActorApproachWatchedActor();
Sint32 fdCameraFollowLeaderInBounds();
Sint32 fdActorScriptRepeatOrJump();
Sint32 fdActorScriptJumpToLabel();
Uint32 fdActorScriptJumpIfTrue();
Uint32 fdActorScriptJumpIfFalse();
Sint32 fdActorScriptEnd();
Sint32 fdActorScriptTestFlag();
Sint32 fdActorScriptSetFlagAndTest();
Sint32 fdActorScriptClearFlagAndTest();
Sint32 fdActorScriptToggleFlagAndTest();
Sint32 fdActorScriptApplyOperandSet();
Sint32 fdActorScriptApplyOperandAdd();
Sint32 fdActorScriptApplyOperandCompare();
Sint32 fdActorUpdatePlayerControl();
Sint32 fdActorMoveOnWorldMap();
Sint32 fdActorMoveByKeys();
Sint32 fdActorCrossRope();
Sint32 fdActorCrossTightrope();
Sint32 fdActorMoveAsSand();
Sint32 fdActorSailOnMap();
Sint32 fdActorSailOnWorldMap();
Sint32 fdActorFlyOnWorldMap();
Sint32 fdActorMoveAsSandOnWorldMap();
Bool   fdActorUpdateHoverControl();
Sint32 fdActorUpdateWalkingControl();
Sint32 fdCheckConfiguredKeysAndCount();
Sint32 fdActorScriptRelease();
Sint32 fdActorScriptSetAnimation();
Sint32 fdActorScriptNoOperation();
Sint32 fdActorScriptPlaySound();
Sint32 fdActorScriptPlaySoundInView();
Bool   fdActorScriptInvokeCallback();
Sint32 fdCameraFollowWatchedInBounds();
Sint32 fdActorScriptSetMoveTargetXZ();

/* Each runs one control command every tick: the command, SC_WAIT(1),
   SC_LOOP(SC_FOREVER, SC_TOP). */
const Sint32 ActorKeyCheckScript[] = {
	SC_KEY_COUNT,
	SC_WAIT(1),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

/* Climbing by the keys (fdActorMoveByKeys). */
const Sint32 ActorKeysScript[] = {
	SC_KEYS,
	SC_WAIT(1),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

/* Crossing a tied rope (fdActorCrossRope). */
const Sint32 ActorRopeScript[] = {
	SC_ROPE,
	SC_WAIT(1),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

/* Crossing a tightrope (fdActorCrossTightrope). */
const Sint32 ActorTightropeScript[] = {
	SC_TIGHTROPE,
	SC_WAIT(1),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

/* Moving as sand (fdActorMoveAsSand). */
const Sint32 ActorSandScript[] = {
	SC_SAND,
	SC_WAIT(1),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

/* Hovering (fdActorUpdateHoverControl). */
const Sint32 ActorHoverScript[] = {
	SC_HOVER,
	SC_WAIT(1),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

/* Walking (fdActorUpdateWalkingControl). */
const Sint32 ActorWalkingScript[] = {
	SC_WALK,
	SC_WAIT(1),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

/* The leader on foot (fdActorUpdatePlayerControl). */
const Sint32 ActorPlayerScript[] = {
	SC_PLAYER,
	SC_WAIT(1),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

/* The leader on the World Map (fdActorMoveOnWorldMap). */
const Sint32 ActorWorldMapScript[] = {
	SC_WORLD,
	SC_WAIT(1),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

/* Sailing on a map (fdActorSailOnMap). */
const Sint32 ActorSailOnMapScript[] = {
	SC_SAIL,
	SC_WAIT(1),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

/* Sailing on the World Map (fdActorSailOnWorldMap). */
const Sint32 ActorSailScript[] = {
	SC_WORLD_SAIL,
	SC_WAIT(1),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

/* Flying on the World Map (fdActorFlyOnWorldMap). */
const Sint32 ActorFlyScript[] = {
	SC_WORLD_FLY,
	SC_WAIT(1),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

/* Moving as sand on the World Map (fdActorMoveAsSandOnWorldMap). */
const Sint32 ActorSandOnWorldMapScript[] = {
	SC_WORLD_SAND,
	SC_WAIT(1),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

/* The camera following the leader within the map's bounds. */
const Sint32 ActorCameraScript[] = {
	SC_CAMERA,
	SC_WAIT(1),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

/* Following the watched actor (fdActorApproachWatchedActor). */
const Sint32 ActorApproachWatchedScript[] = {
	SC_APPROACH_WATCHED,
	SC_WAIT(1),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

/* A script that ends at once. */
const Sint32 ActorScriptEndScript[] = {
	SC_END,
};

/* The handler of each command. 0x31 to 0x35 repeat SC_END's. */
const SCRIPT_COMMAND_FN ActorScriptCommandTable[54] = {
	fdActorScriptWait,							 /* 0x00 SC_WAIT */
	fdActorScriptWaitForArrival,				 /* 0x01 SC_SYNC */
	fdActorScriptSetPosAndResetMotion,			 /* 0x02 SC_POSITION */
	fdActorScriptResetToOrigin,					 /* 0x03 SC_POSITION_ORIGIN */
	fdActorScriptApplyAbsolutePos,				 /* 0x04 SC_MOVE */
	fdActorScriptApplyRelativePos,				 /* 0x05 SC_MOVE_BY */
	fdActorScriptStoreAngleToWatchedActor,		 /* 0x06 SC_FACE_WATCHED */
	fdActorScriptApplyWatchedActorPos,			 /* 0x07 SC_MOVE_TO_WATCHED */
	fdActorScriptApplyLocalOffsetPos,			 /* 0x08 SC_MOVE_LOCAL */
	fdActorWander,								 /* 0x09 SC_WANDER */
	fdActorScriptWanderNearHome,				 /* 0x0a SC_WANDER_HOME */
	fdActorApproachWatchedActor,				 /* 0x0b SC_APPROACH_WATCHED */
	fdCameraFollowLeaderInBounds,				 /* 0x0c SC_CAMERA */
	fdActorScriptRepeatOrJump,					 /* 0x0d SC_LOOP */
	fdActorScriptJumpToLabel,					 /* 0x0e SC_JUMP */
	(SCRIPT_COMMAND_FN)fdActorScriptJumpIfTrue,	 /* 0x0f SC_IF */
	(SCRIPT_COMMAND_FN)fdActorScriptJumpIfFalse, /* 0x10 SC_IFNOT */
	fdActorScriptEnd,							 /* 0x11 SC_END */
	fdActorScriptTestFlag,						 /* 0x12 SC_FLAG_CHECK */
	fdActorScriptSetFlagAndTest,				 /* 0x13 SC_FLAG_ON */
	fdActorScriptClearFlagAndTest,				 /* 0x14 SC_FLAG_OFF */
	fdActorScriptToggleFlagAndTest,				 /* 0x15 SC_FLAG_REV */
	fdActorScriptApplyOperandSet,				 /* 0x16 SC_SET */
	fdActorScriptApplyOperandAdd,				 /* 0x17 SC_ADD */
	fdActorScriptApplyOperandCompare,			 /* 0x18 SC_CMP */
	fdActorUpdatePlayerControl,					 /* 0x19 SC_PLAYER */
	fdActorMoveOnWorldMap,						 /* 0x1a SC_WORLD */
	fdActorMoveByKeys,							 /* 0x1b SC_KEYS */
	fdActorCrossRope,							 /* 0x1c SC_ROPE */
	fdActorCrossTightrope,						 /* 0x1d SC_TIGHTROPE */
	fdActorMoveAsSand,							 /* 0x1e SC_SAND */
	fdActorSailOnMap,							 /* 0x1f SC_SAIL */
	fdActorSailOnWorldMap,						 /* 0x20 SC_WORLD_SAIL */
	fdActorFlyOnWorldMap,						 /* 0x21 SC_WORLD_FLY */
	fdActorMoveAsSandOnWorldMap,				 /* 0x22 SC_WORLD_SAND */
	fdActorUpdateHoverControl,					 /* 0x23 SC_HOVER */
	fdActorUpdateWalkingControl,				 /* 0x24 SC_WALK */
	fdCheckConfiguredKeysAndCount,				 /* 0x25 SC_KEY_COUNT */
	fdActorScriptRelease,						 /* 0x26 SC_RELEASE */
	fdActorScriptSetAnimation,					 /* 0x27 SC_ANIMATION */
	fdActorScriptNoOperation,					 /* 0x28 SC_NOP */
	fdActorScriptPlaySound,						 /* 0x29 SC_SE */
	fdActorScriptPlaySoundInView,				 /* 0x2a SC_SE_IN_VIEW */
	fdActorScriptGoto,							 /* 0x2b SC_GOTO */
	fdActorScriptHideSprites,					 /* 0x2c SC_HIDE_SPRITES */
	fdActorScriptShowSprite,					 /* 0x2d SC_SHOW_SPRITE */
	fdActorScriptInvokeCallback,				 /* 0x2e SC_CALL */
	fdCameraFollowWatchedInBounds,				 /* 0x2f SC_CAMERA_IN_BOUNDS */
	fdActorScriptSetMoveTargetXZ,				 /* 0x30 SC_MOVE_XZ */
	fdActorScriptEnd,							 /* 0x31 */
	fdActorScriptEnd,							 /* 0x32 */
	fdActorScriptEnd,							 /* 0x33 */
	fdActorScriptEnd,							 /* 0x34 */
	fdActorScriptEnd,							 /* 0x35 */
};

/* The handler of each work value (ACTOR_SCRIPT_VALUE in actor_script.h). */
const OPERAND_FUNC ActorScriptOperandHandlerTable[41] = {
	fdActorScriptSetOrCompareScriptAddress,		/* 0x00 SW_SCRIPT */
	fdActorScriptSetOrCompareCursor,			/* 0x01 SW_CURSOR */
	fdActorScriptSetOrCompareFacing,			/* 0x02 */
	fdActorScriptSetOrCompareFacing,			/* 0x03 SW_FACING */
	fdActorScriptSetOrCompareFacing,			/* 0x04 */
	fdActorScriptSetOrCompareX,					/* 0x05 SW_X */
	fdActorScriptSetOrCompareY,					/* 0x06 SW_Y */
	fdActorScriptSetOrCompareZ,					/* 0x07 SW_Z */
	fdActorScriptSetOrCompareRadius,			/* 0x08 SW_RADIUS */
	fdActorScriptSetOrCompareScaleX,			/* 0x09 SW_SCALE_X */
	fdActorScriptSetOrCompareScaleY,			/* 0x0a SW_SCALE_Y */
	fdActorScriptSetOrCompareScaleY,			/* 0x0b */
	fdActorScriptSetOrCompareVelocityX,			/* 0x0c SW_VELOCITY_X */
	fdActorScriptSetOrCompareVelocityY,			/* 0x0d SW_VELOCITY_Y */
	fdActorScriptSetOrCompareVelocityZ,			/* 0x0e SW_VELOCITY_Z */
	fdActorScriptSetOrCompareSpeedLimit,		/* 0x0f SW_SPEED */
	fdActorScriptSetOrCompareAcceleration,		/* 0x10 SW_ACCELERATION */
	fdActorScriptSetOrCompareTargetX,			/* 0x11 SW_TARGET_X */
	fdActorScriptSetOrCompareTargetY,			/* 0x12 SW_TARGET_Y */
	fdActorScriptSetOrCompareTargetZ,			/* 0x13 SW_TARGET_Z */
	fdActorScriptSetOrCompareBounce,			/* 0x14 SW_BOUNCE */
	fdActorScriptSetOrCompareGravity,			/* 0x15 SW_GRAVITY */
	fdActorScriptSetOrCompareTerrainHeight,		/* 0x16 SW_GROUND */
	fdActorScriptSetOrCompareSceneWord,			/* 0x17 */
	fdActorScriptSetOrCompareSprite,			/* 0x18 */
	fdActorScriptSetOrCompareSpriteStorageKind, /* 0x19 */
	fdActorScriptSetOrCompareMotionFlags,		/* 0x1a SW_MOTION */
	fdActorScriptSetOrCompareArrivalAxis,		/* 0x1b SW_ARRIVAL */
	fdActorScriptSetOrCompareComparisonResult,	/* 0x1c SW_RESULT */
	fdActorScriptSetOrCompareSnapToTarget,		/* 0x1d SW_SNAP */
	fdActorScriptSetOrCompareCollisionFlags,	/* 0x1e SW_COLLISION */
	fdActorScriptSetOrCompareSteeringFlags,		/* 0x1f SW_STEERING */
	fdActorScriptSetOrComparePaused,			/* 0x20 SW_PAUSED */
	fdActorScriptSetOrCompareRepeatCount,		/* 0x21 SW_REPEAT */
	fdActorScriptSetOrCompareWait,				/* 0x22 SW_WAIT */
	fdActorScriptSetOrCompareTimer,				/* 0x23 SW_TIMER */
	fdActorScriptSetOrCompareUserValue,			/* 0x24 SW_USER_VALUE */
	fdActorScriptSetOrCompareWatchedActor,		/* 0x25 */
	fdActorScriptSetOrCompareUpdateAddress,		/* 0x26 SW_HOOK */
	fdActorScriptSetOrCompareSceneCounter,		/* 0x27 SW_SCENE_COUNTER */
	fdActorScriptSetOrCompareSceneByte,			/* 0x28 SW_SCENE_BYTE */
};
