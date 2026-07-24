typedef unsigned int u32;

void Func_08004858(void)
{
    u32 zero;
    u32 *source;
    u32 *state;
    u32 *dma;

    state = (u32 *)0x03001e50;
    source = &zero;
    *source = 0;
    dma = (u32 *)0x040000d4;
    dma[0] = (u32)source;
    dma[1] = (u32)state;
    dma[2] = 0x85000040;
    state[1] = 0x03002000;
    state[0] = 0x02030000;
}
