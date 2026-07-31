/*
 * resource_3a7 scene-request step at 0x02000410, 70 bytes.
 *
 * TRANSPOSED from semantic/overlays/resource_3a7_c_02000398.c, its twin in the
 * same overlay.  Over all 35 halfwords the two owners differ only in eight
 * immediates (the scene/strip ids below); the prologue, the frame, the three
 * BL encodings and the epilogue are identical.  That equality is the
 * correctness proof for this file.
 *
 * Complete owner: `push {r5, lr}` / `sub sp, #8` at 0x02000410 through
 * `add sp,#8 / pop {r5} / pop {r0} / bx r0` at 0x0200044e.  The return address
 * is popped into r0, so the owner is `void`.  No literal pool inside the span.
 *
 * Three call sites, matching the inventory's calls=3, resolved with
 * `bun tools/overlay_call_targets.ts resource_3a7 0410`:
 *   Func_080091b8 x2  <- veneer 0x020010c8
 *   Func_080091c0 x1  <- veneer 0x020010d0
 * Six-argument ABI: four register arguments plus [sp,#0] and [sp,#4].
 *
 * r5 holds the shared [sp,#0] word (17) across the second and third calls.
 *
 * Uncertainty: the ids are transcribed, not interpreted.
 */

/* Old-style declarations: arities left open, as elsewhere in this overlay. */
void Func_080091b8();   /* six-argument scene/strip request, last two on stack */
void Func_080091c0();   /* six-argument renderer, last two on the stack */

void Func_02000410(void)
{
    /* r5, live across all three calls */
    int strip = 17;

    Func_080091b8(90, 9, 2, 3, 80, 9);
    Func_080091b8(27, 10, 1, 2, strip, 10);
    Func_080091c0(17, 10, 1, 1, strip, 11);
}
