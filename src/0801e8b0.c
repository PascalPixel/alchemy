typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

struct Work_0801e8b0 {
    u8 pad_00[12];
    u16 x;
    u16 y;
};

extern u8 *Data_03001e8c;

s32 Func_08002df0(s16 *);
s16 *Func_08004970(s32);
s32 Func_0801de5c(s16 *, s32, s32, s32);

void Func_0801e8b0(
    u8 *text,
    struct Work_0801e8b0 *work,
    s32 offset_x,
    s32 offset_y)
{
    s16 *buffer;
    s16 *output;
    u8 *base;
    u32 cell;
    s32 vram;
    s32 source_address;
    s32 destination_address;
    s32 phase;

    buffer = Func_08004970(0x200);
    base = Data_03001e8c;
    output = buffer;
    if (*text != 0) {
        do {
            *output = *text;
            text++;
            output++;
        } while (*text != 0);
    }
    *output = 0;

    cell = ((work->y + ((u32)offset_y >> 3) + 1) << 5)
        + (work->x + ((u32)offset_x >> 3)) + 1;
    if (cell < 0x280) {
        vram = 0x06002000;
        cell *= 2;
        destination_address = vram + cell;
        source_address = (s32)base + cell;
        phase = offset_x & 7;
        Func_0801de5c(
            buffer,
            source_address,
            destination_address,
            phase);
        Func_08002df0(buffer);
    }
}
