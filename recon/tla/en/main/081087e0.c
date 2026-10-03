#include "TYPES.H"
#include "RENDER_INPUT.H"
#include "SYSTEM.H"
#include "RESOURCE.H"

s32 __divsi3(s32, s32);
s32 __modsi3(s32, s32);
void Shop_CopyGlyphs(s32 digit, u8 *buf, s32 pos);
s32 VramBlock_LoadCached(s32 slot, s32 size, const void *src);

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
    Shop_CopyGlyphs(__modsi3(value, 10), buf, 0);
    value = __divsi3(value, 10);
    if (value != 0) {
        Shop_CopyGlyphs(__modsi3(value, 10), buf, 1);
        value = __divsi3(value, 10);
        if (value != 0) {
            Shop_CopyGlyphs(__modsi3(value, 10), buf, 2);
            value = __divsi3(value, 10);
            if (value != 0) {
                s32 last;

                Shop_CopyGlyphs(__modsi3(value, 10), buf, 3);
                last = __divsi3(value, 10);
                if (last != 0)
                    Shop_CopyGlyphs(__modsi3(last, 10), buf, 4);
            }
        }
    }
    slot = Resource_FindFreeEntry();
    if (slot != 96) {
        VramBlock_LoadCached(slot, 0x100, buf);
        sprite = (u8 *)RenderOutput_CreateFar(slot, 0x80008000, (struct RenderInput *)window, x, y);
    }
    Runtime_ReleaseHeapBlock(14);
    return sprite;
}
