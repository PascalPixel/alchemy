#include "types.h"
#include "frame.h"
#include "object_runtime.h"

void Animation_ApplyChildValues(struct ObjectRuntime *object, s32 value);

void Func_02001a0c(struct ObjectRuntime *object)
{
    u32 frame = gFrameCount;

    if (frame & 1)
        Animation_ApplyChildValues(object, (frame >> 1) % 6);
}
