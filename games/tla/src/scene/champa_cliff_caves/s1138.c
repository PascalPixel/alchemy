#include "scene.h"

void Func_02001138(struct RiseObject *object)
{
    object->scale += 0xc00;
    if (object->scale >= 0x10000) {
        object->scale = 0x10000;
        object->update = 0;
    }
    object->scaleCopy = object->scale;
}
