#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

extern volatile u16 Data_03001f64;
extern volatile u32 Data_04000128;
void Func_080030f8(u32);

u32 Func_08006384(s32 arg0) {
    if ((arg0 & Data_03001f64) != arg0) {
        do {
            Func_080030f8(1);
        } while ((arg0 & Data_03001f64) != arg0);
    }
    return (Data_04000128 << 0x1A) >> 0x1E;
}
