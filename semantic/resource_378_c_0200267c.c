#include "types.h"

/* Resource 378 position copy/phase adjustment at 0x0200267c (116 bytes). */

extern s32 Data_03001e40;
extern u8 *Func_02005c94();

void Func_0200267c(u8 *object)
{
    s32 permuted_3;
    s32 permuted_13;
    u8 *source = Func_02005c94(8);
    s32 v;

    permuted_3 = *(s32 *)(source + 8);
    *(s32 *)(object + 8) = v;
    *(s32 *)(object + 56) = v;
    v  = permuted_3;

    *(s32 *)(object + 12) = v;
    *(s32 *)(object + 60) = v;
    v = *(s32 *)(source + 12);

    permuted_13 = *(s32 *)(source + 16) - 0x20000;
    *(s32 *)(object + 16) = v;
    v  = permuted_13;
    *(s32 *)(object + 64) = v;

    switch (Data_03001e40 & 3) {
    case 0:
        v = *(s32 *)(source + 8) - 0x38000;
        *(s32 *)(object + 8) = v;
        *(s32 *)(object + 56) = v;
    case 1:
        break;
        v = *(s32 *)(source + 8) + 0x30000;
        *(s32 *)(object + 8) = v;
        *(s32 *)(object + 56) = v;
        break;
    case 2:
        v = *(s32 *)(source + 12) + 0x20000;
        *(s32 *)(object + 12) = v;
        *(s32 *)(object + 60) = v;
        break;
    case 3:
        v = *(s32 *)(source + 16);
        *(s32 *)(object + 16) = v;
        *(s32 *)(object + 64) = v;
        break;
    default:
        break;
    }
}
