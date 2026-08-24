#include "types.h"

extern void Func_02006b68(void);
extern s32 Func_02006b4e(s32);
extern void Func_02006b60(s32);
extern void Func_02006c46(s32);
extern void Func_02006c90(s32, s32, s32);
extern void Func_02006c4a(s32, s32, s32);
extern void Func_02006c7c(s32, s32, s32);
extern void Func_02006c98(s32, s32, s32);
extern void Func_02006c9c(s32, s32, s32);
extern void Func_02006c7e(s32, s32, s32);
extern void Func_02006cd8(s32, s32, s32);
extern void Func_02006cba(s32, s32, s32);
extern void Func_02006cee(s32, s32, s32);
extern void Func_02006cfa(s32, s32, s32);
extern void Func_02006cc0(s32);
extern void Func_02006ce2(s32, s32, s32);
extern void Func_02006c0e(void);

void Func_02000ba8(void)
{
    s32 k103 = 0x103;
    s32 k8000 = 0x8000;
    s32 k102 = 0x102;
    Func_02006b68();
    if (Func_02006b4e(0x807) == 0) {
        Func_02006b60(0x807);
        Func_02006c46(0xf63);
        Func_02006c90(18, k103, 0);
        Func_02006c4a(0, 18, 20);
        Func_02006c7c(18, 0, 6);
        Func_02006c98(18, k8000, 30);
        Func_02006c4a(18, 2, 20);
        Func_02006c9c(18, 0, 6);
        Func_02006c7e(18, 0, 10);
        Func_02006cd8(18, k103, 0);
        Func_02006cba(18, 0, 10);
        Func_02006cee(0, k102, 60);
    } else {
        Func_02006cfa(18, k103, 0);
        Func_02006cc0(0xf66);
        Func_02006ce2(18, 0, 20);
    }
    Func_02006c0e();
}
