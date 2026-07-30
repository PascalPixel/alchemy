typedef signed int s32;
typedef signed short s16;
typedef unsigned char u8;

extern s32 *Func_02003a98(s32 a);
extern void Func_02003a98_b(void);
extern s32 Func_02003a40(s32 *p, s16 *q);
extern void Func_02003a20(s32 *p, s32 b);
extern void Func_020039ee(s32 a);
extern void Func_02003bbc(s32 a);
extern void Func_02003a34(s32 *p, s32 b);
extern void Func_02003a8e(s32 *p, s32 b);
extern s32 Func_02003b14(s32 a, s32 b, s32 c);
extern void Func_02003a6c(s32 *p, s32 b);
extern void Func_02003aac(s32 *p, s32 b);
extern void Func_02003b04(void);

s32 Func_02000cd0(s16 *arg) {
    s32 *p = Func_02003a98(0);
    u8 *f = (u8 *)p + 0x55;
    s32 saved = *f;

    s32 r = Func_02003a40(p, arg);

    if (r == 0) {
        s32 m;

        Func_02003a98_b();
        Func_02003a20(p, 6);
        Func_020039ee(6);
        Func_02003bbc(152);
        Func_02003a34(p, 7);
        p[12] = 0x30000;
        p[13] = 0x20000;
        p[10] = 0x40000;
        m = 0x7e;
        m &= *f;
        *f = m;
        Func_02003a8e(p, 0);
        Func_02003b14(0, arg[1], arg[5]);
        Func_02003a6c(p, 6);
        Func_02003aac(p, 1);
        *f = saved;
        Func_02003b04();
        return 1;
    }
    return 0;
}
