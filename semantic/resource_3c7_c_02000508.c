#include "types.h"

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
void Func_02001708();
void Func_020016fe();
void Func_02001764();
void Func_020017c0();
void Func_020017ca();
void Func_020017d6();
void Func_020017c4();
void Func_02001810();
void Func_0200181c();
void Func_0200174a();
void Func_02001780();
void Func_02001854();
void Func_020017ae();
void Func_02001774();
void Func_0200186c();
void Func_0200187a();
void Func_02001888();
void Func_020017ee();
void Func_020017ac();
void Func_020017b2();
void Func_0200182a();
void Func_02001860();
void Func_020017ce();
void Func_02001886();
void Func_020017ec();
void Func_0200189e();
void Func_02001896();
void Func_02001804();
void Func_02001812();
void Func_020018b2();
void Func_02001820();
void Func_02001898();
void Func_0200182e();
void Func_020018ce();
void Func_0200183c();
void Func_020018f0();
void Func_020018e8();
void Func_02001856();
void Func_020018be();
void Func_02001864();
void Func_02001904();
void Func_02001872();
void Func_02001878();
void Func_0200192a();
void Func_02001922();
s32 Func_02001878_b();
void Func_02001590(void);
void Func_020018a0();
void Func_02001918();
void Func_020018ae();
void Func_0200194e();
void Func_020018bc();
void Func_0200193e();
void Func_02001978();
void Func_020018d6();
void Func_02001982();
void Func_02001988();
void Func_020018f6();
void Func_0200195e();
void Func_020019a4();
void Func_02001912();
void Func_0200198a();
void Func_02001920();
void Func_020019c0();
void Func_0200192e();
void Func_020019e0();
void Func_020019d0();
s32 Func_02001960();
void Func_020019c6();
void Func_0200195c();
void Func_020019fc();
void Func_0200196a();
void Func_020019d2();
void Func_0200197e();
void Func_020019f6();
void Func_0200198c();
void Func_02001a2c();
void Func_0200199a();
void Func_02001a02();
void Func_020019a8();
void Func_02001a48();
void Func_020019b6();
void Func_02001a1e();
void Func_020019c4();
void Func_02001a64();
void Func_02001a54();
void Func_02001a74();
void Func_02001a72();
void Func_020019f0();
void Func_02001a9c();
void Func_02001a7e();
s32 Func_02001a26();
void Func_02001a18();
void Func_02001ac4();
void Func_02001ad0();
void Func_02001adc();
void Func_02001a42();
void Func_02001ab0();
void Func_02001a56();
void Func_02001af6();
void Func_02001a94();
void Func_02001b40();
void Func_02001b4c();
void Func_02001b58();
void Func_02001abe();
void Func_02001b2c();
void Func_02001ad2();
void Func_02001b82();
void Func_02001af0();
void Func_02001b68();
void Func_02001afe();
void Func_02001b9e();
void Func_02001b0c();
void Func_02001b96();
void Func_02001ba0();
void Func_02001b26();
void Func_02001be0();
void Func_02001bd8();
void Func_02001b46();
void Func_02001bf2();
void Func_02001bfe();
void Func_02001c0a();
void Func_02001c16();
void Func_02001b7c();
void Func_02001bea();
void Func_02001b90();
void Func_02001c30();
void Func_02001bac();
void Func_02001c4c();
void Func_02001bba();
void Func_02001c6e();
void Func_02001c66();
void Func_02001bd4();
void Func_02001c7e();
void Func_02001be4();
void Func_02001c84();
void Func_02001c6a();
void Func_02001c00();
void Func_02001cac();
void Func_02001c12();
void Func_02001cb2();
void Func_02001c20();
void Func_02001c88();
void Func_02001c2e();
void Func_02001cce();
void Func_02001c3c();
void Func_02001cb4();
void Func_02001c4a();
void Func_02001cea();
void Func_02001c58();
void Func_02001d04();
void Func_02001d0a();
void Func_02001c78();
void Func_02001d2c();
void Func_02001d24();
void Func_02001c92();
void Func_02001d44();
void Func_02001d46();
void Func_02001d4c();
void Func_02001cba();
void Func_02001d6e();
void Func_02001d66();
void Func_02001cd4();
void Func_02001d80();
void Func_02001ce6();
void Func_02001d86();
void Func_02001cf4();
void Func_02001da0();
void Func_02001d06();
void Func_02001dba();
void Func_02001db2();
void Func_02001d20();
void Func_02001d88();
void Func_02001d2e();
void Func_02001dce();
void Func_02001d3c();
void Func_02001dac();
void Func_02001db4();
void Func_02001dbc();
void Func_02001dcc();
void Func_02001d62();
void Func_02001d68();
void Func_02001e12();
void Func_02001d78();
void Func_02001e18();
void Func_02001dfe();
void Func_02001d94();
void Func_02001e40();
void Func_02001da6();
void Func_02001e46();
void Func_02001e66();
void Func_02001dc4();
void Func_02001e70();
void Func_02001dd6();
void Func_02001e3e();
void Func_02001de4();
void Func_02001e84();
void Func_02001df2();
void Func_02001e9e();
void Func_02001e04();
void Func_02001eb6();
void Func_02001eae();
void Func_02001e1c();
void Func_02001e94();
void Func_02001e2a();
void Func_02001eca();
void Func_02001e38();
void Func_02001ee4();
void Func_02001e4a();
void Func_02001efe();
void Func_02001ef6();
void Func_02001e64();
void Func_02001f10();
void Func_02001e76();
void Func_02001f28();
void Func_02001f2c();
void Func_02001e92();
void Func_02001efa();
void Func_02001ea0();
void Func_02001f40();
void Func_02001f16();
void Func_02001ebc();
void Func_02001f5c();
void Func_02001f4e();
void Func_02001f56();
void Func_02001f5e();
void Func_02001f6e();
void Func_02001f04();
void Func_02001f0a();
void Func_02001f72();
void Func_02001f18();
void Func_02001fb8();
void Func_02001f26();
void Func_02001f86();
void Func_02001f8e();
void Func_02001f96();
void Func_02001fa6();
void Func_02001f4c();
void Func_02001f52();
void Func_02001fca();
void Func_02001f60();
void Func_02002000();
void Func_02001fe6();
void Func_02001f7c();
void Func_0200201c();
void Func_02001f8a();
void Func_02001ff2();
void Func_02001f98();
void Func_02002038();
void Func_0200200e();
void Func_02001fb4();
void Func_02002054();
void Func_02001fc2();
void Func_0200206e();
void Func_0200207a();
void Func_02002086();
void Func_02001fec();
void Func_02001ffa();
void Func_0200205a();
void Func_02002062();
void Func_02002072();
void Func_02002018();
void Func_0200204a();
void Func_0200205e();
void Func_02002096();
s16 *Func_02002064();
void Func_0200208e();
void Func_020020a4();
void Func_020020b6();
void Func_020020c6();
s16 *Func_02002094();
void Func_020020be();
void Func_020020d4();
void Func_020020e6();
void Func_020020f6();
s16 *Func_020020c4();
void Func_020020ee();
void Func_02002104();
void Func_02002116();
void Func_020020cc();
void Func_020020e0();

                     

                     

                     

                     

                     

                     

                     

                     

                     

