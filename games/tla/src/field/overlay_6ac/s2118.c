#include "ov6ac.h"

void Func_02002118(void)
{
    struct Ov6acRideWork *work = (struct Ov6acRideWork *)(Data_020023c4 + 0x188);
    struct ObjectRuntime *object;
    struct Ov6acAnimation *animation;

    if (!GameFlag_Test(0x109))
        Iwram_ClearWords(work, 116);
    object = (struct ObjectRuntime *)Value4((s32 (*)())Func_02003128, 365, 1, 0, 0);
    object->unknown_5c[0] = 2;
    Func_02003118(object, 1);
    animation = object->animation;
    work->handle = object;
    work->value_0c = animation->value_10;
    animation->flag_11 = 1;
}
