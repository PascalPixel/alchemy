typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16; typedef signed int s32; typedef unsigned int u32;
#define F(base, type, off) (*(type *)((u8 *)(base) + (off)))

extern u8 *Func_02005e8c(s32);
extern u8 *Func_0200073e(s32 *, u8 *);
extern u8 *Func_0200076a(s32 *, u8 *);
extern u8 *Func_02000796(s32 *, u8 *);
extern s32 Func_02005ea8(u8 *, s32 *);
extern void Func_02005e60(u8 *, s32);
extern void Func_02005e20(s32);
extern void Func_0200610e(s32);
extern void Func_02005ea6(u8 *, s32, s32, s32);
extern void Func_02005eb6(u8 *, s32, s32, s32);
extern void Func_02005ec4(u8 *);
extern void Func_02006130(void);
extern void Func_02005ecc(u8 *, s32);
extern s32 Data_0200dd50[];

void Func_020003a8(void)
{
    s32 pos[3];
    u8 *p;
    u8 *v;
    u8 *q;
    u32 m;
    s32 d;
    u32 idx;
    s32 flag;
    s32 k;

    p = Func_02005e8c(0);
    idx = F(p, u16, 6) >> 12;
    d = Data_0200dd50[idx];
    m = 0xffff0000;
    {
        s32 t = d & m;
        pos[0] = F(p, s32, 8) + t;
        pos[1] = F(p, s32, 12);
        d <<= 16;
        pos[2] = F(p, s32, 16) + d;
    }
    v = Func_0200073e(pos, p);
    if (v == 0) {
        return;
    }
    d = Data_0200dd50[idx];
    {
        s32 t = d & m;
        pos[0] = F(v, s32, 8) + t;
        pos[1] = F(v, s32, 12);
        d <<= 16;
        pos[2] = F(v, s32, 16) + d;
    }
    q = Func_0200076a(pos, v);
    if (q != 0 && (F(q, u8, 0x59) & 1)) {
        return;
    }
    pos[0] = F(v, s32, 8);
    pos[1] = F(v, s32, 12) + 0x100000;
    pos[2] = F(v, s32, 16);
    q = Func_02000796(pos, v);
    if (q != 0 && (F(q, u8, 0x59) & 1)) {
        return;
    }
    F(v, u8, 0x22) = 2;
    d = Data_0200dd50[idx];
    {
        s32 t = d & m;
        pos[0] = F(v, s32, 8) + t;
        pos[1] = F(v, s32, 12);
        d <<= 16;
        pos[2] = F(v, s32, 16) + d;
    }
    if (Func_02005ea8(v, pos) > 0) {
        return;
    }
    flag = F(v, u8, 0x62);
    if (flag != 0) {
        return;
    }
    Func_02005e60(p, 8);
    k = 0x3333;
    Func_02005e20(15);
    Func_0200610e(0xb9);
    F(v, s32, 0x30) = k;
    F(v, s32, 0x34) = k;
    Func_02005ea6(v, pos[0], pos[1], pos[2]);
    F(p, s32, 0x30) = k;
    F(p, s32, 0x34) = k;
    Func_02005eb6(p, pos[0], pos[1], pos[2]);
    Func_02005ec4(v);
    Func_02006130();
    F(v, s32, 8) = pos[0];
    F(v, s32, 16) = pos[2];
    F(v, s32, 0x24) = flag;
    F(v, s32, 0x2c) = flag;
    F(p, s32, 0x38) = 0x80000000;
    F(p, s32, 0x40) = 0x80000000;
    F(p, s32, 8) = F(p, s16, 10) << 16;
    F(p, s32, 0x24) = flag;
    F(p, s32, 0x2c) = flag;
    F(p, s32, 16) = F(p, s16, 18) << 16;
    Func_02005ecc(p, 1);
}
