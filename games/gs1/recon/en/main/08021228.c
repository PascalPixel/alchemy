#include "types.h"

extern u8 *Data_03001e8c;

s32 Func_080162d4(s32, s32, s32, s32, s32);
void Func_0801e41c(s32, s32, s32, s32, s32);
void Func_08019d2c(s32);
s32 Func_0801a4fc(s32, s32, s32 *, s32 *, s32, s32);
void Func_08019908(s32, s32);
s32 Func_08019ba0(s32);
void Func_080165d8(s32, s32, s32, s32, s32);
void Func_080f9010(s32);
void Func_08003dec(s32 *, s32);
void Func_080030f8(s32);
s32 Func_080f9048(void);
void Func_08016418(s32, s32);
void Func_08003f3c(s32);

extern volatile u32 Data_03001c94;

void Func_08021228(s32 p1, s32 p2, s32 p3)
{
    u8 *base = Data_03001e8c;
    s32 tableVal = *(s16 *)(0x080371fe + (p2 & 3) * 2);
    s32 obj;
    s32 buf[4];
    s32 sp12, sp16;
    s32 d1;
    s32 d2;

    obj = Func_080162d4(2, 1, 26, 5, 0);

    if (obj != 0) {
        Func_0801e41c(obj, 4, 0, 4, 4);

        base[0xea3] = 1;

        Func_08019d2c(tableVal);
        d1 = Func_0801a4fc(0, 0, &sp16, &sp12, 14, 0);

        buf[0] = 0;
        buf[1] = 0x8014000c;
        buf[2] = sp12 | 0xe000;
        buf[3] = 0x00000303;

        *(u16 *)(base + 0x12f4) = 0;
        *(u16 *)(base + 0x12f6) = 0;

        Func_08019908(p1, 1);
        Func_08019908(p2 * 20 + p3 + 300, 4);

        d2 = Func_08019ba0(p2 + 0x980);

        Func_080165d8(obj, d2, 36, 2, 0);

        Func_080f9010(81);

        do {
            Func_08003dec(buf, 250);
            Func_080030f8(1);
        } while (Func_080f9048() != 0 && (Data_03001c94 & 0x303) == 0);

        Func_08016418(obj, 2);
        Func_080030f8(1);
        Func_08003f3c(sp16);
    }
}
