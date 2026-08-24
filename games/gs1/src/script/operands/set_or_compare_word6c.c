#include "script_operands.h"

void Script_SetOrCompareWord6c(struct ScriptOperands *work, s32 operation, s32 value)
{
    s8 result;

    if (operation == 0) {
        work->word_6c = value;
        return;
    }
    if (operation == 1) {
        work->word_6c = work->word_6c + (u32)value;
        return;
    }
    result = 0;
    if (work->word_6c == (u32)value) {
        result = 1;
    }
    work->comparison_result = result;
}
