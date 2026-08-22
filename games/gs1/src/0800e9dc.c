#include "script_operands.h"

typedef void (*OperandFunc)(struct ScriptOperands *, s32, s32);
extern OperandFunc Data_080136e0[];

s32 Func_0800e9dc(struct ScriptOperands *work)
{
    s16 index = (s16)work->cursor;
    u8 *entry = (u8 *)(work->script_address + index * 4 + 4);
    OperandFunc callback = Data_080136e0[*(s32 *)entry];

    if (callback != 0)
        callback(work, 1, *(s32 *)(entry + 4));
    work->cursor += 3;
    return 1;
}
