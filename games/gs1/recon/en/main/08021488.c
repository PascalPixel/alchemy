#include "types.h"

#define M2C_FIELD(expr, type_ptr, offset) (*(type_ptr)((s8 *)(expr) + (offset)))

extern u8 *Data_03001e8c;

s32 Func_080162d4(s32, s32, s32, s32, s32);
void Func_0801e41c(s32, s32, s32, s32, s32);
s32 Func_08021360(u32);
s32 Func_08019d2c(void);
void Func_0801a4fc(s32, s32, s32 *, s32 *, s32, s32);
void Func_08019908(u32, u32);
s32 Func_08019ba0(s32);
void Func_080165d8(s32, s32, s32, s32, s32);
void Func_080f9010(s32);
void Func_08003dec(void *, s32);
void Func_080030f8(s32);
s32 Func_080f9048(void);
void Func_08016418(s32, s32);
s32 Func_08003f3c(s32);

s32 Func_08021488(s32 msg0, s32 msg1) {
    s32 spC;
    s32 sp10;
    s32 sp14;
    s32 sp18;
    s32 sp1C;
    s32 sp20;
    s32 sp24;
    s32 sp28;
    s32 sp2C;
    s32 window;
    u8 *base;
    void *box1;
    void *box2;

    base = Data_03001e8c;
    box1 = &sp18;
    window = Func_080162d4(1, 1, 0x1C, 5, 0);
    if (window != 0) {
        Func_0801e41c(window, 8, 0, 4, 4);
        M2C_FIELD(base, s8 *, 0xEA3) = 1;
        Func_08021360(msg0);
        Func_0801a4fc(Func_08019d2c(), 0, &sp14, &sp10, 0xE, 0);
        sp18 = 0;
        sp1C = 0x800C000C;
        sp20 = sp10 | 0xE000;
        box2 = &sp24;
        Func_08021360(msg1);
        Func_0801a4fc(Func_08019d2c(), 0, &spC, &sp10, 0xF, 0);
        sp24 = 0;
        sp28 = 0x802C000C;
        sp2C = sp10 | 0xF000;
        M2C_FIELD(base, s16 *, 0x12F4) = 0;
        M2C_FIELD(base, s16 *, 0x12F6) = 0;
        Func_08019908(msg0, 1);
        Func_08019908(msg1, 1);
        Func_080165d8(window, Func_08019ba0(0x1D), 0x44, 2, 0);
        Func_080f9010(0x51);
    loop_2:
        Func_08003dec(box1, 0xFA);
        Func_08003dec(box2, 0xFA);
        Func_080030f8(1);
        if (Func_080f9048() != 0) {
            if (!(*(s32 *)0x03001C94 & 0x303)) {
                goto loop_2;
            }
        }
        Func_08016418(window, 2);
        Func_080030f8(1);
        Func_08003f3c(sp14);
        window = Func_08003f3c(spC);
    }
    return window;
}
