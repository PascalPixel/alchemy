#include "types.h"
#include "input.h"
#include "object_runtime.h"

extern s32 Data_080f3944;

void Func_080d8fa8(s32 a0, s32 a1, s32 a2, s32 a3);

void Func_080d9978(struct ObjectRuntime *object)
{
    u8 *parts = object->animation;

    parts[22] = 253;
    parts[23] = 12;
    if (gInput.repeat & 0x200)
        Data_080f3944 = 1;
    if (Data_080f3944 != 0)
        Func_080d8fa8(0, 12, 13, 0);
}
