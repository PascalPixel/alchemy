/* @file */
/* Map ground: the height function of each cell kind, the world map's battle */
/* environments and the sprite viewer's small tables after them; then the */
/* actor scripts the field starts by itself, the actor script command table */
/* and the work value handlers. Each map's resources follow in map_resources.s.
 */

#include "field/field.h"
#include "field/curve.h"
#include "field/script_operands.h"
#include "field/actor_script.h"
#include "field/map_grid.h"

/* -------------------------------------------------------------------------- */
/* Map terrain: the height function of each cell kind, the world map's */
/* battle environments, and the small tables after them. */
/* -------------------------------------------------------------------------- */

/* Each height function reads only the arguments it needs, so the type */
/* leaves them unspecified. */
typedef s32 (*TERRAIN_HEIGHT_FN)();

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

/* Ground height functions, by TERRAIN_KIND, called with (the remaining */
/* samples, x within the cell, y within the cell). Each reads only the */
/* arguments it needs. */
const TERRAIN_HEIGHT_FN MapGrid_TerrainHeightFunctions[TERRAIN_KIND_COUNT] = {
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

/* The world map's battle environments, read at a collision value plus a set */
/* of rows (fdWorldMapGetBattleEnvironment): the plain rows; the beach rows for
 */
/* a cell with its beach bit; the snowfield rows for the encounter regions from
 */
/* SNOWFIELD_REGION_FIRST; the desert rows for the regions round Air's Rock and
 */
/* north of Shaman Village; the blizzard rows for the region round Prox. */
/* clang-format off */
/* ENV(name) is BATTLE_ENVIRONMENT_name, to keep each row's sixteen short. */
#define ENV(name) BATTLE_ENVIRONMENT_##name
/* The World Map's battle environments, by collision value, sixteen to each of */
/* its sets of rows. */
const u8 WorldMap_TerrainBehaviorTable[80] = {
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
const u16 SpriteViewer_BackdropColors[8] = {
    RGB(0, 0, 0),    RGB(0, 0, 31),   RGB(0, 31, 0),
    RGB(31, 0, 0),   RGB(7, 7, 7),    RGB(15, 15, 15),
    RGB(31, 31, 31), RGB(15, 0, 0),
};

/* The sprite ids the sprite viewer's fast step (Start held with L or R) */
/* stops at, the first of each group of sprites: a skip forward goes to the */
/* next stop above the sprite shown, back to the last stop below it. (u16)-1 */
/* ends the list. */
const u16 SpriteViewer_SkipStops[10] = {
    SPRITE_ISAAC,              /* the party */
    SPRITE_BRIGGS,             /* the people */
    SPRITE_VENUS_DJINNI,       /* the Djinn */
    SPRITE_PUNCH_ANT,          /* monsters on the field */
    SPRITE_VINE_PILLAR,        /* things on the field */
    SPRITE_BATTLE_ISAAC,       /* the party in battle, and their weapons */
    SPRITE_BATTLE_BLUE_DRAGON, /* monsters in battle */
    SPRITE_THOR,               /* the summons */
    SPRITE_STAR_BURST,         /* effects */
    (u16)-1,
};
/* clang-format on */

/* Unused: nothing refers to it. Eight directions at a radius of 40 (x, y). */
const s8 WorldMap_DirectionOffsets[8][2] = {
	{0, -40}, {28, -28}, {40, 0},  {28, 28},
	{0, 40},  {-28, 28}, {-40, 0}, {-28, -28},
};

/* Unused: nothing refers to it. A scale of 1 on both axes (16.16). */
const u32 WorldMap_UnitScale[2] = {
	FIXED_ONE,
	FIXED_ONE,
};

/* The sprite viewer's scale for its ring of copies: full size. */
const SPR_SCALE SpriteViewer_Scale = {{{FIXED_ONE, FIXED_ONE}}};

/* -------------------------------------------------------------------------- */
/* Actor scripts: the ones the field starts by itself, the command table and */
/* the work value handlers SC_SET, SC_ADD and SC_CMP call. */
/* -------------------------------------------------------------------------- */

s32 fdActorScriptWait();
s32 fdActorScriptWaitForArrival();
s32 fdActorScriptSetPosAndResetMotion();
s32 fdActorScriptResetToOrigin();
s32 fdActorScriptApplyAbsolutePos();
s32 fdActorScriptApplyRelativePos();
s32 fdActorScriptStoreAngToWatchedActor();
s32 fdActorScriptApplyWatchedActorPos();
s32 fdActorScriptApplyLocalOffsetPos();
s32 fdActorWander();
s32 fdActorScriptWanderNearHome();
s32 fdActorApproachWatchedActor();
s32 fdCameraFollowLeaderInBounds();
s32 fdActorScriptRepeatOrJump();
s32 fdActorScriptJumpToLabel();
u32 fdActorScriptJumpIfTrue();
u32 fdActorScriptJumpIfFalse();
s32 fdActorScriptEnd();
s32 fdActorScriptTestFlag();
s32 fdActorScriptSetFlagAndTest();
s32 fdActorScriptClearFlagAndTest();
s32 fdActorScriptToggleFlagAndTest();
s32 fdActorScriptApplyOperandSet();
s32 fdActorScriptApplyOperandAdd();
s32 fdActorScriptApplyOperandCompare();
s32 fdActorUpdatePlayerCtrl();
s32 fdActorMoveOnWorldMap();
s32 fdActorMoveByKeys();
s32 fdActorCrossRope();
s32 fdActorCrossTightrope();
s32 fdActorMoveAsSand();
s32 fdActorSailOnMap();
s32 fdActorSailOnWorldMap();
s32 fdActorFlyOnWorldMap();
s32 fdActorMoveAsSandOnWorldMap();
s32 fdActorUpdateHoverCtrl();
s32 fdActorUpdateWalkingCtrl();
s32 fdCheckConfiguredKeysAndCount();
s32 fdActorScriptRelease();
s32 fdActorScriptSetAnimation();
s32 fdActorScriptNop();
s32 fdActorScriptPlaySnd();
s32 fdActorScriptPlaySndInView();
s32 fdActorScriptInvokeCallback();
s32 fdCameraFollowWatchedInBounds();
s32 fdActorScriptSetMoveTargetXZ();

/* Each runs one control command every frame: the command, SC_WAIT(1), */
/* SC_LOOP(SC_FOREVER, SC_TOP). */
const s32 Actor_KeyCheckScript[] = {
	SC_KEY_COUNT,
	SC_WAIT(1),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

/* Climbing by the keys (fdActorMoveByKeys). */
const s32 Actor_KeysScript[] = {
	SC_KEYS,
	SC_WAIT(1),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

/* Crossing a tied rope (fdActorCrossRope). */
const s32 Actor_RopeScript[] = {
	SC_ROPE,
	SC_WAIT(1),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

/* Crossing a tightrope (fdActorCrossTightrope). */
const s32 Actor_TightropeScript[] = {
	SC_TIGHTROPE,
	SC_WAIT(1),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

/* Moving as sand (fdActorMoveAsSand). */
const s32 Actor_SandScript[] = {
	SC_SAND,
	SC_WAIT(1),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

/* Hovering (fdActorUpdateHoverCtrl). */
const s32 Actor_HoverScript[] = {
	SC_HOVER,
	SC_WAIT(1),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

/* Walking (fdActorUpdateWalkingCtrl). */
const s32 Actor_WalkingScript[] = {
	SC_WALK,
	SC_WAIT(1),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

/* The leader on foot (fdActorUpdatePlayerCtrl). */
const s32 Actor_PlayerScript[] = {
	SC_PLAYER,
	SC_WAIT(1),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

/* The leader on the World Map (fdActorMoveOnWorldMap). */
const s32 Actor_WorldMapScript[] = {
	SC_WORLD,
	SC_WAIT(1),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

/* Sailing on a map (fdActorSailOnMap). */
const s32 Actor_SailOnMapScript[] = {
	SC_SAIL,
	SC_WAIT(1),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

/* Sailing on the World Map (fdActorSailOnWorldMap). */
const s32 Actor_SailScript[] = {
	SC_WORLD_SAIL,
	SC_WAIT(1),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

/* Flying on the World Map (fdActorFlyOnWorldMap). */
const s32 Actor_FlyScript[] = {
	SC_WORLD_FLY,
	SC_WAIT(1),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

/* Moving as sand on the World Map (fdActorMoveAsSandOnWorldMap). */
const s32 Actor_SandOnWorldMapScript[] = {
	SC_WORLD_SAND,
	SC_WAIT(1),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

/* The camera following the leader within the map's bounds. */
const s32 Actor_CameraScript[] = {
	SC_CAMERA,
	SC_WAIT(1),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

/* Following the watched actor (fdActorApproachWatchedActor). */
const s32 Actor_ApproachWatchedScript[] = {
	SC_APPROACH_WATCHED,
	SC_WAIT(1),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

/* A script that ends at once. */
const s32 ActorScript_EndScript[] = {
	SC_END,
};

/* The handler of each command. 0x31 to 0x35 repeat SC_END's. */
const SCRIPT_COMMAND_FN ActorScript_CommandTable[54] = {
	fdActorScriptWait,							 /* 0x00 SC_WAIT */
	fdActorScriptWaitForArrival,				 /* 0x01 SC_SYNC */
	fdActorScriptSetPosAndResetMotion,			 /* 0x02 SC_POSITION */
	fdActorScriptResetToOrigin,					 /* 0x03 SC_POSITION_ORIGIN */
	fdActorScriptApplyAbsolutePos,				 /* 0x04 SC_MOVE */
	fdActorScriptApplyRelativePos,				 /* 0x05 SC_MOVE_BY */
	fdActorScriptStoreAngToWatchedActor,		 /* 0x06 SC_FACE_WATCHED */
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
	fdActorUpdatePlayerCtrl,					 /* 0x19 SC_PLAYER */
	fdActorMoveOnWorldMap,						 /* 0x1a SC_WORLD */
	fdActorMoveByKeys,							 /* 0x1b SC_KEYS */
	fdActorCrossRope,							 /* 0x1c SC_ROPE */
	fdActorCrossTightrope,						 /* 0x1d SC_TIGHTROPE */
	fdActorMoveAsSand,							 /* 0x1e SC_SAND */
	fdActorSailOnMap,							 /* 0x1f SC_SAIL */
	fdActorSailOnWorldMap,						 /* 0x20 SC_WORLD_SAIL */
	fdActorFlyOnWorldMap,						 /* 0x21 SC_WORLD_FLY */
	fdActorMoveAsSandOnWorldMap,				 /* 0x22 SC_WORLD_SAND */
	fdActorUpdateHoverCtrl,						 /* 0x23 SC_HOVER */
	fdActorUpdateWalkingCtrl,					 /* 0x24 SC_WALK */
	fdCheckConfiguredKeysAndCount,				 /* 0x25 SC_KEY_COUNT */
	fdActorScriptRelease,						 /* 0x26 SC_RELEASE */
	fdActorScriptSetAnimation,					 /* 0x27 SC_ANIMATION */
	fdActorScriptNop,							 /* 0x28 SC_NOP */
	fdActorScriptPlaySnd,						 /* 0x29 SC_SE */
	fdActorScriptPlaySndInView,					 /* 0x2a SC_SE_IN_VIEW */
	fdActorScriptGoto,							 /* 0x2b SC_GOTO */
	fdActorScriptHideSprs,						 /* 0x2c SC_HIDE_SPRITES */
	fdActorScriptShowSpr,						 /* 0x2d SC_SHOW_SPRITE */
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
const OPERAND_FUNC ActorScript_OperandHandlerTable[41] = {
	fdActorScriptSetOrCompareScriptAddress,	   /* 0x00 SW_SCRIPT */
	fdActorScriptSetOrCompareCursor,		   /* 0x01 SW_CURSOR */
	fdActorScriptSetOrCompareFacing,		   /* 0x02 */
	fdActorScriptSetOrCompareFacing,		   /* 0x03 SW_FACING */
	fdActorScriptSetOrCompareFacing,		   /* 0x04 */
	fdActorScriptSetOrCompareX,				   /* 0x05 SW_X */
	fdActorScriptSetOrCompareY,				   /* 0x06 SW_Y */
	fdActorScriptSetOrCompareZ,				   /* 0x07 SW_Z */
	fdActorScriptSetOrCompareRadius,		   /* 0x08 SW_RADIUS */
	fdActorScriptSetOrCompareScaleX,		   /* 0x09 SW_SCALE_X */
	fdActorScriptSetOrCompareScaleY,		   /* 0x0a SW_SCALE_Y */
	fdActorScriptSetOrCompareScaleY,		   /* 0x0b */
	fdActorScriptSetOrCompareVelocityX,		   /* 0x0c SW_VELOCITY_X */
	fdActorScriptSetOrCompareVelocityY,		   /* 0x0d SW_VELOCITY_Y */
	fdActorScriptSetOrCompareVelocityZ,		   /* 0x0e SW_VELOCITY_Z */
	fdActorScriptSetOrCompareSpeedLimit,	   /* 0x0f SW_SPEED */
	fdActorScriptSetOrCompareAcceleration,	   /* 0x10 SW_ACCELERATION */
	fdActorScriptSetOrCompareTargetX,		   /* 0x11 SW_TARGET_X */
	fdActorScriptSetOrCompareTargetY,		   /* 0x12 SW_TARGET_Y */
	fdActorScriptSetOrCompareTargetZ,		   /* 0x13 SW_TARGET_Z */
	fdActorScriptSetOrCompareBounce,		   /* 0x14 SW_BOUNCE */
	fdActorScriptSetOrCompareGravity,		   /* 0x15 SW_GRAVITY */
	fdActorScriptSetOrCompareTerrainHeight,	   /* 0x16 SW_GROUND */
	fdActorScriptSetOrCompareSceneWord,		   /* 0x17 */
	fdActorScriptSetOrCompareSpr,			   /* 0x18 */
	fdActorScriptSetOrCompareSprStorageKind,   /* 0x19 */
	fdActorScriptSetOrCompareMotionFlags,	   /* 0x1a SW_MOTION */
	fdActorScriptSetOrCompareArrivalAxis,	   /* 0x1b SW_ARRIVAL */
	fdActorScriptSetOrCompareComparisonResult, /* 0x1c SW_RESULT */
	fdActorScriptSetOrCompareSnapToTarget,	   /* 0x1d SW_SNAP */
	fdActorScriptSetOrCompareCollisionFlags,   /* 0x1e SW_COLLISION */
	fdActorScriptSetOrCompareSteeringFlags,	   /* 0x1f SW_STEERING */
	fdActorScriptSetOrComparePaused,		   /* 0x20 SW_PAUSED */
	fdActorScriptSetOrCompareRepeatCount,	   /* 0x21 SW_REPEAT */
	fdActorScriptSetOrCompareWait,			   /* 0x22 SW_WAIT */
	fdActorScriptSetOrCompareTimer,			   /* 0x23 SW_TIMER */
	fdActorScriptSetOrCompareUserValue,		   /* 0x24 SW_USER_VALUE */
	fdActorScriptSetOrCompareWatchedActor,	   /* 0x25 */
	fdActorScriptSetOrCompareUpdateAddress,	   /* 0x26 SW_HOOK */
	fdActorScriptSetOrCompareSceneCounter,	   /* 0x27 SW_SCENE_COUNTER */
	fdActorScriptSetOrCompareSceneByte,		   /* 0x28 SW_SCENE_BYTE */
};
