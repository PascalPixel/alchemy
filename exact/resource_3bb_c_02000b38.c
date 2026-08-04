#include "types.h"

extern void Func_02004a98(s32);
extern void Func_02004a9e(s32);
extern void Func_02004a74(s32);
extern void Func_02004a88(void);
extern void Func_02004aee(s32, s32, s32);
extern void Func_02004afc(s32, s32, s32);
extern void Func_02004b30();
extern void Func_02004b3c(s32, s32, s32);
extern void Func_02004b28(s32, s32);
extern void Func_02004b3a(s32, s32);
extern void Func_02004b42(s32, s32);
extern void Func_02004950(s32);
extern void Func_02004be0(s32, s32, s32, s32);
extern void Func_020036a2(s32);
extern void Func_02004b0a(void);

void Func_02000b38(s32 a)
{
    Func_02004a98(40);
    Func_02004a9e(41);
    Func_02004a74(1);
    Func_02004a88();
    Func_02004aee(8, 0x580000, 0x1000000);
    Func_02004afc(0, 0x780000, 0x1000000);
    Func_02004b30(8, 0x4000, 0);
    Func_02004b3c(0, 0x4000, 0);
    if (a < 0) {
        Func_02004b28(8, 10);
        Func_02004b30(0, 35);
    } else {
        Func_02004b3a(8, 8);
        Func_02004b42(0, 28);
    }
    Func_02004950(1);
    Func_02004be0(0x680000, 0, 0xC00000, 0);
    Func_020036a2(a);
    Func_02004b0a();
}
