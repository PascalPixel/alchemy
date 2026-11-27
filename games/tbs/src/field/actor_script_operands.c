/* @file */
/* Script state values: the handler SC_SET, SC_ADD and SC_CMP call for each. */

#include "field/actor_script_operands.h"
#include "field/field.h"

/* The handler for each state value number (ACTOR_SCRIPT_VALUE). */
const OPERAND_FUNC ActorScript_OperandHandlerTable[41] = {
	fdActorScriptSetOrCompareScriptAddress,	   /* 0 SW_SCRIPT */
	fdActorScriptSetOrCompareCursor,		   /* 1 SW_CURSOR */
	fdActorScriptSetOrCompareFacing,		   /* 2, as SW_FACING */
	fdActorScriptSetOrCompareFacing,		   /* 3 SW_FACING */
	fdActorScriptSetOrCompareFacing,		   /* 4, as SW_FACING */
	fdActorScriptSetOrCompareX,				   /* 5 SW_X */
	fdActorScriptSetOrCompareY,				   /* 6 SW_Y */
	fdActorScriptSetOrCompareZ,				   /* 7 SW_Z */
	fdActorScriptSetOrCompareRadius,		   /* 8 SW_RADIUS */
	fdActorScriptSetOrCompareScaleX,		   /* 9 SW_SCALE_X */
	fdActorScriptSetOrCompareScaleY,		   /* 10 SW_SCALE_Y */
	fdActorScriptSetOrCompareScaleY,		   /* 11, as SW_SCALE_Y */
	fdActorScriptSetOrCompareVelocityX,		   /* 12 SW_VELOCITY_X */
	fdActorScriptSetOrCompareVelocityY,		   /* 13 SW_VELOCITY_Y */
	fdActorScriptSetOrCompareVelocityZ,		   /* 14 SW_VELOCITY_Z */
	fdActorScriptSetOrCompareSpeedLimit,	   /* 15 SW_SPEED */
	fdActorScriptSetOrCompareAcceleration,	   /* 16 SW_ACCELERATION */
	fdActorScriptSetOrCompareTargetX,		   /* 17 SW_TARGET_X */
	fdActorScriptSetOrCompareTargetY,		   /* 18 SW_TARGET_Y */
	fdActorScriptSetOrCompareTargetZ,		   /* 19 SW_TARGET_Z */
	fdActorScriptSetOrCompareBounce,		   /* 20 SW_BOUNCE */
	fdActorScriptSetOrCompareGravity,		   /* 21 SW_GRAVITY */
	fdActorScriptSetOrCompareTerrainHeight,	   /* 22 SW_GROUND */
	fdActorScriptSetOrCompareSceneWord,		   /* 23 SW_SCENE_WORD */
	fdActorScriptSetOrCompareSpr,			   /* 24 SW_SPRITE */
	fdActorScriptSetOrCompareSprStorageKind,   /* 25 SW_SPRITE_KIND */
	fdActorScriptSetOrCompareMotionFlags,	   /* 26 SW_MOTION */
	fdActorScriptSetOrCompareArrivalAxis,	   /* 27 SW_ARRIVAL */
	fdActorScriptSetOrCompareComparisonResult, /* 28 SW_RESULT */
	fdActorScriptSetOrCompareSnapToTarget,	   /* 29 SW_SNAP */
	fdActorScriptSetOrCompareCollisionFlags,   /* 30 SW_COLLISION */
	fdActorScriptSetOrCompareSteeringFlags,	   /* 31 SW_STEERING */
	fdActorScriptSetOrComparePaused,		   /* 32 SW_PAUSED */
	fdActorScriptSetOrCompareRepeatCount,	   /* 33 SW_REPEAT */
	fdActorScriptSetOrCompareWait,			   /* 34 SW_WAIT */
	fdActorScriptSetOrCompareTimer,			   /* 35 SW_TIMER */
	fdActorScriptSetOrCompareUserValue,		   /* 36 SW_USER_VALUE */
	fdActorScriptSetOrCompareWatchedActor,	   /* 37 SW_WATCHED_ACTOR */
	fdActorScriptSetOrCompareUpdateAddress,	   /* 38 SW_HOOK */
	fdActorScriptSetOrCompareSceneCounter,	   /* 39 SW_SCENE_COUNTER */
	fdActorScriptSetOrCompareSceneByte,		   /* 40 SW_SCENE_BYTE */
};
