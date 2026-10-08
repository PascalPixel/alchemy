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
#include "MAPFRAME.H"
#include "HEAP_STATE.H"
#include "IO_REG.H"



extern u8 gDecodeBuffer[];

typedef void (*TransformFn)(const s32 *source, s32 *destination);

static __inline__ void Transform(const s32 *source, s32 *destination, TransformFn routine)
{
    /* FAKEMATCH: needed to match: a direct call holds the output vector in r9 instead of rematerializing sp+16. */
    routine(source, destination);
}

s32 CheckMapPositionCellOccupied(struct WorldPosition *position)
{
    s32 x;
    s32 y;
    s32 tile_x;
    s32 tile_y;
    struct MapState *work;
    struct MapCell *cell;

    x = position->x / 65536;
    y = (position->y - position->height) / 65536;
    work = gMapWork[0];
    if (work == NULL)
        return 0;
    cell = work->layers[2].cells;
    tile_x = x / 16;
    tile_y = y / 16;
    cell += tile_x + tile_y * 128;
    return (cell->collision_code != 0xff) - 1;
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

    cell = (((u32)(y_step / 8) & 63) << 6) +
        ((u32)(x_step / 8) & 63);
    pixel_offset = (((u32)(y_step / 2) & 3) << 1) +
        ((u32)(x_step / 4) & 1);

    tile = BG_SCREEN_BLOCK(10)[cell];
    packed_pixels = *(Ram_MapCollision + 0x800 + ((u32)tile << 3) + pixel_offset);
    if (packed_pixels != 0) {
        if ((u32)x_step & 2)
            result = packed_pixels >> 4;
        else
            result = packed_pixels & 15;
        if (result != 0)
            return result;
    }

    tile = BG_SCREEN_BLOCK(8)[cell];
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
    if ((u32)(GetWorldMapCollision(position) - 5) <= 7U) {
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

void Resource_RunCopiedDecoder(s32 a, s32 b)
{
    u8 *base;
    u32 size;
    void *code;

    base = gDecodeBuffer;
    /* FAKEMATCH: needed to match: the do-whiles order the size load and the call. */
    do { size = (u32)TileMap_DrawRowsCodeSize; } while (0);
    code = Runtime_AllocateHeapBlock(49, size);
    Dma_Set((const void *)TileMap_DrawRows, code, DMA_ENABLE32 | DMA_32BIT | size >> 2, REG_DMA3);
    do { ((void (*)(s32, s32, void *, void *))gWorkSlot[49])(a, b, gBgTileBuffer + 0x4000, base + 0x1000); } while (0);
    Runtime_ReleaseHeapBlock(49);
}

void WorldMap_BuildScanlineTable(s32 depth, s32 *position, struct MapScanline *row)
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
            row->scale = Iwram_RatioMulQ14(gProjection.focal, diagonal);
            diagonal = Iwram_MulQ16(scale, distance);
            horizontal = (view[2] - scale) >> 4;
            vertical = (diagonal - view[1]) >> 4;
            x = Iwram_MulQ16(horizontal, horizontal);
            y = Iwram_MulQ16(vertical, vertical);
            y = Iwram_Sqrt(x + y) << 12;
            if (vertical < 0)
                y = -y;
            row->depth = Iwram_MulQ16(y, 0x8000);
        } else {
            row->scale = 0;
            row->depth = 0;
        }
        row->offset_x = 0;
        row->offset_y = 0;
        row++;
    }
}
