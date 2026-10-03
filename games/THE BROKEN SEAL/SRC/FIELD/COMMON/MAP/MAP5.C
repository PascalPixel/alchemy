#include "PROJECT.H"
#include "MAP.H"
#include "RAM_BUFFER.H"
#include "TYPES.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"
#include "SYSTEM.H"
#include "DMA.H"
#include "IWRAM_CALL.H"
#include "MAP_SCROLL.H"
#include "HEAP_STATE.H"

extern u8 WorldMap_TerrainBehaviorTable[];



extern const u8 TileMap_DrawRows[];

extern u8 TileMap_DrawRowsCodeSize[];
extern u8 gDecodeBuffer[];
void *Runtime_AllocateHeapBlock(s32 kind, s32 size);


struct ScanlineRow {
    s32 x;
    s32 y;
    s32 zero1;
    s32 zero2;
    s32 unknown;
};


typedef void (*TransformFn)(const s32 *source, s32 *destination);

/* The routine is the last argument, so its address is loaded before the
   vectors, as at Iwram_Call2's call sites. */
static __inline__ void Transform(const s32 *source, s32 *destination, TransformFn routine)
{
    /* FAKEMATCH: a direct call holds the output vector in r9 instead of rematerializing sp+16. */
    routine(source, destination);
}

s32 CheckMapPositionCellOccupied(struct WorldPosition *position)
{
    s32 x;
    s32 y;
    s32 tile_x;
    s32 tile_y;
    struct MapState *work;
    u8 *cell;

    x = position->x / 65536;
    y = (position->y - *(s32 *)((u8 *)position + 4)) / 65536;
    work = gMapWork[0];
    if (work == NULL)
        return 0;
    cell = (u8 *)work->layers[2].cells;
    tile_x = x / 16;
    tile_y = y / 16;
    cell += (tile_x + tile_y * 128) * 4;
    return (cell[2] != 0xff) - 1;
}

s32 GetWorldMapCollision(struct WorldPosition *position)
{
    s32 x_step = position->x >> 17;
    s32 y = position->y;
    s32 y_step = y >> 17;
    u32 cell;
    u32 pixel_offset;
    u8 tile;
    u8 packed_pixels;
    u32 result;

    /* 1行64バイトで衝突判定タイル64個を表す。 */
    cell = (((u32)(y_step / 8) & 63) << 6) +
        ((u32)(x_step / 8) & 63);
    /* 4×4ドット・4bppの衝突判定タイルは8バイト。 */
    pixel_offset = (((u32)(y_step / 2) & 3) << 1) +
        ((u32)(x_step / 4) & 1);

    tile = *(u8 *)(0x06005000 + cell);
    packed_pixels = *(Ram_MapCollision + 0x800 + ((u32)tile << 3) + pixel_offset);
    if (packed_pixels != 0) {
        if ((u32)x_step & 2)
            result = packed_pixels >> 4;
        else
            result = packed_pixels & 15;
        if (result != 0)
            return result;
    }

    /* 前面が空なら背面マップを調べる。 */
    tile = *(u8 *)(0x06004000 + cell);
    packed_pixels = *(Ram_MapCollision + ((u32)tile << 3) + pixel_offset);
    if (packed_pixels != 0) {
        if ((u32)x_step & 2)
            result = packed_pixels >> 4;
        else
            result = packed_pixels & 15;
        if (result != 0)
            return result;
    }

    return 7;
}

s32 CheckWorldMapCollisionRange(s32 unused, struct WorldPosition *position)
{
    if ((u32)(GetWorldMapCollision(position)- 5) <= 7U) {
        return 0;
    }
    return -1;
}

u8 GetWorldMapTerrainBehavior(struct WorldPosition *position, s32 *terrain_kind)
{
    s32 selector = GetWorldMapCollision(position);
    s32 x = position->x;
    s32 y;
    s32 flag = 0;
    u32 tile_x;
    u32 index;
    u32 *tile;

    if (x < 0)
        x += 0x1fffff;
    tile_x = (x >> 21) & 31;
    y = position->y;
    if (y < 0)
        y += 0x1fffff;

    index = tile_x + (((y >> 21) & 31) << 5);
    tile = gMapBlocks + index;
    if (((u8 *)tile)[3] & 0x80)
        flag = 0x10;

    *terrain_kind = (*tile << 1) >> 25;
    if (*terrain_kind == 21)
        flag = 0x20;

    return WorldMap_TerrainBehaviorTable[flag + selector];
}

