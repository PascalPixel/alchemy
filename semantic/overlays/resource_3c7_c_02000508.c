typedef signed short s16;
typedef signed int s32;

/*
 * Resource 3c7 main cutscene at 0x02000508 — the overlay's largest owner.
 *
 * Complete owner: `push {lr}` at 0x02000508 through the single interworking
 * return `pop {r0} / bx r0` at 0x02000ede.  r0 receives the popped return
 * address, so the owner returns nothing (HANDOVER §0).  r0's entry value is
 * never read, so the owner takes no arguments.
 *
 * POOL MAP, derived from a control-flow walk from the prologue (never from the
 * "pool words referenced" listing, per HANDOVER §0).  Three ranges are reached
 * only by being branched over or by lying past the epilogue:
 *   0x0200087e-0x0200089b  hopped by the `b.n` at 0x0200087c
 *   0x02000cda-0x02000ceb  hopped by the LONE `b.n` at 0x02000cd8, which has
 *                          no conditional structure around it at all
 *   0x02000ee2-0x02000eeb  alignment and pool past the epilogue
 * Register state is carried ACROSS all three; in particular the 0x02000cd8 hop
 * sits between two ordinary beats and changes nothing.
 *
 * COMPLETENESS PROOF, per target rather than by total (a matching total can
 * hide a permutation, HANDOVER §0).  `overlay_call_targets.ts` reports 306
 * sites / 28 distinct targets for this owner; the C below contains exactly the
 * same multiset:
 *   Func_0808a010 121   Func_0808a180  46   Func_0808a1b8  33
 *   Func_0808a110  20   Func_0808a138  16   Func_0808a1e8  15
 *   Func_0808a100   8   Func_0808a130   6   Func_0808a090   4
 *   Func_0808a0e8   4   Func_0808a0f0   4   Func_0808a080   3
 *   Func_0808a0b8   3   Func_0808a148   3   Func_0808a570   3
 *   Func_0808a070   2   Func_0808a150   2   Func_0808a170   2
 *   Func_0808a178   2   Func_080770c0   1   Func_08077260   1
 *   Func_0808a018   1   Func_0808a020   1   Func_0808a0d0   1
 *   Func_0808a360   1   Func_0808a370   1   Func_0808a580   1
 *   Func_02000508's own in-overlay callee Func_02000eec  1
 * The inventory's `calls=280` for this row is the usual undercount and is not
 * used as the proof.
 *
 * THE THREE SHARED-CALL-SITE SHAPES THAT COULD HAVE INFLATED THAT MULTISET,
 * and how each is handled here:
 *  - the two arms of the 0x0200081e test are behaviourally near-identical and
 *    differ only in one beat argument (Func_0808a110's 3 vs 4) and in where
 *    the frame-counter bump sits relative to the last Func_0808a180.  The
 *    second arm's Func_0808a180 at 0x020008f4 is the site the FIRST arm's
 *    `b.n` at 0x0200087c jumps past, i.e. the first arm has its own copy at
 *    0x02000868.  Each arm therefore genuinely owns one Func_0808a180 and the
 *    natural per-arm spelling is correct here — checked, not assumed.
 *  - the 0x020006a0 test's two arms rejoin at 0x020007f2 with no shared call
 *    site, so no `goto` is needed.
 *  - the three guarded Func_0808a0b8 blocks at the end each have their own
 *    Func_0808a080 and their own call site.
 *
 * THE PHANTOM-ARGUMENT TRAP fires three times here, at 0x02000e60,
 * 0x02000e90 and 0x02000ec0.  Each is `movs r3,#10 / ldrsh r1,[r0,r3] /
 * movs r3,#18 / ldrsh r2,[r0,r3] / bl`, so r3 still holds the load OFFSET 18
 * when the branch is taken.  Func_0808a0b8 takes THREE arguments, not four —
 * exactly the case HANDOVER §0 warns about.  The halfwords at record offsets
 * 10 and 18 are the integer parts of the 16.16 coordinate words at +8 and
 * +0x10, which is why they are read as s16.
 *
 * THE SKIP-BEAT COUNTER appears twice, once in each arm of the 0x0200081e
 * test: `ldr r3,[0x03001ebc] / ldr r2,[r3] / movs r3,#236 / lsls r3,#1 /
 * adds r2,r2,r3 / ldrh / adds #1 / strh` — a u16 at workspace + 472, bumped
 * by one.  This is the verbatim cross-overlay idiom HANDOVER §0 records for
 * resource_3c6, and recognising it is what makes the two arms read as one
 * beat with two variants.  `Data_03001ebc` is the workspace POINTER, proven by
 * the byte-exact sibling `assets/code/resource_3c7_c_0200048c.c`.
 *
 * Uncertainties, recorded rather than guessed:
 *  - the leading argument of the beat calls is a channel/actor id (0-3, 11,
 *    12); what each channel drives is not established here.
 *  - Func_0808a010's argument is a frame count; the identification rests on
 *    its position between beats, not on the callee.
 *  - 0x9bf is a story-flag id and only its truth value is used.
 *  - the constants 0x100-0x107 passed to Func_0808a1e8 / Func_0808a090 are
 *    animation ids; 0x2850, 0x2861 and 0x2002 are message ids.
 *  - Func_0808a070(0, 0) is called three times with both arguments zero; two
 *    of those results are tested and one (0x02000752) is discarded.
 */

