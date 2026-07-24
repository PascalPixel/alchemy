typedef unsigned int u32;

void Func_080c0184(void)
{
    u32 index;
    u32 *dma;
    u32 source;
    u32 destination;
    u32 control;

    index = *(*(u32 **)0x03001ef8) - 1;
    if (index <= 31)
    {
        dma = (u32 *)0x040000d4;
        source = 0x080c5a30 + (index >> 2) * 32;
        destination = 0x06005000;
        control = 0x84000008;

        dma[0] = source;
        dma[1] = destination;
        dma[2] = control;
    }
}
