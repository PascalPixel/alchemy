#include "RESOURCE.H"
#include "FIELDOBJ.H"
#include "ANIMSPR.H"
#include "RUNTIME_MEM.H"
#include "DMA.H"

u8 *Object_GetByIdFar(s32 id);
void *Runtime_AllocateHeapBlockFar(s32 slot, s32 size);
void ItemIcon_LoadTilesFar(s32 item);
void ResourceMetadata_ClearRecordFar(void *record);

/* Colosso: when object id is in state 1, borrow heap block 17, clear its
 * 128-byte icon buffer, draw the item icon there, load it into the sprite
 * slot of the object and point its sprite at the new tiles. The
 * same function sits in each of the three Colosso trial overlays. */
void Korosseo_ShowItemIcon(s32 id, s32 item)
{
    struct FieldActor *obj;
    s32 one;
    struct AnimationObject *spr;
    s32 base;
    s32 tile;
    s32 none;
    volatile u32 zero;

    obj = (struct FieldActor *)Object_GetByIdFar(id);
    if (obj != 0) {
        one = obj->active;
        if (one == 1) {
            spr = (struct AnimationObject *)obj->sprite;
            base = (s32)Runtime_AllocateHeapBlockFar(17, 0x608);
            none = 0;
            base += 0x400;
            zero = none;
            Dma_Set((const void *)&zero, (void *)base, 0x85000020, (volatile u32 *)0x040000d4);
            ItemIcon_LoadTilesFar(item);
            tile = VramBlock_LoadCached(spr->slot, 128, base);
            Runtime_ReleaseHeapBlock(17);
            obj->unknown_5c = one;
            ResourceMetadata_ClearRecordFar(spr->entries[0]);
            spr->entries[0] = (void *)none;
            spr->count = none;
            spr->part[0].full_color = none;
            spr->part[0].tile = tile;
            spr->dirty = none;
            spr->flags = none;
        }
    }
}
