#include "types.h"

/* Main-image symbols: every pool word inside the ROM or the work RAM. */
extern u8 Data_03001ef8[];

void BattlePresentation_DrawTransitionRows(void)
{
    u32 i;
    s32 rec;
    s32 q;
    s32 tile;
    u32 row;
    u16 *p;

    rec = *(s32 *)(*(s32 *)Data_03001ef8);
    if ((u32)rec <= 79) {
        tile = (7 & rec) + 0xf081;
        if (rec >= 0) {
            q = rec;
        } else {
            q = rec + 7;
        }
        row = 13 - (q >> 3);
        i = 0;
        p = (u16 *)((row << 6) + 0x06006000);
        do {
            i++;
            *p = tile;
            p++;
        } while (i != 32);

        tile = tile | 0x800;
        q = rec;
        if (rec < 0) {
            q = q + 7;
        }
        row = (q >> 3) + 13;
        if (row <= 20) {
            i = 0;
            p = (u16 *)((row << 6) + 0x06006000);
            do {
                i++;
                *p = tile;
                p++;
            } while (i != 32);
        }
    }
}
