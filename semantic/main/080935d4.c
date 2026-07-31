#include "types.h"

/*
 * One step of a scripted interpolation held in the progress block at
 * 0x03001e70.  It runs only while the record hanging off resource 0xccc has
 * a zero busy byte at +0x5b and while the step count at +0x358 is non-zero.
 * Each call advances the step index at +0x35a, takes that many steps' share
 * of the span from +0x350 to +0x354, scales the result by the factor at
 * +0x348 and writes it to +0x34c, then republishes the block's frame stamp
 * at 0x03001af4.  When the step index has caught up with the step count the
 * run is over: the count is cleared and this function's own Thumb pointer,
 * 0x080935d5, is handed to Func_08004278, which drops that entry from the
 * 20-entry callback table at 0x03001a20.
 *
 * `ldr r4,[pc,#80]` loads 0x03000118 - the ARM fixed-point multiply
 * relocated into IWRAM - and `mov ip, pc ; bx r4` calls it: mov ip,pc sets
 * the return address to the halfword after the bx, so control resumes in the
 * same frame with every live register intact.  It is an ordinary indirect
 * call, the shape already resolved at 0x0800ebec, and is written below as a
 * call through a function pointer.
 *
 * `bl Func_080022ec` is the runtime veneer to the IWRAM signed divide at
 * 0x03000380, so the share is spelled as a division.
 *
 * Reached only as a published Thumb pointer, at 0x0809369c - its own pool
 * word, the argument to Func_08004278 - and at 0x08093708, the table entry
 * that installs it.
 */

/* 0x03000118, the IWRAM-relocated ARM fixed-point multiply. */
typedef s32 (*Multiply_080935d4)(s32 a, s32 b);

/* Progress block at 0x03001e70; only the fields this owner uses are named. */
struct Progress_080935d4 {
    u8 unknown_000[0x118];
    u16 frame_118;
    u8 unknown_11a[0x22e];
    s32 factor_348;
    s32 result_34c;
    s32 start_350;
    s32 end_354;
    s16 steps_358;
    s16 step_35a;
};

/* Record hanging off the resource; +0x5b is its busy byte. */
struct Record_080935d4 {
    u8 unknown_00[0x5b];
    u8 busy_5b;
};

s32 Func_080022ec(s32 numerator, s32 denominator);
s32 Func_08004278(u32 handler);
void *Func_080048f4(s32 kind, s32 id);

void Func_080935d4(void)
{
    Multiply_080935d4 multiply = (Multiply_080935d4)0x03000118;
    struct Progress_080935d4 *progress =
        *(struct Progress_080935d4 **)0x03001e70;
    void **resource = (void **)Func_080048f4(27, 0xccc);
    struct Record_080935d4 *record =
        (struct Record_080935d4 *)resource[0x1e0 / 4];
    s32 span;
    s32 step;

    if (record->busy_5b != 0 || progress->steps_358 == 0) {
        return;
    }

    span = progress->end_354 - progress->start_350;
    progress->step_35a++;
    step = progress->step_35a;
    progress->result_34c = multiply(
        progress->factor_348,
        progress->start_350 + Func_080022ec(step * span,
                                            progress->steps_358));
    *(u32 *)0x03001af4 = progress->frame_118 + 1;

    if (progress->step_35a != progress->steps_358) {
        return;
    }

    /* The busy byte is still live in sl here, and is zero on this path. */
    progress->steps_358 = record->busy_5b;
    Func_08004278((u32)Func_080935d4);
}
