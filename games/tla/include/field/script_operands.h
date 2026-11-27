#ifndef ALCHEMY_SCRIPT_OPERANDS_H
#define ALCHEMY_SCRIPT_OPERANDS_H

// The actor script interpreter and the operands its commands read.

#include "edition.h"
#include "types.h"
#include "static_assert.h"

struct ACTOR;

// An actor script command's handler: it reads its operands at the actor's script cursor, moves the
// cursor past them and returns 1 to run the next command this tick or 0 to wait.
typedef Sint32 (*SCRIPT_COMMAND)(struct ACTOR *);

// ACTOR as the actor script's SC_SET, SC_ADD and SC_CMP commands reach it, every field a plain
// number (ACTOR_SCRIPT_VALUE numbers them).
typedef struct SCRIPT_OPERANDS
{
	Uint32 script;
	Uint16 cursor;
	Uint16 facing;
	Uint32 x;
	Uint32 y;
	Uint32 z;
	Uint32 groundHeight;
	Uint32 scaleX;
	Uint32 scaleY;
	Uint16 radius;
	Uint8  unusedAfterRadius[2]; // no code names them
	Uint32 velocityX;
	Uint32 velocityY;
	Uint32 velocityZ;
	Uint32 speedLimit;
	Uint32 acceleration;
	Uint32 targetX;
	Uint32 targetY;
	Uint32 targetZ;
	Uint32 bounce;
	Uint32 gravity;
	Uint32 sceneWord;
	Uint32 sprite;
	Uint8  spriteStorageKind; // ACTOR.spriteStorageKind
	Uint8  motionFlags;
	Uint8  arrivalAxis;
	Uint8  conditionResult;   // the last test; every compare sets it
	Uint8  snapToTarget;
	Uint8  collisionFlags;
	Uint8  steeringFlags;
	Uint8  paused;
	Uint8  retainSpriteCache;
	Uint8  repeatCount; // the SC_LOOP count
	Sint16 wait;        // SC_WAIT's ticks left
	Uint8  refused;     // ACTOR.refused, the blocked ticks
	Uint8  frozen;
	Uint8  sceneCount;
	Uint8  sceneByte;
	Sint16 timer;
	Sint16 userValue;
	Uint32 watchedActor;
	Uint32 update;
} SCRIPT_OPERANDS;

STATIC_ASSERT(offsetof(SCRIPT_OPERANDS, script) == 0);
STATIC_ASSERT(offsetof(SCRIPT_OPERANDS, cursor) == 4);
STATIC_ASSERT(offsetof(SCRIPT_OPERANDS, facing) == 6);
STATIC_ASSERT(offsetof(SCRIPT_OPERANDS, spriteStorageKind) == 84);
STATIC_ASSERT(offsetof(SCRIPT_OPERANDS, repeatCount) == 93);
STATIC_ASSERT(offsetof(SCRIPT_OPERANDS, refused) == 96);
STATIC_ASSERT(offsetof(SCRIPT_OPERANDS, conditionResult) == 87);
STATIC_ASSERT(offsetof(SCRIPT_OPERANDS, wait) == 94);
STATIC_ASSERT(offsetof(SCRIPT_OPERANDS, timer) == 100);
STATIC_ASSERT(offsetof(SCRIPT_OPERANDS, watchedActor) == 104);
STATIC_ASSERT(offsetof(SCRIPT_OPERANDS, update) == 108);
STATIC_ASSERT(sizeof(SCRIPT_OPERANDS) == 112);

// A work value's handler: operation 0 sets the value, 1 adds to it and 2 compares it, into
// conditionResult.
typedef void (*OPERAND_FUNC)(SCRIPT_OPERANDS *operands, Sint32 operation, Sint32 value);

// The handler for each work value number (map_grid_data.c).
extern const OPERAND_FUNC ActorScriptOperandHandlerTable[41];

// The work value handlers of ActorScriptOperandHandlerTable, one an SW_ value: each sets, adds to
// or compares (into conditionResult) the actor field it names; positions, targets, velocities and
// gravity are 16.16 pixels.
void                      fdActorScriptSetOrCompareScriptAddress(SCRIPT_OPERANDS *operands,
                                                                 Sint32           operation,
                                                                 Sint32           value);
