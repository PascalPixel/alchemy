#include "types.h"

extern u8 *Data_03001e70;

s32 Func_080108e4(s32 layer, s32 y, s32 x, s32 tile, s32 flag);

void Func_08010d48(s32 pixelX, s32 tileValue, s32 pixelY, s32 pixelMapX)
{
    u8 *state;
    s32 cameraY;
    s32 cameraX;
    s32 *position;
    s32 mapX;
    s32 mapY;
    s32 tile;
    s32 difference;
    u32 index;

    state = Data_03001e70;
    position = *(s32 **)state;
    cameraY = 0;
    cameraX = 0;
    if (position != 0) {
        cameraY = position[0];
        cameraX = position[2];
    }

    tileValue >>= 4;
    tile = tileValue * 16 + (pixelX >> 4);
    mapX = pixelMapX >> 3;
    mapY = pixelY >> 3;
    cameraX >>= 24;
    cameraY >>= 24;

    index = (((mapX / 2) & 15) << 4) + ((mapY / 2) & 15);
    *(u16 *)(state + 0x138 + index * 2) = tile;

    difference = cameraY - mapY;
    if (difference >= 0) {
        if (difference > 1)
            return;
    } else if (mapY - cameraY > 1) {
        return;
    }

    difference = cameraX - mapX;
    if (difference >= 0) {
        if (difference > 1)
            return;
    } else if (mapX - cameraX > 1) {
        return;
    }

    mapY /= 2;
    mapX /= 2;
    Func_080108e4(0, mapY, mapX, tile, 1);
    Func_080108e4(1, mapY, mapX, tile + 0x140, 1);
}
