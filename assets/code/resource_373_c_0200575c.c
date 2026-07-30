typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

struct O {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct P {
    u8 f00[9];
    u8 f09;
};

struct Q {
    u8 f00[0x48];
    s32 f48;
    s32 f4c;
    struct P *f50;
    u8 f54;
    u8 f55;
    u8 f56[8];
    u16 f5e;
};

extern s32 Func_0200b60e(struct O *);
extern s32 Func_0200b622(void);
extern s32 Func_0200b628(void);
extern void Func_0200b656(s32, s32, s32 *);
extern struct Q *Func_0200b6a2(s32, s32, s32, s32);
extern void Func_0200b708(struct Q *, s32);
extern void Func_0200b6b8(struct Q *, s32);
extern void Func_0200b6c8(struct Q *, s32);
extern void Func_0200b92e(s32);
extern s32 Data_0200e6e0[];

void Func_0200575c(struct O *o)
{
    s32 buf[3];
    s32 n;
    struct Q *q;
    buf[0] = o->f08;
    buf[1] = o->f0c - (Func_0200b60e(o) << 4) + (s32)0xfff80000;
    buf[2] = o->f10;
    n = Func_0200b622();
    Func_0200b656(((n << 1) + n) << 4, Func_0200b628(), buf);
    q = Func_0200b6a2(0x11d, buf[0], buf[1], buf[2]);
    if (q != 0) {
        q->f55 = 2;
        q->f48 = 0x1999;
        q->f5e = 12;
        Func_0200b708(q, 0);
        Func_0200b6b8(q, 0);
        Func_0200b6c8(q, (s32)Data_0200e6e0);
        {
            struct P *p = q->f50;
            s32 v = ~12;
            v &= p->f09;
            v |= 4;
            p->f09 = v;
        }
    }
    Func_0200b92e(0x8a);
}
