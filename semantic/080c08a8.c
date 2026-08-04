#include "types.h"

struct State_080c08a8 {
    u8 padding000[8];
    u32 value008;
};

void *Func_080048f4(s32 kind, s32 size);
extern struct State_080c08a8 *Data_03001f00;

void Func_080c08a8(void)
{
    void *buffer;
    struct State_080c08a8 *state;
    u32 zero;
    volatile u32 *dma;
    u32 source;
    u32 destination;
    u32 control;

    buffer = Func_080048f4(10, 0x2a0);
    state = Data_03001f00;
    zero = 0;

    dma = (volatile u32 *)0x040000d4;
    source = (u32)&zero;
    destination = (u32)buffer;
    control = 0x850000a8;
    dma[0] = source;
    dma[1] = destination;
    dma[2] = control;

    state->value008 = 0;
}
