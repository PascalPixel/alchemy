#include "script_operands.h"

typedef void (*OperandFunc)(struct ScriptOperands *, s32, s32);
extern OperandFunc Data_080136e0[];

s32 Func_0800e9a0(struct ScriptOperands *work)
{
    s16 index = work->signed_halfword;
    u8 *entry = (u8 *)(work->script_address + index * 4 + 4);
    OperandFunc callback = Data_080136e0[*(s32 *)entry];

    if (callback != 0)
        callback(work, 0, *(s32 *)(entry + 4));
    work->signed_halfword = (u16)work->signed_halfword + 3;
    return 1;
}
