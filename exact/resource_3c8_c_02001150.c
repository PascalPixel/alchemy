#include "types.h"

typedef void (*Fn)(void);

typedef struct {
    u8 filler0[10];
    s16 unkA;
    u8 fillerC[4];
    s32 unk10;
    u8 filler14[20];
    s32 unk28;
    u8 filler2C[64];
    Fn unk6C;
} Struct_1150;

extern u8 Data_0200d21c[];
extern void Func_020085e4(void);

extern Struct_1150 *Func_02006002(s32);
extern void Func_02005ff0(void);
extern void Func_02006020(s32, u8 *);
extern void Func_0200602e(s32);
extern void Func_020060a6(s32, s32);
extern void Func_02006036(s32, s32, s32);
extern u8 *Func_0200603c(s32);
extern u8 *Func_02006050(s32);
extern void Func_02006098(s32, s32, s32);
extern void Func_0200604e(s32);
extern u8 *Func_02006074(s32);
extern void Func_02006064(s32);
extern void Func_02006154(s32, s32, s32);
extern void Func_020060fc(s32, s32);
extern void Func_0200612c(s32, s32);
extern void Func_0200610c(s32, s32);
extern void Func_020060a4(void);

void Func_02001150(void)
{
    Struct_1150 *s;
    u8 *p;
    s32 x;
    s32 c;
    s32 y;
    s32 z;
    s32 k;
    u8 v;
    u8 m;

    s = Func_02006002(0);
    Func_02005ff0();
    Func_02006020(0, Data_0200d21c);
    Func_0200602e(0);
    Func_020060a6(0, 6);
    z = 128;
    x = 0x40000;
    s->unk28 = x;
    Func_02006036(0, x, z << 10);
    if (s->unk10 >> 20 <= 54) {
        p = Func_0200603c(0) + 90;
        *p &= 0xfe;
        c = 210;
    } else {
        p = Func_02006050(0) + 90;
        *p &= 0xfe;
        c = 238;
    }
    Func_02006098(0, s->unkA, c * 4);
    Func_0200604e(1);
    p = Func_02006074(0) + 90;
    {
        s32 w = 1;
        w |= *p;
        *p = w;
    }
    Func_02006064(20);
    k = 129;
    s->unk6C = Func_020085e4;
    Func_02006154(0, k << 1, 60);
    Func_020060fc(0, 4);
    Func_0200612c(0, 0);
    Func_0200610c(0, 4);
    s->unk6C = 0;
    Func_020060a4();
}
