#include "TYPES.H"
#include "DMA.H"
#include "IO_REG.H"

/* Fills count halfwords with value by DMA and returns the end of the run. */
u16 *Memory_FillHalfwordsDma(u16 *destination, s32 value, s32 count)
{
    volatile u16 fill;
    if (count > 0) {
        fill = value;
        Dma_Set(&fill, destination, count | 0x81000000, REG_DMA3);
        destination += count;
    }
    return destination;
}
