typedef signed int s32;

/*
 * resource_3aa owner at 0x02001450, 68 bytes: code 0x02001450-0x02001493 with
 * no literal pool (every constant is `movs` + `lsls`).  The next inventory row
 * starts at 0x02001494, so the owner is exactly its advertised span and
 * nothing live escapes it.
 *
 * This is the sibling of 0x0200140c: byte-for-byte the same shape, differing
 * only in the four constants, which here are all 0xc000.  It sets the same
 * channel property on channels 0..3 to one common value and then waits the
 * caller's delay when it is non-zero.
 *
 * Return type, by the interworking-epilogue rule: `pop {r5} / pop {r0} /
 * bx r0` pops the return address into r0, so the owner is void.
 *
 * Signature: r0 is saved into r5 by the prologue and used only as the delay
 * test and the delay argument, so the owner takes exactly one argument.
 *
 * Call accounting: five `bl` sites, all resolved with
 * `bun tools/overlay_call_targets.ts resource_3aa 1450` - four to the import
 * veneer at 0x02001b34 (main-image Func_0808a1b8) and one to 0x02001a7c
 * (main-image Func_0808a010, the frame wait).  The four setter branches are
 * bit-identical `f001 fd99` pairs printed as four different addresses by the
 * disassembler; they are one callee.  That this owner and 0x0200140c produce
 * the same five resolved targets from the same encodings is itself the
 * cheapest confirmation of the overlay `bl` rule here.
 */

/* Imports via the veneers at 0x02001b34 and 0x02001a7c. Old-style: arities
 * vary per site across this overlay. */
void Func_0808a1b8();
void Func_0808a010();

void Func_02001450(s32 delay)
{
    Func_0808a1b8(0, 0xc000, 0);
    Func_0808a1b8(1, 0xc000, 0);
    Func_0808a1b8(2, 0xc000, 0);
    Func_0808a1b8(3, 0xc000, 0);

    if (delay != 0) {
        Func_0808a010(delay);
    }
}
