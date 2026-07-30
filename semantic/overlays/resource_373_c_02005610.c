typedef signed int s32;

/*
 * Resource 373: squared-distance-then-square-root of two 16.16 fixed-point
 * 3-vectors.  This is a byte-identical twin of the routine at 0x02000030 in
 * the same overlay; only the encoded veneer address differs.
 *
 * The owner is complete: `push {r5, lr}` at 0x02005610 and the matching
 * `pop {r5} ; pop {r1} ; bx r1` at 0x02005642..0x02005646, with a two-halfword
 * literal pool (0x030001d8) following it.  No live frame or register state
 * escapes the row.
 *
 * The trailing call is a `call_via r3` interworking thunk, exactly like the
 * main image's `Func_080072f0`: r3 is loaded from the pool with 0x030001d8 —
 * the ARM-mode helper relocated into IWRAM — and the `bl` reaches an
 * interworking veneer that does `bx r3`.  r3 is therefore the callee, never
 * scratch.  Its argument is the accumulated squared length in r0 and its
 * result is returned unchanged by this routine, which identifies it as the
 * integer square root helper.
 *
 * UNCERTAINTY: the veneer address encoded here (0x0200b7a0) lies past the end
 * of this overlay's image, while the twin at 0x02000030 encodes 0x020061c0,
 * which lands inside it.  Overlay `bl` displacements are evidently fixed up
 * when the module is loaded, so an encoded address is a stable identity for
 * the veneer rather than something a reader can disassemble.  Both are
 * recorded exactly as encoded.
 */

/*
 * Interworking veneer: transfers to the ARM helper whose address is in r3.
 * The first three parameters are the helper's own arguments; only the first
 * is live here, so the other two are spelled 0 (the original leaves whatever
 * the distance computation happened to leave in r1/r2, which the one-argument
 * helper ignores).
 */
typedef s32 (*Resource373IwramHelper)(s32 value);
s32 Func_0200b7a0(s32 value, s32 unusedR1, s32 unusedR2,
                  Resource373IwramHelper viaR3);

s32 Func_02005610(const s32 *a, const s32 *b)
{
    /*
     * The original interleaves post-indexed loads from both vectors; the
     * observable effect is the three componentwise differences below.  Each
     * difference is arithmetically shifted down by 16, so the products are
     * formed from the integer part of a 16.16 fixed-point delta.
     */
    s32 dx = (a[0] - b[0]) >> 16;
    s32 dy = (a[1] - b[1]) >> 16;
    s32 dz = (a[2] - b[2]) >> 16;
    s32 lengthSquared = dx * dx + dy * dy + dz * dz;

    /* r3 = 0x030001d8, the relocated IWRAM square-root helper. */
    return Func_0200b7a0(lengthSquared, 0, 0,
                         (Resource373IwramHelper)0x030001d8);
}
