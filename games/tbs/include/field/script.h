#ifndef GUARD_FIELD_SCRIPT_H
#define GUARD_FIELD_SCRIPT_H

/// @file
/// The field object script interpreter and the operands its commands read.

#include "edition.h"
#include "types.h"
#include "static_assert.h"

struct ScriptInterpreter;

typedef s32 (*ScriptCommand)(struct ScriptInterpreter *);

/// The prefix that command handlers read from a field object's 0x70-byte slot.
struct ScriptInterpreter {
  const s32 *script;  ///< the running script; SC_END sets gScript_MainScript
  s16 cursor;         ///< the word index of the next command in script
  u8 padding06[0x4e]; ///< the object's own fields; not read by the commands
  /// FieldActor's spriteStorageKind: SC_B54_ON sets 1, one sprite drawn, and
  /// SC_B54_OFF sets 0, which hides the object's sprites.
  u8 byte54;
  u8 padding55[2];    ///< the object's own fields; not read by the commands
  u8 conditionResult; ///< the last flag test, read by SC_IF and SC_IFNOT
  u8 padding58[5];    ///< the object's own fields; not read by the commands
  u8 repeatCount;     ///< SC_LOOP's count of jumps so far
  /// Frames left of SC_WAIT (n - 1 when it starts); Script_FindLabel clears
  /// it.
  s16 lookupResult;
  /// Frames a solid object has blocked the object; SC_SYNC stops waiting
  /// after 60 and clears it.
  u8 delay;
};

STATIC_ASSERT(sizeof(struct ScriptInterpreter) == 0x64);
STATIC_ASSERT(offsetof(struct ScriptInterpreter, script) == 0x00);
STATIC_ASSERT(offsetof(struct ScriptInterpreter, cursor) == 0x04);
STATIC_ASSERT(offsetof(struct ScriptInterpreter, byte54) == 0x54);
STATIC_ASSERT(offsetof(struct ScriptInterpreter, conditionResult) == 0x57);
STATIC_ASSERT(offsetof(struct ScriptInterpreter, repeatCount) == 0x5d);
STATIC_ASSERT(offsetof(struct ScriptInterpreter, lookupResult) == 0x5e);
STATIC_ASSERT(offsetof(struct ScriptInterpreter, delay) == 0x60);

/// Raw fields of the full slot, addressed by the set/add/compare commands.
struct ScriptOperands {
  u32 scriptAddress;
  u16 cursor;
  u16 unsignedHalfword;
  u32 word08;
  u32 word0c;
  u32 word10;
  u32 word14;
  u32 word18;
  u32 word1c;
  u16 halfword20;
  u8 padding22[2];
  u32 word24;
  u32 word28;
  u32 word2c;
  u32 word30;
  u32 word34;
  u32 word38;
  u32 word3c;
  u32 word40;
  u32 word44;
  u32 word48;
  u32 word4c;
  u32 address50;
  u8 byte54; ///< ScriptInterpreter.byte54, the sprite storage kind
  u8 byte55;
  u8 byte56;
  u8 comparisonResult; ///< ScriptInterpreter.conditionResult
  u8 byte58;
  u8 byte59;
  u8 byte5a;
  u8 byte5b;
  u8 byte5c;
  u8 byte5d;      ///< ScriptInterpreter.repeatCount, the SC_LOOP count
  u16 halfword5e; ///< ScriptInterpreter.lookupResult, SC_WAIT's frames left
  u8 byte60;      ///< ScriptInterpreter.delay, the blocked frames
  u8 byte61;
  u8 byte62;
  u8 byte63;
  s16 signedHalfword64;
  s16 signedHalfword66;
  u32 word68;
  u32 word6c;
};

STATIC_ASSERT(offsetof(struct ScriptOperands, scriptAddress) == 0x00);
STATIC_ASSERT(offsetof(struct ScriptOperands, cursor) == 0x04);
STATIC_ASSERT(offsetof(struct ScriptOperands, unsignedHalfword) == 0x06);
STATIC_ASSERT(offsetof(struct ScriptOperands, byte54) == 0x54);
STATIC_ASSERT(offsetof(struct ScriptOperands, byte5d) == 0x5d);
STATIC_ASSERT(offsetof(struct ScriptOperands, byte60) == 0x60);
STATIC_ASSERT(offsetof(struct ScriptOperands, comparisonResult) == 0x57);
STATIC_ASSERT(offsetof(struct ScriptOperands, halfword5e) == 0x5e);
STATIC_ASSERT(offsetof(struct ScriptOperands, signedHalfword64) == 0x64);
STATIC_ASSERT(offsetof(struct ScriptOperands, word68) == 0x68);
STATIC_ASSERT(offsetof(struct ScriptOperands, word6c) == 0x6c);
STATIC_ASSERT(sizeof(struct ScriptOperands) == 0x70);

#endif
