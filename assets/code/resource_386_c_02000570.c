typedef unsigned char u8;
typedef signed int s32;

extern u8 *Func_02000b76(s32);
extern u8 *Func_02000b98(s32, s32);
extern void Func_02000bc8(s32);
extern void Func_02000bc0(s32, s32, u8 *);
extern void Func_02000bbe(s32);

void Func_02000570(s32 a)
{
    u8 *o;
    u8 *q;
    u8 *p;
    u8 *v;
    s32 z;
    s32 m;

    z = 0;
    o = Func_02000b76(22);
    if (o != 0) {
        q = *(u8 **)(o + 0x50);
        p = q + 38;
        *p = z;
        p += 1;
        *p = z;
        m = 33;
        m = -m;
        q[5] &= m;
        q[9] &= 15;
        o[0x55] = z;
        o[0x5c] = 1;
        v = Func_02000b98(17, 0x608);
        Func_02000bc8(a);
        v += 0x400;
        Func_02000bc0(q[28], 0x80, v);
        Func_02000bbe(17);
    }
}
