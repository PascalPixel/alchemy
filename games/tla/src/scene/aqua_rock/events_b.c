#include "local.h"

void Func_02000b10(struct ObjectRuntime *object)
{
    if (gFrameCount & 2)
        Animation_ApplyChildValues(object, 7);
    else
        Animation_ApplyChildValues(object, 0);
}

void Func_02000b34(struct ObjectRuntime *object)
{
    if (gFrameCount & 2)
        Animation_ApplyChildValues(object, 6);
    else
        Animation_ApplyChildValues(object, 0);
}

void Func_02000b58(struct ObjectRuntime *object)
{
    Animation_ApplyChildValues(object, 0);
    object->update = 0;
}
