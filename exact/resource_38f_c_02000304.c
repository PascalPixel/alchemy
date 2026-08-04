#include "types.h"

extern u8 Data_0200ae48[];

void Func_02002d40();
void Func_02002ede();
u8 *Func_02002db4();
void Func_02002dd8();
void Func_02002e22();
void Func_02002eee();
void Func_02002dd2();
void Func_02002d96();
void Func_02000664();
void Func_02002df0();

void Func_02000304(void)
{
    u8 *scene;
    s16 step;
    s32 a;
    s32 b;
    u8 *record;

    scene = *(u8 **)0x03001ebc;

    a = 0;
    b = 0;

    Func_02002d40();
    Func_02002ede(158);

    step = *(s16 *)(scene + 0x16c);
    switch ((s32)step) {
    case 5:
        a = 71;
        b = 9;
        break;
    case 6:
        a = 73;
        b = 17;
        break;
    case 7:
        a = 80;
        b = 21;
        break;
    case 8:
        a = 84;
        b = 12;
        break;
    case 9:
        record = Func_02002db4(0);
        record[0x55] = 0;
        Func_02002dd8(0, 0x8000, 0x4000);
        Func_02002e22(0, 0, 8);
        *(s32 *)(*(u8 **)0x03001ebc + 0x1c8) = 16;
        Func_02002eee(9);
        Func_02002dd2();
        return;
    default:
        break;
    }

    Func_02002d96(Data_0200ae48, a, b);
    Func_02000664(*(s16 *)(scene + 0x16c));
    Func_02002df0();
}