void Runtime_SetWorkTripleIfNonNegative(s32 value0, s32 value1, s32 value2)
{
    struct MapScrollWork *work;

    work = gMapWork[0];
    if (value0 >= 0) {
        work->shake_x = value0;
    }
    if (value1 >= 0) {
        work->shake_y = value1;
    }
    if (value2 >= 0) {
        work->shake_decay = value2;
    }
}

void Map_WaitWorkValuesBelow256(void)
{
    struct MapScrollWork *work;
    s32 cnt;

    work = gMapWork[0];
    cnt = 0;
    if (work->shake_x > 255 || work->shake_y > 255) {
        goto body;
body:
        WaitFrames(1);
        ++cnt;
        if (cnt >= 300) {
            goto done;
        }
        if (work->shake_x > 255) {
            goto body;
        }
        if (work->shake_y > 255) {
            goto body;
        }
    }
done:
    work->shake_decay = 0;
}

/* Copies the ARM decoder TileMap_DrawRows to a heap block and runs the
   decoder gWorkSlot holds on a and b. */
void Resource_RunCopiedDecoder(s32 a, s32 b)
{
    u8 *base;
    u32 size;
    void *code;

    base = gDecodeBuffer;
    /* FAKEMATCH: the do-whiles order the size load and the call. */
    do { size = (u32)TileMap_DrawRowsCodeSize; } while (0);
    code = Runtime_AllocateHeapBlock(49, size);
    Dma_Set((const void *)TileMap_DrawRows, code, 0x84000000 | (size >> 2), (volatile u32 *)0x040000d4);
    do { ((void (*)(s32, s32, void *, void *))((union HeapState *)gWorkSlot)->slots[49])(a, b, gBgTileBuffer + 0x4000, base + 0x1000); } while (0);
    Runtime_ReleaseHeapBlock(49);
}

/* field/common/map/build_scanline_table.c */

/* Fill the world map's 160 scanline rows from the camera's view of the
   ground plane at POSITION: a line that meets the plane gets a scale and a
   signed ground distance, any other line zeros. */
void WorldMap_BuildScanlineTable(s32 depth, s32 *position, struct ScanlineRow *row)
{
    s32 ground[3];
    s32 viewed[3];
    s32 horizon;
    s32 focus;
    s32 *view;
    register s32 line;
    s32 distance;
    s32 difference;
    s32 scale;
    s32 x;
    s32 y;
    s32 diagonal;
    s32 vertical;
    s32 zero;
    s32 horizontal;

    zero = 0;
    ground[0] = position[0];
    ground[1] = zero;
    ground[2] = position[2];
    Transform(ground, viewed, Iwram_TransformVector);
    view = viewed;
    horizon = view[1] - Iwram_MulQ16(view[2], depth);
    focus = -gProjection.focal;
    for (line = 0; line < 160; line++) {
        distance = Iwram_RatioMulQ14(focus, (gProjection.center_y - line) << 16);
        difference = distance - depth;
        if (difference == 0)
            difference = 1;
        scale = Iwram_RatioMulQ14(difference, horizon);
        if (scale < 0) {
            diagonal = Iwram_MulQ16(-scale, 0x8000);
            row->x = Iwram_RatioMulQ14(gProjection.focal, diagonal);
            diagonal = Iwram_MulQ16(scale, distance);
            horizontal = (view[2] - scale) >> 4;
            vertical = (diagonal - view[1]) >> 4;
            x = Iwram_MulQ16(horizontal, horizontal);
            y = Iwram_MulQ16(vertical, vertical);
            y = Iwram_Sqrt(x + y) << 12;
            if (vertical < 0)
                y = -y;
            row->y = Iwram_MulQ16(y, 0x8000);
        } else {
            row->x = 0;
            row->y = 0;
        }
        row->zero1 = 0;
        row->zero2 = 0;
        row++;
    }
}
