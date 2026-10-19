#ifndef GUARD_FIELD_SCRIPT_H
#define GUARD_FIELD_SCRIPT_H

#include "edition.h"
#include "types.h"
#include "static_assert.h"

struct ScriptInterpreter;

typedef s32 (*ScriptCommand)(struct ScriptInterpreter *);

// The prefix that command handlers read from a field object's 0x70-byte slot.
struct ScriptInterpreter {
  const s32 *script;
  s16 cursor;
  u8 padding06[0x4e];
  u8 byte54;
  u8 padding55[2];
  u8 conditionResult;
  u8 padding58[5];
  u8 repeatCount;
  s16 lookupResult;
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

// Raw fields of the full slot, addressed by the set/add/compare commands.
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
  u8 byte54;
  u8 byte55;
  u8 byte56;
  u8 comparisonResult;
  u8 byte58;
  u8 byte59;
  u8 byte5a;
  u8 byte5b;
  u8 byte5c;
  u8 byte5d;
  u16 halfword5e;
  u8 byte60;
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

s32 Script_JumpToNextOperandPointer(struct ScriptInterpreter *interpreter);
s32 Script_ClearByte54(struct ScriptInterpreter *interpreter);
s32 Script_SetByte54(struct ScriptInterpreter *interpreter);

#endif
