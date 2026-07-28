#include "script_interpreter.h"

s32 Func_0800d710(struct ScriptInterpreter *interpreter) {
    const s32 *ptr;
    s32 temp_r4;
    s32 temp_r1;
    u8 *p;
    s32 temp_r2;

    ptr = &interpreter->script[interpreter->cursor + 1];
    temp_r4 = *ptr++;
    temp_r1 = *ptr;
    if (temp_r4 == 0xFFFF) {
        interpreter->cursor = Func_0800d6d8(interpreter, temp_r1);
    } else {
        p = &interpreter->repeat_count;
        temp_r2 = *p + 1;
        *p = temp_r2;
        if ((s32) (u8) temp_r2 < (s32) (s16) temp_r4) {
            interpreter->cursor = Func_0800d6d8(interpreter, temp_r1);
        } else {
            *p = 0;
            interpreter->cursor = interpreter->cursor + 3;
        }
    }
    return 1;
}
