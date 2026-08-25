#include "types.h"

struct TileMaskPair {
    u32 left;
    u32 right;
};

extern const struct TileMaskPair Data_080af23c[];
void Func_0800730c(void *destination, s32 size, u32 value);

void Func_080a8c2c(void)
{
    u32 *tiles = (u32 *)0x06005000;
    s32 group;
    s32 block;

    for (group = 0; group < 2; group++) {
        for (block = 0; block < 6; block++) {
            u32 *tile = tiles + group * 0x60 + block * 0x10;
            s32 column;

            Func_0800730c(tile, 64, 0x44444444);
            for (column = 1; column <= 7; column++) {
                s32 mask_index = block;

                if (group == 1 && column <= 1) {
                    continue;
                }
                if (group == 0 && block > column - 2) {
                    mask_index = column - 2;
                    if (mask_index < 0) {
                        mask_index = 0;
                    }
                }
                tile[column] ^= Data_080af23c[mask_index].left;
                tile[column + 8] ^= Data_080af23c[mask_index].right;
            }
        }
    }
}
