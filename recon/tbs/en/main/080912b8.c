/* Not exact: 102 on the permuter scorer, 2 stack offsets. Every instruction
   and the literal pool match; the reference frame is 16 bytes with the top
   word unused (floor at sp, the camera pair at sp+4 and sp+8), where this
   builds 12. A four-byte stack object allocated before reload, never read
   or written, would account for it; none is known. The map work is reached
   112 bytes below the effect work label, as EFFECT2.C reaches it at 84. */
#include "DMA.H"
#include "BATTLE_EFFECT_RUNTIME.H"
#include "VRAM_BLOCK.H"

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

s32 Map_GetTerrainHeightFar(s32 layer, s32 x, s32 z);
void Runtime_PushSlotEntry(void *entry, s32 value);
extern struct FadeWork *gActorEffectWork;
#define FADE_VIEW (*(struct FadeView **)((u8 *)&gActorEffectWork - 112))

void Object_EffectSpawnCallback(void)
{
    struct FadeWork *work = gActorEffectWork;
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
