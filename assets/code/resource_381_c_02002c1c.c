typedef unsigned char u8;
typedef signed int s32;

typedef struct {
    u8 filler0[8];
    s32 unk8;
    u8 fillerC[4];
    s32 unk10;
    u8 filler14[16];
    s32 unk24;
    u8 filler28[4];
    s32 unk2C;
    u8 filler30[8];
    s32 unk38;
    u8 filler3C[4];
    s32 unk40;
} Ent;

extern Ent *Func_020061fc(s32);
extern void Func_02006204(s32, s32, s32, s32);
extern void Func_02006234(s32, s32);
extern void Func_02006242(s32);
extern void Func_02006038(s32);
extern void Func_02006052(s32);
extern void Func_020060d6(void);
extern void Func_0200605c(s32);
extern void Func_0200626e(s32, s32);
extern void Func_0200627c(s32);
extern void Func_02006072(s32);

void Func_02002c1c(s32 a, s32 b)
{
    Ent *p;

    p = Func_020061fc(a);
    b = b << 16;
    a = a << 16;
    Func_02006204(a, -1, b, 1);
    Func_02006234(0, 0);
    Func_02006242(20);
    Func_02006038(40);
    p->unk10 = b;
    p->unk8 = a;
    p->unk38 = 0x80000000;
    p->unk40 = 0x80000000;
    p->unk24 = 0;
    p->unk2C = 0;
    Func_02006052(5);
    Func_020060d6();
    Func_0200605c(5);
    Func_0200626e(0x10000, 0);
    Func_0200627c(20);
    Func_02006072(30);
}
