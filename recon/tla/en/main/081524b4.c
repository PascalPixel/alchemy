#include "TYPES.H"
#include "RAM_BUFFER.H"
#include "DMA.H"
#include "IO_REG.H"

void Func_081963ec(s32 slot, s32 size);

void Func_081524b4(void)
{
    u8 *work = Ram_HeapSlots->unknown_4c[3];
    volatile u16 *channel = (volatile u16 *)REG_DMA0;

    channel[5] &= 0xc5ff;
    channel[5] &= 0x7fff;
    (void)channel[5];
    Dma_Set(work + 0x7080, (void *)0x04000014, 0xa6600001, (volatile u32 *)channel);
}
