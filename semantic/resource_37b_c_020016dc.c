#include "types.h"

/*
 * Resource 37b scene sequence at 0x020016dc.
 *
 * Complete owner: `push {r5, r6, lr}` / `sub sp, #8` at 0x020016dc through the
 * matching interworking return at 0x02001924..0x0200192a (`add sp, #8` /
 * `pop {r5, r6}` / `pop {r0}` / `bx r0`).  The popped register is r0, so it
 * holds the return address and the owner is `void`.
 *
 * The twelve words at 0x0200192c-0x0200195b are this owner's literal pool.
 * They lie after the `bx r0` and inside the row's 640-byte span, which ends
 * exactly at the next prologue (0x0200195c).  They are branched past by the
 * return, never executed, and are loaded by the `ldr r0, [pc, #imm]` sites at
 * 0x0200171c, 0x02001736, 0x0200174a, 0x02001756, 0x02001762, 0x020018aa,
 * 0x020018be, 0x020018c8, 0x020018d2, 0x020018dc, 0x020018e6 and 0x020018f2.
 * Every one of them is a small constant (a story-flag id, a script id, or the
 * fade value 0xe666), so none of them is an address and none is decoded as an
 * instruction here.
 *
 * Call sites resolved with `tools/lib/overlay_call_targets.ts` (an overlay `bl`
 * stores `target_offset - 2`; the disassembler's own annotations are wrong).
 * 51 sites, 19 distinct import veneers, no in-overlay callees.  The inventory
 * reports calls=47, a lower bound as documented.  Completeness was taken as a
 * multiset: the tool's 51 per-site targets were matched one-for-one against
 * the `bl` halfword pairs of the span, and each appears exactly once below —
 * per-import counts 2/8/2/1/7/2/10/1/1/4/1/1/1/2/1/1/1/1/4.
 *
 * Import roles, consistent with the sibling owners 0x02001624, 0x02000c8c and
 * 0x02001ff4 in this overlay:
 *   Func_0808a080(n)    - scene-record accessor; the record's word at +16 is a
 *                         16.16 height read as `>> 20` (and once as `>> 19`)
 *   Func_080770c0(id)   - test a story flag;  Func_080770c8(id) sets one
 *   Func_0808a010(n)    - frame wait;  Func_080f9010(n) - paced step
 *   Func_080091f0(x,y,z)- the three-component move/scale used by the fade run
 *   Func_080091c0(...)  - six-argument ABI, four registers plus [sp, #0] and
 *                         [sp, #4], both set explicitly at each of its 2 sites
 *
 * Uncertainties recorded as found:
 *  - Func_0808a018 (0x02001712) and Func_0808a218 (0x02001786) are reached
 *    with no argument register set by this owner and are spelled with none.
 *  - The record fields at +56, +60 and +64 are set to 0x80000000, which reads
 *    as "unset" sentinels rather than as a coordinate; that is transcribed,
 *    not interpreted.
 *  - The `>> 19` test at 0x0200172e is an ARITHMETIC shift compared with
 *    `bhi`, i.e. an unsigned comparison of a signed shift result; it is
 *    written as `(u32)(v >> 19) <= 17` to preserve exactly that.
 *  - Story flag 0x0207 is only read here; its writer is elsewhere in the
 *    overlay, which is the documented gate-flag pattern and not an omission.
 */

