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
s32 *Func_0808a080();
s32 Func_080770c0();
void Func_080770c8();
void Func_0808a010();
void Func_0808a018();
void Func_0808a020();
void Func_0808a090();
void Func_0808a0b8();
void Func_0808a0d0();
void Func_0808a0f0();
void Func_0808a1e0();
void Func_0808a208();
void Func_0808a210();
void Func_0808a218();
void Func_08009278();
void Func_080091c0();
void Func_080091f0();
void Func_08015040();
void Func_080f9010();

void Func_020016dc(void)
{
    s32 *actor;
    s32 *other;

    actor = Func_0808a080(17);
    Func_08009278(2, 0x1100000, 0x800000, 0);
    Func_08009278(2, 0x1200000, 0x800000, 0);

    if (actor == 0) {
        return;
    }

    {
        s32 height = actor[4] >> 20;

        Func_0808a018();
        if (height != 8) {
            goto done;
        }
    }

    if (Func_080770c0(0x207) == 0) {
        other = Func_0808a080(0);
        if ((u32)(other[4] >> 19) <= 17) {
            Func_0808a0d0(0, 0x121, 0x9e);
            *(u16 *)((u8 *)other + 6) = 0xc000;
            other = Func_0808a080(0);
        }
    }

    if (Func_080770c0(0x816) == 0) {
        goto done;
    }
    if (Func_080770c0(0x817) == 0) {
        goto done;
    }
    Func_080770c8(0x818);

    Func_0808a208(0x20000, 0x4000);
    Func_0808a210(0x11e0000, -1, 0x920000, 1);
    Func_0808a218();

    ((u8 *)other)[90] &= 0xfe;
    other = Func_0808a080(17);
    Func_0808a090(17, 0x30000, 0x10000);

    ((u8 *)actor)[85] = 0;

    Func_0808a1e0(17, 3);
    Func_080f9010(189);
    Func_0808a0b8(17, 0x120, 178);
    Func_0808a010(8);
    Func_0808a0f0(18, 0x1200000, 0xb20000);

    actor[14] = 0x80000000;
    actor[15] = 0x80000000;
    actor[16] = 0x80000000;
    actor[2] = 0;
    actor[3] = 0;
    actor[4] = 0;
    actor[9] = 0;
    actor[10] = 0;
    actor[11] = 0;

    Func_0808a0f0(17, 0, 0);

    Func_080091f0(0x10000, 0x10000, 0x10000);
    Func_0808a010(10);
    Func_080f9010(141);
    Func_080091f0(0x30000, 0x30000, 0x10000);
    Func_0808a010(10);
    Func_080091f0(0x50000, 0x50000, 0x10000);
    Func_0808a010(35);
    Func_080091f0(0x40000, 0x40000, 0x10000);
    Func_0808a010(20);
    Func_080091f0(0x30000, 0x30000, 0x10000);
    Func_0808a010(30);
    Func_080091f0(0x20000, 0x20000, 0x10000);
    Func_0808a010(40);
    Func_080f9010(0x121);
    Func_080091f0(0x10000, 0x10000, 0x10000);
    Func_0808a010(10);
    Func_080091f0(-1, -1, 0xe666);
    Func_0808a010(60);
    Func_080f9010(188);

    if (Func_080770c0(0x80b) != 0 && Func_080770c0(0x80c) != 0 &&
        Func_080770c0(0x80d) != 0 && Func_080770c0(0x80e) != 0) {
        Func_080770c8(0x80f);
    }

    Func_0808a010(40);
    Func_08015040(0x1038, 1);

    Func_080091c0(0, 1, 2, 1, 17, 8);
    Func_080091c0(17, 9, 2, 1, 17, 7);

    Func_0808a020();
done:
}
