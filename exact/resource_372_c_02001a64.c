#include "types.h"

typedef struct {
    u8 filler0[8];
    s32 unk8;
    u8 fillerC[4];
    s32 unk10;
} Thing1;

typedef struct {
    u8 filler0[10];
    s16 unkA;
    u8 fillerC[6];
    s16 unk12;
} Thing2;

extern Thing1 *Func_0200619a(s32);
extern void Func_02006208(s32, s32, s32);
extern void Func_020061c6(s32, s32, s32);
extern void Func_02006210(s32, s32, s32);
extern void Func_02006262(s32, s32, s32);
extern void Func_02006198(s32);
extern void Func_020062c2(s32, s32);
extern void Func_020061a8(s32);
extern void Func_0200628e(s32);
extern void Func_020062a6(s32, s32);
extern void Func_0200627e(s32, s32);
extern void Func_020062b6(s32, s32);
extern void Func_02006276(s32, s32);
extern Thing2 *Func_02006214(s32);
extern void Func_0200625e(s32, s32, s32);
extern void Func_02006284(s32);
extern void Func_02006296(s32, s32, s32);

void Func_02001a64(s32 x, s32 y)
{
    Thing1 *a;
    s32 w = 0x10000;
    s32 h = 0x8000;
    Thing2 *b;

    a = Func_0200619a(0);
    if (a != 0) {
        Func_02006208(22, a->unk8, a->unk10);
    }
    Func_020061c6(22, w, h);
    Func_02006210(22, x, y);
    Func_02006262(0, 22, 0);
    Func_02006198(20);
    Func_020062c2(0, 0x102);
    Func_020061a8(40);
    Func_0200628e(0xe7d);
    Func_020062a6(22, 0);
    Func_0200627e(22, 2);
    Func_020062b6(22, 0);
    Func_02006276(0, 3);
    Func_02006276(22, 2);
    b = Func_02006214(0);
    if (b != 0) {
        Func_0200625e(22, b->unkA, b->unk12);
    }
    Func_02006284(22);
    Func_02006296(22, 0, 0);
}
