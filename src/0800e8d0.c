#include "script_operands.h"

void Func_0800e8d0(struct ScriptOperands *work, s32 operation, s32 value)
{
    s8 result;

    if (operation == 0) {
        work->word_68 = value;
        return;
    }
    if (operation == 1) {
        work->word_68 = work->word_68 + (u32)value;
        return;
    }
    result = 0;
    if (work->word_68 == (u32)value) {
        result = 1;
    }
    work->comparison_result = result;
}
