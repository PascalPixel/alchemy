#include "kibombo.h"

s32 Func_02000038(struct KibomboObject *object)
{
    object->sprite->rotation += 0x600;
    return 0;
}

s32 Func_02000048(struct KibomboObject *object)
{
    object->field_44 = object->x;
    object->field_48 = object->y;
    return 0;
}
