#include "inventory.h"
#undef Inventory_Add
#define Inventory_Add Func_080b5a0c


extern u8 *Data_03001e74;
extern s8 Data_080c2a10[];

s32 Func_080b6a60(u16 *out);
s32 Func_080b6ae0(u16 *out);

void Inventory_Add(void)
{
    u16 buf[14];
    u8 *state;
    s32 count;
    s32 i;
    s32 offset;
    u16 *out;

    state = Data_03001e74;
    count = Func_080b6a60(buf);
    for (i = 0; i < count; i++) {
        *(u16 *)(state + 88 + i * 2) = buf[i];
    }
    offset = count * 2 + 88;
    *(u16 *)(state + offset) = 0xFF;

    count = Func_080b6ae0(buf);
    switch (state[66]) {
    case 0:
    case 1:
        out = (u16 *)(state + 2);
        for (i = 0; i < count; i++) {
            out[50 + i] = buf[i];
        }
        break;
    default:
        out = (u16 *)(state + 2);
        for (i = 0; i < count; i++) {
            out[50 + Data_080c2a10[i] + count / 2] = buf[i];
        }
        break;
    }
    out[50 + count] = 0xFF;
}
