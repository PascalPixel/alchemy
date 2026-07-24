typedef unsigned int u32;

void Func_08004838(void)
{
    u32 *dma = (u32 *)0x040000d4;
    u32 source = 0x0800779c;
    u32 destination = 0x05000200;
    u32 control = 0x800000e0;

    dma[0] = source;
    dma[1] = destination;
    dma[2] = control;
}
