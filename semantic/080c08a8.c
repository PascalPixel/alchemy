#include "types.h"

struct State_080c08a8 {
    u8 padding000[8];
    u32 value008;
};
struct DmaChannel_080c08a8 { const void *source; void *destination; u32 control; };

void *Func_080048f4(s32 kind, s32 size);
extern struct State_080c08a8 *Data_03001f00;

void Func_080c08a8(void)
{
    void *buffer;
    struct State_080c08a8 *state;
    u32 zero;
    u32 *zeroPtr;

    buffer = Func_080048f4(10, 0x2a0);
    state = Data_03001f00;
    {
        volatile struct DmaChannel_080c08a8 *dma;

        zeroPtr = &zero;
        *zeroPtr = 0;
        dma = (volatile struct DmaChannel_080c08a8 *)0x040000d4;
        dma->source = zeroPtr;
        dma->destination = buffer;
        dma->control = 0x850000a8;
    }
    state->value008 = 0;
}
