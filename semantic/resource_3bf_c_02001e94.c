#include "types.h"

/*
 * Resource 3bf cutscene script at 0x02001e94.
 *
 * Complete owner: `push {r5, lr}` at 0x02001e94 through
 * `pop {r5} / pop {r0} / bx r0` at 0x0200204c.  r0 holds the popped return
 * address, so the owner is VOID (HANDOVER section 0's interworking-epilogue
 * rule).  The six-word literal pool at 0x02002054..0x0200206b closes the row's
 * 472 bytes and is data.
 *
 * This is a straight-line script — no branches at all between the prologue and
 * the epilogue.  Every one of its 50 `bl` sites was resolved with
 * `bun tools/lib/overlay_call_targets.ts resource_3bf 1e94`, never from the
 * disassembler's pc-relative annotations, and every one of them is an import
 * veneer; there are no local callees.  The 17 distinct imports are:
 *   0x02005618 -> Func_080770c8  x1    0x020056d0 -> Func_0808a148  x7
 *   0x02005628 -> Func_0808a010  x5    0x020056e8 -> Func_0808a170  x7
 *   0x02005638 -> Func_0808a020  x1    0x020056f8 -> Func_0808a180  x7
 *   0x02005680 -> Func_0808a0c8  x1    0x02005708 -> Func_0808a1b8  x2
 *   0x02005698 -> Func_0808a0e8  x1    0x02005718 -> Func_0808a1e8  x3
 *   0x020056a0 -> Func_0808a0f0  x3    0x02005760 -> Func_0808a250  x1
 *   0x020056a8 -> Func_0808a100  x4    0x02005768 -> Func_0808a260  x1
 *   0x020056b0 -> Func_0808a110  x4    0x020057a8 -> Func_080f9010  x1
 *   0x020056c8 -> Func_0808a138  x1
 * That is 50 sites against the inventory's `calls=46`; the field is neither a
 * site count nor reliable, as HANDOVER records, and the multiset above is the
 * completeness proof.
 *
 * The script's shape is legible even without the imports' real names.  r5 is
 * loaded once with the pooled constant 0x2438 and used as a SEQUENTIAL LINE
 * CURSOR: Func_0808a170 is called seven times with 0x2438, +1, +2, +3, +4, +5
 * and finally +6 (the last after an explicit `adds r5,#6`), and each of those
 * seven calls is immediately followed by a Func_0808a180(actor, 0) — the
 * classic "show line / wait for it" pair.  Actors 12, 13 and 14 are the three
 * participants; Func_0808a010 takes bare delays (30, 65, 60, 70, 30).
 *
 * Link base.  None of this owner's pool words is in the proven in-image band
 * (0x0200_8xxx and above), so all of them are ordinary addresses: 0x03001ebc
 * is the IWRAM workspace pointer the sibling gates in this overlay also load,
 * and 0x02000240 is the same table those siblings index.  0x00002438,
 * 0x000000a1, 0x0000022b and 0x0000094a are plain small constants that merely
 * needed pooling.
 *
 * Uncertainties:
 *  - Func_0808a020 is called with no argument register set at all; r0/r1/r2
 *    still hold 14, 0 and 0 from the preceding Func_0808a0f0 call.  It is
 *    called with no arguments here and the leftovers are noted rather than
 *    asserted as parameters.
 *  - the byte written at 0x02000240 + 0x22b is at an ODD offset, so it is a
 *    byte field of that table and not one of the s16 entries the sibling
 *    sources index; it is spelled as a byte store for that reason.
 *  - the store to workspace + 448 is built as `224 << 1` for the offset and
 *    then `+ 64` reusing the same register for the value, so the stored word
 *    is 512.  Both numbers come from the one register and are easy to
 *    misread; they are 448 and 512 respectively.
 */

/* Old-style declarations are mandatory in overlay sources: one name can be
 * reached with different argument counts from different sites. */
void Func_080770c8();
void Func_0808a010();
void Func_0808a020();
void Func_0808a0c8();
void Func_0808a0e8();
void Func_0808a0f0();
void Func_0808a100();
void Func_0808a110();
void Func_0808a138();
void Func_0808a148();
void Func_0808a170();
void Func_0808a180();
void Func_0808a1b8();
void Func_0808a1e8();
void Func_0808a250();
void Func_0808a260();
void Func_080f9010();

void Func_02001e94(void)
{
    s32 line = 0x2438;

    Func_0808a100(0, 1);
    Func_0808a100(12, 1);
    Func_0808a100(13, 1);
    Func_0808a100(14, 1);
    Func_080f9010(113);

    Func_0808a1e8(12, 256, 0);
    Func_0808a010(30);
    Func_0808a148(12, 0, 0);
    Func_0808a170(line);
    Func_0808a180(12, 0);

    Func_0808a1e8(13, 256, 0);
    Func_0808a1e8(14, 256, 0);
    Func_0808a148(13, 0, 0);
    Func_0808a148(14, 0, 0);
    Func_0808a148(0, 13, 0);
    Func_0808a010(65);

    Func_0808a1b8(13, 0x5000, 0);
    Func_0808a1b8(14, 0xd000, 0);

    Func_0808a170(line + 1);
    Func_0808a180(13, 0);
    Func_0808a110(14, 3);

    Func_0808a170(line + 2);
    Func_0808a180(14, 0);

    Func_0808a170(line + 3);
    Func_0808a180(12, 0);
    Func_0808a138(13, 1);

    Func_0808a170(line + 4);
    Func_0808a180(13, 0);
    Func_0808a110(14, 3);

    Func_0808a170(line + 5);
    Func_0808a180(14, 0);
    Func_0808a110(14, 3);

    Func_0808a010(60);
    Func_0808a148(13, 0, 0);
    Func_0808a148(14, 0, 0);
    Func_0808a010(70);

    Func_0808a0c8(12, 0x2a0, 88);
    Func_0808a0e8(12);
    Func_0808a148(12, 0, 0);
    Func_0808a110(12, 3);

    Func_0808a010(30);
    Func_0808a170(line);
    line += 6;
    Func_0808a180(12, 0);

    {
        u8 *workspace = *(u8 **)0x03001ebc;

        *(s32 *)(workspace + 448) = 512;
    }

    Func_0808a260(0xa1, 31);

    *((u8 *)0x02000240 + 0x22b) = 3;

    Func_0808a250(98, 3);

    Func_0808a0f0(12, 0, 0);
    Func_0808a0f0(13, 0, 0);
    Func_0808a0f0(14, 0, 0);
    /* No argument register is set for this call; r0/r1/r2 still hold 14/0/0. */
    Func_0808a020();

    Func_080770c8(0x94a);
}
