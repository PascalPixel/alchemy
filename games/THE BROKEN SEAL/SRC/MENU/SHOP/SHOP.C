#include "TYPES.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"
#include "DMA.H"
#include "SYSTEM.H"
#include "RESOURCE.H"
#include "SHOP.H"

extern u16 RomBytes_080b413c[];

/* shop/sel/fill.c */
struct Record_080b06c0 {
    u8 filler0[4];
    u8 values[21];
};

extern u16 RomBytes_080b4100[];

/* shop/draw/glyphs.c */
extern u8 Shop_GlyphBytes[];

s32 Math_Div(s32, s32);
s32 Math_Mod(s32, s32);
s32 VramBlock_LoadCached(s32 slot, s32 size, const void *src);
u8 *RenderOutput_CreateFar(s32 no, u32 flags, s32 window, s32 x, s32 y);

void Shop_CopyGlyphs(s32 arg0, s32 arg1, u32 arg2);

void Shop_FillSelector(s32 count, s32 selector, u8 *base)
{
    u32 shifted = selector << 4;
    u16 *offset;

    selector = shifted + 1;

    if (count > 0) {
        offset = RomBytes_080b4100;
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

/* 4行分の非0バイトを指定配置へ順にコピーする。 */
void Shop_CopyGlyphs(s32 arg0, s32 arg1, u32 arg2)
{
    u8 *src = Shop_GlyphBytes + ((u32)arg0 << 5);
    u8 *dst =
        (u8 *)((u32)arg1 + RomBytes_080b413c[arg2] + 2);
    s32 count = 3;

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

/* Builds a sprite showing a price of up to five digits, least significant
   digit first, over the blank price tiles. */
u8 *Shop_CreatePriceSprite(s32 value, s32 window, s32 x, s32 y)
{
    u8 *buf;
    s32 slot;
    u8 *sprite;

    buf = Runtime_AllocateBlock(14, 0x400);
    sprite = 0;
    Dma_Set(Shop_PriceTiles, buf, 0x84000040, (volatile u32 *)0x040000d4);
    Shop_CopyGlyphs(Math_Mod(value, 10), buf, 0);
    value = Math_Div(value, 10);
    if (value != 0) {
        Shop_CopyGlyphs(Math_Mod(value, 10), buf, 1);
        value = Math_Div(value, 10);
        if (value != 0) {
            Shop_CopyGlyphs(Math_Mod(value, 10), buf, 2);
            value = Math_Div(value, 10);
            if (value != 0) {
                s32 last;

                Shop_CopyGlyphs(Math_Mod(value, 10), buf, 3);
                last = Math_Div(value, 10);
                if (last != 0)
                    Shop_CopyGlyphs(Math_Mod(last, 10), buf, 4);
            }
        }
    }
    slot = Resource_FindFreeEntry();
    if (slot != 96) {
        VramBlock_LoadCached(slot, 0x100, buf);
        sprite = RenderOutput_CreateFar(slot, 0x80008000, window, x, y);
    }
    Runtime_ReleaseHeapBlock(14);
    return sprite;
}
