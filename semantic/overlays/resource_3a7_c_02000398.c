/*
 * resource_3a7 scene-request step at 0x02000398, 70 bytes.
 *
 * Complete owner: `push {r5, lr}` / `sub sp, #8` at 0x02000398 through the
 * interworking return `add sp,#8 / pop {r5} / pop {r0} / bx r0` at 0x020003d6.
 * The epilogue pops the return address into r0, so nothing is returned and the
 * owner is `void` (HANDOVER "the interworking epilogue tells you the return
 * type").  No literal pool sits inside the span - every value is an immediate.
 *
 * The owner takes no arguments: r0-r3 are written before any read.
 *
 * Call targets resolved with
 *   bun tools/overlay_show.ts resource_3a7 0398 -n 70 |
 *   bun tools/overlay_call_targets.ts resource_3a7 --annotate
 * Three sites, all import veneers, matching the inventory's calls=3:
 *   Func_080091b8 x2  <- veneer 0x020010c8
 *   Func_080091c0 x1  <- veneer 0x020010d0
 * Both are the established six-argument renderer/scene ABI: four register
 * arguments plus two stack words at [sp,#0] and [sp,#4].
 *
 * This owner is a byte-for-byte twin of 0x02000410 in the same overlay (same
 * 70-byte shape, same three callees in the same order); the two differ only in
 * their immediate ids.  Each therefore cross-checks the other.
 *
 * r5 is a constant holder for the [sp,#0] word shared by the second and third
 * calls (16 here, 17 in the twin); it is the only saved register.
 *
 * Uncertainty: the ids are transcribed, not interpreted.  The first call's
 * stack word 80 also appears as the twin's, so it is a mode/duration rather
 * than a scene id, but nothing here proves that.
 */

/* Old-style declarations: overlay imports are reached with varying argument
 * counts elsewhere in this overlay, so arities are left open. */
void Func_080091b8();   /* six-argument scene/strip request, last two on stack */
void Func_080091c0();   /* six-argument renderer, last two on the stack */

void Func_02000398(void)
{
    /* r5, live across all three calls */
    int strip = 16;

    Func_080091b8(87, 50, 2, 4, 80, 50);
    Func_080091b8(23, 52, 1, 2, strip, 52);
    Func_080091c0(16, 52, 1, 1, strip, 53);
}
