#include "types.h"

/*
 * Resource 395 scene setup at 0x02001070 (376 bytes,
 * 0x02001070-0x020011e7).
 *
 * Complete owner: `push {r5, r6, r7, lr} / mov r7,sl / mov r6,r8 /
 * push {r6, r7}` prologue at 0x02001070 and the matching
 * `pop {r3, r5} / mov r8,r3 / mov sl,r5 / pop {r5, r6, r7} / pop {r1} / bx r1`
 * interworking return at 0x020011dc.  The popped return address lands in r1,
 * NOT r0, so r0 survives and IS the result; `movs r0, #0` at 0x020011da is on
 * the single exit path, so this owner always returns 0.
 *
 * ONE LITERAL POOL SITS INSIDE THE SPAN: 0x020010d4-0x020010eb (six words),
 * hopped by the `bne.n 0x020010ec` at 0x020010ca and by the `b.n 0x020010ec`
 * at 0x020010d2.  Its bytes are not decoded as instructions, but register
 * state IS carried straight across it: r6 is loaded from the pool's own first
 * word (0x020010d4 = 0) at 0x020010c2, before the branch, and is not used
 * until 0x02001166 on the far side, where it is the zero operand of three
 * `strb`s.  A pool walk that restarted register state at the boundary would
 * lose that.
 *
 * Link base: resource_395 is linked at 0x02008000, proven by eleven pool words
 * across the overlay that are its own prologues plus the Thumb bit — including
 * 0x02009071, which is this owner + 1, so this owner is installed as a task
 * elsewhere.  Under that base the pool word 0x02000240 used below is BELOW the
 * image (which occupies 0x02008000-0x02009dbb) and is therefore an ordinary
 * EWRAM base address, not overlay data; the two halfwords it addresses live at
 * 0x02000480 and 0x02000482.
 *
 * All 31 call sites were resolved with tools/overlay_call_targets.ts
 * (true_target_offset = stored_displacement + 2), never from overlay_show.ts's
 * `bl` annotations, which are off by one veneer entry on this overlay.  Placed
 * multiset, matching the tool's sites=31 exactly: Func_0808a080 x17,
 * Func_080091e0 x5, Func_0808a1e0 x3, Func_0808a100 x2, and one each of
 * Func_080000c0, Func_0808a158, Func_080770c0, Func_02001768.  The inventory
 * reports calls=29, which is the usual lower bound.
 *
 * Behaviour: cache three participant records, wait a frame, select entity 14's
 * presentation, publish two workspace/EWRAM parameters, optionally run the
 * overlay-local step 0x02001768 when flag 0x845 is still clear, then push a
 * common initial state onto the eight participants.
 *
 * Uncertainties:
 *  - `movs r0,#224 / lsls r0,#1` builds 448 and the same register is then
 *    `adds r0,#128` to build 576 for the second base.  The two bases are
 *    different objects (the 0x03001ebc workspace and the 0x02000240 block), so
 *    the shared register is codegen, not a relationship.
 *  - records 10, 11 and 14 are held in r8/sl/r7 from the top of the owner and
 *    are the same records Func_0808a080 would return again; the cached
 *    pointers are used rather than re-fetched, which is what the assembly
 *    does.
 *  - the meaning of the +0x20 halfword (6), the +0x0c word (0x01c00000), the
 *    +0x55 byte (0) and bit 3 of the +0x59 byte is not established.
 */

u8 *Func_0808a080();            /* record by slot index */
void Func_080000c0();           /* wait n frames */
void Func_0808a158();           /* select an entity presentation */
s32 Func_080770c0();            /* story-flag test; used in a condition */
void Func_080091e0();           /* set presentation mode (record, phase) */
void Func_0808a1e0();           /* per-record scene mode */
void Func_0808a100();           /* per-record scene request */
void Func_02001768();           /* overlay-local scene step */

s32 Func_02001070(void)
{
    u8 *record10;
    u8 *record14;
    u8 *record11;
    u8 *record;

    record10 = Func_0808a080(10);
    record14 = Func_0808a080(14);
    record11 = Func_0808a080(11);

    Func_080000c0(1);
    Func_0808a158(14, 15);

    /* 129 << 2 = 516, stored at workspace + 448 (224 << 1). */
    *(u32 *)(*(u8 **)0x03001ebc + 448) = 516;

    /* 448 + 128 = 576 and 578, relative to the EWRAM block at 0x02000240. */
    *(u16 *)((u8 *)0x02000240 + 576) = 0x28;
    *(u16 *)((u8 *)0x02000240 + 578) = 4;

    if (Func_080770c0(0x845) == 0) {
        Func_02001768(3);
    }

    /* --- the literal pool at 0x020010d4-0x020010eb is skipped here; r6 (0)
     * was loaded from it before the branch and is used below. --- */

    *(u16 *)(Func_0808a080(8) + 0x20) = 6;
    *(u16 *)(Func_0808a080(9) + 0x20) = 6;
    *(u16 *)(Func_0808a080(12) + 0x20) = 6;
    *(u16 *)(Func_0808a080(13) + 0x20) = 6;

    Func_080091e0(Func_0808a080(14), 0);
    Func_080091e0(Func_0808a080(10), 0);
    Func_080091e0(Func_0808a080(11), 0);
    Func_080091e0(Func_0808a080(8), 0);
    Func_080091e0(Func_0808a080(9), 0);

    Func_0808a1e0(8, 2);
    Func_0808a1e0(14, 2);
    Func_0808a1e0(9, 2);

    record10[0x55] = 0;
    *(s32 *)(record10 + 0x0c) = 0x01c00000;      /* 224 << 13 */
    record11[0x55] = 0;
    *(s32 *)(record11 + 0x0c) = 0x01c00000;
    record14[0x55] = 0;
    *(s32 *)(record14 + 0x0c) = 0x01c00000;

    Func_0808a100(9, 3);
    Func_0808a100(8, 3);

    record[0x59] |= 8;
    record = Func_0808a080(8);
    record = Func_0808a080(9);
    record = Func_0808a080(10);
    record[0x59] |= 8;
    record[0x59] |= 8;
    record = Func_0808a080(11);
    record[0x59] |= 8;
    record = Func_0808a080(14);
    record[0x59] |= 8;

    return 0;
}
