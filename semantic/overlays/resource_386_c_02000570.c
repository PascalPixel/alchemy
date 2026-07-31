typedef unsigned char u8;
typedef signed int s32;

/*
 * Resource 386 actor-16-slot reset at 0x02000570.
 *
 * Complete owner: `push {r5, r6, r7, lr}` at 0x02000570, interworking return
 * `pop {r5, r6, r7} / pop {r0} / bx r0` at 0x020005d4-0x020005d8.  The
 * control-flow walk reaches every halfword to 0x020005d8; the two bytes at
 * 0x020005da are alignment, and 0x020005dc is the first entry of the import
 * veneer band.  The 106-byte span is therefore all code.
 *
 * Signature: `pop {r0} / bx r0` — the popped value is the return address, so
 * the owner is `void`.
 *
 * Argument.  Only r0 is read (copied to r7 at 0x02000572 and passed on to
 * Func_08015250).  Its sole caller, the initialiser at 0x020004e4, sets r0-r3
 * before the `bl`; r1-r3 are dead here.  That is recorded rather than
 * "fixed": the bytes are not in doubt, and the caller's file declares this
 * routine old-style so the extra registers it sets stay visible there.
 *
 * The early return is a genuine guard, not a merged arm: when Func_080090c8
 * reports no slot, nothing at all is done.
 *
 * Import naming uses the overlay `bl` rule (stored displacement + 2) against
 * the veneer band at 0x020005dc-0x020006dc.  This owner is the only user of
 * four of those veneers (0x080001c8, 0x08000140, 0x08000150, 0x08015250),
 * which is consistent with it being a one-off slot reset.
 */

u8  *Func_080090c8();  /* veneer 0x5fc — slot lookup, 0 when absent */
s32  Func_08000140();  /* veneer 0x5e4 */
void Func_08000150();  /* veneer 0x5ec */
void Func_080001c8();  /* veneer 0x5f4 */
void Func_08015250();  /* veneer 0x60c */

void Func_02000570(s32 arg0)
{
    u8 *slot;
    u8 *record;
    s32 buffer;

    slot = Func_080090c8(22);
    if (slot == 0) {
        return;
    }

    record = *(u8 **)(slot + 0x50);
    record[0x26] = 0;
    record[0x27] = 0;
    /* `movs r3,#33 / negs r3` is the mask -33 = 0xdf: it clears bit 0x20 and
     * leaves bit 0x01 alone, despite the 33 in the source. */
    record[5] = (u8) (record[5] & (u8) -33);
    record[9] = (u8) (record[9] & 0x0f);

    slot[0x55] = 0;
    slot[0x5c] = 1;

    /* r1 is built as 193 << 3. */
    buffer = Func_08000140(17, 0x608);
    Func_08015250(arg0);
    /* r3 is built as 128 << 3. */
    buffer += 0x400;
    Func_080001c8(record[28], 128, buffer);
    Func_08000150(17);
}
