#include "types.h"
#include "ram_buffer.h"
#include "dma.h"
#include "io_reg.h"

void Func_081963ec(s32 slot, s32 size);

void Func_081693c0(void)
{
    *(volatile u16 *)0x04000020 = 0x80;
}
