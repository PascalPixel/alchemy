/*
 * resource_3a7 scene fade-out / score-settle beat at 0x02000754, 340 bytes.
 *
 * Complete owner: `push {r5, r6, r7, lr} / mov r7,sl / mov r6,r8 /
 * push {r6, r7} / sub sp, #16` at 0x02000754 through the matching
 * `add sp,#16 / pop {r3, r5} / mov r8,r3 / mov sl,r5 / pop {r5, r6, r7} /
 * pop {r0} / bx r0` at 0x02000874.  The return address is popped into r0, so
 * the owner is `void`.  Its literal pool occupies 0x02000884 .. 0x020008a7,
 * past the epilogue and inside the 340-byte span; the control-flow walk reaches
 * none of it (0x02000882 is alignment).  Several of those words decode as
 * plausible instructions - 0x0000e666 as `b.n 0x200055c`, 0x8d4fdf3b as an
 * `svc` - which is exactly why the pool map is taken from the control flow and
 * not from the listing.
 *
 * The owner takes no arguments.
 *
 * Workspace: `*(u8 **)0x03001e70 + 356`, the documented pointer CELL (its
 * neighbour 0x03001e70 + 76 is 0x03001ebc, the workspace pointer the rest of
 * this overlay loads directly), so one dereference, not two.  Fields used: two
 * s32 direction flags at +24 and +28, and an s32 accumulator at +36.
 *
 * Shape:
 *   1. set both direction flags to +1 or -1 from bit 0 of `*(s32 *)0x03001e40`;
 *   2. two Func_080091f0 requests, then a 163-frame wait;
 *   3. 480 iterations of a soft-float settle: convert the accumulator and a
 *      16-bit random sample to soft doubles, compute
 *      `accumulator - (K - sample)` with the overlay's own double subtraction,
 *      convert back to an integer and store it, ticking the scene once per
 *      iteration.  K is the pool pair 0x8d4fdf3b:0x40b26e97;
 *   4. 70 iterations of a colour ramp: write the packed 5-5-5 value
 *      `(level << 10) | (level << 5) | level` and step `level` down by one
 *      every 20th iteration (6 -> 2), ticking the scene each time;
 *   5. a final six-argument request, a 288-frame wait, and three teardown calls.
 *
 * SOFT-FLOAT ABI, taken from the sibling conversions already in this overlay:
 * `SoftDouble` is a u64 whose LOW half is the r0 word (see
 * resource_3a7_c_020015a4.c - the packer leaves the double's HIGH word in r0),
 * Func_0200145c converts s32 -> SoftDouble, Func_020013ac adds and
 * Func_020013e4 subtracts.  Func_020014d8 is the matching SoftDouble -> s32
 * conversion; it is not a strict-queue row and is declared only.
 * The 0x41f00000:0x00000000 pair is 2^32 exactly, and the `if (sample < 0) add
 * 2^32` around it is the compiler's standard unsigned-to-double fixup.
 *
 * DEAD BUT TRANSCRIBED: `sample` is produced by `lsls #11` then a LOGICAL
 * `lsrs #16`, so it is always in 0..0xffff and the `bge` at 0x020007d6 is always
 * taken - the Func_020013ac call can never execute.  It is written out anyway,
 * because deleting it would drop a real call site from the per-target multiset.
 *
 * Call targets resolved with
 *   bun tools-rs/overlay-show resource_3a7 0754 -n 340 |
 *   bun tools/lib/overlay_call_targets.ts resource_3a7 --annotate
 * Per-target multiset over the 20 sites, matching the inventory's calls=20:
 *   Func_0808a018 x1 <- veneer 0x02001120
 *   Func_080091f0 x2 <- veneer 0x020010e8
 *   Func_080f9010 x2 <- veneer 0x02001150   (163 frames, then 288)
 *   Func_080000f8 x1 <- veneer 0x02001090   (random source, no arguments)
 *   Func_0200145c x2  (this overlay's s32 -> soft double)
 *   Func_020013ac x1  (this overlay's soft-double addition; see above)
 *   Func_020013e4 x2  (this overlay's soft-double subtraction)
 *   Func_020014d8 x1  (this overlay's soft double -> s32)
 *   Func_0808a010 x2 <- veneer 0x02001118  (once per loop, two loops)
 *   Func_08000318 x1 <- veneer 0x02001078+
 *   Func_030003ac x1 <- relocated IWRAM modulo helper
 *   Func_080091b8 x1 <- veneer 0x020010c8
 *   Func_08009128 x1 <- veneer 0x020010b0
 *   Func_080091f8 x1 <- veneer 0x020010f0
 *   Func_0808a020 x1 <- veneer 0x02001128
 * Both loops are single-site: each `bl` inside them is one C call expression,
 * not one per iteration.
 *
 * Uncertainties:
 *  - 0x04000052 is passed to Func_08000318 as a plain address argument.  It is
 *    numerically the GBA blend-weight register, and the second argument packs
 *    three 5-bit fields, which reads as a colour rather than a blend pair; the
 *    import is not established, so both are transcribed rather than named.
 *  - the two Func_080091f0 argument triples (0x30000/0x30000/0x10000 and
 *    -1/-1/0xe666) are transcribed, not interpreted.
 *  - the constant K is left as its two pool words; no interpretation of the
 *    double's value is asserted.
 */

