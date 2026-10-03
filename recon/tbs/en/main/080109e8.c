#include "RESOURCE.H"
#include "RUNTIME_MEM.H"
/* 2026-10-03: the tile-animation declaration now comes from its actual
   const-u16 script API in MAP.H. Fresh baseline compiled all six editions;
   the grouped owner-cleanup comparison is pending. Prior score notes below
   describe the measured source before this declaration-only closure. */
#include "FIXED_POINT_POSITION.H"
#include "BATTLE_PRESENTATION.H"
#include "MAP_SCROLL.H"
#include "PROJECT.H"
/* 2026-09-30 (Mercury): EXACT, 864 of 864 bytes with approved agscc with the game build flags and four
   tagged loop-note FAKEMATCHes. It precedes
   FIELD/COMMON/MAP/SET_WINDOW_CELL_TILE, so its module is Mars's to choose;
   compile it under #if defined(TBS_EDITION_EN) until the other editions
   adopt theirs. */
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
#include "TYPES.H"
#include "DMA.H"

/* Sets up the tilted-plane map view: clears the scene work, loads the map
   graphics and animation, programs the two affine backgrounds, builds the
   camera and projects the plane once, then tilts the camera down to its
   resting pitch and starts the per-frame callbacks. */

extern char ResourceId_PerspectiveDataA;
extern char ResourceId_DefaultMapCells;
extern char ResourceId_DefaultMapAnimation;
extern char ResourceId_DefaultMetatileAttributes;
void Transform_UpdateVertices(void);
extern u8 Transform_UpdateVerticesSize;

extern u32 Data_03001f60;
extern u32 Data_03001af4;
extern u32 gFrameCount;
extern void *gWorkSlot[];

void Blend_SetDarkenTarget0(s32);
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

static __inline__ void Transform(struct FixedPointPosition *vector,
                                struct BattleCamera *camera,
                                s32 (*routine)(struct FixedPointPosition *,
                                                struct BattleCamera *))
{
    routine(vector, camera);
}

s32 Map_InitializePerspectiveScene(void)
{
    struct PerspectiveWork *work;
    struct BattleCamera *camera;
    void *tiles;
    u8 *lines;
    s32 *position;
    s32 *distance;
    u16 *yaw;
    u16 *pitch;
    u16 *turn;
    volatile u32 fill;
    struct FixedPointPosition vector;
    s32 far_plane;
    u32 size;
    s32 i;

    *(volatile u16 *)0x04000000 &= 0xc1ff;
    Blend_SetDarkenTarget0(0);
    work = (struct PerspectiveWork *)Runtime_AllocateHeapBlock(8, sizeof(struct PerspectiveWork));
    fill = 0;
    Dma_Set(&fill, work, 0x85000000 | (sizeof(struct PerspectiveWork) / 4), (volatile u32 *)0x040000d4);
    work->view_x = 0;
    work->view_y = 0;
    work->scale_x = 0x200000;
    work->scale_y = 0x400000;
    work->limit_x = 0x1fe00000;
    work->limit_y = 0x1fe00000;
    work->unknown_010 = 0;
    work->tiles = Resource_GetTableEntry((s32)&ResourceId_PerspectiveDataA);
    Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_DefaultMapAnimation), (void *)0x0202d000);
    MapAnimation_StartChannels((const u16 *)0x0202d000);
    Io_Set16(0x3f9e, (u16 *)0x04000050);
    Io_Set16(0x1010, (u16 *)0x04000052);
    *(u16 *)0x04000054 = 0;
    Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_DefaultMapCells), (void *)0x02010000);
    Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_DefaultMetatileAttributes), (void *)0x0202c000);
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

    camera = Runtime_AllocateBlock(12, sizeof(struct BattleCamera));
    tiles = (void *)Runtime_AllocateHeapBlock(7, 0x3484);
    position = camera->pos;
    lines = (u8 *)tiles + 0xc80;
    far_plane = 0x1fe0000;
    work->far_plane = far_plane;
    distance = &work->distance;
    *distance = far_plane;
    work->zoom = 0x10000;
    turn = &work->turn;
    camera->unknown_18 = 0;
    camera->follow_pos = 0;
    *turn = 0;
    gProjection.center_x = 120;
    gProjection.center_y = 96;
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
              (s32 (*)(struct FixedPointPosition *, struct BattleCamera *))0x03000250);
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
              (s32 (*)(struct FixedPointPosition *, struct BattleCamera *))0x03000250);
    *(volatile u16 *)0x0400004c = 0;
    /* FAKEMATCH: loop notes keep the display writes in source order */
    do {
        Io_Put16((u16 *)0x04000000, 0x42);
    } while (0);
    gBgScroll[1].x = 0;
    gBgScroll[1].y = 0;
    gBgScroll[2].x = 0;
    gBgScroll[2].y = 0;
    gBgScroll[3].x = 0;
    gBgScroll[3].y = 0;
    work->window_top = 0;
    work->window_bottom = 159;
    Scheduler_AddOrUpdateCallback((s32)WorldMap_UpdateView, 0xc85);
    Scheduler_AddOrUpdateCallback((s32)MapAnimation_ApplyAffineFrame, 0x480);
    for (i = 255; i >= 0; i--)
        work->tile_ids[i] = i;
}
