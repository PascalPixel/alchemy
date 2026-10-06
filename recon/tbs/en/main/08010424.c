#include "MAP_SCROLL.H"
#include "RAM_BUFFER.H"
struct TilePos { s32 x, y; };

void Map_CopyMetatileIndicesRect(s32 src_x, s32 src_y,
    u32 dst_x, u32 dst_y, u32 width, u32 height)
{
    /* FAKEMATCH: measured ordinary rewrites retain the reference frame and
       instruction stream; temporary sharing steers register allocation
       and scheduling (score 2683 to 495, no added/deleted instructions). */
    u32 *src = (u32 *)Ram_MapCellBuffer + (src_y * 128 + src_x);
    u32 *dst = (u32 *)Ram_MapCellBuffer + (dst_y * 128 + dst_x);
    register struct MapLayerScroll *layer = ((struct MapScrollWork *)gMapWork[0])->layers;
    struct TilePos tile[3];
    s32 i;
    struct TilePos *pos = tile;
    u32 y;
    register u32 x;

    i = 2;
    if (i >= 0) {
        while (1) {
            s32 tmp5;
            s32 tmp8;
            tmp8 = layer->x;
            tmp5 = tmp8 >> 20;
            pos->x = tmp5;
            tmp8 = layer->y;
            tmp5 = tmp8;
            pos->y = tmp5 >> 20;
            ++pos;
            layer++;
            i--;
            if (0 > i)
                break;
        }
    }
    y = dst_y;
    if (y < dst_y + height) {
        do {
            u32 tmp20;
            x = dst_x;
            while (x < dst_x + width) {
                /* FAKEMATCH: the cell value remains live in r5 while layers are tested. */
                register u32 cell __asm__ ("r5") = 0xfff & *src++;
                volatile u32 *dest;
                u32 offset;
                u32 tmp3;
                u32 n;
                register u32 *tiles;
                u32 tmp6;
                u32 tmp9;
                u32 tmp12;
                u32 tmp11;
                u32 tmp13;
                u32 tmp15;
                tmp9 = *dst;
                tmp3 = tmp9 & -0x1000;
                tmp12 = x & 15;
                (s32)(dst[0] = cell | tmp3);
                tmp6 = tmp12;
                dst++;
                tmp15 = y & 15;
                tmp3 = tmp15;
                pos = tile;
                tmp13 = tmp3 << 5;
                tmp12 = tmp13;
                n = tmp12;
                n += tmp6;
                tmp13 = 4 * n;
                i = 0;
                tmp11 = tmp13;
                tmp6 = tmp11;
                offset = tmp6;
                for (; i < 3; i++) {
                    if (pos->x <= (s32)x && 16 + pos->x > (s32)x && pos->y <= (s32)y && pos->y + 12 > (s32)y) {
                        u32 *tmp;
                        u32 tmp2;
                        u32 tmp4;
                        u32 *tmp7;
                        u32 tmp10;
                        u32 tmp14;
                        u32 tmp18;
                        u32 tmp21;
                        /* FAKEMATCH: consumed offset in r3 changes only registers/order, preserving all instructions. */
                        register u32 bytes __asm__ ("r3") = cell * 8;
                        u32 tmp16;
                        {
                            tmp2 = 0x06002800;
                            /* FAKEMATCH: removing this consumed-base input barrier scores70 (two register choices and one reordered load), with no instruction or frame change. */
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
                        (s32)(*dest = tmp2);
                        break;
                    }
                    n = 0x800;
                    offset += n;
                    pos++;
                }
                x++;
            }
            src += (s32)(128 - width);
            tmp20 = 128 - width;
            dst = dst + tmp20;
            y++;
        } while (y < dst_y + height);
    }
}
