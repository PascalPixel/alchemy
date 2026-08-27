#include "types.h"

extern u8 *Data_03001e8c;
extern volatile s32 Data_03001c94;

s32 Func_080162d4(s32, s32, s32, s32, s32);
void Func_0801e41c(s32, s32, s32, s32, s32);
s32 Func_08021360(u32 index);
s32 Func_08019d2c(s32 value);
void Func_0801a4fc(s32, s32, s32 *, s32 *, s32, s32);
void Func_08019908(u32, u32);
s32 Func_08019ba0(s32);
void Func_080165d8(s32, s32, s32, s32, s32);
void Func_080f9010(s32);
void Func_08003dec(void *, s32);
void Func_080030f8(s32);
s32 Func_080f9048(void);
void Func_08016418(s32, s32);
void Func_08003f3c(s32);

s32 Func_08021390(s32 value)
{
    u8 *state = Data_03001e8c;
    s32 buf[7];
    s32 window;
    s32 result = 0;

    buf[0] = 0;
    window = Func_080162d4(2, 1, 26, 5, 0);

    if (window != 0) {
        Func_0801e41c(window, 4, 0, 4, 4);
        state[0xea3] = 1;
        Func_0801a4fc(
            Func_08019d2c(Func_08021360(value)), 0,
            &buf[3], &buf[2], 14, result);
        buf[4] = result;
        buf[5] = 0x8014000c;
        buf[6] = buf[2] | 0xe000;
        *(u16 *)(state + 0x12f4) = 0;
        *(u16 *)(state + 0x12f6) = 0;
        Func_08019908(value, 1);
        Func_080165d8(window, Func_08019ba0(0x1b), 36, 2, result);
        Func_080f9010(81);

        for (;;) {
            Func_08003dec(&buf[4], 250);
            Func_080030f8(1);
            if (Func_080f9048() == 0)
                break;
            if ((Data_03001c94 & 0x303) == 0)
                break;
        }

        Func_08016418(window, 2);
        Func_080030f8(1);
        Func_08003f3c(buf[3]);
    }

    return result;
}
