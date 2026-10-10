/* Pin-free UiText_DecodeSymbol trial: native and built 134 instructions,
 * common 131, differ 6 in all six TBS editions, both with a 0x110-byte extent.
 * All traversal registers and instructions match. The first packed-leaf
 * case copies r2 to r3, shifts back into r2, then branches to a shared OR;
 * native shifts r2 into r3 and ORs within that case. Not adopted.
 * Removing both pins alone gives 134 built, 66 common, differ 136.
 * Declaration/read-order permutations (8,308 cases), separate bit scopes,
 * bit-reader helpers, pointer qualifiers, equivalent depth loops, CSE block
 * wrappers and forced temporaries did not close that allocation mismatch.
 * A 17,033-candidate search found a widened identity form, differ 47; its
 * reader alias and reuse of index for the tree bit are unnecessary.
 * Split restoration of tree_bits after context offset addition reduces
 * that to differ 7: 135 built, 131 common, 0x114 bytes, with one extra final
 * copy and corresponding pool/branch offsets. Restoring it after rank=0
 * instead gives differ 19; placing it after hi or code setup gives 15.
 * Signed all-ones masks, direct widening casts, 32/33-bit masks, widening the
 * whole result and masking before a wide shift revert to differ 136. Wide
 * shifts 32/36, 48/52 and 56/60 give 136 built, differ 48 before restoration.
 * Unsigned wide mask, +0, *1, /1 and union low-word access retain the copy.
 * Split restores at final byte or inside the traversal, reused shift
 * temporaries and equivalent switch depth updates did not remove it.
 * A further 38,849-candidate search did not beat the 7-difference form.
 * Even-arm OR wrappers and split restores, odd-arm cast/reuse variants,
 * widening earlier bit tests and alternate signed leaf carriers did not
 * improve this 6-difference draft. Widening rank before index setup gives
 * 32 differences with the native final shift/OR restored, but swaps rank
 * and the 128 sentinel between lr/r8 and changes final address copies.
 * No compiler route was changed.
 */
#include "text_reader.h"

extern const void *const Text_MessageContexts[][2];

s32 UiText_DecodeSymbol(struct TextReader *reader)
{
    u32 code;
    u32 hi;
    const u8 *tree;
    const u8 *leaves;
    const u8 *text;
    s32 text_bits;
    s32 tree_bits;
    s32 rank;
    s32 depth;
    s32 bit;
    u32 index;
    u32 half;
    s32 saved_bits;

    code = reader->previous_code;
    text = (const u8 *)reader->words;
    text_bits = reader->bits;
    tree_bits = 1;
    hi = code >> 8;
    code &= 0xff;
    tree = Text_MessageContexts[hi][0];
    tree = tree + ((const u16 *)Text_MessageContexts[hi][1])[code];
    /* FAKEMATCH: the split restore ends the context setup's CSE block.
     * Both conditions disappear, keeping tree/tree_bits in r1/r4 instead
     * of r4/r1. Putting this after rank initialization swaps rank/sentinel
     * from lr/r8 to r8/lr and gives 19 rather than 7 differences. */
    saved_bits = tree_bits;
    if (code)
        tree_bits = 0;
    if (code)
        tree_bits = saved_bits;
    leaves = tree - 1;
    rank = 0;
    for (;;) {
        bit = tree_bits & 1;
        tree_bits >>= 1;
        if (bit != 0 && tree_bits == 0) {
            tree_bits = *tree++;
            bit = tree_bits & 1;
            tree_bits = (tree_bits >> 1) | 0x80;
        }
        if (bit != 0)
            break;
        bit = text_bits & 1;
        text_bits >>= 1;
        if (bit != 0 && text_bits == 0) {
            text_bits = *text++;
            bit = text_bits & 1;
            text_bits = (text_bits >> 1) | 0x80;
        }
        if (bit == 0)
            continue;
        depth = 0;
        do {
            bit = tree_bits & 1;
            tree_bits >>= 1;
            if (bit != 0 && tree_bits == 0) {
                tree_bits = *tree++;
                bit = tree_bits & 1;
                tree_bits = (tree_bits >> 1) | 0x80;
            }
            if (bit == 0)
                depth++;
            else {
                rank++;
                depth--;
            }
        } while (depth >= 0);
    }
    index = rank * 3;
    if (((index * 4) & 7) == 0) {
        code = leaves[-(index >> 1)] << 4;
        bit = leaves[-(index >> 1) - 1];
        /* FAKEMATCH: a widened packed-byte temporary keeps bit/depth in
         * r2/r3 instead of r3/r2. Updating bit before the OR removes the
         * extra instruction but merges that OR with the other leaf case. */
        bit = (u32)(bit & ~(u64)0) >> 4;
        code |= (u32)bit;
    } else {
        half = leaves[-(index >> 1)];
        half &= 0xf;
        code = leaves[-(index >> 1) - 1] | (half << 8);
    }
    reader->bits = text_bits;
    reader->previous_code = code;
    reader->words = (const u32 *)text;
    return code;
}
