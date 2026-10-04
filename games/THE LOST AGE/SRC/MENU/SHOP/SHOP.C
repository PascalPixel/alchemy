#include "TYPES.H"
#include "DMA.H"
#include "RESOURCE.H"
#include "RENDER_INPUT.H"
#include "RUNTIME_MEM.H"

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

/* The blank price tiles the digits are drawn over. */
extern const u8 Shop_PriceTiles[];

/* ☀️'s, staging in ⚓️'s heap block 56: builds a sprite showing a price of
   up to five digits, least significant digit first, over the blank price
   tiles. */
struct RenderOutput *Shop_CreatePriceSprite(s32 value, s32 window, s32 x, s32 y)
{
    u8 *buf;
    s32 slot;
    struct RenderOutput *sprite;

    buf = Runtime_AllocateBlock(56, 0x400);
    sprite = 0;
    Dma_Set(Shop_PriceTiles, buf, 0x84000040, (volatile u32 *)0x040000d4);
    Shop_CopyGlyphs(value % 10, (s32)buf, 0);
    value = value / 10;
    if (value != 0) {
        Shop_CopyGlyphs(value % 10, (s32)buf, 1);
        value = value / 10;
        if (value != 0) {
            Shop_CopyGlyphs(value % 10, (s32)buf, 2);
            value = value / 10;
            if (value != 0) {
                s32 last;

                Shop_CopyGlyphs(value % 10, (s32)buf, 3);
                last = value / 10;
                if (last != 0)
                    Shop_CopyGlyphs(last % 10, (s32)buf, 4);
            }
        }
    }
    slot = Resource_FindFreeEntry();
    if (slot != 96) {
        VramBlock_LoadCached(slot, 0x100, buf);
        sprite = RenderOutput_CreateFar(slot, 0x80008000, (struct RenderInput *)window, x, y);
    }
    Runtime_ReleaseHeapBlock(56);
    return sprite;
}
