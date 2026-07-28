#include "script_operands.h"

void Func_0800e964(struct ScriptOperands *work, s32 operation, s32 value)
{
    s32 result;
    if (operation == 0) {
        work->byte_63 = value;
    } else if (operation == 1) {
        work->byte_63 += value;
    } else {
        result = 0;
        if (work->byte_63 == (u8)value)
            result = 1;
        work->comparison_result = result;
    }
}
