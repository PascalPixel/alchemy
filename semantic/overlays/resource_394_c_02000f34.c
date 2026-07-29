typedef unsigned int u32;

struct DmaTransfer {
    u32 source;
    u32 destination;
    u32 control;
};

/*
 * Copy 224 words from the active work buffer into the first saved palette.
 *
 * Owner span: 0x02000f34-0x02000f54, including its three-word literal pool.
 */
void Func_02000f34(void)
{
    volatile struct DmaTransfer *const dma3 =
        (volatile struct DmaTransfer *)0x040000d4;

    dma3->source = *(volatile u32 *)0x03001ed0;
    dma3->destination = 0x020092d0;
    dma3->control = 0x840000e0;
}
