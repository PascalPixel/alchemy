/* DRAFT of Func_08019bfc, a byte-wise Thumb twin of the ARM symbol decoder
   (DECODE_SYMBOL.S): walk the code tree chosen by the previous character,
   reading tree bits and message bits through one-bit readers whose buffer
   carries a sentinel 1 above the unread bits, skip a whole subtree for
   each set message bit while counting its leaves, then fetch the leaf's
   12-bit value from the table packed backwards before the tree.
   2026-10-04: rewritten as plain structured C without steering devices;
   for (;;) with a break at the leaf gives the reference's layout (entry
   branch to the tree read at the bottom). Score 1065 (the old goto draft
   with tagged temporaries scored 1170). Every loop instruction has the
   reference's shape; left: the reference keeps the tree cursor in r1 and
   the read bit in r2 (here exchanged, 50 register-only lines), and loads
   the tree base and offset table as [T, hi*8] then [T, hi*8 + 4] before
   the low byte's offset, where the struct index forms T + hi*8 first.
   Pointer-array or 2D-array tables give that addressing but move every
   other register (1275-1660); hi/lo temporaries do not help.
   2026-10-04 (finisher 5), instruction diff 123: the greg dump explains
   the swap. bit (84 uses) is allocated first, but global.c's
   regs_someone_prefers keeps it off r3 (tree prefers r3) and r2 (rank,
   pseudo 40, prefers r2 through the tail's rank * 3 temporaries), so bit
   takes r1 and tree r2; the native bit can take r2, so its rank carries no
   r2 preference. Tried without effect: separate bit variables per reader
   (123), rank as u32 (123), rank + rank * 2 (125), rank *= 3 (157),
   (index & 1) parity (130), reusing code for the result as the native's r5
   suggests (125), a pointer for the leaf pair (123), pins of bit r2 (144)
   or tree r1 (135). The [][2] table with code >> 8 / code &= 0xff gives the
   native head sequence exactly but reallocates the loop (184). */
#include "TYPES.H"
#include "TEXT_READER.H"

/* One code tree per previous character: the tree bits, with the leaf
   values packed as 12-bit entries just before them, and per-character
   offsets of each tree. */
struct TextContext {
    const u8 *trees;
    const u16 *offsets;
};

extern const struct TextContext Text_MessageContexts[];

s32 Func_08019bfc(struct TextReader *reader)
{
    u32 code;
    const struct TextContext *context;
    const u8 *tree;
    const u8 *leaves;
    const u8 *text;
    s32 text_bits;
    s32 tree_bits;
    s32 rank;
    s32 depth;
    s32 bit;
    s32 value;
    u32 index;

    code = reader->previous_code;
    context = &Text_MessageContexts[code >> 8];
    tree = context->trees + context->offsets[code & 0xff];
    text = (const u8 *)reader->words;
    text_bits = reader->bits;
    tree_bits = 1;
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
    if (((index * 4) & 7) == 0)
        value = (leaves[-(index >> 1)] << 4) | (leaves[-(index >> 1) - 1] >> 4);
    else
        value = ((leaves[-(index >> 1)] & 0xf) << 8) | leaves[-(index >> 1) - 1];
    reader->bits = text_bits;
    reader->previous_code = value;
    reader->words = (const u32 *)text;
    return value;
}
