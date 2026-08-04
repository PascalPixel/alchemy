#include "types.h"

struct DmaTransfer_08097868 {
    const void *source;
    void *destination;
    u32 control;
};

struct EffectState_08097868 {
    u8 data[0x28a];
    u8 frame;
    u8 padding[9];
    u8 inactive;
};

extern struct EffectState_08097868 *Data_03001ea8;

void Func_08097868(void)
{
    struct EffectState_08097868 *state = Data_03001ea8;

    if (state->inactive == 0) {
        volatile u32 *dma = (volatile u32 *)0x040000b0;
        u32 source;
        u32 destination;
        u32 dmaControl;
        volatile u16 *control = (volatile u16 *)0x040000ba;

        source = (u32)((u8 *)state + state->frame * 0x144);
        destination = 0x04000010;
        dmaControl = 0xa2600001;
        *control &= 0xc5ff;
        *control &= 0x7fff;
        dma[0] = source;
        dma[1] = destination;
        dma[2] = dmaControl;
    }
}
