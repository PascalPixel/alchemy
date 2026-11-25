#ifndef ALCHEMY_SCRIPT_OPERANDS_H
#define ALCHEMY_SCRIPT_OPERANDS_H

/// @file
/// The actor script interpreter and the operands its commands read.

#include "edition.h"
#include "static_assert.h"
#include "types.h"

struct Actor;

/// An actor script command's handler: it reads its operands at the actor's
/// script cursor, moves the cursor past them and returns 1 to run the next
/// command this frame or 0 to wait.
typedef s32 (*ScriptCommand)(struct Actor *);

/// The whole 112-byte slot as SC_SET, SC_ADD and SC_CMP see it: each field is
/// one state value (enum ActorScriptValue in actor_script.h), reached through
/// gActorScript_OperandHandlerTable, and named for what the field keeps there.
struct ScriptOperands {
  u32 scriptAddress; ///< SW_SCRIPT; an add steps it by whole words
  u16 cursor;        ///< SW_CURSOR, compared as signed
  u16 facing;        ///< SW_FACING, and the state values either side
  u32 x;             ///< SW_X, 16.16 fixed-point pixels
  u32 y;             ///< SW_Y
  u32 z;             ///< SW_Z
  u32 groundHeight;  ///< SW_GROUND
  u32 scaleX;        ///< SW_SCALE_X, 16.16 fixed point
  u32 scaleY;        ///< SW_SCALE_Y, and the state value after it
  u16 radius;        ///< SW_RADIUS, in pixels; compared as signed
  u8 layer;          ///< no state value
  u8 priorityFlags;  ///< no state value
  u32 velocityX;     ///< SW_VELOCITY_X, 16.16 fixed-point pixels per frame
  u32 velocityY;     ///< SW_VELOCITY_Y
  u32 velocityZ;     ///< SW_VELOCITY_Z
  u32 speedLimit;    ///< SW_SPEED
  u32 acceleration;  ///< SW_ACCELERATION
  u32 targetX;       ///< SW_TARGET_X; ACTOR_NO_TARGET when unset
  u32 targetY;       ///< SW_TARGET_Y
  u32 targetZ;       ///< SW_TARGET_Z
  u32 bounce;        ///< SW_BOUNCE, or a floating actor's phase
  u32 gravity;       ///< SW_GRAVITY
  /// SW_SCENE_WORD: the word Actor_Create zeroes and only scene code
  /// reads.
  u32 sceneWord;
  u32 sprite;           ///< SW_SPRITE: the sprite, or its list of four
  u8 spriteStorageKind; ///< SW_SPRITE_KIND, an enum ActorSpriteKind
  u8 motionFlags;       ///< SW_MOTION, ActorMotionFlag bits
  u8 arrivalAxis;       ///< SW_ARRIVAL
  u8 conditionResult;   ///< SW_RESULT, the last test; every compare sets it
  u8 snapToTarget;      ///< SW_SNAP
  u8 collisionFlags;    ///< SW_COLLISION
  u8 steeringFlags;     ///< SW_STEERING
  u8 paused;            ///< SW_PAUSED
  u8 retainSpriteCache; ///< keeps the sprite's VRAM; no state value
  u8 repeatCount;       ///< SW_REPEAT, SC_LOOP's count
  s16 wait;             ///< SW_WAIT
  u8 refused;           ///< Actor.refused; no state value
  u8 frozen;            ///< ActorArmView.frozen; no state value
  u8 sceneCounter;      ///< SW_SCENE_COUNTER, kept by scene code
  u8 sceneByte;         ///< SW_SCENE_BYTE, kept by scene code
  s16 timer;            ///< SW_TIMER, what its actor script reads
  s16 userValue;        ///< SW_USER_VALUE, or scene scratch
  u32 watchedActor;     ///< SW_WATCHED_ACTOR, the actor it follows or faces
  u32 update;           ///< SW_HOOK, the routine run each frame
};

STATIC_ASSERT(offsetof(struct ScriptOperands, scriptAddress) == 0);
STATIC_ASSERT(offsetof(struct ScriptOperands, cursor) == 4);
STATIC_ASSERT(offsetof(struct ScriptOperands, facing) == 6);
STATIC_ASSERT(offsetof(struct ScriptOperands, radius) == 32);
STATIC_ASSERT(offsetof(struct ScriptOperands, sceneWord) == 76);
STATIC_ASSERT(offsetof(struct ScriptOperands, spriteStorageKind) == 84);
STATIC_ASSERT(offsetof(struct ScriptOperands, repeatCount) == 93);
STATIC_ASSERT(offsetof(struct ScriptOperands, refused) == 96);
STATIC_ASSERT(offsetof(struct ScriptOperands, conditionResult) == 87);
STATIC_ASSERT(offsetof(struct ScriptOperands, wait) == 94);
STATIC_ASSERT(offsetof(struct ScriptOperands, timer) == 100);
STATIC_ASSERT(offsetof(struct ScriptOperands, watchedActor) == 104);
STATIC_ASSERT(offsetof(struct ScriptOperands, update) == 108);
STATIC_ASSERT(sizeof(struct ScriptOperands) == 112);

#endif
