#include "TYPES.H"
#include "DMA.H"
#include "MAP.H"
#include "RAM_BUFFER.H"

struct PerspectiveWork {
    u8 unknown_000[0x10];
    void *unknown_010;              /* 0x010 */
    u16 fade;                       /* 0x014 */
    u8 fade_step;                   /* 0x016 */
    u8 unknown_017[0xe4 - 0x17];
    s32 scroll_x;                   /* 0x0e4 */
    s32 scroll_y;                   /* 0x0e8 */
    s32 scale_x;                    /* 0x0ec */
    s32 scale_y;                    /* 0x0f0 */
    s32 limit_x;                    /* 0x0f4 */
    s32 limit_y;                    /* 0x0f8 */
    u8 unknown_0fc[4];
    u16 window_top;                 /* 0x100 */
    u16 window_bottom;              /* 0x102 */
    u8 unknown_104[0xc];
    void *tiles;                    /* 0x110 */
    u8 unknown_114[4];
    u16 pitch;                      /* 0x118 */
    u16 yaw;                        /* 0x11a */
    u8 unknown_11c[0x1c];
    u16 lines[256];                 /* 0x138 */
    u8 unknown_338[0x10];
    s32 far_plane;                  /* 0x348 */
    s32 distance;                   /* 0x34c */
    u8 unknown_350[4];
    s32 zoom;                       /* 0x354 */
    u16 turn;                       /* 0x358 */
    u8 unknown_35a[2];
};

struct PerspectiveCamera {
    u8 unknown_00[0xc];
    s32 position[3];                /* 0x0c */
    s32 unknown_18;                 /* 0x18 */
    s32 unknown_1c;                 /* 0x1c */
    u8 unknown_20[0x2c];
};

struct PerspectiveVector {
    s32 x;
    s32 y;
    s32 z;
};

extern char ResourceId_PerspectiveDataA;
extern char ResourceId_DefaultMapCells;
extern char ResourceId_DefaultMapAnimation;
extern char ResourceId_DefaultMetatileAttributes;
void Transform_UpdateVertices(void);
extern u8 Transform_UpdateVerticesSize;
extern u32 gProjection[];
extern u32 Data_03001f60;
extern u32 Data_03001af4;
extern u32 gFrameCount;
extern void *gWorkSlot[];
extern u16 gBgScroll[];
void Blend_SetDarkenTarget0(s32);
s32 Runtime_AllocateHeapBlock(s32, s32);
void *Runtime_AllocateBlock(s32, s32);
void *Resource_GetTableEntry(s32);
s32 Resource_DecodeType01(const void *source, void *destination);
void MapAnimation_StartChannels(void *);
void Camera_StoreSceneParameters(u32, u32, u32);
void Render_ResetTransformState(void);
void SceneTransform_ApplyPosition(s32 *);
void SceneTransform_ApplyYaw(s32);
void SceneTransform_ApplyPitch(s32);
void Graphics_PrepareTransferInIwramWork(s32, s32);
s32 Trig_Cos(s32);
s32 Trig_Sin(s32);
void WorldMap_BuildScanlineTable(s32, s32 *, void *);
s32 Scheduler_AddOrUpdateCallback(s32, s32);
void MapAnimation_ApplyAffineFrame(void);
void WorldMap_UpdateView(void);

static __inline__ void Io_Set16(s32 value, u16 *reg)
{
    *reg = value;
}

static __inline__ void Io_Put16(u16 *reg, s32 value)
{
    *reg = value;
}

typedef s32 (*RatioFn)(s32, s32);
typedef s32 (*PlaneFn)(void *camera, s32 *position, void *lines, void *out);

static __inline__ void Transform(struct PerspectiveVector *vector,
                                struct PerspectiveCamera *camera,
                                s32 (*routine)(struct PerspectiveVector *,
                                                struct PerspectiveCamera *))
{
    routine(vector, camera);
}

#define ABS(v) ((v) < 0 ? -(v) : (v))

struct MapTileWindow_08010d48 {
    s32 *position;
    u8 unknown_004[0x134];
    u16 tiles[16][16];
};

