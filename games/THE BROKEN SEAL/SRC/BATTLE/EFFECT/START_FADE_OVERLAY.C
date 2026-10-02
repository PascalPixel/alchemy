#include "DMA.H"
#include "BATTLE_EFFECT_RUNTIME.H"
#include "VRAM_BLOCK.H"

u8 *Runtime_AllocateBlock(s32 slot, u32 size);
u8 *Runtime_AllocateHeapBlock(s32 slot, u32 size);
void Runtime_ReleaseHeapBlock(s32 slot);
void VramBlock_LoadCached(s32 slot, s32 size, const void *source);
void *ObjectTable_Get(u32 object);
void Object_EffectSpawnCallback(void);
s32 Map_GetTerrainHeightFar(s32 layer, s32 x, s32 z);
void Runtime_PushSlotEntry(void *entry, s32 value);

#define REG_BLDCNT (*(volatile u16 *)0x04000050)
#define REG_BLDALPHA (*(volatile u16 *)0x04000052)
#define REG_BLDY (*(volatile u16 *)0x04000054)

/* One sprite entry of the fade overlay, with its attribute words. */
struct FadeMark {
    void *next;
    u16 y : 8;
    u16 affine : 2;
    u16 mode : 2;
    u16 mosaic : 1;
    u16 colors : 1;
    u16 shape : 2;
    u16 x : 9;
    u16 param : 5;
    u16 size : 2;
    u16 tile : 10;
    u16 priority : 2;
    u16 palette : 4;
    u8 unknown_0a[2];
};

/* The object the overlay follows. */
struct FadeActor {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[2];
    s16 floor;
    u8 unknown_18[10];
    u8 layer;
};

struct FadeWork {
    struct FadeMark marks[2];
    struct FadeActor *actor;
};

struct FadeView {
    u8 unknown_00[228];
    s16 camera[4];
};

extern struct FadeWork *gActorEffectWork;
#define FADE_VIEW (*(struct FadeView **)((u8 *)&gActorEffectWork - 112))

/* Each frame, lay a semi-transparent mark over the tile to either side of
   the actor wherever the terrain a tile or two in front of it stands above
   the actor's floor, so the actor shows through what it walks behind. */
void Object_EffectSpawnCallback(void)
{
    struct FadeWork *work = gActorEffectWork;
    /* FAKEMATCH: the map work pointer is read 112 bytes below the effect
       work pointer, so both loads share one pool address, as the field
       effect module reaches it 84 bytes below the particle work. */
    s16 *camera = FADE_VIEW->camera;
    s32 camera_x = camera[1];
    s32 camera_z = camera[3];
    struct FadeMark *mark = work->marks;
    struct FadeActor *actor = work->actor;
    s32 x;
    s32 z;
    s32 floor;
    s32 layer;
    u32 tile;
    s32 height;
    s32 other;
    s32 row;
    /* FAKEMATCH: nothing reads or writes these four bytes; the reference
       frame holds them above the three values it keeps on the stack. */
    s16 unused[2];

    if (actor == 0)
        return;
    x = actor->x;
    z = actor->z;
    floor = actor->floor;
    layer = actor->layer;
    tile = gVramBlockCache[94].offset >> 5;
    x -= 0x80000;
    height = Map_GetTerrainHeightFar(layer, x, z + 0x100000) >> 16;
    other = (Map_GetTerrainHeightFar(layer, x, z + 0x200000) >> 16) - 16;
    if (other > height)
        height = other;
    if (height > 0 && height > floor) {
        ((u32 *)mark)[1] = 0x40000800;
        ((u32 *)mark)[2] = 0x400;
        mark->priority = 0;
        mark->tile = tile;
        mark->mode = 1;
        mark->x = ((u32)(x >> 16) & 0xfff0) - camera_x;
        row = (z >> 16) & 0xf0;
        mark->y = row - camera_z - height + 16;
        Runtime_PushSlotEntry(mark, 0);
    }
    x += 0x100000;
    height = Map_GetTerrainHeightFar(layer, x, z + 0x100000) >> 16;
    mark = &work->marks[1];
    other = (Map_GetTerrainHeightFar(layer, x, z + 0x200000) >> 16) - 16;
    if (other > height)
        height = other;
    if (height > 0 && height > floor) {
        ((u32 *)mark)[1] = 0x40000800;
        ((u32 *)mark)[2] = 0;
        mark->priority = 0;
        mark->tile = tile;
        mark->mode = 1;
        mark->x = ((u32)(x >> 16) & 0xfff0) - camera_x;
        row = (z >> 16) & 0xf0;
        mark->y = row - camera_z - height + 16;
        Runtime_PushSlotEntry(mark, 0);
    }
}

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
    Scheduler_AddOrUpdateCallback((s32)(Object_EffectSpawnCallback), 0xc80);
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
        object = ObjectTable_Get(Data_02000240.object_id);
    work[6] = (u32)object;
}
