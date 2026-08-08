#include "types.h"

/*
 * Resource 39d alternating 32-frame emitter at 0x02003060 (340 bytes, 22
 * calls).
 *
 * Derived span, not an inventory row: this owner has no row in
 * out/decomp/overlays.json and no exact sibling.  It was found by sweeping the
 * two-byte gaps in metrics/gs1-en-executable.json for `push {..,lr}`
 * prologues.  Walking from the prologue at 0x02003060, every branch
 * (0x020030c6 -> 0x02003112, 0x02003110 -> 0x02003158, 0x0200315c ->
 * 0x02003172, and the back edge 0x0200317a -> 0x020030b8) lands inside
 * 0x02003060-0x020031b3, and the walk stops at the high-register unwind
 * `add sp,#56 / pop {r3,r5,r6} / mov r8,r3 / mov r9,r5 / mov sl,r6 /
 * pop {r5,r6,r7} / pop {r0} / bx r0` at 0x020031a4-0x020031b2.  So the
 * executable extent is exactly 0x02003060-0x020031b3 (340 bytes).  The popped
 * register is r0, so the popped value is the return address and the owner is
 * `void`.
 *
 * Pool map — three words the walk never reaches as instructions, two of which
 * objdump renders as plausible NEON garbage:
 *   0x020031b4  0x00006666   0.4 in 16.16, a parameter-block field
 *   0x020031b8  0xfff40000   -12.0 in 16.16
 *   0x020031bc  0xfffc0000   -4.0 in 16.16
 * The next prologue (`push {r5, r6, lr}`) begins at 0x020031c0 and is already
 * converted as resource_39d_c_020031c0.c, so this owner is bounded on both
 * sides by known code.
 *
 * All 22 call sites were resolved with
 * `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_39d 3060 31b4`, and the
 * per-target histogram is Func_0808a080 x6, Func_080091e0 x4, Func_080000f8
 * x4, Func_0808a158 x4, Func_0200013c x2, Func_080f9010 x1, Func_0808a010 x1:
 *   0x0200306e 0x02003076 0x02003084 0x02003090 0x0200318e 0x0200319a
 *                              -> veneer 0x036f0 -> Func_0808a080
 *   0x0200308a 0x02003096 0x02003194 0x020031a0
 *                              -> veneer 0x03690 -> Func_080091e0
 *   0x020030c8 0x020030e0 0x02003112 0x0200312a
 *                              -> veneer 0x03640 -> Func_080000f8
 *   0x02003164 0x0200316e 0x02003180 0x02003188
 *                              -> veneer 0x03758 -> Func_0808a158
 *   0x0200310c 0x02003154 -> prologue 0x0013c -> Func_0200013c   local spawn
 *   0x0200307e -> veneer 0x03840 -> Func_080f9010
 *   0x020030ba -> veneer 0x036d0 -> Func_0808a010   wait frames
 * `overlay_show` prints the two local calls as 0x0200324a and 0x02003292,
 * which are not even the same address as each other although both are the one
 * callee — the pc-relative misreading again.
 *
 * ONE Func_0808a010 SITE INSIDE A 32-ITERATION LOOP.  The site count is the
 * multiset, not the dynamic count, so the wait is written once inside the loop
 * body; hoisting or unrolling it would be the inflation defect.
 *
 * THE TWO ARMS ARE NOT MERGEABLE even though they have the same shape.  The
 * odd-index arm reads its base record out of r9 (record 22) and the even-index
 * arm out of sl (record 24), and each arm has its own pair of Func_080000f8
 * draws and its own Func_0200013c site — four and two in the assembly.  Folding
 * them into one arm with a selected record would halve both counts.  The only
 * other difference is cosmetic: the odd arm passes a fresh `movs r3,#0` for the
 * sixth argument while the even arm passes r6, which on that path is provably
 * `i & 1 == 0`.
 *
 * Random placement, the established scaling idiom in its 16.16 spelling:
 * `lsls #1 / adds / lsls #3` is a multiply by 24 and `lsls #5` a multiply by
 * 32, and the `lsrs #16 / lsls #16` that follows each is not a shift pair but a
 * FLOOR — it clears the low 16 bits, so the product is a whole number carried
 * in 16.16.  So the spawn is offset from the record's +8 by a random 0..23
 * tiles less 12.0, and from its +12 by a random 0..31 tiles plus 32.0
 * (`movs #128 / lsls #14`); +16 is passed through unchanged.
 *
 * The 40-byte parameter block at sp+16 is spelled as bytes rather than as a
 * struct because only five of its fields are written (+0, +4, +8, +12 as words
 * and +24 as a halfword) and the rest of the frame is left as it was — an
 * invented struct would imply the holes are zero.  It is filled once before the
 * loop and the same block is handed to every spawn.
 *
 * At iteration 20 exactly, both records get Func_0808a158(record, 256); after
 * the loop both get Func_0808a158(record, 0) and then Func_080091e0(record, 1),
 * mirroring the Func_080091e0(record, 0) pair that opened the run.  So the
 * owner brackets its own effect: disable, emit for 32 frames, re-enable.
 */

u8 *Func_0808a080();           /* record fetch, returns the record */
void Func_080091e0();          /* record enable/disable */
s32 Func_080000f8(void);       /* 0..0xffff random source */
void Func_0808a158();          /* two-argument actor call */
void Func_0808a010();          /* wait this many frames */
void Func_080f9010();          /* one-argument notify */
void Func_0200013c();          /* local: spawn, eight arguments */

void Func_02003060(void)
{
    s32 permuted_1;
    s32 permuted_0;
    u8 *first = Func_0808a080(22);
    u8 *second = Func_0808a080(24);
    u8 params[40];
    s32 index;
    s32 odd;
    s32 x;
    s32 y;

    Func_080f9010(190);
    Func_080091e0(Func_0808a080(22), 0);
    Func_080091e0(Func_0808a080(24), 0);

    *(s32 *)(params + 0) = 1;
    *(u16 *)(params + 24) = 284;               /* 142 << 1 */
    *(s32 *)(params + 8) = 0x6666;
    *(s32 *)(params + 12) = 0x30000;           /* 192 << 10, i.e. 3.0 */
    *(s32 *)(params + 4) = 5;

    for (index = 0; index <= 31; index++) {
        Func_0808a010(1);
        if (odd != 0) {
            permuted_0 = *(s32 *)(first + 8) + ((Func_080000f8() * 24) & ~0xffff) - 0xc0000;
            permuted_1 = *(s32 *)(first + 12) + ((Func_080000f8() * 32) & ~0xffff) + 0x200000;
            Func_0200013c(x, y, *(s32 *)(first + 16), 0,
                          -0x40000, 0, 0x1b0000, params);
        } else {
            x  = permuted_0;
            y  = permuted_1;
            x = *(s32 *)(second + 8) + ((Func_080000f8() * 24) & ~0xffff) - 0xc0000;
            y = *(s32 *)(second + 12) + ((Func_080000f8() * 32) & ~0xffff) + 0x200000;
            Func_0200013c(x, y, *(s32 *)(second + 16), 0,
                          -0x40000, odd, 0x1b0000, params);
        }
        if (index == 20) {
            Func_0808a158(22, 256);            /* 128 << 1 */
            Func_0808a158(24, 256);
        }
        odd = index & 1;
    }

    Func_0808a158(22, 0);
    Func_0808a158(24, 0);
    Func_080091e0(Func_0808a080(22), 1);
    Func_080091e0(Func_0808a080(24), 1);
}
