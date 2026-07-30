typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

void Func_08006ba8(u16, s32, void *, s32);
u16 Func_08005ae0(void);

s32 Func_080058ac(u16 index)
{
    u16 buffer[8];
    u8 *entry;
    u32 *dma;
    u32 source;
    u32 destination;
    u32 control;
    volatile u32 *status;

    entry = *(u8 **)0x03001f1c + 64;
    Func_08006ba8(index, 0, entry, 4096);
    dma = (u32 *)0x040000d4;
    source = (u32)entry;
    destination = (u32)buffer;
    control = 0x84000004;
    dma[0] = source;
    dma[1] = destination;
    dma[2] = control;
    status = (volatile u32 *)0x040000d4;
    while ((status[2] & 0x80000000) != 0) {
    }
    return Func_08005ae0() - buffer[4];
}
