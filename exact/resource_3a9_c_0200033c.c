#include "resource_3a9.h"

#include "types.h"

/*
 * resource_3a9 owner at 0x0200033c, 172 bytes: the per-sub-state slot cleanup
 * — decide which set of scene slots this sub-state leaves behind and clear
 * them.
 *
 * Unlike this overlay's other rows, 0x0200033c is NOT named by any in-image
 * pool word and no script-table record carries 0x0200833d, so nothing inside
 * the overlay references it and `overlay_call_targets.ts` reports no prologue
 * targets anywhere here.  It is nonetheless an ordinary, self-contained,
 * frame-balanced owner — one prologue, one interworking return, no live state
 * crossing either end — so it converts normally, exactly as the analogous
 * unreferenced owner in `semantic/overlays/resource_3ce_c_02000cf4.c` did.
 * Link-base evidence for the overlay is in the header of
 * `semantic/overlays/resource_3a9_c_0200007c.c`.
 *
 * Complete owner.  Prologue `push {lr}` at 0x0200033c with an 8-byte frame
 * (`sub sp,#8`) for the two stacked arguments of the six-argument call; single
 * epilogue `add sp,#8 / pop {r0} / bx r0` at 0x020003d8, so the popped word is
 * the return address and the owner is **void**.  Code runs
 * 0x0200033c..0x020003dd, the 2-byte zero at 0x020003de is the alignment word,
 * the literal pool is 0x020003e0..0x020003e7, and 0x020003e8 is the first
 * import veneer.  172 bytes, matching the inventory row.
 *
 * All 15 call sites resolved with `bun tools/overlay_call_targets.ts
 * resource_3a9 033c` (an overlay `bl` stores target offset - 2).  Per-target:
 *   Func_0808a088 11   Func_080770c0 2   Func_08009180 1   Func_0808a158 1
 *
 * THE ELEVEN Func_0808a088 SITES ARE ELEVEN SITES, NOT A LOOP.  Nine of them
 * are a straight run of `movs r0,#k / bl` pairs with no counter, no back edge
 * and no compare, and the ids they pass are 10, 11, 12, 13, 14, 17, 18, 19, 15
 * — non-contiguous and ending out of order, which is the tell.  The other two
 * are on the unrelated final arm with ids 16 and 17.  Folding either run into a
 * loop would deflate the multiset by nine; that is the documented script-table
 * shape and it is written out here the same way.  The odd trailing 15 is
 * preserved rather than sorted, on the same grounds as the documented
 * non-sequential refresh orders.
 *
 * The sub-state dispatch is a three-way compare chain with a SHARED taken arm:
 * `> 15` falls to a `== 17` test, otherwise `>= 9` takes the same arm, and only
 * a bare `== 3` gets the six-argument call.  Everything else — including 16,
 * which sits inside the middle of the accepted range — goes to the last arm.
 * That hole is real and is why the test is not written as `9..17`.  The same
 * hole appears in Func_0200007c's window over the same halfword, which is a
 * free cross-check that both were read correctly.
 *
 * `Data_02000240 + 450` is the s16 sub-state slot of the shared work area,
 * `Data_02000240[225]`, as the tracked `assets/code/resource_3a9_c_02000308.c`
 * and the tracked resource_36f sources spell it.
 *
 * Uncertainties: 0x911 is read as an event-flag id from its argument position
 * on Func_080770c0; the six-argument Func_08009180(30, 14, 30, 16, 4, 2) is the
 * established six-argument scripted-actor ABI, with the last two arguments
 * passed on the stack, and its argument meanings are not established here.
 */

/* Old-style declarations: overlay imports vary in arity between call sites. */
void Func_020007b4();
void Func_020007ba();
void Func_020007c0();
void Func_020007c6();
void Func_020007cc();
void Func_020007d2();
void Func_020007d8();
void Func_020007de();
void Func_020007e4();
void Func_02000800();
void Func_02000806();
void Func_0200080e();
void Func_02000762(s32, s32, s32, s32, s32, s32);
s32 Func_02000772();
s32 Func_020007be();

/* RAM: the shared work area. */
extern s16 Data_02000240[];

void Func_0200033c(void)
{
    s16 sub = Data_02000240[225];

    switch (sub) {
    case 3:
    {
        s32 fifth = 4;
        s32 sixth = 2;
        Func_02000762(30, 14, 30, 16, fifth, sixth);
        return;
    }
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 17:
        break;
    default:
        goto other;
    }

    /* sub is 9..15 or 17. */
    if (Func_02000772(0x911) != 0) {
        /* Nine distinct sites; the trailing 15 is out of order in the
         * reference and is kept that way. */
        Func_020007b4(10);
        Func_020007ba(11);
        Func_020007c0(12);
        Func_020007c6(13);
        Func_020007cc(14);
        Func_020007d2(17);
        Func_020007d8(18);
        Func_020007de(19);
        Func_020007e4(15);
    } else {
        Func_0200080e(13, 2);
    }
    return;

other:
    if (Func_020007be(0x911) != 0) {
        Func_02000800(16);
        Func_02000806(17);
    }
}
