/*
 * resource_3b7 owner at 0x020004bc, 962 bytes (0x020004bc-0x0200087d).
 *
 * A scripted musical performance: 29 timed note events through Func_08009180,
 * each followed by a sound trigger and a wait, with one mid-piece flourish that
 * targets the actor passed in.
 *
 * Code runs 0x020004bc-0x0200087d with NO literal pool at all - every operand
 * is an immediate or a shifted immediate, and the span ends on the `bx r0` of
 * the interworking return.  Nothing here depends on the overlay's link base.
 *
 * Call targets were resolved with `tools/lib/overlay_call_targets.ts` (an overlay
 * `bl` stores `target_offset - 2`).  Completeness, as a multiset:
 * Func_0808a010 x33, Func_080f9010 x30, Func_08009180 x29, and one each of
 * Func_0808a018, Func_0808a020, Func_0808a060, Func_0808a1b8, Func_0808a398 -
 * 97 sites over 8 distinct targets, exactly the tool's
 * `sites=97 distinct_targets=8`.  The inventory row says calls=95, the
 * documented lower bound.
 *
 * Every call's arguments were recovered by an argument-window walk over the
 * disassembly with the documented guard (an unmodelled instruction clears only
 * its destination register, never the window), which is safe here because
 * there is no pool to hop and no branch of any kind in the owner: it is one
 * straight-line block from prologue to epilogue.
 *
 * Func_08009180 takes six arguments - four in registers and two on the stack
 * at [sp, #0] and [sp, #4].  The prologue reserves those eight bytes with
 * `sub sp, #8`, keeps 3 in r6 and 8 in r8 for the whole owner, and rewrites
 * both slots before every call.  The four constant arguments (70, 0, 3, 8)
 * never change; only the first two vary, and they read as a pitch and a
 * duration: the piece runs 82, 85, 88, 91, 94, 97, 100 at duration 20, then
 * 79, 82, 85, 88, 91, 94, 97, 100 at duration 29, then descends 97, 94, 91,
 * 88, 85, 82 at 29 and 100, 97, 94, 91, 88, 85, 82, 79 at 20.  Each step is
 * three, which is the shape of a scale walk, but the units are not
 * established.
 *
 * The epilogue is `add sp, #8 / pop {r3} / mov r8, r3 / pop {r5, r6} /
 * pop {r0} / bx r0`, so r0 holds the popped return address and the owner
 * returns nothing: void.  The single argument arrives in r0, is saved to r5 in
 * the prologue, and is used only by the Func_0808a398 / Func_0808a060 pair in
 * the middle.
 *
 * This owner is called from Func_02000ac8, which passes it one of the fifteen
 * in-image draw ids at file offset 0x1fd0.
 *
 * Uncertainties:
 *  - Func_0808a018 at the top has no argument register set, and Func_0808a020
 *    at the bottom likewise; both are spelled with no arguments.  They are the
 *    established cutscene bracket.
 *  - The first note is followed by a 3-frame wait and no Func_080f9010(154);
 *    every later note gets the trigger and an 8-frame wait.  That asymmetry is
 *    in the bytes, not a transcription slip - Func_080f9010(148) at the top and
 *    Func_080f9010(126) mid-piece are the other two distinct trigger ids.
 *  - The imports' interfaces are unknown, hence the old-style declarations.
 */

#include "types.h"

void Func_0808a018();
void Func_0808a020();
void Func_0808a010();
void Func_0808a060();
void Func_0808a1b8();
void Func_0808a398();
void Func_08009180();
void Func_080f9010();

void Func_020004bc(s32 actor)
{
    Func_0808a018();
    Func_0808a010(30);
    Func_080f9010(148);
    Func_0808a010(100);
    Func_0808a1b8(0, 0xc000, 0);
    Func_0808a010(40);
    Func_08009180(82, 20, 70, 0, 3, 8);
    Func_0808a010(3);
    Func_08009180(85, 20, 70, 0, 3, 8);
    Func_080f9010(154);
    Func_0808a010(8);
    Func_08009180(88, 20, 70, 0, 3, 8);
    Func_080f9010(154);
    Func_0808a010(8);
    Func_08009180(91, 20, 70, 0, 3, 8);
    Func_080f9010(154);
    Func_0808a010(8);
    Func_08009180(94, 20, 70, 0, 3, 8);
    Func_080f9010(154);
    Func_0808a010(8);
    Func_08009180(97, 20, 70, 0, 3, 8);
    Func_080f9010(154);
    Func_0808a010(8);
    Func_08009180(100, 20, 70, 0, 3, 8);
    Func_080f9010(154);
    Func_0808a010(8);
    Func_08009180(79, 29, 70, 0, 3, 8);
    Func_080f9010(154);
    Func_0808a010(8);
    Func_08009180(82, 29, 70, 0, 3, 8);
    Func_080f9010(154);
    Func_0808a010(8);
    Func_08009180(85, 29, 70, 0, 3, 8);
    Func_080f9010(154);
    Func_0808a010(8);
    Func_08009180(88, 29, 70, 0, 3, 8);
    Func_080f9010(154);
    Func_0808a010(8);
    Func_08009180(91, 29, 70, 0, 3, 8);
    Func_080f9010(154);
    Func_0808a010(8);
    Func_08009180(94, 29, 70, 0, 3, 8);
    Func_080f9010(154);
    Func_0808a010(8);
    Func_08009180(97, 29, 70, 0, 3, 8);
    Func_080f9010(154);
    Func_0808a010(8);
    Func_08009180(100, 29, 70, 0, 3, 8);
    Func_080f9010(154);
    Func_0808a010(70);
    Func_080f9010(126);
    Func_0808a398(actor, 3);
    Func_0808a060(actor, 0);
    Func_0808a010(20);
    Func_08009180(97, 29, 70, 0, 3, 8);
    Func_080f9010(154);
    Func_0808a010(8);
    Func_08009180(94, 29, 70, 0, 3, 8);
    Func_080f9010(154);
    Func_0808a010(8);
    Func_08009180(91, 29, 70, 0, 3, 8);
    Func_080f9010(154);
    Func_0808a010(8);
    Func_08009180(88, 29, 70, 0, 3, 8);
    Func_080f9010(154);
    Func_0808a010(8);
    Func_08009180(85, 29, 70, 0, 3, 8);
    Func_080f9010(154);
    Func_0808a010(8);
    Func_08009180(82, 29, 70, 0, 3, 8);
    Func_080f9010(154);
    Func_0808a010(8);
    Func_08009180(100, 20, 70, 0, 3, 8);
    Func_080f9010(154);
    Func_0808a010(8);
    Func_08009180(97, 20, 70, 0, 3, 8);
    Func_080f9010(154);
    Func_0808a010(8);
    Func_08009180(94, 20, 70, 0, 3, 8);
    Func_080f9010(154);
    Func_0808a010(8);
    Func_08009180(91, 20, 70, 0, 3, 8);
    Func_080f9010(154);
    Func_0808a010(8);
    Func_08009180(88, 20, 70, 0, 3, 8);
    Func_080f9010(154);
    Func_0808a010(8);
    Func_08009180(85, 20, 70, 0, 3, 8);
    Func_080f9010(154);
    Func_0808a010(8);
    Func_08009180(82, 20, 70, 0, 3, 8);
    Func_080f9010(154);
    Func_0808a010(8);
    Func_08009180(79, 20, 70, 0, 3, 8);
    Func_080f9010(154);
    Func_0808a010(8);
    Func_0808a020();

}
