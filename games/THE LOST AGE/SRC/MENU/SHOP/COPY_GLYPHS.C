#include "TYPES.H"

extern u8 Shop_GlyphBytes[];
extern u16 Shop_GlyphRowOffsets[];

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
