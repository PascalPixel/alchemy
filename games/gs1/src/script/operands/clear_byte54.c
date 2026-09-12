#include "script_operands.h"

s32 Script_ClearByte54(struct ScriptOperands *work)
{
    work->byte_54 = 0;
    work->cursor++;
    return 1;
}
