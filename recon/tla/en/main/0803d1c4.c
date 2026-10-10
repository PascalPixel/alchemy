/* Pin-free trial (tla-en): listing 132 instructions, draft 132, common 61, differ 142.
 * Earlier register-allocation notes describe the prior pinned form.
 */
/* differ 6: ☀️ UiText_DecodeSymbol; the native loads the bit count before addressing the leaves, and the second leaf byte before masking the first; only ARM9TDMI load scheduling differs */
#include "graphics.h"
#include "text_reader.h"
#include "types.h"
extern const void *const Text_MessageContexts[][2];

s32 Func_0803d1c4(struct TextReader *reader)
{
    u32 code;
    u32 hi;
    /* FAKEMATCH: tree pinned to r1, as the native allocates it (62 instructions differ without it). */
    const u8 *tree;
    const u8 *leaves;
    const u8 *text;
    s32 text_bits;
    s32 tree_bits;
    s32 rank;
    /* FAKEMATCH: depth pinned to r3, as the native allocates it (92 differ without it). */
    s32 depth;
    s32 bit;
    u32 index;
    u32 half;

    code = reader->previous_code;
    text = (const u8 *)reader->words;
    text_bits = reader->bits;
    tree_bits = 1;
    hi = code >> 8;
    code &= 0xff;
    tree = Text_MessageContexts[hi][0];
    tree = tree + ((const u16 *)Text_MessageContexts[hi][1])[code];
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
        code |= (u32)bit >> 4;
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
