#include "TYPES.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"
#include "SYSTEM.H"
#include "DMA.H"

extern u8 ResourceTableEntries[];
extern u8 gCam[];

/* map/shared/Map_RenderAnimatedTileFrame.c */
struct MapBase {
    u16 unused;
    u16 offset;
};

extern u8 Map_TileDissolveOrder[];

/* map/shared/render_animated_tile_frames_for_object.c */
void Map_RenderAnimatedTileFrame(u8 *object, u32 position);

/* map/shared/render_all_animated_tile_frames.c */
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

/* map/shared/get_screen_relative_position.c */
struct Thing {
    u8 filler0[8];
    s32 field8;
    u8 filler12[4];
    s32 field16;
};

/* The object system state block (92 bytes). */
struct ObjectSystem {
    u8 unk_00[6];
    u8 priority;
    u8 flag;
    u8 unk_08[84];
};

s32 Scheduler_AddOrUpdateCallback(void *callback, s32 priority);
void ObjectSystem_Configure(s32 mode);
void Object_UpdateAllMotion(void);
void Object_UpdateAllThumb(void);
void ObjectSystem_UpdateCameraFixed(void);
void ObjectSystem_UpdateCamera(void);
extern s32 Data_03001d1c;
extern s32 Data_03001cc0;

void Map_RenderAnimatedTileFrame(u8 *object, u32 position)
{
    u16 *destination;
    s32 count;
    u32 row;
    u8 *table;
    u32 high_mask;
    u32 index_mask;
    u8 offset_mask;

    destination = (u16 *)(0x06010000
        + ((struct MapBase *)((u32)&ResourceTableEntries))[object[0x1C]].offset);
    count = (object[0x20] * object[0x21]) / 64;
    row = 0;

    if (row < (u32)count) {
        table = Map_TileDissolveOrder;
        high_mask = 0xFF00;
        index_mask = 0x3F;
        offset_mask = 0x3E;

        /* 1行ごとに32セル進める。 */
        for (; row < (u32)count; row++, destination += 32, position++) {
            if ((u32)(position - 0x40) <= 0x3F) {
                u8 entry;
                u16 *cell;
                u32 parity;

                entry = table[(position + row * 16) & index_mask];
                cell = (u16 *)((u8 *)destination + (entry & offset_mask));
                parity = 1;
                parity &= entry;
                if (parity != 0)
                    *cell = *(u8 *)cell;
                else
                    *cell &= high_mask;
            }
        }
    }
}

void Map_RenderAnimatedTileFramesForObject(u8 *object)
{
    u32 pos;

    pos = 0;
    do {
        Map_RenderAnimatedTileFrame(object, pos);
        Map_RenderAnimatedTileFrame(object, pos + 1);
        Map_RenderAnimatedTileFrame(object, pos + 2);
        Map_RenderAnimatedTileFrame(object, pos + 3);
        pos += 4;
        WaitFrames(1);
    } while (pos <= 0x7f);
}

void Map_RenderAllAnimatedTileFrames(u8 **tbl, s32 cnt)
{
    u8 **top;
    u8 **p;
    s32 n;
    u32 pos;
    u32 pos1;
    u32 pos2;
    u32 pos3;

    top = tbl;
    pos = 0;
    do {
        if (cnt > 0) {
            pos1 = pos + 1;
            pos2 = pos + 2;
            p = top;
            pos3 = pos + 3;
            n = cnt;
            do {
                Map_RenderAnimatedTileFrame(*p, pos);
                Map_RenderAnimatedTileFrame(*p, pos1);
                Map_RenderAnimatedTileFrame(*p, pos2);
                n -= 1;
                Map_RenderAnimatedTileFrame(*p++, pos3);
            } while (n != 0);
        }
        WaitFrames(1U);
        pos += 4;
    } while (pos <= 0x7FU);
}

s32 Map_GetScreenRelativePosition(struct Thing *obj, s32 *out)
{
    u8 *state = *(u8 **)((u32)&gCam);
    s32 *org = (s32 *)(state + 228);
    s32 a;
    s32 b;
    s32 x;
    s32 y;

    a = org[0] & 0xffff0000;
    b = org[1] & 0xffff0000;
    x = obj->field8 - a;
    y = obj->field16 - b;
    if ((u32)(x + 0x001fffff) <= 0x012ffffe && y > 0 && y < 0xe00000) {
        *out++ = x >> 16;
        *out = y >> 16;
        return 0;
    }
    *out++ = 0;
    *out = 0;
    return -1;
}

/* Allocates and clears the object system state and the object table,
   configures the system for the mode and schedules the object update (the
   motion-only one in mode 4) and the camera (fixed in modes 3 and 4). */
void ObjectSystem_Initialize(s32 mode)
{
    struct ObjectSystem *state;
    u8 *objects;
    volatile u32 fill;

    state = (struct ObjectSystem *)Runtime_AllocateBlock(6, 92);
    objects = Runtime_AllocateBlock(5, 0x1c00);
    ObjectSystem_Configure(mode);
    fill = 0;
    Dma_Set((const void *)&fill, objects, 0x85000700, (volatile u32 *)0x040000d4);
    fill = 0;
    Dma_Set((const void *)&fill, state, 0x85000017, (volatile u32 *)0x040000d4);
    if (mode == 4)
        Scheduler_AddOrUpdateCallback(Object_UpdateAllMotion, 0xc8a);
    else
        Scheduler_AddOrUpdateCallback(Object_UpdateAllThumb, 0xc8a);
    if ((u32)(mode - 3) <= 1) {
        Scheduler_AddOrUpdateCallback(ObjectSystem_UpdateCameraFixed, 0xc80);
    } else {
        Scheduler_AddOrUpdateCallback(ObjectSystem_UpdateCamera, 0xc80);
        Data_03001d1c = 0;
        Data_03001cc0 = 0;
    }
    state->priority = 15;
    state->flag = 0;
}

void ResourceTable_ReservedNoOpC0C4(void)
{
}

/* The second empty routine after the resource table code; nothing in the
   image calls it. */
void ResourceTable_ReservedNoOp2(void)
{
}
