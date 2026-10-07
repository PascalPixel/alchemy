#include "TYPES.H"
#include "RAM_BUFFER.H"
#include "DMA.H"
#include "IO_REG.H"

void Func_081963ec(s32 slot, s32 size);

void Func_081693c0(void)
{
    *(volatile u16 *)0x04000020 = 0x80;
}
