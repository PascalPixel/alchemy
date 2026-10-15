#include "room.h"

void Func_02002f6c(void);

void Func_0200328c(s32 id, s32 target, s32 kind, s32 a3, const void *script)
{
    struct ObjectRuntime *source = Object_GetById(id);
    struct ObjectRuntime *object = Object_GetById(target);

    Object_SetPositionAndResetMotion(object, source->x, source->y, source->z);
    Func_02005490(object, script);
    ObjectDispatch_SetSingleChildFlags(object, 0);
    object->motionFlags = 0;
    Func_02005480(object, 1);
    object->linkedObject = source;
    object->action = 0;
    object->unk66 = a3;
    BYTE_AT(object, 98) = kind;
    object->update = Func_02002f6c;
}