void fdActorScriptSetOrCompareCursor(SCRIPT_OPERANDS *operands, Sint32 operation, Sint32 value);
void fdActorScriptSetOrCompareFacing(SCRIPT_OPERANDS *operands, Sint32 operation, Sint32 value);
void fdActorScriptSetOrCompareX(SCRIPT_OPERANDS *operands, Sint32 operation, Sint32 value);
void fdActorScriptSetOrCompareY(SCRIPT_OPERANDS *operands, Sint32 operation, Sint32 value);
void fdActorScriptSetOrCompareZ(SCRIPT_OPERANDS *operands, Sint32 operation, Sint32 value);
void fdActorScriptSetOrCompareRadius(SCRIPT_OPERANDS *operands, Sint32 operation, Sint32 value);
void fdActorScriptSetOrCompareScaleX(SCRIPT_OPERANDS *operands, Sint32 operation, Sint32 value);
void fdActorScriptSetOrCompareScaleY(SCRIPT_OPERANDS *operands, Sint32 operation, Sint32 value);
void fdActorScriptSetOrCompareVelocityX(SCRIPT_OPERANDS *operands, Sint32 operation, Sint32 value);
void fdActorScriptSetOrCompareVelocityY(SCRIPT_OPERANDS *operands, Sint32 operation, Sint32 value);
void fdActorScriptSetOrCompareVelocityZ(SCRIPT_OPERANDS *operands, Sint32 operation, Sint32 value);
void fdActorScriptSetOrCompareSpeedLimit(SCRIPT_OPERANDS *operands, Sint32 operation, Sint32 value);
void fdActorScriptSetOrCompareAcceleration(SCRIPT_OPERANDS *operands,
                                           Sint32           operation,
                                           Sint32           value);
void fdActorScriptSetOrCompareTargetX(SCRIPT_OPERANDS *operands, Sint32 operation, Sint32 value);
void fdActorScriptSetOrCompareTargetY(SCRIPT_OPERANDS *operands, Sint32 operation, Sint32 value);
void fdActorScriptSetOrCompareTargetZ(SCRIPT_OPERANDS *operands, Sint32 operation, Sint32 value);
void fdActorScriptSetOrCompareBounce(SCRIPT_OPERANDS *operands, Sint32 operation, Sint32 value);
void fdActorScriptSetOrCompareGravity(SCRIPT_OPERANDS *operands, Sint32 operation, Sint32 value);
void fdActorScriptSetOrCompareTerrainHeight(SCRIPT_OPERANDS *operands,
                                            Sint32           operation,
                                            Sint32           value);
void fdActorScriptSetOrCompareSceneWord(SCRIPT_OPERANDS *operands, Sint32 operation, Sint32 value);
void fdActorScriptSetOrCompareSprite(SCRIPT_OPERANDS *operands, Sint32 operation, Sint32 value);
void fdActorScriptSetOrCompareSpriteStorageKind(SCRIPT_OPERANDS *operands,
                                                Sint32           operation,
                                                Sint32           value);
void fdActorScriptSetOrCompareMotionFlags(SCRIPT_OPERANDS *operands,
                                          Sint32           operation,
                                          Sint32           value);
void fdActorScriptSetOrCompareArrivalAxis(SCRIPT_OPERANDS *operands,
                                          Sint32           operation,
                                          Sint32           value);
void fdActorScriptSetOrCompareComparisonResult(SCRIPT_OPERANDS *operands,
                                               Sint32           operation,
                                               Sint32           value);
void fdActorScriptSetOrCompareSnapToTarget(SCRIPT_OPERANDS *operands,
                                           Sint32           operation,
                                           Sint32           value);
void fdActorScriptSetOrCompareCollisionFlags(SCRIPT_OPERANDS *operands,
                                             Sint32           operation,
                                             Sint32           value);
void fdActorScriptSetOrCompareSteeringFlags(SCRIPT_OPERANDS *operands,
                                            Sint32           operation,
                                            Sint32           value);
void fdActorScriptSetOrComparePaused(SCRIPT_OPERANDS *operands, Sint32 operation, Sint32 value);
void fdActorScriptSetOrCompareRepeatCount(SCRIPT_OPERANDS *operands,
                                          Sint32           operation,
                                          Sint32           value);
void fdActorScriptSetOrCompareWait(SCRIPT_OPERANDS *operands, Sint32 operation, Sint32 value);
void fdActorScriptSetOrCompareTimer(SCRIPT_OPERANDS *operands, Sint32 operation, Sint32 value);
void fdActorScriptSetOrCompareUserValue(SCRIPT_OPERANDS *operands, Sint32 operation, Sint32 value);
void fdActorScriptSetOrCompareWatchedActor(SCRIPT_OPERANDS *operands,
                                           Sint32           operation,
                                           Sint32           value);
void fdActorScriptSetOrCompareUpdateAddress(SCRIPT_OPERANDS *operands,
                                            Sint32           operation,
                                            Sint32           value);
void fdActorScriptSetOrCompareSceneCounter(SCRIPT_OPERANDS *operands,
                                           Sint32           operation,
                                           Sint32           value);
void fdActorScriptSetOrCompareSceneByte(SCRIPT_OPERANDS *operands, Sint32 operation, Sint32 value);

Sint32 fdActorScriptGoto(struct ACTOR *interpreter);
Sint32 fdActorScriptHideSprites(struct ACTOR *interpreter);
Sint32 fdActorScriptShowSprite(struct ACTOR *interpreter);

#endif
