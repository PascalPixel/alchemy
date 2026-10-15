#include "script.h"
#include "object_runtime.h"

s32 Object_IsTargetUnset(struct ObjectRuntime *object);

s32 Script_StoreLookupResult(struct ScriptInterpreter *interpreter)
{
    s16 cursor = interpreter->cursor;

    interpreter->lookupResult = interpreter->script[cursor + 1] - 1;
    interpreter->cursor += 2;
    return 0;
}

// Waits until the object reaches its target or 60 frames pass; on the
// timeout the target is cleared. Returns 1 when the script moves on.
s32 Script_WaitForEvent(struct ScriptInterpreter *interpreter)
{
    struct ObjectRuntime *object = (struct ObjectRuntime *)interpreter;
    u8 *delay = &interpreter->delay;

    if (*delay > 59)
    {
        /* FAKEMATCH: two forced temporaries. The 0 in a variable: both byte stores share its register (r2), as the native; with the plain constant each store builds its own 0 and the second address is derived from the first (differ 16). The delay through a byte pointer: its store stays ahead of the target stores; through the field the scheduler moves it after them (differ 2). */
        u32 zero = 0;

        *delay = zero;
        object->targetX = 0x80000000;
        object->targetY = 0x80000000;
        object->targetZ = 0x80000000;
        object->unk56 = zero;
        interpreter->cursor++;
        return 1;
    }
    if (Object_IsTargetUnset(object) != 0)
    {
        interpreter->cursor++;
        return 1;
    }
    return 0;
}
