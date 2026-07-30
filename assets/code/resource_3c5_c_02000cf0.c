typedef signed int s32;
typedef unsigned short u16;
typedef unsigned char u8;

struct Sub {
    u8 pad[30];
    u16 f1e;
};

struct P {
    u8 pad00[8];
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 pad14[4];
    s32 f18;
    s32 f1c;
    u8 pad20[16];
    s32 f30;
    s32 f34;
    u8 pad38[12];
    s32 f44;
    s32 f48;
    s32 f4c;
    struct Sub *f50;
    u8 pad54[16];
    u16 f64;
};

extern s32 Func_02003a8a();

void Func_02000cf0(struct P *p) {
    s32 a = p->f44;
    s32 c;

    p->f08 += a;
    p->f0c += p->f48;
    c = p->f4c;
    p->f10 += c;
    p->f44 = a - Func_02003a8a(a, 18);
    p->f4c = c - c / 16;
    p->f18 += p->f30;
    p->f1c += p->f34;
    p->f50->f1e += p->f64;
}
