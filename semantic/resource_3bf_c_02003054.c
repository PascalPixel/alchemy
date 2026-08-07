#include "types.h"

/*
 * Resource 3bf long cutscene script at 0x02003054.
 *
 * Complete owner: `push {r5, lr} / sub sp, #8` at 0x02003054 through
 * `add sp, #8 / pop {r5} / pop {r0} / bx r0` at 0x02004630.  r0 holds the
 * popped return address, so the owner is VOID (HANDOVER section 0's
 * interworking-epilogue rule).  The row's 5,604 bytes end at 0x02004638, which
 * is exactly where the already-converted dispatcher at 0x02004638 begins.
 *
 * Five literal pools sit inside the span and are DATA, not code:
 *   0x02003372..0x02003387   0x020039c2..0x020039df   0x02003c82..0x02003c87
 *   0x020040fc..0x02004107   0x0200460c..0x0200461b
 * Each is branched over.  They matter more than usual here, because they
 * disassemble as perfectly plausible register writes — 0x02003384 decodes as
 * `lsls r5, r0, #4`, which would silently overwrite the line cursor if the
 * listing were read as code.  Every argument below was derived with those five
 * ranges excluded.
 *
 * CONTROL FLOW.  Six branches in 5,604 bytes; everything else is straight
 * line.  Three of them test the same query, Func_0808a070(0, 0):
 *   0x02003344  two-way, joining at 0x02003494
 *   0x02003c66  two-way, joining at 0x02003c9e
 *   0x0200434c  one-sided, joining at 0x02004360
 * and three are null checks on the pointer Func_0808a080(0) has just returned.
 *
 * CALL ACCOUNTING.  All 636 `bl` sites were resolved with
 * `bun tools/lib/overlay_call_targets.ts resource_3bf 3054`, never from the
 * disassembler's pc-relative annotations.  They collapse to 34 distinct
 * targets: 33 import veneers (635 sites) and one ordinary prologue of this
 * overlay, 0x02000c90, which already has a byte-exact source under
 * assets/code.  Veneers, each named by the main-image function in its trailing
 * word:
 *   0x020055d0 Func_080091b8 x1    0x020056a0 Func_0808a0f0 x12
 *   0x020055e8 Func_080091e0 x1    0x020056a8 Func_0808a100 x65
 *   0x02005618 Func_080770c8 x2    0x020056b0 Func_0808a110 x11
 *   0x02005628 Func_0808a010 x99   0x020056c0 Func_0808a130 x19
 *   0x02005630 Func_0808a018 x1    0x020056c8 Func_0808a138 x2
 *   0x02005638 Func_0808a020 x1    0x020056d0 Func_0808a148 x79
 *   0x02005648 Func_0808a070 x3    0x020056d8 Func_0808a150 x7
 *   0x02005650 Func_0808a080 x4    0x020056e8 Func_0808a170 x78
 *   0x02005658 Func_0808a090 x18   0x020056f0 Func_0808a178 x3
 *   0x02005678 Func_0808a0b8 x3    0x020056f8 Func_0808a180 x75
 *   0x02005680 Func_0808a0c8 x31   0x02005708 Func_0808a1b8 x24
 *   0x02005690 Func_0808a0e0 x6    0x02005710 Func_0808a1e0 x3
 *   0x02005698 Func_0808a0e8 x32   0x02005718 Func_0808a1e8 x41
 *   0x02005720 Func_0808a1f0 x2    0x02005728 Func_0808a200 x1
 *   0x02005738 Func_0808a210 x2    0x02005758 Func_0808a248 x1
 *   0x02005770 Func_0808a360 x1    0x02005778 Func_0808a368 x1
 *   0x020057a8 Func_080f9010 x6
 * That is 636 sites against the inventory's `calls=577`; as HANDOVER records,
 * the field is neither a site count nor reliable, and the multiset above is
 * the completeness proof.  Every call carries its site address in a trailing
 * comment so the transcription can be checked line by line.
 *
 * THE ONE SIX-ARGUMENT CALL.  Func_080091b8 at 0x02003074 is the only site in
 * this owner with stack-carried arguments: `movs r3,#26 / movs r2,#55 /
 * str r3,[sp,#0] / str r2,[sp,#4]` fills the 8-byte frame the prologue
 * reserved, and r0-r3 are then reloaded with 16, 75, 7 and 4.  Missing those
 * two is exactly the decompiler defect HANDOVER section 0 rule 3 warns about,
 * so they are spelled out.  The frame is used for nothing else.
 *
 * LINE CURSOR.  r5 is a text-line base reloaded three times — 0x2481 at
 * 0x02003174, 0x248e at 0x020037be and 0x24a6 at 0x02003d00 — with each line
 * reached as `r5 + n`.  The line ids are written below as absolute constants.
 * As in the sibling scripts 0x02001e94 and 0x020027b0, each Func_0808a170 line
 * is followed by a Func_0808a180(actor, 0) wait.
 *
 * COORDINATE VIEW.  The three null-checked blocks read `ldrsh [ptr, #0x0a]`
 * and `ldrsh [ptr, #0x12]`.  Those are the documented halfword views of the
 * 16.16 words at +0x08 and +0x10 — the integer parts of the X and Z
 * coordinates — so the pointer is typed s16 * and indexed [5] and [9] rather
 * than given a speculative struct.  Both reads are SIGNED.
 *
 * Link base.  None of this owner's pool words falls in the proven in-image
 * band (0x0200_8xxx and above), so all are ordinary values; 0x03001ebc is the
 * IWRAM workspace pointer the rest of this overlay loads.  The stored word at
 * workspace + 448 is 515 here (built as `224 << 1` for the offset then `+ 67`
 * on the same register for the value), against 512 in 0x02001e94/0x020027b0
 * and 516 in 0x02004da0 — the three scripts hand off different phases.
 *
 * Uncertainties:
 *  - Func_0808a018, Func_0808a020 and Func_0808a368 are each called with no
 *    argument register set; they are called with none here and the leftovers
 *    are noted rather than asserted as parameters.
 *  - Func_080091e0's first argument is the pointer Func_0808a080 returned in
 *    r0; the assembly does not reload r0 between the two branches.
 *  - Func_0808a080 is called with kind 12 once and kind 0 three times, and its
 *    result is used as an opaque handle in the first case and as the s16
 *    coordinate view in the other three.  One declaration covers both, which
 *    is why it is old-style.
 */