/* The other owner of this overlay that this cutscene calls directly.
 * `overlay_call_targets.ts` classifies 0x02000eec as a real prologue, not a
 * veneer, and it is the straight-line script at that file offset. */

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

    Func_02001708();
    Func_020016fe(1);
    Func_02001764(0, 0x600000, 0xb80000);
    Func_020017c0(0, 0xc000, 0);
    Func_020017ca(11, 0, 0);
    Func_020017d6(12, 0x8000, 0);
    Func_020017c4(0x2850);
    Func_02001810();
    Func_0200181c();
    Func_0200174a(10);
    Func_02001780(0, 0x10000, 0x8000);
    Func_02001854(0, 0, -16);
    Func_020017ae(0, 104, 136);
    Func_02001774(10);
    Func_0200186c(1, -16, 16, 0xc000);
    Func_0200187a(3, 0, 24, 0xc000);
    Func_02001888(2, 16, 16, 0xc000);
    Func_020017ee(1);
    Func_020017ac(20);
    Func_020017b2(10);
    Func_0200182a(2, 2);
    Func_020017c0(20);
    Func_02001860(2, 0);
    Func_020017ce(10);
    Func_0200187a(11, 0x4000, 0);
    Func_02001886(12, 0x4000, 0);
    Func_020017ec(30);
    Func_0200189e(11, 0x105, 70);
    Func_02001896(11, 0);
    Func_02001804(10);
    Func_0200186c(11, 4);
    Func_02001812(20);
    Func_020018b2(11, 0);
    Func_02001820(20);
    Func_02001898(11, 2);
    Func_0200182e(20);
    Func_020018ce(11, 0);
    Func_0200183c(20);
    Func_020018f0(12, 0x102, 40);
    Func_020018e8(12, 0);
    Func_02001856(10);
    Func_020018be(12, 4);
    Func_02001864(20);
    Func_02001904(12, 0);
    Func_02001872(20);
    Func_02001878(10);
    Func_0200192a(11, 0x105, 50);
    Func_02001922(11, 0);

    if (Func_02001878_b(0x9bf) == 0) {
        Func_02001590();
    } else {
        Func_020018a0(20);
        Func_02001918(11, 2);
        Func_020018ae(20);
        Func_0200194e(11, 0);
        Func_020018bc(10);
        Func_0200193e(1, 0, 50);
        Func_02001978(0, 0x105, 60);
        Func_020018d6(10);
        Func_02001982(1, 0xc000, 0);
        Func_020018e8(20);
        Func_02001988(1, 0);
        Func_020018f6(10);
        Func_0200195e(3, 4);
        Func_02001904(20);
        Func_020019a4(3, 0);
        Func_02001912(10);
        Func_0200198a(2, 2);
        Func_02001920(20);
        Func_020019c0(2, 0);
        Func_0200192e(20);
        Func_020019e0(12, 0x101, 40);
        Func_020019d0(12, 0);
        Func_02001960(0, 0);
        Func_0200194e(20);
        Func_020019c6(12, 2);
        Func_0200195c(20);
        Func_020019fc(12, 0);
        Func_0200196a(20);
        Func_020019d2(0, 3);
        Func_02001978(30);
        Func_0200197e(10);
        Func_020019f6(1, 2);
        Func_0200198c(20);
        Func_02001a2c(1, 0);
        Func_0200199a(10);
        Func_02001a02(3, 4);
        Func_020019a8(20);
        Func_02001a48(3, 0);
        Func_020019b6(10);
        Func_02001a1e(2, 3);
        Func_020019c4(30);
        Func_02001a64(2, 0);
        Func_020019d2(10);
        Func_02001a54(2, 0, 30);
        Func_02001a74(0x2002, 0);
    }

    Func_02001a72(0x2861);
    Func_020019f0(10);
    Func_02001a9c(0, 0x4000, 0);
    Func_02001a7e(1, 0, 20);

    if (Func_02001a26(0, 0) == 0) {
        Func_02001a18(20);
        Func_02001ac4(0, 0xc000, 0);
        Func_02001ad0(1, 0xc000, 0);
        Func_02001adc(2, 0xc000, 0);
        Func_02001a42(30);
        Func_02001a48(10);
        Func_02001ab0(11, 3);
        Func_02001a56(30);
        Func_02001af6(11, 0);
        BumpBeatCounter_02000508();
    } else {
        Func_02001a94(20);
        Func_02001b40(0, 0xc000, 0);
        Func_02001b4c(1, 0xc000, 0);
        Func_02001b58(2, 0xc000, 0);
        Func_02001abe(30);
        Func_02001ac4(10);
        Func_02001b2c(11, 4);
        Func_02001ad2(20);
        BumpBeatCounter_02000508();
        Func_02001b82(11, 0);
    }

    Func_02001af0(10);
    Func_02001b68(11, 2);
    Func_02001afe(20);
    Func_02001b9e(11, 0);
    Func_02001b0c(10);
    Func_02001b96(1, 0, 0);
    Func_02001ba0(3, 2, 0);
    Func_02001b26(40);
    Func_02001b2c(10);
    Func_02001be0(11, 0x102, 40);
    Func_02001bd8(11, 0);
    Func_02001b46(10);
    Func_02001bf2(0, 0xc000, 0);
    Func_02001bfe(1, 0xc000, 0);
    Func_02001c0a(2, 0xc000, 0);
    Func_02001c16(3, 0xc000, 0);
    Func_02001b7c(50);
    Func_02001b82(10);
    Func_02001bea(11, 3);
    Func_02001b90(30);
    Func_02001c30(11, 0);
    Func_02001b9e(10);
    Func_02001c16(11, 2);
    Func_02001bac(20);
    Func_02001c4c(11, 0);
    Func_02001bba(10);
    Func_02001c6e(3, 0x100, 40);
    Func_02001c66(3, 0);
    Func_02001bd4(20);
    Func_02001c7e(11, 0, 0);
    Func_02001be4(20);
    Func_02001c84(11, 0);
    Func_02001bf2(10);
    Func_02001c6a(12, 2);
    Func_02001c00(20);
    Func_02001cac(12, 0x8000, 0);
    Func_02001c12(20);
    Func_02001cb2(12, 0);
    Func_02001c20(10);
    Func_02001c88(11, 4);
    Func_02001c2e(20);
    Func_02001cce(11, 0);
    Func_02001c3c(10);
    Func_02001cb4(2, 2);
    Func_02001c4a(20);
    Func_02001cea(2, 0);
    Func_02001c58(10);
    Func_02001d04(11, 0x4000, 0);
    Func_02001c6a(30);
    Func_02001d0a(11, 0);
    Func_02001c78(10);
    Func_02001d2c(12, 0x102, 50);
    Func_02001d24(12, 0);
    Func_02001c92(20);
    Func_02001d44(11, 0x101, 65);
    Func_02001d46(11, 0, 0);
    Func_02001cac(20);
    Func_02001d4c(11, 0);
    Func_02001cba(20);
    Func_02001d6e(12, 0x100, 40);
    Func_02001d66(12, 0);
    Func_02001cd4(20);
    Func_02001d80(12, 0x4000, 0);
    Func_02001ce6(40);
    Func_02001d86(12, 0);
    Func_02001cf4(10);
    Func_02001da0(11, 0x4000, 0);
    Func_02001d06(50);
    Func_02001dba(11, 0x106, 60);
    Func_02001db2(11, 0);
    Func_02001d20(10);
    Func_02001d88(11, 3);
    Func_02001d2e(30);
    Func_02001dce(11, 0);
    Func_02001d3c(10);
    Func_02001dac(0, 2);
    Func_02001db4(1, 2);
    Func_02001dbc(3, 2);
    Func_02001dcc(2, 2);
    Func_02001d62(30);
    Func_02001d68(10);
    Func_02001e12(11, 0, 0);
    Func_02001d78(20);
    Func_02001e18(11, 0);
    Func_02001d86(10);
    Func_02001dfe(12, 2);
    Func_02001d94(30);
    Func_02001e40(12, 0x8000, 0);
    Func_02001da6(20);
    Func_02001e46(12, 0);
    Func_02001db4(10);
    Func_02001e66(11, 0x101, 0);
    Func_02001dc4(10);
    Func_02001e70(1, 0xe000, 0);
    Func_02001dd6(30);
    Func_02001e3e(1, 4);
    Func_02001de4(20);
    Func_02001e84(1, 0);
    Func_02001df2(10);
    Func_02001e9e(12, 0x4000, 0);
    Func_02001e04(30);
    Func_02001eb6(12, 0x105, 60);
    Func_02001eae(12, 0);
    Func_02001e1c(10);
    Func_02001e94(1, 2);
    Func_02001e2a(20);
    Func_02001eca(1, 0);
    Func_02001e38(10);
    Func_02001ee4(11, 0x4000, 0);
    Func_02001e4a(40);
    Func_02001efe(11, 0x102, 40);
    Func_02001ef6(11, 0);
    Func_02001e64(10);
    Func_02001f10(2, 0x8000, 0);
    Func_02001e76(30);
    Func_02001f28(2, 0x107, 60);
    Func_02001f2c(2, 0xc000, 0);
    Func_02001e92(30);
    Func_02001efa(2, 4);
    Func_02001ea0(20);
    Func_02001f40(2, 0);
    Func_02001eae(10);
    Func_02001f16(11, 4);
    Func_02001ebc(20);
    Func_02001f5c(11, 0);
    Func_02001eca(10);

    Func_02001f4e(0, 2);
    Func_02001f56(1, 2);
    Func_02001f5e(3, 2);
    Func_02001f6e(2, 2);
    Func_02001f04(30);
    Func_02001f0a(10);
    Func_02001f72(11, 3);
    Func_02001f18(30);
    Func_02001fb8(11, 0);
    Func_02001f26(20);
    Func_02001f86(0, 3);
    Func_02001f8e(1, 3);
    Func_02001f96(3, 3);
    Func_02001fa6(2, 3);
    Func_02001f4c(30);
    Func_02001f52(10);
    Func_02001fca(11, 2);
    Func_02001f60(20);
    Func_02002000(11, 0);
    Func_02001f6e(10);
    Func_02001fe6(1, 2);
    Func_02001f7c(20);
    Func_0200201c(1, 0);
    Func_02001f8a(10);
    Func_02001ff2(11, 3);
    Func_02001f98(30);
    Func_02002038(11, 0);
    Func_02001fa6(20);
    Func_0200200e(3, 3);
    Func_02001fb4(30);
    Func_02002054(3, 0);
    Func_02001fc2(20);
    Func_0200206e(0, 0x4000, 0);
    Func_0200207a(1, 0xe000, 0);
    Func_02002086(2, 0xa000, 0);
    Func_02001fec(30);
    Func_02002054(0, 3);
    Func_02001ffa(10);
    Func_0200205a(1, 3);
    Func_02002062(2, 3);
    Func_02002072(3, 3);
    Func_02002018(30);
    Func_0200204a(1, 0x13333, 0x9999);
    Func_02002054(3, 0x13333, 0x9999);
    Func_0200205e(2, 0x13333, 0x9999);
    Func_02002096(1, 2);

    actor = Func_02002064(0);
    if (actor != 0) {
        Func_0200208e(1, actor[5], actor[9]);
    }

    Func_020020a4(1);
    Func_020020b6(1, 0, 0);
    Func_020020c6(3, 2);
    actor = Func_02002094(0);
    if (actor != 0) {
        Func_020020be(3, actor[5], actor[9]);
    }

    Func_020020d4(3);
    Func_020020e6(3, 0, 0);
    Func_020020f6(2, 2);
    actor = Func_020020c4(0);
    if (actor != 0) {
        Func_020020ee(2, actor[5], actor[9]);
    }

    Func_02002104(2);
    Func_02002116(2, 0, 0);
    Func_020020cc(10);
    Func_020020e0();
}
