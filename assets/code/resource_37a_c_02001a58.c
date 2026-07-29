typedef signed int s32;
typedef signed short s16;
typedef unsigned char u8;

typedef struct {
    u8 filler0[10];
    s16 unkA;
    u8 fillerC[6];
    s16 unk12;
} EntB;

extern s32 Func_02004414(s32);
extern s32 Func_0200401a(void);
extern void Func_02004448(void);
extern void Func_0200449c(s32, s32, s32);
extern void Func_02004500(s32, s32, s32);
extern void Func_02004538(s32, s32, s32, s32);
extern void Func_020044f6(s32);
extern void Func_020044ba(s32, s32, s32);
extern void Func_020044da(s32, s32);
extern void Func_0200455e(void);
extern void Func_0200448c(s32);
extern void Func_020044c2(s32, s32, s32);
extern void Func_020044e6(s32, s32, s32);
extern void Func_020044ac(s32);
extern void Func_0200451e(s32, s32, s32);
extern void Func_020040ee(s32, s32);
extern void Func_02004526(s32, s32);
extern void Func_020044cc(s32);
extern void Func_02004534(s32, s32);
extern void Func_0200410c(s32, s32);
extern void Func_0200459e(s32, s32);
extern void Func_020044ec(s32);
extern void Func_02004564(s32, s32);
extern void Func_020044fa(s32);
extern void Func_02004132(s32, s32);
extern void Func_0200456a(s32, s32);
extern void Func_02004556(s32, s32, s32);
extern void Func_0200451c(s32);
extern void Func_02004594(s32, s32);
extern void Func_0200452a(s32);
extern void Func_02004162(s32, s32);
extern void Func_0200457e(s32, s32, s32);
extern void Func_02004544(s32);
extern void Func_020045ac();
extern void Func_02004552(s32);
extern void Func_02004588();
extern void Func_020045c0(s32, s32);
extern EntB *Func_0200458e(s32);
extern void Func_020045b0(s32, s32, s32);
extern void Func_020045ce(s32);
extern void Func_020045e0(s32, s32, s32);

void Func_02001a58(void)
{
    EntB *b;
    s32 s1 = 0x4000;
    s32 s2 = 0x4000;
    s32 a1 = 0x02410000;
    s32 k1 = 0x930000;
    s32 t1 = 0x10000;
    s32 h1 = 0x8000;
    s32 h2 = 0x8000;
    s32 d1 = 0x240;
    s32 d2 = 0x240;
    s32 d3 = 0x240;
    s32 d4 = 0x240;

    if (Func_02004414(0x810) != 0) {
        return;
    }
    if (Func_0200401a() == 0) {
        return;
    }
    Func_02004448();
    Func_0200449c(16, a1, k1);
    Func_02004500(16, s1, 1);
    Func_02004538(0x023e0000, -1, 0xb80000, 1);
    Func_020044f6(0x1027);
    Func_020044ba(0, d1, 0xe8);
    Func_020044da(0, 0);
    Func_0200455e();
    Func_0200448c(10);
    Func_020044c2(16, t1, h1);
    Func_020044e6(16, d2, 0x98);
    Func_020044ac(6);
    Func_0200451e(16, 6, 30);
    Func_020040ee(16, 6);
    Func_02004526(0, 3);
    Func_020044cc(2);
    Func_02004534(16, 4);
    Func_0200410c(16, 6);
    Func_0200459e(0, 0x102);
    Func_020044ec(40);
    Func_02004564(16, 2);
    Func_020044fa(30);
    Func_02004132(16, 6);
    Func_0200456a(0, 3);
    Func_02004556(16, d3, 0xb8);
    Func_0200451c(6);
    Func_02004594(16, 2);
    Func_0200452a(40);
    Func_02004162(0x4010, 6);
    Func_0200457e(16, d4, 0xd0);
    Func_02004544(40);
    Func_020045ac(0, 3);
    Func_02004552(6);
    Func_02004588(16, h2, s2);
    Func_020045c0(16, 2);
    b = Func_0200458e(0);
    if (b != 0) {
        Func_020045b0(16, b->unkA, b->unk12);
    }
    Func_020045ce(16);
    Func_020045e0(16, 0, 0);
    {
        s32 g2 = 0x810;
        Func_02004588(g2);
    }
    Func_020045ac();
}
