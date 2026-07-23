typedef unsigned int u32;

void Func_08004a28(u32 destination)
{
    u32 *dma = (u32 *)0x040000d4;

    dma[0] = 0x03000350;
    dma[1] = destination;
    dma[2] = 0x8400000c;
}
