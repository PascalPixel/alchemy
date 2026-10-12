/* Draft, differ 2 of 6: the native loads the halfword from the pool before
 * it starts the register address (ldr r3; movs r2, #128); this starts the
 * address first. In the sched2 dump the address's first instruction has
 * the higher priority (36 against 35). Value and address belong to the one
 * store, so no barrier fits between them. Func_081693c0 is the same. */
#include "types.h"
#include "ram_buffer.h"
#include "dma.h"
#include "io_reg.h"

void Func_081963ec(s32 slot, s32 size);

void Func_081693b0(void)
{
    *(volatile u16 *)0x04000020 = 0x100;
}
