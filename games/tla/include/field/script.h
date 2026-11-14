#ifndef GUARD_FIELD_SCRIPT_H
#define GUARD_FIELD_SCRIPT_H

#include "edition.h"
#include "types.h"
#include "static_assert.h"

struct ScriptInterpreter;

typedef s32 (*ScriptCommand)(struct ScriptInterpreter *);

/// The prefix that command handlers read from a field object's 0x70-byte slot.
struct ScriptInterpreter {
  const s32 *script;    ///< the running script; SC_END sets gScript_MainScript
  s16 cursor;           ///< the word index of the next command in script
  u8 padding06[0x4e];   ///< the object's own fields; not read by the commands
  u8 spriteStorageKind; ///< SC_B54_ON (show a sprite) sets 1, SC_B54_OFF 0
  u8 padding55[2];      ///< the object's own fields; not read by the commands
  u8 conditionResult;   ///< the last flag test, read by SC_IF and SC_IFNOT
  u8 padding58[5];      ///< the object's own fields; not read by the commands
  u8 repeatCount;       ///< SC_LOOP's count of jumps so far
  /// Frames left of SC_WAIT (n - 1 when it starts); ActorScript_FindLabel
  /// clears it.
  s16 lookupResult;
  /// Frames a solid object has blocked the object; SC_SYNC stops waiting
  /// after 60 and clears it.
  u8 delay;
};

STATIC_ASSERT(sizeof(struct ScriptInterpreter) == 0x64);
STATIC_ASSERT(offsetof(struct ScriptInterpreter, script) == 0x00);
STATIC_ASSERT(offsetof(struct ScriptInterpreter, cursor) == 0x04);
STATIC_ASSERT(offsetof(struct ScriptInterpreter, spriteStorageKind) == 0x54);
STATIC_ASSERT(offsetof(struct ScriptInterpreter, conditionResult) == 0x57);
STATIC_ASSERT(offsetof(struct ScriptInterpreter, repeatCount) == 0x5d);
STATIC_ASSERT(offsetof(struct ScriptInterpreter, lookupResult) == 0x5e);
STATIC_ASSERT(offsetof(struct ScriptInterpreter, delay) == 0x60);

/// Raw fields of the full slot, addressed by the set/add/compare commands.
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
  u8 padding22[2];
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
  u8 spriteStorageKind; ///< ScriptInterpreter.spriteStorageKind
  u8 motionFlags;
  u8 arrivalAxis;
  u8 comparisonResult; ///< ScriptInterpreter.conditionResult
  u8 snapToTarget;
  u8 collisionFlags;
  u8 steeringFlags;
  u8 paused;
  u8 retainSpriteCache;
  u8 repeatCount; ///< ScriptInterpreter.repeatCount, the SC_LOOP count
  u16 wait;       ///< ScriptInterpreter.lookupResult, SC_WAIT's frames left
  u8 refused;     ///< ScriptInterpreter.delay, the blocked frames
  u8 frozen;
  u8 riseCounter;
  u8 riseEnabled;
  s16 timer;
  s16 userValue;
  u32 watchedActor;
  u32 update;
};

STATIC_ASSERT(offsetof(struct ScriptOperands, script) == 0x00);
STATIC_ASSERT(offsetof(struct ScriptOperands, cursor) == 0x04);
STATIC_ASSERT(offsetof(struct ScriptOperands, facing) == 0x06);
STATIC_ASSERT(offsetof(struct ScriptOperands, spriteStorageKind) == 0x54);
STATIC_ASSERT(offsetof(struct ScriptOperands, repeatCount) == 0x5d);
STATIC_ASSERT(offsetof(struct ScriptOperands, refused) == 0x60);
STATIC_ASSERT(offsetof(struct ScriptOperands, comparisonResult) == 0x57);
STATIC_ASSERT(offsetof(struct ScriptOperands, wait) == 0x5e);
STATIC_ASSERT(offsetof(struct ScriptOperands, timer) == 0x64);
STATIC_ASSERT(offsetof(struct ScriptOperands, watchedActor) == 0x68);
STATIC_ASSERT(offsetof(struct ScriptOperands, update) == 0x6c);
STATIC_ASSERT(sizeof(struct ScriptOperands) == 0x70);

s32 Script_JumpToNextOperandPointer(struct ScriptInterpreter *interpreter);
s32 ActorScript_HideSprites(struct ScriptInterpreter *interpreter);
s32 ActorScript_ShowSprite(struct ScriptInterpreter *interpreter);

#endif
