/* Near miss, differ 2 of 51 (one reordered pair): the native loads x (ldr
 * r3, [r1]) before it starts the affine reference register's address (movs
 * r2, #128); this form starts the address first. Measured: x in a local
 * behind a one-pass block (differ 16: the two registers trade places and the
 * second address is rebuilt, not stepped by 4), behind an empty asm barrier
 * (differ 34), behind a "+l" barrier (differ 24: the load order is the
 * native's, the x and y pointers trade r0 and r1). */
#include "types.h"
#include "ram_buffer.h"

struct AffineStepWork {
    u8 unknown_0000[0x7790];
    s32 counter;
    s32 period;
    s32 step_x;
    s32 step_y;
    u8 unknown_77a0[0x30];
    s32 x;
    s32 y;
};

void Func_0813bb38(void)
{
    struct AffineStepWork *work;

    /* FAKEMATCH: the one-pass block loads the effect work before the counter offset is started (one reordered pair, differ 2 more without it). */
    do {
        work = (struct AffineStepWork *)Ram_HeapSlots->battle_fx_work[0];
    } while (0);

    if (++work->counter == work->period) {
        *(volatile s32 *)0x04000028 = work->x;
        *(volatile s32 *)0x0400002c = work->y;
        work->x += work->step_x;
        work->y += work->step_y;
        work->counter = 0;
    }
}
