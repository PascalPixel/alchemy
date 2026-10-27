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

/// The whole 0x70-byte slot as SC_SET, SC_ADD and SC_CMP see it: each field
/// is one work value (enum ActorScriptValue in actor_script.h), reached through
/// gActorScript_OperandHandlerTable, and named for what the engine keeps
/// there. The types are the widths the handlers read, not the engine's own.
struct ScriptOperands {
  u32 scriptAddress; ///< SW_SCRIPT; an add steps it by whole words
  u16 cursor;        ///< SW_CURSOR, compared as signed
  u16 facing;        ///< SW_FACING, and work values 2 and 4
  u32 x;             ///< SW_X, 16.16 fixed-point pixels
  u32 y;             ///< SW_Y
  u32 z;             ///< SW_Z
  u32 groundHeight;  ///< SW_GROUND
  u32 scaleX;        ///< SW_SCALE_X, 16.16 fixed point
  u32 scaleY;        ///< SW_SCALE_Y, and work value 0x0b
  u16 radius;        ///< SW_RADIUS, in pixels; compared as signed
  u8 padding22[2];   ///< the layer and priority flags; no work value
  u32 velocityX;     ///< SW_VELOCITY_X, 16.16 fixed-point pixels per frame
  u32 velocityY;     ///< SW_VELOCITY_Y
  u32 velocityZ;     ///< SW_VELOCITY_Z
  u32 speedLimit;    ///< SW_SPEED
  u32 acceleration;  ///< SW_ACCELERATION
  u32 targetX;       ///< SW_TARGET_X; 0x80000000 when unset
  u32 targetY;       ///< SW_TARGET_Y
  u32 targetZ;       ///< SW_TARGET_Z
  u32 bounce;        ///< SW_BOUNCE, or a floating actor's phase
  u32 gravity;       ///< SW_GRAVITY
  /// Work value 0x17: the word Actor_Create zeroes and only scene code
  /// reads.
  u32 riseBaseY;
  u32 sprite;           ///< work value 0x18: the sprite, or its list of four
  u8 spriteStorageKind; ///< work value 0x19: 0 hidden, 1 one, 2 a group
  u8 motionFlags;       ///< SW_MOTION, ActorMotionFlag bits
  u8 arrivalAxis;       ///< SW_ARRIVAL
  u8 conditionResult;   ///< SW_RESULT, the last test; every compare sets it
  u8 snapToTarget;      ///< SW_SNAP
  u8 collisionFlags;    ///< SW_COLLISION
  u8 steeringFlags;     ///< SW_STEERING
  u8 paused;            ///< SW_PAUSED
  u8 retainSpriteCache; ///< keeps the sprite's VRAM; no work value
  u8 repeatCount;       ///< SW_REPEAT, SC_LOOP's count
  u16 wait;             ///< SW_WAIT, compared as signed
  u8 refused;           ///< Actor.refused; no work value
  u8 frozen;            ///< ActorArmView.frozen; no work value
  u8 riseCounter;       ///< SW_RISE_COUNTER, kept by scene code
  u8 riseEnabled;       ///< SW_RISE_ENABLED, kept by scene code
  s16 timer;            ///< work value 0x23, what its actor script reads
  s16 userValue;        ///< work value 0x24, or scene scratch
  u32 linkedActor;      ///< work value 0x25, the actor it follows or faces
  u32 update;           ///< SW_HOOK, the routine run each frame
};

STATIC_ASSERT(offsetof(struct ScriptOperands, scriptAddress) == 0x00);
STATIC_ASSERT(offsetof(struct ScriptOperands, cursor) == 0x04);
STATIC_ASSERT(offsetof(struct ScriptOperands, facing) == 0x06);
STATIC_ASSERT(offsetof(struct ScriptOperands, radius) == 0x20);
STATIC_ASSERT(offsetof(struct ScriptOperands, riseBaseY) == 0x4c);
STATIC_ASSERT(offsetof(struct ScriptOperands, spriteStorageKind) == 0x54);
STATIC_ASSERT(offsetof(struct ScriptOperands, repeatCount) == 0x5d);
STATIC_ASSERT(offsetof(struct ScriptOperands, refused) == 0x60);
STATIC_ASSERT(offsetof(struct ScriptOperands, conditionResult) == 0x57);
STATIC_ASSERT(offsetof(struct ScriptOperands, wait) == 0x5e);
STATIC_ASSERT(offsetof(struct ScriptOperands, timer) == 0x64);
STATIC_ASSERT(offsetof(struct ScriptOperands, linkedActor) == 0x68);
STATIC_ASSERT(offsetof(struct ScriptOperands, update) == 0x6c);
STATIC_ASSERT(sizeof(struct ScriptOperands) == 0x70);

#endif
