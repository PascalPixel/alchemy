typedef signed short s16;
typedef signed int s32;

/*
 * Resource 394 status-strip placement at 0x02000150.
 *
 * Complete owner: `push {lr}` at 0x02000150 and the matching
 * `add sp,#8 / pop {r0} / bx r0` at 0x02000188.  68-byte row: 62 bytes of
 * code, an alignment halfword at 0x0200018e, and the single pool word
 * 0x020092c4 at 0x02000190.  Control-flow walk: the only branch targets are
 * 0x02000174 and 0x02000188, so nothing reaches 0x0200018e.  The return
 * address is popped into r0, so the owner is `void`.
 *
 * Called once, from 0x02000194's tail.
 *
 * 0x020092c4 is a pointer CELL, not the value: the body is
 * `ldr r3,[pc] / ldr r3,[r3] / ldrsh r3,[r3]`, and banked byte-exact
 * assets/code/resource_394_c_020008b0.c already declares it
 * `extern u16 *Data_020092c4` and writes through it.  The selector it points
 * at is the same one 0x02000194 branches on.
 *
 * Cross-check on the argument order: the else arm's six arguments are
 * (0, 0, 1, 4, 6, 9), which is literally the argument list banked byte-exact
 * assets/code/resource_394_c_020008b0.c passes to the same import under its
 * printed name Func_020019cc.  That fixes the four-register-plus-two-stack
 * spelling without inference.
 *
 * Two call sites, matching the row's advertised count.  The arms are kept
 * separate: they differ only in the fifth argument, and merging them would
 * halve this import's site count.
 */

/* Old-style declarations: interfaces vary by call site across this overlay. */
void Func_080091c0(); /* place a fixture: (x, y, w, h, sx, sy) */

void Func_02000150(void)
{
    if (**(s16 **)0x020092c4 == 1) {
        Func_080091c0(0, 0, 1, 4, 4, 9);
    } else {
        Func_080091c0(0, 0, 1, 4, 6, 9);
    }
}
