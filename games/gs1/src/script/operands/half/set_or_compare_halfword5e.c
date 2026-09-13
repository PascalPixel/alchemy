#include "script_operands.h"

void Script_SetOrCompareHalfword5e(struct ScriptOperands *work, s32 operation, s32 value)
{
    s8 result;

    if (operation == 0) {
        work->halfword_5e = value;
        return;
    }
    if (operation == 1) {
        work->halfword_5e = work->halfword_5e + value;
        return;
    }
    result = 0;
    if ((s16)work->halfword_5e == (s16)value) {
        result = 1;
    }
    work->comparison_result = result;
}
