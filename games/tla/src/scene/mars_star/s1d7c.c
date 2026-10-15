#include "mars_star.h"
#include "iwram_call.h"

struct RideAnimation {
    u8 unk00[0x10];
    u8 value10;
    u8 flag11 : 1;
};

struct RideWork {
    u8 unk00[0xc];
    u16 value0c;
    u8 unk0e[0x62];
    struct ObjectRuntime *handle;
};

extern u8 gData_020023c4[];

void Func_02001d7c(void)
{
    struct RideWork *work = (struct RideWork *)(gData_020023c4 + 0x188);
    struct ObjectRuntime *object;
    struct RideAnimation *animation;

    if (!GameFlag_Test(0x109))
        Iwram_ClearWords(work, 116);
    object = (struct ObjectRuntime *)Value4((s32 (*)())Func_020041e4, 365, 1, 0, 0);
    object->unk5c[0] = 2;
    Func_020041cc(object, 1);
    animation = object->animation;
    work->handle = object;
    work->value0c = animation->value10;
    animation->flag11 = 1;
}
