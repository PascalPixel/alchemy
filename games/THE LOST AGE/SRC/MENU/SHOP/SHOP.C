#include "TYPES.H"

struct Record_080b06c0 {
    u8 filler0[4];
    u8 values[21];
};

/* The byte offset of each selector cell in the shop's tilemap. */
extern u16 Shop_SelectorOffsets[];

/* Each glyph's rows of bytes, and the byte offset of each glyph cell. */
extern u8 Shop_GlyphBytes[];
extern u16 Shop_GlyphRowOffsets[];

void Shop_FillSelector(s32 count, s32 selector, u8 *base)
{
    u32 shifted = selector << 4;
    u16 *offset;

    selector = shifted + 1;

    if (count > 0) {
        offset = Shop_SelectorOffsets;
        do {
            struct Record_080b06c0 *record = (struct Record_080b06c0 *)(base + *offset++);
            record->values[0] = selector;
            record->values[4] = selector;
            record->values[8] = selector;
            record->values[12] = selector;
            record->values[16] = selector;
            record->values[20] = selector;
            count--;
        } while (count != 0);
    }
}

/* ☀️'s: copy a glyph's rows of bytes into a shop window cell, stopping at
   each row's first empty byte. */
void Shop_CopyGlyphs(s32 arg0, s32 arg1, u32 arg2)
{
    u8 *src = Shop_GlyphBytes + ((u32)arg0 << 5);
    u32 offset = Shop_GlyphRowOffsets[arg2];
    s32 count = 3;
    u8 *dst;

    asm volatile("" : "+l"(count)); /* FAKEMATCH: ⚓️ sets the count in the offset load's delay slot */
    dst = (u8 *)((u32)arg1 + offset + 2);

    do {
        if (*src != 0) {
            dst[0] = *src++;
            if (*src != 0) {
                dst[1] = *src++;
                if (*src != 0) {
                    dst[30] = *src++;
                    if (*src != 0) dst[31] = *src++;
                }
            }
        }
        dst += 4;
        count--;
    } while (count >= 0);
}
