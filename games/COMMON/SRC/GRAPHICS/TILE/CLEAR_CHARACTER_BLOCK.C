#include "DMA.H"
#include "IO_REG.H"
#include "TYPES.H"

void Graphics_ClearCharacterBlockAndPalette(s32 alternate)
{
    u32 value;
    u32 vram;
    u32 palette;
    volatile u32 fill;

    if (alternate == 0) {
        value = 0x01010101;
        vram = 0x06000000;
        palette = 0x05000000;
    } else {
        value = 0x81818181;
        vram = 0x06008000;
        palette = 0x05000100;
    }
    fill = value;
    Dma_Set((const void *)&fill, (void *)vram, 0x85001e00, REG_DMA3);
    fill = 0;
    Dma_Set((const void *)&fill, (void *)palette, 0x85000040, REG_DMA3);
}
