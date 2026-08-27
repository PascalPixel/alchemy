#include "types.h"

extern u8 *Data_03001e70;

struct TilePos {
    s32 x;
    s32 y;
};

void Func_08010424(s32 srcX, s32 srcY, u32 dstX, u32 dstY, u32 height, u32 width)
{
    u32 *src = (u32 *)(0x02010000 + ((srcY << 7) + srcX) * 4);
    u32 *dst = (u32 *)(0x02010000 + ((dstY << 7) + dstX) * 4);
    u8 *actors = Data_03001e70 + 0x104;
    struct TilePos tile[3];
    s32 i;
    u32 y;
    u32 x;

    for (i = 2; i >= 0; i--) {
        u8 *actor = actors + i * 48;
        tile[i].x = *(s32 *)actor >> 20;
        tile[i].y = *(s32 *)(actor + 4) >> 20;
    }

    if (dstY >= dstY + height)
        return;

    for (y = dstY; y < dstY + height; y++) {
        if (dstX >= dstX + width)
            continue;

        for (x = dstX; x < dstX + width; x++) {
            u32 cell = *src++ & 0xFFF;

            *dst = (*dst & 0xFFFFF000) | cell;
            dst++;

            for (i = 0; i < 3; i++) {
                if ((s32)x >= tile[i].x && (s32)x < tile[i].x + 16 &&
                    (s32)y >= tile[i].y && (s32)y < tile[i].y + 12) {
                    u32 tileOffset = ((y & 15) << 5) + (x & 15);
                    u8 *overlayDst = (u8 *)(0x06002800 + tileOffset * 4 + i * 2048);
                    u8 *overlaySrc = (u8 *)(0x02020000 + cell * 8);

                    *(u32 *)overlayDst = *(u32 *)overlaySrc;
                    *(u32 *)(overlayDst + 0x40) = *(u32 *)(overlaySrc + 0x40);
                    break;
                }
            }
        }
    }
}
