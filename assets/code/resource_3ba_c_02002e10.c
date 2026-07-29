typedef unsigned char u8;
typedef signed int s32;

typedef struct Obj {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    u8 f10[32];
    s32 f30;
    s32 f34;
    u8 f38[35];
    u8 f5b;
} Obj;

extern Obj *Func_02006be0(s32);
extern void Func_020069fc(Obj *);
extern void Func_020069e4(Obj *, s32);
extern void Func_02006a20(Obj *, s32, s32, s32);

void Func_02002e10(s32 a, s32 b, s32 c)
{
    Obj *o = Func_02006be0(a);

    if (o != 0) {
        s32 v = 0x20000;
        s32 z = 0;

        o->f30 = v;
        o->f34 = v >> 1;
        o->f5b = z;
        Func_020069fc(o);
        Func_020069e4(o, 5);
        Func_02006a20(o, b << 16, o->f0c, c << 16);
    }
}
