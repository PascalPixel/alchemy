#include "types.h"

/*
 * resource_3bb owner at 0x02003cf8, 160 bytes (0x02003cf8-0x02003d97):
 * 150 bytes of code, two alignment bytes at 0x02003d8e, and the two-word
 * literal pool at 0x02003d90.
 *
 * Prologue `push {r5, r6, r7, lr} / mov r7, r8 / push {r7} / sub sp, #12` at
 * 0x02003cf8; epilogue `add sp, #12 / pop {r3} / mov r8, r3 /
 * pop {r5, r6, r7} / pop {r1} / bx r1`.  The interworking return pops into r1,
 * not r0, so r0 survives the epilogue and IS the result: this owner returns
 * the value of its last Func_02003b48 call.
 *
 * All five `bl` sites are placed and reach three distinct callees, matching
 * the inventory row's calls=5.  Targets come from
 * tools/lib/overlay_call_targets.ts (target offset = stored displacement + 2),
 * never from the disassembler's annotations: 0x3f50 -> Func_0808a080,
 * 0x3dc8 -> Func_08000128 (twice), and this overlay's own Func_02003b48
 * (twice), whose byte-exact source is assets/code/resource_3bb_c_02003b48.c.
 *
 * That byte-exact sibling settles the interface: Func_02003b48 takes one
 * argument, the three-word position below, and returns the occupying slot
 * pointer or 0.  Both call sites here also load r1 with the record pointer;
 * since the callee's own reconstructed source takes a single parameter, the
 * extra register is not asserted as an argument.  Its result is what decides
 * whether the second probe runs and what this owner returns.
 *
 * The pool word 0x02000240 is below this overlay's 0x02008000 link base (base
 * witnessed by 0x02008715 = Func_02000714 + 1 in the byte-exact sibling
 * assets/code/resource_3bb_c_02000950.c), so it is a RAM global.  The scaled
 * index is built as 250 << 1 = 500, giving the same word at 0x02000434 that
 * Func_020002e8 reads.
 *
 * Shape: take the active subject's record, derive its facing as
 * `(record halfword at +6 + 0x2000) & 0xc000` - the biased quadrant, with no
 * sign extension here - then probe one step ahead at 0x100000 and, if nothing
 * occupies it, one step further at 0x200000.  Each probe rounds the record's
 * x and z words down to whole units (`& 0xfff00000`) and re-centres them by
 * half a unit (0x80000) while carrying y through unrounded.
 *
 * Uncertainties: only the record fields at +6 (halfword), +8, +12 and +16 are
 * asserted.  r8 merely caches the 0xfff00000 mask across the first call and
 * carries no other value.
 */

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay. */
u8 *Func_0808a080();
void Func_08000128();

/* This overlay's own lookup; byte-exact source in assets/code. */
s32 *Func_02003b48(s32 *);

s32 *Func_02003cf8(void)
{
    u8 *record;
    s32 facing;
    s32 position[3];
    s32 *occupant;

    s32 permuted_0;
    s32 permuted_12;
    record = Func_0808a080(*(s32 *)(0x02000240 + 500));

    /* 128 << 6 = 0x2000 bias, then masked to bits 14-15 (192 << 8). */
    facing = (*(u16 *)(record + 6) + 0x2000) & 0xc000;

    position[0] = (*(s32 *)(record + 8) & 0xfff00000) + 0x80000;
    permuted_12 = *(s32 *)(record + 12);
    position[2] = (*(s32 *)(record + 16) & 0xfff00000) + 0x80000;
    Func_08000128(0x100000, facing, position);          /* 128 << 13 */
    position[1]  = permuted_12;

    occupant = Func_02003b48(position);
    if (occupant == 0) {
        permuted_0 = (*(s32 *)(record + 8) & 0xfff00000) + 0x80000;
        position[1] = *(s32 *)(record + 12);
        position[0]  = permuted_0;
        position[2] = (*(s32 *)(record + 16) & 0xfff00000) + 0x80000;
        Func_08000128(0x200000, facing, position);      /* 128 << 14 */

        occupant = Func_02003b48(position);
    }

    return occupant;
}