extern struct MapTileWindow_08010d48 *gMapWork;
s32 Map_WriteLayerCellTile(s32 layer, s32 x, s32 y, s32 tile, s32 update);

u32 Runtime_BumpAllocate(s32 size);
void Runtime_BumpFree(void *block);
s32 Resource_DecodeByteLz(const void *source, void *destination);
void Map_UpdateCurrentTileBlock(void);
extern u8 *gCam;
extern u32 Data_080132cc[][6];

struct WorldCell {
    u32 tile : 24;
    u32 kind : 7;
    u32 flag : 1;
};

#define BG_PALETTE ((s16 *)0x05000000)
#define WORLD_CELLS ((struct WorldCell *)Ram_MapBlocks)

struct WorldMapState {
    u8 unk_000[0x11c];
    u32 *resources;
    u8 unk_120[0x2c];
    u16 animated_a[3];
    u8 unk_152[0x1a];
    u16 animated_b[3];
};

/* Exact (2026-09-30): complete 864-byte extent, 0 differing halfwords.
 * The ROM keeps several statement groups in source order where sched2
 * would otherwise hoist constants and argument loads: GCC 2.96 treats a
 * loop note as a scheduling barrier, so four do-while(0) groups (the
 * BG2 PA..PC writes, the rest of the affine block, the vertex-routine DMA
 * and the DISPCNT write) reproduce those barriers. The 0x10 slot is a
 * pointer (its alias set frees the store from the int-field stores), the
 * turn address is taken before the camera stores, and the pitch cache is
 * read through work->pitch so its load depends on the preceding store.
 */
/* Sets up the tilted-plane map view: clears the scene work, loads the map
   graphics and animation, programs the two affine backgrounds, builds the
   camera and projects the plane once, then tilts the camera down to its
   resting pitch and starts the per-frame callbacks. */
