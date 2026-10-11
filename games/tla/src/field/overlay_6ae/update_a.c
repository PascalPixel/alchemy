#include "ov6ae.h"

void Func_02000038(struct ObjectRuntime *object)
{
    u8 *timer = &object->unknown_5c[6];

    if (*timer != 0) {
        (*timer)--;
    } else {
        *timer = (((u32)Random16Far() * 80) >> 16) + 60;
        object->angle = Random16Far();
    }
}

void Func_02000068(struct ObjectRuntime *object)
{
    if ((gFrameCount >> 1) & 1)
        Object_SetPartAttribute(object, 15);
    else
        Object_SetPartAttribute(object, 0);
}
