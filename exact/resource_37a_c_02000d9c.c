#include "types.h"

extern s32 Func_02003726(void);
extern void Func_020038ca(s32);
extern void Func_0200377e(s32, s32, s32, s32, s32, s32);
extern void Func_02003790(s32, s32, s32, s32, s32, s32);
extern void Func_02003808(s32, s32, s32, s32, s32, s32);
extern void Func_020037b0(s32, s32, s32, s32, s32, s32);
extern void Func_020037d0(s32, s32, s32, s32, s32, s32);
extern void Func_020037f6(s32, s32, s32, s32, s32, s32);
extern void Func_0200381e(s32, s32, s32, s32, s32, s32);
extern s32 Func_0200381a(void);
extern void Func_02003870(s32, s32, s32);
extern void Func_02003890(s32, s32, s32);
extern u16 Data_0200ade4;
extern s32 Data_0200ade8;

void Func_02000d9c(void)
{
    s32 *st;
    s32 s;
    s32 t1 = 0x10000;
    s32 t2 = 0x10000;
    s32 t3 = 0x10000;
    s32 m1 = -1;
    s32 m2 = -1;
    s32 m3 = 0xe666;

    if ((Func_02003726() & 3) != 0) {
        u16 v = Data_0200ade4;
        switch (v) {
        case 0: {
            s32 a;
            s32 b;
            Func_020038ca(0xbb);
            a = 1;
            b = 5;
            Func_0200377e(0x2e, 0x3b, 30, 0x21, a, b);
            break;
        }
        case 1:
            Func_02003790(0x2e, 0x3b, 30, 0x21, v, v);
            Func_02003808(0x2e, 0x3b, 30, 0x22, v, 5);
            break;
        case 2:
            v = 1;
            Func_020037b0(0x2e, 0x3b, 30, 0x22, v, v);
            Func_02003808(0x2e, 0x3b, 30, 0x23, v, 5);
            break;
        case 3:
            v = 1;
            Func_020037d0(0x2e, 0x3b, 30, 0x23, v, v);
            Func_02003808(0x2e, 0x3b, 30, 0x24, v, 5);
            break;
        case 4: {
            s32 val = 2;
            s32 *p = &Data_0200ade8;
            *p = val;
        }
            v = 1;
            Func_020037f6(0x2e, 0x3b, 30, 0x24, v, v);
            Func_02003808(0x2e, 0x3b, 30, 0x25, v, 5);
            break;
        case 0x50: {
            s32 a = 1;
            s32 b = 10;
            Func_0200381e(0x2e, 0x31, 30, 0x21, a, b);
            break;
        }
        }
        {
            u16 *c = &Data_0200ade4;
            u32 w;
            *c = *c + 1;
            w = *c;
            if (w > ((u32)(Func_0200381a() * 40) >> 16) + 90) {
                *c = 0;
            }
        }
    }
    st = &Data_0200ade8;
    s = *st;
    if (s != 0) {
        if (s == 2) {
            Func_02003870(t1, t2, t3);
            *st = *st - 1;
        } else {
            if (s == 1) {
                Func_02003890(m1, m2, m3);
            }
            *st = *st - 1;
        }
    }
}
