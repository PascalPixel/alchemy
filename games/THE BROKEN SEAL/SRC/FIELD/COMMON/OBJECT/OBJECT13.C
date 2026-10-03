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
    if (object->flags == 0)
        return object->target_x == (s32)0x80000000
            && object->target_y == (s32)0x80000000
            && object->target_z == (s32)0x80000000;
    return object->target_x == (s32)0x80000000
        && object->target_z == object->target_x;
}
