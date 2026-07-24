typedef unsigned int u32;

void Func_0800bc48(void *destination)
{
    u32 zero;
    u32 *source;
    u32 *dma;
    u32 control;

    if (destination != 0) {
        source = &zero;
        *source = 0;
        dma = (u32 *)0x040000d4;
        control = 0x85000006;
        dma[0] = (u32)source;
        dma[1] = (u32)destination;
        dma[2] = control;
    }
}
