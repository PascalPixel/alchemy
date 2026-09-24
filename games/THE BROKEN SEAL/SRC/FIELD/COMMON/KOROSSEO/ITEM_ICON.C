#include "DMA.H"

struct KorosseoSprite {
    u8 pad00[5];
    u8 low : 5;
    u8 hidden : 1;
    u8 high : 2;
    u8 pad06[2];
    u16 tile : 10;
    u16 attr : 6;
    u8 pad0a[18];
    u8 palette;
    u8 pad1d[8];
    u8 flag25;
    u8 flag26;
    u8 flag27;
    void *record;
};

u8 *Object_GetByIdFar(s32 id);
s32 Runtime_AllocateHeapBlockFar(s32 slot, s32 size);
void Runtime_ReleaseHeapBlock(s32 slot);
void ItemIcon_LoadTilesFar(s32 item);
s32 VramBlock_LoadCached(s32 slot, s32 size, s32 source);
void ResourceMetadata_ClearRecordFar(void *record);

/* Colosso: when object id is in state 1, borrow heap block 17, clear its
 * 128-byte icon buffer, draw the item icon there, load it into the sprite
 * slot of the object and point its sprite at the new tiles. The
 * same function sits in each of the three Colosso trial overlays. */
void Korosseo_ShowItemIcon(s32 id, s32 item)
{
    u8 *obj;
    s32 one;
    struct KorosseoSprite *spr;
    s32 base;
    s32 tile;
    s32 none;
    volatile u32 zero;

    obj = Object_GetByIdFar(id);
    if (obj != 0) {
        one = obj[84];
        if (one == 1) {
            spr = *(struct KorosseoSprite **)(obj + 80);
            base = Runtime_AllocateHeapBlockFar(17, 0x608);
            none = 0;
            base += 0x400;
            zero = none;
            Dma_Set((const void *)&zero, (void *)base, 0x85000020, (volatile u32 *)0x040000d4);
            ItemIcon_LoadTilesFar(item);
            tile = VramBlock_LoadCached(spr->palette, 128, base);
            Runtime_ReleaseHeapBlock(17);
            obj[92] = one;
            ResourceMetadata_ClearRecordFar(spr->record);
            spr->record = (void *)none;
            spr->flag27 = none;
            spr->hidden = none;
            spr->tile = tile;
            spr->flag25 = none;
            spr->flag26 = none;
        }
    }
}
