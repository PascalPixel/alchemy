#ifndef ALCHEMY_FIELD_ACTION_SCRIPT_H
#define ALCHEMY_FIELD_ACTION_SCRIPT_H

/// @file
/// The object script interpreter and the operands its commands read.

#include "edition.h"
#include "types.h"
#include "static_assert.h"

struct ObjectScriptContext;

typedef s32 (*ScriptCommand)(struct ObjectScriptContext *);

/// The prefix that command handlers read from an object's 0x70-byte slot.
struct ObjectScriptContext {
  const s32 *script;  ///< the running script; SC_END sets gScript_MainScript
  s16 cursor;         ///< the word index of the next command in script
  u8 padding06[0x4e]; ///< the object's own fields; not read by the commands
  /// ObjectEvent's spriteStorageKind: SC_SHOW_SPRITE sets 1, one sprite drawn,
  /// and SC_HIDE_SPRITES sets 0, which hides the object's sprites.
  u8 spriteStorageKind;
  u8 padding55[2];    ///< the object's own fields; not read by the commands
  u8 conditionResult; ///< the last flag test, read by SC_IF and SC_IFNOT
  u8 padding58[5];    ///< the object's own fields; not read by the commands
  u8 repeatCount;     ///< SC_LOOP's count of jumps so far
  /// Frames left of SC_WAIT (n - 1 when it starts); the update loops count
  /// it down, and ObjectScript_FindLabel clears it.
  s16 wait;
  /// Frames in a row a solid object has refused the object's move
  /// (ScriptMotionObject.refused); SC_SYNC stops waiting after 60 and clears
  /// it.
  u8 refused;
};

STATIC_ASSERT(sizeof(struct ObjectScriptContext) == 0x64);
STATIC_ASSERT(offsetof(struct ObjectScriptContext, script) == 0x00);
STATIC_ASSERT(offsetof(struct ObjectScriptContext, cursor) == 0x04);
STATIC_ASSERT(offsetof(struct ObjectScriptContext, spriteStorageKind) == 0x54);
STATIC_ASSERT(offsetof(struct ObjectScriptContext, conditionResult) == 0x57);
STATIC_ASSERT(offsetof(struct ObjectScriptContext, repeatCount) == 0x5d);
STATIC_ASSERT(offsetof(struct ObjectScriptContext, wait) == 0x5e);
STATIC_ASSERT(offsetof(struct ObjectScriptContext, refused) == 0x60);

/// The whole 0x70-byte slot as SC_SET, SC_ADD and SC_CMP see it: each field
/// is one work value (enum ObjectScriptValue in objcmd.h), reached through
/// gObjectScript_OperandHandlerTable, and named for what the engine keeps
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
  u32 bounce;        ///< SW_BOUNCE, or a floating object's phase
  u32 gravity;       ///< SW_GRAVITY
  /// Work value 0x17: the word Object_CreateField zeroes and only map code
  /// reads.
  u32 mapWord;
  u32 sprite;           ///< work value 0x18: the sprite, or its list of four
  u8 spriteStorageKind; ///< work value 0x19: 0 hidden, 1 one, 2 a group
  u8 motionFlags;       ///< SW_MOTION, ObjectEventMotionFlag bits
  u8 arrivalAxis;       ///< SW_ARRIVAL
  u8 conditionResult;   ///< SW_RESULT, the last test; every compare sets it
  u8 snapToTarget;      ///< SW_SNAP
  u8 collisionFlags;    ///< SW_COLLISION
  u8 steeringFlags;     ///< SW_STEERING
  u8 paused;            ///< SW_PAUSED
  u8 byte5c;            ///< keeps the sprite's VRAM; no work value
  u8 repeatCount;       ///< SW_REPEAT, SC_LOOP's count
  u16 wait;             ///< SW_WAIT, compared as signed
  u8 refused;           ///< ObjectScriptContext.refused; no work value
  u8 frozen;            ///< ScriptMotionObject.frozen; no work value
  u8 mapByte62;         ///< work value 0x27, kept by map code
  u8 mapByte63;         ///< work value 0x28, kept by map code
  s16 actionParameter;  ///< work value 0x23, what its action script reads
  s16 footprintSide;    ///< work value 0x24, or map scratch
  u32 linkedObject;     ///< work value 0x25, the object it follows or faces
  u32 hook;             ///< SW_HOOK, the routine run each frame
};

STATIC_ASSERT(offsetof(struct ScriptOperands, scriptAddress) == 0x00);
STATIC_ASSERT(offsetof(struct ScriptOperands, cursor) == 0x04);
STATIC_ASSERT(offsetof(struct ScriptOperands, facing) == 0x06);
STATIC_ASSERT(offsetof(struct ScriptOperands, radius) == 0x20);
STATIC_ASSERT(offsetof(struct ScriptOperands, mapWord) == 0x4c);
STATIC_ASSERT(offsetof(struct ScriptOperands, spriteStorageKind) == 0x54);
STATIC_ASSERT(offsetof(struct ScriptOperands, repeatCount) == 0x5d);
STATIC_ASSERT(offsetof(struct ScriptOperands, refused) == 0x60);
STATIC_ASSERT(offsetof(struct ScriptOperands, conditionResult) == 0x57);
STATIC_ASSERT(offsetof(struct ScriptOperands, wait) == 0x5e);
STATIC_ASSERT(offsetof(struct ScriptOperands, actionParameter) == 0x64);
STATIC_ASSERT(offsetof(struct ScriptOperands, linkedObject) == 0x68);
STATIC_ASSERT(offsetof(struct ScriptOperands, hook) == 0x6c);
STATIC_ASSERT(sizeof(struct ScriptOperands) == 0x70);

#endif
