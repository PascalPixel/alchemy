#include "scene.h"

void Func_02000038(struct RiseObject *object)
{
    object->scale += 0x1000;
    if (object->scale >= 0x10000) {
        object->scale = 0x10000;
        object->update = 0;
    }
    object->scaleCopy = object->scale;
}
