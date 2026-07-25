typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

extern void Func_08006b84(u32 source, u8 *destination, s32 size);
extern s32 Func_080072f0(u8 *source, u8 *destination, s32 size, void *core);
extern u8 Data_08007abc[];

void Func_08006ba8(u16 sectorNum, u32 offset, u8 *destination, s32 size)
{
    u16 buffer[64];
    u16 *source;
    u16 *cursor;
    u16 count;
    void *core;
    u32 shifted;
    u32 address;

    *(volatile u16 *)0x04000204 = (*(volatile u16 *)0x04000204 & 0xFFFC) | 3;

    source = (u16 *)Func_08006b84;
    source = (u16 *)((u32)source ^ 1);
    cursor = buffer;
    count = ((u32)Func_08006ba8 - (u32)Func_08006b84) / 2;

    while (count != 0) {
        *cursor = *source;
        source++;
        cursor++;
        count--;
    }

    core = (u8 *)buffer + 1;
    shifted = sectorNum << Data_08007abc[28];
    address = offset + 0x0E000000;
    Func_080072f0((u8 *)(shifted + address), destination, size, core);
}
