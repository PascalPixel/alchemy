#include "types.h"

#define FieldScene_RunLayoutAt93By30 Func_02001374
#define FieldScene_RunLayoutAt83By45 Func_02002300
#define FieldScene_PlaceAndPinSlots10And11 Func_02002410
typedef struct Slot_02002410 {
    s32 pad0;
    s32 pad4;
    s32 column;            /* +8,  12.20 fixed point */
    s32 pad12;
    s32 row;               /* +16, 12.20 fixed point */
} Slot_02002410;

void Func_0200446e(void);
void Func_0200442a(s32, s32, s32, s32, s32, s32);
void Func_02001456(void);
void Func_020037a6(void);
void Func_02004496(void);
void Func_020053fa(void);
void Func_020053b6(s32, s32, s32, s32, s32, s32);
void Func_020023e2(void);
void Func_020043c6(void);
void Func_02005422(void);
void Func_020054c2();
void Func_0200333e();
Slot_02002410 *Func_02005540();
Slot_02002410 *Func_02005548();
void Func_020054ee();
Slot_02002410 *Func_02005564();
Slot_02002410 *Func_0200556c();
void Func_02005512();

/*
 * Layout step in resource_3c4 for slots 10 and 11. One six-argument
 * placement, then two grid-cell pins built from each slot's +8 and +16 words
 * shifted right by 20.
 *
 * Several call sites reach the same routine, but each keeps its own call
 * word; the sites must not be collapsed onto one alias.
 */

void FieldScene_RunLayoutAt93By30(void)
{
    Func_0200446e();
    {
        s32 width = 29;
        s32 height = 30;

        Func_0200442a(93, 30, 6, 5, width, height);
    }
    Func_02001456();
    Func_020037a6();
    Func_02004496();
}

void FieldScene_RunLayoutAt83By45(void)
{
    Func_020053fa();
    {
        s32 width = 19;
        s32 height = 45;

        Func_020053b6(83, 45, 11, 8, width, height);
    }
    Func_020023e2();
    Func_020043c6();
    Func_02005422();
}

void FieldScene_PlaceAndPinSlots10And11(void)
{
    s32 row;

    {
        s32 k5 = 29, k6 = 30;
        Func_020054c2(93, 30, 6, 5, k5, k6);
    }
    Func_0200333e(11, 10);

    {
        s32 col20 = Func_02005540(10)->column >> 20;
        row = Func_02005548(10)->row >> 20;
        Func_020054ee(2, 36, 1, 1, col20, row);
    }

    {
        s32 col20 = Func_02005564(11)->column >> 20;
        row = Func_0200556c(11)->row >> 20;
        Func_02005512(2, 36, 1, 1, col20, row);
    }
}
