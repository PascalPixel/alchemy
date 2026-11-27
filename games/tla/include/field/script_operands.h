#ifndef ALCHEMY_SCRIPT_OPERANDS_H
#define ALCHEMY_SCRIPT_OPERANDS_H

/* @file */
/* The actor script interpreter and the operands its commands read. */

#include "edition.h"
#include "types.h"
#include "static_assert.h"

struct ACTOR;

/* An actor script command's handler: it reads its operands at the actor's */
/* script cursor, moves the cursor past them and returns 1 to run the next */
/* command this frame or 0 to wait. */
typedef s32 (*SCRIPT_COMMAND)(struct ACTOR *);

/* ACTOR as the actor script's SC_SET, SC_ADD and SC_CMP commands reach
 */
/* it, every field a plain number (ACTOR_SCRIPT_VALUE numbers them). */
typedef struct SCRIPT_OPERANDS {
	u32 script;
	u16 cursor;
	u16 facing;
	u32 x;
	u32 y;
	u32 z;
	u32 groundHeight;
	u32 scaleX;
	u32 scaleY;
	u16 radius;
	u8 unusedAfterRadius[2]; /* no code names them */
	u32 velX;
	u32 velY;
	u32 velZ;
	u32 speedLimit;
	u32 acceleration;
	u32 tgtX;
	u32 tgtY;
	u32 tgtZ;
	u32 bounce;
	u32 gravity;
	u32 sceneWord;
	u32 spr;
	u8 sprStorageKind; /* ACTOR.sprStorageKind */
	u8 motionFlg;
	u8 arrivalAxis;
	u8 conditionResult; /* the last test; every compare sets it */
	u8 snapToTgt;
	u8 collisionFlg;
	u8 steeringFlg;
	u8 paused;
	u8 retainSprCache;
	u8 repeatCnt; /* the SC_LOOP count */
	s16 wait;	  /* SC_WAIT's frames left */
	u8 refused;	  /* ACTOR.refused, the blocked frames */
	u8 frozen;
	u8 sceneCnt;
	u8 sceneByte;
	s16 tmr;
	s16 userValue;
	u32 watchedActor;
	u32 update;
} SCRIPT_OPERANDS;

STATIC_ASSERT(offsetof(SCRIPT_OPERANDS, script) == 0);
STATIC_ASSERT(offsetof(SCRIPT_OPERANDS, cursor) == 4);
STATIC_ASSERT(offsetof(SCRIPT_OPERANDS, facing) == 6);
STATIC_ASSERT(offsetof(SCRIPT_OPERANDS, sprStorageKind) == 84);
STATIC_ASSERT(offsetof(SCRIPT_OPERANDS, repeatCnt) == 93);
STATIC_ASSERT(offsetof(SCRIPT_OPERANDS, refused) == 96);
STATIC_ASSERT(offsetof(SCRIPT_OPERANDS, conditionResult) == 87);
STATIC_ASSERT(offsetof(SCRIPT_OPERANDS, wait) == 94);
STATIC_ASSERT(offsetof(SCRIPT_OPERANDS, tmr) == 100);
STATIC_ASSERT(offsetof(SCRIPT_OPERANDS, watchedActor) == 104);
STATIC_ASSERT(offsetof(SCRIPT_OPERANDS, update) == 108);
STATIC_ASSERT(sizeof(SCRIPT_OPERANDS) == 112);

/* A work value's handler: operation 0 sets the value, 1 adds to it and 2 */
/* compares it, into conditionResult. */
typedef void (*OPERAND_FUNC)(SCRIPT_OPERANDS *operands, s32 operation,
							 s32 value);

/* The handler for each work value number (map_grid_data.c). */
extern const OPERAND_FUNC ActorScript_OperandHandlerTable[41];

/* The work value handlers of ActorScript_OperandHandlerTable, one an SW_ */
/* value: each sets, adds to or compares (into conditionResult) the actor field
 */
/* it names; positions, targets, velocities and gravity are 16.16 pixels. */
void fdActorScriptSetOrCompareScriptAddress(SCRIPT_OPERANDS *operands,
											s32 operation, s32 value);
void fdActorScriptSetOrCompareCursor(SCRIPT_OPERANDS *operands, s32 operation,
									 s32 value);
void fdActorScriptSetOrCompareFacing(SCRIPT_OPERANDS *operands, s32 operation,
									 s32 value);
void fdActorScriptSetOrCompareX(SCRIPT_OPERANDS *operands, s32 operation,
								s32 value);
void fdActorScriptSetOrCompareY(SCRIPT_OPERANDS *operands, s32 operation,
								s32 value);
