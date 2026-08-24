#include "script_operands.h"

void Script_SetOrCompareSignedHalfword64(struct ScriptOperands *work, s32 operation, s32 value)
{
    s32 result;

    if (operation == 0) {
        work->signed_halfword_64 = value;
    } else if (operation == 1) {
        work->signed_halfword_64 =
            (u16)work->signed_halfword_64 + value;
    } else {
        result = 0;
        if (work->signed_halfword_64 == (s16)value)
            result = 1;
        work->comparison_result = result;
    }
}
