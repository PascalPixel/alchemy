/* Near miss, differ 2 of 30 with a barrier before each store (differ 8 without):
   as in 08143460.c and 081433b4.c the native runs in source order. What is left
   is the first store, where the native reads the value before it starts the
   registers' address (ldrh r3, [r3]; movs r1, #128). */
#include "types.h"
#include "ram_buffer.h"

void Func_08143424(void)
{
    u8 *work = Ram_HeapSlots->battle_fx_work[0];

    /* FAKEMATCH: keeps this store in source order: the native loads each value before it starts the constant that follows. */
    __asm__ volatile("");
    *(volatile u16 *)0x04000020 = *(u16 *)(work + 0x77d8);
    /* FAKEMATCH: keeps this store in source order: the native loads each value before it starts the constant that follows. */
    __asm__ volatile("");
    *(volatile u16 *)0x04000026 = *(u16 *)(work + 0x77da);
    /* FAKEMATCH: keeps this store in source order: the native loads each value before it starts the constant that follows. */
    __asm__ volatile("");
    *(volatile u32 *)0x04000028 = *(u32 *)(work + 0x77d0);
    /* FAKEMATCH: keeps this store in source order: the native loads each value before it starts the constant that follows. */
    __asm__ volatile("");
    *(volatile u32 *)0x0400002c = *(u32 *)(work + 0x77d4);
}
