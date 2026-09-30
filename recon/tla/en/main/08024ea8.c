/*
 * Draft: Script_JumpToLabel does not yet match; 5 halfwords differ from ☀️'s C, first at +0x14 (ldrh r3, [r5, #4]).
 * Links as recon/tla/raw/08024ea8.s.
 */
#include "SCRIPT_INTERPRETER.H"

s32 Script_JumpToLabel(struct ScriptInterpreter *interpreter)
{
    interpreter->cursor =
        Script_FindLabel(interpreter, interpreter->script[interpreter->cursor + 1]);
    return 1;
}
