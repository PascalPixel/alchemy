#include "SCRIPT.H"
#include "OBJECT_RUNTIME.H"

void Object_SetMoveTarget(void *, s32, s32, s32);

s32 Script_ApplyLocalOffsetPosition(struct ObjectRuntime *object)
{
    s32 done;
    s32 cursor;
    s32 offset[3];

    Object_SetMoveTarget(
        object,
        object->x + offset[0],
        object->y + offset[1],
        object->z + offset[2]
    );
    cursor = (u16)object->step;
    done = 1;
    asm volatile("" : "+l"(done)); /* FAKEMATCH: ⚓️ sets the result between the cursor load and store */
    object->step = cursor + 3;
    return done;
}
