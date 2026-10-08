#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "RESOURCE.H"
#include "FIELDOBJ.H"
#include "FIELD_EVENT.H"
#include "ANIMSPR.H"
#include "MEM.H"
#include "DMA.H"
#include "IO_REG.H"

void Korosseo_ShowItemIcon(s32 id, s32 item)
{
    struct FieldActor *obj;
    s32 one;
    struct AnimationObject *spr;
    s32 base;
    s32 tile;
    s32 none;
    volatile u32 zero;

    obj = Object_GetById(id);
    if (obj != 0) {
        one = obj->active;
        if (one == 1) {
            spr = (struct AnimationObject *)obj->sprite;
            base = (s32)Runtime_AllocateHeapBlock(17, 0x608);
            none = 0;
            base += 0x400;
            zero = none;
            Dma_Set((const void *)&zero, (void *)base, DMA_ENABLE32 | DMA_32BIT | DMA_SRC_FIXED32 | 0x20, REG_DMA3);
            ItemIcon_LoadTilesFar(item);
            tile = Engine_VramLoad(spr->slot, 128, base);
            Runtime_ReleaseHeapBlock(17);
            obj->unknown_5c = one;
            ResourceMetadata_ClearRecord(spr->entries[0]);
            spr->entries[0] = (void *)none;
            spr->count = none;
            spr->part[0].full_color = none;
            spr->part[0].tile = tile;
            spr->dirty = none;
            spr->flags = none;
        }
    }
}
