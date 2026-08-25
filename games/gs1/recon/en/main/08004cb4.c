#include "types.h"

#define SceneTransform_ApplyPosition Func_08004cb4

void SceneTransform_ApplyPosition(const s32 *position)
{
    s32 transform[12];

    transform[0] = 0x10000;
    transform[1] = 0;
    transform[2] = 0;
    transform[3] = 0;
    transform[4] = 0x10000;
    transform[5] = 0;
    transform[6] = 0;
    transform[7] = 0;
    transform[8] = 0x10000;
    transform[9] = position[0];
    transform[10] = position[1];
    transform[11] = position[2];
    Func_080072f0(transform, 0x10000, 0, 0x030002c0);
}
