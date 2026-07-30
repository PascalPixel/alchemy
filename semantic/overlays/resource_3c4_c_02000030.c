/*
 * resource_3c4 @ 0x02000030 (60 bytes: 54 code + alignment + one pool word).
 *
 * Three-dimensional distance between two 12.20 fixed-point position triples.
 * Each component difference is taken at full width and then arithmetically
 * shifted right by 16 before squaring, which keeps the sum of squares inside
 * 32 bits; the shift is `asrs`, so the arithmetic is signed throughout.
 *
 * The tail is an overlay call_via veneer, not an ordinary call: the pool word
 * at 0x02000068 is 0x030001d8 and is loaded into r3 immediately before
 * `bl 0x02003220`, which is this overlay's `bx r3` thunk.  0x030001d8 is the
 * relocated IWRAM square-root helper, so the veneer's real callee is that
 * helper and r0 carries the sum of squares.  r1 and r2 still hold the last two
 * squares at the call and are passed on unchanged, which is what the encoding
 * says; whether the helper reads them is not decidable from here.
 *
 * The epilogue is `pop {r5} ; pop {r1} ; bx r1`, so r0 survives and the
 * helper's result is this owner's result.
 */
typedef signed int s32;

typedef s32 (*SquareRoot_02000030)(s32 value);

/*
 * Overlay `bx r3` veneer.  The last parameter is the branch target, not data.
 */
s32 Func_02003220(s32 arg0, s32 arg1, s32 arg2, SquareRoot_02000030 target);

s32 Func_02000030(const s32 *a, const s32 *b)
{
    s32 dx = (a[0] - b[0]) >> 16;
    s32 dy = (a[1] - b[1]) >> 16;
    s32 dz = (a[2] - b[2]) >> 16;

    return Func_02003220(dx * dx + dy * dy + dz * dz, dz * dz, dy * dy,
                         (SquareRoot_02000030)0x030001d8);
}
