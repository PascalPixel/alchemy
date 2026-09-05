#include "script_operands.h"

s32 Script_SetByte54(struct ScriptOperands *work)
{
    work->byte_54 = 1;
    work->cursor++;
    return 1;
}
