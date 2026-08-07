#include "types.h"

/*
 * Resource 37f, owner at 0x02000154 (164 bytes of code, prologue
 * `push {r5, r6, lr} / sub sp, #8` at 0x02000154 through the interworking
 * return `add sp, #8 / pop {r5, r6} / pop {r0} / bx r0` at 0x020001f0, plus a
 * two-word literal pool at 0x020001f8-0x020001ff).  r0 is the popped return
 * address, so the owner is `void`.  Straight-line: no branches at all.
 *
 * All 18 call sites were resolved with `tools/lib/overlay_call_targets.ts` (an
 * overlay `bl` stores target_offset - 2, so the disassembler's annotations are
 * wrong); all 18 are import veneers, and 18 matches the inventory's `calls`
 * field for this row.
 *
 * The two pool words are plain constants, 0x9999 and 0x4ccc.  0x4ccc happens
 * to decode as `ldr r4, [pc, #816]`, which is exactly the pool-word-decodes-
 * as-an-instruction trap; the control-flow walk ends at the `bx r0`, so both
 * words are data.
 *
 * Func_08009180 is the six-argument renderer-family call: r0-r3 plus two
 * stack words, both of which are held in r5 (3) and r6 (2) across the whole
 * owner and re-stored before each of the three calls.  That is why r5/r6 are
 * saved.
 */
















extern void Func_02001d06();
extern void Func_02001e2c();
extern void Func_02001cd0();
extern void Func_02001c9e();
extern void Func_02001ce6();
extern void Func_02001cb4();
extern void Func_02001cfc();
extern void Func_02001cca();
extern void Func_02001e12();
extern void Func_02001d8c();
extern void Func_02001da6();
extern void Func_02001dce();
extern void Func_02001dc2();
extern void Func_02001d80();
extern void Func_02001e8c();
extern void Func_02001e98();
extern void Func_02001e7e();
extern void Func_02001da2();
void Func_02000154(void)
{
    s32 scale = 3;
    s32 style = 2;

    Func_02001d06();
    Func_02001e2c(181);

    Func_02001cd0(16, 28, 21, 3, scale, style);
    Func_02001c9e(10);
    Func_02001ce6(16, 30, 21, 3, scale, style);
    Func_02001cb4(10);
    Func_02001cfc(16, 32, 21, 3, scale, style);
    Func_02001cca(10);

    Func_02001e12(0, 2);
    Func_02001d8c(0, 0x9999, 0x4ccc);
    Func_02001da6(0, 120, 98);
    Func_02001dce(0, 2);
    Func_02001dc2(0, 0, -8);
    Func_02001d80(10);

    Func_02001e8c();
    Func_02001e98();
    Func_02001e7e(2);
    Func_02001da2();
}
