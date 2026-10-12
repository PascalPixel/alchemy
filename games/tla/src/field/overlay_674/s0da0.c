#include "ov674.h"

struct ObjectRuntime *Func_02000da0(struct ObjectRuntime *object, s32 value)
{
    struct ObjectRuntime *spawned;

    spawned = (struct ObjectRuntime *)Value4((s32 (*)())Func_02003df4, 489, object->x, object->y + 0x180000, object->z);
    if (spawned != 0) {
        Object_SetModeById(gPartyState.current_owner, 28);
        spawned->motion_flags = 0;
        Func_02003e3c(spawned, value);
        ObjectDispatch_SetSingleChildFlags(spawned, 0);
        spawned->unknown_5c[0] = 1;
    }
    return spawned;
}

void Func_02000df8(struct ObjectRuntime *object)
{
    struct ObjectRuntime *spawned;

    if ((gFrameCount & 3) == 0) {
        spawned = Func_02003df4(14, object->x, object->y, object->z);
        if (spawned != 0) {
            ObjectDispatch_SetSingleChildFlags(spawned, 0);
            Func_02003de4(spawned, 1);
            Func_02003dec(spawned, Data_020044f0);
        }
    }
}
