#include "local.h"

void Func_02000da4(struct ObjectRuntime *object)
{
    if (gFrameCount & object->unk5c[7])
        Object_SetPartAttribute(object, 10);
    else
        Object_SetPartAttribute(object, 0);
}
