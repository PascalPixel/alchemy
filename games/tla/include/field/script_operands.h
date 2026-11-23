#ifndef ALCHEMY_SCRIPT_OPERANDS_H
#define ALCHEMY_SCRIPT_OPERANDS_H

/// @file
/// The actor script interpreter and the operands its commands read.

#include "edition.h"
#include "types.h"
#include "static_assert.h"

struct Actor;

/// An actor script command's handler: it reads its operands at the actor's
/// script cursor, moves the cursor past them and returns 1 to run the next
/// command this frame or 0 to wait.
typedef s32 (*ScriptCommand)(struct Actor *);

/// struct Actor as the actor script's SC_SET, SC_ADD and SC_CMP commands reach
/// it, every field a plain number (enum ActorScriptValue numbers them).
struct ScriptOperands {
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
  u8 unusedAfterRadius[2]; ///< no code names them
  u32 velocityX;
  u32 velocityY;
  u32 velocityZ;
  u32 speedLimit;
  u32 acceleration;
  u32 targetX;
  u32 targetY;
  u32 targetZ;
  u32 bounce;
  u32 gravity;
  u32 riseBaseY;
  u32 sprite;
  u8 spriteStorageKind; ///< Actor.spriteStorageKind
  u8 motionFlags;
  u8 arrivalAxis;
  u8 conditionResult; ///< the last test; every compare sets it
  u8 snapToTarget;
  u8 collisionFlags;
  u8 steeringFlags;
  u8 paused;
  u8 retainSpriteCache;
  u8 repeatCount; ///< the SC_LOOP count
  u16 wait;       ///< SC_WAIT's frames left, compared as signed
  u8 refused;     ///< Actor.refused, the blocked frames
  u8 frozen;
  u8 riseCounter;
  u8 riseEnabled;
  s16 timer;
  s16 userValue;
  u32 watchedActor;
  u32 update;
};

STATIC_ASSERT(offsetof(struct ScriptOperands, script) == 0);
STATIC_ASSERT(offsetof(struct ScriptOperands, cursor) == 4);
STATIC_ASSERT(offsetof(struct ScriptOperands, facing) == 6);
STATIC_ASSERT(offsetof(struct ScriptOperands, spriteStorageKind) == 84);
STATIC_ASSERT(offsetof(struct ScriptOperands, repeatCount) == 93);
STATIC_ASSERT(offsetof(struct ScriptOperands, refused) == 96);
STATIC_ASSERT(offsetof(struct ScriptOperands, conditionResult) == 87);
STATIC_ASSERT(offsetof(struct ScriptOperands, wait) == 94);
STATIC_ASSERT(offsetof(struct ScriptOperands, timer) == 100);
STATIC_ASSERT(offsetof(struct ScriptOperands, watchedActor) == 104);
STATIC_ASSERT(offsetof(struct ScriptOperands, update) == 108);
STATIC_ASSERT(sizeof(struct ScriptOperands) == 112);

/// A work value's handler: operation 0 sets the value, 1 adds to it and 2
/// compares it, into conditionResult.
typedef void (*OperandFunc)(struct ScriptOperands *operands, s32 operation,
                            s32 value);

/// The handler for each work value number (map_grid_data.c).
extern const OperandFunc gActorScript_OperandHandlerTable[41];

s32 ActorScript_Goto(struct Actor *interpreter);
s32 ActorScript_HideSprites(struct Actor *interpreter);
s32 ActorScript_ShowSprite(struct Actor *interpreter);

#endif
