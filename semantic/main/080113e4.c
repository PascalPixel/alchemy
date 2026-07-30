#include "types.h"

extern u8 *Data_03001e70;

void Func_080108e4(u32 layer, s32 x, s32 y, s32 tile, s32 flag);

void Func_080113e4(void)
{
    u8 *state = Data_03001e70;
    s32 x = 0;
    s32 y = 0;
    u32 layer;
    s32 tileBase;

    if (*(s32 **)state != 0) {
        s32 *position = *(s32 **)state;

        x = position[0];
        y = position[2];
    }

    x = (x - 0x01000000) >> 25;
    y = (y - 0x01400000) >> 25;

    layer = 0;
    tileBase = 0;
    do {
        u32 row = 0;
        s32 tileY = y;

        do {
            u32 column = 0;

            do {
                s32 tileX = x + column;
                u32 index = ((tileY & 15) << 4) + (tileX & 15);
                s32 tile = *(u16 *)(state + 0x138 + index * 2);

                Func_080108e4(layer, tileX, tileY, tile + tileBase, 1);
                column++;
            } while (column <= 1);

            row++;
            tileY++;
        } while (row <= 1);

        layer++;
        tileBase += 0x140;
    } while (layer <= 1);
}
