typedef unsigned int u32;

void Func_080072e4(u32);

void Func_080bd7a4(void)
{
    u32 *dma;
    u32 *state;
    u32 source;
    u32 destination;
    u32 control;

    dma = (u32 *)0x040000d4;
    source = 0;
    destination = 0;
    control = 0x84000000;
    dma[0] = source;
    dma[1] = destination;
    dma[2] = control;

    source = 0;
    control = 0x84000000;
    dma[0] = source;
    dma[1] = destination;
    dma[2] = control;

    source = 0;
    control = 0x84000000;
    dma[0] = source;
    dma[1] = destination;
    dma[2] = control;

    state = (u32 *)0x030000c4;
    Func_080072e4(state[0]);
}
