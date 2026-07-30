#include "types.h"

struct DmaTransfer_08012388 {
    const void *source;
    void *destination;
    u32 control;
};

void *Func_080048b0(s32, s32);
void Func_080072f4(s32, s32, void *, void *);
void Func_08002dd8(s32);

void Func_08012388(s32 arg0, s32 arg1)
{
    struct DmaTransfer_08012388 *dma =
        (struct DmaTransfer_08012388 *)0x040000d4;
    void *buffer = Func_080048b0(0x31, 0x27c);

    dma->source = (const void *)0x08009e7c;
    dma->destination = buffer;
    dma->control = 0x8400009f;
    Func_080072f4(arg0, arg1, (void *)0x0203c000, (void *)0x0201d000);
    Func_08002dd8(0x31);
}
