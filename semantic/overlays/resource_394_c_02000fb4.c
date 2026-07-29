typedef unsigned int u32;
typedef signed int s32;

struct DmaTransfer {
    u32 source;
    u32 destination;
    u32 control;
};

void Func_020020da(s32 size, s32 mode);
void Func_02001f4e(void);

/*
 * Restore one of the saved palettes to the active work buffer and finalize the
 * palette update.
 *
 * Owner span: 0x02000fb4-0x02000ff4.  The next word begins the veneer bank.
 */
void Func_02000fb4(s32 useAlternate)
{
    volatile struct DmaTransfer *const dma3 =
        (volatile struct DmaTransfer *)0x040000d4;
    u32 source = useAlternate ? 0x020099d0 : 0x020092d0;

    dma3->source = source;
    dma3->destination = *(volatile u32 *)0x03001ed0;
    dma3->control = 0x840000e0;

    Func_020020da(0x10000, 0);
    Func_02001f4e();
}
