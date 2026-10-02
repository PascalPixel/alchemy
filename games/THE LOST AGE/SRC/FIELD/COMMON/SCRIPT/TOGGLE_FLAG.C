#include "SCRIPT.H"

s32 GameFlag_Test(s32);
void GameFlag_ClearBit(s32);
void GameFlag_SetBit(s32);

/* Toggles the flag named by the operand, keeping its old state as the
   condition result. */
s32 Script_ToggleFlag(struct ScriptInterpreter *interpreter)
{
    s32 value;
    s32 cursor;
    s32 done;

    value = interpreter->script[interpreter->cursor + 1];
    interpreter->condition_result = GameFlag_Test(value);
    if (interpreter->condition_result == 1)
        GameFlag_ClearBit(value);
    else
        GameFlag_SetBit(value);
    cursor = (u16)interpreter->cursor;
    done = 1;
    asm volatile("" : "+l"(done)); /* FAKEMATCH: ⚓️ sets the result between the cursor load and store */
    interpreter->cursor = cursor + 2;
    return done;
}

struct DispatchObject;
void ObjectDispatch_ApplyArgumentToChildren(struct DispatchObject *object, s32 argument);

/* Passes the operand to the children of the object the interpreter runs. */
s32 Script_ApplyArgumentToChildren(struct ScriptInterpreter *interpreter)
{
    s32 cursor;
    s32 done;

    ObjectDispatch_ApplyArgumentToChildren((struct DispatchObject *)interpreter,
                                           interpreter->script[interpreter->cursor + 1]);
    cursor = (u16)interpreter->cursor;
    done = 1;
    asm volatile("" : "+l"(done)); /* FAKEMATCH: ⚓️ sets the result between the cursor load and store */
    interpreter->cursor = cursor + 2;
    return done;
}