s32 Map_InitializePerspectiveScene(void)
{
    struct PerspectiveWork *work;
    struct PerspectiveCamera *camera;
    void *tiles;
    u8 *lines;
    s32 *position;
    s32 *distance;
    u16 *yaw;
    u16 *pitch;
    u16 *turn;
    volatile u32 fill;
    struct PerspectiveVector vector;
    s32 far_plane;
    u32 size;
    s32 i;

    *(volatile u16 *)0x04000000 &= 0xc1ff;
    Blend_SetDarkenTarget0(0);
    work = (struct PerspectiveWork *)Runtime_AllocateHeapBlock(8, sizeof(struct PerspectiveWork));
    fill = 0;
    Dma_Set(&fill, work, 0x85000000 | (sizeof(struct PerspectiveWork) / 4), (volatile u32 *)0x040000d4);
    work->scroll_x = 0;
    work->scroll_y = 0;
    work->scale_x = 0x200000;
    work->scale_y = 0x400000;
    work->limit_x = 0x1fe00000;
    work->limit_y = 0x1fe00000;
    work->unknown_010 = 0;
    work->tiles = Resource_GetTableEntry((s32)&ResourceId_PerspectiveDataA);
    Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_DefaultMapAnimation), Ram_MapCollision + 0x1000);
    MapAnimation_StartChannels(Ram_MapCollision + 0x1000);
    Io_Set16(0x3f9e, (u16 *)0x04000050);
    Io_Set16(0x1010, (u16 *)0x04000052);
    *(u16 *)0x04000054 = 0;
    Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_DefaultMapCells), Ram_MapCellBuffer);
    Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_DefaultMetatileAttributes), Ram_MapCollision);
    work->fade = 0x1f00;
    work->fade_step = 0x80;
    Io_Set16(0xa80a, (u16 *)0x0400000e);
    Io_Set16(0xaa0e, (u16 *)0x0400000c);
    Io_Set16(0x0501, (u16 *)0x0400000a);
    /* FAKEMATCH: loop note keeps BG2PA's constant after its address */
    do {
        Io_Put16((u16 *)0x04000020, 0x100);
        *(u16 *)0x04000022 = 0;
        *(u16 *)0x04000024 = 0;
    } while (0);
    /* FAKEMATCH: loop notes keep BG2PD's constant late and r0 first below */
    do {
        Io_Put16((u16 *)0x04000026, 0x100);
        *(s32 *)0x04000028 = 0;
        *(s32 *)0x0400002c = 0;
        Io_Put16((u16 *)0x04000030, 0x100);
        *(u16 *)0x04000032 = 0;
        *(u16 *)0x04000034 = 0;
        Io_Put16((u16 *)0x04000036, 0x100);
        *(s32 *)0x04000038 = 0;
        *(s32 *)0x0400003c = 0;
    } while (0);

    camera = Runtime_AllocateBlock(12, sizeof(struct PerspectiveCamera));
    tiles = (void *)Runtime_AllocateHeapBlock(7, 0x3484);
    position = camera->position;
    lines = (u8 *)tiles + 0xc80;
    far_plane = 0x1fe0000;
    work->far_plane = far_plane;
    distance = &work->distance;
    *distance = far_plane;
    work->zoom = 0x10000;
    turn = &work->turn;
    camera->unknown_18 = 0;
    camera->unknown_1c = 0;
    *turn = 0;
    gProjection[3] = 120;
    gProjection[4] = 96;
    Camera_StoreSceneParameters(far_plane, far_plane >> 1, far_plane << 1);
    position[0] = 0;
    position[1] = 0;
    position[2] = 0;
    Render_ResetTransformState();
    SceneTransform_ApplyPosition(position);
    yaw = &work->yaw;
    SceneTransform_ApplyYaw(*yaw);
    pitch = &work->pitch;
    SceneTransform_ApplyPitch(*pitch);
    vector.x = 0;
    vector.y = 0;
    vector.z = far_plane;
    Transform(&vector, camera,
              (s32 (*)(struct PerspectiveVector *, struct PerspectiveCamera *))0x03000250);
    Render_ResetTransformState();
    Graphics_PrepareTransferInIwramWork((s32)camera, (s32)position);
    /* FAKEMATCH: loop note orders the transform call's r1, routine, r0 */
    do {
        size = (s32)&Transform_UpdateVerticesSize;
        Dma_Set((void *)Transform_UpdateVertices, (void *)Runtime_AllocateHeapBlock(46, size),
                0x84000000 | (size >> 2), (volatile u32 *)0x040000d4);
    } while (0);
    WorldMap_BuildScanlineTable(((RatioFn)0x0300013c)(Trig_Cos(*pitch), Trig_Sin(*pitch)),
                  position, tiles);
    Data_03001f60 = 0;
    Data_03001af4 = work->pitch;
    ((PlaneFn)gWorkSlot[46])(camera, position, tiles,
                                 lines + (gFrameCount & 1) * 0x1400);
    position[0] = 0;
    position[1] = 0;
    position[2] = 0;
    Render_ResetTransformState();
    Io_Put16(pitch, 0xe000);
    *yaw = 0;
    Render_ResetTransformState();
    SceneTransform_ApplyPosition(position);
    SceneTransform_ApplyYaw(*yaw);
    SceneTransform_ApplyPitch(*pitch);
    vector.x = 0;
    vector.y = 0;
    vector.z = *distance + 0x10000;
    Transform(&vector, camera,
              (s32 (*)(struct PerspectiveVector *, struct PerspectiveCamera *))0x03000250);
    *(volatile u16 *)0x0400004c = 0;
    /* FAKEMATCH: loop notes keep the display writes in source order */
    do {
        Io_Put16((u16 *)0x04000000, 0x42);
    } while (0);
    gBgScroll[2] = 0;
    gBgScroll[3] = 0;
    gBgScroll[4] = 0;
    gBgScroll[5] = 0;
    gBgScroll[6] = 0;
    gBgScroll[7] = 0;
    work->window_top = 0;
    work->window_bottom = 159;
    Scheduler_AddOrUpdateCallback((s32)WorldMap_UpdateView, 0xc85);
    Scheduler_AddOrUpdateCallback((s32)MapAnimation_ApplyAffineFrame, 0x480);
    for (i = 255; i >= 0; i--)
        work->lines[i] = i;
}

