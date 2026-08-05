#include "types.h"

/*
 * Resource 3b3 descent loop at 0x02001174 (80-byte whole-owner span, 2 calls).
 *
 * Derived span, not an inventory row: this owner has no row in
 * out/decomp/overlays.json and no exact sibling.  It was found by sweeping the
 * two-byte gaps in metrics/gs1-en-executable.json for `push {..,lr}`
 * prologues.  Walking from the prologue at 0x02001174, the two branches
 * (`bhi` at 0x0200118a to 0x020011ac and `bgt` at 0x020011a8 back to
 * 0x02001188) both stay inside the body, and the walk stops at the
 * interworking return `pop {r5, r6, r7} / pop {r0} / bx r0` at
 * 0x020011ac-0x020011b0.  The executable extent is therefore 62 bytes; the
 * whole-owner span is 80 bytes after annexing its alignment and literal pool
 * through 0x020011c3.  The popped register is r0, so the popped value is the
 * return address and the owner is `void`.
 *
 * Pool map — the walk never reaches these as instructions:
 *   0x020011b2  0x0000       alignment
 *   0x020011b4  0x00001999   the clamp value
 *   0x020011b8  0xffffe667   -0x1999
 *   0x020011bc  0xffff3334   -0xcccc
 *   0x020011c0  0x00001998   the loop bound
 * None is an address; 0x020011b8 in particular decodes as a plausible `b.n`
 * and 0x020011ba as a NEON instruction, which is the "a pool word disassembles
 * as code" trap — all four are reached only through `ldr rN,[pc,#imm]`.  They
 * belong to this owner, and the next prologue begins at 0x020011c4.
 *
 * Both call sites are named at their PC-RELATIVE-DECODED addresses:
 *   0x02001176 -> Func_02003c94   (veneer physically at 0x02002b1c)
 *   0x0200118e -> Func_02003bc4   (veneer physically at 0x02002a34)
 * An overlay `bl` does not store the assembler's PC-relative displacement, so
 * decoding the reference encoding as if it did yields the address the symbol
 * must carry for the assembler to reproduce those bytes. That address depends
 * on the CALL SITE, not just the callee -- which is the mechanical reason the
 * craft rule says symbols are per-site. Naming the veneer's physical address
 * instead leaves the displacement low halfword wrong (see resource_382:3ac,
 * where this rule was worked out).
 * The first `bl` is the very first instruction after the `push`, so r0 is
 * still this owner's own argument when it is made — the record index is passed
 * straight through rather than materialised.
 *
 * -0x1999 is -0.1 and -0xcccc is -0.8 in 16.16, and the bound 0x1998 is one
 * less than the clamp 0x1999: the loop steps the record down by a tenth per
 * frame until it is at or below a tenth, then pins it to exactly a tenth.  The
 * counter caps the run at 32 frames, and THE TWO EXITS ARE NOT THE SAME — the
 * cap exits without pinning.  Folding them together would delete the clamp.
 *
 */

u8 *Func_02003c94();           /* record fetch, returns the record */
void Func_02003bc4();          /* ROM dispatch stub table entry 0 */

void Func_02001174(s32 index)
{
    u8 *record = Func_02003c94(index);
    u32 frames;

    record[0x55] = 0;

    frames = 0;
    for (;;) {
        if (frames > 31) return;
        Func_02003bc4(1);
        *(s32 *)(record + 28) += -0x1999;
        *(s32 *)(record + 12) += -0xcccc;
        frames++;
        if (*(s32 *)(record + 28) <= 0x1998) {
            *(s32 *)(record + 28) = 0x1999;
            return;
        }
    }
}
