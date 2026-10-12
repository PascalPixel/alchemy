/* Near miss, differ 2 of 36 (one reordered pair): inside the branch the
 * native loads the scroll table's address between the two halves of the
 * step offset (movs r2, #238; ldr r1, =gBgScroll; lsls r2, r2, #7); this
 * form finishes the shift first. The counter reset written last gives the
 * native registers (work in r4, the counter's address in r5, the zero in
 * r0); written between the two additions it moves all three (differ 24).
 * Measured: the table in a pointer local (same 2), behind a "+l" barrier
 * (differ 34). */
#include "types.h"
#include "ram_buffer.h"
#include "bg_scroll.h"

struct ScrollStepWork {
    u8 unknown_0000[0x7790];
    s32 counter;
    s32 period;
    s32 step_x;
    s32 step_y;
};

void Func_0813baec(void)
{
    struct ScrollStepWork *work;

    /* FAKEMATCH: the one-pass block loads the effect work before the counter offset is started (one reordered pair, differ 2 more without it). */
    do {
        work = (struct ScrollStepWork *)Ram_HeapSlots->battle_fx_work[0];
    } while (0);

    if (++work->counter == work->period) {
        gBgScroll[1].x += work->step_x;
        gBgScroll[1].y += work->step_y;
        work->counter = 0;
    }
}
