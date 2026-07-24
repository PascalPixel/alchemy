typedef unsigned char u8;
typedef unsigned int u32;

extern u8 Data_08000404[];

u32 Func_080037d4(u32 index)
{
    u32 offset;
    u32 *dma;
    u32 source;
    u32 destination;
    u32 control;

    if (index > 4) index = 0;
    offset = index * 152;
    dma = (u32 *)0x040000d4;
    source = (u32)Data_08000404;
    source = offset + source;
    destination = 0x03000bd8;
    control = 0x84000026;
    dma[0] = source;
    dma[1] = destination;
    dma[2] = control;
    return 0;
}

void Func_08003808(void)
{
}
