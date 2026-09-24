#include "TYPES.H"
#include "DMA.H"
#include "SYSTEM.H"
#include "RESOURCE.H"

s32 Math_Div(s32, s32);
s32 Math_Mod(s32, s32);
void Shop_CopyGlyphs(s32 digit, u8 *buf, s32 pos);
s32 VramBlock_LoadCached(s32 slot, s32 size, const void *src);
u8 *RenderOutput_CreateFar(s32 no, u32 flags, s32 window, s32 x, s32 y);

/* Builds a sprite showing a price of up to five digits, least significant
   digit first, over the blank price tiles. */
u8 *Shop_CreatePriceSprite(s32 value, s32 window, s32 x, s32 y)
{
    u8 *buf;
    s32 slot;
    u8 *sprite;

    buf = Runtime_AllocateBlock(14, 0x400);
    sprite = 0;
    Dma_Set((const void *)0x080b3e80, buf, 0x84000040, (volatile u32 *)0x040000d4);
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
