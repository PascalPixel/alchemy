typedef unsigned int u32;

void Func_08004a44(u32 source)
{
    u32 *dma = (u32 *)0x040000d4;
    u32 destination = 0x03000350;
    u32 control = 0x8400000c;

    dma[0] = source;
    dma[1] = destination;
    dma[2] = control;
}
