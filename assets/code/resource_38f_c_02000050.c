typedef signed int s32;
typedef unsigned int u32;
typedef signed short s16;
typedef unsigned short u16;

struct Obj {
    s32 f00, f04, f08, f0c, f10, f14, f18, f1c;
    s32 f20, f24, f28, f2c, f30, f34, f38, f3c;
    s32 f40, f44, f48, f4c, f50, f54, f58, f5c;
    s32 f60;
    u16 f64;
};

s32 Func_02002a0a(struct Obj *);
void Func_02002b6e(struct Obj *, s32);
void Func_02002b78(struct Obj *, s32);

s32 Func_02000050(struct Obj *p) {
    s32 v = Func_02002a0a(p);
    s32 t = v * 100;
    s32 h = p->f64 + ((u32)t >> 16);
    p->f64 = h;
    if ((s16)h > 1000) {
        Func_02002b6e(p, 7);
    } else {
        Func_02002b78(p, 10);
    }
    if ((s16)p->f64 > 1200) {
        p->f64 = 0;
    }
    return 1;
}
