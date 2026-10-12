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

    /* FAKEMATCH: the one-pass block loads the effect work before the counter offset is started (one reordered pair, differ 2 without it). */
    do {
        work = (struct AffineStepWork *)Ram_HeapSlots->battle_fx_work[0];
    } while (0);

    if (++work->counter == work->period) {
        /* FAKEMATCH: the native loads x before it starts the affine reference register's address; the plain store starts the address first (one reordered pair, differ 2). The value is pinned to the r3 it already takes: in a plain local the two registers trade places (differ 16). */
        register s32 x __asm__("r3") = work->x;

        /* FAKEMATCH: the one-pass block's loop note holds the address until the pinned value is loaded. */
        do { } while (0);
        *(volatile s32 *)0x04000028 = x;
        *(volatile s32 *)0x0400002c = work->y;
        work->x += work->step_x;
        work->y += work->step_y;
        work->counter = 0;
    }
}
