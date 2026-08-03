#include "resource_38c.h"

typedef signed int s32;

extern void Func_02000878(void);
extern s32 Func_02000868(s32);
extern void Func_020008c2(s32);
extern void Func_020008da(s32, s32);
extern void Func_020008fe(s32, s32, s32);
extern void Func_020008ee(s32, s32);
extern void Func_020008d6(s32, s32);
extern void Func_02000920(s32, s32);
extern void Func_020008b6(s32);
extern void Func_020008c4(s32);
/* Reached with two and with three arguments from different sites. */
extern void Func_0200090e();
extern void Func_0200091c(s32);
extern void Func_02000934(s32, s32);
extern void Func_02000958(s32, s32, s32);
extern void Func_02000904(void);

void Func_0200035c(void)
{
    Func_02000878();

    if (Func_02000868(768) == 0) {
        Func_020008c2(0x16FF);
        Func_020008da(21, 0);
        Func_020008fe(21, 0x8000, 20);
        Func_020008ee(21, 0);
        Func_020008d6(22, 2);
        Func_02000920(22, 258);
        Func_020008b6(60);
        Func_0200090e(22, 0);
        Func_020008c4(10);
        Func_020008c4(768);
    }

    Func_0200090e(21, 0, 0);
    Func_0200091c(0x1702);
    Func_02000934(21, 0);
    Func_02000958(21, 0xC000, 10);
    Func_02000904();
}
