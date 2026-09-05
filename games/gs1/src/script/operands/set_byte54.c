#include "script_operands.h"

#define Script_SetByte54 Func_0800ca58

s32 Script_SetByte54(struct ScriptOperands *work)
{
    work->byte_54 = 1;
    work->cursor++;
    return 1;
}
