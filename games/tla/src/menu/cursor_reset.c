#include "types.h"
#include "ram_buffer.h"

struct MenuRuntimeCursor {
    u8 unknown_00[0x24];
    s32 unknown_24;
    s32 unknown_28;
    s32 unknown_2c;
    s32 unknown_30;
    u8 unknown_34[0xc0];
    u8 unknown_f4;
    u8 unknown_f5;
    u8 unknown_f6;
    u8 unknown_f7;
};

s32 Func_080f88d0(void *work);
void Func_08104fe0(s32 value, s32 a, s32 b);

void Func_080fee04(void)
{
    struct MenuRuntimeCursor *work = Ram_HeapSlots->menu_runtime;

    Func_08104fe0(Func_080f88d0(work), 2, 2);
    work->unknown_2c = 0;
    work->unknown_28 = 0;
    work->unknown_30 = 0;
    work->unknown_24 = 0;
    work->unknown_f4 = 0;
    work->unknown_f5 = 0;
    work->unknown_f6 = 8;
    work->unknown_f7 = 2;
}
