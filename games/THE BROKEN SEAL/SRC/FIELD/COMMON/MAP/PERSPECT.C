#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "MEM.H"
#include "RESOURCE.H"
#include "RESOURCE_IDS.H"
#include "FIXED_POINT_POSITION.H"
#include "MAP_SCROLL.H"
#include "MAPFRAME.H"
#include "HEAP_STATE.H"
#include "BTLDISP.H"
#include "PROJECT.H"
#include "GLOBAL_CELLS.H"
#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "DMA.H"
#include "MAP.H"
#include "RAM_BUFFER.H"
#include "IO_REG.H"
#include "IWRAM_CALL.H"
#include "FIXED_MATH.H"
#include "TRANSFORM.H"

#ifndef MODERN // modern GCC keeps the headers' volatile declaration
extern u32 gFrameCount;
#endif

static __inline__ void Io_Set16(s32 value, volatile u16 *reg)
{
    *reg = value;
}

static __inline__ void Io_Put16(volatile u16 *reg, s32 value)
{
    *reg = value;
}

typedef s32 (*RatioFn)(s32, s32);
typedef s32 (*PlaneFn)(void *camera, s32 *position, void *lines, void *out);

#define ABS(v) ((v) < 0 ? -(v) : (v))


s32 Resource_DecodeByteLz(const void *source, void *destination);

struct WorldCell {
    u32 tile : 24;
    u32 kind : 7;
    u32 flag : 1;
};

#define BG_PALETTE ((s16 *)BG_PLTT)
#define WORLD_CELLS ((struct WorldCell *)Ram_MapBlocks)

s32 Map_WriteLayerCellTile(s32 layer, s32 x, s32 y, s32 tile, s32 update)
{
    struct PerspectiveWork *work = gMapWork[0];
    u32 *graphics = work->tiles;
    u16 *shown;
    u16 *buffer;
    u16 *source;
    u8 *blocks;
    u16 *screen;
    u32 i;
    u32 j;
    u32 id;

    x &= 1;
    y &= 1;
    shown = &work->quarter_tiles[(layer * 2 + y) * 2 + x];
    if (update == 0 && tile == *shown)
        return 0;
    *shown = tile;
    buffer = Runtime_AllocateHeapBlock(14, 0x400);
    Resource_DecodeByteLz((u8 *)graphics + graphics[tile], buffer);

    source = buffer;
    blocks = Ram_MapBlocks + (((layer * 2 + y) * 32 + x) << 6);
    for (i = 0; i < 16; i++) {
        Dma_Set(source, blocks, DMA_ENABLE32 | DMA_32BIT | 0x10, REG_DMA3);
        source += 32;
        blocks += 128;
    }

    if (update != 0) {
        screen = (u16 *)(BG_SCREEN_BLOCK(8) + ((((layer * 2 + y) << 6) + x) << 5));
        source = buffer;
        for (i = 0; i < 16; i++) {
            for (j = 0; j < 16; j++) {
                u16 *bottom;

                id = *source;
                screen[0] = *(u16 *)(Ram_MapCellBuffer + id * 4);
                bottom = (u16 *)(Ram_MapCellBuffer + 2);
                bottom = (u16 *)((u8 *)bottom + id * 4);
                screen[32] = *bottom;
                screen++;
                source += 2;
            }
            screen += 48;
        }
    }
    Runtime_ReleaseHeapBlock(14);
    return 1;
}

