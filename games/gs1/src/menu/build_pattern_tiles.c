#include "types.h"

struct TileMask {
    u32 word0;
    u32 word1;
};

typedef void (*FillProc)(void *dst, s32 size, u32 word);

static __inline__ u32 XorWord(u32 word, u32 mask)
{
    return word ^ mask;
}

extern const struct TileMask Data_080af23c[];

#define MaskTbl Data_080af23c
#define FillWords ((FillProc)0x03000168)

#define Menu_BuildPatternTiles Func_080a8c2c

void Menu_BuildPatternTiles(void)
{
    u32 *vram = (u32 *)0x06005000;
    s32 set;
    s32 n;

    for (set = 0; set < 2; set++) {
        for (n = 0; n < 6; n++) {
            u32 *tile = vram + set * 0x60 + n * 0x10;
            s32 x;

            FillWords(tile, 64, 0x44444444);
            for (x = 1; x <= 7; x++) {
                s32 mi = n;

                if (set == 1 && x <= 1) {
                    continue;
                }
                if (set == 0 && n > x - 2) {
                    mi = x - 2;
                    if (mi < 0) {
                        mi = 0;
                    }
                }
                tile[x] = XorWord(tile[x], MaskTbl[mi].word0);
                tile[x + 8] = XorWord(tile[x + 8], MaskTbl[mi].word1);
            }
        }
    }
}
