#include "EDITION.H"
#include "MAPCOPY.H"

#if defined(TLA_EDITION_JA) || defined(TLA_EDITION_EN) || \
    defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) || \
    defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)

void Map_CopyCellsRect(
    s32 src_x, s32 src_y, s32 width, s32 height, s32 dst_x, s32 dst_y)
{
    Map_CopyMetatileIndicesRect(src_x, src_y, dst_x, dst_y, width, height);
    Map_CopyCellAttributeRect(src_x, src_y, width, height, dst_x, dst_y);
}
#else
#include "MAP_SCROLL.H"
#include "RAM_BUFFER.H"

extern u8 gMapCellBuffer[];

#define MAP_CELLS ((u32 *)gMapCellBuffer)
#define MAP_CELL_TILE_MASK 0x00000fff
#define MAP_CELL_ATTRIBUTE_MASK 0xfffff000

struct TilePos { s32 x, y; };

void Map_CopyMetatileCellsRect(s32 src_x, s32 src_y,
    s32 width, s32 height, s32 dst_x, s32 dst_y)
{
    /* FAKEMATCH: the consumed r3 binding preserves register allocation and order without changing instruction count or the 36-byte frame. */
    s32 y = dst_y;
    u32 *src = (u32 *)Ram_MapCellBuffer + (src_y * 128 + src_x);
    u32 *dst = &((u32 *)Ram_MapCellBuffer)[y * 128 + dst_x];
    struct MapLayerScroll *layer = ((struct MapScrollWork *)*gMapWork)->layers;
    struct TilePos tile[3];
    s32 i;
    struct TilePos *pos = tile;
    register s32 x;

    i = 2;
    while (i >= 0) {
        register s32 tmp5;
        tmp5 = layer->x >> 20;
        pos->x = tmp5;
        tmp5 = (*layer).y;
        pos->y = tmp5 >> 20;
        pos++;
        layer++;
        i--;
    }
    if (y < dst_y + height) {
        do {
            for (x = dst_x; x < dst_x + width; x++) {
                u32 whole = *src++;
                u32 cell;
                register u32 offset;
                u32 tmp3;
                u32 *dest;
                u32 n;
                register u32 *tiles;
                u32 tmp6;
                u32 tmp12;
                u32 tmp13;
                dst[0] = whole;
                cell = whole & 0xfff;
                tmp12 = x & 15;
                tmp6 = tmp12;
                dst += 1;
                tmp3 = y & 15;
                tmp13 = tmp3 << 5;
                pos = tile;
                tmp12 = tmp13;
                n = tmp12;
                n += tmp6;
                tmp13 = n * 4;
                i = 0;
                tmp6 = tmp13;
                offset = tmp6;
                for (; i < 3; i += 1) {
                    if (pos->x <= x && 16 + (*pos).x > x && pos->y <= y && pos->y + 12 > y) {
                        u32 *tmp;
                        u32 tmp2;
                        u32 tmp4;
                        u32 *tmp7;
                        u32 tmp10;
                        u32 tmp14;
                        u32 tmp16;
                        /* FAKEMATCH: the consumed r3 binding preserves register allocation and order without adding instructions or changing the frame. */
                        register u32 bytes __asm__ ("r3") = cell * 8;
                        u32 tmp8;
                        register u32 tmp17;
                        tmp = (u32 *)Ram_MapBlocks;
                        tmp8 = 0x06002800 + offset;
                        tiles = tmp;
                        tmp2 = tmp8;
                        tmp = (u32 *)tmp2;
                        dest = tmp;
                        tiles = (u32 *)((u8 *)tiles + bytes);
                        tmp16 = *tiles;
                        tmp14 = tmp16;
                        tmp17 = 0x06002840 + offset;
                        tmp10 = tmp14;
                        *dest = tmp10;
                        tmp7 = (u32 *)(Ram_MapBlocks + 4);
                        tiles = tmp7;
                        tiles = (u32 *)((u8 *)tiles + bytes);
                        tmp8 = tmp17;
                        tmp14 = tmp8;
                        tmp17 = tiles[0];
                        tmp16 = tmp17;
                        tmp10 = tmp16;
                        tmp4 = tmp10;
                        tmp2 = tmp4;
                        tmp4 = tmp14;
                        tmp7 = (u32 *)tmp4;
                        dest = tmp7;
                        *dest = tmp2;
                        break;
                    }
                    n = 0x800;
                    offset += n;
                    pos++;
                }
            }
            src = src + (128 - width);
            dst = 128 - width + dst;
            y++;
        } while (y < dst_y + height);
    }
}

