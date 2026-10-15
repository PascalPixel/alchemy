#ifndef GUARD_FIELD_SCRIPT_H
#define GUARD_FIELD_SCRIPT_H

#include "edition.h"
#include "types.h"

struct ScriptInterpreter;

typedef s32 (*ScriptCommand)(struct ScriptInterpreter *);

struct ScriptInterpreter
{
	const s32 *script;
	s16		   cursor;
	u8		   padding06[0x4e];
	u8		   byte54;
	u8		   padding55[2];
	u8		   conditionResult;
	u8		   padding58[5];
	u8		   repeatCount;
	s16		   lookupResult;
	u8		   delay;
};

struct ScriptOperands
{
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
	u8	padding22[2];
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
	u8	byte54;
	u8	byte55;
	u8	byte56;
	u8	comparisonResult;
	u8	byte58;
	u8	byte59;
	u8	byte5a;
	u8	byte5b;
	u8	byte5c;
	u8	byte5d;
	u16 halfword5e;
	u8	byte60;
	u8	byte61;
	u8	byte62;
	u8	byte63;
	s16 signedHalfword64;
	s16 signedHalfword66;
	u32 word68;
	u32 word6c;
};

#define SCRIPT_OPERANDS_OFFSET(field) ((u32) & (((struct ScriptOperands *)0)->field))

typedef char
	ScriptOperands_comparison_result_offset[SCRIPT_OPERANDS_OFFSET(comparisonResult) == 0x57 ? 1
																							  : -1];
typedef char
	ScriptOperands_halfword_5e_offset[SCRIPT_OPERANDS_OFFSET(halfword5e) == 0x5e ? 1 : -1];
typedef char ScriptOperands_signed_halfword_64_offset
	[SCRIPT_OPERANDS_OFFSET(signedHalfword64) == 0x64 ? 1 : -1];
typedef char ScriptOperands_word_68_offset[SCRIPT_OPERANDS_OFFSET(word68) == 0x68 ? 1 : -1];
typedef char ScriptOperands_word_6c_offset[SCRIPT_OPERANDS_OFFSET(word6c) == 0x6c ? 1 : -1];
typedef char ScriptOperands_size[sizeof(struct ScriptOperands) == 0x70 ? 1 : -1];

#undef SCRIPT_OPERANDS_OFFSET

#if defined(TLA_EDITION_JA) || defined(TLA_EDITION_EN) || defined(TLA_EDITION_DE) \
	|| defined(TLA_EDITION_ES) || defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
s32 Script_JumpToNextOperandPointer(struct ScriptInterpreter *interpreter);
#else
#if GAME_TLA
s32 Script_AdvancePastCursor(struct ScriptInterpreter *interpreter);
#endif
#endif
#if GAME_TLA
s32 Script_ClearByte54(struct ScriptInterpreter *interpreter);
s32 Script_SetByte54(struct ScriptInterpreter *interpreter);
#endif

#endif
