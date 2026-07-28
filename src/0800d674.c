#include "script_interpreter.h"

s32 Func_0800ca98(void *);

s32 Func_0800d674(struct ScriptInterpreter *interpreter) {
    if ((u32)interpreter->delay > 0x3B) {
        interpreter->delay = 0;
        goto block_3;
    }
    if (Func_0800ca98(interpreter) != 0) {
block_3:
        interpreter->cursor = (u16)interpreter->cursor + 1;
        return 1;
    }
    return 0;
}
