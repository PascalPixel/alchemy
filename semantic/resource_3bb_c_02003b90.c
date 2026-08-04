#include "types.h"

/*
 * resource_3bb owner at 0x02003b90, 360 bytes (0x02003b90-0x02003cf7):
 * 344 bytes of code plus the four-word literal pool at 0x02003ce8.
 *
 * Prologue at 0x02003b90 saves r5, r6, r7, lr and then fp, sl, r9 and r8
 * before `sub sp, #12`; the matching epilogue at 0x02003cd6 restores all of
 * them and ends `pop {r0} / bx r0`, so r0 holds the popped return address and
 * the owner is void.  The twelve bytes of frame are the three-word position
 * scratch that every probe and every move uses; r7 aliases it.  The high
 * registers are caches: r9 = the direction table, sl = 0xffff0000, r8 = the
 * subject record, fp = the constant 0 stored at the end.
 *
 * All 13 `bl` sites are placed and reach seven distinct callees, matching the
 * inventory row's calls=13.  Targets come from tools/overlay_call_targets.ts
 * (target offset = stored displacement + 2), never from the disassembler's
 * annotations - it prints the three identical Func_02003b48 halfwords, and the
 * two identical Func_08009150 halfwords, as different callees.  0x3f50 ->
 * Func_0808a080, 0x3e98 -> Func_080091d8, 0x3e48 -> Func_08009080 (twice),
 * 0x3da0 -> Func_080000c0, 0x3e78 -> Func_08009150 (twice), 0x40b0 ->
 * Func_080f9010 (twice), plus three calls to this overlay's own
 * Func_02003b48, whose byte-exact source is
 * assets/code/resource_3bb_c_02003b48.c.
 *
 * That byte-exact sibling settles the interface: Func_02003b48 takes the
 * three-word position and returns the occupying slot pointer or 0.  All three
 * sites here also load r1 with a record pointer; since the callee's own
 * reconstructed source takes a single parameter, the extra register is not
 * asserted as an argument.
 *
 * The pool word 0x02000240 is below the overlay's 0x02008000 link base (base
 * witnessed by 0x02008715 = Func_02000714 + 1 in the byte-exact sibling
 * assets/code/resource_3bb_c_02000950.c), so it is a RAM global holding the
 * active subject id at +500 (250 << 1).  0x0200c3d4 is inside the link band
 * and is therefore in-image data at file offset 0x43d4: a sixteen-entry table
 * indexed by the top nibble of the subject's facing halfword at +6.  Each
 * entry packs two signed 16-bit steps - the high half is added to x directly
 * (`& 0xffff0000`) and the low half is shifted up 16 and added to z.
 *
 * Shape: a push.  Find what occupies the tile one step ahead of the subject;
 * if nothing does, stop.  Refuse the push when the tile beyond that occupant,
 * or the tile above it, is occupied by something whose flag byte at +89 has
 * bit 0 set.  Otherwise mark the occupant with state 2, ask Func_080091d8
 * whether the destination is legal (a result greater than zero refuses), and
 * play the slide: both records get 0x3333 in their +48 and +52 speed fields,
 * both are moved with Func_08009150, the two sound ids 238 and 288 are played
 * around Func_08009158, and only the occupant's x and z are committed while
 * its +36 and +44 are cleared and the subject is put back into pose 1.
 *
 * Uncertainties: only the record fields at +6, +8, +12, +16, +34, +36,
 * +44, +48, +52 and +89 are asserted.  Both Func_08009150 calls are issued
 * with the *same* destination, which is what the instructions do - the
 * scratch is not recomputed between them - and the subject's own record
 * position is never written back, so only the occupant's move is committed.
 * The sign of Func_080091d8's result is tested with `bgt`, so it is signed.
 */

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay. */
u8 *Func_0808a080();
s32 Func_080091d8();
void Func_08009080();
void Func_080000c0();
void Func_08009150();
void Func_080f9010();
void Func_08009158();

/* This overlay's own lookup; byte-exact source in assets/code. */
u8 *Func_02003b48(s32 *);

/* In-image direction table at file offset 0x43d4, sixteen packed steps. */
extern s32 Data_0200c3d4[];

void Func_02003b90(void)
{
    u8 *subject;
    u8 *occupant;
    u8 *blocker;
    s32 facing;
    s32 step;
    s32 position[3];

    subject = Func_0808a080(*(s32 *)(0x02000240 + 500));
    facing = *(u16 *)(subject + 6) >> 12;

    step = Data_0200c3d4[facing];
    position[0] = *(s32 *)(subject + 8) + (step & 0xffff0000);
    position[1] = *(s32 *)(subject + 12);
    position[2] = *(s32 *)(subject + 16) + (step << 16);

    occupant = Func_02003b48(position);
    if (occupant == 0) return;

    step = Data_0200c3d4[facing];
    position[0] = *(s32 *)(occupant + 8) + (step & 0xffff0000);
    position[1] = *(s32 *)(occupant + 12);
    position[2] = *(s32 *)(occupant + 16) + (step << 16);

    blocker = Func_02003b48(position);
    if (blocker != 0 && (blocker[89] & 1) != 0) return;

    position[0] = *(s32 *)(occupant + 8);
    position[1] = *(s32 *)(occupant + 12) + 0x100000;   /* 128 << 13 */
    position[2] = *(s32 *)(occupant + 16);

    blocker = Func_02003b48(position);
    if (blocker != 0 && (blocker[89] & 1) != 0) return;

    occupant[34] = 2;

    step = Data_0200c3d4[facing];
    position[0] = *(s32 *)(occupant + 8) + (step & 0xffff0000);
    position[1] = *(s32 *)(occupant + 12);
    position[2] = *(s32 *)(occupant + 16) + (step << 16);

    if (Func_080091d8(occupant, position) > 0) return;

    Func_08009080(subject, 8);
    Func_080000c0(15);

    *(s32 *)(occupant + 48) = 0x3333;
    *(s32 *)(occupant + 52) = 0x3333;
    Func_08009150(occupant, position[0], position[1], position[2]);

    *(s32 *)(subject + 48) = 0x3333;
    *(s32 *)(subject + 52) = 0x3333;
    Func_08009150(subject, position[0], position[1], position[2]);

    Func_080f9010(238);
    Func_08009158(occupant);
    Func_080f9010(288);                                 /* 144 << 1 */

    *(s32 *)(occupant + 8) = position[0];
    *(s32 *)(occupant + 16) = position[2];
    *(s32 *)(occupant + 36) = 0;
    *(s32 *)(occupant + 44) = 0;

    Func_08009080(subject, 1);
}
