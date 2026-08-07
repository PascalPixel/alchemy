#include "types.h"

/*
 * Resource 3bf scene-phase advance at 0x02004da0.
 *
 * Complete owner: `push {r5, r6, r7, lr}` at 0x02004da0 through
 * `movs r0,#0 / pop {r5,r6,r7} / pop {r1} / bx r1` at 0x02004f32.  The
 * epilogue pops the return address into r1, not r0, so r0 survives and IS the
 * result (HANDOVER section 0's interworking-epilogue rule) — and r0 is set to
 * a literal 0 immediately before the pop on every path, so the owner returns a
 * constant 0.  The nine-word literal pool at 0x02004f3c..0x02004f5f closes the
 * row's 448 bytes and is data.
 *
 * All 31 `bl` sites were resolved with
 * `bun tools/lib/overlay_call_targets.ts resource_3bf 4da0`, never from the
 * disassembler's pc-relative annotations.  They collapse to 13 distinct
 * targets, which is the row's `calls` field exactly:
 *   0x020055a0 -> Func_080000f8   x1   (a value source; see below)
 *   0x020055e8 -> Func_080091e0   x6   (actor, 0)
 *   0x020055f8 -> Func_08009258   x4   (one id argument)
 *   0x02005610 -> Func_080770c0   x1   (state query)
 *   0x02005650 -> Func_0808a080   x6   (actor fetch by kind)
 *   0x020056a0 -> Func_0808a0f0   x4   (kind, x, z)
 *   0x020056a8 -> Func_0808a100   x1   (kind, 0)
 *   0x02005708 -> Func_0808a1b8   x3   (kind, a, b)
 *   0x02003054, 0x02004f60, 0x020050e4, 0x02005324, 0x02005520 — ordinary
 *   prologues of this same overlay, named by the tree's offset convention.
 *
 * LINK BASE.  0x0200dfa4 is the one pool word inside the proven in-image band
 * (0x0200_8xxx..0200_bxxx and above), so under this overlay's confirmed
 * 0x02008000 link base it is the module's OWN data at file offset 0x5fa4 —
 * writable EWRAM, not a RAM global and not const.  The tree's convention is
 * kept: the raw pool value is used for the data address while symbol names
 * stay on printed offsets.  The other three absolute constants are below the
 * band and are ordinary addresses: 0x02000240 is the s16 table the sibling
 * gates index (they use [294]; this one uses [224] and [225]), and 0x03001ebc
 * is the IWRAM workspace pointer those siblings also load.
 *
 * Shape.  A value is fetched, scaled by 7/65536 and published to the module's
 * own word at 0x0200dfa4.  Then table[224] is walked through four phase
 * values, 160/161/162/163: each of the first three runs one step routine and
 * re-reads the halfword, because the step is allowed to advance the phase.
 * Only phase 163 continues; anything else returns immediately.  Phase 163
 * writes 516 into workspace+448, resets actors 12/8/9/10 (each of 8, 9 and 10
 * also getting its byte at +35 set to 2), then switches on table[225]: value 4
 * runs one branch, and value 3 additionally runs a gated setup that positions
 * actors 16, 13 and 17.  The tail always resets actor 15, sets its +35 byte to
 * 2 and stores 0xcccc at its +24.
 *
 * Uncertainties, all of them things the assembly does that read oddly:
 *  - the four `object[35] = 2` stores and the final `object[24] = 0xcccc` are
 *    UNCONDITIONAL, while the reset call just above each of them is guarded by
 *    a null check on the very same pointer.  A null actor therefore writes
 *    through address 35 (and 24).  This is what the bytes say; it is preserved
 *    rather than "fixed".
 *  - Func_02005520 is called with r0 still holding 12 and r1 still holding 0
 *    from the preceding call block; the assembly sets no argument for it, so
 *    it is called with none here and the leftovers are noted instead.
 *  - the phase halfword is read `ldrsh` for the 160 test and `ldrh` +
 *    `lsls #16 / asrs #16` for the others; both are the signed 16-bit value.
 *    The table[225] == 3 test is the exception: it compares `ldrh << 16`
 *    against 0x00030000, i.e. it tests the UNSIGNED halfword.
 *  - the actors are only touched at +24 and +35, so they are modelled as
 *    opaque byte blocks rather than given a speculative layout.
 */

