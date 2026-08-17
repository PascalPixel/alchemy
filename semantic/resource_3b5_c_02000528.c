#include "types.h"

extern u8 Value_00001fa0;

extern void Func_02001288(void);
extern void Func_02001326(s32);
extern void Func_0200135a(s32, s32, s32);
extern void Func_0200134a(s32, s32);
extern void Func_0200136e(s32, s32, s32);
extern void Func_0200135e(s32, s32);
extern void Func_020012c2(void);

void Func_02000528(void)
{
    Func_02001288();
    Func_02001326((s32) &Value_00001fa0);
    Func_0200135a(25, 0xC000, 0);
    Func_0200134a(25, 0);
    Func_0200136e(25, 0x8000, 0);
    Func_0200135e(25, 0);
    Func_020012c2();
}
