#include "script_operands.h"

#define Script_ClearByte54 Func_0800ca44

s32 Script_ClearByte54(struct ScriptOperands *work)
{
    work->byte_54 = 0;
    work->cursor++;
    return 1;
}