/* Old-style declarations are mandatory in overlay sources: one name can be
 * reached with different argument counts from different sites. */
s32 Func_0200a346();
void Func_0200a3c2();
void Func_02009d2e();
void Func_02009ec2();
void Func_0200a112();
s32 Func_0200a462();
s32 Func_0200a400();
void Func_0200a52a();
void Func_0200a4d2();
void Func_0200a34e();
s32 Func_0200a484();
s32 Func_0200a428();
s32 Func_0200a49e();
s32 Func_0200a442();
s32 Func_0200a4b6();
s32 Func_0200a45a();
void Func_0200a478();
void Func_0200a490();
void Func_02007ef0();
void Func_0200a4a8();
s32 Func_0200a4c6();
void Func_0200a564();
void Func_0200a572();
void Func_0200a5e6();
void Func_0200a58c();
void Func_0200a600();
void Func_0200a5a6();
s32 Func_0200a55c();
s32 Func_0200a4fa();
s32 Func_0200a568();
s32 Func_0200a50c();

                     

                    

                    

                     

s32 Func_02004da0(void)
{
    /* In-image module data at file offset 0x5fa4; writable. */
    s32 *published = (s32 *)0x0200dfa4;
    const s16 *table = (const s16 *)0x02000240;
    u8 *actor;

    published[0] = (s32)(((u32)(Func_0200a346() * 7)) >> 16);

    if (table[224] == 160) {
        Func_0200a3c2(3584);
        Func_02009d2e();
    }
    if (table[224] == 161) {
        Func_02009ec2();
    }
    if (table[224] == 162) {
        Func_0200a112();
    }
    if (table[224] != 163) {
        return 0;
    }

    {
        u8 *workspace = *(u8 **)0x03001ebc;

        *(s32 *)(workspace + 448) = 516;
    }

    actor = Func_0200a462(12);
    Func_0200a400(actor, 0);
    Func_0200a52a(12, 0, 0);
    Func_0200a4d2(12, 0);
    /* No argument register is set for this call; r0 still holds 12. */
    Func_0200a34e();

    actor = Func_0200a484(8);
    if (actor != 0) {
        Func_0200a428(actor, 0);
    }
    actor[35] = 2;

    actor = Func_0200a49e(9);
    if (actor != 0) {
        Func_0200a442(actor, 0);
    }
    actor[35] = 2;

    actor = Func_0200a4b6(10);
    if (actor != 0) {
        Func_0200a45a(actor, 0);
    }
    actor[35] = 2;

    Func_0200a478(3584);

    if (table[225] == 4) {
        Func_0200a490(3072);
        Func_02007ef0();
    }

    if (*(const u16 *)&table[225] == 3) {
        Func_0200a4a8(3072);
        if (Func_0200a4c6(0x941) != 0) {
            Func_0200a564(12, 0, 0);

            Func_0200a572(16, 0x01b00000, 0x01580000);
            Func_0200a5e6(16, 0x5000, 0);

            Func_0200a58c(13, 0x01c80000, 0x01200000);
            Func_0200a600(13, 0x5000, 0);

            Func_0200a5a6(17, 0x01c80000, 0x01400000);
            Func_0200a4fa(Func_0200a55c(17), 0);
        }
    }

    actor = Func_0200a568(15);
    if (actor != 0) {
        Func_0200a50c(actor, 0);
    }
    actor[35] = 2;
    *(s32 *)(actor + 24) = 0xcccc;

    return 0;
}
