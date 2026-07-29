typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

s32 Func_08002f40(s32);

typedef struct {
    u32 source;
    u32 destination;
    u32 control;
} DmaTransfer;

void Func_0800479c(void)
{
    DmaTransfer *dma = (DmaTransfer *)0x040000D4;
    u16 *palette = (u16 *)0x050001E8;

    dma->source = Func_08002f40(0x13);
    dma->destination = 0x06000000;
    dma->control = 0x84000800;
    dma->source = 0x0800777C;
    dma->destination = 0x050001E0;
    dma->control = 0x80000010;

    *(volatile u16 *)0x05000000 = 0;
    *palette++ = 0x4180;
    *palette++ = 0x3960;
    *palette++ = 0x3140;
    *palette++ = 0x2920;
    *palette++ = 0x49A0;
    *palette++ = 0x51C0;
    *palette = 0x59E0;

    dma->source = 0x0800779C;
    dma->destination = 0x05000200;
    dma->control = 0x800000E0;
}
