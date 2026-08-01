typedef signed int s32;

/*
 * resource_3bd owner at 0x02000030, 60 bytes: 56 of code, a one-word pool.
 *
 * HEAD FUNCTION -- the only one of the tree's 25 CODE-SUSPECT heads that is
 * neither an export-table stub nor the shared bitfield setter at 0x30.
 *
 * It lies BEFORE the first recorded owner (0x0200006c), in the region
 * `gapsBetween` never read. Nothing publishes it and no `bl` targets it
 * (`overlay_call_targets` over explicit whole-image bounds, sites=1224, a live
 * run). Sweep C is the ONE instrument that sees it, and its output says so in a
 * way easy to misread: `C shaped 0x2000030 b520 nearest owner 0x? +null`. The
 * `+null` is not a formatting wart -- it is the statement that no owner
 * precedes it.
 *
 * The ROM, verbatim:
 *
 *   push {r5, lr}
 *   ldmia r0!, {r5} / ldmia r1!, {r3} / ldmia r0!, {r4} / subs r5, r5, r3
 *   ldmia r1!, {r3} / ldr r2, [r1] / subs r4, r4, r3
 *   ldr r3, [r0]    / subs r3, r3, r2
 *   asrs r5, #16 / asrs r4, #16 / asrs r3, #16
 *   muls r0, r5  / muls r2, r4  / muls r1, r3
 *   adds r0, r0, r2 / adds r0, r0, r3
 *   ldr r3, [pc, #8]       -> 0x030001d8
 *   bl <call_via r3>
 *   pop {r5} / pop {r1} / bx r1
 *
 * WHAT IT DOES. Two pointers to three-component vectors are walked in step by
 * the paired `ldmia` writebacks; each component difference is taken as a full
 * word and then `asrs #16`, so the inputs are 16.16 FIXED POINT and the
 * arithmetic is done on their integer parts. The three differences are squared
 * and summed, and the sum is passed to the IWRAM routine at 0x030001d8 through
 * the `call_via` bank -- a squared distance handed to a callee, whose result
 * this row returns unchanged.
 *
 * WHAT IT DOES NOT SETTLE, and this is the load-bearing bound: 0x030001d8 is
 * an IWRAM address, not an overlay one, so what it computes is not decidable
 * from this row. A square root is the obvious reading and obvious is not
 * measured. The row is a distance-squared reducer plus a tail call; naming the
 * callee would be a guess dressed as a reconstruction.
 *
 * THE C BELOW IS AN APPROXIMATION AND IS MARKED AS ONE. The tail call through
 * a function pointer held in an IWRAM word, and the `pop {r5} / pop {r1} /
 * bx r1` epilogue that returns through a popped register rather than lr,
 * are not expressible in the semantic dialect. What is reconstructed is the
 * arithmetic; the call is written as an indirect call and the shape of the
 * return is documented here rather than encoded.
 *
 * SPAN IS 60 BYTES. The pool word 0x030001d8 at 0x2000068 is reached by this
 * row's own `ldr r3, [pc, #8]` (pc 0x2000060 after alignment, plus 8). The far
 * bound is the ROM's: the first recorded owner starts at 0x0200006c.
 *
 * THE INVENTORY HAS NO ROW FOR THIS FUNCTION, and what it has instead is worth
 * recording. Its only row below 0x120 on this overlay is at 0x24, span 12,
 * returns 0 -- and 0x24 is the TARGET WORD of the interworking veneer at 0x20,
 * holding 0x0200b599, whose low halfword 0xb599 wears a `push {r0,r3,r4,r7,lr}`
 * shape. A published pointer half was seeded as a function; the real function
 * twelve bytes later was not. `maskBanks` covers 0x20-0x27 as a veneer and so
 * never sees the phantom, which is why sweep D's head reports exactly one
 * return here (0x66) and no prologue noise.
 *
 * 1 of 1 callees resolved (the indirect target is IWRAM and out of scope).
 */

s32 Func_02000030(const s32 *a, const s32 *b)
{
    s32 dx = (a[0] - b[0]) >> 16;
    s32 dy = (a[1] - b[1]) >> 16;
    s32 dz = (a[2] - b[2]) >> 16;

    return (*(s32 (**)(s32))0x030001d8)(dx * dx + dy * dy + dz * dz);
}
