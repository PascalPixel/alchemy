#include "types.h"
extern void Func_02007242(void);
extern s32 Func_02004612(s32, s32, s32, s32);
extern void Func_02007246(s32);
extern void Func_020071ea(s32, s32, s32, s32, s32, s32);
extern void Func_02007276(void);
void Func_02001280(void)
{
    s32 m, n;
    Func_02007242();
    Func_02004612(0, 13, 10, 1);
    Func_02007246(0x204);
    m = 20;
    n = 50;
    Func_020071ea(49, 46, 8, 4, m, n);
    Func_02007276();
}
