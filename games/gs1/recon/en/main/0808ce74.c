#include "types.h"

struct Position_0808bd24 {
    s32 x;
    s32 unknown_04;
    s32 y;
};

void RotateVectorByMagnitude(s32, u32, struct Position_0808bd24 *);
s32 Func_080091a8(s32, s32, s32);
s32 Func_0808d48c(s32, s32);

extern u8 Data_02000240[];
extern u8 *Data_03001ebc;

s32 Func_0808ce74(void)
{
    u32 runtimeSlotAddr = (u32)&Data_03001ebc;
    u8 *runtime = Data_03001ebc;
    u32 offset = *(u32 *)(Data_02000240 + 0x1f4) * 4 + 0x14;
    u8 *object = *(u8 **)(runtime + offset);
    u8 *map = *(u8 **)(runtimeSlotAddr - 76);
    struct Position_0808bd24 position;
    u8 *tile;
    s32 x, y;
    s32 result = 0;

    if (object != 0) {
        position.x = *(s32 *)(object + 8);
        position.unknown_04 = *(s32 *)(object + 12);
        position.y = *(s32 *)(object + 16);
        RotateVectorByMagnitude(0x100000, *(u16 *)(object + 6), &position);

        if (*(s16 *)(runtime + 0x19e) == 3) {
            x = position.x;
            if (x < 0)
                x += 0x1fffff;
            y = position.y;
            if (y < 0)
                y += 0x1fffff;
            tile = (u8 *)(0x02020000 +
                          ((((x >> 21) & 31) + (((y >> 21) & 31) << 5)) << 2));
        } else {
            tile = *(u8 **)(map + 0x130);
            x = position.x;
            if (x < 0)
                x += 0xfffff;
            y = position.y;
            if (y < 0)
                y += 0xfffff;
            tile = tile + (((x >> 20) + ((y >> 20) << 7)) << 2);
        }

        {
            u8 tileValue = tile[2];

            if ((u32)(tileValue - 242) <= 5) {
                s32 r = Func_080091a8(*(u8 *)(object + 34), position.x, position.y);
                s32 base = *(s32 *)(object + 12);

                if (r >= base && r <= base + 0x400000) {
                    result = tileValue;
                }
            } else {
                if (Func_0808d48c(tileValue, 3) != 0) {
                    result = tileValue;
                }
            }
        }
    }

    return result;
}
