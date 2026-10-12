#include "types.h"
#include "ram_buffer.h"
#include "dma.h"
#include "io_reg.h"

extern const u8 Data_0812cc74[];

void Func_08125c5c(void)
{
    u32 index = *(s32 *)Ram_HeapSlots->unknown_84[9] - 1;

    if (index <= 31)
        Dma_Set(Data_0812cc74 + (index >> 2) * 32, (void *)0x06005000, 0x84000008, REG_DMA3);
}
