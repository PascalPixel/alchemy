typedef unsigned char u8;
typedef signed int s32;

typedef struct Obj {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 f14[28];
    s32 f30;
    s32 f34;
} Obj;

extern s32 Data_02000240[];

extern Obj *Func_020024f2(s32);
extern Obj *Func_020024fa(s32);
extern void Func_020024e8(void);
extern void Func_020024b2(Obj *, s32, s32, s32);
extern void Func_02002492(Obj *, s32);
extern void Func_020024de(Obj *, s32, s32, s32);
extern void Func_020024ca(Obj *, s32);
extern void Func_020024d4(Obj *, s32);
extern void Func_0200250a(Obj *);
extern void Func_0200257e(void);

void Func_02000180(s32 a0, s32 a1, s32 a2)
{
    Obj *p;
    Obj *q;
    s32 x;
    s32 y;

    p = Func_020024f2(Data_02000240[125]);
    q = Func_020024fa(a0);
    Func_020024e8();
    {
        x = ((p->f08 + (a1 << 16)) & 0xFFF00000) + 0x80000;
        y = ((p->f10 + (a2 << 16)) & 0xFFF00000) + 0x80000;

        p->f30 = 0x10000;
        p->f34 = 0x8000;
        Func_020024b2(p, x, p->f0c, y);
    }
    Func_02002492(p, 27);
    {
        x = ((q->f08 + (a1 << 16)) & 0xFFF00000) + 0x80000;
        y = ((q->f10 + (a2 << 16)) & 0xFFF00000) + 0x80000;

        q->f30 = 0x10000;
        q->f34 = 0x8000;
        Func_020024de(q, x, q->f0c, y);
    }
    if (a1 < 0 || a2 < 0) {
        Func_020024ca(q, 4);
    } else {
        Func_020024d4(q, 3);
    }
    Func_0200250a(p);
    Func_0200257e();
}
