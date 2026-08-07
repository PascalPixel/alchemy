#include "types.h"

/*
 * Resource 39d record-flag initialiser at 0x02000ad0 (82 bytes, 7 calls).
 *
 * Derived span, not an inventory row: this owner has no row in
 * out/decomp/overlays.json and no exact sibling.  It was found by sweeping the
 * two-byte gaps in metrics/gs1-en-executable.json for `push {..,lr}`
 * prologues.  Walking from the prologue at 0x02000ad0 the body is entirely
 * straight-line — seven `bl`s and no conditional branch at all — and the walk
 * stops at the interworking return `pop {r5} / pop {r0} / bx r0` at
 * 0x02000b1c-0x02000b20.  So the executable extent is exactly
 * 0x02000ad0-0x02000b21 (82 bytes).  The popped register is r0, so the popped
 * value is the return address and the owner is `void`.
 *
 * Pool map: none — this owner loads no literal, which is why the seven record
 * indices are built with `movs` immediates.  The halfword at 0x02000b22 is
 * `0x0000` alignment before the next prologue at 0x02000b24 (`push {r5, r6,
 * lr}`), and the walk never reaches it as an instruction.
 *
 * All seven call sites were resolved with
 * `bun tools/lib/overlay_call_targets.ts resource_39d 0ad0 0b22`: every one of
 * them is veneer 0x020036f0 -> Func_0808a080, the established record fetch
 * that returns the record for the index in r0.  `overlay_show`'s printed
 * targets (0x20041c6, 0x20041d2, ...) are the pc-relative misreading and all
 * seven differ from each other despite being the same callee, which is the
 * textbook symptom of that bug.
 *
 * `adds r0, #85` on the returned pointer is a byte field at +0x55 in the
 * record, written with `strb`.  Record 0 gets 3; records 14 through 19 all get
 * the same 4, held in r5 across the whole run rather than re-materialised —
 * which is the evidence that the six stores really are one repeated value and
 * not six coincidentally equal immediates.
 *
 * The neighbouring owner at 0x02000b24 read-modify-writes the same +0x55 byte
 * with `ands #0xfa`, so the field is a bitmask rather than an enumeration;
 * this owner sets it wholesale, which is what an initialiser does.
 */

u8 *Func_0808a080();           /* record fetch, returns the record */

void Func_02000ad0(void)
{
    Func_0808a080(0)[0x55] = 3;
    Func_0808a080(14)[0x55] = 4;
    Func_0808a080(15)[0x55] = 4;
    Func_0808a080(16)[0x55] = 4;
    Func_0808a080(17)[0x55] = 4;
    Func_0808a080(18)[0x55] = 4;
    Func_0808a080(19)[0x55] = 4;
}
