typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

typedef struct Obj {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 f14[65];
    u8 f55;
    u8 f56[14];
    s16 f64;
} Obj;

extern u8 Data_0200c008[];

extern Obj *Func_02006d24(s16);
extern void Func_02006c56(Obj *, s32, s32, s32);
extern void Func_02006c3e(Obj *, s32);
extern void Func_02006e8c(s32);

s32 Func_02003058(Obj *a)
{
    Obj *o = Func_02006d24(a->f64);

    Func_02006c56(o, a->f08, a->f0c + 0x240000, a->f10);
    o->f55 = 0;
    Func_02006c3e(o, (s32)Data_0200c008);
    Func_02006e8c(83);
    a->f64 = 0;
    return 0;
}
