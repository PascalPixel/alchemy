typedef signed int s32;
typedef unsigned int u32;

struct DmaTransfer {
    const void *source;
    void *destination;
    u32 control;
};

void *Func_080048f4(s32, s32);
void Func_080041d8(const void *, s32);

void Func_0801d980(void)
{
    u32 zero;
    u32 *source;
    void *destination;
    struct DmaTransfer *dma;

    destination = Func_080048f4(20, 0x628);
    source = &zero;
    *source = 0;
    dma = (struct DmaTransfer *)0x040000d4;
    dma->source = source;
    dma->destination = destination;
    dma->control = 0x8500018a;
    Func_080041d8((const void *)0x0801d94d, 0xc80);
}
