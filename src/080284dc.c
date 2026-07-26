#include "types.h"

struct DmaTransfer {
    const void *source;
    void *destination;
    u32 control;
};

void *Func_080048f4(s32 arg0, s32 arg1);
void Func_080041d8(const void *, s32);

void *Func_080284dc(void)
{
    u32 zero;
    u32 *source;
    void *destination;
    struct DmaTransfer *dma;
    s32 line;

    destination = Func_080048f4(58, 0x98);
    source = &zero;
    *source = 0;
    dma = (struct DmaTransfer *)0x040000d4;
    dma->source = source;
    dma->destination = destination;
    dma->control = 0x85000026;
    line = 0xc76;
    Func_080041d8((const void *)0x08028195, line);
    return destination;
}
