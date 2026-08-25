#include "types.h"

typedef struct DmaTransfer {
    const void *source;
    void *destination;
    u32 control;
} DmaTransfer;

void *Func_080048f4(s32 kind, s32 size);

void *Func_0808fecc(void)
{
    u32 clear_value = 0;
    void *destination = Func_080048f4(31, 0x540);
    volatile DmaTransfer *dma = (volatile DmaTransfer *)0x040000d4;

    dma->source = &clear_value;
    dma->destination = destination;
    dma->control = 0x85000150;
    return destination;
}