void fdActorScriptSetOrCompareZ(SCRIPT_OPERANDS *operands, s32 operation,
								s32 value);
void fdActorScriptSetOrCompareRadius(SCRIPT_OPERANDS *operands, s32 operation,
									 s32 value);
void fdActorScriptSetOrCompareScaleX(SCRIPT_OPERANDS *operands, s32 operation,
									 s32 value);
void fdActorScriptSetOrCompareScaleY(SCRIPT_OPERANDS *operands, s32 operation,
									 s32 value);
void fdActorScriptSetOrCompareVelocityX(SCRIPT_OPERANDS *operands,
										s32 operation, s32 value);
void fdActorScriptSetOrCompareVelocityY(SCRIPT_OPERANDS *operands,
										s32 operation, s32 value);
void fdActorScriptSetOrCompareVelocityZ(SCRIPT_OPERANDS *operands,
										s32 operation, s32 value);
void fdActorScriptSetOrCompareSpeedLimit(SCRIPT_OPERANDS *operands,
										 s32 operation, s32 value);
void fdActorScriptSetOrCompareAcceleration(SCRIPT_OPERANDS *operands,
										   s32 operation, s32 value);
void fdActorScriptSetOrCompareTargetX(SCRIPT_OPERANDS *operands, s32 operation,
									  s32 value);
void fdActorScriptSetOrCompareTargetY(SCRIPT_OPERANDS *operands, s32 operation,
									  s32 value);
void fdActorScriptSetOrCompareTargetZ(SCRIPT_OPERANDS *operands, s32 operation,
									  s32 value);
void fdActorScriptSetOrCompareBounce(SCRIPT_OPERANDS *operands, s32 operation,
									 s32 value);
void fdActorScriptSetOrCompareGravity(SCRIPT_OPERANDS *operands, s32 operation,
									  s32 value);
void fdActorScriptSetOrCompareTerrainHeight(SCRIPT_OPERANDS *operands,
											s32 operation, s32 value);
void fdActorScriptSetOrCompareSceneWord(SCRIPT_OPERANDS *operands,
										s32 operation, s32 value);
void fdActorScriptSetOrCompareSpr(SCRIPT_OPERANDS *operands, s32 operation,
								  s32 value);
void fdActorScriptSetOrCompareSprStorageKind(SCRIPT_OPERANDS *operands,
											 s32 operation, s32 value);
void fdActorScriptSetOrCompareMotionFlags(SCRIPT_OPERANDS *operands,
										  s32 operation, s32 value);
void fdActorScriptSetOrCompareArrivalAxis(SCRIPT_OPERANDS *operands,
										  s32 operation, s32 value);
void fdActorScriptSetOrCompareComparisonResult(SCRIPT_OPERANDS *operands,
											   s32 operation, s32 value);
void fdActorScriptSetOrCompareSnapToTarget(SCRIPT_OPERANDS *operands,
										   s32 operation, s32 value);
void fdActorScriptSetOrCompareCollisionFlags(SCRIPT_OPERANDS *operands,
											 s32 operation, s32 value);
void fdActorScriptSetOrCompareSteeringFlags(SCRIPT_OPERANDS *operands,
											s32 operation, s32 value);
void fdActorScriptSetOrComparePaused(SCRIPT_OPERANDS *operands, s32 operation,
									 s32 value);
void fdActorScriptSetOrCompareRepeatCount(SCRIPT_OPERANDS *operands,
										  s32 operation, s32 value);
void fdActorScriptSetOrCompareWait(SCRIPT_OPERANDS *operands, s32 operation,
								   s32 value);
void fdActorScriptSetOrCompareTimer(SCRIPT_OPERANDS *operands, s32 operation,
									s32 value);
void fdActorScriptSetOrCompareUserValue(SCRIPT_OPERANDS *operands,
										s32 operation, s32 value);
void fdActorScriptSetOrCompareWatchedActor(SCRIPT_OPERANDS *operands,
										   s32 operation, s32 value);
void fdActorScriptSetOrCompareUpdateAddress(SCRIPT_OPERANDS *operands,
											s32 operation, s32 value);
void fdActorScriptSetOrCompareSceneCounter(SCRIPT_OPERANDS *operands,
										   s32 operation, s32 value);
void fdActorScriptSetOrCompareSceneByte(SCRIPT_OPERANDS *operands,
										s32 operation, s32 value);

s32 fdActorScriptGoto(struct ACTOR *interpreter);
s32 fdActorScriptHideSprs(struct ACTOR *interpreter);
s32 fdActorScriptShowSpr(struct ACTOR *interpreter);

#endif
