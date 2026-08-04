#include "types.h"

extern u32 Func_0200958a(void);
extern u32 Func_0200959a(void);
extern void Func_02003d68(s32, s32, s32, s32, s32, s32, s32, s32 *);

void Func_02003bd4(s32 x, s32 y, s32 z)
{
    s32 buf[10];

    buf[1] = 7;
    buf[0] = 1;
    buf[2] = 0xb333;
    buf[3] = 0xb333;
    {
        s32 a = x + (((Func_0200958a() << 4) >> 16) << 16) + 0xfff80000;
        s32 b = z + (((Func_0200959a() << 3) >> 16) << 16) + 0xfffc0000;

        Func_02003d68(a, y, b, 0, 0, 0, 0xb0000, buf);
    }
}
