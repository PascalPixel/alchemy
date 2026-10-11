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
