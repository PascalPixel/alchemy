#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "DMA.H"
#include "IO_REG.H"

s32 BattlePresentation_InitializeTilePattern(void)
{
    Dma_Set((const void *)0x06000290, (void *)0x06000280, 0x80000008,
            REG_DMA3);
    return Iwram_ClearWords((void *)0x0600028c, 20);
}

void BattlePresentation_ClearBg0VerticalOffset(void)
{
    u32 zero = 0;

    REG_BG0VOFS = zero;
}

void Party_ReservedNoOp5B14(void)
{
}
