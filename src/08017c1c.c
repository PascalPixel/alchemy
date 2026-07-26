#include "types.h"

struct Work_08017c1c {
    u8 pad_00[12];
    u16 x;
    u16 y;
};

extern u8 *Data_03001e8c;

s32 Func_0801de5c(u16 *, s32, s32, s32);

void Func_08017c1c(
    u16 *buffer,
    struct Work_08017c1c *work,
    s32 offset_x,
    s32 offset_y)
{
    u8 *base = Data_03001e8c;
    s32 index;
    u32 cell;

    if (buffer == NULL) {
        u16 *counter = (u16 *)(base + 0x12B2);

        index = *counter * 2 + 0xEB0;
        buffer = (u16 *)(base + 0xEB0);
        *(u16 *)(base + index) = 0;
        *counter = (*counter + 1) & 0x1FF;
    }

    cell = ((work->y + offset_y + 1) << 5)
        + (work->x + offset_x) + 1;
    if (cell < 0x280) {
        s32 source_address;
        s32 destination_address;

        cell *= 2;
        destination_address = 0x06002000 + cell;
        source_address = (s32)base + cell;

        Func_0801de5c(
            buffer,
            source_address,
            destination_address,
            0x06002000);
    }
}
