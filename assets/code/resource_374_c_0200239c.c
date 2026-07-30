typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
#define NULL ((void *)0)

struct Sub {
    u8 pad00[8];
    s32 f08;
    u8 pad0c[4];
    s32 f10;
};

struct Obj {
    u8 pad00[8];
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 pad14[4];
    s32 f18;
    s32 f1c;
    u8 pad20[0x44];
    u16 f64;
    u8 pad66[2];
    struct Sub *f68;
};

s32 Func_020049cc(s32);
void Func_020049e4(struct Obj *);

void Func_0200239c(struct Obj *p) {
    struct Sub *q;
    s32 v;
    s32 r;
    s32 k;

    q = p->f68;
    p->f64 = p->f64 + 1;
    v = (s16) p->f64;
    if (v > 31) {
        Func_020049e4(p);
        return;
    }
    r = Func_020049cc(v << 10);
    p->f18 = r;
    p->f1c = r;
    p->f08 = q->f08;
    k = 0x10000;
    p->f0c = p->f0c + k;
    k = k - r;
    p->f10 = q->f10 + (k * 4 + k) + 0x80000;
}
