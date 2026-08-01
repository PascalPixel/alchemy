typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;

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

extern Obj *Func_02004282(s32);
extern Obj *Func_0200428a(s32);
extern void Func_02004278(void);
extern void Func_020041f2(Obj *, s32, s32, s32);
extern void Func_020041ca(Obj *, s32);
extern void Func_0200421e(Obj *, s32, s32, s32);
extern void Func_02004202(Obj *, s32);
extern void Func_0200420c(Obj *, s32);
extern void Func_0200447a(s32);
extern void Func_02004250(Obj *);
extern void Func_02004228(Obj *, s32);
extern void Func_02004488(s32);
extern void Func_02004324(void);

void Func_02000310(s32 a0, s32 a1, s32 a2)
{
    Obj *p;
    Obj *q;
    s32 x;
    s32 y;

    p = Func_02004282(Data_02000240[125]);
    q = Func_0200428a(a0);
    Func_02004278();
    {
        x = ((p->f08 + (a1 << 16)) & 0xFFF00000) + 0x80000;
        y = ((p->f10 + (a2 << 16)) & 0xFFF00000) + 0x80000;

        p->f30 = 0x10000;
        p->f34 = 0x8000;
        Func_020041f2(p, x, p->f0c, y);
    }
    Func_020041ca(p, 27);
    {
        x = ((q->f08 + (a1 << 16)) & 0xFFF00000) + 0x80000;
        y = ((q->f10 + (a2 << 16)) & 0xFFF00000) + 0x80000;

        q->f30 = 0x10000;
        q->f34 = 0x8000;
        Func_0200421e(q, x, q->f0c, y);
    }
    if (a1 < 0 || a2 < 0) {
        Func_02004202(q, 4);
    } else {
        Func_0200420c(q, 3);
    }
    Func_0200447a(226);
    Func_02004250(p);
    Func_02004488(288);
    Func_02004228(q, 2);
    Func_02004324();
}
