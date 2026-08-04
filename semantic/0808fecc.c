#include "types.h"

struct DmaTransfer_0808fecc {
    const void *source;
    void *destination;
    u32 control;
};

void *Func_080048f4(s32 kind, s32 size);

void *Func_0808fecc(void)
{
    u32 zero;
    u32 *source;
    void *destination;
    struct DmaTransfer_0808fecc *dma;

    destination = Func_080048f4(31, 0x540);
    source = &zero;
    *source = 0;
    dma = (struct DmaTransfer_0808fecc *)0x040000d4;
    dma->source = source;
    dma->destination = destination;
    dma->control = 0x85000150;
    return destination;
}
