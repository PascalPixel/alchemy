#include "types.h"

extern s32 *Func_02009548(s32);
extern u32 Func_02009426(void);
extern u32 Func_02009440(void);
extern u32 Func_02009452(void);
extern u32 Func_02009466(void);
extern void Func_02003c40(s32, s32, s32, s32, s32, s32, s32, s32 *);
extern u32 Data_03001e40;

void Func_02003a64(void)
{
    s32 buf[10];
    s32 *p = Func_02009548(0);
    s32 m = Data_03001e40 & 3;

    if (m == 0) {
        buf[1] = 7;
        if ((((Func_02009426() << 1) >> 16) & 1) == 0)
            buf[1] = 5;
        buf[2] = 0xb333;
        buf[3] = 0xb333;
        {
            s32 y = p[3] + (((Func_02009440() << 2) >> 16) << 16);
            s32 a = ((Func_02009452() << 3) >> 16) * 0x3333 + 0xffff3334;
            s32 b = ((Func_02009466() << 3) >> 16) * 0x3333 + 0xffff3334;

            Func_02003c40(p[2], y, p[4], a, b, m, 0x90000, buf);
        }
    }
}
