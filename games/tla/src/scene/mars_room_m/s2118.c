#include "mars_room_m.h"

void Func_02002118(void)
{
    struct MarsRoomMRideWork *work = (struct MarsRoomMRideWork *)(Data_020023c4 + 0x188);
    struct ObjectRuntime *object;
    struct MarsRoomMAnimation *animation;

    if (!GameFlag_Test(0x109))
        Iwram_ClearWords(work, 116);
    object = (struct ObjectRuntime *)Value4((s32 (*)())Func_02003128, 365, 1, 0, 0);
    object->unk5c[0] = 2;
    Func_02003118(object, 1);
    animation = object->animation;
    work->handle = object;
    work->value0c = animation->value10;
    animation->flag11 = 1;
}