s32 Map_InitializePerspectiveScene(void)
{
    struct PerspectiveWork *work;
    struct BattleCamera *camera;
    struct MapFrameWork *tiles;
    struct MapAffinePair (*lines)[160];
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

    REG_DISPCNT &= ~(DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON);
    Blend_SetDarkenTarget0(0);
    work = Runtime_AllocateHeapBlock(8, sizeof(struct PerspectiveWork));
    fill = 0;
    Dma_Set(&fill, work, DMA_ENABLE32 | DMA_SRC_FIXED32 | DMA_32BIT | (sizeof(struct PerspectiveWork) / 4), REG_DMA3);
    work->view_x = 0;
    work->view_y = 0;
    work->scale_x = 0x200000;
    work->scale_y = 0x400000;
    work->limit_x = 0x1fe00000;
    work->limit_y = 0x1fe00000;
    work->script = NULL;
    work->tiles = Resource_GetTableEntry((s32)&ResourceId_PerspectiveDataA);
    Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_DefaultMapAnimation), Ram_MapCollision + 0x1000);
    MapAnimation_StartChannels((const u16 *)(Ram_MapCollision + 0x1000));
    Io_Set16(0x3f9e, &REG_BLDCNT);
    Io_Set16(0x1010, &REG_BLDALPHA);
    REG_BLDY = 0;
    Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_DefaultMapCells), Ram_MapCellBuffer);
    Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_DefaultMetatileAttributes), Ram_MapCollision);
    work->fade = 0x1f00;
    work->fade_step = 0x80;
    Io_Set16(0xa80a, &REG_BG3CNT);
    Io_Set16(0xaa0e, &REG_BG2CNT);
    Io_Set16(0x0501, &REG_BG1CNT);
    /* FAKEMATCH: needed to match: loop note keeps BG2PA's constant after its address */
    do {
        Io_Put16(&REG_BG2PA, 0x100);
        REG_BG2PB = 0;
        REG_BG2PC = 0;
    } while (0);
    /* FAKEMATCH: needed to match: loop notes keep BG2PD's constant late and r0 first below */
    do {
        Io_Put16(&REG_BG2PD, 0x100);
        REG_BG2X = 0;
        REG_BG2Y = 0;
        Io_Put16(&REG_BG3PA, 0x100);
        REG_BG3PB = 0;
        REG_BG3PC = 0;
        Io_Put16(&REG_BG3PD, 0x100);
        REG_BG3X = 0;
        REG_BG3Y = 0;
    } while (0);

    camera = Runtime_AllocateBlock(12, sizeof(struct BattleCamera));
    tiles = Runtime_AllocateHeapBlock(7, sizeof(struct MapFrameWork));
    position = camera->pos;
    lines = tiles->pages;
    far_plane = 0x1fe0000;
    work->far_plane = far_plane;
    distance = &work->distance;
    *distance = far_plane;
    work->zoom = 0x10000;
    turn = &work->zoom_transition_frames;
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
    /* FAKEMATCH: needed to match: Iwram_TransformVector's void call loads camera into r1 after the routine; the value-returning cast keeps r1 first. */
    ((s32 (*)(struct FixedPointPosition *, struct BattleCamera *))Iwram_TransformVector)(&vector, camera);
    Render_ResetTransformState();
    Graphics_PrepareTransferInIwramWork(camera->eye, position);
    /* FAKEMATCH: needed to match: loop note orders the transform call's r1, routine, r0 */
    do {
        size = (s32)&Transform_UpdateVerticesSize;
        Dma_Set((void *)Transform_UpdateVertices, Runtime_AllocateHeapBlock(46, size),
                DMA_ENABLE32 | DMA_32BIT | (size >> 2), REG_DMA3);
    } while (0);
    WorldMap_BuildScanlineTable(Iwram_RatioMulQ14(Trig_Cos(*pitch), Trig_Sin(*pitch)),
                  position, tiles->scanlines);
    gScanlineAngle = 0;
    gScanlinePitch = work->pitch;
    ((PlaneFn)gWorkSlot[46])(camera, position, tiles->scanlines,
                                 lines[gFrameCount & 1]);
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
    Iwram_TransformVector((s32 *)&vector, camera->eye);
    REG_MOSAIC = 0;
    /* FAKEMATCH: needed to match: loop notes keep the display writes in source order */
    do {
        Io_Put16(&REG_DISPCNT, 0x42);
    } while (0);
    gBgScroll[1].x = 0;
    gBgScroll[1].y = 0;
    gBgScroll[2].x = 0;
    gBgScroll[2].y = 0;
    gBgScroll[3].x = 0;
    gBgScroll[3].y = 0;
    work->window_top = 0;
    work->window_bottom = 159;
    Scheduler_AddOrUpdateCallback(WorldMap_UpdateView, 0xc85);
    Scheduler_AddOrUpdateCallback(MapAnimation_ApplyAffineFrame, 0x480);
    for (i = 255; i >= 0; i--)
        work->tile_ids[i] = i;
}

void Map_SetWindowCellTile(s32 x, s32 y, s32 px, s32 py)
{
    struct PerspectiveWork *window;
    s32 *position;
    s32 origin_x;
    s32 origin_y;
    s32 tile;

    window = gMapWork[0];
    origin_x = 0;
    origin_y = 0;
    position = window->origin;
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
    window->tile_ids[(((py / 2) & 15) << 4) + ((px / 2) & 15)] = tile;

    if (ABS(origin_x - px) <= 1 && ABS(origin_y - py) <= 1) {
        Map_WriteLayerCellTile(0, px / 2, py / 2, tile, 1);
        Map_WriteLayerCellTile(1, px / 2, py / 2, tile + 0x140, 1);
    }
}

