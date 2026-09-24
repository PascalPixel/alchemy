#include "DMA.H"
#include "BATTLE_EFFECT_RUNTIME.H"

u8 *Runtime_AllocateBlock(s32 slot, u32 size);
u8 *Runtime_AllocateHeapBlock(s32 slot, u32 size);
void Runtime_ReleaseHeapBlock(s32 slot);
void VramBlock_LoadCached(s32 slot, s32 size, const void *source);
s32 Scheduler_AddOrUpdateCallback(void *callback, s32 priority);
void *ObjectTable_Get(u32 object);
void Object_EffectSpawnCallback(void);

#define REG_BLDCNT (*(volatile u16 *)0x04000050)
#define REG_BLDALPHA (*(volatile u16 *)0x04000052)
#define REG_BLDY (*(volatile u16 *)0x04000054)

void BattleFx_StartFadeOverlay(void *object)
{
    u32 *work;
    u8 *tiles;
    volatile u32 fill;
    volatile u16 *blend;
    s32 value;

    work = (u32 *)Runtime_AllocateBlock(36, 28);
    tiles = Runtime_AllocateHeapBlock(14, 0x400);
    fill = 0x11111111;
    Dma_Set((const void *)&fill, tiles, 0x85000080, (volatile u32 *)0x040000d4);
    VramBlock_LoadCached(94, 0x200, tiles);
    Runtime_ReleaseHeapBlock(14);
    Scheduler_AddOrUpdateCallback(Object_EffectSpawnCallback, 0xc80);
    /* FAKEMATCH: the blend values pass through an int and one register
       pointer so GCC builds them with mov and steps the address. */
    value = 0x3f9e;
    blend = &REG_BLDCNT;
    *blend = value;
    value = 16;
    *++blend = value;
    value = 31;
    *++blend = value;
    fill = 0;
    Dma_Set((const void *)&fill, work, 0x85000007, (volatile u32 *)0x040000d4);
    if (object == NULL)
        object = ObjectTable_Get(gGameState.object_id);
    work[6] = (u32)object;
}