/* Old-style declarations: overlay imports vary their argument count between
 * call sites in this overlay.  Those used for their results need a type. */
#include "types.h"

/* A double carried in the r0/r1 register pair; the LOW half of this u64 is the
 * r0 word, which the overlay's packer leaves holding the double's HIGH word. */
typedef u64 SoftDouble;

void Func_0200188c();
void Func_02001882();
void Func_02001890();
void Func_020018fe();
s32 Func_02001836();
SoftDouble Func_02001c1c();
SoftDouble Func_02001c2a();
SoftDouble Func_02001b8a();
SoftDouble Func_02001bd2();
SoftDouble Func_02001bde();
s32 Func_02001cd6();
void Func_02001920();
void Func_020018ba();
void Func_02001948();
s32 Func_02001898();
void Func_02001926();
void Func_020019b6();
void Func_0200191a();
void Func_0200195e();
void Func_0200199a();

                            /* scripted delay, in frames */
                            /* random source, no arguments */
                            /* s32 -> soft double */
                            /* soft-double addition */
                            /* soft-double subtraction */
                            /* soft double -> s32 */

                            /* relocated IWRAM modulo helper */
                            /* six-argument scene request, last two on stack */

                     

void Func_02000754(void)
{
    /* Pointer CELL, one dereference. */
    u8 *workspace = *(u8 **)0x03001e70 + 356;
    s32 remaining;
    s32 index;
    s32 level;
    s32 trailing;

    Func_0200188c();

    if ((*(s32 *)0x03001e40 & 1) != 0) {
        *(s32 *)(workspace + 24) = 1;
        *(s32 *)(workspace + 28) = 1;
        *(s32 *)(workspace + 28) = -1;
    } else {
        *(s32 *)(workspace + 24) = -1;
    }

    Func_02001882(0x30000, 0x30000, 0x10000);
    Func_02001890(-1, -1, 0xe666);
    Func_020018fe(0xa3);

    remaining = 0x1df;
    do {
        s32 raw = Func_02001836();
        SoftDouble accumulator = Func_02001c1c(*(s32 *)(workspace + 36));
        s32 sample = (s32)(((u32)raw << 11) >> 16);
        SoftDouble sampleValue = Func_02001c2a(sample);
        SoftDouble term;

        /* Unsigned-to-double fixup; unreachable because `sample` is a 16-bit
         * logical shift result and therefore never negative.  Kept. */
        if (sample < 0) {
            sampleValue = Func_02001b8a(sampleValue,
                                        ((SoftDouble)0x00000000u << 32)
                                            | (SoftDouble)0x41f00000u);
        }

        term = Func_02001bd2(((SoftDouble)0x8d4fdf3bu << 32)
                                 | (SoftDouble)0x40b26e97u,
                             sampleValue);

        *(s32 *)(workspace + 36) =
            Func_02001cd6(Func_02001bde(accumulator, term));

        Func_02001920(1);

        remaining--;
    } while (remaining >= 0);

    index = 0;
    trailing = 6;
    level = 6;
    do {
        Func_020018ba(0x04000052,
                      (level << 10) | (level << 5) | trailing);
        Func_02001948(1);

        if (Func_02001898(index, 20) == 0) {
            trailing--;
            level--;
        }

        index++;
    } while (index <= 69);

    Func_02001926(19, 83, 15, 8, 19, 91);
    Func_020019b6(288);

    Func_0200191a();
    Func_0200195e();
    Func_0200199a();
}
