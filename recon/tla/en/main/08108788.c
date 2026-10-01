/* Near miss: one reordered instruction by objdump: ⚓️ sets the loop count
   (movs r4, #3) straight after loading the row offset, a load-delay fill;
   with -mtune=arm9tdmi this draft compiles exactly. The two tables take
   names in ⚓️'s listings. */
#include "TYPES.H"
#include "SCENE.H"

extern u8 Shop_GlyphBytes[];
extern u16 Shop_GlyphRowOffsets[];

/* shop/sel/fill.c */
struct Record_080b06c0 {
    u8 filler0[4];
    u8 values[21];
};

extern u16 RomBytes_080b4100[];

void Shop_CopyGlyphs(s32 arg0, s32 arg1, u32 arg2)
{
    u8 *src = Shop_GlyphBytes + ((u32)arg0 << 5);
    u32 offset = Shop_GlyphRowOffsets[arg2];
    s32 count = 3;
    u8 *dst = (u8 *)((u32)arg1 + offset + 2);

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
