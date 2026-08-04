#include "types.h"

extern u32 Func_020094e8(void);
extern u32 Func_02009502(void);
extern u32 Func_02009516(void);
extern void Func_02003cee(s32, s32, s32, s32, s32, s32, s32, s32 *);
extern u32 Data_03001e40;

void Func_02003b24(s32 x, s32 y, s32 z)
{
    s32 buf[10];
    s32 m = Data_03001e40 & 3;

    if (m == 0) {
        buf[1] = 7;
        if ((((Func_020094e8() << 1) >> 16) & 1) == 0)
            buf[1] = 5;
        buf[2] = 0xb333;
        buf[3] = 0xb333;
        {
            s32 a = ((Func_02009502() << 3) >> 16) * 0x3333 + 0xffff3334;
            s32 b = ((Func_02009516() << 3) >> 16) * 0x3333 + 0xffff3334;

            Func_02003cee(x, y, z, a, b, m, 0x90000, buf);
        }
    }
}
