typedef signed int s32;

/*
 * Resource 3c8 at 0x020002f0: distance between two 16.16 fixed-point
 * 3-vectors.
 *
 * The owner is complete: `push {r5, lr}` at 0x020002f0 and the matching
 * `pop {r5} ; pop {r1} ; bx r1` at 0x02000322..0x02000326, followed by a
 * two-halfword literal pool (0x02000328 = 0x030001d8) that is data, not code.
 * No live frame or register state escapes the row.
 *
 * `pop {r1} ; bx r1` (N != 0) leaves r0 alive across the return, so the
 * trailing call's result is this routine's result.
 *
 * The trailing `bl 0x02005330` is an overlay `call_via r3` veneer, the same
 * construct as the main image's `Func_080072f0`: r3 is loaded from the pool
 * with 0x030001d8 - the ARM-mode helper relocated into IWRAM - immediately
 * before the branch, and the veneer does `bx r3`.  r3 is the callee, never
 * scratch.  Its argument is the accumulated squared length in r0 and its
 * result is returned unchanged, which identifies it as the integer square
 * root helper.  A byte-identical construct is already recorded for
 * resource_373 (0x02000030 / 0x02005610) and resource_3bf.
 *
 * UNCERTAINTY: 0x02005330 falls numerically inside this overlay's own image,
 * where the reconstruction holds descriptor/import data rather than code.
 * Overlay `bl` displacements are evidently fixed up when the module is
 * loaded, so the encoded address is a stable identity for the veneer, not a
 * place a reader can disassemble.  It is recorded as-encoded, which is what
 * the byte-exact `assets/code/resource_3c8_c_*.c` sources already do.
 */

/*
 * Interworking veneer: transfers to the ARM helper whose address is in r3.
 * The first three parameters are the helper's own arguments.  Only the first
 * is live: at the branch r1 holds dz*dz and r2 holds dy*dy, leftovers of the
 * accumulation that the one-argument helper ignores.  They are spelled 0
 * here, matching the resource_373 twin.
 */
typedef s32 (*IwramHelper_020002f0)(s32 value);
s32 Func_02005330(s32 value, s32 unusedR1, s32 unusedR2,
                  IwramHelper_020002f0 viaR3);

s32 Func_020002f0(const s32 *a, const s32 *b)
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
    return Func_02005330(lengthSquared, 0, 0,
                         (IwramHelper_020002f0)0x030001d8);
}
