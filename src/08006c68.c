typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

u8 *Func_08006c24(u8 *, u8 *, u16);
s32 Func_080072f0(s32, s32, s32, s32);

extern u8 Data_08007abc[];

s32 Func_08006c68(u16 sector, s32 arg1)
{
    u16 buffer[128];
    u16 *source;
    u16 *destination;
    u16 count;
    u32 wait;
    s32 core;
    u32 shifted;
    u32 address;

    wait = *(volatile u16 *)0x04000204;
    wait &= 0xFFFC;
    wait |= 3;
    *(volatile u16 *)0x04000204 = wait;

    source = (u16 *)Func_08006c24;
    source = (u16 *)((u32)source ^ 1);
    destination = buffer;
    count = ((u32)Func_08006c68 - (u32)Func_08006c24) / 2;

    while (count != 0) {
        *destination = *source;
        source++;
        destination++;
        count--;
    }

    core = (s32)((u8 *)buffer + 1);
    shifted = sector << Data_08007abc[28];
    address = shifted + 0x0E000000;
    return Func_080072f0(
        arg1,
        address,
        *(u16 *)(Data_08007abc + 24),
        core);
}
