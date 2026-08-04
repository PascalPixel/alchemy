#include "types.h"

extern void Func_0200662c(void);
extern void Func_020066e2(s32);
extern void Func_020066cc(s32, s32, s32);
extern void Func_0200663a(s32);
extern void Func_0200670c(s32, s32, s32);
extern void Func_02006720(s32, s32, s32);
extern void Func_0200674c(s32, s32);
extern void Func_02006766(s32, s32, s32, s32);
extern void Func_02006772(void);
extern void Func_0200673a(s32, s32);
extern void Func_0200668e(void);

void Func_02001780(void)
{
    u8 *state;
    u16 *p;
    u16 *q;
    s32 x;
    s32 y;

    state = *(u8 **)0x03001ebc;
    p = (u16 *)(state + 0xcba);
    x = 0;
    *p = x;
    q = (u16 *)(state + 0xcb6);
    y = 1;
    *q = y;
    Func_0200662c();
    Func_020066e2(0x267d);
    Func_020066cc(10, 0, 0);
    Func_0200663a(10);
    Func_0200670c(10, 0, 20);
    Func_02006720(10, 0xe000, 0);
    Func_0200674c(0x10000, 0x2000);
    Func_02006766(0x1c00000, -1, 0x1b00000, 1);
    Func_02006772();
    Func_0200673a(10, 0);
    Func_0200668e();
}
