#include "types.h"

extern volatile u16 Data_03001f64;
extern volatile u32 Data_04000128;
void Func_080030f8(u32);

u32 Func_08006384(s32 mask)
{
    if ((mask & Data_03001f64) != mask) {
        do {
            Func_080030f8(1);
        } while ((mask & Data_03001f64) != mask);
    }
    return (Data_04000128 << 0x1A) >> 0x1E;
}
