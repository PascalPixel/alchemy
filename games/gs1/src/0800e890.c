#include "script_operands.h"

void Func_0800e890(struct ScriptOperands *work, s32 operation, s32 value)
{
    s32 result;

    if (operation == 0) {
        work->signed_halfword_66 = value;
    } else if (operation == 1) {
        work->signed_halfword_66 =
            (u16)work->signed_halfword_66 + value;
    } else {
        result = 0;
        if (work->signed_halfword_66 == (s16)value)
            result = 1;
        work->comparison_result = result;
    }
}
