#include "types.h"

struct ZoomLimit {
    s32 field0;
    s32 field1;
};

void Func_0800b074(u8 *window, s32 a1, s32 x1, s32 a3, s32 a4,
    struct ZoomLimit *limit)
{
    s32 halfW = (s8)window[32] >> 1;
    s32 halfH = (s8)window[33] >> 1;
    s32 marginA = 8;
    s32 marginB = 4;
    u32 flag = 1;
    s32 skew = (s8)window[35];
    s32 factor;
    s32 scaled;
    s32 clampedTop;
    s32 clampedBottom;
    u8 byte5;
    u16 half6;

    if (limit->field0 > 0x10000 || limit->field1 > 0x10000) {
        flag = 3;
        marginA = 16;
        marginB = 8;
        halfW <<= 1;
        halfH <<= 1;
    }

    factor = (halfH - skew) * limit->field1;
    scaled = (factor + 0x8000) >> 16;
    clampedTop = ((a3 - x1) >> 16) - halfH - scaled;

    byte5 = window[5];
    window[5] = (byte5 & ~3) | flag;

    half6 = *(u16 *)(window + 6);
    *(u16 *)(window + 6) = half6 & 0x1FF;
    window[4] = (u8)clampedTop;

    clampedBottom = ((a4 - x1) >> 16) - marginB;
    window[16 + 5] = (window[16 + 5] & ~3) | flag;
    half6 = *(u16 *)(window + 16 + 6);
    *(u16 *)(window + 16 + 6) = half6 & 0xFFFF;
    window[16 + 4] = (u8)clampedBottom;

    (void)a1;
    (void)marginA;
}