/* Imports.  Old-style declarations throughout: two Func_ names in an overlay
 * can be the same import and one name can take different argument counts at
 * different sites, so no arity is asserted.  The two imports whose results are
 * used declare a return type, which the toolchain requires. */
s32 Func_080770c0();
s32 Func_0808a070();
s16 *Func_0808a080();
void Func_08077260();
void Func_0808a010();
void Func_0808a018();
void Func_0808a020();
void Func_0808a090();
void Func_0808a0b8();
void Func_0808a0d0();
void Func_0808a0e8();
void Func_0808a0f0();
void Func_0808a100();
void Func_0808a110();
void Func_0808a130();
void Func_0808a138();
void Func_0808a148();
void Func_0808a150();
void Func_0808a170();
void Func_0808a178();
void Func_0808a180();
void Func_0808a1b8();
void Func_0808a1e8();
void Func_0808a360();
void Func_0808a370();
void Func_0808a570();
void Func_0808a580();

/* The other owner of this overlay that this cutscene calls directly.
 * `overlay_call_targets.ts` classifies 0x02000eec as a real prologue, not a
 * veneer, and it is the straight-line script at that file offset. */
void Func_02000eec(void);

/* Overlay workspace pointer (see assets/code/resource_3c7_c_0200048c.c). */
extern unsigned char *Data_03001ebc;

/* The frame/beat counter both arms of the 0x0200081e test bump. */
static void BumpBeatCounter_02000508(void)
{
    unsigned short *counter = (unsigned short *)(Data_03001ebc + 472);

    *counter = (unsigned short)(*counter + 1);
}

