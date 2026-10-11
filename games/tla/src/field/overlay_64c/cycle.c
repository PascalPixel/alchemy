#include "types.h"
#include "frame.h"
#include "object_runtime.h"

void Animation_ApplyChildValues(struct ObjectRuntime *object, s32 value);
void Func_02001ad0(struct ObjectRuntime *object);

void Func_0200199c(struct ObjectRuntime *object)
{
    if ((gFrameCount & 2) != 0) {
        Animation_ApplyChildValues(object, 7);
    } else {
        Animation_ApplyChildValues(object, 0);
    }
    if ((gFrameCount & 15) == 0) {
        Func_02001ad0(object);
    }
}

void Func_020019d4(struct ObjectRuntime *object)
{
    u32 frame = gFrameCount;

    if (frame & 1)
        Animation_ApplyChildValues(object, (frame >> 1) % 6);
    /* FAKEMATCH: the native reads the counter again into r3 on both paths; a plain read keeps the first one in r0 and branches past the reload */
    if ((*(volatile u32 *)&gFrameCount & 15) == 0)
        Func_02001ad0(object);
}

void Func_02001a0c(struct ObjectRuntime *object)
{
    u32 frame = gFrameCount;

    if (frame & 1)
        Animation_ApplyChildValues(object, (frame >> 1) % 6);
}
