#include "mars_star.h"

void Func_02002a58(s32 sourceId, s32 id, s32 kind, s32 value, const void *script)
{
    struct ObjectRuntime *source = Object_GetById(sourceId);
    struct ObjectRuntime *object = Object_GetById(id);

    Object_SetPositionAndResetMotion(object, source->x, source->y, source->z);
    Func_020041dc(object, script);
    ObjectDispatch_SetSingleChildFlags(object, 0);
    object->motionFlags = 0;
    Func_020041cc(object, 1);
    *(struct ObjectRuntime **)((u8 *)object + 0x68) = source;
    object->action = 0;
    *(s16 *)((u8 *)object + 0x66) = value;
    *(u8 *)((u8 *)object + 0x62) = kind;
    object->update = Func_02002738;
}
