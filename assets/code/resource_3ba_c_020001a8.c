typedef unsigned char u8;
typedef signed int s32;

typedef struct Obj {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 f14[15];
    u8 f23;
    u8 f24[49];
    u8 f55;
} Obj;

extern void Func_02003db6(s32, s32, s32, s32, s32, s32);
extern Obj *Func_02003e7c(s32);
extern s32 Func_02003db8(s32, s32, s32);
extern void Func_02003df8(s32, s32, s32, s32, s32, s32);
extern Obj *Func_02003ebe(s32);
extern void Func_02003e84(s32, s32);
extern void Func_02003e24(s32, s32, s32, s32, s32, s32);

void Func_020001a8(void)
{
    Obj *o;
    s32 r;

    {
        s32 x = 23;
        s32 y = 12;

        Func_02003db6(27, 13, 3, 1, x, y);
    }
    o = Func_02003e7c(9);
    r = Func_02003db8(0, o->f08, o->f10);
    if (o->f0c == 0 && r == 0) {
        o->f23 = 2;
        o->f55 = 0;
        {
            s32 x = o->f08 >> 20;
            s32 y = o->f10 >> 20;

            Func_02003df8(14, 13, 1, 1, x, y);
        }
    }
    o = Func_02003ebe(10);
    {
        s32 x = o->f08 >> 20;

        Func_02003e84(784, x);
    }
    {
            s32 x = o->f08 >> 20;
            s32 y = o->f10 >> 20;

        Func_02003e24(14, 13, 1, 1, x, y);
    }
}
