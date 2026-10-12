#include "ov6ab.h"

void Func_020019b4(s32 *position, s32 speed, s32 flag)
{
    struct ObjectRuntime *object = (struct ObjectRuntime *)Value4((s32 (*)())Func_020041e4, 366, position[0], position[1], position[2]);

    if (object != NULL) {
        object->bounce = 0;
        object->gravity = 0;
        object->unknown_4c = speed;
        Func_02001928(object, 3);
        object->update = Func_020017c8;
        if (flag != 0)
            object->unknown_5c[0] = 2;
        Func_020041cc(object, 2);
    }
}

void Func_020019fc(s32 *position, s32 speed, s32 flag)
{
    struct ObjectRuntime *object = (struct ObjectRuntime *)Value4((s32 (*)())Func_020041e4, 366, position[0], position[1], position[2]);

    if (object != NULL) {
        object->bounce = 0;
        object->gravity = 0;
        object->unknown_4c = -speed;
        Func_02001928(object, 3);
        object->update = Func_020017c8;
        if (flag != 0)
            object->unknown_5c[0] = 2;
        Func_020041cc(object, 2);
    }
}

s32 Func_02001a44(struct ObjectRuntime *a, struct ObjectRuntime *b)
{
    if (DIFF(a->x, b->x) < 0xc0000) {
        if ((u32)(a->y - b->y - 1) <= 0xffffe) {
            if (DIFF(a->z, b->z) < 0x80000)
                return 1;
        }
    }
    return 0;
}
