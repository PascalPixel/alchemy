#include "types.h"
#include "object_runtime.h"
#include "ram_buffer.h"
#include "call.h"
#include "iwram_call.h"

struct MarsEntranceAnimation {
    u8 unknown_00[9];
    u8 unused_09 : 2;
    u8 layer : 2;
    u8 unknown_0a[6];
    u8 value_10;
    u8 flag_11 : 1;
    u16 angle;
};

struct MarsEntranceRideHead {
    s32 counter;
    s16 active;
    u8 unknown_06[6];
    u16 value_0c;
    s16 value_0e;
    u8 unknown_10[0x60];
    struct ObjectRuntime *handle;
};

extern u8 Data_020023c4[];

s32 GameFlag_Test(s32 flag);
void Func_02004bc0(struct ObjectRuntime *object, s32 value);
struct ObjectRuntime *Func_02004bd8(s32 kind, s32 x, s32 y, s32 z);

void Func_02002270(void)
{
    struct MarsEntranceRideHead *work = (struct MarsEntranceRideHead *)(Data_020023c4 + 0x188);
    struct ObjectRuntime *object;
    struct MarsEntranceAnimation *animation;

    if (!GameFlag_Test(0x109))
        Iwram_ClearWords(work, 116);
    object = (struct ObjectRuntime *)Value4((s32 (*)())Func_02004bd8, 365, 1, 0, 0);
    object->unknown_5c[0] = 2;
    Func_02004bc0(object, 1);
    animation = object->animation;
    work->handle = object;
    work->value_0c = animation->value_10;
    animation->flag_11 = 1;
}
