#include "types.h"

#define FieldScene_DispatchByRange Func_02002508

extern void Func_020068f0();
extern void Func_0200691c();
extern u8 *Func_02006912();
extern void Func_020069c8();
extern void Func_020069e0();
extern void Func_02006a98();

void FieldScene_DispatchByRange(void)
{
    u8 *record;
    u32 biased;

    record = Func_02006912(0);
    biased = *(u16 *)(record + 6);
    Func_020068f0();

    biased = biased + 0xffff5fff;
    if (biased <= 0x3ffe) {
        Func_02006a98(13);
    } else {
        Func_020069c8(0x1a1c);
        Func_020069e0(13, 0);
    }

    Func_0200691c();
}
