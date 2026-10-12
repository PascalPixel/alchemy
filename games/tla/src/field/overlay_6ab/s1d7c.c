#include "ov6ab.h"
#include "iwram_call.h"

struct RideAnimation {
    u8 unknown_00[0x10];
    u8 value_10;
    u8 flag_11 : 1;
};

struct RideWork {
    u8 unknown_00[0xc];
    u16 value_0c;
    u8 unknown_0e[0x62];
    struct ObjectRuntime *handle;
};

extern u8 Data_020023c4[];

void Func_02001d7c(void)
{
    struct RideWork *work = (struct RideWork *)(Data_020023c4 + 0x188);
    struct ObjectRuntime *object;
    struct RideAnimation *animation;

    if (!GameFlag_Test(0x109))
        Iwram_ClearWords(work, 116);
    object = (struct ObjectRuntime *)Value4((s32 (*)())Func_020041e4, 365, 1, 0, 0);
    object->unknown_5c[0] = 2;
    Func_020041cc(object, 1);
    animation = object->animation;
    work->handle = object;
    work->value_0c = animation->value_10;
    animation->flag_11 = 1;
}
