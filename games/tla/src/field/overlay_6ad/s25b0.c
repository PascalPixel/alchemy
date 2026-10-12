#include "room.h"
#include "call.h"
#include "iwram_call.h"

struct Ov6adAnimation {
    u8 unknown_00[0x10];
    u8 value_10;
    u8 flag_11 : 1;
};

struct Ov6adRideWork {
    u8 unknown_00[0x0c];
    u16 value_0c;
    u8 unknown_0e[0x62];
    struct ObjectRuntime *handle;
};

extern u8 Data_020023c4[];

s32 GameFlag_Test(s32 flag);

void Func_020025b0(void)
{
    struct Ov6adRideWork *work = (struct Ov6adRideWork *)(Data_020023c4 + 0x188);
    struct ObjectRuntime *object;
    struct Ov6adAnimation *animation;

    if (!GameFlag_Test(0x109))
        Iwram_ClearWords(work, 116);
    object = (struct ObjectRuntime *)Value4((s32 (*)())Func_02005498, 365, 1, 0, 0);
    object->unknown_5c[0] = 2;
    Func_02005480(object, 1);
    animation = object->animation;
    work->handle = object;
    work->value_0c = animation->value_10;
    animation->flag_11 = 1;
}
