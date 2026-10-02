#include "SCRIPT.H"

/* The two games differ in the first command: TBS advances the current
   pointer; TLA follows the pointer stored in the next operand. */
#if defined(TLA_EDITION_JA) || defined(TLA_EDITION_EN) || \
    defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) || \
    defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
s32 Script_JumpToNextOperandPointer(struct ScriptInterpreter *interpreter)
#else
s32 Script_AdvancePastCursor(struct ScriptInterpreter *interpreter)
#endif
{
    s32 zero;

#if defined(TLA_EDITION_JA) || defined(TLA_EDITION_EN) || \
    defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) || \
    defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
    interpreter->script =
        (const s32 *)interpreter->script[interpreter->cursor + 1];
#else
    interpreter->script =
        interpreter->script + interpreter->cursor + 1;
#endif
    zero = 0;
    interpreter->cursor = zero;
    return 1;
}

s32 Script_ClearByte54(struct ScriptInterpreter *interpreter)
{
    interpreter->byte_54 = 0;
    interpreter->cursor = (u16)interpreter->cursor + 1;
    return 1;
}

s32 Script_SetByte54(struct ScriptInterpreter *interpreter)
{
    interpreter->byte_54 = 1;
    interpreter->cursor = (u16)interpreter->cursor + 1;
    return 1;
}
