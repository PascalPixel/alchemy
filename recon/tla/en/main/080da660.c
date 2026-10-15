/* Near miss, differ 8 of 57: scheduling only. The native opens the frame and
 * saves the third argument before it starts the heap slot address, and the
 * argument setup of Func_080da6e0 is ordered as in Func_080da334. */
#include "worka0.h"
#include "object_runtime.h"
#include "system.h"

extern s32 gData_080f394c;
s32 Trig_Sin(s32 angle);
void Func_080da6e0(s32 arg, s32 x1, s32 y1, s32 z1, s32 x2, s32 y2, s32 z2, s32 wave);

void Func_080da660(s32 arg, s32 firstId, s32 secondId)
{
    struct ObjectRuntime **work = (struct ObjectRuntime **)WORK_A0;
    struct ObjectRuntime *first = ObjectTable_Get(firstId);
    struct ObjectRuntime *second = ObjectTable_Get(secondId);

    if (first != NULL && second != NULL) {
        work[0] = first;
        work[1] = second;
        Func_080da6e0(arg, first->x, first->y + 0x80000, first->z, second->x, second->y + 0x80000, second->z, Trig_Sin(gData_080f394c) << 2);
        gData_080f394c += 0x1000;
    }
}
