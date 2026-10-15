#include "types.h"
#include "frame.h"
#include "object_runtime.h"

void Animation_ApplyChildValues(struct ObjectRuntime *object, s32 value);
void Func_02004634(struct ObjectRuntime *object);

void Func_02004500(struct ObjectRuntime *object)
{
    if ((gFrameCount & 2) != 0)
    {
        Animation_ApplyChildValues(object, 7);
    }
    else
    {
        Animation_ApplyChildValues(object, 0);
    }
    if ((gFrameCount & 15) == 0)
    {
        Func_02004634(object);
    }
}
