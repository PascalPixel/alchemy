#include "types.h"
#include "frame.h"
#include "object_runtime.h"

extern s32       gData_02006be4;
extern const s32 gData_02004fc8[];
void             Animation_ApplyChildValues(struct ObjectRuntime *object, s32 value);
void             Func_02003aac(struct ObjectRuntime *object);

void Func_02003b70(struct ObjectRuntime *object)
{
    /* FAKEMATCH: a volatile first read keeps the second read of the frame counter after the call; plain, it is hoisted above the branch */
    if (*(volatile u32 *)&gFrameCount & 1)
    {
        Animation_ApplyChildValues(object, gData_02004fc8[gData_02006be4]);
        gData_02006be4 = (gData_02006be4 + 1) & 3;
    }
    if (!(gFrameCount & 3))
    {
        Func_02003aac(object);
    }
}
