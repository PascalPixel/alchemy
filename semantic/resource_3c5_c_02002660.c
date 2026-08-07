#include "types.h"

/*
 * resource_3c5 owner at 0x02002660, 72 bytes: "which object stands on the
 * square one step ahead of this mover".
 *
 * Shape, straight from the disassembly:
 *
 *   push {r5, r6, lr}; r5 = arg0
 *   r3 = [r5, #6] >> 12                 heading -> direction index
 *   r2 = 0x0200afd4                     packed direction-step table
 *   r0 = r2[r3]
 *   sub sp, #12; r6 = sp                the 12-byte probe record
 *   probe.x = [r5, #8]  + (step & 0xffff0000)
 *   probe.z = [r5, #16] + (step << 16)
 *   r0 = [r5, #0x22]; bl <veneer> -> Func_080091a8(layer, probe.x, probe.z)
 *   probe.y = r0
 *   bl <in-image 0x0200006c>(&probe, r5)
 *   add sp, #12; pop {r5, r6}; pop {r1}; bx r1
 *
 * Two facts settle the signature and the constants without guessing:
 *
 *  - The epilogue is `pop {r1} ; bx r1` (N != 0), so r0 survives the return
 *    and IS the result: the value returned is whatever 0x0200006c returned,
 *    i.e. the occupying record or 0.
 *  - Both calls were resolved with `bun tools/lib/overlay_call_targets.ts
 *    resource_3c5 2660`, which reports sites=2: the site at 0x0200268a is an
 *    import veneer publishing Func_080091a8, and the site at 0x02002694
 *    resolves to the in-image prologue at 0x0200006c.  The printed `bl`
 *    annotations (0x0200545c and 0x02002702) are the usual overlay decoding
 *    artefact and are not used here.
 *
 * The pool word 0x0200afd4 is the same packed direction-step table that
 * semantic/overlays/resource_3c5_c_020000c4.c loads: X step in the high
 * halfword, Z step in the low, promoted to 16.16 by masking and shifting
 * rather than by multiplying.  Under this overlay's proven 0x02008000 link
 * base it is file offset 0x2fd4, i.e. in-image data, not a RAM global.
 *
 * Func_080091a8 is the main image's terrain-height probe; the semantic tree
 * already types it as `s32 (layer, x, z)` (semantic/main/0808b674.c,
 * 0808ce74.c) and resource_399's owner passes exactly this record's byte 34
 * (0x22) as the layer, which is what fixes the argument here.
 *
 * Uncertainty: the byte at +0x22 is called `state22` by the 0x020000c4 source
 * (which stores 2 into it) and `subject[34]` by resource_399; the reading
 * consistent with both is a small terrain/collision layer id.  Nothing in this
 * owner depends on which name is right.
 */

struct ProbeMover {
    u8 unknown_00[6];
    u16 heading;                    /* +0x06, top four bits select direction */
    s32 x;                          /* +0x08 */
    s32 y;                          /* +0x0c (not read here) */
    s32 z;                          /* +0x10 */
    u8 unknown_14[0x0e];
    u8 layer22;                     /* +0x22 */
};

struct ProbeSquare {
    s32 x;
    s32 y;
    s32 z;
};

/* Packed direction steps: X step in the high halfword, Z step in the low. */
#define PROBE_DIRECTION_STEPS ((const s32 *)0x0200afd4)

/* Old-style declarations are mandatory in overlay sources: one import name can
 * be reached with different argument counts at different sites. */
s32 Func_080091a8();                    /* terrain height at (layer, x, z) */
struct ProbeMover *Func_0200006c();     /* occupant of a square, or 0 */

struct ProbeMover *Func_02002660(struct ProbeMover *mover)
{
    struct ProbeSquare probe;
    s32 step = PROBE_DIRECTION_STEPS[mover->heading >> 12];

    probe.x = mover->x + (step & (s32)0xffff0000);
    probe.z = mover->z + (step << 16);
    probe.y = Func_080091a8((s32)mover->layer22, probe.x, probe.z);

    return Func_0200006c(&probe, mover);
}
