#include "local.h"

void Func_020007a8(void)
{
    struct ObjectRuntime *object;
    struct FieldSpriteFlags *sprite;

    Func_02003eec(10, 0x1c8);
    object = Object_GetById(20);
    sprite = object->animation;
    sprite->flag_0 = 1;
    object->motion_flags = 0;
    object->y = 0x180000;
}
