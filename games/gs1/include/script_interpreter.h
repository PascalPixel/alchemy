#ifndef ALCHEMY_SCRIPT_INTERPRETER_H
#define ALCHEMY_SCRIPT_INTERPRETER_H

#include "types.h"

struct ScriptInterpreter;

typedef s32 (*ScriptCommand)(struct ScriptInterpreter *);

struct ScriptInterpreter {
    const s32 *script;
    s16 cursor;
    u8 padding_06[0x51];
    u8 condition_result;
    u8 padding_58[5];
    u8 repeat_count;
    s16 lookup_result;
    u8 delay;
};

s32 Func_0800d6d8(struct ScriptInterpreter *interpreter, u32 key);

#endif
