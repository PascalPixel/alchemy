#include "types.h"
#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void Func_02002808();
void Func_0200281a(s32);
void Func_0200283e();
void Func_020028aa(s32, s32, s32);
void Func_020028d0(s32);
void Func_020028e8(s32, s32);
void Func_0200290c(s32, s32, s32);

void Func_02000ef8(void) {
    Func_02002808();
    Func_020028aa(0x10, 0, 0xA);
    Func_020028d0(0x1C13);
    Func_020028e8(0x10, 0);
    Func_0200290c(0x10, 0xB000, 0xA);
    Func_0200281a(0x301);
    Func_0200283e();
}
