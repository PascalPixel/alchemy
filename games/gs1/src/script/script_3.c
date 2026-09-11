#include "scene.h"
#include "script_interpreter.h"
#include "script_operands.h"
#include "types.h"

/* script/interpreter/cmd/advance.c */
s32 Script_AdvancePastCursor(struct ScriptInterpreter *interpreter)
{
    s32 zero;

    interpreter->script = interpreter->script + interpreter->cursor + 1;
    zero = 0;
    interpreter->cursor = zero;
    return 1;
}

/* script/operands/byte/clear_byte54.c */
s32 Script_ClearByte54(struct ScriptOperands *work)
{
    work->byte_54 = 0;
    work->cursor++;
    return 1;
}

/* script/operands/byte/set_byte54.c */
s32 Script_SetByte54(struct ScriptOperands *work)
{
    work->byte_54 = 1;
    work->cursor++;
    return 1;
}

/* script/interpreter/wait/timeout.c */
void WaitFrames(s32);

void Script_WaitForEventTimeout(s32 arg0)
{
    s32 cnt;

    cnt = 0;
    while (cnt <= 0x257 && Script_Check(arg0) == 0) {
        WaitFrames(1);
        cnt++;
    }
}
