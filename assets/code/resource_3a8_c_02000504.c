#include "types.h"

extern void Func_020040fe(void);
extern void Func_0200428c(s32);
extern void Func_020040be(s32, s32, s32, s32, s32, s32);
extern void Func_0200403c(s32);
extern void Func_020040d4(s32, s32, s32, s32, s32, s32);
extern void Func_02004052(s32);
extern void Func_02004178(s32, s32, s32);
extern s32 Func_0200416e(s32);
extern void Func_020041dc(s32, s32);
extern void Func_020041d8(s32, s32, s32);
extern void Func_0200415e(s32);
extern void Func_0200429c(s32);
extern void Func_020042b0(void);
extern void Func_020042bc(void);
extern void Func_02004180(void);

void Func_02000504(void) {
    s32 k = 2;
    u8 *p;

    Func_020040fe();
    Func_0200428c(188);
    Func_020040be(36, 23, 43, 12, k, k);
    Func_0200403c(5);
    Func_020040d4(39, 23, 43, 12, k, k);
    Func_02004052(5);
    Func_02004178(0, 0x8000, 0x4000);
    p = (u8 *)(Func_0200416e(0) + 85);
    *p = 0;
    Func_020041dc(0, 2);
    Func_020041d8(0, 0, -8);
    Func_0200415e(10);
    Func_0200429c(2);
    Func_020042b0();
    Func_020042bc();
    Func_02004180();
}