void Map_CopyCellAttributeRect(
    s32 src_x, s32 src_y, s32 width, s32 height, s32 dst_x, s32 dst_y)
{
    u32 *dst = MAP_CELLS;
    u32 *src = dst;
    u16 row;
    u16 col;

    dst += (dst_y << 7) + dst_x;
    src += (src_y << 7) + src_x;

    for (row = 0; row < height; row++) {
        u32 *dst_cell = dst + (row << 7);
        u32 *src_cell = src + (row << 7);

        for (col = 0; col < width; col++) {
            *dst_cell = (*dst_cell & MAP_CELL_TILE_MASK) |
                (*src_cell++ & MAP_CELL_ATTRIBUTE_MASK);
            dst_cell++;
        }
    }
}

void Map_CopyCellIndexRect(s32 src_x, s32 src_y,
    s32 width, s32 height, s32 dst_x, s32 dst_y)
{
    /* FAKEMATCH: word/index temporaries preserve register allocation; the plain body allocates differently and uses a 40-byte frame. */
    s32 y;
    u32 *src = (u32 *)Ram_MapCellBuffer + (src_y * 128 + src_x);
    u32 *dst = &((u32 *)Ram_MapCellBuffer)[dst_y * 128 + dst_x];
    struct MapLayerScroll *layer = ((struct MapScrollWork *)*gMapWork)->layers;
    struct TilePos tile[3];
    s32 i;
    struct TilePos *pos = tile;
    register s32 x;

    i = 2;
    while (i >= 0) {
        register s32 tmp5;
        tmp5 = layer->x >> 20;
        pos->x = tmp5;
        tmp5 = (*layer).y;
        pos->y = tmp5 >> 20;
        pos++;
        layer++;
        i--;
    }
    y = dst_y;
    if (y < dst_y + height) {
        do {
            for (x = dst_x; x < dst_x + width; x++) {
                u32 whole = *src++;
                u32 cell;
                register u32 offset;
                u32 tmp3;
                volatile u32 *dest;
                u32 n;
                register u32 *tiles;
                u32 tmp6;
                u32 tmp12;
                u32 tmp13;
                cell = whole & MAP_CELL_TILE_MASK;
                dst[0] = (dst[0] & MAP_CELL_ATTRIBUTE_MASK) | cell;
                tmp12 = x & 15;
                tmp6 = tmp12;
                dst += 1;
                tmp3 = y & 15;
                tmp13 = tmp3 << 5;
                pos = tile;
                i = 0;
                tmp12 = tmp13;
                n = tmp12;
                n += tmp6;
                tmp13 = n * 4;
                tmp6 = tmp13;
                offset = tmp6;
                for (; i < 3; i += 1) {
                    if (pos->x <= x && 16 + (*pos).x > x && pos->y <= y && pos->y + 12 > y) {
                        u32 *tmp;
                        u32 tmp2;
                        u32 tmp4;
                        u32 *tmp7;
                        u32 tmp10;
                        u32 tmp14;
                        u32 tmp18;
                        u32 tmp21;
                        /* FAKEMATCH: the scaled cell stays in r3 for both tile words; registers and order only. */
                        register u32 bytes __asm__ ("r3") = cell * 8;
                        u32 tmp16;
                        {
                            tmp2 = 0x06002800;
                            /* FAKEMATCH: the consumed-base barrier preserves two register choices and one load order without changing instruction count or frame. */
                            __asm__ __volatile__ ("" : : "l" (tmp2));
                            tmp = (u32 *)Ram_MapBlocks;
                            tiles = tmp;
                            tmp16 = offset + tmp2;
                        }
                        tmp21 = tmp16;
                        tmp18 = tmp21;
                        tmp2 = tmp18;
                        tmp = (u32 *)tmp2;
                        dest = tmp;
                        tiles = (u32 *)((u8 *)tiles + bytes);
                        tmp14 = *tiles;
                        tmp10 = tmp14;
                        *dest = tmp10;
                        tmp7 = (u32 *)(Ram_MapBlocks + 4);
                        tiles = tmp7;
                        tiles = (u32 *)((u8 *)tiles + bytes);
                        tmp16 = 0x06002840 + offset;
                        tmp18 = tmp16;
                        tmp14 = tmp18;
                        tmp21 = *tiles;
                        tmp10 = tmp21;
                        tmp4 = tmp10;
                        tmp2 = tmp4;
                        tmp4 = tmp14;
                        tmp7 = (u32 *)tmp4;
                        dest = tmp7;
                        *dest = tmp2;
                        break;
                    }
                    n = 0x800;
                    offset += n;
                    pos++;
                }
            }
            src = src + (128 - width);
            dst = 128 - width + dst;
            y++;
        } while (y < dst_y + height);
    }
}
#endif
