#include "worka0.h"
#include "types.h"
#include "input.h"
#include "object_runtime.h"

extern s32 gData_080f3950;

void Func_080da060(s32 a0, s32 a1, s32 a2, s32 a3);

void Func_080da968(struct ObjectRuntime *object)
{
    u8 *parts = object->animation;

    parts[22] = 253;
    parts[23] = 12;
    if (gInput.repeat & 0x200)
        gData_080f3950 = 1;
    if (gData_080f3950 != 0)
        Func_080da060(0, 12, 13, 0);
}
