typedef signed int s32;

/*
 * resource_381 owner at 0x02000038, 4 bytes: `movs r0, #0 / bx lr`.
 *
 * HEAD EXPORT STUB. Published from this overlay's own header word at image
 * offset 0x2c -- the same slot on eight of the ten overlays that carry a
 * head export table, which is a layout fact worth more than the body.
 *
 * Complete owner: both instructions. No prologue, no stack frame, no literal
 * pool, no callees, no argument read. SPAN IS 4 BYTES and there is nothing
 * past the `bx lr` to annex.
 *
 * The body is shared with dozens of rows tree-wide; the identity is not.
 * This row is bounded by ITS overlay's neighbours and published from ITS
 * overlay's header slot.
 *
 * 0 of 0 callees.
 */

s32 Func_02000038(void)
{
    return 0;
}
