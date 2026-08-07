#include "types.h"

/*
 * Resource 3b3 proximity/priority merge at 0x020019f0 (172 bytes, 0 calls).
 *
 * Derived span, not an inventory row: this owner has no row in
 * out/decomp/overlays.json and no exact sibling.  It was found by sweeping the
 * two-byte gaps in metrics/gs1-en-executable.json for `push {..,lr}`
 * prologues.  Walking from the prologue at 0x020019f0, every branch
 * (0x020019fe, 0x02001a06, 0x02001a0e, 0x02001a16, 0x02001a20, 0x02001a26,
 * 0x02001a32, 0x02001a3c, 0x02001a44, 0x02001a4c, 0x02001a60) lands inside
 * 0x020019f0-0x02001a96, and the walk stops at the interworking return
 * `pop {r5, r6} / pop {r1} / bx r1` at 0x02001a96-0x02001a9a.  So the
 * executable extent is exactly 0x020019f0-0x02001a9b (172 bytes).  The
 * epilogue pops into r1, not r0, so by the epilogue rule r0 is the result —
 * and r0 is 0 on every rejecting path and 1 on the two accepting ones.
 *
 * Pool map — the walk never reaches these as instructions, and all three
 * disassemble as plausible code, which is the pool-as-instruction trap:
 *   0x02001a9c  0xfff00000   -0x100000
 *   0x02001aa0  0x0000ffff   the round-toward-zero bias
 *   0x02001aa4  0xffe00000   -0x200000
 * None is an address; all three are arithmetic operands well below the
 * 0x02008000 link base.  The next prologue begins at 0x02001aa8.
 *
 * There are no `bl`s at all: `bun tools/lib/overlay_call_targets.ts resource_3b3
 * 19f0 1a9c` reports `sites=0`, so the multiset is empty on both sides.
 *
 * The owner takes two objects and asks whether the second should inherit the
 * first's layer.  Three tests in sequence, and none of them is a plain range:
 *   * an exact three-way equality on +8/+12/+16 REJECTS (identical position is
 *     not a merge candidate), so the compare chain has a hole in it in the
 *     sense HANDOVER describes — the accepted set is a band minus its centre.
 *   * +8 must lie strictly inside +-0x100000 of the first object's, spelled as
 *     two one-sided tests around the biased value rather than as an absolute
 *     difference.
 *   * +12 is compared after `if (v < 0) v += 0xffff; v >>= 16`, which is
 *     round-TOWARD-ZERO division by 65536, not an arithmetic shift.  Dropping
 *     the bias changes the result for every negative coordinate.
 *   * +16 must be strictly below the first object's and no more than 0x200000
 *     under it — an asymmetric window, unlike the +8 test.
 *
 * `lsls #28 / lsrs #30` extracts bits 2-3 of the byte at +9 of the record the
 * object points to at +80: a two-bit layer.  When the first object's layer is
 * not greater, the owner accepts and changes nothing; only a strictly greater
 * layer is written down into the second object, together with bits 2-3 of the
 * neighbouring byte at +21.  `movs r2,#13 / negs r2` is the mask 0xfffffff3
 * that clears exactly those two bits, and it is reused for both fields.
 */

s32 Func_020019f0(u8 *source, u8 *target)
{
    s32 permuted_1;
    s32 sz = *(s32 *)(source + 16);
    s32 sx = *(s32 *)(source + 8);
    s32 tx = *(s32 *)(target + 8);
    s32 ty;
    s32 sy;
    u8 *source_record;
    u8 *target_record;
    s32 source_layer;
    s32 target_layer;

    if (tx == sx &&
        *(s32 *)(target + 12) == *(s32 *)(source + 12) &&
        *(s32 *)(target + 16) == sz) {
        return 0;
    }

    if (sx + -0x100000 >= tx) return 0;
    if (tx >= sx + 0x100000) return 0;

    if (ty < 0) ty += 0xffff;
    ty = *(s32 *)(target + 12);
    sy = *(s32 *)(source + 12);
    if (sy < 0) sy += 0xffff;
    if ((ty >> 16) != (sy >> 16)) return 0;

    if (sz <= *(s32 *)(target + 16)) return 0;
    if (sz + -0x200000 >= *(s32 *)(target + 16)) return 0;

    source_record = *(u8 **)(source + 80);
    target_record = *(u8 **)(target + 80);
    source_layer = (source_record[9] >> 2) & 3;
    target_layer = (target_record[9] >> 2) & 3;

    /* `bls` is an unsigned test, but both operands are a two-bit field, so an
     * ordinary comparison is exact here. */
    if (source_layer > target_layer) {
        target[35] &= 0xfe;
        permuted_1 = (u8)((target_record[9] & 0xf3) | (source_layer << 2));
        target_record[21] =
            (u8)((target_record[21] & 0xf3) | (source_record[21] & 12));
        target_record[9]  = permuted_1;
    }

    return 1;
}