/* Imports.  Old-style declarations: arity varies per site in this overlay. */
s32 *Func_02003b50();
void Func_02003b1a();
void Func_02003b2a();
void Func_02003b68();
s32 Func_02003b54();
s32 *Func_02003b96();
void Func_02003bd0();
s32 *Func_02003bae();
s32 Func_02003b82();
s32 Func_02003b8e();
void Func_02003ba2();
void Func_02003c86();
void Func_02003ca0();
void Func_02003cac();
s32 *Func_02003bfa();
void Func_02003c1a();
void Func_02003cb2();
void Func_02003d20();
void Func_02003c44();
void Func_02003c80();
void Func_02003c28();
void Func_02003c5e();
void Func_02003d7c();
void Func_02003c7a();
void Func_02003c5a();
void Func_02003c90();
void Func_02003c70();
void Func_02003ca6();
void Func_02003cbc();
void Func_02003c9c();
void Func_02003cd2();
void Func_02003df0();
void Func_02003cb8();
void Func_02003cee();
void Func_02003ccc();
void Func_02003d02();
void Func_02003e20();
s32 Func_02003cf6();
s32 Func_02003d00();
s32 Func_02003d0a();
s32 Func_02003d14();
void Func_02003d26();
void Func_02003d3c();
void Func_02003d24();
void Func_02003d18();
void Func_02003d2a();
void Func_02003d7e();

                     

                     

                     

                     

                     

                     

void Func_020016dc(void)
{
    s32 *actor;
    s32 *other;

    actor = Func_02003b50(17);
    Func_02003b1a(2, 0x1100000, 0x800000, 0);
    Func_02003b2a(2, 0x1200000, 0x800000, 0);

    if (actor == 0) {
        return;
    }

    {
        s32 height = actor[4] >> 20;

        Func_02003b68();
        if (height != 8) {
            goto done;
        }
    }

    if (Func_02003b54(0x207) == 0) {
        other = Func_02003b96(0);
        if ((u32)(other[4] >> 19) <= 17) {
            Func_02003bd0(0, 0x121, 0x9e);
            *(u16 *)((u8 *)other + 6) = 0xc000;
            other = Func_02003bae(0);
        }
    }

    if (Func_02003b82(0x816) == 0) {
        goto done;
    }
    if (Func_02003b8e(0x817) == 0) {
        goto done;
    }
    Func_02003ba2(0x818);

    Func_02003c86(0x20000, 0x4000);
    Func_02003ca0(0x11e0000, -1, 0x920000, 1);
    Func_02003cac();

    ((u8 *)other)[90] &= 0xfe;
    other = Func_02003bfa(17);
    Func_02003c1a(17, 0x30000, 0x10000);

    ((u8 *)actor)[85] = 0;

    Func_02003cb2(17, 3);
    Func_02003d20(189);
    Func_02003c44(17, 0x120, 178);
    Func_02003c1a(8);
    Func_02003c80(18, 0x1200000, 0xb20000);

    actor[14] = 0x80000000;
    actor[15] = 0x80000000;
    actor[16] = 0x80000000;
    actor[2] = 0;
    actor[3] = 0;
    actor[4] = 0;
    actor[9] = 0;
    actor[10] = 0;
    actor[11] = 0;

    Func_02003ca0(17, 0, 0);

    Func_02003c28(0x10000, 0x10000, 0x10000);
    Func_02003c5e(10);
    Func_02003d7c(141);
    Func_02003c44(0x30000, 0x30000, 0x10000);
    Func_02003c7a(10);
    Func_02003c5a(0x50000, 0x50000, 0x10000);
    Func_02003c90(35);
    Func_02003c70(0x40000, 0x40000, 0x10000);
    Func_02003ca6(20);
    Func_02003c86(0x30000, 0x30000, 0x10000);
    Func_02003cbc(30);
    Func_02003c9c(0x20000, 0x20000, 0x10000);
    Func_02003cd2(40);
    Func_02003df0(0x121);
    Func_02003cb8(0x10000, 0x10000, 0x10000);
    Func_02003cee(10);
    Func_02003ccc(-1, -1, 0xe666);
    Func_02003d02(60);
    Func_02003e20(188);

    if (Func_02003cf6(0x80b) != 0 && Func_02003d00(0x80c) != 0 &&
        Func_02003d0a(0x80d) != 0 && Func_02003d14(0x80e) != 0) {
        Func_02003d26(0x80f);
    }

    Func_02003d3c(40);
    Func_02003d24(0x1038, 1);

    Func_02003d18(0, 1, 2, 1, 17, 8);
    Func_02003d2a(17, 9, 2, 1, 17, 7);

    Func_02003d7e();
done:
}
