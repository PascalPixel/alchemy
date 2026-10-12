/* Near miss, differ 4 of 20: the native loads the work pointer before it starts
   the 0x77d0 offset and reads the value before it starts the register address
   (ldr r2, [r3, #92]; movs r0, #238 ... ldr r3, [r3]; movs r1, #128); this
   draft starts each constant first. Barriers at the two spots break the
   shared register address (adds r1, #4) and are further away (differ 22). */
#include "types.h"
#include "ram_buffer.h"

void Func_08143460(void)
{
    u8 *work = Ram_HeapSlots->battle_fx_work[0];

    *(volatile u32 *)0x04000028 = *(u32 *)(work + 0x77d0);
    *(volatile u32 *)0x0400002c = *(u32 *)(work + 0x77d4);
}
