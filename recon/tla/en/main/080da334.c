#include "worka0.h"
#include "object_runtime.h"
#include "system.h"

extern s32 Data_080f3948;
s32 Trig_Sin(s32 angle);
void Func_080da3bc(s32 arg, s32 x1, s32 y1, s32 z1, s32 x2, s32 y2, s32 z2, s32 wave);

void Func_080da334(s32 arg, s32 first_id, s32 second_id)
{
    struct ObjectRuntime *first = ObjectTable_Get(first_id);
    struct ObjectRuntime *second = ObjectTable_Get(second_id);

    if (first != NULL && second != NULL) {
        Func_080da3bc(arg, first->x, first->y + 0x80000, first->z, second->x, second->y + 0x80000, second->z, Trig_Sin(Data_080f3948) << 2);
        Data_080f3948 += 0x1000;
        WaitFrames(6);
        Object_SetMode(first, 2);
        Object_SetMode(second, 2);
    }
}
