typedef signed int s32;

struct Obj {
    s32 f00, f04, f08, f0c, f10, f14;
    s32 f18;
    s32 f1c, f20, f24, f28, f2c, f30, f34;
    s32 f38, f3c, f40;
};

void Func_02004fd6(struct Obj *);

s32 Func_020025d8(struct Obj *p) {
    p->f18 += 0x1eb8;
    if (p->f38 == 0x80000000 && p->f3c == p->f38 && p->f40 == p->f3c) {
        Func_02004fd6(p);
    }
    return 1;
}