/* Old-style declarations are mandatory in overlay sources: one name can be
 * reached with different argument counts from different sites. */
void Func_02000c90();
void Func_080091b8();
void Func_080091e0();
void Func_080770c8();
void Func_0808a010();
void Func_0808a018();
void Func_0808a020();
s32 Func_0808a070();
s32 Func_0808a080();
void Func_0808a090();
void Func_0808a0b8();
void Func_0808a0c8();
void Func_0808a0e0();
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
void Func_0808a1e0();
void Func_0808a1e8();
void Func_0808a1f0();
void Func_0808a200();
void Func_0808a210();
void Func_0808a248();
void Func_0808a360();
void Func_0808a368();
void Func_080f9010();


void Func_02003054(void)
{
    s16 *actor;

    Func_080770c8(0x301);   /* 200305a */
    Func_080770c8(0x941);   /* 2003060 */

    /* Six-argument call: r0-r3 plus 26 and 55 spilled to sp+0 and sp+4 by the
     * `str r3,[sp,#0] / str r2,[sp,#4]` pair at 0x02003068. */
    Func_080091b8(0x10, 0x4b, 7, 4, 26, 55);   /* 2003074 */
    Func_02000c90(4);   /* 200307a */
    Func_0808a018();   /* 200307e */
    actor = Func_0808a080(0xc);   /* 2003084 */
    Func_080091e0(actor, 1);   /* 200308a */
    Func_0808a0f0(0, 0x1c80000, 0xb80000);   /* 2003098 */
    Func_0808a1b8(0, 0x4000, 0);   /* 20030a4 */
    Func_0808a0f0(1, 0x1b80000, 0xc00000);   /* 20030b2 */
    Func_0808a1b8(1, 0x4000, 0);   /* 20030be */
    Func_0808a0f0(3, 0x1e80000, 0xb80000);   /* 20030cc */
    Func_0808a1b8(3, 0x4000, 0);   /* 20030d8 */
    Func_0808a0f0(2, 0x1d80000, 0xb80000);   /* 20030e6 */
    Func_0808a1b8(2, 0x4000, 0);   /* 20030f2 */
    Func_0808a0f0(0xc, 0x2080000, 0xe00000);   /* 2003100 */
    Func_0808a1b8(0xc, 0xb000, 0);   /* 200310c */
    Func_0808a0f0(0xb, 0x1c00000, 0xed0000);   /* 200311a */
    Func_0808a1b8(0xb, 0x8000, 0);   /* 2003126 */
    Func_0808a0f0(0xf, 0x1c00000, 0xee0000);   /* 2003134 */
    Func_0808a1e0(0xf, 3);   /* 200313c */
    Func_0808a100(0xb, 3);   /* 2003144 */
    Func_0808a0f0(0xd, 0x1ca0000, 0xf30000);   /* 2003152 */
    Func_0808a1b8(0xd, 0x4000, 0);   /* 200315e */
    Func_0808a100(0xd, 5);   /* 2003166 */
    Func_0808a360();   /* 200316a */
    Func_0808a010(0x78);   /* 2003170 */
    Func_0808a170(0x2481);   /* 2003178 */
    Func_0808a180(0xd, 0);   /* 2003180 */
    Func_0808a1e8(1, 0x101, 0x3c);   /* 200318a */
    Func_0808a170(0x2482);   /* 2003190 */
    Func_0808a180(1, 0);   /* 2003198 */
    Func_0808a1e8(0, 0x101, 1);   /* 20031a2 */
    Func_0808a1e8(2, 0x101, 1);   /* 20031ac */
    Func_0808a1e8(3, 0x101, 1);   /* 20031b6 */
    Func_0808a010(0x3c);   /* 20031bc */
    Func_0808a148(0xc, 0xd, 0);   /* 20031c6 */
    Func_0808a010(0x3c);   /* 20031cc */
    Func_0808a170(0x2483);   /* 20031d2 */
    Func_0808a180(0xc, 0);   /* 20031da */
    Func_0808a148(0, 0xc, 0);   /* 20031e4 */
    Func_0808a148(1, 0xc, 0);   /* 20031ee */
    Func_0808a148(2, 0xc, 0);   /* 20031f8 */
    Func_0808a148(3, 0xc, 0);   /* 2003202 */
    Func_0808a010(0x3c);   /* 2003208 */
    Func_0808a0c8(0xc, 0x200, 0xe8);   /* 2003214 */
    Func_0808a100(0xc, 4);   /* 200321c */
    Func_0808a010(0x3c);   /* 2003222 */
    Func_0808a170(0x2484);   /* 2003228 */
    Func_0808a180(0xc, 0);   /* 2003230 */
    Func_0808a010(0xf);   /* 2003236 */
    Func_0808a130(0xd, 2);   /* 200323e */
    Func_0808a170(0x2485);   /* 2003244 */
    Func_0808a180(0xd, 0);   /* 200324c */
    Func_0808a148(0, 0xd, 0);   /* 2003256 */
    Func_0808a148(2, 0xd, 0);   /* 2003260 */
    Func_0808a148(3, 0xd, 0);   /* 200326a */
    Func_0808a148(1, 0xd, 0);   /* 2003274 */
    Func_0808a130(2, 1);   /* 200327c */
    Func_0808a170(0x2486);   /* 2003282 */
    Func_0808a180(2, 0);   /* 200328a */
    Func_0808a1e8(3, 0x105, 0);   /* 2003294 */
    Func_0808a100(3, 4);   /* 200329c */
    Func_0808a010(0x50);   /* 20032a2 */
    Func_0808a100(3, 1);   /* 20032aa */
    Func_0808a170(0x2487);   /* 20032b0 */
    Func_0808a180(3, 0);   /* 20032b8 */
    Func_0808a1e8(0xc, 0x102, 0x41);   /* 20032c4 */
    Func_0808a130(0xc, 2);   /* 20032cc */
    Func_0808a010(0x64);   /* 20032d2 */
    Func_0808a170(0x2488);   /* 20032d8 */
    Func_0808a180(0xd, 0);   /* 20032e0 */
    Func_0808a148(1, 0, 0);   /* 20032ea */
    Func_0808a010(0xa);   /* 20032f0 */
    Func_0808a170(0x2489);   /* 20032f8 */
    Func_0808a180(1, 0);   /* 2003300 */
    Func_0808a130(2, 1);   /* 2003308 */
    Func_0808a010(0x1e);   /* 200330e */
    Func_0808a170(0x248a);   /* 2003316 */
    Func_0808a180(2, 0);   /* 200331e */
    Func_0808a148(3, 0, 0);   /* 2003328 */
    Func_0808a170(0x248b);   /* 2003330 */
    Func_0808a178(3, 0);   /* 2003338 */

    if (Func_0808a070(0, 0) != 0) {   /* 2003340 */
        Func_0808a100(0x248c);   /* 200338c */
        Func_0808a100(3, 0);   /* 2003394 */
        Func_0808a100(0, 0xc, 0);   /* 200339e */
        Func_0808a148(1, 0xc, 0);   /* 20033a8 */
        Func_0808a010(2, 0xc, 0);   /* 20033b2 */
        Func_0808a170(3, 0xc, 0);   /* 20033bc */
        Func_0808a180(0x14);   /* 20033c2 */
        Func_0808a148(0xc, 0, 0);   /* 20033cc */
        Func_0808a148(0x3c);   /* 20033d2 */
        Func_0808a148(0xc, 0xd, 0);   /* 20033dc */
        Func_0808a148(0x50);   /* 20033e2 */
        Func_0808a010(0xc, 3);   /* 20033ea */
        Func_0808a148(0x1e);   /* 20033f0 */
        Func_0808a010(0xc, 0, 0);   /* 20033fa */
        Func_0808a148(0x248d);   /* 2003402 */
        Func_0808a010(0xc, 0);   /* 200340a */
        Func_0808a110(0x3c);   /* 2003410 */
        Func_0808a010(2, 0xc, 0);   /* 200341a */
        Func_0808a148(1, 0xc, 0);   /* 2003424 */
        Func_0808a170(3, 0xc, 0);   /* 200342e */
        Func_0808a180(0, 0xc, 0);   /* 2003438 */
        Func_0808a010(2, 3);   /* 2003440 */
        Func_0808a148(1, 3);   /* 2003448 */
        Func_0808a148(3, 3);   /* 2003450 */
        Func_0808a148(0, 3);   /* 2003458 */
        Func_0808a148(0x50);   /* 200345e */
        Func_0808a100(0xc, 0xd, 0);   /* 2003468 */
        Func_0808a100(2, 0xd, 0);   /* 2003472 */
        Func_0808a100(1, 0xd, 0);   /* 200347c */
        Func_0808a100(3, 0xd, 0);   /* 2003486 */
        Func_0808a010(0, 0xd, 0);   /* 2003490 */
    } else {
    }
        Func_0808a148(2, 3);   /* 200334c */
        Func_0808a148(1, 3);   /* 2003354 */
        Func_0808a148(3, 3);   /* 200335c */
        Func_0808a148(0xc, 0xd, 0);   /* 2003366 */
        Func_0808a148(0x3c);   /* 200336c */

    Func_0808a090(0, 0x8000, 0x4000);   /* 200349e */
    Func_0808a090(1, 0x8000, 0x4000);   /* 20034ac */
    Func_0808a090(3, 0x8000, 0x4000);   /* 20034ba */
    Func_0808a090(2, 0x8000, 0x4000);   /* 20034c8 */
    Func_0808a0c8(1, 0x1a0, 0xd8);   /* 20034d4 */
    Func_0808a0e8(1);   /* 20034da */
    Func_0808a0c8(1, 0x1a0, 0xf8);   /* 20034e6 */
    Func_0808a0e8(1);   /* 20034ec */
    Func_0808a0c8(1, 0x1b8, 0xf8);   /* 20034f8 */
    Func_0808a0e8(1);   /* 20034fe */
    Func_0808a1b8(1, 0xc000, 0);   /* 200350a */
    Func_0808a0c8(0, 0x1b8, 0xd8);   /* 2003516 */
    Func_0808a0e8(0);   /* 200351c */
    Func_0808a1b8(0, 0, 0);   /* 2003526 */
    Func_0808a0c8(3, 0x1e8, 0xf8);   /* 2003532 */
    Func_0808a0e8(3);   /* 2003538 */
    Func_0808a0c8(3, 0x1c8, 0xf8);   /* 2003544 */
    Func_0808a0e8(3);   /* 200354a */
    Func_0808a1b8(3, 0xc000, 0);   /* 2003556 */
    Func_0808a0c8(2, 0x1c8, 0xd8);   /* 2003562 */
    Func_0808a0e8(2);   /* 2003568 */
    Func_0808a1b8(2, 0x8000, 0);   /* 2003574 */
    Func_0808a150(1, 0, 0);   /* 200357e */
    Func_0808a150(3, 2, 0);   /* 2003588 */
    Func_0808a010(0x3c);   /* 200358e */
    Func_0808a110(0, 3);   /* 2003596 */
    Func_0808a100(2, 3);   /* 200359e */
    Func_0808a100(1, 3);   /* 20035a6 */
    Func_0808a100(3, 3);   /* 20035ae */
    Func_0808a010(0x64);   /* 20035b4 */
    Func_080f9010(0xe2);   /* 20035ba */
    Func_0808a100(0xd, 7);   /* 20035c2 */
    Func_0808a090(0, 0x1999, 0xccc);   /* 20035cc */
    Func_0808a0e0(0, -24, 0);   /* 20035d8 */
    Func_0808a090(1, 0x1999, 0xccc);   /* 20035e2 */
    Func_0808a0e0(1, -24, 0);   /* 20035ee */
    Func_0808a090(3, 0x1999, 0xccc);   /* 20035f8 */
    Func_0808a0e0(3, -24, 0);   /* 2003604 */
    Func_0808a090(2, 0x1999, 0xccc);   /* 200360e */
    Func_0808a0e0(2, -24, 0);   /* 200361a */
    Func_0808a090(0xb, 0x1999, 0xccc);   /* 2003624 */
    Func_0808a090(0xf, 0x1999, 0xccc);   /* 200362e */
    Func_0808a0e0(0xb, -24, 0);   /* 200363a */
    Func_0808a0e0(0xf, -24, 0);   /* 2003646 */
    Func_0808a0e8(0);   /* 200364c */
    Func_080f9010(0x120);   /* 2003654 */
    Func_0808a010(0x3c);   /* 200365a */
    Func_0808a1b8(0, 0x4000, 0);   /* 2003666 */
    Func_0808a1b8(1, 0xc000, 0);   /* 2003672 */
    Func_0808a1b8(2, 0x4000, 0);   /* 200367e */
    Func_0808a1b8(3, 0xc000, 0);   /* 200368a */
    Func_0808a010(0x3c);   /* 2003690 */
    Func_0808a100(0, 3);   /* 2003698 */
    Func_0808a100(1, 3);   /* 20036a0 */
    Func_0808a100(2, 3);   /* 20036a8 */
    Func_0808a100(3, 3);   /* 20036b0 */
    Func_0808a010(0x64);   /* 20036b6 */
    Func_0808a090(0, 0xb333, 0x5999);   /* 20036c0 */
    Func_0808a090(1, 0xb333, 0x5999);   /* 20036ca */
    Func_0808a090(2, 0xb333, 0x5999);   /* 20036d4 */
    Func_0808a090(3, 0xb333, 0x5999);   /* 20036de */
    Func_0808a0c8(0, 0x1c8, 0xb8);   /* 20036ea */
    Func_0808a0e8(0);   /* 20036f0 */
    Func_0808a100(0, 1);   /* 20036f8 */
    Func_0808a1b8(0, 0x4000, 0);   /* 2003704 */
    Func_0808a0c8(1, 0x1d0, 0x100);   /* 2003712 */
    Func_0808a0e8(1);   /* 2003718 */
    Func_0808a0c8(1, 0x1e0, 0xf8);   /* 2003724 */
    Func_0808a0e8(1);   /* 200372a */
    Func_0808a0c8(1, 0x1b8, 0xc0);   /* 2003736 */
    Func_0808a0e8(1);   /* 200373c */
    Func_0808a100(1, 1);   /* 2003744 */
    Func_0808a1b8(1, 0x4000, 0);   /* 2003750 */
    Func_0808a0c8(3, 0x1e8, 0xf8);   /* 200375c */
    Func_0808a0e8(3);   /* 2003762 */
    Func_0808a0c8(3, 0x1e8, 0xb8);   /* 200376e */
    Func_0808a0c8(2, 0x1d8, 0xb8);   /* 200377a */
    Func_0808a0e8(2);   /* 2003780 */
    Func_0808a100(2, 1);   /* 2003788 */
    Func_0808a1b8(2, 0x4000, 0);   /* 2003794 */
    Func_0808a0e8(3);   /* 200379a */
    Func_0808a100(3, 1);   /* 20037a2 */
    Func_0808a1b8(3, 0x4000, 0);   /* 20037ae */
    Func_0808a010(0x1e);   /* 20037b4 */
    Func_0808a010(0x3c);   /* 20037ba */
    Func_0808a170(0x248e);   /* 20037c2 */
    Func_0808a180(1, 0);   /* 20037ca */
    Func_0808a100(0xd, 6);   /* 20037d2 */
    Func_0808a010(0x78);   /* 20037d8 */
    Func_0808a130(0xd, 2);   /* 20037e0 */
    Func_0808a010(0x3c);   /* 20037e6 */
    Func_0808a100(0xd, 7);   /* 20037ee */
    Func_0808a170(0x248f);   /* 20037f4 */
    Func_0808a180(0xd, 0);   /* 20037fc */
    Func_0808a010(0x14);   /* 2003802 */
    Func_0808a150(3, 2, 0);   /* 200380c */
    Func_0808a010(0xa);   /* 2003812 */
    Func_0808a1e8(3, 0x101, 0x50);   /* 200381c */
    Func_0808a148(3, 0xd, 0);   /* 2003826 */
    Func_0808a148(2, 0xd, 0);   /* 2003830 */
    Func_0808a170(0x2490);   /* 2003836 */
    Func_0808a180(3, 0);   /* 200383e */
    Func_0808a1e8(0xd, 0x102, 0x46);   /* 200384a */
    Func_0808a130(0xd, 2);   /* 2003852 */
    Func_0808a010(0x3c);   /* 2003858 */
    Func_0808a100(0xd, 5);   /* 2003860 */
    Func_0808a010(0x46);   /* 2003866 */
    Func_0808a170(0x2491);   /* 200386c */
    Func_0808a180(0xd, 0);   /* 2003874 */
    Func_0808a100(0xd, 7);   /* 200387c */
    Func_0808a1e8(2, 0x108, 0x28);   /* 2003888 */
    Func_0808a170(0x2492);   /* 200388e */
    Func_0808a180(2, 0);   /* 2003896 */
    Func_0808a110(0xc, 3);   /* 200389e */
    Func_0808a1e8(1, 0x103, 0x3c);   /* 20038a8 */
    Func_0808a0c8(1, 0x1b8, 0xd0);   /* 20038b4 */
    Func_0808a0e8(1);   /* 20038ba */
    Func_0808a148(1, 0, 0);   /* 20038c4 */
    Func_0808a170(0x2493);   /* 20038ca */
    Func_0808a180(1, 0);   /* 20038d2 */
    Func_0808a010(0x14);   /* 20038d8 */
    Func_0808a1e8(0, 0x101, 0);   /* 20038e2 */
    Func_0808a1e8(2, 0x101, 0);   /* 20038ec */
    Func_0808a1e8(3, 0x101, 0);   /* 20038f6 */
    Func_0808a1e8(0xc, 0x101, 0);   /* 2003900 */
    Func_0808a010(0x46);   /* 2003906 */
    Func_0808a148(1, 2, 0);   /* 2003910 */
    Func_0808a1e8(1, 0x102, 0x4b);   /* 200391c */
    Func_0808a148(1, 0, 0);   /* 2003926 */
    Func_0808a100(1, 4);   /* 200392e */
    Func_0808a170(0x2494);   /* 2003934 */
    Func_0808a180(1, 0);   /* 200393c */
    Func_0808a1e8(0xc, 0x101, 0);   /* 2003946 */
    Func_0808a010(0x3c);   /* 200394c */
    Func_0808a170(0x2495);   /* 2003952 */
    Func_0808a180(0xc, 0);   /* 200395a */
    Func_0808a0c8(1, 0x1c0, 0xd0);   /* 2003966 */
    Func_0808a0e8(1);   /* 200396c */
    Func_0808a100(1, 1);   /* 2003974 */
    Func_0808a148(1, 0xc, 0);   /* 200397e */
    Func_0808a170(0x2496);   /* 2003986 */
    Func_0808a180(1, 0);   /* 200398e */
    Func_0808a130(0xc, 1);   /* 2003996 */
    Func_0808a010(0x3c);   /* 200399c */
    Func_0808a148(1, 0xc, 0);   /* 20039a6 */
    Func_0808a010(0x3c);   /* 20039ac */
    Func_0808a170(0x2497);   /* 20039b4 */
    Func_0808a180(0xc, 0);   /* 20039bc */
    Func_0808a100(1, 4);   /* 20039e4 */
    Func_0808a010(0x3c);   /* 20039ea */
    Func_0808a170(0x2498);   /* 20039f2 */
    Func_0808a180(1, 0);   /* 20039fa */
    Func_0808a130(2, 1);   /* 2003a02 */
    Func_0808a010(0x1e);   /* 2003a08 */
    Func_0808a170(0x2499);   /* 2003a10 */
    Func_0808a180(2, 0);   /* 2003a18 */
    Func_0808a148(1, 3, 0);   /* 2003a22 */
    Func_0808a010(0x14);   /* 2003a28 */
    Func_0808a170(0x249a);   /* 2003a30 */
    Func_0808a180(1, 0);   /* 2003a38 */
    Func_0808a010(0x1e);   /* 2003a3e */
    Func_0808a1e8(3, 0x100, 0x50);   /* 2003a4a */
    Func_0808a148(3, 1, 0);   /* 2003a54 */
    Func_0808a170(0x249b);   /* 2003a5c */
    Func_0808a180(3, 0);   /* 2003a64 */
    Func_0808a148(3, 0xd, 0);   /* 2003a6e */
    Func_0808a170(0x249c);   /* 2003a76 */
    Func_0808a180(3, 0);   /* 2003a7e */
    Func_0808a148(0, 0xd, 0);   /* 2003a88 */
    Func_0808a148(2, 0xd, 0);   /* 2003a92 */
    Func_0808a148(1, 0xd, 0);   /* 2003a9c */
    Func_0808a148(3, 0xd, 0);   /* 2003aa6 */
    Func_0808a010(0x78);   /* 2003aac */
    Func_0808a1e8(0xd, 0x102, 0x1e);   /* 2003ab8 */
    Func_0808a130(0xd, 1);   /* 2003ac0 */
    Func_0808a010(0x78);   /* 2003ac6 */
    Func_0808a130(0xc, 1);   /* 2003ace */
    Func_0808a010(0x3c);   /* 2003ad4 */
    Func_0808a170(0x249d);   /* 2003adc */
    Func_0808a180(0xc, 0);   /* 2003ae4 */
    Func_0808a1e8(1, 0x107, 0x6e);   /* 2003aee */
    Func_0808a0c8(1, 0x1c8, 0xd4);   /* 2003afa */
    Func_0808a0e8(1);   /* 2003b00 */
    Func_0808a100(1, 1);   /* 2003b08 */
    Func_0808a170(0x249e);   /* 2003b10 */
    Func_0808a180(1, 0);   /* 2003b18 */
    Func_0808a148(2, 1, 0);   /* 2003b22 */
    Func_0808a010(0x3c);   /* 2003b28 */
    Func_0808a130(2, 1);   /* 2003b30 */
    Func_0808a170(0x249f);   /* 2003b38 */
    Func_0808a180(2, 0);   /* 2003b40 */
    Func_0808a148(1, 0xd, 0);   /* 2003b4a */
    Func_0808a010(0x50);   /* 2003b50 */
    Func_0808a148(1, 2, 0);   /* 2003b5a */
    Func_0808a010(0x3c);   /* 2003b60 */
    Func_0808a148(1, 0xd, 0);   /* 2003b6a */
    Func_0808a010(0x1e);   /* 2003b70 */
    Func_0808a148(0, 0xd, 0);   /* 2003b7a */
    Func_0808a148(3, 0xd, 0);   /* 2003b84 */
    Func_0808a148(0xd, 0xd, 0);   /* 2003b8e */
    Func_0808a148(0xc, 0xd, 0);   /* 2003b98 */
    Func_0808a010(0x50);   /* 2003b9e */
    Func_0808a110(1, 3);   /* 2003ba6 */
    Func_0808a010(0x1e);   /* 2003bac */
    Func_0808a170(0x24a0);   /* 2003bb4 */
    Func_0808a180(1, 0);   /* 2003bbc */
    Func_0808a130(0xd, 2);   /* 2003bc4 */
    Func_0808a010(0x46);   /* 2003bca */
    Func_0808a170(0x24a1);   /* 2003bd2 */
    Func_0808a180(0xd, 0);   /* 2003bda */
    Func_0808a1e8(3, 0x100, 0x3c);   /* 2003be6 */
    Func_0808a170(0x24a2);   /* 2003bee */
    Func_0808a180(3, 0);   /* 2003bf6 */
    Func_0808a148(2, 0, 0);   /* 2003c00 */
    Func_0808a010(0x50);   /* 2003c06 */
    Func_0808a170(0x24a3);   /* 2003c0e */
    Func_0808a178(2, 0);   /* 2003c16 */
    Func_0808a148(1, 0, 0);   /* 2003c20 */
    Func_0808a148(3, 0, 0);   /* 2003c2a */
    Func_0808a148(2, 0, 0);   /* 2003c34 */
    Func_0808a148(0xc, 0, 0);   /* 2003c3e */
    Func_0808a0f0(0xe, 0x1c80000, 0x1300000);   /* 2003c4c */
    Func_0808a090(0xe, 0x8000, 0x4000);   /* 2003c5a */

    if (Func_0808a070(0, 0) != 0) {   /* 2003c62 */
        Func_080f9010(0x13);   /* 2003c8a */
        Func_0808a170(0x24a5);   /* 2003c92 */
        Func_0808a180(0xe, 0);   /* 2003c9a */
    } else {
    }
        Func_080f9010(0x13);   /* 2003c6c */
        Func_0808a170(0x24a4);   /* 2003c74 */
        Func_0808a180(0xe, 0);   /* 2003c7c */

    Func_0808a148(0, 0xe, 0);   /* 2003ca4 */
    Func_0808a148(1, 0xe, 0);   /* 2003cae */
    Func_0808a148(3, 0xe, 0);   /* 2003cb8 */
    Func_0808a148(2, 0xe, 0);   /* 2003cc2 */
    Func_0808a148(0xc, 0xe, 0);   /* 2003ccc */
    Func_0808a148(0xd, 0xe, 0);   /* 2003cd6 */
    Func_0808a210(0x1c80000, -1, 0xf00000, 1);   /* 2003ce8 */
    Func_0808a130(0xd, 1);   /* 2003cf0 */
    Func_0808a010(0x3c);   /* 2003cf6 */
    Func_080f9010(8);   /* 2003cfc */
    Func_0808a170(0x24a6);   /* 2003d04 */
    Func_0808a180(0xd, 0);   /* 2003d0c */
    Func_0808a0c8(0xe, 0x1c8, 0x118);   /* 2003d1a */
    Func_0808a0e8(0xe);   /* 2003d20 */
    Func_0808a0c8(0xe, 0x1b8, 0x100);   /* 2003d2e */
    Func_0808a210(0x1c80000, -1, 0xe00000, 1);   /* 2003d40 */
    Func_0808a170(0x24a7);   /* 2003d46 */
    Func_0808a180(0xe, 0);   /* 2003d4e */
    Func_0808a0e8(0xe);   /* 2003d54 */
    Func_0808a1b8(0xe, 0xd000, 0);   /* 2003d60 */
    Func_0808a1e8(0xd, 0x102, 0x50);   /* 2003d6c */
    Func_0808a100(0xe, 4);   /* 2003d74 */
    Func_0808a010(0x59);   /* 2003d7a */
    Func_0808a170(0x24a8);   /* 2003d80 */
    Func_0808a180(0xe, 0);   /* 2003d88 */
    Func_0808a138(0xd, 2);   /* 2003d90 */
    Func_0808a110(0xe, 3);   /* 2003d98 */
    Func_0808a170(0x24a9);   /* 2003d9e */
    Func_0808a180(0xe, 0);   /* 2003da6 */
    Func_0808a1e8(0xd, 0x100, 0x50);   /* 2003db2 */
    Func_0808a100(0xe, 4);   /* 2003dba */
    Func_0808a010(0x50);   /* 2003dc0 */
    Func_0808a1b8(0xe, 0x3000, 0);   /* 2003dcc */
    Func_0808a010(0x14);   /* 2003dd2 */
    Func_0808a170(0x24aa);   /* 2003dd8 */
    Func_0808a180(0xe, 0);   /* 2003de0 */
    Func_0808a130(0xd, 1);   /* 2003de8 */
    Func_0808a170(0x24ab);   /* 2003dee */
    Func_0808a180(0xd, 0);   /* 2003df6 */
    Func_0808a010(0x1e);   /* 2003dfc */
    Func_0808a110(0xe, 3);   /* 2003e04 */
    Func_0808a148(0xe, 0xd, 0);   /* 2003e0e */
    Func_0808a010(0x14);   /* 2003e14 */
    Func_0808a170(0x24ac);   /* 2003e1a */
    Func_0808a180(0xe, 0);   /* 2003e22 */
    Func_0808a130(0xd, 1);   /* 2003e2a */
    Func_0808a1e8(0xd, 0x102, 0x50);   /* 2003e36 */
    Func_0808a170(0x24ad);   /* 2003e3c */
    Func_0808a180(0xd, 0);   /* 2003e44 */
    Func_0808a1e8(0xe, 0x103, 0x3c);   /* 2003e4e */
    Func_0808a170(0x24ae);   /* 2003e56 */
    Func_0808a180(0xe, 0);   /* 2003e5e */
    Func_0808a110(0xe, 3);   /* 2003e66 */
    Func_0808a170(0x24af);   /* 2003e6e */
    Func_0808a180(0xe, 0);   /* 2003e76 */
    Func_0808a010(0x14);   /* 2003e7c */
    Func_0808a1f0(0xd, 0x101);   /* 2003e84 */
    Func_0808a010(0x50);   /* 2003e8a */
    Func_0808a1f0(0xd, 0);   /* 2003e92 */
    Func_0808a0c8(0xe, 0x1f0, 0xf0);   /* 2003e9e */
    Func_0808a0e8(0xe);   /* 2003ea4 */
    Func_0808a148(0xe, 0xc, 0);   /* 2003eae */
    Func_0808a010(0x14);   /* 2003eb4 */
    Func_0808a148(0xc, 0xe, 0);   /* 2003ebe */
    Func_0808a170(0x24b0);   /* 2003ec6 */
    Func_0808a180(0xe, 0);   /* 2003ece */
    Func_0808a130(0xc, 1);   /* 2003ed6 */
    Func_0808a170(0x24b1);   /* 2003ede */
    Func_0808a180(0xc, 0);   /* 2003ee6 */
    Func_0808a010(0x28);   /* 2003eec */
    Func_0808a110(0xe, 3);   /* 2003ef4 */
    Func_0808a010(0x14);   /* 2003efa */
    Func_0808a170(0x24b2);   /* 2003f02 */
    Func_0808a180(0xe, 0);   /* 2003f0a */
    Func_0808a100(0xe, 3);   /* 2003f12 */
    Func_0808a1e8(0xc, 0x102, 0x3c);   /* 2003f1e */
    Func_0808a170(0x24b3);   /* 2003f26 */
    Func_0808a180(0xc, 0);   /* 2003f2e */
    Func_0808a1e8(0xe, 0x100, 0x46);   /* 2003f3a */
    Func_0808a170(0x24b4);   /* 2003f42 */
    Func_0808a180(0xe, 0);   /* 2003f4a */
    Func_0808a100(0xc, 3);   /* 2003f52 */
    Func_0808a010(0x8c);   /* 2003f58 */
    Func_0808a100(0xe, 3);   /* 2003f60 */
    Func_0808a010(0x78);   /* 2003f66 */
    Func_0808a150(0, 0xe, 0);   /* 2003f70 */
    Func_0808a100(0, 3);   /* 2003f78 */
    Func_0808a100(2, 3);   /* 2003f80 */
    Func_0808a100(1, 3);   /* 2003f88 */
    Func_0808a100(3, 3);   /* 2003f90 */
    Func_0808a010(0x78);   /* 2003f96 */
    Func_0808a1e8(0xe, 0x108, 0xb4);   /* 2003fa2 */
    Func_0808a170(0x24b5);   /* 2003faa */
    Func_0808a180(0xe, 0);   /* 2003fb2 */
    Func_0808a1e8(0xc, 0x101, 0x50);   /* 2003fbc */
    Func_0808a170(0x24b6);   /* 2003fc4 */
    Func_0808a180(0xc, 0);   /* 2003fcc */
    Func_0808a150(0, 1, 0);   /* 2003fd6 */
    Func_0808a150(3, 2, 0);   /* 2003fe0 */
    Func_0808a010(0x50);   /* 2003fe6 */
    Func_0808a148(0xe, 0, 0);   /* 2003ff0 */
    Func_0808a010(0x3c);   /* 2003ff6 */
    Func_0808a148(0, 0xe, 0);   /* 2004000 */
    Func_0808a148(1, 0xe, 0);   /* 200400a */
    Func_0808a148(2, 0xe, 0);   /* 2004014 */
    Func_0808a148(3, 0xe, 0);   /* 200401e */
    Func_0808a010(0x3c);   /* 2004024 */
    Func_0808a170(0x24b7);   /* 200402c */
    Func_0808a180(0xe, 0);   /* 2004034 */
    Func_0808a1e8(0, 0x101, 0);   /* 200403e */
    Func_0808a1e8(1, 0x101, 0);   /* 2004048 */
    Func_0808a1e8(2, 0x101, 0);   /* 2004052 */
    Func_0808a1e8(3, 0x101, 0);   /* 200405c */
    Func_0808a010(0x64);   /* 2004062 */
    Func_0808a170(0x24b8);   /* 200406a */
    Func_0808a180(1, 0);   /* 2004072 */
    Func_0808a130(2, 1);   /* 200407a */
    Func_0808a010(0x3c);   /* 2004080 */
    Func_0808a170(0x24b9);   /* 2004088 */
    Func_0808a180(2, 0);   /* 2004090 */
    Func_0808a148(0xe, 2, 0);   /* 200409a */
    Func_0808a010(0x14);   /* 20040a0 */
    Func_0808a170(0x24ba);   /* 20040a8 */
    Func_0808a180(0xe, 0);   /* 20040b0 */
    Func_0808a1e8(0, 0x101, 0);   /* 20040ba */
    Func_0808a1e8(1, 0x101, 0);   /* 20040c4 */
    Func_0808a1e8(2, 0x101, 0);   /* 20040ce */
    Func_0808a1e8(3, 0x101, 0);   /* 20040d8 */
    Func_0808a010(0x78);   /* 20040de */
    Func_0808a100(0xe, 4);   /* 20040e6 */
    Func_0808a010(0x78);   /* 20040ec */
    Func_0808a170(0x24bb);   /* 20040f4 */
    Func_0808a180(0xe, 0);   /* 200410a */
    Func_0808a1e8(0xe, 0x102, 0x5a);   /* 2004116 */
    Func_0808a170(0x24bc);   /* 200411e */
    Func_0808a180(0xe, 0);   /* 2004126 */
    Func_0808a100(0, 3);   /* 200412e */
    Func_0808a100(2, 3);   /* 2004136 */
    Func_0808a100(3, 3);   /* 200413e */
    Func_0808a100(1, 3);   /* 2004146 */
    Func_0808a100(0xc, 3);   /* 200414e */
    Func_0808a010(0x50);   /* 2004154 */
    Func_0808a100(0xe, 4);   /* 200415c */
    Func_0808a010(0x78);   /* 2004162 */
    Func_0808a170(0x24bd);   /* 200416a */
    Func_0808a180(0xe, 0);   /* 2004172 */
    Func_0808a1e8(0xc, 0x100, 0x3c);   /* 200417e */
    Func_0808a170(0x24be);   /* 2004186 */
    Func_0808a180(0xc, 0);   /* 200418e */
    Func_0808a148(0xe, 0xc, 0);   /* 2004198 */
    Func_0808a010(0x14);   /* 200419e */
    Func_0808a130(0xe, 1);   /* 20041a6 */
    Func_0808a010(0x32);   /* 20041ac */
    Func_0808a170(0x24bf);   /* 20041b4 */
    Func_0808a180(0xe, 0);   /* 20041bc */
    Func_0808a100(1, 3);   /* 20041c4 */
    Func_0808a010(0x46);   /* 20041ca */
    Func_0808a170(0x24c0);   /* 20041d2 */
    Func_0808a180(1, 0);   /* 20041da */
    Func_0808a100(3, 3);   /* 20041e2 */
    Func_0808a170(0x24c1);   /* 20041ea */
    Func_0808a180(3, 0);   /* 20041f2 */
    Func_0808a148(0xe, 0, 0);   /* 20041fc */
    Func_0808a110(0xe, 3);   /* 2004204 */
    Func_0808a010(0x14);   /* 200420a */
    Func_0808a170(0x24c2);   /* 2004212 */
    Func_0808a180(0xe, 0);   /* 200421a */
    Func_0808a1e8(2, 0x101, 0x5a);   /* 2004224 */
    Func_0808a170(0x24c3);   /* 200422c */
    Func_0808a180(2, 0);   /* 2004234 */
    Func_0808a1b8(0xe, 0x5000, 0);   /* 2004240 */
    Func_0808a010(0x14);   /* 2004246 */
    Func_0808a170(0x24c4);   /* 200424e */
    Func_0808a180(0xe, 0);   /* 2004256 */
    Func_0808a130(0xc, 1);   /* 200425e */
    Func_0808a010(0x3c);   /* 2004264 */
    Func_0808a170(0x24c5);   /* 200426c */
    Func_0808a180(0xc, 0);   /* 2004274 */
    Func_0808a148(0xe, 0xc, 0);   /* 200427e */
    Func_0808a010(0x14);   /* 2004284 */
    Func_0808a110(0xe, 3);   /* 200428c */
    Func_0808a010(0x14);   /* 2004292 */
    Func_0808a170(0x24c6);   /* 200429a */
    Func_0808a180(0xe, 0);   /* 20042a2 */
    Func_0808a1e8(1, 0x100, 0x46);   /* 20042ae */
    Func_0808a170(0x24c7);   /* 20042b6 */
    Func_0808a180(1, 0);   /* 20042be */
    Func_0808a148(0xe, 0, 0);   /* 20042c8 */
    Func_0808a010(0x1e);   /* 20042ce */
    Func_0808a170(0x24c8);   /* 20042d6 */
    Func_0808a180(0xe, 0);   /* 20042de */
    Func_0808a1e8(3, 0x100, 0x50);   /* 20042ea */
    Func_0808a170(0x24c9);   /* 20042f2 */
    Func_0808a180(3, 0);   /* 20042fa */
    Func_0808a110(0xe, 3);   /* 2004302 */
    Func_0808a170(0x24ca);   /* 200430a */
    Func_0808a180(0xe, 0);   /* 2004312 */
    Func_0808a148(2, 0, 0);   /* 200431c */
    Func_0808a010(0x1e);   /* 2004322 */
    Func_0808a100(2, 3);   /* 200432a */
    Func_0808a010(0x64);   /* 2004330 */
    Func_0808a170(0x24cb);   /* 2004338 */
    Func_0808a178(2, 0);   /* 2004340 */

    if (Func_0808a070(0, 0) != 0) {   /* 2004348 */
    }
        Func_0808a170(0x24cc);   /* 2004354 */
        Func_0808a180(0xe, 0);   /* 200435c */

    Func_0808a010(0x1e);   /* 2004362 */
    Func_0808a100(1, 3);   /* 200436a */
    Func_0808a100(2, 3);   /* 2004372 */
    Func_0808a100(3, 3);   /* 200437a */
    Func_0808a010(0x64);   /* 2004380 */
    Func_0808a150(0, 0xc, 0);   /* 200438a */
    Func_0808a100(0, 3);   /* 2004392 */
    Func_0808a100(0xc, 3);   /* 200439a */
    Func_0808a010(0x64);   /* 20043a0 */
    Func_0808a100(1, 2);   /* 20043a8 */
    actor = Func_0808a080(0);   /* 20043ae */
    if (actor != 0) {
        Func_0808a0b8(1, actor[5], actor[9]);   /* 20043c0 */
    }
    Func_0808a0e8(1);   /* 20043c6 */
    Func_0808a0f0(1, 0, 0);   /* 20043d0 */
    Func_0808a100(2, 2);   /* 20043d8 */
    actor = Func_0808a080(0);   /* 20043de */
    if (actor != 0) {
        Func_0808a0b8(2, actor[5], actor[9]);   /* 20043f0 */
    }
    Func_0808a0e8(2);   /* 20043f6 */
    Func_0808a0f0(2, 0, 0);   /* 2004400 */
    Func_0808a100(3, 2);   /* 2004408 */
    actor = Func_0808a080(0);   /* 200440e */
    if (actor != 0) {
        Func_0808a0b8(3, actor[5], actor[9]);   /* 2004420 */
    }
    Func_0808a0e8(3);   /* 2004426 */
    Func_0808a0f0(3, 0, 0);   /* 2004430 */
    Func_0808a010(0x1e);   /* 2004436 */
    Func_0808a090(0xc, 0x6666, 0x3333);   /* 2004440 */
    Func_0808a0c8(0xc, 0x1d8, 0xb8);   /* 200444c */
    Func_0808a0e8(0xc);   /* 2004452 */
    Func_0808a100(0xc, 1);   /* 200445a */
    Func_0808a1b8(0xc, 0x3000, 0);   /* 2004466 */
    Func_0808a148(0xe, 0, 0);   /* 2004470 */
    Func_0808a100(0xe, 3);   /* 2004478 */
    Func_0808a010(0x14);   /* 200447e */
    Func_0808a148(0xe, 0xd, 0);   /* 2004488 */
    Func_0808a010(0x14);   /* 200448e */
    Func_0808a170(0x24cd);   /* 2004496 */
    Func_0808a180(0xe, 0);   /* 200449e */
    Func_0808a138(0xd, 2);   /* 20044a6 */
    Func_0808a170(0x24ce);   /* 20044ae */
    Func_0808a180(0xd, 0);   /* 20044b6 */
    Func_0808a100(0, 3);   /* 20044be */
    Func_0808a010(0x1e);   /* 20044c4 */
    Func_0808a090(0, 0x10000, 0x8000);   /* 20044d2 */
    Func_0808a090(0xc, 0x10000, 0x8000);   /* 20044e0 */
    Func_0808a200(0, 1);   /* 20044e8 */
    Func_0808a0c8(0, 0x1e0, 0xf8);   /* 20044f4 */
    Func_0808a010(0x28);   /* 20044fa */
    Func_0808a1e0(0xc, 0);   /* 2004502 */
    Func_0808a1e0(0, 0);   /* 200450a */
    Func_0808a0c8(0xc, 0x1e0, 0xd8);   /* 2004516 */
    Func_0808a0e8(0);   /* 200451c */
    Func_0808a148(0, 0xe, 0);   /* 2004526 */
    Func_0808a0e8(0xc);   /* 200452c */
    Func_0808a148(0xc, 0xe, 0);   /* 2004536 */
    Func_0808a1b8(0xc, 0x3000, 0);   /* 2004542 */
    Func_0808a100(0, 3);   /* 200454a */
    Func_0808a010(5);   /* 2004550 */
    Func_0808a100(0xc, 3);   /* 2004558 */
    Func_0808a010(0x64);   /* 200455e */
    Func_0808a100(0xe, 3);   /* 2004566 */
    Func_0808a010(0x64);   /* 200456c */
    Func_0808a0c8(0xc, 0x1e0, 0xf8);   /* 2004578 */
    Func_0808a0c8(0, 0x1c8, 0xf8);   /* 2004584 */
    Func_0808a0e8(0xc);   /* 200458a */
    Func_0808a0c8(0xc, 0x1c8, 0xf8);   /* 2004596 */
    Func_0808a0e8(0);   /* 200459c */
    Func_0808a0c8(0, 0x1c8, 0x168);   /* 20045aa */
    Func_0808a0e8(0xc);   /* 20045b0 */
    Func_0808a0c8(0xc, 0x1c8, 0x168);   /* 20045be */
    Func_0808a0e8(0);   /* 20045c4 */
    Func_0808a0c8(0, 0x160, 0x168);   /* 20045d2 */
    Func_0808a0e8(0xc);   /* 20045d8 */
    Func_0808a0c8(0xc, 0x160, 0x168);   /* 20045e6 */
    Func_0808a010(0x14);   /* 20045ec */
    Func_080f9010(0x11);   /* 20045f2 */

    {
        u8 *workspace = *(u8 **)0x03001ebc;

        *(s32 *)(workspace + 448) = 515;
    }
    /* No argument register is set for this call. */
    Func_0808a368();   /* 2004604 */
    Func_0808a010(1);   /* 200461c */
    Func_0808a010(0xd2);   /* 2004622 */
    Func_0808a248(4);   /* 2004628 */
    Func_0808a020();   /* 200462c */
}
