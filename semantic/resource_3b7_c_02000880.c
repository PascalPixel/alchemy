/*
 * resource_3b7 owner at 0x02000880, 120 bytes (0x02000880-0x020008f7).
 *
 * A two-stage scene transition: sixteen frames of pulling one workspace field
 * down by 1.0 in 16.16, then eight frames of an alpha-blend ramp driven from a
 * table of BLDALPHA values.
 *
 * Code runs 0x02000880-0x020008df.  The trailing 24 bytes,
 * 0x020008e0-0x020008f7, are a literal pool of six words - 0x03001e70,
 * 0xffff0000, 0x00003f42, 0x04000050, 0x02009fc0, 0x04000052.  They sit past
 * the `bx r0` return and are reached only by the `ldr rN, [pc, ...]` loads, so
 * they are data.  (The disassembler decodes 0x020008e6 as a NEON instruction,
 * which is a good illustration of why pool bytes must never be modelled as
 * instructions.)
 *
 * LINK BASE - confirmed again here.  The pool word 0x02009fc0 is in the
 * in-image band, so under the proven 0x02008000 base it is file offset 0x1fc0,
 * and the eight halfwords there are 0x090a, 0x0a08, 0x0b06, 0x0c04, 0x0d03,
 * 0x0e02, 0x0f01, 0x1000 - exactly the eight the loop consumes, and exactly a
 * BLDALPHA ramp whose EVA/EVB coefficient pairs run (10,9), (8,10), (6,11),
 * (4,12), (3,13), (2,14), (1,15), (0,16).  The value is even, which is the
 * documented one-bit data/callback test, and it agrees.
 *
 * 0x04000050 and 0x04000052 are BLDCNT and BLDALPHA; 0x00003f42 is the blend
 * control word written to BLDCNT on every one of the eight frames.  0x03001e70
 * is the IWRAM workspace pointer already established for this project (the same
 * base whose +76 is the 0x03001ebc pointer other overlays load directly); this
 * owner loads the pointer stored there and works 356 bytes into it.
 *
 * Call targets were resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --` (an overlay
 * `bl` stores `target_offset - 2`).  In program order:
 *   0x0200088e -> veneer 0x020019c4 -> Func_080f9010
 *   0x020008a6 -> veneer 0x02001864 -> Func_080000c0   (in the 16-frame loop)
 *   0x020008cc -> veneer 0x02001864 -> Func_080000c0   (in the 8-frame loop)
 * Completeness: 3 sites over 2 distinct targets, matching the tool's
 * `sites=3 distinct_targets=2` and the inventory row's calls=3.
 *
 * Both loops are `subs r6, #1 / cmp r6, #0 / bge`, so the counter is tested
 * AFTER the decrement and the body runs one more time than the initial value:
 * 16 iterations from r6 = 15, 8 iterations from r6 = 7.
 *
 * The epilogue is `pop {r3, r5} / mov r8, r3 / mov sl, r5 / pop {r5, r6, r7} /
 * pop {r0} / bx r0`, so r0 holds the popped return address and the owner
 * returns nothing: void.  r0 is written before any read, so it takes no
 * argument.  It is called from Func_02000104.
 *
 * Uncertainties: Func_080f9010(216) and Func_080000c0(N) have unknown
 * interfaces, hence the old-style declarations; Func_080000c0's argument being
 * 4 in the first loop and 8 in the second, with no result used, reads as a
 * frame/step wait, but nothing here proves that.  sl and r8 are used only as
 * spill slots for the two loop-invariant constants.
 */

#include "types.h"

extern u16 Data_02009fc0[];     /* in-image, file offset 0x1fc0: 8 BLDALPHA words */

void Func_02002254();
void Func_0200210c();
void Func_02002132();

void Func_02000880(void)
{
    s32 *workspace;
    const u16 *ramp;
    s32 i;

    /* The pointer stored at the IWRAM workspace base, then 178 * 2 bytes in. */
    workspace = (s32 *)(*(char **)0x03001e70 + 356);

    Func_02002254(216);

    for (i = 15; i >= 0; i--) {
        /* Adding 0xffff0000 is subtracting 1.0 in 16.16. */
        workspace[3] = workspace[3] + (s32)0xffff0000;
        Func_0200210c(4);
    }

    ramp = Data_02009fc0;
    for (i = 7; i >= 0; i--) {
        *(volatile u16 *)0x04000050 = 0x3f42;    /* BLDCNT */
        *(volatile u16 *)0x04000052 = *ramp++;   /* BLDALPHA */
        Func_02002132(8);
    }
}
