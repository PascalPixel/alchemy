#include "types.h"

/*
 * resource_37b owner at 0x02001a84, 40 bytes: fetches scene record 10
 * and, if it exists, hands a coarse coordinate derived from it to a
 * five-argument in-overlay routine.
 *
 * Complete owner: `push {lr}` + `sub sp, #4` at 0x02001a84 through the
 * single epilogue `add sp, #4 / pop {r0} / bx r0` at
 * 0x02001aa4-0x02001aa8, no literal pool. Next owner at 0x02001aac. The
 * halfword at 0x02001a82 is alignment belonging to the previous owner,
 * not to this one -- which is why sweep D reports the gap as starting
 * two bytes early. Measured to the epilogue. Takes no argument.
 *
 * Call sites resolved with `bun tools/lib/overlay_call_targets.ts
 * resource_37b 1a84 1aac`'s `+2` rule: `sites=2 veneer=1 prologue=1`,
 * and two are transcribed. The second is in-overlay, to Func_02001b44.
 *
 * Published, not called: its address is stored as 0x02009a84 in exactly
 * one word, at overlay offset 0x28ec -- in the data region past the last
 * owner, so it is installed from a TABLE and no drafted row points at
 * it. That is the same shape as resource_3c9's 0x02000518.
 *
 * The stack slot is a FIFTH ARGUMENT, not a local. `sub sp, #4` reserves
 * one word, `str r3, [sp, #0]` writes it immediately before the call,
 * and nothing ever reads it back -- so it is the argument that did not
 * fit in r0-r3, and the callee takes five. The value stored is the
 * coordinate BEFORE the increment, while r2 carries the same coordinate
 * PLUS ONE, so the callee receives both.
 *
 * Uncertainties: `>> 20` on the +16 field is the established coarse-cell
 * shape in this tree (a 16.16-style fixed-point field reduced to a cell
 * index), but nothing here proves the unit. The three constants 10, 13
 * and 13 are transcribed as written; the repeat of 13 in r1 and r3 is
 * real and not a transcription slip. Func_02001b44 is not drafted, so
 * it is declared old-style rather than given a signature this row cannot
 * establish.
 */

   /* scene-record accessor */


extern u8 * Func_02003ef8(s32 index);
extern void Func_020035e6();
void Func_02001a84(void)
{
    u8 *record = Func_02003ef8(10);
    s32 cell;

    if (record == 0) {
        return;
    }

    cell = *(s32 *)(record + 16) >> 20;
    Func_020035e6(10, 13, cell + 1, 13, cell);
}
