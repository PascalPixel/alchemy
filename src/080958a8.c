typedef signed int s32;
typedef unsigned int u32;

struct DmaTransfer {
    const void *source;
    void *destination;
    u32 control;
};

void *Func_080048b0(s32, s32);
void Func_080041d8(const void *, s32);

void Func_080958a8(void)
{
    u32 zero;
    u32 *source;
    void *destination;
    struct DmaTransfer *dma;

    destination = Func_080048b0(56, 0x720);
    source = &zero;
    *source = 0;
    dma = (struct DmaTransfer *)0x040000d4;
    dma->source = source;
    dma->destination = destination;
    dma->control = 0x850001c8;
    Func_080041d8((const void *)0x08095885, 0xc80);
}
