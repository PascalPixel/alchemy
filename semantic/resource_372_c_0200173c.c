#include "types.h"
extern u8 Value_00000e70;

extern s32 Func_02005f20(s32, s32);
extern void Func_02005f12(s32, s32, s32);
extern s32 Func_02005e7c(s32, s32);
extern void Func_02005f36(s32);
extern void Func_02005f40(s32);
extern void Func_02005e66(s32);
extern void Func_02005f68(s32, s32, s32);
extern void Func_02005f9a(s32, s32);
extern void Func_02005f22(s32, s32);
extern void Func_02005f2a(s32, s32);
extern void Func_02005e90(s32);
extern void Func_02005f62(s32, s32, s32);
extern void Func_02005ea0(s32);
extern void Func_02005f50(s32, s32);
extern void Func_02005f92(s32);
extern void Func_02005f9a_b(s32);
extern void Func_02005fb2(s32, s32);
extern void Func_02005f6a(s32, s32);
extern u8 *Func_02005f08(s32);
extern void Func_02005f52(s32, s32, s32);
extern void Func_02005f78(s32);
extern void Func_02005f8a(s32, s32, s32);
extern void Func_02005f1a(s32, s32);
extern void Func_02005ef0(s32);

void Func_0200173c(void)
{
    s32 flag;
    u8 *p;

    Func_02005f20(22, 0);
    Func_02005f12(0, 22, 0);
    flag = 0;
    if (Func_02005e7c(0, 0) == 0) {
        Func_02005f36(0xee5);
        flag = 1;
    } else {
        Func_02005f40(0xee6);
    }
    Func_02005e66(20);
    Func_02005f68(22, 0, 40);
    Func_02005f9a(22, 0x100);
    Func_02005f22(21, 3);
    Func_02005f2a(22, 1);
    Func_02005e90(40);
    Func_02005f62(22, 0, 0);
    Func_02005ea0(20);
    Func_02005f50(22, 3);
    if (flag != 0) {
        Func_02005f92((s32)&Value_00000e70);
    } else {
        Func_02005f9a_b(0xee7);
    }
    Func_02005fb2(22, 0);
    Func_02005f6a(22, 2);
    p = Func_02005f08(0);
    if (p != 0) {
        Func_02005f52(22, *(s16 *)(p + 10), *(s16 *)(p + 18));
    }
    Func_02005f78(22);
    Func_02005f8a(22, 0, 0);
    Func_02005f1a(1, 1);
    Func_02005ef0(0x837);
}