void Func_02000508(void)
{
    s16 *actor;

    Func_0808a018();
    Func_08077260(1);
    Func_0808a0f0(0, 0x600000, 0xb80000);
    Func_0808a1b8(0, 0xc000, 0);
    Func_0808a1b8(11, 0, 0);
    Func_0808a1b8(12, 0x8000, 0);
    Func_0808a170(0x2850);
    Func_0808a360();
    Func_0808a370();
    Func_0808a010(10);
    Func_0808a090(0, 0x10000, 0x8000);
    Func_0808a580(0, 0, -16);
    Func_0808a0d0(0, 104, 136);
    Func_0808a010(10);
    Func_0808a570(1, -16, 16, 0xc000);
    Func_0808a570(3, 0, 24, 0xc000);
    Func_0808a570(2, 16, 16, 0xc000);
    Func_0808a0e8(1);
    Func_0808a010(20);
    Func_0808a010(10);
    Func_0808a138(2, 2);
    Func_0808a010(20);
    Func_0808a180(2, 0);
    Func_0808a010(10);
    Func_0808a1b8(11, 0x4000, 0);
    Func_0808a1b8(12, 0x4000, 0);
    Func_0808a010(30);
    Func_0808a1e8(11, 0x105, 70);
    Func_0808a180(11, 0);
    Func_0808a010(10);
    Func_0808a110(11, 4);
    Func_0808a010(20);
    Func_0808a180(11, 0);
    Func_0808a010(20);
    Func_0808a138(11, 2);
    Func_0808a010(20);
    Func_0808a180(11, 0);
    Func_0808a010(20);
    Func_0808a1e8(12, 0x102, 40);
    Func_0808a180(12, 0);
    Func_0808a010(10);
    Func_0808a110(12, 4);
    Func_0808a010(20);
    Func_0808a180(12, 0);
    Func_0808a010(20);
    Func_0808a010(10);
    Func_0808a1e8(11, 0x105, 50);
    Func_0808a180(11, 0);

    if (Func_080770c0(0x9bf) == 0) {
        Func_02000eec();
    } else {
        Func_0808a010(20);
        Func_0808a138(11, 2);
        Func_0808a010(20);
        Func_0808a180(11, 0);
        Func_0808a010(10);
        Func_0808a148(1, 0, 50);
        Func_0808a1e8(0, 0x105, 60);
        Func_0808a010(10);
        Func_0808a1b8(1, 0xc000, 0);
        Func_0808a010(20);
        Func_0808a180(1, 0);
        Func_0808a010(10);
        Func_0808a110(3, 4);
        Func_0808a010(20);
        Func_0808a180(3, 0);
        Func_0808a010(10);
        Func_0808a138(2, 2);
        Func_0808a010(20);
        Func_0808a180(2, 0);
        Func_0808a010(20);
        Func_0808a1e8(12, 0x101, 40);
        Func_0808a178(12, 0);
        Func_0808a070(0, 0);
        Func_0808a010(20);
        Func_0808a138(12, 2);
        Func_0808a010(20);
        Func_0808a180(12, 0);
        Func_0808a010(20);
        Func_0808a110(0, 3);
        Func_0808a010(30);
        Func_0808a010(10);
        Func_0808a138(1, 2);
        Func_0808a010(20);
        Func_0808a180(1, 0);
        Func_0808a010(10);
        Func_0808a110(3, 4);
        Func_0808a010(20);
        Func_0808a180(3, 0);
        Func_0808a010(10);
        Func_0808a110(2, 3);
        Func_0808a010(30);
        Func_0808a180(2, 0);
        Func_0808a010(10);
        Func_0808a148(2, 0, 30);
        Func_0808a178(0x2002, 0);
    }

    Func_0808a170(0x2861);
    Func_0808a010(10);
    Func_0808a1b8(0, 0x4000, 0);
    Func_0808a148(1, 0, 20);

    if (Func_0808a070(0, 0) == 0) {
        Func_0808a010(20);
        Func_0808a1b8(0, 0xc000, 0);
        Func_0808a1b8(1, 0xc000, 0);
        Func_0808a1b8(2, 0xc000, 0);
        Func_0808a010(30);
        Func_0808a010(10);
        Func_0808a110(11, 3);
        Func_0808a010(30);
        Func_0808a180(11, 0);
        BumpBeatCounter_02000508();
    } else {
        Func_0808a010(20);
        Func_0808a1b8(0, 0xc000, 0);
        Func_0808a1b8(1, 0xc000, 0);
        Func_0808a1b8(2, 0xc000, 0);
        Func_0808a010(30);
        Func_0808a010(10);
        Func_0808a110(11, 4);
        Func_0808a010(20);
        BumpBeatCounter_02000508();
        Func_0808a180(11, 0);
    }

    Func_0808a010(10);
    Func_0808a138(11, 2);
    Func_0808a010(20);
    Func_0808a180(11, 0);
    Func_0808a010(10);
    Func_0808a150(1, 0, 0);
    Func_0808a150(3, 2, 0);
    Func_0808a010(40);
    Func_0808a010(10);
    Func_0808a1e8(11, 0x102, 40);
    Func_0808a180(11, 0);
    Func_0808a010(10);
    Func_0808a1b8(0, 0xc000, 0);
    Func_0808a1b8(1, 0xc000, 0);
    Func_0808a1b8(2, 0xc000, 0);
    Func_0808a1b8(3, 0xc000, 0);
    Func_0808a010(50);
    Func_0808a010(10);
    Func_0808a110(11, 3);
    Func_0808a010(30);
    Func_0808a180(11, 0);
    Func_0808a010(10);
    Func_0808a138(11, 2);
    Func_0808a010(20);
    Func_0808a180(11, 0);
    Func_0808a010(10);
    Func_0808a1e8(3, 0x100, 40);
    Func_0808a180(3, 0);
    Func_0808a010(20);
    Func_0808a1b8(11, 0, 0);
    Func_0808a010(20);
    Func_0808a180(11, 0);
    Func_0808a010(10);
    Func_0808a138(12, 2);
    Func_0808a010(20);
    Func_0808a1b8(12, 0x8000, 0);
    Func_0808a010(20);
    Func_0808a180(12, 0);
    Func_0808a010(10);
    Func_0808a110(11, 4);
    Func_0808a010(20);
    Func_0808a180(11, 0);
    Func_0808a010(10);
    Func_0808a138(2, 2);
    Func_0808a010(20);
    Func_0808a180(2, 0);
    Func_0808a010(10);
    Func_0808a1b8(11, 0x4000, 0);
    Func_0808a010(30);
    Func_0808a180(11, 0);
    Func_0808a010(10);
    Func_0808a1e8(12, 0x102, 50);
    Func_0808a180(12, 0);
    Func_0808a010(20);
    Func_0808a1e8(11, 0x101, 65);
    Func_0808a1b8(11, 0, 0);
    Func_0808a010(20);
    Func_0808a180(11, 0);
    Func_0808a010(20);
    Func_0808a1e8(12, 0x100, 40);
    Func_0808a180(12, 0);
    Func_0808a010(20);
    Func_0808a1b8(12, 0x4000, 0);
    Func_0808a010(40);
    Func_0808a180(12, 0);
    Func_0808a010(10);
    Func_0808a1b8(11, 0x4000, 0);
    Func_0808a010(50);
    Func_0808a1e8(11, 0x106, 60);
    Func_0808a180(11, 0);
    Func_0808a010(10);
    Func_0808a110(11, 3);
    Func_0808a010(30);
    Func_0808a180(11, 0);
    Func_0808a010(10);
    Func_0808a130(0, 2);
    Func_0808a130(1, 2);
    Func_0808a130(3, 2);
    Func_0808a138(2, 2);
    Func_0808a010(30);
    Func_0808a010(10);
    Func_0808a1b8(11, 0, 0);
    Func_0808a010(20);
    Func_0808a180(11, 0);
    Func_0808a010(10);
    Func_0808a138(12, 2);
    Func_0808a010(30);
    Func_0808a1b8(12, 0x8000, 0);
    Func_0808a010(20);
    Func_0808a180(12, 0);
    Func_0808a010(10);
    Func_0808a1e8(11, 0x101, 0);
    Func_0808a010(10);
    Func_0808a1b8(1, 0xe000, 0);
    Func_0808a010(30);
    Func_0808a110(1, 4);
    Func_0808a010(20);
    Func_0808a180(1, 0);
    Func_0808a010(10);
    Func_0808a1b8(12, 0x4000, 0);
    Func_0808a010(30);
    Func_0808a1e8(12, 0x105, 60);
    Func_0808a180(12, 0);
    Func_0808a010(10);
    Func_0808a138(1, 2);
    Func_0808a010(20);
    Func_0808a180(1, 0);
    Func_0808a010(10);
    Func_0808a1b8(11, 0x4000, 0);
    Func_0808a010(40);
    Func_0808a1e8(11, 0x102, 40);
    Func_0808a180(11, 0);
    Func_0808a010(10);
    Func_0808a1b8(2, 0x8000, 0);
    Func_0808a010(30);
    Func_0808a1e8(2, 0x107, 60);
    Func_0808a1b8(2, 0xc000, 0);
    Func_0808a010(30);
    Func_0808a110(2, 4);
    Func_0808a010(20);
    Func_0808a180(2, 0);
    Func_0808a010(10);
    Func_0808a110(11, 4);
    Func_0808a010(20);
    Func_0808a180(11, 0);
    Func_0808a010(10);

    Func_0808a130(0, 2);
    Func_0808a130(1, 2);
    Func_0808a130(3, 2);
    Func_0808a138(2, 2);
    Func_0808a010(30);
    Func_0808a010(10);
    Func_0808a110(11, 3);
    Func_0808a010(30);
    Func_0808a180(11, 0);
    Func_0808a010(20);
    Func_0808a100(0, 3);
    Func_0808a100(1, 3);
    Func_0808a100(3, 3);
    Func_0808a110(2, 3);
    Func_0808a010(30);
    Func_0808a010(10);
    Func_0808a138(11, 2);
    Func_0808a010(20);
    Func_0808a180(11, 0);
    Func_0808a010(10);
    Func_0808a138(1, 2);
    Func_0808a010(20);
    Func_0808a180(1, 0);
    Func_0808a010(10);
    Func_0808a110(11, 3);
    Func_0808a010(30);
    Func_0808a180(11, 0);
    Func_0808a010(20);
    Func_0808a110(3, 3);
    Func_0808a010(30);
    Func_0808a180(3, 0);
    Func_0808a010(20);
    Func_0808a1b8(0, 0x4000, 0);
    Func_0808a1b8(1, 0xe000, 0);
    Func_0808a1b8(2, 0xa000, 0);
    Func_0808a010(30);
    Func_0808a110(0, 3);
    Func_0808a010(10);
    Func_0808a100(1, 3);
    Func_0808a100(2, 3);
    Func_0808a110(3, 3);
    Func_0808a010(30);
    Func_0808a090(1, 0x13333, 0x9999);
    Func_0808a090(3, 0x13333, 0x9999);
    Func_0808a090(2, 0x13333, 0x9999);
    Func_0808a100(1, 2);

    actor = Func_0808a080(0);
    if (actor != 0) {
        Func_0808a0b8(1, actor[5], actor[9]);
    }

    Func_0808a0e8(1);
    Func_0808a0f0(1, 0, 0);
    Func_0808a100(3, 2);
    actor = Func_0808a080(0);
    if (actor != 0) {
        Func_0808a0b8(3, actor[5], actor[9]);
    }

    Func_0808a0e8(3);
    Func_0808a0f0(3, 0, 0);
    Func_0808a100(2, 2);
    actor = Func_0808a080(0);
    if (actor != 0) {
        Func_0808a0b8(2, actor[5], actor[9]);
    }

    Func_0808a0e8(2);
    Func_0808a0f0(2, 0, 0);
    Func_0808a010(10);
    Func_0808a020();
}
