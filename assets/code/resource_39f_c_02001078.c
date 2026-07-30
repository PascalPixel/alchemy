typedef signed int s32;
typedef unsigned char u8;

extern s32 *Func_02003e40(s32 a);
extern s32 Func_02001d82(s32 *p);
extern void Func_02003e5e(void);
extern void Func_02003ec2(s32 a, s32 b);
extern void Func_02003ec2_b(void);
extern void Func_02003dc6(s32 a);
extern void Func_02003dd8(s32 a);
extern void Func_02003dee(s32 a);

void Func_02001078(void) {
    s32 v[3];
    s32 *p = Func_02003e40(0);
    u8 *f = (u8 *)p + 0x55;
    s32 saved = *f;

    v[0] = (p[2] & 0xfff00000) + 0x80000;
    v[1] = p[3];
    v[2] = (p[4] & 0xfff00000) + 0x280000;
    if (Func_02001d82(v) != 0) {
        Func_02003e5e();
        *f = 0;
        Func_02003ec2(9, 7);
        p[3] += -0x10000;
        p[5] += -0x10000;
        Func_02003dc6(2);
        p[3] += -0x10000;
        p[5] += -0x10000;
        Func_02003dd8(10);
        p[3] += 0x10000;
        p[5] += 0x10000;
        Func_02003dee(4);
        p[3] += 0x10000;
        p[5] += 0x10000;
        *f = saved;
        Func_02003ec2_b();
    }
}
