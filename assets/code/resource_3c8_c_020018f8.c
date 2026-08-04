#include "types.h"

typedef struct {
    u8 filler0[8];
    s32 unk8;
    u8 fillerC[4];
    s32 unk10;
} Struct_18f8;

extern void Func_020067ac(s32);
extern void Func_02006798(void);
extern void Func_0200674c(s32, s32, s32, s32, s32, s32);
extern void Func_02006760(s32, s32, s32, s32, s32, s32);
extern Struct_18f8 *Func_020067e2(s32);
extern void Func_02006784(s32, s32, s32, s32, s32, s32);
extern void Func_0200226c(s32, s32);
extern void Func_020067fc(void);

void Func_020018f8(void)
{
    u32 i;
    Struct_18f8 *p;
    s32 x;
    s32 y;
    s32 a;
    s32 b;

    Func_020067ac(8);
    Func_02006798();
    x = 12;
    y = 44;
    Func_0200674c(19, 44, 4, 1, x, y);
    x = 11;
    y = 51;
    Func_02006760(17, 51, 2, 2, x, y);
    i = 0;
    do {
        p = Func_020067e2(i + 8);
        a = p->unk8 >> 20;
        b = p->unk10 >> 20;
        Func_02006784(12, 50, 1, 1, a, b);
        i++;
    } while (i <= 2);
    Func_0200226c(10, 9);
    Func_020067fc();
}
