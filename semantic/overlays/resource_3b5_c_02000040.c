typedef signed int s32;

/*
 * resource_3b5 owner at 0x02000040, 60 bytes: squared distance between two 16.16
 * fixed-point 3-vectors, followed by the integer square root.
 *
 * Complete owner: 'push {r5, lr}' and the matching 'pop {r5} ; pop {r1} ; bx r1',
 * with a two-halfword literal pool (0x030001d8) following it.
 *
 * TRANSPOSED from semantic/overlays/resource_373_c_02000030.c.  The two owners
 * are the same routine shared verbatim: over all 30 halfwords they differ in
 * exactly 2 places, both halves of the single BL pair.  The pool word 0x030001d8
 * is byte-identical, so the ARM-mode IWRAM square-root helper is the same.
 *
 * What was changed: the entry symbol, and the call, re-resolved with
 * 'bun tools/overlay_call_targets.ts resource_3b5 0040' - one site, classified
 * call_via, reaching this overlay's own interworking veneer at 0x02000e78
 * (resource_373's is at 0x020061c0).  The veneer does 'bx r3', and r3 is loaded
 * from the pool with 0x030001d8, so r3 is the callee and never scratch.
 */

/*
 * Interworking veneer: transfers to the ARM helper whose address is in r3.
 * The first three parameters are the helper's own arguments; only the first
 * is live here, so the other two are spelled 0 (the original leaves whatever
 * the distance computation happened to leave in r1/r2, which the one-argument
 * helper ignores).
 */
typedef s32 (*IwramHelper)(s32 value);
s32 Func_02000e78(s32 value, s32 unusedR1, s32 unusedR2,
                  IwramHelper viaR3);

s32 Func_02000040(const s32 *a, const s32 *b)
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
    return Func_02000e78(lengthSquared, 0, 0,
                         (IwramHelper)0x030001d8);
}