void WorldMap_LoadGraphics(s32 x, s32 z)
{
    struct PerspectiveWork *state;
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
    buffer = MemAlloc(0x200);
    state = gMapWork[0];
    if (WORLD_CELLS[((x / 0x200000) & 31) + (((z / 0x200000) & 31) << 5)].kind == 21)
        variant = 1;
    resources = WorldMap_GraphicsResources[variant];
    state->resources = resources;
    value = BG_PALETTE[0];
    Resource_DecodeByteLz(Resource_GetTableEntry(resources[0]), buffer);
    *(s16 *)buffer = value;
    Dma_Set(buffer, BG_PALETTE, DMA_ENABLE32 | DMA_32BIT | 0x70, REG_DMA3);
    tiles = gBgTileBuffer;
    Resource_DecodeType01(Resource_GetTableEntry(resources[1]), tiles);
    Dma_Set(tiles, BG_CHAR_BLOCK(2), DMA_ENABLE32 | DMA_32BIT | 0x800, REG_DMA3);
    Resource_DecodeType01(Resource_GetTableEntry(resources[2]), Ram_BgTileBuffer + 0x2000);
    Dma_Set(Ram_BgTileBuffer + 0x2000, BG_CHAR_BLOCK(2) + 0x2000, DMA_ENABLE32 | DMA_32BIT | 0x800, REG_DMA3);
    Resource_DecodeType01(Resource_GetTableEntry(resources[3]), Ram_BgTileBuffer + 0x4000);
    Dma_Set(Ram_BgTileBuffer + 0x4000, BG_CHAR_BLOCK(3), DMA_ENABLE32 | DMA_32BIT | 0x800, REG_DMA3);
    Resource_DecodeType01(Resource_GetTableEntry(resources[4]), Ram_BgTileBuffer + 0x6000);
    Dma_Set(Ram_BgTileBuffer + 0x6000, BG_CHAR_BLOCK(3) + 0x2000, DMA_ENABLE32 | DMA_32BIT | 0x800, REG_DMA3);
    Resource_DecodeType01(Resource_GetTableEntry(resources[5]), gMapLayerData);
    fill = 0xf07ff07f;
    Dma_Set((const void *)&fill, BG_SCREEN_BLOCK(5), DMA_ENABLE32 | DMA_32BIT | DMA_SRC_FIXED32 | 0x180, REG_DMA3);
    cursor = (u32 *)BG_SCREEN_BLOCK(6);
    tile = 0x01a901a8;
    for (row = 0; row < 20; row++) {
        for (col = 0; col < 15; col++) {
            *cursor++ = tile;
            tile += 0x00020002;
        }
        cursor++;
    }
    if (variant == 1) {
        state->tile_ids[10] = 0x10a;
        state->tile_ids[11] = 0x10b;
        state->tile_ids[12] = 0x10c;
        state->tile_ids[26] = 0x11a;
        state->tile_ids[27] = 0x11b;
        state->tile_ids[28] = 0x11c;
        Map_UpdateCurrentTileBlock();
    }
    MemFree(buffer);
}

void MapAnimation_ApplyAffineFrame(void)
{
    /* FAKEMATCH: needed to match: the adjacent slot-cell walk avoids the eight bytes added by separate gMapWork loads. */
    u8 **pointers = &gMapAnimationPages;
    struct MapAffinePair (*lines)[160] = ((struct MapFrameWork *)*pointers++)->pages;
    s32 dispcnt = (s16)(REG_DISPCNT & 0xfff8);
    u32 *dst = (u32 *)&REG_BG2PA;
    struct PerspectiveWork *work = *(struct PerspectiveWork **)pointers++;
    volatile u16 *channel = REG_DMA0;
    u32 *src;
    u32 mode;
    u32 top;
    u32 control;

    channel[5] &= 0xc5ff;
    channel[5] &= 0x7fff;
    (void)channel[5];
    if (lines != NULL) {
        src = (u32 *)lines[gFrameCount & 1];
        *dst++ = *src++;
        *dst++ = *src++;
        *dst++ = *src++;
        *dst++ = *src++;
        *dst++ = *src++;
        *dst++ = *src++;
        *dst++ = *src++;
        *dst = *src++;
        Dma_Set(src, (void *)&REG_BG2PA, 0xa6600008, (volatile u32 *)channel);
    }
    work->shown_top = work->window_top;
    work->shown_bottom = work->window_bottom;
    top = work->shown_top;
    mode = 0;
    if (top < 200) {
        u16 bottom = work->shown_bottom;

        mode = (bottom != 0) << 1;
        if (top <= bottom) {
            mode = 0;
            if (top == 0)
                mode = 2;
        }
    }
    dispcnt |= mode;
    control = (u16)dispcnt;
    REG_DISPCNT = control;
    work->window_pending = 0;
}
