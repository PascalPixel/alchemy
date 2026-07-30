typedef signed int s32;

/*
 * resource_3aa owner at 0x0200140c, 68 bytes: code 0x0200140c-0x0200144f with
 * no literal pool at all (every constant is built with `movs` + `lsls`).  The
 * next inventory row starts at 0x02001450, so the owner is exactly its
 * advertised span and nothing live escapes it.
 *
 * One scripted step: set the same channel property on channels 0..3 to four
 * fixed values, then, when the caller passed a non-zero delay, wait that many
 * frames.
 *
 * Return type, by the interworking-epilogue rule: `pop {r5} / pop {r0} /
 * bx r0` pops the return address into r0, so nothing is returned - the owner
 * is void.
 *
 * Signature: r0 is copied straight into the callee-saved r5 by the prologue
 * and is only used as the delay test and the delay argument, so the owner
 * takes exactly one argument.  r1-r3 are overwritten before any use.
 *
 * Call accounting: five `bl` sites, all resolved with
 * `bun tools/overlay_call_targets.ts resource_3aa 140c` - four to the import
 * veneer at 0x02001b34 (main-image Func_0808a1b8) and one to the veneer at
 * 0x02001a7c (main-image Func_0808a010).  The four are *bit-identical*
 * halfword pairs (`f001 fd99`) that the disassembler prints as four different
 * addresses; that is the usual overlay `bl` mis-decoding and they are one
 * callee.  Func_0808a010(frames) is the wait import documented in
 * semantic/overlays/resource_373_c_02002cb0.c; Func_0808a1b8(selector, value,
 * mode) is the same three-argument setter that file also uses, with its third
 * argument 0 here throughout.
 *
 * This owner and its neighbour at 0x02001450 are the near-identical sibling
 * pair HANDOVER §0 describes: same 68 bytes, same instruction sequence, and
 * they differ only in the four constants and in which shift builds them.
 *
 * Uncertainty: the meaning of the four values (0x6000, 0xe000, 0x2000,
 * 0xa000) is not established.  They are plain immediates, they are what the
 * sibling varies, and 0x2000/0x6000/0xa000/0xe000 are 0x4000-spaced, which
 * reads as a quarter-turn orientation per channel; nothing here proves it.
 */

/* Imports via the veneers at 0x02001b34 and 0x02001a7c. Old-style: arities
 * vary per site across this overlay. */
void Func_0808a1b8();
void Func_0808a010();

void Func_0200140c(s32 delay)
{
    Func_0808a1b8(0, 0x6000, 0);
    Func_0808a1b8(1, 0xe000, 0);
    Func_0808a1b8(2, 0x2000, 0);
    Func_0808a1b8(3, 0xa000, 0);

    if (delay != 0) {
        Func_0808a010(delay);
    }
}
