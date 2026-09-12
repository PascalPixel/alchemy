#include "script_interpreter.h"

/* script/advance_past_cursor.c */
/* script/interpreter/cmd/advance.c */
s32 Script_AdvancePastCursor(struct ScriptInterpreter *interpreter)
{
    s32 zero;

    interpreter->script = interpreter->script + interpreter->cursor + 1;
    zero = 0;
    interpreter->cursor = zero;
    return 1;
}
