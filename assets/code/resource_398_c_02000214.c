#include "types.h"

extern void Func_02000b82(void);
extern void Func_02000ba4(s32, s32, s32);
extern void Func_02000b7a(s32);
extern void Func_02000b90(s32);
extern void Func_02000bc8(s32, s32);
extern u8 *Func_02000bb6(s32);
extern u8 *Func_02000bc2(s32);
extern void Func_02000bfc(s32, s32);
extern void Func_02000ba0(s32, s32, s32, s32, s32, s32);
extern void Func_02000be4(void);

void Func_02000214(void)
{
    Func_02000b82();
    Func_02000ba4(8, 0, 0);
    Func_02000b7a(0x883);
    Func_02000b90(0x28);
    Func_02000bc8(15, 2);
    Func_02000bb6(15)[0x55] = 0;
    { u8 *p = Func_02000bc2(15); p[0x23] = 2 | p[0x23]; }
    Func_02000bfc(15, 2);
    { s32 sx = 18, sz = 14; Func_02000ba0(0, 0, 1, 1, sx, sz); }
    Func_02000be4();
}
