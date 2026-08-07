#include "types.h"

/*
 * Resource 3a0 one-shot scene at 0x02000cec (28 bytes, 4 calls).
 *
 * Derived span, not an inventory row: this owner has no row in
 * out/decomp/overlays.json and no exact sibling.  It was found by sweeping the
 * two-byte gaps in metrics/gs1-en-executable.json for `push {..,lr}`
 * prologues.  Walking from the prologue at 0x02000cec the body is
 * straight-line — four `bl`s, no conditional branch — and the walk stops at
 * the interworking return `pop {r0} / bx r0` at 0x02000d04-0x02000d06.  So the
 * executable extent is exactly 0x02000cec-0x02000d07 (28 bytes).  The popped
 * register is r0, so the popped value is the return address and the owner is
 * `void`.
 *
 * Pool map — the walk never reaches these as instructions:
 *   0x02000d08  0x000017f7   a small identifier, not an address
 *   0x02000d0c                the next prologue (`push {r5, r6, r7, lr}`)
 * 0x000017f7 is far below the 0x02008000 link base and is passed as an
 * argument rather than dereferenced, so it is a value; there is no alignment
 * halfword between the pool and the next owner.
 *
 * All four call sites were resolved with
 * `bun tools/lib/overlay_call_targets.ts resource_3a0 0cec 0d08`, assigning the
 * i-th occurrence in the source to the i-th site in address order:
 *   0x02000cee -> veneer 0x020013c0 -> Func_0808a018
 *   0x02000cf4 -> veneer 0x02001460 -> Func_0808a170
 *   0x02000cfc -> veneer 0x02001478 -> Func_0808a190
 *   0x02000d00 -> veneer 0x020013c8 -> Func_0808a020
 * `overlay_show`'s printed targets are the pc-relative misreading and are not
 * used.
 *
 * First `Func_0808a018` and last `Func_0808a020` is the scene bracket the
 * overlay reconstruction's convention predicts, and it is the cheapest check that the
 * call assignment above is in the right order.  The bracket is symmetric here:
 * there is no arm that reaches the close without the open, so nothing needs
 * preserving in the asymmetric form documented for resource_3ae.
 */

void Func_020020b0();          /* scene open */
void Func_02002156();
s32 Func_02002176();
void Func_020020ca();          /* scene close */

void Func_02000cec(void)
{
    Func_020020b0();
    Func_02002156(0x17f7);
    Func_02002176(17, 0);
    Func_020020ca();
}
