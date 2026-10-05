#include "TYPES.H"
#include "SYSTEM.H"
#include "OBJECT_RUNTIME.H"

s32 Object_IsTargetUnset(struct ObjectRuntime *object);

void Script_WaitForEventTimeout(s32 arg0)
{
    s32 cnt;

    cnt = 0;
    while (cnt <= 0x257 && Object_IsTargetUnset((struct ObjectRuntime *)arg0) == 0) {
        WaitFrames(1);
        cnt++;
    }
}

s32 Object_IsTargetUnset(struct ObjectRuntime *object)
{
    s32 first;
    s32 second;

    if (object->flags == 0) {
        second = object->target_x;
        if (second != (s32)0x80000000)
            return 0;
        first = object->target_y;
    } else {
        first = object->target_x;
        second = (s32)0x80000000;
    }
    if (first != second || object->target_z != first)
        return 0;
    return 1;
}
