typedef unsigned int u32;
typedef signed int s32;

struct DmaTransfer {
    u32 source;
    u32 destination;
    u32 control;
};

extern void Func_0808a338(s32 value, s32 mode);

/* Capture both hardware-palette banks into the work buffer, then publish it. */
void Func_02001878(void)
{
    volatile struct DmaTransfer *const dma3 =
        (volatile struct DmaTransfer *)0x040000d4;
    u32 destination = *(volatile u32 *)0x03001ed0;

    dma3->source = 0x05000000;
    dma3->destination = destination;
    dma3->control = 0x84000070;

    dma3->source = 0x05000200;
    dma3->destination = destination + 0x1c0;
    dma3->control = 0x84000070;

    Func_0808a338(0x10000, 0);
}
