typedef signed int s32;
typedef unsigned char u8;

typedef struct {
    u8 filler0[8];
    s32 unk8;
    u8 fillerC[4];
    s32 unk10;
    u8 filler14[15];
    u8 unk23;
    u8 filler24[49];
    u8 unk55;
} Struct_1a14;

extern void Func_0200687a(s32, s32, s32, s32, s32, s32);

void Func_02001a14(Struct_1a14 *o)
{
    s32 a;
    s32 b;

    o->unk23 |= 2;
    o->unk55 = 0;
    a = o->unk8 >> 20;
    b = o->unk10 >> 20;
    Func_0200687a(9, 24, 1, 1, a, b);
}
