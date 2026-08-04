#include "types.h"

struct State_08021390 {
    u8 filler0[0xEA3];
    u8 field_EA3;
    u8 filler_EA4[0x12F4 - 0xEA4];
    u16 field_12F4;
    u16 field_12F6;
};

extern struct State_08021390 *Data_03001e8c;
extern u32 Data_03001c94;

s32 Func_080162d4(s32, s32, s32, s32, s32);
void Func_0801e41c(s32, s32, s32, s32);
s32 Func_08021360(u32);
s32 Func_08019d2c(s32);
void Func_0801a4fc(s32, s32, s32 *, s32 *, s32, s32);
void Func_08019908(u32, u32);
s32 Func_08019ba0(s32);
void *Func_080165d8(s32, s32, s32, s32, const u16 *, s32);
void Func_080f9010(s32);
void Func_08003dec(u32 *, s32);
void Func_080030f8(s32);
s32 Func_080f9048(void);
void Func_08016418(s32, s32);
void Func_08003f3c(s32);

void Func_08021390(u32 arg0)
{
    s32 sp8;
    s32 spC;
    volatile u32 sp18;
    volatile u32 sp14;
    volatile u32 sp10;
    u32 state = (u32)Data_03001e8c;
    s32 handle = Func_080162d4(2, 1, 26, 5, 0);
    u32 *desc = (u32 *)&sp10;
    s32 zero = 0;

    if (handle != 0) {
        Func_0801e41c(handle, 4, 0, 4);
        *(u8 *)(state + 0xEA3) = 1;
        Func_0801a4fc(Func_08019d2c(Func_08021360(arg0)), 0, &spC, &sp8, 14, zero);
        *desc = zero;
        sp14 = 0x8014000C;
        sp18 = (u32)sp8 | 0xE000;
        *(u16 *)(state + 0x12F4) = zero;
        *(u16 *)(state + 0x12F6) = zero;
        Func_08019908(arg0, 1);
        Func_080165d8(
            handle, Func_08019ba0(0x1B), 36, 2, 0, zero);
        Func_080f9010(81);
        do {
            Func_08003dec(desc, 250);
            Func_080030f8(1);
            if (Func_080f9048() == 0) {
                break;
            }
        } while ((Data_03001c94 & 0x303) == 0);
        Func_08016418(handle, 2);
        Func_080030f8(1);
        Func_08003f3c(spC);
    }
}
