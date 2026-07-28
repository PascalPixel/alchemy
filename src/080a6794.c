#include "types.h"

struct MenuRuntime {
    u8 padding_000[0x20];
    s32 handle;
    u8 padding_024[0x44 - 0x24];
    u8 *object;
    void *entries[16];
    u8 padding_088[0x110 - 0x88];
    u8 flag_110;
    u8 flag_111;
    u8 flag_112;
    u8 flag_113;
};

s32 Func_080a1814(void *);
s32 Func_080a1870(s32, s32, s32, s32, s32);
s32 Func_08015010(s32, s32, s32, s32, s32);
u8 *Func_080a1778(s32, s32, s32);
void *Func_080150d8(s32, s32, s32, s32, s32, s32);
void *Func_080150d0(s32, s32, s32, s32, s32);

void Func_080a6794(void)
{
    s32 handle;
    struct MenuRuntime *runtime;
    u8 *object;
    s32 index;
    s32 coordinate;
    s32 row;
    void **output;

    runtime = *(struct MenuRuntime **)0x03001f2c;
    handle = Func_080a1814(runtime);
    Func_080a1870(handle, 2, 2, 8, 0);

    handle = Func_08015010(0, 5, 30, 15, 2);
    runtime->handle = handle;
    runtime->flag_110 = 0;
    runtime->flag_111 = 0;
    runtime->flag_112 = 8;
    runtime->flag_113 = 2;

    object = Func_080a1778(handle, 0, 4);
    object[5] = 13;
    runtime->object = object;
    Func_080150d8(0, 0, 0, handle, 0, 0);

    row = 8;
    index = 0;
    output = &runtime->entries[0];
    coordinate = 96;
    do {
        *output++ = Func_080150d0(4, index, handle, coordinate, row);
        index++;
        coordinate += 16;
    } while (index <= 7);

    index = 8;
    row = 24;
    output = &runtime->entries[8];
    coordinate = 96;
    do {
        *output++ = Func_080150d0(4, index, handle, coordinate, row);
        index++;
        coordinate += 16;
    } while (index <= 15);
}
