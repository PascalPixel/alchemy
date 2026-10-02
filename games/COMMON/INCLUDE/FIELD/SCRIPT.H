#ifndef GUARD_FIELD_SCRIPT_H
#define GUARD_FIELD_SCRIPT_H

#include "EDITION.H"
#include "TYPES.H"

/* Command callbacks observe the current script, its signed cursor and
   condition/repeat state. The operand view below exposes the same work
   cells for the existing mixed-width set/add/compare operations. */
struct ScriptInterpreter;

typedef s32 (*ScriptCommand)(struct ScriptInterpreter *);

struct ScriptInterpreter {
    const s32 *script;
    s16 cursor;
    u8 padding_06[0x4e];
    u8 byte_54;
    u8 padding_55[2];
    u8 condition_result;
    u8 padding_58[5];
    u8 repeat_count;
    s16 lookup_result;
    u8 delay;
};

/* Script operand work. Mixed widths stay as placed in the image. */
struct ScriptOperands {
    u32 script_address;      /* 0x00 */
    u16 cursor;              /* 0x04; cast to s16 for signed interpretation */
    u16 unsigned_halfword;   /* 0x06 */
    u32 word_08;
    u32 word_0c;
    u32 word_10;
    u32 word_14;
    u32 word_18;
    u32 word_1c;
    u16 halfword_20;
    u8 padding_22[2];
    u32 word_24;
    u32 word_28;
    u32 word_2c;
    u32 word_30;
    u32 word_34;
    u32 word_38;
    u32 word_3c;
    u32 word_40;
    u32 word_44;
    u32 word_48;
    u32 word_4c;
    u32 address_50;
    u8 byte_54;
    u8 byte_55;
    u8 byte_56;
    u8 comparison_result;
    u8 byte_58;
    u8 byte_59;
    u8 byte_5a;
    u8 byte_5b;
    u8 byte_5c;
    u8 byte_5d;
    u16 halfword_5e;
    u8 byte_60;
    u8 byte_61;
    u8 byte_62;
    u8 byte_63;
    s16 signed_halfword_64;
    s16 signed_halfword_66;
    u32 word_68;
    u32 word_6c;
};

#define SCRIPT_OPERANDS_OFFSET(field) \
    ((u32)&(((struct ScriptOperands *)0)->field))

typedef char ScriptOperands_comparison_result_offset[
    SCRIPT_OPERANDS_OFFSET(comparison_result) == 0x57 ? 1 : -1
];
typedef char ScriptOperands_halfword_5e_offset[
    SCRIPT_OPERANDS_OFFSET(halfword_5e) == 0x5e ? 1 : -1
];
typedef char ScriptOperands_signed_halfword_64_offset[
    SCRIPT_OPERANDS_OFFSET(signed_halfword_64) == 0x64 ? 1 : -1
];
typedef char ScriptOperands_word_68_offset[
    SCRIPT_OPERANDS_OFFSET(word_68) == 0x68 ? 1 : -1
];
typedef char ScriptOperands_word_6c_offset[
    SCRIPT_OPERANDS_OFFSET(word_6c) == 0x6c ? 1 : -1
];
typedef char ScriptOperands_size[
    sizeof(struct ScriptOperands) == 0x70 ? 1 : -1
];

#undef SCRIPT_OPERANDS_OFFSET

#if defined(TLA_EDITION_JA) || defined(TLA_EDITION_EN) || \
    defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) || \
    defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
s32 Script_JumpToNextOperandPointer(struct ScriptInterpreter *interpreter);
#else
s32 Script_AdvancePastCursor(struct ScriptInterpreter *interpreter);
#endif
s32 Script_ClearByte54(struct ScriptInterpreter *interpreter);
s32 Script_SetByte54(struct ScriptInterpreter *interpreter);

#endif
