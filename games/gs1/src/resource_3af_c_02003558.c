#include "types.h"

extern void Func_02007902(s32, s32);
extern void Func_02007922(s32, s32);
extern void Func_02007936(s32, s32);

extern u16 Data_0200db40[];
extern u16 Data_0200db30[];

void Func_02003558(s32 a, s32 i)
{
    u16 v = Data_0200db40[i];

    if (v >= 0x6801 && v <= 0x6FFF) {
        Data_0200db30[i] += 0x70;
        Func_02007902(a, 3);
    } else if (v >= 0xE801 && v <= 0xEFFF) {
        Data_0200db30[i] += 0xE0;
        Func_02007902(a, 3);
    } else if (v >= 0x7001 && v <= 0xEFFF) {
        Data_0200db30[i] += 0x1C0;
        Func_02007922(a, 2);
    } else {
        Data_0200db30[i] += 0x300;
        Func_02007936(a, 1);
    }
}
