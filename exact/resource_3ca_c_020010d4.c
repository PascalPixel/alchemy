#include "types.h"

extern u8 *Func_02002430(s32, s32);
extern void Func_0200246e(void *, s32);
extern void Func_02002478(void *, s32);
extern u16 Data_02008f81[];
extern u16 Data_02008f31[];

void Func_020010d4(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g)
{
    u8 *o;
    s32 *src;
    u32 *dma;
    volatile u32 *p;
    s32 zero;
    s32 n;
    s32 ac, bc, cc, dc;

    bc = b;
    n = 0xF20;
    ac = a;
    cc = c;
    dc = d;
    o = Func_02002430(0x22, n);
    src = &zero;
    *src = 0;
    dma = (u32 *)0x040000D4;
    dma[0] = (u32)src;
    dma[1] = (u32)o;
    dma[2] = 0x850003C8;
    p = (volatile u32 *)dma;
    do {
    } while (p[2] & 0x80000000);
    o[0xF01] = ac;
    *(s32 *)(o + 0xF08) = bc;
    *(s32 *)(o + 0xF0C) = e;
    *(s32 *)(o + 0xF18) = dc;
    *(s32 *)(o + 0xF1C) = g;
    *(s32 *)(o + 0xF10) = cc;
    *(s32 *)(o + 0xF14) = f;
    n = 0xC80;
    Func_0200246e(Data_02008f81, n);
    n = 0x480;
    Func_02002478(Data_02008f31, n);
}
