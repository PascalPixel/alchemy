#include "ov670.h"

s32 Func_02000038(struct Ov670Object *object)
{
    object->sprite->rotation += 0x600;
    return 0;
}

s32 Func_02000048(struct Ov670Object *object)
{
    object->field_44 = object->x;
    object->field_48 = object->y;
    return 0;
}
