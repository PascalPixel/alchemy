typedef signed int s32;

/*
 * Resource 3a6 camera-shift scene at 0x020018b4 (54 bytes, 7 calls).
 *
 * Derived span, not an inventory row: this owner has no row in
 * out/decomp/overlays.json and no exact sibling.  It was found by sweeping the
 * two-byte gaps in metrics/gs1-en-executable.json for `push {..,lr}`
 * prologues.  Walking from the prologue at 0x020018b4 the body is
 * straight-line — seven `bl`s, no conditional branch — and the walk stops at
 * the interworking return `pop {r0} / bx r0` at 0x020018e6-0x020018e8.  So the
 * executable extent is exactly 0x020018b4-0x020018e9 (54 bytes).  The popped
 * register is r0, so the popped value is the return address and the owner is
 * `void`.
 *
 * Pool map — the walk never reaches these as instructions:
 *   0x020018ea  0x0000       alignment
 *   0x020018ec  0x00000301   an identifier, not an address
 *   0x020018f0  0x00000121   an identifier, not an address
 * Both words are far below the 0x02008000 link base and both are passed as
 * arguments rather than dereferenced.  The next prologue begins at 0x020018f4.
 *
 * All seven call sites were resolved with
 * `bun tools/overlay_call_targets.ts resource_3a6 18b4 18ec`, assigning the
 * i-th occurrence in the source to the i-th site in address order:
 *   0x020018b8 -> veneer 0x02001d5c -> Func_080f9010
 *   0x020018c2 -> prologue 0x020017d8
 *   0x020018cc -> prologue 0x020017d8
 *   0x020018d2 -> veneer 0x02001c94 -> Func_080770c8
 *   0x020018d8 -> veneer 0x02001c1c -> Func_080000c0
 *   0x020018dc -> prologue 0x02001770
 *   0x020018e2 -> veneer 0x02001d5c -> Func_080f9010
 * Two of those (0x020018b8 and 0x020018e2) are the SAME callee under different
 * printed names in `overlay_show`, which is the pc-relative bug's signature;
 * the printed targets are not used.
 *
 * The mirror owner at 0x020018f4 is this routine with 112 and 80 negated and
 * the same 0x0301 identifier, so the two 0x020017d8 calls are a signed
 * displacement pair (an axis and its opposite), not two unrelated magnitudes.
 * Func_080770c8 is the event-flag import, so 0x0301 is this scene's flag id.
 */

void Func_080f9010();
void Func_020017d8();          /* in-overlay, three arguments */
void Func_080770c8();          /* event flag */
void Func_080000c0();          /* ROM dispatch stub table entry 0 */
void Func_02001770();          /* in-overlay, no arguments */

void Func_020018b4(void)
{
    Func_080f9010(241);
    Func_020017d8(11, 112, 0);
    Func_020017d8(11, 80, 0);
    Func_080770c8(0x301);
    Func_080000c0(2);
    Func_02001770();
    Func_080f9010(0x121);
}
