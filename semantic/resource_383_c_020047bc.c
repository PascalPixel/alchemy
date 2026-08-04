typedef signed int s32;

/*
 * resource_383 owner at 0x020047bc, 64 bytes (0x020047bc-0x020047fb), all
 * code; no literal pool.
 *
 * "Is entity `who` within 8 units of (x, z)?"  The entity record is fetched
 * through the import at veneer 0x02004ec4 (main image Func_0808a400).  Its
 * words at +56/+64 are the entity's destination coordinates and carry the
 * sentinel 0x80000000 when no destination is set; in that case the current
 * coordinates at +8/+16 are used instead.  Both are 16.16 fixed point, so the
 * `asrs #16` pairs reduce the differences to whole units before squaring, and
 * the test is `dx*dx + dz*dz <= 64`.
 *
 * 0x020047fc is the same routine with the threshold 256 instead of 64.
 *
 * `bl` target resolved with tools/overlay_call_targets.ts (target offset =
 * stored displacement + 2): the single site reaches the import veneer at
 * 0x02004ec4.  The disassembler's annotation for it is wrong.
 *
 * Epilogue is `pop {r5, r6} / pop {r1} / bx r1`, so r0 survives and is the
 * result.
 */

/* Import veneer at 0x02004ec4 -> main image Func_0808a400.  Returns the
 * entity record for the given handle. */
s32 *Func_0808a400();

s32 Func_020047bc(s32 x, s32 z, s32 who)
{
    s32 *entity;
    s32 ex;
    s32 ez;
    s32 dx;
    s32 dz;

    entity = Func_0808a400(who);

    ex = entity[14];                    /* +56 */
    if (ex == (s32)0x80000000) {
        ex = entity[2];                 /* +8 */
    }
    ez = entity[16];                    /* +64 */
    if (ez == (s32)0x80000000) {
        ez = entity[4];                 /* +16 */
    }

    dx = (ex - x) >> 16;
    dz = (ez - z) >> 16;

    if ((dx * dx + dz * dz) > 64) {
        return 0;
    }
    return 1;
}
