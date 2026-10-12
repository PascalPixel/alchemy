/* Near miss, differ 8 of 30: as in 08143460.c, the native loads the work
   pointer and each value before it starts the constant that follows, and
   steps the shared offset (adds r0, #2; subs r0, #10) after each store; this
   draft starts the constants first and steps before the stores. */
#include "types.h"
#include "ram_buffer.h"

void Func_08143424(void)
{
    u8 *work = Ram_HeapSlots->battle_fx_work[0];

    *(volatile u16 *)0x04000020 = *(u16 *)(work + 0x77d8);
    *(volatile u16 *)0x04000026 = *(u16 *)(work + 0x77da);
    *(volatile u32 *)0x04000028 = *(u32 *)(work + 0x77d0);
    *(volatile u32 *)0x0400002c = *(u32 *)(work + 0x77d4);
}
