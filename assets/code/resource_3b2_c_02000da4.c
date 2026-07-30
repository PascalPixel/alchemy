typedef signed int s32;
typedef unsigned short u16;
typedef unsigned char u8;

struct T {
    u8 pad00[30];
    u16 f1e;
};

struct S {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    s32 f14;
    s32 f18;
    s32 f1c;
    u8 pad20[16];
    s32 f30;
    s32 f34;
    u8 pad38[12];
    s32 f44;
    s32 f48;
    s32 f4c;
    struct T *f50;
    u8 pad54[16];
    u16 f64;
};

extern s32 Func_02003d76(s32 arg0, s32 arg1);

void Func_02000da4(struct S *p) {
    s32 vx = p->f44;
    s32 vz;

    p->f08 += vx;
    p->f0c += p->f48;
    vz = p->f4c;
    p->f10 += vz;
    vx -= Func_02003d76(vx, 18);
    p->f44 = vx;
    p->f4c = vz - vz / 16;
    p->f18 += p->f30;
    p->f1c += p->f34;
    p->f50->f1e += p->f64;
}
