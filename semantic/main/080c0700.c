typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

void Func_080c1724(u8 *, u32, s32, s32);

s32 Func_080c0700(s32 unused, s32 index)
{
    u8 *base = *(u8 **)0x03001e74;
    u8 *entry = base + 0x544;
    volatile u16 *interruptMaster;
    u32 savedInterruptMaster;
    u32 *slot;
    u32 *dma;
    u32 source;
    u32 destination;
    u32 control;

    interruptMaster = (volatile u16 *)0x04000208;
    slot = &savedInterruptMaster;
    *slot = *interruptMaster;
    *interruptMaster = (u16)(u32)interruptMaster;
    if (index == 0) {
        dma = (u32 *)0x040000d4;
        source = (u32)entry;
        destination = 0x050000c0;
        control = 0x80000080;
        dma[0] = source;
        dma[1] = destination;
        dma[2] = control;
    } else {
        s32 offset = 0x10000 - index * 1092;

        *(u32 *)(base + 0x644) = offset;
        Func_080c1724(entry, 0x050000c0, offset, 128);
    }
    *interruptMaster = *slot;
    return 0;
}
