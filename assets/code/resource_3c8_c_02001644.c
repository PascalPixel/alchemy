typedef signed int s32;
typedef unsigned char u8;

typedef struct {
    u8 filler0[8];
    s32 unk8;
    s32 unkC;
    u8 filler10[4];
    s32 unk14;
    u8 filler18[61];
    u8 unk55;
} Struct_1644;

extern Struct_1644 *Func_020064f8();
extern void Func_020064e6(void);
extern void Func_020064ec(s32);
extern void Func_02006662(s32);
extern void Func_020064ac(s32, s32, s32, s32, s32, s32);
extern void Func_02006530(void);

void Func_02001644(void)
{
    Struct_1644 *o;
    s32 v;
    s32 a;
    s32 b;

    o = Func_020064f8(13);
    Func_020064e6();
    if (o->unk8 >> 20 == 42) {
        Func_020064ec(30);
        Func_02006662(188);
        o->unk55 = 0;
        v = 0xfffe0000;
        o->unk14 = v;
        o->unkC = v;
        Func_020064f8(0x200);
        a = 3;
        b = 5;
        Func_020064ac(44, 117, 41, 117, a, b);
    }
    Func_02006530();
}
