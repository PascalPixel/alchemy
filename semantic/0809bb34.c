#include "effect_0809b11c.h"

void Func_08009038(void *);

void Func_0809bb34(struct EffectSlot *effect)
{
    void *object;
    u32 zero;
    u32 *source;
    u32 *dma;

    object = effect->object;
    if (object != 0)
        Func_08009038(object);

    source = &zero;
    *source = 0;
    dma = (u32 *)0x040000d4;
    dma[0] = (u32)source;
    dma[1] = (u32)effect;
    dma[2] = 0x85000012;
}