void Map_SetWindowCellTile(s32 x, s32 y, s32 px, s32 py)
{
    struct MapTileWindow_08010d48 *window;
    s32 *position;
    s32 origin_x;
    s32 origin_y;
    s32 tile;

    window = gMapWork;
    origin_x = 0;
    origin_y = 0;
    position = window->position;
    if (position != 0) {
        origin_x = *position++;
        origin_y = position[1];
    }

    origin_x >>= 24;
    origin_y >>= 24;
    x >>= 4;
    y >>= 4;
    px >>= 3;
    py >>= 3;
    tile = (y << 4) + x;
    window->tiles[(py / 2) & 15][(px / 2) & 15] = tile;

    if (ABS(origin_x - px) <= 1 && ABS(origin_y - py) <= 1) {
        Map_WriteLayerCellTile(0, px / 2, py / 2, tile, 1);
        Map_WriteLayerCellTile(1, px / 2, py / 2, tile + 0x140, 1);
    }
}

/* Loads the world map graphics for the region around (x, z): the palette
   (keeping the backdrop colour), four tile banks and the cell graphics,
   clears the overlay map, lays out the fixed tile grid, and for the sea
   variant also sets up its animated tiles. */
void WorldMap_LoadGraphics(s32 x, s32 z)
{
    struct WorldMapState *state;
    u32 *resources;
    u8 *buffer;
    u8 *tiles;
    u32 *cursor;
    u32 tile;
    s32 variant;
    s32 row;
    s32 col;
    s16 value;
    volatile u32 fill;

    variant = 0;
    buffer = (u8 *)Runtime_BumpAllocate(0x200);
    state = (struct WorldMapState *)gCam;
    if (WORLD_CELLS[((x / 0x200000) & 31) + (((z / 0x200000) & 31) << 5)].kind == 21)
        variant = 1;
    resources = Data_080132cc[variant];
    state->resources = resources;
    value = BG_PALETTE[0];
    Resource_DecodeByteLz((const void *)Resource_GetTableEntry(resources[0]), buffer);
    *(s16 *)buffer = value;
    Dma_Set(buffer, BG_PALETTE, 0x84000070, (volatile u32 *)0x040000d4);
    tiles = gBgTileBuffer;
    Resource_DecodeType01((const void *)Resource_GetTableEntry(resources[1]), tiles);
    Dma_Set(tiles, (void *)0x06008000, 0x84000800, (volatile u32 *)0x040000d4);
    Resource_DecodeType01((const void *)Resource_GetTableEntry(resources[2]), Ram_BgTileBuffer + 0x2000);
    Dma_Set(Ram_BgTileBuffer + 0x2000, (void *)0x0600a000, 0x84000800, (volatile u32 *)0x040000d4);
    Resource_DecodeType01((const void *)Resource_GetTableEntry(resources[3]), Ram_BgTileBuffer + 0x4000);
    Dma_Set(Ram_BgTileBuffer + 0x4000, (void *)0x0600c000, 0x84000800, (volatile u32 *)0x040000d4);
    Resource_DecodeType01((const void *)Resource_GetTableEntry(resources[4]), Ram_BgTileBuffer + 0x6000);
    Dma_Set(Ram_BgTileBuffer + 0x6000, (void *)0x0600e000, 0x84000800, (volatile u32 *)0x040000d4);
    Resource_DecodeType01((const void *)Resource_GetTableEntry(resources[5]), (void *)gMapLayerData);
    fill = 0xf07ff07f;
    Dma_Set((const void *)&fill, (void *)0x06002800, 0x85000180, (volatile u32 *)0x040000d4);
    cursor = (u32 *)0x06003000;
    tile = 0x01a901a8;
    for (row = 0; row < 20; row++) {
        for (col = 0; col < 15; col++) {
            *cursor++ = tile;
            tile += 0x00020002;
        }
        cursor++;
    }
    if (variant == 1) {
        state->animated_a[0] = 0x10a;
        state->animated_a[1] = 0x10b;
        state->animated_a[2] = 0x10c;
        state->animated_b[0] = 0x11a;
        state->animated_b[1] = 0x11b;
        state->animated_b[2] = 0x11c;
        Map_UpdateCurrentTileBlock();
    }
    Runtime_BumpFree(buffer);
}
