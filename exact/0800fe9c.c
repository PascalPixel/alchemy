#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_08010000();
s32 Func_08010230(s32, s32);

void Func_0800fe9c(void) {
    s32 first;
    s32 third;
    s32 second;
    s32 *coordinates;

    coordinates = **(s32 ***)0x03001E70;
    first = 0;
    second = 0;
    third = 0;
    if (coordinates != NULL) {
        first = *coordinates++;
        second = *coordinates++;
        third = *coordinates;
    }
    Func_08010230(first, (s32)((u32)third - (u32)second));
    Func_08010000();
}
