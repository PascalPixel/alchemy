#include "types.h"

#define FieldScene_RunFiveCallSequence Func_02001628
#define FieldScene_PlaceAndPinSlots8To10 Func_020018f8
#define FieldScene_RunGuardedThreeStepSetup Func_020019f0
#define FieldScene_RunThreeCallSequence Func_020026e4
#define FieldScene_DrawTilesWhenCheckClear Func_02002ee4
typedef struct {
    u8 filler0[8];
    s32 unk8;
    u8 fillerC[4];
    s32 unk10;
} Struct_18f8;

void Func_020064c0(void);
void Func_020019a4(void);
void Func_020064c2(s32);
void Func_020064d6(void);
void Func_02002b32(void);
void Func_020067ac(s32);
void Func_02006798(void);
void Func_0200674c(s32, s32, s32, s32, s32, s32);
void Func_02006760(s32, s32, s32, s32, s32, s32);
Struct_18f8 *Func_020067e2(s32);
void Func_02006784(s32, s32, s32, s32, s32, s32);
void Func_0200226c(s32, s32);
void Func_020067fc(void);
void Func_02006888(void);
s32 Func_020026c0(void);
void Func_0200339c(void);
void Func_02001d78(void);
void Func_020033c8(void);
void Func_020068a8(void);
void Func_0200757c(void);
void Func_02002a60(void);
void Func_0200758c(void);
void Func_02007d7e(void);
s32 Func_02003bb6(void);
void Func_02007d3a(s32, s32, s32, s32, s32, s32);
void Func_02007d4e(s32, s32, s32, s32, s32, s32);
void Func_02003292(void);
void Func_02007dbe(void);
void Func_02005a3a(void);

void FieldScene_RunFiveCallSequence(void)
{
    Func_020064c0();
    Func_020019a4();
    Func_020064c2(20);
    Func_020064d6();
    Func_02002b32();
}

void FieldScene_PlaceAndPinSlots8To10(void)
{
    u32 i;
    Struct_18f8 *rec;
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
        rec = Func_020067e2(i + 8);
        a = rec->unk8 >> 20;
        b = rec->unk10 >> 20;
        Func_02006784(12, 50, 1, 1, a, b);
        i++;
    } while (i <= 2);
    Func_0200226c(10, 9);
    Func_020067fc();
}

void FieldScene_RunGuardedThreeStepSetup(void)
{
    Func_02006888();
    if (Func_020026c0() == 0) {
        Func_0200339c();
        Func_02001d78();
        Func_020033c8();
    }
    Func_020068a8();
}

void FieldScene_RunThreeCallSequence(void)
{
    Func_0200757c();
    Func_02002a60();
    Func_0200758c();
}

void FieldScene_DrawTilesWhenCheckClear(void)
{
    Func_02007d7e();
    if (Func_02003bb6() == 0) {
        { s32 k5 = 5, k6 = 48; Func_02007d3a(69, 48, 4, 2, k5, k6); }
        { s32 j5 = 9, j6 = 37; Func_02007d4e(73, 37, 9, 13, j5, j6); }
        Func_02003292();
    }
    Func_02007dbe();
    Func_02005a3a();
}
