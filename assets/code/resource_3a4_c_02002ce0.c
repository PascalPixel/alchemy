#include "types.h"

/*
 * resource_3a4 owner at 0x02002ce0, 40 bytes: read the low three bits of
 * the runtime word at 0x03001e40 and, on exactly two of the eight
 * values, forward its own incoming r0 to Func_0808a160 with a different
 * second argument.  A PUBLISHED CALLBACK -- reached only through an
 * address stored as a plain word, never by a `bl`, so sweep B of
 * tools/overlay_published.ts is what surfaced it.
 *
 * Complete owner: `push {lr}` at 0x02002ce0 through the interworking
 * return `pop {r0} / bx r0` at 0x02002d00-0x02002d02, one alignment
 * halfword, and the one-word literal pool 0x02002d04-0x02002d07; the
 * next owner begins at 0x02002d08 and is already drafted as
 * resource_3a4_c_02002d08.c.  measureSpan reports 36; the pool makes it
 * 40.
 *
 * ONE ARGUMENT, AND IT IS NEVER MATERIALISED.  r0 is not written
 * anywhere in this body, yet it is live at both call sites -- the
 * function takes its first argument and passes it straight through.
 * Reading this row as `void` would silently drop a parameter, so it is
 * spelled with one.  Both calls resolve to the same veneer,
 * Func_0808a160, by targetOffset; the listing prints them as two
 * different addresses, which is the usual lie.
 *
 * Uncertainty: 0x03001e40 is a runtime address outside this image (the
 * image is 21048 bytes, so the 0x8000-shifted in-image reading does not
 * apply here) and its low three bits are treated as a small enum whose
 * meaning is open.  Only values 0 and 2 do anything; 1 and 3..7 fall
 * through silently, and that is what the compiled code does rather than
 * an omission in this transcription.
 */

extern s32 Data_03001e40;

extern void Func_02006912();
extern void Func_0200691e();


void Func_02002ce0(s32 arg0)
{
    s32 selector = Data_03001e40 & 7;

    if (selector == 0) {
        Func_02006912(arg0, 2);
    } else if (selector == 2) {
        Func_0200691e(arg0, 0);
    }
}
