typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;

typedef struct {
    u8 filler0[0xc];
    s32 unk0c;
    u8 filler10[8];
    s32 unk18;
    u8 filler1c[7];
    u8 unk23;
    u8 filler24[0x31];
    u8 unk55;
} Spr;

extern s16 Data_02000240[];

void Func_02006438(s32);
void Func_02006438_a(Spr *, s32);
void Func_02006326(s32);
void Func_02006446(s32);
Spr *Func_02006484(s32);
void Func_020063ea(Spr *, s32);
Spr *Func_02006490(s32);
void Func_020063f6(Spr *, s32);
Spr *Func_0200649c(s32);
void Func_02006402(Spr *, s32);
Spr *Func_020064a8(s32);
void Func_0200640e(Spr *, s32);
Spr *Func_020064b4(s32);
Spr *Func_020064be(s32);
Spr *Func_020064ca(s32);
Spr *Func_020064d2(s32);
void Func_020065b0(s32, s32);
s32 Func_020064f8(s32);
void Func_020010a8(void);
void Func_02002b5a(void);
void Func_020040c0(void);
void Func_020040e2(void);
void Func_02004130(void);
void Func_02006546(void);
s32 Func_02006524(s32);
s32 Func_02006532(s32);
s32 Func_02006540(s32);
void Func_02006534(s32, s32);
void Func_0200653e(s32, s32);
void Func_020066ac(s32);
s32 Func_02006562(s32);
s32 Func_02000d1a(void);
Spr *Func_020065b2(void);

s32 Func_0200071c(void)
{
    s32 k;
    u32 i;
    s32 z;
    s16 mode;
    Spr *s;

    Func_02006438(324);
    Func_02006326(1);
    Func_02006446(272);
    Func_020063ea(Func_02006484(8), 0);
    Func_020063f6(Func_02006490(9), 0);
    Func_02006402(Func_0200649c(10), 0);
    Func_0200640e(Func_020064a8(11), 0);
    k = (s32)0xffff0000;
    Func_020064b4(10)->unk18 = k;
    Func_020064be(11)->unk18 = k;
    i = 12;
    z = 0;
    do {
        s = Func_020064ca(i);
        Func_02006438_a(Func_020064d2(i), z);
        Func_020065b0(i, 1);
        s->unk55 = 4;
        s->unk23 |= 2;
        s->unk0c = 0x8000;
        i++;
    } while (i <= 17);

    mode = Data_02000240[225];

    switch (mode) {
    case 1:
        if (Func_020064f8(0x109) == 0) {
            Func_020010a8();
        }
        break;
    case 2:
        Func_02002b5a();
        break;
    case 3:
        Func_020040c0();
        break;
    case 0x5d:
        Func_020040e2();
        break;
    case 4:
        Func_02004130();
        break;
    case 9:
        Func_02006546();
        if (Func_02006524(0x345) != 0) {
            Func_02006534(0, 65);
        } else if (Func_02006532(0x346) != 0) {
            Func_02006534(1, 65);
        } else if (Func_02006540(0x347) != 0) {
            Func_02006534(2, 65);
        } else {
            Func_0200653e(3, 65);
        }
        Func_020066ac(9);
        break;
    }

    if (Func_02006562(0x109) != 0) {
        if (Func_02000d1a() != 0) {
            Spr *t = Func_020065b2();
            if (t != 0) {
                t->unk55 = 0;
            }
        }
    }
    return z;
}
