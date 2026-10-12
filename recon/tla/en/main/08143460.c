/* Near miss, differ 2 of 20 with a barrier before each store (differ 4 without):
   the native runs in source order, as 081433b4 does. What is left is the first
   store, where the native reads the value before it starts the registers' address
   (ldr r3, [r3]; movs r1, #128). A temporary there, behind a barrier or in a block
   that runs once, moves the value to r1 and the address to r3 (differ 22). */
#include "types.h"
#include "ram_buffer.h"

void Func_08143460(void)
{
    u8 *work = Ram_HeapSlots->battle_fx_work[0];

    /* FAKEMATCH: keeps this store in source order: the native loads each value before it starts the constant that follows. */
    __asm__ volatile("");
    *(volatile u32 *)0x04000028 = *(u32 *)(work + 0x77d0);
    /* FAKEMATCH: keeps this store in source order: the native loads each value before it starts the constant that follows. */
    __asm__ volatile("");
    *(volatile u32 *)0x0400002c = *(u32 *)(work + 0x77d4);
}
